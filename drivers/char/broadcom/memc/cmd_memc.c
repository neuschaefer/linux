/************************************************************************************************/
/*                                                                                              */
/*  Copyright 2011  Broadcom Corporation                                                        */
/*                                                                                              */
/*     Unless you and Broadcom execute a separate written software license agreement governing  */
/*     use of this software, this software is licensed to you under the terms of the GNU        */
/*     General Public License version 2 (the GPL), available at                                 */
/*                                                                                              */
/*          http://www.broadcom.com/licenses/GPLv2.php                                          */
/*                                                                                              */
/*     with the following added to such license:                                                */
/*                                                                                              */
/*     As a special exception, the copyright holders of this software give you permission to    */
/*     link this software with independent modules, and to copy and distribute the resulting    */
/*     executable under terms of your choice, provided that you also meet, for each linked      */
/*     independent module, the terms and conditions of the license of that module.              */
/*     An independent module is a module which is not derived from this software.  The special  */
/*     exception does not apply to any modifications of the software.                           */
/*                                                                                              */
/*     Notwithstanding the above, under no circumstances may you combine this software in any   */
/*     way with any other Broadcom software provided under a license other than the GPL,        */
/*     without Broadcom's express prior written consent.                                        */
/*                                                                                              */
/************************************************************************************************/
#include <linux/version.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/platform_device.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/vmalloc.h>
#include <linux/semaphore.h>
#include <linux/ctype.h>
#include <linux/crc32.h>

#include <mach/rdb/brcm_rdb_sysmap.h>
#include <mach/rdb/brcm_rdb_csr.h>
#include <asm/io.h>

// 312MHz counter for demesh clock

#define NS_TO_QOS_TIMEOUT(T) ( ((((unsigned)(T))*312u+500u)/1000u) << 12 )
#define QOS_TIMEOUT_TO_NS(t) (((unsigned)(t)*1000+156)/312)

#define CAM_TAG_PORT_PRI_AID( port, port_mask, priority, priority_mask, aid, aid_mask )   \
                ( ( ((aid_mask)&0xFF) | (((priority_mask) & 3) << 10) | (((port_mask) & 3) << 13) ) << 16 ) | \
                ( ((aid)&0xFF) | (((priority) & 3) << 10) | (((port) & 3) << 13) )

#define CAM_TAG_PORT_PRI(port, port_mask, priority, priority_mask ) CAM_TAG_PORT_PRI_AID( port, port_mask, priority, priority_mask, 0, 0 /* Don't care */ )

#define CAM_TAG_PORT( port )  CAM_TAG_PORT_PRI( port, 3, 0, 0 /* Don't care */ )
#define CAM_TAG_PRI( pri, pri_mask )  CAM_TAG_PORT_PRI( 0, 0, /* Don't care */ pri, pri_mask )
#define CAM_TAG_NONE    (0x7FFF7FFFu)

#define CAM_ENTRY_PRI_LATENCY( pri, ns )    ( 0x0D000000 | NS_TO_QOS_TIMEOUT(ns) | (!!(pri) << 25) )
#define CAM_ENTRY_LATENCY( ns )   CAM_ENTRY_PRI_LATENCY( 0, ns )

typedef struct unit_info_struct
{
	const char *name;
	uint32_t base;
} UNIT_INFO_T;

static UNIT_INFO_T
units[] =
{
	{ "SYS", MEMC0_OPEN_BASE_ADDR },
	{ "VC4", MEMC1_OPEN_BASE_ADDR }
};

static const int
num_units = (sizeof(units)/sizeof(units[0]));

int do_memc_qos (char * const cmd);
int do_memc_bwc (char * const cmd);
int do_memc_spr (char * const cmd);
int do_memc_apal(char * const cmd);
int do_memc_regdump(char * const cmd);
int do_memc_crc(char * const cmd);
static int get_unit_mask(const char *str);
static int prefix_match(const char *substr, const char *str);
static unsigned long get_reg_value( unsigned long regaddr );

/*******************************************************************/
/* QOS - EMI quality-of-service settings  */
/*******************************************************************/

