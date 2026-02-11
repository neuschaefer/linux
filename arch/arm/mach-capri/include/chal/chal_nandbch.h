/*****************************************************************************
* Copyright 2004 - 2011 Broadcom Corporation.  All rights reserved.
*
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed to you
* under the terms of the GNU General Public License version 2, available at
* http://www.broadcom.com/licenses/GPLv2.php (the "GPL").
*
* Notwithstanding the above, under no circumstances may you combine this
* software in any way with any other Broadcom software provided under a
* license other than the GPL, without Broadcom's express prior written
* consent.
*****************************************************************************/
/**
 * @file    chal_nandbch.h
 * @brief   CHAL NANDBCH header.
 * @note
 **/


#ifndef __CHAL_NANDBCH_H__
#define __CHAL_NANDBCH_H__

#include <mach/rdb/brcm_rdb_nand.h>
#include <mach/io_map.h>

#define CHAL_REG_WRITE32(x,y) iowrite32(y,x)
#define CHAL_REG_READ32(x) ioread32(x)

#define MM_IO_BASE_NAND KONA_NAND_VA


/* DMA buffers and descriptor aligment */
#define NAND_DMA_ALIGN           4

/* NAND controller register definitions */

#define NAND_REG_COMMAND         (MM_IO_BASE_NAND + NAND_COMMAND_OFFSET)
#define NAND_REG_ADDRESS	      (MM_IO_BASE_NAND + NAND_ADDRESS_OFFSET)
#define NAND_REG_ATTR0		      (MM_IO_BASE_NAND + NAND_ATTRI0_OFFSET)
#define NAND_REG_ATTR1		      (MM_IO_BASE_NAND + NAND_ATTRI1_OFFSET)
#define NAND_REG_BANK		      (MM_IO_BASE_NAND + NAND_BANK_OFFSET)
#define NAND_REG_CONTROL	      (MM_IO_BASE_NAND + NAND_CONTROL_OFFSET)
#define NAND_REG_CONFIG0	      (MM_IO_BASE_NAND + NAND_CONFIG0_OFFSET)
#define NAND_REG_CONFIG1	      (MM_IO_BASE_NAND + NAND_CONFIG1_OFFSET)
#define NAND_REG_STATUS 	      (MM_IO_BASE_NAND + NAND_STATUS_OFFSET)

#define NAND_REG_IRQCTRL	      (MM_IO_BASE_NAND + NAND_IRQCTRL_OFFSET)
#define NAND_REG_IRQSTATUS	      (MM_IO_BASE_NAND + NAND_IRQSTATUS_OFFSET)
#define NAND_REG_PRDBASE         (MM_IO_BASE_NAND + NAND_PRDBASE_OFFSET)

#define NAND_REG_ERRLOG0	      (MM_IO_BASE_NAND + NAND_ERRLOG0_OFFSET)
#define NAND_REG_ERRLOG1	      (MM_IO_BASE_NAND + NAND_ERRLOG1_OFFSET)
#define NAND_REG_ERRLOG2	      (MM_IO_BASE_NAND + NAND_ERRLOG2_OFFSET)
#define NAND_REG_ERRLOG3	      (MM_IO_BASE_NAND + NAND_ERRLOG3_OFFSET)
#define NAND_REG_ERRLOG4	      (MM_IO_BASE_NAND + NAND_ERRLOG4_OFFSET)
#define NAND_REG_ERRLOG5	      (MM_IO_BASE_NAND + NAND_ERRLOG5_OFFSET)
#define NAND_REG_ERRLOG6	      (MM_IO_BASE_NAND + NAND_ERRLOG6_OFFSET)
#define NAND_REG_ERRLOG7	      (MM_IO_BASE_NAND + NAND_ERRLOG7_OFFSET)

#define NAND_REG_CONFIG2		   (MM_IO_BASE_NAND + NAND_CONFIG2_OFFSET)

#define NAND_REG_DMAINT0	      (MM_IO_BASE_NAND + NAND_DMAINT0_OFFSET)
#define NAND_REG_DMAINT1	      (MM_IO_BASE_NAND + NAND_DMAINT1_OFFSET)
#define NAND_REG_DMAINT2	      (MM_IO_BASE_NAND + NAND_DMAINT2_OFFSET)
#define NAND_REG_DMAINT3	      (MM_IO_BASE_NAND + NAND_DMAINT3_OFFSET)

#define NAND_REG_ECCSTATIS0      (MM_IO_BASE_NAND + NAND_ECCSTATIS0_OFFSET)
#define NAND_REG_ECCSTATIS1      (MM_IO_BASE_NAND + NAND_ECCSTATIS1_OFFSET)
#define NAND_REG_ECCSTATIS2      (MM_IO_BASE_NAND + NAND_ECCSTATIS2_OFFSET)
#define NAND_REG_ECCSTATIS3      (MM_IO_BASE_NAND + NAND_ECCSTATIS3_OFFSET)
#define NAND_REG_ECCSTATIS4      (MM_IO_BASE_NAND + NAND_ECCSTATIS4_OFFSET)
#define NAND_REG_ECCSTATIS5      (MM_IO_BASE_NAND + NAND_ECCSTATIS5_OFFSET)
#define NAND_REG_ECCSTATIS6      (MM_IO_BASE_NAND + NAND_ECCSTATIS6_OFFSET)
#define NAND_REG_ECCSTATIS7      (MM_IO_BASE_NAND + NAND_ECCSTATIS7_OFFSET)

#define NAND_REG_MINSTR	         (MM_IO_BASE_NAND + NAND_MINSTR_OFFSET)
#define NAND_REG_MADDR0	         (MM_IO_BASE_NAND + NAND_MADDR0_OFFSET)
#define NAND_REG_MADDR1	         (MM_IO_BASE_NAND + NAND_MADDR1_OFFSET)
#define NAND_REG_MRESP		      (MM_IO_BASE_NAND + NAND_MRESP_OFFSET)
#define NAND_REG_B0_R1	         (MM_IO_BASE_NAND + NAND_R1_OFFSET)
#define NAND_REG_B1_R1	         (MM_IO_BASE_NAND + NAND_B1_OFFSET)
#define NAND_REG_B2_R1	         (MM_IO_BASE_NAND + NAND_B2_OFFSET)
#define NAND_REG_B3_R1	         (MM_IO_BASE_NAND + NAND_B3_OFFSET)
#define NAND_REG_B4_R1	         (MM_IO_BASE_NAND + NAND_B4_OFFSET)
#define NAND_REG_B5_R1	         (MM_IO_BASE_NAND + NAND_B5_OFFSET)
#define NAND_REG_B6_R1	         (MM_IO_BASE_NAND + NAND_B6_OFFSET)
#define NAND_REG_B7_R1	         (MM_IO_BASE_NAND + NAND_B7_OFFSET)

