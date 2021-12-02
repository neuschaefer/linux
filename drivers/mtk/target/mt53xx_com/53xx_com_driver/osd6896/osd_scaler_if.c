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
 * $Author: pan.huang $
 * $Date: 2012/12/12 $
 * $RCSfile: osd_scaler_if.c,v $
 * $Revision: #5 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_scaler_if.c
 *  This header file includes public function definitions of OSD scaler.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_hw.h"
#include "osd_drvif.h"
#include "osd_debug.h"
#include "drvcust_if.h"

LINT_EXT_HEADER_BEGIN
#include "x_ckgen.h"
//#include "hw_scpos.h"
//#include "scpos_reg.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct _OSD_SCALER_INFO
{
    UINT32 u4Enable;
    UINT32 u4SrcWidth;
    UINT32 u4SrcHeight;
    UINT32 u4DstWidth;
    UINT32 u4DstHeight;
} OSD_SCALER_INFO;


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
#ifdef CC_MT5396
static OSD_SCALER_INFO _arScalerInfo[3];
#else
static OSD_SCALER_INFO _arScalerInfo[2];
#endif
//#ifdef CC_MT5363
//static BOOL gfgSCSwap=FALSE;
//#endif
static INT32 _ai4ScalerRegion[3] = {(-1), (-1), (-1)};
#ifdef SYS_3D_SUPPORT
static BOOL _fgForceSize = 0;
#endif

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
EXTERN BOOL gfgOSD_Mirror;
EXTERN BOOL gfgOSD_Flip;


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void OSD_SC_UpdateDstSize(UINT32 u4Scaler)
{
    OSD_SCALER_INFO* prInfo = &_arScalerInfo[u4Scaler];
    //#ifdef CC_MT5363
    //if(gfgSCSwap)
    //{
    //	if(u4Scaler==OSD_SCALER_2)
    //		prInfo = &_arScalerInfo[OSD_SCALER_3];
    //	else
    //		prInfo = &_arScalerInfo[OSD_SCALER_2];
    //}
    //#endif
    IGNORE_RET(OSD_SC_Scale(u4Scaler, prInfo->u4Enable,
                            prInfo->u4SrcWidth, prInfo->u4SrcHeight,
                            prInfo->u4DstWidth, prInfo->u4DstHeight));
}


INT32 OSD_SC_CheckCapability(UINT32 u4SrcW, UINT32 u4SrcH, UINT32 u4DstH)
{
#if 0
    UINT32 u4InTime, u4OutTime, u4OsdClock, u4OutputClock, u4HTotal;
    u4HTotal = (bReadSCPOS(RW_PIP_OHL) << 4) |
               (bReadSCPOSMsk(RW_PIP_OHSL, MOHLEN3_0) >> 4);
    u4OsdClock = BSP_GetClock(CAL_SRC_SYSPLL, NULL, NULL, NULL, NULL) / 2000;
    u4OutputClock = BSP_GetClock(CAL_SRC_VOPLL, NULL, NULL, NULL, NULL) / 1000;
    u4InTime = ((u4SrcW * ((u4SrcH / u4DstH) + (((u4SrcH % u4DstH) != 0)?1:0)))
               * 1000000) / u4OsdClock;
    u4OutTime = (u4HTotal * 1000000) / u4OutputClock;
    if (u4InTime < u4OutTime)
    {
        return (INT32)OSD_RET_OK;
    }
#endif
    UNUSED(u4SrcW);
    UNUSED(u4SrcH);
    UNUSED(u4DstH);

#ifdef SYS_3D_SUPPORT
    if (_fgForceSize)
    {
        return (INT32)OSD_RET_INV_SCALER;    
    }
    else
    {
    return (INT32)OSD_RET_OK;
    }
#else
    return (INT32)OSD_RET_OK;
#endif
}


