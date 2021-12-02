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
#include "handle/handle.h"
#include "strm_mngr/scc/c_scc.h"
#include "strm_mngr/scc/x_scc.h"
#include "strm_mngr/u_sm.h"
#include "strm_mngr/cc_hdlr/u_sm_cc_hdlr.h"
#include "strm_mngr/sess_gnrc_hdlr/u_sm_sess_gnrc.h"
#include "rpc.h"
#include "ri_common.h"

#include "ri_desc.h"

#define RET_ON_FAIL(stmt) \
do {\
    INT32 __i4_ret;\
    __i4_ret = (stmt);\
    if(__i4_ret != RPCR_OK)\
    {   RPC_ERROR("RPC Failure %s, #%d\n", __FUNCTION__, __LINE__);\
        return __i4_ret;\
    }\
}while(0)

INT32 c_scc_comp_close( HANDLE_T               h_comp )
{
    RPC_DECL(1, INT32);        
    
    RPC_ARG_INP(ARG_TYPE_UINT32,   h_comp);

    RPC_DO_OP("x_scc_comp_close");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
	
}

static INT32 _set_cmd_array( SM_COMMAND_T *  pt_cmd_sets)
{
    UINT32              ui4_i;
    /*INT32                     i4_ret; */
    SM_COMMAND_T*       pt_iter;
    SCC_CMD_TYPE_T      e_cmd_type;
    
  
    if(pt_cmd_sets == NULL)
    {
        return RPCR_OK;
    }
    
    pt_iter = pt_cmd_sets;
    e_cmd_type = (SCC_CMD_TYPE_T)pt_iter->u.ui4_data;
    
    for (ui4_i = 0; pt_cmd_sets[ui4_i].e_code != SM_CMD_CODE_END; ui4_i ++)
    {
        if(pt_cmd_sets[ui4_i].e_code == SCC_CMD_CODE_NAME)
        {
            RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                             &pt_cmd_sets[ui4_i].u, 
                             RPC_DESC_SM_COMMAND_T_u, 
                             "u.ps_text"));    

            RET_ON_FAIL(RPC_ADD_REF_STR(RPC_DEFAULT_ID, pt_cmd_sets[ui4_i].u.ps_text));
        }
     }

    switch( e_cmd_type )
    {
    case SCC_CMD_TYPE_VIDEO:
         {
             for (ui4_i = 0; pt_cmd_sets[ui4_i].e_code != SM_CMD_CODE_END; ui4_i ++)
             {
                 switch(pt_cmd_sets[ui4_i].e_code)
                 {
                     case SCC_CMD_CODE_VIDEO_BACKGROUND:
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          &pt_cmd_sets[ui4_i].u, 
                                          RPC_DESC_SM_COMMAND_T_u, 
                                          "u.pv_data"));    
                                          
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          pt_cmd_sets[ui4_i].u.pv_data, 
                                          RPC_DESC_SCC_BG_COLOR_T, 
                                          NULL));
                     break;
                     case SCC_CMD_CODE_VIDEO_OVERSCAN_CLIPPER:
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          &pt_cmd_sets[ui4_i].u, 
                                          RPC_DESC_SM_COMMAND_T_u, 
                                          "u.pv_data"));
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          pt_cmd_sets[ui4_i].u.pv_data, 
                                          RPC_DESC_SCC_VID_OVER_SCAN_CLIPPER_T, 
                                          NULL));
                     break;
                     
                     case SCC_CMD_CODE_VIDEO_LB_DETECT_CONFIG:
                         
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          &pt_cmd_sets[ui4_i].u, 
                                          RPC_DESC_SM_COMMAND_T_u, 
                                          "u.pv_data"));
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          pt_cmd_sets[ui4_i].u.pv_data, 
                                          RPC_DESC_SCC_VID_LB_DETECT_CONFIG_T, 
                                          NULL));
                     break;
                     
                     case SCC_CMD_CODE_VIDEO_MJC:
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          &pt_cmd_sets[ui4_i].u, 
                                          RPC_DESC_SM_COMMAND_T_u, 
                                          "u.pv_data"));
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          pt_cmd_sets[ui4_i].u.pv_data, 
                                          RPC_DESC_SCC_VID_MJC_T, 
                                          NULL));
                     break;
                     
                     case SCC_CMD_CODE_VIDEO_SUPER_BLANK:
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          &pt_cmd_sets[ui4_i].u, 
                                          RPC_DESC_SM_COMMAND_T_u, 
                                          "u.pv_data"));
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          pt_cmd_sets[ui4_i].u.pv_data, 
                                          RPC_DESC_SCC_VID_SUPER_BLANK_T, 
                                          NULL));
                     break;
                     case SCC_CMD_CODE_VIDEO_MODE_EX:
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          &pt_cmd_sets[ui4_i].u, 
                                          RPC_DESC_SM_COMMAND_T_u, 
                                          "u.pv_data"));
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          pt_cmd_sets[ui4_i].u.pv_data, 
                                          RPC_DESC_SCC_VID_MODE_EX_T, 
                                          NULL));
                     break;
                     
                     case SCC_CMD_CODE_VIDEO_DEBLOCKING:
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          &pt_cmd_sets[ui4_i].u, 
                                          RPC_DESC_SM_COMMAND_T_u, 
                                          "u.pv_data"));
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          pt_cmd_sets[ui4_i].u.pv_data, 
                                          RPC_DESC_SCC_VID_DEBLOCKING_INFO_T, 
                                          NULL));
                     
                     break;
                     
                     case SCC_CMD_CODE_VIDEO_DISP_REGION:
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          &pt_cmd_sets[ui4_i].u, 
                                          RPC_DESC_SM_COMMAND_T_u, 
                                          "u.pv_data"));
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          pt_cmd_sets[ui4_i].u.pv_data, 
                                          RPC_DESC_VSH_REGION_INFO_T, 
                                          NULL));
                     
                     break;
                     
                     case SCC_CMD_CODE_VIDEO_SUPER_FREEZE:
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          &pt_cmd_sets[ui4_i].u, 
                                          RPC_DESC_SM_COMMAND_T_u, 
                                          "u.pv_data"));
                         RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                          pt_cmd_sets[ui4_i].u.pv_data, 
                                          RPC_DESC_SCC_VID_SUPER_FREEZE_T, 
                                          NULL));
                     
                     break;
                     
                     default:
                         RI_LOG(" SM CMD not add %d , %d!!\n", pt_cmd_sets[ui4_i].e_code, pt_cmd_sets[ui4_i].u.ui4_data);
                         break;
                 }
              }
         }
        break;

    case SCC_CMD_TYPE_AUDIO:
        {
            for (ui4_i = 0; pt_cmd_sets[ui4_i].e_code != SM_CMD_CODE_END; ui4_i ++)
            {
                switch(pt_cmd_sets[ui4_i].e_code)
                {
                    /* Audio command*/
                    case SCC_CMD_CODE_AUDIO_VOLUME:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                                                         
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_AUD_VOLUME_INFO_T, 
                                         NULL));
                    break;
                    case SCC_CMD_CODE_AUDIO_VOLUME_EX:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                                                         
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_AUD_VOLUME_INFO_T, 
                                         NULL));
                    break;
                    case SCC_CMD_CODE_AUDIO_LOUD_SPEAKER:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                                                         
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_AUD_LOUD_SPEAKER_T, 
                                         NULL));
                    break;
                    case SCC_CMD_CODE_AUDIO_SE_TRIM:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                                                         
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_AUD_SE_TRIM_T, 
                                         NULL));
                    break;
                    case SCC_CMD_CODE_AUDIO_SE_VOLUME:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                                                         
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_AUD_VOLUME_INFO_T, 
                                         NULL));
                    break;
                    case SCC_CMD_CODE_AUDIO_CHNL_DELAY:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_AUD_CHNL_DELAY_T, 
                                         NULL));
                    break;
                    case SCC_CMD_CODE_AUDIO_SE_EQ_USER_LVL:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_AUD_SE_QA_USER_LVL_T, 
                                         NULL));
                    break;
                    case SCC_CMD_CODE_AUDIO_SE_EQ_USER_LVL_EX:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_AUD_SE_QA_USER_LVL_T, 
                                         NULL));
                    break;
                    case SCC_CMD_CODE_AUDIO_HDPHONE_NFY:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_AUD_HDPHONE_DETECT_NFT_INFO_T, 
                                         NULL));
                    break;
                    case SCC_CMD_CODE_AUDIO_SE_SURROUND_EX:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_AUD_SE_SURROUND_INFO_T, 
                                         NULL));
                    break;
                    case SCC_CMD_CODE_AUDIO_SE_LINOUT_VOL_MODE:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_AUD_DEC_LINEOUT_VOL_T, 
                                         NULL));
                    break;
                    case SCC_CMD_CODE_AUDIO_SET_PEQ_INFO:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T, 
                                         NULL));
                    break;
                    case SCC_CMD_CODE_AUDIO_CLIP_VOLUME:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                                                         
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_AUD_VOLUME_INFO_T,
                                         NULL));
                    break;
                    default:
                        RI_LOG(" SM CMD not add %d , %d!!\n", pt_cmd_sets[ui4_i].e_code, pt_cmd_sets[ui4_i].u.ui4_data);
                        break;
                }
             }
        }
        break;

    case SCC_CMD_TYPE_DISP:
        {
            for (ui4_i = 0; pt_cmd_sets[ui4_i].e_code != SM_CMD_CODE_END; ui4_i ++)
            {
                switch(pt_cmd_sets[ui4_i].e_code)
                {
                    case SCC_CMD_CODE_DISP_RESOLUTION:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_DISP_RESOLUTION_T, 
                                         NULL));
                    
                    break;
                    case SCC_CMD_CODE_DISP_BACKGROUND:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_BG_COLOR_T, 
                                         NULL));
                    
                    break;            
                    
                    case SCC_CMD_CODE_DISP_COLOR_GAIN:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_DISP_COLOR_GAIN_T, 
                                         NULL));
                    
                    break;
                    case SCC_CMD_CODE_DISP_COLOR_OFFSET:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_DISP_COLOR_GAIN_T, 
                                         NULL));
                    
                    break;
                    case SCC_CMD_CODE_DISP_ADP_BACK_LIGHT_RNG:
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         &pt_cmd_sets[ui4_i].u, 
                                         RPC_DESC_SM_COMMAND_T_u, 
                                         "u.pv_data"));
                        RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                         pt_cmd_sets[ui4_i].u.pv_data, 
                                         RPC_DESC_SCC_DISP_RANGE_INFO_T, 
                                         NULL));
                    
                    break;      
                    default:
                        RI_LOG(" SM CMD not add %d , %d!!\n", pt_cmd_sets[ui4_i].e_code, pt_cmd_sets[ui4_i].u.ui4_data);
                        break;
                }
             }
        }
        break;

    default:
        return SMR_INV_COMMAND;
    }
    

    RET_ON_FAIL(RPC_ADD_REF_DESC_ARR(RPC_DEFAULT_ID, 
                         ui4_i + 1, 
                         pt_cmd_sets, 
                         RPC_DESC_SM_COMMAND_T, 
                         NULL));

    return RPCR_OK;
    
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_comp_open
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
 INT32 _x_scc_comp_open(
    SM_COMMAND_T *  pt_cmd_sets,
    HANDLE_T *      ph_handle)
{
    RPC_DECL(2, INT32);        

    _set_cmd_array(pt_cmd_sets);
    
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_cmd_sets);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_handle);
    
    RPC_DO_OP("x_scc_comp_open");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
INT32 c_scc_comp_open( SM_COMMAND_T*           pt_cmd_sets,
                       HANDLE_T*               ph_comp )
{
    INT32     i4_ret;

    i4_ret = _x_scc_comp_open( pt_cmd_sets,
                              ph_comp );

    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_comp_grp_set
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_comp_grp_set( SM_COMMAND_T*        pt_cmd_sets )
{
    RPC_DECL(1, INT32);        
    
    _set_cmd_array(pt_cmd_sets);
    
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_cmd_sets);
    
    RPC_DO_OP("x_scc_comp_grp_set");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_comp_grp_set
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_comp_att_incr(
    HANDLE_T            h_comp,
    SM_CMD_CODE_T       e_scc_cmd_code,
    VOID*               pv_cmd_arg,
    UINT16*             pui2_curr_value )
{

    RPC_DECL(4, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_comp);
    RPC_ARG_INP(ARG_TYPE_UINT32, e_scc_cmd_code);    
    RPC_ARG_INP(ARG_TYPE_UINT32,  pv_cmd_arg);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_curr_value);

    
    RPC_DO_OP("x_scc_comp_att_incr");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}


/*-----------------------------------------------------------------------------
 * Name: c_scc_comp_grp_set
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_comp_att_decr( 
    HANDLE_T            h_comp,
    SM_CMD_CODE_T       e_scc_cmd_code,
    VOID*               pv_cmd_arg,
    UINT16*             pui2_curr_value )
{
    RPC_DECL(4, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_comp);
    RPC_ARG_INP(ARG_TYPE_UINT32, e_scc_cmd_code);    
    RPC_ARG_INP(ARG_TYPE_UINT32,  pv_cmd_arg);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_curr_value);

    
    RPC_DO_OP("x_scc_comp_att_decr");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}


/*-----------------------------------------------------------------------------
 * Name: c_scc_comp_grp_set
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_comp_att_get_num_steps( 
    HANDLE_T            h_comp,
    SM_CMD_CODE_T       e_scc_cmd_code,
    VOID*               pv_cmd_arg,
    UINT16*             pui2_num_steps )
{
    RPC_DECL(4, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_comp);
    RPC_ARG_INP(ARG_TYPE_UINT32, e_scc_cmd_code);    
    RPC_ARG_INP(ARG_TYPE_UINT32,  pv_cmd_arg);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_num_steps);

    
    RPC_DO_OP("x_scc_comp_att_get_num_steps");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/* Video related API *********************************************************/
