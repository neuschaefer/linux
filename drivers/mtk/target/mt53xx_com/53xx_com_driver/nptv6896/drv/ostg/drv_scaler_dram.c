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
 * $RCSfile: drv_scaler.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_scaler.c
 *  Brief of file drv_scaler.c.
 *  Details of file drv_scaler.c (optional).
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
// Scaler related header files
#include "hw_ckgen.h"
#include "hw_scpos.h"
#include "drv_scpos.h"
#include "drv_scaler.h"
#include "drv_upscaler.h"
#include "drv_scaler_drvif.h"
#include "frametrack_drvif.h"
#include "scpos_debug.h"

// nptv header files
#include "general.h"       
#include "sv_const.h"       
#include "video_def.h"      
#include "vdo_misc.h"       
#include "vdp_display.h"
#include "drv_video.h"   
#include "source_select.h"
#include "drv_display.h"
#include "panel.h"
#include "mute_if.h"
#ifdef CC_MT5396
#include "drv_mjc.h"
#endif

#include "drv_tdtv_drvif.h"

// drvif header files
#include "fbm_drvif.h"
#include "srm_drvif.h"
#include "vdp_drvif.h"
#include "nptv_drvif.h"

// Other header files
#include "c_model.h"
#include "x_bim.h"
#include "x_mid.h"
#include "x_assert.h"
#include "x_util.h"

LINT_EXT_HEADER_END

static UINT32 _au4ForceWriteOff[VDP_NS] = {0, 0};
//static UINT8 _bIsDramBufInit[2]= {0, 0};  //MOVE to _arScalePrm??
static UINT8  _bSRMStable[2]= {SV_FALSE, SV_FALSE};     //MOVE to _arScalePrm??

EXTERN VDP_SCALER_PRM_T _arScalePrm[VDP_NS];
EXTERN UINT32 _au4OriFrameNum[VDP_NS];

//static function
static SCALER_DRAM_INIT_STATE _eScpipGetDramInitState(UINT32 u4VdpId);
static void _vScpipSetDramInitState(UINT32 u4VdpId,SCALER_DRAM_INIT_STATE state);
static void _vScpipUpdatePageNum(UINT32 u4VdpId);
static void _vScpipToggleForceOff(UINT32 u4VdpId);
static void _vScpipUpdateLinePitch(UINT32 u4VdpId);

EXTERN void vScpipUpdateTdtvInfo(UINT32 u4VdpId, UINT32 u4TdtvPrm);
EXTERN UINT32 _u4ScpipReCalDelayCounter(UINT32 u4VdpId, UINT32 u4Delay);

static SCALER_DRAM_INIT_STATE _eScpipGetDramInitState(UINT32 u4VdpId)
{
    if(u4VdpId >=VDP_NS)
    {
        return SCALER_DRAM_INIT_STATE_NOT_INIT;
    }

    return _arScalePrm[u4VdpId].eDramInitState;
}

static void _vScpipSetDramInitState(UINT32 u4VdpId,SCALER_DRAM_INIT_STATE state)
{
    if(u4VdpId >=VDP_NS)
    {
        return;
    }

    _arScalePrm[u4VdpId].eDramInitState = state;
}

/**
 *  @6896 porting donw
 */
#define SCPIP_DEBUG_DRAM_HEIGHT 10
void vScpipCalDramLimit(UINT32 u4VdpId, UINT32 u4SrcWidth, UINT32 u4SrcHeight, UINT32 u4OutWidth, UINT32 u4OutHeight, UINT32 *u4DramW, UINT32 *u4DramH)
{
    UINT32 u4WidthLimitation, u4HeightLimitation, u4DispWidth;//, u4DramWidth;

    u4HeightLimitation = wDISPLAY_HEIGHT;
    u4DispWidth = wDISPLAY_WIDTH/(u4VdpId+1);    
    u4WidthLimitation = u4DispWidth;

    if ((u1ScpipGetDispMode(getScalerPath(u4VdpId)) == VDP_SCPOS_DISPMODE_OFF) && _bSRMStable[u4VdpId])  // dram mode
    {
        // vertical limitation
        if(u4HeightLimitation >u4SrcHeight)
            u4HeightLimitation =u4SrcHeight;

        if(u4HeightLimitation >u4OutHeight)
            u4HeightLimitation =u4OutHeight;

        #if 0 // do not check height limitation from FBM because it is not accurate
        if(u4HeightLimitation> _arScalePrm[u4VdpId].u4DramHeight)
            u4HeightLimitation = _arScalePrm[u4VdpId].u4DramHeight;
        #endif

        // horizontal limitation
        if(u4WidthLimitation > u4SrcWidth)
            u4WidthLimitation = u4SrcWidth;

        if(u4WidthLimitation > u4OutWidth)
            u4WidthLimitation = u4OutWidth;

        if(u4WidthLimitation> _arScalePrm[u4VdpId].u4DramWidth)
            u4WidthLimitation = _arScalePrm[u4VdpId].u4DramWidth;

        // check if current size setting exceeds FBM buffer size
        if((u4WidthLimitation * u4HeightLimitation) 
                > (_arScalePrm[u4VdpId].u4DramWidth * _arScalePrm[u4VdpId].u4DramHeight))
        {
            u4HeightLimitation = SCPIP_DEBUG_DRAM_HEIGHT; // this will cause scaler to do v up+down            
            LOG(1,"===============================================\n");
            LOG(1,"[Error] SRM allocated buffer for Scaler(%d) is too small!!, Please call SRM owner to check this.\n",u4VdpId);
            LOG(1,"[Error] Scaler Dram Height:%d, Input height:%d, Output height:%d\n",_arScalePrm[u4VdpId].u4DramHeight ,u4SrcHeight,u4OutHeight);
            LOG(1,"===============================================\n");            
        }

        LOG(3, "Scaler Dram Limit Path(%d) W(%d) H(%d)\n", u4VdpId, u4WidthLimitation, u4HeightLimitation);
    }

    *u4DramW = u4WidthLimitation;
    *u4DramH = u4HeightLimitation;
}


/**
 * @brief Check write control status
 * @param bPath VDP_1/VDP_2
 * @6896 porting done
 * @note write off condition
 *		 1. signal unstable
 *		 2. snow screen & freeze is on (snow screen is a special case. It's under ATV and signal is unstable)
 */
void vScpipCheckWriteCtrl(UINT8 bPath)
{
    RVChannel *prChannel;
    UINT8 bSigStatus;
    UINT8 bWriteOnOff= SV_OFF;

    bSigStatus = bDrvVideoSignalStatus(bPath);
    prChannel = getChannel(bPath);

    if (prChannel->bIsChannelOn == (UINT8)SV_ON)
    {
		#if SUPPORT_ATV_SNOWSCREEN
        if(fgIsSrcAtv(bPath) && (u4ApiVideoGetSnowScreen() ==SV_TRUE))
        {
            bWriteOnOff = ((prChannel->bIsFreeze == SV_OFF)? SV_ON : SV_OFF);
        }
        else // non-atv input
		#endif
        {
            if ((bSigStatus == (UINT8)SV_VDO_STABLE) && (prChannel->bIsFreeze == (UINT8)SV_OFF))
            {
                bWriteOnOff = SV_ON;
            }
            else
            {
                bWriteOnOff = SV_OFF;
            }
        }
    }

    vScpipWriteCtrlOnOff(bPath, bWriteOnOff);
}

