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
 * $RCSfile: drv_mpeg.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_mpeg.c
 *  Brief of file drv_mpeg.c.
 *  Details of file drv_mpeg.c (optional).
 */

#ifdef CC_B2R_SKYPE_SUPPORT
    #if (defined(CC_MT5395) && !defined(CC_DISABLE_TWO_B2R))
    #define SUPPORT_2_B2R
    #endif
#endif
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------


#include "vdp_drvif.h"
#include "b2r_drvif.h"
#include "fbm_drvif.h"
#include "b2r_if.h"
#include "nptv_debug.h"

#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "general.h"
#include "hw_vdoin.h"
#include "video_timing.h"
#include "video_def.h"
#include "drv_mpeg.h"
#include "x_assert.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// function forward declarations
//-----------------------------------------------------------------------------

extern VDP_PRM_T* _VDP_GetVdpPrm(UINT8 u1B2rId);

void vMpegInit(void);

void vMpegInitB2rConf(void);

void vMpegModeChg(UCHAR ucVdpId);

void vMpegModeDetDone(UCHAR ucVdpId);

void vMpegModeChg(UCHAR ucVdpId);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

// B2R_NS is 1 now. Plus 1 for B2R SD
static VDP_CONF_T* _prB2rConf[B2R_NS+1];
static VDP_PRM_T* _prB2rPrm[B2R_NS+1];


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief of vMpegInitB2rConf.
 */
//-----------------------------------------------------------------------------
void vMpegInitB2rConf(void)
{
    UCHAR ucVdpId;
    UCHAR ucB2rId;

    for (ucB2rId = 0; ucB2rId < B2R_NS; ucB2rId++)
    {
        ucVdpId = VDP_B2r2Vdp(ucB2rId);  
        _prB2rConf[ucB2rId] = VDP_GetConf(ucVdpId);
        _prB2rPrm[ucB2rId] = _VDP_GetVdpPrm(ucVdpId);
    }

    /* Set B2R_2 to NULL */
    _prB2rConf[B2R_NS] = NULL;
}

//-----------------------------------------------------------------------------
/** Brief of vMpegInit.
 */
//-----------------------------------------------------------------------------
void vMpegInit(void)
{
    ASSERT((UINT32)SV_VDO_NOSIGNAL == (UINT32)VDP_STATUS_NOSIGNAL);
    ASSERT((UINT32)SV_VDO_NOSUPPORT == (UINT32)VDP_STATUS_NOSUPPORT);
    ASSERT((UINT32)SV_VDO_UNKNOWN == (UINT32)VDP_STATUS_UNKNOWN);
    ASSERT((UINT32)SV_VDO_STABLE == (UINT32)VDP_STATUS_STABLE);

    vMpegInitB2rConf();
}

//-----------------------------------------------------------------------------
/** Brief of vMpegSdConnect.
 */
//-----------------------------------------------------------------------------
void vMpegSdConnect(UINT8 bchannel, UINT8 fgIsOn) 
{
    if(fgIsOn == SV_ON)
    {
        if(bchannel == SV_VP_MAIN)
        {
            vClrMainFlg(MAIN_FLG_MODE_CHG | MAIN_FLG_MODE_DET_DONE);

            vVDOINIrqOn(MSK_MAIN_DET);
            _rMpegSDStat.bIsMain = SV_TRUE;
            _rMpegSDStat.bIsPip = SV_FALSE;
        }
        else
        {
            vClrPipFlg(PIP_FLG_MODE_CHG | PIP_FLG_MODE_DET_DONE);
        
            vVDOINIrqOn(MSK_PIP_DET);
            _rMpegSDStat.bIsMain = SV_FALSE;
            _rMpegSDStat.bIsPip = SV_TRUE;
        }
    }
    else
    {
        if(bchannel == SV_VP_MAIN)
        {
            vVDOINIrqOff(MSK_MAIN_DET);
            _rMpegSDStat.bIsMain = SV_FALSE;
            _rMpegSDStat.bIsPip = SV_FALSE;
        }
        else
        {
            vVDOINIrqOff(MSK_PIP_DET);
            _rMpegSDStat.bIsMain = SV_FALSE;
            _rMpegSDStat.bIsPip = SV_FALSE;
        }
    }

    LOG(9, "vMpegSdConnect\n");
}

