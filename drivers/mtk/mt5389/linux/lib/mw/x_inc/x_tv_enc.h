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
 * $RCSfile: x_tv_enc.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains video decoder driver specific
 *         definitions, which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_TV_ENC_H_
#define _X_TV_ENC_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_common.h"
#include "x_rm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/


/* Get operations */
#define TV_ENC_GET_TYPE_CTRL         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define TV_ENC_GET_TYPE_FMT          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))
#define TV_ENC_GET_TYPE_FMT_CAP      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))
#define TV_ENC_GET_TYPE_TV_TYPE      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 3))
#define TV_ENC_GET_TYPE_MV           (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 4))
#define TV_ENC_GET_TYPE_CCI          (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 5))
#define TV_ENC_GET_TYPE_ENC_CAP      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 6))   

/* Set operations */
#define TV_ENC_SET_TYPE_CTRL        ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0)) | RM_SET_TYPE_ARG_NO_REF)
#define TV_ENC_SET_TYPE_FMT         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1)) | RM_SET_TYPE_ARG_NO_REF)
#define TV_ENC_SET_TYPE_VBI_CONTENT  (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2))
#define TV_ENC_SET_TYPE_TV_TYPE     ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3)) | RM_SET_TYPE_ARG_NO_REF)
#define TV_ENC_SET_TYPE_MV           (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4))
#define TV_ENC_SET_TYPE_CCI         ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5)) | RM_SET_TYPE_ARG_NO_REF)
#define TV_ENC_SET_TYPE_ENABLE_EU_VBI_FIELD_DATA_GEN    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6))
#define TV_ENC_SET_TYPE_EU_VBI_FIELD_DATA               ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 7)) | RM_SET_TYPE_ARG_NO_REF)

/* TV_ENC_GET_TYPE_CTRL/TV_ENC_SET_TYPE_CTRL data info ***********************/
/* Control settings. */
typedef enum
{
    TV_ENC_CTRL_RESET       = 0,
    TV_ENC_CTRL_DISABLE     = 0x1,  /* disable TV encoder */
    TV_ENC_CTRL_ENABLE      = 0x2   /* enable TV encoder */
}   TV_ENC_CTRL_T;

/* TV_ENC_GET_TYPE_FMT *******************************************************/
/* TV output format */
typedef enum
{
    TV_ENC_FMT_NULL = 0,
    TV_ENC_FMT_CVBS,
    TV_ENC_FMT_S_VIDEO,
    TV_ENC_FMT_YBR,
    TV_ENC_FMT_RGB
}   TV_ENC_FMT_T;

/* TV_ENC_GET_TYPE_FMT_CAP ***************************************************/
/* Capability of TV output formats */
#define TV_ENC_FMT_CAP_NULL			(((UINT32)1) << TV_ENC_FMT_NULL)
#define TV_ENC_FMT_CAP_CVBS			(((UINT32)1) << TV_ENC_FMT_CVBS)
#define TV_ENC_FMT_CAP_S_VIDEO		(((UINT32)1) << TV_ENC_FMT_S_VIDEO)
#define TV_ENC_FMT_CAP_YBR			(((UINT32)1) << TV_ENC_FMT_YBR)
#define TV_ENC_FMT_CAP_RGB			(((UINT32)1) << TV_ENC_FMT_RGB)

typedef UINT32 TV_ENC_FMT_CAP_INFO_T;

/* Capability of TV encoding contents */
#define TV_ENC_ENC_CAP_VBI          ((UINT32) 0x1)

typedef UINT32 TV_ENC_ENC_CAP_INFO_T;

/* TV_ENC_SET_TYPE_VBI_CONTENT data info *************************************/
/* Field types */
typedef enum
{
    TV_ENC_FIELD_ODD = 0,
    TV_ENC_FIELD_EVEN,
    TV_ENC_FIELD_BOTH
}   TV_ENC_FIELD_TYPE_T;

/* VBI content info */
typedef struct _TV_ENC_VBI_CONTENT_INFO_T
{
    TV_ENC_FIELD_TYPE_T e_field;
    CHAR *              ps_content;
}   TV_ENC_VBI_CONTENT_INFO_T;

/* TV_ENC_GET_TYPE_ASPECT_RATIO/TV_ENC_SET_TYPE_ASPECT_RATIO data info *******/
typedef enum
{
    TV_ENC_ASPECT_RATIO_4_3,
    TV_ENC_ASPECT_RATIO_14_9,
    TV_ENC_ASPECT_RATIO_16_9
}   TV_ENC_ASPECT_RATIO_INFO_T;

