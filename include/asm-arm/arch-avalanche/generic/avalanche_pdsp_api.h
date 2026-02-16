/*
  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015-2017 Intel Corporation.

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

  Copyright(c) 2014-2017 Intel Corporation. All rights reserved.

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

#ifndef _AVALANCHE_PDSP_H
#define _AVALANCHE_PDSP_H

#ifdef CONFIG_ARM_AVALANCHE_SOC
#include <asm-arm/arch-avalanche/generic/_tistdtypes.h>
#ifndef __KERNEL__
#include <puma_autoconf.h>
#endif
#else /* NP- CPU */
#ifdef __KERNEL__
#include <linux/avalanche/puma7/puma7_defs.h>
#include <linux/avalanche/generic/_tistdtypes.h>
#else
#include "_tistdtypes.h"
#endif
#endif
#include <linux/ioctl.h>


#if PUMA7_OR_NEWER_SOC_TYPE
    #define AVALANCHE_PDSP_H_PUMA7

#define PRECMD_INDEX_SHIFT        16
#define PRECMD_OPTION_SHIFT       8
#define PRECMD_COMMAND_SHIFT      0
#define PRECMD_INDEX_MASK         (0xFFFFu << PRECMD_INDEX_SHIFT)

#define PRECMD_COMMAND_MASK       (0xFFu << PRECMD_COMMAND_SHIFT)
#define PRECMD_COMMAND(x)         (((x) << PRECMD_COMMAND_SHIFT) & PRECMD_COMMAND_MASK)
#define PRECMD_INDEX(x)           (((x) << PRECMD_INDEX_SHIFT)   & PRECMD_INDEX_MASK)
#define PRECMD_OPTION_MASK        (0xFFu << PRECMD_OPTION_SHIFT)
#define PRECMD_OPTION(x)          (((x) << PRECMD_OPTION_SHIFT)  & PRECMD_OPTION_MASK)

#define MIN_PKTS_FOR_QDA_ACTIVATION_STEP_A0 (0xffffffff)
#define MIN_PKTS_FOR_QDA_ACTIVATION_STEP_B0 (0xffffffff)
#define MIN_PKTS_FOR_QDA_ACTIVATION_ADDR    (0xF3E13200)

#if defined(CONFIG_MACH_PUMA7_FPGA_PP) || defined (CONFIG_MACH_PUMA7_BOARD)
    //#define FPGA_SANITY_UNITEST     0
#endif
#elif defined(PUMA6_SOC_TYPE) && PUMA6_SOC_TYPE
    #define AVALANCHE_PDSP_H_PUMA6
#else
    #undef  AVALANCHE_PDSP_H_PUMA6
    #undef  AVALANCHE_PDSP_H_PUMA7
#endif

#ifdef CONFIG_WIFI_MESH_TUNNEL
#define WIFI_MESH_MODE                      (1<<0)
#endif

