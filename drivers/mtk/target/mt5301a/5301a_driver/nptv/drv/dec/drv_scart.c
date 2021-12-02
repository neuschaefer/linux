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
 * $RCSfile: drv_scart.c,v $
 * $Revision: 
 *
 *---------------------------------------------------------------------------*/

#include "general.h"
#include "drv_pwm.h"
#include "hw_vdoin.h"
#include "sv_const.h"
#include "drv_adcset.h"
#include "drv_scart.h"
#include "hw_vdoin.h"
#include "hw_scart.h"
#include "hw_pdwnc.h"
#include "x_tv_dec.h"
#include "drvcust_if.h"
#include "drv_tvd.h"
#include "drv_video.h"

#include "source_select.h"
#include "nptv_drvif.h"
#include "mute_if.h"
#include "drv_vdoclk.h"
#include "drv_async.h"

//537x's SRVAD has been move to pdwnc
//#include "x_pdwnc.h"
#define vDrvSRVADEnable()
#define bDrvReadSRVAD(x)    (PDWNC_ReadServoADCChannelValue(x)&0xff)

#define RGB_IMUX_PERM  5        /* Permutatios of RGB data */
#define FB_GLITCH_TAP  3        /* 0: no glitch, 1: 3-tap, 2: 5-tap, 3: 7-tap */
#define FB_GLITCH_HIGH_THRE  5
#define FB_GLITCH_LOW_THRE  2

#define EVENT_CHECK_COUNT 10

#define SCART_PIN8_STS_WITH_VSYNC (4)
#define SCART_STATUS_COUNT_THRES (12) /* # of vsync, 240ms ~= 12, reduce the timing to detect scart unplug event when in decode channel*/

#define SCART_DBG_MSG 0

/*The define here is mapping to the SERVO_ADC input number*/
/* make every variable global to be easy to debug */
UINT8 SERVOADC_SCART1_FS = 0;
UINT8 SERVOADC_SCART2_FS = 1;
UINT8 _bSCART1Status;
UINT8 _bSCART2Status;
UINT8 _bScartPin8ASR_save[2];

UINT8 _bUIScartMode;
UINT8 _bTrigScartAuto;
UINT8 _bSFisSV;                 //Gene Chang For the SCART Auto to remove the S first function 2006.05.12
UINT8 _bOldSFisSV;     //avoid unnecessary AV/SV transit
UINT8 _bScartAutoMode;

/* gellmann */
extern void vDrvScartRGBHdtvInit(void);

extern UINT32 PDWNC_ReadServoADCChannelValue(UINT32);

#if SUPPORT_SCART

