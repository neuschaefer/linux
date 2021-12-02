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
/*! @file u_net_wlan.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains wifi definitions, which are exported
 */
/*----------------------------------------------------------------------------*/
#ifndef _U_NET_WLAN_H
#define _U_NET_WLAN_H

/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_INET_WIFI WIFI
 *  @ingroup groupMW_INET
 *  @brief The Svl builder module is the super module of other svl builder engine
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could create scan engine
 *   or start scan from the exported API of svl builder.
 *   
 *  @see groupMW_INET
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define WLAN_SUB_VERSION     0x00         /**< Wlan FW version.*/

#define NET_WLAN_OK                 0     /**< Success. */
#define NET_WLAN_ERROR              -1    /**< Undefined error.*/
#define NET_WLAN_INVALID_PARAM      -2    /**< The parameter was incompatible to this software or invalid.*/
#define NET_WLAN_DEV_NOT_READY      -3    /**< Wlan driver/device is not ready.       */
#define NET_WLAN_NOT_INIT           -4    /**< Wlan module is not init.        */
#define NET_WLAN_ALREADY_INIT       -6    /**< Wlan module is already init.       */
#define NET_WLAN_NOT_STARTED        -7    /**< Wlan module is not started.       */
#define NET_WLAN_ALREADY_STARTED    -8    /**< Wlan module is already started.       */
#define NET_WLAN_DRV_INTERFACE_ERR  -9    /**< Wlan module function pointer is incorrect.       */


#define NET_WLAN_WPS_FAILED       (-1)    /**< Process is terminated because some error occurred or WPS process was cancelled by a user. */
#define NET_WLAN_WPS_IDLE          (0)    /**< Idle.    */
#define NET_WLAN_WPS_SEARCHING     (1)    /**< Searching a registrar.   */
#define NET_WLAN_WPS_REGISTERING   (2)    /**< Registering.  */
#define NET_WLAN_WPS_REGISTERED    (3)    /**< Registered.   */


#define NET_802_11_MAX_LEN_SSID     32    /**< maximum SSID length.       */

#define NET_WPA_DATA_LENGTH         1024  /**< NET_802_11_WPA_T DATA length.       */
#define NET_NFY_DATA_LENGTH         256   /**< NET_802_11_NFY_T DATA length.       */

#define ACFG_MAX_ANTENNA       3


typedef UINT8 NET_802_11_BSSID_T [6]; /**< Type of BSSID data array.       */

/*------------------------------------------------------------------*/
/*! @brief Type of wlan callback fuction that notifies upper layer.
 *  @param [in]  i4CallbackId - callback ID
 *  @param [in]  pParam - parameter
 *  @return   results
 *  @retval   WLAN_NFY_MSG_OK   -  successful
 *  @retval   Others         -  failed             
 */
/*------------------------------------------------------------------*/
typedef INT32 (*x_net_wlan_notify_fct)(INT32 i4CallbackId,
                                            VOID *pParam);

/*--------------------------------------------------------------*/
/*                                                              */
/* Enumeration                                                  */
/*                                                              */
/*--------------------------------------------------------------*/

/*------------------------------------------------------------------*/
/*! @enum WLAN_NOTIFY_ID_T
 *  @brief NotifyID Defines for notification types 
 *  @code
 *  typedef enum
 *  {
 *      WLAN_NOTIFY_START_IND,
 *      WLAN_NOTIFY_STOP_IND,
 *      WLAN_NOTIFY_INIT_IND,               
 *      WLAN_NOTIFY_DEINIT_IND,             
 *      WLAN_NOTIFY_SCAN,                   
 *      WLAN_NOTIFY_ASSOCIATE,              
 *      WLAN_NOTIFY_WPS_ASSOC_SETTING,      
 *      WLAN_NOTIFY_WPS_STATE,              
 *      WLAN_NOTIFY_WPS_ASSOCIATE           
 *  }   WLAN_NOTIFY_ID_T;
 *  @endcode
 *  @li@c  WLAN_NOTIFY_START_IND                            - WLAN module start \ref NotifyMsg "indication
 *  @li@c  WLAN_NOTIFY_STOP_IND                              - WLAN module stopinit \ref NotifyMsg "indication 
 *  @li@c  WLAN_NOTIFY_INIT_IND                               - WLAN module init \ref NotifyMsg "indication
 *  @li@c  WLAN_NOTIFY_DEINIT_IND                            - WLAN module deinit \ref NotifyMsg "indication"               
 *  @li@c  WLAN_NOTIFY_SCAN                                    - Scan result notification \ref NotifyMsg "message"           
 *  @li@c  WLAN_NOTIFY_ASSOCIATE                            - Association result notification \ref NotifyMsg "message"     
 *  @li@c  WLAN_NOTIFY_WPS_ASSOC_SETTING             - WPS association notification \ref NotifyMsg "message"       
 *  @li@c  WLAN_NOTIFY_WPS_STATE                           - WPS processing \ref WPSStatus "state"                        
 *  @li@c  WLAN_NOTIFY_WPS_ASSOCIATE                    - WPS Association result notification \ref NotifyMsg "message" 
 */
