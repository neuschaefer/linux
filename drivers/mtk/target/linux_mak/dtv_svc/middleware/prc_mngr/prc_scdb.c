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
 * $RCSfile: prc_scdb.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Felisa Hsiao $
 * $MD5HEX: ecfb15ec779e268e1afadc20ffcb71ac $
 *
 * Description: 
 *         This file contains the SCDB query related operations.
 * History:
 *                
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "prc_mngr/prc_mngr.h"
#include "prc_mngr/prc_common.h"
#include "prc_mngr/prc_db.h"
#include "prc_mngr/prc_scdb.h"
#include "prc_mngr/prc_lock.h"
#include "prc_mngr/prc_util.h"
#include "prc_mngr/prc_thread.h"

#include "scdb/x_scdb_api.h"  

#include "prc_mngr/prc_debug.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
 
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: pmi_scdb_nfy_fct
 *
 * Description: This API is responsible to handle the notification from SCDB.
 *
 * Inputs:  h_scdb           Contains the scdb handle.
 *          e_cond           Contains the condition. 
 *          ui4_reason       Contains the reason.
 *          pv_tag           Contains the private tag.
 *          ui4_data         Contains addition data.
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/  
VOID pmi_scdb_nfy_fct
(
    HANDLE_T            h_scdb,
    SCDB_COND_T         e_cond,
    UINT32              ui4_reason,
    VOID*               pv_tag,  /* reference to ESNODE */
    UINT32              ui4_data
)
{
    #if 0
    if ((e_cond == SCDB_UPDATED) && (ui4_reason & SCDB_RECORD_MOD))  
        pmi_thread_scdb_updated(h_scdb, pv_tag);         
    #endif
        
}  

