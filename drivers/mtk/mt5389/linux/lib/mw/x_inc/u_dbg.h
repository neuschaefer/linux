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
/*! @file u_dbg.h  
 * $Revision: #2 $
 * $Date: 2012/06/11 $
 * $Author: yingjoe.chen $
 *
 * @par Description:
 *         This header file contains debug related definitions, which are
 *         known to applications and middleware.
 *---------------------------------------------------------------------------*/

 
 
 
/*----------------------------------------------------------------------------*/

  
/*! @defgroup groupMW_DBG Debug Module Interface  
  *  @ingroup groupMW
  *  @see groupMW
  *  @{  
  */
/*----------------------------------------------------------------------------*/

#ifndef _U_DBG_H_
#define _U_DBG_H_


/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/


#ifdef __KERNEL__
#include "u_dbg_drv.h"
#include "u_common.h"
#include "x_dbg.h"
#else
#include "u_dbg_drv.h"
#include "u_common.h"
#include "x_dbg.h"
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#ifndef UNUSED
#define UNUSED(x)               (void)x         /**<The return value of routine is not cared*/
#endif

/* Retun values. */
#define DBGR_OK                 ((INT32)   0)   /**<Routine successful.*/
#define DBGR_OPEN_FAIL          ((INT32)  -1)   /**<The open operation failed for some reason.*/
#define DBGR_INV_ARG            ((INT32)  -2)   /**<The Input arguments are invalid.*/
#define DBGR_NOT_ENOUGH_MEM     ((INT32)  -3)   /**<Not enough memory to process the function*/
#define DBGR_ALREADY_INIT       ((INT32)  -4)   /**<Debug library already initialized.*/
#define DBGR_NOT_INIT           ((INT32)  -5)   /**<The DBG moudle hasn't be initilized.*/
#define DBGR_NO_TRACE_BUFFER    ((INT32)  -6)   /**<No trace buffer allocated.*/
#define DBGR_NO_OUTPUT_DEVICE   ((INT32)  -7)   /**<No output device available.*/
#define DBGR_INV_OUTPUT_DEVICE  ((INT32)  -8)   /**<The output device is invalid*/
#define DBGR_NOT_ENABLED        ((INT32)  -9)   /**<Debug library not compiled with define DEBUG.*/
#define DBGR_DUMP_IN_PROGRESS   ((INT32) -10)   /**<Trace buffer dump is in progress.*/
#define DBGR_REG_CB_ACTIVE      ((INT32) -11)   /**<A callback is still active.*/


/* The debug code is the concatenation of three values. The most significat   */
/* 12 bits contain the modul identification, the next 4 bits contain a        */
/* a category value and the least significat 16 bits contain a value, which   */
/* is defined by the module. Note that an abort code value of '0' is reserved */
/* for special usage. The following defines are the module definitions.       */
#define DBG_MOD_DELTA  ((UINT32) 0x00100000)                /**<The module identification delta definition*/

