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
 * $RCSfile: osd_base_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file osd_base_if.c
 *  osd base header file, includes general osd setting
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "osd_hw.h"
#include "osd_drvif.h"
#ifndef CC_MTK_LOADER
#include "drvcust_if.h"
//#include "ostg_if.h"
#include "x_os.h"
#endif
LINT_EXT_HEADER_BEGIN
#include "x_hal_5381.h"
#include "panel.h"
LINT_EXT_HEADER_END

#ifdef CC_FLIP_MIRROR_SUPPORT
#include "eeprom_if.h"
#endif

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
#ifdef CC_FLIP_MIRROR_SUPPORT
BOOL gfgOSD_Mirror=FALSE;
BOOL gfgOSD_Flip=FALSE;
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static UINT32 _u4Clock = OSD_CK_XTAL;
static BOOL _fgYuvOutput = FALSE;
#ifdef CC_MT5363
//static BOOL gfgSwap=FALSE;
BOOL gfgSwap=FALSE;
#endif
#ifndef CC_MTK_LOADER
static BOOL fgLogoInit=FALSE;
static HANDLE_T _hOsdWaitVsyncSema;
static UINT32 _u4FlipCnt = 0;
#endif

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static INT32 _OsdBaseInit(void)
{
    IGNORE_RET(_OSD_BASE_SetVsWidth(1));
    IGNORE_RET(_OSD_BASE_SetOsd1HStart(0));
    IGNORE_RET(_OSD_BASE_SetOsd2HStart(0));
    //IGNORE_RET(_OSD_BASE_SetOsd3HStart(0));
    IGNORE_RET(_OSD_BASE_SetOsd1VStart(0));
    IGNORE_RET(_OSD_BASE_SetOsd2VStart(0));
    #ifdef CC_MT5363
    IGNORE_RET(_OSD_BASE_SetOsd3HStart(0));
    IGNORE_RET(_OSD_BASE_SetOsd3VStart(0));
    #endif

    return (INT32)OSD_RET_OK;
}

#ifdef CC_FLIP_MIRROR_SUPPORT
#include "drv_display.h"
static void _OsdGetMirrorFlipSetting(void)
{

	UINT8 u1Config;
	u1Config= u1GetFlipMirrorConfig(); 


    //if(EEPDTV_GetCfg(&rDtvCfg))                                                                                                                                   
   //{
   //     Printf(" osd Get DTV config fail\n");
   // }
    if(u1Config & SYS_FLIP_CONFIG_ON)
    {
        gfgOSD_Flip=TRUE;
        Printf(" osd flip en\n");
    }
    if(u1Config & SYS_MIRROR_CONFIG_ON)
    {
        gfgOSD_Mirror=TRUE;
        Printf(" osd mirror en\n");                                                                                                                              
     }	
    return;
}
#endif

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

INT32 OSD_BASE_SetClock(UINT32 u4Clock)
{
#if 0 // def CC_MT5360B
    IO_WRITE32(OSD_CKEN_REG, 0, ((1U << 4) | IO_READ32(OSD_CKEN_REG, 0)));
#endif /* CC_MT5360B */
    if (_u4Clock != u4Clock)
    {
        _u4Clock = u4Clock;
        IO_WRITE32(OSD_CK_REG, 0, u4Clock);
    }
    return (INT32)OSD_RET_OK;
}


void OSD_WaitVsync(void)
{
#if !defined(CC_MTK_LOADER)    
    _u4FlipCnt++;
    
    VERIFY(x_sema_lock(_hOsdWaitVsyncSema, X_SEMA_OPTION_WAIT) ==
               OSR_OK);
#endif               
}


