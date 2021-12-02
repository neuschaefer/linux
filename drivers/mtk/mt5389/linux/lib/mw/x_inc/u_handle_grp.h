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
/*! @file u_handle_grp.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains handle group definitions which are known
 *         to the whole Middleware, Drivers and Applications.
 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_HANDLE
 *  @{ 
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_HANDLE_GRP_H_
#define _U_HANDLE_GRP_H_


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "u_handle.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Specify handle groups */
#define HT_GROUP_SIZE     ((HANDLE_TYPE_T) 64)    /**<  size of one group      */

/* A handle type of value '0' will be reserved as invalid handle. */
/* The first HT_GROUP_SIZE handle type values are reserved by the */
/* handle library. The first group starts at value HT_GROUP_SIZE. */
#define HT_RES_BY_HANDLE_LIB  HT_GROUP_SIZE    /**< size of one group       */

#define HT_GROUP_RM       ((HANDLE_TYPE_T) ( 1 * HT_GROUP_SIZE))  /**< Resource Manager */
#define HT_GROUP_OS       ((HANDLE_TYPE_T) ( 2 * HT_GROUP_SIZE))  /**< OS */
#define HT_GROUP_CM       ((HANDLE_TYPE_T) ( 3 * HT_GROUP_SIZE))  /**< Connection Manager */
#define HT_GROUP_SM       ((HANDLE_TYPE_T) ( 4 * HT_GROUP_SIZE))  /**< Stream Manager */
#define HT_GROUP_FM       ((HANDLE_TYPE_T) ( 5 * HT_GROUP_SIZE))  /**< File Manager */
#define HT_GROUP_TM       ((HANDLE_TYPE_T) ( 6 * HT_GROUP_SIZE))  /**< Table Manager RESERVE 128 TYPES. */
#define HT_GROUP_COMMON   ((HANDLE_TYPE_T) ( 8 * HT_GROUP_SIZE))  /**< Common libraries */
#define HT_GROUP_CDB      ((HANDLE_TYPE_T) ( 9 * HT_GROUP_SIZE))  /**< Core DB library */
#define HT_GROUP_PM       ((HANDLE_TYPE_T) (10 * HT_GROUP_SIZE))  /**< Process Manager */
#define HT_GROUP_GL       ((HANDLE_TYPE_T) (11 * HT_GROUP_SIZE))  /**< Graphic library */
#define HT_GROUP_BRDCST   ((HANDLE_TYPE_T) (12 * HT_GROUP_SIZE))  /**< Broadcast handler */
#define HT_GROUP_IOM      ((HANDLE_TYPE_T) (13 * HT_GROUP_SIZE))  /**< IO Manager */
#define HT_GROUP_CL       ((HANDLE_TYPE_T) (14 * HT_GROUP_SIZE))  /**< Compression / Decompression library */
#define HT_GROUP_WGL      ((HANDLE_TYPE_T) (15 * HT_GROUP_SIZE))  /**< Widget library. RESERVE 256 TYPES. */
#define HT_GROUP_EVCTX    ((HANDLE_TYPE_T) (20 * HT_GROUP_SIZE))  /**< Event context */
#define HT_GROUP_SVCTX    ((HANDLE_TYPE_T) (21 * HT_GROUP_SIZE))  /**< Service context */
#define HT_GROUP_FLM      ((HANDLE_TYPE_T) (22 * HT_GROUP_SIZE))  /**< Favorites List Manager context */
#define HT_GROUP_SVL_BLDR ((HANDLE_TYPE_T) (23 * HT_GROUP_SIZE))  /**< SVL builder. */
#define HT_GROUP_AEE      ((HANDLE_TYPE_T) (24 * HT_GROUP_SIZE))  /**< AEE Manager */
#define HT_GROUP_FONT     ((HANDLE_TYPE_T) (25 * HT_GROUP_SIZE))  /**< Font engine */
#define HT_GROUP_ATV      ((HANDLE_TYPE_T) (26 * HT_GROUP_SIZE))  /**< ATV engine */
#define HT_GROUP_CFG      ((HANDLE_TYPE_T) (27 * HT_GROUP_SIZE))  /**< Configuration Manager */
#define HT_GROUP_IMG      ((HANDLE_TYPE_T) (28 * HT_GROUP_SIZE))  /**< Image library */
#define HT_GROUP_SCDB     ((HANDLE_TYPE_T) (29 * HT_GROUP_SIZE))  /**< Stream Component database module */
#define HT_GROUP_SVL      ((HANDLE_TYPE_T) (30 * HT_GROUP_SIZE))  /**< Service List library */
#define HT_GROUP_TSL      ((HANDLE_TYPE_T) (31 * HT_GROUP_SIZE))  /**< Transport Stream List library */
#define HT_GROUP_DT       ((HANDLE_TYPE_T) (32 * HT_GROUP_SIZE))  /**< DateTime library */
#define HT_GROUP_OOB      ((HANDLE_TYPE_T) (33 * HT_GROUP_SIZE))  /**< Out-of-band handler */
#define HT_GROUP_1394     ((HANDLE_TYPE_T) (34 * HT_GROUP_SIZE))  /**< 1394 handler */
#define HT_GROUP_POD      ((HANDLE_TYPE_T) (35 * HT_GROUP_SIZE))  /**< POD stack */
#define HT_GROUP_DSM      ((HANDLE_TYPE_T) (36 * HT_GROUP_SIZE))  /**< Device Status Manager */
#define HT_GROUP_ABRDCST  ((HANDLE_TYPE_T) (37 * HT_GROUP_SIZE))  /**< Analog Broadcast handler */
#define HT_GROUP_AVC      ((HANDLE_TYPE_T) (38 * HT_GROUP_SIZE))  /**< AVC handler */
#define HT_GROUP_RRCTX    ((HANDLE_TYPE_T) (39 * HT_GROUP_SIZE))  /**< Rating Recion Context */
#define HT_GROUP_PCL      ((HANDLE_TYPE_T) (40 * HT_GROUP_SIZE))  /**< Power Control Library */
#define HT_GROUP_NETWORK  ((HANDLE_TYPE_T) (41 * HT_GROUP_SIZE))  /**< Network Libraries */
#define HT_GROUP_VBIF     ((HANDLE_TYPE_T) (42 * HT_GROUP_SIZE))  /**< VBI Filter */
#define HT_GROUP_MHEG_5   ((HANDLE_TYPE_T) (43 * HT_GROUP_SIZE))  /**< MHEG5 */
#define HT_GROUP_PLAYBACK ((HANDLE_TYPE_T) (44 * HT_GROUP_SIZE))  /**< Playback handler */
#define HT_GROUP_MSVCTX   ((HANDLE_TYPE_T) (45 * HT_GROUP_SIZE))  /**< Media service context handler */
#define HT_GROUP_MINFO    ((HANDLE_TYPE_T) (46 * HT_GROUP_SIZE))  /**< Media info handler */
#define HT_GROUP_MIDXBULD ((HANDLE_TYPE_T) (47 * HT_GROUP_SIZE))  /**< Media index builder handler */
#define HT_GROUP_CECM     ((HANDLE_TYPE_T) (48 * HT_GROUP_SIZE))  /**< CEC Manager */
#define HT_GROUP_DM       ((HANDLE_TYPE_T) (49 * HT_GROUP_SIZE))  /**< Device Manager */
#define HT_GROUP_SSUM     ((HANDLE_TYPE_T) (50 * HT_GROUP_SIZE))  /**< SSU/OAD Manager */
#define HT_GROUP_TVGUIDE  ((HANDLE_TYPE_T) (51 * HT_GROUP_SIZE))  /**< TV Guide */
#define HT_GROUP_MTP      ((HANDLE_TYPE_T) (52 * HT_GROUP_SIZE))  /**< MTP Manager */
#define HT_GROUP_NWL      ((HANDLE_TYPE_T) (53 * HT_GROUP_SIZE))  /**< Network List */
#define HT_GROUP_DRM      ((HANDLE_TYPE_T) (54 * HT_GROUP_SIZE))  /**< DRM */
#define HT_GROUP_DF_MNGR  ((HANDLE_TYPE_T) (55 * HT_GROUP_SIZE))  /**< DFM */
#define HT_GROUP_LOL      ((HANDLE_TYPE_T) (56 * HT_GROUP_SIZE))  /**< LOL */
#define HT_GROUP_REC_UTIL ((HANDLE_TYPE_T) (57 * HT_GROUP_SIZE))  /**< Record Utility*/
#define HT_GROUP_SATL     ((HANDLE_TYPE_T) (58 * HT_GROUP_SIZE))  /**< Satellite List */
#define HT_GROUP_CAP      ((HANDLE_TYPE_T) (59 * HT_GROUP_SIZE))  /**< Capture */
#define HT_GROUP_DMX_UTIL ((HANDLE_TYPE_T) (60 * HT_GROUP_SIZE))  /**< Demux Utility */
#define HT_GROUP_EDB      ((HANDLE_TYPE_T) (61 * HT_GROUP_SIZE))  /**< event database */
#define HT_GROUP_GINGA    ((HANDLE_TYPE_T) (62 * HT_GROUP_SIZE))  /**< SBTVD Ginga */

#define HT_NB_GROUPS  (63)    /**<        */

#endif /* _U_HANDLE_GRP_H_ */
/*----------------------------------------------------------------------------*/
/*! @} */                                             /* end of groupMW_HANDLE */
/*----------------------------------------------------------------------------*/

