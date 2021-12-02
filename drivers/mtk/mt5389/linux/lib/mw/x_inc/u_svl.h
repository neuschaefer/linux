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
/*! @file u_svl.h 
 *  $File: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/middleware/svl/u_svl.h $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *    This header file contains service list (SVL) database API definitions,
 *    which are exported to other Middleware components and applications.
 *
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SVL_H_
#define _U_SVL_H_
/*----------------------------------------------------------------------------*/
/*! @defgroup   groupMW_SVL Service List
 *  @ingroup    groupMW
 *  @brief      The SVL database object is used to store and retrieve the
 *              services information received from the digital or analog
 *              broadcast on the DTV box.
 *  @see        groupMW_CDB
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "u_common.h"
#include "u_handle.h"
#include "u_handle_grp.h"

/*--------------------------------------------------------------------------
 Constants, enumerations, and macros
---------------------------------------------------------------------------*/

/* Return code status from API */
#define         SVLR_NEAREST_CHAN_MATCH    ((INT32)  1)    /**< Channel not found and nearest channel is returned for the iteration. */
#define         SVLR_OK                    ((INT32)  0)    /**< Operation succeeded. */
#define	        SVLR_FAIL                  ((INT32) -1)    /**< Operation failed. */
#define         SVLR_OUT_OF_HANDLE         ((INT32) -2)    /**< Handle allocation failed. */
#define	        SVLR_OUT_OF_MEM            ((INT32) -3)    /**< Memory exhausted. */
#define	        SVLR_INV_HANDLE            ((INT32) -4)    /**< Bad handle. */
#define         SVLR_INV_ARG               ((INT32) -5)    /**< Invalid argument. */
#define	        SVLR_REC_NOT_FOUND         ((INT32) -6)    /**< Record not found. */
#define	        SVLR_NO_WRITE_LOCK         ((INT32) -7)    /**< Failed initializing RW Lock. */
#define         SVLR_NOT_FOUND             ((INT32) -8)    /**< Specified SVL object not found. */
#define         SVLR_END_OF_ITERATION      ((INT32) -9)    /**< Iteration reached end. */
#define         SVLR_MODIFIED              ((INT32) -10)    /**< Database modified since last iteration. */
#define         SVLR_DUPLICATE_REC         ((INT32) -11)    /**< Existing record with the same key value is found. */
#define         SVLR_INV_FILE_PATH         ((INT32) -12)    /**< Invalid file path. */
#define         SVLR_OUT_OF_SPACE          ((INT32) -13)    /**< Failed to allocate space on file system. */
#define         SVLR_AEE_OUT_OF_RESOURCES  ((INT32) -14)    /**< Can not allocate AEE resources. */
#define         SVLR_AEE_NO_RIGHTS         ((INT32) -15)    /**< No right for the AEE resources. */
#define         SVLR_FILE_READ_FAIL        ((INT32) -16)    /**< Failed to read from file. */
#define         SVLR_FILE_WRITE_FAIL       ((INT32) -17)    /**< Failed to write to file. */
#define         SVLR_INV_FILE_VER          ((INT32) -18)    /**< Incompatible file version. */


/* Handle type for the SVL handle. */
#define SVLT_SERVICE_LIST (HT_GROUP_SVL + ((HANDLE_TYPE_T) 0))    /**< Handle type for the Service List. */

/* Notification condition */
/*------------------------------------------------------------------*/
/*!  @brief  Enumeration for SVL Database conditions
 *  @code
 *  typedef  enum
 *  {
 *      SVL_UPDATING = 1,  
 *      SVL_UPDATED,       
 *      SVL_CLOSED,        
 *      SVL_UNKNOWN
 *  } SVL_COND_T;
 *  @endcode
 *  @li@c  SVL_UPDATING                  - When svl_lock is called.
 *  @li@c  SVL_UPDATED                   - When svl_unlock is called.
 *  @li@c  SVL_CLOSED                    - When svl_delete is called.
 *  @li@c  SVL_UNKNOWN                   - Unknown condition.
 */
/*------------------------------------------------------------------*/
typedef  enum
{
    SVL_UPDATING = 1,  /* When svl_lock is called. */
    SVL_UPDATED,       /* When svl_unlock is called. */
    SVL_CLOSED,        /* When svl_delete is called. */
    SVL_UNKNOWN
} SVL_COND_T;

/* Notification reasons */
#define   SVL_REASON_UNKNOWN   ((UINT32) ( 0x0000 ))    /**< Reason unknown */
#define   SVL_RECORD_ADD       (MAKE_BIT_MASK_32(1))    /**< Reason for adding records */
#define   SVL_RECORD_DEL       (MAKE_BIT_MASK_32(2))    /**< Reason for deleting records */
#define   SVL_RECORD_MOD       (MAKE_BIT_MASK_32(3))    /**< Reason for modifying records */

/* Size of the string buffer for the program name. */
#define   MAX_PROG_NAME_LEN     64    /**< Size of the string buffer for the program name. spec needs 48 displayable characters not 48 bytes*/

/* Size of SVL header custom data size. */
#define   MW_SVL_HEADER_REC_CUSTOM_BYTES (8)
/* Size of the string buffer for the DVB short service name. */
#define   MAX_DVB_SERVICE_NAME_LEN          (16)    /**< Size of the string buffer for the DVB short service name. */
#define   MAX_ISDB_SERVICE_NAME_LEN         (30)    /**< Size of the string buffer for the ISDB short service name. */
#define   MAX_DTMB_SERVICE_NAME_LEN         (16)    /**< Size of the string buffer for the DTMB short service name. */

/* Size of the tring buffer for the service list name. */
#define   MAX_SVL_NAME_LEN      16    /**< Size of the string buffer for the service list name. */

/* Typedef and constants for the service type of the service */
typedef	UINT8	                 SVL_SERVICE_TYPE_T;    /**< Typedef for the service type of the service */

#define SVL_SERVICE_TYPE_UNKNOWN ((SVL_SERVICE_TYPE_T) 0)    /**< Unknown service type. */
#define SVL_SERVICE_TYPE_TV      ((SVL_SERVICE_TYPE_T) 1)    /**< Television service type. */
#define SVL_SERVICE_TYPE_RADIO   ((SVL_SERVICE_TYPE_T) 2)    /**< Radio service type. */
#define SVL_SERVICE_TYPE_APP     ((SVL_SERVICE_TYPE_T) 3)    /**< Application service type. */
#define SVL_SERVICE_TYPE_ISDB_DIGITAL_TV    ((SVL_SERVICE_TYPE_T) 4)    /**< ISDB digital TV service type. */
#define SVL_SERVICE_TYPE_ISDB_DIGITAL_AUDIO ((SVL_SERVICE_TYPE_T) 5)    /**< ISDB digital audio service type. */
#define SVL_SERVICE_TYPE_ISDB_DATA          ((SVL_SERVICE_TYPE_T) 6)    /**< ISDB data service type. */
#define SVL_SERVICE_TYPE_ISDB_SPECIAL_VIDEO ((SVL_SERVICE_TYPE_T) 7)    /**< ISDB special video service type. */
#define SVL_SERVICE_TYPE_ISDB_SPECIAL_AUDIO ((SVL_SERVICE_TYPE_T) 8)    /**< ISDB special audio service type. */
#define SVL_SERVICE_TYPE_ISDB_SPECIAL_DATA  ((SVL_SERVICE_TYPE_T) 9)    /**< ISDB special data service type. */
#define SVL_SERVICE_TYPE_ISDB_ENGINEERING   ((SVL_SERVICE_TYPE_T) 10)    /**< ISDB engineering service type. */
#define SVL_SERVICE_TYPE_ISDB_BOOKMARK      ((SVL_SERVICE_TYPE_T) 11)    /**< ISDB bookmark service type. */

/* Typedef and constants for the ISDB-T service layer*/
typedef UINT8					SVL_SERVICE_LAYER_T;    /**< Typedef for the ISDB-T service layer */

#define SVL_SERVICE_LAYER_LOWER		        ((SVL_SERVICE_LAYER_T) 0x0)  /*00*/    /**< Low service layer. */
#define SVL_SERVICE_LAYER_MIDDLE	        ((SVL_SERVICE_LAYER_T) 0x1)  /*01*/    /**< Middle service layer. */
#define SVL_SERVICE_LAYER_HIGH		        ((SVL_SERVICE_LAYER_T) 0x2)  /*10*/    /**< High service layer. */
#define SVL_SERVICE_LAYER_UNKOWNED          ((SVL_SERVICE_LAYER_T) 0xF)  /*1111*/    /**< Unknown service layer. */

/* Size (byte) of SVL record header and data structure: */
#ifndef ANDROID
#define	SVL_REC_HEADER_PADDING    104    /**< Padding size (byte) of SVL record header structure. */
#else
#define	SVL_REC_HEADER_PADDING    104 + 20    /**< Padding size (byte) of SVL record header structure. */
#endif
#define	SVL_REC_DATA_PADDING      136   /**< Padding size (byte) of SVL record data structure.

                                           Note: SVL_REC_DATA_PADDING should
                                           set to the size of largest field in
                                           u_data union structure in SVL_REC_T.

                                           Increase to 64 due to new field (
                                           t_orig_svc_info field added in
                                           SVL_DVB_T structure.) 2009 Feb 12.

                                           Increase to 80 due to new field (
                                           t_sbtl_pref field added in
                                           SVL_DVB_T structure.) 2010 Mar 26.

                                           Increase to 128 due to new field (
                                           fields for NVOD & MF added in
                                           SVL_DVB_T structure.) 2010 May 18.
                                           Increase to 136 due to new field (
                                           fields for on_id & ts_id added in
                                           SVL_DVB_T structure.) 2011 Oct 28.
                                        */

/* Pre-set size of the SVL record structure. */
#define SVL_REC_SIZE   (SVL_REC_HEADER_PADDING+SVL_REC_DATA_PADDING+8)    /**< Pre-set size of the SVL record structure. */

/* Typedef and constants for SVL record id query flag. */
typedef     UINT8                 SVL_REC_ID_QRY_T;    /**< Typedef for SVL record id query flag. */

#define     SVL_FIRST_REC_ID      ((SVL_REC_ID_QRY_T) 1)    /**< First record ID. */
#define     SVL_LAST_REC_ID       ((SVL_REC_ID_QRY_T) 2)    /**< Last record ID. */
#define     SVL_NEXT_REC_ID       ((SVL_REC_ID_QRY_T) 3)    /**< Next record ID. */

/* Typedef and constants for SVL record iteration flag. */
typedef	    UINT8                 SVL_ITERATE_T;    /**< Typedef for SVL record iteration flag. */

#define	    SVL_CHANNEL_PLUS      ((SVL_ITERATE_T) 1)    /**< Next channel with larger channel number. */
#define	    SVL_CHANNEL_MINUS     ((SVL_ITERATE_T) 2)    /**< Next channel with smaller channel number. */
#define	    SVL_CHANNEL_SMALLEST  ((SVL_ITERATE_T) 3)    /**< Channel with smallest channel number. */
#define	    SVL_CHANNEL_LARGEST   ((SVL_ITERATE_T) 4)    /**< Channel with largest channel number. */

#define     SVL_NEXT_REC          ((SVL_ITERATE_T) 1)    /**< Next channel with larger channel number. */
#define     SVL_PREV_REC          ((SVL_ITERATE_T) 2)    /**< Next channel with smaller channel number. */
#define     SVL_FIRST_REC         ((SVL_ITERATE_T) 3)    /**< Channel with smallest channel number. */
#define     SVL_LAST_REC          ((SVL_ITERATE_T) 4)    /**< Channel with largest channel number. */

/* Typedef and constants for SVL memory mangement policy. */
typedef	    UINT8	              SVL_MEM_POLICY_T;    /**< Typedef for SVL memory mangement policy. */

#define	    SVL_MEM_STATIC         ((SVL_MEM_POLICY_T) 0)    /**< Static memory policy */
#define	    SVL_MEM_DYNAMIC        ((SVL_MEM_POLICY_T) 1)    /**< Dynamic memory policy */

/*
   Null version id. This value should be consistent with
   the null version value used by the CDB
*/
#define SVL_NULL_VER_ID     ((UINT32)0)    /**< NULL SVL version ID used at the start of the iteration. */

/* NULL SVL ID value */
#define SVL_NULL_REC_ID     ((UINT16)0)    /**< Null SVL record ID which is invalid. */

/* NULL SVL ID value. */
#define SVL_NULL_ID         ((UINT16)0)    /**< Null SVL record ID which is invalid. */

/*---------------------------------------------------------------------------
 Structures
----------------------------------------------------------------------------*/

/********** Structure for common data *********/
/*------------------------------------------------------------------*/
/*!  @brief  Structure for simple date time representation
 *  @code
 *  typedef struct _SVL_SIMPLE_DATE_TIME_T_
 *  {
 *      UINT16  ui2_mjd;
 *      UINT16  ui2_bcd;
 *  }   SVL_SIMPLE_DATE_TIME_T;
 *  @endcode
 *  @li@c  ui2_mjd                       - Date in MJD representation
 *  @li@c  ui2_bcd                       - Time (hour, minute) in BCD
 */
/*------------------------------------------------------------------*/
typedef struct _SVL_SIMPLE_DATE_TIME_T_
{
    UINT16  ui2_mjd;
    UINT16  ui2_bcd;
}   SVL_SIMPLE_DATE_TIME_T;


/********* Structure for analog service *********/

typedef UINT8   SVL_ANALOG_EU_DATA_MASK_T;    /**< Typedef for EU analog channel data mask */

#define SVL_ANALOG_EU_DATA_MASK_NI_TTX_8_30_FMT_1      MAKE_BIT_MASK_8(0)    /**<  */
#define SVL_ANALOG_EU_DATA_MASK_CNI_TTX_8_30_FMT_2     MAKE_BIT_MASK_8(1)    /**<  */
#define SVL_ANALOG_EU_DATA_MASK_CNI_VPS                MAKE_BIT_MASK_8(2)    /**<  */
#define SVL_ANALOG_EU_DATA_MASK_VPS_DATA_BYTE_5        MAKE_BIT_MASK_8(3)    /**<  */


