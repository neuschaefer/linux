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
 * $RCSfile: io_MTIMAGE.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/
/** @file io_MTIMAGE.c
 */

/********************************************************************
  INCLUDE FILES
********************************************************************/
#include "mtimage.h"
#include "img_lib_if.h"
#include "fbm_drvif.h"
#include "cb_data.h"
#include "jpg_if.h"
#include "imgrz_if.h"
#include "imgrz_drvif.h"
#include "gfx_if.h"
#include "osd_if.h"
#include "gfx_drvif.h"
#include "srm_drvif.h"
#include "drv_tdtv_drvif.h"
#include <linux/mtal_ioctl.h>
#include "x_img_slideshow.h"
#include "vdp_if.h"
#include "drv_tdtv_drvif.h"

/********************************************************************
  MACRO CONSTANT DEFINITIONS
********************************************************************/
#define MAX_WIDTH 1920
#define MAX_HEIGHT 1080
#define MPF_HALF_W 1920
#define MPF_HALF_H 540

#define MAX_VIEW_NUM 1
#define ALIGN16(X) (((X) + 0xF) & (~0xF))    ///< for imgrz pitch alignment

#define MAX_SLIDE_SHOW_PIC_PERIOD 1500 // ms
#define MAX_SLIDE_SHOW_FRM_PERIOD 800 // ms
#define MAX_SLIDE_SHOW_FRM_NUM 16

/********************************************************************
  TYPE DEFINITION
********************************************************************/
typedef struct _MTIMAGE_INFO_T
{
    BOOL fgConnect;
    BOOL fgIsPushMode;
    BOOL fgIsVdoMode;
    BOOL fgIsMPF;
    UINT8 u1PicIdx;
    UINT8 u1CurView;
    UINT8 u1SlideShowType;
    UINT8 u1SlideShowEvent;
    HANDLE_T hSlideShowSema;
    ENUM_IMG_FORMAT	imgFormat;
    ENUM_IMG_ROTATE_PHASE eRotateDegree;
    ENUM_IMG_DISP_COLOR_MODE eImgDispCM;
    MTIMAGE_IMGCOLORFORMAT_T eImgColorFmt;
    IMG_REGION_T rDecOut;
    IMG_REGION_T rDecOut2;
    IMG_CALLBACK_INFO_T rCallbackInfo;
    UCHAR aucFbgId[MAX_VIEW_NUM];
    UCHAR aucCurImgFbId[MAX_VIEW_NUM];
    UCHAR aucPrevImgFbId[MAX_VIEW_NUM];
    UCHAR aucCurDispFbId[MAX_VIEW_NUM];
    E_TDTV_DECODER_INPUT eDetectedMode;
    UINT32 u4PlaneId;
    UINT32 u4DisplayRegionId;

    BOOL fgLibjpeg;
    UINT32 u4TgtUsrAddr;
} MTIMAGE_INFO_T;


/********************************************************************
  VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
static MTIMAGE_INFO_T _rImageInfo = {0};
static MTVDO_REGION_T _rMt3DCropinfo = {0};


#define USR_SPACE_ACCESS_VALIDATE_1ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(INT32)))                            \
   {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \
 
#define COPY_FROM_USER_1ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(INT32)))                        \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage argument error\n");      \
        return MTR_ERR_INV;                                          \
    }                                                       \
 
#define USR_SPACE_ACCESS_VALIDATE_2ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_2ARG_T)))              \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \
 
#define COPY_FROM_USER_2ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_2ARG_T)))          \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_3ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_3ARG_T)))                            \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage 3 arguments error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \
 
#define COPY_FROM_USER_3ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_3ARG_T)))                        \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage 3 arguments error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_4ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_4ARG_T)))              \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \
 
#define COPY_FROM_USER_4ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_4ARG_T)))          \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_6ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_6ARG_T)))              \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \
 
#define COPY_FROM_USER_6ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_6ARG_T)))          \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_8ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_8ARG_T)))              \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \
 
#define COPY_FROM_USER_8ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_8ARG_T)))          \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_10ARG(arg)                 \
    if (!access_ok(VERIFY_READ, (void __user *)arg,         \
                   sizeof(MTAL_IOCTL_10ARG_T)))              \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \
 
#define COPY_FROM_USER_10ARG(ArgUsr, ArgKernel)              \
    if (copy_from_user(&ArgKernel, (void __user *)ArgUsr,   \
                       sizeof(MTAL_IOCTL_10ARG_T)))          \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define USR_SPACE_ACCESS_VALIDATE_ARG(arg,type)                 \
    if (!access_ok(VERIFY_WRITE, (void __user *)arg,         \
                   sizeof(type)))              \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage argument error\n");     \
        return MTR_ERR_INV;                                          \
    }                                                       \
 
#define COPY_TO_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_to_user((void __user *)ArgUsr, &ArgKernel,   \
                       sizeof(type)))          \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

#define COPY_FROM_USER_ARG(ArgUsr, ArgKernel,type)              \
    if (copy_from_user(&ArgKernel,(void __user *)ArgUsr,    \
                       sizeof(type)))          \
    {                                                       \
        MTIMAGE_DEBUG("mtal_ioctl_mtimage argument error\n");      \
        return MTR_ERR_INV;                                          \
    }

extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

static BOOL _MTIMAGE_3DCropFuncCB(TDTV_3D_CROP_INTO_T* prCropInfo)
{
    prCropInfo->fgIsFixSrcAspectRatio = 1;
    if (_rImageInfo.fgIsMPF)
    {
        prCropInfo->fgIsFullResolution = 0;    //vertical will be 1/2, and scaler will enlarge it to 2 multiple ratio.
    }
    else
    {
        prCropInfo->fgIsFullResolution = 1;
    }
    prCropInfo->u4CropBottomOffset = _rMt3DCropinfo.u4Y;
    prCropInfo->u4CropTopOffset = _rMt3DCropinfo.u4Y;
    prCropInfo->u4CropLeftOffset = _rMt3DCropinfo.u4X;
    prCropInfo->u4CropRightOffset = _rMt3DCropinfo.u4X;

    //printf("#@# _MTIMAGE_3DCropFuncCB [%d][%d][%d][%d]/n", _rMt3DCropinfo.u4X, _rMt3DCropinfo.u4Y, _rMt3DCropinfo.u4Width, _rMt3DCropinfo.u4Height);

    return TRUE;
}


/*
Assign FBG ID, and set SRM mode.
*/
static BOOL _FBGHandler(MTIMAGE_INFO_T* prImageInfo, BOOL fgConnect)
{
    UINT32 u4ViewIdx;
    UCHAR ucFbgType;
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    FBM_CREATE_FBG_PAR_T rPar;
    x_memset(&rPar, 0, sizeof(FBM_CREATE_FBG_PAR_T));
    rPar.u1VdecId = (UINT8)0;
    rPar.u1AppMode = FBM_FBG_APP_MTIMAGE;


    //prepare VDP frame buffer, create FBG
    prImageInfo->u1CurView = 0;
    for (u4ViewIdx = 0; u4ViewIdx < MAX_VIEW_NUM; u4ViewIdx++)
    {
        if (fgConnect)
        {
            //reset
            prImageInfo->aucFbgId[u4ViewIdx] = 0xFF;
            prImageInfo->aucCurImgFbId[u4ViewIdx] = 0xFF;
            prImageInfo->aucPrevImgFbId[u4ViewIdx] = 0xFF;
            prImageInfo->aucCurDispFbId[u4ViewIdx] = 0xFF;

            if (prImageInfo->fgConnect)
            {
                FBM_ReleaseGroup(prImageInfo->aucFbgId[u4ViewIdx]);
            }
            //todo
            //SRM_SetMmMode(SRM_MM_MODE_TYPE_JPEG);

            ucFbgType = FBM_SelectGroupType(MAX_WIDTH, MAX_HEIGHT);
            FBM_SetColorMode(FBM_CM_422);
            //prImageInfo->aucFbgId[u4ViewIdx] = FBM_CreateGroup(ucFbgType, FBM_VDEC_JPEG, MAX_WIDTH, MAX_HEIGHT);
            prImageInfo->aucFbgId[u4ViewIdx] = FBM_CreateGroupExt(ucFbgType, FBM_VDEC_JPEG, MAX_WIDTH, MAX_HEIGHT, &rPar);
            if (prImageInfo->aucFbgId[u4ViewIdx] == FBM_FBG_ID_UNKNOWN)
            {
                MTIMAGE_DEBUG("Create View %d Fbg Fail!!\n", u4ViewIdx);
                //SRM_SetMmMode(SRM_MM_MODE_TYPE_OFF);
                return FALSE;
            }
            FBM_SetPlayMode(prImageInfo->aucFbgId[u4ViewIdx], FBM_FBG_MM_MODE);
            FBM_SetSyncStc(prImageInfo->aucFbgId[u4ViewIdx], 0, 0); //none
            FBM_FbgChgNotify(prImageInfo->aucFbgId[u4ViewIdx], 0);

            prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(prImageInfo->aucFbgId[u4ViewIdx]);
            if (prFbmSeqHdr == NULL)
            {
                ASSERT(prFbmSeqHdr != NULL);
                return FALSE;
            }
            prFbmSeqHdr->u2HSize = MAX_WIDTH;
            prFbmSeqHdr->u2VSize = MAX_HEIGHT;
            prFbmSeqHdr->u2OrgHSize = prFbmSeqHdr->u2HSize;
            prFbmSeqHdr->u2OrgVSize = prFbmSeqHdr->u2VSize;
            prFbmSeqHdr->u2LineSize = MAX_WIDTH;
            prFbmSeqHdr->fgProgressiveSeq = TRUE;
            prFbmSeqHdr->fgRasterOrder = TRUE;
            prFbmSeqHdr->fg422Mode = TRUE;
            prFbmSeqHdr->ucFrmRatCod = MPEG_FRAME_RATE_30;

            FBM_SetFrameBufferFlag(prImageInfo->aucFbgId[u4ViewIdx], FBM_FLAG_SEQ_CHG);
        }
        else
        {
            //if ever connected and fbgid isn't 0xff, then release FBG
            if ((prImageInfo->fgConnect)&&(prImageInfo->aucFbgId[u4ViewIdx] != 0xFF))
            {
                FBM_ReleaseGroup(prImageInfo->aucFbgId[u4ViewIdx]);
                FBM_FbgChgNotify(FBM_FBG_ID_UNKNOWN, 0);
            }
            //reset
            prImageInfo->aucFbgId[u4ViewIdx] = 0xFF;
            prImageInfo->aucCurImgFbId[u4ViewIdx] = 0xFF;
            prImageInfo->aucPrevImgFbId[u4ViewIdx] = 0xFF;
            prImageInfo->aucCurDispFbId[u4ViewIdx] = 0xFF;

            //SRM_SetMmMode(SRM_MM_MODE_TYPE_OFF);
        }
    }
    return TRUE;
}

