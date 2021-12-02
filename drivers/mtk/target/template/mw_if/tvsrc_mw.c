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
 * $RCSfile: tvsrc_mw.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/14 $
 * $SWAuthor: Clear Case Administrator $
 * $MD5HEX: a872122d22ab4b982b54b29e95d0ff55 $
 *
 * Description:
 *         AV Source middleware driver API.
 *---------------------------------------------------------------------------*/ 
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_assert.h"
#include "source_table.h"
#include "x_avc.h"
#include "x_dbg.h"

#include "api_video.h"
#include "video_def.h"
#include "drv_video.h"
#include "drv_scart.h"
#include "sv_const.h"
#include "drv_adcset.h"
#include "mw_comp_excl_reg.h"
#include "d_drv_cust.h"
#include "d_drv_cust_util.h"
#include "tvsrc_mw.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define AVC_DBG_STMT( n )     { if ( b_avc_dbg ) { UNUSED(x_dbg_stmt n); } }

#define AVC_COMP_LIST_NUM_MAX      ((SIZE_T) 48)
#define SRC_COMP_NS                (SV_VS_MAX+1)

#define NPTV_VID_SRC_MAPPING_TBL_END             { (UINT32)SV_VS_MAX, DRVT_UNKNOWN, 0 }
#define IS_NPTV_VID_SRC_MAPPING_TBL_END(pt_elm)   \
(                                                 \
    ((pt_elm)->e_vid_src_idx == (UINT32)SV_VS_MAX) &&     \
    ((pt_elm)->e_type == DRVT_UNKNOWN) &&         \
    ((pt_elm)->ui2_comp_id == 0)                  \
)

#define AVC_IS_TUNER_SRC( e_type )   ( (e_type == DRVT_TUNER_SAT_ANA) ||         \
                                       (e_type == DRVT_TUNER_CAB_ANA) ||         \
                                       (e_type == DRVT_TUNER_TER_ANA) )
                                     
typedef struct _NPTV_VID_SRC_MAPPING_TBL_T
{
    UINT32             e_vid_src_idx;
    DRV_TYPE_T         e_type;
    UINT16             ui2_comp_id;
}   NPTV_VID_SRC_MAPPING_TBL_T;

typedef struct _AVC_COMBI_MW_NFY_INFO_T
{
    avc_combi_nfy_fct   pf_combi_nfy;
    VOID*               pv_tag;
}   AVC_COMBI_MW_NFY_INFO_T;

/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/
static BOOL                 b_avc_dbg = FALSE;
static AVC_SCART_INFO_T     t_scart_info = {FALSE, 0, 0, 0, 0, AVC_SCART_BYPASS_TRUE};

/* NPTV defined video source mapping table */
static NPTV_VID_SRC_MAPPING_TBL_T at_vid_src_mapping_tbl[] =
{
    { (UINT32)SV_VS_TUNER1, DRVT_TUNER_CAB_ANA,  0 },
    { (UINT32)SV_VS_TUNER1, DRVT_TUNER_TER_ANA,  0 },
    { (UINT32)SV_VS_CVBS1,  DRVT_AVC_COMP_VIDEO, 0 },
    { (UINT32)SV_VS_CVBS2,  DRVT_AVC_COMP_VIDEO, 1 },
    { (UINT32)SV_VS_CVBS3,  DRVT_AVC_COMP_VIDEO, 2 },
    { (UINT32)SV_VS_S1,     DRVT_AVC_S_VIDEO,    0 },
    { (UINT32)SV_VS_S2,     DRVT_AVC_S_VIDEO,    1 },
    { (UINT32)SV_VS_S3,     DRVT_AVC_S_VIDEO,    2 },
    { (UINT32)SV_VS_YPbPr1, DRVT_AVC_Y_PB_PR,    0 },
    { (UINT32)SV_VS_YPbPr2, DRVT_AVC_Y_PB_PR,    1 },
    { (UINT32)SV_VS_YPbPr3, DRVT_AVC_Y_PB_PR,    2 },
    { (UINT32)SV_VS_YPbPr4, DRVT_AVC_Y_PB_PR,    3 },
    { (UINT32)SV_VS_VGACOMP1,DRVT_AVC_Y_PB_PR,   4 },
    { (UINT32)SV_VS_HDMI1,  DRVT_AVC_HDMI,       0 },
    { (UINT32)SV_VS_HDMI2,  DRVT_AVC_HDMI,       1 },
    { (UINT32)SV_VS_HDMI3,  DRVT_AVC_HDMI,       2 },
    { (UINT32)SV_VS_HDMI4,  DRVT_AVC_HDMI,       3 },
    { (UINT32)SV_VS_HDMI5,  DRVT_AVC_HDMI,       4 },
    { (UINT32)SV_VS_VGA,    DRVT_AVC_VGA,        0 },
    { (UINT32)SV_VS_DVI,    DRVT_AVC_DVI,        0 },
    { (UINT32)SV_VS_SCART1, DRVT_AVC_SCART,      (AVC_SCART_TYPE_SOURCE + 0) },
    { (UINT32)SV_VS_SCART1, DRVT_AVC_SCART,      (AVC_SCART_TYPE_SINK + 0) },
    { (UINT32)SV_VS_SCART1, DRVT_AVC_SCART,      (AVC_SCART_TYPE_SOURCE_SINK + 0) },    
    { (UINT32)SV_VS_SCART1, DRVT_AVC_SCART,      (AVC_SCART_TYPE_MONITOR + 0) },
    { (UINT32)SV_VS_SCART2, DRVT_AVC_SCART,      (AVC_SCART_TYPE_SOURCE + 1) },
    { (UINT32)SV_VS_SCART2, DRVT_AVC_SCART,      (AVC_SCART_TYPE_SINK + 1) },   
    { (UINT32)SV_VS_SCART2, DRVT_AVC_SCART,      (AVC_SCART_TYPE_SOURCE_SINK + 1) },        
    { (UINT32)SV_VS_SCART2, DRVT_AVC_SCART,      (AVC_SCART_TYPE_MONITOR + 1) },    

    /* DO NOT delete this */
    NPTV_VID_SRC_MAPPING_TBL_END
};

