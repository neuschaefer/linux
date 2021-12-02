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
/*! @file u_rrctx_vchip_eng.h 
 *  $RCSfile: $
 *  $Revision: #1 $
 *  $Date: 2012/04/27 $
 *  $Author: dtvbm11 $
 *  
 *  @par Description:
 *         This header file contains exported definitions and delcarations of
 *         VCHIP engine of RRCTX.
 */
/*----------------------------------------------------------------------------*/
 
#ifndef _U_RRCTX_VCHIP_ENG_H_
#define _U_RRCTX_VCHIP_ENG_H_

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "u_common.h"
 
/*----------------------------------------------------------------------------*/
/*! @defgroup groupMW_RRCTX_VCHIP_ENG RRCTX VCHIP Engine
 *
 *  @ingroup groupMW_RRCTX
 *  @brief The RRCTX RRT engine module is used to get rating information.
 *
 *  %MW Application API would be used by Application, lot of these APIs would
 *   be wrapped from Exported API. Using these APIs, application could get rating infromation
 *   from the exported API of RRCTX VCHIP engine.
 *
 *  @see groupMW_RRCTX_VCHIP_ENG
 *  @{ 
 */
/*----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Rating Region */
#define VCHIP_REGION_UNKNOWN         ((UINT8)    0xff)    /**< Region index	*/
#define VCHIP_REGION_US              ((UINT8)    0x01)    /**< Region index     */
#define VCHIP_REGION_CAN             ((UINT8)    0x02)    /**< Region index     */

/* MPAA Rating */
#define RRCTX_ENG_VCHIP_US_MPAA_G     ((UINT8)   0)    /**<   MPAA rating type     */
#define RRCTX_ENG_VCHIP_US_MPAA_PG    ((UINT8)   1)    /**<   MPAA rating type      */
#define RRCTX_ENG_VCHIP_US_MPAA_PG_13 ((UINT8)   2)    /**<   MPAA rating type      */
#define RRCTX_ENG_VCHIP_US_MPAA_R     ((UINT8)   3)    /**<   MPAA rating type     */
#define RRCTX_ENG_VCHIP_US_MPAA_NC_17 ((UINT8)   4)    /**<   MPAA rating type      */
#define RRCTX_ENG_VCHIP_US_MPAA_X     ((UINT8)   5)    /**<   MPAA rating type     */
#define RRCTX_ENG_VCHIP_US_MPAA_OFF   ((UINT8)   6)    /**<   MPAA rating type      */

/* TV Age-based */
#define RRCTX_ENG_VCHIP_US_AGE_TV_Y   ((UINT8)   0)    /**<    TV Age-based type    */
#define RRCTX_ENG_VCHIP_US_AGE_TV_Y7  ((UINT8)   1)    /**<    TV Age-based type    */
#define RRCTX_ENG_VCHIP_US_AGE_TV_G   ((UINT8)   2)    /**<    TV Age-based type    */
#define RRCTX_ENG_VCHIP_US_AGE_TV_PG  ((UINT8)   3)    /**<    TV Age-based type    */
#define RRCTX_ENG_VCHIP_US_AGE_TV_14  ((UINT8)   4)    /**<    TV Age-based type   */
#define RRCTX_ENG_VCHIP_US_AGE_TV_MA  ((UINT8)   5)    /**<    TV Age-based type    */
#define RRCTX_ENG_VCHIP_US_AGE_OFF    ((UINT8)   6)    /**<    TV Age-based type    */

