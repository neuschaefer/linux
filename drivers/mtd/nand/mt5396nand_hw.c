/*
 *  drivers/mtd/nand/mt5365nand_hw.c
 *
 *  This is the MTD driver for MT5365 NAND controller
 *
 *
 * Licensed under the GPL
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html#TOC1
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile:  $
 * $Revision: #1 $
 *
 */

#include <linux/slab.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/dma-mapping.h>
#include <linux/jiffies.h>
#include <linux/semaphore.h>

#include <x_typedef.h>
#include <asm/io.h>
#include <mach/hardware.h>
#include <crypto/mtk_crypto.h>

#include "mt5396nand_hw.h"

#define NAND_DEBUG_PRINT            (0)

#define NAND_DMA_CACHE_LINE         (32)
#define NAND_DMA_READ_VERIFY        (0)
#define NAND_DMA_PATTERN            (0xFEDCBA98)
#define NAND_DMA_RETYR_DELAY        (2)
#define NAND_DMA_RETYR_CNT          (50)

static UINT32 _NAND_busy_count = 0;

// nand isr used
UINT32 mtd_nand_isr_en = 1;
static struct completion comp;

//static struct completion comp;
struct semaphore nand_pin_share;

// MTD structure for MT5365 board
struct mtd_info *mt53xx_mtd = NULL;

// For mtdchar.c and mtdsdm.c used 
extern char *mtdchar_buf;
extern struct semaphore mtdchar_share;

// Partition encypt information.
mtd_enc_info_t rMTDPartENC[MAX_MTD_DEVICES];

// static variables
static BOOL _fgDmaEn = TRUE;
static NAND_FLASH_DEV_T *_prFlashDev;
static UINT32 _u4BlkPgCount;
static UINT32 _u4FdmNum;
static UINT32 _u4SectorNum;
static UINT32 _u4PgAddrLen;
static UINT32 _u4TotalAddrLen;
static UINT32 _u4PageSize = 2048;
static UINT32 _u4OOBSize = 16;
static UINT32 _u4SectorSize = 1024;
static UINT32 _u4ECCBits = 0;
static UINT32 _u4CurBitFlip = 0;
static UINT32 _u4CurPage = 0xFFFFFFFF;

static UINT32 _u4NandDataBuf[(NAND_MAX_PAGE_SIZE + 64)/4];
static UINT32 _u4NandOobBuf[NAND_MAX_OOB_SIZE/4];

static UINT8 *_pu1NandDataBuf = NULL;
static UINT8 *_pu1NandOobBuf  = NULL;

static irqreturn_t mtd_nand_isr(int irq, void *dev_id)
{
    u32 intr_status = NAND_READ32(NAND_NFI_INTR);
    if(intr_status == 0)
    {
        printk(KERN_CRIT "curious mtd_nand_irq!\n");
    }

    complete(&comp);
    return IRQ_HANDLED;
}

static int mtd_nand_regisr(void)
{
    NAND_READ32(NAND_NFI_INTR);  // Clear interrupt, read clear

    if (request_irq(VECTOR_NAND, mtd_nand_isr, 0, "NAND DMA", NULL) != 0)
    {
        printk(KERN_ERR "Request NAND IRQ fail!\n");
        return -1;
    }

    NAND_READ32(NAND_NFI_INTR);  // Clear interrupt, read clear
    disable_irq(VECTOR_NAND);
    return 0;
}

void mtd_nand_setisr(UINT32 u4Mask)
{
    if (mtd_nand_isr_en)
    {
        enable_irq(VECTOR_NAND);
        NAND_WRITE32(NAND_NFI_INTR_EN, u4Mask);

        init_completion(&comp);
    }
}

void mtd_nand_waitisr(UINT32 u4Mask)
{
    if (mtd_nand_isr_en)
    {
        wait_for_completion(&comp);

        NAND_WRITE32(NAND_NFI_INTR_EN, 0);
        disable_irq(VECTOR_NAND);
    }
}

EXPORT_SYMBOL(mtd_nand_setisr);
EXPORT_SYMBOL(mtd_nand_waitisr);

//-----------------------------------------------------------------------------
/** NANDHW_GetDevCount()
 */
//-----------------------------------------------------------------------------
static UINT32 _NANDHW_GetDevCount(void)
{
    return sizeof(_arNand_DevInfo) / sizeof(NAND_FLASH_DEV_T);
}

//-----------------------------------------------------------------------------
/** NANDHW_GetDev()
 */
//-----------------------------------------------------------------------------
static NAND_FLASH_DEV_T* _NANDHW_GetDev(UINT32 u4Idx)
{
    UINT32 u4DevCount;

    u4DevCount = _NANDHW_GetDevCount();

    if(u4Idx >= u4DevCount)
    {
        return NULL;
    }

    return (((NAND_FLASH_DEV_T *)_arNand_DevInfo) + u4Idx);
}

//-----------------------------------------------------------------------------
/** _NANDHW_countbits()
 */
//-----------------------------------------------------------------------------
static UINT32 _NANDHW_countbits(UINT32 byte)
{
    UINT32 res = 0;

    for (; byte; byte >>= 1)
    {
        res += (byte & 0x01);
    }

    return res;
}

//-----------------------------------------------------------------------------
/** _NANDHW_WaitRead()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_WaitRead(UINT32 u4WaitNum)
{
    UINT32 u4Val;

    do
    {
        u4Val = NAND_READ32(NAND_NFI_FIFOSTA);
        u4Val &= NAND_NFI_FIFOSTA_RD_REMAIN;
    } while (u4Val < u4WaitNum);
}

//-----------------------------------------------------------------------------
/** _NANDHW_WaitWrite()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_WaitWrite(void)
{
    UINT32 u4Val;

    do
    {
        u4Val = NAND_READ32(NAND_NFI_FIFOSTA);
        u4Val &= NAND_NFI_FIFOSTA_WT_EMPTY;
    } while (u4Val == 0);
}

//-----------------------------------------------------------------------------
/** _NANDHW_WaitEccDone() must wait done
 */
//-----------------------------------------------------------------------------
static void _NANDHW_WaitEccDone(UINT32 u4SectIdx)
{
    UINT32 u4Val;

    do
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECDONE);
        u4Val = (u4Val >> u4SectIdx) & 0x1;
    } while (u4Val == 0);
}

//-----------------------------------------------------------------------------
/** _NANDHW_WaitBusy()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_WaitBusy(void)
{
    u_long timeo;
    UINT32 u4Val;

    _NAND_busy_count = 0;
    timeo = jiffies + HZ;
    do
    {
        _NAND_busy_count++;
        if (time_after(jiffies, timeo))
        {
            u4Val = NAND_READ32(NAND_NFI_STA);
            u4Val &= 0xFF000DFF;
            if (u4Val == 0)
            {
                break;
            }

            printk(KERN_ERR "NAND driver wait timeout! NAND_NFI_STA = 0x%08x\n", u4Val);
        }

        u4Val = NAND_READ32(NAND_NFI_STA);
        u4Val &= 0xFF000DFF;
    } while (u4Val != 0);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_WaitBusy()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIECC_WaitBusy(void)
{
    UINT32 u4Val;

    do
    {
        u4Val = NAND_READ32(NAND_NFIECC_ENCIDLE);
    } while (u4Val == 0);

    do
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECIDLE);
    } while (u4Val == 0);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_WaitSectorCnt()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_WaitSectorCnt(void)
{
    UINT32 u4Val, u4Cnt;

    do
    {
        u4Val = NAND_READ32(NAND_NFI_ADDRCNTR);
        u4Val &= 0xF000;
        u4Cnt = NAND_NFI_ADDRCNTR_SEC_CNTR(_u4SectorNum);
    } while (u4Val != u4Cnt);
}

//-----------------------------------------------------------------------------
/** _NANDHW_Command()
 */
//-----------------------------------------------------------------------------
#if defined(CC_AC_DROP_GPIO_ISR)
UINT8 _bACDropISR = 0;
EXPORT_SYMBOL(_bACDropISR);
#endif

static void _NANDHW_Command(UINT32 u4Cmd)
{
#if defined(CC_AC_DROP_GPIO_ISR)
    if (u4Cmd == NAND_CMD_SEQIN || u4Cmd == NAND_CMD_ERASE1 || u4Cmd == NAND_CMD_READ0)
    {
        UINT32 u4Val = NAND_IO_READ32(REG_ACDROP_GPIO);

        if ((u4Val & REG_ACDROP_GPIOMASK) == 0)
        {
            printk(KERN_CRIT "Turn off nand access(native command:0x%x), detect AC drop GPIO!\n", u4Cmd);
            while (1);
        }

        if (_bACDropISR)
        {
            printk(KERN_CRIT "Turn off nand access(native command:0x%x), detect AC drop ISR!\n", u4Cmd);
            while (1);
        }
    }
#endif

    NAND_WRITE32(NAND_NFI_CMD, u4Cmd);
    _NANDHW_WaitBusy();
}