/* TV_ENC_GET_TYPE_TV_TYPE/TV_ENC_SET_TYPE_TV_TYPE data info *****************/
typedef enum
{
    TV_ENC_TV_TYPE_NTSC_M,
    TV_ENC_TV_TYPE_PAL_B,
    TV_ENC_TV_TYPE_PAL_G,
    TV_ENC_TV_TYPE_PAL_H,
    TV_ENC_TV_TYPE_PAL_I,
    TV_ENC_TV_TYPE_PAL_D,
    TV_ENC_TV_TYPE_PAL_N,
    TV_ENC_TV_TYPE_PAL_M,
    TV_ENC_TV_TYPE_SECAM_B,
    TV_ENC_TV_TYPE_SECAM_G,
    TV_ENC_TV_TYPE_SECAM_H,
    TV_ENC_TV_TYPE_SECAM_D,
    TV_ENC_TV_TYPE_SECAM_K,
    TV_ENC_TV_TYPE_SECAM_K1,
    TV_ENC_TV_TYPE_SECAM_L
}   TV_ENC_TV_TYPE_INFO_T;

/* TV_ENC_GET_TYPE_MV/TV_ENC_SET_TYPE_MV data info ***************************/
#define TV_ENC_MV_CPS_DATA_SIZE      ((SIZE_T) 17)

typedef enum
{
    TV_ENC_MV_TYPE_OFF                       = 0,
    TV_ENC_MV_TYPE_APS                       = 1,
    TV_ENC_MV_TYPE_CPC_CPS                   = 2
}   TV_ENC_MV_TYPE_T;

typedef enum
{
    TV_ENC_MV_APS_TYPE_OFF                   = 0,
    TV_ENC_MV_APS_TYPE_AGC_ONLY              = 1,
    TV_ENC_MV_APS_TYPE_AGC_WI_2L_SPLIT_BURST = 2,
    TV_ENC_MV_APS_TYPE_AGC_WI_4L_SPLIT_BURST = 3
}   TV_ENC_MV_APS_TYPE_T;

typedef struct _TV_ENC_MV_CPC_CPS_T
{
    UINT8               ui1_cpc_data;
    UINT8               aui1_cps_data[TV_ENC_MV_CPS_DATA_SIZE];
}   TV_ENC_MV_CPC_CPS_T;

typedef struct _TV_ENC_MV_INFO_T
{
    TV_ENC_MV_TYPE_T    e_mv_type;

    union
    {
        TV_ENC_MV_APS_TYPE_T     e_aps_type;
        TV_ENC_MV_CPC_CPS_T      t_cpc_cps_data;
    } u;
} TV_ENC_MV_INFO_T;

/* TV_ENC_GET_TYPE_CCI/TV_ENC_SET_TYPE_CCI data info *************************/
typedef enum
{
    TV_ENC_CCI_TYPE_NO_RESTRICT      = 0,
    TV_ENC_CCI_TYPE_NO_FURTHER_COPY  = 1,
    TV_ENC_CCI_TYPE_COPY_ONCE        = 2,
    TV_ENC_CCI_TYPE_COPY_NEVER       = 3
}   TV_ENC_CCI_TYPE_T;

typedef UINT32 TV_ENC_CCI_DATA_INFO_T;

/* TV_ENC_SET_TYPE_EU_VBI_FIELD_DATA data info *******************************/
typedef struct _TV_ENC_EU_VBI_DATA_TTX_LINE_T
{
    UINT8                   ui1_field_parity;   /* 1:odd, 0:even (same as 4.5.2 ETSI EN 301 775) */
    UINT8                   ui1_line_offset;    /* definition same as Table 5, ETSI EN 301 775 */
    UINT8                   ui1_framing_code;   /* definition same as 4.5.2 ETSI EN 301 775 */
    UINT8                   aui1_data[42];      
    
} TV_ENC_EU_VBI_DATA_TTX_LINE_T;

#define TV_ENC_TTX_FIELD_MAX_LINE_NUM    40
typedef struct _TV_ENC_EU_VBI_DATA_TTX_T
{
    UINT32                          ui4_num_line;
    TV_ENC_EU_VBI_DATA_TTX_LINE_T   at_lines_data[TV_ENC_TTX_FIELD_MAX_LINE_NUM];
    
} TV_ENC_EU_VBI_DATA_TTX_T;

