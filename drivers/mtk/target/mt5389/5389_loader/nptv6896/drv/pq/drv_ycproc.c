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
 */
 /*------------------------------------------------------------------------------------------------ 
 * Owner: menghu.lin
 * 
 * Description: 
 * This file contains Luma/Chroma processing functions. 
 *-----------------------------------------------------------------------------------------------
 */ 

// === INCLUDE =============================================================================
#include "x_os.h"	
#include "x_rand.h"
#include "sv_const.h"
#include "hw_vdoin.h"
#include "vdo_misc.h"
#include "nptv_debug.h"
#include "video_def.h"
#include "drv_scaler.h"
#include "drv_lcdim.h"
#include "drv_od.h"
#include "drv_meter.h"
#include "drv_pqstress.h"
#include "x_assert.h"
#include "b2r_drvif.h"
#include "drv_upscaler.h"
#include "drv_scaler_drvif.h"
#include "drv_di.h"
#include "drv_contrast.h"
// === HW INCLUDE =============================================================================
#include "hw_ycproc.h"
#include "hw_sw.h"
#include "hw_ospe.h"
#include "hw_scpos.h"
#include "drv_tdtv_drvif.h"
// === DEFINE =============================================================================

// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================
extern void vDrvSharpProc(void);
extern void vDrvADLProc(void);
extern void vDrvGammaProc(void);
extern void vDrvGetSixColorReg(void);

// === GLOBAL VARIABLE ====================================================================
MMAppInfo sMMAppInfo; 

// === EXTERN VARIABLE ====================================================================
extern UINT16 u2PQItemMatrix[SV_VP_NA][PQ_MAX];
extern UINT8 bSceGain[3];
extern UINT8 bSkinTable[3];
extern UINT8 bSce3dGain[2];
extern UINT8 _bMAIN_COLOR_SYS;
extern UINT8 _bPIP_COLOR_SYS;

extern UINT16 wAngularHue[BASE_COLOR_MAX]; // hue : 128 as 0
extern UINT16 wAngularSat[BASE_COLOR_MAX]; // sat : 128 as 1.0
extern UINT16 wAngularBri[BASE_COLOR_MAX]; // bri : 128 as 0

// === STATIC VARIABLE ====================================================================
static Region DemoRec, b4ScalerDemoRec;
static INT8 bHMove=1, bVMove=1;