/* PP PDSP commands */
typedef enum
{
#if defined (AVALANCHE_PDSP_H_PUMA7)

    // Global PDSP  commands
    PDSP_ENABLE                     = 0x80,
    PDSP_GET_STATUS                 = 0x81,
    PDSP_DISABLE                    = 0x82,
    PDSP_SANITY_CHECK_EXECUTE       = 0x90,

    // Sequencer    commands
    PDSP_SEQUENCER_SET_TDQ          = 0x83,
    PDSP_SEQUENCER_PID_COMMAND      = 0x84,

    // Classifier1  commands
    PDSP_CLASSIFIER1_VPID_COMMAND   = 0x83,

    // Classifier2  commands
    PDSP_CLASSIFIER2_PID_COMMAND                 = 0x83,
    PDSP_CLASSIFIER2_CLASSIFICATION_MODE_COMMAND = 0x84,
    PDSP_CLASSIFIER2_MOCA_IRREG_SESS_OPEN        = 0x85,
    PDSP_CLASSIFIER2_SUPPORT_BP_FRAG             = 0x86,
    PDSP_CLASSIFIER2_SUPPORT_MULTI_DROP          = 0x87,
    PDSP_CLASSIFIER2_CHANGE_LUT_CLASSIFICATION   = 0x88,
    PDSP_CLASSIFIER2_SUPPORT_SMART_PRIORITIZATION= 0x89,

    // PDSP ReSequencer commands
    PDSP_RESEQUENCER_SET_SYNC_Q_THRSHOLD = 0x83,

    // QoS          commands
    PDSP_QOS_CMD_CONFIG_CLUSTER     = 0x83,

    // Accumulator  commands
    PDSP_ACCUMULATOR_ENABLE_CH              = 0x83,
    PDSP_ACCUMULATOR_DISABLE_CH             = 0x84,
    PDSP_ACCUMULATOR_ENABLE_CH_RATE_LIMIT   = 0x85,
    PDSP_ACCUMULATOR_ENABLE_RATE_LIMIT      = 0x86,

    // Recycler  commands (For Sanity tests)
    PDSP_RECYCLER_SANITY_START_CMD   = 0x83,
    PDSP_RECYCLER_SANITY_STOP_CMD    = 0x84,

    // Session Cache Commands
    PDSP_SESSION_CACHE_SESSION_CMD               = 0x83,
    PDSP_SESSION_CACHE_FLUSH_ALL                 = 0x84,
    PDSP_SESSION_CACHE_SESSION_TIMEOUT           = 0x85,
    PDSP_SESSION_CACHE_TIMEOUT_THRESHOLDS_SET    = 0x86,
    PDSP_SESSION_CACHE_TIMEOUT_THRESHOLDS_ENABLE = 0x87,
    PDSP_SESSION_CACHE_INTC_IRQ                  = 0x88,

    // tDox Commands
    PDSP_TDOX_CONFIGURE_PARAMS          = 0x83,
    PDSP_TDOX_SET_SUPPRESSION           = 0x84,
    PDSP_TDOX_CMD_SET_RECORD            = 0x85,
    PDSP_TDOX_CONFIGURE_SUPPRESSION     = 0x86,

    // Moca Commands
    PDSP_MOCA_SET_PADDING                = 0x83,

    // AQMC Commands
    PDSP_AQMC_ADD_MC_MEMBER              = 0x83,
    PDSP_AQMC_DEL_MC_MEMBER              = 0x84,
    PDSP_AQMC_SET_AQM_BYPASS_MODE        = 0x85,
    PDSP_AQMC_DEL_MC_GROUP               = 0x86,

#ifdef CONFIG_WIFI_PROXY
    //Wifi TX commands
    PDSP_CMD_WIFI_TX_ADD_CHANNEL         = 0x84,
    PDSP_CMD_WIFI_TX_REMOVE_CHANNEL      = 0x85,
    PDSP_CMD_WIFI_TX_START_CHANNEL       = 0x86,
    PDSP_CMD_WIFI_TX_STOP_CHANNEL        = 0x87,
    PDSP_CMD_WIFI_TX_INIT_DBG            = 0x88,

    //Wifi RX commands
    PDSP_CMD_WIFI_RX_INIT                = 0x83,
    PDSP_CMD_WIFI_RX_ADD_CHANNEL         = 0x84,
    PDSP_CMD_WIFI_RX_REMOVE_CHANNEL      = 0x85,
    PDSP_CMD_WIFI_RX_START_CHANNEL       = 0x86,
    PDSP_CMD_WIFI_RX_STOP_CHANNEL        = 0x87,
    PDSP_CMD_WIFI_RX_INIT_DBG            = 0x88,
#endif

#else
    PP_HAL_PDSP_CMD_OPEN    =   0x80,
    PP_HAL_PDSP_CMD_FLUSH_ALL,
    PP_HAL_PDSP_CMD_FLUSH_MANY,
    PP_HAL_PDSP_CMD_reserved_x83,
    PP_HAL_PDSP_CMD_PID,
    PP_HAL_PDSP_CMD_VPID,
    PP_HAL_PDSP_CMD_SESSION,
    PP_HAL_PDSP_CMD_STATUS,
    PP_HAL_PDSP_CMD_PSM,
    PP_HAL_PDSP_CMD_VERSION,
    PP_HAL_PDSP_CMD_reserved_x8A,
    PP_HAL_PDSP_CMD_reserved_x8B,
    PP_HAL_PDSP_CMD_ACK_SUPPRESS,
    PP_HAL_PDSP_CMD_QOS_CLUSTER = 0xA0,

    PDSP_PREFETCHER_CONFIG_TDQ = 0x87,
    PDSP_PREFETCHER_ENABLE  = 0x83
#endif
}PP_HAL_PDSP_CMD_e;

