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
 * $RCSfile: imgrz_cmdque.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file imgrz_cmdque.c
 *  Brief of file imgrz_cmdque.c.
 *  Details of file imgrz_cmdque.c (optional).
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "imgrz_cmdque.h"
#include "imgrz_dif.h"
#include "imgrz_hw.h"
#include "imgrz_debug.h"


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// que capacity
#define E_IMGRZ_CQ_QCAPACITY      (_rRzCmdQue.i4QueCapacity)

/// que align mask
//  command que should align on dram word (128 bits = 16 bytes)
#define E_IMGRZ_CQ_ALIGN          16

// min of que capacity is 2
#define IMGRZ_CMQ_MIN_SIZE        2

#define IMGRZ_CMD_MARGIN          2

#define IMGRZ_ONE_CMD_SIZE        8   // one cmd = 8 bytes


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static INT32 _i4RzCmdqConfig = (INT32)E_IMGRZ_CMDQ_CFG_4KB;

static INT32 _i4RzCmdqNewConfig = (INT32)E_IMGRZ_CMDQ_CFG_4KB;

static INT32 _i4RzEngCrashCount = 0;

static volatile UINT64 *_pu8RzCmdqueBuf = NULL;

static volatile IMGRZ_CMDQUE_T _rRzCmdQue;


#if defined(IMGRZ_DEBUG_MODE)
static IMGRZ_DBG_ENUM2TEXT_T _rRzDbgQueCapEnum2TextTbl[] = 
{
    {(INT32)E_IMGRZ_CMDQ_CAP_2KB,     "CMDQ_CFG_2KB"},
    {(INT32)E_IMGRZ_CMDQ_CAP_4KB,     "CMDQ_CFG_4KB"},
    {(INT32)E_IMGRZ_CMDQ_CAP_8KB,     "CMDQ_CFG_8KB"},
    {(INT32)E_IMGRZ_CMDQ_CAP_16KB,    "CMDQ_CFG_16KB"},
    {(INT32)E_IMGRZ_CMDQ_CAP_32KB,    "CMDQ_CFG_32KB"},
    {(INT32)E_IMGRZ_CMDQ_CAP_64KB,    "CMDQ_CFG_64KB"},
    {(INT32)E_IMGRZ_CMDQ_CAP_128KB,   "CMDQ_CFG_128KB"},
    {(INT32)E_IMGRZ_CMDQ_CAP_256KB,   "CMDQ_CFG_256KB"},

    {(INT32)E_IMGRZ_CMDQ_CAP_MAX,     NULL}
};
#endif // #if defined(IMGRZ_DEBUG_MODE)


#if defined(IMGRZ_ENABLE_SW_MODE)
static volatile E_RZ_DIF_UNION_T *_prRzRegBase = NULL;
#endif  //#if defined(IMGRZ_ENABLE_SW_MODE)


#if defined(IMGRZ_RISC_MODE)  // risc mode   
    static INT32 _i4RzCmdqueBufExist = (INT32)TRUE;
#else                       // cmdque mode
    static INT32 _i4RzCmdqueBufExist = (INT32)FALSE;
#endif



//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** _ImgrzRecoverEngine
 *  Recover engine when engine crashed
 */
//-------------------------------------------------------------------------
static void _ImgrzRecoverEngine(HANDLE_T pt_tm_handle, VOID *pv_tag)
{
    UNUSED(pt_tm_handle);
    UNUSED(pv_tag);

    // stop crash detection timer ?
    if ((_IMGRZ_GetCrashDetection() == IMGRZ_ENABLE)  &&
        (_IMGRZ_GetScaleOpt() != E_RZ_INOUT_TYPE_JPGMD))
    {
        VERIFY(x_timer_stop(_IMGRZ_GetCrashDetectTimer()) == OSR_OK);
    }

    LOG(0, "\n--- Error --- Error --- Error --- Error ---\n");
    LOG(0, "ImageResizer hw is crashed %d times, will be recovered later !!",
        ++_i4RzEngCrashCount);
    LOG(0, "\n--- Error --- Error --- Error --- Error ---\n");

    IMGRZ_DBG_INC_ENG_CRASH_COUNT();
    IMGRZ_DBG_REC_CUR_STATUS((INT32)E_IMGRZ_CRASHED);

#if !defined(IMGRZ_RISC_MODE) // cmdque mode
    IMGRZ_HwISR();
#endif

    // re-init
    _IMGRZ_ReInit();

    // unlock cmdque resource
    IMGRZ_UnlockCmdque();
}