//-----------------------------------------------------------------------------
/** cinfugure osd scaler
 *  @param  u4Scaler scaler id(only OSD_SCALER_2)
 *  @param  fgEnable enable=TRUE/disable=FALSE
 *  @param  u4SrcWidth osd input width
 *  @param  u4SrcHeight osd input height
 *  @param  u4DstWidth osd output width
 *  @param  u4DstHeight osd output height
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_SC_Scale(UINT32 u4Scaler, UINT32 u4Enable, UINT32 u4SrcWidth,
                   UINT32 u4SrcHeight, UINT32 u4DstWidth, UINT32 u4DstHeight)
{
    UINT32 u4Tmp, u4MaxWidth, u4MaxHeight, fgPrgs;
    //UINT32 u4RgnID,u4Old_Y,u4New_Y,u4Tmp_Y,scn_h;
    UINT32 u4Old_Y,u4New_Y,u4Tmp_Y,scn_h;
    UINT32 u4Old_X,u4New_X,u4Tmp_X,scn_w;
    INT32 i4RgnID=(-1);
    OSD_VERIFY_SCALER(u4Scaler);
    //#ifdef CC_MT5363
    #if 0
    if(gfgSCSwap)
    {
    	if(u4Scaler==OSD_SCALER_2)
    		u4Scaler=OSD_SCALER_3;
    	else
    		u4Scaler=OSD_SCALER_2;
    }
    #endif

#if defined(DRV_SUPPORT_EXTMJC) && !defined(CC_MTK_LOADER)
    if (IS_COMPANION_CHIP_ON())
    {
        LOG(5, "drv_extmjc_set_osd_info, u4Enable=%d, u4DstWidth=%d, u4DstHeight=%d\n",
            u4Enable, u4DstWidth, u4DstHeight);
        drv_extmjc_set_osd_info(u4Enable, u4DstWidth, u4DstHeight, NULL);
    }
#endif

    IGNORE_RET(_OSD_BASE_GetScrnHSize(&u4MaxWidth));
    IGNORE_RET(_OSD_BASE_GetScrnVSize(&u4MaxHeight));
    IGNORE_RET(_OSD_BASE_GetPrgs(&fgPrgs));
    ASSERT((u4MaxWidth > 0) && (u4MaxHeight > 0));

    // protect if hstart+dst_w > max_w or vstart+dst_h > max_h
    #if defined(CC_MT5398)
	if (u4Scaler == (UINT32)OSD_SCALER_1)
	{
		IGNORE_RET(_OSD_BASE_GetOsd2HStart(&u4Tmp));
		u4MaxWidth -= u4Tmp;
		IGNORE_RET(_OSD_BASE_GetOsd2VStart(&u4Tmp));
		u4MaxHeight -= u4Tmp;
	}
	#endif
    if (u4Scaler == (UINT32)OSD_SCALER_2)
    {
        IGNORE_RET(_OSD_BASE_GetOsd2HStart(&u4Tmp));
        u4MaxWidth -= u4Tmp;
        IGNORE_RET(_OSD_BASE_GetOsd2VStart(&u4Tmp));
        u4MaxHeight -= u4Tmp;
    }
    //#ifdef CC_MT5363
    else if (u4Scaler == (UINT32)OSD_SCALER_3)
    {
        IGNORE_RET(_OSD_BASE_GetOsd3HStart(&u4Tmp));
        u4MaxWidth -= u4Tmp;
        IGNORE_RET(_OSD_BASE_GetOsd3VStart(&u4Tmp));
        u4MaxHeight -= u4Tmp;
    }
    //#endif
    // copy osd info
    _arScalerInfo[u4Scaler].u4Enable = u4Enable;
    _arScalerInfo[u4Scaler].u4SrcWidth = u4SrcWidth;
    _arScalerInfo[u4Scaler].u4SrcHeight = u4SrcHeight;
    _arScalerInfo[u4Scaler].u4DstWidth = u4DstWidth;
    _arScalerInfo[u4Scaler].u4DstHeight = u4DstHeight;

    if (u4SrcWidth == 0)
    {
        u4SrcWidth = u4MaxWidth;
    }
    if (u4DstWidth == 0)
    {
        u4DstWidth = u4MaxWidth;
    }
    if (u4SrcHeight == 0)
    {
        u4SrcHeight = (fgPrgs)?u4MaxHeight:(u4MaxHeight << 1);
    }
    if (u4DstHeight == 0)
    {
        u4DstHeight = (fgPrgs)?u4MaxHeight:(u4MaxHeight << 1);
    }

	if(u4SrcWidth > OSD_SC_MAX_SRCWIDTH)
	{
	  u4SrcWidth = OSD_SC_MAX_SRCWIDTH;
	}
	if(u4SrcHeight > OSD_SC_MAX_SRCHEIGHT)
	{
	  u4SrcWidth = OSD_SC_MAX_SRCHEIGHT;
	}
    // modify for MW's API require
    IGNORE_RET(_OSD_SC_SetScEn(u4Scaler, u4Enable));
    

    // to cut non-necessary src input
    if (u4DstWidth > u4MaxWidth)
    {
        u4SrcWidth = (u4MaxWidth * u4SrcWidth) / u4DstWidth;
        u4DstWidth = u4MaxWidth;
        // boundry condition protection
        u4SrcWidth = MAX(u4SrcWidth, 1);
        u4DstWidth = MAX(u4DstWidth, 2);
    }

    if (u4SrcWidth == u4DstWidth)
    {
        IGNORE_RET(_OSD_SC_SetHuscEn(u4Scaler, 0));
        
        IGNORE_RET(_OSD_SC_SetSrcHSize(u4Scaler, u4SrcWidth));
        IGNORE_RET(_OSD_SC_SetVscHSize(u4Scaler, u4SrcWidth));
        IGNORE_RET(_OSD_SC_SetDstHSize(u4Scaler, u4SrcWidth));
        // clear
        IGNORE_RET(_OSD_SC_SetHuscOfst(u4Scaler, 0));
        IGNORE_RET(_OSD_SC_SetHuscStep(u4Scaler, 0));
		
	    #if defined(CC_MT5398)
		IGNORE_RET(_OSD_SC_SetHdscEn(u4Scaler, 0));
        IGNORE_RET(_OSD_SC_SetHdscOfst(u4Scaler, 0));
        IGNORE_RET(_OSD_SC_SetHdscStep(u4Scaler, 0));
        #endif
    }
    else
    {
        if (u4SrcWidth < u4DstWidth)
        {
            // horizontal scaling up
            IGNORE_RET(_OSD_SC_SetHuscEn(u4Scaler, 1));
            //IGNORE_RET(_OSD_SC_SetHdscEn(u4Scaler, 0));
            IGNORE_RET(_OSD_SC_SetHuscOfst(u4Scaler, 0));

            u4Tmp = ((u4SrcWidth - 1) << OSD_SC_STEP_BIT) / (u4DstWidth - 1);
            IGNORE_RET(_OSD_SC_SetHuscStep(u4Scaler, u4Tmp));

            IGNORE_RET(_OSD_SC_SetSrcHSize(u4Scaler, u4SrcWidth));
            IGNORE_RET(_OSD_SC_SetVscHSize(u4Scaler, u4SrcWidth));
            IGNORE_RET(_OSD_SC_SetDstHSize(u4Scaler, u4DstWidth));
        }
        else
        {
#if 1
            // horizontal scaling down
            
            IGNORE_RET(_OSD_SC_SetHuscEn(u4Scaler, 0));
            u4Tmp = ((u4DstWidth << OSD_SC_STEP_BIT) / u4SrcWidth) +
                    (((u4DstWidth << OSD_SC_STEP_BIT) % u4SrcWidth)?1:0);
			#if defined(CC_MT5398)
			IGNORE_RET(_OSD_SC_SetHdscEn(u4Scaler, 1));
            IGNORE_RET(_OSD_SC_SetHdscOfst(u4Scaler, u4Tmp));
            IGNORE_RET(_OSD_SC_SetHdscStep(u4Scaler, u4Tmp));
			#endif
			
            IGNORE_RET(_OSD_SC_SetSrcHSize(u4Scaler, u4SrcWidth));
            IGNORE_RET(_OSD_SC_SetVscHSize(u4Scaler, u4DstWidth));
            IGNORE_RET(_OSD_SC_SetDstHSize(u4Scaler, u4DstWidth));
#else
            // enable both scale up and scale down, for FPGA verification
            _OSD_SC_SetHdscEn(u4Scaler, 1);
            _OSD_SC_SetHuscEn(u4Scaler, 1);
            u4Tmp = ((u4DstWidth << (OSD_SC_STEP_BIT - 1)) / u4SrcWidth) +
                    (((u4DstWidth << (OSD_SC_STEP_BIT - 1)) % u4SrcWidth)?1:0);
            _OSD_SC_SetHdscOfst(u4Scaler, u4Tmp);
            _OSD_SC_SetHdscStep(u4Scaler, u4Tmp);

            _OSD_SC_SetSrcHSize(u4Scaler, u4SrcWidth);
            _OSD_SC_SetVscHSize(u4Scaler, u4DstWidth >> 1);

            _OSD_SC_SetHuscOfst(u4Scaler, 0);
            u4Tmp = (((u4DstWidth >> 1) - 1) << OSD_SC_STEP_BIT) /
                    (u4DstWidth - 1);
            _OSD_SC_SetHuscStep(u4Scaler, u4Tmp);
            _OSD_SC_SetDstHSize(u4Scaler, u4DstWidth);
#endif
        }      
    }

    if (u4SrcHeight == u4DstHeight)
    {
        //IGNORE_RET(_OSD_SC_SetVdscEn(u4Scaler, 0));
        IGNORE_RET(_OSD_SC_SetVuscEn(u4Scaler, 0));

        if (fgPrgs)
        {
            if (u4DstHeight > u4MaxHeight)
            {
                u4DstHeight = u4MaxHeight;
            }
            IGNORE_RET(_OSD_SC_SetSrcVSize(u4Scaler, u4DstHeight));
            IGNORE_RET(_OSD_SC_SetDstVSize(u4Scaler, u4DstHeight));
        }
        else
        {
            u4DstHeight >>= 1;
            if (u4DstHeight > u4MaxHeight)
            {
                u4DstHeight = u4MaxHeight;
            }
            IGNORE_RET(_OSD_SC_SetSrcVSize(u4Scaler, u4DstHeight));
            IGNORE_RET(_OSD_SC_SetDstVSize(u4Scaler, u4DstHeight));
        }

        // clear
        IGNORE_RET(_OSD_SC_SetVscOfstTop(u4Scaler, 0));
        IGNORE_RET(_OSD_SC_SetVscOfstBot(u4Scaler, 0));
        IGNORE_RET(_OSD_SC_SetVscStep(u4Scaler, 0));

        // to choose osd clock as output clock
        //OSD_BASE_SetClock(OSD_CK_OCLK);
        //IGNORE_RET(OSD_BASE_SetClock(OSD_CK_TVD));
    }
    else
    {
        if (!fgPrgs) // interlaced mode
        {
            u4DstHeight = u4DstHeight >> 1;
        }
        if (u4DstHeight > u4MaxHeight)
        {
            u4SrcHeight = (u4MaxHeight * u4SrcHeight) / u4DstHeight;
            u4DstHeight = u4MaxHeight;
            // boundry condition protection
            u4SrcHeight = MAX(u4SrcHeight, 1);
            u4DstHeight = MAX(u4DstHeight, 2);

        }

        // "=" only happen when src=2*dst in interlaced mode
        if (u4SrcHeight <= u4DstHeight)
        {
            // vertical scaling up
            //IGNORE_RET(_OSD_SC_SetVdscEn(u4Scaler, 0));
            IGNORE_RET(_OSD_SC_SetVuscEn(u4Scaler, 1));

            if (fgPrgs)
            {
                u4Tmp = ((u4SrcHeight - 1) << OSD_SC_STEP_BIT) /
                        (u4DstHeight - 1);

                IGNORE_RET(_OSD_SC_SetVscOfstTop(u4Scaler, 0));
                IGNORE_RET(_OSD_SC_SetVscOfstBot(u4Scaler, 0));
            }
            else
            {
                u4Tmp = ((u4SrcHeight - 1 ) << OSD_SC_STEP_BIT) / (u4DstHeight);
                if ((u4Tmp % (1 << OSD_SC_STEP_BIT)) == 0)
                {
                    u4Tmp--;
                }
                IGNORE_RET(_OSD_SC_SetVscOfstTop(u4Scaler, u4Tmp >> 2));
                IGNORE_RET(_OSD_SC_SetVscOfstBot(u4Scaler, ((3 * u4Tmp) >> 2) &
                           0x3ffc));
            }

            IGNORE_RET(_OSD_SC_SetVscStep(u4Scaler, u4Tmp));
            IGNORE_RET(_OSD_SC_SetSrcVSize(u4Scaler, u4SrcHeight));
            IGNORE_RET(_OSD_SC_SetDstVSize(u4Scaler, u4DstHeight));


            // to choose osd clock as output clock
            //OSD_BASE_SetClock(OSD_CK_OCLK);
            //IGNORE_RET(OSD_BASE_SetClock(OSD_CK_TVD));
        }
        else
        {
            if (OSD_SC_CheckCapability(u4SrcWidth, u4SrcHeight, u4DstHeight) !=
                (INT32)OSD_RET_OK)
            {
                LOG(7, "OSD_SC_CheckCapability: fail\n");
            }

            // vertical scaling down
            //IGNORE_RET(_OSD_SC_SetVdscEn(u4Scaler, 1));
            IGNORE_RET(_OSD_SC_SetVuscEn(u4Scaler, 0));

            if (fgPrgs)
            {
                u4Tmp = ((u4DstHeight << OSD_SC_STEP_BIT) / u4SrcHeight) +
                        (((u4DstHeight << OSD_SC_STEP_BIT) % u4SrcHeight)?1:0);
                IGNORE_RET(_OSD_SC_SetVscOfstTop(u4Scaler, u4Tmp));
                IGNORE_RET(_OSD_SC_SetVscOfstBot(u4Scaler, u4Tmp));
            }
            else
            {
                if (u4DstHeight < (u4SrcHeight >> 1))
                {
                    //step<=0x2000
                    u4Tmp = (((u4DstHeight + 1) << OSD_SC_STEP_BIT) /
                             u4SrcHeight) +
                            ((((u4DstHeight + 1) << OSD_SC_STEP_BIT) %
                              u4SrcHeight)?1:0);
                    if (u4Tmp == (1 << (OSD_SC_STEP_BIT - 1)))
                    {
                        u4Tmp--;
                    }
                    IGNORE_RET(_OSD_SC_SetVscOfstBot(u4Scaler, u4Tmp));
                    IGNORE_RET(_OSD_SC_SetVscOfstTop(u4Scaler, u4Tmp +
                               (1 << (OSD_SC_STEP_BIT - 1))));
                }
                else
                {
                    //step>0x2000
                    u4Tmp = (((u4DstHeight + 1) << OSD_SC_STEP_BIT) /
                             u4SrcHeight) +
                            ((((u4DstHeight + 1) << OSD_SC_STEP_BIT) %
                              u4SrcHeight)?1:0);
                    if ((u4Tmp % (1 << OSD_SC_STEP_BIT)) == 0)
                    {
                        u4Tmp--;
                    }
                    IGNORE_RET(_OSD_SC_SetVscOfstBot(u4Scaler, u4Tmp -
                               (1 << (OSD_SC_STEP_BIT - 1))));
                    IGNORE_RET(_OSD_SC_SetVscOfstTop(u4Scaler, u4Tmp));
                }
            }

            IGNORE_RET(_OSD_SC_SetVscStep(u4Scaler, u4Tmp));
            IGNORE_RET(_OSD_SC_SetSrcVSize(u4Scaler, u4SrcHeight));
            IGNORE_RET(_OSD_SC_SetDstVSize(u4Scaler, u4DstHeight));

/*            
            // to choose osd clock as syspll_d2_ck
#ifdef CC_LOW_POWER
            IGNORE_RET(OSD_BASE_SetClock(OSD_CK_SYS));
#else
            IGNORE_RET(OSD_BASE_SetClock(OSD_CK_TVD));
#endif
*/
        }     
    }

    _OSD_SC_SetAlphaCountEn(u4Scaler, u4Enable);

	i4RgnID = OSD_PLA_GetFirstRegion(u4Scaler + 1);

    OSD_RGN_MirrorFlipUpdate(i4RgnID, TRUE, TRUE);	
	
    IGNORE_RET(_OSD_SC_UpdateHwReg(u4Scaler));
    return (INT32)OSD_RET_OK;
}

