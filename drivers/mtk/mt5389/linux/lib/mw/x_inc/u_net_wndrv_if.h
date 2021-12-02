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
/*! @file u_net_wndrv_if.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description: This file define the common definition of wpa suplicant.
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_NET_WNDRV_IF_H
#define _U_NET_WNDRV_IF_H

/*----------------------------------------------------------------------------*/
/*! @addtogroup groupMW_INET_WIFI
 *  @{
 */
/*----------------------------------------------------------------------------*/



//#include "inc/u_common.h"
//#include "u_net_common.h"
#include "u_net_oid.h"
#include "u_net_drv_if.h"
#ifdef __linux__
#include "wpa_os_linux.h"
#else
#include "u_linux_errno.h"
#include "u_linux_wireless.h"
#endif

/*******************************************************************************
*
*  Constant
*
*******************************************************************************/

/*------------------------------------------------------------------*/
/*! @enum SUPC_MSG_ID_T
 *  @brief Message ID
 *  @code
 *  typedef enum
 *  {
 *  	MSG_SUPC_INTERNAL_START,
 *      MSG_SUPC_WNDRV_INIT,                                
 *      MSG_SUPC_INIT,                                      
 *      MSG_SUPC_DEINIT,      
 *      MSG_SUPC_START,                            	
 *      MSG_SUPC_STOP,
 *      MSG_SUPC_RUN_ELOOP,                                 
 *      MSG_SUPC_CLI_COMMAND,                               
 *  	  MSG_SUPC_INTERNAL_END,
 *      MSG_WNDRV_SUPC_INIT_IND,                            
 *      MSG_WNDRV_SUPC_WEXT_EVENT,                          
 *      MSG_WNDRV_SUPC_L2_PACKET,                           
 *      MSG_WNDRV_SUPC_DEINIT_IND,                          
 *  	MSG_WNDRV_SUPC_END
 *  }   SUPC_MSG_ID_T;
 *  @endcode
 *  @li@c  MSG_SUPC_INTERNAL_START                        - supc start
 *  @li@c  MSG_SUPC_WNDRV_INIT                              - supc driver init
 *  @li@c  MSG_SUPC_INIT                                         - supc init
 *  @li@c  MSG_SUPC_DEINIT                                      - supc deinit
 *  @li@c  MSG_SUPC_START                                      - supc start 
 *  @li@c  MSG_SUPC_STOP                                        - supc stop 
 *  @li@c  MSG_SUPC_RUN_ELOOP                                - supc run eloop
 *  @li@c  MSG_SUPC_CLI_COMMAND                            - supc cli
 *  @li@c  MSG_SUPC_INTERNAL_END                            - 
 *  @li@c  MSG_WNDRV_SUPC_INIT_IND                        - supc init indication
 *  @li@c  MSG_WNDRV_SUPC_WEXT_EVENT                  - wext event
 *  @li@c  MSG_WNDRV_SUPC_L2_PACKET                     - l2 packet event
 *  @li@c  MSG_WNDRV_SUPC_DEINIT_IND                     - deinit indication
 *  @li@c  MSG_WNDRV_SUPC_END                               - supc end
 */
/*------------------------------------------------------------------*/
typedef enum
{
    /* messages for supplicant internal process */
	MSG_SUPC_INTERNAL_START,
    MSG_SUPC_WNDRV_INIT,                                /* param : NET_SUPC_IF_T */
    MSG_SUPC_INIT,                                      /* param : NET_SUPC_IF_T */
    MSG_SUPC_DEINIT,                                  	/* param : NET_SUPC_IF_T */
    MSG_SUPC_START,                                     /* param : NET_SUPC_IF_T */
    MSG_SUPC_STOP,                                  	/* param : NET_SUPC_IF_T */
    MSG_SUPC_RUN_ELOOP,                                 /* param : NULL */
    MSG_SUPC_CLI_COMMAND,                               /* param : CLI command string */
	MSG_SUPC_INTERNAL_END,
    /* messages between NI and SUPC */
	
    /* messages between WNDRV and SUPC */
    MSG_WNDRV_SUPC_INIT_IND,                            /* param : INT32 */
    MSG_WNDRV_SUPC_WEXT_EVENT,                          /* param : iw_event */
    MSG_WNDRV_SUPC_L2_PACKET,                           /* param : PKT_BUFF_T */    
    MSG_WNDRV_SUPC_DEINIT_IND,                          /* param : INT32 */
	MSG_WNDRV_SUPC_END
}   SUPC_MSG_ID_T;

