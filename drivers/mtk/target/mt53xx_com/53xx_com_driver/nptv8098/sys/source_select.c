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
#ifdef CC_SOURCE_AUTO_DETECT
#include "nptv_debug.h"
#include "hw_pdwnc.h"
#include "hw_async.h"
#include "hw_scslicer.h"
#include "api_notify.h"
#include "x_pdwnc.h"
#include "drv_hdtv.h"
#include "drv_async.h"

#endif

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
	if (((bPath == SV_VP_MAIN) && ((_bSrcMainNew == SV_VS_ATV1) || (_bSrcMainNew == SV_VS_ATD1))) ||
		((bPath == SV_VP_PIP) && ((_bSrcSubNew == SV_VS_ATV1) || (_bSrcSubNew == SV_VS_ATD1))))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL fgIsSrcScart(UINT8 bPath)
{
	if (((!bPath) && (VSS_MAJOR(_bSrcMainNew) == VSS_SCART)) ||
        ((bPath) && (VSS_MAJOR(_bSrcSubNew) == VSS_SCART)))
	{
		return TRUE;
	}
	return FALSE;
}


#ifdef CC_SOURCE_AUTO_DETECT

BOOL fgIsSrcDtv(UINT8 bPath)
{
	if (((bPath == SV_VP_MAIN) && (_bSrcMainNew == SV_VS_DT1)) ||
		((bPath == SV_VP_PIP) && (_bSrcSubNew == SV_VS_DT2)))
	{
		return TRUE;
	}
	return FALSE;	
}

BOOL fgIsSrcAV(UINT8 bPath)
{
    if (((bPath == SV_VP_MAIN) && (VSS_MAJOR(_bSrcMainNew) == VSS_CVBS)) ||
        ((bPath == SV_VP_PIP) && (VSS_MAJOR(_bSrcSubNew) == VSS_CVBS)))
	{
		return TRUE;
	}
	return FALSE;	
}

BOOL fgIsSrcSV(UINT8 bPath)
{
    if (((bPath == SV_VP_MAIN) && (VSS_MAJOR(_bSrcMainNew) == VSS_SV)) ||
        ((bPath == SV_VP_PIP) && (VSS_MAJOR(_bSrcSubNew) == VSS_SV)))
	{
		return TRUE;
	}
	return FALSE;	
}

BOOL fgIsSrcVGA(UINT8 bPath)
{
    if (((bPath == SV_VP_MAIN) && (VSS_MAJOR(_bSrcMainNew) == VSS_VGA)) ||
        ((bPath == SV_VP_PIP) && (VSS_MAJOR(_bSrcSubNew) == VSS_VGA)))
	{
		return TRUE;
	}
	return FALSE;	
}

BOOL fgIsSrcYPBPR(UINT8 bPath)
{
    if (((bPath == SV_VP_MAIN) && (VSS_MAJOR(_bSrcMainNew) == VSS_YPBPR)) ||
        ((bPath == SV_VP_PIP) && (VSS_MAJOR(_bSrcSubNew) == VSS_YPBPR)))
	{
		return TRUE;
	}
	return FALSE;	
}


#endif