#ifdef CC_DEBUG
INT32 OSD_SC_HDown(UINT32 u4Scaler, UINT32 u4SrcWidth, UINT32 u4Step)
{
#if defined(CC_MT5398)
    UINT32 u4DstWidth;
	if( u4Scaler == OSD_SCALER_1)
	{
		return OSD_RET_INV_SCALER;
	}

    OSD_VERIFY_SCALER(u4Scaler);
    IGNORE_RET(_OSD_SC_SetScEn(u4Scaler, 1));
    IGNORE_RET(_OSD_SC_SetHdscEn(u4Scaler, 1));
    IGNORE_RET(_OSD_SC_SetHuscEn(u4Scaler, 0));
    IGNORE_RET(_OSD_SC_SetHdscOfst(u4Scaler, u4Step));
    IGNORE_RET(_OSD_SC_SetHdscStep(u4Scaler, u4Step));

    u4DstWidth = (u4SrcWidth * u4Step) >> OSD_SC_STEP_BIT;
    IGNORE_RET(_OSD_SC_SetVscHSize(u4Scaler, u4DstWidth));
    IGNORE_RET(_OSD_SC_SetDstHSize(u4Scaler, u4DstWidth));

    IGNORE_RET(_OSD_SC_UpdateHwReg(u4Scaler));
#endif
    return (INT32)OSD_RET_OK;
}