#define NAND_REG_RDFIFO 	      (MM_IO_BASE_NAND + NAND_RDFIFO_OFFSET)
#define NAND_REG_WRFIFO 	      (MM_IO_BASE_NAND + NAND_WRFIFO_OFFSET)

#define NAND_REG_BCH_CTRL 	      (MM_IO_BASE_NAND + NAND_BCH_CTRL_OFFSET)
#define NAND_REG_BCH_STATUS      (MM_IO_BASE_NAND + NAND_BCH_STATUS_OFFSET)
#define NAND_REG_BCH_NK 	      (MM_IO_BASE_NAND + NAND_BCH_NK_OFFSET)
#define NAND_REG_BCH_T 	         (MM_IO_BASE_NAND + NAND_BCH_T_OFFSET)
#define NAND_REG_BCH_WR_ECC0 	   (MM_IO_BASE_NAND + NAND_BCH_WR_ECC0_OFFSET)
#define NAND_REG_BCH_WR_ECC1 	   (MM_IO_BASE_NAND + NAND_BCH_WR_ECC1_OFFSET)
#define NAND_REG_BCH_WR_ECC2 	   (MM_IO_BASE_NAND + NAND_BCH_WR_ECC2_OFFSET)
#define NAND_REG_BCH_WR_ECC3 	   (MM_IO_BASE_NAND + NAND_BCH_WR_ECC3_OFFSET)
#define NAND_REG_BCH_WR_ECC4 	   (MM_IO_BASE_NAND + NAND_BCH_WR_ECC4_OFFSET)
#define NAND_REG_BCH_WR_ECC5 	   (MM_IO_BASE_NAND + NAND_BCH_WR_ECC5_OFFSET)
#define NAND_REG_BCH_WR_ECC6 	   (MM_IO_BASE_NAND + NAND_BCH_WR_ECC6_OFFSET)
#define NAND_REG_BCH_WR_ECC7 	   (MM_IO_BASE_NAND + NAND_BCH_WR_ECC7_OFFSET)
#define NAND_REG_BCH_WR_ECC8 	   (MM_IO_BASE_NAND + NAND_BCH_WR_ECC8_OFFSET)
#define NAND_REG_BCH_WR_ECC9 	   (MM_IO_BASE_NAND + NAND_BCH_WR_ECC9_OFFSET)
#define NAND_REG_BCH_WR_ECC10    (MM_IO_BASE_NAND + NAND_BCH_WR_ECC10_OFFSET)
#define NAND_REG_BCH_WR_ECC11    (MM_IO_BASE_NAND + NAND_BCH_WR_ECC11_OFFSET)
#define NAND_REG_BCH_WR_ECC12    (MM_IO_BASE_NAND + NAND_BCH_WR_ECC12_OFFSET)
#define NAND_REG_BCH_WR_ECC13    (MM_IO_BASE_NAND + NAND_BCH_WR_ECC13_OFFSET)
#define NAND_REG_BCH_WR_ECC14    (MM_IO_BASE_NAND + NAND_BCH_WR_ECC14_OFFSET)
#define NAND_REG_BCH_WR_ECC15    (MM_IO_BASE_NAND + NAND_BCH_WR_ECC15_OFFSET)
#define NAND_REG_BCH_WR_ECC16    (MM_IO_BASE_NAND + NAND_BCH_WR_ECC16_OFFSET)
#define NAND_REG_BCH_WR_ECC17    (MM_IO_BASE_NAND + NAND_BCH_WR_ECC17_OFFSET)
#define NAND_REG_BCH_ERR_LOC 	   (MM_IO_BASE_NAND + NAND_BCH_ERR_LOC_OFFSET)

#define NAND_REG_UCODE_START	   (MM_IO_BASE_NAND + 0xF000)
#define NAND_REG_UCODE_END	      (MM_IO_BASE_NAND + 0x10000)



/* Control bits definitions */
#define NAND_CONTROL_RST			         NAND_CONTROL_RESETALL_MASK
#define NAND_CONTROL_DMA_RST		         NAND_CONTROL_DMA_RESET_MASK
#define NAND_CONTROL_ECC_RST		         NAND_CONTROL_ECC_RST_MASK
#define NAND_CONTROL_WP		               NAND_CONTROL_WP_MASK
#define NAND_CONTROL_STOP			         NAND_CONTROL_STARTSTOP_MASK
#define NAND_CONTROL_DMA_START		      NAND_CONTROL_DMA_STARTSTOP_MASK
#define NAND_CONTROL_ECC_BYPASS		      NAND_CONTROL_ECC_BYP_MASK
#define NAND_CONTROL_ECC_HM_DISABLE       NAND_CONTROL_ECC_HM_MASK
#define NAND_CONTROL_ECC_RS_DISABLE       NAND_CONTROL_ECC_RS_MASK
#define NAND_CONTROL_2NAND			         NAND_CONTROL_TRAN_DIR_MASK
#define NAND_CONTROL_DATA_TEST            NAND_CONTROL_TEST_MODE_MASK
#define NAND_CONTROL_DMA_MODE             NAND_CONTROL_DMA_MODE_MASK
#define NAND_CONTROL_OP_MODE_TEST         NAND_CONTROL_OP_MODE_MASK


#define NAND_CONTROL_NORMAL_CONFIG     (  NAND_CONTROL_DMA_MODE | \
                                          NAND_CONTROL_ECC_BYPASS | \
                                          NAND_CONTROL_ECC_HM_DISABLE | \
                                          NAND_CONTROL_ECC_RS_DISABLE )


/* Auxiliary data type */
#define NAND_CONFIG0_AUX_DATA_TYPE_2B  (0x0)
#define NAND_CONFIG0_AUX_DATA_TYPE_4B  (0x1)
#define NAND_CONFIG0_AUX_DATA_TYPE_8B  (0x2)

/* Data bus width */
#define NAND_CONFIG0_DATA_WIDTH_8	   (0x0)
#define NAND_CONFIG0_DATA_WIDTH_16	   (0x1)

/* Transfer direction */
#define NAND_DIRECTION_FROM_NAND       (0x0)
#define NAND_DIRECTION_TO_NAND         (0x1)

