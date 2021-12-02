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

UINT32 DiReg06, DiReg07, DiReg12, DiReg16;

void vDrvDIDumpInit(UINT8 bPath)
{
    UINT32 u4SubOffset = (bPath == VDP_2) ? 0x800 : 0;
	UINT8 bIsInterlace = MDDI_READ_FLD(bPath, MCVP_KC_00, I2P_EN);
	UINT32 u4SrcWidth = MDDI_READ_FLD(bPath, MCVP_KC_0A, HDEW);
    UINT32 u4SrcHeight = MDDI_READ_FLD(bPath, MCVP_KC_0A, VDEW);
    UINT32 u4DumpSize = u4SrcWidth*u4SrcHeight*4;
    FBM_POOL_T* prPool;    
    
    if (bIsInterlace || (bPath == VDP_2)) // sub path always interlace, progresive will bypass Pscan
    {
        u4SrcHeight = u4SrcHeight/2;
    }

    LOG(4, "PSCAN Dump Start \n");    
    LOG(4, "W %d  H %d  Size %d \n", u4SrcWidth, u4SrcHeight, u4DumpSize);
    
    prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_PQ_TOOL, NULL);
        
    if (u4DumpSize > prPool->u4Size)
    {
        LOG(0, "PSCAN Dump buffer too small  %d  %d\n", u4DumpSize, prPool->u4Size);
        vRegWriteFldAlign(SWRW_04, DUMP_ENG_NONE, DUMP_CTRL_ENG_SEL);
        vRegWriteFldAlign(SWRW_04, DUMP_STATE_INIT, DUMP_CTRL_STATE);
        return;
    }
    
    vRegWrite4B(SWRW_01, prPool->u4Addr);
    vRegWrite4B(SWRW_02, prPool->u4Size);
    vRegWriteFldAlign(SWRW_03, u4SrcWidth, DUMP_CTRL_WIDTH);
    vRegWriteFldAlign(SWRW_03, u4SrcHeight, DUMP_CTRL_HEIGHT);
    vRegWriteFldAlign(SWRW_03, DUMP_FMT_YCBCR_444_10BIT, DUMP_CTRL_FORMAT);    
    vRegWriteFldAlign(SWRW_04, u4DumpSize, DUMP_CTRL_SIZE); 
    
	DiReg12 = u4RegRead4B(MCVP_DRAM_12+u4SubOffset);
	DiReg06 = u4RegRead4B(MCVP_DRAM_06+u4SubOffset);
    DiReg07 = u4RegRead4B(MCVP_DRAM_07+u4SubOffset);
	DiReg16 = u4RegRead4B(MCVP_DRAM_16+u4SubOffset);	

	MDDI_WRITE_FLD(bPath, MCVP_DRAM_12, 0, DA_NR_SNAKE_MAPPING);
	MDDI_WRITE_FLD(bPath, MCVP_DRAM_06, 0, DA_SNAKE_MAPPING);
	MDDI_WRITE_FLD(bPath, MCVP_DRAM_16, 0, DA_SC_SNAKE_MAPPING);    
    MDDI_WRITE_FLD(bPath, MCVP_DRAM_07, u4SrcWidth>>4, DA_DRAM_LINE_LENGTH);
}

