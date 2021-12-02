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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: storage.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file storage.c
 *  This C file implements the storage related function. It setup 3 threads which are IDE, FCI, and FLASH.
 *  The interface is also thread safe for multi-thread operation.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifndef CC_5391_LOADER

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_pinmux.h"

#include "nor_debug.h" // for storage.c debug LOG.

#ifndef CC_NOR_ENABLE
#define CC_NOR_ENABLE
#endif

#ifndef CC_EEPROM_ENABLE
#define CC_EEPROM_ENABLE
#endif

#ifdef CC_NOR_ENABLE
#include "nor_if.h"
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

#include "storage_if.h"
#include "drv_common.h"
#include "drvcust_if.h"

#include "blkmgr_if.h"

LINT_EXT_HEADER_END
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define STORG_BLOCK_FLAG      (STORG_FLAG_NAND | STORG_FLAG_CF | STORG_FLAG_HD \
                                                        | STORG_FLAG_MS | STORG_FLAG_SD | STORG_FLAG_SMXD)
#define STORG_CHAR_FLAG       (STORG_FLAG_NOR | STORG_FLAG_EEPROM)
#define STORG_SUPPORT_FLAG    (STORG_BLOCK_FLAG | STORG_CHAR_FLAG)
#define STORG_FLASH_FLAG      (STORG_FLAG_NAND | STORG_FLAG_NOR | STORG_FLAG_EEPROM)
#define STORG_FCI_FLAG        (STORG_FLAG_MS | STORG_FLAG_SD |STORG_FLAG_SMXD)
#define STORG_IDE_FLAG        (STORG_FLAG_CF | STORG_FLAG_HD)

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

// FCI control and data bus noise protected buffer control pin.
//#define FCI_NOISE_BUFFER_PIN     ((INT32)11)

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
EXTERN VOID FCIHW_SetCardDetectFct(FCI_CALLBACK pfnCallback);

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

#if defined(CC_NAND_ENABLE) || defined(CC_NOR_ENABLE) || defined(CC_EEPROM_ENABLE) || defined(CC_FCI_ENABLE)

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

#ifdef CC_NOR_ENABLE

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
    CF_Read, CF_Write, NULL, NULL, NULL, CF_Insert, CF_Remove
};

static STORG_FUNCTBL_T _rHDFuncTbl =
{
    HD_Read, HD_Write, NULL, HD_Flush, HD_SetStdbyTmr, HD_Insert, HD_Remove
};
#endif // CC_IDE_ENABLE 

#ifdef CC_FCI_ENABLE

static STORG_FUNCTBL_T _rFCIFuncTbl =
{
    NULL, NULL, NULL, NULL, NULL, FCI_Init, FCI_Stop
};

static STORG_FUNCTBL_T _rMSFuncTbl =
{
    MS_Read, MS_Write, NULL, NULL, NULL, NULL, NULL
};

static STORG_FUNCTBL_T _rSDFuncTbl =
{
    SD_Read, SD_Write, NULL, NULL, NULL, NULL, NULL
};

static STORG_FUNCTBL_T _rSMXDFuncTbl =
{
    SMXD_Read, SMXD_Write, NULL, NULL, NULL, NULL, NULL
};
#endif // CC_FCI_ENABLE 

#ifdef CC_NOR_ENABLE

static STORG_FUNCTBL_T _rNORFuncTbl =
{
    NORPART_Read, NORPART_Write, NORPART_Erase, NULL, NULL, NULL, NULL
};
#endif

#ifdef CC_NAND_ENABLE

static STORG_FUNCTBL_T _rNANDFuncTbl =
{
    NAND_Read, NAND_Write, NAND_Erase, NULL, NULL, NULL, NULL
};
#endif // CC_NAND_ENABLE 

#ifdef CC_EEPROM_ENABLE

static STORG_FUNCTBL_T _rEEPROMFuncTbl =
{
    EEPROM_Read, EEPROM_Write, NULL, NULL, NULL, NULL, NULL
};
#endif // CC_EEPROM_ENABLE 

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
    case STORG_FCI_FLAG:
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
    _Storge_Hotswap(STORG_FCI_FLAG, fgStatus);
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
            else
            {
                eResult = COND_HOTSWAP_FAIL;                
            }
#else
            if (i4Ret == 0)
            {
                eResult = COND_INSERT_OK;
            }        