/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_mode( HANDLE_T                   h_vid,
                          SCC_VID_MODE_T             e_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_mode);
    
    RPC_DO_OP("x_scc_vid_set_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_mode_ex
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_mode_ex( HANDLE_T                   h_vid,
                             const SCC_VID_MODE_EX_T*   pt_mode )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_mode, RPC_DESC_SCC_VID_MODE_EX_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_mode);   
    
    RPC_DO_OP("x_scc_vid_set_mode_ex");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_sig_booster
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_sig_booster( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_BOOL, b_is_on);

    RPC_DO_OP("x_scc_vid_set_sig_booster");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_bg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_bg( HANDLE_T                     h_vid,
                        const SCC_BG_COLOR_T*        pt_bg )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_bg, RPC_DESC_SCC_BG_COLOR_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_bg);   
    
    RPC_DO_OP("x_scc_vid_set_bg");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_super_blank
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_super_blank( HANDLE_T                     h_vid,
                                 const SCC_VID_SUPER_BLANK_T* pt_blank )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_blank, RPC_DESC_SCC_VID_SUPER_BLANK_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_blank);   
    
    RPC_DO_OP("x_scc_vid_set_super_blank");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_pic_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_pic_mode( HANDLE_T               h_vid,
                              SCC_VID_PIC_MODE_T     e_pic_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_pic_mode);
    
    RPC_DO_OP("x_scc_vid_set_pic_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_hdmi_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_hdmi_mode( HANDLE_T               h_vid,
                               SCC_VID_HDMI_MODE_T    e_hdmi_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_hdmi_mode);
    
    RPC_DO_OP("x_scc_vid_set_hdmi_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_color_sys
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_color_sys( HANDLE_T               h_vid,
                               SCC_VID_COLOR_SYS_T    e_color_sys )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_color_sys);
    
    RPC_DO_OP("x_scc_vid_set_color_sys");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_scr_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_scr_mode( HANDLE_T               h_vid,
                              SCC_VID_SCREEN_MODE_T  e_scr_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_scr_mode);
    
    RPC_DO_OP("x_scc_vid_set_scr_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_enhance
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_enhance( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_enhance )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_enhance);
    
    RPC_DO_OP("x_scc_vid_set_enhance");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_blur
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_blur( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_blur )
{ 
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_blur);

    RPC_DO_OP("x_scc_vid_set_blur");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_overscan_clipper
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_overscan_clipper( 
    HANDLE_T                   h_vid,
    const SCC_VID_OVER_SCAN_CLIPPER_T* pt_clipper )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_clipper, RPC_DESC_SCC_VID_OVER_SCAN_CLIPPER_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_clipper);   
    
    RPC_DO_OP("x_scc_vid_set_overscan_clipper");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_cti
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_cti( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_cti )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_cti);

    RPC_DO_OP("x_scc_vid_set_cti");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_eti
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_eti( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_eti )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_eti);

    RPC_DO_OP("x_scc_vid_set_eti");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_sharpness
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_sharpness( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_sharpness )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_sharpness);

    RPC_DO_OP("x_scc_vid_set_sharpness");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_color_suppress
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_color_suppress( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_color_supress )
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_color_supress);

    RPC_DO_OP("x_scc_vid_set_color_suppress");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_nr
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_nr( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_nr )
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_nr);

    RPC_DO_OP("x_scc_vid_set_nr");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  

}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3dnr
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3dnr( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_3dnr )
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_3dnr);

    RPC_DO_OP("x_scc_vid_set_3dnr");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_black_lvl_ext
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_black_lvl_ext( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_black_lvl_ext )
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_black_lvl_ext);

    RPC_DO_OP("x_scc_vid_set_black_lvl_ext");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_white_peak_rdct
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_white_peak_rdct( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_white_peak_rdct )
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_white_peak_rdct);

    RPC_DO_OP("x_scc_vid_set_white_peak_rdct");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_flesh_tone
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_flesh_tone( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_flesh_tone )
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_flesh_tone);

    RPC_DO_OP("x_scc_vid_set_flesh_tone");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_luma
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_luma( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_luma )
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_luma);

    RPC_DO_OP("x_scc_vid_set_luma");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_letter_box_detect
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_letter_box_detect( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_BOOL, b_is_on);

    RPC_DO_OP("x_scc_vid_set_letter_box_detect");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_lb_detect_config
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_lb_detect_config( 
    HANDLE_T                   h_vid,
    SCC_VID_LB_DETECT_CONFIG_T* pt_config )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_config, RPC_DESC_SCC_VID_LB_DETECT_CONFIG_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_config);   
    
    RPC_DO_OP("x_scc_vid_set_lb_detect_config");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_dynamic_scaling
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_dynamic_scaling( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_effect_time )
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_effect_time);

    RPC_DO_OP("x_scc_vid_set_dynamic_scaling");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_deblocking
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_deblocking( 
    HANDLE_T                   h_vid,
    SCC_VID_DEBLOCKING_INFO_T* pt_deblocking )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_deblocking, RPC_DESC_SCC_VID_DEBLOCKING_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_deblocking);   
    
    RPC_DO_OP("x_scc_vid_set_deblocking");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_split_scrn_demo
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_split_scrn_demo( 
    HANDLE_T                   h_vid,
    SCC_VID_SPLIT_SCRN_DEMO_T  e_splt_scrn_demo )
{
    RPC_DECL(2, INT32);
   
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32, e_splt_scrn_demo);   
    
    RPC_DO_OP("x_scc_vid_set_split_scrn_demo");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 

}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_h_position
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_h_position( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_offset);
    
    RPC_DO_OP("x_scc_vid_set_h_position");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_v_position
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_v_position( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_offset);
    
    RPC_DO_OP("x_scc_vid_set_v_position");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_h_size
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_h_size( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_offset);
    
    RPC_DO_OP("x_scc_vid_set_h_size");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_v_size
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_v_size( 
    HANDLE_T                   h_vid,
    UINT16                     ui2_offset )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16, ui2_offset);
    
    RPC_DO_OP("x_scc_vid_set_v_size");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_film_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_film_mode( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_BOOL, b_is_on);

    RPC_DO_OP("x_scc_vid_set_film_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_reset_vga_timing_option
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_reset_vga_timing_option( 
    HANDLE_T                   h_vid )
{
    RPC_DECL(1, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);

    RPC_DO_OP("x_scc_vid_set_reset_vga_timing_option");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_ambig_vga_timing_ctrl
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_ambig_vga_timing_ctrl( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_BOOL, b_is_on);

    RPC_DO_OP("x_scc_vid_set_ambig_vga_timing_ctrl");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_force_unmute_on_no_sig
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_force_unmute_on_no_sig( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_BOOL, b_is_on);

    RPC_DO_OP("x_scc_vid_set_force_unmute_on_no_sig");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_srm_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_srm_mode( 
    HANDLE_T                   h_vid,
    SCC_VID_SRM_MODE_T         e_srm_mode  )
{
    RPC_DECL(2, INT32);
   
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32, e_srm_mode);   
    
    RPC_DO_OP("x_scc_vid_set_srm_mode");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 

}
/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_super_freeze
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_super_freeze( 
    HANDLE_T                   h_vid,
    const SCC_VID_SUPER_FREEZE_T*   pt_freeze)
/*    BOOL                       b_is_on ) */
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_freeze, RPC_DESC_SCC_VID_SUPER_FREEZE_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_freeze);   
    
    RPC_DO_OP("x_scc_vid_set_super_freeze");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_super_freeze
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_super_freeze( 
    HANDLE_T                   h_vid,
    BOOL*                       pb_is_on ) 
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_is_on);
    
    RPC_DO_OP("x_scc_vid_get_super_freeze");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_force_unmute_on_no_sig
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_force_unmute_on_no_sig( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_is_on);
    
    RPC_DO_OP("x_scc_vid_get_force_unmute_on_no_sig");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_mode( HANDLE_T                   h_vid,
                          SCC_VID_MODE_T*            pe_mode )
{
    RPC_DECL(2, INT32);

   
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_mode);   
   
    RPC_DO_OP("x_scc_vid_get_mode");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_mode_capability
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_NOT_SUPPORT     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_mode_capability( HANDLE_T                   h_vid,
                                     SCC_VID_MODE_T             e_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,  h_vid);
    //todo e_mode is in or out ?
    RPC_ARG_INP(ARG_TYPE_INT32,   e_mode);
    
    RPC_DO_OP("x_scc_vid_get_mode_capability");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_bg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_bg( HANDLE_T                     h_vid,
                        SCC_BG_COLOR_T*              pt_bg )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_bg, RPC_DESC_SCC_BG_COLOR_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_bg);   
    
    RPC_DO_OP("x_scc_vid_get_bg");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_pic_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_pic_mode( HANDLE_T               h_vid,
                              SCC_VID_PIC_MODE_T*    pe_pic_mode )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_pic_mode);   
    
    RPC_DO_OP("x_scc_vid_get_pic_mode");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_hdmi_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_hdmi_mode( HANDLE_T               h_vid,
                               SCC_VID_HDMI_MODE_T*   pe_hdmi_mode )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_hdmi_mode);   
    
    RPC_DO_OP("x_scc_vid_get_hdmi_mode");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_color_sys
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_color_sys( HANDLE_T               h_vid,
                               SCC_VID_COLOR_SYS_T*   pe_color_sys )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_color_sys);   
    
    RPC_DO_OP("x_scc_vid_get_color_sys");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_scr_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_scr_mode( HANDLE_T               h_vid,
                              SCC_VID_SCREEN_MODE_T* pe_scr_mode )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_scr_mode);   
    
    RPC_DO_OP("x_scc_vid_get_scr_mode");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_enhance
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_enhance( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_enhance )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_enhance);
    
    RPC_DO_OP("x_scc_vid_get_enhance");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_blur
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_blur( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_blur )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_blur);
    
    RPC_DO_OP("x_scc_vid_get_blur");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_overscan_clipper
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_overscan_clipper( 
    HANDLE_T                     h_vid,
    SCC_VID_OVER_SCAN_CLIPPER_T* pt_clipper )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_clipper,RPC_DESC_SCC_VID_OVER_SCAN_CLIPPER_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_clipper);   
    
    RPC_DO_OP("x_scc_vid_get_overscan_clipper");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_cti
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_cti( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_cti )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_cti);
    
    RPC_DO_OP("x_scc_vid_get_cti");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_eti
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_eti( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_eti )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_eti);
    
    RPC_DO_OP("x_scc_vid_get_eti");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_sharpness
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_sharpness( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_sharpness )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_sharpness);
    
    RPC_DO_OP("x_scc_vid_get_sharpness");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_color_supress
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_color_supress( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_color_supress )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_color_supress);
    
    RPC_DO_OP("x_scc_vid_get_color_supress");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_nr
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_nr( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_nr )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_nr);
    
    RPC_DO_OP("x_scc_vid_get_nr");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3dnr
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3dnr( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_3dnr )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_3dnr);
    
    RPC_DO_OP("x_scc_vid_get_3dnr");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_black_lvl_ext
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_black_lvl_ext( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_black_lvl_ext )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_black_lvl_ext);
    
    RPC_DO_OP("x_scc_vid_get_black_lvl_ext");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_white_peak_rdct
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_white_peak_rdct( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_white_peak_rdct )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_white_peak_rdct);
    
    RPC_DO_OP("x_scc_vid_get_white_peak_rdct");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_flesh_tone
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_flesh_tone( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_flesh_tone )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_flesh_tone);
    
    RPC_DO_OP("x_scc_vid_get_flesh_tone");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_luma
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_luma( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_luma )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_luma);
    
    RPC_DO_OP("x_scc_vid_get_luma");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_letter_box_detect
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_letter_box_detect( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_is_on);
    
    RPC_DO_OP("x_scc_vid_get_letter_box_detect");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_lb_detect_config
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_lb_detect_config( 
    HANDLE_T                   h_vid,
    SCC_VID_LB_DETECT_CONFIG_T* pt_config )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_config, RPC_DESC_SCC_VID_LB_DETECT_CONFIG_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_config);   
    
    RPC_DO_OP("x_scc_vid_get_lb_detect_config");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_vid_get_dynamic_scaling
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_dynamic_scaling( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_effect_time )
{
    RPC_DECL(2, INT32);
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_effect_time);
    
    RPC_DO_OP("x_scc_vid_get_dynamic_scaling");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_deblocking
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_deblocking( 
    HANDLE_T                   h_vid,
    SCC_VID_DEBLOCKING_INFO_T* pt_deblocking )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_deblocking, RPC_DESC_SCC_VID_DEBLOCKING_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_deblocking);   
    
    RPC_DO_OP("x_scc_vid_get_deblocking");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_split_scrn_demo
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_split_scrn_demo( 
    HANDLE_T                   h_vid,
    SCC_VID_SPLIT_SCRN_DEMO_T* pe_splt_scrn_demo )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_splt_scrn_demo);   
    
    RPC_DO_OP("x_scc_vid_get_split_scrn_demo");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_h_position
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_h_position( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr )
{
    RPC_DECL(4, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_min);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_max);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_curr);
    
    RPC_DO_OP("x_scc_vid_get_h_position");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_v_position
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_v_position( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr )
{
    RPC_DECL(4, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_min);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_max);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_curr);
    
    RPC_DO_OP("x_scc_vid_get_v_position");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_h_size
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_h_size( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr )
{
    RPC_DECL(4, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_min);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_max);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_curr);
    
    RPC_DO_OP("x_scc_vid_get_h_size");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  

}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_v_size
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_v_size( 
    HANDLE_T                   h_vid,
    UINT16*                    pui2_min,
    UINT16*                    pui2_max,
    UINT16*                    pui2_curr )
{
    RPC_DECL(4, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_min);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_max);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_curr);
    
    RPC_DO_OP("x_scc_vid_get_v_size");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_film_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_film_mode( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_is_on);
    
    RPC_DO_OP("x_scc_vid_get_film_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_iframe_nfy
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_nfy(
    HANDLE_T                     h_vid, 
    x_scc_vid_iframe_nfy_fct     pf_scc_iframe_nfy, 
    VOID*                        pv_nfy_tag )
{
    //todo  what type pv_nfy_tag is ?
    
    RPC_DECL(3, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_scc_iframe_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    
    RPC_DO_OP("x_scc_vid_set_iframe_nfy");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_iframe_alloc_buffer
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_iframe_alloc_buffer(
    HANDLE_T                     h_vid, 
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf)
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_iframe_buf, RPC_DESC_SCC_VID_IFRAME_BUFF_T, NULL));
    RPC_CHECK(
          RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_iframe_buf->puc_iframe_buff,pt_iframe_buf->z_iframe_size));

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    //todo is right:Input/Output?
    RPC_ARG_IO(ARG_TYPE_REF_DESC, pt_iframe_buf);
    
    RPC_DO_OP("x_scc_vid_iframe_alloc_buffer");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_iframe_decode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_buffer(
    HANDLE_T                     h_vid, 
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf)
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_iframe_buf, RPC_DESC_SCC_VID_IFRAME_BUFF_T, NULL));
    RPC_CHECK(
          RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_iframe_buf->puc_iframe_buff,pt_iframe_buf->z_iframe_size));

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_iframe_buf);
    
    RPC_DO_OP("x_scc_vid_set_iframe_buffer");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_iframe_decode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_iframe_decode(
    HANDLE_T                     h_vid, 
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf)
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_iframe_buf, RPC_DESC_SCC_VID_IFRAME_BUFF_T, NULL));
    RPC_CHECK(
          RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_iframe_buf->puc_iframe_buff,pt_iframe_buf->z_iframe_size));

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_iframe_buf);
    
    RPC_DO_OP("x_scc_vid_iframe_decode");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_iframe_free_buffer
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_iframe_free_buffer(
    HANDLE_T                     h_vid, 
    SCC_VID_IFRAME_BUFF_T*       pt_iframe_buf)
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_iframe_buf, RPC_DESC_SCC_VID_IFRAME_BUFF_T, NULL));
    RPC_CHECK(
          RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_iframe_buf->puc_iframe_buff,pt_iframe_buf->z_iframe_size));

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    //todo is right:Input/Output?
    RPC_ARG_IO(ARG_TYPE_REF_DESC, pt_iframe_buf);
    
    RPC_DO_OP("x_scc_vid_iframe_free_buffer");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_iframe_codec
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_codec(
    HANDLE_T                      h_vid, 
    SCC_VID_IFRAME_CODEC_T        e_codec )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_codec);
    
    RPC_DO_OP("x_scc_vid_set_iframe_codec");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_iframe_ctrl
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_ctrl(
    HANDLE_T                      h_vid, 
    SCC_VID_IFRAME_CTRL_T         e_ctrl_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_ctrl_mode);
    
    RPC_DO_OP("x_scc_vid_set_iframe_ctrl");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_iframe_resolution
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_iframe_resolution(
    HANDLE_T                       h_vid, 
    UINT32*                        pui4_width, 
    UINT32*                        pui4_height)

