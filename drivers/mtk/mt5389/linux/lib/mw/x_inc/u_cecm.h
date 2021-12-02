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
/*! @file u_cecm.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains CEC Manager related definitions, which
 *         are known to application and middleware.
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_CECM_H_
#define _U_CECM_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
#include "u_lnk_list.h"

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_CECM
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* CEC Manager API return values */
#define CECMR_OK                        ((INT32)    0)    /**< Routine successful  */
#define CECMR_NOT_INIT                  ((INT32)   -1)    /**< The CEC Manager is not initialized */
#define CECMR_ALREADY_INIT              ((INT32)   -2)    /**< The CEC Manager already initialized */
#define CECMR_INV_ARG                   ((INT32)   -3)    /**< One or more of the arguments contain invalid data */
#define CECMR_ERROR                     ((INT32)   -4)    /**< Internal error */
#define CECMR_CEC_FCT_DISABLED          ((INT32)   -5)    /**< The  CEC function discable */
#define CECMR_OUT_OF_HANDLE             ((INT32)   -6)    /**< No handle available */
#define CECMR_OUT_OF_MEM                ((INT32)   -7)    /**< Out of memory */
#define CECMR_OUT_OF_RESOURCE           ((INT32)   -8)    /**< Not enough CECM resource */
#define CECMR_HANDLE_FREE_ERROR         ((INT32)   -9)    /**< Free handle error */
#define CECMR_DRV_COMP_ERROR            ((INT32)  -10)    /**< CEC driver operation error  */
#define CECMR_MSG_SEND_ERROR            ((INT32)  -11)    /**< Send message failed  */
#define CECMR_SEMA_ERROR                ((INT32)  -12)    /**< Semaphore operation error  */
#define CECMR_DEV_NOT_AVIL              ((INT32)  -13)    /**< Device is not available */
#define CECMR_CLI_ERROR                 ((INT32)  -14)    /**< CLI operation is failed */

#define CECM_INV_PA                     ((UINT16) 0xdead)    /**< Invalid physical address */
#define CECM_INV_COMP_ID                ((UINT16) 0xdead)    /**< Invalid component id  */
#define CECM_TV_COMP_ID                 ((UINT16) 0xffff)    /**< TV component id       */

#define CECM_MAX_OPERAND_SIZE           ((SIZE_T)  14)    /**< Max operand size */
#define CECM_DEV_VNDR_ID_SIZE           ((SIZE_T)   3)    /**< Device vendor id size  */
#define CECM_OSD_STRING_SIZE            ((SIZE_T)  13)    /**< OSD string size  */
#define CECM_OSD_NAME_SIZE              ((SIZE_T)  15)    /**< OSD name size  */
#define CECM_PROG_TITLE_STRING_SIZE     ((SIZE_T)  15)    /**< Program title string size  */
#define CECM_VNDR_CMD_WITH_ID_SIZE      ((SIZE_T)  11)    /**< Device vendor commond size */
#define CECM_SHORT_AUD_DESCRIPTOR_SIZE  ((SIZE_T)  3)     /**<  Short audio descriptor size */
#define CECM_AUD_FMT_CODE_MAX_SIZE      ((SIZE_T)   4)    /**< Audio format code size */
#define CECM_USER_CTRL_OPERAND_SIZE     ((SIZE_T)   4)    /**< User control operand size */

#define CECM_DD_VNDR_CMD_MAX_SIZE    ((SIZE_T)   5)       /**< Device discovery vendor command size */

typedef UINT8 CECM_DEV_VNDR_ID_T [CECM_DEV_VNDR_ID_SIZE];     /**< Device vendor id */

typedef UINT8 CECM_SHORT_AUD_DESCRIPTOR_T[CECM_SHORT_AUD_DESCRIPTOR_SIZE];    /**< Short audio descriptor  */

/* Definition of CECM notify attribute */
/** CECM notify attribute none */
#define CECM_ATTR_IGNORE                        ((UINT32) 0)         
/** CECM notify attribute used for the opcode is generated by CECM */
#define CECM_ATTR_OPCODE_GEN_BY_CECM            MAKE_BIT_MASK_32(0) 
/** CECM notify attribute used for Request Active Source */
#define CECM_ATTR_REQ_ACT_SRC                   MAKE_BIT_MASK_32(1) 
/** CECM notify attribute used for Give physical address */
#define CECM_ATTR_QUERY_PHY_ADDR                MAKE_BIT_MASK_32(2)   

/** If the CECM notify attribute is opcode is generated by CECM */
#define CECM_IS_OPCODE_GEN_BY_CECM(_attr)       ((_attr & CECM_ATTR_OPCODE_GEN_BY_CECM) == CECM_ATTR_OPCODE_GEN_BY_CECM)    
/** If the CECM notify attribute is request active source*/
#define CECM_IS_REQ_ACT_SRC(_attr)              ((_attr & CECM_ATTR_REQ_ACT_SRC) == CECM_ATTR_REQ_ACT_SRC)    
/** If the CECM notify attribute is give physical address */
#define CECM_IS_QUERY_PHY_ADDR(_attr)           ((_attr & CECM_ATTR_QUERY_PHY_ADDR) == CECM_ATTR_QUERY_PHY_ADDR)   

/* Definition for CECM device record *****************************************/
/* Definition of device update mask */

/** device descivery update mask for physical address */
#define CECM_UPDATE_MASK_PA             MAKE_BIT_MASK_32(0)
/** device descivery update mask for get device vendor ID */
#define CECM_UPDATE_MASK_DEV_VNDR_ID    MAKE_BIT_MASK_32(1)
/** device descivery update mask for get device vendor information */
#define CECM_UPDATE_MASK_DEV_VNDR_INFO  MAKE_BIT_MASK_32(2)
/** device descivery update mask for get CEC version */
#define CECM_UPDATE_MASK_CEC_VER        MAKE_BIT_MASK_32(3)
/** device descivery update mask for get OSD name */
#define CECM_UPDATE_MASK_OSD_NAME       MAKE_BIT_MASK_32(4)
/** device descivery update mask for get power status */
#define CECM_UPDATE_MASK_PWR_STS        MAKE_BIT_MASK_32(5)
/** device descivery update mask none*/
#define CECM_UPDATE_MASK_NONE           ((UINT32) 0)
/** device descivery update mask all*/
#define CECM_UPDATE_MASK_ALL            ((UINT32) (CECM_UPDATE_MASK_PA          | \
                                                   CECM_UPDATE_MASK_DEV_VNDR_ID | \
                                                   CECM_UPDATE_MASK_DEV_VNDR_INFO |\
                                                   CECM_UPDATE_MASK_CEC_VER       |\
                                                   CECM_UPDATE_MASK_OSD_NAME      |\
                                                   CECM_UPDATE_MASK_PWR_STS       ))


/* Definition of set_cec_fct parameters */
/*------------------------------------------------------------------*/
/*! @struct CECM_SET_CEC_FCT_PARAMS_T
 *  @brief  CECM function parameters structure
 *  @code
 *  typedef struct _CECM_SET_CEC_FCT_PARAMS_T
 *  {
 *      BOOL        b_enable;
 *      UINT8       ui1_hdmi_port_num;
 *  }   CECM_SET_CEC_FCT_PARAMS_T;
 *  @endcode
 *  @li@c  b_enable                    - Enable or Disable the CEC function
 *  @li@c  ui1_hdmi_port_num           - HDMI port number
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_SET_CEC_FCT_PARAMS_T
{
    BOOL        b_enable;
    UINT8       ui1_hdmi_port_num;
}   CECM_SET_CEC_FCT_PARAMS_T;


typedef struct _CECM_ARC_CTRL_T
{
    BOOL        b_enable;
}   CECM_ARC_CTRL_T;

/* CEC logical address */
/*------------------------------------------------------------------*/
/*! @enum   CECM_LOG_ADDR_T
 *  @brief  Device logical address
 *  @code
 *  typedef enum
 *  {
 *      CECM_LOG_ADDR_TV = 0,
 *      CECM_LOG_ADDR_REC_DEV_1,
 *      CECM_LOG_ADDR_REC_DEV_2,
 *      CECM_LOG_ADDR_TUNER_1,
 *      CECM_LOG_ADDR_PLAYBACK_DEV_1,
 *      CECM_LOG_ADDR_AUD_SYS,
 *      CECM_LOG_ADDR_TUNER_2,
 *      CECM_LOG_ADDR_TUNER_3,
 *      CECM_LOG_ADDR_PLAYBACK_DEV_2,
 *      CECM_LOG_ADDR_REC_DEV_3,
 *      CECM_LOG_ADDR_TUNER_4,
 *      CECM_LOG_ADDR_PLAYBACK_DEV_3,
 *      CECM_LOG_ADDR_RESERVED_1,
 *      CECM_LOG_ADDR_RESERVED_2,
 *      CECM_LOG_ADDR_FREE_USE,
 *      CECM_LOG_ADDR_UNREGED_BRDCST,
 *      CECM_LOG_ADDR_MAX
 *  }   CECM_LOG_ADDR_T;
 *  @endcode
 *  @li@c  CECM_LOG_ADDR_TV                                  - TV logical address
 *  @li@c  CECM_LOG_ADDR_REC_DEV_1                           - Record device address
 *  @li@c  CECM_LOG_ADDR_REC_DEV_2                           - Record device address
 *  @li@c  CECM_LOG_ADDR_TUNER_1                             - Tuner device address
 *  @li@c  CECM_LOG_ADDR_PLAYBACK_DEV_1                      - Playback device address
 *  @li@c  CECM_LOG_ADDR_AUD_SYS                             - Audio system device address
 *  @li@c  CECM_LOG_ADDR_TUNER_2                             - Tuner device address
 *  @li@c  CECM_LOG_ADDR_TUNER_3                             - Tuner device address
 *  @li@c  CECM_LOG_ADDR_PLAYBACK_DEV_2                      - Playback device address
 *  @li@c  CECM_LOG_ADDR_REC_DEV_3                           - Record device address
 *  @li@c  CECM_LOG_ADDR_TUNER_4                             - Tuner device address
 *  @li@c  CECM_LOG_ADDR_PLAYBACK_DEV_3                      - Playback device address
 *  @li@c  CECM_LOG_ADDR_RESERVED_1                          - Reserved
 *  @li@c  CECM_LOG_ADDR_RESERVED_2                          - Reserved
 *  @li@c  CECM_LOG_ADDR_FREE_USE                            - Free used
 *  @li@c  CECM_LOG_ADDR_UNREGED_BRDCST                      - Broadcast address
 *  @li@c  CECM_LOG_ADDR_MAX                                 - Max address
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_LOG_ADDR_TV = 0,
    CECM_LOG_ADDR_REC_DEV_1,
    CECM_LOG_ADDR_REC_DEV_2,
    CECM_LOG_ADDR_TUNER_1,
    CECM_LOG_ADDR_PLAYBACK_DEV_1,
    CECM_LOG_ADDR_AUD_SYS,
    CECM_LOG_ADDR_TUNER_2,
    CECM_LOG_ADDR_TUNER_3,
    CECM_LOG_ADDR_PLAYBACK_DEV_2,
    CECM_LOG_ADDR_REC_DEV_3,
    CECM_LOG_ADDR_TUNER_4,
    CECM_LOG_ADDR_PLAYBACK_DEV_3,
    CECM_LOG_ADDR_RESERVED_1,
    CECM_LOG_ADDR_RESERVED_2,
    CECM_LOG_ADDR_FREE_USE,
    CECM_LOG_ADDR_UNREGED_BRDCST,
    CECM_LOG_ADDR_MAX
}   CECM_LOG_ADDR_T;

/* CECM notify type */
/*------------------------------------------------------------------*/
/*! @enum   CECM_NFY_TYPE_T
 *  @brief  CECM notify type
 *  @code
 *  typedef enum
 *  {
 *      CECM_NFY_TYPE_OPCODE = 0,
 *      CECM_NFY_TYPE_MSGCODE
 *  }   CECM_NFY_TYPE_T;
 *  @endcode
 *  @li@c  CECM_NFY_TYPE_OPCODE                - notify opration code
 *  @li@c  CECM_NFY_TYPE_MSGCODE               - notify message code
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_NFY_TYPE_OPCODE = 0,
    CECM_NFY_TYPE_MSGCODE
}   CECM_NFY_TYPE_T;

/* CECM notify type */
/*------------------------------------------------------------------*/
/*! @enum    CECM_BRDCST_TYPE_T
 *  @brief   Broadcast type
 *  @code
 *  typedef enum
 *  {
 *      CECM_BRDCST_TYPE_NONE = 0,
 *      CECM_BRDCST_TYPE_DIRECT,
 *      CECM_BRDCST_TYPE_BRDCST
 *  }   CECM_BRDCST_TYPE_T;
 *  @endcode
 *  @li@c  CECM_BRDCST_TYPE_NONE                    - Broadcast none
 *  @li@c  CECM_BRDCST_TYPE_DIRECT                  - Send message directly
 *  @li@c  CECM_BRDCST_TYPE_BRDCST                  - Broadcast the message
 */
/*------------------------------------------------------------------*/

typedef enum
{
    CECM_BRDCST_TYPE_NONE = 0,
    CECM_BRDCST_TYPE_DIRECT,
    CECM_BRDCST_TYPE_BRDCST
}   CECM_BRDCST_TYPE_T;

/* Feature opcode */
/*------------------------------------------------------------------*/
/*! @enum   CECM_OPCODE_T
 *  @brief  The operation code
 *  @code
 *  typedef enum
 *  {                                            
 *      CECM_OPCODE_FEATURE_ABORT        = 0x00, 
 *      CECM_OPCODE_IMG_VIEW_ON          = 0x04, 
 *      CECM_OPCODE_TUNER_STEP_INCR      = 0x05, 
 *      CECM_OPCODE_TUNER_STEP_DECR      = 0x06, 
 *      CECM_OPCODE_TUNER_DEV_STS        = 0x07, 
 *      CECM_OPCODE_GIVE_TUNER_DEV_STS   = 0x08, 
 *      CECM_OPCODE_REC_ON               = 0x09, 
 *      CECM_OPCODE_REC_STS              = 0x0a, 
 *      CECM_OPCODE_REC_OFF              = 0x0b, 
 *      CECM_OPCODE_TXT_VIEW_ON          = 0x0d, 
 *      CECM_OPCODE_REC_TV_SCREEN        = 0x0f, 
 *      CECM_OPCODE_GIVE_DECK_STS        = 0x1a, 
 *      CECM_OPCODE_DECK_STS             = 0x1b, 
 *      CECM_OPCODE_SET_MENU_LANG        = 0x32, 
 *      CECM_OPCODE_CLR_ANA_TIMER        = 0x33, 
 *      CECM_OPCODE_SET_ANA_TIMER        = 0x34, 
 *      CECM_OPCODE_TIMER_STS            = 0x35, 
 *      CECM_OPCODE_STANDBY              = 0x36, 
 *      CECM_OPCODE_PLAY                 = 0x41, 
 *      CECM_OPCODE_DECK_CTRL            = 0x42, 
 *      CECM_OPCODE_TIMER_CLRED_STS      = 0x43, 
 *      CECM_OPCODE_USER_CTRL_PRESSED    = 0x44, 
 *      CECM_OPCODE_USER_CTRL_RELEASED   = 0x45, 
 *      CECM_OPCODE_GIVE_OSD_NAME        = 0x46, 
 *      CECM_OPCODE_SET_OSD_NAME         = 0x47, 
 *      CECM_OPCODE_SET_OSD_STRING       = 0x64, 
 *      CECM_OPCODE_SET_TIMER_PROG_TITLE = 0x67, 
 *      CECM_OPCODE_SYS_AUD_MOD_REQ      = 0x70, 
 *      CECM_OPCODE_GIVE_AUD_STS         = 0x71, 
 *      CECM_OPCODE_SET_SYS_AUD_MOD      = 0x72, 
 *      CECM_OPCODE_REPORT_AUD_STS       = 0x7a, 
 *      CECM_OPCODE_GIVE_SYS_AUD_MOD_STS = 0x7d, 
 *      CECM_OPCODE_SYS_AUD_MOD_STS      = 0x7e, 
 *      CECM_OPCODE_ROUTING_CHG          = 0x80, 
 *      CECM_OPCODE_ROUTING_INFO         = 0x81, 
 *      CECM_OPCODE_ACT_SRC              = 0x82, 
 *      CECM_OPCODE_GIVE_PHY_ADDR        = 0x83, 
 *      CECM_OPCODE_REPORT_PHY_ADDR      = 0x84, 
 *      CECM_OPCODE_REQ_ACT_SRC          = 0x85, 
 *      CECM_OPCODE_SET_STREAM_PATH      = 0x86, 
 *      CECM_OPCODE_DEV_VNDR_ID          = 0x87, 
 *      CECM_OPCODE_VNDR_CMD             = 0x89, 
 *      CECM_OPCODE_VNDR_REMOTE_BTN_DOWN = 0x8a, 
 *      CECM_OPCODE_VNDR_REMOTE_BTN_UP   = 0x8b, 
 *      CECM_OPCODE_GIVE_DEV_VNDR_ID     = 0x8c, 
 *      CECM_OPCODE_MENU_REQ             = 0x8d, 
 *      CECM_OPCODE_MENU_STS             = 0x8e, 
 *      CECM_OPCODE_GIVE_DEV_PWR_STS     = 0x8f, 
 *      CECM_OPCODE_REPORT_PWR_STS       = 0x90, 
 *      CECM_OPCODE_GET_MENU_LANG        = 0x91, 
 *      CECM_OPCODE_SELECT_ANA_SVC       = 0x92, 
 *      CECM_OPCODE_SELECT_DIG_SVC       = 0x93, 
 *      CECM_OPCODE_SET_DIG_TIMER        = 0x97, 
 *      CECM_OPCODE_CLR_DIG_TIMER        = 0x99, 
 *      CECM_OPCODE_SET_AUD_RATE         = 0x9a, 
 *      CECM_OPCODE_INACT_SRC            = 0x9d, 
 *      CECM_OPCODE_CEC_VER              = 0x9e, 
 *      CECM_OPCODE_GET_CEC_VER          = 0x9f, 
 *      CECM_OPCODE_VNDR_CMD_WITH_ID     = 0xa0, 
 *      CECM_OPCODE_CLR_EXT_TIMER        = 0xa1, 
 *      CECM_OPCODE_SET_EXT_TIMER        = 0xa2, 
 *      CECM_OPCODE_REPORT_SAD           = 0xa3, 
 *      CECM_OPCODE_REQUEST_SAD          = 0xa4, 
 *      CECM_OPCODE_INIT_ARC             = 0xc0, 
 *      CECM_OPCODE_REPORT_INIT_ARC      = 0xc1, 
 *      CECM_OPCODE_REPORT_TERMINATE_ARC = 0xc2, 
 *      CECM_OPCODE_REQ_INIT_ARC         = 0xc3, 
 *      CECM_OPCODE_REQ_TERMINATE_ARC    = 0xc4, 
 *      CECM_OPCODE_TERMINATE_ARC        = 0xc5, 
 *      CECM_OPCODE_ABORT                = 0xff, 
 *      CECM_OPCODE_NONE                 = 0xffff 
 *  }   CECM_OPCODE_T;
 *  @endcode
 *  @li@c  CECM_OPCODE_FEATURE_ABORT                         - Used as a response to indicate that the device does not support the message
 *  @li@c  CECM_OPCODE_IMG_VIEW_ON                           - Sent by a source device to the TV
 *  @li@c  CECM_OPCODE_TUNER_STEP_INCR                       - Used to tune to next highest service in a tuner's service list
 *  @li@c  CECM_OPCODE_TUNER_STEP_DECR                       - Used to tune to next lowest service in a tuner's service list
 *  @li@c  CECM_OPCODE_TUNER_DEV_STS                         - Use by a tuner device to provide its status
 *  @li@c  CECM_OPCODE_GIVE_TUNER_DEV_STS                    - Request the status of a tuner device
 *  @li@c  CECM_OPCODE_REC_ON                                - Attempt to record the specified source
 *  @li@c  CECM_OPCODE_REC_STS                               - Inform the initiator of the message Record On about its staus
 *  @li@c  CECM_OPCODE_REC_OFF                               - Requests a device to stop a recording
 *  @li@c  CECM_OPCODE_TXT_VIEW_ON                           - As Image View On but should also remove any text
 *  @li@c  CECM_OPCODE_REC_TV_SCREEN                         - Request by the recording device to record the presently display source 
 *  @li@c  CECM_OPCODE_GIVE_DECK_STS                         - Requst the status of a device
 *  @li@c  CECM_OPCODE_DECK_STS                              - Provide a deck's status
 *  @li@c  CECM_OPCODE_SET_MENU_LANG                         - Set device language
 *  @li@c  CECM_OPCODE_CLR_ANA_TIMER                         - Clear an analogue timer block of a device
 *  @li@c  CECM_OPCODE_SET_ANA_TIMER                         - Set a timer block on an analogue recording device
 *  @li@c  CECM_OPCODE_TIMER_STS                             - Report the timer status
 *  @li@c  CECM_OPCODE_STANDBY                               - Set the divice to standby
 *  @li@c  CECM_OPCODE_PLAY                                  - Control the playback behaviour of a source device
 *  @li@c  CECM_OPCODE_DECK_CTRL                             - Control a deive's media functions
 *  @li@c  CECM_OPCODE_TIMER_CLRED_STS                       - Give the clear timer status 
 *  @li@c  CECM_OPCODE_USER_CTRL_PRESSED                     - Indicate that the user pressed a remote control button
 *  @li@c  CECM_OPCODE_USER_CTRL_RELEASED                    - Indicate that the user released a remote control button
 *  @li@c  CECM_OPCODE_GIVE_OSD_NAME                         - Request preferred OSD name of a device 
 *  @li@c  CECM_OPCODE_SET_OSD_NAME                          - Set the preferred OSD name of a device 
 *  @li@c  CECM_OPCODE_SET_OSD_STRING                        - Send a text to display on TV
 *  @li@c  CECM_OPCODE_SET_TIMER_PROG_TITLE                  - Set timer program title
 *  @li@c  CECM_OPCODE_SYS_AUD_MOD_REQ                       - Request to use system audio mode to the amplifier
 *  @li@c  CECM_OPCODE_GIVE_AUD_STS                          - Request an amplifier to send its volume and mute status
 *  @li@c  CECM_OPCODE_SET_SYS_AUD_MOD                       - Set the system audio mode on or off
 *  @li@c  CECM_OPCODE_REPORT_AUD_STS                        - Report an amplifier's volume and mute status
 *  @li@c  CECM_OPCODE_GIVE_SYS_AUD_MOD_STS                  - Request the status of the system audio mode
 *  @li@c  CECM_OPCODE_SYS_AUD_MOD_STS                       - Report the current status of the system audio mode
 *  @li@c  CECM_OPCODE_ROUTING_CHG                           - Used to inform all other device the active route has changed
 *  @li@c  CECM_OPCODE_ROUTING_INFO                          - Indicate the active route 
 *  @li@c  CECM_OPCODE_ACT_SRC                               - Used by a new source to indicate that it has started to transmit a stream or used in response Request Active Source
 *  @li@c  CECM_OPCODE_GIVE_PHY_ADDR                         - Request device physical address
 *  @li@c  CECM_OPCODE_REPORT_PHY_ADDR                       - Report device physical address
 *  @li@c  CECM_OPCODE_REQ_ACT_SRC                           - Used to discovery the current active source device
 *  @li@c  CECM_OPCODE_SET_STREAM_PATH                       - Used by TV to request a streaming path from the specified physical address
 *  @li@c  CECM_OPCODE_DEV_VNDR_ID                           - Report the vendor ID of device
 *  @li@c  CECM_OPCODE_VNDR_CMD                              - Vendor specific commands 
 *  @li@c  CECM_OPCODE_VNDR_REMOTE_BTN_DOWN                  - Indicates that a remote control button has been depressed
 *  @li@c  CECM_OPCODE_VNDR_REMOTE_BTN_UP                    - Indicates that a remote control button has been released
 *  @li@c  CECM_OPCODE_GIVE_DEV_VNDR_ID                      - Request the vendor of device
 *  @li@c  CECM_OPCODE_MENU_REQ                              - Request show/remove menu or query device currently menu status
 *  @li@c  CECM_OPCODE_MENU_STS                              - Report the device menu status
 *  @li@c  CECM_OPCODE_GIVE_DEV_PWR_STS                      - Request the current power status of device
 *  @li@c  CECM_OPCODE_REPORT_PWR_STS                        - Report the current power status of device
 *  @li@c  CECM_OPCODE_GET_MENU_LANG                         - Get the device currently set menu language
 *  @li@c  CECM_OPCODE_SELECT_ANA_SVC                        - Select an analogue TV service
 *  @li@c  CECM_OPCODE_SELECT_DIG_SVC                        - Select a digital TV service
 *  @li@c  CECM_OPCODE_SET_DIG_TIMER                         - Set a timer block on a digital recording device
 *  @li@c  CECM_OPCODE_CLR_DIG_TIMER                         - Clear a digital timer block of a device
 *  @li@c  CECM_OPCODE_SET_AUD_RATE                          - Control audio rate from source device
 *  @li@c  CECM_OPCODE_INACT_SRC                             - Inform the TV the device has no video to be presented or going into standby
 *  @li@c  CECM_OPCODE_CEC_VER                               - Indicate the supported CEC version
 *  @li@c  CECM_OPCODE_GET_CEC_VER                           - To enquire which version of CEC the device supports
 *  @li@c  CECM_OPCODE_VNDR_CMD_WITH_ID                      - Vendor specific commands with vendor ID
 *  @li@c  CECM_OPCODE_CLR_EXT_TIMER                         - Clear an external timer block of a device
 *  @li@c  CECM_OPCODE_SET_EXT_TIMER                         - Set a timer block record from an external device
 *  @li@c  CECM_OPCODE_REPORT_SAD                            - Report short audio descriptor
 *  @li@c  CECM_OPCODE_REQUEST_SAD                           - Request short audio descriptor
 *  @li@c  CECM_OPCODE_INIT_ARC                              - Initiate audio return channel
 *  @li@c  CECM_OPCODE_REPORT_INIT_ARC                       - Report initiate audio return channel
 *  @li@c  CECM_OPCODE_REPORT_TERMINATE_ARC                  - Report terminate audio return channel
 *  @li@c  CECM_OPCODE_REQ_INIT_ARC                          - Request initiate audio return channel
 *  @li@c  CECM_OPCODE_REQ_TERMINATE_ARC                     - Request terminate audio return channel
 *  @li@c  CECM_OPCODE_TERMINATE_ARC                         - Terminate audio return channel
 *  @li@c  CECM_OPCODE_ABORT                                 - This message is reserved for testing purpose
 *  @li@c  CECM_OPCODE_NONE                                  -
 */
