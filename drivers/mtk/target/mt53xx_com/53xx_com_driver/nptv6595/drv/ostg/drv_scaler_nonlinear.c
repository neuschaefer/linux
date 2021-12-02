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
 * $RCSfile: drv_scaler_nonlinear.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_scaler_nonlinear.c
 *  Brief of file drv_scaler_nonlinear.c.
 *  Details of file drv_scaler_nonlinear.c (optional).
 */

//-----------------------------------------------------------------------------
// Include files
//----------------------------------------------------------------------------- 
#include "drv_scaler.h"
#include "drv_scaler_drvif.h"
#include "vdo_misc.h"
#include "general.h"
#include "video_def.h"
#include "scpos_debug.h"
#include "x_assert.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define CC_NONLINEAR_AUTO

//#define DFT_NONLINER_DS_BIT_NUM    (11)
#define DFT_NONLINER_US_BIT_NUM    (11)

 
// New algorithm to auto fine tune non-linear parameters.
#define CC_NONLINEAR_AUTO_FINE_TUNE
#ifdef CC_NONLINEAR_AUTO_FINE_TUNE
UINT32 u4RefWidth;
UINT32 u4RefHeight;
#endif

static VDP_NEW_NONLINEAR_PRM_T _rNonlPrm[VDP_NS];

//-----------------------------------------------------------------------------
// Static function 
//-----------------------------------------------------------------------------

/**
* @brief Integer square root algorithm
* @param u4Integer
* @return Result
*/
static UINT32 _SquareRoot(UINT32 u4Integer) 
{
    UINT32 u4Result;
    UINT32 u4Var1;
    UINT32 u4Var2;
    UINT32 u4Var3;

    u4Result = 0;
    u4Var2 = 0x8000;
    u4Var3 = 15;

    do 
    {
        if (u4Integer >= (u4Var1 = (((u4Result << 1) + u4Var2) << u4Var3--))) 
        {
            u4Result += u4Var2;
            u4Integer -= u4Var1;
        }
    } while ((u4Var2 >>= 1) != 0);

    return u4Result;
}


#ifndef CC_COPLAT_MT82
/**
* @brief Get nonlinear customization table
* @param ucVdpId VDP_1/VDP_2
* @return NONLINEAR_PRM_T nonlinear customization table
*/
static NONLINEAR_PRM_T _rScpipGetNonLinearTable(UCHAR ucVdpId) 
{
    UINT32 u4Idx, u4Size, u4Width;
    NONLINEAR_PRM_T* Nonlinear_Prm_Tbl_1;
    
    Nonlinear_Prm_Tbl_1 = DRVCUST_NonLinearTblGet();
    u4Size = DRVCUST_NonLinearTblSizeGet();
    u4Width = DIVIDE_WITH_CARRIER((wDrvVideoInputWidth(ucVdpId) * _arScalePrm[ucVdpId].u4HpsFactor), DFT_DS_FACTOR);

    if (Nonlinear_Prm_Tbl_1 == NULL)
    {
        NONLINEAR_PRM_T Nonlinear_Tbl;

        x_memset(&Nonlinear_Tbl, 0, sizeof(Nonlinear_Tbl));
        Nonlinear_Tbl.bAuto = TRUE;
        return Nonlinear_Tbl;
    }

    for (u4Idx = 1; u4Idx < u4Size; u4Idx++)
    {
        if ((Nonlinear_Prm_Tbl_1[u4Idx].bDecType == _pChannel[ucVdpId]->bDecType) &&
        (Nonlinear_Prm_Tbl_1[u4Idx].u4PanelW == wDISPLAY_WIDTH) && 
        (Nonlinear_Prm_Tbl_1[u4Idx].u4PanelH == wDISPLAY_HEIGHT) &&
        (Nonlinear_Prm_Tbl_1[u4Idx].u4Width == u4Width) &&
        (Nonlinear_Prm_Tbl_1[u4Idx].u4Height == wDrvVideoInputHeight(ucVdpId)) && 
        (Nonlinear_Prm_Tbl_1[u4Idx].u4Interlace == bDrvVideoIsSrcInterlace(ucVdpId)))
        {
            return Nonlinear_Prm_Tbl_1[u4Idx];
        }
    }
    return Nonlinear_Prm_Tbl_1[0];
}
#endif //CC_COPLAT_MT82