/*------------------------------------------------------------------*/
typedef enum
{
    WLAN_NOTIFY_START_IND,
    WLAN_NOTIFY_STOP_IND,
    WLAN_NOTIFY_INIT_IND,               
    WLAN_NOTIFY_DEINIT_IND,             
    WLAN_NOTIFY_SCAN,                   
    WLAN_NOTIFY_ASSOCIATE,              
    WLAN_NOTIFY_WPS_ASSOC_SETTING,      
    WLAN_NOTIFY_WPS_STATE,              
    WLAN_NOTIFY_WPS_ASSOCIATE ,
    WLAN_NOTIFY_P2P_DEVICE_FOUND ,
    WLAN_NOTIFY_DRIVER_START_IND ,
    WLAN_NOTIFY_DRIVER_START_FAILED
}   WLAN_NOTIFY_ID_T;


/*------------------------------------------------------------------*/
/*! @brief NotifyMsg Defines for notification messages
 *  @code
 *  typedef enum
 *  {
 *      WLAN_NFY_MSG_OK                     = 0,                
 *      WLAN_NFY_MSG_ALREADY_ASSOCIATED,
 *      WLAN_NFY_MSG_INIT_FAIL,                     
 *      WLAN_NFY_MSG_DEINIT_FAIL,                   
 *      WLAN_NFY_MSG_DONGLE_UNPLUG,                 
 *      WLAN_NFY_MSG_ASSOC_FAIL             = 10,             
 *      WLAN_NFY_MSG_ASSOC_TIMEOUT,                                          
 *      WLAN_NFY_MSG_AUTH_TIMEOUT,                     
 *      WLAN_NFY_MSG_PSK_INCORRECT,                    
 *      WLAN_NFY_MSG_NO_MATCHED_CONFIG,             
 *      WLAN_NFY_MSG_NO_MATCHED_SSID,               
 *      WLAN_NFY_MSG_WPS_CANCELLED,                 
 *      WLAN_NFY_MSG_WPS_PIN_MISMATCH,              
 *      WLAN_NFY_MSG_WPS_SCAN_TIMEOUT,              
 *      WLAN_NFY_MSG_WPS_PBC_OVERLAP,               
 *      WLAN_NFY_MSG_WPS_PROTOCOL_TIMEOUT,          
 *      WLAN_NFY_MSG_WPS_CREDENTIAL_INVALID         
 *  }   WLAN_NOTIFY_MSG_T;
 *  @endcode
 *  @li@c  WLAN_NFY_MSG_OK                                             - Success.
 *  @li@c  WLAN_NFY_MSG_ALREADY_ASSOCIATED                  - WLAN already associated.
 *  @li@c  WLAN_NFY_MSG_INIT_FAIL                                    - WLAN driver init fail. 
 *  @li@c  WLAN_NFY_MSG_DEINIT_FAIL                                 - WLAN driver deinit fail. 
 *  @li@c  WLAN_NFY_MSG_DONGLE_UNPLUG                           - WLAN dongle is unplugged.
 *  @li@c  WLAN_NFY_MSG_ASSOC_FAIL                                 - Undefined association fail.                    
 *  @li@c  WLAN_NFY_MSG_ASSOC_TIMEOUT                           - Association timeout.                           
 *  @li@c  WLAN_NFY_MSG_AUTH_TIMEOUT                             - WPA association fail, authentication timeout.  
 *  @li@c  WLAN_NFY_MSG_PSK_INCORRECT                             - WPA association fail, incorrect Pre-share key. 
 *  @li@c  WLAN_NFY_MSG_NO_MATCHED_CONFIG                     - WPA association fail, no matched config is found.
 *  @li@c  WLAN_NFY_MSG_NO_MATCHED_SSID                         - WPA association fail, no matched SSID is found. 
 *  @li@c  WLAN_NFY_MSG_WPS_CANCELLED                            - WPS Registration was cancelled by a user. 
 *  @li@c  WLAN_NFY_MSG_WPS_PIN_MISMATCH                       - WPS Association fail, PIN was mismatched to the registrar's PIN. 
 *  @li@c  WLAN_NFY_MSG_WPS_SCAN_TIMEOUT                      - WPS Association fail, failed to detect a registrar in PIN/PBC mode.
 *  @li@c  WLAN_NFY_MSG_WPS_PBC_OVERLAP                         - WPS Association fail, due to PBC overlap. 
 *  @li@c  WLAN_NFY_MSG_WPS_PROTOCOL_TIMEOUT               - WPS Association fail, protocol failed to complete after the credential(PIN/PBC) was authorized. 
 *  @li@c  WLAN_NFY_MSG_WPS_CREDENTIAL_INVALID               - WPS Association fail, credential received from the registrar is incompatible or invalid. 
 */
