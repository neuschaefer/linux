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
 * $RCSfile: pcmcia_if.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/
 
#ifndef PCMCIA_IF_H
#define PCMCIA_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "x_gpio.h"
#include "x_pcmcia.h"
#include "x_os.h"
LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// PCMCIA Global Configuration Definitions
//-----------------------------------------------------------------------------
#if defined(CC_INTERNAL_CI) || defined(CC_EXTERNAL_CI)  
    #define PCMCIA_TYPE_CI
#elif defined(CC_INTERNAL_POD) || defined(CC_EXTERNAL_POD)
    #define PCMCIA_TYPE_POD
#endif

#if defined(CC_INTERNAL_CI) || defined(CC_INTERNAL_POD)
    #define PCMCIA_TYPE_INTERNAL
#elif defined(CC_EXTERNAL_CI) || defined(CC_EXTERNAL_POD)
    #define PCMCIA_TYPE_EXTERNAL
#endif

#if defined(PCMCIA_TYPE_INTERNAL) || defined(PCMCIA_TYPE_EXTERNAL)
    #define PCMCIA_ENABLED
#else
    #define PCMCIA_DISABLED
#endif

#if defined(CC_INTERNAL_CI) && defined(CC_EXTERNAL_CI)
#error "Both CC_INTERNAL_CI & CC_EXTERNAL_CI are defined !"
#endif

#if defined(CC_INTERNAL_POD) && defined(CC_EXTERNAL_POD)
#error "Both CC_INTERNAL_POD & CC_EXTERNAL_POD are defined !"
#endif


#define MAX_CIS_MAP_SIZE    256 /* Max CIS map buffer size */

//#define CC_FAST_INIT 1

/// Return values
#define POD_OK                          ((INT32)     0)
#define POD_NOT_INIT                    ((INT32)    -1)
#define POD_ALREADY_INIT                ((INT32)    -2)
#define POD_INIT_FAILED                 ((INT32)    -3)
#define POD_INV_ARG                     ((INT32)    -4)
#define POD_MODULE_NOT_INSERTED         ((INT32)    -5)
#define POD_CIS_ERROR                   ((INT32)    -6)
#define POD_REGISTER_ISR_FAILED         ((INT32)    -7)
#define POD_CHANNEL_RESET_FAILED        ((INT32)    -8)
#define POD_BUF_SIZE_NGO_FAILED         ((INT32)    -9)
#define POD_DA_FR_INT_ENABLE_FAILED     ((INT32)   -10)
#define POD_INVALID_ACCESS_MODE         ((INT32)   -11)
#define POD_EXCEED_MAX_BUF_SIZE         ((INT32)   -12)
#define POD_TIME_OUT                    ((INT32)   -13)
#define POD_CMD_ERROR                   ((INT32)   -14)
#define POD_READ_ERROR                  ((INT32)   -15)
#define POD_WRITE_ERROR                 ((INT32)   -16)
#define POD_DATA_AVAILABLE              ((INT32)   -17)
#define POD_POWER_CTRL_ERROR            ((INT32)   -18)
#define POD_REG_TEST_ERROR              ((INT32)   -19)
#define POD_CLI_ERROR                   ((INT32)   -20)
#define POD_DMA_BUF_NOT_ALLOC			((INT32)   -21)
#define POD_UNKNOWN_COND                ((INT32)   -22)

// POD state
#define POD_INSERTED                ((UINT32)   (1 << 0))
#define POD_FREE                    ((UINT32)   (1 << 1))
#define POD_DATA_AVIL               ((UINT32)   (1 << 2))
#define POD_FREE_EXT                ((UINT32)   (1 << 3))
#define POD_DATA_AVIL_EXT           ((UINT32)   (1 << 4))
#define POD_VS1                     ((UINT32)   (1 << 5)) /* Added for DK PODware */
#define POD_VS2                     ((UINT32)   (1 << 6)) /* Added for DK PODware */
#define POD_READY_IREQ              ((UINT32)   (1 << 7)) /* Added for DK PODware */
#define POD_OVERCURRENT             ((UINT32)   (1 << 8)) /* Added for DK PODware */

