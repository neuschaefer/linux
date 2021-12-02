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
 * $RCSfile: cb_table.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $SWAuthor: Yan Wang $
 *
 * Description:
 *         This file contains all Callback Tabel related implementations.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"

#include "mw_adpt_cb_table.h"
#include "mw_adpt_cb_if.h"

#include "include/cb_inc/cb_param.h"
#include "include/cb_inc/cb_data_rm.h"
#include "include/ioctl_inc/ioctl_param.h"
#include "ioctl/adpt_ioctl.h"
#include "mw_adpt_mt53xx_cb.h"

/* MHF Linux - yjg 9x mmp */
#include "res_mngr/drv/x_img_dec.h"
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define RM_DEBUG_INF(A) 

#ifndef PAGE_SIZE
#define PAGE_SIZE (4096)
#endif

#ifndef PAGE_OFFSET
#define PAGE_OFFSET  (0xc0000000)
#endif

#define RM_DEV "/dev/rmmgr"

/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/
static VOID *pv_kernel_param_addr;
static int fdmem;
extern INT32 adpt_ioctl(unsigned int cmd,unsigned long arg);

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
UINT32 ttx_mmap(UINT8 ui1_path, UINT32 ui4_kernel_addr, size_t size);

#define MW_ADPT_CB_SET_OUTPUT( pOut, i4Size )          \
do                                                                \
{                                                               \
    ( ( CB_GET_MWADPT_CALLBACK_T* )( ( UINT32 )( p_param )        \
        - sizeof( CB_GET_MWADPT_CALLBACK_T ) )                    \
        )->pv_Output = (void *)( pOut );                            \
        ( ( CB_GET_MWADPT_CALLBACK_T* )( ( UINT32 )( p_param ) \
            - sizeof( CB_GET_MWADPT_CALLBACK_T ) )                \
            )->i4_OutSize = (int)( i4Size );                     \
}while( 0 )

#define MAKE_NOTIFY(name,type)  \
INT32 name(VOID *p_param)       \
{                               \
    type *p;                    \
    p = (type *)p_param;        \
    p->pf_nfy(field);           \
    return 0;                   \
}

#define MAKE_NOTIFY_INT32(name,type)\
INT32 name(VOID *p_param)           \
{                                   \
    INT32 i4_ret;                   \
    type *p;                        \
    p = (type *)p_param;            \
    i4_ret =p->pf_nfy(field);       \
    return i4_ret;                  \
}

#define MAKE_NOTIFY_BOOL(name,type) \
BOOL name(VOID *p_param)           \
{                                   \
    BOOL b_ret;                     \
    type *p;                        \
    p = (type *)p_param;            \
    b_ret =p->pf_nfy(field);        \
    return b_ret;                    \
}

#define MAKE_NOTIFY_VOID(name,type) \
VOID name(VOID *p_param)            \
{                                   \
    type *p;                        \
    p = (type *)p_param;            \
    p->pf_nfy(field);               \
}

#define MAKE_NOTIFY_VOID_UINT32(name,type)  \
VOID name(VOID *p_param)                    \
{                                           \
    type *p;                                \
    p = (type *)(*(UINT32*)p_param);        \
    if (p->pf_nfy) p->pf_nfy(field);        \
}

/*-----------------------------------------------------------------------------*/
#define field           \
        p->h_obj,       \
        p->ui1_port,    \
        p->e_nfy_cond,  \
        p->pv_tag,      \
        p->ui4_data

MAKE_NOTIFY(_rm_cb_nfy,PARAM_PF_NFY)
#undef field

#define field               \
        p->e_type,          \
        p->ui2_id,          \
        p->pv_comp_sel_data,\
        p->pv_comp_data,    \
        p->z_comp_data_len, \
        p->ui2_num_active,  \
        p->ui2_num_wait,    \
        p->e_ctrl,          \
        p->b_would_conflict

MAKE_NOTIFY_INT32(_rm_cb_selection,PARAM_PF_SEL_NFY)
#undef field

#define field \
        p->ui2_dev_id

MAKE_NOTIFY_INT32(_rm_mc_nfy_add_dev_fct,PARAM_RM_MC_ADD_DEV)
#undef field

#define field \
        p->ui2_dev_id

MAKE_NOTIFY_VOID(_rm_mc_nfy_del_dev_fct,PARAM_RM_MC_DEL_DEV)
#undef field

#define field \
        p->ui2_dev_id

MAKE_NOTIFY_VOID(_rm_mc_nfy_rst_dev_fct,PARAM_RM_MC_RST_DEV)
#undef field

#define field           \
        p->h_obj,       \
        p->ui2_dev_id

MAKE_NOTIFY_INT32(_rm_mc_nfy_add_inst_fct,PARAM_RM_MC_ADD_INST)
#undef field

#define field           \
        p->h_obj,       \
        p->ui2_dev_id

MAKE_NOTIFY_VOID(_rm_mc_nfy_del_inst_fct,PARAM_RM_MC_DEL_INST)
#undef field

#define field               \
        p->h_obj,           \
        p->ui2_dev_id,      \
        p->pt_comp_id,      \
        p->e_set_type,      \
        p->ppv_set_info,    \
        p->pz_set_info_len, \
        p->pi4_return