/**
 * @brief Turn On/Off Main Channel Write Control 
 * @param u1OnOff On/Off
 * @6896 porting done
 */
void vScpipWriteCtrlOnOff(UCHAR ucVdpId, UCHAR ucOnOff)
{
    CRIT_STATE_T csState;
    RVChannel *prChannel;

    if (ucVdpId >= VDP_NS)
    {
        return;
    }

    //don't do anything when dram buffer isn't init yet.
    if(_eScpipGetDramInitState(ucVdpId) != SCALER_DRAM_INIT_STATE_REG_INITED)
    {
        return;
    }
    
    if (ucOnOff == SV_ON && _arScalePrm[ucVdpId].u4CntToRstDramPrm != 0)
    {
        //do nothing
        return;
    }    

    prChannel = getChannel(ucVdpId);
    csState = x_crit_start();   

	/* when ucOnOff==SV_ON, need to check following condition,
	 * If anyone of following condition is true, write enable won't be enabled.
	 */
    if(ucOnOff == SV_ON)
    {
		//condition 1. force write off
		if(_au4ForceWriteOff[ucVdpId] == SV_ON)
		{
			ucOnOff = SV_OFF;
		}
		//condition 2. scops not enabled
		if(_arScalePrm[ucVdpId].u4Enable ==0)
		{
			ucOnOff = SV_OFF;
		}
		//condition3. channel is OFF
		if(prChannel->bIsChannelOn == SV_OFF)
		{
			ucOnOff = SV_OFF;
		}
		//condition4. channel is Freezed
		if(prChannel->bIsFreeze == SV_ON)
		{
			ucOnOff = SV_OFF;
		}
		//condition5. Scaler is NOT in Dram mode
		if(u1ScpipGetDispMode(getScalerPath(ucVdpId)) != VDP_SCPOS_DISPMODE_OFF)
		{
			ucOnOff = SV_OFF;
		}
    }
    
    if (ucOnOff == SV_ON)
    {
        if(ucVdpId==VDP_1)
        {
            //@6896 review this. no use anymore
            //vRegWriteFldAlign(SCPIP_PIP1_11, 1, PIP1_11_RFIFO_PRO_1);   
        }
        #ifdef CC_SCPOS_LOG_DRAM_PARAM
        if (bScpipGetDramWrite(ucVdpId) == SV_OFF)    
        {
            HAL_TIME_T dt;
            HAL_GetTime(&dt);
            _u4LogIdx = (_u4LogIdx >= (CC_SCPOS_LOG_DRAM_PARAM-1))? 0: (_u4LogIdx+1);
            _au4AddressLog[_u4LogIdx] = SCPIP_PIP1_09+pipOffset;
            _au4ValueLog[_u4LogIdx] = (1);
            _au4TimeLog[_u4LogIdx] = (dt.u4Seconds * 1000000) + dt.u4Micros;
        }
        #endif

        vScpipSetDramWrite(ucVdpId, SV_ON);
        vScpipSetDramRead(ucVdpId, SV_ON);
    } 
    else
    {        
#ifdef CC_SCPOS_LOG_DRAM_PARAM
        if (bScpipGetDramWrite(ucVdpId) == SV_ON)
        {
            HAL_TIME_T dt;
            HAL_GetTime(&dt);
            _u4LogIdx = (_u4LogIdx >= (CC_SCPOS_LOG_DRAM_PARAM-1))? 0: (_u4LogIdx+1);
            _au4AddressLog[_u4LogIdx] = SCPIP_PIP1_09+pipOffset;
            _au4ValueLog[_u4LogIdx] = (0);
            _au4TimeLog[_u4LogIdx] = (dt.u4Seconds * 1000000) + dt.u4Micros;
        }
#endif
        vScpipSetDramWrite(ucVdpId, SV_OFF);
#ifndef __MODEL_slt__
        //if not in freeze mode, turn off read control
        if(prChannel->bIsFreeze == SV_OFF)
        {
            if((u1ScpipGetDispMode(ucVdpId) == VDP_SCPOS_DISPMODE_OFF) && (bScpipGetDramWrite(ucVdpId) == SV_ON) )
            {
                _vDrvVideoSetMute(MUTE_MODULE_SCPOS_DRAM, ucVdpId, VDP_SCPOS_RESCALE_TIME+VDP_SCPOS_WEN_WAIT, TRUE);
            }
            
            vScpipSetDramRead(ucVdpId, SV_OFF);   
        }
#endif        
    }
    x_crit_end(csState);
}

/**
 *	@6896 review this. 
 *  @note: 1. Turn on write relatch when count to 0??
 *         2. called in input vsync isr.
 */
void vScpipResetDramCountDown(UINT8 bPath)
{
    UINT16 dramOffset; 
    CRIT_STATE_T csState;
    dramOffset = getDramOffset(getScalerPath(bPath));
    csState = x_crit_start();
    {    
        //counter != 0 and not in parameter update
        if(_arScalePrm[bPath].u4CntToRstDramPrm !=0)
        {
            _arScalePrm[bPath].u4CntToRstDramPrm--;
            if(_arScalePrm[bPath].u4CntToRstDramPrm==0)
            {
                vRegWriteFldAlign(SCPIP_DRAM_M_00+dramOffset, 1, DRAM_M_00_DA_WRITE_EN_RELATCH);            
            }
        }
    }    
    x_crit_end(csState);
}

/**
 *  Turn off write enable immediately and set counter.
 *  Write enable will not be turned on before counter clear to 0.
 *	@6896 porting done
 */
static void _vScpipToggleForceOff(UINT32 u4VdpId)
{
	UINT8 bPath;
	UINT16 dramOffset;
	CRIT_STATE_T csState; 

	bPath = getScalerPath(u4VdpId);
	dramOffset = getDramOffset(bPath);

    LOG(3, "vDrvScpipForceWENOff: Disable (%d) Write Relatch\n", u4VdpId); 
	    
    csState = x_crit_start(); 
    _arScalePrm[u4VdpId].u4CntToRstDramPrm = (_arScalePrm[u4VdpId].u4CntToRstDramPrm < 2)? 2: _arScalePrm[u4VdpId].u4CntToRstDramPrm;
    vRegWriteFldAlign(SCPIP_DRAM_M_00+dramOffset, 0, DRAM_M_00_DA_WRITE_EN_RELATCH);
    x_crit_end(csState);
    vScpipWriteCtrlOnOff(u4VdpId, SV_OFF);      	
}

/**
 * @brief Force to turn Off Channel Write Control 
 * @param u4VdpId VDP_1/VDP_2
 * @return u1OnOff On/Off
 * @6896 review done
 */
void vScpipForceWriteOff(UINT32 u4VdpId, UINT8 u1OnOff) 
{
    CRIT_STATE_T csState;

    if (u1OnOff)
    {
        LOG(9, "Enable SCPOS(%d) Force-write-control-off\n", u4VdpId);
    }
    else
    {
        LOG(9, "Cancel SCPOS(%d) Force-write-control-off\n", u4VdpId);
    }
    csState = x_crit_start();    
    _au4ForceWriteOff[u4VdpId] = u1OnOff;
    x_crit_end(csState);   
}

