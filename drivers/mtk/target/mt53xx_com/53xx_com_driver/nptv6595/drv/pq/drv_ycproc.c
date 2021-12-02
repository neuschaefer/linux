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
#include "drv_meter.h"
#include "x_assert.h"
#include "b2r_drvif.h"
#include "drv_upscaler.h"
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

// === GLOBAL VARIABLE ====================================================================
MMAppInfo sMMAppInfo; 

// === EXTERN VARIABLE ====================================================================
extern UINT16 u2PQItemMatrix[SV_VP_NA][PQ_MAX];
extern UINT8 bSceGain[3];
extern UINT8 bSkinTable[3];
extern UINT8 _bMAIN_COLOR_SYS;
extern UINT8 _bPIP_COLOR_SYS;

// === STATIC VARIABLE ====================================================================
#if PREPROC_ENABLE_SYNC_REGEN
static ClipParam _arClipPrm[2];
#endif

#define IS_MPEG_420_INTERLACE(u1Id)       (bDrvVideoIsSrcInterlace(u1Id) && (bGetVideoDecType(u1Id) == SV_VD_MPEGHD))

// === END OF STATIC VARIABLE =============================================================

// === BODY ===============================================================================
/**
 * @brief initialize YCPROC module
 */
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

void vDrvPreprocInit(void)
{
    //Set 5365 default value
    vRegWriteFldAlign(PAT_PIP_00,0,C_PIP_SELFGEN_ENABLE);
    vRegWriteFldAlign(TRANS_PIP_00,0x600, IN_OFFSET_1P);
    vRegWriteFldAlign(PAT_MAIN_02,1,C_DONT_KEEP_BLANK_MAIN);
    vRegWriteFldAlign(PAT_PIP_02,1,C_DONT_KEEP_BLANK);    
    vRegWriteFldAlign(YCBCR2YC_MAIN_00, 0x0, HSYNC_BLACK);
    vRegWriteFldAlign(YCBCR2YC_PIP_00, 0x0, HSYNC_BLACK);
    vDrvChromaCoringInit();
    vDrvAntiAliasingFilterOnOff(SV_VP_MAIN, SV_OFF);
    vDrvAntiAliasingFilterOnOff(SV_VP_PIP, SV_OFF);
}

void vDrvOstgInit(void)
{
    vDrvxvYCCSetClip(DRVCUST_OptGet(eFlagxvYCCClip));
    vDrvSetOstgOptPos(DRVCUST_OptGet(eOSOptionPos));
    vRegWriteFldAlign(OS_84, 0x0, YUV2RGB_BYPASS_MAIN);
    vRegWriteFldAlign(OS_84, 0x0, YUV2RGB_BYPASS_PIP);
    vRegWriteFldAlign(OS_2D, 0x0, MAIN_RGB_OFFSET_BYPASS);
    vRegWriteFldAlign(OS_2E, 0x0, PIP_RGB_OFFSET_BYPASS);
    vRegWriteFldAlign(OS_7A, 0x0, M_RGB_GAIN_SEL);      // Select RGB gain range 0~2
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
    vDrvSCEInit(SV_VP_MAIN);

    // Output-Stage initialize
    vDrvOstgInit();
    vDrvMeterInit();
    
    //vDrvDitherInit(LFSR_DITHER); Dither is controled by Panel Table, not fix value
#ifdef CC_MT5395
	vDrvCBEInit();
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
		vRegWriteFldAlign(PAT_MAIN_00, bFieldResync, C_MAIN_F_V_SHK);    //Field resync by Vsync when vsize is reduced.
	}
	else
	{
		vRegWriteFldAlign(PAT_PIP_00, bFieldResync, C_PIP_F_V_SHK);    //Field resync by Vsync when vsize is reduced.
	}
}

#if PREPROC_ENABLE_SYNC_REGEN
void PROC_WRITE_FLD(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Value, UINT32 u4Fld)
{
	vRegWriteFldAlign((u2Addr + ((u1VdpId == SV_VP_MAIN) ? 0 : 0x200)), u4Value, u4Fld);
}