MAKE_NOTIFY_BOOL(_rm_mc_nfy_set_fct,PARAM_RM_MC_SET)
#undef field

#if 0
#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_evt_code,\
        p->pv_data

MAKE_NOTIFY_VOID(_rm_irrc_nfy_fct,PARAM_IRRC_NFY_INFO_T)
#undef field
#else
VOID _rm_irrc_nfy_fct(VOID *p_param)
{
    PARAM_IRRC_NFY_INFO_T *p = (PARAM_IRRC_NFY_INFO_T *)p_param;

    p->pf_nfy(p->pv_nfy_tag,p->e_nfy_cond,p->ui4_evt_code,&p->t_data);
}
#endif

#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

MAKE_NOTIFY_VOID(_rm_avc_nfy_fct,AVC_NFY_PARAM)
#undef field

#if 0
#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

MAKE_NOTIFY_VOID(_rm_tv_dec_nfy_fct,TV_DEC_NFY_PARAM)
#undef field
#else
VOID _rm_tv_dec_nfy_fct(VOID *p_param)
{
    TV_DEC_NFY_PARAM *p = (TV_DEC_NFY_PARAM *)p_param;
    
    if (p->e_nfy_cond == TV_DEC_COND_RESOLUTION_CHG)
    {
        p->pf_nfy(p->pv_nfy_tag,p->e_nfy_cond,(UINT32)(&p->t_data_1),p->ui4_data_2);
    }
    else
    {
        p->pf_nfy(p->pv_nfy_tag,p->e_nfy_cond,p->ui4_data_1,p->ui4_data_2);
    }
}
#endif

VOID _rm_tv_dec_src_detect_nfy_fct(VOID *p_param)
{
    TV_DEC_SRC_DETECT_NFY_PARAM *p = (TV_DEC_SRC_DETECT_NFY_PARAM *)p_param;
    
    p->pf_nfy(p->pv_nfy_tag,p->e_nfy_cond,p->ui4_data_1,p->ui4_data_2);
}

#if 0
#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

MAKE_NOTIFY_VOID(_rm_tv_dec_ttx_nfy_fct, TV_DEC_TTX_NFY_PARAM)
#undef field
#else
VOID _rm_tv_dec_ttx_nfy_fct(VOID *p_param)
{
    TV_DEC_TTX_NFY_PARAM *p = (TV_DEC_TTX_NFY_PARAM *)p_param;

    /* special case to handle ttx memeory map to user space. */
    if (p->e_nfy_cond == TV_DEC_TTX_COND_MEM_MAP)
    {
        ttx_mmap (p->ui4_data_0, p->ui4_data_1, p->ui4_data_2);
        return;
    }
    
    if (p->e_nfy_cond== TV_DEC_TTX_COND_DATA_ARRIVAL)
    {
        TV_DEC_TTX_DATA_T t_ttx_data;
        t_ttx_data.t_data.ui2_num_pkt = p->t_data_1.t_data.ui2_num_pkt;
        t_ttx_data.t_data.at_pkts = p->t_data_1.t_data.at_pkts;
        p->pf_nfy(p->pv_nfy_tag, p->e_nfy_cond, &t_ttx_data);        
    }
    else
    {
        p->pf_nfy(p->pv_nfy_tag, p->e_nfy_cond, (VOID*)(p->ui4_data_1));
    }    
}
#endif

#if 0
#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

MAKE_NOTIFY_VOID(_rm_tv_dec_vps_data_nfy_fct, TV_DEC_VPS_DATA_NFY_PARAM )
#undef field
#else
VOID _rm_tv_dec_vps_data_nfy_fct(VOID *p_param)
{
    TV_DEC_VPS_DATA_NFY_PARAM *p = (TV_DEC_VPS_DATA_NFY_PARAM *)p_param;
    TV_DEC_VPS_DATA_T t_vps_data;
    t_vps_data.aui1_data = p->aui1_vps_data;
    p->pf_nfy(p->pv_nfy_tag, p->e_nfy_cond, &t_vps_data);
}
#endif

#if 0
#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

MAKE_NOTIFY_VOID(_rm_tv_dec_eu_wss_nfy_fct, TV_DEC_EU_WSS_NFY_PARAM)
#undef field
#else
VOID _rm_tv_dec_eu_wss_nfy_fct(VOID *p_param)
{
    TV_DEC_EU_WSS_NFY_PARAM *p = (TV_DEC_EU_WSS_NFY_PARAM *)p_param;

    TV_DEC_EU_WSS_DATA_T t_eu_wss_data;
    t_eu_wss_data.ui2_data = p->t_data_1.ui2_data;    
    p->pf_nfy(p->pv_nfy_tag, p->e_nfy_cond, &t_eu_wss_data);
}
#endif

#if 0 /* For fix in_interrupt case */
#define field           \
        p->pv_tag,      \
        p->e_cc_cond,   \
        p->pv_arg
        
MAKE_NOTIFY_VOID(_rm_cc_nfy,CC_NFY_PARAM)
#undef field
#else
VOID _rm_cc_nfy(VOID *p_param)
{
    CC_NFY_PARAM *p = (CC_NFY_PARAM *)p_param;
    TV_DEC_CC_DATA_T t_tmp = {0};

    t_tmp.b_even_field = p->b_even_field;
    t_tmp.ui1_byte1 = p->ui1_byte1;
    t_tmp.ui1_byte2 = p->ui1_byte2;

    p->pf_nfy(p->pv_tag,p->e_cc_cond,&t_tmp);
}
#endif

