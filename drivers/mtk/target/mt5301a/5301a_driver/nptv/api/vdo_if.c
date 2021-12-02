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
 * $RCSfile: vdo_if.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file vdo_if.c
 *  Brief of file vdo_if.c.
 *  Details of file vdo_if.c (optional).
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "vdo_drvif.h"
#include "b2r_drvif.h"
#include "x_bim.h"
#include "x_assert.h"
#include "general.h"
#include "hw_vdoin.h"
#include "drv_video.h"
#include "video_def.h"
#include "nptv_drvif.h"
#include "source_table.h"
#include "source_select.h"
#include "api_backlt.h"
#include "api_eep.h"
#include "eepdef.h"
#include "util.h"
#include "drv_vga.h"
#include "drv_scaler.h"
#include "drv_adcset.h"
#include "drv_tvd.h"
#include "drvcust_if.h"
#include "drv_scart.h"
#include "drv_ycproc.h"
#include "drv_di.h"
#include "cec_if.h"
#include "drv_dvi.h"
#include "drv_hdtv.h"
#include "drv_lvds.h"
#include "api_notify.h"
#include "nptv_debug.h"
#include "c_model.h"
#include "drv_vdoclk.h"
#include "mute_if.h"
#if defined(CC_SUPPORT_TVE) ||defined(CC_SUPPORT_TVE_82xx)
#include "tve_if.h"
#endif
#if SUPPORT_VBI
#include "drv_vbi.h"
#endif

#ifdef THE_3RD_PARTY_HEADER_FILE_1
#include THE_3RD_PARTY_HEADER_FILE_1
#endif

#ifdef CUSTOM_VIDEO_DRV_INCLUDE_FILE
#include CUSTOM_VIDEO_DRV_INCLUDE_FILE
#endif // #ifdef CUSTOM_VIDEO_DRV_INCLUDE_FILE

#include "vdp_drvif.h"
#include "srm_drvif.h"
#include "vdo_misc.h"
#ifdef CC_MT8223
#include "hw_pdwnc.h"
#endif


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------
#if SUPPORT_SCART
//extern UINT8 u1ScartOutSwitch;
#endif

extern  HANDLE_T _hMainSubSrcSemaphore; /* from mloop_video_if.c */
extern UINT8 _bHDMIColorSpace;

//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------
extern void vApiVideoExternalGammaInit(void);

//-----------------------------------------------------------------------------
// Static function forward declarations
//-----------------------------------------------------------------------------
PRIVATE void vDrvVideoPathShutDown(UINT8 path);


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

UINT8 _bFixColorSpace[2]={0,0};
UINT8 _bOSDFixColorSpace[2]={0,0}; //add by ciwu

UINT32 _u4ScanMode[2] = {0, 0};


#if SUPPORT_VBI
static VBI_MODE_T _t_Vbi_Mode = VBI_CC_VCHIP_MODE;
#endif

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#ifndef CC_UP8032_ATV
#define MainSubSrc_Biglock_Release() \
	VERIFY(BIM_EnableIrq(VECTOR_VDOIN)); \
	VERIFY(AD_ReleaseControl()==OSR_OK); \
  	VERIFY (x_sema_unlock(_hMainSubSrcSemaphore) == OSR_OK);
#else
#define MainSubSrc_Biglock_Release() \
        vEnableSysInt2(VDOIN_INT_EN);
#endif


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

PRIVATE void vDrvVideoPathShutDown(UINT8 path)
{
    //[DTV00045974]
    //vModeChgMute(path, SV_OFF);
    _vDrvVideoSetMute(MUTE_MODULE_MODECHG, path, FOREVER_MUTE, FALSE);

    /* Scaler */
    vDrvScpipWriteCtrl(path,SV_OFF);
    /* VBI */
#if SUPPORT_VBI
    (VBI_GetSlicerPath(VBI0) == path)?
    VBI_DisableBySlicer(VBI0):
    VBI_DisableBySlicer(VBI2);
#endif

    /* Disconnect the original function */
    vDrvVideoConnect(path, SV_OFF);
}


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