/*------------------------------------------------------------------*/
typedef enum
{                                            /* TV - Dev */
    CECM_OPCODE_FEATURE_ABORT        = 0x00, /* <--> */
    CECM_OPCODE_IMG_VIEW_ON          = 0x04, /* <--- */
    CECM_OPCODE_TUNER_STEP_INCR      = 0x05, /* ---> */
    CECM_OPCODE_TUNER_STEP_DECR      = 0x06, /* ---> */
    CECM_OPCODE_TUNER_DEV_STS        = 0x07, /* <--- */
    CECM_OPCODE_GIVE_TUNER_DEV_STS   = 0x08, /* ---> */
    CECM_OPCODE_REC_ON               = 0x09, /* ---> */
    CECM_OPCODE_REC_STS              = 0x0a, /* <--- */
    CECM_OPCODE_REC_OFF              = 0x0b, /* ---> */
    CECM_OPCODE_TXT_VIEW_ON          = 0x0d, /* <--- */
    CECM_OPCODE_REC_TV_SCREEN        = 0x0f, /* <--- */
    CECM_OPCODE_GIVE_DECK_STS        = 0x1a, /* <--> */
    CECM_OPCODE_DECK_STS             = 0x1b, /* <--> */
    CECM_OPCODE_SET_MENU_LANG        = 0x32, /* <--> */
    CECM_OPCODE_CLR_ANA_TIMER        = 0x33, /* ---> */
    CECM_OPCODE_SET_ANA_TIMER        = 0x34, /* ---> */
    CECM_OPCODE_TIMER_STS            = 0x35, /* <--- */
    CECM_OPCODE_STANDBY              = 0x36, /* <--> */
    CECM_OPCODE_PLAY                 = 0x41, /* ---> */
    CECM_OPCODE_DECK_CTRL            = 0x42, /* ---> */
    CECM_OPCODE_TIMER_CLRED_STS      = 0x43, /* <--- */
    CECM_OPCODE_USER_CTRL_PRESSED    = 0x44, /* <--> */
    CECM_OPCODE_USER_CTRL_RELEASED   = 0x45, /* <--> */
    CECM_OPCODE_GIVE_OSD_NAME        = 0x46, /* <--> */
    CECM_OPCODE_SET_OSD_NAME         = 0x47, /* <--- */
    CECM_OPCODE_SET_OSD_STRING       = 0x64, /* <--- */
    CECM_OPCODE_SET_TIMER_PROG_TITLE = 0x67, /* ---> */
    CECM_OPCODE_SYS_AUD_MOD_REQ      = 0x70, /* ---> */
    CECM_OPCODE_GIVE_AUD_STS         = 0x71, /* ---> */
    CECM_OPCODE_SET_SYS_AUD_MOD      = 0x72, /* <--- */
    CECM_OPCODE_REPORT_AUD_STS       = 0x7a, /* <--- */
    CECM_OPCODE_GIVE_SYS_AUD_MOD_STS = 0x7d, /* ---> */
    CECM_OPCODE_SYS_AUD_MOD_STS      = 0x7e, /* <--- */
    CECM_OPCODE_ROUTING_CHG          = 0x80, /* <--> */
    CECM_OPCODE_ROUTING_INFO         = 0x81, /* <--- */
    CECM_OPCODE_ACT_SRC              = 0x82, /* <--> */
    CECM_OPCODE_GIVE_PHY_ADDR        = 0x83, /* <--> */
    CECM_OPCODE_REPORT_PHY_ADDR      = 0x84, /* <--> */ /* Commanded by CECM */
    CECM_OPCODE_REQ_ACT_SRC          = 0x85, /* <--> */
    CECM_OPCODE_SET_STREAM_PATH      = 0x86, /* <--> */
    CECM_OPCODE_DEV_VNDR_ID          = 0x87, /* <--> */
    CECM_OPCODE_VNDR_CMD             = 0x89, /* <--> */
    CECM_OPCODE_VNDR_REMOTE_BTN_DOWN = 0x8a, /* ---> */
    CECM_OPCODE_VNDR_REMOTE_BTN_UP   = 0x8b, /* ---> */
    CECM_OPCODE_GIVE_DEV_VNDR_ID     = 0x8c, /* <--> */
    CECM_OPCODE_MENU_REQ             = 0x8d, /* ---> */
    CECM_OPCODE_MENU_STS             = 0x8e, /* <--- */
    CECM_OPCODE_GIVE_DEV_PWR_STS     = 0x8f, /* <--> */
    CECM_OPCODE_REPORT_PWR_STS       = 0x90, /* <--> */
    CECM_OPCODE_GET_MENU_LANG        = 0x91, /* <--> */
    CECM_OPCODE_SELECT_ANA_SVC       = 0x92, /* ---> */
    CECM_OPCODE_SELECT_DIG_SVC       = 0x93, /* ---> */
    CECM_OPCODE_SET_DIG_TIMER        = 0x97, /* ---> */
    CECM_OPCODE_CLR_DIG_TIMER        = 0x99, /* ---> */
    CECM_OPCODE_SET_AUD_RATE         = 0x9a, /* ---> */
    CECM_OPCODE_INACT_SRC            = 0x9d, /* <--- */
    CECM_OPCODE_CEC_VER              = 0x9e, /* <--> */
    CECM_OPCODE_GET_CEC_VER          = 0x9f, /* <--> */
    CECM_OPCODE_VNDR_CMD_WITH_ID     = 0xa0, /* <--> */
    CECM_OPCODE_CLR_EXT_TIMER        = 0xa1, /* ---> */
    CECM_OPCODE_SET_EXT_TIMER        = 0xa2, /* ---> */
    CECM_OPCODE_REPORT_SAD           = 0xa3, /* ---> */
    CECM_OPCODE_REQUEST_SAD          = 0xa4, /* ---> */
    CECM_OPCODE_INIT_ARC             = 0xc0, /* ---> */
    CECM_OPCODE_REPORT_INIT_ARC      = 0xc1, /* ---> */
    CECM_OPCODE_REPORT_TERMINATE_ARC = 0xc2, /* ---> */
    CECM_OPCODE_REQ_INIT_ARC         = 0xc3, /* ---> */
    CECM_OPCODE_REQ_TERMINATE_ARC    = 0xc4, /* ---> */
    CECM_OPCODE_TERMINATE_ARC        = 0xc5, /* ---> */
    CECM_OPCODE_ABORT                = 0xff, /* <--> */ /* Commanded by CECM */
    CECM_OPCODE_NONE                 = 0xffff /* For polling message */ /* Commanded by CECM */
}   CECM_OPCODE_T;



/* CEC version */
/*------------------------------------------------------------------*/
/*! @enum    CECM_CEC_VER_T
 *  @brief   The CEC version
 *  @code
 *  typedef enum
 *  {
 *      CECM_CEC_VER_1_1    = 0x00,
 *      CECM_CEC_VER_1_2    = 0x01,
 *      CECM_CEC_VER_1_2A   = 0x02,
 *      CECM_CEC_VER_1_3    = 0x03,
 *      CECM_CEC_VER_1_3A   = 0x04
 *  }   CECM_CEC_VER_T;
 *  @endcode
 *  @li@c  CECM_CEC_VER_1_1                                  - CEC Version 1.1
 *  @li@c  CECM_CEC_VER_1_2                                  - CEC Version 1.2
 *  @li@c  CECM_CEC_VER_1_2A                                 - CEC Version 1.2a
 *  @li@c  CECM_CEC_VER_1_3                                  - CEC Version 1.3
 *  @li@c  CECM_CEC_VER_1_3A                                 - CEC Version 1.3a
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_CEC_VER_1_1     = 0x00,
    CECM_CEC_VER_1_2     = 0x01,
    CECM_CEC_VER_1_2A    = 0x02,
    CECM_CEC_VER_1_3     = 0x03,
    CECM_CEC_VER_1_3A    = 0x04,
    CECM_CEC_VER_1_4A    = 0x05,
    CECM_CEC_VER_INVALID = 0xff
}   CECM_CEC_VER_T;

/* Power Status Feature ******************************************************/
/* Power status */
/*------------------------------------------------------------------*/
/*! @enum    CECM_PWR_STS_T
 *  @brief   Device power status
 *  @code
 *  typedef enum
 *  {
 *      CECM_PWR_STS_ON                             = 0,
 *      CECM_PWR_STS_STANDBY                        = 1,
 *      CECM_PWR_STS_STANDBY_TO_ON                  = 2,
 *      CECM_PWR_STS_ON_TO_STANDBY                  = 3,
 *      CECM_PWR_STS_ON_THEN_ACT_SRC                = 4,
 *      CECM_PWR_STS_1ST_GIVE_DEV_PWR_STS_ACK_OK    = 5,
 *      CECM_PWR_STS_1ST_GIVE_DEV_PWR_STS_NO_ACK    = 6,
 *      CECM_PWR_STS_TIMEOUT                        = 0xfe,
 *      CECM_PWR_STS_ERROR                          = 0xff
 *  }   CECM_PWR_STS_T;
 *  @endcode
 *  @li@c  CECM_PWR_STS_ON                                   - Power status on
 *  @li@c  CECM_PWR_STS_STANDBY                              - Power status standby
 *  @li@c  CECM_PWR_STS_STANDBY_TO_ON                        - Power status standby to on
 *  @li@c  CECM_PWR_STS_ON_TO_STANDBY                        - Power status on to standby
 *  @li@c  CECM_PWR_STS_ON_THEN_ACT_SRC                      - Power status  
 *  @li@c  CECM_PWR_STS_1ST_GIVE_DEV_PWR_STS_ACK_OK          - Request power status received acknowledge
 *  @li@c  CECM_PWR_STS_1ST_GIVE_DEV_PWR_STS_NO_ACK          - Request power status no acknowledge
 *  @li@c  CECM_PWR_STS_TIMEOUT                              - Wait report power status timeout
 *  @li@c  CECM_PWR_STS_ERROR                                - Give power status error
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_PWR_STS_ON                             = 0,
    CECM_PWR_STS_STANDBY                        = 1,
    CECM_PWR_STS_STANDBY_TO_ON                  = 2,
    CECM_PWR_STS_ON_TO_STANDBY                  = 3,
    CECM_PWR_STS_ON_THEN_ACT_SRC                = 4,
    CECM_PWR_STS_1ST_GIVE_DEV_PWR_STS_ACK_OK    = 5,
    CECM_PWR_STS_1ST_GIVE_DEV_PWR_STS_NO_ACK    = 6,
    CECM_PWR_STS_TIMEOUT                        = 0xfe,
    CECM_PWR_STS_ERROR                          = 0xff
}   CECM_PWR_STS_T;

/* Vendor Specific Commands Feature ******************************************/
/** Vendor commands */
typedef UINT8 CECM_VNDR_CMD_T [CECM_MAX_OPERAND_SIZE];  
/** Vendor commands with vendor ID */
typedef UINT8 CECM_VNDR_CMD_WITH_ID_T [CECM_VNDR_CMD_WITH_ID_SIZE];   
/** vendor specific remote control commands  */
typedef UINT8 CECM_VNDR_RC_CODE_T [CECM_MAX_OPERAND_SIZE]; 
/** Vendor specific commands */
typedef UINT8 CECM_SPEC_CMD_T [CECM_MAX_OPERAND_SIZE];  


typedef struct _CECM_ACT_SRC_REVED_T
{
    BOOL        b_act_src_reved;
    UINT16      ui2_pa;
}   CECM_ACT_SRC_REVED_T;


/* Vendor command data */
/*------------------------------------------------------------------*/
/*! @struct    CECM_VNDR_CMD_DATA_T
 *  @brief     Vendor specific commands information
 *  @code
 *  typedef struct _CECM_VNDR_CMD_DATA_T
 *  {
 *      CECM_VNDR_CMD_T         t_vndr_cmd;
 *      SIZE_T                  z_vndr_cmd_size;
 *  }   CECM_VNDR_CMD_DATA_T;
 *  @endcode
 *  @li@c  t_vndr_cmd                                        - Vendor specific command 
 *  @li@c  z_vndr_cmd_size                                   - Vendor command size
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_VNDR_CMD_DATA_T
{
    CECM_VNDR_CMD_T         t_vndr_cmd;
    SIZE_T                  z_vndr_cmd_size;
}   CECM_VNDR_CMD_DATA_T;

/* Specific command data */
/*------------------------------------------------------------------*/
/*! @struct    _CECM_SPEC_CMD_DATA_T
 *  @brief     Vendor specific commands information
 *  @code
 *  typedef struct _CECM_SPEC_CMD_DATA_T
 * {
 *     CECM_SPEC_CMD_T         t_spec_cmd;
 *     SIZE_T                  z_spec_cmd_size;
 * }   CECM_SPEC_CMD_DATA_T;
 *  @endcode
 *  @li@c  t_spec_cmd                                        - Vendor specific command 
 *  @li@c  z_spec_cmd_size                                   - Vendor command size
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_SPEC_CMD_DATA_T
{
    CECM_SPEC_CMD_T         t_spec_cmd;
    SIZE_T                  z_spec_cmd_size;
}   CECM_SPEC_CMD_DATA_T;

/* Vendor command with ID data */
/*------------------------------------------------------------------*/
/*! @struct    CECM_VNDR_CMD_WITH_ID_DATA_T
 *  @brief     Vendor specific command with vendor ID information
 *  @code
 *  typedef struct _CECM_VNDR_CMD_WITH_ID_DATA_T
 *  {
 *      CECM_DEV_VNDR_ID_T          t_dev_vndr_id;
 *      CECM_VNDR_CMD_WITH_ID_T     t_vndr_cmd_with_id;
 *      SIZE_T                      z_vndr_cmd_with_id_size;
 *  }   CECM_VNDR_CMD_WITH_ID_DATA_T;
 *  @endcode
 *  @li@c  t_dev_vndr_id                                     - Vendor ID obtained from IEEE Registration Authority Committee
 *  @li@c  t_vndr_cmd_with_id                                - Vendor specific command 
 *  @li@c  z_vndr_cmd_with_id_size                           - Vendor command with vendor ID size
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_VNDR_CMD_WITH_ID_DATA_T
{
    CECM_DEV_VNDR_ID_T          t_dev_vndr_id;
    CECM_VNDR_CMD_WITH_ID_T     t_vndr_cmd_with_id;
    SIZE_T                      z_vndr_cmd_with_id_size;
}   CECM_VNDR_CMD_WITH_ID_DATA_T;

/* Vendor command data */
/*------------------------------------------------------------------*/
/*! @struct    CECM_VNDR_RC_CODE_DATA_T
 *  @brief     Send vendor specific remote control commands information
 *  @code
 *  typedef struct _CECM_VNDR_RC_CODE_DATA_T
 *  {
 *      CECM_VNDR_RC_CODE_T     t_vndr_rc_code;
 *      SIZE_T                  z_vndr_rc_code_size;
 *  }   CECM_VNDR_RC_CODE_DATA_T;
 *  @endcode
 *  @li@c  t_vndr_rc_code                                    - Remote control code, as defined by the manufacturer
 *  @li@c  z_vndr_rc_code_size                               - Remote control code size
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_VNDR_RC_CODE_DATA_T
{
    CECM_VNDR_RC_CODE_T     t_vndr_rc_code;
    SIZE_T                  z_vndr_rc_code_size;
}   CECM_VNDR_RC_CODE_DATA_T;



/* CECM message code */
/*------------------------------------------------------------------*/
/*! @enum   CECM_MSGCODE_T
 *  @brief  The message code used for notify
 *  @code 
 *  typedef enum
 *  {
 *      CECM_MSGCODE_ACK = 0,
 *      CECM_MSGCODE_WAIT_FOR_OPCODE_TIME_OUT, 
 *      CECM_MSGCODE_DEV_LIST_NFY,
 *      CECM_MSGCODE_ONE_TOUCH_PLAY,
 *      CECM_MSGCODE_PWR_ON_DEV
 *  }   CECM_MSGCODE_T;
 *  @endcode
 *  @li@c  CECM_MSGCODE_ACK                                  - Notify received acknowledge
 *  @li@c  CECM_MSGCODE_WAIT_FOR_OPCODE_TIME_OUT             - Wait responding operation code time out 
 *  @li@c  CECM_MSGCODE_DEV_LIST_NFY                         - Notify device list
 *  @li@c  CECM_MSGCODE_ONE_TOUCH_PLAY                       - Notify One Touch Paly message
 *  @li@c  CECM_MSGCODE_PWR_ON_DEV                           - Notify Power On Device message
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_MSGCODE_ACK = 0,
    CECM_MSGCODE_WAIT_FOR_OPCODE_TIME_OUT, /* Time-out in waiting for responding opcode */
    CECM_MSGCODE_DEV_LIST_NFY,
    CECM_MSGCODE_ONE_TOUCH_PLAY,
    CECM_MSGCODE_PWR_ON_DEV,
    CECM_MSGCODE_5V_PWR,
    CECM_MSGCODE_TV_LA,      /*current TV LA*/
}   CECM_MSGCODE_T;

