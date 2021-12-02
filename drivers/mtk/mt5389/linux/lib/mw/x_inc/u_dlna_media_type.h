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
/*----------------------------------------------------------------------------*/
/*! @file u_dlna_media_type.h
*  $RCSfile: $
*  $Revision: #1 $
*  $Date: 2012/04/27 $
*  $Author: dtvbm11 $
*  
*  @par Description:
*         This header file contains DLNA media type exported constants, macros,
*         and types.
*/
/*----------------------------------------------------------------------------*/

/*!
* @addtogroup  groupMW_INET_DLNA
* @brief       Provide an external API about DLNA.
* %DLNA API would be used by user who wants to play media stream in local network  
* 
* @see 
*
* @{
* */

#ifndef U_DLNA_MEDIA_TYPE_H
#define U_DLNA_MEDIA_TYPE_H

#include "u_common.h"


/* Audio Class */
#define DLNA_MEDIA_MIME_TYPE_AUDIO_3GPP "audio/3gpp"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_EAC3 "audio/eac3"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_L16 "audio/L16"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_MP4 "audio/mp4"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_MPEG "audio/mpeg"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_VND_DLNA_ADTS "audio/vnd.dlna.adts"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_VND_DOLBY_DD_RAW "audio/vnd.dolby.dd-raw"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_VND_DOLBY_MLP "audio/vnd.dolby.mlp"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_WAV "audio/wav"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_X_DAB "audio/x-dab"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_X_DTS "audio/x-dts"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_X_DTS_HD_HRA "audio/x-dts-hd-hra"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_X_DTS_HD_LBR "audio/x-dts-hd-lbr"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_X_DTS_HD_MA "audio/x-dts-hd-ma"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_X_MS_WMA "audio/x-ms-wma"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_X_SONY_OMA "audio/x-sony-oma"

/* Audio playlist Class */
#define DLNA_MEDIA_MIME_TYPE_AUDIO_M3U "audio/m3u"
#define DLNA_MEDIA_MIME_TYPE_AUDIO_PLS "audio/pls"

/* Image Class */
#define DLNA_MEDIA_MIME_TYPE_IMAGE_BMP "image/bmp"
#define DLNA_MEDIA_MIME_TYPE_IMAGE_GIF "image/gif"
#define DLNA_MEDIA_MIME_TYPE_IMAGE_JPEG "image/jpeg"
#define DLNA_MEDIA_MIME_TYPE_IMAGE_PNG "image/png"

/* Text Class */
#define DLNA_MEDIA_MIME_TYPE_TEXT_XML "text/xml"

/* Video Class */
#define DLNA_MEDIA_MIME_TYPE_VIDEO_3GPP "video/3gpp"
#define DLNA_MEDIA_MIME_TYPE_VIDEO_MP4 "video/mp4"
#define DLNA_MEDIA_MIME_TYPE_VIDEO_MPEG "video/mpeg"
#define DLNA_MEDIA_MIME_TYPE_VIDEO_VND_DLNA_MPEG_TTS "video/vnd.dlna.mpeg-tts"
#define DLNA_MEDIA_MIME_TYPE_VIDEO_X_MPEG2_DIRECTV "video/x-mpeg2-directv"
#define DLNA_MEDIA_MIME_TYPE_VIDEO_X_MS_ASF "video/x-ms-asf"
#define DLNA_MEDIA_MIME_TYPE_VIDEO_X_MS_AVI "video/x-ms-avi"
#define DLNA_MEDIA_MIME_TYPE_VIDEO_X_MS_WMV "video/x-ms-wmv"

/* External Video Class */
#define DLNA_MEDIA_MIME_TYPE_VIDEO_AVI "video/avi"
#define DLNA_MEDIA_MIME_TYPE_VIDEO_DIVX "video/divx"
#define DLNA_MEDIA_MIME_TYPE_VIDEO_H263 "video/H263"
#define DLNA_MEDIA_MIME_TYPE_VIDEO_H264 "video/H264"
#define DLNA_MEDIA_MIME_TYPE_VIDEO_X_MS_VIDEO "video/x-ms-video"



/* Media Format Profile ID */

/* Image Class: JPEG Profiles */
#define DLNA_MFP_JPEG_SM "JPEG_SM"
#define DLNA_MFP_JPEG_MED "JPEG_MED"
#define DLNA_MFP_JPEG_LRG "JPEG_LRG"
#define DLNA_MFP_JPEG_TN "JPEG_TN"
#define DLNA_MFP_JPEG_SM_ICO "JPEG_SM_ICO"
#define DLNA_MFP_JPEG_LRG_ICO "JPEG_LRG_ICO"

/* Image Class: PNG Profiles */
#define DLNA_MFP_PNG_TN "PNG_TN"
#define DLNA_MFP_PNG_SM_ICO "PNG_SM_ICO"
#define DLNA_MFP_PNG_LRG_ICO "PNG_LRG_ICO"
#define DLNA_MFP_PNG_LRG "PNG_LRG"

/* Audio Class: AC-3 Profiles */
#define DLNA_MFP_AC3 "AC3"

/* Audio Class: AMR Profiles */
#define DLNA_MFP_AMR_3GPP "AMR_3GPP"
#define DLNA_MFP_AMR_WBplus "AMR_WBplus"

/* Audio Class: ATRAC3plus Profiles */
#define DLNA_MFP_ATRAC3plus "ATRAC3plus"

/* Audio Class: LPCM Profiles */
#define DLNA_MFP_LPCM "LPCM"

/* Audio Class: MP3 Profiles */
#define DLNA_MFP_MP3 "MP3"
#define DLNA_MFP_MP3X "MP3X"

/* Audio Class: MPEG-4 Profiles */
#define DLNA_MFP_AAC_ADTS "AAC_ADTS"
#define DLNA_MFP_AAC_ADTS_320 "AAC_ADTS_320"
#define DLNA_MFP_AAC_ISO "AAC_ISO"
#define DLNA_MFP_AAC_ISO_320 "AAC_ISO_320"
#define DLNA_MFP_AAC_LTP_ISO "AAC_LTP_ISO"
#define DLNA_MFP_AAC_LTP_MULT5_ISO "AAC_LTP_MULT5_ISO"
#define DLNA_MFP_HEAAC_L2_ADTS "HEAAC_L2_ADTS"
#define DLNA_MFP_HEAAC_L2_ISO "HEAAC_L2_ISO"
#define DLNA_MFP_HEAAC_L3_ADTS "HEAAC_L3_ADTS"
#define DLNA_MFP_HEAAC_L3_ISO "HEAAC_L3_ISO"
#define DLNA_MFP_HEAAC_MULT5_ADTS "HEAAC_MULT5_ADTS"
#define DLNA_MFP_HEAAC_MULT5_ISO "HEAAC_MULT5_ISO"
#define DLNA_MFP_HEAAC_L2_ADTS_320 "HEAAC_L2_ADTS_320"
#define DLNA_MFP_HEAAC_L2_ISO_320 "HEAAC_L2_ISO_320"
#define DLNA_MFP_BSAC_ISO "BSAC_ISO"
#define DLNA_MFP_BSAC_MULT5_ISO "BSAC_MULT5_ISO"

/* Audio Class: WMA Profiles */
#define DLNA_MFP_WMABASE "WMABASE"
#define DLNA_MFP_WMAFULL "WMAFULL"
#define DLNA_MFP_WMAPRO "WMAPRO"

/* AV Class: MPEG-1 Profiles */
#define DLNA_MFP_MPEG1 "MPEG1"