#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

MAKE_NOTIFY_VOID(_rm_tv_src_avc_nfy,AVC_NFY_PARAM)
#undef field

#if 0
#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

MAKE_NOTIFY_VOID(_rm_aud_nfy,AUD_NFY_PARAM)
#undef field
#else
VOID _rm_aud_nfy(VOID *p_param)
{
    AUD_NFY_PARAM *p = (AUD_NFY_PARAM *)p_param;

    if (p->e_nfy_cond == AUD_DEC_COND_AUD_INFO_CHG)
    {
        p->pf_nfy(p->pv_nfy_tag,p->e_nfy_cond,(UINT32)(p->ai1_data_1),p->ui4_data_2);
    }
    else
    {
        p->pf_nfy(p->pv_nfy_tag,p->e_nfy_cond,p->ui4_data_1,p->ui4_data_2);
    }
}
#endif

#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

MAKE_NOTIFY_VOID(_rm_bagt_nfy,BAGT_NFY_PARAM)
#undef field

/* MHF - Linux Basara for mmp */
#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

MAKE_NOTIFY_VOID(_rm_aud_hp_nfy,AUD_NFY_HP_PARAM)
#undef field

#define field           \
        p->ui1_dec_id,  \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

MAKE_NOTIFY_VOID(_rm_aud_upload_nfy,AUD_NFY_YW_PARAM)
#undef field


#if 0
#ifdef MW_CLASSIC_DMX_NFY
#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

#else
#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2,  \
        p->ui8_ext_data_1, \
        p->ui8_ext_data_2, \
        p->ui4_ext_data_3
#endif

MAKE_NOTIFY_BOOL(_rm_dmx_mm_nfy,DMX_NFY_PARAM)
#undef field
#else
VOID _rm_dmx_mm_nfy(VOID *p_param)
{
    DMX_NFY_PARAM *p = (DMX_NFY_PARAM *)p_param;
    UINT32 ui4_tmp = 0;

    if (1 == p->ui4_is_address)
    {
        ui4_tmp = (UINT32)&p->t_data_1;
    }
    else
    {
        ui4_tmp = p->ui4_data_1;
    }

    p->pf_nfy(p->pv_nfy_tag,
              p->e_nfy_cond,
              ui4_tmp,
              p->ui4_data_2
            #ifndef MW_CLASSIC_DMX_NFY
              ,
              p->ui8_ext_data_1,
              p->ui8_ext_data_2,
              p->ui4_ext_data_3
            #endif
              );
            
}
#endif

#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

MAKE_NOTIFY_BOOL(_rm_mwdmx_nfy,MWDMX_NFY_PARAM)
#undef field

#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

MAKE_NOTIFY_VOID(_rm_vdp_pla_nfy,VDP_PLA_NFY_PARAM)
#undef field
/* MHF - Linux Basara end */

#if 0
#define field           \
        p->pt_nfy_data, \
        p->pv_nfy_tag,  \
        p->ui4_data

MAKE_NOTIFY_VOID(_rm_cec_nfy,CEC_NFY_PARAM)
#undef field
#else
VOID _rm_cec_nfy(VOID *p_param)
{
    CEC_NFY_PARAM *p;
    p = (CEC_NFY_PARAM *)p_param;
    p->pf_nfy(&p->t_nfy,p->pv_nfy_tag,p->ui4_data);
}


#endif
#ifdef MW_CLASSIC_DMX_NFY
#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2
#else
#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2,  \
        p->ui8_ext_data_1, \
        p->ui8_ext_data_2, \
        p->ui4_ext_data_3
#endif

MAKE_NOTIFY_VOID(_rm_dmx_nfy,DMX_NFY_PARAM)
#undef field

#ifdef MW_CLASSIC_DMX_NFY
#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2
#else
#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2,  \
        p->ui8_ext_data_1, \
        p->ui8_ext_data_2, \
        p->ui4_ext_data_3
#endif

MAKE_NOTIFY_VOID(_rm_dmx_set_nfy,DMX_SET_NFY_PARAM)
#undef field

#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

MAKE_NOTIFY_VOID(_rm_mpv_set_nfy,MPV_SET_NFY_PARAM)
#undef field

#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

MAKE_NOTIFY_VOID(_rm_mpv_frame_nfy,MPV_FRAME_NFY_PARAM)
#undef field

#define field           \
        p->pv_tag,      \
        p->e_cc_cond,   \
        p->pv_arg

MAKE_NOTIFY_VOID(_rm_mpv_dec_cc_nfy,MPV_DEC_CC_NFY_PARAM)
#undef field

#define field           \
        p->pv_nfy_tag,  \
        p->t_pts,       \
        p->b_out_of_sync

MAKE_NOTIFY_VOID(_rm_mpv_reach_fct_nfy,MPV_REACH_FCT_NFY_PARAM)
#undef field

#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data

MAKE_NOTIFY_VOID(_rm_pcd_nfy,PCD_NFY_PARAM)
#undef field

