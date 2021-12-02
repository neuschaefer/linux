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
 * $RCSfile: drv_scaler_3d.c,v $
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

#include "panel.h"
#include "drv_scaler.h"
#include "drv_scaler_drvif.h"
#include "vdo_misc.h"
#include "drv_tdtv_drvif.h"
#include "scpos_debug.h"
#include "hw_di.h"
#include "mute_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
UCHAR* SCPIP_TV3D_TYPE_NAMES[SCPIP_TV3D_TYPE_MAX] = 
{
    DEF_TO_STR(SCPIP_TV3D_INPUT_IS_3D      ), // 0x0
    DEF_TO_STR(SCPIP_TV3D_SHUTTER_OUT      ), // 0x1
    DEF_TO_STR(SCPIP_TV3D_FRAME_INTERLEAVE ), // 0x2
    DEF_TO_STR(SCPIP_TV3D_LINE_INTERLEAVE  ), // 0x3
    DEF_TO_STR(SCPIP_TV3D_SBSH_SPLIT       ), // 0x4
    DEF_TO_STR(SCPIP_TV3D_TNB_SPLIT        ), // 0x5
    DEF_TO_STR(SCPIP_TV3D_SBS_Q_SPLIT      ), // 0x6
    DEF_TO_STR(SCPIP_TV3D_SBS_Q_I_SPLIT    ), // 0x7
    DEF_TO_STR(SCPIP_TV3D_TNB_SW_WA        ), // 0x8
    DEF_TO_STR(SCPIP_TV3D_3D_TO_2D_DROP_R  ), // 0x9
    DEF_TO_STR(SCPIP_TV3D_3D_TO_2D_SBS     ), // 0xa
    DEF_TO_STR(SCPIP_TV3D_3D_TO_2D_SBS_Q   ), // 0xb
    DEF_TO_STR(SCPIP_TV3D_3D_TO_2D_SBS_Q_I ), // 0xc
    DEF_TO_STR(SCPIP_TV3D_3D_TO_2D_TNB     ), // 0xd
    DEF_TO_STR(SCPIP_TV3D_3D_TO_2D_FS      ), // 0xe
    DEF_TO_STR(SCPIP_TV3D_3D_REPEAT        ), // 0xf
    DEF_TO_STR(SCPIP_TV3D_3D_REPEAT_DS_HALF), // 0x10
    DEF_TO_STR(SCPIP_TV3D_FHD_PR           ), // 0x11
    DEF_TO_STR(SCPIP_TV3D_FHD_PR_USE_V2    )  // 0x12        
};

EXTERN MJC_CLIP_INFO_T rMjcClipInfo;

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static UINT32 _u4ScpipTv3dFlg[VDP_NS] = {0, 0};
static E_TDTV_DECODER_INPUT _rScpipTv3dInType[VDP_NS] = {E_TDTV_DECODER_INPUT_END, E_TDTV_DECODER_INPUT_END};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void _vScpipUpdate3DPROutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DFPROutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DSBSOutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DSGOutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DTo2DOutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
//static void _vScpipUpdate3DFSInPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
//static void _vScpipUpdate3DSBSInPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
//static void _vScpipUpdate3DTABInPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);            
//static void _vScpipUpdate3DLIInPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);            
static void _vScpipUpdate3DFSInFPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DSBSInFPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DTABInFPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DLIInFPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);            
static void _vScpipUpdate3DFSInSBSOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);            
static void _vScpipUpdate3DSBSInSBSOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);            
static void _vScpipUpdate3DTABInSBSOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DLIInSBSOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DFSInSGOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DSBSInSGOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DTABInSGOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);            
static void _vScpipUpdate3DLIInSGOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DFSIn2DOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DSBSIn2DOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DTABIn2DOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DLIIn2DOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static void _vScpipUpdate3DScalingFactor(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm);
static UINT8 _bScpipIsScalerInterlaceInput(UINT32 u4VdpId);
//for set 3D-related register
static void _vScpipSet3DIOConvertReg(UINT32 u4VdpId);
static void _vScpipSet3DPRUpReg(UINT32 u4VdpId);

#if defined(SUPPORT_TV3D_OVERSCAN)
static void _vScpipUpdateMJC3DOverscanInfo(UINT32 u4VdpId);
#endif
//-----------------------------------------------------------------------------
// Global functions
//-----------------------------------------------------------------------------
void vScpipSetTdtvInType(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in);
SCALER_TDTV_IN_TYPE eScpipGetTdtvInType(UINT32 u4VdpId);
void vScpipSetTdtvOutType(UINT32 u4VdpId, SCALER_TDTV_OUT_TYPE in);
SCALER_TDTV_OUT_TYPE eScpipGetTdtvOutType(UINT32 u4VdpId);
void vScpipSetTdtvResolutionType(UINT32 u4VdpId, SCALER_TDTV_RES_POSITION pos, E_TDTV_RES_OUTPUT type);
E_TDTV_RES_OUTPUT eScpipGetTdtvResolutionType(UINT32 u4VdpId, SCALER_TDTV_RES_POSITION pos);

UINT32 u4ScpipGetTDTVPRUpFlag(UINT32 u4VdpId);
void vScpipUpdateUs3DFieldFlag(UINT32 u4VdpId);
void vScpipUpdate3DPRUpFlag(UINT32 u4VdpId);

BOOL fgIsScpipTv3dFlgSet(UINT32 u4VdpId, UINT32 u4Flag)
{
    if(u4VdpId >= VDP_NS)
    {
        u4VdpId = VDP_2;

    }
    return ((_u4ScpipTv3dFlg[u4VdpId] & u4Flag) == u4Flag);
}

void vScpipTv3dSetFlg(UINT32 u4VdpId, UINT32 u4Flag)
{
    if(u4VdpId >= VDP_NS)
    {
        u4VdpId = VDP_2;
    }

    _u4ScpipTv3dFlg[u4VdpId] |= u4Flag;
}

void vScpipTv3dClrFlg(UINT32 u4VdpId, UINT32 u4Flag)
{
    if(u4VdpId >= VDP_NS)
    {
        u4VdpId = VDP_2;
    }

    _u4ScpipTv3dFlg[u4VdpId] &= (~u4Flag);
}

E_TDTV_DECODER_INPUT rScpipTv3dGetInType(UINT32 u4VdpId)
{
    if(u4VdpId >= VDP_NS)
    {
        u4VdpId = VDP_2;
    }

    return _rScpipTv3dInType[u4VdpId];
}

/*******************
 * 3DTV *
 *******************/
void vScpipSetTdtvIO(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, SCALER_TDTV_OUT_TYPE out)
{
    if (u4VdpId >= VDP_NS)
    {
        return;
    }
    
    vScpipSetTdtvInType(u4VdpId, in);
    vScpipSetTdtvOutType(u4VdpId, out);
    vScpipReconfig(u4VdpId);
}

void vScpipSetTdtvInType(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in)
{
    SCALER_TDTV_PRM_T* tdtvInfo;

    tdtvInfo = getTdtvInfo(u4VdpId);
    tdtvInfo->eTdtvInType = in;
}

void vScpipSetTdtvOutType(UINT32 u4VdpId, SCALER_TDTV_OUT_TYPE out)
{
    SCALER_TDTV_PRM_T* tdtvInfo;

    tdtvInfo = getTdtvInfo(u4VdpId);    
    tdtvInfo->eTdtvOutType = out;
}

SCALER_TDTV_IN_TYPE eScpipGetTdtvInType(UINT32 u4VdpId)
{
    SCALER_TDTV_PRM_T* tdtvInfo;
    
    tdtvInfo = getTdtvInfo(u4VdpId);
    return tdtvInfo->eTdtvInType;
}

SCALER_TDTV_OUT_TYPE eScpipGetTdtvOutType(UINT32 u4VdpId)
{
    SCALER_TDTV_PRM_T* tdtvInfo;
    
    tdtvInfo = getTdtvInfo(u4VdpId);
    return tdtvInfo->eTdtvOutType;
}

