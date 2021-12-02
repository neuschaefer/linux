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
 * $RCSfile: vga_um.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#define _VGA_UM_C_

#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "hw_vdoin.h"
#include "hw_async.h"
#include "drv_async.h"
#ifdef CC_COPLAT_MT82
#include "feature.h"
#endif
#include "vga_table.h"
#include "api_eep.h"
#include "eepdef.h"
#include "vga_auto.h"
#ifndef CC_COPLAT_MT82
#include "x_os.h"
#endif
#include "nptv_debug.h"

extern UINT8 _bHsyncWidth; //in drv_hdtv.c
//extern UINT8 _bVsyncWidth;

//maybe need to move to drv_vga.h
//extern UINT8  _bHsyncWidth_var; //27M resolution is not good enough
//extern UINT8   wSP0HLength_var;

extern UINT8   _bVgaUserMode_FIFO;

UINT8 vgaum_waitstable=0;
UINT8 vgaum_save_fifo;
VGAMODE 	 vgaum_save_tm;
VGA_USRMODE vgaum_save_eep;

UINT8 bVgaUsrTimingSearch(UINT8 eepinit);
//PRIVATE UINT16 CVTRB_Valign4(UINT16 vlen);
void vVgaUsrBroken(void);
void vVgaUsrStable(void);
/**
 * @brief bVgaUsrTimingSearch( void )
 * VGA User Mode Timing search according to bSP0Vclk & wSP0Hclk
 * @param  void
 * @retval User Mode Timing index.
 * @pre  Need to do bVgaStdTimingSearch, then do it.
 */

#if 0
PRIVATE UINT16 CVTRB_Valign4(UINT16 vlen)
{
#if 0

	if ((vlen&3)<2)
		return vlen&(~3);
	else
		return (vlen+4)&(~3);

#else
	if (vlen&3)
		return (vlen+4)&(~3);
	else
		return vlen;

#endif
}

PRIVATE UINT16 CVTRB_Halign16(UINT16 hlen)

{
	if ((hlen&0xf)<8)
		return hlen&(~0xf);
	else
		return (hlen+16)&(~0xf);
}


PRIVATE UINT16 CVT_VH_WL[][2]= //white list for correct H
    {
        /* get V , ret H */
        { 480, 640 },
        { 600, 800 },
        { 612, 1088 },
        { 720, 1280},
        { 768, 1280 },
        { 800, 1280 },
        { 864, 1152 },
        { 870, 1152 },
        { 960, 1280 },
        { 1024, 1280 },
        { 1080, 1920 }
    };

PRIVATE UINT16 CVT_Hguess(UINT16 vlen)
{
	UINT8 mod=0;
	UINT16 aspect4_3,aspect16_9,aspect16_10;

	if (BSP_GetIcVersion()>=IC_VER_AC)
	{
		switch (_bVsyncWidth)
		{

			case 4:/* 4:3*/
				return CVTRB_Halign16((vlen*4)/3);

			case 5:/* 16:9*/
				return CVTRB_Halign16((vlen*16)/9);

			case 6:/* 16:10*/
				return CVTRB_Halign16((vlen*16)/10);

			case 7:/* 5:4 or 15:9*/
				if (!(vlen%3))
				{
					return CVTRB_Halign16((vlen*5)/3);
				}
				else
				{
					return CVTRB_Halign16((vlen*5)/4);
				}

			default:
				/* custom,or 8,9(reserve) */
				break;
		}
	}

	/* white list test */
	for (mod=0;mod<(sizeof(CVT_VH_WL)/4);mod++)
	{
		if (CVT_VH_WL[mod][0]==vlen)
			return CVT_VH_WL[mod][1];
	}

	aspect4_3=(vlen*4)/3;

	aspect16_9=(vlen*16)/9;
	aspect16_10=(vlen*16)/10;

	if (!(vlen%3) && !(aspect4_3&0xf))
		mod|=1;

	if (!(vlen%9) && !(aspect16_9&0xf))
		mod|=2;

	if (!(vlen%10) && !(aspect16_10&0xf))
		mod|=4;

	switch (mod)
	{

		case 0:

		default:
			break;

		case 1:
			/* assume 4:3, 1.33x */
			return  aspect4_3;

		case 2:
			/* assume 16:9 1.77x*/
			return  aspect16_9;

		case 3:
			return aspect16_9;//aspect4_3

		case 4:
			/* assume 16:10 1.60 */
			return  aspect16_10;

		case 5:
			return aspect16_10;//aspect4_3

		case 6:

		case 7:
			return aspect16_10;//aspect16_9
	}

	return CVTRB_Halign16((vlen*4)/3);
}