/*------------------------------------------------------------------*/
/*! @enum SUPC_DRV_INIT_RESULT_T
 *  @brief Init result
 *  @code
 *  typedef enum
 *  {
 *      MSG_SUPC_DRV_INIT_RES_OK = 0,                       
 *      MSG_SUPC_DRV_INIT_RES_DONGLE_UNPLUG,                
 *      MSG_SUPC_DRV_INIT_RES_FAIL                      
 *  }   SUPC_DRV_INIT_RESULT_T;
 *  @endcode
 *  @li@c  MSG_SUPC_DRV_INIT_RES_OK                                  - init ok
 *  @li@c  MSG_SUPC_DRV_INIT_RES_DONGLE_UNPLUG               - dongle unplug
 *  @li@c  MSG_SUPC_DRV_INIT_RES_FAIL                                - init fail
 */
/*------------------------------------------------------------------*/
typedef enum
{
    /* result code for driver to notify the supplicant */
    MSG_SUPC_DRV_INIT_RES_OK = 0,                       /* OK */
    MSG_SUPC_DRV_INIT_RES_DONGLE_UNPLUG,                /* Wlan dongle not attached */
    MSG_SUPC_DRV_INIT_RES_FAIL                      /* Driver fail */
}   SUPC_DRV_INIT_RESULT_T;

/* Ioctl cmd for setting WPS IE  */
#define IOCTL_SET_WPS_IE    (SIOCIWFIRSTPRIV + 15)    /**< io cmd for set wps ie       */

