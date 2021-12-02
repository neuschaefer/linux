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
/*
Description: This is tuner driver API source file.
*/

#ifndef _X_DRVAPI_TUNER_C_
#define _X_DRVAPI_TUNER_C_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include "drvapi_tuner.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    variables definitions
 ----------------------------------------------------------------------------*/

static DRV_COMP_FCT_TBL_T t_tuner_fct_tbl =
{
    TunerConnect,
    TunerDisconnect,
    TunerGet,
    TunerSet
};

static BOOL             b_tuner_init = (FALSE);
static DRV_COMP_REG_T    t_tuner_comp_id;
static UINT32           ui4_tuner_comp_tag;
static x_rm_nfy_fct     pf_tuner_nfy;
static VOID*            pv_tuner_nfy_tag;

/*-----------------------------------------------------------------------------
                    functions definitions
 ----------------------------------------------------------------------------*/



/*
This function implements component connect function.
*/
static INT32 TunerConnect(DRV_COMP_ID_T*   pt_comp_id,
                          DRV_CONN_TYPE_T  e_conn_type,
                          const VOID*      pv_conn_info,
                          SIZE_T           z_conn_info_len,
                          VOID*            pv_nfy_tag,
                          x_rm_nfy_fct     pf_nfy)
{
    TUNER_CAB_DIG_TUNE_INFO_T * pt_tuner_info;
            
    /* verify the request */
    if (pt_comp_id->e_type != DRVT_TUNER_CAB_DIG ||
        pt_comp_id->ui2_id != DRVAPI_TUNER_DVBC_29708_ID)//Ares
    {
        DBG_PRINT("\nTunerConnect: RMR_DRV_INV_CONN_INFO!\n");
        return (RMR_DRV_INV_CONN_INFO);
    }

    /* update notify function */
    if (pf_nfy == NULL)
    {
        DBG_PRINT("\nTunerConnect: RMR_DRV_INV_CONN_INFO!\n");
        return (RMR_DRV_INV_CONN_INFO);
    }
    pf_tuner_nfy = pf_nfy;
    pv_tuner_nfy_tag = pv_nfy_tag;
    
    /* verify connect info */
    if (pv_conn_info == NULL)
    {
        DBG_PRINT("\nTunerConnect: RMR_DRV_INV_CONN_INFO!\n");
        return (RMR_DRV_INV_CONN_INFO);
    }
    pt_tuner_info = (TUNER_CAB_DIG_TUNE_INFO_T *)pv_conn_info;
    if (pt_tuner_info->ui4_freq > DRVAPI_TUNER_CABLE_FREQ_HIGH ||
        pt_tuner_info->ui4_freq < DRVAPI_TUNER_CABLE_FREQ_LOW ||
        pt_tuner_info->ui4_sym_rate > DRVAPI_TUNER_CABLE_BAUD_HIGH ||
        pt_tuner_info->ui4_sym_rate < DRVAPI_TUNER_CABLE_BAUD_LOW ||
        pt_tuner_info->e_mod > MOD_QAM_256 ||
        pt_tuner_info->e_mod < MOD_QAM_16 ||
        pt_tuner_info->e_mod == MOD_QAM_80 ||
        pt_tuner_info->e_mod == MOD_QAM_96 ||
        pt_tuner_info->e_mod == MOD_QAM_112 ||
        pt_tuner_info->e_mod == MOD_QAM_160 ||
        pt_tuner_info->e_mod == MOD_QAM_192 ||
        pt_tuner_info->e_mod == MOD_QAM_224 ||
        pt_tuner_info->e_fec_outer != FEC_O_RS_204_188 ||
        pt_tuner_info->e_fec_inner != FEC_I_NO_CODING)
    {
        DBG_PRINT("\nTunerConnect: RMR_DRV_INV_CONN_INFO!\n");
        return (RMR_DRV_INV_CONN_INFO);
    }


    return (RMR_OK);
}

/*
This function implements component disconnect function.
*/
static INT32 TunerDisconnect(DRV_COMP_ID_T*   pt_comp_id,
                             DRV_DISC_TYPE_T  e_disc_type,
                             const VOID*      pv_disc_info,
                             SIZE_T           z_disc_info_len)
{
    /* verify the request */
    if (pt_comp_id->e_type != DRVT_TUNER_CAB_DIG ||
        pt_comp_id->ui2_id != DRVAPI_TUNER_DVBC_29708_ID)//Ares
    {
        DBG_PRINT("\nTunerDisconnect: RMR_DRV_INV_DISC_INFO!\n");
        return (RMR_DRV_INV_DISC_INFO);
    }

    DBG_PRINT("\nTunerDisconnect: RMR_OK!\n");
    return (RMR_OK);
}

