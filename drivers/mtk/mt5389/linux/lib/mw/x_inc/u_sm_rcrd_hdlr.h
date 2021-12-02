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
/*! @file u_sm_rcrd_hdlr.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Stream Manager's record stream handler exported 
 *         definitions.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_STRM_MNGR_RCRD_STRM_HDLR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SM_RCRD_HDLR_H_
#define _U_SM_RCRD_HDLR_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/* command sets *************************************************************/
/* primary commands */
#define SM_RS_CMD_CODE_SET_RCRD_TYPE            \
      ((SM_CMD_CODE_T) (SM_CMD_CODE_PRIMARY_BIT_FLAG | 0))   /**< (RSH_RCRD_TYPE_T)         */
#define SM_RS_CMD_CODE_SET_PID                  \
      ((SM_CMD_CODE_T) (SM_CMD_CODE_PRIMARY_BIT_FLAG | 1))   /**<  (e_rcrd_type == (RSH_RCRD_TYPE_SECT || RSH_RCRD_TYPE_PCR)) => (MPEG_2_PID_T)        */
#define SM_RS_CMD_CODE_SET_REC_UTIL_NAME        \
      ((SM_CMD_CODE_T) (SM_CMD_CODE_PRIMARY_BIT_FLAG | 2))   /**< (e_rcrd_type == RSH_RCRD_TYPE_SECT) => (CHAR *)         */
#define SM_RS_CMD_CODE_SET_RCRD_INFO            \
      ((SM_CMD_CODE_T) (SM_CMD_CODE_PRIMARY_BIT_FLAG | 3))  /**<(e_rcrd_type == RSH_RCRD_TYPE_SECT) => (RSH_RCRD_INFO_SECT_T *)        */
#define SM_RS_CMD_CODE_SET_PRG_NUM              \
      ((SM_CMD_CODE_T) (SM_CMD_CODE_PRIMARY_BIT_FLAG | 4))  /**<(e_rcrd_type == RSH_RCRD_TYPE_SECT && e_tbl_grp == RSH_TBL_GRP_PSI_PAT) => (UINT16)         */
#define SM_RS_CMD_CODE_SET_STRM_COMP_ID              \
      ((SM_CMD_CODE_T) (SM_CMD_CODE_PRIMARY_BIT_FLAG | 5))  /**<ALL type => (STREAM_COMP_ID_T*)=>pv_data        */

typedef UINT8 RSH_RCRD_TYPE_T;    /**<record types        */

#define RSH_RCRD_TYPE_ES      ((RSH_RCRD_TYPE_T) 0)    /**< Elementary stream e.g. audio video subtitle.      */
#define RSH_RCRD_TYPE_SECT    ((RSH_RCRD_TYPE_T) 1)    /**<PAT , PMT  PSI stream     */
#define RSH_RCRD_TYPE_PCR     ((RSH_RCRD_TYPE_T) 2)    /**< PCR stream */


typedef UINT32 RSH_TBL_GRP_T;    /**<   table groups    */

#define RSH_TBL_GRP_PSI_PAT         ((RSH_TBL_GRP_T) 0)    /**< PSI PAT        */
#define RSH_TBL_GRP_PSI_PMT         ((RSH_TBL_GRP_T) 1)    /**< PSI PMT       */

/* set types *****************************************************************/
#define SM_GRP_RSH                      (( SM_GRP_LAST_ENTRY + 17 ) << SM_CODE_BITS)    /**< record stream handler cmd group       */
#define SM_GRP_RCRD_ES                  (( SM_GRP_LAST_ENTRY + 18 ) << SM_CODE_BITS)    /**<record ES stream cmd  group        */
#define SM_GRP_RCRD_SECT                (( SM_GRP_LAST_ENTRY + 19 ) << SM_CODE_BITS)    /**< record SECT stream  cmd  group       */

