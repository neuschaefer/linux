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
 * $RCSfile: swdmx_mpg_psipsr.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file psipsr.h
 *  This header file declares public function prototypes of widget.
 */

#ifndef PSIPSR_H
#define PSIPSR_H
/*
typedef enum _E_PSIPSR_TBL_TYPE
{
	E_PSIPSR_TBL_TVCT,
	E_PSIPSR_TBL_CVCT,	
	E_PSIPSR_TBL_MGT,	
	E_PSIPSR_TBL_PAT,
	E_PSIPSR_TBL_PMT	,
	E_PSIPSR_TBL_STT	,
	E_PSIPSR_TBL_PSI
} E_PSIPSR_TBL_TYPE;

typedef enum _E_PSIPSR_MSG
{
	E_PSIPSR_OK,
	E_PSIPSR_TIMEOUT,
	E_PSIPSR_SLD_NOT_MATCH,
	E_PSIPSR_TABLE_ERROR,
	E_PSIPSR_OUT_OF_BOUNDRY,
	E_PSIPSR_OUT_OF_RESOURCE,
	E_PSIPSR_ERROR
} E_PSIPSR_MSG;

typedef struct _PSIPSR_TBL_TS2FREQ_T
{
	UINT16 u2TsId;
	UINT8 u1PhyCh; // physical channel, to acquire frequency
	UINT8 u1Version;
} PSIPSR_TBL_TS2FREQ_T;

typedef struct _PSIPSR_TBL_PMT_T
{
	UINT32 u4PID;
	UINT32 u4PCR;
	UINT32 u4VPID;
	UINT32 u4APID1;
	UINT32 u4APID2;
	UINT32 u4APID3;
	UINT32 u4APID1Type;
	UINT32 u4APID2Type;
	UINT32 u4APID3Type;
	UINT32 u4ProgNum;
} PSIPSR_TBL_PMT_T;


// 16 bytes (*128=2048bytes!)
typedef struct _PSIPSR_TBL_VCT_T
{
	UINT16 u2MajorChNum;
	UINT16 u2MinorChNum;
	UINT8 u1NextCh; //max 256 ch
	UCHAR szShortName[8];
	UINT16 u2ChTsId;
	UINT16 u2ProgNum;
	// volatile parts
	UINT16 u2PCR;
	UINT16 u2VPID;
	UINT16 u2APID1;
	UINT16 u2APID1Type;
	UINT16 u2APID2;
	UINT16 u2APID2Type;
	UINT16 u2APID3;
	UINT16 u2APID3Type;

	UINT16 u2PhyFreq;
} PSIPSR_TBL_VCT_T;

typedef struct _PSIPSR_TBL_MGT_T
{
	UINT32 u4Type;
	UINT32 u4Pid;
	UINT32 u4Version;
	UINT32 u4Length;
} PSIPSR_TBL_MGT_T;

typedef struct _PSIPSR_TBL_STT_T
{
	UINT32 u4SysTime;
	UINT8 u1GPSUTCOffset;
	UINT16 u2DayLightSavings;

} PSIPSR_TBL_STT_T;

typedef struct _PSIPSR_SECTION
{
	UINT32 u4BufID;
	UINT32 u4Len;
	UINT32 u4Pidx;
} PSIPSR_SECTION;



typedef struct _PSIPSR_PARAM_T {
	UINT32 u4MaxEleNum;
	UINT32 u4RetEleNum;
	UINT32 u4TblType;
	UINT32 u4PMTTblIdx;
	UINT32 u4Timeout;
	//UINT32 u4Pidx;
	//UINT32 u4Filter;
	PSIPSR_TBL_VCT_T* prVctTbl;
	PSIPSR_TBL_TS2FREQ_T* prTsTbl;
	PSIPSR_TBL_MGT_T* prMgtTbl;
	PSIPSR_TBL_PMT_T* prPMTTbl;
	PSIPSR_TBL_STT_T* prSttTbl;

	UINT32 u4TsIndex;
} PSIPSR_PARAM_T;

typedef struct _PSIPSR_RTC_T
{
    UINT8   u1Year;
    UINT8   u1Month;
    UINT8   u1Day;
    UINT8   u1Hour;
    UINT8   u1Minute;
    UINT8   u1Second;
} PSIPSR_RTC_T;
*/
#define DMX_DEFAULT_ES_FIFO_SIZE                0x300000
#define DMX_DEFAULT_PSI_FIFO_SIZE               0x1000

#endif