{
    RPC_DECL(3, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,  pui4_width);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,  pui4_height);
    
    RPC_DO_OP("x_scc_vid_get_iframe_resolution");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_iframe_pla_order
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_pla_order(
    HANDLE_T                       h_vid, 
    VSH_SET_PLANE_ORDER_INFO_T*    pt_order_info)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_order_info, RPC_DESC_VSH_SET_PLANE_ORDER_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_order_info);   
    
    RPC_DO_OP("x_scc_vid_set_iframe_pla_order");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_iframe_src_region
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_src_region(
    HANDLE_T                       h_vid, 
    VSH_REGION_INFO_T*             pt_region_info)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_region_info, RPC_DESC_VSH_REGION_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_region_info);   
    
    RPC_DO_OP("x_scc_vid_set_iframe_src_region");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_iframe_disp_region
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_disp_region(
    HANDLE_T                       h_vid, 
    VSH_REGION_INFO_T*             pt_region_info)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_region_info, RPC_DESC_VSH_REGION_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_region_info);   
    
    RPC_DO_OP("x_scc_vid_set_iframe_disp_region");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_iframe_src_region
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_iframe_src_region(
    HANDLE_T                       h_vid, 
    VSH_REGION_INFO_T*             pt_region_info)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_region_info, RPC_DESC_VSH_REGION_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_region_info);   
    
    RPC_DO_OP("x_scc_vid_get_iframe_src_region");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_iframe_disp_region
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_iframe_disp_region(
    HANDLE_T                       h_vid, 
    VSH_REGION_INFO_T*             pt_region_info)
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_region_info, RPC_DESC_VSH_REGION_INFO_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_region_info);   
    
    RPC_DO_OP("x_scc_vid_get_iframe_disp_region");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_iframe_surface
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_iframe_surface(
    HANDLE_T                       h_vid,
    HANDLE_T                       h_surface)
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    //todo h_surface is in or out ?
    RPC_ARG_INP(ARG_TYPE_UINT32,  h_surface);
    
    RPC_DO_OP("x_scc_vid_get_iframe_surface");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

#ifdef SYS_MHP_SUPPORT
/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_iframe_not_display
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_iframe_not_display(
    HANDLE_T                      h_vid, 
    BOOL                          b_not_display )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_BOOL,  b_not_display);
    
    RPC_DO_OP("x_scc_vid_set_iframe_not_display");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
#endif

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_vdrips_nfy
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_vdrips_nfy(
    HANDLE_T                     h_vid,  
    x_scc_vid_vdrips_nfy_fct     pf_scc_vdrips_nfy, 
    VOID*                        pv_nfy_tag )
{
    //todo  what type pv_nfy_tag is ?
    
    RPC_DECL(3, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_scc_vdrips_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    
    RPC_DO_OP("x_scc_vid_set_vdrips_nfy");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 

}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_vdrips_feed
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_vdrips_feed(
    HANDLE_T                     h_vid, 
    SCC_VID_VDRIPS_BUFF_T*       pt_vdrips_buf)
{
     RPC_DECL(2, INT32);    
    
     RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_vdrips_buf, RPC_DESC_SCC_VID_VDRIPS_BUFF_T, NULL));
     RPC_CHECK(
           RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_vdrips_buf->puc_vdrips_buff,pt_vdrips_buf->z_vdrips_size));
    
     RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
     //todo pt_vdrips_buf is in or out ?
     RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_vdrips_buf);
     
     RPC_DO_OP("x_scc_vid_vdrips_feed");
     
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_vdrips_play_ctrl
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_vdrips_play_ctrl(
    HANDLE_T                      h_vid, 
    SCC_VID_VDRIPS_PLAY_CTRL_T    e_ctrl_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_ctrl_mode);
    
    RPC_DO_OP("x_scc_vid_vdrips_play_ctrl");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_vdrips_duration
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_vdrips_duration(
    HANDLE_T                       h_vid, 
    UINT32                         ui4_duration )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    //todo ui4_duration is in or out ?
    RPC_ARG_INP(ARG_TYPE_UINT32, ui4_duration);
    
    RPC_DO_OP("x_scc_vid_get_vdrips_duration");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_di_film_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_di_film_mode( HANDLE_T                  h_vid,
                                  SCC_VID_DI_FILM_MODE_T    e_film_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_film_mode);
    
    RPC_DO_OP("x_scc_vid_set_di_film_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_di_film_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_di_film_mode( HANDLE_T                   h_vid,
                                  SCC_VID_DI_FILM_MODE_T*    pe_film_mode )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_film_mode);   
    
    RPC_DO_OP("x_scc_vid_get_di_film_mode");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_di_ma
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_di_ma( HANDLE_T           h_vid,
                           SCC_VID_DI_MA_T    e_di_ma )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_di_ma);
    
    RPC_DO_OP("x_scc_vid_set_di_ma");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_di_ma
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_di_ma( HANDLE_T            h_vid,
                           SCC_VID_DI_MA_T*    pe_di_ma )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_di_ma);   
    
    RPC_DO_OP("x_scc_vid_get_di_ma");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_di_edge_preserve
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_di_edge_preserve( HANDLE_T                      h_vid,
                                      SCC_VID_DI_EDGE_PRESERVE_T    e_edge )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_edge);
    
    RPC_DO_OP("x_scc_vid_set_di_edge_preserve");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_di_edge_preserve
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_di_edge_preserve( HANDLE_T                       h_vid,
                                      SCC_VID_DI_EDGE_PRESERVE_T*    pe_edge )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_edge);   
    
    RPC_DO_OP("x_scc_vid_get_di_edge_preserve");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_mjc
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_mjc( HANDLE_T                h_vid,
                         const SCC_VID_MJC_T*    pt_mjc )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_mjc, RPC_DESC_SCC_VID_MJC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_mjc);   
    
    RPC_DO_OP("x_scc_vid_set_mjc");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_mjc
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_mjc( HANDLE_T          h_vid,
                         SCC_VID_MJC_T*    pt_mjc )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_mjc, RPC_DESC_SCC_VID_MJC_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_mjc);   
    
    RPC_DO_OP("x_scc_vid_get_mjc");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_xvycc
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_xvycc( HANDLE_T           h_vid,
                           SCC_VID_XVYCC_T    e_xvycc )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_xvycc);
    
    RPC_DO_OP("x_scc_vid_set_xvycc");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_xvycc
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_xvycc( HANDLE_T            h_vid,
                           SCC_VID_XVYCC_T*    pe_xvycc )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_xvycc);   
    
    RPC_DO_OP("x_scc_vid_get_xvycc");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_wcg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_wcg( HANDLE_T         h_vid,
                         SCC_VID_WCG_T    e_wcg )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_wcg);
    
    RPC_DO_OP("x_scc_vid_set_wcg");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_wcg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_wcg( HANDLE_T          h_vid,
                         SCC_VID_WCG_T*    pe_wcg )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_wcg);   
    
    RPC_DO_OP("x_scc_vid_get_wcg");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_game_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_game_mode( HANDLE_T               h_vid,
                               SCC_VID_GAME_MODE_T    e_game )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_game);
    
    RPC_DO_OP("x_scc_vid_set_game_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_game_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_game_mode( HANDLE_T                h_vid,
                               SCC_VID_GAME_MODE_T*    pe_game )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_game);   
    
    RPC_DO_OP("x_scc_vid_get_game_mode");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_blue_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_blue_stretch( HANDLE_T                  h_vid,
                                  SCC_VID_BLUE_STRETCH_T    e_stretch )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_stretch);
    
    RPC_DO_OP("x_scc_vid_set_blue_stretch");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_blue_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_blue_stretch( HANDLE_T                   h_vid,
                                  SCC_VID_BLUE_STRETCH_T*    pe_stretch )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_stretch);   
    
    RPC_DO_OP("x_scc_vid_get_blue_stretch");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_mpeg_nr
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_mpeg_nr( HANDLE_T             h_vid,
                             SCC_VID_MPEG_NR_T    e_nr )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_nr);
    
    RPC_DO_OP("x_scc_vid_set_mpeg_nr");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_mpeg_nr
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_mpeg_nr( HANDLE_T              h_vid,
                             SCC_VID_MPEG_NR_T*    pe_nr )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_nr);   
    
    RPC_DO_OP("x_scc_vid_get_mpeg_nr");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_hdcp_key_exist
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_hdcp_key_exist( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_exist )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_is_exist);
    
    RPC_DO_OP("x_scc_vid_get_hdcp_key_exist");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_black_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_black_stretch( HANDLE_T                  h_vid,
                                     SCC_VID_BLACK_STRETCH_T   e_stretch )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,    e_stretch);
    
    RPC_DO_OP("x_scc_vid_set_black_stretch");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_black_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_black_stretch( HANDLE_T                   h_vid,
                                     SCC_VID_BLACK_STRETCH_T*   pe_stretch )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_stretch);
    
    RPC_DO_OP("x_scc_vid_get_black_stretch");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_white_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_white_stretch( HANDLE_T                  h_vid,
                                     SCC_VID_WHITE_STRETCH_T   e_stretch )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,  h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,   e_stretch);
    
    RPC_DO_OP("x_scc_vid_set_white_stretch");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_white_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_white_stretch( HANDLE_T                   h_vid,
                                     SCC_VID_WHITE_STRETCH_T*   pe_stretch )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_stretch);
    
    RPC_DO_OP("x_scc_vid_get_white_stretch");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}


