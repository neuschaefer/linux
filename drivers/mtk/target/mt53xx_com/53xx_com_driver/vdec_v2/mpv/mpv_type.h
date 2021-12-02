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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: mpv_type.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file mpv_type.h
 *  This header file contains common constant and type definitions of MPV.
 */

#ifndef MPV_TYPE_H
#define MPV_TYPE_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mpv_if.h"
#include "mpv_drvif.h"
LINT_EXT_HEADER_BEGIN
#include "drv_dbase.h"
#include "x_typedef.h"
#include "x_csemaphore.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//MPEG Version
#define MPEG1       1
#define MPEG2       2

// Picture Structure
#define TOP_FLD_PIC    1
#define BTM_FLD_PIC    2
#define FRM_PIC            3
// The follow 2 define use in Reference Field Picture
#define TWO_FLDPIC_TOPFIRST  0x11
#define TWO_FLDPIC_BTMFIRST  0x12
#define ERR_PIC_STRUCT          0xFF
#define PIC_STRUCT_MASK       0xF

//Block Mode
#define MC_BLK_TB        0
#define MC_BLK_SWAP   1 
#define MC_BLK_NORM   2
#define MC_DESIGNER_VIEW   3

// Referenc Buf / B Buf / Digest Buf / Post Processing Buf Index
#define MC_REF_BUF_1       0
#define MC_REF_BUF_2       1
#define MC_DIG_BUF           2
#define MC_B_BUF               3
#define MC_Prg_B_BUF_1    4
#define MC_Prg_B_BUF_2    5
#define MC_SCALER_BUF      6
#define MC_DBK_BUF            7

// Picture Field control for MC
#define MC_TOP_FLD             0
#define MC_BOTTOM_FLD       1

#define MC_2ND_FLD_OFF      0
#define MC_2ND_FLD_ON       1

#define MPV_ESFIFO_ALIGN     0x3F

// Read address must 128 bit alignment
#define MPV_READ_ADDR_ALIGN        (128 / 8)

// Quantization Table Size, 64 = 16 * 4 (Byte)
#define MPV_MATRIX_SIZE            16
#define MPV_MATRIX_RAW_SIZE   ((MPV_MATRIX_SIZE * 8) + MPV_READ_ADDR_ALIGN)

//Block Mode Alignment
#define MPV_DBK_ALIGMENT         0xF
#define MPV_BLK_ALIGMENT         0x3F
#define MPV_SWAP_BLK_ALIGMENT    0x7F

#define STC_DELTA_MSB            (0x80000000)

#define MPV_FORCE_SD   1

//#define MAX_USERDATA_SIZE          1024

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define MPV_ALIGN_MASK(value, mask)     ((value + mask) & (~mask))
#define GET_FBG_ID(prVdecEsInfo)     (prVdecEsInfo->ucFbgId)
#define IS_FRMPIC(prFbmPicHdr)      (prFbmPicHdr->ucPicStruct == FRM_PIC)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------



