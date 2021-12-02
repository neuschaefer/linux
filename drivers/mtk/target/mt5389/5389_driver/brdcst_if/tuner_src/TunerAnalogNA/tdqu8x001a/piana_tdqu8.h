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
 * $RCSfile: piana_tdqu8.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/**
 * @file piana_tdqu.h
 *
 **/

#ifndef _PIANA_TDQU_H_
#define _PIANA_TDQU_H_

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
//#include "PI_IFDemod.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
/**  Tuner I2C Address*/
#define TUNER_ONE_ADDR_TUNER        0xC2
#define TUNER_ONE_ADDR_IF           0x86
/**   Tuner different band range  */
/* 5371_driver\nptv\inc\custom\demo\us\vendor.h */
//#define CONFIG_PHILIPS_FM1236MK3    1
//#define TUNER_ONE_ADDR_TUNER    0xC2
//#define TDQU_ANA_INIT_FREQ   55000       // Khz
//#define TDQU_ANA_PIC_IF      45750       // Khz
//#define TDQU_ANA_FREQ_STEP   625         // 100hz

//#define TDQU_ANA_UNLOCK      0
//#define TDQU_ANA_LOCK        1

#if 0
typedef struct _TDQU_ATUNER_CTX_T
{ 
// Hardware Configure
    UINT8       u1DEMOD_I2c_Addr;
    UINT8       u1RF_I2c_Addr;
    UINT16      u2PIC_If_Freq;
    UINT16      u2LO_Step;
    INT32       u4Frequency;
    UINT8       u1DEMOD_state;
    UINT8       u1AnaMode;
    UINT8       u1VIFLock;
    char        aSW_Ver_HW_Ver[24];
    UINT8       u1IF_I2cAddr;
// Access Link
    void        *pDigiTunerCtx;
    x_break_fct isBreak;
// TV Color / Sound Sub-System
    UINT8       u1SubSysIdx;
} sTDQU_ATUNER_CTX_T, *psTDQU_ATUNER_CTX_T;
#endif

//jackson need refine this. move from tvsys_info.h to here
#define SCAN_TINY_STEP          0x02    // 125khz
#define SCAN_SMALL_STEP         0x04    // 250khz
#define SCAN_MIDDLE_STEP        0x08    // 500khz
//#define SCAN_LARGE_STEP         0x10    // 1.0Mhz
//#define SCAN_QUICK_STEP         0x20    // 2.0Mhz
//#define SCAN_NEXT_CH_STEP       0x30    // 3.0Mhz

/** TunerDev Parameters */
#define SCAN_DIRECTION_DEC                0
#define SCAN_DIRECTION_INC                1

// Every Tuner have one
#define TV_FINE_TUNE_THRESHOLD  0x48
#define TV_FINE_TUNE_THRESHOLD_KHZ  4500    // -2.25MHz ~  +2.25MHz
#define SCAN_TINY_STEP_KHZ          125
#define SCAN_SMALL_STEP_KHZ         250
#define SCAN_MIDDLE_STEP_KHZ        500
//#define SCAN_LARGE_STEP_KHZ         1000
//#define SCAN_QUICK_STEP_KHZ         2000
//#define SCAN_NEXT_CH_STEP_KHZ       3000

//#define TUNER_I2C_RETRY         10
#define SIF_NPTV_CLOCK          270

enum
{
    TUNER_PLL_LOCK_STATE = 0,
    TUNER_VIF_LOCK_STATE
};
#if 0
/* 5371_driver\nptv\inc\api_tvtuner.h */
enum TUNER_TYPE_LIST
{
    TUNER_SWITCH_PHILIPS_1236MK3 =0,
    TUNER_SWITCH_PHILIPS_1236MK5,
    TUNER_SWITCH_PHILIPS_1216MEMK3,
    TUNER_SWITCH_PHILIPS_1216MEMK5,
    TUNER_SWITCH_OTHER
};

/***********************************************************************/
#if CONFIG_PHILIPS_FM1216ME
  #define TUNER_RANGE_LOW     0x054A  /*  45.725 MHz */ 
  #define TUNER_RANGE_HIGH    0x3862  /* 863.225 MHz */ 
  #define TUNER_LOWBAND_MIN   0x054A  /*  45.725 MHz -- Ch A for Ireland*/ 
  #define TUNER_LOWBAND_MAX   0x0C4E  /* 157.975 MHz */ 
  #define TUNER_MIDBAND_MIN   0x0C6E  /* 159.975 MHz */ 
  #define TUNER_MIDBAND_MAX   0x1E0E  /* 441.975 MHz */ 
  #define TUNER_HIGBAND_MIN   0x1E0E  /* 441.975 MHz */ 
  #define TUNER_HIGBAND_MAX   0x3862  /* 863.225 MHz */ 