/* Notify info */
static AVC_NFY_INFO_T at_avc_nfy_info[AVC_COMP_LIST_NUM_MAX];

/* COMBI Nofity info */
static AVC_COMBI_MW_NFY_INFO_T at_avc_combi_mw_nfy_info[AVC_COMP_LIST_NUM_MAX];

/* Connection notification tags */
static VOID* apf_rm_conn_tag[AVC_COMP_LIST_NUM_MAX];

static DRV_ID_T at_avc_comp_list[AVC_COMP_LIST_NUM_MAX] =
{
    DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END,
    DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END,
    DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END,
    DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END,
    DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END,
    DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END,
    DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END,
    DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END,
    DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END, DRV_ID_END,
    DRV_ID_END, DRV_ID_END, DRV_ID_END
};

static BOOL ab_avc_comp_no_disc_list[AVC_COMP_LIST_NUM_MAX] =
{
    FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE, FALSE, FALSE,
    FALSE, FALSE, FALSE
};

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern BOOL fgHDMISupportAudio(void);

/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _get_avc_comp_list_idx
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/ 
static UINT8 _get_avc_comp_list_idx( DRV_TYPE_T e_type, UINT16 ui2_comp_id )
{
    UINT8 ui1_idx;
    
    for(ui1_idx = 0; !IS_DRV_ID_END((&at_avc_comp_list[ui1_idx])); ui1_idx++)
    {
        if( (at_avc_comp_list[ui1_idx].e_type == e_type) && 
            (at_avc_comp_list[ui1_idx].ui2_id == ui2_comp_id) )
        {
            return (ui1_idx);
        }
    }
    
    return (0xFF);
}