#if defined (AVALANCHE_PDSP_H_PUMA7)

typedef enum
{
    PDSP_ID_START,
    PDSP_ID_Sequencer = PDSP_ID_START,          //  Sequencer

    PDSP_ID_Prefetcher_0,                       //  Prefetcher   group - PDSP 0
    PDSP_ID_Prefetcher_1,                       //  Prefetcher   group - PDSP 1
    PDSP_ID_Prefetcher_2,                       //  Prefetcher   group - PDSP 2
    PDSP_ID_Prefetcher_3,                       //  Prefetcher   group - PDSP 3

    PDSP_ID_Classifier1_0,                      //  Classifier1  group - PDSP 0
    PDSP_ID_Classifier1_1,                      //  Classifier1  group - PDSP 1
    PDSP_ID_Classifier1_2,                      //  Classifier1  group - PDSP 2
    PDSP_ID_Classifier1_3,                      //  Classifier1  group - PDSP 3

    PDSP_ID_Classifier2_0,                      //  Classifier2  group - PDSP 0
    PDSP_ID_Classifier2_1,                      //  Classifier2  group - PDSP 1
    PDSP_ID_Classifier2_2,                      //  Classifier2  group - PDSP 2
    PDSP_ID_Classifier2_3,                      //  Classifier2  group - PDSP 3

    PDSP_ID_Modifier_0,                         //  Modifier     group - PDSP 0
    PDSP_ID_Modifier_1,                         //  Modifier     group - PDSP 1
    PDSP_ID_Modifier_2,                         //  Modifier     group - PDSP 2
    PDSP_ID_Modifier_3,                         //  Modifier     group - PDSP 3

    PDSP_ID_ReSequencer,                        //  ReSequencer

    PDSP_ID_QoS_0,                              //  QoS          group - PDSP 0
    PDSP_ID_QoS_1,                              //  QoS          group - PDSP 1

    PDSP_ID_Recycler,                           //  Recycler PDSP
    PDSP_ID_Accumulator,                        //  Accumulator PDSP
    PDSP_ID_Wifi_TX,                            //  Wifi proxy - TX PDSP (Wifi_0)
    PDSP_ID_Wifi_RX,                            //  Wifi proxy - RX PDSP (Wifi_1)
    PDSP_ID_Moca,                               //  Moca PDSP

    PDSP_ID_Encrypt,                            // Encrypt (formerly FCC_0)
    PDSP_ID_Decrypt,                            // Decrypt (formerly FCC_1)
    PDSP_ID_DPI,                                // DPI PDSP
    PDSP_ID_Crypto,                             // Crypto ("Security")
    PDSP_ID_RCE,                                // RCE PDSP

    PDSP_ID_Session_Cache,                      //  Session Cache PDSP
    PDSP_ID_AQM,                                //  AQM PDSP
    PDSP_ID_MC,                                 //  MC PDSP
    PDSP_ID_TurboDOX,                           //  TurboDOX PDSP

    PDSP_ID_DS_RESEQ,
    PDSP_ID_US_START,                           //  US PDSP first index
    PDSP_ID_US_Preprocessor = PDSP_ID_US_START, //  US Preprocessing PDSP
    PDSP_ID_US_Request_Builder,                 //  US REQuest Builder PDSP
    PDSP_ID_US_Frag_Divert,                     //  US Frag& Divert PDSP
    PDSP_ID_US_END = PDSP_ID_US_Frag_Divert,    //  US PDSP last index
    PDSP_ID_MAX
}pdsp_id_t;