/*
This function implements component get function.
*/
static INT32 TunerGet(DRV_COMP_ID_T*  pt_comp_id,
                      DRV_GET_TYPE_T  e_get_type,
                      VOID*           pv_get_info,
                      SIZE_T*         pz_get_info_len)
{
    /* verify the request */
    if (pt_comp_id->e_type != DRVT_TUNER_CAB_DIG ||
        pt_comp_id->ui2_id != DRVAPI_TUNER_DVBC_29708_ID)//Ares
    {
        DBG_PRINT("\nTunerGet: RMR_DRV_INV_GET_INFO!\n");
        return (RMR_DRV_INV_GET_INFO);
    }
    
    /* supported types */
    switch (e_get_type)
    {
        case TUNER_GET_TYPE_CAB_DIG:
        {
            break;
        }
        case TUNER_GET_TYPE_SIGNAL_LEVEL:
        {           
            break;
        }
        case TUNER_GET_TYPE_STATE:
        {  
            break;
        }
        default:
        {
            DBG_PRINT("\nTunerGet: RMR_DRV_INV_GET_INFO!\n");
            return (RMR_DRV_INV_GET_INFO);
            break;
        }
    }
    
    DBG_PRINT("\nTunerGet: RMR_OK!\n");
    return (RMR_OK);
}

/*
This function implements component set function.
*/
static INT32 TunerSet(DRV_COMP_ID_T*  pt_comp_id,
                      DRV_SET_TYPE_T  e_set_type,
                      const VOID*           pv_set_info,
                      SIZE_T          z_set_info_len)
{
    /* verify the request */
    if (pt_comp_id->e_type != DRVT_TUNER_CAB_DIG ||
        pt_comp_id->ui2_id != DRVAPI_TUNER_DVBC_29708_ID)//Ares
    {
        DBG_PRINT("\nTunerSet: RMR_DRV_INV_SET_INFO!\n");
        return (RMR_DRV_INV_SET_INFO);
    }

    /* there is nothing to be set with digital cable tuner */
    DBG_PRINT("\nTunerSet: RMR_DRV_INV_SET_INFO!\n");
    return (RMR_DRV_INV_SET_INFO);
}


/*
This function initializes tuner hardware, create the tuner
state machine thread, and registers to resource manager.
*/
INT32 TunerInit(void)
{
    INT32                res;

    /* multiple inits are not allowed */
    if (b_tuner_init)
    {
        DBG_PRINT("\nTunerInit: DRVAPI_TUNER_ALREDAY_INIT!\n");
        return (DRVAPI_TUNER_ALREDAY_INIT);
    }
    b_tuner_init = (TRUE);

    /* register the tuner to resource manager */
    t_tuner_comp_id.e_type = DRVT_TUNER_CAB_DIG;
    t_tuner_comp_id.e_id_type = ID_TYPE_IND;
    t_tuner_comp_id.u.t_ind.ui2_id = DRVAPI_TUNER_DVBC_29708_ID;//Ares
    t_tuner_comp_id.u.t_ind.pv_tag = (void *)0;
    t_tuner_comp_id.u.t_ind.ui1_port = 0;
    ui4_tuner_comp_tag = DRVAPI_TUNER_COMP_TAG;
    
    res = x_rm_reg_comp (&t_tuner_comp_id, 1, 1,
                         "Comtech_DVBC29708",
                         0,
                         &t_tuner_fct_tbl,
                         (const VOID*)NULL,
                         (SIZE_T)0);
    if (res != RMR_OK)
    {
        DBG_PRINT("\nTunerInit: DRVAPI_TUNER_ERROR!\n");
        return (DRVAPI_TUNER_ERROR);
    }

    /* everything is Ok and quit */
    DBG_PRINT("\nTunerInit: DRVAPI_TUNER_OK!\n");
    return (DRVAPI_TUNER_OK);
}

#endif /* _X_DRVAPI_TUNER_C_ */