/*-----------------------------------------------------------------------------
 * Name: _add_avc_comp_list
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/ 
static BOOL _add_avc_comp_list( DRV_TYPE_T e_type, UINT16 ui2_comp_id )
{
    UINT8 ui1_idx;
    
    for(ui1_idx = 0; !IS_DRV_ID_END((&at_avc_comp_list[ui1_idx])); ui1_idx++)
    {
        if( (at_avc_comp_list[ui1_idx].e_type == e_type) && 
            (at_avc_comp_list[ui1_idx].ui2_id == ui2_comp_id) )
        {
            return FALSE;
        }
    }
    
    if ( ui1_idx >= AVC_COMP_LIST_NUM_MAX )
    {
        return FALSE;
    }
    
    at_avc_comp_list[ui1_idx].e_type = e_type;
    at_avc_comp_list[ui1_idx].ui2_id = ui2_comp_id;
    
    return TRUE;
}


/*-----------------------------------------------------------------------------
 * Name: _SrcMwConnect
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
//lint -e818
static INT32 _SrcMwConnect(
    DRV_COMP_ID_T*   pt_comp_id,
    DRV_CONN_TYPE_T  e_conn_type,
    const VOID*      pv_conn_info,
    SIZE_T           z_conn_info_len,
    VOID*            pv_tag,
    x_rm_nfy_fct     pf_nfy )
{
    UINT8           ui1_avc_comp_list_idx;
    DRV_COMP_ID_T*  prVSrcCompReg;    

    UNUSED(pf_nfy);
    UNUSED(e_conn_type);
    UNUSED(z_conn_info_len);

    if (pt_comp_id == NULL)
    {
        return RMR_DRV_INV_CONN_INFO;
    }
    
    ui1_avc_comp_list_idx = _get_avc_comp_list_idx( 
        pt_comp_id->e_type, pt_comp_id->ui2_id );
    if ( ui1_avc_comp_list_idx == 0xFF )
    {
        return RMR_DRV_INV_CONN_INFO;
    }
    
    /* Should we separate video/audio here? */
    apf_rm_conn_tag[ui1_avc_comp_list_idx] = pv_tag;
    
    /* Clear the notify function */
    /* If the driver type is SCART, its notify function record won't be cleared.
       This is for SCART pin-8 detection purpose */
    if (pt_comp_id->e_type != DRVT_AVC_SCART)
    {        
        at_avc_nfy_info[ui1_avc_comp_list_idx].pf_avc_nfy = NULL;
        at_avc_nfy_info[ui1_avc_comp_list_idx].pv_tag = NULL;
    }
            
    ab_avc_comp_no_disc_list[ui1_avc_comp_list_idx] = FALSE;
    
    /* Store SCART related info */
    if (pv_conn_info == NULL)
    {
        return RMR_DRV_INV_CONN_INFO;
    }
    prVSrcCompReg = (DRV_COMP_ID_T*)pv_conn_info;    
    t_scart_info.e_tuner_type = prVSrcCompReg->e_type;
    t_scart_info.ui2_tuner_id = prVSrcCompReg->ui2_id;    
    t_scart_info.e_scart_type = pt_comp_id->e_type;
    t_scart_info.ui2_scart_id = pt_comp_id->ui2_id;     
    if ( AVC_IS_TUNER_SRC( prVSrcCompReg->e_type ) )
    {    
        t_scart_info.b_canal_plus_support = TRUE;
    }
    else
    {
        t_scart_info.b_canal_plus_support = FALSE;
    }        
        
    /* We don't check the valid of source combination here, 
       just a pseudo connection */

    return RMR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: _SrcMwDisconnect
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _SrcMwDisconnect(DRV_COMP_ID_T*   pt_comp_id,
                        DRV_DISC_TYPE_T  e_disc_type,
                        const VOID*      pv_disc_info,
                        SIZE_T           z_disc_info_len)
{
    UINT8           ui1_avc_comp_list_idx;

    UNUSED(e_disc_type);
    UNUSED(pv_disc_info);
    UNUSED(z_disc_info_len);

    if (pt_comp_id == NULL)
    {
        return RMR_DRV_INV_CONN_INFO;
    }
    
    ui1_avc_comp_list_idx = _get_avc_comp_list_idx( 
        pt_comp_id->e_type, pt_comp_id->ui2_id );
    if ( ui1_avc_comp_list_idx == 0xFF )
    {
        return RMR_DRV_INV_CONN_INFO;
    }

    /* Clear the notify function */
    /* If the driver type is SCART, its notify function record won't be cleared.
       This is for SCART pin-8 detection purpose */    
    if (pt_comp_id->e_type != DRVT_AVC_SCART)
    {          
        at_avc_nfy_info[ui1_avc_comp_list_idx].pf_avc_nfy = NULL;
        at_avc_nfy_info[ui1_avc_comp_list_idx].pv_tag = NULL;
    }
            
    if ( apf_rm_conn_tag[ui1_avc_comp_list_idx] )
    {
        apf_rm_conn_tag[ui1_avc_comp_list_idx] = NULL;
    }

    return RMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _SrcMwGet
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static INT32 _SrcMwGet(DRV_COMP_ID_T*  pt_comp_id,
                    DRV_GET_TYPE_T  e_get_type,
                    VOID*           pv_get_info,
                    SIZE_T*         pz_get_info_len)
{
    UINT8       u1Path = 0xff;
    UINT8       u1SigStatus;
    UINT8       ui1_avc_comp_list_idx;
    UINT8       u1Pin8Status;
    UINT8       u1Pin16Status;
    
    /* Check component type and id */
    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_GET_INFO);
    }
    
    ui1_avc_comp_list_idx = _get_avc_comp_list_idx( 
        pt_comp_id->e_type, pt_comp_id->ui2_id);
    if ( ui1_avc_comp_list_idx == 0xFF )
    {
        return (RMR_DRV_INV_GET_INFO);
    }

    if ( (pt_comp_id->e_type != DRVT_AVC_AUDIO_INP) &&
         (pt_comp_id->e_type != DRVT_AVC_SCART) &&
         (pt_comp_id->e_type != DRVT_AVC_SPDIF) )
    {
        if( (_rMChannel.u2_e_type == pt_comp_id->e_type) &&
            (_rMChannel.u2_e_ui2_id == pt_comp_id->ui2_id) )
        {
            u1Path = SV_VP_MAIN;
        }
        else if( (_rPChannel.u2_e_type == pt_comp_id->e_type) &&
                 (_rPChannel.u2_e_ui2_id == pt_comp_id->ui2_id) )
        {
            u1Path = SV_VP_PIP;
        }
        else
        {
            return RMR_DRV_INV_GET_INFO; 
        }
    }
      
    /* Perform get opertion */
    switch (e_get_type)
    {
        case AVC_GET_TYPE_NO_CONN_DISC_NFY:
            if ( (NULL == pv_get_info) || (NULL == pz_get_info_len) || (*pz_get_info_len < sizeof(BOOL)) )
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            *(BOOL*)pv_get_info = ab_avc_comp_no_disc_list[ui1_avc_comp_list_idx];
            break;
                    
        case AVC_GET_TYPE_SIGNAL_STATUS:
            if ( (NULL == pv_get_info) || (NULL == pz_get_info_len) || (*pz_get_info_len < sizeof(UINT32)) )
            {
                return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            if ( (pt_comp_id->e_type == DRVT_AVC_AUDIO_INP) ||
                 (pt_comp_id->e_type == DRVT_AVC_SPDIF) )
            {
                (*(UINT32 *)pv_get_info) = AVC_STATE_AUDIO_SIGNAL;
            }
            else /* Is Video source */
            {
                /* Get Signal Status */
                u1SigStatus = bDrvVideoSignalStatus(u1Path); 
                if( u1SigStatus == (UINT8)SV_VDO_STABLE )
                {
                    (*(UINT32 *)pv_get_info) = AVC_STATE_TV_SIGNAL;
                }
                else if ( u1SigStatus == (UINT8)SV_VDO_UNKNOWN )
                {
                    (*(UINT32 *)pv_get_info) = AVC_STATE_TV_SIGNAL_UNKNOWN;
                }
                else /* u1SigStatus == SV_VDO_NOSIGNAL or SV_VDO_NOSUPPORT */
                {
                    (*(UINT32 *)pv_get_info) = AVC_STATE_NONE;
                }
                
                if ( pt_comp_id->e_type == DRVT_AVC_HDMI )
                {
                    if ( fgHDMISupportAudio() )
                    {
                        (*(UINT32 *)pv_get_info) |= AVC_STATE_AUDIO_SIGNAL;
                    }
                    
                    AVC_DBG_STMT(( "[AVC] GET HDMI%d has%s Audio\r\n",
                        pt_comp_id->ui2_id, 
                        (((*(UINT32 *)pv_get_info) & AVC_STATE_AUDIO_SIGNAL)?"":" NO") ));
                }
            }
            break;
    
        case AVC_GET_TYPE_SCART_PIN_8_STATUS:
            if (pt_comp_id->e_type == DRVT_AVC_SCART) //chack source whether SCART is
            {   
                if ( (pv_get_info) && (pz_get_info_len) && (*pz_get_info_len >= sizeof(AVC_SCART_PIN_8_T)) )
                {
                    //Gene Chang For the SCART driver 2006.10.31
                    UNUSED(bDrvGetScartFSStatus(rGetVSrcMap(pt_comp_id->e_type, pt_comp_id->ui2_id)));
                    u1Pin8Status = bDrvGetScartPin8ASR();

                    switch(u1Pin8Status)
                    {
                        case SV_WS_4W_3H :
                            *(AVC_SCART_PIN_8_T*)pv_get_info = AVC_SCART_PIN_8_ASPECT_4_3;
                            break;
                            
                        case SV_WS_16W_9H :
                            *(AVC_SCART_PIN_8_T*)pv_get_info = AVC_SCART_PIN_8_ASPECT_16_9;
                            break;
                            
                        case SV_WS_MAX :
                            *(AVC_SCART_PIN_8_T*)pv_get_info = AVC_SCART_PIN_8_NO_SIGNAL;
                            break;
                            
                        default :
                            *(AVC_SCART_PIN_8_T*)pv_get_info = AVC_SCART_PIN_8_UNKNOWN;
                            break;
                    }
                    
                    *pz_get_info_len = sizeof(AVC_SCART_PIN_8_T);
                }
                else
                {
                    return RMR_DRV_NOT_ENOUGH_SPACE;
                }
            }
            else
            {
                return RMR_DRV_INV_GET_INFO;
            }
            break;
    
        case AVC_GET_TYPE_SCART_PIN_16_STATUS:
            if (pt_comp_id->e_type == DRVT_AVC_SCART) //chack source whether SCART is
            {   
                if ( (pv_get_info) && (pz_get_info_len) && (*pz_get_info_len >= sizeof(AVC_SCART_PIN_16_T)) )
                {
                    //Gene Chang For the SCART driver 2006.10.31
                    u1Pin16Status = bDrvGetScartFBStatus();  //read register
                    if (u1Pin16Status == 0x0F)
                    {                       
                        *(AVC_SCART_PIN_16_T*)pv_get_info = AVC_SCART_PIN_16_COMPOSITE;
                    }                       
                    else if (u1Pin16Status == 0xF0)
                    {                       
                        *(AVC_SCART_PIN_16_T*)pv_get_info = AVC_SCART_PIN_16_RGB;
                    }                       
                    else if (u1Pin16Status == 0xFF)
                    {                       
                        *(AVC_SCART_PIN_16_T*)pv_get_info = AVC_SCART_PIN_16_TOGGLE;
                    }                       
                    else
                    {                       
                        *(AVC_SCART_PIN_16_T*)pv_get_info = AVC_SCART_PIN_16_UNKNOWN;
                    }                       
                    
                    *pz_get_info_len = sizeof(AVC_SCART_PIN_16_T);
                }
                else
                {
                    return RMR_DRV_NOT_ENOUGH_SPACE;
                }
            }           
            else
            {
                return RMR_DRV_INV_GET_INFO;
            }
            break;
        
        case AVC_GET_TYPE_SCART_INP_CAP:
            if (pt_comp_id->e_type == DRVT_AVC_SCART) //chack source whether SCART is
            {
                if ( (pv_get_info) && (pz_get_info_len) && (*pz_get_info_len >= sizeof(UINT32)) )
                {
                    //Gene Chang For the SCART driver 2006.10.31
                    *(UINT32*)pv_get_info = (AVC_SCART_INP_CAP_RGB | AVC_SCART_INP_CAP_COMPOSITE | AVC_SCART_INP_CAP_S_VIDEO); /* return SCART support input type - Full SCART */
                    *pz_get_info_len = sizeof(UINT32);
                }
                else
                {
                    return RMR_DRV_NOT_ENOUGH_SPACE;
                }
            }
            else
            {
                return RMR_DRV_INV_GET_INFO;
            }
            break;
        
        case AVC_GET_TYPE_SCART_BYPASS:
            /* Not support... */
            break;
        
        case AVC_GET_TYPE_SCART_OUTPUT:
            /* Not support... */            
            break;
                    
        default:
            return RMR_DRV_INV_GET_INFO;
    }
    
    return RMR_OK;
}
//lint -restore

