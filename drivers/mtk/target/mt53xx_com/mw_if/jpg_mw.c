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
/******************************************************************************
* Include files
******************************************************************************/
#define	DEFINE_IS_LOG	MPV_IsLog
#include "x_debug.h"
#include "x_assert.h"
#include "x_hal_926.h"

#include "x_handle.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_img_dec.h"
#include "x_timer.h"
#include "x_cli.h"

#include "drv_comp_id.h"
#include "jpg_if.h"
#include "fbm_drvif.h"
#include "img_lib_if.h"

#ifdef LINUX_TURNKEY_SOLUTION
#include "cb_data_rm.h"
#include "cb_param.h"
extern unsigned int fb_convert_app_to_phy(unsigned int u);
extern unsigned int addr_user_to_kernel(unsigned int addr);
extern unsigned int adapt_fbm_knl_to_usr(unsigned int addr);
extern INT32 _CB_PutEvent_Adpt(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);
#endif
/******************************************************************************
* Local	Macros,	defines, typedefs, enums
******************************************************************************/

/******************************************************************************
* Function prototype
******************************************************************************/
static INT32 _JpgGet(DRV_COMP_ID_T*	 pt_comp_id,
                     DRV_GET_TYPE_T	e_get_type,
                     VOID*			pv_get_info,
                     SIZE_T*			pz_get_info_len);

static INT32 _JpgSet(DRV_COMP_ID_T*	 pt_comp_id,
                     DRV_SET_TYPE_T	e_set_type,
                     const VOID*		pv_set_info,
                     SIZE_T			z_set_info_len);

/* get types */
static INT32 _IMGGetProg(VOID* pv_get_info);
static INT32 _IMGGetCap(VOID* pv_get_info);

/* set types */
/* 538x new set types */
static INT32 _IMGSetInit(IMG_FRM_START_T *prInitInfo);
static INT32 _IMGSetDec(IMG_DECODE_T *prDec);
static INT32 _IMGSetStop(void);
static INT32 _IMGSetEnd(void);
static INT32 _IMGSetBufFilled(IMG_BUF_FILLED_T* prBufInfo);
static INT32 _IMGSetTypeConfig(ENUM_IMG_TYPE img_type);

/******************************************************************************
* Local	variables
******************************************************************************/
static DRV_COMP_FCT_TBL_T _rJpgFctTbl =
{
    NULL,
    NULL,
    _JpgGet,
    _JpgSet
};

static DRV_COMP_REG_T _rJpgComp;

#define MAX_MWDRV_JPG_BUF 2

/* performance measurement */
/*
static HAL_TIME_T _rJpgPicStart;
static HAL_TIME_T _rJpgPicEnd;
static HAL_TIME_T _rJpgPicTime;

static HAL_TIME_T _rJpgRowStart;
static HAL_TIME_T _rJpgRowEnd;
static HAL_TIME_T _rJpgHwAcc;
*/

#ifdef LINUX_TURNKEY_SOLUTION
JPG_IMG_SET_NFY_PARM gt_jpg_set_nfy_parm;
#endif

/******************************************************************************
* Global Function definition
******************************************************************************/
INT32 MW_JpgInit(void)
{
    INT32 i4Ret;

    /* Init	MPV	*/
    MPV_Init();
    if (IMG_Lib_Init()!= E_IMG_OK)
    {
        return RMR_DRV_SET_FAILED;
    }

    /* Register	component with the Resource	Manager	database */
    _rJpgComp.e_type = DRVT_JPG_DEC;
    _rJpgComp.e_id_type = ID_TYPE_IND;
    _rJpgComp.u.t_ind.ui2_id = (UINT16)JPG_COMP_1;
    _rJpgComp.u.t_ind.pv_tag = (VOID*)NULL;
    _rJpgComp.u.t_ind.ui1_port = ALL_PORTS;

    i4Ret =	x_rm_reg_comp(&_rJpgComp,
                          1,
                          1,
                          "JPG",
#if (defined(CC_MT5368) || defined(CC_MT5396))
                          DRV_FLAG_SINGLE_CONN_ON_OUTPUT|JPG_COMP_FLAG_JPG_SUPPORT|JPG_COMP_FLAG_PNG_SUPPORT|JPG_COMP_FLAG_WEBP_SUPPORT,
#elif defined(CC_MT5389)
                          DRV_FLAG_SINGLE_CONN_ON_OUTPUT|JPG_COMP_FLAG_JPG_SUPPORT|JPG_COMP_FLAG_WEBP_SUPPORT,
#else
                          DRV_FLAG_SINGLE_CONN_ON_OUTPUT|JPG_COMP_FLAG_JPG_SUPPORT,
#endif
                          &_rJpgFctTbl,
                          NULL,
                          0);
    if (i4Ret != RMR_OK)
    {
        LOG(1, "MW_JpgInit(): x_rm_reg_comp	failed [%d]\n",	(UINT32)i4Ret);
        return i4Ret;
    }

    return (RMR_OK);
}