/**
 * @brief Set frame buffer number
 * @param u4VdpId VDP_1/VDP_2
 * @param u4BufNum frame buffer number
 * @6896 porting done
 */
void vScpipSetBufNum(UINT32 u4VdpId, UINT32 u4BufNum)
{
    UINT8 bPath;
    UINT16 dramOffset;

    bPath = getScalerPath(u4VdpId);
    dramOffset = getDramOffset(bPath);

    if (u4BufNum < 1 || u4BufNum > 4)
        return;

    vRegWriteFldAlign(SCPIP_DRAM_M_00+dramOffset, u4BufNum-1, DRAM_M_00_DA_FRAME_NUM);
}

/**
 * @brief Get frame buffer number
 * @param u4VdpId VDP_1/VDP_2
 * @6896 porting done 
 */
UINT32 u4ScpipGetBufNum(UINT32 u4VdpId)
{
    UINT32 u4FrameNum;
    UINT8 bPath;
    UINT16 dramOffset;

    bPath = getScalerPath(u4VdpId);
    dramOffset = getDramOffset(bPath);

    u4FrameNum = RegReadFldAlign(SCPIP_DRAM_M_00+dramOffset,DRAM_M_00_DA_FRAME_NUM) + 1;

    return u4FrameNum;    
}

/**
 *  @6896 new
 */
UINT8 bScpipGetDramWrite(UINT32 u4VdpId)
{
    UINT8 bRet = SV_OFF;
    UINT8 bPath;
    UINT16 dramOffset;

    if (u4VdpId >= VDP_NS)
    {
        return bRet;
    }
    
    bPath = getScalerPath(u4VdpId);
    dramOffset = getDramOffset(bPath);
    if(RegReadFldAlign(SCPIP_DRAM_M_00+dramOffset, DRAM_M_00_DA_WRITE_ENABLE)==1)
    {
        bRet = SV_ON;
    }

    return bRet;
}

/**
 *  @6896 porting done
 */
void vScpipSetDramWrite(UCHAR u4VdpId, UCHAR ucOnOff)
{
    UINT8 bPath;
    UINT16 dramOffset;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }
    
    bPath = getScalerPath(u4VdpId);
    dramOffset = getDramOffset(bPath);

    if(ucOnOff == SV_ON)
    {    
        vRegWriteFldAlign(SCPIP_DRAM_M_00+dramOffset, 1, DRAM_M_00_DA_WRITE_ENABLE);
    }
    else
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_00+dramOffset, 0, DRAM_M_00_DA_WRITE_ENABLE);
    }
    LOG(9, "vScpipSetDramWrite(Path:%d, OnOff %d)\n", bPath,ucOnOff );
}

/**
 *  @6896 new
 */
UINT8 bScpipGetDramRead(UINT32 u4VdpId)
{
    UINT8 bRet = SV_OFF;
    UINT8 bPath;
    UINT16 dramOffset;

    if (u4VdpId >= VDP_NS)
    {
        return bRet;
    }
    
    bPath = getScalerPath(u4VdpId);
    dramOffset = getDramOffset(bPath);
    if(RegReadFldAlign(SCPIP_DRAM_M_00+dramOffset, DRAM_M_00_DA_READ_ENABLE)==1)
    {
        bRet = SV_ON;
    }

    return bRet;
}

/**
 *  @6896 porting done
 */
void vScpipSetDramRead(UCHAR u4VdpId, UCHAR ucOnOff)
{
    UINT8 bPath;
    UINT16 dramOffset;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    bPath = getScalerPath(u4VdpId);
    dramOffset = getDramOffset(bPath);    
    vRegWriteFldAlign(SCPIP_DRAM_M_00+dramOffset, ucOnOff, DRAM_M_00_DA_READ_ENABLE);
    LOG(9, "vScpipSetDramRead(Path:%d, OnOff:%d)\n", bPath,ucOnOff);
}

static void _vScpipSrmUpdateDramInfo(UINT32 u4VdpId, FBM_POOL_T* prPool, FBM_POOL_T* prMddiPool, UINT32 u4Mode)
{
    //update scaler dram info
    _arScalePrm[u4VdpId].rFbmInfo.u4Addr = prPool->u4Addr;
    _arScalePrm[u4VdpId].rFbmInfo.u4Width = prPool->u4Width;
    _arScalePrm[u4VdpId].rFbmInfo.u4Height = prPool->u4Height;
    _arScalePrm[u4VdpId].rFbmInfo.u4FbmSize = prPool->u4Size;       
    _arScalePrm[u4VdpId].rFbmInfo.fg2FB = ((u4Mode & FBM_POOL_MODE_SCPOS_2FB) > 0)? 1: 0;
    _arScalePrm[u4VdpId].rFbmInfo.fg10bit = ((u4Mode & FBM_POOL_MODE_10BIT) > 0)? 1: 0;
    _arScalePrm[u4VdpId].rFbmInfo.fg8bit = ((u4Mode & FBM_POOL_MODE_8BIT) > 0)? 1: 0;
    _arScalePrm[u4VdpId].rFbmInfo.fg422 = ((u4Mode & FBM_POOL_MODE_422_FB) > 0)? 1: 0;

    //When calculating pre-down factor, need to consider how much dram Pscan get from SRM
    if (prMddiPool->u4Mode & FBM_POOL_MODE_SCPOS_BYPASS_PSCANNR)
    {
        _arScalePrm[u4VdpId].rFbmInfo.u4MddiDramWidth = SCPOS_BYPASS_PSCANNR;
    }
    else
    {
        _arScalePrm[u4VdpId].rFbmInfo.u4MddiDramWidth = prMddiPool->u4Width;
    }

    //update bit resolution
    if (((_arScalePrm[u4VdpId].rFbmInfo.fg10bit) && (_arScalePrm[u4VdpId].rFbmInfo.fg422== SV_TRUE)) 
            && (_arScalePrm[u4VdpId].u410BitMode == 0))             //422 8bits change to 422 10bits
    {
        _arScalePrm[u4VdpId].u410BitMode = 1;
        LOG(3, "SCPOS 10 Bit mode\n");          
    }
    else if (((_arScalePrm[u4VdpId].rFbmInfo.fg8bit) || (_arScalePrm[u4VdpId].rFbmInfo.fg422==SV_FALSE))  //10 bits change to 444 8bits
            && (_arScalePrm[u4VdpId].u410BitMode == 1))
    {
        _arScalePrm[u4VdpId].u410BitMode = 0;
        LOG(3, "SCPOS 8 Bit mode\n");         
    }

    _arScalePrm[u4VdpId].u4DramWidth = _arScalePrm[u4VdpId].rFbmInfo.u4Width;   //@todo: remove u4DramWidth from and replace by rFbmInfo.u4DramWidth??
    _arScalePrm[u4VdpId].u4DramHeight = _arScalePrm[u4VdpId].rFbmInfo.u4Height; //@todo: remove u4DramWidth from and replace by rFbmInfo.u4DramWidth??

    LOG(3, "[SRM Callback]Set Scaler Buffer: addr=0x%x, w=%d, h=%d, 2FB=%d, 10bit=%d, DIWidth=%d\n", 
            (UINT32)_arScalePrm[u4VdpId].rFbmInfo.u4Addr, 
            (UINT32)_arScalePrm[u4VdpId].rFbmInfo.u4Width, 
            (UINT32)_arScalePrm[u4VdpId].rFbmInfo.u4Height, 
            (UINT32)_arScalePrm[u4VdpId].rFbmInfo.fg2FB,
            (UINT32)_arScalePrm[u4VdpId].rFbmInfo.fg10bit, 
            (UINT32)_arScalePrm[u4VdpId].rFbmInfo.u4MddiDramWidth);
}