UINT32 PROC_READ_FLD(UINT8 u1VdpId, UINT16 u2Addr, UINT32 u4Fld)
{
	return RegReadFldAlign((u2Addr + ((u1VdpId == SV_VP_MAIN) ? 0 : 0x200)), u4Fld);
}

void vDrvRegenClip(UINT8 bPath)
{
    #ifdef CC_SCPOS_3DTV_SUPPORT
    //for E2 IC ONLY
    UINT16 LStart;
    UINT16 RStart;
    LStart = (_arClipPrm[bPath].wVEnd / 2) + 1;
    RStart = (_arClipPrm[bPath].wVEnd / 2) + 2;
    
    if(vDrvNRGetTDTVDatamode() == E_TDTV_NR_SYNC_PADDING && (BSP_GetIcVersion()>=IC_VER_5395_AA) && IS_IC_5395())
    {
        if(PROC_READ_FLD(bPath, PAT_MAIN_02, C_MAIN_SELF_VSTART) != LStart ||
           PROC_READ_FLD(bPath, PAT_MAIN_02, C_MAIN_SELF_VEND) != RStart)
        {
    		PROC_WRITE_FLD(bPath, PAT_MAIN_02, LStart, C_MAIN_SELF_VSTART);
    		PROC_WRITE_FLD(bPath, PAT_MAIN_02, RStart, C_MAIN_SELF_VEND);
    		//enable regen
    		PROC_WRITE_FLD(bPath, PAT_MAIN_00, 0x0, C_MAIN_SYNC_MODIFY);            
        }                 
		return;
    }
    #endif

	if ((_arClipPrm[bPath].u1OnOff) &&
	        ((PROC_READ_FLD(bPath, PAT_MAIN_01, C_MAIN_SELF_HSTART) != _arClipPrm[bPath].wHStart) ||
	         (PROC_READ_FLD(bPath, PAT_MAIN_01, C_MAIN_SELF_HEND) != _arClipPrm[bPath].wHEnd) ||
	         (PROC_READ_FLD(bPath, PAT_MAIN_02, C_MAIN_SELF_VSTART) != _arClipPrm[bPath].wVStart) ||
	         (PROC_READ_FLD(bPath, PAT_MAIN_02, C_MAIN_SELF_VEND) != _arClipPrm[bPath].wVEnd)))
	{	       	
        //disable regen first
        PROC_WRITE_FLD(bPath, PAT_MAIN_00, 0x0, C_MAIN_SYNC_MODIFY);
        //X offset
        PROC_WRITE_FLD(bPath, PAT_MAIN_01, _arClipPrm[bPath].wHStart, (bPath == SV_VP_MAIN)?C_MAIN_SELF_HSTART: C_MAIN_SELF_HEND);
        //Width
        PROC_WRITE_FLD(bPath, PAT_MAIN_01, _arClipPrm[bPath].wHEnd, (bPath == SV_VP_MAIN)?C_MAIN_SELF_HEND: C_MAIN_SELF_HSTART);
    		
		//V
		PROC_WRITE_FLD(bPath, PAT_MAIN_02, _arClipPrm[bPath].wVStart, C_MAIN_SELF_VSTART);
		PROC_WRITE_FLD(bPath, PAT_MAIN_02, _arClipPrm[bPath].wVEnd, C_MAIN_SELF_VEND);
		//enable regen
		PROC_WRITE_FLD(bPath, PAT_MAIN_00, 0x1, C_MAIN_SYNC_MODIFY);
		if(_arClipPrm[bPath].wVEnd > 2047)
		{		
			PROC_WRITE_FLD(bPath, PAT_MAIN_00, 0x0, C_MAIN_SYNC_MODIFY);
		}
		_arClipPrm[bPath].u1OnOff = SV_OFF;
	}
}