void vSCARTInit(UINT8 vsrc)     //Gene Chang For the TV/SCART switching problem 2006.09.29
{
#if SCART_DBG_MSG
    Printf("vSCARTInit\n");
#endif
    vRegWriteFldAlign(SCART_02, RGB_IMUX_PERM, FLD_RGB_IMUX);

    vRegWriteFldAlign(SCART_01, FB_GLITCH_TAP, FLD_FB_DEGLITCH);
    vRegWriteFldAlign(SCART_01, FB_GLITCH_HIGH_THRE, FLD_FB_HIGH_TH);
    vRegWriteFldAlign(SCART_01, FB_GLITCH_LOW_THRE, FLD_FB_LOW_TH);

    /* Monitoring correct FB status */
    vRegWriteFldAlign(SCART_00, 1, FLD_MON_V_POL);


    /* Turn on mix mode */
#if 0//YPBPR_480IP_27MHZ
	vRegWriteFldAlign(SCART_00, 0, FLD_MIX_MODE);
	vRegWriteFldAlign(SCART_00, 1, FLD_MIX_ORDER); 
#else 
    vRegWriteFldAlign(SCART_00, 1, FLD_MIX_MODE);
	vRegWriteFldAlign(SCART_00, 1, FLD_MIX_ORDER); 
#endif
 
	vRegWriteFldAlign(SCART_00, 16, FLD_RGBOFFSET);
    //set for ultra-black

	vRegWriteFldAlign(SCART_00, 40, FLD_YOFFSET);

	    
#if 0
        vDrvSetTVMux(vsrc, SV_VS_MAX);  /* not monitor? P_C0 ~ P_C3 */
        vDrvSetTVADC(vsrc);

    if ((_bMainICIn == P_FB0) || (_bSubICIn == P_FB0))
    {
        vDrvSetHDTVADC(P_FB0);
        vDrvSetHDTVMux(bDrvGetFBRGB(P_FB0));    /* P_VGA, P_YP0, or P_YP1 */

    }
    else if ((_bMainICIn == P_FB1) || (_bSubICIn == P_FB1))
    {

        vDrvSetHDTVMux(bDrvGetFBRGB(P_FB1));    /* P_VGA, P_YP0, or P_YP1 */
        vDrvSetHDTVADC(P_FB1);
    }
#endif    

    vScartVfeConnect();

    /* Set HDTV Clock here ... */
    vDrvScartRGBHdtvInit();

    vRegWriteFldAlign(SCART_00, 0x1, FLD_OUTPUT_EN);        /* Enable mix_en */

    /* Enable fifo mode to don't send blank area data */
    vRegWriteFldAlign(SCART_00, 0x0, FLD_FIFO_MODE);    
	//don't set the FIFO depth to 0x7f
    vRegWriteFldAlign(SCART_01, 0x7E, FLD_FIFO_DEPTH);    
	//Enable downsample 1/2 default which is compatible to before	
    vRegWriteFldAlign(SCART_00, 1, FLD_SCART_DOWNSAMPLE);  
    _bSFisSV = SV_FALSE;        /* Initially set as CVBS mode */
    _bOldSFisSV = SV_FALSE;     /* Initially set as CVBS mode */

}

void vSCARTDisable(void)        //Gene Chang For the TV/SCART switching problem 2006.09.29
{
#if SCART_DBG_MSG
    Printf("vSCARTDisable\n");
#endif

    vRegWriteFldAlign(SCART_00, 0x0, FLD_OUTPUT_EN);        /* Disable mix_en */
    vScartVfeDisconnect();
}

PRIVATE void vScartAVSVTransit(UINT8 bAvSv)
{
    if (bAvSv == SCART_COLOR_CVBS)
    {
        vScartMix();
        vTvd3dSVInit(SV_OFF);

#if SCART_DBG_MSG
        Printf("SCART Type: %d\n", DRVCUST_OptGet(eScartType));
#endif
        if ((DRVCUST_OptGet(eScart1Type) == SCART_HALF_TYPE)
            && ((_bMainICIn == P_FB0) || (_bSubICIn == P_FB0)))
        {
            vRegWriteFldAlign(SCART_01, 1, FLD_FB_FORCE_OFF);   // set the force Off
        }
        else  if ((DRVCUST_OptGet(eScart2Type) == SCART_HALF_TYPE)
            && ((_bMainICIn == P_FB1) || (_bSubICIn == P_FB1)))
        {
            vRegWriteFldAlign(SCART_01, 1, FLD_FB_FORCE_OFF);   // set the force Off
        }        
        else
        {
            vRegWriteFldAlign(SCART_01, 0, FLD_FB_FORCE_OFF);   // clear the force Off
        }
    }
    else if (bAvSv == SCART_COLOR_SV)
    {
        vScartSvConnect();
        vTvd3dSVInit(SV_ON);
        vRegWriteFldAlign(SCART_01, 1, FLD_FB_FORCE_OFF);   //Disable fast blanking under SCART SV.
    }
    #if 0 // 5387
    if ((_bMainICIn == P_FB0) || (_bSubICIn == P_FB0))
    {
        vApiVideoBypass(P_FB0);
    }
    else if ((_bMainICIn == P_FB1) || (_bSubICIn == P_FB1))
    {
        vApiVideoBypass(P_FB1);
    }
    #endif
}