typedef UINT8 CI_HOST_SHUNNING_MODE_T;      /**< Typedef for CI host shunning mode */
#define CI_HOST_SHUNNING_MODE_INVALID   ((CI_HOST_SHUNNING_MODE_T)0)    /* Untrustable */
#define CI_HOST_SHUNNING_MODE_INACTIVE  ((CI_HOST_SHUNNING_MODE_T)1)    /* bypass mode */
#define CI_HOST_SHUNNING_MODE_ACTIVE    ((CI_HOST_SHUNNING_MODE_T)2)    /* pass-through mode */
#define CI_HOST_SHUNNING_MODE_AMBIGOUS  ((CI_HOST_SHUNNING_MODE_T)3)    /* SDT is got in pass though mode, ambigous */

#define SVL_ANALOG_MAX_NUM_SCHEDULE    1    /**< Maximum number of schedule block settings for analog services. */

/*------------------------------------------------------------------*/
/*!  @brief  Structure for analog service information
 *  @code
 *  typedef struct _SVL_ANALOG_T
 *  {
 *        Stream component mask: e.g., audio stream, video stream,
 *        sub-title, closed caption, etc.
 *      UINT32	           ui4_s_comp_mask;
 *        Bitmask for TV system (B/G, D/K, I, M, N, L/L', etc)
 *      UINT32             ui4_tv_sys;
 *        Bitmask for Audio system (AM, FM, NICAM, BTSC)
 *      UINT32             ui4_audio_sys;
 *      UINT16             ui2_vps_ni_code;
 *      UINT16             ui2_ttx_fmt_1_ni_code;
 *      UINT16             ui2_ttx_fmt_2_cni_code;
 *      SVL_ANALOG_EU_DATA_MASK_T t_eu_data_mask;
 *      UINT8              ui1_vps_data_byte_5;
 *        For TV color systems (NTSC, PAL, SECAM,)
 *      COLOR_SYS_T        e_vid_color_sys;
 *        For analog channel info received via OOB, a valid source
 *        id can be associated with analog channel.
 *      UINT16             ui2_src_id;
 *      BOOL               b_valid_src_id;
 *      BOOL               b_no_aft;
 *      INT16              i2_aft_offset;
 *      INT8               ui1_aud_mts;
 *  } SVL_ANALOG_T;
 *  @endcode
 *  @li@c  ui4_s_comp_mask               - Stream component mask: e.g., audio
 *                                         stream, video stream, sub-title,
 *                                         closed caption, etc.
 *  @li@c  ui4_tv_sys                    - Bitmask specifying the type of TV
 *                                         system (e.g., B/G, D/K, I, M, N,
 *                                         L/L').
 *  @li@c  ui4_audio_sys                 - Bitmask specifying the Audio system
 *                                         (AM, FM, NICAM, BTSC)
 *  @li@c  ui2_vps_ni_code               - Network Identifier (NI) code
 *                                         retrieved from Video Programming
 *                                         System (VPS) packet. For EU only.
 *  @li@c  ui2_ttx_fmt_1_ni_code         - Network Identifier (NI) code
 *                                         retrieved from teletext table. For EU
 *                                         only.
 *  @li@c  ui2_ttx_fmt_2_cni_code        - Country & Network Identifier (CNI)
 *                                         code retrieved from teletext table.
 *                                         For EU only.
 *  @li@c  t_eu_data_mask                - EU data mask. Not used currently.
 *  @li@c  ui1_vps_data_byte_5           - VPS data byte 5. Not used currently.
 *  @li@c  e_vid_color_sys               - Specify the TV color system (e.g.,
 *                                         NTSC, PAL, SECAM)
 *  @li@c  ui2_src_id                    - A source ID associated with this
 *                                         analog channel.  The source ID can be
 *                                         used to look up analog channel
 *                                         information received via OOB
 *                                         (Out-Of-Band) path.
 *  @li@c  b_valid_src_id                - Flag to indicate if there is a
 *                                         `valid source ID' associated with
 *                                         this analog channel.
 *  @li@c  b_no_aft                      - Flag to indicate whether AFT (Auto
 *                                         Fine-Tune) is enabled. 
 *  @li@c  i2_aft_offset                 - When value is 0, it indicates auto
 *                                         AFT. Otherwise, it stands for the
 *                                         offset for manual AFT (-15 ~ +15).
 *                                         Only valid when b_no_aft is TRUE.
 *  @li@c  ui1_aud_mts                   - Index of user selected audio MTS.
 *  @li@c  ui2_schedule_num              - Number of schedule block settings.
 *                                         The maximum available number is
 *                                         defined in
 *                                         SVL_ANALOG_MAX_NUM_SCHEDULE.
 *  @li@c  at_schedule                   - Array of schedule block settings.
 *                                         There are at most
 *                                         SVL_ANALOG_MAX_NUM_SCHEDULE block
 *                                         schedules, each item has two date
 *                                         time values indicating the start and
 *                                         end time of the scheduled block.
 */
/*------------------------------------------------------------------*/
typedef struct _SVL_ANALOG_T
{
    /*
      Stream component mask: e.g., audio stream, video stream,
      sub-title, closed caption, etc.
    */
    UINT32	           ui4_s_comp_mask;

    /*
      Bitmask for TV system (B/G, D/K, I, M, N, L/L', etc)
    */
    UINT32             ui4_tv_sys;

    /*
      Bitmask for Audio system (AM, FM, NICAM, BTSC)
    */
    UINT32             ui4_audio_sys;

    /* For EU project. */
    UINT16             ui2_vps_ni_code;

    UINT16             ui2_ttx_fmt_1_ni_code;

    UINT16             ui2_ttx_fmt_2_cni_code;

    SVL_ANALOG_EU_DATA_MASK_T t_eu_data_mask;

    UINT8              ui1_vps_data_byte_5;

    /*
      For TV color systems (NTSC, PAL, SECAM,)
    */
    COLOR_SYS_T        e_vid_color_sys;

    /*
      For analog channel info received via OOB, a valid source
      id can be associated with analog channel.
    */
    UINT16             ui2_src_id;
    BOOL               b_valid_src_id;


    /* [DTV00062624] for Auto Fine Tune */
    BOOL               b_no_aft;
    INT16              i2_aft_offset;

    /* for China ATV nav enhancement */
    UINT8              ui1_aud_mts;

	/* for schedule block */
    UINT16                      ui2_schedule_num;
    SVL_SIMPLE_DATE_TIME_T      at_schedule[SVL_ANALOG_MAX_NUM_SCHEDULE][2];

} SVL_ANALOG_T;

/********* Structure for ATSC and SCTE service *********/
/*------------------------------------------------------------------*/
/*!  @brief  Structure for storing ATSC and SCTE specific information
 *  @code
 *  typedef struct 	_SVL_ATSC_T
 *  {
 *      UINT16          ui2_src_id;
 *      UINT16          ui2_prog_id;
 *      UINT8           ui1_downmix_mode;
 *  } SVL_ATSC_T;
 *  @endcode
 *  @li@c  ui2_src_id                    - Source ID to find VCT entry for
 *                                         additional information for this ATSC
 *                                         service.
 *  @li@c  ui2_prog_id                   - Program ID for this ATSC service.
 *  @li@c  ui1_downmix_mode              - Audio downmix mode preference.
 */
/*------------------------------------------------------------------*/
typedef struct 	_SVL_ATSC_T
{
    UINT16          ui2_src_id;
    UINT16          ui2_prog_id;
    UINT8           ui1_downmix_mode;
} SVL_ATSC_T;

/********* Structure for DVB service *********/

#define SVL_DVB_CA_REPLMNT_PRESENT          MAKE_BIT_MASK_32(0)    /**< Bitmask indicating the presence of DVB CA replacement information. */
#define SVL_DVB_SVC_REPLMNT_PRESENT         MAKE_BIT_MASK_32(1)    /**< Bitmask indicating the presence of DVB service replacement information. */
#define SVL_DVB_CMPLT_EIT_SCH_TS_PRESENT    MAKE_BIT_MASK_32(2)    /**< Bitmask indicating the presence of complete EIT-Schedule TS information. */
/*#define SVL_DVB_HD_SIMULCAST_PRESENT        MAKE_BIT_MASK_32(3)*/

/*------------------------------------------------------------------*/
/*!  @brief  Enumeration for DVB service running status.
 *  @code
 *  typedef enum
 *  {
 *      DVB_SVC_STATUS_UNKNOWN,
 *  	DVB_SVC_RUNNING_STATUS_UNDEFINED,
 *  	DVB_SVC_NOT_RUNNING,
 *  	DVB_SVC_START_IN_SECONDS,
 *  	DVB_SVC_PAUSING,
 *  	DVB_SVC_RUNNING,
 *  	DVB_SVC_OFF_AIR
 *  }	DVB_SVC_RUNNING_STATUS_T;
 *  @endcode
 *  @li@c  DVB_SVC_STATUS_UNKNOWN        - Running status unknown.
 *  @li@c  DVB_SVC_RUNNING_STATUS_UNDEFINED    - Running status undefined.
 *  @li@c  DVB_SVC_NOT_RUNNING           - Not running.
 *  @li@c  DVB_SVC_START_IN_SECONDS      - Start in seconds.
 *  @li@c  DVB_SVC_PAUSING               - Pausing.
 *  @li@c  DVB_SVC_RUNNING               - Running.
 *  @li@c  DVB_SVC_OFF_AIR               - Off air.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DVB_SVC_STATUS_UNKNOWN,
	DVB_SVC_RUNNING_STATUS_UNDEFINED,
	DVB_SVC_NOT_RUNNING,
	DVB_SVC_START_IN_SECONDS,
	DVB_SVC_PAUSING,
	DVB_SVC_RUNNING,
	DVB_SVC_OFF_AIR
}	DVB_SVC_RUNNING_STATUS_T;

/*------------------------------------------------------------------*/
/*!  @brief  Enumeration for DTMB service running status.
 *  @code
 *  typedef enum
 *  {
 *      DTMB_SVC_STATUS_UNKNOWN,
 *  	DTMB_SVC_RUNNING_STATUS_UNDEFINED,
 *  	DTMB_SVC_NOT_RUNNING,
 *  	DTMB_SVC_START_IN_SECONDS,
 *  	DTMB_SVC_PAUSING,
 *  	DTMB_SVC_RUNNING,
 *  	DTMB_SVC_OFF_AIR
 *  }	DTMB_SVC_RUNNING_STATUS_T;
 *  @endcode
 *  @li@c  DTMB_SVC_STATUS_UNKNOWN        - Running status unknown.
 *  @li@c  DTMB_SVC_RUNNING_STATUS_UNDEFINED    - Running status undefined.
 *  @li@c  DTMB_SVC_NOT_RUNNING           - Not running.
 *  @li@c  DTMB_SVC_START_IN_SECONDS      - Start in seconds.
 *  @li@c  DTMB_SVC_PAUSING               - Pausing.
 *  @li@c  DTMB_SVC_RUNNING               - Running.
 *  @li@c  DTMB_SVC_OFF_AIR               - Off air.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DTMB_SVC_STATUS_UNKNOWN,
	DTMB_SVC_RUNNING_STATUS_UNDEFINED,
	DTMB_SVC_NOT_RUNNING,
	DTMB_SVC_START_IN_SECONDS,
	DTMB_SVC_PAUSING,
	DTMB_SVC_RUNNING,
	DTMB_SVC_OFF_AIR
}	DTMB_SVC_RUNNING_STATUS_T;

/* This structure contains a tables original network,
   transport stream and  service id values.
   This structure is equivalent to DVB_SI_ON_TS_SVC_ID_INFO_T structure
*/
/*------------------------------------------------------------------*/
/*!  @brief  Structure for identifying a DVB service
 *  @code
 *  typedef struct _SVL_DVB_TS_SVC_ID_INFO_T_
 *  {
 *      UINT16  ui2_on_id;
 *      UINT16  ui2_ts_id;
 *      UINT16  ui2_svc_id;
 *  }   SVL_DVB_TS_SVC_ID_INFO_T;
 *  @endcode
 *  @li@c  ui2_on_id                     - Original network ID
 *  @li@c  ui2_ts_id                     - Transport stream ID
 *  @li@c  ui2_svc_id                    - Serivce ID
 */
/*------------------------------------------------------------------*/
typedef struct _SVL_DVB_TS_SVC_ID_INFO_T_
{
    UINT16  ui2_on_id;
    UINT16  ui2_ts_id;
    UINT16  ui2_svc_id;
}   SVL_DVB_TS_SVC_ID_INFO_T;

/*------------------------------------------------------------------*/
/*! @brief  Structure for storing user preference for subtitle
 *  @code
 *  typedef struct _SVL_DVB_SBTL_PREF_T_
 *  {
 *      ISO_639_LANG_T  t_lang_1st;
 *      ISO_639_LANG_T  t_lang_2nd;
 *  } SVL_DVB_SBTL_PREF_T;
 *  @endcode
 *  @li@c  t_lang_1st                    - 1st preferred subtitle language.
 *  @li@c  t_lang_2nd                    - 2nd preferred subtitle language.
 */
/*------------------------------------------------------------------*/
typedef struct _SVL_DVB_SBTL_PREF_T_
{
    ISO_639_LANG_T  t_lang_1st;
    ISO_639_LANG_T  t_lang_2nd;
} SVL_DVB_SBTL_PREF_T;

