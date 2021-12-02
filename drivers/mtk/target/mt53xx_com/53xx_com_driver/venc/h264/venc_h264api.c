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
 * $RCSfile: venc_h264api.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file venc_h264api.c
 *  This file contains implementation of exported APIs of H.264.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "venc_drvif.h"
#include "venc_h264api.h"
#include "venc_h264hw.h"
#include "x_mid.h"


#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_assert.h"
#include "x_typedef.h"
#include "x_debug.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
//#define VENC_DISABLE_MID


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define VENC_H264_MAX_COARSE_IMAGE_SIZE VENC_ALIGN_MASK((VENC_H264_MAX_VIDEO_RESOLUTION / 16), (VIDEOCODEC_HAL_DEF_DRAM_PAGE_SIZE - 1))

#define VENC_H264_BUF_ALIGN_MASK (31)
#define VENC_H264_SW_NALU_BUF_SIZE (4 * 1024)
#define VENC_H264_SW_NALU_SIZE_THRD (64)
#define VENC_H264_ROM_CODE_SIZE (4 * 1024 * 8 + 8 * 8)
#define VENC_H264_DUMP_INFO_SIZE (2048)
#define VENC_H264_UPPER_INFO_MB_SIZE ((VENC_H264_MAX_VIDEO_RESOLUTION_WIDTH / 16) * 64)
#define VENC_H264_COL_INFO_SIZE VENC_ALIGN_MASK((((VENC_H264_MAX_VIDEO_RESOLUTION_WIDTH / 16) * (VENC_H264_MAX_VIDEO_RESOLUTION_HEIGHT / 16) * 4) / 8), 63)
#define VENC_H264_CS_INFO_SIZE (((((VENC_H264_MAX_VIDEO_RESOLUTION_WIDTH / 4) / 8) * ((((VENC_H264_MAX_VIDEO_RESOLUTION_HEIGHT / 4) / 8 + 3) >> 2) << 2) + 63 + 64) >> 6) << 9)

#ifdef CC_VENC_DUMP_INFO
#define VENC_H264_WORKING_BUF_SIZE (VENC_H264_UPPER_INFO_MB_SIZE + VENC_H264_COL_INFO_SIZE + VENC_H264_CS_INFO_SIZE + VENC_H264_DUMP_INFO_SIZE)
#else
#define VENC_H264_WORKING_BUF_SIZE (VENC_H264_UPPER_INFO_MB_SIZE + VENC_H264_COL_INFO_SIZE + VENC_H264_CS_INFO_SIZE)
#endif


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define SEI_BUFFERING_PERIOD                     0
#define SEI_PIC_TIMING                           1
#define SEI_PAN_SCAN_RECT                        2
#define SEI_FILLER_PAYLOAD                       3
#define SEI_USER_DATA_REGISTERED_ITU_T_T35       4
#define SEI_USER_DATA_UNREGISTERED               5
#define SEI_RECOVERY_POINT                       6
#define SEI_DEC_REF_PIC_MARKING_REPETITION       7
#define SEI_SPARE_PIC                            8
#define SEI_SCENE_INFO                           9
#define SEI_SUB_SEQ_INFO                         10
#define SEI_SUB_SEQ_LAYER_CHARACTERISTICS        11
#define SEI_SUB_SEQ_CHARACTERISTICS              12
#define SEI_FULL_FRAME_FREEZE                    13
#define SEI_FULL_FRAME_FREEZE_RELEASE            14
#define SEI_FULL_FRAME_SNAPSHOT                  15
#define SEI_PROGRESSIVE_REFINEMENT_SEGMENT_START 16
#define SEI_PROGRESSIVE_REFINEMENT_SEGMENT_END   17
#define SEI_MOTION_CONSTRAINED_SLICE_GROUP_SET   18
#define SEI_FILM_GRAIN_CHARACTERISTICS           19
#define SEI_DEBLOCKING_FILTER_DISPLAY_PREFERENCE 20
#define SEI_STEREO_VIDEO_INFO                    21


#define SEI_FUNC_MARK_CALC_SIZE   0x01
#define SEI_FUNC_MARK_WRIT_DATA   0x02


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

VENC_BUFMGR_DECLARE(h264Rez, UINT32, 10);


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define _H264_NEED_COARSE(_w,_h) (!((_w) < 128 || (_h) < 120))

#define SEI_PUT_BIT(name, val, n) if (mask&SEI_FUNC_MARK_WRIT_DATA){PUT_BIT(val,n);} size+=n;
#define SEI_PUT_BIT_UE(name, val) if (mask&SEI_FUNC_MARK_WRIT_DATA){PUT_BIT_UE(val);} size+=bits_ue_size(val);
#define SEI_PUT_BIT_SE(name, val) if (mask&SEI_FUNC_MARK_WRIT_DATA){PUT_BIT_SE(val);} size+=bits_se_size(val);
#define SEI_ALIGN_BIT()           if (mask&SEI_FUNC_MARK_WRIT_DATA){ALIGN_BIT();}

//#define DECLARE_SEI_FUNC(type) static void _H264SEIPayload##type(VENC_BITS_BUFFER *PBITS_BUFFER,UINT32 *pSize,UINT8 mask)
//#define IMPLEMENT_SEI_FUNC(type) DECLARE_SEI_FUNC(type)

#define PUT_SEI_PAYLOAD(obj,type) _H264SEIPayload##type(obj,PBITS_BUFFER, 0, SEI_FUNC_MARK_WRIT_DATA)
#define GET_SEI_PAYLOAD_SIZE(obj,type,size) _H264SEIPayload##type(obj,0, &size, SEI_FUNC_MARK_CALC_SIZE)

#define PUT_SEI_MESSAGE(obj,type)  \
do \
{ \
    UINT16 payloadType;\
    UINT32 payloadSize;\
    payloadType = SEI_##type;\
    GET_SEI_PAYLOAD_SIZE(obj,type,payloadSize);\
    while (payloadType/0xFF) {PUT_BIT(0xFF,8);payloadType -= 0xFF;}\
    PUT_BIT(payloadType,8);\
    while (payloadSize/0xFF) {PUT_BIT(0xFF,8);payloadSize -= 0xFF;}\
    PUT_BIT(payloadSize,8);\
    PUT_SEI_PAYLOAD(obj,type);\
    PUT_BIT(1,1);ALIGN_BIT();\
}while (0)

#define UP_SW_NALU_WP(ctrl, size)\
(ctrl)->u4NaluBufWp += (size);\
ASSERT((size) < 64);\
if ((ctrl)->u4NaluBufWp + 64 > (ctrl)->u4NaluBufEa) {(ctrl)->u4NaluBufWp = (ctrl)->u4NaluBufSa;}


/*
- If any of the following is true, the value of CpbDpbDelaysPresentFlag shall be set equal to 1.
- nal_hrd_parameters_present_flag is present in the bitstream and is equal to 1
- vcl_hrd_parameters_present_flag is present in the bitstream and is equal to 1
- the need for presence of CPB and DPB output delays to be present in the bitstream in picture timing SEI
messages is determined by the application, by some means not specified in this Recommendation | International
Standard.
- Otherwise, the value of CpbDpbDelaysPresentFlag shall be set equal to 0.
*/
#define CpbDpbDelaysPresentFlag (nal_hrd_parameters_present_flag || vcl_hrd_parameters_present_flag)
#define bit_rate_scale (4)
#define cpb_size_scale (6)
#define cpb_cnt_minus1 (0)
#define Extended_SAR (255)



//just for dev
#define _H264_DBG(fmt...)     LOG(7, fmt)
#define _H264_FUNC_BEGIN()    _H264_DBG("_VENC<H264>::func Begin:%s\n", __FUNCTION__)
#define _H264_FUNC_END(ret)   _H264_DBG("_VEHC<H264>::func end:  %s, ret:%d\n", __FUNCTION__, ret)
#define _H264_FUNC_ENTER()    _H264_DBG("_VENC<H264>::func Enter:%s\n", __FUNCTION__)
#define _H264_FUNC_LEAVE()    _H264_DBG("_VENC<H264>::func leave:%s\n", __FUNCTION__)


#define _H264_MEMBER_OFFSET(type, member)  ((UINT32)(&(((type*)0)->member)))

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
//static UINT8 _h264CosFBSeqRef[] = {0,1,2,0,1,2};
//static UINT8 _h264CosFBSeq1B[]  = {3,4,3,4};
//static UINT8 _h264CosFBSeq2B[]  = {3,4,5,6};

//static UINT8 _h264YVVFBSeqRef[] = {2,3,2,3,2,3};
//static UINT8 _h264YVVFBSeq1B[]  = {4,5,4,5};
//static UINT8 _h264YVVFBSeq2B[]  = {4,5,6,7};


static UINT8 pic_struct_present_flag = 1;
static UINT8 nal_hrd_parameters_present_flag = 1;
static UINT8 vcl_hrd_parameters_present_flag = 0;
static UINT8 cpb_removal_delay_length_minus1 = 7;
static UINT8 dpb_output_delay_length_minus1= 7;

// the following global variable can be local if code size reduction needed
UINT8 aspect_ratio_info_present_flag = 0;
UINT8 overscan_info_present_flag = 0;
UINT8 video_signal_type_present_flag = 0;
UINT8 chroma_loc_info_present_flag = 0;
UINT8 timing_info_present_flag = 1;
UINT8 bitstream_restriction_flag = 0;


//for debug
static VENC_BUFMGR_T(h264Rez) *_ptCsBuffMgrDbg = NULL;

#ifdef VENC_EMU
#define REZ_REC_N 256
UINT32 _u4RzYAddr[REZ_REC_N];
UINT32 _u4RzData[REZ_REC_N];
UINT32 _u4RzIdx;
#endif

UINT32 _u4BsTmp;


//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------
//DECLARE_SEI_FUNC(PIC_TIMING);
static void _H264SEIPayloadPIC_TIMING(VENC_OBJECT_T *this, VENC_BITS_BUFFER *PBITS_BUFFER, UINT32 *pSize, UINT8 mask);
static UINT32 _H264CalcCoarseRes(VENC_OBJECT_T *this);
static UINT32 _H264RezInit(VENC_OBJECT_T *this, H264_CTRL_T* pCtrl);


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static VOID _H264RezSendFrm(VENC_OBJECT_T *this, VENC_FRMQ_DATA_T *prFrm)
{
    INT32 i4Ret;
    SIZE_T zMsgSize = sizeof(VENC_FRMQ_DATA_T);
    H264_CTRL_T *pCtrl = (H264_CTRL_T *)this->hCtrl;

    _H264_FUNC_ENTER();

    i4Ret = x_msg_q_send(pCtrl->hRezFrmQ, prFrm, zMsgSize, VENC_FRMQ_PRIORITY);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        LOG(0, "VEnc SendFrm Queue, OSR_TOO_MANY\n");
        i4Ret = x_msg_q_send(pCtrl->hRezFrmQ, prFrm, zMsgSize, VENC_FRMQ_PRIORITY);
    }

    ASSERT(i4Ret == OSR_OK);
}


static UINT32 _H264RezSendCmd(VENC_OBJECT_T *this, VENC_CMD_T eRezCmd)
{
    INT32 i4Ret;
    UINT32 u4Cmd = eRezCmd;
    SIZE_T zMsgSize = sizeof(UINT32);
    H264_CTRL_T *pCtrl = (H264_CTRL_T *)this->hCtrl;
    VENC_FRMQ_DATA_T rFrm;

    i4Ret = x_msg_q_send(pCtrl->hRezCmdQ, &u4Cmd, zMsgSize, VENC_CMDQ_PRIORITY);
    while (i4Ret == OSR_TOO_MANY)
    {
        x_thread_delay(10);
        i4Ret = x_msg_q_send(pCtrl->hRezCmdQ, &u4Cmd, zMsgSize, VENC_CMDQ_PRIORITY);
    }

    ASSERT(i4Ret == OSR_OK);

    rFrm.eType = VENC_FRMQ_TYPE_DUMMY;
    _H264RezSendFrm(this, &rFrm);

    return VENC_OK;
}


static UINT32 _H264RezReceiveCmd(H264_CTRL_T *pCtrl, VOID *pvData)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;

    ASSERT(pCtrl);

    zMsgSize = sizeof(UINT32);

    if (pCtrl->eRezCmdState != VENC_STATE_RUN)
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                                &(pCtrl->hRezCmdQ), 1, X_MSGQ_OPTION_WAIT);
    }
    else
    {
        i4Ret = x_msg_q_receive(&u2MsgQIdx, pvData, &zMsgSize,
                                &(pCtrl->hRezCmdQ), 1, X_MSGQ_OPTION_NOWAIT);
    }

    ASSERT(zMsgSize == sizeof(UINT32));

    if (i4Ret != OSR_OK)
    {
        return VENC_INTERNAL_ERR;
    }

    return VENC_OK;
}

static UINT32 _H264RezProcessCmd(VENC_OBJECT_T *this, UINT32 u4Cmd, BOOL *pfgRun)
{
    VENC_CMD_T eCmd = (VENC_CMD_T)u4Cmd;
    H264_CTRL_T *pCtrl = (H264_CTRL_T *)this->hCtrl;

    if (eCmd == VENC_CMD_START_ASYNC || eCmd == VENC_CMD_START_SYNC)
    {
        _H264_FUNC_ENTER();
        if (pCtrl->eRezCmdState != VENC_STATE_STOP)
        {
            return VENC_OK;
        }
        pCtrl->eRezCmdState   = VENC_STATE_RUN;
        pCtrl->eRezFrmState   = H264_REZ_STATE_REZ;
    }
    else if (eCmd == VENC_CMD_STOP_ASYNC || eCmd == VENC_CMD_STOP_SYNC)
    {
        _H264_FUNC_ENTER();
        pCtrl->eRezCmdState    = VENC_STATE_STOP;
        pCtrl->eRezFrmState   = H264_REZ_STATE_IDLE;

    }
    else if (eCmd == VENC_CMD_EXIT_ASYNC || eCmd == VENC_CMD_EXIT_SYNC)
    {
        pCtrl->eRezCmdState    = VENC_STATE_STOP;
        pCtrl->eRezFrmState   = H264_REZ_STATE_IDLE;
        *pfgRun = FALSE;
    }

    if ((((UINT8)eCmd)>>4) == 0X1)   //SYNC
    {
        _VEncNotifySyncCmdDone(this);
    }

    return VENC_OK;
}


