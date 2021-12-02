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
 * $RCSfile: adsp_task.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file adsp_task.c
 *  Brief of file adsp_task.c.
 *  Source file for ADSP related control routine.
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
#include "x_ckgen.h"
LINT_EXT_HEADER_BEGIN
#include "x_hal_926.h"
#include "dsp_common.h"
#include "ctrl_para.h"
#include "adsp_task.h"
#include "dsp_var_def.h"
#include "dsp_uop.h"
#include "dsp_shm.h"
#include "dsp_intf.h"
#include "dsp_func.h"
#include "dsp_rg_ctl.h"
#include "dsp_general.h"
#include "aud_debug.h"
#include "aud_hw.h"
#include "fbm_drvif.h"  // for DSP memory allocation
#include "drv_common.h"
#include "drv_adsp.h"
#include "drvcust_if.h"

#include "x_os.h"
#include "x_assert.h"
#include "hw_ckgen.h"


#define AUD_STATISTICS

#ifdef AUD_STATISTICS
#include "drv_dbase.h"
#include "x_timer.h"
#endif
LINT_EXT_HEADER_END

//lint -e950 use __align()

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//#define DSP_DRAM_BLOCK_TEST
#define AUD_STATISTICS

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define _ADSP_ENV_BLOCK       1
#define DSP_WORKING_BUF_ALIGN  0x100
#define MSG_QUEUE_TIME_OUT  1000   // In times of 10 ms => total 10 sec

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

extern void AUD_DspClkSel(UINT8 u1ADSPclkSel);
extern BOOL fgTriggerDspErrorLogs(void);
extern BOOL fgGetDspErrorLogs(ADSP_ERROR_LOG_T* ptADSPErrLog);
extern void vDspSetSamplingRate(void);
extern void vDspSetSamplingRateDec2(void);
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
extern void vDspSetSamplingRateDec3(void);
#endif
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

 #ifdef SIF_ADC
static UINT8 _au1AdspWorkingBufferUnAligned[DSP_WORK_BUF_SIZE + DSP_WORKING_BUF_ALIGN] ;
static UINT8 *  _pu1AdspWorkingBuffer = NULL;
 #endif

HANDLE_T hDspCmdQueue;

#ifdef AUD_STATISTICS

static UINT32 _u4TriggerPeriod = 2;    // Seconds for Trigger Log
static UINT32 _u4LogPeriod = 3;        // Seconds per message
static PARAM_AUD_T* _prAud = NULL;

#endif  // PSR_STATISTICS

// Light added for wakeup retry (poweron retry)
static UINT32   _u4PowerOnRetry=0;

static INT8 _i1DspMemBlock = -1;
static UINT32 _u4DspMemBufAddr = NULL;
static UINT32 _u4DspMemBufSize = 0;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define DSP_MEM_BLOCK(u4Addr)	(PHYSICAL(u4Addr)/DSP_BUF_BLOCK)

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
UINT32 vReadPowerOnRetryCount(void);
void vSendADSPTaskMessage(UINT32 u4Msg);

static UINT32 _AudAlign(UINT32 u4Addr, UINT32 u4Alignment)
{
	UINT32 u4Unaligned;

	if (u4Alignment <= 1)
	{
		return u4Addr;
    }

	u4Unaligned = u4Addr % u4Alignment;
	if (u4Unaligned != 0)
	{
		u4Addr += u4Alignment - u4Unaligned;
    }

	return u4Addr;
}

//-----------------------------------------------------------------------------
//  fgDspGetDspMem
//
/** Brief: Get DSP memory address and size
 *  @param 
 *  @retval
 */
//-----------------------------------------------------------------------------
BOOL fgDspGetDspMem(UINT32* pu4Addr, UINT32* pu4Size)
{
	if ((pu4Addr != NULL) && (pu4Size != NULL) && (_u4DspMemBufAddr != NULL))
	{
		*pu4Addr = _u4DspMemBufAddr;
		*pu4Size = _u4DspMemBufSize;
		return TRUE;
	}
	else
	{
		return FALSE;	
	}
}

//-----------------------------------------------------------------------------
//  u4DspInternalLogicalAddress
//
/** Brief: DSP inside logical address (0 ~ 16 MB) 
 *  @param 
 *  @retval
 */
//-----------------------------------------------------------------------------
UINT32 u4DspInternalLogicalAddress(UINT32 u4Addr)
{
	// Check if memory is initilized
	VERIFY(_i1DspMemBlock >= 0);
	return (u4Addr & (DSP_BUF_BLOCK-1));
}

//-----------------------------------------------------------------------------
//  u4DspPhysicalAddress
//
/** Brief: Physical address (0 ~ 64 MB)
 *  @param 
 *  @retval
 */