#elif CONFIG_PHILIPS_FM1216PN
  #define TUNER_RANGE_LOW     0x054A  /*  45.725 MHz */ 
  #define TUNER_RANGE_HIGH    0x3862  /* 863.225 MHz */ 
  #define TUNER_LOWBAND_MIN   0x054A  /*  45.725 MHz -- Ch A for Ireland*/ 
  #define TUNER_LOWBAND_MAX   0x0C4E  /* 157.975 MHz */ 
  #define TUNER_MIDBAND_MIN   0x0C6E  /* 159.975 MHz */ 
  #define TUNER_MIDBAND_MAX   0x1E0E  /* 441.975 MHz */ 
  #define TUNER_HIGBAND_MIN   0x1E0E  /* 441.975 MHz */ 
  #define TUNER_HIGBAND_MAX   0x3862  /* 863.225 MHz */ 
#elif CONFIG_PHILIPS_FM1236MK3
  #define TUNER_RANGE_LOW     0x0650  /*  55.25 MHz */ 
  #define TUNER_RANGE_HIGH    0x34F0  /* 801.25 MHz */ 
  #define TUNER_LOWBAND_MIN   0x0650  /*  55.25 MHz */ 
  #define TUNER_LOWBAND_MAX   0x0CDC  /* 160.00 MHz */ 
  #define TUNER_MIDBAND_MIN   0x0CDC  /* 160.00 MHz */ 
  #define TUNER_MIDBAND_MAX   0x1E7C  /* 442.00 MHz */ 
  #define TUNER_HIGBAND_MIN   0x1E7C  /* 442.00 MHz */ 
  #define TUNER_HIGBAND_MAX   0x34F0  /* 801.25 MHz */ 
#endif 

/**
 *   Tuner driver setting for tuner service
 */
#if CONFIG_PHILIPS_FM1216ME
  #define SUPPORT_SECAM_L    1
  #define SECAM_L_START_SCAN_FREQ       0x850    // 0x850 = 99Mhz of PHILIPS_FM1216ME
#else
  #define SUPPORT_SECAM_L    0
  #define SECAM_L_START_SCAN_FREQ       TUNER_LOWBAND_MIN
#endif

  #define SUPPORT_TUNER_INTERNAL_MTS    0

enum __TUNERPART_CTRLDATA
{
    TUNER_PART_CTRL_FREQ_H = 0,
    TUNER_PART_CTRL_FREQ_L,
    TUNER_PART_CTRL_STEP,
    TUNER_PART_CTRL_BAND,
    TUNER_PART_CTRL_AUX = TUNER_PART_CTRL_BAND,
    TUNER_PART_CTRL_DATASIZE
};

  /**
 *  Control Byte CB
 *
 *   Bit7  Bit6  Bit5  Bit4  Bit3  Bit2 Bit1 Bit0
 *    1     CP    T2    T1    T0   RSA  RSB   OS
 *
 *  Default value: 11001110 = 0xCE
 *
 *   PLL Disabling (normal operation) : OS=0
 *  Resolution frequency (62.5KHz) :  RSA=1, RSB=1
 *   Test Mode Setting (normal operation) : T2=0, T1=0, T0=1
 *   Charge-pump current (High) :  CP=1
  */
  
#define TUNER_PART_CTRL_STEP_DEFAULT    0xCE
#define TUNER_PART_CTRL_DEFAULT         0x9E

 /**
*    Auxiliary Byte AB
*
*    Bit7  Bit6  Bit5  Bit4  Bit3  Bit2 Bit1 Bit0
*    ATC   AL2   AL1   AL0    0     0    0    0
*
*   Default value: 01000000 = 0x40
*
*   AGC time constant (2 sec) : ATC=0
*   AGC TOP Setting (106 dBuV) : AL2=1, AL1=0, AL0=0
*       106 dBuV: 0x40
*       112 dBuV: 0x20  ==> reduce light bar, while enhance noise
*/
#define TUNER_PART_CTRL_AUX_DEFAULT     0x40

#endif
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
typedef struct _structIFPart
{
    UINT8 bBdata;
    UINT8 bCdata;
    UINT8 bEdata;
} strucIFPART, *pstrucIFPART;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define SCAN_TUNE_STEP(bIFState)    (bDrvMonitorAFCStatus((bIFState))/2)

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

#endif /* __PIANA_TDQU_H__ */
  
