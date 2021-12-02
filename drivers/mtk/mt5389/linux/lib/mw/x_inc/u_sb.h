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
/*! @file u_sb.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains SVL Builder specific
 *         definitions, which are exported.
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_SB_H_
#define _U_SB_H_

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_SVL_BLDR
 *  @{
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_handle_grp.h"
#include "u_svl.h"

#ifdef  SB_LEAKAGE_TEST

#include "x_leakage_test.h"

#else /* SB_LEAKAGE_TEST */

#define x_sb_mem_alloc(z_size)          x_mem_alloc(z_size)    /**<sb memory allocate */
#define x_sb_mem_free(pv_mem_block)     x_mem_free(pv_mem_block)    /**<sb memory free        */

#endif  /* SB_LEAKAGE_TEST */

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* SVL Builder Return values */
#define SBR_OK                          ((INT32)     0)    /**< ok       */
#define SBR_ALREADY_INIT                ((INT32)    -1)    /**< engine already init       */
#define SBR_BLDR_NOT_FOUND              ((INT32)    -2)    /**< no such build name       */
#define SBR_DUP_NAME                    ((INT32)    -3)    /**< duplicate name       */
#define SBR_FILE_NOT_FOUND              ((INT32)    -4)    /**< storage file not found       */
#define SBR_INTERNAL_ERROR              ((INT32)    -5)    /**< internal error       */
#define SBR_INV_ARG                     ((INT32)    -6)    /**< invalid arguments       */
#define SBR_INV_COND                    ((INT32)    -7)    /**< invalid condition       */
#define SBR_INV_HANDLE                  ((INT32)    -8)    /**< invalid handle       */
#define SBR_LIST_NOT_FOUND              ((INT32)    -9)    /**< no such channel list       */
#define SBR_NOT_INIT                    ((INT32)   -10)    /**< sb eng is not init       */
#define SBR_NOT_MATCH                   ((INT32)   -11)    /**< can't match sb eng       */
#define SBR_NOT_STARTER                 ((INT32)   -12)    /**< scan is not started       */
#define SBR_OUT_OF_HANDLES              ((INT32)   -13)    /**< out of handle       */
#define SBR_OUT_OF_MEM                  ((INT32)   -14)    /**< out of memory       */
#define SBR_ERR_LOCK                    ((INT32)   -15)    /**< can't lock critical code       */
#define SBR_ERR_UNLOCK                  ((INT32)   -16)    /**< can't unlock critical code       */
#define SBR_ERR_READ_REC                ((INT32)   -17)    /**< read svl record error       */
#define SBR_ERR_UPDT_REC                ((INT32)   -18)    /**< update svl record error       */
#define SBR_ERR_DEL_REC                 ((INT32)   -19)    /**< delete svl record error       */
#define SBR_ERR_SAVE_LIST               ((INT32)   -20)    /**< save channel list error       */
#define SBR_ERR_LOAD_LIST               ((INT32)   -21)    /**< load channel list error       */
#define SBR_ERR_CLEAN_LIST              ((INT32)   -22)    /**< clean channel list error       */
#define SBR_ERR_OUT_OF_RESOURCES        ((INT32)   -23)    /**< out of resouce       */
#define SBR_NOT_ENOUGH_SPACE            ((INT32)   -24)    /**< space not enough       */
#define SBR_NOT_FOUND                   ((INT32)   -25)    /**< not found       */
#define SBR_OUT_OF_RANGE                ((INT32)   -26)    /**< over the the scan range        */
#define SBR_INV_KEY_TYPE                ((INT32)   -27)    /**< invalid sb_set or sb_get key type       */
#define SBR_INV_KEY_INFO                ((INT32)   -28)    /**< invalid sb_set or sb_get key info       */
#define SBR_NOT_SUPPORT                 ((INT32)   -29)    /**< this operation is not support       */
#define SBR_INV_DB_VERSION              ((INT32)   -30)    /**< invalid database version       */

/* SVL Builder Notification Reason */
#define SB_REASON_UNKNOWN               ((UINT32)  (0x00000000))    /**<scan stop reason unknown        */
#define SB_REASON_COMPLETE              ((UINT32)  (0x00000001))    /**<scan stop reason complete        */
#define SB_REASON_PROGRESS              MAKE_BIT_MASK_32 (1)    /**<scan condition in progress        */
#define SB_REASON_CANCEL                MAKE_BIT_MASK_32 (2)    /**<scan stop reason cancel        */
#define SB_REASON_ABORT                 MAKE_BIT_MASK_32 (3)    /**<scan stop reason abort        */
#define SB_REASON_FREQ_REPORT           MAKE_BIT_MASK_32 (4)    /**<scan condition to report current frequency*/

#define SB_VNET_ALL                     ((UINT32)   (0x00000001))    /**<svl record bit mask        */
#define SB_VNET_ACTIVE                  MAKE_BIT_MASK_32 (1)    /**<svl record bit mask        */
#define SB_VNET_EPG                     MAKE_BIT_MASK_32 (2)    /**<svl record bit mask        */
#define SB_VNET_VISIBLE                 MAKE_BIT_MASK_32 (3)    /**<svl record bit mask        */
#define SB_VNET_FAVORITE1               MAKE_BIT_MASK_32 (4)    /**<svl record bit mask        */
#define SB_VNET_FAVORITE2               MAKE_BIT_MASK_32 (5)    /**<svl record bit mask        */
#define SB_VNET_FAVORITE3               MAKE_BIT_MASK_32 (6)    /**<svl record bit mask        */
#define SB_VNET_FAVORITE4               MAKE_BIT_MASK_32 (7)    /**<svl record bit mask        */
#define SB_VNET_BLOCKED                 MAKE_BIT_MASK_32 (8)    /**<svl record bit mask        */
#define SB_VNET_OOB_LIST                MAKE_BIT_MASK_32 (9)    /**<svl record bit mask        */
#define SB_VNET_INB_LIST                MAKE_BIT_MASK_32 (10)    /**<svl record bit mask        */
#define SB_VNET_SCRAMBLED               MAKE_BIT_MASK_32 (11)    /**<svl record bit mask        */
#define SB_VNET_BACKUP1                 MAKE_BIT_MASK_32 (12)    /**<svl record bit mask        */
#define SB_VNET_BACKUP2                 MAKE_BIT_MASK_32 (13)    /**<svl record bit mask        */
#define SB_VNET_BACKUP3                 MAKE_BIT_MASK_32 (14)    /**<svl record bit mask        */
#define SB_VNET_FAKE                    MAKE_BIT_MASK_32 (15)    /**<svl record bit mask        */
#define SB_VNET_USER_TMP_UNLOCK         MAKE_BIT_MASK_32 (16)    /**<svl record bit mask        */
#define SB_VNET_CH_NAME_EDITED          MAKE_BIT_MASK_32 (17)    /**<svl record bit mask        */
#define SB_VNET_LCN_APPLIED             MAKE_BIT_MASK_32 (18)    /**<svl record bit mask        */
#define SB_VNET_USE_DECODER             MAKE_BIT_MASK_32 (19)    /**<svl record bit mask        */
#define SB_VNET_ACTIVE_EPG_EDITED       MAKE_BIT_MASK_32 (20)    /**<deprecated, please use  SB_VOPT_CH_NUM/NAME_EDITED        */
#define SB_VNET_FREQ_EDITED             MAKE_BIT_MASK_32 (21)    /**<svl record bit mask        */
#define SB_VNET_REMOVAL                 MAKE_BIT_MASK_32 (22)    /**<svl record bit mask        */
#define SB_VNET_REMOVAL_TO_CONFIRM      MAKE_BIT_MASK_32 (23)    /**< svl record bit mask       */