typedef struct _MPV_ES_INFO_T_
{ 
    BOOL fgLoadIntraMatrix;
    BOOL fgLoadNonIntraMatrix;
    BOOL fgGopHdr;    
    BOOL fgPicHdr;    
    BOOL fgDetectRefLost;    
    BOOL fgDetectIntra;
    BOOL fgDecTimeout;    
    BOOL fgPreB;    
    BOOL fgChgFbgT;   
    BOOL fgHAvg;
    BOOL fgFreeP;
    BOOL fgMcPipe;
    BOOL fgMcFastAddr;
    BOOL fgMcNewFsm;
    BOOL fgHalfFltY;
    BOOL fgHalfFltC;    
    BOOL fgVirtualCc;
    BOOL fgNextBtm;
    BOOL fgBit5;
    BOOL fgBit4;    
    BOOL fgBit3;    
    BOOL fgDbkFbg;
    //BOOL fgDisableDbk;
    BOOL fgDisableDbkThrsd;
    BOOL fgBNoDbk;
    BOOL fgVideoLock;
    BOOL fgRatioCLI;
    BOOL fgClearRRDbk;
    BOOL fgAbortDecode;
    BOOL fgNotFirstPass;
        
    UCHAR ucType;
    UCHAR ucMpegVer;
    UCHAR ucFbgType;
    UCHAR ucBitCount;
    //UCHAR ucFbId;        // Current Fb Id
  //  UCHAR aucMatrix[MPV_MATRIX_RAW_SIZE];
    UCHAR *pucMatrix; //[MPV_MATRIX_RAW_SIZE];
    UCHAR ucECMethod;        
    UCHAR ucHScale;
    UCHAR ucVScale;
    UCHAR ucBNs;    
    UCHAR ucLastPicStruct;
    UCHAR ucLastPicTp;   
    UCHAR *pucIFrameBuf;    
    UCHAR ucBigB;    
    
    UINT32 u4DecMode; // ilii detele it
    //UINT32 u4PreMode;    
    UINT32 *pu4IntraMatrix;
    UINT32 *pu4NonIntraMatrix;
    UINT32 u4MatrixId;
    UINT32 u4PreRefPTS;     //For detecting Reference Pic missing    
    UINT32 u4ErrThrsd;    
    UINT32 u4PreBTemporalRef;    
    UINT32 u4VldWptr;     
    #if 0
    //for Scaler and DBK test
    UINT32 u4YBuf[3];
    UINT32 u4CBuf[3];    
    #endif
    UINT32 u4IntraSliceReg0;
    UINT32 u4IntraSliceReg1;
    UINT32 u4IntraSliceReg2;
    UINT32 u4SeqCnt;
    UINT32 u4HalfRefType;
    UINT32 u4DbkXStart;
    UINT32 u4DbkXEnd;
    UINT32 u4DbkYStart;    
    UINT32 u4DbkYEnd;
    UINT32 u4EcSetting;   
    UINT32 u4AtscCcUsrDataCnt; 
    UINT32 u4ScteCcUsrDataCnt;     
    UINT32 u4LastQp;
    UINT32 u4QpInterval;         
    UINT32 u4RRatio; 

    
    //for DBK para test 
    UINT32 u4P1; 
    UINT32 u4P2; 
    UINT32 u4P3; 
    UINT32 u4P4; 
    UINT32 u4P5; 
    UINT32 u4P6; 
    UINT32 u4P7; 
    UINT32 u4P8; 
    UINT32 u4P9; 
    UINT32 u4P10; 
    UINT32 u4P11; 
    UINT32 u4P12; 
    UINT32 u4P13; 
    UINT32 u4P14; 
    UINT32 u4P15; 
    UINT32 u4P16; 
    UINT32 u4P17; 
    UINT32 u4P18; 
    UINT32 u4P19; 
    UINT32 u4P20; 
    UINT32 u4QpAvg;    
    UINT32 u4QpAvgThrsd;
    UINT32 u4DbkRatio;    
    UINT32 u4DbkRatioThrsd;
    UINT32 u4QpAvgThrsdP;
    UINT32 u4DbkRatioThrsdP;
    UINT32 u4Mv;
    UINT32 u4MvTotal;
    UINT32 u4MvThrsd;    
    UINT32 u4DbkCnt;
    UINT32 u4NoDbkCnt;
    
#ifdef MPV_SEMI_HOST
    UINT32 u4StartSaveNo;
    UINT32 u4EndSaveNo;
    BOOL fgDecodeOnRepeat;
#endif
    
    UINT32 u4LastGetLockRxPacket;
    UINT32 u4LastGetLockDecOkPacket;
    HAL_TIME_T rLastGetLockTime;
    
    /*
    void* pvPtsNfyTag;
    PFN_PTS_NFY_FCT pfPtsNfyFct; 
    void* pvInpNfyTag;
    PFN_ACQUIRE_FCT pfAcquireNfy; 
    PFN_RELEASE_FCT pfReleaseNfy; 
    */

    //PARAM_MPV_T *prMpvCounter;
    FBM_PIC_HDR_T rFbmPicHdr;    
    HAL_TIME_T rILastTime;
    

#ifdef __MODEL_slt__
    UCHAR fgCountPicNs;
    UCHAR fgCountPicFinish;    
    UINT32 u4StartPts; 
    UINT32 u4EndPts;
    UINT32 u4PicNs;
#endif

}MPV_ES_INFO_T;

/*
// ESM Queue
typedef struct _ESMQ_T
{
    UCHAR           ucDropBNs;
    UCHAR           ucStarted;
    UINT16          u2Count;
    UINT16          u2UdfNs;
    HANDLE_T        hMsgQueue;
    HANDLE_T        hUnderFlowSema;
    HANDLE_T        hCmdQueue;
    HANDLE_T        hMutex;

} ESMQ_T;
*/

typedef struct _MPV_INFO_T
{
    BOOL            fgChgResolution[MPV_MAX_ES];
    //BOOL            afgDmxPartialPes[MPV_MAX_ES];
    BOOL            fgDisableDbk;
    BOOL            fgDbkEngrMode;
    //UCHAR           ucMode;    
    UINT16          u2DbkXStartT;
    UINT16          u2DbkYStartT;    
    UINT16          u2DbkWidthT;            
    UINT16          u2DbkHeightT;            
    //void*           pvDecNfyTag[MPV_MAX_ES];    
    //PFN_MPV_EVENT_NOTIFY pfErrNotify;    
    //PFN_DEC_NFY_FCT pfDecNfyFct[MPV_MAX_ES];       
} MPV_INFO_T;


/*typedef struct _MPV_EXT_INFO_T
{
    UCHAR           aucUserDataBuf[MAX_USERDATA_SIZE];        
    BOOL            fgUserData[MPV_MAX_ES];
    BOOL            fgAfd[MPV_MAX_ES];    
    //BOOL            fgSdUserDataFilter[MPV_MAX_ES];    
    //UINT32          u4FilterMask[MPV_MAX_ES];
    UINT32          u4UserDataRptr;
    UINT32          u4UserDataWptr;    
    PFN_MPV_AVDEC_CB   pfAVDecCb[MPV_MAX_ES];
    PFN_MPV_USERDATA_CB   pfUserDataCb;
       
} MPV_EXT_INFO_T;*/


#endif // MPV_TYPE_H
