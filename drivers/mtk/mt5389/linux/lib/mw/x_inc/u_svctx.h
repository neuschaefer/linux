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
/*! @file u_svctx.h 
 *  $File: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/middleware/svctx/u_svctx.h $
 *  $Revision: #3 $
 *  $Date: 2012/06/28 $
 *  $Author: wengao.han $
 *  
 *  @par Description:
 *
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_SVCTX_H_
#define _U_SVCTX_H_

/*-----------------------------------------------------------------------------
 *  include files
 *---------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_sys_name.h"
#include "u_device_type.h"
#include "u_evctx.h"
#ifdef MW_EDB_SUPPORT
#include "u_edb.h"
#endif
#include "u_sm_video_hdlr.h"
#include "u_sm_audio_hdlr.h"
#include "u_sm_cc_hdlr.h"
#include "u_sm_sbtl_hdlr.h"
#include "u_prc_ca.h"
#include "u_scc.h"
#include "u_scdb.h"
#include "u_svl.h"
#include "u_ch_hdlr.h"
#include "u_inp_hdlr.h"
#include "u_mm_hdlr.h"
#include "u_gl.h"

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_SVCTX Service Context
 *
 *  @ingroup groupMW
 *  @brief For user change input source, change channel, select multi media and so on.
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could change input source, 
 *   change channel, select multi media and so on.
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
 * structure, constants, macro definitions
 *---------------------------------------------------------------------------*/
/* constants */
#define INVALID_CHANNEL_ID            ((UINT32)0xffffffff) /* will change to UINT64 */    /**< A const value for invalid channel ID. */
#define DEFAULT_STOP_SVC_TIMEOUT      ((UINT32)20000)      /* ms */    /**< The timeout for sync stop channel. */
#define DEFAULT_STOP_STRM_TIMEOUT     ((UINT32)20000)      /* ms */    /**< The timeout for sync stop stream. */
#define SVCTX_NAME_MAX_SIZE           (16)    /**< The max length for svctx name. */
#define SVCTX_SRC_NAME_MAX_SIZE       CONN_SRC_NAME_MAX_STRLEN    /**< The max length for svctx src name. */
#define SVCTX_SNK_NAME_MAX_SIZE       SYS_NAME_LEN    /**< The max length for svctx snk name. */
#define SVCTX_SRC_FILE_PATH_MAX_SIZE  (180)    /**< The max length for svctx src file path. */

#define SVCTX_STRM_MODE_AS_LAST       ((UINT32)0xffffffff)    /**< A const value for invalid stream mode. */

#define NTSC_CC1                      (CC_CC1)    /**< NTSC_CC1 */
#define NTSC_CC2                      (CC_CC2)    /**< NTSC_CC2 */
#define NTSC_CC3                      (CC_CC3)    /**< NTSC_CC3 */
#define NTSC_CC4                      (CC_CC4)    /**< NTSC_CC4 */
#define NTSC_T1                       ((UINT8)(CC_CC4 + 1))    /**< NTSC_T1 */
#define NTSC_T2                       ((UINT8)(CC_CC4 + 2))    /**< NTSC_T2 */
#define NTSC_T3                       ((UINT8)(CC_CC4 + 3))    /**< NTSC_T3 */
#define NTSC_T4                       ((UINT8)(CC_CC4 + 4))    /**< NTSC_T4 */

#ifdef MW_TV_AV_BYPASS_SUPPORT
/* Copy protection bitmask */
#define CP_NONE                       ((UINT32) 0)    /**< CP_NONE */
#define CP_AUDIO                      MAKE_BIT_MASK_32(ST_AUDIO)    /**< CP_AUDIO */
#define CP_VIDEO                      MAKE_BIT_MASK_32(ST_VIDEO)    /**< CP_VIDEO */
#define CP_CLOSED_CAPTION             MAKE_BIT_MASK_32(ST_CLOSED_CAPTION)    /**< CP_CLOSED_CAPTION */
#define CP_TELETEXT                   MAKE_BIT_MASK_32(ST_TELETEXT)    /**< CP_TELETEXT */
#define CP_SUBTITLE                   MAKE_BIT_MASK_32(ST_SUBTITLE)    /**CP_SUBTITLE */
#define CP_DATA                       MAKE_BIT_MASK_32(ST_DATA)    /**< CP_DATA */
#define CP_BYPASS                     MAKE_BIT_MASK_32(ST_BYPASS)    /**< CP_BYPASS */
#define CP_MM_SUBTITLE                MAKE_BIT_MASK_32(ST_MM_SUBTITLE)    /**< CP_MM_SUBTITLE */
#define CP_ALL                        ((UINT32)(CP_AUDIO | CP_VIDEO | CP_VIDEO | CP_CLOSED_CAPTION | \
                                                CP_TELETEXT | CP_SUBTITLE | CP_DATA | CP_BYPASS | CP_MM_SUBTITLE))    /**< CP_ALL */
#define CP_AUDIO_VIDEO                ((UINT32)(CP_AUDIO | CP_VIDEO))    /**< CP_AUDIO_VIDEO */
#endif

typedef UINT32 SVCTX_SET_TYPE_T;    /**< Svctx common set type. */
typedef UINT32 SVCTX_GET_TYPE_T;    /**< Svctx common get type. */
/* relative macro for service context get/set type */
#define SVCTX_TYPE_CODE_BITS           24    /**< Svctx type for code region */
#define SVCTX_TYPE_GRP_BITS            8    /**< Svctx type for group region */
#define SVCTX_TYPE_GRP_MASK            0xFF000000    /**< Svctx type for group mask */
#define SVCTX_TYPE_CODE_MASK           0x00FFFFFF    /**< Svctx type for code mask */
#define SVCTX_TYPE_GET_GRP(_type)      (_type & SVCTX_GRP_MASK)    /**< Acquire group type. */
#define SVCTX_TYPE_GET_CODE(_type)     (_type & SVCTX_CODE_MASK)    /**< Acquire code type. */

#define SVCTX_GRP_COMMON                                  (1<<SVCTX_TYPE_CODE_BITS)    /**< Svctx common group. */
/* common set types *********************************************************/
#define SVCTX_COMMON_SET_TYPE_IGNORE                      ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON | 0)) /**< Ignore */
#define SVCTX_COMMON_SET_TYPE_SNK_NAME                    ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON | 1)) /**< type = CHAR*,                      size = SVCTX_SNK_NAME_MAX_SIZE */
#define SVCTX_COMMON_SET_VIDEO_MUTE                       ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON | 2)) /**< type = SVCTX_VIDEO_MUTE_INFO_T*,   size = sizeof(SVCTX_VIDEO_MUTE_INFO_T)     */
#define SVCTX_COMMON_SET_VIDEO_MUTE_VIA_PMX_BG            ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON | 3)) /**< type = BOOL*                       size = sizeof(BOOL)                        */
#define SVCTX_COMMON_SET_VIDEO_MUTE_ATTR                  ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON | 4)) /**< type = SVCTX_VIDEO_MUTE_INFO_T*,   size = sizeof(SVCTX_VIDEO_MUTE_INFO_T)     */
#define SVCTX_COMMON_SET_MTS                              ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON | 5)) /**< type = SCC_AUD_MTS_T*              size = sizeof(SCC_AUD_MTS_T)               */
#define SVCTX_COMMON_SET_STOP_AUDIO_AS_NO_SIGNAL          ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON | 6)) /**< type = UINT8*                      size = sizeof(UINT8), value = [MAKE_BIT_MASK_8(SRC_TYPE_BRDCST_SVL)] | [MAKE_BIT_MASK_8(SRC_TYPE_AVC)] */
#define SVCTX_COMMON_SET_NO_SIGNAL_PRECEDE_BLOCK          ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON | 7)) /**< type = BOOL*                       size = sizeof(BOOL)                        */
#define SVCTX_COMMON_SET_DEFALUT_BLOCK_CHECK_FCT          ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON | 8)) /**< type = x_svctx_svc_block_check_fct size = sizeof(x_svctx_svc_block_check_fct) */
#define SVCTX_COMMON_SET_DEFAULT_VIDEO_MODE               ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON | 9)) /**< type = SCC_VID_MODE_T*             size = sizeof(SCC_VID_MODE_T)              */
#define SVCTX_COMMON_SET_SCART_OUT                        ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |10)) /**< type = SCART_OUT_INFO_T*           size = sizeof(SCART_OUT_INFO_T)            */
#define SVCTX_COMMON_SET_ANALOG_BLOCK_BIAS_FREQ           ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |11)) /**< type = CH_BLOCK_BIAS_FREQ_T*       size = sizeof(CH_BLOCK_BIAS_FREQ_T)        */
#define SVCTX_COMMON_SET_PMX_BG_ENABLE_FLAG               ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |12)) /**< type = BOOL*                       size = sizeof(BOOL)                        */
/* the difference between SVCTX_COMMON_SET_VIDEO_MUTE_VIA_PMX_BG and SVCTX_COMMON_SET_PMX_BG_ENABLE_FLAG is
 * SVCTX_COMMON_SET_VIDEO_MUTE_VIA_PMX_BG: it sets the service context's internal flag for PMX bg enable flag
 *                                         and also invoke SM's x_scc_disp_set_bg() to set PMX
 * SVCTX_COMMON_SET_PMX_BG_ENABLE_FLAG:    it only sets the service context's internal flag for PMX bg enable flag
 */
#define SVCTX_COMMON_SET_TYPE_SRC_NAME                    ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |13)) /**< type = CHAR*,                      size = SVCTX_SNK_NAME_MAX_SIZE             */    
#define SVCTX_COMMON_SET_TYPE_SIGNAL_LOSS_DELAY_TIME      ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |14)) /**< type = SIGNAL_LOSS_DELAY_T*,       size = sizeof(SIGNAL_LOSS_DELAY_T)         */    
/* the command only sets the video mute enable flag, it doesn't change the RGB color
 * it is useful for caller just want to disable the video mute and will enable it later
 * and it doesn't need to know the actual RGB of the video mute
 */
#define SVCTX_COMMON_SET_VIDEO_MUTE_VIA_FLAG              ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |15)) /**< type = BOOL*                       size = sizeof(BOOL)                        */    
#define SVCTX_COMMON_SET_VIDEO_MUTE_SUPERVISOR            SVCTX_COMMON_SET_VIDEO_MUTE_VIA_FLAG    /**< type=BOOL size = sizeof(BOOL)*/
#define SVCTX_COMMON_SET_BLOCK_MUTE                       ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |16)) /**< type = SVCTX_VIDEO_MUTE_INFO_T*,   size = sizeof(SVCTX_VIDEO_MUTE_INFO_T)     */    
#define SVCTX_COMMON_SET_SCREEN_STATUS                    ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |17)) /**< type = SVCTX_SCREEN_STATUS_T*,     size = sizeof(SVCTX_SCREEN_STATUS_T)       */    
#define SVCTX_COMMON_SET_BLOCK_RULE                       ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |18)) /**< type = SVCTX_BLOCK_RULE_T*,        size = sizeof(SVCTX_BLOCK_RULE_T)          */    
#define SVCTX_COMMON_SET_ALL_SCART_OUT_DISABLE            ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |19)) /**< type = BOOL*                       size = sizeof(BOOL)                        */    
#define SVCTX_COMMON_SET_DETECTION_MODE                   ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |20)) /**< type = DETECTION_MODE_T*           size = sizeof(DETECTION_MODE_T)            */    
#define SVCTX_COMMON_SET_ENABLE_VIDEO                     ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |21)) /**< type = BOOL*                       size = sizeof(BOOL)                        */    
#define SVCTX_COMMON_SET_SHOW_SNOW_AS_NO_SIGNAL           ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |22)) /**< type = BOOL*                       size = sizeof(BOOL)                        */    
/* the follow two set commands just change the stream mode, but do not select/stop the relative stream accordingly. */
#define SVCTX_COMMON_SET_ADD_STREAM_MODE_BIT              ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |23)) /**< type = UINT32*                     size = sizeof(UINT32)                      */    
#define SVCTX_COMMON_SET_DEL_STREAM_MODE_BIT              ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |24)) /**< type = UINT32*                     size = sizeof(UINT32)                      */    
#define SVCTX_COMMON_SET_STREAM_SELECT_BLOCK              ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |25)) /**< type = BOOL*                       size = sizeof(BOOL)                        */    

#ifdef MW_TV_AV_BYPASS_SUPPORT
#define SVCTX_COMMON_SET_ENABLE_SVC_SELECT_WITH_ASSOCIATE ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |26)) /**< type = BOOL*                       size = sizeof(BOOL)                        */    
#define SVCTX_COMMON_SET_KEEP_ALIVE_AS_STOP_LEAD          ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |27)) /**< type = BOOL*                       size = sizeof(BOOL)                        */    
#define SVCTX_COMMON_SET_COPY_PROTECTION                  ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |28)) /**< type = SVCTX_COPY_PROTECTION_T*    size = sizeof(SVCTX_COPY_PROTECTION_T)     */    
#endif

#ifdef SVCTX_SPLIT_SUPPORT
#define SVCTX_COMMON_SET_DEFALUT_BLOCK_CHECK_FCT_TAG      ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |34)) /**< type = svctx_svc_block_check_fct size = sizeof(svctx_svc_block_check_fct) */
#endif

/* ATOMIC command */
#define SVCTX_COMMON_SET_ATOMIC_CMD_GRP                   ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |29))    /**< type = SVCTX_ATOMIC_CMD_T* size=sizof(SVCTX_ATOMIC_CMD_T) */
#define SVCTX_COMMON_SET_ATOMIC_DISP_RGN                  ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |30))    /**< type = VSH_REGION_INFO_T* size=sizeof(VSH_REGION_INFO_T) */
#define SVCTX_COMMON_SET_ATOMIC_SCRN_MODE                 ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |31))    /**< type = SCC_VID_SCREEN_MODE_T, size=sizeof(SCC_VID_SCREEN_MODE_T)  */
#define SVCTX_COMMON_SET_ATOMIC_OVERSCAN                  ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |32))    /**< type = SCC_VID_SCREEN_MODE_T, size=sizeof(SCC_VID_SCREEN_MODE_T)  */
#define SVCTX_COMMON_SET_ATOMIC_CMD_END                   ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |33))    /**< type = SM_CMD_CODE_T,  size=sizeof(SM_CMD_CODE_T)  */

#define SVCTX_COMMON_SET_SCDB_SORT_FUNC                   ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |100))    /**< type = SM_CMD_CODE_T,  size=sizeof(SM_CMD_CODE_T)  */

#define SVCTX_COMMON_SET_MHEG_SCREEN_MODE                 ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |40))    /**< type = SVCTX_MHEG_SCREEN_MODE_T,  size=sizeof(SVCTX_MHEG_SCREEN_MODE_T)  */

#define SVCTX_COMMON_SET_CONTROL_RIKS_TV                  ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |41))    /**< type = BOOL,  size=sizeof(BOOL)  */



/* common get types *********************************************************/
#define SVCTX_COMMON_GET_TYPE_IGNORE                      ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON | 0)) /**< Ignore */
#define SVCTX_COMMON_GET_TYPE_CRNT_VIDEO_DEVICE_TYPE      ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON | 1)) /**< type = DEVICE_TYPE_T*,             size = sizeof(DEVICE_TYPE_T)               */    
#define SVCTX_COMMON_GET_TYPE_CRNT_AUDIO_DEVICE_TYPE      ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON | 2)) /**< type = DEVICE_TYPE_T*,             size = sizeof(DEVICE_TYPE_T)               */    
#define SVCTX_COMMON_GET_TYPE_COLOR_SYSTEM                ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON | 3)) /**< type = COLOR_SYS_T*,               size = sizeof(COLOR_SYS_T)                 */    
#define SVCTX_COMMON_GET_TYPE_NAME                        ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON | 4)) /**< type = CHAR*,                      size = SVCTX_NAME_MAX_SIZE                 */    
#define SVCTX_COMMON_GET_TYPE_SRC_NAME                    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON | 5)) /**< type = CHAR*,                      size = SVCTX_SRC_NAME_MAX_SIZE             */    
#define SVCTX_COMMON_GET_TYPE_SNK_NAME                    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON | 6)) /**< type = CHAR*,                      size = SVCTX_SNK_NAME_MAX_SIZE             */    
#define SVCTX_COMMON_GET_TYPE_BLOCK_COND                  ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON | 7)) /**< type = SVC_BLOCK_COND_T*           size = sizeof(SVC_BLOCK_COND_T)            */    
#define SVCTX_COMMON_GET_VIDEO_MUTE                       ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON | 8)) /**< type = SVCTX_VIDEO_MUTE_INFO_T*,   size = sizeof(SVCTX_VIDEO_MUTE_INFO_T)     */    
#define SVCTX_COMMON_GET_MTS                              ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON | 9)) /**< type = SCC_AUD_MTS_T*,             size = sizeof(SCC_AUD_MTS_T)               */    
#define SVCTX_COMMON_GET_CC_AVIL_COND                     ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |10)) /**< type = SVCTX_CC_AVIL_COND_T*,      size = sizeof(SVCTX_CC_AVIL_COND_T)        */    
#define SVCTX_COMMON_GET_TYPE_IS_BLOCK                    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |11)) /**< type = BOOL*                       size = sizeof(BOOL)                        */    
#define SVCTX_COMMON_GET_STREAM_MODE                      ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |12)) /**< type = UINT32*                     size = sizeof(UINT32)                      */    
#define SVCTX_COMMON_GET_SIGNAL_STATUS                    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |13)) /**< type = SIGNAL_STATUS_T*            size = sizeof(SIGNAL_STATUS_T)             */    
#define SVCTX_COMMON_GET_SCDB_NUM_RECS                    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |14)) /**< type = SVCTX_GET_SCDB_NUM_RECS_T*  size = sizeof(SVCTX_GET_SCDB_NUM_RECS_T)   */    
#define SVCTX_COMMON_GET_SCDB_REC_BY_IDX                  ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |15)) /**< type = SVCTX_GET_SCDB_REC_BY_IDX_T*size = sizeof(SVCTX_GET_SCDB_REC_BY_IDX_T) */    
#define SVCTX_COMMON_GET_USING_SCDB_REC                   ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |16)) /**< type = SVCTX_GET_USING_SCDB_REC_T* size = sizeof(SVCTX_GET_USING_SCDB_REC_T)  */    
#define SVCTX_COMMON_GET_AUTO_COLOR_COND                  ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |17)) /**< type = AUTO_COLOR_COND_T*          size = sizeof(AUTO_COLOR_COND_T)           */    
#define SVCTX_COMMON_GET_AUTO_CLK_POS_PHS_COND            ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |18)) /**< type = AUTO_CLK_POS_PHS_COND_T*    size = sizeof(AUTO_CLK_POS_PHS_COND_T)     */    
#define SVCTX_COMMON_GET_VIDEO_DECODE_STATUS              ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |19)) /**< type = VSH_DIG_DECODE_STATUS_T*    size = sizeof(VSH_DIG_DECODE_STATUS_T)     */    
#define SVCTX_COMMON_GET_AUDIO_DECODE_STATUS              ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |20)) /**< type = ASH_AUDIO_DECODE_STATUS_T*  size = sizeof(ASH_AUDIO_DECODE_STATUS_T)   */    
#define SVCTX_COMMON_GET_DEFALUT_BLOCK_CHECK_FCT          ((SVCTX_SET_TYPE_T)(SVCTX_GRP_COMMON |21)) /**< type = x_svctx_svc_block_check_fct size = sizeof(x_svctx_svc_block_check_fct) */    
#define SVCTX_COMMON_GET_DEFAULT_VIDEO_MODE               ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |22)) /**< type = SCC_VID_MODE_T*             size = sizeof(SCC_VID_MODE_T)              */    
#define SVCTX_COMMON_GET_SVC_REQ_SERIAL_NUM               ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |23)) /**< type = UINT32*                     size = sizeof(UINT32)                      */    
#define SVCTX_COMMON_GET_SRC_TYPE                         ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |24)) /**< type = SRC_TYPE_T*                 size = sizeof(SRC_TYPE_T)                      */    

#ifdef MW_TV_AV_BYPASS_SUPPORT
#define SVCTX_COMMON_GET_ENABLE_SVC_SELECT_WITH_ASSOCIATE ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |25)) /**< type = BOOL*                       size = sizeof(BOOL)                        */    
#define SVCTX_COMMON_GET_COPY_PROTECTION                  ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |26)) /**< type = SVCTX_COPY_PROTECTION_T*    size = sizeof(SVCTX_COPY_PROTECTION_T)     */    
#define SVCTX_COMMON_GET_PRIORITY                         ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |27)) /**< type = INT8*                       size = sizeof(INT8)                        */    

#define SVCTX_SVC_SELECT_FLAG_TV            ((UINT32) 0x1)    /**< select tv flag */
#define SVCTX_SVC_SELECT_FLAG_AV            ((UINT32) 0x2)    /**< select av flag */
#endif

#define SVCTX_COMMON_GET_SCREEN_STATUS                    ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |28)) /**< type = SVCTX_SCREEN_STATUS_T*   size = sizeof(SVCTX_SCREEN_STATUS_T) */    
#define SVCTX_COMMON_GET_CRNT_SVL_REC_ID                  ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |29)) /**< type = UINT16*                     size = sizeof(UINT16)   */    
#define SVCTX_COMMON_GET_CRNT_SVL_ID                      ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |30)) /**< type = UINT16*                     size = sizeof(UINT16)   */    
#define SVCTX_COMMON_GET_TUNER_NOTIFIED_FREQ              ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |31)) /**< type = UINT32*                     size = sizeof(UINT32)   */    
#define SVCTX_COMMON_GET_ANA_AUTO_DETECTED_AUD_SYS        ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |32)) /**< type = SVCTX_ANA_AUTO_DETECTED_AUD_SYS_T*  size = sizeof(SVCTX_ANA_AUTO_DETECTED_AUD_SYS_T)   */    

#ifdef SYS_MHP_SUPPORT
#define SVCTX_COMMON_GET_AV_STATUS                        ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |33)) /**< type = SVC_AV_STATUS_T*  size = sizeof(SVC_AV_STATUS_T)   */     
#endif /*  SYS_MHP_SUPPORT */

