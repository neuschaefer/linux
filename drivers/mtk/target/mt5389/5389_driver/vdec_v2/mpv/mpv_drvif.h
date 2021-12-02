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

#ifndef MPV_DRVIF_H
#define MPV_DRVIF_H

#if 0
#ifndef NDEBUG
#define MPV_VIRTUAL_CC
#endif
#endif

#include "mpv_if.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


// VLD ID
#ifndef VLD0
#define VLD0   0
#define VLD1   1
#endif

//ES ID
#ifndef ES0
#define ES0     0
#define ES1     1
#define ES2     2
#define ES3     3
#endif

//Maximum VLD number
#define MPV_MAX_VLD			1

// Maximum Elementary Stream number
#ifdef CC_FBM_TWO_FBP
#define MPV_MAX_ES         2
#else
#define MPV_MAX_ES         1
#endif

// Picture Coding Type
#define MPV_I_TYPE            1
#define MPV_P_TYPE            2
#define MPV_B_TYPE            3
/*
//Decode Mode
#define MPV_CTRL_MODE_MASK    0xF
#define MPV_IDLE_MODE         0x0
#define MPV_PLAY_MODE         0x1
#define MPV_STOP_MODE         0x2
#define MPV_AVSYNC_MODE       0x3
#define MPV_HALFREF_MODE      0x4
#define MPV_PLAY_I_FRAME_MODE 0x5
#define MPV_DEBLOCK_MODE      0x6
*/
//Event notify
#define MPV_DEC_ERR			1
#define MPV_DEC_TIMEOUT	    2
#define MPV_SEQH_NOI		3
#define MPV_SD_HD			4
#define MPV_FIELD			5
#define MPV_PAN_SCAN		6

//Start code
#define MPV_SEQ_HDR         0x1b3
#define MPV_GOP_HDR         0x1b8
#define MPV_PIC_HDR         0x100
#define MPV_NO_HDR          0x0

#define MPV_DBK_LEVEL_MAX   29

#define MPV_SEMI_FLAG_SAVE          (0x1 << 0)
#define MPV_SEMI_FLAG_IGNORE_ERR    (0x1 << 1)
typedef struct _MPV_VDEC_LOGO_CTRL_FIELD_T
{
    /* 8 bits */
    UCHAR ucDefault   : 1;
    UCHAR ucEnable    : 1;
    UCHAR ucSelect    : 5;
    UCHAR ucReserved  : 1;
} MPV_VDEC_LOGO_CTRL_FIELD_T;

typedef union _MPV_VDEC_LOGO_CTRL_UNION_T
{
LINT_SUPPRESS_BRACE(960)

    UCHAR                        aucReg[1];
    MPV_VDEC_LOGO_CTRL_FIELD_T   rField;
} MPV_VDEC_LOGO_CTRL_UNION_T;


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//ES Info in ESMQ

typedef struct 
{
    BOOL fgSeqHdr;
    BOOL fgGop;
    BOOL fgDtsValid;      //DTS is valid
    UCHAR ucMpvId;
    UCHAR ucEsId;
    UCHAR ucPicType;
    UINT32 u4FifoStart;
    UINT32 u4FifoEnd;
    UINT32 u4VldReadPtr;
    UINT32 u4PTS;
    UINT32 u4DTS;
} MPV_PES_INFO_T;

typedef void (*PFN_MPV_EVENT_NOTIFY)(
	UINT32 u4Arg1, 
	UINT32 u4Arg2, 
	UINT32 u4Arg3, 
	UINT32 u4Arg4
);


typedef struct //_CC_INFO_T
{
    UINT32   u4BufRptr;
    UINT32   u4Size;        
    PTS_T    u8Pts;     
    BOOL     fgIsScte;
    BOOL     fgBottomFirst;
} CC_INFO_T;


typedef struct //_CC_BUF_INFO_T
{
    UINT32        u4Rptr;
    UINT32        u4Wptr;

}CC_BUF_INFO_T;