INT32 OSD_SC_HUp(UINT32 u4Scaler, UINT32 u4SrcWidth, UINT32 u4Step)
{
    UINT32 u4DstWidth, u4MaxWidth;
    OSD_VERIFY_SCALER(u4Scaler);

    IGNORE_RET(_OSD_SC_SetScEn(u4Scaler, 1));
    //IGNORE_RET(_OSD_SC_SetHdscEn(u4Scaler, 0));
    IGNORE_RET(_OSD_SC_SetHuscEn(u4Scaler, 1));
    //IGNORE_RET(_OSD_SC_SetHuscOfst(u4Scaler, 0));
    IGNORE_RET(_OSD_SC_SetHuscStep(u4Scaler, u4Step));

    u4DstWidth = (((u4SrcWidth - 1) * OSD_SC_STEP_BASE) / u4Step) + 1;
    IGNORE_RET(_OSD_BASE_GetScrnHSize(&u4MaxWidth));
    if (u4DstWidth>u4MaxWidth)
    {
        u4DstWidth = u4MaxWidth;
        u4SrcWidth = (((u4DstWidth - 1) * u4Step) >> OSD_SC_STEP_BIT) +
                     ((((u4DstWidth - 1) * u4Step) % OSD_SC_STEP_BASE)?1:0) + 1;
        IGNORE_RET(_OSD_SC_SetVscHSize(u4Scaler, u4SrcWidth));
        IGNORE_RET(_OSD_SC_SetSrcHSize(u4Scaler, u4SrcWidth));
    }
    else
    {
        IGNORE_RET(_OSD_SC_SetVscHSize(u4Scaler, u4SrcWidth));
    }
    IGNORE_RET(_OSD_SC_SetDstHSize(u4Scaler, u4DstWidth));

    IGNORE_RET(_OSD_SC_UpdateHwReg(u4Scaler));

    return (INT32)OSD_RET_OK;
}