/* SM_GRP_RSH */
#define SM_RSH_SET_TYPE_IGNORE              ((SM_SET_TYPE_T)(SM_GRP_RSH | 0))    /**<  not to handle      */
#define SM_RSH_SET_TYPE_CTRL_OUTPUT         ((SM_SET_TYPE_T)(SM_GRP_RSH | 1))    /**<  turn on/off output of es     */

/* SM_GRP_RCRD_ES set types */
/*For record video stream I frame*/
#define SM_RSH_SET_TYPE_EN_PIC_SEARCH       ((SM_SET_TYPE_T)(SM_GRP_RCRD_ES | 0))    /**< picture search       */
#define SM_RSH_SET_TYPE_ES_ENCRYPTION       ((SM_SET_TYPE_T)(SM_GRP_RCRD_ES | 1))    /**<  es encryption     */

/* SM_GRP_RCRD_SECT set types */

/* (SM_RSM_PMT_ES_LIST_INFO_T *) */
#define SM_RSH_SET_TYPE_PMT_ES_LIST_INFO    ((SM_SET_TYPE_T)(SM_GRP_RCRD_SECT | 0))    /**< set es list to record       */


/* get types *****************************************************************/
/* SM_GRP_RSH get types */
#define SM_RSH_GET_TYPE_IGNORE              ((SM_GET_TYPE_T)(SM_GRP_RSH | 0))    /**<  not to handle      */
#define SM_RSH_GET_TYPE_RCRD_TYPE           ((SM_GET_TYPE_T)(SM_GRP_RSH | 1))    /**<  get record stream type      */

/* SM_GRP_RCRD_ES get types */

/* SM_GRP_RCRD_SECT get types */


/* event types *****************************************************************/
/* (e_rcrd_type == RSH_RCRD_TYPE_SECT && e_tbl_grp == RSH_TBL_GRP_PSI_PAT) */
#define SM_EVN_RSH_GRP_APP                SM_MAKE_EVN_GRP(SM_EVN_GRP_RSH, 0)      /**< record stream handler  event group     */

#define SM_EVN_RSH_APP_OPEN_REQ           ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 0)    /**< record stream open request       */
#define SM_EVN_RSH_APP_PLAY_REQ           ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 1)    /**<record  stream play request       */
#define SM_EVN_RSH_APP_STOP_REQ           ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 2)    /**<record stream stop request        */
#define SM_EVN_RSH_APP_CLOSE_REQ          ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 3)    /**<record  stream close request       */
#define SM_EVN_RSH_ALL_OPENED             ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 4)    /**<record stream opened event        */
#define SM_EVN_RSH_ALL_PLAYED             ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 5)    /**<record  stream played event          */
#define SM_EVN_RSH_ALL_STOPPED            ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 6)    /**<record  stream stopped event          */
#define SM_EVN_RSH_ALL_CLOSED             ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 7)    /**<record stream closed event           */
#define SM_EVN_RSH_COMP_ERR               ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 8)    /**<record  stream component error event          */
#define SM_EVN_RSH_SCDB_RECORD_DEL        ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 9)    /**<record  stream scdb delete  event           */
#define SM_EVN_RSH_SCDB_RECORD_MOD        ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 10)    /**<record  stream scdb modify  event          */
#define SM_EVN_RSH_SCDB_CLOSED            ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 11)    /**<record  stream scdb closed  event          */

#define SM_EVN_RSH_INTERNAL_ERR           ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 12)    /**< record  stream internal error  event         */
#define SM_EVN_RSH_PMT_PID_UPDATED        ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 13)    /**<record  stream  PID update event        */
#define SM_EVN_RSH_PAT_SWAPED             ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 14)    /**<set program pat PSI        */
#define SM_EVN_RSH_PMT_SWAPED             ((SM_EVN_TYPE_T) SM_EVN_GRP_RSH + 15)    /**<set program pmt PSI        */