/******************************************************************************
* Local	Function definition
******************************************************************************/
/*
static void _JpgPictureStart(void)
{
	HAL_GetTime(&_rJpgPicStart);
	_rJpgHwAcc.u4Seconds = 0;
	_rJpgHwAcc.u4Micros  = 0;
}

static void _JpgPictureEnd(void)
{
	HAL_GetTime(&_rJpgPicEnd);
	HAL_GetDeltaTime(&_rJpgPicTime, &_rJpgPicStart, &_rJpgPicEnd);
}

static void _JpgRowStart(void)
{
	HAL_GetTime(&_rJpgRowStart);
}

static void _JpgRowEnd(void)
{
	HAL_TIME_T rRowTime;
	HAL_GetTime(&_rJpgRowEnd);
	HAL_GetDeltaTime(&rRowTime, &_rJpgRowStart, &_rJpgRowEnd);
	_rJpgHwAcc.u4Seconds += rRowTime.u4Seconds;
	_rJpgHwAcc.u4Micros  += rRowTime.u4Micros;
	if (_rJpgHwAcc.u4Micros >= 1000000)
	{
		_rJpgHwAcc.u4Seconds += _rJpgHwAcc.u4Micros / 1000000;
		_rJpgHwAcc.u4Micros  %= 1000000;
	}
}

extern VOID os_mem_cli_stats(HANDLE_T h_part_hdl,
				SIZE_T* pz_totalfree,
				SIZE_T* pz_bigfree,
				SIZE_T* pz_freeobj,
				SIZE_T* pz_allocatedobj,
				SIZE_T* pz_totalsize,
				CHAR  * ps_name);

static void _JpgPicShowTime(void)
{

	SIZE_T z_freeobj;
	SIZE_T z_allocatedobj;
	SIZE_T z_totalsize;
	CHAR s_name[20];


	os_mem_cli_stats(NULL_HANDLE,
			&z_totalfree,
			&z_bigfree,
			&z_freeobj,
			&z_allocatedobj,
			&z_totalsize,
			s_name);

	Printf("%3u.%06u / %3u.%06u / %u x %u pix(%u) / mem(%d)\n",
		_rJpgPicTime.u4Seconds, _rJpgPicTime.u4Micros,
		_rJpgHwAcc.u4Seconds, _rJpgHwAcc.u4Micros,
		_u4JpgWidth, _u4JpgHeight, _u4JpgWidth * _u4JpgHeight,
		z_totalfree
		);

}*/

static INT32 _JpgGet(
    DRV_COMP_ID_T*		pt_comp_id,
    DRV_GET_TYPE_T		e_get_type,
    VOID*				pv_get_info,
    SIZE_T*				pz_get_info_len)
{
    INT32 i4Ret	= RMR_DRV_INV_GET_INFO;
    // Check component type	and	id
    if ((pt_comp_id	== NULL)					||
            (pt_comp_id->e_type	!= DRVT_JPG_DEC)	||
            (pt_comp_id->ui2_id	>= JPG_COMP_NS))
    {
        return (RMR_DRV_INV_GET_INFO);
    }

    switch (e_get_type)
    {
    case IMG_GET_TYPE_PROGRESS:
        i4Ret = _IMGGetProg(pv_get_info);
        break;
    case IMG_GET_TYPE_LIMITATION:
        i4Ret = _IMGGetCap(pv_get_info);
        break;
    default:
        LOG(1, "_JpgGet(): Invalid get_type	: %d\n", e_get_type);
        return (RMR_DRV_INV_GET_INFO);
    }

    return i4Ret;
}