#define DBG_MOD_DEBUG       ((UINT32) ( 1 * DBG_MOD_DELTA)) /**<Module ID of Debug*/
#define DBG_MOD_HANDLE      ((UINT32) ( 2 * DBG_MOD_DELTA)) /**<Module ID of Handle*/
#define DBG_MOD_OS          ((UINT32) ( 3 * DBG_MOD_DELTA)) /**<Module ID of OS*/
#define DBG_MOD_RES_MNGR    ((UINT32) ( 4 * DBG_MOD_DELTA)) /**<Module ID of Resource Manager*/
#define DBG_MOD_CONN_MNGR   ((UINT32) ( 5 * DBG_MOD_DELTA)) /**<Module ID of Connect Manager*/
#define DBG_MOD_STRM_MNGR   ((UINT32) ( 6 * DBG_MOD_DELTA)) /**<Module ID of Stream Manager*/
#define DBG_MOD_TBL_MNGR    ((UINT32) ( 7 * DBG_MOD_DELTA)) /**<Module ID of Table Manager*/
#define DBG_MOD_RWLOCK      ((UINT32) ( 8 * DBG_MOD_DELTA)) /**<Module ID of Read/Write Lock*/
#define DBG_MOD_START       ((UINT32) ( 9 * DBG_MOD_DELTA)) /**<Module ID of Start*/
#define DBG_MOD_CDB         ((UINT32) (10 * DBG_MOD_DELTA)) /**<Module ID of Core Database*/
#define DBG_MOD_SCDB        ((UINT32) (11 * DBG_MOD_DELTA)) /**<Module ID of SCDB*/
#define DBG_MOD_GRAPHIC     ((UINT32) (12 * DBG_MOD_DELTA)) /**<Module ID of Graphics*/
#define DBG_MOD_PRC_MNGR    ((UINT32) (13 * DBG_MOD_DELTA)) /**<Module ID of Process Manager*/
#define DBG_MOD_SVL         ((UINT32) (14 * DBG_MOD_DELTA)) /**<Module ID of SVL*/
#define DBG_MOD_TSL         ((UINT32) (15 * DBG_MOD_DELTA)) /**<Module ID of TSL*/
#define DBG_MOD_SET_JMP     ((UINT32) (16 * DBG_MOD_DELTA)) /**<Module ID of Set Jump*/
#define DBG_MOD_AEE         ((UINT32) (17 * DBG_MOD_DELTA)) /**<Module ID of AEE*/
#define DBG_MOD_BRDCST      ((UINT32) (18 * DBG_MOD_DELTA)) /**<Module ID of Broadcast*/
#define DBG_MOD_SVL_BLDR    ((UINT32) (19 * DBG_MOD_DELTA)) /**<Module ID of Svl Builder*/
#define DBG_MOD_EVTCTX      ((UINT32) (20 * DBG_MOD_DELTA)) /**<Module ID of Event Context*/
#define DBG_MOD_IO_MNGR     ((UINT32) (21 * DBG_MOD_DELTA)) /**<Module ID of IO Manager*/
#define DBG_MOD_FLM         ((UINT32) (22 * DBG_MOD_DELTA)) /**<Module ID of FLM*/
#define DBG_MOD_CL          ((UINT32) (23 * DBG_MOD_DELTA)) /**<Module ID of Compress Library*/
#define DBG_MOD_ATV_MNGR    ((UINT32) (24 * DBG_MOD_DELTA)) /**<Module ID of Atv Manager*/
#define DBG_MOD_CFG         ((UINT32) (25 * DBG_MOD_DELTA)) /**<Module ID of Config*/
#define DBG_MOD_FONT        ((UINT32) (26 * DBG_MOD_DELTA)) /**<Module ID of Font*/
#define DBG_MOD_UTIL        ((UINT32) (27 * DBG_MOD_DELTA)) /**<Module ID of Util*/
#define DBG_MOD_PSI_ENG     ((UINT32) (28 * DBG_MOD_DELTA)) /**<Module ID of Psi Engine*/
#define DBG_MOD_IMG         ((UINT32) (29 * DBG_MOD_DELTA)) /**<Module ID of Image*/
#define DBG_MOD_CLI         ((UINT32) (30 * DBG_MOD_DELTA)) /**<Module ID of Cli*/
#define DBG_MOD_SEC_MNGR    ((UINT32) (31 * DBG_MOD_DELTA)) /**<Module ID of Section Manager*/
#define DBG_MOD_POD         ((UINT32) (32 * DBG_MOD_DELTA)) /**<Module ID of POD*/
#define DBG_MOD_SVCTX       ((UINT32) (33 * DBG_MOD_DELTA)) /**<Module ID of Svctx*/
#define DBG_MOD_DATE_TIME   ((UINT32) (34 * DBG_MOD_DELTA)) /**<Module ID of Date Time*/
#define DBG_MOD_OUTOFBAND   ((UINT32) (35 * DBG_MOD_DELTA)) /**<Module ID of out of band*/
#define DBG_MOD_1394_STACK  ((UINT32) (36 * DBG_MOD_DELTA)) /**<Module ID of 1394 Stack*/
#define DBG_MOD_DSM         ((UINT32) (37 * DBG_MOD_DELTA)) /**<Module ID of DSM*/
#define DBG_MOD_ABRDCST     ((UINT32) (38 * DBG_MOD_DELTA)) /**<Module ID of Abrdcst*/
#define DBG_MOD_AVC_HANDLER ((UINT32) (39 * DBG_MOD_DELTA)) /**<Module ID of AVC Handler*/
#define DBG_MOD_SECT_ENG    ((UINT32) (40 * DBG_MOD_DELTA)) /**<Module ID of Section Engine */
#define DBG_MOD_STRM_SBTL   ((UINT32) (41 * DBG_MOD_DELTA)) /**<Module ID of Stream Manager Subtitle*/
#define DBG_MOD_DSMCC_ENG   ((UINT32) (42 * DBG_MOD_DELTA)) /**<Module ID of DSMCC Engine*/
#define DBG_MOD_MHP_ENG     ((UINT32) (43 * DBG_MOD_DELTA)) /**<Module ID of MHP Engine*/
#define DBG_MOD_RRCTX       ((UINT32) (44 * DBG_MOD_DELTA)) /**<Module ID of RRCTX*/
#define DBG_MOD_WGL         ((UINT32) (45 * DBG_MOD_DELTA)) /**<Module ID of Widget*/
#define DBG_MOD_STRM_TTX    ((UINT32) (46 * DBG_MOD_DELTA)) /**<Module ID of Stream Manager Teletext*/
#define DBG_MOD_SOCK        ((UINT32) (47 * DBG_MOD_DELTA)) /**<Module ID of Sock*/
#define DBG_MOD_MHEG_5      ((UINT32) (48 * DBG_MOD_DELTA)) /**<Module ID of MHEG5*/
#define DBG_MOD_PLAYBACK    ((UINT32) (49 * DBG_MOD_DELTA)) /**<Module ID of Playback*/
#define DBG_MOD_MSVCTX      ((UINT32) (50 * DBG_MOD_DELTA)) /**<Module ID of Multi Svctx*/
#define DBG_MOD_CEC_MNGR    ((UINT32) (51 * DBG_MOD_DELTA)) /**<Module ID of CEC Manager*/
#define DBG_MOD_PCL         ((UINT32) (52 * DBG_MOD_DELTA)) /**<Module ID of PCL*/
#define DBG_MOD_DSMCC_OC    ((UINT32) (53 * DBG_MOD_DELTA)) /**<Module ID of DSMCC OC*/
#define DBG_MOD_DSMCC_DB    ((UINT32) (54 * DBG_MOD_DELTA)) /**<Module ID of DSMCC DB*/
#define DBG_MOD_FM          ((UINT32) (55 * DBG_MOD_DELTA)) /**<Module ID of File Manager*/
#define DBG_MOD_DM          ((UINT32) (56 * DBG_MOD_DELTA)) /**<Module ID of DM*/
#define DBG_MOD_DFM         ((UINT32) (57 * DBG_MOD_DELTA)) /**<Module ID of DFM*/
#define DBG_MOD_VBI_FLTR    ((UINT32) (58 * DBG_MOD_DELTA)) /**<Module ID of Vbi Filter*/
#define DBG_MOD_REC_UTIL    ((UINT32) (59 * DBG_MOD_DELTA)) /**<Module ID of Rec Util*/
#define DBG_MOD_CI          ((UINT32) (60 * DBG_MOD_DELTA)) /**<Module ID of CI*/
#define DBG_MOD_DMX_UTIL    ((UINT32) (61 * DBG_MOD_DELTA)) /**<Module ID of Dmx Util*/
#define DBG_MOD_EDB         ((UINT32) (62 * DBG_MOD_DELTA)) /**<Module ID of EDB*/
#define DBG_MOD_GINGA       ((UINT32) (63 * DBG_MOD_DELTA)) /**<Module ID of Ginga*/
#define DBG_MOD_HBBTV       ((UINT32) (64 * DBG_MOD_DELTA)) /**<Module ID of HBBTV*/