/* AV Class: MPEG-2 Profiles */
#define DLNA_MFP_MPEG_PS_NTSC "MPEG_PS_NTSC"
#define DLNA_MFP_MPEG_PS_NTSC_XAC3 "MPEG_PS_NTSC_XAC3"
#define DLNA_MFP_MPEG_PS_PAL "MPEG_PS_PAL"
#define DLNA_MFP_MPEG_PS_PAL_XAC3 "MPEG_PS_PAL_XAC3"
#define DLNA_MFP_MPEG_TS_SD_NA "MPEG_TS_SD_NA"
#define DLNA_MFP_MPEG_TS_SD_NA_T "MPEG_TS_SD_NA_T"
#define DLNA_MFP_MPEG_TS_SD_NA_ISO "MPEG_TS_SD_NA_ISO"
#define DLNA_MFP_MPEG_TS_HD_NA "MPEG_TS_HD_NA"
#define DLNA_MFP_MPEG_TS_HD_NA_T "MPEG_TS_HD_NA_T"
#define DLNA_MFP_MPEG_TS_HD_NA_ISO "MPEG_TS_HD_NA_ISO"
#define DLNA_MFP_MPEG_TS_SD_EU "MPEG_TS_SD_EU"
#define DLNA_MFP_MPEG_TS_SD_EU_T "MPEG_TS_SD_EU_T"
#define DLNA_MFP_MPEG_TS_SD_EU_ISO "MPEG_TS_SD_EU_ISO"
#define DLNA_MFP_MPEG_TS_SD_KO "MPEG_TS_SD_KO"
#define DLNA_MFP_MPEG_TS_SD_KO_T "MPEG_TS_SD_KO_T"
#define DLNA_MFP_MPEG_TS_SD_KO_ISO "MPEG_TS_SD_KO_ISO"
#define DLNA_MFP_MPEG_TS_HD_KO "MPEG_TS_HD_KO"
#define DLNA_MFP_MPEG_TS_HD_KO_T "MPEG_TS_HD_KO_T"
#define DLNA_MFP_MPEG_TS_HD_KO_ISO "MPEG_TS_HD_KO_ISO"
#define DLNA_MFP_MPEG_TS_HD_KO_XAC3 "MPEG_TS_HD_KO_XAC3"
#define DLNA_MFP_MPEG_TS_HD_KO_XAC3_T "MPEG_TS_HD_KO_XAC3_T"
#define DLNA_MFP_MPEG_TS_HD_KO_XAC3_ISO "MPEG_TS_HD_KO_XAC3_ISO"
#define DLNA_MFP_MPEG_TS_HD_NA_XAC3 "MPEG_TS_HD_NA_XAC3"
#define DLNA_MFP_MPEG_TS_HD_NA_XAC3_T "MPEG_TS_HD_NA_XAC3_T"
#define DLNA_MFP_MPEG_TS_HD_NA_XAC3_ISO "MPEG_TS_HD_NA_XAC3_ISO"
#define DLNA_MFP_MPEG_TS_SD_KO_XAC3 "MPEG_TS_SD_KO_XAC3"
#define DLNA_MFP_MPEG_TS_SD_KO_XAC3_T "MPEG_TS_SD_KO_XAC3_T"
#define DLNA_MFP_MPEG_TS_SD_KO_XAC3_ISO "MPEG_TS_SD_KO_XAC3_ISO"
#define DLNA_MFP_MPEG_TS_SD_NA_XAC3 "MPEG_TS_SD_NA_XAC3"
#define DLNA_MFP_MPEG_TS_SD_NA_XAC3_T "MPEG_TS_SD_NA_XAC3_T"
#define DLNA_MFP_MPEG_TS_SD_NA_XAC3_ISO "MPEG_TS_SD_NA_XAC3_ISO"
#define DLNA_MFP_MPEG_TS_MP_LL_AAC "MPEG_TS_MP_LL_AAC"
#define DLNA_MFP_MPEG_TS_MP_LL_AAC_T "MPEG_TS_MP_LL_AAC_T"
#define DLNA_MFP_MPEG_TS_MP_LL_AAC_ISO "MPEG_TS_MP_LL_AAC_ISO"
#define DLNA_MFP_MPEG_ES_PAL "MPEG_ES_PAL"
#define DLNA_MFP_MPEG_ES_NTSC "MPEG_ES_NTSC"
#define DLNA_MFP_MPEG_ES_PAL_XAC3 "MPEG_ES_PAL_XAC3"
#define DLNA_MFP_MPEG_ES_NTSC_XAC3 "MPEG_ES_NTSC_XAC3"

