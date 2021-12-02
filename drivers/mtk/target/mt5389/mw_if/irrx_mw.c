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
 * $RCSfile: irrx_mw.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This is IRRC driver middleware adaptation API source file
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    Include files
 ----------------------------------------------------------------------------*/
#include "x_os.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_irrc_btn_def.h"
#include "x_irrc.h"

#include "ir_if.h"

#include "x_lint.h"
#include "x_assert.h"
#include "x_debug.h"
#include "x_printf.h"
#include "x_pdwnc.h"

#include "drvcust_if.h"

#ifdef LINUX_TURNKEY_SOLUTION
#include "cb_data_rm.h"
#include "cb_param.h"
#endif
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)  
#include "aud_if.h"
#endif
/*-----------------------------------------------------------------------------
                    Macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define DRV_IRRC_ID             0x1234

/*-----------------------------------------------------------------------------
                    Variable definitions
 ----------------------------------------------------------------------------*/
static CHAR*                pc_irrc_comp_name = SN_IRRC;
static UINT32               ui4_irrc_comp_tag = 0x1234;
static DRV_COMP_REG_T       t_irrc_comp_reg;
static DRV_COMP_FCT_TBL_T   t_irrc_comp_fct_tbl;
static VOID                 *pv_irrc_nfy_tag;
static x_irrc_nfy_fct       pf_irrc_nfy_fct = NULL;
#ifndef LINUX_TURNKEY_SOLUTION
static x_irrc_nfy_fct       pf_irrc_usr_nfy_fct = NULL;
#endif
#ifdef LINUX_TURNKEY_SOLUTION
static PARAM_IRRC_NFY_INFO_T param_irrc;
extern INT32 _CB_PutEvent_Adpt(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

static VOID irrc_nfy_fct (VOID*             pv_nfy_tag,
                          IRRC_COND_T       e_nfy_cond,
                          UINT32            ui4_evt_code,
                          IRRX_RAW_DATA_T*  prRaw
                         /*,UINT32            ui4_data*/)
{
    param_irrc.pv_nfy_tag = pv_nfy_tag;
    param_irrc.e_nfy_cond = e_nfy_cond;
    param_irrc.ui4_evt_code = ui4_evt_code;
#if 0
    lock_nfy_param_buf(PARAM_IRRX_NFY);

    param_irrc.pv_data = get_nfy_param_buf(PARAM_IRRX_NFY, sizeof(IRRX_RAW_DATA_T));
    rm_copy_to_user(param_irrc.pv_data, prRaw, sizeof(IRRX_RAW_DATA_T));
    unlock_nfy_param_buf(PARAM_IRRX_NFY);
#else
    param_irrc.t_data = *((RAW_DATA_T*)prRaw);
#endif
    _CB_PutEvent_Adpt(CB_DRV_IRRX, sizeof(PARAM_IRRC_NFY_INFO_T), &param_irrc);
}
#endif

/*-----------------------------------------------------------------------------
                    Prototypes
 ----------------------------------------------------------------------------*/
extern void _MW_PCDCheckInit(void);


/*-----------------------------------------------------------------------------
                    Function definitions
 ----------------------------------------------------------------------------*/
static INT32 IRRC_Get(DRV_COMP_ID_T*   pt_comp_id,
               DRV_GET_TYPE_T   e_get_type,
               VOID*            pv_get_info,
               SIZE_T*          pz_get_info_len);

static INT32 IRRC_Set(DRV_COMP_ID_T*   pt_comp_id,
               DRV_SET_TYPE_T   e_set_type,
               const VOID*      pv_set_info,
               SIZE_T           z_set_info_len);
#ifndef LINUX_TURNKEY_SOLUTION
static void MW_IRRC_NotifyFunc(UINT32 u4BtnState, UINT32 u4Key, IRRX_RAW_DATA_T *prRaw);
#endif
INT32 MW_IRRC_Init(void);

INT32 d_custom_irrx_system_code_filter(BOOL fgEnable, BOOL fgPositiveFilter, UINT32 u4Num, UINT32 au4FilterArray[]);
//lint -save -e818
/*-----------------------------------------------------------------------------
                    IRRC get function
 ----------------------------------------------------------------------------*/
static INT32 IRRC_Get(DRV_COMP_ID_T*   pt_comp_id,
               DRV_GET_TYPE_T   e_get_type,
               VOID*            pv_get_info,
               SIZE_T*          pz_get_info_len)
{
    SIZE_T      t_get_info_len;

    /* Check component type and id */
    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_GET_INFO);
    }

    if ((pt_comp_id->e_type != DRVT_IRRC) ||
        (pt_comp_id->ui2_id != DRV_IRRC_ID))
    {
       LOG(1, "IRRC_Get(): component type or id error\n");
       return (RMR_DRV_INV_SET_INFO);
    }

    /* Perform get opertion */
    switch (e_get_type)
    {
        case IRRC_GET_TYPE_GROUP_ID:
            t_get_info_len = sizeof(UINT32);
            ASSERT(pz_get_info_len!=NULL);
            if (*pz_get_info_len < t_get_info_len)
            {
                LOG(1, "IRRC_Get(): space is not enough\n");
                return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            *pz_get_info_len = t_get_info_len;
            ASSERT(pv_get_info!=NULL);
            *((UINT32 *)pv_get_info) = 0; // IRRX_QuerySetGroupId(NULL);
            break;

        case IRRC_GET_TYPE_REPETITION_TIME:
            t_get_info_len = sizeof(UINT16);
            ASSERT(pz_get_info_len!=NULL);
            if (*pz_get_info_len < t_get_info_len)
            {
                LOG(1, "IRRC_Get(): space is not enough\n");
                return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            *pz_get_info_len = t_get_info_len;
            ASSERT(pv_get_info!=NULL);
            *(UINT16*)pv_get_info = (UINT16)IRRX_QuerySetRepeatTime(NULL);
            /* *(UINT16*)pv_get_info = 0; */ /* Repetition time is unknown */
            break;

        case IRRC_GET_TYPE_RAW_DATA_TO_EVT_CODE:
            {
            	  IRRC_RAW_DATA_TO_EVT_CODE_T* pt_get_evt;
                t_get_info_len = sizeof(IRRC_RAW_DATA_TO_EVT_CODE_T);
                ASSERT(pz_get_info_len!=NULL);
                if (*pz_get_info_len < t_get_info_len)
                {
                    LOG(1, "IRRC_Get(): space is not enough\n");
                    return (RMR_DRV_NOT_ENOUGH_SPACE);
                }
                *pz_get_info_len = t_get_info_len;
                ASSERT(pv_get_info!=NULL);
                pt_get_evt = (IRRC_RAW_DATA_TO_EVT_CODE_T*)pv_get_info;
                pt_get_evt->ui4_evt_code = IRRX_RawToCrystal(pt_get_evt->ui1_sys_code, pt_get_evt->ui1_cmd_code);
            }
            break;

        case IRRC_GET_TYPE_RAW_BITS_TO_EVT_CODE:
            {
            	  IRRC_RAW_BITS_TO_EVT_CODE_T* pt_get_evt;
                t_get_info_len = sizeof(IRRC_RAW_BITS_TO_EVT_CODE_T);
                ASSERT(pz_get_info_len!=NULL);
                if (*pz_get_info_len < t_get_info_len)
                {
                    LOG(1, "IRRC_Get(): space is not enough\n");
                    return (RMR_DRV_NOT_ENOUGH_SPACE);
                }
                *pz_get_info_len = t_get_info_len;
                ASSERT(pv_get_info!=NULL);
                pt_get_evt = (IRRC_RAW_BITS_TO_EVT_CODE_T*)pv_get_info;
                pt_get_evt->ui4_evt_code = IRRX_RawBitsToCrystal(pt_get_evt->ui4_raw_bits);
            }
            break;
            
        default:
            LOG(1, "IRRC_Get(): invalid get_type\n");
            return (RMR_DRV_INV_GET_INFO);
    }

    return (RMR_OK);
}


/*-----------------------------------------------------------------------------
                    IRRC set function
 ----------------------------------------------------------------------------*/
static INT32 IRRC_Set(DRV_COMP_ID_T*   pt_comp_id,
               DRV_SET_TYPE_T   e_set_type,
               const VOID*      pv_set_info,
               SIZE_T           z_set_info_len)
{
    IRRC_NFY_INFO_T     *pt_nfy_info;

    /* Check component type and id */
    if (pt_comp_id == NULL) {
        return (RMR_DRV_INV_SET_INFO);
    }

    if ((pt_comp_id->e_type != DRVT_IRRC) ||
        (pt_comp_id->ui2_id != DRV_IRRC_ID))
    {
       LOG(1, "IRRC_Set(): component type or id error\n");
       return (RMR_DRV_INV_SET_INFO);
    }

    /* Check set_info */
    if ((pv_set_info == NULL) &&
        (e_set_type != IRRC_SET_TYPE_RESET) &&
        (e_set_type != IRRC_SET_TYPE_POWER_DOWN))
    {
        LOG(1, "IRRC_Set(): invalid set_info\n");
        return (RMR_DRV_INV_SET_INFO);
    }

    /* Perform set opertion */
    switch (e_set_type)
    {
        case IRRC_SET_TYPE_GROUP_ID:
            /* obsolete */
            break;

        case IRRC_SET_TYPE_RESET: /* Reset IRRC component immediately */
            LINT_SUPPRESS_NEXT_EXPRESSION(534);
            IRRX_ResetMtkIr();
            break;

        case IRRC_SET_TYPE_NFY_FCT:
            if (!pv_set_info)
            {
                LOG(1, "IRRC_Set(): notify function is null\n");
                return (RMR_DRV_INV_SET_INFO);
            }

            pt_nfy_info = (IRRC_NFY_INFO_T*)pv_set_info;
            pv_irrc_nfy_tag = pt_nfy_info->pv_tag;
        #ifndef LINUX_TURNKEY_SOLUTION
            pf_irrc_nfy_fct = pt_nfy_info->pf_irrc_nfy;
        #else
            pf_irrc_nfy_fct = (x_irrc_nfy_fct)irrc_nfy_fct;
            param_irrc.pf_nfy = pt_nfy_info->pf_irrc_nfy;
        #endif            
            break;

        case IRRC_SET_TYPE_POWER_DOWN:
            /* obsolete */
            break;

        case IRRC_SET_TYPE_SYS_CODE_FILTER:
        {
            INT32 i4_ret;
            IRRC_SYS_CODE_FILTER_T* pt_sys_code_filter = (IRRC_SYS_CODE_FILTER_T*) pv_set_info;

            i4_ret = d_custom_irrx_system_code_filter(pt_sys_code_filter->b_enable,
                                                      pt_sys_code_filter->b_positive_filter,
                                                      pt_sys_code_filter->z_filter_arr_size,
                                                      pt_sys_code_filter->pui4_filter_arr);
            if (i4_ret != IR_SUCC)
            {
                LOG(1, "iIRRC_Set(IRRC_SET_TYPE_SYS_CODE_FILTER): Failed return %d\n", i4_ret);
            }
            break;
        }

        case IRRC_SET_TYPE_PROTOCOL:
            {
            	  UINT32 u4Protocol;

            	u4Protocol = *(IRRC_PROTOCOL_T*)pv_set_info;
                IRRX_StopMtkIr();            
                if(u4Protocol == IRRC_PROTOCOL_FUNAI_FACTORY)
                {
                    LOG(0, "IRRC_SET_TYPE_PROTOCOL: IRRC_PROTOCOL_FUNAI_FACTORY\n");		                
                    IRRX_InitMtkIr(IRRX_FUNAI_FACTORY_TYPE);                                      
                }
                else if(u4Protocol == IRRC_PROTOCOL_RC5 || u4Protocol == IRRC_PROTOCOL_RC6)
                {
					LOG(0, "IRRC_SET_TYPE_PROTOCOL: IRRC_PROTOCOL_RC56\n");
					IRRX_InitMtkIr(IRRX_RC5_TYPE);                       
                }
                else if(u4Protocol == IRRC_PROTOCOL_NEC)
                {
                    LOG(0, "IRRC_SET_TYPE_PROTOCOL: IRRC_PROTOCOL_(USR:NEC+Matsushita)\n"); 									
					IRRX_InitMtkIr(IRRX_USR_TYPE);                      
                }                
            }
            break;

        case IRRC_SET_TYPE_LED_DRIVER_CONTROL:
            {
                UINT32 u4DriverLedCtrl;
                u4DriverLedCtrl  = *(IRRC_DRIVER_LED_CTRL_T*)pv_set_info;
                if(u4DriverLedCtrl == IRRC_LED_DRIVER_CTRL_OFF)
                {
                    IRRC_LedBlinkConfig(0);
                }
                else
                {
                    IRRC_LedBlinkConfig(1);
                }
            }
            break;
        default:
            LOG(1, "IRRC_Set(): nothing has been done\n");
            return (RMR_DRV_INV_SET_INFO);
    }

    return (RMR_OK);
}
//lint -restore
#if 0
static void _MW_IRRC_PowerOffHandler(UINT32 u4BtnState, UINT32 u4Key, UINT32 u4Raw)
{
    if((u4BtnState == IRRX_COND_BTN_UP) && (u4Key == BTN_POWER))
    {
        UINT32 u4Val;
        u4Val = u4IO32Read4B(PDWNC_WAKEN);//PDWNC_READ32(REG_RW_PDWAKE_EN);
        _MW_PCDCheckInit();
        PDWNC_EnterPowerDown(u4Val | WAK_IRRX, PDWNC_POWER_DOWN_IRRC);
    }
}
#endif
#ifndef LINUX_TURNKEY_SOLUTION /* !KEY_FROM_DFB */

static void MW_IRRC_NotifyFunc(UINT32 u4BtnState, UINT32 u4Key, IRRX_RAW_DATA_T* prRaw)
{


    RAW_DATA_T rRawData;    
    
    rRawData.e_protocol = (IRRC_PROTOCOL_T)(UINT32)prRaw->u1Protocol;
    rRawData.z_data_len = (SIZE_T)prRaw->u4Len;        
    x_memcpy(rRawData.aui1_data, prRaw->au1Data, prRaw->u4Len);
    
    if (pf_irrc_usr_nfy_fct != NULL)
    {
        pf_irrc_usr_nfy_fct(pv_irrc_nfy_tag, (IRRC_COND_T)u4BtnState, u4Key, &rRawData);
    }
    
    if (pf_irrc_nfy_fct != NULL)
    {
        pf_irrc_nfy_fct(pv_irrc_nfy_tag, (IRRC_COND_T)u4BtnState, u4Key, &rRawData);
    }
#if 0    
    else
    {
        _MW_IRRC_PowerOffHandler(u4BtnState, u4Key, prRaw->au1Data[0]);
    }
#endif    
#if defined(CC_MT5365) || defined(CC_MT5395) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)  
    IRRC_MixSound(u4BtnState, u4Key, prRaw);
#endif    
  
}
#endif  

/*-----------------------------------------------------------------------------
                    IRRC init function
 ----------------------------------------------------------------------------*/
INT32 MW_IRRC_Init(void)
{
    INT32       i4_return;
    
#ifndef LINUX_TURNKEY_SOLUTION /* !KEY_FROM_DFB */
    i4_return = IRRC_ThreadInit(MW_IRRC_NotifyFunc);
    if (i4_return)
    {
        Printf("IRRX_MW_Init(): failed [%d]\n", (UINT32)i4_return);
        return (-1);
    }

    UNUSED(DRVCUST_InitQuery(eIrrxSetUsrNotifyFunc, 
                             (UINT32 *)(void *)&(pf_irrc_usr_nfy_fct)));
#endif

    /* Setup IRRC component ID */
    t_irrc_comp_reg.e_type = DRVT_IRRC;
    t_irrc_comp_reg.e_id_type = ID_TYPE_IND;
    t_irrc_comp_reg.u.t_ind.ui2_id = DRV_IRRC_ID;
    t_irrc_comp_reg.u.t_ind.pv_tag = &ui4_irrc_comp_tag;
    t_irrc_comp_reg.u.t_ind.ui1_port = 0;

    /* Setup IRRC component function table */
    t_irrc_comp_fct_tbl.pf_rm_connect = NULL;
    t_irrc_comp_fct_tbl.pf_rm_disconnect = NULL;
    t_irrc_comp_fct_tbl.pf_rm_get = IRRC_Get;
    t_irrc_comp_fct_tbl.pf_rm_set = IRRC_Set;

    /* Register IRRC component with the Resource Manager database */
    i4_return = x_rm_reg_comp(&t_irrc_comp_reg,
                              1,
                              1,
                              (const CHAR*)pc_irrc_comp_name,
                              0,
                              &t_irrc_comp_fct_tbl,
                              NULL,
                              0);

    if (i4_return != RMR_OK)
    {
        Printf("IRRC_Init(): rm_reg_comp failed [%d]\n", (UINT32)i4_return);
        return (-1);
    }

    return (0);
}

/*-----------------------------------------------------------------------------
                    System code filter D interface
 ----------------------------------------------------------------------------*/
INT32 d_custom_irrx_system_code_filter(BOOL fgEnable, BOOL fgPositiveFilter, UINT32 u4Num, UINT32 au4FilterArray[])
{
    return IRRX_FilterSystemCode(fgEnable, fgPositiveFilter, u4Num, au4FilterArray);
}

