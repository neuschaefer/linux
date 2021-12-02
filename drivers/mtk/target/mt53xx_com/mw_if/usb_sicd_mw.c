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
 
/** @file usb_sicd_mw.c
 *  This C file implements the block device interface for middleware.
 */
#ifndef CC_USB_DISABLE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "mu_hfi_sicd_if.h"
#include "usb_sicd_mw.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** MUSB_SicdGetFct
 *  USB SICD device get function registered with Resource Manager.
 *  @param      See Resource Manager specification.
 *  @retval       RMR_OK    Success.
 *  @retval       RMR_DRV_NOT_ENOUGH_SPACE    The given buffer does not have enough space.
 *  @retval       RMR_DRV_GET_FAILED   Other errors.
 */
//-------------------------------------------------------------------------
static INT32 MUSB_SicdGetFct(DRV_COMP_ID_T *pt_comp_id, DRV_GET_TYPE_T e_get_type, 
                            VOID *pv_get_info, SIZE_T *pz_get_info_len)
{
    INT32 i4_ret = RMR_OK;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_SICD_REQUEST_T *pSicdRequest = NULL;
    MUSB_HFI_SICD_DEV_STATUS_E *pSicdDevStatus = NULL;
    MUSB_HfiDeviceInfo *pHfiDevInfo = NULL;
    INT32 dwStatusCode = MUSB_SICD_RM_RT_OK;
    
    if ((pt_comp_id == NULL) || (pv_get_info == NULL) || (pz_get_info_len == NULL))
    {
        return RMR_DRV_GET_FAILED;
    }
    
    pHfiDriver = (MU_FS_DEVICE_TYPE *)pt_comp_id->pv_tag;
    pSicdRequest = (MUSB_SICD_REQUEST_T*)pv_get_info;
    pSicdRequest->dwReturn = MUSB_SICD_RM_RT_OK;

    switch (e_get_type)
    {
    case SICDDEV_GET_TYPE_ALIGNMENT: 
        pSicdRequest->uContent.dwAlignment = SICD_DEF_ALIGNMENT;
        break;
        
    case SICDDEV_GET_TYPE_DEVICE_STATUS: 
        pSicdDevStatus = &(pSicdRequest->uContent.eDeviceStatus);
        dwStatusCode = (INT32)pHfiDriver->pfIoCtl(pt_comp_id->ui2_id, 
                    (UINT32)MUSB_SICD_IOCTL_GET_DEVICE_STATUS, 0, pSicdDevStatus);
        break;
        
    case SICDDEV_GET_TYPE_DEVICE_INFO: 
        pHfiDevInfo = &(pSicdRequest->uContent.tDeviceInfo);
        dwStatusCode = (INT32)pHfiDriver->pfIoCtl(pt_comp_id->ui2_id, 
                    (UINT32)MUSB_SICD_IOCTL_GET_DEVICE_INFO, 0, &pHfiDevInfo);
        break;

    case SICDDEV_GET_TYPE_DEVICE_CAPABILITY: 
        // TODO
        ASSERT(0);
        break;

    default:
        ASSERT(0);
        break;
    }
    
    if ( dwStatusCode != MUSB_SICD_RM_RT_OK )
    {
        pSicdRequest->dwReturn = (int32_t)dwStatusCode;
        i4_ret = RMR_DRV_GET_FAILED;
    }
    
    UNUSED(pt_comp_id);
    return i4_ret;
}

//-------------------------------------------------------------------------
/** MUSB_SicdSetFct
 *  USB SICD set function registered with Resource Manager.
 *  @param      See Resource Manager specification.
 *  @retval       RMR_OK    Success.
 *  @retval       RMR_DRV_NOT_ENOUGH_SPACE    The given buffer does not have enough space.
 *  @retval       RMR_DRV_INV_SET_INFO   Other errors.
 */
