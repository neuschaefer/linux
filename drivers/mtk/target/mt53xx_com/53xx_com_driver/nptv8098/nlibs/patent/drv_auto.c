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
 * $RCSfile: drv_auto.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#define _DRV_AUTO_

#ifndef CC_MTK_LOADER

/******************************************************************************
 * Header Files
 *****************************************************************************/
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "video_def.h"
#include "hw_vdoin.h"
#include "hw_async.h"
#include "hw_vfe.h"
#include "drv_auto.h"
#include "vga_auto.h"
#include "drv_vga.h"
#include "drv_hdtv.h"
#include "vga_table.h"
#include  "drv_vdoclk.h"
#include "api_eep.h"
#include "util.h"
#include "drv_async.h"
#include "hw_hdtv.h"
#include "x_printf.h"
/******************************************************************************
 * Macro, Type Definition, and Variable Declaratoin
 *****************************************************************************/

//temp

extern UINT16   wSP0Vtotal;
UINT32 dVGAGetAllDiffValue_peak(void);

/******************************************************************************
 * Function Forward Declaration
 *****************************************************************************/
/**
 * @brief u1DrvVGAGetPhase(void )
 * Get phase value  for VGA
 * @param  NONE
 * @retval Current phase value
 */

UINT8 u1DrvVGAGetPhase(void)
{
	return RegReadFldAlign(ASYNC_0F, AS_PHASESEL_RX);
}


UINT16 wDrvVGAGetClock(void) //127 offset

{
	UINT8 mode;

	if (fgIsMainVga()||fgIsPipVga())
	{
		mode=_bVgaTiming;
	}
	else
	{
		mode=_bHdtvTiming;
	}

	if ((mode>0)&&(mode<253))
	{
		return  (RegReadFldAlign(VFE_16, DCLK_HLINE_LENGTH)+1+127)-Get_VGAMODE_IHTOTAL(mode) ;
	}
	else
	{
		return 127;
	}
}

UINT8 wDrvVgaGetClockMax(void)
{
	UINT8 mode;

	if (fgIsMainVga()||fgIsPipVga())
	{
		mode=_bVgaTiming;
	}
	else
	{
		mode=_bHdtvTiming;
	}

	if(fgIsValidTiming(mode))
	{
	    UINT16 u2MaxHtotal ;
	    UINT16 u2Clock ;
	    
            u2MaxHtotal = 1690*1000/wSP0Hclk ;  
            u2Clock = u2MaxHtotal - Get_VGAMODE_IHTOTAL(mode) ;
             //Printf("htotal %d %d %d\n", u2MaxHtotal, Get_VGAMODE_IHTOTAL(mode), u2Clock );
            if(u2Clock < 128)
            {
                return (UINT8) u2Clock ;
            }
            else
            {
                return 128 ;
            }
	}
	else
	{

            return 128 ;
	}
}


UINT16 wDrvVGAHPositionMin(void)
{
	//real min = 3
	return 10; //for H/V align , 1 line different 
}

UINT16 wDrvVGAHPositionMax(void)
{
	UINT8 mode;

	if (fgIsMainVga()||fgIsPipVga())
	{
		mode=_bVgaTiming;
	}
	else
	{
		mode=_bHdtvTiming;
	}

	if ((mode>0)&&(mode<253))
	{
		//HTOTAL always get it from table, should be safe - width
		UINT16 HMaxValue;

		if (fgIsVideoTiming(mode))
		{
		#if 0
			if ((mode == MODE_525I)||(mode==MODE_625I)||(mode==MODE_525I_OVERSAMPLE)||(mode == MODE_625I_OVERSAMPLE))
			{
				HMaxValue = (Get_VGAMODE_IHTOTAL(mode)-(Get_VGAMODE_IPH_WID(mode)*2)); // Modify larger tolerance H-position adjust space for ambiguous mode limited by UI flow by W.C Shih 2006/11/23
			}
			else
                #endif			
			{
				HMaxValue = (Get_VGAMODE_IHTOTAL(mode)-Get_VGAMODE_IPH_WID(mode));
			}
		}
		else
		{
			//HMaxValue = (Get_VGAMODE_IHTOTAL(mode)-Get_VGAMODE_IPH_WID(mode))*4/3; // Modify larger tolerance H-position adjust space for ambiguous mode limited by UI flow by W.C Shih 2006/11/23

			if (Get_VGAMODE_IPH_WID(mode)<=800)
			{
				HMaxValue = (Get_VGAMODE_IHTOTAL(mode)-Get_VGAMODE_IPH_WID(mode)); // Modify larger tolerance H-position adjust space for ambiguous mode limited by UI flow by W.C Shih 2006/11/23
			}
			else
			{
				HMaxValue = ((Get_VGAMODE_IHTOTAL(mode)-Get_VGAMODE_IPH_WID(mode))*4)/3; // Modify larger tolerance H-position adjust space for ambiguous mode limited by UI flow by W.C Shih 2006/11/23
			}

		}

		return HMaxValue; //Get_VGAMODE_IHTOTAL(mode)-Get_VGAMODE_IPH_WID(mode);
	}
	else
	{
		return wDrvVGAHPositionMin();
	}
}