#define field               \
        &p->t_src_res,      \
        &p->t_disp_res,     \
        p->ui1_afd,         \
        &p->t_src_region,   \
        &p->t_disp_region

MAKE_NOTIFY_VOID_UINT32(_rm_vdp_nfy,VDP_NFY_PARAM)
#undef field

#if 1
VOID _rm_vdp_ex_nfy(VOID *p_param)
{                                 
    VDP_EX_NFY_PARAM *p;                     
//    VDP_NFY_OUT_PARAM tOutData;

    p = (VDP_EX_NFY_PARAM *)p_param;         
    p->pf_nfy(&p->t_src_res, &p->t_disp_res, &p->t_vid_src_info, &p->tOut.t_src_region, &p->tOut.t_disp_region
    #ifdef SYS_OVERRIDE_DISP_REGION_SUPPORT	
    ,&p->tOut.t_disp_reg_override
    #endif
    );

//    tOutData.t_disp_region = p->t_disp_region;
//    tOutData.t_src_region = p->t_src_region;
//    MW_ADPT_CB_SET_OUTPUT(&tOutData, sizeof(VDP_NFY_OUT_PARAM));      
    MW_ADPT_CB_SET_OUTPUT(&(p->tOut), sizeof(VDP_NFY_OUT_PARAM));      
}
#else
#define field               \
        &p->t_src_res,      \
        &p->t_disp_res,     \
        &p->t_vid_src_info, \
        &p->t_src_region,   \
        &p->t_disp_region

MAKE_NOTIFY_VOID_UINT32(_rm_vdp_ex_nfy,VDP_EX_NFY_PARAM)
#undef field
#endif

#if 1
VOID _rm_vdp_get_cur_disp_fmt(VOID *p_param)
{                                 
    PARMA_GET_CUR_DISP_FMT *p;
    p = (PARMA_GET_CUR_DISP_FMT *)p_param;
    p->pf_nfy(&p->t_src_res, &p->t_disp_res, &p->t_vid_src_info, &p->e_disp_fmt);
    MW_ADPT_CB_SET_OUTPUT(&(p->e_disp_fmt), (sizeof(VID_PLA_DISP_FMT_T)));
}
#else
#define field               \
        &p->t_src_res,      \
        &p->t_disp_res,     \
        &p->t_vid_src_info, \
        &p->e_disp_fmt

MAKE_NOTIFY_VOID_UINT32(_rm_vdp_get_cur_disp_fmt, PARMA_GET_CUR_DISP_FMT)
#undef field
#endif

#if 1
VOID _rm_vdp_get_cur_src_region(VOID *p_param)
{                                 
    PARMA_GET_CUR_SRC_REGION *p;
    p = (PARMA_GET_CUR_SRC_REGION *)p_param;
    p->pf_nfy(p->ui2_vdp_id, p->ui4X, p->ui4Y, p->ui4Width, p->ui4Height);
 //   MW_ADPT_CB_SET_OUTPUT(&(p->e_disp_fmt), (sizeof(VID_PLA_DISP_FMT_T)));
}
#else
#define field               \
        &p->ui2_vdp_id_,      \
        &p->ui4X,     \
        &p->ui4Y, \
        &p->ui4Width,
        &p->ui4Height

//MAKE_NOTIFY_VOID_UINT32(_rm_vdp_get_cur_src_region, PARMA_GET_CUR_SRC_REGION)
#undef field
#endif
#if 1
VOID _rm_vdp_get_cur_out_region(VOID *p_param)
{                                 
    PARMA_GET_CUR_OUT_REGION *p;
    p = (PARMA_GET_CUR_OUT_REGION *)p_param;
    p->pf_nfy(p->ui2_vdp_id, p->ui4X, p->ui4Y, p->ui4Width, p->ui4Height);
 //   MW_ADPT_CB_SET_OUTPUT(&(p->e_disp_fmt), (sizeof(VID_PLA_DISP_FMT_T)));
}
#else
#define field               \
        &p->ui2_vdp_id_,      \
        &p->ui4X,     \
        &p->ui4Y, \
        &p->ui4Width,
        &p->ui4Height

//MAKE_NOTIFY_VOID_UINT32(_rm_vdp_get_cur_src_region, PARMA_GET_CUR_OUT_REGION)
#undef field
#endif

#if 1
VOID _rm_vdp_overscan(VOID *p_param)
{                                 
    PARAM_VDP_OVERSCAN *p;                     
    //VDP_OVERSCAN_OUT_PARAM tOutData;

    p = (PARAM_VDP_OVERSCAN *)p_param;         
    p->pf_nfy(p->ui2_vdp_id, p->e_src_type, &p->t_src_timing, &p->tOut.ui2_up, &p->tOut.ui2_down, &p->tOut.ui2_left, &p->tOut.ui2_right);            

 //   tOutData.ui2_up = p->ui2_up;
//    tOutData.ui2_down = p->ui2_down;
//    tOutData.ui2_left = p->ui2_left;
//    tOutData.ui2_right = p->ui2_right;    
    MW_ADPT_CB_SET_OUTPUT(&(p->tOut), sizeof(VDP_OVERSCAN_OUT_PARAM));      
}
#else
#define field               \
        p->ui2_vdp_id,      \
        p->e_src_type,      \
        &p->t_src_timing,   \
        &p->ui2_up,         \
        &p->ui2_down,       \
        &p->ui2_left,       \
        &p->ui2_right

