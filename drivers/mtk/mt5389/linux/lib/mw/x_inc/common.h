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
 * $RCSfile: common.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains internal definitions common to the whole
 *         Middleware.
 *---------------------------------------------------------------------------*/

#ifndef _COMMON_H_
#define _COMMON_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__
 
#include "inc/x_common.h"

#else

#include "x_common.h"

#endif


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Define a PCR stream type. The values 24 to 31 (inclusive) */
/* are reserved for internal usage.                          */
#define ST_PCR  ((STREAM_TYPE_T) 31)

#define ST_MASK_PCR  MAKE_BIT_MASK_32 (ST_PCR)


/* Define thread and message default settings after this comment. */

/* MIDXBULD notification and async execute thread and message queue definitions. */
#define MIDXBULD_NFY_ASYNC_EXEC_THREAD_NAME                "MIDX nfy asyc"
#define MIDXBULD_NFY_ASYNC_EXEC_THREAD_DEFAULT_PRIORITY    ((UINT8)   254)
#define MIDXBULD_NFY_ASYNC_EXEC_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#define MIDXBULD_NFY_ASYNC_EXEC_MSG_Q_NAME            "MIDX nfy asyc"
#define MIDXBULD_NFY_ASYNC_EXEC_NUM_OF_MSGS           ((UINT16)  20)
#define MIDXBULD_NFY_ASYNC_EXEC_MSG_DEFAULT_PRIORITY  ((UINT8)  128)

/* MINFO notification and async execute thread and message queue definitions. */
#define MINFO_NFY_ASYNC_EXEC_THREAD_NAME                "MINFO nfy asyc"
#define MINFO_NFY_ASYNC_EXEC_THREAD_DEFAULT_PRIORITY    ((UINT8)   200)
#define MINFO_NFY_ASYNC_EXEC_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#define MINFO_NFY_ASYNC_EXEC_MSG_Q_NAME            "MINFO nfy asyc"
#define MINFO_NFY_ASYNC_EXEC_NUM_OF_MSGS           ((UINT16)  20)
#define MINFO_NFY_ASYNC_EXEC_MSG_DEFAULT_PRIORITY  ((UINT8)  128)

/* MSVCTX async execute thread and message queue definitions. */
#define MSVCTX_ASYNC_THREAD_NAME                "MSVCTX_async"
#define MSVCTX_ASYNC_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define MSVCTX_ASYNC_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#define MSVCTX_ASYNC_MSG_Q_NAME            "MSVCTX_async"
#define MSVCTX_ASYNC_SUB_MSG_Q_NAME        "MSVCTX_async_sub"
#define MSVCTX_ASYNC_NUM_OF_MSGS           ((UINT16)  20)
#define MSVCTX_ASYNC_MSG_DEFAULT_PRIORITY  ((UINT8)  128)

/* MSVCTX notification thread and message queue definitions. */
#define MSVCTX_NOTIFY_THREAD_NAME                "MSVCTX_notify"
#define MSVCTX_NOTIFY_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define MSVCTX_NOTIFY_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 1024)

#define MSVCTX_NOTIFY_MSG_Q_NAME            "MSVCTX_notify"
#define MSVCTX_NOTIFY_NUM_OF_MSGS           ((UINT16)  20)
#define MSVCTX_NOTIFY_MSG_DEFAULT_PRIORITY  ((UINT8)  128)

/* IMAGE decode thread and decode message queue definitions. */
#define IMG_DECODE_THREAD_NAME                "IMG_decode"
#define IMG_DECODE_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define IMG_DECODE_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#define IMG_DECODE_MSG_Q_NAME            "IMG_decode"
#define IMG_DECODE_NUM_OF_MSGS           ((UINT16)  20)
#define IMG_DECODE_MSG_DEFAULT_PRIORITY  ((UINT8)  128)

/* IMAGE prefetch file thread and prefetch file message queue definitions. */
#define IMG_PREFETCH_THREAD_NAME                "IMG_prefetch"
#define IMG_PREFETCH_THREAD_DEFAULT_PRIORITY    ((UINT8)   100)
#define IMG_PREFETCH_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#define IMG_PREFETCH_MSG_Q_NAME            "IMG_prefetch"
#define IMG_PREFETCH_NUM_OF_MSGS           ((UINT16)  20)
#define IMG_PREFETCH_MSG_DEFAULT_PRIORITY  ((UINT8)  128)

