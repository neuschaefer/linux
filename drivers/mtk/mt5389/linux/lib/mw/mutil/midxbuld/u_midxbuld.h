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
/*! @file u_midxbuld.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This file exports all media index builder specific definitions and structures.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_MUTIL_MIDXBULD
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_MIDXBULD_H_
#define _U_MIDXBULD_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#ifdef _WIN32_
#include "inc/u_os.h"
#else
#include "os/inc/u_os.h"
#endif
#include "inc/u_common.h"
#include "handle/u_handle.h"
#include "mutil/mfmtrecg/u_mfmtrecg.h"

#else

#include "u_os.h"
#include "u_common.h"
#include "u_handle.h"
#include "u_mfmtrecg.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MIDXBULD_IDX_RANGE_FULL                  0xFFFFFFFF    /**<        */
#define MIDXBULD_IDX_ENTRY_SIZE_UNKNOWN          0xFFFFFFFF    /**<        */
#define MIDXBULD_IDX_ENTRY_OFFSET_UNKNOWN        0xFFFFFFFF    /**<        */
#define MIDXBULD_ELMT_TBL_MAX                    17         /* 1VID+8AUD+8ST */    /**<        */

#define MIDXBULD_NULL_ID                         0    /**<        */

/*------------------------------------------------------------------*/
/*! @enum  MIDXBULD_NFY_EVENT_TYPE_T
 *  @brief Midxbuild notify type. 
 *  @code
 *  typedef enum
 *  {
 *      MIDXBULD_NFY_EVENT_INDEXING_DONE,
 *      MIDXBULD_NFY_EVENT_INDEXING_ABORT,
 *      MIDXBULD_NFY_EVENT_HANDLE_CLOSED,
 *      MIDXBULD_NFY_EVENT_ERROR
 *  }   MIDXBULD_NFY_EVENT_TYPE_T;
 *  @endcode
 *  @li@c  MIDXBULD_NFY_EVENT_INDEXING_DONE                  - Indexing done.
 *  @li@c  MIDXBULD_NFY_EVENT_INDEXING_ABORT                 - Indexing abort.
 *  @li@c  MIDXBULD_NFY_EVENT_HANDLE_CLOSED                  - Handle closed.
 *  @li@c  MIDXBULD_NFY_EVENT_ERROR                          - Error.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MIDXBULD_NFY_EVENT_INDEXING_DONE,
    MIDXBULD_NFY_EVENT_INDEXING_ABORT,
    MIDXBULD_NFY_EVENT_HANDLE_CLOSED,
    MIDXBULD_NFY_EVENT_ERROR
}   MIDXBULD_NFY_EVENT_TYPE_T;

#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
/*------------------------------------------------------------------*/
/*! @enum MIXBULD_START_INFO_T
 *  @brief Start Info.
 *  @code
 *  typedef struct _MIXBULD_START_INFO_T
 *  {
 *      UINT32          ui4_offset;
 *      UINT32          ui4_1st_pkgnum;
 *  }MIXBULD_START_INFO_T;
 *  @endcode
 *  @li@c  ui4_offset                   - offset .
 *  @li@c  ui4_1st_pkgnum           - first packet number.
 */
/*------------------------------------------------------------------*/
typedef struct _MIXBULD_START_INFO_T
{
    UINT32          ui4_offset;
    UINT32          ui4_1st_pkgnum;
}MIXBULD_START_INFO_T;
#endif

/* This structure enumerates all GET info type of media index builder. */
/*------------------------------------------------------------------*/
/*! @enum MIDXBULD_GET_INFO_TYPE_T
 *  @brief Midxbuld get information type.
 *  @code
 *  typedef enum
 *  {
 *      MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL = 0, 
 *      MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST,  
 *      MIDXBULD_GET_INFO_TYPE_BULD_PROGRESS      
 *  } MIDXBULD_GET_INFO_TYPE_T;
 *  @endcode
 *  @li@c  MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL               - Index element tble.
 *  @li@c  MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST           - Key Index table list.
 *  @li@c  MIDXBULD_GET_INFO_TYPE_BULD_PROGRESS              - Current build progress.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL = 0, /* get index element table */
    MIDXBULD_GET_INFO_TYPE_KEY_IDX_TBL_LIST,  /* get key index table list */
    MIDXBULD_GET_INFO_TYPE_BULD_PROGRESS      /* get index build progress */
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    ,MIDXBULD_GET_INFO_TYPE_AUDIO_START_INFO,   /* get audio start info for bad interleaved*/
    MIDXBULD_GET_INFO_TYPE_VIDEO_START_INFO    /* get video start info for bad interleaved*/
#endif
} MIDXBULD_GET_INFO_TYPE_T;

