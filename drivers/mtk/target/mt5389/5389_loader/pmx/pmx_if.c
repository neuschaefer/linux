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
 * $RCSfile: pmx_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pmx_if.c
 *  plane mixer(pmx) public functions
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "pmx_hw.h"
#include "pmx_drvif.h"
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "drvcust_if.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/** Plane mixer configuration.
 */
typedef struct
{
    UINT32 u4BgColor;
    UCHAR aucPlaneOrder[PMX_MAX_INPORT_NS];
} PMX_CONF_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static PMX_CONF_T _rPmxConf =
{
    0x000000,
    {PMX_OSD2, PMX_OSD1, PMX_MAIN, PMX_PIP}
};

static UCHAR _fgPmxInit = 0;
static BOOL _fgPmxYuvMode = FALSE;

static UCHAR PMX_GetMixAlphaType(const UINT32* pu4PlaneOrder);
UINT32 PMX_SetMixAlpha(UCHAR ucType,const UINT32* pu4Alpha);



//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define VERIFY_NULL(ptr)                                    \
          do {                                              \
            if (ptr == NULL) { return PMX_SET_ERROR; }      \
          } while (0)


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** initialize pmx
 */
//-----------------------------------------------------------------------------
void PMX_Init(void)
{
    if (_fgPmxInit == 0)
    {
        _PMX_Init();
		
		PMX_Set_ALPHA_MIX_GRP1_0(TRUE);
		
        _fgPmxInit = 1;
    }
}


UINT32 PMX_SetPlaneOrderArray(const UINT32* pu4PlaneOrder)
{
    UINT32  u4PoIdx;
    UINT32  u4Alpha[4]={0x0,0x0,0x0,0x0};
    UINT32* pu4Alpha=u4Alpha;

    VERIFY_NULL(pu4PlaneOrder);
    
    for (u4PoIdx = 0; u4PoIdx < PMX_MAX_INPORT_NS; u4PoIdx++)
    {
        _rPmxConf.aucPlaneOrder[u4PoIdx] = pu4PlaneOrder[u4PoIdx];
    }
    
    if( PMX_MAX_INPORT_NS >= ( PMX_PLANE_NUMBER + 0x3 ) )
    {
         pu4Alpha = (UINT32*)&pu4PlaneOrder[PMX_ALPHA];
    }

    PMX_SetMixAlpha(PMX_GetMixAlphaType(pu4PlaneOrder),pu4Alpha);   
    
    _PMX_SetPlaneOrderArray(pu4PlaneOrder);
    
    return PMX_SET_OK;
}

UINT32 PMX_GetPlaneOrderArray(UINT32* pu4PlaneOrder)
{
	UINT32 u4PoIdx;

	VERIFY_NULL(pu4PlaneOrder);

	for (u4PoIdx = 0; u4PoIdx < PMX_MAX_INPORT_NS; u4PoIdx++)
	{
		pu4PlaneOrder[u4PoIdx]	=	_rPmxConf.aucPlaneOrder[u4PoIdx];
	}

	return PMX_SET_OK;
}



//-----------------------------------------------------------------------------
/** set plane z-order
 *  @param  ucPmxId only PMX_1
 *  @param  ucVdpId video plane=VDP1/osd plane 2=VDP4
 *  @param  ucPlaneOrder PLA_MXR_Z_ORDER_1 or PLA_MXR_Z_ORDER_2
 *  @retval PMX_SET_OK success
 */
//-----------------------------------------------------------------------------
UINT32 PMX_SetPlaneOrder(UCHAR ucVdpId, UCHAR ucPlaneOrder)
{
    _rPmxConf.aucPlaneOrder[ucPlaneOrder] = ucVdpId;

    _PMX_SetPlaneOrder(ucVdpId, ucPlaneOrder);

    return PMX_SET_OK;
}


//-----------------------------------------------------------------------------
/** get plane z-order
 *  @param  ucPmxId only PMX_1
 *  @param  ucVdpId video plane=VDP1/osd plane 2=VDP4
 *  @param  pucPlaneOrder order pointer
 *  @retval PMX_SET_OK success
 */
//-----------------------------------------------------------------------------
UINT32 PMX_GetPlaneOrder(UCHAR ucVdpId, UCHAR* pucPlaneOrder)
{
    UINT32 u4PoIdx;

    VERIFY_NULL(pucPlaneOrder);

    for (u4PoIdx = 0; u4PoIdx < PMX_MAX_INPORT_NS; u4PoIdx++)
    {
        if (_rPmxConf.aucPlaneOrder[u4PoIdx] == ucVdpId)
        {
            *pucPlaneOrder = u4PoIdx;
            return PMX_SET_OK;
        }
    }

    *pucPlaneOrder = PMX_MAX_INPORT_NS;
    return PMX_SET_OK;
}