void vScpipSetTdtvResolutionType(UINT32 u4VdpId, SCALER_TDTV_RES_POSITION pos, E_TDTV_RES_OUTPUT type)
{
    SCALER_TDTV_PRM_T* tdtvInfo;
    E_TDTV_RES_OUTPUT preResType;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    tdtvInfo = getTdtvInfo(u4VdpId);
    preResType = eScpipGetTdtvResolutionType(u4VdpId, pos);    
        
    LOG(1, "Set scaler resolution prm for 3D VDP(%d) position(%d) type(%d) previous type(%d)\n", u4VdpId,pos,type,preResType);
    
    if(preResType == type)
    {
        return;
    }
    
    switch(pos)
    {
        case SCALER_TDTV_RES_POSITION_PRE_H:
                tdtvInfo->ePreH = type;
            break;
        case SCALER_TDTV_RES_POSITION_PRE_V:
                tdtvInfo->ePreV = type;
            break;
        case SCALER_TDTV_RES_POSITION_PST_H:
                tdtvInfo->ePstH = type;
            break;
        case SCALER_TDTV_RES_POSITION_PST_V: 
                tdtvInfo->ePstV = type;
            break;            
        default:
            break;
    };

    vScpipFire3DResChange(u4VdpId);
}

E_TDTV_RES_OUTPUT eScpipGetTdtvResolutionType(UINT32 u4VdpId, SCALER_TDTV_RES_POSITION pos)
{
    E_TDTV_RES_OUTPUT ret = E_TD_RES_NORMAL;
    SCALER_TDTV_PRM_T* tdtvInfo;
    
    tdtvInfo = getTdtvInfo(u4VdpId);

    switch(pos)
    {
        case SCALER_TDTV_RES_POSITION_PRE_H:
            ret = tdtvInfo->ePreH;
            break;
        case SCALER_TDTV_RES_POSITION_PRE_V:
            ret = tdtvInfo->ePreV;
            break;
        case SCALER_TDTV_RES_POSITION_PST_H:
            ret = tdtvInfo->ePstH;
            break;
        case SCALER_TDTV_RES_POSITION_PST_V: 
            ret = tdtvInfo->ePstV;
            break;            
        default:
            break;
    };  

    return ret;
}

void vScpipUpdateTdtvInfo(UINT32 u4VdpId, UINT32 u4TdtvPrm)
{
    
    E_TD_IN tdtvIn;
    E_TD_OUT tdtvOut;
    SCALER_TDTV_IN_TYPE sclIn;
    SCALER_TDTV_OUT_TYPE sclOut;
    UINT8 bUpdate = SV_FALSE;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }
    
    //PRE_H
    if(eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_H) != TD_SCL_PRE_H(u4TdtvPrm))
    {
        vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_H, TD_SCL_PRE_H(u4TdtvPrm));
        bUpdate = SV_TRUE;
    }
    //PRE_V    
    if(eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_V) != TD_SCL_PRE_V(u4TdtvPrm))
    {
        vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_V, TD_SCL_PRE_V(u4TdtvPrm));
        bUpdate = SV_TRUE;
    }
    //PST_H
    if(eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_H) != TD_SCL_PST_H(u4TdtvPrm))
    {
        vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_H, TD_SCL_PST_H(u4TdtvPrm));
        bUpdate = SV_TRUE;        
    }
    //PST_V  
    if(eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_V) != TD_SCL_PST_V(u4TdtvPrm))
    {
        vScpipSetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_V, TD_SCL_PST_V(u4TdtvPrm));
        bUpdate = SV_TRUE;
    }
    
    tdtvIn = TD_SCL_IN(u4TdtvPrm);
    tdtvOut = TD_SCL_OUT(u4TdtvPrm);    

    switch(tdtvIn)
    {
        case E_TD_IN_AUTO:
        case E_TD_IN_RESERVE:
        case E_TD_IN_2D_I:   
        case E_TD_IN_2D_P:
            sclIn = SCALER_TDTV_IN_TYPE_2D;
            break;
        case E_TD_IN_FS_I:   
        case E_TD_IN_FS_P:   
        case E_TD_IN_FP_I:   
        case E_TD_IN_FP_P:
            sclIn = SCALER_TDTV_IN_TYPE_FS;
            break;
        case E_TD_IN_TB_I:
        case E_TD_IN_TB_P: 
            sclIn = SCALER_TDTV_IN_TYPE_TAB;
            break;
        case E_TD_IN_SBS_I:  
        case E_TD_IN_SBS_P:
            sclIn = SCALER_TDTV_IN_TYPE_SBS;
            break;
        case E_TD_IN_LI_I:   
        case E_TD_IN_LI_P:
            sclIn = SCALER_TDTV_IN_TYPE_LI;
            break;
        default:
            sclIn = SCALER_TDTV_IN_TYPE_2D;
            break;
    };

    if(sclIn != eScpipGetTdtvInType(u4VdpId))
    {
        vScpipSetTdtvInType(u4VdpId, sclIn);
        bUpdate = SV_TRUE;        
    }

    switch(tdtvOut)
    {
        case E_TD_OUT_NATIVE:
            sclOut = SCALER_TDTV_OUT_TYPE_2D;
            break;
        case E_TD_OUT_3D_FS:
            sclOut = SCALER_TDTV_OUT_TYPE_SG;
            break;
        case E_TD_OUT_3D_LI:
            sclOut = SCALER_TDTV_OUT_TYPE_PR;
            break;            
        case E_TD_OUT_3D_SBS:
            sclOut = SCALER_TDTV_OUT_TYPE_SBS;
            break;
        case E_TD_OUT_3D_FPR:   
            sclOut = SCALER_TDTV_OUT_TYPE_FPR;
            break;
        default:
            sclOut = SCALER_TDTV_OUT_TYPE_2D;
            break;
    };

    if(sclOut != eScpipGetTdtvOutType(u4VdpId))
    {
        vScpipSetTdtvOutType(u4VdpId,sclOut);
        bUpdate = SV_TRUE;        
    }

    if(bUpdate == SV_TRUE)
    {
        #if defined(SCPIP_TURNOFF_IMPORT_AT_MODCHG)
        //when 3D mode changed, turn off import. After mode change done, import will be turned on.
        vScpipTurnOffImportProtection(getScalerPath(u4VdpId));
        LOG(0,"3D Mode Chg. Turn Off import protection\n");    
        vScpipOnVdoModeChangeDone(getScalerPath(u4VdpId));
        #endif
        _vDrvVideoSetMute(MUTE_MODULE_SCPOS_TV3D, u4VdpId, VDP_SCPOS_TV3D_MUTE_DELAY, TRUE);
    }
    
    vScpipFire3DResChange(u4VdpId);    
}

void vScpipFire3DResChange(UINT32 u4VdpId)
{
    if (u4VdpId == VDP_1)
    {
        vSetScposFlg(MAIN_3D_RES_CHG);
    }
    else if (u4VdpId == VDP_2)
    {
        vSetScposFlg(PIP_3D_RES_CHG);
    }    
}