/* TV Content-based */
#define RRCTX_ENG_VCHIP_US_CNT_TV_Y       MAKE_BIT_MASK_64((8 * 2 + 2))    /**<   TV Content-based type     */
#define RRCTX_ENG_VCHIP_US_CNT_TV_Y7      MAKE_BIT_MASK_64((8 * 2 + 3))    /**<   TV Content-based type     */
#define RRCTX_ENG_VCHIP_US_CNT_TV_Y7_FV   MAKE_BIT_MASK_64((8 * 2 + 4))    /**<   TV Content-based type     */
#define RRCTX_ENG_VCHIP_US_CNT_TV_PG_D    MAKE_BIT_MASK_64((8 * 3 + 0))    /**<   TV Content-based type     */
#define RRCTX_ENG_VCHIP_US_CNT_TV_PG_L    MAKE_BIT_MASK_64((8 * 3 + 1))    /**<   TV Content-based type    */
#define RRCTX_ENG_VCHIP_US_CNT_TV_PG_S    MAKE_BIT_MASK_64((8 * 3 + 2))    /**<   TV Content-based type     */
#define RRCTX_ENG_VCHIP_US_CNT_TV_PG_V    MAKE_BIT_MASK_64((8 * 3 + 3))    /**<   TV Content-based type     */
#define RRCTX_ENG_VCHIP_US_CNT_TV_14_D    MAKE_BIT_MASK_64((8 * 4 + 0))    /**<   TV Content-based type    */
#define RRCTX_ENG_VCHIP_US_CNT_TV_14_L    MAKE_BIT_MASK_64((8 * 4 + 1))    /**<   TV Content-based type     */
#define RRCTX_ENG_VCHIP_US_CNT_TV_14_S    MAKE_BIT_MASK_64((8 * 4 + 2))    /**<   TV Content-based type    */
#define RRCTX_ENG_VCHIP_US_CNT_TV_14_V    MAKE_BIT_MASK_64((8 * 4 + 3))    /**<   TV Content-based type    */
#define RRCTX_ENG_VCHIP_US_CNT_TV_MA_L    MAKE_BIT_MASK_64((8 * 5 + 1))    /**<   TV Content-based type    */
#define RRCTX_ENG_VCHIP_US_CNT_TV_MA_S    MAKE_BIT_MASK_64((8 * 5 + 2))    /**<   TV Content-based type     */
#define RRCTX_ENG_VCHIP_US_CNT_TV_MA_V    MAKE_BIT_MASK_64((8 * 5 + 3))    /**<   TV Content-based type     */

/* VCHIP-Canada English */
#define RRCTX_ENG_VCHIP_CA_EN_E         ((UINT8)    0)    /**<   VCHIP-Canada English rating type     */
#define RRCTX_ENG_VCHIP_CA_EN_C         ((UINT8)    1)    /**<   VCHIP-Canada English rating type    */
#define RRCTX_ENG_VCHIP_CA_EN_C8        ((UINT8)    2)    /**<   VCHIP-Canada English rating type     */
#define RRCTX_ENG_VCHIP_CA_EN_G         ((UINT8)    3)    /**<   VCHIP-Canada English rating type     */
#define RRCTX_ENG_VCHIP_CA_EN_PG        ((UINT8)    4)    /**<   VCHIP-Canada English rating type     */
#define RRCTX_ENG_VCHIP_CA_EN_14        ((UINT8)    5)    /**<   VCHIP-Canada English rating type    */
#define RRCTX_ENG_VCHIP_CA_EN_18        ((UINT8)    6)    /**<   VCHIP-Canada English rating type     */
#define RRCTX_ENG_VCHIP_CA_EN_OFF       ((UINT8)    7)    /**<   VCHIP-Canada English rating type     */

/* VCHIP-Canada French */
#define RRCTX_ENG_VCHIP_CA_FR_E         ((UINT8)    0)    /**<    VCHIP-Canada French rating type   */
#define RRCTX_ENG_VCHIP_CA_FR_G         ((UINT8)    1)    /**<    VCHIP-Canada French rating type    */
#define RRCTX_ENG_VCHIP_CA_FR_8         ((UINT8)    2)    /**<    VCHIP-Canada French rating type    */
#define RRCTX_ENG_VCHIP_CA_FR_13        ((UINT8)    3)    /**<    VCHIP-Canada French rating type    */
#define RRCTX_ENG_VCHIP_CA_FR_16        ((UINT8)    4)    /**<    VCHIP-Canada French rating type    */
#define RRCTX_ENG_VCHIP_CA_FR_18        ((UINT8)    5)    /**<    VCHIP-Canada French rating type    */
#define RRCTX_ENG_VCHIP_CA_FR_OFF       ((UINT8)    6)    /**<    VCHIP-Canada French rating type    */

