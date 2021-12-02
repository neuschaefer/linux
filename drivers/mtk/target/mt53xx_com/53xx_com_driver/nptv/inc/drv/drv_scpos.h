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

/*************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: drv_scpos.h $
 *
 * Project:
 * --------
 *
 *
 * Description:
 * ------------
 *
 *
 * Author:
 * -------
 *
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 *
 * $Modtime: 04/05/28 9:33a $
 *
 * $Revision: #1 $
*************************************************************************/

#ifndef _DRV_SCPOS_H_
#define _DRV_SCPOS_H_

#include "hw_scpos.h"
#include "debug.h"
#include "feature.h"

#ifdef CC_COPLAT_MT82
extern BOOL _fgPIP,_fgLRPOP;
#endif
extern UINT32 u4ForceFrameTrackCtrl;
extern UINT32 u4ForceFrameTrackMode;

extern UINT8 bScpipMirrorEnable[];
extern UINT8 bScpipFlipEnable[];

#define fgMain444Mode() ((RegReadFld(SCPIP_DS1_02, DS1_02_SEL444_1) > 0)? TRUE: FALSE)
#define fgMainWriteOn() ((RegReadFld(SCPIP_PIP1_09, PIP1_09_W_EN_1) > 0)? TRUE: FALSE)

//#define vDrvMainSetWriteControl(bOnOff)  (vRegWriteFldAlign(SCPIP_PIP1_09, bOnOff, PIP1_09_W_EN_1))
//#define vDrvMainSetReadControl(bOnOff)  (vRegWriteFldAlign(SCPIP_PIP1_09, bOnOff, PIP1_09_R_EN_1))

#define fgPIP444Mode() ((RegReadFld(SCPIP_DS2_02, DS2_02_SEL444_2) > 0)? TRUE: FALSE)
#define fgPIPWriteOn() ((RegReadFld(SCPIP_PIP2_09, PIP2_09_W_EN_2) > 0)? TRUE: FALSE)

//#define vDrvPIPSetWriteControl(bOnOff)  (vRegWriteFldAlign(SCPIP_PIP2_09, bOnOff, PIP2_09_W_EN_2))
//#define vDrvPIPSetReadControl(bOnOff)  (vRegWriteFldAlign(SCPIP_PIP2_09, bOnOff, PIP2_09_R_EN_2))
#define vDrvMainUpdateVBound(u2Vline) (vRegWriteFldAlign(SCPIP_US1_14, u2Vline, US1_14_UP_VBOUND_Y))
#define vDrvMainImportOnOff(bOnOff) (vRegWriteFldAlign(SCPIP_PIP1_09, bOnOff, PIP1_09_IMPORT_1))
#define vDrvPIPImportOnOff(bOnOff) (vRegWriteFldAlign(SCPIP_PIP2_09, bOnOff, PIP2_09_IMPORT_2))

#define vDrvMainMute(bOnOff) (vRegWriteFldAlign(OUTSTG_OS_17, bOnOff, OS_17_MUTE_MAIN))
#define vDrvPIPMute(bOnOff) (vRegWriteFldAlign(OUTSTG_OS_17, bOnOff, OS_17_MUTE_PIP))

#define fgMainMuteOn() (RegReadFld(OUTSTG_OS_17, OS_17_MUTE_MAIN))
#define fgPIPMuteOn() (RegReadFld(OUTSTG_OS_17, OS_17_MUTE_PIP))

typedef enum
{
    QUICK_BLANK_NO_FUNCTION = 0,
    QUICK_BLANK_BY_MAIN = 2,
    QUICK_BLANK_BY_PIP = 3,
}E_QUICK_BLANK_MODE;

typedef enum
{
    SCPIP_FRAME_TRACK_DISABLE =0,
    SCPIP_FRAME_TRACK_LOCK =1,
    SCPIP_FRAME_TRACK_NOT_STALBE =2,
}E_FRAME_TRACK_STAUS;

typedef enum
{
    SCPIP_CLK_TYPE_VDOIN = 0x0,
    SCPIP_CLK_TYPE_OCLK  = 0x1,    
    SCPIP_CLK_TYPE_PS_DIV_CLK = 0x2,
    SCPIP_CLK_TYPE_TVE27_CK = 0x3,
}SCPIP_CLOCK_TYPE;

#define SCPIP_FRAME_TRACK_LOCK_RANGE    2
#define SCPIP_FRAME_TRACK_LOSS_LOCK_RANGE    5
#define SCPIP_FRAME_TRACK_STEP_SMALL 1
#define SCPIP_FRAME_TRACK_STEP_LARGE 20

