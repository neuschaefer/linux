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
 * $RCSfile: ci_mw.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file ci_mw.c
 *  This file contains implementation middleware interface of CI.
 */

#ifndef CI_SUPPORT_DVBT_BYPASS
//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"
#include "x_printf.h"
#include "x_rm_dev_types.h"
#include "x_rm.h"
#include "x_pinmux.h"
#include "x_pcmcia.h"
#include "x_ci.h"
#include "drv_name.h"
#include "pcmcia_if.h"
#include "x_debug.h"
#include "dmx_mw.h"
#include "drvcust_if.h"

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define CI_DEBUG_SWITCH		1

#define DRV_CI_ID			0x341
#define DRVT_CI				DRVT_POD

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#if CI_DEBUG_SWITCH
#define CI_DEBUG(fmt...)   Printf(fmt)
#else
#define CI_DEBUG(fmt...)
#endif

#if defined (CC_MT5391) && !defined (CC_MT5392B)
    #define DMX_FORCE_KICK_CLOCK
#endif
#ifdef DMX_FORCE_KICK_CLOCK
    #include "pd_dvbt_if.h"
#endif

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static CHAR                 *ac_pod_comp_name = DRVN_POD;
static DRV_COMP_REG_T       t_pod_comp_reg;
static DRV_COMP_FCT_TBL_T   t_pod_comp_fct_tbl;
static UINT32               ui4_pod_comp_tag = 0x341;

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------
typedef enum
{
    POD_POWER_DRIVING_2MA = 0,
    POD_POWER_DRIVING_4MA = 1,
    POD_POWER_DRIVING_6MA = 2,
    POD_POWER_DRIVING_8MA = 3
} POD_POWER_DRIVING_T;