//-----------------------------------------------------------------------------
/** set pmx background color
 *  @param  ucPmxId only PMX_1
 *  @param  u4BgColor background color(0xrrggbb)
 *  @retval PMX_SET_OK success
 */
//-----------------------------------------------------------------------------
void PMX_SetYuvMode(BOOL fgYuvMode)
{
    _fgPmxYuvMode = fgYuvMode;
    IGNORE_RET(PMX_SetBg(_rPmxConf.u4BgColor, TRUE));
    PMX_OnOutputVSync();
}

UINT32 PMX_SetBg(UINT32 u4BgColor, BOOL fgInVsync)
{
    _rPmxConf.u4BgColor = u4BgColor;
    _PMX_SetBgColor(u4BgColor, _fgPmxYuvMode, fgInVsync);

    return PMX_SET_OK;
}

//-----------------------------------------------------------------------------
/** setup alpha value for video plane
 *  @param  ucPlaneId video plane id(onlu VDP_1 in 5381)
 *  @param  ucValue alpha value(0~255)
 *  @retval PMX_SET_OK success
 */
//-----------------------------------------------------------------------------
UINT32 PMX_SetAlpha(UCHAR ucPlaneId, UCHAR ucValue)
{
    if ((ucPlaneId != PMX_MAIN) && (ucPlaneId != PMX_PIP))
    {
        return PMX_SET_ERROR;
    }

    _PMX_SetAlpha(ucPlaneId, ucValue);

    return PMX_SET_OK;
}


void PMX_Set_ALPHA_MIX_GRP1_0(BOOL fgEnable)
{
	_PMX_Set_ALPHA_MIX_GRP1_0(fgEnable);

	return;
}

static UCHAR PMX_GetMixAlphaType(const UINT32* pu4PlaneOrder)
{
    UCHAR   ucTemp          =0x0;
    UCHAR   ucVideoBot      =0x0;
    
    #ifdef CC_MT5396
    UCHAR   ucOsdNum        =0x3;
    #else
    UCHAR   ucOsdNum        =0x2;
    #endif
    
    for(ucTemp=0x0; ucTemp<ucOsdNum; ucTemp++)
    {
        if( ( pu4PlaneOrder[ucTemp]==PMX_OSD1 ) ||
            ( pu4PlaneOrder[ucTemp]==PMX_OSD2 ) || 
            ( pu4PlaneOrder[ucTemp]==PMX_OSD3 ) )
        {
            ucVideoBot++;
        }
    }

    if(ucVideoBot == ucOsdNum)
    {
        return  PMX_MIX_AL_VIDEO_BOT;
    }
    
    if((PMX_PIP == pu4PlaneOrder[0])||
        (PMX_MAIN == pu4PlaneOrder[0]))
    {
        return PMX_MIX_AL_VIDEO_TOP;
    }

    if((PMX_PIP == pu4PlaneOrder[PMX_ALPHA-1])||
        (PMX_MAIN == pu4PlaneOrder[PMX_ALPHA-1]))
    {
        return PMX_MIX_AL_VIDEO_BOT;
    }
    
    if((PMX_PIP == pu4PlaneOrder[PMX_ALPHA-2])||
        (PMX_MAIN == pu4PlaneOrder[PMX_ALPHA-2]))
    {
        return PMX_MIX_AL_VIDEO_1_OSD_BELOW;
    }
    
    return PMX_MIX_AL_VIDEO_2_OSD_BELOW;
}

//-----------------------------------------------------------------------------
/** setup alpha value for video plane
 *  @param  ucPlaneId video plane id(onlu VDP_1 in 5381)
 *  @param  ucValue alpha value(0~255)
 *  @retval PMX_SET_OK success
 */
//-----------------------------------------------------------------------------
UINT32 PMX_SetMixAlpha(UCHAR ucType,const UINT32* pu4Alpha)
{
    switch(ucType)
    {
        case PMX_MIX_AL_VIDEO_BOT:
            _PMX_Set_ALPHA_MIX_VIDEO_BOT(pu4Alpha);
            break;
        case PMX_MIX_AL_VIDEO_1_OSD_BELOW:
            _PMX_Set_ALPHA_MIX_VIDEO_1_OSD_BELOW(pu4Alpha);
            break;
        case PMX_MIX_AL_VIDEO_2_OSD_BELOW:
            _PMX_Set_ALPHA_MIX_VIDEO_2_OSD_BELOW(pu4Alpha);
            break;
        case PMX_MIX_AL_VIDEO_TOP:
            _PMX_Set_ALPHA_MIX_VIDEO_TOP(pu4Alpha);
            break;
        default:
            return PMX_SET_ERROR;
    }

    return PMX_SET_OK;
}