/*-----------------------------------------------------------------------------
 * Name: pmi_scdb_query
 *
 * Description: This API is responsible to query SCDB about ES information.
 *
 * Inputs:  pt_svdb_node     Contains the reference to SVDB node.
 *          pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     Contains the reference to ESDB node which includes 
 *                           the returned query data.
 *
 * Returns: PMR_OK           Success
 *          PMR_INV_ARG      Invalid arguments. 
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/     
INT32 pmi_scdb_query(PM_SERVICE_INFO_T* pt_svdb_node, PM_COMPONENT_INFO_T* pt_esdb_node)
{
    SCDB_REC_T          t_scdb_rec;
    STREAM_COMP_ID_T    t_stream_descr;   
    INT32               i4_return;
#if (PM_ALLOCATE_PCR)    
    SCDB_COMMON_T       t_scdb_common;
#endif     
    
    i4_return = PMR_INV_ARG;
    x_memset( &t_scdb_rec, 0, sizeof(SCDB_REC_T) );
    
    if ((pt_esdb_node != NULL)&&(pt_svdb_node != NULL))
    {
        i4_return = PMR_FAILED;        
    
        /* query SCDB */
        if (x_scdb_open(SVNODE_CONNECTION_HANDLE(pt_svdb_node), &(ESNODE_SCDB_HANDLE(pt_esdb_node)), pt_esdb_node, pmi_scdb_nfy_fct) == SCDBR_OK)
        {
            switch (ESNODE_PROC_TYPE(pt_esdb_node))
            {
                case PM_PRESENT:
                case PM_DETECTION:
                    t_stream_descr.e_type = ESNODE_DESCR_TYPE(pt_esdb_node);
                    t_stream_descr.pv_stream_tag = ESNODE_DESCR_TAG(pt_esdb_node);
                    switch (ESNODE_DESCR_TYPE(pt_esdb_node))
                    {
                        case ST_AUDIO:
                            if (x_scdb_get_rec(ESNODE_SCDB_HANDLE(pt_esdb_node), &t_stream_descr, &t_scdb_rec) == SCDBR_OK)
                            {
                                ESNODE_PID(pt_esdb_node) = t_scdb_rec.u.t_audio_mpeg.ui2_pid;
                                i4_return = PMR_OK;
                            }
                            break;
                        case ST_VIDEO:
                            if (x_scdb_get_rec(ESNODE_SCDB_HANDLE(pt_esdb_node), &t_stream_descr, &t_scdb_rec) == SCDBR_OK)
                            {
                                ESNODE_PID(pt_esdb_node) = t_scdb_rec.u.t_video_mpeg.ui2_pid;
                                i4_return = PMR_OK;
                            }
                            break;
                    #if (PM_ALLOCATE_PCR)
                        case ST_PCR:
                            if (x_scdb_get_common(ESNODE_SCDB_HANDLE(pt_esdb_node), &t_scdb_common) == SCDBR_OK)
                            {
                                ESNODE_PID(pt_esdb_node) = t_scdb_common.u_data.t_mpeg.ui2_pcr;
                                i4_return = PMR_OK;
                            }
                            break;       
                    #endif             
                        default:
                            /* ToDo */
                            break;
                    }
                    break;
 
            #if (PM_ENABLE_MULTIMEDIA)                     
                #if (PM_ENABLE_RECORD)
                case PM_RECORDING:
                    t_stream_descr.e_type        = ESNODE_DESCR_TYPE(pt_esdb_node);
                    t_stream_descr.pv_stream_tag = ESNODE_DESCR_TAG(pt_esdb_node);
                    switch (ESNODE_DESCR_TYPE(pt_esdb_node))
                    {
                        case ST_AUDIO:
                            if (x_scdb_get_rec(
                                    ESNODE_SCDB_HANDLE(pt_esdb_node), 
                                    &t_stream_descr, 
                                    &t_scdb_rec) == SCDBR_OK)
                            {
                                ESNODE_PID(pt_esdb_node) = t_scdb_rec.u.t_audio_mpeg.ui2_pid;
                                i4_return = PMR_OK;
                            }
                            break;
                        case ST_VIDEO:
                            if (x_scdb_get_rec(
                                    ESNODE_SCDB_HANDLE(pt_esdb_node), 
                                    &t_stream_descr, 
                                    &t_scdb_rec) == SCDBR_OK)
                            {
                                ESNODE_PID(pt_esdb_node) = t_scdb_rec.u.t_video_mpeg.ui2_pid;
                                i4_return = PMR_OK;
                            }
                            break;
                        default:
                            /* ToDo */
                            break;
                    }
                    break;
                #endif
                    
                case PM_MULTIMEDIA:
                    t_stream_descr.e_type = ESNODE_MMP_DESCR_TYPE(pt_esdb_node);
                    t_stream_descr.pv_stream_tag = ESNODE_MMP_DESCR_TAG(pt_esdb_node);
                    switch (ESNODE_MMP_DESCR_TYPE(pt_esdb_node))
                    {
                        case ST_AUDIO:
                            t_scdb_rec.e_rec_type = SCDB_REC_TYPE_UNKNOWN;
                            if (x_scdb_get_rec(ESNODE_SCDB_HANDLE(pt_esdb_node), &t_stream_descr, &t_scdb_rec) == SCDBR_OK)
                            {
                                switch (t_scdb_rec.e_rec_type)
                                {
                                    case SCDB_REC_TYPE_AUDIO_AVI:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_AVI;
                                        ESNODE_MMP_STREAM_AVI_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_avi.ui4_stream_id;                                        
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_MP4:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_MP4;
                                        ESNODE_MMP_STREAM_MP4_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_mp4.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_ASF:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_ASF;
                                        ESNODE_MMP_STREAM_ASF_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_asf.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_MKV:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_MKV;
                                        ESNODE_MMP_STREAM_MKV_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_mkv.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_PS:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_PS;
                                        ESNODE_MMP_STREAM_PS_ID(pt_esdb_node) = ((t_scdb_rec.u.t_audio_ps.ui1_stream_id<<8)|t_scdb_rec.u.t_audio_ps.ui1_sub_stream_id);
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_MPEG:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_TS;
                                        ESNODE_MMP_STREAM_TS_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_mpeg.ui2_pid;
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_RM:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_RM;
                                        ESNODE_MMP_STREAM_RM_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_rm.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_ES:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_ES;
                                        ESNODE_MMP_STREAM_ES_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_es.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_OGG:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_OGG;
                                        ESNODE_MMP_STREAM_OGG_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_ogg.ui4_stream_id;
                                        break;
                                    default:
                                        /* ToDo */
                                        break; 
                                }                                
                                i4_return = PMR_OK;
                            }
                            break;
                        case ST_VIDEO:
                            t_scdb_rec.e_rec_type = SCDB_REC_TYPE_UNKNOWN;
                            if (x_scdb_get_rec(ESNODE_SCDB_HANDLE(pt_esdb_node), &t_stream_descr, &t_scdb_rec) == SCDBR_OK)
                            {
                                switch (t_scdb_rec.e_rec_type)
                                {
                                    case SCDB_REC_TYPE_VIDEO_AVI:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_AVI;
                                        ESNODE_MMP_STREAM_AVI_ID(pt_esdb_node) = t_scdb_rec.u.t_video_avi.ui4_stream_id;                                        
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_MP4:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_MP4;
                                        ESNODE_MMP_STREAM_MP4_ID(pt_esdb_node) = t_scdb_rec.u.t_video_mp4.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_ASF:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_ASF;
                                        ESNODE_MMP_STREAM_ASF_ID(pt_esdb_node) = t_scdb_rec.u.t_video_asf.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_MKV:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_MKV;
                                        ESNODE_MMP_STREAM_MKV_ID(pt_esdb_node) = t_scdb_rec.u.t_video_mkv.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_PS:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_PS;
                                        ESNODE_MMP_STREAM_PS_ID(pt_esdb_node) = (t_scdb_rec.u.t_video_ps.ui1_stream_id<<8);
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_MPEG:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_TS;
                                        ESNODE_MMP_STREAM_TS_ID(pt_esdb_node) = t_scdb_rec.u.t_video_mpeg.ui2_pid;
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_RM:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_RM;
                                        ESNODE_MMP_STREAM_RM_ID(pt_esdb_node) = t_scdb_rec.u.t_video_rm.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_ES:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_ES;
                                        ESNODE_MMP_STREAM_ES_ID(pt_esdb_node) = t_scdb_rec.u.t_video_es.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_OGG:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_OGG;
                                        ESNODE_MMP_STREAM_OGG_ID(pt_esdb_node) = t_scdb_rec.u.t_video_ogg.ui4_stream_id;
                                        break;
                                    default:
                                        /* ToDo */
                                        break; 
                                }                                                      
                                i4_return = PMR_OK;
                            }
                            break;
                    
                        case ST_SUBTITLE:
                            /* ToDo */                           
                            break;       
                             
                        default:
                            /* ToDo */
                            break;
                    }                    
                    break;
            #endif               
                    
                default:
                    break;
            }        
        }
    }    
    
    if (i4_return != PMR_OK)
    {
        PM_DBG_ERROR("Query PID failed\n");
    }
        
    return i4_return;    
}

