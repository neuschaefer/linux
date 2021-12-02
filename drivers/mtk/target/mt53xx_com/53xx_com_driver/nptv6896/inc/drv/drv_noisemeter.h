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
 * $RCSfile: drv_nr.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/**
 * @file Noise meter header file
 *
 * @author skywalker.yin@mediatek.com
 */

#ifndef _DRV_NOISEMETER_H_
#define _DRV_NOISEMETER_H_

#include "typedef.h"

/** Data structure
 */
typedef struct
{
	UINT32 u4CurSum;                //current block sum
	UINT32 u4PreSum;                //previoud block sum
	UINT32 u4AbsDiffSum;            //Sum of absolute diff
	UINT32 u4SquareDiffSum;	        //sun of square diff
	UINT32 u4BlockAPL;              //APL of current block
}NR_NOISE_METER_LOCALAREA_INFO;

typedef struct
{
    UINT16 u2NoiseLevel;            //final noise level
    UINT16 u2FinalConfidence;       //final noise level confidence
}NR_NOISE_METER_RESULT;

typedef struct
{
	UINT8  fgPixelCountStable;      //pixel count of diff. histogram is stable or not
	UINT8  bMotionPixelRatio;       //motion pixel count ratio
    UINT16 u2MoPxlConfidence;       //confidence1: motion pixel ratio confidence	
}NR_NOISE_METER_MPR_INFO;

typedef struct
{
	UINT32 u4MinDS;                 //Minimum DS
	UINT32 u4MaxDS;                 //Maximum DS
	UINT32 u4AvgDS;                 //Average DS
	UINT16 u2DSRatio;               //original DSR
	UINT16 u2NDSRatio;              //normalized DSR
    UINT16 u2DSRatioConfidence;     //confidence2: DS ratio confidence	
	UINT8 fgDSStable;               //flag to show if DS info. is stable or not
}NR_NOISE_METER_DSR_INFO;

//u2GlobalHistogram[0]  -128 ~ -84
//u2GlobalHistogram[1]  -83 ~ -72
//u2GlobalHistogram[2]  -71 ~ -60
//u2GlobalHistogram[3]  -59 ~ -48
//u2GlobalHistogram[4]  -47 ~ -36
//u2GlobalHistogram[5]  -35 ~ -24
//u2GlobalHistogram[6]  -23 ~ -12
//u2GlobalHistogram[7]  -11 ~   0
//u2GlobalHistogram[8]    1 ~  12
//u2GlobalHistogram[9]   13 ~  24
//u2GlobalHistogram[10]  25 ~  36
//u2GlobalHistogram[11]  37 ~  48
//u2GlobalHistogram[12]  49 ~  60
//u2GlobalHistogram[13]  61 ~  72
//u2GlobalHistogram[14]  73 ~  84
//u2GlobalHistogram[15]  85 ~ 127		
typedef struct
{
	UINT16 u2NoiseValue;                                //Noise Value
	UINT16 u2GlobalHistogram[16];                       //16 bin of diff. histogram
	NR_NOISE_METER_LOCALAREA_INFO NMLocalAreaInfo[16];	//info. of 16 local area
	UINT8  bAPLW;                                       //APL weighting
	NR_NOISE_METER_MPR_INFO nmMPR;                      //MPR info.	
	NR_NOISE_METER_DSR_INFO nmDSR;                      //DSR info.
	NR_NOISE_METER_RESULT nmResult;                     //NM result
	UINT8 fgTriggerStatus;                              //flag to trigger all table update
}NR_NOISE_METER_INFO;

typedef struct
{
    UINT32 xTurn1;          //Table x turn point1
    UINT32 xTurn2;          //Table x turn point2
    UINT32 yStart;          //Table y start value
    UINT32 yEnd;            //Table y end value
}NM_LINEAR_LUT;

/** Public function    
 */

//noise meter
void vDrvNMInit(UINT8 fgNM,UINT32 loopNumber,UINT32 intervalNumber);
void vDrvNMSetBlockSize(UINT16 width, UINT16 height);
void vDrvNMProc(void);
NR_NOISE_METER_RESULT* vDrvNMGetResult(void);
void vDrvNMLUTMappingUT(void);      //Function for CLI unit test

#endif