#if 1 /* Network */
/**
 * Network module
 */
#define DBG_MOD_ACCESS_IP   ((UINT32) (90 * DBG_MOD_DELTA)) /**<Module ID of Access IP*/
#define DBG_MOD_ACCESS_DLNA ((UINT32) (91 * DBG_MOD_DELTA)) /**<Module ID of Access Dlna*/
#define DBG_MOD_EXPAT_XML   ((UINT32) (92 * DBG_MOD_DELTA)) /**<Module ID of Expat XML*/
#endif

/* For applications, we reserve the module range 0xf0000000 to 0xff000000 (exclusive).  */
/* Note that individual applications must define their own individual module value. For */
/* example:                                                                             */
/*   #define DBG_MOD_NAVIGATOR  DBG_MOD_APPL + (1 * DBG_MOD_DELTA)                      */
/*   #define DBG_MOD_EPG        DBG_MOD_APPL + (2 * DBG_MOD_DELTA)                      */
#define DBG_MOD_APPL       ((UINT32) 0xf0000000)            /**<Module ID of Application*/

/* The following defines are used for special modules or init functions. */
#define DBG_MOD_UT           ((UINT32) 0xffb00000)          /**<Module ID of UT*/
#define DBG_MOD_CUSTOM_INIT  ((UINT32) 0xffc00000)          /**<Module ID of Custom Init*/
#define DBG_MOD_APPL_INIT    ((UINT32) 0xffd00000)          /**<Module ID of Application Init*/
#define DBG_MOD_BOARD        ((UINT32) 0xffe00000)          /**<Module ID of Board*/
#define DBG_MOD_QA           ((UINT32) 0xfff00000)          /**<Module ID of QA*/