#if defined(SUPPORT_TV3D_OVERSCAN)
#if defined(CC_MT5396)
extern UINT8 fgMJCModeChg;
#endif
static void _vScpipUpdateMJC3DOverscanInfo(UINT32 u4VdpId)
{    
    UINT8 bPath;
    RPicInfo* picInfo;
    RDispInfo* dispInfo;
    E_TDTV_RES_OUTPUT pstHType;
    E_TDTV_RES_OUTPUT pstVType;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    bPath = getScalerPath(u4VdpId);
    picInfo = getPicInfo(bPath);
    dispInfo = getDispInfo(bPath);
    pstHType = eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_H);
    pstVType = eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_V);
    
    if(vScpipIs3DDoneByMJC() == SV_TRUE)
    {
        /***[MJC width info]***/        
        rMjcClipInfo.wTargetWidth = dispInfo->wWidth;                           //assign real target width
        rMjcClipInfo.wWidth = ALIGN_32_CEIL(rMjcClipInfo.wTargetWidth);         //assign 32 aligned width
        dispInfo->wWidth = ALIGN_32_CEIL(dispInfo->wWidth);                     //up to 32 aligned width
        rMjcClipInfo.wClipX = 0;                                                //?? should be (wWidth - wTargetWidth)
        rMjcClipInfo.wLeft = 0;                                                 //H overscan currently done by PSC
        rMjcClipInfo.wRight = 0;                                                //H overscan currently done by PSC

        if(pstHType == E_TD_RES_HALF)
        {
            rMjcClipInfo.wWidth = rMjcClipInfo.wWidth>>1;                       //When SBS, width/2
        }
        else if(pstHType == E_TD_RES_DOUBLE)
        {
            rMjcClipInfo.wWidth = rMjcClipInfo.wWidth<<1;                       //no use currently
        }

        /***[MJC Height info]***/
        if (pstVType == E_TD_RES_HALF)
        {
            rMjcClipInfo.wTargetHeight = dispInfo->wHeight >> 1;                //no use currently
        }
        else if (pstVType == E_TD_RES_DOUBLE)
        {
            rMjcClipInfo.wTargetHeight = dispInfo->wHeight << 1;                //When TAB, height * 2
        }
        else
        {
            rMjcClipInfo.wTargetHeight = dispInfo->wHeight;
        }

        rMjcClipInfo.wHeight = ALIGN_16_CEIL(rMjcClipInfo.wTargetHeight);       //16 Aligned height        
        dispInfo->wHeight = DIVIDE_WITH_ROUND(dispInfo->wHeight * picInfo->wSrcH, picInfo->wTargetH);               // display height after consider v overscan
        rMjcClipInfo.wTop = DIVIDE_WITH_ROUND(rMjcClipInfo.wHeight * picInfo->wYOffset, picInfo->wTargetH);         // MJC top overscan
        rMjcClipInfo.wBottom = DIVIDE_WITH_ROUND(rMjcClipInfo.wHeight * (picInfo->wSrcH - picInfo->wYOffset - picInfo->wTargetH), picInfo->wTargetH);// MJC bottom overscan        

        if (pstVType == E_TD_RES_HALF)
        {
            rMjcClipInfo.wTop = rMjcClipInfo.wTop<<1; 
            rMjcClipInfo.wBottom = rMjcClipInfo.wBottom<<1;        
        }
        else if (pstVType == E_TD_RES_DOUBLE)
        {
            rMjcClipInfo.wTop = rMjcClipInfo.wTop>>1; 
            rMjcClipInfo.wBottom = rMjcClipInfo.wBottom>>1;
        }

        // MJC clip on bottom
        rMjcClipInfo.wClipY = rMjcClipInfo.wHeight - rMjcClipInfo.wTargetHeight;

        #if defined(CC_MT5396)
        fgMJCModeChg = SV_TRUE;        
        #endif
    }
}
#endif

void vScpipUpdateSrm3DResolution(UINT32 u4VdpId)
{
    UINT8 bPath;
    RPicInfo* picInfo;
    RDispInfo* dispInfo;
    E_TDTV_RES_OUTPUT preHType;
    E_TDTV_RES_OUTPUT preVType;
    E_TDTV_RES_OUTPUT pstVType;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    bPath = getScalerPath(u4VdpId);
    picInfo = getPicInfo(bPath);
    dispInfo = getDispInfo(bPath);
    
    //PRE H
    preHType = eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_H);
    if(preHType == E_TD_RES_HALF)
    {
        picInfo->wHTotal = picInfo->wHTotal>>1;
        picInfo->wSrcW = picInfo->wSrcW>>1;
        picInfo->wTargetW = picInfo->wTargetW>>1;
        picInfo->wHPorch = picInfo->wHPorch>>1;
        picInfo->wXOffset= picInfo->wXOffset>>1;
    }
    else if(preHType == E_TD_RES_DOUBLE)
    {
        picInfo->wHTotal = picInfo->wHTotal<<1;
        picInfo->wSrcW = picInfo->wSrcW<<1;
        picInfo->wTargetW = picInfo->wTargetW<<1;
        picInfo->wHPorch = picInfo->wHPorch<<1;    
        picInfo->wXOffset= picInfo->wXOffset<<1;
    }

    //PRE V
    preVType = eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PRE_V);
    if(preVType == E_TD_RES_HALF)
    {
        picInfo->wVTotal = picInfo->wVTotal>>1;
        picInfo->wSrcH = picInfo->wSrcH>>1;
        picInfo->wTargetH = picInfo->wTargetH>>1;
        picInfo->wVPorch = picInfo->wVPorch>>1;
        picInfo->wYOffset= picInfo->wYOffset>>1;        
    }
    else if(preVType == E_TD_RES_DOUBLE)
    {
        picInfo->wVTotal = picInfo->wVTotal<<1;
        picInfo->wSrcH = picInfo->wSrcH<<1;
        picInfo->wTargetH = picInfo->wTargetH<<1;
        picInfo->wVPorch = picInfo->wVPorch<<1;    
        picInfo->wYOffset = picInfo->wYOffset<<1;    
    }

    //PST V
    pstVType = eScpipGetTdtvResolutionType(u4VdpId, SCALER_TDTV_RES_POSITION_PST_V);
    if(pstVType == E_TD_RES_HALF)    //means MJC will make vertical to half, so scaler have to up scale to double height of panel.
    {
        dispInfo->wYOffset= dispInfo->wYOffset<<1;
        dispInfo->wHeight = dispInfo->wHeight<<1;
    }
    else if(pstVType == E_TD_RES_DOUBLE) //means MJC will make vertical double, so scaler have to down to half of panel size
    {
        dispInfo->wYOffset= dispInfo->wYOffset>>1;    
        dispInfo->wHeight = dispInfo->wHeight>>1;
    }

    #if defined(SUPPORT_TV3D_OVERSCAN)
    _vScpipUpdateMJC3DOverscanInfo(u4VdpId);
    #endif

    vScpipReconfig(u4VdpId);
}


UINT32 u4ScpipGetTDTVPRUpFlag(UINT32 u4VdpId)
{
    SCALER_TDTV_PRM_T* tdtvInfo;
    
    tdtvInfo = getTdtvInfo(u4VdpId);
    return tdtvInfo->u4IsPRUp;
}

UINT8 vScpipIs3DDoneByScaler(UINT32 u4VdpId)
{
    UINT8 ret = SV_FALSE;    
    UINT8 bPath;
    
    bPath = getScalerPath(u4VdpId);

    if (u4VdpId >= VDP_NS)
    {
        return ret;
    }
    
    if(eScpipGetTdtvInType(u4VdpId) != SCALER_TDTV_IN_TYPE_2D && 
        eScpipGetTdtvOutType(u4VdpId) != SCALER_TDTV_OUT_TYPE_2D &&
        getScalerMode(bPath) == VDP_SCPOS_DISPMODE_OFF) //dram mode
    {
        ret = SV_TRUE;
    }

    return ret;
}

UINT8 vScpipIs3DDoneByMJC(void)
{
    UINT8 ret = SV_FALSE;    
    SCALER_TDTV_IN_TYPE in;
    SCALER_TDTV_OUT_TYPE out;
#if defined(CC_MT5396)
    //determine if current mode is TTD mode.
	UINT32 u4TTD;
    E_TD_IN eTTDIn;
    E_TD_OUT eTTDOut;
#endif    

    in = eScpipGetTdtvInType(SV_VP_MAIN);
    out = eScpipGetTdtvOutType(SV_VP_MAIN);
    
    if((in != SCALER_TDTV_IN_TYPE_2D) &&
        (out == SCALER_TDTV_OUT_TYPE_2D))
    {
        ret = SV_TRUE;
    }

    #if defined(CC_MT5396)
    //determine if current mode is TTD mode.

    u4TTD = u4DrvTDTVTTDModeQuery();
    eTTDIn  = TD_TTD_IN(u4TTD);
    eTTDOut = TD_TTD_OUT(u4TTD);    
    if ( ((eTTDIn == E_TD_IN_2D_P) && (eTTDOut == E_TD_OUT_3D_FS)) ||
         ((eTTDIn == E_TD_IN_FS_P) && ((eTTDOut == E_TD_OUT_3D_FS) || (eTTDOut == E_TD_OUT_NATIVE))))
    {
        ret = SV_TRUE;
    }
    #endif
        
    return ret;    
}

void vScpipSetDispROutInv(UINT32 bOnOff)
{
    _arScalePrm[SV_VP_MAIN].u4IsLRDataSwap = bOnOff;
    vRegWriteFldAlign(SCPIP_DRAM_M_13, _arScalePrm[SV_VP_MAIN].u4FlipEnable^_arScalePrm[SV_VP_MAIN].u4IsLRDataSwap, DRAM_M_13_DA_3D_OUTPUT_LR_SWAP);
}

