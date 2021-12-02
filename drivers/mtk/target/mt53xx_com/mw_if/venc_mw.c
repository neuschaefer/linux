/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: venc_mw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file venc_mw.c
 *  This file defines the interfaces for middleware to access MPV driver
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------


#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "venc_if.h"
#include "venc_util_drvif.h"
#include "vdec_drvif.h"
#include "drv_comp_id.h"
#include "drv_name.h"

#define DEFINE_IS_LOG    VENC_IsLog
#include "x_debug.h"
#include "x_assert.h"

#include "x_rm.h"
#ifndef LINUX_TURNKEY_SOLUTION
#include "x_rm_dev_types.h"
#else
#include "u_rm_dev_types.h"
#endif
#include "x_vid_enc.h"
#ifndef LINUX_TURNKEY_SOLUTION
#include "u_mm_common.h"
#endif

#ifdef LINUX_TURNKEY_SOLUTION
#include "cb_data_rm.h"
#include "cb_param.h"
#endif

#ifdef VENC_TO_MUXER_IF
#include "muxer_if.h"
#endif

#include "drvcust_if.h"
#include "u_drv_cust.h"
LINT_EXT_HEADER_END



//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
VENC_BUFMGR_DECLARE(VENC_FRM, VENC_FRM_T, 10);

typedef struct _VENC_MW_INFO_T
{
    VENC_HANDLE    hVEnc;
    BOOL           fgIsRunning;

    UINT32         u4Width;
    UINT32         u4Height;
    UINT32         u4Fps;
    UINT32         u4Bps;

    UINT32         u4EncType;
    VENC_PROFILE_T eProfileDef;
    UINT32         u4Profile;
    UINT32         u4Level;

    VENC_PIX_FMT_T ePixFmt;

    UINT16         u2CompId;
    DRV_TYPE_T     eDrvType;
    VID_SRC_TYPE_T eSrcType;

    VENC_BUFMGR_BASE_T * ptFrmBuffMgr;
    INT32 (*SetFmtInfo)     (struct _VENC_MW_INFO_T *, VOID *);
    INT32 (*ConnectInput)   (struct _VENC_MW_INFO_T *);
    INT32 (*DisconnectInput)(struct _VENC_MW_INFO_T *);
    INT32 (*SetInputParam)  (struct _VENC_MW_INFO_T *);
} VENC_MW_INFO_T;

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//lint --e{717} do ... while (0);
#define M_GET_VALIDATE(TYPE)             \
    do                                    \
    {                                    \
        if (sizeof(TYPE) > *pz_get_info_len)    \
        {                                \
            *pz_get_info_len = sizeof(TYPE);    \
            return RMR_DRV_NOT_ENOUGH_SPACE;\
        }                                \
        *pz_get_info_len = sizeof(TYPE);        \
    } while (0)

#define M_SET_VALIDATE(TYPE)             \
    do                                    \
    {                                    \
        if (sizeof(TYPE) > z_set_info_len)    \
        {                                \
            return RMR_DRV_INV_SET_INFO;\
        }                                \
    } while (0)

#define _ALIGN_MASK(VAL, MSK) (((VAL) + (MSK)) & (~(MSK)))

#define _VENC_MAX_RES 0xE6000 //(1280x736)
//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern UCHAR _CompIdtoEsId(UINT16 u2CompId);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static VENC_MW_INFO_T _arVEncMwInfo[VENC_COMP_NS];


//-----------------------------------------------------------------------------
// Static functions declare
//-----------------------------------------------------------------------------
static VOID _VEncMwCb(VOID *pvArg, VENC_IN_PIC_T * ptIn, VENC_OUT_PIC_T * ptOut, UINT32 type);


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
//--ouput 
#ifdef VENC_TO_MUXER_IF
static VOID _VEncMwCbToMuxer(VOID *pvArg, VENC_IN_PIC_T * ptIn, VENC_OUT_PIC_T * ptOut, UINT32 type)
{
    _VEncMwCb(pvArg, ptIn, ptOut, type);
    
    if (ptOut)
    {
        VENC_PES_INFO_T rVEncPes;
        // send msg to mux    
        // picSa, picSz, PTS, picType    
        rVEncPes.u4FifoStart      = ptOut->u4RStart;
        rVEncPes.u4FifoEnd        = ptOut->u4REnd;
        rVEncPes.u4StartAddr      = ptOut->u4Addr;
        rVEncPes.u4FrameSize      = ptOut->u4Size;  
        rVEncPes.u4VideoType      = (UINT32)2; // set anything excpet 1, MUX should provide transform api for ePicType later   
        rVEncPes.u4Pts            = ptOut->u4Pts;    
        rVEncPes.u2SkipFrameCount = 0;    
        MUXER_SendVideoPes(0, &rVEncPes);    
    }
}
#endif /*VENC_TO_MUXER_IF*/