/* This structure enumerates all SET info type of media index builder. */
/*------------------------------------------------------------------*/
/*! @enum MIDXBULD_SET_INFO_TYPE_T
 *  @brief Midxbuld set info type.
 *  @code
 *  typedef enum
 *  {
 *      MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE = 0,
 *      MIDXBULD_SET_INFO_TYPE_FREE_RANGE
 *  } MIDXBULD_SET_INFO_TYPE_T;
 *  @endcode
 *  @li@c  MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE                - Type allocate range.
 *  @li@c  MIDXBULD_SET_INFO_TYPE_FREE_RANGE                 - Type free range.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE = 0,
    MIDXBULD_SET_INFO_TYPE_FREE_RANGE
} MIDXBULD_SET_INFO_TYPE_T;

/* MIDXBULD_SET_INFO_TYPE_ALLOC_RANGE data info ***************************/
/*------------------------------------------------------------------*/
/*! @enum MIDXBULD_ELMT_STRM_TYPE_T
 *  @brief Element stream type.
 *  @code
 *  typedef enum
 *  {
 *      MIDXBULD_ELEM_STRM_TYPE_UNKNOWN = 0,
 *      MIDXBULD_ELEM_STRM_TYPE_AUDIO,
 *      MIDXBULD_ELEM_STRM_TYPE_VIDEO,
 *      MIDXBULD_ELEM_STRM_TYPE_SUBTITLE,
 *      MIDXBULD_ELEM_STRM_TYPE_KEY 
 *  } MIDXBULD_ELMT_STRM_TYPE_T;
 *  @endcode
 *  @li@c  MIDXBULD_ELEM_STRM_TYPE_UNKNOWN                   - Unknown.
 *  @li@c  MIDXBULD_ELEM_STRM_TYPE_AUDIO                     - Audio.
 *  @li@c  MIDXBULD_ELEM_STRM_TYPE_VIDEO                     - Video.
 *  @li@c  MIDXBULD_ELEM_STRM_TYPE_SUBTITLE                  - Subtitle.
 *  @li@c  MIDXBULD_ELEM_STRM_TYPE_KEY                       - Key.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MIDXBULD_ELEM_STRM_TYPE_UNKNOWN = 0,
    MIDXBULD_ELEM_STRM_TYPE_AUDIO,
    MIDXBULD_ELEM_STRM_TYPE_VIDEO,
    MIDXBULD_ELEM_STRM_TYPE_SUBTITLE,
    MIDXBULD_ELEM_STRM_TYPE_KEY /* key frame table will be auto built after open handler */
} MIDXBULD_ELMT_STRM_TYPE_T;

#define MIDXBULD_SAMPLE_SIZE_UNKNOWN             0xFFFFFFFF    /**<        */
#define MIDXBULD_AVG_BYTES_PER_SEC_UNKNOWN       0xFFFFFFFF    /**<        */

/*------------------------------------------------------------------*/
/*! @struct MIDXBULD_ELMT_STRM_TBL_INFO_T
 *  @brief Element stream table information.
 *  @code
 *  typedef struct _MIDXBULD_ELMT_STRM_TBL_INFO_T
 *  {
 *      MIDXBULD_ELMT_STRM_TYPE_T e_es_type; 
 *      UINT32 ui4_strm_id;                  
 *      UINT32 ui4_scale;                    
 *      UINT32 ui4_rate;                     
 *      UINT32 ui4_sample_sz;                
 *      UINT32 ui4_avg_bytes_per_sec;        
 *      UINT32 ui4_indx_offset;              
 *      UINT32 ui4_indx_size;                
 *  } MIDXBULD_ELMT_STRM_TBL_INFO_T;
 *  @endcode
 *  @li@c  e_es_type                                         - Element stream type.
 *  @li@c  ui4_strm_id                                       - Stream id.
 *  @li@c  ui4_scale                                         - Timescale.
 *  @li@c  ui4_rate                                          - Rate.
 *  @li@c  ui4_sample_sz                                     - Sample size.
 *  @li@c  ui4_avg_bytes_per_sec                             - Average byte per second.
 *  @li@c  ui4_indx_offset                                   - Index table offset.
 *  @li@c  ui4_indx_size                                     - Index table size.
 */
