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
 * $RCSfile: x_avc.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains AV Connectors driver specific
 *         definitions, which are exported.
 *---------------------------------------------------------------------------*/

#ifndef _X_AVC_H_
#define _X_AVC_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_common.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
/* Get operations */
#define AVC_GET_TYPE_NO_CONN_DISC_NFY   		    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 0))
#define AVC_GET_TYPE_SIGNAL_STATUS      		    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 1))
#define AVC_GET_TYPE_SCART_PIN_8_STATUS 		    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 2))
#define AVC_GET_TYPE_SCART_PIN_16_STATUS		    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 3))
#define AVC_GET_TYPE_COMBI_SIGNAL_STATUS		    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 4))
#define AVC_GET_TYPE_NUM_COMP_IDS       		    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 5))
#define AVC_GET_TYPE_COMP_ID            		    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 6))
#define AVC_GET_TYPE_CONT_COMBI_MON     		    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 7))
#define AVC_GET_TYPE_SCART_INP_CAP				    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 8))
#define AVC_GET_TYPE_SCART_BYPASS        		    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T) 9))
#define AVC_GET_TYPE_SCART_OUTPUT        		    (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)10))
#define AVC_GET_TYPE_CABLE_STATUS	                (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)11))
#define AVC_GET_TYPE_COMBI_CABLE_STATUS             (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)12))
#define AVC_GET_TYPE_SCART_INPUT_MODE               (RM_GET_TYPE_LAST_ENTRY + ((DRV_GET_TYPE_T)13))

/* Set operations */
#define AVC_SET_TYPE_NFY_FCT                 	    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 0))
#define AVC_SET_TYPE_NO_CONN_DISC_NFY              ((RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 1)) | RM_SET_TYPE_ARG_NO_REF)
#define AVC_SET_TYPE_SCART_BYPASS        		    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 2))
#define AVC_SET_TYPE_SCART_OUTPUT        		    (RM_SET_TYPE_LAST_ENTRY + ((DRV_SET_TYPE_T) 3))

/* AV Connector driver flags */
#define AVC_FLAG_DEC_REQ_FOR_TV_SIG_DETECTION     	MAKE_BIT_MASK_32(16)
#define AVC_FLAG_DEC_REQ_FOR_COLOR_SYS_DETECTION  	MAKE_BIT_MASK_32(17)
#define AVC_FLAG_DEC_REQ_FOR_AUD_SIG_DETECTION    	MAKE_BIT_MASK_32(18)
#define AVC_FLAG_SCART_SOURCE                       MAKE_BIT_MASK_32(19)
#define AVC_FLAG_SCART_SINK                         MAKE_BIT_MASK_32(20)
#define AVC_FLAG_SCART_MONITOR                      MAKE_BIT_MASK_32(21)

/* Disconnect Reason */
#define AVC_DISC_REASON_SIGNAL_LOSS   		        (RM_DISC_REASON_LAST_ENTRY + ((UINT32) 0))

/* Connect Type */
#define AVC_CONN_TYPE_ANY             		        (RM_CONN_TYPE_LAST_ENTRY + ((DRV_CONN_TYPE_T)  0))

/* Signal state bitmask */
#define AVC_STATE_NONE                		        MAKE_BIT_MASK_32(0)
#define AVC_STATE_TV_SIGNAL           		        MAKE_BIT_MASK_32(1)
#define AVC_STATE_AUDIO_SIGNAL        		        MAKE_BIT_MASK_32(2)
#define AVC_STATE_TV_SIGNAL_UNKNOWN                 MAKE_BIT_MASK_32(3)

/* Cable state bitmask */
#define AVC_STATE_CABLE_NONE                        ((UINT32) 0)
#define AVC_STATE_CABLE_PLUGGED                     MAKE_BIT_MASK_32(1)
#define AVC_STATE_CABLE_UNKNOWN                     MAKE_BIT_MASK_32(2)

/* SCART input capability bitmask */
#define AVC_SCART_INP_CAP_S_VIDEO       	        MAKE_BIT_MASK_32(0)
#define AVC_SCART_INP_CAP_COMPOSITE     	        MAKE_BIT_MASK_32(1)
#define AVC_SCART_INP_CAP_RGB   			        MAKE_BIT_MASK_32(2)
#define AVC_SCART_INP_CAP_COMPONENT     	        MAKE_BIT_MASK_32(3)

typedef enum
{
    AVC_COND_SIGNAL_STATUS = 0,
    AVC_COND_CABLE_STATUS,
    AVC_COND_SCART_PIN_8_STATUS,
    AVC_COND_SCART_PIN_16_STATUS,
    AVC_COND_COMBI_SIGNAL_STATUS,
    AVC_COND_COMBI_CABLE_STATUS
}   AVC_COND_T;

typedef enum
{
    AVC_SCART_PIN_8_UNKNOWN = 0,
    AVC_SCART_PIN_8_NO_SIGNAL,    
    AVC_SCART_PIN_8_ASPECT_16_9,
    AVC_SCART_PIN_8_ASPECT_4_3
}   AVC_SCART_PIN_8_T;

typedef enum
{
    AVC_SCART_PIN_16_UNKNOWN = 0,
    AVC_SCART_PIN_16_COMPOSITE,
    AVC_SCART_PIN_16_RGB,
    AVC_SCART_PIN_16_TOGGLE
}   AVC_SCART_PIN_16_T;

typedef enum
{
    AVC_SCART_INPUT_MODE_CVBS,
    AVC_SCART_INPUT_MODE_SV,
    AVC_SCART_INPUT_MODE_RGB,
    AVC_SCART_INPUT_MODE_MIX,
    AVC_SCART_INPUT_MODE_UNKNOWN,
}   AVC_SCART_INPUT_MODE_T;


typedef enum
{
    AVC_SCART_BYPASS_TRUE = 0,
    AVC_SCART_BYPASS_FALSE
}   AVC_SCART_BYPASS_T;

typedef struct _AVC_SCART_OUTPUT_T
{
    DRV_TYPE_T      t_dev_type;
    UINT16          ui2_id;
}   AVC_SCART_OUTPUT_T;

typedef VOID (*x_avc_nfy_fct)(VOID*     	pv_nfy_tag,
    						  AVC_COND_T	e_nfy_cond,
    						  UINT32    	ui4_data_1,
    						  UINT32    	ui4_data_2);

typedef struct _AVC_NFY_INFO_T
{
    VOID*  				pv_tag;
    x_avc_nfy_fct  		pf_avc_nfy;
}   AVC_NFY_INFO_T;

typedef struct _AVC_INDEX_COMP_ID_T
{
    UINT16            	ui2_index;
    DRV_COMP_ID_T     	t_comp_id;
}   AVC_INDEX_COMP_ID_T;

typedef struct _AVC_COMBI_SIGNAL_STATUS_T
{
    UINT16				ui2_index;
    UINT32              ui4_signal_status;
}   AVC_COMBI_SIGNAL_STATUS_T;

typedef struct _AVC_COMBI_CABLE_STATUS_T
{
    UINT16              ui2_index;
    UINT32              ui4_cable_status;
}   AVC_COMBI_CABLE_STATUS_T;

/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/

#endif /* _X_AVC_H_ */

