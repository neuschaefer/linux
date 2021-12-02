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
#ifdef CC_SCPOS_3DTV_SUPPORT

#include "x_assert.h"
#include "panel.h"
#include "drv_scaler.h"
#include "drv_scaler_drvif.h"
#include "vdo_misc.h"
#include "drv_tdtv_drvif.h"
#include "scpos_debug.h"

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
    DEF_TO_STR(SCPIP_TV3D_REALD            ), // 0x8 
    DEF_TO_STR(SCPIP_TV3D_TNB_SW_WA        ), // 0x9  
    DEF_TO_STR(SCPIP_TV3D_3D_TO_2D_DROP_R  ), // 0xa  
    DEF_TO_STR(SCPIP_TV3D_3D_TO_2D_SBS     ), // 0xb  
    DEF_TO_STR(SCPIP_TV3D_3D_TO_2D_SBS_Q   ), // 0xc  
    DEF_TO_STR(SCPIP_TV3D_3D_TO_2D_SBS_Q_I ), // 0xd  
    DEF_TO_STR(SCPIP_TV3D_3D_TO_2D_TNB     ), // 0xe  
    DEF_TO_STR(SCPIP_TV3D_3D_TO_2D_FS      ), // 0xf  
    DEF_TO_STR(SCPIP_TV3D_3D_REPEAT        ), // 0x10 
    DEF_TO_STR(SCPIP_TV3D_3D_REPEAT_DS_HALF), // 0x11 
    DEF_TO_STR(SCPIP_TV3D_FHD_PR           ), // 0x12 
    DEF_TO_STR(SCPIP_TV3D_FHD_PR_USE_V2    ), // 0x13       
    DEF_TO_STR(SCPIP_TV3D_FS_INPUT         ), // 0x14
    DEF_TO_STR(SCPIP_TV3D_FP_I_HDS_HALF    )  // 0x15 
    DEF_TO_STR(SCPIP_TV3D_INTERLEAVE_SPLIT ), // 0x16 
    DEF_TO_STR(SCPIP_TV3D_3D_TO_2D_INTERLEAVE)  // 0x17
#if (defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR))
                                            ,
    DEF_TO_STR(SCPIP_TV3D_SBSH_PR          ), //0x18  
    DEF_TO_STR(SCPIP_TV3D_TNB_PR           ), //0x19           
    DEF_TO_STR(SCPIP_TV3D_FP_PR            ), //0x1a
    DEF_TO_STR(SCPIP_TV3D_TTD_PR           )  //0x1b
#endif             
};


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static UINT32 _u4ScpipTv3dFlg[VDP_NS] = {0, 0};
static E_TDTV_DECODER_INPUT _rScpipTv3dInType[VDP_NS] = {E_TDTV_DECODER_INPUT_END, E_TDTV_DECODER_INPUT_END};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global functions
//-----------------------------------------------------------------------------

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

#if 0
void vScpipTv3dSetFullPrEn(BOOL fgOnOff)
{
    _fgScpipTv3dFullPrEn = fgOnOff;
    
    vSetScposFlg(MAIN_DISP_PRM_CHG);
}
#endif

#if defined(CC_MT5365) && defined(MT5366_SUPPORT_3D_PR)
void vScpipTv3dModeSwitch(void *pvArgv)
{
    UINT32 u4Mode = 0;    
    UINT32 u4TDTVQueryResult;
    E_TDTV_DECODER_INPUT eInType;
    E_TDTV_DECODER_OUTPUT eOutType;
    
    UINT32 u4VdpId = VDP_1;

    u4TDTVQueryResult = u4DrvTDTVScalerModeQuery(E_TDTV_DECODER_INPUT_AUTO);
    eInType = (E_TDTV_DECODER_INPUT)(u4TDTVQueryResult & 0xff);
    eOutType = (E_TDTV_DECODER_OUTPUT)((u4TDTVQueryResult>>16) & 0xff);

    _rScpipTv3dInType[u4VdpId] = (E_TDTV_DECODER_INPUT)eInType;
    
    if((eInType == E_TDTV_DECODER_INPUT_2D_P) && (eOutType == E_TDTV_DECODER_OUTPUT_3D_60_PR))
    {
        u4Mode |= SCPIP_TV3D_TTD_PR;    /* 2D to 3D */
    }    
    else if(eInType != E_TDTV_DECODER_INPUT_2D_P)
    {           
        u4Mode |= SCPIP_TV3D_INPUT_IS_3D;
        if (eOutType == E_TDTV_DECODER_OUTPUT_3D_60_PR)
        {
            if(eInType == E_TDTV_DECODER_INPUT_SBS_P)
            {
                u4Mode |= SCPIP_TV3D_SBSH_PR;
            }
            else if(eInType == E_TDTV_DECODER_INPUT_TB_P)
            {
                u4Mode |= SCPIP_TV3D_TNB_PR;
            }
            else if(eInType == E_TDTV_DECODER_INPUT_FP_P)
            {
                u4Mode |= SCPIP_TV3D_FP_PR;
            }
            else if(eInType == E_TDTV_DECODER_INPUT_RD_P)
            {
                u4Mode |= SCPIP_TV3D_SBSH_PR;
                u4Mode |= SCPIP_TV3D_REALD;
            }
        }
    }
    else
    {
        // 2d mode
        u4Mode = 0;
    }    
    vScpipTv3dClrFlg(u4VdpId, SCPIP_TV3D_ALL);
    vScpipTv3dSetFlg(u4VdpId, u4Mode);
    
    vScpip3DPRWA();     
}