static void vScartUpdateAutoMode()
{
    if (_bSFisSV)
    {
        _bScartAutoMode = SCART_COLOR_SV;
    }
    else
    {
        if (RegReadFldAlign(SCART_STA_00, FLD_FB_OFF_CNT) < 10
            && RegReadFldAlign(SCART_STA_00, FLD_FB_ON_CNT) > 512)
        {
            /* almost all RGB */
            _bScartAutoMode = SCART_COLOR_RGB;
        }
        else if (RegReadFldAlign(SCART_STA_00, FLD_FB_ON_CNT) < 10)
        {
            _bScartAutoMode = SCART_COLOR_CVBS;
        }
        else
        {
            _bScartAutoMode = SCART_COLOR_MIX;
        }
    }
}

PRIVATE UINT8 bScartEvtchk_ticks;

#if CHANGE_SDDS_KPI
extern UINT8 _bClkTrigger  ;
#endif

void vScartEvtchk(void)
{
    BOOL fgScartModeChg = SV_FALSE;
    static UINT8 bScartMixCnt = 0;
    UINT8 bPrevScartAutoMode;

    bScartEvtchk_ticks++;
    /* simulate 8202's  UI  20ms loop, assume display Vsync in 60Hz */
    if (bScartEvtchk_ticks >= EVENT_CHECK_COUNT)
    {
        bScartEvtchk_ticks = 0;
        if (fgApiVideoIsEvtFlgSet(VDO_FLG_SCART_SF_CHG))
        {
            vApiVideoClrEvtFlg(VDO_FLG_SCART_SF_CHG);
            if (_bUIScartMode == SV_SCART_AUTO)
            {
	        vScartUpdateAutoMode();
                if (_bSFisSV)   /* TVD flag check */
                {
#if SCART_DBG_MSG
                    Printf("Auto to S-Video\n");
#endif

	            /* detect by vScartUpdateAutoMode(), comment out the next */
                    //_bScartAutoMode = SCART_COLOR_SV;
                    vScartAVSVTransit(SCART_COLOR_SV);
                }
                else
                {
#if SCART_DBG_MSG
                    Printf("Auto to CVBS\n");
#endif

	            /* detect by vScartUpdateAutoMode(), comment out the next */
                    //_bScartAutoMode = SCART_COLOR_CVBS;
                    vScartAVSVTransit(SCART_COLOR_CVBS);
                }
            }
        }

        /* for checking RGB/CVBS in for main path */
        if ( (_bMainICIn == P_FB0 || _bMainICIn == P_FB1)
	    && (_bUIScartMode == SV_SCART_AUTO
            || _bUIScartMode == SV_SCART_CVBS_RGB))
        {
	    bPrevScartAutoMode = _bScartAutoMode;
	    vScartUpdateAutoMode();
	    if((bPrevScartAutoMode != _bScartAutoMode) &&
	        (bScartMixCnt < 2) &&
		(_bScartAutoMode == SCART_COLOR_CVBS ||
		 _bScartAutoMode == SCART_COLOR_RGB)
	      )
	    {
	        /* in order to reload the correct overscan setting */
                fgScartModeChg = SV_TRUE;
	    }
	    /* considering when transition between MIX <-> RGB/CVBS
	     * NOT to change the PQ setting */
	    if((bPrevScartAutoMode == _bScartAutoMode) &&
	        (bPrevScartAutoMode == SCART_COLOR_MIX))
	    {
	        if(bScartMixCnt < 254)
		{
	            bScartMixCnt++;
		}
	    }
	    else
	    {
	        bScartMixCnt=0;
	    }

        }
    }

    if(fgScartModeChg != FALSE)
    {
        /* trigger tvd mode change */
        vTvd3dReset();
    }

  #if CHANGE_SDDS_KPI
     if(fgIsCLKLock() && _bClkTrigger)
     {
         vDrvClkSetLockBandwidth(SV_ON);
         _bClkTrigger = 0 ;
#if SCART_DBG_MSG         
         Printf("------- SV_ON @ vScartEvtchk ------\n");
#endif
     }
   #endif
}