#define SVCTX_COMMON_GET_AD_RECS_NUM                      ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |34)) /**< type = UINT16*                     size = sizeof(UINT16)   */    
#define SVCTX_COMMON_GET_AD_RECS                          ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |35)) /**< type = SCDB_REC_T*                 size = sizeof(SCDB_REC_T)*num  */    
#define SVCTX_COMMON_GET_AD_USING_REC                     ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |36)) /**< type = SCDB_REC_T*                 size = sizeof(SCDB_REC_T)  */    
#define SVCTX_COMMON_GET_AD_SUITABILITY                   ((SVCTX_GET_TYPE_T)(SVCTX_GRP_COMMON |37)) /**< type = BOOL*                       size = sizeof(BOOL)  */    


typedef struct _SVCTX_MHEG_SCREEN_MODE_T
{
    BOOL                    b_enable;
    SCC_VID_SCREEN_MODE_T   e_scr_mode;
} SVCTX_MHEG_SCREEN_MODE_T;

typedef struct _SVCTX_ANA_AUTO_DETECTED_AUD_SYS_T
{
    UINT32                                  ui4_aud_sys;
    UINT32                                  ui4_tv_sys;
} SVCTX_ANA_AUTO_DETECTED_AUD_SYS_T;


typedef struct _SVCTX_SCDB_SORT_PARAM_T
{
    x_scdb_sort_cmp                         pf_scdb_sort_fct;
    VOID*                                   pv_tag;
} SVCTX_SCDB_SORT_PARAM_T;


/*------------------------------------------------------------------*/
/*! @struct SVCTX_ATOMIC_CMD_T
 *  @brief  Both set VSH_REGION_INFO_T && SCC_VID_SCREEN_MODE_T.
 *  @code
 *  typedef struct _SVCTX_ATOMIC_CMD_T
 *  {
 *      SVCTX_SET_TYPE_T    t_type;
 *      VOID*               pv_data;
 *  } SVCTX_ATOMIC_CMD_T;
 *  @endcode
 *  @li@c  t_type                                            SVCTX_SET_TYPE_T - Svctx common set type.
 *  @li@c  pv_data                                           VOID* - include region and mode info.
 */
/*------------------------------------------------------------------*/
typedef struct _SVCTX_ATOMIC_CMD_T
{
    SVCTX_SET_TYPE_T    t_type;
    VOID*               pv_data;
} SVCTX_ATOMIC_CMD_T;


/*------------------------------------------------------------------*/
/*! @struct SVCTX_VID_STREAM_T
 *  @brief  This structure defines stream info for svctx.
 *  @code
 *  typedef struct _SVCTX_VID_STREAM_T
 *  {
 *      VID_ENC_T               e_vid_enc;
 *  }   SVCTX_VID_STREAM_T;
 *  @endcode
 *  @li@c  e_vid_enc                                         VID_ENC_T - multi media type.
 */
/*------------------------------------------------------------------*/
typedef struct _SVCTX_VID_STREAM_T
{
    VID_ENC_T               e_vid_enc;
}   SVCTX_VID_STREAM_T;

/*------------------------------------------------------------------*/
/*! @struct SVCTX_AUD_STREAM_T
 *  @brief  This structure defines audio encode type.
 *  @code
 *  typedef struct _SVCTX_AUD_STREAM_T
 *  {
 *      AUD_ENC_T               e_aud_enc;
 *  }   SVCTX_AUD_STREAM_T;
 *  @endcode
 *  @li@c  e_aud_enc                                         AUD_ENC_T - audio encode type.
 */
/*------------------------------------------------------------------*/
typedef struct _SVCTX_AUD_STREAM_T
{
    AUD_ENC_T               e_aud_enc;
}   SVCTX_AUD_STREAM_T;

/*------------------------------------------------------------------*/
/*! @struct SVCTX_STREAM_INFO_T
 *  @brief  This structure defines stream information.
 *  @code
 *  typedef struct _SVCTX_STREAM_INFO_T
 *  {
 *      STREAM_TYPE_T           e_strm_type;
 *      MPEG_2_PID_T            t_mpeg_2_pid;
 *      union
 *      {
 *          SVCTX_VID_STREAM_T  t_vid_stream;
 *          SVCTX_AUD_STREAM_T  t_aud_stream;
 *      } u;
 *  }   SVCTX_STREAM_INFO_T;
 *  @endcode
 *  @li@c  e_strm_type                                       STREAM_TYPE_T- Stream type. include audio, video and so on.
 *  @li@c  t_mpeg_2_pid                                      MPEG_2_PID_T- Component PID.
 *  @li@c  t_vid_stream                                      SVCTX_VID_STREAM_T- Video stream type.
 *  @li@c  t_aud_stream                                      SVCTX_AUD_STREAM_T- Audio stream type.
 */
/*------------------------------------------------------------------*/
typedef struct _SVCTX_STREAM_INFO_T
{
    STREAM_TYPE_T           e_strm_type;
    MPEG_2_PID_T            t_mpeg_2_pid;
    union
    {
        SVCTX_VID_STREAM_T  t_vid_stream;
        SVCTX_AUD_STREAM_T  t_aud_stream;
    } u;
}   SVCTX_STREAM_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct SVCTX_VIDEO_MUTE_INFO_T
 *  @brief  video mute info.
 *  @code
 *  typedef struct _SVCTX_VIDEO_MUTE_INFO_T
 *  {
 *      BOOL                b_enable;
 *      SCC_BG_COLOR_T      t_color;
 *  }   SVCTX_VIDEO_MUTE_INFO_T;
 *  @endcode
 *  @li@c  b_enable                                          b_enable - open mute.
 *  @li@c  t_color                                           t_color - mute color.
 */
/*------------------------------------------------------------------*/
typedef struct _SVCTX_VIDEO_MUTE_INFO_T
{
    BOOL                b_enable;
    SCC_BG_COLOR_T      t_color;
}   SVCTX_VIDEO_MUTE_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct SVCTX_GET_SCDB_NUM_RECS_T
 *  @brief  get using scdb record number.
 *  @code
 *  typedef struct _SVCTX_GET_SCDB_NUM_RECS_T
 *  {   
 *      STREAM_TYPE_T       e_pipe_type;   
 *      STREAM_TYPE_T       e_strm_type;   
 *      UINT16              ui2_num_recs;  
 *  } SVCTX_GET_SCDB_NUM_RECS_T;
 *  @endcode
 *  @li@c  e_pipe_type                                       e_pipe_type - ref STREAM_TYPE_T.
 *  @li@c  e_strm_type                                       e_strm_type - ref STREAM_TYPE_T.
 *  @li@c  ui2_num_recs                                      ui2_num_recs - record number.
 */
/*------------------------------------------------------------------*/
typedef struct _SVCTX_GET_SCDB_NUM_RECS_T
{   /* a wraper of c_scdb_get_num_recs */
    STREAM_TYPE_T       e_pipe_type;   /* input; identify pipe type; for TV source, this is ignore since only one pipe */
    STREAM_TYPE_T       e_strm_type;   /* input  */
    UINT16              ui2_num_recs;  /* output */
} SVCTX_GET_SCDB_NUM_RECS_T;

/*------------------------------------------------------------------*/
/*! @struct SVCTX_GET_SCDB_REC_BY_IDX_T
 *  @brief  get using scdb record by index.
 *  @code
 *  typedef struct _SVCTX_GET_SCDB_REC_BY_IDX_T
 *  {   
 *      STREAM_TYPE_T       e_pipe_type;   
 *      STREAM_TYPE_T       e_strm_type;   
 *      UINT16              ui2_idx;       
 *      STREAM_COMP_ID_T    t_strm_comp_id;
 *      SCDB_REC_T          t_scdb_rec;    
 *  } SVCTX_GET_SCDB_REC_BY_IDX_T;
 *  @endcode
 *  @li@c  e_pipe_type                                       e_pipe_type - ref STREAM_TYPE_T.
 *  @li@c  e_strm_type                                       e_strm_type - ref STREAM_TYPE_T.
 *  @li@c  ui2_idx                                           ui2_idx - index in records.
 *  @li@c  t_strm_comp_id                                    t_strm_comp_id - ref STREAM_COMP_ID_T.
 *  @li@c  t_scdb_rec                                        t_scdb_rec - ref SCDB_REC_T.
 */
/*------------------------------------------------------------------*/
typedef struct _SVCTX_GET_SCDB_REC_BY_IDX_T
{   /* a wraper of c_scdb_get_rec_by_idx */
    STREAM_TYPE_T       e_pipe_type;   /* input; identify pipe type; for TV source, this is ignore since only one pipe */
    STREAM_TYPE_T       e_strm_type;   /* input  */
    UINT16              ui2_idx;       /* input  */
    STREAM_COMP_ID_T    t_strm_comp_id;/* output */
    SCDB_REC_T          t_scdb_rec;    /* output */
} SVCTX_GET_SCDB_REC_BY_IDX_T;

/*------------------------------------------------------------------*/
/*! @struct SVCTX_GET_USING_SCDB_REC_T
 *  @brief  get using scdb record.
 *  @code
 *  typedef struct _SVCTX_GET_USING_SCDB_REC_T
 *  {
 *      STREAM_TYPE_T       e_pipe_type;   
 *      STREAM_TYPE_T       e_strm_type;   
 *      UINT16              ui2_idx;       
 *      STREAM_COMP_ID_T    t_strm_comp_id;
 *      SCDB_REC_T          t_scdb_rec;    
 *  } SVCTX_GET_USING_SCDB_REC_T;
 *  @endcode
 *  @li@c  e_pipe_type                                       e_pipe_type - ref STREAM_TYPE_T.
 *  @li@c  e_strm_type                                       e_strm_type - ref STREAM_TYPE_T.
 *  @li@c  ui2_idx                                           ui2_idx - index in records.
 *  @li@c  t_strm_comp_id                                    t_strm_comp_id - ref STREAM_COMP_ID_T.
 *  @li@c  t_scdb_rec                                        t_scdb_rec - ref SCDB_REC_T.
 */
/*------------------------------------------------------------------*/
typedef struct _SVCTX_GET_USING_SCDB_REC_T
{
    STREAM_TYPE_T       e_pipe_type;   /* input; identify pipe type; for TV source, this is ignore since only one pipe */
    STREAM_TYPE_T       e_strm_type;   /* input  */
    UINT16              ui2_idx;       /* output */
    STREAM_COMP_ID_T    t_strm_comp_id;/* output */
    SCDB_REC_T          t_scdb_rec;    /* output */
} SVCTX_GET_USING_SCDB_REC_T;

#ifdef MW_TV_AV_BYPASS_SUPPORT
/*------------------------------------------------------------------*/
/*! @struct SVCTX_COPY_PROTECTION_T
 *  @brief  copy protection information.
 *  @code
 *  typedef struct _SVCTX_COPY_PROTECTION_T
 *  {
 *      UINT32              ui4_macrovision;
 *      UINT32              ui4_hdcp;
 *      UINT32              ui4_ca;
 *  }   SVCTX_COPY_PROTECTION_T;
 *  @endcode
 *  @li@c  ui4_macrovision                                   ui4_macrovision - exist macrovision, cann't copy.
 *  @li@c  ui4_hdcp                                          ui4_hdcp  - exist hdcp, cann't copy.
 *  @li@c  ui4_ca                                            ui4_ca - exist ca, cann't copy.
 */
/*------------------------------------------------------------------*/
typedef struct _SVCTX_COPY_PROTECTION_T
{
    UINT32              ui4_macrovision;
    UINT32              ui4_hdcp;
    UINT32              ui4_ca;
}   SVCTX_COPY_PROTECTION_T;
#endif

/* service context states */
/*------------------------------------------------------------------*/
/*! @enum   SVCTX_COND_T
 *  @brief  svctx status.
 *  @code
 *  typedef enum
 *  {
 *      SVCTX_COND_POOLED = 0,
 *      SVCTX_COND_READY,
 *      SVCTX_COND_PENDING,
 *      SVCTX_COND_PRESENTING,
 *      SVCTX_COND_PENDING_SELECT_SAME_REQ,    
 *      SVCTX_COND_PRESENTING_SELECT_SAME_REQ, 
 *      SVCTX_COND_LAST_VALID_ENTRY, 
 *      SVCTX_COND_INVALID
 *  } SVCTX_COND_T;
 *  @endcode
 *  @li@c  SVCTX_COND_POOLED                                 SVCTX_COND_POOLED - not using yet.
 *  @li@c  SVCTX_COND_READY                                  SVCTX_COND_READY - ready use, only init field.
 *  @li@c  SVCTX_COND_PENDING                                SVCTX_COND_PENDING - selecting.
 *  @li@c  SVCTX_COND_PRESENTING                             SVCTX_COND_PRESENTING - started.
 *  @li@c  SVCTX_COND_PENDING_SELECT_SAME_REQ                SVCTX_COND_PENDING_SELECT_SAME_REQ - continue select same channel, and first select was not started.
 *  @li@c  SVCTX_COND_PRESENTING_SELECT_SAME_REQ             SVCTX_COND_PRESENTING_SELECT_SAME_REQ - continue select same channel, and first select was started.
 *  @li@c  SVCTX_COND_LAST_VALID_ENTRY                       SVCTX_COND_LAST_VALID_ENTRY - Ingnor.
 *  @li@c  SVCTX_COND_INVALID                                SVCTX_COND_INVALID - svctx was destroied.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SVCTX_COND_POOLED = 0,
    SVCTX_COND_READY,
    SVCTX_COND_PENDING,
    SVCTX_COND_PRESENTING,
    SVCTX_COND_PENDING_SELECT_SAME_REQ,    /* only for identify the notification is for the same request */
    SVCTX_COND_PRESENTING_SELECT_SAME_REQ, /* after the notification are sent, the state will be return to previous one */
    SVCTX_COND_LAST_VALID_ENTRY, /* only for counting purpose */
    SVCTX_COND_INVALID
} SVCTX_COND_T;

