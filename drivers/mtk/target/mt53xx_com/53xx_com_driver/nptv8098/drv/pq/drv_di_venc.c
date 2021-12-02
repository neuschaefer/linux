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

/*-----------------------------------------------------------------------------
 *
 * MIB Help 264 Decoder to Write DRAM
 *
 *---------------------------------------------------------------------------*/


////////////////////////////////////////////////////////////////////////////////
// Include files
////////////////////////////////////////////////////////////////////////////////

#include "drv_di.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "di_debug.h"
#include "hw_di.h"
#include "hw_ycproc.h"
#include "sv_const.h"
#include "vdo_misc.h"
#include "video_def.h"
#include "video_timing.h"
#include "drv_ycproc.h"

#include "x_ckgen.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_timer.h"
#include "x_util.h"
#include "x_hal_5381.h"

#include "fbm_drvif.h"

#include "drv_tdnavi.h"

#ifdef CC_SUPPORT_VENC
#include "venc_drvif.h"
#include "venc_util_drvif.h"
#include "fbm_drvif.h"
#if 0//def CC_ATV_PVR_SUPPORT
#include "drv_vbi.h"
#endif


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef enum
{
    MDDI_NORMAL_MODE,
    MDDI_H264_MODE,
    MDDI_OSD_BLEND_MODE
}DI_WRITE_MODE;

typedef enum{
    E_DI_WAIT_BUF,
    E_DI_WRITING,
    E_DI_DONE	
} E_DI_VENC_STATE;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static E_DI_VENC_STATE _u1VencSta;
static BOOL _fgStopSendFrm;
static UINT8 _u1YUVMode;
static VENC_BUFMGR_BASE_T *_ptDIVEncFrmBuffMgr;
static VENC_BUFMGR_BASE_T *_ptDIVEncResizeFrmBuffMgr;
static VENC_HANDLE _hDIVEnc;
static VENC_IN_PIC_T _tDIVEncIn;
static VENC_FRM_T _tDIVEncFrm;
static VENC_FILTER_T * _ptDIVEncFilter;
static UINT32 _u4WriteProtectLow, _u4WriteProtectHigh;
static UINT8 u1HalfFps;

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

EXTERN void vDrvDISetDoubleBuffer(UINT8 u1VdpId, UINT8 u1OnOff);

// temporary use extern directly, dsp should provide header file later
//#if defined(CC_AUD_DATA_UPLOAD_SUPPORT)
EXTERN UINT32 _AUD_ReadPsrStc1(void);
//#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/**
 * @brief Send frame to video encoder routine
 */
