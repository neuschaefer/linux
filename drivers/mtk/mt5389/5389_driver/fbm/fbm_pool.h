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
 * $RCSfile: fbm_pool.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file sample.h
 *  Brief of file sample.h.
 *  Details of file sample.h (optional).
 */

#ifndef FBM_POOL_H
#define FBM_POOL_H


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "fbm_fb.h"
#include "x_typedef.h"


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

/** Brief of RESIDUAL_POOL_T
 *  Residual Frame Buffer Pool
 */
typedef struct
{
    UINT32            u4PoolIdx;
    UINT32            u4FbMemoryPool;
    UINT32            u4FbMemoryPoolSize;

    UCHAR            aucFbStatusY[FBM_MAX_FB_NS_PER_GROUP];
    UCHAR            aucFbStatusC[FBM_MAX_FB_NS_PER_GROUP];

    UINT32            au4AddrY[FBM_MAX_FB_NS_PER_GROUP];
    UINT32            au4AddrC[FBM_MAX_FB_NS_PER_GROUP];
} RESIDUAL_POOL_T;

/** Brief of FBP_LIST_T
 *  MPEG Frame Buffer Pool Type List
 */
typedef struct
{
    UCHAR ucType;

    UINT32 u4Size;
    UINT32 u4YSize;
    UINT32 u4CSize;
    UINT32 u4Width;
    UINT32 u4Height;
} FBP_LIST_T;

/** Brief of POOL_LIST_T
 *  Frame Buffer Memory Pool Type List
 */
typedef struct
{
    CHAR* szName;
    FBM_POOL_TYPE_T eType;
    UINT32 u4AddrAlign;
    UINT32 u4SizeAlign;
    UINT32 u4Mode;
    UINT32 u4PoolSize;
    UINT32 u4Width;
    UINT32 u4Height;
    UINT32 u4Inherit;
} POOL_LIST_T;

// Frame Buffer Pool arrangement type
typedef enum
{
    FBM_POOL_ARRANGE_AFTER,             // pool arrange after ePool1, need to specify pool size
    FBM_POOL_ARRANGE_BEFORE,            // pool arrange before ePool1, need to specify pool size
    FBM_POOL_ARRANGE_BETWEEN,           // pool arrange after ePool1 & before ePool2, no need to specify pool size
    FBM_POOL_ARRANGE_SHARE,             // pool arrange from ePool1 start address plus offset, need to specify pool size
    FBM_POOL_ARRANGE_SHARE_ALL          // no need to specify pool size (1) pool arrange form ePool 1 start address plus offset to end of ePool 1, (2) pool arrange form ePool 2 start address to end of ePool 1
} POOL_ARRANGE_TYPE_T;

// Frame Buffer Pool arrangement info
typedef struct
{
    POOL_ARRANGE_TYPE_T eMode;
    FBM_POOL_TYPE_T ePool1;
    FBM_POOL_TYPE_T ePool2;    
    UINT8 u1Ready;
    UINT32 u4Offset;
} POOL_ARRANGE_INFO_T;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

EXTERN FBM_POOL_T* _FBM_PoolInit(VOID);

EXTERN VOID _FBM_GetPoolList(FBP_LIST_T** pprFbpList, UCHAR* pucFbpNs);

EXTERN UINT32 _FBM_PoolAllocate(UCHAR ucFbgType, UINT32* pu4FbMemoryPool);

EXTERN VOID _FBM_PoolRelease(UINT32 u4FbMemoryPool);

EXTERN VOID _FBM_PoolStatus(VOID);

EXTERN VOID FBM_ConfigurePool(POOL_LIST_T* prPoolList, POOL_ARRANGE_INFO_T* prPoolArrangeInfo);

EXTERN FBM_POOL_T* FBM_GetPoolInfoAuto(UCHAR ucPoolType, const FBM_POOL_MODULE_INFO_T* pInfo);

EXTERN VOID FBM_PrintPoolInfo(FBM_POOL_T* prPool, POOL_LIST_T* prPoolList);

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------


#endif  // FBM_POOL_H