/*------------------------------------------------------------------*/
/*!  @brief  Structure for storing DVB specific information
 *  @code
 *  typedef struct 	_SVL_DVB_T
 *  {
 *      UINT32         ui4_reserve;
 *      CHAR           s_svc_name[MAX_DVB_SERVICE_NAME_LEN+1];
 *      UINT16		   ui2_ca_system_id;
 *      DVB_SVC_RUNNING_STATUS_T	e_running_status;
 *      UINT32         ui4_linkage_descr_mask;
 *      SVL_DVB_TS_SVC_ID_INFO_T  t_ca_replmnt_on_ts_svc_id_info;
 *      SVL_DVB_TS_SVC_ID_INFO_T  t_svc_replmnt_on_ts_svc_id_info;
 *      SVL_DVB_TS_SVC_ID_INFO_T  t_cmplt_eit_sch_on_ts_svc_id_info;
 *      SVL_DVB_TS_SVC_ID_INFO_T  t_hd_simulcast_on_ts_svc_id_info;
 *      SVL_DVB_TS_SVC_ID_INFO_T  t_orig_svc_info;
 *      BOOL b_free_ca_mode;
 *      CI_HOST_SHUNNING_MODE_T t_ci_host_shunning_mode;
 *      UINT8           ui1_downmix_mode;
 *      SVL_DVB_SBTL_PREF_T       t_sbtl_pref;
 *      TIME_T          t_ts_svc_time_start;
 *      TIME_T          t_ts_svc_duration;
 *      UINT16          ui2_ref_svl_rec_id;
 *      UINT16          ui2_last_view_svl_id;
 *      UINT16          ui2_last_view_svl_rec_id;
 *      UINT8           ui1_mf_type;
 *      UINT16          ui2_ts_id;    
 *      UINT16          ui2_on_id;     
 *  } SVL_DVB_T;
 *  @endcode
 *  @li@c  ui4_reserve                   - Reserved.
 *  @li@c  s_svc_name                    - Service name used in DVB
 *                                         broadcast.
 *  @li@c  ui2_ca_system_id              - CA system ID from CA descriptor in
 *                                         SDT.
 *  @li@c  e_running_status              - The current status of the service,
 *                                         retrieved from SDT.
 *  @li@c  ui4_linkage_descr_mask        - This mask indicates the presence of
 *                                         the following information retrieved
 *                                         from Linkage descriptors.
 *  @li@c  t_ca_replmnt_on_ts_svc_id_info    - CA replacement information from
 *                                             Linkage descriptor in SDT.
 *  @li@c  t_svc_replmnt_on_ts_svc_id_info   - Service replacement information
 *                                             from Linkage descriptor in SDT.
 *  @li@c  t_cmplt_eit_sch_on_ts_svc_id_info - Refer to the barker channel with
 *                                             complete EIT schedule information
 *                                             retrieved from Linkage descriptor
 *                                             in NIT.  Used in NorDig.
 *  @li@c  t_hd_simulcast_on_ts_svc_id_info  - HD simulcast replacement
 *                                             information from Linkage
 *                                             descriptor in SDT.
 *  @li@c  t_orig_svc_info               - Original service information.
 *  @li@c  b_free_ca_mode                - Indicate whether stream components in
 *                                         the service are not scrambled.
 *  @li@c  t_ci_host_shunning_mode       - Indicate whether CI host is in shunning
 *                                         mode. For CI plus.
 *                                         protection.
 *  @li@c  ui1_downmix_mode              - Audio downmix mode preference.
 *  @li@c  t_sbtl_pref                   - Subtitle preference info.
 *  @li@c  t_ts_svc_time_start           - Service start time for NVOD TS
 *                                         services, not to be stored in flash.
 *  @li@c  t_ts_svc_duration             - Service duration for NVOD TS
 *                                         services, not to be stored in flash.
 *  @li@c  ui2_ref_svl_rec_id            - Service record ID for reference
 *                                         service, used in NVOD TS services or
 *                                         MF sub-feed services. Placed after
 *                                         t_ts_svc_duration for padding.
 *  @li@c  ui2_last_view_svl_id          - Last view service ID, for NVOD
 *                                         reference service or MF portal
 *                                         service, not to be stored in flash.
 *  @li@c  ui2_last_view_svl_rec_id      - Last view service record ID, for NVOD
 *                                         reference service or MF portal
 *                                         service, not to be stored in flash.
 *  @li@c  ui1_mf_type                   - MF type: 0: none, 1: MF portal
 *                                         service, 2: MF sub-feed service. Not
 *                                         to be stored in flash.
 */
/*------------------------------------------------------------------*/
typedef struct 	_SVL_DVB_T
{
    UINT32         ui4_reserve;
    CHAR           s_svc_name[MAX_DVB_SERVICE_NAME_LEN+1];
    UINT16		   ui2_ca_system_id;
    DVB_SVC_RUNNING_STATUS_T	e_running_status;
    UINT32         ui4_linkage_descr_mask;

    SVL_DVB_TS_SVC_ID_INFO_T  t_ca_replmnt_on_ts_svc_id_info;
    SVL_DVB_TS_SVC_ID_INFO_T  t_svc_replmnt_on_ts_svc_id_info;
    SVL_DVB_TS_SVC_ID_INFO_T  t_cmplt_eit_sch_on_ts_svc_id_info;
    SVL_DVB_TS_SVC_ID_INFO_T  t_hd_simulcast_on_ts_svc_id_info;
    SVL_DVB_TS_SVC_ID_INFO_T  t_orig_svc_info;

    BOOL b_free_ca_mode;
    CI_HOST_SHUNNING_MODE_T t_ci_host_shunning_mode;

    UINT8           ui1_downmix_mode;

    SVL_DVB_SBTL_PREF_T       t_sbtl_pref;

    TIME_T          t_ts_svc_time_start;
    TIME_T          t_ts_svc_duration;
    UINT16          ui2_ref_svl_rec_id;
    UINT16          ui2_last_view_svl_id;
    UINT16          ui2_last_view_svl_rec_id;
    UINT8           ui1_mf_type;
    BOOL            b_eit_pf_flag;
    UINT16          ui2_lcn;
   /*ts_id & on_id only used in dvb_c*/
    UINT16              ui2_ts_id;     /* Transport stream id. */
    UINT16              ui2_on_id;     /* Original network id. */
} SVL_DVB_T;

/********* Structure for ISDB service *********/
#define SVL_ISDB_MAX_NUM_SCHEDULE    5    /**< Maximum number of schedule block settings for ISDB digital services. */

/*------------------------------------------------------------------*/
/*!  @brief  Structure for storing ISDB specific information
 *  @code
 *  typedef struct 	_SVL_ISDB_T
 *  {
 *  	UINT8		   				ui1_reg_key;
 *  	SVL_SERVICE_LAYER_T		    e_svc_layer;
 *      UINT8                       ui1_downmix_mode;
 *  }   SVL_ISDB_T;
 *  @endcode
 *  @li@c  ui1_reg_key                   - Registered key for one-touch play. In
 *                                         ISDB, this value is used for
 *                                         selecting channel for one-touch play,
 *                                         and may differ from
 *                                         remote_control_key_id. The value lies
 *                                         between 1 and 12. In SBTVD, the value
 *                                         ranges from 1 ~ 99 and is the same
 *                                         as remote_control_key_id. This
 *                                         information is obtained from TS
 *                                         Information descriptor.
 *  @li@c  e_svc_layer                   - The transmission layer the service
 *                                         lies in.  Layers include transmission
 *                                         type A 64QAM, type B 16QAM, type C
 *                                         QPSK, type A 16QAM and unknown. See
 *                                         ARIB B14 30.4.3.4 for details.  This
 *                                         information is obtained from TS
 *                                         Information descriptor.
 *  @li@c  ui1_downmix_mode              - Audio downmix mode preference.
 *  @li@c  ui2_schedule_num              - Number of schedule block settings.
 *                                         The maximum available number is
 *                                         defined in SVL_ISDB_MAX_NUM_SCHEDULE.
 *  @li@c  at_schedule                   - Array of schedule block settings.
 *                                         There are at most
 *                                         SVL_ISDB_MAX_NUM_SCHEDULE block
 *                                         schedules, each item has two date
 *                                         time values indicating the start and
 *                                         end time of the scheduled block.
 */
/*------------------------------------------------------------------*/
typedef struct 	_SVL_ISDB_T
{
	UINT8		   				ui1_reg_key;
	SVL_SERVICE_LAYER_T		    e_svc_layer;
    UINT8                       ui1_downmix_mode;
    UINT16                      ui2_schedule_num;
    SVL_SIMPLE_DATE_TIME_T      at_schedule[SVL_ISDB_MAX_NUM_SCHEDULE][2];
}   SVL_ISDB_T;

/*------------------------------------------------------------------*/
/*!  @brief  Structure for storing DTMB specific information
 *  @code
 *  typedef struct 	_SVL_DTMB_T
 *  {
 *      UINT32         ui4_reserve;
 *      CHAR           s_svc_name[MAX_DTMB_SERVICE_NAME_LEN+1];
 *      UINT16		   ui2_ca_system_id;
 *      DTMB_SVC_RUNNING_STATUS_T	e_running_status;
 *      UINT32         ui4_linkage_descr_mask;
 *      SVL_DVB_TS_SVC_ID_INFO_T  t_ca_replmnt_on_ts_svc_id_info;
 *      SVL_DVB_TS_SVC_ID_INFO_T  t_svc_replmnt_on_ts_svc_id_info;
 *      SVL_DVB_TS_SVC_ID_INFO_T  t_cmplt_eit_sch_on_ts_svc_id_info;
 *      SVL_DVB_TS_SVC_ID_INFO_T  t_hd_simulcast_on_ts_svc_id_info;
 *      SVL_DVB_TS_SVC_ID_INFO_T  t_orig_svc_info;
 *      BOOL b_free_ca_mode;
 *      UINT8           ui1_downmix_mode;
 *  } SVL_DTMB_T;
 *  @endcode
 *  @li@c  ui4_reserve                   - Reserved.
 *  @li@c  s_svc_name                    - Service name used in DTMB
 *                                         broadcast.
 *  @li@c  ui2_ca_system_id              - CA system ID from CA descriptor in
 *                                         SDT.
 *  @li@c  e_running_status              - The current status of the service,
 *                                         retrieved from SDT.
 *  @li@c  ui4_linkage_descr_mask        - This mask indicates the presence of
 *                                         the following information retrieved
 *                                         from Linkage descriptors.
 *  @li@c  t_ca_replmnt_on_ts_svc_id_info    - CA replacement information from
 *                                             Linkage descriptor in SDT.
 *  @li@c  t_svc_replmnt_on_ts_svc_id_info   - Service replacement information
 *                                             from Linkage descriptor in SDT.
 *  @li@c  t_cmplt_eit_sch_on_ts_svc_id_info - Refer to the barker channel with
 *                                             complete EIT schedule information
 *                                             retrieved from Linkage descriptor
 *                                             in NIT.  Used in NorDig.
 *  @li@c  t_hd_simulcast_on_ts_svc_id_info  - HD simulcast replacement
 *                                             information from Linkage
 *                                             descriptor in SDT.
 *  @li@c  t_orig_svc_info               - Original service information.
 *  @li@c  b_free_ca_mode                - Indicate whether stream components in
 *                                         the service are not scrambled.
 *  @li@c  ui1_downmix_mode              - Audio downmix mode preference.
 */
/*------------------------------------------------------------------*/
typedef struct 	_SVL_DTMB_T
{
    UINT32         ui4_reserve;
    CHAR           s_svc_name[MAX_DTMB_SERVICE_NAME_LEN+1];
    UINT16		   ui2_ca_system_id;
    DTMB_SVC_RUNNING_STATUS_T	e_running_status;
    UINT32         ui4_linkage_descr_mask;

    SVL_DVB_TS_SVC_ID_INFO_T  t_ca_replmnt_on_ts_svc_id_info;
    SVL_DVB_TS_SVC_ID_INFO_T  t_svc_replmnt_on_ts_svc_id_info;
    SVL_DVB_TS_SVC_ID_INFO_T  t_cmplt_eit_sch_on_ts_svc_id_info;
    SVL_DVB_TS_SVC_ID_INFO_T  t_hd_simulcast_on_ts_svc_id_info;
    SVL_DVB_TS_SVC_ID_INFO_T  t_orig_svc_info;

    BOOL b_free_ca_mode;

    UINT8           ui1_downmix_mode;
} SVL_DTMB_T;

/********* Structure for FM Radio service *********/
/*------------------------------------------------------------------*/
/*!  @brief  Structure for storing FM Radio specific information
 *  @code
 *  typedef struct _SVL_FMRDO_T
 *  {
 *      UINT8               ui1_sgl_lvl;
 *  }   SVL_FMRDO_T;
 *  @endcode
 *  @li@c  ui1_sgl_lvl                   - The signal level.
 */
/*------------------------------------------------------------------*/
typedef struct _SVL_FMRDO_T
{
    UINT8               ui1_sgl_lvl;
}   SVL_FMRDO_T;

