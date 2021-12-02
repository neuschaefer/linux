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
 * $RCSfile: drv_comp_id.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file drv_comp_id.h
 *  Brief of file drv_comp_id.h.
 *  Details of file drv_comp_id.h (optional).
 */

#ifndef DRV_COMP_ID_H
#define DRV_COMP_ID_H


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "fbm_drvif.h"
#include "vdp_drvif.h"
#include "pmx_if.h"
#include "osd_if.h"
#include "mpv_if.h"
#include "stc_drvif.h"
#include "d_drv_cust.h"

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/* DMX Component ID */
// PCR: 0 - 3
#define DMX_PCR_COMP_NUM                    (4)  /**/
#define DMX_PCR_COMP_ID_START               (0)  /**/
#define DMX_PCR_COMP_ID_END                 ((DMX_PCR_COMP_ID_START + DMX_PCR_COMP_NUM) - 1)

// PSI + PES = 19 + 3 = 22
#ifndef DMX_PES_MEM_COMP_NUM
#define DMX_PES_MEM_COMP_NUM                (3)
#endif

// PSI: original (4 - 22), new use 22 - PES num
#define DMX_SECTION_MEM_COMP_NUM            (22 - DMX_PES_MEM_COMP_NUM)    // PSI = 22
#define DMX_SECTION_MEM_COMP_ID_START       (DMX_PCR_COMP_ID_END + 1)
#define DMX_SECTION_MEM_COMP_ID_END \
        ((DMX_SECTION_MEM_COMP_ID_START + DMX_SECTION_MEM_COMP_NUM) - 1)

// PES memory: 23 - 25
#define DMX_PES_MEM_COMP_ID_START           (DMX_SECTION_MEM_COMP_ID_END + 1)
#define DMX_PES_MEM_COMP_ID_END             ((DMX_PES_MEM_COMP_ID_START + DMX_PES_MEM_COMP_NUM) - 1)

// PES: 26 - 31
#define DMX_PES_COMP_NUM                    (6) /**/
#define DMX_PES_COMP_ID_START               (DMX_PES_MEM_COMP_ID_END + 1)  /* 4 */
#define DMX_PES_COMP_ID_END                 ((DMX_PES_COMP_ID_START + DMX_PES_COMP_NUM) - 1) /**/

#define DMX_SECTION_FILTER_COMP_NUM         32
#define DMX_SECTION_FILTER_COMP_ID_START    0
#define DMX_SECTION_FILTER_COMP_ID_END \
        ((DMX_SECTION_FILTER_COMP_ID_START + DMX_SECTION_FILTER_COMP_NUM) - 1)

// Container_ES: 32-34
#define DMX_MM_CONTAINER_ES_COMP_NUM         (3)
#define DMX_MM_CONTAINER_ES_COMP_ID_START    (DMX_SECTION_MEM_COMP_ID_END + 1)
#define DMX_MM_CONTAINER_ES_COMP_ID_END \
        ((DMX_MM_CONTAINER_ES_COMP_ID_START + DMX_MM_CONTAINER_ES_COMP_NUM) - 1)

// Demux Control: 35
#define DMX_MM_CONTROL_COMP_NUM              (3)
#define DMX_MM_CONTROL_COMP_ID_START    (DMX_MM_CONTAINER_ES_COMP_ID_END + 1)
#define DMX_MM_CONTROL_COMP_ID_END \
        ((DMX_MM_CONTROL_COMP_ID_START + DMX_MM_CONTROL_COMP_NUM) - 1)
#define DMX_MM_CONTROL_COMP_ID \
	(DMX_MM_CONTAINER_ES_COMP_ID_END + 1)

// Container_ES_Memory: 36-37
#define DMX_MM_CONTAINER_ES_MEM_COMP_NUM      (2)
#define DMX_MM_CONTAINER_ES_MEM_COMP_ID_START (DMX_MM_CONTROL_COMP_ID + 1)
#define DMX_MM_CONTAINER_ES_MEM_COMP_ID_END \
        ((DMX_MM_CONTAINER_ES_MEM_COMP_ID_START + DMX_MM_CONTAINER_ES_MEM_COMP_NUM) - 1)

// Record DMX
#define REC_DMX_COMP_NUM                     30
#define REC_DMX_COMP_ID_START                0
#define REC_DMX_COMP_ID_END \
        ((REC_DMX_COMP_ID_START + REC_DMX_COMP_NUM) - 1)

// Record DMX PCR
#define REC_DMX_PCR_COMP_NUM                 2
#define REC_DMX_PCR_COMP_ID_START            0

// PVR
#define PVR_CTRL_COMP_NUM                    1
#define PVR_CTRL_COMP_ID_START               0

#define PVR_STREAM_COMP_NUM                  (REC_DMX_COMP_NUM)
#define PVR_STREAM_COMP_ID_START             0

#define PVR_STREAM_PCR_COMP_NUM              (REC_DMX_PCR_COMP_NUM)
#define PVR_STREAM_PCR_COMP_ID_START         0

// Descrambler: 0-31 (total virtual 32 avaliable key)
#define DMX_DESC_COMP_NUM              (32)
#define DMX_DESC_COMP_ID_START    0
#define DMX_DESC_COMP_ID_END \
        ((DMX_DESC_COMP_ID_START + DMX_DESC_COMP_NUM) - 1)
        
/* MPV Component ID */
#define MPV_COMP_SD_NS  4

typedef enum
{
    MPV_COMP_1,
    MPV_COMP_2,
    MPV_COMP_3,
    MPV_COMP_4,
    MPV_COMP_5,
    MPV_COMP_NS,
} MPV_COMP_ID_T;

