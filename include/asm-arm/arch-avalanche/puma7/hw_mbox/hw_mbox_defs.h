/*
 *
 * hw_mbox_defs.h
 * Description:
 * contains basic definitions puma7 HW_MBOX controller SW.
 *

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2014-2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution
  in the file called LICENSE.GPL.


  Contact Information:
  Intel Corporation
  2200 Mission College Blvd.
  Santa Clara, CA  97052

  BSD LICENSE

  Copyright(c) 2014-2016 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.

    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef _HW_MBOX_DEFS_H_
#define _HW_MBOX_DEFS_H_

/******************************************
|                                         |
|        HW_MBOX Basic Definitions        |
|                                         |
******************************************/

/*! \def HW_MBOX_MASTER_COUNT
    \brief Count of HW_MBOX Masters. */
#define HW_MBOX_MASTER_COUNT     16

/*! \def HW_MBOX_CHANNEL_COUNT
    \brief Count of HW_MBOX Channel. */
#define HW_MBOX_CHANNEL_COUNT    64

#define HW_MBOX_CID_INVALID    0xFFFFFFFF

/*! \def HW_MBOX_MID_VALIDITY_CHECK(paramMID)
    \brief evaluates as True if paramMID is a valid MID. */
#define HW_MBOX_MID_VALIDITY_CHECK(paramMID)    (((paramMID) >= 0) && ((paramMID) < HW_MBOX_MASTER_COUNT))

/*! \def HW_MBOX_CID_VALIDITY_CHECK(paramCID)
    \brief evaluates as True if paramCID is a valid CID. */
#define HW_MBOX_CID_VALIDITY_CHECK(paramCID)    (((paramCID) >= 0) && ((paramCID) < HW_MBOX_CHANNEL_COUNT))

/* HW_MBOX_MASTER_MID defines: */
/* Note: should be kept in sync with HW_MBOX_IS_MASTER defines and with HW_MBOX_MASTER_TABLE */
#define HW_MBOX_NP_CPU_MID          0  /*! MID of NP_CPU        */
#define HW_MBOX_BBU_C_MID           1  /*! MID of BBU_C         */
#define HW_MBOX_APP_CPU_MID         2  /*! MID of APP_CPU       */
#define HW_MBOX_RCE_MID             3  /*! MID of RCE           */
#define HW_MBOX_US_MAC_CPU_MID      4  /*! MID of US_MAC_CPU    */
#define HW_MBOX_PHY_CPU_MID         5  /*! MID of PHY_CPU       */
#define HW_MBOX_MOCA_C_MID          6  /*! MID of MOCA_C        */
#define HW_MBOX_DS_MAC_C_MID        7  /*! MID of DS_MAC_C      */
#define HW_MBOX_PP_NETSEC_MID       8  /*! MID of PP_NETSEC     */
#define HW_MBOX_PP_RESEQ_MID        9  /*! MID of PP_RESEQ      */
#define HW_MBOX_PP_STAGE1_MID      10  /*! MID of PP_STAGE1     */
#define HW_MBOX_PP_QOS_MID         11  /*! MID of PP_QOS        */
#define HW_MBOX_C55_MID            12  /*! MID of C55           */
#define HW_MBOX_FREE0_MID          13  /*! MID of FREE0         */
#define HW_MBOX_SEC_MID            14  /*! MID of SEC           */
#define HW_MBOX_CSE_MID            15  /*! MID of CSE           */

/*! \def HW_MBOX_DEFAULT_TIMEOUT_SEC
    \brief Default timeout value in seconds.
    \note can comment out define to set wait forever.
    \attention wait forever setting should only be used for debug!
*/
#define HW_MBOX_DEFAULT_TIMEOUT_SEC    10  /* 10 sec */

/* Error Notification: */

/* supervisor gets notified of access violation errors through a HW_MBOX interrupt.
   To signal that there's an access violation error a Certain predetermined CID is
   selected. This CID should not be allocaed to supervisor as a channel. */

/*! \def HW_MBOX_ERROR_NOTIFY_CID_FOR_SUPERVISOR
    \brief This CID gets signaled upon HW access violation errors.
    \attention should only be used by supervisor.
*/
#define HW_MBOX_ERROR_NOTIFY_CID_FOR_SUPERVISOR    63

/* Some General Macros we will use throughout the Code: */

#define HW_MBOX_CONCAT2(a, b)          a ## b
#define HW_MBOX_CONCAT3(a, b, c)       a ## b ## c

#define HW_MBOX_STRINGIFY(x)    #x

#ifndef BIT
#define BIT( _i )       (1UL << ( _i ))
#endif

/**********************************************
|                                             |
|        HW_MBOX Package Configuration        |
|                                             |
**********************************************/

/*! \def HW_MBOX_MONITOR    <MID_of_Monitor>
    \brief MID of the Master to act as Monitor.
    \attention use a value from HW_MBOX_MASTER_MID defines.
    \note Feature is OPTIONAL, comment out define to disable.
    \note Monitor gets an interrupt upon every HW_MBOX interrupt of any channel and should log the commands.
    \note There's no lock on the command register value, this Monitoring feature is best effort only!
*/
//#define HW_MBOX_MONITOR    HW_MBOX_NP_CPU_MID

/** --------------------------------------- **/

/*! \def HW_MBOX_START_IN_HW_DEBUG_MODE
    \brief Start HW_MBOX in HW Debug Mode.
    \attention In HW Debug Mode access protection rules are not enforced and every Master access is permitted.
    \note Feature is OPTIONAL, comment out define to disable.
*/
//#define HW_MBOX_START_IN_HW_DEBUG_MODE

/** --------------------------------------- **/

/*! \def __HW_MBOX_CHANGE_DEFAULT_SUPERVISOR_TO    <MID_of_new_Supervisor>
    \brief MID of the Master to act as supervisor (instead of default one).
    \warning ONLY USE IF YOU ABSOULTLY KNOW WHAT YOU ARE DOING!!!
    \attention use a value from HW_MBOX_MASTER_MID defines.
    \note Feature is OPTIONAL, comment out define to disable.
    \note This will change the default supervisor's MID (MID == 0) to the new value indicated here.
    \note There has to be a special way to change it since only the supervisor can access the ctrl Register.
*/
//#define __HW_MBOX_CHANGE_DEFAULT_SUPERVISOR_TO    HW_MBOX_NP_CPU_MID