MAKE_NOTIFY_VOID_UINT32(_rm_vdp_overscan, PARAM_VDP_OVERSCAN)
#undef field
#endif

#if 1
VOID _rm_vdp_update_overscan(VOID *p_param)
{                                 
    PARAM_UPDATE_OVERSCAN *p;                     

    p = (PARAM_UPDATE_OVERSCAN *)p_param;         
    p->pf_nfy(p->ui2_vdp_id, p->e_src_type, &p->t_src_timing, p->ui2_up, p->ui2_down, p->ui2_left, p->ui2_right);            
}
#else
#define field               \
        p->ui2_vdp_id,      \
        p->e_src_type,      \
        &p->t_src_timing,   \
        p->ui2_up,          \
        p->ui2_down,        \
        p->ui2_left,        \
        p->ui2_right

MAKE_NOTIFY_VOID_UINT32(_rm_vdp_update_overscan, PARAM_UPDATE_OVERSCAN)
#undef field
#endif

#if 0
#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1,  \
        p->ui4_data_2

MAKE_NOTIFY_VOID(_rm_pcmcia_interrupt_nfy_fct, PCMCIA_INTERRUPT_NFY_PARAM)
#undef field
#else
VOID _rm_pcmcia_interrupt_nfy_fct(VOID *p_param)
{
    PCMCIA_INTERRUPT_NFY_PARAM *p = (PCMCIA_INTERRUPT_NFY_PARAM *)p_param;
    p->pf_nfy(p->ui2_data);
}
#endif

#ifdef TIME_SHIFT_SUPPORT
#if 0
#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->u.ui4_data_1
MAKE_NOTIFY_BOOL(_rm_pvr_nfy, PVR_SET_NFY_PARAM)
#undef field
#endif
INT32 _rm_pvr_nfy(VOID *p_param)          
{                                  
    INT32                    i4_ret;                     
    PVR_SET_NFY_PARAM       *p;                        
    p = ( PVR_SET_NFY_PARAM* ) p_param; 
    
    if (PVR_COND_TICK == p->e_nfy_cond)
    {
        i4_ret = p->pf_nfy(p->pv_nfy_tag, p->e_nfy_cond, (UINT32)(&(p->u.rTickData)));
    }
    else
    {
        i4_ret = p->pf_nfy(p->pv_nfy_tag, p->e_nfy_cond, p->u.ui4_data_1);
    }
    return i4_ret;
            
}

#define field           \
        p->pv_nfy_tag,  \
        p->e_nfy_cond,  \
        p->ui4_data_1
MAKE_NOTIFY_BOOL(_rm_pvr_scr_nfy, PVR_SCR_SET_NFY_PARAM)
#undef field
#endif  // TIME_SHIFT_SUPPORT

INT32 rm_user_callback(UINT32 sub_id)
{
    switch (sub_id)
    {
    case 0:
        return _rm_cb_selection(pv_kernel_param_addr);
        break;
    case 1:
        _rm_cb_nfy(pv_kernel_param_addr);
        break;
    }
    return 0;
}


#if 1 /* MHF Linux - yjg 9x mmp */

VOID _rm_jpg_nfy(VOID * p_param)
{
    JPG_IMG_SET_NFY_PARM *p_jpg;
    /*printf("OOOOOOOOO%s,#%d\n", __FUNCTION__, __LINE__);*/

    p_jpg = (JPG_IMG_SET_NFY_PARM *)p_param;

    if(0 /*JPG_HW_NFY_FILL_BUF*/ == p_jpg->e_state)
    {
#if 0 /* Fix the overlap data bug. */
        IMG_FILL_BUF_MW_T *prFillBuf;

        int ret;

        prFillBuf = (IMG_FILL_BUF_MW_T *)p_jpg->pv_data;

        p_jpg->pf_nfy(p_jpg->ui4_img_id, p_jpg->pv_tag, prFillBuf, p_jpg->e_state);
#else
        p_jpg->pf_nfy(p_jpg->ui4_img_id, p_jpg->pv_tag, &p_jpg->t_attach_buf, p_jpg->e_state);
#endif
    }
    else
    {
        /*printf("\n -- user : _rm_jpg_nfy 0 : p_jpg = %lx-- \n",p_jpg);*/
        /*printf("\n -- user : _rm_jpg_nfy 1 : p_jpg->pf_nfy = %lx-- \n",p_jpg->pf_nfy);*/
        p_jpg->pf_nfy(p_jpg->ui4_img_id, p_jpg->pv_tag, p_jpg->pv_data, p_jpg->e_state);
        /*printf("\n -- user : _rm_jpg_nfy 2-- \n");*/
    }
}
#endif

