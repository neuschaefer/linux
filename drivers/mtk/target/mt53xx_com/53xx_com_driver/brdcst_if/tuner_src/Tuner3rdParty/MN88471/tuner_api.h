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
/*==========================================================================================*/
/*  Panasonic System LSI Design Europe, a Division of Panasonic Europe Ltd.					*/
/*                                                                                          */
/*  Module Name:            tuner_api.h														*/
/*  Author (Nationality):   Luke Grove (British)	                                        */
/*  Description:            Definifitions for tuner_api										*/
/*																						    */
/*  Date First Created:     1st January 2010                                                */
/*  Related Specifications (PSDE document refs):                                            */
/*  Master file stored in CVS at: $Achive: $ */
/*                                                                                          */
/*  Status (choose one):    (*) under development           ( ) internal release            */
/*                          ( ) external release            ( ) example only                */
/*  Confidentiality:        ( ) Strictly Confidential       (*) Confidential                */
/*  (choose one)            ( ) Internal Use Only (IUO)     ( ) Open                        */
/*                                                                                          */
/*  Note: This code should only be viewed or otherwise used by customers of Panasonic after */
/*  an appropriate Non-Disclosure Agreement and/or Licence Agreement has been signed by.    */
/*  both Panasonic and the customer.  If in doubt, please check with your Panasonic         */
/*  representative. If you have reason to believe this code is NOT subject to the above,    */
/*  please delete all copies of it and related files from your systems immediately.         */
/*                                                                                          */
/*  (c) 2010 Panasonic Europe Limited.														*/
/*==========================================================================================*/

/*==========================================================================================*/
/*  THIS PROGRAM IS EXAMPLE SOFTWARE PROVIDED TO CUSTOMERS OF PANASONIC IN THE HOPE THAT IT */
/*  WILL BE USEFUL, BUT WITHOUT ANY WARRANTY EXPRESSED OR IMPLIED.  THE PROGRAM IS PROVIDED */
/*  ON AN "AS IS" BASIS.  THE ENTIRE RISK AS TO THE PROGRAM QUALITY AND PERFORMANCE IS WITH */
/*  THE USER. IN NO EVENT WILL PANASONIC BE LIABLE FOR DIRECT,  INDIRECT,  OR INCIDENTAL OR */
/*  CONSEQUENTIAL DAMAGES RESULTING FROM ANY DEFECT IN THE PROGRAM,   EVEN IF PANASONIC HAS */
/*  BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.                                        */
/*                                                                                          */
/*  THIS PROGRAM  HAS BEEN DEVELOPED  OUTSIDE THE SCOPE OF  PANASONIC SEMICONDUCTOR COMPANY */
/*  DEVELOPMENT PROCEDURES,  AND IS PROVIDED ONLY AS EXAMPLE SOFTWARE.                      */
/*==========================================================================================*/

#ifndef _TUNER_API_H_
#define _TUNER_API_H_

//#include <math.h>

#ifdef __cplusplus
extern "C"
{
#endif


#include "dd_types.h"
#include "dd_tuner.h"
#include "tuner_api_data.h"
#include "demod_MN88471.h"

extern 	MNDemodInstance_t	 demod; 
//extern TUNERInstance_t          tunerPanasonicENV57U04D8F;

	typedef MNReg_set_t tu_reg_set_t;
    /*****************************************
    *
    *   Tuner API functions declarations:
    *
    *****************************************/
    tu_return_t tu_api_init ( TUNERInstance_t *tunerInstance );

    tu_return_t tu_api_shut_down ( void );
	tu_return_t tu_api_wake_up( void );

    tu_return_t tu_api_tune_request ( tu_chan_setup_t *setup_ptr );

    tu_rx_status_t tu_api_get_syn_status ( void );

    tu_return_t tu_api_set_output_mode ( tu_output_mode_t mode );

    tu_output_mode_t tu_api_get_output_mode ( void );
	
    tu_return_t tu_api_set_receiver_mode ( ReceiverMode_t type, Bandwidth_t bandwidth );

    tu_return_t tu_api_get_receiver_mode ( ReceiverMode_t *type );

    s32_t tu_api_get_AGC ( void );	tu_return_t tu_api_get_SSI ( u16_t RF_AGC_Voltage, SSI_t *SSI_params );
	TS_FMT_T tu_api_get_ts_fmt( void );

    s16_t tu_api_get_regval ( tu_reg_set_t regset, u8_t regAddr );

    s16_t tu_api_set_regval ( tu_reg_set_t regset, u8_t regAddr, u8_t regData );

    MNdouble_t tu_api_get_CN ( void );

    MNdouble_t tu_api_get_BER ( tu_ber_mode_t ber_state );

    tu_return_t tu_api_get_tune_parameters ( tu_chan_database_t *current_chan_database );

	tu_return_t tu_api_channel_info ( tu_chan_setup_t *channelInfo );

    tu_return_t tu_api_plp_info ( tu_chan_plp_info_t *plpInfo );
    tu_return_t tu_api_plp_set ( u8_t plp_num );
    tu_return_t tu_api_priority_set ( Priority_t priority );

    tu_return_t tu_api_search_request ( tu_search_setup_t *setup_ptr );

#ifdef __cplusplus
}
#endif
#endif  /* _TUNER_API_H_ */

/*============================================================================================
    Revision History

    Dlr_Log: tuner_api.h,v Dlr_
    Revision 1.1  2010/09/22 13:58:53  bells
    Initial check-in

    Revision 1.1  2010/09/16 17:30:36  bells
    Initial check-in to PSDE CVS repository

==============================================================================================*/