UINT8 bGetICInputType(UINT8 bPath)
{
	UINT8 icin = 0;
	ASSERT((bPath==SV_VP_MAIN) || (bPath == SV_VP_PIP));

	if (bPath == (UINT8)SV_VP_MAIN)
	{
		icin = bGetMainInput();
	}
	else if (bPath == (UINT8)SV_VP_PIP)
	{
		icin = bGetSubInput();
	}

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
	UINT8 src=(bPath?_bSrcSubNew:_bSrcMainNew);	

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
	UINT8 src=(bPath?_bSrcSubNew:_bSrcMainNew);

	if (VSS_MAJOR(src)<VSS_RESERVE)
		return SrcInternalMap[VSS_MAJOR(src)].signaltype;
	else	return SV_ST_MAX;
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

/**
 * Return signal type of the bPath channel.
 *
 * @param bSrc Source Type
 * @return enum Signal_Type
 */
#if defined(CC_SUPPORT_TVE) ||defined(CC_SUPPORT_TVE_82xx)
UINT8 bGetSignalTypeBySrc(UINT8 bSrc) 
{
	if (VSS_MAJOR(bSrc)<VSS_RESERVE)
		return SrcInternalMap[VSS_MAJOR(bSrc)].signaltype;
	else	return SV_ST_MAX;
}
#endif



void vDrvAvMux(UINT32 dMUX) 
{
       //vRegWrite4BMsk(VFE_E4, dMUX,0x0fff0fff);
       //vRegWrite4BMsk(VFE_E3, dMUX,0x7f7f0000);
       vIO32Write4BMsk(REG_CVBS_CFG0, dMUX,0x7f7f0000);
}

UINT32 u1DrvGetAvMux(void)
{
    //return u4RegRead4B(VFE_E3);
	  return(IO32ReadFldAlign(REG_CVBS_CFG0, Fld(32, 0, AC_FULLDW)));

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
PRIVATE code UINT8 bAvMuxMskshift[8][2]=
{
   #if 0
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
  #endif

	{0,0},	/*none*/
	{26,18},	/*CVBS0*/  
	{19,27},	/*CVBS1*/ 
	{28,20},	/*CVBS2*/ 
	{29,21},	/*CVBS3*/  
	{24,16},	/*SY0*/  
	{17,25},	/*SC0*/ 
	{30,22},	/*SIPF*/  

};

#define AVMUX_CH0_MSK (0x354A0000)
#define AVMUX_CH1_MSK (0x4A350000) 

UINT32 dAvMuxMask(UINT8 bAvMux,UINT8 bCh)
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

    if (path)
    {
#ifdef SUPPORT_AV_COMP_AUTO
        if(_bMonSubNew == src)
        {
            icin=(enum IC_Input)_bSubMonICIn;
            mon_src=SV_VP_MAX;
            
        }
        else
#endif
        {
            icin=(enum IC_Input)_bSubICIn;
            mon_src=_bMonSubNew;
        }
    }
    else
    {
#ifdef SUPPORT_AV_COMP_AUTO
		if(_bMonMainNew == src)
		{
			icin=(enum IC_Input)_bMainMonICIn;
			mon_src=SV_VP_MAX;
				
		}
		else
#endif
		{
        icin=(enum IC_Input)_bMainICIn;
        mon_src=_bMonMainNew;
    	}
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
        vScartChangeMode();
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
void vSetSOutMux(UINT8 dec)
{
    vRegWriteFldAlign(OMUX_00,bOMuxMap(dec),OMUX_PIP_SEL);
    return;
}

#ifdef CC_SOURCE_AUTO_DETECT


typedef enum _E_DETECT_SOURCE
{
    Detect_Source_AVSV,
    Detect_Source_YPBPR,
    Detect_Source_VGA,
    Detect_Source_HDMI,
    Detect_Source_Max
} E_DETECT_SOURCE;

typedef enum _E_DETECT_SOURCE_INDEX
{
	SRC_DET_ATV1 = 0,
	SRC_DET_ATD1,
	SRC_DET_CVBS1, 
	SRC_DET_CVBS2,
	SRC_DET_CVBS3,
	SRC_DET_CVBS4, 
	SRC_DET_S1,   
	SRC_DET_S2, 
	SRC_DET_S3,
	SRC_DET_YPbPr1,
	SRC_DET_YPbPr2,
	SRC_DET_VGACOMP1,
	SRC_DET_VGA,
	SRC_DET_HDMI1,
	SRC_DET_HDMI2,
	SRC_DET_HDMI3,
	SRC_DET_HDMI4,
	SRC_DET_DVI1,
	SRC_DET_DVI2,
	SRC_DET_DVI3,
	SRC_DET_DTV1,
	SRC_DET_DTD1,
	SRC_DET_DTV2,
	SRC_DET_DTD2,
	SRC_DET_SCART1, 
	SRC_DET_SCART2, 
	SRC_DET_MAX
}E_DETECT_SOURCE_INDEX;

typedef enum _E_DETECT_SOURCE_STATUS
{
	SRC_DET_STABLE,
	SRC_DET_NOSIGNAL,
	SRC_DET_UNKNOWN,
	SRC_DET_UNCHG
} E_DETECT_SOURCE_STATUS;

 SOURCE_DETECT_STATUS_T _rSignalDetectStatus = {
				 	               SRC_DET_UNKNOWN,
								   SRC_DET_UNKNOWN,
								   SRC_DET_UNKNOWN,
								   SRC_DET_UNKNOWN,
								   SRC_DET_UNKNOWN,
								   SRC_DET_UNKNOWN,
								   SRC_DET_UNKNOWN};

extern source_detect_t src_det_tbl[];

BOOL abSkipFlag[ SRC_DET_MAX ];

// for source stable cnt
UINT8 au1AVSourceStable[4] = {0,0,0,0};
UINT8 au1SVSourceStable[3] = {0,0,0};
UINT8 u1VGASourceStable 	= 0;
UINT8 au1YPBPRSourceStable[3] = {0,0,0};
UINT8 au1HDMISourceStable[4] = {0,0,0,0};

#define SourceStableCnt 5

#define vDrvRstAVStableCnt(s)        (au1AVSourceStable[(s)] = 0)
#define vDrvIncAVStableCnt(s)        ((au1AVSourceStable[(s)] == 0xff)? au1AVSourceStable[(s)] = 0 : au1AVSourceStable[(s)]++ )
#define fgDrvAVStableCnt(s,Cnt)       ((au1AVSourceStable[(s)] == (Cnt)) ? TRUE : FALSE)

#define vDrvRstSVStableCnt(s)        (au1SVSourceStable[(s)] = 0)
#define vDrvIncSVStableCnt(s)        ((au1SVSourceStable[(s)] == 0xff)? au1SVSourceStable[(s)] = 0 : au1SVSourceStable[(s)]++ )
#define fgDrvSVStableCnt(s,Cnt)       ((au1SVSourceStable[(s)] == (Cnt)) ? TRUE : FALSE)

#define vDrvRstVGAStableCnt()	    (u1VGASourceStable = 0)
#define vDrvIncVGAStableCnt()		((u1VGASourceStable == 0xff)? u1VGASourceStable = 0 : u1VGASourceStable++ )
#define fgDrvVGAStableCnt(Cnt) 		((u1VGASourceStable == (Cnt)) ? TRUE : FALSE) 

#define vDrvRstYPBPRStableCnt(s)     (au1YPBPRSourceStable[(s)] = 0)
#define vDrvIncYPBPRStableCnt(s)     ((au1YPBPRSourceStable[(s)] == 0xff)? au1YPBPRSourceStable[(s)] = 0 : au1YPBPRSourceStable[(s)]++ )
#define fgDrvYPBPRStableCnt(s, Cnt)    ((au1YPBPRSourceStable[(s)] == (Cnt)) ? TRUE : FALSE)

#define vDrvRstHDMIStableCnt(s)     (au1HDMISourceStable[(s)] = 0)
#define vDrvIncHDMIStableCnt(s)     ((au1HDMISourceStable[(s)] == 0xff)? au1HDMISourceStable[(s)] = 0 : au1HDMISourceStable[(s)]++ )
#define fgDrvHDMIStableCnt(s, Cnt)    ((au1HDMISourceStable[(s)] == (Cnt)) ? TRUE : FALSE)

// for source no signal cnt
UINT8 au1AVNoSig[4] = {0,0,0,0};
UINT8 au1SVNoSig[3] = {0,0,0};
UINT8 u1VGANoSig 		= 0;
UINT8 au1YPBPRNoSig[3] = {0,0,0};
UINT8 au1HDMINoSig[4] = {0,0,0,0};

#define DetectNoSigCnt 5

#define vDrvRstAVNoSigCnt(s)        (au1AVNoSig[(s)] = 0)
#define vDrvIncAVNoSigCnt(s)        ((au1AVNoSig[(s)]== 0xff)? au1AVNoSig[(s)] = 0 : au1AVNoSig[(s)]++ )
#define fgDrvAVNoSigCnt(s,Cnt)       ((au1AVNoSig[(s)] == (Cnt)) ? TRUE : FALSE)

#define vDrvRstSVNoSigCnt(s)        (au1SVNoSig[(s)] = 0)
#define vDrvIncSVNoSigCnt(s)        ((au1SVNoSig[(s)] == 0xff)? au1SVNoSig[(s)] = 0 : au1SVNoSig[(s)]++ )
#define fgDrvSVNoSigCnt(s,Cnt)       ((au1SVNoSig[(s)] == (Cnt)) ? TRUE : FALSE)

#define vDrvRstVGANoSigCnt()	    (u1VGANoSig = 0)
#define vDrvIncVGANoSigCnt()		((u1VGANoSig == 0xff)? u1VGANoSig = 0 : u1VGANoSig++ )
#define fgDrvVGANoSigCnt(Cnt) 		((u1VGANoSig == (Cnt)) ? TRUE : FALSE) 

#define vDrvRstYPBPRNoSigCnt(s)     (au1YPBPRNoSig[(s)] = 0)
#define vDrvIncYPBPRNoSigCnt(s)     ((au1YPBPRNoSig[(s)] == 0xff)? au1YPBPRNoSig[(s)] = 0 : au1YPBPRNoSig[(s)]++ )
#define fgDrvYPBPRNoSigCnt(s, Cnt)    ((au1YPBPRNoSig[(s)] == (Cnt)) ? TRUE : FALSE)

#define vDrvRstHDMINoSigCnt(s)     (au1HDMINoSig[(s)] = 0)
#define vDrvIncHDMINoSigCnt(s)     ((au1HDMINoSig[(s)] == 0xff)? au1HDMINoSig[(s)] = 0 : au1HDMINoSig[(s)]++ )
#define fgDrvHDMINoSigCnt(s, Cnt)    ((au1HDMINoSig[(s)] == (Cnt)) ? TRUE : FALSE)

extern void vHdtvHwInit_SrcDetect (void);
extern UINT8 u1SrcDet_Int_Flag;
void vDrvDetectSourceInit(void)
{
    UINT8 u1TmpCnt;
	
    LOG(3, "*******========source detect initial\n");
    if ( !fgIsSrcAV(SV_VP_MAIN) && !fgIsSrcSV(SV_VP_MAIN))
    {
        LOG(3, "========source detect: cvbsvfe pwon\n");
        vDrvCvbsVfePWON();
        // for CHB detect, must set these bit as follows
        vRegWriteFldAlign(VSRC_01, 2, VSRC_MUX1);
       // vRegWriteFldAlign(VFE_E0, 1, RG_CLAMP_SINK_ENB);        //
       // vRegWriteFldAlign(VFE_E0, 1, RG_CLAMP_SOURCE_ENB);      //            
    }

    if (bGetICInputType(SV_VP_MAIN) == P_DT1)
    {
        vRegWriteFldAlign(VSRC_07, SV_OFF, ATD_ENABLE); //disable ATD path for TVD      
    }

    if ((!fgIsSrcVGA(SV_VP_MAIN)) && (!fgIsSrcYPBPR(SV_VP_MAIN)))
    {
        LOG(3, "===========source detect initail hdtv hw\n");
        vHdtvHwInit();
        vHdtvHwInit_SrcDetect();
        //set new slicer for soy
        //vRegWriteFldAlign(SyncSlice_Control_01, 0x96, VTLH_SEL);
        vIO32WriteFldAlign(PDWNC_SOGYCFG,0x69,FLD_PD_SOGY_VTLH);
    }

    _PdwncSetupVGA(WAK_VGA);

    for(u1TmpCnt=0;u1TmpCnt<SRC_DET_MAX;u1TmpCnt++)
    {
        abSkipFlag[u1TmpCnt] = FALSE;
    }
}

UINT8 u1DrvDetectSourceAV(UINT8 av_minor)
{
    UINT8 u1RetSrcSts = SRC_DET_UNKNOWN;
    UINT8 fgAVPres;

    fgAVPres = fgHwTvdVPresSVF();

    //LOG(3, "======AV source detect: fgAVPres=%d\n", fgAVPres);
    if (fgAVPres == TRUE)
    {
        if (fgDrvAVStableCnt(av_minor, SourceStableCnt+1))
        {
            vDrvRstAVNoSigCnt(av_minor);
            return SRC_DET_UNCHG;
        }
        if (fgDrvAVStableCnt(av_minor, SourceStableCnt))
        {
            LOG(3, "*********=======av %d source detect OK =%d \n",
                av_minor, fgAVPres);
            vDrvRstAVNoSigCnt(av_minor);
            u1RetSrcSts = SRC_DET_STABLE;
            _rSignalDetectStatus.ui2SrcStatus_av1 = SRC_DET_STABLE;
        }
        vDrvIncAVStableCnt(av_minor);
    }
    else
    {
        if (fgDrvAVNoSigCnt(av_minor, DetectNoSigCnt+1))
        {
            vDrvRstAVStableCnt(av_minor);
            return SRC_DET_UNCHG;
        }
        if (fgDrvAVNoSigCnt(av_minor, DetectNoSigCnt))
        {
            LOG(3, "********========av %d source detect no signal\n", av_minor);
            vDrvRstAVStableCnt(av_minor);
            u1RetSrcSts = SRC_DET_NOSIGNAL;
            _rSignalDetectStatus.ui2SrcStatus_av1 = SRC_DET_NOSIGNAL;
        }
        vDrvIncAVNoSigCnt(av_minor);
    }

    return u1RetSrcSts;
}

UINT8 u1DrvDetectSourceSV(UINT8 sv_minor)
{
    UINT8 u1RetSrcSts = SRC_DET_UNKNOWN;
    UINT8 fgSVPres;

    fgSVPres = fgHwTvdVPresSVF();

    //LOG(3,"======SV source detect fgSVPres= %d \n", fgSVPres);
    if (fgSVPres == TRUE)
    {
        if (fgDrvSVStableCnt(sv_minor, SourceStableCnt+1))
        {
            vDrvRstSVNoSigCnt(sv_minor);
            return SRC_DET_UNCHG;
        }
        if (fgDrvSVStableCnt(sv_minor, SourceStableCnt))
        {
            LOG(3, "*******=====sv %d source detect OK =%d \n", sv_minor, fgSVPres);
            vDrvRstSVNoSigCnt(sv_minor);
            u1RetSrcSts = SRC_DET_STABLE;
        }
        vDrvIncSVStableCnt(sv_minor);
    }
    else
    {
        if (fgDrvSVNoSigCnt(sv_minor, DetectNoSigCnt+1))
        {
            vDrvRstSVStableCnt(sv_minor);
            return SRC_DET_UNCHG;
        }
        if (fgDrvSVNoSigCnt(sv_minor, DetectNoSigCnt))
        {
            u1RetSrcSts = SRC_DET_NOSIGNAL;
            LOG(3, "*******=====sv %d source detect no signal\n", sv_minor);
            vDrvRstSVStableCnt(sv_minor);
        }
        vDrvIncSVNoSigCnt(sv_minor);
    }

    return u1RetSrcSts;
}

UINT8 u1DrvDetectSourceVGA(void)
{
    UINT8 u1RetSrcSts = SRC_DET_UNKNOWN;
    UINT8 u1SyncAct = 0, u1DetectSyncAct = 0;
    UINT8 u1PwdncSyncStable = 0;        // u1PwdncCSyncStable = 0;

    //reset pwdnc sync type stable bit
    vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 1,
                       FLD_STABLE_RESET);
    vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 0,
                       FLD_STABLE_RESET);

    // check sp0 hsync/vsync activity
    u1SyncAct = bASActiveChk();
    if ((u1SyncAct & 0x06) == 0x06)
    {
        //      LOG(3, "======VGA source detect: h/v syncact\n");
        u1DetectSyncAct = TRUE;
    }

#if 0
    //check pwdnc stable
    if (IO32ReadFldAlign
        (PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, FLD_SOG_WAK_DIS) == 0)
    {
        vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 1,
                           FLD_SOG_WAK_DIS);
    }
    if (IO32ReadFldAlign
        (PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, FLD_C_SYNC_WAK_DIS) == 0)
    {
        vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 1,
                           FLD_C_SYNC_WAK_DIS);
    }
    if (IO32ReadFldAlign
        (PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, FLD_S_SYNC_WAK_DIS) == 1)
    {
        vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 0,
                           FLD_S_SYNC_WAK_DIS);
    }