#if 0
typedef enum
{
    SCPIP_FRAME_TRACK_AUTO=0,
    SCPIP_FRAME_TRACK_CLI_ONOFF=1,  // force off with cli
    SCPIP_FRAME_TRACK_OFF_DS_PATTERN=2,  // force off when down scaling pattern enable
}E_FRAME_TRACK_CTRL;
#else
#define SCPIP_FRAME_TRACK_AUTO 0
#define SCPIP_FRAME_TRACK_ON_CLI ((UINT8)1 << 1)
#define SCPIP_FRAME_TRACK_OFF_CLI ((UINT8)1 << 2)
#define SCPIP_FRAME_TRACK_OFF_DS_PATTERN ((UINT8)1 << 3)
#define SCPIP_FRAME_TRACK_OFF_V_PORCH ((UINT8)1 << 4)
#endif

#if SUPPORT_HDTV_HARDWARE_MUTE
void vDrvConfigHardwareMute(void);
void vDrvConfigQuickBlank(E_QUICK_BLANK_MODE mode);
void vDrvConfigQuickMute(UINT8 bPath,UINT8 mode);
#endif

// drv_scpos.c
extern void vScposInit(UINT8 bPath);
extern void vDrvSetSCPIPClock(UINT8 bPath, SCPIP_CLOCK_TYPE eType);
extern void vScpipPScanDispmodeWaterCtrl(UINT8 bPath, UINT8 bOnOff);
extern UINT8 vScpipGetDispmodeLineBufNum(UINT32 bPath);

//Frame Track functions for input/output V sync alignment
extern UINT32 u4GetFrameTrackMode(void);
extern void vScpipFrameTrackStep(UINT8 bStep);
extern void vFrameTrackConfig(UINT32 u4LockRange, UINT32 u4LossLockRange, UINT32 u4Step);
extern void vSwitchFrameTrackOnOff(UINT32 u4OnOff);
extern UINT32 u4GetFrameTrackOnOff(void);
extern E_FRAME_TRACK_STAUS  fgGetFrameTrackStatus(void);

//extern void vDrvSetBgMuteColor(void);
//extern void vSetMainMuteColor(UINT8 bMuteColor);
//extern void vSetPipMuteColor(UINT8 bMuteColor);
extern void vDrvUVDelayCtrl(UINT8 bUVDelay);
extern void vDrvMuteRelatedChk(UINT8 bPath);
//extern void vSetMuteColor(UINT8 R, UINT8 G, UINT8 B);
extern void vScposSetMuteMask(UINT8 bPath, UINT8 bOnOff, UINT32 u4Mask);
#if CC_SCPOS_MUTE_RELATCH_WA
UINT32 u4DrvScposGetHwMuteStatus(UINT8 bPath);
void vDrvScposSetHwMuteStatus(UINT8 bPath, UINT8 u1OnOff);
#endif

extern void vDrvOutRGBColorOnOff(UINT8 bOnOff);
extern void vDrvOutRGBColor(UINT8 RValue,UINT8 GValue,UINT8 BValue);

extern UINT32 dwDrvGetMuteMask(UINT8 bMode);
extern void vScposMuteMaskOnOff(UINT8 bPath, UINT32 u4OnOff);

extern UINT32 _dwDrvSetScalerDRAM(UINT32 u4VdpId, UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height, UINT32 u4GBRYCbCr);

extern UINT8 fgApiVideoStable(UINT8 u1VideoFrame);
extern void vDrvSetOSTGInPattern(UINT32 u4VdpId, UINT32 u4OnOff);
extern void vDrvSetOSTGOutPattern(UINT32 u4OnOff);

extern void vScpipSetMirror(UINT8 bPath, UINT8 bOnOff);
extern void vScpipSetFlip(UINT8 bPath, UINT8 bOnOff);
extern void vScpipSetMirrorFlipAll(UINT8 bOnOff);


typedef struct
{
    UINT32 dwAddr;
    UINT32 dwSize;
    UINT32 dwWidth;	
    UINT32 dwHeight;	    
    UINT32 dwWINC;    
    UINT16 u2W_POS;	    
    UINT16 u2R_POS;	    
    UINT32 dwMHL;
    UINT32 dwMVL;
    UINT8 bIs444;
    UINT8 bIs10bit;
}tagDramDumpInfo_T;

void vGetScalerDRAMInfo(tagDramDumpInfo_T *rDramDumpInfo);

#endif