/* WPS Attribute Types */
/*------------------------------------------------------------------*/
/*! @enum wps_attribute
 *  @brief wps attrib definition
 *  @code
 *  typedef enum wps_attribute 
 *  {
 *  	ATTR_AP_CHANNEL = 0x1001,
 *  	ATTR_ASSOC_STATE = 0x1002,
 *  	ATTR_AUTH_TYPE = 0x1003,
 *  	ATTR_AUTH_TYPE_FLAGS = 0x1004,
 *  	ATTR_AUTHENTICATOR = 0x1005,
 *  	ATTR_CONFIG_METHODS = 0x1008,
 *  	ATTR_CONFIG_ERROR = 0x1009,
 *  	ATTR_CONFIRM_URL4 = 0x100a,
 *  	ATTR_CONFIRM_URL6 = 0x100b,
 *  	ATTR_CONN_TYPE = 0x100c,
 *  	ATTR_CONN_TYPE_FLAGS = 0x100d,
 *  	ATTR_CRED = 0x100e,
 *  	ATTR_ENCR_TYPE = 0x100f,
 *  	ATTR_ENCR_TYPE_FLAGS = 0x1010,
 *  	ATTR_DEV_NAME = 0x1011,
 *  	ATTR_DEV_PASSWORD_ID = 0x1012,
 *  	ATTR_E_HASH1 = 0x1014,
 *  	ATTR_E_HASH2 = 0x1015,
 *  	ATTR_E_SNONCE1 = 0x1016,
 *  	ATTR_E_SNONCE2 = 0x1017,
 *  	ATTR_ENCR_SETTINGS = 0x1018,
 *  	ATTR_ENROLLEE_NONCE = 0x101a,
 *  	ATTR_FEATURE_ID = 0x101b,
 *  	ATTR_IDENTITY = 0x101c,
 *  	ATTR_IDENTITY_PROOF = 0x101d,
 *  	ATTR_KEY_WRAP_AUTH = 0x101e,
 *  	ATTR_KEY_ID = 0x101f,
 *  	ATTR_MAC_ADDR = 0x1020,
 *  	ATTR_MANUFACTURER = 0x1021,
 *  	ATTR_MSG_TYPE = 0x1022,
 *  	ATTR_MODEL_NAME = 0x1023,
 *  	ATTR_MODEL_NUMBER = 0x1024,
 *  	ATTR_NETWORK_INDEX = 0x1026,
 *  	ATTR_NETWORK_KEY = 0x1027,
 *  	ATTR_NETWORK_KEY_INDEX = 0x1028,
 *  	ATTR_NEW_DEVICE_NAME = 0x1029,
 *  	ATTR_NEW_PASSWORD = 0x102a,
 *  	ATTR_OOB_DEVICE_PASSWORD = 0x102c,
 *  	ATTR_OS_VERSION = 0x102d,
 *  	ATTR_POWER_LEVEL = 0x102f,
 *  	ATTR_PSK_CURRENT = 0x1030,
 *  	ATTR_PSK_MAX = 0x1031,
 *  	ATTR_PUBLIC_KEY = 0x1032,
 *  	ATTR_RADIO_ENABLE = 0x1033,
 *  	ATTR_REBOOT = 0x1034,
 *  	ATTR_REGISTRAR_CURRENT = 0x1035,
 *  	ATTR_REGISTRAR_ESTABLISHED = 0x1036,
 *  	ATTR_REGISTRAR_LIST = 0x1037,
 *  	ATTR_REGISTRAR_MAX = 0x1038,
 *  	ATTR_REGISTRAR_NONCE = 0x1039,
 *  	ATTR_REQUEST_TYPE = 0x103a,
 *  	ATTR_RESPONSE_TYPE = 0x103b,
 *  	ATTR_RF_BANDS = 0x103c,
 *  	ATTR_R_HASH1 = 0x103d,
 *  	ATTR_R_HASH2 = 0x103e,
 *  	ATTR_R_SNONCE1 = 0x103f,
 *  	ATTR_R_SNONCE2 = 0x1040,
 *  	ATTR_SELECTED_REGISTRAR = 0x1041,
 *  	ATTR_SERIAL_NUMBER = 0x1042,
 *  	ATTR_WPS_STATE = 0x1044,
 *  	ATTR_SSID = 0x1045,
 *  	ATTR_TOTAL_NETWORKS = 0x1046,
 *  	ATTR_UUID_E = 0x1047,
 *  	ATTR_UUID_R = 0x1048,
 *  	ATTR_VENDOR_EXT = 0x1049,
 *  	ATTR_VERSION = 0x104a,
 *  	ATTR_X509_CERT_REQ = 0x104b,
 *  	ATTR_X509_CERT = 0x104c,
 *  	ATTR_EAP_IDENTITY = 0x104d,
 *  	ATTR_MSG_COUNTER = 0x104e,
 *  	ATTR_PUBKEY_HASH = 0x104f,
 *  	ATTR_REKEY_KEY = 0x1050,
 *  	ATTR_KEY_LIFETIME = 0x1051,
 *  	ATTR_PERMITTED_CFG_METHODS = 0x1052,
 *  	ATTR_SELECTED_REGISTRAR_CONFIG_METHODS = 0x1053,
 *  	ATTR_PRIMARY_DEV_TYPE = 0x1054,
 *  	ATTR_SECONDARY_DEV_TYP_ELIST = 0x1055,
 *  	ATTR_PORTABLE_DEV = 0x1056,
 *  	ATTR_AP_SETUP_LOCKED = 0x1057,
 *  	ATTR_APPLICATION_EXT = 0x1058,
 *  	ATTR_EAP_TYPE = 0x1059,
 *  	ATTR_IV = 0x1060,
 *  	ATTR_KEY_PROVIDED_AUTO = 0x1061,
 *  	ATTR_802_1X_ENABLED = 0x1062,
 *  	ATTR_APPSESSIONKEY = 0x1063,
 *  	ATTR_WEPTRANSMITKEY = 0x1064
 *  };
 *  @endcode
 *  @li@c  ATTR_AP_CHANNEL                                   - ATTR_AP_CHANNEL 
 *  @li@c  ATTR_ASSOC_STATE                                - ATTR_ASSOC_STATE
 *  @li@c  ATTR_AUTH_TYPE                                    - ATTR_AUTH_TYPE 
 *  @li@c  ATTR_AUTH_TYPE_FLAGS                          - ATTR_AUTH_TYPE_FLAGS
 *  @li@c  ATTR_AUTHENTICATOR                             - ATTR_AUTHENTICATOR 
 *  @li@c  ATTR_CONFIG_METHODS                           - ATTR_CONFIG_METHODS
 *  @li@c  ATTR_CONFIG_ERROR                                - ATTR_CONFIG_ERROR 
 *  @li@c  ATTR_CONFIRM_URL4                                - ATTR_CONFIRM_URL4 
 *  @li@c  ATTR_CONFIRM_URL6                                - ATTR_CONFIRM_URL6 
 *  @li@c  ATTR_CONN_TYPE                                    - ATTR_CONN_TYPE 
 *  @li@c  ATTR_CONN_TYPE_FLAGS                          - ATTR_CONN_TYPE_FLAGS
 *  @li@c  ATTR_CRED                                             - ATTR_CRED 
 *  @li@c  ATTR_ENCR_TYPE                                    - ATTR_ENCR_TYPE 
 *  @li@c  ATTR_ENCR_TYPE_FLAGS                          - ATTR_ENCR_TYPE_FLAGS 
 *  @li@c  ATTR_DEV_NAME                                     - ATTR_DEV_NAME 
 *  @li@c  ATTR_DEV_PASSWORD_ID                         - ATTR_DEV_PASSWORD_ID 
 *  @li@c  ATTR_E_HASH1                                       - ATTR_E_HASH1 
 *  @li@c  ATTR_E_HASH2                                       - ATTR_E_HASH2 
 *  @li@c  ATTR_E_SNONCE1                                   - ATTR_E_SNONCE1 
 *  @li@c  ATTR_E_SNONCE2                                   - ATTR_E_SNONCE2 
 *  @li@c  ATTR_ENCR_SETTINGS                             - ATTR_ENCR_SETTINGS 
 *  @li@c  ATTR_ENROLLEE_NONCE                           - ATTR_ENROLLEE_NONCE 
 *  @li@c  ATTR_FEATURE_ID                                  - ATTR_FEATURE_ID 
 *  @li@c  ATTR_IDENTITY                                      - ATTR_IDENTITY 
 *  @li@c  ATTR_IDENTITY_PROOF                            - ATTR_IDENTITY_PROOF 
 *  @li@c  ATTR_KEY_WRAP_AUTH                            - ATTR_KEY_WRAP_AUTH 
 *  @li@c  ATTR_KEY_ID                                          - ATTR_KEY_ID 
 *  @li@c  ATTR_MAC_ADDR                                     - ATTR_MAC_ADDR 
 *  @li@c  ATTR_MANUFACTURER                              - ATTR_MANUFACTURER 
 *  @li@c  ATTR_MSG_TYPE                                     - ATTR_MSG_TYPE 
 *  @li@c  ATTR_MODEL_NAME                                 - ATTR_MODEL_NAME 
 *  @li@c  ATTR_MODEL_NUMBER                              - ATTR_MODEL_NUMBER 
 *  @li@c  ATTR_NETWORK_INDEX                             - ATTR_NETWORK_INDEX 
 *  @li@c  ATTR_NETWORK_KEY                                - ATTR_NETWORK_KEY 
 *  @li@c  ATTR_NETWORK_KEY_INDEX                      - ATTR_NETWORK_KEY_INDEX 
 *  @li@c  ATTR_NEW_DEVICE_NAME                         - ATTR_NEW_DEVICE_NAME 
 *  @li@c  ATTR_NEW_PASSWORD                             - ATTR_NEW_PASSWORD 
 *  @li@c  ATTR_OOB_DEVICE_PASSWORD                  - ATTR_OOB_DEVICE_PASSWORD 
 *  @li@c  ATTR_OS_VERSION                                  - ATTR_OS_VERSION 
 *  @li@c  ATTR_POWER_LEVEL                                - ATTR_POWER_LEVEL 
 *  @li@c  ATTR_PSK_CURRENT                                - ATTR_PSK_CURRENT 
 *  @li@c  ATTR_PSK_MAX                                      - ATTR_PSK_MAX 
 *  @li@c  ATTR_PUBLIC_KEY                                   - ATTR_PUBLIC_KEY 
 *  @li@c  ATTR_RADIO_ENABLE                               - ATTR_RADIO_ENABLE 
 *  @li@c  ATTR_REBOOT                                        - ATTR_REBOOT 
 *  @li@c  ATTR_REGISTRAR_CURRENT                      - ATTR_REGISTRAR_CURRENT 
 *  @li@c  ATTR_REGISTRAR_ESTABLISHED                - ATTR_REGISTRAR_ESTABLISHED 
 *  @li@c  ATTR_REGISTRAR_LIST                            - ATTR_REGISTRAR_LIST 
 *  @li@c  ATTR_REGISTRAR_MAX                            - ATTR_REGISTRAR_MAX 
 *  @li@c  ATTR_REGISTRAR_NONCE                         - ATTR_REGISTRAR_NONCE 
 *  @li@c  ATTR_REQUEST_TYPE                              - ATTR_REQUEST_TYPE 
 *  @li@c  ATTR_RESPONSE_TYPE                            - ATTR_RESPONSE_TYPE 
 *  @li@c  ATTR_RF_BANDS                                    - ATTR_RF_BANDS 
 *  @li@c  ATTR_R_HASH1                                      - ATTR_R_HASH1 
 *  @li@c  ATTR_R_HASH2                                      - ATTR_R_HASH2 
 *  @li@c  ATTR_R_SNONCE1                                  - ATTR_R_SNONCE1 
 *  @li@c  ATTR_R_SNONCE2                                  - ATTR_R_SNONCE2 
 *  @li@c  ATTR_SELECTED_REGISTRAR                    - ATTR_SELECTED_REGISTRAR 
 *  @li@c  ATTR_SERIAL_NUMBER                            - ATTR_SERIAL_NUMBER 
 *  @li@c  ATTR_WPS_STATE                                 - ATTR_WPS_STATE 
 *  @li@c  ATTR_SSID                                           - ATTR_SSID 
 *  @li@c  ATTR_TOTAL_NETWORKS                        - ATTR_TOTAL_NETWORKS 
 *  @li@c  ATTR_UUID_E                                        - ATTR_UUID_E 
 *  @li@c  ATTR_UUID_R                                        - ATTR_UUID_R 
 *  @li@c  ATTR_VENDOR_EXT                                - ATTR_VENDOR_EXT 
 *  @li@c  ATTR_VERSION                                      - ATTR_VERSION 
 *  @li@c  ATTR_X509_CERT_REQ                            - ATTR_X509_CERT_REQ 
 *  @li@c  ATTR_X509_CERT                                   - ATTR_X509_CERT 
 *  @li@c  ATTR_EAP_IDENTITY                               - ATTR_EAP_IDENTITY 
 *  @li@c  ATTR_MSG_COUNTER                              - ATTR_MSG_COUNTER 
 *  @li@c  ATTR_PUBKEY_HASH                               - ATTR_PUBKEY_HASH 
 *  @li@c  ATTR_REKEY_KEY                                   - ATTR_REKEY_KEY 
 *  @li@c  ATTR_KEY_LIFETIME                               - ATTR_KEY_LIFETIME 
 *  @li@c  ATTR_PERMITTED_CFG_METHODS             - ATTR_PERMITTED_CFG_METHODS 
 *  @li@c  ATTR_SELECTED_REGISTRAR_CONFIG_METHODS           - ATTR_SELECTED_REGISTRAR_CONFIG_METHODS
 *  @li@c  ATTR_PRIMARY_DEV_TYPE                             - ATTR_PRIMARY_DEV_TYPE 
 *  @li@c  ATTR_SECONDARY_DEV_TYP_ELIST                 - ATTR_SECONDARY_DEV_TYP_ELIST 
 *  @li@c  ATTR_PORTABLE_DEV                                   - ATTR_PORTABLE_DEV 
 *  @li@c  ATTR_AP_SETUP_LOCKED                              - ATTR_AP_SETUP_LOCKED 
 *  @li@c  ATTR_APPLICATION_EXT                               - ATTR_APPLICATION_EXT 
 *  @li@c  ATTR_EAP_TYPE                                          - ATTR_EAP_TYPE 
 *  @li@c  ATTR_IV                                                     - ATTR_IV 
 *  @li@c  ATTR_KEY_PROVIDED_AUTO                           - ATTR_KEY_PROVIDED_AUTO 
 *  @li@c  ATTR_802_1X_ENABLED                                 - ATTR_802_1X_ENABLED 
 *  @li@c  ATTR_APPSESSIONKEY                                  - ATTR_APPSESSIONKEY 
 *  @li@c  ATTR_WEPTRANSMITKEY                               - ATTR_WEPTRANSMITKEY 
 */