/* PDSP Timers definitions */
#define AVALANCHE_PDSP_TIMER_CTRL_OFF                       0x00
#define AVALANCHE_PDSP_TIMER_LOAD_OFF                       0x04
#define AVALANCHE_PDSP_TIMER_CTRL_PRESCALER_ENABLE_SHIFT    15
#define AVALANCHE_PDSP_TIMER_CTRL_PRESCALER_VALUE_SHIFT     2
#define AVALANCHE_PDSP_TIMER_CTRL_AUTO_LOAD_SHIFT           1
#define AVALANCHE_PDSP_TIMER_CTRL_START_SHIFT               0

#elif defined (AVALANCHE_PDSP_H_PUMA6)
typedef enum
{
    PDSP_ID_START,
    PDSP_ID_Prefetcher_0 = PDSP_ID_START,   //  Prefetcher   group - PDSP 0
    PDSP_ID_Classifier1_0,                  //  Classifier1  group - PDSP 0
    PDSP_ID_Classifier2_0,                  //  Classifier2  group - PDSP 0
    PDSP_ID_Modifier_0,                     //  Modifier     group - PDSP 0
    PDSP_ID_QoS_0,                          //  QoS          group - PDSP 0
    PDSP_ID_LAN_Proxy,                      //  _PrxPDSP only in P6
    PDSP_ID_CoE,                            //  _CoePDSP only in P6
    PDSP_ID_MAX
}pdsp_id_t;

#else // PUMA5
typedef enum
{
    PDSP_ID_START,
    PDSP_ID_Prefetcher_0 = PDSP_ID_START,   //  Prefetcher
    PDSP_ID_Classifier1_0,                  //  Classifier
    PDSP_ID_Modifier_0,                     //  Modifier
    PDSP_ID_QoS_0,                          //  QoS
    PDSP_ID_MAX
}pdsp_id_t;
#endif


typedef Uint32 pdsp_cmd_t;

typedef struct
{
    pdsp_id_t       pdsp_id;
    pdsp_cmd_t      cmd;
    Uint32          params_len;
    Uint32          params[64];
}
pdsp_cmd_params_t;

/********************************************************************************************************/
/* IOCTL commands:

   If you are adding new ioctl's to the kernel, you should use the _IO
   macros defined in <linux/ioctl.h> _IO macros are used to create ioctl numbers:

    _IO(type, nr)         - an ioctl with no parameter.
   _IOW(type, nr, size)  - an ioctl with write parameters (copy_from_user), kernel would actually read data from user space
   _IOR(type, nr, size)  - an ioctl with read parameters (copy_to_user), kernel would actually write data to user space
   _IOWR(type, nr, size) - an ioctl with both write and read parameters

   'Write' and 'read' are from the user's point of view, just like the
    system calls 'write' and 'read'.  For example, a SET_FOO ioctl would
    be _IOW, although the kernel would actually read data from user space;
    a GET_FOO ioctl would be _IOR, although the kernel would actually write
    data to user space.

    The first argument to _IO, _IOW, _IOR, or _IOWR is an identifying letter
    or number from the SoC_ModuleIds_e enum located in this file.

    The second argument to _IO, _IOW, _IOR, or _IOWR is a sequence number
    to distinguish ioctls from each other.

   The third argument to _IOW, _IOR, or _IOWR is the type of the data going
   into the kernel or coming out of the kernel (e.g.  'int' or 'struct foo').

   NOTE!  Do NOT use sizeof(arg) as the third argument as this results in
   your ioctl thinking it passes an argument of type size_t.

*/
#define PDSP_DRIVER_MODULE_ID                   (0xDE)