/* AV Class: MPEG-4 Part 2 Profiles */
#define DLNA_MFP_MPEG4_P2_MP4_SP_AAC "MPEG4_P2_MP4_SP_AAC"
#define DLNA_MFP_MPEG4_P2_MP4_SP_HEAAC "MPEG4_P2_MP4_SP_HEAAC"
#define DLNA_MFP_MPEG4_P2_MP4_SP_ATRAC3plus "MPEG4_P2_MP4_SP_ATRAC3plus"
#define DLNA_MFP_MPEG4_P2_MP4_SP_AAC_LTP "MPEG4_P2_MP4_SP_AAC_LTP"
#define DLNA_MFP_MPEG4_P2_MP4_SP_L2_AAC "MPEG4_P2_MP4_SP_L2_AAC"
#define DLNA_MFP_MPEG4_P2_MP4_SP_L2_AMR "MPEG4_P2_MP4_SP_L2_AMR"
#define DLNA_MFP_MPEG4_P2_TS_SP_AAC "MPEG4_P2_TS_SP_AAC"
#define DLNA_MFP_MPEG4_P2_TS_SP_AAC_T "MPEG4_P2_TS_SP_AAC_T"
#define DLNA_MFP_MPEG4_P2_TS_SP_AAC_ISO "MPEG4_P2_TS_SP_AAC_ISO"
#define DLNA_MFP_MPEG4_P2_TS_SP_MPEG1_L3 "MPEG4_P2_TS_SP_MPEG1_L3"
#define DLNA_MFP_MPEG4_P2_TS_SP_MPEG1_L3_T "MPEG4_P2_TS_SP_MPEG1_L3_T"
#define DLNA_MFP_MPEG4_P2_TS_SP_MPEG1_L3_ISO "MPEG4_P2_TS_SP_MPEG1_L3_ISO"
#define DLNA_MFP_MPEG4_P2_TS_SP_AC3 "MPEG4_P2_TS_SP_AC3"
#define DLNA_MFP_MPEG4_P2_TS_SP_AC3_T "MPEG4_P2_TS_SP_AC3_T"
#define DLNA_MFP_MPEG4_P2_TS_SP_AC3_ISO "MPEG4_P2_TS_SP_AC3_ISO"
#define DLNA_MFP_MPEG4_P2_TS_SP_MPEG2_L2 "MPEG4_P2_TS_SP_MPEG2_L2"
#define DLNA_MFP_MPEG4_P2_TS_SP_MPEG2_L2_T "MPEG4_P2_TS_SP_MPEG2_L2_T"
#define DLNA_MFP_MPEG4_P2_TS_SP_MPEG2_L2_ISO "MPEG4_P2_TS_SP_MPEG2_L2_ISO"
#define DLNA_MFP_MPEG4_P2_ASF_SP_G726 "MPEG4_P2_ASF_SP_G726"
#define DLNA_MFP_MPEG4_P2_MP4_SP_VGA_AAC "MPEG4_P2_MP4_SP_VGA_AAC"
#define DLNA_MFP_MPEG4_P2_MP4_SP_VGA_HEAAC "MPEG4_P2_MP4_SP_VGA_HEAAC"
#define DLNA_MFP_MPEG4_P2_MP4_ASP_AAC "MPEG4_P2_MP4_ASP_AAC"
#define DLNA_MFP_MPEG4_P2_MP4_ASP_HEAAC "MPEG4_P2_MP4_ASP_HEAAC"
#define DLNA_MFP_MPEG4_P2_MP4_ASP_HEAAC_MULT5 "MPEG4_P2_MP4_ASP_HEAAC_MULT5"
#define DLNA_MFP_MPEG4_P2_MP4_ASP_ATRAC3plus "MPEG4_P2_MP4_ASP_ATRAC3plus"
#define DLNA_MFP_MPEG4_P2_TS_ASP_AAC "MPEG4_P2_TS_ASP_AAC"
#define DLNA_MFP_MPEG4_P2_TS_ASP_AAC_T "MPEG4_P2_TS_ASP_AAC_T"
#define DLNA_MFP_MPEG4_P2_TS_ASP_AAC_ISO "MPEG4_P2_TS_ASP_AAC_ISO"
#define DLNA_MFP_MPEG4_P2_TS_ASP_MPEG1_L3 "MPEG4_P2_TS_ASP_MPEG1_L3"
#define DLNA_MFP_MPEG4_P2_TS_ASP_MPEG1_L3_T "MPEG4_P2_TS_ASP_MPEG1_L3_T"
#define DLNA_MFP_MPEG4_P2_TS_ASP_MPEG1_L3_ISO "MPEG4_P2_TS_ASP_MPEG1_L3_ISO"
#define DLNA_MFP_MPEG4_P2_TS_ASP_AC3 "MPEG4_P2_TS_ASP_AC3"
#define DLNA_MFP_MPEG4_P2_TS_ASP_AC3_T "MPEG4_P2_TS_ASP_AC3_T"
#define DLNA_MFP_MPEG4_P2_TS_ASP_AC3_ISO "MPEG4_P2_TS_ASP_AC3_ISO"
#define DLNA_MFP_MPEG4_P2_MP4_ASP_L5_SO_AAC "MPEG4_P2_MP4_ASP_L5_SO_AAC"
#define DLNA_MFP_MPEG4_P2_MP4_ASP_L5_SO_HEAAC "MPEG4_P2_MP4_ASP_L5_SO_HEAAC"
#define DLNA_MFP_MPEG4_P2_MP4_ASP_L5_SO_HEAAC_MULT5 "MPEG4_P2_MP4_ASP_L5_SO_HEAAC_MULT5"
#define DLNA_MFP_MPEG4_P2_ASF_ASP_L5_SO_G726 "MPEG4_P2_ASF_ASP_L5_SO_G726"
#define DLNA_MFP_MPEG4_P2_MP4_ASP_L4_SO_AAC "MPEG4_P2_MP4_ASP_L4_SO_AAC"
#define DLNA_MFP_MPEG4_P2_MP4_ASP_L4_SO_HEAAC "MPEG4_P2_MP4_ASP_L4_SO_HEAAC"
#define DLNA_MFP_MPEG4_P2_MP4_ASP_L4_SO_HEAAC_MULT5 "MPEG4_P2_MP4_ASP_L4_SO_HEAAC_MULT5"
#define DLNA_MFP_MPEG4_P2_ASF_ASP_L4_SO_G726 "MPEG4_P2_ASF_ASP_L4_SO_G726"
#define DLNA_MFP_MPEG4_H263_MP4_P0_L10_AAC "MPEG4_H263_MP4_P0_L10_AAC"
#define DLNA_MFP_MPEG4_H263_MP4_P0_L10_AAC_LTP "MPEG4_H263_MP4_P0_L10_AAC_LTP"
#define DLNA_MFP_MPEG4_H263_3GPP_P0_L10_AMR_WBplus "MPEG4_H263_3GPP_P0_L10_AMR_WBplus"
#define DLNA_MFP_MPEG4_P2_TS_CO_AC3 "MPEG4_P2_TS_CO_AC3"
#define DLNA_MFP_MPEG4_P2_TS_CO_AC3_T "MPEG4_P2_TS_CO_AC3_T"
#define DLNA_MFP_MPEG4_P2_TS_CO_AC3_ISO "MPEG4_P2_TS_CO_AC3_ISO"
#define DLNA_MFP_MPEG4_P2_TS_CO_MPEG2_L2 "MPEG4_P2_TS_CO_MPEG2_L2"
#define DLNA_MFP_MPEG4_P2_TS_CO_MPEG2_L2_T "MPEG4_P2_TS_CO_MPEG2_L2_T"
#define DLNA_MFP_MPEG4_P2_TS_CO_MPEG2_L2_ISO "MPEG4_P2_TS_CO_MPEG2_L2_ISO"
#define DLNA_MFP_MPEG4_P2_3GPP_SP_L0B_AAC "MPEG4_P2_3GPP_SP_L0B_AAC"
#define DLNA_MFP_MPEG4_P2_3GPP_SP_L0B_AMR "MPEG4_P2_3GPP_SP_L0B_AMR"
#define DLNA_MFP_MPEG4_H263_3GPP_P3_L10_AMR "MPEG4_H263_3GPP_P3_L10_AMR"