/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_black_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_black_stretch_val( HANDLE_T                  h_vid,
                                        UINT16              ui2_stretch )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16,     ui2_stretch);
    
    RPC_DO_OP("x_scc_vid_set_black_stretch_val");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_black_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_black_stretch_val( HANDLE_T                   h_vid,
                                        UINT16*             pui2_stretch )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_stretch);
    
    RPC_DO_OP("x_scc_vid_get_black_stretch_val");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_white_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_white_stretch_val( HANDLE_T          h_vid,
                                        UINT16            ui2_stretch )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT16,     ui2_stretch);
    
    RPC_DO_OP("x_scc_vid_set_white_stretch_val");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_white_stretch
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_white_stretch_val( HANDLE_T            h_vid,
                                        UINT16*             pui2_stretch )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_stretch);
    
    RPC_DO_OP("x_scc_vid_get_white_stretch_val");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_vdp_atomic_cmd
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          pt_atomic_cmd           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_vdp_atomic_cmd( HANDLE_T        h_vid,
                                            SM_COMMAND_T*   pt_atomic_cmd )
{
    UINT32 ui4_i;
    
    RPC_DECL(2, INT32);

    for (ui4_i = 0; pt_atomic_cmd[ui4_i].e_code != SM_CMD_CODE_END; ui4_i ++)
    {
        switch(pt_atomic_cmd[ui4_i].e_code)
        {
            case SCC_CMD_CODE_VIDEO_DISP_REGION:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                            &pt_atomic_cmd[ui4_i].u, 
                            RPC_DESC_SM_COMMAND_T_u, 
                            "u.pv_data"));    
                                  
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                            pt_atomic_cmd[ui4_i].u.pv_data, 
                            RPC_DESC_VSH_REGION_INFO_T, 
                            NULL));
                break;
             
            case SCC_CMD_CODE_VIDEO_OVERSCAN_CLIPPER:
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                            &pt_atomic_cmd[ui4_i].u, 
                            RPC_DESC_SM_COMMAND_T_u, 
                            "u.pv_data"));    
                                  
                RET_ON_FAIL(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                            pt_atomic_cmd[ui4_i].u.pv_data, 
                            RPC_DESC_SCC_VID_OVER_SCAN_CLIPPER_T, 
                            NULL));
                break;
             
            default:
                RI_LOG(" SM atomic CMD not add %d , %d!!\n", 
                       pt_atomic_cmd[ui4_i].e_code, pt_atomic_cmd[ui4_i].u.ui4_data);
                break;
        }
    }
    RET_ON_FAIL(RPC_ADD_REF_DESC_ARR(RPC_DEFAULT_ID, 
                ui4_i + 1, 
                pt_atomic_cmd, 
                RPC_DESC_SM_COMMAND_T, 
                NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_DESC, pt_atomic_cmd);   
    
    RPC_DO_OP("x_scc_vid_set_vdp_atomic_cmd");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_colorspace
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_colorspace           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_colorspace( HANDLE_T                  h_vid,
                                  SCC_VID_COLORSPACE_T    e_colorspace )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_colorspace);
    
    RPC_DO_OP("x_scc_vid_set_colorspace");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_flip
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_flip( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_INP(ARG_TYPE_BOOL,  b_is_on);
    
    RPC_DO_OP("x_scc_vid_set_flip");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_mirror
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_mirror( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_INP(ARG_TYPE_BOOL,  b_is_on);
    
    RPC_DO_OP("x_scc_vid_set_mirror");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_flip
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_flip( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_is_on);
    
    RPC_DO_OP("x_scc_vid_get_flip");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_mirror
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_mirror( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_is_on);
    
    RPC_DO_OP("x_scc_vid_get_mirror");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_local_dimming_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_local_dimming_mode(
    HANDLE_T                        h_vid,
    SCC_VID_LOCAL_DIMMING_TYPE_T    e_lcdim_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_vid);
    RPC_ARG_OUT(ARG_TYPE_INT32,       e_lcdim_mode);
    
    RPC_DO_OP("x_scc_vid_set_local_dimming_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_local_dimming_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_local_dimming_mode( 
    HANDLE_T                        h_vid,
    SCC_VID_LOCAL_DIMMING_TYPE_T*   pe_lcdim_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_lcdim_mode);

    RPC_DO_OP("x_scc_vid_get_local_dimming_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_src_detect_nfy
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 _hndlr_x_scc_vid_src_detect_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }

    pt_return->e_type = ARG_TYPE_VOID;
    
    ((x_scc_vid_src_detect_nfy_fct)pv_cb_addr)(
                                (HANDLE_T)pt_args[0].u.ui4_arg,
           (SCC_VID_SRC_DETECT_NFY_COND_T)pt_args[1].u.ui4_arg,
                                          pt_args[2].u.pv_arg,
                                  (UINT32)pt_args[3].u.ui4_arg,
                                  (UINT32)pt_args[4].u.ui4_arg);

    return RPCR_OK;
}

INT32 c_scc_vid_set_src_detect_nfy(
    HANDLE_T                        h_vid, 
    x_scc_vid_src_detect_nfy_fct    pf_scc_vid_src_detect_nfy, 
    VOID*                           pv_nfy_tag) 
{
    RPC_DECL(3, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC, pf_scc_vid_src_detect_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID, pv_nfy_tag);
    
    RPC_DO_OP("x_scc_vid_set_src_detect_nfy");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_nav
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_nav                 References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_nav( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_T                e_nav )
{        
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_nav);
    
    RPC_DO_OP("x_scc_vid_set_3d_nav");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_lr_switch
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          pe_nav             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_nav( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_T*               pe_nav )
{        
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_nav);   
    
    RPC_DO_OP("x_scc_vid_get_3d_nav");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          e_3d_mode           References.
 *
 * Outputs: 
 *
 *Returns: SMR_OK              Success.
 *         SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_mode( 
    HANDLE_T                    h_vid,
    SCC_3D_MODE_T               e_3d_mode )
{        
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_3d_mode);
    
    RPC_DO_OP("x_scc_vid_set_3d_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          pe_3d_mode            References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK               Success.
 *          SMR_INTERNAL_ERROR   Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_mode( 
    HANDLE_T                    h_vid,
    SCC_3D_MODE_T*              pe_3d_mode )
{    
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_3d_mode);   
    
    RPC_DO_OP("x_scc_vid_get_3d_mode");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_parallax
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          ui4_prlx               References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_parallax( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_prlx )
{        
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  ui4_prlx);
    
    RPC_DO_OP("x_scc_vid_set_3d_parallax");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_parallax
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          pui4_prlx              References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_parallax( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_prlx )
{        
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pui4_prlx);   
    
    RPC_DO_OP("x_scc_vid_get_3d_parallax");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_fld_depth
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          ui4_fld_depth          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_fld_depth( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_fld_depth )
{        
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  ui4_fld_depth);
    
    RPC_DO_OP("x_scc_vid_set_3d_fld_depth");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_fld_depth
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          pui4_fld_depth          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_fld_depth( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_fld_depth )
{        
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pui4_fld_depth);   
    
    RPC_DO_OP("x_scc_vid_get_3d_fld_depth");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_lr_switch
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_lr_switch           References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_lr_switch( 
    HANDLE_T                    h_vid,
    SCC_3D_LR_SWITCH_T          e_lr_switch )
{        
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_lr_switch);
    
    RPC_DO_OP("x_scc_vid_set_3d_lr_switch");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_lr_switch
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          pe_lr_switch       References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_lr_switch( 
    HANDLE_T                    h_vid,
    SCC_3D_LR_SWITCH_T*         pe_lr_switch )
{        
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_lr_switch);   
    
    RPC_DO_OP("x_scc_vid_get_3d_lr_switch");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_to_2d
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                References.
 *           e_3d_to_2d          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK               Success.
 *          SMR_INTERNAL_ERROR   Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_to_2d( 
    HANDLE_T                    h_vid,
    SCC_3D_TO_2D_T              e_3d_to_2d )
{        
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_3d_to_2d);
    
    RPC_DO_OP("x_scc_vid_set_3d_to_2d");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_to_2d
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_3d_to_2d            References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_to_2d( 
    HANDLE_T                    h_vid,
    SCC_3D_TO_2D_T*             pe_3d_to_2d )
{        
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_3d_to_2d);   
    
    RPC_DO_OP("x_scc_vid_get_3d_to_2d");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_fpr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_fpr                 References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_fpr( 
    HANDLE_T                    h_vid,
    SCC_3D_FPR_T                e_fpr )
{        
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_fpr);
    
    RPC_DO_OP("x_scc_vid_set_3d_fpr");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_fpr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          pe_fpr             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_fpr( 
    HANDLE_T                    h_vid,
    SCC_3D_FPR_T*               pe_fpr )
{        
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_fpr);   
    
    RPC_DO_OP("x_scc_vid_get_3d_fpr");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_protrude_scr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          ui4_pro_scr             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_protrude_scr( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_pro_scr )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,  h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT32,  ui4_pro_scr);
    
    RPC_DO_OP("x_scc_vid_set_3d_protrude_scr");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_protrude_scr
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          pui4_pro_scr            References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_protrude_scr( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_pro_scr )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_pro_scr);   
    
    RPC_DO_OP("x_scc_vid_get_3d_protrude_scr");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_distance_2_tv
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          ui4_dis_2_tv            References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_distance_2_tv( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_dis_2_tv )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,  h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT32,  ui4_dis_2_tv);
    
    RPC_DO_OP("x_scc_vid_set_3d_distance_2_tv");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_distance_2_tv
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          pui4_dis_2_tv          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_distance_2_tv( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_dis_2_tv )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_dis_2_tv);   
    
    RPC_DO_OP("x_scc_vid_get_3d_distance_2_tv");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_osd_depth
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          ui4_osd_depth           References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_osd_depth( 
    HANDLE_T                    h_vid,
    UINT32                      ui4_osd_depth )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,  h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT32,  ui4_osd_depth);
    
    RPC_DO_OP("x_scc_vid_set_3d_osd_depth");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_osd_depth
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                   References.
 *          pui4_osd_depth          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_osd_depth( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_osd_depth )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_osd_depth);   
    
    RPC_DO_OP("x_scc_vid_get_3d_osd_depth");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_fmt_cap
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          pui4_fmt_cap          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_fmt_cap( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_fmt_cap )
{        
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pui4_fmt_cap);   
    
    RPC_DO_OP("x_scc_vid_get_3d_fmt_cap");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_ctrl_cap
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          pui4_ctrl_cap          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_ctrl_cap( 
    HANDLE_T                    h_vid,
    UINT32*                     pui4_ctrl_cap )
{        
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pui4_ctrl_cap);   
    
    RPC_DO_OP("x_scc_vid_get_3d_ctrl_cap");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_nav_auto_chg_en
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_auto_chg            References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_nav_auto_chg_en( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_AUTO_CHG_T       e_auto_chg )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_auto_chg);
    
    RPC_DO_OP("x_scc_vid_set_3d_nav_auto_chg_en");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_nav_auto_chg_en
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          
 * Outputs: 
 *          pe_auto_chg            References.
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_nav_auto_chg_en( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_AUTO_CHG_T*      pe_auto_chg )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_auto_chg);   
    
    RPC_DO_OP("x_scc_vid_get_3d_nav_auto_chg_en");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_3d_image_safety_en
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          e_img_safe             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_3d_image_safety_en( 
    HANDLE_T                    h_vid,
    SCC_3D_IMAGE_SAFETY_T       e_img_safe )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_img_safe);
    
    RPC_DO_OP("x_scc_vid_set_3d_image_safety_en");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_nav_auto_chg_en
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          
 * Outputs: 
 *          pe_img_safe            References.
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_image_safety_en( 
    HANDLE_T                    h_vid,
    SCC_3D_IMAGE_SAFETY_T*      pe_img_safe )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_img_safe);   
    
    RPC_DO_OP("x_scc_vid_get_3d_image_safety_en");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_nav_tag
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                  References.
 *          
 * Outputs: 
 *          pe_nav_tag             References.
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_nav_tag( 
    HANDLE_T                    h_vid,
    SCC_3D_NAV_TAG_T*           pe_nav_tag )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_nav_tag);   
    
    RPC_DO_OP("x_scc_vid_get_3d_nav_tag");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_pedestal
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *              b_is_on             References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_pedestal(
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_BOOL, b_is_on);

    RPC_DO_OP("x_scc_vid_set_pedestal");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_hdmi_range
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          e_hdmi_range       References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_hdmi_range( 
    HANDLE_T                    h_vid,
    SCC_HDMI_RANGE_T            e_hdmi_range )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_hdmi_range);
    
    RPC_DO_OP("x_scc_vid_set_hdmi_range");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_overscan_enable
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_overscan_enable( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,   pb_is_enable);
    
    RPC_DO_OP("x_scc_vid_get_overscan_enable");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_nonlinear_scaling_enable
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_nonlinear_scaling_enable( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable )
{ 
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,   pb_is_enable);
    
    RPC_DO_OP("x_scc_vid_get_nonlinear_scaling_enable");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_panel_type
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          pe_panel_type      References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_panel_type( 
    HANDLE_T                    h_vid,
    SCC_3D_PANEL_TYPE_T*        pe_panel_type )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_panel_type);   
    
    RPC_DO_OP("x_scc_vid_get_3d_panel_type");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_vga_detect
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          e_vga_detect       References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_vga_detect( 
    HANDLE_T                    h_vid,
    SCC_VGA_DETECT_T            e_vga_detect )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_vga_detect);
    
    RPC_DO_OP("x_scc_vid_set_vga_detect");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_vga_status
 *
 * Description: <function description>
 *
 * Inputs:  h_vid              References.
 *          pe_vga_status   References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_vga_status( 
    HANDLE_T                    h_vid,
    SCC_VGA_STATUS_T*           pe_vga_status )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_vga_status);   
    
    RPC_DO_OP("x_scc_vid_get_vga_status");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_static_gamma
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          ui1_static_gamma      References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_static_gamma( 
    HANDLE_T                    h_vid,
    UINT8                       ui1_static_gamma )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT8,  ui1_static_gamma);
    
    RPC_DO_OP("x_scc_vid_set_static_gamma");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_angular_sce
 *
 * Description: <function description>
 *
 * Inputs:  h_vid                 References.
 *          pt_angular_sce        References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                Success.
 *          SMR_INTERNAL_ERROR    Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_angular_sce( 
    HANDLE_T                    h_vid,
    SCC_ANGULAR_SCE_T*          pt_angular_sce )
{        
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_angular_sce);
    
    RPC_DO_OP("x_scc_vid_set_angular_sce");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_down_bw_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_vid               References.
 *          ui1_down_bw_mode    References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                 Success.
 *          SMR_INTERNAL_ERROR     Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_down_bw_mode( 
    HANDLE_T                    h_vid,
    UINT8                       ui1_down_bw_mode )
{    
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_UINT8,  ui1_down_bw_mode);
    
    RPC_DO_OP("x_scc_vid_set_down_bw_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_mjc_enable
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_3d_mjc_enable( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,   pb_is_enable);
    
    RPC_DO_OP("x_scc_vid_get_3d_mjc_enable");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_freq
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_freq( 
    HANDLE_T                h_vid,
    SCC_VID_FREQ_T*         pt_freq )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_freq, RPC_DESC_SCC_VID_FREQ_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_freq);   
    
    RPC_DO_OP("x_scc_vid_get_freq");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_src_detect_status
 *
 * Description: For AP query source detect status.
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_src_detect_status( 
    HANDLE_T                    h_vid,
    SCC_VID_SRC_DET_STATUS_T*   pt_src_status )
{
    RPC_DECL(2, INT32);

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_src_status, RPC_DESC_SCC_VID_SRC_DET_STATUS_T, NULL));
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_src_status);   
    
    RPC_DO_OP("x_scc_vid_get_src_detect_status");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_set_super_resolution
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_set_super_resolution( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    RPC_DECL(2, INT32);    
    
    RPC_ARG_INP(ARG_TYPE_UINT32, h_vid);
    RPC_ARG_INP(ARG_TYPE_BOOL, b_is_on);

    RPC_DO_OP("x_scc_vid_set_super_resolution");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_3d_asp_ratio_enable
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_asp_ratio_enable( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_enable )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,   pb_is_enable);
    
    RPC_DO_OP("x_scc_vid_get_asp_ratio_enable");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_vid_get_screen_mode_enable
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_vid_get_screen_mode_enable( 
    HANDLE_T                   h_vid,
    SCC_VID_SCR_MODE_T*        pt_scr_enable )
{
    RPC_DECL(2, INT32);  
    
    RPC_CHECK(RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pt_scr_enable,  sizeof(SCC_VID_SCR_MODE_T) ));
    
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_vid);
    RPC_ARG_IO(ARG_TYPE_REF_DESC,    pt_scr_enable);
    
    RPC_DO_OP("x_scc_vid_get_screen_mode_enable");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/* Display related API *******************************************************/