PRIVATE UINT16 CVT_V_WL[][3]= //white list for correct V
    {
        /* from,to , ret */
        { 1432,1448,1440},
        { 1192,1208,1200},
        { 1072,1088,1080},
        { 1012,1030,1024},
        { 952,968,960},
        { 892,908,900},
        { 868,878,870},
        { 856,867,864}, //close to 870
        { 792,808,800},
        { 760,772,768},
        { 712,728,720},
        { 608,620,612},
        { 592,607,600}, //close to 612
        { 472,488,480},
        { 392,408,400},
        { 342,358,350}
    };


PRIVATE UINT16 CVT_Vguess(UINT16 vlen)
{
	UINT8 mod;

	/* white list test */

	for (mod=0;mod<(sizeof(CVT_V_WL)/6);mod++)
	{
		if (	(CVT_V_WL[mod][0]<=vlen)&&
		                                        (CVT_V_WL[mod][1]>=vlen))
			return CVT_V_WL[mod][2];
	}

	return vlen;
}

PRIVATE UINT16 CVTRB_HV_WL[][4]= //white list for not guessing H
    {
        /* from,to , v, h */
        { 1340,1480,1360,768 }, //a little problem with 1280x768
        { 1400,1800,1600,1024}
    };

PRIVATE UINT16 CVTRB_Hguess(UINT16 vlen,UINT16 hlen)
{
	UINT8 mod;
	UINT16 aspect[5];

	if (BSP_GetIcVersion()>=IC_VER_AC)
	{
		switch (_bVsyncWidth)
		{

			case 4:/* 4:3*/
				return CVTRB_Halign16((vlen*4)/3);

			case 5:/* 16:9*/
				return CVTRB_Halign16((vlen*16)/9);

			case 6:/* 16:10*/
				return CVTRB_Halign16((vlen*16)/10);

			case 7:/* 5:4 or 15:9*/
				{
					INT32 diff1,diff2,asp1,asp2;
					asp1=CVTRB_Halign16((vlen*5)/3);
					asp2=CVTRB_Halign16((vlen*5)/4);
					diff1=asp1-hlen;
					diff2=asp2-hlen;

					if (diff1<0)
						diff1=-diff1;

					if (diff2<0)
						diff2=-diff2;

					if (diff1<=diff2)
						return asp1;
					else
						return asp2;
				}

			default:
				/* custom,or 8,9(reserve) */
				break;
		}
	}

	/* white list test */
	for (mod=0;mod<(sizeof(CVTRB_HV_WL)/8);mod++)
	{
		if ((CVTRB_HV_WL[mod][3]==vlen)&&
		                                        (CVTRB_HV_WL[mod][0]<=hlen)&&
		                                        (CVTRB_HV_WL[mod][1]>=hlen))
			return CVTRB_HV_WL[mod][2];
	}

	/* special for  1280x768->1360x768 */
	// DMT's 1360x768 pol++ not CVT compatible
	//	if ((vlen==768)&&(hlen==1280))
	//		return 1360;

	/* end of white list  */
	mod=0;

	aspect[0]=(vlen*4)/3;

	aspect[1]=(vlen*16)/9;

	aspect[2]=(vlen*16)/10;

	aspect[3]=(vlen*5)/4;

	aspect[4]=(vlen*5)/3; //15:9

	if (!(vlen%3) && !(aspect[0]&0xf))
		mod|=1;

	if (!(vlen%9) && !(aspect[1]&0xf))
		mod|=2;

	if (!(vlen%10) && !(aspect[2]&0xf))
		mod|=4;

	if (!(vlen%4) && !(aspect[3]&0xf))
		mod|=8;

	if (!(vlen%3) && !(aspect[4]&0xf))
		mod|=16;

	if (mod)
	{
		UINT16  min_off,i,offset,candidate=0;//for LINT
		min_off=0xffff;

		for (i=0;i<5;i++)
		{
			if (mod&(1<<i))
			{
				offset=aspect[i]>hlen?aspect[i]-hlen:hlen-aspect[i];

				if (offset<min_off)
				{
					min_off=offset;
					candidate=i;
				}
			}
		}

		return aspect[candidate];
	}

	return CVTRB_Halign16((vlen*4)/3);

}
#endif

