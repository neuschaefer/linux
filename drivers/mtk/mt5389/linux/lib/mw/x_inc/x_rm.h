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
 * $RCSfile: x_rm.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains Resource Manager specific definitions,
 *         which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_RM_H_
#define _X_RM_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include "x_common.h"
#include "x_sys_name.h"
#include "u_rm.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Max number of characters in a Resource Manager name */
/* (excluding the zero terminating character).         */
#define RM_NAME_LEN  SYS_NAME_LEN

/* Do not increase the number of ports past the value '32'. If that is  */
/* truly required, then one must also check the code to ensure that bit */
/* mask remain valid etc.                                               */
#define MAX_NUM_PORTS  ((UINT8)  32)
#define MAX_COMP_ID    ((UINT16) 0xffff)

#ifdef MW_RM_SET_DB

typedef enum 
{ 
DB_STATUS_AVAIL = 0, 
DB_STATUS_UNAVAIL_RD, 
DB_STATUS_UNAVAIL_WR 
} DB_STATUS_T; 

#endif


/* Selects the union member in DRV_COMP_REG_T */
typedef enum
{
    ID_TYPE_IND = 0,
    ID_TYPE_RANGE,
    ID_TYPE_LIST
}   ID_TYPE_T;

/* Individual id, tag and port value */
typedef struct _ID_IND_T
{
    UINT16  ui2_id;

    VOID*  pv_tag;

    UINT8  ui1_port;
}   ID_IND_T;

/* List of id, tag and port values */
typedef struct _ID_LIST_T
{
    ID_IND_T*  pt_list;

    UINT16  ui2_num_of_ids;
}   ID_LIST_T;

/* Range of id, tag and port values */
typedef struct _ID_RANGE_T
{
    UINT16  ui2_first_id;
    UINT16  ui2_delta_id;

    VOID*  pv_first_tag;
    VOID*  pv_delta_tag;

    UINT16  ui2_num_of_ids;

    UINT8  ui1_port;
}   ID_RANGE_T;


/* Driver component id used by the driver API's */
typedef struct _DRV_COMP_ID_T
{
    VOID*  pv_tag;

    DRV_TYPE_T  e_type;

    UINT16  ui2_id;

    BOOL  b_sel_out_port;

    union
    {
        UINT8  ui1_inp_port;
        UINT8  ui1_out_port;
    }   u;
}   DRV_COMP_ID_T;


/* Driver registration structure */
typedef struct _DRV_COMP_REG_T
{
    DRV_TYPE_T  e_type;

    ID_TYPE_T  e_id_type;

    union
    {
        ID_IND_T    t_ind;
        ID_RANGE_T  t_range;
        ID_LIST_T   t_list;
    }   u;
}   DRV_COMP_REG_T;


/* Connection list direction. */
typedef enum
{
    CONN_DIR_OUT_TO_INP = 0,
    CONN_DIR_INP_TO_OUT,
    CONN_DIR_TO_SUPPORT,
    CONN_DIR_FROM_SUPPORT
}   CONN_DIR_TYPE_T;


/* Callback condition */
typedef enum
{
    DRV_COND_DISCONNECTED = 0,
    DRV_COND_CONNECTED,
    DRV_COND_STATUS,
    DRV_COND_UPDATE
}   DRV_COND_T;

/* Connect reasons */
#define RM_CONN_REASON_AS_REQUESTED  ((UINT32)   0)

/* Disconnect reasons */
#define RM_DISC_REASON_AS_REQUESTED  ((UINT32)   0)

/* Driver callback function. */
typedef VOID (*x_rm_nfy_fct) (DRV_COMP_ID_T*  pt_comp_id,
                              DRV_COND_T      e_nfy_cond,
                              VOID*           pv_tag,
                              UINT32          ui4_data);

/* Connection types */
#define RM_CONN_TYPE_IGNORE   ((DRV_CONN_TYPE_T)   0)
#define RM_CONN_TYPE_RESTORE  ((DRV_CONN_TYPE_T)   1)
#define RM_CONN_TYPE_COMP_ID  ((DRV_CONN_TYPE_T)   2)

/* Driver control functions. */
typedef INT32 (*x_rm_connect_fct) (DRV_COMP_ID_T*   pt_comp_id,
                                   DRV_CONN_TYPE_T  e_conn_type,
                                   const VOID*      pv_conn_info,
                                   SIZE_T           z_conn_info_len,
                                   VOID*            pv_tag,
                                   x_rm_nfy_fct     pf_nfy);


/* Disconnect types */
#define RM_DISC_TYPE_IGNORE   ((DRV_DISC_TYPE_T)   0)
#define RM_DISC_TYPE_COMP_ID  ((DRV_DISC_TYPE_T)   2)

typedef INT32 (*x_rm_disconnect_fct) (DRV_COMP_ID_T*   pt_comp_id,
                                      DRV_DISC_TYPE_T  e_disc_type,
                                      const VOID*      pv_disc_info,
                                      SIZE_T           z_disc_info_len);

/* Get types */
#define RM_GET_TYPE_IGNORE       ((DRV_GET_TYPE_T)   0)
#define RM_GET_TYPE_ISR_CONTEXT  ((DRV_GET_TYPE_T) 0x00100000)

typedef INT32 (*x_rm_get_fct) (DRV_COMP_ID_T*  pt_comp_id,
                               DRV_GET_TYPE_T  e_get_type,
                               VOID*           pv_get_info,
                               SIZE_T*         pz_get_info_len);

