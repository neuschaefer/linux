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
 * $RCSfile: pmx_hw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pmx_hw.c
 *  Brief of file pmx_hw.c.
 *  Details of file pmx_hw.c (optional).
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "pmx_hw.h"
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "osd_drvif.h"
#include "pmx_drvif.h"
#include "x_hal_5381.h"
#include "drvcust_if.h"
//#include "vdp_drvif.h"
#include "drv_scaler.h"
#if defined(CC_MTK_LOADER)
#define DEFINE_IS_LOG(level, fmt...) Printf(fmt)
#else /* CC_MTK_LOADER  */
#define DEFINE_IS_LOG PMX_IsLog
#endif /* CC_MTK_LOADER */
#if defined(CC_MT5368) ||defined(CC_MT5396)
#include "hw_ospe.h"
#endif
#include "x_debug.h"
LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


// hw id for plane
#define PMX_SRC_PIP 0
#define PMX_SRC_MAIN 1
#define PMX_SRC_OSD1 3
#define PMX_SRC_OSD3 2
#ifdef CC_MT5360
    #ifdef CC_MT5360B
		#define PMX_SRC_OSD2 3
		#else
   	#define PMX_SRC_OSD2 2
   	#endif
#else
    #define PMX_SRC_OSD2 3
#endif

//#define PMX_SIZE_OF_MIX_CTRL_FIELD 2
#define PMX_SIZE_OF_MIX_CTRL_FIELD 1

#define PMX_UPDATE_PLANE_ORDER 1
#define PMX_UPDATE_BG_COLOR 2

#define PMX_TOP_PLANE_INDEX 3

#if 0
#define PMX_UPDATE_DISPLAY_MODE 8
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
/*
typedef struct _PMX_MIX_GRP_FIELD_T
{
    UINT8 fgTopMixBg   :   1;
    UINT8 fgDeOr       :   1;
    UINT8 fgMuteDe     :   1;
    UINT8 fgGrpSel     :   2;
    UINT8 fgBypass     :   1;
    UINT8              :   2;
LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed)) PMX_MIX_GRP_FIELD_T;
*/
// arGroup[0] is bottommost, arGroup[4] is topmost and must be OSD1
/*
typedef struct _PMX_MIX_CTRL_FIELD_T
{
    PMX_MIX_GRP_FIELD_T arGroup[5];

    UINT8 fgMainAlpha;
    UINT8 fgPipAlpha;
    UINT8 fgOutputDelay;
LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed)) PMX_MIX_CTRL_FIELD_T;
*/
typedef struct _PMX_MIX_CTRL_FIELD_T
{
    UINT32        MainTopEn          :   1;	//0: PIP top, 1: main top
    UINT32                      :   7;
    UINT32        AlphaMixGrp1_0          :   1;	//Group1 Top mixed with Bot, 0: bypass Bot, 1: mix output
    UINT32        AlphaMixGrp1_1          :   1;   //Group1 DE output, 0: Bot DE, 1: Top and Bot DE
    UINT32                  :   3; 
    UINT32        AlphaMixGrp1_5          :   1;   //Group1 bypass mode
    UINT32                  :   2; 
    UINT32        AlphaMixGrp2_0          :   1;	//Group2 Top mixed with Bot, 0: bypass Bot, 1: mix output
    UINT32        AlphaMixGrp2_1          :   1;   //Group2 DE output, 0: Bot DE, 1: Top and Bot DE
    UINT32                  :   3; 
    UINT32        AlphaMixGrp2_5          :   1;   //Group2 bypass mode
    UINT32                  :   2; 
    UINT32        RGBOP2Sel          :   1; 
    UINT32                  :   7; 
LINT_SUPPRESS_NEXT_EXPRESSION(950)
} __attribute__ ((packed)) PMX_MIX_CTRL_FIELD_T;

typedef union _PMX_MIX_CTRL_UNION_T
{
    UINT32 u4Reg[PMX_SIZE_OF_MIX_CTRL_FIELD];
    PMX_MIX_CTRL_FIELD_T rField;
}PMX_MIX_CTRL_UNION_T;


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static UINT32 _u4UpdateEvent = 0;
static UINT32 _u4BgColor = 0;
static PMX_MIX_CTRL_UNION_T _rPmxMixCtrlReg;
//static UINT8 _au1PreAllHideState[PMX_PLANE_NUMBER];

static void _PMX_Set_ALPHA_MIX_VIDEO(const UINT32* pu4Alpha);

