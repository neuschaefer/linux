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
 * $RCSfile: fbm_fb.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sample.h
 *  Brief of file sample.h.
 *  Details of file sample.h (optional).
 */

#ifndef FBM_FB_H
#define FBM_FB_H


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "fbm_drvif.h"
#include "fbm_cs.h"
#include "x_typedef.h"


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

// Maximum frame buffer number per frame buffer group
#ifdef DEBUG_MAX_FB
#define FBM_MAX_FB_NS_PER_GROUP		32
#else
//for H264
//#define FBM_MAX_FB_NS_PER_GROUP     10
//#define FBM_MAX_FB_NS_PER_GROUP     18
#define FBM_MAX_FB_NS_PER_GROUP     22
#endif

// Reference buffer number per frame buffer group
#define FBM_REF_FB_NS_PER_GROUP		2


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

/** Brief of FBM_FBQ_T
 *  Frame Buffer Queue
 */
typedef struct
{
    UCHAR ucReadIdx;
    UCHAR ucCount;
    BOOL fgTrueDispQ;
    UCHAR aucQueue[FBM_MAX_FB_NS_PER_GROUP];
} FBM_FBQ_T;


/** Brief of FBM_FBG_T
 *  Frame Buffer Group
 */
typedef struct
{
    UCHAR ucFbgType;                        // Frame Buffer Group Type
    UCHAR ucFbgId;                          // Frame Buffer Group ID
    UCHAR ucFbgCm;                          // Frame Buffer Color Mode
    UCHAR ucFbNs;                           // Frame Buffer Number
    UCHAR ucMvBufNs;                        // MV Buffer Number

    UCHAR ucFbDecode;                       // Current Decoding FB
    UCHAR ucFbFRef;                         // Forward Reference FB
    UCHAR ucFbBRef;                         // Backward Reference FB

    UCHAR ucFbFRefValid;
    UCHAR ucFbBRefValid;

    UCHAR ucStcSrc;                         // AV Sync Source
    UCHAR ucAvSyncMode;                     // AV Sync Mode

    UCHAR ucPlayMode;                       // DTV or MM or ...
    UCHAR ucRecordNextFbId;
    UCHAR ucNewFbId;
    UCHAR ucSeqChg;
    UCHAR ucLastDecodeReference;            // Last Decode Picture is Reference

    UCHAR aucFbStatus[FBM_MAX_FB_NS_PER_GROUP];
    UCHAR aucPlayBackNs[FBM_MAX_FB_NS_PER_GROUP];
    UCHAR aucChangeFieldNs[FBM_MAX_FB_NS_PER_GROUP];
    UCHAR aucChgFldPlayBackNs[FBM_MAX_FB_NS_PER_GROUP];
    BOOL   afgRefList[FBM_MAX_FB_NS_PER_GROUP];    //for H264 to maintain reference list

    UINT8 u1DecoderSrcId;
    UINT8 u1FbgAppMode;

    HANDLE_T hChgFieldSema;

    UINT32 u4VDecFmt;                       //for mt539x various video decoder
    UINT32 u4FbgFlag;                       // Flag
    UINT32 u4FbWidth;                       // Frame Buffer Width, According to Color Mode
    UINT32 u4FbHeight;                      // Frame Buffer Height, According to Color Mode
    UINT32 u4ResidualFb;                    // Indicate whether residual FB is used
    UINT32 u4ResidualFree;                  // Indicate whether residual FB is pending to free
    UINT32 u4WorkBufSize;                   // The size for working buffer

    UINT32 u4FbMemoryPool;
    UINT32 u4FbMemoryPoolSize;

    UINT32 u4AvSyncStatus;                  // AV sync status, repeat or drop

#ifdef FBM_VIRTUAL_MPEG
    UINT32 u4VmStatus;
#endif

    void* pMallocAddr;                      // Memory allocation addr

    UINT32 au4AddrY[FBM_MAX_FB_NS_PER_GROUP];
    UINT32 au4AddrC[FBM_MAX_FB_NS_PER_GROUP];
    UINT32 au4AddrMv[FBM_MAX_FB_NS_PER_GROUP-1];

    UINT32 au4DbkOfstCnt[FBM_MAX_FB_NS_PER_GROUP];
    UINT32 au4Dbk4pCnt[FBM_MAX_FB_NS_PER_GROUP];

    UINT32 u4ExtraYBuffer;
    UINT32 u4ExtraCBuffer;
    FBM_SEQ_HDR_T rSeqHdr;
    FBM_PIC_HDR_T* prPicHdr;
    FBM_FBQ_T rEmptyQ;
    FBM_FBQ_T rDisplayQ;

    COUNTING_SEMAPHORE_T hEmptyQSemaphore;  // Empty Queue Counting Semaphore
    COUNTING_SEMAPHORE_T hEmptyRQSemaphore; // Empty R Queue Counting Semaphore
    COUNTING_SEMAPHORE_T hEmptyBQSemaphore; // Empty B Queue Counting Semaphore

    HANDLE_T hMutex;                        // Thread safe protection
    CRIT_STATE_T* prState;                  // Thread/Isr safe protection
} FBM_FBG_T;


//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

extern void _FBM_FbInit(FBM_FBG_T* prFbg);

extern void _FBM_PutFrameBufferToEmptyQ(UCHAR ucFbgId, UCHAR ucFbId);

extern UCHAR FBM_GetLatestEmptyFrameBuffer(UCHAR ucFbgId, UINT32 u4Delay);


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


#endif	// FBM_FB_H