void vDrvDIDumpStart(UINT8 bPath)
{
    UINT8* Y_MSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_08, DA_ADDR_BASE_MSB_Y)<<11);
    UINT8* Y_LSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_09, DA_ADDR_BASE_LSB_Y)<<11);
    UINT8* U_MSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0A, DA_ADDR_BASE_MSB_U)<<11);
    UINT8* U_LSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0B, DA_ADDR_BASE_LSB_U)<<11);
    UINT8* V_MSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0C, DA_ADDR_BASE_MSB_V)<<11);
    //UINT8* V_LSB_Addr = (UINT8 *)VIRTUAL(MDDI_READ_FLD(bPath, MCVP_DRAM_0D, DA_ADDR_BASE_LSB_V)<<11);
    UINT8 u1BlockSize = MDDI_READ_FLD(bPath, MCVP_DRAM_06, DA_BLOCK_SIZE)+1;
    UINT8 bYFrmNum = MDDI_READ_FLD(bPath, MCVP_DRAM_06, DA_FRAME_NUM_Y);
    UINT8 bCFrmNum = MDDI_READ_FLD(bPath, MCVP_DRAM_06, DA_FRAME_NUM_U);
    UINT8 bIs10bit = MDDI_READ_FLD(bPath, MCVP_DRAM_06, DA_BIT_SEL_WY) ? 0 : 1; 
    UINT8 bIs444 = MDDI_READ_FLD(bPath, MCVP_KC_1E, M444);     
	UINT8 bIsInterlace = MDDI_READ_FLD(bPath, MCVP_KC_00, I2P_EN);
    
	UINT32 u4SrcWidth = RegReadFldAlign(SWRW_03, DUMP_CTRL_WIDTH);
    UINT32 u4SrcHeight = RegReadFldAlign(SWRW_03, DUMP_CTRL_HEIGHT);
    UINT32* pBuf = (UINT32 *)VIRTUAL(u4RegRead4B(SWRW_01));
    
    UINT32 u4HPxlPerPgMsb = 1 << (6 + u1BlockSize);
    UINT32 u4VPxlPerPgMsb = 1 << (5 - u1BlockSize);  	
	UINT32 u4HPxlPerPgLsb = 64*4; // 2bit data
    UINT32 u4VPxlPerPgLsb = 32;  
	UINT32 HPageMsb = (u4SrcWidth+u4HPxlPerPgMsb-1)/u4HPxlPerPgMsb;
	UINT32 HPageLsb = (u4SrcWidth+u4HPxlPerPgLsb-1)/u4HPxlPerPgLsb;  
	UINT32 blkNumMsb, lineNumMsb, posMsb;
	UINT32 blkNumLsb, lineNumLsb, posLsb, Lsbshf;
    UINT32 Y0 = 0, Y1 = 0, Cb = 0, Cr = 0;
    UINT32 YMsb, YLsb, CMsb, CLsb;
    UINT32 R, G, B;  
    UINT32 x, y;

    vDrvDIFreezeOnOff(bPath, SV_ON);
    
    if ((bYFrmNum == 0) && bIsInterlace)
    {
        LOG(0, "PSCAN 0W0R mode");
        return;
    }
    else
    {
        bYFrmNum++;
        bCFrmNum++;
    }
    
    if (bIs444 == 0)
    {   
        for (y=0; y < u4SrcHeight; y++)
        {
            for (x=0; x < u4SrcWidth; x++)
            {
                blkNumMsb = y*bYFrmNum/u4VPxlPerPgMsb*HPageMsb+x/u4HPxlPerPgMsb;
                lineNumMsb = y*bYFrmNum%u4VPxlPerPgMsb;

                blkNumLsb = y*bYFrmNum/u4VPxlPerPgLsb*HPageLsb+x/u4HPxlPerPgLsb;
                lineNumLsb = y*bYFrmNum%u4VPxlPerPgLsb;
                
                posMsb = x%u4HPxlPerPgMsb;          
                posLsb = x%u4HPxlPerPgLsb;  
                Lsbshf = (x%4)*2;
                
                YMsb = (*(Y_MSB_Addr+blkNumMsb*2048+lineNumMsb*u4HPxlPerPgMsb+posMsb))<<2;               
                CMsb = (*(U_MSB_Addr+blkNumMsb*2048+lineNumMsb*u4HPxlPerPgMsb+posMsb))<<2;    
                
                if (bIs10bit)
                {
                    YLsb = ((*(Y_LSB_Addr+blkNumLsb*2048+lineNumLsb*u4HPxlPerPgLsb/4+posLsb/4))>>Lsbshf)&0x3; 
                    CLsb = ((*(U_LSB_Addr+blkNumLsb*2048+lineNumLsb*u4HPxlPerPgLsb/4+posLsb/4))>>Lsbshf)&0x3; 
                }
                else
                {                   
                    YLsb = 0;   
                    CLsb = 0;   
                }
                
                if ((x % 2) == 0)
                {
                    Y0 = YMsb+YLsb;
                    Cb = CMsb+CLsb; 
                }
                else
                {
                    Y1 = YMsb+YLsb;
                    Cr = CMsb+CLsb; 

                    pBuf[y*u4SrcWidth+x-1] = (Y0<<20) | (Cr<<10) | Cb;
                    pBuf[y*u4SrcWidth+x] = (Y1<<20) | (Cr<<10) | Cb; 
                }
            }
        }
        
        vRegWriteFldAlign(SWRW_03, 3, DUMP_CTRL_FORMAT);    
    }
    else
    {       
        for (y=0; y < u4SrcHeight; y++)
        {
            for (x=0; x < u4SrcWidth; x++)
            {
                blkNumMsb = y*bYFrmNum/u4VPxlPerPgMsb*HPageMsb+x/u4HPxlPerPgMsb;
                lineNumMsb = y*bYFrmNum%u4VPxlPerPgMsb;
                
                posMsb = x%u4HPxlPerPgMsb;          
                
                G = (*(Y_MSB_Addr+blkNumMsb*2048+lineNumMsb*u4HPxlPerPgMsb+posMsb));         
                B = (*(U_MSB_Addr+blkNumMsb*2048+lineNumMsb*u4HPxlPerPgMsb+posMsb));               
                R = (*(V_MSB_Addr+blkNumMsb*2048+lineNumMsb*u4HPxlPerPgMsb+posMsb));

                pBuf[y*u4SrcWidth+x] = (R<<20) | (G<<10) | B;
            }
        }
        
        vRegWriteFldAlign(SWRW_03, 1, DUMP_CTRL_FORMAT);    
    }    
}