UINT16 wDrvVGAVPositionMin(void)
{
	UINT8 mode;

	if (fgIsMainVga()||fgIsPipVga())
	{
		mode=_bVgaTiming;
	}
	else
	{
		mode=_bHdtvTiming;
	}

	if(Get_VGAMODE_INTERLACE(mode))
	{
		return 6;
	}
	else
	{
		return 3;	//assume Vsout[0-3],5371 vsout must >=3
	}
}

UINT16 wDrvVGAVPositionMax(void)
{
	UINT8 mode;

	if (fgIsMainVga()||fgIsPipVga())
	{
		mode=_bVgaTiming;
	}
	else
	{
		mode=_bHdtvTiming;
	}


	if(Get_VGAMODE_INTERLACE(mode))
	{
		return Get_VGAMODE_IPV_STA(mode)*2-8;
	}

	if ((mode>0)&&(mode<253))
	{
		UINT16 ret,min;
		
		//assume wSP0Vtotal>Get_VGAMODE_IPV_LEN(mode)
		min=wDrvVGAVPositionMin();
		ret=((Get_VGAMODE_IVTOTAL(mode)-Get_VGAMODE_IPV_LEN(mode)) + Get_VGAMODE_IPV_STA(mode)) -3;

		if ((ret<min)||(Get_VGAMODE_IVTOTAL(mode) <= Get_VGAMODE_IPV_LEN(mode)))
		{
			return min;
		}

		if (ret>(Get_VGAMODE_IVTOTAL(mode)>>1))
		{
			return (Get_VGAMODE_IVTOTAL(mode)>>1);
		}

		return ret;
	}
	else
	{
		return wDrvVGAVPositionMin();
	}

}


UINT16 wDrvVGAGetHPosition(void)
{

	UINT8 mode;

	if (fgIsMainVga()||fgIsPipVga())
	{
		mode=_bVgaTiming;
	}
	else
	{
		mode=_bHdtvTiming;
	}

	if ((mode>0)&&(mode<253))
	{

		return RegReadFldAlign(HDTV_01, HDTV_AV_START);
	}
	else
	{
		return 0;
	}
}

UINT16 wDrvVGAGetVPosition(void)
{
	UINT8 mode;

	if (fgIsMainVga()||fgIsPipVga())
	{
		mode=_bVgaTiming;
	}
	else
	{
		mode=_bHdtvTiming;
	}

	if ((mode>0)&&(mode<253))
	{
		return RegReadFldAlign(ASYNC_11, AS_NEW_VS_OUTP_S1);
	}
	else
	{
		return 2;
	}

}