/*------------------------------------------------------------------*/
typedef enum
{
    /* normal message */
    WLAN_NFY_MSG_OK                     = 0,                
    WLAN_NFY_MSG_ALREADY_ASSOCIATED,
    
    /* wndrv abnormal message */
    WLAN_NFY_MSG_INIT_FAIL,                     
    WLAN_NFY_MSG_DEINIT_FAIL,                   
    WLAN_NFY_MSG_DONGLE_UNPLUG,                 
    /* wpa abnormal message */
    WLAN_NFY_MSG_ASSOC_FAIL             = 10,             
    WLAN_NFY_MSG_ASSOC_TIMEOUT,                                          
    WLAN_NFY_MSG_AUTH_TIMEOUT,                     
    WLAN_NFY_MSG_PSK_INCORRECT,                    
    WLAN_NFY_MSG_NO_MATCHED_CONFIG,             
    WLAN_NFY_MSG_NO_MATCHED_SSID,               
    /* wps abnormal message */
    WLAN_NFY_MSG_WPS_CANCELLED,                 
    WLAN_NFY_MSG_WPS_PIN_MISMATCH,              
    WLAN_NFY_MSG_WPS_SCAN_TIMEOUT,              
    WLAN_NFY_MSG_WPS_PBC_OVERLAP,               
    WLAN_NFY_MSG_WPS_PROTOCOL_TIMEOUT,          
    WLAN_NFY_MSG_WPS_CREDENTIAL_INVALID ,
    WLAN_NFY_MSG_DISCONNECTED,
    WLAN_NFY_MSG_WPS_FAIL
}   WLAN_NOTIFY_MSG_T;


/*------------------------------------------------------------------*/
/*! @enum WLAN_WPS_STATE_T
 *  @brief WPSStatus Defines for WPS status codes
 *  @code
 *  typedef enum
 *  {
 *      WLAN_MW_WPS_IDLE            = 0,   
 *      WLAN_MW_WPS_FAIL_RE_DO,            
 *      WLAN_MW_WPS_DISCOVER,              
 *      WLAN_MW_WPS_ASSOCIATING,           
 *      WLAN_MW_WPS_WAIT_REQ_ID,           
 *      WLAN_MW_WPS_WAIT_WSC_START,        
 *      WLAN_MW_WPS_WAIT_WSC_M2,           
 *      WLAN_MW_WPS_WAIT_WSC_M4,           
 *      WLAN_MW_WPS_WAIT_WSC_M6,           
 *      WLAN_MW_WPS_WAIT_WSC_M8,           
 *      WLAN_MW_WPS_WAIT_EAP_FAIL,            
 *      WLAN_MW_WPS_WAIT_WPA_SUPC,         
 *      WLAN_MW_WPS_CONNECTED,             
 *      WLAN_MW_WPS_FAIL,                  
 *      WLAN_MW_WPS_PERIOD_START,          
 *      WLAN_MW_WPS_STATE_MAX              
 *  }   WLAN_WPS_STATE_T;
 *  @endcode
 *  @li@c  WLAN_MW_WPS_IDLE                                      - WPS Idle state. 
 *  @li@c  WLAN_MW_WPS_FAIL_RE_DO                            - WPS Fail and Redo state. 
 *  @li@c  WLAN_MW_WPS_DISCOVER                               - WPS Discover AP state.
 *  @li@c  WLAN_MW_WPS_ASSOCIATING                          - WPS Associating AP state.
 *  @li@c  WLAN_MW_WPS_WAIT_REQ_ID                          - WPS Wait Req ID state. 
 *  @li@c  WLAN_MW_WPS_WAIT_WSC_START                   - WPS Wait Start state. 
 *  @li@c  WLAN_MW_WPS_WAIT_WSC_M2                        - WPS Wait M2 state. 
 *  @li@c  WLAN_MW_WPS_WAIT_WSC_M4                        - WPS Wait M4 state. 
 *  @li@c  WLAN_MW_WPS_WAIT_WSC_M6                        - WPS Wait M6 state. 
 *  @li@c  WLAN_MW_WPS_WAIT_WSC_M8                        - WPS Wait M8 state. 
 *  @li@c  WLAN_MW_WPS_WAIT_EAP_FAIL                       - WPS Wait EAP Fail state. 
 *  @li@c  WLAN_MW_WPS_WAIT_WPA_SUPC                     - WPS Connecting state. 
 *  @li@c  WLAN_MW_WPS_CONNECTED                             - WPS Connected state. 
 *  @li@c  WLAN_MW_WPS_FAIL                                       - WPS Fail state. 
 *  @li@c  WLAN_MW_WPS_PERIOD_START                         - WPS period state. 
 *  @li@c  WLAN_MW_WPS_STATE_MAX                             - WPS state Max number. 
 */
/*------------------------------------------------------------------*/
typedef enum
{
    WLAN_MW_WPS_IDLE            = 0,   
    WLAN_MW_WPS_FAIL_RE_DO,            
    WLAN_MW_WPS_DISCOVER,              
    WLAN_MW_WPS_ASSOCIATING,           
    WLAN_MW_WPS_WAIT_REQ_ID,           
    WLAN_MW_WPS_WAIT_WSC_START,        
    WLAN_MW_WPS_WAIT_WSC_M2,           
    WLAN_MW_WPS_WAIT_WSC_M4,           
    WLAN_MW_WPS_WAIT_WSC_M6,           
    WLAN_MW_WPS_WAIT_WSC_M8,           
    WLAN_MW_WPS_WAIT_EAP_FAIL,            
    WLAN_MW_WPS_WAIT_WPA_SUPC,         
    WLAN_MW_WPS_CONNECTED,             
    WLAN_MW_WPS_FAIL,                  
    WLAN_MW_WPS_PERIOD_START,          
    WLAN_MW_WPS_STATE_MAX              
}   WLAN_WPS_STATE_T;