void vApiVideoInit(void)
{
    LOG(0, "vApiVideoInit\n");
#ifdef CC_MT8223
	BIM_WRITE32(0x0a4,5);	//
	IO_WRITE32(PDWNC_BASE, 4, 0x10006); // CLOCK FOR 8032 RUN NORMAL

#endif
    vDrvADCOffsetCal();

    vApiVideoPeInit();

    #ifndef CC_UP8032_ATV
    // Enable Video Interrupt
    UNUSED(BIM_EnableIrq(VECTOR_VDOIN));
    #else
    // Enable Video Interrupt
    vEnableSysInt2(VDOIN_INT_EN);
    // Turn On Output VSync Interrupt
    vEnableSysInt2(POST_PROC_INT_EN);
    #endif

    LboxDetectionInit();


#if SUPPORT_CEC
    CEC_Init();
#endif
#ifdef CC_MT8223
		vIO32WriteFldAlign(PDWNC_UP_CFG, 0x0, FLD_VIR_EN);
		vIO32WriteFldAlign(PDWNC_UP_CFG, 0x1, FLD_T8032_RST); //8032 not run at boot up (neither in ac on nor dc on)
		vIO32WriteFld(PDWNC_UP_CFG, 0x1, FLD_XDATA_ACC);//switch xdata to ARM
		IO_WRITE8(PDWNC_BASE, 0x140, 0); //srm to normal clock

//		Printf("-----PDWNC_InitT8032 28178=%08x\n",IO_READ32(PDWNC_BASE,0x178));
//		Printf("-----PDWNC_InitT8032 28140=%08x\n",IO_READ32(PDWNC_BASE,0x140));

#endif
#if SUPPORT_SCART
    //u1ScartOutSwitch = TRUE; //default to enable output through scart2
    //vDrvBypPD();
#endif
}

void vApiHwInit(void)
{
	// Initialize Global Variable
	vUtInitVar();

	// GPIO Init
	// ToDo

	//vDrvVOPLLAutoKVCOBand();// 5391 add VOPLL K VCO Band

#if SUPPORT_DISPLAY
	// Display Init
	vDrvDisplayInit();
	vDrvLVDSInit();
//	#if DMP_HWINIT_DBGMSG
	LOG(0, "Display Init");
//	#endif
#endif

#if SUPPORT_VIDEO
	// Video Init
	vDrvVideoSwInit();
//	#if DMP_HWINIT_DBGMSG
	LOG(0, "Video SW Init");
//	#endif
	vDrvVideoHwInit();
//	#if DMP_HWINIT_DBGMSG
	LOG(0, "Video HW Init");
//	#endif
#endif

    //TVD init
    vTvd3dInit();

	// VBI Init
#if SUPPORT_VBI
#ifdef COUNTRY_FROM_EEPROM
    UINT8 u1TargetCountryInfo;
    u1TargetCountryInfo = GetTargetCountry() & 0xFF;
    if (u1TargetCountryInfo & COUNTRY_CC_ENABLE_MASK ) //Bit 7 CC Enable/Disable
    {
	    _t_Vbi_Mode = VBI_CC_VCHIP_MODE;
    }
    else
    {
	    _t_Vbi_Mode = VBI_TTX_MODE;
    }
#else
	if(GetTargetCountry() == COUNTRY_US)
	{
	    _t_Vbi_Mode = VBI_CC_VCHIP_MODE;
	}
	else if(GetTargetCountry() == COUNTRY_EU)
	{
	    _t_Vbi_Mode = VBI_TTX_MODE;
	}
#endif

#ifdef CC_MT5363
	VBIISR_Init();
#endif
	VBI_Init(_t_Vbi_Mode);
	VBI_Enable();

//	#if DMP_HWINIT_DBGMSG
	LOG(0, "VBI_Init");
//	#endif
#endif


	// HDMI Init
	vHDMIInit();
#ifdef CC_SUPPORT_TVE_82xx
         /* TVE Bypass Init */
         Bypass_Init();
#endif
	// Enable Interrupt
	// vKrEnableInt();
//#if DMP_HWINIT_DBGMSG
//	LogS("Enable Int");
//#endif
#ifndef CC_UP8032_ATV
#ifndef CC_DISABLE_EEP_MAG_CHK
	if( !fgApiIsEepromMagicOK() )
	{	//reset EEPROM
            vApiEepromDefaultInit();
	}
#endif //#ifndef CC_DISABLE_EEP_MAG_CHK
#endif
}

