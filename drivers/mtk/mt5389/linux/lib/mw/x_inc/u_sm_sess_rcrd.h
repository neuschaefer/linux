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
/*! @file u_sm_sess_rcrd.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains Stream Manager's record session handler exported 
 *         definitions.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_STRM_MNGR_SESS_RCRD_HDLR
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_SM_SESS_RCRD_H_
#define _U_SM_SESS_RCRD_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/

/* set/get types *************************************************************/
#define SM_GRP_SESS_RCRD                (( SM_GRP_LAST_ENTRY + 16 ) << SM_CODE_BITS)    /**<record session handler cmd group   */


/* get types *****************************************************************/
#define SM_SESS_RCRD_GET_TYPE_IGNORE            ((SM_GET_TYPE_T)(SM_GRP_SESS_RCRD | 0))    /**<not to handle        */
#define SM_SESS_RCRD_GET_TYPE_PVR_BUFFER_INFO   ((SM_GET_TYPE_T)(SM_GRP_SESS_RCRD | 1))    /**< get PVR ring buffer   info     */
#define SM_SESS_RCRD_GET_TYPE_PVR_BUFFER_STATUS ((SM_GET_TYPE_T)(SM_GRP_SESS_RCRD | 2))    /**< get PVR ring buffer status         */

/* set types *****************************************************************/
#define SM_SESS_RCRD_SET_TYPE_IGNORE            ((SM_SET_TYPE_T)(SM_GRP_SESS_RCRD | 0))    /**< not to handle         */
#define SM_SESS_RCRD_SET_TYPE_ENCRYPTION        ((SM_SET_TYPE_T)(SM_GRP_SESS_RCRD | 1))    /**< set encryption to recorded es       */

/* event types *****************************************************************/
/* APP events . */
#define SM_EVN_SESS_RCRD_GRP_APP              SM_MAKE_EVN_GRP(SM_EVN_GRP_SESS_RCRD, 0)    /**< record session handler  event group        */
   
#define SM_EVN_SESS_RCRD_APP_OPEN_REQ         SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 0)    /**<record session open request         */
#define SM_EVN_SESS_RCRD_APP_PLAY_REQ         SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 1)    /**<record session play request         */
#define SM_EVN_SESS_RCRD_APP_STOP_REQ         SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 2)    /**<record session stop request         */
#define SM_EVN_SESS_RCRD_APP_CLOSE_REQ        SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 3)    /**<record session close request        */
#define SM_EVN_SESS_RCRD_ALL_OPENED           SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 4)    /**<record session opened request         */
#define SM_EVN_SESS_RCRD_ALL_PLAYED           SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 5)    /**<record session played request         */
#define SM_EVN_SESS_RCRD_ALL_STOPPED          SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 6)    /**<record session stopped request         */
#define SM_EVN_SESS_RCRD_ALL_CLOSED           SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 7)    /**<record session closed request         */
#define SM_EVN_SESS_RCRD_COMP_ERR             SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 8)    /**<record session component error event     */
#define SM_EVN_SESS_RCRD_SCDB_CLOSED          SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 9)    /**< scdb closed event       */
#define SM_EVN_SESS_RCRD_SCDB_UPDATE          SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 10)    /**<scdb update event       */
#define SM_EVN_SESS_RCRD_PVR_CTRL_OVERFLOW    SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 11)    /**<PVR ring buffer overflow  event        */
#define SM_EVN_SESS_RCRD_PVR_CTRL_FLUSHED     SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 12)    /**< PVR ring buffer flushed  event           */

#define SM_EVN_SESS_RCRD_INTERNAL_ERR         SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 13)    /**< record session internal error event        */

/*need notify to AP so define here*/
#define SM_EVN_SESS_RCRD_PVR_CTRL_OPENED      SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 14)    /**<PVR ring buffer opened event        */

/*#define SM_EVN_SESS_RCRD_PVR_PMT_UPDATED      SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 15)*/
#define SM_EVN_SESS_RCRD_REACH_BOUND          SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 15)    /**<TS file reach bound        */
#define SM_EVN_SESS_RCRD_STRG_FULL            SM_MAKE_EVN(SM_EVN_SESS_RCRD_GRP_APP, 16)    /**<TS file is  full                  */