/*------------------------------------------------------------------*/
/*! @enum WLAN_WPA_STATUS_T
 *  @brief WPAStatus Defines for WPA status codes
 *  @code
 *  typedef enum
 *  {
 *  	WLAN_WPA_DISCONNECTED           = 0,   
 *  	WLAN_WPA_INACTIVE,                     
 *  	WLAN_WPA_SCANNING,                     
 *  	WLAN_WPA_ASSOCIATING,                  
 *  	WLAN_WPA_ASSOCIATED,                   
 *  	WLAN_WPA_4WAY_HANDSHAKE,               
 *  	WLAN_WPA_GROUP_HANDSHAKE,              
 *  	WLAN_WPA_COMPLETED                     
 *  }   WLAN_WPA_STATUS_T;
 *  @endcode
 *  @li@c  WLAN_WPA_DISCONNECTED                         - Disconnected state.                
 *  @li@c  WLAN_WPA_INACTIVE                                 - Inactive state (WPA disabled).     
 *  @li@c  WLAN_WPA_SCANNING                                - Scanning for a network.            
 *  @li@c  WLAN_WPA_ASSOCIATING                           - Trying to associate with a BSS/SSID. 
 *  @li@c  WLAN_WPA_ASSOCIATED                            - Association completed.             
 *  @li@c  WLAN_WPA_4WAY_HANDSHAKE                    - WPA 4-Way Key Handshake in progress. 
 *  @li@c  WLAN_WPA_GROUP_HANDSHAKE                   - WPA Group Key Handshake in progress. 
 *  @li@c  WLAN_WPA_COMPLETED                             - All authentication completed.       
 */
/*------------------------------------------------------------------*/
typedef enum
{
    /* normal status */
	WLAN_WPA_DISCONNECTED           = 0,   
	WLAN_WPA_INACTIVE,                     
	WLAN_WPA_SCANNING,                     
	WLAN_WPA_ASSOCIATING,                  
	WLAN_WPA_ASSOCIATED,                   
	WLAN_WPA_4WAY_HANDSHAKE,               
	WLAN_WPA_GROUP_HANDSHAKE,              
	WLAN_WPA_COMPLETED                     
}   WLAN_WPA_STATUS_T;



/*------------------------------------------------------------------*/
/*! @enum NET_802_11_ASSOC_CASE_T
 *  @brief AssocCase Defines for association cases
 *  @code
 *  typedef enum
 *  {
 *      IEEE_802_11_PWR_ON_ASSOC,                       
 *      IEEE_802_11_MANUAL_ASSOC,                       
 *      IEEE_802_11_SCAN_ASSOC,                         
 *      IEEE_802_11_WPS_ASSOC                           
 *  }   NET_802_11_ASSOC_CASE_T;
 *  @endcode
 *  @li@c  IEEE_802_11_PWR_ON_ASSOC                         - power on association.           
 *  @li@c  IEEE_802_11_MANUAL_ASSOC                         - manual association.             
 *  @li@c  IEEE_802_11_SCAN_ASSOC                             - scan and select SSID to associate.
 *  @li@c  IEEE_802_11_WPS_ASSOC                              - WPS association. 
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IEEE_802_11_PWR_ON_ASSOC,                       
    IEEE_802_11_MANUAL_ASSOC,                       
    IEEE_802_11_SCAN_ASSOC,                         
    IEEE_802_11_WPS_ASSOC                           
}   NET_802_11_ASSOC_CASE_T;



/*------------------------------------------------------------------*/
/*! @enum NET_802_11_AUTH_MODE_T
 *  @brief  802.11 auth mode
 *  @code
 *  typedef enum
 *  {
 *      IEEE_802_11_AUTH_MODE_NONE,                     
 *      IEEE_802_11_AUTH_MODE_OPEN,                     
 *      IEEE_802_11_AUTH_MODE_SHARED,                   
 *      IEEE_802_11_AUTH_MODE_WEPAUTO,                  
 *      IEEE_802_11_AUTH_MODE_WPA,                      
 *      IEEE_802_11_AUTH_MODE_WPA_PSK,                  
 *      IEEE_802_11_AUTH_MODE_WPA2,                     
 *      IEEE_802_11_AUTH_MODE_WPA2_PSK,                 
 *      IEEE_802_11_AUTH_MODE_WPAPSK_OR_WPA2PSK         
 *  }   NET_802_11_AUTH_MODE_T;
 *  @endcode
 *  @li@c  IEEE_802_11_AUTH_MODE_NONE                                - no security. 
 *  @li@c  IEEE_802_11_AUTH_MODE_OPEN                                - WEP open system.
 *  @li@c  IEEE_802_11_AUTH_MODE_SHARED                             - WEP shared key. 
 *  @li@c  IEEE_802_11_AUTH_MODE_WEPAUTO                          - WEP open/share auto switch.
 *  @li@c  IEEE_802_11_AUTH_MODE_WPA                                 - WPA 802.1X enterprise mdoe.
 *  @li@c  IEEE_802_11_AUTH_MODE_WPA_PSK                           - WPA pre-share key mdoe. 
 *  @li@c  IEEE_802_11_AUTH_MODE_WPA2                                - WPA2 802.1X enterprise mdoe. 
 *  @li@c  IEEE_802_11_AUTH_MODE_WPA2_PSK                         - WPA2 pre-share key mdoe. 
 *  @li@c  IEEE_802_11_AUTH_MODE_WPAPSK_OR_WPA2PSK         - Either WPA-PSK or WPA2-PSK mdoe.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IEEE_802_11_AUTH_MODE_NONE,                     
    IEEE_802_11_AUTH_MODE_OPEN,                     
    IEEE_802_11_AUTH_MODE_SHARED,                   
    IEEE_802_11_AUTH_MODE_WEPAUTO,                  
    IEEE_802_11_AUTH_MODE_WPA,                      
    IEEE_802_11_AUTH_MODE_WPA_PSK,                  
    IEEE_802_11_AUTH_MODE_WPA2,                     
    IEEE_802_11_AUTH_MODE_WPA2_PSK,                 
    IEEE_802_11_AUTH_MODE_WPAPSK_OR_WPA2PSK,         
    IEEE_802_11_AUTH_MODE_WPA_EAP,   // add for wpa_eap now DTV is not support
    IEEE_802_11_AUTH_MODE_WPA2_EAP,	// add for wpa_eap now DTV is not support
	IEEE_802_11_AUTH_MODE_WPAEAP_OR_WPA2EAP // add for wpa_eap now DTV is not support
}   NET_802_11_AUTH_MODE_T;

#define IEEE_802_11_AUTH_MODE_WEP IEEE_802_11_AUTH_MODE_WEPAUTO    /**< AuthMode       */