static void _vDrvScpipSrmUpdateDispmode(UINT32 u4VdpId, UINT32 u4Mode)
{
    if (u4Mode & FBM_POOL_MODE_UNSTABLE)
    {
        return;
    }
    if (u4Mode & FBM_POOL_MODE_SCPOS_FULL)
    {
        _VDP_DispModeEnable((UCHAR)u4VdpId, (UCHAR)VDP_SCPOS_DISPMODE_OFF);
    }
    else if (u4Mode & FBM_POOL_MODE_SCPOS_DISP)
    {
        _VDP_DispModeEnable((UCHAR)u4VdpId, (UCHAR)VDP_SCPOS_DISPMODE_AUTO);
    }
    else if (u4Mode & FBM_POOL_MODE_SCPOS_PSCAN_DISP)
    {
        _VDP_DispModeEnable((UCHAR)u4VdpId, (UCHAR)VDP_SCPOS_PSCAN_DISPMODE);
    }
    else if (u4Mode & FBM_POOL_MODE_SCPOS_LINESYNC_DISP)
    {
        _VDP_DispModeEnable((UCHAR)u4VdpId, (UCHAR)VDP_SCPOS_LINESYNC_DISPMODE);
    }
    else if (u4Mode & FBM_POOL_MODE_SCPOS_MJC_DISP)
    {
        //@6896 review this
        ASSERT(0); // no mjc display mode in 5365/5395        
    }
    else
    {
        ASSERT(0); // should never get in here!
    }    
}

static void _vScpipUpdatePageNum(UINT32 u4VdpId)
{
    SCALER_DRAM_ADDR_T* dramAddrInfo;
    SCALER_FBM_POOL_T* fbmInfo;
    UINT32 u4FbTotalSize;
    UINT32 u4FbUnitSize;
    UINT8 u1Divider = 1;    //to avoid divide by 0
    UINT8 bIs10Bit;
    UINT8 bIs422;
    UINT8 bPath;
    UINT8 bMode;

    dramAddrInfo = getDramAddrInfo(u4VdpId);
    fbmInfo = getFbmPoolInfo(u4VdpId);    
    u4FbTotalSize = (fbmInfo->u4FbmSize) >> 11;  //total page number from FBM
    bIs10Bit = fbmInfo->fg10bit;
    bIs422 = fbmInfo->fg422;
    bPath = getScalerPath(u4VdpId);
    bMode = getScalerMode(bPath);

    if(bMode != VDP_SCPOS_DISPMODE_OFF)
    {
        dramAddrInfo->MsbPageNum = 0;
        dramAddrInfo->LsbPageNum = 0;
        return;
    }
    
    if(bIs10Bit == SV_TRUE && bIs422 == SV_TRUE)    //422 10 bit
    {
        u4FbTotalSize -= 8; // fbm size +padding
        u1Divider = 10;  // (Y,U) MSB 4 unit for each, (Y,U) LSB 1 unit for each
    }
    else if(bIs10Bit == SV_FALSE && bIs422 == SV_TRUE)    //422 8 bit
    {
        u4FbTotalSize -= 8; // fbm size +padding
        u1Divider = 8;  // (Y,U) MSB 4 unit for each
    }
    else if(bIs10Bit == SV_TRUE && bIs422 == SV_FALSE)    //444 10 bit
    {
        u4FbTotalSize -= 12; // fbm size +padding, FIXME
        u1Divider = 15; // (Y,U,V) MSB 4 unit for each, (Y,U,V) LSB 1 unit for each
    }
    else if(bIs10Bit == SV_FALSE && bIs422 == SV_FALSE)    //444 8 bit
    {
        u4FbTotalSize -= 12; // fbm size +padding
        u1Divider = 12; // (Y,U,V) MSB 4 unit for each
    }

    u4FbUnitSize = u4FbTotalSize / u1Divider;

    dramAddrInfo->MsbPageNum = u4FbUnitSize * 4;

    if(bIs10Bit == SV_TRUE)
    {
        dramAddrInfo->LsbPageNum = u4FbUnitSize;
    }
    else
    {
        dramAddrInfo->LsbPageNum = 0;
    }
    LOG(1,"Update Scaler Dram Page Info. MSB Page number (0x%x) LSB Page number(0x%x)\n ",
        dramAddrInfo->MsbPageNum,dramAddrInfo->LsbPageNum);    
}

static void _vScpipCalBufferAddress(UINT32 u4VdpId)
{
    SCALER_DRAM_ADDR_T* dramAddrInfo;
    SCALER_FBM_POOL_T* fbmInfo;
    UINT32 u4MsbTotalPage;
    UINT32 u4LsbTotalPage;

    dramAddrInfo = getDramAddrInfo(u4VdpId);
    fbmInfo= getFbmPoolInfo(u4VdpId);\
    u4MsbTotalPage = dramAddrInfo->MsbPageNum;
    u4LsbTotalPage = dramAddrInfo->LsbPageNum;

    //1. Low bound
    dramAddrInfo->LowAddr = fbmInfo->u4Addr >> 11;
    //2. Y Msb
    dramAddrInfo->YMsbAddr = fbmInfo->u4Addr >> 11; 
    //3. Y Lsb = Y Msb + Msb Total Page
    dramAddrInfo->YLsbAddr = MAKE_ALIGN_TO((dramAddrInfo->YMsbAddr + u4MsbTotalPage), 3, (((dramAddrInfo->LowAddr)%8)+2)%8);
    //4. U Msb = Y Lsb + Lsb Total Page
    dramAddrInfo->UMsbAddr = MAKE_ALIGN_TO((dramAddrInfo->YLsbAddr + u4LsbTotalPage), 3, (((dramAddrInfo->LowAddr)%8)+4)%8);
    //5. U Lsb = U Msb + Msb Total Page
    dramAddrInfo->ULsbAddr = MAKE_ALIGN_TO((dramAddrInfo->UMsbAddr + u4MsbTotalPage), 3, (((dramAddrInfo->LowAddr)%8)+6)%8);
    //6. V Msb & V Lsb
    if(fbmInfo->fg422 == SV_FALSE)  //444
    {
        dramAddrInfo->VMsbAddr = MAKE_ALIGN_TO((dramAddrInfo->ULsbAddr + u4LsbTotalPage), 3, (((dramAddrInfo->LowAddr)%8)+0)%8);
        dramAddrInfo->VLsbAddr = MAKE_ALIGN_TO((dramAddrInfo->VMsbAddr + u4MsbTotalPage), 3, (((dramAddrInfo->LowAddr)%8)+2)%8);
    }
    else    //422
    {
        dramAddrInfo->VMsbAddr = dramAddrInfo->UMsbAddr;
        dramAddrInfo->VLsbAddr = dramAddrInfo->ULsbAddr;
    }
    //8. High bound
    //dramAddrInfo->HighAddr = MAKE_ALIGN_TO(((fbmInfo->u4Addr+fbmInfo->u4FbmSize)>>11), 3, (((dramAddrInfo->LowAddr)%8)+4)%8);
    dramAddrInfo->HighAddr = (fbmInfo->u4Addr + fbmInfo->u4FbmSize)>>11; // FIXME
    LOG(1,"Update Scaler Dram Info\n");
    LOG(1,"==========================\n");    
    LOG(1,"YMSB(0x%x) YLSB(0x%x)\n",dramAddrInfo->YMsbAddr,dramAddrInfo->YLsbAddr);
    LOG(1,"UMSB(0x%x) ULSB(0x%x)\n",dramAddrInfo->UMsbAddr, dramAddrInfo->ULsbAddr);
    LOG(1,"VMSB(0x%x) VLSB(0x%x)\n",dramAddrInfo->VMsbAddr , dramAddrInfo->VLsbAddr);
    LOG(1,"Low(0x%x) High(0x%x)\n",dramAddrInfo->LowAddr, dramAddrInfo->HighAddr);    
    LOG(1,"==========================\n");    
}

