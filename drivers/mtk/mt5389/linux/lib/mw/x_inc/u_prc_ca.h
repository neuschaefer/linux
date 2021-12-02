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
/*! @file u_prc_ca.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *
 *  @par Description:
 *         This header file contains the CA related definition.  
 *
 * History:
 *  
 */
/*----------------------------------------------------------------------------*/

#ifndef _U_PM_CA_API_H_
#define _U_PM_CA_API_H_    /**<        */


/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_handle.h"
#include "u_common.h"
#include "u_os.h"
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_PRC_MNGR Process Manager
 *
 *  @ingroup groupMW
 *  @brief PM will manage filter and CA resource.
 *
 *  %MW Stream Manager will allocates video and audio filter by PM. If CA function
 *   is enabled, PM will handle the descrambling control.
 *
 *  @see groupMW
 *  @{ 
 */
/*----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* CP CCI */
#define CP_TYPE_MAX  (5)    /**< The CP data array MAX length */

/*------------------------------------------------------------------*/
/*! @enum  CP_TYPE_T
 *  @brief This enumeration contains the CP type PM supported
 *  @code
 *  typedef enum
 *  {
 *      CP_TYPE_NOTUSED = 0,
 *      CP_TYPE_UNKNOWN,
 *      CP_TYPE_EMI,
 *      CP_TYPE_APS,
 *      CP_TYPE_CIT,
 *      CP_TYPE_RCT,
 *      CP_TYPE_RL
 *  } CP_TYPE_T;
 *  @endcode
 *  @li@c  CP_TYPE_NOTUSED                                   - CP type is NOTUSED
 *  @li@c  CP_TYPE_UNKNOWN                                   - CP type is UNKNOWN
 *  @li@c  CP_TYPE_EMI                                       - CP type is EMI
 *  @li@c  CP_TYPE_APS                                       - CP type is APS
 *  @li@c  CP_TYPE_CIT                                       - CP type is CIT
 *  @li@c  CP_TYPE_RCT                                       - CP type is RCT
 *  @li@c  CP_TYPE_RL                                        - CP type is RL
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CP_TYPE_NOTUSED = 0,
    CP_TYPE_UNKNOWN,
    CP_TYPE_EMI,
    CP_TYPE_APS,
    CP_TYPE_CIT,
    CP_TYPE_RCT,
    CP_TYPE_RL
} CP_TYPE_T;

/*
EMI_COPY_NOT_RESTRICTED: Copying not restricted
EMI_NO_FURTHER_COPY: No further copying is permitted
EMI_ONE_GENERATION_COPY: One generation copy is permitted
EMI_COPY_PROHIBITED: Copying is prohibited
*/
/*------------------------------------------------------------------*/
/*! @enum  EMI_TYPE_T
 *  @brief This enumeration contains EMI information
 *  @code
 *  typedef enum
 *  {
 *      EMI_COPY_NOT_RESTRICTED = 0,
 *      EMI_NO_FURTHER_COPY = 0x01,
 *      EMI_ONE_GENERATION_COPY = 0x02,
 *      EMI_COPY_PROHIBITED = 0x03
 *  }   EMI_TYPE_T; 
 *  @endcode
 *  @li@c  EMI_COPY_NOT_RESTRICTED                           - Copy not restricted
 *  @li@c  EMI_NO_FURTHER_COPY                               - No further copy
 *  @li@c  EMI_ONE_GENERATION_COPY                           - One generation copy
 *  @li@c  EMI_COPY_PROHIBITED                               - Copy prohibited
 */
/*------------------------------------------------------------------*/
typedef enum
{
    EMI_COPY_NOT_RESTRICTED = 0,
    EMI_NO_FURTHER_COPY = 0x01,
    EMI_ONE_GENERATION_COPY = 0x02,
    EMI_COPY_PROHIBITED = 0x03
}   EMI_TYPE_T; 

/*
APS_CP_OFF: Copy protection encoding off.
APS_AGC_ON_SPLIT_BURST_OFF: AGC Process On, Split Burst Off.
APS_AGC_ON_2_LINE_SPLIT_BURST_ON: AGC Process On, 2 line Split Burst On.
PS_AGC_ON_4_LINE_SPLIT_BURST_OFF: AGC Process On, 4 line Split Burst On.
*/
/*------------------------------------------------------------------*/
/*! @enum  APS_TYPE_T
 *  @brief This enumeration contains APS information
 *  @code
 *  typedef enum
 *  {
 *      APS_CP_OFF = 0,
 *      APS_AGC_ON_SPLIT_BURST_OFF = 0x01,
 *      APS_AGC_ON_2_LINE_SPLIT_BURST_ON = 0x02,
 *      APS_AGC_ON_4_LINE_SPLIT_BURST_OFF = 0x03
 *  }   APS_TYPE_T; 
 *  @endcode
 *  @li@c  APS_CP_OFF                                        - cp off
 *  @li@c  APS_AGC_ON_SPLIT_BURST_OFF                        - agc on split burst off
 *  @li@c  APS_AGC_ON_2_LINE_SPLIT_BURST_ON                  - agc on 2 line split burst on
 *  @li@c  APS_AGC_ON_4_LINE_SPLIT_BURST_OFF                 - agc on 4 line split burst off
 */
/*------------------------------------------------------------------*/
typedef enum
{
    APS_CP_OFF = 0,
    APS_AGC_ON_SPLIT_BURST_OFF = 0x01,
    APS_AGC_ON_2_LINE_SPLIT_BURST_ON = 0x02,
    APS_AGC_ON_4_LINE_SPLIT_BURST_OFF = 0x03
}   APS_TYPE_T; 

/*------------------------------------------------------------------*/
/*! @enum  CIT_TYPE_T
 *  @brief This enumeration contains CIT information
 *  @code
 *  typedef enum
 *  {
 *      CIT_OFF = 0,
 *      CIT_ON = 0x01
 *  }   CIT_TYPE_T; 
 *  @endcode
 *  @li@c  CIT_OFF                                           - CIT off
 *  @li@c  CIT_ON                                            - CIT on
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CIT_OFF = 0,
    CIT_ON = 0x01
}   CIT_TYPE_T; 

/* RCT_OFF: No Redistribution Control asserted. Default. */
/* RCT_ON: Redistribution Control asserted. */
/*------------------------------------------------------------------*/
/*! @enum  RCT_TYPE_T
 *  @brief This enumeration contains RCT information
 *  @code
 *  typedef enum
 *  {
 *      RCT_OFF = 0,
 *      RCT_ON = 0x01
 *  }   RCT_TYPE_T;
 *  @endcode
 *  @li@c  RCT_OFF                                           - RCT off
 *  @li@c  RCT_ON                                            - RCT on
 */
/*------------------------------------------------------------------*/
typedef enum
{
    RCT_OFF = 0,
    RCT_ON = 0x01
}   RCT_TYPE_T;

/*
typedef UINT32 RL_TYPE_T;
0x00 : Default retention limit of 90 minutes applies.
0x01 : Retention limit 6 hours applies.
0x02 : Retention limit 12 hours applies
0x03..0x3F : Retention limit  1~ 61 multiples of 24 Hours applies.
*/
typedef UINT32 RL_TYPE_T;    /**< CP data type */

/*------------------------------------------------------------------*/
/*! @struct CP_INFO_T
 *  @brief  This structure contains the detail CP data
 *  @code
 *  typedef struct _CP_INFO_T
 *  {
 *      CP_TYPE_T t_cp_type;    
 *      union
 *      {
 *          EMI_TYPE_T e_emi_ctrl;
 *          APS_TYPE_T e_aps_ctrl;
 *          CIT_TYPE_T e_cit_ctrl;        
 *          RCT_TYPE_T e_rct_ctrl;
 *          RL_TYPE_T  t_rl_ctrl;
 *      }   u;  
 *  }   CP_INFO_T;
 *  @endcode
 *  @li@c  t_cp_type                                         - indicate CP data type
 *  @li@c  e_emi_ctrl                                        - EMI data
 *  @li@c  e_aps_ctrl                                        - APS data 
 *  @li@c  e_cit_ctrl                                        - CIT data 
 *  @li@c  e_rct_ctrl                                        - RCT data 
 *  @li@c  t_rl_ctrl                                         - RL data 
 */
/*------------------------------------------------------------------*/
typedef struct _CP_INFO_T
{
    CP_TYPE_T t_cp_type;    
    union
    {
        EMI_TYPE_T e_emi_ctrl;
        APS_TYPE_T e_aps_ctrl;
        CIT_TYPE_T e_cit_ctrl;        
        RCT_TYPE_T e_rct_ctrl;
        RL_TYPE_T  t_rl_ctrl;
    }   u;  
        
}   CP_INFO_T;

/*------------------------------------------------------------------*/
/*! @struct CP_INFO_LIST_T
 *  @brief  This structure contains the CP data group
 *  @code
 *  typedef struct _CP_INFO_LIST_T
 *  {
 *      UINT8             ui1_ctrl_size;
 *      CP_INFO_T         at_cp_ctrl_list[CP_TYPE_MAX];
 *  }   CP_INFO_LIST_T;
 *  @endcode
 *  @li@c  ui1_ctrl_size                                     - vaild size in at_cp_ctrl_list
 *  @li@c  at_cp_ctrl_list[CP_TYPE_MAX]                      - CP data array
 */
/*------------------------------------------------------------------*/
typedef struct _CP_INFO_LIST_T
{
    UINT8             ui1_ctrl_size;
    CP_INFO_T         at_cp_ctrl_list[CP_TYPE_MAX];
}   CP_INFO_LIST_T;


/* CA */
/*------------------------------------------------------------------*/
/*! @enum  CA_DETAIL_COND_T
 *  @brief This enumeration contains detail CA condition
 *  @code
 *  typedef enum
 *  {
 *      CA_DETAIL_UNKNOW = 0,
 *      POD_CA = 0x10,
 *      PODCA_DESCRAMBLE_POSSIBLE,
 *      PODCA_DESCRAMBLE_POSSIBLE_PURCHASE_DIALOGUE,
 *      PODCA_DESCRAMBLE_POSSIBLE_TECH_DIALOGUE,
 *      PODCA_DESCRAMBLE_NOT_POSSIBLE_NO_ENTITLEMENT,
 *      PODCA_DESCRAMBLE_NOT_POSSIBLE_TECH_REASON,
 *      PODCA_OTHER_COND
 *  }   CA_DETAIL_COND_T;
 *  @endcode
 *  @li@c  CA_DETAIL_UNKNOW                                  - unknown the detail condition
 *  @li@c  POD_CA                                            - control access
 *  @li@c  PODCA_DESCRAMBLE_POSSIBLE                         - descramble possible
 *  @li@c  PODCA_DESCRAMBLE_POSSIBLE_PURCHASE_DIALOGUE       - descramble possible purchase dialogue
 *  @li@c  PODCA_DESCRAMBLE_POSSIBLE_TECH_DIALOGUE           - descramble possible tech dialogue
 *  @li@c  PODCA_DESCRAMBLE_NOT_POSSIBLE_NO_ENTITLEMENT      - descramble not possible no entitlement
 *  @li@c  PODCA_DESCRAMBLE_NOT_POSSIBLE_TECH_REASON         - descramble not possible tech reason
 *  @li@c  PODCA_OTHER_COND                                  - other condition
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CA_DETAIL_UNKNOW = 0,
    POD_CA = 0x10,
    PODCA_DESCRAMBLE_POSSIBLE,
    PODCA_DESCRAMBLE_POSSIBLE_PURCHASE_DIALOGUE,
    PODCA_DESCRAMBLE_POSSIBLE_TECH_DIALOGUE,
    PODCA_DESCRAMBLE_NOT_POSSIBLE_NO_ENTITLEMENT,
    PODCA_DESCRAMBLE_NOT_POSSIBLE_TECH_REASON,
    PODCA_OTHER_COND
}   CA_DETAIL_COND_T;


/*------------------------------------------------------------------*/
/*! @enum  CA_CRYPTO_KEY_TYPE_T
 *  @brief This enumeration contains the descrambler key type
 *  @code
 *  typedef enum
 *  {
 *      CA_CRYPTO_KEY_INVALID = -1,
 *      CA_CRYPTO_KEY_EVEN,
 *      CA_CRYPTO_KEY_ODD
 *  }   CA_CRYPTO_KEY_TYPE_T;
 *  @endcode
 *  @li@c  CA_CRYPTO_KEY_INVALID                             - invalid key
 *  @li@c  CA_CRYPTO_KEY_EVEN                                - even key
 *  @li@c  CA_CRYPTO_KEY_ODD                                 - odd key
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CA_CRYPTO_KEY_INVALID = -1,
    CA_CRYPTO_KEY_EVEN,
    CA_CRYPTO_KEY_ODD
}   CA_CRYPTO_KEY_TYPE_T;


/*------------------------------------------------------------------*/
/*! @enum  CA_SET_COMMAND_T
 *  @brief This enumeration contains the command type
 *  @code
 *  typedef enum
 *  {
 *      CA_SET_INVALID = -1,
 *      CA_SET_ALG,
 *      CA_SET_KEY,
 *  }   CA_SET_COMMAND_T;
 *  @endcode
 *  @li@c  CA_SET_INVALID                                    - invalid command
 *  @li@c  CA_SET_ALG                                        - set algorithm command
 *  @li@c  CA_SET_KEY                                        - set key command
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CA_SET_INVALID = -1,
    CA_SET_ALG,
    CA_SET_KEY,
}   CA_SET_COMMAND_T;


/* x_ interface does not release. do not include x_desc.h in the file. 
   Re-declare Descrambler algorithms to CA_DESC_ALG_T inside u_prc_ca.h
   The structue must be the same as DESC_ALG_T inside x_desc.h */

/* Descrambler algorithms */
/*------------------------------------------------------------------*/
/*! @enum  CA_DESC_ALG_T
 *  @brief This enumeration contains the detail descrambler algorithm
 *  @code
 *  typedef enum
 *  {
 *      CA_DESC_ALG_NONE = 0,          
 *      CA_DESC_ALG_DES_ECB,           
 *      CA_DESC_ALG_DES_CBC,           
 *      CA_DESC_ALG_3DES_ECB,          
 *      CA_DESC_ALG_3DES_CBC,          
 *      CA_DESC_ALG_DVB,               
 *      CA_DESC_ALG_AES_128_ECB,       
 *      CA_DESC_ALG_AES_128_CBC        
 *  }   CA_DESC_ALG_T;
 *  @endcode
 *  @li@c  CA_DESC_ALG_NONE                                  - The unknown algorithm
 *  @li@c  CA_DESC_ALG_DES_ECB                               - DES-56-ECB algorithm
 *  @li@c  CA_DESC_ALG_DES_CBC                               - DES-56-CCB algorithm  
 *  @li@c  CA_DESC_ALG_3DES_ECB                              - 3DES-ECB algorithm
 *  @li@c  CA_DESC_ALG_3DES_CBC                              - 3DES-CBC algorithm
 *  @li@c  CA_DESC_ALG_DVB                                   - DVB algorithm
 *  @li@c  CA_DESC_ALG_AES_128_ECB                           - AES-128-ECB algorithm
 *  @li@c  CA_DESC_ALG_AES_128_CBC                           - AES-128-CBC algorithm
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CA_DESC_ALG_NONE = 0,          /* None scramble */
    CA_DESC_ALG_DES_ECB,           /* DES-56-ECB*/
    CA_DESC_ALG_DES_CBC,           /* DES-56-CCB*/
    CA_DESC_ALG_3DES_ECB,          /* 3DES-ECB*/
    CA_DESC_ALG_3DES_CBC,          /* 3DES-CCB*/
    CA_DESC_ALG_DVB,               /* DVB */
    CA_DESC_ALG_AES_128_ECB,       /* AES-128-ECB*/
    CA_DESC_ALG_AES_128_CBC        /* AES-128-CCB*/
}   CA_DESC_ALG_T;

