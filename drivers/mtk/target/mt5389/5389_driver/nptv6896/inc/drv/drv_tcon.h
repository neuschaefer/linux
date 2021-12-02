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
 * $RCSfile: drv_tcon.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _DRV_TCON_H_
#define _DRV_TCON_H_


/*
MLVDS Bit mode 0xf0035000[7:6]
2'b00 : 6  Bit mode
2'b01 : 8  Bit mode
2'b10 :10 Bit mode
MLVDS Bit mode 0xf0035000[5:2]
MLVDS Pair mode
4'd3 : 3 Pair mode
4'd4 : 4 Pair mode
4'd5 : 5 Pair mode
4'd6 : 6 Pair mode
4'd8 : 8 Pair mode
MLVDS Port mode 0xf0035000[1:0]
2'b00 : 1 Port
2'b01 : 2 Port
2'b10 : 4 Port
*/

#define MLVDS_SINGLE_PORT 1
#define MLVDS_DUAL_PORT 2
#define MLVDS_QUAL_PORT 4

#define MLVDS_3_PAIR 3
#define MLVDS_4_PAIR 4
#define MLVDS_5_PAIR 5
#define MLVDS_6_PAIR 6
#define MLVDS_8_PAIR 8

#define MLVDS_6_BIT 6
#define MLVDS_8_BIT 8
#define MLVDS_10_BIT 10
#define HISENSE_IPS_2010_PANEL 0
//#define SUPPORT_LOCK_FROM_SRVAD 

enum 
{
  TCON_STATE_NOTREADY,
  TCON_STATE_INIT,
  TCON_STATE_STANDBY,
  TCON_STATE_MUTE,
  TCON_STATE_NORMAL,
  TCON_STATE_ERROR,
  TCON_STATE_MAX
};

enum 
{
  TCON_CMD_NONE,
  TCON_CMD_INIT,
  TCON_CMD_GAMMA,
  TCON_CMD_PROGRAM_GAMMA,
  TCON_CMD_STANDBY,
  TCON_CMD_POWERON,
  TCON_CMD_POWEROFF,
  TCON_CMD_MUTE,
  TCON_CMD_UNMUTE,
  TCON_CMD_TP,
  TCON_CMD_REGDUMP,
  TCON_CMD_DMSG,
  TCON_CMD_DLEVEL,	
  TCON_CMD_FW_STOP,
  TCON_CMD_FW_RESUME,
  TCON_CMD_MAX
};

enum 
{
  TCON_TP_OFF,
  TCON_TP_R,
  TCON_TP_G,
  TCON_TP_B,
  TCON_TP_BLACK,
  TCON_TP_WHITE,
  TCON_TP_COLORRAMP,
  TCON_TP_MOVING,
  TCON_TP_AUTO,
  TCON_TP_MAX,
};

enum {
  TCON_DISABLE,
  TCON_ENABLE
};

struct sTCONREGTABLE {
    UINT32 u4Addr;
	UINT32 u4Val;
	UINT32 u4Msk;
};

// flash tcon
#define FLASH_TCON_SIZE (64*1024) //Total Flash AQ size on NAND Flash partition.
#define FLASH_TCON_BUF_SIZE (1024)

void vDrvTconHwInit(void);
void vDrvTconOn(void);
void vDrvTconOff(void);

UINT8 u1DrvTconPreInit(void);
UINT8 u1DrvTconInit(void);

UINT8 u1DrvTconConfig(UINT8 config);
UINT8 u1DrvTconOffOn(UINT8 val);
UINT8 u1DrvTconForceMute(UINT8 val);

UINT8 u1DrvTconIsReady(void);
UINT8 u1DrvTconState(void);

UINT8 u1TconCustTgInit(void);
UINT8 u1TconCustMlvdsInit(void);
UINT8 u1TconCustEPIInit(void);

UINT8 u1TconCustGammaInit(void);
UINT8  u1TconCustGammaFlashing(void);

UINT8 u1TconCustDPMCtrlEN1(UINT8 val);
UINT8 u1TconCustDPMCtrlEN2(UINT8 val);

// for debugging
UINT8 u1DrvTconTP(UINT8 val);
UINT8 u1DrvTconRegDump(void);
UINT8 u1DrvTconRegDiff(void);
void vDrvTconDmsg(void);
UINT8 u1DrvTconDbgCmd(UINT8 cmd);
void u1DrvTconMLDSVOPLLFreqSet(UINT32 u4Freq, UINT8 MLVDS_PORT,UINT8 MLVDS_RATION);
void vDrvMiniLvdsConfig(UINT8 u1port, UINT8 u1pair, UINT8 u1bits);
void vErrorHandleSetByTiming(void);
void vErrorHandleInit(void);
void vErrorHandleStatus(void);
void u1ErrorHandleFixEnable(UINT8 val);
void u1ErrorHandleMuteEnable(UINT8 val);
void vErrorHandleSetByPanel(void);
void vDrvMLVDSPatternGen(UINT8 u1OnOff);
void u1ErrorHandlingSourceSet(UINT8 val);
UINT8 u1ErrorHandlingSourceGet(void);
void vDrvEPISLT(void);
UINT8 vDrvMLVDSCRC(void);

// flash tcon
#ifdef CC_SUPPORT_FLASH_TCON
INT32 i4FlashTconRead(UINT16 u2Offset, UINT8 *u1Data, UINT32 u4Length);
void vFlashTconWrite(UINT16 u2Offset, UINT8 *u1Data, UINT32 u4size);
void vFlashTconInit(void);
void vFlashTconWriteDefault(void);
void vFlashTconSave(void);
struct sTCONREGTABLE * pTconCustTgGet(UINT32 *u4Length);
#endif  /*#ifdef CC_SUPPORT_FLASH_TCON*/
#endif
#ifdef SUPPORT_LOCK_FROM_SRVAD
void vDrvTconOutVsyncISR(void);
#endif

//#ifdef CC_SMALL_TCON_BOARD
void vTconCustTGPrint(void);
void vTconCustTGLoadEEP(void);
UINT8 tcon_DTV(void);  
UINT8 tcon_Vcom_Read(void);
UINT8 tcon_Vcom_Write(UINT8 val);
//#endif