// Definitions of POD status register
#define POD_STS_RE                  ((UINT32)   (1 << 0))
#define POD_STS_WE                  ((UINT32)   (1 << 1))
#define POD_STS_IIR                 ((UINT32)   (1 << 4))
#define POD_STS_FR                  ((UINT32)   (1 << 6))
#define POD_STS_DA                  ((UINT32)   (1 << 7))


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define MIN_VALUE(a, b)                 ((a < b) ? a : b)


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef void (*pcmcia_cam_connectivity_nfy)(BOOL  fgCamConnected);

typedef enum
{
    PCMCIA_ROUTE_TYPE_IN_INTERNAL_DEMOD,
    PCMCIA_ROUTE_TYPE_IN_PVR,
    PCMCIA_ROUTE_TYPE_IN_DVBS2_SERIAL,
    PCMCIA_ROUTE_TYPE_IN_DVBS2_PARALLEL,
}PCMCIA_ROUTE_TYPE;

typedef enum
{
    PCMCIA_TS_EXTERNAL_DEMOD_P,
    PCMCIA_TS_EXTERNAL_DEMOD_S1,
    PCMCIA_TS_EXTERNAL_DEMOD_S2,
    PCMCIA_TS_INTERNAL_DEMOD_P,
    PCMCIA_TS_INTERNAL_DEMOD_S,

    // not used, just avoid build error
    PCMCIA_TS_TYPE_DVB_T,
    PCMCIA_TS_TYPE_DVB_C,
    PCMCIA_TS_TYPE_DVB_S,
    PCMCIA_TS_TYPE_DTMB,
    
    PCMCIA_TS_TYPE_UNKNOWN
}PCMCIA_TS_TYPE;

typedef enum
{
    CI_CAM_WORKAROUND1,  //CAM019 can not receive 36Mhz CLK.
    CI_CAM_WORKAROUND2,  
    CI_CAM_WORKAROUND3,  //SMARCAM has mosaic
}CI_CAM_WORKAROUND;

typedef enum
{
    PCMCIA_ISR_NONE,
    PCMCIA_ISR_SIF,     // used only for external CI
    PCMCIA_ISR_IREQ     // used for both internal/external CI
}PCMCIA_ISR;

typedef enum
{
    MT8295_VERSION_NONE, 
    MT8295_VERSION_AE, 
    MT8295_VERSION_BNE, 
    MT5135_VERSION_AE
}PCMCIA_EXTERNAL_IC_VERSION;

typedef enum
{
    POD_SDA = 0,
    POD_DMA
} POD_IO_STATE_T;

/** POD operation mode
 */
typedef enum
{
    PCMCIA_IF_TYPE_NFI,
    PCMCIA_IF_TYPE_SPI
} PCMCIA_IF_TYPE_T;

/** POD channel selection
 */
typedef enum
{
    EXT_CHANNEL         = (1 << 26),
    DATA_CHANNEL        = ~EXT_CHANNEL
} POD_CHANNEL_T;

/// CIS information structure
typedef struct
{
    BOOL      fgPodModuleType;
    BOOL      fgVpp;
    BOOL      fgIrq;
    UINT8     u1ModuleInterfaceType;
    UINT8     u1CorValue;
    UINT16    u2CorAddr;
} CIS_INFO_T;

typedef enum
{
    PCMCIA_CARD_TYPE_UNKNOWN = 0,
    PCMCIA_CARD_TYPE_CI,
    PCMCIA_CARD_TYPE_POD
}PCMCIA_CARD_TYPE;

typedef enum
{
    PCMCIA_CHIP_TYPE_UNKNOWN = 0,
    PCMCIA_CHIP_TYPE_INTERNAL,
    PCMCIA_CHIP_TYPE_EXTERNAL
}PCMCIA_CHIP_TYPE;

typedef enum
{
    PCMCIA_CAM_CONNECT = 0,
    PCMCIA_CAM_DISCONNECT,
    PCMCIA_OVER_CURRENT
}PCMCIA_NFY_EVENT;

