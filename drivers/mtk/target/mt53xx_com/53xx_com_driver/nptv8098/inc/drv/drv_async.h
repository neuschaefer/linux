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
 * $RCSfile: drv_async.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _DRV_ASYNC_H_
#define _DRV_ASYNC_H_

#include "typedef.h"
#include "hw_vdoin.h"
#include  "drv_vdoclk.h"
#include "hw_async.h"

#define PIP5382Enable		(SUPPORT_POP)
#define SUPPORT_82xx_ASYNC		0

#define HDTV_MV_DETECT_SUPPORT 1
#define ASYNC_FULL_SCREEN_WA	1
#define VSYNC_OUT_UPDATE_TRIGGER 1
/* ====================================================================
      SYNC TYPE SELECT
  ====================================================================*/
 #define SEPERATESYNC       0
 #define COMPOSITESYNC     1
 #define SYNCONGREEN        2

#if SUPPORT_HDTV_HARDWARE_MUTE
/*=====================================================================
      HARDWARE MUTE TYPE
  ====================================================================*/
#define AS_MUTE_HSACT (1<<6)
#define AS_MUTE_VSACT (1<<5)
#define AS_MUTE_CSACT (1<<4)
#define AS_MUTE_HSPOL (1<<3)
#define AS_MUTE_VSPOL (1<<2)
#define AS_MUTE_HLEN  (1<<1)
#define AS_MUTE_VLEN  (1<<0)
#endif

/*=====================================================================
            CS SEPARATOR THRESHOLD SELTECT
  ====================================================================*/
#define CS_SEPARATOR_THU_DEFAULT   0x01a4
#define CS_SEPARATOR_THL_DEFAULT   0x00ff
#define CS_SEPARATOR_THU_DEFAULT_1080   0x01c4
#define CS_SEPARATOR_THL_DEFAULT_1080   0x00f0
#define CS_SEPARATOR_THH_DEFAULT	0x19c

#define vASPathReset()	vRegWriteFldAlign(ASYNC_00,SEPERATESYNC,AS_SYNC_SEL)
#define vASSetSSync()	vRegWriteFldAlign(ASYNC_00,SEPERATESYNC,AS_SYNC_SEL)
#define vASSetCSync()	vRegWriteFldAlign(ASYNC_00,COMPOSITESYNC,AS_SYNC_SEL)
#define vASSetSOGSync()	vRegWriteFldAlign(ASYNC_00,SYNCONGREEN,AS_SYNC_SEL)

//#define vASDeCompSel(bOn)	vRegWriteFldAlign(ASYNC_01, bOn, AS_DE_COMP_SEL)

#define vDrvCsyncInvPol(bPol) vRegWriteFldAlign(ASYNC_00,bPol,AS_CSYNC_INV)
#define vDrvHsLockInv(bOn) vRegWriteFldAlign(ASYNC_0E, bOn, AS_HSYNC_LOCK_INV)
#define vDrvHsInv(bOn) vRegWriteFldAlign(ASYNC_00,bOn,AS_HSYNC_INV)
#define vDrvVsInv(bOn) vRegWriteFldAlign(ASYNC_00,bOn,AS_VSYNC_INV)
#define vDrvVsOutInvPol(bPol) vRegWriteFldAlign(ASYNC_11,bPol,AS_VSYNC_OUTP_INV)

#define bDrvASFieldAct()  (RegReadFldAlign(STA_SYNC0_00, AS_FIELD_ACT))
#define bDrvASFieldNum()  (RegReadFldAlign(STA_SYNC0_00, AS_FIELD_CK27_DET))

#define fgIsCLKLock() (RegReadFldAlign(VFE_STA_00,DDS_LOCK))
#define vDDS_MAX_PERR() (RegReadFldAlign(VFE_STA_00,DDS_MAX_PERR))
#if 	SUPPORT_82xx_ASYNC
#define vDrvAsStatHold(bOn)	vRegWriteFldAlign(ASYNC_02, bOn, AS_LONG_LENGTH_STATUS_HOLD);
#define vDrvDDSCwGate(bOn) vRegWriteFldAlign(SYS_00,bOn,DDS_CW_GATE)
#else
#define vDrvAsStatHold(bOn)
#define vDrvDDSCwGate(bOn)
#endif



// SP0 Event Flag
#define vSetSP0Flg(arg) (_bSP0Flag |= (arg))
#define vClrSP0Flg(arg) (_bSP0Flag &= (~(arg)))
#define fgIsSP0FlgSet(arg) ((_bSP0Flag & (arg)) > 0)

#define SP0_MODE_DET_FLG (1<<0)
#define SP0_VGA_AUTO_FLG (1<<1)
#define SP0_AUTOCOLOR_FLG (1<<2)
#define SP0_MCHG_BYPASS_FLG (1<<7)


#if HDTV_MV_DETECT_SUPPORT
extern void vDrvAsyncMvDetectH (UINT16 wstart1, UINT16 wend1) ;
extern void vDrvAsyncMvDetectV (UINT16 wstart1, UINT16 wend1)  ;
extern BYTE bDrvAsyncMvStatus (void)  ;
extern UINT8 _bHdtvMvOn ;
#endif
void vDrvAsyncHsyncEnable(UINT8 bEnable);


enum{	
            HDTV_NO_SIGNAL,	
            HDTV_CHK_MODECHG,	
            HDTV_WAIT_STABLE
         };

enum{	
            VGA_NO_SIGNAL,	
            VGA_CHK_MODECHG,	
            VGA_WAIT_STABLE
         };