#define SB_VNET_USER_INDEX_BGN          ((INT32) 24)    /**< svl record bit mask       */
#define SB_VNET_USER_INDEX_END          ((INT32) 31)    /**<svl record bit mask        */

#define SB_MAKE_VNET_USER(x)            MAKE_BIT_MASK_32 ((x))    /**< svl record bit mask       */

/* Redefine for DVB */
#define SB_VNET_NUMERIC_SELECTABLE      MAKE_BIT_MASK_32 (9)    /**< svl record bit mask       */
#define SB_VNET_RADIO_SERVICE           MAKE_BIT_MASK_32 (10)    /**<svl record bit mask        */
#define SB_VNET_ANALOG_SERVICE          MAKE_BIT_MASK_32 (12)    /**< svl record bit mask       */
#define SB_VNET_TV_SERVICE              MAKE_BIT_MASK_32 (13)    /**<svl record bit mask        */
#define SB_VNET_USE_DECODER_2           MAKE_BIT_MASK_32 (14)    /**<svl record bit mask        */

/* Redefine for PAL/SECAM */
#define SB_VNET_ACI_APPIED              MAKE_BIT_MASK_32 (13)    /**<svl record bit mask        */
#define SB_VNET_DEFAULT_CH              MAKE_BIT_MASK_32 (14)    /**<svl record bit mask        */


/* SVL record options mask */
#define SB_VOPT_ALL                     ((UINT32)   (0x00000001))    /**<svl record bit mask        */
#define SB_RECORD_NOT_SAVE_CH_NUM       MAKE_BIT_MASK_32 (1)    /**<svl record bit mask        */
#define SB_VOPT_NOT_SAVE_CH_NUM         MAKE_BIT_MASK_32 (1)    /**<svl record bit mask        */
   
#define SB_VOPT_USER_TMP_UNLOCK         MAKE_BIT_MASK_32 (2)    /**<svl record bit mask        */
#define SB_VOPT_CH_NAME_EDITED          MAKE_BIT_MASK_32 (3)    /**<svl record bit mask        */
#define SB_VOPT_FREQ_EDITED             MAKE_BIT_MASK_32 (4)    /**<svl record bit mask        */
#define SB_VOPT_CH_NAME_EMPTY_EDITED    MAKE_BIT_MASK_32 (5)


#define SB_VOPT_USER_DEFINED            (MAKE_BIT_MASK_32 (31)| \
                                         MAKE_BIT_MASK_32 (30)| \
                                         MAKE_BIT_MASK_32 (29)| \
                                         MAKE_BIT_MASK_32 (28)| \
                                         MAKE_BIT_MASK_32 (27)| \
                                         MAKE_BIT_MASK_32 (26)| \
                                         MAKE_BIT_MASK_32 (25)| \
                                         MAKE_BIT_MASK_32 (24))    /**<svl record bit mask        */

/* DVB */
#define SB_VOPT_LCN_APPLIED             MAKE_BIT_MASK_32 (5)    /**<svl record bit mask        */
#define SB_VOPT_MANUAL_OBTAINED         MAKE_BIT_MASK_32 (6)    /**< svl record bit mask       */
#define SB_VOPT_HD_SIMULCAST            MAKE_BIT_MASK_32 (7)    /**<svl record bit mask        */
#define SB_VOPT_SDT_AVAILABLE           MAKE_BIT_MASK_32 (9)    /**<svl record bit mask        */
#define SB_VOPT_CH_NUM_EDITED           MAKE_BIT_MASK_32 (10)    /**<svl record bit mask        */
#define SB_VOPT_PORTUGAL_HD_SIMULCAST   MAKE_BIT_MASK_32 (11)    /**<svl record bit mask        */
#define SB_VOPT_CURRENT_COUNTRY         MAKE_BIT_MASK_32 (12)    /**<svl record bit mask        */
#define SB_VOPT_DELETED_BY_USER         MAKE_BIT_MASK_32 (13)    /**<svl record bit mask        */
#define SB_VOPT_NVOD_REF                MAKE_BIT_MASK_32 (14)    /**<svl record bit mask        */
#define SB_VOPT_NVOD_TS                 MAKE_BIT_MASK_32 (15)    /**<svl record bit mask        */
#define SB_VOPT_SVC_REMOVE_NORDIG_SIMULICAST   MAKE_BIT_MASK_32 (16)    /**<svl record bit mask        */
#define SB_VOPT_HD_SIMULICAST_LCN_APPLIED   MAKE_BIT_MASK_32 (17)    /**<svl record bit mask        */
#define SB_VOPT_REMOVED_BY_HD_SIMULICAST MAKE_BIT_MASK_32 (18)    /**<svl record bit mask        */
#define SB_VOPT_LCN_REORDERING          MAKE_BIT_MASK_32 (19)
#define SB_VOPT_3D_SERVICE              MAKE_BIT_MASK_32 (20)
#define SB_VOPT_NZL_REMOVE_DUPLICATE_LCN_CH          MAKE_BIT_MASK_32 (21)


