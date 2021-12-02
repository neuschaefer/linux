/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file pql_ddi.h
 *
 *  PQL include header.
 *
 *  @author	Youm, Seung-Jun(sjyoum@lge.com)
 *  @version	1.0
 *  @date		2006.08.30
 *  @see
 */


/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef	__PQL_DDI_H_
#define	__PQL_DDI_H_

/******************************************************************************
    전역 제어 상수(Control Constants)
******************************************************************************/

/******************************************************************************
	#include 파일들 (File Inclusions)
******************************************************************************/
#include "common.h"
#include "osa_modeldef.h"
#include "branches.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
	상수 정의
	(Constant Definitions)
*******************************************************************************/
//#define	FMT_CONVERT	1	// jackee@lge.com, moved to incs.mk

//#define ADCCAL_BYCSC

#define PQL_SELECT_ON	1
#ifdef INCLUDE_PQL_GP4_RESTRUCT
#define PQL_ON					(1)
#define PQL_OFF					(0)

#define PQL_OK					(0)
#define PQL_NOT_OK				(-1)

#define PQL_SUCCESS(e)      ((e) == PQL_OK)
#define PQL_ERROR(e)        ((e) != PQL_OK)
#endif


/* LG DDI -------------------------------------------------------------------*/
#define PQL_NR_PROGRESSIVE_TIMING			0x01
#define PQL_NR_NO_PROGRESSIVE_TIMING		0x02

#define DC2APLCTRL_DATA_LOW 	0x00
#define DC2APLCTRL_DATA_MEDIUM 	0x01
#define DC2APLCTRL_DATA_HIGH 	0x02
#define DC2APLCTRL_DATA_STORE	0x03

//For Chip Support Data inform (DC2)
#define	SUPPORT_HISTOGRAM_DATA			0x00000001
#define	SUPPORT_APL_TOTAL_DATA			0x00000002
#define	SUPPORT_APL_MIN_DATA			0x00000004
#define	SUPPORT_APL_MAX_DATA			0x00000008
#define	SUPPORT_PEAK_LOW_DATA			0x00000010
#define	SUPPORT_PEAK_HIGH_DATA			0x00000020


#define SUPPORT_HISTOGRAM(_v)			( _v | SUPPORT_HISTOGRAM_DATA)
#define NO_SUPPORT_HISTOGRAM(_v)		( _v & (~SUPPORT_HISTOGRAM_DATA))

#define SUPPORT_APL_TOTAL(_v)			( _v | SUPPORT_APL_TOTAL_DATA)
#define NO_SUPPORT_APL_TOTAL(_v)		( _v & (~SUPPORT_APL_TOTAL_DATA))

#define SUPPORT_APL_MIN(_v)			( _v | SUPPORT_APL_MIN_DATA)
#define NO_SUPPORT_APL_MIN(_v)			( _v & (~SUPPORT_APL_MIN_DATA))

#define SUPPORT_APL_MAX(_v)			( _v | SUPPORT_APL_MAX_DATA)
#define NO_SUPPORT_APL_MAX(_v)		( _v & (~SUPPORT_APL_MAX_DATA))

#define SUPPORT_PEAK_LOW(_v)			( _v | SUPPORT_PEAK_LOW_DATA)
#define NO_SUPPORT_PEAK_LOW(_v)		( _v & (~SUPPORT_PEAK_LOW_DATA))

#define SUPPORT_PEAK_HIGH(_v)			( _v | SUPPORT_PEAK_HIGH_DATA)
#define NO_SUPPORT_PEAK_HIGH(_v)		( _v & (~SUPPORT_PEAK_HIGH_DATA))

/**
* Input Type & Format Type Definition for PQL
*/
/* PQL_INPUT_TYPE */
#define PQL_INPUT_DTV			0		/* Digital_RF */
#define PQL_INPUT_ANA			1		/* Analog_RF/CVBS/S-video  */
#define PQL_INPUT_COMP			2		/* Component */
#define PQL_INPUT_HDMI			3		/* HDMI/DVIDTV/DVIPC */
#define PQL_INPUT_RGBPC			4		/* RGB_PC  */
#define PQL_INPUT_DIVX			5		/* DIVX */
#define PQL_INPUT_PVR			6		/* PVR*/
#define PQL_INPUT_IPTV			7		/* IPTV*/
#define PQL_SOURCE_MAX			8		/* Max source in this system */



#define PQL_ALL_THE_SAME_INDEX			0
#define PQL_ALL_THE_SAME_INDEX_MAX		1
#define PQL_NA_INDEX_MAX				0
#define PQL_END_OF_ARRAY				0xFF

#define PQL_INPUT_SRC_FMT_T			UINT32

#define PQL_LSHIFT_DATA(val, _bits, _pos)	((val & _bits) << _pos)
#define PQL_DATA_MASK(_bits, _pos)			((_bits) << _pos)
#define PQL_RSHIFT_DATA(val, _bits, _pos)	((val & PQL_DATA_MASK(_bits, _pos)) >> _pos)


#define PQL_3DMODE_POS				30
#define PQL_3DMODE_BITS				0x00000003
#define PQL_SET_3DMODE(_srcfmt,_idx)	\
	_srcfmt = ((_srcfmt \
						& ~(PQL_DATA_MASK(PQL_3DMODE_BITS, PQL_3DMODE_POS))) \
						| (PQL_LSHIFT_DATA(_idx, PQL_3DMODE_BITS, PQL_3DMODE_POS)))

#define PQL_GET_3DMODE(_srcfmt)		PQL_RSHIFT_DATA(_srcfmt, PQL_3DMODE_BITS, PQL_3DMODE_POS)

#define PQL_3D_IN_FMT_POS			26
#define PQL_3D_IN_FMT_BITS			0x0000000f
#define PQL_SET_3D_IN_FMT(_srcfmt,_idx)	\
	_srcfmt = ((_srcfmt \
						& ~(PQL_DATA_MASK(PQL_3D_IN_FMT_BITS, PQL_3D_IN_FMT_POS))) \
						| (PQL_LSHIFT_DATA(_idx, PQL_3D_IN_FMT_BITS, PQL_3D_IN_FMT_POS)))

#define PQL_GET_3D_IN_FMT(_srcfmt)		PQL_RSHIFT_DATA(_srcfmt, PQL_3D_IN_FMT_BITS, PQL_3D_IN_FMT_POS)


#define PQL_SHOPMODE_POS			25
#define PQL_SHOPMODE_BITS			0x00000001
#define PQL_SET_SHOPMODE(_srcfmt,_idx)	\
	_srcfmt = ((_srcfmt \
						& ~(PQL_DATA_MASK(PQL_SHOPMODE_BITS, PQL_SHOPMODE_POS))) \
						| (PQL_LSHIFT_DATA(_idx, PQL_SHOPMODE_BITS, PQL_SHOPMODE_POS)))
#define PQL_GET_SHOPMODE(_srcfmt)		PQL_RSHIFT_DATA(_srcfmt, PQL_SHOPMODE_BITS, PQL_SHOPMODE_POS)

#define PQL_MAIN_SUB_POS			23
#define PQL_MAIN_SUB_BITS			0x00000003
#define PQL_SET_MAIN_SUB(_srcfmt,_idx)	\
	_srcfmt = ((_srcfmt \
						& ~(PQL_DATA_MASK(PQL_MAIN_SUB_BITS, PQL_MAIN_SUB_POS))) \
						| (PQL_LSHIFT_DATA(_idx, PQL_MAIN_SUB_BITS, PQL_MAIN_SUB_POS)))
#define PQL_GET_MAIN_SUB(_srcfmt)		PQL_RSHIFT_DATA(_srcfmt, PQL_MAIN_SUB_BITS, PQL_MAIN_SUB_POS)

#define PQL_WRDY_POS			16
#define PQL_WRDY_BITS			0x00000001
#define PQL_SET_WRDY(_srcfmt,_idx)	\
	_srcfmt = ((_srcfmt \
						& ~(PQL_DATA_MASK(PQL_WRDY_BITS, PQL_WRDY_POS))) \
						| (PQL_LSHIFT_DATA(_idx, PQL_WRDY_BITS, PQL_WRDY_POS)))
#define PQL_GET_WRDY(_srcfmt)		PQL_RSHIFT_DATA(_srcfmt, PQL_WRDY_BITS, PQL_WRDY_POS)

#define PQL_INPUT_TYPE_POS			8
#define PQL_INPUT_TYPE_BITS			0x000000ff
#define PQL_SET_INPUT_TYPE(_srcfmt,_idx)	\
	_srcfmt = ((_srcfmt \
						& ~(PQL_DATA_MASK(PQL_INPUT_TYPE_BITS, PQL_INPUT_TYPE_POS))) \
						| (PQL_LSHIFT_DATA(_idx, PQL_INPUT_TYPE_BITS, PQL_INPUT_TYPE_POS)))
#define PQL_GET_INPUT_TYPE(_srcfmt)		PQL_RSHIFT_DATA(_srcfmt, PQL_INPUT_TYPE_BITS, PQL_INPUT_TYPE_POS)

#define PQL_RES_INDEX_POS			0
#define PQL_RES_INDEX_BITS			0x000000ff
#define PQL_SET_RES_INDEX(_srcfmt,_idx)	\
	_srcfmt = ((_srcfmt \
						& ~(PQL_DATA_MASK(PQL_RES_INDEX_BITS, PQL_RES_INDEX_POS))) \
						| (PQL_LSHIFT_DATA(_idx, PQL_RES_INDEX_BITS, PQL_RES_INDEX_POS)))
#define PQL_GET_RES_INDEX(_srcfmt)		PQL_RSHIFT_DATA(_srcfmt, PQL_RES_INDEX_BITS, PQL_RES_INDEX_POS)

#define PQL_INDEXTBL_START_INDEX				0
#define PQL_INDEXTBL_TRID_START_INDEX			4
#define PQL_3D_IN_FMT_NUM						4
#define PQL_INDEXTBL_TRID_WRDY_OFFSET			(PQL_3D_IN_FMT_NUM*2)
#define PQL_INDEXTBL_SHOP_MODE_OFFSET			1
#define PQL_INDEXTBL_SUB_MODE_OFFSET			0
#define PQL_INDEXTBL_WRDY_OFFSET				2

//[090908-clearcut] DCNR
#define DCNR_DNR_LUT_NUM 16             // Dnr 메인 LUT의 entry 수
#define DCNR_HISTO_BIN_NUM 4                     // Histogram 정보 수집 시 사용될 BIN의 수 (x)
#define DCNR_HISTO_THRD 64800            // 알고리즘 적용 조건 판단을 위한 Threshold
#define DCNR_FIR_TAB_NUM 5


