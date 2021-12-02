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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: pod_mw.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file pod_mw.c
 *  This file contains implementation middleware interface of POD.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "x_printf.h"
#include "x_rm_dev_types.h"
#include "x_rm.h"
#include "x_pinmux.h"
#include "x_pod.h"
#include "drv_name.h"
#include "../5381_driver/pod/pod_isr.h"
#include "pod_if.h"

#define DEFINE_IS_LOG	POD_IsLog
#include "x_debug.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define DRV_POD_ID          0x341

#ifdef POD_DRV_DMA
#define POD_IO_STATE        POD_DMA
#else
#ifdef POD_DRV_SDA
#define POD_IO_STATE        POD_SDA
#else
#define POD_IO_STATE        POD_DMA	/* Default IO state */
#endif
#endif

#define POD_DEBUG_SWITCH    0


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#if POD_DEBUG_SWITCH
#define POD_DEBUG(fmt...)   Printf(fmt)
#else
#define POD_DEBUG(fmt...)
#endif


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static CHAR                 *ac_pod_comp_name = DRVN_POD;
static DRV_COMP_REG_T       t_pod_comp_reg;
static DRV_COMP_FCT_TBL_T   t_pod_comp_fct_tbl;
static UINT32               ui4_pod_comp_tag = 0x341;


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** This API implements Middleware get functionality for PCMCIA/POD/CI.
 */