void vScpipUpdate3DPrm(UINT32 u4VdpId)
{    
    SCALER_TDTV_IN_TYPE in;
    SCALER_TDTV_OUT_TYPE out;
    UINT8 bPath;
    RPicInfo* picInfo;
    RDispInfo* dispInfo;
    RDispPrm* dispPrm;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    in = eScpipGetTdtvInType(u4VdpId);
    out = eScpipGetTdtvOutType(u4VdpId);
    bPath = getScalerPath(u4VdpId);
    picInfo = getPicInfo(bPath);
    dispInfo = getDispInfo(bPath);
    dispPrm = getDispPrm(bPath);

    if(picInfo->wHTotal == 0 || picInfo->wVTotal == 0 ||
       picInfo->wSrcW == 0 || picInfo->wSrcH == 0 || 
       picInfo->wTargetW == 0 || picInfo->wTargetH == 0)
    {
        return;
    }

    if(dispInfo->wWidth == 0 || dispInfo->wHeight == 0)
    {
        return;
    }
    
    switch(out)
    {
        case SCALER_TDTV_OUT_TYPE_PR:
            _vScpipUpdate3DPROutPrm(u4VdpId,in, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_OUT_TYPE_FPR:
            _vScpipUpdate3DFPROutPrm(u4VdpId,in, picInfo, dispInfo, dispPrm);            
            break;
        case SCALER_TDTV_OUT_TYPE_SBS:
            _vScpipUpdate3DSBSOutPrm(u4VdpId,in, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_OUT_TYPE_SG:
            _vScpipUpdate3DSGOutPrm(u4VdpId,in, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_OUT_TYPE_3D_TO_2D:
            _vScpipUpdate3DTo2DOutPrm(u4VdpId,in, picInfo, dispInfo, dispPrm);
            break;
        default:
            //do nothing
            break;
    }
    _vScpipUpdate3DScalingFactor(u4VdpId, picInfo, dispInfo, dispPrm);

    //workaround for FPR flip
    if(BSP_GetIcVersion() == IC_VER_5396_AA || BSP_GetIcVersion() == IC_VER_5368_AA)
    {
        if(out == SCALER_TDTV_OUT_TYPE_FPR)
        {
            if(_arScalePrm[u4VdpId].u4FlipEnable == SV_TRUE)
            {
                dispPrm->u2DramReadHeight = dispPrm->u2DramReadHeight << 1;
            }
        }
    }
}

void vScpipSet3DReg(UINT32 u4VdpId)
{
    _vScpipSet3DIOConvertReg(u4VdpId);
    _vScpipSet3DPRUpReg(u4VdpId);
}

static void _vScpipSet3DIOConvertReg(UINT32 u4VdpId)
{    
    UINT8 bPath; 
    UINT16 dramOffset;
    SCALER_TDTV_IN_TYPE in;
    SCALER_TDTV_OUT_TYPE out;
    
    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    bPath  = getScalerPath(u4VdpId);
    dramOffset = getDramOffset(bPath);    
    in = eScpipGetTdtvInType(u4VdpId);
    out = eScpipGetTdtvOutType(u4VdpId);
    
    if(getScalerMode(bPath) != VDP_SCPOS_DISPMODE_OFF)  //not in dram mode
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 0, DRAM_M_13_DA_3D_INPUT_EN);
    }
    else if(in == SCALER_TDTV_IN_TYPE_2D)               //input is 2D
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 0, DRAM_M_13_DA_3D_INPUT_EN);
    }
    else                                                //input is 3D & scaler is in dram mode
    {
        if(vScpipIs3DDoneByScaler(u4VdpId) == SV_TRUE)
        {
            vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 1, DRAM_M_13_DA_3D_INPUT_EN);    
            vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, ((UINT8)in)-1, DRAM_M_13_DA_3D_INPUT_MODE);            
        }
        else if(vScpipIs3DDoneByMJC() == SV_TRUE && in == SCALER_TDTV_IN_TYPE_FS)
        {
            vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 1, DRAM_M_13_DA_3D_INPUT_EN);    
            vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 0, DRAM_M_13_DA_3D_INPUT_MODE);
        }
    }

    if(getScalerMode(bPath) != VDP_SCPOS_DISPMODE_OFF)  //not in dram mode
    {
        vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 0, DRAM_M_13_DA_3D_OUTPUT_EN);        
        vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 0, DRAM_M_13_DA_WRITE_3D_TO_2D_EN);
    }    
    else if(out == SCALER_TDTV_OUT_TYPE_2D)             //output is 2D
    {
        //if input is frame sequential and MJC do 3D. Scaler need to set as FS in SG out.
        if(vScpipIs3DDoneByMJC() == SV_TRUE && in == SCALER_TDTV_IN_TYPE_FS)
        {
            vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 1, DRAM_M_13_DA_3D_OUTPUT_EN);        
            vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 0, DRAM_M_13_DA_WRITE_3D_TO_2D_EN);
            vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 3, DRAM_M_13_DA_3D_OUTPUT_MODE);            
        }
        else
        {
            vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 0, DRAM_M_13_DA_3D_OUTPUT_EN);        
            vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 0, DRAM_M_13_DA_WRITE_3D_TO_2D_EN);            
        }
    }
    else                                                //output is 3D
    {
        if(vScpipIs3DDoneByScaler(u4VdpId) == SV_TRUE)
        {
            if(out == SCALER_TDTV_OUT_TYPE_3D_TO_2D)    //3D to 2D
            {
                vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 0, DRAM_M_13_DA_3D_OUTPUT_EN);
                vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 1, DRAM_M_13_DA_WRITE_3D_TO_2D_EN);            
            }
            else    //3D convert
            {
                vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 1, DRAM_M_13_DA_3D_OUTPUT_EN);    
                vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, 0, DRAM_M_13_DA_WRITE_3D_TO_2D_EN);
                vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset, ((UINT8)out)-1, DRAM_M_13_DA_3D_OUTPUT_MODE);            
            }
        }
    }
}

static void _vScpipSet3DPRUpReg(UINT32 u4VdpId)
{
    UINT32 u4PRUpFlag;
    UINT8 bPath;
    UINT16 usOffset;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    u4PRUpFlag = u4ScpipGetTDTVPRUpFlag(u4VdpId);
    bPath = getScalerPath(u4VdpId);
    usOffset = getUSOffset(bPath);
    
    vRegWriteFldAlign(SCPIP_US1_13+usOffset,(u4PRUpFlag==SV_TRUE)?1:0, US1_13_US_3DTV_PR_MODE);
    if (BSP_GetIcVersion() >= IC_VER_5389_AA && BSP_GetIcVersion() <= IC_VER_5389_AC)
    {
        vRegWriteFldAlign(SCPIP_US1_1E+usOffset, (u4PRUpFlag==SV_TRUE)?1:0, US1_1E_US422_MODE);
    }

    //workaround for us sram power down in ES verision
    if( BSP_GetIcVersion() == IC_VER_5396_AA || 
        BSP_GetIcVersion() == IC_VER_5368_AA || 
       (BSP_GetIcVersion() >= IC_VER_5389_AA && BSP_GetIcVersion() <= IC_VER_5389_AC))
    {
        vRegWriteFldAlign(SCPIP_US1_01+usOffset,(u4PRUpFlag==SV_TRUE)?0:1, US1_01_VUS_SRAM_CS_SEL_1);
    }
}

