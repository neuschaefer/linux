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
 * $RCSfile: uart_mw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file uart_mw.c
 *  This file defines the interfaces for middleware to access uart driver
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_rm.h"
#include "x_rm_dev_types.h"
// #include "drv_comp_id.h"
#include "drv_name.h"
#include "x_serial.h"
#include "x_rs_232.h"
#include "../uart/uart_drv.h"
#include "../uart/uart_debug.h"
#include "x_printf.h"
#include "x_assert.h"

#include "uart_hw_drvif.h"
LINT_EXT_HEADER_END
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define FACTORY_PORT_ID     (UINT16)(7)

//---------------------------------------------------------------------------
// Variables
//---------------------------------------------------------------------------
static const CHAR             *i1_rs232_comp_name = DRVN_COM_RS_232;
static const CHAR             *i1_rs232_factory_name = "FactoryCom";
#ifdef CC_UART_PD_SUPPORT
static const CHAR             *i1_rs232_aux_name = "AuxCom";
#endif
static void *pvFACTORYTag = NULL;
static x_rs_232_nfy_fct rRs232FACTORYNfy = NULL;
static RS_232_SPEED_T eFactorySpeed = RS_232_SPEED_75;

//---------------------------------------------------------------------------
// External function declaration.
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
static void FACTORY_UartCallback(INT32 i4DataSz)
{
    if (rRs232FACTORYNfy != NULL)
    {
        rRs232FACTORYNfy(pvFACTORYTag, RS_232_COND_REC_BUFFER, (UINT32)i4DataSz);
    }
}

static void FACTORY_GetCapability(RS_232_CAPABILITY_INFO_T * prCapability)
{
    ASSERT(prCapability);
    LINT_SUPPRESS_BRACE(641);
    
    UART_GetCapability(UART_PORT_0, prCapability);
    prCapability->ui4_data_len = (UINT32)(RS_232_CAP_DATA_LEN_8);
    prCapability->ui4_stop_bit = (UINT32)(RS_232_CAP_STOP_BIT_1);
    prCapability->ui4_parity   = (UINT32)(RS_232_CAP_PARITY_NONE);
}

static void FACTORY_GetParameter(RS_232_SETUP_INFO_T * prSetupInfo)
{
    ASSERT(prSetupInfo);
    prSetupInfo->e_speed = eFactorySpeed;
    prSetupInfo->e_data_len = RS_232_DATA_LEN_8;
    prSetupInfo->e_parity = RS_232_PARITY_NONE;
    prSetupInfo->e_stop_bit = RS_232_STOP_BIT_1;
}

static INT32 FACTORY_SetParameter(RS_232_SETUP_INFO_T * prSetupInfo)
{
    INT32 i4Ret = 0;
    
    ASSERT(prSetupInfo);

    if (prSetupInfo->e_speed == RS_232_SPEED_75)        // close factory function
    {
        // hook NULL callback function into uart_drv.c for factory input
        i4Ret= DBG_FactorySetCallback(FALSE, NULL);
        eFactorySpeed = prSetupInfo->e_speed;
        
        // Enable outbyte function.
        SerSetoutbyte(TRUE);
        
        // Switch input into middleware
        while (UART_InputSwitch()==1) {;}
    }
    else if (prSetupInfo->e_speed <= RS_232_SPEED_921600)  // open factory function
    {
        // Change uart 0 baudrate
        i4Ret= DBG_FactorySetCallback(TRUE, FACTORY_UartCallback);
        UART_HwSetBaudRate(UART_PORT_0, (UINT8)prSetupInfo->e_speed);
        eFactorySpeed = prSetupInfo->e_speed;
    }
    else
    {
        return UART_FAIL;
    }

    UNUSED(i4Ret);        

    // other function does nothing
    return UART_SUCC;
}

static void FACTORY_SetNotifyFuc(RS_232_NFY_INFO_T* prRs232NfyInfo) // factory mode input notify function
{
    ASSERT(prRs232NfyInfo);

    pvFACTORYTag = prRs232NfyInfo->pv_tag;
    rRs232FACTORYNfy = prRs232NfyInfo->pf_rs_232_nfy;
}

static void FACTORY_Write(UINT8 u1Data) // factory mode output function
{
    SerPollPutChar(u1Data);
}