/* The following defines are the category definitions. */
#define DBG_CAT_MEMORY          ((UINT32) 0x00010000)       /**<Debug category of memory*/
#define DBG_CAT_INV_OP          ((UINT32) 0x00020000)       /**<Debug category of op*/
#define DBG_CAT_SEMAPHORE       ((UINT32) 0x00030000)       /**<Debug category of semaphore*/
#define DBG_CAT_THREAD          ((UINT32) 0x00040000)       /**<Debug category of thread*/
#define DBG_CAT_MESSAGE         ((UINT32) 0x00050000)       /**<Debug category of message*/
#define DBG_CAT_HANDLE          ((UINT32) 0x00060000)       /**<Debug category of handle*/
#define DBG_CAT_TIMER           ((UINT32) 0x00070000)       /**<Debug category of timer*/
#define DBG_CAT_CB_RETURN_VAL   ((UINT32) 0x00080000)       /**<Debug category of callback return value*/
#define DBG_CAT_DRV_RETURN_VAL  ((UINT32) 0x00090000)       /**<Debug category of driver return value*/
#define DBG_CAT_INIT            ((UINT32) 0x000a0000)       /**<Debug category of init*/
#define DBG_CAT_IO              ((UINT32) 0x000b0000)       /**<Debug category of IO*/
#define DBG_CAT_NO_RECOVERY     ((UINT32) 0x000c0000)       /**<Debug category of no recovery*/
#define DBG_CAT_TRACE_REC       ((UINT32) 0x000f0000)       /**<Debug category of trace record*/


/* Macro for debug abort / assert. */
#ifdef __KERNEL__
#define DBG_ABORT(_code)  x_do_dbg_abort (__FUNCTION__, ((UINT32) __LINE__), _code)      
#else
#define DBG_ABORT(_code)  {x_do_dbg_abort (__FILE__, ((UINT32) __LINE__), _code);}      /**<        */
#endif
/**<Definition prototype of abort*/


#define DBG_ASSERT(_expr, _code)  { if (! (_expr)) DBG_ABORT (_code); } /**<Definition prototype of assert*/


#define dbg_abort(_code)  DBG_ABORT (_code) /**<Definition function prototype of abort*/                 

/* Debug level defines. */
#define DBG_LEVEL_NONE   ((UINT16) 0x0000)  /**<Definition of debug level  NONE*/
#define DBG_LEVEL_ALL    ((UINT16) 0xffff)  /**<Definition of debug level  ALL*/

/* Common macros to perform CLI controlled debug statements. */
/* Note that an individual SW Module MUST set the macro      */
/* DBG_LEVE_MODULE else a compile error will occur.          */
#undef DBG_ERROR
#undef DBG_API
#undef DBG_INFO
/*
#define DBG_LEVEL_ERROR  ((UINT16) 0x0001)
#define DBG_LEVEL_API    ((UINT16) 0x0002)
#define DBG_LEVEL_INFO   ((UINT16) 0x0004)
*/

#define CLI_LEVEL_RELEASE  1            /**<Definition of debug level  RELEASE*/
#define CLI_LEVEL_CLI      2            /**<Definition of debug level  CLI*/
#define CLI_LEVEL_DEBUG    4            /**<Definition of debug level  DEBUG*/