/* Timing configuration expressed in 5ns steps (actual 4.8ns) */
#define NAND_CONFIG_TIMING_STEP	      (0x5)


/* CONFIG0_NORMAL  0x00000000
 * Aux_data_type:    0x0      (2 bytes)
 * Data_width:       0x0      (8 bit)
 */
#define NAND_CONFIG0_NORMAL   (((NAND_CONFIG0_AUX_DATA_TYPE_2B << NAND_CONFIG0_AUX_DATA_TYPE_SHIFT) & NAND_CONFIG0_AUX_DATA_TYPE_MASK) \
                               | ((NAND_CONFIG0_DATA_WIDTH_8 << NAND_CONFIG0_DATA_WIDTH_SHIFT) & NAND_CONFIG0_DATA_WIDTH_MASK))


/*
 * Timing Mode 0 config1:  0x005F9C9C
 * Hold time:		         24ns
 * Setup time:		         72ns
 * Write high time:	      48ns
 * Write pulse width:	   62ns
 * Read high time:		   48ns
 * Read pulse width:	      62ns
 */
#define NAND_CONFIG1_TMODE0   (((5 << NAND_CONFIG1_HOLD_TH_SHIFT) & NAND_CONFIG1_HOLD_TH_MASK)  \
                               | ((15 << NAND_CONFIG1_SETUP_TS_SHIFT) & NAND_CONFIG1_SETUP_TS_MASK)     \
                               | ((9 << NAND_CONFIG1_WR_CYCLE_TWH_SHIFT) & NAND_CONFIG1_WR_CYCLE_TWH_MASK)   \
                               | ((12 << NAND_CONFIG1_WR_PULSE_WIDTH_TWP_SHIFT) & NAND_CONFIG1_WR_PULSE_WIDTH_TWP_MASK)   \
                               | ((9 << NAND_CONFIG1_RD_CYCLE_TREH_SHIFT) & NAND_CONFIG1_RD_CYCLE_TREH_MASK) \
                               | ((12 << NAND_CONFIG1_RD_PULSE_WIDTH_TRP_SHIFT) & NAND_CONFIG1_RD_PULSE_WIDTH_TRP_MASK))

/*
 * Timing Mode 0 config2:                       0x0000FF00
 * RE high to out impedance time:		         72ns
 * difference between CE and RE access time:    72ns
 * Output enable delay tOE:                     4ns
 * (Time adjustment for output enable to control the sampling time)
*/
#define NAND_CONFIG2_TMODE0   (((15 << NAND_CONFIG2_TRHZ_SHIFT) & NAND_CONFIG2_TRHZ_MASK)  \
                               | ((15 << NAND_CONFIG2_TCEA_TREA_SHIFT) & NAND_CONFIG2_TCEA_TREA_MASK) \
                               | ((0 << NAND_CONFIG2_TOE_SHIFT) & NAND_CONFIG2_TOE_MASK))


/*
 * Timing Mode 1 config1:  0x00383535
 * Hold time:		         14ns
 * Setup time:		         38ns
 * Write high time:	      19ns
 * Write pulse width:	   28ns
 * Read high time:		   19ns
 * Read pulse width:	      28ns
 */
#define NAND_CONFIG1_TMODE1   (((3 << NAND_CONFIG1_HOLD_TH_SHIFT) & NAND_CONFIG1_HOLD_TH_MASK)  \
                               | ((8 << NAND_CONFIG1_SETUP_TS_SHIFT) & NAND_CONFIG1_SETUP_TS_MASK)     \
                               | ((3 << NAND_CONFIG1_WR_CYCLE_TWH_SHIFT) & NAND_CONFIG1_WR_CYCLE_TWH_MASK)   \
                               | ((5 << NAND_CONFIG1_WR_PULSE_WIDTH_TWP_SHIFT) & NAND_CONFIG1_WR_PULSE_WIDTH_TWP_MASK)   \
                               | ((3 << NAND_CONFIG1_RD_CYCLE_TREH_SHIFT) & NAND_CONFIG1_RD_CYCLE_TREH_MASK) \
                               | ((5 << NAND_CONFIG1_RD_PULSE_WIDTH_TRP_SHIFT) & NAND_CONFIG1_RD_PULSE_WIDTH_TRP_MASK))

/*
 * Timing Mode 1 config2:                       0x0000F800
 * RE high to out impedance time:		         72ns
 * difference between CE and RE access time:    38ns
 * Output enable delay tOE:                     4ns
 * (Time adjustment for output enable to control the sampling time)
*/
#define NAND_CONFIG2_TMODE1   (((15 << NAND_CONFIG2_TRHZ_SHIFT) & NAND_CONFIG2_TRHZ_MASK)  \
                               | ((8 << NAND_CONFIG2_TCEA_TREA_SHIFT) & NAND_CONFIG2_TCEA_TREA_MASK) \
                               | ((0 << NAND_CONFIG2_TOE_SHIFT) & NAND_CONFIG2_TOE_MASK))


/*
 * Timing Mode 2 config1:  0x00363434
 * Hold time:		         14ns
 * Setup time:		         28ns
 * Write high time:	      19ns
 * Write pulse width:	   24ns
 * Read high time:		   19ns
 * Read pulse width:	      24ns
 */
#define NAND_CONFIG1_TMODE2   (((3 << NAND_CONFIG1_HOLD_TH_SHIFT) & NAND_CONFIG1_HOLD_TH_MASK)  \
                               | ((6 << NAND_CONFIG1_SETUP_TS_SHIFT) & NAND_CONFIG1_SETUP_TS_MASK)     \
                               | ((3 << NAND_CONFIG1_WR_CYCLE_TWH_SHIFT) & NAND_CONFIG1_WR_CYCLE_TWH_MASK)   \
                               | ((4 << NAND_CONFIG1_WR_PULSE_WIDTH_TWP_SHIFT) & NAND_CONFIG1_WR_PULSE_WIDTH_TWP_MASK)   \
                               | ((3 << NAND_CONFIG1_RD_CYCLE_TREH_SHIFT) & NAND_CONFIG1_RD_CYCLE_TREH_MASK) \
                               | ((4 << NAND_CONFIG1_RD_PULSE_WIDTH_TRP_SHIFT) & NAND_CONFIG1_RD_PULSE_WIDTH_TRP_MASK))