#if defined(IMGRZ_ENABLE_SW_MODE)
//-------------------------------------------------------------------------
/** _ImgrzSwRealIsr
 *  imgrz software interrupt routine
 */
//-------------------------------------------------------------------------
/*
static void _ImgrzSwRealIsr(void)
{
    IMGRZ_DBG_INC_CM_INT_COUNT();  // for debug use
    IMGRZ_CMDQUE_INC_INT_COUNT();  // for debug use

    // unlock cmdque resource
    IMGRZ_UnlockCmdque();
}
*/

//-------------------------------------------------------------------------
/** _ImgrzSwQueAction
 *  start sw engine
 */
//-------------------------------------------------------------------------
/*
static void _ImgrzSwQueAction(void)
{
    INT32 i4Element, i4CurIdx;
    UINT64 *pu8QueTop;
    UINT32 *pu4RegBase;
    UINT32 u4RegAddr, u4RegValue;

    pu4RegBase = (UINT32 *)(_prRzRegBase);
#if 0//!defined (CC_MT5387) && !defined (CC_MT5363)
    i4Element  = (INT32)(_prRzRegBase->rField.fg_DRAMQ_LEN >> 3);
    ASSERT(i4Element == _rRzCmdQue.i4QueSize);
#endif
    pu8QueTop = (UINT64 *)_rRzCmdQue.pu8QueTop;
    i4CurIdx  = _rRzCmdQue.i4ReadIndex;

    while (i4Element--)
    {
        u4RegAddr  = (UINT32)((pu8QueTop[i4CurIdx] >> 34) & 0x3ff);
        u4RegValue = (UINT32)(pu8QueTop[i4CurIdx] & 0xffffffff);
        
        pu4RegBase[u4RegAddr] = u4RegValue;
        
        // only direct mode by now
        if ((u4RegAddr == (UINT32)RZ_REG_RZSTART) && 
            ((u4RegValue & RZREG_MSK_START) == RZREG_MSK_START))
        {
            pfnIMGRZ_DifAction();
        }

        i4CurIdx++;     // cmdque current index

        // If current index >= bottom of cmdque then
        // set current index to top of cmdque.
        if (i4CurIdx >= E_IMGRZ_CQ_QCAPACITY)
        {
            i4CurIdx = 0;
        }
    }

    _ImgrzSwRealIsr();
}
*/
#endif // #if defined(IMGRZ_ENABLE_SW_MODE)



//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** IMGRZ_CmdQueInit
 *  init command queue
 *    allocate cmdq buffer
 *    get hw register address (sw mod)
 *    set command queue info
 *    reset command queue hw
 */