//-----------------------------------------------------------------------------
UINT32 u4DspPhysicalAddress(UINT32 u4Addr)
{
	// Check if memory is initialized
	VERIFY(_i1DspMemBlock >= 0);
 
	// Check 256 MB boundary
    VERIFY(PHYSICAL(u4Addr / DSP_BUF_BOUNDARY) == 0);

	// If within 16 MB
	if ((PHYSICAL(u4Addr) / DSP_BUF_BLOCK) == 0)
	{
		return (PHYSICAL(u4Addr) + (UINT32)((_i1DspMemBlock)*DSP_BUF_BLOCK));
	}

	// If within 16 ~ 256 MB
	else if (((PHYSICAL(u4Addr)/DSP_BUF_BLOCK) > 0) && ((PHYSICAL(u4Addr)/DSP_BUF_BLOCK) < 16))
	{
		return PHYSICAL(u4Addr);
	}
	else
	{
		ASSERT(0);
		LINT_SUPPRESS_NEXT_EXPRESSION(527);
		return PHYSICAL(u4Addr);
	}
}

//-----------------------------------------------------------------------------
//  u4DspVirtualAddress
//
/** Brief: Virtual address (0 ~ 64 MB | 0x30000000)
 *  @param 
 *  @retval
 */
//-----------------------------------------------------------------------------
UINT32 u4DspVirtualAddress(UINT32 u4Addr)
{
	return VIRTUAL(u4DspPhysicalAddress(u4Addr));
}

#ifdef DSP_TEST_DRAM_BLOCK
__align(0x100)  static UINT8 _au1AdspWorkingBufferUnAligned[DSP_WORK_BUF_SIZE + DSP_WORKING_BUF_ALIGN] ;
#endif

#ifndef SIF_ADC
//-----------------------------------------------------------------------------
//  u4GetDspWorkBuf
//
/** Brief:  Allocate DSP working buffer from FB pool
 *  @param 
 *  @retval
 */
//-----------------------------------------------------------------------------
UINT32 u4GetDspWorkBuf(void)
{
    FBM_POOL_T* prFbmPool;
    UINT32           u4BufAddr;
    UINT32           u4BufSize;


#ifndef DSP_TEST_DRAM_BLOCK
    // Get buffer from FB pool
    prFbmPool = VIRTUAL(FBM_GetPoolInfo((UINT8) FBM_POOL_TYPE_DSP));  

    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != NULL);
    u4BufAddr= prFbmPool->u4Addr;
    u4BufSize = prFbmPool->u4Size;
    _u4DspMemBufAddr = u4BufAddr;
    _u4DspMemBufSize = u4BufSize;
#else
    UINT32           u4TmpAddr;	
    
    u4TmpAddr = _AudAlign(_au1AdspWorkingBufferUnAligned,0x100);
    u4BufAddr = u4TmpAddr + (64*1024*1024);
    u4BufSize = DSP_WORK_BUF_SIZE;
    _u4DspMemBufAddr = u4BufAddr;
    _u4DspMemBufSize = u4BufSize;    
#endif
    
    // Check 256 Bytes alignment
    VERIFY(_AudAlign(VIRTUAL(u4BufAddr), 0x100) == VIRTUAL(u4BufAddr));

    // Check 64 MB boundary
    VERIFY(PHYSICAL((u4BufAddr + u4BufSize) / DSP_BUF_BOUNDARY) == 0);

    // Check if start and end address are in the same 16 MB boundary
    VERIFY(DSP_MEM_BLOCK(u4BufAddr) == DSP_MEM_BLOCK(u4BufAddr + u4BufSize));

    // Setup memory block to register
    _i1DspMemBlock = (INT8)DSP_MEM_BLOCK(u4BufAddr);
    if (DSP_SetAudDramBlock((UINT8)_i1DspMemBlock))
    {
        ASSERT(0);
    }

	// Clear memory

#ifndef DSP_TEST_DRAM_BLOCK
    x_memset((VOID *)VIRTUAL(u4BufAddr), 0, prFbmPool->u4Size);
#else
    x_memset((VOID *)VIRTUAL(u4BufAddr), 0, u4BufSize);
#endif
    return VIRTUAL(u4BufAddr);
}

#else