/*-----------------------------------------------------------------------------
 * Name: pmi_scdb_query_update
 *
 * Description: This API is responsible to query SCDB about updated ES 
 *              information.
 *
 * Inputs:  pt_svdb_node     Contains the reference to SVDB node.
 *          pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     Contains the reference to ESDB node which includes 
 *                           the returned query data.
 *          pb_update        Contains the reference to the result about update.
 *                           *pb_update:TRUE  means updated, FALSE: mean not updated
 *
 * Returns: PMR_OK           Success
 *          PMR_INV_ARG      Invalid arguments. 
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/     
INT32 pmi_scdb_query_update(PM_COMPONENT_INFO_T* pt_esdb_node, BOOL *pb_update)
{
    SCDB_REC_T          t_scdb_rec;
    STREAM_COMP_ID_T    t_stream_descr;   
    INT32               i4_return;
#if (PM_ALLOCATE_PCR)    
    SCDB_COMMON_T       t_scdb_common;
#endif     
    
    i4_return = PMR_INV_ARG;
    *pb_update = FALSE;
    x_memset( &t_scdb_rec, 0, sizeof(SCDB_REC_T) );
    
    if (pt_esdb_node != NULL)
    {
        if (ESNODE_SCDB_HANDLE(pt_esdb_node)!= NULL_HANDLE)
        {        
            i4_return = PMR_FAILED;
            
            switch (ESNODE_PROC_TYPE(pt_esdb_node))
            {
                case PM_PRESENT:
                case PM_DETECTION:            
                    t_stream_descr.e_type = ESNODE_DESCR_TYPE(pt_esdb_node);
                    t_stream_descr.pv_stream_tag = ESNODE_DESCR_TAG(pt_esdb_node);
        
                    /* query SCDB */
                    switch (ESNODE_DESCR_TYPE(pt_esdb_node))
                    {
                        case ST_AUDIO:
                            if (x_scdb_get_rec(ESNODE_SCDB_HANDLE(pt_esdb_node), &t_stream_descr, &t_scdb_rec) == SCDBR_OK)
                            {
                                if (ESNODE_PID(pt_esdb_node) != t_scdb_rec.u.t_audio_mpeg.ui2_pid)
                                {
                                    *pb_update = TRUE;
                                    ESNODE_PID(pt_esdb_node) = t_scdb_rec.u.t_audio_mpeg.ui2_pid;
                                }
                                i4_return = PMR_OK;
                                
                                PM_DBG_INFO("Update Audio PID[%5d]\n", ESNODE_PID(pt_esdb_node));
                            }
                            break;
                        case ST_VIDEO:
                            if (x_scdb_get_rec(ESNODE_SCDB_HANDLE(pt_esdb_node), &t_stream_descr, &t_scdb_rec) == SCDBR_OK)
                            {
                                if (ESNODE_PID(pt_esdb_node) != t_scdb_rec.u.t_video_mpeg.ui2_pid)
                                {
                                    *pb_update = TRUE;
                                    ESNODE_PID(pt_esdb_node) = t_scdb_rec.u.t_video_mpeg.ui2_pid;
                                }
                                i4_return = PMR_OK;
                                
                                PM_DBG_INFO("Update Video PID[%5d]\n", ESNODE_PID(pt_esdb_node));                        
                            }
                            break;
                    #if (PM_ALLOCATE_PCR)
                        case ST_PCR:
                            if (x_scdb_get_common(ESNODE_SCDB_HANDLE(pt_esdb_node), &t_scdb_common) == SCDBR_OK)
                            {
                                if (ESNODE_PID(pt_esdb_node) != t_scdb_common.u_data.t_mpeg.ui2_pcr)
                                {
                                    *pb_update = TRUE;
                                    ESNODE_PID(pt_esdb_node) = t_scdb_common.u_data.t_mpeg.ui2_pcr;
                                }
                                i4_return = PMR_OK;
                                
                                PM_DBG_INFO("Update PCR PID[%5d]\n", ESNODE_PID(pt_esdb_node));                         
                            }
                            break;       
                    #endif             
                        default:
                            /* ToDo */
                            break;
                    }
                    break;
            #if (PM_ENABLE_MULTIMEDIA)                     
                #if (PM_ENABLE_RECORD)
                case PM_RECORDING:
                    t_stream_descr.e_type        = ESNODE_DESCR_TYPE(pt_esdb_node);
                    t_stream_descr.pv_stream_tag = ESNODE_DESCR_TAG(pt_esdb_node);
        
                    /* query SCDB */
                    switch (ESNODE_DESCR_TYPE(pt_esdb_node))
                    {
                        case ST_AUDIO:
                            if (x_scdb_get_rec(
                                    ESNODE_SCDB_HANDLE(pt_esdb_node), 
                                    &t_stream_descr, 
                                    &t_scdb_rec) == SCDBR_OK)
                            {
                                if (ESNODE_PID(pt_esdb_node) != t_scdb_rec.u.t_audio_mpeg.ui2_pid)
                                {
                                    *pb_update = TRUE;
                                    ESNODE_PID(pt_esdb_node) = t_scdb_rec.u.t_audio_mpeg.ui2_pid;
                                }
                                i4_return = PMR_OK;
                                
                                PM_DBG_INFO("Update Audio PID[%5d]\n", ESNODE_PID(pt_esdb_node));
                            }
                            break;
                        case ST_VIDEO:
                            if (x_scdb_get_rec(
                                    ESNODE_SCDB_HANDLE(pt_esdb_node), 
                                    &t_stream_descr, 
                                    &t_scdb_rec) == SCDBR_OK)
                            {
                                if (ESNODE_PID(pt_esdb_node) != t_scdb_rec.u.t_video_mpeg.ui2_pid)
                                {
                                    *pb_update = TRUE;
                                    ESNODE_PID(pt_esdb_node) = t_scdb_rec.u.t_video_mpeg.ui2_pid;
                                }
                                i4_return = PMR_OK;
                                
                                PM_DBG_INFO("Update Video PID[%5d]\n", ESNODE_PID(pt_esdb_node));                        
                            }
                            break;
                        default:
                            /* ToDo */
                            break;
                    }
                    break;
                #endif
                    
                case PM_MULTIMEDIA:
                    t_stream_descr.e_type = ESNODE_MMP_DESCR_TYPE(pt_esdb_node);
                    t_stream_descr.pv_stream_tag = ESNODE_MMP_DESCR_TAG(pt_esdb_node);
                    switch (ESNODE_MMP_DESCR_TYPE(pt_esdb_node))
                    {
                        case ST_AUDIO:
                            t_scdb_rec.e_rec_type = SCDB_REC_TYPE_UNKNOWN;
                            if (x_scdb_get_rec(ESNODE_SCDB_HANDLE(pt_esdb_node), &t_stream_descr, &t_scdb_rec) == SCDBR_OK)
                            {
                                switch (t_scdb_rec.e_rec_type)
                                {
                                    case SCDB_REC_TYPE_AUDIO_AVI:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_AVI;
                                        ESNODE_MMP_STREAM_AVI_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_avi.ui4_stream_id;                                        
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_MP4:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_MP4;
                                        ESNODE_MMP_STREAM_MP4_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_mp4.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_ASF:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_ASF;
                                        ESNODE_MMP_STREAM_ASF_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_asf.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_MKV:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_MKV;
                                        ESNODE_MMP_STREAM_MKV_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_mkv.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_PS:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_PS;
                                        ESNODE_MMP_STREAM_PS_ID(pt_esdb_node)  = ((t_scdb_rec.u.t_audio_ps.ui1_stream_id<<8)|t_scdb_rec.u.t_audio_ps.ui1_sub_stream_id);
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_MPEG:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_TS;
                                        ESNODE_MMP_STREAM_TS_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_mpeg.ui2_pid;
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_RM:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_RM;
                                        ESNODE_MMP_STREAM_RM_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_rm.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_ES:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_ES;
                                        ESNODE_MMP_STREAM_ES_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_es.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_AUDIO_OGG:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_OGG;
                                        ESNODE_MMP_STREAM_OGG_ID(pt_esdb_node) = t_scdb_rec.u.t_audio_ogg.ui4_stream_id;
                                        break;
                                    default:
                                        /* ToDo */
                                        break; 
                                }                                
                                i4_return = PMR_OK;
                            }
                            break;
                        case ST_VIDEO:
                            t_scdb_rec.e_rec_type = SCDB_REC_TYPE_UNKNOWN;
                            if (x_scdb_get_rec(ESNODE_SCDB_HANDLE(pt_esdb_node), &t_stream_descr, &t_scdb_rec) == SCDBR_OK)
                            {
                                switch (t_scdb_rec.e_rec_type)
                                {
                                    case SCDB_REC_TYPE_VIDEO_AVI:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_AVI;
                                        ESNODE_MMP_STREAM_AVI_ID(pt_esdb_node) = t_scdb_rec.u.t_video_avi.ui4_stream_id;                                        
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_MP4:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_MP4;
                                        ESNODE_MMP_STREAM_MP4_ID(pt_esdb_node) = t_scdb_rec.u.t_video_mp4.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_ASF:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_ASF;
                                        ESNODE_MMP_STREAM_ASF_ID(pt_esdb_node) = t_scdb_rec.u.t_video_asf.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_MKV:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_MKV;
                                        ESNODE_MMP_STREAM_MKV_ID(pt_esdb_node) = t_scdb_rec.u.t_video_mkv.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_PS:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_PS;
                                        ESNODE_MMP_STREAM_PS_ID(pt_esdb_node) = (t_scdb_rec.u.t_video_ps.ui1_stream_id<<8);
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_MPEG:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_TS;
                                        ESNODE_MMP_STREAM_TS_ID(pt_esdb_node) = t_scdb_rec.u.t_video_mpeg.ui2_pid;
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_RM:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_RM;
                                        ESNODE_MMP_STREAM_RM_ID(pt_esdb_node) = t_scdb_rec.u.t_video_rm.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_ES:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_ES;
                                        ESNODE_MMP_STREAM_ES_ID(pt_esdb_node) = t_scdb_rec.u.t_video_es.ui4_stream_id;
                                        break;
                                    case SCDB_REC_TYPE_VIDEO_OGG:
                                        ESNODE_MMP_STREAM_TYPE(pt_esdb_node) = STREAM_ID_TYPE_OGG;
                                        ESNODE_MMP_STREAM_OGG_ID(pt_esdb_node) = t_scdb_rec.u.t_video_ogg.ui4_stream_id;
                                        break;
                                    default:
                                        /* ToDo */
                                        break; 
                                }                                                      
                                i4_return = PMR_OK;
                            }
                            break;
                    
                        case ST_SUBTITLE:
                            /* ToDo */                           
                            break;       
                             
                        default:
                            /* ToDo */
                            break;
                    }                                        
                    break;
            #endif
                
                default:
                    break;
            }                
        }
    } 
    
    if (i4_return != PMR_OK)
    {
        PM_DBG_ERROR("Query updated PID failed\n");
    }
           
    return i4_return;    
}