static INT32 _JpgSet(
    DRV_COMP_ID_T*		pt_comp_id,
    DRV_SET_TYPE_T		e_set_type,
    const VOID*			pv_set_info,
    SIZE_T				z_set_info_len)
{
    INT32 i4Ret	= RMR_OK;

    // Check component type and id
    if ((pt_comp_id == NULL)					||
            (pt_comp_id->e_type != DRVT_JPG_DEC)	||
            (pt_comp_id->ui2_id >= JPG_COMP_NS))
    {
        return (RMR_DRV_INV_SET_INFO);
    }

    switch (e_set_type)
    {
        //538x new interface

    case IMG_SET_TYPE_CONFIG:
        i4Ret = _IMGSetTypeConfig(*(ENUM_IMG_TYPE *)pv_set_info);
        break;

    case IMG_SET_TYPE_FRM_START:
#ifndef LINUX_TURNKEY_SOLUTION
        i4Ret = _IMGSetInit((IMG_FRM_START_T*)pv_set_info);
#else
        {
            IMG_FRM_START_T t_set_info;
            x_memcpy(&t_set_info, pv_set_info, sizeof(IMG_FRM_START_T));
            //DBG("+++++++++ pv_img_buf is %x, pv_aux_cache is %x\n", ((IMG_FRM_START_T*)pv_set_info)->pv_img_buf, ((IMG_FRM_START_T*)pv_set_info)->pv_aux_cache);
            t_set_info.pv_img_buf = (VOID *)addr_user_to_kernel((unsigned int)(((IMG_FRM_START_T*)pv_set_info)->pv_img_buf));
            t_set_info.pv_aux_cache = (VOID *)addr_user_to_kernel((unsigned int)(((IMG_FRM_START_T*)pv_set_info)->pv_aux_cache));
            i4Ret = _IMGSetInit(&t_set_info);
        }
#endif
        break;
    case IMG_SET_TYPE_DECODE:
        i4Ret = _IMGSetDec((IMG_DECODE_T*)pv_set_info);
        break;
    case IMG_SET_TYPE_STOP:
        i4Ret = _IMGSetStop();
        break;
    case IMG_SET_TYPE_BUF_FILLED:
        i4Ret = _IMGSetBufFilled((IMG_BUF_FILLED_T*)pv_set_info);
        break;
    case IMG_SET_TYPE_FRM_END:
        i4Ret = _IMGSetEnd();
        break;
    default:
        LOG(1, "_JpgSet(): Invalid set_type : %d\n", e_set_type);
        return (RMR_DRV_INV_SET_INFO);
    }


    return i4Ret;
}