UINT32 u4GetDspWorkBuf(void)
{
#ifdef DSP_DRAM_BLOCK_TEST
    static BOOL fgInit = FALSE;

    if (!fgInit)
    {
        UINT32 u4Tmp = ReadREG(RW_AENV_CFG);         
        UINT8  u1Blk;
        UINT32 u4AddrEnd;
        
        // Memory allocate
        _pu1AdspWorkingBuffer = x_mem_alloc(50*1024*1024);
        VERIFY(_pu1AdspWorkingBuffer != NULL);

        // 256 bytes Alignment
        _pu1AdspWorkingBuffer = _AudAlign(_pu1AdspWorkingBuffer, 0x100);

        // Check 16 MB boundary
        u4AddrEnd = ((UINT32)_pu1AdspWorkingBuffer + DSP_WORK_BUF_SIZE) / DSP_BUF_BOUNDARY;
        VERIFY(u4AddrEnd == 0);
        
        // Go to assigned block
        _pu1AdspWorkingBuffer += _ADSP_ENV_BLOCK*16*1024*1024;
        
        // set ADSP ENV Config.: AUDIO_BLOCK (0x50A8, bit[2:1])
        u4Tmp &= 0xFFFFFFF9;
        u4Tmp |= (_ADSP_ENV_BLOCK<<1);
        WriteREG(RW_AENV_CFG, u4Tmp);
        
        fgInit = TRUE;
    }
    return _pu1AdspWorkingBuffer;
#else
    UINT32 u4WorkingBuf;
    UINT32 u4TmpAddr;

    UNUSED(_pu1AdspWorkingBuffer);
    // 256 Bytes alignment
    u4TmpAddr = _AudAlign((UINT32)_au1AdspWorkingBufferUnAligned, 0x100);
    _u4DspMemBufAddr = _AudAlign((UINT32)_au1AdspWorkingBufferUnAligned, 0x200)  /*0x100000 - 0x100*/;
    _u4DspMemBufSize = DSP_WORK_BUF_SIZE - (u4TmpAddr - (UINT32)_au1AdspWorkingBufferUnAligned);

    // Check 16 MB boundary
   // VERIFY(((u4WorkingBuf + DSP_WORK_BUF_SIZE) / DSP_BUF_BOUNDARY) == 0);
    
    // Check 256 MB boundary
    VERIFY(PHYSICAL((_u4DspMemBufAddr ) / DSP_BUF_BOUNDARY) == 0);

   // Check if start and end address are in the same 16 MB boundary
	VERIFY(DSP_MEM_BLOCK(_u4DspMemBufAddr) == DSP_MEM_BLOCK(_u4DspMemBufAddr + _u4DspMemBufSize));

  // Clear memory
    x_memset((VOID *)VIRTUAL(_u4DspMemBufAddr), 0, _u4DspMemBufSize);

    // Setup memory block to register
	_i1DspMemBlock = DSP_MEM_BLOCK(_u4DspMemBufAddr );
	if (DSP_SetAudDramBlock((UINT8)_i1DspMemBlock))
	{
	    ASSERT(0);
}

	
    return VIRTUAL(_u4DspMemBufAddr);
    
#endif

}

#endif  ////SIF_ADC


/******************************************************************************
* Function      : u4GetAFIFOStart
* Description   : get audio FIFO start address in physical address
* Parameter     : uDecIndex: 0: first decoder,1: second decoder
* Return        : None
******************************************************************************/
UINT32 u4GetAFIFOStart(UINT8 uDecIndex)
{
    if(uDecIndex == SECOND_DECODER)
    {
       return(_u4AFIFODec2[0]);
    }
    return (_u4AFIFO[0]) ; // default use first decoder
}
/******************************************************************************
* Function      : u4GetAFIFOEnd
* Description   : get audio FIFO end address
* Parameter     : uDecIndex: 0: first decoder,1: second decoder
* Return        : None
******************************************************************************/
UINT32 u4GetAFIFOEnd(UINT8 uDecIndex)
{
    if(uDecIndex == SECOND_DECODER)
    {
       return(_u4AFIFODec2[1]);
    }
    return (_u4AFIFO[1]);// default use first decoder
}