INT32 OSD_SC_VDown(UINT32 u4Scaler, UINT32 u4SrcHeight, UINT32 u4Step)
{
#if defined(CC_MT5398)
    UINT32 u4DstHeight = 0, fgPrgs;
	if( u4Scaler == OSD_SCALER_1)
	{
		return OSD_RET_INV_SCALER;
	}

    OSD_VERIFY_SCALER(u4Scaler);
    IGNORE_RET(_OSD_BASE_GetPrgs(&fgPrgs));
    IGNORE_RET(_OSD_SC_SetScEn(u4Scaler, 1));
    IGNORE_RET(_OSD_SC_SetVdscEn(u4Scaler, 1));
    IGNORE_RET(_OSD_SC_SetVuscEn(u4Scaler, 0));

    if (fgPrgs)
    {
        IGNORE_RET(_OSD_SC_SetVscOfstTop(u4Scaler, u4Step));
        IGNORE_RET(_OSD_SC_SetVscOfstBot(u4Scaler, u4Step));
        IGNORE_RET(_OSD_SC_SetVscStep(u4Scaler, u4Step));
        u4DstHeight = (u4SrcHeight * u4Step) >> OSD_SC_STEP_BIT;
    }
    IGNORE_RET(_OSD_SC_SetDstVSize(u4Scaler, u4DstHeight));

    IGNORE_RET(_OSD_SC_UpdateHwReg(u4Scaler));
#endif
    return (INT32)OSD_RET_OK;
}