/*------------------------------------------------------------------*/
/*! @struct CA_ALG_INFO_T
 *  @brief  This enumeration contains the descrambler algorithm
 *  @code
 *  typedef struct _CA_ALG_INFO_T
 *  {
 *      CA_DESC_ALG_T              t_desc_alg;
 *  }   CA_ALG_INFO_T;
 *  @endcode
 *  @li@c  t_desc_alg                                        - algorithm
 */
/*------------------------------------------------------------------*/
typedef struct _CA_ALG_INFO_T
{
    CA_DESC_ALG_T              t_desc_alg;
}   CA_ALG_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct CA_KEY_INFO_T
 *  @brief  This enumeration contains the descrambler key
 *  @code
 *  typedef struct _CA_KEY_INFO_T
 *  {
 *      CA_DESC_ALG_T           e_descrambler_alg;
 *      CA_CRYPTO_KEY_TYPE_T    t_key_type;
 *      UINT8*                  auc_key;
 *      INT32                   i4_key_size;
 *      UINT8*                  auc_iv;
 *      INT32                   i4_iv_size;
 *  }   CA_KEY_INFO_T;
 *  @endcode
 *  @li@c  e_descrambler_alg                                 - the corresponded algorithm
 *  @li@c  t_key_type                                        - key type
 *  @li@c  auc_key                                           - key data
 *  @li@c  i4_key_size                                       - valid data in auc_key
 *  @li@c  auc_iv                                            - iv key
 *  @li@c  i4_iv_size                                        - valid data in auc_iv
 */