/* PAL/SECAM */
#define SB_VOPT_ACI_APPIED              MAKE_BIT_MASK_32 (7)    /**<svl record bit mask        */

#define SB_VOPT_SIGNAL_BOOSTER          MAKE_BIT_MASK_32 (8)    /**<svl record bit mask        */

/*Redefine for ISDB-T*/
#define SB_VOPT_HAS_SMD                 MAKE_BIT_MASK_32 (13)    /**<svl record bit mask        */
#define SB_VOPT_IS_BRDCSTING            MAKE_BIT_MASK_32 (14)    /**<svl record bit mask        */
#define SB_VOPT_MODIFIED_VISIBLE        MAKE_BIT_MASK_32 (17)
#define SB_VOPT_IS_ONE_SEGMENT          MAKE_BIT_MASK_32 (18)

/*ISDB-T*/
/*TSL record operation masks*/
#define SB_TSL_MASK_CONFIG_DATA_SVC                 MAKE_BIT_MASK_32 (1)/*the user configure in scan result*/

/*DVBS*/
/*TSL record operation masks*/
#define SB_TSL_MASK_ADDED_BY_USER                   MAKE_BIT_MASK_32 (1)/*the ts record is added by user*/
#define SB_TSL_MASK_ADDED_BY_SCAN                   MAKE_BIT_MASK_32 (2)/*the ts record is added by auto scan */
#define SB_TSL_MASK_EDITED                          MAKE_BIT_MASK_32 (3)/*the ts record is edited by user */
#define SB_TSL_MASK_CONFIG_FREE_SVC_ONLY            MAKE_BIT_MASK_32 (4)/*the user configure in scan result*/
#define SB_TSL_MASK_CONFIG_TV_SVC_ONLY              MAKE_BIT_MASK_32 (5)/*the user configure in scan result */
#define SB_TSL_MASK_CONFIG_RADIO_SVC_ONLY           MAKE_BIT_MASK_32 (6)/*the user configure in scan result */

/*Network mask defination*/
#define SB_NWL_MASK_FAV_CANDIDATE       MAKE_BIT_MASK_32 (1)
#define SB_NWL_MASK_FAV                 MAKE_BIT_MASK_32 (2)
#define SB_NWL_MASK_REMOVAL             MAKE_BIT_MASK_32 (3)

/*Satellite record operation masks*/
#define SB_SATL_MASK_ADDED              MAKE_BIT_MASK_32 (1)/*the satellite record is added by user*/
#define SB_SATL_MASK_EDITED             MAKE_BIT_MASK_32 (2)/*the satellite record is edited by user */
#define SB_SATL_MASK_HIDDEN             MAKE_BIT_MASK_32 (3)/*the satellite record is hidden by user */

#define SB_CH_IDX_FOR_ANALOG                    0x00
#define SB_CH_IDX_FOR_TV                        0x01
#define SB_CH_IDX_FOR_RADIO                     0x02
#define SB_CH_IDX_FOR_DATA                      0x03

/* SVL Builder States */
/*------------------------------------------------------------------*/
/*! @enum SB_COND_T
 *  @brief Svl builder engine condition
 *  @code
 *  typedef enum
 *  {
 *      SB_COND_AVAIL = 0,
 *      SB_COND_UNCONFIG,
 *      SB_COND_INITING,
 *      SB_COND_BUILDING,
 *      SB_COND_UPDATE,
 *      SB_COND_CLOSED 
 *  } SB_COND_T;
 *  @endcode
 *  @li@c  SB_COND_AVAIL                                     - Current state is available for scan
 *  @li@c  SB_COND_UNCONFIG                                  - Current state is never scaned
 *  @li@c  SB_COND_INITING                                   - Current state is initializing, can't scan
 *  @li@c  SB_COND_BUILDING                                  - Current state is scaning
 *  @li@c  SB_COND_UPDATE                                    - The update is notified by table manager
 *  @li@c  SB_COND_CLOSED                                    - Current state is closed, can't scan
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SB_COND_AVAIL = 0,
    SB_COND_UNCONFIG,
    SB_COND_INITING,
    SB_COND_BUILDING,
    SB_COND_UPDATE,
    SB_COND_CLOSED /* used to notify the client */
} SB_COND_T;


/* SVL Builder notification function type of state changes */
/*------------------------------------------------------------------*/
/*! @brief x_sb_nfy_fct callback function is call back when scan engine condition changed and registered on SB handle creating.
 *  @param [in] the notified engine handle
 *  @param [in] the condtion of notified engine
 *  @param [in] the tag value passed by AP
 *  @return      NA   
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_nfy_fct) (
      HANDLE_T          h_builder,
      SB_COND_T         e_builder_cond,
      VOID*             pv_nfy_tag);


/* SVL Builder notification function type of building progress/status */
/*------------------------------------------------------------------*/
/*! @brief x_sb_scan_nfy_fct is call back when scan engine notify scan progress. 
 *  @param [in] the notified sb engine    
 *  @param [in] the notified reason
 *  @param [in] the notified data which include the channels scaned
 *  @param [in] the tag value passed by AP
 *  @return      NA
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_scan_nfy_fct) (
      HANDLE_T          h_builder,
      UINT32            ui4_nfy_reason,
      INT32             i4_nfy_data,
      VOID*             pv_nfy_tag);


/* index macros for duplicated channel numbers */
#ifndef MW_SB_MAX_CH_INDEX_BITS
#define _CH_INDEX_BITS                  ((UINT32) 6)    /**< internal use      */
#else
#define _CH_INDEX_BITS                  ((UINT32) MW_SB_MAX_CH_INDEX_BITS)    /**<internal use        */
#endif

#define _CH_INDEX_MASK                  (MAKE_BIT_MASK_32(_CH_INDEX_BITS) - 1)    /**<internal use        */

/* Macros for ATSC channel number */
#define SB_ATSC_ONE_PART_FLAG           MAKE_BIT_MASK_32((_CH_INDEX_BITS+1))    /**<internal use       */
#define SB_ATSC_NON_PSIP_FLAG           MAKE_BIT_MASK_32(_CH_INDEX_BITS)    /**<internal use       */

