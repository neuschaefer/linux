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
/*! @file u_psi_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains PSI engine specific definitions, which
 *         are exported.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_PSI_H_
#define _U_PSI_H_    /**<        */


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "u_handle_grp.h"
#include "u_common.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_TBL_MNGR_PSI_ENG  MPEG2 PSI Engine
 *
 *  @ingroup groupMW_TBL_MNGR
 *  @brief Load and get MPEG2 program information for Application
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could load or get
 *   the detail information of MPEG2 program information table.
 *   
 *  @see groupMW_TBL_MNGR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Default PSI engine name. */
#define PSI_DEFAULT_NAME  "psi_engine"    /**< Default name of PSI engine       */


/* PSI engine handle types. */
#define TMT_PSI_PAT  (TMT_PSI + ((HANDLE_TYPE_T)  0))    /**< Handle type for PAT */
#define TMT_PSI_PMT  (TMT_PSI + ((HANDLE_TYPE_T)  1))    /**< Handle type for PMT */
#define TMT_PSI_CAT  (TMT_PSI + ((HANDLE_TYPE_T)  2))    /**< Handle type for CAT */
#define TMT_PSI_TSDT (TMT_PSI + ((HANDLE_TYPE_T)  3))    /**< Handle type TSDT table */

#define TMT_PSI_PAT_MON  (TMT_PSI + ((HANDLE_TYPE_T)  4))    /**< Handle type for monitor PAT       */


/* PSI engine get operations. */
#define PSI_GET_TYPE_SVC_ENTRY     (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 0))    /**< get information, reference to PSI_PID_SVC_ID_INFO_T */
#define PSI_GET_TYPE_PCR_PID       (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 1))    /**< get information, reference to MPEG_2_PID_T */
#define PSI_GET_TYPE_STREAM_ENTRY  (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 2))    /**< get information, reference to PSI_STREAM_INFO_T */
#define PSI_GET_TYPE_PAT_ENTRY     (TM_GET_TYPE_LAST_ENTRY + ((TM_GET_TYPE_T) 3))    /**< get information, reference to PSI_PID_SVC_ID_INFO_T */

/* PSI engine load types. */
#define PSI_LOAD_TYPE_SVC_ID       (TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 0))    /**< Load type for service id */
#define PSI_LOAD_TYPE_PID_SVC_ID  ((TM_LOAD_TYPE_LAST_ENTRY + ((TM_LOAD_TYPE_T) 1)) | TM_LOAD_TYPE_SINGLE_SRC)    /**< Load type for PID, service id  */

/* PSI engine set operations. */
#define PSI_SET_TYPE_PCR_PID       (TM_SET_TYPE_LAST_ENTRY + ((TM_SET_TYPE_T) 0))    /**< Set type for PCR PID  */
#define PSI_SET_TYPE_STREAM_DESCR  (TM_SET_TYPE_LAST_ENTRY + ((TM_SET_TYPE_T) 1))    /**< Set type for stream description */

/* Holds a TS id and version value. */
/*------------------------------------------------------------------*/
/*! @struct PSI_TS_VER_INFO_T
 *  @brief This structure contains all information of a table's TS ID and version value.
 *  @code
 *  typedef struct _PSI_TS_VER_INFO_T
 *  {
 *      UINT16  ui2_ts_id;
 *      UINT8   ui1_version;
 *  }   PSI_TS_VER_INFO_T;
 *  @endcode
 *  @li@c  ui2_ts_id         - This field contains the table's TS ID value
 *  @li@c  ui1_version     - This field contains the table's version value
 */
/*------------------------------------------------------------------*/
typedef struct _PSI_TS_VER_INFO_T
{
    UINT16  ui2_ts_id;

    UINT8   ui1_version;
}   PSI_TS_VER_INFO_T;

/* Holds a PID and SVC id value. */
/*------------------------------------------------------------------*/
/*! @struct PSI_PID_SVC_ID_INFO_T
 *  @brief This structure contains information of PAT table's loop entry
 *  @code
 *  typedef struct _PSI_PID_SVC_ID_INFO_T
 *  {
 *      MPEG_2_PID_T  t_pid;
 *      UINT16  ui2_svc_id;
 *  }   PSI_PID_SVC_ID_INFO_T;
 *  @endcode
 *  @li@c  t_pid             - This field contains the table's PID value
 *  @li@c  ui2_svc_id     - This field contains the table's service ID value
 */
/*------------------------------------------------------------------*/
typedef struct _PSI_PID_SVC_ID_INFO_T
{
    MPEG_2_PID_T  t_pid;

    UINT16  ui2_svc_id;
}   PSI_PID_SVC_ID_INFO_T;