UINT8 bVgaUsrTimingSearch(UINT8 eepinit)
{
#if 0
	UINT8 hsyncwidth;
	UINT32  hsyncwidth_duty;
	UINT16 iclk,h_total,h_sta,h_active,h_bp,v_sta,v_active;
	UINT32  h_blank,v_blank;
	UINT8 mode_no,iscvtrb;

	hsyncwidth=_bHsyncWidth;
	hsyncwidth_duty= (hsyncwidth*1000)/wSP0HLength;

	if ((hsyncwidth_duty<70) &&(fgSP0Hpol)&&(!fgSP0Vpol))
	{
//		LOG(3,"User Timing using CVT-RB mode\n");
		iscvtrb=1;
		v_blank=((460*27*8)/wSP0HLength);

		if ((v_blank&7)>1)
		{
			v_blank=(v_blank/8)+1;
		}
		else
		{
			v_blank=(v_blank/8);
		}

		if (v_blank<3)
			v_blank=3; //for safe

		v_sta=v_blank-3;

		v_active=CVTRB_Valign4(wSP0Vtotal-v_blank); //need align to 4

		h_active=CVTRB_Halign16((32*16*wSP0HLength+wSP0HLength_var)/(hsyncwidth*16+_bHsyncWidth_var)-160);

		h_active=CVTRB_Hguess(v_active,h_active);

		if (_wForceXRes)
		{
			h_active=_wForceXRes;
		}

		h_blank=160;

		h_total=h_blank+h_active;
		h_bp=h_blank/2;
		h_sta=32+h_bp;
	}
	else
	{	//hsyncwidth_duty should be 80 exactly..
		UINT16 h_blank_percent;
		iscvtrb=0;
		//  v_blank>= 550us
		v_blank=((550*27*8)/wSP0HLength);

		if ((v_blank&7)>1)
		{
			v_blank=(v_blank/8)+1;
		}
		else
		{
			v_blank=(v_blank/8);
		}

		if (v_blank<3)
			v_blank=3; //for safe

		v_sta=v_blank-3;

		v_active=CVTRB_Valign4((wSP0Vtotal-v_blank)-3); //need align to 4

		if ((!fgSP0Hpol)&&fgSP0Vpol&&(hsyncwidth_duty>73)&&(hsyncwidth_duty<87))
		{
//			LOG(3,"User Timing using CVT mode\n");
		}
		else
		{
			UINT16 gotten;
			// using actve area to try
			gotten=(_wVgaVEnd-_wVgaVStart)+1;
			gotten=CVTRB_Valign4(gotten);
//			LOG(3,"User Timing using CVT-like mode\n");

			if ((gotten>(v_active-6))&&(gotten<(v_active+18))&&(gotten<(wSP0Vtotal-5)))
				v_active=gotten;

		}

		v_active=CVT_Vguess(v_active);

		h_active=CVT_Hguess(v_active);

		if (_wForceXRes)
		{
			h_active=_wForceXRes;
		}

		//h_blank_percent=300 -3*(wSP0HLength/27);//*10
		h_blank_percent=300 -(wSP0HLength/9);//*10

		h_blank=((h_active*1000)/(1000-h_blank_percent))-h_active;

		h_blank=CVTRB_Halign16(h_blank);

		h_total=h_blank+h_active;

		h_bp=h_blank/2;

		h_sta=(h_total*8)/100+h_bp;
	}


	if (_wForceYRes&&(_wForceYRes<(wSP0Vtotal-5)))
	{
		v_active=_wForceYRes;
	}

	//re-calculate V related parameter
	v_blank=wSP0Vtotal-v_active;

	v_sta=v_blank-3;


	iclk=((h_total*270)/(wSP0HLength));//iclk=(h_total*10)/(wSP0HLength/27);

	if (iclk<80)
	{
		iclk = 270;
	}

	if (iclk > wVGAADSpec)
	{
		return MODE_NOSUPPORT;
	}
	
	//wHFLow
	//wHFHeight
	//wVFLow
	//wVFHeight
	if ((wSP0Hclk > wHFHeight) || (wSP0Hclk < wHFLow))
	{
		return MODE_NOSUPPORT;
	}

	if ((bSP0Vclk > wVFHeight) || (bSP0Vclk < wVFLow))
	{
		return MODE_NOSUPPORT;
	}


	if (_wForceXRes||_wForceYRes) //Do filo roll back
	{
		UINT8 backup_fifo=_bVgaUserMode_FIFO;

		if (_bVgaUserMode_FIFO)
			_bVgaUserMode_FIFO--;
		else	_bVgaUserMode_FIFO=USERMODE_TIMING-1;

		if ((rVgaUsrEEP[_bVgaUserMode_FIFO].vpol==fgSP0Vpol)&&
		                                        (rVgaUsrEEP[_bVgaUserMode_FIFO].hpol==fgSP0Hpol)&&
		                                        (rVgaUsrEEP[_bVgaUserMode_FIFO].vlen<=(wSP0Vtotal+1))&&
		                                        (rVgaUsrEEP[_bVgaUserMode_FIFO].vlen>=(wSP0Vtotal-1))&&
		                                        (rVgaUsrEEP[_bVgaUserMode_FIFO].hlen<=(wSP0HLength+1))&&
		                                        (rVgaUsrEEP[_bVgaUserMode_FIFO].hlen>=(wSP0HLength-1)))
		{
			;//let _bVgaUserMode_FIFO-- ,  kill current entry
		}
		else
		{
			_bVgaUserMode_FIFO=backup_fifo;
		}
	}

	//Filling  User mode extension struct in RAM
	rVgaUsrExt[_bVgaUserMode_FIFO].h_res=h_active;


	/* protect for  ADCPLL  min/max */

	while (iclk<=270)
	{
		h_blank*=2;
		/* followin fill to table */
		iclk*=2;
		h_total*=2;
		h_active*=2;
		h_bp*=2;
		h_sta*=2	;
//		LOG(3,"oversample ");
	}

	while (iclk> 1620)
	{
		UINT32 i;
		/* trying ratio:  0.8  0.75  0.6  0.5 */
		const UINT8 tryratioMul[] ={4,3,3,1};
		const UINT8 tryratioDiv[] ={5,4,5,2};
		UINT32 est_clk;

		if ((iclk/2)>1620)
			i=3;
		else
		{
			for (i=0;i<(sizeof(tryratioMul)/sizeof(UINT16));i++)
			{
				if ((h_active%5)&&(tryratioDiv[i]==5))
				{
					//not trying 0.8 or 0.6
					continue;
				}

				est_clk=(iclk*tryratioMul[i])/tryratioDiv[i];

				if (est_clk<=1620)
					break;
			}
		}

		h_blank=(h_blank*tryratioMul[i])/tryratioDiv[i];

		/* followin fill to table */
		iclk=(iclk*tryratioMul[i])/tryratioDiv[i];
		h_total=(h_total*tryratioMul[i])/tryratioDiv[i];
		h_active=(h_active*tryratioMul[i])/tryratioDiv[i];
		h_bp=(h_bp*tryratioMul[i])/tryratioDiv[i];
		h_sta=(h_sta*tryratioMul[i])/tryratioDiv[i];
//		LOG(3,"downsample ");
	}

//	LOG(3,"%dx%d  [%dx%d] vb=%d hb=%d vsta=%d hsta=%d hbp=%d clk=%d\n",
//		h_active,v_active,h_total,wSP0Vtotal,v_blank,h_blank,v_sta,h_sta,h_bp,iclk);
#endif

#if 1
	UINT16 iclk,h_total,h_syncw,h_active,h_bp,v_sta,v_active;
	UINT8 i, candidate, Vdiff;
	//UINT16 Hdiff;
	UINT16 VTdiff;
	UINT16 diffsum, mindiffsum;
	UINT8 mode_no, iscvtrb;
	UINT8 hsyncwidth;
	UINT32  hsyncwidth_duty;

	mindiffsum = 0xFFFF;
	candidate = 0;
	for(i=VGA_SEARCH_START;i<bUserVgaTimingBegin;i++)
	{
		if((Get_VGAMODE_IVF(i)== 0) || (Get_VGAMODE_IHF(i) == 0))
		{
			continue ;
		}
		if(Get_VGAMODE_VgaDisabled(i))
		{
			continue;
		}
		//Hdiff = (wSP0Hclk>Get_VGAMODE_IHF(i))?(wSP0Hclk-Get_VGAMODE_IHF(i)):(Get_VGAMODE_IHF(i)-wSP0Hclk);
		Vdiff = (bSP0Vclk>Get_VGAMODE_IVF(i))?(bSP0Vclk-Get_VGAMODE_IVF(i)):(Get_VGAMODE_IVF(i)-bSP0Vclk);	
		VTdiff = (wSP0Vtotal>Get_VGAMODE_IVTOTAL(i))?(wSP0Vtotal-Get_VGAMODE_IVTOTAL(i)):(Get_VGAMODE_IVTOTAL(i)-wSP0Vtotal);	
		diffsum = Vdiff+VTdiff;
		
		if((mindiffsum > diffsum) && (Get_VGAMODE_IPV_LEN(i) < wSP0Vtotal) )
		{
			mindiffsum = diffsum;
			candidate = i;
		}
	}

#if 0	
	h_total = Get_VGAMODE_IHTOTAL(candidate);
	h_syncw= Get_VGAMODE_IPH_SYNCW(candidate);
	h_active = Get_VGAMODE_IPH_WID(candidate);
	h_bp = Get_VGAMODE_IPH_BP(candidate);
	v_sta = Get_VGAMODE_IPV_STA(candidate);
	v_active = Get_VGAMODE_IPV_LEN(candidate);
#else
    h_total = VGATIMING_TABLE[candidate].IHTOTAL;
    h_syncw= VGATIMING_TABLE[candidate].IPH_SYNCW;
    h_active = VGATIMING_TABLE[candidate].IPH_WID;
    h_bp = VGATIMING_TABLE[candidate].IPH_BP;
    v_sta = VGATIMING_TABLE[candidate].IPV_STA;
    v_active = VGATIMING_TABLE[candidate].IPV_LEN;
#endif
	iclk = (wSP0Hclk*h_total/1000);

	hsyncwidth=_bHsyncWidth;
	hsyncwidth_duty= (hsyncwidth*1000)/wSP0HLength;
	if ((hsyncwidth_duty<70) &&(fgSP0Hpol)&&(!fgSP0Vpol))
	{
		iscvtrb = 1;
	}
	else
	{
		iscvtrb = 0;
	}


	if ((iclk > wVGAADSpec) || (iclk ==0) || (candidate == 0))
	{
		return MODE_NOSUPPORT;
	}
	
	//wHFLow
	//wHFHeight
	//wVFLow
	//wVFHeight
	if ((wSP0Hclk > wHFHeight) || (wSP0Hclk < wHFLow))
	{
		return MODE_NOSUPPORT;
	}

	if ((bSP0Vclk > wVFHeight) || (bSP0Vclk < wVFLow))
	{
		return MODE_NOSUPPORT;
	}	
	
#endif
	// filling table
	mode_no=bUserVgaTimingBegin+_bVgaUserMode_FIFO;

	if (!eepinit)
	{
		//backup for unstable user mode
		vgaum_save_fifo=_bVgaUserMode_FIFO;
        #ifndef CC_UP8032_ATV		
		x_memcpy(&vgaum_save_tm,&VGATIMING_TABLE[mode_no],sizeof(VGAMODE));
		#endif

	}


    #ifndef CC_UP8032_ATV
	VGATIMING_TABLE[mode_no].IHF=wSP0Hclk;  //W.C Shih User Mode Modify
	VGATIMING_TABLE[mode_no].IVF=bSP0Vclk;  //W.C Shih User Mode Modify
	VGATIMING_TABLE[mode_no].IVTOTAL=wSP0Vtotal;  //W.C Shih User Mode Modify
	VGATIMING_TABLE[mode_no].ICLK=iclk;   //W.C Shih User Mode Modify
	VGATIMING_TABLE[mode_no].IHTOTAL=h_total;  //W.C Shih User Mode Modify
	VGATIMING_TABLE[mode_no].IPH_SYNCW=h_syncw;  //W.C Shih User Mode Modify
	VGATIMING_TABLE[mode_no].IPH_WID=h_active;  //W.C Shih User Mode Modify
	VGATIMING_TABLE[mode_no].IPH_BP=h_bp;  //W.C Shih User Mode Modify
	VGATIMING_TABLE[mode_no].IPV_STA=v_sta;  //W.C Shih User Mode Modify
	VGATIMING_TABLE[mode_no].IPV_LEN=v_active;  //W.C Shih User Mode Modify
    #endif

	if (!eepinit)
	{
		/* backup for unstable user mode */
		x_memcpy(&vgaum_save_eep,&rVgaUsrEEP[_bVgaUserMode_FIFO],sizeof(VGA_USRMODE));
		/* overwrite eep entry */
		rVgaUsrEEP[_bVgaUserMode_FIFO].vlen=wSP0Vtotal;
		rVgaUsrEEP[_bVgaUserMode_FIFO].vpol=fgSP0Vpol;
		rVgaUsrEEP[_bVgaUserMode_FIFO].hlen=wSP0HLength;
		rVgaUsrEEP[_bVgaUserMode_FIFO].hpol=fgSP0Hpol;
		rVgaUsrEEP[_bVgaUserMode_FIFO].hsync_w=_bHsyncWidth;
		//rVgaUsrEEP[_bVgaUserMode_FIFO].hsync_wvar=_bHsyncWidth_var;
		//rVgaUsrEEP[_bVgaUserMode_FIFO].hlen_var=wSP0HLength_var;
		rVgaUsrEEP[_bVgaUserMode_FIFO].isCVTRB=iscvtrb;
		rVgaUsrEEP[_bVgaUserMode_FIFO].id^=1;
		vgaum_waitstable=1;
	}
	else
	{
		vgaum_waitstable=0;
	}

	_bVgaUserMode_FIFO++;

	if (_bVgaUserMode_FIFO==USERMODE_TIMING)
	{
		_bVgaUserMode_FIFO=0;
	}

	return mode_no;

}