#define SM_RSH_PMT_ES_LIST_INFO_FLAG_AVAIL_SCDB_INFO    MAKE_BIT_MASK_32(0)        /**<indicates es scdb exist       */




/*-----------------------------------------------------------------------------
                    structures
-----------------------------------------------------------------------------*/

/* record info when record type is RSH_RCRD_TYPE_ES */
/*------------------------------------------------------------------*/
/*! @struct RSH_RCRD_INFO_ES_T
 *  @brief  record info when record type is RSH_RCRD_TYPE_ES
 *  @code
* typedef struct _RSH_RCRD_INFO_ES_T
 *{
 *    UINT32          ui4_flags;                  
 *    UINT32          ui4_type_mask;       
 *} RSH_RCRD_INFO_ES_T;
 *  @endcode
 *  @li@c  ui4_flags                                        - reserved, set as 0
 *  @li@c  ui4_type_mask                              -mask for stream types (ST_MASK_*) 
 */
/*------------------------------------------------------------------*/
typedef struct _RSH_RCRD_INFO_ES_T
{
    UINT32          ui4_flags;          /* reserved, set as 0 */
    
    UINT32          ui4_type_mask;      /* mask for stream types (ST_MASK_*) */
        
} RSH_RCRD_INFO_ES_T;

/* record info when record type is RSH_RCRD_TYPE_SECT */
/*------------------------------------------------------------------*/
/*! @struct RSH_RCRD_INFO_SECT_T
 *  @brief Indicates the recorded stream is PAT or PMT
 *  @code
 *  typedef struct _RSH_RCRD_INFO_SECT_T
 *  {
 *      UINT32          ui4_flags;      
 *      RSH_TBL_GRP_T   e_tbl_grp;
 *  } RSH_RCRD_INFO_SECT_T;
 *  @endcode
 *  @li@c  ui4_flags                                        - reserved
 *  @li@c  e_tbl_grp                                        -indicates pat or pmt 
 */
/*------------------------------------------------------------------*/
typedef struct _RSH_RCRD_INFO_SECT_T
{
    UINT32          ui4_flags;      /* reserved, set as 0 */
    
    RSH_TBL_GRP_T   e_tbl_grp;
   
} RSH_RCRD_INFO_SECT_T;

/* SM_RSH_SET_TYPE_PMT_ES_LIST_INFO */
/*------------------------------------------------------------------*/
/*! @struct SM_RSM_PMT_ES_LIST_INFO_T
 *  @brief Descript All ES which should be recorded
 *  @code
 *  typedef struct _SM_RSM_PMT_ES_LIST_INFO_T
 *  {
 *      UINT16                  ui2_num_es;   
 *      const MPEG_2_PID_T*     at_pids_es;
 *      UINT16                  ui2_num_recs;
 *      const SCDB_REC_T*       at_scdb_recs;
 *  } SM_RSM_PMT_ES_LIST_INFO_T;
 *  @endcode
 *  @li@c  ui4_flags                                             -indicate which field is valid
 *  @li@c  ui2_num_es                                        -total stream number to record
 *  @li@c  at_pids_es                                          -total PIDs which need be recorded
 *  @li@c  ui2_num_recs                                     -total scdb number to record
 *  @li@c  at_scdb_recs                                      -total scdb info to record
 */
/*------------------------------------------------------------------*/
typedef struct _SM_RSH_PMT_ES_LIST_INFO_T
{
    UINT32                  ui4_flags;   
    UINT16                  ui2_num_es;   /* 0: the empty ES LIST, PMT stream handler should not output PMT in this case */
    
    const MPEG_2_PID_T*     at_pids_es;
    
    UINT16                  ui2_num_recs;
    
    const SCDB_REC_T*       at_scdb_recs;
    
} SM_RSH_PMT_ES_LIST_INFO_T;


#endif  /* _U_SM_RCRD_HDLR_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_STRM_MNGR_RCRD_STRM_HDLR */
/*----------------------------------------------------------------------------*/