/* IMAGE GIFPLAYBACK thread and message queue definitions. */
#define IMG_GIFPLAYBACK_THREAD_NAME                "IMG_gifplayback"
#define IMG_GIFPLAYBACK_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define IMG_GIFPLAYBACK_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#define IMG_GIFPLAYBACK_MSG_Q_NAME            "IMG_gifplayback"
#define IMG_GIFPLAYBACK_NUM_OF_MSGS           ((UINT16)  20)
#define IMG_GIFPLAYBACK_MSG_DEFAULT_PRIORITY  ((UINT8)  128)

/* TXT scan thread and scan message queue definitions. */
#define TXT_SCAN_THREAD_NAME                "TXT_scan"
#define TXT_SCAN_THREAD_DEFAULT_PRIORITY    ((UINT8)   200)
#define TXT_SCAN_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#define TXT_SCAN_MSG_Q_NAME            "TXT_scan"
#define TXT_SCAN_NUM_OF_MSGS           ((UINT16)  20)
#define TXT_SCAN_MSG_DEFAULT_PRIORITY  ((UINT8)  200)


/* Resource Manager thread and message queue definitions. */
#define RM_THREAD_NAME                "rm_thread"
#define RM_THREAD_DEFAULT_PRIORITY    ((UINT8)    128)
#define RM_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T)  1024)

#define RM_MSG_Q_NAME            "rm_msg_q"
#define RM_NUM_OF_MSGS           ((UINT16)  20)
#define RM_MSG_DEFAULT_PRIORITY  ((UINT8)  128)

/* Stream Manager thread and message queue definitions. */
#define SM_SLCTR_THREAD_NAME          "sm_slctr_thread"
#define SM_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define SM_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#define SM_SLCTR_MSGQ_NAME      "sm_slctr_msgq"
#define SM_DEFAULT_NUM_OF_MSGS  ((UINT16) 1024)

/* Process Manager thread and message queue definitions. */
#define PM_THREAD_NAME         "pm_thread"
#define PM_DEFAULT_PRIORITY    ((UINT8)   128)
#define PM_DEFAULT_STACK_SIZE  ((SIZE_T) 1024)

#define PM_MSG_Q_NAME            "pm_msg_q"
#define PM_DEFAULT_NUM_MSGS      ((UINT16)  32)
#define PM_MSG_DEFAULT_PRIORITY  ((UINT8)  128)

/* POD Handler thread and message queue definitions. */
#define POD_THREAD_NAME         "pod_thread"
#define POD_DEFAULT_PRIORITY    ((UINT8)   128)
#define POD_DEFAULT_STACK_SIZE  ((SIZE_T) 1024)

#define POD_MSG_Q_NAME            "pod_msg_q"
#define POD_DEFAULT_NUM_MSGS      ((UINT16)  32)
#define POD_MSG_DEFAULT_PRIORITY  ((UINT8)  128)

/* POD CA Handler thread and message queue definitions. */
#define PODCA_THREAD_NAME         "podca_thread"
#define PODCA_DEFAULT_PRIORITY    ((UINT8)   128)
#define PODCA_DEFAULT_STACK_SIZE  ((SIZE_T) 1024)

#define PODCA_MSG_Q_NAME            "podca_msg_q"
#define PODCA_DEFAULT_NUM_MSGS      ((UINT16)  32)
#define PODCA_MSG_DEFAULT_PRIORITY  ((UINT8)  128)

/* CA Handler thread and message queue definitions. */
#define CA_THREAD_NAME         "ca_thread"
#define CA_DEFAULT_PRIORITY    ((UINT8)   128)
#define CA_DEFAULT_STACK_SIZE  ((SIZE_T) 1024)

#define CA_MSG_Q_NAME            "ca_msg_q"
#define CA_DEFAULT_NUM_MSGS      ((UINT16)  32)
#define CA_MSG_DEFAULT_PRIORITY  ((UINT8)  128)