/********* Structure for the SVL record header. *********/
/*------------------------------------------------------------------*/
/*!  @brief  Structure for the SVL record header
 *  @code
 *  typedef struct _SVL_REC_HEADER_T
 *  {
 *      UINT32              ui4_channel_id;
 *      UINT32              ui4_nw_mask;
 *      UINT32              ui4_option_mask;
 *      UINT16              ui2_prog_id;
 *      UINT16              ui2_tsl_id;
 *      UINT16              ui2_tsl_rec_id;
 *      UINT16              ui2_nwl_id;
 *      UINT16              ui2_nwl_rec_id;
 *      UINT16              ui2_satl_id;
 *      UINT16              ui2_satl_rec_id;
 *      BRDCST_TYPE_T       e_brdcst_type;
 *      SVL_SERVICE_TYPE_T  e_serv_type;
 *      CHAR                ac_name[MAX_PROG_NAME_LEN+1];
 *  } SVL_REC_HEADER_T ;
 *  @endcode
 *  @li@c  ui4_channel_id                - Specify the channel number for this
 *                                         service.
 *  @li@c  ui4_nw_mask                   - Specify mask to indicate a group
 *                                         which this service belongs to.
 *  @li@c  ui4_option_mask               - Supplementary mask to indicate a
 *                                         group which this service belongs to.
 *  @li@c  ui2_prog_id                   - Channel identification number for
 *                                         this service.
 *  @li@c  ui2_tsl_id                    - Specify the TSL database that store
 *                                         the transport stream data.
 *  @li@c  ui2_tsl_rec_id                - Specify the record in the TSL
 *                                         database that store the transport
 *                                         stream information for this service.
 *  @li@c  ui2_nwl_id                    - Specify the NWL database that store
 *                                         the network data.
 *  @li@c  ui2_nwl_rec_id                - Specify the record in the NWL
 *                                         database that store the network
 *                                         information for this service.
 *  @li@c  ui2_satl_id                   - Specify the SATL database that store
 *                                         the satellite data.
 *  @li@c  ui2_satl_rec_id               - Specify the record in the SATL
 *                                         database that store the satellite
 *                                         information for this service.
 *  @li@c  e_brdcst_type                 - Specify the broadcast protocol:  DVB,
 *                                         ATSC, SCTE, ISDB, FM radio or analog.
 *  @li@c  e_serv_type                   - Specify the service:  TV, Radio, or
 *                                         Application.
 *  @li@c  ac_name                       - Specify the channel name.
 */
/*------------------------------------------------------------------*/
typedef struct _SVL_REC_HEADER_T
{
    UINT32              ui4_channel_id;
    UINT32              ui4_nw_mask;
    UINT32              ui4_option_mask;
    UINT16              ui2_prog_id;
    UINT16              ui2_tsl_id;
    UINT16              ui2_tsl_rec_id;
    UINT16              ui2_nwl_id;
    UINT16              ui2_nwl_rec_id;
    UINT16              ui2_satl_id;
    UINT16              ui2_satl_rec_id;
    BRDCST_TYPE_T       e_brdcst_type;
    SVL_SERVICE_TYPE_T  e_serv_type;
    CHAR                ac_name[MAX_PROG_NAME_LEN+1];
    UINT8               aui1_custom_data[MW_SVL_HEADER_REC_CUSTOM_BYTES];
#ifdef ANDROID
    INT8                ai1_private_data[20];
#endif
} SVL_REC_HEADER_T ;

/* Structure for the SVL data record */
/*------------------------------------------------------------------*/
/*!  @brief  Structure for the SVL data record
 *  @code
 *  typedef struct _SVL_REC_T
 *  {
 *      UINT16   ui2_svl_rec_id;
 *      union
 *      {
 *          SVL_REC_HEADER_T     t_rec_hdr;
 *          UINT8  _rec_header_padding[SVL_REC_HEADER_PADDING];
 *      } uheader;
 *      union
 *      {
 *          SVL_ATSC_T      t_atsc;
 *          SVL_DVB_T       t_dvb;
 *          SVL_ANALOG_T    t_analog;
 *          SVL_ISDB_T      t_isdb;   
 *          SVL_DTMB_T      t_dtmb;
 *          SVL_FMRDO_T     t_fmrdo;  
 *          UINT8  _rec_data_padding[SVL_REC_DATA_PADDING];
 *      } u_data;
 *  } SVL_REC_T;
 *  @endcode
 *  @li@c  ui2_svl_rec_id                - Specify the SVL record id.
 *  @li@c  t_rec_hdr                     - Specify the common information for
 *                                         this Service record.
 *  @li@c  _rec_header_padding           - Padding for the record header union
 *                                         structure.
 *  @li@c  t_atsc                        - Specify the ATSC specific data in
 *                                         this service record.
 *  @li@c  t_dvb                         - Specify the DVB specific data in this
 *                                         service record.
 *  @li@c  t_analog                      - Specify analog specific data.
 *  @li@c  t_isdb                        - Specify the ISDB specific data in
 *                                         this service record.
 *  @li@c  t_dtmb                        - Specify the DTMB specific data in
 *                                         this service record.
 *  @li@c  t_fmrdo                       - Specify FM Radio specific data in
 *                                         this service record.
 *  @li@c  _rec_data_padding             - Padding for the record data union
 *                                         structure.
 */
/*------------------------------------------------------------------*/
typedef struct _SVL_REC_T
{
    UINT16   ui2_svl_rec_id;
    union
    {
        SVL_REC_HEADER_T     t_rec_hdr;

        /* PADDING for the record header union structure. */
        UINT8  _rec_header_padding[SVL_REC_HEADER_PADDING];
    } uheader;

    union
    {
        SVL_ATSC_T      t_atsc;
        SVL_DVB_T       t_dvb;
        SVL_ANALOG_T    t_analog;
        SVL_ISDB_T      t_isdb;   /* For ISDB */
        SVL_DTMB_T      t_dtmb;   /* For DTMB */
        SVL_FMRDO_T     t_fmrdo;  /* For FM Radio */

        /* PADDING for the record data union structure. */
        UINT8  _rec_data_padding[SVL_REC_DATA_PADDING];
    } u_data;
} SVL_REC_T;


/*------------------------------------------------------------------*/
/*!  @brief  Enumeration for SVL record comparing information
 *  @code
 *  typedef enum
 *  {
 *      SVL_UNKNOWN_SEARCH_PARM  = 0,
 *      SVL_COMPARE_BY_CHAN_NUM,
 *      SVL_SPECIFIED_BY_CLIENT
 *  }  SVL_COMPARE_INFO_T ;
 *  @endcode
 *  @li@c  SVL_UNKNOWN_SEARCH_PARM       - Unknown search parameter.
 *  @li@c  SVL_COMPARE_BY_CHAN_NUM       - Compare by channel number.
 *  @li@c  SVL_SPECIFIED_BY_CLIENT       - Client specified comparison.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SVL_UNKNOWN_SEARCH_PARM  = 0,
    SVL_COMPARE_BY_CHAN_NUM,
    SVL_SPECIFIED_BY_CLIENT
}  SVL_COMPARE_INFO_T ;

/*
 * Structure for reading/writing client/customer private data
 * associated with SVL record.
 */
/*------------------------------------------------------------------*/
/*!  @brief  Structure for reading/writing client/customer private data
 *          associated with SVL record.
 *  @code
 *  typedef struct _SVL_CLIENT_DATA_T_
 *  {
 *    UINT8*     pui1_data_buf;  
 *    UINT16     ui2_data_tag;   
 *    UINT8      ui1_buf_len;    
 *    UINT8      ui1_data_len;   
 *  }  SVL_CLIENT_DATA_T ;
 *  @endcode
 *  @li@c  pui1_data_buf                 - Pointer to buffer containing customer
 *                                         data.  On 'set' operation, the buffer
 *                                         contains the customer data of
 *                                         'ui1_data_len'.  Currently, the
 *                                         maximum len of the object data is
 *                                         limited to 255 bytes. Buffer with
 *                                         data greater than 255 bytes are
 *                                         truncated.
 *                                         On 'get' operation, SVL library will
 *                                         copy customer data of to this buffer,
 *                                         If the buffer is too small, only data
 *                                         up to the len of 'ui1_buf_len' is
 *                                         copied out.  The paramter
 *                                         'ui1_data_len' will be set to
 *                                         indicate the total len of customer
 *                                         data in the internal SVL record.
 *  @li@c  ui2_data_tag                  - optional tag parameter associated
 *                                         with the customer data
 *                                         On 'set' operation, this value is
 *                                         provided by the caller.
 *                                         On 'get' operation, SVL library will
 *                                         copy this tag value from the internal
 *                                         SVL record to this variable.
 *  @li@c  ui1_buf_len                   - Specifies the size of the buffer
 *                                         referencedby the 'pui1_data_buf'
 *                                         pointer.
 *  @li@c  ui1_data_len                  - On 'set' operation, this parameter
 *                                         specifies the the len of valid data
 *                                         in 'pui1_cust_data' buffer.  The
 *                                         caller provides this value.
 *                                         On 'get' operation, this field is
 *                                         filled by the SVL library to indicate
 *                                         the len of customer data in the SVL
 *                                         record.  If this value is greater
 *                                         than 'ui1_buf_len', then only data up
 *                                         to 'ui1_buf_len' is copied.
 */
/*------------------------------------------------------------------*/
typedef struct _SVL_CLIENT_DATA_T_
{

  UINT8*     pui1_data_buf;  /* Pointer to buffer containing customer data.
                                On 'set' operation, the buffer contains the
                                customer data of 'ui1_data_len'.  Currently,
                                the maximum len of the object data is limited
                                to 255 bytes. Buffer with data greater than 255
                                bytes are truncated.

                                On 'get' operation, SVL library will copy
                                customer data of to this buffer,
                                If the buffer is too small, only data up to
                                the len of 'ui1_buf_len'
                                is copied out.  The paramter 'ui1_data_len'
                                will be set to indicate the
                                total len of customer data in the internal
                                SVL record.
                             */

  UINT16     ui2_data_tag;   /* optional tag parameter associated with the
                                customer data
                                On 'set' operation, this value is provided
                                by the caller..
                                On 'get' operation, SVL library will copy
                                this tag value from the internal SVL record
                                to this variable.
                             */

  UINT8      ui1_buf_len;    /* Specifies the size of the buffer reference'd
                                by the 'pui1_data_buf' pointer.
                             */

  UINT8      ui1_data_len;   /* On 'set' operation, this parameter specifies
                                the the len of valid data in 'pui1_cust_data'
                                buffer.  The caller provides this value.

                                On 'get' operation, this field is filled
                                by the SVL library to indicate the len of
                                customer data in the SVL record.  If this
                                value is greater than 'ui1_buf_len', then
                                only data up to 'ui1_buf_len' is copied.
                             */
}  SVL_CLIENT_DATA_T ;


/*
 * Structure for reading/writing multi-language associated with SVL record.
 */
/*------------------------------------------------------------------*/
/*!  @brief  Structure for reading/writing multi-language associated with SVL record.
 *  @code
 *  typedef struct _SVL_MULTI_LANG_T_
 *  {
 *    UINT8*     pui1_data_buf;  
 *    UINT16     ui2_data_tag;   
 *    UINT8      ui1_buf_len;    
 *    UINT8      ui1_data_len;   
 *  }  SVL_MULTI_LANG_T ;
 *  @endcode
 *  @li@c  pui1_data_buf                 - Pointer to buffer containing multi-language
 *                                         data.  On 'set' operation, the buffer
 *                                         contains the multi-language data of
 *                                         'ui1_data_len'.  Currently, the
 *                                         maximum len of the object data is
 *                                         limited to 255 bytes. Buffer with
 *                                         data greater than 255 bytes are
 *                                         truncated.
 *                                         On 'get' operation, SVL library will
 *                                         copy multi-language data of to this buffer,
 *                                         If the buffer is too small, only data
 *                                         up to the len of 'ui1_buf_len' is
 *                                         copied out.  The paramter
 *                                         'ui1_data_len' will be set to
 *                                         indicate the total len of multi-language
 *                                         data in the internal SVL record.
 *  @li@c  ui2_data_tag                  - optional tag parameter associated
 *                                         with the multi-language data
 *                                         On 'set' operation, this value is
 *                                         provided by the caller.
 *                                         On 'get' operation, SVL library will
 *                                         copy this tag value from the internal
 *                                         SVL record to this variable.
 *  @li@c  ui1_buf_len                   - Specifies the size of the buffer
 *                                         referencedby the 'pui1_data_buf'
 *                                         pointer.
 *  @li@c  ui1_data_len                  - On 'set' operation, this parameter
 *                                         specifies the the len of valid data
 *                                         in 'pui1_cust_data' buffer.  The
 *                                         caller provides this value.
 *                                         On 'get' operation, this field is
 *                                         filled by the SVL library to indicate
 *                                         the len of multi-language data in the SVL
 *                                         record.  If this value is greater
 *                                         than 'ui1_buf_len', then only data up
 *                                         to 'ui1_buf_len' is copied.
 */
/*------------------------------------------------------------------*/

typedef struct _SVL_MULTI_LANG_T_
{
  UINT8*     pui1_data_buf; 
  UINT16     ui2_data_tag;   
  UINT8      ui1_buf_len;   
  UINT8      ui1_data_len;   
} SVL_MULTI_LANG_T;

/*
 * Structure for memory dump.
 */
/*------------------------------------------------------------------*/
/*!  @brief  Structure for memory dump
 *  @code
 *  typedef struct _SVL_MEM_DUMP_T_
 *  {
 *      UINT16      ui2_svl_id;
 *      CHAR        ps_svl_name[MAX_SVL_NAME_LEN+1];
 *      VOID*       pv_data;
 *      UINT32      ui4_data_len;
 *  }  SVL_MEM_DUMP_T ;
 *  @endcode
 *  @li@c  ui2_svl_id                    - ID of the Service List
 *  @li@c  ps_svl_name                   - Name of the Service List
 *  @li@c  pv_data                       - Memory dump of SVL database
 *  @li@c  ui4_data_len                  - Size of memory allocated for pv_data
 */
/*------------------------------------------------------------------*/
typedef struct _SVL_MEM_DUMP_T_
{
    UINT16      ui2_svl_id;
    CHAR        ps_svl_name[MAX_SVL_NAME_LEN+1];
    VOID*       pv_data;
    UINT32      ui4_data_len;
}  SVL_MEM_DUMP_T ;


/*---------------------------------------------------------------------------
 Type definition
----------------------------------------------------------------------------*/