static VOID _H264RezRoutine(VENC_OBJECT_T *this)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;
    VENC_FRMQ_DATA_T tData;
    H264_CTRL_T *pCtrl;

    zMsgSize = sizeof(VENC_FRMQ_DATA_T);
    pCtrl    = (H264_CTRL_T *)(this->hCtrl);
    i4Ret    = x_msg_q_receive(&u2MsgQIdx, (VOID*)&tData, &zMsgSize,
                            &(pCtrl->hRezFrmQ), 1, X_MSGQ_OPTION_WAIT);

    ASSERT(i4Ret == OSR_OK);

    if (VENC_FRMQ_TYPE_FRM == tData.eType)
    {
        UINT32 u4Ret;
        _H264_FUNC_ENTER();

        if (tData.u.frm.tIn.fgRzSrc)
        {
            extern VOID VencRezSrc(VENC_OBJECT_T *this, VENC_IN_PIC_T *ptIn);
            VencRezSrc(this, &tData.u.frm.tIn);
        }

        ASSERT(pCtrl && pCtrl->ptCoarseBuffMgr && pCtrl->ptCoarseBuffMgr->Get);
        u4Ret = pCtrl->ptCoarseBuffMgr->Get(pCtrl->ptCoarseBuffMgr, (VOID *)(&(tData.u.frm.tPram.data)), VENC_BUFMGR_WAIT);
        ASSERT(u4Ret == VENC_BUFMGR_OK);
        _H264_DBG("_VENC<H264>::rez frame (src,tag) => (0x%08x, 0x%08x)\n", tData.u.frm.tIn.u4YAddr, tData.u.frm.tPram.data);
#ifdef VENC_EMU
        _u4RzYAddr[_u4RzIdx] = tData.u.frm.tIn.u4YAddr;
        _u4RzData[_u4RzIdx] = tData.u.frm.tPram.data;
        if (++_u4RzIdx >= REZ_REC_N) { _u4RzIdx = 0; }
#endif
        ASSERT(pCtrl && pCtrl->ptRezObj && pCtrl->ptRezObj->Resize);
        pCtrl->ptRezObj->Resize(pCtrl->ptRezObj, tData.u.frm.tIn.u4YAddr, tData.u.frm.tPram.data);
        _VENC_SendFrm(this, &tData);
    }
    else if (VENC_FRMQ_TYPE_RT_PARM == tData.eType)
    {
        VENC_RT_PARM_T *ptParm = (VENC_RT_PARM_T *)&(tData.u.tRTParm);
        _H264_DBG("_VENC<H264>::rez send RT PARM, flag -> 0x%x\n", tData.u.tRTCtrl.u4Flag);
        if (ptParm->u4Flag & VENC_RT_PARM_FLAG_RES)
        {
            if (_H264_NEED_COARSE(this->u2Width, this->u2Height) ^ _H264_NEED_COARSE(ptParm->u2Width, ptParm->u2Height))
            {
                ASSERT(0);
            }
            this->u2Width  = ptParm->u2Width;
            this->u2Height = ptParm->u2Height;
            this->u2Pitch  = ptParm->u2Pitch;
            _H264CalcCoarseRes(this);
            _H264RezInit(this, pCtrl);
        }
        _VENC_SendFrm(this, &tData);
    }
    else if (VENC_FRMQ_TYPE_RT_CTRL == tData.eType)
    {
        _H264_DBG("_VENC<H264>::rez send RT CTRL, flag -> 0x%x\n", tData.u.tRTCtrl.u4Flag);
        _VENC_SendFrm(this, &tData);
    }
}


static VOID _H264RezMainLoop(VOID *pvArg)
{
    VENC_OBJECT_T *this;
    H264_CTRL_T* pCtrl;
    UINT32 u4Cmd;
    BOOL fgRun = TRUE;

    _H264_FUNC_ENTER();
    ASSERT(pvArg);

    this  = (VENC_OBJECT_T *)(*(UINT32 *)pvArg);
    pCtrl = (H264_CTRL_T *)(this->hCtrl);

    _H264_DBG("Rez Object:0x%08x\n", (UINT32)this);


    while (fgRun)
    {
        if (VENC_OK == _H264RezReceiveCmd(pCtrl, &u4Cmd))
        {
            _H264RezProcessCmd(this, u4Cmd, &fgRun);
        }

        if (H264_REZ_STATE_REZ == pCtrl->eRezFrmState)
        {
            _H264RezRoutine(this);
        }
    }

    _H264_FUNC_END(VENC_OK);

    x_thread_exit();

}

static UINT32 _H264RezInit(VENC_OBJECT_T *this, H264_CTRL_T* pCtrl)
{
    UINT32 u4Ret = VENC_OK;

    _H264_FUNC_ENTER();

    ASSERT(this && pCtrl);

    if (!(pCtrl->ptRezObj))
    {
        pCtrl->ptRezObj = VencCreateRezObject();//VencCreate68SwRezObject();
        ASSERT(pCtrl->ptRezObj);
        if (!pCtrl->ptRezObj)
        {
            return VENC_FAIL;
        }
    }

    pCtrl->ptRezObj->ePixFmt   = this->ePixFmt;

    pCtrl->ptRezObj->u4SrcH    = this->u2Height;
    pCtrl->ptRezObj->u4SrcW    = this->u2Width;
    pCtrl->ptRezObj->u4SrcPitch= this->u2Pitch;

    pCtrl->ptRezObj->u4TgH     = pCtrl->u2DesHeidht;
    pCtrl->ptRezObj->u4TgW     = pCtrl->u2DesWidth;
    pCtrl->ptRezObj->u4TgPitch = pCtrl->u2DesPitch;

    ASSERT(pCtrl->ptRezObj->Init);
    pCtrl->ptRezObj->Init(pCtrl->ptRezObj);

    //async mode resizer mainloop

    if (VENC_SUPPORT_ASYNC_MODE(this->eSMode))
    {
        INT32 i4Ret = OSR_OK;
        CHAR acName[16];
        UINT16 u2Hash = (UINT32)this;

        // create rez command queue
        if (NULL_HANDLE == pCtrl->hRezCmdQ)
        {
            x_snprintf(acName, 16, "ERCQ%4X", u2Hash);
            i4Ret = x_msg_q_create(&(pCtrl->hRezCmdQ), acName/*"VREZ_CMDQ"*/,
                sizeof(UINT32), VENC_CMDQ_SIZE);
            ASSERT(i4Ret == OSR_OK);
            if (i4Ret != OSR_OK)
            {
                return VENC_FAIL;
            }
        }

        // create rez frame queue
        if (NULL_HANDLE == pCtrl->hRezFrmQ)
        {
            x_snprintf(acName, 16, "ERFQ%4X", u2Hash);
            i4Ret = x_msg_q_create(&(pCtrl->hRezFrmQ), acName/*"VREZ_FRMQ"*/,
                sizeof(VENC_FRMQ_DATA_T), VENC_FRMQ_SIZE);

            ASSERT(i4Ret == OSR_OK);
            if (i4Ret != OSR_OK)
            {
                return VENC_FAIL;
            }
        }

        //sema for sync cmd
        // create rez main loop thread for command receiving
        if (NULL_HANDLE == pCtrl->hRezThread)
        {
            UINT32 arg = (UINT32)this;
            x_snprintf(acName, 16, "ERzT%4X", u2Hash);
            i4Ret = x_thread_create(&(pCtrl->hRezThread),
                acName/*"RezThread"*/, VENC_STACK_SIZE, VENC_THREAD_PRIORITY,
                (x_os_thread_main_fct)_H264RezMainLoop, sizeof(UINT32), (VOID *)&arg);
            ASSERT(i4Ret == OSR_OK);
            if (i4Ret != OSR_OK)
            {
                return VENC_FAIL;
            }
        }

        UNUSED(acName);
        UNUSED(u2Hash);
    }

    _H264_FUNC_LEAVE();

    return u4Ret;
}


static UINT32 _H264BuffInit(VENC_OBJECT_T *this, H264_CTRL_T* pCtrl)
{
    UINT32 u4Ret = VENC_OK;
    UINT32 u4TmpAddr;
    UINT32 u4Idx, u4FrmBufNum;

    UINT32 u4YSize;
    UINT32 u4Value;
    _H264_FUNC_ENTER();

    // ********************************************************************
    // h.264 video encoder memory allocation rule
    // put memory written by same module together
    // written module is listed as follows:
    // 1. Coarse 0~6 is IMGRZ or CPU(SWIMGRZ)
    // 2. RC temp buffer is CPU
    // 3. Y/UV 0~1, Working buffer and Bistream buffer is VENC HW
    // ********************************************************************
    u4FrmBufNum = 8;
    u4TmpAddr = this->u4BuffAddr + this->u4UsedSize;
    u4TmpAddr = VENC_ALIGN_MASK(u4TmpAddr, VENC_H264_BUF_ALIGN_MASK);

    u4YSize   = VENC_ALIGN_MASK(this->u2Width, 63) * VENC_ALIGN_MASK(this->u2Height, 31);

    if (u4FrmBufNum > VENC_MAX_FRAME_BUFFER_NUM)
    {
        LOG(1, "%s: (u4FrmBufNum==%d) is too large\n", __FUNCTION__, u4FrmBufNum);
        ASSERT(0);
        u4FrmBufNum = VENC_MAX_FRAME_BUFFER_NUM;
    }

    // Rate Control Rom Code buffer
    pCtrl->u4RomCodeAddr = u4TmpAddr;
    u4TmpAddr += VENC_H264_ROM_CODE_SIZE;
    u4TmpAddr = VENC_ALIGN_MASK(u4TmpAddr, VENC_H264_BUF_ALIGN_MASK);

    // SW NALU buffer
    pCtrl->u4NaluBufSa = u4TmpAddr;
    pCtrl->u4NaluBufWp = u4TmpAddr;
    pCtrl->u4NaluBufSz = VENC_H264_SW_NALU_BUF_SIZE;
    u4TmpAddr += VENC_H264_SW_NALU_BUF_SIZE;
    pCtrl->u4NaluBufEa = u4TmpAddr;
    u4TmpAddr = VENC_ALIGN_MASK(u4TmpAddr, VENC_H264_BUF_ALIGN_MASK);

    // Working buffer
    pCtrl->u4UpperInfoMBAddr = u4TmpAddr;
    u4TmpAddr += VENC_H264_UPPER_INFO_MB_SIZE;
    u4TmpAddr = VENC_ALIGN_MASK(u4TmpAddr, VENC_H264_BUF_ALIGN_MASK);

    pCtrl->u4ColInfoAddr = u4TmpAddr;
    u4TmpAddr += VENC_H264_COL_INFO_SIZE;
    u4TmpAddr = VENC_ALIGN_MASK(u4TmpAddr, VENC_H264_BUF_ALIGN_MASK);

    pCtrl->u4CSInfoAddr = u4TmpAddr;
    u4TmpAddr += VENC_H264_CS_INFO_SIZE;
    u4TmpAddr = VENC_ALIGN_MASK(u4TmpAddr, VENC_H264_BUF_ALIGN_MASK);

    // Dump info buffer
    pCtrl->u4DumpInfoAddr = u4TmpAddr;
    u4TmpAddr += VENC_H264_DUMP_INFO_SIZE;
    u4TmpAddr = VENC_ALIGN_MASK(u4TmpAddr, VENC_H264_BUF_ALIGN_MASK);

    // Luma frame buffer recon.
    //u4Value = u4YSize;
    u4Value = MAX(u4YSize, VENC_H264_MAX_VIDEO_RESOLUTION); //For changing encode parameters in run-time
    for (u4Idx = 0; u4Idx < 2; u4Idx++)
    {
        pCtrl->u4YFrmBuffAddr[u4Idx] = u4TmpAddr;
        //u4TmpAddr += VENC_H264_MAX_VIDEO_RESOLUTION;
        u4TmpAddr += u4Value;
        u4TmpAddr = VENC_ALIGN_MASK(u4TmpAddr, VENC_H264_BUF_ALIGN_MASK);
    }

    // Chroma frame buffer 0~1
    //u4Value = u4YSize >> 1;
    u4Value = MAX((u4YSize >> 1), (VENC_H264_MAX_VIDEO_RESOLUTION >> 1)); //For changing encode parameters in run-time
    for (u4Idx = 0; u4Idx < 2; u4Idx++)
    {
        pCtrl->u4UVFrmBuffAddr[u4Idx] = u4TmpAddr;
        u4TmpAddr += u4Value;
        u4TmpAddr = VENC_ALIGN_MASK(u4TmpAddr, VENC_H264_BUF_ALIGN_MASK);
    }

    // Bitstream buffer
#ifdef VENC_DUMP_BS
#ifdef CC_DRIVER_DEMO
#define DUMP_SIZE (64 * 1024 * 1024)
    if (_u4BsTmp == NULL)
    {
        _u4BsTmp = (UINT32)BSP_AllocAlignedDmaMemory(DUMP_SIZE, 1024);
        ASSERT(_u4BsTmp);
    }

    pCtrl->u4BsSa = _u4BsTmp;
    pCtrl->u4NextCpSa = pCtrl->u4BsSa;
    pCtrl->u4BsSz = DUMP_SIZE;
    pCtrl->u4BsEa = pCtrl->u4BsSa + pCtrl->u4BsSz;
    pCtrl->u4InitWp = pCtrl->u4BsSa; //for interal reset
#else
    {
        FBM_POOL_T *prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_MPEG);
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0);
        ASSERT(prFbmPool->u4Size != 0);

        pCtrl->u4BsSa = prFbmPool->u4Addr;// + VENC_RESIZE_BUF_TOTAL;
        pCtrl->u4NextCpSa = pCtrl->u4BsSa;
        _u4BsTmp = pCtrl->u4BsSa;
        pCtrl->u4BsSz = prFbmPool->u4Size;// - VENC_RESIZE_BUF_TOTAL;
        pCtrl->u4BsEa = pCtrl->u4BsSa + pCtrl->u4BsSz;
        pCtrl->u4InitWp = pCtrl->u4BsSa; //for interal reset
    }
#endif
#else
    pCtrl->u4BsSa = u4TmpAddr;
    pCtrl->u4NextCpSa = pCtrl->u4BsSa;
    _u4BsTmp = pCtrl->u4BsSa;
    pCtrl->u4BsSz = VENC_H264_BS_SIZE;
    u4TmpAddr += pCtrl->u4BsSz;
    pCtrl->u4BsEa = u4TmpAddr;
    u4TmpAddr = VENC_ALIGN_MASK(u4TmpAddr, VENC_H264_BUF_ALIGN_MASK);
    pCtrl->u4InitWp = pCtrl->u4BsSa; //for interal reset
