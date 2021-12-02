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
 * $RCSfile: gfx_cmdque.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gfx_cmdque.c
 *  Brief of file gfx_cmdque.c.
 *  Details of file gfx_cmdque.c (optional).
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "gfx_if.h"
#include "gfx_drvif.h"
#include "gfx_cmdque.h"
#include "gfx_dif.h"
#include "gfx_hw.h"
#include "gfx_debug.h"


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// que capacity
#define E_GFX_CQ_QCAPACITY      (_prGfxCmdQue->i4QueCapacity)

/// que align mask
//  gfx command que should align on dram word (128 bits = 16 bytes)
#define E_GFX_CQ_ALIGN          16

// min of que capacity is 2 (mt5351 is 4)
#define GFX_CMQ_MIN_SIZE        2

#define GFX_CMD_MARGIN          2

#define GFX_ONE_CMD_SIZE        8   // one cmd = 8 bytes


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
extern UINT32 _GFX_GetCrashDetection(void);


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static INT32 _i4GfxCmdqConfig = (INT32)E_GFX_CMDQ_CFG_8KB;

static INT32 _i4GfxCmdqNewConfig = (INT32)E_GFX_CMDQ_CFG_8KB;

static INT32 _i4GfxEngCrashCount = 0;

static volatile UINT64 *_pu8GfxCmdqueBuf = NULL;

static volatile GFX_CMDQUE_T _rGfxCmdQue;

static volatile GFX_CMDQUE_T *_prGfxCmdQue = NULL;

static BOOL _fgExtCmdQue = FALSE;

#if defined(GFX_DEBUG_MODE)
static GFX_DBG_ENUM2TEXT_T _rGfxDbgQueCapEnum2TextTbl[] = 
{
    {(INT32)E_GFX_CMDQ_CAP_2KB,     "CMDQ_CFG_2KB"},
    {(INT32)E_GFX_CMDQ_CAP_4KB,     "CMDQ_CFG_4KB"},
    {(INT32)E_GFX_CMDQ_CAP_8KB,     "CMDQ_CFG_8KB"},
    {(INT32)E_GFX_CMDQ_CAP_16KB,    "CMDQ_CFG_16KB"},
    {(INT32)E_GFX_CMDQ_CAP_32KB,    "CMDQ_CFG_32KB"},
    {(INT32)E_GFX_CMDQ_CAP_64KB,    "CMDQ_CFG_64KB"},
    {(INT32)E_GFX_CMDQ_CAP_128KB,   "CMDQ_CFG_128KB"},
    {(INT32)E_GFX_CMDQ_CAP_256KB,   "CMDQ_CFG_256KB"},

    {(INT32)E_GFX_CMDQ_CAP_MAX,     NULL}
};
#endif // #if defined(GFX_DEBUG_MODE)


#if defined(GFX_ENABLE_SW_MODE)
static volatile MI_DIF_UNION_T *_prRegBase = NULL;
#endif  //#if defined(GFX_ENABLE_SW_MODE)


#if defined(GFX_RISC_MODE)  // risc mode   
    static INT32 _i4GfxCmdqueBufExist = (INT32)TRUE;
#else                       // cmdque mode
    static INT32 _i4GfxCmdqueBufExist = (INT32)FALSE;
#endif

#if defined(CC_MT5365) || defined(CC_MT5395)  
#ifdef __KERNEL__ 
static UINT32 _u4CmdQSize = 0;
#endif
#endif
//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** _GfxRecoverEngine
 *  Recover gfx engine when gfx engine crashed
 */
//-------------------------------------------------------------------------
static void _GfxRecoverEngine(HANDLE_T pt_tm_handle, VOID *pv_tag)
{
    UNUSED(pt_tm_handle);
    UNUSED(pv_tag);

    // stop crash detection timer ?
    if (_GFX_GetCrashDetection() == GFX_ENABLE)
    {
        VERIFY(x_timer_stop(_GFX_GetCrashDetectTimer()) == OSR_OK);
    }

    LOG(0, "\n--- Error --- Error --- Error --- Error ---\n");
    LOG(0, "GFX hw is crashed %d times, will be recovered later !!",
        ++_i4GfxEngCrashCount);
    LOG(0, "\n--- Error --- Error --- Error --- Error ---\n");

    GFX_DBG_INC_ENG_CRASH_COUNT();
    GFX_DBG_REC_CUR_STATUS((INT32)E_GFX_CRASHED);

#if !defined(GFX_RISC_MODE) // cmdque mode
    GFX_HwISR();
#endif

    // re-init
    _GFX_ReInit();

    // unlock cmdque resource
    GFX_UnlockCmdque();
}


