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
 * $Author: hongjun.chu $
 * $Date: 2012/04/28 $
 * $RCSfile: loader_panel.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file loader_if.c
 *  loader_if.c provides loader high-level interface functions.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "c_model.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#ifndef LOADER_LOGO_NUMBER
#define LOADER_LOGO_NUMBER          (0)
#endif

#ifndef LOADER_MULTIPLE_LOGO_PARTITION
#define LOADER_MULTIPLE_LOGO_PARTITION	(0)
#endif

#ifndef LOADER_LED_TWINKLE
#define LOADER_LED_TWINKLE          (0)
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#if LOADER_LOGO_NUMBER || LOADER_MULTIPLE_LOGO_PARTITION
#include "ostg_if.h"
#include "pmx_drvif.h"
#include "osd_drvif.h"
#include "panel.h"
#include "drv_video.h"
#include "drv_vdoclk.h"
#include "drv_display.h"
#include "drv_lvds.h"
#include "sv_const.h"
#include "hw_scpos.h"
#include "hw_lvds.h"
#include "vdp_if.h"
#include "nptv_if.h"
#include "x_pdwnc.h"
#if defined(CC_MT5368)|| defined(CC_MT5396)|| defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5860)
#include "drv_tcon.h"
#endif
#endif /* LOADER_LOGO_NUMBER */
#include "loader_if.h"

#include "x_hal_926.h"
#ifdef CC_MT5395
#include "drv_vbyone.h"
#endif
#include "drv_scpos.h"
//---------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------

#if LOADER_LOGO_NUMBER || LOADER_MULTIPLE_LOGO_PARTITION
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** LDR_OsdDisplay(): Setup OSD driver.
 *  This function is going to setup osd driver with bmp info.
 *
 *  @param  u4BmpAddr       The bmp data pointer.
 *  @param  u4Width         The width info of the bmp data.
 *  @param  u4Height        The height info of the bmp data.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
UINT32 LDR_OsdDisplay(UINT32 u4ColorMode, UINT32 u4BmpAddr, UINT32 u4Width, UINT32 u4Height)
{
    UINT32 u4RegionList, u4Region, u4BmpPitch;
    UINT32 u4PanelWidth, u4PanelHeight;
    INT32 ret;

    Printf("Color:%d BmpAddr:0x%08x Width:%d Height:%d\n", u4ColorMode, u4BmpAddr, u4Width, u4Height);

    OSD_Init();

    u4PanelWidth = PANEL_GetPanelWidth();
    u4PanelHeight = PANEL_GetPanelHeight();

    Printf("Panel %d x %d \n", u4PanelWidth, u4PanelHeight);

    ret = OSD_RGN_LIST_Create(&u4RegionList);
    if (ret != OSD_RET_OK) return 1;
    u4BmpPitch = 0;
    OSD_GET_PITCH_SIZE(u4ColorMode, u4Width, u4BmpPitch); // to set u4BmpPitch by u4ColorMode and u4Width.
    ret = OSD_RGN_Create(&u4Region, u4Width, u4Height, (void *)PHYSICAL(u4BmpAddr),
                            u4ColorMode, u4BmpPitch, 0, 0, u4Width, u4Height);
    if (ret != OSD_RET_OK) return 2;
    ret = OSD_RGN_Insert(u4Region, u4RegionList);
    if (ret != OSD_RET_OK) return 3;
    ret = OSD_PLA_FlipTo(OSD_PLANE_2, u4RegionList);
    if (ret != OSD_RET_OK) return 4;

#if 1   // original bmp size.
    if (u4PanelWidth > u4Width)
    {
        ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_X, (u4PanelWidth - u4Width) >> 1);
    }
    else
    {
        ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_X, 0);
    }
    if (ret != OSD_RET_OK) return 5;

    if (u4PanelHeight > u4Height)
    {
        ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_Y, (u4PanelHeight - u4Height) >> 1);
    }
    else
    {
        ret = OSD_RGN_Set(u4Region, OSD_RGN_POS_Y, 0);
    }
    if (ret != OSD_RET_OK) return 6;
#else   // scale size.
    ret = OSD_SC_Scale(OSD_SCALER_2, 1, u4Width, u4Height, u4PanelWidth, u4PanelHeight);
    if (ret != OSD_RET_OK) return 7;
#endif

    HalFlushDCache();

    ret = OSD_PLA_Enable(OSD_PLANE_2, TRUE);
    if (ret != OSD_RET_OK) return 8;

    return 0;
}
#endif /* LOADER_LOGO_NUMBER */