#endif

    HalInvalidateDCacheMultipleLine(VIRTUAL(pCtrl->u4BsSa), pCtrl->u4BsSz);

    //coarse buffer
    if (pCtrl->fgCoarse)
    {
        //create buffer mgr first
        if (!(pCtrl->ptCoarseBuffMgr))
        {
            VENC_BUFMGR_T(h264Rez) *pObj = NULL;
            pObj = VENC_BUFMGR_OBJ_CREATE(h264Rez);
            _ptCsBuffMgrDbg = pObj;
            UNUSED(_ptCsBuffMgrDbg);
            ASSERT(pObj);
            pCtrl->ptCoarseBuffMgr = (VENC_BUFMGR_BASE_T *)pObj;
        }
        _BUFMGR_OBJ_Reset(pCtrl->ptCoarseBuffMgr);

        u4Value = pCtrl->u2DesPitch * VENC_ALIGN_MASK(pCtrl->u2DesHeidht, 15);
        u4Value = MAX(u4Value, VENC_H264_MAX_COARSE_RES); //For changing encode parameters in run-time
        u4Value = VENC_ALIGN_MASK(u4Value, (VIDEOCODEC_HAL_DEF_DRAM_PAGE_SIZE - 1));
        // Coarse buffer 0~6
        for (u4Idx = 0; u4Idx < u4FrmBufNum - 1; u4Idx++)
        {
            _BUFMGR_OBJ_Add(pCtrl->ptCoarseBuffMgr, (VOID *)&u4TmpAddr);
            //u4TmpAddr += VENC_H264_MAX_COARSE_IMAGE_SIZE;
            u4TmpAddr += u4Value;
            u4TmpAddr = VENC_ALIGN_MASK(u4TmpAddr, VENC_H264_BUF_ALIGN_MASK);
        }
    }

    //check if buffer is enough
    ASSERT(u4TmpAddr <= (this->u4BuffAddr + this->u4BuffSize));
    if (u4TmpAddr > (this->u4BuffAddr + this->u4BuffSize))
    {
        u4Ret = VENC_NO_MEMORY;
    }

    this->u4UsedSize = u4TmpAddr - this->u4BuffAddr;

    _H264_FUNC_LEAVE();

    return u4Ret;
}

static UINT32 _H264MidInit(VENC_OBJECT_T *this)
{
    BOOL fgRet = TRUE;
    #ifndef VENC_DISABLE_MID

    H264_CTRL_T * pCtrl;
    MID_AGENT_ID_T eAgentId;
    UINT32 u4AgentList;
    UINT32 u4LowAddr, u4HighAddr;
    UINT32 u4Region;

    _H264_FUNC_ENTER();

    pCtrl = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);


    fgRet &= MID_Init();
    fgRet &= MID_Reset();

    eAgentId = MID_AGENT_NONE;
    u4AgentList = 0;
    u4Region = 0;

    // Protect VENC not to intrude other's memory
    do
    {
        u4LowAddr  = pCtrl->u4NaluBufSa;
        u4HighAddr = pCtrl->u4BsEa;
        if (u4LowAddr < u4HighAddr)
        {
            u4AgentList = ~((UINT32)1 << MID_AGENT_VENC);
            fgRet &= MID_SetRegionEx2(
                u4Region,
                MID_AGENT_CPU,
                u4LowAddr,
                u4HighAddr,
                MID_FLAG_INVERSE,
                u4AgentList);
            fgRet &= MID_EnableRegionProtect(u4Region);
            _H264_DBG("Region %d: %08x~%08x %s\n", u4Region, u4LowAddr, u4HighAddr, (fgRet)? "ok" : "NG");
            if (++u4Region >= MID_MAX_REGIONS)
            {
                _H264_DBG("_VENC_MidSetting: Mid regions are exhausted\n");
                break;
            }
        }

        // Protect RC Rom Code buffer
        u4LowAddr  = pCtrl->u4RomCodeAddr;
        u4HighAddr = pCtrl->u4NaluBufSa;
        if (u4LowAddr < u4HighAddr)
        {
            eAgentId = MID_AGENT_CPU;
            fgRet &= MID_SetRegion(u4Region, eAgentId, u4LowAddr, u4HighAddr);
            fgRet &= MID_EnableRegionProtect(u4Region);
            _H264_DBG("Region %d: %08x~%08x %s\n", u4Region, u4LowAddr, u4HighAddr, (fgRet)? "ok" : "NG");
            if (++u4Region >= MID_MAX_REGIONS)
            {
                _H264_DBG("_VENC_MidSetting: Mid regions are exhausted\n");
                break;
            }
        }
    }while (0);

    _H264_FUNC_LEAVE();

    #endif /*VENC_DISABLE_MID*/

    UNUSED(this);

    return fgRet ? VENC_OK : VENC_FAIL;

}
static UINT32 _H264VarInit(VENC_OBJECT_T *this)
{
    H264_CTRL_T * pCtrl     = (H264_CTRL_T * )this->hCtrl;
    H264_MMR_PARAM_T * pMMR = (H264_MMR_PARAM_T *)this->hParam;

    if (!pCtrl->u1RepeatHeader)
    {
        pCtrl->u1RepeatHeader = 1;  //ONCE
    }
    pCtrl->fgWriteSEI     = (pMMR->write_pic_timing)? TRUE : FALSE;
    pCtrl->u1BFrameNo     = pMMR->mmr_num_of_b_frm_2;
    pCtrl->u2IDRFrmPeriod = pMMR->mmr_period_idr_frm_10;
    pCtrl->u2IFrmPeriod   = pMMR->mmr_period_i_frm_10;
    pCtrl->fgCoarse       = _H264_NEED_COARSE(this->u2Width, this->u2Height) ? TRUE : FALSE;

    return VENC_OK;
}


static UINT32 _H264CalcCoarseRes(VENC_OBJECT_T *this)
{
    UINT32 u4BufWidth, u4BufHeight;
    H264_CTRL_T * pCtrl     = (H264_CTRL_T * )this->hCtrl;
    H264_MMR_PARAM_T * pMMR = (H264_MMR_PARAM_T *)this->hParam;

    u4BufWidth = VENC_ALIGN_MASK(this->u2Width, 15);
    if (pMMR->mmr_use_mbaff_1)
    {
        u4BufHeight = VENC_ALIGN_MASK(this->u2Height, 31);
    }
    else
    {
        u4BufHeight = VENC_ALIGN_MASK(this->u2Height, 15);
    }

    pCtrl->u2DesWidth  = ((u4BufWidth >>2) >> 4) << 4;
    pCtrl->u2DesHeidht = VENC_ALIGN_MASK((u4BufHeight >>2), 7);
    pCtrl->u2DesPitch  = VENC_ALIGN_MASK(pCtrl->u2DesWidth, 15);//pCtrl->u2DesWidth;

    return VENC_OK;

}

static UINT32 _H264MmrMECfg(VENC_OBJECT_T *this)
{
    H264_MMR_PARAM_T * pMMR = (H264_MMR_PARAM_T *)this->hParam;
    UINT32 u4Val;

    _H264_FUNC_ENTER();

#if defined(__MODEL_slt__) // SLT should enable all ME function
    UNUSED(u4Val);
    pMMR->mmr_en_b_inter16x08_1 = 1;
    pMMR->mmr_en_b_inter08x16_1 = 1;
    pMMR->mmr_en_b_inter08x08_1 = 1;
#else
    //ME
    u4Val  = VENC_ALIGN_MASK(this->u2Width, 15) / 16;
    u4Val *= VENC_ALIGN_MASK(this->u2Height, 31) / 32 * 2;
    //u4NumMB
    ASSERT(u4Val);
    //u4CyclePerMB
    u4Val = VENC_H264_HW_CYCLE / (this->u1Framerate * u4Val);
    if (u4Val > VENC_H264_BME11111)
    {
        pMMR->mmr_en_b_inter16x08_1 = 1;
        pMMR->mmr_en_b_inter08x16_1 = 1;
        pMMR->mmr_en_b_inter08x08_1 = 1;
    }
    else if (u4Val > VENC_H264_BME11101)
    {
        pMMR->mmr_en_b_inter16x08_1 = 1;
        pMMR->mmr_en_b_inter08x16_1 = 0;
        pMMR->mmr_en_b_inter08x08_1 = 1;
    }
    else if (u4Val > VENC_H264_BME11001)
    {
        pMMR->mmr_en_b_inter16x08_1 = 0;
        pMMR->mmr_en_b_inter08x16_1 = 0;
        pMMR->mmr_en_b_inter08x08_1 = 1;
    }
    else if (u4Val > VENC_H264_BME11000)
    {
        pMMR->mmr_en_b_inter16x08_1 = 0;
        pMMR->mmr_en_b_inter08x16_1 = 0;
        pMMR->mmr_en_b_inter08x08_1 = 0;
    }
    else
    {
        LOG(3, "%dx%d, %d fps might be over spec.\nMaybe not to encode B frame\n",
            this->u2Width, this->u2Height, this->u1Framerate);
        pMMR->mmr_en_b_inter16x08_1 = 0;
        pMMR->mmr_en_b_inter08x16_1 = 0;
        pMMR->mmr_en_b_inter08x08_1 = 0;
    }
#endif

    _H264_FUNC_LEAVE();

    return VENC_OK;
}

static UINT32 _H264MmrImageCfg(VENC_OBJECT_T *this)
{
    H264_CTRL_T * pCtrl     = (H264_CTRL_T * )this->hCtrl;
    H264_MMR_PARAM_T * pMMR = (H264_MMR_PARAM_T *)this->hParam;
    UINT32 u4BufWidth, u4BufHeight, u4MinCompRatio;
    UINT32 u4Val;

    _H264_FUNC_ENTER();

    //IMAGE
    u4BufWidth = VENC_ALIGN_MASK(this->u2Width, 15);
    if (pMMR->mmr_use_mbaff_1)
    {
        u4BufHeight = VENC_ALIGN_MASK(this->u2Height, 31);
    }
    else
    {
        u4BufHeight = VENC_ALIGN_MASK(this->u2Height, 15);
    }

    pMMR->mmr_image_width_div16_7  = (u4BufWidth  >> 4);
    pMMR->mmr_image_height_div16_6 = (u4BufHeight >> 4);
    pMMR->mmr_image_crop_right_4   = u4BufWidth  - this->u2Width;
    pMMR->mmr_image_crop_bottom_5  = u4BufHeight - this->u2Height;

    u4Val = pMMR->mmr_image_width_div16_7 * pMMR->mmr_image_height_div16_6;
    pMMR->mmr_num_mb_in_frm_12 = u4Val;

    pMMR->mmr_coarse_image_width_div8_6  = pCtrl->u2DesWidth >>3;
    pMMR->mmr_coarse_image_height_div8_5 = pCtrl->u2DesHeidht>>3;
    pMMR->mmr_coarse_image_crop_bottom_3 = (8 - (pCtrl->u2DesHeidht & 7)) & 7;

    u4Val = pMMR->mmr_coarse_image_width_div8_6 * pMMR->mmr_coarse_image_height_div8_5;
    pMMR->mmr_num_blk_in_coarse_frm_10 = u4Val;

    pMMR->mmr_coarse_multiplier_7 = (8192 + (u4Val / 2)) / u4Val;

    switch(pMMR->mmr_h264_level_6)
    {
    default:
        u4MinCompRatio = 2;
        break;
    case 31:
    case 32:
    case 40:
        u4MinCompRatio = 4;
        break;
    }

    pMMR->mmr_max_pic_bitstream_byte_cnt_22 = u4BufWidth * VENC_ALIGN_MASK(this->u2Height, 15) * 3 / 2 / u4MinCompRatio;
    pMMR->mmr_max_pic_bitstream_byte_cnt_22 -= pMMR->mmr_max_pic_bitstream_byte_cnt_22 % (pMMR->mmr_dram_page_size_div256_7 << 8);

    _H264_FUNC_LEAVE();

    return VENC_OK;
}

static UINT32 _H264MMRInit(VENC_OBJECT_T *this)
{
    H264_CTRL_T * pCtrl     = (H264_CTRL_T * )this->hCtrl;
    H264_MMR_PARAM_T * pMMR = (H264_MMR_PARAM_T *)this->hParam;

    _H264_FUNC_ENTER();

    //emu
    if (pCtrl->fgEMUDirectMode)
    {
        pCtrl->fgCoarse    = pMMR->mmr_use_coarse_search_1;
        pCtrl->u2DesWidth  = pMMR->mmr_coarse_image_width_div8_6  << 3;
        pCtrl->u2DesHeidht = pMMR->mmr_coarse_image_height_div8_5 <<3;
        pCtrl->u2DesPitch  = VENC_ALIGN_MASK(pCtrl->u2DesWidth, 15);
        return VENC_OK;
    }

    //input source format
    pMMR->mmr_source_scan_type_2  = 0;
    pMMR->mmr_pitch_width_in_pels = this->u2Pitch;
    if (VENC_PIX_FMT_B16X32 == this->ePixFmt)
    {
        pMMR->mmr_source_scan_type_2  = 1;
    }
    else if (VENC_PIX_FMT_B64X32 == this->ePixFmt)
    {
        pMMR->mmr_source_scan_type_2  = 2;
    }

    //encode parameters
    _H264MmrMECfg(this);

    //image part
    _H264MmrImageCfg(this);

    //bitrate
    pMMR->mmr_rc_target_bit_rate_14 = this->u4Bitrate;

    pMMR->mmr_vui_time_scale = this->u1Framerate * 1000;

    //rate control
#ifndef VENC_EMU
    pMMR->mmr_use_rc_1 = pCtrl->fgCoarse ? 1:0;
#endif

#ifdef __MODEL_slt__
    pMMR->mmr_crc_check_enable_1 = 1;
#endif

    _H264_FUNC_LEAVE();

    return VENC_OK;
}