typedef enum
{
    PCMCIA_MAINCHIP_UNKNOWN = 0,
    PCMCIA_MAINCHIP_MT5365,
    PCMCIA_MAINCHIP_MT5366,
} PCMCIA_MAINCHIP;

typedef VOID (*x_pcmcia_nfy_fct)(PCMCIA_NFY_EVENT eNfyEventCode);

EXTERN PCMCIA_CARD_TYPE ePcmciaCardType;
EXTERN PCMCIA_CHIP_TYPE ePcmciaChipType;


#ifndef CC_MTK_LOADER
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------
EXTERN void     PCMCIA_SetTsPath(BOOL fgExternalDemod, BOOL fgThroughCard);
EXTERN INT32    PCMCIA_Init(void);
EXTERN INT32    PCMCIA_LowLevelInit(void);
EXTERN UINT32   PCMCIA_RawRead32(UINT32 offset);
EXTERN UINT32   PCMCIA_RawWrite32(UINT32 offset, UINT32 value);
EXTERN INT32    PCMCIA_Read(POD_IO_STATE_T eIoState, UINT8* pu1Data, UINT16* pu2DataLen);
EXTERN INT32    PCMCIA_Write(POD_IO_STATE_T eIoState, const UINT8* pu1Data, UINT16 u2DataLen);
EXTERN INT32    PCMCIA_ReadReg(POD_IO_STATE_T eIoState, UINT32 u4RegAddr, UINT8* pu1Data, UINT16 pu2DataLen);
EXTERN INT32    PCMCIA_WriteReg(POD_IO_STATE_T eIoState, UINT32 u4RegAddr, const UINT8* pu1Data, UINT16 u2DataLen);
EXTERN INT32    PCMCIA_ReadCor(UINT16 u2CorAddress);
EXTERN INT32    PCMCIA_WriteCor(UINT16 u2CorAddress, UINT8 u1CorValue);
EXTERN void     PCMCIA_CamPwrControl(BOOL bManualControl);
EXTERN void     PCMCIA_CamPwrOnOff(BOOL bPwrOn);
EXTERN INT32    PCMCIA_Reset(BOOL b_set);
EXTERN void     PCMCIA_ResetCam(void);
EXTERN void     PCMCIA_ResetPcmciaHw(void);
EXTERN void     PCMCIA_ThreadNoAssert(BOOL bNoAssert);
EXTERN void     PCMCIA_StopThread(void);
EXTERN INT32    PCMCIA_EnableIsr(void);
EXTERN PCMCIA_IF_TYPE_T     PCMCIA_GetIfType(void);
EXTERN void                 PCMCIA_SetIfType(PCMCIA_IF_TYPE_T eOpMode);
EXTERN INT32    PCMCIA_GetPodCiAccessMode(POD_ACCESS_MODE_T* peAccessMode);
EXTERN INT32    PCMCIA_SetPodCiAccessMode(POD_ACCESS_MODE_T eAccessMode);
EXTERN INT32    PCMCIA_GetDaFrIntStatus(POD_ACCESS_MODE_T  eAccessMode, UINT8* pu1DaFrSts);
EXTERN BOOL PCMCIA_ResetChannel(void);