//--input source settings
#ifdef VDEC_VENC_PATH
static INT32 _VEncMwConnectVdec(VENC_MW_INFO_T *prVEncMwInfo) 
{
    UCHAR ucEsId;
    if (prVEncMwInfo->u2CompId >= (UINT16)MPV_COMP_NS)
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    ucEsId = _CompIdtoEsId(prVEncMwInfo->u2CompId);
    _VDEC_Set2VencFlow(ucEsId, TRUE);

    return RMR_OK;
}

static INT32 _VEncMwDisconnectVdec(VENC_MW_INFO_T *prVEncMwInfo) 
{
    UCHAR ucEsId;
    ucEsId = _CompIdtoEsId(prVEncMwInfo->u2CompId);
    _VDEC_Set2VencFlow(ucEsId, FALSE);

    return RMR_OK;
}

static INT32 _VEncMwSetParamVdec(VENC_MW_INFO_T *prVEncMwInfo) 
{
    UCHAR ucEsId;
    ucEsId = _CompIdtoEsId(prVEncMwInfo->u2CompId);
    _VDEC_SetVencHandle(ucEsId, (VOID *)prVEncMwInfo->hVEnc, (VOID *)prVEncMwInfo->ptFrmBuffMgr, NULL);
    _VDEC_SetVencFrmSz(ucEsId, prVEncMwInfo->u4Width, prVEncMwInfo->u4Height, prVEncMwInfo->u4Width);
    return RMR_OK;
}
#endif /*VDEC_VENC_PATH*/


static INT32 _VEncMwSetH264Info(
    VENC_MW_INFO_T *prVEncMwInfo, 
    VOID *pvFmt)
{
    UINT32 u4Width, u4Height, u4Fps, u4Bps;
    VID_ENC_COMMON_INFO_T  *pt_vid_enc_info;
    VID_FRAME_RESOLUTION_T  *pt_resolution;
    VID_ENC_H264_INFO_T *pt_fmt_h264 = (VID_ENC_H264_INFO_T *)pvFmt;


    if (NULL == prVEncMwInfo)
    {
        LOG(1, "%s: prVEncMwInfo is NULL\n", __FUNCTION__);
        return RMR_DRV_INV_SET_INFO;
    }

    if (NULL == pt_fmt_h264)
    {
        LOG(1, "%s: pt_fmt_h264 is NULL\n", __FUNCTION__);
        return RMR_DRV_INV_SET_INFO;
    }

    pt_vid_enc_info = &pt_fmt_h264->t_vid_enc_info;
    u4Bps           = pt_vid_enc_info->ui4_bitrate;
    u4Fps           = pt_vid_enc_info->ui4_fps;
    pt_resolution   = &pt_vid_enc_info->t_resolution;
    u4Width         = pt_resolution->ui4_width;
    u4Height        = pt_resolution->ui4_height;

    if (prVEncMwInfo->fgIsRunning)
    {
        VENC_RT_PARM_T rRTParm;
        //RESOLUTION
        rRTParm.u4Flag      = VENC_RT_PARM_FLAG_RES;
        rRTParm.u2Width     = u4Width;
        rRTParm.u2Height    = u4Height;
        rRTParm.u2Pitch     = u4Width;
        
        //bitrate
        rRTParm.u4Flag     |= VENC_RT_PARM_FLAG_BITRATE;
        rRTParm.u4Bitrate   = u4Bps;
        
        //framterate
        rRTParm.u4Flag     |= VENC_RT_PARM_FLAG_FRAMERATE;
        rRTParm.u1Framerate = u4Fps;  

        if (VENC_OK != VENC_SetRtParamAsync(prVEncMwInfo->hVEnc, &rRTParm))
        {
            return RMR_DRV_SET_FAILED;
        }
    }
    else
    {
        prVEncMwInfo->u4Profile = 0;
        prVEncMwInfo->u4Level   = 0;
        prVEncMwInfo->u4Width   = u4Width;
        prVEncMwInfo->u4Height  = u4Height;
        prVEncMwInfo->u4Fps     = u4Fps;
        prVEncMwInfo->u4Bps     = u4Bps;
    }

    return RMR_OK;
}


static VENC_MW_INFO_T *_VEncMwGetInfoInst(UINT16 u2CompId)
{
    return (u2CompId < (UINT16)VENC_COMP_NS)? &_arVEncMwInfo[u2CompId] : NULL;
}
static VOID _VEncMwCb(VOID *pvArg, VENC_IN_PIC_T * ptIn, VENC_OUT_PIC_T * ptOut, UINT32 type)
{
    VENC_MW_INFO_T *prVEncMwInfo = (VENC_MW_INFO_T *)pvArg;
    if (ptIn)
    {
        VENC_FRM_T rFrm;
        rFrm.u4YAddr  = ptIn->u4YAddr;
        rFrm.u4UVAddr = ptIn->u4CAddr;
        _BUFMGR_OBJ_Free(prVEncMwInfo->ptFrmBuffMgr, (VOID *)&rFrm);
        //Printf("_emu_demo_cb free: src(Y,UV) => (0x%08x, 0x%08x)\n", ptIn->u4YAddr, ptIn->u4CAddr);
    }
    UNUSED(ptOut);
    UNUSED(type);
}