/*-----------------------------------------------------------------------------
 * Name: _SrcMwSet
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
//lint -e818
static INT32 _SrcMwSet(
    DRV_COMP_ID_T*  pt_comp_id,
    DRV_SET_TYPE_T  e_set_type,
    const VOID*     pv_set_info,
    SIZE_T          z_set_info_len )
{
    UINT8         ui1_avc_comp_list_idx;
    
    /* Check component type and id */
    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_GET_INFO);
    }
    
    ui1_avc_comp_list_idx = _get_avc_comp_list_idx( 
        pt_comp_id->e_type, pt_comp_id->ui2_id);
    if ( ui1_avc_comp_list_idx == 0xFF )
    {
        return (RMR_DRV_INV_GET_INFO);
    }

    /* Perform set opertion */
    switch (e_set_type)
    {
        case AVC_SET_TYPE_NFY_FCT:
            if ( (NULL == pv_set_info) || (z_set_info_len < sizeof(AVC_NFY_INFO_T)) )
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            at_avc_nfy_info[ui1_avc_comp_list_idx].pv_tag = ((AVC_NFY_INFO_T*)pv_set_info)->pv_tag;
            at_avc_nfy_info[ui1_avc_comp_list_idx].pf_avc_nfy = ((AVC_NFY_INFO_T*)pv_set_info)->pf_avc_nfy;
            break;

        case AVC_SET_TYPE_NO_CONN_DISC_NFY:
            if ( z_set_info_len < sizeof(BOOL) )
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            ab_avc_comp_no_disc_list[ui1_avc_comp_list_idx] = (BOOL)(UINT32)pv_set_info;
            break;
        
        //Gene Chang For the SCART driver 2006.10.31    
        case AVC_SET_TYPE_SCART_BYPASS:
            if ( (NULL == pv_set_info) || (z_set_info_len < sizeof(AVC_SCART_BYPASS_T)) )
            {
                return RMR_DRV_INV_SET_INFO;
            }
            
            t_scart_info.e_scart_bypass = *(AVC_SCART_BYPASS_T*)pv_set_info;
            
            if (t_scart_info.e_scart_bypass == AVC_SCART_BYPASS_TRUE)
            {                   
                UNUSED(bApiVideoMainSubSrc(SV_VS_TUNER1, _bSrcSubNew));
            }                   
            else
            {               
                UNUSED(bApiVideoMainSubSrc(SV_VS_SCART1, _bSrcSubNew));
            }                   
            break;
        
        case AVC_SET_TYPE_SCART_OUTPUT:
            if (z_set_info_len < sizeof(AVC_SCART_OUTPUT_T))
            {
                return RMR_DRV_INV_SET_INFO;
            }
            
	    switch(((AVC_SCART_OUTPUT_T*)pv_set_info)->t_dev_type)
	    {
	    	case DRVT_AVC_COMP_VIDEO:
	    	case DRVT_TUNER_SAT_ANA:
	    	case DRVT_TUNER_CAB_ANA:
	    	case DRVT_TUNER_TER_ANA:
	    		vScartOutCtrl(SV_VS_CVBS1);
	    		break;
	    	case DRVT_AVC_S_VIDEO:
	    		vScartOutCtrl(SV_VS_S1);
	    		break;
	    	case DRVT_AVC_SCART:
	    		vScartOutCtrl(SV_VS_SCART2);
	    		break;
	    	case DRVT_UNKNOWN:
	    		//vScartOutCtrl(SV_VS_MAX); //scart2 out off
	    	default:
	    		vScartOutCtrl(SV_VS_MAX);
	    		break;
	    }
            #if 0
            if (*(AVC_SCART_OUTPUT_T*)pv_set_info == AVC_SCART_OUTPUT_ON)
            {                   
                /* To be implemented: Please support auto-configuration (composite/S-Video) for SCART2 */
/*                              
                vDrvAvByp();
                vDrvSVdoByp();
*/                
            }
            else
            {
                vDrvBypPD();
            }
            #endif
            break;          
        
        default:
            return RMR_DRV_INV_SET_INFO;
    }    

    return RMR_OK;
}
//lint -restore