#if 0
static UINT32 _u4UpdateDisplayModeState = 0;
static BOOL _fgDisplayModeEnable = FALSE;
#endif

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define PLANE_TO_SRC(PLANE, SRC) \
    do \
    { \
        switch (PLANE) \
        { \
        case PMX_MAIN: SRC = PMX_SRC_MAIN; break; \
        case PMX_PIP : SRC = PMX_SRC_PIP; break; \
        case PMX_OSD1: SRC = PMX_SRC_OSD1; break; \
        case PMX_OSD2: SRC = PMX_SRC_OSD2; break; \
        case PMX_OSD3: SRC = PMX_SRC_OSD3; break; \
        default: SRC = PMX_MAX_INPORT_NS; break; \
        } \
    } \
    while (0)

// 3 depends on hardware bit allocation
#define ORDER_TO_GRP_IDX(PLANE_ORDER) \
    (((INT32)(PLANE_ORDER) > (INT32)PMX_TOP_PLANE_INDEX) ? \
    0 : ((INT32)PMX_TOP_PLANE_INDEX - (INT32)(PLANE_ORDER)))


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


static UINT32 _PmxRgb2Ybr(UINT32 u4RGB)
{
#ifdef DRV_SUPPORT_EXTMJC
    INT32 s4Y, s4Cb, s4Cr;
    INT32 s4R, s4G, s4B;

    s4R = (u4RGB >> 16) & 0xff;
    s4G = (u4RGB >> 8) & 0xff;
    s4B = u4RGB & 0xff;

    s4Y = (((s4R * 76) + (s4G * 149) + (s4B * 29)) / 255);
    s4Cb = (((128 * 255) + (s4B * 130)) - ((s4R * 43) + (s4G * 86)) ) / 255;
    s4Cr = (((128 * 255) + (s4R * 130)) - ((s4G * 109) + (s4B * 21))) / 255;

    if (s4Y > 255)
	{
		s4Y = 255;
	}

    if (s4Cb > 255)
	{
		s4Cb = 255;
	}

    if (s4Cr > 255)
	{
		s4Cr = 255;
	}

    if (s4Y < 0)
	{
		s4Y = 0;
	}

    if (s4Cb < 0)
	{
		s4Cb = 0;
	}

    if (s4Cr < 0)
	{
		s4Cr = 0;
	}
    //Printf("\nMute Value:%x",((UINT32)(s4Cb << 16) |(UINT32) (s4Y << 8) |(UINT32) (s4Cr)));

    return ((UINT32)(s4Cb << 16) |(UINT32) (s4Y << 8) |(UINT32) (s4Cr));
#else
    UINT32 u4Y, u4Cb, u4Cr;
    UINT32 u4R, u4G, u4B;

    u4R = (u4RGB >> 16) & 0xff;
    u4G = (u4RGB >> 8) & 0xff;
    u4B = u4RGB & 0xff;

    u4Y = ((((u4R * 76) + (u4G * 149) + (u4B * 29)) / 255)) & 0xFF;
    u4Cb = ((((128 * 255) + (u4B * 130)) - ((u4R * 43) + (u4G * 86)) ) / 255) & 0xFF;
    u4Cr = ((((128 * 255) + (u4R * 130)) - ((u4G * 109) + (u4B * 21))) / 255) & 0xFF;


    return ((u4Cb << 16) | (u4Y << 8) | (u4Cr));
#endif    
}


static void _PmxWriteHwRegister(void)
{
    #if defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
    vRegWriteFldAlign(OSTG_01, _rPmxMixCtrlReg.rField.MainTopEn, REG_MAIN_TOP_EN);
    #else
    IO_WRITE32MSK(POST_PROC_BASE, REG_OS_ALPHA_MIX1, _rPmxMixCtrlReg.u4Reg[0], 0xFFFFFF);
    #endif
}