void vDrvDIDumpEnd(UINT8 bPath)
{
    UINT32 u4SubOffset = (bPath == VDP_2) ? 0x800 : 0;
    
    vRegWrite4B(MCVP_DRAM_12+u4SubOffset, DiReg12);
	vRegWrite4B(MCVP_DRAM_06+u4SubOffset, DiReg06);
    vRegWrite4B(MCVP_DRAM_07+u4SubOffset, DiReg07);
	vRegWrite4B(MCVP_DRAM_16+u4SubOffset, DiReg16);

    vDrvDIFreezeOnOff(bPath, SV_OFF);
}


#define DRAM_ALIGN_WIDTH 256
#define DRAM_ALIGN_HEIGHT 8

#if 0//CC_OPTIMIZE_MIB_DUMP //only improve a little , forbandone
static inline void vDrvDIDumpDramForTDNAVI_1Frame(UINT8 u1VdpId, UINT16 x, UINT16 y, UINT16 width, UINT16 height,UINT32 *pu4Data)
{
    UINT32 u4Addr = VIRTUAL(MDDI_READ_FLD(u1VdpId,MCVP_DRAM_08, DA_ADDR_BASE_MSB_Y) << 11);
    UINT32 u4Pitch = MDDI_READ_FLD(u1VdpId ,MCVP_DRAM_07, DA_DRAM_LINE_LENGTH)*16;
    UINT32 *pu4AddrTmp;
    UINT32 u4HPageMsb;
    UINT32 blkNumMsb, lineNumMsb, posMsb;
    UINT32 u4Tmp;
    UINT32 i,j;
    UINT32 u4Bnd = MIN((x + FW_NAVI_WINDOW_WIDTH-1)/DRAM_ALIGN_WIDTH*DRAM_ALIGN_WIDTH,x +FW_NAVI_WINDOW_WIDTH);
    
    u4HPageMsb = (u4Pitch+DRAM_ALIGN_WIDTH-1)/DRAM_ALIGN_WIDTH;
    
    for(i = y; i < y+FW_NAVI_WINDOW_HEIGHT ; i++)
    {
        u4Tmp = i / DRAM_ALIGN_HEIGHT * u4HPageMsb;
        lineNumMsb = i % DRAM_ALIGN_HEIGHT;        

        for(j = x 
            , blkNumMsb = u4Tmp + x /DRAM_ALIGN_WIDTH 
            , posMsb = x % DRAM_ALIGN_WIDTH                       
            , pu4AddrTmp = (UINT32 *)(u4Addr + blkNumMsb*DRAM_ALIGN_WIDTH*DRAM_ALIGN_HEIGHT + lineNumMsb*DRAM_ALIGN_WIDTH+ posMsb);
            j < u4Bnd; j+=4)
        {              
            *pu4Data++ =  *pu4AddrTmp++;
        }

        for( blkNumMsb = u4Tmp + j /DRAM_ALIGN_WIDTH 
            , posMsb = j % DRAM_ALIGN_WIDTH                       
            , u4AddrTmp = (UINT32 *)(u4Addr + blkNumMsb*DRAM_ALIGN_WIDTH*DRAM_ALIGN_HEIGHT + lineNumMsb*DRAM_ALIGN_WIDTH+ posMsb);
            j < x +FW_NAVI_WINDOW_WIDTH; j+=4)
        {              
            *pu4Data++ =  *pu4AddrTmp++;
        }
    }        
}
#endif