#if defined(GFX_ENABLE_SW_MODE)
//-------------------------------------------------------------------------
/** _GfxSwRealIsr
 *  gfx software interrupt routine
 */
//-------------------------------------------------------------------------
static void _GfxSwRealIsr(void)
{
    GFX_DBG_INC_CM_INT_COUNT();  // for debug use
    GFX_CMDQUE_INC_INT_COUNT();  // for debug use

    // unlock cmdque resource
    GFX_UnlockCmdque();
}


//-------------------------------------------------------------------------
/** _GfxSwQueAction
 *  start sw engine
 */
//-------------------------------------------------------------------------
static void _GfxSwQueAction(void)
{
    INT32 i4Element, i4CurIdx;
    UINT64 *pu8QueTop;
    UINT32 *pu4RegBase;
    UINT32 u4RegAddr, u4RegValue;

    pu4RegBase = (UINT32 *)(_prRegBase);

    i4Element  = (INT32)(_prRegBase->rField.fg_DRAMQ_LEN >> 3);
    ASSERT(i4Element == _prGfxCmdQue->i4QueSize);

    pu8QueTop = (UINT64 *)_prGfxCmdQue->pu8QueTop;
    i4CurIdx  = _prGfxCmdQue->i4ReadIndex;

    while (i4Element--)
    {
        u4RegAddr  = (UINT32)((pu8QueTop[i4CurIdx] >> 34) & 0x3ff);
        u4RegValue = (UINT32)(pu8QueTop[i4CurIdx] & 0xffffffff);
        
        pu4RegBase[u4RegAddr] = u4RegValue;
        
        // only direct mode by now
        if ((u4RegAddr == (UINT32)GREG_G_MODE) && 
            ((u4RegValue & 0x00000800) != 0))
        {
            pfnGFX_DifAction();
        }

        i4CurIdx++;     // cmdque current index

        // If current index >= bottom of cmdque then
        // set current index to top of cmdque.
        if (i4CurIdx >= E_GFX_CQ_QCAPACITY)
        {
            i4CurIdx = 0;
        }
    }

    _GfxSwRealIsr();
}
#endif // #if defined(GFX_ENABLE_SW_MODE)



//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


//-------------------------------------------------------------------------
/** GFX_CmdQueInit
 *  init gfx command queue
 *    allocate cmdq buffer
 *    get hw register address (sw mod)
 *    set command queue info
 *    reset command queue hw
 */
