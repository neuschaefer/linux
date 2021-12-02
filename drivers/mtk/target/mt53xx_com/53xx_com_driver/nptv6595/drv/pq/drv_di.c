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
 * $RCSfile: drv_di.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/


////////////////////////////////////////////////////////////////////////////////
// Include files
////////////////////////////////////////////////////////////////////////////////

#include "drv_di.h"
#include "drv_di_int.h"
#include "drv_hdtv.h"
#include "drv_nr.h"
#include "drv_scaler.h"
#include "drv_scpos.h"
#include "drv_tdtv_drvif.h"
#include "fbm_drvif.h"
#include "mute_if.h"
#include "source_select.h"
#include "srm_drvif.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "di_debug.h"
#include "hw_ckgen.h"
#include "hw_di.h"
#include "hw_di_int.h"
#include "hw_nr.h"
#include "sv_const.h"
#include "vdo_misc.h"
#include "video_def.h"
#include "video_timing.h"
#include "x_ckgen.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_timer.h"
#include "x_util.h"
#include "x_hal_5381.h"
#ifdef CC_MT5395
#include "drv_display.h"
#include "drv_mjc.h"
#endif
#ifdef CC_FLIP_MIRROR_SUPPORT
#include "drv_display.h"
#endif
#ifdef CC_SUPPORT_VENC
#include "venc_drvif.h"
#include "venc_util_drvif.h"
#include "fbm_drvif.h"
#ifdef CC_ATV_PVR_SUPPORT
#include "drv_vbi.h"
#endif
#endif

////////////////////////////////////////////////////////////////////////////////
//  Macro & Define
////////////////////////////////////////////////////////////////////////////////
#define IS_OVERSAMPLE_OVER_720(u1Id)    ((bDrvVideoGetWidthOverSample(u1Id)) && (_arMDDiPrm[u1Id].u2Width > (720 + DECODER_ADD_WIDTH))) 

#define IS_PROG_DISPMODE(u1Id)      (_arMDDiPrm[u1Id].u1PSCANDisplay == DI_PROGRESSIVE_DISP) 

#define IS_INTERLACE(u1Id)      (_arMDDiPrm[u1Id].u1Interlace)

#define IS_PROGRESSIVE(u1Id)    (!_arMDDiPrm[u1Id].u1Interlace)

#define IS_420TO422(u1Id)       (_arMDDiPrm[u1Id].u1420Mode)
                               
#define IS_NR_ON(u1Id)          ((_arMDDiPrm[u1Id].u1NRDataMode != E_DI_NR_OFF) \
                                && (_arMDDiPrm[u1Id].u1NRDataMode != E_DI_NR_UNKNOWN))

#define IS_ECO_IC(void)          (((BSP_GetIcVersion() >= IC_VER_5365_AB) && IS_IC_5365()) \
                                || ((BSP_GetIcVersion() >= IC_VER_5395_AB) && IS_IC_5395()))

#define IS_PXLCLK_NOT_SUPPORT(u1Id)     (wDrvVideoGetHTotal(u1Id) * wDrvVideoGetVTotal(u1Id) \
                                * bDrvVideoGetRefreshRate(u1Id) > 74250000)

#define IS_DI_3D_FPI_MODE(u1Id) (_arMDDiPrm[u1Id].u4TdTvDIType == E_TDTV_DI_3D_FPI)

#define IS_DI_3D_I_MODE(u1Id) (_arMDDiPrm[u1Id].u4TdTvDIType == E_TDTV_DI_3D_I)

#define IS_DI_3D_P_MODE(u1Id) (_arMDDiPrm[u1Id].u4TdTvDIType == E_TDTV_DI_3D_P)

#define IS_DI_3D_DI_BYPASS(u1Id) (_arMDDiPrm[u1Id].u4TdTvDIType == E_TDTV_DI_BYPASS)

#if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
#define SUPPORT_3D_PR_WIDTH  (IS_ECO_IC() ? 960 : 720)
#endif

////////////////////////////////////////////////////////////////////////////////
//  external function
////////////////////////////////////////////////////////////////////////////////
EXTERN void vDrvDIEnableISR(UINT8 u1VdpId, UINT8 u1Enable);
EXTERN void vDrvDISetKDE(UINT8 u1VdpId, UINT16 u2N, UINT16 u2M);
EXTERN void vDrvDISetSDE(UINT8 u1VdpId, UINT8 bOnOff, UINT16 u2N, UINT16 u2M);
EXTERN void vDrvDISetODE(UINT8 u1VdpId, UINT8 bOnOff, UINT16 u2N, UINT16 u2M);
EXTERN void vDrvDISetBufferAddr(UINT8 u1VdpId, UINT32 u4AddrYMsb, UINT32 u4AddrYLsb, UINT32 u4AddrUMsb, 
                    UINT32 u4AddrULsb, UINT32 u4AddrVMsb, UINT32 u4AddrVLsb, UINT32 u4AddrMVFlg, UINT32 u4AddrLMFlg);
EXTERN void vDrvDISetBufferProtect(UINT8 u1VdpId, UINT32 u4Lo, UINT32 u4Hi, UINT32 u4FlgLo, UINT32 u4FlgHi);
EXTERN void vDrvDISetFrmNum(UINT8 u1VdpId, UINT8 u1YNum, UINT8 u1UNum, UINT8 u1VNum);
EXTERN void vDrvDISetDoubleBuffer(UINT8 u1VdpId, UINT8 u1OnOff);
EXTERN void vDrvDISetClkReset(UINT8 u1VdpId, UINT8 u1OnOff);
EXTERN void vDrvDISetFldPrediction(UINT8 u1VdpId, UINT8 u1OnOff);
EXTERN void vDrvDISetBgn(UINT8 u1VdpId, UINT8 u1S_V_Bgn, UINT8 u1K_H_Bgn, UINT8 u1K_V_Bgn);
EXTERN void vDrvDISRCWOutIOnOff(UINT8 u1VdpId, UINT8 u1OnOff);
EXTERN void vDrvDIProgModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff);
EXTERN void vDrvDISetFrcLine(UINT8 u1VdpId, UINT8 u1FrcLine);
EXTERN void vDrvDISetStdVMask(UINT8 u1VdpId, UINT8 u1OnOff);
EXTERN void vDrvDISetStdVBlank(UINT8 u1VdpId, UINT8 u1OnOff);
EXTERN void vDrvDISetAutoStop(UINT8 u1VdpId, UINT8 u1OnOff);
EXTERN void vDrvDISetClip(UINT8 u1VdpId, UINT16 u2HClip, UINT16 u2VClip);
EXTERN void vDrvDISetClock(UINT8 u1VdpId, UINT8 u1OnOff);
EXTERN void vDrvDISetErrDefusion(UINT8 u1VdpId, UINT8 u18bit);
EXTERN void vDrvDISetReSyncOnOff(UINT8 u1VdpId, UINT8 u1OnOff);

// temporary use extern directly, dsp should provide header file later
//#if defined(CC_SUPPORT_H264 ) || defined(ENABLE_AUDIO_RECORD_SUPPORT)
extern UINT32 _AUD_ReadPsrStc1(void);
//#endif

////////////////////////////////////////////////////////////////////////////////
//  interal enum
////////////////////////////////////////////////////////////////////////////////
typedef enum{
    E_AF_Y = 0x01,
    E_AF_C = 0x02,
    E_BF_Y = 0x04,    
    E_BF_C = 0x08,
    E_CF_Y = 0x10,
    E_CF_C = 0x20,
    E_DF_Y = 0x40,    
    E_DF_C = 0x80,
    E_ALL_FLD = 0XFF,
} E_DI_FLD_R_EN;

typedef enum{
    E_SRC_W = 0x01,
    E_SRC_R = 0x02,
    EN_LM_W = 0x04,    
    EN_LM_R = 0x08,
    EN_MV_W = 0x10,
    EN_MV_R = 0x20,
    E_DRAM_ALL_RW = 0X3F,
} E_DI_DRAM_RW_EN;

typedef enum
{
    E_TG_FREE_RUN,     
    E_TG_LOCK_SCALER,     
    E_TG_LOCK_INPUT_A_SRC,     
    E_TG_LOCK_INPUT_A_DRAM,
} E_TIME_GEN_MODE;

////////////////////////////////////////////////////////////////////////////////
//  Global variables enum
////////////////////////////////////////////////////////////////////////////////
VDP_MDDI_PRM_T _arMDDiPrm[VDP_NS];
HANDLE_T _hDIPQIsrSema;
static BOOL _bFifoShareMode = 0;
static BOOL _bUpdate = FALSE;
static UINT8 _u1Cnt = 0;
static UINT8 _u1CrcLog = 0;
static UINT16 _u2Vcnt;
static UINT8 _bTVMode = SRM_TV_MODE_TYPE_NORMAL;
static UINT32 _u4TdTvDIType[VDP_NS] = {E_TDTV_DI_2D, E_TDTV_DI_2D};

#define DTV_420TO422_HTOTAL 2200

#ifdef CC_FLIP_MIRROR_SUPPORT
UINT8 u1FlipMirrorConfig = DI_NORMAL_MODE;
#endif

#ifdef CC_DOWMBWMODE
UINT8 u1DowmBWMode = 0;
BOOL bNROnOff = 0;
UINT8 u1UpdateCnt = 0;
#endif

#ifdef CC_SUPPORT_H264
typedef enum
{
    MDDI_NORMAL_MODE,
    MDDI_H264_MODE,
    MDDI_OSD_BLEND_MODE
}DI_WRITE_MODE;

static BOOL _fgFrmRdy;
static BOOL _fgStopSendFrm;
static UINT32 _u4SkipCnt;
static UINT8 _u1YUVMode;
static VENC_BUFMGR_BASE_T *_ptDIVEncFrmBuffMgr;
static VENC_HANDLE _hDIVEncVenc;
static VENC_IN_PIC_T _tDIVEncIn;
static VENC_FRM_T _tDIVEncFrm;

#endif

#if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
extern UINT8 _bLRInverse;
#endif

////////////////////////////////////////////////////////////////////////////////
// Register table
////////////////////////////////////////////////////////////////////////////////
MDDI_REGTBL_T const CODE MDDI_MIF_INIT[] =
{
 //{ MDDI_KC_00,      0x00000000, 0x00003000}, // DTV00084276:Set Sub Path AUTO_TRICK_ON default value from 1  to 0 
 { MDDI_OC_02,      0x00000000, 0x08000000}, // For display mode, always VDE_ALIGN_HS_FALL = 0
 { MDDI_MULTI_03,   0x00001000, 0x00001000}, // DA_BLOCK_SIZE = 1
 { MDDI_REGTBL_END, 0x00000000, 0x00000000}
};

#ifdef LOG_MDDI_REG
#define DI_LOG_NUM 1024
#define DI_LOG_MSK (DI_LOG_NUM-1)
MDDI_REGTBL_T MDDI_REG_LOG[DI_LOG_NUM]; 
UINT32 u4LogReg = 0;

void MDDI_WRITE_FLD(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Fld)
{
    vRegWriteFldAlign((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Value, u4Fld);
    //Avoid interupt clear bit access
    if (u2Addr == 0x402C) return;
    MDDI_REG_LOG[u4LogReg].u2Addr = (u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM))|0x1;
    MDDI_REG_LOG[u4LogReg].u4Value = u4Value;
    MDDI_REG_LOG[u4LogReg].u4Mask = u4Fld;
    u4LogReg = (u4LogReg+1)&DI_LOG_MSK;
}

void MDDI_WRITE32_MSK(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Msk) 
{
    vRegWrite4BMsk((u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM)), u4Value, u4Msk);
    MDDI_REG_LOG[u4LogReg].u2Addr = (u2Addr + ((u1VdpId == VDP_1) ? 0 : MDDI_REG_NUM));
    MDDI_REG_LOG[u4LogReg].u4Value = u4Value;
    MDDI_REG_LOG[u4LogReg].u4Mask = u4Msk;
    u4LogReg = (u4LogReg+1)&DI_LOG_MSK;
}

void MDDI_PRINT_REG_LOG(void)
{
    UINT16 i, index, j = u4LogReg;

    Printf("\nIndex    u2Addr         u4Value        u4Mask\n");    
    
    for (i = j; i < (1024 + j); i++)
    {
        index = i&DI_LOG_MSK;
        if ((index&0xFF) == 0xFF)
        {
            x_thread_delay(20);
        }
        Printf("%4d    %6x        %8x        %08x\n", index,
            MDDI_REG_LOG[index].u2Addr&0xFFFC, MDDI_REG_LOG[index].u4Value,
            (MDDI_REG_LOG[u4LogReg].u2Addr & 0x1) 
            ? (Fld2Msk32(MDDI_REG_LOG[index].u4Mask))
            : MDDI_REG_LOG[index].u4Mask);
    }
}
#endif

void vDrvDILoadRegTbl(UINT8 u1VdpId, const MDDI_REGTBL_T * prRegTbl)
{
    while (prRegTbl->u2Addr != MDDI_REGTBL_END)
    {
    	MDDI_WRITE32_MSK(u1VdpId, prRegTbl->u2Addr, prRegTbl->u4Value, prRegTbl->u4Mask);
    	prRegTbl++;
    }
}