//-----------------------------------------------------------------------------
/** _NANDHW_Addr()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_Addr(UINT32 u4RowAddr, UINT32 u4ColAddr, UINT32 u4ByteCount)
{
    if (_prFlashDev != NULL)
    {
       if (u4RowAddr >= (_prFlashDev->u4ChipSize/_prFlashDev->u4PageSize))
       {
          printk(KERN_CRIT "page index = 0x%x exceeds nand flash page range,page range from 0 to 0x%x\n", u4RowAddr,
		  	     (_prFlashDev->u4ChipSize/_prFlashDev->u4PageSize)-1);
          BUG();
       }
    }
    NAND_WRITE32(NAND_NFI_COLADDR, u4ColAddr);
    NAND_WRITE32(NAND_NFI_ROWADDR, u4RowAddr);
    NAND_WRITE32(NAND_NFI_ADDRNOB, u4ByteCount);
    _NANDHW_WaitBusy();
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_Reset()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_Reset(void)
{
    NAND_WRITE32(NAND_NFI_CNFG, 0);
    NAND_WRITE32(NAND_NFI_CON, NAND_NFI_CON_NFI_RST | NAND_NFI_CON_FIFO_FLUSH);
    _NANDHW_WaitBusy();
}

//-----------------------------------------------------------------------------
/** _NANDHW_Reset()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_Reset(void)
{
    _NANDHW_NFI_Reset();
    _NANDHW_Command(NAND_CMD_RESET);

    _NANDHW_WaitBusy();
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_Reset()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIECC_Reset(void)
{
    NAND_WRITE32(NAND_NFIECC_ENCON, 0);
    NAND_WRITE32(NAND_NFIECC_DECCON, 0);
    _NANDHW_NFIECC_WaitBusy();
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_SetPageFmt()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_SetPageFmt(void)
{
    UINT32 u4Val = 0;
    _u4FdmNum = 8;

    if (_u4OOBSize == 16)
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_16;
    }
    else
    {
        u4Val = NAND_NFI_PAGEFMT_SPARE_26;
    }

    if (_u4PageSize == 4096)
    {
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_4k;
    }
    else if (_u4PageSize == 8192)
    {
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_8k;
    }
    else
    {
        u4Val |= NAND_NFI_PAGEFMT_PAGE_SIZE_512_2k;
    }

    u4Val |= NAND_NFI_PAGEFMT_FDM_NUM(8) | NAND_NFI_PAGEFMT_ECC_NUM(8);

    NAND_WRITE32(NAND_NFI_PAGEFMT, u4Val);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_Cfg()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_Cfg(nand_ecc_modes_t rECCType, BOOL fgDmaEn, UINT32 u4OpMode)
{
    UINT32 u4Val = 0;

    if(rECCType == NAND_ECC_HW)
    {
        u4Val = NAND_NFI_CNFG_AUTO_FMT_EN | NAND_NFI_CNFG_HW_ECC_EN;
    }
    else if(rECCType == NAND_ECC_SOFT)
    {
        u4Val = NAND_NFI_CNFG_HW_ECC_EN;
    }
    else
    {
        u4Val = 0;
    }

    if(fgDmaEn)
    {
        u4Val |= (NAND_NFI_CNFG_AUTO_FMT_EN | NAND_NFI_CNFG_AHB_MODE);
    }

    switch(u4OpMode)
    {
    case NAND_NFI_CNFG_OP_IDLE:
        NAND_WRITE32(NAND_NFI_CNFG, 0);
        return;

    case NAND_NFI_CNFG_OP_SINGLE:
    case NAND_NFI_CNFG_OP_READ:
        u4Val |= (NAND_NFI_CNFG_READ_MODE | u4OpMode);
        break;

    case NAND_NFI_CNFG_OP_PROGRAM:
    case NAND_NFI_CNFG_OP_ERASE:
    case NAND_NFI_CNFG_OP_RESET:
    case NAND_NFI_CNFG_OP_CUSTOM:
        u4Val |= u4OpMode;
        break;

    default:
        break;
    }

    if(_u4PageSize == 512)
    {
        u4Val |= NAND_NFI_CNFG_SEL_SEC_512BYTE;
    }

    NAND_WRITE32(NAND_NFI_CNFG, u4Val);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_Cfg()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIECC_Cfg(nand_ecc_modes_t rECCType)
{
    UINT32 u4EncMsg, u4DecMsg;

    if (_u4OOBSize == 16)
    {
        if (_u4PageSize == 512)
        {
            _u4ECCBits = 4;
            u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_4 | NAND_NFIECC_ENCCNFG_ENC_MS_520;
            u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_4 | NAND_NFIECC_DECCNFG_DEC_CS_520_4;
        }
        else
        {
            _u4ECCBits = 12;
            u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_12 | NAND_NFIECC_ENCCNFG_ENC_MS_1032;
            u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_12 | NAND_NFIECC_DECCNFG_DEC_CS_1032_12;
        }
    }
    else
    {
        if (_u4PageSize == 512)
        {
            _u4ECCBits = 10;
            u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_10 | NAND_NFIECC_ENCCNFG_ENC_MS_520;
            u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_10 | NAND_NFIECC_DECCNFG_DEC_CS_520_10;
        }
        else
        {
            _u4ECCBits = 24;
            u4EncMsg = NAND_NFIECC_ENCCNFG_ENC_TNUM_24 | NAND_NFIECC_ENCCNFG_ENC_MS_1032;
            u4DecMsg = NAND_NFIECC_DECCNFG_DEC_TNUM_24 | NAND_NFIECC_DECCNFG_DEC_CS_1032_24;
        }
    }

    u4EncMsg |= NAND_NFIECC_ENCCNFG_ENC_NFI_MODE;
    u4DecMsg |= NAND_NFIECC_DECCNFG_DEC_CS_EMPTY_EN | NAND_NFIECC_DECCNFG_DEC_NFI_MODE;

    if (rECCType == NAND_ECC_HW)
    {
        u4DecMsg |= NAND_NFIECC_DECCNFG_DEC_CON_AUTO;
    }
    else if (rECCType == NAND_ECC_SOFT)
    {
        u4DecMsg |= NAND_NFIECC_DECCNFG_DEC_CON_SOFT;
    }
    else
    {
        u4DecMsg |= NAND_NFIECC_DECCNFG_DEC_CON_NONE;
    }

    NAND_WRITE32(NAND_NFIECC_ENCCNFG, u4EncMsg);
    NAND_WRITE32(NAND_NFIECC_DECCNFG, u4DecMsg);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_Trig()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_Trig(UINT32 u4OpMode)
{
    UINT32 u4Val;

    switch(u4OpMode)
    {
    case NAND_NFI_CNFG_OP_SINGLE:
        u4Val = NAND_NFI_CON_NOB(4) | NAND_NFI_CON_SRD;
        break;

    case NAND_NFI_CNFG_OP_READ:
        u4Val = NAND_NFI_CON_SEC_NUM(_u4SectorNum) | NAND_NFI_CON_BRD;
        break;

    case NAND_NFI_CNFG_OP_PROGRAM:
        u4Val = NAND_NFI_CON_SEC_NUM(_u4SectorNum) | NAND_NFI_CON_BWR;
        break;

    case NAND_NFI_CNFG_OP_CUSTOM:
        u4Val = NAND_NFI_CON_SEC_NUM(_u4SectorNum) | NAND_NFI_CON_BWR;
        NAND_WRITE32(NAND_NFI_CON, u4Val);
        NAND_WRITE32(NAND_NFI_STRDATA, NAND_NFI_STRDATA_STRDATA);
        return;

    default:
        u4Val = 0;
        break;
    }

    NAND_WRITE32(NAND_NFI_CON, u4Val);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_Trig()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIECC_Trig(UINT32 u4OpMode)
{
    switch(u4OpMode)
    {
    case NAND_NFI_CNFG_OP_READ:
        NAND_WRITE32(NAND_NFIECC_DECCON, 1);
        break;

    case NAND_NFI_CNFG_OP_PROGRAM:
        NAND_WRITE32(NAND_NFIECC_ENCON, 1);
        break;

    default:
        break;
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_CheckOpMode()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFI_CheckOpMode(UINT32 u4OpMode)
{
    UINT32 i, u4Val;

    for(i = 0; i < 3; i++)
    {
        switch(u4OpMode)
        {
        case NAND_NFI_CNFG_OP_READ:
            u4Val = NAND_READ32(NAND_NFI_STA);
            u4Val &= NAND_NFI_STA_NFI_FSM;
            if(u4Val == NAND_NFI_STA_NFI_FSM_READ_DATA)
            {
                return NAND_SUCC;
            }
            break;

        default:
            return NAND_INVALID_ARG;
        }
    }

    return NAND_CMD_ERROR;
}

//-----------------------------------------------------------------------------
/** _NANDHW_ReadID()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_ReadID(UINT32 *pu4ID)
{
    _NANDHW_NFI_Reset();

    _NANDHW_Command(NAND_CMD_READID);
    _NANDHW_Addr(0, 0, 1);

    _NANDHW_NFI_Cfg(NAND_ECC_NONE, 0, NAND_NFI_CNFG_OP_SINGLE);
    _NANDHW_NFI_Trig(NAND_NFI_CNFG_OP_SINGLE);

    _NANDHW_WaitBusy();
    _NANDHW_WaitRead(4);
    *pu4ID = NAND_READ32(NAND_NFI_DATAR);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_WriteFifo()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_WriteFifo(UINT8 *pu1Buf, UINT32 u4Len)
{
    UINT32 i;
    UINT32 *pu4Buf = (UINT32 *)pu1Buf;

    for(i = 0; i < (u4Len>>2); i++)
    {
        _NANDHW_WaitWrite();
        if(pu1Buf)
        {
            NAND_WRITE32(NAND_NFI_DATAW, pu4Buf[i]);
        }
        else
        {
            NAND_WRITE32(NAND_NFI_DATAW, (UINT32)0xFFFFFFFF);
        }
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_DataExchange()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_DataExchange(UINT32 *pu4Buf, UINT32 *pu4Spare, UINT32 u4PgOff, UINT32 u4Len)
{
    UINT32 u4Val, u4Addr;
    if(!pu4Buf)
    {
        return NAND_FAIL;
    }

    if(_u4OOBSize == 16)
    {
        if(_u4PageSize == 0x800)
        {
            u4Addr = 0x7E0;
        }
        else if(_u4PageSize == 0x1000)
        {
            u4Addr = 0xFA0;
        }
        else if(_u4PageSize == 0x2000)
        {
            u4Addr = 0x1F20;
        }
        else
        {
            return NAND_SUCC;
        }
    }
    else if(_u4OOBSize == 26)
    {
        if(_u4PageSize == 0x800)
        {
            u4Addr = 0x7CC;
        }
        else if(_u4PageSize == 0x1000)
        {
            u4Addr = 0xF64;
        }
        else if(_u4PageSize == 0x2000)
        {
            u4Addr = 0x1E94;
        }
        else
        {
            return NAND_SUCC;
        }
    }
    else
    {
        return NAND_FAIL;
    }

    if((u4Addr >= u4PgOff) && (u4Addr < (u4PgOff + u4Len)))
    {
        u4Val = pu4Buf[(u4Addr-u4PgOff)>>2];
        pu4Buf[(u4Addr-u4PgOff)>>2] = pu4Spare[0];
        pu4Spare[0] = u4Val;
    }

    return NAND_SUCC;
}

//-----------------------------------------------------------------------------
/** _NANDHW_ProbeDev()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_ProbeDev(void)
{
    NAND_FLASH_DEV_T *prDev;
    UINT32 u4ID, i, u4DevCount;

    _NANDHW_ReadID(&u4ID);
    printk(KERN_INFO "Detect NAND flash ID: 0x%X\n", u4ID);

    u4DevCount = _NANDHW_GetDevCount();
    for(i = 0; i < u4DevCount; i++)
    {
        prDev = (NAND_FLASH_DEV_T *)_NANDHW_GetDev(i);
        if((u4ID & prDev->u4IDMask) == (prDev->u4IDVal & prDev->u4IDMask))
        {
            _prFlashDev = prDev;
            break;
        }
    }

    if(i == u4DevCount)
    {
        _prFlashDev = NULL;
        return NAND_FAIL;
    }

    return NAND_SUCC;
}

//-----------------------------------------------------------------------------
/** _NANDHW_SetSectorSize()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_SetSectorSize(void)
{
    if(_u4PageSize > 512)
    {
        _u4SectorSize = 1024;
    }
    else
    {
        _u4SectorSize = 512;
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_Init()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_Init(void)
{
    UINT32 i, u4Val;

    // Set nand/nor pinmux.
    u4Val = NAND_READ32(NAND_NFI_MISC);
    u4Val |= NAND_NFI_MISC_FLASH_PMUX;
    NAND_WRITE32(NAND_NFI_MISC, u4Val);

    _u4BlkPgCount = 64;
    _prFlashDev = NULL;
    _u4PgAddrLen  = 3;
    _u4TotalAddrLen = 5;
    _u4PageSize = 2048;
    _u4OOBSize = 16;

    for(i = 0; i < 3; i++)
    {
        // Reset NFI controller
        _NANDHW_Reset();

        if(_NANDHW_ProbeDev() == NAND_SUCC)
        {
            printk(KERN_INFO "Detect %s NAND flash: %dMB\n", 
                _prFlashDev->acName, (_prFlashDev->u4ChipSize/0x100000));

            _u4PageSize = _prFlashDev->u4PageSize;
            _u4OOBSize =  _prFlashDev->u4OOBSize;

            _NANDHW_SetSectorSize();
            _NANDHW_NFI_Cfg(NAND_ECC_NONE, 0, NAND_NFI_CNFG_OP_SINGLE);
            _NANDHW_NFIECC_Cfg(NAND_ECC_NONE);

            if(_prFlashDev->u4PageSize == 512)
            {
                _u4PgAddrLen = (UINT32)(_prFlashDev->u1AddrCycle - 1);
                _u4TotalAddrLen = _prFlashDev->u1AddrCycle;
            }
            else
            {
                _u4PgAddrLen = (UINT32)(_prFlashDev->u1AddrCycle - 2);
                _u4TotalAddrLen = _prFlashDev->u1AddrCycle;
            }

            _u4BlkPgCount = _prFlashDev->u4BlkPgCount;
            break;
        }

        printk(KERN_ERR "Not detect any flash!\n");
        _u4PgAddrLen = 2;
        _u4TotalAddrLen = 5;
        _u4PageSize = 2048;
        _u4OOBSize = 16;
    }

    mtd_nand_regisr();

    _NANDHW_NFI_SetPageFmt();
    _u4SectorNum = _u4PageSize/_u4SectorSize;
    
    // Aligned the buffer for DMA used.
    _pu1NandDataBuf = (UINT8 *)((((UINT32)_u4NandDataBuf >> 6) << 6) + 64);
    _pu1NandOobBuf  = (UINT8 *)_u4NandOobBuf;

#ifdef CC_MTD_ENCRYPT_SUPPORT
    NAND_AES_INIT();
#endif

    return NAND_SUCC;
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_GetEcc()
 */