typedef struct //_MPV_CC_T
{
    BOOL fgPlayCc;
    BOOL   fgCcIPTmpIsScte;    
    BOOL fgCcPreRef;    
    BOOL fgCcBottomFirst;
    HANDLE_T hCcQueue;    
    UCHAR* pucCcBuf;
    UCHAR* pucCcTmpBuf;
    UCHAR* pucCcAtscIPTmpBuf;
    UCHAR* pucCcScteIPTmpBuf;	
    UINT32 u4CcBufSize;        
    UINT32 u4AtscIPTmpSize;		
    UINT32 u4AtscIPTmpPts;
    UINT32 u4ScteIPTmpSize;		
    UINT32 u4ScteIPTmpPts;	
    void* pvCcNfyTag;
    PFN_CC_NFY_FCT pfCcNfyFct; 
    CC_INFO_T rCcInfo;
    CC_BUF_INFO_T rCcBufInfo;    
    
} MPV_CC_T;
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------


EXTERN void _MPV_VparserReloadEs(UCHAR ucEsId);
EXTERN void _MPV_CreateDecSema(UCHAR ucMpvId);
EXTERN void _MPV_DeleteDecSema(UCHAR ucMpvId);
EXTERN void _MPV_CleanRRDbk(UCHAR ucEsId);
EXTERN void _MPV_SetDbk(UCHAR ucEsId);
EXTERN void _MPV_SetRR(UCHAR ucEsId);
EXTERN void _MPV_IsrStop(void);
EXTERN void _MPV_VldPower(UCHAR ucMpvId, BOOL fgOn);
EXTERN void* _MpvGetHdlr(UCHAR ucEsId);
EXTERN void* _MPV_GetExtInfo(void);
EXTERN void* _MPV_GetMpvInfo(void);




EXTERN void MPV_CleanRRDbk(UCHAR ucEsId);
EXTERN UINT32 MPV_GetDecMode(UCHAR ucEsId);
EXTERN void MPV_SetDecMode(UCHAR ucEsId, UINT32 u4DecMode);
EXTERN UCHAR MPV_CallBackEsm(const MPV_PES_INFO_T *prPesInfo);
EXTERN void MPV_FlushEsmQ(UCHAR ucEsId, BOOL fgAbortDecode);
EXTERN void MPV_SetEC(UCHAR ucEsId, UCHAR ucECMethod, UINT32 u4ErrThrsd, 
    BOOL fgDetectRefLost);
EXTERN void MPV_RegEventNotifyFunc(PFN_MPV_EVENT_NOTIFY pfFunc);
EXTERN void MPV_EventNotify(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4Arg3, UINT32 u4Arg4);
EXTERN BOOL MPV_IsDisplay(UCHAR ucEsId);
EXTERN void MPV_UpdateVldRptr(UCHAR ucEsId);
EXTERN void MPV_UpdateVldWptr(UCHAR ucEsId, UINT32 u4VldWptr);
//EXTERN void MPV_SetHalfRef(UCHAR ucEsId, UINT32 u4HRefMode, BOOL fgFilterY, BOOL fgFilterC, BOOL fgBit5, BOOL fgBit4, BOOL fgBit3);
EXTERN void MPV_SetMcMode(UCHAR ucEsId, BOOL fgMcPipe, BOOL fgMcFastAddr, BOOL fgMcNewFsm);
EXTERN void MPV_SetDbkRegion(UCHAR ucEsId, UINT32 u4XStart, UINT32 u4XEnd,  UINT32 u4YStart, UINT32 u4YEnd);

EXTERN void MPV_SetDBkThrsd(UCHAR ucEsId, BOOL fgDisable, UINT32 u4QpAvgThrsd, UINT32 u4DbkRatioThrsd, 
                                    UINT32 u4QpAvgThrsdP, UINT32 u4DbkRatioThrsdP, UINT32 u4MvThrsd);

EXTERN void MPV_SetDbkPara1(UCHAR ucEsId, UINT32 u4P1, UINT32 u4P2,UINT32 u4P3,UINT32 u4P4, UINT32 u4P5,
                                                UINT32 u4P6, UINT32 u4P7, UINT32 u4P8,UINT32 u4P9,UINT32 u4P10);
EXTERN void MPV_SetDbkPara2(UCHAR ucEsId, UINT32 u4P1, UINT32 u4P2,UINT32 u4P3,UINT32 u4P4, UINT32 u4P5,
                                                UINT32 u4P6, UINT32 u4P7, UINT32 u4P8,UINT32 u4P9,UINT32 u4P10);                                                