#ifdef CC_SUPPORT_H264
static void _vDrvDIRunH264Loop(void)
{    
    if (MDDI_READ_FLD(VDP_1, MDDI_H264_00, DA_WRITE_MODE) == 0) // normal mode
    {
        return;
    }

    if (VENC_NULL_HANDLE == _hDIVEncVenc)    
    {
        LOG(3, "%s: _hDIVEncVenc is NULL!!\n", __FUNCTION__);
        return;
    }

    if (!_ptDIVEncFrmBuffMgr)    
    {
        LOG(3, "%s: _ptDIVEncFrmBuffMgr is NULL!!\n", __FUNCTION__);
        return;
    }


    if (_fgStopSendFrm)    
    {
        vDrvDISetH264WriteMode(MDDI_NORMAL_MODE);
        _fgStopSendFrm = FALSE;
        return;
    }

    if (!IS_PROGRESSIVE(VDP_2)
        && (IS_INTERLACE(VDP_2) && (MDDI_READ_FLD(VDP_2, MDDI_KC_04, CF_BOTTOM)) != 0)) 
    {
        // donothing at bottom field
        return;        
    }

    if (_fgFrmRdy)
    {
        if (_u4SkipCnt < 2)
        {
            _u4SkipCnt++;
            return;
        }

#if 0//def CC_ATV_PVR_SUPPORT
        if (!VBI_PVRRetrieveCCData(&rPreImgrzInBufInfo.rCC_Data))
        {
            rPreImgrzInBufInfo.rCC_Data.u4Size = 0;
        }
        if (!VBI_PVRRetrieveAFDData(&rPreImgrzInBufInfo.rAFD_Data))
        {
            rPreImgrzInBufInfo.rAFD_Data.u4Size = 0;
        }
#endif
        if (VENC_OK != _VENC_EncFrameAsync(_hDIVEncVenc, &_tDIVEncIn, NULL))
        {
            LOG(0, "enc frame fail\n");
        }
    }


    if (VENC_BUFMGR_OK == _BUFMGR_OBJ_Get(_ptDIVEncFrmBuffMgr, (VOID *)&_tDIVEncFrm, VENC_BUFMGR_NOWAIT))
    {
        _fgFrmRdy = TRUE;
    }
    else
    {
        LOG(5, "no buffer!!!\n");
        _fgFrmRdy = FALSE;
    }

    if (_fgFrmRdy)
    {
//#ifdef ENABLE_AUDIO_RECORD_SUPPORT
        _tDIVEncIn.u4Pts = _AUD_ReadPsrStc1();
//#endif
        MDDI_WRITE_FLD(VDP_1, MDDI_H264_01, (_tDIVEncFrm.u4YAddr >> 4), DA_H264_ADDR_BASE_Y);
        MDDI_WRITE_FLD(VDP_1, MDDI_H264_02, (_tDIVEncFrm.u4UVAddr >> 4), DA_H264_ADDR_BASE_C);   
        MDDI_WRITE_FLD(VDP_2, MDDI_MULTI_00, SV_ON, EN_SRCW);
        _tDIVEncIn.u4YAddr = _tDIVEncFrm.u4YAddr;
        _tDIVEncIn.u4CAddr = _tDIVEncFrm.u4UVAddr;

    }
    else
    {
        MDDI_WRITE_FLD(VDP_2, MDDI_MULTI_00, SV_OFF, EN_SRCW);
    }
}

static void _vDrvDISupportH264Init(void)
{
    _ptDIVEncFrmBuffMgr = NULL;
    _hDIVEncVenc = VENC_NULL_HANDLE;
    _fgFrmRdy = FALSE;
    _fgStopSendFrm = FALSE;
    vDrvDISetDoubleBuffer(VDP_2, SV_OFF);
}

/**
 * @brief Set H.264 encoder handle
 */
void vDrvDISetVEncHandle(void *handle, void *ptFrmBuffMgr)
{
    _hDIVEncVenc = (VENC_HANDLE)handle;
    _ptDIVEncFrmBuffMgr = (VENC_BUFMGR_BASE_T *)ptFrmBuffMgr;
}

/**
 * @brief Disable send buffer to VENC
 **/
void vDrvDIStopSendBufToVENC(BOOL bFlag)
{
    _fgStopSendFrm = bFlag;
}

/**
* @brief Set H.264 write dram mode
* @param bMode
*/
void vDrvDISetH264WriteMode(UINT8 bMode)
{
    FBM_POOL_T *prFbmPool;
    UINT32 u4Low, u4High;

    switch (bMode)
    {
        case MDDI_NORMAL_MODE:
            vDrvDISetWritePrecision(VDP_2, ((_arMDDiPrm[VDP_2].u1BitRes == 8) ? 1 : 0));
            MDDI_WRITE_FLD(VDP_2, MDDI_SC_01, 0x8, SRCW_V_BGN);
            MDDI_WRITE_FLD(VDP_2, MDDI_SC_01, 0x9, KERNEL_V_BGN);
            MDDI_WRITE_FLD(VDP_1, MDDI_H264_00, 0x0, DA_WRITE_MODE);
            MDDI_WRITE_FLD(VDP_2, MDDI_MULTI_00, (!_arMDDiPrm[VDP_2].u1420Mode), M422);
            MDDI_WRITE_FLD(VDP_2, MDDI_MULTI_03, _u1YUVMode, DA_MODE_YUV);
            MDDI_WRITE_FLD(VDP_2, MDDI_MC_05, 0x1, DA_SNAKE_MAPPING);
            MDDI_WRITE_FLD(VDP_2, MDDI_MC_0F, _arMDDiPrm[VDP_2].rFbmPool.u4Addr >> 11, DA_WADDR_LO_LIMIT);
            MDDI_WRITE_FLD(VDP_2, MDDI_MULTI_00, SV_ON, EN_SRCW);
            MDDI_WRITE_FLD(VDP_2, MDDI_MC_0E, (_arMDDiPrm[VDP_2].rFbmPool.u4Addr + _arMDDiPrm[VDP_2].rFbmPool.u4Size) >> 11, DA_WADDR_HI_LIMIT);   
            break;

        case MDDI_H264_MODE:
            prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_VENC);
            u4Low = prFbmPool->u4Addr;
            u4High = u4Low + prFbmPool->u4Size;
            _u4SkipCnt = 0;
            _fgFrmRdy = FALSE;
            _fgStopSendFrm = FALSE;
            _tDIVEncIn.u4Pts = 0;
            _u1YUVMode = MDDI_READ_FLD(VDP_2, MDDI_MULTI_03, DA_MODE_YUV);
            vDrvDISetWritePrecision(VDP_2, 0x1); // force 8 bit
            MDDI_WRITE_FLD(VDP_2, MDDI_SC_01, 0xe, SRCW_V_BGN);
            MDDI_WRITE_FLD(VDP_2, MDDI_SC_01, 0xf, KERNEL_V_BGN);
            MDDI_WRITE_FLD(VDP_1, MDDI_H264_00, (IS_INTERLACE(VDP_2) ? 0 : 1), DA_H264_FRAME_MODE); // force field mode
            MDDI_WRITE_FLD(VDP_1, MDDI_H264_00, bMode, DA_WRITE_MODE); // set write mode
            MDDI_WRITE_FLD(VDP_2, MDDI_MULTI_00, 0x0, M422); // force 420 mode
            MDDI_WRITE_FLD(VDP_2, MDDI_MULTI_03, DI_YUV_MODE_YC, DA_MODE_YUV); // force Y/C mode
            MDDI_WRITE_FLD(VDP_2, MDDI_MC_05, 0x0, DA_SNAKE_MAPPING); // force raster scan mapping
            MDDI_WRITE_FLD(VDP_2, MDDI_MC_0F, u4Low >> 11, DA_WADDR_LO_LIMIT);
            MDDI_WRITE_FLD(VDP_2, MDDI_MC_0E, u4High >> 11, DA_WADDR_HI_LIMIT);    
            break;
            
        case MDDI_OSD_BLEND_MODE:
            break;
        default:
            break;
    }
}
#endif

static UINT8 _u1DrvGetNRDataMode(UINT8 u1VdpId)
{        
    UINT8 bVdpIdOther = (u1VdpId==VDP_1) ? VDP_2 : VDP_1;
    UINT32 u4Mode = _arMDDiPrm[u1VdpId].rFbmPool.u4Mode; 

    if (_arMDDiPrm[u1VdpId].u1DebugNRDataMode != E_DI_NR_UNKNOWN)        
    {
        if (_arMDDiPrm[bVdpIdOther].u1DebugNRDataMode != E_DI_NR_UNKNOWN)
        {
            _arMDDiPrm[bVdpIdOther].u1DebugNRDataMode = E_DI_NR_UNKNOWN;
        }
        
        return _arMDDiPrm[u1VdpId].u1DebugNRDataMode;
    }

    if (IS_DI_3D_FPI_MODE(u1VdpId) || IS_DI_3D_I_MODE(u1VdpId))
    {
        return  E_DI_NR_OFF;
    }
    
    if (u4Mode & FBM_POOL_MODE_MDDI_NR_OFF)
    {
        return E_DI_NR_OFF;
    }
    else 
    {
        if (bIsScalerInput444(u1VdpId))
        {
            return E_DI_NR_RGB;
        }
        else
        {
            if (u4Mode & FBM_POOL_MODE_MDDI_NR_Y_C)
            {
                return IS_420TO422(u1VdpId) ? E_DI_NR_YC_420 : E_DI_NR_YC_422;
            }
            else if (u4Mode & FBM_POOL_MODE_MDDI_NR_Y_ONLY)
            {
                return E_DI_NR_Y_ONLY;
            }
            else
            {
                return E_DI_NR_UNKNOWN;
            }
        }        
    }
}

static UINT8 _u1DrvDIGetDataMode(UINT8 u1VdpId)
{
    UINT8 u1DataMode, u1PSCANDisplay, u1NROnOff;
    UINT8 u1IsPScanFlipMirror;
    
    if (u1VdpId >= VDP_NS)
    {
        return E_DI_UNKNOWN;
    }
    
    if (_arMDDiPrm[u1VdpId].u1DebugDataMode != E_DI_UNKNOWN)
    {
    	return _arMDDiPrm[u1VdpId].u1DebugDataMode;
    }
        
    u1NROnOff = IS_NR_ON(u1VdpId);
    u1PSCANDisplay = _arMDDiPrm[u1VdpId].u1PSCANDisplay;
    #ifdef CC_FLIP_MIRROR_SUPPORT
    u1IsPScanFlipMirror = (u4GetFlipMirrorModule(u1VdpId)==FLIP_BY_PSCAN) ? 1 : 0;
    #else
    u1IsPScanFlipMirror = 0;
    #endif

    switch (_arMDDiPrm[u1VdpId].rFbmPool.u4Mode & 0x00070000)
    {
    	case FBM_POOL_MODE_MDDI_FULL:
            if(u1IsPScanFlipMirror)
            {
                u1DataMode = E_DI_FLIP_MIRROR_1W4R;
            }
            else  if (IS_DI_3D_FPI_MODE(u1VdpId) || IS_DI_3D_I_MODE(u1VdpId))
            {
                u1DataMode = E_DI_YCBOB_0W0R;
            }
            else
            {
                u1DataMode = u1PSCANDisplay ? E_DI_FULL_1W4R : E_DI_FULL_1W3R;
            }
            break;
    	case FBM_POOL_MODE_MDDI_BOB:
            if(u1IsPScanFlipMirror)
            {
                u1DataMode =  E_DI_FLIP_MIRROR_YCBOB_1W1R;
            }
            else
            {
                u1DataMode = (u1PSCANDisplay || u1NROnOff) ? E_DI_YCBOB_1W1R : E_DI_YCBOB_0W0R;
            }
            break;
    	case FBM_POOL_MODE_MDDI_CBOB:
            if(u1IsPScanFlipMirror)
            {
                u1DataMode = E_DI_FLIP_MIRROR_CBOB_1W4R;
            }
            else  if (IS_DI_3D_FPI_MODE(u1VdpId) || IS_DI_3D_I_MODE(u1VdpId))
            {
                u1DataMode = E_DI_YCBOB_0W0R;
            }
            else
            {
                u1DataMode = u1PSCANDisplay ? E_DI_CBOB_1W4R : E_DI_CBOB_1W3R;
            }
            break;
    	default:
            u1DataMode = (u1NROnOff) ? E_DI_YCBOB_1W1R : E_DI_YCBOB_0W0R;
            break;
    }

    return u1DataMode;
}

static UINT8 _u1DrvDIGetDispmode(UINT8 u1VdpId)
{
    if (_arMDDiPrm[u1VdpId].u1DebugPSCANDisplay != DI_DISP_UNKNOWN)
    {
        if (_arMDDiPrm[u1VdpId].u1DebugPSCANDisplay) // check progressive/interlace dispmode again
        {
            _arMDDiPrm[u1VdpId].u1DebugPSCANDisplay = (IS_INTERLACE(u1VdpId) ? DI_INTERLACE_DISP : DI_PROGRESSIVE_DISP);
        }        
        
        return _arMDDiPrm[u1VdpId].u1DebugPSCANDisplay;
    }
    
    if (_arMDDiPrm[u1VdpId].rFbmPool.u4Mode & FBM_POOL_MODE_MDDI_DISP)
    {
        return (IS_INTERLACE(u1VdpId) ? DI_INTERLACE_DISP : DI_PROGRESSIVE_DISP);
    }
    else
    {
        return DI_DISP_OFF;
    }
}

static UINT8 _u1DrvDI420T0422En(UINT8 u1VdpId)
{
    if ((bGetVideoDecType(u1VdpId) == SV_VD_MPEGSD) || (bGetVideoDecType(u1VdpId) == SV_VD_MPEGHD))
    {
        //1. pixel clock larger than 720p60 has bandwidth issue
        //2. Progressive display mode has problem when dot by dot or even more small size when resolution < 720p
        //3. do no 420 -> 422 when 5365
        if (((IS_PXLCLK_NOT_SUPPORT(u1VdpId) || (wDrvVideoGetHTotal(u1VdpId) >= DTV_420TO422_HTOTAL)) && (IS_PROGRESSIVE(u1VdpId)))
        || (IS_PROG_DISPMODE(u1VdpId))
#ifdef CC_SCPOS_3DTV_SUPPORT
        || (IS_MPEG_PROG(u1VdpId) && (u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF))
#endif
        )
        {
            return SV_FALSE;
        }
        else
        {
            return SV_TRUE;
        }    
    }
    else
    {
        return SV_FALSE;
    }
}

static UINT8 _u1DrvDIGetSrcSel(UINT8 u1VdpId)
{
    // u1AF_SRC_SEL = 1 for 420 dram mode
    // u1AF_SRC_SEL = 0 for else
    #if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
    if(IS_SUPPORT_3D_PR(u1VdpId))
    {
        return 1;
    }
    else
    #endif
    {
        return ((IS_MPEG_PROG(u1VdpId) && (!IS_PROG_DISPMODE(u1VdpId))) ? 1 : 0);
    }

}