/*------------------------------------------------------------------*/
typedef struct _MIDXBULD_ELMT_STRM_TBL_INFO_T
{
    MIDXBULD_ELMT_STRM_TYPE_T e_es_type; /* (I) es type */
    UINT32 ui4_strm_id;                  /* (I) stream id */
    UINT32 ui4_scale;                    /* (I) denominator */
    UINT32 ui4_rate;                     /* (I) numerator */
    UINT32 ui4_sample_sz;                /* (I) sample size */
    UINT32 ui4_avg_bytes_per_sec;        /* (I) avg. bytes per second */
    UINT32 ui4_indx_offset;              /* (I) avi2.0 'indx' offset */
    UINT32 ui4_indx_size;                /* (I) avi2.0 'indx' size */
    UINT64 ui8_timeunit;
    UINT64 ui8_granule_pos;
    UINT32 ui4_avg_frame_num;
    UINT32 ui4_avg_packet_size;
    BOOL   b_is_cur_strm_codec_spt;      /* (I) Now only used for mp4 */
    BOOL   b_is_pcm_aud;                 /* (I) Now only used for mp4 */
} MIDXBULD_ELMT_STRM_TBL_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct MIDXBULD_ELMT_STRM_RANGE_T
 *  @brief Range information.
 *  @code
 *  typedef struct _MIDXBULD_ELMT_STRM_RANGE_T
 *  {
 *      UINT32 ui4_start_pts; 
 *      UINT32 ui4_end_pts;   
 *  } MIDXBULD_ELMT_STRM_RANGE_T;
 *  @endcode
 *  @li@c  ui4_start_pts                                     - Start pts.
 *  @li@c  ui4_end_pts                                       - End pts.
 */
/*------------------------------------------------------------------*/
typedef struct _MIDXBULD_ELMT_STRM_RANGE_T
{
    UINT32 ui4_start_pts; /* (I) start pts */
    UINT32 ui4_end_pts;   /* (I) end pts */
    UINT64 ui8_start_pcr; /*start pcr value in file*/
} MIDXBULD_ELMT_STRM_RANGE_T;

/* MIDXBULD stream table type */
/*------------------------------------------------------------------*/
/*! @enum MIDXBULD_ELMT_TBL_TYPE_T
 *  @brief Midxbuld element table type.
 *  @code
 *  typedef enum
 *  {
 *      MIDXBULD_TBL_TYPE_ELMT_STRM = 0,
 *      MIDXBULD_TBL_TYPE_AUDIO_KEY
 *  } MIDXBULD_ELMT_TBL_TYPE_T;
 *  @endcode
 *  @li@c  MIDXBULD_TBL_TYPE_ELMT_STRM                       - Element stream.
 *  @li@c  MIDXBULD_TBL_TYPE_AUDIO_KEY                       - Audio key.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MIDXBULD_TBL_TYPE_ELMT_STRM = 0,
    MIDXBULD_TBL_TYPE_AUDIO_KEY
} MIDXBULD_ELMT_TBL_TYPE_T;

#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
typedef enum
{
    MIDXBULD_AUD_PTS_ENTRY_TYPE = 0,
    MIDXBULD_AUD_ALL_ENTRY_TYPE
} MIDXBULD_AUD_ENTRY_TYPE_T;
#endif

/*------------------------------------------------------------------*/
/*! @struct MIDXBULD_ELMT_TBL_DESCRIB_T
 *  @brief Element table description
 *  @code
 *  typedef struct _MIDXBULD_ELMT_TBL_DESCRIB_T
 *  {
 *      union
 *      {
 *          MIDXBULD_ELMT_STRM_TBL_INFO_T t_es;
 *      } u_info;
 *      struct _MIDXBULD_ELMT_TBL_DESCRIB_T* pt_next;
 *      UINT32 ui4_tbl_id;
 *  } MIDXBULD_ELMT_TBL_DESCRIB_T;
 *  @endcode
 *  @li@c  t_es                                              - Stream table information.
 *  @li@c  pt_next                                           - Next element table.
 *  @li@c  ui4_tbl_id                                        - Table id.
 */
/*------------------------------------------------------------------*/
typedef struct _MIDXBULD_ELMT_TBL_DESCRIB_T
{
    union
    {
        MIDXBULD_ELMT_STRM_TBL_INFO_T t_es;
    } u_info;
    struct _MIDXBULD_ELMT_TBL_DESCRIB_T* pt_next;
    UINT32 ui4_tbl_id;
} MIDXBULD_ELMT_TBL_DESCRIB_T;

/*------------------------------------------------------------------*/
/*! @struct MIDXBULD_ELMT_TBL_ALLOC_INFO_T
 *  @brief Allocate range information.
 *  @code
 *  typedef struct _MIDXBULD_ELMT_TBL_ALLOC_INFO_T
 *  {
 *      MIDXBULD_ELMT_TBL_TYPE_T e_tbl_type;         
 *      union 
 *      {
 *          MIDXBULD_ELMT_STRM_RANGE_T t_es;
 *      } u_range;                                   
 *      UINT32 ui4_range_id;                         
 *      MIDXBULD_ELMT_TBL_DESCRIB_T* pt_tbl_describ; 
 *  } MIDXBULD_ELMT_TBL_ALLOC_INFO_T;
 *  @endcode
 *  @li@c  e_tbl_type                                        - Table type.
 *  @li@c  t_es                                              - Element stream information.
 *  @li@c  ui4_range_id                                      - Range id.
 *  @li@c  pt_tbl_describ                                    - Description infor.
 */