/*------------------------------------------------------------------*/
/*! @enum NET_802_11_AUTH_CIPHER_T
 *  @brief CipherMode Defines for cipher modes
 *  @code
 *  typedef enum
 *  {
 *      IEEE_802_11_AUTH_CIPHER_NONE        = (0x01 << 0),  
 *      IEEE_802_11_AUTH_CIPHER_WEP40      = (0x01 << 1),  
 *      IEEE_802_11_AUTH_CIPHER_WEP104     = (0x01 << 2),  
 *      IEEE_802_11_AUTH_CIPHER_TKIP         = (0x01 << 3),  
 *      IEEE_802_11_AUTH_CIPHER_AES          = (0x01 << 4),  
 *      IEEE_802_11_AUTH_CIPHER_AES_OR_TKIP = (IEEE_802_11_AUTH_CIPHER_AES | IEEE_802_11_AUTH_CIPHER_TKIP)  
 *  }   NET_802_11_AUTH_CIPHER_T;
 *  @endcode
 *  @li@c  IEEE_802_11_AUTH_CIPHER_NONE                      - No cipher  
 *  @li@c  IEEE_802_11_AUTH_CIPHER_WEP40                    - WEP40 cipher 
 *  @li@c  IEEE_802_11_AUTH_CIPHER_WEP104                  - WEP104 cipher
 *  @li@c  IEEE_802_11_AUTH_CIPHER_TKIP                       - YKIP cipher 
 *  @li@c  IEEE_802_11_AUTH_CIPHER_AES                        - AES cipher 
 *  @li@c  IEEE_802_11_AUTH_CIPHER_AES_OR_TKIP           - TKIP/AES cipher 
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IEEE_802_11_AUTH_CIPHER_NONE        = (0x01 << 0),  
    IEEE_802_11_AUTH_CIPHER_WEP40       = (0x01 << 1),  
    IEEE_802_11_AUTH_CIPHER_WEP104      = (0x01 << 2),  
    IEEE_802_11_AUTH_CIPHER_TKIP        = (0x01 << 3),  
    IEEE_802_11_AUTH_CIPHER_AES         = (0x01 << 4),  
    IEEE_802_11_AUTH_CIPHER_AES_OR_TKIP = (IEEE_802_11_AUTH_CIPHER_AES | IEEE_802_11_AUTH_CIPHER_TKIP)  
}   NET_802_11_AUTH_CIPHER_T;



/*------------------------------------------------------------------*/
/*! @enum NET_802_11_WPS_ASSOC_MODE_T
 *  @brief WPSOpMode Defines for WPS operation mode
 *  @code
 *  typedef enum
 *  {
 *      IEEE_802_11_PIN_SETUP,                          
 *      IEEE_802_11_PBC_SETUP,                          
 *      IEEE_802_11_NFC_SETUP,                          
 *      IEEE_802_11_USB_SETUP                           
 *  }   NET_802_11_WPS_ASSOC_MODE_T;
 *  @endcode
 *  @li@c  IEEE_802_11_PIN_SETUP                             - WPS PIN.
 *  @li@c  IEEE_802_11_PBC_SETUP                             - WPS PBC.
 *  @li@c  IEEE_802_11_NFC_SETUP                             - WPS NFC.
 *  @li@c  IEEE_802_11_USB_SETUP                             - WPS USB.
 */
/*------------------------------------------------------------------*/
typedef enum
{
    IEEE_802_11_PIN_SETUP,                          
    IEEE_802_11_PBC_SETUP,                          
    IEEE_802_11_NFC_SETUP,                          
    IEEE_802_11_USB_SETUP                           
}   NET_802_11_WPS_ASSOC_MODE_T;


/*--------------------------------------------------------------*/
/*                                                              */
/* Structure                                                    */
/*                                                              */
/*--------------------------------------------------------------*/




