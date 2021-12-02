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
 * $Date: 2012/06/18 $
 * $RCSfile: api_notify.c,v $
 * $Revision: #4 $
 *
 *---------------------------------------------------------------------------*/
#include "typedef.h"

#include "sv_const.h"
#include "api_notify.h"
#include "source_table.h"
#include "vendor.h"
#include "nptv_if.h"
#include "nptv_debug.h"




/*lint -save -e506 -e950 */
#ifndef  DISABLE_NPTV_NOTIFY 

static NPTV_SIG_NFY_INFO_T _arSigNfyInfo[2];
static NPTV_PFN_SCART_NFY _pfnScartNfy;
static NPTV_PFN_VGA_AUTO_NFY _pfnVgaAutocolorNfy;
static NPTV_PFN_TVD_MVCHG_NFY _pfnTvdMVChgNfy;
static NPTV_PFN_TTX_NFY _pfnTtxNfy = NULL;
static NPTV_PFN_CC_NFY _pfnCcNfy = NULL;
static NPTV_PFN_CC2X_NFY _pfnCc2xNfy = NULL;
static NPTV_PFN_VPS_NFY _pfnVpsNfy = NULL;
static NPTV_PFN_WSS_NFY _pfnWssNfy = NULL;
static NPTV_PFN_WSS_NFY _pfnWss525Nfy = NULL;
static NPTV_PFN_VBI_INFO_NFY _pfnVbiInfoNfy = NULL;
//static NPTV_PFN_OUT_SYNC  _pfnOutVSync = NULL;
static NPTV_PFN_HDMI_NFY _pfnHdmiNfy = NULL;

INT32 _VBI_SetVbiInfoNfy(const NPTV_PFN_VBI_INFO_NFY pfFct)
{
    _pfnVbiInfoNfy = pfFct;
    return 1;
}

INT32 _VBI_SetTtxNfy(const NPTV_PFN_TTX_NFY pfFct)
{
    _pfnTtxNfy = pfFct;
    return 1;
}
INT32 _VBI_SetCcNfy(const NPTV_PFN_CC_NFY pfFct)
{
    _pfnCcNfy = pfFct;
    return 1;
}
INT32 _VBI_SetCc2xNfy(const NPTV_PFN_CC2X_NFY pfFct)
{
    _pfnCc2xNfy = pfFct;
    return 1;
}
INT32 _VBI_SetVpsNfy(const NPTV_PFN_VPS_NFY pfFct)
{
    _pfnVpsNfy = pfFct;
    return 1;    
}
INT32 _VBI_SetWssNfy(const NPTV_PFN_WSS_NFY pfFct)
{
    _pfnWssNfy = pfFct;
    return 1;    
}

INT32 _VBI_SetWss525Nfy(const NPTV_PFN_WSS_NFY pfFct)
{
    _pfnWss525Nfy = pfFct;
    return 1;    
}

INT32 _NPTV_SetVgaAutocolorNfy(const NPTV_PFN_VGA_AUTOCOLOR_NFY pfnFct)
{
    _pfnVgaAutocolorNfy = pfnFct;
    return SV_SUCCESS;
}

INT32 _NPTV_SetTvdMVChgNfy(const NPTV_PFN_TVD_MVCHG_NFY pfnFct)
{
    _pfnTvdMVChgNfy = pfnFct;
    return SV_SUCCESS;
}

INT32 _NPTV_SetSigChgNfyFct(const NPTV_PFN_SIG_NFY_FCT pfnFct)
{
    if (pfnFct == NULL)
    {
        return (-1);
    }

    _arSigNfyInfo[0].pfSigNfy = pfnFct;
    _arSigNfyInfo[0].pvTag = 0;
    _arSigNfyInfo[1].pfSigNfy = pfnFct;
    _arSigNfyInfo[1].pvTag = (void*) 1;

    return (0);
}
INT32 _NPTV_SetScartNfy(const NPTV_PFN_SCART_NFY pfnFct)
{
    _pfnScartNfy = pfnFct;
    return SV_SUCCESS;
}


NPTV_PFN_VGA_AUTO_NFY _pfnVgaAutoNfy;
INT32 _NPTV_SetVgaAutoNfy(const NPTV_PFN_VGA_AUTO_NFY pfnFct)
{
    _pfnVgaAutoNfy = pfnFct;
    return SV_SUCCESS;
}
#if 0
INT32 NPTV_SetOutVSyncNfyFct(const NPTV_PFN_OUT_SYNC pfnFct)
{
	_pfnOutVSync = pfnFct;
	return (INT32)SV_SUCCESS;
}
#endif
INT32 NPTV_SetSigChgNfyFct(const NPTV_PFN_SIG_NFY_FCT pfnFct)
{
    return _NPTV_SetSigChgNfyFct(pfnFct);
}