/*------------------------------------------------------------------*/
typedef struct _MIDXBULD_ELMT_TBL_ALLOC_INFO_T
{
    MIDXBULD_ELMT_TBL_TYPE_T e_tbl_type;         /* (I) */
    union 
    {
        MIDXBULD_ELMT_STRM_RANGE_T t_es;
    } u_range;                                   /* (I) */
    UINT32 ui4_range_id;                         /* (O) */
    MIDXBULD_ELMT_TBL_DESCRIB_T* pt_tbl_describ; /* (I/O) */
} MIDXBULD_ELMT_TBL_ALLOC_INFO_T;

/* MIDXBULD_SET_INFO_TYPE_FREE_RANGE data info ***************************/
/* the free handle should be a range handle */
#if 0
/*------------------------------------------------------------------*/
/*! @enum
 *  @brief
 *  @code
 *  typedef enum
 *  {
 *      MIDXBULD_ELMT_TBL_FREE_TYPE_RANGE = 0,
 *      MIDXBULD_ELMT_TBL_FREE_TYPE_ELMT_TBL
 *  } MIDXBULD_ELMT_TBL_FREE_TYPE_T;
 *  @endcode
 *  @li@c  MIDXBULD_ELMT_TBL_FREE_TYPE_RANGE                 -
 *  @li@c  MIDXBULD_ELMT_TBL_FREE_TYPE_ELMT_TBL              -
 */
/*------------------------------------------------------------------*/
typedef enum
{
    MIDXBULD_ELMT_TBL_FREE_TYPE_RANGE = 0,
    MIDXBULD_ELMT_TBL_FREE_TYPE_ELMT_TBL
} MIDXBULD_ELMT_TBL_FREE_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct
 *  @brief
 *  @code
 *  typedef struct _MIDXBULD_ELMT_TBL_FREE_INFO_T
 *  {
 *      MIDXBULD_ELMT_TBL_FREE_TYPE_T e_type; 
 *      HANDLE_T                      h_free; 
 *  } MIDXBULD_ELMT_TBL_FREE_INFO_T;
 *  @endcode
 *  @li@c  e_type                                            -
 *  @li@c  h_free                                            -
 */
/*------------------------------------------------------------------*/
typedef struct _MIDXBULD_ELMT_TBL_FREE_INFO_T
{
    MIDXBULD_ELMT_TBL_FREE_TYPE_T e_type; /* (I) */
    HANDLE_T                      h_free; /* (I) */
} MIDXBULD_ELMT_TBL_FREE_INFO_T;
#endif

/* MIDXBULD_GET_INFO_TYPE_IDX_ELMT_TBL data info ***************************/
/*------------------------------------------------------------------*/
/*! @struct MIDXBULD_ELMT_STRM_ENTRY_T
 *  @brief Element stream index table entry information.
 *  @code
 *  typedef struct _MIDXBULD_ELMT_STRM_ENTRY_T
 *  {
 *      UINT32 ui4_pts;             
 *      UINT32 ui4_relative_offset; 
 *      UINT32 ui4_size;            
 *  } MIDXBULD_ELMT_STRM_ENTRY_T;
 *  @endcode
 *  @li@c  ui4_pts                                           - Sample pts.
 *  @li@c  ui4_relative_offset                               - Sample relative offset.
 *  @li@c  ui4_size                                          - Sample size.
 */
/*------------------------------------------------------------------*/
typedef struct _MIDXBULD_ELMT_STRM_ENTRY_T
{
    UINT32 ui4_pts;             /* might be invalid */
    UINT32 ui4_relative_offset; /* physcical file offset = ui8_base_offset + ui4_relative_offset */
    UINT32 ui4_size;            /* pure data size, not include header */
} MIDXBULD_ELMT_STRM_ENTRY_T;

/*------------------------------------------------------------------*/
/*! @struct MIDXBULD_ELMT_STRM_TBL_FMT_T
 *  @brief Element stream table format.
 *  @code
 *  typedef struct _MIDXBULD_ELMT_STRM_TBL_FMT_T
 *  {
 *      MIDXBULD_ELMT_STRM_TBL_INFO_T t_info;
 *      MIDXBULD_ELMT_STRM_ENTRY_T*   pt_idx_entry;
 *  } MIDXBULD_ELMT_STRM_TBL_FMT_T;
 *  @endcode
 *  @li@c  t_info                                            - Stream table information.
 *  @li@c  pt_idx_entry                                      - Point to the entry.
 */