/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_attrib
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_attrib( HANDLE_T             h_disp,
                             SCC_DISP_ATTRIB_T    e_attrib,
                             UINT16               ui2_value )
{
    RPC_DECL(3, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_disp);
    RPC_ARG_OUT(ARG_TYPE_INT32,       e_attrib);
    RPC_ARG_OUT(ARG_TYPE_UINT16,      ui2_value);
    
    RPC_DO_OP("x_scc_disp_set_attrib");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_adp_back_light_rng
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_adp_back_light_rng( 
    HANDLE_T                       h_disp,
    const SCC_DISP_RANGE_INFO_T*   pt_range )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_range, RPC_DESC_SCC_DISP_RANGE_INFO_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,    pt_range);
    
    RPC_DO_OP("x_scc_disp_set_adp_back_light_rng");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_resolution
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_resolution( 
    HANDLE_T                       h_disp,
    const SCC_DISP_RESOLUTION_T*   pt_resolution )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_resolution, RPC_DESC_SCC_DISP_RESOLUTION_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,    pt_resolution);
    
    RPC_DO_OP("x_scc_disp_set_resolution");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_frame_rate
 *
 * Description: <function description>
 *
 * Inputs:  h_disp                   References.
 *          ui4_frame_rate           References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_frame_rate( 
    HANDLE_T                    h_disp,
    UINT32                      ui4_frame_rate )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,  h_disp);
    RPC_ARG_INP(ARG_TYPE_UINT32,  ui4_frame_rate);
    
    RPC_DO_OP("x_scc_disp_set_frame_rate");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_frame_rate
 *
 * Description: <function description>
 *
 * Inputs:  h_disp                   References.
 *          pui4_frame_rate          References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_frame_rate( 
    HANDLE_T                    h_disp,
    UINT32*                     pui4_frame_rate )
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_frame_rate);   
    
    RPC_DO_OP("x_scc_disp_get_frame_rate");
    
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_force_frame_rate
 *
 * Description: <function description>
 *
 * Inputs:  h_disp                 References.
 *          ui4_force_fr           References.
 *
 * Outputs: 
 *
 * Returns: SMR_OK                  Success.
 *          SMR_INTERNAL_ERROR      Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_force_frame_rate( 
    HANDLE_T                    h_disp,
    UINT32                      ui4_force_fr )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,  h_disp);
    RPC_ARG_INP(ARG_TYPE_UINT32,  ui4_force_fr);
    
    RPC_DO_OP("x_scc_disp_set_force_frame_rate");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_aspect_ratio
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_aspect_ratio( 
    HANDLE_T                h_disp,
    SCC_DISP_ASPECT_RATIO_T e_aspect_ratio )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_disp);
    RPC_ARG_OUT(ARG_TYPE_INT32,       e_aspect_ratio);
    
    RPC_DO_OP("x_scc_disp_set_aspect_ratio");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_fmt
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_fmt( HANDLE_T                h_disp,
                          SCC_DISP_FMT_T          e_fmt )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_disp);
    RPC_ARG_OUT(ARG_TYPE_INT32,       e_fmt);
    
    RPC_DO_OP("x_scc_disp_set_fmt");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_tv_type
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_tv_type( HANDLE_T            h_disp,
                              SCC_DISP_TV_TYPE_T  e_tv_type )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_disp);
    RPC_ARG_OUT(ARG_TYPE_INT32,       e_tv_type);
    
    RPC_DO_OP("x_scc_disp_set_tv_type");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_bg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_bg( HANDLE_T                 h_disp,
                         const SCC_BG_COLOR_T*    pt_bg )
{
    RPC_DECL(2, INT32);    
    
    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_bg, RPC_DESC_SCC_BG_COLOR_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,    pt_bg);
    
    RPC_DO_OP("x_scc_disp_set_bg");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_bg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_digit_output_fmt( 
    HANDLE_T                        h_disp,
    SCC_DISP_DIGIT_OUTPUT_FMT_T     e_digit_out_fmt )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_disp);
    RPC_ARG_OUT(ARG_TYPE_INT32,       e_digit_out_fmt);
    
    RPC_DO_OP("x_scc_disp_set_digit_output_fmt");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_color_gain
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          pt_color_gain       References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_color_gain( 
    HANDLE_T                        h_disp,
    const SCC_DISP_COLOR_GAIN_T*    pt_color_gain )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_color_gain, RPC_DESC_SCC_DISP_COLOR_GAIN_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,    pt_color_gain);
    
    RPC_DO_OP("x_scc_disp_set_color_gain");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_color_offset
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          pt_color_gain       References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_color_offset( 
    HANDLE_T                        h_disp,
    const SCC_DISP_COLOR_GAIN_T*    pt_color_offset )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_color_offset, RPC_DESC_SCC_DISP_COLOR_GAIN_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,    pt_color_offset);
    
    RPC_DO_OP("x_scc_disp_set_color_offset");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_tv_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: e_tv_mode           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_tv_mode(
    HANDLE_T                        h_disp,
    SCC_TV_MODE_T                   e_tv_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_disp);
    RPC_ARG_OUT(ARG_TYPE_INT32,       e_tv_mode);
    
    RPC_DO_OP("x_scc_disp_set_tv_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_mjc
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: b_mjc               References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_mjc(
    HANDLE_T                        h_disp,
    SCC_DISP_MJC_T                  e_mjc )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_disp);
    RPC_ARG_OUT(ARG_TYPE_INT32,       e_mjc);
    
    RPC_DO_OP("x_scc_disp_set_mjc");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_test_pattern
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *          e_pattern           References.
 *
 * Outputs: -
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_test_pattern(
    HANDLE_T                        h_disp,
    SCC_DISP_TEST_PATTERN_T         e_pattern )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_disp);
    RPC_ARG_OUT(ARG_TYPE_INT32,       e_pattern);
    
    RPC_DO_OP("x_scc_disp_set_test_pattern");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_set_back_light_ctrl
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_set_back_light_ctrl( 
    HANDLE_T                   h_vid,
    BOOL                       b_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_vid);
    RPC_ARG_OUT(ARG_TYPE_BOOL,        b_is_on);
    
    RPC_DO_OP("x_scc_disp_set_back_light_ctrl");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_back_light_ctrl
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_back_light_ctrl( 
    HANDLE_T                   h_vid,
    BOOL*                      pb_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_vid);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_is_on);

    RPC_DO_OP("x_scc_disp_get_back_light_ctrl");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_max_gamma
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_max_gamma( HANDLE_T          h_disp,
                                UINT8*            pui1_max_gamma )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_INT8,  pui1_max_gamma);

    RPC_DO_OP("x_scc_disp_get_max_gamma");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_attrib
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_attrib( HANDLE_T             h_disp,
                             SCC_DISP_ATTRIB_T    e_attrib,
                             UINT16*              pui2_value )
{
    RPC_DECL(3, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,      h_disp);
    RPC_ARG_OUT(ARG_TYPE_INT32,       e_attrib);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_value);
    
    RPC_DO_OP("x_scc_disp_get_attrib");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_adp_back_light_rng
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_adp_back_light_rng( 
    HANDLE_T                       h_disp,
    SCC_DISP_RANGE_INFO_T*         pt_range )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_range, RPC_DESC_SCC_DISP_RANGE_INFO_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_range);

    RPC_DO_OP("x_scc_disp_get_adp_back_light_rng");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_resolution
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_resolution( 
    HANDLE_T                       h_disp,
    SCC_DISP_RESOLUTION_T*         pt_resolution )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_resolution, RPC_DESC_SCC_DISP_RESOLUTION_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_resolution);

    RPC_DO_OP("x_scc_disp_get_resolution");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_aspect_ratio
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
extern INT32 c_scc_disp_get_aspect_ratio( 
    HANDLE_T                 h_disp,
    SCC_DISP_ASPECT_RATIO_T* pe_aspect_ratio )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_aspect_ratio);

    RPC_DO_OP("x_scc_disp_get_aspect_ratio");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_fmt
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_fmt( HANDLE_T                h_disp,
                          SCC_DISP_FMT_T*         pe_fmt )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_fmt);

    RPC_DO_OP("x_scc_disp_get_fmt");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_tv_type
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_tv_type( HANDLE_T            h_disp,
                              SCC_DISP_TV_TYPE_T* pe_tv_type )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_tv_type);

    RPC_DO_OP("x_scc_disp_get_tv_type");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_bg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_bg( HANDLE_T                 h_disp,
                         SCC_BG_COLOR_T*          pt_bg )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_bg, RPC_DESC_SCC_BG_COLOR_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_bg);

    RPC_DO_OP("x_scc_disp_get_bg");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_bg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_digit_output_fmt( 
    HANDLE_T                        h_disp,
    SCC_DISP_DIGIT_OUTPUT_FMT_T*    pe_digit_out_fmt )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_digit_out_fmt);

    RPC_DO_OP("x_scc_disp_get_digit_output_fmt");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_bg
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_dither_cap( 
    HANDLE_T          h_disp,
    UINT32*           pui4_dither_cap )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_dither_cap);

    RPC_DO_OP("x_scc_disp_get_dither_cap");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_fmt_cap
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_fmt_cap( HANDLE_T            h_disp,
                              SCC_DISP_FMT_CAP_T* pui4_fmt_cap )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_fmt_cap);

    RPC_DO_OP("x_scc_disp_get_fmt_cap");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_color_gain
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pt_color_gain       References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_color_gain( 
    HANDLE_T                        h_disp,
    SCC_DISP_COLOR_GAIN_T*          pt_color_gain )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_color_gain, RPC_DESC_SCC_DISP_COLOR_GAIN_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_color_gain);

    RPC_DO_OP("x_scc_disp_get_color_gain");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_color_offset
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pt_color_gain       References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_color_offset( 
    HANDLE_T                        h_disp,
    SCC_DISP_COLOR_GAIN_T*          pt_color_offset )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_color_offset, RPC_DESC_SCC_DISP_COLOR_GAIN_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_color_offset);

    RPC_DO_OP("x_scc_disp_get_color_offset");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_tv_mode
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pe_tv_mode          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_tv_mode(
    HANDLE_T                        h_disp,
    SCC_TV_MODE_T*                  pe_tv_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_tv_mode);

    RPC_DO_OP("x_scc_disp_get_tv_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_mjc
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pe_mjc              References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_mjc(
    HANDLE_T                        h_disp,
    SCC_DISP_MJC_T*                 pe_mjc )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_mjc);

    RPC_DO_OP("x_scc_disp_get_mjc");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_test_pattern
 *
 * Description: <function description>
 *
 * Inputs:  h_disp              References.
 *
 * Outputs: pe_pattern          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_test_pattern(
    HANDLE_T                        h_disp,
    SCC_DISP_TEST_PATTERN_T*        pe_pattern )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_pattern);

    RPC_DO_OP("x_scc_disp_get_test_pattern");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_disp_get_screen_resolution
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_disp_get_screen_resolution( 
    HANDLE_T                       h_disp,
    SCC_DISP_SCREEN_RESOLUTION_T*  pt_scr_resolution )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_scr_resolution, RPC_DESC_SCC_DISP_SCREEN_RESOLUTION_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_disp);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_scr_resolution);

    RPC_DO_OP("x_scc_disp_get_screen_resolution");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_query_bootup_status
 *
 * Description: <function description>
 *
 * Inputs:                References.
 *
 * Outputs:           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_query_bootup_status( UINT8  ui1_mode, UINT8* pui1_status )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT8, ui1_mode);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT8,  pui1_status);
    
    RPC_DO_OP("x_scc_query_bootup_status");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_set_crnt_inp_src_info
 *
 * Description: set current input source info
 *
 * Inputs: e_inp_src_type   References
 *            ui1_port_num     References
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_set_crnt_inp_src_info(
    SCC_CRNT_INP_SRC_TYPE_T             e_inp_src_type, 
    UINT8                               ui1_port_num)
{
    RPC_DECL(2, INT32);

    RPC_ARG_INP(ARG_TYPE_INT32, e_inp_src_type);
    RPC_ARG_INP(ARG_TYPE_UINT8, ui1_port_num);
    
    RPC_DO_OP("x_scc_set_crnt_inp_src_info");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/* Audio related API *********************************************************/
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_volume
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_volume( HANDLE_T                      h_aud,
                            const SCC_AUD_VOLUME_INFO_T*  pt_volume )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_volume, RPC_DESC_SCC_AUD_VOLUME_INFO_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_volume);

    RPC_DO_OP("x_scc_aud_set_volume");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/* Audio related API *********************************************************/
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_volume_ex
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_volume_ex( HANDLE_T                      h_aud,
                            const SCC_AUD_VOLUME_INFO_T*  pt_volume )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_volume, RPC_DESC_SCC_AUD_VOLUME_INFO_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_volume);

    RPC_DO_OP("x_scc_aud_set_volume_ex");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_mute
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_mute( HANDLE_T                            h_aud )
{
    RPC_DECL(1, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    
    RPC_DO_OP("x_scc_aud_mute");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_unmute
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_unmute( HANDLE_T                          h_aud )
{
    RPC_DECL(1, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    
    RPC_DO_OP("x_scc_aud_unmute");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_spdif_fmt
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_spdif_fmt( HANDLE_T                   h_aud,
                               SCC_AUD_SPDIF_FMT_T        e_spdif_fmt )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_spdif_fmt);
    
    RPC_DO_OP("x_scc_aud_set_spdif_fmt");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_hdcd_filter
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_hdcd_filter( HANDLE_T                 h_aud,
                                 SCC_AUD_HDCD_FLTR_T      e_hdcd_fltr )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_hdcd_fltr);
    
    RPC_DO_OP("x_scc_aud_set_hdcd_filter");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_loudspeaker
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_loudspeaker( HANDLE_T                      h_aud,
                                 const SCC_AUD_LOUD_SPEAKER_T* pt_ls_type )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_ls_type, RPC_DESC_SCC_AUD_LOUD_SPEAKER_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_ls_type);

    RPC_DO_OP("x_scc_aud_set_loudspeaker");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 

}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_trim
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_se_trim          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_trim( 
    HANDLE_T                        h_aud,
    const SCC_AUD_SE_TRIM_T*        pt_se_trim )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_se_trim, RPC_DESC_SCC_AUD_SE_TRIM_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_se_trim);

    RPC_DO_OP("x_scc_aud_set_se_trim");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_se_superbase
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_superbase( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_BOOL,   b_is_on);
    
    RPC_DO_OP("x_scc_aud_set_se_superbase");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_equalizer
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_equalizer         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_equalizer( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_EQ_T                 e_equalizer )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_equalizer);
    
    RPC_DO_OP("x_scc_aud_set_se_equalizer");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_reverb
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_reverb            References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_reverb( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_REVERB_MODE_T        e_reverb )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_reverb);
    
    RPC_DO_OP("x_scc_aud_set_se_reverb");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_base
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui1_base            References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_base( 
    HANDLE_T                        h_aud,
    UINT8                           ui1_base )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_UINT8,  ui1_base);
    
    RPC_DO_OP("x_scc_aud_set_se_base");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_treble
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui1_treble          References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_treble( 
    HANDLE_T                        h_aud,
    UINT8                           ui1_treble )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_UINT8,  ui1_treble);
    
    RPC_DO_OP("x_scc_aud_set_se_treble");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_balance
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          ui1_balance         References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_balance( 
    HANDLE_T                        h_aud,
    UINT8                           ui1_balance )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_UINT8,  ui1_balance);
    
    RPC_DO_OP("x_scc_aud_set_se_balance");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_postdr
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_postdr( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_BOOL,  b_is_on);
    
    RPC_DO_OP("x_scc_aud_set_se_postdr");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_volume
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_volume           References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_volume( 
    HANDLE_T                        h_aud,
    const SCC_AUD_VOLUME_INFO_T*    pt_volume )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_volume, RPC_DESC_SCC_AUD_VOLUME_INFO_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_volume);

    RPC_DO_OP("x_scc_aud_set_se_volume");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_surround
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_surround( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_BOOL,  b_is_on);
    
    RPC_DO_OP("x_scc_aud_set_se_surround");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_superwoofer
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_superwoofer( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_BOOL,  b_is_on);
    
    RPC_DO_OP("x_scc_aud_set_se_superwoofer");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_srs_bypass
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_srs_bypass( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_BOOL,  b_is_on);
    
    RPC_DO_OP("x_scc_aud_set_srs_bypass");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}


/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_superwoofer
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_panfade( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_BOOL,  b_is_on);
    
    RPC_DO_OP("x_scc_aud_set_panfade");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_sound_mode_start 
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          b_is_on             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_sound_mode_start( 
    HANDLE_T                        h_aud,
    BOOL                            b_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_BOOL,  b_is_on);
    
    RPC_DO_OP("x_scc_aud_set_se_sound_mode_start");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_sound_mde
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          e_sound_mode        References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_sound_mode( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_SOUND_MODE_T         e_sound_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_sound_mode);
    
    RPC_DO_OP("x_scc_aud_set_se_sound_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_surround_ex
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *          pt_surround_ex             References.
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_surround_ex( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_SURROUND_INFO_T*                pt_surround_ex)
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_surround_ex, RPC_DESC_SCC_AUD_SE_SURROUND_INFO_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_surround_ex);

    RPC_DO_OP("x_scc_aud_set_se_surround_ex");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 

}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_downmix_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_downmix_mode( HANDLE_T                h_aud,
                                  SCC_AUD_DOWNMIX_MODE_T  e_dm_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_dm_mode);
    
    RPC_DO_OP("x_scc_aud_set_downmix_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_dolby_drc
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_dolby_drc( HANDLE_T                   h_aud,
                               SCC_AUD_DOLBY_DRC_T        e_drc )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_drc);
    
    RPC_DO_OP("x_scc_aud_set_dolby_drc");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_output_port
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_output_port( HANDLE_T                 h_aud,
                                 SCC_AUD_OUT_PORT_T       e_port,
                                 BOOL                     b_is_active )
{
    RPC_DECL(3, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_port);
    RPC_ARG_INP(ARG_TYPE_BOOL,   b_is_active);
    
    RPC_DO_OP("x_scc_aud_set_output_port");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_output_port_ex
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_output_port_ex( HANDLE_T                 h_aud,
                                 UINT32                          ui4_enable_mask )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_UINT32,  ui4_enable_mask);
    
    RPC_DO_OP("x_scc_aud_set_output_port_ex");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_cmpss_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_cmpss_mode( HANDLE_T                 h_aud,
                                SCC_AUD_CMPSS_MDOE_T     e_cmpss_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_cmpss_mode);
    
    RPC_DO_OP("x_scc_aud_set_cmpss_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_agc
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_agc( HANDLE_T                 h_aud,
                         BOOL                     b_agc )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_BOOL,  b_agc);
    
    RPC_DO_OP("x_scc_aud_set_agc");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_mts
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_mts( 
    HANDLE_T                        h_aud,
    SCC_AUD_MTS_T                   e_mts )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_mts);

    RPC_DO_OP("x_scc_aud_set_mts");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_eq_user_lvl
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_eq_user_lvl( 
    HANDLE_T                        h_aud,
    const SCC_AUD_SE_QA_USER_LVL_T* pt_eq_user_lvl )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_eq_user_lvl, RPC_DESC_SCC_AUD_SE_QA_USER_LVL_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_eq_user_lvl);

    RPC_DO_OP("x_scc_aud_set_se_eq_user_lvl");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_eq_user_lvl_ex
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_eq_user_lvl_ex( 
    HANDLE_T                        h_aud,
    const SCC_AUD_SE_QA_USER_LVL_T* pt_eq_user_lvl )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_eq_user_lvl, RPC_DESC_SCC_AUD_SE_QA_USER_LVL_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_eq_user_lvl);

    RPC_DO_OP("x_scc_aud_set_se_eq_user_lvl_ex");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_chnl_delay
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_chnl_delay( HANDLE_T                  h_aud,
                                SCC_AUD_CHNL_DELAY_T*     pt_chnl_delay )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_chnl_delay, RPC_DESC_SCC_AUD_CHNL_DELAY_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_chnl_delay);

    RPC_DO_OP("x_scc_aud_set_chnl_delay");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);   
}


