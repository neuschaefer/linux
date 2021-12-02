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

/**************************************************************************
 * Filename:
 * ---------
 *   $Workfile: source_table.h $
 *
 * Description:
 *    deal with source select
 * ------------
 *
 *
 * Author:  Rock Hsieh
 * -------
 *
 * Last changed:
 * -------------
 * $Author: dtvbm11 $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 ****************************************************************************/
#ifndef _SOURCE_TABLE_H_
#define _SOURCE_TABLE_H_

#include "typedef.h"
#include "debug.h"


/**
 *  source_select  structures & defines
 */

typedef struct
{
	UINT8 SrcNew; // to replace _bSrcMainNew _bSrcSubNew
	UINT8 SrcOld;// to replace _bSrcMainOld _bSrcSubOld
	UINT8 ICIn; // to replace _bMainICIn _bSubICIn

	/* For Combi Component, we should keep monitored information */
	UINT8 MonSrcNew; // to replace _bMonMainNew  _bMonSubNew
	UINT8 MonSrcOld; // to replace _bMonMainNew  _bMonSubNew
	UINT8 MonICIn; // to replace _bMonMainNew  _bMonSubNew
} RVideoSrcSelect;


extern RVideoSrcSelect  _rVdoSrc[2];

#ifdef CC_SOURCE_AUTO_DETECT
typedef struct _source_detect_t
{
    UINT8  u1DetectSrc;
    BOOL   bDetectEnabled;
} source_detect_t;
#endif


/* interface for getting input video source information */
#define bGetVdoSrcNew(bPath) 		_rVdoSrc[bPath].SrcNew
#define bGetVdoSrcOld(bPath) 			_rVdoSrc[bPath].SrcOld
#define bGetVdoICIn(bPath) 			_rVdoSrc[bPath].ICIn

#define bGetVdoMonSrcNew(bPath) 		_rVdoSrc[bPath].MonSrcNew
#define bGetVdoMonSrcOld(bPath) 			_rVdoSrc[bPath].MonSrcOld
#define bGetVdoMonICIn(bPath) 			_rVdoSrc[bPath].MonICIn


/* initialize _bSrcMainNew to SV_VS_MAX */
#define _bSrcMainNew  _rVdoSrc[0].SrcNew	/* current main source */
/* initialize _bSrcSubNew to SV_VS_MAX */
#define _bSrcSubNew  _rVdoSrc[1].SrcNew	/* current sub source */

/* initialize _bSrcMainOld to SV_VS_MAX */
#define _bSrcMainOld _rVdoSrc[0].SrcOld
/* initialize _bSrcSubOld to SV_VS_MAX */
#define _bSrcSubOld _rVdoSrc[1].SrcOld

#define _bMainICIn _rVdoSrc[0].ICIn
#define _bSubICIn _rVdoSrc[1].ICIn

/* For Combi Component, we should keep monitored information */
#define _bMonMainNew _rVdoSrc[0].MonSrcNew
#define _bMonSubNew _rVdoSrc[1].MonSrcNew
#define _bMonMainOld _rVdoSrc[0].MonSrcOld
#define _bMonSubOld _rVdoSrc[1].MonSrcOld
#define _bMainMonICIn _rVdoSrc[0].MonICIn
#define _bSubMonICIn _rVdoSrc[1].MonICIn


////////////////////////////////////////////////////////////////////////////////
// UI Related define
////////////////////////////////////////////////////////////////////////////////

/************************************************************
Define if related device is included as a input source
 1 - Indicates related device is    included as a input source
 0 - Indicates related device isn't included as a input source
************************************************************/


#define SUPPORT_TV1_INPUT             1
#define SUPPORT_TV2_INPUT             0
#define SUPPORT_AV1_INPUT             1
#define SUPPORT_AV2_INPUT             1
#define SUPPORT_AV3_INPUT             1
#define SUPPORT_SV1_INPUT             1	/* S-Video */
#define SUPPORT_SV2_INPUT             1
#define SUPPORT_SV3_INPUT             0
#define SUPPORT_YPbPr1_INPUT          1	/* YPbPr 1 */
#define SUPPORT_YPbPr2_INPUT          1	/* YPbPr 2 */
#define SUPPORT_HDMI_INPUT            1
#define SUPPORT_DVI_INPUT             0
#define SUPPORT_VGA_INPUT             1
#define SUPPORT_SCART1_INPUT          1
#define SUPPORT_SCART2_INPUT          1
#define SUPPORT_SCART3_INPUT          1
#define SUPPORT_DTV_INPUT             0
#define SUPPORT_EXTTVD_AV1_INPUT			0
#define SUPPORT_EXTTVD_SV1_INPUT			0
#define SUPPORT_EXTTVD_YPbPr1_INPUT		0