static INT32 _VEncMwSetEncFmt(
    VENC_MW_INFO_T *prVEncMwInfo, 
    VID_ENC_FMT_TYPE_T e_enc_fmt)
{
    UINT32 type = 0;
    switch (e_enc_fmt)
    {
    default:
        prVEncMwInfo->SetFmtInfo = NULL;
        return RMR_DRV_INV_SET_INFO;
    case VID_ENC_FMT_H264:
        type = VENC_H264;
        prVEncMwInfo->SetFmtInfo = _VEncMwSetH264Info;
        break;
    }
    prVEncMwInfo->u4EncType = type;
    return RMR_OK;
}

static INT32 _VEncMwSetSrcType(
    VENC_MW_INFO_T *prVEncMwInfo, 
    VID_SRC_TYPE_T e_src_type)
{
    switch(e_src_type)
    {
    default:
        return RMR_DRV_INV_SET_INFO;
    case VID_ENC_SRC_TYPE_VDEC: //skype
        prVEncMwInfo->ePixFmt     = VENC_PIX_FMT_NV12;
        prVEncMwInfo->eProfileDef = VENC_PROFILE_VPHONE;
        #ifdef VDEC_VENC_PATH
        prVEncMwInfo->ConnectInput    = _VEncMwConnectVdec;
        prVEncMwInfo->DisconnectInput = _VEncMwDisconnectVdec;
        prVEncMwInfo->SetInputParam   = _VEncMwSetParamVdec;
        #endif /*VDEC_VENC_PATH*/
        break;
    case VID_ENC_SRC_TYPE_TVD: //PRV
        prVEncMwInfo->ePixFmt     = VENC_PIX_FMT_PSCAN;
        prVEncMwInfo->eProfileDef = VENC_PROFILE_PVR;
        break;
    }

    prVEncMwInfo->eSrcType = e_src_type;

    return RMR_OK;
}


static INT32 _VEncMwSetFmtInfo(
    VENC_MW_INFO_T *prVEncMwInfo, 
    VOID *pvFmt)
{

    if (NULL == prVEncMwInfo->SetFmtInfo)
    {
        return RMR_NOT_INIT;
    }

    return prVEncMwInfo->SetFmtInfo(prVEncMwInfo, pvFmt);
}

static INT32 _VEncMwGetCtrl(VENC_MW_INFO_T *prVEncMwInfo, const VOID* pv_get_info, SIZE_T *pz_get_info_len)
{
    BOOL *pfgCtrl;
    M_GET_VALIDATE(BOOL);

    if (NULL == prVEncMwInfo)
    {
        return RMR_DRV_GET_FAILED;
    }

    if (NULL == pv_get_info)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    pfgCtrl = (BOOL *)pv_get_info;
    *pfgCtrl = prVEncMwInfo->fgIsRunning;

    return RMR_OK;
}


