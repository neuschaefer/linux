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
#include "x_common.h"
#include "rm.h"

#ifdef noinline
#undef noinline
#warning "somebody defined noinline"
#endif
#define ARM_ASM_CALL extern __attribute__((noinline))


#include "include/cb_inc/swi_cb_param.h"
extern  void*  ssma_alloc(size_t size);
extern void ssma_free(void * p);
extern INT32 printf( const CHAR *format, ... );

extern int rm_swi_nfy_dispatcher_sel(
        rm_comp_sel_fct pf_nfy,
        PARAM_PF_SEL_NFY * p_nfy_par)
{
    /*printf("rm swi nfy dispatcher, nfy is %x\n",pf_nfy );*/
    return pf_nfy(
            p_nfy_par->e_type,
            p_nfy_par->ui2_id,
            p_nfy_par->pv_comp_sel_data,
            p_nfy_par->pv_comp_data,
            p_nfy_par->z_comp_data_len,
            p_nfy_par->ui2_num_active,
            p_nfy_par->ui2_num_wait,
            p_nfy_par->e_ctrl,
            p_nfy_par->b_would_conflict);
}

ARM_ASM_CALL INT32 rm_swi_open_comp_swi_wrapper(volatile SWI_SESS_OPEN_COMP_T * p_sess)
{
    register INT32 i4_ret asm("r0");
    i4_ret = (INT32)p_sess;	
    __asm__("swi   2048" : "+r" (i4_ret));
    /*__asm__("bkpt 0");*/
    /*return 0;*/
    return i4_ret;
}

INT32 rm_swi_open_comp (
        HANDLE_T           h_obj,
        OBJ_TO_COMP_REL_T  e_obj_to_comp_rel,
        DRV_TYPE_T         e_type,
        UINT16             ui2_id,
        const CHAR*        ps_name,
        CTRL_TYPE_T        e_ctrl,
        VOID*              pv_comp_sel_data,
        rm_comp_sel_fct    pf_comp_sel,
        VOID*              pv_tag,
        rm_nfy_fct         pf_nfy,
        HANDLE_T*          ph_comp,
        RM_COND_T*         pe_cond)
{

    volatile SWI_SESS_OPEN_COMP_T * p_sess;
    volatile INT32 i4_ret;

    /*volatile VOID * i4_ret_addr  = &i4_ret;*/
     

    p_sess = ( SWI_SESS_OPEN_COMP_T * )ssma_alloc(sizeof(SWI_SESS_OPEN_COMP_T));
    if ( !p_sess )
    {
        return RMR_NOT_ENOUGH_SPACE;
    }

    /*printf("SWI TRACE U %s, #%d, %s, p_sess :%x\n",
           __FILE__, __LINE__, __FUNCTION__,
           p_sess);*/

    p_sess->par_open.h_obj             = h_obj;
    p_sess->par_open.e_obj_to_comp_rel = e_obj_to_comp_rel;
    p_sess->par_open.e_type            = e_type;
    p_sess->par_open.ui2_id            = ui2_id;
    p_sess->par_open.ps_name           = ps_name;
    p_sess->par_open.e_ctrl            = e_ctrl;
    p_sess->par_open.pv_comp_sel_data  = pv_comp_sel_data;
    p_sess->par_open.pf_comp_sel       = pf_comp_sel;
    p_sess->par_open.pv_tag            = pv_tag;
    p_sess->par_open.pf_nfy            = pf_nfy;
    p_sess->par_open.ph_comp           = ph_comp;
    p_sess->par_open.pe_cond           = pe_cond;

    
    i4_ret = rm_swi_open_comp_swi_wrapper(p_sess);

    
    ssma_free((VOID*)p_sess);

    return (i4_ret );
}

/* volatile extern void mtk_swi_cb_entry(void); */

ARM_ASM_CALL VOID mtk_swi_cb_entry_ret_wrapper(INT32 i4_ret)
{
    /* R0 from i4_ret is passed to swi handler */
    __asm__("swi 4224");
}

/* The user callback entry.               */
/* This will call user specified callback */
/* and then SWI to return to Kernel mode. */
INT32 mtk_swi_cb_entry(UINT32 u4_cb_number, VOID * pv_user_cb, VOID * pv_args)
{
    INT32 i4_ret;
    switch(u4_cb_number)
    {
        case 0:
            i4_ret = rm_swi_nfy_dispatcher_sel(pv_user_cb, pv_args);
            mtk_swi_cb_entry_ret_wrapper(i4_ret);
            /* No need break */
    }
    /* Assert here?                                   */
    /* Cannot dispatch the callback, something wrong? */
    /* For avoid system hang, back to Kernel          */
    i4_ret = 0;
    mtk_swi_cb_entry_ret_wrapper(i4_ret);   
    return 0;
}

ARM_ASM_CALL void register_cb_entery_wrapper(void * pv_entry)
{
    __asm__("swi 4096"); 
    /*__asm__("bkpt 0");*/
}
ARM_ASM_CALL void register_cb_entry(void)
{
   
   /*__asm__("ldr r0, =mtk_swi_cb_entry");
   
   __asm__("swi 4095");*/
   
   register_cb_entery_wrapper(mtk_swi_cb_entry);

   printf("Register the cb_entry 0x%08lx\n", (UINT32)mtk_swi_cb_entry);
  
}

void path_test()
{
    printf("This is just test PATH\n");
}

void path_test_arg2_block(int r0, int r1)
{
    printf("This is just test PATH2 :0x%x, 0x%x\n", r0, r1);
    while(1);
}