/*------------------------------------------------------------------*/
enum wps_attribute 
{
	ATTR_AP_CHANNEL = 0x1001,
	ATTR_ASSOC_STATE = 0x1002,
	ATTR_AUTH_TYPE = 0x1003,
	ATTR_AUTH_TYPE_FLAGS = 0x1004,
	ATTR_AUTHENTICATOR = 0x1005,
	ATTR_CONFIG_METHODS = 0x1008,
	ATTR_CONFIG_ERROR = 0x1009,
	ATTR_CONFIRM_URL4 = 0x100a,
	ATTR_CONFIRM_URL6 = 0x100b,
	ATTR_CONN_TYPE = 0x100c,
	ATTR_CONN_TYPE_FLAGS = 0x100d,
	ATTR_CRED = 0x100e,
	ATTR_ENCR_TYPE = 0x100f,
	ATTR_ENCR_TYPE_FLAGS = 0x1010,
	ATTR_DEV_NAME = 0x1011,
	ATTR_DEV_PASSWORD_ID = 0x1012,
	ATTR_E_HASH1 = 0x1014,
	ATTR_E_HASH2 = 0x1015,
	ATTR_E_SNONCE1 = 0x1016,
	ATTR_E_SNONCE2 = 0x1017,
	ATTR_ENCR_SETTINGS = 0x1018,
	ATTR_ENROLLEE_NONCE = 0x101a,
	ATTR_FEATURE_ID = 0x101b,
	ATTR_IDENTITY = 0x101c,
	ATTR_IDENTITY_PROOF = 0x101d,
	ATTR_KEY_WRAP_AUTH = 0x101e,
	ATTR_KEY_ID = 0x101f,
	ATTR_MAC_ADDR = 0x1020,
	ATTR_MANUFACTURER = 0x1021,
	ATTR_MSG_TYPE = 0x1022,
	ATTR_MODEL_NAME = 0x1023,
	ATTR_MODEL_NUMBER = 0x1024,
	ATTR_NETWORK_INDEX = 0x1026,
	ATTR_NETWORK_KEY = 0x1027,
	ATTR_NETWORK_KEY_INDEX = 0x1028,
	ATTR_NEW_DEVICE_NAME = 0x1029,
	ATTR_NEW_PASSWORD = 0x102a,
	ATTR_OOB_DEVICE_PASSWORD = 0x102c,
	ATTR_OS_VERSION = 0x102d,
	ATTR_POWER_LEVEL = 0x102f,
	ATTR_PSK_CURRENT = 0x1030,
	ATTR_PSK_MAX = 0x1031,
	ATTR_PUBLIC_KEY = 0x1032,
	ATTR_RADIO_ENABLE = 0x1033,
	ATTR_REBOOT = 0x1034,
	ATTR_REGISTRAR_CURRENT = 0x1035,
	ATTR_REGISTRAR_ESTABLISHED = 0x1036,
	ATTR_REGISTRAR_LIST = 0x1037,
	ATTR_REGISTRAR_MAX = 0x1038,
	ATTR_REGISTRAR_NONCE = 0x1039,
	ATTR_REQUEST_TYPE = 0x103a,
	ATTR_RESPONSE_TYPE = 0x103b,
	ATTR_RF_BANDS = 0x103c,
	ATTR_R_HASH1 = 0x103d,
	ATTR_R_HASH2 = 0x103e,
	ATTR_R_SNONCE1 = 0x103f,
	ATTR_R_SNONCE2 = 0x1040,
	ATTR_SELECTED_REGISTRAR = 0x1041,
	ATTR_SERIAL_NUMBER = 0x1042,
	ATTR_WPS_STATE = 0x1044,
	ATTR_SSID = 0x1045,
	ATTR_TOTAL_NETWORKS = 0x1046,
	ATTR_UUID_E = 0x1047,
	ATTR_UUID_R = 0x1048,
	ATTR_VENDOR_EXT = 0x1049,
	ATTR_VERSION = 0x104a,
	ATTR_X509_CERT_REQ = 0x104b,
	ATTR_X509_CERT = 0x104c,
	ATTR_EAP_IDENTITY = 0x104d,
	ATTR_MSG_COUNTER = 0x104e,
	ATTR_PUBKEY_HASH = 0x104f,
	ATTR_REKEY_KEY = 0x1050,
	ATTR_KEY_LIFETIME = 0x1051,
	ATTR_PERMITTED_CFG_METHODS = 0x1052,
	ATTR_SELECTED_REGISTRAR_CONFIG_METHODS = 0x1053,
	ATTR_PRIMARY_DEV_TYPE = 0x1054,
	ATTR_SECONDARY_DEV_TYP_ELIST = 0x1055,
	ATTR_PORTABLE_DEV = 0x1056,
	ATTR_AP_SETUP_LOCKED = 0x1057,
	ATTR_APPLICATION_EXT = 0x1058,
	ATTR_EAP_TYPE = 0x1059,
	ATTR_IV = 0x1060,
	ATTR_KEY_PROVIDED_AUTO = 0x1061,
	ATTR_802_1X_ENABLED = 0x1062,
	ATTR_APPSESSIONKEY = 0x1063,
	ATTR_WEPTRANSMITKEY = 0x1064
};