/*------------------------------------------------------------------*/
/*! @enum   STRM_REQ_TYPE_T
 *  @brief  Stream request type.
 *  @code
 *  typedef enum
 *  {
 *      STRM_REQ_SCDB = 0,
 *      STRM_REQ_PID
 *  } STRM_REQ_TYPE_T;
 *  @li@c   STRM_REQ_SCDB                      STRM_REQ_SCDB - stream selecting req using SCDB record.
 *  @li@c   STRM_REQ_PID                       STRM_REQ_PID  - stream selecting req using PID info.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    STRM_REQ_SCDB = 0,
    STRM_REQ_PID
} STRM_REQ_TYPE_T;

/*------------------------------------------------------------------*/
/*! @enum   SVCTX_NTFY_CODE_T
 *  @brief  <++>
 *  @code
 *  typedef enum
 *  {
 *      SVCTX_NTFY_CODE_NONE = -1,                   
 *      SVCTX_NTFY_CODE_NORMAL = 0,                  
 *      SVCTX_NTFY_CODE_SIGNAL_DETECTING,            
 *                                                    * It is only valid for HDMI source for current turn-key driver, but the mechansim
 *                                                    * middleware is ready for all analog sources.
 *                                                    * 2008/3/2 weiderchang
 *      SVCTX_NTFY_CODE_SIGNAL_LOCKED,               
 *      SVCTX_NTFY_CODE_SIGNAL_LOSS,                 
 *      SVCTX_NTFY_CODE_INTERRUPTED,                 
 *      SVCTX_NTFY_CODE_SERVICE_CHANGING,            
 *      SVCTX_NTFY_CODE_OWN_SERVICE_CHANGING,        
 *      SVCTX_NTFY_CODE_SERVICE_BLOCKED,             
 *      SVCTX_NTFY_CODE_SIGNAL_LOSS_AS_BLOCKED,      
 *      SVCTX_NTFY_CODE_SIGNAL_LOCKED_AS_BLOCKED,    
 *      SVCTX_NTFY_CODE_SERVICE_UNBLOCKED,           
 *      SVCTX_NTFY_CODE_STOPPED,                     
 *      SVCTX_NTFY_CODE_IN_STOPPING,                 
 *      SVCTX_NTFY_CODE_CRNT_EVN_START_QUERY,
 *      SVCTX_NTFY_CODE_CRNT_EVN_NOT_READY,
 *      SVCTX_NTFY_CODE_CRNT_EVN_READY,
 *      SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC,
 *      SVCTX_NTFY_CODE_AUDIO_ONLY_SVC,
 *      SVCTX_NTFY_CODE_VIDEO_ONLY_SVC,
 *      SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC,
 *      SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_VIDEO_SVC,
 *      SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_CLEAR_VIDEO_SVC,
 *      SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_NO_VIDEO_SVC,
 *      SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_CLEAR_AUDIO_SVC,
 *      SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_NO_AUDIO_SVC,
 *      SVCTX_NTFY_CODE_WITH_CC_AVIL_INDICATOR,
 *      SVCTX_NTFY_CODE_NO_CC_AVIL_INDICATOR,
 *      SVCTX_NTFY_CODE_EXISTED_CC_IND_CHG,
 *      SVCTX_NTFY_CODE_NO_ANY_STREAM,
 *      SVCTX_NTFY_CODE_NO_STREAM,
 *      SVCTX_NTFY_CODE_STREAM_STOPPED,
 *      SVCTX_NTFY_CODE_STREAM_STARTED,
 *      SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
 *      SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE,
 *      SVCTX_NTFY_CODE_VIDEO_AUTO_CLK_POS_PHS_DONE,
 *      SVCTX_NTFY_CODE_VIDEO_AUTO_COLOR_DONE,
 *      SVCTX_NTFY_CODE_VIDEO_ANA_COLOR_SYSTEM,
 *      SVCTX_NTFY_CODE_STREAM_STOPPED_AS_BLOCKED,
 *      SVCTX_NTFY_CODE_STREAM_STARTED_AS_BLOCKED,
 *      SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE_AS_BLOCKED,
 *      SVCTX_NTFY_CODE_AUDIO_AUTO_MUTE,
 *      SVCTX_NTFY_CODE_AUDIO_AUTO_UNMUTE,
 *      SVCTX_NTFY_CODE_VIDEO_AUTO_MUTE,
 *      SVCTX_NTFY_CODE_VIDEO_AUTO_UNMUTE,
 *      SVCTX_NTFY_CODE_SCDB_ADD,
 *      SVCTX_NTFY_CODE_SCDB_DEL,
 *      SVCTX_NTFY_CODE_SCDB_MODIFY,
 *      SVCTX_NTFY_CODE_NO_RESOURCES,
 *      SVCTX_NTFY_CODE_INTERNAL_ERROR,
 *      SVCTX_NTFY_CODE_CA, 
 *      SVCTX_NTFY_CODE_CP, 
 *      SVCTX_NTFY_CODE_TUNER_SIGNAL_LOCKED,
 *      SVCTX_NTFY_CODE_TUNER_SIGNAL_LOSS,
 *      SVCTX_NTFY_CODE_CH_SRC_FROM_TUNER,
 *      SVCTX_NTFY_CODE_CH_SRC_FROM_SCART,
 *      SVCTX_NTFY_CODE_RF_SCAN_REQUIRED,
 *      SVCTX_NTFY_CODE_UNFREEZE,
 *      SVCTX_NTFY_CODE_TV_SYS_DETECTED,
 *      SVCTX_NTFY_CODE_AUD_SYS_DETECTED,
 *      SVCTX_NTFY_CODE_START_ALTER_FREQ_TUNE,
 *      SVCTX_NTFY_CODE_END_ALTER_FREQ_TUNE,
 *      SVCTX_NTFY_CODE_START_FIRST_TUNE,
 *      SVCTX_NTFY_CODE_END_FIRST_TUNE,
 *      SVCTX_NTFY_CODE_START_FREQ_LIST_UPDATED_CHK,
 *      SVCTX_NTFY_CODE_END_FREQ_LIST_UPDATED_CHK,
 *      SVCTX_NTFY_CODE_MEDIA_LOST,           
 *      SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSING, 
 *      SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSED,  
 *      SVCTX_NTFY_CODE_MEDIA_PB_RANGE_ALLOC, 
 *      SVCTX_NTFY_CODE_MEDIA_ERROR,          
 *      SVCTX_NTFY_CODE_MEDIA_CHAPTER_UPDATE,
 *      SVCTX_NTFY_CODE_MEDIA_TITLE_UPDATE,
 *      SVCTX_NTFY_CODE_MEDIA_DURATION_UPDATE,
 *      SVCTX_NTFY_CODE_MEDIA_SPEED_UPDATE,
 *      SVCTX_NTFY_CODE_MEDIA_UOP_INVALID,
 *      SVCTX_NTFY_CODE_MEDIA_SEEK_READY,   
 *      SVCTX_NTFY_CODE_MEDIA_SEEK_ERROR,   
 *      SVCTX_NTFY_CODE_MEDIA_REPEAT_DONE,    
 *      SVCTX_NTFY_CODE_MEDIA_MENU_PRESNT,    
 *      SVCTX_NTFY_CODE_MEDIA_EOS,            
 *      SVCTX_NTFY_CODE_MEDIA_AS_CODEC_NOT_SUPPORT,
 *      SVCTX_NTFY_CODE_MEDIA_VS_CODEC_NOT_SUPPORT,
 *      SVCTX_NTFY_CODE_MEDIA_THUMBNAIL_DONE,  
 *      SVCTX_NTFY_CODE_MEDIA_HD_NOT_SUPPORT,
 *      SVCTX_NTFY_CODE_MEDIA_DRM_NOT_SUPPORT,
 *      SVCTX_NTFY_CODE_MEDIA_SUBTITLE_FMT_UPDATE,
 *      SVCTX_NTFY_CODE_REACH_VALID_RANGE_START,
 *      SVCTX_NTFY_CODE_REACH_VALID_RANGE_END,
 *      SVCTX_NTFY_CODE_MEDIA_BAD_TICK_Y_TO_N,
 *      SVCTX_NTFY_CODE_MEDIA_BAD_TICK_N_TO_Y,
 *      SVCTX_NTFY_CODE_MEDIA_FILE_NOT_SUPPORT,
 *      SVCTX_NTFY_CODE_MEDIA_RESOLUTION_NOT_SUPPORT,
 *      SVCTX_NTFY_CODE_MEDIA_FRAMERATE_NOT_SUPPORT,
 *      SVCTX_NTFY_CODE_MEDIA_NO_CONTENT,
 *      SVCTX_NTFY_CODE_HDCP_DETECT,
 *      SVCTX_NTFY_CODE_HDCP_NOT_DETECT,
 *      SVCTX_NTFY_CODE_MACROVISION_DETECT,
 *      SVCTX_NTFY_CODE_MACROVISION_NOT_DETECT,
 *  #ifdef MW_ISDB_SUPPORT
 *      SVCTX_NTFY_CODE_EWS_ON,
 *      SVCTX_NTFY_CODE_EWS_OFF,
 *      SVCTX_NTFY_CODE_BRDCSTING_SVC,
 *      SVCTX_NTFY_CODE_NON_BRDCSTING_SVC,
 *  #endif
 *  #ifdef SYS_RECORD_SUPPORT    
 *      SVCTX_NTFY_CODE_REC_STARTED,  
 *      SVCTX_NTFY_CODE_REC_STOPPED,
 *      SVCTX_NTFY_CODE_REC_PAUSED,
 *      SVCTX_NTFY_CODE_REC_REQ_ES_LIST,
 *      SVCTX_NTFY_CODE_REC_REACH_BOUND,
 *      SVCTX_NTFY_CODE_REC_STRG_FULL,
 *      SVCTX_NTFY_CODE_REC_BITRATE_TOO_HIGH,
 *      SVCTX_NTFY_CODE_REC_NOT_SUPPORTED,
 *      SVCTX_NTFY_CODE_REC_STRG_ERROR,
 *      SVCTX_NTFY_CODE_REC_INTERNAL_ERROR,
 *  #endif
 *      SVCTX_NTFY_CODE_LAST_ENTRY
 *  } SVCTX_NTFY_CODE_T;
 *  @endcode
 *  @li@c  SVCTX_NTFY_CODE_NONE                              SVCTX_NTFY_CODE_NONE - svctx internal using.
 *  @li@c  SVCTX_NTFY_CODE_NORMAL                            SVCTX_NTFY_CODE_NORMAL - svctx select channel normal.
 *  @li@c  SVCTX_NTFY_CODE_SIGNAL_DETECTING                  SVCTX_NTFY_CODE_SIGNAL_DETECTING - hdmi was detecing. only for hdmi.
 *  @li@c  SVCTX_NTFY_CODE_SIGNAL_LOCKED                     SVCTX_NTFY_CODE_SIGNAL_LOCKED - tv signal locked.
 *  @li@c  SVCTX_NTFY_CODE_SIGNAL_LOSS                       SVCTX_NTFY_CODE_SIGNAL_LOSS - tv signal loss.
 *  @li@c  SVCTX_NTFY_CODE_INTERRUPTED                       SVCTX_NTFY_CODE_INTERRUPTED - was stopping channel.
 *  @li@c  SVCTX_NTFY_CODE_SERVICE_CHANGING                  SVCTX_NTFY_CODE_SERVICE_CHANGING - before service change.
 *  @li@c  SVCTX_NTFY_CODE_OWN_SERVICE_CHANGING              SVCTX_NTFY_CODE_OWN_SERVICE_CHANGING - before owner service change.
 *  @li@c  SVCTX_NTFY_CODE_SERVICE_BLOCKED                   SVCTX_NTFY_CODE_SERVICE_BLOCKED - service was blocked.
 *  @li@c  SVCTX_NTFY_CODE_SIGNAL_LOSS_AS_BLOCKED            SVCTX_NTFY_CODE_SIGNAL_LOSS_AS_BLOCKED - blocked channel signal loss.
 *  @li@c  SVCTX_NTFY_CODE_SIGNAL_LOCKED_AS_BLOCKED          SVCTX_NTFY_CODE_SIGNAL_LOCKED_AS_BLOCKED - blocked channel signal locked.
 *  @li@c  SVCTX_NTFY_CODE_SERVICE_UNBLOCKED                 SVCTX_NTFY_CODE_SERVICE_UNBLOCKED - channel was unblocked.
 *  @li@c  SVCTX_NTFY_CODE_STOPPED                           SVCTX_NTFY_CODE_STOPPED - channel was stopped.
 *  @li@c  SVCTX_NTFY_CODE_IN_STOPPING                       SVCTX_NTFY_CODE_IN_STOPPING - channel was stopping.
 *  @li@c  SVCTX_NTFY_CODE_CRNT_EVN_START_QUERY              SVCTX_NTFY_CODE_CRNT_EVN_START_QUERY - start query current event.
 *  @li@c  SVCTX_NTFY_CODE_CRNT_EVN_NOT_READY                SVCTX_NTFY_CODE_CRNT_EVN_NOT_READY - current event don't ready.
 *  @li@c  SVCTX_NTFY_CODE_CRNT_EVN_READY                    SVCTX_NTFY_CODE_CRNT_EVN_READY - current event have ready.
 *  @li@c  SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC                SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC - no audio and no video service.
 *  @li@c  SVCTX_NTFY_CODE_AUDIO_ONLY_SVC                    SVCTX_NTFY_CODE_AUDIO_ONLY_SVC - audio only service.
 *  @li@c  SVCTX_NTFY_CODE_VIDEO_ONLY_SVC                    SVCTX_NTFY_CODE_VIDEO_ONLY_SVC - video only service.
 *  @li@c  SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC                   SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC - audio and video service.
 *  @li@c  SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_VIDEO_SVC         SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_VIDEO_SVC - scrambled audio and scrambled video.
 *  @li@c  SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_CLEAR_VIDEO_SVC   SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_CLEAR_VIDEO_SVC - scrambled audio and clear video.
 *  @li@c  SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_NO_VIDEO_SVC      SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_NO_VIDEO_SVC - scrambled audio and no video.
 *  @li@c  SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_CLEAR_AUDIO_SVC   SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_CLEAR_AUDIO_SVC - scrambled video and clear audio.
 *  @li@c  SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_NO_AUDIO_SVC      SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_NO_AUDIO_SVC - scrambled video and no audio.
 *  @li@c  SVCTX_NTFY_CODE_WITH_CC_AVIL_INDICATOR            SVCTX_NTFY_CODE_WITH_CC_AVIL_INDICATOR - cc indicator for check avil.
 *  @li@c  SVCTX_NTFY_CODE_NO_CC_AVIL_INDICATOR              SVCTX_NTFY_CODE_NO_CC_AVIL_INDICATOR - no cc indicator.
 *  @li@c  SVCTX_NTFY_CODE_EXISTED_CC_IND_CHG                SVCTX_NTFY_CODE_EXISTED_CC_IND_CHG - cc indicator changed.
 *  @li@c  SVCTX_NTFY_CODE_NO_ANY_STREAM                     SVCTX_NTFY_CODE_NO_ANY_STREAM - no any stream.
 *  @li@c  SVCTX_NTFY_CODE_NO_STREAM                         SVCTX_NTFY_CODE_NO_STREAM - no stream.
 *  @li@c  SVCTX_NTFY_CODE_STREAM_STOPPED                    SVCTX_NTFY_CODE_STREAM_STOPPED - stream was stopped.
 *  @li@c  SVCTX_NTFY_CODE_STREAM_STARTED                    SVCTX_NTFY_CODE_STREAM_STARTED - stream was started.
 *  @li@c  SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE                  SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE - video have update.
 *  @li@c  SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE                  SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE - audio have update.
 *  @li@c  SVCTX_NTFY_CODE_VIDEO_AUTO_CLK_POS_PHS_DONE       SVCTX_NTFY_CODE_VIDEO_AUTO_CLK_POS_PHS_DONE - video auto clk pos phs done.
 *  @li@c  SVCTX_NTFY_CODE_VIDEO_AUTO_COLOR_DONE             SVCTX_NTFY_CODE_VIDEO_AUTO_COLOR_DONE - video auto color done.
 *  @li@c  SVCTX_NTFY_CODE_VIDEO_ANA_COLOR_SYSTEM            SVCTX_NTFY_CODE_VIDEO_ANA_COLOR_SYSTEM - video ananlog color system
 *  @li@c  SVCTX_NTFY_CODE_STREAM_STOPPED_AS_BLOCKED         SVCTX_NTFY_CODE_STREAM_STOPPED_AS_BLOCKED - blocked stream was stopped.
 *  @li@c  SVCTX_NTFY_CODE_STREAM_STARTED_AS_BLOCKED         SVCTX_NTFY_CODE_STREAM_STARTED_AS_BLOCKED - blocked stream was started.
 *  @li@c  SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE_AS_BLOCKED       SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE_AS_BLOCKED - blocked video have update.
 *  @li@c  SVCTX_NTFY_CODE_AUDIO_AUTO_MUTE                   SVCTX_NTFY_CODE_AUDIO_AUTO_MUTE - audio auto mute.
 *  @li@c  SVCTX_NTFY_CODE_AUDIO_AUTO_UNMUTE                 SVCTX_NTFY_CODE_AUDIO_AUTO_UNMUTE - audio auto unmute.
 *  @li@c  SVCTX_NTFY_CODE_VIDEO_AUTO_MUTE                   SVCTX_NTFY_CODE_VIDEO_AUTO_MUTE - video auto mute.
 *  @li@c  SVCTX_NTFY_CODE_VIDEO_AUTO_UNMUTE                 SVCTX_NTFY_CODE_VIDEO_AUTO_UNMUTE - video auto unmute.
 *  @li@c  SVCTX_NTFY_CODE_SCDB_ADD                          SVCTX_NTFY_CODE_SCDB_ADD - a new component was add to scdb.
 *  @li@c  SVCTX_NTFY_CODE_SCDB_DEL                          SVCTX_NTFY_CODE_SCDB_DEL - a old component was deleted from scdb.
 *  @li@c  SVCTX_NTFY_CODE_SCDB_MODIFY                       SVCTX_NTFY_CODE_SCDB_MODIFY - scdb have update.
 *  @li@c  SVCTX_NTFY_CODE_NO_RESOURCES                      SVCTX_NTFY_CODE_NO_RESOURCES - no resources fro channel.
 *  @li@c  SVCTX_NTFY_CODE_INTERNAL_ERROR                    SVCTX_NTFY_CODE_INTERNAL_ERROR - svctx internal error.
 *  @li@c  SVCTX_NTFY_CODE_CA                                SVCTX_NTFY_CODE_CA - ca notify.
 *  @li@c  SVCTX_NTFY_CODE_CP                                SVCTX_NTFY_CODE_CP - cp notify.
 *  @li@c  SVCTX_NTFY_CODE_TUNER_SIGNAL_LOCKED               SVCTX_NTFY_CODE_TUNER_SIGNAL_LOCKED - tuner signal was locked.
 *  @li@c  SVCTX_NTFY_CODE_TUNER_SIGNAL_LOSS                 SVCTX_NTFY_CODE_TUNER_SIGNAL_LOSS - tuner signal was loss.
 *  @li@c  SVCTX_NTFY_CODE_CH_SRC_FROM_TUNER                 SVCTX_NTFY_CODE_CH_SRC_FROM_TUNER - ch source from tuner.
 *  @li@c  SVCTX_NTFY_CODE_CH_SRC_FROM_SCART                 SVCTX_NTFY_CODE_CH_SRC_FROM_SCART - ch source from scart.
 *  @li@c  SVCTX_NTFY_CODE_RF_SCAN_REQUIRED                  SVCTX_NTFY_CODE_RF_SCAN_REQUIRED - RF scan was required.
 *  @li@c  SVCTX_NTFY_CODE_UNFREEZE                          SVCTX_NTFY_CODE_UNFREEZE - unfreeze.
 *  @li@c  SVCTX_NTFY_CODE_TV_SYS_DETECTED                   SVCTX_NTFY_CODE_TV_SYS_DETECTED - analog tv system was detected.
 *  @li@c  SVCTX_NTFY_CODE_AUD_SYS_DETECTED                  SVCTX_NTFY_CODE_AUD_SYS_DETECTED - analog audio system was detected.
 *  @li@c  SVCTX_NTFY_CODE_START_ALTER_FREQ_TUNE,            SVCTX_NTFY_CODE_START_ALTER_FREQ_TUNE - tuner start alter frequency for tune.
 *  @li@c  SVCTX_NTFY_CODE_END_ALTER_FREQ_TUNE,              SVCTX_NTFY_CODE_END_ALTER_FREQ_TUNE - tuner end alfter frequency fro tune.
 *  @li@c  SVCTX_NTFY_CODE_START_FIRST_TUNE,                 SVCTX_NTFY_CODE_START_FIRST_TUNE - start tune first frequency.
 *  @li@c  SVCTX_NTFY_CODE_END_FIRST_TUNE,                   SVCTX_NTFY_CODE_END_FIRST_TUNE - end tune first frequency.
 *  @li@c  SVCTX_NTFY_CODE_START_FREQ_LIST_UPDATED_CHK,      SVCTX_NTFY_CODE_START_FREQ_LIST_UPDATED_CHK - start check frequency list update.
 *  @li@c  SVCTX_NTFY_CODE_END_FREQ_LIST_UPDATED_CHK,        SVCTX_NTFY_CODE_END_FREQ_LIST_UPDATED_CHK - end check frequency list update.
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_LOST                        SVCTX_NTFY_CODE_MEDIA_LOST - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSING              SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSING - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSED               SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSED - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_PB_RANGE_ALLOC              SVCTX_NTFY_CODE_MEDIA_PB_RANGE_ALLOC - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_ERROR                       SVCTX_NTFY_CODE_MEDIA_ERROR - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_CHAPTER_UPDATE              SVCTX_NTFY_CODE_MEDIA_CHAPTER_UPDATE - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_TITLE_UPDATE                SVCTX_NTFY_CODE_MEDIA_TITLE_UPDATE - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_DURATION_UPDATE             SVCTX_NTFY_CODE_MEDIA_DURATION_UPDATE - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_SPEED_UPDATE                SVCTX_NTFY_CODE_MEDIA_SPEED_UPDATE - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_UOP_INVALID                 SVCTX_NTFY_CODE_MEDIA_UOP_INVALID - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_SEEK_READY                  SVCTX_NTFY_CODE_MEDIA_SEEK_READY - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_SEEK_ERROR                  SVCTX_NTFY_CODE_MEDIA_SEEK_ERROR - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_REPEAT_DONE                 SVCTX_NTFY_CODE_MEDIA_REPEAT_DONE - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_MENU_PRESNT                 SVCTX_NTFY_CODE_MEDIA_MENU_PRESNT - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_EOS                         SVCTX_NTFY_CODE_MEDIA_EOS - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_AS_CODEC_NOT_SUPPORT        SVCTX_NTFY_CODE_MEDIA_AS_CODEC_NOT_SUPPORT - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_VS_CODEC_NOT_SUPPORT        SVCTX_NTFY_CODE_MEDIA_VS_CODEC_NOT_SUPPORT - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_THUMBNAIL_DONE              SVCTX_NTFY_CODE_MEDIA_THUMBNAIL_DONE - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_HD_NOT_SUPPORT              SVCTX_NTFY_CODE_MEDIA_HD_NOT_SUPPORT - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_DRM_NOT_SUPPORT             SVCTX_NTFY_CODE_MEDIA_DRM_NOT_SUPPORT - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_SUBTITLE_FMT_UPDATE         SVCTX_NTFY_CODE_MEDIA_SUBTITLE_FMT_UPDATE - <++>
 *  @li@c  SVCTX_NTFY_CODE_REACH_VALID_RANGE_START           SVCTX_NTFY_CODE_REACH_VALID_RANGE_START - <++>
 *  @li@c  SVCTX_NTFY_CODE_REACH_VALID_RANGE_END             SVCTX_NTFY_CODE_REACH_VALID_RANGE_END - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_BAD_TICK_Y_TO_N             SVCTX_NTFY_CODE_MEDIA_BAD_TICK_Y_TO_N - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_BAD_TICK_N_TO_Y              SVCTX_NTFY_CODE_MEDIA_BAD_TICK_N_TO_Y - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_FILE_NOT_SUPPORT            SVCTX_NTFY_CODE_MEDIA_FILE_NOT_SUPPORT - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_RESOLUTION_NOT_SUPPORT      SVCTX_NTFY_CODE_MEDIA_RESOLUTION_NOT_SUPPORT - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_FRAMERATE_NOT_SUPPORT       SVCTX_NTFY_CODE_MEDIA_FRAMERATE_NOT_SUPPORT - <++>
 *  @li@c  SVCTX_NTFY_CODE_MEDIA_NO_CONTENT                  SVCTX_NTFY_CODE_MEDIA_NO_CONTENT - <++>
 *  @li@c  SVCTX_NTFY_CODE_HDCP_DETECT                       SVCTX_NTFY_CODE_HDCP_DETECT - <++>
 *  @li@c  SVCTX_NTFY_CODE_HDCP_NOT_DETECT                   SVCTX_NTFY_CODE_HDCP_NOT_DETECT - <++>
 *  @li@c  SVCTX_NTFY_CODE_MACROVISION_DETECT                SVCTX_NTFY_CODE_MACROVISION_DETECT - <++>
 *  @li@c  SVCTX_NTFY_CODE_MACROVISION_NOT_DETECT            SVCTX_NTFY_CODE_MACROVISION_NOT_DETECT - <++>
 *  @li@c  SVCTX_NTFY_CODE_EWS_ON                            SVCTX_NTFY_CODE_EWS_ON - <++>
 *  @li@c  SVCTX_NTFY_CODE_EWS_OFF                           SVCTX_NTFY_CODE_EWS_OFF - <++>
 *  @li@c  SVCTX_NTFY_CODE_BRDCSTING_SVC                     SVCTX_NTFY_CODE_BRDCSTING_SVC - <++>
 *  @li@c  SVCTX_NTFY_CODE_NON_BRDCSTING_SVC                 SVCTX_NTFY_CODE_NON_BRDCSTING_SVC - <++>
 *  @li@c  SVCTX_NTFY_CODE_REC_STARTED                       SVCTX_NTFY_CODE_REC_STARTED - <++>
 *  @li@c  SVCTX_NTFY_CODE_REC_STOPPED                       SVCTX_NTFY_CODE_REC_STOPPED - <++>
 *  @li@c  SVCTX_NTFY_CODE_REC_PAUSED                        SVCTX_NTFY_CODE_REC_PAUSED - <++>
 *  @li@c  SVCTX_NTFY_CODE_REC_REQ_ES_LIST                   SVCTX_NTFY_CODE_REC_REQ_ES_LIST - <++>
 *  @li@c  SVCTX_NTFY_CODE_REC_REACH_BOUND                   SVCTX_NTFY_CODE_REC_REACH_BOUND - <++>
 *  @li@c  SVCTX_NTFY_CODE_REC_STRG_FULL                     SVCTX_NTFY_CODE_REC_STRG_FULL - <++>
 *  @li@c  SVCTX_NTFY_CODE_REC_BITRATE_TOO_HIGH              SVCTX_NTFY_CODE_REC_BITRATE_TOO_HIGH - <++>
 *  @li@c  SVCTX_NTFY_CODE_REC_NOT_SUPPORTED                 SVCTX_NTFY_CODE_REC_NOT_SUPPORTED - <++>
 *  @li@c  SVCTX_NTFY_CODE_REC_STRG_ERROR                    SVCTX_NTFY_CODE_REC_STRG_ERROR - <++>
 *  @li@c  SVCTX_NTFY_CODE_REC_INTERNAL_ERROR                SVCTX_NTFY_CODE_REC_INTERNAL_ERROR - <++>
 *  @li@c  SVCTX_NTFY_CODE_LAST_ENTRY                        SVCTX_NTFY_CODE_LAST_ENTRY - <++>
 */