#define CA_AGENT_THREAD_NAME         "ca_agent_thread"
#define CA_AGENT_DEFAULT_PRIORITY    ((UINT8)   128)
#define CA_AGENT_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#define CA_AGENT_MSG_Q_NAME            "ca_agent_msg_q"
#define CA_AGENT_DEFAULT_NUM_MSGS      ((UINT16)  32)
#define CA_AGENT_MSG_DEFAULT_PRIORITY  ((UINT8)  128)

/* IO Manager thread and message queue definitions. */
#define IOM_THREAD_NAME                 "iom_thread"
#define IOM_THREAD_DEFAULT_PRIORITY     ((UINT8)   128)
#define IOM_THREAD_DEFAULT_STACK_SIZE   ((SIZE_T) 2048)

#define IOM_MSGQ_NAME                   "iom_msgq"
#define IOM_NUM_OF_MSGS                 ((UINT16)  32)
#define IOM_MSGQ_DEFAULT_PRIORITY       ((UINT8)  128)

/* CLI thread and message queue definitions. */
#define CLI_THREAD_NAME                 "cli_thread"
#define CLI_THREAD_DEFAULT_PRIORITY     ((UINT8)   235)
#define CLI_THREAD_DEFAULT_STACK_SIZE   ((SIZE_T) 8192)

#define CLI_MSGQ_NAME                   "cli_msgq"
#define CLI_MSGQ_DEFAULT_PRIORITY       ((UINT8)   128)
#define CLI_NUM_OF_MSGS                 ((UINT16) 2048)

/* ATV Manager thread and message queue definitions. */
#define ATVM_RTX_THREAD_NAME                "atvm_rtx_thread"
#define ATVM_RTX_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define ATVM_RTX_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 2048)

#define ATVM_RTX_MSGQ_NAME              "atvm_rtx_msgq"
#define ATVM_RTX_MSGQ_DEFAULT_PRIORITY  ((UINT8)  128)
#define ATVM_RTX_NUM_OF_MSGS            ((UINT16)  40)

#define ATVM_FLOW_THREAD_NAME                "atvm_flow_thread"
#define ATVM_FLOW_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define ATVM_FLOW_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 1024)

/* WM thread and message queue definitions. */
#define WM_THREAD_NAME                  "wm_thread"
#define WM_THREAD_DEFAULT_PRIORITY      ((UINT8)   128)
#define WM_THREAD_DEFAULT_STACK_SIZE    ((SIZE_T) 8192)

#define WM_MSG_Q_NAME                   "wm_msg_q"
#define WM_MSG_DEFAULT_PRIORITY         ((UINT8)  128)
#define WM_NUM_OF_MSGS                  ((UINT16)  128)

/* WGL animation manager thread definition */
#define WM_ANIMATION_NAME               "wm_anim"
#define WM_ANIMATION_PRIORITY           ((UINT8)   128)
#define WM_ANIMATION_STACK_SIZE         ((SIZE_T) 1024)

#define WM_ANIMATION_MSG_Q_NAME         "wm_anim_msg_q"
#define WM_ANIMATION_MSG_PRIORITY       ((UINT8)  128)
#define WM_ANIMATION_NUM_OF_MSGS        ((UINT16)  16)

/* File Manager thread and message queue definitions. */
#define FM_BUF_THREAD_NAME                "fm_buf_thread"
#define FM_BUF_THREAD_DEFAULT_PRIORITY    ((UINT8)  128)
#define FM_BUF_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 1024)

#define FM_DEV_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define FM_DEV_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 2048)

#define FM_DLNA_DEV_THREAD_DEFAULT_PRIORITY    ((UINT8)   210)
#define FM_DLNA_DEV_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 2048)

/* Service Context thread and message queue definitions. */
#define SVCTX_THREAD_NAME                "svctx_thread"
#define SVCTX_MSG_Q_NAME                 "svctx_msgq"
#define SVCTX_THREAD_DEFAULT_PRIORITY    ((UINT8)  128)
#define SVCTX_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)
#define SVCTX_MSGQ_DEFAULT_PRIORITY      ((UINT8)  128)
#ifdef MW_TV_AV_BYPASS_SUPPORT
#define SVCTX_NUM_OF_MSGS                ((UINT16) 128)
#else
#define SVCTX_NUM_OF_MSGS                ((UINT16) 32)
#endif

