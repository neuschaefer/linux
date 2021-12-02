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

/**
 * @file source_select.c
 *
 * source select function
 *
 * @author yj_chiang@mtk.com.tw
 */


#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "hw_vdoin.h"
#include "util.h"
#include "source_select.h"
#include "drv_tvd.h"
#include "drv_adcset.h"
#include "drv_vdoclk.h"
#include "drv_scart.h"
#include "vga_auto.h"
#include "source_table.h"
#include "video_def.h"
#include "api_eep.h"
#include "eepdef.h"
#include "drv_dvi.h"
#include"drv_hdmi.h"
#include "x_assert.h"

#include "x_lint.h"
// 2005/2/24 sc_yang add for YPbPr and S-Video Blank level reset
//extern UINT8   fgHdtvMoveBlankOut;

RVideoSrcSelect  _rVdoSrc[2];

UINT8 _VSS_Support_Mask[VSS_MAJORNR];
void VSS_Support_Initial(void)
{	
	UINT8 i,major,minor;
	for(i=0;i<VSS_MAJORNR;i++)
	{
		_VSS_Support_Mask[i]=0;
	}

	for(i=0;i<portmap_table_num; i++)
	{
		major=VSS_MAJOR(portmap_table[i].vs_src);
		minor=VSS_MINOR(portmap_table[i].vs_src);
		if(major<VSS_RESERVE)
		{
			_VSS_Support_Mask[major]|=(1<<minor);
		}
	}
}

UINT8   _bADCTVGain;
UINT8   _bAutoGain;

#define bGetMainInput() (bDrvGetMapIntMode(_bSrcMainNew,_bSrcSubNew) >> 8)
#define bGetSubInput() (bDrvGetMapIntMode(_bSrcMainNew,_bSrcSubNew) & 0xFF)

/**
 * Check if channel source is ATV.
 *
 * @param bPath SV_VP_MAIN / SV_VP_PIP
 */

BOOL fgIsSrcAtv(UINT8 bPath)
{
#if SUPPORT_POP
	if (((bPath == SV_VP_MAIN) && (_bSrcMainNew == SV_VS_TUNER1)) ||
		((bPath == SV_VP_PIP) && (_bSrcSubNew == SV_VS_TUNER1)))
#else
	if(_bSrcMainNew == SV_VS_TUNER1)
#endif
	{
		return TRUE;
	}
	return FALSE;
}

UINT8 bGetICInputType(UINT8 bPath)
{
	UINT8 icin = 0;
	ASSERT((bPath==SV_VP_MAIN) || (bPath == SV_VP_PIP));

	if (bPath == (UINT8)SV_VP_MAIN)
	{
		icin = bGetMainInput();
	}
#if SUPPORT_POP
	else if (bPath == (UINT8)SV_VP_PIP)
	{
		icin = bGetSubInput();
	}
#endif

	return icin;
}

typedef struct {
	UINT8	decoder;
	UINT8	signaltype;
} SrcInternalMap_t;


PRIVATE	code SrcInternalMap_t SrcInternalMap[]=
{
/*VSS_ATV	*/	{SV_VD_TVD3D, SV_ST_TV},
/*VSS_ATD	*/	{SV_VD_TVD3D, SV_ST_TV},
/*VSS_CVBS	*/	{SV_VD_TVD3D, SV_ST_AV},
/*VSS_SV	*/	{SV_VD_TVD3D, SV_ST_SV},
/*VSS_YPBPR	*/	{SV_VD_YPBPR, SV_ST_YP},
/*VSS_VGACOMP	*/	{SV_VD_YPBPR, SV_ST_YP},
/*VSS_VGA	*/	{SV_VD_VGA,   SV_ST_VGA},
/*VSS_HDMI	*/	{SV_VD_DVI,   SV_ST_DVI},
/*VSS_DVI	*/	{SV_VD_DVI,   SV_ST_DVI},
/*VSS_DTV	*/	{SV_VD_MPEGHD,SV_ST_MPEG},
/*VSS_DTD	*/	{SV_VD_MPEGHD,SV_ST_MPEG},
/*VSS_SCART	*/	{SV_VD_TVD3D, SV_ST_AV}
};



/**
 * Get the video decoder of the specified path.
 *
 * @param bPath SV_VP_MAIN / SV_VP_PIP
 */

UINT8 bGetInternalDec(UINT8 bPath) 
{

	UINT8 dec;
	#if SUPPORT_POP
	UINT8 src=(bPath?_bSrcSubNew:_bSrcMainNew);	
	#else
	UINT8 src = _bSrcMainNew;
	#endif

	if (VSS_MAJOR(src)<VSS_RESERVE)
	{
		dec=SrcInternalMap[VSS_MAJOR(src)].decoder;
		if((dec==SV_VD_MPEGHD) && (VSS_MINOR(src)==1))
			dec=SV_VD_MPEGSD; //DTV2 or DTD2
		return dec;
	}
	else	return SV_VD_NA;
}