//-------------------------------------------------------------------------
static INT32 MUSB_SicdSetFct(DRV_COMP_ID_T *pt_comp_id, DRV_SET_TYPE_T e_set_type,
                            const VOID *pv_set_info, SIZE_T z_set_info_len)
{
    INT32 i4_ret = RMR_OK;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_SICD_REQUEST_T *pSicdRequest = NULL;
    MUSB_SICD_IO_DATA_T *pSicdIoData = NULL;
    MUSB_NFY_INFO_T     *pSicdNfyInfo = NULL;
    MUSB_HOTSWAP_NFY_INFO_T *pSicdHotswapNfyInfo = NULL;
    UINT32 dwStatusCode = MUSB_SICD_RM_RT_OK;
    
    if ((pt_comp_id == NULL) || (pv_set_info == NULL))
    {
        return RMR_DRV_SET_FAILED;
    }

    pHfiDriver = (MU_FS_DEVICE_TYPE *)pt_comp_id->pv_tag;
    pSicdRequest = (MUSB_SICD_REQUEST_T*)pv_set_info;
    pSicdRequest->dwReturn = MUSB_SICD_RM_RT_OK;

    switch (e_set_type)
    {
    case SICDDEV_SET_TYPE_SEND_OPERATION:
        pSicdIoData = &(pSicdRequest->uContent.tIoData);
        dwStatusCode = pHfiDriver->pfRWBurst(pt_comp_id->ui2_id, 
                                            (UINT32)MUSB_SICD_RW_SEND_OPERATION, 
                                            (void *)pSicdIoData);
        break;
        
    case SICDDEV_SET_TYPE_REGISTER_NFY_FCT:
        pSicdNfyInfo = &(pSicdRequest->uContent.tNfyInfo);
        dwStatusCode = pHfiDriver->pfIoCtl(pt_comp_id->ui2_id, 
                                            (UINT32)MUSB_SICD_IOCTL_SET_TYPENFY, 0,  
                                            (void *)pSicdNfyInfo);
        break;

    case SICDDEV_SET_TYPE_CLEAR_NFY_FCT:
        // TODO
        ASSERT(0);
        break;

    case SICDDEV_SET_TYPE_HOTSWAP_NFY_FCT: 
        pSicdHotswapNfyInfo = &(pSicdRequest->uContent.tHotswapNfyInfo);
        dwStatusCode = pHfiDriver->pfIoCtl(pt_comp_id->ui2_id, 
                                            (UINT32)MUSB_SICD_IOCTL_SET_HOTSWAP_NFY, 0,  
                                            (void *)pSicdHotswapNfyInfo);
        break;
        
    case SICDDEV_SET_TYPE_DEVICE_RESET:
        dwStatusCode = pHfiDriver->pfRWBurst(pt_comp_id->ui2_id, 
                                    (UINT32)MUSB_SICD_RW_SET_DEVICE_RESET, 
                                            NULL);
        break;
        
    case SICDDEV_SET_TYPE_OPERATION_ABORT:
        dwStatusCode = pHfiDriver->pfRWBurst(pt_comp_id->ui2_id, 
                                (UINT32)MUSB_SICD_RW_SET_OPERATION_ABORT, 
                                        NULL);
        break;

    case SICDDEV_SET_TYPE_EXTENDED_EVENT_DATA: 
        dwStatusCode = pHfiDriver->pfRWBurst(pt_comp_id->ui2_id, 
                                    (UINT32)MUSB_SICD_RW_GET_EXTENDED_EVENT_DATA, 
                                    NULL);
        break;

    case SICDDEV_SET_TYPE_GET_NEXT_DATA:
        pSicdIoData = &(pSicdRequest->uContent.tIoData);
        dwStatusCode = pHfiDriver->pfRWBurst(pt_comp_id->ui2_id, 
                                    (UINT32)MUSB_SICD_RW_GET_NEXT_DATA, 
                                    (void *)pSicdIoData);
        break;

    case SICDDEV_SET_TYPE_DATA_PHASE_CONTROLLED_BY_MIDDLEWARE:
        dwStatusCode = pHfiDriver->pfIoCtl(pt_comp_id->ui2_id, 
                                (UINT32)MUSB_SICD_IOCTL_DP_CTRL_BY_MW, 
                                0, NULL);
        break;
        
    case SICDDEV_SET_TYPE_DATA_PHASE_CONTROLLED_BY_SICD:
        dwStatusCode = pHfiDriver->pfIoCtl(pt_comp_id->ui2_id, 
                                (UINT32)MUSB_SICD_IOCTL_DP_CTRL_BY_SICD, 
                                0, NULL);
        break;
    
    case SICDDEV_SET_TYPE_SEND_NEXT_DATA:
        // Todo, send object operation is not ready.
        ASSERT(0);
        break;

    case SICDDEV_SET_TYPE_DATA_SKIP: 
        // TODO
        ASSERT(0);
        break;

    case SICDDEV_SET_TYPE_SWITCH_TO_MSD: 
        // TODO
        ASSERT(0);
        break;

    default: 
        ASSERT(0);
        break;
    }
    
    if ( dwStatusCode != MUSB_SICD_RM_RT_OK )
    {
        pSicdRequest->dwReturn = dwStatusCode;
        i4_ret = RMR_DRV_SET_FAILED;
    }
    
    UNUSED(pt_comp_id);
    return i4_ret;
}
//-------------------------------------------------------------------------
/** _USBSicd_Init
 *  Register USB SICD driver init function to resouce manager.
 *  @param      u4DevFlag    The block device id flag.
 *  @param      szDefName   The block device name string.
 *  @param      drvType         The block device type.
 *  @retval       RMR_OK    Success.
 *  @retval       RMR_INV_ARG    Invalid arguments.
 *  @retval       RMR_DRV_INV_GET_INFO   Other errors.
 */