// === BODY ===============================================================================
UINT8 bDrvCheckPqReg(UINT32* pu4Table, UINT32 u4Size)
{
    UINT32 i, u4RegCheck = 0, u4MskCheck = 0;
    UINT32 uCnt = u4Size/12-1;

    for (i=0; i<uCnt; i++)
    {
        u4RegCheck += (pu4Table[i*3]>>(i&0xF));
        u4MskCheck += (pu4Table[i*3+2]>>(i&0xF));   
    }

    if ((u4RegCheck == pu4Table[i*3]) && (u4MskCheck == pu4Table[i*3+2]))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void vDrvSetPqReg(UINT32* pu4Table, UINT32 u4Size)
{
    UINT32 i;
    UINT32 uCnt = u4Size/12-1;

    for (i=0; i<uCnt; i++)
    {
        if (pu4Table[i*3] < VDOIN_BASE)
        {
            vIO32Write4BMsk(u4SWReg(pu4Table[i*3]), pu4Table[i*3+1], pu4Table[i*3+2]);
        }
        else
        {
        	vIO32Write4BMsk(pu4Table[i*3], pu4Table[i*3+1], pu4Table[i*3+2]);
    	}
    }
    
    return;
}

void vDrvGetPqReg(UINT32* pu4Table, UINT32 u4Size)
{
    UINT32 i;
    UINT32 uCnt = u4Size/12-1;

    for (i=0; i<uCnt; i++)
    {
        if (pu4Table[i*3] < VDOIN_BASE)
        {
            pu4Table[i*3+1] = u4IO32Read4B(u4SWReg(pu4Table[i*3]))&pu4Table[i*3+2];
        }
        else
        {
        	pu4Table[i*3+1] = u4IO32Read4B(pu4Table[i*3])&pu4Table[i*3+2];
    	}
    }
    
    return;
}

void vDrvPreprocHWInit(void)
{
    vRegWriteFldAlign(PAT_PIP_00,0,C_MAIN_SELFGEN_EN);
    vRegWriteFldAlign(TRANS_PIP_00,0x600, IN_OFFSET_1P);
// TODO
//    vRegWriteFldAlign(PAT_MAIN_02,1,C_DONT_KEEP_BLANK_MAIN);
//    vRegWriteFldAlign(PAT_PIP_02,1,C_DONT_KEEP_BLANK);    
    vRegWriteFldAlign(TRANS_MAIN_07, 0, CONTROL_SEL);
    vRegWriteFldAlign(TRANS_MAIN_07, 6, INPUT_SELP);
    vRegWriteFldAlign(TRANS_PIP_07, 0, CONTROL_SEL);
    vRegWriteFldAlign(TRANS_PIP_07, 6, INPUT_SELP);
    vRegWriteFldAlign(YCBCR2YC_MAIN_00, 0x0, C_MAIN_HSYNC_BLACK);
    vRegWriteFldAlign(YCBCR2YC_PIP_00, 0x0, C_MAIN_HSYNC_BLACK);
    vRegWriteFldAlign(DQ_00, 0x1, DQ_BYPASS);
    vRegWriteFldAlign(CB_00, 0x1, CB_BYPASS);
    vRegWriteFldAlign(DQ_00, 0x1, DQ_444_422_SEL);  // DQ always output 444
    vDrvAntiAliasingFilterOnOff(SV_VP_MAIN, SV_OFF);
    vDrvAntiAliasingFilterOnOff(SV_VP_PIP, SV_OFF);

}
void vDrvPreprocSWInit(void)
{
    vIO32WriteFldAlign(MATRIX_00, 0x0, IN_CSC_BLACK_LVL_M);
    vIO32WriteFldAlign(MATRIX_00, 0x0, IN_CSC_BLACK_LVL_P);
}


void vDrvPreprocInit(void)
{
    vDrvPreprocHWInit();
    vDrvPreprocSWInit();
}

void vDrvOstgHWInit(void)
{
    vDrvxvYCCSetClip(DRVCUST_OptGet(eFlagxvYCCClip));
    vDrvSetOstgOptPos(DRVCUST_OptGet(eOSOptionPos));
    vRegWriteFldAlign(YCBCR2RGB_00, 0x0, YUV2RGB_BYPASS_MAIN);
    vRegWriteFldAlign(YCBCR2RGB_00, 0x0, YUV2RGB_BYPASS_PIP);
    vRegWriteFldAlign(GAINOSET_00, 0x0, RGB_GAIN_SEL);      // Select RGB gain range 0~2
    vRegWriteFldAlign(GAMMA_00, 0, C_GM_SWAP_POST);         // default use pre-gamma (before local dimming)
#ifdef CC_MT5396
    vRegWriteFldAlign(OSTG_00, 0, C_DBUF_ON);         // 5396 disable OSTG double buffer
#endif    
    vDrvDBLHWInit();

}

void vDrvOstgSWInit(void)
{
    vDrvDBLSWInit();
    // Initial color filter value
    vIO32WriteFldAlign(COLOR_FILTER_00, 0x200, COLOR_FILTER_R_GAIN);
    vIO32WriteFldAlign(COLOR_FILTER_00, 0x200, COLOR_FILTER_G_GAIN);
    vIO32WriteFldAlign(COLOR_FILTER_00, 0x200, COLOR_FILTER_B_GAIN);

}

void vDrvGlobalPQSWInit(void)
{
    // initial PQ setting
    vDrvSetBrightness(SV_VP_MAIN, 0x80);
    vDrvSetContrast(SV_VP_MAIN, 0x80);
    vDrvSetSaturation(SV_VP_MAIN, 0x80);
    vDrvSetHue(SV_VP_MAIN, 0x80);
    vDrvSetBrightness(SV_VP_PIP, 0x80);
    vDrvSetContrast(SV_VP_PIP, 0x80);
    vDrvSetSaturation(SV_VP_PIP, 0x80);
    vDrvSetHue(SV_VP_PIP, 0x80);
    vApiRegisterVideoEvent(PE_EVENT_OD, SV_VP_MAIN, SV_ON);
}

void vDrvOstgInit(void)
{
    vDrvOstgHWInit();
    vDrvOstgSWInit();
    vDrvGlobalPQSWInit();
}

void vDrvYcprocInit(void)
{
    //Pre-Proc initialize
    vDrvPreprocInit();

    //Post-Proc initialize
    vDrvPreSharpInit();
    vDrvPostSharpInit();
    vDrvCTIFInit();
    vDrvCTIRInit();    
    vDrvLTIInit();
    vDrvSCEInit();
    vDrvADLInit();

    // Output-Stage initialize
    vDrvOstgInit();
    vDrvMeterInit();
#ifdef CC_MT5396
	vDrvCBEInit();
#endif


#if 0   // for IC verification, bypass PQ modules at initialization
    vDrvNRSetForceBypass(SV_ON);

    vDrvDeRingOnOff(SV_VP_MAIN, SV_OFF);
    vDrvUsPeiOnOff(SV_OFF);

    vHalVideoCTIOnOff(SV_VP_MAIN, SV_OFF);
    vHalVideoLTIOnOff(SV_VP_MAIN, SV_OFF);
    vHalVideoSharpnessOnOff(SV_VP_MAIN, SV_OFF);

    //vDrvCBEOnOff(SV_OFF);
    
    vDrvSCEAllOnOff(SV_OFF);
    vHalVideoBlueStretchOnOff(SV_VP_MAIN, SV_OFF);        
    vDrvxvYCCOnOff(SV_OFF);    
    vDrvSetLcDimOnOff(SV_OFF);
    vDrvGammaOnOff(SV_OFF);        
    vDrvODOnOff(SV_OFF);
    vDrvODBypass(SV_ON);

    vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_R, 0x200);
    vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_G, 0x200);
    vHalVideoRGBGain(SV_VP_MAIN, SV_RGB_B, 0x200);
    vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_R ,0);
    vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_G ,0);
    vHalVideoRGBOffset(SV_VP_MAIN, SV_RGB_B ,0);    
#endif

}

// ============================================================================
/**
 *  @brief Set filed signal resync with vsync when input is interlace.
 *  @param bPath SV_VP_MAIN/SV_VP_PIP
 */
void vDrvPreprocSetFieldResync(UINT8 bPath)
{
	UINT8 bFieldResync;

	if ((bGetVideoDecType(bPath) == SV_VD_MPEGHD) ||
	        (bDrvVideoIsSrcInterlace(bPath) == SV_FALSE)) // DTV or progressive timing, disable field prediction
	{
		bFieldResync = SV_FALSE;
	}
	else
	{
		bFieldResync = SV_TRUE;
	}

	LOG(3,"Set Field Resync to (%d)\n",bFieldResync);

	if (bPath == SV_VP_MAIN)
	{
        // TODO
		//vRegWriteFldAlign(PAT_MAIN_00, bFieldResync, C_MAIN_F_V_SHK);    //Field resync by Vsync when vsize is reduced.
	}
	else
	{
        // TODO
        //vRegWriteFldAlign(PAT_PIP_00, bFieldResync, C_PIP_F_V_SHK);    //Field resync by Vsync when vsize is reduced.
	}
}