/*
 * Timing Mode 2 config2:                       0x0000F600
 * RE high to out impedance time:		         72ns
 * difference between CE and RE access time:    28ns
 * Output enable delay tOE:                     4ns
 * (Time adjustment for output enable to control the sampling time)
*/
#define NAND_CONFIG2_TMODE2   (((15 << NAND_CONFIG2_TRHZ_SHIFT) & NAND_CONFIG2_TRHZ_MASK)  \
                               | ((6 << NAND_CONFIG2_TCEA_TREA_SHIFT) & NAND_CONFIG2_TCEA_TREA_MASK) \
                               | ((0 << NAND_CONFIG2_TOE_SHIFT) & NAND_CONFIG2_TOE_MASK))


/*
 * Timing Mode 3 config1:  0x00262323
 * Hold time:		         9ns
 * Setup time:		         28ns
 * Write high time:	      14ns
 * Write pulse width:	   19ns
 * Read high time:		   14ns
 * Read pulse width:	      19ns
 */
#define NAND_CONFIG1_TMODE3   (((2 << NAND_CONFIG1_HOLD_TH_SHIFT) & NAND_CONFIG1_HOLD_TH_MASK)  \
                               | ((6 << NAND_CONFIG1_SETUP_TS_SHIFT) & NAND_CONFIG1_SETUP_TS_MASK)     \
                               | ((2 << NAND_CONFIG1_WR_CYCLE_TWH_SHIFT) & NAND_CONFIG1_WR_CYCLE_TWH_MASK)   \
                               | ((3 << NAND_CONFIG1_WR_PULSE_WIDTH_TWP_SHIFT) & NAND_CONFIG1_WR_PULSE_WIDTH_TWP_MASK)   \
                               | ((2 << NAND_CONFIG1_RD_CYCLE_TREH_SHIFT) & NAND_CONFIG1_RD_CYCLE_TREH_MASK) \
                               | ((3 << NAND_CONFIG1_RD_PULSE_WIDTH_TRP_SHIFT) & NAND_CONFIG1_RD_PULSE_WIDTH_TRP_MASK))

/*
 * Timing Mode 3 config2:                       0x0000F600
 * RE high to out impedance time:		         72ns
 * difference between CE and RE access time:    28ns
 * Output enable delay tOE:                     4ns
 * (Time adjustment for output enable to control the sampling time)
*/
#define NAND_CONFIG2_TMODE3   (((15 << NAND_CONFIG2_TRHZ_SHIFT) & NAND_CONFIG2_TRHZ_MASK)  \
                               | ((6 << NAND_CONFIG2_TCEA_TREA_SHIFT) & NAND_CONFIG2_TCEA_TREA_MASK) \
                               | ((0 << NAND_CONFIG2_TOE_SHIFT) & NAND_CONFIG2_TOE_MASK))


/*
 * Timing Mode 4 config1:  0x00252222
 * Hold time:		         9ns
 * Setup time:		         24ns
 * Write high time:	      14ns
 * Write pulse width:	   14ns
 * Read high time:		   14ns
 * Read pulse width:	      14ns
 */
#define NAND_CONFIG1_TMODE4   (((2 << NAND_CONFIG1_HOLD_TH_SHIFT) & NAND_CONFIG1_HOLD_TH_MASK)  \
                               | ((5 << NAND_CONFIG1_SETUP_TS_SHIFT) & NAND_CONFIG1_SETUP_TS_MASK)     \
                               | ((2 << NAND_CONFIG1_WR_CYCLE_TWH_SHIFT) & NAND_CONFIG1_WR_CYCLE_TWH_MASK)   \
                               | ((2 << NAND_CONFIG1_WR_PULSE_WIDTH_TWP_SHIFT) & NAND_CONFIG1_WR_PULSE_WIDTH_TWP_MASK)   \
                               | ((2 << NAND_CONFIG1_RD_CYCLE_TREH_SHIFT) & NAND_CONFIG1_RD_CYCLE_TREH_MASK) \
                               | ((2 << NAND_CONFIG1_RD_PULSE_WIDTH_TRP_SHIFT) & NAND_CONFIG1_RD_PULSE_WIDTH_TRP_MASK))

/*
 * Timing Mode 4 config2:                       0x0000F501
 * RE high to out impedance time:		         72ns
 * difference between CE and RE access time:    24ns
 * Output enable delay tOE:                     9ns
 * (Time adjustment for output enable to control the sampling time)
*/
#define NAND_CONFIG2_TMODE4   (((15 << NAND_CONFIG2_TRHZ_SHIFT) & NAND_CONFIG2_TRHZ_MASK)  \
                               | ((5 << NAND_CONFIG2_TCEA_TREA_SHIFT) & NAND_CONFIG2_TCEA_TREA_MASK) \
                               | ((1 << NAND_CONFIG2_TOE_SHIFT) & NAND_CONFIG2_TOE_MASK))


/*
 * Timing Mode 5 config1:  0x00241212
 * Hold time:		         9ns
 * Setup time:		         19ns
 * Write high time:	       9ns
 * Write pulse width:	   14ns
 * Read high time:		    9ns
 * Read pulse width:	      14ns
 */
#define NAND_CONFIG1_TMODE5   (((2 << NAND_CONFIG1_HOLD_TH_SHIFT) & NAND_CONFIG1_HOLD_TH_MASK)  \
                               | ((4 << NAND_CONFIG1_SETUP_TS_SHIFT) & NAND_CONFIG1_SETUP_TS_MASK)     \
                               | ((1 << NAND_CONFIG1_WR_CYCLE_TWH_SHIFT) & NAND_CONFIG1_WR_CYCLE_TWH_MASK)   \
                               | ((2 << NAND_CONFIG1_WR_PULSE_WIDTH_TWP_SHIFT) & NAND_CONFIG1_WR_PULSE_WIDTH_TWP_MASK)   \
                               | ((1 << NAND_CONFIG1_RD_CYCLE_TREH_SHIFT) & NAND_CONFIG1_RD_CYCLE_TREH_MASK) \
                               | ((2 << NAND_CONFIG1_RD_PULSE_WIDTH_TRP_SHIFT) & NAND_CONFIG1_RD_PULSE_WIDTH_TRP_MASK))

/*
 * Timing Mode 5 config2:                       0x0000F401
 * RE high to out impedance time:		         72ns
 * difference between CE and RE access time:    19ns
 * Output enable delay tOE:                     9ns
 * (Time adjustment for output enable to control the sampling time)
*/
#define NAND_CONFIG2_TMODE5   (((15 << NAND_CONFIG2_TRHZ_SHIFT) & NAND_CONFIG2_TRHZ_MASK)  \
                               | ((4 << NAND_CONFIG2_TCEA_TREA_SHIFT) & NAND_CONFIG2_TCEA_TREA_MASK) \
                               | ((1 << NAND_CONFIG2_TOE_SHIFT) & NAND_CONFIG2_TOE_MASK))