/*
ucFbgId [In]
pucFbId [Out]
pu4YAddr [Out]
pu4CAddr [Out]
*/
static BOOL _AllocFrame(UCHAR ucFbgId, UCHAR* pucFbId, UINT32* pu4YAddr, UINT32* pu4CAddr)
{
    FBM_PIC_HDR_T* prFbmPicHdr;
    UINT32 u4FrameSize, u4W, u4H;

    if ((pucFbId == NULL) || (pu4YAddr == NULL) || (pu4CAddr == NULL))
    {
        MTIMAGE_DEBUG("AllocFrame input parameter error\n");
        return FALSE;
    }
    //get frame buffer
    *pucFbId = FBM_GetEmptyFrameBuffer(ucFbgId, (UINT32)1500);
    if (*pucFbId == (UCHAR)FBM_FB_ID_UNKNOWN)
    {
        MTIMAGE_DEBUG("Get empty frame buffer failed.\n");
        return FALSE;
    }
    FBM_GetFrameBufferAddr(ucFbgId, *pucFbId, pu4YAddr, pu4CAddr);
    prFbmPicHdr = FBM_GetFrameBufferPicHdr(ucFbgId, *pucFbId);
    if ((prFbmPicHdr == NULL) || (pu4YAddr == NULL) || (pu4YAddr == NULL))
    {
        MTIMAGE_DEBUG("Alloc frame buffer failed.\n");
        return FALSE;
    }
    prFbmPicHdr->ucPicStruct = (UCHAR)3;
    prFbmPicHdr->fgProgressiveFrm = TRUE;
    prFbmPicHdr->fgRepFirstFld = FALSE;
    prFbmPicHdr->fgTopFldFirst = FALSE;
    FBM_UpdateReferenceList(ucFbgId, *pucFbId, FALSE);    //default
    FBM_SetFrameBufferStatus(ucFbgId, *pucFbId, (UCHAR)FBM_FB_STATUS_READY);

    u4FrameSize = (UINT32)(*pu4CAddr - *pu4YAddr);
    MTIMAGE_DEBUG("Alloc Y 0x%x, C 0x%x, Sz 0x%x\n", *pu4YAddr, *pu4CAddr, u4FrameSize);

    //clean buffer content
    FBM_GetFrameBufferSize(ucFbgId, &u4W, &u4H);

    GFX_Memset((UINT8*)*pu4YAddr, u4W, u4H, 0);
    if (_rImageInfo.eImgDispCM == Y_CBCR420_SEP16)
    {
        GFX_Memset((UINT8*)*pu4CAddr, u4W>>1, u4H, 0x80);
    }
    else    // 422
    {
        GFX_Memset((UINT8*)*pu4CAddr, u4W, u4H, 0x80);
    }

    return TRUE;
}

#if 0
static IMG_SLIDE_SHOW_EFFECT_T _ConvertSlideShowType(UINT8 u1Type)
{
    //check data structure "IMG_SLIDE_SHOW_EFFECT_T" and "MTIMAGE_SLIDETYPE"
    return (IMG_SLIDE_SHOW_EFFECT_T)u1Type;
}
#endif

void _MTIMAGE_Notify(UINT32 u4ImgID, void *pv_tag, void *pv_data, UINT32 u4State)
{
    IMG_FILL_BUF_MW_T* prFillBuf;
    UCHAR ucFbgId, ucFbId;
    FBM_POOL_T *prFbmPool;

    ucFbgId = _rImageInfo.aucFbgId[_rImageInfo.u1CurView];
    ucFbId = _rImageInfo.aucCurImgFbId[_rImageInfo.u1CurView];

    if (u4State == JPG_HW_NFY_FINISHED)
    {
        _rImageInfo.rCallbackInfo.u4Condition = MT_IMAGE_CALLBACK_FINISH;
        _rImageInfo.rCallbackInfo.u4Param = (UINT32)pv_tag;
        if (_rImageInfo.fgIsVdoMode)
        {
            FBM_UpdateReferenceList(ucFbgId, ucFbId, TRUE);
            FBM_SetFrameBufferStatus(ucFbgId, ucFbId, (UCHAR)FBM_FB_STATUS_READY);
        }
    }
    else if (u4State == JPG_HW_NFY_ERROR)
    {
        _rImageInfo.rCallbackInfo.u4Condition = MT_IMAGE_CALLBACK_ERROR;
        if ((UINT32)pv_data == IMG_DRV_NOT_SUPPORT)
        {
            _rImageInfo.rCallbackInfo.u4Param = MT_IMAGE_FILE_ERROR;
        }
        else if ((UINT32)pv_data == IMG_DRV_ERR_COMPONENT)
        {
            _rImageInfo.rCallbackInfo.u4Param = MT_IMAGE_JPEG_COLOR_FORMAT_ERROR;
        }
        else    //eState==IMG_DRV_HW_ERROR
        {
            _rImageInfo.rCallbackInfo.u4Param = MT_IMAGE_IMG_ERROR;
        }
        if (_rImageInfo.fgIsVdoMode)
        {
            FBM_UpdateReferenceList(ucFbgId, ucFbId, FALSE);
            FBM_SetFrameBufferStatus(ucFbgId, ucFbId, (UCHAR)FBM_FB_STATUS_EMPTY);
            _rImageInfo.aucCurImgFbId[_rImageInfo.u1CurView] = 0xFF;
        }
    }
    else if (u4State == JPG_HW_NFY_FILL_BUF)
    {
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG);
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0);
        _rImageInfo.rCallbackInfo.u4Condition = MT_IMAGE_CALLBACK_NEEDDATA;
        prFillBuf = (IMG_FILL_BUF_MW_T *)pv_data;

        //printk("_MTIMAGE_Notify prFillBuf->ui4_trsn_id %d prFillBuf->ui4_required_len %x",prFillBuf->ui4_trsn_id,prFillBuf->ui4_required_len);
        _rImageInfo.rCallbackInfo.rReqDataInfo.u4_trsn_id = prFillBuf->ui4_trsn_id;
        _rImageInfo.rCallbackInfo.rReqDataInfo.u4_start_add = (UINT32)prFillBuf->pv_start_add - VIRTUAL(prFbmPool->u4Addr);
        _rImageInfo.rCallbackInfo.rReqDataInfo.u4_required_len = prFillBuf->ui4_required_len;
        _rImageInfo.rCallbackInfo.rReqDataInfo.u1_reset_pos = (UCHAR)prFillBuf->b_reset_pos;
        _rImageInfo.rCallbackInfo.rReqDataInfo.u4_position = prFillBuf->ui4_position;
    }
    else
    {
        return;
    }
    _CB_PutEvent(CB_MTIMAGE_TRIGGER, sizeof(IMG_CALLBACK_INFO_T), &_rImageInfo.rCallbackInfo);
}
#ifdef MTIMAGE_ANDROID_SUPPORT
void _MTIMAGE_Notify_VDO(UINT32 u4ImgID, void *pv_tag, void *pv_data, UINT32 u4State)
{
    IMG_FILL_BUF_MW_T* prFillBuf;
    UCHAR ucFbgId, ucFbId;
    FBM_POOL_T *prFbmPool;

    ucFbgId = _rImageInfo.aucFbgId[_rImageInfo.u1CurView];
    ucFbId = _rImageInfo.aucCurImgFbId[_rImageInfo.u1CurView];

    if (u4State == JPG_HW_NFY_FINISHED)
    {

        printk("_MTIMAGE_Notify  finish \n");
        _rImageInfo.rCallbackInfo.u4Condition = MT_IMAGE_CALLBACK_FINISH;
        _rImageInfo.rCallbackInfo.u4Param = (UINT32)pv_tag;
        if (_rImageInfo.fgIsVdoMode)
        {
            FBM_UpdateReferenceList(ucFbgId, ucFbId, TRUE);
            FBM_SetFrameBufferStatus(ucFbgId, ucFbId, (UCHAR)FBM_FB_STATUS_READY);
        }
    }
    else if (u4State == JPG_HW_NFY_ERROR)
    {

        printk("_MTIMAGE_Notify  error\n");
        _rImageInfo.rCallbackInfo.u4Condition = MT_IMAGE_CALLBACK_ERROR;
        if ((UINT32)pv_data == IMG_DRV_NOT_SUPPORT)
        {
            _rImageInfo.rCallbackInfo.u4Param = MT_IMAGE_FILE_ERROR;
        }
        else if ((UINT32)pv_data == IMG_DRV_ERR_COMPONENT)
        {
            _rImageInfo.rCallbackInfo.u4Param = MT_IMAGE_JPEG_COLOR_FORMAT_ERROR;
        }
        else    //eState==IMG_DRV_HW_ERROR
        {
            _rImageInfo.rCallbackInfo.u4Param = MT_IMAGE_IMG_ERROR;
        }
        if (_rImageInfo.fgIsVdoMode)
        {
            FBM_UpdateReferenceList(ucFbgId, ucFbId, FALSE);
            FBM_SetFrameBufferStatus(ucFbgId, ucFbId, (UCHAR)FBM_FB_STATUS_EMPTY);
            _rImageInfo.aucCurImgFbId[_rImageInfo.u1CurView] = 0xFF;
        }
    }
    else if (u4State == JPG_HW_NFY_FILL_BUF)
    {
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG);
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0);
        _rImageInfo.rCallbackInfo.u4Condition = MT_IMAGE_CALLBACK_NEEDDATA;
        prFillBuf = (IMG_FILL_BUF_MW_T *)pv_data;
        _rImageInfo.rCallbackInfo.rReqDataInfo.u4_trsn_id = prFillBuf->ui4_trsn_id;
        _rImageInfo.rCallbackInfo.rReqDataInfo.u4_start_add = (UINT32)prFillBuf->pv_start_add - VIRTUAL(prFbmPool->u4Addr);
        _rImageInfo.rCallbackInfo.rReqDataInfo.u4_required_len = prFillBuf->ui4_required_len;
        _rImageInfo.rCallbackInfo.rReqDataInfo.u1_reset_pos = (UCHAR)prFillBuf->b_reset_pos;
        _rImageInfo.rCallbackInfo.rReqDataInfo.u4_position = prFillBuf->ui4_position;
    }
    else
    {
        return;
    }
    _CB_PutEvent(CB_MTIMAGE_TRIGGER_VDO, sizeof(IMG_CALLBACK_INFO_T), &_rImageInfo.rCallbackInfo);

}
#endif