static void _vDrvDISetFrameNum(UINT8 u1VdpId)
{
    UINT8 u1YFieldNum, u1CFieldNum;
    
    switch (_arMDDiPrm[u1VdpId].u1DataMode)
    {
        case E_DI_FULL_1W3R:
            u1YFieldNum = 4; 
            u1CFieldNum = 4;
            break;
        case E_DI_FULL_1W4R:
            u1YFieldNum = 5; 
            u1CFieldNum = 5;
            break;
        case E_DI_CBOB_1W3R:
            u1YFieldNum = 4;
            u1CFieldNum = 3; 
            break;
        case E_DI_CBOB_1W4R:
            u1YFieldNum = 5; 
            u1CFieldNum = 4; 
            break;
        case E_DI_YCBOB_0W0R:
            u1YFieldNum = 0; 
            u1CFieldNum = 0;
            break;
        case E_DI_YCBOB_1W1R:
            u1YFieldNum = 2; 
            u1CFieldNum = 2;
            break;
        case E_DI_FLIP_MIRROR_1W4R:
            u1YFieldNum = 6; 
            u1CFieldNum = 6;
            break;
        case E_DI_FLIP_MIRROR_CBOB_1W4R:
            u1YFieldNum = 6; 
            u1CFieldNum = 5;
            break;
        case E_DI_FLIP_MIRROR_YCBOB_1W1R:
            u1YFieldNum = 3; 
            u1CFieldNum = 3;
            break;
        default:
            u1YFieldNum = 0;
            u1CFieldNum = 0;
            break;
    }

    if ((!IS_DI_3D_P_MODE(u1VdpId)) && IS_PROGRESSIVE(u1VdpId) 
        && (!IS_PROG_DISPMODE(u1VdpId)) && IS_NR_ON(u1VdpId)) // progressive with NR on
    {
        u1YFieldNum = u1YFieldNum ? (u1YFieldNum - 1) : u1YFieldNum;
        u1CFieldNum = u1CFieldNum ? (u1CFieldNum - 1) : u1CFieldNum;
    }

    //Set the number of frame buffer
    u1YFieldNum = u1YFieldNum ? (u1YFieldNum - 1) : 0;
    u1CFieldNum = u1CFieldNum ? (u1CFieldNum - 1) : 0;
    vDrvDISetFrmNum(u1VdpId, u1YFieldNum, u1CFieldNum, u1CFieldNum);
}

static void _vDrvDICalMsbPageNum(UINT8 u1VdpId, UINT16 u2Width, UINT16 u2Height, UINT8 u1YNum, UINT8 u1CNum)
{
    UINT8 u1BlockSize;
    UINT32 u4HPxlPerPg, u4VPxlPerPg, u4VMsbCPgNum;

    u1BlockSize = MDDI_READ_FLD(u1VdpId, MDDI_MULTI_03, DA_BLOCK_SIZE) + 1;
    u4HPxlPerPg = 1 << (6 + u1BlockSize);
    u4VPxlPerPg = 1 << (5 - u1BlockSize);  

    _arMDDiPrm[u1VdpId].rPgNum.u4HMsbPgNum = MPY_AND_CEIL(u2Width, 1, u4HPxlPerPg);  
    _arMDDiPrm[u1VdpId].rPgNum.u4VMsbYPgNum = MPY_AND_CEIL((u2Height * u1YNum), 1, u4VPxlPerPg);    
    u4VMsbCPgNum = MPY_AND_CEIL((u2Height * u1CNum), 1, u4VPxlPerPg);
    _arMDDiPrm[u1VdpId].rPgNum.u4VMsbCPgNum = IS_420TO422(u1VdpId) ? MPY_AND_CEIL(u4VMsbCPgNum, 1, 2) : u4VMsbCPgNum;
}
    
static void _vDrvDICalLsbPageNum(UINT8 u1VdpId, UINT16 u2Width, UINT16 u2Height, UINT8 u1YNum, UINT8 u1CNum)
{
    UINT8 u18bit = (_arMDDiPrm[u1VdpId].u1BitRes == 8) ? 1 : 0; 
    UINT32 u4HPxlPerPg = 256;
    UINT32 u4VPxlPerPg = 32;
    UINT32 u4VMsbCPgNum;
    
    _arMDDiPrm[u1VdpId].rPgNum.u4HLsbPgNum = u18bit ? 0 : (MPY_AND_CEIL(u2Width, 1, u4HPxlPerPg));
    _arMDDiPrm[u1VdpId].rPgNum.u4VLsbYPgNum = u18bit ? 0 : (MPY_AND_CEIL((u2Height * u1YNum), 1, u4VPxlPerPg));
    u4VMsbCPgNum = u18bit ? 0 : (MPY_AND_CEIL((u2Height * u1CNum), 1, u4VPxlPerPg));
    _arMDDiPrm[u1VdpId].rPgNum.u4VLsbCPgNum =  IS_420TO422(u1VdpId) ? 
        MPY_AND_CEIL(u4VMsbCPgNum, 1, 2) : u4VMsbCPgNum;
}

static void _vDrvDICalBufferAddress(UINT8 u1VdpId, UINT16 u2Width, UINT16 u2Height, 
                                    UINT32 u4StartAddr, UINT32 u4EndAddr)
{
    UINT16 u1AlignBit1, u1AlignBit2;
    UINT32 u4YAddrLsb, u4UAddrMsb, u4UAddrLsb, u4VAddrMsb, u4VAddrLsb;
    UINT32 u4FlgMVAddr, u4FlgLMAddr, u4FlgEndAddr;
    UINT32 u4FlgMVPerLine, u4FlgLMPerLine;
    UINT32 u4u4HMsbPgNum = _arMDDiPrm[u1VdpId].rPgNum.u4HMsbPgNum;
    UINT32 u4u4HLsbPgNum = _arMDDiPrm[u1VdpId].rPgNum.u4HLsbPgNum;
    UINT32 u4VMsbCPgNum = _arMDDiPrm[u1VdpId].rPgNum.u4VMsbCPgNum;
    UINT32 u4VLsbCPgNum = _arMDDiPrm[u1VdpId].rPgNum.u4VLsbCPgNum;

    // Calculate data address
    u4YAddrLsb = MAKE_ALIGN_TO((u4StartAddr + (u4u4HMsbPgNum * _arMDDiPrm[u1VdpId].rPgNum.u4VMsbYPgNum)), 3, ((u4StartAddr%8)+2)%8);
    u4UAddrMsb = MAKE_ALIGN_TO((u4YAddrLsb + (u4u4HLsbPgNum * _arMDDiPrm[u1VdpId].rPgNum.u4VLsbYPgNum)), 3, ((u4StartAddr%8)+4)%8);
    u4UAddrLsb = MAKE_ALIGN_TO((u4UAddrMsb + (u4u4HMsbPgNum * u4VMsbCPgNum)), 3, ((u4StartAddr%8)+6)%8);
    
    if ((_arMDDiPrm[u1VdpId].u1NRDataMode == E_DI_NR_RGB) ||
        (bIsScalerInput444(u1VdpId) && (IS_PROG_DISPMODE(u1VdpId))))
    {
        u4VAddrMsb = MAKE_ALIGN_TO((u4UAddrLsb + (u4u4HLsbPgNum * u4VLsbCPgNum)), 3, ((u4StartAddr%8)+0)%8);
        u4VAddrLsb = MAKE_ALIGN_TO((u4VAddrMsb + (u4u4HMsbPgNum * u4VMsbCPgNum)), 3, ((u4StartAddr%8)+2)%8);
    }
    else
    {
        u4VAddrMsb = u4UAddrMsb;
        u4VAddrLsb = u4UAddrLsb;
    }
    
    // Calculate flag address
    u4FlgMVAddr = (u4VAddrLsb + (u4u4HLsbPgNum * u4VLsbCPgNum)) << 11;
    u1AlignBit1 = IS_OVERSAMPLE_OVER_720(u1VdpId) ? 512 : 256;
    u1AlignBit2 = IS_OVERSAMPLE_OVER_720(u1VdpId) ? 16 : 8;

    if ((u1VdpId == VDP_1) && IS_INTERLACE(u1VdpId)) // onlt interlace timing need to use flag
    {
        // Motion Vector Flag
        u4FlgMVPerLine = MPY_AND_CEIL(u2Width, 1, u1AlignBit1) * u1AlignBit1 / u1AlignBit2;
        u4FlgLMPerLine = MPY_AND_CEIL(u2Width, 1, u1AlignBit1) * u1AlignBit1 / 8;
        // Local Motion Flag
        u4FlgLMAddr = u4FlgMVAddr + MPY_AND_CEIL((u4FlgMVPerLine * u2Height * 16 / 8), 1, 16) * 16;
        u4FlgEndAddr = u4FlgLMAddr + MPY_AND_CEIL((u4FlgLMPerLine * MPY_AND_CEIL(u2Height, 1, 4) * 16 / 8), 1, 16) * 16;
    }
    else
    {
        u4FlgLMAddr = u4FlgMVAddr;
        u4FlgEndAddr = u4FlgMVAddr;
    }
    
    if (u4FlgEndAddr > u4EndAddr)
    {
        u4FlgEndAddr = u4EndAddr;
    }
    if (u4FlgMVAddr > u4EndAddr)
    {
        u4FlgMVAddr = u4EndAddr;
    }
    
    vDrvDISetBufferProtect(u1VdpId, u4StartAddr, (u4FlgMVAddr >> 11), (u4FlgMVAddr >> 4), (u4FlgEndAddr >> 4));

    vDrvDISetBufferAddr(u1VdpId, u4StartAddr, u4YAddrLsb, u4UAddrMsb, u4UAddrLsb, 
    u4VAddrMsb, u4VAddrLsb, (u4FlgMVAddr >> 4), (u4FlgLMAddr >> 4));
}

static void _vDrvDISrmSetBufImpl(UINT8 u1VdpId, const MDDI_FBM_POOL_T * prFbmPool)
{
    UINT8 u1YFldNum, u1CFldNum;
    UINT16 u2Width, u2Height;
    UINT32 u4StartAddr = prFbmPool->u4Addr >> 11;
    UINT32 u4EndAddr = (prFbmPool->u4Addr + prFbmPool->u4Size);

    if (u1VdpId >= VDP_NS)
    {
        return;
    }

    u2Width = _arMDDiPrm[u1VdpId].u2Width;
    u2Height = IS_INTERLACE(u1VdpId) ? (_arMDDiPrm[u1VdpId].u2Height/2) : _arMDDiPrm[u1VdpId].u2Height;
    u2Height = IS_420TO422(u1VdpId) ? (((u2Height+1)>>1)<<1) : u2Height;
    
    vDrvDISetBitRes(u1VdpId, (_arMDDiPrm[u1VdpId].u1BitRes == 8) ? 1 : 0);
    _vDrvDISetFrameNum(u1VdpId);
    
    u1YFldNum = MDDI_READ_FLD(u1VdpId, MDDI_MULTI_03, DA_FRAME_NUM_Y) + 1;
    u1CFldNum = MDDI_READ_FLD(u1VdpId, MDDI_MULTI_03, DA_FRAME_NUM_U) + 1;

    _vDrvDICalMsbPageNum(u1VdpId, u2Width, u2Height, u1YFldNum, u1CFldNum);
    _vDrvDICalLsbPageNum(u1VdpId, u2Width, u2Height, u1YFldNum, u1CFldNum);
    _vDrvDICalBufferAddress(u1VdpId, u2Width, u2Height, u4StartAddr, u4EndAddr);
}

static void _vDrvDICrcTest(void)
{
    if (_u1CrcLog == 1)
    {
        Printf("Src H %08x V %08x\n", 
            MDDI_READ_FLD(VDP_1, MDDI_SC_07, SRCW_CRC_OUT_H),
            MDDI_READ_FLD(VDP_1, MDDI_SC_08, SRCW_CRC_OUT_V));
    }
    
    if (_u1CrcLog == 2)
    {
        Printf("Out H %08x V %08x\n\n", 
            MDDI_READ_FLD(VDP_1, MDDI_OC_0A, OUT_CRC_OUT_H),
            MDDI_READ_FLD(VDP_1, MDDI_OC_0B, OUT_CRC_OUT_V));
    }
}

static void _vDrvDIISR(UINT16 u2Vector)
{
    UINT8 u1IntOn[2];

    ASSERT(u2Vector == VECTOR_PSCAN);

    // Input Vsync Rising
    u1IntOn[VDP_1] = MDDI_READ_FLD(VDP_1, MDDI_GC_01, INT_MIVS_R_STA);
    u1IntOn[VDP_2] = MDDI_READ_FLD(VDP_1, MDDI_GC_01, INT_PIVS_R_STA);
    
    if (u1IntOn[VDP_1])
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, SV_ON, INT_MIVS_R_CLR);//Clear interrupt
        _u2Vcnt = MDDI_READ_FLD(VDP_1, MDDI_OC_05, STATUS_OUT_VCNT);        
    }

    if (u1IntOn[VDP_2])
    {
        #ifdef CC_SUPPORT_H264
        _vDrvDIRunH264Loop();
        #endif
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, SV_ON, INT_PIVS_R_CLR);//Clear interrupt
    }

    // Input Vsync Falling
    u1IntOn[VDP_1] = MDDI_READ_FLD(VDP_1, MDDI_GC_01, INT_MIVS_F_STA);
    u1IntOn[VDP_2] = MDDI_READ_FLD(VDP_1, MDDI_GC_01, INT_PIVS_F_STA);
    if (u1IntOn[VDP_1])
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, SV_ON, INT_MIVS_F_CLR);//Clear interrupt
    }

    if (u1IntOn[VDP_2])
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, SV_ON, INT_PIVS_F_CLR);//Clear interrupt
    }

    // Output Vsync Rising
    u1IntOn[VDP_1] = MDDI_READ_FLD(VDP_1, MDDI_GC_01, INT_MOVS_R_STA);
    u1IntOn[VDP_2] = MDDI_READ_FLD(VDP_1, MDDI_GC_01, INT_POVS_R_STA);
    if (u1IntOn[VDP_1])
    {
        #ifdef CC_SCPOS_3DTV_SUPPORT        
            vScpipOnDIOutputISR();
        #endif  
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, SV_ON, INT_MOVS_R_CLR);//Clear interrupt
    }

    if (u1IntOn[VDP_2])
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, SV_ON, INT_POVS_R_CLR);//Clear interrupt
    }

    // Output Vsync Falling
    u1IntOn[VDP_1] = MDDI_READ_FLD(VDP_1, MDDI_GC_01, INT_MOVS_F_STA);
    u1IntOn[VDP_2] = MDDI_READ_FLD(VDP_1, MDDI_GC_01, INT_POVS_F_STA);
    if (u1IntOn[VDP_1])
    {
        vDrvDISetQualityISR();
        #ifdef PSCAN_AUTO_VERIFY
        vDrvDIInkHistogramPolling(VDP_1);
        vDrvDIGetCRCResult(VDP_1);
        vDrvDIGetClipLength(VDP_1);
        #endif
        _vDrvDICrcTest();
        x_sema_unlock(_hDIPQIsrSema);    
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, SV_ON, INT_MOVS_F_CLR);//Clear interrupt
    }

    if (u1IntOn[VDP_2])
    {
        #ifdef PSCAN_AUTO_VERIFY
        vDrvDIInkHistogramPolling(VDP_2);
        vDrvDIGetCRCResult(VDP_2);
        vDrvDIGetClipLength(VDP_2);
        #endif
        MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, SV_ON, INT_POVS_F_CLR);//Clear interrupt
    }

    
    VERIFY(BIM_ClearIrq(VECTOR_PSCAN));
}