/** --------------------------------------- **/

/*! \def HW_MBOX_DEBUG
    \brief Enable this flag for various debug operations of the HW_MBOX package.
    \note Feature is OPTIONAL, comment out define to disable.
    \note Debug Features:
    \note 1. Free access to the HW_MBOX registers and Memory via CLI.
    \note 2. MID Disguise: enable the current Master to access the HW_MBOX "disguised" as a different Master.
*/
//#define HW_MBOX_DEBUG

/** --------------------------------------- **/

/*! \def HW_MBOX_SOCKETS_API
    \brief Enable AF_HWMBOX socket family support.
*/
#define HW_MBOX_SOCKETS_API

/** --------------------------------------- **/

/*! \def HW_MBOX_EXTENDED_MEMORY
    \brief Enable usage of extended memory.
*/
#define HW_MBOX_EXTENDED_MEMORY

/******************************************************
|                                                     |
|        HW_MBOX Specific Master Configuration        |
|                                                     |
******************************************************/

/*! \def HW_MBOX_MID    <MID_of_this_Master>
    \brief MID of this Master, current Master, the one running this code.
    \attention use a value from HW_MBOX_MASTER_MID defines.
    \note It is derived from which physical HW_MBOX interrupt line is connected to this Master.
    \note Values for all Master are enumerated by hw_mbox_Masters_e, just pick the correct one.
*/
#define HW_MBOX_MID    HW_MBOX_NP_CPU_MID

/*! \def HW_MBOX_OS_LINUX\HW_MBOX_OS_FLAT\HW_MBOX_OS_NONE
    \brief Set one appropriate define according to Variant (OS).
    \attention Enable Exactly one of these!
    \note HW_MBOX_OS_LINUX: U-services.
    \note HW_MBOX_OS_FLAT:  F-services.
    \note HW_MBOX_OS_NONE:  N-services.
*/
#define HW_MBOX_OS_LINUX     /* U-services */
//#define HW_MBOX_OS_FLAT    /* F-services */
//#define HW_MBOX_OS_NONE    /* N-services */

/* Variant Selection Check: */
#if ((defined(HW_MBOX_OS_LINUX) + defined(HW_MBOX_OS_FLAT) + defined(HW_MBOX_OS_NONE)) != 1)
#   error "HW_MBOX defs: Enable ONE OS Variant exactly!"
#endif

/*! \var HW_MBOX_HARDCODED_MID
    \brief Remember the HARDCODED MID of this Master.
    \attention This is needed as under debug "MID Disguise Feature" undefs HW_MBOX_MID and redefines it as a dynamic field.
    \note using enum to qualify as CONST.
*/
enum { HW_MBOX_HARDCODED_MID = HW_MBOX_MID };

/* HW_MBOX_IS_MASTER defines (useful for #ifdef-ing include for instance): */
/* Note: These should be kept in sync with HW_MBOX_MASTER_MID defines and with HW_MBOX_MASTER_TABLE  */
#if (HW_MBOX_MID == HW_MBOX_NP_CPU_MID)
#define HW_MBOX_IS_NP_CPU               /*! defined if current HW_MBOX Master is NP_CPU     */
#elif (HW_MBOX_MID == HW_MBOX_BBU_C_MID)
#define HW_MBOX_IS_BBU_C                /*! defined if current HW_MBOX Master is BBU_C      */
#elif (HW_MBOX_MID == HW_MBOX_APP_CPU_MID)
#define HW_MBOX_IS_APP_CPU              /*! defined if current HW_MBOX Master is APP_CPU    */
#elif (HW_MBOX_MID == HW_MBOX_RCE_MID)
#define HW_MBOX_IS_RCE                  /*! defined if current HW_MBOX Master is RCE        */
#elif (HW_MBOX_MID == HW_MBOX_US_MAC_CPU_MID)
#define HW_MBOX_IS_US_MAC_CPU           /*! defined if current HW_MBOX Master is US_MAC_CPU */
#elif (HW_MBOX_MID == HW_MBOX_PHY_CPU_MID)
#define HW_MBOX_IS_PHY_CPU              /*! defined if current HW_MBOX Master is PHY_CPU    */
#elif (HW_MBOX_MID == HW_MBOX_MOCA_C_MID)
#define HW_MBOX_IS_MOCA_C               /*! defined if current HW_MBOX Master is MOCA_C     */
#endif

/**********************************************
|                                             |
|        HW_MBOX Types and Definitions        |
|                                             |
**********************************************/

/* Generic Primitive typedefs: */

/* need typedefs of Uint32, Uint8 and Bool(True/False). */
#if   defined HW_MBOX_IS_NP_CPU
#ifdef __KERNEL__
#include "_tistdtypes.h"
#else
#include "sys_types.h"
#endif /* __KERNEL__ */
#elif defined HW_MBOX_IS_APP_CPU
#include "hwMbox_types.h"
#elif defined HW_MBOX_IS_PHY_CPU
#include "types.h"
#endif

/* STATUS (Return Codes): */