/**
 * Set video source of the specified channel.
 *
 * @param bPath SV_VP_MAIN or SV_VP_PIP
 * @param bSrc Select the video source. See enum eVideoSrc.
 *
 * @retval SV_TRUE
 * @retval SV_FALSE
 */
UINT8 bApiVideoSetVideoSrc(UINT8 bPath, UINT8 bSrc)
{
    UINT8 bStatus;
    LOG(3, "ApiVideoSetVideoSrc(%d, %d)\n", (UINT32)bPath, (UINT32)bSrc);   

    if (bPath == SV_VP_MAIN)	//check the real source
    {
         bStatus = bApiVideoMainSubSrc(bSrc, SV_VS_NO_CHANGE);
    } else {
         bStatus = bApiVideoMainSubSrc(SV_VS_NO_CHANGE, bSrc);
    }

#ifdef CC_SUPPORT_TVE_82xx
    vApiTVECtrl(&bSrc);
#endif

    return bStatus;
}


UINT8 bApiVideoMainSubSrc(UINT8 bMainSrc, UINT8 bSubSrc)
{
    static UINT8 bOldMainDec = 0xff;
    static UINT8 bOldSubDec = 0xff;
    UINT8 bNewMainDec, bNewSubDec;
    ExtInputTable NewExtInput;
    BOOL fgMainCh = FALSE;
    BOOL fgPipCh = FALSE;
    BOOL fgMainCombi;
    BOOL fgSubCombi;

    #ifndef CC_UP8032_ATV
    VERIFY (x_sema_lock(_hMainSubSrcSemaphore, X_SEMA_OPTION_WAIT) == OSR_OK);
    VERIFY(AD_AcquireControl()==OSR_OK);
    VERIFY(BIM_DisableIrq(VECTOR_VDOIN));
    #else
    vDisableSysInt2(VDOIN_INT_EN);
    #endif

#if 0 // 5387
#if DISP_MODE_USE == DISP_MODE_ON
    // Disable Display Mode of the SCPOS. SJ Added 050615
    vSwitchDispMode(SV_OFF);
#endif
#endif
    if(bMainSrc ==SV_VS_NO_CHANGE)
        bMainSrc = _bSrcMainOld;

    if(bSubSrc ==SV_VS_NO_CHANGE)
        bSubSrc = _bSrcSubOld;


    NewExtInput.MapIntMode = bDrvGetMapIntMode(bMainSrc, bSubSrc);

    /* is changed ? */
    {
        if (NewExtInput.MapIntMode == 0xFFFF) /* check PIP constrain */
        {
            MainSubSrc_Biglock_Release();
            return (SV_FAIL);
        }

        fgMainCombi=fgApiMonitorSetup(SV_VP_MAIN,bMainSrc);
        if (_bMonMainNew == bMainSrc)
        {
            fgMainCombi = FALSE;
        }
        fgSubCombi=fgApiMonitorSetup(SV_VP_PIP,bSubSrc);
        if (_bMonSubNew == bSubSrc)
        {
            fgSubCombi = FALSE;
        }

        if (!fgMainCombi)
        {
#ifndef CC_DRIVER_PROGRAM
            _rMChannel.t_mon_id.e_type = DRVT_UNKNOWN;
#endif
            _bMonMainOld = SV_VS_MAX;
            _bMonMainNew = SV_VS_MAX;
            _bMainMonICIn = (UINT8)P_FA;
        }


        if (!fgSubCombi)
        {
#ifndef CC_DRIVER_PROGRAM
            _rPChannel.t_mon_id.e_type = DRVT_UNKNOWN;
#endif
            _bMonSubOld = SV_VS_MAX;
            _bMonSubNew = SV_VS_MAX;
            _bSubMonICIn = (UINT8)P_FA;
        }


        /* Combi related behavior here */

        if (_bSrcMainOld != bMainSrc)
        {
            fgMainCh = TRUE;
            _bFixColorSpace[0]=0;
            vDrvSwitchMTKGoodDclk(SV_OFF);

        }
        if (_bSrcSubOld != bSubSrc)
        {
            fgPipCh = TRUE;
            _bFixColorSpace[1]=0;

        }
        _bSrcMainNew = bMainSrc;
        _bSrcMainOld = bMainSrc;
        _bSrcSubNew = bSubSrc;
        _bSrcSubOld = bSubSrc;


        _bMainICIn = NewExtInput.MapIntMode >> 8;
        _bSubICIn = NewExtInput.MapIntMode & 0xff;

        bNewMainDec = bGetInternalDec(SV_VP_MAIN);
        bNewSubDec = bGetInternalDec(SV_VP_PIP);

        if (bNewMainDec != bOldMainDec)
        {
            fgMainCh = TRUE;
        }
        if (bNewSubDec != bOldSubDec)
        {
            fgPipCh = TRUE;
        }
        bOldMainDec = bNewMainDec;
        bOldSubDec = bNewSubDec;

        #if !SUPPORT_POP
        fgPipCh = FALSE;
        #endif

        if ((!fgMainCh) && (!fgPipCh))
        {
            MainSubSrc_Biglock_Release();
            return SV_NO_CHANGE;
        }
    }

    /************************  input changed *********************/

    /* Move SRM_SendEvent after critical section */

    // Restore fgAutoSearch Flag
    if (((fgMainCh== TRUE) && (bGetSignalType(SV_VP_MAIN) != SV_ST_TV) && (bGetSignalType(SV_VP_MAIN) != SV_ST_MAX))||
            ((fgPipCh== TRUE)&& (bGetSignalType(SV_VP_PIP) != SV_ST_TV)&& (bGetSignalType(SV_VP_PIP) != SV_ST_MAX)))
    {
        _fgAutoSearch = FALSE;
    }

    /* shutdown hardware/firmware in main path */
    if (fgMainCh && fgIsMainDec())
    {
        vDrvVideoPathShutDown(SV_VP_MAIN);
    }
    /* shutdown hardware/firmware in pip path */
    if (fgPipCh && fgIsPipDec())
    {
        vDrvVideoPathShutDown(SV_VP_PIP);
    }

    /* bIsChannelOn -> vVdoInISR -> signal status -> WriteCtrl(on/off) */
    /* bSigStatus -> mode change/detect state machine */
    /* only in ISR, it can turn on WriteCtrl */
    if (bMainSrc == SV_VS_MAX)
    {
        _rMChannel.bIsChannelOn = SV_OFF;
        _rMChannel.bDecType = SV_VD_NA;
        vDrvScpipWriteCtrl(SV_VP_MAIN,SV_OFF);
    }
    else
    {
        _rMChannel.bIsChannelOn = SV_ON;
    }

    if (bSubSrc == SV_VS_MAX)
    {
        _rPChannel.bIsChannelOn = SV_OFF;
        _rPChannel.bDecType = SV_VD_NA;
        vDrvScpipWriteCtrl(SV_VP_PIP,SV_OFF);
    }
    else
    {
        _rPChannel.bIsChannelOn = SV_ON;
    }


    /* power down put here */
    if (((_bMainICIn < (UINT8)P_YP0) || (_bMainICIn > (UINT8)P_FB1)) && ((_bSubICIn < (UINT8)P_YP0) || (_bSubICIn > (UINT8)P_FB1)))
    {
        vDrvAllHDADCPow(SV_FALSE);
    }

    if (((_bMainICIn > (UINT8)P_YP0)&&(_bMainICIn != (UINT8)P_FB0)&&(_bMainICIn !=(UINT8)P_FB1))&&((_bSubICIn > (UINT8)P_YP0)&&(_bSubICIn !=(UINT8)P_FB0)&&(_bSubICIn !=(UINT8)P_FB1)))
    {
        vDrvCvbsVfePD();
    }

#if FIX_VGA_HDMI_INTERFERENCE
    if( (VSS_MAJOR(bMainSrc) == VSS_VGA && VSS_MAJOR(bSubSrc) != VSS_HDMI) ||
        (VSS_MAJOR(bSubSrc) == VSS_VGA && VSS_MAJOR(bMainSrc) != VSS_HDMI))
    {
        vHDMIPowerOff();
    }
#endif

#if SUPPORT_SCART && SUPPORT_PIP_NOSCARTOUT
    if ((_bSubICIn <= P_MA))	vDrvBypPD();
#endif

    /* disconnect unused internal mux */
    vMuxCleanup();

#if 0 /*  not return so early, the next block of code     */
      /*   is needed to clear some flags for correct flow */
    /* off all hardware first */
    if ((bMainSrc == SV_VS_MAX) && (bSubSrc == SV_VS_MAX))
    {
#ifdef CC_SUPPORT_TVE
        /* b2r setting clean-up */
        VDP_B2rSwitch(SV_VP_MAIN, B2R_NS);
        VDP_B2rSwitch(SV_VP_PIP, B2R_NS);
#endif
        MainSubSrc_Biglock_Release();
#ifdef CC_SRM_ON
        SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_SOURCE + (UINT32)VDP_1), (UINT32)bMainSrc, 0);
        SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_SOURCE + (UINT32)VDP_2), (UINT32)bSubSrc, 0);
