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
 
#ifndef _PDWNC_IF_H_
#define _PDWNC_IF_H_

//----------------------------------------------------------------------------
// Constant definitions
//----------------------------------------------------------------------------
#include "x_typedef.h"
#include "x_pdwnc.h"
#include "sif_if.h"

//----------------------------------------------------------------------------
// Config
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Constant definitions
//----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define T8032_HOLD_ENGINE() 0//(PDWNC_READ32(REG_RW_UP_CFG) & ENGINE_EN)
#define T8032_ALIVE() 0//  ((IO32ReadFldAlign(PDWNC_UP_CFG, FLD_T8032_RST) == 0) && (IO32ReadFldAlign(PDWNC_UP_CFG, FLD_VIR_EN) == 1))

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/** PDWNC function table
 */
typedef struct {
    void (*pfnInit)(void);	
    void (*pfnPowerDown)(UINT32, UINT32*);    
    void (*pfnReadWakeupReason)(UINT32*);
    void (*pfnWatchDogInit)(void);    
    void (*pfnErrorHandleInit)(void);
    void (*pfnWriteErrorCode)(UINT8);
    void (*pfnReadErrorCode)(UINT8*);
    void (*pfnGetCurrentState)(UINT32*);
    void (*pfnDetermineNextState)(UINT32, UINT32, UINT32*, UINT32 *, UINT32 *);
    void (*pfnEnterNextState)(UINT32);
    INT32 (*pfnSet8032uPLedBlink)(UINT32);
    UINT32 (*pfnCustomRead)(UINT32, void *, UINT32);
    UINT32 (*pfnCustomWrite)(UINT32, void *, UINT32);
} PDWNC_FuncTbl;

typedef enum
{
    PDWNC_T8032_SET_NORMAL = 0,
    PDWNC_T8032_SET_STANDBY,          
    PDWNC_T8032_SET_XDATA_ARM,              
    PDWNC_T8032_SET_XDATA_T8032,
    PDWNC_T8032_SET_ENGINE_ARM,              
    PDWNC_T8032_SET_ENGINE_T8032,    
} E_PDWNC_T8032_SET_TYPE;

typedef enum
    {
        BR_115200 = 0,
        BR_57600,       
        BR_38400,       
        BR_19200,       
        BR_9600,        
        BR_2400
} E_PDWNC_T8032_UART_PD_BAUDRATE;


//----------------------------------------------------------------------------
// Static variables
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Public interface
//----------------------------------------------------------------------------
EXTERN PDWNC_FuncTbl* PDWNC_EXT_GetFunc(void);
EXTERN INT32 PDWNC_InitT8032(UINT32 u4Addr, UINT32 u4Size);
EXTERN INT32 PDWNC_T8032Cmd(PDWNC_T8032_CMD_T* prCmd, PDWNC_T8032_RESPONSE_T* prResponse);
EXTERN INT32 PDWNC_T8032GpioCmd(PDWNC_T8032_CMD_T* prCmd, PDWNC_T8032_RESPONSE_T* prResponse);
EXTERN INT32 PDWNC_ReadSwVersion(UINT32* pu4SwVersion);
EXTERN INT32 PDWNC_WaitLastTransactionFin(void);
EXTERN INT32 PDWNC_SetLastTransactionFin(void);
extern INT32 PDWNC_SetUartPDBaudrate(UINT8 u1baudrate);
EXTERN BOOL PDWNC_SetupFpWakeup(BOOL *pfgEnable);
EXTERN INT32 PDWNC_SetT8032ToggleLed(UINT32 u4Enable);
EXTERN INT32 PDWNC_GpioEnable(INT32 i4Gpio, INT32 *pfgSet);
EXTERN INT32 PDWNC_GpioOutput(INT32 i4Gpio, INT32 *pfgSet);
EXTERN INT32 PDWNC_GpioInput(INT32 i4Gpio);
EXTERN INT32 PDWNC_ServoGpioRangeCheck(INT32 i4Gpio);
EXTERN INT32 PDWNC_ServoGpioEnable(INT32 i4Gpio, INT32 *pfgSet);
EXTERN INT32 PDWNC_ServoGpioOutput(INT32 i4Gpio, INT32 *pfgSet);
EXTERN INT32 PDWNC_ServoGpioInput(INT32 i4Gpio);
EXTERN UINT8 PDWNC_SetQueryRtcYear(const UINT8 *pu1Year);
EXTERN void PDWNC_SetupGPIO(UINT32 *pu4PowerDownEn);
EXTERN void PDWNC_SetupServoADC(UINT32 *pu4PowerDownEn);
EXTERN INT32 _PDWNC_SetT8032(UINT32 u4Set);
EXTERN UINT32 _PDWNC_ReadWakeupStatus(void);
EXTERN INT32 _PDWNC_RegLedBlinker(INT32 i4Gpio, BOOL fgEnable, UINT8 u1RoundPeriod, UINT8 u1LightPeriod);
EXTERN void PDWNC_SetupPowerDown(void);
EXTERN void PDWNC_EnableIrLed(BOOL fgEnable);
EXTERN void PDWNC_InitServoADC(void);
EXTERN UINT8 PDWNC_SetQueryRtcYear(const UINT8 *pu1Year);
EXTERN void PDWNC_SetupGPIO(UINT32 *pu4PowerDownEn);
EXTERN void PDWNC_SetupServoADC(UINT32 *pu4PowerDownEn);
EXTERN INT32 PDWNC_GpioRangeCheck(INT32 i4Gpio);
EXTERN INT32 _PDWNC_GetT8302CECData(UINT32 u4XAddr, UINT32 u4Size, UINT8* pu1Data);
EXTERN INT32 _CopyToT8032_XDATA(UINT32 u4XAddr, UINT32 u4Size, UINT32 u4RiscAddr);
EXTERN INT32 _CopyFromT8032_XDATA(UINT32 u4XAddr, UINT32 u4Size, UINT32 u4RiscAddr);
EXTERN INT32 PDWNC_T8032GpioReg(INT32 i4Gpio, GPIO_TYPE eType, PFN_GPIO_CALLBACK pfnCallback);
EXTERN INT32 PDWNC_GpioSetIntExtParam(INT32 i4Gpio, UINT32 u4Arg);
EXTERN INT32 PDWNC_GpioGetIntExtParam(INT32 i4Gpio, UINT32 *pu4Arg);

#endif