void vDrvDIDumpDramForTDNAVI(UINT8 u1VdpId, UINT16 x, UINT16 y, UINT16 width, UINT16 height,UINT32 *pu4Data)
{
    UINT32 u4Addr = MDDI_READ_FLD(u1VdpId,MCVP_DRAM_08, DA_ADDR_BASE_MSB_Y) << 11;
    UINT32 u4Pitch = MDDI_READ_FLD(u1VdpId ,MCVP_DRAM_07, DA_DRAM_LINE_LENGTH)*16;
    UINT32 u4FrmNum  = MDDI_READ_FLD(u1VdpId ,MCVP_DRAM_06, DA_FRAME_NUM_Y)+1;

	UINT32 u4HPageMsb;
	UINT32 blkNumMsb, lineNumMsb, posMsb;
    UINT32 u4Tmp;
    UINT32 i,j;

    if(MDDI_READ_FLD(u1VdpId,MCVP_MULTI_00, EN_NRW)==SV_OFF)
    {
        return;
    }

    width = width &(~0x3);
    u4HPageMsb = (u4Pitch+DRAM_ALIGN_WIDTH-1)/DRAM_ALIGN_WIDTH;
    
    for(i = y; i < y+height ; i++)
    {
        u4Tmp = i * u4FrmNum / DRAM_ALIGN_HEIGHT * u4HPageMsb;
        lineNumMsb = i *   u4FrmNum % DRAM_ALIGN_HEIGHT;
        
        for(j = x ; j < x + width ; j+=4)
        {   
           blkNumMsb = u4Tmp + j /DRAM_ALIGN_WIDTH;
           posMsb = j % DRAM_ALIGN_WIDTH;            
            *pu4Data++ =  *(UINT32 *)VIRTUAL((u4Addr + blkNumMsb*DRAM_ALIGN_WIDTH*DRAM_ALIGN_HEIGHT + lineNumMsb*DRAM_ALIGN_WIDTH + posMsb));
        }
    }

}

EXTERN void HalFlushInvalidateDCacheSingleLine(UINT32 u4Addr);
EXTERN void HalInvalidateDCacheMultipleLine(UINT32 u4Addr, UINT32 u4Size);

void vDrvDIFlushDramForTDNAVI(UINT8 u1VdpId, UINT16 x, UINT16 y, UINT16 width, UINT16 height)
{  
    UINT32 u4Addr = MDDI_READ_FLD(u1VdpId,MCVP_DRAM_08, DA_ADDR_BASE_MSB_Y) << 11;
    UINT32 u4Pitch = MDDI_READ_FLD(u1VdpId ,MCVP_DRAM_07, DA_DRAM_LINE_LENGTH)*16;
    UINT32 u4FrmNum  = MDDI_READ_FLD(u1VdpId ,MCVP_DRAM_06, DA_FRAME_NUM_Y)+1;
    
    UINT32 u4AddrTmp;
    UINT32 u4Step;
    UINT32 u4HPageMsb;
    UINT32 blkNumMsb;
    UINT32 u4Tmp;
    UINT32 i;

    if(MDDI_READ_FLD(u1VdpId,MCVP_MULTI_00, EN_NRW)==SV_OFF)
    {
        return;
    }

    u4Step = u4FrmNum==1 ? 8 :
             u4FrmNum==2 ? 4 :
             u4FrmNum==3 ? 2 :
             u4FrmNum==4 ? 2 : 1;   
    u4HPageMsb = (u4Pitch+DRAM_ALIGN_WIDTH-1)/DRAM_ALIGN_WIDTH;

    for(i = y&(~(DRAM_ALIGN_HEIGHT-1)); i < y+height ; i+=u4Step)
    {
        u4Tmp = i * u4FrmNum/ DRAM_ALIGN_HEIGHT * u4HPageMsb;
        blkNumMsb = u4Tmp + x/DRAM_ALIGN_WIDTH ;          
        u4AddrTmp = VIRTUAL(u4Addr + blkNumMsb*DRAM_ALIGN_WIDTH*DRAM_ALIGN_HEIGHT);
        HalInvalidateDCacheMultipleLine(u4AddrTmp,DRAM_ALIGN_WIDTH*DRAM_ALIGN_HEIGHT*2);
    }
}


/*******for Dram Dump*******************/

UINT8 bDiSnakeMapping = 0;
DI_DRAM_DUMP_INTO_T rDIDramDunpInfo;