//-------------------------------------------------------------------------
void GFX_CmdQueInit(void)
{
    UINT32 u4Size = 0;
    UINT32 u4Align;

    _prGfxCmdQue = &_rGfxCmdQue;

    if (_i4GfxCmdqConfig == _i4GfxCmdqNewConfig)
    {
        // allocate cmdque buffer by means of default config
        if (_i4GfxCmdqueBufExist == (INT32)FALSE)
        {            
        
            // calculate cmdque capacity
            GFX_CmdQueCalculateCapacity(_i4GfxCmdqConfig);

            u4Size  = (UINT32)(E_GFX_CQ_QCAPACITY * GFX_ONE_CMD_SIZE);
            u4Align = (UINT32)(E_GFX_CQ_ALIGN);

            // allocate a cmdque buffer for engine use
            _pu8GfxCmdqueBuf = 
                (volatile UINT64 *)BSP_AllocAlignedDmaMemory(u4Size, u4Align);
            VERIFY(_pu8GfxCmdqueBuf != NULL);

            _i4GfxCmdqueBufExist = (INT32)TRUE;
        }
    }
    else    // re-allocate cmdque buffer
    {
        // calculate cmdque capacity by means of new config
        GFX_CmdQueCalculateCapacity(_i4GfxCmdqNewConfig);

        u4Size  = (UINT32)(E_GFX_CQ_QCAPACITY * GFX_ONE_CMD_SIZE);
        u4Align = (UINT32)(E_GFX_CQ_ALIGN);

        // free cmdque buffer
        VERIFY(BSP_FreeAlignedDmaMemory((UINT32)_pu8GfxCmdqueBuf) == TRUE);

        // allocate new cmdque buffer for engine use
        _pu8GfxCmdqueBuf = 
            (volatile UINT64 *)BSP_AllocAlignedDmaMemory(u4Size, u4Align);
        VERIFY(_pu8GfxCmdqueBuf != NULL);

        _i4GfxCmdqConfig = _i4GfxCmdqNewConfig;
    }


#if defined(GFX_ENABLE_SW_MODE)
//-----------------------------------------------------------
    // sw mode
    if ((UINT32)E_GFX_SW_MOD == GFX_DifGetData()->u4GfxMode)
    {
        UINT32 u4GfxRegBase;
        
        GFX_DifGetRegBase(&u4GfxRegBase);

        _prRegBase = (volatile MI_DIF_UNION_T *)u4GfxRegBase;
    }
//-----------------------------------------------------------
#endif  //#if defined(GFX_ENABLE_SW_MODE)


    // setup cmdque info
    GFX_CmdQueSetup();

    GFX_CmdQueReset();
#if defined(CC_MT5365) || defined(CC_MT5395) 
 #ifdef __KERNEL__        
       if (u4Size != 0)
	{
		_u4CmdQSize = u4Size;
       }
	 else
	 {
	 	_u4CmdQSize = (UINT32)(E_GFX_CQ_QCAPACITY * GFX_ONE_CMD_SIZE);
	 }
 #endif
#endif
}


//-------------------------------------------------------------------------
/** GFX_CmdQueReset
 *  reset cmdque hw
 */
//-------------------------------------------------------------------------
void GFX_CmdQueReset(void)
{
    // hw mode setting
    if ((UINT32)E_GFX_HW_MOD == GFX_DifGetData()->u4GfxMode)
    {
        volatile UINT32 u4Value;

    #if defined(GFX_RISC_MODE)
    
        u4Value = GFX_READ32(GFX_REG_G_CONFIG);
        // (1) disable EN_DRAMQ flag
        // _prRegBase->rField.fg_EN_DRAMQ = GFX_DISABLE;
        u4Value = (u4Value) & (~GREG_MSK_EN_DRAMQ);
        GFX_WRITE32(GFX_REG_G_CONFIG, u4Value);
        
    #else
        u4Value = GFX_READ32(GFX_REG_G_CONFIG);
        // (1) enable EN_DRAMQ flag
        // _prRegBase->rField.fg_EN_DRAMQ = GFX_ENABLE;
        u4Value = (u4Value) | (GREG_MSK_EN_DRAMQ);
        
        // (2) set DRAMQ_MODE to cyclic buffer mode
        // _prRegBase->rField.fg_DRAMQ_MODE = GFX_CMD_BUF_CYLIC;
        u4Value = (u4Value) & (~GREG_MSK_DRAMQ_MODE);

        // (3) enable/disable SHT_CMDQ flag
        // _prRegBase->rField.fg_SHT_CMDQ = _rGfxCmdQue.i4ShortCmdque;
        u4Value = (u4Value) | 
                  ((UINT32)_prGfxCmdQue->i4ShortQue << GREG_SHT_SHT_CMDQ);
        GFX_WRITE32(GFX_REG_G_CONFIG, u4Value);

        // (4) set CYC_SIZE and DRAMQ_BSAD
        // _prRegBase->rField.fg_CYC_SIZE   = (UINT32)_rGfxCmdQue.i4QueConfig;
        // _prRegBase->rField.fg_DRAMQ_BSAD = (UINT32)_rGfxCmdQue.pu8QueTop;
        // Hw must use PHYSICAL address
        u4Value = ((UINT32)_prGfxCmdQue->i4QueConfig << GREG_SHT_CYC_SIZE) | 
                  PHYSICAL(((UINT32)_prGfxCmdQue->pu8QueTop));
        GFX_WRITE32(GFX_REG_DRAMQ_STAD, u4Value);

        // power saving mode (need to measure HW)
        //_prRegBase->rField.fg_ENG_LP = GFX_ENABLE;
    #endif // #if defined(GFX_RISC_MODE)
    }


#if defined(GFX_ENABLE_SW_MODE)
//-----------------------------------------------------------
    // sw mode setting
    if ((UINT32)E_GFX_SW_MOD == GFX_DifGetData()->u4GfxMode)
    {
    #if defined(GFX_RISC_MODE)
    
        _prRegBase->rField.fg_EN_DRAMQ = GFX_DISABLE;
        
    #else
    
        _prRegBase->rField.fg_EN_DRAMQ = GFX_ENABLE;
        _prRegBase->rField.fg_DRAMQ_MODE = GFX_CMD_BUF_CYLIC;
        _prRegBase->rField.fg_SHT_CMDQ = (UINT32)_prGfxCmdQue->i4ShortQue;
        
        _prRegBase->rField.fg_CYC_SIZE   = (UINT32)_prGfxCmdQue->i4QueConfig;        
        // Sw must use VIRTUAL address
        _prRegBase->rField.fg_DRAMQ_BSAD = (UINT32)_prGfxCmdQue->pu8QueTop;

        // power saving mode (need to measure HW)
        //_prRegBase->rField.fg_ENG_LP = GFX_ENABLE;

    #endif // #if defined(GFX_RISC_MODE)
    }
//-----------------------------------------------------------
#endif  //#if defined(GFX_ENABLE_SW_MODE)
}