void  _MTIMAGE_SlideShow_Notify(SLIDESHOW_NFY_EVENT_TYPE_T e_event)
{
    _rImageInfo.u1SlideShowEvent = (UINT8)e_event;
    switch (e_event)
    {
    case SLIDESHOW_NFY_COMPLETE_ONE_FRAME:
    case SLIDESHOW_NFY_COMPLETE_ONE_PIC:
        VERIFY(x_sema_unlock(_rImageInfo.hSlideShowSema)== OSR_OK);
        break;
    default:
        break;
    }
    MTIMAGE_DEBUG("SlideShowEvent %d\n", (UINT8)e_event);
}


/**set image connect to vdp
*@param connect or disconnect
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTIMAGE_Connect(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    BOOL fgConnect, fgIsVdoMode;
    MTIMAGE_IMGCOLORFORMAT_T eColorFMT;
    //UINT32 u4Idx;
    //INT32 i4Ret;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    fgConnect = (BOOL)rArg.ai4Arg[0];
    eColorFMT = (MTIMAGE_IMGCOLORFORMAT_T)rArg.ai4Arg[1];
    _rImageInfo.eImgColorFmt = eColorFMT;
    fgIsVdoMode = (eColorFMT == MT_IMAGE_COLOR_FORMAT_Y_CBCR422) ? TRUE : FALSE;

    IMG_Lib_Init();
    switch (eColorFMT)
    {
    case MT_IMAGE_COLOR_FORMAT_Y_CBCR422:
        _rImageInfo.eImgDispCM = Y_CBCR422_SEP16;
        break;
    case MT_IMAGE_COLOR_FORMAT_ARGB8888:
        _rImageInfo.eImgDispCM = ARGB8888_DIRECT32;
#ifdef MTIMAGE_ANDROID_SUPPORT
        _rImageInfo.fgLibjpeg = TRUE;
#endif
        break;
    case MT_IMAGE_COLOR_FORMAT_ARGB1555:
        _rImageInfo.eImgDispCM = ARGB1555_DIRECT16;
        break;
    case MT_IMAGE_COLOR_FORMAT_ARGB4444:
        _rImageInfo.eImgDispCM = ARGB4444_DIRECT16;
        break;
    case MT_IMAGE_COLOR_FORMAT_ARGB565:
        _rImageInfo.eImgDispCM = RGB565_DIRECT16;
        break;
    case MT_IMAGE_COLOR_FORMAT_AYCBCR8888:
        _rImageInfo.eImgDispCM = AYCBCR8888_DIRECT32;
        break;
    default:
        break;
    }
#ifndef MTIMAGE_ANDROID_SUPPORT
    _rImageInfo.fgLibjpeg = FALSE;
#endif

    //if OSD mode, do nothing
    //else if VDP mode, prepare frame buffer
    if (!_rImageInfo.fgLibjpeg)
    {
        if (fgIsVdoMode)
        {
            if ((_rImageInfo.fgConnect==0)||fgConnect==0)   //when first connect => creat FBG, when exit => close FBG
            {
                _rImageInfo.eDetectedMode = 0xff;
                if (!_FBGHandler(&_rImageInfo, fgConnect))
                {
                    _rImageInfo.fgConnect = FALSE;
                    return MTR_NOT_OK;
                }
            }
            //slide show init/stop
            if (fgConnect)
            {
#if 0
                if ((i4Ret = x_img_init_slideshow(_MTIMAGE_SlideShow_Notify)) != SLIDESHOW_OK)
                {
                    MTIMAGE_DEBUG("InitSlideShowLib fail %d\n", i4Ret);
                }
                if (_rImageInfo.hSlideShowSema == (HANDLE_T)NULL)
                {
                    VERIFY(x_sema_create(&_rImageInfo.hSlideShowSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
                }

#endif
                //rImageInfo->eDetectedMode = E_TDTV_DECODER_INPUT_2D_P;
            }
            else
            {
                fpDrvTDTVReg3DCropCB(E_TDTV_CB_3D_CROP_MM, NULL);
#if 0   //cannt re-init?
                if ((i4Ret = x_img_stop_slideshow()) != SLIDESHOW_OK)
                {
                    MTIMAGE_DEBUG("InitSlideShowStop fail %d\n", i4Ret);
                }
#endif
            }
        }
        _rImageInfo.fgConnect = fgConnect;
        _rImageInfo.fgIsVdoMode = fgIsVdoMode;
    }
    else
    {
        _rImageInfo.fgConnect = fgConnect;
        _rImageInfo.fgIsVdoMode = 0;
    }
    return MTR_OK;
}


/**Initialize jpeg hw for decoding.
*@param eColorFMT            display color format
*@retval TRUE                 Success.
*@retval FALSE                Fail.
*/
static MT_RESULT_T _MTIMAGE_Open(unsigned long arg)
{
    MTIMAGE_HANDLE hHandle;
    ENUM_IMG_BUFFER_TYPE eImgBufType;
    UINT32 u4CurState;

    hHandle=(MTIMAGE_HANDLE)arg;

    eImgBufType = (_rImageInfo.eImgDispCM == Y_CBCR422_SEP16)?RING_IN_DOUBLE_OUT: RING_IN_SINGLE_OUT;

    //for jpg ARGB8888
    if (((_rImageInfo.imgFormat==IMAGE_JPEG)||(_rImageInfo.imgFormat==IMAGE_MPO))&&(_rImageInfo.eImgDispCM == ARGB8888_DIRECT32))
    {
        eImgBufType = RING_IN_SINGLE_OUT;
    }

    u4CurState = IMG_GetDecState();
    if (u4CurState == IMG_RELEASE)
    {
#ifdef MTIMAGE_ANDROID_SUPPORT
        if (eImgBufType == RING_IN_SINGLE_OUT)
        {
#endif
            if (IMG_Init(eImgBufType, hHandle, 0, _MTIMAGE_Notify) != E_IMG_OK)
            {
                return MTR_NOT_OK;
            }
#ifdef MTIMAGE_ANDROID_SUPPORT
        }
        else
        {
            if (IMG_Init(eImgBufType, hHandle, 0, _MTIMAGE_Notify_VDO) != E_IMG_OK)
            {
                return MTR_NOT_OK;
            }
        }
#endif
        IMG_SetUseMw(TRUE);
        _rImageInfo.eRotateDegree = CLOCKWISE_0;    //reset setting
        if (!(_rImageInfo.u1PicIdx == 1))   //in the middle of MPF decoding
        {
            x_memset((void *)&_rImageInfo.rDecOut, 0, sizeof(IMG_REGION_T));
            x_memset((void *)&_rImageInfo.rDecOut2, 0, sizeof(IMG_REGION_T));
        }
        x_memset((void *)&_rImageInfo.rCallbackInfo, 0, sizeof(IMG_CALLBACK_INFO_T));
    }
    else
    {
        MTIMAGE_DEBUG("_MTIMAGE_Open only available when IMG_RELEASE, Cur %d\n", u4CurState);
        return MTR_NOT_OK;
    }
    return MTR_OK;
}


