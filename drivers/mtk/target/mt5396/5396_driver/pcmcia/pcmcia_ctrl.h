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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: pcmcia_ctrl.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/
 
#ifndef PCMCIA_CTRL_H
#define PCMCIA_CTRL_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "x_gpio.h"
LINT_EXT_HEADER_END

#include "pcmcia_debug.h"
#include "x_tuner.h"
#include "dmx_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define ATTRIBUTE_MEM_END_ADDR      0x2000
#define COMMON_MEM_END_ADDR         0x2000

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef enum
{
    PCMCIA_CMD_DONE_POLLING = 0, 
    PCMCIA_CMD_DONE_INTERRUPT
}PCMCIA_CMD_DONE_METHOD;

typedef struct
{
    PCMCIA_CMD_DONE_METHOD u1CmdDoneMethod;
} PCMCIA_DRIVER_CONFIG_T;

#if 0
typedef void (* PFN_GPIO_CALLBACK)(INT32 i4Gpio, BOOL fgStatus);

typedef enum {
    GPIO_TYPE_NONE = 0,
    GPIO_TYPE_INTR_RISE = 1,        
    GPIO_TYPE_INTR_FALL = 2,
    GPIO_TYPE_INTR_BOTH = 3,
    GPIO_TYPE_INTR_LEVEL_HIGH = 4,    
    GPIO_TYPE_INTR_LEVEL_LOW = 8,
#if GPIO_THREAD_ENABLE
    GPIO_TYPE_POLL_FALL = 9,
    GPIO_TYPE_POLL_RISE = 10,
    GPIO_TYPE_POLL_BOTH = 11,
#endif /* GPIO_THREAD_ENABLE */
    GPIO_TYPE_LAST = 12
} GPIO_TYPE;
#endif


extern PCMCIA_DRIVER_CONFIG_T tPcmciaDriverConfig;

EXTERN void     PCMCIA_PinMux(void);
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
#ifndef CC_MTK_LOADER
EXTERN void     PCMCIA_CDPinHandler(void);
EXTERN INT32    PCMCIA_Initialize(void);
EXTERN void     PCMCIA_CamPwrCtrl(BOOL fgManualControl);
EXTERN void     PCMCIA_CamPwrONOFF(BOOL fgPwrOnOff);
EXTERN void     PCMCIA_ThdNoAsrt(BOOL bNoAssert);
EXTERN void     PCMCIA_StopThd(void);
EXTERN void     PCMCIA_ResetPcmciaHW(void);
EXTERN BOOL     PCMCIA_GetCmdStatusDone(void);
EXTERN void     PCMCIA_SetCmdStatusDone(void);
EXTERN INT32    PCMCIA_PcmciaToPodCi(BOOL fgPcmciaToPod);
EXTERN INT32    PCMCIA_EnableTS(BOOL fgPcmciaEnableTS);
EXTERN INT32    PCMCIA_DetectCableCard(void);
EXTERN BOOL     PCMCIA_DetectPodCiActive(void);
EXTERN void     PCMCIA_ResetCableCard(void);
EXTERN INT32    PCMCIA_ResetCableCardOnOff(BOOL b_set);
EXTERN void     PCMCIA_OnOffCableCard(BOOL OnOff);
EXTERN void     PCMCIA_SetExtDemodOnOff(BOOL fgOnOff);
EXTERN void     PCMCIA_SetParallelTsOnOff(BOOL fgOnOff);
EXTERN void     PCMCIA_SetInputTs(PCMCIA_TS_TYPE eInputTs);
EXTERN void     PCMCIA_ExternalICSetTsClkRate(UINT32 u4ClkRate);
EXTERN void     PCMCIA_RegISR(UINT16, x_os_isr_fct, x_os_isr_fct*);
EXTERN void     PCMCIA_RegCAMStatusCbk(x_pcmcia_nfy_fct pfn);
EXTERN void     PCMCIA_NfyCamStatusCbk(PCMCIA_NFY_EVENT eNfyEvent);
EXTERN void     PCMCIA_AutoConfigCamPwrSwitch(void);
EXTERN void     PCMCIA_SetDriving(UINT32 u4Driving);
EXTERN void     PCMCIA_Suspend(void);
#endif
EXTERN void     PCMCIA_SetCiTsClock(UINT32 u4Clock);
EXTERN void     PCMCIA_SetCiSysClock(UINT32 u4Clock);

EXTERN INT32    PCMCIA_GPIO_Init(void);
EXTERN INT32    PCMCIA_GPIO_Range_Check(INT32 i4GpioNum);
EXTERN INT32    PCMCIA_MT8295_GPIO_Range_Check(INT32 i4GpioNum);
EXTERN INT32    PCMCIA_MT5135_GPIO_Range_Check(INT32 i4GpioNum);
EXTERN INT32    PCMCIA_GPIO_Enable(INT32 i4GpioNum, INT32 * pi4Polarity);
EXTERN INT32    PCMCIA_GPIO_Input(INT32 i4GpioNum);
EXTERN INT32    PCMCIA_GPIO_Output(INT32 i4GpioNum, INT32 * pi4DriveHighLow);
EXTERN INT32    PCMCIA_GPIO_Intrq(INT32 i4Gpio, INT32 *pfgSet);
EXTERN INT32    PCMCIA_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
EXTERN INT32    PCMCIA_GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise);
#ifdef CC_FAST_INIT
EXTERN INT32    PCMCIA_GPIO_Suspend(void);
EXTERN INT32    PCMCIA_GPIO_Resume(void);
#endif

#ifndef CC_MTK_LOADER
EXTERN void     _PCMCIA_Set2Bit(BOOL fg2Bit);
EXTERN BOOL     _PCMCIA_Get2Bit(void);
EXTERN void     PCMCIA_SetTsRouting(BOOL fgTsGoThroughCam, TS_FMT_T *prTsFmt);
#endif // CC_MTK_LOADER
#endif //PCMCIA_CTRL_H
