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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: smc_bcas.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file smc_bcas.c
 *  smc_bcas.c is the implementation of BCAS SmartCard command.
 */

#include "x_os.h"
#include "smc_debug.h"
#include "smc_drvif.h"
#include "smc_apdu.h"
#include "smc_bcas.h"

static UINT8 _au1CardID[6];

static INT32 common_error_sw1sw2 (UINT8 sw1, UINT8 sw2, UINT8 smc_slot_nbr)
{
    INT32       ret_val = SMCR_OK;
    SMC_BCAS_EVENT_T card_event;
    UINT16 error_value     = 0;
    UNUSED(card_event);        
    error_value = (sw1 << 8) | sw2;
//    card_event.smc_slot_nbr = smc_slot_nbr;
    switch (error_value) {
        case 0x6400:
            card_event.card_error = 0x6400;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            ret_val = SMCR_FAIL;
            break;

        case 0x6581:
            card_event.card_error = 0x6581;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            ret_val = SMCR_FAIL;
            break;
    }

	return ret_val;
}

static INT32 get_smc_card_id (UINT8 smc_slot_nbr, UINT8 *card_id)
{
       x_memcpy(card_id, _au1CardID, 6);
	return SMCR_OK;
}


INT32 _SMC_BCAS_InitialSettings(SMC_BCAS_CARD_INFO_T* prCardInfo)
{
    UINT8 au1ResponseData[258];
    UINT8 u1ResponseLength;

    INT32 i4Ret = SMCR_OK;

    UINT8 au1BlockData[5];
    
    au1BlockData[0] = CLA;
    /* ------------------- Filling the Instruction ---------------------- */
    au1BlockData[1] = INS_SET;
    au1BlockData[2] = 0x00;
    au1BlockData[3] = 0x00;
    au1BlockData[4] = 0;

    if((i4Ret = _SMC_SendReceiveAPDU(
    	0, 
    	au1BlockData, 
    	5, 
    	au1ResponseData, 
    	&u1ResponseLength)) != SMCR_OK)
    { 
        return i4Ret;
    }
    
    x_memcpy(_au1CardID, &au1ResponseData[2], 6);

    if(prCardInfo != NULL)
    {
        x_memcpy(prCardInfo->card_id, &au1ResponseData[2], 6);
        //prCardInfo->card_type = au1ResponseData[8];
        x_memcpy(prCardInfo->system_key, &au1ResponseData[10], 32);
        x_memcpy(prCardInfo->cbc_initial_vector, &au1ResponseData[42], 8);    
    }   
    
    return i4Ret;
}

INT32 _SMC_BCAS_GetCardId(UINT8* pu1CardId)
{
    UINT8 au1ResponseData[258];
    UINT8 u1ResponseLength;

    UINT8 au1BlockData[5];
    
    au1BlockData[0] = CLA;
    /* ------------------- Filling the Instruction ---------------------- */
    au1BlockData[1] = INS_IDI;
    au1BlockData[2] = 0x00;
    au1BlockData[3] = 0x00;
    au1BlockData[4] = 0;

    if(_SMC_SendReceiveAPDU(
    	0, 
    	au1BlockData, 
    	5, 
    	au1ResponseData, 
    	&u1ResponseLength) != SMCR_OK)
    {
        return SMCR_FAIL;
    }
    
    x_memcpy(pu1CardId, &au1ResponseData[1], 10);
    
    return SMCR_OK;
}