/*------------------------------------------------------------------*/
/*! @struct NET_802_11_KEY_T
 *  @brief Structure of Key
 *  @code
 *  typedef struct
 *  {
 *      BOOL    b_IsAscii;		
 *      CHAR    *pui1_PassPhrase;   
 *  }   NET_802_11_KEY_T;
 *  @endcode
 *  @li@c  b_IsAscii                                         - TRUE for ASCII passphrase(key).
 *  @li@c  *pui1_PassPhrase                             -Buffer pointer of the key content. 
 *
 *                                                            1.)8-63 ASCII or 64 hexadecimal words for valid WPA key
 *
 *                                                            2.)5 ASCII or 10 hexadecimal words for valid 64-bit WEP key 
 *
 *                                                            3.)13 ASCII or 26 hexadecimal words for valid 128-bit WEP key
 */
/*------------------------------------------------------------------*/
typedef struct
{
    BOOL    b_IsAscii;		
    CHAR    *pui1_PassPhrase;   
    CHAR    key_body[64];         /* Benny'100521 add */
}   NET_802_11_KEY_T;



/*------------------------------------------------------------------*/
/*! @struct NET_802_11_SSID_T
 *  @brief Structure of SSID
 *  @code
 *  typedef struct 
 *  {
 *      UINT32  ui4_SsidLen;                            
 *      CHAR    ui1_aSsid[NET_802_11_MAX_LEN_SSID];     
 *  }   NET_802_11_SSID_T;
 *  @endcode
 *  @li@c  ui4_SsidLen                                                     - SSID length in bytes.                        
 *  @li@c  ui1_aSsid[NET_802_11_MAX_LEN_SSID]                - SSID array with \ref SSIDLen "maximum length". 
 */
/*------------------------------------------------------------------*/
typedef struct 
{
    UINT32  ui4_SsidLen;                            
    CHAR    ui1_aSsid[NET_802_11_MAX_LEN_SSID];     
}   NET_802_11_SSID_T;





/*------------------------------------------------------------------*/
/*! @struct NET_802_11_SCAN_T
 *  @brief Structure of Scan Mode 
 *  @code
 *  typedef struct 
 *  {
 *      NET_802_11_SSID_T           t_Ssid;             
 *      BOOL                        b_ImplicitScan;     
 *  }   NET_802_11_SCAN_T;
 *  @endcode
 *  @li@c  t_Ssid                                               - SSID in null-terminated string form. 
 *  @li@c  b_ImplicitScan                                    - TRUE for Implicit scan or FLASE for explicit scan.
 */
/*------------------------------------------------------------------*/
typedef struct 
{
    NET_802_11_SSID_T           t_Ssid;             
    BOOL                        b_ImplicitScan;     
}   NET_802_11_SCAN_T;





/*------------------------------------------------------------------*/
/*! @struct NET_802_11_BSS_INFO_T
 *  @brief Structure of BSS Information
 *  @code
 *  typedef struct 
 *  {
 *      NET_802_11_BSSID_T                  t_Bssid;        
 *      NET_802_11_SSID_T                    t_Ssid;         
 *      NET_802_11_AUTH_MODE_T          e_AuthMode;     
 *      NET_802_11_AUTH_CIPHER_T        e_AuthCipher;   
 *      INT16                                        i2_Freq;        
 *      INT16                                        i2_Channel;     
 *      INT16                                        i2_Quality;
 *      INT16                                        i2_Noise;
 *      INT16                                        i2_Level;       
 *      INT32                                        i4_MaxRate;     
 *      UINT16                                      u2_Caps;        
 *      UINT8                                       *ie;            
 *      UINT32                                      u4_ieLength;    
 *  }   NET_802_11_BSS_INFO_T;
 *  @endcode
 *  @li@c  t_Bssid                                           - BSSID in binary form. If it's unknown, set it as ff:ff:ff:ff:ff:ff
 *  @li@c  t_Ssid                                            - SSID in null-terminated string form. 
 *  @li@c  e_AuthMode                                    - Authentication mode. 
 *  @li@c  e_AuthCipher                                   - Cipher mode. 
 *  @li@c  i2_Freq                                           - Primary radio frequency in unit of MHz 
 *  @li@c  i2_Channel                                      - Primary radio channel number
 *  @li@c  i2_Quality                                        - Quality of signal
 *  @li@c  i2_Noise                                          - Noise level
 *  @li@c  i2_Level                                          - Normalized signal strength ranged from 1 to 100. 
 *  @li@c  i4_MaxRate                                      - Max. supported radio rate of WiFi driver.
 *  @li@c  u2_Caps                                          - Capabilities included in the 802.11 management frames.
 *  @li@c  *ie                                                 - Buffer pointer of the information elements.
 *  @li@c  u4_ieLength                                     - Buffer length of the information elements. 
 *  @li@is_wps_support						0 not support 1-support 2-unknown
 */
