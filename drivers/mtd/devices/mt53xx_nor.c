/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/******************************************************************************
* [File]			mtk_nor.c
* [Version]			v1.0
* [Revision Date]	2011-05-04
* [Author]			Shunli Wang, shunli.wang@mediatek.inc, 82896, 2012-04-27
* [Description]
*	SPI-NOR Driver Source File
* [Copyright]
*	Copyright (C) 2011 MediaTek Incorporation. All Rights Reserved.
******************************************************************************/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/mutex.h>
#include <linux/math64.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/mod_devicetable.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/spi/spi.h>
#include <linux/spi/flash.h>
#include <linux/delay.h>
#include <asm/delay.h>
#include <linux/semaphore.h>
#include <linux/dma-mapping.h>
#include <linux/completion.h>
#include <linux/time.h>

#include <mach/irqs.h>

#include "mt53xx_nor.h"

#define  NOR_DEBUG 0

// get ns time
extern void do_gettimeofday(struct timeval * tv);
/* For mtdchar.c and mtdsdm.c used 
  */
extern char *mtdchar_buf;
extern struct semaphore mtdchar_share;

// Aligned the buffer address for DMA used.
static u8 _pu1NorDataBuf[SFLASH_MAX_DMA_SIZE]  __attribute__((aligned(0x10)));
struct mtd_partition *mtdparts;
static  u8  partnum;



/* handle interrupt waiting 
  */
static struct completion comp;

struct mt53xx_nor
{
  struct mutex lock;
  struct mtd_info mtd;

  /* flash information struct obejct pointer
     */
  SFLASHHW_VENDOR_T *flash_info;

  /* flash number
     * total capacity
     */
  u32 total_sz;
  u8 flash_num;
  
  /* current accessed flash index
     * current command
     */
  u8 cur_flash_index;
  u8 cur_cmd_val;

  /* isr enable or not
     * dma read enable or not
     */
  u8 host_act_attr;

  /* current sector index
     */

  u32 cur_sector_index;
  u32 cur_addr_offset;
  
};