/* AV Class: MPEG-4 Part 10 (AVC) Profiles */
#define DLNA_MFP_AVC_TS_MP_SD_AAC_MULT5 "AVC_TS_MP_SD_AAC_MULT5"
#define DLNA_MFP_AVC_TS_MP_SD_AAC_MULT5_T "AVC_TS_MP_SD_AAC_MULT5_T"
#define DLNA_MFP_AVC_TS_MP_SD_AAC_MULT5_ISO "AVC_TS_MP_SD_AAC_MULT5_ISO"
#define DLNA_MFP_AVC_TS_MP_SD_HEAAC_L2 "AVC_TS_MP_SD_HEAAC_L2"
#define DLNA_MFP_AVC_TS_MP_SD_HEAAC_L2_T "AVC_TS_MP_SD_HEAAC_L2_T"
#define DLNA_MFP_AVC_TS_MP_SD_HEAAC_L2_ISO "AVC_TS_MP_SD_HEAAC_L2_ISO"
#define DLNA_MFP_AVC_TS_MP_SD_MPEG1_L3 "AVC_TS_MP_SD_MPEG1_L3"
#define DLNA_MFP_AVC_TS_MP_SD_MPEG1_L3_T "AVC_TS_MP_SD_MPEG1_L3_T"
#define DLNA_MFP_AVC_TS_MP_SD_MPEG1_L3_ISO "AVC_TS_MP_SD_MPEG1_L3_ISO"
#define DLNA_MFP_AVC_TS_MP_SD_AC3 "AVC_TS_MP_SD_AC3"
#define DLNA_MFP_AVC_TS_MP_SD_AC3_T "AVC_TS_MP_SD_AC3_T"
#define DLNA_MFP_AVC_TS_MP_SD_AC3_T "AVC_TS_MP_SD_AC3_T"
#define DLNA_MFP_AVC_TS_MP_SD_AAC_LTP "AVC_TS_MP_SD_AAC_LTP"
#define DLNA_MFP_AVC_TS_MP_SD_AAC_LTP_T "AVC_TS_MP_SD_AAC_LTP_T"
#define DLNA_MFP_AVC_TS_MP_SD_AAC_LTP_ISO "AVC_TS_MP_SD_AAC_LTP_ISO"
#define DLNA_MFP_AVC_TS_MP_SD_AAC_LTP_MULT5 "AVC_TS_MP_SD_AAC_LTP_MULT5"
#define DLNA_MFP_AVC_TS_MP_SD_AAC_LTP_MULT5_T "AVC_TS_MP_SD_AAC_LTP_MULT5_T"
#define DLNA_MFP_AVC_TS_MP_SD_AAC_LTP_MULT5_ISO "AVC_TS_MP_SD_AAC_LTP_MULT5_ISO"
#define DLNA_MFP_AVC_TS_MP_SD_AAC_LTP_MULT7 "AVC_TS_MP_SD_AAC_LTP_MULT7"
#define DLNA_MFP_AVC_TS_MP_SD_AAC_LTP_MULT7_T "AVC_TS_MP_SD_AAC_LTP_MULT7_T"
#define DLNA_MFP_AVC_TS_MP_SD_AAC_LTP_MULT7_ISO "AVC_TS_MP_SD_AAC_LTP_MULT7_ISO"
#define DLNA_MFP_AVC_TS_MP_SD_BSAC "AVC_TS_MP_SD_BSAC"
#define DLNA_MFP_AVC_TS_MP_SD_BSAC_T "AVC_TS_MP_SD_BSAC_T"
#define DLNA_MFP_AVC_TS_MP_SD_BSAC_ISO "AVC_TS_MP_SD_BSAC_ISO"
#define DLNA_MFP_AVC_MP4_MP_SD_AAC_MULT5 "AVC_MP4_MP_SD_AAC_MULT5"
#define DLNA_MFP_AVC_MP4_MP_SD_HEAAC_L2 "AVC_MP4_MP_SD_HEAAC_L2"
#define DLNA_MFP_AVC_MP4_MP_SD_MPEG1_L3 "AVC_MP4_MP_SD_MPEG1_L3"
#define DLNA_MFP_AVC_MP4_MP_SD_AC3 "AVC_MP4_MP_SD_AC3"
#define DLNA_MFP_AVC_MP4_MP_SD_AAC_LTP "AVC_MP4_MP_SD_AAC_LTP"
#define DLNA_MFP_AVC_MP4_MP_SD_AAC_LTP_MULT5 "AVC_MP4_MP_SD_AAC_LTP_MULT5"
#define DLNA_MFP_AVC_MP4_MP_SD_AAC_LTP_MULT7 "AVC_MP4_MP_SD_AAC_LTP_MULT7"
#define DLNA_MFP_AVC_MP4_MP_SD_ATRAC3plus "AVC_MP4_MP_SD_ATRAC3plus"
#define DLNA_MFP_AVC_MP4_BL_L3L_SD_AAC "AVC_MP4_BL_L3L_SD_AAC"
#define DLNA_MFP_AVC_MP4_BL_L3L_SD_HEAAC "AVC_MP4_BL_L3L_SD_HEAAC"
#define DLNA_MFP_AVC_MP4_BL_L3_SD_AAC "AVC_MP4_BL_L3_SD_AAC"
#define DLNA_MFP_AVC_MP4_MP_SD_BSAC "AVC_MP4_MP_SD_BSAC"
#define DLNA_MFP_AVC_TS_BL_CIF30_AAC_MULT5 "AVC_TS_BL_CIF30_AAC_MULT5"
#define DLNA_MFP_AVC_TS_BL_CIF30_AAC_MULT5_T "AVC_TS_BL_CIF30_AAC_MULT5_T"
#define DLNA_MFP_AVC_TS_BL_CIF30_AAC_MULT5_ISO "AVC_TS_BL_CIF30_AAC_MULT5_ISO"
#define DLNA_MFP_AVC_TS_BL_CIF30_HEAAC_L2 "AVC_TS_BL_CIF30_HEAAC_L2"
#define DLNA_MFP_AVC_TS_BL_CIF30_HEAAC_L2_T "AVC_TS_BL_CIF30_HEAAC_L2_T"
#define DLNA_MFP_AVC_TS_BL_CIF30_HEAAC_L2_ISO "AVC_TS_BL_CIF30_HEAAC_L2_ISO"
#define DLNA_MFP_AVC_TS_BL_CIF30_MPEG1_L3 "AVC_TS_BL_CIF30_MPEG1_L3"
#define DLNA_MFP_AVC_TS_BL_CIF30_MPEG1_L3_T "AVC_TS_BL_CIF30_MPEG1_L3_T"
#define DLNA_MFP_AVC_TS_BL_CIF30_MPEG1_L3_ISO "AVC_TS_BL_CIF30_MPEG1_L3_ISO"
#define DLNA_MFP_AVC_TS_BL_CIF30_AC3 "AVC_TS_BL_CIF30_AC3"
#define DLNA_MFP_AVC_TS_BL_CIF30_AC3_T "AVC_TS_BL_CIF30_AC3_T"
#define DLNA_MFP_AVC_TS_BL_CIF30_AC3_ISO "AVC_TS_BL_CIF30_AC3_ISO"
#define DLNA_MFP_AVC_TS_BL_CIF30_AAC_LTP "AVC_TS_BL_CIF30_AAC_LTP"
#define DLNA_MFP_AVC_TS_BL_CIF30_AAC_LTP_T "AVC_TS_BL_CIF30_AAC_LTP_T"
#define DLNA_MFP_AVC_TS_BL_CIF30_AAC_LTP_ISO "AVC_TS_BL_CIF30_AAC_LTP_ISO"
#define DLNA_MFP_AVC_TS_BL_CIF30_AAC_LTP_MULT5 "AVC_TS_BL_CIF30_AAC_LTP_MULT5"
#define DLNA_MFP_AVC_TS_BL_CIF30_AAC_LTP_MULT5_T "AVC_TS_BL_CIF30_AAC_LTP_MULT5_T"
#define DLNA_MFP_AVC_TS_BL_CIF30_AAC_LTP_MULT5_ISO "AVC_TS_BL_CIF30_AAC_LTP_MULT5_ISO"
#define DLNA_MFP_AVC_TS_BL_CIF30_AAC_940 "AVC_TS_BL_CIF30_AAC_940"
#define DLNA_MFP_AVC_TS_BL_CIF30_AAC_940_T "AVC_TS_BL_CIF30_AAC_940_T"
#define DLNA_MFP_AVC_TS_BL_CIF30_AAC_940_ISO "AVC_TS_BL_CIF30_AAC_940_ISO"
#define DLNA_MFP_AVC_MP4_BL_CIF30_AAC_MULT5 "AVC_MP4_BL_CIF30_AAC_MULT5"
#define DLNA_MFP_AVC_MP4_BL_CIF30_HEAAC_L2 "AVC_MP4_BL_CIF30_HEAAC_L2"
#define DLNA_MFP_AVC_MP4_BL_CIF30_MPEG1_L3 "AVC_MP4_BL_CIF30_MPEG1_L3"
#define DLNA_MFP_AVC_MP4_BL_CIF30_AC3 "AVC_MP4_BL_CIF30_AC3"
#define DLNA_MFP_AVC_MP4_BL_CIF30_AAC_LTP "AVC_MP4_BL_CIF30_AAC_LTP"
#define DLNA_MFP_AVC_MP4_BL_CIF30_AAC_LTP_MULT5 "AVC_MP4_BL_CIF30_AAC_LTP_MULT5"
#define DLNA_MFP_AVC_MP4_BL_L2_CIF30_AAC "AVC_MP4_BL_L2_CIF30_AAC"
#define DLNA_MFP_AVC_MP4_BL_CIF30_BSAC "AVC_MP4_BL_CIF30_BSAC"
#define DLNA_MFP_AVC_MP4_BL_CIF30_BSAC_MULT5 "AVC_MP4_BL_CIF30_BSAC_MULT5"
#define DLNA_MFP_AVC_MP4_BL_CIF15_HEAAC "AVC_MP4_BL_CIF15_HEAAC"
#define DLNA_MFP_AVC_MP4_BL_CIF15_AMR "AVC_MP4_BL_CIF15_AMR"
#define DLNA_MFP_AVC_TS_MP_HD_AAC_MULT5 "AVC_TS_MP_HD_AAC_MULT5"
#define DLNA_MFP_AVC_TS_MP_HD_AAC_MULT5_T "AVC_TS_MP_HD_AAC_MULT5_T"
#define DLNA_MFP_AVC_TS_MP_HD_AAC_MULT5_ISO "AVC_TS_MP_HD_AAC_MULT5_ISO"
#define DLNA_MFP_AVC_TS_MP_HD_HEAAC_L2 "AVC_TS_MP_HD_HEAAC_L2"
#define DLNA_MFP_AVC_TS_MP_HD_HEAAC_L2_T "AVC_TS_MP_HD_HEAAC_L2_T"
#define DLNA_MFP_AVC_TS_MP_HD_HEAAC_L2_ISO "AVC_TS_MP_HD_HEAAC_L2_ISO"
#define DLNA_MFP_AVC_TS_MP_HD_MPEG1_L3 "AVC_TS_MP_HD_MPEG1_L3"
#define DLNA_MFP_AVC_TS_MP_HD_MPEG1_L3_T "AVC_TS_MP_HD_MPEG1_L3_T"
#define DLNA_MFP_AVC_TS_MP_HD_MPEG1_L3_ISO "AVC_TS_MP_HD_MPEG1_L3_ISO"
#define DLNA_MFP_AVC_TS_MP_HD_AC3 "AVC_TS_MP_HD_AC3"
#define DLNA_MFP_AVC_TS_MP_HD_AC3_T "AVC_TS_MP_HD_AC3_T"
#define DLNA_MFP_AVC_TS_MP_HD_AC3_ISO "AVC_TS_MP_HD_AC3_ISO"
#define DLNA_MFP_AVC_TS_MP_HD_AAC "AVC_TS_MP_HD_AAC"
#define DLNA_MFP_AVC_TS_MP_HD_AAC_T "AVC_TS_MP_HD_AAC_T"
#define DLNA_MFP_AVC_TS_MP_HD_AAC_ISO "AVC_TS_MP_HD_AAC_ISO"
#define DLNA_MFP_AVC_TS_MP_HD_AAC_LTP "AVC_TS_MP_HD_AAC_LTP"
#define DLNA_MFP_AVC_TS_MP_HD_AAC_LTP_T "AVC_TS_MP_HD_AAC_LTP_T"
#define DLNA_MFP_AVC_TS_MP_HD_AAC_LTP_ISO "AVC_TS_MP_HD_AAC_LTP_ISO"
#define DLNA_MFP_AVC_TS_MP_HD_AAC_LTP_MULT5 "AVC_TS_MP_HD_AAC_LTP_MULT5"
#define DLNA_MFP_AVC_TS_MP_HD_AAC_LTP_MULT5_T "AVC_TS_MP_HD_AAC_LTP_MULT5_T"
#define DLNA_MFP_AVC_TS_MP_HD_AAC_LTP_MULT5_ISO "AVC_TS_MP_HD_AAC_LTP_MULT5_ISO"
#define DLNA_MFP_AVC_TS_MP_HD_AAC_LTP_MULT7 "AVC_TS_MP_HD_AAC_LTP_MULT7"
#define DLNA_MFP_AVC_TS_MP_HD_AAC_LTP_MULT7_T "AVC_TS_MP_HD_AAC_LTP_MULT7_T"
#define DLNA_MFP_AVC_TS_MP_HD_AAC_LTP_MULT7_ISO "AVC_TS_MP_HD_AAC_LTP_MULT7_ISO"
#define DLNA_MFP_AVC_TS_BL_CIF15_AAC "AVC_TS_BL_CIF15_AAC"
#define DLNA_MFP_AVC_TS_BL_CIF15_AAC_T "AVC_TS_BL_CIF15_AAC_T"
#define DLNA_MFP_AVC_TS_BL_CIF15_AAC_ISO "AVC_TS_BL_CIF15_AAC_ISO"
#define DLNA_MFP_AVC_TS_BL_CIF15_AAC_540 "AVC_TS_BL_CIF15_AAC_540"
#define DLNA_MFP_AVC_TS_BL_CIF15_AAC_540_T "AVC_TS_BL_CIF15_AAC_540_T"
#define DLNA_MFP_AVC_TS_BL_CIF15_AAC_540_ISO "AVC_TS_BL_CIF15_AAC_540_ISO"
#define DLNA_MFP_AVC_TS_BL_CIF15_AAC_LTP "AVC_TS_BL_CIF15_AAC_LTP"
#define DLNA_MFP_AVC_TS_BL_CIF15_AAC_LTP_T "AVC_TS_BL_CIF15_AAC_LTP_T"
#define DLNA_MFP_AVC_TS_BL_CIF15_AAC_LTP_ISO "AVC_TS_BL_CIF15_AAC_LTP_ISO"
#define DLNA_MFP_AVC_TS_BL_CIF15_BSAC "AVC_TS_BL_CIF15_BSAC"
#define DLNA_MFP_AVC_TS_BL_CIF15_BSAC_T "AVC_TS_BL_CIF15_BSAC_T"
#define DLNA_MFP_AVC_TS_BL_CIF15_BSAC_ISO "AVC_TS_BL_CIF15_BSAC_ISO"
#define DLNA_MFP_AVC_MP4_BL_CIF15_AAC "AVC_MP4_BL_CIF15_AAC"
#define DLNA_MFP_AVC_MP4_BL_CIF15_AAC_520 "AVC_MP4_BL_CIF15_AAC_520"
#define DLNA_MFP_AVC_MP4_BL_CIF15_AAC_LTP "AVC_MP4_BL_CIF15_AAC_LTP"
#define DLNA_MFP_AVC_MP4_BL_CIF15_AAC_LTP_520 "AVC_MP4_BL_CIF15_AAC_LTP_520"
#define DLNA_MFP_AVC_MP4_BL_CIF15_BSAC "AVC_MP4_BL_CIF15_BSAC"
#define DLNA_MFP_AVC_MP4_BL_L12_CIF15_HEAAC "AVC_MP4_BL_L12_CIF15_HEAAC"
#define DLNA_MFP_AVC_MP4_BL_L1B_QCIF15_HEAAC "AVC_MP4_BL_L1B_QCIF15_HEAAC"
#define DLNA_MFP_AVC_3GPP_BL_CIF30_AMR_WBplus "AVC_3GPP_BL_CIF30_AMR_WBplus"
#define DLNA_MFP_AVC_3GPP_BL_CIF15_AMR_WBplus "AVC_3GPP_BL_CIF15_AMR_WBplus"
#define DLNA_MFP_AVC_3GPP_BL_QCIF15_AAC "AVC_3GPP_BL_QCIF15_AAC"
#define DLNA_MFP_AVC_3GPP_BL_QCIF15_AAC_LTP "AVC_3GPP_BL_QCIF15_AAC_LTP"
#define DLNA_MFP_AVC_3GPP_BL_QCIF15_HEAAC "AVC_3GPP_BL_QCIF15_HEAAC"
#define DLNA_MFP_AVC_3GPP_BL_QCIF15_AMR_WBplus "AVC_3GPP_BL_QCIF15_AMR_WBplus"
#define DLNA_MFP_AVC_3GPP_BL_QCIF15_AMR "AVC_3GPP_BL_QCIF15_AMR"