static INT32 _RS_232_Get(DRV_COMP_ID_T*   pt_comp_id,
                 DRV_GET_TYPE_T   e_get_type,
                 VOID*            pv_get_info,
                 SIZE_T*          pz_get_info_len)
{
    SIZE_T      t_get_info_len;
    //int         i_group_id;
    INT32 i4Ret = 0;

    // Check component type and id
    if ((pt_comp_id == NULL) ||(pv_get_info == NULL) || (pz_get_info_len == NULL))
    {
        return (RMR_DRV_INV_GET_INFO);
    }

    if (pt_comp_id->e_type != DRVT_COM_RS_232)
    {
       LOG(1, "RS_232_Get(): error component type or id type\n");
       return (RMR_DRV_INV_GET_INFO);
    }

    if ((pt_comp_id->ui2_id > (UINT16)(UART_PORT_NUM - 1)) &&
        (pt_comp_id->ui2_id != FACTORY_PORT_ID))
    {
       LOG(1,"RS_232_Get(): error component id [%d]\n", pt_comp_id->ui2_id);
       return (RMR_DRV_INV_GET_INFO);
    }

    // Perform get opertion
    switch (e_get_type)
    {
        case RS_232_GET_TYPE_CAPABILITY:
            t_get_info_len = sizeof(RS_232_CAPABILITY_INFO_T);
            if (*pz_get_info_len < t_get_info_len)
            {
                LOG(1,"RS_232_Get(): space is not enough\n");
                return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

            if (pt_comp_id->ui2_id == FACTORY_PORT_ID)
            {
                FACTORY_GetCapability((RS_232_CAPABILITY_INFO_T *)pv_get_info);
            }
            else
            {
                UART_GetCapability((UINT8)pt_comp_id->ui2_id, (RS_232_CAPABILITY_INFO_T *)pv_get_info);
            }
            break;

        case RS_232_GET_TYPE_SETUP:
            t_get_info_len = sizeof(RS_232_SETUP_INFO_T);
            if (*pz_get_info_len < t_get_info_len)
            {
                LOG(1,"RS_232_Get(): space is not enough\n");
                return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

            if (pt_comp_id->ui2_id == FACTORY_PORT_ID)
            {
                FACTORY_GetParameter((RS_232_SETUP_INFO_T *)pv_get_info);
            }
            else
            {
                i4Ret = UART_GetParameter((UINT8)pt_comp_id->ui2_id, (RS_232_SETUP_INFO_T *)pv_get_info);
                UNUSED(i4Ret);
            }
            break;

        case RS_232_GET_TYPE_CTRL_LINE:
            {
            t_get_info_len = sizeof(UINT8);
            if (*pz_get_info_len < t_get_info_len)
            {
                LOG(1,"RS_232_Get(): space is not enough\n");
                return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }    

            *(UINT8 *)pv_get_info = (UINT8)(UART_GetControlLine((UINT8)pt_comp_id->ui2_id)); 
            }
            break;

        case RS_232_GET_TYPE_REC_DATA:
            t_get_info_len = sizeof(UINT8);
            if (*pz_get_info_len < t_get_info_len)
            {
                LOG(1,"RS_232_Get(): space is not enough\n");
                return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }    

            {
                VERIFY(UART_Read((UINT8)pt_comp_id->ui2_id, (UINT8 *)pv_get_info, 1, TRUE) == 1);
            }
            break;

        case RS_232_GET_TYPE_REC_MULTI_DATA:
            t_get_info_len = sizeof(RS_232_MULTI_DATA_INFO_T);
            if (*pz_get_info_len < t_get_info_len)
            {
                 LOG(1,"RS_232_Get(): space is not enough\n");
                return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }    

            {
                UINT32 u4RcvLen = 0;
                RS_232_MULTI_DATA_INFO_T * prDataInfo = (RS_232_MULTI_DATA_INFO_T *)pv_get_info;
                u4RcvLen = (UINT32)((prDataInfo->z_data_len > SW_FIFO_SIZE) ? SW_FIFO_SIZE : prDataInfo->z_data_len);
                if (pt_comp_id->ui2_id != FACTORY_PORT_ID)
                {
                    prDataInfo->z_data_len = UART_Read((UINT8)pt_comp_id->ui2_id, prDataInfo->pui1_data, u4RcvLen, TRUE);
                }
                else
                {
                    prDataInfo->z_data_len = UART_Read(SerGetDebugPortNum(), prDataInfo->pui1_data, u4RcvLen, TRUE);
                }
            }
            break;

        case RS_232_GET_TYPE_REC_DATA_LEN:
            t_get_info_len = sizeof(SIZE_T);
            if (*pz_get_info_len < t_get_info_len)
            {
                 LOG(1,"RS_232_Get(): space is not enough\n");
                return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }    

            if (pt_comp_id->ui2_id != FACTORY_PORT_ID)
            {
                *(SIZE_T *)pv_get_info = (SIZE_T)UART_GetRcvDataLen((UINT8)pt_comp_id->ui2_id);
            }
            else
            {
                *(SIZE_T *)pv_get_info = (SIZE_T)UART_GetRcvDataLen(SerGetDebugPortNum());
            }
            break;

        case RS_232_GET_TYPE_XMT_DATA_LEN:
            t_get_info_len = sizeof(SIZE_T);
            if (*pz_get_info_len < t_get_info_len)
            {
                 LOG(1,"RS_232_Get(): space is not enough\n");
                 return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }    

            *(SIZE_T *)pv_get_info = (SIZE_T)UART_GetTxFreeLen((UINT8)pt_comp_id->ui2_id);
            break;

        case RS_232_GET_TYPE_OPERATION_MODE:
            t_get_info_len = sizeof(UINT32);
            if (*pz_get_info_len < t_get_info_len)
            {
                 LOG(1,"RS_232_Get(): space is not enough\n");
                return (RMR_DRV_NOT_ENOUGH_SPACE);
            }
            else
            {
                UINT32 u4OpMode = 0;
                
                *pz_get_info_len = t_get_info_len;
                u4OpMode  = (UART_IsTransMode()) ? 
                            RS_232_OPERATION_MODE_TRANSPARENT : 
                            RS_232_OPERATION_MODE_NORMAL;
                u4OpMode |= UART_IsInputFactory() ?
                            RS_232_OPERATION_MODE_FACTORY :
                            RS_232_OPERATION_MODE_CLI;
                            
                *(UINT32 *)pv_get_info = u4OpMode; 
            }    
            break;
            
        default:
            LOG(1,"RS_232_Get(): invalid get_type\n");
            return (RMR_DRV_INV_GET_INFO);
    }

    UNUSED(pt_comp_id);
    return (RMR_OK);
}


/*-----------------------------------------------------------------------------
                    RS232 set function
 ----------------------------------------------------------------------------*/

static INT32 _RS_232_Set(DRV_COMP_ID_T* pt_comp_id,
                       DRV_SET_TYPE_T   e_set_type,
                       const VOID*      pv_set_info,
                       SIZE_T           z_set_info_len)

{
    SIZE_T              t_set_info_len;
    INT32 i4Ret = 0;

    // Check component type and id
    if (pt_comp_id == NULL)
    {
        return (RMR_DRV_INV_SET_INFO);
    }

    if ((pt_comp_id->e_type != DRVT_COM_RS_232))
    {
       LOG(1,"RS_232_Set(): error component type or id type\n");
       return (RMR_DRV_INV_SET_INFO);
    }

    if ((pt_comp_id->ui2_id > (UINT16)(UART_PORT_NUM - 1)) &&
        (pt_comp_id->ui2_id != FACTORY_PORT_ID))
    {
       LOG(1,"RS_232_Set(): error component id [%d]\n", pt_comp_id->ui2_id);
       return (RMR_DRV_INV_SET_INFO);
    }

    // Perform set opertion
    switch (e_set_type)
    {
        case RS_232_SET_TYPE_SETUP:
            if (pv_set_info == NULL)
            {
                return (RMR_DRV_INV_SET_INFO);
            }
     
            t_set_info_len = sizeof(RS_232_SETUP_INFO_T);
            if (z_set_info_len != t_set_info_len)
            {
                LOG(1,"RS_232_Set(): space is not enough\n");
                return (RMR_DRV_INV_SET_INFO);
            }
            if (pt_comp_id->ui2_id == FACTORY_PORT_ID)
            {
                i4Ret = FACTORY_SetParameter((RS_232_SETUP_INFO_T *)pv_set_info);
            }
            else
            {
                i4Ret = UART_SetParameter((UINT8)pt_comp_id->ui2_id, (RS_232_SETUP_INFO_T *)pv_set_info);
            }
            break;

        case RS_232_SET_TYPE_CTRL_LINE:
            if (pv_set_info == NULL)
            {
                return (RMR_DRV_INV_SET_INFO);
            }

            t_set_info_len = sizeof(UINT8);
            if (z_set_info_len != t_set_info_len)
            {
                LOG(1,"RS_232_Set(): space is not enough\n");
                return (RMR_DRV_INV_SET_INFO);
            }
            UART_SetControlLine((UINT8)pt_comp_id->ui2_id, (UINT8)(UINT32)pv_set_info);
            break;

        case RS_232_SET_TYPE_NFY_FCT:
            if (pv_set_info == NULL)
            {
                return (RMR_DRV_INV_SET_INFO);
            }

            t_set_info_len = sizeof(RS_232_NFY_INFO_T);
            if (z_set_info_len != t_set_info_len)
            {
                LOG(1,"RS_232_Set(): space is not enough\n");
                return (RMR_DRV_INV_SET_INFO);
            }

            if (pt_comp_id->ui2_id == FACTORY_PORT_ID)
            {
                FACTORY_SetNotifyFuc((RS_232_NFY_INFO_T *)pv_set_info);
                UART_SetNotifyFuc(0, (RS_232_NFY_INFO_T *)pv_set_info);
            }
            else
            {
                UART_SetNotifyFuc((UINT8)pt_comp_id->ui2_id, (RS_232_NFY_INFO_T *)pv_set_info);
            }

            /*
            switch (pt_comp_id->ui2_id)
            {
                case UART_PORT_0:

                    pv_rs_232_port0_nfy_tag = pt_nfy_info->pv_tag;
                    pf_rs_232_port0_nfy_fct = pt_nfy_info->pf_rs_232_nfy;
                    break;
                case UART_PORT_1:
                    pt_nfy_info = (RS_232_NFY_INFO_T *)pv_set_info;
                    pv_rs_232_port1_nfy_tag = pt_nfy_info->pv_tag;
                    pf_rs_232_port1_nfy_fct = pt_nfy_info->pf_rs_232_nfy;
                    break;
                case UART_PORT_2:
                    pt_nfy_info = (RS_232_NFY_INFO_T *)pv_set_info;
                    pv_rs_232_port2_nfy_tag = pt_nfy_info->pv_tag;
                    pf_rs_232_port2_nfy_fct = pt_nfy_info->pf_rs_232_nfy;
                    break;
                default:
                    break;
            }
            */
            break;

        case RS_232_SET_TYPE_CTRL_NFY_COND:
            if (pv_set_info == NULL)
            {
                return (RMR_DRV_INV_SET_INFO);
            }

            t_set_info_len = sizeof(RS_232_NFY_INFO_T);
            if (z_set_info_len != t_set_info_len)
            {
                LOG(1,"RS_232_Set(): space is not enough\n");
                return (RMR_DRV_INV_SET_INFO);
            }
            break;

        case RS_232_SET_TYPE_XMT_DATA:
            if (pv_set_info == NULL)
            {
                UCHAR ucData = 0;
                UART_SetTxPrivateTag((UINT8)pt_comp_id->ui2_id, 0);
                if (1 != UART_Write((UINT8)pt_comp_id->ui2_id, &ucData, 1, FALSE))
                {
                    return (RMR_DRV_INV_SET_INFO);
                }
                return (RMR_OK);
            }
            
            t_set_info_len = sizeof(UINT8);
            if (z_set_info_len != t_set_info_len)
            {
                LOG(1,"RS_232_Set(): space is not enough\n");
                return (RMR_DRV_INV_SET_INFO);
            }
            if (pt_comp_id->ui2_id == FACTORY_PORT_ID)
            {
                UCHAR ucData;
                ucData = (UINT8)(UINT32)pv_set_info;
                FACTORY_Write(ucData);
            }
            else
            {
                UCHAR ucData;
                ucData = (UINT8)(UINT32)pv_set_info;
                UART_SetTxPrivateTag((UINT8)pt_comp_id->ui2_id, 0);
                if (1 != UART_Write((UINT8)pt_comp_id->ui2_id, &ucData, 1, FALSE))
                {
                    return (RMR_DRV_INV_SET_INFO);
                }
            }
            break;

        case RS_232_SET_TYPE_XMT_MULTI_DATA:
            if (pv_set_info == NULL)
            {
                return (RMR_DRV_INV_SET_INFO);
            }

            t_set_info_len = sizeof(RS_232_MULTI_DATA_INFO_T);
            if (z_set_info_len != t_set_info_len)
            {
                LOG(1,"RS_232_Set(): space is not enough\n");
                return (RMR_DRV_INV_SET_INFO);
            }

            {
                UINT32 u4Len;
                UINT16 u2Id;
                RS_232_MULTI_DATA_INFO_T * prDataInfo;

                prDataInfo = (RS_232_MULTI_DATA_INFO_T *)pv_set_info;

                u4Len = (UINT32)((prDataInfo->z_data_len >= SW_FIFO_SIZE) ? (SW_FIFO_SIZE - 1) : prDataInfo->z_data_len);

                u2Id = (pt_comp_id->ui2_id == FACTORY_PORT_ID) ?
                       0 : pt_comp_id->ui2_id;
                UART_SetTxPrivateTag((UINT8)u2Id, (UINT32)prDataInfo->pv_tag);
                
                if (pt_comp_id->ui2_id != FACTORY_PORT_ID)
                {
                    prDataInfo->z_data_len = UART_Write((UINT8)pt_comp_id->ui2_id, prDataInfo->pui1_data, u4Len, FALSE);
                }
                else
                {
                    prDataInfo->z_data_len = UART_Write(SerGetDebugPortNum(), prDataInfo->pui1_data, u4Len, FALSE);
                }
            }
            break;

        case RS_232_SET_TYPE_OPERATION_MODE:
            t_set_info_len = sizeof(UINT32);
            if (z_set_info_len != t_set_info_len)
            {
                LOG(1,"RS_232_Set(): space is not enough\n");
                return (RMR_DRV_INV_SET_INFO);
            }
            else
            {
                UINT32 u4Mode = (UINT32)pv_set_info;
                
                switch (u4Mode) {
                case (RS_232_OPERATION_MODE_FACTORY | RS_232_OPERATION_MODE_TRANSPARENT):
                    UART_SetDebugPortFactory(TRUE);
                    SerSetoutbyte(FALSE);
                    break;
                case (RS_232_OPERATION_MODE_CLI | RS_232_OPERATION_MODE_TRANSPARENT):
                    UART_EnterInputFactory(FALSE);
                    UART_SetInterfaceMode(TRUE);
                    break;
                case (RS_232_OPERATION_MODE_CLI | RS_232_OPERATION_MODE_NORMAL):
                    UART_EnterInputFactory(FALSE);
                    UART_SetInterfaceMode(FALSE);
                    break;
                case (RS_232_OPERATION_MODE_TRANSPARENT):
                    UART_SetInterfaceMode(TRUE);
                    break;
                case (RS_232_OPERATION_MODE_NORMAL):
                    if (UART_IsInputFactory())
                    {
                        return (RMR_DRV_INV_SET_INFO);
                    }
                    UART_SetInterfaceMode(FALSE);
                    break;
                default:
                    return (RMR_DRV_INV_SET_INFO);
                }
            }
            break;
        default:
            LOG(1,"RS_232_Set(): nothing has been done\n");
            return (RMR_DRV_INV_SET_INFO);
    }

    UNUSED(pt_comp_id);
    UNUSED(i4Ret);
    return (RMR_OK);
}

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** RS_232_Init
 *
 */
//-------------------------------------------------------------------------
INT32 RS_232_Init(void)
{
    INT32       i4_return;
    DRV_COMP_REG_T tRs232CompReg;
    DRV_COMP_REG_T tRs232FactoryReg;
    DRV_COMP_FCT_TBL_T tRs232FctTbl;
    UINT32      u4CompFlags;
    INT32 i4Ret = 0;

    UNUSED(tRs232CompReg);
    UNUSED(i1_rs232_comp_name);
    
    /* Init Uart hardware */
    i4Ret = UART_Init();
    UNUSED(i4Ret);

    /* Setup component function table */
    tRs232FctTbl.pf_rm_connect = NULL;
    tRs232FctTbl.pf_rm_disconnect = NULL;
    tRs232FctTbl.pf_rm_get = _RS_232_Get;
    tRs232FctTbl.pf_rm_set = _RS_232_Set;

#ifndef DRV_SUPPORT_EXTMJC
    /* Setup component ID */
    tRs232CompReg.e_type = DRVT_COM_RS_232;
    tRs232CompReg.e_id_type = ID_TYPE_IND;
    tRs232CompReg.u.t_range.ui2_first_id = UART_PORT_1;
    tRs232CompReg.u.t_range.ui2_delta_id = 1;
    tRs232CompReg.u.t_range.ui2_num_of_ids = 1;
    tRs232CompReg.u.t_range.pv_first_tag = NULL;
    tRs232CompReg.u.t_range.pv_delta_tag = NULL;
    tRs232CompReg.u.t_range.ui1_port = 1;

    /* Setup component flags */
    u4CompFlags = (RS_232_FLAG_REC_DATA_BUFFER | RS_232_FLAG_XMT_DATA_BUFFER);

    /* Register component with the Resource Manager database */
    i4_return = x_rm_reg_comp(&tRs232CompReg,
                                1,
                                1,
                                i1_rs232_comp_name,
                                u4CompFlags,
                                &tRs232FctTbl,
                                NULL,
                                0);
    if (i4_return != RMR_OK)
    {
        return i4_return;
    }
#endif
    /* Factory mode uart registration */

    /* Setup component ID */
    tRs232FactoryReg.e_type = DRVT_COM_RS_232;
    tRs232FactoryReg.e_id_type = ID_TYPE_IND;
    tRs232FactoryReg.u.t_range.ui2_first_id = FACTORY_PORT_ID;
    tRs232FactoryReg.u.t_range.ui2_delta_id = 1;
    tRs232FactoryReg.u.t_range.ui2_num_of_ids = 1;
    tRs232FactoryReg.u.t_range.pv_first_tag = NULL;
    tRs232FactoryReg.u.t_range.pv_delta_tag = NULL;
    tRs232FactoryReg.u.t_range.ui1_port = 1;

    /* Setup component flags */
    u4CompFlags = 0;

    /* Register component with the Resource Manager database */
    i4_return = x_rm_reg_comp(&tRs232FactoryReg,
                                1,
                                1,
                                i1_rs232_factory_name,
                                u4CompFlags,
                                &tRs232FctTbl,
                                NULL,
                                0);
    if (i4_return != RMR_OK)
    {
        return i4_return;
    }

#ifdef CC_UART_PD_SUPPORT
    /* Setup component ID */
    tRs232CompReg.e_type = DRVT_COM_RS_232;
    tRs232CompReg.e_id_type = ID_TYPE_IND;
    tRs232CompReg.u.t_range.ui2_first_id = UART_PORT_PD;
    tRs232CompReg.u.t_range.ui2_delta_id = 1;
    tRs232CompReg.u.t_range.ui2_num_of_ids = 1;
    tRs232CompReg.u.t_range.pv_first_tag = NULL;
    tRs232CompReg.u.t_range.pv_delta_tag = NULL;
    tRs232CompReg.u.t_range.ui1_port = 1;

    /* Setup component flags */
    //u4CompFlags = (RS_232_FLAG_REC_DATA_BUFFER | RS_232_FLAG_XMT_DATA_BUFFER);
    u4CompFlags = 0;

    /* Register component with the Resource Manager database */
    i4_return = x_rm_reg_comp(&tRs232CompReg,
                                1,
                                1,
                                i1_rs232_aux_name,
                                u4CompFlags,
                                &tRs232FctTbl,
                                NULL,
                                0);
    if (i4_return != RMR_OK)
    {
        return i4_return;
    }
    #endif
    /* Register component exclusive list */

    /* Register connection exclusive list */

    return (RMR_OK);
}