void vDrvSetDQ(sDQConfig *sConfig)
{
    UINT32 u4PSCANFull,u4PSCANHalf,u4NRFull;
    UINT32 u4Height = sConfig->u2SrcHeight;

    u4Height = bDrvVideoIsSrcInterlace(SV_VP_MAIN) ? u4Height>>1 : u4Height;
    
    vRegWriteFldAlign(DQ_00, !sConfig->u1OnOff, DQ_BYPASS);
    vRegWriteFldAlign(DQ_09, sConfig->u2SrcWidth, SRCIMG_WIDTH);
    vRegWriteFldAlign(DQ_09, u4Height, SRCIMG_HEIGHT);
    vRegWriteFldAlign(DQ_00, sConfig->u1SrcMode, FS_SS_SEL);
    vRegWriteFldAlign(DQ_00, SV_OFF, DQ_DBUF);
    
    if (sConfig->u1X2Mode)
    {
        vRegWriteFldAlign(DQ_00, 0x1, DQ_LUMA_X2_MODE);
        vRegWriteFldAlign(DQ_00, 0x1, DQ_CHROMA_X2_MODE);
    }
    else
    {
        vRegWriteFldAlign(DQ_00, 0x0, DQ_LUMA_X2_MODE);
        vRegWriteFldAlign(DQ_00, 0x1, DQ_CHROMA_X2_MODE);
    }

    // TODO: SRAM MODE
    if(bDrvVideoIsSrcInterlace(SV_VP_MAIN))
    {
        if(u2DrvDIGetVdpWidth(SV_VP_MAIN)<960)
        {
            u4PSCANFull=SV_OFF;
            u4PSCANHalf=SV_ON;
            u4NRFull=SV_OFF;
        }
        else
        {
            u4PSCANFull=SV_OFF;
            u4PSCANHalf=SV_OFF;
            u4NRFull=SV_ON;
        }   
    }
    else
    {
        u4PSCANFull=SV_ON;
        u4PSCANHalf=SV_OFF;
        u4NRFull=SV_OFF;
    }

    
    vRegWriteFldAlign(DQ_00, u4PSCANFull, SRAM_PSCAN_FULL);
    vRegWriteFldAlign(DQ_00, u4PSCANHalf, SRAM_PSCAN_HALF);
    vRegWriteFldAlign(DQ_00, u4NRFull   , SRAM_NR_FULL);    
}

void vDrvSetCB(sCBConfig *sConfig)
{
    UINT32 u4Val = 0;
    u4Val = (sConfig->u1VI << 13) | ((!sConfig->u1OnOff) << 12) | (sConfig->u2SrcWidth);
    vRegWrite4B(CB_00, u4Val);
    vRegWrite4B(CB_01, 0);    
}

void vDrvSetLI(sLIConfig *sConfig)
{
#if defined(CC_MT5396)
	if ((BSP_GetIcVersion() >= IC_VER_5396_AB))
	{
    	vRegWriteFldAlign(SCPIP_HDS_01, sConfig->u1OnOff, SCPIP_HDS_01_MAIN_LINE_MERGE_EN);
	}
#endif
}