/* CECM device state */
/*------------------------------------------------------------------*/
/*! @enum   CECM_DEV_STATE_T
 *  @brief  The device state
 *  @code
 *  typedef enum
 *  {
 *      CECM_DEV_STATE_INIT = 0,
 *      CECM_DEV_STATE_UPDATING,
 *      CECM_DEV_STATE_UPDATED
 *  }   CECM_DEV_STATE_T;
 *  @endcode
 *  @li@c  CECM_DEV_STATE_INIT                               - The device state is initiate
 *  @li@c  CECM_DEV_STATE_UPDATING                           - The device is updating
 *  @li@c  CECM_DEV_STATE_UPDATED                            - Updated state
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_DEV_STATE_INIT = 0,
    CECM_DEV_STATE_UPDATING,
    CECM_DEV_STATE_UPDATED
}   CECM_DEV_STATE_T;

/* CECM device list status */
/*------------------------------------------------------------------*/
/*! @enum   CECM_DEV_LIST_STS_T
 *  @brief  Device list status
 *  @code
 *  typedef enum
 *  {
 *      CECM_DEV_LIST_STS_NO_CHANGE = 0,
 *      CECM_DEV_LIST_STS_UPDATED
 *  }   CECM_DEV_LIST_STS_T;
 *  @endcode
 *  @li@c  CECM_DEV_LIST_STS_NO_CHANGE                       - The device list no change
 *  @li@c  CECM_DEV_LIST_STS_UPDATED                         - The device is updated
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_DEV_LIST_STS_NO_CHANGE = 0,
    CECM_DEV_LIST_STS_UPDATED
}   CECM_DEV_LIST_STS_T;

/* CECM device list notify */
/*------------------------------------------------------------------*/
/*! @struct CECM_DEV_LIST_NFY_T
 *  @brief  Notify the device list
 *  @code
 *  typedef struct _CECM_DEV_LIST_NFY_T
 *  {
 *      CECM_DEV_LIST_STS_T     e_dev_lst_sts;
 *      CECM_LOG_ADDR_T         e_start_la;
 *      CECM_LOG_ADDR_T         e_stop_la;
 *  }   CECM_DEV_LIST_NFY_T;
 *  @endcode
 *  @li@c  e_dev_lst_sts                                     - Notify the device state
 *  @li@c  e_start_la                                        - Device list start logical address
 *  @li@c  e_stop_la                                         - Device list stop logical address
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_DEV_LIST_NFY_T
{
    CECM_DEV_LIST_STS_T     e_dev_lst_sts;
    CECM_LOG_ADDR_T         e_start_la;
    CECM_LOG_ADDR_T         e_stop_la;
}   CECM_DEV_LIST_NFY_T;

/* CECM ACK condition */
/*------------------------------------------------------------------*/
/*! @enum   CECM_ACK_COND_T
 *  @brief  The acknowledge type
 *  @code
 *  typedef enum
 *  {
 *      CECM_ACK_COND_OK = 0,
 *      CECM_ACK_COND_NO_RESPONSE
 *  }   CECM_ACK_COND_T;
 *  @endcode
 *  @li@c  CECM_ACK_COND_OK                                  - Recevice the acknowledge from the device
 *  @li@c  CECM_ACK_COND_NO_RESPONSE                         - No response from the device
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_ACK_COND_OK = 0,
    CECM_ACK_COND_NO_RESPONSE
}   CECM_ACK_COND_T;

/*------------------------------------------------------------------*/
/*! @struct   CECM_5V_PWR_INFO_T
 *  @brief    Plug or un-plug HDMI information
 *  @code
 *  typedef struct _CECM_5V_PWR_INFO_T
 *  {
 *      BOOL      b_5v_power;
 *      UINT16    ui2_comp_id;
 *  }   CECM_5V_PWR_INFO_T;
 *  @endcode
 *  @li@c  b_5v_power                      - There is 5v power or not, indicate plug or un-plug HDMI
 *  @li@c  ui2_comp_id                     - The component id
 */
/*------------------------------------------------------------------*/

typedef struct _CECM_5V_PWR_INFO_T
{
    BOOL      b_5v_power;
    UINT16    ui2_comp_id;
} CECM_5V_PWR_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct   CECM_CTRL_T
 *  @brief    CECM control sync info
 *  @code
 *  typedef struct _CECM_CTRL_T
 *  {
 *      BOOL        b_sync;
 }   CECM_CTRL_T;
 *  @endcode
 *  @li@c  b_sync       - Sync operation or not.
 */
/*------------------------------------------------------------------*/

/* CECM control */
typedef struct _CECM_CTRL_T
{
    BOOL        b_sync;
}   CECM_CTRL_T;

/* Device discovery info */
/*------------------------------------------------------------------*/
/*! @struct CECM_DEV_DISCOVERY_T
 *  @brief  Device discovery information
 *  @code
 *  typedef struct _CECM_DEV_DISCOVERY_T
 *  {
 *      CECM_LOG_ADDR_T     e_start_la;
 *      CECM_LOG_ADDR_T     e_stop_la;
 *      UINT32              ui4_itvl;
 *      BOOL                b_batch_polling;
 *  }   CECM_DEV_DISCOVERY_T;
 *  @endcode
 *  @li@c  e_start_la                                        - Device discovery start address
 *  @li@c  e_stop_la                                         - Device discovery stop address
 *  @li@c  ui4_itvl                                          - The interval between the  device discovery
 *  @li@c  b_batch_polling                                   - Indicate the device discovery pattern
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_DEV_DISCOVERY_T
{
    CECM_LOG_ADDR_T     e_start_la;
    CECM_LOG_ADDR_T     e_stop_la;
    UINT32              ui4_itvl;
    BOOL                b_batch_polling;
}   CECM_DEV_DISCOVERY_T;


/* CEC logical address list */
/*------------------------------------------------------------------*/
/*! @struct CECM_LOG_ADD_LST_T
 *  @brief  Logical address list
 *  @code
 *  typedef struct _CECM_LOG_ADD_LST_T
 *  {
 *      UINT8               ui1_num;
 *      CECM_LOG_ADDR_T     ae_la[CECM_LOG_ADDR_MAX];
 *  }   CECM_LOG_ADD_LST_T;
 *  @endcode
 *  @li@c  ui1_num                                           - The logical address num
 *  @li@c  ae_la[CECM_LOG_ADDR_MAX]                          - CECM logical address
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_LOG_ADD_LST_T
{
    UINT8               ui1_num;
    CECM_LOG_ADDR_T     ae_la[CECM_LOG_ADDR_MAX];
}   CECM_LOG_ADD_LST_T;

/* General Feature ***********************************************************/
/* CECM wake-up control */
/*------------------------------------------------------------------*/
/*! @enum    CECM_CEC_FCT_CTRL_T
 *  @brief   CEC function control
 *  @code
 *  typedef enum
 *  {
 *      CECM_CEC_FCT_CTRL_OFF = 0,
 *      CECM_CEC_FCT_CTRL_ON
 *  }   CECM_CEC_FCT_CTRL_T;
 *  @endcode
 *  @li@c  CECM_CEC_FCT_CTRL_OFF                             - CEC function is off
 *  @li@c  CECM_CEC_FCT_CTRL_ON                              - CEC function is on
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_CEC_FCT_CTRL_OFF = 0,
    CECM_CEC_FCT_CTRL_ON
}   CECM_CEC_FCT_CTRL_T;

/*------------------------------------------------------------------*/
/*! @struct    CECM_WAKE_UP_CTRL_T
 *  @brief     CECM wake up information
 *  @code
 *  typedef struct _CECM_WAKE_UP_CTRL_T
 *  {
 *      CECM_CEC_FCT_CTRL_T     t_cec_fct_ctrl;
 *      CECM_CEC_VER_T          e_cec_ver;
 *      ISO_639_LANG_T          t_menu_lang;
 *      CECM_DEV_VNDR_ID_T      t_dev_vndr_id;
 *      BOOL                    b_tv_auto_pwr_on;
 *  }   CECM_WAKE_UP_CTRL_T;
 *  @endcode
 *  @li@c  t_cec_fct_ctrl                                    - Turn on or off the CEC function
 *  @li@c  e_cec_ver                                         - Indicate the CEC Version that the device support
 *  @li@c  t_menu_lang                                       - Indicate the current menu language
 *  @li@c  t_dev_vndr_id                                     - Indicate the device verdor ID
 *  @li@c  b_tv_auto_pwr_on                                  - Auto power on or not
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_WAKE_UP_CTRL_T
{
    CECM_CEC_FCT_CTRL_T     t_cec_fct_ctrl;
    CECM_CEC_VER_T          e_cec_ver;
    ISO_639_LANG_T          t_menu_lang;
    CECM_DEV_VNDR_ID_T      t_dev_vndr_id;
    BOOL                    b_tv_auto_pwr_on;
}   CECM_WAKE_UP_CTRL_T;

/* One Touch Play Feature ****************************************************/
/* One touch play action */
/*------------------------------------------------------------------*/
/*! @enum    CECM_OTP_ACT_T
 *  @brief   The One Touch Play trigger by which message
 *  @code
 *  typedef enum
 *  {
 *      CECM_OTP_ACT_NONE = 0,
 *      CECM_OTP_ACT_IMG_VIEW_ON,
 *      CECM_OTP_ACT_TXT_VIEW_ON
 *  }   CECM_OTP_ACT_T;
 *  @endcode
 *  @li@c  CECM_OTP_ACT_NONE                                 - None 
 *  @li@c  CECM_OTP_ACT_IMG_VIEW_ON                          - Trigger One Touch Play by CECM_OPCODE_IMG_VIEW_ON
 *  @li@c  CECM_OTP_ACT_TXT_VIEW_ON                          - Trigger One Touch Play by CECM_OPCODE_TXT_VIEW_ON
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_OTP_ACT_NONE = 0,
    CECM_OTP_ACT_IMG_VIEW_ON,
    CECM_OTP_ACT_TXT_VIEW_ON
}   CECM_OTP_ACT_T;

/*------------------------------------------------------------------*/
/*! @struct     CECM_ONE_TOUCH_PLAY_T
 *  @brief      One Touch Play information
 *  @code
 *  typedef struct _CECM_ONE_TOUCH_PLAY_T
 *  {
 *      UINT16              ui2_pa;
 *      CECM_LOG_ADDR_T     e_la;
 *      UINT16              ui2_comp_id;
 *      CECM_OTP_ACT_T      e_action;
 *  }   CECM_ONE_TOUCH_PLAY_T;
 *  @endcode
 *  @li@c  ui2_pa                                            - The device physical address
 *  @li@c  e_la                                              - The device logical address
 *  @li@c  ui2_comp_id                                       - The HDMI port ID
 *  @li@c  e_action                                          - Indicate One Touch Play trigger by which message
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_ONE_TOUCH_PLAY_T
{
    UINT16              ui2_pa;
    CECM_LOG_ADDR_T     e_la;
    UINT16              ui2_comp_id;
    CECM_OTP_ACT_T      e_action;
}   CECM_ONE_TOUCH_PLAY_T;

/*------------------------------------------------------------------*/
/*! @struct   CECM_ACT_SRC_T
 *  @brief    Notify active source information
 *  @code
 *  typedef struct _CECM_ACT_SRC_T
 *  {
 *      UINT16              ui2_pa;
 *      CECM_LOG_ADDR_T     e_la;
 *      UINT16              ui2_comp_id;
 *  }   CECM_ACT_SRC_T;
 *  @endcode
 *  @li@c  ui2_pa                                            - The current active source device physical address
 *  @li@c  e_la                                              - The current active source device logical address
 *  @li@c  ui2_comp_id                                       - The HDMI port ID
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_ACT_SRC_T
{
    UINT16              ui2_pa;
    CECM_LOG_ADDR_T     e_la;
    UINT16              ui2_comp_id;
}   CECM_ACT_SRC_T;

/* Routing Control Feature ***************************************************/
/* Routing change info */
/*------------------------------------------------------------------*/
/*! @struct   CECM_ROUTING_CHG_INFO_T
 *  @brief    Routing change information
 *  @code
 *  typedef struct _CECM_ROUTING_CHG_INFO_T
 *  {
 *      UINT16      ui2_orig_pa;
 *      UINT16      ui2_new_pa;
 *  }   CECM_ROUTING_CHG_INFO_T;
 *  @endcode
 *  @li@c  ui2_orig_pa                                       - The original physical address
 *  @li@c  ui2_new_pa                                        - The new physical address
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_ROUTING_CHG_INFO_T
{
    UINT16      ui2_orig_pa;
    UINT16      ui2_new_pa;
}   CECM_ROUTING_CHG_INFO_T;

/* Standby Feature ***********************************************************/

/* One Touch Record Feature **************************************************/
/* Record status info */
/*------------------------------------------------------------------*/
/*! @enum    CECM_REC_STS_INFO_T
 *  @brief   The record device status information
 *  @code
 *  typedef enum
 *  {
 *      CECM_REC_STS_INFO_REC_CUR_SELECTED_SRC                  = 0x01,
 *      CECM_REC_STS_INFO_REC_DIG_SRC                           = 0x02,
 *      CECM_REC_STS_INFO_REC_ANA_SRC                           = 0x03,
 *      CECM_REC_STS_INFO_REC_EXT_INPUT                         = 0x04,
 *      CECM_REC_STS_INFO_NO_REC_UNABLE_TO_REC_DIG_SRC          = 0x05,
 *      CECM_REC_STS_INFO_NO_REC_UNABLE_TO_REC_ANA_SRC          = 0x06,
 *      CECM_REC_STS_INFO_NO_REC_UNABLE_TO_REC_SELECT_REQ_SRC   = 0x07,
 *      CECM_REC_STS_INFO_NO_REC_INV_EXT_PLUG_NUM               = 0x09,
 *      CECM_REC_STS_INFO_NO_REC_INV_EXT_PHY_ADDR               = 0x0A,
 *      CECM_REC_STS_INFO_NO_REC_CA_SYS_NOT_SUPPORTED           = 0x0B,
 *      CECM_REC_STS_INFO_NO_REC_NO_OR_INSUFF_CA_ENTITLEMENTS   = 0x0C,
 *      CECM_REC_STS_INFO_NO_REC_NOT_ALLOWED_TO_COPY_SRC        = 0x0D,
 *      CECM_REC_STS_INFO_NO_REC_NOT_FURTHER_COPIES_ALLOWED     = 0x0E,
 *      CECM_REC_STS_INFO_NO_REC_NO_MEDIA                       = 0x10,
 *      CECM_REC_STS_INFO_NO_REC_PLAYING                        = 0x11,
 *      CECM_REC_STS_INFO_NO_REC_ALREADY_REC                    = 0x12,
 *      CECM_REC_STS_INFO_NO_REC_MEDIA_PROTECTED                = 0x13,
 *      CECM_REC_STS_INFO_NO_REC_NO_SRC_SIGNAL                  = 0x14,
 *      CECM_REC_STS_INFO_NO_REC_MEDIA_PROBLEM                  = 0x15,
 *      CECM_REC_STS_INFO_NO_REC_NOT_ENOUGH_SPACE_AVAL          = 0x16,
 *      CECM_REC_STS_INFO_NO_REC_PARENTAL_LOCK_ON               = 0x17,
 *      CECM_REC_STS_INFO_REC_TERMINATED_NORMALLY               = 0x1A,
 *      CECM_REC_STS_INFO_REC_HAS_ALREADY_TERMINATED            = 0x1B,
 *      CECM_REC_STS_INFO_NO_REC_OTHER_REASON                   = 0x1F
 *  }   CECM_REC_STS_INFO_T;
 *  @endcode
 *  @li@c  CECM_REC_STS_INFO_REC_CUR_SELECTED_SRC                   - Recording currently selected source
 *  @li@c  CECM_REC_STS_INFO_REC_DIG_SRC                            - Recording digital source
 *  @li@c  CECM_REC_STS_INFO_REC_ANA_SRC                            - Recording analog source
 *  @li@c  CECM_REC_STS_INFO_REC_EXT_INPUT                          - Recording external source
 *  @li@c  CECM_REC_STS_INFO_NO_REC_UNABLE_TO_REC_DIG_SRC           - No suitable tuner
 *  @li@c  CECM_REC_STS_INFO_NO_REC_UNABLE_TO_REC_ANA_SRC           - No suitable tuner
 *  @li@c  CECM_REC_STS_INFO_NO_REC_UNABLE_TO_REC_SELECT_REQ_SRC    - Has suitable tuner, but the requested parameters are invalid
 *  @li@c  CECM_REC_STS_INFO_NO_REC_INV_EXT_PLUG_NUM                - Invalid external plug number
 *  @li@c  CECM_REC_STS_INFO_NO_REC_INV_EXT_PHY_ADDR                - Invalid external physical address
 *  @li@c  CECM_REC_STS_INFO_NO_REC_CA_SYS_NOT_SUPPORTED            - CA system not supported
 *  @li@c  CECM_REC_STS_INFO_NO_REC_NO_OR_INSUFF_CA_ENTITLEMENTS    - No or insufficient CA entitlements
 *  @li@c  CECM_REC_STS_INFO_NO_REC_NOT_ALLOWED_TO_COPY_SRC         - Not allowed to copy source
 *  @li@c  CECM_REC_STS_INFO_NO_REC_NOT_FURTHER_COPIES_ALLOWED      - No further copies allowed
 *  @li@c  CECM_REC_STS_INFO_NO_REC_NO_MEDIA                        - No media
 *  @li@c  CECM_REC_STS_INFO_NO_REC_PLAYING                         - Playing
 *  @li@c  CECM_REC_STS_INFO_NO_REC_ALREADY_REC                     - Already recording
 *  @li@c  CECM_REC_STS_INFO_NO_REC_MEDIA_PROTECTED                 - Media protected
 *  @li@c  CECM_REC_STS_INFO_NO_REC_NO_SRC_SIGNAL                   - No source signal
 *  @li@c  CECM_REC_STS_INFO_NO_REC_MEDIA_PROBLEM                   - Media problem
 *  @li@c  CECM_REC_STS_INFO_NO_REC_NOT_ENOUGH_SPACE_AVAL           - Not enough space available
 *  @li@c  CECM_REC_STS_INFO_NO_REC_PARENTAL_LOCK_ON                - Parental lock
 *  @li@c  CECM_REC_STS_INFO_REC_TERMINATED_NORMALLY                - Recording terminated normally
 *  @li@c  CECM_REC_STS_INFO_REC_HAS_ALREADY_TERMINATED             - Recording has already terminated
 *  @li@c  CECM_REC_STS_INFO_NO_REC_OTHER_REASON                    - Other reason
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_REC_STS_INFO_REC_CUR_SELECTED_SRC                  = 0x01,
    CECM_REC_STS_INFO_REC_DIG_SRC                           = 0x02,
    CECM_REC_STS_INFO_REC_ANA_SRC                           = 0x03,
    CECM_REC_STS_INFO_REC_EXT_INPUT                         = 0x04,
    CECM_REC_STS_INFO_NO_REC_UNABLE_TO_REC_DIG_SRC          = 0x05,
    CECM_REC_STS_INFO_NO_REC_UNABLE_TO_REC_ANA_SRC          = 0x06,
    CECM_REC_STS_INFO_NO_REC_UNABLE_TO_REC_SELECT_REQ_SRC   = 0x07,
    CECM_REC_STS_INFO_NO_REC_INV_EXT_PLUG_NUM               = 0x09,
    CECM_REC_STS_INFO_NO_REC_INV_EXT_PHY_ADDR               = 0x0A,
    CECM_REC_STS_INFO_NO_REC_CA_SYS_NOT_SUPPORTED           = 0x0B,
    CECM_REC_STS_INFO_NO_REC_NO_OR_INSUFF_CA_ENTITLEMENTS   = 0x0C,
    CECM_REC_STS_INFO_NO_REC_NOT_ALLOWED_TO_COPY_SRC        = 0x0D,
    CECM_REC_STS_INFO_NO_REC_NOT_FURTHER_COPIES_ALLOWED     = 0x0E,
    CECM_REC_STS_INFO_NO_REC_NO_MEDIA                       = 0x10,
    CECM_REC_STS_INFO_NO_REC_PLAYING                        = 0x11,
    CECM_REC_STS_INFO_NO_REC_ALREADY_REC                    = 0x12,
    CECM_REC_STS_INFO_NO_REC_MEDIA_PROTECTED                = 0x13,
    CECM_REC_STS_INFO_NO_REC_NO_SRC_SIGNAL                  = 0x14,
    CECM_REC_STS_INFO_NO_REC_MEDIA_PROBLEM                  = 0x15,
    CECM_REC_STS_INFO_NO_REC_NOT_ENOUGH_SPACE_AVAL          = 0x16,
    CECM_REC_STS_INFO_NO_REC_PARENTAL_LOCK_ON               = 0x17,
    CECM_REC_STS_INFO_REC_TERMINATED_NORMALLY               = 0x1A,
    CECM_REC_STS_INFO_REC_HAS_ALREADY_TERMINATED            = 0x1B,
    CECM_REC_STS_INFO_NO_REC_OTHER_REASON                   = 0x1F
}   CECM_REC_STS_INFO_T;

/* Analog broadcast type */
/*------------------------------------------------------------------*/
/*! @enum    CECM_ANA_BRDCST_T
 *  @brief   Analog broadcast type
 *  @code
 *  typedef enum
 *  {
 *      CECM_ANA_BRDCST_CABLE       = 0x00,
 *      CECM_ANA_BRDCST_SATELLITE   = 0x01,
 *      CECM_ANA_BRDCST_TERRSATRIAL = 0x02
 *  }   CECM_ANA_BRDCST_T;
 *  @endcode
 *  @li@c  CECM_ANA_BRDCST_CABLE                             - Broadcast type cable
 *  @li@c  CECM_ANA_BRDCST_SATELLITE                         - Broadcast type statellite
 *  @li@c  CECM_ANA_BRDCST_TERRSATRIAL                       - Broadcast type terrsatrial
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_ANA_BRDCST_CABLE       = 0x00,
    CECM_ANA_BRDCST_SATELLITE   = 0x01,
    CECM_ANA_BRDCST_TERRSATRIAL = 0x02
}   CECM_ANA_BRDCST_T;

/* Digital identification method */
/*------------------------------------------------------------------*/
/*! @enum    CECM_DIG_ID_METHOD_T
 *  @brief   Digital identification method
 *  @code
 *  typedef enum
 *  {
 *      CECM_DIG_ID_METHOD_IDS      = 0,
 *      CECM_DIG_ID_METHOD_CHANNEL  = 1
 *  }   CECM_DIG_ID_METHOD_T;
 *  @endcode
 *  @li@c  CECM_DIG_ID_METHOD_IDS                            - Indicates a service is identified by digital service IDs
 *  @li@c  CECM_DIG_ID_METHOD_CHANNEL                        - Indicates a service is identified by a logical or virtual channel number
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_DIG_ID_METHOD_IDS      = 0,
    CECM_DIG_ID_METHOD_CHANNEL  = 1
}   CECM_DIG_ID_METHOD_T;

/* Digital broadcast system */
/*------------------------------------------------------------------*/
/*! @enum    CECM_DIG_BRDCST_T
 *  @brief   Digital broadcast system
 *  @code
 *  typedef enum
 *  {
 *      CECM_DIG_BRDCST_ARIB_GEN            = 0x00,
 *      CECM_DIG_BRDCST_ATSC_GEN            = 0x01,
 *      CECM_DIG_BRDCST_DVB_GEN             = 0x02,
 *      CECM_DIG_BRDCST_ARIB_BS             = 0x08,
 *      CECM_DIG_BRDCST_ARIB_CS             = 0x09,
 *      CECM_DIG_BRDCST_ARIB_T              = 0x0A,
 *      CECM_DIG_BRDCST_ATSC_CABLE          = 0x10,
 *      CECM_DIG_BRDCST_ATSC_SATELLITE      = 0x11,
 *      CECM_DIG_BRDCST_ATSC_TERRESTRIAL    = 0x12,
 *      CECM_DIG_BRDCST_DVB_C               = 0x18,
 *      CECM_DIG_BRDCST_DVB_S               = 0x19,
 *      CECM_DIG_BRDCST_DVB_S2              = 0x1A,
 *      CECM_DIG_BRDCST_DVB_T               = 0x1B
 *  }   CECM_DIG_BRDCST_T;
 *  @endcode
 *  @li@c  CECM_DIG_BRDCST_ARIB_GEN                          - Digital broadcast system ARIB GEN
 *  @li@c  CECM_DIG_BRDCST_ATSC_GEN                          - Digital broadcast system ATSC GEN
 *  @li@c  CECM_DIG_BRDCST_DVB_GEN                           - Digital broadcast system DVB GEN
 *  @li@c  CECM_DIG_BRDCST_ARIB_BS                           - Digital broadcast system ARIB BS
 *  @li@c  CECM_DIG_BRDCST_ARIB_CS                           - Digital broadcast system AIRB BS
 *  @li@c  CECM_DIG_BRDCST_ARIB_T                            - Digital broadcast system AIRB CS
 *  @li@c  CECM_DIG_BRDCST_ATSC_CABLE                        - Digital broadcast system ATSC CABLE
 *  @li@c  CECM_DIG_BRDCST_ATSC_SATELLITE                    - Digital broadcast system ATSC STATELLITE
 *  @li@c  CECM_DIG_BRDCST_ATSC_TERRESTRIAL                  - Digital broadcast system ATSC TERRESTRIAL
 *  @li@c  CECM_DIG_BRDCST_DVB_C                             - Digital broadcast system DVB C
 *  @li@c  CECM_DIG_BRDCST_DVB_S                             - Digital broadcast system DVB S
 *  @li@c  CECM_DIG_BRDCST_DVB_S2                            - Digital broadcast system DVB S2
 *  @li@c  CECM_DIG_BRDCST_DVB_T                             - Digital broadcast system DVT T
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_DIG_BRDCST_ARIB_GEN            = 0x00,
    CECM_DIG_BRDCST_ATSC_GEN            = 0x01,
    CECM_DIG_BRDCST_DVB_GEN             = 0x02,
    CECM_DIG_BRDCST_ARIB_BS             = 0x08,
    CECM_DIG_BRDCST_ARIB_CS             = 0x09,
    CECM_DIG_BRDCST_ARIB_T              = 0x0A,
    CECM_DIG_BRDCST_ATSC_CABLE          = 0x10,
    CECM_DIG_BRDCST_ATSC_SATELLITE      = 0x11,
    CECM_DIG_BRDCST_ATSC_TERRESTRIAL    = 0x12,
    CECM_DIG_BRDCST_DVB_C               = 0x18,
    CECM_DIG_BRDCST_DVB_S               = 0x19,
    CECM_DIG_BRDCST_DVB_S2              = 0x1A,
    CECM_DIG_BRDCST_DVB_T               = 0x1B
}   CECM_DIG_BRDCST_T;

/* Analog service info */
/*------------------------------------------------------------------*/
/*! @struct    CECM_ANA_SVC_T
 *  @brief     Analog service information
 *  @code
 *  typedef struct _CECM_ANA_SVC_T
 *  {
 *      CECM_ANA_BRDCST_T       e_ana_brdcst_type;
 *      UINT16                  ui2_freq;
 *      UINT8                   ui1_brdcst_sys;
 *  }   CECM_ANA_SVC_T;
 *  @endcode
 *  @li@c  e_ana_brdcst_type                                 - Analog broadcast type
 *  @li@c  ui2_freq                                          - The frequence
 *  @li@c  ui1_brdcst_sys                                    - Broadcast system
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_ANA_SVC_T
{
    CECM_ANA_BRDCST_T       e_ana_brdcst_type;
    UINT16                  ui2_freq;
    UINT8                   ui1_brdcst_sys;
}   CECM_ANA_SVC_T;

/* Digital service info (i.e. digital service identification) */
/*------------------------------------------------------------------*/
/*! @struct    CECM_DIG_SVC_T
 *  @brief     Digital service information
 *  @code
 *  typedef struct _CECM_DIG_SVC_T
 *  {
 *      CECM_DIG_ID_METHOD_T    e_dig_id_method;
 *      CECM_DIG_BRDCST_T       e_dig_brdcst_sys;
 *      UINT8                   aui1_svc_id[6];
 *  }   CECM_DIG_SVC_T;
 *  @endcode
 *  @li@c  e_dig_id_method                                   - Digital identification method
 *  @li@c  e_dig_brdcst_sys                                  - Digital broadcast system
 *  @li@c  aui1_svc_id[6]                                    - Service ID
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_DIG_SVC_T
{
    CECM_DIG_ID_METHOD_T    e_dig_id_method;
    CECM_DIG_BRDCST_T       e_dig_brdcst_sys;
    UINT8                   aui1_svc_id[6];
}   CECM_DIG_SVC_T;

/* Record soruce type */
/*------------------------------------------------------------------*/
/*! @enum    CECM_REC_SRC_TYPE_T
 *  @brief   Record source type
 *  @code
 *  typedef enum
 *  {
 *      CECM_REC_SRC_TYPE_OWN_SRC        = 1,
 *      CECM_REC_SRC_TYPE_DIG_SVC        = 2,
 *      CECM_REC_SRC_TYPE_ANA_SVC        = 3,
 *      CECM_REC_SRC_TYPE_EXT_PLUG       = 4,
 *      CECM_REC_SRC_TYPE_EXT_PHY_ADDR   = 5
 *  }   CECM_REC_SRC_TYPE_T;
 *  @endcode
 *  @li@c  CECM_REC_SRC_TYPE_OWN_SRC                         - Record own source
 *  @li@c  CECM_REC_SRC_TYPE_DIG_SVC                         - Record digital service
 *  @li@c  CECM_REC_SRC_TYPE_ANA_SVC                         - Record analog service
 *  @li@c  CECM_REC_SRC_TYPE_EXT_PLUG                        - External source device plug
 *  @li@c  CECM_REC_SRC_TYPE_EXT_PHY_ADDR                    - External source device physical address
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_REC_SRC_TYPE_OWN_SRC        = 1,
    CECM_REC_SRC_TYPE_DIG_SVC        = 2,
    CECM_REC_SRC_TYPE_ANA_SVC        = 3,
    CECM_REC_SRC_TYPE_EXT_PLUG       = 4,
    CECM_REC_SRC_TYPE_EXT_PHY_ADDR   = 5
}   CECM_REC_SRC_TYPE_T;

/* Record source */
/*------------------------------------------------------------------*/
/*! @struct    CECM_REC_SRC_T
 *  @brief     The record source information
 *  @code
 *  typedef struct _CECM_REC_SRC_T
 *  {
 *      CECM_REC_SRC_TYPE_T     e_rec_src_type;
 *      union
 *      {
 *          CECM_ANA_SVC_T      t_ana_svc;
 *          CECM_DIG_SVC_T      t_dig_svc;
 *          UINT8               ui1_plug;
 *          UINT16              ui2_phy_addr;
 *      }   u;
 *  }   CECM_REC_SRC_T;
 *  @endcode
 *  @li@c  e_rec_src_type                                    - The record source type
 *  @li@c  t_ana_svc                                         - Analog service information
 *  @li@c  t_dig_svc                                         - Digital service information
 *  @li@c  ui1_plug                                          - The record source device plug
 *  @li@c  ui2_phy_addr                                      - The record source device physical address
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_REC_SRC_T
{
    CECM_REC_SRC_TYPE_T     e_rec_src_type;

    union
    {
        CECM_ANA_SVC_T      t_ana_svc;
        CECM_DIG_SVC_T      t_dig_svc;
        UINT8               ui1_plug;
        UINT16              ui2_phy_addr;
    }   u;
}   CECM_REC_SRC_T;

/* Timer Programming Feature *************************************************/

