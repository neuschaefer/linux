/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 *
 * $Author: hongjun.chu $
 * $Date: 2012/05/25 $
 * $RCSfile: storage.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file storage.c
 *  This C file implements the storage related function. It setup 3 threads which are IDE, FCI, and FLASH.
 *  The interface is also thread safe for multi-thread operation.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#if !defined (CC_MTK_LOADER) && !defined(CC_MTK_LOADER)

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_pinmux.h"

#include "nor_debug.h"
#include "nor_if.h"

#ifndef CC_EEPROM_ENABLE
#define CC_EEPROM_ENABLE
#endif

#ifdef CC_IDE_ENABLE
#include "ide_if.h"
#endif

#ifdef CC_FCI_ENABLE
#include "fci_if.h"
#endif

#ifdef CC_NAND_ENABLE
#include "nand_if.h"
#endif

#ifdef CC_EEPROM_ENABLE
#include "eeprom_if.h"
#endif

#ifdef CC_MSDC_ENABLE
#include "msdc_if.h"
#endif

#include "storage_if.h"
#include "drv_common.h"
#include "drvcust_if.h"
#include "x_blk_dev.h"

LINT_EXT_HEADER_END

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define STORG_BLOCK_FLAG        (STORG_FLAG_NAND | STORG_FLAG_CF | STORG_FLAG_HD | \
                                 STORG_FLAG_MS | STORG_FLAG_SD | STORG_FLAG_SMXD | \
                                 STORG_FLAG_MSDC)
#define STORG_CHAR_FLAG         (STORG_FLAG_NOR | STORG_FLAG_EEPROM)
#define STORG_SUPPORT_FLAG      (STORG_BLOCK_FLAG | STORG_CHAR_FLAG)
#define STORG_FLASH_FLAG        (STORG_FLAG_NAND | STORG_FLAG_NOR | STORG_FLAG_EEPROM)

#define STORG_IDE_FLAG          (STORG_FLAG_CF | STORG_FLAG_HD)

#define STORG_MSG_NUM           10
#define STORG_MSG_PROI          0

#define NULL_FUNC_LIST  { NULL, NULL, NULL, NULL, NULL, NULL, NULL }

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
typedef VOID (*FCI_CALLBACK)(INT32 fgStatus);

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
EXTERN VOID FCIHW_SetCardDetectFct(FCI_CALLBACK pfnCallback);