/*! \def HW_MBOX_STATUS_TABLE( __ENTRY )
    \brief X-macro table definition of the HW_MBOX_STATUS enum.
    \param[in] __ENTRY - X-pander.
*/
#define HW_MBOX_STATUS_TABLE( __ENTRY ) \
/* +------------------------------------------------------+ */ \
/* |        HW_MBOX_STATUS_e name                         | */ \
/* |                                                      | */ \
/* |                                                      | */ \
/* +------------------------------------------------------+ */ \
    __ENTRY(HW_MBOX_STATUS_OK                             ) \
    __ENTRY(HW_MBOX_STATUS_NOK                            ) \
    __ENTRY(HW_MBOX_STATUS_INTERNAL_ERROR                 ) \
    __ENTRY(HW_MBOX_STATUS_DRIVER_NOT_INITIALIZED         ) \
    __ENTRY(HW_MBOX_STATUS_PEER_NOT_INITIALIZED           ) \
    __ENTRY(HW_MBOX_STATUS_PEER_NOT_VALID                 ) \
    __ENTRY(HW_MBOX_STATUS_NO_CHANNEL_FOUND               ) \
    __ENTRY(HW_MBOX_STATUS_INTERRUPTED_BY_SIGNAL          ) \
    __ENTRY(HW_MBOX_STATUS_ERROR                          ) \
    __ENTRY(HW_MBOX_STATUS_ERROR_TIMEOUT_EXPIRED          ) \
    __ENTRY(HW_MBOX_STATUS_OPCODE_ERROR                   ) \
    __ENTRY(HW_MBOX_STATUS_OPCODE_ERROR_INVALID_TAG       ) \
    __ENTRY(HW_MBOX_STATUS_OPCODE_ERROR_MESSAGE_TOO_LONG  ) \
    __ENTRY(HW_MBOX_STATUS_OPCODE_ERROR_INVALID_MESSAGE   ) \
    __ENTRY(HW_MBOX_STATUS_OPCODE_ERROR_ILLEGAL_MESSAGE   ) \
    __ENTRY(HW_MBOX_STATUS_OPCODE_ERROR_CALLBACK_FAILED   )

#define HW_MBOX_STATUS_TABLE_AS_ENUM( _enumName )       _enumName,
#define HW_MBOX_STATUS_TABLE_AS_STRS( _enumName )       HW_MBOX_STRINGIFY( _enumName ),
#define HW_MBOX_STATUS_TABLE_AS_COUNT( _enumName )      1+

#define HW_MBOX_STATUS_TABLE_COUNT    ( HW_MBOX_STATUS_TABLE(HW_MBOX_STATUS_TABLE_AS_COUNT) 0 )

/*! \enum HW_MBOX_STATUS_e
    \brief HW MBOX return codes.
*/
typedef enum HW_MBOX_STATUS
{
    HW_MBOX_STATUS_TABLE(HW_MBOX_STATUS_TABLE_AS_ENUM)

    /* Don't add any values after this line!!! */

    HW_MBOX_STATUS_ONE_AFTER_LAST,
    HW_MBOX_STATUS_LAST = (HW_MBOX_STATUS_ONE_AFTER_LAST - 1),

    HW_MBOX_STATUS_COUNT = HW_MBOX_STATUS_TABLE_COUNT
} HW_MBOX_STATUS_e;

/*! \def HW_MBOX_STATUS_STRS
    \brief Creates an string array with HW MBOX STATUS.
    \attention only use inside c file.
*/
#define HW_MBOX_STATUS_STRS( _arrayName ) \
    static const Char * _arrayName[] =                     \
    {                                                      \
        HW_MBOX_STATUS_TABLE(HW_MBOX_STATUS_TABLE_AS_STRS) \
        NULL                                               \
    }

/********************************************
|                                           |
|        HW_MBOX Master Declarations        |
|                                           |
********************************************/

/* Master Related Types and Defines: */

/*! \enum hw_mbox_support_e
    \brief This enum denotes which SW support exists on each Master. */
typedef enum hw_mbox_support
{
    hw_mbox_support_none = 0, /* No SW support on this Master */
    hw_mbox_support_static,   /* only supports static channel allocation */
    hw_mbox_support_dynamic   /* supports static and Initialization protocol */
} hw_mbox_support_e;

/*! \def HW_MBOX_SUPPORT_STR(mboxSupport)
    \brief gets a string according to <mboxSupport>
    \attention <mboxSupport> should be a value from hw_mbox_support_e
*/
#define HW_MBOX_SUPPORT_STR(mboxSupport)    ( (mboxSupport) == hw_mbox_support_none    ) ? ("None"   ) : \
                                            ( (mboxSupport) == hw_mbox_support_static  ) ? ("Static" ) : \
                                            ( (mboxSupport) == hw_mbox_support_dynamic ) ? ("Dynamic") : \
                                              ("__ERROR_UNKNOWN_HW_MBOX_SUPPORT__")

/*! \struct hw_mbox_masterConfig_t
    \brief Master configuration struct. */
typedef struct hw_mbox_masterConfig
{
    hw_mbox_support_e   support;
} hw_mbox_masterConfig_t;

/*! \def HW_MBOX_MASTER_CONFIG_SIZE
    \brief Master configuration struct's size. */
#define HW_MBOX_MASTER_CONFIG_SIZE    (sizeof(hw_mbox_masterConfig_t))

/*! \def HW_MBOX_MASTER_TABLE(ENTRY)
    \brief X-MACRO hardcoded table for HW_MBOX Master declarations.
    \brief Position in the Table determines MID (Master ID) of the Master, a unique identifier
           dictated by Hardware (HW_MBOX Interrupt Line Number which connects to the Master).
    \attention Should be kept in sync with HW_MBOX_MASTER_MID defines and with HW_MBOX_IS_MASTER defines.
    \note NP_CPU is generic name for ARM11.
    \note APP_CPU is generic name for ATOM.
    \note US_MAC_CPU is generic name for Upstream MAC CPU ARM9.
    \note Interrupt line 13 is not physically connected, reserved for possible SW internal usage.
*/
#define HW_MBOX_MASTER_TABLE(ENTRY) \
/* +------------------------------------------------------------+ */ \
/* | MID |         | Name /        | HW_MBOX SW support         | */ \
/* |     |         | Description   | [hw_mbox_support_e]        | */ \
/* +------------------------------------------------------------+ */ \
/* |  0  | */ ENTRY( NP_CPU        , hw_mbox_support_dynamic    )    \
/* |  1  | */ ENTRY( BBU_C         , hw_mbox_support_static     )    \
/* |  2  | */ ENTRY( APP_CPU       , hw_mbox_support_dynamic    )    \
/* |  3  | */ ENTRY( RCE           , hw_mbox_support_static     )    \
/* |  4  | */ ENTRY( US_MAC_CPU    , hw_mbox_support_none       )    \
/* |  5  | */ ENTRY( PHY_CPU       , hw_mbox_support_static     )    \
/* |  6  | */ ENTRY( MOCA_C        , hw_mbox_support_static     )    \
/* |  7  | */ ENTRY( DS_MAC_C      , hw_mbox_support_static     )    \
/* |  8  | */ ENTRY( PP_NETSEC     , hw_mbox_support_none       )    \
/* |  9  | */ ENTRY( PP_RESEQ      , hw_mbox_support_none       )    \
/* | 10  | */ ENTRY( PP_STAGE1     , hw_mbox_support_none       )    \
/* | 11  | */ ENTRY( PP_QOS        , hw_mbox_support_none       )    \
/* | 12  | */ ENTRY( C55           , hw_mbox_support_none       )    \
/* | 13  | */ ENTRY( FREE0         , hw_mbox_support_none       )    \
/* | 14  | */ ENTRY( SEC           , hw_mbox_support_none       )    \
/* | 15  | */ ENTRY( CSE           , hw_mbox_support_none       )