static INT32 _VEncMwSetCtrl(VENC_MW_INFO_T *prVEncMwInfo, const VOID* pv_set_info, SIZE_T z_set_info_len)
{
    BOOL fgCtrl;

    M_SET_VALIDATE(BOOL);

    if (NULL == prVEncMwInfo)
    {
        return RMR_DRV_SET_FAILED;
    }

    fgCtrl = (pv_set_info)? TRUE : FALSE;

    if (prVEncMwInfo->fgIsRunning != fgCtrl)
    {
        if (fgCtrl)
        {
            UINT32 u4Value;
            VENC_PRAM_RES_T res;
            VENC_CTRL_PIXFMT_T pix;

            //open encoder
            if (VENC_NULL_HANDLE == prVEncMwInfo->hVEnc) 
            {
                //##open
                prVEncMwInfo->hVEnc = VENC_Open(prVEncMwInfo->u4EncType, prVEncMwInfo->eProfileDef);
                if (VENC_NULL_HANDLE == prVEncMwInfo->hVEnc)
                {
                    LOG(1, "%s: venc open failed!\n", __FUNCTION__);
                    return RMR_DRV_SET_FAILED;
                }

                //##buffer allocation
                {
                UINT32 u4Size;
                VENC_FRM_T rFrm;
                UINT32 u4Idx, u4TmpAddr;
                FBM_POOL_T *prFbmPool;
                VENC_CTRL_BUFF_T buff;
                prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_VENC);
                
                ASSERT(prFbmPool != NULL);
                ASSERT(prFbmPool->u4Addr != 0);
                ASSERT(prFbmPool->u4Size != 0);

                //--raw data buffer
                prVEncMwInfo->ptFrmBuffMgr = (VENC_BUFMGR_BASE_T *)VENC_BUFMGR_OBJ_CREATE(VENC_FRM); 
                ASSERT(prVEncMwInfo->ptFrmBuffMgr);
                _BUFMGR_OBJ_Reset(prVEncMwInfo->ptFrmBuffMgr);
                u4Size = _ALIGN_MASK(res.u2Width, 31) * _ALIGN_MASK(res.u2Height, 31); 
                u4Size = MAX(u4Size, _VENC_MAX_RES); //for changing res in runtime
                u4TmpAddr = _ALIGN_MASK(prFbmPool->u4Addr, 31);
                for (u4Idx = 2; u4Idx < 8; u4Idx++)
                {
                    rFrm.u4YAddr  = u4TmpAddr;
                    u4TmpAddr    += u4Size;
                    u4TmpAddr     = _ALIGN_MASK(u4TmpAddr, 31);
                    rFrm.u4UVAddr = u4TmpAddr;
                    u4TmpAddr    += (u4Size >> 1);
                    u4TmpAddr     = _ALIGN_MASK(u4TmpAddr, 31);
                    _BUFMGR_OBJ_Add(prVEncMwInfo->ptFrmBuffMgr, &rFrm);
                }

                buff.u4Addr = _ALIGN_MASK(u4TmpAddr, 31);
                buff.u4Size = prFbmPool->u4Size - (u4TmpAddr - prFbmPool->u4Addr);
                
                if (VENC_OK != VENC_SetCtrl(prVEncMwInfo->hVEnc, VENC_CTRL_BUFF, (VOID*)&buff))
                {
                    LOG(1, "%s: Set ctrl buffer failed!\n", __FUNCTION__);
                    return RMR_DRV_SET_FAILED;
                }
                }
                
                //##reg callback function
                {
                UINT32 u4Val;
                VENC_CTRL_OUTCB_T rCb;
                
                //--SYNC MODE
                u4Val = (UINT32)VENC_SMODE_ASYNC;
                if (VENC_OK != VENC_SetCtrl(prVEncMwInfo->hVEnc, VENC_CTRL_SYNC_MODE, (VOID*)&u4Val))
                {
                    LOG(1, "%s: Set ctrl sync mode failed!\n", __FUNCTION__);
                    return RMR_DRV_SET_FAILED;
                }
                
                //--Output Callback
                rCb.pvData = (VOID *)prVEncMwInfo;
                rCb.cb     = _VEncMwCb;
                
                if (VENC_OK != VENC_SetCtrl(prVEncMwInfo->hVEnc, VENC_CTRL_OUT_CB, (VOID*)&rCb))
                {
                    LOG(1, "%s: Set ctrl cb failed!\n", __FUNCTION__);
                    return RMR_DRV_SET_FAILED;
                }
                }

            }

            //set venc parameters
            //##30 frame per sec.
            u4Value = prVEncMwInfo->u4Fps;  
            if (VENC_OK != VENC_SetParam(prVEncMwInfo->hVEnc, VENC_PRAM_FRAMERATE, (VOID*)&u4Value))
            {
                LOG(1, "%s: Set framerate failed!\n", __FUNCTION__);
                return RMR_DRV_SET_FAILED;
            }
            
            //##bitrate.
            u4Value = prVEncMwInfo->u4Bps;  
            if (VENC_OK != VENC_SetParam(prVEncMwInfo->hVEnc, VENC_PRAM_BITRATE, (VOID*)&u4Value))
            {
                LOG(1, "%s: Set bitrate failed!\n", __FUNCTION__);
                return RMR_DRV_SET_FAILED;
            }
            
            //##resolution
            res.u2Width  = (UINT16)prVEncMwInfo->u4Width;
            res.u2Height = (UINT16)prVEncMwInfo->u4Height;
            if (VENC_OK != VENC_SetParam(prVEncMwInfo->hVEnc, VENC_PRAM_RESOLUTION, (VOID*)&res))
            {
                LOG(1, "%s: Set resolution failed!\n", __FUNCTION__);
                return RMR_DRV_SET_FAILED;
            }
           
            //##pix format
            pix.ePixFmt  = prVEncMwInfo->ePixFmt;
            pix.u2Pitch  = res.u2Width;
            if (VENC_OK != VENC_SetCtrl(prVEncMwInfo->hVEnc, VENC_CTRL_PIX_FMT, (VOID*)&pix))
            {
               LOG(1, "%s: Set pixfmt failed!\n", __FUNCTION__);
               return RMR_DRV_SET_FAILED;
            }

            //init
            if (VENC_OK != VENC_Init(prVEncMwInfo->hVEnc))
            {
                LOG(1, "%s: venc init failed!\n", __FUNCTION__);
                return RMR_DRV_SET_FAILED;
            }

            //run
            if (VENC_OK != VENC_Run(prVEncMwInfo->hVEnc))
            {
                LOG(1, "%s: venc run failed!\n", __FUNCTION__);
                return RMR_DRV_SET_FAILED;
            }

            //set input source parameters
            if (prVEncMwInfo->SetInputParam && (RMR_OK != prVEncMwInfo->SetInputParam(prVEncMwInfo)))
            {
                LOG(1, "%s: Set input source parameters failed!\n", __FUNCTION__);
                return RMR_DRV_SET_FAILED;
            }

            //##connect input source
            if (prVEncMwInfo->ConnectInput && (RMR_OK != prVEncMwInfo->ConnectInput(prVEncMwInfo)))
            {
                LOG(1, "%s: Connect to input source failed!\n", __FUNCTION__);
                return RMR_DRV_SET_FAILED;
            }
            
        }
        else
        {
            //disconnect  input source
            if (prVEncMwInfo->DisconnectInput && (RMR_OK != prVEncMwInfo->DisconnectInput(prVEncMwInfo)))
            {
                LOG(1, "%s: disconnect input source failed!\n", __FUNCTION__);
                return RMR_DRV_SET_FAILED;
            }
        
            //stop
            if (VENC_OK != VENC_Stop(prVEncMwInfo->hVEnc))
            {
                LOG(1, "%s: venc stop failed!\n", __FUNCTION__);
                return RMR_DRV_SET_FAILED;
            }
        }
        prVEncMwInfo->fgIsRunning = fgCtrl;
    }

    return RMR_OK;
}