void vScartChangeMode(UINT8 bICIn)
{
    UINT8 bSFisSV;
    
#if SCART_DBG_MSG
    Printf("vScartChangeMode %d\n", _bUIScartMode);
#endif
    _bTrigScartAuto = SV_FALSE;

    if (bICIn == P_FB0 || bICIn == P_FB1)
    {
        vRegWriteFldAlign(SCART_01, 0, FLD_FB_FORCE_ON);    // clear the force On
        vRegWriteFldAlign(SCART_01, 0, FLD_FB_FORCE_OFF);   // clear the force Off          

        if (_bUIScartMode == SV_SCART_CVBS_RGB)
        {
#if SCART_DBG_MSG
            Printf("Manual CVBS/RGB\n");
#endif
            vScartMix();
            bSFisSV = SV_FALSE;
        }
        else if (_bUIScartMode == SV_SCART_SV)
        {
#if SCART_DBG_MSG
            Printf("Manual SV\n");
#endif
            vScartSvConnect();
            bSFisSV = SV_TRUE;
            vRegWriteFldAlign(SCART_01, 1, FLD_FB_FORCE_OFF);       // clear the force On
        }
        else if (_bUIScartMode == SV_SCART_RGB) //Gene Chang For the SCART 8202B_E2 FB modification 2006.07.05
        {
            // force CVBS/RGB mode and mute CVBS , huahua update 20061130
#if SCART_DBG_MSG
            Printf("Manual RGB\n");
#endif
            vScartMix();
            bSFisSV = SV_FALSE;
            vRegWriteFldAlign(SCART_01, 1, FLD_FB_FORCE_ON);        // set the force On
        }
        else if (_bUIScartMode == SV_SCART_CVBS)        //Gene Chang For the SCART 8202B_E2 FB modification 2006.07.05
        {
#if SCART_DBG_MSG
            Printf("Manual CVBS\n");
#endif
            vScartMix();
            bSFisSV = SV_FALSE;
            vRegWriteFldAlign(SCART_01, 1, FLD_FB_FORCE_OFF);       // clear the force On
        }
        else                    //if (_bUIScartMode == SV_SCART_AUTO)
        {
#if SCART_DBG_MSG
            Printf("Auto\n");
#endif
            _bTrigScartAuto = SV_TRUE;
            bSFisSV = SV_FALSE;
	     _bScartAutoMode = SCART_COLOR_CVBS;	
            //force init to cvbs mode for auto/svideo mode fast change bug
            vScartMix();

        }

        if(_bOldSFisSV != bSFisSV)
        {
            if(bSFisSV != SV_FALSE)
            {
                vTvd3dSVInit(SV_ON);
            }
            else
            {
                vTvd3dSVInit(SV_OFF);
            }
        }

        _bSFisSV = bSFisSV;
        _bOldSFisSV = bSFisSV;
        vTvd3dReset();
#if 0 //5387
        vApiVideoBypass(bICIn);
#endif
    }
}

void vScartUIChangeInputMode(UINT8 bScartMode)
{
#if SCART_DBG_MSG
    Printf("vScartUIChangeInputMode: %d\n", bScartMode);
#endif

		if (_bMainICIn == P_FB0) 
		{
			if(bScartMode != _bUIScartMode)
			{	
				_vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_MAIN, FOREVER_MUTE, FALSE);
				_bUIScartMode = bScartMode;
				vScartChangeMode(P_FB0);
			}
		}
		else if (_bMainICIn == P_FB1)
		{
			if(bScartMode != _bUIScartMode)
			{	
				_vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_MAIN, FOREVER_MUTE, FALSE);
				_bUIScartMode = bScartMode;
				vScartChangeMode(P_FB1);
			}
		}
		else if (_bSubICIn == P_FB0)
		{
			if(bScartMode != _bUIScartMode)
			{	
				_vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_PIP, FOREVER_MUTE, FALSE);
				_bUIScartMode = bScartMode;
				vScartChangeMode(P_FB0);
			}
		}
		else if (_bSubICIn == P_FB1)
		{
			if(bScartMode != _bUIScartMode)
			{	
				_vDrvVideoSetMute(MUTE_MODULE_MODECHG, SV_VP_PIP, FOREVER_MUTE, FALSE);
				_bUIScartMode = bScartMode;
				vScartChangeMode(P_FB1);
			}
		}
	}


