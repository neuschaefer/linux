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
 * $RCSfile: u_scte_si_eng.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: 0d036d6863c3fb51c7e6cad0171240c6 $
 *
 * Description:
 *         This header file contains SCTE-SI engine specific definitions,
 *         which are exported.
 *----------------------------------------------------------------------------*/

#ifndef _U_SCTE_SI_H_
#define _U_SCTE_SI_H_

/*-----------------------------------------------------------------------------
                    include files
 -----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_handle_grp.h"
#include "u_tm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 -----------------------------------------------------------------------------*/
#define SCTE_SI_DEFAULT_NAME        "scte_si_engine"

/* SCTE-SI engine handle types */
#define TMT_SCTE_NIT_CDS            (TMT_SCTE + ((HANDLE_TYPE_T)  0)) /* 449 */
#define TMT_SCTE_NIT_MMS            (TMT_SCTE + ((HANDLE_TYPE_T)  1))
#define TMT_SCTE_NTT_SNS            (TMT_SCTE + ((HANDLE_TYPE_T)  2))
#define TMT_SCTE_SVCT_VCM           (TMT_SCTE + ((HANDLE_TYPE_T)  3))
#define TMT_SCTE_SVCT_DCM           (TMT_SCTE + ((HANDLE_TYPE_T)  4))
#define TMT_SCTE_SVCT_ICM           (TMT_SCTE + ((HANDLE_TYPE_T)  5))
#define TMT_SCTE_AEIT_SRC           (TMT_SCTE + ((HANDLE_TYPE_T)  6))
#define TMT_SCTE_AETT_BLK           (TMT_SCTE + ((HANDLE_TYPE_T)  7))
#define TMT_SCTE_TXT                (TMT_SCTE + ((HANDLE_TYPE_T)  8))

/* SCTE-SI engine get types */
#define SCTE_SI_GET_TYPE_NIT_CDS_ENTRY              \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      0))

#define SCTE_SI_GET_TYPE_NIT_MMS_ENTRY              \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      1))

#define SCTE_SI_GET_TYPE_NTT_SNS_ENTRY              \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      2))

#define SCTE_SI_GET_TYPE_SVCT_VCM_ENTRY             \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      3))

#define SCTE_SI_GET_TYPE_SVCT_DCM_ENTRY             \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      4))

#define SCTE_SI_GET_TYPE_SVCT_ICM_ENTRY             \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      5))

#define SCTE_SI_GET_TYPE_SVCT_VCM_VC_RECORD         \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      6))

#define SCTE_SI_GET_TYPE_SVCT_DCM_DATA_OBJ          \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      7))

#define SCTE_SI_GET_TYPE_SVCT_ICM_RECORD            \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      8))

#define SCTE_SI_GET_TYPE_AEIT_SRC_ENTRY             \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      9))

#define SCTE_SI_GET_TYPE_AEIT_EVT_ENTRY             \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      10))

#define SCTE_SI_GET_TYPE_NTT_LANG_CODE              \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      11))
    
#define SCTE_SI_GET_TYPE_SVCT_VCT_ID                \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      12))
    
#define SCTE_SI_GET_TYPE_MGT_TAG                    \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      13))
        
#define SCTE_SI_GET_TYPE_AETT_ETM_ID                \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      14))

#define SCTE_SI_GET_TYPE_TXT_NUM_LANG               \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      15))

#define SCTE_SI_GET_TYPE_TXT_LANG_INFO              \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      16))

#define SCTE_SI_GET_TYPE_TXT_SEG_INFO               \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      17))

#define SCTE_SI_GET_TYPE_TXT                        \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      18))

#define SCTE_SI_GET_TYPE_TXT_IDX                    \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      19))
    
#define SCTE_SI_GET_TYPE_TXT_ID                     \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      20))    

#define SCTE_SI_GET_TYPE_TXT_DESCR                  \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      21))

#define SCTE_SI_GET_TYPE_PROTOCOL_VERSION           \
    (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T)      22))

/* SCTE-SI engine load types */
#define SCTE_SI_LOAD_TYPE_TIME                      \
    (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T)    0))