/******************************************************************************
* Function      : vAdspTaskMain
* Description   : main routine for ADSP Task
* Parameter     : None
* Return        : None
******************************************************************************/
//lint -e{818}
static void vADSPTaskMain(VOID* pvArg)
{
    UINT32 u4Msg;
    UINT16 u2MsgIdx;
    BOOL fgGetRet;
    //UINT32 u4RegValue;
    
    // Light added for poweron retry
    _u4PowerOnRetry = 0;

    VERIFY(pvArg == NULL);

#if defined(CC_MT8223)
    vADCInit();
#ifndef CC_MT5391_AUD_SUPPORT 
    if (DRVCUST_InitGet(eFlagDDRQfp))
    {
#ifndef CC_MT5360B
        AUD_DspClkSel(4);
#else
        AUD_DspClkSel(ADSP_CLK_dtd_2);
        LOG(1, "[DSP]ADSP_CLK_dtd_2\n");
#endif
    }
    else
    {
#ifndef CC_MT5360B
        AUD_DspClkSel(ADSP_CLK_270MHZ);
#else
        AUD_DspClkSel(ADSP_CLK_dtd_2);
        LOG(1, "[DSP]ADSP_CLK_dtd_2\n");
#endif
    }
	//APLL ref SYS
	Printf("[ADSP] APLL ref SYS");
	vIO32WriteFldAlign(CKGEN_AFECFG5, 0, FLD_RG_SYS_DIGI_SEL);
	vIO32WriteFldAlign(CKGEN_AFECFG5, 1, FLD_RG_SYS_DIGI_DIV_SEL);
	vIO32WriteFldAlign(CKGEN_AFECFG5, 1, FLD_RG_SYS_AUADC_SEL);
	vIO32WriteFldAlign(CKGEN_AFECFG4, 3, FLD_RG_SYS_APLL_SEL);
	
	//AIN clock
	Printf("[ADSP] AIN Clock");
	vIO32WriteFldAlign(CKGEN_AIN_CKCFG, 0, FLD_AIN_CK_PD);
	vIO32WriteFldAlign(CKGEN_AIN_CKCFG, 0, FLD_AIN_CK_TST);
	vIO32WriteFldAlign(CKGEN_AIN_CKCFG, 1, FLD_AIN_CK_SEL);


 #else
    AUD_DspClkSel(4); // Light: DSP run 324M Hz

    // Ain Clock source select
    CKGEN_WRITE32(0x23c, 0x00000002);  // 1: TVD, 2: SYSPLL
    CKGEN_WRITE32(0x26c, 0x00000202);
   #endif
    vDspMemInit((UINT8 *)u4GetDspWorkBuf());
    vDspShareInfoInit(); // initialization for shared information
    //initial AFIFO hardware register
    vSetSPDIFInFIFO(u4GetAFIFOStart(FIRST_DECODER), u4GetAFIFOEnd(FIRST_DECODER), FIRST_DECODER);

#if 0
    WriteDspCommDram(ADDR_RD2D_POWERON_RETRY_COUNT,0);
#endif
    vWriteDspWORD (ADDR_RC2D_DDCO_FLAG, 0x0);
    /* initialization for state machine*/
    vDspStateInit(); // initialization for ADSP state machine
    _uDspState = (UINT8)DSP_IDLE; // initially, we enter suspend mode
#endif
    
    //while(TRUE) //prevent lint warning
    while(1)
    {
        SIZE_T zMsgSize = sizeof(UINT32);
        INT32  i4MsgRet;
     
        i4MsgRet = x_msg_q_receive(&u2MsgIdx,&u4Msg, &zMsgSize, &hDspCmdQueue, 1, X_MSGQ_OPTION_WAIT); // Light: Set queue as wait type

        VERIFY((i4MsgRet == OSR_OK) || (i4MsgRet == OSR_NO_MSG));

#ifdef ADSP_CHECK_RAM_CODE
        if (_fgDspWakeupOK)
        {
            LOG(7, "[DSP]========= Check RAM Start =========\n");
            
            if (fgCheckDspData())
            {
                LOG(7, "[DSP] RAM check pass\n");
            }
            else
            {
                LOG(7, "[DSP] Error: RAM check fail\n");
            }
            
            LOG(7, "[DSP]========== Check RAM End ==========\n");
        }
#endif        

        if (i4MsgRet == OSR_OK)
        {
            switch(u4Msg & 0xFFFF)
            {
                case ADSPTASK_MSG_INTERRUPT:
                    // interrupt
                     vDspIntSvc();
//                    LOG(9 , "Rcv ADSPTASK_MSG_INTERRUPT\n");
                break;
                
                case ADSPTASK_MSG_POWER_ON:
                    // set to power initial state
                    vDspStateInit(); // initialization for ADSP state machine
                    fgGetRet = fgDspInitState (); // Light: wait for DSP wakeup in fgDspInitState
                    if(fgGetRet)
                    {
                        _uDspState = (UINT8)DSP_CK_INT;
#if 0
                        WriteDspCommDram(ADDR_RD2D_POWERON_RETRY_COUNT,0xbeef);
#endif
                    }
                    else // wakeup isn't finished...
                    {
#if 1                    
                        LOG(1, "DSP wake up fail\n");
                        ASSERT(0);
#else

/*
                        LOG(1 , "ADSPTASK_MSG_POWER_ON_RETRY\n");
                        while(TRUE)
                        {
                            _u4PowerOnRetry ++;
                            AUD_DspClkSel(4);
                            vDspMemInit(_ucAdspWorkingBuffer);
                            vDspShareInfoInit(); // initialization for shared information
                            vDspStateInit(); // initialization for ADSP state machine
                            _uDspState = DSP_IDLE; // initially, we enter suspend mode
                            vDspStateInit(); // initialization for ADSP state machine
                            fgGetRet = fgDspInitState (); // Light: wait for DSP wakeup in fgDspInitState
                           
                            if (fgGetRet)
                            {
                                LOG(1 , "ADSPTASK_MSG_POWER_ON_RETRY COUNT = %d\n",_u4PowerOnRetry);
                                _uDspState = DSP_CK_INT;
                                break;
                            }
                            else if (_u4PowerOnRetry>1000) // Time out
                            {
                                LOG(1 , "ADSPTASK_MSG_POWER_ON_RETRY TIME OUT!!\n");
                               _uDspState = DSP_IDLE;
                               break;
                            }
                        }
                        WriteDspCommDram(ADDR_RD2D_POWERON_RETRY_COUNT, _u4PowerOnRetry);
*/
#endif	
                    }

                    LOG(9 , "ADSPTASK_MSG_POWER_ON\n");
                break;
                
                case ADSPTASK_MSG_POWER_OFF:
                    if(_fgDspPlay)
                    {
                      vDspCmd(DSP_STOP);
                    }
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
                    if(_fgDspPlayDec3)
                    {
                      vDspCmd(DSP_STOP_DEC3);
                    }
#endif
                    if(_fgDspPlayDec2)
                    {
                      vDspCmd(DSP_STOP_DEC2);
                    }
                    _uDspState = (UINT8)DSP_WAIT_POWER_OFF;
                    LOG(9 , "ADSPTASK_MSG_POWER_OFF\n");
                break;
               #if (1) //refine ADSDP control
                case ADSPTASK_MSG_INT_SAMPLE_RATE:
                    vDspSetSamplingRate();
                    vDspFlowControlNotify(D2RC_FLOW_CONTROL_SAMPLING_RATE);
                break;
                case ADSPTASK_MSG_INT_SAMPLE_RATE_DEC2:
                    vDspSetSamplingRateDec2();
                    vDspFlowControlNotify(D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC2);
                break;
                case ADSPTASK_MSG_INT_AOUT_ENABLE:
                	vDspFlowControlNotify(D2RC_FLOW_CONTROL_AOUT_ENABLE);
                break;
                case ADSPTASK_MSG_INT_AOUT_ENABLE_DEC2:
                	vDspFlowControlNotify(D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC2);
                break;
                case ADSPTASK_MSG_INT_STOP:
                    vDspFlowControlNotify(D2RC_FLOW_CONTROL_FLUSH_DONE);
                break;
                case ADSPTASK_MSG_INT_STOP_DEC2:
                    vDspFlowControlNotify(D2RC_FLOW_CONTROL_FLUSH_DONE_DEC2);
                break;                
                case ADSPTASK_MSG_INT_MODE_CHANGE:
                    vDspFlowControlNotify(D2RC_FLOW_CONTROL_MODE_CHANGE);
                break;
                case ADSPTASK_MSG_INT_MODE_CHANGE_DEC2:
                    vDspFlowControlNotify(D2RC_FLOW_CONTROL_MODE_CHANGE_DEC2);
                break;
                case ADSPTASK_MSG_INT_PAL_MODE:
                    vDspDemodDetModeNotify(INT_DSP_PAL_DETECTED_MODE, u4Msg>>16);
                break;
                case ADSPTASK_MSG_INT_JPN_MODE:
                    vDspDemodDetModeNotify(INT_DSP_JPN_DETECTED_MODE, u4Msg>>16);
                break;
                case ADSPTASK_MSG_INT_MTS_MODE:
                    vDspDemodDetModeNotify(INT_DSP_MTS_DETECTED_MODE, u4Msg>>16);
                break;
                case ADSPTASK_MSG_INT_DETECTOR_NOTIFY:
                    vDspTvSysDetectedNotify(AUD_DEC_MAIN, (TV_AUD_SYS_T)(u4Msg>>16));
                break;
                case ADSPTASK_MSG_INT_ATV_CHANGE:
                	vDspTvSysChangeNotify((TV_AUD_SYS_T)(u4Msg>>16));
                break;
                case ADSPTASK_MSG_INT_ATV_HDEV_SWITCH:
                    //vDspDemodDetModeNotify(INT_DSP_HDEV_AUTO_SWITCH, u4Msg>>16);
                    vDspHdevModeChangeNotify(u4Msg>>16);
                break;
                case ADSPTASK_MSG_INT_FM_RADIO_DET:
                    vDspFMRadioDetectionNotify((AUD_FM_RADIO_DET_T)(u4Msg>>16));
                break;
                case ADSPTASK_MSG_INT_MINER_NOTIFY:
                    vDspDemodDetModeNotify(INT_DSP_MINER_NOTIFY, u4Msg>>16);
                break;
                case ADSPTASK_MSG_INT_UPDATE_EFFECT:
			    	vDspUpdatePostEffect();
                break;
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
               case ADSPTASK_MSG_INT_SAMPLE_RATE_DEC3:
                    vDspSetSamplingRateDec3();
                    vDspFlowControlNotify(D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC3);
                break;
                case ADSPTASK_MSG_INT_AOUT_ENABLE_DEC3:
                	vDspFlowControlNotify(D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC3);
                break;
                case ADSPTASK_MSG_INT_STOP_DEC3:
                    vDspFlowControlNotify(D2RC_FLOW_CONTROL_FLUSH_DONE_DEC3);
                break; 
                case ADSPTASK_MSG_INT_MODE_CHANGE_DEC3:
                    vDspFlowControlNotify(D2RC_FLOW_CONTROL_MODE_CHANGE_DEC3);
                break;
#endif                
                #endif  // refine ADSP control
                case ADSPTASK_MSG_INT_SAMPLING_RATE_CHANGE:
                    vDspFlowControlNotify(D2RC_FLOW_CONTROL_SAMPLING_RATE_CHANGE);
                break;
                case ADSPTASK_MSG_INT_SAMPLING_RATE_CHANGE_DEC2:
                    vDspFlowControlNotify(D2RC_FLOW_CONTROL_SAMPLING_RATE_CHANGE_DEC2);
                break;		  				  		
                default:
                break;
            }

            vDspState(u4Msg);
        }
    }
}