//-------------------------------------------------------------------------
void IMGRZ_CmdQueInit(void)
{
    UINT32 u4Size, u4Align;

    if (_i4RzCmdqConfig == _i4RzCmdqNewConfig)
    {
        // allocate cmdque buffer by means of default config
        if (_i4RzCmdqueBufExist == (INT32)FALSE)
        {
            // calculate cmdque capacity
            IMGRZ_CmdQueCalculateCapacity(_i4RzCmdqConfig);

            u4Size  = (UINT32)(E_IMGRZ_CQ_QCAPACITY * IMGRZ_ONE_CMD_SIZE);
            u4Align = (UINT32)(E_IMGRZ_CQ_ALIGN);

            // allocate a cmdque buffer for engine use
            _pu8RzCmdqueBuf = 
                (volatile UINT64 *)BSP_AllocAlignedDmaMemory(u4Size, u4Align);
            VERIFY(_pu8RzCmdqueBuf != NULL);

            _i4RzCmdqueBufExist = (INT32)TRUE;
        }
    }
    else    // re-allocate cmdque buffer
    {
        // calculate cmdque capacity by means of new config
        IMGRZ_CmdQueCalculateCapacity(_i4RzCmdqNewConfig);

        u4Size  = (UINT32)(E_IMGRZ_CQ_QCAPACITY * IMGRZ_ONE_CMD_SIZE);
        u4Align = (UINT32)(E_IMGRZ_CQ_ALIGN);

        // free cmdque buffer
        VERIFY(BSP_FreeAlignedDmaMemory((UINT32)_pu8RzCmdqueBuf) == TRUE);

        // allocate new cmdque buffer for engine use
        _pu8RzCmdqueBuf = 
            (volatile UINT64 *)BSP_AllocAlignedDmaMemory(u4Size, u4Align);
        VERIFY(_pu8RzCmdqueBuf != NULL);

        _i4RzCmdqConfig = _i4RzCmdqNewConfig;
    }


#if defined(IMGRZ_ENABLE_SW_MODE)
//-----------------------------------------------------------
    // sw mode
    if ((UINT32)E_RZ_SW_MOD == IMGRZ_DifGetData()->u4ImgrzMode)
    {
        UINT32 u4ImgrzRegBase;
        
        IMGRZ_DifGetRegBase(&u4ImgrzRegBase);

        _prRzRegBase = (volatile E_RZ_DIF_UNION_T *)u4ImgrzRegBase;
	_prRzRegBase=_prRzRegBase;
    }
//-----------------------------------------------------------
#endif  //#if defined(IMGRZ_ENABLE_SW_MODE)


    // setup cmdque info
    IMGRZ_CmdQueSetup();

    IMGRZ_CmdQueReset();
}


//-------------------------------------------------------------------------
/** IMGRZ_CmdQueReset
 *  reset cmdque hw
 */
//-------------------------------------------------------------------------
void IMGRZ_CmdQueReset(void)
{
    // hw mode setting
    if ((UINT32)E_RZ_HW_MOD == IMGRZ_DifGetData()->u4ImgrzMode)
    {
        volatile UINT32 u4Value;
        volatile UINT32 u4RegOffset;

    #if defined(IMGRZ_RISC_MODE)
    #if 0//!defined (CC_MT5387) && !defined (CC_MT5363)    
        u4RegOffset = ((RZ_REG_DRAMQ_CFG << 2) & IMGRZ_REG_MASK);
        u4Value = IMGRZ_READ32(u4RegOffset);
        // (1) disable EN_DRAMQ flag
        // _prRzRegBase->rField.fg_EN_DRAMQ = IMGRZ_DISABLE;
        u4Value = (u4Value) & (~RZREG_MSK_EN_DRAMQ);
        IMGRZ_WRITE32(u4RegOffset, u4Value);
    #endif        
    #else
        
        u4RegOffset = ((RZ_REG_DRAMQ_CFG << 2) & IMGRZ_REG_MASK);
        u4Value = IMGRZ_READ32(u4RegOffset);
        // (1) enable EN_DRAMQ flag
        // _prRzRegBase->rField.fg_EN_DRAMQ = IMGRZ_ENABLE;
        u4Value = (u4Value) | (RZREG_MSK_EN_DRAMQ);
        
        // (2) set DRAMQ_MODE to cyclic buffer mode
        // _prRzRegBase->rField.fg_DRAMQ_MODE = IMGRZ_CMD_BUF_CYLIC;
        u4Value = (u4Value) & (~RZREG_MSK_DRAMQ_MODE);

        // (3) set DRAMQ_SIZE
        // _prRzRegBase->rField.fg_DRAMQ_SIZE   = (UINT32)_rRzCmdQue.i4QueConfig;
        u4Value = (u4Value) | 
                  ((UINT32)_rRzCmdQue.i4QueConfig << RZREG_SHT_DRAMQ_SIZE);
        IMGRZ_WRITE32(u4RegOffset, u4Value);

        // (4) set DRAMQ_BSAD
        // _prRzRegBase->rField.fg_DRAMQ_BSAD = (UINT32)_rRzCmdQue.pu8QueTop;
        // Hw must use PHYSICAL address
        u4RegOffset = ((RZ_REG_DRAMQ_STAD << 2) & IMGRZ_REG_MASK);
        u4Value = PHYSICAL(((UINT32)_rRzCmdQue.pu8QueTop));
        IMGRZ_WRITE32(u4RegOffset, u4Value);

    #endif // #if defined(IMGRZ_RISC_MODE)
    }


#if defined(IMGRZ_ENABLE_SW_MODE)
//-----------------------------------------------------------
    // sw mode setting
    if ((UINT32)E_RZ_SW_MOD == IMGRZ_DifGetData()->u4ImgrzMode)
    {
    #if defined(IMGRZ_RISC_MODE)
    #if 0//!defined (CC_MT5387) && !defined (CC_MT5363)    
        _prRzRegBase->rField.fg_EN_DRAMQ = IMGRZ_DISABLE;
    #endif    
    #else
    
        _prRzRegBase->rField.fg_EN_DRAMQ = IMGRZ_ENABLE;
        _prRzRegBase->rField.fg_DRAMQ_MODE = IMGRZ_CMD_BUF_CYLIC;
        _prRzRegBase->rField.fg_DRAMQ_SIZE = (UINT32)_rRzCmdQue.i4QueConfig;

        // Sw must use VIRTUAL address
        _prRzRegBase->rField.fg_DRAMQ_BSAD = (UINT32)_rRzCmdQue.pu8QueTop;

    #endif // #if defined(IMGRZ_RISC_MODE)
    }
//-----------------------------------------------------------
#endif  //#if defined(IMGRZ_ENABLE_SW_MODE)
}