static SFLASHHW_VENDOR_T _aVendorFlash[] =
{
    { 0x01, 0x02, 0x12, 0x0,  0x80000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL004A)" },
    { 0x01, 0x02, 0x13, 0x0, 0x100000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL08A)" },
    { 0x01, 0x02, 0x14, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL016A)" },
    { 0x01, 0x02, 0x15, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL032A)" },
    { 0x01, 0x02, 0x16, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL064A)" },
    { 0x01, 0x20, 0x18, 0x0, 0x1000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "Spansion(S25FL128P)" },

    { 0xC2, 0x20, 0x13, 0x0,  0x80000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L400)" },
    { 0xC2, 0x20, 0x14, 0x0, 0x100000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L800)" },
    { 0xC2, 0x20, 0x15, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x3B, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L160)" },
    { 0xC2, 0x24, 0x15, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x3B, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L1635D)" },
    { 0xC2, 0x20, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x3B, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L320)" },
    { 0xC2, 0x20, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x3B, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L640)" },
    { 0xC2, 0x20, 0x18, 0x0, 0x1000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0xBB, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L128)" },
    { 0xC2, 0x5E, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "MXIC(25L3235D)" },

	{ 0xC8, 0x40, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x3B, SFLASH_WRITE_PROTECTION_VAL, "GD(GD25QBSIG)" },

    { 0x20, 0x20, 0x14, 0x0, 0x100000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P80)" },
    { 0x20, 0x20, 0x15, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P16)" },
    { 0x20, 0x20, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P32)" },
    { 0x20, 0x20, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P64)" },
    { 0x20, 0x20, 0x18, 0x0, 0x1000000, 0x40000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25P128)" },
    { 0x20, 0x71, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25PX32)" },
    { 0x20, 0x71, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ST(M25PX64)" },

    { 0xEF, 0x30, 0x13, 0x0,  0x80000,   0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25X40)" },
    { 0xEF, 0x30, 0x14, 0x0, 0x100000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25X80)" },
    { 0xEF, 0x30, 0x15, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25X16)" },
    { 0xEF, 0x30, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25X32)" },
    { 0xEF, 0x30, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25X64)" },

	{ 0xEF, 0x40, 0x15, 0x0, 0x200000,	0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0xBB, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25Q16FV)" },
	{ 0xEF, 0x40, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25Q32BV)" },
    { 0xEF, 0x40, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0xBB, SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25Q64FV)" },
    { 0xEF, 0x40, 0x18, 0x0, 0x1000000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00,SFLASH_WRITE_PROTECTION_VAL, "WINBOND(W25Q128BV)" },
#if 0 // sector size is not all 64KB, not support!!
    { 0x1C, 0x20, 0x15, 0x0, 0x200000,  0x1000,  60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25B16)" },
    { 0x1C, 0x20, 0x16, 0x0, 0x400000,  0x1000,  60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25B32)" },
    { 0x7F, 0x37, 0x20, 0x0, 0x200000,  0x1000,  60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "AMIC(A25L16P)"},
    { 0xBF, 0x25, 0x41, 0x0, 0x200000,  0x1000,  60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0xAD, 0xAD, 0x00, SFLASH_WRITE_PROTECTION_VAL, "SST(25VF016B)" },
    { 0x8C, 0x20, 0x15, 0x0, 0x200000,  0x1000,  60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0xAD, 0xAD, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ESMT(F25L016A)" },
#endif

#if (defined(CC_MT5360B) || defined(CC_MT5387) ||defined(CC_MT5363) ||defined(ENABLE_AAIWRITE))
    { 0xBF, 0x43, 0x10, 0x0,  0x40000,  0x8000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0x52, 0x60, 0x02, 0xAF, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF020)" },
    { 0xBF, 0x25, 0x8D, 0x0,  0x80000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF040B)" },
    { 0xBF, 0x25, 0x8E, 0x0, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF080B)" },
    { 0xBF, 0x25, 0x41, 0x0, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF016B)" },
    { 0xBF, 0x25, 0x4A, 0x0, 0x400000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF032B)" },
    { 0xBF, 0x25, 0x4B, 0x0, 0x800000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF064C)" },
#else
    { 0xBF, 0x43, 0x10, 0x1,  0x40000,  0x8000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0x52, 0x60, 0x02, 0xAF, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF020)" },
    { 0xBF, 0x25, 0x8D, 0x1,  0x80000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF040B)" },
    { 0xBF, 0x25, 0x8E, 0x1, 0x100000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF080B)" },
    { 0xBF, 0x25, 0x41, 0x1, 0x200000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF016B)" },
    { 0xBF, 0x25, 0x4A, 0x1, 0x400000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF032B)" },
    { 0xBF, 0x25, 0x4B, 0x1, 0x800000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0xAD, 0x00,  SFLASH_WRITE_PROTECTION_VAL,"SST(SST25VF064C)" },
#endif

    { 0x1F, 0x47, 0x00, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ATMEL(AT25DF321)" },
    { 0x1F, 0x48, 0x00, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "ATMEL(AT25DF641)" },

    { 0x1C, 0x20, 0x13, 0x0,  0x80000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25B40)" },
    { 0x1C, 0x31, 0x14, 0x0, 0x100000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25F80)" },
    { 0x1C, 0x20, 0x15, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25P16)" },
    { 0x1C, 0x70, 0x15, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0xBB, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25NEW)" },
    { 0x1C, 0x20, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0xBB, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25P32)" },
    { 0x1C, 0x20, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0xBB, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25P64)" },
    { 0x1C, 0x30, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0xBB, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25Q64)" },
    { 0x1C, 0x30, 0x18, 0x0, 0x1000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "EON(EN25P128)" },

    { 0x7F, 0x37, 0x20, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "AMIC(A25L40P)" },
    { 0x37, 0x30, 0x13, 0x0, 0x100000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "AMIC(A25L040)" },
    { 0x37, 0x30, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "AMIC(A25L032)" },

    { 0xFF, 0xFF, 0x10, 0x0,  0x10000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xC7, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "512Kb" },
//    { 0xFF, 0xFF, 0x11, 0x0,  0x20000,  0x8000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "1Mb" },
    { 0xFF, 0xFF, 0x12, 0x0,  0x40000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "2Mb" },
    { 0xFF, 0xFF, 0x13, 0x0,  0x80000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "4Mb" },
    { 0xFF, 0xFF, 0x14, 0x0, 0x100000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "8Mb" },
    { 0xFF, 0xFF, 0x15, 0x0, 0x200000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "16Mb" },
    { 0xFF, 0xFF, 0x16, 0x0, 0x400000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "32Mb" },
    { 0xFF, 0xFF, 0x17, 0x0, 0x800000,  0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "64Mb" },
    { 0xFF, 0xFF, 0x18, 0x0, 0x1000000, 0x10000, 60000, 0x06, 0x04, 0x05, 0x01, 0x03, 0x0B, 0x9F, 0xD8, 0xC7, 0x02, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "128Mb" },

    { 0x00, 0x00, 0x00, 0x0, 0x000000,  0x00000, 0x000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, SFLASH_WRITE_PROTECTION_VAL, "NULL Device" },
};


static inline struct mt53xx_nor *mtd_to_mt53xx_nor(struct mtd_info *mtd)
{
  return container_of(mtd, struct mt53xx_nor, mtd);
  
}

static void mt53xx_nor_DumpReg()
{
  u32 i;
  for(i = 0;i<0x100;i+=4)
  	printk(KERN_ERR "%08x ", SFLASH_RREG32(i));

  printk(KERN_ERR "--------\n");
  for(i=0;i<0x10;i+=4)
  	printk(KERN_ERR "%08x ", (*(volatile u32 *)(0xF000D400+i)));

  printk(KERN_ERR "0xF0060F00: %08x ", (*(volatile u32 *)(0xF0060F00)));
  
}

static void mt53xx_nor_PinMux()
{
  u32 val;
  
  // Set to NOR flash
  val = SFLASH_READ32(0xF000D3B0);
  val &= ~0x4;
  SFLASH_WRITE32(0xF000D3B0, val);

  val = SFLASH_READ32(0xF00299A0);
  val &= ~(0x1<<2);
  SFLASH_WRITE32(0xF00299A0, val);

}

static void mt53xx_nor_ClrInt()
{
  SFLASH_WREG8(SFLASH_SF_INTRSTUS_REG, SFLASH_RREG8(SFLASH_SF_INTRSTUS_REG));
}

static irqreturn_t mt53xx_nor_HandleIsr(int irq, void *dev_id)
{
  u8 intvect;

  intvect = SFLASH_RREG8(SFLASH_SF_INTRSTUS_REG);
  SFLASH_WREG8(SFLASH_SF_INTRSTUS_REG, intvect);

  //printk(KERN_ERR "[mt53xx_nor] interrupt(%02X)!", intvect);

  if(intvect & SFLASH_EN_INT)
  {
    complete(&comp);
  }
	
  return IRQ_HANDLED;
}

static void mt53xx_nor_WaitInt(struct mt53xx_nor *mt53xx_nor)
{
  u8 attr = mt53xx_nor->host_act_attr;

  if(attr & SFLASH_USE_ISR)
  {
    wait_for_completion(&comp);
  }
  else
  {
    while(!SFLASH_DMA_IS_COMPLETED);  
  }
}

static void mt53xx_nor_SetIsr(struct mt53xx_nor *mt53xx_nor)
{
  SFLASH_WREG32(SFLASH_SF_INTREN_REG, SFLASH_EN_INT);

  enable_irq(VECTOR_FLASH);    
  init_completion(&comp);

}

static u32 mt53xx_nor_RegIsr(struct mt53xx_nor *mt53xx_nor)
{
  mt53xx_nor_ClrInt();

  if (request_irq(VECTOR_FLASH, mt53xx_nor_HandleIsr, IRQF_SHARED, 
  	                                  mt53xx_nor->mtd.name, mt53xx_nor) != 0)	 
  {		  
    printk(KERN_ERR "request serial flash IRQ fail!\n");		  
    return -1;	
  }

  disable_irq(VECTOR_FLASH);

  return 0;
}

static u32 mt53xx_nor_CheckFlashIndex(struct mt53xx_nor *mt53xx_nor)
{
  u8 index = mt53xx_nor->cur_flash_index;
  
  if((index < 0) || (index > MAX_FLASHCOUNT - 1))
  {
    printk(KERN_ERR "invalid flash index:%d!\n", index);
    return -EINVAL;;
  }

  return 0;
  
}

static u32 mt53xx_nor_Offset2Index(struct mt53xx_nor *mt53xx_nor, u32 offset)
{
  u8 index;
  u32 chipLimit = 0, chipStart = 0, chipSectorSz;

  if(offset >= mt53xx_nor->total_sz)
  {
    printk(KERN_ERR "invalid offset exceeds max possible address!\b");
	return -EINVAL;;
  }

  for(index = 0; index < mt53xx_nor->flash_num; index++)
  {
    chipStart = chipLimit;
    chipLimit += mt53xx_nor->flash_info[index].u4ChipSize;
	chipSectorSz = mt53xx_nor->flash_info[index].u4SecSize;
	
    if(offset < chipLimit)
    {
      mt53xx_nor->cur_flash_index = index;
	  mt53xx_nor->cur_addr_offset = offset - chipStart;
	  mt53xx_nor->cur_sector_index = mt53xx_nor->cur_addr_offset/(chipSectorSz - 1); 
    }
  }

  return 0;
  
}

static u8 mt53xx_nor_Region2Info(struct mt53xx_nor *mt53xx_nor, u32 offset, u32 len)
{
  u8 info = 0, flash_cout = mt53xx_nor->flash_num;
  u32 first_chip_sz = mt53xx_nor->flash_info[0].u4ChipSize;

  if(len == mt53xx_nor->total_sz)
  {
    info |= REGION_RANK_TWO_WHOLE_FLASH;
    return info;
  }

  if(offset == 0)
  {
    info |= REGION_RANK_FIRST_FLASH;
	if(len >= first_chip_sz)
	{
      info |= REGION_RANK_FIRST_WHOLE_FLASH;
	  if((len > first_chip_sz) && (flash_cout > 1))
	  {
        info |= REGION_RANK_SECOND_FLASH;
	  }
	}
  }
  else if(offset < first_chip_sz)
  {
    info |= REGION_RANK_FIRST_FLASH;
	if(((offset + len) == mt53xx_nor->total_sz) && (flash_cout > 1))
	{
      info |= (REGION_RANK_SECOND_FLASH | REGION_RANK_SECOND_WHOLE_FLASH);
	}
  }
  else if((offset == first_chip_sz) && (flash_cout > 1))
  {
    info |= REGION_RANK_SECOND_FLASH;
    if(len == mt53xx_nor->total_sz - first_chip_sz)
    {
      info |= REGION_RANK_SECOND_WHOLE_FLASH;
    }
  }
  else if((offset > first_chip_sz) && (flash_cout > 1))
  {
    info |= REGION_RANK_SECOND_FLASH;
  }

  return info;
  
}

static void mt53xx_nor_SetDualRead(struct mt53xx_nor *mt53xx_nor)
{
  u8 attr = mt53xx_nor->host_act_attr;
  u8 dualread = mt53xx_nor->flash_info[0].u1DualREADCmd;
  

  /* make sure the same dual read command about two sflash
     */
  if((attr & SFLASH_USE_DUAL_READ) &&
  	 (dualread != 0x00))
  {
    SFLASH_WREG8(SFLASH_PRGDATA3_REG, dualread);
    SFLASH_WREG8(SFLASH_DUAL_REG, (dualread == 0xbb) ? 0x3 : 0x1);
  }
  else
  {
    SFLASH_WREG8(SFLASH_DUAL_REG, 0x0);
  }
  
}

static void mt53xx_nor_SendCmd(struct mt53xx_nor *mt53xx_nor)
{
  u8 index = mt53xx_nor->cur_flash_index;
  u8 cmdval = mt53xx_nor->cur_cmd_val;
  
  if((index < 0) || (index > MAX_FLASHCOUNT - 1))
  {
    printk(KERN_ERR "invalid flash index:%d!\n", index);
    return -EINVAL;;
  }

  SFLASH_WREG8(SFLASH_CMD_REG, index*0x40 + cmdval);
  
}

static u32 mt53xx_nor_PollingReg(u8 u1RegOffset, u8 u8Compare)
{
  u32 u4Polling;
  u8 u1Reg;

  u4Polling = 0;
  while(1)
  {
    u1Reg = SFLASH_RREG8(u1RegOffset);
    if (0x00 == (u1Reg & u8Compare))
    {
      break;
    }
    u4Polling++;
    if(u4Polling > SFLASH_POLLINGREG_COUNT)
    {
      printk("polling reg%02X using compare val%02X timeout!\n", 
	  	                                 u1RegOffset, u8Compare);
      return -1;
    }
  }
  
  return 0;
  
}

static u32 mt53xx_nor_ExecuteCmd(struct mt53xx_nor *mt53xx_nor)
{
  u8 val = (mt53xx_nor->cur_cmd_val) & 0x1F;

  mt53xx_nor_SendCmd(mt53xx_nor);

  return mt53xx_nor_PollingReg(SFLASH_CMD_REG, val);
  
}

static void mt53xx_nor_SetCfg1(u8 sf_num, u32 first_sf_cap)
{
    u8 u1Reg;

    u1Reg = SFLASH_RREG8(SFLASH_CFG1_REG);
    u1Reg &= (~0x1C);

#if (MAX_FLASHCOUNT > 1)
    if(sf_num > 1)
    {
        switch(first_sf_cap)
        {
        case 0x200000:
            u1Reg |= 0xC;
            break;
        case 0x400000:
            u1Reg |= 0x8;
            break;
        case 0x800000:
            u1Reg |= 0x4;
            break;
		case 0x1000000:
			u1Reg |= 0x14;
            break;
        default:
            u1Reg |= 0x4;
            break;
        }
    }
#endif
	
	SFLASH_WREG8(SFLASH_CFG1_REG, u1Reg);

	return;
	
}

static u32 mt53xx_nor_GetFlashInfo(SFLASHHW_VENDOR_T *tmp_flash_info)
{
  u32 i = 0;

  while(_aVendorFlash[i].u1MenuID != 0x00)
  {
    if((_aVendorFlash[i].u1MenuID == tmp_flash_info->u1MenuID) &&
       (_aVendorFlash[i].u1DevID1 == tmp_flash_info->u1DevID1) &&
       (_aVendorFlash[i].u1DevID2 == tmp_flash_info->u1DevID2))
    {	
      memcpy((VOID*)tmp_flash_info, (VOID*)&(_aVendorFlash[i]), sizeof(SFLASHHW_VENDOR_T));
      printk(KERN_ERR "Setup flash information successful, support list index: %d\n", i);

      return 0;
    }
	
    i++;
  }

  return -1;
  
}

static u32 mt53xx_nor_GetID(struct mt53xx_nor *mt53xx_nor)
{
  u32 u4Index, u4Ret;
  u8 cmdval;
  SFLASHHW_VENDOR_T *tmp_flash_info;

  mt53xx_nor->flash_num = 0;
  mt53xx_nor->total_sz = 0;
  
  for(u4Index = 0;u4Index < MAX_FLASHCOUNT;u4Index++)
  {
    /* we can not use this sentence firstly because of empty flash_info:
        * cmdval = mt53xx_nor->flash_info[u4Index].u1READIDCmd;
        * so it is necessary to consider read id command as constant.
        */
    cmdval = 0x9F;
    SFLASH_WREG8(SFLASH_PRGDATA5_REG, cmdval);
    SFLASH_WREG8(SFLASH_PRGDATA4_REG, 0x00);
    SFLASH_WREG8(SFLASH_PRGDATA3_REG, 0x00);
    SFLASH_WREG8(SFLASH_PRGDATA2_REG, 0x00);
    SFLASH_WREG8(SFLASH_CNT_REG, 32);

	
    /* pointer to the current flash info struct object
        * Save the current flash index
        */
    tmp_flash_info = mt53xx_nor->flash_info + u4Index;
	mt53xx_nor->cur_flash_index = u4Index;
	mt53xx_nor->cur_cmd_val = 0x04;

	if(0 != mt53xx_nor_ExecuteCmd(mt53xx_nor))
	{
      break;
	}

    tmp_flash_info->u1DevID2 = SFLASH_RREG8(SFLASH_SHREG0_REG);
    tmp_flash_info->u1DevID1 = SFLASH_RREG8(SFLASH_SHREG1_REG);	
    tmp_flash_info->u1MenuID = SFLASH_RREG8(SFLASH_SHREG2_REG);

	mt53xx_nor->cur_cmd_val = 0x00;
    mt53xx_nor_SendCmd(mt53xx_nor);

    printk(KERN_ERR "Flash Index: %d, MenuID: %02x, DevID1: %02x, DevID2: %02x\n", 
                                   u4Index, tmp_flash_info->u1MenuID,
                                            tmp_flash_info->u1DevID1,
                                            tmp_flash_info->u1DevID2);

    u4Ret = mt53xx_nor_GetFlashInfo(tmp_flash_info);
	if(0 == u4Ret)
	{
	    mt53xx_nor->flash_num++;
		mt53xx_nor->total_sz += tmp_flash_info->u4ChipSize;
		
#if (MAX_FLASHCOUNT > 1) 
	    if(u4Index == 0)
	    {
	        /* It is necessary when the first flash is identified sucessfully and
	              *   more than one flash is intended to be supported.
	              */
            mt53xx_nor_SetCfg1(2, tmp_flash_info->u4ChipSize);    
	    }
#endif
	}
	else
	{
      break;
	}
	
  }

  if(0 == mt53xx_nor->flash_num)
  {
    printk(KERN_ERR "any flash is not found!\n");
	return -1;
  }

  mt53xx_nor_SetCfg1(mt53xx_nor->flash_num, mt53xx_nor->flash_info[0].u4ChipSize); 

  return 0;
  
}

static u32 mt53xx_nor_ReadStatus(struct mt53xx_nor *mt53xx_nor, u8 *status)
{
  if(status == NULL)
  {
    return -1;
  }

  mt53xx_nor->cur_cmd_val = 0x02;
  if(-1 == mt53xx_nor_ExecuteCmd(mt53xx_nor))
  {
    printk(KERN_ERR "read status failed!\n");
    return -1;
  }
	
  *status = SFLASH_RREG8(SFLASH_RDSR_REG);

  return 0;
  
}

static u32 mt53xx_nor_WriteStatus(struct mt53xx_nor *mt53xx_nor, u8 status)
{
  SFLASH_WREG8(SFLASH_PRGDATA5_REG, status);
  SFLASH_WREG8(SFLASH_CNT_REG,8);

  mt53xx_nor->cur_cmd_val = 0x20;
  if(-1 == mt53xx_nor_ExecuteCmd(mt53xx_nor))
  {
    printk(KERN_ERR "write status failed!\n");
    return -1;
  }
	  
  return 0;

}

static u32 mt53xx_nor_WaitBusy(struct mt53xx_nor *mt53xx_nor, u32 timeout)
{
  u32 count;
  u8 reg;

  count = 0;
  while(1)
  {
    if(mt53xx_nor_ReadStatus(mt53xx_nor, &reg) != 0)
    {
      return -1;
    }

    if(0 == (reg & 0x1))
    {
      break;
    }

    count++;
    if(count > timeout)
    {
      printk(KERN_ERR "wait write busy timeout, failed!\n");
      return -1;
    }

    udelay(5);
  }

  return 0;
  
}

static u32 mt53xx_nor_WaitBusySleep(struct mt53xx_nor *mt53xx_nor, u32 timeout)
{
  u32 count;
  u8 reg;

  count = 0;
  while(1)
  {
    if(mt53xx_nor_ReadStatus(mt53xx_nor, &reg) != 0)
    {
      return -1;
    }

    if(0 == (reg & 0x1))
    {
      break;
    }

    count++;
    if(count > timeout)
    {
      printk(KERN_ERR "wait write busy timeout, failed!\n");
      return -1;
    }

    msleep(5);
  }

  return 0;
  
}

static u32 mt53xx_nor_WriteEnable(struct mt53xx_nor *mt53xx_nor)
{
  u8 cur_flash_index, val;

  cur_flash_index = mt53xx_nor->cur_flash_index;
  val = mt53xx_nor->flash_info[cur_flash_index].u1WRENCmd;

  SFLASH_WREG8(SFLASH_PRGDATA5_REG, val);
  SFLASH_WREG8(SFLASH_CNT_REG,8);

  mt53xx_nor->cur_cmd_val = 0x04;
  if(-1 == mt53xx_nor_ExecuteCmd(mt53xx_nor))
  {
    printk(KERN_ERR "write enable failed!\n");
    return -1;
  }
	
  return 0;

}

static u32 mt53xx_nor_WriteBufferEnable(struct mt53xx_nor *mt53xx_nor)
{
  u32 polling;
  u8 reg;
  
  SFLASH_WREG8(SFLASH_CFG2_REG, 0x0D);

  polling = 0;
  while(1)
  {
    reg = SFLASH_RREG8(SFLASH_CFG2_REG);
    if (0x01 == (reg & 0x01))
    {
      break;
    }

    polling++;
    if(polling > SFLASH_POLLINGREG_COUNT)
    {
      return -1;
    }
  }

  return 0;
}


static u32 mt53xx_nor_WriteBufferDisable(struct mt53xx_nor *mt53xx_nor)
{
  u32 polling;
  u8 reg;

  SFLASH_WREG8(SFLASH_CFG2_REG, 0xC);
  
  polling = 0;
  while(1)    // Wait for finish write buffer
  {
    reg = SFLASH_RREG8(SFLASH_CFG2_REG);
    if (0x0C == (reg & 0xF))
    {
      break;
    }

    polling++;
    if(polling > SFLASH_POLLINGREG_COUNT)
    {
      return -1;
    }
  }

  return 0;
  
}


static int mt53xx_nor_WriteProtect(struct mt53xx_nor *mt53xx_nor, u32 fgEn)
{  
  u8 val = fgEn?0x3C:0x00;
  
  if(0 != mt53xx_nor_WaitBusy(mt53xx_nor, SFLASH_WRITEBUSY_TIMEOUT))
  {
    printk(KERN_ERR, "wait write busy #1 failed in write protect process!\n");
    return -1;
  }

  if(0 != mt53xx_nor_WriteEnable(mt53xx_nor))
  {
    printk(KERN_ERR, "write enable failed in write protect process!\n");
    return -1;
  }

  if (0 != mt53xx_nor_WriteStatus(mt53xx_nor, val))
  {
    printk(KERN_ERR, "write status failed in write protect process!\n");
    return -1;
  }

  if(mt53xx_nor_WaitBusy(mt53xx_nor, SFLASH_WRITEBUSY_TIMEOUT) != 0)
  {
    printk(KERN_ERR, "wait write busy #2 failed in write protect process!\n");
    return -1;
  }

  return 0;
  
}

static u32 mt53xx_nor_WriteProtectAllChips(struct mt53xx_nor *mt53xx_nor, u32 fgEn)
{ 
  u8 index, cur_index = mt53xx_nor->cur_flash_index;

  for(index = 0; index < mt53xx_nor->flash_num; index++)
  {
    mt53xx_nor->cur_flash_index = index;
    if(0 != mt53xx_nor_WriteProtect(mt53xx_nor, fgEn))
    {
      printk(KERN_ERR, "Flash #%d: write protect disable failed!\n", index);
	  mt53xx_nor->cur_flash_index = cur_index;
	  return -1;
    }
	else
		printk(KERN_ERR, "Flash #%d: write protect disable successfully!\n", index);
  }

  mt53xx_nor->cur_flash_index = cur_index;
  
  return 0;

}

static u32 mt53xx_nor_EraseChip(struct mt53xx_nor *mt53xx_nor)
{
  u8 reg, cur_flash_index, val;
  
  cur_flash_index = mt53xx_nor->cur_flash_index;
  val = mt53xx_nor->flash_info[cur_flash_index].u1CHIPERASECmd;
  printk(KERN_ERR "%s \n",__FUNCTION__);

  if(mt53xx_nor_WaitBusySleep(mt53xx_nor, SFLASH_WRITEBUSY_TIMEOUT) != 0)
  {
    printk(KERN_ERR, "wait write busy #1 failed in erase chip process!\n");
    return -1;
  }
	
  if(mt53xx_nor_WriteEnable(mt53xx_nor) != 0)
  {
    printk(KERN_ERR, "write enable failed in erase chip process!\n");
    return -1;
  }
	
  /* Execute sector erase command
     */
  SFLASH_WREG8(SFLASH_PRGDATA5_REG, val);
  SFLASH_WREG8(SFLASH_CNT_REG, 8);

  mt53xx_nor->cur_cmd_val = 0x04;
  if(-1 == mt53xx_nor_ExecuteCmd(mt53xx_nor))
  {
    printk(KERN_ERR "erase chip#%d failed!\n", cur_flash_index);
    return -1;
  }

  if(mt53xx_nor_WaitBusySleep(mt53xx_nor, SFLASH_CHIPERASE_TIMEOUT) != 0)
  {
    printk(KERN_ERR, "wait write busy #2 failed in erase chip process!\n");
    return -1;
  }

  mt53xx_nor->cur_cmd_val = 0x00;
  mt53xx_nor_SendCmd(mt53xx_nor);
	
  return 0;

}

static u32 mt53xx_nor_EraseAllChips(struct mt53xx_nor *mt53xx_nor)
{
  u8 index, cur_index = mt53xx_nor->cur_flash_index;
	printk(KERN_ERR "%s \n",__FUNCTION__);
  for(index = 0; index < mt53xx_nor->flash_num; index++)
  {
    mt53xx_nor->cur_flash_index = index;
    if(0 != mt53xx_nor_EraseChip(mt53xx_nor))
    {
      printk(KERN_ERR, "Flash #%d: erase chip failed!\n", index);
      mt53xx_nor->cur_flash_index = cur_index;
      return -1;
    }
  }

  mt53xx_nor->cur_flash_index = cur_index;	
  
  return 0;

}

static u32 mt53xx_nor_EraseSector(struct mt53xx_nor *mt53xx_nor, u32 offset)
{
  u32 polling, addr;
  u8 reg, cmdval, index;

  if(0 != mt53xx_nor_WaitBusySleep(mt53xx_nor, SFLASH_WRITEBUSY_TIMEOUT))
  {
    printk(KERN_ERR, "wait write busy #1 failed in erase sector process!\n");
    return -1;
  }

  if(0 != mt53xx_nor_WriteEnable(mt53xx_nor))
  {
    printk(KERN_ERR, "write enable failed in erase sector process!\n");
    return -1;
  }

  if(0 != mt53xx_nor_Offset2Index(mt53xx_nor, offset))
  {
    printk(KERN_ERR "offset parse failed in erase sector process!\n");
	return -1;
  }

  /* Execute sector erase command
     */
  index = mt53xx_nor->cur_flash_index;
  addr = mt53xx_nor->cur_addr_offset;
  cmdval = mt53xx_nor->flash_info[index].u1SECERASECmd;

  SFLASH_WREG8(SFLASH_PRGDATA5_REG, cmdval);
  SFLASH_WREG8(SFLASH_PRGDATA4_REG, LoByte(HiWord(addr)));
  SFLASH_WREG8(SFLASH_PRGDATA3_REG, HiByte(LoWord(addr)));
  SFLASH_WREG8(SFLASH_PRGDATA2_REG, LoByte(LoWord(addr)));
  SFLASH_WREG8(SFLASH_CNT_REG, 32);

  mt53xx_nor->cur_cmd_val = 0x04;
  if(0 != mt53xx_nor_ExecuteCmd(mt53xx_nor))
  {
    printk(KERN_ERR "erase sector(offset:%08x flash#%d, sector index:%d, offset:%08x) failed!\n", 
		                                                      offset, index, 
		                                                      mt53xx_nor->cur_sector_index, 
		                                                      mt53xx_nor->cur_addr_offset);
    return -1;
  }

  if(0 != mt53xx_nor_WaitBusySleep(mt53xx_nor, SFLASH_ERASESECTOR_TIMEOUT))
  {
    printk(KERN_ERR, "wait write busy #2 failed in erase sector process!\n");
    return -1;
  }

  mt53xx_nor->cur_cmd_val = 0x00;
  mt53xx_nor_SendCmd(mt53xx_nor);
	
  return 0;

}

static u32 mt53xx_nor_erase(struct mtd_info *mtd, struct erase_info *instr)
{
  struct mt53xx_nor *mt53xx_nor = mtd_to_mt53xx_nor(mtd);
  u32 addr, len, rem, first_chip_sz, total_sz, ret = 0;
  u8 info;
  
  addr = instr->addr;
  len = instr->len;
  first_chip_sz = mt53xx_nor->flash_info[0].u4ChipSize;
  total_sz = mt53xx_nor->total_sz;
//#if NOR_DEBUG
  printk(KERN_ERR "%s addr=0x%x,len=0x%x\n",__FUNCTION__,instr->addr,instr->len);
//#endif
  if(addr + len > mt53xx_nor->total_sz)
  {
  	return -EINVAL;
  }
  
  rem = len - (len & (~(mtd->erasesize - 1)));
  if(rem)
  {
  	return -EINVAL;
  }

  mutex_lock(&mt53xx_nor->lock);

  /*
     * pimux switch firstly 
     */
  mt53xx_nor_PinMux(); 

  
  /* get rank-flash info of region indicated by addr and len
     */
  info = mt53xx_nor_Region2Info(mt53xx_nor, addr, len);

  /*
     * disable write protect of all related chips 
     */
  if(info & REGION_RANK_FIRST_FLASH)
  {
    mt53xx_nor->cur_flash_index = 0;
	if(0 != mt53xx_nor_WriteProtect(mt53xx_nor, 0))
    {
      printk(KERN_ERR "disable write protect of flash#0 failed!\n");
	  ret = -1;
	  goto done;
    }
  }
  if(info & REGION_RANK_SECOND_FLASH)
  {
    mt53xx_nor->cur_flash_index = 1;
	if(0 != mt53xx_nor_WriteProtect(mt53xx_nor, 0))
    {
      printk(KERN_ERR "disable write protect of flash#1 failed!\n");
	  ret = -1;
	  goto done;
    }
  }  
 
  //printk(KERN_ERR "erase addr: %08x~%08x\n", addr, len);
  
  /* erase the first whole flash if necessary
     */
  if(info & REGION_RANK_FIRST_WHOLE_FLASH)
  {
    mt53xx_nor->cur_flash_index = 0;
    if(mt53xx_nor_EraseChip(mt53xx_nor))
    {
      printk(KERN_ERR "erase whole flash#0 failed!\n");
	  ret = -EIO; 
	  goto done;
    }
	else
	printk(KERN_ERR "erase whole flash#0 successful!\n");

    addr += first_chip_sz;
	len -= first_chip_sz;
  }

  /* erase involved region of the first flash if necessary
     */
  mt53xx_nor->cur_flash_index = 0;
  while((len)&&(addr < first_chip_sz))
  {
    if(mt53xx_nor_EraseSector(mt53xx_nor, addr))
    {
      printk(KERN_ERR "erase part of flash#0 failed!\n");
      ret = -EIO; 
	  goto done;
    }
	else
		printk(KERN_ERR "erase part flash#0 successful!\n");

    addr += mtd->erasesize;
    len -= mtd->erasesize;    
  }

  /* erase the first whole flash if necessary
     */
  if(info & REGION_RANK_SECOND_WHOLE_FLASH)
  {
    mt53xx_nor->cur_flash_index = 1;
    if(mt53xx_nor_EraseChip(mt53xx_nor))
    {
      printk(KERN_ERR "erase whole flash#1 failed!\n");
	  ret = -EIO; 
	  goto done;  
    }
	else
		printk(KERN_ERR "erase whole flash#1 successful!\n");

    addr += first_chip_sz;
	len -= first_chip_sz;
  }

  /* erase involved region of the first flash if necessary
     */
  mt53xx_nor->cur_flash_index = 1;
  while((len)&&(addr < total_sz))
  {
    if(mt53xx_nor_EraseSector(mt53xx_nor, addr))
    {
      printk(KERN_ERR "erase part of flash#1 failed!\n");
      ret = -EIO; 
	  goto done;
    }
	else
	printk(KERN_ERR "erase part flash#1 successful!\n");

    addr += mtd->erasesize;
    len -= mtd->erasesize;    
  }

done:
	
  mutex_unlock(&mt53xx_nor->lock);
  if(ret)
  {
    instr->state = MTD_ERASE_FAILED;
  }
  else
  {
    instr->state = MTD_ERASE_DONE;
    mtd_erase_callback(instr);
  }

  return ret;
}

static u32 mt53xx_nor_ReadPIO(struct mt53xx_nor *mt53xx_nor, u32 addr, u32 len, u32 *retlen, u8 *buf)
{
  u32 i = 0, ret = 0; 
	
  /* why addr3 should be set?
     * if 0xFFFFFF and auto-increment address are survived in the previous read operation,
     * SFLASH_RADR3_REG will be set as 1.
     * if that, 0x1a2800 will mean that 0x9a2800 in the current read operation
     * so, it is better to fill SFLASH_RADR3_REG by high byte in high word
     */
  SFLASH_WREG8(SFLASH_RADR3_REG, HiByte(HiWord(addr)));
  SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(addr)));
  SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(addr)));
  SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(addr))); 
	
  for (i=0; i<len; i++, (*retlen)++)
  {  
    mt53xx_nor->cur_cmd_val = 0x81;
    if(0 != mt53xx_nor_ExecuteCmd(mt53xx_nor))
    {
      printk(KERN_ERR "read flash failed!\n");
      ret = -1;
      goto done;
    }
	
    buf[i] = SFLASH_RREG8(SFLASH_RDATA_REG); // Get data
  } 
	
  mt53xx_nor->cur_cmd_val = 0x00;
  mt53xx_nor_SendCmd(mt53xx_nor);