/*-----------------------------------------------------------------------------
 * Name: avc_drv_cust_parse
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
static DRVP_TYPE_T avc_drv_cust_parse(
    const DRV_CUSTOM_T*     pt_drv_cust,
    VOID*                   pv_tag )
{
    INT32       i4_ret;
    
    UNUSED(pv_tag);

    if ( !pt_drv_cust )
    {
        return DRVP_ERROR;
    }
    
    switch( pt_drv_cust->e_custom_type )
    {
        case DRV_CUSTOM_TYPE_COMMON_REG:
            {
                UINT32     ui4_flag;
                DRV_REG_T* pt_drv_reg = (DRV_REG_T*)pt_drv_cust->pv_arg;
                
                /* Do mapping of RM component id to true HARDWARE ID,
                 * pt_drv_reg->t_drv_id <=======> pt_drv_reg->ui4_hw_addr
                 */
                
                if ( (pt_drv_reg->t_drv_id.e_type != DRVT_AVC_AUDIO_INP) &&
                     (pt_drv_reg->t_drv_id.e_type != DRVT_AVC_SPDIF) &&
                     (rGetVSrcMap( 
                        pt_drv_reg->t_drv_id.e_type, 
                        pt_drv_reg->t_drv_id.ui2_id ) == SV_VS_MAX) )
                {
                    AVC_DBG_STMT(( "[AVC] avc_drv_cust_parse(): invalid driver type=%d, id=%u\n", 
                         _rMChannel.u2_e_type, _rMChannel.u2_e_ui2_id ));
                    return DRVP_ERROR;
                }
                
                /* Do custom registration, this function will register component
                   and conenction */
                ui4_flag = DRV_FLAG_ASYNC_CONN_OR_DISC;
                if ( (pt_drv_reg->t_drv_id.e_type != DRVT_AVC_AUDIO_INP) &&
                     (pt_drv_reg->t_drv_id.e_type != DRVT_AVC_SPDIF) )
                {
                    ui4_flag |= ( AVC_FLAG_DEC_REQ_FOR_TV_SIG_DETECTION |
                                  AVC_FLAG_DEC_REQ_FOR_COLOR_SYS_DETECTION );
                }
                
                /* Decide which flag a SCART driver should register */
                if (pt_drv_reg->t_drv_id.e_type == DRVT_AVC_SCART)
                { 
                    if ((pt_drv_reg->t_drv_id.ui2_id & AVC_SCART_TYPE_SOURCE_SINK) == AVC_SCART_TYPE_SOURCE_SINK)   
                    {
                        ui4_flag |= AVC_FLAG_SCART_SOURCE | AVC_FLAG_SCART_SINK;
                    }                    
                    else if ((pt_drv_reg->t_drv_id.ui2_id & AVC_SCART_TYPE_SOURCE) == AVC_SCART_TYPE_SOURCE)
                    {
                        ui4_flag |= AVC_FLAG_SCART_SOURCE;
                    }
                    else if ((pt_drv_reg->t_drv_id.ui2_id & AVC_SCART_TYPE_SINK) == AVC_SCART_TYPE_SINK)   
                    {
                        ui4_flag |= AVC_FLAG_SCART_SINK;
                    }
                    else if ((pt_drv_reg->t_drv_id.ui2_id & AVC_SCART_TYPE_MONITOR) == AVC_SCART_TYPE_MONITOR)                       
                    {
                        ui4_flag |= AVC_FLAG_SCART_MONITOR;
                    }            
                    else
                    {
                        return DRVP_ERROR;
                    }                        
                }        
                         
                if ((ui4_flag & (AVC_FLAG_SCART_SOURCE | AVC_FLAG_SCART_SINK | AVC_FLAG_SCART_MONITOR))
                    == AVC_FLAG_SCART_SINK)
                {
                    i4_ret = d_drv_cust_reg_ex( 
                        pt_drv_reg,
                        _SrcMwConnect,
                        _SrcMwDisconnect,
                        _SrcMwGet,
                        _SrcMwSet,
                        ui4_flag,
                        CONN_DIR_INP_TO_OUT);
                }
                else
                {
                    i4_ret = d_drv_cust_reg( 
                        pt_drv_reg,
                        _SrcMwConnect,
                        _SrcMwDisconnect,
                        _SrcMwGet,
                        _SrcMwSet,
                        ui4_flag );
                }
                if ( i4_ret != 0 )
                {
                    return DRVP_ERROR;
                }
                
                if ( !_add_avc_comp_list( pt_drv_reg->t_drv_id.e_type, 
                                          pt_drv_reg->t_drv_id.ui2_id ) )
                {
                    return DRVP_ERROR;
                }
            }
            break;
        
        default:
            /* Ignore unknown types */
            break;
    }
    
    return DRVP_NEXT;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: vNotifyComp_Sig_Chg
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
VOID vNotifyComp_Sig_Chg(
    DRV_TYPE_T  e_avc_type,
    UINT16      ui2_avc_id,
    UINT32      ui4_drv_status )
{
    UINT8         ui1_avc_comp_list_idx;
    UINT32        ui4_mw_sig_status;

    ui1_avc_comp_list_idx = _get_avc_comp_list_idx( e_avc_type, ui2_avc_id );
    if ( ui1_avc_comp_list_idx == 0xFF )
    {
        AVC_DBG_STMT(( "[AVC] vNotifyComp_Sig_Chg(): invalid driver type=%d, id=%u\r\n", 
             e_avc_type, ui2_avc_id ));
        return;
    }
    
    if( ui4_drv_status == (UINT32)SV_VDO_STABLE )
    {
        ui4_mw_sig_status = AVC_STATE_TV_SIGNAL;
    }
    else if( ui4_drv_status == (UINT32)SV_VDO_NOSIGNAL )
    {
        ui4_mw_sig_status = AVC_STATE_NONE;
    }
    else
    {
        /* Ignore other states */
        AVC_DBG_STMT(( "[AVC] vNotifyComp_Sig_Chg(): invalid driver signal statustype=%d\r\n", 
             ui4_drv_status ));
        return;
    }
    
    if ( (e_avc_type == DRVT_AVC_HDMI) &&
         (ui4_drv_status == (UINT32)SV_VDO_STABLE) )
    {
        if ( fgHDMISupportAudio() )
        {
            ui4_mw_sig_status |= AVC_STATE_AUDIO_SIGNAL;
        }
        AVC_DBG_STMT(( "[AVC] Notify HDMI%d has%s Audio\r\n",
            ui2_avc_id, 
            ((ui4_mw_sig_status & AVC_STATE_AUDIO_SIGNAL)?"":" NO") ));
    }
    
    if( at_avc_nfy_info[ui1_avc_comp_list_idx].pf_avc_nfy )
    {
        at_avc_nfy_info[ui1_avc_comp_list_idx].pf_avc_nfy(
            at_avc_nfy_info[ui1_avc_comp_list_idx].pv_tag, 
            AVC_COND_SIGNAL_STATUS, 
            ui4_mw_sig_status, 
            0 );
    }
    
    /* Notify COMBI, if required. */
    if ( at_avc_combi_mw_nfy_info[ui1_avc_comp_list_idx].pf_combi_nfy )
    {
        at_avc_combi_mw_nfy_info[ui1_avc_comp_list_idx].pf_combi_nfy(
            at_avc_comp_list[ui1_avc_comp_list_idx].e_type,
            at_avc_comp_list[ui1_avc_comp_list_idx].ui2_id,
            at_avc_combi_mw_nfy_info[ui1_avc_comp_list_idx].pv_tag,
            ui4_mw_sig_status,
            0 );
    }
}


