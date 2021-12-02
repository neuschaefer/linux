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
 * $Author: jiuqiang.ming $
 * $Date: 2012/10/31 $
 * $RCSfile: adsp_task.c,v $
 * $Revision: #4 $
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
#include "x_hal_5381.h"
#include "dsp_common.h"
#include "ctrl_para.h"
#include "adsp_task.h"
#include "dsp_uop.h"
#include "dsp_shm.h"
#include "dsp_intf.h"
#include "dsp_func.h"
#include "dsp_rg_ctl.h"
#include "aud_debug.h"
#include "aud_hw.h"
#include "fbm_drvif.h"  // for DSP memory allocation
#include "drv_common.h"
#include "drv_adsp.h"
#include "drvcust_if.h"
#include "psr_drvif.h"

#include "x_os.h"
#include "x_assert.h"

#include "hw_ckgen.h"

#define AUD_STATISTICS

#ifdef AUD_STATISTICS
#include "drv_dbase.h"
#include "x_timer.h"
#endif

#ifdef LINUX_TURNKEY_SOLUTION
#include <linux/module.h>
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

extern BOOL fgTriggerDspErrorLogs(void);
extern BOOL fgGetDspErrorLogs(ADSP_ERROR_LOG_T* ptADSPErrLog);
extern void vDspSetSamplingRate(UINT8 u1DecId);
extern void AUD_DspClkSel(UINT8 u1ADSPclkSel);
extern UINT32 BSP_GetMemSize(void);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

HANDLE_T hDspCmdQueue;
HANDLE_T hDspCmdDataQueue;  //ADSP Data Queue.
HANDLE_T _hAdspThread;
static HANDLE_T _hRdDspSramSema;


#ifdef AUD_STATISTICS

static UINT32 _u4TriggerPeriod = 2;    // Seconds for Trigger Log
static UINT32 _u4LogPeriod = 3;        // Seconds per message
static PARAM_AUD_T* _prAud = NULL;

#endif  // PSR_STATISTICS

// Light added for wakeup retry (poweron retry)
static UINT32   _u4PowerOnRetry=0;

INT8 _i1DspMemBlock = -1;
UINT32 _u4DspMemBufAddr = (UINT32)NULL;
static UINT32 _u4DspMemBufSize = 0;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define DSP_MEM_BLOCK(u4Addr)	(PHYSICAL(u4Addr)/DSP_BUF_BLOCK)

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
BOOL _fgTaskInit = FALSE;

UINT32 vReadPowerOnRetryCount(void);
void vSendADSPTaskMessage(UINT32 u4Msg);
void vSendADSPTaskMessageNoWait(UINT32 u4Msg);
void vADSPTaskMain_Init(void);

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
	if ((pu4Addr != NULL) && (pu4Size != NULL) && (_u4DspMemBufAddr != (UINT32)NULL))
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
    UINT32 u4MemSize;

	// Check if memory is initialized
	VERIFY(_i1DspMemBlock >= 0);
 
	// Check 256 MB boundary
	u4MemSize = BSP_GetMemSize();
    VERIFY(PHYSICAL(u4Addr / u4MemSize) == 0);

	// If within 16 MB
	if ((PHYSICAL(u4Addr) / DSP_BUF_BLOCK) == 0)
	{
		return (PHYSICAL(u4Addr) + (UINT32)((_i1DspMemBlock)*DSP_BUF_BLOCK));
	}
	else if (((PHYSICAL(u4Addr)/DSP_BUF_BLOCK) > 0) && 
            #if (defined(CC_MT5365) || defined(CC_MT5395))
        	// If within 16 ~ 512 MB
            ((PHYSICAL(u4Addr)/DSP_BUF_BLOCK) < 32)
            #elif (defined(CC_MT5368) || defined(CC_MT5389))
        	// If within 16 ~ 1024 MB
            ((PHYSICAL(u4Addr)/DSP_BUF_BLOCK) < 64)            
            #elif (defined(CC_MT5396))
        	// If within 16 ~ 2048 MB
            ((PHYSICAL(u4Addr)/DSP_BUF_BLOCK) < 128)
            #endif            
            )
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
__align(0x100) static UINT8 _au1AdspWorkingBufferUnAligned[DSP_WORK_BUF_SIZE + DSP_WORKING_BUF_ALIGN] ;
#endif

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
#ifndef DSP_TEST_DRAM_BLOCK
    FBM_POOL_T* prFbmPool;
#endif
    UINT32  u4BufAddr;
    UINT32  u4BufSize;
    UINT32  u4MemSize;
    
#ifndef DSP_TEST_DRAM_BLOCK
    // Get buffer from FB pool
    prFbmPool = FBM_GetPoolInfo((UINT8) FBM_POOL_TYPE_DSP);  

    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != (UINT32)NULL);
    u4BufAddr= prFbmPool->u4Addr;
    u4BufSize = prFbmPool->u4Size;
    _u4DspMemBufAddr = u4BufAddr;
    _u4DspMemBufSize = u4BufSize;
#else
    UINT32 u4TmpAddr;    
    u4TmpAddr = _AudAlign((UINT32)_au1AdspWorkingBufferUnAligned,0x100);
    u4BufAddr = u4TmpAddr + (64*1024*1024);
    u4BufSize = DSP_WORK_BUF_SIZE;
    _u4DspMemBufAddr = u4BufAddr;
    _u4DspMemBufSize = u4BufSize;    
#endif
    
    // Check 256 Bytes alignment
    VERIFY(_AudAlign(VIRTUAL(u4BufAddr), 0x100) == VIRTUAL(u4BufAddr));

    // Check 64 MB boundary
    u4MemSize = BSP_GetMemSize();
    VERIFY(PHYSICAL((u4BufAddr + u4BufSize) / u4MemSize) == 0);

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
    DSP_FlushInvalidateDCacheFree(u4BufAddr, prFbmPool->u4Size);
#else
    x_memset((VOID *)VIRTUAL(u4BufAddr), 0, u4BufSize);
#endif
    return VIRTUAL(u4BufAddr);
}