#endif
        return SV_SUCCESS;
    }
#endif

    vDrvSetExternalMux(_bSrcMainNew, _bSrcSubNew);

    if (fgMainCh)
    {
        vDrvSetInternalMux(0,_bSrcMainNew);
        vSetMOutMux(bNewMainDec);
    }

    #if SUPPORT_POP
    if (fgPipCh)
    {
        vDrvSetInternalMux(1,_bSrcSubNew);
        vSetSOutMux(bNewSubDec);
    }
    #endif

    // MDisableMainINT();

    if (fgMainCh)
    {
        _rMChannel.bDecType = bNewMainDec;

        /* Update information for Main Channel New Decoder */
        if (fgIsMainDec())
        {
            /* VBI */
#if SUPPORT_VBI
            if (VBI_GetSlicerPath(VBI0) == SV_VP_MAIN)
            {
                VBI_DisableBySlicer(VBI0);
            }
            else
            {
                VBI_DisableBySlicer(VBI2);
            }
#endif
            vDrvVideoConnect(SV_VP_MAIN, SV_ON);
        }

        _bMainState = VDO_STATE_IDLE; /* mode change state machine */
        vClrMainFlg(MAIN_FLG_MODE_DET_DONE);
        vSetMainFlg(MAIN_FLG_MODE_CHG);

        // Luis060627, for delay mute
        vApiVideoClrEvtFlg(VDO_FLG_MAIN_MODECHG_DONE);

       #ifndef CC_COPLAT_MT82
        // William 20060316
        if (bMainSrc == SV_VS_DT1)
        {
            VDP_B2rSwitch(SV_VP_MAIN, B2R_1);
        }
        else if (bMainSrc == SV_VS_DT2)
        {
            VDP_B2rSwitch(SV_VP_MAIN, B2R_2);
        }
        else
        {
            VDP_B2rSwitch(SV_VP_MAIN, B2R_NS);
        }
        #endif
    }

    if (fgPipCh)
    {
        _rPChannel.bDecType = bNewSubDec;

        /* Update information for Main Channel New Decoder */
        if (fgIsPipDec())
        {
            /* VBI */
#if SUPPORT_VBI
            if (VBI_GetSlicerPath(VBI0)== SV_VP_PIP)
            {
                VBI_DisableBySlicer(VBI0);
            }
            else
            {
                VBI_DisableBySlicer(VBI2);
            }
#endif
            vDrvVideoConnect(SV_VP_PIP, SV_ON);
        }

        _bPipState = VDO_STATE_IDLE; /* mode change state machine */
        vClrPipFlg(PIP_FLG_MODE_DET_DONE);
        vSetPipFlg(PIP_FLG_MODE_CHG);

        // Luis060627, for delay mute
        vApiVideoClrEvtFlg(VDO_FLG_PIP_MODECHG_DONE);

       #ifndef CC_COPLAT_MT82
        // William 20060316
        if (bSubSrc == SV_VS_DT1)
        {
            VDP_B2rSwitch(SV_VP_PIP, B2R_1);
        }
        else if (bSubSrc == SV_VS_DT2)
        {
            VDP_B2rSwitch(SV_VP_PIP, B2R_2);
        }
        else
        {
            VDP_B2rSwitch(SV_VP_PIP, B2R_NS);
        }
        #endif
    }

    // MEnableMainINT();
    MainSubSrc_Biglock_Release();