//-----------------------------------------------------------------------------
static UINT32 _NANDHW_NFIECC_GetEcc(UINT32 *pu4EccCode)
{
    UINT32 u4Val, i;
    UINT32 u4EccCodeBit, u4EccCodeDWord, u4EccModBit;

    // the redundant bit of parity bit must be 1 for rom code bug. -- 20110328 mtk40109
    u4EccCodeBit = _u4ECCBits * NAND_NFIECC_1BIT;
    u4EccModBit = u4EccCodeBit % 32;
    u4EccCodeDWord = u4EccModBit ? ((u4EccCodeBit>>5) + 1) : (u4EccCodeBit>>5);

    do
    {
        u4Val = NAND_READ32(NAND_NFIECC_ENCSTA);
        u4Val &= NAND_NFIECC_ENCSTA_FSM_MASK;
    } while (u4Val != NAND_NFIECC_ENCSTA_FSM_PAROUT);

    for (i = 0; i < u4EccCodeDWord; i++)
    {
        pu4EccCode[i] = NAND_READ32(NAND_NFIECC_ENCPAR0 + (i<<2));
    }

    for (i = u4EccCodeDWord; i < NAND_NFI_ENCPAR_NUM; i++)
    {
        pu4EccCode[i] = 0xFFFFFFFF;
    }

    if (u4EccModBit)
    {
        pu4EccCode[u4EccCodeDWord - 1] |= (~(UINT32)((1<<u4EccModBit) - 1));
    }

    return NAND_SUCC;
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_GetErrNum()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFIECC_GetErrNum(UINT32 u4SectIdx, UINT32 *pu4ErrNum)
{
    UINT32 u4Val;

    if(u4SectIdx < 4)
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECENUM0);
        *pu4ErrNum = (u4Val >> (u4SectIdx*8)) & 0x1F;
    }
    else
    {
        u4Val = NAND_READ32(NAND_NFIECC_DECENUM1);
        *pu4ErrNum = (u4Val >> ((u4SectIdx-4)*8)) & 0x1F;
    }

    if(*pu4ErrNum == 0x1F)
    {
        return NAND_ECC_ERROR;
    }
    else
    {
        return NAND_SUCC;
    }
}