/*****************************************************************************************/
/**********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/
void vDrvDemoSetRegion(Region rDemoRegion)
{
    if((rDemoRegion.wHEnd<=rDemoRegion.wHStart)||(rDemoRegion.wVEnd<=rDemoRegion.wVStart))
    {
        printf("Invalid Demo Width or Demo Height Set !!\n");
    }
    vIO32WriteFldAlign(DEMO_00, rDemoRegion.wHStart, DEMOMODE_HSTART);
    vIO32WriteFldAlign(DEMO_00, rDemoRegion.wVStart, DEMOMODE_VSTART);
    vIO32WriteFldAlign(DEMO_01, (rDemoRegion.wHEnd-rDemoRegion.wHStart), DEMOMODE_WIDTH);
    vIO32WriteFldAlign(DEMO_01, (rDemoRegion.wVEnd-rDemoRegion.wVStart), DEMOMODE_HEIGHT); 
    vIO32WriteFldAlign(DEMO_00, rDemoRegion.u1OnOff, DEMOMODE_ENABLE);
}

void vDrvGetb4ScalerRec(void)
{
    RDispPrm* dispPrm;    
    UINT16 wHPorch, wVPorch;
    UINT32 u4PreDownScale = u4ScpipGetHwHPS(SV_VP_MAIN);
    dispPrm = getDispPrm(SV_VP_MAIN); 
    
    if((dispPrm==NULL) || 
       (DemoRec.wHEnd<DemoRec.wHStart) || 
       (DemoRec.wVEnd<DemoRec.wVStart))
    {
        printf("Error parameters of demo region ! \n");
        return;  // fix klocwork
	}

    ASSERT(dispPrm->u4DSclH && dispPrm->u4DSclV);

    //In display mode, overscan is clipped by Regen, others by Scaler.
    wHPorch = (u1ScpipGetDispMode(SV_VP_MAIN)) ? 0 : getMixedHPorch(SV_VP_MAIN);
    wVPorch = (u1ScpipGetDispMode(SV_VP_MAIN)) ? 0 : getMixedVPorch(SV_VP_MAIN);
     
    //Transform demo region in panel size base to demo region in input image size base.
    b4ScalerDemoRec.wHStart = (UINT32)DemoRec.wHStart*dispPrm->u4USclH/0x8000*
                               0x8000/dispPrm->u4DSclH + wHPorch*u4PreDownScale/0x8000;
    b4ScalerDemoRec.wVStart = (UINT32)DemoRec.wVStart*dispPrm->u4USclV/0x8000*
                               0x8000/dispPrm->u4DSclV + wVPorch;
    b4ScalerDemoRec.wHEnd = b4ScalerDemoRec.wHStart + (DemoRec.wHEnd-DemoRec.wHStart)*
                               dispPrm->u4USclH/0x8000*0x8000/dispPrm->u4DSclH;
    b4ScalerDemoRec.wVEnd = b4ScalerDemoRec.wVStart + (DemoRec.wVEnd-DemoRec.wVStart)*
                               dispPrm->u4USclV/0x8000*0x8000/dispPrm->u4DSclV;
}

void vDrvDemoRecValidation(void)
{
    UINT16 wWidth = u2ScpipGetDispWidth(SV_VP_MAIN);
    UINT16 wHeight = u2ScpipGetDispHeight(SV_VP_MAIN);

    // region Validation.
    if(IO32ReadFldAlign(DEMO_01, DEMOMODE_WIDTH)>wWidth)
    {
        vIO32WriteFldAlign(DEMO_01, wWidth, DEMOMODE_WIDTH);
    }
    if(IO32ReadFldAlign(DEMO_01, DEMOMODE_HEIGHT)>wHeight)
    {
        vIO32WriteFldAlign(DEMO_01, wHeight, DEMOMODE_HEIGHT);
    }
    if(DemoRec.wHEnd>=wWidth)
    {
        DemoRec.wHStart = wWidth-IO32ReadFldAlign(DEMO_01, DEMOMODE_WIDTH);
        DemoRec.wHEnd = wWidth;
    }
    if(DemoRec.wVEnd>=wHeight)
    {
        DemoRec.wVStart = wHeight-IO32ReadFldAlign(DEMO_01, DEMOMODE_HEIGHT);
        DemoRec.wVEnd = wHeight;
    }    
}

void vDrvDemoRecMoving(void)
{
    UINT16 wWidth = u2ScpipGetDispWidth(SV_VP_MAIN);
    UINT16 wHeight = u2ScpipGetDispHeight(SV_VP_MAIN);
    
    bHMove = (DemoRec.wHStart==0) ? 1 : (DemoRec.wHEnd==wWidth) ? -1 : bHMove;
    bVMove = (DemoRec.wVStart==0) ? 1 : (DemoRec.wVEnd==wHeight) ? -1 : bVMove;
    vIO32WriteFldAlign(DEMO_00, DemoRec.wHStart+bHMove, DEMOMODE_HSTART);
    vIO32WriteFldAlign(DEMO_00, DemoRec.wVStart+bVMove, DEMOMODE_VSTART);
}

void vDrvDemoProc(void)
{
    UINT8 bOnOff = IO32ReadFldAlign(DEMO_00, DEMOMODE_ENABLE);
    UINT8 bMovingOnOff = IO32ReadFldAlign(DEMO_00, MOVINGEMO_ENABLE);    

    if((DemoRec.u1OnOff!=bOnOff)||
        (DemoRec.wHStart!=IO32ReadFldAlign(DEMO_00, DEMOMODE_HSTART))||
        (DemoRec.wVStart!=IO32ReadFldAlign(DEMO_00, DEMOMODE_VSTART))||
        (DemoRec.wHEnd!=(IO32ReadFldAlign(DEMO_00, DEMOMODE_HSTART)+IO32ReadFldAlign(DEMO_01, DEMOMODE_WIDTH)))||
        (DemoRec.wVEnd!=(IO32ReadFldAlign(DEMO_00, DEMOMODE_VSTART)+IO32ReadFldAlign(DEMO_01, DEMOMODE_HEIGHT))))
    {
        DemoRec.u1OnOff = bOnOff;
        DemoRec.wHStart = IO32ReadFldAlign(DEMO_00, DEMOMODE_HSTART);
        DemoRec.wVStart = IO32ReadFldAlign(DEMO_00, DEMOMODE_VSTART);
        DemoRec.wHEnd = IO32ReadFldAlign(DEMO_00, DEMOMODE_HSTART)+IO32ReadFldAlign(DEMO_01, DEMOMODE_WIDTH);
        DemoRec.wVEnd = IO32ReadFldAlign(DEMO_00, DEMOMODE_VSTART)+IO32ReadFldAlign(DEMO_01, DEMOMODE_HEIGHT);
        vDrvDemoRecValidation();
        vDrvGetb4ScalerRec();

        vDrvColorDemo(bOnOff, DemoRec);
        //vDrv3DNRDemo(bOnOff, b4ScalerDemoRec);
        vDrv2DSharpDemo(bOnOff, DemoRec);
        vDrvECTIDemo(bOnOff, DemoRec);         
        vDrvSetPostSharpDemoRange(DemoRec);        
    }
    
    if(bMovingOnOff==SV_ON)
    {    
        vDrvDemoRecMoving();
    }    
}

/*****************************************************************************************/
/*************************************** General  *****************************************/
/*****************************************************************************************/
UINT8 bDrvGetPeUiDebugMode(void)
{
    return IO32ReadFldAlign(PEUI_00, PEUI_DEBUG_EN);
}

void vDrvSetPeUiDebugMode(UINT8 bOnOff)
{
    vIO32WriteFldAlign(PEUI_00, bOnOff, PEUI_DEBUG_EN);
}

void vDrvPreprocEnablePattern(UINT8 bPath,UINT8 bPattern)
{
    if (bPath == SV_VP_MAIN)
    {
        // TODO
        vRegWriteFldAlign(PAT_MAIN_02, bPattern, C_MAIN_PAT_TYPE);
    }
    else
    {
        // TODO
        vRegWriteFldAlign(PAT_PIP_02, bPattern, C_MAIN_PAT_TYPE);
    }
}