/**
 * Return signal type of the bPath channel.
 *
 * @param bPath SV_VP_MAIN or SV_VP_PIP
 * @return enum Signal_Type
 */
UINT8 bGetSignalType(UINT8 bPath) 
{
    #if SUPPORT_POP
    UINT8 src=(bPath?_bSrcSubNew:_bSrcMainNew);
    #else
    UINT8 src = _bSrcMainNew;
    #endif

    #if defined(CC_MT8223)
    if (bPath == SV_VP_PIP)
    {
        return SV_ST_MAX;
    }
    #endif

    if (VSS_MAJOR(src)<VSS_RESERVE)
        return SrcInternalMap[VSS_MAJOR(src)].signaltype;
    else
        return SV_ST_MAX;
}

/**
 * Return signal type of the bPath channel.
 *
 * @param bSrc Source Type
 * @return enum Decoder Type
 */


UINT8 bGetDecodeType(UINT8 bSrc) 
{
	UINT8 dec;

	if (VSS_MAJOR(bSrc)<VSS_RESERVE)
	{
		dec=SrcInternalMap[VSS_MAJOR(bSrc)].decoder;
		if((dec==SV_VD_MPEGHD) && (VSS_MINOR(bSrc)==1))
			dec=SV_VD_MPEGSD; //DTV2 or DTD2
		return dec;
	}
	else	return SV_VD_NA;
}


#if defined(CC_SUPPORT_TVE) ||defined(CC_SUPPORT_TVE_82xx)
/**
 * Return signal type of the bPath channel.
 *
 * @param bSrc Source Type
 * @return enum Signal_Type
 */

UINT8 bGetSignalTypeBySrc(UINT8 bSrc) 
{
	if (VSS_MAJOR(bSrc)<VSS_RESERVE)
		return SrcInternalMap[VSS_MAJOR(bSrc)].signaltype;
	else	return SV_ST_MAX;
}
#endif



void vDrvAvMux(UINT32 dMUX) 
{
       vRegWrite4BMsk(VFE_E4, dMUX,0x0fff0fff);
}


UINT8 u1GetMonICIn(UINT8 bSrc)
{
	UINT8 i;
	for(i=0;i<portmap_table_num; i++)
	{
		if(portmap_table[i].vs_src==bSrc)
			return portmap_table[i].ic_port;
	}
	return (UINT8)P_FA;
}
//================================================================
/**
 * Mux setting for CVBS & SVDO input
 *
 * @param bVDOINPUT {P_C0 P_C1 P_C2 P_C3  P_SV0  P_SV1}
 * @return void
 */
PRIVATE code UINT8 bAvMuxMskshift[12][2]=
{
	{0,0},	/*none*/
	{20,4},	/*CVBS0*/  
	{21,5},	/*CVBS1*/ 
	{6,22},	/*CVBS2*/ 
	{7,23},	/*CVBS3*/  
	{11,27},  /*CVBS4 Not Required*/ 
	{16,0},	/*SY0*/  
	{1,17},	/*SC0*/ 
	{18,2},	/*SY1*/  
	{3,19},	/*SC1*/
	{24,8},	/*SY2*/  
	{9,25},	/*SC2*/ 

};

PRIVATE UINT32 dAvMuxMask(UINT8 bAvMux,UINT8 bCh)
{
	if(bAvMux && (bAvMux<=AVMUX_MAX))
	{
		return 1<<bAvMuxMskshift[bAvMux][bCh];
	}
	else
	{
		return 0;
	}
}

void vDrvSetTVMux(UINT8 bSrc,UINT8 bMonSrc) 
{	
	UINT32 u4Tmp=0;
	UINT16 mux;
	UINT8 i;
	for(i=0;i<portmap_table_num; i++)
	{		
		if((portmap_table[i].vs_src==bSrc)&&portmap_table[i].av_mux)
		{
			mux=portmap_table[i].av_mux;			
			if(AVMUX_CH0(mux)<=AVMUX_MAX)
			{
				u4Tmp=dAvMuxMask(AVMUX_CH0(mux),0);
				if(VSS_MAJOR(bSrc) !=VSS_CVBS)
				{	/* no need to search bMonSrc*/
					u4Tmp|=dAvMuxMask(AVMUX_CH1(mux),1);
					break;
				}
			}
		}

		if((bMonSrc!= SV_VS_MAX)&&(portmap_table[i].vs_src==bMonSrc)&&portmap_table[i].av_mux)
		{
			mux=portmap_table[i].av_mux;			
			if(AVMUX_CH0(mux)<=AVMUX_MAX)
			{
				u4Tmp|=dAvMuxMask(AVMUX_CH0(mux),1);
			}
			bMonSrc=SV_VS_MAX; /* found, skip search in loop */
		}
	}

	vDrvAvMux(u4Tmp);
	
}