/* MHF Linux - Basara */
#if 1 /* MHF Linux - Basara for mmp temp*/
extern BOOL is_fbm_usr_mem(INT32 usr_address);
#endif 
INT32 _rm_bagt_mmap_nfy(VOID *p_param)
{
    BAGT_NFY_PARAM *p_bagt_param;
    BUF_AGT_DATA_BLK_T *prDataBlock;
    UINT32 phy_addr;
    UINT32 ui4_offset = 0;
    UINT8 *pui1_base = NULL;
    INT32  i4_ret;

    p_bagt_param = 
        (BAGT_NFY_PARAM *)p_param;
#if 0 /* MHF Linux - Basara for mmp temp*/

    prDataBlock = (BUF_AGT_DATA_BLK_T *)p_bagt_param->ui4_data_1;
    ui4_offset = (((UINT32)prDataBlock->pui1_buff) % 4096);

    phy_addr = ((UINT32)prDataBlock->pui1_buff) - ui4_offset  - PAGE_OFFSET;
    pui1_base = mmap(0,  prDataBlock->z_size + ui4_offset, PROT_READ|PROT_WRITE, MAP_SHARED, fdmem, phy_addr);
    prDataBlock->pui1_buff = pui1_base + ui4_offset;

    i4_ret = (INT32)p_bagt_param->pf_nfy(p_bagt_param->pv_nfy_tag, p_bagt_param->e_nfy_cond,
                         prDataBlock, p_bagt_param->ui4_data_2);
        

#else

#if 0 /* For fix overlap data */
    prDataBlock = (BUF_AGT_DATA_BLK_T *)p_bagt_param->ui4_data_1;
#else
    prDataBlock = &p_bagt_param->t_data_1;
#endif

    if(is_fbm_usr_mem((INT32)(prDataBlock->pui1_buff)))
    {
        /* x_dbg_stmt("[FBM MEM]prDataBlock->pui1_buff[0x%x]\r\n",prDataBlock->pui1_buff);*/
#if 0 /* For fix overlap data */
        i4_ret = (INT32)p_bagt_param->pf_nfy(p_bagt_param->pv_nfy_tag, p_bagt_param->e_nfy_cond,
                         p_bagt_param->ui4_data_1, p_bagt_param->ui4_data_2);
#else
        i4_ret = (INT32)p_bagt_param->pf_nfy(p_bagt_param->pv_nfy_tag, p_bagt_param->e_nfy_cond,
                         (UINT32)prDataBlock, p_bagt_param->ui4_data_2);
#endif
    }
    else
    {

#if 1 /* MHF - Linux Basara for mmp Use Double mmap buffer for mp3. */
        static UINT8 *pui1_buff_1 = 0;
        static UINT8 *pui1_buff_2 = 0;
        UINT8 *pui1_buff_temp = 0;
        static UINT8 *pui1_buff_mmap_1 = 0;
        static UINT8 *pui1_buff_mmap_2 = 0;
        static UINT32 ui4_offset_1 = 0;
        static UINT32 ui4_offset_2 = 0;
        static SIZE_T z_size_1 = 0;
        static SIZE_T z_size_2 = 0;
        static UINT32 ui4_count = 0;
        
        /*x_dbg_stmt("[NO FBM MEM]prDataBlock->pui1_buff[0x%x]\r\n",prDataBlock->pui1_buff);*/

        if(ui4_count >= 2)
        {
            if(prDataBlock->pui1_buff == pui1_buff_1)
            {
                pui1_buff_temp = pui1_buff_mmap_1;
            }
            else if(prDataBlock->pui1_buff == pui1_buff_2)
            {
                pui1_buff_temp = pui1_buff_mmap_2;
            }

            if(pui1_buff_temp == 0)
            {
                if(pui1_buff_mmap_1 != 0)
                {
                    munmap(pui1_buff_mmap_1 - ui4_offset_1,z_size_1 + ui4_offset_1);
                    pui1_buff_mmap_1 = 0;
                    ui4_offset_1 = 0;
                    z_size_1 = 0;
                }
                if(pui1_buff_mmap_2 != 0)
                {
                    munmap(pui1_buff_mmap_2 - ui4_offset_2,z_size_2 + ui4_offset_2);
                    pui1_buff_mmap_2 = 0;
                    ui4_offset_2 = 0;
                    z_size_2 = 0;
                }
                
                ui4_count = 1;
                pui1_buff_1 = prDataBlock->pui1_buff;
                
                ui4_offset = (((UINT32)prDataBlock->pui1_buff) % 4096);
                phy_addr = ((UINT32)prDataBlock->pui1_buff) - ui4_offset  - PAGE_OFFSET;
                pui1_base = mmap(0,  prDataBlock->z_size + ui4_offset, PROT_READ|PROT_WRITE, MAP_SHARED, fdmem, phy_addr);

                pui1_buff_mmap_1 = pui1_base + ui4_offset;
                ui4_offset_1 = ui4_offset;
                z_size_1 = prDataBlock->z_size;
                pui1_buff_temp = pui1_buff_mmap_1;
            }
        }
        else if(ui4_count == 1)
        {
            ui4_count = 2;
            pui1_buff_2 = prDataBlock->pui1_buff;
            
            ui4_offset = (((UINT32)prDataBlock->pui1_buff) % 4096);
            phy_addr = ((UINT32)prDataBlock->pui1_buff) - ui4_offset  - PAGE_OFFSET;
            pui1_base = mmap(0,  prDataBlock->z_size + ui4_offset, PROT_READ|PROT_WRITE, MAP_SHARED, fdmem, phy_addr);

            pui1_buff_mmap_2 = pui1_base + ui4_offset;
            ui4_offset_2 = ui4_offset;
            z_size_2 = prDataBlock->z_size;
            pui1_buff_temp = pui1_buff_mmap_2;
        }
        else /* ui4_count == 0 */
        {
            ui4_count = 1;
            pui1_buff_1 = prDataBlock->pui1_buff;
            
            ui4_offset = (((UINT32)prDataBlock->pui1_buff) % 4096);
            phy_addr = ((UINT32)prDataBlock->pui1_buff) - ui4_offset  - PAGE_OFFSET;
            pui1_base = mmap(0,  prDataBlock->z_size + ui4_offset, PROT_READ|PROT_WRITE, MAP_SHARED, fdmem, phy_addr);

            pui1_buff_mmap_1 = pui1_base + ui4_offset;
            ui4_offset_1 = ui4_offset;
            z_size_1 = prDataBlock->z_size;
            pui1_buff_temp = pui1_buff_mmap_1;
        }

        prDataBlock->pui1_buff = pui1_buff_temp;

        i4_ret = (INT32)p_bagt_param->pf_nfy(p_bagt_param->pv_nfy_tag, p_bagt_param->e_nfy_cond,
                             (UINT32)prDataBlock, p_bagt_param->ui4_data_2);

#else

        x_dbg_stmt("[NO FBM MEM]prDataBlock->pui1_buff[0x%x]\r\n",prDataBlock->pui1_buff);
        
        ui4_offset = (((UINT32)prDataBlock->pui1_buff) % 4096);

        phy_addr = ((UINT32)prDataBlock->pui1_buff) - ui4_offset  - PAGE_OFFSET;
        pui1_base = mmap(0,  prDataBlock->z_size + ui4_offset, PROT_READ|PROT_WRITE, MAP_SHARED, fdmem, phy_addr);
        prDataBlock->pui1_buff = pui1_base + ui4_offset;

        i4_ret = (INT32)p_bagt_param->pf_nfy(p_bagt_param->pv_nfy_tag, p_bagt_param->e_nfy_cond,
                             prDataBlock, p_bagt_param->ui4_data_2);
#endif
    }
#endif

    return i4_ret;
}

