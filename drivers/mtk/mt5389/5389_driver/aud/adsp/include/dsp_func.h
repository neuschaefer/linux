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
 * $Author: kai.wang $
 * $Date: 2012/06/28 $
 * $RCSfile: dsp_func.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file dsp_func.h
 *  Brief of file dsp_func.h.
 *  Details of file dsp_func.h (optional).
 */

#ifdef AUD_EXTERN_FILE
#error "Please don't include this header file in release C file"
#endif

#ifndef DSP_FUNC_H
#define DSP_FUNC_H

/*
 * Dsp Code Internal use only
 * This files only contains DSP Internal use function
 * Do not share the file with outside modules
 * outside modules should use DspIntf.h
 */
#ifdef __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

#ifndef _DSP_INIT_C
#define _DSP_INIT_C

extern void vDspStateInit (void);
extern void vDspReset(UINT8 u1DecId);
extern void vDspInit (void);
extern void vDspShareInfoInit (void);
extern void vOplFuncInit(void);
extern void vDspPlayCommonInit (void);
extern void vDspPlayCommonInitDec2 (void);
#ifdef CC_MT5391_AUD_3_DECODER
extern void vDspPlayCommonInitDec3 (void);
#endif
#ifdef CC_AUD_4_DECODER_SUPPORT
extern void vDspPlayCommonInitDec4 (void);
#endif

#endif

#ifndef _DSP_STATE_C
#define _DSP_STATE_C

extern BOOL fgDspInitState (void); // Light added
extern void vDspState (UINT32 u4Msg);
extern void vDspPowerOn (void);
extern void vDspPowerOff (void);

#endif


#ifndef _DSP_INT_C
#define _DSP_INT_C

extern void vDspSetTable(UINT8 u1DecId, UINT32 u4FreqIdx);
extern void vDspIntSvc(void);
extern void vDspIntSvc_Data (void); //ADSP Data Queue.
extern void vDspUpdatePostEffect(void);

#endif


#ifndef _DSP_UOP_C
#define _DSP_UOP_C

extern void vDspUopSvc(UINT32 u4DspUop);
extern void vEQBandGainFlush(UINT8 u1DecId);
extern void vEQ2SbassBandGainFlush(UINT8 u1DecId);

#endif

#ifndef _DSP_RGCTL_C
#define _DSP_RGCTL_C

extern BOOL fgDspBusy(void);
extern void vDspSendInt(void);

#endif                          //_DSP_RGCTL_C

#ifndef _DSP_INTF_C
#define _DSP_INTF_C

extern void vDspAdacFmt(UINT8 u1DecId, UINT32 u4ClkFmt);

#endif                          // _DSP_INTF_C

#ifndef _DSP_OP_C
#define _DSP_OP_C

extern void vDspLoadRamCode(UINT32 u4Type);
extern void vDspCmd (UINT32 u4Cmd);
extern UINT32 u4DspGetSampleRate(UINT8 u1DecId);
extern void vDspBlockPtrReset(UINT8 u1DecId);
extern void vSetSpeakerConfig(UINT8 u1DecId);
extern void vWriteDspSram(UINT32 u4Addr, UINT32 u4Value);
extern UINT32 u4ReadDspSram(UINT32 u4Addr);
extern void vDspSetFreq(UINT8 u1DecId, UINT32 u4FreqIdx, BOOL fgResetDAC);
extern void vSetIecConfig(UINT8 u1DecId, UINT32 u4FreqIdx);
extern void vTrnsltSpkCfg(UINT16 u2SpkCfg, SPEAKER_SETTING_T * rSpkCfg);
extern void vLoadDspDRAMTable(UINT32 u4Type);
extern void vDspDecDes(UINT8 u1DecId);
extern void DspIntOrSram(UINT32 u4Addr, UINT32 dwValue);
#ifdef ADSP_CHECK_RAM_CODE
// Check DSP data functions
extern BOOL fgCheckDspData(void);
#endif    
// The following two function should be moved to dspintf.c
extern INT32 i4DspSendPts (UINT8 u1DecId, UINT32 u4Pts, UINT32 u4ParPnt);
    
#endif                          // _DSP_OP_C

#ifndef _DSP_IRQ_C
#define _DSP_IRQ_C

extern UINT32 u4DspIRQSvc (void);

#endif

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif  //DSP_FUNC_H