UINT32 vReadPowerOnRetryCount(void)
{
    return _u4PowerOnRetry;
}

/******************************************************************************
* Function      : u4ADSP_DspReset
* Description   : Reset DSP state and registers, called by DSP monitor thread.
* Parameter     : None
* Return        : None
******************************************************************************/
INT32 u4ADSP_DspReset(void) //  -- DSP Force --
{
    BOOL    fgGetRet;
    INT32  i4MsgRet = 0;
    
    vDspStateInit(); // initialization for ADSP state machine
    
    fgGetRet = fgDspInitState ();
    if (fgGetRet)
    {
        _uDspState = (UINT8)DSP_CK_INT;
    }
    else // wakeup isn't finished...
    {
        LOG(0, "DSP wake up fail\n");
        ASSERT(0);
    }        

    LOG(1, "DSP ack ......\n");

    // Clean up original message queue   
#if 0    
    LOG(1, "Clean up adsp task message count = %d \n", u4MsgSz);

    for (i = 0; i < MSG_QUEUE_SIZE * 10; i++)
    {    
        i4MsgRet = x_msg_q_receive(&u2MsgIdx,&u4Msg, &zMsgSize, &hDspCmdQueue, 1, X_MSGQ_OPTION_NOWAIT); // Light: Set queue as wait type
    }
    // Send reset command to audio driver thread    
    //AUD_DRVCmd(AUD_DEC_MAIN, AUD_CMD_RESET); // need to take care for dangerous usage!!!!! by Shawn
#endif    

    return i4MsgRet;
}