//SPS FUNCTION
UINT32 _H264GenSPS_SW(VENC_OBJECT_T *this, UINT8 *pbBuff, UINT32 *pu4Size)
{
    #define SPS_PUT_BIT(name, val, n) PUT_BIT(val,n)
    #define SPS_PUT_BIT_UE(name, val) PUT_BIT_UE(val)
    #define SPS_PUT_BIT_SE(name, val) PUT_BIT_SE(val)

    UINT32 u4Temp;
    UINT32 buff_size = 0;
    UINT8  profile_idc = 0;
    UINT8  pic_order_cnt_type = 0;
    UINT8  frame_mbs_only_flag = 0;
    UINT8  frame_cropping_flag = 0;
    UINT8  vui_parameters_present_flag = 0;
    H264_MMR_PARAM_T *pMMR = NULL;
    PUT_BIT_START(pbBuff);

    _H264_FUNC_ENTER();

    ASSERT(this && pbBuff && pu4Size);

    if (!this || !pbBuff || !pu4Size)
    {
        return VENC_INV_ARG;
    }

    pMMR = (H264_MMR_PARAM_T *)(this->hParam);

    frame_mbs_only_flag = (pMMR->mmr_use_mbaff_1)? 0 : 1;
    if (pMMR->mmr_image_crop_right_4 > 0 || pMMR->mmr_image_crop_bottom_5 > 0)
    {
        frame_cropping_flag = 1;
    }
    else
    {
        frame_cropping_flag = 0;
    }

    //Start code
    SPS_PUT_BIT("start code", 0x00000001, 32);


    //NAL UNIT
    SPS_PUT_BIT("forbidden_zero_bit", 0, 1);
    SPS_PUT_BIT("nal_ref_idc", 3, 2);
    SPS_PUT_BIT("nal_unit_type", 7, 5);


    //Sequence parameter set RBSP
    profile_idc = pMMR->mmr_h264_profile_8;
    SPS_PUT_BIT("profile_idc", profile_idc, 8);

    SPS_PUT_BIT("constraint_set0_flag", 0, 1);
    SPS_PUT_BIT("constraint_set1_flag", 0, 1);
    SPS_PUT_BIT("constraint_set2_flag", 0, 1);
    SPS_PUT_BIT("constraint_set3_flag", 0, 1);

    SPS_PUT_BIT("reserved_zero_4bits /* equal to 0 */", 0, 4);

    SPS_PUT_BIT("level_idc", pMMR->mmr_h264_level_6, 8);

    SPS_PUT_BIT_UE("seq_parameter_set_id (ue(v))", 0);
    if (profile_idc == 100 || profile_idc == 110 ||
        profile_idc == 122 || profile_idc == 144)
    {
        UINT8 chroma_format_idc = 1; // 4:2:0
        UINT8 seq_scaling_matrix_present_flag = 0;
        SPS_PUT_BIT_UE("chroma_format_idc (ue(v))", chroma_format_idc);
        if( chroma_format_idc == 3 )
        {
            SPS_PUT_BIT("residual_colour_transform_flag", 0, 1);
        }

        SPS_PUT_BIT_UE("bit_depth_luma_minus8 (ue(v))", 0);
        SPS_PUT_BIT_UE("bit_depth_chroma_minus8 (ue(v))", 0);
        SPS_PUT_BIT("qpprime_y_zero_transform_bypass_flag", 0, 1);
        SPS_PUT_BIT("seq_scaling_matrix_present_flag", seq_scaling_matrix_present_flag, 1);
        if (seq_scaling_matrix_present_flag)
        {
            UINT8 i;
            UINT8 seq_scaling_list_present_flag[8] ={0};
            for (i = 0; i < 8; i++)
            {
                if (seq_scaling_list_present_flag[i])
                {
                    if (i < 6)
                    {
                        //scaling_list( ScalingList4x4[ i ], 16,UseDefaultScalingMatrix4x4Flag[ i ])
                    }
                    else
                    {
                        //scaling_list( ScalingList8x8[ i ?6 ], 64,UseDefaultScalingMatrix8x8Flag[ i ?6 ] )
                    }
                }
            }
        }
    }


    SPS_PUT_BIT_UE("log2_max_frame_num_minus4 (ue(v))", 12);/*hard code 12*/
    SPS_PUT_BIT_UE("pic_order_cnt_type (ue(v))", pic_order_cnt_type);        /*hard code 0*/

    if (pic_order_cnt_type == 0)
    {
        SPS_PUT_BIT_UE("log2_max_pic_order_cnt_lsb_minus4 (ue(v))", 12); /*hard code 12*/
    }
    else if (pic_order_cnt_type == 1)
    {
        UINT8 num_ref_frames_in_pic_order_cnt_cycle = 0;
        UINT8 i;
        SPS_PUT_BIT("delta_pic_order_always_zero_flag ", 0, 1);
        SPS_PUT_BIT_SE("offset_for_non_ref_pic se(v) ", 0);
        SPS_PUT_BIT_SE("offset_for_top_to_bottom_field se(v) ", 0);
        SPS_PUT_BIT_SE("num_ref_frames_in_pic_order_cnt_cycle se(v) ", num_ref_frames_in_pic_order_cnt_cycle);
        for( i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++ )
        {
            SPS_PUT_BIT_SE("offset_for_ref_frame[i] se(v) ", 0);
        }
    }

    SPS_PUT_BIT_UE("num_ref_frames (ue(v))", 2);
    SPS_PUT_BIT("gaps_in_frame_num_value_allowed_flag", 0, 1);

    u4Temp = (pMMR->mmr_image_width_div16_7 - 1);
    //Printf("mbs_width: %d\n", u4Temp);
    SPS_PUT_BIT_UE("pic_width_in_mbs_minus1 (ue(v))", u4Temp);

    u4Temp = ((pMMR->mmr_image_height_div16_6 / (2 - frame_mbs_only_flag)) - 1);
    //Printf("height_mbs: %d\n", u4Temp);
    SPS_PUT_BIT_UE("pic_height_in_map_units_minus1 (ue(v))", u4Temp);
    SPS_PUT_BIT("frame_mbs_only_flag", frame_mbs_only_flag, 1);
    if (!frame_mbs_only_flag)
    {
        SPS_PUT_BIT("mb_adaptive_frame_field_flag", 1, 1);
    }

    SPS_PUT_BIT("direct_8x8_inference_flag", 1, 1);  //hard code
    SPS_PUT_BIT("frame_cropping_flag", frame_cropping_flag, 1);
    if (frame_cropping_flag)
    {
        SPS_PUT_BIT_UE("frame_crop_left_offset (ue(v))", 0);
        SPS_PUT_BIT_UE("frame_crop_right_offset (ue(v))", pMMR->mmr_image_crop_right_4);
        SPS_PUT_BIT_UE("frame_crop_top_offset (ue(v))", 0);
        SPS_PUT_BIT_UE("frame_crop_bottom_offset (ue(v))", pMMR->mmr_image_crop_bottom_5);
    }

    {
    //vui_parameter()
    vui_parameters_present_flag = aspect_ratio_info_present_flag |
                                  overscan_info_present_flag|
                                  video_signal_type_present_flag|
                                  chroma_loc_info_present_flag|
                                  timing_info_present_flag|
                                  nal_hrd_parameters_present_flag|
                                  vcl_hrd_parameters_present_flag|
                                  pic_struct_present_flag|
                                  bitstream_restriction_flag;

    SPS_PUT_BIT("vui_parameters_present_flag", vui_parameters_present_flag, 1);
    if (vui_parameters_present_flag)
    {
        SPS_PUT_BIT("aspect_ratio_info_present_flag", aspect_ratio_info_present_flag, 1);
        if( aspect_ratio_info_present_flag )
        {
            UINT8 aspect_ratio_idc = 1;
            SPS_PUT_BIT("aspect_ratio_idc", aspect_ratio_idc, 8);
            if (aspect_ratio_idc == Extended_SAR)
            {
                SPS_PUT_BIT("sar_width", 1, 16);
                SPS_PUT_BIT("sar_height", 1, 16);
            }
        }
        SPS_PUT_BIT("overscan_info_present_flag", overscan_info_present_flag, 1);
        if( overscan_info_present_flag )
        {
            SPS_PUT_BIT("overscan_appropriate_flag", 0, 1);
        }

        SPS_PUT_BIT("video_signal_type_present_flag", video_signal_type_present_flag, 1);
        if( video_signal_type_present_flag )
        {
            UINT8 video_format = 5;
            UINT8 colour_description_present_flag = 1;
            SPS_PUT_BIT("video_format", video_format, 3);
            SPS_PUT_BIT("video_full_range_flag", 0, 1);
            SPS_PUT_BIT("colour_description_present_flag", colour_description_present_flag, 1);
            if( colour_description_present_flag )
            {
                SPS_PUT_BIT("colour_primaries", 6, 8);
                SPS_PUT_BIT("transfer_characteristics", 6, 8);
                SPS_PUT_BIT("matrix_coefficients", 6, 8);
            }
        }

        SPS_PUT_BIT("chroma_loc_info_present_flag", chroma_loc_info_present_flag, 1);
        if( chroma_loc_info_present_flag )
        {
            SPS_PUT_BIT_UE("chroma_sample_loc_type_top_field ue(v)", 0);
            SPS_PUT_BIT_UE("chroma_sample_loc_type_bottom_field ue(v)", 0);
        }

        SPS_PUT_BIT("timing_info_present_flag", timing_info_present_flag, 1);
        if( timing_info_present_flag )
        {
            UINT32 scale;
            scale = this->u1Framerate * 1000 * 2;
            SPS_PUT_BIT("num_units_in_tick ", 1000, 32);
            SPS_PUT_BIT("time_scale", scale, 32);
            SPS_PUT_BIT("fixed_frame_rate_flag", 0, 1);
        }

        SPS_PUT_BIT("nal_hrd_parameters_present_flag", nal_hrd_parameters_present_flag, 1);
        if( nal_hrd_parameters_present_flag )
        {
            UINT8 SchedSelIdx;
            SPS_PUT_BIT_UE("cpb_cnt_minus1", cpb_cnt_minus1);
            SPS_PUT_BIT("bit_rate_scale", bit_rate_scale, 4);
            SPS_PUT_BIT("cpb_size_scale", cpb_size_scale, 4);
            for (SchedSelIdx = 0; SchedSelIdx <= cpb_cnt_minus1; SchedSelIdx++)
            {
                SPS_PUT_BIT_UE("bit_rate_value_minus1", ((14000 * 1200) >> (bit_rate_scale + 6)) - 1); // hardcode for level 3.1
                SPS_PUT_BIT_UE("cpb_size_value_minus1", ((14000 * 1200) >> (cpb_size_scale + 4)) - 1); // hardcode for level 3.1
                SPS_PUT_BIT("cbr_flag", 0, 1);
            }
            SPS_PUT_BIT("inital_cpb_removal_delay_length_minus1", 0, 5);
            SPS_PUT_BIT("cpb_removal_delay_length_minus1", cpb_removal_delay_length_minus1, 5);
            SPS_PUT_BIT("dpb_output_delay_length_minus1", dpb_output_delay_length_minus1, 5);
            SPS_PUT_BIT("time_offset_length", 0, 5);
        }
        SPS_PUT_BIT("vcl_hrd_parameters_present_flag", vcl_hrd_parameters_present_flag, 1);
        if( vcl_hrd_parameters_present_flag )
        {
            UINT8 SchedSelIdx;
            SPS_PUT_BIT_UE("cpb_cnt_minus1", cpb_cnt_minus1);
            SPS_PUT_BIT("bit_rate_scale", bit_rate_scale, 4);
            SPS_PUT_BIT("cpb_size_scale", cpb_size_scale, 4);
            for (SchedSelIdx = 0; SchedSelIdx <= cpb_cnt_minus1; SchedSelIdx++)
            {
                SPS_PUT_BIT_UE("bit_rate_value_minus1", ((14000 * 1000) >> (bit_rate_scale + 6)) - 1); // hardcode for level 3.1
                SPS_PUT_BIT_UE("cpb_size_value_minus1", ((14000 * 1000) >> (cpb_size_scale + 4)) - 1); // hardcode for level 3.1
                SPS_PUT_BIT("cbr_flag", 0, 1);
            }
            SPS_PUT_BIT("inital_cpb_removal_delay_length_minus1", 0, 5);
            SPS_PUT_BIT("cpb_removal_delay_length_minus1", cpb_removal_delay_length_minus1, 5);
            SPS_PUT_BIT("dpb_output_delay_length_minus1", dpb_output_delay_length_minus1, 5);
            SPS_PUT_BIT("time_offset_length", 0, 5);
        }
        if( nal_hrd_parameters_present_flag || vcl_hrd_parameters_present_flag )
        {
            SPS_PUT_BIT("low_delay_hrd_flag", 0, 1);
        }

        SPS_PUT_BIT("pic_struct_present_flag", pic_struct_present_flag, 1);
        SPS_PUT_BIT("bitstream_restriction_flag", bitstream_restriction_flag, 1);
        if( bitstream_restriction_flag )
        {
            SPS_PUT_BIT("motion_vectors_over_pic_boundaries_flag", 0, 1);
            SPS_PUT_BIT_UE("max_bytes_per_pic_denom ue(v)", 0);
            SPS_PUT_BIT_UE("max_bits_per_mb_denom ue(v)", 0);
            SPS_PUT_BIT_UE("log2_max_mv_length_horizontal ue(v)", 0);
            SPS_PUT_BIT_UE("log2_max_mv_length_vertical ue(v)", 0);
            SPS_PUT_BIT_UE("num_reorder_frames ue(v)", 0);
            SPS_PUT_BIT_UE("max_dec_frame_buffering ue(v)", 0);
        }

    }
    }
    //rbsp_tailing_bits()
    SPS_PUT_BIT("rbsp_tailing_bits", 1,1);
    ALIGN_BIT();

    PUT_BIT_END(buff_size);

    *pu4Size = buff_size;

    return VENC_OK;

}