//-------------------------------------------------------------------------
/** GFX_CmdQueAction
 *  start gfx hardware command queue
 */
//-------------------------------------------------------------------------
void GFX_CmdQueAction(void)
{
    volatile UINT32 u4CmdQueLen;

    if (_prGfxCmdQue->i4QueSize < GFX_CMQ_MIN_SIZE)
    {
        INT32 i4Count;
        INT32 i4Times;
        
        i4Times = ((INT32)GFX_CMQ_MIN_SIZE - _prGfxCmdQue->i4QueSize);

        for (i4Count = 0; i4Count < i4Times; i4Count++)
        {
            GFX_CmdQuePushBack((INT32)GREG_DUMMY, 0);
        }
    }

    // # of command must be even
    if (_prGfxCmdQue->i4QueSize & 1)
    {
        GFX_CmdQuePushBack((INT32)(GREG_DUMMY), 0);
    }

    // hw mode
    if ((UINT32)E_GFX_HW_MOD == GFX_DifGetData()->u4GfxMode)
    {
        GFX_DBG_INC_DRV_FLUSH_COUNT();  // for debug use
        GFX_CMDQUE_INC_FLUSH_COUNT();   // for debug use

        // lock cmdque resource
        GFX_LockCmdque();
        
    #if defined(CC_MT5365) || defined(CC_MT5395) 
         #ifdef __KERNEL__ 
             HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)_pu8GfxCmdqueBuf), _u4CmdQSize);  
     #else
         HalFlushInvalidateDCache();
    #endif
        #else
               HalFlushInvalidateDCache();  
        #endif

        GFX_DBG_REC_FLUSH_TIME();      // for debug use

        // start timer to do hw crash detection ?
        if (_GFX_GetCrashDetection() == GFX_ENABLE)
        {
            VERIFY(x_timer_start(_GFX_GetCrashDetectTimer(), 
                GFX_CRASH_DETECT_PERIOD, X_TIMER_FLAG_ONCE, 
                _GfxRecoverEngine, NULL) == OSR_OK);
        }

        // in hw mode, gfx engine will run if DRAMQ_LEN is written.
        // 1 CMD size = 8 bytes
        u4CmdQueLen = (UINT32)(_prGfxCmdQue->i4QueSize * GFX_ONE_CMD_SIZE);
        GFX_WRITE32(GFX_REG_DRAMQ_LEN, u4CmdQueLen);

        _prGfxCmdQue->i4PrevIndex = _prGfxCmdQue->i4ReadIndex;
        _prGfxCmdQue->i4ReadIndex = _prGfxCmdQue->i4WriteIndex;
        _prGfxCmdQue->i4QueSize   = 0;
    }