//-----------------------------------------------------------------------------
// Inter-driver function 
//-----------------------------------------------------------------------------

/**
* @brief Calculate nonlinear parameters
* @param u4VdpId VDP_1/VDP_2
* @param u4SrcWidth input width
* @param u4OutWidth target width
*/
void vScpipCalNonLinear(UINT32 u4VdpId, UINT32 u4SrcWidth, UINT32 u4OutWidth)
{
    UINT32 u4NotSupported;
    UINT32 u4FactorA;
    UINT32 u4FactorB;
    UINT32 u4Input; 
    UINT32 u4Start; 
    UINT32 u4Var1;
    UINT32 u4Var2;

    UINT32 u4Width;
    UINT32 u4TargetW;

    u4NotSupported = 0;

    if ((_pDispPrm[u4VdpId]->u4DSclV != DFT_DS_FACTOR) ||
        (_pDispPrm[u4VdpId]->u4DSclH != DFT_DS_FACTOR))
    {
        u4NotSupported = 1;
    }

#if 1       // TODO, Nonlinear Full Test
    if ((_pPicInfo[u4VdpId]->wTargetW != 0) &&
        (_pPicInfo[u4VdpId]->wTargetH != 0))
    {
#if 0
        // Block square souce (Aspact Ratio < 1.20)
        u4FactorA = (120 * 0x10000) / 100;
        u4FactorB = (_pPicInfo[u4VdpId]->wTargetW * 0x10000) / _pPicInfo[u4VdpId]->wTargetH;

        if (u4FactorA > u4FactorB)
        {
            // Work Around 
            // too square, skip
            u4NotSupported = 5;
        }
#endif
    }
    else
    {
        u4NotSupported = 6;
    }
#endif

    //// TODO, consider aspect ratio
    u4Width = u4OutWidth;

    // consider H-prescale down
    u4TargetW = (u4SrcWidth * u4ScpipGetHorizontalPreScaleDownFactor(u4VdpId)) / DFT_DS_FACTOR;
    _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope = _arScalePrm[u4VdpId].rAutoNonlinearPrm.u4Slope;

    if (_arScalePrm[u4VdpId].rAutoNonlinearPrm.u1Mode == VDO_NONLINEAR_AUTO_1)
    {
      if (bDrvVideoGetWidthOverSample(u4VdpId))
      {
         //printf("\n***nonlinear OverSample:on ***\n");
         _arScalePrm[u4VdpId].rNonlinearPrm.u4End =  _arScalePrm[u4VdpId].rAutoNonlinearPrm.u4EndFactor*2;
      }
      else
      {
        //printf("\n***nonlinear OverSample:off ***\n");
        _arScalePrm[u4VdpId].rNonlinearPrm.u4End =  _arScalePrm[u4VdpId].rAutoNonlinearPrm.u4EndFactor;
      }
    }
    else if(_arScalePrm[u4VdpId].rAutoNonlinearPrm.u1Mode == VDO_NONLINEAR_AUTO)
    {
      _arScalePrm[u4VdpId].rNonlinearPrm.u4End = (_pDispPrm[u4VdpId]->u4USclV >> 4) * _arScalePrm[u4VdpId].rAutoNonlinearPrm.u4EndFactor/UINT_NONLINEAR_END_FACTOR;                
    }

    _arScalePrm[u4VdpId].rNonlinearPrm.u4MiddlePoint = (u4Width >> 3);
    
#ifdef CC_NONLINEAR_AUTO
    // auto mode, find min-slope in case1
    if (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope == VDP_NONLINEAR_AUTO)
    {
        UINT32 u4AvgScaleFactor;
        UINT32 u4Temp;                    

        // Average Scaling Factor
        u4AvgScaleFactor = (u4TargetW << DFT_NONLINER_US_BIT_NUM) / u4Width;

        // S = 2A - E
        u4Start = (2 * u4AvgScaleFactor) - _arScalePrm[u4VdpId].rNonlinearPrm.u4End;

        // 2 * (E - S) > O ?
        u4Temp = 2 * (_arScalePrm[u4VdpId].rNonlinearPrm.u4End - u4Start);

        _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope = UNIT_NONLINEAR;
        if (u4Temp > u4Width)
        {
            while (u4Temp > u4Width)
            {
                u4Temp >>= 1;
                _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope++;
            }
        }
        else
        {
            while (u4Width < u4Temp)
            {
                u4Temp <<= 1;
                _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope--;
            }

            _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope++;
        }

        LOG(3, "Auto Scope(%d)\n", _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope);
    }
#endif

    // YH Shih's Suggestion
    if (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope > 14)
    {
        _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope = 14;
    }

    if (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope > UNIT_NONLINEAR)
    {
        u4FactorA = 1 << (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope - UNIT_NONLINEAR);
        u4FactorB = 1;
    }
    else
    {
        u4FactorA = 1;
        u4FactorB = 1 << (UNIT_NONLINEAR - _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope);
    }

    // Hardware Error Term
    if (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope >= 12)
    {
        u4FactorA <<= 8;
        u4FactorB <<= 8;

        if (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope == 12)
        {
            u4FactorA += 6;
        }
        else
        if (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope == 13)
        {
            u4FactorA += 25;
        }
        else
        if (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope == 14)
        {
            u4FactorA += 76;
        }
    }

    // scaling factor increase level in half output width
    u4Var1 = u4Width >> 1;
    u4Var1 *= u4FactorA;
    u4Var1 /= u4FactorB;

    // Min start value
    if (_arScalePrm[u4VdpId].rNonlinearPrm.u4End > u4Var1)
    {
        u4Start = _arScalePrm[u4VdpId].rNonlinearPrm.u4End - u4Var1;
    }
    else
    {
        u4Start = 0;
    }

    // average scaling factor
    u4Var2 = (u4Start + _arScalePrm[u4VdpId].rNonlinearPrm.u4End) >> 1;

    // Max input width
    u4Input = (u4Width * u4Var2) >> DFT_NONLINER_US_BIT_NUM;

    // Current input width > Max input width
    if (u4TargetW < u4Input)
    {
        // case 2
        
        u4Var2 = (u4TargetW << DFT_NONLINER_US_BIT_NUM) / u4Width;

        if (u4Var2 <= ((u4Var1>>1)+100))
        {               
            u4NotSupported = 6;
        }
        u4Var2 -= (u4Var1 >> 1);

        _arScalePrm[u4VdpId].rNonlinearPrm.u4Start = u4Var2;
        if(_arScalePrm[u4VdpId].rNonlinearPrm.u4Start>0x8000)
        {
            u4NotSupported = 7;
        }

        LOG(3, "[Case 2] Nonlinear(%d) (%d, %d, %d, %d)\n", 
            u4VdpId,  
            _arScalePrm[u4VdpId].rNonlinearPrm.u4Start,
            _arScalePrm[u4VdpId].rNonlinearPrm.u4End,
            _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope,
            _arScalePrm[u4VdpId].rNonlinearPrm.u4MiddlePoint);
    }
    else
    {
        // case 1

        if ((u4TargetW << DFT_NONLINER_US_BIT_NUM) > (u4Width * _arScalePrm[u4VdpId].rNonlinearPrm.u4End))
        {
            u4NotSupported = 3;
        }
        
        u4Var2 = (u4Width * _arScalePrm[u4VdpId].rNonlinearPrm.u4End) - (u4TargetW << DFT_NONLINER_US_BIT_NUM);

        u4Var2 *= u4FactorA;
        u4Var2 /= u4FactorB;

        u4Var2 = _SquareRoot(u4Var2);

        if (u4Var2 > _arScalePrm[u4VdpId].rNonlinearPrm.u4End)
        {
            u4NotSupported = 4;
        }

        _arScalePrm[u4VdpId].rNonlinearPrm.u4Start = _arScalePrm[u4VdpId].rNonlinearPrm.u4End - u4Var2;

        // handle unknown rounding
        _arScalePrm[u4VdpId].rNonlinearPrm.u4Start -= (_arScalePrm[u4VdpId].rNonlinearPrm.u4End - _arScalePrm[u4VdpId].rNonlinearPrm.u4Start) >> 7;
        if(_arScalePrm[u4VdpId].rNonlinearPrm.u4Start>0x8000)
        {
            u4NotSupported = 7;
        }
        LOG(3, "[Case 1] Nonlinear(%d) (%d, %d, %d, %d)\n", 
            u4VdpId,  
            _arScalePrm[u4VdpId].rNonlinearPrm.u4Start,
            _arScalePrm[u4VdpId].rNonlinearPrm.u4End,
            _arScalePrm[u4VdpId].rNonlinearPrm.u4Slope,
            _arScalePrm[u4VdpId].rNonlinearPrm.u4MiddlePoint);
    }

    if (u4NotSupported ==0)
    {
        _arScalePrm[u4VdpId].u4NonlinearCtrl = 1;
    }
    else
    {
        LOG(3, "Nonlinear NOT Supported! (%d)\n", u4NotSupported);
    }
}