/* VCHIP-Canada */
#define RRCTX_ENG_VCHIP_CA_CNT_OFF      ((UINT32)  0)    /**<   VCHIP-Canada rating type     */
#define RRCTX_ENG_VCHIP_CA_CNT_EN_C     MAKE_BIT_MASK_32((8 * 2 + 1))    /**<    VCHIP-Canada rating type     */
#define RRCTX_ENG_VCHIP_CA_CNT_EN_C8    MAKE_BIT_MASK_32((8 * 2 + 2))    /**<    VCHIP-Canada rating type    */
#define RRCTX_ENG_VCHIP_CA_CNT_EN_G     MAKE_BIT_MASK_32((8 * 2 + 3))    /**<    VCHIP-Canada rating type     */
#define RRCTX_ENG_VCHIP_CA_CNT_EN_PG    MAKE_BIT_MASK_32((8 * 2 + 4))    /**<    VCHIP-Canada rating type     */
#define RRCTX_ENG_VCHIP_CA_CNT_EN_14    MAKE_BIT_MASK_32((8 * 2 + 5))    /**<    VCHIP-Canada rating type     */
#define RRCTX_ENG_VCHIP_CA_CNT_EN_18    MAKE_BIT_MASK_32((8 * 2 + 6))    /**<    VCHIP-Canada rating type     */
#define RRCTX_ENG_VCHIP_CA_CNT_FR_G     MAKE_BIT_MASK_32((8 * 3 + 1))    /**<    VCHIP-Canada rating type     */
#define RRCTX_ENG_VCHIP_CA_CNT_FR_8     MAKE_BIT_MASK_32((8 * 3 + 2))    /**<    VCHIP-Canada rating type     */
#define RRCTX_ENG_VCHIP_CA_CNT_FR_13    MAKE_BIT_MASK_32((8 * 3 + 3))    /**<    VCHIP-Canada rating type     */
#define RRCTX_ENG_VCHIP_CA_CNT_FR_16    MAKE_BIT_MASK_32((8 * 3 + 4))    /**<    VCHIP-Canada rating type     */
#define RRCTX_ENG_VCHIP_CA_CNT_FR_18    MAKE_BIT_MASK_32((8 * 3 + 5))    /**<    VCHIP-Canada rating type     */

/*------------------------------------------------------------------*/
/*! @enum	RRCTX_VCHIP_KEY_TYPE_T
 *  @brief		Key type of VCHIP
 
 *  @code
 *	typedef enum _RRCTX_VCHIP_KEY_TYPE_T
 *	{
 *		RRCTX_VCHIP_KEY_TYPE_UNKNOWN = 0,
 *		RRCTX_VCHIP_KEY_TYPE_RGN_TEXT,
 *		RRCTX_VCHIP_KEY_TYPE_RGN_ID,
 *		RRCTX_VCHIP_KEY_TYPE_TOTAL_NUMBER
 *	}RRCTX_VCHIP_KEY_TYPE_T; 
 *  @endcode
 
 *  @li@c	RRCTX_VCHIP_KEY_TYPE_UNKNOWN			- Unknown key type
 *  @li@c	RRCTX_VCHIP_KEY_TYPE_RGN_TEXT			- Key type of VCHIP region text
 *  @li@c	RRCTX_VCHIP_KEY_TYPE_RGN_ID			- Key type of VCHIP region index
 *  @li@c	RRCTX_VCHIP_KEY_TYPE_TOTAL_NUMBER		- Total number of key types
 */
/*------------------------------------------------------------------*/

typedef enum _RRCTX_VCHIP_KEY_TYPE_T
{
    RRCTX_VCHIP_KEY_TYPE_UNKNOWN = 0,
    RRCTX_VCHIP_KEY_TYPE_RGN_TEXT,
    RRCTX_VCHIP_KEY_TYPE_RGN_ID,
    RRCTX_VCHIP_KEY_TYPE_TOTAL_NUMBER
} RRCTX_VCHIP_KEY_TYPE_T;


/*------------------------------------------------------------------*/
/*! @struct	RRCTX_VCHIP_KEY_PARAM_T
 *  @brief		Key parameters structure
 
 *  @code
 *	typedef struct _RRCTX_VCHIP_KEY_PARAM_T
 *	{
 *		RRCTX_VCHIP_KEY_TYPE_T      	e_vchip_key_type;
 *		VOID*      					pv_vchip_key_param;
 *	} RRCTX_VCHIP_KEY_PARAM_T;  
 *  @endcode
 
 *  @li@c	e_vchip_key_type			-Key type of the VCHIP
 *  @li@c	pv_vchip_key_param		-Parameters of key type
 */
/*------------------------------------------------------------------*/

typedef struct _RRCTX_VCHIP_KEY_PARAM_T
{
    RRCTX_VCHIP_KEY_TYPE_T   e_vchip_key_type;
    VOID*                    pv_vchip_key_param;
} RRCTX_VCHIP_KEY_PARAM_T;


#endif /* _U_RRCTX_VCHIP_ENG_H_ */

/*----------------------------------------------------------------------------*/
/*! @} */                               /* end of groupMW_RRCTX_VCHIP_ENG         */
/*----------------------------------------------------------------------------*/