static INT32 _VEncMwSetSettings(VENC_MW_INFO_T *prVEncMwInfo, const VOID* pv_set_info, SIZE_T z_set_info_len)
{
    INT32 i4Ret = RMR_DRV_SET_FAILED;
    VID_ENC_SETTINGS_T *prSettings;

    M_SET_VALIDATE(VID_ENC_SETTINGS_T);

    if (NULL == prVEncMwInfo)
    {
        LOG(1, "%s: prVEncMwInfo is NULL\n", __FUNCTION__);
        return RMR_DRV_SET_FAILED;
    }

    if (NULL == pv_set_info)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    if (prVEncMwInfo->fgIsRunning)
    {
        LOG(1, "%s: venc is running\n", __FUNCTION__);
        return RMR_DRV_SET_FAILED;
    }

    prSettings = (VID_ENC_SETTINGS_T *)pv_set_info;

    i4Ret = _VEncMwSetEncFmt(prVEncMwInfo, prSettings->e_enc_fmt);
    if (i4Ret != RMR_OK)
    {
        LOG(1, "%s: error enc type [%d]\n", __FUNCTION__, prSettings->e_enc_fmt);
        return i4Ret;
    }

    i4Ret = _VEncMwSetSrcType(prVEncMwInfo, prSettings->e_src_type);
    if (i4Ret != RMR_OK)
    {
        LOG(1, "%s: error src type [%d]\n", __FUNCTION__, prSettings->e_src_type);
        return i4Ret;
    }

    i4Ret = _VEncMwSetFmtInfo(prVEncMwInfo, (VOID *)&prSettings->u_fmt);
    if (i4Ret != RMR_OK)
    {
        LOG(1, "%s: error fmt info\n", __FUNCTION__);
        return i4Ret;
    }

    return RMR_OK;
}


static INT32 _VEncMwSetFrameResolution(VENC_MW_INFO_T *prVEncMwInfo, const VOID* pv_set_info, SIZE_T z_set_info_len)
{
    VID_FRAME_RESOLUTION_T *pt_resolution;
    UINT32 u4Width, u4Height;
    M_SET_VALIDATE(VID_FRAME_RESOLUTION_T);

    if (NULL == prVEncMwInfo)
    {
        LOG(1, "%s: prVEncMwInfo is NULL\n", __FUNCTION__);
        return RMR_DRV_SET_FAILED;
    }

    if (NULL == pv_set_info)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    pt_resolution = (VID_FRAME_RESOLUTION_T *)pv_set_info;

    u4Width  = pt_resolution->ui4_width;
    u4Height = pt_resolution->ui4_height;
    prVEncMwInfo->u4Width  = u4Width;
    prVEncMwInfo->u4Height = u4Height;

    if (prVEncMwInfo->fgIsRunning)
    {
        VENC_RT_PARM_T rRTParm;
        //RESOLUTION
        rRTParm.u4Flag   = VENC_RT_PARM_FLAG_RES;
        rRTParm.u2Width  = u4Width;
        rRTParm.u2Height = u4Height;
        rRTParm.u2Pitch  = u4Width;

        if (VENC_OK != VENC_SetRtParamAsync(prVEncMwInfo->hVEnc, &rRTParm))
        {
            return RMR_DRV_SET_FAILED;
        }
    }

    return RMR_OK;
}