// Need to modify souce_table.c  to support Scart decoder with other PCB versions
#define SUPPORT_SCART_DECODER	0
/* SCART S-video first */
#define SUPPORT_SCART_SFIRST 0

#define SUPPORT_S_DETECT 1

/*
 *  New implementation of SOURCE TABLE definition,20061220
 *  try to standardize naming & id  for all input source (become super set), easy to customize
 *  and not to conflict  between source code branches.
 *  currently using 5 bits major & 3 bit minor(like unix devices).
 */
 
/* HWDECs, following's VSS_Major_Rules we have 8 bits for hwdec,
    now only define 4  hwdec */
#define VSS_TVDDEC1		(1)
#define VSS_HDTVDEC1		(1<<1)
#define VSS_HDMIDEC1		(1<<2)
#define VSS_DTVDEC1		(1<<3)

/* Video source super set */
enum VSS_MAJOR_TYPE
{
 	VSS_ATV	=0,
	VSS_ATD	=1, /* ATV internal demod*/
	VSS_CVBS	=2,
	VSS_SV		=3,
	VSS_YPBPR	=4,
	VSS_VGACOMP	=5,	
	VSS_VGA	=6,
	VSS_HDMI	=7,
	VSS_DVI		=8,
	VSS_DTV	=9,
	VSS_DTD	=10, /* DTV internal demod*/
	VSS_SCART	=11,
	/* VSS_CCIR,VSS_MPEGSD will be added when need */
	/* insert new major type here, don't change above for compatibility */
	VSS_RESERVE	=12, /* end for major rule,special rule */
	VSS_MAJORNR	=13,
	VSS_MAX	=31
};

#define  VSS_TUNER VSS_ATV

#define	VSS_MKVSRC(major,minor)		((((UINT8)major)<<3)|(minor))
#define	VSS_MAJOR(dev)			(enum VSS_MAJOR_TYPE)(((dev)>>3)&0x1f)
#define	VSS_MINOR(dev)			((dev)&7)


/* RULE STRUCT, see source_table.c, a table to make sure VS is valid or invalid*/
struct VSS_Major_Rules
{
	UINT8	hwdec_mask;
};

extern struct VSS_Major_Rules code VSS_major_rule[];

extern UINT8 _VSS_Support_Mask[VSS_MAJORNR];
extern void VSS_Support_Initial(void);

#define VSS_Support_Major(x)	(_VSS_Support_Mask[x]?1:0)
#define VSS_Support_Input(x)	((_VSS_Support_Mask[x]&(1<<VSS_MINOR(x)))?1:0)