/*------------------------------------------------------------------*/
typedef struct 
{
    NET_802_11_BSSID_T              t_Bssid;        
    NET_802_11_SSID_T               t_Ssid;         
    NET_802_11_AUTH_MODE_T          e_AuthMode;     
    NET_802_11_AUTH_CIPHER_T        e_AuthCipher;   
    INT16                           i2_Freq;        
    INT16                           i2_Channel;     
    INT16                           i2_Quality;
    INT16                           i2_Noise;
    INT16                           i2_Level;       
    INT32                           i4_MaxRate;     
    UINT16                          u2_Caps;        
    UINT8                           *ie;            
    UINT32                          u4_ieLength;    
	UINT8                           is_wps_support; 
	UINT8 							u8_wpa_status;
}   NET_802_11_BSS_INFO_T;



/*------------------------------------------------------------------*/
/*! @struct NET_802_11_SCAN_RESULT_T
 *  @brief Structure of Scan Result
 *  @code
 *  typedef struct 
 *  {
 *      UINT32                                 ui4_NumberOfItems;  
 *      NET_802_11_BSS_INFO_T       *p_BssInfo;         
 *  }   NET_802_11_SCAN_RESULT_T;
 *  @endcode
 *  @li@c  ui4_NumberOfItems                             - Number of entries in the list.
 *  @li@c  *p_BssInfo                                        - Buffer in which BSS information is stored. The size of this buffer must
 *                                                             be enough to store num x sizeof(NET_802_11_BSS_INFO_T) bytes.
 */
/*------------------------------------------------------------------*/
typedef struct 
{
    UINT32                      ui4_NumberOfItems;  
    NET_802_11_BSS_INFO_T       *p_BssInfo;         
}   NET_802_11_SCAN_RESULT_T;



/*------------------------------------------------------------------*/
/*! @struct NET_802_11_ESS_INFO_T
 *  @brief Structure of ESS Information
 *  @code
 *  typedef struct 
 *  {
 *      NET_802_11_BSSID_T               t_Bssid;           
 *      NET_802_11_SSID_T                t_Ssid;            
 *      NET_802_11_AUTH_MODE_T      e_AuthMode;        
 *      NET_802_11_AUTH_CIPHER_T    e_AuthCipher;      
 *      INT16                                    i2_Level;          
 *      INT32                                    i4_isConnected;
 *  }   NET_802_11_ESS_INFO_T;
 *  @endcode
 *  @li@c  t_Bssid                                           - BSSID in binary form. 
 *  @li@c  t_Ssid                                            - SSID in null-terminated string form. 
 *  @li@c  e_AuthMode                                    - Authentication mode. 
 *  @li@c  e_AuthCipher                                   - Cipher mode. 
 *  @li@c  i2_Level                                          - Normalized signal strength ranged from 1 to 100.
 *  @li@c  i4_isConnected                                 - Already connect with it
 */
/*------------------------------------------------------------------*/
typedef struct 
{
    NET_802_11_BSSID_T          t_Bssid;           
    NET_802_11_SSID_T           t_Ssid;            
    NET_802_11_AUTH_MODE_T      e_AuthMode;        
    NET_802_11_AUTH_CIPHER_T    e_AuthCipher;      
    INT16                       i2_Level;          
    INT32                       i4_isConnected;
	UINT8                       is_wps_support; //	
}   NET_802_11_ESS_INFO_T;



/*------------------------------------------------------------------*/
/*! @struct NET_802_11_ESS_LIST_T
 *  @brief Structure of ESS List
 *  @code
 *  typedef struct 
 *  {
 *      UINT32                                 ui4_NumberOfItems;  
 *      NET_802_11_ESS_INFO_T       *p_EssInfo;         
 *  }   NET_802_11_ESS_LIST_T;
 *  @endcode
 *  @li@c  ui4_NumberOfItems                             - Number of entries in the list.
 *  @li@c  *p_EssInfo                                        -Buffer in which ESS information is stored. The size of this buffer must
 *                                                            be enough to store num x sizeof(NET_802_11_ESS_INFO_T) bytes. 
 */                                                           
/*------------------------------------------------------------------*/
typedef struct 
{
    UINT32                      ui4_NumberOfItems;  
    NET_802_11_ESS_INFO_T       *p_EssInfo;         
                                                    
}   NET_802_11_ESS_LIST_T;



/*------------------------------------------------------------------*/
/*! @struct NET_802_11_ASSOCIATE_T
 *  @brief Structure of Association
 *  @code
 *  typedef struct 
 *  {
 *      NET_802_11_BSSID_T               t_Bssid;            
 *      NET_802_11_SSID_T                t_Ssid;             
 *      UINT8                                    ui1_Priority;       
 *      NET_802_11_ASSOC_CASE_T     e_AssocCase;        
 *      NET_802_11_AUTH_MODE_T      e_AuthMode;         
 *      NET_802_11_AUTH_CIPHER_T    e_AuthCipher;       
 *      NET_802_11_KEY_T        	        t_Key;              
 *  }   NET_802_11_ASSOCIATE_T; 
 *  @endcode
 *  @li@c  t_Bssid                                           - BSSID in binary form. 
 *  @li@c  t_Ssid                                            - SSID in null-terminated string form.
 *  @li@c  ui1_Priority                                      - Priority of the selected SSID.
 *  @li@c  e_AssocCase                                   - Association condition.
 *  @li@c  e_AuthMode                                    - Authentication mode. 
 *  @li@c  e_AuthCipher                                   - Cipher mode.
 *  @li@c  t_Key                                             - Key information. 
 */