/* AV Class: WMV9 Profiles */
#define DLNA_MFP_WMVMED_BASE "WMVMED_BASE"
#define DLNA_MFP_WMVMED_FULL "WMVMED_FULL"
#define DLNA_MFP_WMVMED_PRO "WMVMED_PRO"
#define DLNA_MFP_WMVHIGH_FULL "WMVHIGH_FULL"
#define DLNA_MFP_WMVHIGH_PRO "WMVHIGH_PRO"
#define DLNA_MFP_WMVHM_BASE "WMVHM_BASE"
#define DLNA_MFP_WMVSPLL_BASE "WMVSPLL_BASE"
#define DLNA_MFP_WMVSPML_BASE "WMVSPML_BASE"
#define DLNA_MFP_WMVSPML_MP3 "WMVSPML_MP3"

/* Media Collection Profiles */
#define DLNA_MFP_DIDL_S "DIDL_S"
#define DLNA_MFP_DIDL_V "DIDL_V"

/* Printer XHTML Document Profiles */
#define DLNA_MFP_XHTML_ALL "XHTML_ALL"
#define DLNA_MFP_XHTML_Complex "XHTML_Complex"
#define DLNA_MFP_XHTML_PT "XHTML_PT"
#define DLNA_MFP_XHTML_Baseline "XHTML_Baseline"


/* DLNA Media Format Profile Addendum */