/*------------------------------------------------------------------*/
typedef struct _MIDXBULD_ELMT_STRM_TBL_FMT_T
{
    MIDXBULD_ELMT_STRM_TBL_INFO_T t_info;
    /*UINT32                        ui4_start_frame_num;*/
    /*UINT32                        ui4_end_frame_num;*/
    MIDXBULD_ELMT_STRM_ENTRY_T*   pt_idx_entry;
} MIDXBULD_ELMT_STRM_TBL_FMT_T;

/*------------------------------------------------------------------*/
/*! @struct MIDXBULD_AUD_STRM_TBL_KEY_T
 *  @brief Audio stream table key.
 *  @code
 *  typedef struct _MIDXBULD_AUD_STRM_TBL_KEY_T
 *  {
 *      MIDXBULD_ELMT_STRM_TBL_INFO_T   t_info;
 *      UINT32*                         pui4_pts;
 *  } MIDXBULD_AUD_STRM_TBL_KEY_T;
 *  @endcode
 *  @li@c  t_info                                            - Stream table information.
 *  @li@c  pui4_pts                                          - Pts.
 */
/*------------------------------------------------------------------*/
typedef struct _MIDXBULD_AUD_STRM_TBL_KEY_T
{
    MIDXBULD_ELMT_STRM_TBL_INFO_T   t_info;
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    MIDXBULD_AUD_ENTRY_TYPE_T       e_entry_type;
#endif
    UINT32*                         pui4_pts;
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    MIDXBULD_ELMT_STRM_ENTRY_T*     pt_idx_entry;
#endif
} MIDXBULD_AUD_STRM_TBL_KEY_T;

/*------------------------------------------------------------------*/
/*! @struct MIDXBULD_ELMT_TBL_OBJ_T
 *  @brief Element table object.
 *  @code
 *  typedef struct _MIDXBULD_ELMT_TBL_OBJ_T
 *  {
 *      UINT64 ui8_base_offset;    
 *      UINT32 ui4_number_of_entry;
 *      MIDXBULD_ELMT_TBL_TYPE_T         e_tbl_type;
 *      union
 *      {
 *          MIDXBULD_ELMT_STRM_TBL_FMT_T t_es;
 *          MIDXBULD_AUD_STRM_TBL_KEY_T  t_as_key;
 *      } u_fmt;
 *      struct _MIDXBULD_ELMT_TBL_OBJ_T* pt_next;
 *  } MIDXBULD_ELMT_TBL_OBJ_T;
 *  @endcode
 *  @li@c  ui8_base_offset                                   - Base offset in case of file size large than 4G.
 *  @li@c  ui4_number_of_entry                               - Number of entry.
 *  @li@c  e_tbl_type                                        - Table type.
 *  @li@c  t_es                                              - Element stream infor.
 *  @li@c  t_as_key                                          - Audio stream key.
 *  @li@c  pt_next                                           - Next table.
 */
/*------------------------------------------------------------------*/
typedef struct _MIDXBULD_ELMT_TBL_OBJ_T
{
    UINT64 ui8_base_offset;    /* in case of file size > 4G */
    UINT32 ui4_number_of_entry;
    MIDXBULD_ELMT_TBL_TYPE_T         e_tbl_type;
    union
    {
        MIDXBULD_ELMT_STRM_TBL_FMT_T t_es;
        MIDXBULD_AUD_STRM_TBL_KEY_T  t_as_key;
    } u_fmt;
    struct _MIDXBULD_ELMT_TBL_OBJ_T* pt_next;
} MIDXBULD_ELMT_TBL_OBJ_T;

/*------------------------------------------------------------------*/
/*! @struct MIDXBULD_ELMT_TBL_INFO_T
 *  @brief Element table infor.
 *  @code
 *  typedef struct _MIDXBULD_ELMT_TBL_INFO_T
 *  {
 *      UINT32                  ui4_tbl_id; 
 *      MIDXBULD_ELMT_TBL_OBJ_T t_elmt_tbl; 
 *  } MIDXBULD_ELMT_TBL_INFO_T;
 *  @endcode
 *  @li@c  ui4_tbl_id                                        - Table id.
 *  @li@c  t_elmt_tbl                                        - Element table infor.
 */