void check_cam_tag_entry( int entry, unsigned tag, unsigned val )
{
	if( (entry == 0) || (tag != CAM_TAG_NONE) )
	{
		if( entry == 0 )
		{
			// Entry 0 is special - the tag is ignored, and the entry is always latency
			if( (val & CSR_CAM_ENTRY_0_CAM_LATENCY_0_MASK) == 0 )
			{
				printk( "        CAM_ENTRY_0 is hardwired as latency type, but bandwidth type is requested\n" );
			}
		}
		else // Entries 1..15
		{
			if( (tag & 0x13000000) && (tag != 0x7fff0000) )
			{
				printk( "        Tag tests non-standard bits - may not match\n" );
			}

			if( ((0 ^ ((tag>>13) & 0x3)) & ((tag>>29) & 0x3)) == 0 )  // Port matches VC
			{
				unsigned pritag = (tag >> 10) & 3;
				unsigned prien = (tag >> 26) & 3;

				if( (prien == 3)       // Matching {cam_sel,urgent}
						&& (pritag == 1) // Matches urgent with cam_sel==0
				  )
				{
					printk( "        Tag matches VC4 port, tests for (urgent && !cam_sel) - may not match\n" );
				}
			}
			else  // Does not match VC
			{
				if( tag & 0x0C000000 )
				{
					printk( "        Tag does not (only) match VC4, but tests VC4-only pri bits - may not match\n" );
				}

			}
		}


		if( (val & (CSR_CAM_ENTRY_0_CAM_WRITE_ENABLE_0_MASK| CSR_CAM_ENTRY_0_CAM_READ_ENABLE_0_MASK))
				!= (CSR_CAM_ENTRY_0_CAM_WRITE_ENABLE_0_MASK| CSR_CAM_ENTRY_0_CAM_READ_ENABLE_0_MASK) )
		{
			printk( "        Entry does not have both READ_ and WRITE_ENABLE set: QoS may be disabled.\n" );
		}

		if( val & CSR_CAM_ENTRY_0_CAM_PRIORITY_0_MASK )
		{
			printk( "        Entry has obsolete PRIORITY set. This setting has no effect.\n" );
		}
	}
}

int
do_memc_qos(char * const cmd)
{
	int unitmask = (1 << num_units) - 1;
	int min_slot = 0;
	int max_slot = 15;
	int i;

	unitmask = get_unit_mask(cmd);
	if (!unitmask)
		return -1;

	for (i = 0; i < num_units; i++)
	{
		if (unitmask & (1 << i))
		{
			UNIT_INFO_T *unit = &units[i];
			int j;

			printk("%s QOS (entry: tag value):\n", unit->name);
			for (j = min_slot; j <= max_slot; j++)
			{
				unsigned int tag = (unsigned int)get_reg_value( (unsigned long)(unit->base + CSR_CAM_ENABLE_0_OFFSET + j * 4) );
				unsigned int val = (unsigned int)get_reg_value( (unsigned long)(unit->base + CSR_CAM_ENTRY_0_OFFSET + j * 4) );
				printk("    %2d: 0x%08x 0x%08x - "
					"aid/msk=%02x/%02x pri/msk=%d/%d port/msk=%d/%d -> "
					"thold=%d urg_ctr=%d(%dns) lat=%d pri=%d we=%d re=%d\n",
					j, tag, val,
					(tag>>0) & 0xff, (tag>>16) & 0xff,
					(tag>>10) & 0x3, (tag>>26) & 0x3,
					(tag>>13) & 0x3, (tag>>29) & 0x3,
					(val & CSR_CAM_ENTRY_0_CAM_THOLD_0_MASK) >> CSR_CAM_ENTRY_0_CAM_THOLD_0_SHIFT,
					(val & CSR_CAM_ENTRY_0_CAM_URG_CTR_0_MASK) >> CSR_CAM_ENTRY_0_CAM_URG_CTR_0_SHIFT,
					QOS_TIMEOUT_TO_NS((val & CSR_CAM_ENTRY_0_CAM_URG_CTR_0_MASK) >> CSR_CAM_ENTRY_0_CAM_URG_CTR_0_SHIFT),
					(val & CSR_CAM_ENTRY_0_CAM_LATENCY_0_MASK) >> CSR_CAM_ENTRY_0_CAM_LATENCY_0_SHIFT,
					(val & CSR_CAM_ENTRY_0_CAM_PRIORITY_0_MASK) >> CSR_CAM_ENTRY_0_CAM_PRIORITY_0_SHIFT,
					(val & CSR_CAM_ENTRY_0_CAM_WRITE_ENABLE_0_MASK) >> CSR_CAM_ENTRY_0_CAM_WRITE_ENABLE_0_SHIFT,
					(val & CSR_CAM_ENTRY_0_CAM_READ_ENABLE_0_MASK) >> CSR_CAM_ENTRY_0_CAM_READ_ENABLE_0_SHIFT
					);

				check_cam_tag_entry( j, tag, val );
			}
		}
	}

	return 0;
}