/* Closed Caption Widget Core thread and message queue definitions */
#define CCCORE_THREAD_NAME                      "cccore_thread"
#define CCCORE_THREAD_DEFAULT_PRIORITY          ((UINT8)   129)
#define CCCORE_THREAD_DEFAULT_STACK_SIZE        ((SIZE_T) 8192)

#define CCCORE_MSGQ_NAME                        "cccore_msgq"
#define CCCORE_NUM_OF_MSGS                      ((UINT16)  256)
#define CCCORE_MSGQ_DEFAULT_PRIORITY            ((UINT8)   128)

#define CCCORE_TPWTR_MSGQ_NAME_PREFIX           "cctwq_"
#define CCCORE_TPWTR_NUM_OF_MSGS                ((UINT16)   32)
#define CCCORE_TPWTR_MSGQ_DEFAULT_PRIORITY      ((UINT8)   128)


/* SVL Builder thread and message queue definitions. */
#define SB_SLCTR_MSGQ_NAME                  "sb_slctr_msgq"
#define SB_SLCTR_THREAD_NAME                "sb_slctr_thread"

#define SB_DEFAULT_NUM_OF_MSGS              ((UINT16)  256)
#define SB_THREAD_DEFAULT_PRIORITY          ((UINT8)   128)
#define SB_THREAD_DEFAULT_STACK_SIZE        ((SIZE_T) 8192)


/* Event Context thread and message queue definitions. */
#define EVCTX_SLCTR_MSGQ_NAME               "evctx_slctr_msgq"
#define EVCTX_SLCTR_THREAD_NAME             "evctx_slctr_thread"

#ifdef  MW_EVCTX_CACHE_SUPPORT
#define EVCTX_DEFAULT_NUM_OF_MSGS           ((UINT16)  (192*8))
#else
#define EVCTX_DEFAULT_NUM_OF_MSGS           ((UINT16)  192)
#endif
#define EVCTX_THREAD_DEFAULT_PRIORITY       ((UINT8)   128)
#define EVCTX_THREAD_DEFAULT_STACK_SIZE     ((SIZE_T) 4096)

/* Event database thread and message queue definitions. */
#define EDB_SLCTR_MSGQ_NAME                  "edb_slctr_msgq"
#define EDB_SLCTR_THREAD_NAME                "edb_slctr_thread"

#define EDB_DEFAULT_NUM_OF_MSGS              ((UINT16)  256)
#define EDB_THREAD_DEFAULT_PRIORITY          ((UINT8)   128)
#define EDB_THREAD_DEFAULT_STACK_SIZE        ((SIZE_T) 8192)

#define EDB_SECTION_MSGQ_NAME                  "edb_section_msgq"
#define EDB_SECTION_THREAD_NAME                "edb_section_thread"

#define EDB_SECTION_DEFAULT_NUM_OF_MSGS              ((UINT16)  2048)
#define EDB_SECTION_THREAD_DEFAULT_PRIORITY          ((UINT8)   (128)     )
#define EDB_SECTION_THREAD_DEFAULT_STACK_SIZE        ((SIZE_T)  (8192)    )


/* Rating Region Context thread and message queue definitions. */
#define RRCTX_SLCTR_MSGQ_NAME               "rrctx_slctr_msgq"
#define RRCTX_SLCTR_THREAD_NAME             "rrctx_slctr_thread"

#define RRCTX_DEFAULT_NUM_OF_MSGS           ((UINT16)   24)
#define RRCTX_THREAD_DEFAULT_PRIORITY       ((UINT8)   128)
#define RRCTX_THREAD_DEFAULT_STACK_SIZE     ((SIZE_T) 8192)

/* Section Manager thread and message queue definitions. */
#define SECM_THREAD_NAME                   "sec_mngr_thread"

#define SECM_THREAD_DEFAULT_PRIORITY       ((UINT8)   128)
#define SECM_THREAD_DEFAULT_STACK_SIZE     ((SIZE_T) 8192)