enum{	
            MCHG_NO_CHG = 0,
            MDCHG_CON =1,	
            MCHG_NOSIG =2 ,	 
            MCHG_SIGIN = 3,
            MCHG_UNLOCK =4,
            MCHG_ISR =5,
            MCHG_HVSYNC_LOSE =6,
            MCHG_POL_CHG =7,
            MCHG_HVLEN_CHG =8,
            MCHG_FLD_CHG =9,
            MCHG_HW_DET =10,
			MCHG_VGA_422=11
         };   
#define DBG_MDET	1
#define DBG_MCHG     2
#define DBG_AUTO     4

//#define VGA_DBG_MSG_DMP 1
extern UINT8   bVgadbgmask;

#ifndef CC_UP8032_ATV   
#ifndef NDEBUG
    #define DBG_Printf(msk,fmt...) if(bVgadbgmask&(msk)) Printf(fmt)
#else
    #define DBG_Printf(msk,fmt...)
#endif
#endif
/*======================================================
              crystal number
  ====================================================*/
#define CRYSTAL_27M               27
#define CRYSTAL_40M                40

#define CRYSTAL                    CRYSTAL_27M


/*======================================================
              H/V Sync Internal Delay
  ====================================================*/
#define HSync_Auto_Delay 8

#if ADAPTIVE_MONITOR_SLICER_MEASURE
typedef struct // SP2MonStr
{
	UINT16 SP2_V_LEN_S;
	UINT16 SP2_H_LEN_S;
	UINT16 SP2_V_WIDTH_S;
}  SP2MonStr;

typedef enum
{
	SP2_Specific_LVL,
	SP2_Default_LVL
}  SP2Mon_LVL;

#endif

enum 
{
	ZERO,
	NOT_FINISHED,
	FINISHED,
	VALID,
	INVALID
};

enum 
{
	DOMAIN_27MHz,
	DOMAIN_PIXEL
};

#define DBG_MDET	1
#define DBG_MCHG     2
#define DBG_AUTO     4
#define DBG_SLICER    8 

#define VGA_DBG_MSG_DMP 1
extern UINT8   bVgadbgmask;

#define PRE_DOWN_OFST   2

// SP0 Extern  
extern UINT16   wSP0Hclk;
extern UINT8   bSP0Vclk;
extern UINT16   wSP0Vtotal;
extern UINT16  _wSP0StableVtotal;
extern UINT16   wSP0HLength;
extern UINT8   bSP0VCount;
extern UINT8   bSP0SigChk;
extern UINT8   fgSP0DIChk;
extern UINT8   fgSP0Hpol;
extern UINT8   fgSP0Vpol;
extern UINT16   wSP0VCompare[4];
extern UINT16   wSP0HCompare1;
extern UINT8  _bSP0Flag ;

extern UINT8   bSP0TrySignalState;
extern UINT32 wVGAADSpec;
extern UINT32 wHFHeight;
extern UINT32 wHFLow;
extern UINT32 wVFHeight;
extern UINT32 wVFLow;
extern UINT8 bModeIndex;

//////////////////////////////////////////////////////////////////////////////////
extern void vResetVLen(void) ;

extern UINT8 fgASHPolarityMeasure(void) ;
extern UINT8 fgASVPolarityMeasure(void) ;
extern UINT16 wASHLenMeasure(void) ;
extern UINT16 wASHSyncWidthMeasure(void);
extern UINT16 wASVtotalMeasure(void) ;
#if ADAPTIVE_MONITOR_SLICER_MEASURE
#define wSP2VCompare_Num 2
extern SP2MonStr wSP2VCompare[wSP2VCompare_Num];
extern void vAS2SyncMeasure(SP2Mon_LVL type);
#endif
extern UINT16 wASTopBCLine(void);
extern UINT16 wASBottomBCLine(void);
extern UINT8 bASActiveChk(void) ;
extern UINT8 bASHDTVActiveChk(void);
extern void vASCSSeparatorThre(void);
extern void vSetAsyncMeasureBD(UINT8 bmode) ;
extern void vDrvAsyncVMask(UINT16 wstart1, UINT16 wend1) ;
extern void vDrvAsyncVMask2(UINT16 wstart1, UINT16 wend1) ;
extern void vDrvAsyncClampMask(UINT16 wstart1, UINT16 wend1) ;
extern void vDrvAsyncPreDataActive(UINT16 wstart,UINT16 wend);
extern void vDrvAsyncHBDMask(UINT16 Left, UINT16 Right);
extern void vDrvAsyncBDMask(UINT16 wstart1, UINT16 wend1)  ;
extern void  vDrvAsyncSetFieldDet(UINT16 wHLEN) ;


extern void vDrvAsyncVsyncStart(UINT16 wStart) ;
extern void vDrvAsyncVsyncOut(UINT16 wStart1, UINT16 wH1);

extern UINT16 wSP0IHSClock(UINT16 whtotal) ;
extern UINT8 bSP0IVSClock(UINT16 whtotal, UINT16 wvtotal) ;
extern UINT8 bDrvAsGetActive(UINT8 VSyncNo) ;
extern UINT8 bDrvAsGetVtotal(void) ;
extern UINT8  fgSP0NewSync(void) ;

extern void SP0Initial(void) ;
extern UINT8 SP0SignalIdentify(UINT8 SigMode) ;
extern UINT8 bSP0VsyncValid(void)  ;
extern UINT16 wASHSyncWidthMeasure(void) ;
extern UINT16 wASVSyncWidthMeasure(void);
//extern UINT16 wDrvAsyncGetVactive (void) ;
extern void vASDeCompSel(UINT8 DeCompSel);

#if SUPPORT_HDTV_HARDWARE_MUTE
void vDrvAsyncSetMuteCriteria(UINT32 u4MuteFlag);
#endif

extern void vDrvRETIMEReset(void);

#endif