/* DMA descriptor entry */
typedef struct tag_chal_nand_prd_entry_t
{
  uint32_t  phys_addr;	/* source/destination physical address	*/
  uint32_t	desc;	      /* descriptor		*/
} chal_nand_prd_entry_t;

/* Configure DMA descriptor entry */
static inline void chal_nand_prd_desc_set (chal_nand_prd_entry_t *entry,
                                           void *phys,
                                           uint8_t bank,
                                           uint32_t size,
                                           uint8_t eot)
{
  entry->phys_addr = (uint32_t)phys;
  entry->desc = (eot << 31) |
                ((bank & 0x7) << 28) |
                (size & 0x3FFFFF);
}


/*
 * Reset the NAND controller
 */
static inline void chal_nand_reset(void)
{
    CHAL_REG_WRITE32 (NAND_REG_CONTROL, NAND_CONTROL_NORMAL_CONFIG | NAND_CONTROL_RST);
    /* disable and clear interrupts */
    CHAL_REG_WRITE32 (NAND_REG_IRQCTRL, 0);
    CHAL_REG_WRITE32 (NAND_REG_IRQSTATUS, ~0x0);
    CHAL_REG_WRITE32 (NAND_REG_CONTROL, NAND_CONTROL_NORMAL_CONFIG);
    /* disable BCH-ECC */
    CHAL_REG_WRITE32 (NAND_REG_BCH_CTRL, 0);
}

/*
 * Load micro-code to the NAND controller buffer to the specified offset and increment offset
 */
static inline uint32_t chal_nand_ucode_load (uint32_t *offset, const uint32_t *ucode, uint32_t size)
{
   uint32_t *ptr;
   uint32_t cmd;
   uint32_t nb_instr;

   ptr = (uint32_t *)NAND_REG_UCODE_START + *offset;
   cmd = *offset;
   nb_instr = size / sizeof(uint32_t);
   *offset += nb_instr;

   if ((uint32_t *)NAND_REG_UCODE_END >= ptr + nb_instr)
   {
      while (nb_instr--)
      {
         *ptr++ = *ucode++;
      }
   }

   return ( cmd );
}

/*
 * Verify if the loaded micro-code fits into the NAND controller buffer
 */
static inline uint32_t chal_nand_ucode_buf_overflow (uint32_t offset)
{
   return (offset > (uint32_t *)NAND_REG_UCODE_END - (uint32_t *)NAND_REG_UCODE_START);
}


/*
 * Configure DMAINT register
 */
static inline void chal_nand_dmaint_set(uint32_t bank,
                                        uint32_t data_size,
                                        uint32_t aux_size)
{
   uint32_t aux_type_shift, dmaint;
      
   aux_type_shift = ((CHAL_REG_READ32(NAND_REG_CONFIG0) & NAND_CONFIG0_AUX_DATA_TYPE_MASK) >> NAND_CONFIG0_AUX_DATA_TYPE_SHIFT) +1;

   dmaint = (data_size >> 4) | NAND_DMAINT0_ENABLE_MASK | (bank << 10) | (aux_size >> aux_type_shift);

   CHAL_REG_WRITE32 (NAND_REG_DMAINT0, dmaint);
   CHAL_REG_WRITE32 (NAND_REG_DMAINT1, 0x0);
   CHAL_REG_WRITE32 (NAND_REG_DMAINT2, 0x0);
   CHAL_REG_WRITE32 (NAND_REG_DMAINT3, 0x0);
}


/*
 * Start micro-code program execution
 */
static inline void chal_nand_ucode_exec(uint32_t bank,
                                        uint32_t cmd_offset,
                                        uint32_t addr,
                                        uint32_t attr0,
                                        uint32_t attr1,
                                        void *prdbase,
                                        uint32_t direction)
{
   uint32_t ctrl;
      
   ctrl = CHAL_REG_READ32 (NAND_REG_CONTROL);

   /* set the transfer direction */
   if (direction == NAND_DIRECTION_TO_NAND)
   {
      ctrl |= NAND_CONTROL_2NAND;
   }
   else
   {
      ctrl &= ~NAND_CONTROL_2NAND;
   }

   CHAL_REG_WRITE32 (NAND_REG_CONTROL, ctrl);
   CHAL_REG_WRITE32 (NAND_REG_ADDRESS, addr);
   CHAL_REG_WRITE32 (NAND_REG_ATTR0, attr0);
   CHAL_REG_WRITE32 (NAND_REG_ATTR1, attr1);
   CHAL_REG_WRITE32 (NAND_REG_BANK, bank);

   /* clear all IRQ in the status */
   CHAL_REG_WRITE32 (NAND_REG_IRQSTATUS, (
                                          NAND_IRQSTATUS_BCH_ERR_RDY_MASK |
                                          NAND_IRQSTATUS_BCH_WR_ECC_VALID_MASK |
                                          NAND_IRQSTATUS_BCH_RD_ECC_VALID_MASK |
                                          NAND_IRQSTATUS_DMA_CMPL_IRQ_MASK |
                                          NAND_IRQSTATUS_DMA_ERROR_ENABLE_MASK |
                                          (0x1 << (NAND_IRQSTATUS_BANK_CMPL_IRQ_SHIFT + (bank))) |
                                          (0x1 << (NAND_IRQSTATUS_BANK_ERR_IRQ_SHIFT + (bank))) |
                                          (0x1 << (NAND_IRQSTATUS_RB_IRQ_SHIFT + (bank)))
                                         ));

   CHAL_REG_WRITE32 (NAND_REG_PRDBASE, (uint32_t)prdbase); 

   if (prdbase)
   {
      /* start DMA */
      CHAL_REG_WRITE32 (NAND_REG_CONTROL, CHAL_REG_READ32 (NAND_REG_CONTROL) | NAND_CONTROL_DMA_START);
   }
   else
   {  /* clear DMAINT register */
      CHAL_REG_WRITE32 (NAND_REG_DMAINT0, 0x0);
      CHAL_REG_WRITE32 (NAND_REG_DMAINT1, 0x0);
      CHAL_REG_WRITE32 (NAND_REG_DMAINT2, 0x0);
      CHAL_REG_WRITE32 (NAND_REG_DMAINT3, 0x0);
   }

   /* start ucode execution */
   CHAL_REG_WRITE32(NAND_REG_COMMAND, NAND_COMMAND_VALID_MASK | cmd_offset);
}