static INT32 _hndlr_x_scc_aud_hdphone_detect_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }   
    
    pt_return->e_type   = ARG_TYPE_VOID;
    
    ((x_scc_aud_hdphone_detect_nfy_fct)pv_cb_addr)(
                                    pt_args[0].u.pv_arg,
            (SCC_AUD_HDPHONE_COND_T)pt_args[1].u.i4_arg,
                                    pt_args[2].u.ui4_arg,
                                    pt_args[3].u.ui4_arg);

    return RPCR_OK;
}

static INT32 _hndlr_x_scc_aud_upload_data_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 3)
    {
        return RPCR_INV_ARGS;
    }   
    
    pt_return->e_type   = ARG_TYPE_VOID;
    
    ((x_scc_aud_upload_data_nfy_fct)pv_cb_addr)(
                                    pt_args[0].u.pv_arg,
                                    pt_args[1].u.ui4_arg,
                                    pt_args[2].u.ui4_arg);

    return RPCR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_hdphone_detect_nfy
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_hdphone_detect_nfy( 
    HANDLE_T                           h_aud,
    SCC_AUD_HDPHONE_DETECT_NFT_INFO_T* pt_hdph_nfy_info )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(
        RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_hdph_nfy_info, RPC_DESC_SCC_AUD_HDPHONE_DETECT_NFT_INFO_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_hdph_nfy_info);

    RPC_DO_OP("x_scc_aud_set_hdphone_detect_nfy");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_hdphone_detect_nfy
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_upload_data_nfy( 
    HANDLE_T                           h_aud,
    SCC_AUD_UPLOAD_DATA_NFT_INFO_T* pt_upload_data_nfy_info)
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(
        RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_upload_data_nfy_info, RPC_DESC_SCC_AUD_UPLOAD_DATA_NFT_INFO_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_upload_data_nfy_info);

    RPC_DO_OP("x_scc_aud_set_upload_data_nfy");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_upload_pcm_info
 *
 * Description: <function description>
 *
 * Inputs:  h_aud        Audio component handle.
 *
 * Outputs: pt_pcm_info  References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_upload_pcm_info( 
    HANDLE_T                        h_aud,
    SCC_AUD_UPLOAD_PCM_INFO_T*      pt_pcm_info )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_pcm_info, RPC_DESC_SCC_AUD_UPLOAD_PCM_INFO_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_pcm_info);

    RPC_DO_OP("x_scc_aud_get_upload_pcm_info");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_mute_ex
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_mute_ex( 
    HANDLE_T                        h_aud,
    UINT32                          ui4_muted_out_port_msk )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_UINT32,  ui4_muted_out_port_msk);

    RPC_DO_OP("x_scc_aud_set_mute_ex");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_volume
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_volume( HANDLE_T                      h_aud,
                            SCC_AUD_VOLUME_INFO_T*        pt_volume )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(
        RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_volume, RPC_DESC_SCC_AUD_VOLUME_INFO_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_volume);

    RPC_DO_OP("x_scc_aud_get_volume");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_is_muted
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_is_muted( HANDLE_T                        h_aud,
                          BOOL*                           pb_is_muted )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_is_muted);

    RPC_DO_OP("x_scc_aud_is_muted");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
    
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_spdif_fmt
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_spdif_fmt( HANDLE_T                   h_aud,
                               SCC_AUD_SPDIF_FMT_T*       pe_spdif_fmt )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_spdif_fmt);

    RPC_DO_OP("x_scc_aud_get_spdif_fmt");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_hdcd_filter
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_hdcd_filter( HANDLE_T                 h_aud,
                                 SCC_AUD_HDCD_FLTR_T*     pe_hdcd_fltr )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_hdcd_fltr);

    RPC_DO_OP("x_scc_aud_get_hdcd_filter");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_loudspeaker
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_loudspeaker( HANDLE_T                 h_aud,
                                 SCC_AUD_LOUD_SPEAKER_T*  pt_ls_type )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_ls_type, RPC_DESC_SCC_AUD_LOUD_SPEAKER_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_ls_type);

    RPC_DO_OP("x_scc_aud_get_loudspeaker");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_trim
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pt_se_trim          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_trim( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_TRIM_T*              pt_se_trim )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_se_trim, RPC_DESC_SCC_AUD_SE_TRIM_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_se_trim);

    RPC_DO_OP("x_scc_aud_get_se_trim");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_superbase
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_superbase( 
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_is_on);

    RPC_DO_OP("x_scc_aud_get_se_superbase");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_equalizer
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pt_se_trim          References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_equalizer( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_EQ_T*                pe_equalizer )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_equalizer);
    
    RPC_DO_OP("x_scc_aud_get_se_equalizer");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_reverb
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pe_reverb           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_reverb( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_REVERB_MODE_T*       pe_reverb )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_reverb);
    
    RPC_DO_OP("x_scc_aud_get_se_reverb");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_base
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pui1_base           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_base( 
    HANDLE_T                        h_aud,
    BOOL*                           pui1_base )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pui1_base);
    
    RPC_DO_OP("x_scc_aud_get_se_base");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_treble
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_treble( 
    HANDLE_T                        h_aud,
    UINT8*                          pui1_treble )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT8,  pui1_treble);
    
    RPC_DO_OP("x_scc_aud_get_se_treble");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_balance
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_balance( 
    HANDLE_T                        h_aud,
    UINT8*                          pui1_balance )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT8,  pui1_balance);
    
    RPC_DO_OP("x_scc_aud_get_se_balance");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_postdr
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_is_on            References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_postdr( 
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_is_on);
    
    RPC_DO_OP("x_scc_aud_get_se_postdr");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_volume
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pt_volume           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_volume( 
    HANDLE_T                        h_aud,
    SCC_AUD_VOLUME_INFO_T*          pt_volume )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(
        RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_volume, RPC_DESC_SCC_AUD_VOLUME_INFO_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_volume);

    RPC_DO_OP("x_scc_aud_get_se_volume");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_surround
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pt_volume           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_surround( 
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_is_on);
    
    RPC_DO_OP("x_scc_aud_get_se_surround");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_superwoofer
 *
 * Description: <function description>
 *
 * Inputs:  h_aud               References.
 *
 * Outputs: pb_is_on           References.
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_superwoofer( 
    HANDLE_T                        h_aud,
    BOOL*                           pb_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_is_on);
    
    RPC_DO_OP("x_scc_aud_get_se_superwoofer");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_downmix_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_downmix_mode( HANDLE_T                h_aud,
                                  SCC_AUD_DOWNMIX_MODE_T* pe_dm_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_dm_mode);

    RPC_DO_OP("x_scc_aud_get_downmix_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    

}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_dolby_drc
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_dolby_drc( HANDLE_T                   h_aud,
                               SCC_AUD_DOLBY_DRC_T*       pe_drc )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_drc);

    RPC_DO_OP("x_scc_aud_get_dolby_drc");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);   
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_output_port
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_output_port( HANDLE_T                 h_aud,
                                 SCC_AUD_OUT_PORT_T       e_port,
                                 BOOL*                    pb_is_active )
{
    RPC_DECL(3, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_port);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,   pb_is_active);
    
    RPC_DO_OP("x_scc_aud_get_output_port");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_cmpss_mode
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_cmpss_mode( HANDLE_T                  h_aud,
                                SCC_AUD_CMPSS_MDOE_T*     pe_cmpss_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_INT32,  pe_cmpss_mode);
    
    RPC_DO_OP("x_scc_aud_get_cmpss_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_agc
 *
 * Description: <function description>
 *
 * Inputs:  h_aud    Audio component handle.
 *
 * Outputs: pb_agc   References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_agc( HANDLE_T                         h_aud,
                         BOOL*                            pb_agc )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_agc);
    
    RPC_DO_OP("x_scc_aud_get_agc");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_chnl_delay
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pt_chnl_delay   References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_chnl_delay( HANDLE_T                  h_aud,
                                SCC_AUD_CHNL_DELAY_T*     pt_chnl_delay )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(
        RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_chnl_delay, RPC_DESC_SCC_AUD_CHNL_DELAY_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,   h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_chnl_delay);

    RPC_DO_OP("x_scc_aud_get_chnl_delay");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
    
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_max_chnl_delay
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pui1_max_delay  References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_max_chnl_delay( HANDLE_T              h_aud,
                                    UINT16*               pui2_max_delay )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT16,  pui2_max_delay);
    
    RPC_DO_OP("x_scc_aud_get_max_chnl_delay");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_mts
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pe_mts          References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_mts( 
    HANDLE_T                        h_aud,
    SCC_AUD_MTS_T*                  pe_mts )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_mts);
    
    RPC_DO_OP("x_scc_aud_get_mts");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_eq_user_lvl
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pt_eq_user_lvl  References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_eq_user_lvl( 
    HANDLE_T                        h_aud,
    SCC_AUD_SE_QA_USER_LVL_T*       pt_eq_user_lvl )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_eq_user_lvl, RPC_DESC_SCC_AUD_SE_QA_USER_LVL_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_eq_user_lvl);

    RPC_DO_OP("x_scc_aud_get_se_eq_user_lvl");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_se_eq_user_info
 *
 * Description: <function description>
 *
 * Inputs:  h_aud           Audio component handle.
 *
 * Outputs: pt_eq_user_info References
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_se_eq_user_info( 
    HANDLE_T                        h_aud,
    SCC_AU_SE_QA_USER_BAND_INFO_T*  pt_eq_user_info )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_eq_user_info, RPC_DESC_SCC_AU_SE_QA_USER_BAND_INFO_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_eq_user_info);

    RPC_DO_OP("x_scc_aud_get_se_eq_user_info");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

static INT32 _hndlr_x_scc_aud_clip_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }   
    
    pt_return->e_type   = ARG_TYPE_INT32;

    pt_return->u.i4_arg = ((x_scc_aud_clip_nfy_fct)pv_cb_addr)(
                                                  (HANDLE_T)pt_args[0].u.ui4_arg,
                                       (SCC_AUD_CLIP_COND_T)pt_args[1].u.i4_arg,
                                                            pt_args[2].u.pv_arg,
                                                            pt_args[3].u.ui4_arg,
                                                            pt_args[4].u.ui4_arg);

    return RPCR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_clip
 *
 * Description: set audio clip playback data
 *
 * Inputs:  h_aud                Audio component handle.
 *          pt_aud_clip          Audio clip data structure     
 *          pf_scc_aud_clip_nfy  SCC Audio clip notify function callback
 *          pv_nfy_tag           notify callback tag
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_clip(
    HANDLE_T                     h_aud, 
    SCC_AUD_CLIP_T*              pt_aud_clip, 
    x_scc_aud_clip_nfy_fct       pf_scc_aud_clip_nfy, 
    VOID*                        pv_nfy_tag) 
{
    RPC_DECL(4, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_aud_clip, RPC_DESC_SCC_AUD_CLIP_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_aud);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,    pt_aud_clip);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_scc_aud_clip_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,    pv_nfy_tag);    
    
    RPC_DO_OP("x_scc_aud_set_clip");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);   
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_clip_ctrl
 *
 * Description: trigger audio clip play control
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_ctrl_mode          play control mode(play/pause/stop)     
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_clip_ctrl(
    HANDLE_T                      h_aud, 
    SCC_AUD_CTRL_MODE_T           e_ctrl_mode)
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_ctrl_mode);
    
    RPC_DO_OP("x_scc_aud_set_clip_ctrl");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_clip_ctrl
 *
 * Description: trigger audio clip play control
 *
 * Inputs:  h_aud                Audio component handle.
 *
 * Outputs: pe_ctrl_mode         retrive play control mode(play/pause/stop)     
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_clip_ctrl(
    HANDLE_T                      h_aud, 
    SCC_AUD_CTRL_MODE_T*          pe_ctrl_mode)
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_ctrl_mode);
    
    RPC_DO_OP("x_scc_aud_get_clip_ctrl");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_clip_gain
 *
 * Description: The linear gain converted to a fractional fixed point unsigned integer, as described 
 *              in [BD-ROM3] part 1 I.4. The value to pass is the linear gain multiplied by 8192, and 
 *              rounded to the closest integer. 
 *
 * Inputs:  h_aud       Audio component handle.
 *
 * Outputs: ui4_gain    b0 ~b15 => fraction portion, b16~b31 => integer portion
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_clip_gain(
    HANDLE_T                      h_aud, 
    UINT32                        ui4_gain )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_UINT32,  ui4_gain);
    
    RPC_DO_OP("x_scc_aud_set_clip_gain");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_clip_duration
 *
 * Description: get audio clip duration
 *
 * Inputs:  h_aud                Audio component handle.
 *               
 *
 * Outputs: pui4_duration        retrieve the duration of the audio clip(ms)
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_clip_duration(
    HANDLE_T                       h_aud, 
    UINT32*                        pui4_duration) 
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,  pui4_duration);
    
    RPC_DO_OP("x_scc_aud_get_clip_duration");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_clip_position
 *
 * Description: get audio clip duration
 *
 * Inputs:  h_aud                Audio component handle.
 *               
 *
 * Outputs: pui4_duration        retrieve the current position of the audio clip(ms)
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_clip_position(
    HANDLE_T                        h_aud, 
    UINT32*                         pui4_position) 
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32,  pui4_position);
    
    RPC_DO_OP("x_scc_aud_get_clip_position");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_clip_position
 *
 * Description: get audio clip duration
 *
 * Inputs:  h_aud                Audio component handle.
 *               
 *
 * Outputs: ui4_duration         the current position of the audio clip(ms)
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_clip_position(
    HANDLE_T                        h_aud, 
    UINT32                          ui4_position) 
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_UINT32,  ui4_position);
    
    RPC_DO_OP("x_scc_aud_set_clip_position");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_clip_volume
 *
 * Description: 
 *
 * Inputs:  h_aud                Audio component handle.
 *               
 *             pt_volume          
 * 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_clip_volume( HANDLE_T                      h_aud,
                                const SCC_AUD_VOLUME_INFO_T*  pt_volume )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_volume, RPC_DESC_SCC_AUD_VOLUME_INFO_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_volume);

    RPC_DO_OP("x_scc_aud_set_clip_volume");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}