//-----------------------------------------------------------------------------
/** Brief of vMpegHdConnect.
 */
//-----------------------------------------------------------------------------
void vMpegHdConnect(UINT8 bchannel, UINT8 fgIsOn) 
{
    if(fgIsOn == SV_ON)
    {
        if(bchannel == SV_VP_MAIN)
        {
            vClrMainFlg(MAIN_FLG_MODE_CHG | MAIN_FLG_MODE_DET_DONE);

            vVDOINIrqOn(MSK_MAIN_DET);
            _rMpegHDStat.bIsMain = SV_TRUE;
            _rMpegHDStat.bIsPip = SV_FALSE;
        }
        else
        {
            vClrPipFlg(PIP_FLG_MODE_CHG | PIP_FLG_MODE_DET_DONE);

            vVDOINIrqOn(MSK_PIP_DET);
            _rMpegHDStat.bIsMain = SV_FALSE;
            _rMpegHDStat.bIsPip = SV_TRUE;
        }
    }
    else
    {
        if(bchannel == SV_VP_MAIN)
        {
            vVDOINIrqOff(MSK_MAIN_DET);
            _rMpegHDStat.bIsMain = SV_FALSE;
            _rMpegHDStat.bIsPip = SV_FALSE;
        }
        else
        {
            vVDOINIrqOff(MSK_PIP_DET);
            _rMpegHDStat.bIsMain = SV_FALSE;
            _rMpegHDStat.bIsPip = SV_FALSE;
        }
    }

    LOG(9, "vMpegHdConnect\n");
}

//-----------------------------------------------------------------------------
/** Brief of vMpegModeChg.
 */
//-----------------------------------------------------------------------------
void vMpegModeChg(UCHAR ucVdpId)
{
#ifdef __MODEL_slt__
    if ((ucVdpId == VDP_1) && !_rMChannel.bIsFreeze)
#else
    if (ucVdpId == VDP_1)
#endif
    {
        vSetMainFlg(MAIN_FLG_MODE_CHG);
    }
#ifdef __MODEL_slt__
    else if ((ucVdpId == VDP_2) && !_rPChannel.bIsFreeze)
#else
    else
#endif
    {
        vSetPipFlg(PIP_FLG_MODE_CHG);
    }
}

//-----------------------------------------------------------------------------
/** Brief of vMpegModeDetDone.
 */
//-----------------------------------------------------------------------------
void vMpegModeDetDone(UCHAR ucVdpId)
{
    if (ucVdpId == VDP_1)
    {
        vSetMainFlg(MAIN_FLG_MODE_DET_DONE);
    }
    else
    {
        vSetPipFlg(PIP_FLG_MODE_DET_DONE);
    }
}

//-----------------------------------------------------------------------------
/** Brief of u1MpegHdSigStatus.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegHdSigStatus(UINT8 bPath)
{
    if (_prB2rConf[B2R_1])
    {  
    	VDP_CONF_T* prVdp = VDP_GetConf(bPath);
		if (prVdp && prVdp->ucB2rId == B2R_1)
		{
			return prVdp->ucStatus;
		}
    }

    return (UINT8) SV_VDO_NOSIGNAL;
}

//-----------------------------------------------------------------------------
/** Brief of u1MpegSdSigStatus.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegSdSigStatus(UINT8 bPath)
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
        VDP_CONF_T* prVdp = VDP_GetConf(bPath);
		if (prVdp && prVdp->ucB2rId == B2R_2)
		{
			return prVdp->ucStatus;
		}
    }
#endif
    return (UINT8)SV_VDO_NOSIGNAL;
}

//-----------------------------------------------------------------------------
/** Brief of u2MpegHdInputWidth.
 */