UINT8 u1DrvDIInitDump(UINT8 u1VdpId)
{    
    
	UINT16 u2Offset = (u1VdpId==VDP_1) ? 0 : 0x800;	


    bDiSnakeMapping = RegReadFldAlign(MCVP_DRAM_06+u2Offset, DA_SNAKE_MAPPING);
    
    //1. disable snake mapping first;
	vRegWriteFldAlign(MCVP_DRAM_12+u2Offset, 0, DA_NR_SNAKE_MAPPING);
	vRegWriteFldAlign(MCVP_DRAM_06+u2Offset, 0, DA_SNAKE_MAPPING);
	vRegWriteFldAlign(MCVP_DRAM_16+u2Offset, 0, DA_SC_SNAKE_MAPPING);
	
	vDrvDIGetDumpInfo(u1VdpId, &rDIDramDunpInfo);
    //Print Dram dump info
    LOG(5,"MSB Y Addr(0x%x)\n",rDIDramDunpInfo.u4MsbY);
    LOG(5,"LSB Y Addr(0x%x)\n",rDIDramDunpInfo.u4LsbY);
    LOG(5,"MSB U Addr(0x%x)\n",rDIDramDunpInfo.u4MsbU);
    LOG(5,"LSB U Addr(0x%x)\n",rDIDramDunpInfo.u4LsbU);
    LOG(5,"MSB V Addr(0x%x)\n",rDIDramDunpInfo.u4MsbV);
    LOG(5,"LSB V Addr(0x%x)\n",rDIDramDunpInfo.u4LsbV);
    
    LOG(5,"FB size(0x%x)\n",rDIDramDunpInfo.u4Size);
    LOG(5,"width(%d)\n",rDIDramDunpInfo.u4Width);
    LOG(5,"height(%d)\n",rDIDramDunpInfo.u4Height);
    LOG(5,"Y frame number(%d)\n",rDIDramDunpInfo.u1FrameNumY);
    LOG(5,"U frame number(%d)\n",rDIDramDunpInfo.u1FrameNumU);
    LOG(5,"block size(%d)\n",rDIDramDunpInfo.u1BlockSize);
    LOG(5,"is 10 bit(%d)\n",rDIDramDunpInfo.bIs10Bits);
    LOG(5,"is 444(%d)\n",rDIDramDunpInfo.bIs444);    

	return SV_TRUE;
}

UINT8 u1DrvDIReleaseDump(UINT8 u1VdpId)
{
	UINT16 u2Offset = (u1VdpId==VDP_1) ? 0x800 : 0;	

	//recover snake mapping
	vRegWriteFldAlign(MCVP_DRAM_12+u2Offset, bDiSnakeMapping, DA_NR_SNAKE_MAPPING
);
	vRegWriteFldAlign(MCVP_DRAM_06+u2Offset, bDiSnakeMapping, DA_SNAKE_MAPPING);
	vRegWriteFldAlign(MCVP_DRAM_16+u2Offset, bDiSnakeMapping, DA_SC_SNAKE_MAPPING
);    

    return SV_TRUE;
}