INT32 _SMC_BCAS_SendECM (UINT8	smc_slot_nbr, UINT8 *ecm_data, UINT16 length,
                                                        SMC_BCAS_SCRAMBLE_KEYS_T *keys)
{    
    INT32	ret_val = SMCR_OK;
    UINT8		    response_data[256];
    UINT8		    response_length;
    SMC_BCAS_EVENT_T card_event;

    UINT8 au1BlockData[258];

    if(length >= 253)
    {
        ret_val = SMCR_INV_ARG;
        goto ret;
    }
    
    au1BlockData[0] = CLA;
    /* ------------------- Filling the Instruction ---------------------- */
    au1BlockData[1] = INS_ECM;
    au1BlockData[2] = 0x00;
    au1BlockData[3] = 0x00;
    au1BlockData[4] = length;
    x_memcpy(&au1BlockData[5], ecm_data, length);
    au1BlockData[length + 5] = 0;
    
    UNUSED(card_event);        
    /* -- Sending the ECM to the smartcard and getting the keys in return -- */

    //ret_val =  check_smc_slot_number (smc_slot_nbr);
    if (ret_val == SMCR_OK) {  	
        ret_val = _SMC_SendReceiveAPDU (
        	0,
        	au1BlockData, 
        	length + 6, 
              response_data, 
              &response_length);     
    }

    if (ret_val == SMCR_OK) {
        ret_val = common_error_sw1sw2 (response_data[response_length - 2],
                                response_data[response_length - 1],0);
        if (ret_val != SMCR_OK) {
            goto ret;
        }
        card_event.smc_slot_nbr = 0;
        switch ((response_data[4]<<8) | response_data[5]) {
	    case 0xA102:
            card_event.card_error = 0xA102;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            Printf("[SMC] card inactive \n");
		    ret_val = SMCR_NO_CARD; //inactive
		    break;
	    case 0xA103:
            card_event.card_error = 0xA103;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_NON_CONTRACT,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            Printf ("[SMC] non contract \n");
		    ret_val = SMCR_CARD_READ_ERROR; // NON_CONTRACT;
		    break;
        /*-----------------------------------------------------------------*/
	    case 0x8901:
            card_event.card_error = 0x8901;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            Printf( "[SMC] non contract \n");
		    ret_val = SMCR_CARD_READ_ERROR; // NON_CONTRACT;
		    break;
	    case 0x8501:
            card_event.card_error = 0x8501;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            Printf( "[SMC] non contract \n");
		    ret_val = SMCR_CARD_READ_ERROR; // NON_CONTRACT;
		    break;
	    case 0x8301:
            card_event.card_error = 0x8301;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            Printf( "[SMC] non contract \n");
		    ret_val = SMCR_CARD_READ_ERROR; // NON_CONTRACT;
		    break;
        /*-----------------------------------------------------------------*/
	    case 0x8902:
            card_event.card_error = 0x8902;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            Printf( "[SMC] non contract \n");
		    ret_val = SMCR_CARD_READ_ERROR; // NON_CONTRACT;
		    break;
	    case 0x8502:
            card_event.card_error = 0x8502;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            Printf( "[SMC] non contract \n");
		    ret_val = SMCR_CARD_READ_ERROR; // NON_CONTRACT;
		    break;
	    case 0x8302:
            card_event.card_error = 0x8302;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            Printf( "[SMC] non contract \n");
		    ret_val = SMCR_CARD_READ_ERROR; // NON_CONTRACT;
		    break;
        /*-----------------------------------------------------------------*/
	    case 0x8903:
            card_event.card_error = 0x8903;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            Printf( "[SMC] non contract \n");
		    ret_val = SMCR_CARD_READ_ERROR; // NON_CONTRACT;
		    break;
	    case 0x8503:
            card_event.card_error = 0x8503;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            Printf( "[SMC] non contract \n");
		    ret_val = SMCR_CARD_READ_ERROR; // NON_CONTRACT;
		    break;
	    case 0x8303:
            card_event.card_error = 0x8303;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            Printf( "[SMC] non contract \n");
		    ret_val = SMCR_CARD_READ_ERROR; // NON_CONTRACT;
		    break;
        /*-----------------------------------------------------------------*/
	    case 0x0800:
            card_event.card_error = 0x0800;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
		    if (response_length < 19) {
			    ret_val = SMCR_CARD_READ_ERROR;
			    goto ret;
		    }

            /* -------------- Keys are obtained from Smartcard -------------- */
		    x_memcpy (keys->odd_key, &response_data[0], 8);
		    x_memcpy (keys->even_key, &response_data[8],
                                                           8);
		    keys->smc_slot_nbr = 0;

            /* ----- Getting the card id for the smartcard slot number ------ */
            ret_val = get_smc_card_id (0, keys->card_id);

            /* -Following members have to be filled from the calling funtion -*/
		    keys->ecm_pid = 0x1fff;
		    keys->session_id = 0;
		    ret_val = SMCR_OK;
//            bcas_state_machine = BCAS_ECM_KEY_GENERATED;
		    break;
        /*-----------------------------------------------------------------*/
	    case 0xA106:
            card_event.card_error = 0xA106;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            Printf ("[SMC] ECM manipulatiopn error \n");
//            bcas_state_machine = BCAS_ECM_KEY_GENERATED_ERROR;
		    ret_val = SMCR_CARD_ERROR;//SECUIRITY_ERROR;
		    break;
	    case 0xA1FF:
            card_event.card_error = 0xA1FF;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            Printf ("[SMC] ECM manipulatiopn error \n");
//            bcas_state_machine = BCAS_ECM_KEY_GENERATED_ERROR;
		    ret_val = SMCR_CARD_ERROR; //SECUIRITY_ERROR;
		    break;
	    case 0xA1FE:
            card_event.card_error = 0xA1FE;
//            ret_val = post_data_to_event_handler (SMARTCARD, BCAS_CARD_ERROR,
//                                    SYS_CB_SESSION_ID, (void *) &card_event);
            Printf ("[SMC] ECM manipulatiopn error \n");
//            bcas_state_machine = BCAS_ECM_KEY_GENERATED_ERROR;
		    ret_val = SMCR_CARD_ERROR; //SECUIRITY_ERROR;
		    break;

        default:
		    break;
	    }
    }
    else {
        Printf( "[SMC] smartcard sedning ecm failed \n");
        goto ret;
    }
ret:

	return ret_val;

}