UINT32 ttx_mmap(UINT8 ui1_path, UINT32 ui4_kernel_addr, size_t size)
{
    UINT32 ui4_phy_addr;
    UINT32 ui4_user_virt_addr;
    UINT32 ui4_offset;
    INT32 i4_fdioctrl;
    TV_DEC_TTX_NFY_MAP_ADDR_PARAM t_map_data;
    
    ui4_offset= (((UINT32)ui4_kernel_addr) % PAGE_SIZE);    
    ui4_phy_addr = ((UINT32)ui4_kernel_addr) - PAGE_OFFSET;
    ui4_user_virt_addr = (UINT32)mmap(0,  size + ui4_offset, PROT_READ|PROT_WRITE, MAP_SHARED, fdmem, ui4_phy_addr);

    i4_fdioctrl=open(RM_DEV, O_RDWR);
    if (i4_fdioctrl == -1)    
    {
        /*x_dbg_stmt("ttx_mmap fail\n");*/
        return 0;
    }

    t_map_data.ui1_path = ui1_path;
    t_map_data.ui4_ttx_ker_virt_start_addr = ui4_kernel_addr;
    t_map_data.ui4_ttx_usr_virt_start_addr = ui4_user_virt_addr;
    ioctl(i4_fdioctrl,IOCTL_TTX_MAP_ADDR,&t_map_data);
    close(i4_fdioctrl);
    return ui4_user_virt_addr;
}

