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
 * $RCSfile: handle.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains handle specific definitions which are
 *         known to the whole Middleware.
 *---------------------------------------------------------------------------*/

#ifndef _HANDLE_H_
#define _HANDLE_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "x_handle.h"
#include "x_common.h"
#include "x_os.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Handle types which are recognized and treated in a special manner by the */
/* handle librray MUST fall between the values '1' and the value specified  */
/* by the macro HT_RES_BY_HANDLE_LIB.                                       */
#define HT_AUX_LINK_HEAD  ((HANDLE_TYPE_T)  1)

/* Return values of (*handle_parse_fct) */
typedef enum
{
    HP_NEXT,
    HP_BREAK,
    HP_RESTART
}   HP_TYPE_T;


/* Handle link structure */
typedef struct _HANDLE_LINK_T
{
    UINT32  ui4_data [2];
}   HANDLE_LINK_T;


/* Callback functions */
typedef BOOL (*handle_free_fct) (HANDLE_T       h_handle,
                                 HANDLE_TYPE_T  e_type,
                                 VOID*          pv_obj,
                                 VOID*          pv_tag,
                                 BOOL           b_req_handle);
typedef VOID (*handle_aux_free_fct) (VOID*  pv_obj);
typedef HP_TYPE_T (*handle_parse_fct) (UINT16         ui2_count,
                                       UINT16         ui2_max_count,
                                       HANDLE_T       h_handle,
                                       HANDLE_TYPE_T  e_type,
                                       VOID*          pv_obj,
                                       VOID*          pv_tag,
                                       VOID*          pv_parse_data);

typedef INT32 (*handle_autofree_fct) (HANDLE_T       h_handle,
                                      HANDLE_TYPE_T  e_type);


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

extern INT32 handle_alloc (HANDLE_TYPE_T    e_type,
                           VOID*            pv_obj,
                           VOID*            pv_tag,
                           handle_free_fct  pf_free,
                           HANDLE_T*        ph_handle);
extern INT32 handle_alloc_and_obj (HANDLE_TYPE_T    e_type,
                                   VOID*            pv_tag,
                                   SIZE_T           z_size,
                                   handle_free_fct  pf_free,
                                   HANDLE_T*        ph_handle,
                                   VOID**           ppv_obj);

extern INT32 handle_delink (HANDLE_LINK_T*  pt_h_link,
                            HANDLE_T        h_handle);
extern INT32 handle_free (HANDLE_T  h_handle,
                          BOOL      b_req_handle);
extern INT32 handle_free_all (HANDLE_LINK_T*  pt_h_link);
extern INT32 handle_get_obj (HANDLE_T  h_handle,
                             VOID**    ppv_obj);
extern INT32 handle_get_type_obj (HANDLE_T        h_handle,
                                  HANDLE_TYPE_T*  pe_type,
                                  VOID**          ppv_obj);
extern INT32 handle_init (UINT16   ui2_num_handles,
                          VOID**   ppv_mem_addr,
                          SIZE_T*  pz_mem_size);
extern INT32 handle_link (HANDLE_LINK_T*  pt_h_link,
                          HANDLE_T        h_handle);
extern INT32 handle_link_init (HANDLE_LINK_T*  pt_h_link);
extern BOOL handle_link_is_empty (HANDLE_LINK_T*  pt_h_link);
extern UINT16 handle_num_links (HANDLE_LINK_T*  pt_h_link);
extern INT32 handle_parse (HANDLE_LINK_T*    pt_h_link,
                           handle_parse_fct  pf_parse,
                           VOID*             pv_parse_data);
#if 0
extern INT32 handle_reg_cli (VOID);
#endif
extern INT32 handle_set_obj (HANDLE_T  h_handle,
                             VOID*     pv_obj);

/* Bug-14  2004-12-07  ffr. Add API to handle secondary linked handles. */
extern INT32 handle_alloc_aux_link_head (VOID*                pv_obj,
                                         handle_aux_free_fct  pf_aux_free,
                                         HANDLE_T*            ph_aux_head);
extern INT32 handle_link_to_aux (HANDLE_T  h_aux_head,
                                 HANDLE_T  h_handle);
extern INT32 handle_delink_from_aux (HANDLE_T  h_handle);
extern INT32 handle_next_aux_linked (HANDLE_T   h_curr_handle,
                                     HANDLE_T*  ph_next_handle);
extern UINT16 handle_num_aux_links (HANDLE_T  h_aux_head);

extern INT32 handle_set_autofree_tbl (HANDLE_TYPE_T         e_group,
                                      handle_autofree_fct*  pf_autofree_fcts);

extern VOID handle_autofree (HANDLE_T  h_handle);

#endif /* _HANDLE_H_ */