/* AUD Component ID */
#ifdef CC_AUD_4_DECODER_SUPPORT
#define AUD_COMP_NS             4
#define AUD_COMP_1              0
#define AUD_COMP_2              1
#define	AUD_COMP_3              2
#define	AUD_COMP_4              3
#elif defined (CC_MT5391_AUD_3_DECODER)
#define AUD_COMP_NS             3
#define AUD_COMP_1              0
#define AUD_COMP_2              1
#define	AUD_COMP_3              2
#else
#define AUD_COMP_NS             2
#define AUD_COMP_1              0
#define AUD_COMP_2              1
#endif

/* PCR Component ID */
#define PCR_COMP_NS                 STC_SRC_NS
#define PCR_COMP_1                  STC_SRC_A1
#define PCR_COMP_2                  STC_SRC_A2

/* VDP Component ID */
#define VDP_COMP_NS                 VDP_NS
#define VDP_COMP_1                  VDP_1
#define VDP_COMP_2                  VDP_2
#define VDP_COMP_3                  VDP_3   // in the future, virtual vdp with osd
#define VDP_COMP_4                  VDP_4   // in the future, virtual vdp with osd
#define VDP_COMP_5                  VDP_5   // in the future, virtual vdp with osd

/* PMX Component ID */
#define PMX_COMP_NS                 PMX_MAX_NS
#define PMX_COMP_1                  PMX_1

/* OSD Component ID */
#define OSD_COMP_NS                 OSD_PLANE_MAX_NUM
typedef enum _OSD_COMP_ID_T
{
    OSD_COMP_1,
    OSD_COMP_2,
    OSD_COMP_3,
    OSD_COMP_4,                    // in the future, virtual osd with vdp
    OSD_COMP_5,                    // in the future, virtual osd with vdp
    OSD_COMP_V_NS
} OSD_COMP_ID_T;

/*
#define DRVT_AVC_COMP_VIDEO             ((DRV_TYPE_T) 256)
#define DRVT_AVC_S_VIDEO                ((DRV_TYPE_T) 257)
#define DRVT_AVC_Y_PB_PR                ((DRV_TYPE_T) 258)
#define DRVT_AVC_VGA                    ((DRV_TYPE_T) 259)
#define DRVT_AVC_SCART                  ((DRV_TYPE_T) 260)
#define DRVT_AVC_DVI                    ((DRV_TYPE_T) 261)
#define DRVT_AVC_HDMI                   ((DRV_TYPE_T) 262)
#define DRVT_AVC_AUDIO_INP              ((DRV_TYPE_T) 263)
#define DRVT_AVC_SPDIF                  ((DRV_TYPE_T) 264)
#define DRVT_AVC_COMBI                  ((DRV_TYPE_T) 265)
*/

/* AV Component Number */
#define AVC_COMP_VIDEO_COMP_NUM         (3)
#define AVC_S_VIDEO_COMP_NUM            (3)
#define AVC_Y_PB_PR_COMP_NUM            (3)
#define AVC_VGA_COMP_NUM                (2)
#define AVC_HDMI_COMP_NUM               (4)
#define AVC_SPDIF_COMP_NUM              (2)
#define AVC_AUDIO_INP_NUM               (20)

/* TV Decoder Number */
#ifdef CC_SUPPORT_TVE
    #define TVD_COMP_NS                 4
#else
    #define TVD_COMP_NS                 2
#endif /*CC_SUPPORT_TVE*/

#define TVD_COMP_ID_START           0

/* Analog Tuner Number */
#define TUNER_ANA_COMP_NUM              (1)

/* JPG Component ID */
#define JPG_COMP_NS                     1
#define JPG_COMP_1                      0

#ifdef CC_SUPPORT_TVE
/* Video OUT Component ID*/
#define VDO_OUT_COMP_NS       2
#define VDO_OUT_COMP_1        0

/* TVE Component ID */
#define TVENC_COMP_NS                     2
#define TVENC_COMP_1                      0
#endif /*CC_SUPPORT_TVE*/

/* SCART_0 Component ID */
#define SCART_COMP_NS                   1
#define SCART_COMP_1                    (AVC_SCART_TYPE_SOURCE_SINK + 0)

/* Buffer Agent Component ID */
/*
#define BAGT_COMP_NS                   2
#define BAGT_COMP_2                    1
#define BAGT_COMP_1                    0
*/
typedef enum _BAGT_COMP_ID_T
{
	BAGT_COMP_1 = 0,
	BAGT_COMP_2,
	BAGT_COMP_3,
	BAGT_COMP_4,
	BAGT_COMP_5,	
	BAGT_COMP_NS
}BAGT_COMP_ID_T;


/* STC Control Component ID*/
#define STC_CTRL_COMP_NS               1
#define STC_CTRL_COMP_1                0


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#define MPV_HD(compid) \
        (compid >= MPV_COMP_5)


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

EXTERN UCHAR MW_MPV_CompIdtoVldId(UINT16 u2CompId, UINT32 u4ChannelId);
EXTERN void MW_VDP_SetConnect(UCHAR ucVdpId, UCHAR ucConnect);
EXTERN void MW_VDP_Comp2EsInd(UCHAR ucCompId, UCHAR ucEsId);
EXTERN UINT32 MW_VDP_Comp2Vdp(UCHAR ucCompId);
EXTERN void MW_OsdConnect(INT32 hOsdPlane, INT32 fgEnable);

#endif  // DRV_COMP_ID_H