#ifdef CC_COPLAT_MT82
extern BOOL fgApiEepromWriteByte(UINT16 wAddr, UINT8 bData);
extern void vApiEepromDmaWrite(UINT16 wPrm, void *pSrc, UINT16 wLen);
#endif

void vVgaUsrStable(void)
{
	if (vgaum_waitstable)
	{
		/* overwrite eep entry */
		LINT_SUPPRESS_NEXT_EXPRESSION(792)
		if(vgaum_save_fifo < USERMODE_TIMING)
		{
			UNUSED(vApiEepromDmaWrite(EEP_VGA_USR_START+vgaum_save_fifo*sizeof(VGA_USRMODE),
		                          &rVgaUsrEEP[vgaum_save_fifo],sizeof(VGA_USRMODE)));
			UNUSED(fgApiEepromWriteByte(EEP_VGA_USR_START+USERMODE_TIMING*sizeof(VGA_USRMODE),_bVgaUserMode_FIFO));
		}
		else
		{
			LOG(3,"vgaum_save_fifo over range1\n");
		}
		vgaum_waitstable=0;
//		LOG(3,"vVgaUsrStable [%d]\n",vgaum_save_fifo);
	}
}

void vVgaUsrBroken(void)
{
	UINT8 mode_no;

	if (vgaum_waitstable)
	{
		if(vgaum_save_fifo < USERMODE_TIMING)
		{
			mode_no=bUserVgaTimingBegin+vgaum_save_fifo;
			#ifndef CC_UP8032_ATV
			x_memcpy(&VGATIMING_TABLE[mode_no],&vgaum_save_tm,sizeof(VGAMODE));//W.C Shih User Mode Modify
			#endif
			x_memcpy(&rVgaUsrEEP[vgaum_save_fifo],&vgaum_save_eep,sizeof(VGA_USRMODE));
			_bVgaUserMode_FIFO=vgaum_save_fifo;
		}
		else
		{
			LOG(3,"vgaum_save_fifo over range2\n");	
		}
		vgaum_waitstable=0;
//		LOG(3,"vVgaUsrBroken [%d]\n",vgaum_save_fifo);
	}
}