//-----------------------------------------------------------------------------
UINT16 u2MpegHdInputWidth(void) 
{
    if (_prB2rConf[B2R_1])
    {
        // B2R width issue
        // Work-Around # 1, B2R/PSW Width 64 byte alignment
        return _prB2rConf[B2R_1]->rOutInfo.u4Width;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u2MpegSdInputWidth.
 */
//-----------------------------------------------------------------------------
UINT16 u2MpegSdInputWidth(void) 
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
        // B2R width issue
        // Work-Around # 1, B2R/PSW Width 64 byte alignment
        return _prB2rConf[B2R_2]->rOutInfo.u4Width;
    }
#endif
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u2MpegHdInputValidDataWidth.
 */
//-----------------------------------------------------------------------------
UINT16 u2MpegHdInputValidDataWidth(void) 
{
    if (_prB2rConf[B2R_1])
    {
        // B2R width issue
        return _prB2rConf[B2R_1]->u4SrcWidth;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u2MpegSdInputValidDataWidth.
 */
//-----------------------------------------------------------------------------
UINT16 u2MpegSdInputValidDataWidth(void) 
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
        // B2R width issue
        return _prB2rConf[B2R_2]->u4SrcWidth;
    }
#endif
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u2MpegHdInputHeight.
 */
//-----------------------------------------------------------------------------
UINT16 u2MpegHdInputHeight(void) 
{
    if (_prB2rConf[B2R_1])
    {
        // B2R width issue
        return _prB2rConf[B2R_1]->u4SrcHeight;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u2MpegSdInputHeight.
 */
//-----------------------------------------------------------------------------
UINT16 u2MpegSdInputHeight(void) 
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
        // B2R width issue
        return _prB2rConf[B2R_2]->u4SrcHeight;
    }
#endif
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u1MpegHdRefreshRate.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegHdRefreshRate(void)
{
    if (_prB2rConf[B2R_1])
    {
        return _prB2rConf[B2R_1]->rOutInfo.ucFrameRate;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u1MpegSdRefreshRate.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegSdRefreshRate(void)
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
        // B2R width issue
        return _prB2rConf[B2R_2]->rOutInfo.ucFrameRate;
    }
#endif
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u1MpegHdInterlace.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegHdInterlace(void)
{
    if (_prB2rConf[B2R_1])
    {
        return (_prB2rConf[B2R_1]->rOutInfo.ucPrg ? SV_FALSE : SV_TRUE);
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u1MpegSdInterlace.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegSdInterlace(void)
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
        return (_prB2rConf[B2R_2]->rOutInfo.ucPrg ? SV_FALSE : SV_TRUE);
    }
#endif
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u2MpegHdHTotal.
 */
//-----------------------------------------------------------------------------
UINT16 u2MpegHdHTotal(void)
{
    if (_prB2rConf[B2R_1])
    {
        return _prB2rConf[B2R_1]->rOutInfo.u4TotalWidth;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u2MpegSdHTotal.
 */
//-----------------------------------------------------------------------------
UINT16 u2MpegSdHTotal(void)
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
        return _prB2rConf[B2R_2]->rOutInfo.u4TotalWidth;
    }
#endif
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u2MpegHdVTotal.
 */
//-----------------------------------------------------------------------------
UINT16 u2MpegHdVTotal(void)
{
    if (_prB2rConf[B2R_1])
    {
        return _prB2rConf[B2R_1]->rOutInfo.u4TotalHeight;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u2MpegSdVTotal.
 */
//-----------------------------------------------------------------------------
UINT16 u2MpegSdVTotal(void)
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
        return _prB2rConf[B2R_2]->rOutInfo.u4TotalHeight;
    }
#endif
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u1MpegHdTiming.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegHdTiming(void)
{
    if (_prB2rConf[B2R_1])
    {
    #if 0 /* Consider DTV SCART out, ucPmxMode will be reset */
        switch (_prB2rConf[B2R_1]->rOutInfo.ucPmxMode)
        {
            case VDP_B2R_MODE_480I:
                return MODE_525I;
            
            case VDP_B2R_MODE_576I:
                return MODE_625I;

            case VDP_B2R_MODE_480P_24:
                return MODE_480P_24;

            case VDP_B2R_MODE_480P_30:
                return MODE_480P_30;

            case VDP_B2R_MODE_480P:
                return MODE_480P;

            case VDP_B2R_MODE_576P_25:
                return MODE_576P_25;

            case VDP_B2R_MODE_576P:
                return MODE_576P;

            case VDP_B2R_MODE_720P_24:
                return MODE_720p_24;

            case VDP_B2R_MODE_720P_25:
                return MODE_720p_25;

            case VDP_B2R_MODE_720P_30:
                return MODE_720p_30;

            case VDP_B2R_MODE_720P_50:
                return MODE_720p_50;

            case VDP_B2R_MODE_720P:
                return MODE_720p_60;

            case VDP_B2R_MODE_1080P_24:
                return MODE_1080p_24;

            case VDP_B2R_MODE_1080P_25:
                return MODE_1080p_25;

            case VDP_B2R_MODE_1080P_30:
                return MODE_1080p_30;

            case VDP_B2R_MODE_1080P_50:
                return MODE_1080p_50;

            case VDP_B2R_MODE_1080P:
                return MODE_1080p_60;

            case VDP_B2R_MODE_1080I_50:
                return MODE_1080i_50;

            case VDP_B2R_MODE_1080I:
            default:
                return MODE_1080i;
        }
    #else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 480) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 0))
            {
                return MODE_525I;
            }
            else 
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 576) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 0))
            {
                return MODE_625I;
            }
            else 
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 480) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 24))
            {
                return MODE_480P_24;
            }
            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 480) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 30))
            {
                return MODE_480P_30;
            }
            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 480) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 60))
            {
                return MODE_480P;
            }

            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 576) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 25))
            {
                return MODE_576P_25;
            }

            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 576) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 50))
            {
                return MODE_576P;
            }

            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 720) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 24))
            {
                return MODE_720p_24;
            }
            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 720) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 25))
            {
                return MODE_720p_25;
            }
            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 720) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 30))
            {
                return MODE_720p_30;
            }
            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 720) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 50))
            {
                return MODE_720p_50;
            }
            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 720) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 60))
            {
                return MODE_720p_60;
            }
            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 1080) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 24))
            {
                return MODE_1080p_24;
            }
            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 1080) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 25))
            {
                return MODE_1080p_25;
            }
            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 1080) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 30))
            {
                return MODE_1080p_30;
            }
            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 1080) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 50))
            {
                return MODE_1080p_50;
            }
            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 1080) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 60))
            {
                return MODE_1080p_60;
            }
            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 1080) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 1) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 48))
            {
                return MODE_3D_1080p_24_FP;
            }            
            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 1080) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 0) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 50))
            {
                return MODE_1080i_50;
            }
            else
            if((_prB2rConf[B2R_1]->rOutInfo.u4Height == 1080) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucPrg == 0) &&
               (_prB2rConf[B2R_1]->rOutInfo.ucFrameRate == 60))
            {
                return MODE_1080i;
            }
            else
            {
                return MODE_1080i;
            }
    #endif
    }

    return MODE_525I;
}