static void _vDrvDIInitISR(void)
{
    static UINT8 u1Initialized = SV_FALSE;
    x_os_isr_fct pfnOldIsr;

    if (!u1Initialized)
    {
        VERIFY(x_sema_create(&_hDIPQIsrSema, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);
    
    	if (x_reg_isr(VECTOR_PSCAN, _vDrvDIISR, &pfnOldIsr) != OSR_OK)
    	{
            LOG(7, "Error: fail to register PSCAN ISR!\n");
    	}
    	else
        {
            LOG(7, "Register PSCAN ISR OK!\n");
    	}
    
    	u1Initialized = SV_TRUE;
        LOG(7, "PSCAN ISR registered!\n");
    }
}

static UINT32 _u4DrvDIGetClkRate(UINT8 u1VdpId)
{
    UINT8 u1ClkType;
    UINT32 u4ClkRate = 0xffff;

    u1ClkType = IO32ReadFldAlign(CKGEN_DISP_CKCFG, FLD_PS_DISP_CLK_SEL);    

    switch(u1ClkType)
    {
#if defined (CC_MT5365)
        case 0: // vdoin_mpclk
            u4ClkRate = ((RegReadFldAlign(OMUX_00, VDOIN_MPCLK_SEL) == 0) ? 17550 : 19200);
            break;
        case 1: // oclk, To Do
            u4ClkRate = BSP_GetPLLClock(CAL_SRC_LVDS_DPIX_CLK_SS);
            break;
        case 2: // ps_dig_div_ck
                if (u1VdpId == VDP_1)
                {
                    u4ClkRate = ((IO32ReadFldAlign(CKGEN_VPCLK_CFG, FLD_PS_DIG_DIV_CLK_SEL) == 0) ? 19200 : 18300);
                }
                else
                {
                    u4ClkRate = ((IO32ReadFldAlign(CKGEN_VPCLK_CFG, FLD_PIP_PS_DIG_DIV_SEL) == 0) ? 19200 : 18300);
                }    
            break;
#elif  defined (CC_MT5395)   
        case 0: // oclk
            // to do 
            if (1) // mLVDS, EPI, LVDS mode
            {            
                u4ClkRate = BSP_GetPLLClock(CAL_SRC_VOPLL);
            }
            else // VB1
            {
                u4ClkRate = BSP_GetPLLClock(CAL_SRC_PIXPLL);
            }
            u4ClkRate = u2DrvGetInternalPCLKRatio()? (u4ClkRate/u2DrvGetInternalPCLKRatio()): u4ClkRate;
            break;
        case 2: // vdoin_mpclk
            u4ClkRate = ((RegReadFldAlign(OMUX_00, VDOIN_MPCLK_SEL) == 0) ? 18360 : 19200);
            break;
        case 3: // ps_dig_div_ck
            if (u1VdpId == VDP_1)
            {
                u4ClkRate = ((IO32ReadFldAlign(CKGEN_VPCLK_CFG, FLD_PS_DIG_DIV_CLK_SEL) == 0) ? 16875 : 19200);
            }
            else
            {
                u4ClkRate = ((IO32ReadFldAlign(CKGEN_VPCLK_CFG, FLD_PIP_PS_DIG_DIV_SEL) == 0) ? 16875 : 19200);
            }
            break;
#endif
        default:
            break;
    }
    
    return u4ClkRate;
}

static void _vDrvDISetMisc(UINT8 u1VdpId)
{
    if (u1VdpId >= VDP_NS)
    {
        return;
    }
    
    UINT16 u2HClip = 0, u2VClip = 0;

    // Reset SW counter
    _arMDDiPrm[u1VdpId].u1StatusCnt = 0;
    _arMDDiPrm[u1VdpId].u1FreezeCnt = 0;
        
    #if !PREPROC_ENABLE_SYNC_REGEN
    u2HClip = _arMDDiPrm[u1VdpId].u2HClip;
    u2VClip = (IS_MPEG(u1VdpId))
                ? (_arMDDiPrm[u1VdpId].u2VClip + _arMDDiPrm[u1VdpId].u2DTVClip)
                : (_arMDDiPrm[u1VdpId].u2VClip);
    #endif

    vDrvDISetClip(u1VdpId, u2HClip, u2VClip);
    vDrvDISetROIPixel(u1VdpId, MPY_AND_CEIL(_arMDDiPrm[u1VdpId].u2Width, _arMDDiPrm[u1VdpId].u2Height, 16));
    vDrvDISetColorMode(u1VdpId, IS_420TO422(u1VdpId));
    vDrvDISRCWOutIOnOff(u1VdpId, IS_INTERLACE(u1VdpId));
    vDrvDIProgModeOnOff(u1VdpId, (IS_MPEG_PROG(u1VdpId) && _u1DrvDI420T0422En(u1VdpId)));
    vDrvDISetStdVMask(u1VdpId, (IS_PROG_DISPMODE(u1VdpId)) ? SV_OFF : SV_ON);
    vDrvDISetStdVBlank(u1VdpId, (_arMDDiPrm[u1VdpId].u1DecType == SV_VD_TVD3D) ? SV_ON : SV_OFF);

    #ifdef CC_MT5395
    //Share FIFO to MJC Progressive Film Mode Detection    
    MDDI_WRITE_FLD(SV_VP_MAIN, MDDI_GC_01, !(SV_ON==_arMDDiPrm[SV_VP_MAIN].u1Interlace), 
        MJC_FILM_SRAM_SHARE_EN);    
    #endif
}

static void _vDrvDISetVParam(UINT8 u1VdpId, UINT16 u2VTotal, UINT16 u2Height)
{
    UINT16 u2Vdew = MPY_AND_TRUN(u2Height, 1, 2);
    u2Vdew = (IS_420TO422(u1VdpId)) // vdew needs to be even when 420 mode
                ? (((u2Vdew+1)>>1)<<1) : u2Vdew;

    //Height of active video (in unit of 2 lines)
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_00, u2Vdew, VDEW);
    
    //Output Vtotal under free run mode
    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_02, ((u2VTotal > 2047) ? 2047 : u2VTotal), OUT_VT);    
}

static void _vDrvDISetFldParam(UINT8 u1VdpId)
{
    UINT8 u1S_V_Bgn = (u1VdpId == VDP_1) ? 3 : 8;
    UINT8 u1K_V_Bgn = (u1VdpId == VDP_1) ? 4 : 9;
    UINT8 u1K_H_Bgn = (IS_SD_HEIGHT(u1VdpId)) ? 0x18 : 0x14;
    UINT8 u1FrcLine;
    #ifdef CC_FLIP_MIRROR_SUPPORT
    UINT8 u1IsPScanFlipMirror =0;
    #endif

    if (u1VdpId >= VDP_NS)
    {
        return;
    }

    #ifdef CC_FLIP_MIRROR_SUPPORT
    u1IsPScanFlipMirror = (u4GetFlipMirrorModule(u1VdpId)==FLIP_BY_PSCAN) ? 1 : 0;

    if (u1IsPScanFlipMirror && ((u1FlipMirrorConfig == DI_FLIP_ON_MODE) || (u1FlipMirrorConfig == DI_FLIP_MIRROR_ON_MODE))
    && (((_u1DrvDIGetDispmode(u1VdpId) == DI_PROGRESSIVE_DISP) && IS_420TO422(u1VdpId))
    || (_u1DrvDIGetDispmode(u1VdpId) == DI_INTERLACE_DISP)))
    {
        u1FrcLine = 4;
    }
    else
    #endif
    {
        if ((_arMDDiPrm[u1VdpId].u1DataMode == E_DI_FULL_1W3R)
        || (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_CBOB_1W3R)
        || (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_YCBOB_0W0R))
        {
            u1FrcLine = u1K_V_Bgn + 4;
        }
        else
        {
            u1FrcLine = 0;
        }
    }	

    vDrvDISetBgn(u1VdpId, u1S_V_Bgn, u1K_H_Bgn, u1K_V_Bgn);
    vDrvDISetFrcLine(u1VdpId, u1FrcLine);
    vDrvDISetFldPrediction(u1VdpId, SV_ON);
	//To Solve CR[DTV00334227] DTV 1080i flutter issue.
	vDrvDISetReSyncOnOff(u1VdpId, SV_OFF);
	
    #ifdef CC_FLIP_MIRROR_SUPPORT
	//if(_u1DrvDIGetDispmode(u1VdpId))
	if(u1VdpId ==VDP_1)
	{
            if(u4GetFlipMirrorModule(u1VdpId)==FLIP_BY_PSCAN)
            {
                vDrvDISetFlipMirror(u1VdpId, u1FlipMirrorConfig);
            }
            else
            {
                vDrvDISetFlipMirror(u1VdpId, DI_NORMAL_MODE);
            }
	}
	#endif

}

static void _vDrvDISetHParam(UINT8 u1VdpId, UINT16 u2HTotal, UINT16 u2Width)
{
    UINT32 u2OutHT, u4DataRate;

    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_04, (IS_OVERSAMPLE_OVER_720(u1VdpId) ? 0xC0 : 0x60), H_BLANK);
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_00, u2Width, HDEW);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MC_00, MPY_AND_CEIL(u2Width, 1, 16), H_PITCH);

    // 1W4R + not pscan dispmode
    if (_arMDDiPrm[u1VdpId].u2VTotal == 1125)
    {
    	u4DataRate = 14850; // 148.50MHz
    }
    else
    {
    	u4DataRate = 2700; // 27 MHz
    }

    LOG(6, "Oversample %d   HTotal %d  Clock rate %d \n", 
            IS_OVERSAMPLE_OVER_720(u1VdpId),  _arMDDiPrm[u1VdpId].u2HTotal,  _u4DrvDIGetClkRate(u1VdpId));

    u2OutHT = (bDrvVideoGetWidthOverSample(u1VdpId)) ? (_arMDDiPrm[u1VdpId].u2HTotal/2) : _arMDDiPrm[u1VdpId].u2HTotal;
    u2OutHT = u2OutHT * _u4DrvDIGetClkRate(u1VdpId) / u4DataRate;
    MDDI_WRITE_FLD(u1VdpId, MDDI_OC_02, u2OutHT, OUT_HT);
}

#if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
static BOOL  _bDrvGetDISupport3DPR(UINT8 u1VdpId, UINT16 u1Interlace, UINT16 u2Width, UINT16 u2Height)
{    
    if ((u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_TB) &&
    (!u1Interlace) /*&& (u2Width == SUPPORT_3D_PR_WIDTH) */&& (u2Height == 720)) // treat 720p as 720i
    {
        return SV_TRUE;
    }
    else
    {
        return SV_FALSE;
    }
}
#endif

static UINT8 _u1DrvDIMaskedOnOff(UINT8 u1VdpId)
{
    UINT8 u1IsChannelOn = (u1VdpId == VDP_1) ? _rMChannel.bIsChannelOn : _rPChannel.bIsChannelOn;
    UINT8 u1SigStatus = bDrvVideoSignalStatus(u1VdpId);
    UINT8 u1SigType = bGetSignalType(u1VdpId);

    if ((u1IsChannelOn == SV_FALSE) // 1. Channel is off
    || (u1SigStatus == SV_VDO_NOSUPPORT) // 2. Timing is not supported

    // 3. Snow screen should be displayed even signal is not stable (TV)
    || ((u1SigType != SV_ST_TV) && ((u1SigStatus == SV_VDO_NOSIGNAL) || (u1SigStatus == SV_VDO_UNKNOWN)))    

    // 4. Not mpeg source and progressive timing but not display mode
    || ((u1SigType != SV_ST_MPEG) && (IS_PROGRESSIVE(u1VdpId)) && (!IS_PROG_DISPMODE(u1VdpId)))

    // 5. For Progressive Mpeg 420->422 dram mode and no 420to422
    || ((IS_MPEG_PROG(u1VdpId)) && (!IS_PROG_DISPMODE(u1VdpId)) && (!IS_420TO422(u1VdpId)))

    // 6. TDTV Force Bypass
    || IS_DI_3D_DI_BYPASS(u1VdpId))    
    {
    
        #if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))        
        if  (IS_SUPPORT_3D_PR(u1VdpId))        
        {            
            return SV_ON;        
        }        
        else   
        #endif 
        {
    	    return SV_OFF;
        }
    }
    else
    {
        return SV_ON;
    }
}