/*
 * Start the DMA
 */
static inline void chal_nand_dma_start(void)
{
  CHAL_REG_WRITE32 (NAND_REG_CONTROL, CHAL_REG_READ32 (NAND_REG_CONTROL) | NAND_CONTROL_DMA_START);
}

/*
 * Stop the DMA
 */
static inline void chal_nand_dma_stop(void)
{
  CHAL_REG_WRITE32 (NAND_REG_CONTROL, CHAL_REG_READ32 ( NAND_REG_CONTROL) & ~NAND_CONTROL_DMA_START);
}


/*
 * Get the NAND status 
 */
static inline uint8_t chal_nand_status_result(void)
{
   return (CHAL_REG_READ32(NAND_REG_MRESP) & NAND_MRESP_NANDSTATUS_MASK) >> NAND_MRESP_NANDSTATUS_SHIFT;
}


/*
 * Get the R/B interrupt status
 */
static inline uint32_t chal_nand_rb_irq_status(uint32_t bank)
{
  return 0x1 & ((CHAL_REG_READ32(NAND_REG_IRQSTATUS) & NAND_IRQSTATUS_RB_IRQ_MASK) >> (NAND_IRQSTATUS_RB_IRQ_SHIFT + bank));
}

/*
 * Clear the R/B interrupt status
 */
static inline void chal_nand_rb_irq_clear(uint32_t bank)
{
  CHAL_REG_WRITE32(NAND_REG_IRQSTATUS, NAND_IRQSTATUS_RB_IRQ_MASK & (0x1 << (NAND_IRQSTATUS_RB_IRQ_SHIFT + bank)));
}

/*
 * Get the interrupt status
 */
static inline uint32_t chal_nand_irq_status(void)
{
  return (CHAL_REG_READ32(NAND_REG_IRQSTATUS));
}

/*
 * Clear the specified interrupts
 */
static inline void chal_nand_irq_clear(uint32_t mask)
{
  CHAL_REG_WRITE32(NAND_REG_IRQSTATUS, mask);
}

/*
 * Get the interrupt configuration
 */
static inline uint32_t chal_nand_irq_get(void)
{
  return (CHAL_REG_READ32(NAND_REG_IRQCTRL));
}

/*
 * Set the specified interrupt configuration
 */
static inline void chal_nand_irq_set(uint32_t mask)
{
  CHAL_REG_WRITE32(NAND_REG_IRQCTRL, mask);
}

/*
 * Enable the specified interrupts
 */
static inline void chal_nand_irq_enable(uint32_t mask)
{
  CHAL_REG_WRITE32(NAND_REG_IRQCTRL, (mask | CHAL_REG_READ32(NAND_REG_IRQCTRL)));
}

/*
 * Disable the specified interrupts
 */
static inline void chal_nand_irq_disable(uint32_t mask)
{
  CHAL_REG_WRITE32(NAND_REG_IRQCTRL, (~mask & CHAL_REG_READ32(NAND_REG_IRQCTRL)));
}


/*
 * Get the controller configuration (CONFIG0 register)
 */
static inline uint32_t chal_nand_get_config(void)
{
  return (CHAL_REG_READ32(NAND_REG_CONFIG0));
}

/*
 * Set the controller configuration (CONFIG0 register)
 */
static inline void chal_nand_set_config(uint32_t config)
{
  CHAL_REG_WRITE32(NAND_REG_CONFIG0, config);
}

/*
 * Set the bus width field in the controller configuration 
 */
static inline uint32_t chal_nand_set_data_width(uint32_t config, uint32_t width)
{
   config &= ~NAND_CONFIG0_DATA_WIDTH_MASK;
   return (config | ((width << NAND_CONFIG0_DATA_WIDTH_SHIFT) & NAND_CONFIG0_DATA_WIDTH_MASK));
}

/*
 * Set the auxiliary data type field in the controller configuration 
 */
static inline uint32_t chal_nand_set_aux_data_type(uint32_t config, uint32_t aux_data_type)
{
   config &= ~NAND_CONFIG0_AUX_DATA_TYPE_MASK;
   return (config | ((aux_data_type << NAND_CONFIG0_AUX_DATA_TYPE_SHIFT) & NAND_CONFIG0_AUX_DATA_TYPE_MASK));
}

/*
 * Get the auxiliary data type size in bytes 
 */
static inline uint32_t chal_nand_get_aux_data_type_size(void)
{
   return 1 << (((CHAL_REG_READ32(NAND_REG_CONFIG0) & NAND_CONFIG0_AUX_DATA_TYPE_MASK) >> NAND_CONFIG0_AUX_DATA_TYPE_SHIFT) + 1);
}

/*
 * Get the timers configuration (CONFIG1 and CONFIG2 register)
 */
static inline void chal_nand_get_timers(uint32_t *config1, uint32_t *config2)
{
  *config1 = CHAL_REG_READ32(NAND_REG_CONFIG1);
  *config2 = CHAL_REG_READ32(NAND_REG_CONFIG2);
}

/*
 * Set the timers configuration (CONFIG1 and CONFIG2 register)
 */
static inline void chal_nand_set_timers(uint32_t config1, uint32_t config2)
{
  CHAL_REG_WRITE32(NAND_REG_CONFIG1, config1);
  CHAL_REG_WRITE32(NAND_REG_CONFIG2, config2);
}

/*
 * Set tH in the configuration argument
 */
static inline void chal_nand_set_th(uint32_t *config1, uint32_t th)
{
   *config1 &= ~NAND_CONFIG1_HOLD_TH_MASK;
   *config1 |= ((th << NAND_CONFIG1_HOLD_TH_SHIFT) & NAND_CONFIG1_HOLD_TH_MASK);
}

/*
 * Set tS in the configuration argument 
 */
static inline void chal_nand_set_ts(uint32_t *config1, uint32_t ts)
{
   *config1 &= ~NAND_CONFIG1_SETUP_TS_MASK;
   *config1 |= ((ts << NAND_CONFIG1_SETUP_TS_SHIFT) & NAND_CONFIG1_SETUP_TS_MASK);
}

/*
 * Set tWH in the configuration argument 
 */