/* Audio Class: LPCM Profiles */
#define DLNA_MFP_LPCM_low "LPCM_low"


/* Audio Class: MPEG-4 Profiles */
#define DLNA_MFP_HEAACv2_L2 "HEAACv2_L2"
#define DLNA_MFP_HEAACv2_L2_320 "HEAACv2_L2_320"
#define DLNA_MFP_HEAACv2_L3 "HEAACv2_L3"
#define DLNA_MFP_HEAACv2_MULT5 "HEAACv2_MULT5"
#define DLNA_MFP_ALS_ISO "ALS_ISO"
#define DLNA_MFP_ALS_MULT5_ISO "ALS_MULT5_ISO"


/* AV Class: MPEG-2 Profiles */
#define DLNA_MFP_MPEG_TS_JP_T "MPEG_TS_JP_T"
#define DLNA_MFP_MPEG_TS_SD_60_L2_T "MPEG_TS_SD_60_L2_T"
#define DLNA_MFP_MPEG_TS_SD_60_AC3_T "MPEG_TS_SD_60_AC3_T"
#define DLNA_MFP_MPEG_TS_SD_50_L2_T "MPEG_TS_SD_50_L2_T"
#define DLNA_MFP_MPEG_TS_SD_50_AC3_T "MPEG_TS_SD_50_AC3_T"
#define DLNA_MFP_MPEG_TS_HD_60_L2_T "MPEG_TS_HD_60_L2_T"
#define DLNA_MFP_MPEG_TS_HD_60_L2_ISO "MPEG_TS_HD_60_L2_ISO"
#define DLNA_MFP_MPEG_TS_HD_50_L2_T "MPEG_TS_HD_50_L2_T"
#define DLNA_MFP_MPEG_TS_HD_50_L2_ISO "MPEG_TS_HD_50_L2_ISO"

/* AV Class: MPEG-4 Part 2 Profiles */
#define DLNA_MFP_MPEG4_H263_3GPP_P0_L10_AMR "MPEG4_H263_3GPP_P0_L10_AMR"
#define DLNA_MFP_MPEG4_H263_3GPP_P0_L45_AMR "MPEG4_H263_3GPP_P0_L45_AMR"
#define DLNA_MFP_MPEG4_H263_3GPP_P0_L45_AMR_WBplus "MPEG4_H263_3GPP_P0_L45_AMR_WBplus"
#define DLNA_MFP_MPEG4_P2_MP4_SP_L5_AAC "MPEG4_P2_MP4_SP_L5_AAC"

/* AV Class: MPEG-4 Part 10 (AVC) Profiles */
#define DLNA_MFP_AVC_3GPP_BL_L1B_QCIF15_AMR "AVC_3GPP_BL_L1B_QCIF15_AMR"
#define DLNA_MFP_AVC_3GPP_BL_L1B_QCIF15_AMR_WBplus "AVC_3GPP_BL_L1B_QCIF15_AMR_WBplus"
#define DLNA_MFP_AVC_TS_HP_SD_HEAACv2_L4_T "AVC_TS_HP_SD_HEAACv2_L4_T"
#define DLNA_MFP_AVC_TS_HP_SD_HEAACv2_L4_ISO "AVC_TS_HP_SD_HEAACv2_L4_ISO"
#define DLNA_MFP_AVC_MP4_HP_SD_HEAACv2_L4 "AVC_MP4_HP_SD_HEAACv2_L4"
#define DLNA_MFP_AVC_TS_HP_HD_HEAACv2_L4_T "AVC_TS_HP_HD_HEAACv2_L4_T"
#define DLNA_MFP_AVC_TS_HP_HD_HEAACv2_L4_ISO "AVC_TS_HP_HD_HEAACv2_L4_ISO"
#define DLNA_MFP_AVC_MP4_HP_HD_HEAACv2_L4 "AVC_MP4_HP_HD_HEAACv2_L4"
#define DLNA_MFP_AVC_MP4_BL_CIF15_HEAACv2_L2 "AVC_MP4_BL_CIF15_HEAACv2_L2"
#define DLNA_MFP_AVC_MP4_BL_CIF30_HEAACv2_L2 "AVC_MP4_BL_CIF30_HEAACv2_L2"
#define DLNA_MFP_AVC_3GPP_BL_CIF15_AMR_WBplus_res "AVC_3GPP_BL_CIF15_AMR_WBplus_res"
#define DLNA_MFP_AVC_3GPP_BL_CIF30_AMR_WBplus_res "AVC_3GPP_BL_CIF30_AMR_WBplus_res"


/* DLNA__Media Formats Second Addendum */

/* Audio Class: LPCM Profiles */
#define DLNA_MFP_LPCM_MPS "LPCM_MPS"


/* Audio Class: MPEG-4 Profiles */
#define DLNA_MFP_MPEG2_AAC_MPS "MPEG2_AAC_MPS"
#define DLNA_MFP_AAC_MPS "AAC_MPS"
#define DLNA_MFP_HEAAC_L4 "HEAAC_L4"
#define DLNA_MFP_HEAAC_MULT7 "HEAAC_MULT7"
#define DLNA_MFP_HEAAC_MPS "HEAAC_MPS"
#define DLNA_MFP_HEAACv2_L4 "HEAACv2_L4"
#define DLNA_MFP_HEAACv2_MULT7 "HEAACv2_MULT7"
#define DLNA_MFP_HEAACv2_L2_MPS_DAB "HEAACv2_L2_MPS_DAB"


/* AV Class: MPEG-2 Profiles */
#define DLNA_MFP_MPEG_PS_SD_DTS "MPEG_PS_SD_DTS"
#define DLNA_MFP_MPEG_PS_HD_DTS "MPEG_PS_HD_DTS"
#define DLNA_MFP_MPEG_PS_HD_DTSHD_HRA "MPEG_PS_HD_DTSHD_HRA"
#define DLNA_MFP_MPEG_PS_HD_DTSHD_MA "MPEG_PS_HD_DTSHD_MA"
#define DLNA_MFP_MPEG_PS_HD_DTSHD "MPEG_PS_HD_DTSHD"
#define DLNA_MFP_DIRECTV_TS_SD "DIRECTV_TS_SD"
#define DLNA_MFP_MPEG_TS_SD_NA_MPEG1_L2_T "MPEG_TS_SD_NA_MPEG1_L2_T"
#define DLNA_MFP_MPEG_TS_SD_NA_MPEG1_L2_ISO "MPEG_TS_SD_NA_MPEG1_L2_ISO"
#define DLNA_MFP_MPEG_TS_HD_NA_MPEG1_L2_T "MPEG_TS_HD_NA_MPEG1_L2_T"
#define DLNA_MFP_MPEG_TS_HD_NA_MPEG1_L2_ISO "MPEG_TS_HD_NA_MPEG1_L2_ISO"
#define DLNA_MFP_MPEG_TS_SD_EU_AC3_T "MPEG_TS_SD_EU_AC3_T"
#define DLNA_MFP_MPEG_TS_SD_EU_AC3_ISO "MPEG_TS_SD_EU_AC3_ISO"
#define DLNA_MFP_MPEG_TS_SD_JP_MPEG1_L2_T "MPEG_TS_SD_JP_MPEG1_L2_T"
#define DLNA_MFP_MPEG_TS_DTS_T "MPEG_TS_DTS_T"
#define DLNA_MFP_MPEG_TS_DTS_ISO "MPEG_TS_DTS_ISO"
#define DLNA_MFP_MPEG_TS_DTSHD_HRA_T "MPEG_TS_DTSHD_HRA_T"
#define DLNA_MFP_MPEG_TS_DTSHD_HRA_ISO "MPEG_TS_DTSHD_HRA_ISO"
#define DLNA_MFP_MPEG_TS_DTSHD_MA_T "MPEG_TS_DTSHD_MA_T"
#define DLNA_MFP_MPEG_TS_DTSHD_MA_ISO "MPEG_TS_DTSHD_MA_ISO"