/*------------------------------------------------------------------*/
typedef struct _MIDXBULD_ELMT_TBL_INFO_T
{
    UINT32                  ui4_tbl_id; /* (I) */
    MIDXBULD_ELMT_TBL_OBJ_T t_elmt_tbl; /* (O) */
} MIDXBULD_ELMT_TBL_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct MIDXBULD_KEY_TBL_EXT_T
 *  @brief Externt key table.
 *  @code
 *  typedef struct _MIDXBULD_KEY_TBL_EXT_T
 *  {
 *      MIDXBULD_ELMT_TBL_OBJ_T          t_elmt_tbl;
 *      struct _MIDXBULD_KEY_TBL_EXT_T*  pt_next;
 *  } MIDXBULD_KEY_TBL_EXT_T;
 *  @endcode
 *  @li@c  t_elmt_tbl                                        - Element table infor.
 *  @li@c  pt_next                                           - Next table.
 */
/*------------------------------------------------------------------*/
typedef struct _MIDXBULD_KEY_TBL_EXT_T
{
    MIDXBULD_ELMT_TBL_OBJ_T          t_elmt_tbl;
    BOOL                             b_support_3d_trick;
    struct _MIDXBULD_KEY_TBL_EXT_T*  pt_next;
} MIDXBULD_KEY_TBL_EXT_T;

/*------------------------------------------------------------------*/
/*! @struct MIDXBULD_KEY_TBL_INFO_T
 *  @brief Key table infor.
 *  @code
 *  typedef struct _MIDXBULD_KEY_TBL_INFO_T
 *  {
 *      UINT32                      ui4_key_tbl_id; 
 *      MIDXBULD_KEY_TBL_EXT_T*     pt_key_tbl_ext; 
 *  } MIDXBULD_KEY_TBL_INFO_T;
 *  @endcode
 *  @li@c  ui4_key_tbl_id                                    - Key table id.
 *  @li@c  pt_key_tbl_ext                                    - Extend key table.
 */
/*------------------------------------------------------------------*/
typedef struct _MIDXBULD_KEY_TBL_INFO_T
{
    UINT32                      ui4_key_tbl_id; /* (I) */
    MIDXBULD_KEY_TBL_EXT_T*     pt_key_tbl_ext; /* (O) */
} MIDXBULD_KEY_TBL_INFO_T;

typedef struct 
{
    VOID*  pt_mp4_cmov_info;            /* for MP4 cmov only */
    VOID*  pt_piff_inet_info;           /* for piff inet only */
    VOID*  pt_mp4_multi_file_info;      /* for MP4 seamless playback only */
} MP4_MIDXBULD_EXTRA_INFO_T;
/* This structure lists the indexing source information for creating index table */
/*------------------------------------------------------------------*/
/*! @struct MIDXBULD_SOURCE_INFO_T
 *  @brief Source infor.
 *  @code
 *  typedef struct _MIDXBULD_SOURCE_INFO_T
 *  {
 *      UINT32 ui4_idx_type;       
 *      UINT64 ui8_data_offset;    
 *      UINT64 ui8_data_size;      
 *      UINT64 ui8_idx_offset;     
 *      UINT64 ui8_idx_size;       
 *      UINT64 ui8_seg_data_offset;
 *      UINT64 ui8_seg_tm_scale;   
 *      UINT8  ui1_total_strm_num; 
 *      UINT32 ui4_total_duration; 
 *      UINT32 ui4_packet_sz;      
 *      VOID*  pt_mp4_cmov_info;   
 *      UINT32 ui4_cbr_sub_vbr;    
 *      MIDXBULD_ELMT_STRM_TBL_INFO_T t_es_map[MIDXBULD_ELMT_TBL_MAX];
 *  #if SUPPORT_MULTI_INFO
 *      struct _MIDXBULD_SOURCE_INFO_T* pt_next;
 *  #endif
 *  } MIDXBULD_SOURCE_INFO_T;
 *  @endcode
 *  @li@c  ui4_idx_type                                      - Index type.
 *  @li@c  ui8_data_offset                                   - Data offset in case of >4G
 *  @li@c  ui8_data_size                                     - Data size.
 *  @li@c  ui8_idx_offset                                    - Index table offset.
 *  @li@c  ui8_idx_size                                      - Index table size.
 *  @li@c  ui8_seg_data_offset                               - Segment data offset.
 *  @li@c  ui8_seg_tm_scale                                  - Segment time scale.
 *  @li@c  ui1_total_strm_num                                - Totle stream number.
 *  @li@c  ui4_total_duration                                - Total duration
 *  @li@c  ui4_packet_sz                                     - Packet size.
 *  @li@c  pt_mp4_cmov_info                                  - Mp4 compact moov info.
 *  @li@c  ui4_cbr_sub_vbr                                   - Cbr sub vbr.
 *  @li@c  t_es_map[MIDXBULD_ELMT_TBL_MAX]                   - Element stream infor table.
 *  @li@c  pt_next                                           - Next table.
 */