INT32 NPTV_SetScartNfy(const NPTV_PFN_SCART_NFY pfnFct)
{
    return _NPTV_SetScartNfy(pfnFct);
}
	
INT32 NPTV_SetVgaAutoNfy(const NPTV_PFN_VGA_AUTO_NFY pfnFct)
{
    return _NPTV_SetVgaAutoNfy(pfnFct);
}

INT32 NPTV_SetHdmiNfy(const NPTV_PFN_HDMI_NFY pfnFct)
{
   _pfnHdmiNfy = pfnFct;
   return SV_SUCCESS;
}



INT32 NPTV_SetVgaAutocolorNfy(const NPTV_PFN_VGA_AUTOCOLOR_NFY pfnFct)
{
    return _NPTV_SetVgaAutocolorNfy(pfnFct);
}

INT32 NPTV_SetTvdMVChgNfy(const NPTV_PFN_TVD_MVCHG_NFY pfnFct)
{
    return _NPTV_SetTvdMVChgNfy(pfnFct);
}

INT32 NPTV_VBI_SetVbiInfoNfy(const NPTV_PFN_VBI_INFO_NFY pfnFct)
{
    return _VBI_SetVbiInfoNfy(pfnFct);
}

INT32 NPTV_VBI_SetTtxNfy(const NPTV_PFN_TTX_NFY pfnFct)
{
    return _VBI_SetTtxNfy(pfnFct);
}
INT32 NPTV_VBI_SetCcNfy(const NPTV_PFN_CC_NFY pfnFct)
{
    return _VBI_SetCcNfy(pfnFct);
}
INT32 NPTV_VBI_SetVpsNfy(const NPTV_PFN_VPS_NFY pfnFct)
{
    return _VBI_SetVpsNfy(pfnFct);
}
INT32 NPTV_VBI_SetWssNfy(const NPTV_PFN_WSS_NFY pfnFct)
{
    return _VBI_SetWssNfy(pfnFct);
}

//MTAL use vApiNotifyVGAAutoProgress instead of vApiNotifyAutoDone
// 100% means auto done.
extern void vApiNotifyVGAAutoProgress(UINT8 u1Path, UINT8 u1Progress)
{
    if (_pfnVgaAutoNfy != NULL)
    {
        _pfnVgaAutoNfy(u1Path, u1Progress);
    }
}
#if 0
void vApiNotifyOutVSync(void)
{
	if(_pfnOutVSync != NULL)
		_pfnOutVSync();
}
#endif
#ifdef CC_DRIVER_PROGRAM
/* SDAL part */
//extern void SdalNotifyAutoDone(UINT8 u1Path,BOOL fgSuccess) __attribute__((weak));
//extern void SdalNotifySigChg(UINT8 u1Path,UINT8 u1SigState,UINT8 u1ReportState) __attribute__((weak));

BOOL fgApiMonitorSetup(UINT8 u1Path,UINT8 u1Src)
{
	return  FALSE;
}



/* REAL  NOTIFY PART*/
extern void vApiNotifySigChg(UINT8 u1Path,UINT8 u1SigState,UINT8 u1ReportState)
{
    PFN_NOTIFICATION_VIDEO_FUNC pfnNotifyFunc = NULL;
    
    if((_arSigNfyInfo[u1Path].pfSigNfy)&&(VSS_MAJOR(bGetVdoSrcNew(u1Path))!=VSS_DTV))
    {
        _arSigNfyInfo[u1Path].pfSigNfy((void*)(UINT32)u1Path, u1SigState, 0);
    }
#if 0    
    if(SdalNotifySigChg!=0)
    {
        SdalNotifySigChg(u1Path,u1SigState,u1ReportState);
    }
#else
    if (DRVCUST_OptQuery(eNotifyVideoPropertyFunc, (UINT32 *)(void *)&pfnNotifyFunc) == 0)
    {        
       if (pfnNotifyFunc)
        {
            NOTIFY_VIDEO_T tNotifyInfo;

            tNotifyInfo.u4NotifyId = NOTIFICATION_VIDEO_SIGNALCHANGED;
            tNotifyInfo.u4VdpId = u1Path;
            tNotifyInfo.u4Data1 = u1SigState;
            tNotifyInfo.u4Data2 = u1ReportState;
            
            pfnNotifyFunc(tNotifyInfo);
        }
        else
        {
            LOG(5, "undefine NOTIFY_VIDEO_FUNC\n");
        }
    }
#endif
    
}
extern void vApiNotifyResolutionChg(UINT8 u1Path,UINT8 u1SigState)
{
	/* empty */
}
/* ATV related*/
extern void vApiNotifyColorSystem(UINT8 u1Path,UINT8 u1SigState)
{
	/* empty */
}
extern void vApiNotifySFirst(UINT8 u1Path,UINT8 u1SigState)
{
	/* empty */
}