/******************************************************************************
* Function      : u4GetAFIFOStart
* Description   : get audio FIFO start address in physical address
* Parameter     : uDecIndex: 0: first decoder,1: second decoder
* Return        : None
******************************************************************************/
UINT32 u4GetAFIFOStart(UINT8 uDecIndex)
{
#ifdef CC_MT5391_AUD_3_DECODER  
    #ifdef CC_AUD_DEC3_USE_FIFO1// USE_FIFO3 : dec3 use fifo3
    if (uDecIndex == AUD_DEC_THIRD)
    {
        // Audio description uses AFIFO
        uDecIndex = AUD_DEC_MAIN;
    }
    #endif
#endif    
    
    return _u4AFIFO[uDecIndex][0];
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(u4GetAFIFOStart);
#endif

/******************************************************************************
* Function      : u4GetAFIFOEnd
* Description   : get audio FIFO end address
* Parameter     : uDecIndex: 0: first decoder,1: second decoder
* Return        : None
******************************************************************************/
UINT32 u4GetAFIFOEnd(UINT8 uDecIndex)
{
#ifdef CC_MT5391_AUD_3_DECODER
    #ifdef CC_AUD_DEC3_USE_FIFO1// USE_FIFO3 : dec3 use fifo3
    if (uDecIndex == AUD_DEC_THIRD)
    {
        // Audio description uses AFIFO
        uDecIndex = AUD_DEC_MAIN;
    }
    #endif
#endif    

    return _u4AFIFO[uDecIndex][1];
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(u4GetAFIFOEnd);
#endif

UINT32 u4GetABufPnt(UINT8 uDecIndex)
{
    if (uDecIndex == AUD_DEC_MAIN)
    {
        AUD_WRITE32(REG_ABUF_PNT,0);
        return ((_u4AFIFO[0][0]&0xff000000) | AUD_READ32(REG_ABUF_PNT));
    }
    else
    {
        AUD_WRITE32(REG_ABUF_PNT,0);
        return ((_u4AFIFO[1][0]&0xff000000) | AUD_READ32(REG_ABUF_PNT));
    }
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(u4GetABufPnt);
#endif

UINT32 u4GetAWritePnt(UINT8 uDecIndex)
{
    if (uDecIndex == AUD_DEC_MAIN)
    {
        AUD_WRITE32(REG_ABUF_PNT,1);
        return ((_u4AFIFO[0][0]&0xff000000) | AUD_READ32(REG_ABUF_PNT));
    }
    else
    {
        AUD_WRITE32(REG_ABUF_PNT_DEC2,1);
        return ((_u4AFIFO[1][0]&0xff000000) | AUD_READ32(REG_ABUF_PNT_DEC2));
    }
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(u4GetAWritePnt);
#endif

extern BOOL PSR_RiscSetAudioWp(UINT8 u1DeviceId, UINT32 u4WritePointer);
void vSetAWritePnt(UINT32 u4WritePointer)
{
    PSR_RiscSetAudioWp(AUD_DEC_MAIN, u4WritePointer);
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(vSetAWritePnt);
#endif

#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT 
/******************************************************************************
* Function      : u4GetHDMIParserFIFOStart
* Description   : get HDMI parser FIFO start address in physical address
* Parameter     : void
* Return        : HDMI parser FIFO start address
******************************************************************************/
UINT32 u4GetHDMIParserFIFOStart(void)
{
    return _u4AFIFOHdmiParser[0];
}

/******************************************************************************
* Function      : u4GetHDMIParserFIFOEnd
* Description   : get HDMI parser FIFO end address in physical address
* Parameter     : void
* Return        : HDMI parser FIFO end address
******************************************************************************/
UINT32 u4GetHDMIParserFIFOEnd(void)
{
    return _u4AFIFOHdmiParser[1];
}
#endif

#ifdef CC_AUD_4_DECODER_SUPPORT
UINT32 u4GetSBCEncFIFOStart(void)
{
    return _u4SBCEncBuf[0];
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(u4GetSBCEncFIFOStart);
#endif

UINT32 u4GetSBCEncFIFOEnd(void)
{
    return _u4SBCEncBuf[1];
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(u4GetSBCEncFIFOEnd);
#endif

UINT32 u4GetSBCEncWritePnt(void)
{
    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    return ((_u4SBCEncBuf[0]&0xff000000) | dReadDspCommDram(ADDR_RC2D_SBCE_WRITE_PTR_DEC4));
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(u4GetSBCEncWritePnt);
#endif
#endif

//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
UINT32 u4GetUploadFIFOStart(void)
{
    return (_u4DspCmptBuf[TYPE_COMM_DRAM_IDX] + AUD_UPLOAD_BUFFER_BASE);
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(u4GetUploadFIFOStart);
#endif

UINT32 u4GetUploadFIFOEnd(void)
{
    return (_u4DspCmptBuf[TYPE_COMM_DRAM_IDX] + AUD_UPLOAD_BUFFER_BASE + AUD_UPLOAD_BUFFER_SIZE);
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(u4GetUploadFIFOEnd);
#endif

UINT32 u4GetUploadWritePnt(void)
{
    DSP_FlushInvalidateDCache(CACHE_FLUSH_TYPE_DATA, TYPE_COMM_DRAM_IDX);
    //DSP_FlushInvalidateDCacheSmall2(ADDR_RC2D_UPLOAD_WRITE_PTR);    
    return ((_u4DspCmptBuf[TYPE_COMM_DRAM_IDX]&0xff000000) | dReadDspCommDram(ADDR_RC2D_UPLOAD_WRITE_PTR));
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(u4GetUploadWritePnt);
#endif

//#endif

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
    
    // Light added for poweron retry
    _u4PowerOnRetry = 0;

    VERIFY(pvArg == NULL);

    // Move initialzation relatrd code to vADSPTaskMainInit. To make sure the AUD_Init flow correctness.
    
    while (1)
    {
        SIZE_T zMsgSize = sizeof(UINT32);
        INT32  i4MsgRet;
     
        i4MsgRet = x_msg_q_receive(&u2MsgIdx,&u4Msg, &zMsgSize, &hDspCmdQueue, 1, X_MSGQ_OPTION_WAIT); // Light: Set queue as wait type

        VERIFY((i4MsgRet == OSR_OK) || (i4MsgRet == OSR_NO_MSG));

#ifdef ADSP_CHECK_RAM_CODE
        if (_rDspStatus[AUD_DEC_MAIN].fgDspWakeUpOk)
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
                // LOG(9 , "Rcv ADSPTASK_MSG_INTERRUPT\n");
                break;
            case ADSPTASK_MSG_INTERRUPT_DATA:
                vDspIntSvc_Data();  //ADSP Data Queue.
                break;                
            case ADSPTASK_MSG_POWER_ON:
                // set to power initial state
                vDspStateInit(); // initialization for ADSP state machine
                fgGetRet = fgDspInitState (); // Light: wait for DSP wakeup in fgDspInitState
                if (fgGetRet)
                {
                    _uDspState = (UINT8)DSP_CK_INT;
                }
                else // wakeup isn't finished...
                {
                    LOG(1, "DSP wake up fail\n");
                    ASSERT(0);
                }

                LOG(9 , "ADSPTASK_MSG_POWER_ON\n");
                break;
            case ADSPTASK_MSG_POWER_OFF:
                if (_rDspStatus[AUD_DEC_MAIN].fgDspPlay)
                {
                    vDspCmd(DSP_STOP);
                }
                if (_rDspStatus[AUD_DEC_AUX].fgDspPlay)
                {
                    vDspCmd(DSP_STOP_DEC2);
                }
#ifdef CC_MT5391_AUD_3_DECODER
                if (_rDspStatus[AUD_DEC_THIRD].fgDspPlay)
                {
                    vDspCmd(DSP_STOP_DEC3);
                }
#endif                
#ifdef CC_AUD_4_DECODER_SUPPORT
                if (_rDspStatus[AUD_DEC_4TH].fgDspPlay)
                {
                    vDspCmd(DSP_STOP_DEC4);
                }
#endif
                _uDspState = (UINT8)DSP_WAIT_POWER_OFF;
                LOG(9 , "ADSPTASK_MSG_POWER_OFF\n");
                break;
            case ADSPTASK_MSG_INT_SAMPLE_RATE:
                vDspSetSamplingRate(AUD_DEC_MAIN);
                vDspFlowControlNotify(D2RC_FLOW_CONTROL_SAMPLING_RATE);
                break;
            case ADSPTASK_MSG_INT_SAMPLE_RATE_DEC2:
                vDspSetSamplingRate(AUD_DEC_AUX);
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
            case ADSPTASK_MSG_INT_SAMPLE_RATE_DEC3:
                vDspSetSamplingRate(AUD_DEC_THIRD);
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
            case ADSPTASK_MSG_INT_SAMPLING_RATE_CHANGE:
                vDspFlowControlNotify(D2RC_FLOW_CONTROL_SAMPLING_RATE_CHANGE);
                break;
            case ADSPTASK_MSG_INT_SAMPLING_RATE_CHANGE_DEC2:
                vDspFlowControlNotify(D2RC_FLOW_CONTROL_SAMPLING_RATE_CHANGE_DEC2);
                break;
#ifdef CC_AUD_4_DECODER_SUPPORT
            case ADSPTASK_MSG_INT_SAMPLE_RATE_DEC4:
                vDspSetSamplingRate(AUD_DEC_4TH);
                vDspFlowControlNotify(D2RC_FLOW_CONTROL_SAMPLING_RATE_DEC4);
                break;
            case ADSPTASK_MSG_INT_AOUT_ENABLE_DEC4:
                vDspFlowControlNotify(D2RC_FLOW_CONTROL_AOUT_ENABLE_DEC4);
                break;
            case ADSPTASK_MSG_INT_STOP_DEC4:
                vDspFlowControlNotify(D2RC_FLOW_CONTROL_FLUSH_DONE_DEC4);
                break; 
            case ADSPTASK_MSG_INT_MODE_CHANGE_DEC4:
                vDspFlowControlNotify(D2RC_FLOW_CONTROL_MODE_CHANGE_DEC4);
                break;
#endif
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
                LOG(1, "Audio message queue over flow\n");
                ASSERT(0);
            }
        }
    } while(i4Ret != OSR_OK);
}

void vSendADSPTaskMessageNoWait(UINT32 u4Msg)
{
    INT32 i4Ret;

    i4Ret = x_msg_q_send(hDspCmdQueue,&u4Msg,sizeof(UINT32),1);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "Audio message queue over flow\n");
        ASSERT(0);
    }
}

void vSendADSPTaskData(UINT32 u4Msg) //ADSP Data Queue.
{
    INT32 i4Ret;

    i4Ret = x_msg_q_send(hDspCmdDataQueue,&u4Msg,sizeof(UINT32),1);
    if (i4Ret != OSR_OK)
    {
        LOG(1, "DSP Data queue over flow\n");
        ASSERT(0);
    }
}

void vGetADSPTaskData(UINT32 *u4Msg) //ADSP Data Queue.
{
    INT32 i4Ret;
    UINT16 u2MsgIdx;
    SIZE_T zMsgSize = sizeof(UINT32);
    
    i4Ret = x_msg_q_receive(&u2MsgIdx, u4Msg, &zMsgSize, &hDspCmdDataQueue, 1, X_MSGQ_OPTION_WAIT); 
    if (i4Ret != OSR_OK)  //Should follow "Put 1 cmd, put 1 data", so there is no case the that "No Data"
    {
        LOG(1, "DSP Data queue no data\n");
        ASSERT(0);
    }
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
    HAL_TIME_T rCurTime = {0};
    BOOL fgRet = TRUE;
    static UINT8 uLogState = 0;
    static UINT16 u2TriggerCnt = 0;
    static UINT16 u2LogCnt = 0;
    static UINT16 u2ErrorCnt = 0;
    
#ifdef CC_AUD_4_DECODER_SUPPORT
    if (fgDecoderRealPlay(AUD_DEC_MAIN) || fgDecoderRealPlay(AUD_DEC_AUX) ||
        fgDecoderRealPlay(AUD_DEC_THIRD) || fgDecoderRealPlay(AUD_DEC_4TH))
#elif defined(CC_MT5391_AUD_3_DECODER)
    if (fgDecoderRealPlay(AUD_DEC_MAIN) || fgDecoderRealPlay(AUD_DEC_AUX) || fgDecoderRealPlay(AUD_DEC_THIRD))
#else
    if (fgDecoderRealPlay(AUD_DEC_MAIN) || fgDecoderRealPlay(AUD_DEC_AUX))
#endif
    {
        if ((_rStartTime.u4Seconds == 0) && (_rStartTime.u4Micros == 0))
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
        }
    }
    return fgRet;
}
#endif

static void vADSPIRQHandler(UINT16 u2Vector)
{
    UINT32 u4Msg;
    UINT32 u4Dsp2RiscInt;
 
    u4Dsp2RiscInt = AUD_READ32(REG_RISC_INT); 
    if ((u4Dsp2RiscInt & DSPA2RC_INTR) != 0)
    {
        AUD_SET_BIT(REG_RISC_INT, DSPA2RC_INTR);
    }
    
    u4Msg = u4DspIRQSvc();
    //vSendADSPTaskMessage(u4Msg);
    vSendADSPTaskMessageNoWait(u4Msg);    
    UNUSED(u2Vector);
}

void vADSPIRQEnable(BOOL fgEnable)
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
void CheckSumAll(void);   //DSP_Vestel_8M
void vADSPTaskMain_Init(void)
{
    vADCInit();
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    AUD_DspClkSel(1); // 1: sawlesspll_d1_ck (432MHz) or 4: enetpll_d1p5_ck (450MHz)
#else
    if (DRVCUST_InitGet(eFlagDDRQfp))
    {
        AUD_DspClkSel(SYSPLL_D1_CK);
        LOG(1, "[DSP]System clock (459MHz)\n");
    }
    else
    {
        AUD_DspClkSel(SYSPLL_D1_CK);
        LOG(1, "[DSP]System clock (459MHz)\n");
    }
#endif

    vIO32WriteFldAlign(CKGEN_AIN_CKCFG, 0, FLD_AIN_CK_PD);
    vIO32WriteFldAlign(CKGEN_AIN_CKCFG, 0, FLD_AIN_CK_TST);
    vIO32WriteFldAlign(CKGEN_AIN_CKCFG, 1, FLD_AIN_CK_SEL); // 1:syspll_d4_ck (432/4 = 108), 2: syspll_d6_ck (648/6=108 for 5368/96)

    vDspMemInit((UINT8 *)u4GetDspWorkBuf());
    vDspShareInfoInit(); // initialization for shared information
    //initial AFIFO hardware register
    vSetSPDIFInFIFO(AUD_DEC_MAIN, u4GetAFIFOStart(AUD_DEC_MAIN), u4GetAFIFOEnd(AUD_DEC_MAIN));
    #ifdef DSP_TEST_LINE2
    vSetSPDIFInFIFO(AUD_DEC_AUX, u4GetAFIFOStart(AUD_DEC_AUX), u4GetAFIFOEnd(AUD_DEC_AUX));
    #endif
    //initial Paser register
    VERIFY(PSR_SoftSetAudioFIFO(AUD_DEC_MAIN, u4GetAFIFOStart(AUD_DEC_MAIN), u4GetAFIFOEnd(AUD_DEC_MAIN)));    
    vOplFuncInit();

    vWriteDspWORD (ADDR_RC2D_DDCO_FLAG, 0x0);
    CheckSumAll();  //DSP_Vestel_8M
    /* initialization for state machine*/
    vDspStateInit(); // initialization for ADSP state machine
    _uDspState = (UINT8)DSP_IDLE; // initially, we enter suspend mode

}

/******************************************************************************
* Function      : vADSPTaskInit
* Description   : initialization routine for ADSP Task
* Parameter     : None
* Return        : None
******************************************************************************/
void vADSPTaskInit(void) // first entry point of ADSP control routine
{
#ifdef AUD_STATISTICS
    {
        CRIT_STATE_T rState;
        DRV_DBASE_T* prDbase = DBS_Lock(&rState);
        
        ASSERT(prDbase != NULL);
        //_prAud = &(prDbase->rAud);
        VERIFY(DBS_Unlock(prDbase, rState));
    }
#endif  // PSR_STATISTICS
    
    vADSPTaskMain_Init();

    if (!_fgTaskInit)
    {
        x_os_isr_fct pfnOldIsr;    
        
        // Create message queue
        VERIFY(x_msg_q_create(&hDspCmdQueue, ADSPTASK_CMD_Q_NAME, sizeof(UINT32),128) == OSR_OK);
        VERIFY(x_msg_q_create(&hDspCmdDataQueue, ADSPTASK_CMD_DATA_Q_NAME, sizeof(UINT32),128) == OSR_OK); //ADSP Data Queue.
        // Register ISR
        VERIFY(x_reg_isr(ADSP_INT_VECTOR, vADSPIRQHandler, &pfnOldIsr) == OSR_OK);
        //vADSPIRQEnable(TRUE); // remove because x_reg_isr has enabled it, do not need enable twice

        // Create ADSP task
        VERIFY(x_thread_create(&_hAdspThread, ADSPTASK_NAME, ADSPTASK_STACK_SIZE, ADSPTASK_THREAD_PRIORITY,
            vADSPTaskMain, 0, NULL) == OSR_OK);  

		VERIFY(x_sema_create(&_hRdDspSramSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK) == OSR_OK);

        _fgTaskInit = TRUE;
    }
    /*else
    {
        vADSPIRQEnable(TRUE); // remove because irq has been enabled, do not need enable twice
    }*/
}

/******************************************************************************
* Function      : fgDecoderStopped,fgDecoderRealPlay,vSendADSPCmd,vADSPPowerON,
*                 vADSPPowerOFF
* Description   : APIs for external use
* Parameter     : None
* Return        : None
******************************************************************************/
BOOL fgDecoderStopped(UINT8 u1DecId)
{
    if (u1DecId < AUD_DEC_NUM)
    {
        return (_rDspStatus[u1DecId].fgDspStop);
    }
    return (FALSE); /* default value*/
}

BOOL fgDecoderRealPlay(UINT8 u1DecId)
{
    if (u1DecId < AUD_DEC_NUM)
    {
      return(_rDspStatus[u1DecId].fgDspRealPlay);
    }
    return(FALSE); /* default value*/
}

BOOL fgDecoderPlaySent(UINT8 u1DecId)
{
    if (u1DecId < AUD_DEC_NUM)
    {
      return (_rDspStatus[u1DecId].fgDspPlay);
    }
    return(FALSE); /* default value*/
}

BOOL fgADSPIsIdle(void)
{
    return ( (!_rDspStatus[AUD_DEC_MAIN].fgDspWakeUp) || (!_rDspStatus[AUD_DEC_MAIN].fgDspWakeUpOk) );
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

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
UINT32 _AUD_DspGetMixSndReadPtr(UINT8 u1StreamID)
{
    UINT32 u4ReadPtr;
    DSP_FlushInvalidateDCacheSmall2(ADDR_RC2D_DRAM_MIXSOUND_CLIP1_RP + 16*u1StreamID);
    u4ReadPtr = (((UINT32)_i1DspMemBlock)<<24) + dReadDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_RP + 16*u1StreamID); //TEST
    return u4ReadPtr;
}

void _AUD_DspSetMixSndWritePtr(UINT8 u1StreamID, UINT32 u4WritePtr)
{
    u4WritePtr &= 0xffffff;
    WriteDspCommDram2 (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_WP + 16*u1StreamID, u4WritePtr); //TEST
}

void _AUD_DspShowMixSndStatus(UINT8 u1StreamID)
{
    Printf("ADDR_RC2D_DRAM_MIXSOUND_BASE: 0x%04x\n", (0x8000+ADDR_RC2D_DRAM_MIXSOUND_BASE));
    Printf("ADDR_RC2D_DRAM_MIXSOUND_STATUS(%x): 0x%08x\n", ADDR_RC2D_DRAM_MIXSOUND_STATUS, dReadDspCommDram(ADDR_RC2D_DRAM_MIXSOUND_STATUS));
    Printf("  1: working, 0: idle\n");
    Printf("ADDR_RC2D_DRAM_MIXSOUND_CONFIG(%x): 0x%08x\n", ADDR_RC2D_DRAM_MIXSOUND_CONFIG,  dReadDspCommDram(ADDR_RC2D_DRAM_MIXSOUND_CONFIG));
    Printf("  bit  0: 0-> no need upsampling, 1-> 2x upsampling\n");
    Printf("  bit  1: 2-> 4x upsampling, 3-> 8x upsampling\n");
    Printf("  bit  4: source mode : 0->mono, 1->stereo\n");
    Printf("  bit  8: mixing data to left & right output\n");
    Printf("  bit  9: mixing data to surround output\n");
    Printf("  bit 10: mixing data to center output\n");
    Printf("  bit 11: mixing data to channel 7 & 8\n");
    Printf("  bit 12: mixing data to channel 9 & 10\n");
    Printf("ADDR_RC2D_DRAM_MIXSOUND_PCM_ADDR(%x): 0x%08x\n", ADDR_RC2D_DRAM_MIXSOUND_PCM_ADDR, dReadDspCommDram(ADDR_RC2D_DRAM_MIXSOUND_PCM_ADDR));
    Printf("ADDR_RC2D_DRAM_MIXSOUND_PCM_LEN(%x): 0x%08x\n", ADDR_RC2D_DRAM_MIXSOUND_PCM_LEN, dReadDspCommDram(ADDR_RC2D_DRAM_MIXSOUND_PCM_LEN));
    Printf("ADDR_RC2D_DRAM_MIXSOUND_GAIN(%x): 0x%08x\n", ADDR_RC2D_DRAM_MIXSOUND_GAIN, dReadDspCommDram(ADDR_RC2D_DRAM_MIXSOUND_GAIN));

    if (u1StreamID < MAX_AUD_MIXSND_STREAM_NUM)
    {
        Printf("ADDR_RC2D_DRAM_MIXSOUND_CLIP%d_RP(%x): 0x%08x\n", u1StreamID+1, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_RP+(UINT32)u1StreamID*16, dReadDspCommDram(ADDR_RC2D_DRAM_MIXSOUND_CLIP1_RP+(UINT32)u1StreamID*16)); //TEST
        Printf("ADDR_RC2D_DRAM_MIXSOUND_CLIP%d_WP(%x): 0x%08x\n", u1StreamID+1, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_WP+(UINT32)u1StreamID*16, dReadDspCommDram(ADDR_RC2D_DRAM_MIXSOUND_CLIP1_WP+(UINT32)u1StreamID*16)); //TEST
        Printf("ADDR_RC2D_DRAM_MIXSOUND_CLIP%d_SA(%x): 0x%08x\n", u1StreamID+1, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_SA+(UINT32)u1StreamID*16, dReadDspCommDram(ADDR_RC2D_DRAM_MIXSOUND_CLIP1_SA+(UINT32)u1StreamID*16)); //TEST
        Printf("ADDR_RC2D_DRAM_MIXSOUND_CLIP%d_EA(%x): 0x%08x\n", u1StreamID+1, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_EA+(UINT32)u1StreamID*16, dReadDspCommDram(ADDR_RC2D_DRAM_MIXSOUND_CLIP1_EA+(UINT32)u1StreamID*16)); //TESt
        Printf("ADDR_RC2D_DRAM_MIXSOUND_CLIP%d_GAIN(%x): 0x%08x\n", u1StreamID+1, ADDR_RC2D_DRAM_MIXSOUND_CLIP1_GAIN+(UINT32)u1StreamID*16, dReadDspCommDram(ADDR_RC2D_DRAM_MIXSOUND_CLIP1_GAIN+(UINT32)u1StreamID*16)); //TEST
    }
    else
    {
        Printf("u1StreamID must be less than %d\n", MAX_AUD_MIXSND_STREAM_NUM);
    }
}

#ifdef ALSA_MIXSND_PATH
extern UINT32 u4GetMixSndRingFifoRP(void);
#endif
UINT32 u4GetMixSndReadPtr(void)
{
#ifdef ALSA_MIXSND_PATH
    return u4GetMixSndRingFifoRP();
#else
    return _AUD_DspGetMixSndReadPtr(0);
#endif
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(u4GetMixSndReadPtr);
#endif

#ifdef ALSA_MIXSND_PATH
extern void vSetMixSndRingFifoWP(UINT32 u4WritePtr);
#endif
void vSetMixSndWritePtr(UINT32 u4WritePtr)
{
#ifdef ALSA_MIXSND_PATH
    vSetMixSndRingFifoWP(u4WritePtr);
#else
    _AUD_DspSetMixSndWritePtr(0, u4WritePtr);
#endif
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(vSetMixSndWritePtr);
#endif

UINT32 u4GetMixSndFIFOStart(void)
{
#ifdef ALSA_MIXSND_PATH
    return u4GetMixSoundStartByteAddr3();
#else
    UINT32 u4SA;
    u4SA = (((UINT32)_i1DspMemBlock)<<24) + dReadDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_SA + (UINT32)16*0);
    return u4SA;
#endif    
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(u4GetMixSndFIFOStart);
#endif

UINT32 u4GetMixSndFIFOEnd(void)
{
#ifdef ALSA_MIXSND_PATH
    return u4GetMixSoundStartByteAddr3() + u4GetMixSoundBufSize3();
#else
    UINT32 u4EA;
    u4EA = (((UINT32)_i1DspMemBlock)<<24) + dReadDspCommDram (ADDR_RC2D_DRAM_MIXSOUND_CLIP1_EA + (UINT32)16*0);
    return u4EA;
#endif    
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(u4GetMixSndFIFOEnd);
#endif
#endif

UINT32 u4Virtual(UINT32 u4Addr)
{
    return VIRTUAL(u4Addr);
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(u4Virtual);
#endif

#include "dsp_data.h"
#include "../../fbm/fbm_pool_config.h"
extern UINT32 u4ReadDspSram(UINT32 u4Addr);
extern void AudShowAsrcBufferStatus(UINT8 u1ChMsk,BOOL fgUpdateRWP);

extern UINT32 u4GetMixSoundStartByteAddr(void);
extern UINT32 u4GetMixSoundBufSize(void);
extern UINT32 u4GetMixSoundStartByteAddr2(void);
extern UINT32 u4GetMixSoundBufSize2(void);
#ifdef ALSA_MIXSND_PATH
extern UINT32 u4GetMixSoundStartByteAddr3(void);
extern UINT32 u4GetMixSoundBufSize3(void);
#endif

void AUD_QueryDspMemory(UINT8 u1MemSpace)
{
    if (u1MemSpace == 0)
    {
        Printf("  DSP_SHARED_INFO: %x\n", DSP_SHARED_INF0_SZ);
        Printf("  DSP_CODE_SZ: %x\n", DSP_CODE_SZ);
        Printf("  DSP_DATA_SZ: %x\n", DSP_DATA_SZ);
        Printf("  DSP_AFIFO_SZ: %x\n", DSP_AFIFO_SZ);
        Printf("  DSP_AFIFO_SZ_DEC2: %x\n", DSP_AFIFO_SZ_DEC2);
        Printf("  DSP_AFIFO_SZ_DEC3: %x\n", DSP_AFIFO_SZ_DEC3);
        Printf("  DSP_AFIFO_SZ_DEC4: %x\n", DSP_AFIFO_SZ_DEC4);
        Printf("  DSP_AFIFO_HDMI_PARSER_SZ: %x\n", DSP_AFIFO_HDMI_PARSER_SZ);
        Printf("+ DSP_MIXSNDBUF_ALL_SZ: %x\n", DSP_MIXSNDBUF_ALL_SZ);
        Printf("================================================\n");
        Printf("  %x\n", FBM_DSP_SIZE);
    }
    
    if (u1MemSpace == 1)
    {
        Printf("[DSP Code Space]\n");
        Printf("  ROM : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[0]<<8), VIRTUAL(_u4DspICacheAddr[0]<<8), DSP_ROM_SZ);
        Printf("  POS : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[1]<<8), VIRTUAL(_u4DspICacheAddr[1]<<8), DSP_POS_RAM_SZ);
        Printf("  CRI : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[2]<<8), VIRTUAL(_u4DspICacheAddr[2]<<8), DSP_CTR_RAM_SZ);
        Printf("  COM : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[3]<<8), VIRTUAL(_u4DspICacheAddr[3]<<8), DSP_CMM_RAM_SZ);
        Printf("  DEC4: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[4]<<8), VIRTUAL(_u4DspICacheAddr[4]<<8), DSP_DEC4_RAM_SZ);
        Printf("  DEC1: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[5]<<8), VIRTUAL(_u4DspICacheAddr[5]<<8), DSP_DEC1_RAM_SZ);
        Printf("  DEC3: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[6]<<8), VIRTUAL(_u4DspICacheAddr[6]<<8), DSP_DEC3_RAM_SZ);
        Printf("  DEC2: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspICacheAddr[7]<<8), VIRTUAL(_u4DspICacheAddr[7]<<8), DSP_DEC2_RAM_SZ);
    }

    if (u1MemSpace == 2) 
    {
        Printf("[DSP Normal Space]\n");
        Printf("  DEC1: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[0]), VIRTUAL(_u4DspDramBuf[0]), DSP_DAT_DEC1_SZ);
        Printf("  DEC2: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[1]), VIRTUAL(_u4DspDramBuf[1]), DSP_DAT_DEC2_SZ);
        Printf("  DEC3: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[2]), VIRTUAL(_u4DspDramBuf[2]), DSP_DAT_DEC3_SZ);
        Printf("  COMM: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[3]), VIRTUAL(_u4DspDramBuf[3]), DSP_DAT_CMM_NORMAL_SZ);
        Printf("  DEC4: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[4]), VIRTUAL(_u4DspDramBuf[4]), DSP_DAT_DEC4_SZ);
        Printf("  MIX : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspDramBuf[5]), VIRTUAL(_u4DspDramBuf[5]), DSP_MIXSNDBUF_ALL_SZ);
    }

    if (u1MemSpace == 3) 
    {
        Printf("[DSP Compact Space]\n");
        Printf("  DEC1: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[0]), VIRTUAL(_u4DspCmptBuf[0]), DSP_DAT_DEC1_SZ);
        Printf("  DEC2: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[1]), VIRTUAL(_u4DspCmptBuf[1]), DSP_DAT_DEC2_SZ);
        Printf("  DEC3: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[2]), VIRTUAL(_u4DspCmptBuf[2]), DSP_DAT_DEC3_SZ);
        Printf("  COMM: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[3]), VIRTUAL(_u4DspCmptBuf[3]), DSP_DAT_CMM_SZ);
        Printf("  DEC4: Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[4]), VIRTUAL(_u4DspCmptBuf[4]), DSP_DAT_DEC4_SZ);
        Printf("  MIX : Addr: (P)0x%08x/(V)0x%08x Size: 0x%04x\n", (_u4DspCmptBuf[5]), VIRTUAL(_u4DspCmptBuf[5]), DSP_MIXSNDBUF_ALL_SZ);
    }

    if (u1MemSpace == 4) 
    {
        Printf("[AFIFO Space]\n");
        Printf("  AFIFO1: Start: (P)0x%08x/(V)0x%08x End: (P)0x%08x/(V)0x%08x\n", _u4AFIFO[0][0], VIRTUAL(_u4AFIFO[0][0]), _u4AFIFO[0][1], VIRTUAL(_u4AFIFO[0][1]));
        Printf("  AFIFO2: Start: (P)0x%08x/(V)0x%08x End: (P)0x%08x/(V)0x%08x\n", _u4AFIFO[1][0], VIRTUAL(_u4AFIFO[1][0]), _u4AFIFO[1][1], VIRTUAL(_u4AFIFO[1][1]));
        Printf("  AFIFO3: Start: (P)0x%08x/(V)0x%08x End: (P)0x%08x/(V)0x%08x\n", _u4AFIFO[2][0], VIRTUAL(_u4AFIFO[2][0]), _u4AFIFO[2][1], VIRTUAL(_u4AFIFO[2][1]));
        Printf("  AFIFO4: Start: (P)0x%08x/(V)0x%08x End: (P)0x%08x/(V)0x%08x\n", _u4AFIFO[3][0], VIRTUAL(_u4AFIFO[3][0]), _u4AFIFO[3][1], VIRTUAL(_u4AFIFO[3][1]));
    }

    if (u1MemSpace == 5) 
    {
        Printf("[Internal Buffer Space]\n");
        Printf("  IntBuf1: Start (P)0x%08x/(V)0x%08x Size: 0x%08x\n", (_u4DspCmptBuf[3] + DRAM_INTERNAL_BUF*4), VIRTUAL(_u4DspCmptBuf[3] + DRAM_INTERNAL_BUF*4), DRAM_INTERNAL_BUF_SIZE*4);
        Printf("  IntBuf2: Start (P)0x%08x/(V)0x%08x Size: 0x%08x\n", (_u4DspCmptBuf[3] + DRAM_INTERNAL_BUF2*4), VIRTUAL(_u4DspCmptBuf[3] + DRAM_INTERNAL_BUF*4), DRAM_INTERNAL_BUF2_SIZE*4);
        Printf("  IntBuf3: Start (P)0x%08x/(V)0x%08x Size: 0x%08x\n", (_u4DspCmptBuf[3] + DRAM_INTERNAL_BUF3*4), VIRTUAL(_u4DspCmptBuf[3] + DRAM_INTERNAL_BUF*4), DRAM_INTERNAL_BUF3_SIZE*4);
        Printf("  IntBuf4: Start (P)0x%08x/(V)0x%08x Size: 0x%08x\n", (_u4DspCmptBuf[3] + DRAM_INTERNAL_BUF4*4), VIRTUAL(_u4DspCmptBuf[3] + DRAM_INTERNAL_BUF*4), DRAM_INTERNAL_BUF4_SIZE*4);
        #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
        Printf("  IntBuf5: Start (P)0x%08x/(V)0x%08x Size: 0x%08x\n", (_u4DspCmptBuf[3] + DRAM_INTERNAL_BUF5*4), VIRTUAL(_u4DspCmptBuf[3] + DRAM_INTERNAL_BUF*4), DRAM_INTERNAL_BUF5_SIZE*4);
        Printf("  IntBuf6: Start (P)0x%08x/(V)0x%08x Size: 0x%08x\n", (_u4DspCmptBuf[3] + DRAM_INTERNAL_BUF6*4), VIRTUAL(_u4DspCmptBuf[3] + DRAM_INTERNAL_BUF*4), DRAM_INTERNAL_BUF6_SIZE*4);
        #endif
    }

    if (u1MemSpace == 6) 
    {    
        Printf("[ASRC Space]\n");
        AudShowAsrcBufferStatus(0x1f,FALSE);
    }
    
    if (u1MemSpace == 7) 
    {
        int i;
        UINT32 u4Addr;
        UINT32 u4AoutCmptPage;

        Printf("[AOUT Space]\n");

        u4AoutCmptPage = ((u4ReadDspSram(REG_DSP_AOUT_CTRL)>>12)&0x7);
        Printf("  AOUT1 Cmpt Page: %d Ch Num: %d\n", u4AoutCmptPage, (u4ReadDspSram(REG_DSP_CH_NUM)>>8));
        Printf("        CH CFG: L: %x R: %x SL: %x SR: %x C: %x LFE: %x CH7: %x CH8: %x CH9: %x CH10: %x\n",
                  ((u4ReadDspSram(REG_DSP_CH_CFG_0)>>8)&0xf),
                  ((u4ReadDspSram(REG_DSP_CH_CFG_0)>>12)&0xf),
                  ((u4ReadDspSram(REG_DSP_CH_CFG_1)>>8)&0xf),
                  ((u4ReadDspSram(REG_DSP_CH_CFG_1)>>12)&0xf),
                  ((u4ReadDspSram(REG_DSP_CH_CFG_0)>>16)&0xf),
                  ((u4ReadDspSram(REG_DSP_CH_CFG_1)>>16)&0xf),
                  ((u4ReadDspSram(REG_DSP_CH_CFG_0)>>20)&0xf),
                  ((u4ReadDspSram(REG_DSP_CH_CFG_1)>>20)&0xf),
                  ((u4ReadDspSram(REG_DSP_CH_CFG_2)>>8)&0xf),
                  ((u4ReadDspSram(REG_DSP_CH_CFG_2)>>12)&0xf));
        for (i=0;i<11;i++)
        {
            u4Addr = _u4DspCmptBuf[u4AoutCmptPage] + (u4ReadDspSram(REG_DSP_CHL_1_ADDR+i)<<2);
            Printf("        CH%d addr: (P)0x%08x/(V)0x%08x\n", i+1, u4Addr, VIRTUAL(u4Addr));
        }

        u4AoutCmptPage = ((u4ReadDspSram(REG_DSP_AOUT_CTRL_2)>>12)&0x7);
        Printf("  AOUT2 Cmpt Page: %d Ch Num: %d\n", u4AoutCmptPage, (u4ReadDspSram(REG_DSP_CH_NUM_2)>>8));
        for (i=0;i<3;i++)
        {
            u4Addr = _u4DspCmptBuf[u4AoutCmptPage] + (u4ReadDspSram(REG_DSP_CHL_1_ADDR_2+i)<<2);
            Printf("        CH%d addr: (P)0x%08x/(V)0x%08x\n", i+1, u4Addr, VIRTUAL(u4Addr));
        }        
    }

    if (u1MemSpace == 8) 
    {
        Printf("[Upload Buffer]\n");
        Printf("   SA: (P)0x%08x/(V)0x%08x Size: %x\n", 
                u4GetUploadFIFOStart(), 
                VIRTUAL(u4GetUploadFIFOStart()),
                u4GetUploadFIFOEnd() - u4GetUploadFIFOStart());
    }

    if (u1MemSpace == 9)
    {
        Printf("[MixSnd Buffer]\n");
        Printf("  MIXSND1 SA: (P)0x%08x/(V)0x%08x Size: %x\n", 
                u4GetMixSoundStartByteAddr(), 
                VIRTUAL(u4GetMixSoundStartByteAddr()),
                u4GetMixSoundBufSize());
        Printf("  MIXSND2 SA: (P)0x%08x/(V)0x%08x Size: %x\n", 
                u4GetMixSoundStartByteAddr2(), 
                VIRTUAL(u4GetMixSoundStartByteAddr2()),
                u4GetMixSoundBufSize2());
        Printf("  MIXSND3 SA: (P)0x%08x/(V)0x%08x Size: %x\n", 
                u4GetMixSoundStartByteAddr3(), 
                VIRTUAL(u4GetMixSoundStartByteAddr3()),
                u4GetMixSoundBufSize3());
    }
}

void AUD_DumpDspMemory(UINT8 u1Option, UINT32 u4Size)
{
    UINT32 i;
    UINT32 u4SA;

    switch (u1Option)
    {
        case 0:
            Printf("[Dump AFIFO1]\n");
            u4SA = VIRTUAL(_u4AFIFO[0][0]);
            break;
        case 1:
            Printf("[Dump AFIFO2]\n");
            u4SA = VIRTUAL(_u4AFIFO[1][0]);
            break;
        case 2:
            Printf("[Dump AFIFO3]\n");
            u4SA = VIRTUAL(_u4AFIFO[2][0]);
            break;
        case 3:
            Printf("[Dump AFIFO4]\n");
            u4SA = VIRTUAL(_u4AFIFO[3][0]);
            break;
        case 4:
            Printf("[Dump UPLOAd]\n");
            u4SA = VIRTUAL(u4GetUploadFIFOStart());
            break;
        case 5:
            Printf("[Dump ALSA]\n");
            u4SA = VIRTUAL(u4GetMixSoundStartByteAddr3());
            break;
        case 6:
            Printf("[Dump MIXCLIP]\n");
            u4SA = VIRTUAL(u4GetMixSoundStartByteAddr3()-0x4000);
            break;

        default:
            return;
}

    for (i=0;i<u4Size;i++)
    {
        if ((i&0xf)==0)
        {
            Printf("\n%08xh: ", (u4SA+i));
        }
        Printf(" %02x", *((UINT8*)(u4SA+i)));
    }    
}
//DANIEL_HACK
void vDspGetParameterMP3(UINT32* u4FreqIdx, UINT32* u4DspInputChCfg, UINT32* u4Bitrate)
{
    // get audio information
    *u4FreqIdx = wReadDspWORD(ADDR_D2RC_RISC_INFO_SAMPLING_RATE);
    *u4DspInputChCfg = wReadDspWORD(ADDR_D2RC_RISC_INFO_INPUT_CH_CFG) & 0x03F;
    *u4Bitrate = dReadDspCommDram(ADDR_D2RC_RISC_INFO_BIT_RATE);
}
//DANIEL_HACK

void vDSPLockReadSramSema(void)
{
	VERIFY(x_sema_lock(_hRdDspSramSema, X_SEMA_OPTION_WAIT)== OSR_OK);
}

void vDSPUnlockReadSramSema(void)
{
	VERIFY(x_sema_unlock(_hRdDspSramSema)== OSR_OK);
}