//SEI FUNCTION
static VOID _H264SEIPayloadPIC_TIMING(VENC_OBJECT_T *this, VENC_BITS_BUFFER *PBITS_BUFFER, UINT32 *pu4Size, UINT8 mask)
{
    UINT32 size = 0;
    H264_MMR_PARAM_T *pMMR = NULL;

    _H264_FUNC_ENTER();

    ASSERT(this);

    pMMR = (H264_MMR_PARAM_T *)(this->hParam);

    if ( CpbDpbDelaysPresentFlag )
    {
        SEI_PUT_BIT("cpb_removal_delay", 0, cpb_removal_delay_length_minus1 + 1);
        SEI_PUT_BIT("dpb_output_delay", 0, dpb_output_delay_length_minus1 + 1);
    }

    if ( pic_struct_present_flag )
    {
        UINT8 clock_timestamp_flag[3];
        UINT8 NumClockTS;
        UINT8 time_offset_length = 0;
        UINT8 pic_struct;
        UINT8 i;

        //VENC_H264_CONFIG_T *prVEncH264Config = _VENC_H264_GetConfig();
        pic_struct = pMMR->picture_display_type;

        // Value Indicated display of picture  Restrictions                             NumClockTS
        // 3     top field, bottom field,      in that order field_pic_flag shall be 0   2
        SEI_PUT_BIT("pic_struct", pic_struct, 4);
        switch(pic_struct)
        {
        case 0:
        default:
            NumClockTS = 1;
            // frame
            break;
        case 1:
            //top field
        case 2:
            //bottom field
            NumClockTS = 1;
            break;
        case 3:
            //top filed , bottom field, in that order
        case 4:
            //bottom field, top filed , in that order
            NumClockTS = 2;
            break;
        case 5:
            //top filed , bottom field,  top filed repeat, in that order
        case 6:
            //bottom field, top filed , bottom field repeat, in that order
            NumClockTS = 3;
            break;
        case 7:
            //frame double
            NumClockTS = 2;
            break;
        case 8:
            //frame tripling
            NumClockTS = 3;
            break;
        };
        for (i = 0; i < NumClockTS ; i++ )
        {
            clock_timestamp_flag[i] = 0;
            SEI_PUT_BIT("clock_timestamp_flag[ i ]", clock_timestamp_flag[i], 1);
            if ( clock_timestamp_flag[i] )
            {
                UINT8 full_timestamp_flag = 0;
                SEI_PUT_BIT("ct_type", 1, 2);
                SEI_PUT_BIT("nuit_field_based_flag", 0, 1);
                SEI_PUT_BIT("counting_type", 2, 5);
                SEI_PUT_BIT("full_timestamp_flag", full_timestamp_flag, 1);
                SEI_PUT_BIT("discontinuity_flag", 0, 1);
                SEI_PUT_BIT("cnt_dropped_flag", 1, 1);
                SEI_PUT_BIT("n_frames", 1, 8);
                if ( full_timestamp_flag )
                {
                    SEI_PUT_BIT("seconds_value /* 0..59 */", 0, 6);
                    SEI_PUT_BIT("minutes_value /* 0..59 */", 0, 6);
                    SEI_PUT_BIT("hours_value /* 0..23 */", 0, 5);
                }
                else
                {
                    UINT8 seconds_flag = 0;
                    SEI_PUT_BIT("seconds_flag", seconds_flag, 1);
                    if ( seconds_flag )
                    {
                        UINT8 minutes_flag = 0;
                        SEI_PUT_BIT("seconds_value /* range 0..59 */", 0, 6);
                        SEI_PUT_BIT("minutes_flag ", minutes_flag, 1);
                        if ( minutes_flag )
                        {
                            UINT8 hours_flag = 0;
                            SEI_PUT_BIT("minutes_value /* 0..59 */", 0, 6);
                            SEI_PUT_BIT("hours_flag ", 0, 1);
                            if( hours_flag )
                                SEI_PUT_BIT("hours_value /* 0..23 */", 0, 5);
                        }
                    }
                }

                if( time_offset_length > 0 )
                {
                    SEI_PUT_BIT("time_offset", 0, 1);
                }
            }
        }
    }

    //rbsp
    SEI_PUT_BIT("first bit 1", 1, 1);
    SEI_ALIGN_BIT();

    if (mask&SEI_FUNC_MARK_CALC_SIZE) *pu4Size = (size + 7)/8;

}


static UINT32 _H264GenSEI_SW(VENC_OBJECT_T *this, UINT32 *pu4Addr, UINT32 *pu4Size)
{
    UINT8 *pbBuff = (UINT8 *)(*pu4Addr);
    PUT_BIT_START(pbBuff);

    _H264_FUNC_ENTER();

    ASSERT(this && pu4Size);

    //Start code
    PUT_BIT(0x00000001, 32);

    //NAL UNIT
    PUT_BIT(0, 1); //"forbidden_zero_bit"
    PUT_BIT(0, 2); //"nal_ref_idc",
    PUT_BIT(6, 5); //"nal_unit_type",

    /*SEI struct
       sei_rbsp( ) { C Descriptor
           do
               sei_message( )
           while( more_rbsp_data( ) )
           rbsp_trailing_bits( )
       }
       */
    //if (PIC_TIMING)
    PUT_SEI_MESSAGE(this,PIC_TIMING);

    PUT_BIT_END(*pu4Size);

    return VENC_OK;

}

UINT32 _H264GenPPS_SW(VENC_OBJECT_T *this, UINT32 *pu4Addr, UINT32 *pu4Size)
{
    _H264_FUNC_ENTER();

    UNUSED(pu4Addr);
    UNUSED(pu4Size);

    return VENC_OK;
}

static UINT32 _H264Gen1stSPS_HW(H264_CTRL_T *pCtrl, UINT32 *pu4Addr, UINT32 *pu4Size)
{
    _VENC_H264_HalSetStart(VIDEOCODEC_HAL_STARTOPT_FIRST_SPS);
    _VENC_H264_HalSetStartClear(VIDEOCODEC_HAL_STARTOPT_ENCODE_SPS);
    _VENC_H264_HalSetStart(VIDEOCODEC_HAL_STARTOPT_ENCODE_SPS);
    _VENC_H264_HalSetStartClear(VIDEOCODEC_HAL_STARTOPT_ENCODE_SPS);
    _VENC_H264_HalEncDone();
    _VENC_H264_HalSetStartClear(VIDEOCODEC_HAL_STARTOPT_FIRST_SPS);
    //clear reg
    _VEnc_H264_HalClearBSWriteEnableFlag();

    //_VENC_H264_HalDipDone();
    *pu4Addr = _VENC_H264_HalGetOutputCurAddr();
    *pu4Size = _VENC_H264_HalGetOutputCurSize();

    //debug info
    pCtrl->u4NextCpSa = *pu4Addr ;
    pCtrl->u4SPScrc = _VENC_H264_HalGetECCRC();

    return VENC_OK;
}

static UINT32 _H264GenSPS_HW(H264_CTRL_T *pCtrl, UINT32 *pu4Addr, UINT32 *pu4Size)
{
    _VENC_H264_HalSetStartClear(VIDEOCODEC_HAL_STARTOPT_ENCODE_SPS);
    _VENC_H264_HalSetStart(VIDEOCODEC_HAL_STARTOPT_ENCODE_SPS);
    _VENC_H264_HalSetStartClear(VIDEOCODEC_HAL_STARTOPT_ENCODE_SPS);
    _VENC_H264_HalEncDone();
    //clear reg
    _VEnc_H264_HalClearBSWriteEnableFlag();

    *pu4Addr = _VENC_H264_HalGetOutputCurAddr();
    *pu4Size = _VENC_H264_HalGetOutputCurSize();

    //debug info
    pCtrl->u4NextCpSa = *pu4Addr ;
    pCtrl->u4SPScrc = _VENC_H264_HalGetECCRC();

    return VENC_OK;
}

static UINT32 _H264GenPPS_HW(H264_CTRL_T *pCtrl, UINT32 *pu4Addr, UINT32 *pu4Size)
{
    _VENC_H264_HalSetStartClear(VIDEOCODEC_HAL_STARTOPT_ENCODE_PPS);
    _VENC_H264_HalSetStart(VIDEOCODEC_HAL_STARTOPT_ENCODE_PPS);
    _VENC_H264_HalSetStartClear(VIDEOCODEC_HAL_STARTOPT_ENCODE_PPS);
    _VENC_H264_HalEncDone();
    //clear reg
    _VEnc_H264_HalClearBSWriteEnableFlag();

    *pu4Addr = _VENC_H264_HalGetOutputCurAddr();
    *pu4Size = _VENC_H264_HalGetOutputCurSize();

    //debug info
    pCtrl->u4NextCpSa = *pu4Addr ;
    pCtrl->u4PPScrc = _VENC_H264_HalGetECCRC();

    return VENC_OK;
}

static UINT32 _H264GenFRM_HW(VENC_OBJECT_T *this, H264_CTRL_T *pCtrl, UINT32 u4YAddr, UINT32 u4UVAddr, UINT32 u4CosAddr, UINT32 *pu4Addr, UINT32 *pu4Size)
{
    _H264_FUNC_ENTER();

    //LOG(3, "%s(0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x)\n", __FUNCTION__, u4YAddr, u4UVAddr, u4CosAddr, pCtrl->u4BrefCSAddr, pCtrl->u4FrefCSAddr);

    //  Set cur frm y, uv and cs
    _VENC_H264_HalSetYUVAddr(u4YAddr, u4UVAddr, 0);
    _VENC_H264_HalSetYCoarseAddr(u4CosAddr, 0);


    //  Set f-ref and b-ref cs
    _VEnc_H264_HalSetBrefCoarseAddr(pCtrl->u4BrefCSAddr);
    _VEnc_H264_HalSetFrefCoarseAddr(pCtrl->u4FrefCSAddr);


#ifdef VENC_EMU
{
    extern BOOL _fgSkypeTest;
    if (_fgSkypeTest)
    {
        _VEncH264EmuSetSkypeInfo(this);
    }
}
#endif

#ifdef VENC_DRAM_PROFILE
    _VENC_DramAccessByteStart(this);
#endif

    //  Trigger to encode
    _VENC_H264_HalSetStartClear(VIDEOCODEC_HAL_STARTOPT_ENCODE_FRAME);
    _VENC_H264_HalSetStart(VIDEOCODEC_HAL_STARTOPT_ENCODE_FRAME);
    _VENC_H264_HalSetStartClear(VIDEOCODEC_HAL_STARTOPT_ENCODE_FRAME);

    //  encode done
    _VENC_H264_HalEncDone();
    //_VENC_H264_WaitDramWriteDone();

#ifdef VENC_DRAM_PROFILE
    _VENC_DramAccessByteEnd(this);
#endif

    pCtrl->u4FrameNo++;
    *pu4Addr = _VENC_H264_HalGetOutputCurAddr();
    *pu4Size = _VENC_H264_HalGetOutputCurSize();

    //reset reg.
    _VEnc_H264_HalInsertKeyframeOnOff(0);
    _VEnc_H264_HalClearBSWriteEnableFlag();
    _VEnc_H264_HalSetBSReadAddr(_VENC_H264_HalGetOutputCurAddr());

    //debug info
    pCtrl->u4FRMcrc = _VENC_H264_HalGetECCRC();
    pCtrl->u4NextCpSa = *pu4Addr ;

    _H264_FUNC_LEAVE();

    return VENC_OK;
}


//for async

static void _H264EncRefTypeAnalyse(H264_CTRL_T *pCtrl)
{
    if (pCtrl->u4FrameNo == 0)
    {
        pCtrl->eType = VENC_H264_PIC_TYPE_IDR;
    }
    else if (pCtrl->fgForceIDR)
    {
        pCtrl->eType = VENC_H264_PIC_TYPE_IDR;
        pCtrl->fgForceIDR = FALSE;
        pCtrl->u2IDRFrmIdx = 0;
        pCtrl->u2IFrmIdx   = 0;
    }
    else if (pCtrl->u2IDRFrmPeriod > 0 && pCtrl->u2IDRFrmIdx == 0)
    {
        pCtrl->eType = VENC_H264_PIC_TYPE_IDR;
    }
    else if (pCtrl->u2IFrmPeriod > 0 && pCtrl->u2IFrmIdx == 0)
    {
        pCtrl->eType = VENC_H264_PIC_TYPE_I;
    }
    else
    {
        pCtrl->eType = VENC_H264_PIC_TYPE_P;
    }

    if (pCtrl->u2IDRFrmPeriod > 0)
    {
        pCtrl->u2IDRFrmIdx++;
        if (pCtrl->u2IDRFrmIdx >= pCtrl->u2IDRFrmPeriod)
        {
            pCtrl->u2IDRFrmIdx = 0;
        }
    }

    if (pCtrl->u2IFrmPeriod > 0)
    {
        pCtrl->u2IFrmIdx++;
        if (pCtrl->u2IFrmIdx >= pCtrl->u2IFrmPeriod)
        {
            pCtrl->u2IFrmIdx = 0;
        }
    }
}