static UINT8 _u1DrvDIModeChange(UINT8 u1VdpId)
{
    UINT8 u1Ret, u1OnOff, u1DecType, u1Interlace, u1BitRes;
    UINT8 u1PSCANDisplay, u1DataMode, u1NRDataMode;
    UINT16 u2HTotal, u2VTotal, u2Width, u2Height;
    UINT16 wSrcXOffset, wSrcYOffset, u2BufWidth, u2BufHeight;
    UINT32 u4BufAddr, u4NTDOfst, u4ProgDispClip = 0;
    PSCAN_CLIP_INFO_T ScalerOverscanInfo;

    if (u1VdpId >= VDP_NS)
    {
        return SV_FALSE;
    }

    /* Get video signal status */
    u1OnOff = _u1DrvDIMaskedOnOff(u1VdpId);
    u1DecType = bGetVideoDecType(u1VdpId);
    u1Interlace = bDrvVideoIsSrcInterlace(u1VdpId);
    u2VTotal = wDrvVideoGetVTotal(u1VdpId);
    u2HTotal = wDrvVideoGetHTotal(u1VdpId);

    if (u1ScpipGetClipInfo(u1VdpId, &ScalerOverscanInfo))
    {
        // width needs to be integer times of 16 before 5365/5395 eco
        if ((!IS_ECO_IC()) && IS_PROG_DISPMODE(u1VdpId))
        {
            u2Width = ((ScalerOverscanInfo.wWidth+15)>>4)<<4;
            u4ProgDispClip = u2Width - ScalerOverscanInfo.wWidth;
        }
        else
        {
            u2Width = ((ScalerOverscanInfo.wWidth+7)>>3)<<3; // needs to be mutiple of 8 for mjc film mode
            u4ProgDispClip = 0;
        }
            	
        u2Height = ((ScalerOverscanInfo.wHeight+1)>>1)<<1;
        wSrcXOffset = ScalerOverscanInfo.wX;
        wSrcYOffset = ScalerOverscanInfo.wY;
    }
    else
    {
        wSrcXOffset = _arMDDiPrm[u1VdpId].u2HClip;
        wSrcYOffset = _arMDDiPrm[u1VdpId].u2VClip;
        u2Width = _arMDDiPrm[u1VdpId].u2Width;
        u2Height = _arMDDiPrm[u1VdpId].u2Height;
    }
    
    #if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))    
    if (_bDrvGetDISupport3DPR(u1VdpId, u1Interlace, u2Width, u2Height))
    {
        _arMDDiPrm[u1VdpId].u1Support3DPR = SV_ON;
    }
    else
    {
        _arMDDiPrm[u1VdpId].u1Support3DPR = SV_OFF;
    }
    if (IS_SUPPORT_3D_PR(u1VdpId))
    {
        u2Height = u2Height * 2;
        u2VTotal *= 2; 
        u1Interlace = 1;    
    }
    #endif
        
    //Get SRM buffer information
    u4BufAddr = _arMDDiPrm[u1VdpId].rFbmPool.u4Addr;
    u2BufWidth = _arMDDiPrm[u1VdpId].rFbmPool.u4Width;
    u2BufHeight = _arMDDiPrm[u1VdpId].rFbmPool.u4Height;
    u1BitRes = (_arMDDiPrm[u1VdpId].rFbmPool.fg10bit) ? 10 : 8;
    u1PSCANDisplay = _u1DrvDIGetDispmode(u1VdpId);

    // handle non-standard signal at dram mode
    if ((!u1PSCANDisplay) && (_arMDDiPrm[u1VdpId].u1DecType == SV_VD_YPBPR))
    {
        u4NTDOfst = u4DrvVideoGetNSDOffset(u1VdpId);
        if (u4NTDOfst != 0x800)
        {
            if (((u2Height + u4NTDOfst) > 0x800) && (u4NTDOfst < 0x800))
            {
                LOG(3, "Non-Standard Signal Detected (%d)! Ofst: %d\n", u1VdpId, u4NTDOfst);
                u2Height = u2Height + u4NTDOfst - 0x800;                
            }
        }
    }
    
    u1DataMode = _u1DrvDIGetDataMode(u1VdpId);
    u1NRDataMode = _u1DrvGetNRDataMode(u1VdpId);

    //Please note that u1OnOff is not updated here for control flow purpose
    if ((_arMDDiPrm[u1VdpId].u1OnOff != u1OnOff)
    	|| (_arMDDiPrm[u1VdpId].u4ProgDispClip != u4ProgDispClip)
    	|| (_arMDDiPrm[u1VdpId].u1DecType != u1DecType)
    	|| (_arMDDiPrm[u1VdpId].u1Interlace != u1Interlace)
    	|| (_arMDDiPrm[u1VdpId].u2HTotal != u2HTotal)
    	|| (_arMDDiPrm[u1VdpId].u2VTotal != u2VTotal)
    	|| (_arMDDiPrm[u1VdpId].u2Width != u2Width)
    	|| (_arMDDiPrm[u1VdpId].u2Height != u2Height)
    	//The following settings are controlled by SRM in vDrvDISrmSetBuf function call
    	|| (_arMDDiPrm[u1VdpId].u4BufAddr != u4BufAddr)
    	|| (_arMDDiPrm[u1VdpId].u1BitRes != u1BitRes)
    	|| (_arMDDiPrm[u1VdpId].u1DataMode != u1DataMode)
        || (_arMDDiPrm[u1VdpId].u1NRDataMode != u1NRDataMode)
    	|| (_arMDDiPrm[u1VdpId].u1PSCANDisplay != u1PSCANDisplay)
    	|| (_arMDDiPrm[u1VdpId].u4TdTvDIType !=_u4TdTvDIType[u1VdpId]))
    {
        if ((u1VdpId == VDP_2) && 
        (!IO32ReadFldAlign(CKGEN_DISP_CKCFG, FLD_SCPIP_PIP_CLK_EN))) // clock is not active, do no update
        {
            return SV_FALSE;
        }
    
    	_arMDDiPrm[u1VdpId].u1OnOff = u1OnOff;
    	_arMDDiPrm[u1VdpId].u1DecType = u1DecType;
    	_arMDDiPrm[u1VdpId].u1Interlace = u1Interlace;
    	_arMDDiPrm[u1VdpId].u2HTotal = u2HTotal;
    	_arMDDiPrm[u1VdpId].u2VTotal = u2VTotal;
    	_arMDDiPrm[u1VdpId].u2Width = u2Width;
        _arMDDiPrm[u1VdpId].u2Height = u2Height;
    	_arMDDiPrm[u1VdpId].u4BufAddr = u4BufAddr;
    	_arMDDiPrm[u1VdpId].u2BufWidth = u2BufWidth;
    	_arMDDiPrm[u1VdpId].u2BufHeight = u2BufHeight;
    	_arMDDiPrm[u1VdpId].u1BitRes = u1BitRes;
    	_arMDDiPrm[u1VdpId].u1DataMode = (E_DI_DMODE)u1DataMode;
    	_arMDDiPrm[u1VdpId].u1NRDataMode = (E_NR_DMODE)u1NRDataMode;
    	_arMDDiPrm[u1VdpId].u1PSCANDisplay = (DI_DISPLAY_MODE) u1PSCANDisplay;
         _arMDDiPrm[u1VdpId].u1420Mode = _u1DrvDI420T0422En(u1VdpId);
        _arMDDiPrm[u1VdpId].u4TdTvDIType =_u4TdTvDIType[u1VdpId];

        if (_arMDDiPrm[u1VdpId].u4ProgDispClip != u4ProgDispClip)
        {
            _arMDDiPrm[u1VdpId].u4ProgDispClip = u4ProgDispClip;
            vSetScposFlg((u1VdpId == VDP_1) ? MAIN_DISP_PRM_CHG : PIP_DISP_PRM_CHG);
        }

        #ifdef CC_MT5395
        if (u1VdpId == VDP_1)
        {
            vDrvMJCSet32PdInfoShift(u1Interlace, u1PSCANDisplay);
        }
        #endif

        u1Ret = SV_TRUE;
    }
    else
    {
    	u1Ret = SV_FALSE;
    }
    
    //We don't need to do mode change to reflect offset adjustment
    //We check if offsets are changed here and set registers directly
    if ((_arMDDiPrm[u1VdpId].u2HClip != wSrcXOffset)
        || (_arMDDiPrm[u1VdpId].u2VClip != wSrcYOffset))
    {
        #if PREPROC_ENABLE_SYNC_REGEN
    	vDrvDISetClip(u1VdpId, 0, 0);      
        vDrvPreprocSetSyncRegen(u1VdpId, wSrcXOffset,wSrcYOffset,
                                                _arMDDiPrm[u1VdpId].u2Width,
                                                _arMDDiPrm[u1VdpId].u2Height);    		 
		vDrvNRSetState(NR_STATE_REGEN_CHANGED);		
        #else
        vDrvDISetClip(u1VdpId, wSrcXOffset, wSrcYOffset);      
    	#endif

    	_arMDDiPrm[u1VdpId].u2HClip = wSrcXOffset;
    	_arMDDiPrm[u1VdpId].u2VClip = wSrcYOffset;
    }

    return u1Ret;
}

static void _vDrvDISetDeRatio(UINT8 u1VdpId)
{
    UINT8 u1RatioN = IS_OVERSAMPLE_OVER_720(u1VdpId) ? 2 : 1;

    // HD timing
    if ((_arMDDiPrm[u1VdpId].u1PSCANDisplay) 
    || (IS_MPEG_PROG(u1VdpId))
    || (!IS_SD_HEIGHT(u1VdpId)))
    {
        vDrvDISetKDE(u1VdpId, 1, 2);   
    }
    else 
    {    
        vDrvDISetKDE(u1VdpId, u1RatioN, 10);
    }

    if (_arMDDiPrm[u1VdpId].u1PSCANDisplay)
    {
        vDrvDISetSDE(u1VdpId, 0, 1, 2); 
    }
    else if ((bGetVideoDecType(u1VdpId) == SV_VD_TVD3D) 
    && ((_arMDDiPrm[u1VdpId].u1DataMode == E_DI_FULL_1W4R)
    || (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_YCBOB_1W1R)))
    {
        vDrvDISetSDE(u1VdpId, 0, u1RatioN, 10);     
    }
    else
    {
        vDrvDISetSDE(u1VdpId, 1, 0, 0);
    }
    
    if (IS_PROG_DISPMODE(u1VdpId))
    {
        vDrvDISetODE(u1VdpId, 0, 0, 0);
    }
    else if (IS_MPEG_PROG(u1VdpId))
    {
        vDrvDISetODE(u1VdpId, 0, 1, 2);
    }
    else if ((bGetVideoDecType(u1VdpId) == SV_VD_TVD3D) 
            && ((_arMDDiPrm[u1VdpId].u1DataMode == E_DI_FULL_1W4R)
            || (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_YCBOB_1W1R)))
    {
        vDrvDISetODE(u1VdpId, 0, u1RatioN*2, 10);     
    }
    else
    {
        vDrvDISetODE(u1VdpId, _arMDDiPrm[u1VdpId].u1PSCANDisplay ? 0 : 1, 1, 2);
    }
}

static void _vDrvCounterIncrease(UINT8 u1VdpId)
{
    _arMDDiPrm[u1VdpId].u1StatusCnt++;
}

static void _vDrvDISetFifoShareMode(UINT8 u1VdpId)
{
    if (IS_PROG_DISPMODE(VDP_1))
    {
        _bFifoShareMode = 0x3;
    }    
    else
    {        
        switch(_bTVMode)
        {
            case SRM_TV_MODE_TYPE_PIP:
                _bFifoShareMode = 0x1;
                break;
            case SRM_TV_MODE_TYPE_POP:
                _bFifoShareMode = 0x2;
                break;
            case SRM_TV_MODE_TYPE_NORMAL:
            default:
                _bFifoShareMode = 0x0;
                break;
        }
    }
   
    if (_bFifoShareMode != MDDI_READ_FLD(VDP_1, MDDI_MC_05, DA_FIFO_SHARE_MODE))
    {
        _bUpdate = TRUE;
        _u1Cnt = 3;
        _vDrvVideoSetMute(MUTE_MODULE_MDDI01, VDP_1, MDDI_MUTE_PERIOD, TRUE); // mute to cover garbage
        _vDrvVideoSetMute(MUTE_MODULE_MDDI01, VDP_2, MDDI_MUTE_PERIOD, TRUE); // mute to cover garbage
    }
}


static void _vDrvDISetNRDataMode(UINT8 u1VdpId, E_NR_DMODE u1DataMode)
{
    UINT8 bReadEn = ((u1DataMode == E_DI_NR_OFF) ? SV_OFF : SV_ON);
    UINT8 colorModeSel = 0;
    DI_YUV_MODE rgYUVMode = DI_YUV_MODE_YC;
    NR_YUV_MODE nrYUVMode = NR_YUV_MODE_422;

    _arMDDiPrm[u1VdpId].u1NRDataMode = u1DataMode;
    //pass NR data mode to NR driver
    vDrvNRSetNRDataMode(u1VdpId, u1DataMode);    
    
    switch (u1DataMode)
    {        
        case E_DI_NR_Y_ONLY:
            colorModeSel = IS_420TO422(u1VdpId);
            rgYUVMode = IS_INTERLACE(u1VdpId) ? DI_YUV_MODE_YC : DI_YUV_MODE_Y_ONLY;
            nrYUVMode = NR_YUV_MODE_Y_ONLY;
            break;
        case E_DI_NR_YC_420:
            colorModeSel = 1;
            rgYUVMode = DI_YUV_MODE_YC;
            nrYUVMode = NR_YUV_MODE_420;
            break;
        case E_DI_NR_YC_422:
            colorModeSel = 0;
            rgYUVMode = DI_YUV_MODE_YC;
            nrYUVMode = NR_YUV_MODE_422;
            break;
        case E_DI_NR_RGB:
            colorModeSel = 0;
            rgYUVMode = DI_YUV_MODE_YUV;
            nrYUVMode = NR_YUV_MODE_444;
            break;
        case E_DI_NR_OFF:
        default:
            colorModeSel = IS_420TO422(u1VdpId);
            rgYUVMode = IS_INTERLACE(u1VdpId) ? DI_YUV_MODE_YC : DI_YUV_MODE_Y_ONLY;
            nrYUVMode = NR_YUV_MODE_Y_ONLY;
            break;
    }

    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_09, bReadEn, DA_NR_READ_ENABLE); // pscan read enable
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_03, rgYUVMode, DA_MODE_YUV);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_00, (colorModeSel ? 0 : 1), M422);
    MDDI_WRITE_FLD(u1VdpId, MDDI_SC_06, (nrYUVMode == NR_YUV_MODE_444) ? SV_ON : SV_OFF, SRCW_CRC_V_MASK);
    
    vDrvNRSetReadEnable(u1VdpId,bReadEn);
    vDrvNRSetYUVMode(u1VdpId,nrYUVMode);

    if (IS_NR_ON(u1VdpId))
    {
        vDrvNRSetIIRPath(u1VdpId);
    }
}