/**Start jpeg decoding
*@param pu1Buf                Input Buffer address of jpeg
*@param u4ReadSize            Size of the current jpeg segment
*@param fgEOF                 Eof is in this segment
*@retval TRUE                 Success.
*@retval FALSE                Fail.
*/
static MT_RESULT_T _MTIMAGE_Decode(unsigned long arg)
{
    MTAL_IOCTL_10ARG_T rArg;
    MTIMAGE_HANDLE hHandle;
    ENUM_IMG_FORMAT imgFormat;
    IMG_PARAM_T rInfo;
    UINT32 u4YAddr = 0, u4CAddr = 0, u4CurState, u4DecW, u4DecH, u4Pitch;
    UCHAR ucFbId;
    UCHAR ucFbgId = _rImageInfo.aucFbgId[_rImageInfo.u1CurView];
    UINT32 u4MPFPicIdx; //0: 2D pic, 1: MPF 1st pic, 2: MPF 2nd pic
    UINT32 u4DecAddr = 0;
    UINT32 u4SrcWidth,u4SrcHeight,u4ImgSize;
    USR_SPACE_ACCESS_VALIDATE_10ARG(arg);
    COPY_FROM_USER_10ARG(arg, rArg);

    hHandle = (MTIMAGE_HANDLE)rArg.ai4Arg[0];
    u4DecW = (UINT32)rArg.ai4Arg[1];
    u4DecH = (UINT32)rArg.ai4Arg[2];
    imgFormat = (ENUM_IMG_FORMAT)rArg.ai4Arg[3];
    u4MPFPicIdx = (UINT32)rArg.ai4Arg[4];
    u4DecAddr = (UINT32)rArg.ai4Arg[5];
    u4SrcWidth = (UINT32)rArg.ai4Arg[6];
    u4SrcHeight = (UINT32)rArg.ai4Arg[7];
    u4ImgSize = (UINT32)rArg.ai4Arg[8];
    u4Pitch = (UINT32)rArg.ai4Arg[9];

    //decode
    hHandle=(MTIMAGE_HANDLE)arg;
    u4CurState = IMG_GetDecState();
    if (u4CurState == IMG_INIT)
    {
        if (_rImageInfo.fgIsVdoMode)
        {
            //check color mode
            if ((_rImageInfo.eImgDispCM != Y_CBCR420_SEP16) &&
                    (_rImageInfo.eImgDispCM !=Y_CBCR422_SEP16))
            {
                MTIMAGE_DEBUG("Color mode %d, should be YCbCr420 or YCbCr422 in VdoMode.\n", _rImageInfo.eImgDispCM);
                return MTR_NOT_OK;
            }
            //alloc frame buffer
            if (u4MPFPicIdx <= 1)
            {
                if (!_AllocFrame(ucFbgId, &ucFbId, &u4YAddr, &u4CAddr))
                {
                    return MTR_NOT_OK;
                }
                _rImageInfo.aucPrevImgFbId[_rImageInfo.u1CurView] = _rImageInfo.aucCurImgFbId[_rImageInfo.u1CurView];
                _rImageInfo.aucCurImgFbId[_rImageInfo.u1CurView] = ucFbId;
            }
            else
            {
                FBM_GetFrameBufferAddr(ucFbgId, _rImageInfo.aucCurImgFbId[_rImageInfo.u1CurView], &u4YAddr, &u4CAddr);
            }
        }

        x_memset((void*)&rInfo, 0, sizeof(IMG_PARAM_T));
        rInfo.eFormat = imgFormat;
        rInfo.pu1InBuffer = NULL;    //use internal ring buffer
        rInfo.u8BufSize = 0;            //use internal ring buffer
        rInfo.rDstRegion.u4StartX = 0;
        rInfo.rDstRegion.u4StartY = 0;

        if (!_rImageInfo.fgLibjpeg)
        {
            if (_rImageInfo.fgIsVdoMode)
            {
                if ((u4DecW > 0) && (u4DecH > 0) && (u4DecW < 1920) && (u4DecH < 1080))
                {
                    MTIMAGE_DEBUG("Specify dec resolution %dx%d\n", u4DecW, u4DecH);
                    rInfo.rDstRegion.u4Width = u4DecW;
                    rInfo.rDstRegion.u4Height = u4DecH;
                }
                else
                {
                    _rImageInfo.u1PicIdx = u4MPFPicIdx;
                    if (u4MPFPicIdx > 0)
                    {
                        _rImageInfo.fgIsMPF = TRUE;
                        rInfo.rDstRegion.u4Width = MPF_HALF_W;    //half size
                        rInfo.rDstRegion.u4Height = MPF_HALF_H*2;  //height will be reduce to 1/2 in decoder, but use 1404x1080 output window
                    }
                    else
                    {
                        _rImageInfo.fgIsMPF = FALSE;
                        rInfo.rDstRegion.u4Width = 0;    //keep original width if < 1920
                        rInfo.rDstRegion.u4Height = 0;   //keep original width if < 1080
                    }
                }
                rInfo.rDstRegion.u4Pitch = 0;    //rVpInfo.u4Width;

                rInfo.eDispColorMode = _rImageInfo.eImgDispCM;
                rInfo.eRotateDegree = CLOCKWISE_0;
                rInfo.bKeepRatio = TRUE;
                if (u4MPFPicIdx > 1)
                {
                    rInfo.prDecOut = &_rImageInfo.rDecOut2;
                    rInfo.rDstRegion.pu1Addr =
                        (UINT8 *)(u4YAddr + _rImageInfo.rDecOut.u4Pitch * _rImageInfo.rDecOut.u4TotalLine);
                }
                else
                {
                    rInfo.prDecOut = &_rImageInfo.rDecOut;
                    rInfo.rDstRegion.pu1Addr = (UINT8 *)u4YAddr;
                }
                rInfo.rDstRegion.u4CbCrOffset = u4CAddr - u4YAddr;
            }
            else
            {

#if (defined(CC_MT5396) || defined(CC_MT5368)||defined(CC_MT5389))
                if (imgFormat == IMAGE_WEBP)
                {
                    if ((u4DecW > 0) && (u4DecH > 0) && (u4DecW <= 1920) && (u4DecH <=1080))
                    {
                        rInfo.rDstRegion.u4Width = u4DecW;
                        rInfo.rDstRegion.u4Height = u4DecH;
                    }
                    else
                    {
                        rInfo.rDstRegion.u4Width = 0;
                        rInfo.rDstRegion.u4Height = 0;
                    }
                }
                else
#endif
                {
                    if ((u4DecW > 0) && (u4DecH > 0) && (u4DecW < 1920) && (u4DecH <1080))
                    {
                        rInfo.rDstRegion.u4Width = u4DecW;
                        rInfo.rDstRegion.u4Height = u4DecH;
                    }
                    else
                    {
                        rInfo.rDstRegion.u4Width = 0;
                        rInfo.rDstRegion.u4Height = 0;
                    }

                }
                rInfo.rDstRegion.u4Pitch = u4Pitch;    //rVpInfo.u4Width;
                MTIMAGE_DEBUG("Specify dec addr 0x%08x\n", u4DecAddr);
                rInfo.eDispColorMode = _rImageInfo.eImgDispCM;
                rInfo.eRotateDegree = CLOCKWISE_0;
                rInfo.bKeepRatio = TRUE;
                rInfo.prDecOut = &_rImageInfo.rDecOut;
                rInfo.rDstRegion.pu1Addr = (UINT8 *)u4DecAddr;
                rInfo.rDstRegion.u4CbCrOffset = 0;
#if (defined(CC_MT5396) || defined(CC_MT5368))
                if (imgFormat == IMAGE_PNG)
                {
                    rInfo.rSrcRegion.u4SrcStartX = 0;
                    rInfo.rSrcRegion.u4SrcStartY = 0;
                    rInfo.rSrcRegion.u4SrcWidth = u4SrcWidth;
                    rInfo.rSrcRegion.u4SrcHeight = u4SrcHeight;
                    rInfo.u4DecColorMode = 12;//GFX_COLORMODE_ARGB_D8888
                }
#endif
            }
        }
        else
        {
            rInfo.rDstRegion.u4Width = u4DecW;
            rInfo.rDstRegion.u4Height = u4DecH;
            rInfo.rDstRegion.u4Pitch = 0;

            rInfo.eDispColorMode = _rImageInfo.eImgDispCM;
            rInfo.eRotateDegree = CLOCKWISE_0;
            rInfo.bKeepRatio = TRUE;
            rInfo.prDecOut = &_rImageInfo.rDecOut;

            rInfo.rDstRegion.pu1Addr = NULL;
            rInfo.rDstRegion.u4CbCrOffset = 0;
            _rImageInfo.u4TgtUsrAddr = u4DecAddr;
        }
        IMG_Set_ImgSize(u4ImgSize);
        if (IMG_Decode(rInfo) != E_IMG_OK)
        {
            MTIMAGE_DEBUG("IMG_Decode NG\n");
            return MTR_NOT_OK;
        }
        else
        {
            MTIMAGE_DEBUG("Decode output Y 0x%x, C 0x%x", (UINT32)rInfo.rDstRegion.pu1Addr,
                          (UINT32)rInfo.rDstRegion.pu1Addr + rInfo.rDstRegion.u4CbCrOffset);
        }
    }
    else
    {
        MTIMAGE_DEBUG("_MTIMAGE_Decode only available when IMG_INIT, Cur %d\n", u4CurState);
        return MTR_NOT_OK;
    }

    return MTR_OK;
}