#ifdef CC_SRM_ON
    SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_SOURCE + (UINT32)VDP_1), (UINT32)bMainSrc, 0);
#if SUPPORT_POP
    SRM_SendEvent(SRM_DRV_SCPOS, (SRM_SCPOS_EVENT_SOURCE + (UINT32)VDP_2), (UINT32)bSubSrc, 0);
#endif
#endif
    return SV_SUCCESS;
}

/**
 * Set color systrem.
 *
 * @param bColSys SV_CS_AUTO, SV_CS_PAL, SV_CS_PAL_M, SV_CS_PAL_N,
 * SV_CS_NTSC358, SV_CS_NTSC443 or SV_CS_SECAM
 * @return The exit status of this api.
 */
UINT8 fgApiVideoColorSystem(UINT8 bColSys)
{
	if (fgIsMainTvd3d()||fgIsPipTvd3d())
	{
		vDrvTvd3dSetColorSystem(bColSys);
		return (TRUE);
	}
	return FALSE;
}
/**
 * Get Current TV color system
 *
 * @return Current Color System. SV_CS_AUTO / SV_CS_PAL / SV_CS_PAL_M
 * / SV_CS_PAL_N / SV_CS_NTSC358 / SV_CS_NTSC443 / SV_CS_SECAM
 */