/*
  typedef for the SVL handle callback function.  This fucntion
  is implemented by the client of SVL.
*/
/*------------------------------------------------------------------*/
/*! @brief  Typedef for the SVL handle callback function.  This fucntion
 *          is implemented by the client of SVL.
 *  @param  [in]  h_svl             - Handle to the SVL object.
 *  @param  [in]  e_cond            - The status flag for the SVL object.
 *  @param  [in]  ui4_reason        - The reason for the notification.
 *  @param  [in]  pv_tag            - Private tag for the client.
 *  @param  [in]  ui4_data          - Data pass back to the application.
 *  @note   It is very important that the client MUST NOT try to read the SVL in
 *          the context of notification function.  Otherwise, a `Dead lock'
 *          situation would arise.  The client should only send a message to
 *          itself from the notification function or set a condition flag, and
 *          execute the read operation outside the context of the notification
 *          function.
 *          @par
 *          Client should also avoid doing CPU intensive task in the
 *          notification function.  If CPU intensive task needs to be performed,
 *          then the notification function should send a message to the client
 *          thread, and have the CPU intensive task executed in the client
 *          thread context.
 *  @if DETAIL_DESCRIPTION_TYPE_C
 *  @see    c_svl_create, c_svl_open, c_svl_open_by_name, c_svl_fs_load
 *  @else
 *  @see    x_svl_create, x_svl_open, x_svl_open_by_name, x_svl_fs_load
 *  @endif
 *  @return None
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_svl_nfy_fct)
(
    HANDLE_T            h_svl,
    SVL_COND_T          e_cond,
    UINT32              ui4_reason,
    VOID*               pv_tag,
    UINT32              ui4_data
);

/*
  typedef for SVL user defined compare function that implement customized
  sorting order of SVL record.  This function is called during the
  sorting operation.
*/
/*------------------------------------------------------------------*/
/*! @brief  Typedef for SVL user defined compare function that implement
 *          customized sorting order of SVL record.  This function is called
 *          during the sorting operation.
 *  @param  [in]  pt_svl_rec_1      - Reference a SVL record structure.
 *  @param  [in]  pt_svl_rec_2      - Reference a SVL record structure.
 *  @param  [in]  pv_tag            - Private tag info pass to the comparison
 *                                    function.
 *  @if DETAIL_DESCRIPTION_TYPE_C
 *  @see    c_svl_reg_cmp_fcts
 *  @else
 *  @see    x_svl_reg_cmp_fcts
 *  @endif
 *  @return Comparison result
 *  @retval RC_SMALLER          - `pt_svl_rec_1' is less than `pt_svl_rec_2'.
 *  @retval RC_EQUAL            - `pt_svl_rec_1' is equal to  `pt_svl_rec_2'.
 *  @retval RC_GREATER          - `pt_svl_rec_1' is greater than `pt_svl_rec_2'.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_svl_sort_cmp)
(
    const SVL_REC_T*     pt_svl_rec_1,
    const SVL_REC_T*     pt_svl_rec_2,
    VOID*                pv_tag
);

/*
  typedef for SVL user defined search compare function. This
  function will be called by the SVL library when performing
  binary search operation on sorted SVL records.
  For binary search operation, this function must implement the
  same logic order as implemented by the x_svl_sort_cmp function.
*/
/*------------------------------------------------------------------*/
/*! @brief  Typedef for SVL user defined search compare function. This
 *          function will be called by the SVL library when performing
 *          binary search operation on sorted SVL records.
 *          For binary search operation, this function must implement the
 *          same logic order as implemented by the x_svl_sort_cmp function.
 *  @param  [in]  pt_svl_rec        - Reference a SVL record structure.
 *  @param  [in]  pv_data_to_compare - Value parameter to compare with SVL
 *                                     record `pt_svl_rec'.
 *  @param  [in]  pv_tag            - Private tag info pass to the comparison
 *                                    function.
 *  @if DETAIL_DESCRIPTION_TYPE_C
 *  @see    c_svl_reg_cmp_fcts
 *  @else
 *  @see    x_svl_reg_cmp_fcts
 *  @endif
 *  @return Comparison result
 *  @retval RC_SMALLER      - `pt_svl_rec_1' is less than `pv_data_to_compare'.
 *  @retval RC_EQUAL        - `pt_svl_rec_1' is equal to `pv_data_to_compare'.
 *  @retval RC_GREATER      - `pt_svl_rec_1' is greater than `pt_svl_rec_2'.
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_svl_search_cmp)
(
    const SVL_REC_T*     pt_svl_rec,
    const VOID*          pv_data_to_compare,
    VOID*                pv_tag
);


/*----------------------------------------------------------------------------*/
/*! @} */
/*----------------------------------------------------------------------------*/