/* input source list */
/* VSS_ATV:	0~7 */
#define SV_VS_ATV1	VSS_MKVSRC(VSS_ATV,0)
/* VSS_ATVD:	8~15 */
#define SV_VS_ATD1	VSS_MKVSRC(VSS_ATD,0)
/* VSS_CVBS:	16~23 */
#define SV_VS_CVBS1	VSS_MKVSRC(VSS_CVBS,0)
#define SV_VS_CVBS2	VSS_MKVSRC(VSS_CVBS,1)
#define SV_VS_CVBS3	VSS_MKVSRC(VSS_CVBS,2)
#define SV_VS_CVBS4	VSS_MKVSRC(VSS_CVBS,3)
#define SV_VS_CVBS5	VSS_MKVSRC(VSS_CVBS,4)
#define SV_VS_CVBS6	VSS_MKVSRC(VSS_CVBS,5)
#define SV_VS_CVBS7	VSS_MKVSRC(VSS_CVBS,6)
#define SV_VS_CVBS8	VSS_MKVSRC(VSS_CVBS,7)
/* VSS_SV:	24~31 */
#define SV_VS_S1	VSS_MKVSRC(VSS_SV,0)
#define SV_VS_S2	VSS_MKVSRC(VSS_SV,1)
#define SV_VS_S3	VSS_MKVSRC(VSS_SV,2)
#define SV_VS_S4	VSS_MKVSRC(VSS_SV,3)
/* VSS_YPBPR:	32~39 */
#define SV_VS_YPbPr1	VSS_MKVSRC(VSS_YPBPR,0)
#define SV_VS_YPbPr2	VSS_MKVSRC(VSS_YPBPR,1)
#define SV_VS_YPbPr3	VSS_MKVSRC(VSS_YPBPR,2)
#define SV_VS_YPbPr4	VSS_MKVSRC(VSS_YPBPR,3)
/* VSS_VGACOMP 40~47 */
#define SV_VS_VGACOMP1 VSS_MKVSRC(VSS_VGACOMP,0)
/* VSS_VGA:	48~55 */
#define SV_VS_VGA1	VSS_MKVSRC(VSS_VGA,0)
/* VSS_HDMI:	56~63 */
#define SV_VS_HDMI1	VSS_MKVSRC(VSS_HDMI,0)
#define SV_VS_HDMI2	VSS_MKVSRC(VSS_HDMI,1)
#define SV_VS_HDMI3	VSS_MKVSRC(VSS_HDMI,2)
#define SV_VS_HDMI4	VSS_MKVSRC(VSS_HDMI,3)
#define SV_VS_HDMI5	VSS_MKVSRC(VSS_HDMI,4)
#define SV_VS_HDMI6	VSS_MKVSRC(VSS_HDMI,5)
#define SV_VS_HDMI7	VSS_MKVSRC(VSS_HDMI,6)
#define SV_VS_HDMI8	VSS_MKVSRC(VSS_HDMI,7)
/* VSS_DVI:	64~71 */
#define SV_VS_DVI1	VSS_MKVSRC(VSS_DVI,0)
#define SV_VS_DVI2	VSS_MKVSRC(VSS_DVI,1)
#define SV_VS_DVI3	VSS_MKVSRC(VSS_DVI,2)
#define SV_VS_DVI4	VSS_MKVSRC(VSS_DVI,3)
#define SV_VS_DVI5	VSS_MKVSRC(VSS_DVI,4)
#define SV_VS_DVI6	VSS_MKVSRC(VSS_DVI,5)
#define SV_VS_DVI7	VSS_MKVSRC(VSS_DVI,6)
#define SV_VS_DVI8	VSS_MKVSRC(VSS_DVI,7)
/* VSS_DTV:	72~79 */
#define SV_VS_DTV1	VSS_MKVSRC(VSS_DTV,0)	/*HD*/
#define SV_VS_DTV2	VSS_MKVSRC(VSS_DTV,1)	/*SD*/
/* VSS_DTD:	80~87 */
#define SV_VS_DTD1	VSS_MKVSRC(VSS_DTD,0)	/*HD*/
#define SV_VS_DTD2	VSS_MKVSRC(VSS_DTD,1)	/*SD*/
/* VSS_SCART:	88~95 */
#define SV_VS_SCART1	VSS_MKVSRC(VSS_SCART,0)
#define SV_VS_SCART2	VSS_MKVSRC(VSS_SCART,1)
#define SV_VS_SCART3	VSS_MKVSRC(VSS_SCART,2)
#define SV_VS_SCART4	VSS_MKVSRC(VSS_SCART,3)

/* VSS_RESERVE:	96~ */
#define SV_VS_NO_CHANGE VSS_MKVSRC(VSS_MAX,6)	/*254*/
#define SV_VS_MAX	VSS_MKVSRC(VSS_MAX,7)	/*255*/


//#ifndef USE_ATD_IF_DEMOD
//#define USE_ATD_IF_DEMOD 0
//#endif
#ifndef USE_DTD_IF_DEMOD
#define USE_DTD_IF_DEMOD 1
#endif


//#if USE_ATD_IF_DEMOD
#ifdef USE_ATD_IF_DEMOD
#define SV_VS_AT1	VSS_MKVSRC(VSS_ATD,0)	/* name for Analog Tuner 1*/
#define SV_VS_TUNER1	VSS_MKVSRC(VSS_ATD,0)	/* to be deprecate */
#else
#define SV_VS_AT1	VSS_MKVSRC(VSS_ATV,0)	/* name for Analog Tuner 1*/
#define SV_VS_TUNER1	VSS_MKVSRC(VSS_ATV,0)	/* to be deprecate */
#endif
#define SV_VS_VGA	VSS_MKVSRC(VSS_VGA,0)
#define SV_VS_DVI	VSS_MKVSRC(VSS_DVI,0)