/*******************************************************************/
/* BWC - EMI bandwidth control  */
/*******************************************************************/

int
do_memc_bwc(char * const cmd)
{
	int unitmask = (1 << num_units) - 1;
	int i;

	unitmask = get_unit_mask(cmd);
	if (!unitmask)
		return -1;

	for (i = 0; i < num_units; i++)
	{
		if (unitmask & (1 << i))
		{
			UNIT_INFO_T *unit = &units[i];
			unsigned long regval;
			regval = get_reg_value( (unsigned long)(unit->base + CSR_SEQ_RDWR_BANDWIDTH_CONTROL_OFFSET) );
			printk("%s SEQ_RDWR_BANDWIDTH_CONTROL: 0x%03lx\n", unit->name, regval);
			printk("        q_depth            = %ld\n",
				(regval & CSR_SEQ_RDWR_BANDWIDTH_CONTROL_HIT_Q_DEPTH_MASK) >>
				CSR_SEQ_RDWR_BANDWIDTH_CONTROL_HIT_Q_DEPTH_SHIFT);
			printk("        rd_transaction_cnt = %ld\n",
				(regval & CSR_SEQ_RDWR_BANDWIDTH_CONTROL_RD_TRANSACTION_CNT_MASK) >>
				CSR_SEQ_RDWR_BANDWIDTH_CONTROL_RD_TRANSACTION_CNT_SHIFT);
			printk("        wr_transaction_cnt = %ld\n",
				(regval & CSR_SEQ_RDWR_BANDWIDTH_CONTROL_WR_TRANSACTION_CNT_MASK) >>
				CSR_SEQ_RDWR_BANDWIDTH_CONTROL_WR_TRANSACTION_CNT_SHIFT);
		}
	}

	return 0;
}

/*******************************************************************/
/* SPR - Extra configuration settings  */
/*******************************************************************/

int
do_memc_spr(char * const cmd)
{
	int unitmask = (1 << num_units) - 1;
	int i;

	unitmask = get_unit_mask(cmd);
	if (!unitmask)
		return -1;

	for (i = 0; i < num_units; i++)
	{
		if (unitmask & (1 << i))
		{
			UNIT_INFO_T *unit = &units[i];
			unsigned long regval;
			regval = get_reg_value( (unsigned long)(unit->base + CSR_CORE_SPR_RW_OFFSET) );
			printk("%s CORE_SPR_RW: 0x%08lx\n", unit->name, regval);
		}
	}

	return 0;
}

/*******************************************************************/
/* APAL - AXI Port Access Limit settings  */
/*******************************************************************/