/* Master Table Expanders: */

/*! \def HW_MBOX_MASTER_TABLE_AS_ENUM( _desc, _support )
    \brief use this as argument to HW_MBOX_MASTER_TABLE to expand table as enum.
    \note Masters will be enumerated according to this template: HW_MBOX_MASTER_<_desc>
*/
#define HW_MBOX_MASTER_TABLE_AS_ENUM( _desc, _support )      HW_MBOX_CONCAT2(HW_MBOX_MASTER_, _desc),

/*! \def HW_MBOX_MASTER_TABLE_AS_STRS( _desc, _support )
    \brief use this as argument to HW_MBOX_MASTER_TABLE to expand table as strings.
    \note Strings are created according to this template: "<_desc>"
*/
#define HW_MBOX_MASTER_TABLE_AS_STRS( _desc, _support )      HW_MBOX_STRINGIFY(_desc),

/*! \def HW_MBOX_MASTER_TABLE_AS_COUNT( _desc, _support )
    \brief use this as argument to HW_MBOX_MASTER_TABLE to expand table as entry count. */
#define HW_MBOX_MASTER_TABLE_AS_COUNT( _desc, _support )     1+

/*! \def HW_MBOX_MASTER_TABLE_AS_DB( _desc, _support )
    \brief use this as argument to HW_MBOX_MASTER_TABLE to expand table as DB.
    \note intializes an array of hw_mbox_masterConfig_t.
    \attention only create inside c file.
*/
#define HW_MBOX_MASTER_TABLE_AS_DB( _desc, _support )    { .support = _support },

/* Master Table Expander Wrappers: */

/*! \def HW_MBOX_MASTER_TABLE_STRS
    \brief Creates an string array with HW MBOX Masters names.
    \note "<_desc0>", "<_desc1>", ... [so we need] '\0' for array syntax and proper array null termination.
    \note creates and initializes a static Char * array with name <_arrayName>.
    \attention only use inside c file.
*/
#define HW_MBOX_MASTER_TABLE_STRS( _arrayName ) \
    static const Char * _arrayName[] =                     \
    {                                                      \
        HW_MBOX_MASTER_TABLE(HW_MBOX_MASTER_TABLE_AS_STRS) \
        NULL                                               \
    }

/*! \def HW_MBOX_MASTER_TABLE_DB( _arrayName )
    \brief Creates a hw_mbox_masterConfig_t array with HW MBOX Masters Data.
    \note creates and initializes a static hw_mbox_masterConfig_t array with name <_arrayName>.
    \attention only use inside c file.
*/
#define HW_MBOX_MASTER_TABLE_DB( _arrayName ) \
    static const hw_mbox_masterConfig_t _arrayName[] =     \
    {                                                      \
        HW_MBOX_MASTER_TABLE(HW_MBOX_MASTER_TABLE_AS_DB)   \
    }

/*! \def HW_MBOX_MASTER_TABLE_COUNT
    \brief wrapper for HW_MBOX_MASTER_TABLE_AS_COUNT.
    \note 1+ 1+ ... [so we need] 0 for syntax.
*/
#define HW_MBOX_MASTER_TABLE_COUNT    (HW_MBOX_MASTER_TABLE(HW_MBOX_MASTER_TABLE_AS_COUNT) 0)

/* Basic Sanity Check for Master Table: */
#if (HW_MBOX_MASTER_COUNT != (HW_MBOX_MASTER_TABLE_COUNT))
#   error "HW_MBOX MASTER TABLE: entry count in the table must equal the total Master Count!"
#endif

/*! \enum hw_mbox_Masters_e
    \brief Enumerate all HW_MBOX Master with MID value.
    \note name of the Masters is according to HW_MBOX_MASTER_TABLE_AS_ENUM Macro.
*/
typedef enum hw_mbox_Masters
{
    HW_MBOX_MID_INVALID = -1,
    HW_MBOX_MASTER_TABLE(HW_MBOX_MASTER_TABLE_AS_ENUM)
    HW_MBOX_MASTER_LAST_ENTRY
} hw_mbox_Masters_e;

/**********************************************
|                                             |
|        Variant Specific Declarations        |
|                                             |
**********************************************/

#ifdef HW_MBOX_OS_LINUX
/* Defines related to Kernel Driver: */

/*! \def HW_MBOX_DRV_DEV_DIR_NAME
    \brief directory under /dev containing the chnint char devices. */
#define HW_MBOX_DRV_DEV_DIR_NAME    "hw_mbox_drv"

/*! \def HW_MBOX_CHNINT_DEV_NAME
    \brief Prefix name of the chnint char devices, these devices are used to extend interrupt signals from Kernel to User Space.
    \note for channel XX the device is chnintXX [e.g. channel 8 -> chnint08].
    \note devices located in directory /dev/<HW_MBOX_DRV_DEV_DIR_NAME>
*/
#define HW_MBOX_CHNINT_DEV_NAME    "chnint"

/** --------------------------------------- **/