//-------------------------------------------------------------------------
/** IMGRZ_CmdQueAction
 *  start Imgrz hardware command queue
 */
//-------------------------------------------------------------------------
void IMGRZ_CmdQueAction(void)
{
#if 0//!defined (CC_MT5387) && !defined (CC_MT5363)
    volatile UINT32 u4CmdQueLen;
    volatile UINT32 u4RegOffset;

    if (_rRzCmdQue.i4QueSize < IMGRZ_CMQ_MIN_SIZE)
    {
        INT32 i4Count;
        INT32 i4Times;
        
        i4Times = ((INT32)IMGRZ_CMQ_MIN_SIZE - _rRzCmdQue.i4QueSize);

        for (i4Count = 0; i4Count < i4Times; i4Count++)
        {
            IMGRZ_CmdQuePushBack((UINT32)RZ_REG_DUMMY, 0);
        }
    }

    // # of command must be even
    if (_rRzCmdQue.i4QueSize & 1)
    {
        IMGRZ_CmdQuePushBack((UINT32)(RZ_REG_DUMMY), 0);
    }

    // hw mode
    if ((UINT32)E_RZ_HW_MOD == IMGRZ_DifGetData()->u4ImgrzMode)
    {
        IMGRZ_DBG_INC_DRV_FLUSH_COUNT();  // for debug use
        IMGRZ_CMDQUE_INC_FLUSH_COUNT();   // for debug use

        // lock cmdque resource
        IMGRZ_LockCmdque();

        HalFlushInvalidateDCache();

        IMGRZ_DBG_REC_FLUSH_TIME();      // for debug use

        // start timer to do hw crash detection ?
        if ((_IMGRZ_GetCrashDetection() == IMGRZ_ENABLE)  &&
            (_IMGRZ_GetScaleOpt() != E_RZ_INOUT_TYPE_JPGMD))
        {
            VERIFY(x_timer_start(_IMGRZ_GetCrashDetectTimer(), 
                IMGRZ_CRASH_DETECT_PERIOD, X_TIMER_FLAG_ONCE, 
                _ImgrzRecoverEngine, NULL) == OSR_OK);
        }

        // in hw mode, engine will run if DRAMQ_LEN is written.
        // 1 CMD size = 8 bytes
        u4RegOffset = ((RZ_REG_DRAMQ_LEN << 2) & IMGRZ_REG_MASK);
        u4CmdQueLen = (UINT32)(_rRzCmdQue.i4QueSize * IMGRZ_ONE_CMD_SIZE);
        IMGRZ_WRITE32(u4RegOffset, u4CmdQueLen);

        _rRzCmdQue.i4PrevIndex = _rRzCmdQue.i4ReadIndex;
        _rRzCmdQue.i4ReadIndex = _rRzCmdQue.i4WriteIndex;
        _rRzCmdQue.i4QueSize   = 0;
    }


#if defined(IMGRZ_ENABLE_SW_MODE)
//-----------------------------------------------------------
    // sw mode
    if ((UINT32)E_RZ_SW_MOD == IMGRZ_DifGetData()->u4ImgrzMode)
    {
        IMGRZ_DBG_INC_CM_FLUSH_COUNT();  // for debug use
        IMGRZ_CMDQUE_INC_FLUSH_COUNT();  // for debug use

        // lock cmdque resource
        IMGRZ_LockCmdque();

        HalFlushInvalidateDCache();

        // in sw mode, just record cmdque len for _ImgrzSwQueAction() use
        // 1 CMD size = 8 bytes
        u4CmdQueLen = (UINT32)(_rRzCmdQue.i4QueSize * IMGRZ_ONE_CMD_SIZE);
        _prRzRegBase->rField.fg_DRAMQ_LEN = u4CmdQueLen;

        _ImgrzSwQueAction();

        _rRzCmdQue.i4PrevIndex = _rRzCmdQue.i4ReadIndex;
        _rRzCmdQue.i4ReadIndex = _rRzCmdQue.i4WriteIndex;
        _rRzCmdQue.i4QueSize   = 0;

        IMGRZ_HwISR();    // notify MW
    }
//-----------------------------------------------------------
#endif  //#if defined(IMGRZ_ENABLE_SW_MODE)

#endif //!5387 & !5363
}