INT32 OSD_SC_VUp(UINT32 u4Scaler, UINT32 u4SrcHeight, UINT32 u4Step)
{
    UINT32 u4DstHeight = 0, u4MaxHeight, fgPrgs;
    OSD_VERIFY_SCALER(u4Scaler);
    IGNORE_RET(_OSD_BASE_GetPrgs(&fgPrgs));

    IGNORE_RET(_OSD_SC_SetScEn(u4Scaler, 1));
    //IGNORE_RET(_OSD_SC_SetVdscEn(u4Scaler, 0));
    IGNORE_RET(_OSD_SC_SetVuscEn(u4Scaler, 1));

    if (fgPrgs)
    {
        IGNORE_RET(_OSD_SC_SetVscOfstTop(u4Scaler, 0));
        IGNORE_RET(_OSD_SC_SetVscOfstBot(u4Scaler, 0));
        IGNORE_RET(_OSD_SC_SetVscStep(u4Scaler, u4Step));

        u4DstHeight = (((u4SrcHeight - 1) * OSD_SC_STEP_BASE) / u4Step) + 1;
        IGNORE_RET(_OSD_BASE_GetScrnVSize(&u4MaxHeight));
        if (u4DstHeight>u4MaxHeight)
        {
            u4DstHeight = u4MaxHeight;
            u4SrcHeight = (((u4DstHeight - 1) * u4Step) >> OSD_SC_STEP_BIT) +
                          ((((u4DstHeight - 1) * u4Step) % OSD_SC_STEP_BASE)?
                           1:0) + 1;
            IGNORE_RET(_OSD_SC_SetSrcVSize(u4Scaler, u4SrcHeight));
        }
    }
    IGNORE_RET(_OSD_SC_SetDstVSize(u4Scaler, u4DstHeight));

    IGNORE_RET(_OSD_SC_UpdateHwReg(u4Scaler));

    return (INT32)OSD_RET_OK;
}
#endif