/** Timer programming program title information  */
typedef UTF16_T CECM_PROG_TITLE_T [CECM_PROG_TITLE_STRING_SIZE];

/* Timer device type */
/*------------------------------------------------------------------*/
/*! @enum    CECM_TIMER_DEV_T
 *  @brief   Timer device type
 *  @code
 *  typedef enum
 *  {
 *      CECM_TIMER_DEV_ANA,
 *      CECM_TIMER_DEV_DIG,
 *      CECM_TIMER_DEV_EXT
 *  }   CECM_TIMER_DEV_T;
 *  @endcode
 *  @li@c  CECM_TIMER_DEV_ANA                                - Analog device
 *  @li@c  CECM_TIMER_DEV_DIG                                - Digital device
 *  @li@c  CECM_TIMER_DEV_EXT                                - External device
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_TIMER_DEV_ANA,
    CECM_TIMER_DEV_DIG,
    CECM_TIMER_DEV_EXT
}   CECM_TIMER_DEV_T;

/* Analog device info */
/*------------------------------------------------------------------*/
/*! @struct    CECM_TIMER_ANA_T
 *  @brief     Analog device information
 *  @code
 *  typedef struct _CECM_TIMER_ANA_T
 *  {
 *      CECM_ANA_BRDCST_T   e_ana_brdcst_type;
 *      UINT16              ui2_freq;
 *      UINT8               ui1_brdcst_sys;
 *  }   CECM_TIMER_ANA_T;
 *  @endcode
 *  @li@c  e_ana_brdcst_type                                 - Analog broadcast type
 *  @li@c  ui2_freq                                          - The frequence
 *  @li@c  ui1_brdcst_sys                                    - Brodacast system
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_TIMER_ANA_T
{
    CECM_ANA_BRDCST_T   e_ana_brdcst_type;
    UINT16              ui2_freq;
    UINT8               ui1_brdcst_sys;
}   CECM_TIMER_ANA_T;

/*------------------------------------------------------------------*/
/*! @enum     CECM_TIMER_EXT_SRC_SPECFER_T
 *  @brief    Indicate the source device is specific by plug or phisical address
 *  @code
 *  typedef enum
 *  {
 *      CECM_TIMER_EXT_SRC_SPECFER_EXT_PLUG     = 4,
 *      CECM_TIMER_EXT_SRC_SPECFER_EXT_PHY_ADDR = 5
 *  }   CECM_TIMER_EXT_SRC_SPECFER_T;
 *  @endcode
 *  @li@c  CECM_TIMER_EXT_SRC_SPECFER_EXT_PLUG               - The source device specific by plug
 *  @li@c  CECM_TIMER_EXT_SRC_SPECFER_EXT_PHY_ADDR           - The source device specific by physical address
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_TIMER_EXT_SRC_SPECFER_EXT_PLUG     = 4,
    CECM_TIMER_EXT_SRC_SPECFER_EXT_PHY_ADDR = 5
}   CECM_TIMER_EXT_SRC_SPECFER_T;

/* External device info */
/*------------------------------------------------------------------*/
/*! @struct   CECM_TIMER_EXT_T
 *  @brief    External device information
 *  @code
 *  typedef struct _CECM_TIMER_EXT_T
 *  {
 *      CECM_TIMER_EXT_SRC_SPECFER_T    e_src_specfer;
 *      UINT16                          ui2_phy_addr_or_plug;
 *  }   CECM_TIMER_EXT_T;
 *  @endcode
 *  @li@c  e_src_specfer                                     - Indicate the source device is specific by plug or phisical address
 *  @li@c  ui2_phy_addr_or_plug                              - The device physical address or plug
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_TIMER_EXT_T
{
    CECM_TIMER_EXT_SRC_SPECFER_T    e_src_specfer;
    UINT16                          ui2_phy_addr_or_plug;
}   CECM_TIMER_EXT_T;

/* Timer info */
/*------------------------------------------------------------------*/
/*! @struct    CECM_TIMER_INFO_T
 *  @brief     The timer information
 *  @code
 *  typedef struct _CECM_TIMER_INFO_T
 *  {
 *      CECM_TIMER_DEV_T        e_dev;
 *      UINT8                   ui1_day_of_month;
 *      UINT8                   ui1_month_of_year;
 *      UINT16                  ui2_start_time;
 *      UINT16                  ui2_duration;
 *      UINT8                   ui1_rec_sequence;
 *      union
 *      {
 *          CECM_TIMER_ANA_T    t_ana_info;
 *          UINT8               aui1_dig_info[7];
 *          CECM_TIMER_EXT_T    t_ext_info;
 *      }   u;
 *  }   CECM_TIMER_INFO_T;
 *  @endcode
 *  @li@c  e_dev                                             - The timer device type
 *  @li@c  ui1_day_of_month                                  - Date
 *  @li@c  ui1_month_of_year                                 - Month
 *  @li@c  ui2_start_time                                    - Start time
 *  @li@c  ui2_duration                                      - Duration time
 *  @li@c  ui1_rec_sequence                                  - The record sequence
 *  @li@c  t_ana_info                                        - Analog device information
 *  @li@c  aui1_dig_info[7]                                  - Digital device information
 *  @li@c  t_ext_info                                        - External device information
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_TIMER_INFO_T
{
    CECM_TIMER_DEV_T        e_dev;
    UINT8                   ui1_day_of_month;
    UINT8                   ui1_month_of_year;
    UINT16                  ui2_start_time;
    UINT16                  ui2_duration;
    UINT8                   ui1_rec_sequence;

    union
    {
        CECM_TIMER_ANA_T    t_ana_info;
        UINT8               aui1_dig_info[7];
        CECM_TIMER_EXT_T    t_ext_info;
    }   u;
}   CECM_TIMER_INFO_T;

/* Timer cleared status data */
/*------------------------------------------------------------------*/
/*! @enum    CECM_TIMER_CLRED_STS_T
 *  @brief   Timer cleared status data
 *  @code
 *  typedef enum
 *  {
 *      CECM_TIMER_CLRED_STS_RECORDING      = 0x00,
 *      CECM_TIMER_CLRED_STS_NO_MATCHING    = 0x01,
 *      CECM_TIMER_CLRED_STS_NO_INFO_AVIL   = 0x02,
 *      CECM_TIMER_CLRED_STS_CLEARED        = 0x80
 *  }   CECM_TIMER_CLRED_STS_T;
 *  @endcode
 *  @li@c  CECM_TIMER_CLRED_STS_RECORDING                    - Timer not cleared, it is recording
 *  @li@c  CECM_TIMER_CLRED_STS_NO_MATCHING                  - Timer not cleared, no matching timer
 *  @li@c  CECM_TIMER_CLRED_STS_NO_INFO_AVIL                 - Timer not cleared, no information available
 *  @li@c  CECM_TIMER_CLRED_STS_CLEARED                      - Timer cleared
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_TIMER_CLRED_STS_RECORDING      = 0x00,
    CECM_TIMER_CLRED_STS_NO_MATCHING    = 0x01,
    CECM_TIMER_CLRED_STS_NO_INFO_AVIL   = 0x02,
    CECM_TIMER_CLRED_STS_CLEARED        = 0x80
}   CECM_TIMER_CLRED_STS_T;

/* Timer overlap warnning */
/*------------------------------------------------------------------*/
/*! @enum    CECM_TIMER_OVERLAP_T
 *  @brief   Indicates if there is another timer block already set which overlaps with this new recording request
 *  @code
 *  typedef enum
 *  {
 *      CECM_TIMER_OVERLAP_NO           = 0,
 *      CECM_TIMER_OVERLAP_TIMER_BLOCKS = 1
 *  }   CECM_TIMER_OVERLAP_T;
 *  @endcode
 *  @li@c  CECM_TIMER_OVERLAP_NO                             - No overlap
 *  @li@c  CECM_TIMER_OVERLAP_TIMER_BLOCKS                   - Timer blocks overlap
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_TIMER_OVERLAP_NO           = 0,
    CECM_TIMER_OVERLAP_TIMER_BLOCKS = 1
}   CECM_TIMER_OVERLAP_T;

/* Media info */
/*------------------------------------------------------------------*/
/*! @enum    CECM_TIMER_MEDIA_INFO_T
 *  @brief   Media information, indicates if removable media is present and its write protect state
 *  @code
 *  typedef enum
 *  {
 *      CECM_TIMER_MEDIA_INFO_PRESET_NOT_PROTECTED  = 0,
 *      CECM_TIMER_MEDIA_INFO_PRESET_PROTECTED      = 1,
 *      CECM_TIMER_MEDIA_INFO_NOT_PRESET            = 2,
 *      CECM_TIMER_MEDIA_INFO_FUTURE_USE            = 3
 *  }   CECM_TIMER_MEDIA_INFO_T;
 *  @endcode
 *  @li@c  CECM_TIMER_MEDIA_INFO_PRESET_NOT_PROTECTED        - Media present and not protected
 *  @li@c  CECM_TIMER_MEDIA_INFO_PRESET_PROTECTED            - Media present bu protected
 *  @li@c  CECM_TIMER_MEDIA_INFO_NOT_PRESET                  - Media not present
 *  @li@c  CECM_TIMER_MEDIA_INFO_FUTURE_USE                  - Future used
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_TIMER_MEDIA_INFO_PRESET_NOT_PROTECTED  = 0,
    CECM_TIMER_MEDIA_INFO_PRESET_PROTECTED      = 1,
    CECM_TIMER_MEDIA_INFO_NOT_PRESET            = 2,
    CECM_TIMER_MEDIA_INFO_FUTURE_USE            = 3
}   CECM_TIMER_MEDIA_INFO_T;

/* Programmed indicator */
/*------------------------------------------------------------------*/
/*! @enum    CECM_TIMER_PROG_IND_T
 *  @brief   Selector for [Timer Programmed Info]
 *  @code
 *  typedef enum
 *  {
 *      CECM_TIMER_PROG_IND_NOT_PROGRAMMED  = 0,
 *      CECM_TIMER_PROG_IND_PROGRAMMED      = 1
 *  }   CECM_TIMER_PROG_IND_T;
 *  @endcode
 *  @li@c  CECM_TIMER_PROG_IND_NOT_PROGRAMMED                - Not programmed
 *  @li@c  CECM_TIMER_PROG_IND_PROGRAMMED                    - Programmed
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_TIMER_PROG_IND_NOT_PROGRAMMED  = 0,
    CECM_TIMER_PROG_IND_PROGRAMMED      = 1
}   CECM_TIMER_PROG_IND_T;

/* Programmed info */
/*------------------------------------------------------------------*/
/*! @enum    CECM_TIMER_PROG_INFO_T
 *  @brief   Information indicating any non-fatal issues with the programming request
 *  @code
 *  typedef enum
 *  {
 *      CECM_TIMER_PROG_INFO_FUTURE_USE_0               = 0,
 *      CECM_TIMER_PROG_INFO_FUTURE_USE_1               = 1,
 *      CECM_TIMER_PROG_INFO_FUTURE_USE_2               = 2,
 *      CECM_TIMER_PROG_INFO_FUTURE_USE_3               = 3,
 *      CECM_TIMER_PROG_INFO_FUTURE_USE_4               = 4,
 *      CECM_TIMER_PROG_INFO_FUTURE_USE_5               = 5,
 *      CECM_TIMER_PROG_INFO_FUTURE_USE_6               = 6,
 *      CECM_TIMER_PROG_INFO_FUTURE_USE_7               = 7,
 *      CECM_TIMER_PROG_INFO_ENOUGH_SPACE               = 8,
 *      CECM_TIMER_PROG_INFO_NOT_ENOUGH_SPACE           = 9,
 *      CECM_TIMER_PROG_INFO_MAY_NOT_BE_ENOUGH_SPACE    = 11,
 *      CECM_TIMER_PROG_INFO_NO_MEDIA_INFO_AVIL         = 10
 *  }   CECM_TIMER_PROG_INFO_T;
 *  @endcode
 *  @li@c  CECM_TIMER_PROG_INFO_FUTURE_USE_0                 - Reserved for feature used
 *  @li@c  CECM_TIMER_PROG_INFO_FUTURE_USE_1                 - Reserved for feature used
 *  @li@c  CECM_TIMER_PROG_INFO_FUTURE_USE_2                 - Reserved for feature used
 *  @li@c  CECM_TIMER_PROG_INFO_FUTURE_USE_3                 - Reserved for feature used
 *  @li@c  CECM_TIMER_PROG_INFO_FUTURE_USE_4                 - Reserved for feature used
 *  @li@c  CECM_TIMER_PROG_INFO_FUTURE_USE_5                 - Reserved for feature used
 *  @li@c  CECM_TIMER_PROG_INFO_FUTURE_USE_6                 - Reserved for feature used
 *  @li@c  CECM_TIMER_PROG_INFO_FUTURE_USE_7                 - Reserved for feature used
 *  @li@c  CECM_TIMER_PROG_INFO_ENOUGH_SPACE                 - Enough space available for recording
 *  @li@c  CECM_TIMER_PROG_INFO_NOT_ENOUGH_SPACE             - Not enough space available for recording
 *  @li@c  CECM_TIMER_PROG_INFO_MAY_NOT_BE_ENOUGH_SPACE      - May not be enough space available
 *  @li@c  CECM_TIMER_PROG_INFO_NO_MEDIA_INFO_AVIL           - No media info available
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_TIMER_PROG_INFO_FUTURE_USE_0               = 0,
    CECM_TIMER_PROG_INFO_FUTURE_USE_1               = 1,
    CECM_TIMER_PROG_INFO_FUTURE_USE_2               = 2,
    CECM_TIMER_PROG_INFO_FUTURE_USE_3               = 3,
    CECM_TIMER_PROG_INFO_FUTURE_USE_4               = 4,
    CECM_TIMER_PROG_INFO_FUTURE_USE_5               = 5,
    CECM_TIMER_PROG_INFO_FUTURE_USE_6               = 6,
    CECM_TIMER_PROG_INFO_FUTURE_USE_7               = 7,
    CECM_TIMER_PROG_INFO_ENOUGH_SPACE               = 8,
    CECM_TIMER_PROG_INFO_NOT_ENOUGH_SPACE           = 9,
    CECM_TIMER_PROG_INFO_MAY_NOT_BE_ENOUGH_SPACE    = 11,
    CECM_TIMER_PROG_INFO_NO_MEDIA_INFO_AVIL         = 10
}   CECM_TIMER_PROG_INFO_T;

/* Not programmed error info */
/*------------------------------------------------------------------*/
/*! @enum    CECM_TIMER_PROG_ERR_INFO_T
 *  @brief   Reason for programming failure
 *  @code
 *  typedef enum
 *  {
 *      CECM_TIMER_PROG_ERR_INFO_FUTURE_USE                     = 0,
 *      CECM_TIMER_PROG_ERR_INFO_NO_FREE_TIME_AVIL              = 1,
 *      CECM_TIMER_PROG_ERR_INFO_DATA_OUT_OF_RANGE              = 2,
 *      CECM_TIMER_PROG_ERR_INFO_REC_SEQUENCE_ERR               = 3,
 *      CECM_TIMER_PROG_ERR_INFO_INVALD_EXT_PLUG_NUM            = 4,
 *      CECM_TIMER_PROG_ERR_INFO_INVALD_EXT_PHY_ADDR            = 5,
 *      CECM_TIMER_PROG_ERR_INFO_CA_SYS_NOT_SUPPORTED           = 6,
 *      CECM_TIMER_PROG_ERR_INFO_NO_OR_INSUFF_CA_ENTITLEMENTS   = 7,
 *      CECM_TIMER_PROG_ERR_INFO_DOES_NOT_SUPPORT_RESOLUTION    = 8,
 *      CECM_TIMER_PROG_ERR_INFO_PARENTAL_LOCK_ON               = 9,
 *      CECM_TIMER_PROG_ERR_INFO_CLOCK_FAILURE                  = 10,
 *      CECM_TIMER_PROG_ERR_INFO_RESERVED_0                     = 11,
 *      CECM_TIMER_PROG_ERR_INFO_RESERVED_1                     = 12,
 *      CECM_TIMER_PROG_ERR_INFO_RESERVED_2                     = 13,
 *      CECM_TIMER_PROG_ERR_INFO_DUPLICATE                      = 14
 *  }   CECM_TIMER_PROG_ERR_INFO_T;
 *  @endcode
 *  @li@c  CECM_TIMER_PROG_ERR_INFO_FUTURE_USE                         - Future used
 *  @li@c  CECM_TIMER_PROG_ERR_INFO_NO_FREE_TIME_AVIL                  - No free timer available
 *  @li@c  CECM_TIMER_PROG_ERR_INFO_DATA_OUT_OF_RANGE                  - Date out of range
 *  @li@c  CECM_TIMER_PROG_ERR_INFO_REC_SEQUENCE_ERR                   - Recording device does not support this recording sequence
 *  @li@c  CECM_TIMER_PROG_ERR_INFO_INVALD_EXT_PLUG_NUM                - Invalid external plug number
 *  @li@c  CECM_TIMER_PROG_ERR_INFO_INVALD_EXT_PHY_ADDR                - Invalid external physical address
 *  @li@c  CECM_TIMER_PROG_ERR_INFO_CA_SYS_NOT_SUPPORTED               - CA system not support
 *  @li@c  CECM_TIMER_PROG_ERR_INFO_NO_OR_INSUFF_CA_ENTITLEMENTS       - No or insufficient CA entitlements
 *  @li@c  CECM_TIMER_PROG_ERR_INFO_DOES_NOT_SUPPORT_RESOLUTION        - Tuner does not support HD or recorder does not support HD
 *  @li@c  CECM_TIMER_PROG_ERR_INFO_PARENTAL_LOCK_ON                   - Parental lock on
 *  @li@c  CECM_TIMER_PROG_ERR_INFO_CLOCK_FAILURE                      - Clock failure
 *  @li@c  CECM_TIMER_PROG_ERR_INFO_RESERVED_0                         - Reserved for future used
 *  @li@c  CECM_TIMER_PROG_ERR_INFO_RESERVED_1                         - Reserved for future used
 *  @li@c  CECM_TIMER_PROG_ERR_INFO_RESERVED_2                         - Reserved for future used
 *  @li@c  CECM_TIMER_PROG_ERR_INFO_DUPLICATE                          - A timer block with identical details has already been programmed
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_TIMER_PROG_ERR_INFO_FUTURE_USE                     = 0,
    CECM_TIMER_PROG_ERR_INFO_NO_FREE_TIME_AVIL              = 1,
    CECM_TIMER_PROG_ERR_INFO_DATA_OUT_OF_RANGE              = 2,
    CECM_TIMER_PROG_ERR_INFO_REC_SEQUENCE_ERR               = 3,
    CECM_TIMER_PROG_ERR_INFO_INVALD_EXT_PLUG_NUM            = 4,
    CECM_TIMER_PROG_ERR_INFO_INVALD_EXT_PHY_ADDR            = 5,
    CECM_TIMER_PROG_ERR_INFO_CA_SYS_NOT_SUPPORTED           = 6,
    CECM_TIMER_PROG_ERR_INFO_NO_OR_INSUFF_CA_ENTITLEMENTS   = 7,
    CECM_TIMER_PROG_ERR_INFO_DOES_NOT_SUPPORT_RESOLUTION    = 8,
    CECM_TIMER_PROG_ERR_INFO_PARENTAL_LOCK_ON               = 9,
    CECM_TIMER_PROG_ERR_INFO_CLOCK_FAILURE                  = 10,
    CECM_TIMER_PROG_ERR_INFO_RESERVED_0                     = 11,
    CECM_TIMER_PROG_ERR_INFO_RESERVED_1                     = 12,
    CECM_TIMER_PROG_ERR_INFO_RESERVED_2                     = 13,
    CECM_TIMER_PROG_ERR_INFO_DUPLICATE                      = 14
}   CECM_TIMER_PROG_ERR_INFO_T;

/* Timer staus data */
/*------------------------------------------------------------------*/
/*! @struct    CECM_TIMER_STS_T
 *  @brief     Used by recoding device to respond to the initiator of a Set Timer message
 *  @code
 *  typedef struct _CECM_TIMER_STS_T
 *  {
 *      CECM_TIMER_OVERLAP_T            e_overlap;
 *      CECM_TIMER_MEDIA_INFO_T         e_media_info;
 *      CECM_TIMER_PROG_IND_T           e_prog_ind;
 *      CECM_TIMER_PROG_INFO_T          e_prog_info;
 *      CECM_TIMER_PROG_ERR_INFO_T      e_not_prog_err_info;
 *      UINT16                          ui2_duration;
 *  }   CECM_TIMER_STS_T;
 *  @endcode
 *  @li@c  e_overlap                                         - Indicates if there is another timer block already set which overlaps with this new recording request 
 *  @li@c  e_media_info                                      - Indicates if removable media is present and its write protect state
 *  @li@c  e_prog_ind                                        - Selector for [Timer Programmed Info]
 *  @li@c  e_prog_info                                       - Information indicating any non-fatal issue with the programming request
 *  @li@c  e_not_prog_err_info                               - Programmed error information
 *  @li@c  ui2_duration                                      - Estimate of the space left on the media
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_TIMER_STS_T
{
    CECM_TIMER_OVERLAP_T            e_overlap;
    CECM_TIMER_MEDIA_INFO_T         e_media_info;
    CECM_TIMER_PROG_IND_T           e_prog_ind;
    CECM_TIMER_PROG_INFO_T          e_prog_info;
    CECM_TIMER_PROG_ERR_INFO_T      e_not_prog_err_info;
    UINT16                          ui2_duration;
}   CECM_TIMER_STS_T;

/* System Information Feature ************************************************/
/* Device type */
/*------------------------------------------------------------------*/
/*! @enum    CECM_DEV_TYPE_T
 *  @brief   Device type
 *  @code
 *  typedef enum
 *  {
 *      CECM_DEV_TYPE_TV            = 0,
 *      CECM_DEV_TYPE_REC_DEV       = 1,
 *      CECM_DEV_TYPE_RESERVED      = 2,
 *      CECM_DEV_TYPE_TUNER         = 3,
 *      CECM_DEV_TYPE_PLAYBACK_DEV  = 4,
 *      CECM_DEV_TYPE_AUD_SYS       = 5,
 *      CECM_DEV_TYPE_NONE          = 0xff
 *  }   CECM_DEV_TYPE_T;
 *  @endcode
 *  @li@c  CECM_DEV_TYPE_TV                                  - TV
 *  @li@c  CECM_DEV_TYPE_REC_DEV                             - Record device
 *  @li@c  CECM_DEV_TYPE_RESERVED                            - Reserved for future used
 *  @li@c  CECM_DEV_TYPE_TUNER                               - Tuner device
 *  @li@c  CECM_DEV_TYPE_PLAYBACK_DEV                        - Playback device
 *  @li@c  CECM_DEV_TYPE_AUD_SYS                             - Audio system device
 *  @li@c  CECM_DEV_TYPE_NONE                                - None
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_DEV_TYPE_TV            = 0,
    CECM_DEV_TYPE_REC_DEV       = 1,
    CECM_DEV_TYPE_RESERVED      = 2,
    CECM_DEV_TYPE_TUNER         = 3,
    CECM_DEV_TYPE_PLAYBACK_DEV  = 4,
    CECM_DEV_TYPE_AUD_SYS       = 5,
    CECM_DEV_TYPE_NONE          = 0xff
}   CECM_DEV_TYPE_T;

/* Report physical address */
/*------------------------------------------------------------------*/
/*! @struct    CECM_REPORT_PHY_ADDR_T
 *  @brief     Device report physical address
 *  @code
 *  typedef struct _CECM_REPORT_PHY_ADDR_T
 *  {
 *      UINT16              ui2_pa;
 *      CECM_DEV_TYPE_T     e_dev_type;
 *  }   CECM_REPORT_PHY_ADDR_T;
 *  @endcode
 *  @li@c  ui2_pa                                            - Device physical address
 *  @li@c  e_dev_type                                        - Device type
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_REPORT_PHY_ADDR_T
{
    UINT16              ui2_pa;
    CECM_DEV_TYPE_T     e_dev_type;
}   CECM_REPORT_PHY_ADDR_T;

/* Polling info */
/*------------------------------------------------------------------*/
/*! @struct    CECM_POLLING_INFO_T
 *  @brief     Polling information
 *  @code
 *  typedef struct _CECM_POLLING_INFO_T
 *  {
 *      BOOL    b_ack_ok;
 *  }   CECM_POLLING_INFO_T;
 *  @endcode
 *  @li@c  b_ack_ok                                          - If there is acknowledge
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_POLLING_INFO_T
{
    BOOL    b_ack_ok;
}   CECM_POLLING_INFO_T;



/* Deck Control Feature ******************************************************/
/* Deck control mode */
/*------------------------------------------------------------------*/
/*! @enum    CECM_DECK_CTRL_MOD_T
 *  @brief   Deck control mode
 *  @code
 *  typedef enum
 *  {
 *      CECM_DECK_CTRL_MOD_SKIP_FWR_WIND    = 1,
 *      CECM_DECK_CTRL_MOD_SKIP_RVR_REWIND  = 2,
 *      CECM_DECK_CTRL_MOD_STOP             = 3,
 *      CECM_DECK_CTRL_MOD_EJECT            = 4
 *  }   CECM_DECK_CTRL_MOD_T;
 *  @endcode
 *  @li@c  CECM_DECK_CTRL_MOD_SKIP_FWR_WIND                  - Deck control mode skip forward
 *  @li@c  CECM_DECK_CTRL_MOD_SKIP_RVR_REWIND                - Deck control mode skip reverse
 *  @li@c  CECM_DECK_CTRL_MOD_STOP                           - Deck control mode stop
 *  @li@c  CECM_DECK_CTRL_MOD_EJECT                          - Deck control mode eject
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_DECK_CTRL_MOD_SKIP_FWR_WIND    = 1,
    CECM_DECK_CTRL_MOD_SKIP_RVR_REWIND  = 2,
    CECM_DECK_CTRL_MOD_STOP             = 3,
    CECM_DECK_CTRL_MOD_EJECT            = 4
}   CECM_DECK_CTRL_MOD_T;

/* Deck info */
/*------------------------------------------------------------------*/
/*! @enum    CECM_DECK_INFO_T
 *  @brief   Deck status information
 *  @code
 *  typedef enum
 *  {
 *      CECM_DECK_INFO_PLAY             = 0x11,
 *      CECM_DECK_INFO_REC              = 0x12,
 *      CECM_DECK_INFO_PLAY_RVR         = 0x13,
 *      CECM_DECK_INFO_STILL            = 0x14,
 *      CECM_DECK_INFO_SLOW             = 0x15,
 *      CECM_DECK_INFO_SLOW_RVR         = 0x16,
 *      CECM_DECK_INFO_FAST_FWR         = 0x17,
 *      CECM_DECK_INFO_FAST_RVR         = 0x18,
 *      CECM_DECK_INFO_NO_MEDIA         = 0x19,
 *      CECM_DECK_INFO_STOP             = 0x1a,
 *      CECM_DECK_INFO_SKIP_FWR_WIND    = 0x1b,
 *      CECM_DECK_INFO_SKIP_RVR_REWIND  = 0x1c,
 *      CECM_DECK_INFO_INDEX_SEARCH_FWR = 0x1d,
 *      CECM_DECK_INFO_INDEX_SEARCH_RVR = 0x1e,
 *      CECM_DECK_INFO_OTHER_STS        = 0x1f
 *  }   CECM_DECK_INFO_T;
 *  @endcode
 *  @li@c  CECM_DECK_INFO_PLAY                               - Current status Play
 *  @li@c  CECM_DECK_INFO_REC                                - Current status record
 *  @li@c  CECM_DECK_INFO_PLAY_RVR                           - Current status play reverse
 *  @li@c  CECM_DECK_INFO_STILL                              - Current status still
 *  @li@c  CECM_DECK_INFO_SLOW                               - Current status slow
 *  @li@c  CECM_DECK_INFO_SLOW_RVR                           - Current status slow reverse
 *  @li@c  CECM_DECK_INFO_FAST_FWR                           - Current status fast forward
 *  @li@c  CECM_DECK_INFO_FAST_RVR                           - Current status fast reverse
 *  @li@c  CECM_DECK_INFO_NO_MEDIA                           - Current status no media
 *  @li@c  CECM_DECK_INFO_STOP                               - Current status stop
 *  @li@c  CECM_DECK_INFO_SKIP_FWR_WIND                      - Current status skip forward 
 *  @li@c  CECM_DECK_INFO_SKIP_RVR_REWIND                    - Current status skip reserve
 *  @li@c  CECM_DECK_INFO_INDEX_SEARCH_FWR                   - Current status index search forward
 *  @li@c  CECM_DECK_INFO_INDEX_SEARCH_RVR                   - Current status index search reverse
 *  @li@c  CECM_DECK_INFO_OTHER_STS                          - Other status
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_DECK_INFO_PLAY             = 0x11,
    CECM_DECK_INFO_REC              = 0x12,
    CECM_DECK_INFO_PLAY_RVR         = 0x13,
    CECM_DECK_INFO_STILL            = 0x14,
    CECM_DECK_INFO_SLOW             = 0x15,
    CECM_DECK_INFO_SLOW_RVR         = 0x16,
    CECM_DECK_INFO_FAST_FWR         = 0x17,
    CECM_DECK_INFO_FAST_RVR         = 0x18,
    CECM_DECK_INFO_NO_MEDIA         = 0x19,
    CECM_DECK_INFO_STOP             = 0x1a,
    CECM_DECK_INFO_SKIP_FWR_WIND    = 0x1b,
    CECM_DECK_INFO_SKIP_RVR_REWIND  = 0x1c,
    CECM_DECK_INFO_INDEX_SEARCH_FWR = 0x1d,
    CECM_DECK_INFO_INDEX_SEARCH_RVR = 0x1e,
    CECM_DECK_INFO_OTHER_STS        = 0x1f
}   CECM_DECK_INFO_T;

/* Status request */
/*------------------------------------------------------------------*/
/*! @enum    CECM_STS_REQ_T
 *  @brief   Request the status of a device information
 *  @code
 *  typedef enum
 *  {
 *      CECM_STS_REQ_ON     = 1,
 *      CECM_STS_REQ_OFF    = 2,
 *      CECM_STS_REQ_ONCE   = 3
 *  }   CECM_STS_REQ_T;
 *  @endcode
 *  @li@c  CECM_STS_REQ_ON                                   - Status request mode on all future state changes
 *  @li@c  CECM_STS_REQ_OFF                                  - Status request mode off
 *  @li@c  CECM_STS_REQ_ONCE                                 - Status request mode report once
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_STS_REQ_ON     = 1,
    CECM_STS_REQ_OFF    = 2,
    CECM_STS_REQ_ONCE   = 3
}   CECM_STS_REQ_T;

/* Play mode */
/*------------------------------------------------------------------*/
/*! @enum    CECM_PLAY_MOD_T
 *  @brief   Used to control the palyback behaviour
 *  @code
 *  typedef enum
 *  {
 *      CECM_PLAY_MOD_PLAY_FWR              = 0x24,
 *      CECM_PLAY_MOD_PLAY_RVR              = 0x20,
 *      CECM_PLAY_MOD_PLAY_STILL            = 0x25,
 *      CECM_PLAY_MOD_FAST_FWR_MIN_SPEED    = 0x05,
 *      CECM_PLAY_MOD_FAST_FWR_MED_SPEED    = 0x06,
 *      CECM_PLAY_MOD_FAST_FWR_MAX_SPEED    = 0x07,
 *      CECM_PLAY_MOD_FAST_RVR_MIN_SPEED    = 0x09,
 *      CECM_PLAY_MOD_FAST_RVR_MED_SPEED    = 0x0A,
 *      CECM_PLAY_MOD_FAST_RVR_MAX_SPEED    = 0x0B,
 *      CECM_PLAY_MOD_SLOW_FWR_MIN_SPEED    = 0x15,
 *      CECM_PLAY_MOD_SLOW_FWR_MED_SPEED    = 0x16,
 *      CECM_PLAY_MOD_SLOW_FWR_MAX_SPEED    = 0x17,
 *      CECM_PLAY_MOD_SLOW_RVR_MIN_SPEED    = 0x19,
 *      CECM_PLAY_MOD_SLOW_RVR_MED_SPEED    = 0x1A,
 *      CECM_PLAY_MOD_SLOW_RVR_MAX_SPEED    = 0x1B
 *  }   CECM_PLAY_MOD_T;
 *  @endcode
 *  @li@c  CECM_PLAY_MOD_PLAY_FWR                            - Play forward
 *  @li@c  CECM_PLAY_MOD_PLAY_RVR                            - Play reverse
 *  @li@c  CECM_PLAY_MOD_PLAY_STILL                          - Play still
 *  @li@c  CECM_PLAY_MOD_FAST_FWR_MIN_SPEED                  - Fast forward min speed
 *  @li@c  CECM_PLAY_MOD_FAST_FWR_MED_SPEED                  - Fast forward medium speed
 *  @li@c  CECM_PLAY_MOD_FAST_FWR_MAX_SPEED                  - Fast forward max speed
 *  @li@c  CECM_PLAY_MOD_FAST_RVR_MIN_SPEED                  - Fast reverse min speed
 *  @li@c  CECM_PLAY_MOD_FAST_RVR_MED_SPEED                  - Fast reverse medium speed
 *  @li@c  CECM_PLAY_MOD_FAST_RVR_MAX_SPEED                  - Fast reverse max speed
 *  @li@c  CECM_PLAY_MOD_SLOW_FWR_MIN_SPEED                  - Slow forward min speed
 *  @li@c  CECM_PLAY_MOD_SLOW_FWR_MED_SPEED                  - Slow forward medium speed
 *  @li@c  CECM_PLAY_MOD_SLOW_FWR_MAX_SPEED                  - Slow forward max speed
 *  @li@c  CECM_PLAY_MOD_SLOW_RVR_MIN_SPEED                  - Slow reverse min speed
 *  @li@c  CECM_PLAY_MOD_SLOW_RVR_MED_SPEED                  - Slow reverse medium speed
 *  @li@c  CECM_PLAY_MOD_SLOW_RVR_MAX_SPEED                  - Slow reverse max speed
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_PLAY_MOD_PLAY_FWR              = 0x24,
    CECM_PLAY_MOD_PLAY_RVR              = 0x20,
    CECM_PLAY_MOD_PLAY_STILL            = 0x25,
    CECM_PLAY_MOD_FAST_FWR_MIN_SPEED    = 0x05,
    CECM_PLAY_MOD_FAST_FWR_MED_SPEED    = 0x06,
    CECM_PLAY_MOD_FAST_FWR_MAX_SPEED    = 0x07,
    CECM_PLAY_MOD_FAST_RVR_MIN_SPEED    = 0x09,
    CECM_PLAY_MOD_FAST_RVR_MED_SPEED    = 0x0A,
    CECM_PLAY_MOD_FAST_RVR_MAX_SPEED    = 0x0B,
    CECM_PLAY_MOD_SLOW_FWR_MIN_SPEED    = 0x15,
    CECM_PLAY_MOD_SLOW_FWR_MED_SPEED    = 0x16,
    CECM_PLAY_MOD_SLOW_FWR_MAX_SPEED    = 0x17,
    CECM_PLAY_MOD_SLOW_RVR_MIN_SPEED    = 0x19,
    CECM_PLAY_MOD_SLOW_RVR_MED_SPEED    = 0x1A,
    CECM_PLAY_MOD_SLOW_RVR_MAX_SPEED    = 0x1B
}   CECM_PLAY_MOD_T;

/* Tuner Control Feature *****************************************************/
/* Service type */
/*------------------------------------------------------------------*/
/*! @enum    CECM_TUNER_SVC_T
 *  @brief   Service type
 *  @code
 *  typedef enum
 *  {
 *      CECM_TUNER_SVC_ANA,
 *      CECM_TUNER_SVC_DIG
 *  }   CECM_TUNER_SVC_T;
 *  @endcode
 *  @li@c  CECM_TUNER_SVC_ANA                                - Analog service
 *  @li@c  CECM_TUNER_SVC_DIG                                - Digital service
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_TUNER_SVC_ANA,
    CECM_TUNER_SVC_DIG
}   CECM_TUNER_SVC_T;

/* Tuner analog and digital service info */
typedef CECM_ANA_SVC_T CECM_TUNER_ANA_T;    /**<  Analog service information */
typedef CECM_DIG_SVC_T CECM_TUNER_DIG_T;    /**<  Digital service information */