/*******************************************************************************
*
*  Macro
*
*******************************************************************************/
#define ptr_len_advance(p,l,v) do{p+=v; l+=v;} while(0)    /**< get ptr and len advance v       */

/*******************************************************************************
*
*  Data structure
*
*******************************************************************************/
/* for WPS data */
/*------------------------------------------------------------------*/
/*! @struct WPS_IE_ATTR_T
 *  @brief struct of WPS IE 
 *  @code
 *  typedef struct _WPS_IE_ATTR_S {
 *      UINT8                   version;
 *      UINT8                   req_type;
 *      UINT8                   rf_bands;
 *      UINT8                   rsvd; 
 *      UINT16                 assoc_state;
 *      UINT16                 config_error;
 *      UINT16                 dev_password_id;
 *      UINT16                 config_methods;
 *      UINT8                   primary_dev_type[8];
 *      UINT8                   uuid_e[16];
 *  } WPS_IE_ATTR_T;
 *  @endcode
 *  @li@c  version                                           - version               
 *  @li@c  req_type                                         - req_type              
 *  @li@c  rf_bands                                         - rf_bands              
 *  @li@c  rsvd                                               - rsvd                  
 *  @li@c  assoc_state                                    - assoc_state           
 *  @li@c  config_error                                     - config_error          
 *  @li@c  dev_password_id                              - dev_password_id       
 *  @li@c  config_methods                                - config_methods        
 *  @li@c  primary_dev_type[8]                         - primary_dev_type[8]   
 *  @li@c  uuid_e[16]                                       - uuid_e[16]            
 */