UINT8 bApiVideoGetColorSystem(void)
{
	return bTvd3dGetColorSystem();
}


UINT8 bApiVideoGetInputTimingID(UINT8 bPath)
{
    switch (bGetVideoDecType(bPath))
    {
        case SV_VD_YPBPR:
            return _bHdtvTiming;
        case SV_VD_VGA:
        {
            UINT8 bTimingId;
            if (_bVgaTiming<bUserVgaTimingBegin)
                return _bVgaTiming;
            /* user mode */
            bTimingId = _bVgaTiming-bUserVgaTimingBegin;
            if (bTimingId >= USERMODE_TIMING)
            {
                return 0;
            }
            if (rVgaUsrEEP[bTimingId].id)
                return _bVgaTiming;
            else
                return _bVgaTiming+USERMODE_TIMING;
        }
        case SV_VD_DVI:
            return _bDviTiming;
        default:
            return 0;
    }
}


UINT8 bApiVideoGetVideoTiming(UINT8 bPath)
{
    UINT8 _bTiming;
    UINT8 _bDecType;

    if (bPath == SV_VP_MAIN)
    {
        _bDecType = _rMChannel.bDecType;
    }
    else
    {
        _bDecType = _rPChannel.bDecType;
    }

    switch (_bDecType)
    {
    case SV_VD_YPBPR:
        _bTiming = _bHdtvTiming;
        break;

    case SV_VD_VGA:
        _bTiming = _bVgaTiming;
        break;

#if SUPPORT_DVI
    case SV_VD_DVI:
        _bTiming = _bDviTiming;
        break;
#endif

    default:
        return SV_VIDEO_TM_MAX;
    }

    switch (_bTiming)
    {
    case MODE_525I:
    case MODE_525I_OVERSAMPLE:
        return SV_VIDEO_TM_480I;	// "480I"

    case MODE_625I:
    case MODE_625I_OVERSAMPLE:
        return SV_VIDEO_TM_576I;	// "576I"

    case MODE_480P:
        return SV_VIDEO_TM_480P;	// "480P"

    case MODE_576P:
        return SV_VIDEO_TM_576P;	// "576P"

    case MODE_720p_50:
    case MODE_720p_60:
        return SV_VIDEO_TM_720P;	// "720P"

    case MODE_1080i_50:
    case MODE_1080i:
        return SV_VIDEO_TM_1080I;// "1080i"

    case MODE_1080p_50:
    case MODE_1080p_60:
        return SV_VIDEO_TM_1080P;// "1080P"

    default:
        return SV_VIDEO_TM_MAX;
    }
}