/*------------------------------------------------------------------*/
typedef struct 
{
    NET_802_11_BSSID_T          t_Bssid;            
    NET_802_11_SSID_T           t_Ssid;             
    UINT8                       ui1_Priority;       
    NET_802_11_ASSOC_CASE_T     e_AssocCase;        
    NET_802_11_AUTH_MODE_T      e_AuthMode;         
    NET_802_11_AUTH_CIPHER_T    e_AuthCipher;       
    NET_802_11_KEY_T        	t_Key;              
}   NET_802_11_ASSOCIATE_T; 



/*------------------------------------------------------------------*/
/*! @struct NET_802_11_EEPORM_T
 *  @brief Structure of EEPORM Data
 *  @code
 *  typedef struct 
 *  {
 *      UINT32  u4Addr;                                 
 *      UINT8   *pData;                                 
 *      UINT32  u4Len;                                  
 *  }   NET_802_11_EEPORM_T; 
 *  @endcode
 *  @li@c  u4Addr                                            - EEPORM address. 
 *  @li@c  *pData                                           - Pointer to the data.
 *  @li@c  u4Len                                             - Size in bytes. 
 */
/*------------------------------------------------------------------*/
typedef struct 
{
    UINT32  u4Addr;                                 
    UINT8   *pData;                                 
    UINT32  u4Len;                                  
}   NET_802_11_EEPORM_T; 


typedef enum
{
    FLAG_NONE               = (0x00),                       
    FLAG_REPLY_NOT_END      = (0x01 << 0)
}   WPA_FLAG_T;


/*------------------------------------------------------------------*/
/*! @struct NET_802_11_WPA_T
 * @brief Structure of WPA Data
 *  @code
 *  typedef struct 
 *  {
 *      CHAR     ifname[8]; 
 *      UINT32   ui4_type;
 *      UINT8    pData[256];                                
 *  }   NET_802_11_WPA_T; 
 *  @endcode
 *  @li@c  ifname[8]                                       - Ctrl interface name
 *  @li@c  ui4_type                                         - WLAN_OID_T
 *  @li@c  pData[256]                                     - Data area.
 */
/*------------------------------------------------------------------*/
typedef struct 
{
    CHAR        ifname[8];  
    UINT32      u4type;
    INT32       i4Ret;
    WPA_FLAG_T  eFlag;
    UINT8       pData[NET_WPA_DATA_LENGTH];                                 
}   NET_802_11_WPA_T; 


/*------------------------------------------------------------------*/
/*! @struct NET_802_11_NFY_T
 * @brief Structure of WPA Data
 *  @code
 *  typedef struct 
 *  {
 *      INT32     i4NotifykId;
 *      INT32     pParam[256];                                 
 *  }   NET_802_11_NFY_T; 
 *  @endcode
 *  @li@c  i4NotifykId                                        - WLAN_NOTIFY_ID_T
 *  @li@c  pParam[256]                                     - Data area.
 */
/*------------------------------------------------------------------*/
typedef struct 
{
    INT32   i4NotifykId;
    INT32   i4Ret;
    INT32   pParam[NET_NFY_DATA_LENGTH];                                 
}   NET_802_11_NFY_T; 


/*----------------------------------------------------------------------------*/
/*! @} */                                           /* end of groupMW_INET_WIFI*/
/*----------------------------------------------------------------------------*/
/** 
 * @brief Generic RSSI Types
 */
typedef struct{
    UINT8      bc_avg_rssi;     /* average rssi */
    UINT8      bc_valid_mask;   /* bitmap of valid elements in rssi_ctrl/ext array */
    UINT8      bc_rssi_ctrl[ACFG_MAX_ANTENNA];
    UINT8      bc_rssi_ext[ACFG_MAX_ANTENNA];
    UINT8      data_avg_rssi;     /* average rssi */
    UINT8      data_valid_mask;   /* bitmap of valid elements in rssi_ctrl/ext array */
    UINT8      data_rssi_ctrl[ACFG_MAX_ANTENNA];
    UINT8      data_rssi_ext[ACFG_MAX_ANTENNA];
} NET_802_11_CHAIN_RSSI_T;


#define ACFG_CUSTDATA_LENGTH       20
/**
 * @brief get custdata
 */
typedef struct {   
    UINT8      custdata[ACFG_CUSTDATA_LENGTH]; 
}NET_802_11_ACFG_CUSTDAT_T;


/**
 * @brief get p2p find device data ;
 *  ret_value return value ;
 *  _mac the finded p2p device mac addr (wlan2) 
 * _addr the other mac addr (wlan1) of finded p2p device .
 * _namd the finded p2p devide name.
 */

typedef struct
{
    UINT8    ret_value;		
    CHAR    _mac[18];   
    CHAR    _addr[18];
	CHAR    _name[65];
}   NET_802_11_P2P_DATA;

/**
 * @brief get p2p find device data ;
 *  P2P_CONNECT_MODE_PBC : p2p connect with pbc .
 *  P2P_CONNECT_MODE_PIN  : p2p connect with pin.
 */

typedef enum
{
    P2P_CONNECT_MODE_PBC,
	P2P_CONNECT_MODE_PIN
	
} P2P_CONNECT_MODE_T;


#endif /* end of _U_NET_WLAN_H */