/*------------------------------------------------------------------*/
typedef struct _WPS_IE_ATTR_S {
    UINT8                   version;
    UINT8                   req_type;
    UINT8                   rf_bands;
    UINT8                   rsvd; /* reserved for alignment purpose */
    UINT16                  assoc_state;
    UINT16                  config_error;
    UINT16                  dev_password_id;
    UINT16                  config_methods;
    UINT8                   primary_dev_type[8];
    UINT8                   uuid_e[16];
} WPS_IE_ATTR_T;

/*******************************************************************************
*
*  Function Prototype
*
*******************************************************************************/
/**
 * Forward declare
 */
#ifndef __linux__
typedef struct _SUPC_WNDRV_IF_S SUPC_WNDRV_IF_T;

/*------------------------------------------------------------------*/
/*! @brief Initialize wlan driver(wndrv) module
 *            This function is called to initialize wndrv module.
 *            It's the entry function for supc to enable wlan device driver.
 *  @param [in] ptSupcWndrvIf -Interface structure pointer of supc and wndrv
 *  @return     result
 *  @retval     0 -success
 *  @retval     others -failurel
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wndrv_init_fnct)        (SUPC_WNDRV_IF_T *ptSupcWndrvIf);

/*------------------------------------------------------------------*/
/*! @brief De-initialize wlan driver(wndrv) module
 *            This function is called to release driver, after this callback
 *            is called all driver allocate resource must be released.
 *  @param [in] ptSupcWndrvIf -Interface structure pointer of supc and wndrv
 *  @return     result
 *  @retval     0 -success
 *  @retval     -1 -failurel
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wndrv_deinit_fnct)      (SUPC_WNDRV_IF_T *ptSupcWndrvIf);

/*------------------------------------------------------------------*/
/*! @brief wlan driver start function ptr
 *  @param [in] ptSupcWndrvIf -Interface structure pointer of supc and wndrv
 *  @return     result
 *  @retval     0 -success
 *  @retval     others -failurel
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wndrv_start_fnct)       (SUPC_WNDRV_IF_T *ptSupcWndrvIf);

/*------------------------------------------------------------------*/
/*! @brief wlan driver stop function ptr
 *  @param [in] ptSupcWndrvIf -Interface structure pointer of supc and wndrv
 *  @return     result
 *  @retval     0 -success
 *  @retval     others -failurel
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wndrv_stop_fnct)        (SUPC_WNDRV_IF_T *ptSupcWndrvIf);
#else

/*------------------------------------------------------------------*/
/*! @brief wlan driver init function ptr
 *            This function is called to initialize wndrv module.
 *            It's the entry function for supc to enable wlan device driver.
 *  @param [in] ptSupcWndrvIf -Interface structure pointer of supc and wndrv
 *  @return     result
 *  @retval     0 -success
 *  @retval     others -failurel
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wndrv_init_fnct)        (void *ptSupcWndrvIf);

/*------------------------------------------------------------------*/
/*! @brief De-initialize wlan driver(wndrv) module
 *            This function is called to release driver, after this callback
 *            is called all driver allocate resource must be released.
 *  @param [in] ptSupcWndrvIf -Interface structure pointer of supc and wndrv
 *  @return     result
 *  @retval     0 -success
 *  @retval     -1 -failurel
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wndrv_deinit_fnct)      (void *ptSupcWndrvIf);

/*------------------------------------------------------------------*/
/*! @brief  wlan driver start function ptr.
 *  @param [in] ptSupcWndrvIf -Interface structure pointer of supc and wndrv
 *  @return     result
 *  @retval     0 -success
 *  @retval     others -failurel
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wndrv_start_fnct)       (void *ptSupcWndrvIf);
/*------------------------------------------------------------------*/
/*! @brief wlan driver stop function ptr
 *  @param [in] ptSupcWndrvIf -Interface structure pointer of supc and wndrv
 *  @return     result
 *  @retval     0 -success
 *  @retval     others -failurel
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wndrv_stop_fnct)        (void *ptSupcWndrvIf);

#endif

/*------------------------------------------------------------------*/
/*! @brief Ioctl function.
 *            This function is called to process all ioctl commands 
 *            defined in wireless.h of Linux
 *  @param [in] i4Cmd -Command ID.
 *  @param [in] ptIwr  -The structure to exchange data for ioctl.
 *  @return     result
 *  @retval     0 -success
 *  @retval     -1 -failurel
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wndrv_do_ioctl)         (INT32 i4Cmd, VOID /*struct iwreq*/*ptIwr);