int
do_memc_apal(char * const cmd)
{
	int unitmask = (1 << num_units) - 1;
	int i;

	unitmask = get_unit_mask(cmd);
	if (!unitmask)
		return -1;

	for (i = 0; i < num_units; i++)
	{
		if (unitmask & (1 << i))
		{
			UNIT_INFO_T *unit = &units[i];
			unsigned long regval;
			regval = get_reg_value( (unsigned long)(unit->base + CSR_AXI_PORT_ACCESS_LIMIT_OFFSET) );
			printk("%s AXI_PORT_ACCESS_LIMIT: 0x%08lx\n", unit->name, regval);
			printk("        vc4      = %ld slots\n",
				(regval & CSR_AXI_PORT_ACCESS_LIMIT_PORT0_ACCESS_LIMIT_MASK) >>
				CSR_AXI_PORT_ACCESS_LIMIT_PORT0_ACCESS_LIMIT_SHIFT);
			printk("        arm      = %ld slots\n",
				(regval & CSR_AXI_PORT_ACCESS_LIMIT_PORT1_ACCESS_LIMIT_MASK) >>
				CSR_AXI_PORT_ACCESS_LIMIT_PORT1_ACCESS_LIMIT_SHIFT);
			printk("        periphs  = %ld slots\n",
				(regval & CSR_AXI_PORT_ACCESS_LIMIT_PORT2_ACCESS_LIMIT_MASK) >>
				CSR_AXI_PORT_ACCESS_LIMIT_PORT2_ACCESS_LIMIT_SHIFT);
			printk("        modem    = %ld slots\n",
				(regval & CSR_AXI_PORT_ACCESS_LIMIT_PORT3_ACCESS_LIMIT_MASK) >>
				CSR_AXI_PORT_ACCESS_LIMIT_PORT3_ACCESS_LIMIT_SHIFT);
			printk("        override = %ld slots\n",
				(regval & CSR_AXI_PORT_ACCESS_LIMIT_ACCESS_LIMIT_OVERRIDE_MASK) >>
				CSR_AXI_PORT_ACCESS_LIMIT_ACCESS_LIMIT_OVERRIDE_SHIFT);
		}
	}

	return 0;
}