#endif

#if 0
    if (u1VGASourceStable == (SourceStableCnt - 1))
    {
        vUtDelay1ms(50);        //should be substitude with counter++
        u1Pwdnc_SEP_STABLE =
            IO32ReadFldAlign(PDWNC_VGA_WAKE_UP_STATUS_2_REGISTER,
                             FLD_SEP_STABLE);
        LOG(3, "======VGA source detect u1Pwdnc_SEP_STABLE= %d\n",
            u1Pwdnc_SEP_STABLE);
    }
#endif

    // check sync stable bit
    u1PwdncSyncStable =
        IO32ReadFldAlign(PDWNC_VGA_WAKE_UP_STATUS_0_REGISTER, FLD_V_STABLE);
    //LOG(3, "======VGA source detect u1PwdncSyncStable=%d\n ",u1PwdncSyncStable);

    if ((u1DetectSyncAct == TRUE)
        && (u1PwdncSyncStable == TRUE) /* && (u1Pwdnc_SEP_STABLE == TRUE) */ )
    {
        if (fgDrvVGAStableCnt(SourceStableCnt+1))
        {
            vDrvRstVGANoSigCnt();
            return SRC_DET_UNCHG; //stable;
        }
        if (fgDrvVGAStableCnt(SourceStableCnt))
        {
            vDrvRstVGANoSigCnt();
            LOG(3, "**********======VGA source detect OK = %d \n",
                u1DetectSyncAct && u1PwdncSyncStable);
            u1RetSrcSts = SRC_DET_STABLE;
            _rSignalDetectStatus.ui2SrcStatus_vga= SRC_DET_STABLE;
        }
        vDrvIncVGAStableCnt();
    }
    else
    {
        if (fgDrvVGANoSigCnt(DetectNoSigCnt+1))
        {
            vDrvRstVGAStableCnt();
            return SRC_DET_UNCHG; //nosignal
        }
        if (fgDrvVGANoSigCnt(DetectNoSigCnt))
        {
            vDrvRstVGAStableCnt();
            u1RetSrcSts = SRC_DET_NOSIGNAL;
            LOG(3, "**********======VGA source detect no signal \n");
            _rSignalDetectStatus.ui2SrcStatus_vga= SRC_DET_NOSIGNAL;
        }
        vDrvIncVGANoSigCnt();
    }

    return u1RetSrcSts;
}