void vDrvDIVEncISR(void)
{    
    if (MDDI_READ_FLD(VDP_1, MCVP_H264_00, DA_WRITE_MODE) == 0) // normal mode
    {
        return;
    }

    if ((IS_INTERLACE(VDP_2) && (MDDI_READ_FLD(VDP_2, MCVP_KC_19, CF_BOTTOM)) == 0))
    {
        // donothing at bottom field
        return;
    }
    
    if(_u1VencSta == E_DI_WRITING)
    {
       _u1VencSta = E_DI_DONE;
       if (u1HalfFps)
       {
           return;
       }    
    }

    
    if (_fgStopSendFrm)
    {
        vDrvDISetH264WriteMode(MDDI_NORMAL_MODE);
        _fgStopSendFrm = FALSE;
        return;
    }

    if (VENC_NULL_HANDLE == _hDIVEnc)
    {
        LOG(3, "%s: _hDIVEnc is NULL!!\n", __FUNCTION__);
        return;
    }

    if (!_ptDIVEncFrmBuffMgr)
    {
        LOG(3, "%s: _ptDIVEncFrmBuffMgr is NULL!!\n", __FUNCTION__);
        return;
    }


    if (_u1VencSta == E_DI_DONE)
    {

#if 0//def CC_ATV_PVR_SUPPORT
        if (!VBI_PVRRetrieveCCData(&_tDIVEncIn.rCC_Data))
        {
            _tDIVEncIn.rCC_Data.u4Size = 0;
        }
        if (!VBI_PVRRetrieveAFDData(&_tDIVEncIn.rAFD_Data))
        {
            _tDIVEncIn.rAFD_Data.u4Size = 0;
        }
#endif

        if (VENC_OK != _VENC_EncFrameAsync(_hDIVEnc, &_tDIVEncIn, NULL))
        {
            LOG(0, "enc frame fail\n");
        }
        else
        {
#ifdef VENC_FRAME_CTRL_BY_DRV
            if (_ptDIVEncFilter)
            {                   
                _VENC_Filter_Update(_ptDIVEncFilter);
            }
#endif
        }
    }


    if(_u1VencSta == E_DI_WAIT_BUF || _u1VencSta == E_DI_DONE)
    {
#ifdef CC_AUD_DATA_UPLOAD_SUPPORT
        _tDIVEncIn.u4Pts = _AUD_ReadPsrStc1();
#endif

#ifdef VENC_FRAME_CTRL_BY_DRV                       
        if (!_ptDIVEncFilter || (_VENC_Filter_Do(_ptDIVEncFilter, _tDIVEncIn.u4Pts)))
#endif
        {
            UINT32 u4Ret = VENC_BUFMGR_FAIL;

            u4Ret = (_ptDIVEncResizeFrmBuffMgr)
                ? _BUFMGR_OBJ_Get(_ptDIVEncResizeFrmBuffMgr, (VOID *)&_tDIVEncFrm, VENC_BUFMGR_NOWAIT)
                : _BUFMGR_OBJ_Get(_ptDIVEncFrmBuffMgr, (VOID *)&_tDIVEncFrm, VENC_BUFMGR_NOWAIT);

            if (VENC_BUFMGR_OK == u4Ret)
            {
                // write address should within range [_u4WriteProtectLow, _u4WriteProtectHigh)
                ASSERT(_tDIVEncFrm.u4YAddr >= _u4WriteProtectLow);
                ASSERT(_tDIVEncFrm.u4UVAddr >= _u4WriteProtectLow);
                ASSERT(_tDIVEncFrm.u4YAddr < _u4WriteProtectHigh);
                ASSERT(_tDIVEncFrm.u4UVAddr < _u4WriteProtectHigh);

                MDDI_WRITE_FLD(VDP_1, MCVP_H264_01, (_tDIVEncFrm.u4YAddr >> 4), DA_H264_ADDR_BASE_Y);
                MDDI_WRITE_FLD(VDP_1, MCVP_H264_02, (_tDIVEncFrm.u4UVAddr >> 4), DA_H264_ADDR_BASE_C);  
                MDDI_WRITE_FLD(VDP_2, MCVP_MULTI_00, SV_ON, EN_SRCW);
                _tDIVEncIn.u4YAddr = _tDIVEncFrm.u4YAddr;
                _tDIVEncIn.u4CAddr = _tDIVEncFrm.u4UVAddr;
                _tDIVEncIn.fgRzSrc = (_ptDIVEncResizeFrmBuffMgr)? TRUE : FALSE;
                _u1VencSta = E_DI_WRITING;
#ifdef DI_VENC_DUMP
                {
                    #define DI_VENC_DUMP_SZ (1920 * 1088)
                    static UINT32 u4FrmCnt = 0;
                    LOG(5, "d.save.b D:\\%d.y.raw 0x%08x--0x%08x\n", u4FrmCnt, _tDIVEncFrm.u4YAddr, _tDIVEncFrm.u4YAddr + DI_VENC_DUMP_SZ-1);
                    LOG(5, "d.save.b D:\\%d.c.raw 0x%08x--0x%08x\n", u4FrmCnt, _tDIVEncFrm.u4UVAddr, _tDIVEncFrm.u4UVAddr + DI_VENC_DUMP_SZ/2-1);
                    u4FrmCnt++;
                 }
#endif            
            }
            else
            {
                LOG(1, "no buffer!!!\n");
                _u1VencSta = E_DI_WAIT_BUF;
                MDDI_WRITE_FLD(VDP_2, MCVP_MULTI_00, SV_OFF, EN_SRCW);
            }
        }

    }
}

/**
 * @brief H.264 initialize
 */
void vDrvDISupportH264Init(void)
{
    _ptDIVEncFrmBuffMgr = NULL;
    _ptDIVEncResizeFrmBuffMgr = NULL;
    _ptDIVEncFilter = NULL;
    _hDIVEnc = VENC_NULL_HANDLE;
    vDrvDIStopSendBufToVENC(FALSE);
    _u1VencSta = E_DI_WAIT_BUF;
    vDrvDISetDoubleBuffer(VDP_2, SV_ON);
    UNUSED(_ptDIVEncFilter);

    if (bDrvVideoGetRefreshRate(VDP_2) > 30 && IS_PROGRESSIVE(VDP_2))
    {
        u1HalfFps = TRUE;
    }
    else
    {
        u1HalfFps = FALSE;
    }
}


/**
 * @brief Set H.264 encoder handle
 */