/* VGA/Ypbpr related*/

extern void vApiNotifyAutoDone(UINT8 u1Path,BOOL fgSuccess)
{
    PFN_NOTIFICATION_VIDEO_AUTO_DONE_FUNC pfnNotifyFunc = NULL;
#if 0 ////MTAL use vApiNotifyVGAAutoProgress instead of vApiNotifyAutoDone
    if (_pfnVgaAutoNfy != NULL)
    {
        _pfnVgaAutoNfy(u1Path, fgSuccess);
    }
#endif
#if 0
    if(SdalNotifyAutoDone != NULL)
    {
        SdalNotifyAutoDone(u1Path, fgSuccess);
    }
#else
    if (DRVCUST_OptQuery(eNotifyVideoAutoDoneFunc, (UINT32 *)(void *)&pfnNotifyFunc) == 0)
    {        
       if (pfnNotifyFunc)
        {
            NOTIFY_VIDEO_AUTO_DONE_T tNotifyInfo;

            tNotifyInfo.u4NotifyId = NOTIFICATION_VIDEO_AUTODONE;
            tNotifyInfo.u4VdpId = u1Path;
            tNotifyInfo.u4Data1 = fgSuccess;
            
            pfnNotifyFunc(tNotifyInfo);
        }
        else
        {
            LOG(5, "undefine NOTIFY_VIDEO_AUTO_DONE_FUNC\n");
        }
    }
#endif
}

extern void vApiNotifyAutoColorDone(UINT8 u1Path,BOOL fgSuccess)
{
	if(_pfnVgaAutocolorNfy != NULL)
	{
	    _pfnVgaAutocolorNfy(u1Path, fgSuccess);
	}
}

extern void vApiNotifyMVChg(UINT8 u1Path,BOOL fgStatus)
{
	if(_pfnTvdMVChgNfy != NULL)
	{
	    _pfnTvdMVChgNfy(u1Path, fgStatus);
	}
}

/* non-path related*/
extern void vApiNotifyScartFastSwith(UINT8 u1Scartid, UINT8 u1FSState)
{
	if(_pfnScartNfy != NULL)
	{
	    _pfnScartNfy(u1Scartid, u1FSState);
	}

}
/* EU related*/
extern void vAPiNotifyCCData(UINT8 u1Path, UINT8 d1, UINT8 d2, UINT8 field)
{
	if(_pfnCcNfy != NULL)
	{
	    _pfnCcNfy(u1Path, d1, d2, field);
	}
}

extern void vAPiNotifyCC2XData(UINT8 u1Path, UINT8 u2PktsAvail, UINT32 readptr)
{
	if(_pfnCc2xNfy != NULL)
	{
	    _pfnCc2xNfy(u1Path, u2PktsAvail, readptr);
	}
}

extern void vAPiNotifyTTXData(UINT8 u1Path, UINT8 u2PktsAvail, UINT32 readptr)
{
	if(_pfnTtxNfy != NULL)
	{
	    _pfnTtxNfy(u1Path, u2PktsAvail, readptr);
	}

}

extern void vAPiNotifyVPSData(UINT8 au1Data[])
{
	if(_pfnVpsNfy != NULL)
	{
	    _pfnVpsNfy(au1Data);
	}
}

extern void vAPiNotifyEUWSSData(UINT8 u1Path,UINT32 u4data)
{
	if(_pfnWssNfy != NULL)
	{
	    _pfnWssNfy(u1Path, u4data);
	}

}

extern void vAPiNotifyUSWSSData(UINT8 u1Path,UINT32 u4data)
{
	if(_pfnWss525Nfy != NULL)
	{
	    _pfnWss525Nfy(u1Path, u4data);
	}

}