#ifdef LINUX_TURNKEY_SOLUTION
static VOID _img_set_nfy_fct(
    UINT32          ui4_img_id,         /* the image id which causes this notification */
    VOID            *pv_tag,            /* tag passed to the callback function */
    VOID            *pv_data,           /* data passed with this notification */
    IMG_NFY_STATE_T e_state             /* notification state */
)
{
    /*
    DBG("==================kernel:e_state=%d-\n",e_state);
    */

    if (JPG_HW_NFY_FILL_BUF == e_state)
    {
        IMG_FILL_BUF_T rFillBuf;

        rFillBuf = *(IMG_FILL_BUF_T *)pv_data;

        /*
        DBG("******kernel: rFillBuf.pv_start_add=0x%x\n", rFillBuf.pv_start_add);

        DBG("[][ %x %x %x %x ] []\n",
               ((char *)((IMG_FILL_BUF_MW_T *)pv_data)->pv_start_add)[0],
               ((char *)((IMG_FILL_BUF_MW_T *)pv_data)->pv_start_add)[1],
               ((char *)((IMG_FILL_BUF_MW_T *)pv_data)->pv_start_add)[2],
               ((char *)((IMG_FILL_BUF_MW_T *)pv_data)->pv_start_add)[3]);
        */

        rFillBuf.pv_start_add = (VOID*)adapt_fbm_knl_to_usr((unsigned int)(rFillBuf.pv_start_add));
#if 0 /* Fix the overlap data bug. */
        lock_nfy_param_buf(PARAM_JPG_STREAM_BUF_NFY);

        gt_jpg_set_nfy_parm.ui4_img_id = ui4_img_id;
        gt_jpg_set_nfy_parm.pv_tag = pv_tag;
        gt_jpg_set_nfy_parm.pv_data = (void *)get_nfy_param_buf(PARAM_JPG_STREAM_BUF_NFY,sizeof(IMG_FILL_BUF_MW_T));
        gt_jpg_set_nfy_parm.e_state = e_state;

        rm_copy_to_user(gt_jpg_set_nfy_parm.pv_data, &rFillBuf, sizeof(IMG_FILL_BUF_MW_T));
#else
        gt_jpg_set_nfy_parm.ui4_img_id = ui4_img_id;
        gt_jpg_set_nfy_parm.pv_tag = pv_tag;
        gt_jpg_set_nfy_parm.t_attach_buf = rFillBuf;
        gt_jpg_set_nfy_parm.e_state = e_state;
#endif

        /*
        DBG("******kernel: jpeg gt_jpg_set_nfy_parm.pf_nfy=0x%x-req_len:%d\n",
            gt_jpg_set_nfy_parm.pf_nfy,rFillBuf.ui4_required_len);
        */

        _CB_PutEvent_Adpt(CB_DRV_JPG_SET_NFY, sizeof(JPG_IMG_SET_NFY_PARM), &gt_jpg_set_nfy_parm);
#if 0 /* Fix the overlap data bug. */
        rm_copy_from_user(&rFillBuf, gt_jpg_set_nfy_parm.pv_data, sizeof(IMG_FILL_BUF_MW_T));

        ((IMG_FILL_BUF_MW_T *)pv_data)->pv_start_add =
            adapt_fbm_usr_to_knl(rFillBuf.pv_start_add);
#else
        /*
                rFillBuf = gt_jpg_set_nfy_parm.t_attach_buf;
                ((IMG_FILL_BUF_MW_T *)pv_data)->pv_start_add =
                    adapt_fbm_usr_to_knl(rFillBuf.pv_start_add);
        */
#endif
        /*
        DBG("[][ %x %x %x %x ] []\n",
               ((char *)((IMG_FILL_BUF_MW_T *)pv_data)->pv_start_add)[0],
               ((char *)((IMG_FILL_BUF_MW_T *)pv_data)->pv_start_add)[1],
               ((char *)((IMG_FILL_BUF_MW_T *)pv_data)->pv_start_add)[2],
               ((char *)((IMG_FILL_BUF_MW_T *)pv_data)->pv_start_add)[3]);
        */
#if 0
        ((IMG_FILL_BUF_MW_T *)pv_data)->b_reset_pos = rFillBuf.b_reset_pos;
        ((IMG_FILL_BUF_MW_T *)pv_data)->ui4_position = rFillBuf.ui4_position;
        ((IMG_FILL_BUF_MW_T *)pv_data)->ui4_required_len = rFillBuf.ui4_required_len;
        ((IMG_FILL_BUF_MW_T *)pv_data)->ui4_trsn_id = rFillBuf.ui4_trsn_id;

        unlock_nfy_param_buf(PARAM_JPG_STREAM_BUF_NFY);
#endif
        /*
        DBG("=5=pv_data=0x%x==\n",pv_data);
        */
    }
    else
    {
        gt_jpg_set_nfy_parm.ui4_img_id = ui4_img_id;
        gt_jpg_set_nfy_parm.pv_tag = pv_tag;
        gt_jpg_set_nfy_parm.pv_data = pv_data;
        gt_jpg_set_nfy_parm.e_state = e_state;

        //DBG("\n-- kernel : jpeg _CB_PutEvent_HF-- \n");
        //DBG("\n -- kernel : jpeg gt_jpg_set_nfy_parm.pf_nfy = %lx-- \n", gt_jpg_set_nfy_parm.pf_nfy);
        _CB_PutEvent_Adpt(CB_DRV_JPG_SET_NFY, sizeof(JPG_IMG_SET_NFY_PARM), &gt_jpg_set_nfy_parm);
    }
}
#endif