UINT8 u1DrvDetectSourceYPBPR(UINT8 SOY0_1_G)
{
    UINT8 u1RetSrcSts = SRC_DET_UNKNOWN;
    UINT8 u1CSYNC_ACT = 0;
    UINT8 u1SOGV_Stable = 0;

    if(SOY0_1_G > 2)
    {
        return SRC_DET_UNKNOWN;
    }

    //reset pwdnc sync type stable bit
    vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 1,
                       FLD_STABLE_RESET);
    vIO32WriteFldAlign(PDWNC_VGA_WAKE_UP_CONTROL_1_REGISTER, 0,
                       FLD_STABLE_RESET);

#if 0
    if (u1YPBPRSourceStable == (SourceStableCnt - 1))
    {
        vUtDelay1ms(50);        // substitute with counter++
        u1Pdwnc_SOG_STABLE =
            IO32ReadFldAlign(PDWNC_VGA_WAKE_UP_STATUS_2_REGISTER,
                             FLD_SOG_STABLE);
        LOG(3, "======YPBPR source detect:u1Pdwnc_SOG_STABLE=%d\n",
            u1Pdwnc_SOG_STABLE);
    }
#endif

    u1CSYNC_ACT = bASActiveChk() & 0x01;
    u1SOGV_Stable = RegReadFldAlign(STA_SYNC0_02, AS_SOGV_STABLE);

    //LOG(3, "======YPBPR source detect:Stable=%d\n", u1CSYNC_ACT & u1SOGV_Stable );

    if ((u1CSYNC_ACT == TRUE) && (u1SOGV_Stable == TRUE))
    {
        if (fgDrvYPBPRStableCnt(SOY0_1_G, SourceStableCnt+1))
        {
            vDrvRstYPBPRNoSigCnt(SOY0_1_G);
            return SRC_DET_UNCHG; //stable
        }

        if (fgDrvYPBPRStableCnt(SOY0_1_G, SourceStableCnt))
        {
            LOG(3, "*******======YPBPR %d source detect OK = %d \n",
                SOY0_1_G, u1CSYNC_ACT && u1SOGV_Stable);

            u1RetSrcSts = SRC_DET_STABLE;
            if(1==SOY0_1_G)
            {
                LOG(3, "driver set ypbpr1 stable \n");
                _rSignalDetectStatus.ui2SrcStatus_ypbpr1= SRC_DET_STABLE;
            }
            vDrvRstYPBPRNoSigCnt(SOY0_1_G);
        }
        vDrvIncYPBPRStableCnt(SOY0_1_G);
    }
    else
    {
        if (fgDrvYPBPRNoSigCnt(SOY0_1_G, DetectNoSigCnt+1))
        {
            vDrvRstYPBPRStableCnt(SOY0_1_G);
            return SRC_DET_UNCHG; //nosignal
        }
        if (fgDrvYPBPRNoSigCnt(SOY0_1_G, DetectNoSigCnt))
        {
            u1RetSrcSts = SRC_DET_NOSIGNAL;
            LOG(3, "*******======YPBPR %d source detect no signal\n", SOY0_1_G);
            if(1==SOY0_1_G)
            {
                LOG(3, "driver set ypbpr1 nosignal \n");
                _rSignalDetectStatus.ui2SrcStatus_ypbpr1= SRC_DET_NOSIGNAL;
            }
            vDrvRstYPBPRStableCnt(SOY0_1_G);
        }
        vDrvIncYPBPRNoSigCnt(SOY0_1_G);
    }

    return u1RetSrcSts;
}

