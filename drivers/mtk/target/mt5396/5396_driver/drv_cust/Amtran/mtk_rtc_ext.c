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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: mtk_rtc_ext.c,v $
 * $Revision: #1 $
 *
 * Description:
 *         Digimedia's RTC external hardware ISL1208 driver implementation.
 *---------------------------------------------------------------------------*/

//----------------------------------------------------------------------------
// Constant definitions
//----------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "sif_if.h" 
#include "rtc_if.h"
LINT_EXT_HEADER_END

//----------------------------------------------------------------------------
// Constant definitions
//----------------------------------------------------------------------------

#define ISL1208_REG_RTC_SC 0x0
#define ISL1208_REG_RTC_MN 0x1
#define ISL1208_REG_RTC_HR 0x2
#define ISL1208_REG_RTC_DT 0x3
#define ISL1208_REG_RTC_MO 0x4
#define ISL1208_REG_RTC_YR 0x5
#define ISL1208_REG_RTC_DW 0x6
#define ISL1208_RTC_HR_MIL (1U << 7)

#define ISL1208_REG_CAS_SR 0x7
#define ISL1208_REG_CAS_INT 0x8
#define ISL1208_REG_CAS_ATR 0xA
#define ISL1208_REG_CAS_DTR 0xB
#define ISL1208_CAS_SR_RTCF (1U << 0)
#define ISL1208_CAS_SR_BAT (1U << 1)
#define ISL1208_CAS_SR_ALM (1U << 2)
#define ISL1208_CAS_SR_WRTC (1U << 4)
#define ISL1208_CAS_SR_XTOSCB (1U << 6)
#define ISL1208_CAS_SR_ARST (1U << 7)

#define ISL1208_CAS_INT_FOBATB (1U << 4)
#define ISL1208_CAS_INT_LPMODE (1U << 5)
#define ISL1208_CAS_INT_ALME (1U << 6)
#define ISL1208_CAS_INT_IM (1U << 7)
#define ISL1208_CAS_ATR_BMATR0 (1U << 6)

#define ISL1208_REG_ALM_SCA 0xC
#define ISL1208_REG_ALM_MNA 0xD
#define ISL1208_REG_ALM_HRA 0xE
#define ISL1208_REG_ALM_DTA 0xF
#define ISL1208_REG_ALM_MOA 0x10
#define ISL1208_REG_ALM_DWA 0x11
#define ISL1208_ALM_MATCH (1U << 7)

#define ISL1208_REG_USR_USR1 0x12
#define ISL1208_REG_USR_USR2 0x13

#define ISL1208_DEV_ADDR 0x6F

#define ISL1208_CLK_DIV 					0x100

//----------------------------------------------------------------------------
// Prototypes
//----------------------------------------------------------------------------
static BOOL _ISL1208_Init(void);
static BOOL _ISL1208_GetTimeDate(UINT64* prTime);
static BOOL _ISL1208_SetTimeDate(const UINT64* prTime);
static BOOL _ISL1208_GetAlertTime(UINT64* prTime);
static BOOL _ISL1208_SetAlertTime(UINT64 u8Utc);

//----------------------------------------------------------------------------
// Interface
//----------------------------------------------------------------------------
static RTC_FuncTbl _RTC_EXT_FuncTbl =
{
    _ISL1208_Init,
    _ISL1208_GetTimeDate,
    _ISL1208_SetTimeDate,
    _ISL1208_GetAlertTime,
    _ISL1208_SetAlertTime  
} ;
RTC_FuncTbl* _RTC_EXT_GetFunc(void)
{
    return &_RTC_EXT_FuncTbl;
}

//----------------------------------------------------------------------------
// Static variables
//----------------------------------------------------------------------------
static BOOL _fgRTCInit = FALSE;
static BOOL _fgRTCDisableRTC = FALSE;

//----------------------------------------------------------------------------
// Private functions
//----------------------------------------------------------------------------

static void _ISL1208_Read(UINT8 u1Addr, UINT8* pu1Data, UINT32 u4Len)
{
    UINT32 u4ReadCount = 0; 
    
    u4ReadCount = SIF_Read(ISL1208_CLK_DIV, ISL1208_DEV_ADDR << 1, u1Addr, pu1Data, u4Len);

    if (u4ReadCount > 0)
    {
    	;
    }	
    else
    {
    	Printf("[RTC] Error ISL1208 register setting\n");
    }	
}