void vSendADSPTaskMessage(UINT32 u4Msg)
{
    INT32 i4Ret;
    INT32 i4MsgOverFlowCnt = 0;

    do 
    {
        i4Ret = x_msg_q_send(hDspCmdQueue,&u4Msg,sizeof(UINT32),1);
        VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TOO_MANY));
        if (i4Ret == OSR_TOO_MANY)
        {
            x_thread_delay(10);
            i4MsgOverFlowCnt ++;
            if (i4MsgOverFlowCnt >= MSG_QUEUE_TIME_OUT)
            {
                LOG(1, "Audio message queue over flow => DSP hang ..\n");
                ASSERT(0);
            }
        }
    } while(i4Ret != OSR_OK);
}

#ifdef AUD_STATISTICS

/* Set error log period in Second */
void SetAudLogPeriod(UINT32 u4Period)
{
    if (u4Period == 0) // no log
    {
        _u4TriggerPeriod = 0;
        _u4LogPeriod = 0;
    }
    else if (u4Period == 1) // _u4TriggerPeriod at lease 1 sec
    {
        _u4TriggerPeriod = 1;
        _u4LogPeriod = 2;
    }
    else // u4Period >= 2
    {
        _u4TriggerPeriod = u4Period - 1;
        _u4LogPeriod = u4Period;
    }
}

//lint -e{727}
//lint -e{830}
//lint -e{550}
BOOL fgAudStatistics(void)
{
    static HAL_TIME_T _rStartTime;
    static HAL_TIME_T _rLastTime;
    HAL_TIME_T rCurTime;
    BOOL fgRet = TRUE;
    static UINT8 uLogState = 0;
    static UINT16 u2TriggerCnt = 0;
    static UINT16 u2LogCnt = 0;
    static UINT16 u2ErrorCnt = 0;
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
    if ( fgDecoderRealPlay(AUD_DEC_MAIN) || fgDecoderRealPlay(AUD_DEC_AUX)||fgDecoderRealPlay(AUD_DEC_THIRD))
#else
    if ( fgDecoderRealPlay(AUD_DEC_MAIN) || fgDecoderRealPlay(AUD_DEC_AUX))
#endif
    {
        if ( (_rStartTime.u4Seconds == 0) && (_rStartTime.u4Micros == 0) )
        {
            HAL_GetTime(&_rStartTime);
            _rLastTime = _rStartTime;
        }

        HAL_GetTime(&rCurTime);
        // trigger log at 2nd second
        if ((uLogState == 0) && ( (rCurTime.u4Seconds - _rLastTime.u4Seconds) >= _u4TriggerPeriod))
        {
            fgRet = fgTriggerDspErrorLogs();
            u2TriggerCnt ++;
            if (!fgRet)
            {
                LOG(7, "fgTriggerDspErrorLogs Time out (Dsp Busy)\n");
            }
            else
            {
                uLogState = 1;         // switch to get state
            }    
        }
        // get log at 3rd second
        if ((uLogState == 1) && ( (rCurTime.u4Seconds - _rLastTime.u4Seconds) >= _u4LogPeriod))
        {
            // Get DSP Previous Error Logs
            fgRet = fgGetDspErrorLogs((ADSP_ERROR_LOG_T *)_prAud);
            if (fgRet)
            {
                u2LogCnt++;            // DSP data ready
                uLogState = 0;         // switch to trigger state
                _rLastTime = rCurTime;
            }
            else
            {
                u2ErrorCnt ++;                      // DSP data not ready
                LOG(7, "fgGetDspErrorLogs Time out %d (Command Loss)\n", u2ErrorCnt);

                // If Command loss over 3 times, switch to trigger state
                if ((u2ErrorCnt % 3) == 0)
                {
                     uLogState = 0;  // switch to trigger state
                }
            }
           /*
            LOG(9, "(%x,%x,%x,%x,%x,%x)\n",
             _prAud->dwInputUnderRun,
             _prAud->dwInputUnderRunDec2,
             _prAud->dwInputUnderRunMixSound,
             _prAud->dwOutputUnderRun,
             _prAud->dwOutputUnderRunDec2,
             _prAud->dwOutputUnderRunMixSound);
            */ 
        }
    }
    return fgRet;
}

