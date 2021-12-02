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
/*************************************************************************
*
* Filename:
* ---------
*
* Description:
* ------------
*
* Author:
* -------
*    Justin Wu
*
*************************************************************************/
#include "atd_feature.h"
#include "pi_demod_atd.h"
//#include "pi_demod_atdeu.h"
#include "pi_def_atd.h"
#include "PD_Def_dbg.h"  /*need include RISC IF macro, if re-ware RISC IF , remove this*/
#include "drvcust_if.h"
#include "drv_tvd.h"
#include "pd_atd_glue.h"
#include "drvapi_tuner.h"  

/***********************************************************************/
/*                  Defines                                            */
/***********************************************************************/

#define fcTIMING_MEASURE  0
#define NON_AFT_RANGE_FN_KHZ (INT16)(350) //d111107_Haibo:Non-aft range is 350KHz when FN exist

/***********************************************************************/
/*                  Private Types                                      */
/***********************************************************************/

#if defined(ATD_SNOWSCREEN_MODE) 
STATIC UINT32 u4FreqTemp = 900000;
STATIC UINT32 wPreLockFreq = 900000;
#endif
#if TUNER_DTV_NOTIFY_ENABLE	
    BOOL gfgNFYENABLE = TRUE;	
#endif
/**********************************************************************/
UINT8 ATDEU_SearchNearbyFreq(ATD_CTX_T *psDemodCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct,
                                 void *pvArg, UINT32 u4FreqBDKHz)
{
    UINT32  wFRTmp;
    UINT8   bScanState, bAcqState = 0;
    UINT8   fgScanDir;
    INT32   s4NextFreqStep;
    UINT8   bATVStatus = 0;
    UINT8   u1_small_jump_cnt = 0;
	UINT16  u2BoundaryMore = 0;
#if TUNER_DTV_NOTIFY_ENABLE	
    BOOL fgChannelScan = TRUE;
    UINT32 wLastFreq = *wFreqInKHz;	
#endif
   UINT8 u1Count = 0;  // the count of jump times in 1 searchnearby function!

    psDemodCtx->isBreak = _BreakFct;

    //wDRTmp = *wFreqDR;
    // Justin, 070830, make sure wFRTmp is in 50k Hz base
    wFRTmp = (*wFreqInKHz *1000) / 50000;
    wFRTmp *= (50000 /1000);

    mcSHOW_DBG_MSG(("SearchNearBy: Freq = %d kHz, FreqBD = %d kHz \n", wFRTmp, u4FreqBDKHz));

    fgScanDir = (*wFreqInKHz > u4FreqBDKHz)?SCAN_DIRECTION_DEC:SCAN_DIRECTION_INC;
	
	if((psDemodCtx->pTCtx->specific_member.eu_ctx.m_s4AnaFreqBoundUpper/1000)==u4FreqBDKHz)
	{
		u2BoundaryMore = 1000;//when u4FreqBDKHz == upper freq boundary, more check 300KHz
	}

#if fcTIMING_MEASURE
    mcSHOW_DBG_MSG(("[ATuner] SearchNearby.0=%3u ms",
             (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD()));
    mcSHOW_DBG_MSG((", %d\n", psDemodCtx->u1VIFLock));
#endif

  // Initial check to see if curent freq need full range AFT or not
    if (*wFreqInKHz == u4FreqBDKHz)
    {
#if TUNER_DTV_NOTIFY_ENABLE
	   fgChannelScan = FALSE;   // AFT process,disable Tuner Frequency notify.
#endif
        ATD_GetVIFLock(psDemodCtx, TRUE);
        if ((psDemodCtx->u1VIFLock >= cATD_VIF_FULL_LOCK))
        {
            if(fgGetTVDSts(psDemodCtx,CHN_LOCK_CHECK_TIME))
            {
                ATD_SetNormalMode(psDemodCtx); //EC_code_dA414: move from ATD_Patch                   
                mcSHOW_DBG_MSG(("[ATuner] AFT OK and TVD lock!\n"));
                return (TRUE);
            }
            else
            {
                (wFRTmp -= TV_FINE_TUNE_THRESHOLD_KHZ / 2);
                bScanState = TUNER_PLL_LOCK_STATE;
                psDemodCtx->u1PF = 0x01;
            }
        }
        else
        {
            (wFRTmp -= TV_FINE_TUNE_THRESHOLD_KHZ / 2);
            bScanState = TUNER_PLL_LOCK_STATE;
            psDemodCtx->u1PF = 0x01;
        }
      #if defined(ATD_SNOWSCREEN_MODE) || defined(TVD_SNOWSCREEN_MODE)  
	 ATD_SetSnowMode(psDemodCtx,TRUE);//d20110719_Haibo:Set snow mode when do AFT
	  #endif
    }
    else
    {
        bScanState = TUNER_VIF_LOCK_STATE;
    }

    while (1)
    {

        if (psDemodCtx->isBreak(pvArg))
        {
            mcSHOW_DBG_MSG(("[ATuner] SearchNearby.BreakFct\n"));
            mcSET_BIT(bATVStatus, cpANA_LOCK_STS_BREAK);
            return bATVStatus;
        }

    //mcDELAY_MS(1);

    
        #if fcTIMING_MEASURE
            mcSHOW_DBG_MSG(("[ATuner] SearchNearby.1=%3u ms\n",
                        (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD()));
        #endif

        switch(bScanState)
        {
        case TUNER_PLL_LOCK_STATE:

	      u1Count ++;
	    mcSHOW_DBG_MSG(("The Searchnearby PLL conut = %d\n",u1Count));

	     if (u1Count >15)
	     	{
	     	*wFreqInKHz=wFRTmp;			
            mcSHOW_DBG_MSG(("Note!need to return to let ice know scan process wFreqInKHz=%d!\n",wFreqInKHz));
	     	//mcSHOW_DBG_MSG(("Note!There is probably a  scanning dead loop !\n"));
		return (FALSE) ; 	
	     	}
		 
            // Check if current freq is outside TV_FINE_TUNE_THRESHOLD window
            if (*wFreqInKHz == u4FreqBDKHz)// && ((wFRTmp > (*wFreqInKHz + TV_FINE_TUNE_THRESHOLD_KHZ/2 + 100)) || (wFRTmp < (*wFreqInKHz - TV_FINE_TUNE_THRESHOLD_KHZ/2 - 100)))  )
            {
                //xiaoming modify 130 from 100, most tuner step size is 62.5KHz 
                if(wFRTmp < (*wFreqInKHz - TV_FINE_TUNE_THRESHOLD_KHZ/2 - 130))
                {
                    wFRTmp = wFRTmp + 1500;
                    psDemodCtx->fgCheckCR = TRUE;
                    psDemodCtx->u1PF = 0x01;
                    u1_small_jump_cnt = 0;
                    mcSHOW_DBG_MSG(("[ATuner] Hit lower bound, add 1.5MHz keep searching!\n"));
                }
                else if(wFRTmp > (*wFreqInKHz + TV_FINE_TUNE_THRESHOLD_KHZ/2 + 100))
            {
                //Hit upper bound or lower bound
                return (FALSE);
            }
            }

            if ((*wFreqInKHz != u4FreqBDKHz) && (fgScanDir == SCAN_DIRECTION_INC) )
            {
                if(wFRTmp > (u4FreqBDKHz + u2BoundaryMore))
                {
                    //Hit upper bound
                    *wFreqInKHz = wFRTmp;  //return final frequency, LC Chien, 070115
                    return (FALSE);
                }
                else if(wFRTmp < *wFreqInKHz - 1000)
                {
                	wFRTmp = *wFreqInKHz + 1500;
                    psDemodCtx->fgCheckCR = TRUE;
                    psDemodCtx->u1PF = 0x01;
                    u1_small_jump_cnt = 0;
                }
            }

            if ((*wFreqInKHz != u4FreqBDKHz) && (fgScanDir == SCAN_DIRECTION_DEC) )
            {
                if(wFRTmp < u4FreqBDKHz)
                {
                    //Hit lower bound
                    *wFreqInKHz = wFRTmp;  //return final frequency, LC Chien, 070115
                    return (FALSE);
                }
                else if(wFRTmp > *wFreqInKHz + 1000)
                {
                	wFRTmp = *wFreqInKHz - 1500;
                    psDemodCtx->fgCheckCR = TRUE;
                    psDemodCtx->u1PF = 0x01;
                    u1_small_jump_cnt = 0;
                }
            }

            // Update new freq into tuner part
            //mcSHOW_DBG_MSG(("MOMO LOG: SearchNearBy: PLL_LOCK_STATE: Freq = %d\n", wFRTmp));
    
            //DTOS40A_AnaTunerSetFreq(psAtvPiCtx, wFRTmp, MOD_ATSC_8VSB);
            ATD_SetFreq(psDemodCtx, wFRTmp, TRUE);
            //ATD_SetPF(psDemodCtx, psDemodCtx->u1PF);
            //ATD_SetScanMode(psDemodCtx);
            bAcqState = ATD_StartAcq(psDemodCtx, TRUE);
    
            if(bAcqState)
            {
               if(fgScanDir == SCAN_DIRECTION_INC)
                    {
                	    wFRTmp += 1500;
                    }
                    else
                    {
                	    wFRTmp -= 1500;
                    }
                    psDemodCtx->u1PF = 0x01;
                    psDemodCtx->fgCheckCR = TRUE;
                bScanState = TUNER_PLL_LOCK_STATE;
                u1_small_jump_cnt = 0;
            }
            else
            {
                bScanState = TUNER_VIF_LOCK_STATE;
            }
#if TUNER_DTV_NOTIFY_ENABLE
      if ((gfgNFYENABLE)  && (fgChannelScan==TRUE))
      	{
	   if ( (fgScanDir == SCAN_DIRECTION_INC) &&(wLastFreq <= wFRTmp))
	   	{	   	
		Tuner_Notify(DRV_COND_UPDATE,wFRTmp);
		mcSHOW_DBG_MSG(("Notify Channel scan Tuner freq change to %d\n",wFRTmp));
		wLastFreq = wFRTmp;
	   	}
	    else if ((fgScanDir == SCAN_DIRECTION_DEC) && (wLastFreq >= wFRTmp))
		{	   	
		Tuner_Notify(DRV_COND_UPDATE,wFRTmp);
		mcSHOW_DBG_MSG(("Notify Channel scan Tuner freq change to %d\n",wFRTmp));
		wLastFreq = wFRTmp;
	   	}	
	   else
	   	{
              mcSHOW_DBG_MSG(("Frequency %d reword back,drop it.\n",wFRTmp));
	   	}
      	}   
#endif    
            #if fcTIMING_MEASURE            
                    mcSHOW_DBG_MSG(("[ATuner] SearchNearby.PLL=%3u ms / %d\n",
                             (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD(),
                             (INT16) (wFRTmp - *wFreqInKHz)));
            #endif
          break;

      case TUNER_VIF_LOCK_STATE:

        #if ((defined CC_MT5365) || (defined CC_MT5395) || (defined CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389))
        mcDELAY_MS(100);	//Xiongfeng: add delay time to fix ATV channel loss
        mcSHOW_DBG_MSG(("VIF delay is 100ms\n"));
		#else
		mcDELAY_MS(10); //LC: 5388 needs more VIF stable time
		#endif

        ATD_GetVIFLock(psDemodCtx, TRUE);

        switch(psDemodCtx->u1VIFLock)
        {
            case cATD_VIF_UNLOCK:  //CR unlock or abs(CFO)>1MHz
                u1_small_jump_cnt = 0;
                if(fgScanDir == SCAN_DIRECTION_INC)
                {
            	    wFRTmp += 1500;
                }
                else
                {
            	    wFRTmp -= 1500;
                }
                bScanState = TUNER_PLL_LOCK_STATE;
                psDemodCtx->fgCheckCR = TRUE;
                psDemodCtx->u1PF = 0x01;
            break;

            case cATD_VIF_PARTIAL_LOCK:  //CR lock, but 50K<abs(CFO)<1MHz
                u1_small_jump_cnt++;                
                mcSHOW_DBG_MSG(("small_jump_cnt =%d\n",u1_small_jump_cnt ));			
	        if(u1_small_jump_cnt>10)
		{  
		    mcSHOW_DBG_MSG(("small_jump_cnt timeout!!!\n"));
		    psDemodCtx->fgCheckCR = TRUE;
                    psDemodCtx->u1PF = 0x01;
                    u1_small_jump_cnt = 0;
		    if(fgScanDir == SCAN_DIRECTION_INC)
		    {
                        wFRTmp += 1500;			
                    }
		    else
		    {
                        wFRTmp -= 1500;
                    } 
                    bScanState = TUNER_PLL_LOCK_STATE;
		    break;
		}
                
                psDemodCtx->u1PF = 0x00;
				
                #if ((defined CC_MT5365) || (defined CC_MT5395) || (defined CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389))
		    	psDemodCtx->fgCheckCR = TRUE; //xiaoming: alway check CR 
		    	#else
				psDemodCtx->fgCheckCR = FALSE;
				#endif
				
                s4NextFreqStep = (psDemodCtx->s4Cfo*1000)/50000;
                s4NextFreqStep *= (50000/1000);
                if(psDemodCtx->u1SubSysIdx ==MOD_SECAM_L1)
                {
                    wFRTmp -= s4NextFreqStep;
                }
                else
                {
                    wFRTmp += s4NextFreqStep;
                }

                bScanState = TUNER_PLL_LOCK_STATE;
            break;

            case cATD_VIF_FULL_LOCK:  //VIF lock
                u1_small_jump_cnt = 0;                            
                //*wFreqInKHz = wFRTmp;               
                #if fcTIMING_MEASURE
                mcSHOW_DBG_MSG(("[ATuner] SearchNearby.VIF=%3u ms\n",
                            (mcGET_SYS_TICK() - u2TickStart) * mcGET_TICK_PERIOD()));
                #endif
            break;
        }

        if(psDemodCtx->u1VIFLock == cATD_VIF_FULL_LOCK)
        {
            if(*wFreqInKHz == u4FreqBDKHz) 
            {
                if(fgGetTVDSts(psDemodCtx,CHN_LOCK_CHECK_TIME))
                {
                    ATD_SetNormalMode(psDemodCtx);  //EC_code_dA414: move from ATD_Patch                   
                    *wFreqInKHz = wFRTmp;
                    mcSHOW_DBG_MSG(("[ATuner] AFT OK and TVD lock!\n"));
                    return (TRUE);
                }
                else
                {
                    wFRTmp += 1500;
                    bScanState = TUNER_PLL_LOCK_STATE;
                    psDemodCtx->fgCheckCR = TRUE;
                    psDemodCtx->u1PF = 0x01;
                    mcSHOW_DBG_MSG(("[ATuner] AFT OK, but TVD unlock!\n"));
                }
            }
            else
            {
            *wFreqInKHz = wFRTmp;	
            return (TRUE);
        }
        }

        break;

        default:
        break;
    } //end of state machine
  } //end of while loop

}



UINT8 ATDEU_Monitor(ATD_CTX_T *psDemodCtx, UINT32 *wFreqInKHz, x_break_fct _BreakFct, void *pvArg)
{
#ifdef fc_DISABLE_AFT

    return 1;

#else
    
    UINT32 wFreqDR = *wFreqInKHz;    
    UINT8  ucVpress;    
    UINT16  u2LOStep;
    INT16 s2FullVIFLockRange,s2PartiVIFLockRange;
	
	psDemodCtx->isBreak = _BreakFct;

    u2LOStep=psDemodCtx->pTCtx->u2LO_Step;
	s2FullVIFLockRange=(INT16)(u2LOStep/100);
	if(s2FullVIFLockRange<cATD_VIF_FULL_LOCK_MIN_RANGE)
		s2FullVIFLockRange=cATD_VIF_FULL_LOCK_MIN_RANGE;
	s2PartiVIFLockRange = s2FullVIFLockRange*2;

    #if defined(ATD_SNOWSCREEN_MODE)
    UINT8 ucData;    
    if(wPreLockFreq != *wFreqInKHz)
    {
    	mcSHOW_DBG_MSG(("[ATuner] 1st time Monitor\n"));
        u4FreqTemp = psDemodCtx -> u4Frequency;
    }
    wPreLockFreq = *wFreqInKHz;
    #endif
    
    if (psDemodCtx->isBreak(pvArg))
    {
        mcSHOW_DBG_MSG(("[ATuner] Monitor.BreakFct\n"));
        return (FALSE);
    }
    else if(!psDemodCtx->fgAFT)
    {
    	mcSHOW_DBG_MSG2(("Do not do AFT in monitor!\n"));
    	return TRUE;
    }
    else
    {
        psDemodCtx->fgCheckCR = TRUE;
        ucVpress = (IO_READ8(VIDEO_IN0_BASE, 0x81) & 0x10) >> 4;
          
        if(ATD_GetVIFLock(psDemodCtx, FALSE)<2)
        {
            mcSHOW_DBG_MSG(("[ATuner] VIF unlock!\n"));
            if( (psDemodCtx->u1VIFLock == 1) && (psDemodCtx->s4Cfo > -s2PartiVIFLockRange) && (psDemodCtx->s4Cfo < s2PartiVIFLockRange) )
            {
            	mcSHOW_DBG_MSG(("[ATuner] CFO > %dKhz but < %dKHz!\n",s2FullVIFLockRange,s2PartiVIFLockRange));
                return TRUE;
            }            
            else if((psDemodCtx->s4Cfo > -s2FullVIFLockRange) && (psDemodCtx->s4Cfo < s2FullVIFLockRange) && (ucVpress == 0x01))
            {
            	mcSHOW_DBG_MSG(("[ATuner] CFO <50KHz, Vpress lock, poor pilot quality!\n"));
                return TRUE;
            }            	           
            else if((ATD_GetPnFn(psDemodCtx) & MSK_ATD_CR_STATE1_FNIND) && \
			        (psDemodCtx->u1VIFLock == 1) && \
			        (psDemodCtx->s4Cfo > -NON_AFT_RANGE_FN_KHZ) && \
				    (psDemodCtx->s4Cfo < NON_AFT_RANGE_FN_KHZ) && \
                    (ucVpress == 0x01))
            {
                mcSHOW_DBG_MSG(("[ATuner] CFO=%d KHz(|CFO|<%dKHz), FN case!\n",psDemodCtx->s4Cfo,NON_AFT_RANGE_FN_KHZ));
                return TRUE;			
            }
            else
            {
            	ATD_SetSystem(psDemodCtx, psDemodCtx->u1SubSysIdx);            	            
            	ATD_SetFreq(psDemodCtx, *wFreqInKHz, TRUE);
            	psDemodCtx->u1PF = 0x01;
                ATD_StartAcq(psDemodCtx, TRUE);
                psDemodCtx->fgCheckCR = TRUE;
                if(ATDEU_SearchNearbyFreq(psDemodCtx, &wFreqDR,_BreakFct, pvArg, *wFreqInKHz))
                {                    
                    mcSHOW_DBG_MSG(("[ATuner] New Freq = %dKhz\n", psDemodCtx -> u4Frequency));  
                    return TRUE;
                }
                else
                {                    
                    #if defined(ATD_SNOWSCREEN_MODE)
                    if(u4FreqTemp != *wFreqInKHz)
                        ATD_SetFreq(psDemodCtx, u4FreqTemp, TRUE);                    	    
                    else
                    ATD_SetFreq(psDemodCtx, *wFreqInKHz, TRUE);                    	    
                    
                    psDemodCtx->u1PF = 0x00;
                    ucData = 0x24;                    
                    ATD_SetReg(psDemodCtx, REG_ATD_PE_CFOK, &ucData, 1);
                    ATD_StartAcq(psDemodCtx, FALSE);
                    mcSHOW_DBG_MSG(("[ATuner] AFT fail!\n"));
                    return FALSE;
                    #else 
                    ATD_SetFreq(psDemodCtx, *wFreqInKHz, TRUE); 
                    ATD_StartAcq(psDemodCtx, FALSE);                   	    
                    mcSHOW_DBG_MSG(("[ATuner] AFT fail!\n"));
                    return FALSE;
                    #endif 
                }
            }            
        }
        else
        {            
            #if defined(ATD_SNOWSCREEN_MODE) 
            u4FreqTemp = psDemodCtx -> u4Frequency;
            #endif
            mcSHOW_DBG_MSG2(("[ATuner] AFC in range!\n"));            
            return TRUE;
        }
    }

#endif
}


/**********************************************************************/
#if TUNER_DTV_NOTIFY_ENABLE	
VOID ATDEU_SetFreqAutoNFY(BOOL fgAutoNFY)
{
  gfgNFYENABLE = fgAutoNFY;
  mcSHOW_DBG_MSG2(("gfgNFYENABLE = TRUE\n"));
}
#else
VOID ATDEU_SetFreqAutoNFY(BOOL fgAutoNFY)
{
  mcSHOW_DBG_MSG(("TUNER_ATV_NOTIFY_ENABLE not open in driver\n"));
}

#endif