#if 1 //USE_DTD_IF_DEMOD
/* currently only use DTV in driver, DTD has been created for future */
#define SV_VS_DT1	VSS_MKVSRC(VSS_DTV,0)
#define SV_VS_DT2	VSS_MKVSRC(VSS_DTV,1)
#define SV_VS_SCART	VSS_MKVSRC(VSS_SCART,0) /*may obsolete, don't use*/
#endif


////////////////////////////////////////////////////////////////////////////////
// Origianl Source Table Related define
////////////////////////////////////////////////////////////////////////////////

#define MS(MainSrc, PIPSrc) (((UINT16)(MainSrc)<<8)|(UINT16)(PIPSrc))

/** Input port of IC */
enum IC_Input
{
	P_C0 = 0,
	P_C1 = 1,
	P_C2 = 2,
	P_C3 = 3,
	P_C4 = 4, 
	P_SV0 = 5,
	P_SV1 = 6,
	P_SV2 = 7,
	P_YP0 = 8,
	P_YP1 = 9,
	P_VGA = 10,
	P_FB0 = 11,
	P_FB1 = 12,
	P_DVI = 13,
	P_CCIR = 14,
	P_DT1 = 15, 
	P_DT2 = 16, // for 5371
	P_VGACOMP = 17,
	P_MA = 18,
	P_FA = 0XFF
};
#ifdef CC_UP8032_ATV
typedef enum {
    E_VS_SRC,
    E_IC_PORT,
    E_AV_MUX,
    E_HDTV_IC_PORT
} E_RPORTMAP_T;
extern UINT8 u1Get_portmap_table(UINT8 idx, E_RPORTMAP_T field);
#endif
typedef struct {
	UINT8  vs_src;
	UINT8  ic_port;
	UINT8  av_mux;
	UINT8   hdtv_ic_port;
}  rPortmap_t;


#define AVMUX(ch0, ch1)		((ch1<<4)|(ch0))
#define AVMUX_CH0(x)		((x)&0xf)
#define AVMUX_CH1(x)		(((x)>>4)&0xf)

#if 0
#define GPIOMUX(gpiofld,value8)		((gpiofld<<8)|(value8&0xff))
#define GPIOFLD(wid,shft)			((wid<<4)|shft)
#define GPIOMUX2MSK(fld)		(((1<<(fld>>12))-1)<<((fld>>8)&0xf))
#endif 

extern const rPortmap_t code portmap_table[] ;
extern const UINT8 portmap_table_num;

////////////////////////////////////////////////////////////////////////////////
// 10bit ADC input MUX, 
////////////////////////////////////////////////////////////////////////////////

#if 0
#define CVBS0	1L // penggang add L
#define CVBS1	2L
#define CVBS2	3L
#define CVBS3	4L
#define CVBS4	5L 
#define SY0 	6L
#define SC0	7L
#define SY1	8L
#define SC1	9L
#define SY2	10L
#define SC2	11L
#define AVMUX_MAX SC2
#else
#define CVBS0	1L // penggang add L
#define CVBS1	2L
#define CVBS2	3L
#define SY0 	4L
#define SC0	5L
#define SY1	6L
#define SC1	7L
#define AVMUX_MAX SC1

#define CVBS3	0L
#define CVBS4	0L 
#define SY2	0L
#define SC2	0L
#endif



/**
 * MapIntMode -
 * left nibble: ic input port of main path
 * right nibble: ic input port of pip path
 * SetGPIO - external setting
 */
typedef struct ExtInputTable {
	UINT16 MapIntMode;
	UINT8 SetGPIO;
} ExtInputTable;

void vDrvSetExternalMux(UINT8 bMainSrc, UINT8 bSubSrc) ;

UINT16 bDrvGetMapIntMode(UINT8 bMainSrc, UINT8 bSubSrc) ;


#if 0
UINT8 bDrvGetFBCVBS(UINT8 bInPort) ;
UINT8 bDrvGetFBRGB(UINT8 bInPort) ;
UINT8 fgSFirstAV(void) ;
UINT8 fgSFirstSV(void) ;
UINT8 fgSupportSFirst(void) ;
#endif

void vDrvAvMux(UINT32 dMUX) ;
UINT8 u1GetMonICIn(UINT8 bSrc);
UINT8 bAvmuxCh0ToCh1(UINT8 av_mux);
#ifdef CC_UP8032_ATV
void vChangeAudioSrc(UINT8 bAudioSrc);
#endif
#endif