static void _ISL1208_Write(UINT8 u1Addr, UINT8* pu1Data, UINT32 u4Len)
{
    UINT32 u4WriteCount = 0; 
    
    u4WriteCount = SIF_Write(ISL1208_CLK_DIV, ISL1208_DEV_ADDR << 1, u1Addr, pu1Data, u4Len);

    if (u4WriteCount > 0)
    {
    	;
    }	
    else
    {
    	Printf("[RTC] Error ISL1208 register setting\n");
    }	
}

//----------------------------------------------------------------------------
/** RTC_Init() Initialize RTC driver and hardware
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
static BOOL _ISL1208_Init(void)
{
    UINT64 u8Utc;
    UINT8 u1Temp;

    if (_fgRTCInit)
    {
        // Already initialized
        return TRUE;
    }
    
    // Pass write protection
    _ISL1208_Read(ISL1208_REG_CAS_SR, &u1Temp, 1);
    u1Temp = u1Temp | ISL1208_CAS_SR_WRTC;
    u1Temp = u1Temp | ISL1208_CAS_SR_ARST;
    u1Temp = (u1Temp & (~(1u <<6)));
    _ISL1208_Write(ISL1208_REG_CAS_SR, &u1Temp, 1);

    // Check RTC flags
    _ISL1208_Read(ISL1208_REG_CAS_SR, &u1Temp, 1);
    if ((u1Temp & ISL1208_CAS_SR_WRTC) == 0)
    {
        _fgRTCDisableRTC = TRUE;
        Printf("Swap on RTC.\n");
        return TRUE;
    }

    // Check if total power failure upon init
    if(u1Temp & ISL1208_CAS_SR_RTCF)
    {
#if 1
        u8Utc = UTC_INIT_TIME;
#else
        RTC_T rRtc;
        rRtc.u1Year = 0;
        rRtc.u1Month = 1;
        rRtc.u1Day = 1;
        rRtc.u1Hour = 0;
        rRtc.u1Minute = 0;
        rRtc.u1Second = 0;
        if(!RTC_RtcToUtc(&u8Utc, &rRtc))
        {
            return FALSE;
        }
#endif        
        if(!_ISL1208_SetTimeDate(&u8Utc))
        {
            return FALSE;
        }
    }

    // Enable alarm and set alarm interrupt mode
    u1Temp = ISL1208_CAS_INT_IM | ISL1208_CAS_INT_ALME;
    _ISL1208_Write(ISL1208_REG_CAS_INT, &u1Temp, 1);

    // Start RTC, set to 24-hour mode
    _ISL1208_Read(ISL1208_REG_RTC_HR, &u1Temp, 1);
    u1Temp |= ISL1208_RTC_HR_MIL;
    _ISL1208_Write(ISL1208_REG_RTC_HR, &u1Temp, 1);

    _fgRTCInit = TRUE;
    
    return TRUE;
}


//----------------------------------------------------------------------------
/** RTC_GetTimeDate() Get time from RTC
 *  @param prTime, Output, The time
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
static BOOL _ISL1208_GetTimeDate(UINT64* prTime)
{
    RTC_T rRtc;
    UINT64 u8Utc;
    UINT8 au1Temp[6];

    ASSERT(prTime!=NULL);
    if (_fgRTCDisableRTC)
    {
        *prTime = 0;
        return FALSE;
    }

    // Get time in second from RTC
    _ISL1208_Read(ISL1208_REG_RTC_SC, au1Temp, 6);       
    rRtc.u1Second = RTC_BcdToBinary(au1Temp[0]);  
    rRtc.u1Minute = RTC_BcdToBinary(au1Temp[1]);     
    rRtc.u1Hour = RTC_BcdToBinary(au1Temp[2] & 0x3F);
    rRtc.u1Day =  RTC_BcdToBinary(au1Temp[3]);         
    rRtc.u1Month = RTC_BcdToBinary(au1Temp[4]); 
    rRtc.u1Year = RTC_BcdToBinary(au1Temp[5]);
    
    // Translate to TIME_T(UINT64)
    if (RTC_RtcToUtc(&u8Utc, &rRtc))
    {
        *prTime = (UINT64)u8Utc;
        return TRUE;
    }

    *prTime = 0;

    return FALSE;
}


//----------------------------------------------------------------------------
/** RTC_SetTimeDate() Set time to RTC
 *  @param prTime, Input, The time to set.
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
static BOOL _ISL1208_SetTimeDate(const UINT64* prTime)
{
    RTC_T rRtc;
    UINT64 u8Utc;
    UINT8 au1Temp[6];

    ASSERT(prTime!=NULL);
    // Translate to RTC
    u8Utc = (UINT64)*prTime;
    if (!RTC_UtcToRtc(&rRtc, u8Utc))
    {
        return FALSE;
    }

    if (!_fgRTCDisableRTC)
    {
        // Get time in second from RTC
    au1Temp[0] =  RTC_BinaryToBcd(rRtc.u1Second);   
    au1Temp[1] = RTC_BinaryToBcd(rRtc.u1Minute);
    au1Temp[2] = (RTC_BinaryToBcd(rRtc.u1Hour) | ISL1208_RTC_HR_MIL);
    au1Temp[3] = RTC_BinaryToBcd(rRtc.u1Day);
    au1Temp[4] = RTC_BinaryToBcd(rRtc.u1Month);
    au1Temp[5] =  RTC_BinaryToBcd(rRtc.u1Year);
    _ISL1208_Write(ISL1208_REG_RTC_YR, au1Temp, 6);
    
    // Disable RTC STOP
    // TODO
    
    }

    return TRUE;
}

//----------------------------------------------------------------------------
/** RTC_SetAlertTime() Set time to RTC
 *  @param prTime, Input, The time to set.
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
static BOOL _ISL1208_SetAlertTime(UINT64 u8Utc)
{
    RTC_T rRtc;
    UINT8 au1Temp[5];
    
    if (!RTC_UtcToRtc(&rRtc, u8Utc))
    {
        Printf("SetAlertTime failed.\n");
        return FALSE;
    }

    // Set H24 and Stop mode
    _ISL1208_Read(ISL1208_REG_CAS_INT, au1Temp, 1);
    au1Temp[0] |= ISL1208_CAS_INT_ALME;    
    _ISL1208_Write(ISL1208_REG_CAS_INT, au1Temp, 1);
    
    // Set to RTC
    au1Temp[0] = (RTC_BinaryToBcd(rRtc.u1Second) | ISL1208_ALM_MATCH);    
    au1Temp[1] = (RTC_BinaryToBcd(rRtc.u1Minute) | ISL1208_ALM_MATCH);    
    au1Temp[2] = (RTC_BinaryToBcd(rRtc.u1Hour) | ISL1208_ALM_MATCH);        
    au1Temp[3] = (RTC_BinaryToBcd(rRtc.u1Day) | ISL1208_ALM_MATCH);        
    au1Temp[4] = (RTC_BinaryToBcd(rRtc.u1Month) | ISL1208_ALM_MATCH);    
    _ISL1208_Write(ISL1208_REG_ALM_SCA, au1Temp, 5);
    
    // Disable RTC STOP
    // Not necessary for ISL 1208

    return TRUE;
}

//----------------------------------------------------------------------------
/** RTC_GetAlertTime() Get time from RTC
 *  @param prTime, Output, The time
 *  @retval TRUE, Succeed; FALSE, Fail.
 */