/*------------------------------------------------------------------*/
typedef enum
{
    /* service */
    SVCTX_NTFY_CODE_NONE = -1,                   /* Initial value */
    SVCTX_NTFY_CODE_NORMAL = 0,                  /* service select request is ready to present */
    SVCTX_NTFY_CODE_SVCTX_THREAD_KICK_OFF,

    SVCTX_NTFY_CODE_DELIVERY_SYSTEM_CHANGE,
    
    SVCTX_NTFY_CODE_SIGNAL_DETECTING,            /* signal is detecting, after being detected, one of the following two will send
                                                  * It is only valid for HDMI source for current turn-key driver, but the mechansim
                                                  * middleware is ready for all analog sources.
                                                  * 2008/3/2 weiderchang
                                                  */
    SVCTX_NTFY_CODE_SIGNAL_LOCKED,               /* signal is locked */
    SVCTX_NTFY_CODE_SIGNAL_LOSS,                 /* signal is loss   */
    SVCTX_NTFY_CODE_INTERRUPTED,                 /* start to stop previous service select request                         */
    SVCTX_NTFY_CODE_SERVICE_CHANGING,            /* start to select current service select request but don't notfy issuer */
    SVCTX_NTFY_CODE_OWN_SERVICE_CHANGING,        /* start to select current service select request only notfy issuer      */
    SVCTX_NTFY_CODE_SERVICE_BLOCKED,             /* service request is blocked due to parental control                    */
    SVCTX_NTFY_CODE_SIGNAL_LOSS_AS_BLOCKED,      /* signal loss as the service is blocked                                 */
    SVCTX_NTFY_CODE_SIGNAL_LOCKED_AS_BLOCKED,    /* signal locked as the service is blocked                               */
    SVCTX_NTFY_CODE_SERVICE_UNBLOCKED,           /* service request is unblocked   */
    SVCTX_NTFY_CODE_STOPPED,                     /* service request is stopped     */
    SVCTX_NTFY_CODE_IN_STOPPING,                 /* service request is in stopping */
	SVCTX_NTFY_CODE_MEDIA_BOS,

    /* current event */
    SVCTX_NTFY_CODE_CRNT_EVN_START_QUERY,
    SVCTX_NTFY_CODE_CRNT_EVN_NOT_READY,
    SVCTX_NTFY_CODE_CRNT_EVN_READY,

    /* service type */
    SVCTX_NTFY_CODE_NO_AUDIO_VIDEO_SVC,
    SVCTX_NTFY_CODE_AUDIO_ONLY_SVC,
    SVCTX_NTFY_CODE_VIDEO_ONLY_SVC,
    SVCTX_NTFY_CODE_AUDIO_VIDEO_SVC,
    SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_VIDEO_SVC,
    SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_CLEAR_VIDEO_SVC,
    SVCTX_NTFY_CODE_SCRAMBLED_AUDIO_NO_VIDEO_SVC,
    SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_CLEAR_AUDIO_SVC,
    SVCTX_NTFY_CODE_SCRAMBLED_VIDEO_NO_AUDIO_SVC,
    SVCTX_NTFY_CODE_AUDIO_ONLY_STRM,
    SVCTX_NTFY_CODE_VIDEO_ONLY_STRM,

    /* cc availability */
    SVCTX_NTFY_CODE_WITH_CC_AVIL_INDICATOR,
    SVCTX_NTFY_CODE_NO_CC_AVIL_INDICATOR,
    SVCTX_NTFY_CODE_EXISTED_CC_IND_CHG,

    /* stream */
    SVCTX_NTFY_CODE_NO_ANY_STREAM,
    SVCTX_NTFY_CODE_NO_STREAM,
    SVCTX_NTFY_CODE_STREAM_STOPPED,
    SVCTX_NTFY_CODE_STREAM_STARTED,
    SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE,
    SVCTX_NTFY_CODE_AUDIO_FMT_UPDATE,
    SVCTX_NTFY_CODE_VIDEO_AUTO_CLK_POS_PHS_DONE,
    SVCTX_NTFY_CODE_VIDEO_AUTO_COLOR_DONE,
    SVCTX_NTFY_CODE_VIDEO_ANA_COLOR_SYSTEM,
    SVCTX_NTFY_CODE_STREAM_STOPPED_AS_BLOCKED,
    SVCTX_NTFY_CODE_STREAM_STARTED_AS_BLOCKED,
    SVCTX_NTFY_CODE_VIDEO_FMT_UPDATE_AS_BLOCKED,
    SVCTX_NTFY_CODE_AUDIO_DMUTE_AMUTE,
    SVCTX_NTFY_CODE_AUDIO_DUNMUTE_AMUTE,
    SVCTX_NTFY_CODE_AUDIO_DUNMUTE_AUNMUTE,
    SVCTX_NTFY_CODE_AUDIO_DMUTE_AUNMUTE,

    SVCTX_NTFY_CODE_AUDIO_AUTO_MUTE,
    SVCTX_NTFY_CODE_AUDIO_AUTO_UNMUTE,
    SVCTX_NTFY_CODE_VIDEO_AUTO_MUTE,
    SVCTX_NTFY_CODE_VIDEO_AUTO_UNMUTE,
    SVCTX_NTFY_CODE_VIDEO_PLAY_DONE,

    /* SCDB (PMT) */
    SVCTX_NTFY_CODE_SCDB_ADD,
    SVCTX_NTFY_CODE_SCDB_DEL,
    SVCTX_NTFY_CODE_SCDB_MODIFY,

    /* underneath middlewares' error */
    SVCTX_NTFY_CODE_NO_RESOURCES,
    SVCTX_NTFY_CODE_INTERNAL_ERROR,

    /* CA/CP */
    SVCTX_NTFY_CODE_CA, /* please use c_svctx_get_ca_info() for details information */
    SVCTX_NTFY_CODE_CP, /* please use c_svctx_get_cp_info() for details information */

    /* tuner singal notify for PAL/SECAM */
    SVCTX_NTFY_CODE_TUNER_SIGNAL_LOCKED,
    SVCTX_NTFY_CODE_TUNER_SIGNAL_LOSS,

    /* scart bypass - only sent when analog channel for EU */
    SVCTX_NTFY_CODE_CH_SRC_FROM_TUNER,
    SVCTX_NTFY_CODE_CH_SRC_FROM_SCART,

    /* single RF scan required */
    SVCTX_NTFY_CODE_RF_SCAN_REQUIRED,

    /* unfreeze by SVCTX */
    SVCTX_NTFY_CODE_UNFREEZE,

    /* for analog whatever system detected */
    SVCTX_NTFY_CODE_TV_SYS_DETECTED,
    SVCTX_NTFY_CODE_AUD_SYS_DETECTED,

    /* for Riks TV */
    SVCTX_NTFY_CODE_START_ALTER_FREQ_TUNE,
    SVCTX_NTFY_CODE_END_ALTER_FREQ_TUNE,
    SVCTX_NTFY_CODE_START_FIRST_TUNE,
    SVCTX_NTFY_CODE_END_FIRST_TUNE,
    SVCTX_NTFY_CODE_START_FREQ_LIST_UPDATED_CHK,
    SVCTX_NTFY_CODE_END_FREQ_LIST_UPDATED_CHK,

    /* Multimedia relative events */
    SVCTX_NTFY_CODE_MEDIA_LOST,           /*Media couldn't be found in device or device is unplug */
    SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSING, /*Media content processing*/
    SVCTX_NTFY_CODE_MEDIA_PRE_PROCESSED,  /*Media content processing done*/
    SVCTX_NTFY_CODE_MEDIA_PB_RANGE_ALLOC, /*allocate media range for presentation*/
    SVCTX_NTFY_CODE_MEDIA_ERROR,          /*Media content error*/
    SVCTX_NTFY_CODE_MEDIA_CHAPTER_UPDATE,
    SVCTX_NTFY_CODE_MEDIA_TITLE_UPDATE,
    SVCTX_NTFY_CODE_MEDIA_DURATION_UPDATE,
    SVCTX_NTFY_CODE_MEDIA_SPEED_UPDATE,
    SVCTX_NTFY_CODE_MEDIA_AB_REPEAT_DONE,
    SVCTX_NTFY_CODE_MEDIA_UOP_INVALID,
    SVCTX_NTFY_CODE_MEDIA_SEEK_READY,   /*Media search got done*/
    SVCTX_NTFY_CODE_MEDIA_SEEK_ERROR,   /*Media search got error*/
    SVCTX_NTFY_CODE_MEDIA_REPEAT_DONE,    /*One media repeat happened(chapter/title/all)*/
    SVCTX_NTFY_CODE_MEDIA_MENU_PRESNT,    /*menu presentation by navigation*/
    SVCTX_NTFY_CODE_MEDIA_EOS,            /*Media playback to the end*/
    SVCTX_NTFY_CODE_MEDIA_AS_CODEC_NOT_SUPPORT,
    SVCTX_NTFY_CODE_MEDIA_VS_CODEC_NOT_SUPPORT,
    SVCTX_NTFY_CODE_MEDIA_THUMBNAIL_DONE,  /* Video Thumbnail Gotten */
    SVCTX_NTFY_CODE_MEDIA_HD_NOT_SUPPORT,
    SVCTX_NTFY_CODE_MEDIA_DRM_NOT_SUPPORT,
    SVCTX_NTFY_CODE_MEDIA_SUBTITLE_FMT_UPDATE,
    SVCTX_NTFY_CODE_REACH_VALID_RANGE_START,
    SVCTX_NTFY_CODE_REACH_VALID_RANGE_END,
    SVCTX_NTFY_CODE_MEDIA_BAD_TICK_Y_TO_N,
    SVCTX_NTFY_CODE_MEDIA_BAD_TICK_N_TO_Y,
    SVCTX_NTFY_CODE_MEDIA_FILE_NOT_SUPPORT,
    SVCTX_NTFY_CODE_MEDIA_RESOLUTION_NOT_SUPPORT,
    SVCTX_NTFY_CODE_MEDIA_FRAMERATE_NOT_SUPPORT,
    SVCTX_NTFY_CODE_MEDIA_NO_CONTENT,
    SVCTX_NTFY_CODE_MEDIA_RANGE_AB_DONE,            
    SVCTX_NTFY_CODE_MEDIA_VIDEO_PRESENT,
    SVCTX_NTFY_CODE_MEDIA_VS_BIT_RATE,
    SVCTX_NTFY_CODE_MEDIA_REPLAY,
    SVCTX_NTFY_CODE_MEDIA_URI_FILE_OPENED,
    SVCTX_NTFY_CODE_MEDIA_DESC_ALLOCED,
    /* input source relative events */
    SVCTX_NTFY_CODE_INP_SIGNAL_UNKNOWN,          /* input signal status is unknown */

    /* Copy protection */
    SVCTX_NTFY_CODE_HDCP_DETECT,
    SVCTX_NTFY_CODE_HDCP_NOT_DETECT,
    SVCTX_NTFY_CODE_MACROVISION_DETECT,
    SVCTX_NTFY_CODE_MACROVISION_NOT_DETECT,

#ifdef MW_ISDB_SUPPORT
    /* TMCC events */
    SVCTX_NTFY_CODE_EWS_ON,
    SVCTX_NTFY_CODE_EWS_OFF,

    /* Broadcasting&non-broadcasting events */
    SVCTX_NTFY_CODE_BRDCSTING_SVC,
    SVCTX_NTFY_CODE_NON_BRDCSTING_SVC,
#endif

#ifdef SYS_RECORD_SUPPORT    
    /* record svctx related notify codes */
    SVCTX_NTFY_CODE_REC_STARTED,
    SVCTX_NTFY_CODE_REC_STOPPED,
    SVCTX_NTFY_CODE_REC_PAUSED,
    SVCTX_NTFY_CODE_REC_REQ_ES_LIST,
    SVCTX_NTFY_CODE_REC_ES_AUTH_UPDATED,
    SVCTX_NTFY_CODE_REC_REACH_BOUND,
    SVCTX_NTFY_CODE_REC_UNAUTHORIZED,
    SVCTX_NTFY_CODE_REC_BLOCKED,
    SVCTX_NTFY_CODE_REC_STRG_FULL,
    SVCTX_NTFY_CODE_REC_BITRATE_TOO_HIGH,
    SVCTX_NTFY_CODE_REC_NOT_SUPPORTED,
    SVCTX_NTFY_CODE_REC_STRG_ERROR,
    SVCTX_NTFY_CODE_REC_INTERNAL_ERROR,
    SVCTX_NTFY_CODE_REC_BAD_TICK,
#endif
#if 1/*MKV DivXPlus*/
    SVCTX_NTFY_CODE_CHAPTER_CHANGE,
#endif
    SVCTX_NTFY_CODE_CONNECTION_ANA_FREQ,
    SVCTX_NTFY_CODE_ANA_AUTO_DETECTED_AUD_SYS,
    SVCTX_NTFY_CODE_BUF_READY,
    SVCTX_NTFY_CODE_MEDIA_TIME_UPDATE,
    SVCTX_NTFY_CODE_MEDIA_UNDERFLOW,
    SVCTX_NTFY_CODE_MEDIA_EOF,
    /* Add for CRDTV00332149 place it here for lest effect*/
    SVCTX_NTFY_CODE_STREAM_OPENED,
    SVCTX_NTFY_CODE_VIDEO_NUM_READY,

    /* Add DRM status */
    SVCTX_NTFY_CODE_DRM_STATUS_START,
    SVCTX_NTFY_CODE_DRM_NONE = SVCTX_NTFY_CODE_DRM_STATUS_START,
    SVCTX_NTFY_CODE_DRM_UNKNOWN_ERR,                 /* An unknown error has occured */    
    SVCTX_NTFY_CODE_DRM_LIC_EXPIRATION_ERR,          /* License has expired while playing */
    SVCTX_NTFY_CODE_DRM_SRVDATA_ACCESS_FAIL,         /* SOAP fault, Service data access failure */  
    SVCTX_NTFY_CODE_DRM_INV_BUSINESS_TKN,            /* SOAP fault, Invalid business token */ 
    SVCTX_NTFY_CODE_DRM_CERT_EXPIRED,                /* SOAP fault, Validity of client certificate has been expired and service-specific tolerance level can no longer allow it */
    SVCTX_NTFY_CODE_DRM_LIC_NOT_ALLOW,               /* SOAP fault, Service does not permit issuing license to specified target node */
    SVCTX_NTFY_CODE_DRM_SRV_END,                     /* Action Token Acquisition status, Service has been ended */
    SVCTX_NTFY_CODE_DRM_SRV_UNAVAIL,                 /* Action Token Acquisition status, Service is unavailable */
    SVCTX_NTFY_CODE_DRM_SRV_BUSY,                    /* Action Token Acquisition status, Service is busy */
    SVCTX_NTFY_CODE_DRM_DEV_NOT_REG,                 /* Action Token Acquisition status, Device has not been registered yet */
    SVCTX_NTFY_CODE_DRM_CONTENT_EXHAUSTED,           /* Action Token Acquisition status, Requested content has been exhausted */
    SVCTX_NTFY_CODE_DRM_UNKNOWN_SRV_ERR,             /* Action Token Acquisition status, Unknown server error */
    SVCTX_NTFY_CODE_DRM_INV_USR_AGENT,               /* Action Token Acquisition status, Invalid user agent */
    SVCTX_NTFY_CODE_DRM_UNSUPPORTED_VER,             /* Action Token Acquisition status, Unsupported version */
    SVCTX_NTFY_CODE_DRM_INV_CONTENT_TYPE,            /* Action Token Acquisition status, Invalid content-type */
    SVCTX_NTFY_CODE_DRM_INV_SRV_TKN,                 /* Action Token Acquisition status, Invalid service token */
    SVCTX_NTFY_CODE_DRM_INV_ASSET_ID,                /* Action Token Acquisition status, Invalid Asset ID */

    SVCTX_NTFY_CODE_DRM_INIT_ERROR,
    SVCTX_NTFY_CODE_DRM_LICENSE_EXPIRED_ERROR,
    SVCTX_NTFY_CODE_DRM_LICENSE_ACQUISITION_ERROR,
    SVCTX_NTFY_CODE_DRM_LICENSE_SERVER_CONNECTION_ERROR,
    SVCTX_NTFY_CODE_DRM_TOKEN_ACQUISITION_ERROR ,
    SVCTX_NTFY_CODE_DRM_CAD_ACQUISITION_ERROR,

    SVCTX_NTFY_CODE_LAST_ENTRY
} SVCTX_NTFY_CODE_T;

/*------------------------------------------------------------------*/
/*! @struct SVCTX_CA_INFO_T
 *  @brief  CA information.
 *  @code
 *  typedef struct _SVCTX_CA_INFO_T
 *  {
 *      SM_EVN_TYPE_T                   e_event; 
 *      CA_DETAIL_COND_T                e_code;  
 *  } SVCTX_CA_INFO_T;
 *  @endcode
 *  @li@c  e_event                                           e_event - ref SM_EVN_TYPE_T.
 *  @li@c  e_code                                            e_code - ref CA_DETAIL_COND_T.
 */
/*------------------------------------------------------------------*/
typedef struct _SVCTX_CA_INFO_T
{
    SM_EVN_TYPE_T                   e_event; /* refer to u_sm_event.h's SM_EVN_CA_COND_CLOSED - SM_EVN_CA_COND_NOT_AUTHORIZED*/
    CA_DETAIL_COND_T                e_code;  /* refer to u_prc_ca.h */
} SVCTX_CA_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum   SVCTX_CC_AVIL_COND_T
 *  @brief  CC avil condition
 *  @code
 *  typedef enum
 *  {
 *      SVCTX_CC_AVIL_COND_UNKNOW = 0,
 *      SVCTX_CC_AVIL_COND_YES,
 *      SVCTX_CC_AVIL_COND_NO
 *  } SVCTX_CC_AVIL_COND_T;
 *  @endcode
 *  @li@c  SVCTX_CC_AVIL_COND_UNKNOW                     SVCTX_CC_AVIL_COND_UNKNOW - cc is unknown.
 *  @li@c  SVCTX_CC_AVIL_COND_YES                            SVCTX_CC_AVIL_COND_YES - cc invalid.
 *  @li@c  SVCTX_CC_AVIL_COND_NO                             SVCTX_CC_AVIL_COND_NO - cc valid.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SVCTX_CC_AVIL_COND_UNKNOW = 0,
    SVCTX_CC_AVIL_COND_YES,
    SVCTX_CC_AVIL_COND_NO
} SVCTX_CC_AVIL_COND_T;

/*------------------------------------------------------------------*/
/*! @enum   SURF_MODE_T
 *  @brief  surffing mode
 *  @code
 *  typedef enum
 *  {
 *      SURF_MODE_THIS = 0,
 *      SURF_MODE_PREV,
 *      SURF_MODE_NEXT
 *  } SURF_MODE_T;
 *  @endcode
 *  @li@c  SURF_MODE_THIS                                    SURF_MODE_THIS - suffing current.
 *  @li@c  SURF_MODE_PREV                                    SURF_MODE_PREV - suffing prev.
 *  @li@c  SURF_MODE_NEXT                                    SURF_MODE_NEXT - suffing next.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SURF_MODE_THIS = 0,
    SURF_MODE_PREV,
    SURF_MODE_NEXT
} SURF_MODE_T;

/*------------------------------------------------------------------*/
/*! @enum   SVC_BLOCK_COND_T
 *  @brief  channel block condition.
 *  @code
 *  typedef enum
 *  {
 *      SVC_BLOCK_COND_EXCEED_RATING = 0,
 *      SVC_BLOCK_COND_USER_BLOCK_CH,
 *      SVC_BLOCK_COND_USER_BLOCK_INP,
 *      SVC_BLOCK_COND_PASSED,
 *      SVC_BLOCK_COND_PASSED_AND_NO_RATING_SET
 *  } SVC_BLOCK_COND_T;
 *  @endcode
 *  @li@c  SVC_BLOCK_COND_EXCEED_RATING                      SVC_BLOCK_COND_EXCEED_RATING - rating block.
 *  @li@c  SVC_BLOCK_COND_USER_BLOCK_CH                      SVC_BLOCK_COND_USER_BLOCK_CH - channel block.
 *  @li@c  SVC_BLOCK_COND_USER_BLOCK_INP                     SVC_BLOCK_COND_USER_BLOCK_INP - input source block.
 *  @li@c  SVC_BLOCK_COND_PASSED                             SVC_BLOCK_COND_PASSED - not block.
 *  @li@c  SVC_BLOCK_COND_PASSED_AND_NO_RATING_SET           SVC_BLOCK_COND_PASSED_AND_NO_RATING_SET - rating wasn't set and not block.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SVC_BLOCK_COND_EXCEED_RATING = 0,
    SVC_BLOCK_COND_USER_BLOCK_CH,
    SVC_BLOCK_COND_USER_BLOCK_INP,
    SVC_BLOCK_COND_PASSED,
    SVC_BLOCK_COND_PASSED_AND_NO_RATING_SET
} SVC_BLOCK_COND_T;

/*------------------------------------------------------------------*/
/*! @enum   SIGNAL_STATUS_T
 *  @brief  signal status, for SVCTX_COMMON_GET_SIGNAL_STATUS
 *  @code
 *  typedef enum {
 *      SIGNAL_STATUS_UNKNOWN = 0,
 *      SIGNAL_STATUS_LOCKED,
 *      SIGNAL_STATUS_LOSS,
 *      SIGNAL_STATUS_DETECTING  
 *  } SIGNAL_STATUS_T;
 *  @endcode
 *  @li@c  SIGNAL_STATUS_UNKNOWN                             SIGNAL_STATUS_UNKNOWN - unknown.
 *  @li@c  SIGNAL_STATUS_LOCKED                              SIGNAL_STATUS_LOCKED - signal locked.
 *  @li@c  SIGNAL_STATUS_LOSS                                SIGNAL_STATUS_LOSS - signal loss.
 *  @li@c  SIGNAL_STATUS_DETECTING                           SIGNAL_STATUS_DETECTING - relative with SVCTX_NTFY_CODE_SIGNAL_DETECTING, and only for HDMI 2008/3/2.
 */
/*------------------------------------------------------------------*/
typedef enum {
    SIGNAL_STATUS_UNKNOWN = 0,
    SIGNAL_STATUS_LOCKED,
    SIGNAL_STATUS_LOSS,
    SIGNAL_STATUS_DETECTING
} SIGNAL_STATUS_T;

/*------------------------------------------------------------------*/
/*! @enum   SVCTX_SCREEN_STATUS_T
 *  @brief  screen status
 *  @code
 *  typedef enum {
 *      SVCTX_SCREEN_STATUS_SINGLE = 0,
 *      SVCTX_SCREEN_STATUS_MULTIPLE_FULL,
 *      SVCTX_SCREEN_STATUS_MULTIPLE_PART,
 *      SVCTX_SCREEN_STATUS_UNKNOWN
 *  } SVCTX_SCREEN_STATUS_T;
 *  @endcode
 *  @li@c  SVCTX_SCREEN_STATUS_SINGLE                        SVCTX_SCREEN_STATUS_T - normal.
 *  @li@c  SVCTX_SCREEN_STATUS_MULTIPLE_FULL                 SVCTX_SCREEN_STATUS_T - full in pap.
 *  @li@c  SVCTX_SCREEN_STATUS_MULTIPLE_PART                 SVCTX_SCREEN_STATUS_T - part in pap.
 *  @li@c  SVCTX_SCREEN_STATUS_UNKNOWN                       SVCTX_SCREEN_STATUS_T - unknown.
 */