/*-----------------------------------------------------------------------------
 * Name: vNotify_Sig_Chg
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
VOID vNotify_Sig_Chg(UINT8 u1Path)
{
    DRV_TYPE_T    e_avc_type;
    UINT16        ui2_avc_id;
    UINT32        ui4_drv_status;

    if(u1Path == SV_VP_MAIN)
    {
        e_avc_type = _rMChannel.u2_e_type;
        ui2_avc_id = _rMChannel.u2_e_ui2_id;
        ui4_drv_status = bDrvVideoSignalStatus(SV_VP_MAIN);
    }
    else if(u1Path == SV_VP_PIP)
    {
        e_avc_type = _rPChannel.u2_e_type;
        ui2_avc_id = _rPChannel.u2_e_ui2_id;
        ui4_drv_status = bDrvVideoSignalStatus(SV_VP_PIP);
    }
    else
    {
        AVC_DBG_STMT(( "[AVC] vNotify_Sig_Chg(): invalid u1Path=%u\n", 
             u1Path ));
        return;
    }

    vNotifyComp_Sig_Chg( e_avc_type, ui2_avc_id, ui4_drv_status );
}


//Gene Chang For the SCART driver 2006.10.31    
/*-----------------------------------------------------------------------------
 * Name: vNotifyScart_Sig_Chg
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
void vMLEvtBridge_MsgSend(UINT8 cmd, UINT32 d1, UINT32 d2, UINT32 d3);
#include "source_table.h"
VOID vNotifyScart_Sig_Chg(UINT8 u1Source, UINT8 uStatus)
{
    UINT8           ui1_status;
    DRV_TYPE_T      e_type;
    UINT16          ui2_comp_id;
    UINT8           ui1_avc_comp_list_idx;
    
    UNUSED(rGetVSrcCompFromIdx((UINT32)u1Source, AVC_SCART_TYPE_MONITOR, &e_type, &ui2_comp_id));
    ui1_avc_comp_list_idx = _get_avc_comp_list_idx(e_type, ui2_comp_id);
    if ( ui1_avc_comp_list_idx == 0xFF )
    {
        return;
    }
    
    switch(uStatus)
    {
        case SV_WS_4W_3H :
            ui1_status = (UINT8)AVC_SCART_PIN_8_ASPECT_4_3;
            break;
            
        case SV_WS_16W_9H :
            ui1_status = (UINT8)AVC_SCART_PIN_8_ASPECT_16_9;
            break;
            
        case SV_WS_MAX :
            ui1_status = (UINT8)AVC_SCART_PIN_8_NO_SIGNAL;
            break;

        default :
            ui1_status = (UINT8)AVC_SCART_PIN_8_UNKNOWN;
    }
    
    if( at_avc_nfy_info[ui1_avc_comp_list_idx].pf_avc_nfy )
    {
        at_avc_nfy_info[ui1_avc_comp_list_idx].pf_avc_nfy(
            at_avc_nfy_info[ui1_avc_comp_list_idx].pv_tag, 
            AVC_COND_SCART_PIN_8_STATUS, 
            ui1_status, 
            0 );
    }
    
   if (VSS_MAJOR(_bSrcMainNew)==VSS_SCART)
   		vMLEvtBridge_MsgSend(2,0,ui1_status,0);
   if (VSS_MAJOR(_bSrcSubNew)==VSS_SCART)
   		vMLEvtBridge_MsgSend(2,1,ui1_status,0);
    
}


/*-----------------------------------------------------------------------------
 * Name: MWGetScartPin8Status
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
 
UINT8 bDrvGetScartFSStatus(UINT8 bSrcIdx);

extern UINT8 _bSCART1Status;
extern UINT8 _bSCART2Status;

#ifndef CC_CQAM
extern UINT8 _bScartPin8ASR_save[2];

UINT8 MWGetScartPin8Status(UINT8 u1Path)
{
	UINT8 src,state;
	if(!u1Path)
	{
		src=_bSrcMainNew;
	}
	else
	{
		src=_bSrcSubNew;
	}
	
	if(src==SV_VS_SCART1)
		state=_bScartPin8ASR_save[0];
	else if (src==SV_VS_SCART2)
		state=_bScartPin8ASR_save[1];
	else	return AVC_SCART_PIN_8_UNKNOWN;
	
    switch(state)
    {
        case SV_WS_4W_3H :
			return AVC_SCART_PIN_8_ASPECT_4_3;
        case SV_WS_16W_9H :
			return AVC_SCART_PIN_8_ASPECT_16_9;           
        case SV_WS_MAX :
			return AVC_SCART_PIN_8_NO_SIGNAL;
        default :
			return AVC_SCART_PIN_8_UNKNOWN;
    }
	
}
#endif


/*-----------------------------------------------------------------------------
 * Name: MW_SrcInit
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
//lint -e818
INT32 MW_SrcInit( DRV_CUSTOM_DIR_T* pt_drv_cust_dir )
{
    INT32  i4_ret, i4_idx;

    for(i4_idx = 0; i4_idx < (INT32)AVC_COMP_LIST_NUM_MAX; i4_idx++)
    {
        at_avc_nfy_info[i4_idx].pf_avc_nfy = NULL;
        at_avc_nfy_info[i4_idx].pv_tag = NULL;
        apf_rm_conn_tag[i4_idx] = NULL;
        at_avc_comp_list[i4_idx].e_type = DRVT_UNKNOWN;
        at_avc_comp_list[i4_idx].ui2_id = 0;
        at_avc_combi_mw_nfy_info[i4_idx].pf_combi_nfy = NULL;
        at_avc_combi_mw_nfy_info[i4_idx].pv_tag = NULL;
    }
    
    if ( !pt_drv_cust_dir )
    {
        /* There is no AVC components */
        return 0;
    }

    i4_ret = d_drv_cust_parse( 
        pt_drv_cust_dir->pt_custom,
        avc_drv_cust_parse,
        NULL );
    if ( i4_ret != 0 )
    {
        return -1;
    }
    
    return 0;
}
//lint -restore