void vDrvVGASetPhase_Simple(UINT8 bVal)
{
#if Phase3Channel
UINT8 bPhase_diff, bCnt;
UINT8 _bPhase[CHANNEL_NUM];
        if (fgIsAutoFlgSet(SP0_AUTO_CLOCK))
        {
              _bPhase[0]= bVal;
              _bPhase[1]= bVal;
              _bPhase[2]= bVal;
        }
        else
        {
   	      _bPhase[0] = RegReadFldAlign(ASYNC_0F,AS_PHASESEL_RX);
	      _bPhase[1] = RegReadFldAlign(ASYNC_0F,AS_PHASESEL_GX);
	      _bPhase[2] = RegReadFldAlign(ASYNC_0F,AS_PHASESEL_BX);
	      if((_bPhase[0] == _bPhase[1]) && (_bPhase[0] == _bPhase[2]))
	      {
                  _bPhase[0]= bVal;
                  _bPhase[1]= bVal;
                  _bPhase[2]= bVal;
	      }
	      else 
	      {
                   bPhase_diff = (bVal>=_bPhase[0]) ? (bVal-_bPhase[0]) : (32-_bPhase[0] +bVal) ;
                   _bPhase[0]= bVal;
                   for (bCnt=1; bCnt<CHANNEL_NUM; bCnt++)
                  {
                       _bPhase[bCnt]= _bPhase[bCnt] + bPhase_diff;
                       if (_bPhase[bCnt] > 31)
                       {
                           _bPhase[bCnt] = _bPhase[bCnt]-32;
                       }
                   }
             }
         }
         vRegWriteFldAlign(ASYNC_0F, _bPhase[0] , AS_PHASESEL_RX);
         vRegWriteFldAlign(ASYNC_0F, _bPhase[1] , AS_PHASESEL_GX);
         vRegWriteFldAlign(ASYNC_0F, _bPhase[2] , AS_PHASESEL_BX);
#else
	vRegWriteFldAlign(ASYNC_0F, bVal , AS_PHASESEL_RX);
	vRegWriteFldAlign(ASYNC_0F, bVal , AS_PHASESEL_GX);
	vRegWriteFldAlign(ASYNC_0F, bVal , AS_PHASESEL_BX);
#endif	
}

UINT8 bDrvVGAGetPhase(void)
{
	return RegReadFldAlign(ASYNC_0F, AS_PHASESEL_RX);	
}

/**
 * @brief vDrvVGASetPhase( UINT8 bVal )
 * Set phase value  for VGA
 * @param  bVale - phase value 0~31
 * @retval void
 * @example vDrvVGASetPhase(0) - Set Phase 0 for VGA
 */
void vDrvVGASetPhase(UINT8 bVal)
{
	vDrvVGASetPhase_Simple(bVal);
	 _RETIMENeedReset = TRUE ;
}

/**
 * @brief vDrvVGASetBDDataTh( void )
 * Set Boundary threshold for SP0
 * @param  bTh - boundary threshold value
 * @retval void
 * @example vDrvVGASetBDDataTh(20) - set boundary data threshold 20
 */
void vDrvVGASetBDDataTh(UINT8 bTh)
{
	vRegWriteFldAlign(ASYNC_15, bTh, AS_BDDATATH);
}

/**
 * @brief vDrvVGASetBDCha( void )
 * Set measurement channel(VGA)
 * @param  bCha - Channel (R/G/B)
 * @retval void
 * @example vDrvVGASetBDCha(BD_RED) - switch boundary measurement channel to Red
 */
void vDrvVGASetBDCha(UINT8 bCha)
{
	vRegWriteFldAlign(ASYNC_13, bCha, AS_BDINSEL);
}

/**
 * @brief wDrvVGAGetLeftBound( void )
 * Get Left boundary value of VGA
 * @param  void
 * @retval Left boundary value
 * @example wDrvVGAGetLeftBound()
 */
UINT16 wDrvVGAGetLeftBound(void)
{
	return (RegReadFldAlign(STA_SYNC0_04, AS_LEFTBC_STA_S_11_0)+(RegReadFldAlign(STA_SYNC0_0A, AS_LEFTBC_STA_S_12)<<12));
}

/**
 * @brief wDrvVGAGetTopBound( void )
 * Get Top boundary value of VGA
 * @param  void
 * @retval Left boundary value
 * @example wDrvVGAGetTopBound()
 */
UINT16 wDrvVGAGetTopBound(void)
{
	return RegReadFldAlign(STA_SYNC0_06, AS_NEWTOPBC_S);
}

/**
 * @brief wDrvVGAGetRightBound( void )
 * Get right boundary value of VGA
 * @param  void
 * @retval Right boundary value
 * @example wDrvVGAGetRightBound()
 */
UINT16 wDrvVGAGetRightBound(void)
{
	return (RegReadFldAlign(STA_SYNC0_04, AS_RIGHTBC_STA_S_11_0)+(RegReadFldAlign(STA_SYNC0_0A, AS_RIGHTBC_STA_S_12)<<12));
}


/**
 * @brief wDrvVGAGetBottomBound( void )
 * Get Bottom  boundary value of VGA
 * @param  void
 * @retval Bottom boundary value
 * @example wDrvVGAGetBottomBound()
 */