done:

	return ret;

}

static u32 mt53xx_nor_ReadDMA(struct mt53xx_nor *mt53xx_nor, u32 addr, u32 len, u32 *retlen, u8 *buf)
{ 
  u32 phyaddr; 

  phyaddr = dma_map_single(NULL, buf, len, DMA_FROM_DEVICE);	

  // reset DMA eginee
  SFLASH_DMA_RESET;

  // DMA source address in flash and des address in DRAM in DRAM
  SFLASH_DMA_ADDR_SETTING(addr, phyaddr, len);

  // Trigger DMA eginee
  SFLASH_DMA_START;

  // wait DMA operation is completed
  mt53xx_nor_WaitInt(mt53xx_nor);

  dma_unmap_single(NULL, phyaddr, len, DMA_FROM_DEVICE);	

  // ret value add
  (*retlen) += len;

  return 0;

}

static u32 mt53xx_nor_Read(struct mt53xx_nor *mt53xx_nor, u32 addr, u32 len, u32 *retlen, u8 *buf)
{
  u32 ret = 0, dram_addr_off = 0, len_align = 0, i;
  u8 info, attr;

  attr = mt53xx_nor->host_act_attr;

  mt53xx_nor_PinMux();

  mt53xx_nor_SetDualRead(mt53xx_nor);
	
  /* get rank-flash info of region indicated by addr and len
     */
  info = mt53xx_nor_Region2Info(mt53xx_nor, addr, len);
	
  /* Wait till previous write/erase is done. 
     */
  if(info & REGION_RANK_FIRST_FLASH)
  {
    mt53xx_nor->cur_flash_index = 0;
    if(0 != mt53xx_nor_WaitBusy(mt53xx_nor, SFLASH_WRITEBUSY_TIMEOUT))
    {
      printk(KERN_ERR "disable write protect of flash#0 failed!\n");
      ret = -1;
      goto done;
    }
  }
  if(info & REGION_RANK_SECOND_FLASH)
  {
    mt53xx_nor->cur_flash_index = 1;
    if(0 != mt53xx_nor_WaitBusy(mt53xx_nor, SFLASH_WRITEBUSY_TIMEOUT))
    {
      printk(KERN_ERR "disable write protect of flash#1 failed!\n");
      ret = -1;
      goto done;
    }
  }  
	
  /* read operation is going on
     */
  mt53xx_nor->cur_flash_index = 0;
	
  /* Disable pretch write buffer  
     */
  if(0 != mt53xx_nor_WriteBufferDisable(mt53xx_nor))
  {
    printk(KERN_ERR "disable write buffer in read process failed!\n");
    ret = -1;
    goto done;    
  }


  /* For only pio mode use, we don't care whether dma addr and len is 16byte align
     */
  if(attr & SFLASH_USE_DMA)
  {	  
	  if(virt_addr_valid((u32)buf ))  //buf address can be use for dma
	  {
		  dram_addr_off = (u32)buf - (((u32)buf)&(~0x0F));
		  if(dram_addr_off)
		  {
			  //printk(".......entry mt53xx_nor_Read rdram_addr_off is nor zero!\n");
			  len_align = 0x10 - dram_addr_off;
			  
			  if(len < 0x10 + len_align)
			  {
				  len_align = len;
			  }
				
			  if(0 != mt53xx_nor_ReadPIO(mt53xx_nor, addr, len_align, retlen, buf))
			  {
				  printk(KERN_ERR "read in pio mode #1 failed!\n");
				  ret = -1;
				  goto done;  
			  } 
		  
			  addr += len_align;
			  len  -= len_align;
			  buf  += len_align;
		  } 	  
	  
			
		  if(len >= 0x10)
		  {
			  len_align = (len&(~0xF));    
			  if(0 != mt53xx_nor_ReadDMA(mt53xx_nor, addr, len_align, retlen, buf))
			  {
				  printk(KERN_ERR "read in DMA mode failed!\n");
				  ret = -1;
				  goto done;  
			  }
			  addr += len_align;
			  len  -= len_align;
			  buf  += len_align;
	  
		  
		  }
	  
	  }
	  else		  //buf address can't be use for dma, we must use _pu1NorDataBuf for dma read
	  {  
		  if(len >= 0x10)
		  {
			  len_align = (len&(~0xF));   //len_align is 16 align
			  
			  //printk("..............entry mt53xx_nor_Read len =0x%x len_align =0x%x buf =0x%x !\n",len, len_align, buf);
		  
			  len  -= len_align;
			  //DMA read max size is 8k
			  while(len_align > SFLASH_MAX_DMA_SIZE)	  
			  {
				  if(0 != mt53xx_nor_ReadDMA(mt53xx_nor, addr, SFLASH_MAX_DMA_SIZE, retlen, _pu1NorDataBuf))
				  {
					printk(KERN_ERR "read in DMA mode failed!\n");
					ret = -1;
					goto done;	
				  }
				  memcpy(buf, _pu1NorDataBuf, SFLASH_MAX_DMA_SIZE);
				  addr += SFLASH_MAX_DMA_SIZE;
				  buf += SFLASH_MAX_DMA_SIZE;
				  len_align -= SFLASH_MAX_DMA_SIZE;
			  }
			  if(len_align > 0)   // 16 < len_align < SFLASH_MAX_DMA_SIZE
			  {
				  //printk( "len_align = 0x%x !\n",len_align);
				  if(0 != mt53xx_nor_ReadDMA(mt53xx_nor, addr, len_align, retlen, _pu1NorDataBuf))
				  {
					  printk(KERN_ERR "read in DMA mode failed!\n");
					  ret = -1;
					  goto done;  
				  }
				  memcpy(buf, _pu1NorDataBuf, len_align);
				  addr += len_align;
				  buf += len_align;
			  }
		  }
	  }

  }
 
  if(len > 0)
  {
    len_align = len;
	 
    if(0 != mt53xx_nor_ReadPIO(mt53xx_nor, addr, len_align, retlen, buf))
    {
      printk(KERN_ERR "read in pio mode #2 failed!\n");
      ret = -1;
      goto done;	  
    }   
	  
    addr += len_align;
    len  -= len_align;
    buf += len_align;
  } 

done:

  return ret;

}