//-----------------------------------------------------------------------------
INT32 POD_Get(DRV_COMP_ID_T*    pt_comp_id,
              DRV_GET_TYPE_T    e_get_type,
              VOID*             pv_get_info,
              SIZE_T*           pz_get_info_len)
{
    SIZE_T              t_get_info_len;
    UINT32              u4_pod_state;
    POD_ACCESS_MODE_T   t_access_mode;

    /* Check component type and id */
    if (pt_comp_id == NULL)
    {
        return RMR_DRV_INV_GET_INFO;
    }

    if ((pt_comp_id->e_type != DRVT_POD) ||
        (pt_comp_id->ui2_id != DRV_POD_ID))
    {
        POD_DEBUG("POD_Get(): error component type or id type\n");
        return RMR_DRV_INV_SET_INFO;
    }

    if (pt_comp_id->ui2_id != DRV_POD_ID)
    {
        POD_DEBUG("POD_Get(): error component id [%d]\n", pt_comp_id->ui2_id);
        return RMR_DRV_INV_SET_INFO;
    }

    /* Perform get opertion */
    switch (e_get_type)
    {
        case POD_GET_TYPE_POD_STATE:
            t_get_info_len = sizeof(UINT32);
            if (*pz_get_info_len < t_get_info_len)
            {
                POD_DEBUG("POD_Get(): space is not enough\n");
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

            if (POD_GetPodState(&u4_pod_state) != POD_OK)
            {
                return RMR_DRV_ERROR;
            }

            if (u4_pod_state & POD_INSERTED)
            {
                *(UINT32 *)pv_get_info = POD_COND_INSERTED;
            }
            else
            {
                *(UINT32 *)pv_get_info = POD_COND_REMOVED;
            }
            break;

        case POD_GET_TYPE_ACCESS_MODE:
            t_get_info_len = sizeof(POD_ACCESS_MODE_T);
            if (*pz_get_info_len < t_get_info_len)
            {
                POD_DEBUG("POD_Get(): space is not enough\n");
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

            if (POD_GetAccessMode(&t_access_mode) != POD_OK)
            {
                return RMR_DRV_ERROR;
            }

            *(POD_ACCESS_MODE_T *)pv_get_info = t_access_mode;
            break;

        case POD_GET_TYPE_READ_CIS:
            t_get_info_len = sizeof(POD_DATA_INFO_T);
            if (*pz_get_info_len < t_get_info_len)
            {
                POD_DEBUG("POD_Get(): space is not enough\n");
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

            if (POD_GetCis(POD_IO_STATE,
            			   ((POD_DATA_INFO_T *)pv_get_info)->pui1_data,
                           (UINT32 *)(&(((POD_DATA_INFO_T  *)pv_get_info)->z_data_len))) != POD_OK)
            {
                return RMR_DRV_ERROR;
            }
            break;

        case POD_GET_TYPE_READ_REG:
            t_get_info_len = sizeof(POD_DATA_INFO_T);
            if (*pz_get_info_len < t_get_info_len)
            {
                POD_DEBUG("POD_Get(): space is not enough\n");
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

            if (POD_ReadReg(POD_IO_STATE,
                            ((POD_REG_DATA_INFO_T *)pv_get_info)->ui1_reg_addr,
                            ((POD_REG_DATA_INFO_T *)pv_get_info)->t_data_info.pui1_data,
                            (UINT16 *)(&((POD_REG_DATA_INFO_T *)pv_get_info)->t_data_info.z_data_len)) != POD_OK)
            {
                return RMR_DRV_ERROR;
            }
            break;

        case POD_GET_TYPE_READ_DATA:
            t_get_info_len = sizeof(POD_DATA_INFO_T);
            if (*pz_get_info_len < t_get_info_len)
            {
                POD_DEBUG("POD_Get(): space is not enough\n");
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

            if (POD_Read(POD_IO_STATE,
                         ((POD_DATA_INFO_T *)pv_get_info)->pui1_data,
                         (UINT16 *)(&((POD_DATA_INFO_T *)pv_get_info)->z_data_len)) != POD_OK)
            {
                return RMR_DRV_ERROR;
            }
            break;

        default:
            POD_DEBUG("POD_Get(): invalid get_type\n");
            return RMR_DRV_INV_GET_INFO;
    }

    return RMR_OK;
}

//-----------------------------------------------------------------------------
/** This API implements Middleware set functionality for PCMCIA/POD/CI.
 */
//-----------------------------------------------------------------------------
INT32 POD_Set(DRV_COMP_ID_T*    pt_comp_id,
              DRV_SET_TYPE_T    e_set_type,
              const VOID*       pv_set_info,
              SIZE_T            z_set_info_len)
{
    BOOL    fgPcmciaToPod = FALSE;

    /* Check component type and id */
    if (pt_comp_id == NULL)
    {
        return RMR_DRV_INV_SET_INFO;
    }

    if ((pt_comp_id->e_type != DRVT_POD) ||
        (pt_comp_id->ui2_id != DRV_POD_ID))
    {
        POD_DEBUG("POD_Set(): error component type or id type\n");
        return RMR_DRV_INV_SET_INFO;
    }

    if (pt_comp_id->ui2_id != DRV_POD_ID)
    {
        POD_DEBUG("POD_Set(): error component id [%d]\n", pt_comp_id->ui2_id);
        return RMR_DRV_INV_SET_INFO;
    }

    /* Check set_info */
    if (!pv_set_info &&
        e_set_type != POD_SET_TYPE_LOW_LEVEL_INIT)
    {
        POD_DEBUG("POD_Set(): invalid set_info\n");
        return RMR_DRV_INV_SET_INFO;
    }

    /* Perform set opertion */
    switch (e_set_type)
    {
        case POD_SET_TYPE_RESET:
            if (((POD_SIGNAL_T *)pv_set_info)->ui4_set == POD_PIN_SET)
            {
                POD_Reset(TRUE);
            }

            if (((POD_SIGNAL_T *)pv_set_info)->ui4_clear == POD_PIN_SET)
            {
                POD_Reset(FALSE);
            }
            break;

        case POD_SET_TYPE_LOW_LEVEL_INIT:
            if (POD_LowLevelInit() != POD_OK)
            {
                return RMR_DRV_ERROR;
            }
            break;

        case POD_SET_TYPE_ACCESS_MODE:
            if (POD_SetAccessMode(*(POD_ACCESS_MODE_T *)pv_set_info) != POD_OK)
            {
                return RMR_DRV_ERROR;
            }
            break;

        case POD_SET_TYPE_MODULE_MODE:
            if (*(POD_MODULE_MODE_T *)pv_set_info == MODULE_MODE_PCMCIA)
            {
                fgPcmciaToPod = FALSE;
            }
            else if (*(POD_MODULE_MODE_T *)pv_set_info == MODULE_MODE_POD)
            {
                fgPcmciaToPod = TRUE;
            }

            if (POD_PcmciaToPod(fgPcmciaToPod) != POD_OK)
            {
                return RMR_DRV_ERROR;
            }
            break;

        case POD_SET_TYPE_POWER_CTRL:
            if (POD_ConfigPower(*(POD_POWER_CTRL_T *)pv_set_info) != POD_OK)
            {
                return RMR_DRV_ERROR;
            }
            break;

        case POD_SET_TYPE_NFY_FCT:
            POD_SetCallback(((POD_NFY_INFO_T *)pv_set_info)->pf_pod_nfy,
                            ((POD_NFY_INFO_T *)pv_set_info)->pv_tag);
            break;

        case POD_SET_TYPE_WRITE_COR:
            if (POD_WriteCor(((POD_COR_INFO_T *)pv_set_info)->ui2_cor_address,
                             ((POD_COR_INFO_T *)pv_set_info)->ui1_cor_value) != POD_OK)
            {
                return RMR_DRV_ERROR;
            }
            break;

        case POD_SET_TYPE_WRITE_REG:
            if (POD_WriteReg(POD_IO_STATE,
                             ((POD_REG_DATA_INFO_T *)pv_set_info)->ui1_reg_addr,
                             ((POD_REG_DATA_INFO_T *)pv_set_info)->t_data_info.pui1_data,
                             ((POD_REG_DATA_INFO_T *)pv_set_info)->t_data_info.z_data_len) != POD_OK)
            {
                return RMR_DRV_ERROR;
            }
            break;

        case POD_SET_TYPE_WRITE_DATA:
            if (POD_Write(POD_IO_STATE,
                          ((POD_DATA_INFO_T *)pv_set_info)->pui1_data,
                          ((POD_DATA_INFO_T *)pv_set_info)->z_data_len) != POD_OK)
            {
                return RMR_DRV_ERROR;
            }
            break;

        default:
            POD_DEBUG("POD_Set(): nothing has been done\n");
            return RMR_DRV_INV_SET_INFO;
    }

    return RMR_OK;
}

//-----------------------------------------------------------------------------
/** This API initializes PCMCIA/POD/CI Middleware adaptation layer.
 */
//-----------------------------------------------------------------------------
INT32 MW_PodInit(void)
{
    INT32       i4_return;
    INT32       i4Val;
    INT32       i4Set;
    INT32       i4Gpio;

    /* Configure Vcc = 3.3V and Vpp = 5V as default setting */
    /* Config GPIO 34 for Vpp control (0 = 5V, 1 = 3.3V) */
    i4Gpio = 34;
    i4Val = 0;
    GPIO_Enable(i4Gpio, &i4Val); /* Change to input mode first */
    BSP_PinGpioSel(i4Gpio);
    i4Set = 1; /* Set GPIO 34 to 1 */
    GPIO_Output(i4Gpio, &i4Set);
    i4Val = 1;
    GPIO_Enable(i4Gpio, &i4Val); /* Change to output mode */

    /* Config GPIO 35 for power output control (0 = power off, 1 = power on) */
    i4Gpio = 35;
    i4Val = 0;
    GPIO_Enable(i4Gpio, &i4Val); /* Change to input mode first */
    BSP_PinGpioSel(i4Gpio);
    i4Set = 1; /* Set GPIO 35 to 1 */
    GPIO_Output(i4Gpio, &i4Set);
    i4Val = 1;
    GPIO_Enable(i4Gpio, &i4Val); /* Change to output mode */

	/* Allocate a 32-byte alignment DMA buffer */
	POD_AllocDmaBuf();

    /* Init POD driver */
    i4_return = POD_Init();
    if (i4_return != POD_OK)
    {
        return i4_return;
    }

    /* Register PCMCIA/POD/CI IRQ handler */
    i4_return = POD_EnableIsr();
    if (i4_return != POD_OK)
    {
        return i4_return;
    }

    /* Refresh POD state */
    POD_RefreshPodState();

    /* Setup POD component ID */
    t_pod_comp_reg.e_type = DRVT_POD;
    t_pod_comp_reg.e_id_type = ID_TYPE_IND;
    t_pod_comp_reg.u.t_ind.ui2_id = DRV_POD_ID;
    t_pod_comp_reg.u.t_ind.pv_tag = &ui4_pod_comp_tag;
    t_pod_comp_reg.u.t_ind.ui1_port = 0;

    /* Setup POD component function table */
    t_pod_comp_fct_tbl.pf_rm_connect = NULL;
    t_pod_comp_fct_tbl.pf_rm_disconnect = NULL;
    t_pod_comp_fct_tbl.pf_rm_get = POD_Get;
    t_pod_comp_fct_tbl.pf_rm_set = POD_Set;

    /* Register POD component with Resource Manager database */
    i4_return = x_rm_reg_comp(&t_pod_comp_reg,
                              1,
                              1,
                              (const CHAR *)&ac_pod_comp_name,
                              0,
                              &t_pod_comp_fct_tbl,
                              NULL,
                              0);

    return i4_return;
}