/* SB_ATSC_ONE_PART_FLAG & SB_ATSC_NON_PSIP_FLAG */
#define _CH_ATTR_BITS                   ((UINT32) 2)    /**<internal use        */

#define _NON_NUM_BITS                   (_CH_INDEX_BITS + _CH_ATTR_BITS)    /**< internal use       */

/* two-part major/minor macros */
#define _MAJOR_NUM_BITS                 ((UINT32) 14)    /**< internal use       */
#define _MINOR_NUM_BITS                 ((UINT32) 32 - (_MAJOR_NUM_BITS + _NON_NUM_BITS))    /**< internal use       */

#define _MAJOR_NUM_MASK                 (MAKE_BIT_MASK_32(_MAJOR_NUM_BITS) - 1)    /**< internal use       */
#define _MINOR_NUM_MASK                 (MAKE_BIT_MASK_32(_MINOR_NUM_BITS) - 1)    /**< internal use       */

/* One-Part major/minor macros. Note: _OP_MAJOR_NUM_BITS MUST be 14-bit */
#define _OP_MAJOR_NUM_BITS              ((UINT32) 14)    /**< internal use       */
#define _OP_MINOR_NUM_BITS              ((UINT32) 32 - (_OP_MAJOR_NUM_BITS + _NON_NUM_BITS))    /**< internal use       */

#define _OP_MAJOR_NUM_MASK              (MAKE_BIT_MASK_32(_OP_MAJOR_NUM_BITS) - 1)    /**< internal use       */
#define _OP_MINOR_NUM_MASK              (MAKE_BIT_MASK_32(_OP_MINOR_NUM_BITS) - 1)    /**< internal use       */

/* Non-Psip major/minor macros. Note: _NP_MINOR_NUM_BITS MUST be 16-bit to fit program number */
#define _NP_MINOR_NUM_BITS              ((UINT32) 16)    /**<        */
#define _NP_MAJOR_NUM_BITS              ((UINT32) 32 - (_NP_MINOR_NUM_BITS + _NON_NUM_BITS))    /**< internal use       */

#define _NP_MAJOR_NUM_MASK              (MAKE_BIT_MASK_32(_NP_MAJOR_NUM_BITS) - 1)    /**<internal use        */
#define _NP_MINOR_NUM_MASK              (MAKE_BIT_MASK_32(_NP_MINOR_NUM_BITS) - 1)    /**< internal use       */

#define SB_ATSC_MAX_CHANNEL_INDEX       _CH_INDEX_MASK    /**< internal use       */

#define SB_ATSC_GEN_CHANNEL_ID(major, minor, index)         \
    ((((UINT32) (major) & _MAJOR_NUM_MASK) << (_MINOR_NUM_BITS+_NON_NUM_BITS)) |  \
     (((UINT32) (minor) & _MINOR_NUM_MASK) << (_NON_NUM_BITS)) |  \
     (((UINT32) (index) & _CH_INDEX_MASK)))    /**< internal use       */

#define SB_ATSC_GEN_ONE_PART_CHANNEL_ID(major, index) \
    ((((UINT32) (major) & _OP_MAJOR_NUM_MASK) << (_OP_MINOR_NUM_BITS+_NON_NUM_BITS)) | \
     (((UINT32) (index) & _CH_INDEX_MASK)) | \
     SB_ATSC_ONE_PART_FLAG)    /**< internal use       */
     
#define SB_ATSC_GEN_ONE_PART_CHANNEL_ID_EX(major, index, extra) \
    ((((UINT32) (major) & _OP_MAJOR_NUM_MASK) << (_OP_MINOR_NUM_BITS+_NON_NUM_BITS)) | \
     (((UINT32) (extra) & _OP_MINOR_NUM_MASK) << (_NON_NUM_BITS)) | \
     (((UINT32) (index) & _CH_INDEX_MASK)) | \
     SB_ATSC_ONE_PART_FLAG)    /**<internal use        */

#define SB_ATSC_GEN_NON_PSIP_CHANNEL_ID(major, minor, index) \
    ((((UINT32) (major) & _NP_MAJOR_NUM_MASK) << (_NP_MINOR_NUM_BITS+_NON_NUM_BITS)) | \
     (((UINT32) (minor) & _NP_MINOR_NUM_MASK) << (_NON_NUM_BITS)) | \
     (((UINT32) (index) & _CH_INDEX_MASK)) | \
     SB_ATSC_NON_PSIP_FLAG)    /**< internal use       */

#define SB_ATSC_IS_ONE_PART_CHANNEL_ID(channel_id) \
    ((((UINT32) channel_id) & SB_ATSC_ONE_PART_FLAG) ? TRUE : FALSE)    /**<internal use       */

#define SB_ATSC_IS_NON_PSIP_CHANNEL_ID(channel_id) \
    ((((UINT32) channel_id) & SB_ATSC_NON_PSIP_FLAG) ? TRUE : FALSE)    /**< internal use       */
                     
#define SB_ATSC_GET_MAJOR_CHANNEL_NUM(channel_id) \
    ((SB_ATSC_IS_ONE_PART_CHANNEL_ID(channel_id)) ? \
        ((UINT16)(((UINT32)(channel_id) >> (_OP_MINOR_NUM_BITS+_NON_NUM_BITS)) & \
                                            _OP_MAJOR_NUM_MASK)) : \
        ((SB_ATSC_IS_NON_PSIP_CHANNEL_ID(channel_id)) ? \
        ((UINT16)(((UINT32)(channel_id) >> (_NP_MINOR_NUM_BITS+_NON_NUM_BITS)) & \
                                            _NP_MAJOR_NUM_MASK)) : \
        ((UINT16)(((UINT32)(channel_id) >> (_MINOR_NUM_BITS+_NON_NUM_BITS)) & \
                                           _MAJOR_NUM_MASK))))    /**<internal use        */
                    
#define SB_ATSC_GET_MINOR_CHANNEL_NUM(channel_id) \
    ((SB_ATSC_IS_NON_PSIP_CHANNEL_ID(channel_id)) ? \
        ((UINT16)(((UINT32)(channel_id) >> _NON_NUM_BITS) & _NP_MINOR_NUM_MASK)) : \
        ((UINT16)(((UINT32)(channel_id) >> _NON_NUM_BITS) & _MINOR_NUM_MASK)))    /**< internal use       */