VOID rm_reg_cb_table(VOID)
{
    CB_RegMwAdptCbFunc(CB_RM_CALLBACK, 0, (CB_FCT)_rm_cb_nfy);
    CB_RegMwAdptCbFunc(CB_RM_SELECTION, 0, (CB_FCT)_rm_cb_selection);

    CB_RegMwAdptCbFunc(CB_RM_MC_ADD_DEV, 0, (CB_FCT)_rm_mc_nfy_add_dev_fct);
    CB_RegMwAdptCbFunc(CB_RM_MC_DEL_DEV, 0, (CB_FCT)_rm_mc_nfy_del_dev_fct);
    CB_RegMwAdptCbFunc(CB_RM_MC_RST_DEV, 0, (CB_FCT)_rm_mc_nfy_rst_dev_fct);
    CB_RegMwAdptCbFunc(CB_RM_MC_ADD_INST, 0, (CB_FCT)_rm_mc_nfy_add_inst_fct);
    CB_RegMwAdptCbFunc(CB_RM_MC_DEL_INST, 0, (CB_FCT)_rm_mc_nfy_del_inst_fct);
    CB_RegMwAdptCbFunc(CB_RM_MC_SET, 0, (CB_FCT)_rm_mc_nfy_set_fct);
    CB_RegMwAdptCbFunc(CB_DRV_IRRX, 0, (CB_FCT)_rm_irrc_nfy_fct);
    CB_RegMwAdptCbFunc(CB_DRV_AVC_NFY, 0, (CB_FCT)_rm_avc_nfy_fct);
    CB_RegMwAdptCbFunc(CB_DRV_TV_DEC_NFY, 0, (CB_FCT)_rm_tv_dec_nfy_fct);
    CB_RegMwAdptCbFunc(CB_DRV_TV_DEC_SRC_DETECT_NFY, 0, (CB_FCT)_rm_tv_dec_src_detect_nfy_fct);
    CB_RegMwAdptCbFunc(CB_DRV_TV_DEC_TTX_NFY, 0, (CB_FCT)_rm_tv_dec_ttx_nfy_fct);
    CB_RegMwAdptCbFunc(CB_DRV_TV_DEC_VPS_DATA_NFY, 0, (CB_FCT)_rm_tv_dec_vps_data_nfy_fct);
    CB_RegMwAdptCbFunc(CB_DRV_TV_DEC_EU_WSS_NFY, 0, (CB_FCT)_rm_tv_dec_eu_wss_nfy_fct);
    CB_RegMwAdptCbFunc(CB_DRV_TV_SRC_AVC_NFY, 0, (CB_FCT)_rm_tv_src_avc_nfy);

    /* MHF Linux - yjg */
    CB_RegMwAdptCbFunc(CB_DRV_AUD_NFY, 0, (CB_FCT)_rm_aud_nfy);
    CB_RegMwAdptCbFunc(CB_DRV_BAGT_NFY, 0, (CB_FCT)_rm_bagt_nfy);
    CB_RegMwAdptCbFunc(CB_DRV_CEC_NFY, 0, (CB_FCT)_rm_cec_nfy);
    CB_RegMwAdptCbFunc(CB_DRV_DMX_NFY, 0, (CB_FCT)_rm_dmx_nfy);
    CB_RegMwAdptCbFunc(CB_DRV_DMX_SET_NFY, 0, (CB_FCT)_rm_dmx_set_nfy);
    CB_RegMwAdptCbFunc(CB_DRV_MPV_SET_NFY, 0, (CB_FCT)_rm_mpv_set_nfy);
    CB_RegMwAdptCbFunc(CB_DRV_MPV_FRAME_NFY, 0, (CB_FCT)_rm_mpv_frame_nfy);
    CB_RegMwAdptCbFunc(CB_DRV_MPV_DEC_CC_NFY, 0, (CB_FCT)_rm_mpv_dec_cc_nfy);
    CB_RegMwAdptCbFunc(CB_DRV_MPV_REACH_NFY, 0, (CB_FCT)_rm_mpv_reach_fct_nfy);

    CB_RegMwAdptCbFunc(CB_DRV_PCD_NFY, 0, (CB_FCT)_rm_pcd_nfy);
    CB_RegMwAdptCbFunc(CB_DRV_VDP_NFY, 0, (CB_FCT)_rm_vdp_nfy);
    CB_RegMwAdptCbFunc(CB_DRV_VDP_EX_NFY, 0, (CB_FCT)_rm_vdp_ex_nfy);
    CB_RegMwAdptCbFunc(CB_DRV_CC_NFY, 0, (CB_FCT)_rm_cc_nfy);

    CB_RegMwAdptCbFunc(CB_DRV_VDP_GET_CUR_DISP_FMT, 0, (CB_FCT)_rm_vdp_get_cur_disp_fmt);
    CB_RegMwAdptCbFunc(CB_DRV_VDP_OVERSCAN, 0, (CB_FCT)_rm_vdp_overscan);
    CB_RegMwAdptCbFunc(CB_DRV_VDP_UPDATE_OVERSCAN, 0, (CB_FCT)_rm_vdp_update_overscan);

    /* MHF Linux - yjg 9x mmp */
    fdmem = open("/dev/mem", O_RDWR);
    CB_RegMwAdptCbFunc(CB_DRV_JPG_SET_NFY, 0, (CB_FCT)_rm_jpg_nfy);
    CB_RegMwAdptCbFunc(CB_DRV_BAGT_MMAP_NFY, 0, (CB_FCT)_rm_bagt_mmap_nfy);/* MHF Linux - Basara */
    /* MHF Linux - Basara for mmp */
    CB_RegMwAdptCbFunc(CB_DRV_AUD_HP_NFY, 0, (CB_FCT)_rm_aud_hp_nfy);  
    CB_RegMwAdptCbFunc(CB_DRV_AUD_YW_NFY, 0, (CB_FCT)_rm_aud_upload_nfy);
    CB_RegMwAdptCbFunc(CB_DRV_DMX_MM_NFY, 0, (CB_FCT)_rm_dmx_mm_nfy); 
    CB_RegMwAdptCbFunc(CB_DRV_MWDMX_NFY, 0, (CB_FCT)_rm_mwdmx_nfy); 
    CB_RegMwAdptCbFunc(CB_DRV_VDP_PLA_NFY, 0, (CB_FCT)_rm_vdp_pla_nfy); 
    CB_RegMwAdptCbFunc(CB_DRV_PCMCIA_INTERRUPT_NFY, 0, (CB_FCT)_rm_pcmcia_interrupt_nfy_fct);
    /* MHF Linux - end */

#ifdef TIME_SHIFT_SUPPORT    
    CB_RegMwAdptCbFunc(CB_DRV_PVR_NFY, 0, (CB_FCT)_rm_pvr_nfy); 
    CB_RegMwAdptCbFunc(CB_DRV_PVR_SCR_NFY, 0, (CB_FCT)_rm_pvr_scr_nfy);
#endif // TIME_SHIFT_SUPPORT    
}

