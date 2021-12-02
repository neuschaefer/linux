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
 * $RCSfile: def_dbg_level_mw.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/12 $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: 53b66b4a3dd984f2a68f60ab47208b7e $
 *
 * Description:
 *         This header file contains all default debug levels for every
 *         middleware software module.
 *---------------------------------------------------------------------------*/

#ifndef _DEF_DBG_LEVEL_MW_H_
#define _DEF_DBG_LEVEL_MW_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "u_dbg.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Determine the default common or central debug level   */
/* Note that only the DBG_INIT_LEVEL_MW_DBG will require */
/* a check if DBG_INIT_LEVEL_COMMON has been set.        */
#ifdef DBG_INIT_LEVEL_COMMON
#undef DBG_INIT_LEVEL_MW_DBG
#define DBG_INIT_LEVEL_MW_DBG   DBG_INIT_LEVEL_COMMON
#else
#ifndef DBG_INIT_LEVEL_MW_DBG
#define DBG_INIT_LEVEL_MW_DBG   DBG_LEVEL_NONE
#endif
#endif

/* AEE default debug level */
#ifndef DBG_INIT_LEVEL_MW_AEE
#define DBG_INIT_LEVEL_MW_AEE   DBG_LEVEL_NONE
#endif

/* ATV Manager default debug level */
#ifndef DBG_INIT_LEVEL_MW_ATV_MNGR
#define DBG_INIT_LEVEL_MW_ATV_MNGR   DBG_LEVEL_NONE
#endif

/* ATV Line Translator default debug level */
#ifndef DBG_INIT_LEVEL_MW_ATV_LINE_XLS
#define DBG_INIT_LEVEL_MW_ATV_LINE_XLS   DBG_LEVEL_NONE
#endif

/* ATV 8202 Translator default debug level */
#ifndef DBG_INIT_LEVEL_MW_ATV_8202_XLS
#define DBG_INIT_LEVEL_MW_ATV_8202_XLS   DBG_LEVEL_NONE
#endif

/* ATV 8205 Translator default debug level */
#ifndef DBG_INIT_LEVEL_MW_ATV_8205_XLS
#define DBG_INIT_LEVEL_MW_ATV_8205_XLS   DBG_LEVEL_NONE
#endif

/* Compression library default debug level */
#ifndef DBG_INIT_LEVEL_MW_CL
#define DBG_INIT_LEVEL_MW_CL   DBG_LEVEL_NONE
#endif

/* Configuration Manager default debug level */
#ifndef DBG_INIT_LEVEL_MW_CFG
#define DBG_INIT_LEVEL_MW_CFG   DBG_LEVEL_NONE
#endif

/* Connection Manager default debug level */
#ifndef DBG_INIT_LEVEL_MW_CONN_MNGR
#define DBG_INIT_LEVEL_MW_CONN_MNGR   DBG_LEVEL_NONE
#endif

/* Date Time library default debug level */
#ifndef DBG_INIT_LEVEL_MW_DT
#define DBG_INIT_LEVEL_MW_DT   DBG_LEVEL_NONE
#endif

/* Event Context default debug level */
#ifndef DBG_INIT_LEVEL_MW_EVCTX
#define DBG_INIT_LEVEL_MW_EVCTX   DBG_LEVEL_NONE
#endif

/* File Manager default debug level */
#ifndef DBG_INIT_LEVEL_MW_FILE_MNGR
#define DBG_INIT_LEVEL_MW_FILE_MNGR   DBG_LEVEL_NONE
#endif

/* Favorite List Manager default debug level */
#ifndef DBG_INIT_LEVEL_MW_FLM
#define DBG_INIT_LEVEL_MW_FLM   DBG_LEVEL_NONE
#endif

/* Font library default debug level */
#ifndef DBG_INIT_LEVEL_MW_FONT
#define DBG_INIT_LEVEL_MW_FONT   DBG_LEVEL_NONE
#endif

/* Graphic library default debug level */
#ifndef DBG_INIT_LEVEL_MW_GRAPHIC
#define DBG_INIT_LEVEL_MW_GRAPHIC   DBG_LEVEL_NONE
#endif

/* HTML Parser default debug level */
#ifndef DBG_INIT_LEVEL_MW_HTML_PSR
#define DBG_INIT_LEVEL_MW_HTML_PSR   DBG_LEVEL_NONE
#endif

/* Image library Manager default debug level */
#ifndef DBG_INIT_LEVEL_MW_IMG
#define DBG_INIT_LEVEL_MW_IMG   DBG_LEVEL_NONE
#endif