UINT8 bDrvGetScartFSStatus(UINT8 bSrcIdx)
{
    UINT8 bStatus, bData;
    UINT8 bSCARTStatus, bServoADCNum;
    UINT8 *pbSCARTStatusPre;
    UINT8 *pbSCARTPollingCnt;
    UINT8 bPin8LowThres, bPin8HiThres ;
    UINT32 ui4ScartFsAdcNum=0;
    
    static UINT8 _bSCARTStatusPre[2]={0,0};
    static UINT8 _bSCARTPollingCnt[2]={0,0};

    switch (bSrcIdx)
    {
    case SV_VS_SCART1:
		if(0 == DRVCUST_OptQuery(eScart1FSServoAdc,&ui4ScartFsAdcNum))
		{
			SERVOADC_SCART1_FS =(UINT8)ui4ScartFsAdcNum;
		}
		else
		{
		        return (FS_NO_SUPPORT);
		}
	break;
	
    case SV_VS_SCART2:
		if(0 == DRVCUST_OptQuery(eScart2FSServoAdc,&ui4ScartFsAdcNum))
		{
			SERVOADC_SCART2_FS =(UINT8)ui4ScartFsAdcNum;
		}
		else
		{
		        return (FS_NO_SUPPORT);
		}
	break;
    default:
        return (FS_NO_SUPPORT);
    } 
   
    bPin8LowThres = DRVCUST_OptGet(eSCARTPin8LowThres);
    bPin8HiThres = DRVCUST_OptGet(eSCARTPin8HiThres);
    

    vDrvSRVADEnable();

    switch (bSrcIdx)
    {
    case SV_VS_SCART1:
        bSCARTStatus = _bSCART1Status;
        bServoADCNum = SERVOADC_SCART1_FS;
        pbSCARTStatusPre = &_bSCARTStatusPre[0];
        pbSCARTPollingCnt = &_bSCARTPollingCnt[0];
        break;

    case SV_VS_SCART2:
        bSCARTStatus = _bSCART2Status;
        bServoADCNum = SERVOADC_SCART2_FS;
        pbSCARTStatusPre = &_bSCARTStatusPre[1];
        pbSCARTPollingCnt = &_bSCARTPollingCnt[1];
        break;

    default:
        return (FS_NO_SUPPORT);
    }

    if (bServoADCNum == 0xff)
    {
        return (FS_NO_SUPPORT);
    }

    if(0 == (*pbSCARTPollingCnt % SCART_PIN8_STS_WITH_VSYNC))
    {
        bStatus = bDrvReadSRVAD(bServoADCNum);      //ServoADCNum for SCART FS
    }
    else
    {
        bStatus = *pbSCARTStatusPre;
    }
    
    if(DIFF(bStatus, *pbSCARTStatusPre) < 3)
    {
        if(*pbSCARTPollingCnt < SCART_STATUS_COUNT_THRES)
        {
            (*pbSCARTPollingCnt)++;
        }
        else // when count is reach the threshold
    {
            /* to make the polling less busy */
            (*pbSCARTPollingCnt) -= (SCART_PIN8_STS_WITH_VSYNC-1);
        }
    }
    else
    {
        *pbSCARTPollingCnt=0;
        *pbSCARTStatusPre = bStatus;
    }

    /* wait the FS status stable, or not to notify */
    if(*pbSCARTPollingCnt < SCART_STATUS_COUNT_THRES)
    {
        return (FS_NO_CHANGE);
    }

    /* 8222/536x Servo AD status consist only 6bit instead of 8bit (in MT8226) */
    //bStatus <<= 2;

    // Use 50k and 10k resistance to make 12V -> 2V (Servo ADC upper voltage) for 5387
	//spec > 9.5V : 4:3  , 4.5 to 7V : 16:9
    if (bStatus >= bPin8HiThres)         // 8.5V :32 
    {
        bData = 0x02;
        _bScartPin8ASR_save[bSrcIdx - SV_VS_SCART1] = SV_WS_4W_3H;
    }
    else if ((bStatus < bPin8HiThres) && (bStatus >= bPin8LowThres))        // between 3V:11~8.5V)
    {
        bData = 0x01;
        _bScartPin8ASR_save[bSrcIdx - SV_VS_SCART1] = SV_WS_16W_9H;
    }
    else
    {
        bData = 0x00;
        _bScartPin8ASR_save[bSrcIdx - SV_VS_SCART1] = SV_WS_MAX;
    }

    if ((bSCARTStatus & 0x03) != bData) //old != new
    {
        UINT8 ret;

        if ((bSCARTStatus & 0x03) == 0x00)      //old==off
        {
            ret = (FS_OFF_TO_ON);
        }
        else if (bData == 0x00) //new ==off
        {
            ret = (FS_ON_TO_OFF);
        }
        else
        {
            ret = (FS_SCALE_CHANGE);
        }

        bSCARTStatus = (bSCARTStatus & 0xfc) | bData;
        switch (bSrcIdx)
        {
        case SV_VS_SCART1:
            _bSCART1Status = bSCARTStatus;
            break;

        case SV_VS_SCART2:
            _bSCART2Status = bSCARTStatus;
            break;

        default:
            break;
        }
        return ret;
    }

    return (FS_NO_CHANGE);
}