//-----------------------------------------------------------------------------
/** Brief of u1MpegSdTiming.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegSdTiming(void)
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
#ifndef CC_B2R_SKYPE_SUPPORT /* Consider DTV SCART out, ucPmxMode will be reset */
        switch (_prB2rConf[B2R_2]->rOutInfo.ucPmxMode)
        {
            case VDP_B2R_MODE_480I:
                return MODE_525I;
            
            case VDP_B2R_MODE_576I:
                return MODE_625I;

            case VDP_B2R_MODE_480P_24:
                return MODE_480P_24;

            case VDP_B2R_MODE_480P_30:
                return MODE_480P_30;

            case VDP_B2R_MODE_480P:
                return MODE_480P;

            case VDP_B2R_MODE_576P_25:
                return MODE_576P_25;

            case VDP_B2R_MODE_576P:
                return MODE_576P;

            case VDP_B2R_MODE_720P_24:
                return MODE_720p_24;

            case VDP_B2R_MODE_720P_25:
                return MODE_720p_25;

            case VDP_B2R_MODE_720P_30:
                return MODE_720p_30;

            case VDP_B2R_MODE_720P_50:
                return MODE_720p_50;

            case VDP_B2R_MODE_720P:
                return MODE_720p_60;

            case VDP_B2R_MODE_1080P_24:
                return MODE_1080p_24;

            case VDP_B2R_MODE_1080P_25:
                return MODE_1080p_25;

            case VDP_B2R_MODE_1080P_30:
                return MODE_1080p_30;

            case VDP_B2R_MODE_1080P_50:
                return MODE_1080p_50;

            case VDP_B2R_MODE_1080P:
                return MODE_1080p_60;

            case VDP_B2R_MODE_1080I_50:
                return MODE_1080i_50;

            case VDP_B2R_MODE_1080I:
            default:
                return MODE_1080i;
        }
