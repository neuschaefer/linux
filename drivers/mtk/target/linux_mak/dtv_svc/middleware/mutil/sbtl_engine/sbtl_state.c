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
 * $RCSfile: sbtl_state.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: $
 * $SWAuthor: $
 * $MD5HEX: $
 *
 * Description: 
 *         
 * History:
 *           
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__
#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif
#include "inc/x_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "util/x_lnk_list.h"
#include "mutil/sbtl_engine/sbtl_dbg.h"
#include "mutil/sbtl_engine/sbtl_struct.h"
#include "mutil/sbtl_engine/sbtl_utils.h"
#include "mutil/sbtl_engine/sbtl_register.h"
#include "mutil/sbtl_engine/sbtl_state.h"
     
#include "inc/x_mm_common.h"



#else /*__KERNEL__*/
#include "x_os.h"
#include "x_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "x_dbg.h"
#include "x_lnk_list.h"
#include "x_mm_common.h"

#include "sbtl_dbg.h"
#include "sbtl_struct.h"
#include "sbtl_utils.h"
#include "sbtl_register.h"
#include "sbtl_state.h"
#include "sbtl_widget.h"

#endif /*__KERNEL__*/


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/* Static local function */
static INT32 _mm_sbtl_state_do_opening(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_opened(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_starting(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_started(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_stopping(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_closing(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_closed(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_error(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_ignore_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_opening_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_opened_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_starting_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_started_excpt(
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_stopping_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_closing_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_closed_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

static INT32 _mm_sbtl_state_do_error_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 );

/*-----------------------------------------------------------------------------
                    data declarations, extern, static, const
 ----------------------------------------------------------------------------*/
/* 
 *  Subtitle Stream State Machine 
 */
const SBTL_STATE_DESCR_T   at_mm_sbtl_state_descr_lst[] =
{
  { 
      SBTL_COND_IGNORE,                               /* ui4_state */
      FALSE,                                        /* b_prcss_pending */
      FALSE,                                        /* b_allow_pending */
      NULL,                                         /* pf_state_do */
      _mm_sbtl_state_do_ignore_excpt             /* pf_state_do_exception */
  },                                                
                                                    
  {                                                 
      SBTL_COND_OPENING,                              /* ui4_state */
      FALSE,                                        /* b_prcss_pending */
      TRUE,                                         /* b_allow_pending */
      _mm_sbtl_state_do_opening,                 /* pf_state_do */
      _mm_sbtl_state_do_opening_excpt            /* pf_state_do_exception */
  },                                                
                                                    
  {                                                 
      SBTL_COND_OPENED,                               /* ui4_state */
      TRUE,                                         /* b_prcss_pending */
      FALSE,                                        /* b_allow_pending */
      _mm_sbtl_state_do_opened,                  /* pf_state_do */
      _mm_sbtl_state_do_opened_excpt             /* pf_state_do_exception */
  },                                                
                                                    
  {                                                 
      SBTL_COND_STARTING,                             /* ui4_state */     
      FALSE,                                        /* b_prcss_pending */
      TRUE,                                         /* b_allow_pending */
      _mm_sbtl_state_do_starting,                /* pf_state_do */
      _mm_sbtl_state_do_starting_excpt           /* pf_state_do_exception */
  },                                                
                                                    
  {                                                 
      SBTL_COND_STARTED,                              /* ui4_state */     
      TRUE,                                         /* b_prcss_pending */
      FALSE,                                        /* b_allow_pending */
      _mm_sbtl_state_do_started,                 /* pf_state_do */
      _mm_sbtl_state_do_started_excpt            /* pf_state_do_exception */
  },                                                
                                                    
  {                                                 
      SBTL_COND_STOPPING,                             /* ui4_state */ 
      FALSE,                                        /* b_prcss_pending */
      TRUE,                                         /* b_allow_pending */
      _mm_sbtl_state_do_stopping,                /* pf_state_do */
      _mm_sbtl_state_do_stopping_excpt           /* pf_state_do_exception */
  },                                                
                                                    
  {                                                 
      SBTL_COND_CLOSING,                              /* ui4_state */ 
      FALSE,                                        /* b_prcss_pending */
      TRUE,                                         /* b_allow_pending */
      _mm_sbtl_state_do_closing,                 /* pf_state_do */
      _mm_sbtl_state_do_closing_excpt            /* pf_state_do_exception */
  },                                                
                                                    
  {                                                 
      SBTL_COND_CLOSED,                               /* ui4_state */   
      FALSE,                                        /* b_prcss_pending */
      FALSE,                                        /* b_allow_pending */
      _mm_sbtl_state_do_closed,                  /* pf_state_do */
      _mm_sbtl_state_do_closed_excpt             /* pf_state_do_exception */
  },                                                
                                                    
  {                                                 
      SBTL_COND_ERROR,                                /* ui4_state */
      TRUE,                                         /* b_prcss_pending */
      FALSE,                                        /* b_allow_pending */
      _mm_sbtl_state_do_error,                   /* pf_state_do */
      _mm_sbtl_state_do_error_excpt              /* pf_state_do_exception */
  },

  SBTL_STATE_DESCR_NULL  
};

const UINT32 at_mm_sbtl_allowed_pending_event_lst[] =
{
  EVN_MM_SBTL_APP_PLAY_REQ,
  EVN_MM_SBTL_APP_STOP_REQ,
  EVN_MM_SBTL_APP_CLOSE_REQ,
  EVN_MM_SBTL_SCDB_RECORD_DEL,
  EVN_MM_SBTL_IGNORE
};

const SBTL_STATE_COND_T    at_mm_sbtl_state_cond_lst[] =
{
    /* Ignore state condition */
    { 
        SBTL_COND_IGNORE,                               /* ui4_curr_state */
        EVN_MM_SBTL_APP_OPEN_REQ,                       /* ui4_event */
        SBTL_COND_OPENING,                              /* ui4_next_state */
        EVN_MM_SBTL_IGNORE,                             /* ui4_pending_event */
        NULL                                            /* pf_event_do */
    },

    /* opening state conditions */ 
    { 
        SBTL_COND_OPENING,                            /* ui4_curr_state */    
        EVN_MM_SBTL_DRV_OPENED,                  /* ui4_event */         
        SBTL_COND_OPENED,                             /* ui4_next_state */    
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */ 
        NULL                                        /* pf_event_do */       
    },
    { 
        SBTL_COND_OPENING,                            /* ui4_curr_state */    
        EVN_MM_SBTL_APP_PLAY_REQ,                /* ui4_event */         
        SBTL_COND_OPENING,                            /* ui4_next_state */    
        EVN_MM_SBTL_APP_PLAY_REQ,                /* ui4_pending_event */ 
        NULL                                        /* pf_event_do */       
    },
    {                                                                      
        SBTL_COND_OPENING,                            /* ui4_curr_state */   
        EVN_MM_SBTL_APP_STOP_REQ,               /* ui4_event */        
        SBTL_COND_OPENING,                            /* ui4_next_state */   
        EVN_MM_SBTL_APP_STOP_REQ,               /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    }, 
    {                                                                      
        SBTL_COND_OPENING,                            /* ui4_curr_state */   
        EVN_MM_SBTL_APP_CLOSE_REQ,               /* ui4_event */        
        SBTL_COND_OPENING,                            /* ui4_next_state */   
        EVN_MM_SBTL_APP_CLOSE_REQ,               /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },    
    {                                               
        SBTL_COND_OPENING,                            /* ui4_curr_state */    
        EVN_MM_SBTL_SCDB_RECORD_DEL,             /* ui4_event */         
        SBTL_COND_OPENING,                            /* ui4_next_state */    
        EVN_MM_SBTL_APP_CLOSE_REQ,               /* ui4_pending_event */ 
        NULL                                        /* pf_event_do */                               
    },                                                                      
    {                                               
        SBTL_COND_OPENING,                            /* ui4_curr_state */    
        EVN_MM_SBTL_DRV_PLAYED,                  /* ui4_event */         
        SBTL_COND_ERROR,                              /* ui4_next_state */    
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */ 
        NULL                                        /* pf_event_do */                               
    },                                                                      
    {                                                
        SBTL_COND_OPENING,                            /* ui4_curr_state */     
        EVN_MM_SBTL_DRV_STOPPED,                 /* ui4_event */          
        SBTL_COND_ERROR,                              /* ui4_next_state */     
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */  
        NULL                                        /* pf_event_do */        
    },                                                                      
    {                                               
        SBTL_COND_OPENING,                            /* ui4_curr_state */    
        EVN_MM_SBTL_DRV_CLOSED,                  /* ui4_event */         
        SBTL_COND_ERROR,                              /* ui4_next_state */    
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */ 
        NULL                                        /* pf_event_do */                               
    },
    {                                                
        SBTL_COND_OPENING,                            /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_ERR,                     /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                              
    {                                               
        SBTL_COND_OPENING,                            /* ui4_curr_state */       
        EVN_MM_SBTL_INTERNAL_ERR,                /* ui4_event */            
        SBTL_COND_ERROR,                              /* ui4_next_state */       
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */    
        NULL                                        /* pf_event_do */          
    },                                              
                                                    
    /* opened state conditions *************************************************/ 
    {                                                                         
        SBTL_COND_OPENED,                             /* ui4_curr_state */     
        EVN_MM_SBTL_APP_PLAY_REQ,                /* ui4_event */          
        SBTL_COND_STARTING,                           /* ui4_next_state */     
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */  
        NULL                                        /* pf_event_do */        
    },                                                                   
    {                                                                        
        SBTL_COND_OPENED,                             /* ui4_curr_state */     
        EVN_MM_SBTL_APP_CLOSE_REQ,               /* ui4_event */          
        SBTL_COND_CLOSING,                            /* ui4_next_state */     
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */  
        NULL                                        /* pf_event_do */        
    },                                                                       
    {                                                                        
        SBTL_COND_OPENED,                             /* ui4_curr_state */    
        EVN_MM_SBTL_SCDB_RECORD_DEL,             /* ui4_event */         
        SBTL_COND_CLOSING,                            /* ui4_next_state */    
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */ 
        NULL                                        /* pf_event_do */                              
    },                                                                      
    {                                                                       
        SBTL_COND_OPENED,                             /* ui4_curr_state */    
        EVN_MM_SBTL_DRV_OPENED,                  /* ui4_event */         
        SBTL_COND_ERROR,                              /* ui4_next_state */    
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */ 
        NULL                                        /* pf_event_do */       
    },                                                                      
    {                                                                      
        SBTL_COND_OPENED,                             /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_PLAYED,                  /* ui4_event */        
        SBTL_COND_STARTING,                           /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_OPENED,                             /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_STOPPED,                 /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_OPENED,                             /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_CLOSED,                  /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_OPENED,                             /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_ERR,                     /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    { 
        SBTL_COND_OPENED,                             /* ui4_curr_state */   
        EVN_MM_SBTL_INTERNAL_ERR,                /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },
    
    /* starting state conditions ***********************************************/ 
    {                                                                      
        SBTL_COND_STARTING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_PLAYED,                  /* ui4_event */        
        SBTL_COND_STARTED,                            /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STARTING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_APP_STOP_REQ,                /* ui4_event */        
        SBTL_COND_STARTING,                           /* ui4_next_state */   
        EVN_MM_SBTL_APP_STOP_REQ,                /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                  
    {                                                                      
        SBTL_COND_STARTING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_APP_CLOSE_REQ,               /* ui4_event */        
        SBTL_COND_STARTING,                           /* ui4_next_state */   
        EVN_MM_SBTL_APP_CLOSE_REQ,               /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STARTING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_SCDB_RECORD_DEL,             /* ui4_event */        
        SBTL_COND_STARTING,                           /* ui4_next_state */   
        EVN_MM_SBTL_APP_CLOSE_REQ,               /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STARTING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_OPENED,                  /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STARTING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_STOPPED,                 /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STARTING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_CLOSED,                  /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STARTING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_ERR,                     /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STARTING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_INTERNAL_ERR,                /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    
    /* started state conditions ************************************************/ 
    {                                                                      
        SBTL_COND_STARTED,                            /* ui4_curr_state */   
        EVN_MM_SBTL_APP_STOP_REQ,                /* ui4_event */        
        SBTL_COND_STOPPING,                           /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                           
    {                                                                      
        SBTL_COND_STARTED,                            /* ui4_curr_state */   
        EVN_MM_SBTL_APP_CLOSE_REQ,               /* ui4_event */        
        SBTL_COND_STOPPING,                           /* ui4_next_state */   
        EVN_MM_SBTL_APP_CLOSE_REQ,               /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STARTED,                            /* ui4_curr_state */   
        EVN_MM_SBTL_SCDB_RECORD_DEL,             /* ui4_event */        
        SBTL_COND_STOPPING,                           /* ui4_next_state */   
        EVN_MM_SBTL_APP_CLOSE_REQ,               /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STARTED,                            /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_OPENED,                  /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STARTED,                            /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_PLAYED,                  /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STARTED,                            /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_STOPPED,                 /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STARTED,                            /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_CLOSED,                  /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STARTED,                            /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_ERR,                     /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STARTED,                            /* ui4_curr_state */   
        EVN_MM_SBTL_INTERNAL_ERR,                /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    
    {                                                                      
        SBTL_COND_STARTED,                            /* ui4_curr_state */   
        EVN_MM_SBTL_SCDB_RECORD_MOD,             /* ui4_event */        
        SBTL_COND_STOPPING,                           /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     


    /* stopping state conditions ***********************************************/ 
    {                                                                      
        SBTL_COND_STOPPING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_STOPPED,                 /* ui4_event */        
        SBTL_COND_OPENED,                             /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STOPPING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_APP_CLOSE_REQ,               /* ui4_event */        
        SBTL_COND_STOPPING,                           /* ui4_next_state */   
        EVN_MM_SBTL_APP_CLOSE_REQ,               /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STOPPING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_SCDB_RECORD_DEL,             /* ui4_event */        
        SBTL_COND_STOPPING,                           /* ui4_next_state */   
        EVN_MM_SBTL_APP_CLOSE_REQ,               /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STOPPING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_APP_PLAY_REQ,                /* ui4_event */        
        SBTL_COND_STOPPING,                           /* ui4_next_state */   
        EVN_MM_SBTL_APP_PLAY_REQ,                /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STOPPING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_OPENED,                  /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STOPPING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_PLAYED,                  /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STOPPING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_CLOSED,                  /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STOPPING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_ERR,                     /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_STOPPING,                           /* ui4_curr_state */   
        EVN_MM_SBTL_INTERNAL_ERR,                /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    
    /* closing state conditions ************************************************/ 
    {                                                                      
        SBTL_COND_CLOSING,                            /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_CLOSED,                  /* ui4_event */        
        SBTL_COND_CLOSED,                             /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_CLOSING,                            /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_ERR,                     /* ui4_event */        
        SBTL_COND_CLOSING,                            /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                       
        SBTL_COND_CLOSING,                            /* ui4_curr_state */   
        EVN_MM_SBTL_INTERNAL_ERR,                /* ui4_event */        
        SBTL_COND_CLOSING,                            /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
                                                    
    /* error state conditions ************************************************/ 
    {                                                                        
        SBTL_COND_ERROR,                              /* ui4_curr_state */   
        EVN_MM_SBTL_APP_CLOSE_REQ,               /* ui4_event */        
        SBTL_COND_CLOSING,                            /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_ERROR,                              /* ui4_curr_state */   
        EVN_MM_SBTL_SCDB_RECORD_DEL,             /* ui4_event */        
        SBTL_COND_CLOSING,                            /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_ERROR,                              /* ui4_curr_state */   
        EVN_MM_SBTL_APP_PLAY_REQ,                /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                
    {                                                                      
        SBTL_COND_ERROR,                              /* ui4_curr_state */   
        EVN_MM_SBTL_APP_STOP_REQ,                /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                  
    {                                                                      
        SBTL_COND_ERROR,                              /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_OPENED,                  /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_ERROR,                              /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_OPENED,                  /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_ERROR,                              /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_PLAYED,                  /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_ERROR,                              /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_STOPPED,                 /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    {                                                                      
        SBTL_COND_ERROR,                              /* ui4_curr_state */   
        EVN_MM_SBTL_DRV_CLOSED,                  /* ui4_event */        
        SBTL_COND_ERROR,                              /* ui4_next_state */   
        EVN_MM_SBTL_IGNORE,                              /* ui4_pending_event */
        NULL                                        /* pf_event_do */      
    },                                                                     
    
    /* ANY state events ********************************************************/ 
    /* !!! Don't remove this !!!************************************************/
    SBTL_STATE_COND_NULL                              
};                                                  


/*-----------------------------------------------------------------------------
                    Internal functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: Ignore state handler
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_ignore_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2)
{
    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: Opening state handler
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_opening(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2)
{
    INT32                   i4_ret;
    SBTL_ENGINE_T*          pt_engine;
    MM_SBTL_HDLR_MSG_T      t_sbtl_hdlr_msg;
    BOOL                    is_open_success;
    BOOL                    is_need_dmx;

    is_open_success = FALSE;
    
    pt_engine = (SBTL_ENGINE_T*) pv_tag1;

    ASSERT(pt_engine, ("_mm_sbtl_state_do_opening pt_engine=NULL\n"));
    if (NULL==pt_engine)
    {
        return MMSBTLR_INV_ARG;
    }

    ASSERT(pt_engine->pt_page_mngr, ("_mm_sbtl_state_do_opening pt_engine->pt_page_mngr=NULL\n"));
    if (NULL==pt_engine->pt_page_mngr)
    {
        return MMSBTLR_INV_ARG;
    }

    ASSERT(pt_engine->pt_page_mngr->pf_need_dmx, ("_mm_sbtl_state_do_opening pt_engine->pt_page_mngr->pf_need_dmx=NULL\n"));
    if (NULL==pt_engine->pt_page_mngr->pf_need_dmx)
    {
        return MMSBTLR_INV_ARG;
    }
    

    i4_ret = pt_engine->pt_page_mngr->pf_need_dmx(pt_engine->pt_page_mngr, &is_need_dmx);
    if (i4_ret != MMSBTLR_OK)
    {
        mm_sbtl_hdlr_send_state_msg(t_sbtl_hdlr_msg, 
                                    pt_engine, 
                                    EVN_MM_SBTL_DRV_ERR);
        return (MMSBTLR_OK);
    }

    if (is_need_dmx)
    {
        if (MMSBTLR_OK == sbtl_dmx_open(pt_engine, pt_engine->t_cfg.h_source))
        {
            is_open_success = TRUE;
        }
    }

    i4_ret = sbtl_stc_open(pt_engine);
    
    if (TRUE == is_open_success || !is_need_dmx)
    {
        mm_sbtl_hdlr_send_state_msg(t_sbtl_hdlr_msg, 
                                    pt_engine, 
                                    EVN_MM_SBTL_DRV_OPENED);

    }
    else
    {
        mm_sbtl_hdlr_send_state_msg(t_sbtl_hdlr_msg, 
                                    pt_engine, 
                                    EVN_MM_SBTL_DRV_ERR);
    }

    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_opening_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2)
{
    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: Opened state handler
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_opened(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2)
{
    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_opened_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2)
{
    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_starting(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2)
{
    INT32                       i4_ret;
    SBTL_ENGINE_T*              pt_engine;
    MM_SBTL_HDLR_MSG_T          t_sbtl_hdlr_msg;
    BOOL                        is_need_dmx;
    
    pt_engine = (SBTL_ENGINE_T*) pv_tag1;

    ASSERT(pt_engine, ("_mm_sbtl_state_do_starting pt_engine=NULL\n"));
    if (NULL==pt_engine)
    {
        return MMSBTLR_INV_ARG;
    }

    ASSERT(pt_engine->pt_page_mngr, ("_mm_sbtl_state_do_starting pt_engine->pt_page_mngr=NULL\n"));
    if (NULL==pt_engine->pt_page_mngr)
    {
        return MMSBTLR_INV_ARG;
    }

    ASSERT(pt_engine->pt_page_mngr->pf_need_dmx, ("_mm_sbtl_state_do_starting pt_engine->pt_page_mngr->pf_need_dmx=NULL\n"));
    if (NULL==pt_engine->pt_page_mngr->pf_need_dmx)
    {
        return MMSBTLR_INV_ARG;
    }

    ASSERT(pt_engine->pt_page_mngr->pf_start, ("_mm_sbtl_state_do_starting pt_engine->pt_page_mngr->pf_start=NULL\n"));
    if (NULL==pt_engine->pt_page_mngr->pf_start)
    {
        return MMSBTLR_INV_ARG;
    }

    i4_ret = pt_engine->pt_page_mngr->pf_need_dmx(pt_engine->pt_page_mngr, &is_need_dmx);
    if (i4_ret != MMSBTLR_OK)
    {
        mm_sbtl_hdlr_send_state_msg(t_sbtl_hdlr_msg, 
                                    pt_engine, 
                                    EVN_MM_SBTL_DRV_ERR);
        return (MMSBTLR_OK);
    }

    if (is_need_dmx)
    {
        sbtl_dmx_data_que_open(pt_engine);
        if (MMSBTLR_OK != sbtl_dmx_enable(pt_engine))
        {
            mm_sbtl_hdlr_send_state_msg(t_sbtl_hdlr_msg, 
                                        pt_engine, 
                                        EVN_MM_SBTL_DRV_ERR);
        }
    }
    
    pt_engine->pt_page_mngr->pf_start(pt_engine->pt_page_mngr);

    mm_sbtl_hdlr_send_state_msg(t_sbtl_hdlr_msg, 
                                pt_engine, 
                                EVN_MM_SBTL_DRV_PLAYED);

    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_starting_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2)
{
    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_started(
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2)
{
    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_started_excpt(
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2)
{
    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_stopping(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2)
{
    SBTL_ENGINE_T*              pt_engine;
    MM_SBTL_HDLR_MSG_T          t_sbtl_hdlr_msg;
    
    pt_engine = (SBTL_ENGINE_T*) pv_tag1;

    ASSERT(pt_engine, ("_mm_sbtl_state_do_stopping pt_engine=NULL\n"));
    if (NULL==pt_engine)
    {
        return MMSBTLR_INV_ARG;
    }

    ASSERT(pt_engine->pt_page_mngr, ("_mm_sbtl_state_do_stopping pt_engine->pt_page_mngr=NULL\n"));
    if (NULL==pt_engine)
    {
        return MMSBTLR_INV_ARG;
    }

    ASSERT(pt_engine->pt_page_mngr->pf_stop, ("_mm_sbtl_state_do_stopping pt_engine->pt_page_mngr->pf_stop=NULL\n"));
    if (NULL==pt_engine)
    {
        return MMSBTLR_INV_ARG;
    }

    /* Disable Demux */
    sbtl_dmx_disable(pt_engine);

    pt_engine->pt_page_mngr->pf_stop(pt_engine->pt_page_mngr);

    sbtl_dmx_data_que_close(pt_engine);

    mm_sbtl_hdlr_send_state_msg(t_sbtl_hdlr_msg, 
                                pt_engine, 
                                EVN_MM_SBTL_DRV_STOPPED);
    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_stopping_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2)
{
    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_error(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_error_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_closing(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2)
{
    INT32                       i4_ret;
    SBTL_ENGINE_T*              pt_engine;
    BOOL                        is_need_dmx;
    MM_SBTL_HDLR_MSG_T          t_sbtl_hdlr_msg;
    
    pt_engine = (SBTL_ENGINE_T*) pv_tag1;

    ASSERT(pt_engine, ("_mm_sbtl_state_do_closed pt_engine=NULL\n"));
    if (NULL==pt_engine)
    {
        return MMSBTLR_INV_ARG;
    }

    ASSERT(pt_engine->pt_page_mngr, ("_mm_sbtl_state_do_closed pt_engine->pt_page_mngr=NULL\n"));
    if (NULL==pt_engine)
    {
        return MMSBTLR_INV_ARG;
    }

    ASSERT(pt_engine->pt_page_mngr->pf_need_dmx, ("_mm_sbtl_state_do_closed pt_engine->pt_page_mngr->pf_need_dmx=NULL\n"));
    if (NULL==pt_engine)
    {
        return MMSBTLR_INV_ARG;
    }

    i4_ret = pt_engine->pt_page_mngr->pf_need_dmx(pt_engine->pt_page_mngr, &is_need_dmx);
    if (i4_ret == MMSBTLR_OK)
    {
        if (is_need_dmx)
        {
            /* close Demux */
            sbtl_dmx_close(pt_engine);
        }
    }

    sbtl_stc_close(pt_engine);

    mm_sbtl_hdlr_send_state_msg(t_sbtl_hdlr_msg, 
                                pt_engine, 
                                EVN_MM_SBTL_DRV_CLOSED);
    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_closing_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_closed(     
    UINT32     ui4_previous_state,
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    SBTL_ENGINE_T*                  pt_engine;
    page_inst_mngr_destructor       pf_destructor;
    
    pt_engine = (SBTL_ENGINE_T*) pv_tag1;
    
    ASSERT(pt_engine, ("_mm_sbtl_state_do_closed pt_engine=NULL\n"));
    if (NULL==pt_engine)
    {
        return MMSBTLR_INV_ARG;
    }

    if (pt_engine->pt_fct_tbl)
    {
        if (pt_engine->pt_fct_tbl->pf_mngr_destructor)
        {
            pf_destructor = pt_engine->pt_fct_tbl->pf_mngr_destructor;
            pf_destructor(pt_engine->pt_page_mngr);
        }
    }
    
    if (pt_engine->h_get_sem)
    {
        x_sema_delete(pt_engine->h_get_sem);
        pt_engine->h_get_sem = NULL_HANDLE;
    }
    
    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
static INT32 _mm_sbtl_state_do_closed_excpt(     
    UINT32     ui4_event,
    UINT32     ui4_curr_state,
    VOID*      pv_tag1,
    VOID*      pv_tag2 )
{
    return (MMSBTLR_OK);
}

/*-----------------------------------------------------------------------------
                    External functions implementations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: 
 *
 * Description: 
 *
 * Inputs:  pt_this     Pointer points to the 
 *
 * Outputs: -
 *
 * Returns: -
 ----------------------------------------------------------------------------*/
INT32 mm_sbtl_state_init( sbtl_state_notify_fct     pf_state_nfy,
                          VOID*                     pv_sm_nfy_tag,
                          SBTL_STATE_T*             pt_state)
{
    INT32                   i4_ret;

    i4_ret = sbtl_state_init( at_mm_sbtl_state_cond_lst,
                              at_mm_sbtl_state_descr_lst,
                              at_mm_sbtl_allowed_pending_event_lst,
                              pf_state_nfy,
                              pv_sm_nfy_tag,
                              pt_state);
    return i4_ret;
}