static u32 mt53xx_nor_read(struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf)
{
  struct mt53xx_nor *mt53xx_nor = mtd_to_mt53xx_nor(mtd);
  u32 ret = 0, i;

  if (!len)
  {
    return 0;
  }

  if (from + len > mt53xx_nor->mtd.size)
  {
    return -EINVAL;
  }

  /* Byte count starts at zero. */
  *retlen = 0;

  mutex_lock(&mt53xx_nor->lock);
 // printk(KERN_ERR "%s (addr = %08x, len = %08x, retlen = %08x)\n", __FUNCTION__,(u32)from,(u32)len,(u32)(*retlen));

  if(0 != mt53xx_nor_Read(mt53xx_nor, (u32)from, (u32)len, (u32 *)retlen, (u8 *)buf))
  {
    printk(KERN_ERR "read failed(addr = %08x, len = %08x, retlen = %08x)\n", (u32)from,
		                                                                     (u32)len,
		                                                                     (u32)(*retlen)); 
	ret = -1;
  }

done:

  mutex_unlock(&mt53xx_nor->lock);

  return ret;

}

static u32 mt53xx_nor_WaitHostIdle(struct mt53xx_nor *mt53xx_nor)
{
  u32 polling;
  u8 reg;
	
  polling = 0;
  while(1)
  {
    reg = SFLASH_RREG8(SFLASH_SAMPLE_EDGE_REG);
    if (0x00 != (reg & (0x01<<6)))
    {
      break;
    }
	
    polling++;
    if(polling > SFLASH_POLLINGREG_COUNT)
    {
      printk(KERN_ERR "timeout for wait host idle!\n");
      return -1;
    }

	udelay(5);
  }
	
  return 0;

}