#else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 480) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 0))
        {
            return MODE_525I;
        }
        else 
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 576) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 0))
        {
            return MODE_625I;
        }
        else 
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 480) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 1) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 24))
        {
            return MODE_480P_24;
        }
        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 480) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 1) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 30))
        {
            return MODE_480P_30;
        }
        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 480) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 1) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 60))
        {
            return MODE_480P;
        }

        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 576) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 1) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 25))
        {
            return MODE_576P_25;
        }

        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 576) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 1) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 50))
        {
            return MODE_576P;
        }

        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 720) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 1) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 24))
        {
            return MODE_720p_24;
        }
        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 720) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 1) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 25))
        {
            return MODE_720p_25;
        }
        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 720) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 1) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 30))
        {
            return MODE_720p_30;
        }
        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 720) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 1) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 50))
        {
            return MODE_720p_50;
        }
        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 720) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 1) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 60))
        {
            return MODE_720p_60;
        }
        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 1080) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 1) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 24))
        {
            return MODE_1080p_24;
        }
        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 1080) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 1) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 25))
        {
            return MODE_1080p_25;
        }
        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 1080) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 1) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 30))
        {
            return MODE_1080p_30;
        }
        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 1080) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 1) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 50))
        {
            return MODE_1080p_50;
        }
        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 1080) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 1) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 60))
        {
            return MODE_1080p_60;
        }
        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 1080) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 0) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 50))
        {
            return MODE_1080i_50;
        }

        else
        if((_prB2rConf[B2R_2]->rOutInfo.u4Height == 1080) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucPrg == 0) &&
           (_prB2rConf[B2R_2]->rOutInfo.ucFrameRate == 60))
        {
            return MODE_1080i;
        }
        else
        {
            return MODE_1080i;
        }
#endif
    }

#endif
    return MODE_525I;
}


//-----------------------------------------------------------------------------
/** Brief of u1MpegHdClock
 */
//-----------------------------------------------------------------------------

UINT8 u1MpegHdClock(void)
{
    if (_prB2rConf[B2R_1])
    {
        switch (_prB2rConf[B2R_1]->rOutInfo.u4ClkMode)
        {            
            case VDP_B2R_CLK_74_250:
            case VDP_B2R_CLK_74_176:            
                return B2R_CLK_MODE_74;

            case VDP_B2R_CLK_148_500:
            case VDP_B2R_CLK_148_352:
                return B2R_CLK_MODE_148;

            case VDP_B2R_CLK_27_000:
            case VDP_B2R_CLK_27_027:            
            default:            
                return B2R_CLK_MODE_27;
        }
    }

    return B2R_CLK_MODE_27;
}


//-----------------------------------------------------------------------------
/** Brief of u1MpegHdClock
 */
//-----------------------------------------------------------------------------

UINT8 u1MpegSdClock(void)
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
        switch (_prB2rConf[B2R_2]->rOutInfo.u4ClkMode)
        {            
            case VDP_B2R_CLK_74_250:
            case VDP_B2R_CLK_74_176:            
                return B2R_CLK_MODE_74;

            case VDP_B2R_CLK_148_500:
            case VDP_B2R_CLK_148_352:
                return B2R_CLK_MODE_148;

            case VDP_B2R_CLK_27_000:
            case VDP_B2R_CLK_27_027:            
            default:            
                return B2R_CLK_MODE_27;
        }
    }