UINT16 wDrvVGAGetBottomBound(void)
{
	return RegReadFldAlign(STA_SYNC0_06, AS_NEWBOTTOMBC_S);
}

/**
 * @brief dVGAGetDiffValue( UINT8 bChnl )
 * Get channel evaluation value of phase detection for VGA
 * @param  bChnl : R/G/B channel
 * @retval Channel phase evaluation Value
 * @example dVGAGetDiffValue(BD_RED) -Get SP0 red channel evaluation value
 */
//#if SUPPORT_MIX_PHASE_STA
PRIVATE UINT32 dVGAGetDiffValue(UINT8 bChnl)
{
	switch (bChnl)
	{

		case BD_RED:
			return RegReadFldAlign(STA_SYNC0_0A, AS_STA_R_S);


		case BD_GREEN:
			return RegReadFldAlign(STA_SYNC0_0B, AS_STA_G_S);


		case BD_BLUE:
			return RegReadFldAlign(STA_SYNC0_0C, AS_STA_B_S);


		default:
			return 0;
	}
}

//#else //SUPPORT_MIX_PHASE_STA
#if 1
PRIVATE UINT32 dVGAGetDiffValue_peak(UINT8 bChnl)
{
	switch (bChnl)
	{
		case BD_RED:
			return RegReadFldAlign(STA_SYNC0_07, AS_TOP_SUMRD_S);

		case BD_GREEN:
			return RegReadFldAlign(STA_SYNC0_08, AS_TOP_SUMGD_S);

		case BD_BLUE:
			return RegReadFldAlign(STA_SYNC0_09, AS_TOP_SUMBD_S);

		default:
			return 0 ;
	}

	//return 0;
}

//#endif //SUPPORT_MIX_PHASE_STA
/**
 * @brief dVGAGetAllDiffValue( void )
 * Get R/G/B channel evaluation sum value of phase detection for VGA
 * @param  void
 * @retval VGA phase evaluation value
 * @pre  call vDrvVGASetPhase(bVal) to set phase for phase detection
 * @pre  call vVGAPhaseModeSet() to set phase detection mode
 * @example dVGAGetAllDiffValue() -Get VGA all channel evaluation value
 */
UINT32 dVGAGetAllDiffValue_peak(void)
{
#if Phase3Channel
       UINT32 s;
	    _dwPhase3CH[0]=dVGAGetDiffValue_peak(BD_RED);
	    _dwPhase3CH[1]=dVGAGetDiffValue_peak(BD_GREEN);
	    _dwPhase3CH[2]=dVGAGetDiffValue_peak(BD_BLUE);
	    s= _dwPhase3CH[0]+_dwPhase3CH[1]+_dwPhase3CH[2];
       return s;
#else
	UINT32 r,g,b,s;
	r=dVGAGetDiffValue_peak(BD_RED);
	g=dVGAGetDiffValue_peak(BD_GREEN);
	b=dVGAGetDiffValue_peak(BD_BLUE);
	s=r+g+b;
    #if NEW_PHASE_ALGORITHM
         _dwPhase3CH[0]=s;
    #endif
	return s;
#endif	
}

#endif
UINT8 _bAutoPeakMode = 0 ;
UINT32 dVGAGetAllDiffValue(void)
{
#if Phase3Channel
	UINT32 s,p;
	if (_bAutoPeakMode)
	{
	    p=dVGAGetAllDiffValue_peak();
	    return p;
	}
	else
	{
           _dwPhase3CH[0]=dVGAGetDiffValue(BD_RED);
           _dwPhase3CH[1]=dVGAGetDiffValue(BD_GREEN);
           _dwPhase3CH[2]=dVGAGetDiffValue(BD_BLUE);
           s=_dwPhase3CH[0]+_dwPhase3CH[1]+_dwPhase3CH[2];
           return s;
       }

#else
	UINT32 r,g,b,s,p;
	UINT8 bTmpPhase;
	r=dVGAGetDiffValue(BD_RED);
	g=dVGAGetDiffValue(BD_GREEN);
	b=dVGAGetDiffValue(BD_BLUE);

	 if (fgIsAutoFlgSet(SP0_AUTO_CLOCK))
        { }
        else
        {
            bTmpPhase = (_bCurPhase == 0) ? 31 : (_bCurPhase -1);
            Printf(" Phase=%2u, R=%8u,  G=%8u,  B=%8u\n",bTmpPhase,r,g,b);
        }
        
	s=r+g+b;
	p=dVGAGetAllDiffValue_peak();

	if (_bAutoPeakMode)
	{
	#if NEW_PHASE_ALGORITHM
	_dwPhase3CH[0]= p;
	#endif
		return p;
	}
	else
	{
	#if NEW_PHASE_ALGORITHM
	_dwPhase3CH[0]= s;
	#endif
		return s;
	}
#endif
//	return (dVGAGetDiffValue(BD_RED) + dVGAGetDiffValue(BD_GREEN) + dVGAGetDiffValue(BD_BLUE));
}