/*****************************/
/* Network Information Table */
/*****************************/
typedef UINT8 SCTE_UNIT_T;
typedef UINT8 SCTE_TRANS_SYS_T;
typedef UINT8 SCTE_INNER_CODING_MOD_T;
typedef UINT8 SCTE_MOD_FORMAT_T;

/* Possible values of t_spacing_unit and ui1_freq_unit of SCTE_SI_NIT_CDS_INFO_T */
#define NIT_CDS_UNIT_10_KHZ         ((SCTE_UNIT_T)  0)
#define NIT_CDS_UNIT_125_KHZ        ((SCTE_UNIT_T)  1)

/* Holds values of a NIT_CDS loop entry */
typedef struct _SCTE_SI_NIT_CDS_INFO_T
{
    UINT8           ui1_1st_idx;
    
    UINT8           ui1_num_carrs;

    SCTE_UNIT_T     e_spacing_unit;
    UINT16          ui2_freq_spacing;

    SCTE_UNIT_T     e_freq_unit;
    UINT16          ui2_1st_carr_freq;
}   SCTE_SI_NIT_CDS_INFO_T;

/* Possible values of t_trans_sys of SCTE_SI_NIT_MMS_INFO_T */
#define NIT_MMS_TRANS_SYS_UNKNOWN           ((SCTE_TRANS_SYS_T)  0)
#define NIT_MMS_TRANS_SYS_ITU_T_ANNEX_B     ((SCTE_TRANS_SYS_T)  2)
#define NIT_MMS_TRANS_SYS_ATSC              ((SCTE_TRANS_SYS_T)  4)

/* Possible values of t_inner_coding_mod of SCTE_SI_NIT_MMS_INFO_T */
#define NIT_MMS_INNER_CODING_MOD_5_11       ((SCTE_INNER_CODING_MOD_T)   0)
#define NIT_MMS_INNER_CODING_MOD_2_1        ((SCTE_INNER_CODING_MOD_T)   1)
#define NIT_MMS_INNER_CODING_MOD_3_5        ((SCTE_INNER_CODING_MOD_T)   3)
#define NIT_MMS_INNER_CODING_MOD_2_3        ((SCTE_INNER_CODING_MOD_T)   5)
#define NIT_MMS_INNER_CODING_MOD_3_4        ((SCTE_INNER_CODING_MOD_T)   7)
#define NIT_MMS_INNER_CODING_MOD_4_5        ((SCTE_INNER_CODING_MOD_T)   8)
#define NIT_MMS_INNER_CODING_MOD_5_6        ((SCTE_INNER_CODING_MOD_T)   9)
#define NIT_MMS_INNER_CODING_MOD_7_8        ((SCTE_INNER_CODING_MOD_T)  11)
#define NIT_MMS_INNER_CODING_MOD_NONE       ((SCTE_INNER_CODING_MOD_T)  15)

