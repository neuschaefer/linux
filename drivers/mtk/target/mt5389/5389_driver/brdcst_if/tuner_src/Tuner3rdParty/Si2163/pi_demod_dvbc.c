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
 /** @file pi_demod_dvbc.c
 *  DvbC demod related API implementation
 */


 //-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#define EMB_L1_DRIVER_CORE          /* empty  */
#define SLAB_WINAPI                 /* empty  */
#define EMB_L1_COMMON_DEMOD_API     /* empty  */
#define EMB_L1_MULTISTANDARD_API    /* empty  */
#define EMB_L1_DVB_C_DEMOD_API      /* empty  */
#define WINAPI                      /* empty  */

#include "Emb_L1_Driver_Core.h"
#include "Emb_L1_Common_Demod_API.h"
#include "Emb_L1_Registers_Defines.h"
#include "pd_dvbc_if.h"
#include "pi_demod.h"
#include "PI_Def.h"
#include "PD_Def.h"
#include "x_hal_5381.h"
#include "x_gpio.h"
#include "drvcust_if.h"
#include "x_pinmux.h"


#define mcSET_SHOW_REG(pu4RegBase, u4RegOfst, u4RegValue)   \
{                                                           \
/*    mcRISC_Reg(pu4RegBase, u4RegOfst) = u4RegValue;   */  \
    IO_WRITE32(pu4RegBase, u4RegOfst, u4RegValue);          \
    mcSHOW_DBG_MSG(("w 0x%08"cFMT_XL" 0x%08"cFMT_XL"\n", ((UINT32) (pu4RegBase)) + u4RegOfst, (UINT32) (u4RegValue)));  \
}\


//----------------------------------------------------------------------------- 
/*
 *  DVBC_ChipPowerOnReset
 *  This function initialize DVB-C demod.
 */
//-----------------------------------------------------------------------------
VOID DVBC_ChipPowerOnReset(VOID)
{
    UINT32 u4RstPinNum;
    U16 u2DelayTime = 10;

    if (DRVCUST_OptQuery(eDemodResetGpio, &u4RstPinNum) == 0)
    {
        GPIO_SetOut(u4RstPinNum, 0);
        mcDELAY_MS(u2DelayTime);
        GPIO_SetOut(u4RstPinNum, 1);
        mcDELAY_MS(100); 
        mcSHOW_DBG_MSG(("Si2163 Reset Demod GPIO=0x%x, 0->1, %dms\r\n",u4RstPinNum, u2DelayTime));
    }  
}


//----------------------------------------------------------------------------- 
/*
 *  DVBC_ChipInit
 *  This function initialize DVB-C related registers in the chip.
 *  @retval   1 : I2C error.
 *  @retval   0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_ChipInit(L1_Context *demod)
{
    UCHAR  ucPar = 0;
    UINT8  u1ret = 0;
    UINT32 u4Temp;

    mcSHOW_DBG_MSG(("DVBC_ChipInit()\n"));

    //Set MT5363 RFAGC and IFAGC to high-Z   (pin mux: [29:28] = 11, GPIO: [9:8] = 00)
    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x408);
    u4Temp |= 0x30000000;
    mcSET_SHOW_REG(CKGEN_BASE, 0x408, u4Temp);

    u4Temp = mcRISC_Reg(CKGEN_BASE, 0x514);
    u4Temp &= 0xFFFFFCFF;
    mcSET_SHOW_REG(CKGEN_BASE, 0x514, u4Temp);

    //Disable RFAGC switch
//    u4Temp = mcRISC_Reg(PDWNC_BASE, 0x0b4);   //pin mux [] = 2b'01
//    u4Temp &= 0xFFF7FFFF;
//    mcSET_SHOW_REG(PDWNC_BASE, 0x0b4, u4Temp);

//    u4Temp = mcRISC_Reg(PDWNC_BASE, 0x0b4);
//    mcSHOW_USER_MSG(("RFAGC switch 1 CFG (0x200280b4) = 0x%x\n", u4Temp));

//    u4Temp = mcRISC_Reg(PDWNC_BASE, 0x398);
//    u4Temp |= 0x00000008;
//    mcSET_SHOW_REG(PDWNC_BASE, 0x398, u4Temp);

//    u4Temp = mcRISC_Reg(PDWNC_BASE, 0x398);
//    mcSHOW_USER_MSG(("RFAGC switch 2 CFG (0x20028398) = 0x%x\n", u4Temp));

    //DVB-C demod power up
    ucPar = 0x21;
    if (L1_WriteBytes8Bits(demod, 0x000, 1, &ucPar) == 0) {u1ret = 1;}
    ucPar = 0x01;
    if (L1_WriteBytes8Bits(demod, 0x104, 1, &ucPar) == 0) {u1ret = 1;}

    //Set DVB-C IFAGC to normal
    ucPar = 0;
    if (L1_WriteBytes8Bits(demod, 0x18B, 1, &ucPar) == 0) {u1ret = 1;}
    mcSHOW_DBG_MSG(("Set Si2163 IFAGC to normal\n"));

    //Set TS serial mode
//    L1_Demod_TS_mode(demod, ts_data_mode_serial);
//    if (L1_ReadBytes8Bits(demod, 0x4E4, 1, &ucPar) == 0) {u1ret = 1;}
//    ucPar &= 0xFE;
//    if (L1_WriteBytes8Bits(demod, 0x4E4, 1, &ucPar) == 0) {u1ret = 1;}

    //Enable TS output
//    ucPar = 0;
//    if (L1_WriteBytes8Bits(demod, 0x4EF, 1, &ucPar) == 0) {u1ret = 1;}
//    if (L1_WriteBytes8Bits(demod, 0x4F0, 1, &ucPar) == 0) {u1ret = 1;}


    return u1ret;
}


//----------------------------------------------------------------------------- 
/*
 *  DVBC_ChipPowerDown
 *  This function power down demod module.
 *  @retval   1 : I2C error.
 *  @retval   0 : OK.   
 */
//-----------------------------------------------------------------------------
UINT8 DVBC_ChipPowerDown(L1_Context *demod)
{
    UCHAR ucPar = 0;
    UINT8 u1ret = 0;

    mcSHOW_DBG_MSG(("DVBC_ChipPowerDown()\n"));

    //Set IFAGC to tri-state
    ucPar = 1;
    if (L1_WriteBytes8Bits(demod, 0x18B, 1, &ucPar) == 0) {u1ret = 1;}
    mcSHOW_DBG_MSG(("Set Si2163 IFAGC to high-Z\n"));

    //Set TS output to high-Z
    ucPar = 0xFF;
    L1_WriteBytes8Bits(demod, 0x4EF, 1, &ucPar);
    ucPar = 0x0F;
    L1_WriteBytes8Bits(demod, 0x4F0, 1, &ucPar);
//    L1_Demod_TS_mode(demod, 2);

    //Demod power down
    ucPar = 0;
    if (L1_WriteBytes8Bits(demod, 0x104, 1, &ucPar) == 0) {u1ret = 1;}
    if (L1_WriteBytes8Bits(demod, 0x000, 1, &ucPar) == 0) {u1ret = 1;}

    return u1ret;
}
