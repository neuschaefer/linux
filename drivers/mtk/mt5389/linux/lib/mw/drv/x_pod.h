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
 * $RCSfile: x_pod.h,v $ x_pod.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/5 $
 * $SWAuthor: Alec Lu $
 * $MD5HEX: 97267b1b9fa8e0a096ea2d61b5dcedee $
 *
 * Description:
 *         This header file contains PCMCIA/POD/CI driver specific 
 *         definitions, which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_POD_H_
#define _X_POD_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "res_mngr/x_rm.h"
#include "x_pcmcia.h"

#if 0
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* Get operations */
#define POD_GET_TYPE_POD_STATE      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define POD_GET_TYPE_ACCESS_MODE    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))
#define POD_GET_TYPE_READ_CIS       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))
#define POD_GET_TYPE_READ_REG       (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 3))
#define POD_GET_TYPE_READ_DATA      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 4))

/* Set operations */
#define POD_SET_TYPE_RESET          (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0))
#define POD_SET_TYPE_LOW_LEVEL_INIT (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1))
#define POD_SET_TYPE_ACCESS_MODE    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2))
#define POD_SET_TYPE_MODULE_MODE    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3))
#define POD_SET_TYPE_POWER_CTRL     (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4))
#define POD_SET_TYPE_NFY_FCT        (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5))
#define POD_SET_TYPE_WRITE_COR      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6))
#define POD_SET_TYPE_WRITE_REG      (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 7))
#define POD_SET_TYPE_WRITE_DATA     (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 8))

/* CI specific Get/Set operations */
#ifndef CI_SUPPORT_DVBT_BYPASS
/* Get operations */
#define CI_GET_TYPE_POD_ACTIVE      	 (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define CI_GET_TYPE_CARD_PRESENT    	 (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))
#define CI_GET_TYPE_CARD_READY      	 (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))
#define CI_GET_TYPE_READ_REG       		 (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 3))
#define CI_GET_TYPE_READ_COR      		 (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 4))
#define CI_GET_TYPE_READ_DATA			 (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 5))
#define CI_GET_TYPE_RECEIVED_PKT_NUM     (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 6))
#define CI_GET_TYPE_DROPPED_PKT_NUM      (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 7))
#define CI_GET_TYPE_PID_COUNTERS         (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 8))
#define CI_GET_TYPE_PID                  (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 9))

/* Set operations */
#define CI_SET_TYPE_INIT_CI				(RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0))
#define CI_SET_TYPE_RESET          		(RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1))
#define CI_SET_TYPE_MODULE_MODE    		(RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2))
#define CI_SET_TYPE_ACCESS_MODE    		(RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3))
#define CI_SET_TYPE_ENABLE_TS     		(RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 4))
#define CI_SET_TYPE_WRITE_REG        	(RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 5))
#define CI_SET_TYPE_WRITE_COR      		(RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 6))
#define CI_SET_TYPE_WRITE_DATA      	(RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 7))
#define CI_SET_TYPE_RESET_CAM     		(RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 8))
#define CI_SET_TYPE_CD_ON_OFF           (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 9))
#endif	/* #ifndef CI_SUPPORT_DVBT_BYPASS */
/* End CI specific Get/Set operations */

/* POD state */
#define POD_INSERTED                ((UINT32)   (1 << 0))
#define POD_FREE                    ((UINT32)   (1 << 1))
#define POD_DATA_AVIL               ((UINT32)   (1 << 2))
#define POD_FREE_EXT                ((UINT32)   (1 << 3))
#define POD_DATA_AVIL_EXT           ((UINT32)   (1 << 4))
#define POD_VS1                     ((UINT32)   (1 << 5)) /* Added for DK PODware */
#define POD_VS2                     ((UINT32)   (1 << 6)) /* Added for DK PODware */
#define POD_READY_IREQ              ((UINT32)   (1 << 7)) /* Added for DK PODware */
#define POD_OVERCURRENT             ((UINT32)   (1 << 8)) /* Added for DK PODware */

/* Definitions of POD status register */
#define POD_STS_RE                  ((UINT32)   (1 << 0))     
#define POD_STS_WE                  ((UINT32)   (1 << 1)) 
#define POD_STS_IIR                 ((UINT32)   (1 << 4)) 
#define POD_STS_FR                  ((UINT32)   (1 << 6))                     
#define POD_STS_DA                  ((UINT32)   (1 << 7))

/* Definition of POD signals */
#define POD_PIN_SET                 ((UINT32)   0x00000001)

/* POD condition */
typedef enum
{
    POD_COND_REMOVED = 0,
    POD_COND_INSERTED
}   POD_COND_T;

/* POD access mode */
typedef enum
{
    PCMCIA_ATTRIBUTE_MEM = 0,
    PCMCIA_COMMON_MEM,
    POD_DATA_CHANNEL,
    POD_EXTENDED_CHANNEL
}   POD_ACCESS_MODE_T;

/* POD module mode */
typedef enum
{
    MODULE_MODE_PCMCIA = 0,
    MODULE_MODE_POD
}   POD_MODULE_MODE_T;

/* POD power control */
typedef enum
{
    POD_VCC_OFF_VPP_OFF = 0,
    POD_VCC_33V_VPP_33V,
    POD_VCC_33V_VPP_50V,
}   POD_POWER_CTRL_T;

/* Notify function */
typedef VOID (*x_pod_nfy_fct)(VOID*     pv_nfy_tag,
                              UINT32    ui4_state,
                              UINT32    ui4_data);

/* Notify setting structure */
typedef struct _POD_NFY_INFO_T
{
    VOID*           pv_tag;
    x_pod_nfy_fct   pf_pod_nfy;
}   POD_NFY_INFO_T;                                                       

/* Data structure for POD signal control */
typedef struct _POD_SIGNAL_T
{
    UINT32  ui4_set;
    UINT32  ui4_clear;
}   POD_SIGNAL_T;  

/* COR info for POD personality change */
typedef struct _POD_COR_INFO_T
{
    UINT16  ui2_cor_address;
    UINT8   ui1_cor_value;
}   POD_COR_INFO_T;  

/* Data structure definition */
typedef struct _POD_DATA_INFO_T
{
    SIZE_T  z_data_len;
    UINT8*  pui1_data;
}   POD_DATA_INFO_T;  

/* Data structure for register read and write operation */
typedef struct _POD_REG_DATA_INFO_T
{
    UINT8               ui1_reg_addr;
    POD_DATA_INFO_T     t_data_info;
}   POD_REG_DATA_INFO_T;

/* -----------------------------------------------------------------------------
 * CI specific typedef
 * -----------------------------------------------------------------------------
 */
#ifndef CI_SUPPORT_DVBT_BYPASS
/* POD Enable TS */
typedef struct _POD_ENABLE_TS_T
{
    BOOL			b_from_ca;
    BOOL			b_pod_ts_mode_enable;
}   POD_ENABLE_TS_T;

/* POD Tuner Attributes */
typedef struct _POD_TUNER_ATTRIBUTE_T
{
    UINT16          ui2_tuner_id;
    DRV_SET_TYPE_T  t_drv_set_type;
    VOID*           pv_ana_attribute;
    SIZE_T          z_ana_attribute_len;
}	POD_TUNER_ATTRIBUTE_T;
#endif	/* #ifndef CI_SUPPORT_DVBT_BYPASS */

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
extern INT32 POD_InitMw(void);

#endif
#endif /* _X_POD_H_ */