extern UINT8 bHDMIPort5VStatus(UINT8 port);
UINT8 u1DrvDetectSourceHDMI(UINT8 sv_minor)
{
	UINT8 u1RetSrcSts = SRC_DET_UNKNOWN;

	if (bHDMIPort5VStatus(sv_minor+1))  // 5V
	{
		LOG(11,"*******===== 5V \n");
		if (fgDrvHDMIStableCnt(sv_minor, SourceStableCnt+1))
		{
				LOG(11,"#1 \n");
				vDrvRstHDMINoSigCnt(sv_minor);
	            return SRC_DET_UNCHG;
	       }
		if (fgDrvHDMIStableCnt(sv_minor, SourceStableCnt))
		{
			LOG(11,"#2 \n");
			vDrvRstHDMINoSigCnt(sv_minor);
			if (_bHDMICurrSwitch != (sv_minor+1))
			{
				LOG(0, "*******===== HDMI%d source detect OK =====*******\n", sv_minor+1);
				u1RetSrcSts = SRC_DET_STABLE;
				switch(sv_minor+1)
				{
				   case 1:
				   	_rSignalDetectStatus.ui2SrcStatus_hdmi1= SRC_DET_STABLE;
				   	break;
				   case 2:
				   	_rSignalDetectStatus.ui2SrcStatus_hdmi2= SRC_DET_STABLE;
				   	break;
				   case 3:
				   	_rSignalDetectStatus.ui2SrcStatus_hdmi3= SRC_DET_STABLE;
				   	break;
				   case 4:
				   	_rSignalDetectStatus.ui2SrcStatus_hdmi4= SRC_DET_STABLE;
				   	break;
				   default:
				   	break;
				}
				
			}
		}
		vDrvIncHDMIStableCnt(sv_minor);
	}
	else
	{
		LOG(11,"*******===== no 5V \n");
		if (fgDrvHDMINoSigCnt(sv_minor, SourceStableCnt+1))
		{
			LOG(11,"#3 \n");
			vDrvRstHDMIStableCnt(sv_minor);
			return SRC_DET_UNCHG;
		}
		if (fgDrvHDMINoSigCnt(sv_minor, SourceStableCnt))
		{
			vDrvRstHDMIStableCnt(sv_minor);
			LOG(0, "*******===== HDMI%d source detect no signal =====*******\n", sv_minor+1);
			u1RetSrcSts = SRC_DET_NOSIGNAL;
			switch(sv_minor+1)
			{
			   case 1:
			   	_rSignalDetectStatus.ui2SrcStatus_hdmi1= SRC_DET_NOSIGNAL;
			   	break;
			   case 2:
			   	_rSignalDetectStatus.ui2SrcStatus_hdmi2= SRC_DET_NOSIGNAL;
			   	break;
			   case 3:
			   	_rSignalDetectStatus.ui2SrcStatus_hdmi3= SRC_DET_NOSIGNAL;
			   	break;
			   case 4:
			   	_rSignalDetectStatus.ui2SrcStatus_hdmi4= SRC_DET_NOSIGNAL;
			   	break;
			   default:
			   	break;
			}

		}
		vDrvIncHDMINoSigCnt(sv_minor);
	}

	return u1RetSrcSts;
}