//-------------------------------------------------------------------------
/** IMGRZ_RiscPushBack (Risc Mode)
 *  push back one register value into Imgrz register
 *  @param u4Reg indicates which register
 *  @param u4Val indicates the value of the register
 */
//-------------------------------------------------------------------------
void IMGRZ_RiscPushBack(UINT32 u4Reg, UINT32 u4Val)
{
    UINT32 u4RegOffset;
    
    u4RegOffset = ((u4Reg << 2) & IMGRZ_REG_MASK);

    if (u4Reg == (UINT32)RZ_REG_RZSTART)
    {
        IMGRZ_DBG_INC_DRV_FLUSH_COUNT();  // for debug use
        IMGRZ_CMDQUE_INC_FLUSH_COUNT();   // for debug use

        // lock cmdque resource
        //IMGRZ_LockCmdque();
        //LOG(0, "IMGRZ_RiscPushBack\n");
        IMGRZ_LockWaitIsr();
        HalFlushInvalidateDCache();

        IMGRZ_DBG_REC_FLUSH_TIME();      // for debug use

        // start timer to do hw crash detection ?
        if ((_IMGRZ_GetCrashDetection() == IMGRZ_ENABLE)  &&
            (_IMGRZ_GetScaleOpt() != E_RZ_INOUT_TYPE_JPGMD))
        {
            VERIFY(x_timer_start(_IMGRZ_GetCrashDetectTimer(), 
                IMGRZ_CRASH_DETECT_PERIOD, X_TIMER_FLAG_ONCE, 
                _ImgrzRecoverEngine, NULL) == OSR_OK);
        }

        // write a cmd to the hw register (flush)
        IMGRZ_WRITE32(u4RegOffset, u4Val);
        
        // ... wait for unlock fill command
    }
    else
    {
        // lock cmdque resource
        IMGRZ_LockCmdque();

        // check engine idle (for debug use)
        IMGRZ_DBG_CHK_ENG_IDLE();
    
        // write a cmd to the hw register
        IMGRZ_WRITE32(u4RegOffset, u4Val);

        // unlock cmdque resource
        IMGRZ_UnlockCmdque();
    }
}