void vDrvDIGetDumpInfo(UINT8 u1VdpId, DI_DRAM_DUMP_INTO_T* rDramDumpInfo)
{
	UINT8 u1Offset = (u1VdpId==VDP_1) ? 0 : 0x800;	
    UINT32 u4LineLength;
	//1. base addr
	rDramDumpInfo->u4MsbY = (RegReadFldAlign(MCVP_DRAM_08+u1Offset, DA_ADDR_BASE_MSB_Y) << 11) ;
	rDramDumpInfo->u4LsbY = (RegReadFldAlign(MCVP_DRAM_09+u1Offset, DA_ADDR_BASE_LSB_Y) << 11) ;
	rDramDumpInfo->u4MsbU = (RegReadFldAlign(MCVP_DRAM_0A+u1Offset, DA_ADDR_BASE_MSB_U) << 11) ;
	rDramDumpInfo->u4LsbU = (RegReadFldAlign(MCVP_DRAM_0B+u1Offset, DA_ADDR_BASE_LSB_U) << 11) ;
	rDramDumpInfo->u4MsbV = (RegReadFldAlign(MCVP_DRAM_0C+u1Offset, DA_ADDR_BASE_MSB_V) << 11) ;
	rDramDumpInfo->u4LsbV = (RegReadFldAlign(MCVP_DRAM_0D+u1Offset, DA_ADDR_BASE_LSB_V) << 11) ;

    u4LineLength = RegReadFldAlign(MCVP_DRAM_07+u1Offset, DA_DRAM_LINE_LENGTH);
    rDramDumpInfo->u4Width = u4LineLength << 4;
	//4. height
	if(RegReadFldAlign(MCVP_KC_00+u1Offset, I2P_EN)==1 || u1VdpId == SV_VP_PIP) 
    // sub path always interlace, progresive will bypass Pscan
	{
		rDramDumpInfo->u4Height = RegReadFldAlign(MCVP_KC_0A+u1Offset, VDEW)/2;
	}
	else
	{
		rDramDumpInfo->u4Height = RegReadFldAlign(MCVP_KC_0A+u1Offset, VDEW);
	}	
	//5. 10/8 bit
	rDramDumpInfo->bIs10Bits = RegReadFldAlign(MCVP_DRAM_06+u1Offset, DA_BIT_SEL_WY) ? 0 : 1;     // 0: 8bit / 1: 10bit
    //6. 444/422
	rDramDumpInfo->bIs444 = RegReadFldAlign(MCVP_KC_1E+u1Offset, M444);  
    //7. frame buffer size	
	rDramDumpInfo->u4Size = (RegReadFldAlign(MCVP_DRAM_0E+u1Offset, DA_WADDR_HI_LIMIT) << 11) - (RegReadFldAlign(MCVP_DRAM_0F+u1Offset, DA_WADDR_LO_LIMIT) << 11);	

    //8. block size
	rDramDumpInfo->u1BlockSize = RegReadFldAlign(MCVP_DRAM_06+u1Offset, DA_BLOCK_SIZE);
    //9. Y Frame number
	rDramDumpInfo->u1FrameNumY = RegReadFldAlign(MCVP_DRAM_06+u1Offset, DA_FRAME_NUM_Y);
    //10. U Frame number
	rDramDumpInfo->u1FrameNumU = RegReadFldAlign(MCVP_DRAM_06+u1Offset, DA_FRAME_NUM_U);
}

void vDrvDIDumpDram(UINT8 u1VdpId, UINT16 x, UINT16 y, UINT16 width, UINT16 height)
{
    UINT16 yIndex = 0;
    UINT16 xIndex = 0;
    DI_BUFFER_DATA value;

    HAL_TIME_T startTime;
    HAL_TIME_T endTime;
    HAL_TIME_T durationTime;
    
    HAL_GetTime(&startTime);
    
    for(yIndex = y; yIndex < (y+height); yIndex ++)
    {
        for(xIndex = x; xIndex < (x + width); xIndex++)
        {
            vDrvDIGetPixelValue(u1VdpId, xIndex, yIndex, &value);
            Printf("(%d,%d) = (%d,%d,%d)\n", xIndex,yIndex, value.u4YG, value.u4UB, value.u4VR);
        }
    }
    HAL_GetTime(&endTime);

    HAL_GetDeltaTime(&durationTime, &startTime, &endTime);
    Printf("Dump duration: %d.%d\n", durationTime.u4Seconds, durationTime.u4Micros);
    
}