static INT32 _IMGSetInit(IMG_FRM_START_T *prInitInfo)
{
    INT32 i4Ret;
#ifndef LINUX_TURNKEY_SOLUTION
    i4Ret = (INT32)IMG_Init(RING_IN_SINGLE_OUT, prInitInfo->pv_tag, prInitInfo->ui4_img_id,
                            (PFN_JPG_EVENT_NOTIFY)prInitInfo->pf_func);
#else
    gt_jpg_set_nfy_parm.pf_nfy = prInitInfo->pf_func;

    i4Ret = (INT32)IMG_Init(RING_IN_SINGLE_OUT, prInitInfo->pv_tag, prInitInfo->ui4_img_id,
                            (PFN_JPG_EVENT_NOTIFY)_img_set_nfy_fct);
#endif

    if (IMG_SetUseMw(TRUE)!=E_IMG_OK)
    {
        return RMR_DRV_SET_FAILED;
    }
    IMG_Set_ImgSize(prInitInfo->ui4_img_size);

    if (i4Ret > 0)
    {
        return RMR_DRV_SET_FAILED;
    }
    else
    {
        return RMR_OK;
    }
}

static INT32 _IMGSetDec(IMG_DECODE_T *prDec)
{
    IMG_PARAM_T rInfo;
    IMG_REGION_T rDecOut;

    //src bitstream
    rInfo.pu1InBuffer = NULL; //use internal ring buffer
    rInfo.u8BufSize = 0;

    //dst info
#ifndef LINUX_TURNKEY_SOLUTION
    rInfo.rDstRegion.pu1Addr = (UINT8*)prDec->pv_dst;
#else
    rInfo.rDstRegion.pu1Addr = (UINT8*)fb_convert_app_to_phy((unsigned int)(prDec->pv_dst));
#endif
    rInfo.rDstRegion.u4StartX = prDec->ui4_dst_x;
    rInfo.rDstRegion.u4StartY = prDec->ui4_dst_y;
    rInfo.rDstRegion.u4Width = prDec->ui4_dst_width;
    rInfo.rDstRegion.u4Height = prDec->ui4_dst_height;
    rInfo.rDstRegion.u4Pitch = prDec->ui4_dst_pitch;
	rInfo.rDstRegion.u4CbCrOffset = prDec->ui4_CbCrOffset;
	rInfo.eFormat=IMG_GetImageType();
    switch (rInfo.eFormat)
    {
    case IMAGE_PNG:
        //src info
        rInfo.rSrcRegion.u4SrcStartX = prDec->ui4_src_x;
        rInfo.rSrcRegion.u4SrcStartY = prDec->ui4_src_y;
        rInfo.rSrcRegion.u4SrcWidth = prDec->ui4_src_width;
        rInfo.rSrcRegion.u4SrcHeight = prDec->ui4_src_height;
        if (prDec->e_dst_cm == GFX_COLORMODE_AYCbCr_D8888)
        {
            rInfo.u4DecColorMode = GFX_COLORMODE_ARGB_D8888;
            rInfo.eDispColorMode = AYCBCR8888_DIRECT32;
        }
        else if (prDec->e_dst_cm == GFX_COLORMODE_ARGB_D8888)
        {
            rInfo.u4DecColorMode = GFX_COLORMODE_ARGB_D8888;
            rInfo.eDispColorMode = ARGB8888_DIRECT32;
        }
        else if (prDec->e_dst_cm == GFX_COLORMODE_ARGB_D4444)
        {
            rInfo.u4DecColorMode = GFX_COLORMODE_ARGB_D4444;
            rInfo.eDispColorMode = ARGB4444_DIRECT16;
        }
	else if (prDec->e_dst_cm == GFX_COLORMODE_YUV_422_RS)
	{
		rInfo.u4DecColorMode = GFX_COLORMODE_ARGB_D8888;
		rInfo.eDispColorMode = Y_CBCR422_SEP16;
	}
        else
        {
            return RMR_INV_ARG;
        }
        break;
    case IMAGE_JPEG:
	case IMAGE_WEBP:
        if (prDec->e_dst_cm == GFX_COLORMODE_AYCbCr_D8888)
        {
            rInfo.eDispColorMode = AYCBCR8888_DIRECT32;
        }
        else if (prDec->e_dst_cm == GFX_COLORMODE_ARGB_D8888)
        {
            rInfo.eDispColorMode = ARGB8888_DIRECT32;
        }
        else if (prDec->e_dst_cm == GFX_COLORMODE_ARGB_D4444)
        {
            rInfo.eDispColorMode = ARGB4444_DIRECT16;
        }
		else if (prDec->e_dst_cm == GFX_COLORMODE_YUV_422_RS)
		{
			rInfo.eDispColorMode = Y_CBCR422_SEP16;
		}
        else
        {
            return RMR_INV_ARG;
        }
        break;
    default:
		return RMR_DRV_SET_FAILED;
        break;
    }
    rInfo.eRotateDegree = (ENUM_IMG_ROTATE_PHASE)((UINT32)prDec->e_rotate);
    rInfo.bKeepRatio = FALSE;
    rInfo.prDecOut = &rDecOut;

    IMG_Decode(rInfo);
    return RMR_OK;
}