//-----------------------------------------------------------------------------
// Global function 
//-----------------------------------------------------------------------------

/**
* @brief Enable/Disable nonlinear up scaler
* @param ucVdpId VDP_1/VDP_2
* @param ucOnOff On/Off
*/
void vScpipSetNonLinearUpScaler(UCHAR ucVdpId, UCHAR ucOnOff) 
{
    if (ucVdpId >= VDP_NS)
    {
        return;
    }
    
    if (_arScalePrm[ucVdpId].u4NonlinearEn != ucOnOff) 
    {
        // 121-map & nonlinear scaling can NOT enable together
        if (ucOnOff)
        {
            _arScalePrm[ucVdpId].u4121Map = 0;
        }

        #ifndef CC_COPLAT_MT82
        NONLINEAR_PRM_T Nonlinear_Tbl;
        Nonlinear_Tbl = _rScpipGetNonLinearTable(ucVdpId);

        if (Nonlinear_Tbl.bAuto == FALSE) // manual mode
        {
            _arScalePrm[ucVdpId].rNonlinearPrm.u4Start = Nonlinear_Tbl.u4Start >> 4;
            _arScalePrm[ucVdpId].rNonlinearPrm.u4Slope = Nonlinear_Tbl.u4Slope;
            _arScalePrm[ucVdpId].rNonlinearPrm.u4End = Nonlinear_Tbl.u4Stop << 4;
            _arScalePrm[ucVdpId].rNonlinearPrm.u4MiddlePoint = Nonlinear_Tbl.u4Middle;
            _arScalePrm[ucVdpId].u4NonlinearAuto = FALSE;
        }
        else // auto mode
        #endif //CC_COPLAT_MT82
        {
            _arScalePrm[ucVdpId].rAutoNonlinearPrm.u4EndFactor = DRVCUST_OptGet(eFlagScposNonLinearEndPointRatio);
            _arScalePrm[ucVdpId].rAutoNonlinearPrm.u4Slope= DRVCUST_OptGet(eFlagScposNonLinearSlope);
            _arScalePrm[ucVdpId].u4NonlinearAuto = TRUE;
            _arScalePrm[ucVdpId].rAutoNonlinearPrm.u1Mode = VDO_NONLINEAR_AUTO;
        }
        
        _arScalePrm[ucVdpId].u4NonlinearEn = ucOnOff;
        _arScalePrm[ucVdpId].u4NewNonlinear= FALSE;
        
        if (ucVdpId == VDP_1)
        {
            vSetScposFlg(MAIN_DISP_PRM_CHG);
        }
        else
        {
            vSetScposFlg(PIP_DISP_PRM_CHG);
        }
    }
}