/* AV Class: MPEG-4 Part 2 Profiles */
#define DLNA_MFP_MPEG4_H263_MP4_P0_L45_HEAACv2_L2 "MPEG4_H263_MP4_P0_L45_HEAACv2_L2"
#define DLNA_MFP_MPEG4_P2_3GPP_SP_L0B_AMR_WBplus "MPEG4_P2_3GPP_SP_L0B_AMR_WBplus"
#define DLNA_MFP_MPEG4_P2_MP4_SP_L0B_HEAACv2_L2 "MPEG4_P2_MP4_SP_L0B_HEAACv2_L2"
#define DLNA_MFP_MPEG4_P2_3GPP_SP_L3_AMR_WBplus "MPEG4_P2_3GPP_SP_L3_AMR_WBplus"
#define DLNA_MFP_MPEG4_P2_MP4_SP_L3_HEAACv2_L2 "MPEG4_P2_MP4_SP_L3_HEAACv2_L2"
#define DLNA_MFP_MPEG4_P2_MP4_SP_VGA_HEAAC_res "MPEG4_P2_MP4_SP_VGA_HEAAC_res"
#define DLNA_MFP_MPEG4_P2_MP4_SP_VGA_AAC_res "MPEG4_P2_MP4_SP_VGA_AAC_res"
#define DLNA_MFP_MPEG4_P2_MP4_SP_L6_AAC "MPEG4_P2_MP4_SP_L6_AAC"
#define DLNA_MFP_MPEG4_P2_MP4_SP_L6_AAC_LTP "MPEG4_P2_MP4_SP_L6_AAC_LTP"
#define DLNA_MFP_MPEG4_P2_MP4_SP_L6_HEAAC_L2 "MPEG4_P2_MP4_SP_L6_HEAAC_L2"
#define DLNA_MFP_MPEG4_P2_MP4_NDSD "MPEG4_P2_MP4_NDSD"


/* AV Class: MPEG-4 Part 10 (AVC) Profiles */
#define DLNA_MFP_AVC_PS_HD_DTS "AVC_PS_HD_DTS"
#define DLNA_MFP_AVC_PS_HD_DTSHD_HRA "AVC_PS_HD_DTSHD_HRA"
#define DLNA_MFP_AVC_PS_HD_DTSHD_MA "AVC_PS_HD_DTSHD_MA"
#define DLNA_MFP_AVC_PS_HD_DTSHD "AVC_PS_HD_DTSHD"
#define DLNA_MFP_AVC_TS_MP_SD_EAC3_T "AVC_TS_MP_SD_EAC3_T"
#define DLNA_MFP_AVC_TS_MP_SD_EAC3_ISO "AVC_TS_MP_SD_EAC3_ISO"
#define DLNA_MFP_AVC_TS_HP_SD_AC3_T "AVC_TS_HP_SD_AC3_T"
#define DLNA_MFP_AVC_TS_HP_SD_AC3_ISO "AVC_TS_HP_SD_AC3_ISO"
#define DLNA_MFP_AVC_TS_HP_SD_EAC3_T "AVC_TS_HP_SD_EAC3_T"
#define DLNA_MFP_AVC_TS_HP_SD_EAC3_ISO "AVC_TS_HP_SD_EAC3_ISO"
#define DLNA_MFP_AVC_TS_HP_SD_MPEG1_L2_T "AVC_TS_HP_SD_MPEG1_L2_T"
#define DLNA_MFP_AVC_TS_HP_SD_MPEG1_L2_ISO "AVC_TS_HP_SD_MPEG1_L2_ISO"
#define DLNA_MFP_AVC_TS_SD_EU "AVC_TS_SD_EU"
#define DLNA_MFP_AVC_TS_SD_EU_T "AVC_TS_SD_EU_T"
#define DLNA_MFP_AVC_TS_SD_EU_ISO "AVC_TS_SD_EU_ISO"
#define DLNA_MFP_AVC_TS_MP_HD_EAC3_T "AVC_TS_MP_HD_EAC3_T"
#define DLNA_MFP_AVC_TS_MP_HD_EAC3_ISO "AVC_TS_MP_HD_EAC3_ISO"
#define DLNA_MFP_AVC_TS_HP_HD_AC3_T "AVC_TS_HP_HD_AC3_T"
#define DLNA_MFP_AVC_TS_HP_HD_AC3_ISO "AVC_TS_HP_HD_AC3_ISO"
#define DLNA_MFP_AVC_TS_HP_HD_EAC3_T "AVC_TS_HP_HD_EAC3_T"
#define DLNA_MFP_AVC_TS_HP_HD_EAC3_ISO "AVC_TS_HP_HD_EAC3_ISO"
#define DLNA_MFP_AVC_TS_HP_HD_MPEG1_L2_T "AVC_TS_HP_HD_MPEG1_L2_T"
#define DLNA_MFP_AVC_TS_HP_HD_MPEG1_L2_ISO "AVC_TS_HP_HD_MPEG1_L2_ISO"
#define DLNA_MFP_AVC_TS_HD_EU "AVC_TS_HD_EU"
#define DLNA_MFP_AVC_TS_HD_EU_T "AVC_TS_HD_EU_T"
#define DLNA_MFP_AVC_TS_HD_EU_ISO "AVC_TS_HD_EU_ISO"
#define DLNA_MFP_AVC_TS_JP_AAC_T "AVC_TS_JP_AAC_T"
#define DLNA_MFP_AVC_TS_HD_60_AC3 "AVC_TS_HD_60_AC3"
#define DLNA_MFP_AVC_TS_HD_60_AC3_T "AVC_TS_HD_60_AC3_T"
#define DLNA_MFP_AVC_TS_HD_60_AC3_ISO "AVC_TS_HD_60_AC3_ISO"
#define DLNA_MFP_AVC_TS_HD_50_AC3 "AVC_TS_HD_50_AC3"
#define DLNA_MFP_AVC_TS_HD_50_AC3_T "AVC_TS_HD_50_AC3_T"
#define DLNA_MFP_AVC_TS_HD_50_AC3_ISO "AVC_TS_HD_50_AC3_ISO"
#define DLNA_MFP_AVC_TS_HD_24_AC3 "AVC_TS_HD_24_AC3"
#define DLNA_MFP_AVC_TS_HD_24_AC3_T "AVC_TS_HD_24_AC3_T"
#define DLNA_MFP_AVC_TS_HD_24_AC3_ISO "AVC_TS_HD_24_AC3_ISO"
#define DLNA_MFP_AVC_TS_HD_DTS_T "AVC_TS_HD_DTS_T"
#define DLNA_MFP_AVC_TS_HD_DTS_ISO "AVC_TS_HD_DTS_ISO"
#define DLNA_MFP_AVC_TS_HD_DTSHD_HRA_T "AVC_TS_HD_DTSHD_HRA_T"
#define DLNA_MFP_AVC_TS_HD_DTSHD_HRA_ISO "AVC_TS_HD_DTSHD_HRA_ISO"
#define DLNA_MFP_AVC_TS_DTSHD_MA_T "AVC_TS_DTSHD_MA_T"
#define DLNA_MFP_AVC_TS_DTSHD_MA_ISO "AVC_TS_DTSHD_MA_ISO"
#define DLNA_MFP_AVC_MP4_BL_L1B_QCIF15_HEAACv2 "AVC_MP4_BL_L1B_QCIF15_HEAACv2"
#define DLNA_MFP_AVC_MP4_BL_L12_CIF15_HEAACv2 "AVC_MP4_BL_L12_CIF15_HEAACv2"
#define DLNA_MFP_AVC_MP4_BL_CIF30_HEAAC_MPS "AVC_MP4_BL_CIF30_HEAAC_MPS"
#define DLNA_MFP_AVC_MP4_MP_SD_AAC_MPS "AVC_MP4_MP_SD_AAC_MPS"
#define DLNA_MFP_AVC_MP4_MP_SD_HEAAC_MPS "AVC_MP4_MP_SD_HEAAC_MPS"
#define DLNA_MFP_AVC_MP4_MP_SD_HEAAC_L4 "AVC_MP4_MP_SD_HEAAC_L4"
#define DLNA_MFP_AVC_MP4_MP_HD_AAC_MPS "AVC_MP4_MP_HD_AAC_MPS"
#define DLNA_MFP_AVC_MP4_MP_HD_HEAAC_MPS "AVC_MP4_MP_HD_HEAAC_MPS"
#define DLNA_MFP_AVC_MP4_MP_HD_720p_AAC "AVC_MP4_MP_HD_720p_AAC"
#define DLNA_MFP_AVC_MP4_MP_HD_1080i_AAC "AVC_MP4_MP_HD_1080i_AAC"
#define DLNA_MFP_AVC_MP4_HP_HD_AAC "AVC_MP4_HP_HD_AAC"
#define DLNA_MFP_AVC_MP4_HP_HD_AAC_LTP "AVC_MP4_HP_HD_AAC_LTP"
#define DLNA_MFP_AVC_MP4_HP_HD_HEAAC_L2 "AVC_MP4_HP_HD_HEAAC_L2"
#define DLNA_MFP_AVC_MP4_HP_HD_HEAAC_MULT7 "AVC_MP4_HP_HD_HEAAC_MULT7"
#define DLNA_MFP_AVC_MP4_NDHD "AVC_MP4_NDHD"
#define DLNA_MFP_AVC_3GPP_BL_L12_CIF15_AMR_WBplus "AVC_3GPP_BL_L12_CIF15_AMR_WBplus"