/*! \def HW_MBOX_RECENT_COMMAND_LOG    <Command_Count>
    \brief How many recent commands should be saved.
    \attention use a value > 0.
    \note Feature is OPTIONAL, comment out define to disable.
    \note Monitor Thread is in-charge of updating the Log.
*/
//#define HW_MBOX_RECENT_COMMAND_LOG    16

#ifdef HW_MBOX_RECENT_COMMAND_LOG

/* Argument Sanity Check: */
#if (HW_MBOX_RECENT_COMMAND_LOG <= 0)
#   error "HW_MBOX RECENT COMMAND LOG: Command Count Must be > 0!"
#endif

/*! \def HW_MBOX_RECENT_COMMAND_LOG_INDEX_INVALID
    \brief invalid index for the recent command log and snapshot log.
*/
#define HW_MBOX_RECENT_COMMAND_LOG_INDEX_INVALID    (-1)
#endif /* HW_MBOX_RECENT_COMMAND_LOG */
#endif /* HW_MBOX_OS_LINUX */

/**********************************************
|                                             |
|        Channel Types and Definitions        |
|                                             |
**********************************************/

/*! \enum hw_mbox_type_e
    \brief This enum denotes types of mboxs (protocols) that are supported. */
typedef enum hw_mbox_type
{
    hw_mbox_command = 0,  /* Command */
    hw_mbox_event,        /* Event   */
    hw_mbox_opcode,       /* Opcode (generic HW MBOX command-response protocol) */

    /* Don't add any values after this line!!! */

    HW_MBOX_TYPE_ONE_AFTER_LAST,
    HW_MBOX_TYPE_LAST = (HW_MBOX_TYPE_ONE_AFTER_LAST - 1),
    HW_MBOX_TYPE_COUNT
} hw_mbox_type_e;

/*! \def HW_MBOX_TYPE_IS_VALID( _mboxType )
    \brief check whether <_mboxType> is a valid HW_MBOX type.
    \param[in] _mboxType - value to check.
*/
#define HW_MBOX_TYPE_IS_VALID( _mboxType ) \
    ( ( ( _mboxType ) >= 0 ) && ( ( _mboxType ) <= HW_MBOX_TYPE_LAST ) )

/*! \def HW_MBOX_TYPE_STR(mboxType)
    \brief gets a string according to <mboxType>
    \attention <mboxType> should be a value from hw_mbox_type_e
*/
#define HW_MBOX_TYPE_STR(mboxType)    ( (mboxType) == hw_mbox_command ) ? ("Command"): \
                                      ( (mboxType) == hw_mbox_event   ) ? ("Event"  ): \
                                      ( (mboxType) == hw_mbox_opcode  ) ? ("Opcode" ): \
                                        ("__ERROR_UNKNOWN_HW_MBOX_TYPE__")

/*! \struct hw_mbox_sram_t
    \brief struct for Channel's SRAM configuration. */
typedef struct hw_mbox_sram
{
    Uint32 start; /* start Offset aligned and relvant to the 4KB of HW_MBOX SRAM */
    Uint32 size;  /* size is in bytes */
} hw_mbox_sram_t;

#define HW_MBOX_SRAM_CHUNK    4  /*! Allocated SRAM size must be an integral Multiple of 4B */

/*! \struct hw_mbox_channelConfig_t
    \brief struct for Channel's configuration. */
typedef struct hw_mbox_channelConfig
{
    Uint32              cid;
    hw_mbox_type_e      mboxType;
    hw_mbox_Masters_e   commander;
    hw_mbox_Masters_e   subject;
    hw_mbox_sram_t      memBuff;
} hw_mbox_channelConfig_t;

/*! \def HW_MBOX_CHANNEL_CONFIG_SIZE
    \brief channel configuration struct's size. */
#define HW_MBOX_CHANNEL_CONFIG_SIZE    sizeof(hw_mbox_channelConfig_t)

/*! \struct hw_mbox_masterPhyAddresses_t
    \brief struct for master's phyisical addresses */
typedef struct hw_mbox_masterPhyAddresses_t
{
    Uint32 intr_status_low_addr;
    Uint32 intr_status_high_addr;
    Uint32 intr_mask_low_addr;
    Uint32 intr_mask_high_addr;
    Uint32 intr_clear_low_addr;
    Uint32 intr_clear_high_addr;
    Uint32 commander_low_addr;
    Uint32 commander_high_addr;
    Uint32 subject_low_addr;
    Uint32 subject_high_addr;
} hw_mbox_masterPhyAddresses_t;

/*! \def HW_MBOX_MASTER_PHY_ADDR_SIZE
    \brief master's physical addresses struct's size. */
#define HW_MBOX_MASTER_PHY_ADDR_SIZE    sizeof(hw_mbox_masterPhyAddresses_t)

/*! \struct hw_mbox_channelPhyAddresses_t
    \brief struct for channel's phyisical addresses */
typedef struct hw_mbox_channelPhyAddresses
{
    Uint32 cfgRegAddr;
    Uint32 commandRegAddr;
    Uint32 stateRegAddr;
    Uint32 sramAddr;
    Uint32 sramSize;
} hw_mbox_channelPhyAddresses_t;

/*! \def HW_MBOX_CHANNEL_PHY_ADDR_SIZE
    \brief master's physical addresses struct's size. */
#define HW_MBOX_CHANNEL_PHY_ADDR_SIZE    sizeof(hw_mbox_channelPhyAddresses_t)

/****************************************************
|                                                   |
|        HW_MBOX Opcode                             |
|        (generic command-response protocol)        |
|                                                   |
****************************************************/

/* HW MBOX OPCODE: a generic command-response protocol

   OPCODE type channels exist to implement a generic command-response
   sequence that can be used by package customers. The protocol predefines
   the structure of the values that are to be written to command channel
   register. While enabling less freedom to applications it enables the
   construction of generic entities that can be used for message dispatching
   without relying completely on application context. OPCODE type channels
   enable inherent support for several applications on the same channel.
*/