//-------------------------------------------------------------------------
/** IMGRZ_CmdQuePushBack (Cmdque Mode)
 *  push back one register value into cmdque
 *  @param u4Reg indicates which register
 *  @param u4Val indicates the value of the register
 */
//-------------------------------------------------------------------------
void IMGRZ_CmdQuePushBack(UINT32 u4Reg, UINT32 u4Val)
{
    UINT32 u4RegOffset;
    
    u4RegOffset = ((u4Reg << 2) & IMGRZ_REG_MASK);

    if ((_rRzCmdQue.i4QueSize + IMGRZ_CMD_MARGIN) >= _rRzCmdQue.i4QueCapacity)
    {
        IMGRZ_CmdQueAction();
    }

    // lock cmdque resource
    IMGRZ_LockCmdque();

    // check engine idle (for debug use)
    IMGRZ_DBG_CHK_ENG_IDLE();
    
    // calculate Imgrz register address
    u4Reg = ((IMGRZ_ADDR) | (u4RegOffset));

    // write one cmd to cmdque buffer
    _rRzCmdQue.i4QueSize++;
    _rRzCmdQue.pu8QueTop[_rRzCmdQue.i4WriteIndex++] = 
        ((((UINT64)u4Reg) << 32) | u4Val);

    if (_rRzCmdQue.i4WriteIndex >= E_IMGRZ_CQ_QCAPACITY)
    {
        _rRzCmdQue.i4WriteIndex = 0;
    }

    // unlock cmdque resource

    IMGRZ_UnlockCmdque();
}


//-------------------------------------------------------------------------
/** IMGRZ_CmdQueCalculateCapacity
 *  calculate cmdque capacity
 *  and set queconfig
 */
//-------------------------------------------------------------------------
void IMGRZ_CmdQueCalculateCapacity(INT32 i4CmdqConfig)
{
    INT32 i;
    INT32 i4QueCapacity = 0;

    _rRzCmdQue.i4QueConfig = i4CmdqConfig;

    // calculate cmdque capacity
    i4QueCapacity = (INT32)E_IMGRZ_CMDQ_CAP_2KB;
    for (i = 0; i < i4CmdqConfig; i++)
    {
        i4QueCapacity = (i4QueCapacity * 2);
    } // ~for
    
    _rRzCmdQue.i4QueCapacity = i4QueCapacity;
}


//-------------------------------------------------------------------------
/** IMGRZ_CmdQueSetup
 *  setup command queue info includes
 *  que occupied size, que top, read/write index
 *
 */
//-------------------------------------------------------------------------
void IMGRZ_CmdQueSetup(void)
{
    _rRzCmdQue.i4QueSize       = 0;
    _rRzCmdQue.i4PrevIndex     = 0;
    _rRzCmdQue.i4ReadIndex     = 0;
    _rRzCmdQue.i4WriteIndex    = 0;
    _rRzCmdQue.i4QueFlushCount = 0;
    _rRzCmdQue.i4QueIntCount   = 0;
    _rRzCmdQue.pu8QueTop       = (volatile UINT64 *)
                                  VIRTUAL((UINT32)_pu8RzCmdqueBuf);
    _rRzCmdQue.prNext          = (struct _IMGRZ_CMDQUE_T *)NULL;
}



// for debug use
#if defined(IMGRZ_DEBUG_MODE)
//-------------------------------------------------------------------------
/** IMGRZ_CmdQueDbgInfo
 *  dump previously executed command queue
 *  debug dump directly to console rs232 port
 *  start from previous index (in command queue structure)
 *  till write index
 */