#endif
	

static void vADSPIRQHandler(UINT16 u2Vector)
{
    BOOL u1Ret;
    UINT32 u4Msg;
	//UINT32 u4Dsp2RiscInt;

    //u4Dsp2RiscInt =   AUD_READ32(REG_RISC_INT); 
    AUD_SET_BIT_SPECIAL(REG_RISC_INT, DSPB2RC_INTR);
 
    u1Ret = BIM_ClearIrq(VECTOR_AUDIO); // move before DSP_ClearInt() inside u4DspIRQSvc
    u4Msg = u4DspIRQSvc();
    vSendADSPTaskMessage(u4Msg);
    UNUSED(u1Ret);
    UNUSED(u2Vector);
}

static void vADSPIRQEnable(BOOL fgEnable)
{
    BOOL u1Ret;
	
    // Enable ADSP interrupt
    if(fgEnable)
    {
        u1Ret = BIM_EnableIrq(VECTOR_AUDIO);
    }
    else
    {
        u1Ret = BIM_DisableIrq(VECTOR_AUDIO);
    }
    UNUSED(u1Ret);
}
/******************************************************************************
* Function      : vADSPTaskInit
* Description   : initialization routine for ADSP Task
* Parameter     : None
* Return        : None
******************************************************************************/
void vADSPTaskInit(void) // first entry point of ADSP control routine
{
    HANDLE_T hThread1;
    x_os_isr_fct pfnOldIsr;

    // Create message queue
    VERIFY(x_msg_q_create(&hDspCmdQueue, ADSPTASK_CMD_Q_NAME, sizeof(UINT32),32) == OSR_OK);
    // Register ISR
    VERIFY(x_reg_isr(ADSP_INT_VECTOR, vADSPIRQHandler, &pfnOldIsr) == OSR_OK);
    vADSPIRQEnable(TRUE);

#ifdef AUD_STATISTICS

    {
        CRIT_STATE_T rState;
        DRV_DBASE_T* prDbase = DBS_Lock(&rState);
        ASSERT(prDbase != NULL);
        //_prAud = &(prDbase->rAud);
        VERIFY(DBS_Unlock(prDbase, rState));
    }

#endif  // PSR_STATISTICS
#if !defined(CC_MT8223)
//Move from the init of vDSPTaskMain
    {
        Printf("[ADSP] vADCInit");    
        vADCInit();

        Printf("[ADSP] AUD_DspClkSel(ADSP_CLK_216MHZ)");    
        AUD_DspClkSel(4);
        //5: dsp clk from tvd clk 432m/2;
        //6: dsp clk from mem clk
        //4: dsp clk from usb clk 240m
        LOG(1, "[DSP]ADSP_CLK_126MHZ\n");

        //APLL ref SYS
        Printf("[ADSP] APLL ref SYS");
        vIO32WriteFldAlign(CKGEN_AFECFG5, 0, FLD_RG_SYS_DIGI_SEL);
        vIO32WriteFldAlign(CKGEN_AFECFG5, 1, FLD_RG_SYS_DIGI_DIV_SEL);
        vIO32WriteFldAlign(CKGEN_AFECFG5, 1, FLD_RG_SYS_AUADC_SEL);
        vIO32WriteFldAlign(CKGEN_AFECFG4, 3, FLD_RG_SYS_APLL_SEL);

        //AIN clock
        Printf("[ADSP] AIN Clock");
        vIO32WriteFldAlign(CKGEN_AIN_CKCFG, 0, FLD_AIN_CK_PD);
        vIO32WriteFldAlign(CKGEN_AIN_CKCFG, 0, FLD_AIN_CK_TST);
        vIO32WriteFldAlign(CKGEN_AIN_CKCFG, 1, FLD_AIN_CK_SEL);

        //ADSP
    vDspMemInit((UINT8 *)u4GetDspWorkBuf());
    vDspShareInfoInit(); // initialization for shared information
    //initial AFIFO hardware register
    vSetSPDIFInFIFO(u4GetAFIFOStart(FIRST_DECODER), u4GetAFIFOEnd(FIRST_DECODER), FIRST_DECODER);

#if 0
    WriteDspCommDram(ADDR_RD2D_POWERON_RETRY_COUNT,0);
#endif
    vWriteDspWORD (ADDR_RC2D_DDCO_FLAG, 0x0);
    /* initialization for state machine*/
    vDspStateInit(); // initialization for ADSP state machine
    _uDspState = (UINT8)DSP_IDLE; // initially, we enter suspend mode
    }
//End
#endif  // PSR_STATISTICS
    
    // Create ADSP task
    VERIFY(x_thread_create(&hThread1, ADSPTASK_NAME, ADSPTASK_STACK_SIZE, ADSPTASK_THREAD_PRIORITY,
          vADSPTaskMain, 0, NULL) == OSR_OK);
}
/******************************************************************************
* Function      : fgDecoderStopped,fgDecoderRealPlay,vSendADSPCmd,vADSPPowerON,
*                 vADSPPowerOFF
* Description   : APIs for external use
* Parameter     : None
* Return        : None
******************************************************************************/
#ifdef CC_MT5391_AUD_3_DECODER
//#if 0
BOOL fgDecoderStopped(UINT8 uDecIndex)
{
    volatile DSPSTATUS_T* prDspStatus[3]={&_rDspStatus,&_rDspStatusDec2, &_rDspStatusDec3};
    
    if(uDecIndex <= THIRD_DECODER)
    {
      return(prDspStatus[uDecIndex]->fgDspStop);
    }
    return(FALSE); /* default value*/
}