EXTERN UCHAR VDP_FrcGetLockFrameBuffer(UCHAR ucVdpId); // from /b2r/vdp_frc.h:
/**Set image parameters for display.
*@param prImgSetting          Image setting for display
*@retval TRUE                 Success.
*@retval FALSE                Fail.
*/
static MT_RESULT_T _MTIMAGE_Display(unsigned long arg)
{
    MTIMAGE_HANDLE hHandle;
    MTAL_IOCTL_4ARG_T rArg;
    MTIMAGE_PARAM_T *prImgSetting;
    MTVDO_REGION_T *prRegion;
    MTIMAGE_PARAM_T rImgSetting;
    MTVDO_REGION_T rRegion;
    BOOL fgPartialSrc;
    UCHAR ucFbgId = _rImageInfo.aucFbgId[_rImageInfo.u1CurView];
    IMG_BUF_PARAM_T rCmd={0};
    UINT32 u4YAddr, u4CAddr, u4CurState, u4DispYAddr, u4DispCAddr, u4FrmNum;
    UINT32 u4DstYAddr, u4DstCAddr;
    UCHAR ucFbId, ucSlideShowFbId;
  //  INT32 i4Ret;
    // 3d related parameter
    UINT32 u43DInOut;
    UINT16 u2InFmt, u2OutFmt;
    E_TDTV_DECODER_INPUT eDetectedMode;
    FBM_SEQ_HDR_T* prFbmSeqHdr;
    DRV_OSD_T rOsd;
    FBM_POOL_T *prFbmPoolJPG;
    UINT32 u4List;
    TDTV_3D_CROP_INTO_T cropinfo;

    USR_SPACE_ACCESS_VALIDATE_4ARG(arg);
    COPY_FROM_USER_4ARG(arg, rArg);

    hHandle=(MTIMAGE_HANDLE)rArg.ai4Arg[0];
    prImgSetting=(MTIMAGE_PARAM_T *)rArg.ai4Arg[1];
    fgPartialSrc = (BOOL)rArg.ai4Arg[2];

    if (fgPartialSrc)
    {
        prRegion = (MTVDO_REGION_T *)rArg.ai4Arg[3];
        USR_SPACE_ACCESS_VALIDATE_ARG(prRegion, MTVDO_REGION_T);
        COPY_FROM_USER_ARG(prRegion, rRegion, MTVDO_REGION_T);
    }
    else
    {
        UNUSED(prRegion);
        x_memset((void*)&rRegion,0,sizeof(MTVDO_REGION_T));
    }
    USR_SPACE_ACCESS_VALIDATE_ARG(prImgSetting, MTIMAGE_PARAM_T);
    COPY_FROM_USER_ARG(prImgSetting, rImgSetting, MTIMAGE_PARAM_T);

    _rImageInfo.u4PlaneId = prImgSetting->u1PlaneId;
    u4CurState = IMG_GetDecState();
    if (u4CurState == IMG_IDLE) // will inter idle after decode finish in MW mode
    {
        rCmd.eDispColorMode = _rImageInfo.eImgDispCM;
        rCmd.eRotateDegree = rImgSetting.eRotation;
        rCmd.rSrcRegion.pu1Addr = _rImageInfo.rDecOut.pu1Addr;
        rCmd.rSrcRegion.u4CbCrOffset = _rImageInfo.rDecOut.u4CbCrOffset;
        rCmd.rSrcRegion.u4Depth = _rImageInfo.rDecOut.u4Depth;
        rCmd.rSrcRegion.u4Pitch = _rImageInfo.rDecOut.u4Pitch;
        rCmd.rSrcRegion.u4TotalLine = _rImageInfo.rDecOut.u4TotalLine;
        rCmd.rSrcRegion.u4StartX = (fgPartialSrc) ? rRegion.u4X : _rImageInfo.rDecOut.u4StartX;
        rCmd.rSrcRegion.u4StartY = (fgPartialSrc) ? rRegion.u4Y : _rImageInfo.rDecOut.u4StartY;
        rCmd.rSrcRegion.u4Width = (fgPartialSrc) ? rRegion.u4Width : _rImageInfo.rDecOut.u4Width;
        rCmd.rSrcRegion.u4Height = (fgPartialSrc) ? rRegion.u4Height : _rImageInfo.rDecOut.u4Height;

        rCmd.rDstRegion.u4StartX = rImgSetting.rDstRegion.u4X;
        rCmd.rDstRegion.u4StartY = rImgSetting.rDstRegion.u4Y;
        rCmd.rDstRegion.u4Width = rImgSetting.rDstRegion.u4Width;
        rCmd.rDstRegion.u4Height = rImgSetting.rDstRegion.u4Height;
        rCmd.rDstRegion.u4Depth = _rImageInfo.rDecOut.u4Depth;
        rCmd.bKeepRatio = _rImageInfo.fgIsMPF ? FALSE : TRUE;
        rCmd.bKeepOriResolution = _rImageInfo.fgIsMPF ? FALSE : TRUE;
        //when picture is big enough, we don't keep original resolution and resize it fit to panel
        if ((_rImageInfo.fgIsMPF)||(_rImageInfo.rDecOut.u4Width==MAX_WIDTH)||(_rImageInfo.rDecOut.u4Height==MAX_HEIGHT)
                ||(((rImgSetting.eRotation==MT_IMAGE_ROTATE_90)||(rImgSetting.eRotation==MT_IMAGE_ROTATE_270))&&(_rImageInfo.rDecOut.u4Width>MAX_HEIGHT))
           )
        {
            rCmd.bKeepOriResolution = FALSE;
        }
        else
        {
            rCmd.bKeepOriResolution = TRUE;
        }

        if (_rImageInfo.fgIsMPF)
            rCmd.bKeepOriResolution = TRUE;


        if (_rImageInfo.fgIsVdoMode)
        {
            HalFlushInvalidateDCacheMultipleLine(VIRTUAL((UINT32)rCmd.rSrcRegion.pu1Addr), (UINT32)(rCmd.rSrcRegion.u4CbCrOffset*2));
            if (FBM_GetFrameBufferStatus(ucFbgId, _rImageInfo.aucCurImgFbId[_rImageInfo.u1CurView]) != FBM_FB_STATUS_READY)
            {
                MTIMAGE_DEBUG("Decoded frame not ready!\n");
                return MTR_NOT_OK;
            }
            if (!_AllocFrame(ucFbgId, &_rImageInfo.aucCurDispFbId[_rImageInfo.u1CurView], &u4YAddr, &u4CAddr))
            {
                return MTR_NOT_OK;
            }
            FBM_GetFrameBufferSize(ucFbgId, &rCmd.rDstRegion.u4Pitch, &rCmd.rDstRegion.u4TotalLine);
            rCmd.rDstRegion.pu1Addr = (UINT8*)u4YAddr;
            rCmd.rDstRegion.u4CbCrOffset = u4CAddr - u4YAddr;
            if (rImgSetting.fgClean)
            {   //todo?
            }
        }
        else //osd
        {
            rCmd.rDstRegion.u4Pitch = ALIGN16(rCmd.rDstRegion.u4Width * _rImageInfo.rDecOut.u4Depth);
            rCmd.rDstRegion.u4TotalLine = rImgSetting.rDstRegion.u4Height;
            //  rCmd.rDstRegion.pu1Addr = rCmd.rDstRegion.pu1Addr;
            rCmd.rDstRegion.u4CbCrOffset = 0;
            rOsd.u4Width = rCmd.rDstRegion.u4Width;
            rOsd.u4Height = rCmd.rDstRegion.u4Height;
            rOsd.u4ScaleWidth = rCmd.rDstRegion.u4Width;
            rOsd.u4ScaleHeight = rCmd.rDstRegion.u4Height;
            rOsd.u4Left = rCmd.rDstRegion.u4StartX / 2;
            rOsd.u4Top = rCmd.rDstRegion.u4StartY / 2;

            if ((rCmd.rDstRegion.u4StartX*2>0xffffffff)||(rCmd.rDstRegion.u4StartY*2>0xffffffff))   //shut warning-generator down
            {
                return FALSE;
            }

            if ((rCmd.rDstRegion.u4Width == rCmd.rSrcRegion.u4Width) &&
                    (rCmd.rDstRegion.u4Height == rCmd.rSrcRegion.u4Height))
            {
                rOsd.u4Left = 0;
                rOsd.u4Top = 0;
            }
            else
            {
                rOsd.u4Width = rCmd.rDstRegion.u4Width - rCmd.rDstRegion.u4StartX*2;
                rOsd.u4Height = rCmd.rDstRegion.u4Height - rCmd.rDstRegion.u4StartY*2;
            }
            rCmd.rDstRegion.u4StartX = 0;
            rCmd.rDstRegion.u4StartY = 0;
#if (defined(CC_MT5389))
            prFbmPoolJPG = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_B2R);
#else
            prFbmPoolJPG = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG_OSD1);
#endif
            if (prFbmPoolJPG == NULL)
            {
                return FALSE;
            }
            //rOsd.pu1FrameBaseAddr = BSP_AllocAlignedDmaMemory(rCmd.rDstRegion.u4Pitch * rOsd.u4Height, 16);
            rOsd.pu1FrameBaseAddr = (UINT8 *)PAGE_ALIGN(prFbmPoolJPG->u4Addr);
            //OSD_Init();
            if (rOsd.u4Width* rOsd.u4Height*4>0xffffffff)   //shut warning-generator down
            {
                return FALSE;
            }
            x_memset((void*)VIRTUAL((UINT32)rOsd.pu1FrameBaseAddr),0, rOsd.u4Width* rOsd.u4Height*4);
            OSD_RGN_Create(&_rImageInfo.u4DisplayRegionId, rOsd.u4Width, rOsd.u4Height, (void*)rOsd.pu1FrameBaseAddr,
                           OSD_CM_AYCBCR8888_DIRECT32, rCmd.rDstRegion.u4Pitch, rOsd.u4Left, rOsd.u4Top,
                           rOsd.u4Width, rOsd.u4Height);
            OSD_RGN_LIST_Create(&u4List);
            OSD_RGN_Insert(_rImageInfo.u4DisplayRegionId, u4List);
            OSD_PLA_FlipTo(_rImageInfo.u4PlaneId, u4List);
            OSD_RGN_Set(_rImageInfo.u4DisplayRegionId, (INT32)OSD_RGN_MIX_SEL,
                        (UINT32)OSD_BM_PIXEL);

            rCmd.rDstRegion.pu1Addr = rOsd.pu1FrameBaseAddr;//_rDisplayOSD.pu1FrameBaseAddr;

        }

        if (_rImageInfo.fgIsMPF)
        {
            rCmd.rSrcRegion.u4TotalLine <<= 1;
            rCmd.rSrcRegion.u4Height <<= 1;
        }
        //resize or rotation from decoded frame to display frame
        IMG_Set(rCmd, &_rImageInfo.rDecOut);

        _rMt3DCropinfo.u4X = _rImageInfo.rDecOut.u4StartX;
        _rMt3DCropinfo.u4Y = _rImageInfo.rDecOut.u4StartY;
        _rMt3DCropinfo.u4Width = _rImageInfo.rDecOut.u4Width;
        _rMt3DCropinfo.u4Height = _rImageInfo.rDecOut.u4Height;

        if (_rImageInfo.fgIsVdoMode && rImgSetting.fgDisplay)
        {

            if (_rImageInfo.fgIsMPF)
            {
                eDetectedMode = E_TDTV_DECODER_INPUT_TB_P;
                MTIMAGE_DEBUG("MPO set 3D E_TDTV_DECODER_INPUT_TB_P[%d]\n", _rImageInfo.eDetectedMode);
            }
            else
            {
                eDetectedMode = E_TDTV_DECODER_INPUT_2D_P;
                MTIMAGE_DEBUG("MPO set 2D E_TDTV_DECODER_INPUT_2D_P[%d]\n", _rImageInfo.eDetectedMode);
            }

            _MTIMAGE_3DCropFuncCB(&cropinfo);
            fpDrvTDTVReg3DCropCB(E_TDTV_CB_3D_CROP_MM, _MTIMAGE_3DCropFuncCB);
            u43DInOut = u4DrvTDTVMMModeQuery(eDetectedMode);

            if (eDetectedMode != _rImageInfo.eDetectedMode)
            {
                u43DInOut = u4DrvTDTVMMModeQuery(eDetectedMode);
                u2InFmt = u43DInOut & 0xFFFF;
                u2OutFmt = (u43DInOut >> 16) & 0xFFFF;
                prFbmSeqHdr = FBM_GetFrameBufferSeqHdr(ucFbgId);

                if (prFbmSeqHdr == NULL)
                {
                    ASSERT(prFbmSeqHdr != NULL);
                    return MTR_NOT_OK;
                }
                switch (u2OutFmt)
                {
                case E_TDTV_DECODER_OUTPUT_NATIVE:
                    prFbmSeqHdr->fgB2R3DEnable = false;
                    //MTIMAGE_MPO_DEBUG("MPO set 2D/3D E_TDTV_DECODER_OUTPUT_NATIVE[%d][%d]\n", eDetectedMode, _rImageInfo.eDetectedMode);
                    break;
                case E_TDTV_DECODER_OUTPUT_3D_FS:
                    prFbmSeqHdr->fgB2R3DEnable = TRUE;
                    prFbmSeqHdr->fgB2RForce2D = FALSE;
                    prFbmSeqHdr->u4B2R3DWidthOffset = 0;
                    prFbmSeqHdr->u4B2R3DHeightOffset = rImgSetting.rDstRegion.u4Height >> 1;
                    prFbmSeqHdr->u1B2R3DType = B2R_3D_TOP_N_BOTTOM;
                    break;
                default:
                    break;
                }
                _rImageInfo.eDetectedMode = eDetectedMode;
                FBM_SetFrameBufferPicFlag(ucFbgId, _rImageInfo.aucCurDispFbId[_rImageInfo.u1CurView], FBM_MM_3D_CHG_FLAG);
            }

            //slide show
            if (rImgSetting.fgIsSlideShow)
            {
                _rImageInfo.u1SlideShowEvent = SLIDESHOW_NFY_STOP;
                u4FrmNum = 0;

                do
                {
                    ucFbId = VDP_FrcGetLockFrameBuffer((UCHAR)0);
                    FBM_GetFrameBufferAddr(ucFbgId, ucFbId, &u4DispYAddr, &u4DispCAddr);
                    if (!_AllocFrame(ucFbgId, &ucSlideShowFbId, &u4DstYAddr, &u4DstCAddr))
                    {
                        return MTR_NOT_OK;
                    }
                    MTIMAGE_DEBUG("SlideShow Src Y 0x%x, C 0x%x, Disp Y 0x%x, C 0x%x",
                                  VIRTUAL(u4YAddr), VIRTUAL(u4CAddr), VIRTUAL(u4DispYAddr), VIRTUAL(u4DispCAddr));

                    GFX_Lock();
                    GFX_SetSrc((UINT8 *)u4DispYAddr, (UINT32)CM_RGB_CLUT8, rCmd.rDstRegion.u4Pitch);
                    GFX_SetDst((UINT8 *)u4DstYAddr, (UINT32)CM_RGB_CLUT8, rCmd.rDstRegion.u4Pitch);
                    GFX_BitBlt(0, 0, 0, 0, rCmd.rDstRegion.u4Pitch, rCmd.rDstRegion.u4TotalLine);
                    GFX_SetSrc((UINT8 *)u4DispCAddr, (UINT32)CM_RGB_CLUT8, rCmd.rDstRegion.u4Pitch);
                    GFX_SetDst((UINT8 *)u4DstCAddr, (UINT32)CM_RGB_CLUT8, rCmd.rDstRegion.u4Pitch);
                    GFX_BitBlt(0, 0, 0, 0, rCmd.rDstRegion.u4Pitch, rCmd.rDstRegion.u4TotalLine >> 1);
                    GFX_Flush();
                    GFX_Wait();
                    GFX_Unlock();
#if 0
                    x_img_async_slideshow(_ConvertSlideShowType(_rImageInfo.u1SlideShowType),
                                          VIRTUAL(u4YAddr), VIRTUAL(u4CAddr), VIRTUAL(u4DispYAddr), VIRTUAL(u4DispCAddr),
                                          VIRTUAL(u4DstYAddr), VIRTUAL(u4DstCAddr),
                                          rCmd.rDstRegion.u4CbCrOffset, rCmd.rDstRegion.u4CbCrOffset,
                                          rCmd.rDstRegion.u4Pitch, rCmd.rDstRegion.u4TotalLine, MAX_SLIDE_SHOW_PIC_PERIOD);
                     i4Ret = x_sema_lock_timeout(_rImageInfo.hSlideShowSema, MAX_SLIDE_SHOW_FRM_PERIOD);

                    VERIFY((i4Ret == OSR_OK) || (i4Ret == OSR_TIMEOUT));
                    if (i4Ret == OSR_TIMEOUT)
                    {
                        MTIMAGE_DEBUG("WaitSlideShow timeout\n");
                    }
					
#endif
                    u4FrmNum++;

                    if (_rImageInfo.u1SlideShowEvent == SLIDESHOW_NFY_COMPLETE_ONE_PIC)
                    {
                        FBM_SetFrameBufferStatus(ucFbgId, ucSlideShowFbId, (UCHAR)FBM_FB_STATUS_EMPTY);
                    }
                    else
                    {
                        FBM_SetFrameBufferStatus(ucFbgId, ucSlideShowFbId, (UCHAR)FBM_FB_STATUS_DISPLAYQ);
                    }
                }
                while ((_rImageInfo.u1SlideShowEvent != SLIDESHOW_NFY_COMPLETE_ONE_PIC) &&
                        (u4FrmNum <= MAX_SLIDE_SHOW_FRM_NUM));
                if (_rImageInfo.u1SlideShowEvent != SLIDESHOW_NFY_COMPLETE_ONE_PIC)
                {
                    MTIMAGE_DEBUG("_MTIMAGE_Display SlideShow Fail, event %d~ ", _rImageInfo.u1SlideShowEvent);
                }

                FBM_SetFrameBufferStatus(ucFbgId, _rImageInfo.aucCurDispFbId[_rImageInfo.u1CurView], (UCHAR)FBM_FB_STATUS_EMPTY);
            }
            else
            {
                FBM_SetFrameBufferStatus(ucFbgId, _rImageInfo.aucCurDispFbId[_rImageInfo.u1CurView], (UCHAR)FBM_FB_STATUS_DISPLAYQ);
            }
        }
        else
        {
            OSD_PLA_Enable(_rImageInfo.u4PlaneId, TRUE);
        }

    }
    else
    {
        MTIMAGE_DEBUG("_MTIMAGE_Display only available when IMG_IDLE, Cur %d\n", u4CurState);
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


/**Close jpeg hw
*@param
*@retval TRUE                 Success.
*@retval FALSE                Fail.
*/
static MT_RESULT_T _MTIMAGE_Close(unsigned long arg)
{
    MTIMAGE_HANDLE hHandle;
    UCHAR ucFbgId = _rImageInfo.aucFbgId[_rImageInfo.u1CurView];
    UCHAR ucFbId = _rImageInfo.aucCurImgFbId[_rImageInfo.u1CurView];

    hHandle=(MTIMAGE_HANDLE)arg;
    if (!_rImageInfo.fgLibjpeg)
    {
        if (_rImageInfo.fgIsVdoMode &&
                !(_rImageInfo.u1PicIdx == 1))   //MPF 1st picture
        {
            FBM_UpdateReferenceList(ucFbgId, ucFbId, FALSE);
            FBM_SetFrameBufferStatus(ucFbgId, ucFbId, (UCHAR)FBM_FB_STATUS_EMPTY);
        }
        else
        {
            OSD_PLA_Enable(_rImageInfo.u4PlaneId, FALSE);
            OSD_RGN_Delete(_rImageInfo.u4DisplayRegionId);
        }
    }
    else
    {
        _rImageInfo.fgConnect = FALSE;
        _rImageInfo.fgLibjpeg = FALSE;
    }
    IMG_Release();

    return MTR_OK;
}

/**Stop jpeg processing
*@param
*@retval TRUE                 Success.
*@retval FALSE                Fail.
*/
static MT_RESULT_T _MTIMAGE_Stop(unsigned long arg)
{
    MTIMAGE_HANDLE hHandle;
    hHandle=(MTIMAGE_HANDLE)arg;

    //re-write to abort decoding
    while (IMG_GetDecState() == IMG_DECODE)
    {
        x_thread_delay(1);
    }

    if (IMG_GetDecState() !=IMG_RELEASE)
    {
        IMG_Stop();
        IMG_SetFillBsBufEnd(0, 0, TRUE);
        IMG_Release();
    }

    return MTR_OK;
}

/**Set the stream settings
*@param Image Handle
*@param pSettings
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTIMAGE_Set(unsigned long arg)
{
#if 0
    MTAL_IOCTL_2ARG_T rArg;
    MTIMAGE_HANDLE hHandle;
    MTIMAGE_SETTING_T* prSettings = NULL, rSettings;
    PLAYMGR_SETTING_T rPlaySetting;
    MTIMAGE_INSTANCE_T eInstance;
    INT32 i4Ret = 0;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    hHandle=(MTIMAGE_HANDLE)rArg.ai4Arg[0];
    prSettings=(MTIMAGE_SETTING_T*)rArg.ai4Arg[1];

    USR_SPACE_ACCESS_VALIDATE_ARG(prSettings, MTIMAGE_SETTING_T);
    COPY_FROM_USER_ARG(prSettings, rSettings, MTIMAGE_SETTING_T);

    MTIMAGE_DEBUG(" - void\n");
    //MTIMAGE_SET_PRINT("prSettings",prSettings);

    //add driver implementation code here
    eInstance = _MTIMAGE_GetInstance(hHandle);

    if (eInstance == MTIMAGE_IMAGE)
    {
        _MTIMAGE_OpenImg(hHandle, rSettings.eColorFormat);
        //MT_IMAGE_COLOR_FORMAT_ARGB8888
        dwJpgTgtWidth = rSettings.u2VTrack;
        dwJpgTgtHeight = rSettings.u2ATrack;
        dwJpgTgtPtr = rSettings.pu1SubtitleBuffer;
        uJpgColorFmt = rSettings.eColorFormat;
    }
    else
    {
        MTIMAGE_DEBUG(" - Instance Error \n");
        return MTR_NOT_OK;
    }
#endif

    return MTR_OK;
}


/**Get the stream settings and status
*@param handle
*@param prSettings
*@param prStatus
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTIMAGE_GetInfo(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTIMAGE_HANDLE hHandle;
    UINT32 u4CurState;
    MT_RESULT_T eResult = MTR_NOT_OK;
    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);
    hHandle = (MTIMAGE_HANDLE)rArg.ai4Arg[0];

    u4CurState = IMG_GetDecState();
    rArg.ai4Arg[1] = u4CurState;
//    printk("_MTIMAGE_GetInfo u4CurState = %d\n",u4CurState);
    if (u4CurState == IMG_RELEASE)
    {
        eResult = MTR_OK;
    }

    USR_SPACE_ACCESS_VALIDATE_ARG(arg, MTAL_IOCTL_2ARG_T);
    COPY_TO_USER_ARG(arg, rArg, MTAL_IOCTL_2ARG_T);

    return eResult;
}


//MT_IMAGE_COLOR_FORMAT_ARGB8888
static void  _MTIMAGE_IMGYUV2RGB(unsigned long arg)
{
    MTIMAGE_HANDLE hHandle;
    UINT32 u4YSrcLen=0;
    UINT32 u4ARGBSize = 0;
    FBM_POOL_T *prFbmPoolJPG;
    UINT32  u4Colorfmt,u4Pitch,u4Addr,u4Size;

    u4YSrcLen=(_rImageInfo.rDecOut.u4Width + 0xF) & (~0xF);
    u4ARGBSize = (UINT32)(u4YSrcLen*(_rImageInfo.rDecOut.u4Height)*4);
//    printk("_MTIMAGE_IMGYUV2RGB\n");
    switch (_rImageInfo.eImgColorFmt)
    {
    case MT_IMAGE_COLOR_FORMAT_RGB565:
        u4Colorfmt = CM_RGB565_DIRECT16;
        u4Pitch = u4YSrcLen<<1;
        break;
    case MT_IMAGE_COLOR_FORMAT_RGB888:
    default:
        u4Colorfmt = CM_ARGB8888_DIRECT32;
        u4Pitch = u4YSrcLen<<2;
        break;
    }

    u4Addr = (UINT32)_rImageInfo.rDecOut.pu1Addr-u4ARGBSize;
    u4Size = u4Pitch*_rImageInfo.rDecOut.u4Height;
    prFbmPoolJPG = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG);
    ASSERT(prFbmPoolJPG != NULL);
    ASSERT(prFbmPoolJPG->u4Addr != 0);

    hHandle=(MTIMAGE_HANDLE)arg;

    GFX_Lock();
    GFX_SetDst((_rImageInfo.rDecOut.pu1Addr-u4ARGBSize), u4Colorfmt,u4Pitch);
    GFX_SetColCnvSrc((_rImageInfo.rDecOut.pu1Addr),u4YSrcLen,(_rImageInfo.rDecOut.pu1Addr+_rImageInfo.rDecOut.u4CbCrOffset),u4YSrcLen,0);
    GFX_SetColCnvFmt(E_YCFMT_422LINEAR,E_SWAP_0,E_VSTD_BT709,E_VSYS_COMP);
    GFX_SetAlpha((UINT32)0xff);
    GFX_ColConv(0,0,_rImageInfo.rDecOut.u4Width,_rImageInfo.rDecOut.u4Height);
    GFX_Flush();
    GFX_Wait();
    GFX_Unlock();

    //copy_to_user( (void __user *)_rImageInfo.u4TgtUsrAddr ,VIRTUAL(u4Addr),u4Size);
}

static MT_RESULT_T _MTIMAGE_GetImg(unsigned long arg)
{
    MTAL_IOCTL_8ARG_T rArg;
    MTIMAGE_HANDLE hHandle;
    FBM_POOL_T *prFbmPool;
    UINT32 u4YAddr, u4CAddr;

    USR_SPACE_ACCESS_VALIDATE_8ARG(arg);
    COPY_FROM_USER_8ARG(arg, rArg);
    hHandle = (MTIMAGE_HANDLE)rArg.ai4Arg[0];
//    printk("_MTIMAGE_GetImg\n");
#ifdef MTIMAGE_ANDROID_SUPPORT
    if (_rImageInfo.fgLibjpeg)//jpglib format.
#else
    if (_rImageInfo.eImgDispCM == AYCBCR8888_DIRECT32)//jpglib format.
#endif
    {
        UINT32 u4YSrcLen=0;
        UINT32 u4ARGBSize = 0;
        u4YSrcLen=(_rImageInfo.rDecOut.u4Width + 0xF) & (~0xF);
        u4ARGBSize = (UINT32)(u4YSrcLen*(_rImageInfo.rDecOut.u4Height)*4);


        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_JPEG);

        if (_rImageInfo.eImgDispCM == ARGB8888_DIRECT32)
        {
            x_memcpy((void*)VIRTUAL((UINT32)_rImageInfo.rDecOut.pu1Addr -u4ARGBSize),(void*)VIRTUAL((UINT32)_rImageInfo.rDecOut.pu1Addr),u4ARGBSize);
        }
        else
        {
            _MTIMAGE_IMGYUV2RGB((unsigned long)hHandle);
        }

        rArg.ai4Arg[1] = (INT32)_rImageInfo.rDecOut.pu1Addr -u4ARGBSize - prFbmPool->u4Addr ;
        rArg.ai4Arg[2] = _rImageInfo.rDecOut.u4CbCrOffset; //size
        rArg.ai4Arg[3] = (INT32)_rImageInfo.rDecOut.u4StartX;
        rArg.ai4Arg[4] = (INT32)_rImageInfo.rDecOut.u4StartY;
        rArg.ai4Arg[5] = (INT32)_rImageInfo.rDecOut.u4Width;
        rArg.ai4Arg[6] = (INT32)_rImageInfo.rDecOut.u4Height;

        USR_SPACE_ACCESS_VALIDATE_ARG(arg, MTAL_IOCTL_8ARG_T);
        COPY_TO_USER_ARG(arg, rArg, MTAL_IOCTL_8ARG_T);
    }
    else
    {
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_MPEG);

        FBM_GetFrameBufferAddr(_rImageInfo.aucFbgId[_rImageInfo.u1CurView],
                               _rImageInfo.aucCurImgFbId[_rImageInfo.u1CurView], &u4YAddr, &u4CAddr);

        rArg.ai4Arg[1] = u4YAddr - prFbmPool->u4Addr; //addr offset
        rArg.ai4Arg[2] = u4CAddr - u4YAddr; //size
        rArg.ai4Arg[3] = (INT32)_rImageInfo.rDecOut.u4StartX;
        rArg.ai4Arg[4] = (INT32)_rImageInfo.rDecOut.u4StartY;
        rArg.ai4Arg[5] = (INT32)_rImageInfo.rDecOut.u4Width;
        rArg.ai4Arg[6] = (INT32)_rImageInfo.rDecOut.u4Height;

        MTIMAGE_DEBUG(" - GetImg YOffset 0x%c, Sz %x \n", rArg.ai4Arg[1], rArg.ai4Arg[2]);
        USR_SPACE_ACCESS_VALIDATE_ARG(arg, MTAL_IOCTL_8ARG_T);
        COPY_TO_USER_ARG(arg, rArg, MTAL_IOCTL_8ARG_T);
    }
    return MTR_OK;
}


/**Get a/v meta data
*@param handle
*@param prMeta
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTIMAGE_GetMetaData(unsigned long arg)
{
    MT_RESULT_T eResult = MTR_OK;
#if 0
    MTAL_IOCTL_2ARG_T rArg;
    MTIMAGE_HANDLE hHandle;
    MTIMAGE_AVMETA_T *prMeta=NULL;
    MTIMAGE_AVMETA_T   rMeta;
    PLAYMGR_AVMETA_T rPlaymgrMeta;
    MTIMAGE_INSTANCE_T eInstance;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    hHandle = (MTIMAGE_HANDLE)rArg.ai4Arg[0];
    prMeta = (MTIMAGE_STATUS_T *)rArg.ai4Arg[1];

//add driver implementation code here
    eInstance = _MTIMAGE_GetInstance(hHandle);

    if ((eInstance == MTIMAGE_PLAYMGR) || (eInstance == MTIMAGE_AUDIO))
    {
        if (prMeta == NULL)
        {
            MTIMAGE_DEBUG(" - meta data Error \n");
            return MTR_NOT_OK;
        }
        x_memset((void *)&rPlaymgrMeta, 0, sizeof(PLAYMGR_AVMETA_T));
#ifdef CC_53XX_SWDMX_V2
        PLAYMGR_GetMetaData(MTIMAGE_PLAYMGR0, &rPlaymgrMeta);
#else
        PLAYMGR_GetMetaData(&rPlaymgrMeta);
#endif
        x_memcpy((void *)rMeta.au2Title, (void *)rPlaymgrMeta.pu2Title, rPlaymgrMeta.u4TitleLen);
        x_memcpy((void *)rMeta.au2Album, (void *)rPlaymgrMeta.pu2Album, rPlaymgrMeta.u4AlbumLen);
        x_memcpy((void *)rMeta.au2Artist, (void *)rPlaymgrMeta.pu2Artist, rPlaymgrMeta.u4ArtistLen);
        x_memcpy((void *)rMeta.au2Genre, (void *)rPlaymgrMeta.pu2Genre, rPlaymgrMeta.u4GenreLen);
        rMeta.u4SampleFreq = rPlaymgrMeta.u4SampleFreq;
        rMeta.u4BitRate = rPlaymgrMeta.u4BitRate;
        rMeta.u4FrameSize = rPlaymgrMeta.u4FrameSize;
        rMeta.u4FrameRate = rPlaymgrMeta.u4FrameRate;
        rMeta.u4PicPos = rPlaymgrMeta.u4ImgPos;
        rMeta.u4PicSize = rPlaymgrMeta.u4ImgSize;
        rMeta.u4Width = rPlaymgrMeta.u4Width;
        rMeta.u4Height = rPlaymgrMeta.u4Height;
        rMeta.u4ParWidth = rPlaymgrMeta.u4ParWidth;
        rMeta.u4ParHeight = rPlaymgrMeta.u4ParHeight;
        rMeta.u8MediaSize = rPlaymgrMeta.u8MediaSize;
        rMeta.u8PlayTime = rPlaymgrMeta.u8PlayTime;
    }
    else
    {
        MTIMAGE_DEBUG(" - Instance Error \n");
        return MTR_NOT_OK;
    }
    USR_SPACE_ACCESS_VALIDATE_ARG(prMeta,MTIMAGE_AVMETA_T);
    COPY_TO_USER_ARG(prMeta,rMeta,MTIMAGE_AVMETA_T);

    MTIMAGE_DEBUG(" - void\n");
    //MTIMAGE_STATUS_PRINT("rStatus",&rStatus);
#endif
    return eResult;
}


/**Start slide show
*@param slide type
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTIMAGE_SlideShow(unsigned long arg)
{
    MTAL_IOCTL_2ARG_T rArg;
    MTIMAGE_HANDLE hHandle;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    hHandle = (MTIMAGE_HANDLE)rArg.ai4Arg[0];
    _rImageInfo.u1SlideShowType = (UINT8)rArg.ai4Arg[1];

#if 0
    eInstance = _MTIMAGE_GetInstance(hHandle);
    if (eInstance != MTIMAGE_IMAGE)
    {
        MTIMAGE_DEBUG(" - Instance Error \n");
        return MTR_NOT_OK;
    }

    if (IMG_GetVideoPlaneInfo(&rVpInfo) != E_IMG_OK)
    {
        return MTR_NOT_OK;
    }
#if 0
    if (!_fgVdpConnect)
    {
        if (IMG_VDP_Connect(_u4PlaneId) != E_IMG_OK)
        {
            return FALSE;
        }
        else
        {
            _fgVdpConnect = TRUE;
            IMG_Flip();
            IMG_Flip();
        }
    }
#endif

    switch (eSlideType)
    {
    case MT_MPLAYER_EFFECT_BARWIPE_LR:
        MTIMAGE_BarWipeH(rVpInfo, TRUE);
        break;
    case MT_MPLAYER_EFFECT_BARWIPE_RL:
        MTIMAGE_BarWipeH(rVpInfo, FALSE);
        break;
    case MT_MPLAYER_EFFECT_BARWIPE_TB:
        MTIMAGE_BarWipeV(rVpInfo, TRUE);
        break;
    case MT_MPLAYER_EFFECT_BARWIPE_BT:
        MTIMAGE_BarWipeV(rVpInfo, FALSE);
        break;
    case MT_MPLAYER_EFFECT_BOXWIPE_TL:
        MTIMAGE_BoxWipe(rVpInfo, TRUE, TRUE);
        break;
    case MT_MPLAYER_EFFECT_BOXWIPE_TR:
        MTIMAGE_BoxWipe(rVpInfo, TRUE, FALSE);
        break;
    case MT_MPLAYER_EFFECT_BOXWIPE_BL:
        MTIMAGE_BoxWipe(rVpInfo, FALSE, TRUE);
        break;
    case MT_MPLAYER_EFFECT_BOXWIPE_BR:
        MTIMAGE_BoxWipe(rVpInfo, FALSE, FALSE);
        break;
    case MT_MPLAYER_EFFECT_DOORWIPE_HC:
        MTIMAGE_DoorWipeH(rVpInfo, TRUE);
        break;
    case MT_MPLAYER_EFFECT_DOORWIPE_HO:
        MTIMAGE_DoorWipeH(rVpInfo, FALSE);
        break;
    case MT_MPLAYER_EFFECT_DOORWIPE_VC:
        MTIMAGE_DoorWipeV(rVpInfo, TRUE);
        break;
    case MT_MPLAYER_EFFECT_DOORWIPE_VO:
        MTIMAGE_DoorWipeV(rVpInfo, FALSE);
        break;
    case MT_MPLAYER_EFFECT_IRISWIPE:
        MTIMAGE_IrisWipe(rVpInfo);
        break;
    case MT_MPLAYER_EFFECT_NONE:
        IMG_Flip();
        break;
    default:
        return MTR_NOT_OK;
    }
#endif
    return MTR_OK;
}



/**clean the img frame buffer
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTIMAGE_CleanFB(unsigned long arg)
{
    return MTR_OK;
}


/**Get capability.
*@param type
*@param parameter 1
*@param parameter 2
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
static MT_RESULT_T _MTIMAGE_GetCap(unsigned long arg)
{
#if 0
    MTAL_IOCTL_3ARG_T rArg;
    UINT32 u4Type;
    UINT32 u4Param1;
    UINT32 u4Param2;

    USR_SPACE_ACCESS_VALIDATE_3ARG(arg);
    COPY_FROM_USER_3ARG(arg, rArg);

    u4Type = (UINT32)rArg.ai4Arg[0];
    u4Param1 = (UINT32)rArg.ai4Arg[1];
    u4Param2 = (UINT32)rArg.ai4Arg[2];

    MTIMAGE_DEBUG(" - u4Type = %u  u4Param1 = %u  u4Param2 = %u\n", u4Type, u4Param1, u4Param2);

#ifdef CC_53XX_SWDMX_V2
    if (!PLAYMGR_GetCap(MTIMAGE_PLAYMGR0, u4Type, u4Param1, u4Param2))
#else
    if (1/*!PLAYMGR_GetCap(u4Type, u4Param1, u4Param2)*/) // V1 does not have this API yet!!!