//-----------------------------------------------------------------------------
/** LDR_LogoDisplay(): To display logo with panel index.
 *  This function is going to display logo at boot loader.
 *
 *  @param  u4BmpAddr       The bmp data pointer.
 *  @param  u4Width         The width info of the bmp data.
 *  @param  u4Height        The height info of the bmp data.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
UINT32 LDR_LogoDisplay(LDR_DATA_T *prLdrData, UINT32 u4Idx)
{
#if LOADER_LOGO_NUMBER > 0
    UINT32 u4Ret;

    if (u4Idx >= LOADER_LOGO_NUMBER)
    {
        Printf("no such index %d\n", u4Idx);
        return 0;
    }

    if (DRVCUST_InitGet((QUERY_TYPE_T)(((UINT32)eLoaderLogoDataIdx0) + u4Idx))==0)
    {
        return 0;
    }

    Printf("Display 0x%08x Idx:%d BG:0x%08x\n", prLdrData, u4Idx, DRVCUST_InitGet(eLoaderMsgBackground));
    LDR_PmxDisplay(DRVCUST_InitGet(eLoaderLogoBackground));

    u4Ret = LDR_OsdDisplay(DRVCUST_InitGet(eLoaderLogoColorMode),
                    DRVCUST_InitGet((QUERY_TYPE_T)(((UINT32)eLoaderLogoDataIdx0) + u4Idx)),
                    ((UINT32 *)DRVCUST_InitGet(eLoaderLogoWidthArray))[u4Idx],
                    ((UINT32 *)DRVCUST_InitGet(eLoaderLogoHightArray))[u4Idx]);
    Printf("LDR_OsdDisplay(%d, 0x%08x, %d, %d) return %d\n",
                    DRVCUST_InitGet(eLoaderLogoColorMode),
                    DRVCUST_InitGet((QUERY_TYPE_T)(((UINT32)eLoaderLogoDataIdx0) + u4Idx)),
                    ((UINT32 *)DRVCUST_InitGet(eLoaderLogoWidthArray))[u4Idx],
                    ((UINT32 *)DRVCUST_InitGet(eLoaderLogoHightArray))[u4Idx], u4Ret);
    return u4Ret;
#else /* LOADER_LOGO_NUMBER > 0 */
    return 0;
#endif /* LOADER_LOGO_NUMBER > 0 */
}