EXTERN INT32    PCMCIA_NegotiatedBufSize(UINT16* pu2BufSize);
EXTERN INT32    PCMCIA_WaitForModuleStatus(UINT8 u1StatusBit, UINT16 u2TimeOut);
EXTERN INT32    PCMCIA_ParseCis(UINT8* pu1CisMap, CIS_INFO_T* prInfo);
EXTERN INT32    PCMCIA_CheckCIS(UINT8* pu1Cis, UINT16 *addrcor, UINT8 *cor);
EXTERN INT32    PCMCIA_CheckCIPLUSCapability(UINT8 *pui1CIPlusCap);
EXTERN UINT32   PCMCIA_GetBypass(void);
EXTERN void     PCMCIA_SetBypass(BOOL fgBypass);
EXTERN void     PCMCIA_SetBypassTsRouting(BOOL fgBypass);
EXTERN INT32    PCMCIA_PcmciaToPod(BOOL fgPcmciaToPod);
EXTERN INT32    PCMCIA_PcmciaEnableTS(BOOL fgPcmciaEnableTS);
EXTERN INT32    PCMCIA_DetectCard(void);
EXTERN INT32    PCMCIA_TestCardReady(void);
EXTERN BOOL     PCMCIA_DetectPodActive(void);
EXTERN void     PCMCIA_ExternalDemodOnOff(BOOL fgOnOff);
EXTERN void     PCMCIA_ParallelTsOnOff(BOOL fgOnOff);
EXTERN void     PCMCIA_RegIsr(UINT16 u2IntNo, x_os_isr_fct pfnIsr, x_os_isr_fct *ppfnOld);
EXTERN void     PCMCIA_RegCamStatusCbk(x_pcmcia_nfy_fct pfn);
EXTERN void     PCMCIA_ExternalIcSetTsClkRate(UINT32 u4ClkRate);
EXTERN void     PCMCIA_EnableDemodTs(void);
EXTERN void     PCMCIA_DisableDemodTs(void);
EXTERN void     PCMCIA_AutoSetTsRoutingPath(void);
EXTERN void     PCMCIA_SLT(UINT8 u1TsIndex, UINT8 u1TsClkRate, UINT8 u1TsDataWidth);
EXTERN void     PCMCIA_Demo(pcmcia_cam_connectivity_nfy pfnCbk);
EXTERN void             PCMCIA_SetTsType(PCMCIA_TS_TYPE eTypeValue);
EXTERN PCMCIA_TS_TYPE   PCMCIA_GetTsType(void);
EXTERN void     PCMCIA_UpdateTsStatus(void);
EXTERN void     PCMCIA_CiCamWorkaround(CI_CAM_WORKAROUND eCiCamWorkaround, void* vParam);
#endif // CC_MTK_LOADER
EXTERN INT32    CI_GPIO_Init(void);
EXTERN INT32    CI_GPIO_Range_Check(INT32 i4GpioNum);
EXTERN INT32    CI_MT8295_GPIO_Range_Check(INT32 i4GpioNum);
EXTERN INT32    CI_MT5135_GPIO_Range_Check(INT32 i4GpioNum);
EXTERN INT32    CI_GPIO_Enable(INT32 i4GpioNum, INT32 * pi4Polarity);
EXTERN INT32    CI_GPIO_Input(INT32 i4GpioNum);
EXTERN INT32    CI_GPIO_Output(INT32 i4GpioNum, INT32 * pi4DriveHighLow);
EXTERN INT32    CI_GPIO_Intrq(INT32 i4Gpio, INT32 *pfgSet);
EXTERN INT32    CI_GPIO_Reg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
EXTERN INT32    CI_GPIO_Query(INT32 i4Gpio, INT32 *pi4Intr, INT32 *pi4Fall, INT32 *pi4Rise);
#ifdef CC_FAST_INIT
EXTERN INT32    CI_GPIO_Suspend(void);
EXTERN INT32    CI_GPIO_Resume(void);
EXTERN INT32    CI_GPIO_ResumeINTEN(INT32 i4Gpio, GPIO_TYPE eType);
#endif
#ifndef CC_MTK_LOADER
EXTERN INT32    PCMCIA_BIM_GetDeviceData(BYTE *pu1DeviceID, BYTE u1Size);
EXTERN INT32    PCMCIA_BIM_GetDeviceID(BYTE *pu1DeviceID, BYTE u1Size);
EXTERN INT32    PCMCIA_BIM_GetSecureID(BYTE *pu1SecureID, BYTE u1Size);

EXTERN void     PCMCIA_Set2Bit(BOOL fg2Bit);
EXTERN BOOL     PCMCIA_Get2Bit(void);
EXTERN void     PcmciaSetDriving(UINT32 u4Driving);
EXTERN void     PcmciaSetTsClock(UINT32 u4TsClock);
EXTERN void     PcmciaSetSysClock(UINT32 u4SysClock);
#ifdef CC_FAST_INIT
EXTERN void     PCMCIA_pm_suspend(void);
EXTERN void     PCMCIA_pm_resume(void);
#endif
#endif // CC_MTK_LOADER

#endif //PCMCIA_IF_H