#define SB_ATSC_GET_CHANNEL_INDEX(channel_id)              \
                    ((UINT8) (((UINT32)(channel_id)) & _CH_INDEX_MASK))    /**< internal use       */

#define SB_ATSC_GET_ONE_PART_CHANNEL_EXTRA_INDEX(channel_id) \
        ((UINT16)(((UINT32)(channel_id) >> _NON_NUM_BITS) & _OP_MINOR_NUM_MASK))    /**< internal use       */

#define SB_TSL_REC_ID_OFFSET_MASK           ((UINT16) 0xf000)    /**< internal use       */
#define SB_TSL_GET_PTC_FROM_RECID(x)        (((UINT16) (x)) & (~SB_TSL_REC_ID_OFFSET_MASK))    /**<internal use        */

#define SB_ANA_TSL_REC_ID_OFFSET            (((UINT16) 0x8000) & SB_TSL_REC_ID_OFFSET_MASK)    /**<internal use        */
#define SB_MAKE_ANA_TSL_REC_ID(x)           ((UINT16) (x) | SB_ANA_TSL_REC_ID_OFFSET)    /**< internal use       */

#define SB_DIG_TSL_REC_ID_OFFSET            (((UINT16) 0) & SB_TSL_REC_ID_OFFSET_MASK)    /**<internal use        */
#define SB_MAKE_DIG_TSL_REC_ID(x)           ((UINT16) (x) | SB_DIG_TSL_REC_ID_OFFSET)    /**< internal use       */

#define SB_AUX_DIG_TSL_REC_ID_OFFSET        (((UINT16) 0x4000) & SB_TSL_REC_ID_OFFSET_MASK)    /**<internal use        */
#define SB_MAKE_AUX_DIG_TSL_REC_ID(x)       ((UINT16) (x) | SB_AUX_DIG_TSL_REC_ID_OFFSET)    /**< internal use       */

/* Macros for DVB channel number */
/* These macros are designed to conform to the ATSC ones so that using the ATSC macros can
   also retrieve the "major" channel numbers */
#define SB_DVB_CH_NUM_FLAG              ((UINT32) 0x80)    /**<  to indicate it's one-part channel number     */

#define SB_DVB_CH_TYPE_RANGE                     3         /*The last 3bit is used for service type*/ 
#define SB_DVB_MAX_CHANNEL_INDEX_WITH_CH_TYPE  ((UINT8) 0x7)


#define SB_DVB_MAX_CHANNEL_INDEX        ((UINT8) 0xF)    /**<  bit masks for channel index */

#define SB_DVB_GEN_CHANNEL_ID(major, index) \
    ((((UINT32) (major) & 0x3FFF) << 18) | \
     ((index & SB_DVB_MAX_CHANNEL_INDEX)) | \
     SB_DVB_CH_NUM_FLAG)    /**< to generate channel id with major number and index */

#define SB_DVB_GEN_CHANNEL_ID_WITH_LCN(major, lcn, index) \
    ((((UINT32) (major) & 0x3FFF) << 18) | \
     (((UINT32) (lcn) & 0x3FF)  <<  8) | \
     ((index & SB_DVB_MAX_CHANNEL_INDEX)) | \
     SB_DVB_CH_NUM_FLAG                    | \
     (((UINT32)(lcn) & 0xC00)>>5))    /**< to generate channel id with major number, lcn and index */
                     
#define SB_DVB_GET_CHANNEL_NUM(channel_id) \
    ((UINT16)(((UINT32)(channel_id) >> 18) & 0x3FFF))    /**< to get the major channel number with channel_id       */
                    
#define SB_DVB_GET_LCN(channel_id) \
    ((UINT16)(((UINT32)(channel_id) >> 8) & 0x3ff)| \
    (((UINT32)channel_id & 0x60)<< 5))    /**< to get LCN with channel_id       */


#define SB_DVB_GET_CHANNEL_INDEX(channel_id) \
    ((UINT8) (((UINT32)(channel_id)) & SB_DVB_MAX_CHANNEL_INDEX))    /**< to get channel index with channel_id       */






#ifndef MW_SB_NP_CH_PREFIX
#define MW_SB_NP_CH_PREFIX      "RF"    /**<internal use        */
#endif

#ifndef MW_SB_NP_CH_POSTFIX
#define MW_SB_NP_CH_POSTFIX     ""    /**< internal use       */
#endif

#ifndef MW_SB_NP_CH_SEPARATOR
#define MW_SB_NP_CH_SEPARATOR   "-"    /**< internal use       */
#endif