/*-----------------------------------------------------------------------------
 * Name: pmi_scdb_release
 *
 * Description: This API is responsible to release SCDB query handle. 
 *
 * Inputs:  pt_esdb_node     Contains the reference to ESDB node.
 * Outputs: pt_esdb_node     Contains the reference to ESDB node which includes 
 *                           the update data for release.
 *
 * Returns: PMR_OK           Success
 *          PMR_INV_ARG      Invalid arguments. 
 *          PMR_FAILED       Failed due to other reason.
 ----------------------------------------------------------------------------*/         
INT32 pmi_scdb_release(PM_COMPONENT_INFO_T* pt_esdb_node)
{
    INT32               i4_return;    
    
    i4_return = PMR_INV_ARG;
    if (pt_esdb_node != NULL)
    {
        i4_return = PMR_FAILED;
        
        if (ESNODE_SCDB_HANDLE(pt_esdb_node)!= NULL_HANDLE)
        {        
            if (x_handle_valid(ESNODE_SCDB_HANDLE(pt_esdb_node)) == TRUE)
            {
                if (x_scdb_close(ESNODE_SCDB_HANDLE(pt_esdb_node)) == SCDBR_OK)
                {  
                    ESNODE_SCDB_HANDLE(pt_esdb_node) = NULL_HANDLE;
                    ESNODE_PID(pt_esdb_node) = 0;
                    /* ToDo: Reset Stream type in SCDB_INFO */
                    i4_return = PMR_OK;                
                }
                else
                {
                    PM_DBG_ERROR("Close PID query:0x%x failed\n", ESNODE_SCDB_HANDLE(pt_esdb_node));
                }
            }
            else
            {
                i4_return = PMR_OK;
                
                PM_DBG_ERROR("Invalid query handle for release\n");
            }
        }
        else
        {
            i4_return = PMR_OK;
        }    
    }
    else
    {
        PM_DBG_ERROR("Invalid input for release pid query handle\n");
    }
    return i4_return;    
}    