static u32 mt53xx_nor_ReadSingleByte(struct mt53xx_nor *mt53xx_nor, u32 addr, u8 *buf)
{
  u32 ret = 0, retlen = 0;
	
  if(addr >= mt53xx_nor->total_sz)
  {
    printk(KERN_ERR "invalid write address exceeds MAX possible address!\n");
	ret = -1;
	goto done;
  }
	
  if(0 != mt53xx_nor_Read(mt53xx_nor, addr, 1, &retlen, buf))
  {
    printk(KERN_ERR "read single byte failed!\n");
	ret = -1;
	goto done; 
  }

done:

  return ret;

}

static u32 mt53xx_nor_WriteSingleByte(struct mt53xx_nor *mt53xx_nor, u32 u4addr, u8 u1data)
{
  u32 addr;
  u8 index;
#if 0
  u8 data;
#endif

  if(u4addr >= mt53xx_nor->total_sz)
  {
    printk(KERN_ERR "invalid write address exceeds MAX possible address!\n");
	return -1;
  }

#if 0
  if(0 != mt53xx_nor_ReadSingleByte(mt53xx_nor, u4addr, &data))
  {
    printk(KERN_ERR "read byte before write failed!\n");
  }
  if(data != 0xFF)
  {
    printk(KERN_ERR "read byte before write value: %02x(addr%08x, target:%02x)\n", data, u4addr, u1data);
	while(1);
  }
#endif

  if(0 != mt53xx_nor_Offset2Index(mt53xx_nor, u4addr))
  {
    printk(KERN_ERR "offset parse failed in write byte process!\n");
	return -1;
  }

  index = mt53xx_nor->cur_flash_index;
  addr = mt53xx_nor->cur_addr_offset;

  SFLASH_WREG8(SFLASH_WDATA_REG, u1data);
  SFLASH_WREG8(SFLASH_RADR3_REG, HiByte(HiWord(addr)));
  SFLASH_WREG8(SFLASH_RADR2_REG, LoByte(HiWord(addr)));
  SFLASH_WREG8(SFLASH_RADR1_REG, HiByte(LoWord(addr)));
  SFLASH_WREG8(SFLASH_RADR0_REG, LoByte(LoWord(addr)));

  mt53xx_nor->cur_cmd_val = 0x10;
  if(0 != mt53xx_nor_ExecuteCmd(mt53xx_nor))
  {
    printk(KERN_ERR "write byte(offset:%08x flash#%d, sector index:%d, offset:%08x) failed!\n", 
		                                                      u4addr, index, 
		                                                      mt53xx_nor->cur_sector_index, 
		                                                      mt53xx_nor->cur_addr_offset);
    return -1;
  }

  mt53xx_nor->cur_cmd_val = 0x00;
  mt53xx_nor_SendCmd(mt53xx_nor);

  if(0 != mt53xx_nor_WaitBusy(mt53xx_nor, SFLASH_WRITEBUSY_TIMEOUT))
  {
    printk(KERN_ERR, "wait write busy failed in write byte process!\n");
    return -1;
  }

#if 0
	if(0 != mt53xx_nor_ReadSingleByte(mt53xx_nor, u4addr, &data))
	{
	  printk(KERN_ERR "read byte after write failed!\n");
	}
	if(data != u1data)
	{
	  printk(KERN_ERR "read byte after write value: %02x(addr%08x, target:%02x)\n", data, u4addr, u1data);
	  while(1);
	}
#endif


  return 0;
  
}