void vDrvPreprocSetSyncRegen(UINT8 bPath,UINT16 wXOffset,UINT16 wYOffset,UINT16 wWidth,UINT16 wHeight)
{        
	UINT16 wHStart,wVStart,wHEnd,wVEnd;

    if (IS_MPEG_420_INTERLACE(bPath))
    {
        wYOffset = ((wYOffset/2) % 2) ? (wYOffset + 2) : wYOffset;
    }
        
	//1. 0 means 1. we need to set wWidth to register.
	//2. if width is odd number, make it even. (wWidth%2)==1 => (wWidth -1) +1 = wWidth
	wHStart = wXOffset;
	wHEnd = wXOffset + wWidth - 1;
	wVStart = ((bDrvVideoIsSrcInterlace(bPath))?(wYOffset/2):wYOffset) + ((bPath==SV_VP_MAIN)?1:0);
	wVEnd = wVStart + ((bDrvVideoIsSrcInterlace(bPath))?(wHeight/2):wHeight);

	_arClipPrm[bPath].wHStart = wHStart;
	_arClipPrm[bPath].wHEnd = wHEnd;
	_arClipPrm[bPath].wVStart = wVStart;
	_arClipPrm[bPath].wVEnd = wVEnd;
	_arClipPrm[bPath].u1OnOff = SV_ON;

	LOG(3,"Set SYNC REGEN (HStart,HEnd,VStart,VEnd)=(%d,%d,%d,%d)\n",wHStart,wHEnd,wVStart,wVEnd);
}
#endif

/*****************************************************************************************/
/**********************************   DEMO MODE   ****************************************/
/*****************************************************************************************/
static Region DemoRec,b4ScalerDemoRec;

void vDemoSetRegion(Region rDemoRegion)
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

void vGetb4ScalerRec(void)
{
    RDispPrm* dispPrm;    
    UINT16 wHPorch, wVPorch;
    dispPrm = getDispPrm(SV_VP_MAIN); 
    UINT32 u4PreDownScale = RegReadFldAlign(SCPIP_HDS_00, SCPIP_HDS_MAIN_SCALER);
    
    if((dispPrm==NULL) || 
       (DemoRec.wHEnd<DemoRec.wHStart) || 
       (DemoRec.wVEnd<DemoRec.wVStart))
    {
        printf("Error parameters of demo region ! \n");
		return;
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
void vDemoRecValidation(void)
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

static INT8 bHMove=1, bVMove=1;
void vDemoRecMoving(void)
{
    UINT16 wWidth = u2ScpipGetDispWidth(SV_VP_MAIN);
    UINT16 wHeight = u2ScpipGetDispHeight(SV_VP_MAIN);
    
    bHMove = (DemoRec.wHStart==0)?1:
               (DemoRec.wHEnd==wWidth)?-1:bHMove;
    bVMove  = (DemoRec.wVStart==0)?1:
               (DemoRec.wVEnd==wHeight)?-1:bVMove;
    vIO32WriteFldAlign(DEMO_00, DemoRec.wHStart+bHMove, DEMOMODE_HSTART);
    vIO32WriteFldAlign(DEMO_00, DemoRec.wVStart+bVMove, DEMOMODE_VSTART);
}

void vDemoProc(void)
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
        vDemoRecValidation();
        vGetb4ScalerRec();
        
        vDrvColorDemo(bOnOff && IO32ReadFldAlign(DEMO_02, DEMO_COLOR_EN), DemoRec);
        vDrv3DNRDemo(bOnOff && IO32ReadFldAlign(DEMO_02, DEMO_3DNR_EN), b4ScalerDemoRec);
        vDrv2DSharpDemo(bOnOff && IO32ReadFldAlign(DEMO_02, DEMO_TDSHARP_EN), DemoRec);
        vDrvECTIDemo(bOnOff && IO32ReadFldAlign(DEMO_02, DEMO_ECTI_EN), DemoRec);          
    }
    if(bMovingOnOff==SV_ON)
    {    
        vDemoRecMoving();
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
        vRegWriteFldAlign(PAT_MAIN_03, bPattern, C_MAIN_PAT_SEL);
    }
    else
    {
        vRegWriteFldAlign(PAT_PIP_03, bPattern, C_PIP_PAT_SEL);
    }
}