//-----------------------------------------------------------------------------
/** get osd scaler infomation
 *  @param  u4Scaler scaler id(only OSD_SCALER_2)
 *  @param  pu4Enable enable pointer
 *  @param  pu4SrcW osd input width pointer
 *  @param  pu4SrcH osd input height pointer
 *  @param  pu4DstW osd output width pointer
 *  @param  pu4DstH osd output height pointer
 *  @param  pu4Is16Bpp don't care
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_SC_GetScalerInfo(UINT32 u4Scaler, UINT32* pu4Enable,
                           UINT32* pu4SrcW, UINT32* pu4SrcH,
                           UINT32* pu4DstW, UINT32* pu4DstH,
                           UINT32* pu4Is16Bpp)
{
    OSD_VERIFY_SCALER(u4Scaler);
    OSD_CHECK_NULL(pu4Is16Bpp);

   // #ifdef CC_MT5363
   #if 0
    if(gfgSCSwap)
    {
    	if(u4Scaler==OSD_SCALER_2)
    		u4Scaler=OSD_SCALER_3;
    	else
    		u4Scaler=OSD_SCALER_2;
    }
   #endif
    //IGNORE_RET(_OSD_SC_GetSrcHSize(u4Scaler, pu4SrcW));
    //IGNORE_RET(_OSD_SC_GetSrcVSize(u4Scaler, pu4SrcH));
    //IGNORE_RET(_OSD_SC_GetDstHSize(u4Scaler, pu4DstW));
    //IGNORE_RET(_OSD_SC_GetDstVSize(u4Scaler, pu4DstH));    
   *pu4SrcW =  _arScalerInfo[u4Scaler].u4SrcWidth;
   *pu4SrcH = _arScalerInfo[u4Scaler].u4SrcHeight;
   *pu4DstW =  _arScalerInfo[u4Scaler].u4DstWidth;
   *pu4DstH = _arScalerInfo[u4Scaler].u4DstHeight;

    *pu4Is16Bpp = 0;

    return (INT32)OSD_RET_OK;
}


INT32 OSD_SC_SetLpfInfo(UINT32 u4Scaler, UINT32 u4Enable, INT16 i2C1,
                        INT16 i2C2, INT16 i2C3, INT16 i2C4, INT16 i2C5)
{
   /*
    UINT32 tmp;

    OSD_VERIFY_SCALER(u4Scaler);
    IGNORE_RET(_OSD_SC_SetScLpfEn(u4Scaler, u4Enable));

    OSD_SET_LPF_SIGN_NUM(tmp, 32, i2C1);
    IGNORE_RET(_OSD_SC_SetScLpfC1(u4Scaler, tmp));

    OSD_SET_LPF_SIGN_NUM(tmp, 64, i2C2);
    IGNORE_RET(_OSD_SC_SetScLpfC2(u4Scaler, tmp));

    if ((i2C3 > 127) || (i2C3 < 0))
    {
        return -(INT32)OSD_RET_INV_ARG;
    }
    tmp = (UINT16)i2C3;
    IGNORE_RET(_OSD_SC_SetScLpfC3(u4Scaler, tmp));

    UNUSED(i2C4);
    UNUSED(i2C5);


    if (u4Enable)
    {
        IGNORE_RET(_OSD_SC_SetScEn(u4Scaler, TRUE));
    }

    IGNORE_RET(_OSD_SC_UpdateHwReg(u4Scaler));
*/
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** enable low pass filter
 *  @param  u4Scaler scaler id(only OSD_SCALER_2)
 *  @param  fgEnable enable=TRUE/disable=FALSE
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_SC_SetLpf(UINT32 u4Scaler, UINT32 u4Enable)
{
    return OSD_SC_SetLpfInfo(u4Scaler, u4Enable, OSD_DEFAULT_LPF_C1,
                             OSD_DEFAULT_LPF_C2, OSD_DEFAULT_LPF_C3,
                             OSD_DEFAULT_LPF_C4, OSD_DEFAULT_LPF_C5);
}