/*****************************************************************************************/
/******************************   YCPROC MAIN LOOP   ***************************************/
/*****************************************************************************************/

static void _vDrvSCEParialLoadProc(void)
{
    UINT32 u4Temp = u4IO32Read4B(SCE_SW_LOAD);
    
    if (u4Temp&Fld2Msk32(SCE_SW_LOAD_TRIG))
    {
        vDrvSCELoadIdx(
            (u4Temp&Fld2Msk32(SCE_SW_LOAD_FUNC_IDX))>>Fld_shft(SCE_SW_LOAD_FUNC_IDX),
            (u4Temp&Fld2Msk32(SCE_SW_LOAD_HUE_IDX))>>Fld_shft(SCE_SW_LOAD_HUE_IDX),
            (u4Temp&Fld2Msk32(SCE_SW_LOAD_VALUE))>>Fld_shft(SCE_SW_LOAD_VALUE));
        vIO32WriteFldAlign(SCE_SW_LOAD, 0, SCE_SW_LOAD_TRIG);
    }
}

void vDrvGlobalPQAdj(void)
{
    // Global PQ adjustment
    UINT8 bPath = 0, bOffset, bDemoToggle;
    static UINT8 bDemoModeOnOff = 0xFF;
    UINT32 u4MatrixPQAdj = IO32ReadFldAlign(MATRIX_00, MATRIX_PQ_ADJ);
    UINT16 wBri,wCon,wSat,wHue;  //add for vestel
    
    UINT16 wRGain,wGGain,wBGain; //add for vestel(RGB Gain)
    
    //Begin Add for vestel --- pingan 0606
    bOffset = (bPath ==SV_VP_MAIN) ? 0 : 0x4;
    wBri = IO32ReadFldAlign(GLOBAL_ADJ_00 + bOffset, BRIGHT_DUMMY);
    wCon = IO32ReadFldAlign(GLOBAL_ADJ_02 + bOffset, CONT_DUMMY);
    wSat = IO32ReadFldAlign(GLOBAL_ADJ_02 + bOffset, SAT_DUMMY);
    wHue = IO32ReadFldAlign(GLOBAL_ADJ_00 + bOffset, HUE_DUMMY);

    if(u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF)
    {
        wBri = IO32ReadFldAlign(GLOBAL_ADJ_04 + bOffset, BRIGHT_DUMMY_3D);
        wCon = IO32ReadFldAlign(GLOBAL_ADJ_06 + bOffset, CONT_DUMMY_3D);
        wSat = IO32ReadFldAlign(GLOBAL_ADJ_06 + bOffset, SAT_DUMMY_3D);
        wHue = IO32ReadFldAlign(GLOBAL_ADJ_04 + bOffset, HUE_DUMMY_3D);
    }

    //for (bPath = SV_VP_MAIN; bPath < SV_VP_NA; bPath++)
    {
        bDemoToggle = (IO32ReadFldAlign(DEMO_00, DEMOMODE_ENABLE) != bDemoModeOnOff) ? 1 : 0; 
        bDemoModeOnOff = IO32ReadFldAlign(DEMO_00, DEMOMODE_ENABLE);
        
        if ((bPath == SV_VP_MAIN) && IO32ReadFldAlign(PEUI_00, PEUI_OS_CSC_MODE_CHANGE_M)) 
        {
            vDrvCalVideoColorMatrix(SV_VP_MAIN, 0x80, 0x80, 0x80, 0x80);
            vIO32WriteFldAlign(PEUI_00, 0, PEUI_OS_CSC_MODE_CHANGE_M);
        }

        if ((bPath == SV_VP_PIP) && IO32ReadFldAlign(PEUI_00, PEUI_OS_CSC_MODE_CHANGE_P)) 
        {
            vDrvCalVideoColorMatrix(SV_VP_PIP, 0x80, 0x80, 0x80, 0x80);
            vIO32WriteFldAlign(PEUI_00, 0, PEUI_OS_CSC_MODE_CHANGE_P);
        }           
        
        if ((u4MatrixPQAdj & !bDemoModeOnOff) ||
            (bPath == SV_VP_PIP) || (ISSCALER444(bPath)))
        {
            //bDemoToggle = (IO32ReadFldAlign(DEMO_00, DEMOMODE_ENABLE) != bDemoModeOnOff) ? 1 : 0;
            //bOffset = (bPath ==SV_VP_MAIN) ? 0 : 0x4;
            
            if ((wBri != u2PQItemMatrix[bPath][PQ_BRIGHTNESS]) ||
                (wCon != u2PQItemMatrix[bPath][PQ_CONTRAST]) ||
                (wSat != u2PQItemMatrix[bPath][PQ_SATURATION]) ||
                (wHue != u2PQItemMatrix[bPath][PQ_HUE]) ||
                bDemoToggle)
            {
                vDrvVideoColorMatrix(bPath);
                //bDemoModeOnOff = IO32ReadFldAlign(DEMO_00, DEMOMODE_ENABLE);
                
                vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, 0x400, BRIGHT);
                vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, 0x200, HUE);
                vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, 0x80, SAT);
                vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, 0x80, CONT);

                // Blue stretch
                if (bPath == SV_VP_MAIN)
                {
                    vDrvSetBlueStretchBase();
                }
            }
        }
        else
        {
            if ((wBri != u2PQItemMatrix[bPath][PQ_BRIGHTNESS]) ||
                (wCon != u2PQItemMatrix[bPath][PQ_CONTRAST]) ||
                (wSat != u2PQItemMatrix[bPath][PQ_SATURATION]) ||
                (wHue != u2PQItemMatrix[bPath][PQ_HUE]) ||
                bDemoToggle)
            {
                vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, 
                    0x400 + ((wBri - 0x80) << IO32ReadFldAlign(SCE_REG_00, SCE_BRI_INTERVAL)), 
                    BRIGHT);
                vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, 
                    0x200 + ((wHue - 0x80) << IO32ReadFldAlign(SCE_REG_00, SCE_HUE_INTERVAL)), 
                    HUE);
                vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, wSat, SAT);
                vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, wCon, CONT);
                
                vDrvCalVideoColorMatrix(bPath, 0x80, 0x80, 0x80, 0x80);
                
                u2PQItemMatrix[bPath][PQ_BRIGHTNESS] = wBri;               
                u2PQItemMatrix[bPath][PQ_CONTRAST] = wCon;
                u2PQItemMatrix[bPath][PQ_SATURATION] = wSat;
                u2PQItemMatrix[bPath][PQ_HUE] = wHue;
            }

            // Blue stretch
            if (bPath == SV_VP_MAIN)
            {
                vDrvSetBlueStretchBase();
            }
        }
    }

    if (IO32ReadFldAlign(PEUI_00, PEUI_IN_CSC_MODE_CHANGE_M))
    {
        vVdoSetColorTransform(SV_VP_MAIN);
        vIO32WriteFldAlign(PEUI_00, SV_OFF, PEUI_IN_CSC_MODE_CHANGE_M); 
    }

    if (IO32ReadFldAlign(PEUI_00, PEUI_IN_CSC_MODE_CHANGE_P))
    {
        vVdoSetColorTransform(SV_VP_PIP);
        vIO32WriteFldAlign(PEUI_00, SV_OFF, PEUI_IN_CSC_MODE_CHANGE_P); 
    }


    if(u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF)
    {
        wRGain = IO32ReadFldAlign(GLOBAL_RGB_GAIN_3D, R_GAIN_3D);
        wGGain = IO32ReadFldAlign(GLOBAL_RGB_GAIN_3D, G_GAIN_3D);
        wBGain = IO32ReadFldAlign(GLOBAL_RGB_GAIN_3D, B_GAIN_3D);
    }
    else
    {
        wRGain = IO32ReadFldAlign(GLOBAL_RGB_GAIN, R_GAIN);
        wGGain = IO32ReadFldAlign(GLOBAL_RGB_GAIN, G_GAIN);
        wBGain = IO32ReadFldAlign(GLOBAL_RGB_GAIN, B_GAIN);
    }
    
    if(wRGain != RegReadFldAlign(GAINOSET_00, R_GAIN_MAIN))
    {
        //vApiRegisterVideoEvent(PE_EVENT_R_GAIN, SV_VP_MAIN, SV_ON);
        //vHalVideoRGBGain(bPath, SV_RGB_R, wReadQualityTable(QUALITY_OSD_MAPPING_R_GAIN));
    }
    if(wGGain != RegReadFldAlign(GAINOSET_00, G_GAIN_MAIN))
    {
        //vApiRegisterVideoEvent(PE_EVENT_G_GAIN, SV_VP_MAIN, SV_ON);
        //vHalVideoRGBGain(bPath, SV_RGB_G, wReadQualityTable(QUALITY_OSD_MAPPING_R_GAIN));
    }
    if(wBGain != RegReadFldAlign(GAINOSET_00, B_GAIN_MAIN))
    {
        //vApiRegisterVideoEvent(PE_EVENT_B_GAIN, SV_VP_MAIN, SV_ON);
        //vHalVideoRGBGain(bPath, SV_RGB_B, wReadQualityTable(QUALITY_OSD_MAPPING_R_GAIN));
    }
    //end

}