/*******************************************************************************
	매크로 정의
	(Macro Definitions)
*******************************************************************************/
#ifdef INCLUDE_PQLDB_FIDX
#define IF_PQLFMT_IS_INTERLACE(x) if(	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_DTV && \
										(	PQL_GET_RES_INDEX(x)==PQL_DTV_HD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_HD_I_DVB || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_240I_50HZ || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_SD_I_DVB	\
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_ANA \
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_COMP && \
										(	PQL_GET_RES_INDEX(x)==PQL_COMP_HD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_COMP_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_COMP_HD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_COMP_SD_I_PAL\
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_HDMI && \
										(	PQL_GET_RES_INDEX(x)==PQL_HDMIAV_HD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_HD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_HD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_HD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_SD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_HD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_HD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_SD_I_PAL \
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_PVR && \
										(	PQL_GET_RES_INDEX(x)==PQL_PVRDTV_HD_I || \
											PQL_GET_RES_INDEX(x)==PQL_PVRDTV_SD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_PVRDTV_HD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_PVRDTV_SD_I_PAL \
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_IPTV && \
										(	PQL_GET_RES_INDEX(x)==PQL_IPTV_HD_I || \
											PQL_GET_RES_INDEX(x)==PQL_IPTV_SD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_IPTV_HD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_IPTV_SD_I_PAL \
										)	\
									)	\
								)
#else
#define IF_PQLFMT_IS_INTERLACE(x) if(	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_DTV && \
										(	PQL_GET_RES_INDEX(x)==PQL_DTV_HD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_HD_I_DVB || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_240I_50HZ || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_SD_I_DVB	\
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_ANA \
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_COMP && \
										(	PQL_GET_RES_INDEX(x)==PQL_COMP_HD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_COMP_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_COMP_HD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_COMP_SD_I_PAL\
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_HDMI && \
										(	PQL_GET_RES_INDEX(x)==PQL_HDMIAV_HD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_HD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_HD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_HD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_SD_I_PAL \
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_PVR && \
										(	PQL_GET_RES_INDEX(x)==PQL_PVRDTV_HD_I || \
											PQL_GET_RES_INDEX(x)==PQL_PVRDTV_SD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_PVRDTV_HD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_PVRDTV_SD_I_PAL \
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_IPTV && \
										(	PQL_GET_RES_INDEX(x)==PQL_IPTV_HD_I || \
											PQL_GET_RES_INDEX(x)==PQL_IPTV_SD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_IPTV_HD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_IPTV_SD_I_PAL \
										)	\
									)	\
								)
#endif

#define IF_PQLFMT_IS_PC(x) if(	\
								(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_RGBPC && \
									(	PQL_GET_RES_INDEX(x)==PQL_RGBPC_VGA || \
										PQL_GET_RES_INDEX(x)==PQL_RGBPC_SVGA || \
										PQL_GET_RES_INDEX(x)==PQL_RGBPC_XGA || \
										PQL_GET_RES_INDEX(x)==PQL_RGBPC_WXGA || \
										PQL_GET_RES_INDEX(x)==PQL_RGBPC_UXGA || \
										PQL_GET_RES_INDEX(x)==PQL_RGBPC_FHD	\
									)	\
							   	) || \
								(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_HDMI && \
									(	PQL_GET_RES_INDEX(x)==PQL_DVIPC_VGA  || \
										PQL_GET_RES_INDEX(x)==PQL_DVIPC_SVGA || \
										PQL_GET_RES_INDEX(x)==PQL_DVIPC_XGA || \
										PQL_GET_RES_INDEX(x)==PQL_DVIPC_WXGA || \
										PQL_GET_RES_INDEX(x)==PQL_DVIPC_UXGA || \
										PQL_GET_RES_INDEX(x)==PQL_DVIPC_FHD \
									)	\
								)	\
							)

#define IF_PQLFMT_ISNOT_PC(x) if(	\
									!(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_RGBPC && \
										(	PQL_GET_RES_INDEX(x)==PQL_RGBPC_VGA || \
											PQL_GET_RES_INDEX(x)==PQL_RGBPC_SVGA || \
											PQL_GET_RES_INDEX(x)==PQL_RGBPC_XGA || \
											PQL_GET_RES_INDEX(x)==PQL_RGBPC_WXGA || \
											PQL_GET_RES_INDEX(x)==PQL_RGBPC_UXGA || \
											PQL_GET_RES_INDEX(x)==PQL_RGBPC_FHD	\
										)	\
								   	) || \
									!(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_HDMI && \
										(	PQL_GET_RES_INDEX(x)==PQL_DVIPC_VGA  || \
											PQL_GET_RES_INDEX(x)==PQL_DVIPC_SVGA || \
											PQL_GET_RES_INDEX(x)==PQL_DVIPC_XGA || \
											PQL_GET_RES_INDEX(x)==PQL_DVIPC_WXGA || \
											PQL_GET_RES_INDEX(x)==PQL_DVIPC_UXGA || \
											PQL_GET_RES_INDEX(x)==PQL_DVIPC_FHD \
										)	\
									)	\
								)
#ifdef INCLUDE_PQLDB_FIDX
#define IF_PQLFMT_IS_HDMI(x) if(	(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_HDMI && \
										(	PQL_GET_RES_INDEX(x)==PQL_HDMIAV_FHD  || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_HD_I || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_HD_P || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_P || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_FHD_PAL  || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_HD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_HD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_FHD  || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_HD_I || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_HD_P || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_SD_P || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_FHD_PAL  || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_HD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_HD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_SD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_SD_I_PAL \
										)	\
									)	\
								)
#else
#define IF_PQLFMT_IS_HDMI(x) if(	(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_HDMI && \
										(	PQL_GET_RES_INDEX(x)==PQL_HDMIAV_FHD  || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_HD_I || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_HD_P || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_P || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_FHD_PAL  || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_HD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_HD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_I_PAL \
										)	\
									)	\
								)
#endif