/*! @page pageMW_SVL Service List Detailed Description
 *  @par
 *  @section secMW_SVL_1  1. Introduction
 *  @par
 *      Service lists are databases storing records with information describing
 *      services.
 *      The SVL (Service List) module under Middleware manages the SVL databases and
 *      provide API to create, delete, modify, store to/load from flash the SVL as
 *      well as various query functions such as get_rec_by_XX and iterate_rec to
 *      return matching service list records with specified condition.
 *      SVL is one component of SVL/TSL/NWL databases, which are filled with data by
 *      @ref groupMW_SVL_BLDR "SVL Builder" after channel scan.
 *  @par
 *  @section secMW_SVL_2  2. Concept
 *  @par
 *      For better understand, readers would better bear knowledge of MPEG-2 PSI,
 *      DVB SI, ATSC PSIP and related DTV standards. Here several basic entities in
 *      different layers are described in brief.
 *  @par
 *      -   <b>Transport Stream</b>:
 *      <br>
 *      a stream definition according to 13818-1 for transmitting one or more
 *      programs in error-prone environments. PSI and PES are multiplexed by means of
 *      188-byte or 204-byte TS packets. One TS corresponds to 1 physical channel.
 *  
 *  @par
 *      -   <b>Service</b>:
 *      <br>
 *      sequence of programs under the control of a broadcaster which can be
 *      broadcast as part of a schedule (DVB). One service corresponds to one logical channel.
 *      Services contain ES (and necessary SI to group them together), and are
 *      multiplexed into TS. A service is made up of events over time.
 *  
 *  @par
 *      -   <b>Component</b>:
 *      <br>
 *      one or more entities which together make up an event (DVB), by
 *      means of elementary streams (ES) of video / audio / data. One component corresponds to one ES.
 *  
 *  @par
 *      -   <b>Network</b>:
 *      <br>
 *      collection of MPEG-2 Transport Stream (TS) multiplexes transmitted on
 *      a single delivery system (DVB). One network corresponds to one service provider on a
 *      specific tuner source (terrestrial / cable / satellite).
 *  
 *  @par
 *      -   <b>Broadcaster</b>:
 *      <br>
 *      organization which assembles a sequence of events or programs to
 *      be delivered to the viewer based upon a schedule (DVB). One broadcaster corresponds to one
 *      service provider.
 *  
 *  @par
 *      @image html svl_300468_hier.gif "Figure 1: Hierarchy of digital broadcasting entities (from EN 300468)"
 *  @par
 *      <table>
 *      <tr>
 *      <th>Entity</th><th>Described in what table</th><th>Identifier</th><th>MW module counterpart</th><th>
 *      AP perception
 *      </th>
 *      </tr>
 *      <tr>
 *      <td>Broadcaster</td><td>BIT (ISDB)</td><td>Broadcaster ID</td><td></td><td>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>Network</td><td>NIT</td><td>Network ID</td><td>NWL (EU only)</td><td>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>Transport Stream</td><td>PAT, SDT<br>VCT (ATSC)</td><td>TS ID (+ ON ID)</td><td>TSL</td><td>
 *      Scan index
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>Service</td><td>PMT<br>VCT(ATSC)</td><td>Service ID (program number)<br>Source ID (ATSC)</td><td>SVL</td><td>
 *      Channel number
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>Stream Component</td><td>PMT<br>VCT (ATSC)</td><td>ES PID<br>Component tag</td><td>SCDB</td><td>
 *      Audio / subtitle language, etc
 *      </td>
 *      </tr>
 *      </table>
 *      <center><b>Table 1: Summary of digital broadcasting entities</b></center>
 *  @par
 *  @section secMW_SVL_3  3. Brief Design
 *  @par
 *  @subsection ssecMW_SVL_3_1  3.1. The Three Lists
 *  @par
 *      The SVL is one component of SVL/TSL/NWL databases.
 *      -   @ref groupMW_SVL "SVL" stores information about services.
 *      -   @ref groupMW_TSL "TSL" stores information about transport stream and tuning parameters.
 *      -   @ref groupMW_NWL "NWL" stores information about networks.
 *  
 *  @par
 *      These three lists correspond to the three layers of digital broadcasting
 *      service delivery model in DVB.  As there is no concept of networks in ATSC,
 *      NWL is not used in US models.
 *  @par
 *      Due to the intrinsic hierarchical relation among services, transport streams
 *      and networks, there are also similar reference relations between SVL and TSL,
 *      TSL and NWL.  As a network can contain multiple transport streams, a NWL
 *      record can be referred to by one or many TSL records. Similarly, as a
 *      transport stream can contain multiple services, a TSL record can be referred
 *      to by one or many SVL records. On the contrary, a SVL record must refer to one
 *      and only one TSL record, and similarly a TSL record must refer to one and only
 *      one NWL record if the transport stream is in a DVB network.
 *  @par
 *      Any of SVL / TSL / NWL list can have multiple instances, which are identified by
 *      list ID's.
 *  @par
 *      The following table is the overview of three main databases in Middleware,
 *      namely @ref groupMW_TSL "TSL" , @ref groupMW_SVL "SVL" and @ref groupMW_SCDB "SCDB" .
 *      @ref groupMW_NWL "NWL" is not described here as it is only used for DVB scanning
 *      and is transparent to Applications.
 *  @par
 *      <table>
 *      <tr>
 *      <th></th><th>TSL</th><th>SVL</th><th>
 *      SCDB
 *      </th>
 *      </tr>
 *      <tr>
 *      <td><b>Record for</b></td><td>Transport Stream</td><td>Service</td><td>
 *      Stream Component
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>
 *      <b>Filled after</b>
 *      </td>
 *      <td colspan="2">
 *      Scan (menu), Single scan (nav)
 *      </td>
 *      <td>
 *      Select service / input source
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>
 *      <b>Storage</b>
 *      </td>
 *      <td colspan="2">
 *      NOR/NAND flash, dual bank opt.
 *      </td>
 *      <td>
 *      No persistent storage
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>
 *      <b>Content Filler (DTV)</b>
 *      </td>
 *      <td colspan="2">
 *      SVL Builder DVB / DVBC / ATSC / CQAM / ISDB Engine
 *      </td>
 *      <td>
 *      Connection Manager Brdcst DVB / ATSC / ISDB Handler
 *      </td>
 *      </tr>
 *      <tr>
 *      <td><b>Filling Rule (DTV)</b></td><td>Scan map, tuner lock, table info</td><td>Table info (SDT/VCT,...)</td><td>
 *      Table info (PMT), fake dual mono (2 mono (+ stereo) )
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>
 *      <b>Content Filler (ATV)</b>
 *      </td>
 *      <td colspan="2">
 *      SVL Builder PAL-SECAM / NTSC Engine
 *      </td>
 *      <td>
 *      Connection Manager Analog Broadcast Handler
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>
 *      <b>Filling Rule (ATV)</b>
 *      </td>
 *      <td colspan="2">
 *      Scan map / frequency formula, tuner/TVD lock, fake (NTSC)
 *      </td>
 *      <td>
 *      Fake video/audio record, info updated by TVD. CC if VBI
 *      </td>
 *      </tr>
 *      <tr>
 *      <td><b>Cont. Filler (Input Src)</b></td><td></td><td></td><td>
 *      Connection Manager AVC Handler
 *      </td>
 *      </tr>
 *      </table>
 *      <center><b>Table 2: TSL/SVL/SCDB Overview</b></center>
 *  @par
 *  @subsection ssecMW_SVL_3_2  3.2. Functional Description
 *  @par
 *      As described before, the SVL object stores information of services, such as
 *      MPEG-2 program number, transport stream id, channel number, and other
 *      information that describes a service.
 *  @par
 *      Each service record also contains reference to a TSL (Transport Stream List)
 *      object and a TSL record stored in that TSL object.  The TSL record contains
 *      descriptions for the TS (broadcast type, medium, ON ID, TS ID, etc), as well
 *      as tuning related parameters (frequency, modulation mode, etc) for a given
 *      service.
 *  @par
 *      The NWL (Network list) stores information about a network, e.g., network id
 *      and network name.
 *  @par
 *      The three lists SVL/TSL/NWL provide to the user similar interfaces to
 *      manipulate the database, such as creating, deleting the database, adding,
 *      deleting and updating records, searching and retrieving records meeting
 *      specified criteria, saving and loading from persistent storage, etc.  The
 *      @ref groupMW_CDB "Core DB module" in Middleware is intended to be used as the
 *      underlying database manager.
 *  @par
 *  @subsection ssecMW_SVL_3_3  3.3. Virtual Network Support
 *  @par
 *      Despite of the Network List (NWL), which correspond to the network concept in
 *      DVB and is constructed according to the transmitted SI information, user can
 *      also get a list of services that belong to a certain virtual networks.
 *      Virtual networks are built on top of SVL records, instead of TSL records on
 *      top of which NWL records stays.  Virtual networks are not stored in separate
 *      list structures, but make use of the network mask field of SVL record, and
 *      each mask bit corresponds to one virtual network.
 *  @par
 *      The differences between virtual networks and networks as in NWL (here we call
 *      it `real network') is enumerated as follows.
 *  @par
 *      <table>
 *      <tr>
 *      <th></th><th>Virtual Networks</th><th>
 *      Real Networks
 *      </th>
 *      </tr>
 *      <tr>
 *      <td><b>Origin</b></td><td>User specified</td><td>
 *      From SI information (DVB only)
 *      </td>
 *      </tr>
 *      <tr>
 *      <td><b>List Structure</b></td><td>Virtual (re-use SVL)</td><td>
 *      NWL
 *      </td>
 *      </tr>
 *      <tr>
 *      <td><b>A Network Corresponds to</b></td><td>A bit in the network mask</td><td>
 *      A record in NWL
 *      </td>
 *      </tr>
 *      <tr>
 *      <td><b>A Network is Mapped to</b></td><td>Multiple services</td><td>
 *      Multiple transport streams
 *      </td>
 *      </tr>
 *      <tr>
 *      <td><b>Type of the Mapping</b></td><td>Many-to-many</td><td>
 *      One-to-many
 *      </td>
 *      </tr>
 *      </table>
 *      <center><b>Table 3: Difference between virtual and real networks</b></center>
 *  @par
 *      The relationship between SVL/TSL/NWL and virtual networks are shown in Figure
 *      2.
 *  @par
 *      @image html svl_vnet.gif "Figure 2: Example Relations between SVL/TSL/NWL/VNET"
 *  @par
 *  @subsection ssecMW_SVL_3_4  3.4. SVL Record Data
 *  @par
 *      A record in the SVL database consists of a service record identifier, a header
 *      section, and a data section.  The service record identifier is a 16 bits
 *      unsigned value which is used to identify a specific service entry.  The
 *      service record must be unique for all the record with in a SVL database.
 *  @par
 *      The header section contains generic information about this service. The data
 *      section containing protocol specific information related to the service (e.g.,
 *      a service can belong to DVB, or Analog, or ATSC protocol).
 *  @par
 *      For details on meaning of member fields, see the definition of SVL_REC_T,
 *      SVL_REC_HEADER_T, SVL_ATSC_T, SVL_DVB_T, SVL_ANALOG_T, and etc.
 *  @par
 *  @subsection ssecMW_SVL_3_5  3.5. Interpretation of Channel ID and Network mask
 *  @par
 *      For Channel ID, the current format of ui4_channel_id is shown as follows:
 *      <table>
 *      <tr>
 *      <th>High 14 bits</th><th>10 bits</th><th>1 bit</th><th>1 bit</th><th>
 *      Low 6 bits
 *      </th>
 *      </tr>
 *      <tr>
 *      <td>Major Channel Number</td><td>Minor Channel Number</td><td>One-Part Flag</td><td>Non-PSIP Flag</td><td>
 *      Channel Index
 *      </td>
 *      </tr>
 *      </table>
 *  @par
 *      All of DVB, ATSC and ISDB/SBTVD share the same channel ID format, and use
 *      common macros to access the abovementioned fields.
 *  @par
 *      For Network Mask, currently it only represents `virtual networks'. The `real
 *      networks' mentioned in the previous section are in fact different broadcast
 *      media, which are represented by multiple instances of SVL/TSL/NWL database,
 *      and services belonging to different broadcast media are stored into different
 *      Service Lists. The virtual networks are designated by the user, and could be
 *      regarded as a label applied to the service for classification. See @ref ssecMW_SVL_3_3 for
 *      details.
 *  @par
 *      The network mask value could also be used as matching criteria for searching
 *      records, as described in the previous section.
 *  @par
 *      A new mask variable, the option mask, has been created in order to make space
 *      for the increasing number of virtual networks defined.  `Labels' that are only
 *      used as a control flag and not used for classifying or searching should be
 *      defined as `virtual options' instead of virtual networks, in order not to
 *      occupy one of the only 32 bits in the network mask.
 *  @par
 *      All the abovementioned interpretation are implemented as macros in
 *      @ref groupMW_SVL_BLDR "u_sb.h" .  SVL itself does not care about the value of data
 *      fields.
 *  @par
 *  @subsection ssecMW_SVL_3_6  3.6. Persistent Storage
 *  @par
 *      This section describes saving and re-loading a SVL object to or from the file
 *      system.  The SVL API's provide functions to write the content of service list
 *      (e.g., data records and metadata) to file, and to create a service list object
 *      by loading the content from a previous saved file.  Additional API's allows
 *      deleting SVL file on storage, and finding out what SVL files are available.
 *  @par
 *      The SVL persistent storage API will utilize @ref groupMW_CDB "CDB"
 *      @ref db_save_to_file "save" / @ref db_load_from_file "load" API's to write/read
 *      SVL records to/from the filesystem.
 *  @par
 *  @subsubsection sssecMW_SVL_3_6_1  3.6.1. File Path Specification
 *  @if DETAIL_DESCRIPTION_TYPE_C
 *  @par
 *      In file system related API's, e.g., c_svl_fs_load(), c_svl_fs_store(),
 *      c_svl_fs_del(), c_svl_fs_qry(), the first argument specifies the file path to
 *      store SVL records.  The file path specified must be one of the two file types:
 *      a regular directory, or a RAW character file.
 *  @else
 *  @par
 *      In file system related API's, e.g., x_svl_fs_load(), x_svl_fs_store(),
 *      x_svl_fs_del(), x_svl_fs_qry(), the first argument specifies the file path to
 *      store SVL records.  The file path specified must be one of the two file types:
 *      a regular directory, or a RAW character file.
 *  @endif
 *  @par
 *      If the path specified is a regular directory, then SVL library will
 *      create/open a file under that directory to load/store the SVL object, one SVL
 *      object per file.  If the path specified is a RAW character file, then the SVL
 *      library will write/read the SVL object to/from the RAW file.   Multiple SVL
 *      objects can be store in a single RAW file.
 *  @par
 *      In MediaTek turnkey solution, the channel list file path specified by AP is
 *      defined as <b>DUAL_BANK_SVL_TSL_FILE_MAIN</b> (for main bank) and
 *      <b>DUAL_BANK_SVL_TSL_FILE_SUB</b> (for sub bank if dual bank is enabled), which
 *      will use <tt>/dev/nor_2</tt> & <tt>/dev/nor_4</tt> is NOR flash is used, and
 *      <tt>/dev/nand_2</tt> & <tt>/dev/nand_4</tt> for NAND flash.
 *  @par
 *  @subsubsection sssecMW_SVL_3_6_2  3.6.2. RAW File System Format
 *  @par
 *      To store multiple SVL objects into one RAW file, a simple file system is
 *      constructed within the file.  The RAW file system stores the metadata section
 *      at the start of the file, and the following content is filled with multiple
 *      file objects, with possible space gaps between them.
 *  @par
 *      The format of the RAW file system is shown below:
 *  @par
 *      <table>
 *      <tr>
 *      <th width="50%">
 *      NAME
 *      </th>
 *      <th width="25%">
 *      C-Type
 *      </th>
 *      <th width="25%">
 *      Size (bytes)
 *      </th>
 *      </tr>
 *      <tr>
 *      <td colspan="3">
 *      <center><b>METADATA SECTION (1024 bytes)</b></center>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>magic number (start)</td><td>UINT32</td><td>
 *      4
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>raw file size</td><td>UINT32</td><td>
 *      4
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>file info & version</td><td>UINT32</td><td>
 *      4
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>header size</td><td>UINT16</td><td>
 *      2
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>max number of file objects</td><td>UINT16</td><td>
 *      2
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>number of file objects</td><td>UINT16</td><td>
 *      2
 *      </td>
 *      </tr>
 *      <tr>
 *      <td colspan="3">
 *      <b>for (i = 0; i < number of file objects; i++)</b>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td colspan="3">
 *      <b>{</b>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>file offset</td><td>UINT32</td><td>
 *      4
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>file size</td><td>UINT32</td><td>
 *      4
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>file object ID</td><td>UINT32</td><td>
 *      4
 *      </td>
 *      </tr>
 *      <tr>
 *      <td colspan="3">
 *      <b>}</b>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>dummy filler bytes</td><td>&nbsp;</td><td>
 *      1024 - 18 - 8 - 12 * n
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>CRC number</td><td>UINT32</td><td>
 *      4
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>magic number (end)</td><td>UINT32</td><td>
 *      4
 *      </td>
 *      </tr>
 *      <tr>
 *      <td colspan="3">
 *      <center><b>DATA SECTION</b></center>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td colspan="3">
 *      <b>for (i = 0; i < number of file objects; i++)</b>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td colspan="3">
 *      <b>{</b>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>space</td><td>&nbsp;</td><td>
 *      &nbsp;
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>data bytes</td><td>&nbsp;</td><td>
 *      &nbsp;
 *      </td>
 *      </tr>
 *      <tr>
 *      <td colspan="3">
 *      <b>}</b>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>space</td><td>&nbsp;</td><td>
 *      &nbsp;
 *      </td>
 *      </tr>
 *      </table>
 *  @par
 *  @subsubsection sssecMW_SVL_3_6_3  3.6.3. SVL File Format
 *  @par
 *      The data stored in the SVL file consist of the file format description,
 *      metadata section and data record section. The data section of SVL file
 *      consists of the CDB file object.  The CDB file object format is not described
 *      here.
 *  @par
 *      The content of the file is shown below:
 *  @par
 *      <table>
 *      <tr>
 *      <th width="50%">
 *      NAME
 *      </th>
 *      <th width="25%">
 *      C-Type
 *      </th>
 *      <th width="25%">
 *      Size (bytes)
 *      </th>
 *      </tr>
 *      <tr>
 *      <td colspan="3">
 *      <center><b>FILE FORMAT INFO</b></center>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>magic_number (start)</td><td>UINT32</td><td>
 *      4
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>file_version_number</td><td>UINT8</td><td>
 *      1
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>metadata_size</td><td>UINT16</td><td>
 *      2
 *      </td>
 *      </tr>
 *      <tr>
 *      <td colspan="3">
 *      <center><b>METADATA SECTION</b></center>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>ui2_svl_id</td><td>UINT16</td><td>
 *      2
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>ps_svl_name</td><td>CHAR*</td><td>
 *      17
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>ui4_data_size</td><td>UINT32</td><td>
 *      4
 *      </td>
 *      </tr>
 *      <tr>
 *      <td colspan="3">
 *      <center><b>DATA SECTION</b></center>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td colspan="3">
 *      <b>CDB File Object</b>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td colspan="3">
 *      <b>{</b>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>CDB file object</td><td>Array of UINT8 data (compressed)</td><td>
 *      1 * sizeof (CDB file)
 *      </td>
 *      </tr>
 *      <tr>
 *      <td colspan="3">
 *      <b>}</b>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td colspan="3">
 *      <center><b>END MARKER SECTION</b></center>
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>CRC number</td><td>UINT32</td><td>
 *      4
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>magic number (end)</td><td>UINT32</td><td>
 *      4
 *      </td>
 *      </tr>
 *      </table>
 *  @par
 *      The description for each field in the SVL file is described below:
 *  @par
 *      <table>
 *      <tr>
 *      <th width="40%">
 *      Name
 *      </th>
 *      <th width="60%">
 *      Description
 *      </th>
 *      </tr>
 *      <tr>
 *      <td>Magic Number (start)</td><td>
 *      Starting magic number.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>File version number</td><td>
 *      Version number for the SVL file, this value will initially be 1, and is increase every time the SVL file format changes.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>Metadata size</td><td>
 *      Size of metadata (in bytes) following this field
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>ui2_svl_id</td><td>
 *      Service List ID (must be unique)
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>ps_svl_name</td><td>
 *      Service List Name (must be unique)
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>ui4_data_size</td><td>
 *      Size of record data (in bytes) following this field.  For SVL file object, the data section size is the size of the CDB file object.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>CDB file object</td><td>
 *      The content of the CDB file object.
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>CRC number</td><td>
 *      The CRC number computed from the writing File Format Info and Metadata section
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>Magic Number (end)</td><td>
 *      End magic number, its value is ~(start_magic number).
 *      </td>
 *      </tr>
 *      </table>
 *  @par
 *      All C-type integer values are converted to Big-Endian format prior to writing
 *      to file.
 *  @par
 *  @if DETAIL_DESCRIPTION_TYPE_C
 *  @section secMW_SVL_4  4. Typical Case
 *  @par
 *      @dontinclude svlsample.c
 *  @subsection ssecMW_SVL_4_1  4.1. Create / delete / open / clean / close of SVL object (list)
 *  @par
 *      To create / delete / open / clean / close an SVL object from AP, simply call
 *      respective API's with intuitive naming. Below is an example.
 *  @par
 *      @skip Create
 *      @until svl_delete
 *  @par
 *      Creating SVL is usually called at acfg init time, or when a temporary SVL is
 *      needed, which will be deleted later after use. Opening a handle for SVL is a
 *      common operation when you need to operate on it, and the handle shall be
 *      closed when it is no longer used. Cleaning the SVL is usually invoked when
 *      resetting or cleaning storage from Menu operations.
 *  @par
 *  @subsection ssecMW_SVL_4_2  4.2. Add / delete / update a record in SVL object (list)
 *  @par
 *      To add a record in SVL, we first need to prepare the record structure, and
 *      ensure that it has a unique record ID. Below is a sample snippet.
 *  @par
 *      @skip Add
 *      @until CHK_FAIL
 *  @par
 *      After assigning data for other useful fields, we add the record to SVL.
 *  @par
 *      @until CHK_FAIL
 *  @par
 *      To update an existing SVL record, identified by its record ID, we call
 *      c_svl_update_rec() with 3rd argument (b_must_exist) TRUE.
 *  @par
 *      @skip Update
 *      @until CHK_FAIL
 *  @par
 *      If b_must_exist is FALSE, if the record ID of the SVL record is in use, then
 *      the record is updated; otherwise a new record is added to SVL.
 *  @par
 *      To delete a record from SVL, supplying the record ID is enough.
 *  @par
 *      @skip Delete
 *      @until CHK_FAIL
 *  @par
 *      The add / delete / update operations costs O(<i>n</i>) time, where n is the number of
 *      records in SVL.
 *  @par
 *  @subsection ssecMW_SVL_4_3  4.3. Get an SVL record by specified record ID or channel ID
 *  @par
 *      A record in SVL could be identified by its record ID, or channel ID. We can
 *      query like this.
 *  @par
 *      @skip Get
 *      @until CHK_FAIL
 *  @par
 *      Or this.
 *  @par
 *      @skip Get
 *      @until CHK_FAIL
 *  @par
 *      When querying SVL records by channel ID, a network mask could be specified,
 *      which will be used for filtering and only records with at least one matching
 *      bit in network mask will be returned. If there are multiple records matching
 *      the specified channel ID and network mask, the fourth argument, the index will
 *      be used to identify which will record is to be returned.
 *  @par
 *      The arguments of network mask and index are similiar for other get-by
 *      functions described in the following section, though in the case of channel
 *      ID, index is usually given 0 as no duplicate channel ID shall exist in an SVL
 *      object in normal case, while in other query conditions index are not usually
 *      0, and the number of available indices can be obtained by calling
 *      @ref c_svl_get_num_rec_by_channel "c_svl_get_num_rec_by_*()" .
 *  @par
 *      The pointer to ui4_ver_id given will be used to check SVL version changes, and
 *      if ui4_ver_id is not @ref SVL_NULL_VER_ID and SVL database version differs from
 *      what is given, @ref SVLR_MODIFIED will be returned. The version check is applied
 *      for all get operations, including ones described in the following sections.
 *  @par
 *      Getting record by record ID or channel ID costs O(<i>n</i>) time.
 *  @par
 *  @subsection ssecMW_SVL_4_4  4.4. Get an SVL record by brdcst_type, ts, nw, sat
 *  @par
 *      These operations are similiar to getting an SVL record by channel ID, and the
 *      number of records with matching query field (brdcst_type, ts, nw, sat) and
 *      network mask could be obtained by corresponiding get_num_rec function. One
 *      special case is there that when querying by broadcast type and the type given
 *      is @ref BRDCST_TYPE_UNKNOWN , broadcast type is disregarded and only network
 *      mask is taken into account for the query.
 *  @par
 *      Please refer to the function document of c_svl_get_rec_by_brdcst_type(),
 *      c_svl_get_num_rec_by_brdcst_type(), c_svl_get_rec_by_ts(),
 *      c_svl_get_num_rec_by_ts(), c_svl_get_rec_by_nw(), c_svl_get_num_rec_by_nw(),
 *      c_svl_get_rec_by_sat() and c_svl_get_num_rec_by_sat().
 *  @par
 *      All these get record operations cost O(<i>n</i> log <i>n</i>) time on the first call
 *      after SVL is modified, O(log <i>n</i>) if SVL is not modified, and O(1) for
 *      subsequent invocations where index is greater than 0, while all get number
 *      operations costs O(<i>n</i>) time.
 *  @par
 *  @subsection ssecMW_SVL_4_5  4.5. Get all SVL records by for-loop
 *  @par
 *      To get all SVL records by for-loop, we first need to get the total number of
 *      matching records, and then get records one by one through increasing index. We
 *      need to pay attention to the version ID, which is used to check whether SVL
 *      version change has occurred since the last get operation, and in case this
 *      happens, @ref SVLR_MODIFIED will be returned and we need to restart the
 *      iteration from beginning.
 *  @par
 *      @skip Get all
 *      @until while
 *  @par
 *      An alternative way which does not require checking @ref SVLR_MODIFIED return
 *      value and restarting iteration is, to surround the get-number operation and
 *      the for loop with c_svl_read_lock() and c_svl_read_unlock(), which can ensure
 *      that no write operation could occur during the iteration. However, we should
 *      not make the iteration time too long in this case, lest other threads trying
 *      to write to the SVL be blocked.
 *  @par
 *  @subsection ssecMW_SVL_4_6  4.6. Iterate the next SVL record by specified direction
 *  @par
 *      Sometimes we want to know the next SVL record on a specified direction of
 *      channel ID, e.g.  when user pressing CH+/-, in which case the next / previous
 *      channel will be tuned. c_svl_iterate_rec() and alike functions are suited for
 *      this purpose.
 *  @par
 *      Possible iteration directions defined in @ref SVL_ITERATE_T are:
 *      @ref SVL_CHANNEL_PLUS , @ref SVL_CHANNEL_MINUS , @ref SVL_CHANNEL_SMALLEST and
 *      @ref SVL_CHANNEL_LARGEST , or their alias, @ref SVL_NEXT_REC , @ref SVL_PREV_REC ,
 *      @ref SVL_FIRST_REC , @ref SVL_LAST_REC . Their meanings are intuitive and also
 *      stated in their descriptions.
 *  @par
 *      c_svl_iterate_rec() uses the given channel ID as the pivot. It first tries to
 *      find the SVL record with the matching channel ID, and in case there are
 *      multiple records with the same channel ID, the first record on the specified
 *      direction with non-matching channel ID will be focused. If no record with
 *      matching channel ID is found, the record with closest channel ID on the
 *      specified direction will be focused, or if the closest record could not be
 *      found, which means that the given channel ID is larger than the maximum
 *      channel ID when direction is @ref SVL_CHANNEL_PLUS "PLUS" , or smaller than the
 *      minimum channel ID when direction is @ref SVL_CHANNEL_MINUS "MINUS" ,
 *      @ref SVLR_END_OF_ITERATION will be returned.
 *  @par
 *      After the next SVL record on the specified direction has been focused, we keep
 *      reading records and focusing on the next one until a record with matching
 *      network mask has been found, which is then returned to caller with return
 *      value @ref SVLR_OK . If no record with matching network mask is found after the
 *      record with largest / smallest channel ID has been reached,
 *      @ref SVLR_END_OF_ITERATION will be returned.
 *  @par
 *      The caller shall check the return value and perform appropriate operation when
 *      @ref SVLR_END_OF_ITERATION is returned. Here is a sample usage.
 *  @par
 *      @skip Iterate
 *      @until CHK_FAIL
 *  @par
 *      The iterate operations cost O(<i>n</i> log <i>n</i>) time on the first call after SVL
 *      is modified and O(log <i>n</i>) otherwise.
 *  @par
 *      We can also use c_svl_iterate_rec() to modify all records in an SVL object.
 *      Here is an example.
 *  @par
 *      @skip Modify
 *      @until unlock
 *  @par
 *      The above example utilizes the fact that, in the current implementation, the
 *      version ID passed in for c_svl_iterate_rec() is not used for checking whether
 *      the database version has modified since the last call to c_svl_iterate_rec(),
 *      but rather the value of the version ID is interpreted as follows for version
 *      checking:
 *      -#  c_svl_iterate_rec() internally use another variable for version ID, which is
 *      initialized to @ref SVL_NULL_VER_ID . Therefore no matter whether the SVL
 *      database version has changed since the last call, c_svl_iterate_rec() will
 *      proceed in the same way.
 *      -#  If the version ID passed in is @ref SVL_NULL_VER_ID , and SVL database has
 *      been modified during the current call of c_svl_iterate_rec(), where multiple
 *      records are read one after another to find the one with matching network
 *      mask, the current search loop is terminated, and another loop is started
 *      from the beginning, from sorting and locating onward.
 *      -#  Otherwise if the version ID passed in is non-zero, and SVL database has been
 *      modified during the current call, @ref SVLR_END_OF_ITERATION will be returned.
 *      This case is very rare, though possible. However the previous example does
 *      not need to worry about that, as it has acquired a write lock and all
 *      modifications are under control of itself, without impact on the iteration.
 *  
 *  @par
 *      The abovementioned unusual behavior of version ID checking might be a
 *      historical bug, but it has become a feature now and several AP functions
 *      relies on it. However, since the SVL is modified in the loop body, every call
 *      to c_svl_iterate_rec will lead to a re-sorting, and thus the whole update-all
 *      operation would cost O(<i>n</i><sup>2</sup> log <i>n</i>) time, which is worse than
 *      what we could achieve if we first get a list of SVL record ID's stored in a
 *      temporary place, and simply use a for-loop calling c_svl_get_rec() and
 *      c_svl_update_rec(), which costs O(<i>n</i> log <i>n</i> + <i>n</i><sup>2</sup>), or
 *      O(<i>n</i><sup>2</sup>) time.
 *  @par
 *  @subsection ssecMW_SVL_4_7  4.7. Customizing SVL compare function and manual sorting
 *  @par
 *      SVL uses channel ID as the key field used for sorting, which is performed if
 *      necessary when calling c_svl_iterate_rec() and
 *      @ref c_svl_get_rec_by_brdcst_type "c_svl_get_rec_by_*()" operations except
 *      c_svl_get_rec_by_channel(), as well as searching, which is performed when
 *      calling c_svl_iterate_rec(). Both sorting and searching requires comparison.
 *      While by default SVL uses the numeral value of
 *      @ref SVL_REC_HEADER_T "ui4_channel_id" for interger comparison, applications can
 *      set customized compare functions for both sorting and search, either globally
 *      or for individual SVL handle only, by invoking c_svl_reg_cmp_fcts(), which
 *      takes from application two callback function pointers of type
 *      @ref x_svl_sort_cmp and @ref x_svl_search_cmp respectively.
 *  @par
 *      Below is an example.
 *  @par
 *      @skip Customize
 *      @until CHK_FAIL
 *  @par
 *      Custom sort compare function will be called with the first two arguments being
 *      pointers to the whole SVL record structure, which is not limited to merely the
 *      ui4_channel_id field. However, the second argument of the custom search
 *      compare function is not a complete SVL record structure but only a VOID *
 *      field, whose value is assigned to the given channel ID when calling
 *      c_svl_iterate_rec(). To be consistent between sort and search compare
 *      functions, global custom compare functions shall use the channel ID for
 *      comparision.
 *  @par
 *      What if we want customized sort orders by fields other than channel ID for
 *      local SVL handles? @ref c_svl_get_rec_by_brdcst_type "c_svl_get_rec_by_*()" API's
 *      are not affected, but c_svl_iterate_rec() will be. To solve this, two API's
 *      are introduced: c_svl_iterate_rec_by_custom_ord() and c_svl_sort_rec().
 *  @par
 *      Rather than requiring a channel ID for locating the pivot,
 *      c_svl_iterate_rec_by_custom_ord() expects a VOID * pv_data, which will be
 *      passed the custom search compare function. Differing from c_svl_iterate_rec(),
 *      c_svl_iterate_rec_by_custom_ord() does not automatically sort SVL, and
 *      c_svl_sort_rec() shall be invoked before calling
 *      c_svl_iterate_rec_by_custom_ord() for manual sorting. In this way finding the
 *      next SVL record on specified direction in custom order could be realized.
 *  @par
 *  @subsection ssecMW_SVL_4_8  4.8. Load / save / delete the SVL object (list) to / from file system
 *  @par
 *      This topic is simple enough to omit specific examples. See the function
 *      description of c_svl_fs_sync(), c_svl_fs_store() & c_svl_fs_del(), as well as
 *      @ref ssecMW_SVL_3_6 .
 *  @par
 *  @subsection ssecMW_SVL_4_9  4.9. Backup / restore the SVL object (list) to / from memory
 *  @par
 *      In some projects, application needs to provide options for user to rollback
 *      the channel scan result after scan is completed or canceled. In this case we
 *      can backup the SVL and related lists to memory using c_svl_dump_to_mem() and
 *      alike, and perform rollback action using c_svl_load_from_mem() etc. The
 *      memory dumps shall be initialized using c_svl_init_mem_dump() before dumping,
 *      and freed using c_svl_free_mem_dump() after the dump data is no longer used.
 *  @par
 *      Here is a sample usage.
 *  @par
 *      @skip Memory
 *      @until free_mem
 *      @until CHK_FAIL
 *  @par
 *      The advantage of abovementioned in-memory backup / restore operations over
 *      traditional method of using another SVL list object for backup usage is:
 *      -#  The operation is simpler and faster.
 *      -#  Possible defects in the merging process could be avoided.
 *      -#  @ref groupMW_SVL_BLDR "SVL Builder" people are happy not to maintain a separate
 *      backup / restore procedure.
 *  
 *  @par
 *  @subsection ssecMW_SVL_4_10  4.10. Get / set the client data of SVL record
 *  @par
 *      Some customer projects requires to store extra channel-specific information to
 *      flash, which is not part of @ref SVL_REC_T "SVL record structure" . In this case
 *      c_svl_set_client_data() and c_svl_get_client_data() could be used.
 *  @par
 *      A simple usage example is as follows.
 *  @par
 *      @skip Client
 *      @until are set
 *  @par
 *      If application is to use SVL client data, cautions shall be taken that whether
 *      client data is supported by @ref groupMW_SVL_BLDR "SVL Builder" engines in use
 *      needs to be confirmed beforehand, as when performing update scan or monitoring
 *      service, SVL records might be updated by @ref groupMW_SVL_BLDR "SVL Builder" , but
 *      the associated client data may or may not be retained, depending on the
 *      implementation of @ref groupMW_SVL_BLDR "SVL Builder" engines.
 *  @else
 *  @section secMW_SVL_4  4. Typical Case
 *  @par
 *      @dontinclude svlsample_x.c
 *  @subsection ssecMW_SVL_4_1  4.1. Create / delete / open / clean / close of SVL object (list)
 *  @par
 *      To create / delete / open / clean / close an SVL object from AP, simply call
 *      respective API's with intuitive naming. Below is an example.
 *  @par
 *      @skip Create
 *      @until svl_delete
 *  @par
 *      Creating SVL is usually called at acfg init time, or when a temporary SVL is
 *      needed, which will be deleted later after use. Opening a handle for SVL is a
 *      common operation when you need to operate on it, and the handle shall be
 *      closed when it is no longer used. Cleaning the SVL is usually invoked when
 *      resetting or cleaning storage from Menu operations.
 *  @par
 *  @subsection ssecMW_SVL_4_2  4.2. Add / delete / update a record in SVL object (list)
 *  @par
 *      To add a record in SVL, we first need to prepare the record structure, and
 *      ensure that it has a unique record ID. Below is a sample snippet.
 *  @par
 *      @skip Add
 *      @until CHK_FAIL
 *  @par
 *      After assigning data for other useful fields, we add the record to SVL.
 *  @par
 *      @until CHK_FAIL
 *  @par
 *      To update an existing SVL record, identified by its record ID, we call
 *      x_svl_update_rec() with 3rd argument (b_must_exist) TRUE.
 *  @par
 *      @skip Update
 *      @until CHK_FAIL
 *  @par
 *      If b_must_exist is FALSE, if the record ID of the SVL record is in use, then
 *      the record is updated; otherwise a new record is added to SVL.
 *  @par
 *      To delete a record from SVL, supplying the record ID is enough.
 *  @par
 *      @skip Delete
 *      @until CHK_FAIL
 *  @par
 *      The add / delete / update operations costs O(<i>n</i>) time, where n is the number of
 *      records in SVL.
 *  @par
 *  @subsection ssecMW_SVL_4_3  4.3. Get an SVL record by specified record ID or channel ID
 *  @par
 *      A record in SVL could be identified by its record ID, or channel ID. We can
 *      query like this.
 *  @par
 *      @skip Get
 *      @until CHK_FAIL
 *  @par
 *      Or this.
 *  @par
 *      @skip Get
 *      @until CHK_FAIL
 *  @par
 *      When querying SVL records by channel ID, a network mask could be specified,
 *      which will be used for filtering and only records with at least one matching
 *      bit in network mask will be returned. If there are multiple records matching
 *      the specified channel ID and network mask, the fourth argument, the index will
 *      be used to identify which will record is to be returned.
 *  @par
 *      The arguments of network mask and index are similiar for other get-by
 *      functions described in the following section, though in the case of channel
 *      ID, index is usually given 0 as no duplicate channel ID shall exist in an SVL
 *      object in normal case, while in other query conditions index are not usually
 *      0, and the number of available indices can be obtained by calling
 *      @ref x_svl_get_num_rec_by_channel "x_svl_get_num_rec_by_*()" .
 *  @par
 *      The pointer to ui4_ver_id given will be used to check SVL version changes, and
 *      if ui4_ver_id is not @ref SVL_NULL_VER_ID and SVL database version differs from
 *      what is given, @ref SVLR_MODIFIED will be returned. The version check is applied
 *      for all get operations, including ones described in the following sections.
 *  @par
 *      Getting record by record ID or channel ID costs O(<i>n</i>) time.
 *  @par
 *  @subsection ssecMW_SVL_4_4  4.4. Get an SVL record by brdcst_type, ts, nw, sat
 *  @par
 *      These operations are similiar to getting an SVL record by channel ID, and the
 *      number of records with matching query field (brdcst_type, ts, nw, sat) and
 *      network mask could be obtained by corresponiding get_num_rec function. One
 *      special case is there that when querying by broadcast type and the type given
 *      is @ref BRDCST_TYPE_UNKNOWN , broadcast type is disregarded and only network
 *      mask is taken into account for the query.
 *  @par
 *      Please refer to the function document of x_svl_get_rec_by_brdcst_type(),
 *      x_svl_get_num_rec_by_brdcst_type(), x_svl_get_rec_by_ts(),
 *      x_svl_get_num_rec_by_ts(), x_svl_get_rec_by_nw(), x_svl_get_num_rec_by_nw(),
 *      x_svl_get_rec_by_sat() and x_svl_get_num_rec_by_sat().
 *  @par
 *      All these get record operations cost O(<i>n</i> log <i>n</i>) time on the first call
 *      after SVL is modified, O(log <i>n</i>) if SVL is not modified, and O(1) for
 *      subsequent invocations where index is greater than 0, while all get number
 *      operations costs O(<i>n</i>) time.
 *  @par
 *  @subsection ssecMW_SVL_4_5  4.5. Get all SVL records by for-loop
 *  @par
 *      To get all SVL records by for-loop, we first need to get the total number of
 *      matching records, and then get records one by one through increasing index. We
 *      need to pay attention to the version ID, which is used to check whether SVL
 *      version change has occurred since the last get operation, and in case this
 *      happens, @ref SVLR_MODIFIED will be returned and we need to restart the
 *      iteration from beginning.
 *  @par
 *      @skip Get all
 *      @until while
 *  @par
 *      An alternative way which does not require checking @ref SVLR_MODIFIED return
 *      value and restarting iteration is, to surround the get-number operation and
 *      the for loop with x_svl_read_lock() and x_svl_read_unlock(), which can ensure
 *      that no write operation could occur during the iteration. However, we should
 *      not make the iteration time too long in this case, lest other threads trying
 *      to write to the SVL be blocked.
 *  @par
 *  @subsection ssecMW_SVL_4_6  4.6. Iterate the next SVL record by specified direction
 *  @par
 *      Sometimes we want to know the next SVL record on a specified direction of
 *      channel ID, e.g.  when user pressing CH+/-, in which case the next / previous
 *      channel will be tuned. x_svl_iterate_rec() and alike functions are suited for
 *      this purpose.
 *  @par
 *      Possible iteration directions defined in @ref SVL_ITERATE_T are:
 *      @ref SVL_CHANNEL_PLUS , @ref SVL_CHANNEL_MINUS , @ref SVL_CHANNEL_SMALLEST and
 *      @ref SVL_CHANNEL_LARGEST , or their alias, @ref SVL_NEXT_REC , @ref SVL_PREV_REC ,
 *      @ref SVL_FIRST_REC , @ref SVL_LAST_REC . Their meanings are intuitive and also
 *      stated in their descriptions.
 *  @par
 *      x_svl_iterate_rec() uses the given channel ID as the pivot. It first tries to
 *      find the SVL record with the matching channel ID, and in case there are
 *      multiple records with the same channel ID, the first record on the specified
 *      direction with non-matching channel ID will be focused. If no record with
 *      matching channel ID is found, the record with closest channel ID on the
 *      specified direction will be focused, or if the closest record could not be
 *      found, which means that the given channel ID is larger than the maximum
 *      channel ID when direction is @ref SVL_CHANNEL_PLUS "PLUS" , or smaller than the
 *      minimum channel ID when direction is @ref SVL_CHANNEL_MINUS "MINUS" ,
 *      @ref SVLR_END_OF_ITERATION will be returned.
 *  @par
 *      After the next SVL record on the specified direction has been focused, we keep
 *      reading records and focusing on the next one until a record with matching
 *      network mask has been found, which is then returned to caller with return
 *      value @ref SVLR_OK . If no record with matching network mask is found after the
 *      record with largest / smallest channel ID has been reached,
 *      @ref SVLR_END_OF_ITERATION will be returned.
 *  @par
 *      The caller shall check the return value and perform appropriate operation when
 *      @ref SVLR_END_OF_ITERATION is returned. Here is a sample usage.
 *  @par
 *      @skip Iterate
 *      @until CHK_FAIL
 *  @par
 *      The iterate operations cost O(<i>n</i> log <i>n</i>) time on the first call after SVL
 *      is modified and O(log <i>n</i>) otherwise.
 *  @par
 *      We can also use x_svl_iterate_rec() to modify all records in an SVL object.
 *      Here is an example.
 *  @par
 *      @skip Modify
 *      @until unlock
 *  @par
 *      The above example utilizes the fact that, in the current implementation, the
 *      version ID passed in for x_svl_iterate_rec() is not used for checking whether
 *      the database version has modified since the last call to x_svl_iterate_rec(),
 *      but rather the value of the version ID is interpreted as follows for version
 *      checking:
 *      -#  x_svl_iterate_rec() internally use another variable for version ID, which is
 *      initialized to @ref SVL_NULL_VER_ID . Therefore no matter whether the SVL
 *      database version has changed since the last call, x_svl_iterate_rec() will
 *      proceed in the same way.
 *      -#  If the version ID passed in is @ref SVL_NULL_VER_ID , and SVL database has
 *      been modified during the current call of x_svl_iterate_rec(), where multiple
 *      records are read one after another to find the one with matching network
 *      mask, the current search loop is terminated, and another loop is started
 *      from the beginning, from sorting and locating onward.
 *      -#  Otherwise if the version ID passed in is non-zero, and SVL database has been
 *      modified during the current call, @ref SVLR_END_OF_ITERATION will be returned.
 *      This case is very rare, though possible. However the previous example does
 *      not need to worry about that, as it has acquired a write lock and all
 *      modifications are under control of itself, without impact on the iteration.
 *  
 *  @par
 *      The abovementioned unusual behavior of version ID checking might be a
 *      historical bug, but it has become a feature now and several AP functions
 *      relies on it. However, since the SVL is modified in the loop body, every call
 *      to x_svl_iterate_rec will lead to a re-sorting, and thus the whole update-all
 *      operation would cost O(<i>n</i><sup>2</sup> log <i>n</i>) time, which is worse than
 *      what we could achieve if we first get a list of SVL record ID's stored in a
 *      temporary place, and simply use a for-loop calling x_svl_get_rec() and
 *      x_svl_update_rec(), which costs O(<i>n</i> log <i>n</i> + <i>n</i><sup>2</sup>), or
 *      O(<i>n</i><sup>2</sup>) time.
 *  @par
 *  @subsection ssecMW_SVL_4_7  4.7. Customizing SVL compare function and manual sorting
 *  @par
 *      SVL uses channel ID as the key field used for sorting, which is performed if
 *      necessary when calling x_svl_iterate_rec() and
 *      @ref x_svl_get_rec_by_brdcst_type "x_svl_get_rec_by_*()" operations except
 *      x_svl_get_rec_by_channel(), as well as searching, which is performed when
 *      calling x_svl_iterate_rec(). Both sorting and searching requires comparison.
 *      While by default SVL uses the numeral value of
 *      @ref SVL_REC_HEADER_T "ui4_channel_id" for interger comparison, applications can
 *      set customized compare functions for both sorting and search, either globally
 *      or for individual SVL handle only, by invoking x_svl_reg_cmp_fcts(), which
 *      takes from application two callback function pointers of type
 *      @ref x_svl_sort_cmp and @ref x_svl_search_cmp respectively.
 *  @par
 *      Below is an example.
 *  @par
 *      @skip Customize
 *      @until CHK_FAIL
 *  @par
 *      Custom sort compare function will be called with the first two arguments being
 *      pointers to the whole SVL record structure, which is not limited to merely the
 *      ui4_channel_id field. However, the second argument of the custom search
 *      compare function is not a complete SVL record structure but only a VOID *
 *      field, whose value is assigned to the given channel ID when calling
 *      x_svl_iterate_rec(). To be consistent between sort and search compare
 *      functions, global custom compare functions shall use the channel ID for
 *      comparision.
 *  @par
 *      What if we want customized sort orders by fields other than channel ID for
 *      local SVL handles? @ref x_svl_get_rec_by_brdcst_type "x_svl_get_rec_by_*()" API's
 *      are not affected, but x_svl_iterate_rec() will be. To solve this, two API's
 *      are introduced: x_svl_iterate_rec_by_custom_ord() and x_svl_sort_rec().
 *  @par
 *      Rather than requiring a channel ID for locating the pivot,
 *      x_svl_iterate_rec_by_custom_ord() expects a VOID * pv_data, which will be
 *      passed the custom search compare function. Differing from x_svl_iterate_rec(),
 *      x_svl_iterate_rec_by_custom_ord() does not automatically sort SVL, and
 *      x_svl_sort_rec() shall be invoked before calling
 *      x_svl_iterate_rec_by_custom_ord() for manual sorting. In this way finding the
 *      next SVL record on specified direction in custom order could be realized.
 *  @par
 *  @subsection ssecMW_SVL_4_8  4.8. Load / save / delete the SVL object (list) to / from file system
 *  @par
 *      This topic is simple enough to omit specific examples. See the function
 *      description of x_svl_fs_sync(), x_svl_fs_store() & x_svl_fs_del(), as well as
 *      @ref ssecMW_SVL_3_6 .
 *  @par
 *  @subsection ssecMW_SVL_4_9  4.9. Backup / restore the SVL object (list) to / from memory
 *  @par
 *      In some projects, application needs to provide options for user to rollback
 *      the channel scan result after scan is completed or canceled. In this case we
 *      can backup the SVL and related lists to memory using x_svl_dump_to_mem() and
 *      alike, and perform rollback action using x_svl_load_from_mem() etc. The
 *      memory dumps shall be initialized using x_svl_init_mem_dump() before dumping,
 *      and freed using x_svl_free_mem_dump() after the dump data is no longer used.
 *  @par
 *      Here is a sample usage.
 *  @par
 *      @skip Memory
 *      @until free_mem
 *      @until CHK_FAIL
 *  @par
 *      The advantage of abovementioned in-memory backup / restore operations over
 *      traditional method of using another SVL list object for backup usage is:
 *      -#  The operation is simpler and faster.
 *      -#  Possible defects in the merging process could be avoided.
 *      -#  @ref groupMW_SVL_BLDR "SVL Builder" people are happy not to maintain a separate
 *      backup / restore procedure.
 *  
 *  @par
 *  @subsection ssecMW_SVL_4_10  4.10. Get / set the client data of SVL record
 *  @par
 *      Some customer projects requires to store extra channel-specific information to
 *      flash, which is not part of @ref SVL_REC_T "SVL record structure" . In this case
 *      x_svl_set_client_data() and x_svl_get_client_data() could be used.
 *  @par
 *      A simple usage example is as follows.
 *  @par
 *      @skip Client
 *      @until are set
 *  @par
 *      If application is to use SVL client data, cautions shall be taken that whether
 *      client data is supported by @ref groupMW_SVL_BLDR "SVL Builder" engines in use
 *      needs to be confirmed beforehand, as when performing update scan or monitoring
 *      service, SVL records might be updated by @ref groupMW_SVL_BLDR "SVL Builder" , but
 *      the associated client data may or may not be retained, depending on the
 *      implementation of @ref groupMW_SVL_BLDR "SVL Builder" engines.
 *  @endif
 */
#endif /* _U_SVL_H_ */