//UINT8 u1DetectSrcState = 0;

static UINT8 u1CheckDetectSource(UINT8 curr, E_DETECT_SOURCE eDS)
{
    UINT8 min, max;

    switch(eDS)
    {
        case Detect_Source_AVSV:
            min = SRC_DET_CVBS1;
            max = SRC_DET_S3;
        break;
        case Detect_Source_YPBPR:
            min = SRC_DET_YPbPr1;
            max = SRC_DET_VGACOMP1;
        break;
        case Detect_Source_VGA:
            min = SRC_DET_VGA;
            max = SRC_DET_VGA;
        break;
	  case Detect_Source_HDMI:
            min = SRC_DET_HDMI1;
            max = SRC_DET_HDMI4;
        break;
        default:
            min = SRC_DET_MAX;
            max = SRC_DET_MAX; 
        break;
    }

    if(curr < min || curr > max)
    {
        // out of range, re-check
        curr = min;
    }

    do
    {
        if(src_det_tbl[curr].bDetectEnabled != FALSE)
        {
            if( _bSrcMainNew == src_det_tbl[curr].u1DetectSrc ||
                _bSrcSubNew == src_det_tbl[curr].u1DetectSrc ) 
            {
                abSkipFlag[curr] = TRUE;
            }
            else if(abSkipFlag[curr] == FALSE)
            {
                return curr;
            }
        } 
        
        curr++;
    }while(curr <= max);
    
    return 0xff;
}