static INT32 _NANDHW_NFIECC_CorrectEmptySector(UINT8 *buf, UINT8 *spare, UINT32 u4SectIdx)
{
    UINT32 i, u4Val, u4SectOobSz, u4bitflip = 0;
    UINT8 *pu1Data, *pu1Spare;
    UINT32 *pu4Data, *pu4Spare;

    u4Val = NAND_READ32(NAND_NFIECC_DECFER);
    if ((u4Val & (1 << u4SectIdx)) == 0)
    {
        return NAND_SUCC;
    }

    u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;

    pu1Data = buf + u4SectIdx * _u4SectorSize;
    pu1Spare = spare + u4SectIdx * u4SectOobSz;
    pu4Data = (UINT32 *)pu1Data;
    pu4Spare = (UINT32 *)pu1Spare;

    for (i = 0; i < _u4SectorSize/4; i++)
    {
        u4bitflip += (32 - _NANDHW_countbits(pu4Data[i]));
    }

    for (i = 0; i < _u4FdmNum/4; i++)
    {
        u4bitflip += (32 - _NANDHW_countbits(pu4Spare[i]));
    }

    if (u4bitflip <= _u4ECCBits)
    {
        memset(pu1Data, 0xFF, _u4SectorSize);
        memset(pu1Spare, 0xFF, _u4FdmNum);

        if (u4bitflip > (_u4ECCBits/2))
        {
            _u4CurBitFlip += u4bitflip;
        }

        //printk(KERN_INFO "Correctable ECC: %d bit-flip found in empty page 0x%X, Sector %d\n", u4bitflip, _u4CurPage, u4SectIdx);
        return NAND_SUCC;
    }
    else
    {
        printk(KERN_ERR "Un-Correctable ECC found in page 0x%X, Sector %d\n", _u4CurPage, u4SectIdx);
        return NAND_ECC_ERROR;
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_CorrectEcc()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFIECC_CorrectEcc(UINT8 *buf, UINT8 *spare, UINT32 u4SectIdx)
{
    INT32 i4Ret;
    UINT32 i, u4Val, u4SpareOff, u4SectOobSz;
    UINT32 u4ErrNum, u4ErrFound, u4ErrByte, u4ErrBit;

    _NANDHW_WaitEccDone(u4SectIdx); // must wait done

    if (_NANDHW_NFIECC_GetErrNum(u4SectIdx, &u4ErrNum))
    {
        i4Ret = _NANDHW_NFIECC_CorrectEmptySector(buf, spare, u4SectIdx);
        return i4Ret;
    }

    u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;
    u4ErrFound = NAND_READ32(NAND_NFIECC_DECFER);
    if((u4ErrFound >> u4SectIdx) & 0x01)
    {
        for(i = 0; i < u4ErrNum; i++)
        {
            u4Val = NAND_READ16(NAND_NFIECC_DECEL0 + (i*2));
            u4ErrByte = u4Val >> 3;
            u4ErrBit = u4Val & 0x7;

            if(u4ErrByte < _u4SectorSize)
            {
                // Data area bit error.
                u4ErrByte = (u4Val>>3) + (u4SectIdx*_u4SectorSize);
                buf[u4ErrByte] = buf[u4ErrByte] ^ (((UINT32)1)<<u4ErrBit);
            }
            else if(u4ErrByte < (u4SectOobSz+ _u4SectorSize))
            {
                // Spare area bit error.
                if(spare)
                {
                    u4SpareOff = u4ErrByte - _u4SectorSize + u4SectOobSz*u4SectIdx;
                    spare[u4SpareOff] = spare[u4SpareOff] ^ (((UINT32)1)<<u4ErrBit);
                }
            }
            else
            {
                printk(KERN_ERR "Un-Correctable ECC found in page = 0x%X, Sector = 0x%X, Loc = 0x%X\n", _u4CurPage, u4SectIdx, u4Val);
                return NAND_ECC_ERROR;
            }
        }
    }

    if (u4ErrNum > (_u4ECCBits/2))
    {
        _u4CurBitFlip += u4ErrNum;
    }

    return NAND_SUCC;
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_CheckEccDone()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFIECC_CheckEccDone(void)
{
    UINT32 u4SectIdx;
    
    for (u4SectIdx = 0; u4SectIdx < _u4SectorNum; u4SectIdx++)
    {
        _NANDHW_WaitEccDone(u4SectIdx);
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFIECC_CheckEcc()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFIECC_CheckEcc(UINT8 *buf, UINT8 *spare)
{
    UINT32 u4SectIdx, u4ErrNum;
    INT32 i4Ret = NAND_SUCC;

    for (u4SectIdx = 0; u4SectIdx < _u4SectorNum; u4SectIdx++)
    {
        if (_NANDHW_NFIECC_GetErrNum(u4SectIdx, &u4ErrNum))
        {
            i4Ret |= _NANDHW_NFIECC_CorrectEmptySector(buf, spare, u4SectIdx);
        }
        else if(u4ErrNum != 0)
        {
            if (u4ErrNum > (_u4ECCBits/2))
            {
                _u4CurBitFlip += u4ErrNum;
            }
            //printk(KERN_INFO "Correctable ECC: %d bit-flip found in page 0x%X, Sector %d\n", u4ErrNum, _u4CurPage, u4SectIdx);
        }
    }

    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_SetRdDma()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_SetRdDma(UINT32 *pu4DataAddr)
{
    UINT32 u4PhyAddr;

    u4PhyAddr = virt_to_phys((void *)pu4DataAddr);
    u4PhyAddr += NAND_DRAM_BASE;
    NAND_WRITE32(NAND_NFI_STRADDR, u4PhyAddr);
    NAND_WRITE32(NAND_NFIECC_FDMADDR, NAND_REG_BASE + NAND_NFI_FDM0L);
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_SetWtDma()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_SetWtDma(UINT32 *pu4DataAddr, UINT32 *pu4SpareAddr)
{
    UINT32 i, j, u4SectOobSz, u4PhyAddr;
    UINT32 *pu4SectSpare;

    u4PhyAddr = virt_to_phys((void *)pu4DataAddr);
    u4PhyAddr += NAND_DRAM_BASE;
    NAND_WRITE32(NAND_NFI_STRADDR, u4PhyAddr);

    u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;
    for (i = 0; i < _u4SectorNum; i++)
    {
        pu4SectSpare = (UINT32 *)((UINT32)pu4SpareAddr + u4SectOobSz * i);

        for (j = 0; j < _u4FdmNum/4; j++)
        {
            NAND_WRITE32((NAND_NFI_FDM0L + 4*j+ NAND_NFI_FDM_LEN * i), pu4SectSpare[j]);
        }
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_GetRdDma()
 */
//-----------------------------------------------------------------------------
static void _NANDHW_NFI_GetRdDma(UINT32 *pu4SpareAddr)
{
    UINT32 i, j, u4SectOobSz;
    UINT32 *pu4SectSpare;

    u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;
    for (i = 0; i < _u4SectorNum; i++)
    {
        pu4SectSpare = (UINT32 *)((UINT32)pu4SpareAddr + u4SectOobSz * i);

        for (j = 0; j < _u4FdmNum/4; j++)
        {
            pu4SectSpare[j] = NAND_READ32(NAND_NFI_FDM0L + 4*j + NAND_NFI_FDM_LEN * i);
        }
    }
}

//-----------------------------------------------------------------------------
/** _NANDHW_NFI_ReadSpare()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_NFI_ReadSpare(UINT8 *pu1Buf, UINT32 u4Len, nand_ecc_modes_t rECCType)
{
    UINT32 i, u4Val = 0;
    UINT32 *pu4Buf = (UINT32 *)pu1Buf;
    INT32  i4Ret = NAND_SUCC;

    for (i = 0; i < u4Len; i += 4)
    {
        _NANDHW_WaitRead(4);
        pu4Buf[u4Val] = NAND_READ32(NAND_NFI_DATAR);
        u4Val++;
    }

    if (rECCType == NAND_ECC_SOFT)
    {
        if (u4Len ==  (_u4SectorSize + ((_u4OOBSize * _u4SectorSize)>>9)))
        {
            /* ECC correction. */
            if (_NANDHW_NFIECC_CorrectEcc(pu1Buf, &pu1Buf[_u4SectorSize], 0))
            {
                i4Ret |= NAND_ECC_ERROR;
            }
        }
    }

    _NANDHW_WaitBusy();
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** _NANDHW_ReadSpare()
 */
//-----------------------------------------------------------------------------
static INT32 _NANDHW_ReadSpare(UINT8 *pu1Buf, UINT32 u4PgIdx, UINT32 u4PgOff, UINT32 u4Len,
                               nand_ecc_modes_t rECCType)
{
    INT32 i4Ret = NAND_SUCC;

    _NANDHW_NFI_Reset();
    _NANDHW_NFIECC_Reset();

    _NANDHW_NFI_Cfg(rECCType, FALSE, NAND_NFI_CNFG_OP_READ);
    _NANDHW_NFIECC_Cfg(rECCType);
    _NANDHW_NFIECC_Trig(NAND_NFI_CNFG_OP_READ);

    /* Write command. */
    if ((u4PgOff == 0x200) && (_u4PageSize == 0x200))
    {
        _NANDHW_Command(NAND_CMD_READOOB);
        _NANDHW_Addr(u4PgIdx, 0, (_u4PgAddrLen<<4)|(_u4TotalAddrLen - _u4PgAddrLen));
    }
    else
    {
        _NANDHW_Command(NAND_CMD_READ0);
        _NANDHW_Addr(u4PgIdx, u4PgOff, (_u4PgAddrLen<<4)|(_u4TotalAddrLen - _u4PgAddrLen));
    }

    if (_u4PageSize != 0x200)
    {
        _NANDHW_Command(NAND_CMD_READSTART);
    }
    _NANDHW_WaitBusy();

    /*Trigger Read . */
    i4Ret = _NANDHW_NFI_CheckOpMode(NAND_NFI_CNFG_OP_READ);
    if (i4Ret)
    {
        goto HandleError;
    }

    _NANDHW_NFI_Trig(NAND_NFI_CNFG_OP_READ);

    _NANDHW_WaitBusy();
    i4Ret = _NANDHW_NFI_ReadSpare(pu1Buf, u4Len, rECCType);
    if (i4Ret)
    {
        goto HandleError;
    }

    _NANDHW_WaitBusy();
    NAND_WRITE32(NAND_NFI_CNFG, 0);
    return NAND_SUCC;

HandleError:
    _NANDHW_WaitBusy();
    NAND_WRITE32(NAND_NFI_CNFG, 0);
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** mt5365_dma_readpage() - buf must be virtual memory
 */
//-----------------------------------------------------------------------------
static int mt5365_dma_readpage(struct mtd_info *mtd, UINT8 *buf, UINT8 *spare)
{
    INT32 i4Ret = NAND_SUCC;
    struct nand_chip *chip = mtd->priv;
    dma_addr_t u4DmaAddr, u4DmaPatternAddr;
    UINT32 u4DmaRetryCnt = 0;
    UINT32 *pu4DmaBuf = (UINT32 *)buf;
    
    pu4DmaBuf[_u4PageSize/4-1] = NAND_DMA_PATTERN;
    u4DmaPatternAddr = dma_map_single(NULL, (void *)(&buf[_u4PageSize - NAND_DMA_CACHE_LINE]), NAND_DMA_CACHE_LINE, DMA_TO_DEVICE);
    dma_unmap_single(NULL, u4DmaPatternAddr, NAND_DMA_CACHE_LINE, DMA_TO_DEVICE);
    
    _NANDHW_NFI_SetRdDma((UINT32 *)buf);
    u4DmaAddr = dma_map_single(NULL, (void *)buf, _u4PageSize, DMA_FROM_DEVICE);

    _NANDHW_NFI_Cfg(chip->ecc.mode, _fgDmaEn, NAND_NFI_CNFG_OP_READ);
    _NANDHW_NFIECC_Cfg(chip->ecc.mode);
    _NANDHW_NFIECC_Trig(NAND_NFI_CNFG_OP_READ);

    _NANDHW_Command(NAND_CMD_READ0);
    _NANDHW_Addr(_u4CurPage, 0, (_u4PgAddrLen<<4)|(_u4TotalAddrLen - _u4PgAddrLen));
    if (mtd->writesize != 512)
    {
        mtd_nand_setisr(NAND_NFI_INTR_EN_RD_DONE);
        NAND_WRITE32(NAND_NFI_CMD, NAND_CMD_READSTART);
        mtd_nand_waitisr(NAND_NFI_INTR_RD_DONE);
    }

    _NANDHW_WaitBusy();
    _NANDHW_NFI_CheckOpMode(NAND_NFI_CNFG_OP_READ);

    mtd_nand_setisr(NAND_NFI_INTR_EN_AHB_DONE);
    /*Trigger Read */
    _NANDHW_NFI_Trig(NAND_NFI_CNFG_OP_READ);
    mtd_nand_waitisr(NAND_NFI_INTR_AHB_DONE);

    _NANDHW_WaitBusy();
    _NANDHW_NFIECC_WaitBusy();

    _NANDHW_NFI_GetRdDma((UINT32 *)spare);
    _NANDHW_NFI_WaitSectorCnt();

    // Check ECC done
    _NANDHW_NFIECC_CheckEccDone();
    
    // Cause nand dma read may send finish interrupt before write all data into dram.
    // So we add a pattern at last location to check if dma read is finished.
    while (1)
    {
        dma_unmap_single(NULL, u4DmaAddr, _u4PageSize, DMA_FROM_DEVICE);
        
        if (pu4DmaBuf[_u4PageSize/4-1] != NAND_DMA_PATTERN)
        {
            if (u4DmaRetryCnt > 0)
            {
                printk(KERN_CRIT "native nand driver: u4DmaTryCnt = %d, pageidx = 0x%08X\n", u4DmaRetryCnt, _u4CurPage);
            }
            break;
        }

        u4DmaRetryCnt++;
        if (u4DmaRetryCnt >= NAND_DMA_RETYR_CNT)
        {
            printk(KERN_CRIT "native nand driver: dma pattern check time-out, pageidx = 0x%08X\n", _u4CurPage);
            break;
        }
        
        udelay(NAND_DMA_RETYR_DELAY);
    }
    
    // Cause pattern check and CPU prefetch will update cache.
    // Invalidate whole cache to ensure bit-flips writen into dram.
    dma_unmap_single(NULL, u4DmaAddr, _u4PageSize, DMA_FROM_DEVICE);
            
    i4Ret = _NANDHW_NFIECC_CheckEcc(buf, spare);

    _NANDHW_WaitBusy();
    NAND_WRITE32(NAND_NFI_CNFG, 0);
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** mt5365_dma_writepage() - buf must be virtual memory
 */
//-----------------------------------------------------------------------------
static void mt5365_dma_writepage(struct mtd_info *mtd, UINT8 *buf, UINT8 *spare)
{
    struct nand_chip *chip = mtd->priv;
    dma_addr_t u4DmaAddr;

    _NANDHW_NFI_SetWtDma((UINT32 *)buf, (UINT32 *)spare);
    u4DmaAddr = dma_map_single(NULL, (void *)buf, _u4PageSize, DMA_TO_DEVICE);

    _NANDHW_NFI_Cfg(chip->ecc.mode, _fgDmaEn, NAND_NFI_CNFG_OP_PROGRAM);
    _NANDHW_NFIECC_Cfg(chip->ecc.mode);
    _NANDHW_NFIECC_Trig(NAND_NFI_CNFG_OP_PROGRAM);

    _NANDHW_Command(NAND_CMD_SEQIN);
    _NANDHW_Addr(_u4CurPage, 0, (_u4PgAddrLen<<4)|(_u4TotalAddrLen - _u4PgAddrLen));

    _NANDHW_WaitBusy();

    mtd_nand_setisr(NAND_NFI_INTR_EN_AHB_DONE);
    /*Trigger Write*/
    _NANDHW_NFI_Trig(NAND_NFI_CNFG_OP_PROGRAM);
    mtd_nand_waitisr(NAND_NFI_INTR_AHB_DONE);

    _NANDHW_WaitBusy();
    _NANDHW_NFIECC_WaitBusy();

    _NANDHW_NFI_WaitSectorCnt();
    _NANDHW_WaitBusy();

    NAND_WRITE32(NAND_NFI_CNFG, 0);
    dma_unmap_single(NULL, u4DmaAddr, _u4PageSize, DMA_TO_DEVICE);
}

//-----------------------------------------------------------------------------
/** mt5365_polling_readpage()
 */
//-----------------------------------------------------------------------------
static int mt5365_polling_readpage(struct mtd_info *mtd, UINT8 *buf, UINT8 *spare)
{
    struct nand_chip *chip = mtd->priv;
    UINT32 *pu4Buf = (UINT32 *)buf;
    UINT32 *pu4Spare = (UINT32 *)spare;
    UINT32 u4SectIdx, j, u4Val, u4LoopCnt;
    INT32 i4Ret = NAND_SUCC;

    /*Trigger Read . */
    _NANDHW_NFI_CheckOpMode(NAND_NFI_CNFG_OP_READ);
    _NANDHW_NFI_Trig(NAND_NFI_CNFG_OP_READ);
    _NANDHW_WaitBusy();

    for(u4SectIdx = 0; u4SectIdx < _u4SectorNum; u4SectIdx++)
    {
        // main data
        u4LoopCnt = _u4SectorSize>>2;
        u4Val = u4SectIdx*u4LoopCnt;
        for(j = 0; j < u4LoopCnt; j++)
        {
            _NANDHW_WaitRead(4);
            pu4Buf[u4Val] = NAND_READ32(NAND_NFI_DATAR);
            u4Val++;
        }

        // FDM+ECC data
        u4LoopCnt = (_u4OOBSize * _u4SectorSize) >> 11;
        u4Val = u4SectIdx*u4LoopCnt;
        for(j = 0; j < u4LoopCnt; j++)
        {
            _NANDHW_WaitRead(4);
            pu4Spare[u4Val] = NAND_READ32(NAND_NFI_DATAR);
            u4Val++;
        }

        if(chip->ecc.mode == NAND_ECC_SOFT)
        {
            /* ECC correction. */
            if(_NANDHW_NFIECC_CorrectEcc(buf, spare, u4SectIdx))
            {
                i4Ret |= NAND_ECC_ERROR;
            }
        }

        _NANDHW_WaitBusy();
    }

    _NANDHW_NFIECC_WaitBusy();
    _NANDHW_NFI_WaitSectorCnt();

    _NANDHW_WaitBusy();
    NAND_WRITE32(NAND_NFI_CNFG, 0);
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** mt5365_polling_writepage()
 */
//-----------------------------------------------------------------------------
static int mt5365_polling_writepage(struct mtd_info *mtd, UINT8 *buf, UINT8 *spare)
{
    INT32 i4Ret = 0;
    struct nand_chip *chip = mtd->priv;
    UINT32 i, u4Val, u4SectOobSz;
    u_char u1EccCode[NAND_NFI_ENCPAR_NUM*4];

    u4SectOobSz =  (_u4OOBSize * _u4SectorSize)>>9;
    _NANDHW_NFI_Trig(NAND_NFI_CNFG_OP_PROGRAM);

    for(i = 0; i < _u4SectorNum; i++)
    {
        // write main data
        _NANDHW_NFI_WriteFifo(&buf[i*_u4SectorSize], _u4SectorSize);

        if(chip->ecc.mode == NAND_ECC_SOFT)
        {
            // write fdm data
            _NANDHW_NFI_WriteFifo(&spare[i*u4SectOobSz], _u4FdmNum);

            /* ECC correction. */
            _NANDHW_WaitWrite();
            if(_NANDHW_NFIECC_GetEcc((UINT32 *)u1EccCode))
            {
                i4Ret = NAND_ECC_ERROR;
                //goto HandleError;
            }

            // write ecc data
            u4Val = u4SectOobSz - _u4FdmNum;
            _NANDHW_NFI_WriteFifo(u1EccCode, u4Val);
        }
        else
        {
            // write spare data
            _NANDHW_NFI_WriteFifo(&spare[i*u4SectOobSz], u4SectOobSz);
        }
    }

    _NANDHW_NFI_WaitSectorCnt();

    _NANDHW_WaitBusy();
    NAND_WRITE32(NAND_NFI_CNFG, 0);
    return i4Ret;
}

//-----------------------------------------------------------------------------
/** mt5365_nand_isbadpage()
 */
//-----------------------------------------------------------------------------
BOOL mt5365_nand_isbadpage(int page)
{
    INT32 iRet;
    UINT32 u4BitCnt, u4Offset, u4OobLastIdx, i;
    UINT32 u4SectOobSz, u4PgOobSz, u4EccSectSz;

    UINT8 *pu1Data  = _pu1NandDataBuf;
    UINT8 *pu1Spare = _pu1NandOobBuf;

    // Cause mt5365_nand_isbadblk() is not through mt5365_nand_command().
    // Init variable _u4CurPage & _u4CurBitFlip
    _u4CurPage = (UINT32)page;
    _u4CurBitFlip = 0;
    
    u4SectOobSz = (_u4OOBSize * _u4SectorSize)>>9;
    u4PgOobSz = (_u4OOBSize * _u4PageSize)>>9;
    u4EccSectSz = _u4SectorSize + u4SectOobSz;

    // read spare data without ecc
    u4Offset = _u4PageSize;
    if ((_prFlashDev->u4BBMode & NAND_BBMODE_ECCBIT) == 1)
    {
        iRet = _NANDHW_ReadSpare(pu1Spare, page, u4Offset, _u4OOBSize, NAND_ECC_NONE);
    }
    else
    {
        iRet = _NANDHW_ReadSpare(pu1Spare, page, u4Offset, u4PgOobSz, NAND_ECC_NONE);
    }

    if (iRet)
    {
        goto HandleBad;
    }

    if ((_prFlashDev->u4BBMode & NAND_BBMODE_BBMARK) == 0x00000500)
    {
        u4BitCnt = _NANDHW_countbits(pu1Spare[5]) + _NANDHW_countbits(pu1Spare[6]);
    }
    else
    {
        u4BitCnt = _NANDHW_countbits(pu1Spare[0]) + _NANDHW_countbits(pu1Spare[1]);
    }

    switch (_prFlashDev->u4BBMode & NAND_BBMODE_ECCBIT)
    {
    case 0:
        BUG();
        break;

    case 1:
        if (u4BitCnt < 15)
        {
            goto HandleBad;
        }
        else
        {
            goto HandleGood;
        }

    default:
        u4OobLastIdx = u4PgOobSz - u4SectOobSz;
        for (i = 0; i < 6; i++)
        {
            u4BitCnt += _NANDHW_countbits(pu1Spare[u4OobLastIdx + i]);
        }

        if (u4BitCnt > 62)
        {
            goto HandleGood;
        }

        if (u4BitCnt < 2)
        {
            goto HandleBad;
        }

        break;
    }

    // read last sector data with ecc
    u4Offset = (_u4SectorNum - 1) * u4EccSectSz;
    iRet = _NANDHW_ReadSpare(pu1Data, page, u4Offset, u4EccSectSz, NAND_ECC_SOFT);
    if (iRet)
    {
        goto HandleBad;
    }

    u4OobLastIdx = u4EccSectSz - (u4SectOobSz*_u4SectorNum);
    if ((pu1Data[u4OobLastIdx] != 0xFF) || (pu1Data[u4OobLastIdx + 1] != 0xFF))
    {
        goto HandleBad;
    }

HandleGood:
    return FALSE;       // Good block

HandleBad:
    return TRUE;       // bad block
}

//-----------------------------------------------------------------------------
/** mt5365_nand_isbadblk()
 */
//-----------------------------------------------------------------------------
static int mt5365_nand_isbadblk(struct mtd_info *mtd, loff_t ofs, int getchip)
{
    struct nand_chip *chip = mtd->priv;
    int block = (int)(ofs >> chip->phys_erase_shift);

    // check the 1st page of the block.
    if (mt5365_nand_isbadpage(block * _u4BlkPgCount))
    {
        return 1;
    }

    // check the last page of the block if MLC.
    if (_u4OOBSize > 16)
    {
        if (mt5365_nand_isbadpage(block * _u4BlkPgCount + _u4BlkPgCount -1))
        {
            return 1;
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** mt5365_nand_markbadpage()
 */
//-----------------------------------------------------------------------------
static int mt5365_nand_markbadpage(struct mtd_info *mtd, int page)
{
    struct nand_chip *chip = mtd->priv;
    int status = 0;

    memset(_pu1NandDataBuf, 0, mtd->writesize);
    memset(_pu1NandOobBuf, 0, mtd->oobsize);

    chip->cmdfunc(mtd, NAND_CMD_SEQIN, 0, page);

    if (_fgDmaEn)
    {
        mt5365_dma_writepage(mtd, _pu1NandDataBuf, _pu1NandOobBuf);
    }
    else
    {
        mt5365_polling_writepage(mtd, _pu1NandDataBuf, _pu1NandOobBuf);
    }

    chip->cmdfunc(mtd, NAND_CMD_PAGEPROG, -1, -1);

    status = chip->waitfunc(mtd, chip);
    return status & NAND_STATUS_FAIL ? -EIO : 0;
}

//-----------------------------------------------------------------------------
/** mt5365_nand_eraseblk()
 */
//-----------------------------------------------------------------------------
static INT32 mt5365_nand_eraseblk(struct mtd_info *mtd, UINT32 u4BlkIdx)
{
    struct nand_chip *chip = mtd->priv;
    int status = 0;
    UINT32 u4PgIdx = u4BlkIdx * _u4BlkPgCount;

    _NANDHW_NFI_Reset();
    _NANDHW_NFI_Cfg(NAND_ECC_NONE, FALSE, NAND_NFI_CNFG_OP_ERASE);

    _NANDHW_Command(NAND_CMD_ERASE1);
    _NANDHW_Addr(u4PgIdx, 0, _u4PgAddrLen<<4);
    _NANDHW_WaitBusy();

    mtd_nand_setisr(NAND_NFI_INTR_EN_ERASE_DONE);

    /*Trigger Erase. */
    NAND_WRITE32(NAND_NFI_CMD, NAND_CMD_ERASE2);

    mtd_nand_waitisr(NAND_NFI_INTR_ERASE_DONE);

    _NANDHW_WaitBusy();
    NAND_WRITE32(NAND_NFI_CNFG, 0);

    status = chip->waitfunc(mtd, chip);
    return status & NAND_STATUS_FAIL ? -EIO : 0;
}

//-----------------------------------------------------------------------------
/** mt5365_nand_markbadblk()
 */
//-----------------------------------------------------------------------------
static int mt5365_nand_markbadblk(struct mtd_info *mtd, loff_t ofs)
{
    struct nand_chip *chip = mtd->priv;
    int block;

    block = (int)(ofs >> chip->phys_erase_shift);

    printk(KERN_CRIT "mt5365_nand_markbadblk: %d\n", block);

    // erase block before mark it bad,
    // cause MLC does not support re-write absolutely. mtk40109 2010-12-06
    mt5365_nand_eraseblk(mtd, block);

    // Mark the first page of the block.
    mt5365_nand_markbadpage(mtd, block * _u4BlkPgCount);

    // Mark the last page of the block if MLC.
    if (_u4OOBSize > 16)
    {
        mt5365_nand_markbadpage(mtd, block * _u4BlkPgCount + _u4BlkPgCount - 1);
    }

    // Always return successful ,
    // cause ubifs will become read-only mode while erase fail.  mtk40109 2010-12-06
    return 0;
}

//-----------------------------------------------------------------------------
/** mt5365_nand_verify_buf()
 */
//-----------------------------------------------------------------------------
static int mt5365_nand_verify_buf(struct mtd_info *mtd, const UINT8 *buf, int len)
{
#if defined(CC_NAND_WRITE_VERIFY)
    int i4Ret = 0;

    #if NAND_DEBUG_PRINT
    printk(KERN_INFO "mt5365_nand_verify_buf, page = 0x%X\n", _u4CurPage);
    #endif

    if (_fgDmaEn)
    {
        i4Ret = mt5365_dma_readpage(mtd, _pu1NandDataBuf, _pu1NandOobBuf);
    }
    else
    {
        i4Ret = mt5365_polling_readpage(mtd, _pu1NandDataBuf, _pu1NandOobBuf);
    }

    if (i4Ret)
    {
        printk(KERN_ERR "mt5365_nand_verify_buf read error! page = 0x%X\n", _u4CurPage);
        return -EIO;
    }

    if (_u4CurBitFlip)
    {
        printk(KERN_ERR "mt5365_nand_verify_buf bitflip occur! page = 0x%X\n", _u4CurPage);
        return -EIO;
    }
#endif

    return 0;
}

//-----------------------------------------------------------------------------
/** mt5365_nand_read_byte()
 */
//-----------------------------------------------------------------------------
static u_char mt5365_nand_read_byte(struct mtd_info *mtd)
{
    UINT8 u1Reg;

    _NANDHW_WaitBusy();
    _NANDHW_NFI_Cfg(NAND_ECC_NONE, 0, NAND_NFI_CNFG_OP_SINGLE);
    NAND_WRITE32(NAND_NFI_CON, NAND_NFI_CON_NOB(1) | NAND_NFI_CON_SRD);

    _NANDHW_WaitBusy();
    _NANDHW_WaitRead(1);
    u1Reg = (UINT8)(NAND_READ32(NAND_NFI_DATAR) & 0xFF);

    // Add for reset fifo in byte read mode. (jun-2010/03/23)
    _NANDHW_NFI_Reset();
    return u1Reg;
}

//-----------------------------------------------------------------------------
/** mt5365_nand_read_buf()
 */
//-----------------------------------------------------------------------------
static void mt5365_nand_read_buf(struct mtd_info *mtd, UINT8 *buf, INT32 len)
{
    printk(KERN_ERR "mt5365_nand_read_buf addr=0x%08x, len=%d\n", (UINT32)buf, len);
    BUG();
}

//-----------------------------------------------------------------------------
/** mt5365_nand_write_buf()
 */
//-----------------------------------------------------------------------------
static void mt5365_nand_write_buf(struct mtd_info *mtd, const UINT8 *buf, INT32 len)
{
    printk(KERN_ERR "mt5365_nand_write_buf addr=0x%08x, len=%d\n", (UINT32)buf, len);
    BUG();
}

//-----------------------------------------------------------------------------
/** mt5365_nand_need_aes()
 */
//-----------------------------------------------------------------------------
#ifdef CC_MTD_ENCRYPT_SUPPORT
static int mt5365_nand_enctyped(struct mtd_info *mtd, int page, UINT32 *pu4Data)
{
    int i, j;
    mtd_enc_info_t *p;

    for(i = 0; i < MAX_MTD_DEVICES; i++)
    {
        p = &(rMTDPartENC[i]);

        if (page >= p->u4PgStart && page <= p->u4PgEnd)
        {
            if (p->u4Encryped == 1)
            {
                for (j = 0; j < mtd->writesize/4; j++)
                {
                    if (pu4Data[j] != 0xFFFFFFFF)
                    {
                        return 1;
                    }
                }
            }

            break;
        }
    }

    return 0;
}
#endif

//-----------------------------------------------------------------------------
/** mt5365_nand_read_page()
 */
//-----------------------------------------------------------------------------
static int mt5365_nand_read_page(struct mtd_info *mtd, struct nand_chip *chip, UINT8 *buf, int page)
{
    int i4Ret = NAND_SUCC;
    UINT8 *pu1Data  = (UINT8 *)buf;
    UINT8 *pu1Spare = (UINT8 *)chip->oob_poi;

    #if NAND_DEBUG_PRINT
    printk(KERN_INFO "mt5365_nand_read_page, page = 0x%X\n", _u4CurPage);
    #endif

    if ((UINT32)page != _u4CurPage)
    {
        BUG();
    }

    if (_fgDmaEn)
    {
        i4Ret = mt5365_dma_readpage(mtd, _pu1NandDataBuf, _pu1NandOobBuf);
    }
    else
    {
        i4Ret = mt5365_polling_readpage(mtd, _pu1NandDataBuf, _pu1NandOobBuf);
    }

    if (i4Ret)
    {
        mtd->ecc_stats.failed++;
    }
    else
    {
        mtd->ecc_stats.corrected += _u4CurBitFlip;
        _NANDHW_DataExchange((UINT32 *)_pu1NandDataBuf, (UINT32 *)_pu1NandOobBuf, 0, mtd->writesize);

#ifdef CC_MTD_ENCRYPT_SUPPORT
        if (mt5365_nand_enctyped(mtd, _u4CurPage, (UINT32 *)_pu1NandDataBuf))
        {
            NAND_AES_Decryption(virt_to_phys((void *)_pu1NandDataBuf), virt_to_phys((void *)_pu1NandDataBuf), mtd->writesize);
        }
#endif
    }

     memcpy(pu1Data,  _pu1NandDataBuf, mtd->writesize);
     memcpy(pu1Spare, _pu1NandOobBuf, mtd->oobsize);
     
// use poi read to verify dma read
#if NAND_DMA_READ_VERIFY
    {
        UINT32 i;
        UINT32 *pu4Buf1 = (UINT32 *)pu1Data;
        UINT32 *pu4Buf2 = (UINT32 *)_pu1NandDataBuf;
        
        memset(_pu1NandDataBuf, 0xA5, mtd->writesize);
        memset(_pu1NandOobBuf,  0x5A, mtd->oobsize);
        
        _fgDmaEn = FALSE;
        chip->ecc.mode = NAND_ECC_SOFT;
        
        _NANDHW_NFI_Reset();
        _NANDHW_NFIECC_Reset();
        
        _NANDHW_NFI_Cfg(chip->ecc.mode, _fgDmaEn, NAND_NFI_CNFG_OP_READ);
        _NANDHW_NFIECC_Cfg(chip->ecc.mode);
        _NANDHW_NFIECC_Trig(NAND_NFI_CNFG_OP_READ);
        
        _NANDHW_Command(NAND_CMD_READ0);
        _NANDHW_Addr(_u4CurPage, 0, (_u4PgAddrLen<<4)|(_u4TotalAddrLen - _u4PgAddrLen));
        if (mtd->writesize != 512)
        {
            _NANDHW_Command(NAND_CMD_READSTART);
        }
        
        mt5365_polling_readpage(mtd, _pu1NandDataBuf, _pu1NandOobBuf);
        _NANDHW_DataExchange((UINT32 *)_pu1NandDataBuf, (UINT32 *)_pu1NandOobBuf, 0, mtd->writesize);
        
    #ifdef CC_MTD_ENCRYPT_SUPPORT
        if (mt5365_nand_enctyped(mtd, _u4CurPage, (UINT32 *)_pu1NandDataBuf))
        {
            NAND_AES_Decryption(virt_to_phys((void *)_pu1NandDataBuf), virt_to_phys((void *)_pu1NandDataBuf), mtd->writesize);
        }
    #endif
        
        for (i = 0; i < mtd->writesize/4; i++)
        {
            if (pu4Buf1[i] != pu4Buf2[i])
            {
                printk(KERN_ERR "read verify fail(native): page=0x%X, offset=0x%X, dma=0x%08X, pio=0x%08X\n",
                	_u4CurPage, i*4, pu4Buf1[i], pu4Buf2[i]);
            }
        }
        
        _fgDmaEn = TRUE;
        chip->ecc.mode = NAND_ECC_HW;
    }
#endif

    return  0;
}

//-----------------------------------------------------------------------------
/** mt5365_nand_write_page()
 */
//-----------------------------------------------------------------------------
static void mt5365_nand_write_page(struct mtd_info *mtd, struct nand_chip *chip, const UINT8 *buf)
{
    UINT8 *pu1Data  = (UINT8 *)buf;
    UINT8 *pu1Spare = (UINT8 *)chip->oob_poi;

    #if NAND_DEBUG_PRINT
    printk(KERN_INFO "mt5365_nand_write_page, page = 0x%X\n", _u4CurPage);
    #endif

    memcpy(_pu1NandDataBuf, pu1Data, mtd->writesize);
    memcpy(_pu1NandOobBuf,  pu1Spare, mtd->oobsize);

#ifdef CC_MTD_ENCRYPT_SUPPORT
    if (mt5365_nand_enctyped(mtd, _u4CurPage, (UINT32 *)_pu1NandDataBuf))
    {
        NAND_AES_Encryption(virt_to_phys((void *)_pu1NandDataBuf), virt_to_phys((void *)_pu1NandDataBuf), mtd->writesize);
    }
#endif

    _NANDHW_DataExchange((UINT32 *)_pu1NandDataBuf, (UINT32 *)_pu1NandOobBuf, 0, mtd->writesize);

    if (_fgDmaEn)
    {
        mt5365_dma_writepage(mtd, _pu1NandDataBuf, _pu1NandOobBuf);
    }
    else
    {
        mt5365_polling_writepage(mtd, _pu1NandDataBuf, _pu1NandOobBuf);
    }
}

static int mt5365_nand_read_oob(struct mtd_info *mtd, struct nand_chip *chip, INT32 page, INT32 sndcmd)
{
    int i4Ret = NAND_SUCC;
    UINT8 *pu1Spare = (UINT8 *)chip->oob_poi;

    #if NAND_DEBUG_PRINT
    printk(KERN_INFO "mt5365_nand_read_oob\n");
    #endif

    if (likely(sndcmd))
    {
        chip->cmdfunc(mtd, NAND_CMD_READ0, 0, page);
        sndcmd = 0;
    }

    if (_fgDmaEn)
    {
        i4Ret = mt5365_dma_readpage(mtd, _pu1NandDataBuf, _pu1NandOobBuf);
    }
    else
    {
        i4Ret = mt5365_polling_readpage(mtd, _pu1NandDataBuf, _pu1NandOobBuf);
    }

    if (i4Ret)
    {
        mtd->ecc_stats.failed++;
    }
    else
    {
        mtd->ecc_stats.corrected += _u4CurBitFlip;
        _NANDHW_DataExchange((UINT32 *)_pu1NandDataBuf, (UINT32 *)_pu1NandOobBuf, 0, mtd->writesize);
    }

    memcpy(pu1Spare, _pu1NandOobBuf, mtd->oobsize);
    return sndcmd;
}

static int mt5365_nand_write_oob(struct mtd_info *mtd, struct nand_chip *chip, INT32 page)
{
    printk(KERN_ERR "mt5365_nand_write_oob\n");
    BUG();

    return 0;
}
//-----------------------------------------------------------------------------
/** mt5365_nand_dev_ready()
 */
//-----------------------------------------------------------------------------
static int mt5365_nand_dev_ready(struct mtd_info *mtd)
{
    return 1;
}

//-----------------------------------------------------------------------------
/** mt5365_nand_select_chip()
 */
//-----------------------------------------------------------------------------
static void mt5365_nand_select_chip(struct mtd_info *mtd, INT32 chip)
{
}

//-----------------------------------------------------------------------------
/** mt5365_nand_command()
 */
//-----------------------------------------------------------------------------
static void mt5365_nand_command(struct mtd_info *mtd, unsigned command, INT32 column, INT32 page_addr)
{
    struct nand_chip *chip = mtd->priv;

    #if NAND_DEBUG_PRINT
    printk(KERN_INFO "command: 0x%x, col: 0x%x, page: 0x%08x\n", command, column, page_addr);
    #endif

    _u4CurPage = (UINT32)page_addr;
    _u4CurBitFlip = 0;

    _NANDHW_NFI_Reset();
    _NANDHW_NFIECC_Reset();

    switch(command)
    {
    case NAND_CMD_READID:
        _NANDHW_Command(command);
        _NANDHW_Addr(0, 0, 1);
        break;

    case NAND_CMD_READ0:
        if (column >= (mtd->writesize + mtd->oobsize))
        {
            printk(KERN_ERR "oobsize is too large\n");
            BUG();
        }

        if (!_fgDmaEn)
        {
            _NANDHW_NFI_Cfg(chip->ecc.mode, _fgDmaEn, NAND_NFI_CNFG_OP_READ);
            _NANDHW_NFIECC_Cfg(chip->ecc.mode);
            _NANDHW_NFIECC_Trig(NAND_NFI_CNFG_OP_READ);

            _NANDHW_Command(command);
            _NANDHW_Addr(page_addr, column, (_u4PgAddrLen<<4)|(_u4TotalAddrLen - _u4PgAddrLen));
            if (mtd->writesize != 512)
            {
                _NANDHW_Command(NAND_CMD_READSTART);
            }
        }
        break;

    case NAND_CMD_READ1:
        if (mtd->writesize != 512)
        {
            printk(KERN_ERR "Unhandle command!\n");
            BUG();
        }
        if (column >= (mtd->writesize + mtd->oobsize))
        {
            printk(KERN_ERR "oobsize is too large\n");
            BUG();
        }

        _NANDHW_Command(command);
        _NANDHW_Addr(page_addr, column, (_u4PgAddrLen<<4)|(_u4TotalAddrLen - _u4PgAddrLen));
        break;

    case NAND_CMD_READOOB:
        if (mtd->writesize != 512)
        {
            column += mtd->writesize;
            command = NAND_CMD_READ0;
        }
        if (column >= (mtd->writesize + mtd->oobsize))
        {
            printk(KERN_ERR "oobsize is too large\n");
            BUG();
        }

        _NANDHW_Command(command);
        _NANDHW_Addr(page_addr, column, (_u4PgAddrLen<<4)|(_u4TotalAddrLen - _u4PgAddrLen));
        if (mtd->writesize != 512)
        {
            _NANDHW_Command(NAND_CMD_READSTART);
        }
        break;

    case NAND_CMD_SEQIN:
        if (column != 0)
        {
            BUG();
        }

        if (!_fgDmaEn)
        {
            _NANDHW_NFI_Cfg(chip->ecc.mode, _fgDmaEn, NAND_NFI_CNFG_OP_PROGRAM);
            _NANDHW_NFIECC_Cfg(chip->ecc.mode);
            _NANDHW_NFIECC_Trig(NAND_NFI_CNFG_OP_PROGRAM);

            _NANDHW_Command(NAND_CMD_SEQIN);
            _NANDHW_Addr(page_addr, 0, (_u4PgAddrLen<<4)|(_u4TotalAddrLen - _u4PgAddrLen));
        }
        break;

    case NAND_CMD_ERASE1:
        _NANDHW_Command(NAND_CMD_ERASE1);
        _NANDHW_Addr(page_addr, 0, _u4PgAddrLen<<4);
        break;

    case NAND_CMD_RESET:
    case NAND_CMD_PAGEPROG:
    case NAND_CMD_STATUS:
    case NAND_CMD_ERASE2:
        _NANDHW_Command(command);
        break;

    default:
        printk(KERN_ERR "Unhandle command!\n");
        BUG();
        break;
    }

    _NANDHW_WaitBusy();
}

//-----------------------------------------------------------------------------
/** mt5365_nand_enable_hwecc()
 */
//-----------------------------------------------------------------------------
static void mt5365_nand_enable_hwecc(struct mtd_info *mtd, INT32 mode)
{
    #if NAND_DEBUG_PRINT
    printk(KERN_INFO "mt5365_nand_enable_hwecc\n");
    #endif
}

//-----------------------------------------------------------------------------
/** mt5365_nand_calculate_ecc()
 */
//-----------------------------------------------------------------------------
static int mt5365_nand_calculate_ecc(struct mtd_info *mtd, const UINT8 *dat, UINT8 *ecc_code)
{
    #if NAND_DEBUG_PRINT
    printk(KERN_INFO "mt5365_nand_calculate_ecc\n");
    #endif

    return 0;
}

//-----------------------------------------------------------------------------
/** mt5365_nand_correct_data()
 */
//-----------------------------------------------------------------------------
static int mt5365_nand_correct_data(struct mtd_info *mtd, UINT8 *dat, UINT8 *read_ecc, UINT8 *calc_ecc)
{
    #if NAND_DEBUG_PRINT
    printk(KERN_INFO "mt5365_nand_correct_data\n");
    #endif

    return 0;
}

//-----------------------------------------------------------------------------
/** add_dynamic_parts()
 */
//-----------------------------------------------------------------------------
static inline int add_dynamic_parts(struct mtd_info *mtd)
{
    static const char *part_parsers[] = {"cmdlinepart", NULL};
    struct mtd_partition *parts;
    INT32 i, c, err;
    mtd_enc_info_t *p;

    #if NAND_DEBUG_PRINT
    printk(KERN_INFO "add_dynamic_parts\n");
    #endif

    c = parse_mtd_partitions(mtd, part_parsers, &parts, 0);
    if (c <= 0)
    {
        return -EIO;
    }

    err = add_mtd_partitions(mtd, parts, c);
    if (err != 0)
    {
        return -EIO;
    }

    for (i = 0; i < c; i++)
    {
        p = &(rMTDPartENC[i]);
        p->u4PgStart = parts[i].offset >> mt53xx_mtd->writesize_shift;
        p->u4PgEnd = p->u4PgStart + (parts[i].size >> mt53xx_mtd->writesize_shift) - 1;
        p->u4Encryped = (parts[i].mask_flags & MTD_ENCYPTED) ? 1 : 0;
        
        printk(KERN_INFO "partid=%02d, startpg=0x%08x, endpg=0x%08x, enc=%d\n",
            i, p->u4PgStart, p->u4PgEnd, p->u4Encryped);
    }

    return add_mtd_device(mt53xx_mtd);
}

//-----------------------------------------------------------------------------
/** mt5365_nand_init() - Main initialization routine
 */
//-----------------------------------------------------------------------------
static INT32 __init mt5365_nand_init(void)
{
    struct nand_chip *this;

    #if NAND_DEBUG_PRINT
    printk(KERN_INFO "mt5365_nand_init\n");
    #endif

    init_MUTEX(&nand_pin_share);

    _NANDHW_Init();

    /* Allocate memory for MTD device structure and private data */
    mt53xx_mtd = kmalloc(sizeof(struct mtd_info) + sizeof(struct nand_chip), GFP_KERNEL);
    if (!mt53xx_mtd)
    {
        printk(KERN_ERR "mt5365_nand_init: Unable to allocate MTD device structure!\n");
        return (-ENOMEM);
    }
    
    /* Get pointer to private data */
    this = (struct nand_chip *) (&mt53xx_mtd[1]);

    /* Initialize structures */
    memset((char *)mt53xx_mtd, 0, sizeof(struct mtd_info));
    memset((char *)this, 0, sizeof(struct nand_chip));

    /* Link the private data with the MTD structure */
    mt53xx_mtd->priv = this;
    mt53xx_mtd->name = "mt53xx-nand";
    mt53xx_mtd->writesize = _u4PageSize;
    mt53xx_mtd->oobsize = (_u4OOBSize*_u4PageSize)>>9;   // Not use value in nand_base.c

    this->chip_delay = 0;
    this->badblockpos = 0;
    this->options |= NAND_SKIP_BBTSCAN;
    //this->options |= NAND_USE_FLASH_BBT;
    
    this->read_byte = mt5365_nand_read_byte;
    this->select_chip = mt5365_nand_select_chip;
    this->dev_ready = mt5365_nand_dev_ready;
    this->cmdfunc = mt5365_nand_command;
    this->write_buf = mt5365_nand_write_buf;
    this->read_buf = mt5365_nand_read_buf;
    this->verify_buf = mt5365_nand_verify_buf;
    this->block_bad = mt5365_nand_isbadblk;
    this->block_markbad = mt5365_nand_markbadblk;
    
    if (_fgDmaEn)
    {
        this->ecc.mode = NAND_ECC_HW;
    }
    else
    {
        this->ecc.mode = NAND_ECC_SOFT;
    }
    this->ecc.size = _u4PageSize;
    
    this->ecc.hwctl = mt5365_nand_enable_hwecc;
    this->ecc.calculate = mt5365_nand_calculate_ecc;
    this->ecc.correct = mt5365_nand_correct_data;
    this->ecc.read_page = mt5365_nand_read_page;
    this->ecc.write_page = mt5365_nand_write_page;
    this->ecc.read_oob = mt5365_nand_read_oob;
    this->ecc.write_oob = mt5365_nand_write_oob;
        
    /* Scan to find existance of the device */
    if (nand_scan(mt53xx_mtd, 1))
    {
        kfree(mt53xx_mtd);
        mt53xx_mtd = NULL;
        return (-ENXIO);
    }
    
    if (add_dynamic_parts(mt53xx_mtd) < 0)
    {
        nand_release(mt53xx_mtd);
        kfree(mt53xx_mtd);
        mt53xx_mtd = NULL;

        printk(KERN_ERR "mt5365_nand_init: no partitions defined!\n");
        return (-ENODEV);
    }

    mtdchar_buf = kmalloc(_prFlashDev->u4BlkSize, GFP_KERNEL);
    init_MUTEX(&mtdchar_share);

    /* init completed */
    return 0;
}

//-----------------------------------------------------------------------------
/** mt5365_nand_cleanup() - Clean up routine
 */
//-----------------------------------------------------------------------------
static void __exit mt5365_nand_cleanup(void)
{
    nand_release(mt53xx_mtd);
    kfree(mt53xx_mtd);
    mt53xx_mtd = NULL;
    kfree(mtdchar_buf);
    mtdchar_buf = NULL;
}

void MTDNANDGetDmaBuf(UINT8 **ppu1DataBuf, UINT8 **ppu1OobBuf)
{
    *ppu1DataBuf = _pu1NandDataBuf;
    *ppu1OobBuf  = _pu1NandOobBuf;
}

void NANDPinShareLock(void)
{
    down(&nand_pin_share);
}

void NANDPinShareUnlock(void)
{
    up(&nand_pin_share);
}

void MTDNANDPinShareEntry(void)
{
    UINT32 u4Val;

    NANDPinShareLock();

    // Set nand/nor pinmux.
    u4Val = NAND_READ32(NAND_NFI_MISC);
    u4Val |= NAND_NFI_MISC_FLASH_PMUX;
    NAND_WRITE32(NAND_NFI_MISC, u4Val);

    _NANDHW_Reset();
}

void MTDNANDPinShareExit(void)
{
    NANDPinShareUnlock();
}

EXPORT_SYMBOL(MTDNANDGetDmaBuf);
EXPORT_SYMBOL(NANDPinShareLock);
EXPORT_SYMBOL(NANDPinShareUnlock);
EXPORT_SYMBOL(MTDNANDPinShareEntry);
EXPORT_SYMBOL(MTDNANDPinShareExit);

module_init(mt5365_nand_init);
module_exit(mt5365_nand_cleanup);