void vDrvSCEProc(void)
{
    static UINT8 bPreSceGain[3] = {0x80, 0x80, 0x80};
    static UINT8 bPreSkinTable[3] = {0x80, 0x80, 0x80};
    static UINT8 bPreSce3dGain[2] = {0x80, 0x80};
    static UINT16 wPreAngularHue[BASE_COLOR_MAX] = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80}; 
    static UINT16 wPreAngularSat[BASE_COLOR_MAX] = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80};
    static UINT16 wPreAngularBri[BASE_COLOR_MAX] = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80};

#ifdef CC_MT5396  
    vApiRegisterVideoEvent(PE_EVENT_OUT_3x3, SV_VP_MAIN, SV_ON);
#else
    vDrvGlobalPQAdj();
#endif

    //SCE cannot be enabled under VGA signal
    if (ISSCALER444(SV_VP_MAIN))
    {
        return;
    }

    bSceGain[0] = IO32ReadFldAlign(ADAPTIVE_SCE_REG, ADAPTIVE_SCE_L);
    bSceGain[1] = IO32ReadFldAlign(ADAPTIVE_SCE_REG, ADAPTIVE_SCE_S);
    bSceGain[2] = IO32ReadFldAlign(ADAPTIVE_SCE_REG, ADAPTIVE_SCE_H);
    bSkinTable[0] = IO32ReadFldAlign(ADAPTIVE_SKIN_REG, ADAPTIVE_SKIN_L);
    bSkinTable[1] = IO32ReadFldAlign(ADAPTIVE_SKIN_REG, ADAPTIVE_SKIN_S);
    bSkinTable[2] = IO32ReadFldAlign(ADAPTIVE_SKIN_REG, ADAPTIVE_SKIN_H);
    bSce3dGain[0] = IO32ReadFldAlign(TDTV_FW_00, SCE_3D_GAIN_00);
    bSce3dGain[1] = IO32ReadFldAlign(TDTV_FW_00, SCE_3D_GAIN_01);
    
    vDrvGetSixColorReg();
        
    if ((IO32ReadFldAlign(ADAPTIVE_SCE_REG, ADAPTIVE_SCE_FORCE_LOAD) == SV_ON) 
        || (bPreSceGain[0] != bSceGain[0]) || (bPreSceGain[1] != bSceGain[1]) || (bPreSceGain[2] != bSceGain[2])
        || (bPreSkinTable[0] != bSkinTable[0]) || (bPreSkinTable[1] != bSkinTable[1]) || (bPreSkinTable[2] != bSkinTable[2])
        || (((bPreSce3dGain[0] != bSce3dGain[0]) || (bPreSce3dGain[1] != bSce3dGain[1])) && (u4DrvTDTV3DModeQuery() != E_TDTV_UI_3D_MODE_OFF))
        || (wPreAngularHue[0] != wAngularHue[0]) || (wPreAngularHue[1] != wAngularHue[1]) || (wPreAngularHue[2] != wAngularHue[2])
        || (wPreAngularHue[3] != wAngularHue[3]) || (wPreAngularHue[4] != wAngularHue[4]) || (wPreAngularHue[5] != wAngularHue[5])
        || (wPreAngularSat[0] != wAngularSat[0]) || (wPreAngularSat[1] != wAngularSat[1]) || (wPreAngularSat[2] != wAngularSat[2])
        || (wPreAngularSat[3] != wAngularSat[3]) || (wPreAngularSat[4] != wAngularSat[4]) || (wPreAngularSat[5] != wAngularSat[5])
        || (wPreAngularBri[0] != wAngularBri[0]) || (wPreAngularBri[1] != wAngularBri[1]) || (wPreAngularBri[2] != wAngularBri[2])
        || (wPreAngularBri[3] != wAngularBri[3]) || (wPreAngularBri[4] != wAngularBri[4]) || (wPreAngularBri[5] != wAngularBri[5]))
    {
        bPreSceGain[0] = bSceGain[0]; 
        bPreSceGain[1] = bSceGain[1]; 
        bPreSceGain[2] = bSceGain[2]; 
        bPreSkinTable[0] = bSkinTable[0]; 
        bPreSkinTable[1] = bSkinTable[1]; 
        bPreSkinTable[2] = bSkinTable[2]; 
        bPreSce3dGain[0] = bSce3dGain[0];
        bPreSce3dGain[1] = bSce3dGain[1];
        
        wPreAngularHue[0] = wAngularHue[0];
        wPreAngularHue[1] = wAngularHue[1];
        wPreAngularHue[2] = wAngularHue[2];
        wPreAngularHue[3] = wAngularHue[3];
        wPreAngularHue[4] = wAngularHue[4];
        wPreAngularHue[5] = wAngularHue[5];
        wPreAngularSat[0] = wAngularSat[0];
        wPreAngularSat[1] = wAngularSat[1];
        wPreAngularSat[2] = wAngularSat[2];
        wPreAngularSat[3] = wAngularSat[3];
        wPreAngularSat[4] = wAngularSat[4];
        wPreAngularSat[5] = wAngularSat[5];
        wPreAngularBri[0] = wAngularBri[0];
        wPreAngularBri[1] = wAngularBri[1];
        wPreAngularBri[2] = wAngularBri[2];
        wPreAngularBri[3] = wAngularBri[3];
        wPreAngularBri[4] = wAngularBri[4];
        wPreAngularBri[5] = wAngularBri[5];
        
        vDrvSCECalculate();
        vIO32WriteFldAlign(ADAPTIVE_SCE_REG,SV_OFF, ADAPTIVE_SCE_FORCE_LOAD);
        vApiRegisterVideoEvent(PE_EVENT_SCE, SV_VP_MAIN, SV_ON);
    }

    _vDrvSCEParialLoadProc();
}