/* Possible values of t_mod_format of SCTE_SI_NIT_MMS_INFO_T */
#define NIT_MMS_MOD_FMT_UNKNOWN             ((SCTE_MOD_FORMAT_T)   0)
#define NIT_MMS_MOD_FMT_QPSK                ((SCTE_MOD_FORMAT_T)   1)
#define NIT_MMS_MOD_FMT_BPSK                ((SCTE_MOD_FORMAT_T)   2)
#define NIT_MMS_MOD_FMT_QQPSK               ((SCTE_MOD_FORMAT_T)   3)
#define NIT_MMS_MOD_FMT_VSB_8               ((SCTE_MOD_FORMAT_T)   4)
#define NIT_MMS_MOD_FMT_VSB_16              ((SCTE_MOD_FORMAT_T)   5)
#define NIT_MMS_MOD_FMT_QAM_16              ((SCTE_MOD_FORMAT_T)   6)
#define NIT_MMS_MOD_FMT_QAM_32              ((SCTE_MOD_FORMAT_T)   7)
#define NIT_MMS_MOD_FMT_QAM_64              ((SCTE_MOD_FORMAT_T)   8)
#define NIT_MMS_MOD_FMT_QAM_80              ((SCTE_MOD_FORMAT_T)   9)
#define NIT_MMS_MOD_FMT_QAM_96              ((SCTE_MOD_FORMAT_T)  10)
#define NIT_MMS_MOD_FMT_QAM_112             ((SCTE_MOD_FORMAT_T)  11)
#define NIT_MMS_MOD_FMT_QAM_128             ((SCTE_MOD_FORMAT_T)  12)
#define NIT_MMS_MOD_FMT_QAM_160             ((SCTE_MOD_FORMAT_T)  13)
#define NIT_MMS_MOD_FMT_QAM_192             ((SCTE_MOD_FORMAT_T)  14)
#define NIT_MMS_MOD_FMT_QAM_224             ((SCTE_MOD_FORMAT_T)  15)
#define NIT_MMS_MOD_FMT_QAM_256             ((SCTE_MOD_FORMAT_T)  16)
#define NIT_MMS_MOD_FMT_QAM_320             ((SCTE_MOD_FORMAT_T)  17)
#define NIT_MMS_MOD_FMT_QAM_384             ((SCTE_MOD_FORMAT_T)  18)
#define NIT_MMS_MOD_FMT_QAM_448             ((SCTE_MOD_FORMAT_T)  19)
#define NIT_MMS_MOD_FMT_QAM_512             ((SCTE_MOD_FORMAT_T)  20)
#define NIT_MMS_MOD_FMT_QAM_640             ((SCTE_MOD_FORMAT_T)  21)
#define NIT_MMS_MOD_FMT_QAM_768             ((SCTE_MOD_FORMAT_T)  22)
#define NIT_MMS_MOD_FMT_QAM_896             ((SCTE_MOD_FORMAT_T)  23)
#define NIT_MMS_MOD_FMT_QAM_1024            ((SCTE_MOD_FORMAT_T)  24)

/* Holds values of a NIT_MMS loop entry */
typedef struct _SCTE_SI_NIT_MMS_INFO_T
{
    UINT8                       ui1_1st_idx;
    
    SCTE_TRANS_SYS_T            e_trans_sys;
    SCTE_INNER_CODING_MOD_T     e_inner_coding_mod;
    BOOL                        b_split_bs_mod;
    SCTE_MOD_FORMAT_T           e_mod_format;
    UINT32                      ui4_sym_rate;
}   SCTE_SI_NIT_MMS_INFO_T;

/**********************/
/* Network Text Table */
/**********************/
/* Holds values of a NTT_SNS loop entry */
typedef struct _SCTE_SI_NTT_SNS_INFO_T
{
    BOOL        b_app_type;
    union
    {
        UINT16  ui2_app_id;
        UINT16  ui2_src_id;
    }   u_id;
}   SCTE_SI_NTT_SNS_INFO_T;

/*
   Structure for the pv_get_info when e_get_type
   is set to SCTE_SI_GET_TYPE_TXT_HANDLE
*/
typedef struct _SCTE_SI_NTT_ID_INFO_T
{
    BOOL        b_app_type;
    UINT16      ui2_id;
}   SCTE_SI_NTT_ID_INFO_T;

/************************************/
/* Short-from Virtual Channel Table */
/************************************/
typedef UINT8 SCTE_PATH_T;
typedef UINT8 SCTE_TRANS_T;
typedef UINT8 SCTE_CH_T;
typedef UINT8 SCTE_VIDEO_STD_T;

/* Possible values of t_path_select of VC_RECORD_OBJ_T */
#define SVCT_VCM_PATH1          ((SCTE_PATH_T)  0)
#define SVCT_VCM_PATH2          ((SCTE_PATH_T)  1)

/* Possible values of t_trans_type of VC_RECORD_OBJ_T */
#define SVCT_VCM_MPEG2          ((SCTE_TRANS_T)  0)
#define SVCT_VCM_NON_MPEG2      ((SCTE_TRANS_T)  1)