/**
* @brief Set new nonlinear
* @param u4VdpId VDP_1/VDP_2
* @param rNonlPrm nonlinear paramters
*/
void vScpipSetNewNonLinear(const UINT32 u4VdpId, VDP_NEW_NONLINEAR_PRM_T rNonlPrm) 
{
    if (u4VdpId >= VDP_2)
    {
        return;
    }

    LOG(3, "Set New NonLinear (%d) (%d, %d)\n", 
        u4VdpId,
        rNonlPrm.u4SrcMiddleRatio, 
        rNonlPrm.u4OutMiddleRatio);

    if(rNonlPrm.u4SrcMiddleRatio > 10000)
    {
        rNonlPrm.u4SrcMiddleRatio = 10000;
    }

    if(rNonlPrm.u4OutMiddleRatio > 10000)
    {
        rNonlPrm.u4OutMiddleRatio = 10000;
    }

    _rNonlPrm[u4VdpId] = rNonlPrm;
    _arScalePrm[u4VdpId].u4NonlinearAuto= FALSE;
    _arScalePrm[u4VdpId].u4NewNonlinear= TRUE;
    
    if(u4VdpId == VDP_1)
    {
        vSetScposFlg(MAIN_DISP_PRM_CHG);    
    }
}

/**
* @brief Get new nonlinear
* @param u4VdpId VDP_1/VDP_2
* @param prNonlPrm nonlinear paramters
*/
void vScpipGetNewNonLinear(const UINT32 u4VdpId, VDP_NEW_NONLINEAR_PRM_T *prNonlPrm) 
{
    if (u4VdpId >= VDP_2)
    {
        return;
    }

    prNonlPrm->u4SrcMiddleRatio = _rNonlPrm[u4VdpId].u4SrcMiddleRatio;
    prNonlPrm->u4OutMiddleRatio = _rNonlPrm[u4VdpId].u4OutMiddleRatio;
}