void vDrvMMAPPorc(void)
{
    if (IO32ReadFldAlign(MMAPP_04, APP_CHANGE_NOTIFY) == SV_ON)
    {
        vIO32WriteFldAlign(MMAPP_04, SV_OFF, APP_CHANGE_NOTIFY);

        sMMAppInfo.u1AppType = VDP_GetPlayMode(SV_VP_MAIN);
        //sMMAppInfo.u1AppType = IO32ReadFldAlign(MMAPP_00, APP_TYPE);
        
        if (sMMAppInfo.u1AppType == 0xFF) /*Pretend for Netflix App */
        {
            sMMAppInfo.u2SrcWidth = IO32ReadFldAlign(MMAPP_01, APP_SRC_WIDTH);
            sMMAppInfo.u2SrcHeight = IO32ReadFldAlign(MMAPP_01, APP_SRC_HEIGHT);
            sMMAppInfo.u2OutWidth = IO32ReadFldAlign(MMAPP_02, APP_OUT_WIDTH);
            sMMAppInfo.u2OutHeight = IO32ReadFldAlign(MMAPP_02, APP_OUT_HEIGHT);
            sMMAppInfo.u4BitRate = IO32ReadFldAlign(MMAPP_03, APP_BITRATE);
            DRVCUST_MMAPProc();
        }
    }
}


#define OD_STATE_CNT 5
#define DI_STATE_CNT 3
EXTERN void HalFlushInvalidateDCache(void);