static UINT32  _H264EncFrameA (VENC_OBJECT_T *this, VENC_IN_PIC_T *pIn, VENC_OUT_PIC_T *pOut, H246_PIC_PRAM* pPram)
{
    UINT32 u4Ret = VENC_OK;
    VENC_IN_PIC_T  *pCurrIn   = pIn;
    VENC_OUT_PIC_T *pCurrOut  = pOut;
    H246_PIC_PRAM  *pCurrPram = pPram;
    UINT32 i;
    H264_CTRL_T *pCtrl;
    UINT32 u4OutAddr = 0;
    UINT32 u4OutSize = 0;
    //UINT32 u4Addr = 0;
    //UINT32 u4Size = 0;

    VENC_OUT_PIC_T rOut;

    _H264_FUNC_ENTER();

    ASSERT(this);

    if (!this)
    {
        return VENC_INV_ARG;
    }

    pCtrl = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);

    rOut.u4RStart = pCtrl->u4BsSa;
    rOut.u4REnd = pCtrl->u4BsEa;

    //INIT
    if (pCtrl->fg1stHeader)
    {
        rOut.u4Pts = pIn->u4Pts;
        _H264Gen1stSPS_HW(pCtrl, &u4OutAddr, &u4OutSize);
        rOut.u4Addr = u4OutAddr;
        rOut.u4Size = u4OutSize;
        if (this->OutputCallback)
        {
            this->OutputCallback(this->pvCallbackData, NULL, &rOut, VENC_H264_PIC_TYPE_SPS);
        }
        pCtrl->fg1stHeader  = FALSE;
        pCtrl->fgSkipHeaderOnce = TRUE;

        //write pps
        _H264GenPPS_HW(pCtrl, &u4OutAddr, &u4OutSize);
        rOut.u4Addr = u4OutAddr;
        rOut.u4Size = u4OutSize;
        if (this->OutputCallback)
        {
            this->OutputCallback(this->pvCallbackData, NULL, &rOut, VENC_H264_PIC_TYPE_PPS);
        }
    }

    //encode seq. 1.IPPPPP
    //                  2.BBIBBPBBPBBP
    //                  3.BIBPBP
    //
    if ((pCtrl->u1BFrmIdx < pCtrl->u1BFrameNo) && (pCtrl->u1BFrmIdx < H264_BFRAME_MAX))
    {
        pCtrl->uaIn[pCtrl->u1BFrmIdx]  = *pCurrIn;
        pCtrl->uaOut[pCtrl->u1BFrmIdx] = *pCurrOut;
        pCtrl->uaPram[pCtrl->u1BFrmIdx]= *pCurrPram;
        pCtrl->u1BFrmIdx++;
        //prepare for dip
        //_VENC_H264_HalSetStartClear(VIDEOCODEC_HAL_STARTOPT_ENCODE_DIP);
        //_VENC_H264_HalSetStart(VIDEOCODEC_HAL_STARTOPT_ENCODE_DIP);
        //_VENC_H264_HalSetStartClear(VIDEOCODEC_HAL_STARTOPT_ENCODE_DIP);
        //_VENC_H264_HalDipDone();
        _H264_FUNC_LEAVE();
        return VENC_WAIT_YUV;
    }
    pCtrl->u1BFrmIdx = 0;

    rOut.u4Pts  = pCurrIn->u4Pts;
    //gen ref frame first
    pCtrl->u4BrefCSAddr = pCurrPram->u4CosAddr;
    _H264EncRefTypeAnalyse(pCtrl);
    if (VENC_H264_PIC_TYPE_IDR == pCtrl->eType)
    {
        //encoder header
        if (pCtrl->u1RepeatHeader > 0)
        {
            //write sps
            if (pCtrl->fgSkipHeaderOnce)
            {
                pCtrl->fgSkipHeaderOnce = FALSE;
            }
            else
            {
                _H264GenSPS_HW(pCtrl, &u4OutAddr, &u4OutSize);
                rOut.u4Addr = u4OutAddr;
                rOut.u4Size = u4OutSize;
                if (this->OutputCallback)
                {
                    this->OutputCallback(this->pvCallbackData, NULL, &rOut, VENC_H264_PIC_TYPE_SPS);
                }

                //write pps
                _H264GenPPS_HW(pCtrl, &u4OutAddr, &u4OutSize);
                rOut.u4Addr = u4OutAddr;
                rOut.u4Size = u4OutSize;
                if (this->OutputCallback)
                {
                    this->OutputCallback(this->pvCallbackData, NULL, &rOut, VENC_H264_PIC_TYPE_PPS);
                }
            }

            //write sei
            if (pCtrl->fgWriteSEI)
            {
#ifndef VENC_DUMP_BS
                VENC_OUT_PIC_T rSeiOut;
                x_memset(&rSeiOut, 0, sizeof(rSeiOut));
                u4OutAddr = VIRTUAL(pCtrl->u4NaluBufWp);
                _H264GenSEI_SW(this, &u4OutAddr, &u4OutSize);
                rSeiOut.u4Pts  = pCurrIn->u4Pts;
                rSeiOut.u4RStart = pCtrl->u4NaluBufSa;
                rSeiOut.u4REnd = pCtrl->u4NaluBufEa;
                rSeiOut.u4Addr = u4OutAddr;
                rSeiOut.u4Size = u4OutSize;
                UP_SW_NALU_WP(pCtrl, u4OutSize);
                if (this->OutputCallback)
                {
                    this->OutputCallback(this->pvCallbackData, NULL, &rSeiOut, VENC_H264_PIC_TYPE_SEI);
                }
#else
                UINT32 u4SwOutAddr;
                _H264GenPPS_HW(pCtrl, &u4OutAddr, &u4OutSize);
                rOut.u4Addr = u4OutAddr;
                rOut.u4Size = VENC_ALIGN_MASK(u4OutSize, 7);
                u4SwOutAddr = VIRTUAL(u4OutAddr);

                u4OutAddr = VIRTUAL(pCtrl->u4NaluBufWp);
                _H264GenSEI_SW(this, &u4OutAddr, &u4OutSize);
                UP_SW_NALU_WP(pCtrl, u4OutSize);
                ASSERT(u4OutSize <= rOut.u4Size);
                x_memset(u4SwOutAddr, 0, rOut.u4Size);
                x_memcpy(u4SwOutAddr, u4OutAddr, u4OutSize);
                HalFlushDCacheMultipleLine(u4SwOutAddr, u4OutSize);
                rOut.u4Size = u4OutSize;
                if (this->OutputCallback)
                {
                    this->OutputCallback(this->pvCallbackData, NULL, &rOut, VENC_H264_PIC_TYPE_SEI);
                }
#endif
            }

            if (pCtrl->u1RepeatHeader == 1)
            {
                pCtrl->u1RepeatHeader = 0;
            }
        }
    }

    _H264GenFRM_HW(this, pCtrl, pCurrIn->u4YAddr, pCurrIn->u4CAddr, pCurrPram->u4CosAddr, &u4OutAddr, &u4OutSize);
    rOut.u4Addr = u4OutAddr;
    rOut.u4Size = u4OutSize;
    if (this->OutputCallback)
    {
        this->OutputCallback(this->pvCallbackData, pCurrIn, &rOut, pCtrl->eType);
    }

    //gen b frame
    pCtrl->eType = VENC_H264_PIC_TYPE_B;
    for (i = 0; (i < pCtrl->u1BFrameNo) && (i < H264_BFRAME_MAX); i++)
    {
        pCurrIn   = &(pCtrl->uaIn[i]);
        pCurrOut  = &(pCtrl->uaOut[i]);
        pCurrPram = &(pCtrl->uaPram[i]);

        _H264GenFRM_HW(this, pCtrl, pCurrIn->u4YAddr, pCurrIn->u4CAddr, pCurrPram->u4CosAddr, &u4OutAddr, &u4OutSize);

        //release cs buffer
        if (pCtrl->fgCoarse)
        {
            pCtrl->ptCoarseBuffMgr->Free(pCtrl->ptCoarseBuffMgr, (VOID*)(&(pCurrPram->u4CosAddr)));
        }
        rOut.u4Pts  = pCurrIn->u4Pts;
        rOut.u4Addr = u4OutAddr;
        rOut.u4Size = u4OutSize;
        if (this->OutputCallback)
        {
            this->OutputCallback(this->pvCallbackData, pCurrIn, &rOut, pCtrl->eType);
        }
    }

    //release f ref corase buffer
    if (pCtrl->fgCoarse && pCtrl->u4FrefCSAddr)
    {
        pCtrl->ptCoarseBuffMgr->Free(pCtrl->ptCoarseBuffMgr, (VOID*)(&(pCtrl->u4FrefCSAddr)));
    }

    //b ref -> f ref
    pCtrl->u4FrefCSAddr = pCtrl->u4BrefCSAddr;

    _H264_FUNC_LEAVE();

    return u4Ret;
}

static UINT32 _H264HWReset(VENC_OBJECT_T *this)
{
    H264_CTRL_T *pCtrl;
    H264_MMR_PARAM_T * pMMR;

    UINT32 u4RP, u4WP;


    ASSERT(this);
    _H264_FUNC_ENTER();

    pCtrl = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);
    pMMR  = VENC_HANDLE_TO_OBJECT(this->hParam, H264_MMR_PARAM_T);

    //release f ref corase buffer
    if (pCtrl->fgCoarse && pCtrl->u4FrefCSAddr)
    {
        pCtrl->ptCoarseBuffMgr->Free(pCtrl->ptCoarseBuffMgr, (VOID*)(&(pCtrl->u4FrefCSAddr)));
    }

    pCtrl->u1RepeatHeader = pCtrl->u1RepeatHeader ? pCtrl->u1RepeatHeader : 1;

    pCtrl->fg1stHeader    = TRUE;
    pCtrl->u4FrefCSAddr   = 0;
    pCtrl->u4BrefCSAddr   = 0;
    pCtrl->u1BFrmIdx      = 0;
    pCtrl->u1BSeqIdx      = 0;
    pCtrl->u1RefSeqIdx    = 0;
    pCtrl->u2IDRFrmIdx    = 0;
    pCtrl->u2IFrmIdx      = 0;
    pCtrl->u4FrameNo      = 0;

    //pitch
    pMMR->mmr_pitch_width_in_pels = this->u2Pitch;

    //encode parameters
    _H264MmrMECfg(this);

    //image part
    _H264MmrImageCfg(this);

    //bitrate
    pMMR->mmr_rc_target_bit_rate_14 = this->u4Bitrate;

    //hw reset
    u4WP = _VEnc_H264_HalGetBSCurrWriteAddr();
    u4RP = _VEnc_H264_HalGetBSReadAddr();
    if (u4WP < pCtrl->u4BsSa || u4WP > pCtrl->u4BsEa)
    {
        u4WP = pCtrl->u4BsSa;
    }

    _VENC_H264_HalStop(this);

    _VENC_H264_HalStart(this);

    _VEnc_H264_HalSetBSWriteAddr(u4WP);
    _VEnc_H264_HalSetBSReadAddr(u4RP);

    _H264_FUNC_LEAVE();

    return VENC_OK;
}

static UINT32 _H264RtReset(VENC_OBJECT_T *this)
{
    H264_MMR_PARAM_T *pMMR;

    ASSERT(this);
    pMMR  = VENC_HANDLE_TO_OBJECT(this->hParam, H264_MMR_PARAM_T);

    //Mmr
    _H264MmrMECfg(this);

    //bitrate
    pMMR->mmr_rc_target_bit_rate_14 = this->u4Bitrate;

    //hw runtime reset
    _VENC_H264_HalRtReset(this);

    return VENC_OK;
}

static UINT32 _H264EncRoutine(VENC_OBJECT_T *this)
{
    UINT16 u2MsgQIdx;
    SIZE_T zMsgSize;
    INT32 i4Ret;
    VENC_FRMQ_DATA_T tData;

    zMsgSize = sizeof(VENC_FRMQ_DATA_T);

    i4Ret = x_msg_q_receive(&u2MsgQIdx, (VOID*)&tData, &zMsgSize,
                            &(this->hEncFrmQ), 1, X_MSGQ_OPTION_WAIT);
    ASSERT(i4Ret == OSR_OK);

    _H264_FUNC_ENTER();
    _H264_DBG("_VENC<H264>::(eType) = >(%d)\n", (UINT32)tData.eType);

    if (VENC_FRMQ_TYPE_FRM == tData.eType)
    {
        _H264EncFrameA(this, &(tData.u.frm.tIn), &(tData.u.frm.tOut), (H246_PIC_PRAM *)&(tData.u.frm.tPram));
    }
    else if (VENC_FRMQ_TYPE_RT_CTRL == tData.eType)
    {
        H264_CTRL_T * pCtrl    = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);
        VENC_RT_CTRL_T *ptParm = (VENC_RT_CTRL_T *)&(tData.u.tRTCtrl);
        if (ptParm->u4Flag & VENC_RT_CTRL_FLAG_INSERT_KEYFRAME)
        {
            _H264_DBG("_VENC<H264>:: insert keyframe\n");
            _VEnc_H264_HalInsertKeyframeOnOff(1);
            pCtrl->u1RepeatHeader = 1;
            pCtrl->fgForceIDR = TRUE;
        }
    }
    else if (VENC_FRMQ_TYPE_RT_PARM == tData.eType)
    {
        H264_CTRL_T * pCtrl    = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);
        VENC_RT_PARM_T *ptParm = (VENC_RT_PARM_T *)&(tData.u.tRTParm);
        BOOL fgResetHW = FALSE;
        if (ptParm->u4Flag & VENC_RT_PARM_FLAG_RES)
        {
            UINT8 i;
            this->u2Width  = ptParm->u2Width;
            this->u2Height = ptParm->u2Height;
            this->u2Pitch  = ptParm->u2Pitch;

            //release  buffer
            for (i = 0; i < (pCtrl->u1BFrmIdx) && (i<H264_BFRAME_MAX); i++)
            {
                //coarse buffer
                if (pCtrl->fgCoarse)
                {
                    pCtrl->ptCoarseBuffMgr->Free(pCtrl->ptCoarseBuffMgr, (VOID*)(&(pCtrl->uaPram[i].u4CosAddr)));
                }
                //yuv buffer
                if (this->OutputCallback)
                {
                    this->OutputCallback(this->pvCallbackData, &(pCtrl->uaIn[i]), NULL, (UINT32)(-1));
                }
            }
            fgResetHW = TRUE;
        }

        if (ptParm->u4Flag & VENC_RT_PARM_FLAG_BITRATE)
        {
            this->u4Bitrate = ptParm->u4Bitrate;
        }

        if (ptParm->u4Flag & VENC_RT_PARM_FLAG_FRAMERATE)
        {
            this->u1Framerate = ptParm->u1Framerate;
        }

        if (fgResetHW)
        {
            _H264HWReset(this);
        }
        else
        {
            _H264RtReset(this);
        }

    }
    else if (VENC_FRMQ_TYPE_DUMMY != tData.eType)
    {
        if (this->OutputCallback)
        {
            this->OutputCallback(this->pvCallbackData, &(tData.u.frm.tIn), &(tData.u.frm.tOut), (UINT32)(-1));
        }
    }

    _H264_FUNC_LEAVE();


    return VENC_OK;
}

static VOID _H264EncMainLoop(VOID *pvArg)
{
    VENC_OBJECT_T *this = (VENC_OBJECT_T *)(*(UINT32 *)pvArg);
    UINT32 u4VEncCmd;
    BOOL fgRun = TRUE;

    _H264_FUNC_ENTER();

    ASSERT(this);

    while (fgRun)
    {
        if (VENC_OK == _VENC_ReceiveCmd(this, &u4VEncCmd))
        {
            _VENC_ProcessCmd(this, u4VEncCmd, &fgRun);
        }

        if (VENC_ENC_STATE_ENC == this->eEncState)
        {
            _H264EncRoutine(this);
        }
    }

    _H264_FUNC_LEAVE();

    x_thread_exit();
}


VENC_OBJECT_T *_prH264Inst;
static VENC_OBJECT_T *_H264GetInst(void)
{
    static VENC_OBJECT_T _h264Object;

    _prH264Inst = &_h264Object;
    UNUSED(_prH264Inst);

    _H264_FUNC_ENTER();

    return &_h264Object;
}

static UINT32 _H264FreeInst(VENC_OBJECT_T *this)
{
    _H264_FUNC_ENTER();
    this->fgUsed          = !VENC_DEV_USED;
    return VENC_OK;
}