void vScpipCalNewNonlinear(const UINT32 u4VdpId, const UINT32 u4SrcWidth, const UINT32 u4OutWidth)
{
    UINT32 u4NonlinearCtrl = 1;    
    UINT32 u4SrcMiddle = u4SrcWidth * _rNonlPrm[u4VdpId].u4SrcMiddleRatio / 10000;    
    UINT32 u4OutMiddle = u4OutWidth * _rNonlPrm[u4VdpId].u4OutMiddleRatio / 10000;    
    UINT32 u4SrcSideLen = (u4SrcWidth - u4SrcMiddle) / 2;
    UINT32 u4OutSideLen = (u4OutWidth - u4OutMiddle) / 2;
    UINT32 u4StopFactor = DFT_US_FACTOR;
    UINT32 u4StartFactor = DFT_US_FACTOR;
    UINT32 u4StepFactor = 0;
    UINT32 u4StopPoint = 0;
    
    if((_rNonlPrm[u4VdpId].u4SrcMiddleRatio) != 0 && (_rNonlPrm[u4VdpId].u4OutMiddleRatio != 0))
    {

        if(u4SrcMiddle > u4OutMiddle) // down scale
        {
            u4NonlinearCtrl = 0;
        }

        u4StopFactor = (u4SrcMiddle * DFT_US_FACTOR) / u4OutMiddle;

        LOG(3, "vScpipSetNewNonLinear: (%d, %d) --> (%d, %d)\n", u4SrcMiddle, u4SrcWidth, u4OutMiddle, u4OutWidth);
        LOG(4, "vScpipSetNewNonLinear: u4SrcSideLen=%d, u4OutSideLen=%d\n", u4SrcSideLen, u4OutSideLen);
        
        if((u4SrcSideLen >= u4OutSideLen) || // side down scale
            (((u4SrcSideLen * DFT_US_FACTOR) / u4OutSideLen) >= u4StopFactor)) // side up scaling ratio is less than middle
        {
            u4NonlinearCtrl = 0;
        }
        else
        {
            int i = 0;
            
            u4StopPoint = u4OutSideLen;

            // scale factor variance for each step, remove fractional part for register setting
            u4StepFactor = (u4OutWidth*u4StopFactor - DFT_US_FACTOR*u4SrcWidth)/(u4OutSideLen*u4OutSideLen);
            
            do
            {
                i++;
                    
                // re-calculate new stop factor for "integer" scale factor variance
                u4StopFactor = (u4StepFactor*(u4OutSideLen*u4OutSideLen) + DFT_US_FACTOR*u4SrcWidth)/u4OutWidth;
                // calculate correct start factor by new stop factor & scale factor variance for each step
                u4StartFactor = u4StopFactor-(u4StepFactor*u4OutSideLen);

                // underflow
                // try to adjust start factor to positive value by decrease step factor
                if((u4StartFactor > DFT_US_FACTOR) || (u4StartFactor == 0)) 
                {
                    u4StepFactor = (u4StopFactor - 1) / u4OutSideLen;                    
                }
            }while((i <= 10) && ((u4StartFactor > DFT_US_FACTOR) || (u4StartFactor == 0)));

            if((u4StartFactor > DFT_US_FACTOR) || (u4StartFactor == 0))
            {
                u4NonlinearCtrl = 0; 
            }
        }    
    }
    else // TODO: Add customized stop factor to the flow
    {
        UINT32 u4Temp;

        u4Temp = (u4SrcWidth * 2 * DFT_US_FACTOR) / u4OutWidth;

        if(u4Temp < DFT_US_FACTOR)
        {
            u4StopFactor = u4Temp - (DFT_US_FACTOR/10);
        }
        else
        {
            u4StopFactor = DFT_US_FACTOR;            
        }
        
        u4StartFactor = u4Temp - u4StopFactor;

        if(u4StartFactor >= u4StopFactor) // not supported
        {            
            u4NonlinearCtrl = 0; 
        }
        else
        {
            INT32 i = 0;
            UINT32 u4LastStartFactor = 0xffffffff;            
            
            u4StepFactor = (2 * (u4StopFactor - u4StartFactor)) / u4OutWidth;
            u4StopPoint = u4OutWidth >> 1;

            // full screen finetune
            do{                
                i++;
                u4StopFactor = _SquareRoot(DFT_US_FACTOR * u4StepFactor * u4SrcWidth + (u4StartFactor*u4StartFactor));
                u4StartFactor = u4Temp - u4StopFactor;
                
                if(u4LastStartFactor == u4StartFactor)
                {
                    // if the start factor value is stable, break the loop to save some time
                    break;
                }
                else
                {
                    u4LastStartFactor = u4StartFactor;
                }
            }while(i <= 10);
        }
    }

    _arScalePrm[u4VdpId].u4NonlinearCtrl = u4NonlinearCtrl;

    if(u4NonlinearCtrl == 0)
    {
        LOG(3, "Non-linear scaling not supported case !!!\n");
        _arScalePrm[u4VdpId].rNewNonlinearPrm.u4StartFactor= 0;
        _arScalePrm[u4VdpId].rNewNonlinearPrm.u4StopPoint= 0;
        _arScalePrm[u4VdpId].rNewNonlinearPrm.u4StepFactor= 0;
        _arScalePrm[u4VdpId].rNewNonlinearPrm.u4MiddlePoint = 0;
        _arScalePrm[u4VdpId].rNewNonlinearPrm.u1Step = 1;        
    }
    else
    {
        LOG(3, "New nonlinear scaling (S=0x%x, E=0x%x, a=0x%x, stop_p=%d)\n",
            u4StartFactor,
            u4StopFactor,
            u4StepFactor,
            u4StopPoint);        
        _arScalePrm[u4VdpId].rNewNonlinearPrm.u4StartFactor= u4StartFactor;
        _arScalePrm[u4VdpId].rNewNonlinearPrm.u4StopPoint= u4StopPoint;
        _arScalePrm[u4VdpId].rNewNonlinearPrm.u4StepFactor= u4StepFactor;
        _arScalePrm[u4VdpId].rNewNonlinearPrm.u4MiddlePoint = u4OutWidth >> 1;
        _arScalePrm[u4VdpId].rNewNonlinearPrm.u1Step = 1;
    }    
}