/* Possible values of t_ch_type of VC_RECORD_OBJ_T */
#define SVCT_VCM_NORMAL         ((SCTE_CH_T)  0)
#define SVCT_VCM_HIDDEN         ((SCTE_CH_T)  1)

/* Possible values of t_video_std of MPEG2_SCTE_TRANS_TYPE_T */
#define SVCT_VCM_NTSC           ((SCTE_VIDEO_STD_T)  0)
#define SVCT_VCM_PAL_625        ((SCTE_VIDEO_STD_T)  1)
#define SVCT_VCM_PAL_525        ((SCTE_VIDEO_STD_T)  2)
#define SVCT_VCM_SECAM          ((SCTE_VIDEO_STD_T)  3)
#define SVCT_VCM_MAC            ((SCTE_VIDEO_STD_T)  4)

/* MPEG2 transport type */
typedef struct _SCTE_SI_MPEG2_TRANS_TYPE_T
{
    UINT8       ui1_cds_ref;
    UINT16      ui2_prog_num;
    UINT8       ui1_mms_ref;
}   SCTE_SI_MPEG2_TRANS_TYPE_T;

/* Non-MPEG2 transport type */
typedef struct _SCTE_SI_NON_MPEG2_TRANS_TYPE_T
{
    UINT8               ui1_cds_ref;
    BOOL                b_scrambled;
    SCTE_VIDEO_STD_T    e_video_std;
}   SCTE_SI_NON_MPEG2_TRANS_TYPE_T;

/* An individual VC record object */
typedef struct _SCTE_SI_SVCT_VC_RECORD_T
{
    UINT16          ui2_vc_num;

    BOOL            b_app_vc;
    union
    {
        UINT16      ui2_app_id;
        UINT16      ui2_src_id;
    }   u_id;

    SCTE_PATH_T     e_path_select;
    SCTE_CH_T       e_ch_type;

    SCTE_TRANS_T    e_trans_type;
    union
    {
        SCTE_SI_MPEG2_TRANS_TYPE_T      t_mpeg2_trans_type;
        SCTE_SI_NON_MPEG2_TRANS_TYPE_T  t_non_mpeg2_trans_type;
    }   u_trans_type;
}   SCTE_SI_SVCT_VC_RECORD_T;

/* Holds values of a SVCT_VCM entry */
typedef struct _SCTE_SI_SVCT_VCM_INFO_T
{
    BOOL        b_descr_included;
    BOOL        b_splice;
    TIME_T      t_act_time;
    UINT16      ui2_num_vc_records;
}   SCTE_SI_SVCT_VCM_INFO_T;

/* An individual DCM data object */
typedef struct _SCTE_SI_SVCT_DCM_DATA_T
{
    BOOL        b_range_defined;
    UINT8       ui1_ch_cnt;
}   SCTE_SI_SVCT_DCM_DATA_T;

/* Holds values of a SVCT_DCM entry */
typedef struct _SCTE_SI_SVCT_DCM_INFO_T
{
    UINT16      ui2_1st_vitual_ch;
    UINT16      ui2_dcm_data_len;    
}   SCTE_SI_SVCT_DCM_INFO_T;

/* An individual ICM record */
typedef struct _SCTE_SI_SVCT_ICM_RECORD_T
{
    UINT16      ui2_src_id;
    UINT16      ui2_vc_num;
}   SCTE_SI_SVCT_ICM_RECORD_T;

/* Holds values of a SVCT_ICM entry */
typedef struct _SCTE_SI_SVCT_ICM_INFO_T
{
    UINT16      ui2_1st_map_idx;
    UINT16      ui2_icm_rec_cnt;  
}   SCTE_SI_SVCT_ICM_INFO_T;

/*
   Structure for the pv_get_info when e_get_type is set to
   SCTE_SI_GET_TYPE_SVCT_VCM_VC_RECORD, SCTE_SI_GET_TYPE_SVCT_DCM_DATA_OBJ, or
   SCTE_SI_GET_TYPE_SVCT_ICM_RECORD
*/
typedef struct _SCTE_SI_SVCT_SECOND_LOOP_IDX_T
{
    UINT16      ui2_1st_loop_idx;
    UINT16      ui2_2nd_loop_idx;
}   SCTE_SI_SVCT_SECOND_LOOP_IDX_T;

