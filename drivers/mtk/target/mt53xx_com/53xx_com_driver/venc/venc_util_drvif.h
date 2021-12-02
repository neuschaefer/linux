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
 * $RCSfile: venc_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file venc_if.h
 *  Public interface of software program stream demux driver
 */


#ifndef VENC_UTIL_H
#define VENC_UTIL_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "drv_common.h"
#include "x_common.h"
#include "x_typedef.h"
#include "x_timer.h"
#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_clksrc.h"
#include "x_hal_5381.h"
#include "x_hal_926.h"
#include "venc_if.h"

//-----------------------------------------------------------------------------
// resizer util
//-----------------------------------------------------------------------------
typedef struct _VENC_REZ_OBJ_T
{
    UINT8 type;
    UINT32 u4SrcW;
    UINT32 u4SrcH;
    UINT32 u4SrcPitch;
    UINT32 u4TgW;
    UINT32 u4TgH;
    UINT32 u4TgPitch;
    VENC_PIX_FMT_T  ePixFmt;
    UINT32 (*Init)  (struct _VENC_REZ_OBJ_T *);
    UINT32 (*Resize)(struct _VENC_REZ_OBJ_T *,UINT32, UINT32);
    VOID  *Members;
} VENC_REZ_OBJ_T;


VENC_REZ_OBJ_T *VencCreateRezObject(void);
VOID VencDeleteRezObject(VENC_REZ_OBJ_T *pObj);


//-----------------------------------------------------------------------------
// buffer manager util
//-----------------------------------------------------------------------------
#define VENC_BUFMGR_WAIT        1
#define VENC_BUFMGR_NOWAIT      2


#define VENC_BUFMGR_OK        0
#define VENC_BUFMGR_EMPTY     1
#define VENC_BUFMGR_FULL      2
#define VENC_BUFMGR_INV_ARG   3
#define VENC_BUFMGR_FAIL      4



#define VENC_BUFMGR_BASE_MEMBERS                                         \
    HANDLE_T hCntSem;                                                    \
    UINT8    u1Size;                                                     \
    UINT8    u1MaxSize;                                                  \
    UINT8    u1StackNs;                                                  \
    UINT8    u1MemSize;                                                  \
    UINT32 (*Get)   (struct _VENC_BUFMGR_BASE_T *, VOID *, UINT32);      \
    UINT32 (*Free)  (struct _VENC_BUFMGR_BASE_T *, VOID *);              \


typedef struct _VENC_BUFMGR_BASE_T
{
    VENC_BUFMGR_BASE_MEMBERS
} VENC_BUFMGR_BASE_T;

UINT32 _BUFMGR_OBJ_Get    (VENC_BUFMGR_BASE_T *, VOID *, UINT32);
UINT32 _BUFMGR_OBJ_Free  (VENC_BUFMGR_BASE_T *, VOID *);
UINT32 _BUFMGR_OBJ_Add   (VENC_BUFMGR_BASE_T *, VOID *);
VOID   _BUFMGR_OBJ_Reset(VENC_BUFMGR_BASE_T *);


#define VENC_BUFMGR_T(mod)   VENC_BUFMGR_##mod##OBJ_T

#define VENC_BUFMGR_DECLARE(mod,type,size)                               \
    typedef struct                                                       \
    {                                                                    \
        VENC_BUFMGR_BASE_MEMBERS                                         \
        UINT32 auStack[size];                                            \
        type   atData[size];                                             \
    } VENC_BUFMGR_T(mod);                                                \
                                                                         \
    /*constructor*/                                                        \
    static VENC_BUFMGR_T(mod) *_BUFMGR_##mod##OBJ_constructor(void)      \
    {                                                                    \
        static VENC_BUFMGR_T(mod) __##mod##Obj;                          \
        __##mod##Obj.Get        = _BUFMGR_OBJ_Get;                       \
        __##mod##Obj.Free       = _BUFMGR_OBJ_Free;                      \
        __##mod##Obj.u1Size     = 0;                                     \
        __##mod##Obj.u1MaxSize  = size;                                  \
        __##mod##Obj.u1StackNs  = 0;                                     \
        __##mod##Obj.u1MemSize  = sizeof(type);                          \
        /* create semaphore*/                                              \
        VERIFY(x_sema_create(&(__##mod##Obj.hCntSem),                    \
                             X_SEMA_TYPE_COUNTING,                       \
                             size) == OSR_OK);                           \
        return &__##mod##Obj;                                            \
    }                                                                    \
    /*destructor*/                                                         \
    static VOID _BUFMGR_##mod##OBJ_destructor(VENC_BUFMGR_T(mod) *pObj)  \
    {                                                                    \
        VERIFY(x_sema_delete(pObj->hCntSem) == OSR_OK);                  \
    }                                                                    \


#define VENC_MEMBER_OFFSET(type, member)  ((UINT32)(&(((type*)0)->member)))


#define VENC_BUFMGR_OBJ_CREATE(mod)  _BUFMGR_##mod##OBJ_constructor()


#define VENC_BUFMGR_OBJ_DELETE(mod,pO) _BUFMGR_##mod##OBJ_destructor(pO)



//VENC filter
typedef struct _VENC_FILTER_T
{
    UINT32 u4PtsDelta;
    UINT32 u4NextPts;
} VENC_FILTER_T;

VOID _VENC_Filter_Init     (VENC_FILTER_T *, UINT32);
BOOL _VENC_Filter_Do      (VENC_FILTER_T *, UINT32);
VOID _VENC_Filter_Reset(VENC_FILTER_T *);
VOID _VENC_Filter_Update(VENC_FILTER_T *);


#endif // VENC_UTIL_H