#define PDSP_DRIVER_RESET_PDSP                      _IOW (PDSP_DRIVER_MODULE_ID,  1, pdsp_id_t)
#define PDSP_DRIVER_START_PDSP                      _IOW (PDSP_DRIVER_MODULE_ID,  2, pdsp_id_t)
#define PDSP_DRIVER_DOWNLOAD_START                  _IOW (PDSP_DRIVER_MODULE_ID,  3, pdsp_id_t)
#define PDSP_DRIVER_DOWNLOAD_FINISH                 _IOW (PDSP_DRIVER_MODULE_ID,  4, pdsp_id_t)
#define PDSP_DRIVER_TEST_IRAM                       _IOW (PDSP_DRIVER_MODULE_ID,  5, pdsp_id_t)
#define PDSP_DRIVER_PUT_CMD                         _IOWR(PDSP_DRIVER_MODULE_ID,  6, pdsp_cmd_params_t)
#define PDSP_DRIVER_SANITY_CHECK_START              _IOW (PDSP_DRIVER_MODULE_ID,  7, pdsp_id_t)
#define PDSP_DRIVER_EXECUTE_SANITY_CHECK            _IOWR(PDSP_DRIVER_MODULE_ID,  8, pdsp_cmd_params_t)
#define PDSP_DRIVER_SANITY_CHECK_END                _IOW (PDSP_DRIVER_MODULE_ID,  9, pdsp_id_t)
#define PDSP_DRIVER_EXECUTE_TURBODOX_SANITY_CHECK   _IOWR(PDSP_DRIVER_MODULE_ID, 10, pdsp_cmd_params_t)
#define PDSP_DRIVER_HALT_PDSP                       _IOWR(PDSP_DRIVER_MODULE_ID, 11, pdsp_cmd_params_t)
#define PDSP_DRIVER_RESUME_PDSP                     _IOWR(PDSP_DRIVER_MODULE_ID, 12, pdsp_cmd_params_t)
#define PDSP_DRIVER_PDSP_BREAKPOINT_ENABLE          _IOWR(PDSP_DRIVER_MODULE_ID, 13, pdsp_cmd_params_t)
#define PDSP_DRIVER_PDSP_BREAKPOINT_DELETE_ALL      _IOWR(PDSP_DRIVER_MODULE_ID, 14, pdsp_cmd_params_t)
#define PDSP_DRIVER_PDSP_BREAKPOINT_DISABLE         _IOWR(PDSP_DRIVER_MODULE_ID, 15, pdsp_cmd_params_t)
#define PDSP_DRIVER_PDSP_BREAKPOINT_PRINT           _IOWR(PDSP_DRIVER_MODULE_ID, 16, pdsp_cmd_params_t)
#define PDSP_DRIVER_PDSP_SINGLE_STEP_ENABLE         _IOWR(PDSP_DRIVER_MODULE_ID, 17, pdsp_cmd_params_t)
#define PDSP_DRIVER_PDSP_SINGLE_STEP_DISABLE        _IOWR(PDSP_DRIVER_MODULE_ID, 18, pdsp_cmd_params_t)
#define PDSP_DRIVER_PDSP_RUN_TO_OP_CODE             _IOWR(PDSP_DRIVER_MODULE_ID, 19, pdsp_cmd_params_t)
#define PDSP_DRIVER_PDSP_MODIFY_DBG_REG             _IOWR(PDSP_DRIVER_MODULE_ID, 20, pdsp_cmd_params_t)
#define PDSP_DRIVER_PDSP_SHOW_STATUS                _IOWR(PDSP_DRIVER_MODULE_ID, 21, pdsp_cmd_params_t)
#define PDSP_DRIVER_PDSP_PRINT_CTRL_REGS            _IOWR(PDSP_DRIVER_MODULE_ID, 22, pdsp_cmd_params_t)


#ifdef __KERNEL__

/* Success Code */
#define SR_RETCODE_SUCCESS          1

/* PDSP error codes */
#define SRPDSP_ENORES                   -1
#define SRPDSP_EINVCMD                  -2
#define SRPDSP_EINVOPT                  -3
#define SRPDSP_EINVINDEX                -4
#define SRPDSP_EALREADYOPEN             -5
#define SRPDSP_ENOTOPEN                 -6
#define SRPDSP_EMAPERROR                -7
#define SRPDSP_EINVPORT                 -8
#define SRPDSP_EINVPID                  -9
#define SRPDSP_EPAUSELIMITEXCEED        -10
#define SRPDSP_ESESSIONNOTPAUSED        -11
#define SRPDSP_ESESSIONPAUSED           -12
#define SRPDSP_EREOPENINVALID           -13
#define SRPDSP_EINTERROR                -99