//----------------------------------------------------------------------------
static BOOL _ISL1208_GetAlertTime(UINT64* prTime)
{

    RTC_T rRtc;
    UINT64 u8Utc;
    UINT8 au1Temp[5];

    ASSERT(prTime!=NULL);
    if (_fgRTCDisableRTC)
    {
        *prTime = 0;
        return FALSE;
    }

    // Get year from RTC
    _ISL1208_Read(ISL1208_REG_ALM_SCA, au1Temp, 5);
    rRtc.u1Second = RTC_BcdToBinary(au1Temp[0] & 0x7F);      
    rRtc.u1Minute = RTC_BcdToBinary(au1Temp[1] & 0x7F);     
    rRtc.u1Hour = RTC_BcdToBinary(au1Temp[2] & 0x3F);    
    rRtc.u1Day =  RTC_BcdToBinary(au1Temp[3] & 0x3F);
    rRtc.u1Month = RTC_BcdToBinary(au1Temp[4] & 0x1F); 
    _ISL1208_Read(ISL1208_REG_RTC_YR, au1Temp, 1);
    rRtc.u1Year = RTC_BcdToBinary(au1Temp[0]); 
   
    // Translate to TIME_T(UINT64)
    if (RTC_RtcToUtc(&u8Utc, &rRtc))
    {
        *prTime = (UINT64)u8Utc;
        return TRUE;
    }

    *prTime = 0;

    return FALSE;
}


