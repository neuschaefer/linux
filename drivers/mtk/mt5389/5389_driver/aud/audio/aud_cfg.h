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
 * $Date: 2012/07/17 $
 * $RCSfile: aud_cfg.h,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_cfg.h
 *  Brief of file aud_cfg.h.
 *  Details of file aud_cfg.h (optional).
 */

#ifndef _AUD_CFG_H_
#define _AUD_CFG_H_

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_typedef.h"
#include "aud_if.h"
#include "drvcust_if.h"
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT 
#include "feeder_if.h"
#endif

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

#define AUD_HW_LR_INVERSE_DECT
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define SPL_LINE                  0
#define SPL_SPDIF                 1

// SPDIF/line in configuration interface 0
#define AIN_SPDIFIN_RJ          (0)      // Right aligned with LRCK
#define AIN_SPDIFIN_LJ          (1)      // Left aligned with LRCK
#define AIN_SPDIFIN_I2S         (3)      // I2S interface
#define AIN_SPDIFIN_16BIT       (0x0F)   // 16 bits
#define AIN_SPDIFIN_18BIT       (0x11)   // 18 bits
#define AIN_SPDIFIN_20BIT       (0x13)   // 20 bits
#define AIN_SPDIFIN_24BIT       (0x17)   // 24 bits

// SPDIF/line in configuration interface 1
#define AIN_SPDIFIN_CYC16       (0)      // 16 cycles
#define AIN_SPDIFIN_CYC24       (1)      // 24 cycles
#define AIN_SPDIFIN_CYC32       (2)      // 32 cycles

#define DMX_FIFO_ALIGNMENT      (16)

#ifdef CC_MT5365
#define DSP_ROM_CFG             0x234
#define AIN_ROM_CFG             0x3
#endif

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

extern BOOL _AUD_DMX_SetAudFifo(UINT8 u1DecId, UINT32 u4FifoStr, UINT32 u4FifoEnd);
extern void _AUD_DMX_GetAudFifo(UINT8 u1DecId, UINT32 *pu4FifoStr, UINT32 *pu4FifoEnd);
extern void _AUD_DMX_GetAudPtr(UINT8 u1DecId, UINT32 *pu4ReadPtr, UINT32 *pu4WritePtr);
extern void _AUD_DMX_GetAudWrtPtr(UINT8 u1DecId, UINT32 *pu4WritePtr);
extern BOOL _AUD_DMX_UpdateWritePtr(UINT8 u1DecId, UINT32 u4WritePtr);
extern VOID _AUD_DMX_UpdateWPtr(UINT8 u1DecId, UINT32 u4Wp);
extern void _AUD_BitStrPtrSel(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom)	;

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
extern void AUD_AoutPADMux(AUD_CH_T ePAD_SDATA0, AUD_CH_T ePAD_SDATA1, AUD_CH_T ePAD_SDATA2,
                    AUD_CH_T ePAD_SDATA3, AUD_CH_T ePAD_SDATA4, AUD_CH_T ePAD_SDATA5, 
                    UINT8 PAD_MCLK0);
extern void AUD_AoutPADMuxQuery(void);