extern void vAPiNotifyVbiInfoData(BOOL fgAVFieldIVS)
{
    if (_pfnVbiInfoNfy != NULL)
    {
	    _pfnVbiInfoNfy(fgAVFieldIVS);
    }

}

#else /*MW if*/
#include "video_def.h"


extern INT32 avc_combi_get_default_comp(
    UINT16    ui2_combi_id,
    DRV_ID_T* pt_drv_comp ) __attribute__((weak));
extern BOOL avc_combi_is_a_combi_comp(
    DRV_TYPE_T  e_avc_type,
    UINT16      ui2_avc_id,
    UINT16*     pui2_combi_id ) __attribute__((weak));
extern INT32 avc_combi_get_monitor_comp(
    UINT16    ui2_combi_id,
    DRV_ID_T* pt_drv_comp ) __attribute__((weak));
extern UINT32 rGetVSrcMap( 
    DRV_TYPE_T e_type, 
    UINT16     ui2_comp_id ) __attribute__((weak));


extern INT32 rGetVSrcCompFromIdx(
    UINT32      e_vid_src_idx,
    UINT16      ui2_flag,
    DRV_TYPE_T* pe_type, 
    UINT16*     pui2_comp_id ) __attribute__((weak));




BOOL fgApiMonitorSetup(UINT8 u1Path,UINT8 u1Src)
{
	BOOL ret;
/*lint -save -e644 */
	UINT8  u1MonSrc=SV_VS_MAX;
    	DRV_TYPE_T t_e_type;
    	UINT16  ui2_comp_id;
	UINT16 u2MainCombiId;
	DRV_ID_T t_main_mon_comp;
/*lint restore */
	/* Combi related behavior here */
	ret = FALSE;
	if(u1Src != SV_VS_MAX)
	{		
		if(rGetVSrcCompFromIdx&&(rGetVSrcCompFromIdx((UINT32)u1Src, 0, &t_e_type, &ui2_comp_id) == 0))
		{
			if((avc_combi_is_a_combi_comp!=0)&&(u1Src != SV_VS_MAX))
			{
				ret = avc_combi_is_a_combi_comp(t_e_type, ui2_comp_id, &u2MainCombiId);
		}
		}
	}
	
	if(ret)
	{
		RVChannel  *chnl=u1Path?(&_rPChannel):(&_rMChannel);
		UINT8 *bMonOldSrc=u1Path?&_bMonSubOld:&_bMonMainOld;
		UINT8 *bMonSrc=u1Path?&_bMonSubNew:&_bMonMainNew;
		UINT8 *bMonNewICIn=u1Path?&_bSubMonICIn:&_bMainMonICIn;
		

		/* Get Monitored Component */
		if(avc_combi_get_monitor_comp!=0)
		{
			UNUSED(avc_combi_get_monitor_comp(u2MainCombiId,
			&t_main_mon_comp));
		}

		/* translate monitored comp to nptv source */
		if(rGetVSrcMap!=0)
		{
			u1MonSrc = rGetVSrcMap(t_main_mon_comp.e_type, t_main_mon_comp.ui2_id);
		}

		//ret = true
		if(u1MonSrc!= (*bMonOldSrc))
		{			
			// record information about monitored component
			chnl->t_mon_id = t_main_mon_comp;			
			*bMonNewICIn = u1GetMonICIn(u1MonSrc);			
			*bMonOldSrc = *bMonSrc;
			*bMonSrc=u1MonSrc;
		}
	}
	return ret;
}

extern void vNotify_Sig_Chg(UINT8 u1Path) __attribute__((weak));
extern void _Tvd_NotifyTVSig(UINT8 u1Path, UINT8 u1SigState) __attribute__((weak));
extern void vNotifyScart_Sig_Chg(UINT8 u1Source, UINT8 uStatus) __attribute__((weak));
extern void _TvdNotifyResolutionChg(UINT8 u1Path, UINT8 u1SigState) __attribute__((weak));
extern void _TvdNotifyColorSystem(UINT8 u1Path, UINT8 u1SigState) __attribute__((weak));