/*------------------------------------------------------------------*/
typedef enum {
    SVCTX_SCREEN_STATUS_SINGLE = 0,
    SVCTX_SCREEN_STATUS_MULTIPLE_FULL,
    SVCTX_SCREEN_STATUS_MULTIPLE_PART,
    SVCTX_SCREEN_STATUS_UNKNOWN
} SVCTX_SCREEN_STATUS_T;
#ifdef SYS_MHP_SUPPORT
/*------------------------------------------------------------------*/
/*! @enum   SVC_AV_STATUS_T
 *  @brief  Service AV status
 *  @code
 *  typedef enum {
 *      SVC_AV_STATUS_AUDIO_ONLY = 0,
 *      SVC_AV_STATUS_VIDEO_ONLY,
 *      SVC_AV_STATUS_AUDIO_VIDEO,
 *      SVC_AV_STATUS_NO_AUDIO_VIDEO,
 *      SVC_AV_STATUS_SCRAMBLED_AUDIO_VIDEO,
 *      SVC_AV_STATUS_SCRAMBLED_AUDIO_CLEAR_VIDEO,
 *      SVC_AV_STATUS_SCRAMBLED_AUDIO_NO_VIDEO,
 *      SVC_AV_STATUS_SCRAMBLED_VIDEO_CLEAR_AUDIO,
 *      SVC_AV_STATUS_SCRAMBLED_VIDEO_NO_AUDIO,
 *      SVC_AV_STATUS_UNKNOWN
 *  } SVC_AV_STATUS_T;
 *  @endcode
 *  @li@c  SVC_AV_STATUS_AUDIO_ONLY                            SVC_AV_STATUS_T - Audio only
 *  @li@c  SVC_AV_STATUS_VIDEO_ONLY                            SVC_AV_STATUS_T - Video only
 *  @li@c  SVC_AV_STATUS_AUDIO_VIDEO                           SVC_AV_STATUS_T - Audio and video
 *  @li@c  SVC_AV_STATUS_NO_AUDIO_VIDEO                        SVC_AV_STATUS_T - No audio and video
 *  @li@c  SVC_AV_STATUS_SCRAMBLED_AUDIO_VIDEO                 SVC_AV_STATUS_T - Scrambled audio and video
 *  @li@c  SVC_AV_STATUS_SCRAMBLED_AUDIO_CLEAR_VIDEO           SVC_AV_STATUS_T - Scrambled audio and clear video
 *  @li@c  SVC_AV_STATUS_SCRAMBLED_AUDIO_NO_VIDEO              SVC_AV_STATUS_T - Scrambled audio and no video
 *  @li@c  SVC_AV_STATUS_SCRAMBLED_VIDEO_CLEAR_AUDIO           SVC_AV_STATUS_T - Scrambled video and clear audio
 *  @li@c  SVC_AV_STATUS_SCRAMBLED_VIDEO_NO_AUDIO              SVC_AV_STATUS_T - Scrambled video and no audio
 *  @li@c  SVC_AV_STATUS_UNKNOWN                               SVC_AV_STATUS_T - Unknown
 */        
/*------------------------------------------------------------------*/
typedef enum
{          
    SVC_AV_STATUS_AUDIO_ONLY = 0,
    SVC_AV_STATUS_VIDEO_ONLY,
    SVC_AV_STATUS_AUDIO_VIDEO,
    SVC_AV_STATUS_NO_AUDIO_VIDEO,
    SVC_AV_STATUS_SCRAMBLED_AUDIO_VIDEO,
    SVC_AV_STATUS_SCRAMBLED_AUDIO_CLEAR_VIDEO,
    SVC_AV_STATUS_SCRAMBLED_AUDIO_NO_VIDEO,
    SVC_AV_STATUS_SCRAMBLED_VIDEO_CLEAR_AUDIO,
    SVC_AV_STATUS_SCRAMBLED_VIDEO_NO_AUDIO,
    SVC_AV_STATUS_UNKNOWN
} SVC_AV_STATUS_T;
#endif /* SYS_MHP_SUPPORT */
/*------------------------------------------------------------------*/
/*! @enum   SVCTX_BLOCK_RULE_T
 *  @brief  Block rule mask item
 *  @code
 *  typedef enum
 *  {
 *      SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG = 0,
 *      SVCTX_BLOCK_RULE_INP_CH_SIGNAL_LOSS_PROG,
 *      SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS
 *  } SVCTX_BLOCK_RULE_T;
 *  @endcode
 *  @li@c  SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG          SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG - signal loss channel.
 *  @li@c  SVCTX_BLOCK_RULE_INP_CH_SIGNAL_LOSS_PROG          SVCTX_BLOCK_RULE_INP_CH_SIGNAL_LOSS_PROG - channel signal loss.
 *  @li@c  SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS          SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS - program signal loss.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SVCTX_BLOCK_RULE_INP_SIGNAL_LOSS_CH_PROG = 0,
    SVCTX_BLOCK_RULE_INP_CH_SIGNAL_LOSS_PROG,
    SVCTX_BLOCK_RULE_INP_CH_PROG_SIGNAL_LOSS
} SVCTX_BLOCK_RULE_T;

/*------------------------------------------------------------------*/
/*! @enum   AUTO_COLOR_COND_T
 *  @brief  Auto color condition.
 *  @code
 *  typedef enum
 *  {
 *      AUTO_COLOR_COND_UNKNOWN = 0,
 *      AUTO_COLOR_COND_SUCCESS,
 *      AUTO_COLOR_COND_FAILED
 *  } AUTO_COLOR_COND_T;
 *  @endcode
 *  @li@c  AUTO_COLOR_COND_UNKNOWN                           AUTO_COLOR_COND_UNKNOWN - unknown.
 *  @li@c  AUTO_COLOR_COND_SUCCESS                           AUTO_COLOR_COND_SUCCESS - success.
 *  @li@c  AUTO_COLOR_COND_FAILED                            AUTO_COLOR_COND_FAILED - failed.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    AUTO_COLOR_COND_UNKNOWN = 0,
    AUTO_COLOR_COND_SUCCESS,
    AUTO_COLOR_COND_FAILED
} AUTO_COLOR_COND_T;

/*------------------------------------------------------------------*/
/*! @enum   AUTO_CLK_POS_PHS_COND_T
 *  @brief  Auto clock postion PHS condition.
 *  @code
 *  typedef enum
 *  {
 *      AUTO_CLK_POS_PHS_COND_UNKNOWN = 0,
 *      AUTO_CLK_POS_PHS_COND_SUCCESS,
 *      AUTO_CLK_POS_PHS_COND_FAILED
 *  } AUTO_CLK_POS_PHS_COND_T;
 *  @endcode
 *  @li@c  AUTO_CLK_POS_PHS_COND_UNKNOWN                     AUTO_CLK_POS_PHS_COND_UNKNOWN - unknown.
 *  @li@c  AUTO_CLK_POS_PHS_COND_SUCCESS                     AUTO_CLK_POS_PHS_COND_SUCCESS - success.
 *  @li@c  AUTO_CLK_POS_PHS_COND_FAILED                      AUTO_CLK_POS_PHS_COND_FAILED - failed.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    AUTO_CLK_POS_PHS_COND_UNKNOWN = 0,
    AUTO_CLK_POS_PHS_COND_SUCCESS,
    AUTO_CLK_POS_PHS_COND_FAILED
} AUTO_CLK_POS_PHS_COND_T;

/*------------------------------------------------------------------*/
/*! @enum   DETECTION_MODE_T
 *  @brief  Detection mode type.
 *  @code
 *  typedef enum
 *  {
 *      DETECTION_MODE_BY_SIGNAL_STATUS = 0,
 *      DETECTION_MODE_BY_CABLE_STATUS
 *  } DETECTION_MODE_T;
 *  @endcode
 *  @li@c  DETECTION_MODE_BY_SIGNAL_STATUS                   DETECTION_MODE_BY_SIGNAL_STATUS - signal status.
 *  @li@c  DETECTION_MODE_BY_CABLE_STATUS                    DETECTION_MODE_BY_CABLE_STATUS - cable status.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    DETECTION_MODE_BY_SIGNAL_STATUS = 0,
    DETECTION_MODE_BY_CABLE_STATUS
} DETECTION_MODE_T;

typedef UINT32 SVC_BLOCK_CHECK_GUDIE_T;    /**< Svctx block mask type */

#define SVC_BLOCK_CHECK_USER_BLOCK_INP  ((SVC_BLOCK_CHECK_GUDIE_T)0x01)    /**< Input is block or not. */
#define SVC_BLOCK_CHECK_USER_BLOCK_CH   ((SVC_BLOCK_CHECK_GUDIE_T)0x02)    /**< Channel is block or not. */
#define SVC_BLOCK_CHECK_USER_BLOCK      (SVC_BLOCK_CHECK_USER_BLOCK_CH | SVC_BLOCK_CHECK_USER_BLOCK_INP)    /**< User is block or not.  */
#define SVC_BLOCK_CHECK_RATING          ((SVC_BLOCK_CHECK_GUDIE_T)0x04)    /**< Rating is block or not. */
#define SVC_BLOCK_CHECK_ALL             (SVC_BLOCK_CHECK_USER_BLOCK|SVC_BLOCK_CHECK_RATING)    /**< all block mask. */


/*------------------------------------------------------------------*/
/*! @brief  prototype for media process notification
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_nfy_cond - notify current svctx condition.
 *  @param  [in]  e_code - ref SVCTX_NTFY_CODE_T.
 *  @param  [in]  e_stream_type - ref STRM_TYPE_T.
 *  @param  [in]  pv_nfy_tag - tag for notify.
 *  @return VOID
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_svctx_media_prc_nfy_fct)(
                    HANDLE_T                    h_svctx,          /* in  */
                    SVCTX_COND_T                e_nfy_cond,       /* in  */
                    SVCTX_NTFY_CODE_T           e_code,           /* in  */
                    STREAM_TYPE_T               e_stream_type,    /* in  */
                    VOID*                       pv_nfy_tag,       /* in  */
                    VOID*                       pv_data           /* in  */
                    );


/*------------------------------------------------------------------*/
/*! @brief  prototype for service context notification
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_nfy_cond - notify current svctx condition.
 *  @param  [in]  e_code - ref SVCTX_NTFY_CODE_T.
 *  @param  [in]  e_stream_type - ref STRM_TYPE_T.
 *  @param  [in]  pv_nfy_tag - tag for notify.
 *  @return VOID
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_svctx_select_nfy_fct)(
                    HANDLE_T                    h_svctx,          /* in  */
                    SVCTX_COND_T                e_nfy_cond,       /* in  */
                    SVCTX_NTFY_CODE_T           e_code,           /* in  */
                    STREAM_TYPE_T               e_stream_type,    /* in  */
                    VOID*                       pv_nfy_tag        /* in  */
                    );
/* prototype for service context stop notification */
/*------------------------------------------------------------------*/
/*! @brief  prototype for service context stop notification
 *  @param  [in]  h_svctx    - Handle to svctx instance
 *  @param  [in]  pv_nfy_tag - Notification tag
 *  @return None
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_svctx_stop_nfy_fct)(
                    HANDLE_T                    h_svctx,          /* in  */
                    VOID*                       pv_nfy_tag        /* in  */
                    );
/*------------------------------------------------------------------*/
/*! @brief  prototype for stream component search filter
 *  @param  [in]  ui2_num_recs - record number.
 *  @param  [in]  ui2_idx - index in all record.
 *  @param  [in]  pt_comp_id - a pointer for STREAM_COMP_ID_T.
 *  @param  [in]  pt_scdb_rec - a pointer for SCDB_REC_T.
 *  @param  [in]  pv_fltr_tag - tag for filter stream.
 *  @return TRUE or FALSE.
 *  @retval TRUE - Filter was completed.
 *  @retval FALSE - Filter was not completed.
 */
/*------------------------------------------------------------------*/
typedef BOOL (*x_svctx_strm_comp_filter_fct)  (
                    UINT16                      ui2_num_recs,     /* in  */
                    UINT16                      ui2_idx,          /* in  */
                    const STREAM_COMP_ID_T*     pt_comp_id,       /* in  */
                    const SCDB_REC_T*           pt_scdb_rec,      /* in  */
                    VOID*                       pv_fltr_tag       /* in  */
                    );

/*------------------------------------------------------------------*/
/*! @brief  prototype for service block check fucntion
 *  @param  [in]  e_check_guide - ref SVC_BLOCK_CHECK_GUDIE_T.
 *  @param  [in]  ps_src_name - source name.
 *  @param  [in]  pt_svl_rec - svl record entry.
 *  @param  [in]  pv_evn_rating - event rating info.
 *  @return Block condition.
 *  @retval SVC_BLOCK_COND_T - ref SVC_BLOCK_COND_T.
 */
/*------------------------------------------------------------------*/
typedef SVC_BLOCK_COND_T (*x_svctx_svc_block_check_fct) (
                    SVC_BLOCK_CHECK_GUDIE_T     e_check_guide,    /* in  */
                    const CHAR*                 ps_src_name,      /* in  */
                    const SVL_REC_T*            pt_svl_rec,       /* in  */
                    const VOID*                 pv_evn_rating     /* in  */
                    );

#ifdef SVCTX_SPLIT_SUPPORT
typedef SVC_BLOCK_COND_T (*svctx_svc_block_check_fct) (
                    SVC_BLOCK_CHECK_GUDIE_T  e_check_guide,       /* in  */
                    const CHAR*              ps_src_name,         /* in  */
                    const SVL_REC_T*         pt_svl_rec,          /* in  */
                    const VOID*              pv_evn_rating,       /* in  */
                    VOID*                    pv_tag               /* in  */
                    );
#endif

/*------------------------------------------------------------------*/
/*! @brief  prototype for service context listener notification fucntion.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_nfy_cond - notify current svctx condition.
 *  @param  [in]  e_code - ref SVCTX_NTFY_CODE_T.
 *  @param  [in]  e_stream_type - ref STRM_TYPE_T.
 *  @param  [in]  pv_nfy_tag - tag for notify.
 *  @return VOID
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_svctx_listener_nfy_fct)(
                    HANDLE_T                    h_svctx,          /* in  */
                    SVCTX_COND_T                e_nfy_cond,       /* in  */
                    SVCTX_NTFY_CODE_T           e_code,           /* in  */
                    STREAM_TYPE_T               e_stream_type,    /* in  */
                    VOID*                       pv_nfy_tag        /* in  */
                    );

#ifdef GINGA_SUPPORT
/*------------------------------------------------------------------*/
/*! @brief  prototype for service context pre-select stream notification fucntion.
 *  @param  [in]  h_svctx - Contains a handle to a service context.
 *  @param  [in]  e_stream_type - notify the stream type to select.
 *  @return VOID 
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_svctx_pre_select_stream_nfy_fct)(
                    HANDLE_T                    h_svctx,         /* in  */
                    STREAM_TYPE_T               e_stream_type    /* in  */ 
                    );

#endif 

typedef UINT8 SRC_TYPE_T;    /**< select service source type */

typedef enum _SRC_SUB_TYPE_T    /**< select service source sub type */
{
    SRC_SUB_TYPE_NETWORK_UNKNOWN,
    SRC_SUB_TYPE_NETWORK_NETFLIX, 
    SRC_SUB_TYPE_NETWORK_MP3_ONLY,
    SRC_SUB_TYPE_NETWORK_SKYPE,
    SRC_SUB_TYPE_VESTEL_TS,
    SRC_SUB_TYPE_PUSH_MHP,
    SRC_SUB_TYPE_SUB_FREE_RUN,       /*Main Other source, sub A/V  free run*/
    SRC_SUB_TYPE_HIGH_SPEED_STORAGE  /*LOCAL HIGH SPEED STORAGE*/       
} SRC_SUB_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct SIGNAL_LOSS_DELAY_T
 *  @brief  configure handler specific signal loss chk delay setting
 *  @code
 *  typedef struct _SIGNAL_LOSS_DELAY_T
 *  {
 *      SRC_TYPE_T                      e_src_type;
 *      UINT32                          ui4_time;  
 *  } SIGNAL_LOSS_DELAY_T;
 *  @endcode
 *  @li@c  e_src_type                                        e_src_type - source type to search specific handler .
 *  @li@c  ui4_time                                          ui4_time - how long the signal loss check will be delayed.
 */
/*------------------------------------------------------------------*/
typedef struct _SIGNAL_LOSS_DELAY_T
{
    SRC_TYPE_T                      e_src_type;
    UINT32                          ui4_time;
} SIGNAL_LOSS_DELAY_T;

/*------------------------------------------------------------------*/
/*! @struct SCART_OUT_DESC_T
 *  @brief  Svctx out description.
 *  @code
 *  typedef struct _SCART_OUT_DESC_T
 *  {
 *      BOOL                            b_enable;
 *      CHAR                            s_name[SVCTX_SRC_NAME_MAX_SIZE+1];
 *  } SCART_OUT_DESC_T;
 *  @endcode
 *  @li@c  b_enable                                          b_enable - enable or disable.
 *  @li@c  s_name[SVCTX_SRC_NAME_MAX_SIZE+1]                 s_name - out name.
 */
/*------------------------------------------------------------------*/
typedef struct _SCART_OUT_DESC_T
{
    BOOL                            b_enable;
    CHAR                            s_name[SVCTX_SRC_NAME_MAX_SIZE+1];
} SCART_OUT_DESC_T;

/*------------------------------------------------------------------*/
/*! @struct SCART_OUT_INFO_T
 *  @brief  Scart info for out.
 *  @code
 *  typedef struct _SCART_OUT_INFO_T
 *  {
 *      BOOL                            b_valid;
 *      UINT8                           ui1_num_descs;
 *      SCART_OUT_DESC_T*               pt_descs;
 *  } SCART_OUT_INFO_T;
 *  @endcode
 *  @li@c  b_valid                                           b_valid - is valid or not.
 *  @li@c  ui1_num_descs                                     ui1_num_descs - description number.
 *  @li@c  pt_descs                                          pt_descs - a pointer structure of SCART_OUT_INFO_T.
 */
/*------------------------------------------------------------------*/
typedef struct _SCART_OUT_INFO_T
{
    BOOL                            b_valid;
    UINT8                           ui1_num_descs;
    SCART_OUT_DESC_T*               pt_descs;
} SCART_OUT_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct CH_BLOCK_BIAS_FREQ_T
 *  @brief  Channel block bias frequency.
 *  @code
 *  typedef struct _CH_BLOCK_BIAS_FREQ_T
 *  {
 *      BOOL                            b_enable;
 *      UINT32                          ui4_freq;
 *  } CH_BLOCK_BIAS_FREQ_T;
 *  @endcode
 *  @li@c  b_enable                                          b_enable - enable frequency.
 *  @li@c  ui4_freq                                          ui4_freq - frequency value.
 */
/*------------------------------------------------------------------*/
typedef struct _CH_BLOCK_BIAS_FREQ_T
{
    BOOL                            b_enable;
    UINT32                          ui4_freq;
} CH_BLOCK_BIAS_FREQ_T;

/*------------------------------------------------------------------*/
/*! @struct RIKS_TV_T
 *  @brief  Riks TV.
 *  @code
 *  typedef struct _RIKS_TV_T
 *  {
 *      BOOL                            b_tune_alter_freq_if_loss_signal;
 *      BOOL                            b_change_freq_if_fail;
 *  } RIKS_TV_T;
 *  @endcode
 *  @li@c  b_tune_alter_freq_if_loss_signal                 b_tune_alter_freq_if_loss_signal - loss signal.
 *  @li@c  b_change_freq_if_fail                            b_change_freq_if_fail - change frequency is signal loss.
 */
/*------------------------------------------------------------------*/
typedef struct _RIKS_TV_T
{
    BOOL                            b_tune_alter_freq_if_loss_signal;
    BOOL                            b_change_freq_if_fail;
} RIKS_TV_T;

/* service list source descriptor */
/*------------------------------------------------------------------*/
/*! @struct SRC_DESC_T
 *  @brief  source description.
 *  @code
 *  typedef struct _SRC_DESC_T
 *  {
 *      CHAR*                           ps_conn_src_name;
 *      SRC_TYPE_T                      e_type;
 *      VOID*                           pv_details; 
 *      SCART_OUT_INFO_T                t_scart_out;
 *      union {
 *          SRC_SVL_INFO_T              t_svl;      
 *          SRC_FREQ_INFO_T             t_freq;     
 *          SRC_MM_INFO_T               t_mm;       
 *      } u_info;
 *      CHAR*                           ps_conn_src_type; 
 *      UINT16                          ui2_svc_lst_id;   
 *      UINT16                          ui2_svl_rec_id;   
 *      RIKS_TV_T                       t_riks_tv;
 *  } SRC_DESC_T;
 *  @endcode
 *  @li@c  ps_conn_src_name                                  ps_conn_src_name - connection source name.
 *  @li@c  e_type                                            e_type - source type.
 *  @li@c  pv_details                                        pv_details - details is deined in each handler's u_ header file.
 *  @li@c  t_scart_out                                       t_scart_out - for enable SCART out.
 *  @li@c  t_svl                                             t_svl - deprecated.
 *  @li@c  t_freq                                            t_freq - deprecated.
 *  @li@c  t_mm                                              t_mm - multimedia.
 *  @li@c  ps_conn_src_type                                  ps_conn_src_type - deprecated
 *  @li@c  ui2_svc_lst_id                                    ui2_svc_lst_id - deprecated
 *  @li@c  ui2_svl_rec_id                                    ui2_svl_rec_id - deprecated
 *  @li@c  t_riks_tv                                         t_riks_tv - For Riks TV
 */
/*------------------------------------------------------------------*/
typedef struct _SRC_DESC_T
{
    /* --- common --- */
    CHAR*                           ps_conn_src_name;
    SRC_TYPE_T                      e_type;
    VOID*                           pv_details;
    SCART_OUT_INFO_T                t_scart_out;

    /* --- private --- */
    union {
        SRC_SVL_INFO_T              t_svl;
        SRC_FREQ_INFO_T             t_freq;
        SRC_MM_INFO_T               t_mm;
    } u_info;

    CHAR*                           ps_conn_src_type;
    UINT16                          ui2_svc_lst_id;
    UINT16                          ui2_svl_rec_id;
    RIKS_TV_T                       t_riks_tv;
    BOOL                            b_freeze_change;
#if 1/*MKV DivXPlus*/
    UINT32                          ui4_title_idx;
    UINT32                          ui4_playlist_idx;
    UINT32                          ui4_chap_idx;
#endif
} SRC_DESC_T;

/*------------------------------------------------------------------*/
/*! @struct STRM_CC_DESC_T
 *  @brief  Close caption description.
 *  @code
 *  typedef struct _STRM_CC_DESC_T
 *  {
 *      HANDLE_T                        h_gl_plane;
 *      SM_CC_FILTER_T*                 pt_filter;
 *      x_svctx_strm_comp_filter_fct    pt_fltr_fct;
 *      VOID*                           pv_fltr_tag;
 *  } STRM_CC_DESC_T;
 *  @endcode
 *  @li@c  h_gl_plane                                        h_gl_plane - gl plane handle
 *  @li@c  pt_fltr_fct                                       pt_fltr_fct - a pointer for filter close caption.
 *  @li@c  pv_fltr_tag                                       pv_fltr_tag - tag for filter close caption.
 */