static u32 mt53xx_nor_WriteBuffer(struct mt53xx_nor *mt53xx_nor, 
	                                      u32 u4addr, u32 u4len, const u8 *buf)
{
  u32 i, j, bufidx, data, addr;
  u8 index;

  if(buf == NULL)
  {
    return -1;
  }

  if(0 != mt53xx_nor_Offset2Index(mt53xx_nor, u4addr))
  {
    printk(KERN_ERR "offset parse failed in write buffer process!\n");
	return -1;
  }

  index = mt53xx_nor->cur_flash_index;
  addr = mt53xx_nor->cur_addr_offset;

  bufidx = 0;
  for(i=0; i<u4len; i+=4)
  {
    for(j=0; j<4; j++)
    {
      (*((u8 *)&data + j)) = buf[bufidx];
      bufidx++;
    }
    SFLASH_WREG32(SFLASH_PP_DATA_REG, data);
  }

  mt53xx_nor->cur_cmd_val = 0x10;
  if(0 != mt53xx_nor_ExecuteCmd(mt53xx_nor))
  {
    printk(KERN_ERR "write buffer(offset:%08x flash#%d, sector index:%d, offset:%08x) failed!\n", 
		                                                      u4addr, index, 
		                                                      mt53xx_nor->cur_sector_index, 
		                                                      mt53xx_nor->cur_addr_offset);
    return -1;
  }

  mt53xx_nor->cur_cmd_val = 0x00;
  mt53xx_nor_SendCmd(mt53xx_nor);

  if(0 != mt53xx_nor_WaitBusy(mt53xx_nor, SFLASH_WRITEBUSY_TIMEOUT))
  {
    printk(KERN_ERR, "wait write buffer failed in write buffer process!\n");
    return -1;
  }

  return 0;
  
}