#endif
    return B2R_CLK_MODE_27;
}

//-----------------------------------------------------------------------------
/** Brief of u4MpegHdQuality.
 */
//-----------------------------------------------------------------------------
UINT32 u4MpegHdQuality(void)
{
    if (_prB2rConf[B2R_1])
    {
        return (_prB2rConf[B2R_1]->u4QpAvg);
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u4MpegSdQuality.
 */
//-----------------------------------------------------------------------------
UINT32 u4MpegSdQuality(void)
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
        return (_prB2rConf[B2R_2]->u4QpAvg);
    }
#endif
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u1MpegHdColorSpace709.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegHdColorSpace709(void)
{
    if (_prB2rConf[B2R_1])
    {
        if (_prB2rConf[B2R_1]->ucColourPrimary == MPEG_COLOR_PRIMARY_709)
        {
            return 1;
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u1MpegSdColorSpace709.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegSdColorSpace709(void)
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
        if (_prB2rConf[B2R_2]->ucColourPrimary == MPEG_COLOR_PRIMARY_709)
        {
            return 1;
        }
    }
#endif
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of u1MpegHdColourPrimary.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegHdColourPrimary(void)
{
    if (_prB2rConf[B2R_1])
    {
        return (_prB2rConf[B2R_1]->ucColourPrimary);
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u1MpegSdColourPrimary.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegSdColourPrimary(void)
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
        return (_prB2rConf[B2R_2]->ucColourPrimary);
    }
#endif
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of u1MpegSdTransCharacter.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegHdTransCharacter(void)
{
    if (_prB2rConf[B2R_1])
    {
        return (_prB2rConf[B2R_1]->ucTransCharacter);
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u1MpegSdTransCharacter.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegSdTransCharacter(void)
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
        return (_prB2rConf[B2R_2]->ucTransCharacter);
    }
#endif
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of u1MpegHdMatrixCoeff.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegHdMatrixCoeff(void)
{
    if (_prB2rConf[B2R_1])
    {
        return (_prB2rConf[B2R_1]->ucMatrixCoeff);
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u1MpegSdMatrixCoeff.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegSdMatrixCoeff(void)
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
        return (_prB2rConf[B2R_2]->ucMatrixCoeff);
    }
#endif
    return 0;
}


//-----------------------------------------------------------------------------
/** Brief of u1MpegHdColourPrimary.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegHdColorDescrip(void)
{
    if (_prB2rConf[B2R_1])
    {
        return (_prB2rConf[B2R_1]->ucColorDescrip);
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u1MpegSdColorDescrip.
 */
//-----------------------------------------------------------------------------
UINT8 u1MpegSdColorDescrip(void)
{
#ifdef SUPPORT_2_B2R
    if (_prB2rConf[B2R_2])
    {
        return (_prB2rConf[B2R_2]->ucColorDescrip);
    }
#endif
    return 0;
}

//-----------------------------------------------------------------------------
/** Brief of u1MpegHdHdeRatio.
 * 10 bit base assumption
 */
//-----------------------------------------------------------------------------
UINT32 u4MpegHdHdeRatio(void)
{
    if (_prB2rConf[B2R_1])
    {
        if(_prB2rConf[B2R_1]->rOutInfo.ucDoubleClk == 0)
        {
            return 1024;
        }
        else if(_prB2rConf[B2R_1]->rOutInfo.ucDoubleClk == 1)
        {
            return 512;
        }
        else if(_prB2rConf[B2R_1]->rOutInfo.ucDoubleClk == 2)
        {
            return 410;
        }
    }

    return 1024;
}

VOID vMpegGet3DInfo(UINT8 u1B2RId,B2R_3D_INFO* prB2R3DInfo)
{
    prB2R3DInfo->fgB2R3DEnable = (_prB2rPrm[u1B2RId]->rPortPrm.fgB2R3DEnable && (!_prB2rPrm[u1B2RId]->rPortPrm.fgB2RForce2D));
    prB2R3DInfo->u1B2R3DType = _prB2rPrm[u1B2RId]->rPortPrm.u1B2R3DType;
}