#if 0    
static void _vScpipUpdate3DPROutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    
    UINT16 dramOffset = getDramOffset(u4VdpId);

    if (u4VdpId >= VDP_NS)
    {
        return;
    }
    
    vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset,(_arScalePrm[u4VdpId].u4FlipEnable)?1:0, DRAM_M_13_DA_3D_OUTPUT_LR_SWAP);
    switch(in)
    {
        case SCALER_TDTV_IN_TYPE_FS:
            _vScpipUpdate3DFSInPROutPrm(u4VdpId, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_IN_TYPE_SBS:
            _vScpipUpdate3DSBSInPROutPrm(u4VdpId, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_IN_TYPE_TAB:
            _vScpipUpdate3DTABInPROutPrm(u4VdpId, picInfo, dispInfo, dispPrm);            
            break;
        case SCALER_TDTV_IN_TYPE_LI:
            _vScpipUpdate3DLIInPROutPrm(u4VdpId, picInfo, dispInfo, dispPrm);            
            break;
        default:
            //do nothing
            break;
    }
}
#endif
static void _vScpipUpdate3DPROutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    UINT16 dramOffset;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }
    
    dramOffset = getDramOffset(u4VdpId);
    
    vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset,_arScalePrm[u4VdpId].u4FlipEnable^_arScalePrm[u4VdpId].u4IsLRDataSwap, DRAM_M_13_DA_3D_OUTPUT_LR_SWAP);
    

    //[Dram Write Width]
    if(dispInfo->wWidth < _arScalePrm[u4VdpId].u4MddiFifoMin)           //output window width < 180
    {
        dispPrm->u2DramWriteWidth = dispInfo->wWidth;                   //hps will predown to 180 then down to output window width
    }
    else
    {
        dispPrm->u2DramWriteWidth = _arScalePrm[u4VdpId].u4HpsWidth;    //other case, write hps width
    }

    if(in == SCALER_TDTV_IN_TYPE_SBS)
    {
        dispPrm->u2DramWriteWidth = ALIGN_4_CEIL(dispPrm->u2DramWriteWidth);  //align to 4
    }
    else
    {
        dispPrm->u2DramWriteWidth = ALIGN_2_CEIL(dispPrm->u2DramWriteWidth);  //align to 2
    }

    //[LF]
    if(in == SCALER_TDTV_IN_TYPE_SBS)
    {
        dispPrm->wHPosOfst = dispPrm->wHPosOfst >> 1;
    }
    dispPrm->wHPosOfst = ALIGN_2_CEIL(dispPrm->wHPosOfst);

    //[Dram Write Height]
    if(in == SCALER_TDTV_IN_TYPE_FS || in == SCALER_TDTV_IN_TYPE_SBS)
    {
        dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, ((picInfo->wSrcH * dispInfo->wHeight) / picInfo->wTargetH) >> 1);
    }
    else
    {
        dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, ((picInfo->wSrcH * dispInfo->wHeight) / picInfo->wTargetH));
    }
        
    //[Dram Read Width]
    if(dispInfo->wWidth < _arScalePrm[u4VdpId].u4MddiFifoMin)
    {
        dispPrm->u2DramReadWidth =  (dispPrm->u2DramWriteWidth  * _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth) / _arScalePrm[u4VdpId].u4HpsWidth;        
    }
    else
    {
        dispPrm->u2DramReadWidth = _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
    }

    if(in == SCALER_TDTV_IN_TYPE_SBS)
    {
        dispPrm->u2DramReadWidth = dispPrm->u2DramReadWidth >> 1;
    }
    dispPrm->u2DramReadWidth = ALIGN_2_CEIL(dispPrm->u2DramReadWidth);

    //[Dram Read Height]
    if(in == SCALER_TDTV_IN_TYPE_FS || in == SCALER_TDTV_IN_TYPE_SBS)
    {
        dispPrm->u2DramReadHeight = DIVIDE_WITH_ROUND(dispPrm->u2DramWriteHeight * picInfo->wTargetH ,picInfo->wSrcH) << 1;
    }
    else    //TAB & LI
    {
        dispPrm->u2DramReadHeight = DIVIDE_WITH_ROUND(dispPrm->u2DramWriteHeight * picInfo->wTargetH , picInfo->wSrcH);    
    }

    //[UP]
    if(in == SCALER_TDTV_IN_TYPE_FS || in == SCALER_TDTV_IN_TYPE_SBS)
    {
        dispPrm->wVPosOfst =  (dispPrm->u2DramWriteHeight * dispPrm->wVPosOfst)/ picInfo->wSrcH;
    }
    else
    {
        dispPrm->wVPosOfst = ((dispPrm->u2DramWriteHeight * dispPrm->wVPosOfst)/ picInfo->wSrcH) >> 1;
    }

    //[DN]
    if(in == SCALER_TDTV_IN_TYPE_FS || in == SCALER_TDTV_IN_TYPE_SBS)
    {
        dispPrm->wOverscanBottom  = (dispPrm->u2DramWriteHeight - (dispPrm->u2DramReadHeight >> 1) - dispPrm->wVPosOfst);
    }
    else
    {
        dispPrm->wOverscanBottom  = (dispPrm->u2DramWriteHeight - dispPrm->u2DramReadHeight - (dispPrm->wVPosOfst << 1)) >> 1;
    }

    //[RT]
    if(in == SCALER_TDTV_IN_TYPE_SBS)
    {
        dispPrm->wOverscanRight = (dispPrm->u2DramWriteWidth>> 1) - dispPrm->u2DramReadWidth - dispPrm->wHPosOfst;    
    }
    else
    {
        dispPrm->wOverscanRight  = dispPrm->u2DramWriteWidth - dispPrm->u2DramReadWidth - dispPrm->wHPosOfst;    
    }
}