/*-----------------------------------------------------------------------------
 * Name: x_scc_aud_set_clip_custom_setting
 *
 * Description: 
 *
 * Inputs:  h_aud                Audio component handle.
 *               
 *             e_setting_type          
 *             e_setting_mode
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_clip_custom_setting(
    HANDLE_T                        h_aud,
    SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_T              e_setting_type,
    SCC_AUD_CLIP_CUSTOM_SETTING_MODE_T              e_setting_mode )

{
    RPC_DECL(3, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_setting_type);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_setting_mode);
    
    RPC_DO_OP("x_scc_aud_set_clip_custom_setting");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_clip_custom_setting
 *
 * Description: 
 *
 * Inputs:  h_aud                Audio component handle.
 *               
 *             e_setting_type          
 * Output:            pe_setting_mode
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_clip_custom_setting(
    HANDLE_T                        h_aud,
    SCC_AUD_CLIP_CUSTOM_SETTING_TYPE_T              e_setting_type,
    SCC_AUD_CLIP_CUSTOM_SETTING_MODE_T*              pe_setting_mode )
{
    RPC_DECL(3, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32, e_setting_type);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_setting_mode);
    
    RPC_DO_OP("x_scc_aud_get_clip_custom_setting");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_pl2_switch
 *
 * Description: set audio Pro Logic II switch
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_value               specify mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_pl2_switch( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_SWITCH_T            e_value )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_value);
    
    RPC_DO_OP("x_scc_aud_set_pl2_switch");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_pl2_mode
 *
 * Description: set audio Pro Logic II mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_value              specify mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_pl2_mode( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_MODE_T              e_value )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_value);
    
    RPC_DO_OP("x_scc_aud_set_pl2_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_pl2_panorama
 *
 * Description: set audio Pro Logic II panorama
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_value               specify mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_pl2_panorama( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_PANORAMA_T          e_value )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_value);
    
    RPC_DO_OP("x_scc_aud_set_pl2_panorama");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_pl2_dimension
 *
 * Description: set audio Pro Logic II dimension
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_value              specify mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_pl2_dimension( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_DIMENSION_T         e_value )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_value);
    
    RPC_DO_OP("x_scc_aud_set_pl2_dimension");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_pl2_c_width
 *
 * Description: set audio Pro Logic II center width
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_value              specify mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_pl2_c_width( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_C_WIDTH_T           e_value )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,  e_value);
    
    RPC_DO_OP("x_scc_aud_set_pl2_c_width");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_pl2_switch
 *
 * Description: get audio Pro Logic II switch
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_pl2_switch( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_SWITCH_T*           pe_value )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_value);
    
    RPC_DO_OP("x_scc_aud_get_pl2_switch");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_pl2_mode
 *
 * Description: get audio Pro Logic II mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_pl2_mode( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_MODE_T*             pe_value )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_value);
    
    RPC_DO_OP("x_scc_aud_get_pl2_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_pl2_panorama
 *
 * Description: get audio Pro Logic II panorama
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_pl2_panorama( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_PANORAMA_T*         pe_value )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_value);
    
    RPC_DO_OP("x_scc_aud_get_pl2_panorama");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_pl2_dimension
 *
 * Description: get audio Pro Logic II dimenstion
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_pl2_dimension( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_DIMENSION_T*        pe_value )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_value);
    
    RPC_DO_OP("x_scc_aud_get_pl2_dimension");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_pl2_c_width
 *
 * Description: get audio Pro Logic II center width
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_value             retrive mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_pl2_c_width( 
    HANDLE_T                        h_aud, 
    SCC_AUD_PL2_C_WIDTH_T*          pe_value )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_value);
    
    RPC_DO_OP("x_scc_aud_get_pl2_c_width");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_dolby_certi_mode
 *
 * Description: get dolby certify mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          pb_mode              retrive mode
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_dolby_certi_mode( 
   HANDLE_T                        h_aud,
   BOOL*                           pb_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,  pb_mode);
    
    RPC_DO_OP("x_scc_aud_get_dolby_certi_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_dolby_certi_mode
 *
 * Description: set dolby certify mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          b_mode               set mode value
 *               
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_dolby_certi_mode( 
    HANDLE_T                        h_aud,
    BOOL                            b_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_BOOL,      b_mode);

    RPC_DO_OP("x_scc_aud_set_dolby_certi_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_output_port_adj
 *
 * Description: set output port whether is adjust or not
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_port               specify set outport  
 *          b_is_fixed           specify is fixed or not for the port
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_output_port_adj( 
    HANDLE_T                        h_aud,
    SCC_AUD_OUT_PORT_T              e_port,
    BOOL                            b_is_fixed )
{
    RPC_DECL(3, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,      e_port);
    RPC_ARG_INP(ARG_TYPE_BOOL,      b_is_fixed);

    RPC_DO_OP("x_scc_aud_set_output_port_adj");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_output_port_adj
 *
 * Description: get output port whether is adjust or not
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_port               specify set outport  
 *
 * Outputs: 
 *          pb_is_fixed          retrive is fixed or not for the port
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_output_port_adj( 
    HANDLE_T                        h_aud,
    SCC_AUD_OUT_PORT_T              e_port,
    BOOL*                           pb_is_fixed )
{
    RPC_DECL(3, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,      e_port);
    RPC_ARG_OUT(ARG_TYPE_REF_BOOL,      pb_is_fixed);

    RPC_DO_OP("x_scc_aud_get_output_port_adj");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_capability
 *
 * Description: get audio capability
 *
 * Inputs:  h_aud                Audio component handle.
 *          pt_capability        retrive the audio capability
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_capability( 
    HANDLE_T                        h_aud,
    SCC_AUD_CAPABILITY_T*           pt_capability )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_capability, RPC_DESC_SCC_AUD_CAPABILITY_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_capability);

    RPC_DO_OP("x_scc_aud_get_capability");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_endian_mode
 *
 * Description: set enduan mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_mode               set endian.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_endian_mode( 
    HANDLE_T                        h_aud,
    SCC_AUD_ENDIAN_MODE_T              e_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,      e_mode);

    RPC_DO_OP("x_scc_aud_set_endian_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_out_device
 *
 * Description: set audio out device
 *
 * Inputs:  h_aud                Audio component handle.
 *            e_out_device       set endian.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_out_device( 
    HANDLE_T                          h_aud,
    SCC_AUD_OUT_DEVICE_T              e_out_device )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,      e_out_device);

    RPC_DO_OP("x_scc_aud_set_out_device");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_bbe_mode
 *
 * Description: set bbe mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          e_mode               set bbe mode.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_bbe_mode( 
    HANDLE_T                        h_aud,
    SCC_AUD_BBE_MODE_T              e_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32,      e_mode);

    RPC_DO_OP("x_scc_aud_set_bbe_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_bbe_mode
 *
 * Description: set bbe mode
 *
 * Inputs:  h_aud                Audio component handle.
 *          pe_mode              get bbe mode.
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_get_bbe_mode( 
    HANDLE_T                        h_aud,
    SCC_AUD_BBE_MODE_T*             pe_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32,      pe_mode);

    RPC_DO_OP("x_scc_aud_get_bbe_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_hdphone_detect_nfy
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_hdphone_detect_nfy( 
    HANDLE_T                           h_aud,
    SCC_AUD_HDPHONE_DETECT_NFT_INFO_T* pt_hdph_nfy_info )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(
        RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_hdph_nfy_info, RPC_DESC_SCC_AUD_HDPHONE_DETECT_NFT_INFO_T, NULL));

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_hdph_nfy_info);

    RPC_DO_OP("x_scc_aud_get_hdphone_detect_nfy");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
}
    
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_hdphone_cond
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_hdphone_cond( 
    HANDLE_T                        h_aud,
    SCC_AUD_HDPHONE_COND_T*         pe_hdphone_cond )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_INT32, pe_hdphone_cond);

    RPC_DO_OP("x_scc_aud_get_hdphone_cond");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);       
}

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_mute_ex
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_mute_ex( 
    HANDLE_T                        h_aud,
    UINT32*                         pui4_muted_out_port_msk )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, pui4_muted_out_port_msk);

    RPC_DO_OP("x_scc_aud_get_mute_ex");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
} 
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_lineout_vol_mode
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.    
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_lineout_vol_mode( 
    HANDLE_T                        h_aud,
     SCC_AUD_DEC_LINEOUT_VOL_T*  pt_lineout_vol_mode)
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_lineout_vol_mode, RPC_DESC_SCC_AUD_DEC_LINEOUT_VOL_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_lineout_vol_mode);

    RPC_DO_OP("x_scc_aud_set_lineout_vol_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
} 
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_se_sph_mode
 *
 * Description: 
 *
 * Inputs:  
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.    
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_se_sph_mode(
    HANDLE_T                        h_aud,
    SCC_AUD_DEC_SE_SPH_MODE_T        e_mode )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_INT32, e_mode);

    RPC_DO_OP("x_scc_aud_set_se_sph_mode");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_set_se_volume_offset
  *
  * Description: 
  *
  * Inputs:  
  *
  * Outputs: 
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.    
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_se_volume_offset(
     HANDLE_T                        h_aud,
     UINT8                           ui1_offset )
 {
     RPC_DECL(2, INT32);    
     
     RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
     RPC_ARG_INP(ARG_TYPE_UINT8, ui1_offset);
     
     RPC_DO_OP("x_scc_aud_set_se_volume_offset");
     
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
 }


  /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_set_se_hp_volume_offset
  *
  * Description: 
  *
  * Inputs:  
  *
  * Outputs: 
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.    
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_se_hp_volume_offset(
     HANDLE_T                        h_aud,
     UINT8                           ui1_offset )
 {
     RPC_DECL(2, INT32);    
     
     RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
     RPC_ARG_INP(ARG_TYPE_UINT8, ui1_offset);
     
     RPC_DO_OP("x_scc_aud_set_se_hp_volume_offset");
     
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
 }

 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_set_peq_info
  *
  * Description: 
  *
  * Inputs:  
  *
  * Outputs: 
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.    
  *
  ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_peq_info( 
    HANDLE_T                 h_aud,
   SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T*  pt_peq_info)
{
     RPC_DECL(2, INT32);    
     
     RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_peq_info, RPC_DESC_SCC_AUD_DEC_SET_TYPE_PEQ_INFO_T, NULL));
     RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
     RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_peq_info);
     
     RPC_DO_OP("x_scc_aud_set_peq_info");
     
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
} 
 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_set_ovm
  *
  * Description: 
  *
  * Inputs:  
  *
  * Outputs: 
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.    
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_ovm(
     HANDLE_T                        h_aud,
     UINT32                           ui4_value )
{
     RPC_DECL(2, INT32);    

     RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
     RPC_ARG_INP(ARG_TYPE_UINT32, ui4_value);
     
     RPC_DO_OP("x_scc_aud_set_ovm");
     
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}
 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_set_btnsnd_mode
  *
  * Description: <function description>
  *
  * Inputs:  h_aud               References.
  *          e_btnsnd_mode             References.
  *
  * Outputs:
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_btnsnd_mode( 
     HANDLE_T                        h_aud,
     SCC_AUD_BTNSND_MODE_T                            e_btnsnd_mode )
 {
     RPC_DECL(2, INT32);    
 
     RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
     RPC_ARG_INP(ARG_TYPE_INT32,   e_btnsnd_mode);
     
     RPC_DO_OP("x_scc_aud_set_btnsnd_mode");
 
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
 }
 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_get_btnsnd_mode
  *
  * Description: get audio button sound  mode
  *
  * Inputs:  h_aud                Audio component handle.
  *          pe_btnsnd_mode             retrive mode value
  *               
  *
  * Outputs: 
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_get_btnsnd_mode( 
     HANDLE_T                        h_aud, 
     SCC_AUD_BTNSND_MODE_T*             pe_btnsnd_mode )
 {
     RPC_DECL(2, INT32);    
 
     RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
     RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_btnsnd_mode);
     
     RPC_DO_OP("x_scc_aud_get_btnsnd_mode");
 
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
 }
 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_set_btnsnd_play
  *
  * Description: <function description>
  *
  * Inputs:  h_aud               References.
  *
  * Outputs:
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_btnsnd_play( 
     HANDLE_T                        h_aud)
 {
     RPC_DECL(1, INT32);    
 
     RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
     
     RPC_DO_OP("x_scc_aud_set_btnsnd_play");
 
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
 }
 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_set_btnsnd_enable
  *
  * Description: <function description>
  *
  * Inputs:  h_aud               References.
  *          e_btnsnd_enable             References.
  *
  * Outputs:
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_btnsnd_enable( 
     HANDLE_T                        h_aud,
     SCC_AUD_BTNSND_ENABLE_T                            e_btnsnd_enable )
 {
     RPC_DECL(2, INT32);    
 
     RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
     RPC_ARG_INP(ARG_TYPE_INT32,   e_btnsnd_enable);
     
     RPC_DO_OP("x_scc_aud_set_btnsnd_enable");
 
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
 }
 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_get_btnsnd_enable
  *
  * Description: get audio button sound  mode
  *
  * Inputs:  h_aud                Audio component handle.
  *          pe_btnsnd_enable             retrive enable value
  *               
  *
  * Outputs: 
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_get_btnsnd_enable( 
     HANDLE_T                        h_aud, 
     SCC_AUD_BTNSND_ENABLE_T*             pe_btnsnd_enable )
 {
     RPC_DECL(2, INT32);    
 
     RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
     RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_btnsnd_enable);
     
     RPC_DO_OP("x_scc_aud_get_btnsnd_enable");
 
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
 }
 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_get_cmpss_drc
  *
  * Description: get audio dynamic dolby range
  *
  * Inputs:  h_aud                Audio component handle.
  *          pe_cmpss_drc             retrive drc mode
  *               
  *
  * Outputs: 
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_get_cmpss_drc( 
     HANDLE_T                        h_aud, 
     SCC_AUD_CMPSS_DRC_MODE_T*             pe_cmpss_drc )
 {
     RPC_DECL(2, INT32);    
 
     RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
     RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_cmpss_drc);
     
     RPC_DO_OP("x_scc_aud_get_cmpss_drc");
 
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
 }
 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_set_cmpss_drc
  *
  * Description: <function description>
  *
  * Inputs:  h_aud               References.
  *          e_cmpss_drc             References.
  *
  * Outputs:
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_cmpss_drc( 
     HANDLE_T                        h_aud,
     SCC_AUD_CMPSS_DRC_MODE_T                            e_cmpss_drc )
 {
     RPC_DECL(2, INT32);    
 
     RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
     RPC_ARG_INP(ARG_TYPE_INT32,   e_cmpss_drc);
     
     RPC_DO_OP("x_scc_aud_set_cmpss_drc");
 
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
 }
 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_set_mpeg_level_adj
  *
  * Description: <function description>
  *
  * Inputs:
  *
  * Outputs:
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_mpeg_level_adj(
     HANDLE_T                        h_aud,
     INT8                            i1_level )
 {
     RPC_DECL(2, INT32);    
     
     RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
     RPC_ARG_INP(ARG_TYPE_INT8,  i1_level);
     
     RPC_DO_OP("x_scc_aud_set_mpeg_level_adj");
     
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
 }
 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_get_mpeg_level_adj
  *
  * Description: <function description>
  *
  * Inputs:
  *
  * Outputs:
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_get_mpeg_level_adj( 
     HANDLE_T                    h_aud,
     INT8*                       pi1_level )
 {
     RPC_DECL(2, INT32);
     
     RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
     RPC_ARG_OUT(ARG_TYPE_REF_INT8, pi1_level);
     
     RPC_DO_OP("x_scc_aud_get_mpeg_level_adj");
     
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
 }
 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_set_av_sync_mode
  *
  * Description: <function description>
  *
  * Inputs:  h_aud               References.
  *          e_av_sync_mode             References.
  *
  * Outputs:
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_set_av_sync_mode( HANDLE_T                  h_aud,
                                  SCC_AUD_DEC_AV_SYNC_INFO_T             e_av_sync_mode )
 {
     RPC_DECL(2, INT32);    
 
     RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
     RPC_ARG_INP(ARG_TYPE_INT32,   e_av_sync_mode);
     
     RPC_DO_OP("x_scc_aud_set_av_sync_mode");
 
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
 }
 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_get_av_sync_mode
  *
  * Description: <function description>
  *
  * Inputs:  h_aud                References
  *          pe_av_sync_mode             References
  *               
  *
  * Outputs: 
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_get_av_sync_mode(
             HANDLE_T                        h_aud,
             SCC_AUD_DEC_AV_SYNC_INFO_T*             pe_av_sync_mode )
 {
     RPC_DECL(2, INT32);    
 
     RPC_ARG_INP(ARG_TYPE_UINT32, h_aud);
     RPC_ARG_OUT(ARG_TYPE_REF_INT32,  pe_av_sync_mode);
     
     RPC_DO_OP("x_scc_aud_get_av_sync_mode");
 
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);
 }

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_karaoke
 *
 * Description: <function description>
 *
 * Inputs:
 *
 * Outputs:
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_karaoke(
        HANDLE_T                       h_aud,
        SCC_AUD_KARAOKE_INFO_T*        pt_karaoke_info )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_karaoke_info, RPC_DESC_SCC_AUD_KARAOKE_INFO_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,  pt_karaoke_info);

    RPC_DO_OP("x_scc_aud_set_karaoke");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}
 
 /*-----------------------------------------------------------------------------
  * Name: c_scc_aud_get_volume
  *
  * Description: <function description>
  *
  * Inputs:
  *
  * Outputs:
  *
  * Returns: SMR_OK              Success.
  *          SMR_INTERNAL_ERROR  Fail.
  *
  ----------------------------------------------------------------------------*/
 INT32 c_scc_aud_get_power_on_off_music_volume( HANDLE_T                      h_aud,
                             SCC_AUD_VOLUME_INFO_T*        pt_volume )
 {
     RPC_DECL(2, INT32);    
 
     RPC_CHECK(
         RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_volume, RPC_DESC_SCC_AUD_VOLUME_INFO_T, NULL));
 
     RPC_ARG_INP(ARG_TYPE_UINT32,   h_aud);
     RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_volume);
 
     RPC_DO_OP("x_scc_aud_get_power_on_off_music_volume");
 
     RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
 }
 