/*-----------------------------------------------------------------------------
 * Name: MW_IsAVCExist
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
BOOL MW_IsAVCExist( 
   DRV_TYPE_T     e_type,
   UINT16         ui2_id )
{
    if ( _get_avc_comp_list_idx( e_type, ui2_id ) != 0xFF )
    {
        return TRUE;
    }

    return FALSE;
}


/*-----------------------------------------------------------------------------
 * Name: rGetVSrcMap
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
UINT32 rGetVSrcMap( 
    DRV_TYPE_T e_type, 
    UINT16     ui2_comp_id )
{
    UINT8 ui1_idx;
    
    for( ui1_idx = 0; 
         !IS_NPTV_VID_SRC_MAPPING_TBL_END(&at_vid_src_mapping_tbl[ui1_idx]); 
         ui1_idx++)
    {
        if( (at_vid_src_mapping_tbl[ui1_idx].e_type == e_type) && 
            (at_vid_src_mapping_tbl[ui1_idx].ui2_comp_id == ui2_comp_id) )
        {
            return (at_vid_src_mapping_tbl[ui1_idx].e_vid_src_idx);
        }
    }
    
    return (UINT32)SV_VS_MAX;
}


/*-----------------------------------------------------------------------------
 * Name: rGetVSrcCompFromIdx
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 rGetVSrcCompFromIdx(
    UINT32      e_vid_src_idx,
    UINT16      ui2_flag,
    DRV_TYPE_T* pe_type, 
    UINT16*     pui2_comp_id )
{
    UINT8 ui1_idx;
    
    if ( !pe_type || !pui2_comp_id )
    {
        return -1;
    }
    
    for( ui1_idx = 0; 
         !IS_NPTV_VID_SRC_MAPPING_TBL_END(&at_vid_src_mapping_tbl[ui1_idx]); 
         ui1_idx++)
    {
        if( (at_vid_src_mapping_tbl[ui1_idx].e_vid_src_idx == e_vid_src_idx) &&
            ((at_vid_src_mapping_tbl[ui1_idx].ui2_comp_id & ui2_flag) == ui2_flag) )
        {
            *pe_type = at_vid_src_mapping_tbl[ui1_idx].e_type;
            *pui2_comp_id = at_vid_src_mapping_tbl[ui1_idx].ui2_comp_id;
            return 0;
        }
    }
    
    *pe_type = (DRV_TYPE_T)SV_VS_MAX;
    *pui2_comp_id = 0;
    
    return -1;
}


/*-----------------------------------------------------------------------------
 * Name: rGetVScartInfo
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 rGetVScartInfo(AVC_SCART_INFO_T* pt_scart_info) 
{
    if (NULL == pt_scart_info)
    {
        return 0;
    }

    *pt_scart_info = t_scart_info;
    
    return 0;
}


/*-----------------------------------------------------------------------------
 * Name: MW_AVCSetNotifyOfCOMBI
 *
 * Description: 
 *
 * Inputs: 
 *
 * Outputs: 
 *
 * Returns: 
 ----------------------------------------------------------------------------*/
INT32 MW_AVCSetNotifyOfCOMBI(
   DRV_TYPE_T         e_type,
   UINT16             ui2_id,
   avc_combi_nfy_fct  pf_combi_nfy,
   VOID*              pv_tag )
{
    UINT8     ui1_idx = _get_avc_comp_list_idx( e_type, ui2_id);
    
    if ( ui1_idx == 0xFF )
    {
        return -1;
    }
    
    at_avc_combi_mw_nfy_info[ui1_idx].pf_combi_nfy = pf_combi_nfy;
    at_avc_combi_mw_nfy_info[ui1_idx].pv_tag = pv_tag;
    
    return 0;
}