void vDrvDIGetPixelValue(UINT8 u1VdpId, UINT16 x, UINT16 y, DI_BUFFER_DATA* value)
{   
	UINT8 u1Offset = (u1VdpId==VDP_1) ? 0 : 0x800;	    
	UINT32 width = rDIDramDunpInfo.u4Width;
	UINT8 FrmNumY = 0;
    UINT8 u1BlockSize = rDIDramDunpInfo.u1BlockSize + 1;
    UINT32 u4HPxlPerPgMsb = 1 << (6 + u1BlockSize);
    UINT32 u4VPxlPerPgMsb = 1 << (5 - u1BlockSize);  	
	UINT32 u4HPxlPerPgLsb = 64*4; // 2bit data
    UINT32 u4VPxlPerPgLsb = 32;  
	UINT8 bIsInterlace = RegReadFldAlign(MCVP_KC_00+u1Offset, I2P_EN);
	UINT32 HPageMsb = (width+u4HPxlPerPgMsb-1)/u4HPxlPerPgMsb;
	UINT32 HPageLsb = (width+u4HPxlPerPgLsb-1)/u4HPxlPerPgLsb;  
	UINT32 blkNumMsb, lineNumMsb, posMsb;
	UINT32	blkNumLsb, lineNumLsb, posLsb, Lsbshf, pLsbM16;

    if(rDIDramDunpInfo.u4MsbY == 0)
    {
        return;
    }
    
	if ((rDIDramDunpInfo.u1FrameNumY == 0) && bIsInterlace) // 0W0R
	{
		Printf("PSCAN 0W0R \n");
	}
	else 
	{
		FrmNumY = rDIDramDunpInfo.u1FrameNumY + 1;
	}

	if (rDIDramDunpInfo.bIs444 == 0)    //422
	{			
		UINT32 Y0 = 0, Cb = 0; 
		UINT32 YMsb, YLsb, CMsb, CLsb;		
		
		blkNumMsb = y * FrmNumY / u4VPxlPerPgMsb * HPageMsb + x/u4HPxlPerPgMsb;
		lineNumMsb = y * FrmNumY % u4VPxlPerPgMsb;

		blkNumLsb = y*FrmNumY/u4VPxlPerPgLsb*HPageLsb+x/u4HPxlPerPgLsb;
		lineNumLsb = y*FrmNumY%u4VPxlPerPgLsb;

		//y offset
		posMsb = x%u4HPxlPerPgMsb;
		posLsb = x%u4HPxlPerPgLsb/4;
		pLsbM16 = posLsb % 16;
		posLsb = posLsb - pLsbM16 + (3 - pLsbM16/4)*4 + posLsb%4;
		Lsbshf = (x%4)*2;

		//read MSB
		YMsb = (*((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbY + blkNumMsb*2048 + lineNumMsb*u4HPxlPerPgMsb + posMsb)))<<2;				
		CMsb = (*((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbU + blkNumMsb*2048 + lineNumMsb*u4HPxlPerPgMsb + posMsb)))<<2;	

		//read LSB
		if (rDIDramDunpInfo.bIs10Bits)
		{
			YLsb = ((*((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4LsbY + blkNumLsb*2048 + lineNumLsb*u4HPxlPerPgLsb/4 + posLsb)))>>Lsbshf)&0x3;	
			CLsb = ((*((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4LsbU + blkNumLsb*2048 + lineNumLsb*u4HPxlPerPgLsb/4 + posLsb)))>>Lsbshf)&0x3;
		}
		else
		{					
			YLsb = 0;	
			CLsb = 0;	
		}

        Y0 = YMsb+YLsb;
        Cb = CMsb+CLsb;

        value->u4YG = Y0;
        value->u4UB= Cb;
        value->u4VR= 0;
	}
	else    //444
	{		
		UINT32 R, G, B;
		
		blkNumMsb = y*FrmNumY/u4VPxlPerPgMsb*HPageMsb+x/u4HPxlPerPgMsb;
		lineNumMsb = y*FrmNumY%u4VPxlPerPgMsb;
		
		posMsb = x%u4HPxlPerPgMsb;			
		
		G = *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbY + blkNumMsb*2048 + lineNumMsb*u4HPxlPerPgMsb + posMsb));			
		B = *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbU + blkNumMsb*2048 + lineNumMsb*u4HPxlPerPgMsb + posMsb));				
		R = *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbV + blkNumMsb*2048 + lineNumMsb*u4HPxlPerPgMsb + posMsb));

		value->u4YG= G;
		value->u4UB= B;
		value->u4VR= R;
	}
}


