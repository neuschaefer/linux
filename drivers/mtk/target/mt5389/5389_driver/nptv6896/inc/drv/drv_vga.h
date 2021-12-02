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
 * $Author: hongjun.chu $
 * $Date: 2012/04/28 $
 * $RCSfile: drv_vga.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/


#ifndef _DRV_VGA_H_
#define _DRV_VGA_H_

#include "general.h"
#include "hw_vdoin.h"
#ifdef CYJ_20090106
#include "x_tv_dec.h"
#endif

/*======================================================
              crystal number
  ====================================================*/
/*#define CRYSTAL_27M                27
#define CRYSTAL_40M                40

#define CRYSTAL                    CRYSTAL_27M*/

/*========================================================================
             SOURCE
  ========================================================================*/
#define  NO_SIGNAL 0
/*
#define  SEPERATE 0
#define  COMPOSITE 1
#define  SOG 2
*/
/*========================================================================
            Timing Judge Range
  ========================================================================*/
#define HSYNC_UPPER_BD           8 //5	//11
#define HSYNC_LOWER_BD          8 //6	//11
#define VSYNC_UPPER_BD            3
#define VSYNC_LOWER_BD           3

#define VSYNC_INVILID                 7	//Hz

/*=====================================================================
          PROCEDURE DEFINE
  ====================================================================*/
/*enum {
	ZERO,
	NOT_FINISHED,
	FINISHED,
	VALID,
	INVALID
};*/

typedef struct VGAWeight		// 2bytes
{
	UINT8 TimingIndex; // Vertical Frequency for timing search
	UINT8 Weight; // Pixel Frequency
}  VGAWeight ; // using __attribute__((packed)) make armcc --gnu  internal fault ?
/*=====================================================================
          MODE Detection State Machine Define
  ====================================================================*/
  enum{
        VGAMD_GET_ACTIVESYNC,
        VGAMD_GET_MODE_N_VTOTAL,
        VGAMD_GET_HTOTAL_N_STDTIME,
        VGAMD_SET_INPUT_CAPTURE,
  	VGAMD_DETECTDONE,
  	VGAMD_AMBCHK,
  	VGAMD_DELAY0,
  	VGAMD_DELAY1,
  	VGAMD_DELAY2,
  	VGAMD_DELAY3,  	
};

/*=====================================================================
          PROCEDURE DEFINE
  ====================================================================*/
#define NEGATIVE   0
#define POSITIVE    1

extern UINT8   _IsVgaDetectDone;
extern UINT8   _IsHdtvDetectDone;
extern UINT8   _bVgaTiming;
extern UINT16   wSP0Hclk;
extern UINT8   _bVgaUserMode;
extern BOOL _RETIMENeedReset;



extern void vVgaConnect(UINT8 bChannel, UINT8 fgIsOn) ;
extern UINT16 wVgaInputWidth(void) ;
extern UINT16 wVgaInputHeight(void) ;
extern UINT8 bVgaRefreshRate(void) ;
extern UINT8 bVgaInterlace(void) ;
extern UINT8 bVgaSigStatus(void) ;
extern void vVgaISR(void) ;

extern void vVgaModeDetect(void) ;
extern void vVgaChkModeChange(void) ;
extern void vVgaSetInputCapature(UINT8 bmode);

#define vVgaSetInput(wstart, wwidth)	vHdtvSetInput(wstart, wwidth)
#define vVgaClampWin( wclamp_start, wclamp_end) 	vHdtvClampWin( wclamp_start, wclamp_end) 
#define vVgaReset()	vHdtvReset()
////////////////////////////////////////////////////////////////////////////////
// for CLI */
void vVgaSwReset(void);
void vVgaStatus(void);
UINT8 bVgaCurPhase(void);

/* VGA options ------------------------------*/
extern UINT8 bVgaOpt01_MDmute0; //for MD , waiting lock mute
extern UINT8 bVgaOpt01_MDmute1; //for MD , after lock stable
extern UINT8 bVgaOpt02_MCmute0; //for MC , waiting stable mute
extern UINT8 bVgaOpt02_MCmute1; //for MC , waiting stable mute
extern UINT8 bVgaOpt02_MCmute2; //for MC , waiting stable mute
extern UINT8 bVgaOpt02_MCmute3; //for MC , waiting stable mute
extern UINT8 bVgaOpt03_SoGen; //for MC , waiting stable mute
extern UINT8 bVgaOpt04_SearchHDTV; //for MD, std timing search
extern UINT8 bVgaOpt05_SearchNewMode; //for MD, new mode & user timing search
extern UINT8 bVgaOpt06_SogMaxVsyncWidth ;
extern UINT8 bVgaOpt07_AutoKeepOldVal;

/* VGA options ------------------------------*/

#define QUERY_CUR_AMBIGUOUS_TIMING 0

typedef struct VGAAmbTimingInfo_s
{
    UINT8  bCandiIdx;
    UINT8  bVer_freq;
    UINT16 wHor_res;
    UINT16 wVer_res;

}VGAAmbTimingInfo_t;

/**
 * Enable/Disable to set the ambiguous vga mode with the previous user select
 * mode
 *
 * @param fgUserSel SV_FALSE to disable, otherwise to enable
 * @return none
 */
void vVgaSetAmbiguousUserSel(BOOL fgUserSel);

/**
 * Get # of items of ambiguous VGA in current VGA timing group
 *
 * @param NONE
 * @return # of items of ambiguous VGA timing
 */
UINT8 bVgaGetAmbiguousTimingNum(void);

UINT8 bVgaForceSyncMode(UINT8 bForceSyncMode);

UINT8 u1VGA_GetEdidData(UINT8 *pData, UINT16 u2Length);
/**
 * Get current sync mode
 *
 * @param none
 * @return the current sync mode.
 */
UINT8 bVgaGetCurrSyncMode(void);

/**
 * Query certain or current VGA timing details.
 *
 * @param pt_ambig_vga_timing_info structure pointer of struct type "TV_DEC_AMBIG_VGA_TIMING_INFO_T"
 * @return NONE. It will return 1. index in current group, 2. V freq, 3. H resolution, 4. V resolution
 */
void vVgaQueryAmbiguousTiming(UINT8 bCandiIdx, VGAAmbTimingInfo_t *rAmbTiming, UINT8 bSize);

/**
 * Set VGA timing according to input structure pointer.
 *
 * @param pt_vga_timing_option structure pointer of struct type "TV_DEC_VGA_TIMING_OPTION_T"
 * @return NONE.
 */
void vVgaSetAmbiguousTiming(UINT8 bCandiIdx, BOOL fgSetAsDefault);

/**
 * Reset EEPROM of VGA timing to default value.
 *
 * @param NONE.
 * @return TRUE is successful, FALSE has failure.
 */
BOOL vVgaResetAmbiguousTimingEEPROM(void);

void vVgaTriggerModeChange(void);


#endif

