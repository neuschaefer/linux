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
#ifndef MTK_SIF_CFG_H
#define MTK_SIF_CFG_H

#include "x_gpio.h"

#include "x_timer.h"
#include "x_pinmux.h"
#include "x_os.h"
#include "x_assert.h"
#include "drvcust_if.h"
#include "pcmcia_if.h"

static SIF_CONFIG_T _rSifConfig =
{
    5, // Total 7 buses
    {
        2, // SystemI2C uses V2 engine
        2, // TunerI2C uses V2 engine 
        1, // SecondI2C uses V1 engine 
        2, // PdwncI2C uses V2 engine 
        2, // PdwncI2C1 uses V2 engine 
        0, // 8295BI2C0
        0  // 8295BI2C1
    }
};



//#define SIF_CFG_BEFORE_ACTION_FUNC      (SIF_MtkCfgBeforeActionFunc)
//#define SIF_CFG_AFTER_ACTION_FUNC     (SIF_MtkCfgAfterActionFunc)
#define SIF_CFG_PIN_SET_FUNC     (SIF_MtkCfgPinSetFunc)
#define SIF_CFG_GET_CONFIG_FUNC     (SIF_MtkCfgGetConfigFunc)
//#define SIF_CFG_TRI_DELAY (0)
//#define SIF_CFG_TRI_DELAY_DEV_ADDR (0)
EXTERN BOOL _SIF_ISR_IsPdwncMasterEnable(void);
EXTERN BOOL _SIF_ISR_IsPdwncMaster1Enable(void);

static void SIF_MtkCfgPinSetFunc(UINT8 u1Channel)
{
    UINT32 u4Reg = 0;
#ifndef CC_MTK_LOADER       
    CRIT_STATE_T rCrit;
    rCrit = x_crit_start();    
#endif    
    if (u1Channel == 0)	//osda0/oscl0, use V2 engine
    {
        u4Reg = CKGEN_READ32(0x400);
        //  [23:22]  != 2; [21:20] = 1
        if((u4Reg & 0x00c00000) == 0x00800000)
        {
            u4Reg &= ~(0x00c00000);
        }
        u4Reg &= ~(0x00300000);        
        u4Reg |= 0x00100000;
        CKGEN_WRITE32(0x400, u4Reg);
        u4Reg = CKGEN_READ32(0x404);        
        // [1:0] != 1
        if((u4Reg & 0x00000003) == 0x00000001)
        {
            u4Reg &= ~(0x00000003);
        }
        CKGEN_WRITE32(0x404, u4Reg);        
    }
    else if (u1Channel == 1)	//osda2/oscl2 for tuner, use V2 engine
    {
        u4Reg = CKGEN_READ32(0x400);
        // [23:22] != 2 ; [21:20]  != 1
        if((u4Reg & 0x00c00000) == 0x00800000)
        {
            u4Reg &= ~(0x00c00000);
        }
        if((u4Reg & 0x00300000) == 0x00100000)
        {
            u4Reg &= ~(0x00300000);
        }        
        CKGEN_WRITE32(0x400, u4Reg);
        u4Reg = CKGEN_READ32(0x404);        
     	// [1:0] = 1
        u4Reg &= ~(0x00000003);
        u4Reg |= 0x00000001;
        CKGEN_WRITE32(0x404, u4Reg);        
    }
    else if (u1Channel == 2)	//osda1/oscl1, use V1 engine
    {
        u4Reg = CKGEN_READ32(0x400);
        // [23:22] = 1; [21:20]  != 2
        u4Reg &= ~(0x00c00000);        
        u4Reg |= 0x00400000;
        if((u4Reg & 0x00300000) == 0x00200000)
        {
            u4Reg &= ~(0x00300000);
        }        
        CKGEN_WRITE32(0x400, u4Reg);
        u4Reg = CKGEN_READ32(0x404);        
        // [1:0]  != 2
        if((u4Reg & 0x00000003) == 0x00000002)
        {
            u4Reg &= ~(0x00000003);
        }
        CKGEN_WRITE32(0x404, u4Reg);        
    }    
    else if (_SIF_ISR_IsPdwncMasterEnable() && u1Channel == 3) // PDWNC SIF0
    {
        u4Reg = IO_READ32(PDWNC_BASE, 0xB4);
        // [3:2] = 0x1, [1:0] = 0x1
        u4Reg &= ~(0x0000000F);
        u4Reg |= 0x00000005;       
        IO_WRITE32(PDWNC_BASE, 0xB4, u4Reg);     
    }
    else if (_SIF_ISR_IsPdwncMaster1Enable() && u1Channel == 4) // PDWNC SIF1
    {
        u4Reg = IO_READ32(PDWNC_BASE, 0xB4);
        // [27:26] = 0x1
        u4Reg &= ~(0x0c000000);
        u4Reg |= 0x04000000;       
        IO_WRITE32(PDWNC_BASE, 0xB4, u4Reg);     
    }
#ifndef CC_MTK_LOADER       
    x_crit_end(rCrit);    
#endif
}


static SIF_CONFIG_T* SIF_MtkCfgGetConfigFunc(void)
{
    return &_rSifConfig;
}

#if 0
/** SIF_MtkCfgBeforeActionFunc
 *  do cfg BeforeAction
 *  @param  u1BusNo       sif bus no.
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pSUbAddr    Pointer to sub addresses
 *  @param  u4SubAddrLen  Sub address length
 *  @param  pData       Pointer to user's buffer.
 *  @param  u4DataLen   Number of bytes to read.
 *  @param  u1IsRead     is in read function or write function. 1: read, 0: write

 */
static void SIF_MtkCfgBeforeActionFunc(UINT8 u1BusNo,UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 *pSubAddr, UINT32 u4SubAddrLen,UINT8 *pData, UINT32 
u4DataLen, UINT8 u1IsRead)
{
        
}
/** SIF_MtkCfgAfterActionFunc
 *  do cfg AfterAction
 *  @param  u1BusNo       sif bus no.
 *  @param  u2ClkDiv    Parameter of divider to divide 27Mhz for SCL.
 *  @param  u1DevAddr   Serial interface device address.
 *  @param  pSUbAddr    Pointer to sub addresses
 *  @param  u4SubAddrLen  Sub address length
 *  @param  pData       Pointer to user's buffer.
 *  @param  u4DataLen   Number of bytes to read.
 *  @param  u1IsRead     is in read function or write function. 1: read, 0: write

 */
static void SIF_MtkCfgAfterActionFunc(UINT8 u1BusNo,UINT16 u2ClkDiv, UINT8 u1DevAddr, UINT8 *pSubAddr, UINT32 u4SubAddrLen,UINT8 *pData, UINT32 
u4DataLen,UINT8 u1IsRead)
{
        
}
#endif

#endif /* MTK_SIF_CFG */