static u32 mt53xx_nor_Write(struct mt53xx_nor *mt53xx_nor, u32 addr, u32 len, u32 *retlen, const u8 *buf)
{
  u32 i, ret = 0;
  u8 info;


  
#if 0
  u32 count, pgalign, j;
#endif

  mt53xx_nor_PinMux();
	  
  /* get rank-flash info of region indicated by addr and len
     */

  info = mt53xx_nor_Region2Info(mt53xx_nor, addr, len);
	
  /*
     * disable write protect of all related chips 
     * make sure all of related flash status is idle
     * make sure flash can be write
     */
  if(info & REGION_RANK_FIRST_FLASH)
  {
    mt53xx_nor->cur_flash_index = 0;
	
    if(0 != mt53xx_nor_WriteProtect(mt53xx_nor, 0))
    {
      printk(KERN_ERR "disable write protect of flash#0 failed!\n");
      ret = -1;
      goto done;
    }

   
    if(0 != mt53xx_nor_WaitBusy(mt53xx_nor, SFLASH_WRITEBUSY_TIMEOUT))
    {
      printk(KERN_ERR "wait write busy of flash#0 failed!\n");
      ret = -1;
      goto done;
    }

    if(0 != mt53xx_nor_WriteEnable(mt53xx_nor))
    {
      printk(KERN_ERR "write enable of flash#0 failed!\n");
      ret = -1;
      goto done;
    }


  }
	  
  if(info & REGION_RANK_SECOND_FLASH)
  {
    mt53xx_nor->cur_flash_index = 1;

    /*
    if(0 != mt53xx_nor_WriteProtect(mt53xx_nor, 0))
    {
      printk(KERN_ERR "disable write protect of flash#1 failed!\n");
      ret = -1;
      goto done;
    }
    */


    if(0 != mt53xx_nor_WaitBusy(mt53xx_nor, SFLASH_WRITEBUSY_TIMEOUT))
    {
      printk(KERN_ERR "wait write busy of flash#1 failed!\n");
      ret = -1;
      goto done;
    }


    if(0 != mt53xx_nor_WriteEnable(mt53xx_nor))
    {
      printk(KERN_ERR "write enable of flash#1 failed!\n");
      ret = -1;
      goto done;
    }	


  } 

#if 0

  /* handle no-buffer-align case */ 
  pgalign = addr % SFLASH_WRBUF_SIZE;
  if(pgalign != 0)
  {
    for(i=0; i<(SFLASH_WRBUF_SIZE - pgalign); i++)
    {
      if(mt53xx_nor_WriteSingleByte(mt53xx_nor, addr, *buf) != 0)
      {
        ret = -1;
        goto done;
      }
      addr++;
      buf++;
      len--;
	  
      if(u4len == 0)
      {
        ret = 0;
        len = (u32)len;
        goto done;		
      }
    }
  }
	
  /* handle buffer case */
  if(mt53xx_nor_WriteBufferEnable(mt53xx_nor) != 0)
  {
    return -1;
  }
  while((u32)len > 0)
  {
    if(len >= SFLASH_WRBUF_SIZE)
    {
      count = SFLASH_WRBUF_SIZE;
    }
    else
    {
      // Not write-buffer alignment
      break;
    }
	
    if(mt53xx_nor_WriteBuffer(mt53xx_nor, addr, count, buf) != 0)
    {
      if(mt53xx_nor_WriteBufferDisable(mt53xx_nor) != 0)
      {
        ret = -1;
      }
      ret = -1;
      goto done;
    }
		
    len -= count;
    addr += count;
    buf += count;
  }
  if(mt53xx_nor_WriteBufferDisable(mt53xx_nor) != 0)
  {
    ret = -1;
    goto done;
  }
	
  /* handle remain case */
  if((INT32)len > 0)
  {
    for(i=0; i<len; i++)
    {
      if(mt53xx_nor_WriteSingleByte(mt53xx_nor, addr, *buf) != 0)
      {
        if(mt53xx_nor_WriteBufferDisable(mt53xx_nor) != 0)
        {
          ret =  -1;
        }
        ret =  -1;
        goto done;
      }
		  
      addr++;
      buf++;
    }
  }
  
#else


  for(i=0; i<((u32)len); i++, (*retlen)++)
  {
    if(mt53xx_nor_WriteSingleByte(mt53xx_nor, addr, *buf) != 0)
    {
      ret = -1;
      goto done;
    }
    addr++;
    buf++;
  }

#endif

done:

	return ret;

}