static void _vScpipUpdateFrameNumber(UINT32 u4VdpId)
{
    UINT32 u4FrameNum;
    
    // 1. scaler dram mode flip (4 frames)
#if defined(CC_FLIP_MIRROR_SUPPORT)
    if(u4GetFlipMirrorModule(u4VdpId)==FLIP_BY_SCALER)
    {	
        u4FrameNum = 4;       
    }
    else if(u1ScpipGetDispMode(u4VdpId) == VDP_SCPOS_DISPMODE_OFF) //dram mode
    {
        u4FrameNum = 3; 	  
    }
    else                                                              //disp mode
    {
        u4FrameNum = 2; 	  
    }
#else    
    if(u1ScpipGetDispMode(u4VdpId) == VDP_SCPOS_DISPMODE_OFF)      //dram mode
    {
        u4FrameNum = 3; 	  
    }
    else                                                              //disp mode
    {
        u4FrameNum = 2; 	  
    }
#endif

    //keep frame number
    _au4OriFrameNum[u4VdpId] = u4FrameNum;

    //game mode, force to 2 frame
    if ((_arScalePrm[u4VdpId].u4IsGameMode==SV_TRUE) && (u4FrameNum > 2))
    {
        LOG(3, "GameMode: Force scalar to 2 frame mode! (%d)\n", u4FrameNum);
        u4FrameNum = 2;
    }

    _arScalePrm[u4VdpId].rDramAddrInfo.FrameNum = u4FrameNum;
}

static void _vScpipUpdateLinePitch(UINT32 u4VdpId)
{
    UINT8 bPath;
    RDispPrm* dispPrm;
    SCALER_FBM_POOL_T* fbmInfo;
    UINT16 width;

    bPath = getScalerPath(u4VdpId);
    dispPrm = getDispPrm(bPath);
    fbmInfo= getFbmPoolInfo(u4VdpId);

    //width = MIN(wDrvVideoInputWidth(bPath), fbmInfo->u4Width);
    if (wDrvVideoInputWidth(bPath) == 0) // unstable
    {
        width = fbmInfo->u4Width;
    }
    else
    {
        width = MIN(wDrvVideoInputWidth(bPath), fbmInfo->u4Width);
    }
    
    //calculate dram write picth  
    dispPrm->u2DramLinePitch = DIVIDE_WITH_CARRIER((UINT16)width,16);
}

void vScpipUpdateDramPrm(UINT32 u4VdpId, const SCALER_FBM_POOL_T* prFbmPool)
{
    if (u4VdpId >= VDP_NS)
    {
        return;
    }
    //1. update frame number to write
    _vScpipUpdateFrameNumber(u4VdpId);
    //2. update Msb & Lsb Page Number
    _vScpipUpdatePageNum(u4VdpId);
    //3. calculate Dram base address & Low/High limit
    _vScpipCalBufferAddress(u4VdpId);
    //4. update line pitch
    _vScpipUpdateLinePitch(u4VdpId);
    
}

void vScpipUpdateDramReg(UINT32 u4VdpId)
{
    UINT8 bPath;
    UINT16 dramOffset;
    SCALER_DRAM_ADDR_T* dramInfo;
    RDispPrm* dispPrm;
        
    bPath = getScalerPath(u4VdpId);
    dramOffset = getDramOffset(bPath);
    dramInfo = getDramAddrInfo(u4VdpId);
    dispPrm = getDispPrm(bPath);
    
    if(dramInfo->YMsbAddr == RegReadFldAlign(SCPIP_DRAM_M_05+dramOffset, DRAM_M_05_DA_ADDR_BASE_MSB_Y) &&
       dramInfo->YLsbAddr == RegReadFldAlign(SCPIP_DRAM_M_06+dramOffset, DRAM_M_06_DA_ADDR_BASE_LSB_Y) &&
       dramInfo->UMsbAddr == RegReadFldAlign(SCPIP_DRAM_M_07+dramOffset, DRAM_M_07_DA_ADDR_BASE_MSB_U) &&
       dramInfo->ULsbAddr == RegReadFldAlign(SCPIP_DRAM_M_08+dramOffset, DRAM_M_08_DA_ADDR_BASE_LSB_U) &&
       dramInfo->VMsbAddr == RegReadFldAlign(SCPIP_DRAM_M_09+dramOffset, DRAM_M_09_DA_ADDR_BASE_MSB_V) &&
       dramInfo->VLsbAddr == RegReadFldAlign(SCPIP_DRAM_M_0A+dramOffset, DRAM_M_0A_DA_ADDR_BASE_LSB_V) &&
       dramInfo->LowAddr  == RegReadFldAlign(SCPIP_DRAM_M_0C+dramOffset, DRAM_M_0C_DA_WADDR_LO_LIMIT) &&
       dramInfo->HighAddr == RegReadFldAlign(SCPIP_DRAM_M_0B+dramOffset, DRAM_M_0B_DA_WADDR_HI_LIMIT) &&
       dramInfo->FrameNum == u4ScpipGetBufNum(u4VdpId))
    {
        //all the same. just return
        return;
    }
    
    if(getScalerMode(bPath) == VDP_SCPOS_DISPMODE_OFF)
    {
	//if dram parameter chang ,set mute 
        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_DRAM, u4VdpId, VDP_SCPOS_RESCALE_TIME+VDP_SCPOS_WEN_WAIT, TRUE);
    }

    //embrace with critical section
    
    //1. turn off dram write & set counter
    _vScpipToggleForceOff(u4VdpId);

    //2. update base address
    vRegWriteFldAlign(SCPIP_DRAM_M_0C+dramOffset, dramInfo->LowAddr ,DRAM_M_0C_DA_WADDR_LO_LIMIT);
    vRegWriteFldAlign(SCPIP_DRAM_M_0B+dramOffset, dramInfo->HighAddr,DRAM_M_0B_DA_WADDR_HI_LIMIT);
    vRegWriteFldAlign(SCPIP_DRAM_M_05+dramOffset, dramInfo->YMsbAddr,DRAM_M_05_DA_ADDR_BASE_MSB_Y);
    vRegWriteFldAlign(SCPIP_DRAM_M_06+dramOffset, dramInfo->YLsbAddr,DRAM_M_06_DA_ADDR_BASE_LSB_Y);
    vRegWriteFldAlign(SCPIP_DRAM_M_07+dramOffset, dramInfo->UMsbAddr,DRAM_M_07_DA_ADDR_BASE_MSB_U);
    vRegWriteFldAlign(SCPIP_DRAM_M_08+dramOffset, dramInfo->ULsbAddr,DRAM_M_08_DA_ADDR_BASE_LSB_U);
    vRegWriteFldAlign(SCPIP_DRAM_M_09+dramOffset, dramInfo->VMsbAddr,DRAM_M_09_DA_ADDR_BASE_MSB_V);
    vRegWriteFldAlign(SCPIP_DRAM_M_0A+dramOffset, dramInfo->VLsbAddr,DRAM_M_0A_DA_ADDR_BASE_LSB_V);
    //3. update frame number
    vScpipSetBufNum(u4VdpId,dramInfo->FrameNum);
    //4. set Dram line pitch
    vRegWriteFldAlign(SCPIP_DRAM_M_00+dramOffset, dispPrm->u2DramLinePitch,DRAM_M_00_DA_DRAM_LINE_PITCH);

    //5. set this means dram register has been inited.
    if(_eScpipGetDramInitState(u4VdpId) == SCALER_DRAM_INIT_STATE_SRM_INITED)
    {
        _vScpipSetDramInitState(u4VdpId, SCALER_DRAM_INIT_STATE_REG_INITED);
    }
}