/* Section Engine thread and message queue definitions. */
#define SECT_THREAD_NAME                   "sect_eng_thread"

#define SECT_THREAD_DEFAULT_PRIORITY       ((UINT8)   128)
#define SECT_THREAD_DEFAULT_STACK_SIZE     ((SIZE_T) 4096)

/* Table Manager DVB-SI engine thread and message queue definitions. */
#define DVB_SI_ENG_THREAD_NAME                 "dvb_si_eng_thread"
#define DVB_SI_ENG_THREAD_DEFAULT_PRIORITY     ((UINT8)   128)
#define DVB_SI_ENG_THREAD_DEFAULT_STACK_SIZE   ((SIZE_T) 4096)

#define DVB_SI_ENG_MSGQ_NAME                   "dvb_si_eng_msgq"
#define DVB_SI_ENG_MSGQ_DEFAULT_PRIORITY       ((UINT8)   128)

/* Download function engine thread and message queue definitions. */
#define DFM_THREAD_NAME                 "dfm_thread"
#define DFM_THREAD_DEFAULT_PRIORITY     ((UINT8)   128)
#define DFM_THREAD_DEFAULT_STACK_SIZE   ((SIZE_T) 4096)

#define DFM_MSGQ_NAME                   "dfm_msgq"
#define DFM_MSGQ_DEFAULT_PRIORITY       ((UINT8)   128)

/* VBI Filter thread and message queue definitions */
#define VBIF_THREAD_NAME                "vbif_thread"
#define VBIF_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define VBIF_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#define VBIF_MSG_Q_NAME                 "vbif_msg_q"
#define VBIF_NUM_OF_MSGS                ((UINT16) 512)

/* CEC Manager thread and message queue definitions. */
#define CECM_THREAD_NAME                    "cecm_thread"
#define CECM_THREAD_DEFAULT_PRIORITY        ((UINT8)   128)
#define CECM_THREAD_DEFAULT_STACK_SIZE      ((SIZE_T) 2048)

#define CECM_MSGQ_NAME                      "cecm_msgq"
#define CECM_NUM_OF_MSGS                    ((UINT16) 128)
#define CECM_MSGQ_DEFAULT_PRIORITY          ((UINT8)  128)

/* Device Manager thread and message queue definitions */
#define DM_CB_MSG_Q_NAME                "dm_cb_msg_q"
#define DM_CB_MSG_Q_NUM                 ((UINT16) 256)

#define DM_NFY_REQ_Q_NAME               "dm_nfy_req_q"
#define DM_NFY_REQ_Q_NUM                ((UINT16) 256)

#define DM_IOC_NFY_Q_NAME               "dm_ioc_nfy_q"
#define DM_IOC_NFY_Q_NUM                ((UINT16) 256)

#define DM_BOARD_MSG_Q_NAME             "dm_board_msg_q"
#define DM_BOARD_MSG_Q_NUM              ((UINT16) 256)

#define DM_CB_MSG_THREAD_NAME                "dm_cb_msg_thrd"
#define DM_CB_MSG_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define DM_CB_MSG_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#define DM_NFY_REQ_THREAD_NAME                "dm_nfy_req_thrd"
#define DM_NFY_REQ_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define DM_NFY_REQ_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#define DM_IOC_NFY_THREAD_NAME                "dm_ioc_nfy_thrd"
#define DM_IOC_NFY_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define DM_IOC_NFY_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#define DM_BOARD_MSG_THREAD_NAME                "dm_board_msg_thrd"
#define DM_BOARD_MSG_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define DM_BOARD_MSG_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#define DM_AUTOMNT_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define DM_AUTOMNT_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

/* TOD thread (stack size and priority) */
#define TOD_THREAD_DEFAULT_PRIORITY           ((UINT8)   128)
#define TOD_THREAD_DEFAULT_STACK_SIZE         ((SIZE_T) 4096)

/* TFTP thread (stack size and priority) */
#define TFTP_THREAD_DEFAULT_PRIORITY          ((UINT8)   128)
#define TFTP_THREAD_DEFAULT_STACK_SIZE        ((SIZE_T) 4096)