/*-----------------------------------------------------------------------------
                    structures
-----------------------------------------------------------------------------*/

/* SM_SESS_RCRD_CFG_T.ui1_frame_mask */
#define SM_SESS_RCRD_FRAME_TYPE_MASK_I       MAKE_BIT_MASK_8(0)    /**< I frame       */
#define SM_SESS_RCRD_FRAME_TYPE_MASK_P       MAKE_BIT_MASK_8(1)    /**< P frame       */
#define SM_SESS_RCRD_FRAME_TYPE_MASK_B       MAKE_BIT_MASK_8(2)    /**< B frame       */

/*------------------------------------------------------------------*/
/*! @enum SM_SESS_RCRD_PVR_OVERFLOW_OPT_T
 *  @brief Defines record behaviour when PVR buffer is overflow
 *  @code
 *  typedef enum _SM_SESS_RCRD_PVR_OVERFLOW_OPT_T
 *  {
 *      SM_SESS_RCRD_OVERFLOW_OPT_CONTINUOUS_TICK = 0,
 *      SM_SESS_RCRD_OVERFLOW_OPT_SKIP_TICK,
 *      SM_SESS_RCRD_OVERFLOW_OPT_LAST_ENTRY
 *  }SM_SESS_RCRD_PVR_OVERFLOW_OPT_T;
 *  @endcode
 *  @li@c  SM_SESS_RCRD_OVERFLOW_OPT_CONTINUOUS_TICK          -continue tick 
 *  @li@c  SM_SESS_RCRD_OVERFLOW_OPT_SKIP_TICK                       -ignore tick which is not continuous
 *  @li@c  SM_SESS_RCRD_OVERFLOW_OPT_LAST_ENTRY                   -bound mark
 */
/*------------------------------------------------------------------*/
typedef enum _SM_SESS_RCRD_PVR_OVERFLOW_OPT_T
{
    SM_SESS_RCRD_OVERFLOW_OPT_CONTINUOUS_TICK = 0,
    SM_SESS_RCRD_OVERFLOW_OPT_SKIP_TICK,
    
    SM_SESS_RCRD_OVERFLOW_OPT_LAST_ENTRY
}SM_SESS_RCRD_PVR_OVERFLOW_OPT_T;

/* record config (x_sm_open_sess (pv_sess_data)) */
/*------------------------------------------------------------------*/
/*! @struct SM_SESS_RCRD_CFG_T
 *  @brief Descript record related information such as PVR alignment. 
 *  This should be set when start to record
 *  @code
 *  typedef struct _SM_SESS_RCRD_CFG_T
 *  {
 *      UINT32                            ui4_flags;      
 *      CHAR*                             s_name_rec_util;
 *      UINT8                             ui1_frame_mask;
 *      UINT32                            ui4_ms_tick_period;
 *      UINT32                            ui4_pvr_buf_align;
 *      SM_SESS_RCRD_PVR_OVERFLOW_OPT_T   e_pvr_overflow_opt;
 *  } SM_SESS_RCRD_CFG_T;
 *  @endcode
 *  @li@c  ui4_flags                                               -indicate which fieldare valid
 *  @li@c  s_name_rec_util                                   -record utility name
 *  @li@c  ui1_frame_mask                                   -frame mask
 *  @li@c  ui4_ms_tick_period                               -tick period to notify tick data
 *  @li@c  ui4_pvr_buf_align                                  -PVR ring buffer alignement
 *  @li@c  e_pvr_overflow_opt                              -how to handle PVR ring buffer overflow event
 */
/*------------------------------------------------------------------*/
typedef struct _SM_SESS_RCRD_CFG_T
{
    UINT32                            ui4_flags;      /* reserved, should be 0 */
    CHAR*                             s_name_rec_util;
    UINT8                             ui1_frame_mask;
    UINT32                            ui4_ms_tick_period;
    UINT32                            ui4_pvr_buf_align;
    SM_SESS_RCRD_PVR_OVERFLOW_OPT_T   e_pvr_overflow_opt;
} SM_SESS_RCRD_CFG_T;


#endif  /* _U_SM_SESS_RCRD_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_STRM_MNGR_SESS_RCRD_HDLR */