// Return codes (duplicate from pdsp_p7.h)
#define PDSP_RET_CODE_IDLE                                  0
#define PDSP_RET_CODE_SUCCESS                               1
#define PDSP_RET_CODE_INVALID_CMD                           2
#define PDSP_RET_CODE_INVALID_OPTION                        3
#define PDSP_RET_CODE_INVALID_INDEX                         4
#define PDSP_RET_CODE_ALREADY_OPEN                          5
#define PDSP_RET_CODE_NOT_OPEN                              6
#define PDSP_RET_CODE_INVALID_PARAM                         7
#define PDSP_RET_CODE_MAPPING_ERROR                         8
#define PDSP_RET_CODE_CH_ALREADY_DISABLE                    9
#define PDSP_RET_CODE_CH_ALREADY_ENABLE                     10
#define PDSP_RET_CODE_INVALID_QUEUE                         11
#define PDSP_RET_CODE_RESET_UNCONFIGURED_PROGRAM            12
#define PDSP_RET_CODE_ADDING_ALREADY_CONFIGURED_PROGRAM     13
#define PDSP_RET_CODE_CHANGING_UNCONFIGURED_PROGRAM         14
#define PDSP_RET_CODE_FATAL_ERR                             99


Int32 pdsp_cmd_send    (pdsp_id_t               id,
                        pdsp_cmd_t              cmd_word,
                        void *wr_ptr,   Uint32  wr_word,
                        void *rd_ptr,   Uint32  rd_word);

typedef enum
{
    PDSPCTRL_HLT      ,
    PDSPCTRL_STEP     ,
    PDSPCTRL_FREERUN  ,
    PDSPCTRL_RESUME   ,
    PDSPCTRL_RST      ,
    PDSPCTRL_START    ,
}
pdsp_ctrl_op_t;
/*
 * pdsp_control -
 *
 * Description: This API provides interface to control PDSPs. Following
 * operations are supported :-
 *  PDSPCTRL_HLT
 *      HALT pdsp execution
 *  PDSPCTRL_STEP
 *      Set PDSP Single step mode. This option halts the PDSP and successive
 *      RESUMEs are carried as single steps.
 *  PDSPCTRL_FREERUN
 *      Set free running mode, i.e., disable single step. PDSP execution is
 *      implicitly RESUMEd as a result of this command.
 *  PDSPCTRL_RESUME
 *      RESUME pdsp execution
 *  PDSPCTRL_RST
 *      RESET PDSP and start execution from specified program counter. The
 *      16-bit program counter shoule be passed by ctl_data pointer.
 *  PDSPCTRL_PSM
 *      Enable or Disable PSM mode. ctl_data should be passed as pointer to
 *      boolean (32-bit integer) flag indicating desired enable (!0) or disable
 *      (0) status of PSM. Note that pdsp_id value is ignored for this option.
 *
 * Note:
 *   Setting option PDSPCTRL_STEP just sets the PDSP in single step
 *  mode and halts its execution, actual single stepping should be performed by
 *  calling this API with PDSPCTRL_RESUME option per step till free
 *  running is enabled explicitly with option PDSPCTRL_FREERUN
 *  single step or halting the pdsp
 *
 * Precondition:
 *  -   ti_ppd_init
 *
 * Parameters:
 *  id (IN)         - Id of PDSP to control: CPDSP(0), MPDSP(1), QPDSP(2).
 *  ctl_op (IN)     - Eiter of the PDSP control options as explained above.
 *  ctl_data (IN)   - Pointer to data corresponding the pdsp control option.
 *
 * Return:
 *  0 on Success, <0 on error.
 */
Int32 pdsp_control (pdsp_id_t   pdsp_id, Uint32 ctl_op, Ptr ctl_data);

#endif

#endif