#if defined(DEBUG)
#define CLI_CMD_LEVEL 7                 
#elif defined(CLI)
#define CLI_CMD_LEVEL 3                 
#else
#define CLI_CMD_LEVEL 1                 
#endif

#ifdef CLI_SUPPORT
    #if defined(DEBUG)
        #ifndef CLI_LVL_ALL
            #define CLI_LVL_ALL
        #endif
        #ifndef CLI_LVL_GUEST
            #define CLI_LVL_GUEST
        #endif
    #else
        #ifndef CLI_LVL_GUEST
            #define CLI_LVL_GUEST
        #endif
    #endif
#endif

/**<Definition of cli command level*/


#ifndef CLI_ERROR_LVL
#define CLI_ERROR_LVL 0                 /**<Definition of cli error level*/
#endif

#ifndef CLI_API_LVL
#define CLI_API_LVL 6                  /**<Definition of cli api level*/
#endif

#ifndef CLI_INFO_LVL
#define CLI_INFO_LVL 2                  /**<Definition of cli info level*/
#endif

#ifdef DEBUG
#define DBG_LEVEL_ERROR  ((UINT16) (1<<(CLI_ERROR_LVL)))        /**<Definition of debug level ERROR*/
#define DBG_LEVEL_API    ((UINT16) (1<<(CLI_API_LVL)))          /**<Definition of debug level  API*/
#define DBG_LEVEL_INFO   ((UINT16) (1<<(CLI_INFO_LVL)))         /**<Definition of debug level  INFO*/

#define DBG_ERROR(_stmt)  DBG_LEVEL_STMT (DBG_LEVEL_MODULE, DBG_LEVEL_ERROR, _stmt) 
/**<Definition prototype of DBG_ERROR*/
#define DBG_API(_stmt)    DBG_LEVEL_STMT (DBG_LEVEL_MODULE, DBG_LEVEL_API,   _stmt) 
/**<Definition prototype of DBG_API*/
#define DBG_INFO(_stmt)   DBG_LEVEL_STMT (DBG_LEVEL_MODULE, DBG_LEVEL_INFO,  _stmt) 
/**<Definition prototype of DBG_INFO*/
#define DBG_LOG_MW(level, _stmt)   DBG_LEVEL_STMT (DBG_LEVEL_MODULE, level,  _stmt) 
/**<Definition prototype of DBG_LOG_MW*/

#define DBG_LOG_MW_1(_stmt)		DBG_LOG_MW((1<<0), _stmt)
#define DBG_LOG_MW_2(_stmt)		DBG_LOG_MW((1<<1), _stmt)
#define DBG_LOG_MW_3(_stmt)		DBG_LOG_MW((1<<2), _stmt)
#define DBG_LOG_MW_4(_stmt)		DBG_LOG_MW((1<<3), _stmt)
#define DBG_LOG_MW_5(_stmt)		DBG_LOG_MW((1<<4), _stmt)
#define DBG_LOG_MW_6(_stmt)		DBG_LOG_MW((1<<5), _stmt)
#define DBG_LOG_MW_7(_stmt)		DBG_LOG_MW((1<<6), _stmt)



#else

#ifndef CLI_LOG_LVL
#define CLI_LOG_LVL 0       /**<Definition of cli log level*/
#endif

#define DBG_LOG_MW(level, _stmt)   DBG_LOG_MW_##level(_stmt)    
/**<Definition prototype of DBG_LOG_MW*/


#define DBG_LEVEL_ERROR  ((UINT16) (1<<(CLI_ERROR_LVL)))    /**<Definition of debug level ERROR*/
#define DBG_LEVEL_API    ((UINT16) (1<<(CLI_API_LVL)))      /**<Definition of debug level  API*/
#define DBG_LEVEL_INFO   ((UINT16) (1<<(CLI_INFO_LVL)))     /**<Definition of debug level  INFO*/

#if (CLI_LOG_LVL>CLI_ERROR_LVL)
#define DBG_ERROR(_stmt)  DBG_LEVEL_STMT (DBG_LEVEL_MODULE, DBG_LEVEL_ERROR, _stmt)
#else
#define DBG_ERROR(_stmt)
#endif
/**<Definition prototype of DBG_ERROR*/