#ifdef INCLUDE_PQLDB_FIDX
#define IF_PQLFMT_IS_SD(x) if(	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_DTV && \
										(	PQL_GET_RES_INDEX(x)==PQL_DTV_SD_P  || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_SD_P_DVB || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_240I_50HZ || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_SD_I_DVB	\
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_ANA \
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_COMP && \
										(	PQL_GET_RES_INDEX(x)==PQL_COMP_SD_P  || \
											PQL_GET_RES_INDEX(x)==PQL_COMP_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_COMP_SD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_COMP_SD_I_PAL\
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_HDMI && \
										(	PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_P  || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_SD_P  || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_SD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_SD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_SD_P  || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_SD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_RGB_SD_I_PAL \
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_PVR && \
										(	PQL_GET_RES_INDEX(x)==PQL_PVRDTV_SD_P || \
											PQL_GET_RES_INDEX(x)==PQL_PVRDTV_SD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_PVRDTV_SD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_PVRDTV_SD_I_PAL \
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_IPTV && \
										(	PQL_GET_RES_INDEX(x)==PQL_IPTV_SD_P || \
											PQL_GET_RES_INDEX(x)==PQL_IPTV_SD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_IPTV_SD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_IPTV_SD_I_PAL \
										)	\
									)	\
								)
#else
#define IF_PQLFMT_IS_SD(x) if(	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_DTV && \
										(	PQL_GET_RES_INDEX(x)==PQL_DTV_SD_P  || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_SD_P_DVB || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_240I_50HZ || \
											PQL_GET_RES_INDEX(x)==PQL_DTV_SD_I_DVB	\
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_ANA \
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_COMP && \
										(	PQL_GET_RES_INDEX(x)==PQL_COMP_SD_P  || \
											PQL_GET_RES_INDEX(x)==PQL_COMP_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_COMP_SD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_COMP_SD_I_PAL\
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_HDMI && \
										(	PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_P  || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_HDMIAV_SD_I_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_SD_P  || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_SD_I || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_SD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_DVIDTV_SD_I_PAL \
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_PVR && \
										(	PQL_GET_RES_INDEX(x)==PQL_PVRDTV_SD_P || \
											PQL_GET_RES_INDEX(x)==PQL_PVRDTV_SD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_PVRDTV_SD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_PVRDTV_SD_I_PAL \
										)	\
									) ||	\
									(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_IPTV && \
										(	PQL_GET_RES_INDEX(x)==PQL_IPTV_SD_P || \
											PQL_GET_RES_INDEX(x)==PQL_IPTV_SD_I  || \
											PQL_GET_RES_INDEX(x)==PQL_IPTV_SD_P_PAL || \
											PQL_GET_RES_INDEX(x)==PQL_IPTV_SD_I_PAL \
										)	\
									)	\
								)
#endif

#define PQLFMT_IS_DTV(x) if(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_DTV)

#define PQLFMT_IS_ANA(x) if(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_ANA)

#define PQLFMT_IS_SIMPLE_MENU_TYPE(x) if(PQL_GET_INPUT_TYPE(x)== PQL_INPUT_DIVX)

#define PQL_ADC_COEFF_MAX			3	/* Coeff_ADC의 Data 길이 변경시 변화시켜야 함.*/
#define PQL_ADC_COEFF_MIN			0
#define PQL_ADC_COEFF_ADAPT_MAX	3	/* Coeff_ADC_Adapt의 Data 길이 변경시 변화시켜야 함.*/
#define PQL_ADC_COEFF_ADAPT_MIN	0

/*******************************************************************************
    형 정의
    (Type Definitions)
*******************************************************************************/


/**
 * color temperature struc.
 *
 * @see
 */
typedef enum {
	PQL_WB_RED		= 0,				/**< white balance Red data*/
	PQL_WB_GRN		= 1,				/**< white balance Green data*/
	PQL_WB_BLU		= 2,				/**< white balance Blue data*/

} PQL_WB_COLOR_T;

/**
 * color temperature 값.
 *
 * @see
 */
typedef struct PQL_COLOR_TEMP
{
	UINT8	rgbGain[3];			/**< Gain. 0:Red, 1:Green, 2:Blue*/
	UINT8	rgbOffset[3];		/**< Offset. 0:Red, 1:Green, 2:Blue*/

}PQL_COLOR_TEMP_T;

/**
 * white balace 값.
 *
 * @see
 */
typedef struct PQL_WB_DATA
{
	PQL_COLOR_TEMP_T	wbCool;				/**< white balance Cool data*/
	PQL_COLOR_TEMP_T	wbNorm;				/**< white balance Normal data*/
	PQL_COLOR_TEMP_T	wbWarm;				/**< white balance Warm data*/

}PQL_WB_DATA_T;


/**
 * color temperature 값.
 *
 * @see
 */
typedef struct PQL_COLOR_TEMP_CORRECT
{
	SINT32	rgbGainCorrect[3];			/**< Gain. 0:Red, 1:Green, 2:Blue*/
	SINT16	rgbOffsetCorrect[3];		/**< Offset. 0:Red, 1:Green, 2:Blue*/

}PQL_COLOR_TEMP_CORRECT_T;

/**
 * white balace 값.
 *
 * @see
 */
typedef struct PQL_WB_CORRECT_DATA
{
	PQL_COLOR_TEMP_CORRECT_T	wbCorrectCool;				/**< white balance Cool data*/
	PQL_COLOR_TEMP_CORRECT_T	wbCorrectNorm;				/**< white balance Normal data*/
	PQL_COLOR_TEMP_CORRECT_T	wbCorrectWarm;				/**< white balance Warm data*/

}PQL_WB_CORRECT_DATA_T;



/**
 * Sub-Brightness, Sub-Contrast struct.
 *
 * @see
 */
typedef struct {

	UINT8	subBrightness;						/**< sub brightness data*/
	UINT8	subContrast;						/**< sub contrast data*/

} PQL_SUB_CONTBRI_T;

/**
 * CM : Output CSC coefficient struct.
 *
 * @see
 */
typedef struct PQL_CM_OUT_CS
{
	//Output CSC Setting Val.
	float fColorStdMatrix1[3];
	float fColorStdMatrix2[3];
	float fColorStdMatrix3[3];
	UINT8 uColorStdForRev ;
}PQL_CM_OUT_CS_T;

/**
 * PQL Process Path.
 * Main Path(Main External Input, Main Format converter, Main Post Processing)와\n
 * Sub Path (Sub External Input, Sub Format converter, Sub Post Processing, DENC)를
 * 지정할 때 사용한다.
 * @ingroup
 */
typedef enum {
	PQL_PATH_MAIN		= 0,		/* main path */
	PQL_PATH_SUB,				/* sub path */
	PQL_PATH_VCR,				/* vcr out path */
	PQL_PATH_MAXN
} PQL_PATH_T;

typedef struct PQL_CM_LEVEL_ADJ{
    UINT16 pqlLevelYGain;
    UINT16 pqlLevelYOfst;
    UINT16 pqlLevelCGain;
}PQL_CM_LEVEL_ADJ_T;

/**
 * SH : Sharpness UI value struct.
 *
 * @see
 */
typedef struct PQL_SHARP_UI_VALUE{
	UINT8	nSharpnessCtrlType;
	float	fSharpnessUi;
	float	fHSharpnessUi;
	float	fVSharpnessUi;
}PQL_SHARP_UI_VALUE_T;

/**
 * SR : SR Level Enumeration.
 *
 * @see
 */
typedef enum {
	  PQL_SR_OFF	  = 0,
	  PQL_SR_ON
} PQL_SR_LEVEL_T;

/**
 * NR : NR Level Enumeration.
 *
 * @see
 */
typedef enum {
	  PQL_NR_OFF	  = 0,
	  PQL_NR_LOW,
	  PQL_NR_MEDIUM,
	  PQL_NR_HIGH,
	  PQL_NR_AUTO,
	  PQL_NR_DISABLE
} PQL_NR_LEVEL_T;

#if 0
/**
 * BLACK : Black Level Source ID.
 *
 * @see
 */
typedef enum {
	PQL_BLACK_HDMI		= 0,			/**< Black for HDMI 		*/
	PQL_BLACK_COMP, 					/**< Black for Component 	*/
	PQL_BLACK_DECODER, 					/**< Black for Decoder type	*/
	PQL_BLACK_OTHERS,					/**< Black for others		*/
} PQL_BLACK_SOURCE_T;
#else
/**
 * BLACK : Black Level Source ID.
 *
 * @see
 */
typedef enum {
	PQL_BLACK_RF		= 0,			/**< Black for RF 		*/
	PQL_BLACK_AV,					/**< Black for AV 		*/
	PQL_BLACK_COMP,				/**< Black for Component	*/
	PQL_BLACK_RGB,					/**< Black for RGB 		*/
	PQL_BLACK_HDMI,				/**< Black for HDMI 		*/
	PQL_BLACK_DECODER, 			/**< Black for Digital Decoder type	*/
	PQL_BLACK_SCARTRGB, 		/**< Black for SCART RGB type	*/
	PQL_BLACK_OTHERS,			/**< Black for others		*/
	PQL_BLACK_JPEG,				/**< Black for JPEG Image */
	PQL_BLACK_INVALID				/**< invalid		*/
} PQL_BLACK_SOURCE_T;
#endif

/**
 * BLACK : Black Level Control Setting Value Enumeration.
 *
 * @see
 */
typedef enum {
	PQL_BLACK_LOW		= 0,			/**< Black Level Low	*/
	PQL_BLACK_HIGH, 					/**< Black Level High	*/
	PQL_BLACK_AUTO, 					/**< Black Level Auto	*/
} PQL_BLACK_T;

/**
 * PC : PC Control UI value Structure.
 *
 * @see
 */
typedef struct PQL_PC_UI_VALUE{
	float contrastUi;
	float brightnessUi;
	float colorUi;
	float tintUi;
	float contrastCentreUi;
}PQL_PC_UI_VALUE_T;

 /**
 * Loading UI matching table val in case of EyeQ.
 *
 * @see
 */
 typedef enum {
		 PQL_EYEQ_CONTRAST	 = 0x0000,
		 PQL_EYEQ_BRIGHTNESS,
		 PQL_EYEQ_COLOR,
		 PQL_EYEQ_TINT,
		 PQL_EYEQ_SHARPNESS,
		 PQL_EYEQ_BACKLIGHT
 } PQL_EYEQ_DB_MEMBER_T;

 /**
 * Loading UI matching Offset table val in case of KIND.
 *
 * @see
 */
 typedef enum {
		 PQL_KIND_CONTRAST_OFFSET	 = 0x0000,
		 PQL_KIND_BRIGHTNESS_OFFSET,
		 PQL_KIND_COLOR_OFFSET,
		 PQL_KIND_TINT_OFFSET,
		 PQL_KIND_SHARPNESS_OFFSET
 } PQL_KIND_DB_MEMBER_T;

 /**
  * Loading DC2P DynamicContrast value parameters table val.
  *
  * @see
  */
  typedef enum {
		  PQL_DIMMING_DC2P_APL	= 0x0000,
		  PQL_DIMMING_DC2P_APLMATCH_DIMRANGE	= 0x0001,
		  PQL_DIMMING_UIPOINT	= 0x0002,
		  PQL_DIMMING_UIMATCH_DIMRANGE	= 0x0003
  } PQL_DIMMING_DB_MEMBER_T;

#define PQL_EYECARE_CURVE_POINT_NUM	7

/**
*	 EyeCare control parameter.
*
*/
typedef struct  {
	SINT32 	 nRawApl[PQL_EYECARE_CURVE_POINT_NUM];
	SINT32 	 nDimRangeWithRawApl[PQL_EYECARE_CURVE_POINT_NUM];
} PQL_EYECARE_DIMMING_DATA_T;

 /**
 *	 EyeCare Total parameter to control dimming.
 *
 */
 typedef struct {
	 PQL_EYECARE_DIMMING_DATA_T  stPcEyecareHighCtrlData ;
	 PQL_EYECARE_DIMMING_DATA_T  stPcEyecareLowCtrlData ;
 }PQL_EYECARE_DIMMING_DB_T;

 /**
 * Loading DC2P DynamicContrast parameters table value.
 *
 * @see
 */
 typedef enum {
		 PQL_DC2P_LOWPEAK_POS	 = 0x0000,	 /*  1 parameter is required	 */
		 PQL_DC2P_HIGHPEAK_POS, 		 		/*  1 parameter is required	 */

		 PQL_DC2P_LOWPEAK,					 /*  2 parameter is required	 */
		 PQL_DC2P_BRIGHTNESS_OFFSET,	 		/*  2 parameter is required	 */
		 PQL_DC2P_HIGHPEAK, 			 		/*  2 parameter is required	 */
		 PQL_DC2P_CONTRAST_GAIN,		 		/*  2 parameter is required	 */
		 PQL_DC2P_DYNAMIC_COLOR_GAIN	 	/*  2 parameter is required	 */
 } PQL_DYNAMICCONTRAST_DB_MEMBER_T;


#define CURVE_NUM	5
#define FREE_POINT 5
#define NUM_TRANSCURVE 33
#define TOTAL_HISTO_BIN_SIZE 32
#define CGAIN_CURVE_SIZE_MAX 16
#define MAX_CURVE_IIR	5

 /**
 *	 pql DC2 algorithm control parameter.
 *
 */
 typedef struct {
 	 SINT32 	 nLowPeakPos;
	 SINT32 	 nHighPeakPos;

	 // Brightness
	 //SINT32 	 nBrightnessMin;	// 20091113, sjyoum, not used!!
	 //SINT32 	 nBrightnessMax;	// 20091113, sjyoum, not used!!
	 SINT32 	 nLowPeak[CURVE_NUM];			 // x1
	 SINT32 	 nBrightnessOffset[CURVE_NUM];		 // y1

	 // Contrast
	 //SINT32 	 nContrastMin;		// 20091113, sjyoum, not used!!
	 //SINT32 	 nContrastMax;		// 20091113, sjyoum, not used!!
	 SINT32 	 nHighPeak[CURVE_NUM];			 // x3
	 SINT32 	 nContrastGain[CURVE_NUM];		 // y3
	 SINT32 	 nDynColorGain[CURVE_NUM];
 } PQL_DC2P_DYN_CONTRAST_DB_T;

/**
*	DC2 algorithm Apl control parameter.
*
*/
typedef struct  {
	SINT32 nAplPoint[FREE_POINT];
	SINT32 nTransCurve[NUM_TRANSCURVE][FREE_POINT];
//이 block이 추가됨.
	SINT32 sWCurve1P[NUM_TRANSCURVE];
	SINT32 sWCurve2P[NUM_TRANSCURVE];
	SINT32 sWCurve3P[NUM_TRANSCURVE];
	SINT32 sWCurve4P[NUM_TRANSCURVE];
	SINT32 sStartWC[NUM_TRANSCURVE];
	SINT32 sMidWC[NUM_TRANSCURVE];
	SINT32 sEndWC[NUM_TRANSCURVE];
	SINT32 sLimitWC[NUM_TRANSCURVE];
	SINT32 swHistoEffectGain[NUM_TRANSCURVE];
} PQL_DC2P_APL_DB_T;

#if 1	//KWON_1022
/**
*	pql DC2 CGain Algorithm Tunning parameter.
*
*/
typedef struct {
	/* FIR for Histogram Modification 			*/
	SINT32			fWHistogramMin;
	SINT32			fWHistogramMax;
	SINT32			fCgainHistogramW[TOTAL_HISTO_BIN_SIZE];
	SINT32			fCgainMin[CGAIN_CURVE_SIZE_MAX];
	SINT32			fCgainMax[CGAIN_CURVE_SIZE_MAX];
} PQL_DC2P_CGAIN_DB_T;

/**
*	pql DC2 Skin Algorithm Tunning parameter.
*
*/
typedef struct {
	SINT32		fSkinMin;
	SINT32		fSkinMax;

	SINT32		fAplCurve[MAX_CURVE_IIR];			// x1
	SINT32		fCurrWeightCurve[MAX_CURVE_IIR];		// y1
} PQL_DC2P_SKIN_DB_T;
#endif

/**
 * CM : Color Standard Setting Value Enumeration.
 *
 * @see
 */
 typedef enum {
	PQL_CS_SD=0,
	PQL_CS_HD,
	PQL_CS_MAX
} PQL_CS_T;

#if 0	//KWON_0810 Unused
/**
 * AFEFilter : AFEFilter Setting type Enumeration.
 *
 * @see
 */
typedef enum {
	SAFEFILTER_RF_TYPE,
	SAFEFILTER_AV_SVIDEO_TYPE,
	SAFEFILTER_SCART_RGB_COMPSD_I_TYPE,
	SAFEFILTER_COMPSD_P_TYPE,
	SAFEFILTER_COMP720_TYPE,
	SAFEFILTER_COMP1080I_TYPE,
	SAFEFILTER_COMP1080P_TYPE
}PQL_AFEF_T;
#endif

/**
 * AVDEC : Analog Video Decoder Input Mode Enumeration.
 *
 * @see
 */
typedef enum
{
	PQL_AVDEC_MODE_NTSC,
	PQL_AVDEC_MODE_PAL,
	PQL_AVDEC_MODE_PAL_N,
	PQL_AVDEC_MODE_PAL_M,
	PQL_AVDEC_MODE_SECAM,
	PQL_AVDEC_MODE_NTSC_443,
	PQL_AVDEC_MODE_PAL_60,
	PQL_AVDEC_MODE_UNKNOWN_525,
	PQL_AVDEC_MODE_UNKNOWN_625,
	PQL_AVDEC_MODE_UNKNOWN,
}PQL_AVDECODER_VIDEO_MODE_T;

/**
 * Edge Enhance Level Enumeration.
 *
 * @see
 */
typedef enum {
	  PQL_EDGE_ENHANCE_OFF	  = 0,
	  PQL_EDGE_ENHANCE_LOW,
	  PQL_EDGE_ENHANCE_HIGH
} PQL_EDGE_ENHANCE_UI_LEVEL_T;


/**
 * DTV fmt type.
 *
 * 2008.10.25 PSI요청으로 변경.
 * 2009.06.19 GP2용 신규 Index 만들기.
 * @see .
*/
typedef enum {
/* PQL_INPUT_DTV			: Digital_RF + Digital_PVR 						*/
	PQL_DTV_FHD				= 0 ,	/*0 HD Progressive :	1080p	*/
	PQL_DTV_HD_I			,		/*1 HD Interlace : 	1080i	*/
	PQL_DTV_HD_P			,		/*2 HD Progressive :	 720p	*/
	PQL_DTV_SD_P			,		/*3 SD Progressive :	 480p	*/
	PQL_DTV_SD_I			,		/*4 SD Interlace :	 480i	*/
	PQL_DTV_FHD_DVB			,		/*5 HD Progressive :	1080p	*/
	PQL_DTV_HD_I_DVB		,		/*6 HD Interlace : 	1080i	*/
	PQL_DTV_HD_P_DVB		,		/*7 HD Progressive :	 720p	*/
	PQL_DTV_SD_P_DVB		,		/*8 SD Progressive :	 576p	*/
	PQL_DTV_SD_I_DVB		,		/*9 SD Interlace : 	 576i	*/
	PQL_DTV_240I_50HZ		,		/*10	*/
	PQL_PICWIZARD			,		/*11	*/
	PQL_DTV_INDEX_MAX		,

}PQL_INPUT_DTV_T;

/**
 * Analog Source fmt type.
 *
 * @see .
*/
typedef enum {
/* PQL_INPUT_ANA		: Analog_RF/CVBS/S-video				*/
	PQL_ANA_RF				= 0,	/*0 Analog_RF + NTSC		*/
	PQL_ANA_RF_PAL			,		/*1 Analog_RF + PAL		*/
	PQL_ANA_RF_SECAM		,		/*2 Analog_RF + SECAM		*/
	PQL_ANA_RF_PAL60		,		/*3 Analog_RF + PAL60		*/
	PQL_ANA_RF_NTSC443		,		/*4 Analog_RF + NTSC443		*/
	PQL_ANA_RF_PAL_M		,		/*5 Analog_RF + PAL_M			*/
	PQL_ANA_RF_PAL_N		,		/*6 Analog_RF + PAL_N			*/
	PQL_ANA_RF_MAX			= PQL_ANA_RF_PAL_N,
	PQL_ANA_CVBS			= PQL_ANA_RF_PAL_N + 1,		/*7 Analog_CVBS + NTSC		*/
	PQL_ANA_CVBS_PAL		,		/*8 Analog_CVBS + PAL		*/
	PQL_ANA_CVBS_SECAM		,		/*9 Analog_CVBS + SECAM		*/
	PQL_ANA_CVBS_PAL60		,		/*10 Analog_CVBS + PAL60		*/
	PQL_ANA_CVBS_NTSC443	,		/*11 Analog_CVBS + NTSC443	*/
	PQL_ANA_CVBS_PAL_M		,		/*12 Analog_CVBS + PAL_M		*/
	PQL_ANA_CVBS_PAL_N		,		/*13 Analog_CVBS + PAL_N		*/
	PQL_ANA_CVBS_MAX		= PQL_ANA_CVBS_PAL_N - PQL_ANA_CVBS + 1,
	PQL_ANA_SVID			= PQL_ANA_CVBS_PAL_N+1,		/*14 Analog_SVID + NTSC		*/
	PQL_ANA_SVID_PAL		,		/*15 Analog_SVID + PAL		*/
	PQL_ANA_SVID_SECAM		,		/*16 Analog_SVID + SECAM		*/
	PQL_ANA_SVID_PAL60		,		/*17 Analog_SVID + PAL60		*/
	PQL_ANA_SVID_NTSC443	,		/*18 Analog_SVID + NTSC443	*/
	PQL_ANA_SVID_PAL_M		,		/*19 Analog_SVID + PAL_M	*/
	PQL_ANA_SVID_PAL_N		,		/*20 Analog_SVID + PAL_N	*/
	PQL_ANA_SVID_MAX		= PQL_ANA_SVID_PAL_N - PQL_ANA_SVID + 1,
	PQL_ANA_SCARTRGB		= PQL_ANA_SVID_PAL_N+1,		/*21 Analog_SVID + SCART_RGB	*/
	PQL_ANA_SCARTRGB_MAX	= PQL_ANA_SCARTRGB	,
	PQL_ANA_INDEX_MAX		= PQL_ANA_SCARTRGB+1,
}PQL_INPUT_ANA_T;

/**
 * Component fmt type.
 *
 * @see .
*/
typedef enum {
/* PQL_INPUT_COMP		: Component							*/
	PQL_COMP_FHD				= 0,	/*0 HD Interlace :  1080P	*/
	PQL_COMP_HD_I				,		/*1 HD Interlace :  1080i	*/
	PQL_COMP_HD_P				,		/*2 HD Progressive : 720p	*/
	PQL_COMP_SD_P				,		/*3 SD Progressive : 480p	*/
	PQL_COMP_SD_I				,		/*4 SD Interlace :   480i	*/
	PQL_COMP_FHD_PAL			,		/*5 HD Progressive :1080P	*/
	PQL_COMP_HD_I_PAL			,		/*6 HD Interlace :  1080i	*/
	PQL_COMP_HD_P_PAL			,		/*7 HD Progressive : 720p	*/
	PQL_COMP_SD_P_PAL			,		/*8 SD Progressive : 576p	*/
	PQL_COMP_SD_I_PAL			,		/*9 SD Interlace :   576i	*/
	PQL_COMP_INDEX_MAX		,
}PQL_INPUT_COMP_T;

/**
 * HDMI DVI fmt type.
 *
 * @see .
*/
typedef enum {
/* PQL_INPUT_HDMI	: HDMI_AV										*/
	PQL_HDMIAV_FHD			= 0,	/*0 HD Progressive : 1080P	*/
	PQL_HDMIAV_HD_I			,		/*1 HD Interlace : 1080i	*/
	PQL_HDMIAV_HD_P			,		/*2 HD Progressive : 720p	*/
	PQL_HDMIAV_SD_P			,		/*3 SD Progressive : 480p	*/
	PQL_HDMIAV_SD_I			,		/*4 SD Interlace : 480i		*/
	PQL_HDMIAV_FHD_PAL		,		/*5 HD Progressive : 1080P	*/
	PQL_HDMIAV_HD_I_PAL		,		/*6 HD Interlace : 1080i	*/
	PQL_HDMIAV_HD_P_PAL		,		/*7 HD Progressive : 720p	*/
	PQL_HDMIAV_SD_P_PAL		,		/*8 SD Progressive : 576p	*/
	PQL_HDMIAV_SD_I_PAL		,		/*9 SD Interlace : 576i		*/
	PQL_HDMIAV_MAX			= PQL_HDMIAV_SD_I_PAL + 1,		/*10 */
#ifdef INCLUDE_PQLDB_FIDX
	PQL_HDMIAV_RGB_FHD		= PQL_HDMIAV_SD_I_PAL + 1	,		/*0 HD Progressive : 1080P	*/
	PQL_HDMIAV_RGB_HD_I 		,		/*1 HD Interlace : 1080i	*/
	PQL_HDMIAV_RGB_HD_P 		,		/*2 HD Progressive : 720p	*/
	PQL_HDMIAV_RGB_SD_P 		,		/*3 SD Progressive : 480p	*/
	PQL_HDMIAV_RGB_SD_I 		,		/*4 SD Interlace : 480i 	*/
	PQL_HDMIAV_RGB_FHD_PAL		,		/*5 HD Progressive : 1080P	*/
	PQL_HDMIAV_RGB_HD_I_PAL 	,		/*6 HD Interlace : 1080i	*/
	PQL_HDMIAV_RGB_HD_P_PAL 	,		/*7 HD Progressive : 720p	*/
	PQL_HDMIAV_RGB_SD_P_PAL 	,		/*8 SD Progressive : 576p	*/
	PQL_HDMIAV_RGB_SD_I_PAL 	,		/*9 SD Interlace : 576i 	*/
	PQL_HDMIAV_RGB_MAX			= PQL_HDMIAV_RGB_SD_I_PAL - PQL_HDMIAV_RGB_FHD + 1,		/*10 */
#endif
/* PQL_INPUT_DVIPC	: DVI_PC											*/
#ifdef INCLUDE_PQLDB_FIDX
	PQL_DVIDTV_FHD 			= PQL_HDMIAV_RGB_SD_I_PAL + 1,		/*10 */
#else
	PQL_DVIDTV_FHD 			= PQL_HDMIAV_SD_I_PAL + 1,		/*10 */
#endif
	PQL_DVIDTV_HD_I			,		/*11 */
	PQL_DVIDTV_HD_P 		,		/*12 */
	PQL_DVIDTV_SD_P			,		/*13 */
	PQL_DVIDTV_SD_I			,		/*14 */
	PQL_DVIDTV_FHD_PAL		,		/*15 */
	PQL_DVIDTV_HD_I_PAL	 	,		/*16 */
	PQL_DVIDTV_HD_P_PAL		,		/*17 */
	PQL_DVIDTV_SD_P_PAL		,		/*18 */
	PQL_DVIDTV_SD_I_PAL	 	,		/*19 */
	PQL_DVIDTV_MAX			= PQL_DVIDTV_SD_I_PAL - PQL_DVIDTV_FHD + 1,		/*10 */
	PQL_DVIPC_VGA			= PQL_DVIDTV_SD_I_PAL + 1,		/*20 640x480			*/
	PQL_DVIPC_SVGA			,		/*21 800x600			*/
	PQL_DVIPC_XGA			,		/*22 1280x768			*/
	PQL_DVIPC_WXGA			,		/*23 1366x768			*/
	PQL_DVIPC_UXGA			,		/*24 1600x1200			*/
	PQL_DVIPC_FHD			,		/*25 */
	PQL_DVIPC_MAX			= PQL_DVIPC_FHD - PQL_DVIPC_VGA + 1,		/*6 */
	PQL_HDMI_INDEX_MAX		= PQL_DVIPC_FHD + 1,		/*26 */
}PQL_INPUT_HDMI_T;

#ifdef INCLUDE_PQLDB_FIDX
/**
 * VCS fmt type.
 *
 * @see .
*/
typedef enum {
/* PQL_INPUT_VCS	: VCS										*/
	PQL_VCS_VGA			= 0,	/*0 640x480			*/
	PQL_VCS_SVGA			,	/*1 800x600			*/
	PQL_VCS_XGA				,	/*2 1280x768			*/
	PQL_VCS_WXGA			,	/*3 1366x768			*/
	PQL_VCS_UXGA			,	/*4 1600x1200			*/
	PQL_VCS_FHD				,	/*5	*/
	PQL_VCS_INDEX_MAX		,
}PQL_INPUT_VCS_T;
#endif

/**
 * RGB PC fmt type.
 *
 * @see .
*/
typedef enum {
/* PQL_INPUT_RGBPC	: RGB_PC 										*/
	PQL_RGBPC_VGA			= 0,	/*0 640x480			*/
	PQL_RGBPC_SVGA			,		/*1 800x600			*/
	PQL_RGBPC_XGA			,		/*2 1280x768			*/
	PQL_RGBPC_WXGA			,		/*3 1366x768			*/
	PQL_RGBPC_UXGA			,		/*4 1600x1200			*/
	PQL_RGBPC_FHD			,		/*5	*/
	PQL_RGBPC_INDEX_MAX		,
}PQL_INPUT_RGBPC_T;

/**
 * DIVX fmt type.
 *
 * @see .
*/
typedef enum {
/* PQL_INPUT_DIVX	: Divx MOVIE (through USB, BT)					*/
	PQL_PHOTO_VGA			= 0,	/*0 640x480			*/
	PQL_PHOTO_SVGA			,		/*1 800x600			*/
	PQL_PHOTO_XGA			,		/*2 1280x768			*/
	PQL_PHOTO_WXGA			,		/*3 1366x768			*/
	PQL_PHOTO_UXGA			,		/*4 1600x1200			*/
	PQL_PHOTO_FHD			,		/*5	*/
	PQL_PHOTO_MAX			= PQL_PHOTO_FHD + 1,
	PQL_DIVX_VGA			= PQL_PHOTO_FHD + 1,	/*6 640x480			*/
	PQL_DIVX_SVGA			,		/*7 800x600			*/
	PQL_DIVX_XGA			,		/*8 1280x768			*/
	PQL_DIVX_WXGA			,		/*9 1366x768			*/
	PQL_DIVX_UXGA			,		/*10 1600x1200			*/
	PQL_DIVX_FHD			,		/*11	*/
	PQL_DIVX_MAX			= PQL_DIVX_FHD - PQL_DIVX_VGA + 1,
	PQL_DIVX_INDEX_MAX		= PQL_DIVX_FHD + 1,
}PQL_INPUT_DIVX_T;

/**
 * PVR fmt type.
 *
 * @see .
*/
typedef enum {
/* PQL_INPUT_PVRDTV	: PVR_Digital Source					*/
	PQL_PVRDTV_FHD			= 0,	/*0 HD Progressive : 1080P	*/
	PQL_PVRDTV_HD_I			,		/*1 HD Interlace : 1080i		*/
	PQL_PVRDTV_HD_P			,		/*2 HD Progressive : 720p	*/
	PQL_PVRDTV_SD_P			,		/*3 SD Progressive : 480p	*/
	PQL_PVRDTV_SD_I			,		/*4 SD Interlace : 480i		*/
	PQL_PVRDTV_FHD_PAL		,		/*5 etc				*/
	PQL_PVRDTV_HD_I_PAL		,		/*6 etc				*/
	PQL_PVRDTV_HD_P_PAL		,		/*7 etc				*/
	PQL_PVRDTV_SD_P_PAL		,		/*8 etc				*/
	PQL_PVRDTV_SD_I_PAL		,		/*9 etc				*/
	PQL_PVRDTV_MAX			= PQL_PVRDTV_SD_I_PAL - PQL_PVRDTV_FHD + 1,
/* PQL_INPUT_PVRANA1	:  PVR_Analog RF, CVBS					*/
	PQL_PVRANA_RF			= PQL_PVRDTV_SD_I_PAL + 1,		/*10 PVR_RF + NTSC		*/
	PQL_PVRANA_RF_PAL		,		/*11 PVR_RF + PAL		*/
	PQL_PVRANA_AV			,		/*12 PVR_AV + NTSC		*/
	PQL_PVRANA_AV_PAL		,		/*13 PVR_AV + PAL		*/
	PQL_PVRANA_SVID			,		/*14 PVR_SVID		*/
	PQL_PVRANA_SVID_PAL		,		/*15 PVR_SVID + PAL	*/
	PQL_PVRANA_SCARTRGB		,		/*16 PVR_CVBS + NTSC		*/
	PQL_PVRANA_MAX			= PQL_PVRANA_SCARTRGB - PQL_PVRANA_RF + 1,
	PQL_PVR_INDEX_MAX				= PQL_PVRANA_SCARTRGB + 1,
}PQL_INPUT_PVR_T;

/**
 * IPTV fmt type.
 *
 * @see .
*/
typedef enum {
/* PQL_INPUT_IPTV	:  IPTV Source							*/
	PQL_IPTV_FHD			= 0,	/*0 HD Interlace :  1080P	*/
	PQL_IPTV_HD_I			,		/*1 HD Interlace :  1080i	*/
	PQL_IPTV_HD_P			,		/*2 HD Progressive : 720p	*/
	PQL_IPTV_SD_P			,		/*3 SD Progressive : 480p	*/
	PQL_IPTV_SD_I			,		/*4 SD Interlace :   480i	*/
	PQL_IPTV_FHD_PAL		,		/*5 HD Progressive :1080P	*/
	PQL_IPTV_HD_I_PAL		,		/*6 HD Interlace :  1080i	*/
	PQL_IPTV_HD_P_PAL		,		/*7 HD Progressive : 720p	*/
	PQL_IPTV_SD_P_PAL		,		/*8 SD Progressive : 576p	*/
	PQL_IPTV_SD_I_PAL		,		/*9 SD Interlace :   576i	*/
	PQL_IPTV_INDEX_MAX			,
}PQL_INPUT_IPTV_T;

 /**
 * Define width and height for In/Out Size.
 *
 * @see
 */
typedef struct PQL_SIZE{
	UINT16	inWidth;				/*input width*/
	UINT16	inHeight;				/*input height*/
	UINT16	outWidth;				/*output width*/
	UINT16	outHeight;				/*output height*/
}PQL_SIZE_T;

/**
* pql status.
*
* @see
*/
typedef struct PQL_STATUS{
	BOOLEAN					pqlPath[PQL_PATH_MAXN];					/* path selection */
	PQL_INPUT_SRC_FMT_T		pqlSrcFmt[PQL_PATH_MAXN];				/* src and fmt */
	PQL_SIZE_T				pqlSize[PQL_PATH_MAXN];					/* size*/
}PQL_STATUS_T;


/**
*	Coefficient ID 선택위함.
*	Coefficient ID 추가시 동시에 추가.
*
* @see
*/
typedef enum
{
	PQL_COEFF_PC			=	0x00001001,
	//PQL_COEFF_PC_KIND		=	0x00001002,
	//PQL_COEFF_PC_CE			=	0x00001003,	//20091103, sjyoum, not used!!
	//PQL_COEFF_PC_EYEQ		=	0x00001004,		//20091103, sjyoum, not used!!
	PQL_COEFF_PC_UIDIMMING_CURVE =	0x00001005,
	PQL_COEFF_PC_EYECARE 	=	0x00001006,	//KWON_1105

	PQL_COEFF_DC2P_DYNCONT =	0x00002001,
	PQL_COEFF_DC2P_APL	    =	0x00002002,
	PQL_COEFF_DC2P_BACKCTRL	=	0x00002003,
	PQL_COEFF_DC2P_CGAIN 	=	0x00002004,		//KWON_1105
	PQL_COEFF_DC2P_SKIN 	=	0x00002005,		//KWON_1105

	/* White Balance Gain/Offset Correction = 1EA*/
	PQL_COEFF_WB_OFFSET		=	0x00005201,
} PQL_COEFF_ID_T;

/**
*   cPQ_ModelName -> 화질 설정 DB 모델명 정의 : In-Start Menu에서 Display.
*       ModelName은 Max 15글자로 제한.
*   PQ Version -> 화질 설정 버젼 + 화질라이브러리 소프트웨어 버젼
*       ex) 01.60.10 => cPQ_DbVersion : 1.6, cPQ_DrvVersion : 0.10
*   cPQ_DbVersion -> 화질 설정 Database version
*       0.x : DV, 1.x : PV, 2.x : PQ, 3.x ~ : MP
*   cPQ_DrvVersion -> 화질 Driver version
*       0.x : DV, 1.x : PV, 2.x : PQ, 3.x ~ : MP
*       X.xx : X - major change, xx - minor change
*       만약 major change가 10회 이상일경우 맨앞 숫자를 사용한다.
*       따라서, 총 99번의 major change를 표시할 수 있다.(숫자단위)
*
* @see
*/
typedef struct {

    char    cPQ_ModelName[15];      /**< model name*/
    char    cPQ_DbVersion[4];           /**< DB ver.*/
    char    cPQ_DrvVersion[4];      /**< DVR ver.*/

} PQL_VERSION_T;

/**
*	Default Data of Global Variables when White Balance Adjustment.
*	White Balance 조정 시 HD2.4 Video 내부 패턴을 이용한다.
*	이때 아래의 변수 설정을 통해
*	Gain 조정을 위해서는 High Luminance Level을 설정하고,
*	Offset 조정을 위해서는 Low Luminance Level을 설정한다.
*	HD2.4 Contrast Value = 0~12~255 (0~1.0~1.99)
*
* @see
*/
typedef struct {

	UINT8	gLuma_WBGainAdj;
	UINT8	gLuma_WBOffsetAdj;

} PQL_LUMA_WB_T;

/**
 * Getting coefficient ID and Max value from pqldb, that is not dependent on the chip specific algorithm.
 *
 * @see .
 */
typedef struct PQL_COEFF_MEMBER {

    UINT8    uPqlCoeffID;      /**< model name*/
    UINT8    uPqlCoeffIDMax;          /**< DB ver.*/

} PQL_COEFF_MEMBER_T;

/**
 * Getting coefficient ID and Max value from pqldb  for new Index structure.
 *
 * @see .
 */
typedef struct PQL_INDEX_MEMBER {
    UINT8   uPqlMainCoeffID;	/* Main Path Index Id				*/

    BOOLEAN bValidSubCoeffID;	/* valid status for Sub Index			*/
    UINT8   uPqlSubCoeffID;		/* Sub Path Index Id				*/

    BOOLEAN bValidBECoeffID;	/* valid status for BackEnd Chip Index*/
    UINT8   uPqlBECoeffID;		/* BackEnd Chip Index Id			*/

	UINT8	uPqlCoeffIDMax;		/* Max Id for Main					*/
} PQL_INDEX_MEMBER_T;


#define	MAX_CHIP_BIN_SIZE	64
#define	MAX_CHIP_NUM_TRANSCURVE		16

#define DC2_SUPPORT_IN_MW	0
#define DC2_SUPPORT_IN_DEVICE	1

/**
*	pql DC2 parameter.
*
* @see
*/
typedef struct PQL_DC2P_RAWDATA_INFO {
	UINT32		nSupportDataType;
	UINT32		nBinNum;
	SINT32		nByPassCurveTable[MAX_CHIP_NUM_TRANSCURVE];		//KWON_0716
	SINT32		nBinStep;
	UINT32		uCountBins[MAX_CHIP_BIN_SIZE];
	UINT32		uAPL;
	SINT32		nMin;
	SINT32		nMax;
	SINT32		nPeakLow;
	SINT32		nPeakHigh;
	SINT32		nSupportLayer;
	UINT32		uCountSkin;		//KWON_0909 add for skin pixel count
} PQL_DC2P_RAWDATA_INFO_T;

/**
* PQL data for control film(Cinema).
*/
typedef struct PQL_FILM_CTRL
{
	BOOLEAN bGameMode;
	BOOLEAN bFilmMode;
}PQL_FILM_CTRL_T;

/**
 * < Expert Control > Color Standard Setting Menu member.
 */
typedef enum {
	PQL_EXP_CS_SD,									/**< SD Color Standard  */
    PQL_EXP_CS_HD,									/**< HD Color Standard  */
    PQL_EXP_XVYCC_SD,								/**< xvYCC SD Color Standard  */
    PQL_EXP_XVYCC_HD,								/**< xvYCC HD Color Standard  */
} PQL_COLORSTANDARD_T;

/**
* PQL data for color gamut
*/
typedef struct PQL_COLORGAMUT_CTRL
{
	PQL_PATH_T pqlPath;
	//pqlstatus
	PQL_STATUS_T pqlStatus;
	UINT8 uxvYCCStatus;
	UINT8 uQuantRangeStatus;
	UINT8 uxvYCCCoeffId;
	UINT8 uUixvYCC;
	UINT8 uUiGamma;	// [jaehan.kim] 2010/01/29		PDP only!  THX 화질 요구 사항 : Gamma 변경시도 Gamut Standard 의 CSC 변경
	//xvYCC meta data
	//xvYCC GBD Color Space
	UINT8 uColorGamutCoeffId;
	UINT8 uColorGamut;

	//20091026, sjyoum, gamut셋팅시 xvYcc 및 colorstd를 동시에 셋팅하기 위해 추가함.
	PQL_COLORSTANDARD_T uXvYccColorStd;

	PQL_BLACK_T         uUiBlackLevel;//20101013 jaeyoung.cho USB BLACK LEVEL지원을 위해 추가함(LOW/HIGH/AUTO)
}PQL_COLORGAMUT_CTRL_T;


/**
* PQL data for color standard
*/
typedef struct PQL_CSC_LEVEL_CTRL
{
	PQL_PATH_T pqlPath;
	//pqlstatus
	PQL_STATUS_T pqlStatus;
	//for colorStandard
	PQL_COLORSTANDARD_T uUI_ColorStandard;			/** UI Color Standard Selection */
	UINT8 uInputCSCCoeffSelection;		/** Input CSC Coefficient Selection */
	//for blackLevel
	UINT8 uUI_BlackLevel;				/** UI Color Standard Selection */
	UINT8 uBlackLevelInput;				/** Current Input */
	UINT8 uBlackLevelHDMISouceType;				/** Current Input */
	BOOLEAN bIsTHXMode;		//[vivakjh] 2009/03/12	PDP THX에서 HDMI Black Level Low 일 경우 Bypass를 위해 삽입함.
}PQL_CSC_LEVEL_CTRL_T;

/**
 * HDMI : Color Space Domain
 *
 * @see
 */
typedef enum
{
	PQL_HDMI_RGB = 0,
	PQL_HDMI_YCBCR,
	PQL_HDMI_XVYCC,
	PQL_DVI_RGB,
	PQL_HDMI_UNKNOWN,
}PQL_HDMI_COLOR_DOMAIN_T;

/**
 * HDMI : Quantization Range
 *
 * @see
 */
typedef enum
{
	PQL_HDMI_QUANT_RANGE_DEFAULT = 0,
	PQL_HDMI_QUANT_RANGE_LIMITEDRANGE,
	PQL_HDMI_QUANT_RANGE_FULLRANGE,
	PQL_HDMI_QUANT_RANGE_RESERVED,
}PQL_HDMI_QUANTIZATION_RANGE_T;


/**
 * PQL_CM_REGION_T
 */
typedef enum {
	PQL_CM_MGT	= 0,
	PQL_CM_RED,
	PQL_CM_YEL,
	PQL_CM_GRN,
	PQL_CM_CYN,
	PQL_CM_BLU,
	PQL_CM_FT,
	PQL_CM_BS,
	PQL_CM_ICC,
	PQL_CM_IHC
}PQL_CM_REGIONID_T;


/**
 * CM : Expert Color Mangement struct.
 *
 * @see
 */
typedef struct PQL_CM_EXPCOLOR_PARAM{
	PQL_PATH_T pqlPath;
	//pqlstatus
	PQL_STATUS_T pqlStatus;
	PQL_CM_REGIONID_T	pqlCmRegionID;			/**< 설정하고자 하는 영역 ID	*/
	//UINT8	pqlCmCoeffID;					/**< DB Table Index	*/
	SINT8	uiExpSatGainValue;				/**< UI Sat Gain Value */
	SINT8	uiExpHueGainValue;				/**< UI Hue Gain Value */
}PQL_CM_EXPCOLOR_PARAM_T;

/**
 * CM : Fresh Color struct.
 *
 * @see
 */
typedef struct PQL_CM_FRESHCOLOR_PARAM{
	PQL_PATH_T pqlPath;
	//pqlstatus
	PQL_STATUS_T pqlStatus;
	UINT8	uiFreshColorValue;				/**< UI Sat Gain Value */
}PQL_CM_FRESHCOLOR_PARAM_T;

/**
 * CM : Skin Color struct.
 *
 * @see
 */
typedef struct PQL_CM_SKINCOLOR_PARAM{
	PQL_PATH_T pqlPath;
	//pqlstatus
	PQL_STATUS_T *pstPqlStatus;
	SINT8	uiSkinColorValue;				/**< UI Skin Color Value */
	UINT8	uiFreshColorValue;				/**< UI Sat Gain Value */
}PQL_CM_SKINCOLOR_PARAM_T;

/**
 * CM : Fresh Color struct.
 *
 * @see
 */
typedef struct PQL_CM_DYNAMICWHITE_PARAM{
	PQL_PATH_T pqlPath;
	//pqlstatus
	PQL_STATUS_T *pstPqlStatus;
	UINT8	uiDynamicWhiteValue;				/**< UI Dynamic White Value */
}PQL_CM_DYNAMICWHITE_PARAM_T;

/**
 * DC2P : Dynamic Contrast struct.
 *
 * @see
 */
typedef struct PQL_DC2P_CTRL{
	PQL_PATH_T pqlPath;
	PQL_STATUS_T stPqlStatus;
	// coeff id
	UINT8	uDc2pDynContCoeffId;	/**< Coeff Index(PQL_COEFF_DC2P_DYNCONT) */
	UINT8	uDc2pAplInfoCeffId;		/**< Coeff Index(PQL_COEFF_DC2P_APL) */
	UINT8	uDc2pCgainCoeffId;		/**< Coeff Index(PQL_COEFF_DC2P_CGAIN) */
	UINT8	uDc2pSkinCoeffId;		/**< Coeff Index(PQL_COEFF_DC2P_SKIN) */
	// ui value
	UINT8	uUiFreshContrast;	/**< UI Fresh Contrast Value */
	BOOLEAN	bStoreMode;				/**< Store mode */
}PQL_DC2P_CTRL_T;

/**
* PQL data for anlog Video Decoder.
*/
typedef struct PQL_ANDECODER_CTRL
{
	PQL_PATH_T pqlPath;
	//pqlstatus
	PQL_STATUS_T pqlStatus;
}PQL_ANDECODER_CTRL_T;

/**
* PQL data for control BLE/WLE
*/
typedef struct PQL_BLEWLE_CTRL
{
	PQL_PATH_T pqlPath;
	//pqlstatus
	PQL_STATUS_T pqlStatus;
	BOOLEAN bBlewleMode;
}PQL_BLEWLE_CTRL_T;		// [vivakjh]  2008/12/29	PDP BLE/WLE(Black/White Level Extension)대응.

/**
 * NR : NR Setting Info Structure.
 *
 * @see
 */
typedef struct PQL_NR_SETTING_INFO
{
	UINT8 uIsProgressive;
	BOOLEAN bNROnOffCtrl;
	BOOLEAN bIsDTVtiming;
	BOOLEAN bIsPCtiming;
} PQL_NR_SETTING_INFO_T;


//by KWON_0618
/**
 * UI Mpeg NR control struct.
 *
 * @see
 */
typedef struct PQL_NR_MPEGNR_PARAM{
	PQL_PATH_T pqlPath;
	//pqlstatus
	PQL_STATUS_T pstPqlStatus;
	PQL_NR_LEVEL_T	uMpegNRValue;				/**< UI MPeg NR Value */
}PQL_NR_MPEGNR_PARAM_T;

/**
 * UI NR control struct.
 *
 * @see
 */
typedef struct PQL_NR_CTRL_PARAM{
	PQL_PATH_T pqlPath;
	PQL_STATUS_T pstPqlStatus;
	PQL_NR_LEVEL_T	uNRValue;				/**< UI NR Value */
	PQL_NR_SETTING_INFO_T stSettingInfo;
}PQL_NR_CTRL_PARAM_T;


/**
 * SR control struct.
 *
 * @see
 */
typedef struct PQL_SR_PARAM{
	PQL_PATH_T pqlPath;
	PQL_STATUS_T stPqlStatus;
	PQL_SR_LEVEL_T	uSRValue;		/**< UI SR Value */
}PQL_SR_PARAM_T;


/**
 * PQL_INDEXTBL_FLAG.
 */
typedef enum {
	PQL_IDXAR_ATTR_NA		= 0x01,	//Not Applicable
	PQL_IDXAR_ATTR_EACH_RES	= 0x02,	//Using Each Resolution
	PQL_IDXAR_ATTR_ALL_THE_SAME	= 0x04,	//Using ALL The Same Index
	PQL_IDXAR_ATTR_ALL			= 0x07,	//Using ALL
}PQL_IDXAR_ATTR_T;


/**
 * PQL_INDEXTBL_COMPONENT_T.
 */
typedef struct PQL_INDEXTBL_COMPONENT {
	UINT8 uPqlIdxArAttr;
	UINT8 uPqlSizeOfIdxArray;
	UINT8 *puPqlIdxArray;
#ifndef _EMUL_WIN
} __attribute__((packed)) PQL_INDEXTBL_COMPONENT_T;
#else
}PQL_INDEXTBL_COMPONENT_T;;
#endif
/**
 * PQL_INDEXTBL_T.
 */
typedef struct PQL_INDEXTBL {
	PQL_INDEXTBL_COMPONENT_T *pPqlSrcInput[PQL_SOURCE_MAX];
}PQL_INDEXTBL_T;

/**
 * PQL_FUNC_IDXTBL_DET_FACTOR_T.
 */
typedef struct PQL_FUNC_IDXTBL_DET_FACTOR {
	UINT8	uPql3DMode;
	UINT8	uPql3DInputFormat;

	UINT8	uPqlShopMode;
	UINT8	uPqlMainOrSub;
	UINT8	uPqlWirelessRdy;

#ifndef INCLUDE_PQLDB_FIDX
	//위 요소로 생성되는 결과
	UINT8	uPqlFuncIdxTblIndex;
#endif
	//
	UINT8	uPqlInputType;
	UINT8	uPqlResIndex;

}PQL_FUNC_IDXTBL_DET_FACTOR_T;

#ifdef INCLUDE_PQLDB_FIDX
/**
 * PQL_DB_IDXTBL_DET_FACTOR.
 */
typedef struct PQL_DB_IDXTBL_DET_FACTOR {
	PQL_FUNC_IDXTBL_DET_FACTOR_T *pstCurInputStatus;
	UINT32 pqlIdCoeff;
	//신규 추가 요소
	//common
	UINT8	nPqlBlkIdx;
	UINT8	nPqlBlkItemsIdx;
	UINT8	nPqlInputItemsIdx;
	//for Store Option
	UINT8	nStoreOptionIdx;
	//for 3D
	UINT8	nPqlTridIdx;

	//final
	UINT8	nPqlResIndex;
	PQL_INDEX_MEMBER_T	*pstPqlCoeffIdIdx;
}PQL_DB_IDXTBL_DET_FACTOR_T;
#endif


/**
 * Match setting value to chip ( or driver )'s characteristic.
*/
typedef struct {

	float	fRGBGainVal[3];		/* 0.6160 ~ 1.0000 ~ 1.1260 : step = 0.002	*/
	float	dwRGBGain[3];		/* 0.6160 ~ 1.0000 ~ 1.1260				*/

	float	fRGBOffsetVal[3];		/* -64 ~ 0 ~ 63 : step = 1bit				*/
	float	dwRGBOffset[3];		/* -0.1600 ~ 0.0000 ~ 0.1575				*/

} PQL_ADC_ADAPT_T;

/**
 * Match OSD value to setting value.
*/
typedef struct {

	float	fRGBGainOSD[3];		/* 0 ~ 192 ~ 255 : default value is 192.	*/
	float	fRGBGainVal[3];		/* 0.616 ~ 1.000 ~ 1.126 : step = 0.002	*/

	float	fRGBOffsetOSD[3];	/* 0 ~ 64 ~ 127 : default value is 64.		*/
	float	fRGBOffsetVal[3];		/* -64 ~ 0 ~ 63 : step = 1bit			*/

} PQL_ADC_CTRL_T;


#ifdef INCLUDE_PQL_GP4_RESTRUCT

/**
 * color temperature information
 * this will be used for getting ct_csc's coefficients
*/
typedef struct PQL_GAIN_OFFSET
{
	float fGainR;
	float fGainG;
	float fGainB;
	float fOffsetR;
	float fOffsetG;
	float fOffsetB;
} PQL_GAIN_OFFSET_T;
#endif

typedef enum {
	PQ_UID_ELEMENT_MISMATCH		= 0,
	PQ_UID_ELEMENT_GOOD_MATCH	= 1,
	PQ_UID_ELEMENT_BEST_MATCH	= 2
} PQ_Tbl_ELEMENT_MATCH_T;

/*******************************************************************************
	Extern 전역변수와 함수 prototype 선언
	(Extern Variables & Function Prototype Declarations)
*******************************************************************************/

/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/
#ifdef INCLUDE_PQL_GP4_RESTRUCT

/***************************************************************************/
/**************** 	LG DDI 		********************************************/
/***************************************************************************/

BOOLEAN DDI_PQL_GetSceneChangeStatus(void);

// by hwangbos 20080925
// 함수 일치율을 위해서 임시로 처리한 루틴 향후 정리해야함
void DDI_PQL_GetDftInitWBData(PQL_WB_DATA_T *pAdjWBdata);
PQL_WB_DATA_T DDI_PQL_GetDftWBData(void);
UINT8 DDI_PQL_GetDftSubBrightness(void);

/* Sharpness */
DTV_STATUS_T DDI_PQL_SetSharpByUserControl(PQL_SHARP_UI_VALUE_T	sPqUiVal, PQL_STATUS_T pqlStatus);
/* NR */
DTV_STATUS_T DDI_PQL_SetNRByUserControl(PQL_STATUS_T pqlStatus, PQL_NR_LEVEL_T pqlNrLevel);
//[090908-clearcut] DCNR
DTV_STATUS_T DDI_PQL_SetNRByDcnr(PQL_DC2P_RAWDATA_INFO_T *pHistoInfo, SINT32 *pLumaCurve);
/* xvYCC */
UINT8 DDI_PQL_GetxvYCCStatus(void);
UINT8 DDI_PQL_GetQuantRangeStatus(void);
/* Timing Change */
void DDI_PQL_SetForTimmingChange(PQL_STATUS_T pqlStatus);

/* PC */
void DDI_PQL_GetAdjSubBrightCont( PQL_SUB_CONTBRI_T *pAdjSubBriCont );
void DDI_PQL_SetAdjSubBrightCont( PQL_SUB_CONTBRI_T adjSubBriCont );

/* Video DDI */
PQL_AVDECODER_VIDEO_MODE_T DDI_PQL_GetColorSystem(void);
DTV_STATUS_T DDI_PQL_GetHDMIColorDomain(PQL_HDMI_COLOR_DOMAIN_T *pqlHDMIColorDomain);
DTV_STATUS_T DDI_PQL_GetHDMIQuantRange(PQL_HDMI_QUANTIZATION_RANGE_T *pPqlHDMIQuantRange);

/* check auto wb tick time */
UINT8 DDI_PQL_CheckAutoWbTickTime(void);

/* Set Test Pattern Status */
void DDI_PQL_SetTestPatternStatus(BOOLEAN bOnOff);
DTV_STATUS_T DDI_PQL_SetColorTemp(PQL_COLOR_TEMP_T cscPcCtData);
DTV_STATUS_T DDI_PQL_SetFactoryStatus(BOOLEAN bFactoryStatus);

/* Eye Q Sensor Gain */
PQ_Tbl_ELEMENT_MATCH_T DDI_PQL_CheckUidElementMatch( UINT32 sourceUid, UINT32 targetUid, UINT32 mask, UINT32 pos);
UINT32 DDI_PQL_GetPQTblIndex(UINT32 pqUid);
UINT32 DDI_PQL_GetEyeQSensorGain(UINT32 uToolType, UINT32 uModuleInch);
UINT16 DDI_PQL_GetEyeQTblId(UINT32 idx);
UINT32 DDI_PQL_GetEyeQTblGain(UINT32 idx);
UINT32 DDI_PQL_GetEyeQTblSize(void);

#ifdef INCLUDE_WIRELESS_READY
void DDI_PQL_SetWirelessInputConnectStatus(BOOLEAN bSet);

void DDI_PQL_SetWirelessInputHDMIColorDomain(PQL_HDMI_COLOR_DOMAIN_T pqlHDMIColorDomain);
void DDI_PQL_SetWirelessInputColorSystem(PQL_AVDECODER_VIDEO_MODE_T pqlMode);
#endif


/***************************************************************************/
/**************** 	DB DDI	(defined in pql_ddi_db.c)	********************/
/***************************************************************************/

void DDI_PQL_DB_InitDB(PQL_CONFIG_T tempPql);
void DDI_PQL_DB_InitDBIndex(PQL_CONFIG_T tempPql);
void DDI_PQL_DB_GetCoeffParam(UINT32 pqlIdCoeff, PQL_INPUT_SRC_FMT_T pqlSrcFmt, PQL_INDEX_MEMBER_T *pPqlIndexMember);
const UINT8 * DDI_PQL_DB_GetDbString(void);

UINT8 DDI_PQL_DB_GetDBEyeQOSDValue( PQL_EYEQ_DB_MEMBER_T memberID, UINT8 index, UINT16 uEyeQStep);
SINT32 DDI_PQL_DB_GetDBDimmingCtrl(PQL_DIMMING_DB_MEMBER_T  memberID, UINT8 index, UINT8 uCurveNum);
void DDI_PQL_DB_GetDBEyecareDimmingCtrl(UINT8 index, PQL_EYECARE_DIMMING_DB_T *pDataBase);

DTV_STATUS_T DDI_PQL_DB_GetBrightContFor10pWB( PQL_PC_UI_VALUE_T pcUiVal, UINT8 pcId, float *pfContrast, float *pfBrightness );
DTV_STATUS_T DDI_PQL_DB_GetCorrectValOfWBGainOffset(UINT8 index, PQL_WB_CORRECT_DATA_T *pWBCorrectVal);

void DDI_PQL_DB_GetDBDC2PDynamicContrastCtrl(UINT8 type, UINT8 index, PQL_DC2P_DYN_CONTRAST_DB_T *pDataBase);
void DDI_PQL_DB_GetDBDC2PApl(UINT8 type, UINT8 index, PQL_DC2P_APL_DB_T *pDataBase);
void DDI_PQL_DB_GetDBDC2PCGain(UINT8 type, UINT8 index, PQL_DC2P_CGAIN_DB_T *pDataBase);
void DDI_PQL_DB_GetDBDC2PSkin(UINT8 type, UINT8 index, PQL_DC2P_SKIN_DB_T *pDataBase);


/***************************************************************************/
/**************** 	SOC DDI (defined in pql_soc.c)	************************/
/***************************************************************************/

/* Picture Control */
DTV_STATUS_T DDI_PQL_SOC_SetPictureControl(PQL_PC_UI_VALUE_T sPcUiVal, UINT8  sPqCoeffId, PQL_PATH_T pqlPath);
DTV_STATUS_T DDI_PQL_SOC_SetColorTemp(PQL_COLOR_TEMP_T cscPcCtData);

/* Sharpness */
DTV_STATUS_T DDI_PQL_SOC_SetSuperResParam(PQL_SR_PARAM_T *pstSRParam);
DTV_STATUS_T DDI_PQL_SOC_SetSharpByUserParam(PQL_SHARP_UI_VALUE_T sPqUiVal, PQL_STATUS_T pqlStatus,UINT8 *pShCoeffId);
DTV_STATUS_T DDI_PQL_SOC_SetSharpSystemParam(PQL_STATUS_T pqlStatus, PQL_PATH_T pqlPath);
DTV_STATUS_T DDI_PQL_SOC_SetSharpByEdgeEnhance(UINT8 nUiValue, PQL_PATH_T pqlPath, PQL_STATUS_T pqlStatus);

/* DC */
DTV_STATUS_T DDI_PQL_SOC_Init(void);
DTV_STATUS_T DDI_PQL_SOC_SetDefaultBySystem(void);
DTV_STATUS_T DDI_PQL_SOC_GetDCChipDataTypeInfo(UINT32 *pChipDataType);
DTV_STATUS_T DDI_PQL_SOC_GetDCChipRawData(PQL_DC2P_RAWDATA_INFO_T *pHistoInfo);
DTV_STATUS_T DDI_PQL_SOC_SetDCFreshContrast(PQL_DC2P_CTRL_T *pCtrlInfo);
DTV_STATUS_T DDI_PQL_SOC_SetDCFreshContrastLUT(SINT32	*pAccLUT);
DTV_STATUS_T DDI_PQL_SOC_SetColorGain(PQL_PC_UI_VALUE_T sPcUiVal, UINT8  sPqCoeffId, PQL_PATH_T pqlPath, SINT32 nColorGain);
DTV_STATUS_T DDI_PQL_SOC_SetAdaptiveCGain(SINT32 *pCGainParam);	//added by request of cho04

/* Dynamic Color */
DTV_STATUS_T DDI_PQL_SOC_InitFreshColorParam(PQL_CM_FRESHCOLOR_PARAM_T *pPqlFreshColorParam);
DTV_STATUS_T DDI_PQL_SOC_SetFreshColor(PQL_CM_FRESHCOLOR_PARAM_T pqlFreshColorParam);
DTV_STATUS_T DDI_PQL_SOC_SetSkinColor(PQL_CM_SKINCOLOR_PARAM_T *pstSkinColor);
DTV_STATUS_T DDI_PQL_SOC_SetDynamicWhite(PQL_CM_DYNAMICWHITE_PARAM_T *pstDynamicWhite);
DTV_STATUS_T DDI_PQL_SOC_SetDCFreshContrastSkinLUT(SINT32 *pAccLUT);
DTV_STATUS_T DDI_PQL_SOC_SetLevelAdj(PQL_CM_LEVEL_ADJ_T pqlLevelParam);

/* Noise Reduction */
DTV_STATUS_T DDI_PQL_SOC_SetNRByUserParam(PQL_NR_CTRL_PARAM_T *pstNrCtrlParam);
DTV_STATUS_T DDI_PQL_SOC_SetNRSystemParam(PQL_STATUS_T pqlStatus, PQL_PATH_T pqlPath);
DTV_STATUS_T DDI_PQL_SOC_SetMPEGNRParam(PQL_NR_MPEGNR_PARAM_T *pstMpegNRParam);	//KWON_0618 Parameter change
DTV_STATUS_T DDI_PQL_SOC_SetNRByDcnrParam(UINT8 *pDcnrParam);

/* Deinterlace */
DTV_STATUS_T DDI_PQL_SOC_SetDeinterParam(UINT8 *pMDCoeffId, PQL_PATH_T pqlPath, PQL_STATUS_T pqlStatus);
DTV_STATUS_T DDI_PQL_SOC_SetFilmMode(PQL_FILM_CTRL_T *pCtrlData, PQL_STATUS_T pqlStatus, PQL_PATH_T  pqlPath);

/* Analog Decoder */
DTV_STATUS_T DDI_PQL_SOC_CtrlDecoderParam(PQL_ANDECODER_CTRL_T *pPqlAnalogDecoderCtrl);

/* Blacklevel */
DTV_STATUS_T DDI_PQL_SOC_SetBlackLevel(PQL_CSC_LEVEL_CTRL_T *pstUserBlackLevelCtrl);

/* Gamma */
DTV_STATUS_T DDI_PQL_SOC_SetGamma(PQL_PATH_T pqlPath, UINT32 *pRedGammaTable, UINT32 *pGreenGammaTable, UINT32 *pBlueGammaTable);

/* Color Gamut */
DTV_STATUS_T DDI_PQL_SOC_SetColorGamut(PQL_COLORGAMUT_CTRL_T *pqlColorGamut);
DTV_STATUS_T DDI_PQL_SOC_SetxvYCCData(PQL_COLORGAMUT_CTRL_T *pqlColorGamut);

/* Color Filter */
DTV_STATUS_T DDI_PQL_SOC_SetColorFilter(UINT8 uSelectColorFilter);

/* Color Manage */
DTV_STATUS_T DDI_PQL_SOC_InitExpColorParam(PQL_CM_EXPCOLOR_PARAM_T *pPqlExpColorParam);
DTV_STATUS_T DDI_PQL_SOC_SetExpColorParam(UINT8 pqlCMCtrl, PQL_CM_EXPCOLOR_PARAM_T pqlExpColorParam);

/* Demo Mode */
DTV_STATUS_T DDI_PQL_SOC_SetSplitDemo( BOOLEAN bOnOff);

#if(PLATFORM_TYPE == BCM_PLATFORM)
/* BCM Only */
DTV_STATUS_T	DDI_PQL_SOC_BCM_InitADC(PQL_CSC_LEVEL_CTRL_T *pstUserBlackLevelCtrl, BOOLEAN bAdjMode);
DTV_STATUS_T	DDI_PQL_SOC_BCM_AdjustADC(UINT16 *pADCOffset, UINT16 *pADCGain, PQL_CSC_LEVEL_CTRL_T *pstUserBlackLevelCtrl, BOOLEAN bAdjMode);
void DDI_PQL_SOC_BCM_DisableNoiseReduction(void); // for ADC Calibration
DTV_STATUS_T DDI_PQL_SOC_BCM_SetUpdateManualMode(BOOLEAN bOnOff);
DTV_STATUS_T DDI_PQL_SOC_BCM_RestoreBlockMode(void);	//KWON_0916 for NR off 해제 문제
void DDI_PQL_SOC_BCM_DisplayIREPattern(UINT8 patternType, UINT8 patternParam, PQL_BLACK_SOURCE_T blackSrc);
DTV_STATUS_T DDI_PQL_SOC_BCM_SetGameMode(PQL_FILM_CTRL_T *pCtrlData);
#endif


#if(PLATFORM_TYPE == MSTAR_PLATFORM)
/* MStar Only */
DTV_STATUS_T DDI_PQL_SOC_MSTAR_SetColorStandard(PQL_CSC_LEVEL_CTRL_T *pstUserColorStandardCtrl);
DTV_STATUS_T DDI_PQL_SOC_MSTAR_SetColorGain(PQL_PC_UI_VALUE_T sPcUiVal, UINT8 sPqCoeffId, PQL_PATH_T pqlPath, SINT32 nColorGain);
DTV_STATUS_T DDI_PQL_SOC_MSTAR_SetAdaptiveCGain(SINT32 *pCGainParam);
void DDI_PQL_SOC_MSTAR_SetCSCOffsetEnable(BOOLEAN bEnable);
DTV_STATUS_T DDI_PQL_SOC_MSTAR_SetYUV2RGBMatrix(PQL_COLORSTANDARD_T outputCSC);
DTV_STATUS_T DDI_PQL_SOC_MSTAR_SetBlackLevelBypass(void);
#endif


/* XD Plasma PDP Only */
// [ vivakjh] 2010/08/13 XD Plasma 2.0 Spec  삭제함. DTV_STATUS_T DDI_PQL_SetXDPCC(UINT8 nUiValue, PQL_PATH_T pqlPath, PQL_STATUS_T pqlStatus);			//[vivakjh] 2009/02/23  PDP XD Plasma 대응.
DTV_STATUS_T DDI_PQL_SetXDPFreshWhite(UINT8 nUiValue, PQL_PATH_T pqlPath, PQL_STATUS_T pqlStatus);	//[vivakjh] 2009/02/23  PDP XD Plasma 대응.
DTV_STATUS_T DDI_PQL_SetXDPColor(UINT8 nUiValue, PQL_PATH_T pqlPath, PQL_STATUS_T pqlStatus);			//[vivakjh] 2009/09/19  PDP XD Plasma 3.0 대응.
DTV_STATUS_T DDI_PQL_SetXDPSharpness(UINT8 nUiValue, PQL_PATH_T pqlPath, PQL_STATUS_T pqlStatus);		//[vivakjh] 2009/02/23  PDP XD Plasma 대응.
void DDI_PQL_ReloadXDPlasmaData(void);
DTV_STATUS_T DDI_PQL_SetFreshWhiteData(UINT8 uFreshColorUIVal);


#else
/***************************************************************************/
/**************** 	LG DDI 		********************************************/
/***************************************************************************/
/* DB    */
void DDI_PQL_InitDB(PQL_CONFIG_T tempPql);
void DDI_PQL_InitDBIndex(PQL_CONFIG_T tempPql);

/* ADC	*/
DTV_STATUS_T	DDI_PQL_InitADC(PQL_CSC_LEVEL_CTRL_T *pstUserBlackLevelCtrl, BOOLEAN bAdjMode);
DTV_STATUS_T	DDI_PQL_AdjustADC(UINT16 *pADCOffset, UINT16 *pADCGain, PQL_CSC_LEVEL_CTRL_T *pstUserBlackLevelCtrl, BOOLEAN bAdjMode);


// by hwangbos 20080925
// 함수 일치율을 위해서 임시로 처리한 루틴 향후 정리해야함
void DDI_PQL_GetDftInitWBData(PQL_WB_DATA_T *pAdjWBdata);
PQL_WB_DATA_T DDI_PQL_GetDftWBData(void);
UINT8 DDI_PQL_GetDftSubBrightness(void);
UINT8 DDI_PQL_GetDBEyeQOSDValue( PQL_EYEQ_DB_MEMBER_T memberID, UINT8 index, UINT16 uEyeQStep);
SINT32 DDI_PQL_GetDBDimmingCtrl(PQL_DIMMING_DB_MEMBER_T  memberID, UINT8 index, UINT8 uCurveNum);
void DDI_PQL_GetDBEyecareDimmingCtrl(UINT8 index, PQL_EYECARE_DIMMING_DB_T *pDataBase);
void DDI_PQL_GetDBDC2PDynamicContrastCtrl(UINT8 type, UINT8 index, PQL_DC2P_DYN_CONTRAST_DB_T *pDataBase);
void DDI_PQL_GetDBDC2PApl(UINT8 type, UINT8 index, PQL_DC2P_APL_DB_T *pDataBase);
void DDI_PQL_GetDBDC2PCGain(UINT8 type, UINT8 index, PQL_DC2P_CGAIN_DB_T *pDataBase);
void DDI_PQL_GetDBDC2PSkin(UINT8 type, UINT8 index, PQL_DC2P_SKIN_DB_T *pDataBase);

void DDI_PQL_GetCoeffParam(UINT32 pqlIdCoeff, PQL_INPUT_SRC_FMT_T pqlSrcFmt, PQL_INDEX_MEMBER_T *pPqlIndexMember);

DTV_STATUS_T DDI_PQL_GetBrightContFor10pWB( PQL_PC_UI_VALUE_T pcUiVal, UINT8 pcId, float *pfContrast, float *pfBrightness );

/* CT */
DTV_STATUS_T DDI_PQL_SetFreshWhiteData(UINT8 uFreshColorUIVal);
DTV_STATUS_T DDI_PQL_GetCorrectValOfWBGainOffset(UINT8 index, PQL_WB_CORRECT_DATA_T *pWBCorrectVal);

/* Gamma */

/* Sharpness */
DTV_STATUS_T DDI_PQL_SetSharpByUserControl(PQL_SHARP_UI_VALUE_T	sPqUiVal, PQL_STATUS_T pqlStatus);

/* Scaling Filter */

/* NR */
DTV_STATUS_T DDI_PQL_SetNRByUserControl(PQL_STATUS_T pqlStatus, PQL_NR_LEVEL_T pqlNrLevel);
//[090908-clearcut] DCNR
DTV_STATUS_T DDI_PQL_SetNRByDcnrParam(UINT8 *pDcnrParam);
DTV_STATUS_T DDI_PQL_SetNRByDcnr(PQL_DC2P_RAWDATA_INFO_T *pHistoInfo, SINT32 *pLumaCurve);

/* PC */

/* DC */
DTV_STATUS_T  DDI_PQL_GetDCChipDataTypeInfo(UINT32 *pChipDataType);

/* Deinterlace */

/* Blacklevel */

/* xvYCC */
UINT8 DDI_PQL_GetxvYCCStatus(void);
UINT8 DDI_PQL_GetQuantRangeStatus(void);


/* etc */
void DDI_PQL_SetForTimmingChange(PQL_STATUS_T pqlStatus);

/***************************************************************************/
/**************** 	SOC DDI		********************************************/
/***************************************************************************/
DTV_STATUS_T DDI_PQL_InitSOC(void);
DTV_STATUS_T DDI_PQL_SetSocDefaultBySystem(void);

/* Color */
#if(PLATFORM_TYPE == MSTAR_PLATFORM)
DTV_STATUS_T DDI_PQL_SetColorStandard(PQL_CSC_LEVEL_CTRL_T *pstUserColorStandardCtrl); /* MStar only */
#endif
DTV_STATUS_T DDI_PQL_InitFreshColorParam(PQL_CM_FRESHCOLOR_PARAM_T *pPqlFreshColorParam);
DTV_STATUS_T DDI_PQL_InitExpColorParam(PQL_CM_EXPCOLOR_PARAM_T *pPqlExpColorParam);
DTV_STATUS_T DDI_PQL_SetFreshColor(PQL_CM_FRESHCOLOR_PARAM_T pqlFreshColorParam);
DTV_STATUS_T DDI_PQL_SetExpColorParam(UINT8 pqlCMCtrl, PQL_CM_EXPCOLOR_PARAM_T pqlExpColorParam);

/* CT */
DTV_STATUS_T DDI_PQL_SetColorTemp(PQL_COLOR_TEMP_T cscPcCtData);
DTV_STATUS_T DDI_PQL_SetFactoryStatus(BOOLEAN bFactoryStatus);

/* Gamma */
DTV_STATUS_T DDI_PQL_SetGamma(PQL_PATH_T pqlPath, UINT32 *pRedGammaTable, UINT32 *pGreenGammaTable, UINT32 *pBlueGammaTable);

/* Sharpness */
DTV_STATUS_T DDI_PQL_SetSharpSystemParam(PQL_STATUS_T pqlStatus, PQL_PATH_T pqlPath);
DTV_STATUS_T DDI_PQL_SetSharpByEdgeEnhance(UINT8 nUiValue, PQL_PATH_T pqlPath, PQL_STATUS_T pqlStatus);
DTV_STATUS_T DDI_PQL_SetSharpByUserParam(PQL_SHARP_UI_VALUE_T sPqUiVal, PQL_STATUS_T pqlStatus,UINT8 *pShCoeffId);
DTV_STATUS_T DDI_PQL_SetSRParam(PQL_SR_PARAM_T *pstSRParam);

/* NR */
DTV_STATUS_T DDI_PQL_SetNRSystemParam(PQL_STATUS_T pqlStatus, PQL_PATH_T pqlPath);
DTV_STATUS_T DDI_PQL_SetNRByUserParam(PQL_NR_CTRL_PARAM_T *pstNrCtrlParam);
DTV_STATUS_T DDI_PQL_SetMpegNRParam(PQL_NR_MPEGNR_PARAM_T *pstMpegNRParam);	//KWON_0618 Parameter change
#if(PLATFORM_TYPE == BCM_PLATFORM)
void DDI_PQL_BCM_DisableNoiseReduction(void); // for ADC Calibration
#endif
//void DDI_PQL_DisableANR(void);
//void DDI_PQL_DisableDNR(void);

/* PC */
void DDI_PQL_GetAdjSubBrightCont( PQL_SUB_CONTBRI_T *pAdjSubBriCont );
void DDI_PQL_SetAdjSubBrightCont( PQL_SUB_CONTBRI_T adjSubBriCont );
DTV_STATUS_T DDI_PQL_SetPC(PQL_PC_UI_VALUE_T sPcUiVal, UINT8  sPqCoeffId, PQL_PATH_T pqlPath);
DTV_STATUS_T DDI_PQL_SetColorGain(PQL_PC_UI_VALUE_T sPcUiVal, UINT8  sPqCoeffId, PQL_PATH_T pqlPath, SINT32 nColorGain);

/* DC */
DTV_STATUS_T DDI_PQL_SetDCFreshContrast(PQL_DC2P_CTRL_T *pCtrlInfo);
DTV_STATUS_T DDI_PQL_SetDCFreshContrastLUT(SINT32	*pAccLUT);
DTV_STATUS_T DDI_PQL_GetDCChipRawData(PQL_DC2P_RAWDATA_INFO_T *pHistoInfo);
DTV_STATUS_T DDI_PQL_SetAdaptiveCGain(SINT32 *pCGainParam);	//added by request of cho04
BOOLEAN DDI_PQL_GetSceneChangeStatus(void);

/* Deinterlace */
DTV_STATUS_T DDI_PQL_SetFilmMode(PQL_FILM_CTRL_T *pCtrlData, PQL_STATUS_T pqlStatus, PQL_PATH_T  pqlPath);
DTV_STATUS_T DDI_PQL_SetDeinterParam(UINT8 *pMDCoeffId, PQL_PATH_T pqlPath, PQL_STATUS_T pqlStatus);

/* Game mode */
DTV_STATUS_T DDI_PQL_SetGameMode(PQL_FILM_CTRL_T *pCtrlData);

/* Blacklevel */
DTV_STATUS_T DDI_PQL_SetBlackLevel(PQL_CSC_LEVEL_CTRL_T *pstUserBlackLevelCtrl);

/* etc */
PQL_AVDECODER_VIDEO_MODE_T DDI_PQL_GetColorSystem(void);
DTV_STATUS_T DDI_PQL_CtrlDecoderParam(PQL_ANDECODER_CTRL_T *pPqlAnalogDecoderCtrl);

/* Blue Only */
DTV_STATUS_T DDI_PQL_SetColorFilter(UINT8 uSelectColorFilter);

/* color gamut */
DTV_STATUS_T DDI_PQL_SetColorGamut(PQL_COLORGAMUT_CTRL_T *pqlColorGamut);
DTV_STATUS_T DDI_PQL_SetxvYCCData(PQL_COLORGAMUT_CTRL_T *pqlColorGamut);

/* XD Demo */
DTV_STATUS_T DDI_PQL_SetXDDemo( BOOLEAN bOnOff);

/* HDMI Color Domain */
DTV_STATUS_T DDI_PQL_GetHDMIColorDomain(PQL_HDMI_COLOR_DOMAIN_T *pqlHDMIColorDomain);
DTV_STATUS_T DDI_PQL_GetHDMIQuantRange(PQL_HDMI_QUANTIZATION_RANGE_T *pPqlHDMIQuantRange);

#if(PLATFORM_TYPE == MSTAR_PLATFORM)
/* CSC Offset Enable bit on off for xvYCC */
void DDI_PQL_SetCSCOffsetEnable(BOOLEAN bEnable);
#endif

/*Set YUV to RGB transform matrix*/
DTV_STATUS_T DDI_PQL_SetYUV2RGBMatrix(PQL_COLORSTANDARD_T outputCSC); //[godfather99] 2009/01/29 YUV2GRB matrix setting 일원화.

#if(PLATFORM_TYPE == MSTAR_PLATFORM)
/* Set Black level to bypass */
DTV_STATUS_T DDI_PQL_SetBlackLevelBypass(void);
#endif

/* Set Skin Color */
DTV_STATUS_T DDI_PQL_SetSkinColor(PQL_CM_SKINCOLOR_PARAM_T *pstSkinColor);

/* Set Dynamic White */
DTV_STATUS_T DDI_PQL_SetDynamicWhite(PQL_CM_DYNAMICWHITE_PARAM_T *pstDynamicWhite);

/* XD Plasma PDP Only */
// [ vivakjh] 2010/08/13 XD Plasma 2.0 Spec  삭제함. DTV_STATUS_T DDI_PQL_SetXDPCC(UINT8 nUiValue, PQL_PATH_T pqlPath, PQL_STATUS_T pqlStatus);			//[vivakjh] 2009/02/23  PDP XD Plasma 대응.
DTV_STATUS_T DDI_PQL_SetXDPFreshWhite(UINT8 nUiValue, PQL_PATH_T pqlPath, PQL_STATUS_T pqlStatus);	//[vivakjh] 2009/02/23  PDP XD Plasma 대응.
DTV_STATUS_T DDI_PQL_SetXDPColor(UINT8 nUiValue, PQL_PATH_T pqlPath, PQL_STATUS_T pqlStatus);			//[vivakjh] 2009/09/19  PDP XD Plasma 3.0 대응.
DTV_STATUS_T DDI_PQL_SetXDPSharpness(UINT8 nUiValue, PQL_PATH_T pqlPath, PQL_STATUS_T pqlStatus);		//[vivakjh] 2009/02/23  PDP XD Plasma 대응.
void DDI_PQL_ReloadXDPlasmaData(void);


/* Inner Pattern use Osd or not */
BOOLEAN DDI_PQL_IsOsdInnerPattern(void);
/* algorithm for preventing osd saturation need or not */
BOOLEAN DDI_PQL_IsAlgoForFixOsdSaturation(void);

/* check auto wb tick time */
UINT8 DDI_PQL_CheckAutoWbTickTime(void);

/* Set Test Pattern Status */
void DDI_PQL_SetTestPatternStatus(BOOLEAN bOnOff);

#if(PLATFORM_TYPE == BCM_PLATFORM)
/* bcm : for reducing a setting time, but saturn : not use*/
DTV_STATUS_T DDI_PQL_BCM_SetUpdateManualMode(BOOLEAN bOnOff);

DTV_STATUS_T DDI_PQL_BCM_RestoreBlockMode(void);	//KWON_0916 for NR off 해제 문제

void DDI_PQL_BCM_DisplayIREPattern(UINT8 patternType, UINT8 patternParam, PQL_BLACK_SOURCE_T blackSrc);	//by may.yoon
#endif

#ifdef INCLUDE_WIRELESS_READY
void DDI_PQL_SetWirelessInputConnectStatus(BOOLEAN bSet);

void DDI_PQL_SetWirelessInputHDMIColorDomain(PQL_HDMI_COLOR_DOMAIN_T pqlHDMIColorDomain);
void DDI_PQL_SetWirelessInputColorSystem(PQL_AVDECODER_VIDEO_MODE_T pqlMode);
#endif

const UINT8*  DDI_PQL_GetDbString(void);
void DDI_PQL_SetPqlStatusReference(const PQL_STATUS_T *pPqlStatus);
const PQL_STATUS_T *DDI_PQL_GetPqlStatus(void);

// [jykim] 100602 : Set 간 sync 동기화 - 전시 mode에서 set간 IR 간섭을 없애기 위한 목적
//DTV_STATUS_T DDI_PQL_SetFrmaeDelay(BOOLEAN OnOffSet,UINT8 InputFreq);

#endif

#ifdef __cplusplus
}
#endif

#endif	/* __PQL_DDI_H_ */