//-------------------------------------------------------------------------
static INT32 _USBSicd_Init(UINT32 u4Uid, const CHAR *szDefName, DRV_TYPE_T drvType,
                              UINT32 ui4_comp_flags)
{
    DRV_COMP_REG_T t_reg_comp;

    DRV_COMP_FCT_TBL_T t_comp_fct_tbl;
    INT32 i4_ret;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    CHAR ps_name[32] = {0};

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiSicdDriverGet();

    if (!pHfiDriver)
    {
        return RMR_DRV_ERROR;
    }    

    if (szDefName)
    {
        x_sprintf(ps_name, szDefName, u4Uid);
    }

    t_reg_comp.e_type = drvType;
    t_reg_comp.e_id_type = ID_TYPE_IND;
    t_reg_comp.u.t_ind.ui2_id = (UINT16)u4Uid;      
    t_reg_comp.u.t_ind.ui1_port = 0;
    t_reg_comp.u.t_ind.pv_tag = pHfiDriver; 

    t_comp_fct_tbl.pf_rm_connect = NULL;
    t_comp_fct_tbl.pf_rm_disconnect = NULL;
    t_comp_fct_tbl.pf_rm_get = MUSB_SicdGetFct;
    t_comp_fct_tbl.pf_rm_set = MUSB_SicdSetFct;

    i4_ret = x_rm_reg_comp(&t_reg_comp, 1, /* num in port */
                           1,              /* num out port */
                           ((szDefName) ? ps_name : NULL), 
                           ui4_comp_flags,
                           &t_comp_fct_tbl, NULL,
                           0);

    if (i4_ret != RMR_OK)
    {
        return RMR_DRV_ERROR;
    }
    return RMR_OK;

}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** MW_SICD_USB_Init
 *  SICD driver init entry function .
 *  @param      void.
 *  @retval       RMR_OK    Success.
 *  @retval       RMR_DRV_ERROR    Error.
 */
//-------------------------------------------------------------------------
INT32 MW_SICD_USB_Init(void)
{
    INT32 i4Ret = 0;
    UINT32 i = 0;

    for (i=0; i < MUSB_SICD_UNIT_COUNT; i++)
    {
        i4Ret |= _USBSicd_Init(i, DRVN_USB_PTP_MTP, DRVT_USB_PTP_MTP, BLKDEV_FLAG_HOTSWAP); // register to RM.
        ASSERT(i4Ret == RMR_OK);
    }
   
    LINT_SUPPRESS_NEXT_EXPRESSION(774);

    if (i4Ret)
    {
        return RMR_DRV_ERROR;
    }

    return RMR_OK;
}
#else
#include "x_typedef.h"
#include "x_rm.h"
INT32 MW_SICD_USB_Init(void)
{
    return RMR_OK;
}
#endif