#if defined(CC_FAST_INIT)
static UINT32 pmx_pm_save_os_mix;
void PMX_pm_suspend(void)
{
    #if defined(CC_MT5368)||defined(CC_MT5396)
    pmx_pm_save_os_mix = RegReadFldAlign(OSTG_01,REG_MAIN_TOP_EN);
    #else
	pmx_pm_save_os_mix=IO_READ32(POST_PROC_BASE, REG_OS_ALPHA_MIX1);
    #endif
}
void PMX_pm_resume(void)
{
    #if defined(CC_MT5368)||defined(CC_MT5396)
    vRegWriteFldAlign(OSTG_01, pmx_pm_save_os_mix, REG_MAIN_TOP_EN);
    #else
    IO_WRITE32(POST_PROC_BASE, REG_OS_ALPHA_MIX1, pmx_pm_save_os_mix);
    #endif
	//instead call _PmxWriteHwRegister() api
	// prevent  _PMX_Init not called, just use HW default value
}
#endif
//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void _PMX_Init(void)
{
    // reset alpha mix control
    _rPmxMixCtrlReg.u4Reg[0] = 0;
   // _rPmxMixCtrlReg.u4Reg[1] = 0;
   #if 0
    _rPmxMixCtrlReg.rField.arGroup[0].fgGrpSel = PMX_SRC_PIP;
    _rPmxMixCtrlReg.rField.arGroup[0].fgTopMixBg = 1;
    _rPmxMixCtrlReg.rField.arGroup[1].fgGrpSel = PMX_SRC_MAIN;
    _rPmxMixCtrlReg.rField.arGroup[1].fgTopMixBg = 1;

#ifdef CC_MT5360
    _rPmxMixCtrlReg.rField.arGroup[2].fgGrpSel = PMX_SRC_OSD2;
    _rPmxMixCtrlReg.rField.arGroup[2].fgTopMixBg = 1;
    _rPmxMixCtrlReg.rField.arGroup[3].fgGrpSel = PMX_SRC_OSD1;
    _rPmxMixCtrlReg.rField.arGroup[3].fgTopMixBg = 1;
#else
    _rPmxMixCtrlReg.rField.arGroup[2].fgGrpSel = PMX_SRC_OSD3;
    _rPmxMixCtrlReg.rField.arGroup[2].fgTopMixBg = 1;
    _rPmxMixCtrlReg.rField.arGroup[3].fgGrpSel = PMX_SRC_OSD2;
    _rPmxMixCtrlReg.rField.arGroup[3].fgTopMixBg = 1;
#endif

    // don't care field
    //_rPmxMixCtrlReg.rField.arGroup[4].fgGrpSel = PMX_SRC_OSD2;
    //_rPmxMixCtrlReg.rField.arGroup[4].fgTopMixBg = 1;
    _rPmxMixCtrlReg.rField.fgMainAlpha = 0xff;
    _rPmxMixCtrlReg.rField.fgPipAlpha = 0xff;
#endif
    _rPmxMixCtrlReg.rField.MainTopEn=0;
    _rPmxMixCtrlReg.rField.AlphaMixGrp1_0=1;
    _rPmxMixCtrlReg.rField.AlphaMixGrp1_1=0;
    _rPmxMixCtrlReg.rField.AlphaMixGrp1_5=0;
    _rPmxMixCtrlReg.rField.AlphaMixGrp2_0=1;
    _rPmxMixCtrlReg.rField.AlphaMixGrp2_1=0;
    _rPmxMixCtrlReg.rField.AlphaMixGrp2_5=0;
    
    _PmxWriteHwRegister();


#if !defined(CC_MTK_LOADER)
    if (!DRVCUST_GetDrvInit())
#endif /* CC_MTK_LOADER */
    {
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void _PMX_SetPlaneOrder(UCHAR ucPlaneId, UCHAR ucPlaneOrder)
{
	UNUSED(ucPlaneId);
	UNUSED(ucPlaneOrder);
#if 0
    UINT32 u4Src = 0;
    UINT32 u4GroupIdx = ORDER_TO_GRP_IDX(ucPlaneOrder);

    PLANE_TO_SRC(ucPlaneId, u4Src);
    if (ucPlaneOrder < PMX_PLANE_NUMBER)
    {
        _rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgGrpSel
        = u4Src;
        _rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgBypass = FALSE;
        _au1PreAllHideState[u4GroupIdx] = _rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgBypass;
    }

    _PmxWriteHwRegister();
#endif
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param pu4PlaneOrder: contains component ID
 *  @return
 */
//-----------------------------------------------------------------------------
void _PMX_SetPlaneOrderArray(const UINT32* pu4PlaneOrder)
{
	//UNUSED(pu4PlaneOrder);
//#ifdef CC_MT5363
#if defined(CC_MT5363) ||defined(CC_MT5365) || defined(CC_MT5395)
	UINT8 i,fgOSD2Top=0;
#elif defined(CC_MT5368)||defined(CC_MT5396)||defined(CC_MT5389)
    UINT8 i;
#else
	UINT8 i,fgOSD2Top=1;
#endif

      //set order of main, pip
	_rPmxMixCtrlReg.rField.MainTopEn=1;	
	for(i=0;i<PMX_PLANE_NUMBER;i++)
	{
		if(pu4PlaneOrder[i]==PMX_PIP)
		{
			for(i+=1;i<PMX_PLANE_NUMBER;i++) // keep searching to find out if PIP is on top of main
			{
				if(pu4PlaneOrder[i]==PMX_MAIN)
				{
					_rPmxMixCtrlReg.rField.MainTopEn=0;
					break;
				}
			}
			//_rPmxMixCtrlReg.rField.MainTopEn=0;
			break;
		}
	}
	_PmxWriteHwRegister();

//#ifdef CC_MT5363
#if defined(CC_MT5363) ||defined(CC_MT5365) || defined(CC_MT5395)
	for(i=0;i<PMX_PLANE_NUMBER;i++)
	{
		if(pu4PlaneOrder[i]==PMX_OSD2)
		{
			for(i+=1;i<PMX_PLANE_NUMBER;i++) // keep searching to find out if PIP is on top of main
			{
				if(pu4PlaneOrder[i]==PMX_OSD3)
				{
					fgOSD2Top=1;
					break;
				}
			}
			//_rPmxMixCtrlReg.rField.MainTopEn=0;
			break;
		}
	}
	if(fgOSD2Top==1)
	{
	    OSD_BASE_SetTopPlane((UINT32)OSD_PLANE_2);
	}
	else
	{
	    OSD_BASE_SetTopPlane((UINT32)OSD_PLANE_3);
	}
#elif defined(CC_MT5368) || defined(CC_MT5389)
	for(i=0;i<PMX_PLANE_NUMBER;i++)
	{
		if(pu4PlaneOrder[i]==PMX_OSD2)
		{ 
			OSD_BASE_SetPlaneSwitchOrder((UINT32)OSD_PLANE_1, (UINT32) OSD_PLANE_2, (UINT32) OSD_PLANE_3);
			break;
		}
		else if(pu4PlaneOrder[i]==PMX_OSD1)
		{
			for(i+=1;i<PMX_PLANE_NUMBER;i++) // keep searching to find out if PIP is on top of main
			{
				if(pu4PlaneOrder[i]==PMX_OSD3)
				{
					OSD_BASE_SetPlaneSwitchOrder((UINT32)OSD_PLANE_1, (UINT32) OSD_PLANE_3, (UINT32) OSD_PLANE_2);
					break;
				}
				else if(pu4PlaneOrder[i]==PMX_OSD2)
				{
					OSD_BASE_SetPlaneSwitchOrder((UINT32)OSD_PLANE_1, (UINT32) OSD_PLANE_2, (UINT32) OSD_PLANE_3);
					break;
				}
			}
			break;
		}
		else if(pu4PlaneOrder[i]==PMX_OSD3)
		{
			OSD_BASE_SetPlaneSwitchOrder((UINT32)OSD_PLANE_1, (UINT32) OSD_PLANE_3, (UINT32) OSD_PLANE_2);
			break;
		}
	} 
#elif defined(CC_MT5396)
	  for(i=0;i<PMX_PLANE_NUMBER;i++)
		  {
			  if(pu4PlaneOrder[i]==PMX_OSD2)
			  {
				  for(i+=1;i<PMX_PLANE_NUMBER;i++) // keep searching to find out if PIP is on top of main
				  {
					  if(pu4PlaneOrder[i]==PMX_OSD3)
					  {
						  OSD_BASE_SetPlaneSwitchOrder((UINT32)OSD_PLANE_2, (UINT32) OSD_PLANE_3, (UINT32) OSD_PLANE_1);
						  break;
					  }
					  else if(pu4PlaneOrder[i]==PMX_OSD1)
					  {
						  OSD_BASE_SetPlaneSwitchOrder((UINT32)OSD_PLANE_2, (UINT32) OSD_PLANE_1, (UINT32) OSD_PLANE_3);
						  break;
					  }
				  }
				  //_rPmxMixCtrlReg.rField.MainTopEn=0;
				  break;
			  }
			 else if(pu4PlaneOrder[i]==PMX_OSD1)
			  {
				  for(i+=1;i<PMX_PLANE_NUMBER;i++) // keep searching to find out if PIP is on top of main
				  {
					  if(pu4PlaneOrder[i]==PMX_OSD3)
					  {
						  OSD_BASE_SetPlaneSwitchOrder((UINT32)OSD_PLANE_1, (UINT32) OSD_PLANE_3, (UINT32) OSD_PLANE_2);
						  break;
					  }
					  else if(pu4PlaneOrder[i]==PMX_OSD2)
					  {
						  OSD_BASE_SetPlaneSwitchOrder((UINT32)OSD_PLANE_1, (UINT32) OSD_PLANE_2, (UINT32) OSD_PLANE_3);
						  break;
					  }
				  }
				  //_rPmxMixCtrlReg.rField.MainTopEn=0;
				  break;
			  }
			  else if(pu4PlaneOrder[i]==PMX_OSD3)
			  {
				  for(i+=1;i<PMX_PLANE_NUMBER;i++) // keep searching to find out if PIP is on top of main
				  {
					  if(pu4PlaneOrder[i]==PMX_OSD2)
					  {
						  OSD_BASE_SetPlaneSwitchOrder((UINT32)OSD_PLANE_3, (UINT32) OSD_PLANE_2, (UINT32) OSD_PLANE_1);
						  break;
					  }
					  else if(pu4PlaneOrder[i]==PMX_OSD1)
					  {
						  OSD_BASE_SetPlaneSwitchOrder((UINT32)OSD_PLANE_3, (UINT32) OSD_PLANE_1, (UINT32) OSD_PLANE_2);
						  break;
					  }
				  }
				  //_rPmxMixCtrlReg.rField.MainTopEn=0;
				  break;
			  }
		  } 

#else //mt5387
       //set order of OSD1,2
	for(i=0;i<PMX_PLANE_NUMBER;i++)
	{
		if(pu4PlaneOrder[i]==PMX_OSD1)
		{
			for(i+=1;i<PMX_PLANE_NUMBER;i++) // keep searching to find out if PIP is on top of main
			{
				if(pu4PlaneOrder[i]==PMX_OSD2)
				{
					fgOSD2Top=0;
					break;
				}
			}
			//_rPmxMixCtrlReg.rField.MainTopEn=0;
			break;
		}
	}
	if(fgOSD2Top==1)
	{
	    OSD_BASE_SetTopPlane((UINT32)OSD_PLANE_2);
	}
	else
	{
	    OSD_BASE_SetTopPlane((UINT32)OSD_PLANE_1);
	}
#endif
	#if 0
    UINT32 u4Src = 0, u4Order, u4GroupIdx;
    BOOL fgUpdate = FALSE, fgBypass;

    if (pu4PlaneOrder == NULL)
    {
        return;
    }

    for (u4Order = 0; u4Order < PMX_PLANE_NUMBER; u4Order++)
    {
        u4GroupIdx = ORDER_TO_GRP_IDX(u4Order);
        PLANE_TO_SRC(pu4PlaneOrder[u4Order], u4Src);
        if (_rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgGrpSel != u4Src)
        {
            fgUpdate = TRUE;
        }
        _rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgGrpSel = u4Src;

        fgBypass = (u4Src < PMX_MAX_INPORT_NS)?FALSE:TRUE;
        if (_rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgBypass != fgBypass)
        {
            fgUpdate = TRUE;
        }
        _rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgBypass = fgBypass;
        _au1PreAllHideState[u4GroupIdx] = _rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgBypass;
    }

    if (fgUpdate)
    {
        _u4UpdateEvent |= PMX_UPDATE_PLANE_ORDER;
    }
    #endif
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void _PMX_SetAlpha(UCHAR ucPlaneId, UCHAR ucValue)
{
	#if 0
    if (ucPlaneId == PMX_MAIN) // main
    {
        _rPmxMixCtrlReg.rField.fgMainAlpha = ucValue;
    }
    else if (ucPlaneId == PMX_PIP) // pip
    {
        _rPmxMixCtrlReg.rField.fgPipAlpha = ucValue;
    }
    else
    {
        return;
    }
    _PmxWriteHwRegister();
    #endif
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void _PMX_Set_ALPHA_MIX_GRP1_0(BOOL fgEnable)
{

	#if defined(CC_MT5368) ||defined(CC_MT5396)||defined(CC_MT5389)
	if(fgEnable)
	{
		vRegWriteFldAlign(OSD_MIX_0,1,ALPHA_MIX_GRP1_0);
	}
	else
	{
		vRegWriteFldAlign(OSD_MIX_0,0,ALPHA_MIX_GRP1_0);
	}
	#endif

	return ;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
static void _PMX_Set_ALPHA_MIX_VIDEO(const UINT32* pu4Alpha)
{
    vRegWriteFldAlign(OSD_MIX_1,(UCHAR)pu4Alpha[OSD_MIX_AL_11],REG_MIX11_BOT_ALPHA);
    #if defined(CC_MT5396)
    vRegWriteFldAlign(OSD_MIX_1,(UCHAR)pu4Alpha[OSD_MIX_AL_12],REG_MIX12_BOT_ALPHA);
    #endif
    vRegWriteFldAlign(OSD_MIX_1,(UCHAR)pu4Alpha[OSD_MIX_AL_2],REG_MIX2_BOT_ALPHA);

    return;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void _PMX_Set_ALPHA_MIX_VIDEO_BOT(const UINT32* pu4Alpha)
{
    vRegWriteFldAlign(OSD_MIX_1,0,REG_MIX11_SEL_BOT_ALPHA);
    #if defined(CC_MT5396)
    vRegWriteFldAlign(OSD_MIX_1,0,REG_MIX12_SEL_BOT_ALPHA);
    #endif
    vRegWriteFldAlign(OSD_MIX_1,0,REG_MIX2_SEL_BOT_ALPHA);

    _PMX_Set_ALPHA_MIX_VIDEO(pu4Alpha);
    
    return ;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void _PMX_Set_ALPHA_MIX_VIDEO_1_OSD_BELOW(const UINT32* pu4Alpha)
{
    _PMX_Set_ALPHA_MIX_VIDEO(pu4Alpha);
	
    vRegWriteFldAlign(OSD_MIX_1,1,REG_MIX11_SEL_BOT_ALPHA);
    #if defined(CC_MT5396)
    vRegWriteFldAlign(OSD_MIX_1,0,REG_MIX12_SEL_BOT_ALPHA);
    #endif
    vRegWriteFldAlign(OSD_MIX_1,0,REG_MIX2_SEL_BOT_ALPHA);
    
    return ;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void _PMX_Set_ALPHA_MIX_VIDEO_2_OSD_BELOW(const UINT32* pu4Alpha)
{
	_PMX_Set_ALPHA_MIX_VIDEO(pu4Alpha);
	
    vRegWriteFldAlign(OSD_MIX_1,1,REG_MIX11_SEL_BOT_ALPHA);
    #if defined(CC_MT5396)
    vRegWriteFldAlign(OSD_MIX_1,1,REG_MIX12_SEL_BOT_ALPHA);
    #endif
    vRegWriteFldAlign(OSD_MIX_1,0,REG_MIX2_SEL_BOT_ALPHA);
    
    return ;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void _PMX_Set_ALPHA_MIX_VIDEO_TOP(const UINT32* pu4Alpha)
{
	_PMX_Set_ALPHA_MIX_VIDEO(pu4Alpha);
	
    vRegWriteFldAlign(OSD_MIX_1,1,REG_MIX11_SEL_BOT_ALPHA);
    #if defined(CC_MT5396)
    vRegWriteFldAlign(OSD_MIX_1,1,REG_MIX12_SEL_BOT_ALPHA);
    #endif
    vRegWriteFldAlign(OSD_MIX_1,1,REG_MIX2_SEL_BOT_ALPHA);
    
    return ;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
void _PMX_SetBgColor(UINT32 u4Value, BOOL fgYuvMode, BOOL fgInVsync)
{
    if (fgYuvMode)
    {
        _u4BgColor = _PmxRgb2Ybr(u4Value);
    }
    else
    {
        UINT32 r, g, b;
        r = (u4Value & 0xff0000) >> 16;
        g = (u4Value & 0xff00) >> 8;
        b = (u4Value & 0xff);
        _u4BgColor = ((b << 16) | (g << 8) | r);
    }

    if (fgInVsync)
    {
        _u4UpdateEvent |= PMX_UPDATE_BG_COLOR;
    }
    else
    {

	    #if defined(CC_MT5368) ||defined(CC_MT5396)
		vRegWriteFldAlign(MUTE_00,_u4BgColor,BGR_BACKGROUND);
		#else		
        IO_WRITE32MSK(POST_PROC_BASE, REG_OS_BGCOLOR, _u4BgColor, REG_OS_BGCOLOR_MSK);
		#endif
		
    }
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void PMX_OnOutputVSync(void)
{
	static UINT32 i4VsyncDelay = PMX_UPDATE_DELAY;
	
	if (_u4UpdateEvent & PMX_UPDATE_PLANE_ORDER)
    	{
       	if (i4VsyncDelay == 0)
        	{
            		_PmxWriteHwRegister();
            		_u4UpdateEvent &= ~PMX_UPDATE_PLANE_ORDER;
            		i4VsyncDelay = PMX_UPDATE_DELAY;
        	}
	}
	if (_u4UpdateEvent & PMX_UPDATE_BG_COLOR)
    	{
        	IO_WRITE32MSK(POST_PROC_BASE, REG_OS_BGCOLOR, _u4BgColor, REG_OS_BGCOLOR_MSK);
       	 _u4UpdateEvent &= ~PMX_UPDATE_BG_COLOR;
    	}
	#if 0
    static UINT32 i4VsyncDelay = PMX_UPDATE_DELAY;
    UINT32 u4Src,u4Bypass;
    INT32 i4Ret;

    if (_u4UpdateEvent & PMX_UPDATE_PLANE_ORDER)
    {
        if (i4VsyncDelay == 0)
        {
            _PmxWriteHwRegister();
#if 0
            u4Src = _rPmxMixCtrlReg.rField.arGroup[PMX_TOP_PLANE_INDEX].fgGrpSel;
            if (u4Src == PMX_SRC_OSD2)
            {
                i4Ret = OSD_BASE_SetTopPlane((UINT32)OSD_PLANE_2);
            }
            else if (u4Src == PMX_SRC_OSD1)
            {
                i4Ret = OSD_BASE_SetTopPlane((UINT32)OSD_PLANE_1);
            }
            else if (u4Src == PMX_SRC_OSD3)
            {
                i4Ret = OSD_BASE_SetTopPlane((UINT32)OSD_PLANE_3);
            }
            else
            {
                i4Ret = OSD_BASE_SetTopPlane((UINT32)OSD_PLANE_NONE);
            }
            (void)i4Ret;
#endif

#ifdef CC_MT5360
	#ifdef CC_MT5360B
		//UNUSED(u4Src);
            //UNUSED(i4Ret);
            for (i4Ret = 0; i4Ret < PMX_PLANE_NUMBER; i4Ret++)
            {
                u4Src = _rPmxMixCtrlReg.rField.arGroup[ORDER_TO_GRP_IDX(i4Ret)].fgGrpSel;
                u4Bypass = _rPmxMixCtrlReg.rField.arGroup[ORDER_TO_GRP_IDX(i4Ret)].fgBypass;
            #if !defined(CC_MTK_LOADER)
                if ( (u4Src == PMX_SRC_PIP)&&(!u4Bypass) )
                {
                     _VDP_NotifyPlaneOrderChanged(1, ORDER_TO_GRP_IDX(i4Ret));
                }
                else if ( (u4Src == PMX_SRC_MAIN)&&(!u4Bypass) )
                {
                    _VDP_NotifyPlaneOrderChanged(0, ORDER_TO_GRP_IDX(i4Ret));
                }
            #else
 		  UNUSED(u4Bypass); //for warning free
            #endif /* !defined(CC_MTK_LOADER) */
            }
	#else
            for (i4Ret = 0; i4Ret < PMX_PLANE_NUMBER; i4Ret++)
            {
                u4Src = _rPmxMixCtrlReg.rField.arGroup[ORDER_TO_GRP_IDX(i4Ret)].fgGrpSel;
                u4Bypass = _rPmxMixCtrlReg.rField.arGroup[ORDER_TO_GRP_IDX(i4Ret)].fgBypass;
                if (u4Src == PMX_SRC_OSD2)
                {
                    OSD_BASE_SetPlaneOrder(OSD_PLANE_2, i4Ret);
                }
                else if (u4Src == PMX_SRC_OSD1)
                {
                    OSD_BASE_SetPlaneOrder(OSD_PLANE_1, i4Ret);
                }
                else if (u4Src == PMX_SRC_OSD3)
                {
                    OSD_BASE_SetPlaneOrder(OSD_PLANE_3, i4Ret);
                }
           #if !defined(CC_MTK_LOADER)
                else if ( (u4Src == PMX_SRC_PIP)&&(!u4Bypass) )
                {
                     _VDP_NotifyPlaneOrderChanged(1,ORDER_TO_GRP_IDX(i4Ret));
                }
                else if ( (u4Src == PMX_SRC_MAIN)&&(!u4Bypass) )
                {
                    _VDP_NotifyPlaneOrderChanged(0,ORDER_TO_GRP_IDX(i4Ret));
                }
           #else
 		  UNUSED(u4Bypass); //for warning free
           #endif /* !defined(CC_MTK_LOADER) */
            }
  #endif
#else
            UNUSED(u4Src);
            UNUSED(i4Ret);
            UNUSED(u4Bypass);
#endif
            _u4UpdateEvent &= ~PMX_UPDATE_PLANE_ORDER;
            i4VsyncDelay = PMX_UPDATE_DELAY;
        }
        i4VsyncDelay--;
    }
    if (_u4UpdateEvent & PMX_UPDATE_BG_COLOR)
    {
        IO_WRITE32MSK(POST_PROC_BASE, REG_OS_BGCOLOR, _u4BgColor, REG_OS_BGCOLOR_MSK);
        _u4UpdateEvent &= ~PMX_UPDATE_BG_COLOR;
    }
    #endif
}

void PMX_ShowPattern(BOOL fgEnable)
{
    if (fgEnable)
    {
        IO_WRITE32MSK(POST_PROC_BASE, REG_OS_PATTERN, REG_OS_PATTERN_MSK, REG_OS_PATTERN_MSK);
    }
    else
    {
        IO_WRITE32MSK(POST_PROC_BASE, REG_OS_PATTERN, 0, REG_OS_PATTERN_MSK);
    }
}

void PMX_SetFastBlank(BOOL fgEnable)
{
    if (fgEnable)
    {
        IO_WRITE32MSK(POST_PROC_BASE, REG_OS_PATTERN, REG_OS_FASTBLANK_MSK, REG_OS_FASTBLANK_MSK);
    }
    else
    {
        IO_WRITE32MSK(POST_PROC_BASE, REG_OS_PATTERN, 0, REG_OS_FASTBLANK_MSK);
    }
}

void PMX_SetPlaneHide(UCHAR ucPlaneId,BOOL fgHide)
{
	#if 0
    UINT32 u4Src;
    UINT32 u4GroupIdx;

    PLANE_TO_SRC(ucPlaneId, u4Src);
    if(u4Src < PMX_PLANE_NUMBER)
    {
    	for(u4GroupIdx=0;u4GroupIdx<PMX_PLANE_NUMBER;u4GroupIdx++)
    	{
    	   if(_rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgGrpSel== u4Src)
    	   {
    	       if(fgHide)
    	       {
    	           _rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgBypass = TRUE;
    	       }
    	       else
    	       {
    	           _rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgBypass = FALSE;
    	       }
    	       _PmxWriteHwRegister();
    	       break;
    	   }
    	}
    }
    #endif
}

void PMX_GetPlaneHide(UCHAR ucPlaneId,BOOL* pHide)
{
	#if 0
    UINT32 u4Src;
    UINT32 u4GroupIdx;

    PLANE_TO_SRC(ucPlaneId, u4Src);
    if(u4Src < PMX_PLANE_NUMBER)
    {
    	for(u4GroupIdx=0;u4GroupIdx<PMX_PLANE_NUMBER;u4GroupIdx++)
    	{
    	   if(_rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgGrpSel== u4Src)
    	   {
    	   	*pHide = _rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgBypass;
    	       break;
    	   }
    	}
    }
    #endif
}

void PMX_SetAllPlaneHide(BOOL fgHide)
{
	#if 0
    UINT32 u4GroupIdx;

    if(fgHide) //Hide all planes
    {
        for(u4GroupIdx=0;u4GroupIdx<PMX_PLANE_NUMBER;u4GroupIdx++)
    	{
            _au1PreAllHideState[u4GroupIdx] = _rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgBypass;
            _rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgBypass = TRUE;
    	}
        _PmxWriteHwRegister();
    }
    else //Unhide all planes that were originally unhidden
    {
    	 for(u4GroupIdx=0;u4GroupIdx<PMX_PLANE_NUMBER;u4GroupIdx++)
    	{
            _rPmxMixCtrlReg.rField.arGroup[u4GroupIdx].fgBypass = _au1PreAllHideState[u4GroupIdx];
    	}
        _PmxWriteHwRegister();
    }
	#endif
}