void vDrvDISetVEncHandle(void *handle, void *ptFrmBuffMgr, void *ptResizeFrmBuffMgr, void *ptFilterPts)
{
    _hDIVEnc = (VENC_HANDLE)handle;
    _ptDIVEncFrmBuffMgr = (VENC_BUFMGR_BASE_T *)ptFrmBuffMgr;
    _ptDIVEncResizeFrmBuffMgr = (VENC_BUFMGR_BASE_T *)ptResizeFrmBuffMgr;
    _ptDIVEncFilter = (VENC_FILTER_T *)ptFilterPts;
}


/**
 * @brief Disable send buffer to VENC
 **/
void vDrvDIStopSendBufToVENC(BOOL bFlag)
{
    _fgStopSendFrm = bFlag;
    vIO32WriteFldAlign(ADAPTIVE_PSCAN_REG, bFlag, ADAPTIVE_PSCAN_SUB); 
}

/**
 * @brief Set H.264 write dram mode
 * @param bMode Write format
 */
void vDrvDISetH264WriteMode(UINT8 bMode)
{
    FBM_POOL_T *prFbmPool;

    switch (bMode)
    {
    case MDDI_NORMAL_MODE:
        vDrvDISetWritePrecision(VDP_2, ((_arMDDiPrm[VDP_2].u1BitRes == 8) ? 1 : 0));
        MDDI_WRITE_FLD(VDP_2, MCVP_KC_00, 0x8, SRCW_V_BGN);
        MDDI_WRITE_FLD(VDP_1, MCVP_H264_00, 0x0, DA_WRITE_MODE);
        MDDI_WRITE_FLD(VDP_2, MCVP_KC_1E, (!_arMDDiPrm[VDP_2].u1420Mode), M422);
        MDDI_WRITE_FLD(VDP_2, MCVP_DRAM_06, _u1YUVMode, DA_MODE_YUV);
        MDDI_WRITE_FLD(VDP_2, MCVP_DRAM_06, 0x1, DA_SNAKE_MAPPING);
        MDDI_WRITE_FLD(VDP_2, MCVP_DRAM_0F, _arMDDiPrm[VDP_2].rFbmPool.u4Addr >> 11, DA_WADDR_LO_LIMIT);
        MDDI_WRITE_FLD(VDP_2, MCVP_MULTI_00, SV_ON, EN_SRCW);
        MDDI_WRITE_FLD(VDP_2, MCVP_DRAM_0E, (_arMDDiPrm[VDP_2].rFbmPool.u4Addr + _arMDDiPrm[VDP_2].rFbmPool.u4Size) >> 11, DA_WADDR_HI_LIMIT);   
        break;

    case MDDI_H264_MODE:
        prFbmPool = FBM_GetPoolInfo((UCHAR)FBM_POOL_TYPE_VENC);
        _u4WriteProtectLow = prFbmPool->u4Addr;
        _u4WriteProtectHigh = _u4WriteProtectLow + prFbmPool->u4Size;
        //_fgFrmRdy = FALSE;
        _fgStopSendFrm = FALSE;
        _tDIVEncIn.u4Pts = 0;
        #ifdef VENC_FRAME_CTRL_BY_DRV           
        if (_ptDIVEncFilter)
        {           
            _VENC_Filter_Reset(_ptDIVEncFilter);
        }
        #endif
        _u1YUVMode = MDDI_READ_FLD(VDP_2, MCVP_DRAM_06, DA_MODE_YUV);
        vDrvDISetWritePrecision(VDP_2, 0x1); // force 8 bit
        MDDI_WRITE_FLD(VDP_2, MCVP_KC_00, 0xe, SRCW_V_BGN);
        MDDI_WRITE_FLD(VDP_1, MCVP_H264_00, (IS_INTERLACE(VDP_2) ? 0 : 1), DA_H264_FRAME_MODE); // force field mode
        MDDI_WRITE_FLD(VDP_1, MCVP_H264_00, bMode, DA_WRITE_MODE); // set write mode
        MDDI_WRITE_FLD(VDP_2, MCVP_KC_1E, 0x0, M422); // force 420 mode
        MDDI_WRITE_FLD(VDP_2, MCVP_DRAM_06, MIB_YUV_MODE_YC, DA_MODE_YUV); // force Y/C mode
        MDDI_WRITE_FLD(VDP_2, MCVP_DRAM_06, 0x0, DA_SNAKE_MAPPING); // force raster scan mapping
        MDDI_WRITE_FLD(VDP_2, MCVP_DRAM_0F, _u4WriteProtectLow >> 11, DA_WADDR_LO_LIMIT);
        MDDI_WRITE_FLD(VDP_2, MCVP_DRAM_0E, _u4WriteProtectHigh >> 11, DA_WADDR_HI_LIMIT);    
        break;

    case MDDI_OSD_BLEND_MODE:
        break;
    default:
        break;
    }
}


#endif