/* Set types */
#define RM_SET_TYPE_IGNORE       ((DRV_SET_TYPE_T)   0)
#define RM_SET_TYPE_ISR_CONTEXT  ((DRV_SET_TYPE_T) 0x00100000)
#define RM_SET_TYPE_ARG_NO_REF   ((DRV_SET_TYPE_T) 0x00200000)
#define RM_SET_TYPE_ARG_CALLBACK ((DRV_SET_TYPE_T) 0x00400000)

typedef INT32 (*x_rm_set_fct) (DRV_COMP_ID_T*  pt_comp_id,
                               DRV_SET_TYPE_T  e_set_type,
                               const VOID*     pv_set_info,
                               SIZE_T          z_set_info_len);

/* Driver component exclusion notify function. */
typedef VOID (*x_rm_comp_excl_nfy_fct) (DRV_COMP_ID_T*  pt_pas_comp_id,
                                        DRV_COMP_ID_T*  pt_act_comp_id);


/* Driver function table */
typedef struct _DRV_COMP_FCT_TBL_T
{
    x_rm_connect_fct     pf_rm_connect;
    x_rm_disconnect_fct  pf_rm_disconnect;
    x_rm_get_fct         pf_rm_get;
    x_rm_set_fct         pf_rm_set;
}   DRV_COMP_FCT_TBL_T;

/* Component flags. */
#define DRV_FLAG_ASYNC_CONN_OR_DISC     ((UINT32) 0x00000001)
#define DRV_FLAG_SINGLE_CONN_ON_OUTPUT  ((UINT32) 0x00000002)

/* Resource Manager API return values */
#define RMR_DRV_INV_CONN_INFO          ((INT32) -256)
#define RMR_DRV_INV_DISC_INFO          ((INT32) -257)
#define RMR_DRV_INV_GET_INFO           ((INT32) -258)
#define RMR_DRV_INV_SET_INFO           ((INT32) -259)
#define RMR_DRV_CONN_FAILED            ((INT32) -260)
#define RMR_DRV_DISC_FAILED            ((INT32) -261)
#define RMR_DRV_SET_FAILED             ((INT32) -262)
#define RMR_DRV_GET_FAILED             ((INT32) -263)
#define RMR_DRV_NO_GET_INFO            ((INT32) -264)
#define RMR_DRV_NOT_ENOUGH_SPACE       ((INT32) -265)
#define RMR_DRV_WRITE_PROTECT          ((INT32) - 266)


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

#ifdef MW_RM_SET_DB

extern INT32 x_rm_set_db_status (DRV_TYPE_T e_type, 
                                           UINT16 ui2_id, 
                                           DB_STATUS_T e_db_status); 

extern INT32 x_rm_get_db_status (DRV_TYPE_T e_type, 
                                           UINT16 ui2_id, 
                                           DB_STATUS_T* pe_db_status); 

#endif

extern INT32 x_rm_get_comp_info_from_name (const CHAR*  ps_name,
                                           UINT16       ui2_idx,
                                           DRV_TYPE_T*  pe_type,
                                           UINT16*      pui2_id,
                                           UINT8*       pui1_num_inp_ports,
                                           UINT8*       pui1_num_out_ports,
                                           UINT32*      pui4_comp_flags);
extern INT32 x_rm_get_num_comps_of_type_from_name (const CHAR*  ps_name,
                                                   DRV_TYPE_T   e_type,
                                                   UINT16*      pui2_num_comps);
extern INT32 x_rm_reg_comp (DRV_COMP_REG_T*      pt_comp_id,
                            UINT8                ui1_num_inp_ports,
                            UINT8                ui1_num_out_ports,
                            const CHAR*          ps_comp_name,
                            UINT32               ui4_comp_flags,
                            DRV_COMP_FCT_TBL_T*  pt_comp_fct_tbl,
                            const VOID*          pv_comp_data,
                            SIZE_T               z_comp_data_len);
extern INT32 x_rm_reg_conn_list (const DRV_COMP_REG_T*  pt_comp_id,
                                 const DRV_COMP_REG_T*  pt_conn_ids,
                                 UINT16                 ui2_num_entries,
                                 CONN_DIR_TYPE_T        e_conn_dir,
                                 BOOL                   b_hard_wired);
extern INT32 x_rm_reg_comp_excl_list (const DRV_COMP_REG_T*  pt_comp_id,
                                      const DRV_COMP_REG_T*  pt_comp_excl_ids,
                                      UINT16                 ui2_num_entries);
extern INT32 x_rm_reg_comp_excl_nfy (const DRV_COMP_REG_T*   pt_comp_ids,
                                     UINT16                  ui2_num_entries,
                                     x_rm_comp_excl_nfy_fct  pf_rm_comp_excl_nfy);
extern INT32 x_rm_reg_conn_excl_list (const DRV_COMP_REG_T*  pt_comp_id,
                                      const DRV_COMP_REG_T*  pt_conn_excl_ids,
                                      UINT16                 ui2_num_entries);
extern INT32 x_rm_reg_group_name (const CHAR*            ps_group_name,
                                  const DRV_COMP_REG_T*  pt_comp_ids,
                                  UINT16                 ui2_num_entries);

extern INT32 x_rm_set_comp (DRV_TYPE_T      e_type,
                            UINT16          ui2_id,
                            BOOL            b_sel_out_port,
                            UINT8           ui1_port,
                            DRV_SET_TYPE_T  e_set_type,
                            const VOID*     pv_set_info,
                            SIZE_T          z_set_info_len);

extern INT32 x_rm_get_comp (DRV_TYPE_T      e_type,
                            UINT16          ui2_id,
                            BOOL            b_sel_out_port,
                            UINT8           ui1_port,
                            DRV_GET_TYPE_T  e_get_type,
                            VOID*           pv_get_info,
                            SIZE_T*         pz_get_info_len);

#endif /* _X_RM_H */