#if defined(GFX_ENABLE_SW_MODE)
//-----------------------------------------------------------
    // sw mode
    if ((UINT32)E_GFX_SW_MOD == GFX_DifGetData()->u4GfxMode)
    {
        GFX_DBG_INC_CM_FLUSH_COUNT();  // for debug use
        GFX_CMDQUE_INC_FLUSH_COUNT();  // for debug use

        // lock cmdque resource
        GFX_LockCmdque();       
        HalFlushInvalidateDCache();

        // in sw mode, just record cmdque len for _GfxSwQueAction() use
        // 1 CMD size = 8 bytes
        u4CmdQueLen = (UINT32)(_prGfxCmdQue->i4QueSize * GFX_ONE_CMD_SIZE);
        _prRegBase->rField.fg_DRAMQ_LEN = u4CmdQueLen;

        _GfxSwQueAction();

        _prGfxCmdQue->i4PrevIndex = _prGfxCmdQue->i4ReadIndex;
        _prGfxCmdQue->i4ReadIndex = _prGfxCmdQue->i4WriteIndex;
        _prGfxCmdQue->i4QueSize   = 0;

        GFX_HwISR();    // notify MW
    }
//-----------------------------------------------------------
#endif  //#if defined(GFX_ENABLE_SW_MODE)
}

//-------------------------------------------------------------------------
/** GFX_RiscPushBack (Risc Mode)
 *  push back one register value into gfx register
 *  @param u4Reg indicates which register
 *  @param u4Val indicates the value of the register
 */
//-------------------------------------------------------------------------
void GFX_RiscPushBack(UINT32 u4Reg, UINT32 u4Val)
{
    UINT32 u4RegOffset;
    
    u4RegOffset = ((u4Reg << 2) & 0xfff);

    if (u4RegOffset == (UINT32)GFX_REG_G_MODE)
    {
        GFX_DBG_INC_DRV_FLUSH_COUNT();  // for debug use
        GFX_CMDQUE_INC_FLUSH_COUNT();   // for debug use

        // lock cmdque resource
        GFX_LockCmdque();

        HalFlushInvalidateDCache();

        GFX_DBG_REC_FLUSH_TIME();      // for debug use

        // start timer to do hw crash detection ?
        if (_GFX_GetCrashDetection() == GFX_ENABLE)
        {
            VERIFY(x_timer_start(_GFX_GetCrashDetectTimer(), 
                GFX_CRASH_DETECT_PERIOD, X_TIMER_FLAG_ONCE, 
                _GfxRecoverEngine, NULL) == OSR_OK);
        }

        // write a cmd to the hw register (flush)
        GFX_WRITE32(u4RegOffset, u4Val);
        //Printf( "(%x, %x)\n", u4RegOffset, u4Val);
        
        // ... wait for unlock fill command
    }
    else
    {
        // lock cmdque resource
        GFX_LockCmdque();

        // check engine idle (for debug use)
        GFX_DBG_CHK_ENG_IDLE();
    
        // write a cmd to the hw register
        GFX_WRITE32(u4RegOffset, u4Val);
        //Printf( "(%x, %x)\n", u4RegOffset, u4Val);

        // unlock cmdque resource
        GFX_UnlockCmdque();
    }
}


//-------------------------------------------------------------------------
/** GFX_CmdQuePushBack (Cmdque Mode)
 *  push back one register value into cmdque
 *  @param u4Reg indicates which register
 *  @param u4Val indicates the value of the register
 */
//-------------------------------------------------------------------------
void GFX_CmdQuePushBack(UINT32 u4Reg, UINT32 u4Val)
{
    UINT32 u4RegOffset;
    
    u4RegOffset = ((u4Reg << 2) & 0xfff);

    if ((_prGfxCmdQue->i4QueSize + GFX_CMD_MARGIN) >= _prGfxCmdQue->i4QueCapacity)
    {
        GFX_CmdQueAction();
    }

    // lock cmdque resource
    GFX_LockCmdque();

    // check engine idle (for debug use)
    GFX_DBG_CHK_ENG_IDLE();
    
    // calculate GFX register address
    u4Reg = (GFX_ADDR) | (u4RegOffset);

    // write one cmd to cmdque buffer
    _prGfxCmdQue->i4QueSize++;
    _prGfxCmdQue->pu8QueTop[_prGfxCmdQue->i4WriteIndex++] = 
        ((((UINT64)u4Reg) << 32) | u4Val);

    if (_prGfxCmdQue->i4WriteIndex >= E_GFX_CQ_QCAPACITY)
    {
        _prGfxCmdQue->i4WriteIndex = 0;
    }

    // unlock cmdque resource
    GFX_UnlockCmdque();
}