static VENC_HANDLE _H264CreateMMRParam(VENC_PROFILE_T eProfile)
{
    static H264_MMR_PARAM_T _mmrParam;
    H264_MMR_PARAM_T * pMMR = &_mmrParam;
    _H264_FUNC_ENTER();

    x_memset((void *)pMMR, 0, sizeof(H264_MMR_PARAM_T));

    switch(eProfile)
    {
    default:
        ASSERT(0);
    case VENC_PROFILE_VPHONE:
        pMMR->mmr_period_i_frm_10     = 0;
        pMMR->mmr_period_idr_frm_10   = 0;
        pMMR->mmr_num_of_b_frm_2      = 0;
        pMMR->mmr_p_frm_q_limiter_8   = 0x54;
        pMMR->mmr_b_frm_q_limiter_8   = 0x32;
        pMMR->mmr_h264_profile_8      = 0;  //baseline

        pMMR->mmr_rc_adaptive_fps     = 1;
        pMMR->mmr_rc_adaptive_tbr     = 1;
        break;

    case VENC_PROFILE_PVR:
        pMMR->mmr_period_i_frm_10     = 10;
        pMMR->mmr_period_idr_frm_10   = 20;
        pMMR->mmr_num_of_b_frm_2      = 2;
        pMMR->mmr_p_frm_q_limiter_8   = 6;
        pMMR->mmr_b_frm_q_limiter_8   = 8;
        pMMR->mmr_h264_profile_8      = 1;  //main

        pMMR->coded_picture_type      = 1; //CODE_FRAME
        pMMR->picture_display_type    = 3; //TOP_BOTTOM_FIELD

        pMMR->mmr_rc_adaptive_fps     = 0;
        pMMR->mmr_rc_adaptive_tbr     = 0;
        break;

    case VENC_PROFILE_MS:
        pMMR->mmr_period_i_frm_10     = 30;
        pMMR->mmr_period_idr_frm_10   = 60;
        pMMR->mmr_num_of_b_frm_2      = 0;
        pMMR->mmr_p_frm_q_limiter_8   = 6;
        pMMR->mmr_b_frm_q_limiter_8   = 8;
        pMMR->mmr_h264_profile_8      = 1;  //main

        pMMR->coded_picture_type      = 1; //CODE_FRAME
        pMMR->picture_display_type    = 3; //TOP_BOTTOM_FIELD
        pMMR->mmr_enable_vui_support_1 = 1;
        pMMR->write_pic_timing = 1;
        pMMR->vui_pic_struct_present_flag = 1;

        pMMR->mmr_rc_adaptive_fps     = 0;
        pMMR->mmr_rc_adaptive_tbr     = 0;
        break;

    case VENC_PROFILE_SLT:
        pMMR->mmr_period_i_frm_10     = 10;
        pMMR->mmr_period_idr_frm_10   = 20;
        pMMR->mmr_num_of_b_frm_2      = 2;
        pMMR->mmr_p_frm_q_limiter_8   = 6;
        pMMR->mmr_b_frm_q_limiter_8   = 8;
        pMMR->mmr_h264_profile_8      = 1;  //main

        pMMR->coded_picture_type      = 1; //CODE_FRAME
        pMMR->picture_display_type    = 3; //TOP_BOTTOM_FIELD

        pMMR->mmr_rc_adaptive_fps     = 0;
        pMMR->mmr_rc_adaptive_tbr     = 0;
        break;

    case VENC_PROFILE_EMU:
        pMMR->mmr_rc_adaptive_fps     = 1;
        pMMR->mmr_rc_adaptive_tbr     = 1;
        break;
    }

    pMMR->mmr_h264_level_6            = 31; // 3.1

    pMMR->mmr_en_b_interdirect_1      = 1;
    pMMR->mmr_en_b_inter16x16_1       = 1;

    pMMR->mmr_dram_page_size_div256_7 = VIDEOCODEC_HAL_DEF_DRAM_PAGE_SIZE / 256;
    pMMR->mmr_use_mbaff_1             = 1;
    pMMR->mmr_use_cabac_1             = 1;
    pMMR->mmr_quant_i_frm_6           = 26;
    pMMR->mmr_quant_p_frm_6           = 26;
    pMMR->mmr_quant_b_frm_6           = 29;
    pMMR->mmr_en_p_interskip_1        = 1;
    pMMR->mmr_en_p_inter16x16_1       = 1;
    pMMR->mmr_en_p_inter16x08_1       = 1;
    pMMR->mmr_en_p_inter08x16_1       = 1;
    pMMR->mmr_en_p_inter08x08_1       = 1;
    pMMR->mmr_p_sr_h_div16_3          = 80 / 16;
    pMMR->mmr_p_sr_v_div16_2          = 48 / 16;
    pMMR->mmr_b_sr_h_div16_2          = 48 / 16;
    pMMR->mmr_b_sr_v_div16_2          = 32 / 16;
    pMMR->mmr_use_coarse_search_1     = 1;
    pMMR->mmr_use_rc_1                = 0;
    pMMR->mmr_rc_use_cbr_1            = 1;
    pMMR->mmr_rc_use_init_qp_1        = 0;
    pMMR->mmr_rc_target_bit_rate_14   = 3000;

    pMMR->use_stuff_byte              = 0;


    return VENC_OBJECT_TO_HANDLE(&_mmrParam);

}

static UINT32 _H264FreeMMRParam(H264_MMR_PARAM_T *thisMMR)
{
    _H264_FUNC_ENTER();

    ASSERT(thisMMR);
    UNUSED(thisMMR);

    return VENC_OK;
}

static UINT32 _H264SetMMRParam(VENC_OBJECT_T *this, UINT32 type, VOID *pvParam)
{
    H264_MMR_PARAM_T *pMMR = NULL;

    _H264_FUNC_ENTER();

    ASSERT(this);

    if (VENC_NULL_HANDLE == this->hParam)
    {
        return VENC_INV_HANDLE;
    }

    pMMR = VENC_HANDLE_TO_OBJECT(this->hParam, H264_MMR_PARAM_T);

    switch(type)
    {
    case VENC_H264_PRAM_PROFILE_LEVEL:
    {
        VENC_H264_PRAM_PL_T *pPL = (VENC_H264_PRAM_PL_T *)pvParam;
        pMMR->mmr_h264_profile_8 = (UINT8)pPL->u2Profile;
        pMMR->mmr_h264_level_6   = (UINT8)pPL->u2Level;
    }
        break;
    #ifdef VENC_EMU
    case H264_EMU_PARM:
    {
        UINT32 size;
        size = _H264_MEMBER_OFFSET(H264_MMR_PARAM_T, mmr_end);
        x_memcpy((VOID *)pMMR, pvParam, size);
    }
        break;
    #endif
    default:
        return VENC_NOT_IMPL;
    }

    UNUSED(pMMR);
    UNUSED(pvParam);

    return VENC_OK;
}

static UINT32 _H264GetMMRParam(VENC_OBJECT_T *this, UINT32 type, VOID *pvParam)
{
    H264_MMR_PARAM_T *pMMR = NULL;

    _H264_FUNC_ENTER();

    ASSERT(this);

    if (VENC_NULL_HANDLE == this->hParam)
    {
        return VENC_INV_HANDLE;
    }

    pMMR = VENC_HANDLE_TO_OBJECT(this->hParam, H264_MMR_PARAM_T);

    switch(type)
    {
    case VENC_H264_PRAM_PROFILE_LEVEL:
    {
        VENC_H264_PRAM_PL_T *pPL = (VENC_H264_PRAM_PL_T *)pvParam;
        pPL->u2Profile = pMMR->mmr_h264_profile_8;
        pPL->u2Level   = pMMR->mmr_h264_level_6;
    }
        break;
    default:
        return VENC_NOT_IMPL;
    }

    UNUSED(pMMR);
    UNUSED(pvParam);

    return VENC_OK;
}


static VENC_HANDLE _H264CreateCtrl(VENC_PROFILE_T eProfile)
{
    static H264_CTRL_T _ctrl;

    _H264_FUNC_ENTER();

    x_memset((void *)(&_ctrl), 0, sizeof(H264_CTRL_T));

    switch(eProfile)
    {
    default:
        break;

    case VENC_PROFILE_MS:
        break;

    case VENC_PROFILE_PVR:
        _ctrl.u1RepeatHeader = 2;
        break;

    case VENC_PROFILE_SLT:
        break;

    case VENC_PROFILE_VPHONE:
        break;

    case VENC_PROFILE_EMU:
        _ctrl.fgEMUDirectMode = TRUE;
        break;
    }

    _ctrl.eRezCmdState = VENC_STATE_STOP;
    _ctrl.eRezFrmState = H264_REZ_STATE_IDLE;

    return VENC_OBJECT_TO_HANDLE(&_ctrl);

}


static UINT32 _H264FreeCtrl(VENC_OBJECT_T *this)
{
    H264_CTRL_T *pCtrl;

    _H264_FUNC_ENTER();

    pCtrl = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);

    ASSERT(pCtrl);
    UNUSED(pCtrl);

    if (NULL_HANDLE != pCtrl->hRezThread)
    {
        _VEncInitSyncCmdFlag(this);
        _H264RezSendCmd(this, VENC_CMD_EXIT_SYNC);
        _VEncWaitSyncCmdDone(this);
        pCtrl->hRezThread = NULL_HANDLE;
    }

    if (pCtrl->ptCoarseBuffMgr)
    {
        VENC_BUFMGR_OBJ_DELETE(h264Rez, (VENC_BUFMGR_T(h264Rez) *)(pCtrl->ptCoarseBuffMgr));
        pCtrl->ptCoarseBuffMgr = NULL;
    }

    if (pCtrl->ptRezObj)
    {
        VencDeleteRezObject(pCtrl->ptRezObj);
        pCtrl->ptRezObj = NULL;
    }

    if (NULL_HANDLE != pCtrl->hRezCmdQ)
    {
        VERIFY(x_msg_q_delete(pCtrl->hRezCmdQ) == OSR_OK);
        pCtrl->hRezCmdQ = NULL_HANDLE;
    }

    if (NULL_HANDLE != pCtrl->hRezFrmQ)
    {
        VERIFY(x_msg_q_delete(pCtrl->hRezFrmQ) == OSR_OK);
        pCtrl->hRezCmdQ = NULL_HANDLE;
    }

    return VENC_OK;
}

static UINT32 _H264SetCtrl(VENC_OBJECT_T *this, UINT32 type, VOID *pvParam)
{
    H264_CTRL_T *pCtrl = NULL;

    _H264_FUNC_ENTER();

    ASSERT(this);

    if (VENC_NULL_HANDLE == this->hCtrl)
    {
        return VENC_INV_HANDLE;
    }

    pCtrl = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);

    switch(type)
    {
    case 0x101: //break;
        break;
    default:
        return VENC_NOT_IMPL;
    }

    UNUSED(pCtrl);
    UNUSED(pvParam);

    return VENC_OK;
}

static UINT32 _H264GetCtrl(VENC_OBJECT_T *this, UINT32 type, VOID *pvParam)
{
    H264_CTRL_T *pCtrl = NULL;

    _H264_FUNC_ENTER();

    ASSERT(this);

    if (VENC_NULL_HANDLE == this->hCtrl)
    {
        return VENC_INV_HANDLE;
    }

    pCtrl = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);

    switch(type)
    {
    case 0x101: //break;
        break;
    case VENC_CTRL_INT_GET_BS_INFO:
    {
        VENC_CTRL_BS_INFO *pBsInfo = (VENC_CTRL_BS_INFO *)pvParam;
        pBsInfo->u4Addr = pCtrl->u4BsSa;
        pBsInfo->u4Size = pCtrl->u4BsSz;
        pBsInfo->u4Wp   = pCtrl->u4NextCpSa;
    }
        break;
    default:
        return VENC_NOT_IMPL;
    }

    UNUSED(pCtrl);
    UNUSED(pvParam);

    return VENC_OK;
}


static UINT32 _H264Init(VENC_OBJECT_T *this)
{
    H264_CTRL_T * pCtrl = NULL;
    //H264_MMR_PARAM_T * pMMR;
    UINT32 u4Ret = VENC_OK;

    _H264_FUNC_ENTER();

    ASSERT(this);

    if (!this)
    {
        return VENC_INV_ARG;
    }

    pCtrl = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);
    //pMMR  = VENC_HANDLE_TO_OBJECT(this->hParam, H264_MMR_PARAM_T);

    if (!pCtrl)
    {
        return VENC_INV_HANDLE;
    }

    do
    {
        //mainloop for async encode
        this->pfnEncMainLoop = _H264EncMainLoop;

        //variable init
        _H264VarInit(this);

        //coarse res.
        _H264CalcCoarseRes(this);

        //mmr init
        _H264MMRInit(this);

        //resizer
        if (pCtrl->fgCoarse)
        {
            u4Ret = _H264RezInit(this, pCtrl);
            VENC_CHK_OK_BREAK(u4Ret);
        }

        //buffer
        u4Ret = _H264BuffInit(this, pCtrl);
        VENC_CHK_OK_BREAK(u4Ret);

        //mid protection
        _H264MidInit(this);

        //hw init
        _VENC_H264_HalPreStart(this);

    }while (0);


    _H264_FUNC_LEAVE();

    return u4Ret;
}


static UINT32 _H264Run(VENC_OBJECT_T *this)
{
    H264_CTRL_T * pCtrl = NULL;
    UINT32 u4Ret;
    _H264_FUNC_ENTER();

    ASSERT(this);
    if (!this)
    {
        return VENC_INV_ARG;
    }

    pCtrl = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);

    do
    {
        u4Ret = (VENC_STATE_STOP != pCtrl->eRezCmdState) ? VENC_INTERNAL_ERR : VENC_OK;
        VENC_CHK_OK_BREAK(u4Ret);

        pCtrl->fg1stHeader    = TRUE;
        pCtrl->u4FrefCSAddr   = 0;
        pCtrl->u4BrefCSAddr   = 0;
        pCtrl->u1BFrmIdx      = 0;
        pCtrl->u1BSeqIdx      = 0;
        pCtrl->u1RefSeqIdx    = 0;
        pCtrl->u2IDRFrmIdx    = 0;
        pCtrl->u2IFrmIdx      = 0;
        pCtrl->u4FrameNo      = 0;
        pCtrl->fgForceIDR     = FALSE;
        //if (VENC_SUPPORT_ASYNC_MODE(this->eSMode))
        if ((VENC_SUPPORT_ASYNC_MODE(this->eSMode)) && pCtrl->fgCoarse)
        {
            ASSERT((NULL_HANDLE != pCtrl->hRezFrmQ) && (NULL_HANDLE != pCtrl->hRezCmdQ));
            _VEncInitSyncCmdFlag(this);
            _H264RezSendCmd(this,VENC_CMD_START_SYNC);
            //wait SYNC cmd done
            //VERIFY(x_sema_lock(pObj->hEncSyncSema, X_SEMA_OPTION_WAIT) == OSR_OK);
            _VEncWaitSyncCmdDone(this);
        }

        _VENC_H264_HalStart(this);

    }while (0);

    _H264_FUNC_LEAVE();

    return u4Ret;
}

static UINT32 _H264Stop(VENC_OBJECT_T *this)
{
    _H264_FUNC_ENTER();

    ASSERT(this);
    if (!this)
    {
        return VENC_INV_ARG;
    }

    _VENC_H264_HalStop(this);

    _H264_FUNC_LEAVE();

    return VENC_OK;
}