static void _vDrvDIConfigProgDispmode(UINT8 u1VdpId, UINT8 u1OnOff)
{   
    UINT8 u1NROnOff = IS_NR_ON(u1VdpId);

    if (u1VdpId == VDP_2)
    {
        return;
    }
    
    if (u1OnOff)
    {
        if (bIsScalerInput444(VDP_1)) // 444 
        {
            MDDI_WRITE_FLD(VDP_1, MDDI_OC_01, SV_ON, PROG444);
            MDDI_WRITE_FLD(VDP_1, MDDI_OC_01, SV_OFF, PROG422);
            MDDI_WRITE_FLD(VDP_1, MDDI_MC_05, SV_OFF, DA_PROG_420);
            MDDI_WRITE_FLD(VDP_1, MDDI_MULTI_03, DI_YUV_MODE_YUV, DA_MODE_YUV);
            MDDI_WRITE_FLD(VDP_1, MDDI_MC_05, SV_ON, DA_SC_READ_ENABLE);
            MDDI_WRITE_FLD(VDP_1, MDDI_SC_09, u1NROnOff, DA_NR_READ_ENABLE);
        }
        else
        {
            if (IS_420TO422(VDP_1)) // 420
            {
                MDDI_WRITE_FLD(VDP_1, MDDI_OC_01, SV_OFF, PROG422);
                MDDI_WRITE_FLD(VDP_1, MDDI_MC_05, SV_ON, DA_PROG_420);
                MDDI_WRITE_FLD(VDP_1, MDDI_SC_09, SV_ON, DA_NR_READ_ENABLE);
                MDDI_WRITE_FLD(VDP_1, MDDI_MC_05, SV_OFF, DA_SC_READ_ENABLE);
            }
            else // 422
            {
                MDDI_WRITE_FLD(VDP_1, MDDI_OC_01, SV_ON, PROG422);
                MDDI_WRITE_FLD(VDP_1, MDDI_MC_05, SV_OFF, DA_PROG_420);
                MDDI_WRITE_FLD(VDP_1, MDDI_MC_05, SV_ON, DA_SC_READ_ENABLE);
                MDDI_WRITE_FLD(VDP_1, MDDI_SC_09, u1NROnOff, DA_NR_READ_ENABLE);
            }
            MDDI_WRITE_FLD(VDP_1, MDDI_OC_01, SV_OFF, PROG444);
            MDDI_WRITE_FLD(VDP_1, MDDI_MULTI_03, DI_YUV_MODE_YC, DA_MODE_YUV);
        }
    }
    else
    {
        MDDI_WRITE_FLD(VDP_1, MDDI_OC_01, SV_OFF, PROG422);
        MDDI_WRITE_FLD(VDP_1, MDDI_OC_01, SV_OFF, PROG444);
        MDDI_WRITE_FLD(VDP_1, MDDI_MC_05, SV_OFF, DA_PROG_420);
        MDDI_WRITE_FLD(VDP_1, MDDI_MC_05, SV_OFF, DA_SC_READ_ENABLE);
    }
}

static void _vDrvSetDILRInverse(UINT8 u1VdpId)
{    
    if (u1VdpId == VDP_2)    
    {        
        return;    
    }    
    if ((_arMDDiPrm[u1VdpId].u1PSCANDisplay) && (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_FS))    
    {        
        MDDI_WRITE_FLD(u1VdpId, MDDI_OC_01, SV_ON, DISP_R_INV);    
    }    
    else    
    {        
        MDDI_WRITE_FLD(u1VdpId, MDDI_OC_01, SV_OFF, DISP_R_INV);    
    }
}
static void _vDrvDISetParam(UINT8 u1VdpId)
{
    if (u1VdpId >= VDP_NS)
    {
        return;
    }
    
    vDrvDILoadRegTbl(u1VdpId, MDDI_MIF_INIT);

    vDrvDIFrameRepeat(u1VdpId, SV_OFF);
        
    vDrvDISetInterpMode(u1VdpId, _arMDDiPrm[u1VdpId].u1DataMode);

    _vDrvDISetFifoShareMode(u1VdpId);

    _vDrvDISetDeRatio(u1VdpId);

    _vDrvDISrmSetBufImpl(u1VdpId, &_arMDDiPrm[u1VdpId].rFbmPool);

    _vDrvDISetHParam(u1VdpId, _arMDDiPrm[u1VdpId].u2HTotal, _arMDDiPrm[u1VdpId].u2Width);

    _vDrvDISetVParam(u1VdpId, _arMDDiPrm[u1VdpId].u2VTotal, _arMDDiPrm[u1VdpId].u2Height);
        
    _vDrvDISetFldParam(u1VdpId);
  
    _vDrvDISetMisc(u1VdpId);

    _vDrvDISetNRDataMode(u1VdpId, _arMDDiPrm[u1VdpId].u1NRDataMode);

    _vDrvDIConfigProgDispmode(u1VdpId, IS_PROG_DISPMODE(u1VdpId) ? SV_ON : SV_OFF);

    _vDrvSetDILRInverse(u1VdpId);
    
    //Set video quality parameters
    #ifndef __MODEL_slt__
    vDrvDISetQuality(u1VdpId);
    #endif
}

static void _vDrvSetDIBufferInfo(UINT8 u1Path, UINT32 u4Mode, 
            UINT32 u4Address, UINT32 u4Size, UINT32 u4Width, UINT32 u4Height)
{
    _arMDDiPrm[u1Path].rFbmPool.u4Addr = u4Address;
    _arMDDiPrm[u1Path].rFbmPool.u4Size = u4Size;
    _arMDDiPrm[u1Path].rFbmPool.u4Width = u4Width;
    _arMDDiPrm[u1Path].rFbmPool.u4Height = u4Height;
    _arMDDiPrm[u1Path].rFbmPool.fg10bit = (u4Mode & FBM_POOL_MODE_10BIT) > 0;
    _arMDDiPrm[u1Path].rFbmPool.fg422 = (u4Mode & FBM_POOL_MODE_422_FB) > 0;
    _arMDDiPrm[u1Path].rFbmPool.u4Mode = u4Mode;

    #ifdef CC_FLIP_MIRROR_SUPPORT
        if (u1Path == VDP_1)
        {
            //if(_u1DrvDIGetDispmode(u1Path))
            if(u4GetFlipMirrorModule(u1Path)==FLIP_BY_PSCAN)
            {
                vDrvDISetFlipMirror(u1Path, u1FlipMirrorConfig);
            }
            else
            {
                vDrvDISetFlipMirror(u1Path, DI_NORMAL_MODE);
            }
        }
    #endif
}

static void _vDrvDIInitClock(UINT8 u1VdpId)
{
    if (u1VdpId == VDP_1)
    {
        vIO32WriteFldAlign(CKGEN_VPCLK_STOP, SV_OFF, FLD_MAIN_CLK_STOP);
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, SV_OFF, FLD_PS_DIG_DIV_CLK_EN);
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0x0, FLD_PS_DIG_DIV_CLK_SEL); // 192m
    }
    else
    {
        vIO32WriteFldAlign(CKGEN_VPCLK_STOP, SV_OFF, FLD_PIP_CLK_STOP);
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, SV_OFF, FLD_PIP_PS_DIG_DIV_EN);
        vIO32WriteFldAlign(CKGEN_VPCLK_CFG, 0x0, FLD_PIP_PS_DIG_DIV_SEL); // 192m
    }
    
    vDrvDISetClock(u1VdpId, SV_ON);
}

static void _vDrvDIReset(UINT8 u1VdpId, UINT8 bStart)
{    
    if (bStart)
    {
        // Disable double buffering and turn off read/write 
        vDrvDISetDoubleBuffer(u1VdpId, SV_OFF);
        vDrvDISetDramCtrl(u1VdpId, SV_OFF);
        vDrvDISetClkReset(u1VdpId, SV_ON);
    }
    else
    {
        vDrvDISetClkReset(u1VdpId, SV_OFF);
    }
}

static UINT32 _u4DrvDIGetFlipMirror(UINT8 u1VdpId)
{    
    // proressive 420 dispmode
    if (((IS_PROGRESSIVE(u1VdpId)) && IS_420TO422(u1VdpId))
		|| IS_INTERLACE(u1VdpId))
    {
        return MDDI_READ_FLD(u1VdpId, MDDI_MULTI_03, DA_DISPLAY_MODE);
    }
    else
    {
        return MDDI_READ_FLD(u1VdpId, MDDI_MULTI_03, DA_SC_DISPLAY_MODE);
    }
}

