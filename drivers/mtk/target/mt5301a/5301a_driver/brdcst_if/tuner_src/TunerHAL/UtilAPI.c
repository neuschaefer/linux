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

#include "PI_Def.h"
#include "drvapi_tuner.h"

/*
This function convert modulation type.
*/
S32 Util_TunerModSize(TUNER_MODULATION_T tuner_mod)
{
    S32 qam_size;

    switch (tuner_mod)
    {
        case MOD_QPSK:
            qam_size = 4;
            break;
        case MOD_VSB_8:
            qam_size = 8;
            break;
        case MOD_QAM_16:
            qam_size = 16;
            break;
        case MOD_QAM_32:
            qam_size = 32;
            break;
        case MOD_QAM_64:
            qam_size = 64;
            break;
        case MOD_QAM_128:
            qam_size = 128;
            break;
        case MOD_QAM_256:
            qam_size = 256;
            break;
         case MOD_UNKNOWN: /*auto QAM*/
            qam_size = 999;
            break;
        default:
            qam_size = 64;
            break;
    }
    return (qam_size);
}

INT32 Util_TranslateConnectInfo(DRV_CONN_TYPE_T  e_conn_type, 
                           VOID* pv_conn_info, SIZE_T z_conn_info_len,
                           U32* pFreq1, U32* pSymbolRate, 
                           TUNER_MODULATION_T* pe_mode,
                           BOOL* pSpecInv)
{                          
                   
    /* verify connect info */
    if (pv_conn_info == NULL)
    {
        mcSHOW_DRVERR_MSG(("Util_TranslateConnectInfo: RMR_DRV_INV_CONN_INFO!\n"));
        return (RMR_DRV_INV_CONN_INFO);
    }

    if (e_conn_type == TUNER_CONN_TYPE_TER_DIG)
    {
        TUNER_TER_DIG_TUNE_INFO_T *pt_tuner_info = 
                    (TUNER_TER_DIG_TUNE_INFO_T *) pv_conn_info;

        if (pt_tuner_info->ui4_freq > DRVAPI_TUNER_TER_FREQ_HIGH ||
            pt_tuner_info->ui4_freq < DRVAPI_TUNER_TER_FREQ_LOW ||
            ((pt_tuner_info->e_mod != MOD_VSB_8)
                && (pt_tuner_info->e_mod != MOD_QAM_256)
                && (pt_tuner_info->e_mod != MOD_QAM_64)
                && (pt_tuner_info->e_mod != MOD_UNKNOWN)
            )) /*MOD_UNKNOWN is autoQAM*/
        {
            mcSHOW_DRVERR_MSG(("Util_TranslateConnectInfo: RMR_DRV_INV_CONN_INFO!\n"));
            return (RMR_DRV_INV_CONN_INFO);
        }
        *pFreq1 = pt_tuner_info->ui4_freq;
        *pe_mode = pt_tuner_info->e_mod;
    }
    else if (e_conn_type == TUNER_CONN_TYPE_CAB_DIG)
    {
        TUNER_CAB_DIG_TUNE_INFO_T *pt_tuner_info = (
                    TUNER_CAB_DIG_TUNE_INFO_T *) pv_conn_info;

        if (pt_tuner_info->ui4_freq > DRVAPI_TUNER_CABLE_FREQ_HIGH ||
            pt_tuner_info->ui4_freq < DRVAPI_TUNER_CABLE_FREQ_LOW ||
            // Mark this to enable VSB mode in CAB_DIG
            //pt_tuner_info->ui4_sym_rate > DRVAPI_TUNER_CABLE_BAUD_HIGH ||
            //pt_tuner_info->ui4_sym_rate < DRVAPI_TUNER_CABLE_BAUD_LOW ||
            ((pt_tuner_info->e_mod != MOD_QAM_256) &&
             (pt_tuner_info->e_mod != MOD_QAM_128) &&
             (pt_tuner_info->e_mod != MOD_QAM_64) &&
             (pt_tuner_info->e_mod != MOD_QAM_32) &&
             (pt_tuner_info->e_mod != MOD_QAM_16) &&
             (pt_tuner_info->e_mod != MOD_QAM_4_NR) &&
             (pt_tuner_info->e_mod != MOD_UNKNOWN) &&
             (pt_tuner_info->e_mod != MOD_VSB_8))) /*MOD_UNKNOWN is autoQAM*/
        {
            mcSHOW_DRVERR_MSG(("Util_TranslateConnectInfo: RMR_DRV_INV_CONN_INFO!\n"));
            return (RMR_DRV_INV_CONN_INFO);
        }
        *pFreq1 = pt_tuner_info->ui4_freq;
        *pSymbolRate = pt_tuner_info->ui4_sym_rate;
        *pe_mode = pt_tuner_info->e_mod;
    }
    else if (e_conn_type == TUNER_CONN_TYPE_CAB_DIG_OOB_RX)
    {
        TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T *pt_tuner_info = 
                    (TUNER_CAB_DIG_OOB_RX_TUNE_INFO_T *) pv_conn_info;

        if (pt_tuner_info->ui4_freq > DRVAPI_TUNER_OOB_RX_FREQ_HIGH ||
            pt_tuner_info->ui4_freq < DRVAPI_TUNER_OOB_RX_FREQ_LOW ||
            pt_tuner_info->ui4_data_rate > DRVAPI_TUNER_OOB_RX_DATARATE_HIGH ||
            pt_tuner_info->ui4_data_rate < DRVAPI_TUNER_OOB_RX_DATARATE_LOW ||
            pt_tuner_info->e_mod != MOD_QPSK)
        {
            mcSHOW_DRVERR_MSG(("Util_TranslateConnectInfo RMR_DRV_INV_CONN_INFO!\n"));
            return (RMR_DRV_INV_CONN_INFO);
        }
        *pFreq1 = pt_tuner_info->ui4_freq;
        *pSymbolRate = pt_tuner_info->ui4_data_rate/2;
        *pe_mode = pt_tuner_info->e_mod;
        *pSpecInv = pt_tuner_info->b_spectrum_inv;
        mcSHOW_DRVAPI_MSG(("b_spectrum_inv = %d\n", *pSpecInv ));
    }
    else if (e_conn_type == TUNER_CONN_TYPE_TER_ANA)
    {
        TUNER_TER_ANA_TUNE_INFO_T *pt_tuner_info = 
                    (TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info;

        *pFreq1 = pt_tuner_info->ui4_freq;
    }
    else if (e_conn_type == TUNER_CONN_TYPE_CAB_ANA)
    {
        TUNER_CAB_ANA_TUNE_INFO_T *pt_tuner_info = 
                    (TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info;

        *pFreq1 = pt_tuner_info->ui4_freq;
    }
#ifdef TUNER_SUPPORT_SCART_OUT
    else if (e_conn_type == TUNER_CONN_TYPE_TER_ANA_SCART_OUT)
    {
        TUNER_TER_ANA_TUNE_INFO_T *pt_tuner_info = 
                    (TUNER_TER_ANA_TUNE_INFO_T *) pv_conn_info;

        *pFreq1 = pt_tuner_info->ui4_freq;
    }
    else if (e_conn_type == TUNER_CONN_TYPE_CAB_ANA_SCART_OUT)
    {
        TUNER_CAB_ANA_TUNE_INFO_T *pt_tuner_info = 
                    (TUNER_CAB_ANA_TUNE_INFO_T *) pv_conn_info;

        *pFreq1 = pt_tuner_info->ui4_freq;
    }
#endif
    return (RMR_OK);
}


INT32 Util_InterlockedIncrement(SEMA_T RefSema, long* pRefCnt)
{
    mcSEMA_LOCK(RefSema);
    *pRefCnt += 1;
    mcSEMA_UNLOCK(RefSema);
    return (*pRefCnt);
}

INT32 Util_InterlockedDecrement(SEMA_T RefSema, long* pRefCnt)
{
    mcSEMA_LOCK(RefSema);
    *pRefCnt -= 1;
    mcSEMA_UNLOCK(RefSema);
    return (*pRefCnt);
}

/***********************************************************************/
char GetChar(void)
{
#ifndef CC_LINUX_KERNEL
// linux platfrom driver should not call MW API cli_get_char
char cCh0;

    if ((cCh0 = mcGETCHAR_CLI()) == 0)
#endif
        return mcGETCHAR_SER();
#ifndef CC_LINUX_KERNEL
    return cCh0;
#endif
}