static INT32 _VEncMwSetFramerate(VENC_MW_INFO_T *prVEncMwInfo, const VOID* pv_set_info, SIZE_T z_set_info_len)
{
    UINT32 u4Fps;
    M_SET_VALIDATE(VID_FRAME_RESOLUTION_T);

    if (NULL == prVEncMwInfo)
    {
        LOG(1, "%s: prVEncMwInfo is NULL\n", __FUNCTION__);
        return RMR_DRV_SET_FAILED;
    }

    if (NULL == pv_set_info) // fps could not be 0
    {
        return RMR_DRV_INV_SET_INFO;
    }

    u4Fps = (UINT32)pv_set_info;
    prVEncMwInfo->u4Fps = u4Fps;

    if (prVEncMwInfo->fgIsRunning)
    {
        VENC_RT_PARM_T rRTParm;
        //RESOLUTION
        rRTParm.u4Flag      = VENC_RT_PARM_FLAG_FRAMERATE;
        rRTParm.u1Framerate = (UINT8)u4Fps;

        if (VENC_OK != VENC_SetRtParamAsync(prVEncMwInfo->hVEnc, &rRTParm))
        {
            return RMR_DRV_SET_FAILED;
        }
    }

    return RMR_OK;
}


static INT32 _VEncMwSetBitrate(VENC_MW_INFO_T *prVEncMwInfo, const VOID* pv_set_info, SIZE_T z_set_info_len)
{
    UINT32 u4Bps;
    M_SET_VALIDATE(VID_FRAME_RESOLUTION_T);
    if (NULL == prVEncMwInfo)
    {
        LOG(1, "%s: prVEncMwInfo is NULL\n", __FUNCTION__);
        return RMR_DRV_SET_FAILED;
    }

    if (NULL == pv_set_info) // bitrate could not be 0
    {
        return RMR_DRV_INV_SET_INFO;
    }

    u4Bps = (UINT32)pv_set_info;
    prVEncMwInfo->u4Bps = u4Bps;

    if (prVEncMwInfo->fgIsRunning)
    {
        VENC_RT_PARM_T rRTParm;
        rRTParm.u4Flag      = VENC_RT_PARM_FLAG_BITRATE;
        rRTParm.u4Bitrate   = u4Bps;

        if (VENC_OK != VENC_SetRtParamAsync(prVEncMwInfo->hVEnc, &rRTParm))
        {
            return RMR_DRV_SET_FAILED;
        }
    }

    return RMR_OK;
}


/* component functions */
static INT32 _VEncMwConnect(
                            DRV_COMP_ID_T*   pt_comp_id,
                            DRV_CONN_TYPE_T  e_conn_type,
                            const VOID*      pv_conn_info,
                            SIZE_T           z_conn_info_len,
                            VOID*            pv_tag,
                            x_rm_nfy_fct     pf_nfy)
{
    DRV_COMP_ID_T* prCompReg;
    VENC_MW_INFO_T *prVEncMwInfo;
    BOOL fgNone = TRUE;

    UNUSED(z_conn_info_len);
    UNUSED(pv_tag);
    UNUSED(pf_nfy);

    // Check component type and id
    if (NULL == pt_comp_id)
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    if (DRVT_VID_ENC != pt_comp_id->e_type)
    {
       return RMR_DRV_INV_CONN_INFO;
    }

    if (pt_comp_id->ui2_id >= (UINT16)VENC_COMP_NS)
    {
       return RMR_DRV_INV_CONN_INFO;
    }

    if (pt_comp_id->b_sel_out_port)
    {
       return RMR_DRV_INV_CONN_INFO;
    }

    if (pt_comp_id->u.ui1_inp_port > 1)
    {
       return RMR_DRV_INV_CONN_INFO;
    }

    if (e_conn_type != RM_CONN_TYPE_COMP_ID)
    {
       return RMR_DRV_INV_CONN_INFO;
    }

    if (NULL == pv_conn_info)
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    if (RM_CONN_TYPE_COMP_ID != e_conn_type)
    {
        LOG(1, "%s: unexpected conn type [%d]\n", __FUNCTION__, e_conn_type);
        return RMR_DRV_INV_CONN_INFO;
    }

    prCompReg = (DRV_COMP_ID_T*)pv_conn_info;
    prVEncMwInfo = _VEncMwGetInfoInst(pt_comp_id->ui2_id);

    if (NULL == prVEncMwInfo)
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    if (VENC_NULL_HANDLE != prVEncMwInfo->hVEnc)
    {
        LOG(1, "%s, [id]%d has already been connected\n", __FUNCTION__, pt_comp_id->ui2_id);
        return RMR_DRV_INV_CONN_INFO;
    }

    if (fgNone
#ifdef CC_ATV_PVR_SUPPORT
    && (prCompReg->e_type != DRVT_TV_DEC)
#endif
#ifdef VDEC_VENC_PATH
    && (prCompReg->e_type != DRVT_VID_DEC)
#endif
    )
    {
        LOG(1, "%s: unexpected comp type [%d]\n", __FUNCTION__, prCompReg->e_type);
        return RMR_DRV_INV_CONN_INFO;
    }

    //init venc MW Info
    x_memset((VOID *)prVEncMwInfo, 0, sizeof(VENC_MW_INFO_T));
    prVEncMwInfo->hVEnc = VENC_NULL_HANDLE;

    prVEncMwInfo->eDrvType = prCompReg->e_type;
    prVEncMwInfo->u2CompId = prCompReg->ui2_id;
        
    return RMR_OK;
}