// VGA Politary Check Modify by W.C Shih2006/10/30
UINT8 bApiVideoGetInputPolarity(UINT8 bPath)
{
    UINT8 bType,bPolarity;
    INT32 i;
    UINT32   PolarityCheckFlag;

    bType = bGetVideoDecType(bPath);

    i = DRVCUST_OptQuery(ePolitaryCheck,&PolarityCheckFlag);
    if ((i<0)||(PolarityCheckFlag==0))
    {
        bPolarity = 0;
    }else
    {
        bPolarity = (fgSP0Hpol<<1) + (fgSP0Vpol);
    }

    switch (bType)
    {
    case SV_VD_VGA:
        return bPolarity;
    default:
        return 0;
    }
}

/**
 * @brief
 * @param
 * @retval
 * @example
 */
UINT16 wApiVideoGetInputWidth(UINT8 bPath)
{
    UINT8 bType;
    UINT16 wRet = 0;

    bType = bGetVideoDecType(bPath);

    switch (bType)
    {
    case SV_VD_YPBPR:
    case SV_VD_VGA:

        wRet = wDrvVideoInputWidth(bPath);//wHdtvInputWidth(); need include
        if (bHdtvInputWidthOverSample(bDrvVideoGetTiming(bPath)))
        {
            wRet>>=1;
        }
        return wRet;

    case SV_VD_TVD3D:
        wRet = wDrvVideoInputWidth(bPath);//wHdtvInputWidth(); need include
        if (vTvdGetOverSampleEnable())
        {
            wRet>>=1;
        }
        return wRet;

    default:
        return wDrvVideoInputWidth(bPath);
    }

}

/**
 * @brief
 * @param
 * @retval
 * @example
 */
UINT16 wApiVideoGetInputHeight(UINT8 bPath)
{
    return wDrvVideoInputHeight(bPath);
}

/**
 * @brief
 * @param
 * @retval
 * @example
 */
UINT8 bApiVideoGetRefreshRate(UINT8 bPath)
{
#if 0
    // ToDo
#if SUPPORT_VGA_USERMODE
    if (_bVgaUserMode)
        return bSP0Vclk;	// Refresh Rate
#endif
#endif
    return bDrvVideoGetRefreshRate(bPath);
}

UINT8 bApiVideoIsSrcInterlace(UINT8 bPath)
{
    return bDrvVideoIsSrcInterlace(bPath);
}

UINT8 bApiVideoFixColorCurState(UINT8 bPath)
{
    return _bFixColorSpace[bPath];
}

void vApiVideoFixColorSpace(UINT8 bPath,UINT8 bMode)
{
	_bFixColorSpace[bPath]=bMode ;
}

void vApiHdmiColorModeChg(UINT8 bPath)
{
	if (fgIsMainDVI() || fgIsPipDVI())
	{
		//force hdmi color space mode change
		_bHDMIColorSpace = 0xff ;
		if (fgIsMainDVI())
		{
            _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_MAIN, FOREVER_MUTE, FALSE);
		}
		if (fgIsPipDVI())
		{
            _vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_PIP, FOREVER_MUTE, FALSE);
		}
		LOG(3, " HDMI color mode change from VDP\n");
	}
}

UINT8 fgApiVideoIsVgaTiming(UINT8 bPath)
{
    UINT8 bDecType;

    bDecType = bGetVideoDecType(bPath);

    switch (bDecType)
    {
    case SV_VD_VGA:
#ifdef CC_SCPOS_PATTERN_GENERATOR
		if(u1GetScposPtGenEnable())
		{
			return SV_TRUE ;
		}
#endif
        return  (((_bVgaTiming) >= VGA_SEARCH_START) && ((_bVgaTiming) <= VGA_SEARCH_END) &&  (!bVgaInterlace()));
#if SUPPORT_DVI
    case SV_VD_DVI:
        if (fgIsVideoTiming(_bDviTiming))
        {
            return 0;	//video
        }
        else
        {
            return 1;	//vga
        }
#endif
    default:
        return 0;
    }
}