#endif
    {
        UNUSED(u4Type);
        UNUSED(u4Param1);
        UNUSED(u4Param2);
        MTIMAGE_DEBUG(" - PLAYMGR_GetCap return FALSE\n");
        return MTR_NOT_OK;
    }

    UNUSED(u4Type);
    UNUSED(u4Param1);
    UNUSED(u4Param2);
#endif
    return MTR_OK;
}


static MT_RESULT_T _MTIMAGE_SetBufFilled(unsigned long arg)
{
    MTIMAGE_HANDLE hHandle;
    static UINT32 u4FillLen1 = 0;
    MTAL_IOCTL_2ARG_T rArg;
    MTIMAGE_BUF_FILLED_T* prBufFillInfo;
    MTIMAGE_BUF_FILLED_T rBufFillInfo;

    USR_SPACE_ACCESS_VALIDATE_2ARG(arg);
    COPY_FROM_USER_2ARG(arg, rArg);

    hHandle = (MTIMAGE_HANDLE)rArg.ai4Arg[0];
    prBufFillInfo = (MTIMAGE_BUF_FILLED_T*)rArg.ai4Arg[1];

    USR_SPACE_ACCESS_VALIDATE_ARG(prBufFillInfo, MTIMAGE_BUF_FILLED_T);
    COPY_FROM_USER_ARG(prBufFillInfo, rBufFillInfo, MTIMAGE_BUF_FILLED_T);

    if (rBufFillInfo.u4_trsn_id == 1)
    {
        if (u4FillLen1 == 0)
        {
            IMG_SetFillBsBufEnd(rBufFillInfo.u4_filled_len, 0,
                                (rBufFillInfo.i4_ret==0)?FALSE:TRUE);   //IMG_MW_FILL_OK
        }
        else
        {
            IMG_SetFillBsBufEnd(u4FillLen1, rBufFillInfo.u4_filled_len,
                                (rBufFillInfo.i4_ret==0)?FALSE:TRUE);   //IMG_MW_FILL_OK
        }
        u4FillLen1 = 0;
    }
    else
    {
        u4FillLen1 = rBufFillInfo.u4_filled_len;
    }

    return MTR_OK;
}