#endif            
        }
        
        return eResult;
        //break;

    case STORG_OP_REMOVE:
        ASSERT(prFuncTbl->pfnRemove != NULL);
        if (prFuncTbl->pfnRemove == NULL)
        {
            return COND_FAIL;
        }
        i4Ret = prFuncTbl->pfnRemove();
        // send notify to middleware.        
        if (prReq->prDev->pfnHotswapNfy)
        {
            eResult = (i4Ret == 0) ? COND_REMOVE_OK : COND_HOTSWAP_FAIL;
            prReq->prDev->pfnHotswapNfy(prReq->prDev->pvHotswapTag, eResult, NULL);
        }
        if (i4Ret == 0)
        {
            return COND_REMOVE_OK;
        }
        break;

    case STORG_OP_NONE:
        return COND_ERASE_OK;

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
        ASSERT(prFuncTbl->pfnErase != NULL);
        if (prFuncTbl->pfnErase == NULL)
        {
            return COND_FAIL;
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

    default:
        LOG(1, "STORG_OP is invalid %d\n", prReq->eOp);
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return COND_FAIL;
    }

    return COND_FAIL;
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
    case STORG_OP_READ:
    case STORG_OP_WRITE:
    case STORG_OP_ERASE:
    case STORG_OP_FLUSH:
    case STORG_OP_STDBYTIME:

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
            VERIFY(OSR_OK == x_sema_unlock(prReq->prDev->prStorgThrd->hSemaDone));
        }
        else
        {        
            prReq->pfnNfy(prReq->pvTag, eResult, prReq->pvData);
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
    //INT32 fgSet;
    INT32 i4Ret = 0;
#endif

    ASSERT(pvData != NULL);
    prSTD = *(STORG_THRD_T **)pvData;
    ASSERT(prSTD != NULL);

    // prepare request message buffer.
    //prReq = x_mem_alloc(sizeof(STORG_REQ_T));
    //ASSERT(prReq);

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

#if 0
#ifdef CC_FCI_ENABLE
        i4Ret = 0;
        // handle FCI pinmux.
        if (prReq->prDev->u4DevFlag &(STORG_FLAG_MS | STORG_FLAG_SD | STORG_FLAG_SMXD))
        {
            if (((PINMUX0_REG() & 0x200) != 0x200) ||((PINMUX1_REG() & 0x16000) != 0x16000))
            {
                if (prReq->eOp != STORG_OP_REMOVE)
                {
                    // set FCI pin mux.
                    //PINMUX0_WRITE(PINMUX0_REG() | 0x200);
                    //PINMUX1_WRITE(PINMUX1_REG() | 0x16000);

                    // turn on  FCI buffer enable pin.
                    //fgSet = 0;
                    //VERIFY(0 == GPIO_Output(FCI_NOISE_BUFFER_PIN, (const INT32 *)&fgSet));

                    // wait pin to stable.
                    x_thread_delay(500);

                    // reinitialize card.
                    //ASSERT(prReq->prDev->prFuncTbl->pfnInsert != NULL);
                    //i4Ret = prReq->prDev->prFuncTbl->pfnInsert();
                }
            }
        }
        else if (prReq->prDev->u4DevFlag &(STORG_FLAG_NOR))
        {
            if (((PINMUX0_REG() & 0x200) == 0x200) ||((PINMUX1_REG() & 0x16000) == 0x16000))
            {
                // turn off  FCI buffer enable pin.
                //fgSet = 1;
                //VERIFY(1 == GPIO_Output(FCI_NOISE_BUFFER_PIN, (const INT32 *)&fgSet));

                // set NOR pin mux.
                //PINMUX0_WRITE(PINMUX0_REG() & ~0x200);
                //PINMUX1_WRITE(PINMUX1_REG() & ~0x16000);

                // wait pin to stable.
                x_thread_delay(20);
            }
        }
#endif // CC_FCI_ENABLE 
#endif

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

    x_memset(prSTD, 0, sizeof(STORG_THRD_T));

    // 1. setup device flags. 
    prSTD->u4DevFlag = u4DevFlag;

    // 2. create semaphore for thread exit. 
    i4Ret = x_sema_create(&prSTD->hSemaExit, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
    ASSERT(i4Ret == OSR_OK);
    UNUSED(i4Ret);

    // 3. create semaphore for reentry of STORGSendReq(). 
    i4Ret = x_sema_create(&prSTD->hSemaLock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
    ASSERT(i4Ret == OSR_OK);
    UNUSED(i4Ret);

    // 4. create semaphore for sync operation. 
    i4Ret = x_sema_create(&prSTD->hSemaDone, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK);
    ASSERT(i4Ret == OSR_OK);
    UNUSED(i4Ret);

    // 5. create request message queue. 
    SPrintf(szBuf, "SREQ%X", u4DevFlag);
    i4Ret = x_msg_q_create(&prSTD->hRequestMsg, szBuf, sizeof(STORG_REQ_T),
    STORG_MSG_NUM);
    ASSERT(i4Ret == OSR_OK);
    UNUSED(i4Ret);

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
    case STORG_FCI_FLAG:
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

    case STORG_FLAG_SMXD:
        prFuncTbl = &_rSMXDFuncTbl;
        prStorgThrd = &_rSTDFlash;
        prDev = &_rStorgDevSMXD;
        break;
#endif

#ifdef CC_NAND_ENABLE

    case STORG_FLAG_NAND:
        prFuncTbl = &_rNANDFuncTbl;
        prStorgThrd = &_rSTDFlash;
        prDev = &_rStorgDevNAND;
        break;
#endif

#ifdef CC_NOR_ENABLE

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
        LOG(1, "%s(%d) Open 0x%08x device failed\n", __FILE__, __LINE__, u4DevFlag);
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return;
    }

    if (prDev->fgInit == 0)
    {
        prDev->eDevType = ((u4DevFlag&STORG_CHAR_FLAG) 
            ? STORG_TYPE_CHR : STORG_TYPE_BLK);

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
    UINT32 u4Val;    
#endif

#if defined(CC_FCI_ENABLE) || defined(CC_EEPROM_ENABLE)
    INT32 i4EdidWP;
    INT32 i4EdidWPEnable;
    INT32 i4SysWP;
    INT32 i4SysWPEnable;    
    INT32 fgSet;
#endif

#ifdef CC_EEPROM_ENABLE
    DTVCFG_T rDtvCfg;
#endif /* CC_EEPROM_ENABLE */

#ifdef CC_IDE_ENABLE
    INT32 i4Gpio = 9;
#endif // CC_IDE_ENABLE 

    if (_fgStorgInit)
    {
        return 0;
    }

    _fgStorgInit = TRUE;

#ifdef CC_NAND_ENABLE

    NAND_Init();
#endif // CC_NAND_ENABLE 

#ifdef CC_IDE_ENABLE

    i4Ret = IDE_Init();
    UNUSED(i4Ret);
    // Setup CF hotswap interface function. 
    // VERIFY(0 <= BSP_PinGpioSel(i4Gpio));
    // VERIFY(0 <= GPIO_Reg(i4Gpio, GPIO_TYPE_NONE, NULL));
    // VERIFY(0 <= GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_FALL, _Storge_CFCallback));
    // VERIFY(0 <= GPIO_Reg(i4Gpio, GPIO_TYPE_INTR_RISE, _Storge_CFCallback));
#endif // CC_IDE_ENABLE 

    // create IO threads. 
    _fgStorgRun = TRUE;

#if defined(CC_NAND_ENABLE) || defined(CC_NOR_ENABLE) || defined(CC_EEPROM_ENABLE)
		// set NOR pin mux.
    //PINMUX0_WRITE(PINMUX0_REG() & ~0x200);
    //PINMUX1_WRITE(PINMUX1_REG() & ~0x16000);

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
#endif // CC_IDE_ENABLE 

#ifdef CC_IDE_ENABLE

    _Storge_InitDev(STORG_FLAG_HD);
    _Storge_InitDev(STORG_FLAG_CF);
#endif

#ifdef CC_FCI_ENABLE

    _Storge_InitDev(STORG_FLAG_MS);
    _Storge_InitDev(STORG_FLAG_SD);
    _Storge_InitDev(STORG_FLAG_SMXD);
    _Storge_InitDev(STORG_FCI_FLAG);

    // Setup MS/SD hotswap interface function. 
    FCIHW_SetCardDetectFct(_Storge_FCICallback);

    i4Ret = FCI_RegInit();
    UNUSED(i4Ret);
    
#endif

#ifdef CC_NAND_ENABLE

    _Storge_InitDev(STORG_FLAG_NAND);
#endif

#ifdef CC_NOR_ENABLE

    _Storge_InitDev(STORG_FLAG_NOR);
#endif

#ifdef CC_EEPROM_ENABLE

    _Storge_InitDev(STORG_FLAG_EEPROM);

    if (0 == DRVCUST_OptQuery(eEdidEepromWPGpio, &u4Val))
    {
        i4EdidWP = (INT32)u4Val;
        i4EdidWPEnable = (INT32)DRVCUST_OptGet(eEdidEepromWPEnablePolarity);

         i4Ret = EEPDTV_Read((UINT64)EEPDTV_DTVCFG_MAGIC1, (UINT32)(void *)&(rDtvCfg), sizeof(rDtvCfg));
        if (i4Ret != 0)
        {
            rDtvCfg.u1Flags = 0;
        }

        //initial eeprom write protect by GPIO.
        fgSet = ((rDtvCfg.u1Flags & DTVCFG_FLAG_FACTORY_MODE) ? (!i4EdidWPEnable) : (i4EdidWPEnable));
        VERIFY(GPIO_Output(i4EdidWP, &fgSet) == fgSet);
    }
    
    i4SysWP = (INT32)DRVCUST_OptGet(eSysEepromWPGpio);
    i4SysWPEnable = (INT32)DRVCUST_OptGet(eSysEepromWPEnablePolarity);
    
    //initial eeprom write protect by GPIO.
    VERIFY(BSP_PinGpioSel(i4SysWP) == 0);
    fgSet = i4SysWPEnable;
    VERIFY(GPIO_Output(i4SysWP, &fgSet) == fgSet);
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

#if defined(CC_NAND_ENABLE) || defined(CC_NOR_ENABLE) || defined(CC_EEPROM_ENABLE)  || defined(CC_FCI_ENABLE)

    _Storge_Close(&_rSTDFlash);
#endif

#ifdef CC_IDE_ENABLE

    _Storge_Close(&_rSTDIde);
#endif

#ifdef CC_IDE_ENABLE

    IDE_Stop();
#endif // CC_IDE_ENABLE 

#ifdef CC_FCI_ENABLE

    VERIFY(0 == FCI_Stop());
#endif // CC_FCI_ENABLE 

#ifdef CC_NAND_ENABLE

    NAND_Stop();
#endif // CC_NAND_ENABLE 

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

#ifdef CC_NOR_ENABLE

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

#ifdef CC_NOR_ENABLE

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

    switch (eOp)
    {
    case STORG_OP_NONE:
        break;

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
        ASSERT(prFuncTbl->pfnErase != NULL);
        if (prFuncTbl->pfnErase == NULL)
        {
            ASSERT(prFuncTbl->pfnErase != NULL);

            ASSERT(0);
            LINT_SUPPRESS_NEXT_EXPRESSION(527);
            return 1;
        }
        // Remove sync deny, because nor part need sync erase. 
        // ASSERT(pfnNfy != NULL);  Always async IO. 
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
        pfnNfy = prDev->pfnHotswapNfy;
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
        pfnNfy = prDev->pfnHotswapNfy;
        pvTag = prDev->pvHotswapTag;
        break;

    default:
        LOG(1, "STORG_OP is invalid %d\n", eOp);
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return 1;
    }

    if ((prDev->u4DevFlag&STORG_BLOCK_FLAG) && 
        ((eOp == STORG_OP_READ) || (eOp == STORG_OP_WRITE)))
    {
        ASSERT((u4Count&(SECTOR_BSIZE - 1)) == 0);
    }

    prReq = x_mem_alloc(sizeof(STORG_REQ_T));
    ASSERT(prReq);

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
#ifdef CC_NOR_ENABLE
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
    case STORG_FLAG_SMXD:
        return (UINT64)SMXD_GetSize();
#endif

#ifdef CC_NAND_ENABLE
    case STORG_FLAG_NAND:
        return (UINT64)NAND_GetSize();
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
    case STORG_FLAG_SMXD:
        return (STORG_DEV_STATUS_T)SMXD_GetStatus();
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
    case STORG_FLAG_SMXD:
        return SMXD_GetModel();
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
    case STORG_FLAG_SMXD:
        return SMXD_GetCard();
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
#endif /* CC_5391_LOADER */