extern void _TvdNotifyAutoDone(UINT16 ui2_id, BOOL fgSuccess) __attribute__((weak));  
extern void _TvdNotifyAutoColorDone(UINT16 ui2_id, BOOL fgSuccess) __attribute__((weak));
extern void _TvdNotifyMVChg(UINT8 u1Path, BOOL fgStatus) __attribute__((weak));
extern void  vNotifyComp_Sig_Chg( DRV_TYPE_T  e_avc_type,UINT16 ui2_avc_id,UINT32 ui4_drv_status ) __attribute__((weak));
extern void TvdNotifyCCData(UINT8 u1Path, UINT8 d1, UINT8 d2, UINT8 field) __attribute__((weak));
extern void TvdNotifyTtxData(UINT8 u1Path, UINT16 u2NumPktAvail, UINT32 _dVbiReadPtr) __attribute__((weak));
extern void TvdNotifyVPSData(UINT8  au1Data[]) __attribute__((weak));
extern void TvdNotifyEUWSSData(UINT8 u1Path, UINT16 u2Data) __attribute__((weak));
extern void TvdNotifyUSWSSData(UINT8 u1Path, UINT16 u2Data) __attribute__((weak)); /* US WSS525 */
#ifdef CC_SOURCE_AUTO_DETECT
extern VOID _TvdNOtifyAutoDetectSource(UINT8 u1SrcIndex, UINT8 u1SrcStatus) __attribute__((weak));
#endif


/* general */
void vApiNotifySigChg(UINT8 u1Path,UINT8 u1SigState,UINT8 u1ReportState)
{

    if (VSS_MAJOR(bGetVdoSrcNew(u1Path))!=VSS_DTV)
	{

		if(vNotify_Sig_Chg!=0 && u1SigState != SV_VDO_UNKNOWN)
		{
			vNotify_Sig_Chg(u1Path);
		}
		if(u1ReportState)
		{
			if(_Tvd_NotifyTVSig!=0)
			{
				_Tvd_NotifyTVSig(u1Path, u1SigState);
			}
		}
    }
//	printf("before mtal notify ------------\n\n\n");
    // callback for MTAL
    if ((_arSigNfyInfo[u1Path].pfSigNfy))// && (u1SigState != SV_VDO_UNKNOWN))
    {
        _arSigNfyInfo[u1Path].pfSigNfy((void*)(UINT32)u1Path, u1SigState, 0);
    }

}
void vApiNotifyResolutionChg(UINT8 u1Path,UINT8 u1SigState)
{
	if(VSS_MAJOR(bGetVdoSrcNew(u1Path))==VSS_DTV)
		return;

	if(_TvdNotifyResolutionChg!=0)
	{
		_TvdNotifyResolutionChg(u1Path, u1SigState);
	}
}
/* ATV related*/
void vApiNotifyColorSystem(UINT8 u1Path,UINT8 u1SigState)
{
	if(VSS_MAJOR(bGetVdoSrcNew(u1Path))==VSS_DTV)
		return;

	if(_TvdNotifyColorSystem!=0)
	{
		_TvdNotifyColorSystem(u1Path, u1SigState);
	}
}
/* input monitor notify*/
void vApiNotifySFirst(UINT8 u1Path,UINT8 u1SigState)
{
	RVChannel * chnl= (u1Path!=1)?(&_rMChannel):(&_rPChannel);	
	if((vNotifyComp_Sig_Chg)&&(chnl->t_mon_id.e_type== DRVT_AVC_S_VIDEO))
	{
		vNotifyComp_Sig_Chg(chnl->t_mon_id.e_type, chnl->t_mon_id.ui2_id, (UINT32)u1SigState);
	}

}

#ifdef  SUPPORT_AV_COMP_AUTO
/* input monitor notify*/
void vApiNotifyAVCOMPAuto(UINT8 u1Path,UINT8 u1SigState)
{
	RVChannel * chnl= (u1Path!=1)?(&_rMChannel):(&_rPChannel);	
	if((vNotifyComp_Sig_Chg))
	{
		vNotifyComp_Sig_Chg(chnl->t_mon_id.e_type, chnl->t_mon_id.ui2_id, (UINT32)u1SigState);
	}

}
#endif
/* VGA/Ypbpr related*/
void vApiNotifyAutoDone(UINT8 u1Path,BOOL fgSuccess)
{
#if 1
    if (_pfnVgaAutoNfy != NULL)
    {
    
        LOG(1,"vApiNotifyAutoDone  \n");
        _pfnVgaAutoNfy(u1Path, fgSuccess);
    }
#else

	if (_TvdNotifyAutoDone!=0)
	{
		_TvdNotifyAutoDone(u1Path, fgSuccess);
	}	
#endif
}
void vApiNotifyAutoColorDone(UINT8 u1Path,BOOL fgSuccess)
{
	// Middleware notify
	if(_TvdNotifyAutoColorDone!=0)
	{
		_TvdNotifyAutoColorDone(u1Path, fgSuccess);
	}

	if(_pfnVgaAutocolorNfy != NULL)
	{
	    _pfnVgaAutocolorNfy(u1Path, fgSuccess);
	}

}