#if (CLI_LOG_LVL>CLI_API_LVL)
#define DBG_API(_stmt)    DBG_LEVEL_STMT (DBG_LEVEL_MODULE, DBG_LEVEL_API,   _stmt)
#else
#define DBG_API(_stmt)
#endif
/**<Definition prototype of DBG_API*/


#if (CLI_LOG_LVL>CLI_INFO_LVL)
#define DBG_INFO(_stmt)   DBG_LEVEL_STMT (DBG_LEVEL_MODULE, DBG_LEVEL_INFO,  _stmt)
#else
#define DBG_INFO(_stmt)
#endif
/**<Definition prototype of DBG_INFO*/



#if (CLI_LOG_LVL>0)
#define DBG_LOG_MW_0(_stmt)            DBG_LEVEL_STMT (DBG_LEVEL_MODULE, (1<<0),  _stmt)
#else
#define DBG_LOG_MW_0(_stmt)            {}
#endif
/**<Definition prototype of DBG_LOG_MW_0*/



#if (CLI_LOG_LVL>1)
#define DBG_LOG_MW_1(_stmt)            DBG_LEVEL_STMT (DBG_LEVEL_MODULE, (1<<1),  _stmt)
#else
#define DBG_LOG_MW_1(_stmt)            {}
#endif
/**<Definition prototype of DBG_LOG_MW_1*/

#if (CLI_LOG_LVL>2)
#define DBG_LOG_MW_2(_stmt)            DBG_LEVEL_STMT (DBG_LEVEL_MODULE, (1<<2),  _stmt)
#else
#define DBG_LOG_MW_2(_stmt)            {}
#endif
/**<Definition prototype of DBG_LOG_MW_2*/

#if (CLI_LOG_LVL>3)
#define DBG_LOG_MW_3(_stmt)            DBG_LEVEL_STMT (DBG_LEVEL_MODULE, (1<<3),  _stmt)
#else
#define DBG_LOG_MW_3(_stmt)            {}
#endif
/**<Definition prototype of DBG_LOG_MW_3*/


#if (CLI_LOG_LVL>4)
#define DBG_LOG_MW_4(_stmt)            DBG_LEVEL_STMT (DBG_LEVEL_MODULE, (1<<4),  _stmt)
#else
#define DBG_LOG_MW_4(_stmt)            {}
#endif
/**<Definition prototype of DBG_LOG_MW_4*/

#if (CLI_LOG_LVL>5)
#define DBG_LOG_MW_5(_stmt)            DBG_LEVEL_STMT (DBG_LEVEL_MODULE, (1<<5),  _stmt)
#else
#define DBG_LOG_MW_5(_stmt)            {}
#endif
/**<Definition prototype of DBG_LOG_MW_5*/

#if (CLI_LOG_LVL>6)
#define DBG_LOG_MW_6(_stmt)            DBG_LEVEL_STMT (DBG_LEVEL_MODULE, (1<<6),  _stmt)
#else
#define DBG_LOG_MW_6(_stmt)            {}
#endif
/**<Definition prototype of DBG_LOG_MW_6*/


#if (CLI_LOG_LVL>7)
#define DBG_LOG_MW_7(_stmt)            DBG_LEVEL_STMT (DBG_LEVEL_MODULE, (1<<7),  _stmt)
#else
#define DBG_LOG_MW_7(_stmt)            {}
#endif
/**<Definition prototype of DBG_LOG_MW_7*/


#if (CLI_LOG_LVL>8)
#define DBG_LOG_MW_8(_stmt)            DBG_LEVEL_STMT (DBG_LEVEL_MODULE, (1<<8),  _stmt)
#else
#define DBG_LOG_MW_8(_stmt)            {}
#endif
/**<Definition prototype of DBG_LOG_MW_8*/


#if (CLI_LOG_LVL>9)
#define DBG_LOG_MW_9(_stmt)            DBG_LEVEL_STMT (DBG_LEVEL_MODULE, (1<<9),  _stmt)
#else
#define DBG_LOG_MW_9(_stmt)            {}
#endif
/**<Definition prototype of DBG_LOG_MW_9*/


#endif

#endif /* _U_DBG_H_ */


/*----------------------------------------------------------------------------*/
/*! @} */                                                 /* end of groupMW_DBG*/
/*----------------------------------------------------------------------------*/