/**Initialize jpeg hw for decoding.
*@param eColorFMT            display color format
*@retval TRUE                 Success.
*@retval FALSE                Fail.
*/
static MT_RESULT_T _MTIMAGE_ConfigImagedecoder(unsigned long arg)
{
    ENUM_IMG_FORMAT	imgFormat;

    imgFormat=(ENUM_IMG_FORMAT)arg;

    _rImageInfo.imgFormat = imgFormat;  // set format for _MTIMAGE_Open user later
    if (IMG_Config(imgFormat) != E_IMG_OK)
    {
        MTIMAGE_DEBUG("IMG_Config NG\n");
        return MTR_NOT_OK;
    }

    return MTR_OK;
}


int mtal_ioctl_mtimage(struct inode *inode, struct file *file, unsigned int cmd,
                       unsigned long arg)
{
    switch (cmd)
    {
    case MTAL_IO_IMAGE_CONNECT:
        return  _MTIMAGE_Connect(arg);
    case MTAL_IO_IMAGE_OPEN:
        return  _MTIMAGE_Open(arg);
    case MTAL_IO_IMAGE_DECODE:
        return  _MTIMAGE_Decode(arg);
    case MTAL_IO_IMAGE_DISPLAY:
        return  _MTIMAGE_Display(arg);
    case MTAL_IO_IMAGE_CLOSE:
        return  _MTIMAGE_Close(arg);
    case MTAL_IO_IMAGE_STOP:
        return  _MTIMAGE_Stop(arg);
    case MTAL_IO_IMAGE_SET:
        return  _MTIMAGE_Set(arg);
    case MTAL_IO_IMAGE_GETINFO:
        return  _MTIMAGE_GetInfo(arg);
    case MTAL_IO_IMAGE_GETIMG:
        return  _MTIMAGE_GetImg(arg);
    case MTAL_IO_IMAGE_GETMETA:
        return  _MTIMAGE_GetMetaData(arg);
    case MTAL_IO_IMAGE_SLIDESHOW:
        return  _MTIMAGE_SlideShow(arg);
    case MTAL_IO_IMAGE_CLEANFB:
        return _MTIMAGE_CleanFB(arg);
    case MTAL_IO_IMAGE_GETCAP:
        return _MTIMAGE_GetCap(arg);
    case MTAL_IO_IMAGE_FILLBUF:
        return _MTIMAGE_SetBufFilled(arg);
    case MTAL_IO_IMAGE_CONFIGIMGDECODER:
        return _MTIMAGE_ConfigImagedecoder(arg);
    default:
        return  MTR_NOT_OK;
    }
    return MTR_OK;
}