static UINT32 _H264Close(VENC_OBJECT_T *this)
{
    _H264_FUNC_ENTER();

    ASSERT(this);

    if (!this)
    {
        return VENC_INV_ARG;
    }

    //free resource
    if (VENC_NULL_HANDLE != this->hParam)
    {
        _H264FreeMMRParam((H264_MMR_PARAM_T *)(this->hParam));
    }

    if (VENC_NULL_HANDLE != this->hCtrl)
    {
        _H264FreeCtrl(this);
    }

    _H264FreeInst(this);

    _H264_FUNC_LEAVE();

    return VENC_OK;
}

static UINT32  _H264EncSync (VENC_OBJECT_T *this, VENC_IN_PIC_T *pIn, VENC_OUT_PIC_T *pOut, VOID* pvPram)
{
    UINT32 u4Ret = VENC_OK;
    UINT32 u4CosAddr = 0;
    H264_CTRL_T *pCtrl;
    UINT32 u4OutAddr, u4OutSize, u4Addr, u4Size;

    #ifdef VENC_EMU
    H264_EMU_OUT_PIC_T *ptEmuOut = (H264_EMU_OUT_PIC_T *)pOut;
    #endif

    _H264_FUNC_ENTER();

    ASSERT(this);

    if (!this)
    {
        return VENC_INV_ARG;
    }

    pCtrl = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);

    ASSERT(pCtrl->u1BFrameNo == 0);
    if (pCtrl->u1BFrameNo > 0)
    {
        return VENC_FAIL;
    }

    pOut->u4Type = 0;
    pOut->u4Pts  = pIn->u4Pts;
    u4Addr = 0;
    u4Size = 0;

    //encode seq. 1.RRRRRR
    if (pCtrl->fgCoarse)
    {
        //get coarse buffer
        ASSERT(pCtrl->ptCoarseBuffMgr && pCtrl->ptCoarseBuffMgr->Get);
        u4Ret = pCtrl->ptCoarseBuffMgr->Get(pCtrl->ptCoarseBuffMgr, (VOID *)(&u4CosAddr), VENC_BUFMGR_WAIT);
        ASSERT(u4Ret == VENC_BUFMGR_OK);
        _H264_DBG("_VENC<H264>::rez frame (src,tag) => (0x%08x, 0x%08x)\n", pIn->u4YAddr, u4CosAddr);
        ASSERT(pCtrl->ptRezObj);
        pCtrl->ptRezObj->Resize(pCtrl->ptRezObj, pIn->u4YAddr, u4CosAddr);
    }

    //INIT
    if (pCtrl->fg1stHeader)
    {
        //write sps
        _H264Gen1stSPS_HW(pCtrl, &u4OutAddr, &u4OutSize);
        u4Addr = u4OutAddr;
        u4Size += u4OutSize;
        #ifdef VENC_EMU
        pOut->u4Type |= VENC_H264_PIC_TYPE_SPS;
        ptEmuOut->u4SPSAddr = u4OutAddr;
        ptEmuOut->u4SPSSize = u4OutSize;
        #endif

        pCtrl->fg1stHeader  = FALSE;
        pCtrl->fgSkipHeaderOnce = TRUE;

        //write pps
        _H264GenPPS_HW(pCtrl, &u4OutAddr, &u4OutSize);
        u4Size += u4OutSize;

        #ifdef VENC_EMU
        pOut->u4Type |= VENC_H264_PIC_TYPE_PPS;
        ptEmuOut->u4PPSAddr = u4OutAddr;
        ptEmuOut->u4PPSSize = u4OutSize;
        #endif

    }

    //encode seq. 1.IPPPPP

    //gen ref frame first
    pCtrl->u4BrefCSAddr = u4CosAddr;
    _H264EncRefTypeAnalyse(pCtrl);
    if (VENC_H264_PIC_TYPE_IDR == pCtrl->eType)
    {
        //encoder header
        if (pCtrl->u1RepeatHeader > 0)
        {
            //write sps
            if (pCtrl->fgSkipHeaderOnce)
            {
                pCtrl->fgSkipHeaderOnce = FALSE;
            }
            else
            {
                _H264GenSPS_HW(pCtrl, &u4OutAddr, &u4OutSize);
                u4Addr = u4OutAddr;
                u4Size += u4OutSize;

                #ifdef VENC_EMU
                pOut->u4Type |= VENC_H264_PIC_TYPE_SPS;
                ptEmuOut->u4SPSAddr = u4OutAddr;
                ptEmuOut->u4SPSSize = u4OutSize;
                #endif

                //write pps
                _H264GenPPS_HW(pCtrl, &u4OutAddr, &u4OutSize);
                u4Size += u4OutSize;

                #ifdef VENC_EMU
                pOut->u4Type |= VENC_H264_PIC_TYPE_PPS;
                ptEmuOut->u4PPSAddr = u4OutAddr;
                ptEmuOut->u4PPSSize = u4OutSize;
                #endif

            }

            //write sei
            if (pCtrl->fgWriteSEI)
            {
                LOG(0, "not support SEI at sync mode\n");
                ASSERT(0);
                _H264GenSEI_SW(this, &u4OutAddr, &u4OutSize);
            }

            if (pCtrl->u1RepeatHeader == 1)
            {
                pCtrl->u1RepeatHeader = 0;
            }
        }
    }

    _H264GenFRM_HW(this, pCtrl, pIn->u4YAddr, pIn->u4CAddr, u4CosAddr, &u4OutAddr, &u4OutSize);
    if (!u4Addr)
    {
        u4Addr = u4OutAddr;
    }
    u4Size += u4OutSize;

    #ifdef VENC_EMU
    ptEmuOut->u4FRMAddr   = u4OutAddr;
    ptEmuOut->u4FRMSize   = u4OutSize;
    #endif

    //release f ref corase buffer
    if (pCtrl->fgCoarse && pCtrl->u4FrefCSAddr)
    {
        pCtrl->ptCoarseBuffMgr->Free(pCtrl->ptCoarseBuffMgr, (VOID*)(&(pCtrl->u4FrefCSAddr)));
    }

    //b ref -> f ref
    pCtrl->u4FrefCSAddr = pCtrl->u4BrefCSAddr;

    //pOut->u4Type   = pCtrl->eType;
    pOut->u4Type  |= (UINT32)pCtrl->eType;
    pOut->u4RStart = pCtrl->u4BsSa;
    pOut->u4REnd   = pCtrl->u4BsEa;
    pOut->u4Addr   = u4Addr;
    pOut->u4Size   = u4Size;

    _H264_FUNC_LEAVE();

    return u4Ret;
}



static UINT32 _H264EncAsync (VENC_OBJECT_T *this, VENC_IN_PIC_T *pIn, VENC_OUT_PIC_T *pOut, VOID* pvPram)
{
    UINT32 u4Ret = VENC_OK;
    VENC_FRMQ_DATA_T rData;
    H264_CTRL_T *pCtrl;

    _H264_FUNC_ENTER();

    ASSERT(this);

    if (!this)
    {
        return VENC_INV_ARG;
    }

    pCtrl = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);


    if (pvPram)
    {
        //TODO
        ASSERT(0);
    }
    else
    {
        rData.eType = VENC_FRMQ_TYPE_FRM;
    }
    rData.u.frm.tIn   = *pIn;

    if (pOut)
    {
        rData.u.frm.tOut  = *pOut;
    }
    else
    {
        rData.u.frm.tOut.u4Addr = 0;
    }

    if (pCtrl->fgCoarse)
    {
        x_memset((void *)&(rData.u.frm.tPram), 0, sizeof(VENC_PIC_PRAM_T));
        _H264RezSendFrm(this, &rData);
    }
    else
    {
        _VENC_SendFrm(this, &rData);
    }

    _H264_FUNC_LEAVE();

    return u4Ret;
}

static UINT32 _H264SetRtParamSync (VENC_OBJECT_T *this, VENC_RT_PARM_T *ptParm)
{
    UINT32 u4Ret = VENC_OK;
    H264_CTRL_T *pCtrl;

    _H264_FUNC_ENTER();

    ASSERT(this);

    if (!this || !ptParm)
    {
        return VENC_INV_ARG;
    }

    if (VENC_NULL_HANDLE == this->hCtrl)
    {
        return VENC_INV_HANDLE;
    }

    pCtrl = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);

    do
    {
        BOOL fgResetHW = FALSE;
        if (ptParm->u4Flag & VENC_RT_PARM_FLAG_RES)
        {
            if (_H264_NEED_COARSE(this->u2Width, this->u2Height) ^ _H264_NEED_COARSE(ptParm->u2Width, ptParm->u2Height))
            {
                u4Ret = VENC_FAIL;
                break;
            }
            this->u2Width  = ptParm->u2Width;
            this->u2Height = ptParm->u2Height;
            this->u2Pitch  = ptParm->u2Pitch;
            if (pCtrl->fgCoarse)
            {
                _H264CalcCoarseRes(this);
                _H264RezInit(this, pCtrl);
            }
            fgResetHW = TRUE;
        }

        if (ptParm->u4Flag & VENC_RT_PARM_FLAG_BITRATE)
        {
            this->u4Bitrate = ptParm->u4Bitrate;
        }

        if (ptParm->u4Flag & VENC_RT_PARM_FLAG_FRAMERATE)
        {
            this->u1Framerate = ptParm->u1Framerate;
        }

        if (fgResetHW)
        {
            _H264HWReset(this);
        }
        else
        {
            _H264RtReset(this);
        }

    }while(0);

    _H264_FUNC_LEAVE();

    return u4Ret;
}

static UINT32 _H264SetRtParamAsync (VENC_OBJECT_T *this, VENC_RT_PARM_T *ptParm)
{
    UINT32 u4Ret = VENC_OK;
    H264_CTRL_T *pCtrl;
    VENC_FRMQ_DATA_T rData;

    _H264_FUNC_ENTER();

    ASSERT(this);

    if (!this || !ptParm)
    {
        return VENC_INV_ARG;
    }

    if (VENC_NULL_HANDLE == this->hCtrl)
    {
        return VENC_INV_HANDLE;
    }

    pCtrl = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);

    rData.eType = VENC_FRMQ_TYPE_RT_PARM;
    x_memcpy((VOID *)&(rData.u.tRTParm), (VOID *)ptParm, sizeof(VENC_RT_PARM_T));
    if (pCtrl->fgCoarse)
    {
        _H264RezSendFrm(this, &rData);
    }
    else
    {
        _VENC_SendFrm(this, &rData);
    }

    _H264_FUNC_LEAVE();

    return u4Ret;
}


static UINT32 _H264SetRtCtrlSync (VENC_OBJECT_T *this, VENC_RT_CTRL_T *ptParm)
{
    UINT32 u4Ret = VENC_OK;
    H264_CTRL_T *pCtrl;

    _H264_FUNC_ENTER();

    ASSERT(this);

    if (!this || !ptParm)
    {
        return VENC_INV_ARG;
    }

    if (VENC_NULL_HANDLE == this->hCtrl)
    {
        return VENC_INV_HANDLE;
    }

    pCtrl = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);

    if (ptParm->u4Flag & VENC_RT_CTRL_FLAG_INSERT_KEYFRAME)
    {
        _VEnc_H264_HalInsertKeyframeOnOff(1);
        pCtrl->u1RepeatHeader = 1;
        pCtrl->fgForceIDR = TRUE;
    }

    _H264_FUNC_LEAVE();

    return u4Ret;
}


static UINT32 _H264SetRtCtrlAsync (VENC_OBJECT_T *this, VENC_RT_CTRL_T *ptParm)
{
    UINT32 u4Ret = VENC_OK;
    H264_CTRL_T *pCtrl;
    VENC_FRMQ_DATA_T rData;

    _H264_FUNC_ENTER();

    ASSERT(this);

    if (!this || !ptParm)
    {
        return VENC_INV_ARG;
    }

    if (VENC_NULL_HANDLE == this->hCtrl)
    {
        return VENC_INV_HANDLE;
    }

    pCtrl = VENC_HANDLE_TO_OBJECT(this->hCtrl, H264_CTRL_T);

    rData.eType = VENC_FRMQ_TYPE_RT_CTRL;
    x_memcpy((VOID *)&(rData.u.tRTCtrl), (VOID *)ptParm, sizeof(VENC_RT_CTRL_T));

    if (pCtrl->fgCoarse)
    {
        _H264RezSendFrm(this, &rData);
    }
    else
    {
        _VENC_SendFrm(this, &rData);
    }

    _H264_FUNC_LEAVE();

    return u4Ret;
}



//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
VOID _H264DumpBst(UINT8 *pu1Buf, UINT32 u4BufSz)
{
    UINT32 i;

    Printf("size: %d\n", u4BufSz);
    for (i = 0; i < u4BufSz; i++)
    {
        if ((i & 0xf) == 0)
        {
            Printf("\n0x%08x: ", pu1Buf + i);
        }
        Printf("%02x ", pu1Buf[i]);
    }
    Printf("\n");
}


VENC_OBJECT_T *H264Open(void)
{
    VENC_OBJECT_T *this = _H264GetInst();

    _H264_FUNC_ENTER();

    if (!this)
    {
        return VENC_NULL_HANDLE;
    }

    if (VENC_DEV_USED == this->fgUsed)
    {
        return VENC_NULL_HANDLE;
    }

    x_memset((void*)this, 0, sizeof(VENC_OBJECT_T));
    x_strncpy((char*)(this->u1aName), "H246", 4);

    this->fgUsed          = VENC_DEV_USED;
    //FUNCTION
    this->CreateParam     = _H264CreateMMRParam;
    this->SetParam        = _H264SetMMRParam;
    this->GetParam        = _H264GetMMRParam;
    this->CreateCtrl      = _H264CreateCtrl;
    this->SetCtrl         = _H264SetCtrl;
    this->GetCtrl         = _H264GetCtrl;
    this->Init            = _H264Init;
    this->Run             = _H264Run;
    this->Stop            = _H264Stop;
    this->Close           = _H264Close;
    this->EncFrameSync    = _H264EncSync;
    this->EncFrameAsync   = _H264EncAsync;
    this->SetRtParamSync  = _H264SetRtParamSync;
    this->SetRtParamAsync = _H264SetRtParamAsync;
    this->SetRtCtrlSync   = _H264SetRtCtrlSync;
    this->SetRtCtrlAsync  = _H264SetRtCtrlAsync;

    _H264_FUNC_LEAVE();

    return (this);

}