/* Tuner service info */
/*------------------------------------------------------------------*/
/*! @struct    CECM_TUNER_SVC_INFO_T
 *  @brief     Tuenr service information
 *  @code
 *  typedef struct _CECM_TUNER_SVC_INFO_T
 *  {
 *      CECM_TUNER_SVC_T        e_svc;
 *      union
 *      {
 *          CECM_TUNER_ANA_T    t_ana_info;
 *          CECM_TUNER_DIG_T    t_dig_info;
 *      }   u;
 *  }   CECM_TUNER_SVC_INFO_T;
 *  @endcode
 *  @li@c  e_svc                                             - Tuner service type
 *  @li@c  t_ana_info                                        - Analog service information
 *  @li@c  t_dig_info                                        - Digital service information
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_TUNER_SVC_INFO_T
{
    CECM_TUNER_SVC_T        e_svc;

    union
    {
        CECM_TUNER_ANA_T    t_ana_info;
        CECM_TUNER_DIG_T    t_dig_info;
    }   u;
}   CECM_TUNER_SVC_INFO_T;

/* Tuner step */
/*------------------------------------------------------------------*/
/*! @enum    CECM_TUNER_STEP_T
 *  @brief   Control the device tuner
 *  @code
 *  typedef enum
 *  {
 *      CECM_TUNER_STEP_DECREMENT,
 *      CECM_TUNER_STEP_INCREMENT
 *  }   CECM_TUNER_STEP_T;
 *  @endcode
 *  @li@c  CECM_TUNER_STEP_DECREMENT                         - Tuner decrement
 *  @li@c  CECM_TUNER_STEP_INCREMENT                         - Tuner increment
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_TUNER_STEP_DECREMENT,
    CECM_TUNER_STEP_INCREMENT
}   CECM_TUNER_STEP_T;

/* Tuner recording flag */
/*------------------------------------------------------------------*/
/*! @enum    CECM_TUNER_REC_FLAG_T
 *  @brief   Tuner record flag
 *  @code
 *  typedef enum
 *  {
 *      CECM_TUNER_REC_FLAG_NOT_BEING_USED_FOR_REC  = 0,
 *      CECM_TUNER_REC_FLAG_BEING_USED_FOR_REC      = 1
 *  }   CECM_TUNER_REC_FLAG_T;
 *  @endcode
 *  @li@c  CECM_TUNER_REC_FLAG_NOT_BEING_USED_FOR_REC        - Not begin used for record
 *  @li@c  CECM_TUNER_REC_FLAG_BEING_USED_FOR_REC            - Begin used for record
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_TUNER_REC_FLAG_NOT_BEING_USED_FOR_REC  = 0,
    CECM_TUNER_REC_FLAG_BEING_USED_FOR_REC      = 1
}   CECM_TUNER_REC_FLAG_T;

/* Tuner display info */
/*------------------------------------------------------------------*/
/*! @enum    CECM_TUNER_DISPLAY_INFO_T
 *  @brief   Tuner display information
 *  @code
 *  typedef enum
 *  {
 *      CECM_TUNER_DISPLAY_INFO_DISPLAY_DIG_TUNER   = 0,
 *      CECM_TUNER_DISPLAY_INFO_NOT_DISPLAY_TUNER   = 1,
 *      CECM_TUNER_DISPLAY_INFO_DISPLAY_ANA_TUNER   = 2
 *  }   CECM_TUNER_DISPLAY_INFO_T;
 *  @endcode
 *  @li@c  CECM_TUNER_DISPLAY_INFO_DISPLAY_DIG_TUNER         - Display digital information
 *  @li@c  CECM_TUNER_DISPLAY_INFO_NOT_DISPLAY_TUNER         - Not display information
 *  @li@c  CECM_TUNER_DISPLAY_INFO_DISPLAY_ANA_TUNER         - Display analoge information
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_TUNER_DISPLAY_INFO_DISPLAY_DIG_TUNER   = 0,
    CECM_TUNER_DISPLAY_INFO_NOT_DISPLAY_TUNER   = 1,
    CECM_TUNER_DISPLAY_INFO_DISPLAY_ANA_TUNER   = 2
}   CECM_TUNER_DISPLAY_INFO_T;

/* Tuner device info */
/*------------------------------------------------------------------*/
/*! @struct    CECM_TUNER_DEV_INFO_T
 *  @brief     The tuner device information
 *  @code
 *  typedef struct _CECM_TUNER_DEV_INFO_T
 *  {
 *      CECM_TUNER_SVC_T            e_svc;
 *      CECM_TUNER_REC_FLAG_T       e_rec_flag;
 *      CECM_TUNER_DISPLAY_INFO_T   e_display_info;
 *      union
 *      {
 *          CECM_TUNER_ANA_T    t_ana_info;
 *          CECM_TUNER_DIG_T    t_dig_info;
 *      }   u;
 *  }   CECM_TUNER_DEV_INFO_T;
 *  @endcode
 *  @li@c  e_svc                                             - The sevice type, digital or analoge
 *  @li@c  e_rec_flag                                        - Record flag, begin or not
 *  @li@c  e_display_info                                    - Tuner display information 
 *  @li@c  t_ana_info                                        - Analoge sevice information
 *  @li@c  t_dig_info                                        - Digital sevice information
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_TUNER_DEV_INFO_T
{
    CECM_TUNER_SVC_T            e_svc;
    CECM_TUNER_REC_FLAG_T       e_rec_flag;
    CECM_TUNER_DISPLAY_INFO_T   e_display_info;

    union
    {
        CECM_TUNER_ANA_T    t_ana_info;
        CECM_TUNER_DIG_T    t_dig_info;
    }   u;
}   CECM_TUNER_DEV_INFO_T;



/* OSD Display Feature *******************************************************/
/* Display control */
/*------------------------------------------------------------------*/
/*! @enum    CECM_DISPLAY_CTRL_T
 *  @brief   Indicate the display mode for an screen display message
 *  @code
 *  typedef enum
 *  {
 *      CECM_DISPLAY_CTRL_BY_DEFAULT_TIME   = 0x0,
 *      CECM_DISPLAY_CTRL_UNTIL_CLRED       = 0x40,
 *      CECM_DISPLAY_CTRL_CLR_PRV_MSG       = 0x80,
 *      CECM_DISPLAY_CTRL_CLR_RESERVED      = 0xC0
 *  }   CECM_DISPLAY_CTRL_T;
 *  @endcode
 *  @li@c  CECM_DISPLAY_CTRL_BY_DEFAULT_TIME                 - Display the string for default time
 *  @li@c  CECM_DISPLAY_CTRL_UNTIL_CLRED                     - Display the string until cleared
 *  @li@c  CECM_DISPLAY_CTRL_CLR_PRV_MSG                     - Clear previous message 
 *  @li@c  CECM_DISPLAY_CTRL_CLR_RESERVED                    - Reserved for future used
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_DISPLAY_CTRL_BY_DEFAULT_TIME   = 0x0,
    CECM_DISPLAY_CTRL_UNTIL_CLRED       = 0x40,
    CECM_DISPLAY_CTRL_CLR_PRV_MSG       = 0x80,
    CECM_DISPLAY_CTRL_CLR_RESERVED      = 0xC0
}   CECM_DISPLAY_CTRL_T;

/* OSD string type */
/*------------------------------------------------------------------*/
/*! @struct    CECM_OSD_STRING_INFO_T
 *  @brief     OSD string to be displayed information
 *  @code
 *  typedef struct _CECM_OSD_STRING_INFO_T
 *  {
 *      CECM_DISPLAY_CTRL_T     e_display_ctrl;
 *      UTF16_T                 aw2_osd_string[CECM_OSD_STRING_SIZE];
 *  }   CECM_OSD_STRING_INFO_T;
 *  @endcode
 *  @li@c  e_display_ctrl                                    - Display control operation
 *  @li@c  aw2_osd_string[CECM_OSD_STRING_SIZE]              - A string to be displayed
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_OSD_STRING_INFO_T
{
    CECM_DISPLAY_CTRL_T     e_display_ctrl;
    UTF16_T                 aw2_osd_string[CECM_MAX_OPERAND_SIZE];
}   CECM_OSD_STRING_INFO_T;

/* Device OSD Transfer Feature ***********************************************/