void vDrvDIWritePixelValue(UINT8 u1VdpId, UINT16 x, UINT16 y, DI_BUFFER_DATA* value)
{    
	UINT8 u1Offset = (u1VdpId==VDP_1) ? 0 : 0x800;	    
	UINT32 width = rDIDramDunpInfo.u4Width;	
	UINT8 FrmNumY = 0;
    UINT8 u1BlockSize = rDIDramDunpInfo.u1BlockSize + 1;
    UINT32 u4HPxlPerPgMsb = 1 << (6 + u1BlockSize);
    UINT32 u4VPxlPerPgMsb = 1 << (5 - u1BlockSize);  	
	UINT32 u4HPxlPerPgLsb = 64*4; // 2bit data
    UINT32 u4VPxlPerPgLsb = 32;  
	UINT8 bIsInterlace = RegReadFldAlign(MCVP_KC_00+u1Offset, I2P_EN);
    UINT16 u2I;
	UINT32 HPageMsb = (width+u4HPxlPerPgMsb-1)/u4HPxlPerPgMsb;
	UINT32 HPageLsb = (width+u4HPxlPerPgLsb-1)/u4HPxlPerPgLsb;  
	UINT32 blkNumMsb, lineNumMsb, posMsb, TmpLineNum;
	UINT32	blkNumLsb, lineNumLsb, posLsb, Lsbshf, pLsbM16;

	if(rDIDramDunpInfo.u4MsbY == 0)
    {
        return;
    }
	    
	if ((rDIDramDunpInfo.u1FrameNumY == 0) && bIsInterlace) // 0W0R
	{
		Printf("PSCAN 0W0R \n");
	}
	else 
	{
		FrmNumY = rDIDramDunpInfo.u1FrameNumY + 1;
	}


	if (rDIDramDunpInfo.bIs444 == 0)    //422
	{			
		UINT32 Y0, Cb; 
		UINT32 YMsb, YLsb, CMsb, CLsb;		
	
		blkNumMsb = y * FrmNumY / u4VPxlPerPgMsb * HPageMsb + x/u4HPxlPerPgMsb;
		lineNumMsb = y * FrmNumY % u4VPxlPerPgMsb;

		blkNumLsb = y*FrmNumY/u4VPxlPerPgLsb*HPageLsb+x/u4HPxlPerPgLsb;
		lineNumLsb = y*FrmNumY%u4VPxlPerPgLsb;

		//y offset
		posMsb = x%u4HPxlPerPgMsb;
		posLsb = x%u4HPxlPerPgLsb/4;
		pLsbM16 = posLsb % 16;
		posLsb = posLsb - pLsbM16 + (3 - pLsbM16/4)*4 + posLsb%4;
		Lsbshf = (x%4)*2;

              Y0 = value->u4YG ;
              Cb = value->u4UB;
              
              YLsb = Y0 & 0x3;
              CLsb = Cb & 0x3;

              YMsb = Y0 >> 2;
              CMsb = Cb >> 2;
		//write MSB
		      for (u2I = 0; u2I < FrmNumY; u2I++)
		      {
		          TmpLineNum = (lineNumMsb + u2I) % u4VPxlPerPgMsb;
                  *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbY + blkNumMsb*2048 + TmpLineNum * u4HPxlPerPgMsb + posMsb)) = (UINT8)YMsb;
                  *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbU + blkNumMsb*2048 + TmpLineNum * u4HPxlPerPgMsb + posMsb)) = (UINT8)CMsb;
		      }
  
		//write LSB
		if (rDIDramDunpInfo.bIs10Bits)
		{
		    for (u2I = 0; u2I < FrmNumY; u2I++)
		    {
		        UINT32 u4Y,u4C,u4Clr = ~(0x3 << Lsbshf);
		        TmpLineNum = (lineNumLsb + u2I) % u4VPxlPerPgMsb;
			    u4Y = (*((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4LsbY + blkNumLsb*2048 + TmpLineNum*u4HPxlPerPgLsb/4 + posLsb)));	
			    u4C = (*((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4LsbU + blkNumLsb*2048 + TmpLineNum*u4HPxlPerPgLsb/4 + posLsb)));	
		
		   
                YLsb = (YLsb << Lsbshf);
                CLsb = (CLsb << Lsbshf);

                u4Y &= u4Clr;
                u4C &= u4Clr;
                u4Y |= YLsb;
                u4C |= CLsb;  

                *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4LsbY + blkNumLsb*2048 + TmpLineNum * u4HPxlPerPgLsb/4 + posLsb)) = (UINT8)u4Y;
                *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4LsbU + blkNumLsb*2048 + TmpLineNum * u4HPxlPerPgLsb/4 + posLsb)) = (UINT8)u4C;
		    }
		}

	}
	else    //444
	{		
		UINT32 R, G, B;
		
		blkNumMsb = y*FrmNumY/u4VPxlPerPgMsb*HPageMsb+x/u4HPxlPerPgMsb;
		lineNumMsb = y*FrmNumY%u4VPxlPerPgMsb;
		
		posMsb = x%u4HPxlPerPgMsb;			

		G = value->u4YG;
		B = value->u4UB;
		R = value->u4VR;

		for (u2I = 0; u2I < FrmNumY; u2I++)
		{
		    TmpLineNum = (lineNumMsb + u2I) % u4VPxlPerPgMsb;
            *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbY + blkNumMsb*2048 + TmpLineNum * u4HPxlPerPgMsb + posMsb)) = (UINT8)G;
            *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbU + blkNumMsb*2048 + TmpLineNum * u4HPxlPerPgMsb + posMsb)) = (UINT8)B;
            *((UINT8 *)VIRTUAL(rDIDramDunpInfo.u4MsbV + blkNumMsb*2048 + TmpLineNum * u4HPxlPerPgMsb + posMsb)) = (UINT8)R;
		}    
	}
}