INT32 OSD_SC_GetLpfInfo(UINT32 u4Scaler, UINT32* pu4Enable, INT16* pi2C1,
                        INT16* pi2C2, INT16* pi2C3, INT16* pi2C4, INT16* pi2C5)
{
    /*
    UINT32 tmp = 0;

    OSD_CHECK_NULL(pu4Enable);
    OSD_CHECK_NULL(pi2C1);
    OSD_CHECK_NULL(pi2C2);
    OSD_CHECK_NULL(pi2C3);
    OSD_CHECK_NULL(pi2C4);
    OSD_CHECK_NULL(pi2C5);

    OSD_VERIFY_SCALER(u4Scaler);
    IGNORE_RET(_OSD_SC_GetScLpfEn(u4Scaler, pu4Enable));

    IGNORE_RET(_OSD_SC_GetScLpfC1(u4Scaler, &tmp));
    OSD_GET_LPF_SIGN_NUM(tmp, 32, *pi2C1);

    IGNORE_RET(_OSD_SC_GetScLpfC2(u4Scaler, &tmp));
    OSD_GET_LPF_SIGN_NUM(tmp, 64, *pi2C2);

    IGNORE_RET(_OSD_SC_GetScLpfC3(u4Scaler, &tmp));
    *pi2C3 = (INT16)tmp;

    (void)pi2C4;
    (void)pi2C5;
*/
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** dump osd scaler debug info
 *  @param  u4Scaler osd scaler
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_SC_Dump(UINT32 u4Scaler)
{
    OSD_SC_UNION_T rOsdScalerReg;
//    UINT32 tmp1, tmp2, tmp3;
//    INT16 i2C1, i2C2, i2C3;

    OSD_VERIFY_SCALER(u4Scaler);

    IGNORE_RET(_OSD_SC_GetReg(u4Scaler, rOsdScalerReg.au4Reg));
    Printf("OSD SCALER[%ld]\n", u4Scaler);
    Printf("\tEnable=(VU=%d, HU=%d, EN=%d)\n", 
           rOsdScalerReg.rField.fgVuscEn, rOsdScalerReg.rField.fgHuscEn,
            rOsdScalerReg.rField.fgScEn);
    Printf("\tSrcSize=(%d, %d), DstDize=(%d, %d)\n",
           rOsdScalerReg.rField.u4SrcHSize, rOsdScalerReg.rField.u4SrcVSize,
           rOsdScalerReg.rField.u4DstHSize, rOsdScalerReg.rField.u4DstVSize);
    /*
    IGNORE_RET(_OSD_SC_GetScLpfC1(u4Scaler, &tmp1));
    OSD_GET_LPF_SIGN_NUM(tmp1, 32, i2C1);
    IGNORE_RET(_OSD_SC_GetScLpfC2(u4Scaler, &tmp2));
    OSD_GET_LPF_SIGN_NUM(tmp2, 64, i2C2);
    IGNORE_RET(_OSD_SC_GetScLpfC3(u4Scaler, &tmp3));
    i2C3 = (INT16)tmp3;
    Printf("\tLpfCoe=(%d, %d, %d), LpfCoeRaw=(%d, %d, %d)\n",
           i2C1, i2C2, i2C3, tmp1, tmp2, tmp3);
    */
    return (INT32)OSD_RET_OK;
}

//#ifdef CC_MT5363
#if 0
INT32 OSD_SC_Swap(BOOL fgSwap)
{
	OSD_SC_UNION_T rOsdScalerRegTmp1,rOsdScalerRegTmp2;
	OSD_SCALER_INFO rScalerInfoTmp;

	if(gfgSCSwap==fgSwap)
		return (INT32)OSD_RET_OK;

	gfgSCSwap=fgSwap;

	_OSD_SC_GetReg(OSD_SCALER_2, rOsdScalerRegTmp1.au4Reg);	//get scaler2 reg to tmp1
	_OSD_SC_GetReg(OSD_SCALER_3, rOsdScalerRegTmp2.au4Reg);	//get scaler3 reg to tmp2
	_OSD_SC_SetReg(OSD_SCALER_2, rOsdScalerRegTmp2.au4Reg);	//Set scaler2 reg with tmp2(old scaler3)
	_OSD_SC_SetReg(OSD_SCALER_3, rOsdScalerRegTmp1.au4Reg);	//Set scaler2 reg with tmp1(old scaler2)

	rScalerInfoTmp=_arScalerInfo[0];
	_arScalerInfo[0]=_arScalerInfo[1];
	_arScalerInfo[1]=rScalerInfoTmp;

	_OSD_SC_UpdateHwReg(OSD_SCALER_2);
	_OSD_SC_UpdateHwReg(OSD_SCALER_3);

	return (INT32)OSD_RET_OK;
}
#endif


INT32 OSD_SC_SetRegion(UINT32 u4Scaler, INT32 i4Region)
{
    _ai4ScalerRegion[u4Scaler] = i4Region;
#ifndef CC_FLIP_MIRROR_SUPPORT    
    (void)(_ai4ScalerRegion[u4Scaler]);
#endif    
    return (INT32)OSD_RET_OK;
}
INT32 OSD_SC_GetScaler(UINT32 u4_plane)
{
    INT32   i4_sc = -1;
    
    switch(u4_plane)
    {
 //       case OSD_PLANE_1:
 //           i4_sc = OSD_SCALER_1;
            break;
        case OSD_PLANE_2:
            i4_sc = OSD_SCALER_2;
            break;
        case OSD_PLANE_3:
            i4_sc = OSD_SCALER_3;
            break;
    }
    
    return i4_sc;
}


INT32 OSD_SC_SetDstSize(UINT32 u4Scaler,UINT32 u4Width, UINT32 u4Height)
{
     UINT32 u4Enable,u4SrcW, u4SrcH, u4DstW, u4DstH, u4Is16Bpp;

     OSD_SC_GetScalerInfo(u4Scaler, &u4Enable, &u4SrcW, &u4SrcH, &u4DstW, &u4DstH, &u4Is16Bpp);

	 OSD_SC_Scale(u4Scaler, 1, u4SrcW, u4SrcH, u4Width, u4Height);

	 return 0;
}