//-------------------------------------------------------------------------
void IMGRZ_CmdQueDbgInfo(void)
{
    CHAR szQueCfgText[D_RZ_STR_SIZE];
    
    LOG(5, "\n--- Imgrz cmdque dump ---- (begin)\n");

    // get cmdq config text from cmdq capacity
    IMGRZ_DbgEnum2Text(_rRzCmdQue.i4QueCapacity, szQueCfgText, 
        _rRzDbgQueCapEnum2TextTbl, (INT32)E_IMGRZ_CMDQ_CAP_MAX);
    LOG(5, "   QueConfig   = %d (%s)\n", _rRzCmdQue.i4QueConfig, szQueCfgText);

    LOG(5, "   QueCapacity = %d\n", _rRzCmdQue.i4QueCapacity);
    LOG(5, "   QueSize     = %d\n", _rRzCmdQue.i4QueSize);
    LOG(5, "   PrevIndex   = %d\n", _rRzCmdQue.i4PrevIndex);
    LOG(5, "   ReadIndex   = %d\n", _rRzCmdQue.i4ReadIndex);
    LOG(5, "   WriteIndex  = %d\n", _rRzCmdQue.i4WriteIndex);
    LOG(5, "   QueFlush    = %d\n", _rRzCmdQue.i4QueFlushCount);
    LOG(5, "   QueIntrupt  = %d\n", _rRzCmdQue.i4QueIntCount);

    _rRzCmdQue.pu1PrevAddr = (UINT8 *)(((UINT32)_rRzCmdQue.pu8QueTop) + 
        ((UINT32)(_rRzCmdQue.i4PrevIndex * IMGRZ_ONE_CMD_SIZE)));
    _rRzCmdQue.pu1ReadAddr = (UINT8 *)(((UINT32)_rRzCmdQue.pu8QueTop) + 
        ((UINT32)(_rRzCmdQue.i4ReadIndex * IMGRZ_ONE_CMD_SIZE)));
    _rRzCmdQue.pu1WriteAddr = (UINT8 *)(((UINT32)_rRzCmdQue.pu8QueTop) + 
        ((UINT32)(_rRzCmdQue.i4WriteIndex * IMGRZ_ONE_CMD_SIZE)));
        
    LOG(5, "   PrevAddr    = 0x%08x\n", _rRzCmdQue.pu1PrevAddr);
    LOG(5, "   ReadAddr    = 0x%08x\n", _rRzCmdQue.pu1ReadAddr);
    LOG(5, "   WriteAddr   = 0x%08x\n", _rRzCmdQue.pu1WriteAddr);

    if (_rRzCmdQue.pu8QueTop == NULL)
    {
        LOG(5, "   QueTop      = 0x%08x (risc mode)\n", _rRzCmdQue.pu8QueTop);
    }
    else
    {
        LOG(5, "   QueTop      = 0x%08x (cmdque mode)\n", _rRzCmdQue.pu8QueTop);
    }

    LOG(5, "--- Imgrz cmdque dump ---- (end)\n\n");
}


//-------------------------------------------------------------------------
/** IMGRZ_CmdQueSetNewConfig
 *  set cmdque new config
 *
 */
//-------------------------------------------------------------------------
void IMGRZ_CmdQueSetNewConfig(INT32 i4NewConfig)
{
    _i4RzCmdqNewConfig = i4NewConfig;
    IMGRZ_Init();
}


//-------------------------------------------------------------------------
/** IMGRZ_CmdQueFlushCount
 *  increase the cmdque flush count
 *
 */
//-------------------------------------------------------------------------
void IMGRZ_CmdQueFlushCount(void)
{
    _rRzCmdQue.i4QueFlushCount++;
}


//-------------------------------------------------------------------------
/** IMGRZ_CmdQueIntCount
 *  increase the cmdque interrrupt count
 *
 */
//-------------------------------------------------------------------------
void IMGRZ_CmdQueIntCount(void)
{
    _rRzCmdQue.i4QueIntCount++;
}
#endif //#if defined(IMGRZ_DEBUG_MODE)