#if defined(CC_MT53xx)

static UINT8 _u1ScpipIsDRAMParamChanged(UINT32 u4VdpId, UINT32 u4Addr, UINT32 u4Size, UINT32 u4FrameNum);
static void _vScpipSetDram(UINT32 u4VdpId, UINT32 u4WAddr, UINT32 u4RAddr, UINT32 u4FrameSize);

/**
 *  @6896 no use in 6896, only for reference
 */
static UINT32 _u4BaseAddress[2]= {0, 0}; // scaler dram base address

/**
 * @brief Set drame related information
 * @param u4VdpId VDP_1/VDP_2
 * @param prFbmPool parameters from srm
 * @6896 no use in 6896, only for reference
 */
void vScpipSrmSetBuf(UINT32 u4VdpId, const SCALER_FBM_POOL_T * prFbmPool) 
{
    UINT32 u4FbSize;
    UINT8 u1ParamChanged;
    UINT32 u4FrameNum = 1;  //to avoid divide by 0

    ASSERT(prFbmPool != NULL);
    ASSERT(prFbmPool->u4Addr != NULL);

    u1ParamChanged = 0;

    // extra frame buffer condition
    // 1. scaler dram mode flip (4 frames)
#if defined(CC_FLIP_MIRROR_SUPPORT)
    if(u4GetFlipMirrorModule(u4VdpId)==FLIP_BY_SCALER)
    {
        u4FrameNum = 4;       
    }
    else
    {
        u4FrameNum = 3;   
    }
#else    
    u4FrameNum = 3; 
#endif       

    //game mode, force to 2 frame
    if ((_arScalePrm[u4VdpId].u4IsGameMode == SV_TRUE) && (u4FrameNum > 2))        
    {
        LOG(3, "GameMode: Force scalar to 2 frame mode! (%d)\n", u4FrameNum);
        u4FrameNum = 2;
    }

    if (((prFbmPool->fg10bit) && (prFbmPool->fg422 == SV_TRUE)) 
            && (_arScalePrm[u4VdpId].u410BitMode == 0))             //422 8bits change to 422 10bits
    {
        _arScalePrm[u4VdpId].u410BitMode = 1;
        u1ParamChanged = COLOR_DEPTH_CHG;
        LOG(3, "SCPOS 10 Bit mode\n");          
    }
    else if (((prFbmPool->fg8bit) || (prFbmPool->fg422==SV_FALSE))  //10 bits change to 444 8bits
            && (_arScalePrm[u4VdpId].u410BitMode == 1))
    {
        _arScalePrm[u4VdpId].u410BitMode = 0;
        u1ParamChanged = COLOR_DEPTH_CHG;
        LOG(3, "SCPOS 8 Bit mode\n");         
    }

    _arScalePrm[u4VdpId].u4DramWidth = prFbmPool->u4Width;
    _arScalePrm[u4VdpId].u4DramHeight = prFbmPool->u4Height;
    
    if ((_arScalePrm[u4VdpId].u4DramWidth == 0) || (_arScalePrm[u4VdpId].u4DramHeight == 0))
    {
        return;
    }

    u4FbSize = (prFbmPool->u4FbmSize / u4FrameNum);

    _u4BaseAddress[u4VdpId] = prFbmPool->u4Addr;
    _au4OriFrameNum[u4VdpId] = u4FrameNum;

    u1ParamChanged |= _u1ScpipIsDRAMParamChanged(u4VdpId, _u4BaseAddress[u4VdpId] , u4FbSize, u4FrameNum);

    if (u1ParamChanged)
    {
        /* Dram address or size changed => handle mute peggy 20071107 */
        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_DRAM, u4VdpId, VDP_SCPOS_RESCALE_TIME+VDP_SCPOS_WEN_WAIT, TRUE);

        UINT32 u4Addr, u4Size, u4Num;
        u4Addr = PARAM_IGNORE;
        u4Size = PARAM_IGNORE;
        u4Num = PARAM_IGNORE;
        
        if ((u1ParamChanged & BASE_WADDRESS_CHG) || (u1ParamChanged & FRAME_SIZE_CHG))
        {
            u4Addr = _u4BaseAddress[u4VdpId];
            u4Size = u4FbSize;
        }
        if (u1ParamChanged & FRAME_NUM_CHG)
        {
            u4Num = u4FrameNum;
        }

        _vScpipUpdateDramParameter(u4VdpId, u4Addr, u4Size, u4Num);                       

        if (u1ParamChanged & COLOR_DEPTH_CHG)
        {
            vScpipReconfig(u4VdpId);
        }           
    } 
}

/**
 * @brief Check if dram paramters change
 * @param u4VdpId VDP_1/VDP_2
 * @param u4Addr base address
 * @param u4Size frame buffer size
 * @param u4FrameNum frame buffer number
 * @6896 no use in 6896, only for reference 
 */
static UINT8 _u1ScpipIsDRAMParamChanged(UINT32 u4VdpId, UINT32 u4Addr, UINT32 u4Size, UINT32 u4FrameNum)
{
    UINT8 u1Changed = 0;
    UINT32 u4OriFrameNum;
    UINT32 u4OriAdr;
    UINT32 u4OriSize;

    ASSERT(u4FrameNum > 0);

    u4Addr >>= 4;
    u4Size >>= 4;
    //u4FrameNum -= 1;

#if !SUPPORT_POP
    if(u4VdpId == VDP_2)
    {
        return 0;
    }
#endif

    //get frame number
    //u4OriFrameNum = RegReadFldAlign(SCPIP_PIP1_09+getPIPOffset(u4VdpId), PIP1_09_FSEL_1);
    u4OriFrameNum = u4ScpipGetBufNum(u4VdpId);
    //get frame size
    u4OriSize = RegReadFldAlign(SCPIP_PIP1_07+getPIPOffset(u4VdpId), PIP1_07_WINC_1);
    u4OriAdr = u4ScpipGetWAddr(u4VdpId);
    if (u4OriFrameNum != u4FrameNum)
    {
        u1Changed |= FRAME_NUM_CHG;
    }
    if (u4OriAdr != u4Addr)
    {
        u1Changed |= BASE_WADDRESS_CHG;
    }
    if (u4OriSize != u4Size)
    {
        u1Changed |= FRAME_SIZE_CHG;
    }
    if (u1Changed > 0)
    {
        LOG(3, "VDP(%d) DRAM parameters are changed! Original:(0x%X, %d, %d), New:(0x%X, %d, %d)\n", 
                u4VdpId, u4OriAdr<<4, u4OriSize<<4, u4OriFrameNum, u4Addr<<4, u4Size<<4, u4FrameNum);
    }

    return u1Changed;
}