static INT32 _IMGSetStop()
{
    IMG_Stop();
    return RMR_OK;
}

static INT32 _IMGSetEnd(void)
{
    IMG_Release();
    return RMR_OK;
}

static INT32 _IMGSetBufFilled(IMG_BUF_FILLED_T* prBufInfo)
{
    static UINT32 u4FillLen1 = 0;

    if (prBufInfo->ui4_trsn_id == 1)
    {
        if (u4FillLen1 == 0)
        {
            IMG_SetFillBsBufEnd(prBufInfo->ui4_filled_len, 0,
                                (prBufInfo->i4_ret==IMG_MW_FILL_OK)?FALSE:TRUE);
        }
        else
        {
            IMG_SetFillBsBufEnd(u4FillLen1, prBufInfo->ui4_filled_len,
                                (prBufInfo->i4_ret==IMG_MW_FILL_OK)?FALSE:TRUE);
        }
        u4FillLen1 = 0;
    }
    else
    {
        u4FillLen1 = prBufInfo->ui4_filled_len;
    }

    return RMR_OK;
}

static INT32 _IMGGetProg(VOID* pv_get_info)
{
    UINT32 u4Progress;

    IMG_GetProg(&u4Progress);
    *((UINT32*)pv_get_info) = u4Progress;
    UNUSED(pv_get_info);
    return RMR_OK;
}

static INT32 _IMGGetCap(VOID* pv_get_info)
{
    IMG_GetCap(pv_get_info);

    UNUSED(pv_get_info);
    return RMR_OK;
}

static INT32 _IMGSetTypeConfig(ENUM_IMG_TYPE img_type)
{
    ENUM_IMG_FORMAT imgtype;
	if(img_type==IMAGE_TYPE_JPEG)
	{
       imgtype=IMAGE_JPEG;
	}
	else if(img_type==IMAGE_TYPE_WEBP)
	{
		imgtype=IMAGE_WEBP;
	}
	else if(img_type==IMAGE_TYPE_PNG)
	{
		imgtype=IMAGE_PNG;
	}
	else
	{
        imgtype=IMAGE_MAX;
	}
	
	if(imgtype!=IMAGE_MAX)
	{
		IMG_Config(imgtype);
        return RMR_OK;
	}
	else
	{
        return RMR_INV_ARG;
	}
}