/* Holds a stream type and PID value. */
/*------------------------------------------------------------------*/
/*! @struct PSI_STREAM_INFO_T
 *  @brief This structure contains all information of a PMT table's stream loop entry
 *  @code
 *  typedef struct _PSI_STREAM_INFO_T
 *  {
 *      MPEG_2_PID_T  t_pid;
 *      UINT8  ui1_stream_type;
 *  }   PSI_STREAM_INFO_T;
 *  @endcode
 *  @li@c  t_pid                     - This field contains the PID value of a PMT table's stream loop entry
 *  @li@c  ui1_stream_type    - This field contains the stream type value of a PMT table's stream
 *                                           loop entry
 */
/*------------------------------------------------------------------*/
typedef struct _PSI_STREAM_INFO_T
{
    MPEG_2_PID_T  t_pid;

    UINT8  ui1_stream_type;
}   PSI_STREAM_INFO_T;

/* PSI engine configuration structure. */
/*------------------------------------------------------------------*/
/*! @struct PSI_CONFIG_T
 *  @brief This structure contains all information for the configuration of the PSI engine
 *  @code
 *  typedef struct _PSI_CONFIG_T
 *  {
 *      UINT8  ui1_num_pat_filters;
 *      UINT8  ui1_num_pmt_filters;
 *      UINT8  ui1_num_cat_filters;
 *      UINT8  ui1_num_tsdt_filters;
 *      SIZE_T  z_pat_buff_size;
 *      SIZE_T  z_pmt_buff_size;
 *      SIZE_T  z_cat_buff_size;
 *      SIZE_T  z_tsdt_buff_size;
 *      UINT16  ui2_num_pat_tbl_objs;
 *      UINT16  ui2_num_pmt_tbl_objs;
 *      UINT16  ui2_num_cat_tbl_objs;
 *      UINT16  ui2_num_tsdt_tbl_objs;
 *      UINT16  ui2_num_mon_pat_tbl_objs;
 *  }   PSI_CONFIG_T;
 *  @endcode
 *  @li@c  ui1_num_pat_filters               -Contains the number of PAT filters the PSI 
 *                                                         engine shall maintain. The value of this field 
 *                                                         must always be greater than '0'
 *  @li@c  ui1_num_pmt_filters              -Contains the number of PMT filters the PSI 
 *                                                         engine shall maintain. The value of this field 
 *                                                         must always be greater than '0'
 *  @li@c  ui1_num_cat_filters               - Contains the number of CAT filters 
 *                                                          the PSI engine shall maintain. The value of 
 *                                                          this field must always be greater than '0'
 *  @li@c  ui1_num_tsdt_filters              - Contains the number of TSDT filters the PSI 
 *                                                          engine shall maintain. The value of this field 
 *                                                          must always be greater than '0'
 *  @li@c  z_pat_buff_size                     - Contains the demux filter buffer size per PAT
 *                                                           filter in number of Bytes. The value of this field
 *                                                          must always be greater than '0'
 *  @li@c  z_pmt_buff_size                    - Contains the demux filter buffer size per PMT
 *                                                           filter in number of Bytes. The value of this field 
 *                                                           must always be greater than '0'
 *  @li@c  z_cat_buff_size                     - Contains the demux filter buffer size per CAT
 *                                                           filter in number of Bytes. The value of this field
 *                                                           must always be greater than '0'
 *  @li@c  z_tsdt_buff_size                    - Contains the demux filter buffer size per TSDT 
 *                                                          filter in number of Bytes. The value of this field
 *                                                          must always be greater than '0'
 *  @li@c  ui2_num_pat_tbl_objs           - Contains the number of PAT table objects
 *  @li@c  ui2_num_pmt_tbl_objs          - Contains the number of PMT table objects
 *  @li@c  ui2_num_cat_tbl_objs           - Contains the number of CAT table objects
 *  @li@c  ui2_num_tsdt_tbl_objs          - Contains the number of TSDT table objects
 *  @li@c  ui2_num_mon_pat_tbl_objs   - Contains the number of PAT monitor  table objects
 */
/*------------------------------------------------------------------*/
typedef struct _PSI_CONFIG_T
{
    UINT8  ui1_num_pat_filters;
    UINT8  ui1_num_pmt_filters;
    UINT8  ui1_num_cat_filters;
    UINT8  ui1_num_tsdt_filters;

    SIZE_T  z_pat_buff_size;
    SIZE_T  z_pmt_buff_size;
    SIZE_T  z_cat_buff_size;
    SIZE_T  z_tsdt_buff_size;

    UINT16  ui2_num_pat_tbl_objs;
    UINT16  ui2_num_pmt_tbl_objs;
    UINT16  ui2_num_cat_tbl_objs;
    UINT16  ui2_num_tsdt_tbl_objs;

    UINT16  ui2_num_mon_pat_tbl_objs;
}   PSI_CONFIG_T;

#endif /* _U_PSI_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                  /* end of groupMW_TBL_MNGR_PSI_ENG */
/*----------------------------------------------------------------------------*/