static INT32 _VEncMwDisconnect(
                            DRV_COMP_ID_T*   pt_comp_id,
                            DRV_DISC_TYPE_T  e_disc_type,
                            const VOID*      pv_disc_info,
                            SIZE_T           z_disc_info_len)
{
    DRV_COMP_ID_T* prCompReg;
    VENC_MW_INFO_T *prVEncMwInfo;
    BOOL fgNone = TRUE;
  
    // Check component type and id
    if (pt_comp_id == NULL)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if (pt_comp_id->e_type != DRVT_VID_ENC)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if (pt_comp_id->ui2_id >= (UINT16)VENC_COMP_NS)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    if (pt_comp_id->b_sel_out_port)
    {
       return RMR_DRV_INV_DISC_INFO;
    }

    if (pt_comp_id->u.ui1_inp_port > 1)
    {
       return RMR_DRV_INV_DISC_INFO;
    }

    if (e_disc_type != RM_CONN_TYPE_COMP_ID)
    {
       return RMR_DRV_INV_DISC_INFO;
    }

    if (pv_disc_info == NULL)
    {
        return RMR_DRV_INV_DISC_INFO;
    }

    prCompReg = (DRV_COMP_ID_T*)pv_disc_info;
    prVEncMwInfo = _VEncMwGetInfoInst(pt_comp_id->ui2_id);

    if (fgNone
#ifdef CC_ATV_PVR_SUPPORT
    && (prCompReg->e_type != DRVT_TV_DEC)
#endif
#ifdef VDEC_VENC_PATH
    && (prCompReg->e_type != DRVT_VID_DEC)
#endif
    )
    {
        LOG(1, "%s: unexpected comp type [%d]\n", __FUNCTION__, prCompReg->e_type);
        return RMR_DRV_INV_DISC_INFO;
    }

    //deinit venc mw info
    if (prVEncMwInfo->hVEnc)
    {
        //STOP VENC
        if (prVEncMwInfo->fgIsRunning && VENC_OK != VENC_Stop(prVEncMwInfo->hVEnc))
        {
            LOG(1, "%s: venc stop fail\n", __FUNCTION__);
            return RMR_DRV_DISC_FAILED;
        }
            
        //CLOSE VENC
        VENC_BUFMGR_OBJ_DELETE(VENC_FRM, (VENC_BUFMGR_T(VENC_FRM) *)(prVEncMwInfo->ptFrmBuffMgr)); 
        if (VENC_OK != VENC_Close(prVEncMwInfo->hVEnc))
        {
            LOG(1, "%s: venc close fail\n", __FUNCTION__);
            return RMR_DRV_DISC_FAILED;
        }
    }
    
    prVEncMwInfo->hVEnc = VENC_NULL_HANDLE;

    UNUSED(z_disc_info_len);

    return RMR_OK;
}


static INT32 _VEncMwGet(
                        DRV_COMP_ID_T*        pt_comp_id,
                        DRV_GET_TYPE_T        e_get_type,
                        VOID*                pv_get_info,
                        SIZE_T*                pz_get_info_len)
{
    INT32 i4RetCode = RMR_DRV_INV_GET_INFO;
    VENC_MW_INFO_T *prVEncMwInfo;

    // Check component type and id
    if (NULL == pt_comp_id)
    {
        return (RMR_DRV_INV_GET_INFO);
    }

    if (DRVT_VID_ENC != pt_comp_id->e_type)
    {
        LOG(1, "%s: error component type or id type [%d]\n", 
            __FUNCTION__, pt_comp_id->e_type);
        return (RMR_DRV_INV_GET_INFO);
    }

    if (pt_comp_id->ui2_id >= (UINT16)VENC_COMP_NS)
    {
        LOG(1, "%s: error component id [%d]\n", 
            __FUNCTION__, pt_comp_id->ui2_id);
        return (RMR_DRV_INV_GET_INFO);
    }

    if (NULL == pv_get_info)
    {
        LOG(1, "%s: pv_get_info is NULL\n", __FUNCTION__);
        return RMR_DRV_INV_GET_INFO;
    }

    if (NULL == pz_get_info_len)
    {
        LOG(1, "%s: pz_get_info_len is NULL\n", __FUNCTION__);
        return RMR_DRV_INV_GET_INFO;
    }

    prVEncMwInfo = _VEncMwGetInfoInst(pt_comp_id->ui2_id);
    
    if (NULL == prVEncMwInfo)
    {
        return (RMR_DRV_INV_SET_INFO);
    }


    switch (e_get_type)
    {
    default:
        LOG(1, "%s: Invalid get_type - %d\n", __FUNCTION__, e_get_type);
        return (RMR_DRV_INV_GET_INFO);
    case VID_ENC_GET_TYPE_CTRL:
        i4RetCode = _VEncMwGetCtrl(prVEncMwInfo, pv_get_info, pz_get_info_len);
        break;
    }

    return i4RetCode;
}