static void _vDrvDIFreezePatch(UINT8 u1VdpId)
{
    if (_arMDDiPrm[u1VdpId].u1FreezeCnt)
    {
        _arMDDiPrm[u1VdpId].u1FreezeCnt--;
        
        if (_arMDDiPrm[u1VdpId].u1FreezeCnt == 0)
        {
            if (!IS_ECO_IC())
            {
                vDrvDIFilmModeFreeze(u1VdpId, SV_ON);
            }
            vDrvNRSet3DNROnOffFreeze(SV_ON);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//  PSCAN API
////////////////////////////////////////////////////////////////////////////////
void vDrvDISetDramCtrl(UINT8 u1VdpId, UINT8 u1DramCOnOff)
{
    UINT8 u1AF_2_CF, u1AF_SRC_SEL, u1DramR;
    UINT8 u1FLD_R_EN, u1DRAM_RW_EN, u1TgMode;

    if (u1VdpId >= VDP_NS)
    {
        return;
    }

    if (u1DramCOnOff)
    {
    	switch (_arMDDiPrm[u1VdpId].u1DataMode)
    	{
            case E_DI_FULL_1W3R:			
                u1AF_SRC_SEL = 0x1;
                u1AF_2_CF = 0x0;
                u1FLD_R_EN = E_BF_Y | E_CF_Y | E_DF_Y | E_BF_C | E_CF_C | E_DF_C;
                #ifdef PSCAN_AUTO_VERIFY
                    u1DRAM_RW_EN = E_SRC_W | E_SRC_R | EN_MV_W | EN_MV_R;
                #else
                    u1DRAM_RW_EN = E_DRAM_ALL_RW;
                #endif
                break;
            case E_DI_FULL_1W4R:
            case E_DI_FLIP_MIRROR_1W4R:
                u1AF_SRC_SEL = 0x0;
                u1AF_2_CF = 0x0;
                u1FLD_R_EN = E_ALL_FLD;
                #ifdef PSCAN_AUTO_VERIFY
                    u1DRAM_RW_EN = E_SRC_W | E_SRC_R | EN_MV_W | EN_MV_R;
                #else
                    u1DRAM_RW_EN = E_DRAM_ALL_RW;
                #endif
                break;
            case E_DI_CBOB_1W3R:            
                u1AF_SRC_SEL = 0x1;
                u1AF_2_CF = 0x0;
                u1FLD_R_EN = E_BF_Y | E_CF_Y | E_DF_Y | E_CF_C;
                #ifdef PSCAN_AUTO_VERIFY
                    u1DRAM_RW_EN = E_SRC_W | E_SRC_R | EN_MV_W | EN_MV_R;
                #else
                    u1DRAM_RW_EN = E_DRAM_ALL_RW;
                #endif
            	break;
            case E_DI_CBOB_1W4R:
            case E_DI_FLIP_MIRROR_CBOB_1W4R:
                u1AF_SRC_SEL = 0x0;
                u1AF_2_CF = 0x0;
                u1FLD_R_EN = E_AF_Y | E_BF_Y | E_CF_Y | E_DF_Y | E_CF_C;
                #ifdef PSCAN_AUTO_VERIFY
                    u1DRAM_RW_EN = E_SRC_W | E_SRC_R | EN_MV_W | EN_MV_R;
                #else
                    u1DRAM_RW_EN = E_DRAM_ALL_RW;
                #endif
                break;
            case E_DI_YCBOB_0W0R:			
                u1AF_SRC_SEL = 0x1;
                u1AF_2_CF = 0x1;
                u1FLD_R_EN = 0x0;
                u1DRAM_RW_EN = ((IS_DI_3D_FPI_MODE(u1VdpId) || IS_DI_3D_I_MODE(u1VdpId)) ? E_SRC_W : 0);  
                break;
            case E_DI_YCBOB_1W1R:
            case E_DI_FLIP_MIRROR_YCBOB_1W1R:
                u1AF_SRC_SEL = _u1DrvDIGetSrcSel(u1VdpId);
                u1AF_2_CF = 0x1;
                u1FLD_R_EN = E_AF_Y | E_AF_C;
                u1DRAM_RW_EN = E_SRC_R | E_SRC_W;
                break;
            default:
            	u1AF_SRC_SEL = 0x1;
            	u1AF_2_CF = 0x1;
            	u1FLD_R_EN = E_AF_Y | E_AF_C;
            	u1DRAM_RW_EN = 0x0;
            	break;
    	}
    }
    else
    {
    	u1AF_2_CF = 0x0;
    	u1AF_SRC_SEL = 0x0;
    	u1FLD_R_EN = 0x0;
    	u1DRAM_RW_EN = EN_LM_R; // workaround for system halt due to FLAG_R hold dram.
    }

    u1DramR = (IS_INTERLACE(u1VdpId) || ((_u1DrvDI420T0422En(u1VdpId)) && IS_PROG_DISPMODE(u1VdpId)));
    u1TgMode = _arMDDiPrm[u1VdpId].u1PSCANDisplay ? E_TG_LOCK_SCALER : 
                (u1AF_SRC_SEL ? E_TG_LOCK_INPUT_A_SRC : E_TG_FREE_RUN);
    
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_00, u1AF_2_CF, AF_TO_CF);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_00, u1AF_SRC_SEL, AF_SRC_SEL);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_00, u1DRAM_RW_EN, EN_DRAM_RW);
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_00, u1DramR ? u1FLD_R_EN : 0, ABCD_YC_EN);    
    
    #ifdef CC_SCPOS_3DTV_SUPPORT
    if (_arMDDiPrm[u1VdpId].u1PSCANDisplay)
    {
        MDDI_WRITE_FLD(u1VdpId, MDDI_MC_14, 0x1, R_DMAG2EN_PIX_W_M);
    }
    else
    {
        MDDI_WRITE_FLD(u1VdpId, MDDI_MC_14, ((IS_DI_3D_FPI_MODE(u1VdpId) || IS_DI_3D_I_MODE(u1VdpId)) ? 0 : 1), R_DMAG2EN_PIX_W_M);
    }
    #endif
    
    if (u1DramCOnOff)
    {
        MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_00, u1DramR ? 1 : 0, EN_SRCR);
        MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_00, IS_OVERSAMPLE_OVER_720(u1VdpId), SD_1440_MODE);
        MDDI_WRITE_FLD(u1VdpId, MDDI_OC_02, u1TgMode, OUT_TG_MODE);
      
        if (IS_ECO_IC())
        {
            if(SRM_GetTvMode() == SRM_TV_MODE_TYPE_POP)
            {
                MDDI_WRITE_FLD(VDP_1, MDDI_KC_00, 0, HD_READ);
                MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, 0, EXTRA_PIP_BUF_EN);
            }
            else
            {
            
                #if (defined(CC_SCPOS_3DTV_SUPPORT) && defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))              
                if((u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_TB)
                    || ((SV_TRUE == _bLRInverse) && (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_SBS))
                    || ((SV_TRUE == _bLRInverse) && (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_RD))
                    || (u4DrvTDTV3DModeQuery() == E_TDTV_UI_3D_MODE_TTD))
                {
                    MDDI_WRITE_FLD(VDP_1, MDDI_KC_00, 0, HD_READ);
                    MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, 0, EXTRA_PIP_BUF_EN);
                
                }
                else
                #endif
                {
                    MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, 1, EXTRA_PIP_BUF_EN);
                    MDDI_WRITE_FLD(VDP_1, MDDI_KC_00, 1, HD_READ);
                }
            }
        }
        else // always use extra buffer for sub before ECO
        {
            MDDI_WRITE_FLD(VDP_1, MDDI_GC_01, 1, EXTRA_PIP_BUF_EN);
            MDDI_WRITE_FLD(VDP_1, MDDI_KC_00, 1, HD_READ);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//  Capture Logo Function PSCAN API
////////////////////////////////////////////////////////////////////////////////
void vDrvDIDispInfoForCapLog(UINT8 bPath, DI_DISP_INFO_T* pDispInfo)
{
		UINT32 u4YStartMSBAddr,u4YStartLSBAddr,u4UStartMSBAddr,u4UStartLSBAddr,u4VStartMSBAddr,u4VStartLSBAddr; 
		UINT8  u1BlockSize,u1BitRes;
		UINT16 u2Width, u2Height;
		UINT32 AF_Index;
		UINT8  AF_Bottom;
		
		if (bPath >= VDP_NS)
		{
			return;
		}
	
		u1BitRes = _arMDDiPrm[bPath].u1BitRes;
		u2Width = _arMDDiPrm[bPath].u2Width;
		u2Height = IS_INTERLACE(bPath) ? (_arMDDiPrm[bPath].u2Height/2) : _arMDDiPrm[bPath].u2Height;
		u2Height = IS_420TO422(bPath) ? (((u2Height+1)>>1)<<1) : u2Height;
		u1BlockSize = MDDI_READ_FLD(bPath, MDDI_MULTI_03, DA_BLOCK_SIZE); 
	
		pDispInfo->u1BlockMode = u1BlockSize;
		pDispInfo->u2Width = u2Width;
		pDispInfo->u2Height = u2Height;
		pDispInfo->u1BitRes = u1BitRes;
		pDispInfo->u1ColorMode = _arMDDiPrm[bPath].u1420Mode; //420 or 422
	
		u4YStartMSBAddr = MDDI_READ_FLD(bPath, MDDI_MC_06, DA_ADDR_BASE_MSB_Y);
		u4YStartLSBAddr = MDDI_READ_FLD(bPath, MDDI_MC_07, DA_ADDR_BASE_LSB_Y);
		u4UStartMSBAddr = MDDI_READ_FLD(bPath, MDDI_MC_08, DA_ADDR_BASE_MSB_U);
		u4UStartLSBAddr = MDDI_READ_FLD(bPath, MDDI_MC_09, DA_ADDR_BASE_LSB_U);
		u4VStartMSBAddr = MDDI_READ_FLD(bPath, MDDI_MC_0A, DA_ADDR_BASE_MSB_V);
		u4VStartLSBAddr = MDDI_READ_FLD(bPath, MDDI_MC_0B, DA_ADDR_BASE_LSB_V);
	
		pDispInfo->u4YStartMSBAddr = u4YStartMSBAddr<<11;
		pDispInfo->u4YStartLSBAddr = u4YStartLSBAddr<<11;
		pDispInfo->u4UStartMSBAddr = u4UStartMSBAddr<<11;
		pDispInfo->u4UStartLSBAddr = u4UStartLSBAddr<<11;
		pDispInfo->u4VStartMSBAddr = u4VStartMSBAddr<<11;
		pDispInfo->u4VStartLSBAddr = u4VStartLSBAddr<<11;
			
		MDDI_WRITE_FLD(bPath, MDDI_MC_05, 1, DA_DEBUG); //A Field Index.
		AF_Index = MDDI_READ_FLD(bPath, MDDI_MC_1A, STA_DEBUG);
		pDispInfo->u1AFYUVIndex = AF_Index & 0xFFF;
	
		AF_Bottom = MDDI_READ_FLD(bPath, MDDI_KC_04, CF_BOTTOM); // A=C  0:Top 1:Bottom
		pDispInfo->u1AFBottom = AF_Bottom;
	
		pDispInfo->u1YFrameNum = MDDI_READ_FLD(bPath, MDDI_MULTI_03, DA_FRAME_NUM_Y) + 1;
		pDispInfo->u1CFrameNUm = MDDI_READ_FLD(bPath, MDDI_MULTI_03, DA_FRAME_NUM_U) + 1;
	
		pDispInfo->u1CfgFlipMirror	= 0; //FlipMirror=Normal
		pDispInfo->u1DramFlipMirror = 0; //DramFlipMirror=Normal consequence.
		
		if(bIsScalerInput444(bPath))
		{
			pDispInfo->u1ColorMode = 2;//RGB444
			pDispInfo->u1OutMode = 2; //RGB444
			
			#ifdef CC_FLIP_MIRROR_SUPPORT
			if(u1FlipMirrorConfig != DI_NORMAL_MODE)
			{
				pDispInfo->u1CfgFlipMirror	= u1FlipMirrorConfig;
				pDispInfo->u1DramFlipMirror = 0; //SC Read Flip/Mirror
			}
			#endif		
		}
		else if(IS_INTERLACE(bPath)) //Interlace
		{
			pDispInfo->u1OutMode = 0; //Interlace based field
			
			#ifdef CC_FLIP_MIRROR_SUPPORT
			if(u1FlipMirrorConfig != DI_NORMAL_MODE)
			{
				pDispInfo->u1CfgFlipMirror	= u1FlipMirrorConfig;
				pDispInfo->u1DramFlipMirror = 1; //PSCAN Write Flip/Mirror
			}
			#endif
		}
		else //Progressive
		{
			pDispInfo->u1OutMode = 1; //Progressive based frame
			
			#ifdef CC_FLIP_MIRROR_SUPPORT
			if(u1FlipMirrorConfig != DI_NORMAL_MODE)
			{
				pDispInfo->u1CfgFlipMirror	= u1FlipMirrorConfig;
				if ((IS_PROGRESSIVE(bPath)) && IS_420TO422(bPath))
				{
					pDispInfo->u1DramFlipMirror = 1; //PSCAN Write Flip/Mirror
				}
				else
				{
					pDispInfo->u1DramFlipMirror = 0; //SC Read Flip/Mirror
				}
			}
			#endif		
		}
}


/**
* @brief Set bit resolution
* @param u1VdpId VDP_1/VDP_2
* @param u1BitRes Bit resolution (8 or 10)
*/
void vDrvDISetBitRes(UINT8 u1VdpId, UINT8 u18bit)
{
    vDrvDISetWritePrecision(u1VdpId, u18bit);
    vDrvDISetErrDefusion(u1VdpId, u18bit);
    vDrvNRSetBitSel(u1VdpId,(u18bit == 1) ? NR_BIT_SEL_8BIT : NR_BIT_SEL_10BIT);
}

/**
* @brief Thread Pending Until PSCAN Interrupt Finished
* @param None
*/
void vDrvWaitDIVSyncSema(void)
{
    VERIFY (x_sema_lock(_hDIPQIsrSema, X_SEMA_OPTION_WAIT) == OSR_OK);
}

/**
* @brief Set demo mode active region
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff SV_ON/SV_OFF
* @param u1Region SV_LEFT/SV_RIGHT
* @param u2Center Center line position
*/
void vDrvDIDemo(UINT8 u1OnOff, UINT8 u1Region, UINT16 u2Center)
{
    // Not support in MT5365
}

/**
* @brief Set total number of lines to be clipped for ONLY DTV case
* @param u1VdpId VDP_1/VDP_2
* @param u2VClip Total number of lines to be clipped
*/
void vDrvDISetDtvClip(UINT8 u1VdpId, UINT16 u2DTVClip)
{
    if (IS_MPEG(u1VdpId))
    {
    	MDDI_WRITE_FLD(u1VdpId, MDDI_SC_02, (_arMDDiPrm[u1VdpId].u2VClip + u2DTVClip), Y_SHIFT);
    	_arMDDiPrm[u1VdpId].u2DTVClip = u2DTVClip;
    }
}

/**
 * @brief Return the minimum horizontal width that PSCAN can support
 * @param u1VdpId VDP_1/VDP_2
 * @return u4MiniInputWidth The minimum horizontal width that PSCAN can support
 */
UINT32 u4DrvDIMiniInputWidth(UINT8 u1VdpId)
{
    //The minimum width is defined as 360 for the following reasons
    //a. For normal signal, the test result for 480i/576i/1080i is 290 (1W3R)
    //b. For non-standard signal, the test result for 480i/576i is 346 (1W3R)
    return 180;
}

/**
 * @brief Return the maximum horizontal width that PSCAN can support
 * @param u1VdpId VDP_1/VDP_2
 * @return u4MaxInputWidth The maximum horizontal width that PSCAN can support
 */
UINT32 u4DrvDIMaxInputWidth(UINT8 u1VdpId)
{
    UINT32 u4MaxInputWidth;
    SRM_TV_MODE_T eMode = SRM_GetTvMode();

    if (eMode == SRM_TV_MODE_TYPE_POP)
    {
        if (IS_ECO_IC())
        {
            u4MaxInputWidth = 960;
        }
        else
        {
            u4MaxInputWidth = (u1VdpId == VDP_1) ? 960 : 720;
    	}
    }
    else if ((eMode == SRM_TV_MODE_TYPE_PIP) && (u1VdpId == VDP_2))
    {
    	u4MaxInputWidth = 720;
    }
    else
    {
    	u4MaxInputWidth = 1920;
    }

    return u4MaxInputWidth;
}

/**
* @brief Turn on/off trick mode
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff SV_ON/SV_OFF
* @warning This should be invoked by TVD only
*/
void vDrvDITrickModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff)
{
    MDDI_WRITE_FLD(u1VdpId, MDDI_KC_00, (u1OnOff ? 1 : 0), AUTO_TRICK_ON);

    //Trick mode is an one-field-only mode. PSCAN has to work in 2D mode under this case
    vDrvDISetInterpMode(u1VdpId, (u1OnOff ? E_DI_YCBOB_0W0R : _arMDDiPrm[u1VdpId].u1DataMode));
}

/**
* @brief Turn on/off frame encode mode
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff SV_ON/SV_OFF
* @param u1BtmFirst SV_TRUE/SV_FALSE
* @warning This should be invoked by B2R only
*/
void vDrvDIFrmEncModeOnOff(UINT8 u1VdpId, UINT8 u1OnOff, UINT8 u1BtmFirst)
{
    //TODO: check if this is necessary in MT5387
    #if 0
    MDDI_WRITE_FLD(u1VdpId, MDDI_MIXER_03, (u1OnOff ? 1 : 0), FRAME_ENCODE_C);
    MDDI_WRITE_FLD(u1VdpId, MDDI_MIXER_03, (u1BtmFirst ? 1 : 0), BOTTOM_FIRST);
    #endif
}

UINT8 IsDTVSource(UINT8 u1Path)
{
    UINT8 u1SrcTypTmg = bDrvVideoGetSourceTypeTiming(u1Path);

	if(SOURCE_TYPE_TIMING_DTV_SD_I_50 <= u1SrcTypTmg &&
		u1SrcTypTmg <= SOURCE_TYPE_TIMING_DTV_1080P_60)
	{
		return SV_TRUE;
	}
	return SV_FALSE;
}
/**
* @brief DI Frame Repeat
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff On/Off
* @warning This should be invoked by B2R only
*/
void vDrvDIFrameRepeat(UINT8 u1VdpId, UINT8 u1OnOff)
{      	
    if (u1VdpId >= VDP_NS)
    {
    	return;
    }
    
    if(fgMainMuteOn())
    {
        u1OnOff=SV_OFF;
    }

    //vDrvNRSet3DNROnOffFreeze(((u1OnOff == SV_ON) ? SV_OFF : SV_ON));    
    vDrvDIFilmModeFreeze(u1VdpId, (!u1OnOff));
    vDrvDISetAutoStop(u1VdpId,u1OnOff);

	//To Solve CR[DTV00334227] DTV 1080i flutter issue.
	if(IsDTVSource(u1VdpId) && 
		IS_INTERLACE(u1VdpId) && 
		(!IS_SD_HEIGHT(u1VdpId)))
	{		
		vDrvDISetFldPrediction(u1VdpId, SV_OFF);	
		vDrvDISetReSyncOnOff(u1VdpId, SV_ON);		
	}
	else
	{
		vDrvDISetFldPrediction(u1VdpId, SV_ON);	
		vDrvDISetReSyncOnOff(u1VdpId, SV_OFF);		
	}
	
}

/**
* @brief Enable/disable bob mode
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff SV_ON/SV_OFF
*/
void vDrvDISetBob(UINT8 u1VdpId, UINT8 u1OnOff)
{
    E_DI_DMODE bMode = u1OnOff ? E_DI_YCBOB_0W0R : _arMDDiPrm[u1VdpId].u1DataMode;
    
    _arMDDiPrm[u1VdpId].u1Bob = u1OnOff;    
    vDrvDISetInterpMode(u1VdpId, bMode);
}

/**
* @brief Return current bob mode status
* @param u1VdpId VDP_1/VDP_2
* @return SV_ON/SV_OFF
*/
UINT8 u1DrvDIGetBob(UINT8 u1VdpId)
{
    return _arMDDiPrm[u1VdpId].u1Bob;
}

/**
* @brief Enable/disable PSCAN freeze mode
* @param u1VdpId VDP_1/VDP_2
* @param u1OnOff SV_ON/SV_OFF
*/
void vDrvDIFreezeOnOff(UINT8 u1VdpId, UINT8 u1OnOff)
{
    if (u1OnOff == SV_ON)
    {
        if (!IS_ECO_IC())
        {
            MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_01, 1, FREEZE_SEL_PXLR);
            vDrvDIFilmModeFreeze(u1VdpId, SV_OFF);
        }
        vDrvNRSet3DNROnOffFreeze(!u1OnOff);
    }
    else
    {
        _arMDDiPrm[u1VdpId].u1FreezeCnt = 0x8;
    }
    
    MDDI_WRITE_FLD(u1VdpId, MDDI_MULTI_01, u1OnOff, FREEZE_ON);
}