// REMOVE ME!
extern UINT8 bSUPPORT_VGA_422;
UINT8 fgApiVideoIsVgaMode(UINT8 bPath, UINT8 bChkFlg) reentrant
{
    if(bSUPPORT_VGA_422 && (bGetSignalType(bPath) == SV_ST_VGA))
    {
        return 0;
    }
    else
    {
        return fgApiVideoIsVgaTiming(bPath);
    }
}

// REMOVE ME!
UINT8 fgApiVideoIsVgaModeEx(UINT8 bPath, UINT8 bChkFlg)
{
    return fgApiVideoIsVgaTiming(bPath);
}


BOOL fgApiVideoIsSourceCVBS(UINT8 bPath)
{
    if (((bPath == SV_VP_MAIN) && ((_rMChannel.bDecType == SV_VD_TVD3D))) ||
            ((bPath == SV_VP_PIP) && (_rPChannel.bDecType == SV_VD_TVD3D))
       )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**
 * @brief Get main/sub path video source type
 * @param bPath SV_VP_MAIN/SV_VP_PIP
 */
UINT8 bApiVideoGetSrcType(UINT8 bPath)
{
    if (bPath == SV_VP_MAIN)
    {
        return _bSrcMainNew;
    }
    else
    {
        return _bSrcSubNew;
    }
}


void vApiVideoSetScanMode(UINT32 u4VdpId, UINT32 u4Mode)
{
    CRIT_STATE_T rCritState;
    
    if (u4VdpId >= VDP_NS)
    {
        return;
    }

    if (_u4ScanMode[u4VdpId] != u4Mode)
    {
        rCritState = x_crit_start();
        _u4ScanMode[u4VdpId] = u4Mode;
        x_crit_end(rCritState);
		LOG(3, "auto search path %d mode %d\n", u4VdpId, u4Mode);
        //set mute 10 for autosearch change
		_vDrvVideoSetMute(MUTE_MODULE_AUTOSEARH, u4VdpId, 10, FALSE);
		if(_u4ScanMode[u4VdpId])
		{
			//set modechange count to 0 and invalid it
			 _vDrvVideoSetMuteInvalidFlag(MUTE_MODULE_MODECHG, u4VdpId, TRUE);             
		}
		else
		{
		  //set invalid flag to false 		
			_vDrvVideoSetMuteInvalidFlag(MUTE_MODULE_MODECHG, u4VdpId, FALSE);
			//_vDrvVideoSetMute(MUTE_MODULE_MODECHG, u4VdpId, FOREVER_MUTE, FALSE);
		}   
        #ifdef CC_SRM_ON
        SRM_SendEvent(SRM_DRV_SCPOS, SRM_SCPOS_EVENT_SCAN_MODE, u4VdpId, u4Mode);
        #endif
        //[DTV00069391] for fine tune , tvd will not notify no signal case , if stable doens't notify
        if(u4Mode == 0 && (bDrvVideoSignalStatus_DBL(u4VdpId) != SV_VDO_STABLE))
        {
        	if (fgIsMainTvd3d())
        	{
        		vSetMainFlg(MAIN_FLG_MODE_CHG);
        		vClrMainFlg(MAIN_FLG_MODE_DET_DONE);	// Clear this flag!
        	}

        	else if (fgIsPipTvd3d())
        	{
        		vSetPipFlg(PIP_FLG_MODE_CHG);
        		vClrPipFlg(PIP_FLG_MODE_DET_DONE);	// Clear this flag!
        	}
			vTvd3dTrigModeDet();
        }
    }
}

UINT32 u4ApiVideoGetScanMode(UINT32 u4VdpId)
{
    if (u4VdpId >= 2)
    {
        return 0;
    }
    return _u4ScanMode[u4VdpId];
}