/**
* @brief Set auto nonlinear
* @param u4VdpId VDP_1/VDP_2
* @param rAutoNonlinearPrm auto nonlinear paramters
*/
void vScpipSetAutoNonLinear(UINT32 u4VdpId, VDP_AUTO_NONLINEAR_PRM_T rAutoNonlinearPrm) 
{
    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    LOG(3, "Set Auto NonLinear (%d) (%d, %d, %d)\n", 
        u4VdpId,
        rAutoNonlinearPrm.u4Adaptive, 
        rAutoNonlinearPrm.u4EndFactor, 
        rAutoNonlinearPrm.u4Slope);

    if ((_arScalePrm[u4VdpId].rAutoNonlinearPrm.u4Slope != rAutoNonlinearPrm.u4Slope) ||
        (_arScalePrm[u4VdpId].rAutoNonlinearPrm.u4EndFactor != rAutoNonlinearPrm.u4EndFactor) ||
        (_arScalePrm[u4VdpId].u4NonlinearAuto != TRUE))     
    {
        #ifdef CC_NONLINEAR_AUTO_FINE_TUNE
        UINT32 u4Width, u4Height;
        UINT32 u4Stop;
        #endif
        _arScalePrm[u4VdpId].rAutoNonlinearPrm = rAutoNonlinearPrm;
        _arScalePrm[u4VdpId].u4NonlinearAuto= TRUE;
        

        #ifdef CC_NONLINEAR_AUTO_FINE_TUNE 
        u4Width = wDrvVideoInputWidth(u4VdpId);
        u4Height = wDrvVideoInputHeight(u4VdpId);
        u4Stop = _arScalePrm[u4VdpId].rAutoNonlinearPrm.u4EndFactor;

        switch (u4Height)
        {
            case 480:
                u4RefWidth = 720;
                u4RefHeight = 480;
                break;
            case 576:
                u4RefWidth = 720;
                u4RefHeight = 576;
                break;
            case 720:
                u4RefWidth = 1280;
                u4RefHeight = 720;
                break;
            case 1080:
                u4RefWidth = 1920;
                u4RefHeight = 1080;
                break;
            default:
                u4RefWidth = 720;
                u4RefHeight = 480;
                break;
        }
        
        if (u4Width && u4Height)
        {
            _arScalePrm[u4VdpId].rAutoNonlinearPrm.u4EndFactor = (u4Stop * u4RefHeight * u4Width) / (u4RefWidth * u4Height);
        }
        #endif

        if (u4VdpId == VDP_1)
        {
            vSetScposFlg(MAIN_DISP_PRM_CHG);
        }
        else
        {
            vSetScposFlg(PIP_DISP_PRM_CHG);
        }
    }

    _arScalePrm[u4VdpId].u4NonlinearAuto= TRUE;
}