//================================================================
void initAVSV(UINT8 vsrc,UINT8 mon_src)
{
    vDrvSetTVMux(vsrc,mon_src);
    vDrvSetTVADC(vsrc); /* mode change: NTSC, PAL, SECAM - who do it - TODO */
}

void initYPbPrVGA(UINT8 ic)
{
    vDrvAllHDADCPow(SV_TRUE);		//for power saving , power on HD ADC , by huahua 20070427
    //setSP0(); /* TODO */
    vDrvSetHDTVADC(ic); /* mode detect done - TODO */
    vDrvSetHDTVMux(ic);
    /* HDTV or VGA - TODO */
}

#if SUPPORT_SCART
void initSCARTRGB(UINT8 bVSrc, UINT8 ic)
{
       UINT8 i, ic_port = 0 ;
	initAVSV(bVSrc, SV_VS_MAX);
        
        vDrvAllHDADCPow(SV_TRUE);		//for power saving , power on HD ADC , by huahua 20070427
    
        vDrvSetHDTVADC(ic); /* mode detect done - TODO */
        for(i=0;i<portmap_table_num; i++)
	{		
		if((portmap_table[i].vs_src==bVSrc)&&portmap_table[i].hdtv_ic_port)
		{
			ic_port=portmap_table[i].hdtv_ic_port;			
                    break;
		}
	}        
       vDrvSetHDTVMux(ic_port);   
}
#endif

void vDrvSetInternalMux(UINT8 path,UINT8 src)
{
    enum IC_Input icin;
    UINT8 mon_src;
#if SUPPORT_POP
    if (path)
    {
        icin=(enum IC_Input)_bSubICIn;
        mon_src=_bMonSubNew;
    }
    else
#endif
    {
        icin=(enum IC_Input)_bMainICIn;
        mon_src=_bMonMainNew;
    }

    if (/*(icin >= P_C0) && */(icin <= P_SV2))
    {
        #if SUPPORT_SCART
        vSCARTDisable(); 
        #endif
        vDrvTVADCDefaultSetting();//5381
        initAVSV(src,mon_src);
    }
    else if ((icin <= P_VGA) || (icin == P_VGACOMP))
    {
        #if SUPPORT_SCART
        vSCARTDisable(); 
        #endif
        initYPbPrVGA((UINT8)icin);
    }
    else if (icin == P_DVI)
    {
    }
    #if SUPPORT_SCART
    else if (icin == P_FB0 || icin == P_FB1)
    {
        initSCARTRGB(src, (UINT8)icin);    
        vSCARTInit(src);  
        vScartChangeMode((UINT8)icin);
    }
    #endif
    else if (icin == P_MA)
    {
    }
    else
    {
        assert(SV_FALSE);
    }
}


void vMuxCleanup(void)
{
    UINT8 i;
    BOOL bAVMuxInUse,bSoGInUse,bSoY0InUse,bSoY1InUse;

    bAVMuxInUse=0;
    bSoGInUse=0;
    bSoY0InUse=0;
    bSoY1InUse=0;

    for (i=0;i<2;i++)
    {
        switch (VSS_MAJOR(_rVdoSrc[i].SrcNew))
        {
        case VSS_ATD:
        case VSS_ATV:
        case VSS_CVBS:
        case VSS_SV:
        case VSS_SCART:
            bAVMuxInUse=1;
            break;
        case VSS_VGA:
        case VSS_VGACOMP:
            bSoGInUse=1;
            break;
        case VSS_YPBPR:
            switch (bGetICInputType(i))
            {
            case P_YP0:
                bSoY0InUse=1;
                break;
            case P_YP1:
                bSoY1InUse=1;
                break;
            default:
                break;
            }
        default:
            break;
        }
    }

    if (!bAVMuxInUse)
        vDrvAvMux(0);
    if (!bSoGInUse)
        vDrvSOGEN(0);
    if (!bSoY0InUse)
        vDrvSOY0EN(0);
    if (!bSoY1InUse)
        vDrvSOY1EN(0);


}


PRIVATE UINT8 OMuxMapTbl[SV_VD_MAX]=
    {	1,	/* NA, tvd2d */
      0,	/* tvd3d */
      2,	/* hdtv */
      2,	/* hdtv */
      6,	/* dvi */
      8,	/* mpeghd */
      9 	/* mpegsd */
    };
#define bOMuxMap(dec) 	(((dec)<=SV_VD_MPEGSD)?OMuxMapTbl[dec]:1)

/**
 * Select video channel to main output path.
 *
 * @param dec Specify the video channel.
 */
 void vSetMOutMux(UINT8 dec)
{
    vRegWriteFldAlign(OMUX_00,bOMuxMap(dec),OMUX_MAIN_SEL);
    return;
}

/**
 * Select video channel to sub output path.
 *
 * @param dec Specify the video channel.
 */

#if SUPPORT_POP 
void vSetSOutMux(UINT8 dec)
{
    vRegWriteFldAlign(OMUX_00,bOMuxMap(dec),OMUX_PIP_SEL);
    return;
}
#endif