typedef struct _TV_ENC_EU_VBI_DATA_VPS_T
{
    /* the VPS 13 bytes data must be at line 16 of the first field */
    UINT8                  aui1_vps_data[13];
    
} TV_ENC_EU_VBI_DATA_VPS_T;

typedef struct _TV_ENC_EU_VBI_DATA_WSS_T
{
    /* the WSS 14 bits data must be at line 23 of the first field */
    /* the bit 0 corresponds to the left-most bit of wss_data_block */
    UINT16                 ui2_wss;
    
} TV_ENC_EU_VBI_DATA_WSS_T;

typedef struct _TV_ENC_EU_VBI_DATA_CC_T
{
    /* 1:odd, 0:even (same as 4.5.2 ETSI EN 301 775) */
    UINT8                   ui1_field_parity;   
    
    /* the CC data must be at line 21 of the first field in 525-line video */
    /* CC bit b0 of character one corresponds to the left-most bit of ui2_cc_data */
    UINT16                  ui2_cc_data;
    
} TV_ENC_EU_VBI_DATA_CC_T;

/* TV_ENC_EU_VBI_FIELD_DATA_T.ui4_flag */
#define TV_ENC_EU_VBI_DATA_FLAG_TTX         MAKE_BIT_MASK_32(0)
#define TV_ENC_EU_VBI_DATA_FLAG_VPS         MAKE_BIT_MASK_32(1)
#define TV_ENC_EU_VBI_DATA_FLAG_WSS         MAKE_BIT_MASK_32(2)
#define TV_ENC_EU_VBI_DATA_FLAG_CC          MAKE_BIT_MASK_32(3)

/* TV_ENC_EU_VBI_FIELD_DATA_T.i4_result_ttx */
#define TV_ENC_EU_VBI_DATA_RESULT_TTX_OK         ((INT32)  0)
#define TV_ENC_EU_VBI_DATA_RESULT_TTX_OVERFLOW   ((INT32) -1)
#define TV_ENC_EU_VBI_DATA_RESULT_TTX_ERROR      ((INT32) -2)

/* TV_ENC_EU_VBI_FIELD_DATA_T.i4_result_vps */
#define TV_ENC_EU_VBI_DATA_RESULT_VPS_OK         ((INT32)  0)
#define TV_ENC_EU_VBI_DATA_RESULT_VPS_ERROR      ((INT32) -1)

/* TV_ENC_EU_VBI_FIELD_DATA_T.i4_result_wss */
#define TV_ENC_EU_VBI_DATA_RESULT_WSS_OK         ((INT32)  0)
#define TV_ENC_EU_VBI_DATA_RESULT_WSS_ERROR      ((INT32) -1)

/* TV_ENC_EU_VBI_FIELD_DATA_T.i4_result_cc */
#define TV_ENC_EU_VBI_DATA_RESULT_CC_OK          ((INT32)  0)
#define TV_ENC_EU_VBI_DATA_RESULT_CC_ERROR       ((INT32) -1)

typedef struct _TV_ENC_EU_VBI_FIELD_DATA_T
{
    PTS_T                       ui8_pts;            /* in */
    UINT32                      ui4_flag;           /* in */
    
    TV_ENC_EU_VBI_DATA_TTX_T    t_ttx;              /* in */
    TV_ENC_EU_VBI_DATA_VPS_T    t_vps;              /* in */
    TV_ENC_EU_VBI_DATA_WSS_T    t_wss;              /* in */
    TV_ENC_EU_VBI_DATA_CC_T     t_cc;               /* in */

    /* process result of each data type */
    INT32                       i4_result_ttx;      /* out */
    INT32                       i4_result_vps;      /* out */
    INT32                       i4_result_wss;      /* out */
    INT32                       i4_result_cc;       /* out */
    
} TV_ENC_EU_VBI_FIELD_DATA_T;    


#ifndef MW_TV_ENC_SCART_1
#define TV_ENC_SCART_1        (0)
#else
#define TV_ENC_SCART_1        (MW_TV_ENC_SCART_1)
#endif

#ifndef MW_TV_ENC_SCART_2
#define TV_ENC_SCART_2        (1)
#else
#define TV_ENC_SCART_2        (MW_TV_ENC_SCART_2)
#endif


#endif /* _X_TV_ENC_H_ */