static void _SetGpioOut(INT32 i4Pin, INT32 i4Val)
{
    INT32 i4Enable;
    UNUSED(_SetGpioOut);

    i4Enable = 0;
    GPIO_Enable(i4Pin, &i4Enable);
    GPIO_Output(i4Pin, &i4Val);
    i4Enable = 1;
    GPIO_Enable(i4Pin, &i4Enable);
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
#if !defined(CC_MT5391) && !defined(CC_MT5360)
void CI_SetPower(POD_POWER_DRIVING_T ePower)
{
    CKGEN_WRITE32(0x380, (CKGEN_READ32(0x380) & 0xFFFFFFCF) | ((INT32)ePower << 4));
}
#endif
BOOL CI_GetModuleExists()
{
#if 1
    PFN_CI_CUSTOM_QUERY_FUNC pfnCiQueryInfo = NULL;
    CiCustomInfo_T CiInfo;
    if (DRVCUST_OptQuery(eCiCustomQueryFunc, (UINT32 *)&pfnCiQueryInfo) == 0)
    {
        if( pfnCiQueryInfo )
        {
            pfnCiQueryInfo( &CiInfo );
            if( CiInfo.fgDisableCI ==TRUE)
            {
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
        else        //no query function
        {
            return TRUE;
        }
    }
    else        //no this type (drv_cust_if.h too old?)
    {
        return TRUE;
    }
#else
	return TRUE;
#endif
}

//-----------------------------------------------------------------------------
/*
 * This API implements Middleware get functionality for PCMCIA/POD/CI.
 */
//-----------------------------------------------------------------------------
INT32 Pcmcia_Get(DRV_COMP_ID_T*    	pt_comp_id,
              DRV_GET_TYPE_T    e_get_type,
              VOID*             pv_get_info,
              SIZE_T*           pz_get_info_len)
{
    SIZE_T              t_get_info_len;
    BOOL				b_return_value;
    INT32				i4_return_value;
    POD_COND_T			t_card_present;

    /* Check component type and id */
    if (pt_comp_id == NULL)
    {
        CI_DEBUG("Pcmcia_Get(): NULL pt_comp_id\n");
        return RMR_DRV_INV_GET_INFO;
    }

    if ((pt_comp_id->e_type != DRVT_CI) ||
        (pt_comp_id->ui2_id != DRV_CI_ID))
    {
        CI_DEBUG("Pcmcia_Get(): error component type or id type\n");
        CI_DEBUG("Type:%d[%d], ID:%d[%d]\n",
                 pt_comp_id->e_type, DRVT_CI,
                 pt_comp_id->ui2_id, DRV_CI_ID);
        return RMR_DRV_INV_SET_INFO;
    }

    /* Perform get opertion */
    switch (e_get_type)
    {
        case CI_GET_TYPE_POD_ACTIVE:
	       	b_return_value = PCMCIA_DetectPodActive();
        	if ((b_return_value == TRUE) || (b_return_value == FALSE))
        	{
        		*(BOOL*)pv_get_info = b_return_value;
        	}
        	else
        	{
                CI_DEBUG("Pcmcia_Get(CI_GET_TYPE_POD_ACTIVE): pod active error\n");
        		return RMR_DRV_ERROR;
        	}
            break;
        case CI_GET_TYPE_CARD_PRESENT:
        	i4_return_value = PCMCIA_DetectCard();
			if (i4_return_value == 0)
			{
				t_card_present = POD_COND_REMOVED;
			}
			else if (i4_return_value == 1)
			{
				t_card_present = POD_COND_INSERTED;
			}
			else
			{
                CI_DEBUG("Pcmcia_Get(CI_GET_TYPE_CARD_PRESENT): card present error\n");
				return RMR_DRV_ERROR;
			}
			*(POD_COND_T*)pv_get_info = t_card_present;
            break;
        case CI_GET_TYPE_CARD_READY:
        	i4_return_value = PCMCIA_TestCardReady();
        	if ((i4_return_value == 0) || (i4_return_value == 1))
        	{
        		*(INT32*)pv_get_info = i4_return_value;
        	}
        	else
        	{
                CI_DEBUG("Pcmcia_Get(CI_GET_TYPE_CARD_READY): card ready error\n");
        		return RMR_DRV_ERROR;
        	}
            break;
        case CI_GET_TYPE_READ_COR:
            i4_return_value = PCMCIA_ReadCor(((POD_COR_INFO_T*)pv_get_info)->ui2_cor_address);
			((POD_COR_INFO_T*)pv_get_info)->ui1_cor_value = (UINT8)i4_return_value;
            break;
        case CI_GET_TYPE_READ_REG:
            t_get_info_len = sizeof(POD_DATA_INFO_T);

            if (*pz_get_info_len < t_get_info_len)
            {
                CI_DEBUG("Pcmcia_Get(CI_GET_TYPE_READ_REG): space is not enough\n");
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

		    PCMCIA_SetPodCiAccessMode(POD_DATA_CHANNEL);
		    PCMCIA_PcmciaToPod(TRUE);

            if (PCMCIA_ReadReg(POD_SDA,
            				((POD_REG_DATA_INFO_T*)pv_get_info)->ui1_reg_addr,
            				((POD_REG_DATA_INFO_T*)pv_get_info)->t_data_info.pui1_data,
            				(UINT16)(((POD_REG_DATA_INFO_T *)pv_get_info)->t_data_info.z_data_len)) != POD_OK)
            {
                CI_DEBUG("Pcmcia_Get(CI_GET_TYPE_READ_REG): read reg error\n");
				return RMR_DRV_ERROR;
			}
            break;
        case CI_GET_TYPE_READ_DATA:
            t_get_info_len = sizeof(POD_DATA_INFO_T);

            if (*pz_get_info_len < t_get_info_len)
            {
                CI_DEBUG("Pcmcia_Get(CI_GET_TYPE_READ_DATA): space is not enough\n");
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                *pz_get_info_len = t_get_info_len;
            }

            if (PCMCIA_Read(POD_SDA,
                         ((POD_DATA_INFO_T *)pv_get_info)->pui1_data,
                         (UINT16 *)(&((POD_DATA_INFO_T *)pv_get_info)->z_data_len)) != POD_OK)
            {
                CI_DEBUG("Pcmcia_Get(CI_GET_TYPE_READ_DATA): read data error\n");
                return RMR_DRV_ERROR;
            }
            break;
        default:
            CI_DEBUG("Pcmcia_Get(): invalid get_type\n");
            return RMR_DRV_INV_GET_INFO;
    }

    return RMR_OK;
}

//-----------------------------------------------------------------------------
/*
 * This API implements Middleware set functionality for PCMCIA/POD/CI.
 */
//-----------------------------------------------------------------------------
INT32 Pcmcia_Set(DRV_COMP_ID_T*    pt_comp_id,
             DRV_SET_TYPE_T    e_set_type,
             const VOID*       pv_set_info,
             SIZE_T            z_set_info_len)
{
	BOOL    fgPcmciaToPod = FALSE;
    INT32	i4_return_value = POD_OK;

    /* Check component type and id */
    if (pt_comp_id == NULL)
    {
        CI_DEBUG("Pcmcia_Set(): NULL pt_comp_id\n");
        return RMR_DRV_INV_SET_INFO;
    }

    if ((pt_comp_id->e_type != DRVT_CI) ||
        (pt_comp_id->ui2_id != DRV_CI_ID))
    {
        CI_DEBUG("Pcmcia_Set(): error component type or id type\n");
        CI_DEBUG("Type:%d[%d], ID:%d[%d]\n",
                 pt_comp_id->e_type, DRVT_CI,
                 pt_comp_id->ui2_id, DRV_CI_ID);
        return RMR_DRV_INV_SET_INFO;
    }

    /* Check set_info */
    /* Only CI_SET_TYPE_RESET_CAM can have pv_set_info as NULL */
    if ((!pv_set_info) &&
        (e_set_type != CI_SET_TYPE_RESET_CAM) &&
        (e_set_type != CI_SET_TYPE_INIT_CI))
    {
        CI_DEBUG("Pcmcia_Set(): invalid set_info\n");
        return RMR_DRV_INV_SET_INFO;
    }

    /* Perform set opertion */
    switch (e_set_type)
    {
    	case CI_SET_TYPE_INIT_CI:
        #if !defined(CC_MT5391) && !defined(CC_MT5360)
	        /* Give CI more power for card compatibility */
	        CI_SetPower(POD_POWER_DRIVING_6MA);
        #endif
	        if ((PCMCIA_Init() == POD_OK) &&
	            (PCMCIA_EnableIsr() == POD_OK))
	        {
	            PCMCIA_AllocDmaBuf();
	        }
	        else
	        {
	            CI_DEBUG("Pcmcia_Set(CI_SET_TYPE_INIT_CI): error\n");
	        	return RMR_DRV_ERROR;
	        }

	        /* Force to PCMCIA mode */
	        PCMCIA_PcmciaToPod(FALSE);

	        /* Disable TS in and out first */
	        PCMCIA_PcmciaEnableTS(FALSE);
	        break;
		case CI_SET_TYPE_MODULE_MODE:
            if (*(POD_MODULE_MODE_T *)pv_set_info == MODULE_MODE_PCMCIA)
            {
                fgPcmciaToPod = FALSE;
            }
            else if (*(POD_MODULE_MODE_T *)pv_set_info == MODULE_MODE_POD)
            {
                fgPcmciaToPod = TRUE;
            }

            if (PCMCIA_PcmciaToPod(fgPcmciaToPod) != POD_OK)
            {
	            CI_DEBUG("Pcmcia_Set(CI_SET_TYPE_MODULE_MODE): error\n");
                return RMR_DRV_ERROR;
            }
            break;
		case CI_SET_TYPE_ENABLE_TS:
		    if (((POD_ENABLE_TS_T*)pv_set_info)->b_from_ca == TRUE)
		    {
		        if (!PCMCIA_DetectCard())
		        {
		            return RMR_OK;
		        }
		    }

			if (PCMCIA_PcmciaEnableTS(((POD_ENABLE_TS_T*)pv_set_info)->b_pod_ts_mode_enable != POD_OK))
			{
			    CI_DEBUG("Pcmcia_Set(CI_SET_TYPE_ENABLE_TS): error\n");
				return RMR_DRV_ERROR;
			}
            #if ( defined(CC_MT5391) || defined(CC_MT5360) )
                if( ((POD_ENABLE_TS_T*)pv_set_info)->b_pod_ts_mode_enable )
                {
                    #ifdef DMX_FORCE_KICK_CLOCK
                        DVBT_SetDmxExt();
                        DMX_SetCIFrontEnd(DMX_FE_EXT_SERIAL);
                    #else
                        if( _MW_DMX_CISetFrontEnd(DMX_FE_EXT_SERIAL) ==FALSE)
                            return RMR_DRV_ERROR;
                    #endif
                }
                else
                {
                    if ( PCMCIA_GetTsType() == 0 )
                    {
                        #ifdef DMX_FORCE_KICK_CLOCK
                            _MW_DMX_CISetFrontEnd(DMX_FE_DVB_DEMOD);
                        #else
                            if( _MW_DMX_CISetFrontEnd(DMX_FE_DVB_DEMOD) ==FALSE)
                                return RMR_DRV_ERROR;
                        #endif
                    }
                    else
                    {
                        #ifdef DMX_FORCE_KICK_CLOCK
                            DVBT_SetDmxExt();
                            DMX_SetCIFrontEnd(DMX_FE_EXT_SERIAL);
                        #else
                            if( _MW_DMX_CISetFrontEnd(DMX_FE_EXT_SERIAL) ==FALSE)
                                return RMR_DRV_ERROR;
                        #endif
                    }
                }
            #else   // CC_MT5363
                PCMCIA_AutoSetTsRoutingPath();
            #endif
            
			break;
		case CI_SET_TYPE_WRITE_DATA:
			if (PCMCIA_Write(POD_SDA,
						  ((POD_DATA_INFO_T*)pv_set_info)->pui1_data,
						  (UINT16)(((POD_DATA_INFO_T*)pv_set_info)->z_data_len)) != POD_OK)
			{
			    CI_DEBUG("Pcmcia_Set(CI_SET_TYPE_WRITE_DATA): error\n");
				return RMR_DRV_ERROR;
			}
			break;
		case CI_SET_TYPE_ACCESS_MODE:	/* may not be needed? */
            if (PCMCIA_SetPodCiAccessMode(*(POD_ACCESS_MODE_T *)pv_set_info) != POD_OK)
            {
                CI_DEBUG("Pcmcia_Set(CI_SET_TYPE_ACCESS_MODE): error\n");
                return RMR_DRV_ERROR;
            }
			break;
		case CI_SET_TYPE_WRITE_REG:
		    PCMCIA_SetPodCiAccessMode(POD_DATA_CHANNEL);
		    PCMCIA_PcmciaToPod(TRUE);

			if (PCMCIA_WriteReg(POD_SDA,
							 ((POD_REG_DATA_INFO_T*)pv_set_info)->ui1_reg_addr,
							 ((POD_REG_DATA_INFO_T*)pv_set_info)->t_data_info.pui1_data,
							 (UINT16)(((POD_REG_DATA_INFO_T*)pv_set_info)->t_data_info.z_data_len)) != POD_OK)
			{
			    CI_DEBUG("Pcmcia_Set(CI_SET_TYPE_WRITE_REG): error\n");
				return RMR_DRV_ERROR;
			}
			break;
		case CI_SET_TYPE_WRITE_COR:
            if (PCMCIA_WriteCor(((POD_COR_INFO_T*)pv_set_info)->ui2_cor_address,
							 ((POD_COR_INFO_T*)pv_set_info)->ui1_cor_value) != POD_OK)
			{
			    CI_DEBUG("Pcmcia_Set(CI_SET_TYPE_WRITE_COR): error\n");
			    return RMR_DRV_ERROR;
			}
			break;
		case CI_SET_TYPE_RESET:
            /*if (*(BOOL*)pv_set_info == TRUE)
            {
                i4_return_value = POD_Reset(TRUE);
            }
			else
            {
                i4_return_value = POD_Reset(FALSE);
            }*/
            i4_return_value = PCMCIA_Reset(*(BOOL*)pv_set_info);

            if (i4_return_value != POD_OK)
            {
                CI_DEBUG("Pcmcia_Set(CI_SET_TYPE_RESET): error\n");
            	return RMR_DRV_ERROR;
            }
			break;
        case CI_SET_TYPE_RESET_CAM:
			PCMCIA_ResetCam();
        	break;
        	
        case CI_SET_TYPE_INTERRUPT:
			PCMCIA_RegIsr(MT8295_ISR_IREQ, (x_os_isr_fct) pv_set_info, NULL);
			break;        	
        	
        default:
            CI_DEBUG("Pcmcia_Set(): nothing has been done\n");
            return RMR_DRV_INV_SET_INFO;
    }

    return RMR_OK;
}

//-----------------------------------------------------------------------------
/** This API initializes PCMCIA/POD/CI/MPOD Middleware adaptation layer.
 */
//-----------------------------------------------------------------------------
INT32 MW_PcmciaInit(void)
{
    INT32       i4_return;

    /* Setup POD component ID */
    t_pod_comp_reg.e_type           = DRVT_CI;
    t_pod_comp_reg.e_id_type        = ID_TYPE_IND;
    t_pod_comp_reg.u.t_ind.ui2_id   = DRV_CI_ID;
    t_pod_comp_reg.u.t_ind.pv_tag   = &ui4_pod_comp_tag;
    t_pod_comp_reg.u.t_ind.ui1_port = 0;

    /* Setup POD component function table */
    t_pod_comp_fct_tbl.pf_rm_connect 	= NULL;
    t_pod_comp_fct_tbl.pf_rm_disconnect = NULL;
    t_pod_comp_fct_tbl.pf_rm_get 		= Pcmcia_Get;
    t_pod_comp_fct_tbl.pf_rm_set 		= Pcmcia_Set;

    /* Register POD component with Resource Manager database */
    i4_return = x_rm_reg_comp(&t_pod_comp_reg,					// component handle
                              1,								// num of input ports
                              1,								// num of output ports
                              (const CHAR *)&ac_pod_comp_name,	// component name
                              0,								// component flags
                              &t_pod_comp_fct_tbl,				// function table
                              NULL,								// component specific data
                              0);								// component specific data length

    if (i4_return != RMR_OK)
    {
        CI_DEBUG("MW_PcmciaInit(): Register POD component fail\n");
    }

    return i4_return;
}

#endif    /* ifndef CI_SUPPORT_DVBT_BYPASS */