void vDrvDetectSourceState(void)
{
    UINT8 u1DetSource;
    UINT8 u1TmpCnt;
    UINT8 u1SrcSts;
    UINT8 u1DetSrcIdxCnt;
    static UINT8 ic_port;
    static UINT8 av_mux;
    static UINT8 au1WaitSyncStableCnt[2] = {0, 0}; /* 2 for av/sv & YPbPr */
    static UINT8 au1DetSrcIdx[4] = {2, /* 2(CVBS1)~8(S3) */
                                    9, /* 9(YPBPR1)~11(VGACOMP) */
                                    12, /* VGA */ 
                                    13 /* 13(HDMI1)~16(HDMI4) */}; 

    au1DetSrcIdx[0] = u1CheckDetectSource(au1DetSrcIdx[0], Detect_Source_AVSV);
    au1DetSrcIdx[1] = u1CheckDetectSource(au1DetSrcIdx[1], Detect_Source_YPBPR);
    au1DetSrcIdx[2] = u1CheckDetectSource(au1DetSrcIdx[2], Detect_Source_VGA);
    au1DetSrcIdx[3] = u1CheckDetectSource(au1DetSrcIdx[3], Detect_Source_HDMI);

    if (u1SrcDet_Int_Flag)
    {
        u1SrcDet_Int_Flag = FALSE;
        au1WaitSyncStableCnt[0] = 0;
        au1WaitSyncStableCnt[1] = 0;
        vDrvDetectSourceInit();
        return ;
    }

    for(u1DetSrcIdxCnt=0;u1DetSrcIdxCnt<4;u1DetSrcIdxCnt++)
    {
        if(au1DetSrcIdx[u1DetSrcIdxCnt] >= SRC_DET_MAX)
        {
            /* out-of-range, skipped */
            continue;
        } 

        u1SrcSts = SRC_DET_UNKNOWN;
        u1DetSource = src_det_tbl[au1DetSrcIdx[u1DetSrcIdxCnt]].u1DetectSrc; 
    
        switch(VSS_MAJOR(u1DetSource))
        {
            case VSS_ATV:
            case VSS_ATD:
            break;
            case VSS_CVBS:
            case VSS_SV:
                if((VSS_MAJOR(_bSrcMainNew) == VSS_SV||VSS_MAJOR(_bSrcSubNew) == VSS_SV))
                {
                    abSkipFlag[au1DetSrcIdx[u1DetSrcIdxCnt]] = TRUE;
                    au1DetSrcIdx[u1DetSrcIdxCnt]++;
                    break;
                }

                if( au1WaitSyncStableCnt[u1DetSrcIdxCnt] == 0)
                {
                    av_mux = 0xff;
                    for (u1TmpCnt = 0; u1TmpCnt < portmap_table_num; u1TmpCnt++)
                    {
                        if ((portmap_table[u1TmpCnt].vs_src == u1DetSource)
                            && portmap_table[u1TmpCnt].av_mux)
                        {
                            av_mux = portmap_table[u1TmpCnt].av_mux;
                            break;
                        }
                    }

                    // keep CH0, switch CH1 (CHB) for detection
                    vDrvAvMux((u1DrvGetAvMux() & (AVMUX_CH0_MSK)) |
                               dAvMuxMask(AVMUX_CH0(av_mux), 1));

                }

                if( au1WaitSyncStableCnt[u1DetSrcIdxCnt] < 10)
                {
                    au1WaitSyncStableCnt[u1DetSrcIdxCnt]++;
                }
                else
                {
                    au1WaitSyncStableCnt[u1DetSrcIdxCnt]=0;
                    if(VSS_MAJOR(u1DetSource) == VSS_CVBS)
                    {
                        u1SrcSts = u1DrvDetectSourceAV(VSS_MINOR(u1DetSource));
                    }
                    else if(VSS_MAJOR(u1DetSource) == VSS_SV)
                    {
                        u1SrcSts = u1DrvDetectSourceSV(VSS_MINOR(u1DetSource));
                    }
                    au1DetSrcIdx[u1DetSrcIdxCnt]++;
                }
                
                break;
            case VSS_YPBPR:
            case VSS_VGACOMP:
                if((VSS_MAJOR(_bSrcMainNew) == VSS_YPBPR||VSS_MAJOR(_bSrcSubNew) == VSS_YPBPR)||
                   (VSS_MAJOR(_bSrcMainNew) == VSS_VGACOMP||VSS_MAJOR(_bSrcSubNew) == VSS_VGACOMP)||
                   (VSS_MAJOR(_bSrcMainNew) == VSS_VGA||VSS_MAJOR(_bSrcSubNew) == VSS_VGA) )
                {
                    abSkipFlag[au1DetSrcIdx[u1DetSrcIdxCnt]] = TRUE;
                    au1DetSrcIdx[u1DetSrcIdxCnt]++;
                    break;
                }

                if(au1WaitSyncStableCnt[u1DetSrcIdxCnt] == 0)
                {
                    ic_port = 0;
                    for (u1TmpCnt = 0; u1TmpCnt < portmap_table_num; u1TmpCnt++)
                    {
                        if ((portmap_table[u1TmpCnt].vs_src == u1DetSource)
                            && portmap_table[u1TmpCnt].ic_port)
                        {
                            ic_port = portmap_table[u1TmpCnt].ic_port;
                            break;
                        }
                    }
                    vDrvSetHDTVMux(ic_port);
                }
                if(au1WaitSyncStableCnt[u1DetSrcIdxCnt] < 20)
                {
                    au1WaitSyncStableCnt[u1DetSrcIdxCnt]++;
                }
                else
                {
                    au1WaitSyncStableCnt[u1DetSrcIdxCnt] = 0;
                    switch(ic_port)
                    {
                        case P_YP0: u1TmpCnt = 0; break;
                        case P_YP1: u1TmpCnt = 1; break;
                        case P_VGACOMP: u1TmpCnt = 2; break;
                        default: u1TmpCnt = 0xff; break;
                    }
                    u1SrcSts = u1DrvDetectSourceYPBPR(u1TmpCnt);
                    au1DetSrcIdx[u1DetSrcIdxCnt]++;
                }
                break;
            case VSS_VGA:
                // only detect H/V sync, no SOG support
                u1SrcSts = u1DrvDetectSourceVGA();
                break;
            case VSS_HDMI:
            case VSS_DVI:
		   u1SrcSts = u1DrvDetectSourceHDMI(VSS_MINOR(u1DetSource));
		   au1DetSrcIdx[u1DetSrcIdxCnt]++;
            break;
            case VSS_DTV:
            case VSS_DTD:
            break;
            case VSS_SCART:
            break;
            default:
            break;
        }

        if(u1SrcSts < SRC_DET_UNKNOWN)
        {
#ifndef CC_DRIVER_PROGRAM
            vApiNotifyAutoDetectSource((UINT8)au1DetSrcIdx[u1DetSrcIdxCnt]-1, (UINT8) u1SrcSts);
#endif
            LOG(3, "NotifyAutoDetSrc(srcidx=%d, sts=%d)\n", au1DetSrcIdx[u1DetSrcIdxCnt], u1SrcSts);
        }
    } // for u1DetSrcIdxCnt
}

VOID vDrvGetSrcDetectStatus(_TV_DEC_SOURCE_STATUS_T* pv_get_info)
{
    pv_get_info->ui2_av1_status=_rSignalDetectStatus.ui2SrcStatus_av1;
    pv_get_info->ui2_ypbpr1_status=_rSignalDetectStatus.ui2SrcStatus_ypbpr1;
    pv_get_info->ui2_hdmi1_status=_rSignalDetectStatus.ui2SrcStatus_hdmi1;
    pv_get_info->ui2_hdmi2_status=_rSignalDetectStatus.ui2SrcStatus_hdmi2;
    pv_get_info->ui2_hdmi3_status=_rSignalDetectStatus.ui2SrcStatus_hdmi3;
    pv_get_info->ui2_hdmi4_status=_rSignalDetectStatus.ui2SrcStatus_hdmi4;
    pv_get_info->ui2_vga_status=_rSignalDetectStatus.ui2SrcStatus_vga;
}
#endif