static u32 mt53xx_nor_write(struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf)
{
  struct mt53xx_nor *mt53xx_nor = mtd_to_mt53xx_nor(mtd);
  u32 addr, u4len, i, count, pgalign, ret = 0, j;
  u8 *u1buf, info;
#if NOR_DEBUG
  struct timeval t0;
#endif
  if (retlen)
    *retlen = 0;
	
  if (!len)
  {
    return 0;
  }
	
  if (to + len > mt53xx_nor->mtd.size)
  {
    return -EINVAL;
  }

  /* Byte count starts at zero. */
  *retlen = 0;

  mutex_lock(&mt53xx_nor->lock);
#if NOR_DEBUG
    printk(KERN_ERR "%s (addr = %08x, len = %08x, retlen = %08x)\n",__FUNCTION__, (u32)to,(u32)len,(u32)(*retlen)); 
	do_gettimeofday(&t0);
	printk(KERN_ERR "[Time trace:%s(%d) time = %ld\n", __FUNCTION__,__LINE__,t0.tv_sec*1000+t0.tv_usec); 
#endif	

  if(0 != mt53xx_nor_Write(mt53xx_nor, (u32)to, (u32)len, (u32 *)retlen, (u8 *)buf))
  {
    printk(KERN_ERR "write failed(addr = %08x, len = %08x, retlen = %08x)\n", (u32)to,
																		      (u32)len,
																			  (u32)(*retlen)); 
    ret = -1;
  }
 #if NOR_DEBUG
  do_gettimeofday(&t0);
  printk(KERN_ERR "[Time trace:%s(%d) time = %ld\n", __FUNCTION__,__LINE__,t0.tv_sec*1000+t0.tv_usec); 
#endif	
done:
	
  mutex_unlock(&mt53xx_nor->lock);

  return ret;
  
}

static struct mt53xx_nor *mt53xx_nor;
//static int mt53xx_nor_read(struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf)
int NORPART_Read(unsigned long long u8Offset, unsigned int u4MemPtr, unsigned int u4MemLen)
{
  int part;
  loff_t offset;
  size_t len, retlen, size;
  static const char *part_parsers[] = {"cmdlinepart", NULL};
  //struct mtd_partition *parts;
  
  // Find part offset and size.
  part = u8Offset >> 32;
  offset = u8Offset & 0xffffffff;
  if (part >= partnum)
  
      return -EINVAL;

  if (offset >= mtdparts[part].size)
      return -EINVAL;

  len = mtdparts[part].size - offset;
  offset += mtdparts[part].offset;
  len = (u4MemLen > len) ? len : u4MemLen;

  // readit
  return mt53xx_nor_read(&mt53xx_nor->mtd, offset, len, &retlen, (void*)u4MemPtr);
}
EXPORT_SYMBOL(NORPART_Read);

static int __devinit mt53xx_nor_RequestIrq(struct mt53xx_nor *mt53xx_nor)
{
  u8 attr = mt53xx_nor->host_act_attr;
  
  if(attr & SFLASH_USE_ISR)
  {
    if(0 != mt53xx_nor_RegIsr(mt53xx_nor))
    {
      printk(KERN_ERR "request flash isr failed!\n");
	  return -1;
    }
	
	mt53xx_nor_SetIsr(mt53xx_nor);
  }

  return 0;
}


//-----------------------------------------------------------------------------
/** add_dynamic_parts()
 */
//-----------------------------------------------------------------------------

static inline int add_dynamic_parts(struct mtd_info *mtd)
{
    static const char *part_parsers[] = {"cmdlinepart", NULL};
    
    INT32 err;

    printk(KERN_INFO "add_dynamic_parts\n");

    partnum = parse_mtd_partitions(mtd, part_parsers, &mtdparts, 0);
    if (partnum <= 0)
    {
        return -EIO;
    }

    err = add_mtd_partitions(mtd, mtdparts, partnum);
    if (err != 0)
    {
        return -EIO;
    }
    return add_mtd_device(&mt53xx_nor->mtd);

	
}

static int __init mt53xx_nor_init(void)
{
  int ret = -ENODEV;
  int result;
  u8 status;
  


  /*
     * malloc the necessary memory space for mt53xx_nor
     */
  //mt53xx_nor = kzalloc(sizeof(mt53xx_nor), GFP_KERNEL);	//error
  mt53xx_nor = kzalloc(sizeof (*mt53xx_nor), GFP_KERNEL);
  if(!mt53xx_nor)
  	return -ENOMEM;

  mt53xx_nor->flash_info = kzalloc(MAX_FLASHCOUNT*sizeof( SFLASHHW_VENDOR_T), GFP_KERNEL);
  //mt53xx_nor->flash_info = kzalloc(MAX_FLASHCOUNT*sizeof(struct SFLASHHW_VENDOR_T), GFP_KERNEL);
  if(!mt53xx_nor->flash_info)
  {
    kfree(mt53xx_nor);
	return -ENOMEM;
  }

  printk(KERN_ERR "<mt53xx_nor_init>hank(protect 0 flash)...\n");

  /*
     * lock initialization
     */
  mutex_init(&mt53xx_nor->lock);

  mt53xx_nor->cur_flash_index = 0;
  mt53xx_nor->cur_cmd_val = 0;

  /*
     * switch pinmux for nor flash
     */
  mt53xx_nor_PinMux();

  /*
     * allow flash controller to send write/write status
     * 
     */
  SFLASH_WREG32(SFLASH_CFG1_REG, 0x20);

  /*
     * wait nor controller idle before identify the emmc
     * 
     */
  mt53xx_nor_WaitHostIdle(mt53xx_nor);

  /*
     * identify the all nor flash and get necessary information 
     */
  mt53xx_nor_GetID(mt53xx_nor);

  /*
     * disable write protect at first 
     */
  mt53xx_nor_WriteProtectAllChips(mt53xx_nor, 0);

  // protect the first Nor flash( partiton: "loader","kernel","root"
   mt53xx_nor->cur_flash_index = 0;
    if(0 != mt53xx_nor_WriteProtect(mt53xx_nor, 1))
    {
        printk(KERN_ERR, "Flash #%d: write protect disable failed!\n", mt53xx_nor->cur_flash_index);
	 
    }
	else
	{
		printk(KERN_ERR, "Flash #%d: write protect enable successfully\n", mt53xx_nor->cur_flash_index);
	}


  /* host action attribute
     */
  mt53xx_nor->host_act_attr = SFLASH_USE_DMA | SFLASH_USE_ISR | SFLASH_USE_DUAL_READ;

  /*
     * initialization the mtd-related variant
     */
  mt53xx_nor->mtd.type = MTD_NORFLASH;
  mt53xx_nor->mtd.writesize = 1;
  mt53xx_nor->mtd.flags = MTD_CAP_NORFLASH;
  mt53xx_nor->mtd.size = mt53xx_nor->total_sz;
  mt53xx_nor->mtd.erasesize = 0x10000;
  mt53xx_nor->mtd.erase = mt53xx_nor_erase;
  mt53xx_nor->mtd.read = mt53xx_nor_read;
  mt53xx_nor->mtd.write = mt53xx_nor_write;
  mt53xx_nor->mtd.owner = THIS_MODULE;
  mt53xx_nor->mtd.name = "mt53xx-nor";
  mt53xx_nor->mtd.priv = NULL;

  
  printk("mt53xx_nor->total_sz =0x%x !\n",mt53xx_nor->total_sz);

  mtdchar_buf = kmalloc(0x200, GFP_KERNEL);
  init_MUTEX(&mtdchar_share);

  /* flash isr request
     */
  mt53xx_nor_RequestIrq(mt53xx_nor);

  /* flash host dual read setting
     */
  mt53xx_nor_SetDualRead(mt53xx_nor);

  /* register mtd partition layout
     */  
  ret = add_dynamic_parts(&mt53xx_nor->mtd);
  return ret;
}

static void __exit mt53xx_nor_exit(void)
{

  kfree(mtdchar_buf);
  mtdchar_buf = NULL;

}

module_init(mt53xx_nor_init);
module_exit(mt53xx_nor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shunli Wang <shunli.wang@mediatek.com>");
MODULE_DESCRIPTION("MTD NOR driver for mediatek DTV boards");