//-------------------------------------------------------------------------
/** GFX_CmdQueCalculateCapacity
 *  calculate cmdque capacity
 *  and set queconfig
 */
//-------------------------------------------------------------------------
void GFX_CmdQueCalculateCapacity(INT32 i4CmdqConfig)
{
    INT32 i;
    INT32 i4QueCapacity = 0;

    if (i4CmdqConfig > (INT32)E_GFX_CMDQ_CFG_16KB)
    {
        // disable short cmdque
        _prGfxCmdQue->i4ShortQue = GFX_DISABLE;
        _prGfxCmdQue->i4QueConfig = i4CmdqConfig - (INT32)E_GFX_CMDQ_CFG_32KB;
    }
    else
    {
        // enable short cmdque
        _prGfxCmdQue->i4ShortQue = GFX_ENABLE;
        _prGfxCmdQue->i4QueConfig = i4CmdqConfig;
    }

    // calculate cmdque capacity
    i4QueCapacity = (INT32)E_GFX_CMDQ_CAP_2KB;
    for (i = 0; i < i4CmdqConfig; i++)
    {
        i4QueCapacity = (i4QueCapacity * 2);
    } // ~for
    
    _prGfxCmdQue->i4QueCapacity = i4QueCapacity;
}


//-------------------------------------------------------------------------
/** GFX_CmdQueSetup
 *  setup command queue info includes
 *  que occupied size, que top, read/write index
 *
 */
//-------------------------------------------------------------------------
void GFX_CmdQueSetup(void)
{
    _prGfxCmdQue->i4QueSize       = 0;
    _prGfxCmdQue->i4PrevIndex     = 0;
    _prGfxCmdQue->i4ReadIndex     = 0;
    _prGfxCmdQue->i4WriteIndex    = 0;
    _prGfxCmdQue->i4QueFlushCount = 0;
    _prGfxCmdQue->i4QueIntCount   = 0;
    _prGfxCmdQue->pu8QueTop       = (volatile UINT64 *)
                                  VIRTUAL((UINT32)_pu8GfxCmdqueBuf);
    _prGfxCmdQue->prNext          = (struct _GFX_CMDQUE_T *)NULL;
}

void GFX_CmdQueSetExtBuf(volatile GFX_CMDQUE_T  *prGfxCmdQue,
                     volatile UINT64       *pu8CmdQueBuf, UINT32 u4Config)
{
    if (_prGfxCmdQue->i4QueSize > 0)
    {
	    GFX_CmdQueAction();
    }

    GFX_LockCmdque();
    
    _fgExtCmdQue = TRUE;

    UNUSED(_fgExtCmdQue);

    _GFX_Reset();

    _pu8GfxCmdqueBuf = pu8CmdQueBuf;

    _prGfxCmdQue = prGfxCmdQue;

    if(u4Config < E_GFX_CMDQ_CFG_MAX)
    {
        _i4GfxCmdqConfig = u4Config;
    }
    GFX_CmdQueCalculateCapacity(_i4GfxCmdqConfig);         
#if defined(CC_MT5365) || defined(CC_MT5395) 
#ifdef __KERNEL__ 
    _u4CmdQSize = (UINT32)(E_GFX_CQ_QCAPACITY * GFX_ONE_CMD_SIZE);
#endif
#endif 
        
    _i4GfxCmdqueBufExist = (INT32)TRUE;
        
    // setup cmdque info
    GFX_CmdQueSetup();

    GFX_CmdQueReset();
    
    GFX_UnlockCmdque();    
}


// for debug use
#if defined(GFX_DEBUG_MODE)
//-------------------------------------------------------------------------
/** GFX_CmdQueDbgInfo
 *  dump previously executed gfx command queue
 *  debug dump directly to console rs232 port
 *  start from previous index (in gfx command queue structure)
 *  till write index
 */