/* Message queue health check */
#define MSG_Q_HEALTH_CHECK_THREAD_NAME        "msgqhealth"
#define MSG_Q_HEALTH_CHECK_THREAD_PRIORITY    ((SIZE_T)    1)
#define MSG_Q_HEALTH_CHECK_THREAD_STACK_SIZE  ((SIZE_T) 1024)

#if 1 /* Network */
/* NET_LIB_IP_RX_THREAD */
#define IP_STACK_RX_THREAD_NAME              "IpDrvRx"
#define IP_STACK_RX_THREAD_PRIORITY          ((UINT8) 90)
#define IP_STACK_RX_THREAD_STACK_SIZE        ((SIZE_T) 4096)

/* NET_LIB_IP_TIMER_THREAD */
#define IP_STACK_TIMER_THREAD_NAME           "IpTimer"
#define IP_STACK_TIMER_PRIORITY              ((UINT8) 90)
#define IP_STACK_TIMER_THREAD_STACK_SIZE     ((SIZE_T) 4096)

/* NET_MT_GLUE_TX_THREAD */
#define IP_DRV_TX_THREAD_NAME                "IpDrvTx"
#define IP_DRV_TX_THREAD_PRIORITY            ((UINT8) 90)
#define IP_DRV_TX_THREAD_STACK_SIZE          ((SIZE_T) 1024)

/* NET_NI_MONITOR_THREAD */
#define NI_MONITOR_THREAD_NAME               "NiEvMon"
#define NI_MONITOR_THREAD_PRIORITY           ((UINT8) 120)
#define NI_MONITOR_THREAD_STACK_SIZE         ((SIZE_T) 1024)

/* NET_LIB_DLNA_DMP_THREAD */
#define DLNA_DMP_THREAD_NAME                 "DmpPrgs"
#define DLNA_DMP_THREAD_PRIORITY             ((UINT8) 210)
#define DLNA_DMP_THREAD_STACK_SIZE           ((SIZE_T) 8192)

/* NET_LIB_DLNA_DMP_HTTP_FM_THREAD */
#define DLNA_HTTP_FM_THREAD_NAME             "HttpFm"
#define DLNA_HTTP_FM_THREAD_PRIORITY         ((UINT8) 210) 
#define DLNA_HTTP_FM_THREAD_STACK_SIZE       ((SIZE_T) 8192)

/* Network upgrade module */
#define HTTP_NETWORK_UPGRADE_THREAD_NAME      "NHDload"
#define HTTP_NETWORK_UPGRADE_PRIORITY         ((UINT8) 180)
#define HTTP_NETWORK_UPGRADE_STACK_SIZE       ((SIZE_T) 4096)

/*Record Utility thread and message queue definitions*/
#define REC_UTIL_MSGQ_NAME                  "rec_util_msgq"
#define REC_UTIL_THREAD_NAME                "rec_util_thread"
#define REC_UTIL_DEFAULT_NUM_OF_MSGS        ((UINT16)  192)
#define REC_UTIL_THREAD_DEFAULT_PRIORITY    ((UINT8)   128)
#define REC_UTIL_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

/* WLAN WPA supplicant thread */
#define WLAN_WPA_THREAD_NAME                "wlan_wpa_thrd"
#define WLAN_WPA_STACK_SIZE                 (6 * 1024)
#define WLAN_WPA_THREAD_PRIORITY            ((UINT8) 170)

#endif

/* capture logo thread */
#define CAP_NFY_ASYNC_EXEC_MSG_Q_NAME            "CAP nfy asyc"
#define CAP_NFY_ASYNC_EXEC_NUM_OF_MSGS           ((UINT16)  10)
#define CAP_NFY_ASYNC_EXEC_MSG_DEFAULT_PRIORITY  ((UINT8)  128)

/* capture logo notification and async execute thread and message queue definitions. */
#define CAP_NFY_ASYNC_EXEC_THREAD_NAME                "CAP nfy asyc"
#define CAP_NFY_ASYNC_EXEC_THREAD_DEFAULT_PRIORITY    ((UINT8)   200)
#define CAP_NFY_ASYNC_EXEC_THREAD_DEFAULT_STACK_SIZE  ((SIZE_T) 4096)

#endif /* _COMMON_H_ */