static void _vScpipUpdate3DFPROutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{    
    UINT16 dramOffset;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    dramOffset = getDramOffset(u4VdpId);
    
    vRegWriteFldAlign(SCPIP_DRAM_M_13+dramOffset,(_arScalePrm[u4VdpId].u4FlipEnable)?1:0, DRAM_M_13_DA_3D_OUTPUT_LR_SWAP);
    switch(in)
    {
        case SCALER_TDTV_IN_TYPE_FS:
            _vScpipUpdate3DFSInFPROutPrm(u4VdpId, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_IN_TYPE_SBS:
            _vScpipUpdate3DSBSInFPROutPrm(u4VdpId, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_IN_TYPE_TAB:
            _vScpipUpdate3DTABInFPROutPrm(u4VdpId, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_IN_TYPE_LI:
            _vScpipUpdate3DLIInFPROutPrm(u4VdpId, picInfo, dispInfo, dispPrm);            
            break;
        default:
            //do nothing
            break;
    }
    #if 0
    //in ES version FPR out + Flip need to set read height to double
    if(BSP_GetIcVersion() == IC_VER_5368_AA || BSP_GetIcVersion() == IC_VER_5396_AA)
    {
        if(_arScalePrm[u4VdpId].u4FlipEnable == SV_TRUE)
        {
            dispPrm->u2DramReadHeight = dispPrm->u2DramReadHeight<<1;
        }
    }
    #endif
}

static void _vScpipUpdate3DSBSOutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    switch(in)
    {
        case SCALER_TDTV_IN_TYPE_FS:
            _vScpipUpdate3DFSInSBSOutPrm(u4VdpId, picInfo, dispInfo, dispPrm);            
            break;
        case SCALER_TDTV_IN_TYPE_SBS:
            _vScpipUpdate3DSBSInSBSOutPrm(u4VdpId, picInfo, dispInfo, dispPrm);            
            break;
        case SCALER_TDTV_IN_TYPE_TAB:
            _vScpipUpdate3DTABInSBSOutPrm(u4VdpId, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_IN_TYPE_LI:
            _vScpipUpdate3DLIInSBSOutPrm(u4VdpId, picInfo, dispInfo, dispPrm);
            break;
        default:
            //do nothing
            break;
    }
}

static void _vScpipUpdate3DSGOutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    switch(in)
    {
        case SCALER_TDTV_IN_TYPE_FS:
            _vScpipUpdate3DFSInSGOutPrm(u4VdpId, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_IN_TYPE_SBS:
            _vScpipUpdate3DSBSInSGOutPrm(u4VdpId, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_IN_TYPE_TAB:
            _vScpipUpdate3DTABInSGOutPrm(u4VdpId, picInfo, dispInfo, dispPrm);            
            break;
        case SCALER_TDTV_IN_TYPE_LI:
            _vScpipUpdate3DLIInSGOutPrm(u4VdpId, picInfo, dispInfo, dispPrm);            
            break;
        default:
            //do nothing
            break;
    }
}

static void _vScpipUpdate3DTo2DOutPrm(UINT32 u4VdpId, SCALER_TDTV_IN_TYPE in, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    switch(in)
    {
        case SCALER_TDTV_IN_TYPE_FS:
            _vScpipUpdate3DFSIn2DOutPrm(u4VdpId, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_IN_TYPE_SBS:
            _vScpipUpdate3DSBSIn2DOutPrm(u4VdpId, picInfo, dispInfo, dispPrm);
            break;
        case SCALER_TDTV_IN_TYPE_TAB:
            _vScpipUpdate3DTABIn2DOutPrm(u4VdpId, picInfo, dispInfo, dispPrm);            
            break;
        case SCALER_TDTV_IN_TYPE_LI:
            _vScpipUpdate3DLIIn2DOutPrm(u4VdpId, picInfo, dispInfo, dispPrm);            
            break;
        default:
            //do nothing
            break;        
    }
}

#if 0
static void _vScpipUpdate3DFSInPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DFSInPROutPrm\n");
    if (u4VdpId >= VDP_NS)
    {
        return;
    }    
    //update dram write width   
    if(dispInfo->wWidth < _arScalePrm[u4VdpId].u4MddiFifoMin)           //output window width < 180
    {
        dispPrm->u2DramWriteWidth = dispInfo->wWidth;                   //hps will predown to 180 then down to output window width
    }
    else
    {
        dispPrm->u2DramWriteWidth = _arScalePrm[u4VdpId].u4HpsWidth;    //other case, write hps width
    }
    dispPrm->u2DramWriteWidth = ALIGN_2_CEIL(dispPrm->u2DramWriteWidth);  //align to 2
    
    //update dram write height
    dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, ((picInfo->wSrcH * dispInfo->wHeight) / picInfo->wTargetH) >> 1);

    //update h porch
    dispPrm->wHPosOfst = dispPrm->wHPosOfst - (dispPrm->wHPosOfst%2);
    
    //update dram read width
    if(dispInfo->wWidth < _arScalePrm[u4VdpId].u4MddiFifoMin)
    {
        dispPrm->u2DramReadWidth =  (dispPrm->u2DramWriteWidth  * _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth) / _arScalePrm[u4VdpId].u4HpsWidth;        
    }
    else
    {
        dispPrm->u2DramReadWidth = _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
    }    
    
    dispPrm->u2DramReadWidth = ALIGN_2_CEIL(dispPrm->u2DramReadWidth);
        
    //update dram read height
    dispPrm->u2DramReadHeight = DIVIDE_WITH_ROUND(dispPrm->u2DramWriteHeight * picInfo->wTargetH ,picInfo->wSrcH) << 1;
    
    //update top overscan
    dispPrm->wVPosOfst        = (dispPrm->u2DramWriteHeight * dispPrm->wVPosOfst)/ picInfo->wSrcH;
    
    //update overscan for bottom side
    dispPrm->wOverscanBottom  = (dispPrm->u2DramWriteHeight - (dispPrm->u2DramReadHeight >> 1) - dispPrm->wVPosOfst);
    
    //update overscan for right side
    dispPrm->wOverscanRight = dispPrm->u2DramWriteWidth - dispPrm->u2DramReadWidth - dispPrm->wHPosOfst;
}

static void _vScpipUpdate3DSBSInPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DSBSInPROutPrm\n");
    if (u4VdpId >= VDP_NS)
    {
        return;
    }
    //update dram write width   
    if(dispInfo->wWidth < _arScalePrm[u4VdpId].u4MddiFifoMin)           //output window width < 180
    {
        dispPrm->u2DramWriteWidth = dispInfo->wWidth;                   //hps will predown to 180 then down to output window width
    }
    else
    {
        dispPrm->u2DramWriteWidth = _arScalePrm[u4VdpId].u4HpsWidth;    //other case, write hps width
    }
    dispPrm->u2DramWriteWidth = ((dispPrm->u2DramWriteWidth + 3)/4)*4;  //align to 4
    
    //update dram write height
    dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, ((picInfo->wSrcH * dispInfo->wHeight) / picInfo->wTargetH) >> 1);
    
    //update h porch
    dispPrm->wHPosOfst = dispPrm->wHPosOfst >> 1;
    dispPrm->wHPosOfst = dispPrm->wHPosOfst - (dispPrm->wHPosOfst%2);
    
    //update dram read width
    if(dispInfo->wWidth < _arScalePrm[u4VdpId].u4MddiFifoMin)
    {
        dispPrm->u2DramReadWidth =  (dispPrm->u2DramWriteWidth  * _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth) / _arScalePrm[u4VdpId].u4HpsWidth;        
    }
    else
    {
        dispPrm->u2DramReadWidth = _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
    }    
    
    dispPrm->u2DramReadWidth = dispPrm->u2DramReadWidth >> 1;
    dispPrm->u2DramReadWidth = dispPrm->u2DramReadWidth + (dispPrm->u2DramReadWidth%2);
        
    //update dram read height
    dispPrm->u2DramReadHeight = DIVIDE_WITH_ROUND(dispPrm->u2DramWriteHeight * picInfo->wTargetH ,picInfo->wSrcH) << 1;
    
    //update top overscan
    dispPrm->wVPosOfst        = (dispPrm->u2DramWriteHeight * dispPrm->wVPosOfst)/ picInfo->wSrcH;
    
    //update overscan for bottom side
    dispPrm->wOverscanBottom  = (dispPrm->u2DramWriteHeight - (dispPrm->u2DramReadHeight >> 1) - dispPrm->wVPosOfst);
    
    //update overscan for right side
    dispPrm->wOverscanRight = (dispPrm->u2DramWriteWidth>> 1) - dispPrm->u2DramReadWidth - dispPrm->wHPosOfst;
}

static void _vScpipUpdate3DTABInPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DTABInPROutPrm\n");
    if (u4VdpId >= VDP_NS)
    {
        return;
    }    
    //update dram write width   
    if(dispInfo->wWidth < _arScalePrm[u4VdpId].u4MddiFifoMin)
    {
        dispPrm->u2DramWriteWidth = dispInfo->wWidth;
    }
    else
    {
        dispPrm->u2DramWriteWidth = _arScalePrm[u4VdpId].u4HpsWidth;
    }

    dispPrm->wHPosOfst = dispPrm->wHPosOfst - (dispPrm->wHPosOfst%2);
    
    //update dram write height
    dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, ((picInfo->wSrcH * dispInfo->wHeight) / picInfo->wTargetH));
    
    //update dram read width
    //dispPrm->u2DramReadWidth =  (dispPrm->u2DramWriteWidth  * _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth) / _arScalePrm[u4VdpId].u4HpsWidth;
    if(dispInfo->wWidth < _arScalePrm[u4VdpId].u4MddiFifoMin)
    {
        dispPrm->u2DramReadWidth =  (dispPrm->u2DramWriteWidth  * _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth) / _arScalePrm[u4VdpId].u4HpsWidth;        
    }
    else
    {
        dispPrm->u2DramReadWidth = _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
    }
    
    //update dram read height
    dispPrm->u2DramReadHeight = (dispPrm->u2DramWriteHeight * picInfo->wTargetH) / picInfo->wSrcH;    
    
    //update top overscan
    dispPrm->wVPosOfst        =((dispPrm->u2DramWriteHeight * dispPrm->wVPosOfst)/ picInfo->wSrcH) >> 1;
    
    //update bottom overscan
    dispPrm->wOverscanBottom  = (dispPrm->u2DramWriteHeight - dispPrm->u2DramReadHeight - (dispPrm->wVPosOfst << 1)) >> 1;

    //update right overscan
    dispPrm->wOverscanRight   = dispPrm->u2DramWriteWidth - dispPrm->u2DramReadWidth - dispPrm->wHPosOfst;
}

static void _vScpipUpdate3DLIInPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{    
    Printf("_vScpipUpdate3DLIInPROutPrm\n");
    //Line interleave setting is the same with Top & Bottom
    _vScpipUpdate3DTABInPROutPrm(u4VdpId,picInfo,dispInfo,dispPrm);
}
#endif