/*------------------------------------------------------------------*/
typedef struct _STRM_CC_DESC_T
{
    HANDLE_T                        h_gl_plane;
    SM_CC_FILTER_T*                 pt_filter;
    x_svctx_strm_comp_filter_fct    pt_fltr_fct;
    VOID*                           pv_fltr_tag;
} STRM_CC_DESC_T;

/*------------------------------------------------------------------*/
/*! @struct STRM_SUBTITLE_DESC_T
 *  @brief  Subtitle description.
 *  @code
 *  typedef struct _STRM_SUBTITLE_DESC_T
 *  {
 *      HANDLE_T                        h_gl_plane;
 *      x_svctx_strm_comp_filter_fct    pt_fltr_fct;
 *      VOID*                           pv_fltr_tag;
 *  } STRM_SUBTITLE_DESC_T;
 *  @endcode
 *  @li@c  h_gl_plane                                        h_gl_plane - gl plane handle
 *  @li@c  pt_fltr_fct                                       pt_fltr_fct - a pointer for filter subtitle.
 *  @li@c  pv_fltr_tag                                       pv_fltr_tag - tag for filter subtitle.
 */
/*------------------------------------------------------------------*/
typedef struct _STRM_SUBTITLE_DESC_T
{
    HANDLE_T                        h_gl_plane;
    x_svctx_strm_comp_filter_fct    pt_fltr_fct;
    VOID*                           pv_fltr_tag;
} STRM_SUBTITLE_DESC_T;

/*------------------------------------------------------------------*/
/*! @struct STRM_TELTEX_DESC_T
 *  @brief  Telectext description.
 *  @code
 *  typedef struct _STRM_TELTEX_DESC_T
 *  {
 *      HANDLE_T                        h_gl_plane;
 *      x_svctx_strm_comp_filter_fct    pt_fltr_fct;
 *      VOID*                           pv_fltr_tag;
 *  } STRM_TELTEX_DESC_T;
 *  @endcode
 *  @li@c  h_gl_plane                                        h_gl_plane - gl plane handle
 *  @li@c  pt_fltr_fct                                       pt_fltr_fct - a pointer for filter teletext.
 *  @li@c  pv_fltr_tag                                       pv_fltr_tag - tag for filter teletext.
 */
/*------------------------------------------------------------------*/
typedef struct _STRM_TELTEX_DESC_T
{
    HANDLE_T                        h_gl_plane;
    x_svctx_strm_comp_filter_fct    pt_fltr_fct;
    VOID*                           pv_fltr_tag;
} STRM_TELTEX_DESC_T;

/*------------------------------------------------------------------*/
/*! @struct SNK_DESC_T
 *  @brief  Snk description.
 *  @code
 *  typedef struct _SNK_DESC_T
 *  {
 *      CHAR*                           ps_snk_grp_name;
 *      CHAR*                           ps_snk_grp_name_aux;
 *      VSH_REGION_INFO_T*              pt_video_region;
 *      VSH_REGION_INFO_T*              pt_disp_region;
 *      VSH_SET_PLANE_ORDER_INFO_T*     pt_video_plane_order;
 *      x_svctx_strm_comp_filter_fct    pt_video_strm_fltr_fct;
 *      VOID*                           pv_video_strm_fltr_tag;
 *      SCC_AUD_VOLUME_INFO_T*          pt_volume_info;         
 *      x_svctx_strm_comp_filter_fct    pt_audio_strm_fltr_fct;
 *      VOID*                           pv_audio_strm_fltr_tag;
 *      HANDLE_T                        h_cc_gl_plane;          
 *      SM_CC_FILTER_T*                 pt_cc_filter;           
 *      x_svctx_strm_comp_filter_fct    pt_cc_strm_fltr_fct;    
 *      VOID*                           pv_cc_strm_fltr_tag;    
 *      STREAM_TYPE_T                   e_text_strm_type;
 *      union{
 *          STRM_CC_DESC_T              t_cc;
 *          STRM_SUBTITLE_DESC_T        t_subtitle;
 *          STRM_TELTEX_DESC_T          t_telex;
 *      } u_text_strm;
 *  } SNK_DESC_T;
 *  @endcode
 *  @li@c  ps_snk_grp_name                                   ps_snk_grp_name - snk group name.
 *  @li@c  ps_snk_grp_name_aux                               ps_snk_grp_name_aux - snk group name aux.
 *  @li@c  pt_video_region                                   pt_video_region - a pointerf for VSH_REGION_INFO_T.
 *  @li@c  pt_disp_region                                    pt_disp_region - a pointerf for VSH_REGION_INFO_T.
 *  @li@c  pt_video_plane_order                              pt_video_plane_order - a pointer for VSH_SET_PLANE_ORDER_INFO_T.
 *  @li@c  pt_video_strm_fltr_fct                            pt_video_strm_fltr_fct - a pointer for x_svctx_strm_comp_filter_fct.
 *  @li@c  pv_video_strm_fltr_tag                            pv_video_strm_fltr_tag - tag for filter video stream.
 *  @li@c  pt_volume_info                                    pt_volume_info - deprecated.
 *  @li@c  pt_audio_strm_fltr_fct                            pt_audio_strm_fltr_fct - a pointer for x_svctx_strm_comp_filter_fct.
 *  @li@c  pv_audio_strm_fltr_tag                            pv_audio_strm_fltr_tag - tag for filter audio stream.
 *  @li@c  h_cc_gl_plane                                     h_cc_gl_plane - deprecated.
 *  @li@c  pt_cc_filter                                      pt_cc_filter - deprecated.
 *  @li@c  pt_cc_strm_fltr_fct                               pt_cc_strm_fltr_fct - deprecated.
 *  @li@c  pv_cc_strm_fltr_tag                               pv_cc_strm_fltr_tag - deprecated.
 *  @li@c  e_text_strm_type                                  e_text_strm_type - ref STREAM_TYPE_T.
 *  @li@c  t_cc                                              t_cc - ref STRM_CC_DESC_T.
 *  @li@c  t_subtitle                                        t_subtitle - ref STRM_SUBTITLE_DESC_T.
 *  @li@c  t_telex                                           t_telex - ref STRM_TELTEX_DESC_T.
 */
/*------------------------------------------------------------------*/
typedef struct _SNK_DESC_T
{
    /* --- common --- */
    CHAR*                           ps_snk_grp_name;
    CHAR*                           ps_snk_grp_name_aux;

    /* video */
    VSH_REGION_INFO_T*              pt_video_region;
    VSH_REGION_INFO_T*              pt_disp_region;
    VSH_SET_PLANE_ORDER_INFO_T*     pt_video_plane_order;
    x_svctx_strm_comp_filter_fct    pt_video_strm_fltr_fct;
    VOID*                           pv_video_strm_fltr_tag;

    /* audio */
    SCC_AUD_VOLUME_INFO_T*          pt_volume_info;
    x_svctx_strm_comp_filter_fct    pt_audio_strm_fltr_fct;
    VOID*                           pv_audio_strm_fltr_tag;

    /* cc */
    HANDLE_T                        h_cc_gl_plane;
    SM_CC_FILTER_T*                 pt_cc_filter;
    x_svctx_strm_comp_filter_fct    pt_cc_strm_fltr_fct;
    VOID*                           pv_cc_strm_fltr_tag;

    STREAM_TYPE_T                   e_text_strm_type;
    union{
        STRM_CC_DESC_T              t_cc;
        STRM_SUBTITLE_DESC_T        t_subtitle;
        STRM_TELTEX_DESC_T          t_telex;
    } u_text_strm;

} SNK_DESC_T;

#ifdef MW_TV_AV_BYPASS_SUPPORT
#define SVCTX_ASSC_ATTR_ASSOCIATE               ((UINT32) 0x01)    /**< associate */
#define SVCTX_ASSC_ATTR_IGNORE_SCART            ((UINT32) 0x02)    /**< ignore scart */
#define SVCTX_ASSC_ATTR_AUDIO_SLAVE             ((UINT32) 0x04)    /**< slave audio */
#define SVCTX_ASSC_ATTR_KEEP_ALIVE              ((UINT32) 0x08)    /**< keep alive */
#define SVCTX_ASSC_ATTR_USE_SAME_STRM_FILTER    ((UINT32) 0x10)    /**< use same stream filter */
#define SVCTX_ASSC_ATTR_USE_SAME_BLOCK_CHECKER  ((UINT32) 0x20)    /**< use same block checker */
#define SVCTX_ASSC_ATTR_SUPPORT_CANAL_PLUS      ((UINT32) 0x40)    /**< Used for specifying a monitor bypass with CANAL+ support */
/* reserve the MSB 8 bits [31:24] for internal usage */


/*------------------------------------------------------------------*/
/*! @struct SVCTX_ASSC_CFG_T
 *  @brief  Svctx association configuration.
 *  @code
 *  typedef struct _SVCTX_ASSC_CFG_T
 *  {
 *      HANDLE_T                h_svctx_lead;
 *      CHAR                    s_snk_grp_name[SVCTX_SNK_NAME_MAX_SIZE+1];
 *      x_svctx_select_nfy_fct  pf_select_svc_nfy;
 *      VOID*                   pv_select_svc_nfy_tag;
 *      VOID*                   pv_video_strm_fltr_tag;
 *      VOID*                   pv_audio_strm_fltr_tag;
 *      VOID*                   pv_cc_strm_fltr_tag;
 *      VOID*                   pv_sbtl_strm_fltr_tag;
 *      VOID*                   pv_ttx_strm_fltr_tag;
 *      UINT32                  ui4_stream_mode;
 *      UINT32                  ui4_assc_src_mask;
 *      UINT32                  ui4_attribute;
 *      INT8                    i1_prio_diff;
 *  } SVCTX_ASSC_CFG_T;
 *  @endcode
 *  @li@c  h_svctx_lead                                      h_svctx_lead - leader svctx handler.
 *  @li@c  s_snk_grp_name[SVCTX_SNK_NAME_MAX_SIZE+1]         s_snk_grp_name - snk grp name.
 *  @li@c  pf_select_svc_nfy                                 pf_select_svc_nfy - function pointer for select channel.
 *  @li@c  pv_select_svc_nfy_tag                             pv_select_svc_nfy_tag - tag for select channel.
 *  @li@c  pv_video_strm_fltr_tag                            pv_video_strm_fltr_tag - tag for filter video stream.
 *  @li@c  pv_audio_strm_fltr_tag                            pv_audio_strm_fltr_tag - tag for filter audio stream.
 *  @li@c  pv_cc_strm_fltr_tag                               pv_cc_strm_fltr_tag - tag for filter cc stream.
 *  @li@c  pv_sbtl_strm_fltr_tag                             pv_sbtl_strm_fltr_tag - tag for filter subtitle stream.
 *  @li@c  pv_ttx_strm_fltr_tag                              pv_ttx_strm_fltr_tag - tag for filter teletext stream.
 *  @li@c  ui4_stream_mode                                   ui4_stream_mode - stream mode. eg. 0x1 = audio type.
 *  @li@c  ui4_assc_src_mask                                 ui4_assc_src_mask - config association source mask.
 *  @li@c  ui4_attribute                                     ui4_attribute - ref SVCTX_ASSC_ATTR_ASSOCIATE and so on.
 *  @li@c  i1_prio_diff                                      i1_prio_diff - prio diff. -1=tv bypass. 0=main. 1=monitor bypass.
 */
/*------------------------------------------------------------------*/
typedef struct _SVCTX_ASSC_CFG_T
{
    HANDLE_T                h_svctx_lead;
    CHAR                    s_snk_grp_name[SVCTX_SNK_NAME_MAX_SIZE+1];
    x_svctx_select_nfy_fct  pf_select_svc_nfy;
    VOID*                   pv_select_svc_nfy_tag;

    VOID*                   pv_video_strm_fltr_tag;
    VOID*                   pv_audio_strm_fltr_tag;
    VOID*                   pv_cc_strm_fltr_tag;
    VOID*                   pv_sbtl_strm_fltr_tag;
    VOID*                   pv_ttx_strm_fltr_tag;

    UINT32                  ui4_stream_mode;
    UINT32                  ui4_assc_src_mask;
    UINT32                  ui4_attribute;
    INT8                    i1_prio_diff;
} SVCTX_ASSC_CFG_T;
#endif

#define VIDEO_HEIGHT_480                      (480)    /**< 480 */
#define VIDEO_HEIGHT_576                      (576)    /**< 576 */
#define VIDEO_HEIGHT_720                      (720)    /**< 720 */
#define VIDEO_HEIGHT_1080                     (1080)    /**< 1080 */

#define VIDEO_RES_MASK_NONE                   ((UINT32)0)    /**< none */
#define VIDEO_RES_MASK_480I                   MAKE_BIT_MASK_32(0)    /**< 0x1 */
#define VIDEO_RES_MASK_480P                   MAKE_BIT_MASK_32(1)    /**< 0x2 */
#define VIDEO_RES_MASK_576I                   MAKE_BIT_MASK_32(2)    /**< 0x4 */
#define VIDEO_RES_MASK_576P                   MAKE_BIT_MASK_32(3)    /**< 0x8 */
#define VIDEO_RES_MASK_720I                   MAKE_BIT_MASK_32(4)    /**< 0x10 */
#define VIDEO_RES_MASK_720P                   MAKE_BIT_MASK_32(5)    /**< 0x20 */
#define VIDEO_RES_MASK_1080I                  MAKE_BIT_MASK_32(6)    /**< 0x40 */
#define VIDEO_RES_MASK_1080P                  MAKE_BIT_MASK_32(7)    /**< 0x80 */
#define VIDEO_RES_MASK_ALL                    ((UINT32)~0)    /**< 0xFFFFFFFF */
#define VIDEO_RES_MASK_480I_576I              (VIDEO_RES_MASK_480I|VIDEO_RES_MASK_576I)    /**< 0x5 */

#define DEV_EVN_CAPABILITY_NONE               ((UINT32)0)    /**< 0x0 */
#define DEV_EVN_CAPABILITY_ALL                ((UINT32)~0)    /**< 0xFFFFFFFF */

#define STRM_COMP_FILTER_BITS_FOR_LOOP        (3)    /**< 0x3 */
#define STRM_COMP_FILTER_MAX_LOOP             MAKE_BIT_MASK_16(STRM_COMP_FILTER_BITS_FOR_LOOP)    /**< 0x4 */
#define STRM_COMP_FILTER_TOTAL_BITS_OF_IDX    (16)    /**< 0x10 */
#define STRM_COMP_FILTER_LOOP_START_BIT_IDX   (STRM_COMP_FILTER_TOTAL_BITS_OF_IDX-STRM_COMP_FILTER_BITS_FOR_LOOP)    /**< 13 */
#define STRM_COMP_FILTER_LOOP_MASK            ((UINT16)(0xFFFF & (0xFFFF << STRM_COMP_FILTER_LOOP_START_BIT_IDX)))    /**<  */
#define STRM_COMP_FILTER_GET_LOOP_IDX(x)      ((((UINT16)(x)) & STRM_COMP_FILTER_LOOP_MASK) >> STRM_COMP_FILTER_LOOP_START_BIT_IDX)    /**<  */
#define STRM_COMP_FILTER_GET_COMP_IDX(x)      ( ((UINT16)(x)) & (~STRM_COMP_FILTER_LOOP_MASK))    /**<  */
#define STRM_COMP_FILTER_IS_LAST_CHANCE(x,n)  (STRM_COMP_FILTER_GET_COMP_IDX(x) == (n))    /**<  */

/*------------------------------------------------------------------*/
/*! @struct EVENT_CAPABILITY_T
 *  @brief  Event capability.
 *  @code
 *  typedef struct _EVENT_CAPABILITY_T
 *  {
 *      BOOL            b_video_res_dependent;
 *      UINT32          ui4_video_res_mask;
 *      UINT32          ui4_capable_mask;
 *  } EVENT_CAPABILITY_T;
 *  @endcode
 *  @li@c  b_video_res_dependent                             b_video_res_dependent - video resource is dependent or not.
 *  @li@c  ui4_video_res_mask                                ui4_video_res_mask - video resource mask.
 *  @li@c  ui4_capable_mask                                  ui4_capable_mask - capable mask.
 */
/*------------------------------------------------------------------*/
typedef struct _EVENT_CAPABILITY_T
{
    BOOL            b_video_res_dependent;
    UINT32          ui4_video_res_mask;
    UINT32          ui4_capable_mask;
} EVENT_CAPABILITY_T;

/*------------------------------------------------------------------*/
/*! @struct TEXT_CAPABILITY_T
 *  @brief  Text capablitity.
 *  @code
 *  typedef struct _TEXT_CAPABILITY_T
 *  {
 *      BOOL            b_video_res_dependent;
 *      UINT32          ui4_video_res_mask;
 *  } TEXT_CAPABILITY_T;
 *  @endcode
 *  @li@c  b_video_res_dependent                             b_video_res_dependent - video resource dependent.
 *  @li@c  ui4_video_res_mask                                ui4_video_res_mask - video resource mask.
 */
/*------------------------------------------------------------------*/
typedef struct _TEXT_CAPABILITY_T
{
    BOOL            b_video_res_dependent;
    UINT32          ui4_video_res_mask;
} TEXT_CAPABILITY_T;

#define ST_MASK_ALL                         ((UINT32)~0)    /**< 0xFFFFFFFF */

/*------------------------------------------------------------------*/
/*! @struct DEVICE_TYPE_PROFILE_T
 *  @brief  Device profile type.
 *  @code
 *  typedef struct _DEVICE_TYPE_PROFILE_T
 *  {
 *      DEVICE_TYPE_T       e_dev_type;
 *      UINT32              ui4_av_class_mask;
 *      UINT32              ui4_strm_capable_mask;
 *      INT8                i1_audio_quality_order;
 *      INT8                i1_video_quality_order;
 *      EVENT_CAPABILITY_T  t_event_capability;
 *      TEXT_CAPABILITY_T   t_cc_capability;
 *      TEXT_CAPABILITY_T   t_subtitle_capability;
 *      TEXT_CAPABILITY_T   t_teletext_capability;
 *  } DEVICE_TYPE_PROFILE_T;
 *  @endcode
 *  @li@c  e_dev_type                                        e_dev_type - device type.
 *  @li@c  ui4_av_class_mask                                 ui4_av_class_mask - support av class.
 *  @li@c  ui4_strm_capable_mask                             ui4_strm_capable_mask - support stream capable.
 *  @li@c  i1_audio_quality_order                            i1_audio_quality_order - support audio quality order.
 *  @li@c  i1_video_quality_order                            i1_video_quality_order - support video quality order.
 *  @li@c  t_event_capability                                t_event_capability - support event.
 *  @li@c  t_cc_capability                                   t_cc_capability - support cc.
 *  @li@c  t_subtitle_capability                             t_subtitle_capability - support subtitle.
 *  @li@c  t_teletext_capability                             t_teletext_capability - support teletext.
 */
/*------------------------------------------------------------------*/
typedef struct _DEVICE_TYPE_PROFILE_T
{
    DEVICE_TYPE_T       e_dev_type;
    UINT32              ui4_av_class_mask;
    UINT32              ui4_strm_capable_mask;
    INT8                i1_audio_quality_order;
    INT8                i1_video_quality_order;
    EVENT_CAPABILITY_T  t_event_capability;
    TEXT_CAPABILITY_T   t_cc_capability;
    TEXT_CAPABILITY_T   t_subtitle_capability;
    TEXT_CAPABILITY_T   t_teletext_capability;
} DEVICE_TYPE_PROFILE_T;

/*------------------------------------------------------------------*/
/*! @enum   SVCTX_CAPABILITY_T
 *  @brief  svctx capability type.
 *  @code
 *  typedef enum
 *  {
 *      SVCTX_CAPABILITY_YES = 0,
 *      SVCTX_CAPABILITY_NO,
 *      SVCTX_CAPABILITY_UNKNOWN
 *  } SVCTX_CAPABILITY_T;
 *  @endcode
 *  @li@c  SVCTX_CAPABILITY_YES                              SVCTX_CAPABILITY_YES - have capability
 *  @li@c  SVCTX_CAPABILITY_NO                               SVCTX_CAPABILITY_NO - have no capability
 *  @li@c  SVCTX_CAPABILITY_UNKNOWN                          SVCTX_CAPABILITY_UNKNOWN- unknown have capability or not.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    SVCTX_CAPABILITY_YES = 0,
    SVCTX_CAPABILITY_NO,
    SVCTX_CAPABILITY_UNKNOWN
} SVCTX_CAPABILITY_T;

/* TSL info */
/*------------------------------------------------------------------*/
/*! @struct TUNER_OUT_INFO_T
 *  @brief  Tuner info for out.
 *  @code
 *  typedef struct TUNER_OUT_INFO_T
 *  {
 *      UINT16                          ui2_svl_id;
 *      SVL_REC_T                       t_svl_rec;
 *      CHAR                            s_tuner_src_name[SVCTX_SRC_NAME_MAX_SIZE+1];
 *      CHAR                            s_scart_out_src_name[SVCTX_SRC_NAME_MAX_SIZE+1];
 *  } TUNER_OUT_INFO_T;
 *  @endcode
 *  @li@c  ui2_svl_id                                        ui2_svl_id - svl record ID.
 *  @li@c  t_svl_rec                                         t_svl_rec - svl record entry.
 *  @li@c  s_tuner_src_name[SVCTX_SRC_NAME_MAX_SIZE+1]       s_tuner_src_name - tuner source name.
 *  @li@c  s_scart_out_src_name[SVCTX_SRC_NAME_MAX_SIZE+1]   s_scart_out_src_name - scart out source name.
 */