#else
void vScpipTv3dModeSwitch(void *pvArgv)
{
    UINT32 u4Mode = 0;    
    UINT32 u4TDTVQueryResult;
    E_TDTV_DECODER_INPUT eInType;
    E_TDTV_DECODER_OUTPUT eOutType;    
    UINT32 u4VdpId = VDP_1;
    
    u4TDTVQueryResult = u4DrvTDTVScalerModeQuery(E_TDTV_DECODER_INPUT_AUTO);
    eInType = (E_TDTV_DECODER_INPUT)(u4TDTVQueryResult & 0xff);
    eOutType = (E_TDTV_DECODER_OUTPUT)((u4TDTVQueryResult>>16) & 0xff);

    _rScpipTv3dInType[u4VdpId] = (E_TDTV_DECODER_INPUT)eInType;
    
    if((eInType == E_TDTV_DECODER_INPUT_2D_P)&&(eOutType == E_TDTV_DECODER_OUTPUT_3D_REPEAT))
    {
        if(IS_LVDS_DISP_3D_POLARIZED)
        {
            u4Mode |= SCPIP_TV3D_3D_REPEAT_DS_HALF;
        }
        else
        {
            u4Mode |= SCPIP_TV3D_3D_REPEAT;
        }
    }
    else if(eInType != E_TDTV_DECODER_INPUT_2D_P)
    {           
        u4Mode |= SCPIP_TV3D_INPUT_IS_3D;
        if(BSP_GetIcVersion() == IC_VER_5395_AA)
        {
            Printf("****************************************\n");
            Printf("3DTV feature is not supported by this IC\n");            
            Printf("****************************************\n");
            ASSERT(0);
            if(IS_LVDS_DISP_3D_POLARIZED)
            {
                if(eInType == E_TDTV_DECODER_INPUT_FS_P)
                {
                    u4Mode |= SCPIP_TV3D_FRAME_INTERLEAVE;
                }
                else if(eInType == E_TDTV_DECODER_INPUT_SBS_P)
                {
                    u4Mode |= SCPIP_TV3D_LINE_INTERLEAVE;
                }
            }
            else //if(IS_LVDS_DISP_3D_SHUTTER) 
            {
                u4Mode |= SCPIP_TV3D_SHUTTER_OUT;
            }
        }
        else if (eOutType == E_TDTV_DECODER_OUTPUT_3D_FPR_V1)
        {
            u4Mode |= SCPIP_TV3D_FHD_PR;
            
        }
        else if (eOutType == E_TDTV_DECODER_OUTPUT_3D_FPR_V2)
        {
            u4Mode |= SCPIP_TV3D_FHD_PR;
            u4Mode |= SCPIP_TV3D_FHD_PR_USE_V2;
        }
        else if (eOutType == E_TDTV_DECODER_OUTPUT_3D_2_2D)
        {
            if (eInType == E_TDTV_DECODER_INPUT_TB_P)
            {
                u4Mode |= SCPIP_TV3D_3D_TO_2D_TNB;
            }
            else if(eInType == E_TDTV_DECODER_INPUT_SBS_P)
            {
                u4Mode |= SCPIP_TV3D_3D_TO_2D_SBS;
            }
            else if (eInType == E_TDTV_DECODER_INPUT_SBS_Q_P)
            {
                u4Mode |= SCPIP_TV3D_3D_TO_2D_SBS_Q;
            }
            else if (eInType == E_TDTV_DECODER_INPUT_SBS_Q_I)
            {
                u4Mode |= SCPIP_TV3D_3D_TO_2D_SBS_Q_I;
            }
            else if (eInType == E_TDTV_DECODER_INPUT_FS_P)
            {
                u4Mode |= SCPIP_TV3D_3D_TO_2D_FS;
            }
        }
        else if (eOutType == E_TDTV_DECODER_OUTPUT_3D_PREDOWN)
        {
            u4Mode |= SCPIP_TV3D_FP_I_HDS_HALF;
            u4Mode |= SCPIP_TV3D_SHUTTER_OUT;
        }
        else
        {   
            // interleave is done in MJC
            // all set to shutter out mode
            u4Mode |= SCPIP_TV3D_SHUTTER_OUT;
            
            if(eInType == E_TDTV_DECODER_INPUT_SBS_P)
            {
                u4Mode |= SCPIP_TV3D_SBSH_SPLIT;
            }
            else if (eInType == E_TDTV_DECODER_INPUT_TB_P)
            {
                u4Mode |= SCPIP_TV3D_TNB_SPLIT;
            }
            else if (eInType == E_TDTV_DECODER_INPUT_TB_I)
        	{
                u4Mode |= SCPIP_TV3D_TNB_SW_WA;
        	}
            else if (eInType == E_TDTV_DECODER_INPUT_SBS_Q_P)
            {
                u4Mode |= SCPIP_TV3D_SBS_Q_SPLIT;
            }
            else if (eInType == E_TDTV_DECODER_INPUT_SBS_Q_I)
            {
                u4Mode |= SCPIP_TV3D_SBS_Q_I_SPLIT;
            }                
            else if (eInType == E_TDTV_DECODER_INPUT_FS_P)
            {
                u4Mode |= SCPIP_TV3D_FS_INPUT;
            }
            else if (eInType == E_TDTV_DECODER_INPUT_RD_P)
            {
                u4Mode |= SCPIP_TV3D_SBSH_SPLIT;
                u4Mode |= SCPIP_TV3D_REALD;
            }
            else if (eInType == E_TDTV_DECODER_INPUT_LI_P)
            {
                u4Mode |= SCPIP_TV3D_INTERLEAVE_SPLIT;
            }              
        }
    }
    else
    {
        // 2d mode
        u4Mode = 0;
    }    
    vScpipTv3dClrFlg(u4VdpId, SCPIP_TV3D_ALL);
    vScpipTv3dSetFlg(u4VdpId, u4Mode);

    LOG(1, "vScpipTv3dModeSwitch: u4Mode=0x%x\n", u4Mode);
}
#endif