/****************************************/
/* Aggregate Extended Information Table */
/****************************************/
typedef UINT8 SCTE_ETM_T;

/* Possible values of t_etm_present of SCTE_SI_AEIT_EVT_INFO_T */
#define AEIT_ETM_NONE       ((SCTE_ETM_T)  0)
#define AEIT_ETM_PRESENT    ((SCTE_ETM_T)  1)

/* Holds values of a AEIT_EVT loop entry */
typedef struct _SCTE_SI_AEIT_EVT_INFO_T
{
    UINT16      ui2_evt_id;
    TIME_T      t_start_time;
    SCTE_ETM_T  e_etm_present;
    TIME_T      t_duration;
}   SCTE_SI_AEIT_EVT_INFO_T;

/* Holds values of a AEIT_SRC loop entry */
typedef struct _SCTE_SI_AEIT_SRC_INFO_T
{
    UINT16      ui2_src_id;
    UINT16      ui2_num_evt;
}   SCTE_SI_AEIT_SRC_INFO_T;

/*
   Structure for the pv_get_info when e_get_type is set to
   SCTE_SI_GET_TYPE_AEIT_EVT_ENTRY
*/
typedef struct _SCTE_SI_AEIT_SECOND_LOOP_IDX_T
{
    UINT16      ui2_src_idx;
    UINT16      ui2_evt_idx;
}   SCTE_SI_AEIT_SECOND_LOOP_IDX_T;

/* Holds values of a text language loop entry */
typedef struct _SCTE_SI_TXT_LANG_INFO_T
{
    ISO_639_LANG_T  t_lang;

    UINT16          ui2_num_seg;
}   SCTE_SI_TXT_LANG_INFO_T;

/* Holds values of a text segment loop entry */
typedef struct _SCTE_SI_TXT_SEG_INFO_T
{
    SIZE_T  z_str_len;
    
    UINT8   ui1_compr_type;
    UINT8   ui1_compr_mode;

    BOOL    b_can_be_decompressed;
}   SCTE_SI_TXT_SEG_INFO_T;

/*
   Structure for the pv_get_info when e_get_type is set to
   SCTE_SI_GET_TYPE_TXT_SEG_INFO or SCTE_SI_GET_TYPE_TXT.
*/
typedef struct _SCTE_SI_LANG_SEG_IDX_INFO_T
{
    UINT16  ui2_lang_idx;
    UINT16  ui2_seg_idx;
}   SCTE_SI_LANG_SEG_IDX_INFO_T;

/*
   Structure for the output pv_data when e_get_type is set to
   SCTE_SI_GET_TYPE_TXT.
*/
typedef struct _SCTE_SI_TXT_INFO_T
{
    CHAR*   ps_txt;
    SIZE_T* pz_txt_len;
}   SCTE_SI_TXT_INFO_T;

/* SCTE-SI engine configuration structure */
typedef struct _SCTE_SI_CONFIG_T
{
    UINT16  ui2_num_nit_tbl_objs;
    UINT16  ui2_num_ntt_tbl_objs;
    UINT16  ui2_num_svct_tbl_objs;
    UINT16  ui2_num_aeit_tbl_objs;
    UINT16  ui2_num_aett_tbl_objs;
    UINT16  ui2_num_txt_tbl_objs;

    UINT8   ui1_num_nit_filters;
    UINT8   ui1_num_ntt_filters;
    UINT8   ui1_num_svct_filters;
    UINT8   ui1_num_aeit_filters;
    UINT8   ui1_num_aett_filters;

    SIZE_T  z_nit_buff_size;
    SIZE_T  z_ntt_buff_size;
    SIZE_T  z_svct_buff_size;
    SIZE_T  z_aeit_buff_size;
    SIZE_T  z_aett_buff_size;
}   SCTE_SI_CONFIG_T;

#endif /* _U_SCTE_SI_H_ */