/*------------------------------------------------------------------*/
/*! @brief wlan driver get mac addr function ptr
 *            This function can be used to get the own MAC address of the 
 *            wlan device.
 *  @param [in,out] ptOwnMac -Pointer to own MAC address or %NULL on failure
 *  @return     result
 *  @retval     0 -success
 *  @retval     others -failurel
 */
/*------------------------------------------------------------------*/
typedef INT32 (*wndrv_get_mac_addr)     (UINT8 *ptOwnMac);

/*------------------------------------------------------------------*/
/*! @brief wlan driver send event function ptr
 *            This function is used for wndrv to send message events to supplicant, 
 *            include wireless extension and l2 packet events and so on. 
 *            The message ID is defined in enumeration SUPC_MSG_ID_T.
 *  @param [in] supc_wndrv_if -Pointer to device operation data structure
 *  @param [in] i4Cmd -Command ID
  *  @param [in] pvParam -event parameter.
 *  @return     result
 *  @retval     0 -success
 *  @retval     -1 -failurel
 */
/*------------------------------------------------------------------*/
typedef INT32 (*supc_msg_event_fnct)    (void *supc_wndrv_if, 
												INT32 i4Cmd, 
												VOID *pvParam);

/*******************************************************************************
*
*  Interface structure
*
*******************************************************************************/
/* Interface between Supplicant and wlan driver */
/*------------------------------------------------------------------*/
/*! @struct SUPC_WNDRV_IF_T
 *  @brief supc and wndrv interface information
 *  @code
 *  typedef struct _SUPC_WNDRV_IF_S
 *  {
 *      wndrv_init_fnct                pf_wndrv_init;
 *      wndrv_deinit_fnct      	 pf_wndrv_deinit;
 *      wndrv_start_fnct              pf_wndrv_start;
 *      wndrv_stop_fnct         	 pf_wndrv_stop;
 *      wndrv_do_ioctl                 pf_do_ioctl;
 *      wndrv_get_mac_addr         pf_get_mac_addr;
 *      supc_msg_event_fnct        pf_msg_event_nfy;
 *      //supc_wext_event_fnct    pf_wext_event_nfy;
 *      //supc_l2_packet_fnct       pf_l2_packet_nfy;
 *      NET_DRV_IF_T            *    pt_ni_wndrv_if;
 *      VOID                    *         pv_priv;
 *  } SUPC_WNDRV_IF_T;
 *  @endcode
 *  @li@c  pf_wndrv_init                                     - init func ptr
 *  @li@c  pf_wndrv_deinit                                  - deinit func ptr
 *  @li@c  pf_wndrv_start                                   - start func prt 
 *  @li@c  pf_wndrv_stop                                    - stop func ptr
 *  @li@c  pf_do_ioctl                                         - io_ctl func ptr
 *  @li@c  pf_get_mac_addr                                 - pf_get_mac_addr func ptr
 *  @li@c  pf_msg_event_nfy                                - pf_msg_event_nfy func ptr
 *  @li@c  pf_wext_event_nfy                               - pf_wext_event_nfy func ptr
 *  @li@c  pf_l2_packet_nfy                                  - pf_l2_packet_nfy func ptr
 *  @li@c  *pt_ni_wndrv_if                                    - pt_ni_wndrv_if sturcture ptr
 *  @li@c  *pv_priv                                              - private sturcture ptr
 */