/* Device Menu Control Feature ***********************************************/
/* Menu request type or menu state */
/*------------------------------------------------------------------*/
/*! @enum   CECM_MENU_REQ_STATE_T 
 *  @brief  Indicate Menu status, TV can active,deactive, query device's menu status
 *  @code
 *  typedef enum
 *  {
 *      CECM_MENU_REQ_STATE_ACT     = 0,
 *      CECM_MENU_REQ_STATE_DEACT   = 1,
 *      CECM_MENU_REQ_STATE_QUERY   = 2
 *  }   CECM_MENU_REQ_STATE_T;
 *  @endcode
 *  @li@c  CECM_MENU_REQ_STATE_ACT                           - Active device's menu
 *  @li@c  CECM_MENU_REQ_STATE_DEACT                         - Deactive divice's menu
 *  @li@c  CECM_MENU_REQ_STATE_QUERY                         - Query device's menu status
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_MENU_REQ_STATE_ACT     = 0,
    CECM_MENU_REQ_STATE_DEACT   = 1,
    CECM_MENU_REQ_STATE_QUERY   = 2,
    CECM_MENU_REQ_STATE_ERR
}   CECM_MENU_REQ_STATE_T;

/* Remote Control Passthrough Feature ****************************************/
/* User control code */
/*------------------------------------------------------------------*/
/*! @enum    CECM_USER_CTRL_T
 *  @brief   User control codes
 *  @code
 *  typedef enum
 *  {
 *      CECM_USER_CTRL_SELECT                   = 0x00,
 *      CECM_USER_CTRL_UP                       = 0x01,
 *      CECM_USER_CTRL_DOWN                     = 0x02,
 *      CECM_USER_CTRL_LEFT                     = 0x03,
 *      CECM_USER_CTRL_RIGHT                    = 0x04,
 *      CECM_USER_CTRL_RIGHT_UP                 = 0x05,
 *      CECM_USER_CTRL_RIGHT_DOWN               = 0x06,
 *      CECM_USER_CTRL_LEFT_UP                  = 0x07,
 *      CECM_USER_CTRL_LEFT_DOWN                = 0x08,
 *      CECM_USER_CTRL_ROOT_MENU                = 0x09,
 *      CECM_USER_CTRL_SETUP_MENU               = 0x0a,
 *      CECM_USER_CTRL_CONTENTS_MENU            = 0x0b,
 *      CECM_USER_CTRL_FAV_MENU                 = 0x0c,
 *      CECM_USER_CTRL_EXIT                     = 0x0d,
 *      CECM_USER_CTRL_NUM_0                    = 0x20,
 *      CECM_USER_CTRL_NUM_1                    = 0x21,
 *      CECM_USER_CTRL_NUM_2                    = 0x22,
 *      CECM_USER_CTRL_NUM_3                    = 0x23,
 *      CECM_USER_CTRL_NUM_4                    = 0x24,
 *      CECM_USER_CTRL_NUM_5                    = 0x25,
 *      CECM_USER_CTRL_NUM_6                    = 0x26,
 *      CECM_USER_CTRL_NUM_7                    = 0x27,
 *      CECM_USER_CTRL_NUM_8                    = 0x28,
 *      CECM_USER_CTRL_NUM_9                    = 0x29,
 *      CECM_USER_CTRL_DOT                      = 0x2a,
 *      CECM_USER_CTRL_ENTER                    = 0x2b,
 *      CECM_USER_CTRL_CLEAR                    = 0x2c,
 *      CECM_USER_CTRL_NEXT_FAV                 = 0x2f,
 *      CECM_USER_CTRL_CH_UP                    = 0x30,
 *      CECM_USER_CTRL_CH_DOWN                  = 0x31,
 *      CECM_USER_CTRL_PREV_CH                  = 0x32,
 *      CECM_USER_CTRL_SOUND_SELECT             = 0x33,
 *      CECM_USER_CTRL_INPUT_SELECT             = 0x34,
 *      CECM_USER_CTRL_DISPLAY_INFO             = 0x35,
 *      CECM_USER_CTRL_HELP                     = 0x36,
 *      CECM_USER_CTRL_PAGE_UP                  = 0x37,
 *      CECM_USER_CTRL_PAGE_DOWN                = 0x38,
 *      CECM_USER_CTRL_POWER                    = 0x40,
 *      CECM_USER_CTRL_VOL_UP                   = 0x41,
 *      CECM_USER_CTRL_VOL_DOWN                 = 0x42,
 *      CECM_USER_CTRL_MUTE                     = 0x43,
 *      CECM_USER_CTRL_PLAY                     = 0x44,
 *      CECM_USER_CTRL_STOP                     = 0x45,
 *      CECM_USER_CTRL_PAUSE                    = 0x46,
 *      CECM_USER_CTRL_RECORD                   = 0x47,
 *      CECM_USER_CTRL_REWIND                   = 0x48,
 *      CECM_USER_CTRL_FAST_FORDWARD            = 0x49,
 *      CECM_USER_CTRL_EJECT                    = 0x4a,
 *      CECM_USER_CTRL_FORWARD                  = 0x4b,
 *      CECM_USER_CTRL_BACKWARD                 = 0x4c,
 *      CECM_USER_CTRL_STOP_REC                 = 0x4d,
 *      CECM_USER_CTRL_PAUSE_REC                = 0x4e,
 *      CECM_USER_CTRL_ANGLE                    = 0x50,
 *      CECM_USER_CTRL_SUB_PIC                  = 0x51,
 *      CECM_USER_CTRL_VOD                      = 0x52,
 *      CECM_USER_CTRL_EPG                      = 0x53,
 *      CECM_USER_CTRL_TIMER_PROG               = 0x54,
 *      CECM_USER_CTRL_INIT_CONFIG              = 0x55,
 *      CECM_USER_CTRL_PLAY_FUNC                = 0x60,
 *      CECM_USER_CTRL_PAUSE_PLAY_FUNC          = 0x61,
 *      CECM_USER_CTRL_REC_FUNC                 = 0x62,
 *      CECM_USER_CTRL_PAUSE_REC_FUNC           = 0x63,
 *      CECM_USER_CTRL_STOP_FUNC                = 0x64,
 *      CECM_USER_CTRL_MUTE_FUNC                = 0x65,
 *      CECM_USER_CTRL_RESTORE_VOL_FUNC         = 0x66,
 *      CECM_USER_CTRL_TUNE_FUNC                = 0x67,
 *      CECM_USER_CTRL_SELECT_MEDIA_FUNC        = 0x68,
 *      CECM_USER_CTRL_SELECT_AV_INPUT_FUNC     = 0x69,
 *      CECM_USER_CTRL_SELECT_AUD_INPUT_FUNC    = 0x6a,
 *      CECM_USER_CTRL_PWR_TOGGLE_FUNC          = 0x6b,
 *      CECM_USER_CTRL_PWR_OFF_FUNC             = 0x6c,
 *      CECM_USER_CTRL_PWR_ON_FUNC              = 0x6d,
 *      CECM_USER_CTRL_F1_BLUE                  = 0x71,
 *      CECM_USER_CTRL_F2_RED                   = 0x72,
 *      CECM_USER_CTRL_F3_GREEN                 = 0x73,
 *      CECM_USER_CTRL_F4_YELLOW                = 0x74,
 *      CECM_USER_CTRL_F5                       = 0x75,
 *      CECM_USER_CTRL_DATA                     = 0x76
 *  }   CECM_USER_CTRL_T;
 *  @endcode
 *  @li@c  CECM_USER_CTRL_SELECT                             - User opration: Select
 *  @li@c  CECM_USER_CTRL_UP                                 - User opration: Up
 *  @li@c  CECM_USER_CTRL_DOWN                               - User opration: Down
 *  @li@c  CECM_USER_CTRL_LEFT                               - User opration: Left
 *  @li@c  CECM_USER_CTRL_RIGHT                              - User opration: Right
 *  @li@c  CECM_USER_CTRL_RIGHT_UP                           - User opration: Right Up
 *  @li@c  CECM_USER_CTRL_RIGHT_DOWN                         - User opration: Right Down
 *  @li@c  CECM_USER_CTRL_LEFT_UP                            - User opration: Left Up
 *  @li@c  CECM_USER_CTRL_LEFT_DOWN                          - User opration: Left Down
 *  @li@c  CECM_USER_CTRL_ROOT_MENU                          - User opration: Root Menu 
 *  @li@c  CECM_USER_CTRL_SETUP_MENU                         - User opration: Setup Menu
 *  @li@c  CECM_USER_CTRL_CONTENTS_MENU                      - User opration: Contents Menu
 *  @li@c  CECM_USER_CTRL_FAV_MENU                           - User opration: Favorite Menu
 *  @li@c  CECM_USER_CTRL_EXIT                               - User opration: Exit
 *  @li@c  CECM_USER_CTRL_NUM_0                              - User opration: Number 0
 *  @li@c  CECM_USER_CTRL_NUM_1                              - User opration: Number 1
 *  @li@c  CECM_USER_CTRL_NUM_2                              - User opration: Number 2
 *  @li@c  CECM_USER_CTRL_NUM_3                              - User opration: Number 3
 *  @li@c  CECM_USER_CTRL_NUM_4                              - User opration: Number 4
 *  @li@c  CECM_USER_CTRL_NUM_5                              - User opration: Number 5 
 *  @li@c  CECM_USER_CTRL_NUM_6                              - User opration: Number 6
 *  @li@c  CECM_USER_CTRL_NUM_7                              - User opration: Number 7
 *  @li@c  CECM_USER_CTRL_NUM_8                              - User opration: Number 8
 *  @li@c  CECM_USER_CTRL_NUM_9                              - User opration: Number 9
 *  @li@c  CECM_USER_CTRL_DOT                                - User opration: Dot
 *  @li@c  CECM_USER_CTRL_ENTER                              - User opration: Enter
 *  @li@c  CECM_USER_CTRL_CLEAR                              - User opration: Clear
 *  @li@c  CECM_USER_CTRL_NEXT_FAV                           - User opration: Next Favorite
 *  @li@c  CECM_USER_CTRL_CH_UP                              - User opration: Channel Up
 *  @li@c  CECM_USER_CTRL_CH_DOWN                            - User opration: Channel Down
 *  @li@c  CECM_USER_CTRL_PREV_CH                            - User opration: Previous Channel
 *  @li@c  CECM_USER_CTRL_SOUND_SELECT                       - User opration: Sound Select
 *  @li@c  CECM_USER_CTRL_INPUT_SELECT                       - User opration: Input Select
 *  @li@c  CECM_USER_CTRL_DISPLAY_INFO                       - User opration: Display Information
 *  @li@c  CECM_USER_CTRL_HELP                               - User opration: Help
 *  @li@c  CECM_USER_CTRL_PAGE_UP                            - User opration: Page Up
 *  @li@c  CECM_USER_CTRL_PAGE_DOWN                          - User opration: Page Down
 *  @li@c  CECM_USER_CTRL_POWER                              - User opration: Power
 *  @li@c  CECM_USER_CTRL_VOL_UP                             - User opration: Volume Up
 *  @li@c  CECM_USER_CTRL_VOL_DOWN                           - User opration: Volume Down
 *  @li@c  CECM_USER_CTRL_MUTE                               - User opration: Mute
 *  @li@c  CECM_USER_CTRL_PLAY                               - User opration: Play
 *  @li@c  CECM_USER_CTRL_STOP                               - User opration: Stop
 *  @li@c  CECM_USER_CTRL_PAUSE                              - User opration: Pause
 *  @li@c  CECM_USER_CTRL_RECORD                             - User opration: Record
 *  @li@c  CECM_USER_CTRL_REWIND                             - User opration: Rewind
 *  @li@c  CECM_USER_CTRL_FAST_FORDWARD                      - User opration: Fast Forward
 *  @li@c  CECM_USER_CTRL_EJECT                              - User opration: Eject
 *  @li@c  CECM_USER_CTRL_FORWARD                            - User opration: Forward
 *  @li@c  CECM_USER_CTRL_BACKWARD                           - User opration: Backward
 *  @li@c  CECM_USER_CTRL_STOP_REC                           - User opration: Stop Record
 *  @li@c  CECM_USER_CTRL_PAUSE_REC                          - User opration: Pause Record
 *  @li@c  CECM_USER_CTRL_ANGLE                              - User opration: Angle
 *  @li@c  CECM_USER_CTRL_SUB_PIC                            - User opration: Sub Picture
 *  @li@c  CECM_USER_CTRL_VOD                                - User opration: Video on Demand
 *  @li@c  CECM_USER_CTRL_EPG                                - User opration: EPG
 *  @li@c  CECM_USER_CTRL_TIMER_PROG                         - User opration: Timer Programming
 *  @li@c  CECM_USER_CTRL_INIT_CONFIG                        - User opration: Initial Configuration
 *  @li@c  CECM_USER_CTRL_PLAY_FUNC                          - User opration: Play Function
 *  @li@c  CECM_USER_CTRL_PAUSE_PLAY_FUNC                    - User opration: Pause Play Function
 *  @li@c  CECM_USER_CTRL_REC_FUNC                           - User opration: Record Function
 *  @li@c  CECM_USER_CTRL_PAUSE_REC_FUNC                     - User opration: Pause Record Function
 *  @li@c  CECM_USER_CTRL_STOP_FUNC                          - User opration: Stop Function
 *  @li@c  CECM_USER_CTRL_MUTE_FUNC                          - User opration: Mute Function
 *  @li@c  CECM_USER_CTRL_RESTORE_VOL_FUNC                   - User opration: Restore Volume Function
 *  @li@c  CECM_USER_CTRL_TUNE_FUNC                          - User opration: Tune Function
 *  @li@c  CECM_USER_CTRL_SELECT_MEDIA_FUNC                  - User opration: Select Media Function
 *  @li@c  CECM_USER_CTRL_SELECT_AV_INPUT_FUNC               - User opration: Select A/V Input Function
 *  @li@c  CECM_USER_CTRL_SELECT_AUD_INPUT_FUNC              - User opration: Select Audio Input Function
 *  @li@c  CECM_USER_CTRL_PWR_TOGGLE_FUNC                    - User opration: Power Toggle Function
 *  @li@c  CECM_USER_CTRL_PWR_OFF_FUNC                       - User opration: Power off Function
 *  @li@c  CECM_USER_CTRL_PWR_ON_FUNC                        - User opration: Power on Function
 *  @li@c  CECM_USER_CTRL_F1_BLUE                            - User opration: F1(Blue)
 *  @li@c  CECM_USER_CTRL_F2_RED                             - User opration: F2(Red)
 *  @li@c  CECM_USER_CTRL_F3_GREEN                           - User opration: F3(Green)
 *  @li@c  CECM_USER_CTRL_F4_YELLOW                          - User opration: F4(Yellow)
 *  @li@c  CECM_USER_CTRL_F5                                 - User opration: F5
 *  @li@c  CECM_USER_CTRL_DATA                               - User opration: To enter or leave a digital TV data broadcast application
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_USER_CTRL_SELECT                   = 0x00,
    CECM_USER_CTRL_UP                       = 0x01,
    CECM_USER_CTRL_DOWN                     = 0x02,
    CECM_USER_CTRL_LEFT                     = 0x03,
    CECM_USER_CTRL_RIGHT                    = 0x04,
    CECM_USER_CTRL_RIGHT_UP                 = 0x05,
    CECM_USER_CTRL_RIGHT_DOWN               = 0x06,
    CECM_USER_CTRL_LEFT_UP                  = 0x07,
    CECM_USER_CTRL_LEFT_DOWN                = 0x08,
    CECM_USER_CTRL_ROOT_MENU                = 0x09,
    CECM_USER_CTRL_SETUP_MENU               = 0x0a,
    CECM_USER_CTRL_CONTENTS_MENU            = 0x0b,
    CECM_USER_CTRL_FAV_MENU                 = 0x0c,
    CECM_USER_CTRL_EXIT                     = 0x0d,
    CECM_USER_CTRL_MEDIA_TOP_MENU           = 0x10,
    CECM_USER_CTRL_MEDIA_CONTEXT_MENU       = 0x11,
    CECM_USER_CTRL_NUMBER_ENTRY_MODE        = 0x1d,
    CECM_USER_CTRL_NUM_11                   = 0x1e,
    CECM_USER_CTRL_NUM_12                   = 0x1f,
    CECM_USER_CTRL_NUM_0                    = 0x20,
    CECM_USER_CTRL_NUM_1                    = 0x21,
    CECM_USER_CTRL_NUM_2                    = 0x22,
    CECM_USER_CTRL_NUM_3                    = 0x23,
    CECM_USER_CTRL_NUM_4                    = 0x24,
    CECM_USER_CTRL_NUM_5                    = 0x25,
    CECM_USER_CTRL_NUM_6                    = 0x26,
    CECM_USER_CTRL_NUM_7                    = 0x27,
    CECM_USER_CTRL_NUM_8                    = 0x28,
    CECM_USER_CTRL_NUM_9                    = 0x29,
    CECM_USER_CTRL_DOT                      = 0x2a,
    CECM_USER_CTRL_ENTER                    = 0x2b,
    CECM_USER_CTRL_CLEAR                    = 0x2c,
    CECM_USER_CTRL_NEXT_FAV                 = 0x2f,
    CECM_USER_CTRL_CH_UP                    = 0x30,
    CECM_USER_CTRL_CH_DOWN                  = 0x31,
    CECM_USER_CTRL_PREV_CH                  = 0x32,
    CECM_USER_CTRL_SOUND_SELECT             = 0x33,
    CECM_USER_CTRL_INPUT_SELECT             = 0x34,
    CECM_USER_CTRL_DISPLAY_INFO             = 0x35,
    CECM_USER_CTRL_HELP                     = 0x36,
    CECM_USER_CTRL_PAGE_UP                  = 0x37,
    CECM_USER_CTRL_PAGE_DOWN                = 0x38,
    CECM_USER_CTRL_POWER                    = 0x40,
    CECM_USER_CTRL_VOL_UP                   = 0x41,
    CECM_USER_CTRL_VOL_DOWN                 = 0x42,
    CECM_USER_CTRL_MUTE                     = 0x43,
    CECM_USER_CTRL_PLAY                     = 0x44,
    CECM_USER_CTRL_STOP                     = 0x45,
    CECM_USER_CTRL_PAUSE                    = 0x46,
    CECM_USER_CTRL_RECORD                   = 0x47,
    CECM_USER_CTRL_REWIND                   = 0x48,
    CECM_USER_CTRL_FAST_FORDWARD            = 0x49,
    CECM_USER_CTRL_EJECT                    = 0x4a,
    CECM_USER_CTRL_FORWARD                  = 0x4b,
    CECM_USER_CTRL_BACKWARD                 = 0x4c,
    CECM_USER_CTRL_STOP_REC                 = 0x4d,
    CECM_USER_CTRL_PAUSE_REC                = 0x4e,
    CECM_USER_CTRL_ANGLE                    = 0x50,
    CECM_USER_CTRL_SUB_PIC                  = 0x51,
    CECM_USER_CTRL_VOD                      = 0x52,
    CECM_USER_CTRL_EPG                      = 0x53,
    CECM_USER_CTRL_TIMER_PROG               = 0x54,
    CECM_USER_CTRL_INIT_CONFIG              = 0x55,
    CECM_USER_CTRL_SELECT_BRDCST_TYPE       = 0x56,
    CECM_USER_CTRL_SELECT_SOUND_PRESEN      = 0X57,
    CECM_USER_CTRL_PLAY_FUNC                = 0x60,
    CECM_USER_CTRL_PAUSE_PLAY_FUNC          = 0x61,
    CECM_USER_CTRL_REC_FUNC                 = 0x62,
    CECM_USER_CTRL_PAUSE_REC_FUNC           = 0x63,
    CECM_USER_CTRL_STOP_FUNC                = 0x64,
    CECM_USER_CTRL_MUTE_FUNC                = 0x65,
    CECM_USER_CTRL_RESTORE_VOL_FUNC         = 0x66,
    CECM_USER_CTRL_TUNE_FUNC                = 0x67,
    CECM_USER_CTRL_SELECT_MEDIA_FUNC        = 0x68,
    CECM_USER_CTRL_SELECT_AV_INPUT_FUNC     = 0x69,
    CECM_USER_CTRL_SELECT_AUD_INPUT_FUNC    = 0x6a,
    CECM_USER_CTRL_PWR_TOGGLE_FUNC          = 0x6b,
    CECM_USER_CTRL_PWR_OFF_FUNC             = 0x6c,
    CECM_USER_CTRL_PWR_ON_FUNC              = 0x6d,
    CECM_USER_CTRL_F1_BLUE                  = 0x71,
    CECM_USER_CTRL_F2_RED                   = 0x72,
    CECM_USER_CTRL_F3_GREEN                 = 0x73,
    CECM_USER_CTRL_F4_YELLOW                = 0x74,
    CECM_USER_CTRL_F5                       = 0x75,
    CECM_USER_CTRL_DATA                     = 0x76
}   CECM_USER_CTRL_T;

/*User control with operand */