static INT32 _VEncMwSet(
                    DRV_COMP_ID_T*        pt_comp_id,
                    DRV_SET_TYPE_T         e_set_type,
                    const VOID*            pv_set_info,
                    SIZE_T                z_set_info_len)
{
    INT32 i4RetCode = RMR_DRV_INV_SET_INFO;
    VENC_MW_INFO_T *prVEncMwInfo;

    // Check component type and id
    if (NULL == pt_comp_id)
    {
        return (RMR_DRV_INV_SET_INFO);
    }

    if (DRVT_VID_ENC != pt_comp_id->e_type)
    {
        LOG(1, "%s: error component type or id type [%d]\n", 
            __FUNCTION__, pt_comp_id->e_type);
        return (RMR_DRV_INV_SET_INFO);
    }

    if (pt_comp_id->ui2_id >= (UINT16)VENC_COMP_NS)
    {
        LOG(1, "%s: error component id [%d]\n", 
            __FUNCTION__, pt_comp_id->ui2_id);
        return (RMR_DRV_INV_SET_INFO);
    }

    prVEncMwInfo = _VEncMwGetInfoInst(pt_comp_id->ui2_id);
    
    if (NULL == prVEncMwInfo)
    {
        return (RMR_DRV_INV_SET_INFO);
    }

    switch (e_set_type)
    {
    default:
        LOG(1, "%s: Invalid set_type - %d\n", __FUNCTION__, e_set_type);
        return (RMR_DRV_INV_SET_INFO);
    case VID_ENC_SET_TYPE_CTRL:
        i4RetCode = _VEncMwSetCtrl(prVEncMwInfo, pv_set_info, z_set_info_len);
        break;

    case VID_ENC_SET_TYPE_SETTINGS:
        i4RetCode = _VEncMwSetSettings(prVEncMwInfo, pv_set_info, z_set_info_len);
        break;

    //maybe used in runtime 
    case VID_ENC_SET_TYPE_FRAME_RESOLUTION:
        i4RetCode = _VEncMwSetFrameResolution(prVEncMwInfo, pv_set_info, z_set_info_len);
        break;
    case VID_ENC_SET_TYPE_FRAME_RATE:
        i4RetCode = _VEncMwSetFramerate(prVEncMwInfo, pv_set_info, z_set_info_len);
        break;
    case VID_ENC_SET_TYPE_BITRATE:
        i4RetCode = _VEncMwSetBitrate(prVEncMwInfo, pv_set_info, z_set_info_len);
        break;
    }

    return i4RetCode;
}


//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------

INT32 MW_VENC_Init(void)
{
    INT32 i4Return;
    DRV_COMP_REG_T tVEncCompReg;
    DRV_COMP_FCT_TBL_T tVEncFctTbl;
    UINT32 u4Flags = 0;
    UINT32 i;

    /* Init VENC MW Info */
    for (i = 0; i < VENC_COMP_NS; i++)
    {
        x_memset((VOID *)&_arVEncMwInfo[i], 0, sizeof(VENC_MW_INFO_T));
        _arVEncMwInfo[i].hVEnc = VENC_NULL_HANDLE;
    }

    /* register venc component */
    tVEncCompReg.e_type           = DRVT_VID_ENC;
    tVEncCompReg.e_id_type        = ID_TYPE_IND;
    tVEncCompReg.u.t_ind.ui2_id   = (UINT16)VENC_COMP_1;
    tVEncCompReg.u.t_ind.ui1_port = ALL_PORTS;
    tVEncCompReg.u.t_ind.pv_tag   = NULL;

    /* Setup component function table */
    tVEncFctTbl.pf_rm_connect     = _VEncMwConnect;
    tVEncFctTbl.pf_rm_disconnect  = _VEncMwDisconnect;
    tVEncFctTbl.pf_rm_get         = _VEncMwGet;
    tVEncFctTbl.pf_rm_set         = _VEncMwSet;

    /* Register component with the Resource Manager database */
    i4Return = x_rm_reg_comp(&tVEncCompReg,
                            1,
                            1,
                            NULL,
                            u4Flags,
                            &tVEncFctTbl,
                            NULL,
                            0);

    if (i4Return != RMR_OK)
    {
        LOG(1, "%s call x_rm_reg_comp NG [%d]\n", __FUNCTION__, i4Return);
        return i4Return;
    }

    return RMR_OK;
}