//YPbPr Auto Phase 2006/11/07
UINT32 dHDTVGetAllDiffValue(void)
{
#if 1
	UINT32 r;


	r=dVGAGetDiffValue(BD_RED);
	//g=dVGAGetDiffValue(BD_GREEN);
	//b=dVGAGetDiffValue(BD_BLUE);
	return r;
	/*s=r+g+b;
	p=dVGAGetAllDiffValue_peak();
	if(p<8000)
		return s; //s>>3;
	return s;*/
#endif

}


/**
 * @brief vDrvVGASetTopThr( UINT8 bVal )
 * Set Threshold for  Top mode of phase detection for VGA
 * @param  bVal : Threshold value
 * @retval void
 * @example vDrvVGASetTopThr(20) -Set threshold 20 for phase detection in Top mode
 */
void vDrvVGASetTopThr(UINT8 bVal)
{
	vRegWriteFldAlign(ASYNC_0F, bVal, AS_TOP_THR);
}

/**
 * @brief vDrvVGASetPsneThr( UINT8 bVal )
 * Set Threshold for  psne mode of phase detection for VGA        __
 * @param  bThr1 : Threshold value 1                           __|
 * @param  bThr2 : Threshold value 2                                ____
 * @retval void
 * @example vDrvVGASetPsneThr(20,0) -Set threshold 20 for phase detection in psne mode
 */
//#if SUPPORT_MIX_PHASE_STA
void vDrvVGASetPsneThr(UINT8 bThr1, UINT8 bThr2)
{
	// Threshold 1 R/G/B
	vRegWriteFldAlign(ASYNC_16, bThr1, AS_PSNE_THB1);
	vRegWriteFldAlign(ASYNC_16, bThr1, AS_PSNE_THG1);
	vRegWriteFldAlign(ASYNC_16, bThr1, AS_PSNE_THR1);

	// Threshold 2 R/G/B
	vRegWriteFldAlign(ASYNC_17, bThr2, AS_PSNE_THB2);
	vRegWriteFldAlign(ASYNC_17, bThr2, AS_PSNE_THG2);
	vRegWriteFldAlign(ASYNC_17, bThr2, AS_PSNE_THR2);
}

//#endif //SUPPORT_MIX_PHASE_STA
/**
 * @brief vVGAPhaseModeSet( UINT8 bVal )
 * Set Top/psne mode for VGA  phase detection
 * @param  bMode  (1:Top mode, 0:psne Mode)
 * @retval void
 * @example vVGAPhaseModeSet(0) - Set psne mode for phase detection
 */
void vVGAPhaseModeSet(UINT8 bMode)
{
	vDrvVGASetTopThr(TOP_THRE);
//#if SUPPORT_MIX_PHASE_STA
	vDrvVGASetPsneThr(PSNE_THRE1, PSNE_THRE2);
	//select phase statistics => mix
	vRegWriteFldAlign(ASYNC_17, MIX_STA, AS_C_PSNE_STA_SEL);
	//vRegWriteFldAlign(ASYNC_00, PSNE_ONLY, PSNE_STA_SEL);
	//select phase add method => 1 Top + 2x psne
	vDrvVGASetPhsMix(PSNE2_ADD);
//#else
	//set default mode to top mode for peak status
	vRegWriteFldAlign(ASYNC_17, bMode, AS_TOP_PE_SW);
//#endif
}

//hardware auto support
/**
 * @brief vVgaHwAutoClkEnable( UINT8 bEnable )
 * Enable and disable hw auto clock
 * @param  bEnable (1:enable , 0:disable)
 * @retval void
 */