/*****************************************************************************************/
/******************************   YCPROC MAIN LOOP   ***************************************/
/*****************************************************************************************/
/**
 *  @brief Set Global PQ setting from SW register to SCE HW register or OS 3x3 matrix
 */
void vDrvDummyToRegister(void)
{
    UINT8 bPath, bOffset;
    static UINT8 bDemoModeOnOff=0;
    UINT8 bDemoToggle;
    static UINT8 bFgUseMatrixPQAdj = 0xFF;

    if(bFgUseMatrixPQAdj == 0xFF)
    {
        bFgUseMatrixPQAdj = DRVCUST_OptGet(eFlagUseMatrixPQAdj);
    }

    for (bPath = SV_VP_MAIN; bPath < SV_VP_NA; bPath ++)
    {
        bDemoToggle = (IO32ReadFldAlign(DEMO_00, DEMOMODE_ENABLE) != bDemoModeOnOff) ? 1 : 0; 
        bDemoModeOnOff = IO32ReadFldAlign(DEMO_00, DEMOMODE_ENABLE);
        
        if ((bFgUseMatrixPQAdj & !bDemoModeOnOff) ||
            (bPath == SV_VP_PIP) ||
            (bIsScalerInput444(bPath)))
        {
           
            bOffset = (bPath ==SV_VP_MAIN) ? 0 : 0x4;
            if ((IO32ReadFldAlign(GLOBAL_ADJ_00 + bOffset, BRIGHT_DUMMY) != u2PQItemMatrix[bPath][PQ_BRIGHTNESS]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_02 + bOffset, CONT_DUMMY) != u2PQItemMatrix[bPath][PQ_CONTRAST]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_02 + bOffset, SAT_DUMMY) != u2PQItemMatrix[bPath][PQ_SATURATION]) ||
                (IO32ReadFldAlign(GLOBAL_ADJ_00 + bOffset, HUE_DUMMY) != u2PQItemMatrix[bPath][PQ_HUE]) ||
                bDemoToggle)
            {
                vDrvVideoColorMatrix(bPath);

                if(bDemoToggle)
                {
                    vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, 0x400, BRIGHT);
                    vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, 0x200, HUE);
                    vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, 0x80, SAT);
                    vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, 0x80, CONT);
                }
            }
        }
        else
        {
            vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, 
                0x400 + ((IO32ReadFldAlign(GLOBAL_ADJ_00, BRIGHT_DUMMY) - 0x80) << DRVCUST_OptGet(eBrightInterval)), 
                BRIGHT);
            vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, 
                0x200 + ((IO32ReadFldAlign(GLOBAL_ADJ_00, HUE_DUMMY) - 0x80) << DRVCUST_OptGet(eHueInterval)), 
                HUE);
            vRegWriteFldAlign(G_PIC_ADJ_MAIN_2, IO32ReadFldAlign(GLOBAL_ADJ_02, SAT_DUMMY), SAT);
            vRegWriteFldAlign(G_PIC_ADJ_MAIN_1, IO32ReadFldAlign(GLOBAL_ADJ_02, CONT_DUMMY), CONT);

            if(bDemoToggle)
            {
                vDrvCalVideoColorMatrix(bPath, 0x80, 0x80, 0x80, 0x80);
            }
        }
    }
}