/** User control operand information  */
typedef UINT8 CECM_USER_CTRL_OPERAND_T[CECM_USER_CTRL_OPERAND_SIZE];

/*------------------------------------------------------------------*/
/*! @struct   CECM_USER_CTRL_OPERAND_INFO_T
 *  @brief    User ctrl with operand info
 *  @code
 *  typedef struct _CECM_USER_CTRL_OPERAND_INFO_T
 *  {
 *      CECM_USER_CTRL_OPERAND_T      t_user_ctrl_operand;
 *      SIZE_T                      t_user_ctrl_operand_size;
 *  }   CECM_USER_CTRL_OPERAND_INFO_T;
 *  @endcode
 *  @li@c  t_user_ctrl_operand                       - User ctrl operand info.
 *  @li@c  t_user_ctrl_operand_size                  - User ctrl operand size.
 */
/*------------------------------------------------------------------*/

typedef struct _CECM_USER_CTRL_OPERAND_INFO_T
{
    CECM_USER_CTRL_OPERAND_T      t_user_ctrl_operand;
    SIZE_T                      t_user_ctrl_operand_size;
}   CECM_USER_CTRL_OPERAND_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct   CECM_USER_CTRL_WITH_OPERAND_T
 *  @brief    User ctrl with operand info
 *  @code
 *  typedef struct _CECM_USER_CTRL_WITH_OPERAND_T
 *  {
 *      CECM_USER_CTRL_T                e_user_ctrl;
 *      CECM_USER_CTRL_OPERAND_INFO_T   t_user_ctrl_operand_info;   
 *  }   CECM_USER_CTRL_WITH_OPERAND_T;
 *  @endcode
 *  @li@c  e_user_ctrl                               - User ctrl info.
 *  @li@c  t_user_ctrl_operand_info                  - User ctrl operand info.
 */
/*------------------------------------------------------------------*/

typedef struct _CECM_USER_CTRL_WITH_OPERAND_T
{
    CECM_USER_CTRL_T                e_user_ctrl;
    CECM_USER_CTRL_OPERAND_INFO_T   t_user_ctrl_operand_info;   
}   CECM_USER_CTRL_WITH_OPERAND_T;
/*Select broadcast type*/
typedef enum 
{
    CECM_UI_BRDCST_TYPE_TOGGLE_ALL                      = 0x00,
    CECM_UI_BRDCST_TYPE_TOGGLE_DIG_ANA                  = 0x01,
    CECM_UI_BRDCST_TYPE_ANA                             = 0x10,
    CECM_UI_BRDCST_TYPE_ANA_TERRSATRIAL                 = 0x20,
    CECM_UI_BRDCST_TYPE_ANA_CABLE                       = 0x30,
    CECM_UI_BRDCST_TYPE_ANA_STATELLITE                  = 0x40,
    CECM_UI_BRDCST_TYPE_DIG                             = 0x50,
    CECM_UI_BRDCST_TYPE_DIT_TERRSATRIAL                 = 0x60,
    CECM_UI_BRDCST_TYPE_DIG_CABEL                       = 0x70,
    CECM_UI_BRDCST_TYPE_DIG_STATELLITE                  = 0x80,
    CECM_UI_BRDCST_TYPE_DIG_COMMU_STATELLITE            = 0x90,
    CECM_UI_BRDCST_TYPE_DIG_COMMU_STATELLITE_2          = 0X91,
    CECM_UI_BRDCST_TYPE_IP                              = 0xA0,
    CECM_UI_BRDCST_TYPE_MAX                             = 0xff
}   CECM_UI_BRDCST_TYPE_T;


/* Power_on_device status */
/*------------------------------------------------------------------*/
/*! @struct   CECM_PWR_ON_DEV_STS_T
 *  @brief    Contains the device power status information, used for notify clients
 *  @code
 *  typedef struct _CECM_PWR_ON_DEV_STS_T
 *  {
 *      CECM_PWR_STS_T      e_pwr_sts;
 *      UINT16              ui2_pa;
 *  }   CECM_PWR_ON_DEV_STS_T;
 *  @endcode
 *  @li@c  e_pwr_sts                                         - The power status of device.
 *  @li@c  ui2_pa                                            - The physical address of device.
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_PWR_ON_DEV_STS_T
{
    CECM_PWR_STS_T      e_pwr_sts;
    UINT16              ui2_pa;
}   CECM_PWR_ON_DEV_STS_T;

/* General Protocol Messages *************************************************/
/* Abort reason */
/*------------------------------------------------------------------*/
/*! @enum    CECM_ABORT_REASON_T
 *  @brief   List all feature abort reason
 *  @code
 *  typedef enum
 *  {
 *      CECM_ABORT_REASON_UNRECOGNIZED          = 0,
 *      CECM_ABORT_REASON_INCORRECT_MOD_RESPOND = 1,
 *      CECM_ABORT_REASON_CANNOT_PROVIDE_SRC    = 2,
 *      CECM_ABORT_REASON_INV_OPERAND           = 3,
 *      CECM_ABORT_REASON_REFUSED               = 4,
 *      CECM_ABORT_REASON_UNABLE_TO_DETERMINE   = 5,
 *      CECM_ABORT_REASON_NONE                  = 0xff
 *  }   CECM_ABORT_REASON_T;
 *  @endcode
 *  @li@c  CECM_ABORT_REASON_UNRECOGNIZED                    -  Do not support the opcode
 *  @li@c  CECM_ABORT_REASON_INCORRECT_MOD_RESPOND           -  In incorrect mode, so do not support the operation
 *  @li@c  CECM_ABORT_REASON_CANNOT_PROVIDE_SRC              -  There is no active source when One Touch Record
 *  @li@c  CECM_ABORT_REASON_INV_OPERAND                     -  The operand is invalid.
 *  @li@c  CECM_ABORT_REASON_REFUSED                         -  Refuse the operation.
 *  @li@c  CECM_ABORT_REASON_UNABLE_TO_DETERMINE             -  The device is in a state where can not determine if the opcode or operands are supported
 *  @li@c  CECM_ABORT_REASON_NONE                            -  Abort reason none
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_ABORT_REASON_UNRECOGNIZED          = 0,
    CECM_ABORT_REASON_INCORRECT_MOD_RESPOND = 1,
    CECM_ABORT_REASON_CANNOT_PROVIDE_SRC    = 2,
    CECM_ABORT_REASON_INV_OPERAND           = 3,
    CECM_ABORT_REASON_REFUSED               = 4,
    CECM_ABORT_REASON_UNABLE_TO_DETERMINE   = 5,
    CECM_ABORT_REASON_NONE                  = 0xff
}   CECM_ABORT_REASON_T;

/*------------------------------------------------------------------*/
/*! @struct    CECM_FEATURE_ABORT_T
 *  @brief     Feature abort information
 *  @code
 *  typedef struct _CECM_FEATURE_ABORT_T
 *  {
 *      CECM_OPCODE_T           e_opcode;
 *      CECM_ABORT_REASON_T     e_abort_reason;
 *  }   CECM_FEATURE_ABORT_T;
 *  @endcode
 *  @li@c  e_opcode                                          - The opcode which feature abort by the device.
 *  @li@c  e_abort_reason                                    - The feature abort reason.
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_FEATURE_ABORT_T
{
    CECM_OPCODE_T           e_opcode;
    CECM_ABORT_REASON_T     e_abort_reason;
}   CECM_FEATURE_ABORT_T;

/* System Audio Control Feature **********************************************/
/*------------------------------------------------------------------*/
/*! @enum    CECM_SYS_AUD_STS_T
 *  @brief   Control system audio status on or off
 *  @code
 *  typedef enum
 *  {
 *      CECM_SYS_AUD_STS_OFF = 0,
 *      CECM_SYS_AUD_STS_ON  = 1
 *  }   CECM_SYS_AUD_STS_T;
 *  @endcode
 *  @li@c  CECM_SYS_AUD_STS_OFF                              - System audio status off
 *  @li@c  CECM_SYS_AUD_STS_ON                               - System audio status on
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_SYS_AUD_STS_OFF = 0,
    CECM_SYS_AUD_STS_ON  = 1
}   CECM_SYS_AUD_STS_T;

/*------------------------------------------------------------------*/
/*! @struct    CECM_SYS_AUD_CTRL_T
 *  @brief     System audio control information
 *  @code
 *  typedef struct _CECM_SYS_AUD_CTRL_T
 *  {
 *      CECM_SYS_AUD_STS_T  e_aud_ctrl;
 *      UINT16              ui2_comp_id;
 *      UINT16              ui2_pa;
 *  }   CECM_SYS_AUD_CTRL_T;
 *  @endcode
 *  @li@c  e_aud_ctrl                                        - Audio control of or on
 *  @li@c  ui2_comp_id                                       - HDMI prot id
 *  @li@c  ui2_pa                                            - Device physical address
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_SYS_AUD_CTRL_T
{
    CECM_SYS_AUD_STS_T  e_aud_ctrl;
    UINT16              ui2_comp_id;
    UINT16              ui2_pa;
}   CECM_SYS_AUD_CTRL_T;

/*------------------------------------------------------------------*/
/*! @enum    CECM_AUD_FMT_CODE_T
 *  @brief   Audio format code
 *  @code
 *  typedef enum
 *  {
 *      CECM_AUD_FMT_CODE_RESERVED      = 0,
 *      CECM_AUD_FMT_CODE_PCM           = 1,
 *      CECM_AUD_FMT_CODE_AC3           = 2,
 *      CECM_AUD_FMT_CODE_MPEG1         = 3,
 *      CECM_AUD_FMT_CODE_MP3           = 4,
 *      CECM_AUD_FMT_CODE_MPEG2         = 5,
 *      CECM_AUD_FMT_CODE_AAC           = 6,
 *      CECM_AUD_FMT_CODE_DTS           = 7,
 *      CECM_AUD_FMT_CODE_ATRAC         = 8,
 *      CECM_AUD_FMT_CODE_ONE_BIT_AUD   = 9,
 *      CECM_AUD_FMT_CODE_DOLBY_DIGITAL = 10,
 *      CECM_AUD_FMT_CODE_DTS_HD        = 11,
 *      CECM_AUD_FMT_CODE_MAP           = 12,
 *      CECM_AUD_FMT_CODE_DST           = 13,
 *      CECM_AUD_FMT_CODE_WMA_PRO       = 14,
 *      CECM_AUD_FMT_CODE_RESERVED_EX   = 15
 *  }   CECM_AUD_FMT_CODE_T;
 *  @endcode
 *  @li@c  CECM_AUD_FMT_CODE_RESERVED                        - Reserved
 *  @li@c  CECM_AUD_FMT_CODE_PCM                             - Audio format Linear PCM
 *  @li@c  CECM_AUD_FMT_CODE_AC3                             - Audio format AC-3
 *  @li@c  CECM_AUD_FMT_CODE_MPEG1                           - Audio format MPEG1
 *  @li@c  CECM_AUD_FMT_CODE_MP3                             - Audio format MP3
 *  @li@c  CECM_AUD_FMT_CODE_MPEG2                           - Audio format MPEG2
 *  @li@c  CECM_AUD_FMT_CODE_AAC                             - Audio format AAC
 *  @li@c  CECM_AUD_FMT_CODE_DTS                             - Audio format DTS
 *  @li@c  CECM_AUD_FMT_CODE_ATRAC                           - Audio format ATRAC
 *  @li@c  CECM_AUD_FMT_CODE_ONE_BIT_AUD                     - Audio format One Bit Audio
 *  @li@c  CECM_AUD_FMT_CODE_DOLBY_DIGITAL                   - Audio format Dolby Digital +
 *  @li@c  CECM_AUD_FMT_CODE_DTS_HD                          - Audio format DTS_HD
 *  @li@c  CECM_AUD_FMT_CODE_MAP                             - Audio format MAP(MLP)
 *  @li@c  CECM_AUD_FMT_CODE_DST                             - Audio format DST
 *  @li@c  CECM_AUD_FMT_CODE_WMA_PRO                         - Audio format WMA Pro
 *  @li@c  CECM_AUD_FMT_CODE_RESERVED_EX                     - Reserved for audio format 15
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_AUD_FMT_CODE_RESERVED      = 0,
    CECM_AUD_FMT_CODE_PCM           = 1,
    CECM_AUD_FMT_CODE_AC3           = 2,
    CECM_AUD_FMT_CODE_MPEG1         = 3,
    CECM_AUD_FMT_CODE_MP3           = 4,
    CECM_AUD_FMT_CODE_MPEG2         = 5,
    CECM_AUD_FMT_CODE_AAC           = 6,
    CECM_AUD_FMT_CODE_DTS           = 7,
    CECM_AUD_FMT_CODE_ATRAC         = 8,
    CECM_AUD_FMT_CODE_ONE_BIT_AUD   = 9,
    CECM_AUD_FMT_CODE_DOLBY_DIGITAL = 10,
    CECM_AUD_FMT_CODE_DTS_HD        = 11,
    CECM_AUD_FMT_CODE_MAP           = 12,
    CECM_AUD_FMT_CODE_DST           = 13,
    CECM_AUD_FMT_CODE_WMA_PRO       = 14,
    CECM_AUD_FMT_CODE_RESERVED_EX   = 15
}   CECM_AUD_FMT_CODE_T;

/*------------------------------------------------------------------*/
/*! @struct    CECM_AUD_DESCRIPTOR_T
 *  @brief     Short audio descriptor information
 *  @code
 *  typedef struct _CECM_AUD_DESCRIPTOR_T
 *  {
 *      UINT8   ui1_short_aud_descriptor_num;
 *      CECM_SHORT_AUD_DESCRIPTOR_T   t_aud_short_aud_descriptor[CECM_AUD_FMT_CODE_MAX_SIZE];
 *  }   CECM_AUD_DESCRIPTOR_T;
 *  @endcode
 *  @li@c  ui1_short_aud_descriptor_num                              - Short audio descriptor number
 *  @li@c  t_aud_short_aud_descriptor[CECM_AUD_FMT_CODE_MAX_SIZE]    - Short audio descriptor
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_AUD_DESCRIPTOR_T
{
    UINT8   ui1_short_aud_descriptor_num;
    CECM_SHORT_AUD_DESCRIPTOR_T   t_aud_short_aud_descriptor[CECM_AUD_FMT_CODE_MAX_SIZE];
}   CECM_AUD_DESCRIPTOR_T;

/*------------------------------------------------------------------*/
/*! @struct    CECM_AUD_FMT_T
 *  @brief     Audio format information, used for request audio descriptor
 *  @code
 *  typedef struct _CECM_AUD_FMT_T
 *  {
 *      UINT8   ui1_aud_fmt_code_num;
 *      CECM_AUD_FMT_CODE_T   ae_aud_fmt_code[CECM_AUD_FMT_CODE_MAX_SIZE];
 *  }   CECM_AUD_FMT_T;
 *  @endcode
 *  @li@c  ui1_aud_fmt_code_num                              - Audio format code number
 *  @li@c  ae_aud_fmt_code[CECM_AUD_FMT_CODE_MAX_SIZE]       - Audio format code
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_AUD_FMT_T
{
    UINT8   ui1_aud_fmt_code_num;
    CECM_AUD_FMT_CODE_T   ae_aud_fmt_code[CECM_AUD_FMT_CODE_MAX_SIZE];
}   CECM_AUD_FMT_T;

/* Audio Rate Control Feature ************************************************/
/* Audio rate */
/*------------------------------------------------------------------*/
/*! @enum    CECM_AUD_RATE_T
 *  @brief   The audio rate information, used for set the device audio rate
 *  @code
 *  typedef enum
 *  {
 *      CECM_AUD_RATE_CTRL_OFF                  = 0,
 *      CECM_AUD_RATE_WIDE_RANGE_CTRL_STD       = 1,
 *      CECM_AUD_RATE_WIDE_RANGE_CTRL_FAST      = 2,
 *      CECM_AUD_RATE_WIDE_RANGE_CTRL_SLOW      = 3,
 *      CECM_AUD_RATE_NARROW_RANGE_CTRL_STD     = 4,
 *      CECM_AUD_RATE_NARROW_RANGE_CTRL_FAST    = 5,
 *      CECM_AUD_RATE_NARROW_RANGE_CTRL_SLOW    = 6
 *  }   CECM_AUD_RATE_T;
 *  @endcode
 *  @li@c  CECM_AUD_RATE_CTRL_OFF                            -  Audio rate control off
 *  @li@c  CECM_AUD_RATE_WIDE_RANGE_CTRL_STD                 -  Widge range control standard rate 
 *  @li@c  CECM_AUD_RATE_WIDE_RANGE_CTRL_FAST                -  Widge range control fast rate
 *  @li@c  CECM_AUD_RATE_WIDE_RANGE_CTRL_SLOW                -  Widge range control slow rate
 *  @li@c  CECM_AUD_RATE_NARROW_RANGE_CTRL_STD               -  Narrow range control stand rate
 *  @li@c  CECM_AUD_RATE_NARROW_RANGE_CTRL_FAST              -  Narrow range control fast rate
 *  @li@c  CECM_AUD_RATE_NARROW_RANGE_CTRL_SLOW              -  Narrow range control slow rate
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_AUD_RATE_CTRL_OFF                  = 0,
    CECM_AUD_RATE_WIDE_RANGE_CTRL_STD       = 1,
    CECM_AUD_RATE_WIDE_RANGE_CTRL_FAST      = 2,
    CECM_AUD_RATE_WIDE_RANGE_CTRL_SLOW      = 3,
    CECM_AUD_RATE_NARROW_RANGE_CTRL_STD     = 4,
    CECM_AUD_RATE_NARROW_RANGE_CTRL_FAST    = 5,
    CECM_AUD_RATE_NARROW_RANGE_CTRL_SLOW    = 6
}   CECM_AUD_RATE_T;
/*  Device discovery config information*/