/*------------------------------------------------------------------*/
typedef struct _MIDXBULD_SOURCE_INFO_T
{
    UINT32 ui4_idx_type;       /* type none, common or stream */
    UINT64 ui8_data_offset;    /* for AVI format, start from 'movi'; */
    UINT64 ui8_data_size;      /* for AVI format, include 'movi'; */
    UINT64 ui8_idx_offset;     /* for AVI format, start from 'idx1'; */
    UINT64 ui8_idx_size;       /* for AVI format, not include fcc('idx1') & cb; */
    UINT64 ui8_seg_data_offset;/* for MKV format, segment data start position*/
    UINT64 ui8_seg_tm_scale;   /* for MKV format, segment timecode scale*/
    UINT8  ui1_total_strm_num; /* total stream number */
    UINT32 ui4_total_duration; /* file playback duration in PTS */
    UINT32 ui4_packet_sz;      /* for ASF format only */
    UINT32 ui4_cbr_sub_vbr;    /* for AVI audio pts offset */
    MP4_MIDXBULD_EXTRA_INFO_T       t_mp4_extra_info;
    MIDXBULD_ELMT_STRM_TBL_INFO_T t_es_map[MIDXBULD_ELMT_TBL_MAX];
#if SUPPORT_MULTI_INFO
    struct _MIDXBULD_SOURCE_INFO_T* pt_next;
#endif
#ifdef MM_BAD_INTERLEAVED_FILE_SUPPORT
    BOOL  b_is_bad_interleaved; /*for bad interleaved*/
    UINT32 ui4_1st_pkgnum;      /*for bad interleaved*/
#endif
} MIDXBULD_SOURCE_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct MIDXBULD_KEY_INFO_T
 *  @brief Key table infor.
 *  @code
 *  typedef struct _MIDXBULD_KEY_INFO_T
 *  {
 *      UINT32 ui4_strm_id;           
 *      UINT32 ui4_scale;             
 *      UINT32 ui4_rate;              
 *      UINT32 ui4_sample_sz;         
 *      UINT32 ui4_avg_bytes_per_sec; 
 *      UINT32 ui4_range_id;          
 *      UINT32 ui4_tbl_id;            
 *  #if SUPPORT_MULTI_INFO
 *      struct _MIDXBULD_KEY_INFO_T* pt_next;
 *  #endif
 *  } MIDXBULD_KEY_INFO_T;
 *  @endcode
 *  @li@c  ui4_strm_id                                       - Stream id.
 *  @li@c  ui4_scale                                         - Scale.
 *  @li@c  ui4_rate                                          - Rate.
 *  @li@c  ui4_sample_sz                                     - Sample size.
 *  @li@c  ui4_avg_bytes_per_sec                             - Average bytes per second.
 *  @li@c  ui4_range_id                                      - Range id.
 *  @li@c  ui4_tbl_id                                        - Table id.
 *  @li@c  pt_next                                           - Next table.
 */
/*------------------------------------------------------------------*/
typedef struct _MIDXBULD_KEY_INFO_T
{
    UINT32 ui4_strm_id;           /* (I) */
    UINT32 ui4_scale;             /* (I) */
    UINT32 ui4_rate;              /* (I) */
    UINT32 ui4_sample_sz;         /* (I) will be obsolete & replaced by ui4_avg_bytes_per_sec, plz don't use it anymore */
    UINT32 ui4_avg_bytes_per_sec; /* (I) zero means VBR */
    UINT32 ui4_range_id;          /* (O) */
    UINT32 ui4_tbl_id;            /* (O) */
    /*For asf  index object use, if the most significant bit is set then index obejcts exsits,
          the other bits indicates non-hidden stream number related to simple index objects
          aui1_idx_strm_map[0].bit0 is most.*/
    UINT8 aui1_idx_strm_map[16]; 
#if SUPPORT_MULTI_INFO
    struct _MIDXBULD_KEY_INFO_T* pt_next;
#endif
} MIDXBULD_KEY_INFO_T;

/* for old mp3/msvctx compatible */
/*------------------------------------------------------------------*/
/*! @struct MIDXBULD_OFFSET_CONVERT_T
 *  @brief Offset convert infor.
 *  @code
 *  typedef struct _MIDXBULD_OFFSET_CONVERT_T
 *  {    
 *      UINT32 ui4_time_offset;    
 *      UINT32 ui4_byte_offset;    
 *      BOOL   b_is_out_of_range;
 *  } MIDXBULD_OFFSET_CONVERT_T;
 *  @endcode
 *  @li@c  ui4_time_offset                                   - Time offset.
 *  @li@c  ui4_byte_offset                                   - Byte offset.
 *  @li@c  b_is_out_of_range                                 - Tag if out of ragne.
 */
