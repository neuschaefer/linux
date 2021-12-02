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
 * $RCSfile: vga_auto.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


#ifndef _VGA_AUTO_H_
#define _VGA_AUTO_H_

/******************************************************************************
 * Header Files
 *****************************************************************************/
#include "typedef.h"

/******************************************************************************
 * Macro, Type Definition, and Variable Declaratoin
 *****************************************************************************/
 // vga debug flag
#define VGA_AUTO_DBG_MSG  	1
#define VGA_AUTO_UI_DBG	1
#define VGA_AUTO_CLK_DBG	1
#define VGA_AUTO_PHASE_DBG 1
#define VGA_AUTO_POS_DBG	1   

#define NOTIFY_PROGRESS 1


#define VGA_AUTO_SPEEDUP 1

#define VGA_AUTO_CLK_SKIP_TABLE_HTOTAL  0


//#define  VGA_HV_AUTO_CENTER 1

#define fgIsAutoFlgSet(arg) ((_bAutoFlag & (arg)) > 0)
#define vSetAutoFlg(arg) (_bAutoFlag |= (arg))
#define vClrAutoFlg(arg) (_bAutoFlag &= (~(arg)))

#define SP0_AUTO_POSITION (0x1)
#define SP0_AUTO_PHASE (0x1<<1)
#define SP0_AUTO_CLOCK (0x1<<2)
#define SP0_AUTO_POSSET (0x1 << 3)
#define SP0_AUTO_ALL (0x0F)
#define SP1_AUTO_POSITION (0x1<<4)
#define SP1_AUTO_PHASE (0x1<<5)
#define SP1_AUTO_CLOCK (0x1<<6)
#define SP1_AUTO_ALL (0x70)


#define BDTHRSH 86 //modified for 5372,gbsh,20060523
//#define VGA_H_OFST 2 // 1 modify by gellmann for 1600x1200
#define VGA_H_OFST 1 // for mt5360 by Tadd
#define VGA_V_OFST 2 // add 1 for omux align , minus 1 for hdtv align
#define VGA_CLK_PH_THRE 0x4000

#ifndef CC_UP8032_ATV
#define LogS(x) DBG_Printf(DBG_AUTO, x "\n")
#define LogSB(str,w) DBG_Printf(DBG_AUTO, str "\t%d\t(0x%04x)\n",(unsigned char)w,(unsigned char)w)
#define LogSW(str,w) DBG_Printf(DBG_AUTO, str "\t%d\t(0x%04x)\n",(unsigned int)w,(unsigned int)w)
#define LogD(w) DBG_Printf(DBG_AUTO, "\t%d\t(0x%08x)\n",(unsigned int)w,(unsigned int)w)
#endif

//SP0
#define SP0_H 1
#define SP0_V  (1<<1)
#define IsSp0SetAuto(flg)   ((_fgIsAuto0PosFlg&(flg))>0)
#define vSetSp0Auto(flg)    (_fgIsAuto0PosFlg |= (flg))
#define vClrSp0Auto(flg)	(_fgIsAuto0PosFlg &= (~(flg)))
#define vSetSP0AutoState(bState)	(_bVdoSP0AutoState = bState)
#define bGetSP0AutoState()	(_bVdoSP0AutoState)


enum {
	AUTO_FINISHED,
	AUTO_CONTINUED
};

enum {
	MIX_STA,
	PSNE_ONLY	
};

enum {
	PSNE1_ADD,
	PSNE2_ADD,
	PSNE4_ADD,
	PSNE8_ADD	
};

//---------------------------------------
// for global variable initialization
extern UINT8   _bAutoFlag;
extern UINT8   _bVdoSP0AutoState;

extern UINT16   _wVgaClock;

extern UINT8   _bVgaDelayCnt;

extern UINT16   _wVgaVStart; 
extern UINT16   _wVgaVEnd;  
extern UINT16   _wVgaHStart;
extern UINT16   _wVgaHEnd;
//extern UINT8   _bBDChnIdx  ;

extern UINT8   _fgIsAuto0PosFlg;
extern UINT8 _bWaitHwRdy ;
extern UINT8	_bCurPhase;
extern UINT8  _bBestPhase;
extern UINT32	_dBestSum;

//---------------------------------------

extern void vVdoSP0AutoState(void) ;
extern void vDrvVGASetPhase(UINT8 bVal) ;
extern void vDrvSetHPosition(UINT16 wValue);
extern void vDrvSetVPosition(UINT16 wValue);
extern BOOL fgDrvAutoEepWriteWord(UINT16 wAddr, UINT16 wData) ;
extern void vVgaAutoInit(void)  ;
extern void vVgaAutoPosInit(void);

#if VGA_AUTO_SPEEDUP
extern UINT8 _bAutoISR ;
extern void vVgaAutoClkIsr(void) ;
//    extern void vYPbPrPhaseIsr(void) ; //YPbPr Auto Phase 2006/11/07
//    extern void vVgaPhaseIsr(void) ;
#endif

#endif