/* Get function keys */
/*------------------------------------------------------------------*/
/*! @enum
 *  @brief the key type for sb_set or sb_get
 *  @code
 *  typedef enum
 *  {
 *      SB_KEY_TYPE_UNKNOWN = 0,
 *      SB_KEY_TYPE_FREQ_RANGE,
 *      SB_KEY_TYPE_COUNTRY_CODE,
 *      SB_KEY_TYPE_FLTR_ACI_INFO,
 *      SB_KEY_TYPE_ACI_SLCT_CITY,
 *      SB_KEY_TYPE_ACI_CANCEL,
 *      SB_KEY_TYPE_TUNER_FREQ_RANGE,   
 *      SB_KEY_TYPE_NW_CHANGE_NFY,      
 *      SB_KEY_TYPE_AUTO_UPDATE_OFF,        
 *      SB_KEY_TYPE_SET_TVD_SNK_NAME,          
 *      SB_KEY_TYPE_RESET_TVD_SNK_NAME,     
 *      SB_KEY_TYPE_INSERT_FAKE_SVL_TSL,	
 *      SB_KEY_TYPE_INSERT_DEFAULT_SVL_TSL, 
 *      SB_KEY_TYPE_FAKE_SETTING,           
 *      SB_KEY_TYPE_TOTAL_NUMBER,        
 *      SB_KEY_TYPE_CHANNEL_DATA_INTEGRITY,
 *      SB_KEY_TYPE_DISABLE_NW_CHANGE_NFY,  
 *  } SB_KEY_TYPE_T;
 *  @endcode
 *  @li@c  SB_KEY_TYPE_UNKNOWN                               - internal use
 *  @li@c  SB_KEY_TYPE_FREQ_RANGE                            - internal use
 *  @li@c  SB_KEY_TYPE_COUNTRY_CODE                          - internal use
 *  @li@c  SB_KEY_TYPE_FLTR_ACI_INFO                         - internal use
 *  @li@c  SB_KEY_TYPE_ACI_SLCT_CITY                         - internal use
 *  @li@c  SB_KEY_TYPE_ACI_CANCEL                            - internal use
 *  @li@c  SB_KEY_TYPE_TUNER_FREQ_RANGE                      - Output: SB_FREQ_RANGE_T
 *  @li@c  SB_KEY_TYPE_NW_CHANGE_NFY                         - Input: SB_DVB_NW_CHANGE_NFY_T
 *  @li@c  SB_KEY_TYPE_AUTO_UPDATE_OFF                       - BOOL: enable/disable auto update
 *  @li@c  SB_KEY_TYPE_SET_TVD_SNK_NAME                      - Input: Set TV Decoder Sink component Name only used on NTSC
 *  @li@c  SB_KEY_TYPE_RESET_TVD_SNK_NAME                    - Input: Reset TV Decoder Sink component Name only used on NTSC
 *  @li@c  SB_KEY_TYPE_INSERT_FAKE_SVL_TSL                   - Input: Insert SVL TSL records only used on NTSC
 *  @li@c  SB_KEY_TYPE_INSERT_DEFAULT_SVL_TSL                - Input: Insert SVL TSL records for default setting
 *  @li@c  SB_KEY_TYPE_FAKE_SETTING                          - BOOL:enable/disable, Set channel list to fake channel or default channel,default is on
 *  @li@c  SB_KEY_TYPE_CHANNEL_DATA_INTEGRITY                - check data os channels whether is integrate
 *  @li@c  SB_KEY_TYPE_DISABLE_NW_CHANGE_NFY                 - BOOL: true:Disable, false:Enable, disable nw change notification
 *  @li@c  SB_KEY_TYPE_TOTAL_NUMBER                          - total number of key type
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SB_KEY_TYPE_UNKNOWN = 0,
    SB_KEY_TYPE_FREQ_RANGE,
    SB_KEY_TYPE_COUNTRY_CODE,
    SB_KEY_TYPE_FLTR_ACI_INFO,
    SB_KEY_TYPE_ACI_SLCT_CITY,
    SB_KEY_TYPE_ACI_CANCEL,
    SB_KEY_TYPE_USER_OPERATION_FINISH,
    SB_KEY_TYPE_TUNER_FREQ_RANGE,   /* Output: SB_FREQ_RANGE_T */
    SB_KEY_TYPE_NW_CHANGE_NFY,      /* Input: SB_DVB_NW_CHANGE_NFY_T */
    SB_KEY_TYPE_AUTO_UPDATE_OFF,        /*BOOL: enable/disable*/
    SB_KEY_TYPE_SET_TVD_SNK_NAME,          /*Input: Set TV Decoder Sink component Name only used on NTSC*/
    SB_KEY_TYPE_RESET_TVD_SNK_NAME,     /*Input: Reset TV Decoder Sink component Name only used on NTSC*/
    SB_KEY_TYPE_INSERT_FAKE_SVL_TSL,	/*Input: Insert SVL TSL records only used on NTSC */
    SB_KEY_TYPE_INSERT_DEFAULT_SVL_TSL, /*Input: Insert SVL TSL records for default setting */
    SB_KEY_TYPE_FAKE_SETTING,           /*BOOL:enable/disable, Set channel list to fake channel or default channel,default is on*/
    SB_KEY_TYPE_CHANNEL_DATA_INTEGRITY,  /* check data os channels whether is integrate */
    SB_KEY_TYPE_DISABLE_NW_CHANGE_NFY,  /* BOOL: true:Disable, false:Enable, disable nw change notification*/
    SB_KEY_TYPE_FAV_NET,
    SB_KEY_TYPE_TUNER_NOTIFIED_FREQ,
    SB_KEY_TYPE_LCN_CONFLICT_NUM_GROUPS, /*DGTVi: output: pv_key_data: pointer contains number of conflict group (UINT8)*/
    /*DGTVi: Input: pv_key_info: group number(UINT8), 
          output: pv_key_data: pointers contains structure SB_DVB_LCN_CONFLICT_GROUP_T*/
    SB_KEY_TYPE_LCN_CONFLICT_NUM_CHANNELS_IN_GROUP, 
    /*DGTVi: Input: pv_key_info: group number(UINT8), 
          output: pv_key_data: pointers contains structure SB_DVB_LCN_CONFLICT_CHANNEL_T, should fill ui1_index inside the structure*/
    SB_KEY_TYPE_LCN_CONFLICT_CHANNEL_IN_GROUP,
    /*DGTVi: Input: pv_key_info: group number(UINT8), 
          output: pv_key_data: pointers contains structure SB_DVB_LCN_CONFLICT_CHANNEL_T, should fill ui1_index inside the structure*/    
    SB_KEY_TYPE_LCN_CONFLICT_PREFERRED_CHANNEL_IN_GROUP,
    SB_KEY_TYPE_CH_LST_LCNV2_CONFLICT_NUM,
    SB_KEY_TYPE_CH_LST_LCNV2_CONFLICT_ENTRY_BY_IDX,
    SB_KEY_TYPE_CH_LST_LCNV2_CONFLICT_PREFERRED,
    /*to get number of regions:
        Input: pv_key_info: SB_DVB_REGION_CODE_T
        if you want to query primary region, give ui1_level = 0;
        if you want to query second region, give ui1_level = 1, and aui1_code = {pri, 0, 0}
        if you want to query tertiary region, give ui1_level =2, and aui1_code = {pri, sec, 0}
        output: pv_key_data: pointer contains number of regions (UINT16)
        (*pz_key_data_len) must be bigger than sizeof (UINT16)*/
    SB_KEY_TYPE_NUM_REGIONS,
      /* use this key to get regions/set region 
        get:
        Input: pv_key_info: SB_DVB_REGION_CODE_T
        if you want to query primary region, give ui1_level = 0;
        if you want to query second region, give ui1_level = 1, and aui1_code = {pri, 0, 0}
        if you want to query tertiary region, give ui1_level =2, and aui1_code = {pri, sec, 0}
        output: pv_key_data: start pointer contains array of regions (SB_DVB_REGION_T)
        in/out: (*pz_key_data_len) must be bigger than sizeof(SB_DVB_REGION_T)*num_of_regions
        set:
        Input: 
        pv_key_data: pointer contains the region code (SB_DVB_REGION_CODE_T)
        z_key_data_len must be bigger than sizeof(SB_DVB_REGION_CODE_T)
        if the target region is primary region, give ui1_level = 1, and aui1_code={pri, 0, 0};
        if the target region is second region, give ui1_level =2, and aui1_code = {pri, sec, 0}
        if the target region is tertiary region, give ui1_level =3, and aui1_code = {pri, sec, ter}*/
    
    SB_KEY_TYPE_REGION,
     /* to set a preferred GUI language
        Input:
        pv_key_data: pointer contains the language code (ISO_639_LANG_T) 
        z_key_data_len must be equal sizeof(ISO_639_LANG_T)*/
    SB_KEY_TYPE_GUI_LANG,
    /* to set MF temp svl id (only for DVB-C)
        example:
        c_sb_set(h_builder, SB_KEY_TYPE_MF_TEMP_SVL_ID, NULL, sizeof(UINT16), ui2_tmp_svl_id)*/
    SB_KEY_TYPE_MF_TEMP_SVL_ID,
    /*to get last successful tuning parameters (only for DVB-C)
        example:
        z_len = sizeof(SB_DVBC_SCAN_DATA_T);
        c_sb_get(h_builder, SB_KEY_TYPE_LAST_TUNING_PARA, NULL, &z_len, &t_dvbc_scan_data)*/
    SB_KEY_TYPE_LAST_TUNING_PARA,
    SB_KEY_TYPE_CALC_FREQ_OFFSET_CB,
    /* to set operator name (only for DVB-C)
        example:
        c_sb_set(h_builder, SB_KEY_TYPE_OPERATOR_NAME, NULL, OPERATOR_NAME_COUNT_LEN, &t_operator_name)*/    
    SB_KEY_TYPE_OPERATOR_NAME,
    /*to get number of scanned channels (only for DVB-C)
        example:
        z_len = sizeof(SB_DVBC_NUM_CH);
        c_sb_get(h_builder, SB_KEY_TYPE_NUM_SCANNED_CH, NULL, &z_len, &t_dvbc_num_ch)*/    
    SB_KEY_TYPE_NUM_SCANNED_CH,
    SB_KEY_TYPE_TOTAL_NUMBER        /* total number of key types */
} SB_KEY_TYPE_T;