/* The Fields that OPCODE channel uses are:

   TAG - Indicates the application that is the destination of the code.
         Note: Several TAGs are reserved for HW MBOX internal usage
               [see hwMbox_opcodeReservedTags_e].

   LEN - How many Bytes are to be read from SRAM (opcode Data).
         [ LEN is always an integral multiple of 4 Bytes so we can
           use word count when writing to the command register ].

   exact Field division is defined in mem_access file.
*/

/*! \struct hwMbox_opcode_t
    \brief struct for opcode fields.
    \note the types used here can have more capacity than actual max value.
*/
typedef struct hwMbox_opcode
{
    Uint32  tag;
    Uint32  lenInBytes;
} hwMbox_opcode_t;

/*! \def HW_MBOX_OPCODE_MINIMAL_SRAM_ALLOCATION_IN_BYTES
    \brief a minimal amount of SRAM that should be allocated to Opcode channels.
    \note an Opcode channel configuration with less than this amount is considered invalid.
*/
#define HW_MBOX_OPCODE_MINIMAL_SRAM_ALLOCATION_IN_BYTES    (2 * HW_MBOX_SRAM_CHUNK)

/* Field Bit Length definitions:
   [These are done here so max value possible can be calculated and used here...]

   Note: if value change please update also corresponding documentation
         in mem access.
*/

#define HW_MBOX_OPCODE_TAG_BITLEN    (12) /*! allocated amount of bits for the TAG field */

/*! \def HW_MBOX_OPCODE_TAG_MAX_VALUE
    \brief Max value available for tag field when using OPCODE. */
#define HW_MBOX_OPCODE_TAG_MAX_VALUE    (BIT(HW_MBOX_OPCODE_TAG_BITLEN) - 1)

#ifdef HW_MBOX_SOCKETS_API
/*! \def HW_MBOX_TAG_INVALID
    \brief Value that indicates invalid TAG. */
#define HW_MBOX_TAG_INVALID             0xFFFFFFFF
#endif /* HW_MBOX_SOCKETS_API */

/* Reserved HW MBOX TAGs: */

#define HW_MBOX_OPCODE_RESERVED_TAGS_START    0xFF0 /*! First Reserved TAG value for HW MBOX */

/*! \enum hwMbox_opcodeReservedTags_e
    \brief This enum denotes HW MBOX reserved opcode tags.
*/
typedef enum hwMbox_opcodeReservedTags
{
    HW_MBOX_LAST_NON_RESREVED_OPCODE_TAG_VALUE = (HW_MBOX_OPCODE_RESERVED_TAGS_START - 1),

    /* Don't add any values before this line!!! */

    HW_MBOX_RESREVED_OPCODE_TAG_0,
    HW_MBOX_RESREVED_OPCODE_TAG_1,
    HW_MBOX_RESREVED_OPCODE_TAG_2,
    HW_MBOX_RESREVED_OPCODE_TAG_3,
    HW_MBOX_RESREVED_OPCODE_TAG_4,
    HW_MBOX_RESREVED_OPCODE_TAG_5,
    HW_MBOX_RESREVED_OPCODE_TAG_6,
    HW_MBOX_RESREVED_OPCODE_TAG_7,
    HW_MBOX_RESREVED_OPCODE_TAG_8,
    HW_MBOX_RESREVED_OPCODE_TAG_9,
    HW_MBOX_RESREVED_OPCODE_TAG_10,
    HW_MBOX_RESREVED_OPCODE_TAG_11,
    HW_MBOX_RESREVED_OPCODE_TAG_12,
    HW_MBOX_RESREVED_OPCODE_TAG_13,
    HW_MBOX_OPCODE_USER_SPACE_TAG,      /*! [0xFFE] Reserved TAG value for User-Space HW MBOX */
    HW_MBOX_OPCODE_TAG,                 /*! [0xFFF] Reserved TAG value for Kernel HW MBOX */

    /* Don't add any values after this line!!! */

    HW_MBOX_RESERVED_OPCODE_ONE_AFTER_LAST,
    HW_MBOX_RESERVED_OPCODE_LAST = (HW_MBOX_RESERVED_OPCODE_ONE_AFTER_LAST - 1)    /*! Last reserved TAG value */
} hwMbox_opcodeReservedTags_e;

/*! \typedef __hwMbox_reservedOpcodesEnumExceededAllowedSize
    \brief Sanity check for reserved opcodes enum.
    \note if compilation fails here then hwMbox_opcodeReservedTags_e exceeded the allowed size.
*/
typedef Uint8 __hwMbox_reservedOpcodesEnumExceededAllowedSize[HW_MBOX_OPCODE_TAG_MAX_VALUE - HW_MBOX_RESERVED_OPCODE_LAST];

/*! \enum hwMbox_appId_e
    \brief This enum denotes HW MBOX APP IDs.
*/
typedef enum hwMbox_appId
{
    HW_MBOX_APPID_MIN_VALUE  = 0,
    HW_MBOX_APPID_OPCODE_MIN = HW_MBOX_APPID_MIN_VALUE,
    /* Opcode Tags... */
    HW_MBOX_APPID_OPCODE_MAX = HW_MBOX_OPCODE_TAG_MAX_VALUE,
    /* Other Protocols: */
    HW_MBOX_APPID_COMMAND,
    HW_MBOX_APPID_EVENT,

    /* Don't add any values after this line!!! */
    HW_MBOX_APPID_ONE_AFTER_LAST,
    HW_MBOX_APPID_LAST = (HW_MBOX_APPID_ONE_AFTER_LAST - 1),
    HW_MBOX_APPID_MAX_VALUE = HW_MBOX_APPID_LAST
} hwMbox_appId_e;

/* Recv Opcode Infrastructure: */

#define HW_MBOX_OPCODE_RECV_COUNT    10    /*! max amount of recv opcode channels per Master. */
#define HW_MBOX_OPCODE_TAG_COUNT     10    /*! max amount of tags that can be registered per RecvOpcode channel. */