static inline void chal_nand_set_twh(uint32_t *config1, uint32_t twh)
{
   *config1 &= ~NAND_CONFIG1_WR_CYCLE_TWH_MASK;
   *config1 |= ((twh << NAND_CONFIG1_WR_CYCLE_TWH_SHIFT) & NAND_CONFIG1_WR_CYCLE_TWH_MASK);
}

/*
 * Set tWP in the configuration argument 
 */
static inline void chal_nand_set_twp(uint32_t *config1, uint32_t twp)
{
   *config1 &= ~NAND_CONFIG1_WR_PULSE_WIDTH_TWP_MASK;
   *config1 |= ((twp << NAND_CONFIG1_WR_PULSE_WIDTH_TWP_SHIFT) & NAND_CONFIG1_WR_PULSE_WIDTH_TWP_MASK);
}

/*
 * Set tREH in the configuration argument 
 */
static inline void chal_nand_set_treh(uint32_t *config1, uint32_t treh)
{
   *config1 &= ~NAND_CONFIG1_RD_CYCLE_TREH_MASK;
   *config1 |= ((treh << NAND_CONFIG1_RD_CYCLE_TREH_SHIFT) & NAND_CONFIG1_RD_CYCLE_TREH_MASK);
}

/*
 * Set tRP in the configuration argument 
 */
static inline void chal_nand_set_trp(uint32_t *config1, uint32_t trp)
{
   *config1 &= ~NAND_CONFIG1_RD_PULSE_WIDTH_TRP_MASK;
   *config1 |= ((trp << NAND_CONFIG1_RD_PULSE_WIDTH_TRP_SHIFT) & NAND_CONFIG1_RD_PULSE_WIDTH_TRP_MASK);
}

/*
 * Set tRHZ in the configuration argument 
 */
static inline void chal_nand_set_trhz(uint32_t *config2, uint32_t trhz)
{
   *config2 &= ~NAND_CONFIG2_TRHZ_MASK;
   *config2 |= ((trhz << NAND_CONFIG2_TRHZ_SHIFT) & NAND_CONFIG2_TRHZ_MASK);
}

/*
 * Set tCEA-tREA differnce in the configuration argument 
 */
static inline void chal_nand_set_tcea_trea(uint32_t *config2, uint32_t tcea_trea)
{
   *config2 &= ~NAND_CONFIG2_TCEA_TREA_MASK;
   *config2 |= ((tcea_trea << NAND_CONFIG2_TCEA_TREA_SHIFT) & NAND_CONFIG2_TCEA_TREA_MASK);
}

/*
 * Set tOE in the configuration argument 
 */
static inline void chal_nand_set_toe(uint32_t *config2, uint32_t toe)
{
   *config2 &= ~NAND_CONFIG2_TOE_MASK;
   *config2 |= ((toe << NAND_CONFIG2_TOE_SHIFT) & NAND_CONFIG2_TOE_MASK);
}


/*
 * Get tH from the configuration argument
 */
static inline uint32_t chal_nand_get_th(uint32_t config1)
{
   return ((config1 & NAND_CONFIG1_HOLD_TH_MASK) >> NAND_CONFIG1_HOLD_TH_SHIFT);
}

/*
 * Get tS from the configuration argument 
 */
static inline uint32_t chal_nand_get_ts(uint32_t config1)
{
   return ((config1 & NAND_CONFIG1_SETUP_TS_MASK) >> NAND_CONFIG1_SETUP_TS_SHIFT);
}

/*
 * Get tWH from the configuration argument 
 */
static inline uint32_t chal_nand_get_twh(uint32_t config1)
{
   return ((config1 & NAND_CONFIG1_WR_CYCLE_TWH_MASK) >> NAND_CONFIG1_WR_CYCLE_TWH_SHIFT);
}

/*
 * Get tWP from the configuration argument 
 */
static inline uint32_t chal_nand_get_twp(uint32_t config1)
{
   return ((config1 & NAND_CONFIG1_WR_PULSE_WIDTH_TWP_MASK) >> NAND_CONFIG1_WR_PULSE_WIDTH_TWP_SHIFT);
}

/*
 * Get tREH from the configuration argument 
 */
static inline uint32_t chal_nand_get_treh(uint32_t config1)
{
   return ((config1 & NAND_CONFIG1_RD_CYCLE_TREH_MASK) >> NAND_CONFIG1_RD_CYCLE_TREH_SHIFT);
}

/*
 * Get tRP from the configuration argument 
 */
static inline uint32_t chal_nand_get_trp(uint32_t config1)
{
   return ((config1 & NAND_CONFIG1_RD_PULSE_WIDTH_TRP_MASK) >> NAND_CONFIG1_RD_PULSE_WIDTH_TRP_SHIFT);
}

/*
 * Get tRHZ from the configuration argument 
 */
static inline uint32_t chal_nand_get_trhz(uint32_t config2)
{
   return ((config2 & NAND_CONFIG2_TRHZ_MASK) >> NAND_CONFIG2_TRHZ_SHIFT);
}

/*
 * Get tCEA-tREA differnce from the configuration argument 
 */
static inline uint32_t chal_nand_get_tcea_trea(uint32_t config2)
{
   return ((config2 & NAND_CONFIG2_TCEA_TREA_MASK) >> NAND_CONFIG2_TCEA_TREA_SHIFT);
}

/*
 * Get tOE from the configuration argument 
 */
static inline uint32_t chal_nand_get_toe(uint32_t config2)
{
   return ((config2 & NAND_CONFIG2_TOE_MASK) >> NAND_CONFIG2_TOE_SHIFT);
}


/***************************************************************************
 *
 * BCH-ECC functions
 *
 ***************************************************************************/


/*
 * Configure BCH ECC parameters
 */
static inline void chal_nand_bch_config(uint32_t bus, uint32_t n, uint32_t k, uint32_t t)
{
   /* Set bus width and disable raead/write BCH ECC calculation */
   CHAL_REG_WRITE32 (NAND_REG_BCH_CTRL, NAND_BCH_CTRL_BUS_WIDTH_MASK & ((bus >> 4) << NAND_BCH_CTRL_BUS_WIDTH_SHIFT));
   /* Set N, K and T parameters */
   CHAL_REG_WRITE32 (NAND_REG_BCH_NK, (NAND_BCH_NK_K_MASK & (k << NAND_BCH_NK_K_SHIFT)) |
                                      (NAND_BCH_NK_N_MASK & (n << NAND_BCH_NK_N_SHIFT)));
   CHAL_REG_WRITE32 (NAND_REG_BCH_T, NAND_BCH_T_T_MASK & (t << NAND_BCH_T_T_SHIFT));
}