void vDrvDumpShrink(void)
{   
    UINT32 i, j;
    UINT32 pos_x, pos_y;
    UINT32 ratio_x, ratio_y;
    UINT32* pBuf = (UINT32 *)VIRTUAL(u4RegRead4B(SWRW_01));
    UINT32 u4SrcWidth = RegReadFldAlign(SWRW_03, DUMP_CTRL_WIDTH);
    UINT32 u4SrcHeight = RegReadFldAlign(SWRW_03, DUMP_CTRL_HEIGHT);
    UINT32 u4DesWidth = RegReadFldAlign(SWRW_05, DUMP_CTRL_SHRINK_W);
    UINT32 u4DesHeight = RegReadFldAlign(SWRW_05, DUMP_CTRL_SHRINK_H);
    
    LOG(4, "OD Dump shrink W %d  H %d", u4DesWidth, u4DesHeight);
    
    if (pBuf == NULL)
    {        
        LOG(0, "Dump Shrink error");        
        return;
    }

    if ((u4DesWidth == 0) || (u4DesHeight == 0))
    {        
        LOG(4, "Dump Shrink", u4DesWidth, u4DesHeight);        
        return;
    }

    if ((u4DesWidth >= u4SrcWidth) || (u4DesHeight >= u4SrcHeight))
    {
        LOG(4, "Dump Not support enlarge function");
        return;
    }

    ratio_x = (u4SrcWidth<<8)/u4DesWidth;
    ratio_y = (u4SrcHeight<<8)/u4DesHeight;
    
    for (j=0; j<u4DesHeight; j++)
    {
        pos_y = (j*ratio_y)>>8;
        
        for (i=0; i<u4DesWidth; i++)
        {            
            pos_x = (i*ratio_x)>>8;
            
            pBuf[j*u4DesWidth+i] = pBuf[pos_y*u4SrcWidth+pos_x];
        }
    }           
    
    vRegWriteFldAlign(SWRW_03, u4DesWidth, DUMP_CTRL_WIDTH);
    vRegWriteFldAlign(SWRW_03, u4DesHeight, DUMP_CTRL_HEIGHT);
    vRegWriteFldAlign(SWRW_04, u4DesWidth*u4DesHeight*4, DUMP_CTRL_SIZE); 
}

void vDrvDumpPostProc(void)
{     
    HalFlushInvalidateDCache();  
    vDrvDumpShrink();      
    HalFlushInvalidateDCache();

    // finish dump
    vRegWriteFldAlign(SWRW_04, 0, DUMP_CTRL_STATE);
    vRegWriteFldAlign(SWRW_04, DUMP_ENG_NONE, DUMP_CTRL_ENG_SEL);  
}


void vDrvDumpProc(void)
{
    UINT32 u4State = RegReadFldAlign(SWRW_04, DUMP_CTRL_STATE);
    UINT32 u4EngSel = RegReadFldAlign(SWRW_04, DUMP_CTRL_ENG_SEL);
    static UINT32 u4DumpWaitCnt = 0;

    if (u4State == DUMP_STATE_INIT)
    {
        if (u4EngSel == DUMP_ENG_OD)
        {            
            vDrvOdDumpInit();
            u4DumpWaitCnt = OD_STATE_CNT;            
        }
        else if (u4EngSel == DUMP_ENG_PSCAN)        
        {            
            vDrvDIDumpInit(VDP_1);
            u4DumpWaitCnt = DI_STATE_CNT;            
        }
        else
        {            
            u4DumpWaitCnt = 0;
            vRegWriteFldAlign(SWRW_04, DUMP_STATE_INIT, DUMP_CTRL_STATE);
            return;
        }       
        
        vRegWriteFldAlign(SWRW_04, DUMP_STATE_START, DUMP_CTRL_STATE);
    }   
    else if (u4State == DUMP_STATE_START)
    {        
        if (u4DumpWaitCnt == 0)
        {
            if (u4EngSel == DUMP_ENG_OD)
            {            
                vDrvOdDumpStart();
                u4DumpWaitCnt = OD_STATE_CNT;
            }        
            else if (u4EngSel == DUMP_ENG_PSCAN)        
            {            
                vDrvDIDumpStart(VDP_1);
                u4DumpWaitCnt = DI_STATE_CNT;
            }        
            else
            {                           
                u4DumpWaitCnt = 0;                
                vRegWriteFldAlign(SWRW_04, DUMP_ENG_NONE, DUMP_CTRL_ENG_SEL);
                vRegWriteFldAlign(SWRW_04, DUMP_STATE_INIT, DUMP_CTRL_STATE);
                return;
            }
        
            vRegWriteFldAlign(SWRW_04, DUMP_STATE_END, DUMP_CTRL_STATE);
        }
        else
        {
            //Printf("Start wait count %d \n", u4DumpWaitCnt);
            u4DumpWaitCnt = (u4DumpWaitCnt > 0) ? (u4DumpWaitCnt-1) : 0;
        }
    }
    else if (u4State == DUMP_STATE_END)
    {           
        if (u4DumpWaitCnt == 0)
        {
            if (u4EngSel == DUMP_ENG_OD)
            {
                vDrvOdDumpEnd();
                vDrvDumpPostProc();
            }
            else if (u4EngSel == DUMP_ENG_PSCAN)
            {
                vDrvDIDumpEnd(VDP_1);
                vDrvDumpPostProc();
            }
            
            vRegWriteFldAlign(SWRW_04, DUMP_ENG_NONE, DUMP_CTRL_ENG_SEL);
            vRegWriteFldAlign(SWRW_04, DUMP_STATE_INIT, DUMP_CTRL_STATE);
        }
        else
        {
            //Printf("End wait count %d \n", u4DumpWaitCnt);
            u4DumpWaitCnt = (u4DumpWaitCnt > 0) ? (u4DumpWaitCnt-1) : 0;
        }            
    }
}

void vDrvYcprocProc(void)
{
    // PQ SRAM Stress Test Procedure
#ifndef NDEBUG
    vDrvPQStressProc();
#endif // NDEBUG
    
    vDrvMeterProc();
    
    vDrvSCEProc();

    vDrvSharpProc();

    //vDrvOPCProc();	

    vDrvDemoProc();

    vDrvMMAPPorc();

    //vDrvADLProc();    

    vDrvLcdimProc();

    vDrvDumpProc();

}