/*! \typedef int (*hwMboxRecvOpcodeCallback)(hw_mbox_Masters_e commander,
                                             Uint8 *dataBuf, Uint32 dataLen,
                                             Uint32 *token)
    \brief prototype for receive opcode channels callback function pointers, to be
           invoked by Receive Opcode Listeners upon incoming opcode.
    \param[in]     commander - Master from which the opcode has been received.
    \param[in,out] dataBuf - opcode data that has been received.
    \param[in]     dataLen - Length in bytes of <dataBuf>.
    \param[in,out] token - 32 bit saved in DB upon registering the callback,
                           modifiable by the callback.
    \note return value (-1) is reserved to indicate generic error, use token
          to output an error from <HW_MBOX_STATUS_e>, it will be sent
          to the commander and the sendOpcode service it invoked will fail.
*/
typedef int (*hwMboxRecvOpcodeCallback)(hw_mbox_Masters_e commander,
                                        Uint8 *dataBuf, Uint32 dataLen,
                                        Uint32 *token);

/* Types and Defines for tag HW_MBOX_OPCODE_TAG: */

typedef enum hwMbox_opcodeMessageTypes
{
    HW_MBOX_OPCODE_MESSAGE_TYPE_CODE = 0,
    HW_MBOX_OPCODE_MESSAGE_TYPE_STATUS,

    HW_MBOX_OPCODE_MESSAGE_TYPE_COUNT /*! this should be last... */
} hwMbox_opcodeMessageTypes_e;

/* Code Message: */

/*! \enum hwMbox_code_e
    \brief This enum denotes codes of internal administrative HW MBOX
           commands to be sent on opcode channel.
*/
typedef enum hwMbox_code
{
    HW_MBOX_CODE_INIT_COMPLETE = 0,    /*! Init complete code [sent to other Masters by supervisor]. */
    HW_MBOX_CODE_PING,                 /*! Ping SW support on other Master */

    HW_MBOX_CODE_COUNT /*! Should always be last... */
} hwMbox_code_e;

/*! \struct hwMbox_opcodeMessageCode_t
    \brief Opcode message to send a HW MBOX code to the other Master.
*/
typedef struct hwMbox_opcodeMessageCode
{
    hwMbox_code_e code;
} hwMbox_opcodeMessageCode_t;

#define HW_MBOX_OPCODE_MESSAGE_CODE_SIZE    sizeof(hwMbox_opcodeMessageCode_t)

/* Status Message: */

/*! \struct hwMbox_opcodeMessageStatus_t
    \brief Opcode message to send a HW MBOX Status to the other Master.
*/
typedef struct hwMbox_opcodeMessageStatus
{
    HW_MBOX_STATUS_e status;
} hwMbox_opcodeMessageStatus_t;

#define HW_MBOX_OPCODE_MESSAGE_STATUS_SIZE     sizeof(hwMbox_opcodeMessageStatus_t)

/* Opcode Message: */

/*! \union hwMbox_opcodeMessageData_u
    \brief Union for all types of opcode message data.
*/
typedef union hwMbox_opcodeMessageData
{
    hwMbox_opcodeMessageCode_t      msgCode;
    hwMbox_opcodeMessageStatus_t    msgStatus;
} hwMbox_opcodeMessageData_u;

/*! \struct hwMbox_opcodeMessage_t
    \brief Opcode message for tag HW_MBOX_OPCODE_TAG.
    \note This message struct size should always be a multiple of 4 Bytes.
    \attention When writing these messages to SRAM it should always be Big Endian.
*/
typedef struct hwMbox_opcodeMessage
{
    hwMbox_opcodeMessageTypes_e msgType;
    hwMbox_opcodeMessageData_u  msgData;
} hwMbox_opcodeMessage_t;

#define HW_MBOX_OPCODE_MESSAGE_SIZE     sizeof(hwMbox_opcodeMessage_t)

/*! \typedef __hwMbox_opcodeMessageSizeIsNotAMultipleOf4Bytes
    \brief Sanity check for opcode message size being a mutiple of 4 Bytes.
    \note if compilation fails here then hwMbox_opcodeMessage_t is not a multiple of 4 Bytes.
*/
typedef Uint8 __hwMbox_opcodeMessageSizeIsNotAMultipleOf4Bytes[0 - (HW_MBOX_OPCODE_MESSAGE_SIZE & 0x3)];

/*! \typedef __hwMbox_opcodeMessageSizeExceededAllowedSize
    \brief Sanity check for opcode message size.
    \note if compilation fails here then hwMbox_opcodeMessage_t exceeded the allowed size.
    \note The maximal allowed size is as such that this message can always be sent on opcode channels.
    \note Meaning, it shouldn't exceed the minimal amount of SRAM allocate to opcode channels.
*/
typedef Uint8 __hwMbox_opcodeMessageSizeExceededAllowedSize[HW_MBOX_OPCODE_MINIMAL_SRAM_ALLOCATION_IN_BYTES - HW_MBOX_OPCODE_MESSAGE_SIZE];

/********************************************
|                                           |
|        STATIC channel Declarations        |
|                                           |
********************************************/

#define HW_MBOX_NP_APP_SRAM_SIZE 256
#define HW_MBOX_APP_NP_SRAM_SIZE 256