//-------------------------------------------------------------------------
void GFX_CmdQueDbgInfo(void)
{
    CHAR szQueCfgText[GFX_STR_SIZE];
    
    LOG(5, "\n--- gfx cmdque dump ---- (begin)\n");

    LOG(5, "   ShortQue    = %d (%s)\n", _prGfxCmdQue->i4ShortQue,
        (_prGfxCmdQue->i4ShortQue) ? "enabled" : "disabled");

    // get cmdq config text from cmdq capacity
    GFX_DbgEnum2Text(_prGfxCmdQue->i4QueCapacity, szQueCfgText, 
        _rGfxDbgQueCapEnum2TextTbl, (INT32)E_GFX_CMDQ_CAP_MAX);
    LOG(5, "   QueConfig   = %d (%s)\n", _prGfxCmdQue->i4QueConfig, szQueCfgText);

    LOG(5, "   QueCapacity = %d\n", _prGfxCmdQue->i4QueCapacity);
    LOG(5, "   QueSize     = %d\n", _prGfxCmdQue->i4QueSize);
    LOG(5, "   PrevIndex   = %d\n", _prGfxCmdQue->i4PrevIndex);
    LOG(5, "   ReadIndex   = %d\n", _prGfxCmdQue->i4ReadIndex);
    LOG(5, "   WriteIndex  = %d\n", _prGfxCmdQue->i4WriteIndex);
    LOG(5, "   QueFlush    = %d\n", _prGfxCmdQue->i4QueFlushCount);
    LOG(5, "   QueIntrupt  = %d\n", _prGfxCmdQue->i4QueIntCount);

    _prGfxCmdQue->pu1PrevAddr = (UINT8 *)(((UINT32)_prGfxCmdQue->pu8QueTop) + 
        ((UINT32)(_prGfxCmdQue->i4PrevIndex * GFX_ONE_CMD_SIZE)));
    _prGfxCmdQue->pu1ReadAddr = (UINT8 *)(((UINT32)_prGfxCmdQue->pu8QueTop) + 
        ((UINT32)(_prGfxCmdQue->i4ReadIndex * GFX_ONE_CMD_SIZE)));
    _prGfxCmdQue->pu1WriteAddr = (UINT8 *)(((UINT32)_prGfxCmdQue->pu8QueTop) + 
        ((UINT32)(_prGfxCmdQue->i4WriteIndex * GFX_ONE_CMD_SIZE)));
        
    LOG(5, "   PrevAddr    = 0x%08x\n", _prGfxCmdQue->pu1PrevAddr);
    LOG(5, "   ReadAddr    = 0x%08x\n", _prGfxCmdQue->pu1ReadAddr);
    LOG(5, "   WriteAddr   = 0x%08x\n", _prGfxCmdQue->pu1WriteAddr);

    if (_prGfxCmdQue->pu8QueTop == NULL)
    {
        LOG(5, "   QueTop      = 0x%08x (risc mode)\n", _prGfxCmdQue->pu8QueTop);
    }
    else
    {
        LOG(5, "   QueTop      = 0x%08x (cmdque mode)\n", _prGfxCmdQue->pu8QueTop);
    }

    LOG(5, "--- gfx cmdque dump ---- (end)\n\n");
}


//-------------------------------------------------------------------------
/** GFX_CmdQueSetNewConfig
 *  set cmdque new config
 *
 */
//-------------------------------------------------------------------------
void GFX_CmdQueSetNewConfig(INT32 i4NewConfig)
{
    _i4GfxCmdqNewConfig = i4NewConfig;
    GFX_Init();
}


//-------------------------------------------------------------------------
/** GFX_CmdQueFlushCount
 *  increase the cmdque flush count
 *
 */
//-------------------------------------------------------------------------
void GFX_CmdQueFlushCount(void)
{
    _prGfxCmdQue->i4QueFlushCount++;
}


//-------------------------------------------------------------------------
/** GFX_CmdQueIntCount
 *  increase the cmdque interrrupt count
 *
 */
//-------------------------------------------------------------------------
void GFX_CmdQueIntCount(void)
{
    _prGfxCmdQue->i4QueIntCount++;
}
#endif //#if defined(GFX_DEBUG_MODE)