void vApiNotifyMVChg(UINT8 u1Path,BOOL fgStatus)
{
	// Middleware notify
	if(_TvdNotifyMVChg!=0)
	{
		_TvdNotifyMVChg(u1Path, fgStatus);
	}
	if(_pfnTvdMVChgNfy != NULL)
	{
	    _pfnTvdMVChgNfy(u1Path, fgStatus);
	}

}

#ifdef CC_SOURCE_AUTO_DETECT
void vApiNotifyAutoDetectSource(UINT8 u1SrcIndex, UINT8 u1SrcStatus)
{
	// Middleware notify
	if(_TvdNOtifyAutoDetectSource != 0)
	{
		_TvdNOtifyAutoDetectSource(u1SrcIndex, u1SrcStatus);
	}
/*	if(_pfnTvdAutoDetectSourceNfy != NULL)
	{
		_pfnTvdAutoDetectSourceNfy(u1SrcIndex, u1SrcStatus);
	}
*/	
}

#endif


/* non-path related*/
void vApiNotifyScartFastSwith(UINT8 u1Scartid, UINT8 u1FSState)
{
#if SUPPORT_MULTI_SYSTEM
	if(vNotifyScart_Sig_Chg!=0)
	{
		vNotifyScart_Sig_Chg(u1Scartid, u1FSState);
	}
#endif
    if(_pfnScartNfy != NULL)
    {
        _pfnScartNfy(u1Scartid, u1FSState);
    }
}
void vApiNotifyHdmiStatus(UINT32 bHdmiPort, UINT32 bStatus)
{
    if(_pfnHdmiNfy != NULL)
    {
        _pfnHdmiNfy(bHdmiPort, bStatus);
    }
}
/* EU related*/
void vAPiNotifyCCData(UINT8 u1Path, UINT8 d1, UINT8 d2, UINT8 field)
{
	if(TvdNotifyCCData!=0)
	{
		TvdNotifyCCData(u1Path, d1, d2, field);
	}
    if(_pfnCcNfy != NULL)
    {
        _pfnCcNfy(u1Path, d1, d2, field);
    }
}
void vAPiNotifyCC2XData(UINT8 u1Path, UINT8 u2PktsAvail, UINT32 readptr)
{
    if(_pfnCc2xNfy != NULL)
    {
        _pfnCc2xNfy(u1Path, u2PktsAvail, readptr);
    }
	return;
}
void vAPiNotifyTTXData(UINT8 u1Path, UINT8 u2PktsAvail, UINT32 readptr)
{
    if(TvdNotifyTtxData!=0)
    {
	    TvdNotifyTtxData(u1Path, u2PktsAvail, readptr);
    }
    if(_pfnTtxNfy != NULL)
    {
        _pfnTtxNfy(u1Path, u2PktsAvail, readptr);
    }
}
void vAPiNotifyVPSData(UINT8 au1Data[])
{
	if(TvdNotifyVPSData!=0)
	{
		TvdNotifyVPSData(au1Data);
	}
    if(_pfnVpsNfy != NULL)
    {
        _pfnVpsNfy(au1Data);
    }
}
void vAPiNotifyEUWSSData(UINT8 u1Path,UINT32 u4data)
{
	if(TvdNotifyEUWSSData!=0)
	{
		TvdNotifyEUWSSData(u1Path,(UINT16) u4data);
	}
    if(_pfnWssNfy != NULL)
    {
        _pfnWssNfy(u1Path, u4data);
    }
}

/* US WSS525 */
void vAPiNotifyUSWSSData(UINT8 u1Path,UINT32 u4data)
{
	if(TvdNotifyUSWSSData!=0)
	{
		TvdNotifyUSWSSData(u1Path,(UINT16) u4data);
	}
    if(_pfnWss525Nfy != NULL)
    {
        _pfnWss525Nfy(u1Path, u4data);
    }
}

/* Dumy VBI Info */
void vAPiNotifyVbiInfoData(BOOL fgAVFieldIVS)
{
    if (_pfnVbiInfoNfy != NULL)
    {
	    _pfnVbiInfoNfy(fgAVFieldIVS);
    }
    return;
}

#endif
#endif

/*lint -restore */