/*------------------------------------------------------------------*/
typedef struct _MIDXBULD_OFFSET_CONVERT_T
{    
    UINT32 ui4_time_offset;    
    UINT32 ui4_byte_offset;    
    BOOL   b_is_out_of_range;
} MIDXBULD_OFFSET_CONVERT_T;

/************************************************************************/
/* Error code                                                           */
/************************************************************************/
#define MIDXBULDR_ABORT_INDEXING            ((INT32)    1)    /**<        */
#define MIDXBULDR_OK                        ((INT32)    0)    /**<        */
#define MIDXBULDR_INSUFFICIENT_BUFFER       ((INT32)   -1)    /**<        */
#define MIDXBULDR_FILE_OR_DIR_MISSING       ((INT32)   -2)    /**<        */
#define MIDXBULDR_INV_HANDLE                ((INT32)   -3)    /**<        */
#define MIDXBULDR_OUT_OF_HANDLE             ((INT32)   -4)    /**<        */
#define MIDXBULDR_OUT_OF_MEMORY             ((INT32)   -5)    /**<        */
#define MIDXBULDR_FCT_NOT_SUPPORT           ((INT32)   -6)    /**<        */
#define MIDXBULDR_NAME_EXIST                ((INT32)   -7)    /**<        */
#define MIDXBULDR_INV_ARG                   ((INT32)   -8)    /**<        */
#define MIDXBULDR_HANDLER_NOT_FOUND         ((INT32)   -9)    /**<        */
#define MIDXBULDR_INTERNAL_ERR              ((INT32)   -10)    /**<        */
#define MIDXBULDR_REENTRY_LOADING           ((INT32)   -11)    /**<        */
#define MIDXBULDR_INITIAL_ERR               ((INT32)   -12)    /**<        */
#define MIDXBULDR_MSG_Q_ERR                 ((INT32)   -13)    /**<        */
#define MIDXBULDR_FILE_ERR                  ((INT32)   -14)    /**<        */
#define MIDXBULDR_CLI_ERROR                 ((INT32)   -15)    /**<        */
#define MIDXBULDR_AUTOFREE_ERROR            ((INT32)   -16)    /**<        */
#define MIDXBULDR_FMT_NOT_SUPPORT           ((INT32)   -17)    /**<        */
#define MIDXBULDR_KEY_TBL_NOT_READY         ((INT32)   -18)    /**<        */
#define MIDXBULDR_RANGE_ERROR               ((INT32)   -19)    /**<        */
#define MIDXBULDR_ENTRY_NOT_EQUAL           ((INT32)   -20)    /**<        */

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/* This callback function is the media index builder module notification function. This will be called when media index builder status changed. 
Indexing media is take time so it is asynchronous functions; it needs this notification function to notify applications. 
After indexing done, AP can seek accurately and get accurate duration. Before that these should be not accurate.*/  
/*------------------------------------------------------------------*/
/*! @brief Midxbuld notify function when index build finish a step.
 *  @param [in] h_handle  - Specifies which handle will be notified.
 *  @param [in] e_event  - It identifies the event type of notification.
 *  @param [in] pv_data1  - Specify the notification data1 which is followed with the notification event. 
 *  @param [in] pv_data2  - Specify the notification data1 which is followed with the notification event. 
 *  @param [in] pv_tag  - The client data specified by the client when callback has been called, the callback function will send it again.
 *  @param [out]
 *  @note
 *  @see
 *  @return
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_midxbuld_nfy_fct) (
                        HANDLE_T                  h_handle, /* (I) Specifies which handle will be notified. */
                        MIDXBULD_NFY_EVENT_TYPE_T e_event,  /* (I) it identifies the event type of notification. */
                        const VOID*               pv_data1, /* (I) specify the notification data1 which is followed with the notification event. */
                        const VOID*               pv_data2, /* (I) specify the notification data2 which is followed with the notification event. */
                        const VOID*               pv_tag);  /* (I) The client data specified by the client when callback has been called, the callback function will send it again. */

/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_MUTIL_MIDXBULD */
/*----------------------------------------------------------------------------*/


/*! @page pageMW_MUTIL_MIDXBULD Multi Media Index Build
 *  @par
 *  @section secMW_MUTIL_MIDXBULD_1  1. Introduction
 *      This part is to build the index table if needed.
 *  @par
 *  @section secMW_MUTIL_MIDXBULD_2  2. Concept
 *      This part is to build index table.
 *  @par
 *  @section secMW_MUTIL_MIDXBULD_3  3. Brief Design
 *      As the memery is limited, mp4 will build by range.
 *  @par
 *      @include midxbuld_sample_code.c
 *      
 */

#endif /* _U_MIDXBULD_H_ */