//-----------------------------------------------------------------------------
/** LDR_PanelLogo(): display logo on panel.
 *  This function is going to find out the logo location and call lvds/pmx/osd 
 *  low level driver to display logo on panel.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
#if LOADER_MULTIPLE_LOGO_PARTITION
EXTERN INT32 Loader_ReadNandFlash(UINT32 u4PartId, UINT32 u4Offset, void *pvMemPtr, UINT32 u4MemLen);
VOID  jpeg_decode(UINT32 u4Buffer,UINT32 u4size,UINT32 u4WorkBuffer,UINT32 u4Worksize,UINT32 u4DstAddr,UINT32 u4Width,UINT32 u4Height);
#endif


void setmute(void)
{
	vRegWriteFldAlign(MUTE_00, 1, R_M_MUTE_FRONT_EN); // video mute

}

void vShowOSTGOutpattern(void)
{
	vRegWriteFldAlign(OSTG_PTGEN_00, 0, FLD_PATGEN_SEL);	 
    vRegWriteFldAlign(OSTG_PTGEN_00, 0, FLD_PATGEN_RSEL);
	vRegWriteFldAlign(OSTG_PTGEN_00, 1, FLD_PATGEN_EN);

}

UINT32 LDR_PanelLogo(LDR_DATA_T *prLdrData)
{
    UINT32 u4Ret;

#if LOADER_LED_TWINKLE
    Printf("Loader_BootLED: PDWNC_BASE=0x%08x\n", CKGEN_BASE);
    u4Ret = IO_READ32(PDWNC_BASE, 0x0b4);
    IO_WRITE32(PDWNC_BASE, 0x0b4, u4Ret | 0x40000);
    u4Ret = (0x6c000 / 1000) * LOADER_LED_TWINKLE;
    IO_WRITE32(PDWNC_BASE, 0x150, 0xf0000001 | (u4Ret << 4));
#endif
	setmute(); //added by hongjun.chu for temp resolution of starup garbage


#if LOADER_MULTIPLE_LOGO_PARTITION

#define JPEG_BUFFER_SIZE 0x00100000
#define LOADER_LOGO_OFFSET    0
    UINT8 *puJpgBuf = (UINT8 *)0x0D000000;
    UINT8 *puJpgWorkBuf = puJpgBuf + 0x01000000; // JPEG_BUFFER_SIZE;
    UINT8 *puJpgDestBuf = puJpgWorkBuf + 0x01000000; //JPEG_BUFFER_SIZE;
    UINT32 w, h;

    u4Ret = Loader_ReadNandFlash(LOADER_MULTIPLE_LOGO_PARTITION, LOADER_LOGO_OFFSET, puJpgBuf, JPEG_BUFFER_SIZE);
    Printf("Loader_ReadNandFlash(%d, %d, 0x%08x, %d)\n", LOADER_MULTIPLE_LOGO_PARTITION, LOADER_LOGO_OFFSET, puJpgBuf, JPEG_BUFFER_SIZE);
    LDR_PmxDisplay(DRVCUST_InitGet(eLoaderLogoBackground));
    w = PANEL_GetPanelWidth();
    h = PANEL_GetPanelHeight();

    jpeg_decode((UINT32)puJpgBuf, JPEG_BUFFER_SIZE, (UINT32)puJpgWorkBuf, JPEG_BUFFER_SIZE, (UINT32)puJpgDestBuf, w, h);
    Printf("jpeg_decode(0x%08x, %d, 0x%08x, %d, 0x%08x, %d, %d)\n",
                    puJpgBuf, JPEG_BUFFER_SIZE, puJpgWorkBuf, JPEG_BUFFER_SIZE, puJpgDestBuf, w, h);

    u4Ret = LDR_OsdDisplay(OSD_CM_ARGB8888_DIRECT32, (UINT32) puJpgDestBuf, w, h);
    Printf("LDR_OsdDisplay(%d, 0x%08x, %d, %d) return %d\n",
                    OSD_CM_ARGB8888_DIRECT32, (UINT32) puJpgDestBuf, w, h, u4Ret);
    return u4Ret;

#elif LOADER_LOGO_NUMBER
	#ifdef DRV_SUPPORT_MT8283
	UINT8 u1RetErr;
	#endif

    if (DRVCUST_InitGet((QUERY_TYPE_T)(((UINT32)eLoaderLogoDataIdx0) + 0))==0)
    {
        return 0;
    }

    // Check quiet boot flag and RTC at the same time.
    if ((prLdrData->rDtvCfg.u1Flags2 & DTVCFG_FLAG2_QUIET_BOOT) && (PDWNC_ReadWakeupReason() == PDWNC_WAKE_UP_REASON_RTC))
    {
        Printf("Quiet Boot\n");
        return 0;
    }

    Printf("Display 0x%08x background:0x%08x\n", prLdrData, DRVCUST_InitGet(eLoaderLogoBackground));
    LDR_PmxDisplay(DRVCUST_InitGet(eLoaderLogoBackground));
    Printf("LDR_OsdDisplay(%d, 0x%08x, %d, %d)\n",
                    DRVCUST_InitGet(eLoaderLogoColorMode),
                    DRVCUST_InitGet((QUERY_TYPE_T)(((UINT32)eLoaderLogoDataIdx0) + 0)),
                    ((UINT32 *)DRVCUST_InitGet(eLoaderLogoWidthArray))[0],
                    ((UINT32 *)DRVCUST_InitGet(eLoaderLogoHightArray))[0]);

    #ifdef DRV_SUPPORT_MT8283
   x_thread_delay(3000);
    drv_extmjc_set_video_fmt(0,60,2200,1125,0,1920,1080,&u1RetErr);
    drv_extmjc_set_Mode_Change(&u1RetErr);
    //x_thread_delay(1000);
    #endif
    
    u4Ret = LDR_OsdDisplay(DRVCUST_InitGet(eLoaderLogoColorMode),
                    DRVCUST_InitGet(eLoaderLogoDataIdx0),
                    ((UINT32 *)DRVCUST_InitGet(eLoaderLogoWidthArray))[0],
                    ((UINT32 *)DRVCUST_InitGet(eLoaderLogoHightArray))[0]);
    #ifdef DRV_SUPPORT_MT8283
    x_thread_delay(1000);
    #endif
	//	vShowOSTGOutpattern();
    return u4Ret;
#else /* LOADER_LOGO_NUMBER */


    UNUSED(u4Ret);
    return 0;
#endif /* LOADER_LOGO_NUMBER */


}