/*
 * Enable BCH ECC computation for read
 */
static inline void chal_nand_bch_read_enable(void)
{
  CHAL_REG_WRITE32 (NAND_REG_BCH_CTRL, CHAL_REG_READ32 (NAND_REG_BCH_CTRL) | NAND_BCH_CTRL_ECC_RD_EN_MASK);
}

/*
 * Enable BCH ECC computation for write
 */
static inline void chal_nand_bch_write_enable(void)
{
  CHAL_REG_WRITE32 (NAND_REG_BCH_CTRL, CHAL_REG_READ32 (NAND_REG_BCH_CTRL) | NAND_BCH_CTRL_ECC_WR_EN_MASK);
}

/*
 * Disable BCH ECC computation (for both read and write)
 */
static inline void chal_nand_bch_disable(void)
{
  CHAL_REG_WRITE32 (NAND_REG_BCH_CTRL, CHAL_REG_READ32 (NAND_REG_BCH_CTRL) &
                     ~(NAND_BCH_CTRL_ECC_RD_EN_MASK | NAND_BCH_CTRL_ECC_WR_EN_MASK));
}

/*
 * Pause BCH ECC computation
 */
static inline void chal_nand_bch_pause(void)
{
  CHAL_REG_WRITE32 (NAND_REG_BCH_CTRL, CHAL_REG_READ32 (NAND_REG_BCH_CTRL) | NAND_BCH_CTRL_RD_PAUSE_MASK);
}

/*
 * Resume BCH ECC computation
 */
static inline void chal_nand_bch_resume(void)
{
  CHAL_REG_WRITE32 (NAND_REG_BCH_CTRL, CHAL_REG_READ32 (NAND_REG_BCH_CTRL) & ~NAND_BCH_CTRL_RD_PAUSE_MASK);
}

/*
 * Returns the number of correctable errors or:
 *    -1 for uncorrectable error 
 *    0 for no error
 */
static inline int32_t chal_nand_bch_err_count(void)
{
   uint32_t bch_status;

   bch_status = CHAL_REG_READ32 (NAND_REG_BCH_STATUS);

   /* check if uncorrectable error was detected */
   if (bch_status & NAND_BCH_STATUS_UNCORR_ERR_MASK)
   {
      return -1;
   }

   /* check if correctable error was detected */
   if (bch_status & NAND_BCH_STATUS_CORR_ERR_MASK)
   {
      return (bch_status & NAND_BCH_STATUS_NB_CORR_ERR_MASK) >> NAND_BCH_STATUS_NB_CORR_ERR_SHIFT;
   }

   return 0;
}

/*
 * Retrive all bad bit locations and fix
 * Note: The len parameter is used to limit the corrections to the data
 * portion only (i.e. not correct bad bits in ECC bytes when this is not
 * needed and/or ECC bytes have been transferred to a different location)
 */
static inline void chal_nand_bch_err_fix(int32_t err_count, uint8_t *buf, uint32_t len)
{
   uint32_t bit, byte;

   while(err_count > 0)
   {
      err_count--;
      bit = (CHAL_REG_READ32(NAND_REG_BCH_ERR_LOC) & NAND_BCH_ERR_LOC_ERR_LOC_MASK) >> NAND_BCH_ERR_LOC_ERR_LOC_SHIFT;
      byte = bit >> 3;
      if (byte < len)
      {
         buf[byte] ^= 0x1 << (bit & 0x7);
      }
   }
}

/*
 * Copy the calculated BCH ECC bytes from controller to specified destination
 */
static inline void chal_nand_bch_ecc_copy(uint8_t *dest, uint32_t len)
{
   uint32_t buf[18];
   uint32_t w;
   uint32_t *w_src, *w_dest, *w_end;
   uint8_t  *b_src, *b_end;

   w = (len + 3) >> 2;

   w_src = (uint32_t*)NAND_REG_BCH_WR_ECC0; 
   w_dest = buf;
   w_end = buf + w;
   while (w_dest < w_end)
   {
      *w_dest = CHAL_REG_READ32(w_src);
      w_dest++;
      w_src++;
   }
  
   b_end = (uint8_t*)buf - 1;
   b_src = (uint8_t*)buf + len - 1;  
   while (b_end < b_src)
   {
      *dest++ = *b_src--;
   }
}


/*
 * Get the bch_err_rdy interrupt status
 */
static inline uint32_t chal_nand_bch_err_rdy_irq_status(void)
{
  return ((CHAL_REG_READ32(NAND_REG_IRQSTATUS) & NAND_IRQSTATUS_BCH_ERR_RDY_MASK) >> NAND_IRQSTATUS_BCH_ERR_RDY_SHIFT);
}

/*
 * Clear the bch_err_rdy interrupt status
 */
static inline void chal_nand_bch_err_rdy_irq_clear(void)
{
  CHAL_REG_WRITE32(NAND_REG_IRQSTATUS, NAND_IRQSTATUS_BCH_ERR_RDY_MASK);
}

/*
 * Get the bch_wr_ecc_valid interrupt status
 */
static inline uint32_t chal_nand_bch_wr_ecc_valid_irq_status(void)
{
  return ((CHAL_REG_READ32(NAND_REG_IRQSTATUS) & NAND_IRQSTATUS_BCH_WR_ECC_VALID_MASK) >> NAND_IRQSTATUS_BCH_WR_ECC_VALID_SHIFT);
}

/*
 * Clear bch_wr_ecc_valid interrupt status
 */
static inline void chal_nand_bch_wr_ecc_valid_irq_clear(void)
{
  CHAL_REG_WRITE32(NAND_REG_IRQSTATUS, NAND_IRQSTATUS_BCH_WR_ECC_VALID_MASK);
}

/*
 * Get the bch_rd_ecc_valid interrupt status
 */
static inline uint32_t chal_nand_bch_rd_ecc_valid_irq_status(void)
{
  return ((CHAL_REG_READ32(NAND_REG_IRQSTATUS) & NAND_IRQSTATUS_BCH_RD_ECC_VALID_MASK) >> NAND_IRQSTATUS_BCH_RD_ECC_VALID_SHIFT);
}

/*
 * Clear bch_rd_ecc_valid interrupt status
 */
static inline void chal_nand_bch_rd_ecc_valid_irq_clear(void)
{
  CHAL_REG_WRITE32(NAND_REG_IRQSTATUS, NAND_IRQSTATUS_BCH_RD_ECC_VALID_MASK);
}

#endif