/**
 * @6896 no use in 6896, only for reference 
 */
void _vScpipUpdateDramParameter(UINT32 u4VdpId, UINT32 u4Addr, UINT32 u4FrameSize, UINT32 u4FrameNum)
{
    UINT32 u4UpdateWAddr, u4UpdateRAddr, u4UpdateSize, u4UpdateNum;
    UINT32 u4OriWAddr, u4OriRAddr, u4OriSize, u4OriNum;
    UINT8 bPath;
    UINT16 offset;
    UINT8 u1UpdateParameter;
    UINT8 u1SwProtect = 0;

#if !SUPPORT_POP
    if(u4VdpId==VDP_2)
    {
        return;
    }
#endif

    // Convert to register-based value
    u4UpdateRAddr = u4Addr >> 4;
    u4UpdateSize = u4FrameSize >> 4;
    //u4UpdateNum = u4FrameNum - 1;
    u4UpdateNum = u4FrameNum;
    u4UpdateWAddr = u4UpdateRAddr;
    u1UpdateParameter = 0;

    bPath = ((u4VdpId == VDP_1) ? SV_VP_MAIN : SV_VP_PIP);
    offset = getPIPOffset(bPath);

    if (h_dramUpdateSema)
    {
        VERIFY(x_sema_lock(h_dramUpdateSema, X_SEMA_OPTION_WAIT) == OSR_OK);
    }

    u4OriSize = RegReadFldAlign(SCPIP_PIP1_07+offset, PIP1_07_WINC_1);    
    //u4OriNum = RegReadFldAlign(SCPIP_PIP1_09+offset, PIP1_09_FSEL_1);
    u4OriNum = u4ScpipGetBufNum(u4VdpId);
    u4OriWAddr = u4ScpipGetWAddr(u4VdpId);
    u4OriRAddr = u4ScpipGetRAddr(u4VdpId);
    if ((u4OriWAddr != u4UpdateWAddr) && (u4Addr != PARAM_IGNORE))     
    {
        u1UpdateParameter |= BASE_WADDRESS_CHG;        
        u4UpdateWAddr <<= 4;
    }
    else
    {
        u4UpdateWAddr = PARAM_IGNORE;
    }

    if ((u4OriRAddr != u4UpdateRAddr) && (u4Addr != PARAM_IGNORE))     
    {
        u1UpdateParameter |= BASE_RADDRESS_CHG;        
        u4UpdateRAddr <<= 4;
    }
    else
    {
        u4UpdateRAddr = PARAM_IGNORE;
    }

    if ((u4OriSize != u4UpdateSize) && (u4FrameSize != PARAM_IGNORE))
    {
        u1UpdateParameter |= FRAME_SIZE_CHG;
        u4UpdateSize <<= 4;
    }
    else
    {
        u4UpdateSize = PARAM_IGNORE;
    }

    if ((u4OriNum != u4UpdateNum) && (u4FrameNum != PARAM_IGNORE))
    {
        u1UpdateParameter |= FRAME_NUM_CHG;
        //u4UpdateNum += 1;
    }
    else
    {
        u4UpdateNum = PARAM_IGNORE;
    }

    if (u1UpdateParameter & (BASE_WADDRESS_CHG | FRAME_SIZE_CHG | FRAME_NUM_CHG))
    {
        CRIT_STATE_T csState; 
        // protect by SW
        u1SwProtect = 1;    
        csState = x_crit_start(); 
        _arScalePrm[u4VdpId].u4CntToRstDramPrm = _arScalePrm[u4VdpId].u4CntToRstDramPrm | 0x80000000;
        x_crit_end(csState);

        //turn off dram write immediately
        if( bScpipFlipEnable[u4VdpId] == SV_FALSE)
        {
            vRegWriteFldAlign(SCPIP_DRAM_M_00+offset, 0, DRAM_M_00_DA_WRITE_EN_RELATCH);
            vScpipWriteCtrlOnOff(u4VdpId, SV_OFF);
        }
    }

    // update base address and frame size
    if (u1UpdateParameter & (BASE_WADDRESS_CHG | FRAME_SIZE_CHG | BASE_RADDRESS_CHG))
    {
        _vScpipSetDram(u4VdpId, u4UpdateWAddr, u4UpdateRAddr, u4UpdateSize);
    }
    // update frame number
    if (u1UpdateParameter & FRAME_NUM_CHG)
    {
        vScpipSetBufNum(u4VdpId, u4UpdateNum);            
    }

    if (u1UpdateParameter & (BASE_WADDRESS_CHG | FRAME_SIZE_CHG | FRAME_NUM_CHG))
    {
        if (u1SwProtect)
        {
            // protect by SW
            CRIT_STATE_T csState; 
            csState = x_crit_start(); 
            _arScalePrm[u4VdpId].u4CntToRstDramPrm = _arScalePrm[u4VdpId].u4CntToRstDramPrm & 0x7fffffff;
            _arScalePrm[u4VdpId].u4CntToRstDramPrm = (_arScalePrm[u4VdpId].u4CntToRstDramPrm < 2)? 2 : _arScalePrm[u4VdpId].u4CntToRstDramPrm;
            x_crit_end(csState);
        }
    }

    if (h_dramUpdateSema)
    {
        VERIFY(x_sema_unlock(h_dramUpdateSema) == OSR_OK);
    }

}

/**
 * @6896 no use in 6896, only for reference 
 */