UINT32 u4ScpipGet3DOneEyeFrameRate(void)
{
    UINT32 u4TDTVQueryResult;
    E_TDTV_DECODER_INPUT eInType;
    
    u4TDTVQueryResult = u4DrvTDTVScalerModeQuery(E_TDTV_DECODER_INPUT_AUTO);
    eInType = (E_TDTV_DECODER_INPUT)(u4TDTVQueryResult & 0xff);
    
    if(eInType != E_TDTV_DECODER_INPUT_2D_P)
    {
        if(eInType == E_TDTV_DECODER_INPUT_FS_P)
        {
            return bDrvVideoGetRefreshRate(SV_VP_MAIN) >> 1;
        }
    }
    return bDrvVideoGetRefreshRate(SV_VP_MAIN);
            
}

VOID vScpipSet3DLRShift(UINT32 u4VdpId, UINT8 u1LR, INT8 i1Shift)
{
    LOG(3, "Set 3D LR shift: %d\n", i1Shift);
    if((i1Shift & 1) != 0) // i1Shift is odd number
    {
        //make it even number
        if(i1Shift > 0)
        {
            i1Shift -= 1;
        }
        else
        {
            i1Shift += 1;
        }        
        LOG(3, "Shift value is odd, set 3D LR shift: %d\n", i1Shift);        
    }
    
    if(u1LR == 1)
    {
        _pDispPrm[u4VdpId]->i1RShift = i1Shift;
    }
    else
    {
        _pDispPrm[u4VdpId]->i1LShift = i1Shift;        
    }

    vSetScposFlg(MAIN_DISP_PRM_CHG);
}

   
INT8 i1ScpipGet3DLRShift(UINT32 u4VdpId, UINT8 u1LR)
{
    if(u1LR == 1)
    {
        return _pDispPrm[u4VdpId]->i1RShift;
    }
    else
    {
        return _pDispPrm[u4VdpId]->i1LShift;        
    }
}

#endif