UINT8 bDrvGetScartPin8ASR(UINT8 bSrcIdx)
{
    if(bSrcIdx != SV_VS_SCART1 && bSrcIdx != SV_VS_SCART2)
    {
        return SV_WS_MAX;
    }
    return _bScartPin8ASR_save[bSrcIdx - SV_VS_SCART1];
}
#endif

UINT8 bDrvGetScartFBStatus(void)
{
    return u1RegRead1B(SCART_STA_00_3);
}

UINT8 bDrvGetScartInputMode(void)
{
    UINT8 bRetScartMode;

    switch (_bUIScartMode)
    {
    default:
    case SV_SCART_AUTO:    
    case SV_SCART_CVBS_RGB:
        bRetScartMode = _bScartAutoMode;
        break;
    case SV_SCART_CVBS:
        bRetScartMode = SCART_COLOR_CVBS;
        break;
    case SV_SCART_RGB:
        bRetScartMode = SCART_COLOR_RGB;
        break;
    case SV_SCART_SV:
        bRetScartMode = SCART_COLOR_SV;
        break;
    }

    /* if SCART FB is force OFF, there is no RGB input */
    if( RegReadFldAlign(SCART_01, FLD_FB_FORCE_OFF) != 0 )
    {
        if(bRetScartMode == SCART_COLOR_RGB || bRetScartMode == SCART_COLOR_MIX)
        {
            bRetScartMode = SCART_COLOR_CVBS;
        }
    }

    return bRetScartMode;
}

UINT8 bDrvGetScartAutoModeStatus(void)
{
    vScartUpdateAutoMode();
    
    return _bScartAutoMode;
}



void vScartReadFsSrvAdc(void)
{
    UINT8  bStatus = 0 ;
    UINT32 ui4ScartFsAdcNum=0, ui4ScartThre = 0;
	
	if(0 == DRVCUST_OptQuery(eScart1FSServoAdc,&ui4ScartFsAdcNum))
	{
	    bStatus = bDrvReadSRVAD((UINT8)ui4ScartFsAdcNum);      //ServoADCNum for SCART FS
	    Printf("Read Scart1 FS value = %d\n", bStatus);
	}
  
	if(0 == DRVCUST_OptQuery(eScart2FSServoAdc,&ui4ScartFsAdcNum))
	{
	    bStatus = bDrvReadSRVAD((UINT8)ui4ScartFsAdcNum);      //ServoADCNum for SCART FS
	    Printf("Read Scart2 FS value = %d\n", bStatus);
	}

	if(0 == DRVCUST_OptQuery(eSCARTPin8HiThres,&ui4ScartThre))
	{	   
	    Printf("Read Scart1 FS hi threshold = %d\n", ui4ScartThre);
	}
  
	if(0 == DRVCUST_OptQuery(eSCARTPin8LowThres,&ui4ScartThre))
	{ 
	    Printf("Read Scart2 FS hi threshold = %d\n", ui4ScartThre);
	}	
}