/* Image Class: GIF Profiles */
#define DLNA_MFP_GIF_LRG "GIF_LRG"

/* Audio Class: DTS Digital Surround Profiles */
#define DLNA_MFP_DTS "DTS"

/* Audio Class: DTS-HD Profiles */
#define DLNA_MFP_DTSHD_HRA "DTSHD_HRA"
#define DLNA_MFP_DTSHD_MA "DTSHD_MA"
#define DLNA_MFP_DTSHD_LBR "DTSHD_LBR"

/* Audio Class: Enhanced AC-3 Profiles */
#define DLNA_MFP_EAC3 "EAC3"

/* Audio Class: MLP Profiles */
#define DLNA_MFP_MLP "MLP"

/* Audio Class: MPEG-1/2 Profiles */
#define DLNA_MFP_MP2_MPS "MP2_MPS"

/* AV Class: VC-1 Profiles */
#define DLNA_MFP_VC1_PS_HD_DTS "VC1_PS_HD_DTS"
#define DLNA_MFP_VC1_PS_HD_DTSHD_HRA "VC1_PS_HD_DTSHD_HRA"
#define DLNA_MFP_VC1_PS_HD_DTSHD_MA "VC1_PS_HD_DTSHD_MA"
#define DLNA_MFP_VC1_PS_HD_DTSHD "VC1_PS_HD_DTSHD"
#define DLNA_MFP_VC1_TS_AP_L1_AC3_ISO "VC1_TS_AP_L1_AC3_ISO"
#define DLNA_MFP_VC1_TS_AP_L1_WMA_ISO "VC1_TS_AP_L1_WMA_ISO"
#define DLNA_MFP_VC1_TS_AP_L2_AC3_ISO "VC1_TS_AP_L2_AC3_ISO"
#define DLNA_MFP_VC1_TS_AP_L2_WMA_ISO "VC1_TS_AP_L2_WMA_ISO"
#define DLNA_MFP_VC1_TS_HD_DTS_T "VC1_TS_HD_DTS_T"
#define DLNA_MFP_VC1_TS_HD_DTS_ISO "VC1_TS_HD_DTS_ISO"
#define DLNA_MFP_VC1_TS_HD_DTSHD_HRA_T "VC1_TS_HD_DTSHD_HRA_T"
#define DLNA_MFP_VC1_TS_HD_DTSHD_HRA_ISO "VC1_TS_HD_DTSHD_HRA_ISO"
#define DLNA_MFP_VC1_TS_HD_DTSHD_MA_T "VC1_TS_HD_DTSHD_MA_T"
#define DLNA_MFP_VC1_TS_HD_DTSHD_MA_ISO "VC1_TS_HD_DTSHD_MA_ISO"
#define DLNA_MFP_VC1_ASF_AP_L1_WMA "VC1_ASF_AP_L1_WMA"
#define DLNA_MFP_VC1_ASF_AP_L2_WMA "VC1_ASF_AP_L2_WMA"

/* DLNA__Media Formats Third Addendum */

/* Audio Class: MPEG-4 Profiles */
#define DLNA_MFP_AAC_ADTS_192 "AAC_ADTS_192"
#define DLNA_MFP_AAC_ISO_192 "AAC_ISO_192"
#define DLNA_MFP_HEAACv2_L2_128 "HEAACv2_L2_128"
#define DLNA_MFP_HEAAC_L2_ISO_128 "HEAAC_L2_ISO_128"

/* Audio Class: WMA Profiles */
#define DLNA_MFP_WMALSL "WMALSL"
#define DLNA_MFP_WMALSL_MULT5 "WMALSL_MULT5"

/* AV Class: MPEG-2 Profiles */
#define DLNA_MFP_MPEG_TS_HD_X_60_L2_T "MPEG_TS_HD_X_60_L2_T"
#define DLNA_MFP_MPEG_TS_HD_X_60_L2_ISO "MPEG_TS_HD_X_60_L2_ISO"
#define DLNA_MFP_MPEG_TS_HD_X_50_L2_T "MPEG_TS_HD_X_50_L2_T"
#define DLNA_MFP_MPEG_TS_HD_X_50_L2_ISO "MPEG_TS_HD_X_50_L2_ISO"

/* AV Class: MPEG-4 Part 10 (AVC) Profiles */
#define DLNA_MFP_AVC_TS_HD_60_AC3_X_T "AVC_TS_HD_60_AC3_X_T"
#define DLNA_MFP_AVC_TS_HD_50_AC3_X_T "AVC_TS_HD_50_AC3_X_T"
#define DLNA_MFP_AVC_TS_HD_24_AC3_X_T "AVC_TS_HD_24_AC3_X_T"
#define DLNA_MFP_AVC_TS_HD_60_LPCM_T "AVC_TS_HD_60_LPCM_T"
#define DLNA_MFP_AVC_TS_HD_50_LPCM_T "AVC_TS_HD_50_LPCM_T"
#define DLNA_MFP_AVC_TS_HD_24_LPCM_T "AVC_TS_HD_24_LPCM_T"
#define DLNA_MFP_AVC_MP4_BL_L2_CIF15_HEAACv2_350 "AVC_MP4_BL_L2_CIF15_HEAACv2_350"
#define DLNA_MFP_AVC_MP4_BL_CIF15_AAC_350 "AVC_MP4_BL_CIF15_AAC_350"
#define DLNA_MFP_AVC_MP4_BL_CIF15_HEAAC_350 "AVC_MP4_BL_CIF15_HEAAC_350"
#define DLNA_MFP_AVC_MP4_MP_SD_AAC_LC "AVC_MP4_MP_SD_AAC_LC"
#define DLNA_MFP_AVC_MP4_BL_L31_HD_AAC "AVC_MP4_BL_L31_HD_AAC"
#define DLNA_MFP_AVC_MP4_BL_L32_HD_AAC "AVC_MP4_BL_L32_HD_AAC"


/* DTCP media format profile prefix */
#ifdef HAVE_DTCP
#define DLNA_MFP_DTCP_PREFIX "DTCP_"
#endif


/* WMDRM media format profile prefix */
#ifdef HAVE_WMDRM 
#define DLNA_MFP_WMDRM_PREFIX "WMDRM_"
#endif

#endif

/*! @} */