/**
* @brief Check if freeze function works under the current configuration
* @param u1VdpId VDP_1/VDP_2
* @return SV_TRUE/SV_FALSE
*/
UINT8 u1DrvDIFreezable(UINT8 u1VdpId)
{
    return (((_arMDDiPrm[u1VdpId].u1DataMode == E_DI_FULL_1W4R) ||
            (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_CBOB_1W4R) ||
            (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_YCBOB_1W1R) ||
            (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_FLIP_MIRROR_1W4R) ||
            (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_FLIP_MIRROR_CBOB_1W4R) ||
            (_arMDDiPrm[u1VdpId].u1DataMode == E_DI_FLIP_MIRROR_YCBOB_1W1R)) ? SV_TRUE : SV_FALSE);
}

/**
* @brief Get PSCAN delay
* @param u1VdpId VDP_1/VDP_2
* @return PSCAN delay value
*/
UINT8 u1DrvDIGetFrameDelay(UINT8 u1VdpId)
{
    UINT8 u1PSCANDisplay = _arMDDiPrm[u1VdpId].u1PSCANDisplay;

    switch (_arMDDiPrm[u1VdpId].rFbmPool.u4Mode & 0x00070000)
    {
    	case FBM_POOL_MODE_MDDI_FULL:
    	    return (u1PSCANDisplay ? 3 : 2);
    	case FBM_POOL_MODE_MDDI_BOB:
    	    return (u1PSCANDisplay ? 1 : 0);
    	case FBM_POOL_MODE_MDDI_CBOB:
    	    return (u1PSCANDisplay ? 3 : 2);
    	default:
            return 0;
    }
}

/**
* @brief PSCAN 3DTV callback function
* @param u4SramMode sram mode
*/
void vDrvDITDTVCb(void)
{ 
    _u4TdTvDIType[VDP_1] = u4DrvTDTVDIModeQuery();
}

/**
* @brief PSCAN SRM callback function
* @param u4Arg1 VDP_1/VDP_2
* @param u4Arg2 configuration
* @param u4SramMode sram mode
*/
void vDrvDISrmCb(UINT32 u4Arg1, UINT32 u4Arg2, UINT32 u4PDSize, UINT32 u4SramMode)
{
    FBM_POOL_T* prPool;

    vDrvVideoFixedPreScaler((UCHAR)u4Arg1, SV_TRUE, 1);

    if (u4Arg1 == VDP_1)
    {
        prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_MAIN, NULL);
    }
    else
    {        
        u4Arg1 = VDP_2;
        prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_PIP, NULL);
    }
    
    if (prPool != NULL)
    {
        _vDrvSetDIBufferInfo((UINT8)u4Arg1, u4Arg2, prPool->u4Addr, prPool->u4Size, prPool->u4Width, prPool->u4Height);
    }
    else
    {
        LOG(1, "vDrvDISrmCb: Fail to get pool\n");
    }
}

/**
* @brief Set pscan output clock
* @param bPath VDP_1/VDP_2
* @param eType clock type
*/
void vDrvDISetPSCANClock(UINT8 u1VdpId, DI_OUTPUT_CLOCK_TYPE eType)
{
    vIO32WriteFldAlign(CKGEN_DISP_CKCFG, eType, FLD_PS_DISP_CLK_SEL);
}

/**
* @brief Set NR Data mode
* @param u1VdpId VDP_1/VDP_2
* @param u1DataMode datamode
*/
void vDrvDISetNRDebugDataMode(UINT8 u1VdpId, E_NR_DMODE u1DataMode)
{
    if ((u1VdpId != VDP_1) && (u1VdpId != VDP_2))
    {
        _arMDDiPrm[VDP_1].u1DebugNRDataMode = E_DI_NR_UNKNOWN;        
        _arMDDiPrm[VDP_2].u1DebugNRDataMode = E_DI_NR_UNKNOWN;                
    }
    else
    {    
        _arMDDiPrm[u1VdpId].u1DebugNRDataMode = u1DataMode;
    }
}

void vDrvDIModeChangeDone(UINT8 u1VdpId)
{
}

/**
* @brief FPGA CRC Test
* @param bPath VDP_1/VDP_2
* @param bMode
* @param u2Arg
*/
void vDrvDISetCrc(UINT16 u2Mode, UINT16 u2Arg)
{
    if (u2Mode == 0)
    {
        _u1CrcLog = (UINT8)u2Arg;
        MDDI_WRITE_FLD(VDP_1, MDDI_OC_09, 0x1, OUT_CRC_SRC_ONLY);
        MDDI_WRITE_FLD(VDP_1, MDDI_OC_09, 0x0, OUT_CRC_V_MASK);
        MDDI_WRITE_FLD(VDP_1, MDDI_SC_06, 0x0, SRCW_CRC_V_MASK);
    }
    else if (u2Mode == 1)
    {
        vDrvDIEnableISR(VDP_1, SV_OFF);
        vDrvDIEnableISR(VDP_1, SV_ON);
    }
}

UINT16 u2DrvDIGetVcnt(UINT8 u1VdpId)
{
    return _u2Vcnt;
}

void vDrvDITVDTrickModeInit(void)
{
	MDDI_WRITE32_MSK(VDP_1, MDDI_KC_00, 0, AUTO_TRICK_ON);
   	MDDI_WRITE32_MSK(VDP_2, MDDI_KC_00, 0, AUTO_TRICK_ON);
}
/**
* @brief Initialize PSCAN register values
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDIInit(UINT8 u1VdpId)
{
    _vDrvDIInitClock(u1VdpId);    
    vDrvDISetDoubleBuffer(u1VdpId, SV_OFF);
    _vDrvDIReset(u1VdpId, SV_ON);

    // Reg 3DTV Callback Function
    fpDrvRegisterTDTVModeChangeCB(E_TDTV_CB_DI, (vTDTVModeChangeCB)vDrvDITDTVCb);
    
    vDrvDIConfigFlipMirror();
    //Initialize PSCAN structure
    _arMDDiPrm[u1VdpId].u1OnOff = SV_OFF;
    _arMDDiPrm[u1VdpId].u2Width = 720;
    _arMDDiPrm[u1VdpId].u2Height = 576;
    _arMDDiPrm[u1VdpId].u1FreezeCnt = 0;
    _arMDDiPrm[u1VdpId].u1StatusCnt = 0;
    _arMDDiPrm[u1VdpId].u1DebugIndex = 0;
    _arMDDiPrm[u1VdpId].u1DebugDataMode = E_DI_UNKNOWN;
    _arMDDiPrm[u1VdpId].u1DebugNRDataMode = E_DI_NR_UNKNOWN;
    _arMDDiPrm[u1VdpId].u1DebugPSCANDisplay = DI_DISP_UNKNOWN;
    _arMDDiPrm[u1VdpId].u4ProgDispClip = 0;
    _arMDDiPrm[u1VdpId].u4TdTvDIType = E_TDTV_DI_2D;
    #if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
    _arMDDiPrm[u1VdpId].u1Support3DPR = SV_OFF;
    #endif    
    MDDI_WRITE_FLD(u1VdpId, MDDI_MC_13, 0x2, DA_READ_REQ_SPACING);

    #if PREPROC_ENABLE_SYNC_REGEN
    _arMDDiPrm[u1VdpId].u2HClip = 0xFFFF;
    _arMDDiPrm[u1VdpId].u2VClip = 0xFFFF;
    #endif

    #ifdef CC_SUPPORT_H264
    _vDrvDISupportH264Init();
    #endif
    
    vDrvDIInitQuality(u1VdpId);
    _vDrvDIInitISR();
     vDrvDIEnableISR(u1VdpId, SV_ON);
    _vDrvDIReset(u1VdpId, SV_OFF);

    vDrvDITVDTrickModeInit();

}


void vDrvDIRecoverOnOff(UINT8 u1VdpId)
{
	vDrvDIOnOff(u1VdpId, _arMDDiPrm[u1VdpId].u1OnOff);
}
/**
* @brief PSCAN main loop
* @param u1VdpId VDP_1/VDP_2
*/
void vDrvDIChkImpl(UINT8 u1VdpId)
{ 
    if (u1VdpId >= VDP_NS)
    {
        return;
    }
    
    if ((IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_MAIN) && (u1VdpId == VDP_1))
    	|| (IO32ReadFldAlign(ADAPTIVE_PSCAN_REG, ADAPTIVE_PSCAN_SUB) && (u1VdpId == VDP_2)))
    {
        if (_u1DrvDIModeChange(u1VdpId))
        {               
            vDrvDISetAutoStop(u1VdpId, SV_OFF);
            
            _vDrvVideoSetMute(MUTE_MODULE_MDDI01, u1VdpId, MDDI_MUTE_PERIOD, TRUE);

            _vDrvDIReset(u1VdpId, SV_ON); 

            _vDrvDISetParam(u1VdpId);
            
            _vDrvDIReset(u1VdpId, SV_OFF);     

            vDrvDISetDramCtrl(u1VdpId, SV_ON);
            
            vDrvDIOnOff(u1VdpId, _arMDDiPrm[u1VdpId].u1OnOff);

            vDrvDISetDoubleBuffer(u1VdpId, (_arMDDiPrm[u1VdpId].u1OnOff ? SV_ON : SV_OFF));

            #ifdef CC_DOWMBWMODE
            if (u1DowmBWMode && (u1VdpId == VDP_1))
            {
                vDrvDISeamlessSwtich(SV_ON);
            }
            #endif

            #if PREPROC_ENABLE_SYNC_REGEN
            vDrvPreprocSetSyncRegen(u1VdpId,_arMDDiPrm[u1VdpId].u2HClip,
                                            _arMDDiPrm[u1VdpId].u2VClip,
                                            _arMDDiPrm[u1VdpId].u2Width,
                                            _arMDDiPrm[u1VdpId].u2Height);
            #endif
        }

        if (_arMDDiPrm[u1VdpId].u1OnOff == SV_ON)
        {
            vDrvDIChkStatus(u1VdpId);
            _vDrvDIFreezePatch(u1VdpId);
            _vDrvCounterIncrease(u1VdpId);    
    	}

    	if (_bTVMode != SRM_GetTvMode())
    	{
    	    _bTVMode = SRM_GetTvMode();
    	    _vDrvDISetFifoShareMode(u1VdpId);
    	}

        if (_bUpdate)
        {   
            _u1Cnt--;
            if((_u1Cnt == 0) && (_bFifoShareMode != MDDI_READ_FLD(VDP_1, MDDI_MC_05, DA_FIFO_SHARE_MODE)))
            {
                _bUpdate = FALSE;
                MDDI_WRITE_FLD(VDP_1, MDDI_MC_05, _bFifoShareMode, DA_FIFO_SHARE_MODE);
            }
        }

        #ifdef CC_DOWMBWMODE
        if (u1UpdateCnt && (u1VdpId == VDP_1))
        {
            u1UpdateCnt--;
            if (u1UpdateCnt == 0)
            {
                vDrvDISetInterpMode(VDP_1, _arMDDiPrm[VDP_1].u1DataMode);
                vDrvNRSet3DNRUIOnOff(bNROnOff);
            }
            }
        #endif    	
    }
    
    //Quality main loop
    #ifndef __MODEL_slt__
    vDrvDIQtyMainLoop(u1VdpId);
    #endif
}

UINT16 wDrvDIGetWidth(void)
{
    return MDDI_READ_FLD(SV_VP_MAIN, MDDI_SC_00, HDEW);
}

UINT32 u4DrvDIGetProgDispmodeClip(UINT8 u1VdpId)
{
    if (u1VdpId >= VDP_NS)
    {
        return 0;
    }

    if ((_u4DrvDIGetFlipMirror(u1VdpId) == DI_NORMAL_MODE) || 
        (_u4DrvDIGetFlipMirror(u1VdpId) == DI_FLIP_ON_MODE))
    {
        return 0;
    }
    else
    {
        return _arMDDiPrm[u1VdpId].u4ProgDispClip;
    }
}
#ifdef CC_DOWMBWMODE
UINT8 vDrvDIGetReadEn(void)
{
    UINT8 u1FLD_R_EN = 0;
    switch (_arMDDiPrm[VDP_1].u1DataMode)
    {
        case E_DI_FULL_1W3R:			
        case E_DI_FULL_1W4R:
        case E_DI_FLIP_MIRROR_1W4R:
        case E_DI_CBOB_1W3R:            
        case E_DI_CBOB_1W4R:
        case E_DI_FLIP_MIRROR_CBOB_1W4R:
            u1FLD_R_EN = E_CF_Y | E_CF_C;
            break;
            
        case E_DI_YCBOB_0W0R:			
            u1FLD_R_EN = 0x0;
            break;
            
        case E_DI_YCBOB_1W1R:
        case E_DI_FLIP_MIRROR_YCBOB_1W1R:
            u1FLD_R_EN = E_AF_Y | E_AF_C;
            break;
    }
    return u1FLD_R_EN;
}

void vDrvDISeamlessSwtich(UINT8 u1OnOff)
{
    BOOL bReadEn = (_arMDDiPrm[VDP_1].u1NRDataMode == E_DI_NR_OFF) ? SV_OFF : SV_ON;

    if (!IS_SD_HEIGHT(VDP_1)) // only apply save bandwidth mode to HD timing
    {    
        if (u1OnOff)
        {
            vDrvNRSet3DNRUIOnOff(SV_OFF); // force 3DNR off
            MDDI_WRITE_FLD(VDP_1, MDDI_SC_09, 0x0, DA_NR_READ_ENABLE); // pscan nr read enable
            MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_20, 0x3, IF_FIX_BOB);  
            MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_20, 0x3, IF_FIX_MOTION); 
            MDDI_WRITE_FLD(VDP_1, MDDI_FUSION_20, 0x88, IF_FORCE_MOTION);
            MDDI_WRITE_FLD(VDP_1, MDDI_KC_00, vDrvDIGetReadEn(), ABCD_YC_EN);
            if (!IS_PROG_DISPMODE(VDP_1))
            {
                vDrvDISetWritePrecision(VDP_1, 1); // force 8 bit
            }
            u1UpdateCnt = 0;
        }
        else
        {
            vDrvDISetWritePrecision(VDP_1, (_arMDDiPrm[VDP_1].u1BitRes == 8) ? 1 : 0);
            vDrvDISetDramCtrl(VDP_1, SV_ON);
            MDDI_WRITE_FLD(VDP_1, MDDI_SC_09, bReadEn, DA_NR_READ_ENABLE); // pscan nr read enable
            bNROnOff = bReadEn;
            u1UpdateCnt = 5;
        }
    }
}
#endif