#if defined(__KERNEL__) && defined(CC_NAND_ENABLE)
typedef INT32 (*PFN_SDMOPS)(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
extern void RegisterSDMOpsCB(UINT32 u4SDMPart1, UINT32 u4SDMPart2, PFN_SDMOPS pfnRead, PFN_SDMOPS pfnWrite);
#endif

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static BOOL _fgStorgInit = FALSE;
static BOOL _fgStorgRun = TRUE;

#ifdef CC_IDE_ENABLE
static STORG_THRD_T _rSTDIde;
#endif

#if defined(CC_NAND_ENABLE) || !defined(CC_NOR_DISABLE) || defined(CC_EEPROM_ENABLE) || defined(CC_FCI_ENABLE)
static STORG_THRD_T _rSTDFlash;
#endif

#ifdef CC_IDE_ENABLE
static STORG_DEV_T _rStorgDevCF;
static STORG_DEV_T _rStorgDevHD;
#endif

#ifdef CC_FCI_ENABLE
static STORG_DEV_T _rStorgDevFCI;
static STORG_DEV_T _rStorgDevMS;
static STORG_DEV_T _rStorgDevSD;
static STORG_DEV_T _rStorgDevSMXD;
#endif

#ifdef CC_NAND_ENABLE
static STORG_DEV_T _rStorgDevNAND;
#endif

#ifdef CC_MSDC_ENABLE
static STORG_DEV_T _rStorgDevMSDC;
#endif

#ifndef CC_NOR_DISABLE
static STORG_DEV_T _rStorgDevNOR;
#endif

#ifdef CC_EEPROM_ENABLE
static STORG_DEV_T _rStorgDevEEPROM;
#endif

// function table =
//{ pfnRead, pfnWrite, pfnErase, pfnFlush, pfnStdbyTime, pfnInsert, pfnRemove };
#ifdef CC_IDE_ENABLE
static STORG_FUNCTBL_T _rCFFuncTbl =
{
    CF_Read, CF_Write, NULL, NULL, NULL, CF_Insert, CF_Remove, NULL
};

static STORG_FUNCTBL_T _rHDFuncTbl =
{
    HD_Read, HD_Write, NULL, HD_Flush, HD_SetStdbyTmr, HD_Insert, HD_Remove, NULL
};
#endif

#ifdef CC_FCI_ENABLE
static STORG_FUNCTBL_T _rFCIFuncTbl =
{
    NULL, NULL, NULL, NULL, NULL, FCI_Init, FCI_Stop, NULL
};

static STORG_FUNCTBL_T _rMSFuncTbl =
{
    MS_Read, MS_Write, NULL, NULL, NULL, NULL, NULL, NULL
};

static STORG_FUNCTBL_T _rSDFuncTbl =
{
    SD_Read, SD_Write, NULL, NULL, NULL, NULL, NULL, NULL
};

#ifdef CC_FCI_SMXD_SUPPORT
static STORG_FUNCTBL_T _rSMXDFuncTbl =
{
    SMXD_Read, SMXD_Write, NULL, NULL, NULL, NULL, NULL, NULL
};
#endif
#endif

#ifndef CC_NOR_DISABLE
static STORG_FUNCTBL_T _rNORFuncTbl =
{
    NORPART_Read, NORPART_Write, NORPART_Erase, NULL, NULL, NULL, NULL, NORPART_IoCtl
};
#endif

#ifdef CC_NAND_ENABLE
static STORG_FUNCTBL_T _rNANDFuncTbl =
{
    NANDPART_Read, NANDPART_Write, NULL, NULL, NULL, NULL, NULL, NULL
};
#endif

#ifdef CC_MSDC_ENABLE
static STORG_FUNCTBL_T _rMSDCFuncTbl =
{
    MSDCPART_Read, MSDCPART_Write, NULL, NULL, NULL, NULL, NULL, NULL
};
#endif

#ifdef CC_EEPROM_ENABLE
static STORG_FUNCTBL_T _rEEPROMFuncTbl =
{
    EEPROM_Read, EEPROM_Write, NULL, NULL, NULL, NULL, NULL, NULL
};
#endif

/// use for ISR massage.
#if defined(CC_IDE_ENABLE) || defined(CC_FCI_ENABLE)
static STORG_REQ_T _rStorgISRReq;
#endif

/// use for storage close massage.
static STORG_REQ_T _rStorgDummyReq;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

#if defined(CC_IDE_ENABLE) || defined(CC_FCI_ENABLE)

//-------------------------------------------------------------------------
/** _Storge_Hotswap
 *  HISR send hotswap signal to storage thread.
 *  @param  u4DevFlag   device flag.
 *  @param  fgStatus      0: card insert, 1: card remove.
 *  @return   VOID
 */
//-------------------------------------------------------------------------

static VOID _Storge_Hotswap(UINT32 u4DevFlag, INT32 fgStatus)
{
    VOID *ppv_msg;

    _rStorgISRReq.eOp = ((fgStatus == 0) ? STORG_OP_INSERT : STORG_OP_REMOVE);

    // check hot plug device.
    switch (u4DevFlag)
    {
#ifdef CC_IDE_ENABLE
    case STORG_FLAG_CF:
        _rStorgISRReq.prDev = &_rStorgDevCF;
        break;
#endif

#ifdef CC_FCI_ENABLE
    case STORG_FLAG_FCI:
        _rStorgISRReq.prDev = &_rStorgDevFCI;
        break;
#endif

    default:
        ASSERT(0);
    }

    ppv_msg = (VOID *)&_rStorgISRReq;

    // Send request to storage task.
    VERIFY(OSR_OK == x_msg_q_send(_rStorgISRReq.prDev->prStorgThrd->hRequestMsg,
                                  (const VOID *)&ppv_msg,
                                  (SIZE_T)sizeof(VOID *),
                                  STORG_MSG_PROI));
}
#endif // defined(CC_IDE_ENABLE) || defined(CC_FCI_ENABLE)

#ifdef CC_IDE_ENABLE
//-------------------------------------------------------------------------
/** _Storge_Hotswap
 *  Handle CF hotswap. Use gpio as h/w interrupt pin.
 *  @param  i4Gpio       h/w pin number.
 *  @param  fgStatus      0: card insert, 1: card remove.
 *  @return   VOID
 */
//-------------------------------------------------------------------------
static VOID _Storge_CFCallback(INT32 i4Gpio, BOOL fgStatus)
{
    UNUSED(i4Gpio);
    _Storge_Hotswap(STORG_FLAG_CF, (INT32)fgStatus);
}
#endif // CC_IDE_ENABLE

#ifdef CC_FCI_ENABLE
//-------------------------------------------------------------------------
/** _Storge_FCICallback
 *  Handle FCI hotswap.
 *  @param  fgStatus      0: card insert, 1: card remove.
 *  @return   VOID
 */
//-------------------------------------------------------------------------
static VOID _Storge_FCICallback(INT32 fgStatus)
{
    _Storge_Hotswap(STORG_FLAG_FCI, fgStatus);
}
#endif // CC_FCI_ENABLE

//-------------------------------------------------------------------------
/** _Storge_DoIORequest
 *  Handle IO request.
 *  @param  prReq       point to IO request structure.
 *  @retval   COND_FAIL     fail.
 *  @retval   Others            success.
 */
//-------------------------------------------------------------------------
static STORG_COND_T _Storge_DoIORequest(const STORG_REQ_T *prReq)
{
    INT32 i4Ret;

    STORG_FUNCTBL_T *prFuncTbl;
    STORG_COND_T eResult = COND_HOTSWAP_FAIL;
#ifdef CC_FCI_ENABLE
    STORG_DEV_T *prDev = NULL;
#endif

    ASSERT(prReq != NULL);
    ASSERT(prReq->prDev != NULL);
    ASSERT(prReq->prDev->prFuncTbl != NULL);

    prFuncTbl = prReq->prDev->prFuncTbl;

#ifdef CC_NAND_ENABLE
    // This is for setting debugger break point.
    if (prReq->prDev == &_rStorgDevNAND)
    {
        i4Ret = 0;
    }
#endif

    switch (prReq->eOp)
    {
    case STORG_OP_INSERT:
        ASSERT(prFuncTbl->pfnInsert != NULL);
        if (prFuncTbl->pfnInsert == NULL)
        {
            return COND_FAIL;
        }
        i4Ret = prFuncTbl->pfnInsert();

        // check card type and report to middleware.
        if (i4Ret < 0)
        {
            eResult = COND_HOTSWAP_FAIL;
        }
        else
        {
#ifdef CC_FCI_ENABLE
            if (i4Ret == (INT32)STORG_FLAG_MS)
            {                
                prDev = &_rStorgDevMS;
                eResult = COND_INSERT_OK;
                
                // send notify to middleware.
                if (prDev->pfnHotswapNfy)
                {
                    prDev->pfnHotswapNfy(prDev->pvHotswapTag, eResult, NULL);
                }
            }
            else if (i4Ret == (INT32)STORG_FLAG_SD)
            {                
                prDev = &_rStorgDevSD;
                eResult = COND_INSERT_OK;
                
                // send notify to middleware.
                if (prDev->pfnHotswapNfy)
                {
                    prDev->pfnHotswapNfy(prDev->pvHotswapTag, eResult, NULL);
                }
            }
    #ifdef CC_FCI_SMXD_SUPPORT            
            else if (i4Ret == (INT32)STORG_FLAG_SMXD)
            {
                prDev = &_rStorgDevSMXD;
                
                eResult = COND_INSERT_OK;
                // send notify to middleware.
                if (prDev->pfnHotswapNfy)
                {
                    prDev->pfnHotswapNfy(prDev->pvHotswapTag, eResult, NULL);
                }
            }
    #endif            
            else
            {
                prDev = &_rStorgDevSD;
                eResult = COND_HOTSWAP_FAIL;
                
                // send notify to middleware.
                if (prDev->pfnHotswapNfy)
                {
                    prDev->pfnHotswapNfy(prDev->pvHotswapTag, eResult, NULL);
                }
            }
            
            // Saved medium prDev to FCI pvHotswapTag for removal
            prReq->prDev->pvHotswapTag = (VOID*)prDev;
#else
            if (i4Ret == 0)
            {
                eResult = COND_INSERT_OK;
            }
#endif
        }

        return eResult;

    case STORG_OP_REMOVE:
        ASSERT(prFuncTbl->pfnRemove != NULL);
        if (prFuncTbl->pfnRemove == NULL)
        {
            return COND_FAIL;
        }
        
        i4Ret = prFuncTbl->pfnRemove();
        // send notify to middleware.
        
#ifdef CC_FCI_ENABLE        
        if (prReq->prDev->u4DevFlag == STORG_FLAG_FCI)
        {
            // Saved medium prDev to FCI pvHotswapTag for removal
            if (prReq->prDev->pvHotswapTag != NULL)
            {        
                prDev = (STORG_DEV_T *)prReq->prDev->pvHotswapTag;
                eResult = (i4Ret == 0) ? COND_REMOVE_OK : COND_HOTSWAP_FAIL;
            
                // send notify to middleware.
                if (prDev->pfnHotswapNfy)
                {
                    prDev->pfnHotswapNfy(prDev->pvHotswapTag, eResult, NULL);
                }
            
                if (i4Ret == 0)
                {
                    return COND_REMOVE_OK;
                }
            }
        }
        else
#endif      
        {
            if (prReq->prDev->pfnHotswapNfy)
            {
                eResult = (i4Ret == 0) ? COND_REMOVE_OK : COND_HOTSWAP_FAIL;
                prReq->prDev->pfnHotswapNfy(prReq->prDev->pvHotswapTag, eResult, NULL);
            }
            
            if (i4Ret == 0)
            {
                return COND_REMOVE_OK;
            }
        }        
        break;

    case STORG_OP_NONE:
        return COND_ERASE_OK;

    case STORG_OP_INIT:
        return COND_INIT_OK;
        
    case STORG_OP_UNINIT:
        #ifdef CC_NAND_ENABLE
        {
            UINT32 i;
            UINT32 u4TotalPartNum = NANDPART_GetMaxPartId();
            for (i = 0 ; i < u4TotalPartNum ; i++)
            {
                NANDPART_UnMount((UINT8)i);
            }
        }
        #endif
        return COND_UNINIT_OK;
        
    case STORG_OP_READ:
        ASSERT(prFuncTbl->pfnRead != NULL);
        if (prFuncTbl->pfnRead == NULL)
        {
            return COND_FAIL;
        }
        
        i4Ret = prFuncTbl->pfnRead(prReq->u8Offset, (UINT32)prReq->pvBuf, prReq->u4Count);
        if (i4Ret == 0)
        {
            return COND_READ_OK;
        }
        break;

    case STORG_OP_WRITE:
        ASSERT(prFuncTbl->pfnWrite != NULL);
        if (prFuncTbl->pfnWrite == NULL)
        {
            return COND_FAIL;
        }
        
        i4Ret = prFuncTbl->pfnWrite(prReq->u8Offset, (UINT32)prReq->pvBuf, prReq->u4Count);
        if (i4Ret == 0)
        {
            return COND_WRITE_OK;
        }
        break;

    case STORG_OP_ERASE:
        if (prFuncTbl->pfnErase == NULL)
        {
            return COND_ERASE_OK;
        }
        
        i4Ret = prFuncTbl->pfnErase(prReq->u8Offset, prReq->u4Count);
        if (i4Ret == 0)
        {
            return COND_ERASE_OK;
        }
        break;

    case STORG_OP_FLUSH:
        ASSERT(prFuncTbl->pfnFlush != NULL);
        if (prFuncTbl->pfnFlush == NULL)
        {
            return COND_FAIL;
        }
        
        i4Ret = prFuncTbl->pfnFlush();
        if (i4Ret == 0)
        {
            return COND_FLUSH_OK;
        }
        break;

    case STORG_OP_STDBYTIME:
        ASSERT(prFuncTbl->pfnStdbyTime != NULL);
        if (prFuncTbl->pfnStdbyTime == NULL)
        {
            return COND_FAIL;
        }
        
        i4Ret = prFuncTbl->pfnStdbyTime(prReq->u4Count);
        if (i4Ret == 0)
        {
            return COND_STDBYTIME_OK;
        }
        break;
        
    case STORG_OP_IOCTL:
        ASSERT(prFuncTbl->pfnIoCtl != NULL);
        if (prFuncTbl->pfnIoCtl == NULL)
        {
            return COND_FAIL;
        }
        
        i4Ret = prFuncTbl->pfnIoCtl(prReq->u4Count, prReq->u8Offset);
        if (i4Ret == 0)
        {
            return COND_WRITE_OK;
        }
        break;

    default:
        LOG(1, "STORG_OP is invalid %d\n", prReq->eOp);
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return COND_FAIL;
    }

    return COND_FAIL;
}

//-------------------------------------------------------------------------
/** _Storge_get_cb_type
 *  convert storage result to block device result
 *  @param  t_eResult       STORG_COND_T
 *  @retval  return BLKDEV_COND_T
 */
//-------------------------------------------------------------------------
static BLKDEV_COND_T _Storge_get_cb_type(STORG_COND_T t_eResult)
{
    switch (t_eResult)
    {
        case COND_FAIL:           return BLKDEV_COND_FAIL;
        case COND_READ_OK:        return BLKDEV_COND_READ_OK;
        case COND_WRITE_OK:       return BLKDEV_COND_WRITE_OK;
        case COND_ERASE_OK:       return BLKDEV_COND_ERASE_OK;
        case COND_FLUSH_OK:       return BLKDEV_COND_FLUSH_OK;
        
        case COND_INIT:        
        case COND_INIT_OK:     
        case COND_UNINIT_OK:   
        case COND_STDBYTIME_OK:
        case COND_INSERT_OK:   
        case COND_REMOVE_OK:   
        case COND_HOTSWAP_FAIL:
        default:
          Printf("%s(%d) unsupported eResult 0x%x\n", __FUNCTION__, __LINE__, t_eResult);
          ASSERT(0);
    }

    return BLKDEV_COND_FAIL;
}

//-------------------------------------------------------------------------
/** _Storge_HandleResult
 *  Handle the result of IO request.
 *  @param  prReq       point to IO request structure.
 *  @param  eResult     the result of IO request.
 *  @retval   COND_FAIL     fail.
 *  @retval   Others            success.
 */
//-------------------------------------------------------------------------
static VOID _Storge_HandleResult(STORG_REQ_T *prReq, STORG_COND_T eResult)
{
    ASSERT(prReq != NULL);
    ASSERT(prReq->prDev != NULL);
    ASSERT(prReq->prDev->prStorgThrd != NULL);

    switch (prReq->eOp)
    {
    case STORG_OP_INIT:
    case STORG_OP_UNINIT:
    case STORG_OP_READ:
    case STORG_OP_WRITE:
    case STORG_OP_ERASE:
    case STORG_OP_FLUSH:
    case STORG_OP_STDBYTIME:
    case STORG_OP_IOCTL:
        // set result.
        prReq->eResult = eResult;

        // set r/w data count.
        if (prReq->pu4XferCnt)
        {
            *(prReq->pu4XferCnt) = (eResult == COND_FAIL) ? 0 : prReq->u4Count;
        }

        // Handle IO sync and async operation.
        if (prReq->pfnNfy == NULL)
        {
            if (prReq->fgInvokedByDriver == TRUE)
            {
                STORGCloseDev(prReq->prDev);
            }
            VERIFY(OSR_OK == x_sema_unlock(prReq->prDev->prStorgThrd->hSemaDone));
        }
        else
        {
            prReq->pfnNfy(prReq->pvTag, _Storge_get_cb_type(eResult), prReq->pvData);

            if (prReq->fgInvokedByDriver == TRUE)
            {
                STORGCloseDev(prReq->prDev);
            }
            
            // ASYNC: free memory after call back function.
            x_mem_free((VOID *)prReq);
        }
        break;

    case STORG_OP_NONE:
    case STORG_OP_INSERT:
    case STORG_OP_REMOVE:
        break;

    default:
        LOG(1, "STORG_OP is invalid %d\n", prReq->eOp);
        ASSERT(0);
    }
}

//-------------------------------------------------------------------------
/** _Storge_Thread
 *  Dedicated thread for different IO requests.
 *  @param  pvData       thread data point to STORG_THRD_T.
 *  @return   VOID
 */
//-------------------------------------------------------------------------
static VOID _Storge_Thread(const VOID *pvData)
{
    STORG_THRD_T *prSTD;
    STORG_REQ_T *prReq;
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    STORG_COND_T eResult;
#ifdef CC_FCI_ENABLE
    INT32 i4Ret = 0;
#endif

    ASSERT(pvData != NULL);
    prSTD = *(STORG_THRD_T **)pvData;
    ASSERT(prSTD != NULL);

    while (1)
    {
        zMsgSize = sizeof(VOID *);

        // receive request message.
        VERIFY(x_msg_q_receive(&u2MsgQIdx, (VOID *)&prReq, (SIZE_T *)&zMsgSize,
                               &(prSTD->hRequestMsg), 1, X_MSGQ_OPTION_WAIT) == OSR_OK);

        ASSERT(zMsgSize == sizeof(VOID *));

        // check thread is terminated or not ?
        if (!_fgStorgRun)
        {
            break;
        }

        // Do the IO Request.
#ifdef CC_FCI_ENABLE
        if (i4Ret == 0)
        {
            eResult = _Storge_DoIORequest(prReq);
        }
        else
        {
            eResult = COND_FAIL;
        }
#else
        eResult = _Storge_DoIORequest(prReq);
#endif
        // handle IO result.
        _Storge_HandleResult(prReq, eResult);
    }

    VERIFY(OSR_OK == x_sema_unlock(prSTD->hSemaExit));
}

//-------------------------------------------------------------------------
/** _Storge_Setup
 *  Dedicated thread setup function.
 *  @param  prSTD           thread data point to STORG_THRD_T.
 *  @param  u4DevFlag    device flag.
 *  @param  szName       thread name.
 *  @return   VOID
 */
//-------------------------------------------------------------------------
static VOID _Storge_Setup(STORG_THRD_T *prSTD, UINT32 u4DevFlag, const CHAR *szName)
{
    INT32 i4Ret;

    CHAR szBuf[16];

    ASSERT(prSTD != NULL);
    ASSERT(szName != NULL);

    UNUSED(i4Ret);
    x_memset(prSTD, 0, sizeof(STORG_THRD_T));

    // 1. setup device flags.
    prSTD->u4DevFlag = u4DevFlag;

    // 2. create semaphore for thread exit.
    i4Ret = x_sema_create(&prSTD->hSemaExit, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
    ASSERT(i4Ret == OSR_OK);

    // 3. create semaphore for reentry of STORGSendReq().
    i4Ret = x_sema_create(&prSTD->hSemaLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
    ASSERT(i4Ret == OSR_OK);

    // 4. create semaphore for sync operation.
    i4Ret = x_sema_create(&prSTD->hSemaDone, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
    ASSERT(i4Ret == OSR_OK);

    // 5. create request message queue.
    SPrintf(szBuf, "SREQ%X", u4DevFlag);
    i4Ret = x_msg_q_create(&prSTD->hRequestMsg, szBuf, sizeof(STORG_REQ_T),
    STORG_MSG_NUM);
    ASSERT(i4Ret == OSR_OK);

    // 4. create thread for io context.
    i4Ret = x_thread_create(&prSTD->hThread, szName, STORG_STACK_SIZE,
        STORG_THREAD_PRIORITY, (x_os_thread_main_fct)_Storge_Thread, sizeof(STORG_THRD_T *), (VOID *)&prSTD);
    ASSERT(i4Ret == OSR_OK);
}

//-------------------------------------------------------------------------
/** _Storge_Close
 *  Dedicated thread close function.
 *  @param  prSTD           thread data point to STORG_THRD_T.
 *  @return   VOID
 */
//-------------------------------------------------------------------------
static VOID _Storge_Close(STORG_THRD_T *prSTD)
{
    ASSERT(prSTD != NULL);

    if (prSTD->u4DevFlag == 0)
    {
        ASSERT(0);

        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return;
    }

    x_memset(&_rStorgDummyReq, 0, sizeof(STORG_REQ_T));
    // Send dummy request to wake up storage task.
    VERIFY(OSR_OK == x_msg_q_send(prSTD->hRequestMsg, (const VOID *)&_rStorgDummyReq,
                                  (SIZE_T)sizeof(VOID *), STORG_MSG_PROI));

    // wait thread exist.
    VERIFY(OSR_OK == x_sema_lock(prSTD->hSemaExit, X_SEMA_OPTION_WAIT));

    // delete thread object.
    VERIFY(OSR_OK == x_sema_delete(prSTD->hSemaExit));
    VERIFY(OSR_OK == x_msg_q_delete(prSTD->hRequestMsg));

    // clean thread structure.
    x_memset(prSTD, 0, sizeof(STORG_THRD_T));
}

//-------------------------------------------------------------------------
/** _Storge_InitDev
 *  Initial dedicated device data structure.
 *  @param  u4DevFlag     selected device flag.
 *  @return   VOID
 */
//-------------------------------------------------------------------------
static VOID _Storge_InitDev(UINT32 u4DevFlag)
{
    STORG_DEV_T *prDev;

    STORG_THRD_T *prStorgThrd;
    STORG_FUNCTBL_T *prFuncTbl;

    // Check support list.
    if ((STORG_SUPPORT_FLAG & u4DevFlag) == 0)
    {
        LOG(1, "%s(%d) Open 0x%08x device failed\n", __FILE__, __LINE__, u4DevFlag);

        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return;
    }

    switch (u4DevFlag)
    {
#ifdef CC_IDE_ENABLE
    case STORG_FLAG_CF:
        prFuncTbl = &_rCFFuncTbl;
        prStorgThrd = &_rSTDIde;
        prDev = &_rStorgDevCF;
        break;

    case STORG_FLAG_HD:
        prFuncTbl = &_rHDFuncTbl;
        prStorgThrd = &_rSTDIde;
        prDev = &_rStorgDevHD;
        break;
#endif

#ifdef CC_FCI_ENABLE
    // fci module only use to detect card.
    case STORG_FLAG_FCI:
        prFuncTbl = &_rFCIFuncTbl;
        prStorgThrd = &_rSTDFlash;
        prDev = &_rStorgDevFCI;
        break;

    case STORG_FLAG_MS:
        prFuncTbl = &_rMSFuncTbl;
        prStorgThrd = &_rSTDFlash;
        prDev = &_rStorgDevMS;
        break;

    case STORG_FLAG_SD:
        prFuncTbl = &_rSDFuncTbl;
        prStorgThrd = &_rSTDFlash;
        prDev = &_rStorgDevSD;
        break;

#ifdef CC_FCI_SMXD_SUPPORT
    case STORG_FLAG_SMXD:
        prFuncTbl = &_rSMXDFuncTbl;
        prStorgThrd = &_rSTDFlash;
        prDev = &_rStorgDevSMXD;
        break;
#endif
#endif

#ifdef CC_NAND_ENABLE
    case STORG_FLAG_NAND:
        prFuncTbl = &_rNANDFuncTbl;
        prStorgThrd = &_rSTDFlash;
        prDev = &_rStorgDevNAND;
        
    // register callback function for raw R/W API of linux-2.6.27/drivers/mtd/mtdsdm.c
    #if defined(__KERNEL__)
        {
            UINT32 u4SDMPart1, u4SDMPart2;
            u4SDMPart1 = (UINT32)DRVCUST_OptGet(eNANDFlashDynamicPartitionID);            
            u4SDMPart2 = (UINT32)DRVCUST_OptGet(eNANDFlashPartEnd);
            RegisterSDMOpsCB(u4SDMPart1, u4SDMPart2, NANDPART_Read, NANDPART_Write);
        }
    #endif
        break;
#endif

#ifdef CC_MSDC_ENABLE
    case STORG_FLAG_MSDC:
        prFuncTbl = &_rMSDCFuncTbl;
        prStorgThrd = &_rSTDFlash;
        prDev = &_rStorgDevMSDC;
        break;
#endif

#ifndef CC_NOR_DISABLE
    case STORG_FLAG_NOR:
        prFuncTbl = &_rNORFuncTbl;
        prStorgThrd = &_rSTDFlash;
        prDev = &_rStorgDevNOR;
        break;
#endif

#ifdef CC_EEPROM_ENABLE
    case STORG_FLAG_EEPROM:
        prFuncTbl = &_rEEPROMFuncTbl;
        prStorgThrd = &_rSTDFlash;
        prDev = &_rStorgDevEEPROM;
        break;
#endif

    default:
        Printf("%s(%d) Open 0x%08x device failed\n", __FILE__, __LINE__, u4DevFlag);
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return;
    }

    if (prDev->fgInit == 0)
    {
        prDev->eDevType = ((u4DevFlag&STORG_CHAR_FLAG) ? STORG_TYPE_CHR : STORG_TYPE_BLK);
        prDev->u4DevFlag = u4DevFlag;
        prDev->prStorgThrd = prStorgThrd;
        prDev->prFuncTbl = prFuncTbl;
        prDev->pvHotswapTag = NULL;
        prDev->pfnHotswapNfy = NULL;
        prDev->fgInit = 1;
    }
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** STORGInit
 *  storage system initial function.
 *  @param  VOID.
 *  @retval   0     Success
 *  @retval   others     fail
 */
//-------------------------------------------------------------------------
INT32 STORGInit(VOID)
{
#if defined(CC_IDE_ENABLE) || defined(CC_FCI_ENABLE) || defined(CC_EEPROM_ENABLE)
    INT32 i4Ret;
    UINT32 u4SysWP, u4SysWPEnable, u4EdidWP, u4EdidWPEnable;
#endif

#if defined(CC_FCI_ENABLE) || defined(CC_EEPROM_ENABLE)
    INT32 fgSet;
#endif

#ifdef CC_EEPROM_ENABLE
    DTVCFG_T rDtvCfg;
#endif

#ifdef CC_IDE_ENABLE
    INT32 i4Gpio = 9;
#endif

    if (_fgStorgInit)
    {
        return 0;
    }

    _fgStorgInit = TRUE;
    UNUSED(i4Ret);

#ifdef CC_IDE_ENABLE
    i4Ret = IDE_Init();
#endif

    // create IO threads.
    _fgStorgRun = TRUE;

#if defined(CC_NAND_ENABLE) || !defined(CC_NOR_DISABLE) || defined(CC_EEPROM_ENABLE)
#ifdef CC_FCI_ENABLE
    _Storge_Setup(&_rSTDFlash, STORG_FLAG_NAND | STORG_FLAG_NOR | STORG_FLAG_EEPROM |
                  STORG_FLAG_MS | STORG_FLAG_SD | STORG_FLAG_SMXD, STORG_FLASH_NAME);
#else
    _Storge_Setup(&_rSTDFlash, STORG_FLAG_NAND | STORG_FLAG_NOR | STORG_FLAG_EEPROM,
                  STORG_FLASH_NAME);
#endif
#endif

#ifdef CC_IDE_ENABLE
    _Storge_Setup(&_rSTDIde, STORG_FLAG_CF | STORG_FLAG_HD, STORG_IDE_NAME);
#endif

#ifdef CC_IDE_ENABLE
    _Storge_InitDev(STORG_FLAG_HD);
    _Storge_InitDev(STORG_FLAG_CF);
#endif

#ifdef CC_FCI_ENABLE
    _Storge_InitDev(STORG_FLAG_MS);
    _Storge_InitDev(STORG_FLAG_SD);

#ifdef CC_FCI_SMXD_SUPPORT    
    _Storge_InitDev(STORG_FLAG_SMXD);
#endif
    _Storge_InitDev(STORG_FLAG_FCI);

    // Setup MS/SD hotswap interface function.
    FCIHW_SetCardDetectFct(_Storge_FCICallback);

    i4Ret = FCI_Init();
#endif

#ifdef CC_NAND_ENABLE
    _Storge_InitDev(STORG_FLAG_NAND);
#endif

#ifdef CC_MSDC_ENABLE
    _Storge_InitDev(STORG_FLAG_MSDC);
#endif

#ifndef CC_NOR_DISABLE
    _Storge_InitDev(STORG_FLAG_NOR);
#endif

#ifdef CC_EEPROM_ENABLE
    rDtvCfg.u1Flags = 0;
    _Storge_InitDev(STORG_FLAG_EEPROM);

    if ((0 == DRVCUST_OptQuery(eEdidEepromWPGpio, &u4EdidWP)) && 
        (0 == DRVCUST_OptQuery(eEdidEepromWPEnablePolarity, &u4EdidWPEnable)))
    {
         i4Ret = EEPDTV_Read((UINT64)EEPDTV_DTVCFG_MAGIC1, (UINT32)(void *)&(rDtvCfg), sizeof(rDtvCfg));
        if (i4Ret != 0)
        {
            rDtvCfg.u1Flags = 0;
        }

        //initial eeprom write protect by GPIO.
        fgSet = (INT32)((rDtvCfg.u1Flags & DTVCFG_FLAG_FACTORY_MODE) ? (!u4EdidWPEnable) : (u4EdidWPEnable));
        VERIFY(GPIO_Output((INT32)u4EdidWP, &fgSet) == fgSet);
    }

    if (0 == DRVCUST_OptQuery(eSysEepromWPGpio, &u4SysWP) && 
       (0 == DRVCUST_OptQuery(eSysEepromWPEnablePolarity, &u4SysWPEnable)))
    {
        //initial eeprom write protect by GPIO.
#if !defined(CC_MT5365)         
        VERIFY(BSP_PinGpioSel((INT32)u4SysWP) == 0);
#endif        
        fgSet = (INT32)u4SysWPEnable;
        VERIFY(GPIO_Output((INT32)u4SysWP, &fgSet) == fgSet);
    }
#endif

    return 0;
}

//-------------------------------------------------------------------------
/** STORGStop
 *  storage system exit function.
 *  @param  VOID.
 *  @retval   0     Success
 *  @retval   others     fail
 */
//-------------------------------------------------------------------------
INT32 STORGStop(VOID)
{
    if (!_fgStorgInit)
    {
        return 1;
    }

    _fgStorgInit = FALSE;
    // delete IO threads.
    _fgStorgRun = FALSE;

#if defined(CC_NAND_ENABLE) || !defined(CC_NOR_DISABLE) || defined(CC_EEPROM_ENABLE)  || defined(CC_FCI_ENABLE)
    _Storge_Close(&_rSTDFlash);
#endif

#ifdef CC_IDE_ENABLE
    _Storge_Close(&_rSTDIde);
#endif

#ifdef CC_IDE_ENABLE
    IDE_Stop();
#endif

#ifdef CC_FCI_ENABLE
    VERIFY(0 == FCI_Stop());
#endif

#ifdef CC_NAND_ENABLE
    NAND_Stop();
#endif

    return 0;
}

//-------------------------------------------------------------------------
/** STORGOpenDev
 *  open selected device.
 *  @param  u4DevFlag   selected device flag.
 *  @return  address of pointer to STORG_DEV_T
 */
//-------------------------------------------------------------------------
STORG_DEV_T *STORGOpenDev(UINT32 u4DevFlag)
{
    STORG_DEV_T *prDev;
        
    VERIFY(0 == STORGInit());

    // Check support list.
    if ((STORG_SUPPORT_FLAG & u4DevFlag) == 0)
    {
        LOG(1, "%s(%d) Open 0x%08x device failed\n", __FILE__, __LINE__, u4DevFlag);

        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return NULL;
    }

    switch (u4DevFlag)
    {

#ifdef CC_IDE_ENABLE
    case STORG_FLAG_CF:
        prDev = &_rStorgDevCF;
        break;

    case STORG_FLAG_HD:
        prDev = &_rStorgDevHD;
        break;
#endif

#ifdef CC_FCI_ENABLE
case STORG_FLAG_FCI:
        prDev = &_rStorgDevFCI;
        break;

    case STORG_FLAG_MS:
        prDev = &_rStorgDevMS;
        break;

    case STORG_FLAG_SD:
        prDev = &_rStorgDevSD;
        break;

    case STORG_FLAG_SMXD:
        prDev = &_rStorgDevSMXD;
        break;
#endif

#ifdef CC_NAND_ENABLE
    case STORG_FLAG_NAND:
        prDev = &_rStorgDevNAND;
        break;
#endif

#ifdef CC_MSDC_ENABLE
    case STORG_FLAG_MSDC:
        prDev = &_rStorgDevMSDC;
        break;
#endif

#ifndef CC_NOR_DISABLE
    case STORG_FLAG_NOR:
        prDev = &_rStorgDevNOR;
        break;
#endif

#ifdef CC_EEPROM_ENABLE
    case STORG_FLAG_EEPROM:
        prDev = &_rStorgDevEEPROM;
        break;
#endif

    default:
        LOG(1, "%s(%d) Open 0x%08x device failed\n", __FILE__, __LINE__, u4DevFlag);
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return NULL;
    }

    ASSERT(prDev->fgInit != 0);

    if (prDev->fgInit == 0)
    {
        return NULL;
    }

    prDev->i4RefCnt++;
    return prDev;
}

//-------------------------------------------------------------------------
/** STORGCloseDev
 *  close selected device.
 *  @param  prDev   selected device structure to close.
 *  @retval     0       success
  *  @retval    1       fail
 */
//-------------------------------------------------------------------------
INT32 STORGCloseDev(STORG_DEV_T *prDev)
{
    if (!_fgStorgInit)
    {
        return 1;
    }

    ASSERT(prDev != NULL);

    switch (prDev->u4DevFlag)
    {

#ifdef CC_IDE_ENABLE
    case STORG_FLAG_CF:
        _rStorgDevCF.i4RefCnt--;
        break;

    case STORG_FLAG_HD:
        _rStorgDevHD.i4RefCnt--;
        break;
#endif

#ifdef CC_FCI_ENABLE
    case STORG_FLAG_FCI:
        _rStorgDevFCI.i4RefCnt--;
        break;
        
    case STORG_FLAG_MS:
        _rStorgDevMS.i4RefCnt--;
        break;

    case STORG_FLAG_SD:
        _rStorgDevSD.i4RefCnt--;
        break;

    case STORG_FLAG_SMXD:
        _rStorgDevSMXD.i4RefCnt--;
        break;
#endif

#ifdef CC_NAND_ENABLE
    case STORG_FLAG_NAND:
        _rStorgDevNAND.i4RefCnt--;
        break;
#endif

#ifdef CC_MSDC_ENABLE
    case STORG_FLAG_MSDC:
        _rStorgDevMSDC.i4RefCnt--;
        break;
#endif

#ifndef CC_NOR_DISABLE
    case STORG_FLAG_NOR:
        _rStorgDevNOR.i4RefCnt--;
        break;
#endif

#ifdef CC_EEPROM_ENABLE
    case STORG_FLAG_EEPROM:
        _rStorgDevEEPROM.i4RefCnt--;
        break;
#endif

    default:
        LOG(1, "%s(%d) Close 0x%08x->0x%08x device failed\n", __FILE__, __LINE__, (UINT32)prDev,
            prDev->u4DevFlag);
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return 1;
    }

    return 0;
}

//-------------------------------------------------------------------------
/** STORGSendReq
 *  Send IO request message to storage thread.
 *  @param  prDev   selected device structure.
 *  @param  eOp      IO function ID.
 *  @param  pvData   user's data pointer. use in async notified callback function.
 *  @param  pvTag    user's data pointer. use in async notified callback function.
 *  @param  pfnNfy   async notified callback function pointer.
 *  @param  u8Offset   IO start offset.
 *  @param  u4Count   IO data count.
 *  @param  pvBuf       IO data buffer pointer.
 *  @param  pu4XferCnt    IO thread return the processed data size.
 *  @retval     0       success
  *  @retval    1       fail
 */
//-------------------------------------------------------------------------
INT32 STORGSendReq(STORG_DEV_T *prDev, STORG_OP_T eOp, VOID *pvData, VOID *pvTag,
                   STORG_NFY_FCT pfnNfy, UINT64 u8Offset, UINT32 u4Count, VOID *pvBuf,
                   UINT32 *pu4XferCnt)
{
    STORG_REQ_T *prReq;
    INT32 i4Ret = 0;
    INT32 i4SendRet = 0;
    STORG_FUNCTBL_T *prFuncTbl;

    if (!_fgStorgInit)
    {
        ASSERT(0);

        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return 1;
    }

    if (prDev == NULL)
    {
        ASSERT(0);

        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return 1;
    }

    ASSERT(prDev->prStorgThrd);
    ASSERT(prDev->prStorgThrd->hRequestMsg);

    // prevent race condition.
    i4Ret = x_sema_lock(prDev->prStorgThrd->hSemaLock, X_SEMA_OPTION_WAIT);
    if (i4Ret != OSR_OK)
    {
        Printf("x_sema_lock error = %d", i4Ret);
        ASSERT(0);

        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return 1;
    }

    if (prDev->prFuncTbl == NULL)
    {
        ASSERT(0);

        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return 1;
    }

    prFuncTbl = prDev->prFuncTbl;

#ifdef CC_NAND_ENABLE
    // This is for setting debugger break point.
    if (prDev == &_rStorgDevNAND)
    {
        i4Ret = 0;
    }
#endif

    switch (eOp)
    {
    case STORG_OP_NONE:
        break;

    case STORG_OP_INIT:
        break;

    case STORG_OP_UNINIT:
        // This is only used for NAND driver. 
#ifdef CC_NAND_ENABLE
        if (prDev == &_rStorgDevNAND)
        {
            break;
        }
#endif
        return 0;

    case STORG_OP_READ:
        ASSERT(prFuncTbl->pfnRead != NULL);
        if (prFuncTbl->pfnRead == NULL)
        {
            ASSERT(prFuncTbl->pfnRead != NULL);

            ASSERT(0);
            LINT_SUPPRESS_NEXT_EXPRESSION(527);
            return 1;
        }
        break;

    case STORG_OP_WRITE:
        ASSERT(prFuncTbl->pfnWrite != NULL);
        if (prFuncTbl->pfnWrite == NULL)
        {
            ASSERT(prFuncTbl->pfnWrite != NULL);

            ASSERT(0);
            LINT_SUPPRESS_NEXT_EXPRESSION(527);
            return 1;
        }
        break;

    case STORG_OP_ERASE:
#if 1 // no erase in nand block
        if (prFuncTbl->pfnErase == NULL)
        {
            break;
        }
#endif
        break;

    case STORG_OP_FLUSH:
        ASSERT(prFuncTbl->pfnFlush != NULL);
        if (prFuncTbl->pfnFlush == NULL)
        {
            ASSERT(prFuncTbl->pfnFlush != NULL);

            ASSERT(0);
            LINT_SUPPRESS_NEXT_EXPRESSION(527);
            return 1;
        }
        break;

    case STORG_OP_STDBYTIME:
        ASSERT(prFuncTbl->pfnStdbyTime != NULL);
        if (prFuncTbl->pfnStdbyTime == NULL)
        {
            ASSERT(prFuncTbl->pfnStdbyTime != NULL);

            ASSERT(0);
            LINT_SUPPRESS_NEXT_EXPRESSION(527);
            return 1;
        }
        
        pfnNfy = NULL; // Always sync IO.
        pvTag = NULL;
        break;

    case STORG_OP_INSERT:
        ASSERT(prFuncTbl->pfnInsert != NULL);
        if (prFuncTbl->pfnInsert == NULL)
        {
            ASSERT(prFuncTbl->pfnInsert != NULL);

            ASSERT(0);
            LINT_SUPPRESS_NEXT_EXPRESSION(527);
            return 1;
        }
        
        pfnNfy = (STORG_NFY_FCT)prDev->pfnHotswapNfy;
        pvTag = prDev->pvHotswapTag;
        break;

    case STORG_OP_REMOVE:
        ASSERT(prFuncTbl->pfnRemove != NULL);
        if (prFuncTbl->pfnRemove == NULL)
        {
            ASSERT(prFuncTbl->pfnRemove != NULL);

            ASSERT(0);
            LINT_SUPPRESS_NEXT_EXPRESSION(527);
            return 1;
        }
        
        pfnNfy = (STORG_NFY_FCT)prDev->pfnHotswapNfy;
        pvTag = prDev->pvHotswapTag;
        break;
        
    case STORG_OP_IOCTL:
        ASSERT(prFuncTbl->pfnIoCtl!= NULL);
        if (prFuncTbl->pfnIoCtl == NULL)
        {
            ASSERT(prFuncTbl->pfnIoCtl != NULL);

            ASSERT(0);
            LINT_SUPPRESS_NEXT_EXPRESSION(527);
            return 1;
        }
        break;

    default:
        LOG(1, "STORG_OP is invalid %d\n", eOp);
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return 1;
    }

    prReq = x_mem_alloc(sizeof(STORG_REQ_T));
    if (prReq == NULL)
    {
        ASSERT(0);
        return 1;
    }

    prReq->prDev = prDev;
    prReq->eOp = eOp;
    prReq->pvData = pvData;
    prReq->pvTag = pvTag;
    prReq->pfnNfy = pfnNfy;
    prReq->u8Offset = u8Offset;
    prReq->u4Count = u4Count;
    prReq->pvBuf = pvBuf;
    prReq->pu4XferCnt = pu4XferCnt;
    prReq->eResult = COND_INIT;
    prReq->fgInvokedByDriver = FALSE;

    // send only the address of message.
    i4SendRet = x_msg_q_send(prDev->prStorgThrd->hRequestMsg, (const VOID *)&prReq,
                         (SIZE_T)sizeof(VOID *), STORG_MSG_PROI);

    if (i4SendRet)
    {
        LOG(1, "DevFlag:0x%08x accept request failed\n", prDev->u4DevFlag);

        // sync: free memory after check result code.
        x_mem_free((VOID *)prReq);

        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        goto _STGSENDREQEXIT;
    }

    // Handle blocking mode IO function. Wait IO completely.
    if (pfnNfy == NULL)
    {
        VERIFY(OSR_OK == x_sema_lock(prDev->prStorgThrd->hSemaDone, X_SEMA_OPTION_WAIT));

        if (prReq->eResult == COND_FAIL)
        {
            LINT_SUPPRESS_NEXT_EXPRESSION(527);
            i4SendRet = 1;
            goto _STGSENDREQEXIT;
        }

        // sync: free memory after check result code.
        x_mem_free((VOID *)prReq);
    }

    _STGSENDREQEXIT:

    // prevent race condition.
    i4Ret = x_sema_unlock(prDev->prStorgThrd->hSemaLock);
    if (i4Ret != OSR_OK)
    {
        Printf("x_sema_lock error = %d", i4Ret);
        ASSERT(0);

        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return 1;
    }

    return i4SendRet;
}

//-------------------------------------------------------------------------
/** STORG_GetDevSize
 *  Get device size.
 *  @param  u4DevFlag   selected device flag.
 *  @param  ui2_id          partition id, only use in NOR flash device.
 *  @return  device size in bytes.
 */
//-------------------------------------------------------------------------
UINT64 STORG_GetDevSize(UINT32 u4DevFlag, UINT16 ui2_id)
{
    switch (u4DevFlag)
    {
#ifndef CC_NOR_DISABLE
    case STORG_FLAG_NOR:
        return (UINT64)NORPART_GetPartSize(ui2_id);
#endif

#ifdef CC_EEPROM_ENABLE
    case STORG_FLAG_EEPROM:
        return (UINT64)(UINT32)EEPROM_GetSize();
#endif

#ifdef CC_IDE_ENABLE
    case STORG_FLAG_CF:
        return (UINT64)CF_GetSize();
    case STORG_FLAG_HD:
        return (UINT64)HD_GetSize();
#endif

#ifdef CC_FCI_ENABLE
    case STORG_FLAG_MS:
        return (UINT64)MS_GetSize();
    case STORG_FLAG_SD:
        return (UINT64)SD_GetSize();

#ifdef CC_FCI_SMXD_SUPPORT
    case STORG_FLAG_SMXD:
        return (UINT64)SMXD_GetSize();
#endif
#endif

#ifdef CC_NAND_ENABLE
    case STORG_FLAG_NAND:
        return (UINT64)NANDPART_GetPartSize(ui2_id);
#endif

    default:
        ASSERT(0);
    }

    LINT_SUPPRESS_NEXT_EXPRESSION(527);
    return 0;
}

//-------------------------------------------------------------------------
/** STORG_GetDevStatus
 *  Get device status.
 *  @param  u4DevFlag   selected device flag.
 *  @return  device status in STORG_DEV_STATUS_T.
 */
//-------------------------------------------------------------------------
STORG_DEV_STATUS_T STORG_GetDevStatus(UINT32 u4DevFlag)
{
    switch (u4DevFlag)
    {
#ifdef CC_IDE_ENABLE
    case STORG_FLAG_CF:
        return (STORG_DEV_STATUS_T)CF_GetStatus();
    case STORG_FLAG_HD:
        return (STORG_DEV_STATUS_T)HD_GetStatus();
#endif

#ifdef CC_FCI_ENABLE
    case STORG_FLAG_MS:
        return (STORG_DEV_STATUS_T)MS_GetStatus();
    case STORG_FLAG_SD:
        return (STORG_DEV_STATUS_T)SD_GetStatus();

#ifdef CC_FCI_SMXD_SUPPORT
    case STORG_FLAG_SMXD:
        return (STORG_DEV_STATUS_T)SMXD_GetStatus();
#endif
#endif

    default:
        ASSERT(0);
    }

    LINT_SUPPRESS_NEXT_EXPRESSION(527);
    return (STORG_DEV_STATUS_T)0;
}

//-------------------------------------------------------------------------
/** STORG_GetDevModel
 *  Get device model name.
 *  @param  u4DevFlag   selected device flag.
 *  @return  model name string.
 */
//-------------------------------------------------------------------------
CHAR *STORG_GetDevModel(UINT32 u4DevFlag)
{
    switch (u4DevFlag)
    {
#ifdef CC_IDE_ENABLE
    case STORG_FLAG_HD:
        return HD_GetModel();
    case STORG_FLAG_CF:
        return CF_GetModel();
#endif

#ifdef CC_FCI_ENABLE
    case STORG_FLAG_MS:
        return MS_GetModel();
    case STORG_FLAG_SD:
        return SD_GetModel();

#ifdef CC_FCI_SMXD_SUPPORT
    case STORG_FLAG_SMXD:
        return SMXD_GetModel();
#endif
#endif

    default:
        ASSERT(0);
    }

    LINT_SUPPRESS_NEXT_EXPRESSION(527);
    return NULL;
}

//-------------------------------------------------------------------------
/** STORG_GetDevCard
 *  Get device type.
 *  @param  u4DevFlag   selected device flag.
 *  @return  selected device type.
 */
//-------------------------------------------------------------------------
INT32 STORG_GetDevCard(UINT32 u4DevFlag)
{
    switch (u4DevFlag)
    {
#ifdef CC_FCI_ENABLE
    case STORG_FLAG_MS:
        return MS_GetCard();
    case STORG_FLAG_SD:
        return SD_GetCard();

    #ifdef CC_FCI_SMXD_SUPPORT
    case STORG_FLAG_SMXD:
        return SMXD_GetCard();
    #endif
#endif

    default:
        ASSERT(0);
    }

    LINT_SUPPRESS_NEXT_EXPRESSION(527);
    return 0;
}

//-------------------------------------------------------------------------
/** STORG_GetDevStandbyTime
 *  Get device standby time.
 *  @param  u4DevFlag   selected device flag.
 *  @return  selected device standby time.
 */
//-------------------------------------------------------------------------
INT32 STORG_GetDevStandbyTime(UINT32 u4DevFlag)
{
    switch (u4DevFlag)
    {
    case STORG_FLAG_HD:
#ifdef CC_IDE_ENABLE
        return HD_GetStandbyTime();
#else
        return 0;
#endif

    default:
        ASSERT(0);
    }

    LINT_SUPPRESS_NEXT_EXPRESSION(527);
    return 0;
}

UINT32 STORG_NandGetBlkSize(void)
{
#ifdef CC_NAND_ENABLE
    SECTTBL_T t_sector_tbl;
    StgSEGINFO_T t_seginfo;
    t_sector_tbl.prSegInfo = &t_seginfo;
    t_sector_tbl.u4SegNum = 1;
    
    if (NANDPART_GetSectorTbl(&t_sector_tbl, 0) == 0)
    {
        return t_sector_tbl.prSegInfo[0].u4SectSize;
    }
#endif

    return 0;
}

INT32 STORG_NandAsyncReadBlock(UINT32 u4PartId, UINT32 u4Offset, VOID* pvBuf, UINT32 u4BufSize, UINT32 *pu4XferCnt, STORG_NFY_FCT pfnNfy)
{
#ifdef CC_NAND_ENABLE
    STORG_DEV_T* prDev;
    STORG_REQ_T *prReq;
    INT32 i4SendRet = 0;
    UINT8 u8Offset;
    UINT32 u4BlkSize;
    
    prDev = STORGOpenDev((UINT32)STORG_FLAG_NAND);

    if (prDev == NULL)
    {
        return -1;
    }

    u4BlkSize = STORG_NandGetBlkSize();
    if ((u4BlkSize != 0) && ((u4BufSize%u4BlkSize)!=0))
    {
        LOG(0, "STORG_NandAsyncRead: u4BufSize is not block size aligned !\n");
        return -1;
    }
    if ((u4BlkSize != 0) && ((u4Offset%u4BlkSize)!=0))
    {
        LOG(0, "STORG_NandAsyncWriteBlock: u4Offset is not block size aligned !\n");
        return -1;
    }

    prReq = x_mem_alloc(sizeof(STORG_REQ_T));
    ASSERT(prReq);
    if (prReq == NULL)
    {
    	return -1;
    }
    
    u8Offset = ((UINT64)u4PartId << 32) | u4Offset;
    prReq->prDev = prDev;
    prReq->eOp = STORG_OP_READ;
    prReq->pvData = NULL;
    prReq->pvTag = NULL;
    prReq->pfnNfy = pfnNfy;
    prReq->u8Offset = u8Offset;
    prReq->u4Count = u4BufSize;
    prReq->pvBuf = pvBuf;
    prReq->pu4XferCnt = pu4XferCnt;
    prReq->eResult = COND_INIT;
    prReq->fgInvokedByDriver = TRUE;

    i4SendRet = x_msg_q_send(prDev->prStorgThrd->hRequestMsg, (const VOID *)&prReq,
                         (SIZE_T)sizeof(VOID *), STORG_MSG_PROI);

    if (i4SendRet)
    { 
        x_mem_free((VOID *)prReq); 
        return -1;
    }
#endif

    return 0;
}

INT32 STORG_NandAsyncWriteBlock(UINT32 u4PartId, UINT32 u4Offset, VOID* pvBuf, UINT32 u4BufSize, UINT32 *pu4XferCnt, STORG_NFY_FCT pfnNfy)
{
#ifdef CC_NAND_ENABLE
    STORG_DEV_T* prDev;
    STORG_REQ_T *prReq;
    INT32 i4SendRet = 0;
    UINT8 u8Offset;
    UINT32 u4BlkSize;
    
    prDev = STORGOpenDev((UINT32)STORG_FLAG_NAND);

    if (prDev == NULL)
    {
        return -1;
    }

    u4BlkSize = STORG_NandGetBlkSize();
    if ((u4BlkSize != 0) && ((u4BufSize%u4BlkSize)!=0))
    {
        LOG(0, "STORG_NandAsyncWriteBlock: u4BufSize is not block size aligned !\n");
        return -1;
    }
    if ((u4BlkSize != 0) && ((u4Offset%u4BlkSize)!=0))
    {
        LOG(0, "STORG_NandAsyncWriteBlock: u4Offset is not block size aligned !\n");
        return -1;
    }

    prReq = x_mem_alloc(sizeof(STORG_REQ_T));
    ASSERT(prReq);
    if (prReq == NULL)
    {
    	return -1;
    }
    
    u8Offset = ((UINT64)u4PartId << 32) | u4Offset;
    prReq->prDev = prDev;
    prReq->eOp = STORG_OP_WRITE;
    prReq->pvData = NULL;
    prReq->pvTag = NULL;
    prReq->pfnNfy = pfnNfy;
    prReq->u8Offset = u8Offset;
    prReq->u4Count = u4BufSize;
    prReq->pvBuf = pvBuf;
    prReq->pu4XferCnt = pu4XferCnt;
    prReq->eResult = COND_INIT;
    prReq->fgInvokedByDriver = TRUE;

    i4SendRet = x_msg_q_send(prDev->prStorgThrd->hRequestMsg, (const VOID *)&prReq,
                         (SIZE_T)sizeof(VOID *), STORG_MSG_PROI);

    if (i4SendRet)
    { 
        x_mem_free((VOID *)prReq); 
        return -1;
    }
#endif

    return 0;
}

INT32 STORG_SyncNandRead(UINT32 u4PartId, UINT32 u4Offset, VOID* pvBuf, UINT32 u4BufSize)
{
#if defined(CC_NAND_ENABLE)

#if defined(__KERNEL__) && !defined(LINUX_NAND_USE_SDM)

#include <linux/mtd/mt53xx_part_oper.h>
    if (i4NFBPartitionRead(u4PartId, u4Offset, (UINT32)pvBuf, u4BufSize))
    {
        return -1;
    }
    else
    {
        return u4BufSize;
    }
    
#else   //__KERNEL__

    STORG_DEV_T* prDev;
    UINT64 u8Offset;
    INT32 i4Result;

    prDev = STORGOpenDev((UINT32)STORG_FLAG_NAND);
    if (prDev == NULL)
    {
        return -1;
    }
    
    u8Offset = ((((UINT64)u4PartId)<<32) | u4Offset);
    i4Result = NANDPART_Read(u8Offset, (UINT32)pvBuf, u4BufSize);
    
    STORGCloseDev(prDev);

    if (i4Result != 0)
    {
        return -1;
    }
    else
    {
        return u4BufSize;
    }
    
#endif  //  __KERNEL__

#else   // CC_NAND_ENABLE

    return 0;

#endif  // CC_NAND_ENABLE
}

INT32 STORG_SyncNandReadBlock(UINT32 u4PartId, UINT32 u4Offset, VOID* pvBuf, UINT32 u4BufSize)
{
#ifdef CC_NAND_ENABLE
    STORG_DEV_T* prDev;
    UINT64 u8Offset; 
    UINT32 u4BlkSize;
    INT32 i4Result;

    prDev = STORGOpenDev((UINT32)STORG_FLAG_NAND);
    if (prDev == NULL)
    {
        return -1;
    }
    
    u4BlkSize = STORG_NandGetBlkSize();
    if ((u4BlkSize != 0) && ((u4BufSize%u4BlkSize)!=0))
    {
        LOG(0, "STORG_SyncNandWriteBlock: u4BufSize is not block size aligned !\n");
        return -1;
    }
    if ((u4BlkSize != 0) && ((u4Offset%u4BlkSize)!=0))
    {
        LOG(0, "STORG_NandAsyncWriteBlock: u4Offset is not block size aligned !\n");
        return -1;
    }

    u8Offset = ((((UINT64)u4PartId)<<32) | u4Offset);

    i4Result = NANDPART_Read(u8Offset, (UINT32)pvBuf, u4BufSize);

    STORGCloseDev(prDev);

    if (i4Result == 0)
    {
        return u4BufSize;
    }
    else
    {
        return -1;
    }
#else
    return 0;
#endif
}

INT32 STORG_SyncNandWriteBlock(UINT32 u4PartId, UINT32 u4Offset, VOID* pvBuf, UINT32 u4BufSize)
{
#ifdef CC_NAND_ENABLE
    STORG_DEV_T* prDev;
    UINT64 u8Offset; 
    UINT32 u4BlkSize;
    INT32 i4Result;

    prDev = STORGOpenDev((UINT32)STORG_FLAG_NAND);
    if (prDev == NULL)
    {
        return -1;
    }
    
    u4BlkSize = STORG_NandGetBlkSize();
    if ((u4BlkSize != 0) && ((u4BufSize%u4BlkSize)!=0))
    {
        LOG(0, "STORG_SyncNandWriteBlock: u4BufSize is not block size aligned !\n");
        return -1;
    }
    if ((u4BlkSize != 0) && ((u4Offset%u4BlkSize)!=0))
    {
        LOG(0, "STORG_NandAsyncWriteBlock: u4Offset is not block size aligned !\n");
        return -1;
    }
    
    u8Offset = ((((UINT64)u4PartId)<<32) | u4Offset);

    i4Result = NANDPART_Write(u8Offset, (UINT32)pvBuf, u4BufSize);

    STORGCloseDev(prDev);

    if (i4Result == 0)
    {
        return u4BufSize;
    }
    else
    {
        return -1;
    }
#else
    return 0;
#endif
}

INT32 STORG_SyncMsdcRead(UINT32 u4PartId, UINT32 u4Offset, VOID* pvBuf, UINT32 u4BufSize)
{
#if defined(CC_MSDC_ENABLE)
    STORG_DEV_T* prDev;
    UINT64 u8Offset;
    INT32 i4Result;
    
    prDev = STORGOpenDev((UINT32)STORG_FLAG_MSDC);
    if (prDev == NULL)
    {
        return -1;
    }
    
    u8Offset = ((((UINT64)u4PartId)<<32) | u4Offset);
    i4Result = MSDCPART_Read(u8Offset, (UINT32)pvBuf, u4BufSize);
    
    STORGCloseDev(prDev);

    if (i4Result != 0)
    {
        return -1;
    }
    else
    {
        return u4BufSize;
    }
#else   // CC_MSDC_ENABLE
    return 0;
#endif  // CC_MSDC_ENABLE
}

INT32 STORG_SyncRead(UINT32 u4PartId, UINT32 u4Offset, VOID* pvBuf, UINT32 u4BufSize)
{
    INT32 i4Ret = -1;
    
#if defined(CC_NAND_ENABLE)
    i4Ret = STORG_SyncNandRead(u4PartId, u4Offset, pvBuf, u4BufSize);
#elif defined(CC_MSDC_ENABLE)
    i4Ret = STORG_SyncMsdcRead(u4PartId, u4Offset, pvBuf, u4BufSize);
#elif !defined(CC_NOR_DISABLE)
    i4Ret = NORSTG_Read(u4PartId, u4Offset, (UINT32)pvBuf, u4BufSize);
#endif

    return i4Ret;
}

#endif /* CC_MTK_LOADER */