/*! \def HW_MBOX_STATIC_CHANNEL_TABLE
    \brief X-MACRO hardcoded table for STATIC Channel declarations.
    \attention SRAM size must be an integral Multiple of HW_MBOX_SRAM_CHUNK = 4B.
    \attention An event channel must have SRAM allocated to accomdate the event buffer.
    \attention An event channel SRAM size must be divisible to 2 equal sized valid individual SRAM buffers
              [for the double buffer] --> meaning it should be an integral multiple of 2 * HW_MBOX_SRAM_CHUNK = 8B.
    \attention An opcode channel SRAM size must be >= HW_MBOX_OPCODE_MINIMAL_SRAM_ALLOCATION_IN_BYTES.
*/
#define HW_MBOX_STATIC_CHANNEL_TABLE(ENTRY) \
/*       +------------------------------------------------------------------------------------------------------------------------------------------+ */ \
/*       |  CID  | Commander                           | Subject                             | HW_MBOX Channel Type     | SRAM size                 | */ \
/*       |       | [hw_mbox_Masters_e]                 | [hw_mbox_Masters_e]                 | [hw_mbox_channel_type_e] | [Bytes]                   | */ \
/*       +------------------------------------------------------------------------------------------------------------------------------------------+ */ \
    ENTRY(    0  , HW_MBOX_MASTER_NP_CPU               , HW_MBOX_MASTER_BBU_C                , hw_mbox_command          , 32                         )    \
    ENTRY(    1  , HW_MBOX_MASTER_NP_CPU               , HW_MBOX_MASTER_PHY_CPU              , hw_mbox_command          , 32                        )    \
    ENTRY(    2  , HW_MBOX_MASTER_NP_CPU               , HW_MBOX_MASTER_MOCA_C               , hw_mbox_opcode           , 8                         )    \
    ENTRY(    3  , HW_MBOX_MASTER_BBU_C                , HW_MBOX_MASTER_NP_CPU               , hw_mbox_event            , 8                         )    \
    ENTRY(    4  , HW_MBOX_MASTER_PHY_CPU              , HW_MBOX_MASTER_NP_CPU               , hw_mbox_event            , 656                       )    \
    ENTRY(    5  , HW_MBOX_MASTER_MOCA_C               , HW_MBOX_MASTER_NP_CPU               , hw_mbox_event            , 8                         )    \
    ENTRY(    6  , HW_MBOX_MASTER_NP_CPU               , HW_MBOX_MASTER_BBU_C                , hw_mbox_opcode           , 8                         )    \
    ENTRY(    7  , HW_MBOX_MASTER_NP_CPU               , HW_MBOX_MASTER_APP_CPU              , hw_mbox_opcode           , HW_MBOX_NP_APP_SRAM_SIZE  )    \
    ENTRY(    8  , HW_MBOX_MASTER_NP_CPU               , HW_MBOX_MASTER_PHY_CPU              , hw_mbox_opcode           , 8                         )    \
    ENTRY(    9  , HW_MBOX_MASTER_NP_CPU               , HW_MBOX_MASTER_MOCA_C               , hw_mbox_opcode           , 8                         )    \
    ENTRY(   10  , HW_MBOX_MASTER_APP_CPU              , HW_MBOX_MASTER_NP_CPU               , hw_mbox_opcode           , HW_MBOX_APP_NP_SRAM_SIZE  )    \
    ENTRY(   11  , HW_MBOX_MASTER_NP_CPU               , HW_MBOX_MASTER_RCE                  , hw_mbox_command          , 0                         )    \
    ENTRY(   12  , HW_MBOX_MASTER_RCE                  , HW_MBOX_MASTER_NP_CPU               , hw_mbox_command          , 0                         )    \
    ENTRY(   13  , HW_MBOX_MASTER_NP_CPU               , HW_MBOX_MASTER_MOCA_C               , hw_mbox_command          , 64                        )    \
    ENTRY(   14  , HW_MBOX_MASTER_MOCA_C               , HW_MBOX_MASTER_NP_CPU               , hw_mbox_command          , 64                        )    \
    ENTRY(   15  , HW_MBOX_MASTER_DS_MAC_C             , HW_MBOX_MASTER_PHY_CPU              , hw_mbox_command          , 24                        )

/* static channel Table Expanders: */

/*! \def HW_MBOX_STATIC_CHANNEL_TABLE_AS_COUNT( _cid, _commander, _subject, _mboxType, _sramSize )
    \brief use this as argument to HW_MBOX_STATIC_CHANNEL_TABLE to expand table as entry count. */
#define HW_MBOX_STATIC_CHANNEL_TABLE_AS_COUNT( _cid, _commander, _subject, _mboxType, _sramSize )    1+

/*! \def HW_MBOX_STATIC_CHANNEL_TABLE_AS_DB(_cid, _commander, _subject, _mboxType, _sramSize)
    \brief use this as argument to HW_MBOX_STATIC_CHANNEL_TABLE to expand table as DB.
    \note intializes an array of hw_mbox_channelConfig_t.
    \attention only create inside c file.
*/
#define HW_MBOX_STATIC_CHANNEL_TABLE_AS_DB(_cid, _commander, _subject, _mboxType, _sramSize) \
{ .cid = _cid, .mboxType = _mboxType, .commander = _commander, .subject = _subject, .memBuff.start = 0 /* To be updated when SRAM is allocated */, .memBuff.size = _sramSize },

/* static channel Table Expander Wrappers: */

/*! \def HW_MBOX_STATIC_CHANNEL_TABLE_DB( _arrayName )
    \brief Creates a hw_mbox_channelConfig_t array with HW MBOX static channels Data.
    \note creates and initializes a static hw_mbox_channelConfig_t array with name <_arrayName>.
    \attention only use inside c file.
*/
#define HW_MBOX_STATIC_CHANNEL_TABLE_DB( _arrayName ) \
    static const hw_mbox_channelConfig_t _arrayName[] =                    \
    {                                                                      \
        HW_MBOX_STATIC_CHANNEL_TABLE(HW_MBOX_STATIC_CHANNEL_TABLE_AS_DB)   \
    }

/*! \def HW_MBOX_STATIC_CHANNEL_COUNT
    \brief wrapper for HW_MBOX_STATIC_CHANNEL_TABLE_AS_COUNT.
    \note 1+ 1+ ... [so we need] 0 for syntax.
*/
#define HW_MBOX_STATIC_CHANNEL_COUNT    (HW_MBOX_STATIC_CHANNEL_TABLE(HW_MBOX_STATIC_CHANNEL_TABLE_AS_COUNT) 0) /* 1+ ... 1+ [need] 0 for syntax */

/* Basic Sanity Check for static channel Table */
#if ((HW_MBOX_STATIC_CHANNEL_COUNT) > HW_MBOX_CHANNEL_COUNT)
#   error "HW_MBOX STATIC CHANNELS: entry count in the table exceeds the total Channel Count!"
#endif

#endif /* ! _HW_MBOX_DEFS_H_ */