/* IO Manager default debug level */
#ifndef DBG_INIT_LEVEL_MW_IO_MNGR
#define DBG_INIT_LEVEL_MW_IO_MNGR   DBG_LEVEL_NONE
#endif

/* OS default debug level */
#ifndef DBG_INIT_LEVEL_MW_OS
#define DBG_INIT_LEVEL_MW_OS   DBG_LEVEL_NONE
#endif

/* POD default debug level */
#ifndef DBG_INIT_LEVEL_MW_POD
#define DBG_INIT_LEVEL_MW_POD   DBG_LEVEL_NONE
#endif

/* Process Manager default debug level */
#ifndef DBG_INIT_LEVEL_MW_PRC_MNGR
#define DBG_INIT_LEVEL_MW_PRC_MNGR   DBG_LEVEL_NONE
#endif

/* Stream Manager default debug level */
#ifndef DBG_INIT_LEVEL_MW_STRM_MNGR
#define DBG_INIT_LEVEL_MW_STRM_MNGR   DBG_LEVEL_NONE
#endif

/* Service Context default debug level */
#ifndef DBG_INIT_LEVEL_MW_SVCTX
#define DBG_INIT_LEVEL_MW_SVCTX   DBG_LEVEL_NONE
#endif

/* SVL default debug level */
#ifndef DBG_INIT_LEVEL_MW_SVL
#define DBG_INIT_LEVEL_MW_SVL   DBG_LEVEL_NONE
#endif

/* SVL Builder default debug level */
#ifndef DBG_INIT_LEVEL_MW_SVL_BLDR
#define DBG_INIT_LEVEL_MW_SVL_BLDR   DBG_LEVEL_NONE
#endif

/* Table Manager default debug level */
#ifndef DBG_INIT_LEVEL_MW_TBL_MNGR
#define DBG_INIT_LEVEL_MW_TBL_MNGR   DBG_LEVEL_NONE
#endif

/* TSL default debug level */
#ifndef DBG_INIT_LEVEL_MW_TSL
#define DBG_INIT_LEVEL_MW_TSL   DBG_LEVEL_NONE
#endif

/* Widget Manager default debug level */
#ifndef DBG_INIT_LEVEL_MW_WGL_MNGR
#define DBG_INIT_LEVEL_MW_WGL_MNGR   DBG_LEVEL_NONE
#endif

/* Date Time library default debug level */
#ifndef DBG_INIT_LEVEL_MW_DATETIME_LIB
#define DBG_INIT_LEVEL_MW_DATETIME_LIB  DBG_LEVEL_NONE
#endif

#ifndef DBG_INIT_LEVEL_MW_SEC_MNGR
#define DBG_INIT_LEVEL_MW_SEC_MNGR  DBG_LEVEL_NONE
#endif

/* Broadcast handler default debug level */
#ifndef DBG_INIT_LEVEL_MW_BRDCST
#define DBG_INIT_LEVEL_MW_BRDCST  DBG_LEVEL_NONE
#endif

/* Analog Broadcast handler default debug level */
#ifndef DBG_INIT_LEVEL_MW_ANALOG_BRDCST
#define DBG_INIT_LEVEL_MW_ANALOG_BRDCST  DBG_LEVEL_NONE
#endif

/* Out-of-band handler default debug level */
#ifndef DBG_INIT_LEVEL_MW_OUTOFBAND
#define DBG_INIT_LEVEL_MW_OUTOFBAND  DBG_LEVEL_NONE
#endif

/* AVC connection handler default debug level */
#ifndef DBG_INIT_LEVEL_MW_AVC_HANDLER
#define DBG_INIT_LEVEL_MW_AVC_HANDLER  DBG_LEVEL_NONE
#endif

/* PSI Engine default debug level */
#ifndef DBG_INIT_LEVEL_MW_PSI
#define DBG_INIT_LEVEL_MW_PSI  DBG_LEVEL_NONE
#endif

/* Device Status Manager default debug level */
#ifndef DBG_INIT_LEVEL_MW_DSM
#define DBG_INIT_LEVEL_MW_DSM  DBG_LEVEL_NONE
#endif

/* Section Engine default debug level */
#ifndef DBG_INIT_LEVEL_MW_SECT
#define DBG_INIT_LEVEL_MW_SECT  DBG_LEVEL_NONE
#endif

/* Core Database module default debug level */
#ifndef DBG_INIT_LEVEL_MW_CDB
#define DBG_INIT_LEVEL_MW_CDB  DBG_LEVEL_NONE
#endif

#endif /* _DEF_DBG_LEVEL_MW_H_ */