/*------------------------------------------------------------------*/
/*! @struct SB_FREQ_RANGE_T
 *  @brief define the frequency of scan range
 *  @code
 *  typedef struct _SB_FREQ_RANGE_T
 *  {
 *      UINT32  ui4_upper_freq;
 *      UINT32  ui4_lower_freq;
 *  }   SB_FREQ_RANGE_T;
 *  @endcode
 *  @li@c  ui4_upper_freq                                    - scan range upper bound
 *  @li@c  ui4_lower_freq                                    - scan range lower bound
 */
/*------------------------------------------------------------------*/
typedef struct _SB_FREQ_RANGE_T
{
    UINT32  ui4_upper_freq;
    UINT32  ui4_lower_freq;
}   SB_FREQ_RANGE_T;

/* The structure SB_NEW_SVC_NFY_DATA is used as the argument 
   of the new service notification function. */
/*------------------------------------------------------------------*/
/*! @struct SB_NEW_SVC_NFY_DATA
 *  @brief The structure SB_NEW_SVC_NFY_DATA is used as the argument of the new service notification function.
 *  @code
 *  typedef struct _SB_NEW_SVC_NFY_DATA
 *  {
 *      BRDCST_TYPE_T       e_brdcst_type;
 *      SVL_SERVICE_TYPE_T  e_serv_type;
 *      CHAR                ac_name[MAX_PROG_NAME_LEN+1];
 *  } SB_NEW_SVC_NFY_DATA;
 *  @endcode
 *  @li@c  e_brdcst_type                                     - the bradcast type
 *  @li@c  e_serv_type                                       - service type
 *  @li@c  ac_name[MAX_PROG_NAME_LEN+1]                      - channel name
 */
/*------------------------------------------------------------------*/
typedef struct _SB_NEW_SVC_NFY_DATA
{
    BRDCST_TYPE_T       e_brdcst_type;
    SVL_SERVICE_TYPE_T  e_serv_type;
    CHAR                ac_name[MAX_PROG_NAME_LEN+1];
} SB_NEW_SVC_NFY_DATA;


#define SB_MAX_LCN_V2_LIST_NAME_LEN       24
#define SB_MAX_NUM_CH_LIST_IDS            32

typedef struct _SB_LCNV2_CH_LST_INFO
{    
    CHAR            ac_channel_list_name[SB_MAX_LCN_V2_LIST_NAME_LEN];
    UINT8           ui1_channel_list_id;    
} SB_LCNV2_CH_LST_INFO;