/*------------------------------------------------------------------*/
typedef struct TUNER_OUT_INFO_T
{
    UINT16                          ui2_svl_id;
    SVL_REC_T                       t_svl_rec;
    CHAR                            s_tuner_src_name[SVCTX_SRC_NAME_MAX_SIZE+1];
    CHAR                            s_scart_out_src_name[SVCTX_SRC_NAME_MAX_SIZE+1];
} TUNER_OUT_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum   TUNER_OUT_TYPE_T
 *  @brief  Tuner status for out.
 *  @code
 *  typedef enum
 *  {
 *      TUNER_OUT_TYPE_IGONRE = 0, 
 *      TUNER_OUT_TYPE_DISABLE,
 *      TUNER_OUT_TYPE_ENABLE,
 *      TUNER_OUT_TYPE_LAST_VALID_ENTRY 
 *  } TUNER_OUT_TYPE_T;
 *  @endcode
 *  @li@c  TUNER_OUT_TYPE_IGONRE                             TUNER_OUT_TYPE_IGONRE - not care.
 *  @li@c  TUNER_OUT_TYPE_DISABLE                            TUNER_OUT_TYPE_DISABLE - disable tuner out.
 *  @li@c  TUNER_OUT_TYPE_ENABLE                             TUNER_OUT_TYPE_ENABLE - enable tuner out.
 *  @li@c  TUNER_OUT_TYPE_LAST_VALID_ENTRY                   TUNER_OUT_TYPE_LAST_VALID_ENTRY - not care.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    TUNER_OUT_TYPE_IGONRE = 0, /* don't set tuner out */
    TUNER_OUT_TYPE_DISABLE,
    TUNER_OUT_TYPE_ENABLE,
    TUNER_OUT_TYPE_LAST_VALID_ENTRY /* only for counting purpose */
} TUNER_OUT_TYPE_T;

/*------------------------------------------------------------------*/
/*! @struct TUNER_OUT_DESC_T
 *  @brief  Tuner description for out
 *  @code
 *  typedef struct _TUNER_OUT_DESC_T
 *  {
 *      TUNER_OUT_TYPE_T                e_type;
 *      TUNER_OUT_INFO_T                t_info; 
 *  } TUNER_OUT_DESC_T;
 *  @endcode
 *  @li@c  e_type                                            e_type - ref TUNER_OUT_TYPE_T.
 *  @li@c  t_info                                            t_info - ref TUNER_OUT_INFO_T.
 */
/*------------------------------------------------------------------*/
typedef struct _TUNER_OUT_DESC_T
{
    TUNER_OUT_TYPE_T                e_type;
    TUNER_OUT_INFO_T                t_info; /* only valid when e_type = TUNER_OUT_TYPE_ENABLE */
} TUNER_OUT_DESC_T;

/* result value **************************************************************/
#ifdef MW_TV_AV_BYPASS_SUPPORT
#define SVCTXR_QUEUED                   ((INT32)  3)    /**< request was queued */
#endif

#define SVCTXR_MSG_LAST_ENTRY           ((INT32) 50)    /**< invalid msg. */

#define SVCTXR_ALREADY_STOPPED          ((INT32)  6)    /**< svctx already stopped for stopping request */
#define SVCTXR_NEW_TRICK_FLOW           ((INT32)  5)    /** <<new trick return> */
#define SVCTXR_GET_POS_AS_TIME          ((INT32)  4)    /**< <++> */
#define SVCTXR_ASYNC_NFY                ((INT32)  2)    /**< <++> */
#define SVCTXR_PENDING                  ((INT32)  1)    /**< request pending */
#define SVCTXR_OK                       ((INT32)  0)    /**< operate OK */
#define SVCTXR_NO_RESOURCE              ((INT32) -1)    /**< resource not enough */
#define SVCTXR_FAIL                     ((INT32) -2)    /**< operate fail */
#define SVCTXR_NOT_EXIST                ((INT32) -3)    /**< request was not exist */
#define SVCTXR_EXIST                    ((INT32) -4)    /**< request was exist */
#define SVCTXR_INV_ARG                  ((INT32) -5)    /**< invalid arg */
#define SVCTXR_WRONG_STATE              ((INT32) -6)    /**< svctx state is error */
#define SVCTXR_INV_HANDLE               ((INT32) -7)    /**< invalid handle */
#define SVCTXR_NOT_SUPPORT              ((INT32) -8)    /**< not support */
#define SVCTXR_NOT_INIT                 ((INT32) -9)    /**< module is not init */
#define SVCTXR_NO_RECORD                ((INT32) -10)    /**< no record */
#define SVCTXR_TIMEOUT                  ((INT32) -11)    /**< operate timeout */
#define SVCTXR_IN_STOPPING              ((INT32) -13)    /**< svctx was stopping */
#define SVCTXR_INV_SET_INFO             ((INT32) -14)    /**< invalid set info */
#define SVCTXR_INV_GET_INFO             ((INT32) -15)    /**< invalid get info */
#define SVCTXR_NO_RIGHT                 ((INT32) -16)    /**< handle is not owner */
#define SVCTXR_FROZEN                   ((INT32) -17)    /**< svctx was frozon */
#define SVCTXR_EVN_INFO_NOT_FOUND       ((INT32) -18)    /**< event was not found */
#define SVCTXR_EVN_INFO_NOT_SUPPORT     ((INT32) -19)    /**< event was not support */
#define SVCTXR_INV_COMMAND              ((INT32) -20)    /**< invalid command */
#define SVCTXR_INV_ENTRY	            ((INT32) -21)    /**< invalid entry */

#define SVCTXR_ERROR_LAST_ENTRY         ((INT32) -50)    /**< last error */

/*------------------------------------------------------------------*/
/*! @struct SVCTX_VIDEO_THUMBNAIL_INFO_T
 *  @brief  <++>
 *  @code
 *  typedef struct _SVCTX_VIDEO_THUMBNAIL_INFO_T
 *  {
 *      GL_HSURFACE_T                       h_surf;
 *      UINT32                              ui4_thumbnail_x;
 *      UINT32                              ui4_thumbnail_y;
 *      UINT32                              ui4_thumbnail_width;
 *      UINT32                              ui4_thumbnail_height;
 *      UINT64                              ui8_pic_idx; 
 *      x_svctx_select_nfy_fct              pf_nfy;
 *      VOID*                               pv_tag;
 *  } SVCTX_VIDEO_THUMBNAIL_INFO_T;
 *  @endcode
 *  @li@c  h_surf                                            <++>- <++>
 *  @li@c  ui4_thumbnail_x                                   <++>- <++>
 *  @li@c  ui4_thumbnail_y                                   <++>- <++>
 *  @li@c  ui4_thumbnail_width                               <++>- <++>
 *  @li@c  ui4_thumbnail_height                              <++>- <++>
 *  @li@c  ui8_pic_idx                                       <++>- <++>
 *  @li@c  pf_nfy                                            <++>- <++>
 *  @li@c  pv_tag                                            <++>- <++>
 */
/*------------------------------------------------------------------*/
typedef struct _SVCTX_VIDEO_THUMBNAIL_INFO_T
{
    GL_HSURFACE_T                       h_surf;
    UINT32                              ui4_thumbnail_x;
    UINT32                              ui4_thumbnail_y;
    UINT32                              ui4_thumbnail_width;
    UINT32                              ui4_thumbnail_height;

    UINT64                              ui8_pic_idx; /* start from 1; 0 means get current picture */

    x_svctx_select_nfy_fct              pf_nfy;
    VOID*                               pv_tag;
} SVCTX_VIDEO_THUMBNAIL_INFO_T;

#endif /* _U_SVCTX_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                                           /* end of groupMW_SVCTX */
/*----------------------------------------------------------------------------*/

