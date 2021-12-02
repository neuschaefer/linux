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
 * $RCSfile: nandhw_reg.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file nand_reg.h
 *  ldm_reg.h The LDM register constant macros
 */

#ifndef LDM_IF_H
#define LDM_IF_H

#include "x_typedef.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_hal_io.h"
#include "panel.h"
#include "x_ckgen.h"


typedef enum {
    LDM_PANEL_NONE = 0,
    LDM_PANEL_LGD,
    LDM_PANEL_AMS,
    LDM_PANEL_IWATT,
    LDM_PANEL_SCT,
    LDM_PANEL_MAX,
} LDM_PANEL_TYPE;

typedef enum {
    LDM_ERR_OK = 0,
    LDM_ERR_NOTFOUND = 1,
    LDM_ERR_TIMEOUT = 2,
} LDM_ERR_TYPE;

enum
{
  SPI_PINMUX_OPTION1,
  SPI_PINMUX_OPTION2,
  SPI_PINMUX_MAX,
};

//macro define  --- need to remove
#define LDM_WRITE32(offset, value)		IO_WRITE32(LDM_BASE, (offset), (value))
#define LDM_READ32(offset)				IO_READ32(LDM_BASE, (offset))
#define LDM_MASK32(offset, value, mask) \
    LDM_WRITE32(offset, (LDM_READ32(offset) & ~mask) | value)



#define LDM_AS3693_ADDR1        0x37
#define LDM_AS3693_ADDR2        0x3F
#define LDM_AS3693_CURREG1      0x12
#define LDM_AS3693_CONREG1      0x01

#define LDM_IWATT_WRITE         0x001
#define LDM_IWATT_CHANNEL1      0x001
#define LDM_IWATT_ADDR1         0x080   //one time register begin
#define LDM_IWATT_ADDR2         0x050   //user mode register begin


#define CKGEN_MASK32(offset, value, mask) \
        CKGEN_WRITE32(offset, (CKGEN_READ32(offset) & ~mask) | value)


//-----------------------------------------------
//pinmux initial function
//----------------------------------------------
UINT8 vDrvLDMHW_setPinmux(UINT8 u1Select);
//-----------------------------------------------
//SPI register initial function
//----------------------------------------------
void vDrvLDMHW_setControl(PANEL_LOCAL_DIMMING_ATTRIBUTE_T* pParam);			
void vDrvLDMHW_setClock(PANEL_LOCAL_DIMMING_ATTRIBUTE_T* pParam); 		  
void vDrvLDMHW_setVsync(PANEL_LOCAL_DIMMING_ATTRIBUTE_T* pParam); 		 
void vDrvLDMHW_setCS(PANEL_LOCAL_DIMMING_ATTRIBUTE_T* pParam);	
void vDrvLDMHW_setFrameRate(PANEL_LOCAL_DIMMING_ATTRIBUTE_T* pParam);
void vDrvLDMHW_setDataLen(PANEL_LOCAL_DIMMING_ATTRIBUTE_T *pParam);	
UINT32 vDrvLDMHW_getDataLen(void);
void vDrvLDMHW_setLSBMode(PANEL_LOCAL_DIMMING_ATTRIBUTE_T* pParam);
UINT8 vDrvLDMHW_getLSBMode(void);
void vDrvLDMHW_setRGBMode(PANEL_LOCAL_DIMMING_ATTRIBUTE_T* pParam);	   
void vDrvLDMHW_setBITMode(PANEL_LOCAL_DIMMING_ATTRIBUTE_T* pParam);	   
void vDrvLDMHW_setLEDType(PANEL_LOCAL_DIMMING_ATTRIBUTE_T* pParam);	   


void vDrvLDMHWInit(UINT8 u1Pinmux, UINT8 u1PanelType);
void vDrvLDMHW_TurnOffSPI(UINT8 u1PanelType);

//-----------------------------------------------
//shared function
//-----------------------------------------------
UINT32 _LDMHW_WaitReady(void);

//-----------------------------------------------
//LGD driver porting
//-----------------------------------------------
void vDrvLDMHW_LightLGD(void);
void vDrvLDMHW_TurnOffLGD(void);
void vDrvLDMHW_LGDTransfer(UINT32 bDataLen, UINT8 *bDimmingData);
void DRVCUST_LDMHW_DataGenLGD(void);

//------------------------------------------------
//AS3693 driver porting
//------------------------------------------------
void vDrvLDMHW_LightAS3693(void);
void vDrvLDMHW_TurnOffAS3693(void);
void _LDMHW_AS3693SPI(UINT8 u1Addr, UINT8 u1Reg, UINT8* au1Data, UINT32 u4Len);
void DRVCUST_LDMHW_DataGenAS3693(void);

//-------------------------------------------------
//iWatt driver porting
//-------------------------------------------------
void vDrvLDMHW_iWattInit(void);
void vDrvLDMHW_TurnOffiWatt(void);
void vDrvLDMHW_iWattTransfer(UINT16 u2Command, UINT16 u2Addr, UINT16* au1Data, UINT32 u4Len);
void DRVCUST_LDMHW_DataGeniWatt(void);

//--------------------------------------------------
//StarChip driver porting
//--------------------------------------------------
void vDrvLDMHW_LightSCT(void);
void vDrvLDMHW_TurnOffSCT(void);
void vDrvLDMHW_SCTTransfer(UINT32 bDataLen, UINT8 *bDimmingData);
void DRVCUST_LDMHW_DataGenSCT(void);

#endif