/*------------------------------------------------------------------*/
typedef struct _SUPC_WNDRV_IF_S
{
    /* supc callback functions, wndrv needs to register it */
    wndrv_init_fnct         pf_wndrv_init;
    wndrv_deinit_fnct      	pf_wndrv_deinit;
    wndrv_start_fnct        pf_wndrv_start;
    wndrv_stop_fnct      	pf_wndrv_stop;
    wndrv_do_ioctl          pf_do_ioctl;
    wndrv_get_mac_addr      pf_get_mac_addr;

    /* wndrv callback functions, supc needs to register it */
    supc_msg_event_fnct     pf_msg_event_nfy;
    //supc_wext_event_fnct    pf_wext_event_nfy;
    //supc_l2_packet_fnct     pf_l2_packet_nfy;

    /* pointer to interface of ni and wndrv*/
    NET_DRV_IF_T            *pt_ni_wndrv_if;

    /* pointer to private data */
    VOID                    *pv_priv;
} SUPC_WNDRV_IF_T;
/*******************************************************************************
*
* Function Declarations
*
*******************************************************************************/
/* Temporal extern wndrv init API */
/*------------------------------------------------------------------*/
/*! @brief wlan driver init function
 *  @param [in] supc_wndrv_if -Interface structure pointer of supc and wndrv.
 *  @return     result
 *  @retval     0 -success
 *  @retval     others -failurel
 */
/*------------------------------------------------------------------*/
extern INT32 i4Wlan_Drv_Init(SUPC_WNDRV_IF_T *supc_wndrv_if);

/*----------------------------------------------------------------------------*/
/*! @} */
/*----------------------------------------------------------------------------*/
 
#endif /* _U_NET_WNDRV_IF_H */