BOOL fgDecoderRealPlay(UINT8 uDecIndex)
{
    volatile DSPSTATUS_T* prDspStatus[3]={&_rDspStatus,&_rDspStatusDec2, &_rDspStatusDec3};
    
    if(uDecIndex <= THIRD_DECODER)
    {
      return(prDspStatus[uDecIndex]->fgDspRealPlay);
    }
    return(FALSE); /* default value*/

}

BOOL fgDecoderPlaySent(UINT8 uDecIndex)
{
    volatile DSPSTATUS_T* prDspStatus[3]={&_rDspStatus,&_rDspStatusDec2, &_rDspStatusDec3};
    
    if(uDecIndex <= THIRD_DECODER)
    {
      return(prDspStatus[uDecIndex]->fgDspPlay);
    }
    return(FALSE); /* default value*/

}

#else   // CC_MT5391_AUD_3_DECODER
BOOL fgDecoderStopped(UINT8 uDecIndex)
{
    volatile DSPSTATUS_T* prDspStatus[2]={&_rDspStatus,&_rDspStatusDec2};
    
    if(uDecIndex <= SECOND_DECODER)
    {
      return(prDspStatus[uDecIndex]->fgDspStop);
    }
    return(FALSE); /* default value*/
}

BOOL fgDecoderRealPlay(UINT8 uDecIndex)
{
    volatile DSPSTATUS_T* prDspStatus[2]={&_rDspStatus,&_rDspStatusDec2};
    
    if(uDecIndex <= SECOND_DECODER)
    {
      return(prDspStatus[uDecIndex]->fgDspRealPlay);
    }
    return(FALSE); /* default value*/

}

BOOL fgDecoderPlaySent(UINT8 uDecIndex)
{
    volatile DSPSTATUS_T* prDspStatus[2]={&_rDspStatus,&_rDspStatusDec2};
    
    if(uDecIndex <= SECOND_DECODER)
    {
      return(prDspStatus[uDecIndex]->fgDspPlay);
    }
    return(FALSE); /* default value*/

}
#endif

BOOL fgADSPIsIdle(void)
{
    return ( (!_fgDspWakeUp) || (!_fgDspWakeupOK) );
}

void vADSPPowerON(void)
{
    vSendADSPTaskMessage(ADSPTASK_MSG_POWER_ON);
}

void vADSPPowerOFF(void)
{
    vSendADSPTaskMessage(ADSPTASK_MSG_POWER_OFF);
}

/******************************************************************************
* Function      : u4ReadShmUINT32
* Description   : read content of shared memory in UINT32
* Parameter     : u2Addr: address of shared memory
* Return        : None
******************************************************************************/
UINT32 u4ReadShmUINT32(UINT16 u2Addr)
{
    return(u4ReadDspShmDWRD(u2Addr));
}
/******************************************************************************
* Function      : u4ReadShmUINT16
* Description   : read content of shared memory in UINT16
* Parameter     : u2Addr: address of shared memory
* Return        : None
******************************************************************************/
UINT16 u2ReadShmUINT16(UINT16 u2Addr)
{
    return(u2ReadDspShmWORD(u2Addr));
}
/******************************************************************************
* Function      : u4ReadShmUINT32
* Description   : read content of shared memory in UINT8
* Parameter     : u2Addr: address of shared memory
* Return        : None
******************************************************************************/
UINT8 uReadShmUINT8(UINT16 u2Addr)
{
    return(uReadDspShmBYTE(u2Addr));
}
/******************************************************************************
* Function      : vWriteShmUINT32
* Description   : write content of shared memory in UINT32
* Parameter     : u2Addr: address of shared memory, u4Value: value in UINT32
* Return        : None
******************************************************************************/
void vWriteShmUINT32(UINT16 u2Addr,UINT32 u4Value)
{
    vWriteDspShmDWRD(u2Addr,u4Value);
}
/******************************************************************************
* Function      : vWriteShmUINT16
* Description   : write content of shared memory in UINT16
* Parameter     : u2Addr: address of shared memory, u2Value: value in UINT16
* Return        : None
******************************************************************************/
void vWriteShmUINT16(UINT16 u2Addr,UINT16 u2Value)
{
    vWriteDspShmWORD(u2Addr,u2Value);
}

/******************************************************************************
* Function      : vWriteShmUINT8
* Description   : write content of shared memory in UINT8
* Parameter     : u2Addr: address of shared memory, uValue: value in UINT8
* Return        : None
******************************************************************************/
void vWriteShmUINT8(UINT16 u2Addr,UINT32 uValue)
{
    vWriteDspShmBYTE(u2Addr,uValue);
}

