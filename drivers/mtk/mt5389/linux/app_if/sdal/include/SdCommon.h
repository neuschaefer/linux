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
/**
* @mainpage SDAL API
	\image html SDAL_Title.jpg
* @section Description 개요
	본 문서는 Samsung Device Abstraction Layer의 Common API를 (이하 SDAL API) 설명한 것으로 
	Platform에 의존하지 않도록 정의하였다.
	
	아래 그림에 SDAL API를 Component 별로 구분하여 Block Diagram으로 나타내었다.
	
	각 Component의 API는 한 파일 내에 정의하는 것은 원칙으로 한다.
	
	This document explains Common API in Samsung Device Abstraction Layer (called SDAL API) which is independent of the specific platform.
	
	The following picture shows the block diagram of the components of SDAL API.
	
	APIs in each component is generally defined in one file.
	
	\image html SDAL_Diagram.jpg "SDAL API Data Flow Block Diagram"
	
* @section Information  작성 정보
* - 작 성 자 Authors : 김대환, 정규해, 윤여상, 김용운, 이광민, 신정은, 김윤희, 최병진, 김병준, 윤이나
* - 작성일자 Date    : 2006/07/17 
*/

/**
 * @file SdCommon.h
 * @brief Samsung DAL API common header file
 	Samsung DAL에서 공통으로 사용하는 API를 모아 놓음.
 	The APIs which are used in common in Samsung DAL are gathered together.
 * @author S/W Platform Lab
 * @date 2006/07/07
 */

#ifndef	_SDCOMMON_H_
#define	_SDCOMMON_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "SdType.h"
#include "SdUtil.h"

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
	TYPE DEFINITION
********************************************************************/

/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
/**
 * @brief Initializes the SdCommon module 
 Initializes the SdCommon module 
 * @remarks OS에 Dependent한 초기화를 실행한다. Initialize the OS dependent parts.
 * @param ePowerState
 * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCommon_Init(SdPowerState_k ePowerState);

/**
 * @brief SDAL에서 사용할 Semaphore 및 Shared Memory를 설정하고 초기화하는 함수
 The function which initializes the both Semaphore and Shared Memory to be used in SDAL
 * @remarks 
  * @return SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCommon_InitSharedResource(void);

/**
 * @brief SdCommon API의 Manual Debug 함수
 The Manual Debug function of SdCommon API
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdCommon_Debug(void);

#endif