EXTERN void MPV_SetEcSetting(UCHAR ucEsId, UINT32 u4Mode);
EXTERN void MPV_LockVld(UCHAR ucMpvId);
EXTERN void MPV_UnlockVld(UCHAR ucMpvId);
EXTERN BOOL MPV_ResetSearch(UCHAR ucMpvId, UINT32 u4FifoStart, UINT32 u4FifoEnd, UINT32 u4SearchStart ,UINT32 u4SearchEnd);
EXTERN UINT32 MPV_SearchStartCode(UCHAR ucMpvId, UCHAR ucEsId, UINT32* pu4Addr);
EXTERN void MPV_QueryStatus(void);

EXTERN BOOL MPV_ShowLogo(VOID);

EXTERN void _MPV_SetSyncStc(UCHAR ucEsId);

#ifdef MPV_VIRTUAL_CC
EXTERN void MPV_PlayVirtualCc(UCHAR ucEsId, BOOL fgEnable);
#endif

#ifdef __MODEL_slt__
EXTERN void MPV_CountPicNs(UINT32 u4StartPts, UINT32 u4EndPts);
EXTERN UINT32 MPV_GetPicNs(void);
#endif

#ifdef MPV_SEMI_HOST
EXTERN void _MPV_SetSaveNo(UINT32 u4StartSaveNo, UINT32 u4EndSaveNo);
EXTERN void _MPV_SetFilePath(UINT32 u4Flag, char *szFilePath);
EXTERN void _MPV_CompareGold(UCHAR ucFbgId, UCHAR ucFbId);
#endif

#ifdef MPV_VIRTUAL_PRS
EXTERN void MPV_VirtualPrsParse(UCHAR ucMvpId, UINT32 u4Addr, UINT32 u4PicNs, UINT32 u4FrmNs);
EXTERN void MPV_VirtualPrsStart(UINT32 u4Delay);
EXTERN void MPV_VirtualPrsStop(void);
#endif

#ifdef MPV_STATISTICS
EXTERN void _MPV_ResetTime(void);
EXTERN void _MPV_PrintTime(UCHAR ucMpvId);
#endif

// parse
EXTERN INT32 _MPV_VParser(UCHAR ucMpvId, UCHAR ucEsId);

// decode
EXTERN void _MPV_DecReset(UCHAR ucMpvId, UCHAR ucEsId);
EXTERN INT32 _MPV_Decode(UCHAR ucMpvId, UCHAR ucEsId);
EXTERN INT32 _MPV_DecodeProc(UCHAR ucMpvId, UCHAR ucEsId);
EXTERN INT32 _MPV_WaitDecodeProc(UCHAR ucMpvId, UCHAR ucEsId);
EXTERN void _MPV_DropPic(UCHAR ucEsId);
EXTERN void _MPV_StopDecES(UCHAR ucEsId);

// mpv_if
EXTERN void _MPV_ClearEsInfo(UCHAR ucEsId);
EXTERN void _MPV_ClearMpvCc(UCHAR ucEsId);
EXTERN void* _MPV_GetMpvCc(UCHAR ucEsId);
EXTERN void _MPV_SkipPic(UCHAR ucMpvId, UCHAR ucEsId);
EXTERN void MPV_SetRR(UCHAR ucEsId, UINT32 u4Ratio);

EXTERN BOOL MPV_EsFlwCtrl(UCHAR ucEsId, UCHAR* pucMpvId, INT8 i1EsBufStatus, UCHAR ucSkipType);
EXTERN BOOL _MPV_EsFlwCtrl(UCHAR ucEsId, UCHAR* pucMpvId, INT8 i1EsBufStatus, UCHAR ucSkipMode);

EXTERN BOOL _MPV_WaitRandomAP(const void* prPesInfo);

EXTERN UCHAR _MPV_HandleOverFlow(UCHAR ucMpvId, UCHAR ucEsmQId, HANDLE_T hMutex,
    HANDLE_T hMsgQueue, const void* pvPesInfo);

EXTERN UINT32 _MPV_CalcPts(UCHAR ucEsId, UCHAR ucFbId);

EXTERN VOID _MPV_SetIsr(UCHAR ucEsId);
EXTERN void _MPV_VldSetVFifo(UCHAR ucMpvId, UINT32 u4Sa, UINT32 u4Ea);
EXTERN BOOL _MPV_VldInitBarrelShifter(UCHAR ucMpvId, UINT32 u4Ptr);
EXTERN UINT32 _MPV_VParserGetHdr(UCHAR ucMpvId, UCHAR ucEsId, 
                                        UINT32* pu4Addr, UCHAR* pu4PicType);
#endif