static void _vScpipUpdate3DFSInFPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DFSInFPROutPrm\n");
    if (u4VdpId >= VDP_NS)
    {
        return;
    }    
    //update dram write width
    dispPrm->u2DramWriteWidth = MIN(_arScalePrm[u4VdpId].u4HpsWidth, dispInfo->wWidth);
    //update dram write height
    if(_bScpipIsScalerInterlaceInput(u4VdpId) == SV_TRUE) // ==> replace this condition by SRM rule.
    {
        dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH>>1, dispInfo->wHeight>>1);
    }
    else
    {
        dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, dispInfo->wHeight);    
    }
    //update dram read width
    dispPrm->u2DramReadWidth = _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
    //update dram read height
    dispPrm->u2DramReadHeight = picInfo->wTargetH;
}

static void _vScpipUpdate3DSBSInFPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DSBSInFPROutPrm\n");
    if (u4VdpId >= VDP_NS)
    {
        return;
    }    
    //update dram write width
    dispPrm->u2DramWriteWidth = MIN(_arScalePrm[u4VdpId].u4HpsWidth, dispInfo->wWidth);
    //update dram write height    
    dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, dispInfo->wHeight);
    //update h porch
    dispPrm->wHPosOfst = dispPrm->wHPosOfst >> 1;
    //update dram read width
    dispPrm->u2DramReadWidth = _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth >> 1;
    //update dram read height
    dispPrm->u2DramReadHeight = picInfo->wTargetH;    
    //make even
    dispPrm->u2DramReadWidth = dispPrm->u2DramReadWidth + (dispPrm->u2DramReadWidth%2);
    dispPrm->wHPosOfst = dispPrm->wHPosOfst - (dispPrm->wHPosOfst%2);    
}

static void _vScpipUpdate3DTABInFPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DTABInFPROutPrm\n");
    if (u4VdpId >= VDP_NS)
    {
        return;
    }    
    //update dram write width
    dispPrm->u2DramWriteWidth = MIN(_arScalePrm[u4VdpId].u4HpsWidth, dispInfo->wWidth);
    //update dram write height
    dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, dispInfo->wHeight);
    //update dram read width
    dispPrm->u2DramReadWidth = _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
    //update dram read height
    dispPrm->u2DramReadHeight = picInfo->wTargetH >>1;    
}

static void _vScpipUpdate3DLIInFPROutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DLIInFPROutPrm\n");
    //Line interleave setting is the same with Top & Bottom
    _vScpipUpdate3DTABInFPROutPrm(u4VdpId, picInfo, dispInfo, dispPrm);
}

static void _vScpipUpdate3DFSInSBSOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DFSInSBSOutPrm\n");
    if (u4VdpId >= VDP_NS)
    {
        return;
    }    
    //update dram write width
    dispPrm->u2DramWriteWidth = MIN(_arScalePrm[u4VdpId].u4HpsWidth, dispInfo->wWidth>>1);
    //update dram write height
    if(_bScpipIsScalerInterlaceInput(u4VdpId) == SV_TRUE) // ==> replace this condition by SRM rule.
    {
        dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH>>1, dispInfo->wHeight>>1);
    }
    else
    {
        dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, dispInfo->wHeight);    
    }

    //update h overscan
    dispPrm->wHPosOfst = (dispPrm->wHPosOfst * dispPrm->u2DramWriteWidth) / _arScalePrm[u4VdpId].u4HpsWidth;
    //update dram read width
    dispPrm->u2DramReadWidth = ((_arScalePrm[u4VdpId].u4HpsAndOverscanedWidth * dispPrm->u2DramWriteWidth) / _arScalePrm[u4VdpId].u4HpsWidth) << 1;    //update dram read height
    //update dram read height
    dispPrm->u2DramReadHeight = picInfo->wTargetH;

    //make even
    dispPrm->wHPosOfst = dispPrm->wHPosOfst - dispPrm->wHPosOfst%2;
    dispPrm->u2DramReadWidth = ALIGN_4_CEIL(dispPrm->u2DramReadWidth);
}

static void _vScpipUpdate3DSBSInSBSOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DSBSInSBSOutPrm\n");
    if (u4VdpId >= VDP_NS)
    {
        return;
    }    
    //update dram write width
    dispPrm->u2DramWriteWidth = MIN(_arScalePrm[u4VdpId].u4HpsWidth, dispInfo->wWidth);
    //update dram write height
    dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, dispInfo->wHeight);
    //update h porch
    dispPrm->wHPosOfst = dispPrm->wHPosOfst >> 1;    
    //update dram read width
    dispPrm->u2DramReadWidth = _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
    //update dram read height
    dispPrm->u2DramReadHeight = picInfo->wTargetH;
    //make even
    dispPrm->wHPosOfst = dispPrm->wHPosOfst - dispPrm->wHPosOfst%2;
    dispPrm->u2DramReadWidth = ALIGN_4_CEIL(dispPrm->u2DramReadWidth);    

    //update overscan for bottom side
    dispPrm->wOverscanBottom = dispPrm->u2DramWriteHeight - dispPrm->u2DramReadHeight - dispPrm->wVPosOfst;
    
    //update overscan for right side
    dispPrm->wOverscanRight = (dispPrm->u2DramWriteWidth - dispPrm->u2DramReadWidth - (dispPrm->wHPosOfst << 1)) >> 1;        
}

static void _vScpipUpdate3DTABInSBSOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DTABInSBSOutPrm\n");
    if (u4VdpId >= VDP_NS)
    {
        return;
    }    
    //update dram write width
    dispPrm->u2DramWriteWidth = MIN(_arScalePrm[u4VdpId].u4HpsWidth, dispInfo->wWidth >> 1);
    //update dram write height
    dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, dispInfo->wHeight);
    //update h overscan
    dispPrm->wHPosOfst = (dispPrm->wHPosOfst * dispPrm->u2DramWriteWidth) / _arScalePrm[u4VdpId].u4HpsWidth;
    //update dram read width
    dispPrm->u2DramReadWidth = ((_arScalePrm[u4VdpId].u4HpsAndOverscanedWidth * dispPrm->u2DramWriteWidth) / _arScalePrm[u4VdpId].u4HpsWidth) << 1;    
    //update dram read height
    dispPrm->u2DramReadHeight = (picInfo->wTargetH * (dispPrm->u2DramWriteHeight / picInfo->wSrcH)) >> 1;        
    //make even
    dispPrm->wHPosOfst = dispPrm->wHPosOfst - dispPrm->wHPosOfst%2;
    dispPrm->u2DramReadWidth = ALIGN_4_CEIL(dispPrm->u2DramReadWidth);

    //update top overscan
    dispPrm->wVPosOfst        =((dispPrm->u2DramWriteHeight * dispPrm->wVPosOfst)/ picInfo->wSrcH) >> 1;
    
    //update bottom overscan
    dispPrm->wOverscanBottom  = (dispPrm->u2DramWriteHeight >> 1) - dispPrm->u2DramReadHeight - dispPrm->wVPosOfst;

    //update right overscan
    dispPrm->wOverscanRight   = dispPrm->u2DramWriteWidth - (dispPrm->u2DramReadWidth >> 1) - dispPrm->wHPosOfst;
}

static void _vScpipUpdate3DLIInSBSOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DLIInSBSOutPrm\n");
    _vScpipUpdate3DTABInSBSOutPrm(u4VdpId, picInfo, dispInfo, dispPrm);
}

static void _vScpipUpdate3DFSInSGOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DFSInSGOutPrm\n");
    if (u4VdpId >= VDP_NS)
    {
        return;
    }    
    //update dram write width
    dispPrm->u2DramWriteWidth = MIN(_arScalePrm[u4VdpId].u4HpsWidth, dispInfo->wWidth);
    //update dram write height
    if(_bScpipIsScalerInterlaceInput(u4VdpId) == SV_TRUE) // ==> replace this condition by SRM rule.
    {
        dispPrm->u2DramWriteHeight = MIN((picInfo->wSrcH >> 1), (dispInfo->wHeight >> 1));

    }
    else
    {
        dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, dispInfo->wHeight);
    }
    //update dram read width      
    dispPrm->u2DramReadWidth = _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;    
    //update dram write height
    dispPrm->u2DramReadHeight = picInfo->wTargetH;
    //make even
    dispPrm->u2DramReadWidth = dispPrm->u2DramReadWidth + (dispPrm->u2DramReadWidth%2);
}