/*device discovery vendor information*/
/*------------------------------------------------------------------*/
/*! @struct  CECM_DD_VNDR_CMD_PSR_INFO_T
 *  @brief   Device discovery vendor command parse information
 *  @code
 *  typedef struct
 *  {
 *      BOOL    b_save_vndr_cmd;   
 *      UINT32  ui4_update_mask;    
 *  }   CECM_DD_VNDR_CMD_INFO;
 *  @endcode
 *  @li@c  b_save_vndr_cmd              - If the vendor command if response for device discovery vendor command or not            
 *  @li@c  ui4_update_mask              - Specify which information should be skip 
 *                                        if received response for device disvovery vendor command
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_DD_VNDR_CMD_PSR_INFO_T
{
    /* If the vendor command is response of device discovery vendor command*/
    BOOL    b_save_vndr_cmd;       

    /* If the device response device discovery vendor command, should skip other step for discovery*/
    UINT32  ui4_update_mask;                  
}   CECM_DD_VNDR_CMD_PSR_INFO_T;


/* Discovery info, specify device discovery should get which infomation */
/*------------------------------------------------------------------*/
/*! @struct  CECM_DD_GET_INFO_T
 *  @brief   Device discovery vendor command information
 *  @code
 *  typedef struct
 *  {
 *      BOOL    b_phy_address;
 *      BOOL    b_dev_vndr_id;
 *      BOOL    b_dev_vndr_info;
 *      BOOL    b_cec_ver;
 *      BOOL    b_osd_name;
 *      BOOL    b_pwr_sts;
 *  }   CECM_DD_GET_INFO;
 *  @endcode
 *  @li@c  b_phy_address        - Get physical address          
 *  @li@c  b_dev_vndr_id        - Get device vendor id
 *  @li@c  b_dev_vndr_info      - Get device vendor information
 *  @li@c  b_cec_ver            - Get device cec version
 *  @li@c  b_osd_name           - Get device OSD name
 *  @li@c  b_pwr_sts            - Get power staus
 */
/*------------------------------------------------------------------*/

typedef struct _CECM_DD_GET_INFO_T
{
    BOOL    b_phy_address;
    BOOL    b_dev_vndr_id;
    BOOL    b_dev_vndr_info;
    BOOL    b_cec_ver;
    BOOL    b_osd_name;
    BOOL    b_pwr_sts;
}   CECM_DD_GET_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum    CECM_VNDR_CMD_TYPE_T
 *  @brief   Device discovery vendor command type
 *  @code
 *  typedef enum
 *  {
 *      CECM_VNDR_CMD = 0,
 *      CECM_VNDR_CMD_WITH_ID
 *  }   CECM_VNDR_CMD_TYPE_T;
 *  @endcode
 *  @li@c  CECM_VNDR_CMD                    - Vendor command               
 *  @li@c  CECM_VNDR_CMD_WITH_ID            - Vendor command with ID
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CECM_VNDR_CMD = 0,
    CECM_VNDR_CMD_WITH_ID   
}   CECM_VNDR_CMD_TYPE_T;
/*Discovery vendor command, specify the vendor command used for device discovery*/
/*------------------------------------------------------------------*/
/*! @struct  CECM_DD_VNDR_CMD_INFO_T
 *  @brief   Device discovery vendor command information
 *  @code
 *  typedef struct _CECM_DD_VNDR_CMD_INFO_T
 *  {
 *      union
 *      {
 *          CECM_VNDR_CMD_DATA_T           t_vndr_cmd_data;
 *          CECM_VNDR_CMD_WITH_ID_DATA_T   t_vndr_cmd_with_id_data;
 *      } u_data;
 *
 *      CECM_VNDR_CMD_TYPE_T                e_vndr_cmd_type;
 *  }   CECM_DD_VNDR_CMD_INFO_T
 *  @endcode
 *  @li@c  t_vndr_cmd_data              - Device discovery vendor command data                
 *  @li@c  t_vndr_cmd_with_id_data      - Device discovery vendor command with ID data
 *  @li@c  e_vndr_cmd_type              - Device discovery vendor command type
 */
/*------------------------------------------------------------------*/

typedef struct _CECM_DD_VNDR_CMD_INFO_T
{
    union
    {
        CECM_VNDR_CMD_DATA_T           t_vndr_cmd_data;
        CECM_VNDR_CMD_WITH_ID_DATA_T   t_vndr_cmd_with_id_data;
    } u_data;
    
    CECM_VNDR_CMD_TYPE_T                e_vndr_cmd_type;
}   CECM_DD_VNDR_CMD_INFO_T;
/*------------------------------------------------------------------*/
/*! @struct  CECM_DD_VNDR_CMD_T
 *  @brief   Device discovery vendor command, used for config device discovery 
 *  @code
 *  typedef struct _CECM_DD_VNDR_CMD_T
 *  {
 *      CECM_VNDR_CMD_DATA_T    at_vndr_cmd_data[CECM_DD_VNDR_CMD_MAX_SIZE];
 *      SIZE_T                  z_dd_vndr_cmd_size;
 *  }   CECM_DD_VNDR_CMD_T
 *  @endcode
 *  @li@c  at_vndr_cmd_data[CECM_DD_VNDR_CMD_MAX_SIZE]  - Device discovery vendor command                
 *  @li@c  z_dd_vndr_cmd_size                           - Device discovery vendor command size
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_DD_VNDR_CMD_T
{
    CECM_DD_VNDR_CMD_INFO_T         at_vndr_cmd_info[CECM_DD_VNDR_CMD_MAX_SIZE];
    SIZE_T                          z_dd_vndr_cmd_size;
}   CECM_DD_VNDR_CMD_T;

/* Used for parse device discovery vendor command */
/*------------------------------------------------------------------*/
/*! @brief       This function used for parse vendor command
 *  @param [in]  CECM_VNDR_CMD_DATA_T     - Vendor command response from device
 *  @param [out] CECM_DD_VNDR_CMD_INFO    - Parse vendor command result
 *  @return    None
 */
/*------------------------------------------------------------------*/

typedef VOID (*x_cecm_parse_dd_vndr_cmd)(CECM_DD_VNDR_CMD_INFO_T*  pt_vndr_cmd_info,
                                            CECM_DD_VNDR_CMD_PSR_INFO_T* pt_dd_vndr_cmd_info);

/* Device Disovery config info*/
/*------------------------------------------------------------------*/
/*! @struct  CECM_DEV_DISCOVERY_CONFIG_T
 *  @brief   Device discovery config information
 *  @code
 *  typedef struct
 *  {
 *      CECM_DD_GET_INFO             t_dd_get_info;
 *      CECM_DD_VNDR_CMD             t_dd_vndr_cmd;
 *      x_cecm_parse_dd_vndr_cmd     pf_cecm_parse_dd_vndr_cmd;
 *  }   CECM_DEV_DISCOVERY_CONFIG;
 *  @endcode
 *  @li@c  t_dd_get_info                  -  Specif device discovery shoul get which infomation
 *  @li@c  t_dd_vndr_cmd                  -  Specify vendor command used for device discovery
 *  @li@c  pf_cecm_parse_dd_vndr_cmd      -  Used for parse vendor command respons from device
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_DEV_DISCOVERY_CONFIG_T
{
    CECM_DD_GET_INFO_T             t_dd_get_info;
    CECM_DD_VNDR_CMD_T             t_dd_vndr_cmd;
    CECM_DEV_VNDR_ID_T             t_dev_vndr_id;
    x_cecm_parse_dd_vndr_cmd       pf_cecm_parse_dd_vndr_cmd;
    UINT8                          ui1_polling_retry_num;     
}   CECM_DEV_DISCOVERY_CONFIG_T;

/*------------------------------------------------------------------*/
/*! @struct    CECM_DEV_VNDR_INFO_T
 *  @brief     Vendor information get by device discovery
 *  @code
 *  typedef struct _CECM_DD_VNDR_INFO
 *  {
 *      CECM_DD_VNDR_CMD_INFO_T at_vndr_cmd_info[CECM_DD_VNDR_CMD_MAX_SIZE];
 *  }   CECM_DEV_VNDR_INFO_T;
 *  @endcode
 *  @li@c  at_vndr_cmd_data[CECM_DD_VNDR_CMD_MAX_SIZE]      - Vendor command data that device response for device discovery
 */
/*------------------------------------------------------------------*/

/* Device vendor information*/
typedef struct _CECM_DD_VNDR_INFO
{
    
    CECM_DD_VNDR_CMD_INFO_T at_vndr_cmd_info[CECM_DD_VNDR_CMD_MAX_SIZE];
    
}   CECM_DEV_VNDR_INFO_T;


/* CECM device */
/*------------------------------------------------------------------*/
/*! @struct    CECM_DEV_T
 *  @brief     The CEC device information
 *  @code
 *  typedef struct _CECM_DEV_T
 *  {
 *      CECM_DEV_STATE_T        e_dev_state;
 *      BOOL                    b_dev_present;
 *      BOOL                    b_act_src;
 *      CECM_LOG_ADDR_T         e_la;
 *      CECM_REPORT_PHY_ADDR_T  t_report_phy_addr;
 *      CECM_DEV_VNDR_ID_T      t_dev_vndr_id;
 *      UTF16_T                 aw2_osd_name[CECM_MAX_OPERAND_SIZE];
 *  }   CECM_DEV_T;
 *  @endcode
 *  @li@c  e_dev_state                                       - Device state
 *  @li@c  b_dev_present                                     - If the device present
 *  @li@c  b_act_src                                         - If the device is as active source
 *  @li@c  e_la                                              - Device logical address
 *  @li@c  t_report_phy_addr                                 - Device phisical address information 
 *  @li@c  t_dev_vndr_id                                     - Device vendor ID
 *  @li@c  aw2_osd_name[CECM_MAX_OPERAND_SIZE]               - Device OSD name
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_DEV_T
{
    CECM_DEV_STATE_T        e_dev_state;
    BOOL                    b_dev_present;
    BOOL                    b_act_src;
    CECM_LOG_ADDR_T         e_la;
    CECM_REPORT_PHY_ADDR_T  t_report_phy_addr;
    CECM_DEV_VNDR_ID_T      t_dev_vndr_id;
    CECM_DEV_VNDR_INFO_T    t_dev_vndr_info;
    CECM_CEC_VER_T          e_cec_ver;
    UTF16_T                 aw2_osd_name[CECM_OSD_NAME_SIZE];
    CECM_PWR_STS_T          e_pwr_sts;
    CECM_MENU_REQ_STATE_T   e_menu_req_state;
}   CECM_DEV_T;


/* CECM notification */
/*------------------------------------------------------------------*/
/*! @struct  CECM_NFY_T
 *  @brief   CECM notify information 
 *  @code
 *  typedef struct _CECM_NFY_T
 *  {
 *      CECM_LOG_ADDR_T         e_sent_la;
 *      CECM_OPCODE_T           e_sent_opcode;
 *      CECM_NFY_TYPE_T         e_nfy_type;
 *      CECM_BRDCST_TYPE_T      e_brdcst_type;
 *      UINT32                  ui4_attr;
 *      union
 *      {
 *          CECM_MSGCODE_T      e_msgcode;  
 *          CECM_OPCODE_T       e_opcode;   
 *      }   u_code;
 *      union
 *      {
 *          CECM_DEV_LIST_NFY_T             t_dev_lst_nfy;
 *          CECM_ACK_COND_T                 e_ack_cond;
 *          CECM_ONE_TOUCH_PLAY_T           t_one_touch_play;
 *          CECM_PWR_ON_DEV_STS_T           t_pwr_on_dev_sts;
 *          UINT16                          ui2_pa;
 *          CECM_LOG_ADDR_T                 e_la;
 *          CECM_ACT_SRC_T                  t_act_src;
 *          CECM_REC_STS_INFO_T             e_rec_sts_info;
 *          CECM_DECK_CTRL_MOD_T            e_deck_ctrl_mod;
 *          CECM_DECK_INFO_T                e_deck_info;
 *          CECM_PLAY_MOD_T                 e_play_mod;
 *          CECM_MENU_REQ_STATE_T           e_menu_req_state;
 *          CECM_USER_CTRL_T                e_usr_ctrl;
 *          CECM_CEC_VER_T                  e_cec_ver;
 *          ISO_639_LANG_T                  t_menu_lang;
 *          CECM_REPORT_PHY_ADDR_T          t_report_phy_addr;
 *          CECM_DEV_VNDR_ID_T              t_dev_vndr_id;
 *          UTF16_T                         aw2_osd_name[CECM_MAX_OPERAND_SIZE];
 *          CECM_SYS_AUD_STS_T              e_sys_aud_sts;
 *          CECM_AUD_DESCRIPTOR_T           t_aud_descriptor;
 *          UINT8                           ui1_aud_sts;
 *          CECM_TIMER_CLRED_STS_T          e_timer_clred_sts;
 *          CECM_TIMER_STS_T                t_timer_sts;
 *          CECM_TUNER_DEV_INFO_T           t_tuner_dev_info;
 *          CECM_OSD_STRING_INFO_T          t_osd_string_info;
 *          CECM_ROUTING_CHG_INFO_T         t_routing_chg_info;
 *          CECM_ABORT_REASON_T             e_abort_reason;
 *        	CECM_VNDR_CMD_DATA_T            t_vndr_cmd_data;
 *        	CECM_VNDR_CMD_WITH_ID_DATA_T    t_vndr_cmd_with_id_data;
 *        	CECM_VNDR_RC_CODE_DATA_T        t_vndr_rc_code_data;
 *          CECM_PWR_STS_T                  e_pwr_sts;
 *      }   u_data;
 *  }   CECM_NFY_T;
 *  @endcode
 *  @li@c  e_sent_la                                         - The logical address of device, if  set as the initiator device logical address.
 *  @li@c  e_sent_opcode                                     - The opcode sent to the device
 *  @li@c  e_nfy_type                                        - Indicates notify type, message code or opcode.
 *  @li@c  e_brdcst_type                                     - If the notify type is message code, set the value as type none, else set the value as directly or broadcast by the message send type.
 *  @li@c  ui4_attr                                          - Notify attribute
 *  @li@c  e_msgcode                                         - The message code, only be used for the notify type is message code
 *  @li@c  e_opcode                                          - Indicate the opcode, only be used for the notify type is opcode.
 *  @li@c  t_dev_lst_nfy                                     - The device list notify information
 *  @li@c  e_ack_cond                                        - The ACK information, notify the clients there is response or not
 *  @li@c  t_one_touch_play                                  - The One Touch Play information, used for notifies clients the One Touch Play result.
 *  @li@c  t_pwr_on_dev_sts                                  - The power status information, used for notifies clients the Power On Device result.
 *  @li@c  ui2_pa                                            - The physical address of device.
 *  @li@c  e_la                                              - The logical address of device
 *  @li@c  t_act_src                                         - Used for notify clients the active source
 *  @li@c  e_rec_sts_info                                    - The record status information notify to clients.
 *  @li@c  e_deck_ctrl_mod                                   - The deck control mode notify to clients.
 *  @li@c  e_deck_info                                       - The playback device current state
 *  @li@c  e_play_mod                                        - The current play mode
 *  @li@c  e_menu_req_state                                  - The current menu state notify to clients.
 *  @li@c  e_usr_ctrl                                        - The value of user control.
 *  @li@c  e_cec_ver                                         - Contains the CEC version notify to clients.
 *  @li@c  t_menu_lang                                       - The menu language of device used currently notify to clients
 *  @li@c  t_report_phy_addr                                 - Notify the clients the reportphysical address of device.
 *  @li@c  t_dev_vndr_id                                     - The vendor ID.
 *  @li@c  aw2_osd_name[CECM_MAX_OPERAND_SIZE]               - Notify clients the name of the device should show.
 *  @li@c  e_sys_aud_sts                                     - The system audio control status, may be on or off
 *  @li@c  t_aud_descriptor                                  - Short audio descriptor
 *  @li@c  ui1_aud_sts                                       - The current audio status, mute or volume.
 *  @li@c  e_timer_clred_sts                                 - Clear timer status. 
 *  @li@c  t_timer_sts                                       - Timer status
 *  @li@c  t_tuner_dev_info                                  - Notify clients the tuner information.
 *  @li@c  t_osd_string_info                                 - Notify clients the string should be display
 *  @li@c  t_routing_chg_info                                - The routing change information.
 *  @li@c  e_abort_reason                                    - Notify clients the abort reason. 	
 *  @li@c  t_vndr_cmd_data                                   - Used for notify clients the opcode CECM_OPCODE_VNDR_CMD operand
 *  @li@c  t_vndr_cmd_with_id_data                           - Used for notify clients the opcode CECM_OPCODE_VNDR_WITH_ID operand
 *  @li@c  t_vndr_rc_code_data                               - Used for notify clients the opcode CECM_OPCODE_VNDR_REMOTE_BTN_DOWN operand                                                                                                         
 *  @li@c  e_pwr_sts                                         - Notify clients the device power status.
 */
/*------------------------------------------------------------------*/
typedef struct _CECM_NFY_T
{
    /****** Data of TV -> dev ******/
    /* If the message is sent by a device actively, e_sent_la stands for device LA */
    CECM_LOG_ADDR_T         e_sent_la;
    CECM_OPCODE_T           e_sent_opcode;

    /****** Data of dev -> TV ******/
    CECM_NFY_TYPE_T         e_nfy_type;
    CECM_BRDCST_TYPE_T      e_brdcst_type;
    UINT32                  ui4_attr;

    union
    {
        CECM_MSGCODE_T      e_msgcode;  /* e_nfy_type = CECM_NFY_TYPE_MSGCODE */
        CECM_OPCODE_T       e_opcode;   /* e_nfy_type = CECM_NFY_TYPE_OPCODE */
    }   u_code;

    union
    {
        /* e_nfy_type = CECM_NFY_TYPE_MSGCODE */
        CECM_DEV_LIST_NFY_T             t_dev_lst_nfy;
        CECM_ACK_COND_T                 e_ack_cond;
        CECM_ONE_TOUCH_PLAY_T           t_one_touch_play;
        CECM_PWR_ON_DEV_STS_T           t_pwr_on_dev_sts;
        CECM_5V_PWR_INFO_T              t_5v_pwr_info;
        /* e_nfy_type = CECM_NFY_TYPE_OPCODE */
        UINT16                          ui2_pa;
        CECM_LOG_ADDR_T                 e_la;
        CECM_ACT_SRC_T                  t_act_src;
        CECM_REC_STS_INFO_T             e_rec_sts_info;
        CECM_DECK_CTRL_MOD_T            e_deck_ctrl_mod;
        CECM_DECK_INFO_T                e_deck_info;
        CECM_PLAY_MOD_T                 e_play_mod;
        CECM_MENU_REQ_STATE_T           e_menu_req_state;
        CECM_USER_CTRL_T                e_usr_ctrl;
        CECM_CEC_VER_T                  e_cec_ver;
        ISO_639_LANG_T                  t_menu_lang;
        CECM_REPORT_PHY_ADDR_T          t_report_phy_addr;
        CECM_DEV_VNDR_ID_T              t_dev_vndr_id;
        UTF16_T                         aw2_osd_name[CECM_OSD_NAME_SIZE];
        CECM_SYS_AUD_STS_T              e_sys_aud_sts;
        CECM_AUD_DESCRIPTOR_T           t_aud_descriptor;
        UINT8                           ui1_aud_sts;
        CECM_TIMER_CLRED_STS_T          e_timer_clred_sts;
        CECM_TIMER_STS_T                t_timer_sts;
        CECM_TUNER_DEV_INFO_T           t_tuner_dev_info;
        CECM_OSD_STRING_INFO_T          t_osd_string_info;
        CECM_ROUTING_CHG_INFO_T         t_routing_chg_info;
        CECM_ABORT_REASON_T             e_abort_reason;
      	CECM_VNDR_CMD_DATA_T            t_vndr_cmd_data;
      	CECM_VNDR_CMD_WITH_ID_DATA_T    t_vndr_cmd_with_id_data;
      	CECM_VNDR_RC_CODE_DATA_T        t_vndr_rc_code_data;
        CECM_PWR_STS_T                  e_pwr_sts;
    }   u_data;
}   CECM_NFY_T;

/* Notify function */
/*------------------------------------------------------------------*/
/*! @brief       The notify function registered to CECM when open the Service, CECM will notice the clients by this function
 *  @param [in]  pt_nfy_data     - Contains the information which should notice to the clients
 *  @param [in]  pv_tag          - Private tag value which set by the clients, will be returned in the notify function, it can be set to NULL.
 *  @return    None
 */
/*------------------------------------------------------------------*/
typedef VOID (*x_cecm_nfy_fct)(CECM_NFY_T* pt_nfy_data,
                               VOID*       pv_tag,
                               UINT32      ui4_data);
/*----------------------------------------------------------------------------*/
/** @} */                                             
/*----------------------------------------------------------------------------*/

#endif /* _U_CECM_H_ */