/**
* @brief Gamma Main Loop
* @param void
*/
void vDrvGammaProc(void)
{
	static UINT8 bPreGammaSel=0x0;

	if (DRVCUST_OptGet(eNVMGamma))
	{
		if ((SV_ON==IO32ReadFldAlign(ADAPTIVE_RGB_GAMMA_REG, ADAPTIVE_RGB_GAMMA_FORCE_LOAD)) ||
		        (bPreGammaSel!= IO32ReadFldAlign(ADAPTIVE_RGB_GAMMA_REG, ADAPTIVE_RGB_GAMMA_SEL)))
		{
			bPreGammaSel= IO32ReadFldAlign(ADAPTIVE_RGB_GAMMA_REG, ADAPTIVE_RGB_GAMMA_SEL);
			vIO32WriteFldAlign(ADAPTIVE_RGB_GAMMA_REG,SV_OFF, ADAPTIVE_RGB_GAMMA_FORCE_LOAD);
			DRVCUST_PANEL_GAMMA_REMAP(bPreGammaSel);
			// vApiRegisterVideoEvent(PE_EVENT_GAMMA, SV_V_GAMMA_MIDDLE, SV_ON);
            vHalVideoGammaLoadTable(SV_ON);
		}
	}
}

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

/**
* @brief SCE Main Loop
* @param void
*/
void vDrvSCEProc(void)
{
    static UINT8 bPreSceGain[3] = {0x80, 0x80, 0x80};
    static UINT8 bPreSkinTable[3] = {0x80, 0x80, 0x80};
    
    //SCE cannot be enabled under VGA signal
    if (bIsScalerInput444(SV_VP_MAIN))
    {
    	return;
    }

    bSceGain[0] = IO32ReadFldAlign(ADAPTIVE_SCE_REG, ADAPTIVE_SCE_L);
    bSceGain[1] = IO32ReadFldAlign(ADAPTIVE_SCE_REG, ADAPTIVE_SCE_S);
    bSceGain[2] = IO32ReadFldAlign(ADAPTIVE_SCE_REG, ADAPTIVE_SCE_H);
    bSkinTable[0] = IO32ReadFldAlign(ADAPTIVE_SKIN_REG, ADAPTIVE_SKIN_L);
    bSkinTable[1] = IO32ReadFldAlign(ADAPTIVE_SKIN_REG, ADAPTIVE_SKIN_S);
    bSkinTable[2] = IO32ReadFldAlign(ADAPTIVE_SKIN_REG, ADAPTIVE_SKIN_H);

    if ((IO32ReadFldAlign(ADAPTIVE_SCE_REG, ADAPTIVE_SCE_FORCE_LOAD) == SV_ON) 
        || (bPreSceGain[0] != bSceGain[0]) || (bPreSceGain[1] != bSceGain[1]) || (bPreSceGain[2] != bSceGain[2])
        || (bPreSkinTable[0] != bSkinTable[0]) || (bPreSkinTable[1] != bSkinTable[1]) || (bPreSkinTable[2] != bSkinTable[2]))
    {
        bPreSceGain[0] = bSceGain[0]; 
        bPreSceGain[1] = bSceGain[1]; 
        bPreSceGain[2] = bSceGain[2]; 
        bPreSkinTable[0] = bSkinTable[0]; 
        bPreSkinTable[1] = bSkinTable[1]; 
        bPreSkinTable[2] = bSkinTable[2]; 

        vDrvSCECalculate();
        vIO32WriteFldAlign(ADAPTIVE_SCE_REG,SV_OFF, ADAPTIVE_SCE_FORCE_LOAD);
        vApiRegisterVideoEvent(PE_EVENT_SCE, SV_VP_MAIN, SV_ON);		
    }

    _vDrvSCEParialLoadProc();
}

/**
* @brief MM Application Main Loop
* @param void
*/
void vMMAPPorc(void)
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

/**
* @brief Yproc Main Loop
* @param void
*/
void vDrvYcprocProc(void)
{
    //Meter Main Loop
    vDrvMeterProc();
    
    //Gamma Main Loop
    vDrvGammaProc();

    //SCE Main Loop
    vDrvSCEProc();

    //SHARPNESS adaptive control : Auto SHARPNESS
    vDrvSharpProc();

    // OPC Main Loop
    //vDrvOPCProc();	

    //Demo mode proc.
    vDemoProc();

    //MM Application proc
    vMMAPPorc();

    // ScalerPQ proc
    vDrvScalerPQProc();
}

// ============================================================================