static void _vScpipUpdate3DSBSInSGOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DSBSInSGOutPrm\n");
    if (u4VdpId >= VDP_NS)
    {
        return;
    }    
    //update dram write width
    dispPrm->u2DramWriteWidth = MIN(_arScalePrm[u4VdpId].u4HpsWidth, dispInfo->wWidth);
    //update dram write height
    dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, dispInfo->wHeight);
    //update h overscan
    dispPrm->wHPosOfst = ((dispPrm->wHPosOfst * dispPrm->u2DramWriteWidth) / _arScalePrm[u4VdpId].u4HpsWidth) >> 1;
    //update dram read width
    dispPrm->u2DramReadWidth = ((_arScalePrm[u4VdpId].u4HpsAndOverscanedWidth * dispPrm->u2DramWriteWidth) / _arScalePrm[u4VdpId].u4HpsWidth) >> 1;
    //update drame read height
    dispPrm->u2DramReadHeight = picInfo->wTargetH;    
    //make even
    dispPrm->wHPosOfst = dispPrm->wHPosOfst - (dispPrm->wHPosOfst%2);        
    dispPrm->u2DramReadWidth = dispPrm->u2DramReadWidth + (dispPrm->u2DramReadWidth%2);
}

static void _vScpipUpdate3DTABInSGOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DTABInSGOutPrm\n");
    if (u4VdpId >= VDP_NS)
    {
        return;
    }    
    //update dram write H & V
    dispPrm->u2DramWriteWidth = MIN(_arScalePrm[u4VdpId].u4HpsWidth, dispInfo->wWidth);
    dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, dispInfo->wHeight);
    //update dram read H & V
    dispPrm->u2DramReadWidth = ((_arScalePrm[u4VdpId].u4HpsAndOverscanedWidth * dispPrm->u2DramWriteWidth) / _arScalePrm[u4VdpId].u4HpsWidth);
    dispPrm->u2DramReadHeight = picInfo->wTargetH >> 1;
    //make even
    dispPrm->wHPosOfst = dispPrm->wHPosOfst - (dispPrm->wHPosOfst%2);        
    dispPrm->u2DramReadWidth = dispPrm->u2DramReadWidth + (dispPrm->u2DramReadWidth%2);    
}

static void _vScpipUpdate3DLIInSGOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DLIInSGOutPrm\n");
    _vScpipUpdate3DTABInSGOutPrm(u4VdpId, picInfo, dispInfo, dispPrm);
}

static void _vScpipUpdate3DFSIn2DOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DFSIn2DOutPrm\n");
    if (u4VdpId >= VDP_NS)
    {
        return;
    }    
    //update dram write H & V
    dispPrm->u2DramWriteWidth = MIN(_arScalePrm[u4VdpId].u4HpsWidth, dispInfo->wWidth);
    if(_bScpipIsScalerInterlaceInput(u4VdpId) == SV_TRUE) // ==> replace this condition by SRM rule.
    {
        dispPrm->u2DramWriteHeight = MIN((picInfo->wSrcH >> 1), (dispInfo->wHeight >> 1));

    }
    else
    {
        dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, dispInfo->wHeight);
    }
    //update dram read H & V        
    dispPrm->u2DramReadWidth = _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
    dispPrm->u2DramReadHeight = picInfo->wTargetH;
}

static void _vScpipUpdate3DSBSIn2DOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    Printf("_vScpipUpdate3DSBSIn2DOutPrm\n");
    if (u4VdpId >= VDP_NS)
    {
        return;
    }    
    //update dram write H & V
    dispPrm->u2DramWriteWidth = MIN(_arScalePrm[u4VdpId].u4HpsWidth, dispInfo->wWidth);
    dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, dispInfo->wHeight);
    //update dram read H & V
    dispPrm->u2DramReadWidth = _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth /2;
    dispPrm->u2DramReadHeight = picInfo->wTargetH;
}

static void _vScpipUpdate3DTABIn2DOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)            
{
    Printf("_vScpipUpdate3DTABIn2DOutPrm\n");
    if (u4VdpId >= VDP_NS)
    {
        return;
    }    
    //update dram write H & V
    dispPrm->u2DramWriteWidth = MIN(_arScalePrm[u4VdpId].u4HpsWidth, dispInfo->wWidth);
    dispPrm->u2DramWriteHeight = MIN(picInfo->wSrcH, dispInfo->wHeight);
    //update dram read H & V
    dispPrm->u2DramReadWidth = _arScalePrm[u4VdpId].u4HpsAndOverscanedWidth;
    dispPrm->u2DramReadHeight = picInfo->wTargetH >> 1;
}

static void _vScpipUpdate3DLIIn2DOutPrm(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)            
{
    _vScpipUpdate3DTABIn2DOutPrm(u4VdpId, picInfo, dispInfo, dispPrm);
}

void vScpipUpdateUs3DFieldFlag(UINT32 u4VdpId)
{
    UINT8 bPath;
    RPicInfo* picInfo;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    bPath = getScalerPath(u4VdpId);
    picInfo = getPicInfo(bPath);
    
    if(_bScpipIsScalerInterlaceInput(u4VdpId) == SV_TRUE)
    {
        _arScalePrm[u4VdpId].u4IsScalerBobMode = SV_TRUE;
        picInfo->wVTotal = picInfo->wVTotal>>1;
        picInfo->wSrcH = picInfo->wSrcH>>1;
        picInfo->wTargetH = picInfo->wTargetH>>1;
        picInfo->wVPorch = picInfo->wVPorch>>1;            
        picInfo->wYOffset = picInfo->wYOffset>>1;
    }
    else
    {
        _arScalePrm[u4VdpId].u4IsScalerBobMode = SV_FALSE;
    }
}

void vScpipUpdate3DPRUpFlag(UINT32 u4VdpId)
{    
    UINT32 u4PRUpFlag;
    UINT8 bPath;
    SCALER_TDTV_OUT_TYPE out;
    RDispInfo* dispInfo;
    RDispPrm* dispPrm;
    SCALER_TDTV_PRM_T* tdtvInfo;

    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    u4PRUpFlag = SV_FALSE;
    bPath = getScalerPath(u4VdpId);
    out = eScpipGetTdtvOutType(u4VdpId);
    dispInfo = getDispInfo(bPath);
    dispPrm = getDispPrm(bPath);
    tdtvInfo = getTdtvInfo(u4VdpId);
    
    if(out == SCALER_TDTV_OUT_TYPE_PR || out == SCALER_TDTV_OUT_TYPE_FPR)
    {
        if(dispPrm->u2DramReadHeight < dispInfo->wHeight)
        {
            u4PRUpFlag = SV_TRUE;
        }
    }

    tdtvInfo->u4IsPRUp = u4PRUpFlag;
}

static void _vScpipUpdate3DScalingFactor(UINT32 u4VdpId, RPicInfo* picInfo, RDispInfo* dispInfo, RDispPrm* dispPrm)
{
    if (u4VdpId >= VDP_NS)
    {
        return;
    }
    //update scaling factor
    dispPrm->u4DSclH = CAL_DS_FACTOR(_arScalePrm[u4VdpId].u4HpsWidth, dispPrm->u2DramWriteWidth);
    dispPrm->u4DSclV = CAL_DS_FACTOR(picInfo->wSrcH, dispPrm->u2DramWriteHeight);    
    dispPrm->u4USclH = CAL_US_FACTOR(dispPrm->u2DramReadWidth, dispInfo->wWidth);
    dispPrm->u4USclV = CAL_US_FACTOR(dispPrm->u2DramReadHeight, dispInfo->wHeight);
}

static UINT8 _bScpipIsScalerInterlaceInput(UINT32 u4VdpId)
{
    UINT8 ret = SV_FALSE;
    UINT8 bPath;
    
    bPath = getScalerPath(u4VdpId);
    
    if((bDrvVideoIsSrcInterlace(bPath) == SV_TRUE) && 
        (((SV_VP_MAIN==u4VdpId)&&((RegReadFldAlign(MCVP_KC_00,I2P_EN)==SV_OFF)||(RegReadFldAlign(MCVP_KC_00,BYPASS_EN)==SV_ON)))
          ||((SV_VP_PIP==u4VdpId)&&(RegReadFldAlign(MCVP_KC_00+0x800,BYPASS_EN)==1)))
        )
    {
        ret = SV_TRUE;
    }
    return ret;
}