/*******************************************************************/
/* REGDUMP - Dumps all set registers for each unit, this dump is   */
/*           raw format on purpose such that the information can   */
/*           be used directly for other use, as example dt-blob    */
/*           auto-configuration.                                   */
/*******************************************************************/
int
do_memc_regdump(char * const cmd)
{
	int unitmask = (1 << num_units) - 1;
	int min_slot = 0;
	int max_slot = 15;
	int i;

	unitmask = get_unit_mask(cmd);
	if (!unitmask)
		return -1;

	for (i = 0; i < num_units; i++)
	{
		if (unitmask & (1 << i))
		{
		   UNIT_INFO_T *unit = &units[i];
		   unsigned long regval;
			int j;

         printk("\n");

         /* APAL register */
         regval = get_reg_value( unit->base + CSR_AXI_PORT_ACCESS_LIMIT_OFFSET );
         printk("0x%08lx /* 0x%04x_%04x : %s AXI_PORT_ACCESS_LIMIT --> number of slots - vc4: %ld - arm: %ld - periphs: %ld - modem: %ld - override: %ld */\n",
                regval,
                (unit->base + CSR_AXI_PORT_ACCESS_LIMIT_OFFSET) >> 16,
                (unit->base + CSR_AXI_PORT_ACCESS_LIMIT_OFFSET) & 0xFFFF,
                unit->name,
                (regval & CSR_AXI_PORT_ACCESS_LIMIT_PORT0_ACCESS_LIMIT_MASK) >> CSR_AXI_PORT_ACCESS_LIMIT_PORT0_ACCESS_LIMIT_SHIFT,
                (regval & CSR_AXI_PORT_ACCESS_LIMIT_PORT1_ACCESS_LIMIT_MASK) >> CSR_AXI_PORT_ACCESS_LIMIT_PORT1_ACCESS_LIMIT_SHIFT,
                (regval & CSR_AXI_PORT_ACCESS_LIMIT_PORT2_ACCESS_LIMIT_MASK) >> CSR_AXI_PORT_ACCESS_LIMIT_PORT2_ACCESS_LIMIT_SHIFT,
                (regval & CSR_AXI_PORT_ACCESS_LIMIT_PORT3_ACCESS_LIMIT_MASK) >> CSR_AXI_PORT_ACCESS_LIMIT_PORT3_ACCESS_LIMIT_SHIFT,
                (regval & CSR_AXI_PORT_ACCESS_LIMIT_ACCESS_LIMIT_OVERRIDE_MASK) >> CSR_AXI_PORT_ACCESS_LIMIT_ACCESS_LIMIT_OVERRIDE_SHIFT );

         printk("\n");

         /* SPR register */
         regval = get_reg_value( unit->base + CSR_CORE_SPR_RW_OFFSET );
         printk("0x%08lx /* 0x%04x_%04x : %s CORE_SPR_RW */\n",
                regval,
                (unit->base + CSR_CORE_SPR_RW_OFFSET) >> 16,
                (unit->base + CSR_CORE_SPR_RW_OFFSET) & 0xFFFF,
                unit->name );

         printk("\n");

         /* BWC register */
         regval = get_reg_value( unit->base + CSR_SEQ_RDWR_BANDWIDTH_CONTROL_OFFSET );
			printk("0x%08lx /* 0x%04x_%04x : %s SEQ_RDWR_BANDWIDTH_CONTROL --> q_depth: %ld - rd_transaction_cnt: %ld - wr_transaction_cnt: %ld */\n",
                regval,
                (unit->base + CSR_SEQ_RDWR_BANDWIDTH_CONTROL_OFFSET) >> 16,
                (unit->base + CSR_SEQ_RDWR_BANDWIDTH_CONTROL_OFFSET) & 0xFFFF,
                unit->name,
                (regval & CSR_SEQ_RDWR_BANDWIDTH_CONTROL_HIT_Q_DEPTH_MASK) >> CSR_SEQ_RDWR_BANDWIDTH_CONTROL_HIT_Q_DEPTH_SHIFT,
                (regval & CSR_SEQ_RDWR_BANDWIDTH_CONTROL_RD_TRANSACTION_CNT_MASK) >> CSR_SEQ_RDWR_BANDWIDTH_CONTROL_RD_TRANSACTION_CNT_SHIFT,
                (regval & CSR_SEQ_RDWR_BANDWIDTH_CONTROL_WR_TRANSACTION_CNT_MASK) >> CSR_SEQ_RDWR_BANDWIDTH_CONTROL_WR_TRANSACTION_CNT_SHIFT );

         printk("\n");

         /* CAM entry */
			for (j = min_slot; j <= max_slot; j++)
			{
            regval = get_reg_value( unit->base + CSR_CAM_ENTRY_0_OFFSET + j * 4 );
            printk("0x%08lx /* 0x%04x_%04x : %s CAM_ENTRY_%d --> thold: %d - urg_ctr: %d(%dns) - lat: %d - pri: %d - we: %d - re: %d */\n",
                   regval,
                   (unit->base + CSR_CAM_ENTRY_0_OFFSET + j * 4) >> 16,
                   (unit->base + CSR_CAM_ENTRY_0_OFFSET + j * 4) & 0xFFFF,
                   unit->name,
                   j,
                   (int)((regval & CSR_CAM_ENTRY_0_CAM_THOLD_0_MASK) >> CSR_CAM_ENTRY_0_CAM_THOLD_0_SHIFT),
                   (int)((regval & CSR_CAM_ENTRY_0_CAM_URG_CTR_0_MASK) >> CSR_CAM_ENTRY_0_CAM_URG_CTR_0_SHIFT),
                   (int)(QOS_TIMEOUT_TO_NS((regval & CSR_CAM_ENTRY_0_CAM_URG_CTR_0_MASK) >> CSR_CAM_ENTRY_0_CAM_URG_CTR_0_SHIFT)),
                   (int)((regval & CSR_CAM_ENTRY_0_CAM_LATENCY_0_MASK) >> CSR_CAM_ENTRY_0_CAM_LATENCY_0_SHIFT),
                   (int)((regval & CSR_CAM_ENTRY_0_CAM_PRIORITY_0_MASK) >> CSR_CAM_ENTRY_0_CAM_PRIORITY_0_SHIFT),
                   (int)((regval & CSR_CAM_ENTRY_0_CAM_WRITE_ENABLE_0_MASK) >> CSR_CAM_ENTRY_0_CAM_WRITE_ENABLE_0_SHIFT),
                   (int)((regval & CSR_CAM_ENTRY_0_CAM_READ_ENABLE_0_MASK) >> CSR_CAM_ENTRY_0_CAM_READ_ENABLE_0_SHIFT) );
         }

         printk("\n");

         /* CAM enable */
         for (j = min_slot; j <= max_slot; j++)
			{
            regval = get_reg_value( unit->base + CSR_CAM_ENABLE_0_OFFSET + j * 4 );
            printk("0x%08lx /* 0x%04x_%04x : %s CAM_ENABLE_%d --> aid/msk: %02x/%02x - pri/msk: %d/%d - port/msk: %d/%d */\n",
                   regval,
                   (unit->base + CSR_CAM_ENABLE_0_OFFSET + j * 4) >> 16,
                   (unit->base + CSR_CAM_ENABLE_0_OFFSET + j * 4) & 0xFFFF,
                   unit->name,
                   j,
                   (unsigned int)((regval>>0) & 0xff), (unsigned int)((regval>>16) & 0xff),
                   (int)((regval>>10) & 0x3), (int)((regval>>26) & 0x3),
                   (int)((regval>>13) & 0x3), (int)((regval>>29) & 0x3) );
         }

         printk("\n");
      }
   }

   return 0;
}