static void _vScpipSetDram(UINT32 u4VdpId, UINT32 u4WAddr, UINT32 u4RAddr, UINT32 u4FrameSize) 
{
    UINT16 offset;
    UINT8 bPath;
    CRIT_STATE_T csState;

    bPath = ((u4VdpId == VDP_1) ? SV_VP_MAIN : SV_VP_PIP);
    offset = getPIPOffset(bPath);

    if (u4WAddr != PARAM_IGNORE)
    {
        LOG(3, "Update Scpip(%d) Write Address: 0x%X\n", u4VdpId, u4WAddr);
        csState = x_crit_start(); 

        if(bScpipFlipEnable[u4VdpId]==SV_FALSE)
        {
            if (RegReadFldAlign(SCPIP_DRAM_M_00+offset, DRAM_M_00_DA_WRITE_EN_RELATCH) || 
                bScpipGetDramWrite(u4VdpId) == SV_ON)
            {
                ASSERT(0);
            }
        }
#ifdef CC_SCPOS_LOG_DRAM_PARAM
        HAL_TIME_T dt;
        HAL_GetTime(&dt);
        _u4LogIdx = (_u4LogIdx >= (CC_SCPOS_LOG_DRAM_PARAM-1))? 0: (_u4LogIdx+1);
        _au4AddressLog[_u4LogIdx] = SCPIP_PIP1_05+offset;
        _au4ValueLog[_u4LogIdx] = (u4WAddr>>4);
        _au4TimeLog[_u4LogIdx] = (dt.u4Seconds * 1000000) + dt.u4Micros;
#endif

        vScpipSetWAddr(u4VdpId, u4WAddr>>4);
        
        //_bIsDramBufInit[u4VdpId] = SV_TRUE;
        x_crit_end(csState);
    }
    if (u4RAddr != PARAM_IGNORE)
    {
        LOG(3, "Update Scpip(%d) Read Address: 0x%X\n", u4VdpId, u4RAddr);

        //vScpipSetRAddr(u4VdpId, u4RAddr>>4);
    }
    if (u4FrameSize != PARAM_IGNORE)
    {
        LOG(3, "Update Scpip(%d) frame size: 0x%X\n", u4VdpId, u4FrameSize);
        csState = x_crit_start(); 

        if(bScpipFlipEnable[u4VdpId]==SV_FALSE)
        {
            if (RegReadFldAlign(SCPIP_DRAM_M_00+offset, DRAM_M_00_DA_WRITE_EN_RELATCH) || bScpipGetDramWrite(u4VdpId) == SV_ON)
            {
                ASSERT(0);
            }
        }
#ifdef CC_SCPOS_LOG_DRAM_PARAM
        HAL_TIME_T dt;
        HAL_GetTime(&dt);
        _u4LogIdx = (_u4LogIdx >= (CC_SCPOS_LOG_DRAM_PARAM-1))? 0: (_u4LogIdx+1);
        _au4AddressLog[_u4LogIdx] = SCPIP_PIP1_07+offset;
        _au4ValueLog[_u4LogIdx] = (u4FrameSize>>4);
        _au4TimeLog[_u4LogIdx] = (dt.u4Seconds * 1000000) + dt.u4Micros;
#endif
        vRegWriteFldAlign(SCPIP_PIP1_07+offset, (u4FrameSize>>4), PIP1_07_WINC_1);
        x_crit_end(csState);
    }    
}

#endif 
/**
 *  scaler control call back
 *
 * @param void
 * @return void
 * @6896 need further review
 */
void vDrvScpipSrmDispCb(UINT32 u4VdpId, UINT32 u4Mode, UINT32 u4PDSize, UINT32 u4SramMode)
{
#ifdef CC_SCPOS_EN
    FBM_POOL_T* prPool;
    FBM_POOL_T* prMddiPool;

    LOG(3, "[SRM] DISP Callback vdp%d mode=0x%x, pds=%d, sram=%d\n", u4VdpId, u4Mode, u4PDSize, u4SramMode);

    // if srm is unstable
    if (u4Mode & FBM_POOL_MODE_UNSTABLE)
    {    
        //if unstable, force dram write off
        vScpipForceWriteOff(u4VdpId, SV_ON);    
        vScpipWriteCtrlOnOff(u4VdpId, SV_OFF);
        
        //disable dispmode
        _VDP_DispModeEnable((UINT8)u4VdpId, (UCHAR)VDP_SCPOS_DISPMODE_OFF);        
        LOG(3, "[SRM] Unstable callback! Disable Dispmode!\n");        
        
        //set srm unstable flag
        _bSRMStable[u4VdpId]=SV_FALSE;
        return;
    }

    //1. update dispmode
    _vDrvScpipSrmUpdateDispmode(u4VdpId, u4Mode);

    //2. update TDTV info
    vScpipUpdateTdtvInfo(u4VdpId,u4SramMode);


    //get Scaler & Pscan Pool info
    if (u4VdpId == VDP_1)
    {
        prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_SCPOS_MAIN, NULL);
        prMddiPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_MAIN, NULL);
    }
    else
    {
        prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_SCPOS_PIP, NULL);
        prMddiPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MDDI_PIP, NULL);
    }

    //3. pool is valid, update dram info.
    if ((prPool != NULL) && (((u4Mode & FBM_POOL_MODE_UNSTABLE) == 0) || (u4Mode & FBM_POOL_MODE_SCPOS_MM_BUFCHG)))
    {
        //srm is stable, disable "force dram write off"
        vScpipForceWriteOff(u4VdpId, SV_OFF);
        //srm is stable
        _bSRMStable[u4VdpId]=SV_TRUE;  

        _vScpipSrmUpdateDramInfo(u4VdpId, prPool, prMddiPool, u4Mode);
        
        #ifdef CC_FLIP_MIRROR_SUPPORT
        //@6896 review this
        vScpipConfigFlip(u4VdpId);
        #endif
        //vScpipSrmSetBuf(u4VdpId,&rScalerDramInfo);    //move this step to scaler mode change
        if(_eScpipGetDramInitState(u4VdpId) == SCALER_DRAM_INIT_STATE_NOT_INIT)
        {
            _vScpipSetDramInitState(u4VdpId, SCALER_DRAM_INIT_STATE_SRM_INITED);
        }

        vScpipUpdateDramPrm(u4VdpId,&_arScalePrm[u4VdpId].rFbmInfo);
        vScpipUpdateDramReg(u4VdpId);
    }
    else
    {
        if (prPool == NULL)
        {
            LOG(1, "_VdpSrmScalerWeCb: fail to get pool\n");
        }
    }

    //4. fire scaler mode change
    vScpipReconfig(u4VdpId);
#endif
}

#if 1 //for capture log
void vScpipGetDramInf(UINT8 bPath,SCALER_INFO_T* inf)
{ 
	SCALER_DRAM_ADDR_T* dramInfo;
	RDispPrm* dispPrm;
	UINT16 dramOffset;

	dramInfo = getDramAddrInfo(bPath);
	dispPrm = getDispPrm(bPath);
	dramOffset = getDramOffset(bPath);
		
	inf->YMsbAddr = dramInfo->YMsbAddr << 11;
	inf->YLsbAddr = dramInfo->YLsbAddr << 11;
	inf->UMsbAddr = dramInfo->UMsbAddr << 11;
	inf->ULsbAddr = dramInfo->ULsbAddr << 11;
	inf->VMsbAddr = dramInfo->VMsbAddr << 11;
	inf->VLsbAddr = dramInfo->VLsbAddr << 11;
	inf->FrameNum = dramInfo->FrameNum;
	inf->PictureWidth = dispPrm->u2DramWriteWidth;
	inf->PictureHeight = dispPrm->u2DramWriteHeight;
	inf->DramPitch = (dispPrm->u2DramLinePitch)<<4;
	inf->u1BitRes = (_arScalePrm[bPath].u410BitMode == 1) ? 10 : 8;
	inf->u1ColorMode = (_arScalePrm[bPath].u4Is444 == 1) ? 2 : 0;

	inf->BlockMode = RegReadFldAlign(SCPIP_DRAM_M_00+dramOffset, DRAM_M_00_DA_BLOCK_SIZE); // 1:256*8 0:128*16 

	inf->bDramSnakeMapping = bScpipIsDramSnakeMapping(bPath);
	
}
#endif