void OSD_UnlockVsyncSema(void)
{
#if !defined(CC_MTK_LOADER)    
    if (_u4FlipCnt > 0)
    {
    	VERIFY(x_sema_unlock(_hOsdWaitVsyncSema) == OSR_OK);
    	
    	_u4FlipCnt --;
    }
#endif    
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

INT32 OSD_BASE_SetYuvOutput(BOOL fgYuvOutput)
{
    _fgYuvOutput = fgYuvOutput;
    IGNORE_RET(_OSD_BASE_SetYuvOut(fgYuvOutput));
    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
    return (INT32)OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** osd global initialization function
 *  @param  void
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_Init(void)
{
    static UINT32 _fgOsdInit = 0;

    if (_fgOsdInit == 0)
    {
        IGNORE_RET(OSD_Reset());

#if !defined(CC_MTK_LOADER)
        // Create OSD Flip Semphore
        if (x_sema_create
            (&_hOsdWaitVsyncSema, X_SEMA_TYPE_BINARY,
             X_SEMA_STATE_LOCK) != OSR_OK)
        {
            ASSERT(0);
        }
#endif          

        #ifdef CC_FLIP_MIRROR_SUPPORT
        _OsdGetMirrorFlipSetting();
        #endif
        _fgOsdInit = 1;
    }

    return (INT32)OSD_RET_OK;
}


INT32 OSD_Reset(void)
{
    // to choose osd clock as oclk
    IGNORE_RET(OSD_BASE_SetClock(OSD_CK_OCLK));

    // reset the whole OSD
    IGNORE_RET(_OSD_BASE_SetReg(NULL));

    IGNORE_RET(_OsdBaseInit());
    IGNORE_RET(_OSD_BASE_SetYuvOut(_fgYuvOutput));
    IGNORE_RET(OSD_UpdateTiming());
    // adjust scaler to fit new resolution and also reset LPF
    // defaule enable scaler for better bandwidth utilization
    IGNORE_RET(OSD_SC_Scale((UINT32)OSD_SCALER_2, TRUE, 0, 0, 0, 0));
    //IGNORE_RET(OSD_SC_SetLpf((UINT32)OSD_SCALER_2, FALSE));
    #ifdef CC_MT5363
    IGNORE_RET(OSD_SC_Scale((UINT32)OSD_SCALER_3, TRUE, 0, 0, 0, 0));
    #endif
    //IGNORE_RET(OSD_SC_SetLpf((UINT32)OSD_SCALER_3, FALSE));

    _OSD_RGN_InitApi();
    IGNORE_RET(OSD_PLA_Init((UINT32)OSD_PLANE_1));
    IGNORE_RET(OSD_PLA_Init((UINT32)OSD_PLANE_2));
    #ifdef CC_MT5363
    IGNORE_RET(OSD_PLA_Init((UINT32)OSD_PLANE_3));
    #endif
    IGNORE_RET(OSD_RGN_LIST_Init());
    IGNORE_RET(OSD_RGN_Init());

    IO_WRITE32(OSD_INT_ENABLE_REG, 0, OSD1_CFUD_IEN |
               OSD2_CFUD_IEN | OSD2_PFUD_IEN | OSD2_VUTO_IEN | OSD2_VDTO_IEN |
               OSD3_CFUD_IEN | OSD3_PFUD_IEN | OSD3_VUTO_IEN | OSD3_VDTO_IEN);

    #ifdef DRV_SUPPORT_EXTMJC
    OSD_DMA_Init();
    #endif


    // update 3x3 matrix for RGB-to-YUV (OSD2)
    /*
    IO_WRITE32(OSD_BASE, 0x260, 0x00420081);
    IO_WRITE32(OSD_BASE, 0x264, 0x00190010);
    IO_WRITE32(OSD_BASE, 0x268, 0x1026104a);
    IO_WRITE32(OSD_BASE, 0x26c, 0x00700080);
    IO_WRITE32(OSD_BASE, 0x270, 0x0070105e);
    IO_WRITE32(OSD_BASE, 0x274, 0x10120080);
	*/
    #ifdef CC_MT5363
    OSD_SetTVEOut(1);
    #endif

    return (INT32)OSD_RET_OK;
}


INT32 OSD_UpdateTiming(void)
{
#ifdef CC_MT5391_FPGA
    IGNORE_RET(_OSD_BASE_SetScrnHSize(720));
    IGNORE_RET(_OSD_BASE_SetScrnVSize(480));
    // osd2 hstart=0xb, osd vstart=0x5
    IO_WRITE32MSK(OSD_BASE, 0x2c, 0x350b0500, 0xffffff00);
    // osd3 hstart=0x1b
    IO_WRITE32MSK(OSD_BASE, 0x38, 0x091d, 0xffff);
#else
    IGNORE_RET(_OSD_BASE_SetScrnHSize(PANEL_GetPanelWidth()));
    IGNORE_RET(_OSD_BASE_SetScrnVSize(PANEL_GetPanelHeight()));
    #ifdef CC_MT5363
    IO_WRITE32MSK(OSD_BASE, 0x2c, 0x0c0410, 0x00ffff00);
    IO_WRITE32MSK(OSD_BASE, 0x38, 0x090c, 0xffff);
    #else
    	#ifdef CC_MT5301A
    	//IO_WRITE32MSK(OSD_BASE, 0x2c, 0x251a0310, 0xffffff00);
    	IO_WRITE32MSK(OSD_BASE, 0x2c, 0x341a0310, 0xffffff00);
    	#else
    	IO_WRITE32MSK(OSD_BASE, 0x2c, 0x250b0410, 0xffffff00);
    	#endif
    #endif
    //IO_WRITE32MSK(OSD_BASE, 0x38, 0x091d, 0xffff);
#endif

    // default OSD2 is top plane
    //OSD_BASE_SetTopPlane((UINT32)OSD_PLANE_2);

    // write hardware
    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** set whether osd1/osd2 is top plane
 *  @param  u4TopPlane OSD_PLANE_1/OSD_PLANE_2/OSD_PLANE_NONE(video plane is on top)
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_BASE_SetTopPlane(UINT32 u4TopPlane)
{
    // this function should be call in vsync, and update HW immediately
    //UNUSED(u4TopPlane);

#ifdef CC_MT5363
    UINT32 u4X_tmp1,u4Y_tmp1,u4X_tmp2,u4Y_tmp2;
    if(u4TopPlane == (UINT32)OSD_PLANE_2)
    {
    	if(gfgSwap==TRUE)
    	{
    		return (INT32)OSD_RET_OK;
    	}
    	 OSD_PLA_Swap(1);
        OSD_SC_Swap(1);
        OSD_BASE_GetOsdPosition(OSD_PLANE_2,&u4X_tmp1,&u4Y_tmp1);
        OSD_BASE_GetOsdPosition(OSD_PLANE_3,&u4X_tmp2,&u4Y_tmp2);
        OSD_BASE_SetOsdPosition(OSD_PLANE_2,u4X_tmp2,u4Y_tmp2);
        OSD_BASE_SetOsdPosition(OSD_PLANE_3,u4X_tmp1,u4Y_tmp1);
	 gfgSwap=TRUE;
    }
    else	//OSD_PLANE_3 Top
    {
    	 if(gfgSwap==FALSE)
    	{
    		return (INT32)OSD_RET_OK;
    	}
    	 OSD_PLA_Swap(0);
        OSD_SC_Swap(0);
        OSD_BASE_GetOsdPosition(OSD_PLANE_2,&u4X_tmp1,&u4Y_tmp1);
        OSD_BASE_GetOsdPosition(OSD_PLANE_3,&u4X_tmp2,&u4Y_tmp2);
        OSD_BASE_SetOsdPosition(OSD_PLANE_2,u4X_tmp2,u4Y_tmp2);
        OSD_BASE_SetOsdPosition(OSD_PLANE_3,u4X_tmp1,u4Y_tmp1);
        gfgSwap=FALSE;
    }
#else
    if(u4TopPlane == (UINT32)OSD_PLANE_1)
    {
        _OSD_BASE_SetOSD12Ex(1);
    }
    else
    {
        _OSD_BASE_SetOSD12Ex(0);
    }
    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
 #endif
    return (INT32)OSD_RET_OK;

#if 0
    _OSD_AlwaysUpdateReg(TRUE);

    if (u4TopPlane == (UINT32)OSD_PLANE_NONE)
    {
        IGNORE_RET(_OSD_BASE_SetDelayAdj(0));
        IO_WRITE32MSK(OSD_BASE, 0x2c, 0x20<<16, 0x00ff0000);	//set OSD2 H auto-position not top
        IO_WRITE32MSK(OSD_BASE, 0x38, 0x1D, 0x000000ff);	//set OSD3 H auto-position not top
    }
    else if (u4TopPlane == (UINT32)OSD_PLANE_1)
    {
        IGNORE_RET(_OSD_BASE_SetDelayAdj(1));
        IO_WRITE32MSK(OSD_BASE, 0x2c, 0x20<<16, 0x00ff0000);	//set OSD2 H auto-position not top
        IO_WRITE32MSK(OSD_BASE, 0x38, 0x1D, 0x000000ff);	//set OSD3 H auto-position not top
    }
    else if (u4TopPlane == (UINT32)OSD_PLANE_2)
    {
        IGNORE_RET(_OSD_BASE_SetDelayAdj(2));
        IO_WRITE32MSK(OSD_BASE, 0x2c, 0x1E<<16, 0x00ff0000);	//set OSD2 H auto-position top
        IO_WRITE32MSK(OSD_BASE, 0x38, 0x1D, 0x000000ff);	//set OSD3 H auto-position not top
    }
    else if (u4TopPlane == (UINT32)OSD_PLANE_3)
    {
        IGNORE_RET(_OSD_BASE_SetDelayAdj(4));
        IO_WRITE32MSK(OSD_BASE, 0x2c, 0x20<<16, 0x00ff0000);	//set OSD2 H auto-position not top
        IO_WRITE32MSK(OSD_BASE, 0x38, 0x1B, 0x000000ff);	//set OSD3 H auto-position top
    }
    else
    {
        return (INT32)OSD_RET_INV_ARG;
    }

    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());
    _OSD_AlwaysUpdateReg(FALSE);
    return (INT32)OSD_RET_OK;
    
#endif

}


INT32 OSD_BASE_SetDisplayMode(UINT32 u4DisplayMode)
{
    if (BSP_IsFPGA())
    {
        OSD_VERIFY_DISPLAY_MODE(u4DisplayMode);

        if (u4DisplayMode == (UINT32)OSD_DM_480P) // 2
        {
            // 720x480
            IO_WRITE32MSK(SCPOS_BASE, 0x24, ((UINT32)0x359 << 20) | 0x20c,
                          0xfff007ff);
            PANEL_SetPanelWidth(720);
            PANEL_SetPanelHeight(480);
        }
        else if (u4DisplayMode == (UINT32)OSD_DM_1080P) // 7
        {
            // 1920x1080
            IO_WRITE32MSK(SCPOS_BASE, 0x24, ((UINT32)0x897 << 20) | 0x464,
                          0xfff007ff);
            PANEL_SetPanelWidth(1920);
            PANEL_SetPanelHeight(1080);
        }
    }

    UNUSED(u4DisplayMode);
    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** set osd plane position
 *  @param  u4Plane plane id
 *  @param  u4X x position
 *  @param  u4Y y position
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_BASE_SetOsdPosition(UINT32 u4Plane, UINT32 u4X, UINT32 u4Y)
{

    OSD_VERIFY_PLANE(u4Plane);

    #ifdef CC_MT5363
    if(gfgSwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif

    if (u4Plane == (UINT32)OSD_PLANE_1)
    {
        IGNORE_RET(_OSD_BASE_SetOsd1HStart(u4X));
        IGNORE_RET(_OSD_BASE_SetOsd1VStart(u4Y));
    }
    else if (u4Plane == (UINT32)OSD_PLANE_2)
    {
        IGNORE_RET(_OSD_BASE_SetOsd2HStart(u4X));
        IGNORE_RET(_OSD_BASE_SetOsd2VStart(u4Y));
        //OSD_SC_UpdateDstSize((UINT32)OSD_SCALER_2);
    }
    #ifdef CC_MT5363
    else if (u4Plane == (UINT32)OSD_PLANE_3)
    {
        IGNORE_RET(_OSD_BASE_SetOsd3HStart(u4X));
        IGNORE_RET(_OSD_BASE_SetOsd3VStart(u4Y));
        //OSD_SC_UpdateDstSize((UINT32)OSD_SCALER_3);
    }
    #endif

    VERIFY((INT32)OSD_RET_OK == _OSD_BASE_UpdateHwReg());

    return (INT32)OSD_RET_OK;
}


//-----------------------------------------------------------------------------
/** get osd plane position
 *  @param  u4Plane plane id
 *  @param  pu4X x position pointer
 *  @param  pu4Y y position pointer
 *  @retval OSD_RET_OK success
 */
//-----------------------------------------------------------------------------
INT32 OSD_BASE_GetOsdPosition(UINT32 u4Plane, UINT32* pu4X, UINT32* pu4Y)
{
    OSD_VERIFY_PLANE(u4Plane);

    #ifdef CC_MT5363
    if(gfgSwap)
    {
    	if(u4Plane==OSD_PLANE_2)
    		u4Plane=OSD_PLANE_3;
    	else if(u4Plane==OSD_PLANE_3)
    		u4Plane=OSD_PLANE_2;
    }
    #endif

    if (u4Plane == (UINT32)OSD_PLANE_1)
    {
        IGNORE_RET(_OSD_BASE_GetOsd1HStart(pu4X));
        IGNORE_RET(_OSD_BASE_GetOsd1VStart(pu4Y));
    }
    else if (u4Plane == (UINT32)OSD_PLANE_2)
    {
        IGNORE_RET(_OSD_BASE_GetOsd2HStart(pu4X));
        IGNORE_RET(_OSD_BASE_GetOsd2VStart(pu4Y));
    }
    #ifdef CC_MT5363
    else if (u4Plane == (UINT32)OSD_PLANE_3)
    {
        IGNORE_RET(_OSD_BASE_GetOsd3HStart(pu4X));
        IGNORE_RET(_OSD_BASE_GetOsd3VStart(pu4Y));
    }
    #endif
    return (INT32)OSD_RET_OK;
}

//-----------------------------------------------------------------------------
/** dump osd debug info
 *  @param  void
 *  @return void
 */
//-----------------------------------------------------------------------------
void OSD_BASE_Dump(void)
{
    UINT32 u4IntStatus;
    u4IntStatus = IO_READ32(OSD_INT_STATUS_REG, 0);

    Printf("OSD INTERRUPT=");
    if (u4IntStatus == 0)
    {
        Printf("none\n");
    }
    else
    {
        if ((u4IntStatus & OSD_INT) > 0)
        {
            Printf("(OSD_INT)");
        }
        if ((u4IntStatus & OSD1_CFUD_IID) > 0)
        {
            Printf("(OSD1_CFUD_IID)");
        }
        if ((u4IntStatus & OSD2_CFUD_IID) > 0)
        {
            Printf("(OSD2_CFUD_IID)");
        }
        if ((u4IntStatus & OSD2_PFUD_IID) > 0)
        {
            Printf("(OSD2_PFUD_IID)");
        }
        if ((u4IntStatus & OSD2_VUTO_IID) > 0)
        {
            Printf("(OSD2_VUTO_IID)");
        }
        if ((u4IntStatus & OSD2_VDTO_IID) > 0)
        {
            Printf("(OSD2_VDTO_IID)");
        }
        if ((u4IntStatus & OSD3_CFUD_IID) > 0)
        {
            Printf("(OSD3_CFUD_IID)");
        }
        if ((u4IntStatus & OSD3_PFUD_IID) > 0)
        {
            Printf("(OSD3_PFUD_IID)");
        }
        if ((u4IntStatus & OSD3_VUTO_IID) > 0)
        {
            Printf("(OSD3_VUTO_IID)");
        }
        if ((u4IntStatus & OSD3_VDTO_IID) > 0)
        {
            Printf("(OSD3_VDTO_IID)");
        }
        Printf("\n");
        IO_WRITE32(OSD_INT_STATUS_REG, 0, 0);
    }
}

//Set 5363 SD TVE output config

INT32 OSD_SetTVEOut(BOOL fgEnable)
{
	Printf("OSD_SetTVEOut: %x\n",fgEnable);
	if(fgEnable)
	{
		//IO_WRITE32(OSD_BASE, 0x8, 0x40128);
		IO_WRITE32(OSD_BASE, 0xc0, 0xa);
		IO_WRITE32(OSD_BASE, 0x10, 0x20766610);
		IO_WRITE32(OSD_BASE, 0,8);
		IO_WRITE32(OSD_BASE, 0x100,0);
		IO_WRITE32(OSD_BASE, 0x4,2);
		IO_WRITE32(OSD_BASE, 0x4,3);
		IO_WRITE32(OSD_BASE, 0,0);
		IO_WRITE32(OSD_BASE, 0x100,1);
		IO_WRITE32(OSD_BASE, 0,8);
		IO_WRITE32(OSD_BASE, 0xc8, 0x03600271);
		IO_WRITE32(OSD_BASE, 0xcc, 0x03600271);
		//IO_WRITE32(OSD_BASE, 0x20, 0x001e000c);
		//IO_WRITE32(OSD_BASE, 0x20, 0x0019002b);
		IGNORE_RET(_OSD_BASE_SetOsd1HStart(0x19));
              IGNORE_RET(_OSD_BASE_SetOsd1VStart(0x2b));
              _OSD_BASE_UpdateHwReg();
		//IO_WRITE32(OSD_BASE, 0,8);
	}
	else
	{
		//IO_WRITE32MSK(OSD_BASE, 0x8, 0, 0x40128);
		IO_WRITE32MSK(OSD_BASE, 0xc0, 0, 0xa);
		IO_WRITE32(OSD_BASE, 0x100,0);
		IO_WRITE32MSK(OSD_BASE, 0xc8, 0,0x03600271);
		IO_WRITE32MSK(OSD_BASE, 0xcc, 0,0x03600271);
		//IO_WRITE32MSK(OSD_BASE, 0x20, 0,0x001e000c);
		IO_WRITE32MSK(OSD_BASE, 0x20, 0,0x0019002b);
		IO_WRITE32(OSD_BASE, 0,8);
	}
	
    return (INT32)OSD_RET_OK;
}

INT32 OSD_SetPowerDwn(BOOL fgEnable)
{
	Printf("OSD_SetPowerDwn: %x\n",fgEnable);
	if(fgEnable)
	{	
		IO_WRITE32(OSD_CK_REG, 0, (_u4Clock |0x80));
	}
	else
	{
		IO_WRITE32(OSD_CK_REG, 0, _u4Clock );
	}
	return (INT32)OSD_RET_OK;
}

INT32 OSD_ShowLogo(void)
{
#ifndef CC_MTK_LOADER
    static UINT32 u4RegionList;
    UINT32 u4Region=0, u4BmpPitch,u4Width,u4Height,u4ColorMode;
    UINT32 u4PanelWidth, u4PanelHeight;
    UINT32 u4OutWidth, u4OutHeight,u4OutX,u4OutY;
    UINT32 i;
    UINT32 u4BmpAddr;//,u4PalAddr;
    INT32 ret;
    #ifdef CC_MT5363
    UINT32 u4tmpScaler;
    #endif


    if(DRVCUST_OptGet(eDrvLogoEnable)==0)
    {
    	 Printf("Driver Logo Disabled!\n");
        return (INT32)OSD_RET_OK;
    }
    if(fgLogoInit==TRUE)
    {
    	ret = OSD_PLA_FlipTo(OSD_PLANE_2, u4RegionList);
    }
    else
    {
       // power on panel
       //vApiPanelPowerSequence(TRUE);
       OSD_Init();
    	u4PanelWidth = PANEL_GetPanelWidth();
    	u4PanelHeight = PANEL_GetPanelHeight();

    	Printf("Panel %d x %d \n", u4PanelWidth, u4PanelHeight);

    	ret = OSD_RGN_LIST_Create(&u4RegionList);
    		if (ret != OSD_RET_OK) return 1;

    	u4BmpPitch = 0;
    	u4ColorMode=DRVCUST_OptGet(eDrvLogoColorMode);
    	u4Width=DRVCUST_OptGet(eDrvLogoWidth);
    	u4Height=DRVCUST_OptGet(eDrvLogoHeight);
       u4BmpAddr=DRVCUST_OptGet(eDrvLogoBmpAddr);
       //if(u4ColorMode==OSD_CM_RGB_CLUT8)
       //{
       //	u4PalAddr=DRVCUST_OptGet(eDrvLogoPalAddr);
       //}
    
    	OSD_GET_PITCH_SIZE(u4ColorMode, u4Width, u4BmpPitch); // to set u4BmpPitch by u4ColorMode and u4Width.
    	ret = OSD_RGN_Create(&u4Region, u4Width, u4Height, (void *)u4BmpAddr,
                            u4ColorMode, u4BmpPitch, 0, 0, u4Width, u4Height);

    	//if(u4ColorMode==OSD_CM_RGB_CLUT8)
       //{
       //	OSD_RGN_Set(u4Region, OSD_RGN_PAL_ADDR, u4PalAddr);
       //}
    	if (ret != OSD_RET_OK) return 2;
    	ret = OSD_RGN_Insert(u4Region, u4RegionList);
    	if (ret != OSD_RET_OK) return 3;
    	ret = OSD_PLA_FlipTo(OSD_PLANE_2, u4RegionList);
    	if (ret != OSD_RET_OK) return 4;

	#if 1   // original bmp size.
	    if(u4PanelWidth>1366)
	    {
      u4OutWidth = (u4Width*u4PanelWidth)/1366;
	    u4OutHeight = (u4Height*u4PanelHeight)/768;
	    //Printf("Out: %d x %d \n", u4OutWidth, u4OutHeight);
	    
	    u4OutX =((u4PanelWidth - u4OutWidth) >> 1);
	    u4OutY =((u4PanelHeight - u4OutHeight) >> 1);
	    ret = OSD_SC_Scale(OSD_SCALER_2, 1, u4Width, u4Height, u4OutWidth, u4OutHeight);
	    if (ret != OSD_RET_OK) return 5;	
	
			#ifdef CC_MT5363    	
	    if(gfgSwap)
	    	u4tmpScaler=OSD_SCALER_3;
	    else
	    	u4tmpScaler=OSD_SCALER_2;

	 			_OSD_SC_SetSrcHSize(u4tmpScaler, u4PanelWidth);
        _OSD_SC_SetVscHSize(u4tmpScaler, u4PanelWidth);
        _OSD_SC_SetDstHSize(u4tmpScaler, u4PanelWidth);
        _OSD_SC_SetSrcVSize(u4tmpScaler, u4PanelHeight);
        _OSD_SC_SetDstVSize(u4tmpScaler, u4PanelHeight);
        _OSD_SC_UpdateHwReg(u4tmpScaler);
      #else
	    	_OSD_SC_SetSrcHSize(OSD_SCALER_2, u4PanelWidth);
        _OSD_SC_SetVscHSize(OSD_SCALER_2, u4PanelWidth);
        _OSD_SC_SetDstHSize(OSD_SCALER_2, u4PanelWidth);
        _OSD_SC_SetSrcVSize(OSD_SCALER_2, u4PanelHeight);
        _OSD_SC_SetDstVSize(OSD_SCALER_2, u4PanelHeight);
        _OSD_SC_UpdateHwReg(OSD_SCALER_2);
      #endif
	    
    	ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_X, (u4OutX*1366)/u4PanelWidth);
    	if (ret != OSD_RET_OK) return 6;
    	ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_Y, (u4OutY*768)/u4PanelHeight);
    	if (ret != OSD_RET_OK) return 7;
    	}
    	else
    	{
      
    	ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_X, (u4PanelWidth - u4Width) >> 1);
    	if (ret != OSD_RET_OK) return 5;
    	ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_Y, (u4PanelHeight - u4Height) >> 1);
    	if (ret != OSD_RET_OK) return 6;
      
      }
	#else   // scale size.
    	ret = OSD_SC_Scale(OSD_SCALER_2, 1, u4Width, u4Height, u4PanelWidth, u4PanelHeight);
    	if (ret != OSD_RET_OK) return 7;
	#endif
	fgLogoInit=TRUE;
    }
    ret = OSD_PLA_Enable(OSD_PLANE_2, TRUE);
    if (ret != OSD_RET_OK) return 8;

    OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, 0);   
    OSD_RGN_Set(u4Region, OSD_RGN_MIX_SEL, (UINT32)OSD_BM_REGION);   
 
	//OSD_PLA_SetFading(OSD_PLANE_2,0);
	for(i=0;i<16;i++)
	{
		x_thread_delay(62);
		//OSD_PLA_SetFading(OSD_PLANE_2,((i+1)*16-1));
		OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, ((i+1)*16-1));   
	}
	x_thread_delay(1000);
	for(i=16;i>0;i--)
	{
		//OSD_PLA_SetFading(OSD_PLANE_2,(i*16-1));
		OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, (i*16-1));   
		x_thread_delay(62);
	}
	//OSD_PLA_SetFading(OSD_PLANE_2,0xff);
	OSD_RGN_Set(u4Region, OSD_RGN_ALPHA, 0);   
	
#endif	//#ifndef CC_MTK_LOADER
    return (INT32)OSD_RET_OK;
}