/*------------------------------------------------------------------*/
typedef struct _CA_KEY_INFO_T
{
    CA_DESC_ALG_T           e_descrambler_alg;
    CA_CRYPTO_KEY_TYPE_T    t_key_type;
    UINT8*                  auc_key;
    INT32                   i4_key_size;
    UINT8*                  auc_iv;
    INT32                   i4_iv_size;
}   CA_KEY_INFO_T;


/*------------------------------------------------------------------*/
/*! @struct CA_SET_COMMAND_INFO_T
 *  @brief  This enumeration contains the command from CA engine
 *  @code
 *  typedef struct _CA_SET_COMMAND_INFO_T
 *  {
 *      CA_SET_COMMAND_T        t_ca_cmd;
 *      union
 *      {
 *          CA_ALG_INFO_T       t_algorithm;
 *          CA_KEY_INFO_T       t_key_info;
 *      }u_set_cmd_info;
 *  }   CA_SET_COMMAND_INFO_T;
 *  @endcode
 *  @li@c  t_ca_cmd                                          - command type
 *  @li@c  t_algorithm                                       - algorithm data
 *  @li@c  t_key_info                                        - key data
 */
/*------------------------------------------------------------------*/
typedef struct _CA_SET_COMMAND_INFO_T
{
    CA_SET_COMMAND_T        t_ca_cmd;
    union
    {
        CA_ALG_INFO_T       t_algorithm;
        CA_KEY_INFO_T       t_key_info;
    }u_set_cmd_info;
}   CA_SET_COMMAND_INFO_T;

/*------------------------------------------------------------------*/
/*! @enum  CAM_STATUS_T
 *  @brief This enumeration contains the command type
 *  @code
 *  typedef enum
 *  {
 *      CAM_NOT_INSERT = 0,,
 *      CAM_INSERT,
 *      CAM_INSERT_CIPLUS,
 *  }   CAM_STATUS_T
 *  @endcode
 *  @li@c  CAM_NOT_INSERT
 *  @li@c  CAM_INSERT
 *  @li@c  CAM_INSERT_CIPLUS
 */
/*------------------------------------------------------------------*/
typedef enum
{
    CAM_NOT_INSERT = 0,
    CAM_INSERT,
    CAM_INSERT_CIPLUS,
}   CAM_STATUS_T;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/


#endif /* _U_PM_CA_API_H_ */    

/*----------------------------------------------------------------------------*/
/*! @} */                                           /* end of groupMW_PRC_MNGR */
/*----------------------------------------------------------------------------*/