/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_set_upload_ctrl
 *
 * Description: set upload data enable or not
 *
 * Inputs:  h_aud                Audio component handle.
 *          b_is_on           specify enable or not
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_set_upload_ctrl( 
    HANDLE_T                        h_aud,
    BOOL                               b_is_on )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_INP(ARG_TYPE_BOOL,      b_is_on);

    RPC_DO_OP("x_scc_aud_set_upload_ctrl");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}
/*-----------------------------------------------------------------------------
 * Name: c_scc_aud_get_upload_data_buffer_info
 *
 * Description: 
 *
 * Inputs:  
 *          
 *
 * Outputs: 
 *
 * Returns: SMR_OK              Success.
 *          SMR_INTERNAL_ERROR  Fail.
 *
 ----------------------------------------------------------------------------*/
INT32 c_scc_aud_get_upload_data_buffer_info(
            HANDLE_T                        h_aud,
            SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T*           pt_buffer_info )
{
    RPC_DECL(2, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_buffer_info, RPC_DESC_SCC_AUD_UPLOAD_DATA_BUFFER_INFO_T, NULL));
    RPC_ARG_INP(ARG_TYPE_UINT32,    h_aud);
    RPC_ARG_OUT(ARG_TYPE_REF_DESC,  pt_buffer_info);

    RPC_DO_OP("x_scc_aud_get_upload_data_buffer_info");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG); 
}
/* SM Start here */

INT32 c_sm_cc_hdlr_init(const SM_CCH_CONFIG_T* pt_config )
{
    RPC_DECL(1, INT32);    

    RPC_CHECK(
        RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_config, RPC_DESC_SM_CCH_CONFIG_T, NULL));

    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_config);

    RPC_DO_OP("x_sm_cc_hdlr_init");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
}

INT32 c_sm_cc_hdlr_set_dtv_cc_att(const SM_CCH_DTVCC_ATTRIBS_INFO_T* pt_dtvcc_att)
{
    RPC_DECL(1, INT32);    

    RPC_CHECK(
        RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_dtvcc_att, RPC_DESC_SM_CCH_DTVCC_ATTRIBS_INFO_T, NULL));

    RPC_ARG_OUT(ARG_TYPE_REF_DESC, pt_dtvcc_att);

    RPC_DO_OP("x_sm_cc_hdlr_set_dtv_cc_att");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}

static INT32 _hndlr_x_sess_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 5)
    {
        return RPCR_INV_ARGS;
    }   
    
    pt_return->e_type   = ARG_TYPE_INT32;

    pt_return->u.i4_arg = ((x_sess_nfy_fct)pv_cb_addr)(
                                            (HANDLE_T      )pt_args[0].u.ui4_arg,
                                            (SM_SESS_COND_T)pt_args[1].u.ui4_arg,
                                                            pt_args[2].u.pv_arg,
                                            (SM_EVN_TYPE_T )pt_args[3].u.ui4_arg,
                                                            pt_args[4].u.ui4_arg);

    return RPCR_OK;
}

INT32 c_sm_open_sess( HANDLE_T              h_connection,
                      const CHAR*           ps_sess_name,
                      const CHAR*           ps_dest_grp_name,
                      VOID*                 pv_sess_data,
                      SIZE_T                z_sess_data_size,
                      INT32                 i4_speed,
                      x_sess_nfy_fct        pf_sess_nfy,
                      VOID*                 pv_sess_nfy_tag,
                      HANDLE_T*             ph_sess )
{
    RPC_DECL(9, INT32);    

    RPC_CHECK(
        RPC_ADD_REF_BUFF(RPC_DEFAULT_ID, pv_sess_data, z_sess_data_size));

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_connection);
    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_sess_name);
    RPC_ARG_INP(ARG_TYPE_REF_STR,    ps_dest_grp_name);    
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pv_sess_data);
    RPC_ARG_INP(ARG_TYPE_SIZE_T,     z_sess_data_size);
    RPC_ARG_INP(ARG_TYPE_INT32,      i4_speed);
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_sess_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_sess_nfy_tag);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_sess);
    
    RPC_DO_OP("x_sm_open_sess");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
}

INT32 c_sm_close_sess( HANDLE_T             h_sess )
{
    RPC_DECL(1, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_sess);
    
    RPC_DO_OP("x_sm_close_sess");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
}

INT32 c_sm_play_sess( HANDLE_T             h_sess,
                      INT32                i4_speed )
{
    RPC_DECL(2, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_sess);
    RPC_ARG_INP(ARG_TYPE_INT32,     i4_speed);
    
    RPC_DO_OP("x_sm_play_sess");
    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);  
}
                      
INT32 c_sm_stop_sess( HANDLE_T              h_sess )
{
    RPC_DECL(1, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_sess);
    
    RPC_DO_OP("x_sm_stop_sess");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
}

INT32 c_sm_open_stream( HANDLE_T            h_sess,
                        const STREAM_COMP_ID_T* pt_strm_comp_id,
                        SM_COMMAND_T*           pt_cmd,
                        x_stream_nfy_fct        pf_strm_nfy,
                        VOID*                   pv_strm_nfy_tag,
                        BOOL                    b_auto_play,
                        HANDLE_T*               ph_stream ) 
{
    RPC_DECL(7, INT32);    

    RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_strm_comp_id, RPC_DESC_STREAM_COMP_ID_T, NULL));
    RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_strm_comp_id->pv_stream_tag));    
    _set_cmd_array(pt_cmd);

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_sess);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_strm_comp_id);
    RPC_ARG_INP(ARG_TYPE_REF_DESC,   pt_cmd);    
    RPC_ARG_INP(ARG_TYPE_REF_FUNC,   pf_strm_nfy);
    RPC_ARG_INP(ARG_TYPE_REF_VOID,   pv_strm_nfy_tag);
    RPC_ARG_INP(ARG_TYPE_BOOL,       b_auto_play);
    RPC_ARG_OUT(ARG_TYPE_REF_UINT32, ph_stream);
    
    RPC_DO_OP("x_sm_open_stream");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
}

INT32 c_sm_close_stream( HANDLE_T h_stream )
{
    RPC_DECL(1, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_stream);
    
    RPC_DO_OP("x_sm_close_stream");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
}

INT32 c_sm_play_stream( HANDLE_T h_stream )
{
    RPC_DECL(1, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_stream);
    
    RPC_DO_OP("x_sm_play_stream");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
}

INT32 c_sm_stop_stream( HANDLE_T h_stream )
{
    RPC_DECL(1, INT32);    

    RPC_ARG_INP(ARG_TYPE_UINT32,     h_stream);
    
    RPC_DO_OP("x_sm_stop_stream");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
}

static INT32 _hndlr_x_mm_range_nfy_fct(
    RPC_ID_T     t_rpc_id,
    const CHAR*  ps_cb_type,
    VOID *       pv_cb_addr,
    UINT32       ui4_num_args,
    ARG_DESC_T*  pt_args,
    ARG_DESC_T*  pt_return)
{  
    if(ui4_num_args != 4)
    {
        return RPCR_INV_ARGS;
    }   
    
    pt_return->e_type   = ARG_TYPE_BOOL;
    
    pt_return->u.b_arg  = ((x_mm_range_nfy_fct)pv_cb_addr)(pt_args[0].u.pv_arg,
             (SCC_AUD_HDPHONE_COND_T)pt_args[1].u.i4_arg,
                                     pt_args[2].u.ui4_arg,
                                     pt_args[3].u.ui4_arg);

    return RPCR_OK;
}

INT32 c_sm_set( HANDLE_T            h_sm_handle,
                SM_SET_TYPE_T       e_set_type,
                VOID*               pv_set_info,
                SIZE_T              z_set_info_size )
{
    RPC_DECL(4, INT32);    

    switch(e_set_type)
    {
        case SM_SESS_GNRC_SET_TYPE_SPEED:
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_INT32));
            break;

        case SM_SESS_GNRC_SET_TYPE_ADD_RANGE:
        {
            MM_RANGE_INFO_T*  pt_range_info = (MM_RANGE_INFO_T*) pv_set_info;
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_REF_DESC));
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, 
                                       pt_range_info,
                                       RPC_DESC_MM_RANGE_INFO_T,
                                       NULL));
            RPC_CHECK(RPC_ADD_REF_VAL(RPC_DEFAULT_ID, &pt_range_info->pv_tag));
            /* pt_idxtbl_lst ignored for now (FIXME) */
            /*RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, &pt_range_info->pt_track_tm_scale_lst, RPC_DESC_MM_RANGE_TRACK_TM_SCALE_T, NULL));*/
        }
        break;
        
        /* Uncomment this for new version */
        case SM_VSH_SET_TYPE_FLUSH:
        {
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_BOOL));
        }
            break;        
        
        case SM_VSH_SET_TYPE_START_RENDER_INFO:
        {
            SM_VSH_START_RENDER_INFO_T * pt_vsh_info = pv_set_info;            
            RPC_CHECK(RPC_ADD_ARG_TYPE(RPC_DEFAULT_ID, 2, ARG_TYPE_REF_DESC));
            RPC_CHECK(RPC_ADD_REF_DESC(RPC_DEFAULT_ID, pt_vsh_info, RPC_DESC_SM_VSH_START_RENDER_INFO_T, NULL));
        }
        break;
        /* end of new version */
        
        default:
            RI_LOG("c_sm_set specify unkown type %d, 0x%x\n", e_set_type, pv_set_info);
            
        
    }
   
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_sm_handle);
    RPC_ARG_INP(ARG_TYPE_UINT32,     e_set_type);
    RPC_ARG_INP(ARG_TYPE_VARIABLE,   pv_set_info);    
    RPC_ARG_INP(ARG_TYPE_SIZE_T,     z_set_info_size);
    
    RPC_DO_OP("x_sm_set");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);    
}

INT32 c_sm_get( HANDLE_T            h_sm_handle,                           
                SM_GET_TYPE_T       e_get_type,                         
                VOID*               pv_get_info,            /* IN/OUT */   
                SIZE_T*             pui4_get_info_size )    /* IN/OUT */
{                                                      
    RPC_DECL(4, INT32);    
   
    RPC_ARG_INP(ARG_TYPE_UINT32,     h_sm_handle);
    RPC_ARG_INP(ARG_TYPE_UINT32,     e_get_type);
    RPC_ARG_IO (ARG_TYPE_REF_DESC,   pv_get_info);    
    RPC_ARG_IO (ARG_TYPE_REF_SIZE_T, pui4_get_info_size);
    
    RPC_DO_OP("x_sm_get");

    RPC_RETURN(ARG_TYPE_INT32, SMR_INV_ARG);   
}

INT32 c_rpc_reg_sm_cb_hndlrs()
{
    RPC_REG_CB_HNDLR(x_scc_aud_hdphone_detect_nfy_fct); 
    RPC_REG_CB_HNDLR(x_mm_range_nfy_fct);     
    RPC_REG_CB_HNDLR(x_sess_nfy_fct); 
    RPC_REG_CB_HNDLR(x_scc_aud_clip_nfy_fct); 
    RPC_REG_CB_HNDLR(x_scc_aud_upload_data_nfy_fct); 
    RPC_REG_CB_HNDLR(x_scc_vid_src_detect_nfy_fct); 

    return RPCR_OK;
}