/**
* @brief Set manual nonlinear
* @param u4VdpId VDP_1/VDP_2
* @param rNonlinearPrm manual nonlinear paramters
*/
void vScpipSetManualNonLinear(UINT32 u4VdpId, VDP_NONLINEAR_PRM_T rNonlinearPrm)
{
        if (u4VdpId >= VDP_NS)
        {
            return;
        }

        LOG(3, "Set Manual NonLinear(%d) (%d, %d, %d, %d)\n", 
                    u4VdpId,  
                    rNonlinearPrm.u4Start,
                    rNonlinearPrm.u4End,
                    rNonlinearPrm.u4Slope,
                    rNonlinearPrm.u4MiddlePoint);

        if ((_arScalePrm[u4VdpId].rNonlinearPrm.u4End != rNonlinearPrm.u4End) ||
            (_arScalePrm[u4VdpId].rNonlinearPrm.u4MiddlePoint != rNonlinearPrm.u4MiddlePoint) ||
            (_arScalePrm[u4VdpId].rNonlinearPrm.u4Slope != rNonlinearPrm.u4Slope) ||
            (_arScalePrm[u4VdpId].rNonlinearPrm.u4Start!= rNonlinearPrm.u4Start) ||
            (_arScalePrm[u4VdpId].u4NonlinearAuto))
        {
            _arScalePrm[u4VdpId].u4NonlinearAuto= FALSE;
            _arScalePrm[u4VdpId].rNonlinearPrm = rNonlinearPrm;

            if (u4VdpId == VDP_1)
            {
                vSetScposFlg(MAIN_DISP_PRM_CHG);
            }
            else
            {
                vSetScposFlg(PIP_DISP_PRM_CHG);
            }
        }

        _arScalePrm[u4VdpId].u4NonlinearAuto= FALSE;
}