/*! @if DETAIL_DESCRIPTION_TYPE_C
 *  @page pageMW_SVCTX Service Context Detailed Description
 *  @par
 *  @section secMW_SVCTX_1  1. Introduction
 *  @par
 *      Service Context(SVCTX) describes the concept, program flows, data structures, and functions, which are required to implement the Service Context. The primary purpose of the Service Context is to represents an environment in which channels (services) are presented in DTV system. Clients (e.g. applications) could use Service Context objects to perform channel change job, and may also use Service Context objects to play a specific stream component.
 *  @par
 *      This document must be read in conjunction with the document, SW Design Middleware - Connection Manager, SW Design Middleware - Broadcast Connection Handler, SW Design Middleware - Stream Component Database, SW Design Middleware - Stream Manager, SW Design Stream Manager - Video Stream Handler, SW Design Stream Manager - Closed Caption Stream Handle, and SW Design Stream Manager - Stream Component Controller.
 *  @par
 *  @section secMW_SVCTX_2  2. Concept
 *  @par
 *  @subsection ssecMW_SVCTX_2_1  2.1. Service Context
 *  @par
 *      Clients (e.g. applications) of the Service Context use the defined interface to open a service context instance or attach to a existing service context instance, once a service has been obtained by some means (e.g. from populated Service List), application could use this service context instance to present the service (channel). In other words, the Service Context allows the clients to control the presentation of services in a simple, high-level approach.
 *  @par
 *      Each service context instance can present a different service (channel), completely independently of the other service context instances. Operations carried out on one service context instance will not affect another (unless they cause tuning which prevents the contents of a service in another service context from being presented).
 *  @par
 *      Service Context coordinates the Connection Manager, Stream Component Database, and the Stream Manager (i.e. the service-selection related middleware modules) to perform service (channel) and stream related operations. Their relationship is depicted in Figure 2.1 @image html  svctx_relationships.gif "Figure 2.1 Relationships between Applications, Service Context and Service-Selection related Middleware Modules"
 *  @par
 *      Basically, the concept of the service context is from the <b>Facade Design Pattern</b> documented in the well-known book - Design Patterns by The Gang of Four. This pattern's intent is to
 *  @par
 *      <i>Provide a unified interface to a set of interfaces in a subsystem. Facade defines a higher-level interface that makes the subsystem easier to use.</i>
 *  @par
 *      So, Service Context offers the following benefits:
 *  @par
 *      -#  It decouples applications from the service-selection related middleware modules, thereby reducing the number of modules or objects that applications deal with and making the service selection (channel change) easier.
 *      -#  It provides weak coupling between the service-selection related middleware modules and applications. Applications that use the Service Context don't have to access the service-selection related middleware modules directly.
 *      -#  It doesn't prevent applications from using the service-selection related middleware modules if they need to. Thus application programmer can choose between ease of use and generality.
 *  
 *  @par
 *      The applications can present a service without needing to know about the service's audio components, video components, or closed captioning components, Service Context will select default components automatically. However, while the service (channel) is presenting, applications can use the service context instance to play a specific stream component to change to different video, audio, or closed captioning according to end-user's need.
 *  @par
 *  @subsection ssecMW_SVCTX_2_2  2.2. Activator
 *  @par
 *      In order to handle multiple client requests and middleware modules' notifications simultaneously, a service context instance is running on its own thread of control and uses a message queue to synchronize access to this instance. In Design Patterns world, there's a existing design pattern used to handle such problem, called Active Object pattern and documented by Douglas C. Schmidt in his well-known book - Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects, Volume 2. The following is the Active Object Pattern's intent
 *  @par
 *      <i>The Active Object design pattern decouples method execution from method invocation to enhance concurrency and simplify synchronized access to an object that resides in its own thread of control.</i>
 *  @par
 *      Activator is an implementation of this pattern, and the service context module uses it to make a service context instance to be active. Activator combines the thread and message queue into an abstract object, and provides a construction function to let service context to specify its private message's size, thread priority, maximum amount of message, and a callback function to invoke when a private message is retrieved from the message queue. Also, the activator provides a message sending function for service context to send its private message to this activator's message queue.
 *  @par
 *  @section secMW_SVCTX_3  3. Brief Design
 *  @par
 *  @subsection ssecMW_SVCTX_3_1  3.1. Service Context Structure
 *  @par
 *      The Service Context Module consists essentially of the following functional blocks
 *  @par
 *      -   Service Context Pool.
 *      -   State Objects
 *      -   Activator
 *  
 *  @par
 *      And, in order to be shared by many clients and applications, the service context's APIs must be thread-safe.
 *  @par
 *  @subsubsection sssecMW_SVCTX_3_1_1  3.1.1. Service Context Pool
 *      While service context module initializing, it will create a pool to maintain a pre-defined amount of service context instances, and when clients or applications request to open a service context (by name), if this name is not used, a new service context instance will be chosen from the pool and start to serve. The other clients (applications) can also use the same API to access this service context object if it knows the name. Such behaviors will let one service context object to be shared between many clients or applications, and don't need to perform the service selection job again. Since the service context objects are rare resource (they will access many hardware devices, likes tuner, video decoder, video plane, plane mixer, and audio decoder, and the amount of each kind of device is limited), if the pool is empty, the request to open a new service context will be failed. After the last client or application return the handle back, the service context instance will be recycled and put back into the service context pool.
 *  @par
 *      A service context instance can only process one service selection request or stream selection request at a time. If a service context instance is already processing a service selection request and a new service selection request is incoming from another or same client, the old service selection request will be stopped and the issuer of this request will receive an “Interrupted�notification, then the service context instance processes the new service selection request as usual.
 *  @par
 *      In order to identify the service context instance easily when receiving notifications from the Connection Manager, the SCDB or the Stream Manager, the service context passes a so-called system service context handle to these modules as the pv_nfy_tag. For example while invoking
 *  @par
 *      INT32 x_cm_connect (HANDLE_T             h_obj,<br>
 *      const CM_COMMAND_T*  pt_connect_cmd,<br>
 *      VOID*                pv_nfy_tag,<br>
 *      x_cm_nfy_fct         pf_nfy,<br>
 *      HANDLE_T*            ph_conn,<br>
 *      CM_COND_T*           pe_cond);<br>
 *  @par
 *      A service context instance set the pv_nfy_tag to its <b>system service context handle</b>. This system handle is used to identify the service context instance. So while the service context instance is first time in <b>POOLED</b> state, a system service context handle is created to associate with this instance. And because the service context HANDLE created for clients or application will be disappeared if clients or applications free it (via x_svctx_open() function), it is not suitable for this purpose.
 *  @par
 *      If just pass the service context instance's memory address to be the pv_nfy_tag, and in some condition, this service context instance had been destroyed and the notification from the connection manager is incoming, then we don't know this instance is still alive or not and this could cause the well-known segmentation fault error. Using the HANDLE to be the pv_nfy_tag provides an easy way to determine the existence of the associated instance.
 *      @image html  svctx_pool.gif "Figure 3.1.1 Service Context Pool"
 *  @par
 *  @subsubsection sssecMW_SVCTX_3_1_2  3.1.2. State Objects
 *  @paragraph ssssecMW_SVCTX_3_1_2_1  3.1.2.1. State Machine
 *      The finite state machine of a service context instance is depicted in Figure 3.1.2 and each state is described as follows:
 *      -#  <b>READY</b> when the very first client invokes the API x_svctx_open(), an POOLED service context will be chosen from the Service Context Pool and this instance will open a pipe automatically. After it receives the notification about that the pipe is opened (CM_COND_OPENED) from the connection manager, its state will be changed to READY. While a service context receives CM_COND_DISCONNECTED notification from the connection manager and this notification is not caused by end-user’s operation (new service selection), the state will be changed to READY and the client’s service selection notification callback function will be invoked, and the reason is signal loss. An explicit x_svctx_stop() will also change the state to READY.
 *      -#  <b>PENDING</b> when one of clients invokes x_svctx_select_svc() to change channel, this service context instance will invoke x_cm_connect() to make a connection and this instance’s state is changed to PENDING. The major job of a service context instance is almost performed in this state. After it received CM_COND_CONNECTED (connection is connected) notification from the connection manager, it will open a session to play several default stream components selected from the related SCDB. Wait until the stream manager notifies SM_SESS_COND_STARTED (the current program of this service or channel is playing), its state changes to PRESENTING. When the service context receives a SCDB_UPDATE notification from the SCDB in this state, it will perform the stream selection procedure again.
 *      -#  <b>PRESENTING</b> when a service context instance is in PRESENTING, it means the current program of this channel is presenting. The stream selection or stop operations are only performed in this state. When the service context receives a SCDB_UPDATE notification from the SCDB in this state, it will perform the stream selection procedure again.
 *      @image html  svctx_fsm.gif "Figure 3.1.2 Finite State Machine"
 *  
 *  @par
 *  @subsubsection sssecMW_SVCTX_3_1_3  3.1.3. Activator
 *      Figure 3.1.3 shows the generic program flow how a service context instance uses an activator. And the explanation for each step is described as follow:
 *  @par
 *      -#  A service context instance receives a service selection or service stop request from clients or applications and these requests (method invocations) are run on the client or application’s thread of control.
 *      -#  Service context uses activator provided function to send a private message to activator’s message queue.
 *      -#  This private message is received by the activator’s thread in its thread main function.
 *      -#  In the thread main function, the hooked message processing function provided by service context will be invoked.
 *      -#  This service context instance processes this request.
 *      -#  If necessary, the service context instance invokes related middleware module, for example, invoke the Connection Manager’s x_cm_connect() to make a connection.
 *      -#  When related middleware module has accomplished its job (e.g. connection is connected) or something wrong (e.g., signal loss), the service context will receive a notification. Such receiving is performed on this related middleware module’s thread of control.
 *      -#  The service context instance packages this notification into its private message, and send it to its activator’s message queue.
 *      -#  This private message is received by the activator’s thread in its thread main function.
 *      -#  In the thread main function, the hooked message processing function provided by service context will be invoked.
 *      -#  This service context instance processes this notification.
 *      -#  If necessary, this service context instance will invoke client provided callback function to notify something happen, for example, “service is presenting� “signal loss� and etc.
 *      @image html  svctx_activator.gif "Figure 3.1.3 Activator and Service Context"
 *  
 *  @par
 *  @subsection ssecMW_SVCTX_3_2  3.2. Notification for Client
 *      The following table describes the notifications that the client will receive
 *      <table>
 *      <tr>
 *      <th>Client issues</th><th>MW notifies/Service block check function</th><th>From state</th><th>Notify client the state is</th><th>
 *      Notify client the reason is
 *      </th>
 *      </tr>
 *      <tr>
 *      <td>c_svctx_select_svc()</td><td>-</td><td>PENDING</td><td>PENDING <br> (previous client)</td><td>
 *      INTERRUPTED
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>c_svctx_select_svc()</td><td>-</td><td>PRESENTING</td><td>PENDING <br> (previous client)</td><td>
 *      INTERRUPTED
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>c_svctx_select_svc()</td><td>CM_COND_CONNECTED <br> (Connection Manager notifies)</td><td>PENDING</td><td>PENDING</td><td>
 *      SIGNAL_LOCK
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>c_svctx_select_svc()</td><td></td><td>PENDING</td><td>PENDING</td><td>
 *      SERVICE_BLOCKED
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>c_svctx_select_svc()</td><td></td><td>PENDING</td><td>PRESENTING</td><td>
 *      NORMAL
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>c_svctx_stop_svc()</td><td>-</td><td>PENDING, <br> PRESENTING</td><td>READY</td><td>
 *      USER_STOP
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>c_svctx_stop_svc()</td><td>CM_COND_DISCONNECTED <br> (Connection Manager notifies)</td><td>READY,<br> PENDING, <br> PRESENTING</td><td>READY</td><td>
 *      SIGNAL_LOSS
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>c_svctx_select_stream() <br> c_svctx_audio_select_stream() <br> c_svctx_cc_select_stream()</td><td>SM_COND_STARTED <br> (Stream Manage notifies)</td><td>PRESENTING</td><td>PRESENTING</td><td>
 *      STREAM_STARTED
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>x_svctx_stop_stream()</td><td>SM_COND_CLOSED <br> (Stream Manage notifies)</td><td>PRESENTING</td><td>PRESENTING</td><td>
 *      STREAM_STARTED
 *      </td>
 *      </tr>
 *      </table>
 *      <center><b></b></center>
 *  @par
 *  @section secMW_SVCTX_4  4. Typical Case
 *  @par
 *  @subsection ssecMW_SVCTX_4_1  4.1. Priority A
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_1  4.1.1. How to create/destroy SVCTX object?
 *      c_svctx_open() create a svctx object.
 *      c_svctx_close() destroy a svctx object.
 *      @include svctx/a1.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_2  4.1.2. How to change the channel by using svctx?
 *      c_svctx_select_svc() change to a channel
 *      @include svctx/a2.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_3  4.1.3. How to change the input source by using svctx?
 *      c_svctx_select_svc() change to input source
 *      @include svctx/a3.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_4  4.1.4. What is the purpose of c_svctx_set_listener? What are the meanings of all SVCTX_NTFY_CODE_XXX ?
 *      custom implement "x_svctx_listener_nfy_fct" callback function.
 *      for listern notify message from SVCTX module.
 *      @include svctx/a4.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_5  4.1.5. How to override the "block check function" by svctx?
 *      c_svctx_set_default_parameters(), The parameter is pf_block_check_fct.
 *      the type is "x_svctx_svc_block_check_fct"
 *      @include svctx/a5.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_6  4.1.6. What types of block scenario supported? What is the input and output of "block check function"?
 *      @include svctx/a6.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_7  4.1.7. How to stop the current running svctx?
 *      c_svctx_sync_stop_svc() for sync stop current running service.
 *      c_svctx_stop_svc() for async top current running service.
 *      @include svctx/a7.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_8  4.1.8. How to trigger svctx to invoke "block check function" to do block check again?
 *      c_svctx_svc_block_check() judge block or not.
 *      c_svctx_unblock_check() use to block or unblock by second parameter.
 *      @include svctx/a8.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_9  4.1.9. How to get the current video resolution? What are the meanings of all members in VSH_SRC_RESOLUTION_INFO_T?
 *      c_svctx_get_video_src_resolution()
 *      @include svctx/a9.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_10  4.1.10. How to get the current audio information? What are the meanings of all members in ASH_AUDIO_INFO_T?
 *      c_svctx_get_audio_info()
 *      @include svctx/a10.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_11  4.1.11. How to change the screen mode?
 *      c_svctx_set_screen_mode()
 *      @include svctx/a11.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_12  4.1.12. How to check the current signal status? (signal or no-signal)
 *      c_svctx_is_signal_loss()
 *      @include svctx/a12.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_13  4.1.13. How to start "auto clock/phase" in VGA mode?
 *      @include svctx/a13.txt
 *  @par
 *  @subsection ssecMW_SVCTX_4_2  4.2. Priority B
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_1  4.2.1. What is the "default video mode" of svctx? How to change the "default video mode" of svctx?
 *      video mode means blank, deint or freeze. default vdioe mode indicates svctx will use
 *      this mode set video plane defaultly.
 *      c_svctx_set(SVCTX_COMMON_SET_DEFAULT_VIDEO_MODE)
 *      @include svctx/b1.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_2  4.2.2. How to get the current signal strength? How to get the current DBM signal strength?
 *      c_svctx_get_signal_level()
 *      c_svctx_get_dbm_signal_level()
 *      @include svctx/b2.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_3  4.2.3. How to iterate all specified type of streams? (eg. all audio streams)
 *      implement a callback pointer
 *      @include svctx/b3.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_4  4.2.4. How to select one stream if I have got a stream by iteration? (eg. select one audio stream)
 *      c_svctx_select_stream()
 *      @include svctx/b4.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_5  4.2.5. How to select CC stream? What parameters should client provide to MW?
 *      c_svctx_select_cc_stream()
 *      @include svctx/b5.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_6  4.2.6. How to select SUBTITLE stream? What parameters should client provide to MW?
 *      c_svctx_select_subtitle_stream()
 *      @include svctx/b6.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_7  4.2.7. How to get the "current running" stream information?
 *      c_svctx_get_stream_attr()
 *      @include svctx/b7.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_8  4.2.8. What are the meanings of all members in _SCDB_REC_T?
 *      @include svctx/b8.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_9  4.2.9. How to set the video plane mode? (ie. SCC_VID_MODE_XXX) Is it suggested to change the video plane mode in AP?
 *      c_svctx_set_video_plane()
 *      @include svctx/b9.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_10  4.2.10. How to change the Display Region/Video Region by "c_svctx_set_stream_attr"?
 *      SM_VSH_SET_TYPE_VIDIO_REGION
 *      SM_VSH_SET_TYPE_DISP_REGION
 *      @include svctx/b10.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_11  4.2.11. How to do "freeze/unfreeze" operation?
 *      c_svctx_freeze() / c_svctx_unfreeze()
 *      @include svctx/b11.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_12  4.2.12. How to get current event's information by SVCTX API?
 *      1. c_svctx_get_current_event_info_len()
 *      2. c_svctx_get_current_event_info()
 *      @include svctx/b12.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_13  4.2.13. How to check current CC/SUBTITLE/TELETEXT/EVENT capability for such running service?
 *      c_svctx_get_cc/subtitle/teletext/event_capablity()
 *      @include svctx/b13.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_14  4.2.14. How to enable/disable the "audio description" capability?
 *      c_svctx_set_ad_state()
 *      @include svctx/b14.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_15  4.2.15. How to enable the "snow-flake" when ATV with no-signal?
 *      SVCTX_COMMON_SET_SHOW_SNOW_AS_NO_SIGNAL
 *      @include svctx/b15.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_16  4.2.16. How to request svctx to make Audio and Video come out simultaneously?
 *      @include svctx/b16.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_17  4.2.17. How to change the block rule? (ie. SVCTX_BLOCK_RULE_T)
 *      c_svctx_set_default_parameters(), (The parameter is pf_block_check_fct.
 *      the type is "x_svctx_svc_block_check_fct") can set callback function.
 *      @include svctx/b17.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_18  4.2.18. How to get the current displaying color system?
 *      c_svctx_get_conn_attr()
 *      @include svctx/b18.txt
 
 * @else
 
 *  @page pageMW_SVCTX Service Context Detailed Description
 *  @par
 *  @section secMW_SVCTX_1  1. Introduction
 *  @par
 *      Service Context(SVCTX) describes the concept, program flows, data structures, and functions, which are required to implement the Service Context. The primary purpose of the Service Context is to represents an environment in which channels (services) are presented in DTV system. Clients (e.g. applications) could use Service Context objects to perform channel change job, and may also use Service Context objects to play a specific stream component.
 *  @par
 *      This document must be read in conjunction with the document, SW Design Middleware - Connection Manager, SW Design Middleware - Broadcast Connection Handler, SW Design Middleware - Stream Component Database, SW Design Middleware - Stream Manager, SW Design Stream Manager - Video Stream Handler, SW Design Stream Manager - Closed Caption Stream Handle, and SW Design Stream Manager - Stream Component Controller.
 *  @par
 *  @section secMW_SVCTX_2  2. Concept
 *  @par
 *  @subsection ssecMW_SVCTX_2_1  2.1. Service Context
 *  @par
 *      Clients (e.g. applications) of the Service Context use the defined interface to open a service context instance or attach to a existing service context instance, once a service has been obtained by some means (e.g. from populated Service List), application could use this service context instance to present the service (channel). In other words, the Service Context allows the clients to control the presentation of services in a simple, high-level approach.
 *  @par
 *      Each service context instance can present a different service (channel), completely independently of the other service context instances. Operations carried out on one service context instance will not affect another (unless they cause tuning which prevents the contents of a service in another service context from being presented).
 *  @par
 *      Service Context coordinates the Connection Manager, Stream Component Database, and the Stream Manager (i.e. the service-selection related middleware modules) to perform service (channel) and stream related operations. Their relationship is depicted in Figure 2.1 @image html  svctx_relationships.gif "Figure 2.1 Relationships between Applications, Service Context and Service-Selection related Middleware Modules"
 *  @par
 *      Basically, the concept of the service context is from the <b>Facade Design Pattern</b> documented in the well-known book - Design Patterns by The Gang of Four. This pattern's intent is to
 *  @par
 *      <i>Provide a unified interface to a set of interfaces in a subsystem. Facade defines a higher-level interface that makes the subsystem easier to use.</i>
 *  @par
 *      So, Service Context offers the following benefits:
 *  @par
 *      -#  It decouples applications from the service-selection related middleware modules, thereby reducing the number of modules or objects that applications deal with and making the service selection (channel change) easier.
 *      -#  It provides weak coupling between the service-selection related middleware modules and applications. Applications that use the Service Context don't have to access the service-selection related middleware modules directly.
 *      -#  It doesn't prevent applications from using the service-selection related middleware modules if they need to. Thus application programmer can choose between ease of use and generality.
 *  
 *  @par
 *      The applications can present a service without needing to know about the service's audio components, video components, or closed captioning components, Service Context will select default components automatically. However, while the service (channel) is presenting, applications can use the service context instance to play a specific stream component to change to different video, audio, or closed captioning according to end-user's need.
 *  @par
 *  @subsection ssecMW_SVCTX_2_2  2.2. Activator
 *  @par
 *      In order to handle multiple client requests and middleware modules' notifications simultaneously, a service context instance is running on its own thread of control and uses a message queue to synchronize access to this instance. In Design Patterns world, there's a existing design pattern used to handle such problem, called Active Object pattern and documented by Douglas C. Schmidt in his well-known book - Pattern-Oriented Software Architecture, Patterns for Concurrent and Networked Objects, Volume 2. The following is the Active Object Pattern's intent
 *  @par
 *      <i>The Active Object design pattern decouples method execution from method invocation to enhance concurrency and simplify synchronized access to an object that resides in its own thread of control.</i>
 *  @par
 *      Activator is an implementation of this pattern, and the service context module uses it to make a service context instance to be active. Activator combines the thread and message queue into an abstract object, and provides a construction function to let service context to specify its private message's size, thread priority, maximum amount of message, and a callback function to invoke when a private message is retrieved from the message queue. Also, the activator provides a message sending function for service context to send its private message to this activator's message queue.
 *  @par
 *  @section secMW_SVCTX_3  3. Brief Design
 *  @par
 *  @subsection ssecMW_SVCTX_3_1  3.1. Service Context Structure
 *  @par
 *      The Service Context Module consists essentially of the following functional blocks
 *  @par
 *      -   Service Context Pool.
 *      -   State Objects
 *      -   Activator
 *  
 *  @par
 *      And, in order to be shared by many clients and applications, the service context's APIs must be thread-safe.
 *  @par
 *  @subsubsection sssecMW_SVCTX_3_1_1  3.1.1. Service Context Pool
 *      While service context module initializing, it will create a pool to maintain a pre-defined amount of service context instances, and when clients or applications request to open a service context (by name), if this name is not used, a new service context instance will be chosen from the pool and start to serve. The other clients (applications) can also use the same API to access this service context object if it knows the name. Such behaviors will let one service context object to be shared between many clients or applications, and don't need to perform the service selection job again. Since the service context objects are rare resource (they will access many hardware devices, likes tuner, video decoder, video plane, plane mixer, and audio decoder, and the amount of each kind of device is limited), if the pool is empty, the request to open a new service context will be failed. After the last client or application return the handle back, the service context instance will be recycled and put back into the service context pool.
 *  @par
 *      A service context instance can only process one service selection request or stream selection request at a time. If a service context instance is already processing a service selection request and a new service selection request is incoming from another or same client, the old service selection request will be stopped and the issuer of this request will receive an “Interrupted�notification, then the service context instance processes the new service selection request as usual.
 *  @par
 *      In order to identify the service context instance easily when receiving notifications from the Connection Manager, the SCDB or the Stream Manager, the service context passes a so-called system service context handle to these modules as the pv_nfy_tag. For example while invoking
 *  @par
 *      INT32 x_cm_connect (HANDLE_T             h_obj,<br>
 *      const CM_COMMAND_T*  pt_connect_cmd,<br>
 *      VOID*                pv_nfy_tag,<br>
 *      x_cm_nfy_fct         pf_nfy,<br>
 *      HANDLE_T*            ph_conn,<br>
 *      CM_COND_T*           pe_cond);<br>
 *  @par
 *      A service context instance set the pv_nfy_tag to its <b>system service context handle</b>. This system handle is used to identify the service context instance. So while the service context instance is first time in <b>POOLED</b> state, a system service context handle is created to associate with this instance. And because the service context HANDLE created for clients or application will be disappeared if clients or applications free it (via x_svctx_open() function), it is not suitable for this purpose.
 *  @par
 *      If just pass the service context instance's memory address to be the pv_nfy_tag, and in some condition, this service context instance had been destroyed and the notification from the connection manager is incoming, then we don't know this instance is still alive or not and this could cause the well-known segmentation fault error. Using the HANDLE to be the pv_nfy_tag provides an easy way to determine the existence of the associated instance.
 *      @image html  svctx_pool.gif "Figure 3.1.1 Service Context Pool"
 *  @par
 *  @subsubsection sssecMW_SVCTX_3_1_2  3.1.2. State Objects
 *  @paragraph ssssecMW_SVCTX_3_1_2_1  3.1.2.1. State Machine
 *      The finite state machine of a service context instance is depicted in Figure 3.1.2 and each state is described as follows:
 *      -#  <b>READY</b> when the very first client invokes the API x_svctx_open(), an POOLED service context will be chosen from the Service Context Pool and this instance will open a pipe automatically. After it receives the notification about that the pipe is opened (CM_COND_OPENED) from the connection manager, its state will be changed to READY. While a service context receives CM_COND_DISCONNECTED notification from the connection manager and this notification is not caused by end-user’s operation (new service selection), the state will be changed to READY and the client’s service selection notification callback function will be invoked, and the reason is signal loss. An explicit x_svctx_stop() will also change the state to READY.
 *      -#  <b>PENDING</b> when one of clients invokes x_svctx_select_svc() to change channel, this service context instance will invoke x_cm_connect() to make a connection and this instance’s state is changed to PENDING. The major job of a service context instance is almost performed in this state. After it received CM_COND_CONNECTED (connection is connected) notification from the connection manager, it will open a session to play several default stream components selected from the related SCDB. Wait until the stream manager notifies SM_SESS_COND_STARTED (the current program of this service or channel is playing), its state changes to PRESENTING. When the service context receives a SCDB_UPDATE notification from the SCDB in this state, it will perform the stream selection procedure again.
 *      -#  <b>PRESENTING</b> when a service context instance is in PRESENTING, it means the current program of this channel is presenting. The stream selection or stop operations are only performed in this state. When the service context receives a SCDB_UPDATE notification from the SCDB in this state, it will perform the stream selection procedure again.
 *      @image html  svctx_fsm.gif "Figure 3.1.2 Finite State Machine"
 *  
 *  @par
 *  @subsubsection sssecMW_SVCTX_3_1_3  3.1.3. Activator
 *      Figure 3.1.3 shows the generic program flow how a service context instance uses an activator. And the explanation for each step is described as follow:
 *  @par
 *      -#  A service context instance receives a service selection or service stop request from clients or applications and these requests (method invocations) are run on the client or application’s thread of control.
 *      -#  Service context uses activator provided function to send a private message to activator’s message queue.
 *      -#  This private message is received by the activator’s thread in its thread main function.
 *      -#  In the thread main function, the hooked message processing function provided by service context will be invoked.
 *      -#  This service context instance processes this request.
 *      -#  If necessary, the service context instance invokes related middleware module, for example, invoke the Connection Manager’s x_cm_connect() to make a connection.
 *      -#  When related middleware module has accomplished its job (e.g. connection is connected) or something wrong (e.g., signal loss), the service context will receive a notification. Such receiving is performed on this related middleware module’s thread of control.
 *      -#  The service context instance packages this notification into its private message, and send it to its activator’s message queue.
 *      -#  This private message is received by the activator’s thread in its thread main function.
 *      -#  In the thread main function, the hooked message processing function provided by service context will be invoked.
 *      -#  This service context instance processes this notification.
 *      -#  If necessary, this service context instance will invoke client provided callback function to notify something happen, for example, “service is presenting� “signal loss� and etc.
 *      @image html  svctx_activator.gif "Figure 3.1.3 Activator and Service Context"
 *  
 *  @par
 *  @subsection ssecMW_SVCTX_3_2  3.2. Notification for Client
 *      The following table describes the notifications that the client will receive
 *      <table>
 *      <tr>
 *      <th>Client issues</th><th>MW notifies/Service block check function</th><th>From state</th><th>Notify client the state is</th><th>
 *      Notify client the reason is
 *      </th>
 *      </tr>
 *      <tr>
 *      <td>x_svctx_select_svc()</td><td>-</td><td>PENDING</td><td>PENDING <br> (previous client)</td><td>
 *      INTERRUPTED
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>x_svctx_select_svc()</td><td>-</td><td>PRESENTING</td><td>PENDING <br> (previous client)</td><td>
 *      INTERRUPTED
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>x_svctx_select_svc()</td><td>CM_COND_CONNECTED <br> (Connection Manager notifies)</td><td>PENDING</td><td>PENDING</td><td>
 *      SIGNAL_LOCK
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>x_svctx_select_svc()</td><td></td><td>PENDING</td><td>PENDING</td><td>
 *      SERVICE_BLOCKED
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>x_svctx_select_svc()</td><td></td><td>PENDING</td><td>PRESENTING</td><td>
 *      NORMAL
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>x_svctx_stop_svc()</td><td>-</td><td>PENDING, <br> PRESENTING</td><td>READY</td><td>
 *      USER_STOP
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>x_svctx_stop_svc()</td><td>CM_COND_DISCONNECTED <br> (Connection Manager notifies)</td><td>READY,<br> PENDING, <br> PRESENTING</td><td>READY</td><td>
 *      SIGNAL_LOSS
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>x_svctx_select_stream() <br> c_svctx_audio_select_stream() <br> c_svctx_cc_select_stream()</td><td>SM_COND_STARTED <br> (Stream Manage notifies)</td><td>PRESENTING</td><td>PRESENTING</td><td>
 *      STREAM_STARTED
 *      </td>
 *      </tr>
 *      <tr>
 *      <td>x_svctx_stop_stream()</td><td>SM_COND_CLOSED <br> (Stream Manage notifies)</td><td>PRESENTING</td><td>PRESENTING</td><td>
 *      STREAM_STARTED
 *      </td>
 *      </tr>
 *      </table>
 *      <center><b></b></center>
 *  @par
 *  @section secMW_SVCTX_4  4. Typical Case
 *  @par
 *  @subsection ssecMW_SVCTX_4_1  4.1. Priority A
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_1  4.1.1. How to create/destroy SVCTX object?
 *      x_svctx_open() create a svctx object.
 *      x_svctx_close() destroy a svctx object.
 *      @include svctx/xa1.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_2  4.1.2. How to change the channel by using svctx?
 *      x_svctx_select_svc() change to a channel
 *      @include svctx/xa2.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_3  4.1.3. How to change the input source by using svctx?
 *      x_svctx_select_svc() change to input source
 *      @include svctx/xa3.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_4  4.1.4. What is the purpose of c_svctx_set_listener? What are the meanings of all SVCTX_NTFY_CODE_XXX ?
 *      custom implement "x_svctx_listener_nfy_fct" callback function.
 *      for listern notify message from SVCTX module.
 *      @include svctx/xa4.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_5  4.1.5. How to override the "block check function" by svctx?
 *      x_svctx_set_default_parameters(), The parameter is pf_block_check_fct.
 *      the type is "x_svctx_svc_block_check_fct"
 *      @include svctx/xa5.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_6  4.1.6. What types of block scenario supported? What is the input and output of "block check function"?
 *      @include svctx/xa6.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_7  4.1.7. How to stop the current running svctx?
 *      x_svctx_sync_stop_svc() for sync stop current running service.
 *      x_svctx_stop_svc() for async top current running service.
 *      @include svctx/xa7.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_8  4.1.8. How to trigger svctx to invoke "block check function" to do block check again?
 *      x_svctx_svc_block_check() judge block or not.
 *      x_svctx_unblock_check() use to block or unblock by second parameter.
 *      @include svctx/xa8.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_9  4.1.9. How to get the current video resolution? What are the meanings of all members in VSH_SRC_RESOLUTION_INFO_T?
 *      x_svctx_get_video_src_resolution()
 *      @include svctx/xa9.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_10  4.1.10. How to get the current audio information? What are the meanings of all members in ASH_AUDIO_INFO_T?
 *      x_svctx_get_audio_info()
 *      @include svctx/xa10.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_11  4.1.11. How to change the screen mode?
 *      x_svctx_set_screen_mode()
 *      @include svctx/xa11.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_12  4.1.12. How to check the current signal status? (signal or no-signal)
 *      x_svctx_is_signal_loss()
 *      @include svctx/xa12.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_1_13  4.1.13. How to start "auto clock/phase" in VGA mode?
 *      @include svctx/xa13.txt
 *  @par
 *  @subsection ssecMW_SVCTX_4_2  4.2. Priority B
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_1  4.2.1. What is the "default video mode" of svctx? How to change the "default video mode" of svctx?
 *      video mode means blank, deint or freeze. default vdioe mode indicates svctx will use
 *      this mode set video plane defaultly.
 *      x_svctx_set(SVCTX_COMMON_SET_DEFAULT_VIDEO_MODE)
 *      @include svctx/xb1.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_2  4.2.2. How to get the current signal strength? How to get the current DBM signal strength?
 *      x_svctx_get_signal_level()
 *      x_svctx_get_dbm_signal_level()
 *      @include svctx/xb2.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_3  4.2.3. How to iterate all specified type of streams? (eg. all audio streams)
 *      implement a callback pointer
 *      @include svctx/xb3.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_4  4.2.4. How to select one stream if I have got a stream by iteration? (eg. select one audio stream)
 *      x_svctx_select_stream()
 *      @include svctx/xb4.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_5  4.2.5. How to select CC stream? What parameters should client provide to MW?
 *      x_svctx_select_cc_stream()
 *      @include svctx/xb5.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_6  4.2.6. How to select SUBTITLE stream? What parameters should client provide to MW?
 *      x_svctx_select_subtitle_stream()
 *      @include svctx/xb6.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_7  4.2.7. How to get the "current running" stream information?
 *      x_svctx_get_stream_attr()
 *      @include svctx/xb7.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_8  4.2.8. What are the meanings of all members in _SCDB_REC_T?
 *      @include svctx/xb8.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_9  4.2.9. How to set the video plane mode? (ie. SCC_VID_MODE_XXX) Is it suggested to change the video plane mode in AP?
 *      x_svctx_set_video_plane()
 *      @include svctx/xb9.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_10  4.2.10. How to change the Display Region/Video Region by "c_svctx_set_stream_attr"?
 *      SM_VSH_SET_TYPE_VIDIO_REGION
 *      SM_VSH_SET_TYPE_DISP_REGION
 *      @include svctx/xb10.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_11  4.2.11. How to do "freeze/unfreeze" operation?
 *      x_svctx_freeze() / x_svctx_unfreeze()
 *      @include svctx/xb11.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_12  4.2.12. How to get current event's information by SVCTX API?
 *      1. x_svctx_get_current_event_info_len()
 *      2. x_svctx_get_current_event_info()
 *      @include svctx/xb12.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_13  4.2.13. How to check current CC/SUBTITLE/TELETEXT/EVENT capability for such running service?
 *      x_svctx_get_cc/subtitle/teletext/event_capablity()
 *      @include svctx/xb13.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_14  4.2.14. How to enable/disable the "audio description" capability?
 *      x_svctx_set_ad_state()
 *      @include svctx/xb14.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_15  4.2.15. How to enable the "snow-flake" when ATV with no-signal?
 *      SVCTX_COMMON_SET_SHOW_SNOW_AS_NO_SIGNAL
 *      @include svctx/xb15.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_16  4.2.16. How to request svctx to make Audio and Video come out simultaneously?
 *      @include svctx/xb16.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_17  4.2.17. How to change the block rule? (ie. SVCTX_BLOCK_RULE_T)
 *      x_svctx_set_default_parameters(), (The parameter is pf_block_check_fct.
 *      the type is "x_svctx_svc_block_check_fct") can set callback function.
 *      @include svctx/xb17.txt
 *  @par
 *  @subsubsection sssecMW_SVCTX_4_2_18  4.2.18. How to get the current displaying color system?
 *      x_svctx_get_conn_attr()
 *      @include svctx/xb18.txt

 *   @end
 */