void vVgaHwAutoClkEnable(UINT8 bEnable)
{
	// reset the clock ready bit
	vRegWriteFldAlign(ASYNC_16, 1, AS_AUTO_CLK_RDY_CLR);
	vUtDelay2us(10); // 10us delay
	vRegWriteFldAlign(ASYNC_16, 0, AS_AUTO_CLK_RDY_CLR);

	// auto clock trigger enalbe and start/disable
	vRegWriteFldAlign(ASYNC_16, bEnable, AS_CLK_AUTO);
	vRegWriteFldAlign(ASYNC_16, bEnable, AS_CLKDET_INI);
}

/**
 * @brief vVgaHwAutoPhaseEnable( UINT8 bEnable )
 * Enable and disable hw auto phase
 * @param  bEnable (1:enable , 0:disable)
 * @retval void
 */

void vVgaHwAutoPhaseEnable(UINT8 bEnable)
{
	// reset the phase ready bit
	vRegWriteFldAlign(ASYNC_16, 1, AS_AUTO_PHASE_RDY_CLR);
	vUtDelay2us(10); // delay
	vRegWriteFldAlign(ASYNC_16, 0, AS_AUTO_PHASE_RDY_CLR);

	// reset the phase  trigger
	vRegWriteFldAlign(ASYNC_16, bEnable, AS_PHASE_AUTO);
	vRegWriteFldAlign(ASYNC_16, bEnable, AS_PHSDET_INI);

}

void vVgaHwAutoPhaseReset(void)
{
	vRegWriteFldAlign(ASYNC_16, 0, AS_PHSDET_INI);
	vRegWriteFldAlign(ASYNC_16, 1, AS_PHSDET_INI);
}

//================================DVI function===================//

#if 0//SUPPORT_DVI
// remove the phase summary in 538x by Adam 2007/04/04
/**
 * @brief vDrvDVISetBDDataTh( void )
 * Set Boundary threshold for SP1
 * @param  bTh - boundary threshold value
 * @retval void
 * @example vDrvDVISetBDDataTh(20) - set boundary data threshold 20
 */
void vDrvDVISetBDDataTh(UINT8 bTh)
{
	vRegWriteFldAlign(DSYNC_00, bTh, BDDATATH);
}

// **********************************************************
// Function : void vDrvDVISetBDCha(UINT8 bCha)
// Description :Set measurement channel(DVI)
// Parameter : bCha-> R/G/B
// Return    : None
// **********************************************************
/**
 * @brief vDrvDVISetBDCha( void )
 * Set measurement channel(SP1)
 * @param  bCha - Channel (R/G/B)
 * @retval void
 * @example vDrvDVISetBDCha(BD_RED) - switch boundary measurement channel to Red
 */
void vDrvDVISetBDCha(UINT8 bCha)
{
	vRegWriteFldAlign(DSYNC_00, bCha, AS_BDINSEL);
}

/**
 * @brief wDrvDVIGetLeftBound( void )
 * Get Left boundary value of SP1
 * @param  void
 * @retval Left boundary value
 * @example wDrvDVIGetLeftBound()
 */
UINT16 wDrvDVIGetLeftBound(void)
{
	return RegReadFldAlign(STA_SYNC1_04, LEFTBC);
}

/**
 * @brief wDrvVGAGetRightBound( void )
 * Get right boundary value of SP1
 * @param  void
 * @retval Right boundary value
 * @example wDrvVGAGetRightBound()
 */
UINT16 wDrvDVIGetRightBound(void)
{
	return RegReadFldAlign(STA_SYNC1_04, RIGHTBC);
}

/**
 * @brief wDrvVGAGetTopBound( void )
 * Get Top boundary value of SP1
 * @param  void
 * @retval Left boundary value
 * @example wDrvVGAGetTopBound()
 */
UINT16 wDrvDVIGetTopBound(void)
{
	return RegReadFldAlign(STA_SYNC1_06, NEWTOPBC);
}

/**
 * @brief wDrvDVIGetBottomBound( void )
 * Get Bottom  boundary value of SP1
 * @param  void
 * @retval Bottom boundary value
 * @example wDrvDVIGetBottomBound()
 */
UINT16 wDrvDVIGetBottomBound(void)
{
	return RegReadFldAlign(STA_SYNC1_06, NEWBOTTOMBC);
}

#endif // SUPPORT_DVI
#endif