/*******************************************************************/
/* CRC - Dumps CRC of all registers selected.                      */
/*******************************************************************/
int
do_memc_crc(char * const cmd)
{
	int unitmask = (1 << num_units) - 1;
	int min_slot = 0;
	int max_slot = 15;
	int i;
   unsigned long crc = 0;
   unsigned long regval;

	unitmask = get_unit_mask(cmd);
	if (!unitmask)
		return -1;

	for (i = 0; i < num_units; i++)
	{
		if (unitmask & (1 << i))
		{
		   UNIT_INFO_T *unit = &units[i];
			int j;

         /* APAL register */
         regval = get_reg_value( (unsigned long)(unit->base + CSR_AXI_PORT_ACCESS_LIMIT_OFFSET) );
         crc = crc32( crc, &regval, sizeof( unsigned long ) );

         /* SPR register */
         regval = get_reg_value( (unsigned long)(unit->base + CSR_CORE_SPR_RW_OFFSET) );
         crc = crc32( crc, &regval, sizeof( unsigned long ) );

         /* BWC register */
         regval = get_reg_value( (unsigned long)(unit->base + CSR_SEQ_RDWR_BANDWIDTH_CONTROL_OFFSET) );
         crc = crc32( crc, &regval, sizeof( unsigned long ) );

         /* QOS registers [CAM entry and CAM enable] */
			for (j = min_slot; j <= max_slot; j++)
			{
            regval = get_reg_value( (unsigned long)(unit->base + CSR_CAM_ENTRY_0_OFFSET + j * 4) );
            crc = crc32( crc, &regval, sizeof( unsigned long ) );

            regval = get_reg_value( (unsigned long)(unit->base + CSR_CAM_ENABLE_0_OFFSET + j * 4) );
            crc = crc32( crc, &regval, sizeof( unsigned long ) );
         }
      }
   }

   printk("Memory Controller (unit: %s) -> CRC: 0x%08lx\n\n", cmd, crc );
   return 0;
}


static int
get_unit_mask(const char *str)
{
	int unitmask = 0;

	if (strcmp(str, "*") == 0)
		unitmask = (1 << num_units) - 1;
	else
	{
		int i;
		for (i = 0; i < num_units; i++)
		{
			if (prefix_match(str, units[i].name))
			{
				unitmask = (1 << i);
				break;
			}
		}
	}

	return unitmask;
}

static int
prefix_match(const char *substr, const char *str)
{
	int i;
	for (i = 0; substr[i]; i++)
	{
		if ((str[i] == 0) || (tolower(substr[i]) != tolower(str[i])))
		{
			return 0;
		}
	}
	return 1;
}

static unsigned long
get_reg_value( unsigned long regaddr )
{
   unsigned long regval = 0;
   unsigned long address;

   address = (unsigned long) ioremap_nocache( regaddr, sizeof( unsigned long ) );
   regval = readl( address );
	iounmap( (void *)address );

   return regval;
}