extern void AUD_HwInit(void);
extern void AUD_AinGetMode(BOOL *pfgDeteced, AUD_FMT_T *peMode);
extern void AUD_AinSetMode(UINT8 u1Mode);
extern void AUD_SetSampleFreq(UINT8 u1DecId, SAMPLE_FREQ_T eSmpFreq);
extern SAMPLE_FREQ_T AUD_GetSampleFreq(UINT8 u1DecId);
extern void AUD_AoutFormat(UINT8 u1DecId, DATA_FORMAT_T eDataFormat);
extern void AUD_AoutDacFs(UINT8 u1DecId, SAMPLE_FREQ_T eFs);
extern void AUD_AoutInvertData(UINT8 u1DecId, BOOL eDataFormat);
extern void AUD_AoutDataNum(UINT8 u1DecId, DAC_DATA_NUMBER_T eDataNum);
extern void AUD_AoutLRCycle(UINT8 u1DecId, LRCK_CYC_T eCycle);
extern void AUD_SpdifInPinSelect(BOOL fgSet);
extern void _AUD_SpdifInBypass(SPDIF_IN_PIN_NUM u1PIN);
extern void AUD_LineInCtrlEnable(UINT8 u1DecId, BOOL fgEnable);
extern void AUD_OutPadEnable(UINT8 u1DecId, BOOL fgEnable);
extern void AUD_AdcEnable(BOOL fgEnable);
extern void AUD_AoutDrvCur(UINT8 u1DecId, UINT8 u1MckDrvCur, UINT8 u1BckDrvCur ,UINT8 u1LRckDrvCur , UINT8 u1AOSDATADrvCur);
extern void AUD_AinEnable(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, BOOL fgEnable);
extern void AUD_AinInit(void);
extern void AUD_AinSrcSel(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eAudFmt);
extern void AUD_AVComponentSel(AV_COMPONENT_T eAVComp);
extern void AUD_StrInputEnable(UINT8 u1DecId, AUD_DEC_STREAM_FROM_T eStreamFrom, AUD_FMT_T eFmt,BOOL fgEnable, UINT32 u4ReceivePesCnt);
extern BOOL _AUD_PLL_ConfigDivider(UINT8 u1DecId, UINT8 K1, UINT8 K2);
extern void AUD_AinFormat(AUD_DEC_STREAM_FROM_T eStrSrc, DATA_FORMAT_T eDataFormat);
extern void AUD_AvMuxSel(AUD_INPUT_ID_T eInputId);
extern void AUD_AvMuxSelByAD(UINT8 eADNum);
extern void _AUD_SpdifInPinMuxEnable(void);
extern INT32 _AUD_GetAudFifo(UINT8 u1DecId, UINT32 * pu4FifoStart, UINT32 * pu4FifoEnd);
extern AUD_INPUT_ID_T AUD_GetAvInputSrcId(void);
extern AUD_DEC_STREAM_FROM_T AUD_GetStrSrc(UINT8 u1DedId);
extern void AUD_ClkCfgDualDecMode(BOOL fgEnable);

//extern BOOL AUD_GetAinCfg(AUD_DEC_STREAM_FROM_T eStreamFrom, AIN_CFG_T* prAinCfg);
extern void AUD_AinCfg(AUD_DEC_STREAM_FROM_T eStrSrc, const AIN_CFG_T* prAinCfg);
extern void AUD_AoutCfg(UINT8 u1DecId, const AOUT_CFG_T* prAoutCfg);

extern void AUD_SPDIF_Output_HDMI_Rx(BOOL fgEnable);
#ifdef CC_AUD_HDMI_SPDIF_CFG
extern void AUD_SPDIF_Output_Mute(BOOL fgMute);
#endif
extern void AUD_GPIOOutput(UINT32 u4GPIONum, INT32 i4Polarity);
extern void AUD_GPIOEnable(UINT32 u4GPIONum, INT32 i4Polarity);

#if (defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389))
extern void AudAoutGainCtrl(UINT8 u1DecId, UINT8 CHID, UINT16 Gain);
#endif
#ifdef CC_AUD_HDMI_PARSER_2_0_SUPPORT 
extern INT32 AUD_FeederTransmitHandler(FeederSourceType eSrcType, UINT32 u4QrySize, UINT64 u8Offset,FEEDER_TRANSMIT_COND *pResult, UINT32 u4ReqId);
extern UINT32 AUD_GetLineinWP(void);
extern void AUD_DrvSetFgHDMIParserthread(UINT8 u1DecId,BOOL fgHDMIParserThread);
#endif

#ifdef CC_FAST_INIT
extern void AUD_Spdif_Power_Down(void);
#endif
extern void AudSPDIFInBypass(BOOL fgBypass);

#endif  // _AUD_CFG_H_