/* The prototype of the new service notification function */
/*------------------------------------------------------------------*/
/*! @brief The prototype of the new service notification function
 *  @param [in] The notified data SB_NEW_SVC_NFY_DATA
 *  @param [in] the tag value passed in to scan engine
 *  @return      VOID
 *  @retval      -
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_sb_new_svc_nfy_fct) (
        SB_NEW_SVC_NFY_DATA* 	pt_nfy_data,
        VOID*                 pv_nfy_tag);


/*----------------------------------------------------------------------------*/
/*! @} */                                          /* end of groupMW_SVL_BLDR */
/*----------------------------------------------------------------------------*/
/*! @page pageMW_SVL_BLDR SVL Builder Detailed Description
 *  @par
 *  @section secMW_SVL_BLDR_1  1. Introduction
 *  @par
 *      In order to provide the basic digital TV operations, such as channel change or events display, the channel information must be cached or recorded in advance. Middleware and applications (Navigator, for example) can share the recorded information. NetWork List (NWL), SerVice List (SVL), and Transport Stream List (TSL) are designed for this purpose.
 *  @par
 *      The SVL Builder (SB) component, which is responsible for retrieving channel information from transport stream, and building NWL, SVL, and TSL. SB is able to create a connection of specified source (Tuner, for example) with Connection Manager (CM, a component of Middleware, see document SW Design Middleware-Connection Manager for more details), and retrieving channel information from the specified source (Tuner, for example). SB is able to change to next RF channel when it has retrieved all channel information from current transport stream. By interfacing with Table Manager (a component of Middleware, see document SW Design Middleware-Table Manager for more details), SB is able to retrieve the required service information and construct the NWL/SVL/TSL database records.
 *  @par
 *      SVL Bulder also provides the auto update scan (Monitor) feature, which can auto detect and update information of channels of the current transport stream.
 *  @par
 *  @section secMW_SVL_BLDR_2  2. Concept
 *  @par
 *  @subsection ssecMW_SVL_BLDR_2_1  2.1. TSL
 *  @par
 *      The content of a TSLDB object should be saved to a TSL file immediately after the scan process is finished.  This type of files would be loaded into TSLDB when the SB component is initialized.  If no such file exists, SB component will create a default TSL according to the builder type.
 *  @par
 *  @subsection ssecMW_SVL_BLDR_2_2  2.2. SVL
 *  @par
 *      The content of an SVLDB object should be saved to an SVL file immediately after the scan process is finished.  This type of files would be loaded into SVLDB when the SB component is initialized.  If no such file exists, SB component will create an empty SVLDB object.
 *  @par
 *  @subsection ssecMW_SVL_BLDR_2_3  2.3. TSL and SVL record
 *  @par
 *      The relation of SVL record and TSL record is many-to-one, as illustrated in following figure. If TSL record broadcast type is analog, there is one-to-one SVL record. If the broadcast type of TSL record is digital, there may be some SVL records relative to it. The fields, ­rui2_tsl_id­_ and ­rui2_tsl_rec_id­_, in the structure SVL_REC_HEADER_T will point to a TSL record.
 *  @par
 *      @image html sb_tsl_svl_record.gif "TSL and SVL record"
 *  @par
 *  @subsection ssecMW_SVL_BLDR_2_4  2.4. Auto Scan
 *  @par
 *      Auto Scan will scan all channels of ATV and DTV. we also call it full mode scanning. The data of SVL and TSL will be cleaned before starting to scan. If the TV is carried from one city to other city, you had better do auto scan at first.
 *  @par
 *  @subsection ssecMW_SVL_BLDR_2_5  2.5. Update Scan
 *  @par
 *      Update Scan will scan all the channels, but the channels on which captured the programs previous scanning will be skipped. If there are some new channels, you could do update scan to save time.
 *  @par
 *  @subsection ssecMW_SVL_BLDR_2_6  2.6. Range Scan
 *  @par
 *      Range Scan will scan the channels which are between the range indicated by user.
 *  @par
 *  @subsection ssecMW_SVL_BLDR_2_7  2.7. Single RF Scan
 *  @par
 *      Single RF scan is just for DTV. Obviously it only scan one channel indicatd by user.
 *  @par
 *  @section secMW_SVL_BLDR_3  3. Brief Design
 *  @par
 *  @subsection ssecMW_SVL_BLDR_3_1  3.1. Selector
 *  @par
 *      The selector passes the protocol-agnostic function calls to a proper builder, monitors the builder­_s internal states, and gets the requested information back to applications.
 *      The selector receives some messages and parses which builder engine task function to call. It can change thread context for other module.
 *  @par
 *  @subsection ssecMW_SVL_BLDR_3_2  3.2. Builder engine
 *  @par
 *      A builder engine should have the capability of interfacing with Table Manager, Connection Manager, and SVLDB.  By registration, a function table (see section 6.2.1) is duplicated in an engine control block.  All engine control blocks are maintained in a double-linked list. Such as DVB, ATSC, CQAM engine, they are divided by the DTV standard.
 *  @par
 *  @subsection ssecMW_SVL_BLDR_3_3  3.3. Notifications
 *  @par
 *      The callback function (*x_sb_scan_nfy_fct) is specified when calling the API x_sb_start_scan.  The suitable time to notify the application depends on the real implementation of builders.  For example, a builder may go through every record in the associated TSLDB object, where each record represents a PTC.  Every time a PTC record is processed (virtual channels inside it are recorded in SVLDB object), the scan progress notify function is called to notify the application of more detected channel(s).
 *  @par
 *  @section secMW_SVL_BLDR_4  4. Typical Case
 *  @par
 *      @dontinclude sb_sample.c
 *  @par
 *  @subsection ssecMW_SVL_BLDR_4_1  4.1. Open builder
 *  @par
 *      The following code is to open antenna digital builder an it takes the ATSC for example. If you want to open other builder, just replace the open data with corresponding data structure.
 *  @par
 *      @skip Open builder
 *      @until }
 *  @par
 *  @subsection ssecMW_SVL_BLDR_4_2  4.2. Close builder
 *  @par
 *      @skip Close builder
 *      @until }
 *  @par
 *  @subsection ssecMW_SVL_BLDR_4_3  4.3. Start scan
 *  @par
 *      The following sample code implements antenna digital channel scan function in menu.
 *  @par
 *  @subsubsection sssecMW_SVL_BLDR_4_3_1  4.3.1. Auto Scan
 *  @par
 *      @skip Scan notify
 *      @until }
 *  @par
 *      @skip Auto scan
 *      @until }
 *  @par
 *  @subsubsection sssecMW_SVL_BLDR_4_3_2  4.3.2. Update Scan
 *  @par
 *      @skip Update scan
 *      @until }
 *  @par
 *  @subsubsection sssecMW_SVL_BLDR_4_3_3  4.3.3. Range Scan
 *  @par
 *      @skip Range scan
 *      @until }
 *  @par
 *  @subsubsection sssecMW_SVL_BLDR_4_3_4  4.3.4. Single RF Scan
 *  @par
 *      @skip Single RF scan
 *      @until }
 *  @par
 *  @subsection ssecMW_SVL_BLDR_4_4  4.4. Cancel scan
 *  @par
 *      @skip Cancel scan
 *      @until }
 */
#endif /* _U_SB_H_ */

