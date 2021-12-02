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
 * $RCSfile: block_mw.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file block_mw.c
 *  This C file implements the block device interface for middleware.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_util.h"
#include "block_mw.h"
#include "u_socket_dev.h"

#ifdef CC_NAND_ENABLE
#include "nand_if.h"
#endif

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
#ifndef BLKDEV_SET_TYPE_HOTSWAP_NFY_FCT
#define BLKDEV_GET_TYPE_STATUS                  200
#define BLKDEV_SET_TYPE_HOTSWAP_NFY_FCT         100
#define BLKDEV_HOTSWAP_NFY_INFO_T               BLKDEV_NFY_INFO_T
#define BLKDEV_STATUS_T                         INT32
#endif

#ifndef BLKDEV_MEMCARD_NONE
#define BLKDEV_MEMCARD_NONE                     ((BLKDEV_MEMCARD_VARIETY_T)(-1))
#endif

#define CF_DEF_NAME                             "cf"
#define CF_MAX_TRANS_SECT                       (((UINT32) 2 * 1024 * 1024) >> 9)
#define HD_DEF_NAME                             "hd"
#define HD_MAX_TRANS_SECT                       (((UINT32) 2 * 1024 * 1024) >> 9)
#define MS_DEF_NAME                             "ms"
// MS update logical to physical address mapping in one segment = 16384 sectors.
#define MS_MAX_TRANS_SECT                       ((UINT32) 16384)
#define SD_DEF_NAME                             "sd"
// FCI support SD multi-blocks = 128 blocks =  in one transaction.
#define SD_MAX_TRANS_SECT                       ((UINT32) 128)
#define SMXD_DEF_NAME                           "smxd"
// SM/xD update logical to physical mapping table in every zone = 32000 sectors.
#define SMXD_MAX_TRANS_SECT                     ((UINT32) 32000)
#define NAND_DEF_NAME                           "nand"
#define NAND_MAX_TRANS_SECT                     (((UINT32) 2 * 1024) >> 9)

#define NAND_BLOCK_NUM                          (2048)
#define NAND_BLOCK_SIZE                         ((UINT32) 64 * 1024)

#define BLOCK_ERROR_UID                         0xffff


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
#ifdef CC_FCI_ENABLE
SKTDEV_DEV_ST_NFY_T rFciSocketNfyInfo = {0};
#endif

#if defined(CC_IDE_ENABLE) || defined(CC_FCI_ENABLE) || defined(CC_NAND_ENABLE)
static BLKDEV_NFY_INFO_T _rCFNfyInfo =
{
    NULL, NULL
};

static BLKDEV_NFY_INFO_T _rMSNfyInfo =
{
    NULL, NULL
};

static BLKDEV_NFY_INFO_T _rSDNfyInfo =
{
    NULL, NULL
};

static BLKDEV_NFY_INFO_T _rSMXDNfyInfo =
{
    NULL, NULL
};

static BLKDEV_NFY_INFO_T _rHDNfyInfo =
{
    NULL, NULL
};

static BLKDEV_HOTSWAP_NFY_INFO_T _rCFHotswapNfyInfo =
{
    NULL, NULL
};

static BLKDEV_HOTSWAP_NFY_INFO_T _rMSHotswapNfyInfo =
{
    NULL, NULL
};

static BLKDEV_HOTSWAP_NFY_INFO_T _rSDHotswapNfyInfo =
{
    NULL, NULL
};

static BLKDEV_HOTSWAP_NFY_INFO_T _rSMXDHotswapNfyInfo =
{
    NULL, NULL
};

static CHAR *_szBlockEmptyModel = "EMPTY";

#ifdef CC_NAND_ENABLE
    static CHAR *_szBlockNandModel  = "NAND FLASH";
    static INT32 _i4NANDPartNum;
    static BLKDEV_NFY_INFO_T *_arNANDNfyInfo;
#endif
#endif

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
#ifndef CC_USB_DISABLE
//-------------------------------------------------------------------------
/** _USBMsd_GetFct
 *  USB Block device get function registered with Resource Manager.
 *  @param      See Resource Manager specification.
 *  @retval       RMR_OK    Success.
 *  @retval       RMR_DRV_NOT_ENOUGH_SPACE    The given buffer does not have enough space.
 *  @retval       RMR_DRV_INV_GET_INFO   Other errors.
 */
//-------------------------------------------------------------------------
static INT32 _USBMsd_GetFct(DRV_COMP_ID_T *pt_comp_id, DRV_GET_TYPE_T e_get_type,
                            VOID *pv_get_info, SIZE_T *pz_get_info_len)
{
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    INT32 i4_strlen;
    INT32 i4_ret = RMR_DRV_INV_GET_INFO;
    UINT64 u8SectorNum;
    MUSB_HfiMediumInfo *pMediumInfo;
    MUSB_HfiDeviceInfo *pDeviceInfo;
    BLKDEV_STATUS_T rStatus;
    CHAR *pcRet = NULL;

    if ((pt_comp_id == NULL) || (pv_get_info == NULL) || (pz_get_info_len == NULL))
    {
        return RMR_DRV_INV_GET_INFO;
    }

    pHfiDriver = (MU_FS_DEVICE_TYPE *)pt_comp_id->pv_tag;

    switch (e_get_type)
    {
    case BLKDEV_GET_TYPE_SIZE:
    case BLKDEV_GET_TYPE_NUMBER_OF_SECTORS:
        if (*pz_get_info_len < sizeof(UINT64))
        {
            *pz_get_info_len = sizeof(UINT64);
            return RMR_NOT_ENOUGH_SPACE;
        }

        // get medium information.
        if (0 > pHfiDriver->pfIoCtl(pt_comp_id->ui2_id, (UINT32)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
        {
            return RMR_DRV_INV_GET_INFO;
        }

        u8SectorNum = MUSB_MAKE64(pMediumInfo->dwBlockCountLo, pMediumInfo->dwBlockCountHi);

        if (e_get_type == BLKDEV_GET_TYPE_SIZE)
        {
            *(UINT64 *)pv_get_info = u8SectorNum * pMediumInfo->dwBlockSize;
        }
        else
        {
            *(UINT64 *)pv_get_info = u8SectorNum;
        }
        *pz_get_info_len = sizeof(UINT64);
        i4_ret = RMR_OK;
        break;

    case BLKDEV_GET_TYPE_SECTOR_SIZE:
    case BLKDEV_GET_TYPE_MAX_TRANS_SECTORS:
    case BLKDEV_GET_TYPE_MAX_IO_REQUEST:
    case BLKDEV_GET_TYPE_ALIGNMENT:
    case BLKDEV_GET_TYPE_WRITE_PROTECT:
        if (*pz_get_info_len < sizeof(UINT32))
        {
            *pz_get_info_len = sizeof(UINT32);
            return RMR_NOT_ENOUGH_SPACE;
        }
        
        if (e_get_type == BLKDEV_GET_TYPE_SECTOR_SIZE)
        {
            // get medium information.
            if (0 > pHfiDriver->pfIoCtl(pt_comp_id->ui2_id, (UINT32)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
            {
                return RMR_DRV_INV_GET_INFO;
            }

            *(UINT32 *)pv_get_info = pMediumInfo->dwBlockSize;
        }
        else if (e_get_type == BLKDEV_GET_TYPE_MAX_TRANS_SECTORS)
        {
            *(UINT32 *)pv_get_info = (MUSB_HFI_MAX_SECTOR_NO - 1);
        }
        else if (e_get_type == BLKDEV_GET_TYPE_MAX_IO_REQUEST)
        {
            *(UINT32 *)pv_get_info = MUSB_HFI_MAX_IO_REQUEST;
        }
        else if (e_get_type == BLKDEV_GET_TYPE_ALIGNMENT)
        {
            *(UINT32 *)pv_get_info = BLOCK_DEF_ALIGNMENT; /* 4 bytes alignment. */
        }
        else if (e_get_type == BLKDEV_GET_TYPE_WRITE_PROTECT)
        {
            // get medium information.
            if (0 > pHfiDriver->pfIoCtl(pt_comp_id->ui2_id, (UINT32)MUSB_IOCTL_GET_MEDIUM_INFO, 0, &pMediumInfo))
            {
                return RMR_DRV_INV_GET_INFO;
            }

            *(UINT32 *)pv_get_info = pMediumInfo->AccessType;
        }
        else
        {
            return i4_ret;
        }
        
        *pz_get_info_len = sizeof(UINT32);
        i4_ret = RMR_OK;
        break;

    case BLKDEV_GET_TYPE_STATUS:
        if (*pz_get_info_len < sizeof(BLKDEV_STATUS_T))
        {
            *pz_get_info_len = sizeof(BLKDEV_STATUS_T);
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }

        // check device status.
        if (0 > pHfiDriver->pfIoCtl(pt_comp_id->ui2_id, (UINT32)MUSB_IOCTL_GET_DEVICE_STATUS, 0, &rStatus))
        {
            return RMR_DRV_INV_GET_INFO;
        }

        *(BLKDEV_STATUS_T *)pv_get_info = rStatus;

        *pz_get_info_len = sizeof(BLKDEV_STATUS_T);
        i4_ret = RMR_OK;
        break;

    case BLKDEV_GET_TYPE_MEMCARD_VARIETY:
        if (*pz_get_info_len < sizeof(BLKDEV_MEMCARD_VARIETY_T))
        {
            *pz_get_info_len = sizeof(BLKDEV_MEMCARD_VARIETY_T);
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }

        *(BLKDEV_MEMCARD_VARIETY_T *)pv_get_info = BLKDEV_USB_MASS;
        *pz_get_info_len = sizeof(BLKDEV_MEMCARD_VARIETY_T);
        i4_ret = RMR_OK;
        break;

    case BLKDEV_GET_TYPE_MODEL:
        // get device information.
        if (0 > pHfiDriver->pfIoCtl(pt_comp_id->ui2_id, (UINT32)MUSB_IOCTL_GET_DEVICE_INFO, 0, &pDeviceInfo))
        {
            return RMR_DRV_INV_GET_INFO;
        }

        i4_strlen = (INT32)x_strlen((const CHAR *)pDeviceInfo->pDiskProduct) + 1;
        pcRet = x_strncpy((CHAR *)pv_get_info, (const CHAR *)pDeviceInfo->pDiskProduct, *pz_get_info_len);
        UNUSED(pcRet);

        ((CHAR *)pv_get_info)[*pz_get_info_len - 1] = 0;
        *pz_get_info_len = (UINT32)i4_strlen;
        i4_ret = RMR_OK;
        break;

    default:
        ASSERT(0);
        /* unknown get type */
        break;
    }

    UNUSED(pt_comp_id);
    return i4_ret;
}

//-------------------------------------------------------------------------
/** _USBMsd_SetFct
 *  USB Block set function registered with Resource Manager.
 *  @param      See Resource Manager specification.
 *  @retval       RMR_OK    Success.
 *  @retval       RMR_DRV_NOT_ENOUGH_SPACE    The given buffer does not have enough space.
 *  @retval       RMR_DRV_INV_GET_INFO   Other errors.
 */
//-------------------------------------------------------------------------
static INT32 _USBMsd_SetFct(DRV_COMP_ID_T *pt_comp_id, DRV_SET_TYPE_T e_set_type,
                            const VOID *pv_set_info, SIZE_T z_set_info_len)
{
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    MUSB_IO_DATA_T *pt_data;
    INT32 i4_ret = RMR_DRV_INV_SET_INFO;
    uint32_t  u4Optype;

    if ((pt_comp_id == NULL) || (pv_set_info == NULL))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    pHfiDriver = (MU_FS_DEVICE_TYPE *)pt_comp_id->pv_tag;

    switch (e_set_type)
    {
    case BLKDEV_SET_TYPE_READ:
    case BLKDEV_SET_TYPE_WRITE:
        if (z_set_info_len < sizeof(MUSB_IO_DATA_T))
        {
            return RMR_NOT_ENOUGH_SPACE;
        }
        
        pt_data = (MUSB_IO_DATA_T *)pv_set_info;
        if (((UINT32)pt_data->pvData & BLOCK_ALIGNMENT_MASK) != 0)
        {
            return RMR_DRV_INV_SET_INFO;
        }

        u4Optype = (uint32_t)((e_set_type == BLKDEV_SET_TYPE_WRITE) ? MUSB_HFI_OP_WRITE : MUSB_HFI_OP_READ);

        if (0 > pHfiDriver->pfRWBurst(pt_comp_id->ui2_id, u4Optype, (void *)pt_data))
        {
            return RMR_DRV_SET_FAILED;
        }
        else
        {
            i4_ret = RMR_ASYNC_NFY;
        }
        break;

    case BLKDEV_SET_TYPE_NFY_FCT:
        if (z_set_info_len < sizeof(BLKDEV_NFY_INFO_T))
        {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }

        if (0 > pHfiDriver->pfIoCtl(pt_comp_id->ui2_id, (UINT32)MUSB_IOCTL_SET_TYPENFY, 0, (void *)pv_set_info))
        {
            return RMR_DRV_INV_SET_INFO;
        }

        i4_ret = RMR_OK;
        break;

    case BLKDEV_SET_TYPE_HOTSWAP_NFY_FCT:
        ASSERT(0);
        i4_ret = RMR_OK;
        break;
        
    default:
        /* unknown set type */
        break;
    }

    UNUSED(pt_comp_id);

    return i4_ret;
}

//-------------------------------------------------------------------------
/** _USBMsd_Init
 *  Register USB Mass storage driver init function to resouce manager.
 *  @param      u4DevFlag    The block device id flag.
 *  @param      szDefName   The block device name string.
 *  @param      drvType         The block device type.
 *  @retval       RMR_OK    Success.
 *  @retval       RMR_INV_ARG    Invalid arguments.
 *  @retval       RMR_DRV_INV_GET_INFO   Other errors.
 */
//-------------------------------------------------------------------------
static INT32 _USBMsd_Init(UINT32 u4Uid, const CHAR *szDefName, DRV_TYPE_T drvType,
                              UINT32 ui4_comp_flags)
{
    DRV_COMP_REG_T t_reg_comp;

    DRV_COMP_FCT_TBL_T t_comp_fct_tbl;
    INT32 i4_ret;
    MU_FS_DEVICE_TYPE *pHfiDriver = NULL;
    CHAR ps_name[32] = {0};

    // get Hfi driver structure.
    pHfiDriver = MUSB_HfiDriverGet();

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
    t_comp_fct_tbl.pf_rm_get = _USBMsd_GetFct;
    t_comp_fct_tbl.pf_rm_set = _USBMsd_SetFct;

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
#endif

#if defined(CC_IDE_ENABLE) || defined(CC_FCI_ENABLE) || defined(CC_NAND_ENABLE)
//-------------------------------------------------------------------------
/** _MwblkGetModel
 *  Get model name.
 *  @param  u4DevFlag   device flag.
 *  @return  the string of model name.
 */
//-------------------------------------------------------------------------
static CHAR *_MwblkGetModel(UINT32 u4DevFlag)
{
    if (u4DevFlag == STORG_FLAG_NAND)
    {
#ifdef CC_NAND_ENABLE
        return _szBlockNandModel;
#else
        ASSERT(0);
#endif
    }

    return STORG_GetDevModel(u4DevFlag);
}

//-------------------------------------------------------------------------
/** _MwblkGetUid
 *  Perform device flag and device ID mapping function.
 *  @param  u4DevFlag   device flag.
 *  @return  device ID
 */
//-------------------------------------------------------------------------
static UINT16 _MwblkGetUid(UINT32 u4DevFlag)
{
    switch (u4DevFlag)
    {
    case STORG_FLAG_CF:
        return 0;

    case STORG_FLAG_MS:
        return 1;

    case STORG_FLAG_SD:
        return 2;

    case STORG_FLAG_SMXD:
        return 3;

    case STORG_FLAG_HD:
    case STORG_FLAG_NAND:
        return 0;

    default:
        ASSERT(0);
    }

    LINT_SUPPRESS_NEXT_EXPRESSION(527);
    return BLOCK_ERROR_UID;
}

//-------------------------------------------------------------------------
/** _MwblkGetNfyInfo
 *  Get the data structure of block device notify information.
 *  @param  u4DevFlag   device flag.
 *  @return  address of the data structure of block device notify function.
 */
//-------------------------------------------------------------------------
static BLKDEV_NFY_INFO_T *_MwblkGetNfyInfo(UINT32 u4DevFlag, UINT16 ui2_id)
{
    switch (u4DevFlag)
    {
    case STORG_FLAG_CF:
        return &_rCFNfyInfo;

    case STORG_FLAG_MS:
        return &_rMSNfyInfo;

    case STORG_FLAG_SD:
        return &_rSDNfyInfo;

    case STORG_FLAG_SMXD:
        return &_rSMXDNfyInfo;

    case STORG_FLAG_HD:
        return &_rHDNfyInfo;
        
#ifdef CC_NAND_ENABLE
    case STORG_FLAG_NAND:
        ASSERT(ui2_id < _i4NANDPartNum);
        if (ui2_id < _i4NANDPartNum)
        {
            return &_arNANDNfyInfo[ui2_id];
        }
#endif

    default:
        ASSERT(0);
    }

    LINT_SUPPRESS_NEXT_EXPRESSION(527);
    return NULL;
}

//-------------------------------------------------------------------------
/** _MwblkGetHotswapNfyInfo
 *  Get the data structure of block device hot swap notify information.
 *  @param  u4DevFlag   device flag.
 *  @return  address of the data structure of block device hot swap notify information.
 */
//-------------------------------------------------------------------------
static BLKDEV_HOTSWAP_NFY_INFO_T *_MwblkGetHotswapNfyInfo(UINT32 u4DevFlag)
{
    switch (u4DevFlag)
    {
    case STORG_FLAG_CF:
        return &_rCFHotswapNfyInfo;

    case STORG_FLAG_MS:
        return &_rMSHotswapNfyInfo;

    case STORG_FLAG_SD:
        return &_rSDHotswapNfyInfo;

    case STORG_FLAG_SMXD:
        return &_rSMXDHotswapNfyInfo;

    case STORG_FLAG_HD:
    case STORG_FLAG_NAND:
        return NULL;

    default:
        ASSERT(0);
    }

    LINT_SUPPRESS_NEXT_EXPRESSION(527);
    return NULL;
}

//-------------------------------------------------------------------------
/** _MwblkGetSize
 *  Get device size.
 *  @param  u4DevFlag   device flag.
 *  @param  ui2_id          only use in NOR device. Nor Partition id.
 *  @return  device size in bytes. (use 64 bits to store)
 */
//-------------------------------------------------------------------------
static UINT64 _MwblkGetSize(UINT32 u4DevFlag, UINT16 ui2_id)
{
    return STORG_GetDevSize(u4DevFlag, ui2_id);
}

//-------------------------------------------------------------------------
/** _MwblkGetMaxTransSect
 *  Get the max number of sectors per transaction.
 *  @param  u4DevFlag   device flag.
 *  @return  Max number of sectors per transaction.
 */
//-------------------------------------------------------------------------
static UINT32 _MwblkGetMaxTransSect(UINT32 u4DevFlag)
{
    switch (u4DevFlag)
    {
    case STORG_FLAG_CF:
        return CF_MAX_TRANS_SECT;

    case STORG_FLAG_MS:
        return MS_MAX_TRANS_SECT;

    case STORG_FLAG_SD:
        return SD_MAX_TRANS_SECT;

    case STORG_FLAG_SMXD:
        return SMXD_MAX_TRANS_SECT;

    case STORG_FLAG_HD:
        return HD_MAX_TRANS_SECT;

    case STORG_FLAG_NAND:
        return NAND_MAX_TRANS_SECT;

    default:
        ASSERT(0);
    }

    LINT_SUPPRESS_NEXT_EXPRESSION(527);
    return 0;
}

//-------------------------------------------------------------------------
/** _MwblkGetStatus
 *  Get device status which define in STORG_DEV_STATUS_T.
 *  @param  u4DevFlag   device flag.
 *  @return  one value in STORG_DEV_STATUS_T.
 */
//-------------------------------------------------------------------------
static STORG_DEV_STATUS_T _MwblkGetStatus(UINT32 u4DevFlag)
{
    return STORG_GetDevStatus(u4DevFlag);
}

//-------------------------------------------------------------------------
/** _MwblkHotswapNfyFct
 *  Perform the status routing to Middleware when device is hot swapped.
 *  @param  pvNfyTag   point to Middleware registered notify tag.
 *  @param  eNfyCond   notify status in STORG_COND_T.
 *  @param  pvData   reserve data pointer.
 *  @return  void
 */
//-------------------------------------------------------------------------
static void _MwblkHotswapNfyFct(void *pvNfyTag, STORG_COND_T eNfyCond, void *pvData)
{
    BLKDEV_HOTSWAP_NFY_INFO_T *pt_hotswap_nfy_info;
    BLKDEV_STATUS_T eBlkDevStatus;

    UNUSED(pvData);
    pt_hotswap_nfy_info = (BLKDEV_HOTSWAP_NFY_INFO_T *)pvNfyTag;

    switch (eNfyCond)
    {
    case COND_INSERT_OK:
        eBlkDevStatus = BLKDEV_STATUS_READY;
        break;

    case COND_REMOVE_OK:
        eBlkDevStatus = BLKDEV_STATUS_ABSENT;
        break;

    case COND_HOTSWAP_FAIL:
        eBlkDevStatus = BLKDEV_STATUS_FAIL;
        break;

    default:
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return;
    }

    if (pt_hotswap_nfy_info)
    {
        pt_hotswap_nfy_info->pf_blkdev_nfy(pt_hotswap_nfy_info->pv_tag, eBlkDevStatus, NULL);
    }
}

//-------------------------------------------------------------------------
/** _MwblkGetCardVariety
 *  Get card type.
 *  @param  u4DevFlag   device flag.
 *  @return  card type ID in BLKDEV_MEMCARD_VARIETY_T.
 */
//-------------------------------------------------------------------------
static BLKDEV_MEMCARD_VARIETY_T _MwblkGetCardVariety(UINT32 u4DevFlag)
{
    INT32 i4CardType;

    switch (u4DevFlag)
    {
    case STORG_FLAG_CF:
        return BLKDEV_MEMCARD_CF;

    case STORG_FLAG_MS:
        i4CardType = STORG_GetDevCard(u4DevFlag);
        if (i4CardType == FCI_MS_CARD)
        {
            return BLKDEV_MEMCARD_MS;
        }
        
        if (i4CardType == FCI_MSPRO_CARD)
        {
            return BLKDEV_MEMCARD_MS_PRO;
        }
        
        break;

    case STORG_FLAG_SD:
        i4CardType = STORG_GetDevCard(u4DevFlag);
        if (i4CardType == FCI_SD_CARD || i4CardType == FCI_SDHC_CARD)
        {
            return BLKDEV_MEMCARD_SD;
        }
        
        if (i4CardType == FCI_MMC_CARD)
        {
            return BLKDEV_MEMCARD_MMC;
        }
        
        break;

    case STORG_FLAG_SMXD:
        i4CardType = STORG_GetDevCard(u4DevFlag);
        if (i4CardType == FCI_SM_CARD)
        {
            return BLKDEV_MEMCARD_SM;
        }
        
        if (i4CardType == FCI_XD_CARD)
        {
            return BLKDEV_MEMCARD_XD;
        }
        
        break;

    default:
        ASSERT(0);
    }

    LINT_SUPPRESS_NEXT_EXPRESSION(527);
    return BLKDEV_MEMCARD_NONE;
}


//lint -save -e818
//-------------------------------------------------------------------------
/** _Mwblkget_fct
 *  Block device get function registered with Resource Manager.
 *  @param      See Resource Manager specification.
 *  @retval       RMR_OK    Success.
 *  @retval       RMR_DRV_NOT_ENOUGH_SPACE    The given buffer does not have enough space.
 *  @retval       RMR_DRV_INV_GET_INFO   Other errors.
 */
//-------------------------------------------------------------------------
static INT32 _Mwblkget_fct(DRV_COMP_ID_T *pt_comp_id, DRV_GET_TYPE_T e_get_type,
                            VOID *pv_get_info, SIZE_T *pz_get_info_len)
{
    BLKDEV_MEMCARD_VARIETY_T t_blk_variety;

    STORG_DEV_T *prDev;
    CHAR *pszModel;
    INT32 i4_strlen, i4_ret = RMR_DRV_INV_GET_INFO;
    UINT64 u8DevSize = 0;

    if ((pt_comp_id == NULL) || (pv_get_info == NULL) || (pz_get_info_len == NULL))
    {
        return RMR_DRV_INV_GET_INFO;
    }

    prDev = (STORG_DEV_T *)pt_comp_id->pv_tag;

    switch (e_get_type)
    {
    case BLKDEV_GET_TYPE_SIZE:
    case BLKDEV_GET_TYPE_NUMBER_OF_SECTORS:
        if (*pz_get_info_len < sizeof(UINT64))
        {
            *pz_get_info_len = sizeof(UINT64);
            return RMR_NOT_ENOUGH_SPACE;
        }

        u8DevSize = _MwblkGetSize(prDev->u4DevFlag, pt_comp_id->ui2_id);
        
        if (e_get_type == BLKDEV_GET_TYPE_SIZE)
        {
            if (prDev->u4DevFlag == STORG_FLAG_MS || prDev->u4DevFlag == STORG_FLAG_SD)
            {
                *(UINT64 *)pv_get_info = u8DevSize * 512;   // u8DevSize : the card size. Unit = sector = 512 bytes.
            }
            else
            {
                *(UINT64 *)pv_get_info = u8DevSize;
            }
        }
        else
        {
            if (prDev->u4DevFlag == STORG_FLAG_MS || prDev->u4DevFlag == STORG_FLAG_SD)
            {
                *(UINT64 *)pv_get_info = u8DevSize;
            }
            else
            {
#ifdef CC_NAND_ENABLE
                *(UINT64 *)pv_get_info = u8Div6432(u8DevSize, NANDPART_GetPageSize(pt_comp_id->ui2_id), NULL);
#else
                ASSERT(0);
#endif
            }
        }
        
        *pz_get_info_len = sizeof(UINT64);
        i4_ret = RMR_OK;
        break;

    case BLKDEV_GET_TYPE_SECTOR_SIZE:
    case BLKDEV_GET_TYPE_MAX_TRANS_SECTORS:
    case BLKDEV_GET_TYPE_MAX_IO_REQUEST:
    case BLKDEV_GET_TYPE_ALIGNMENT:
    case BLKDEV_GET_TYPE_STANDBY:
        if (*pz_get_info_len < sizeof(UINT32))
        {
            *pz_get_info_len = sizeof(UINT32);
            return RMR_NOT_ENOUGH_SPACE;
        }
        
        if (e_get_type == BLKDEV_GET_TYPE_SECTOR_SIZE)
        {
             if (prDev->u4DevFlag == STORG_FLAG_MS || prDev->u4DevFlag == STORG_FLAG_SD)
             {
                *(UINT64 *)pv_get_info = 512;
             }
             else
             {
#ifdef CC_NAND_ENABLE
                *(UINT32 *)pv_get_info = (UINT32)NANDPART_GetPageSize(pt_comp_id->ui2_id);
#else
                ASSERT(0);
#endif
             }
        }
        else if (e_get_type == BLKDEV_GET_TYPE_MAX_TRANS_SECTORS)
        {
            *(UINT32 *)pv_get_info = _MwblkGetMaxTransSect(prDev->u4DevFlag);
        }
        else if (e_get_type == BLKDEV_GET_TYPE_MAX_IO_REQUEST)
        {
            *(UINT32 *)pv_get_info = 10; /* 10 tentatively. XXX fix me. */
        }
        else if (e_get_type == BLKDEV_GET_TYPE_ALIGNMENT)
        {
            *(UINT32 *)pv_get_info = BLOCK_DEF_ALIGNMENT;
        }
        else if ((e_get_type == BLKDEV_GET_TYPE_STANDBY) && (prDev->u4DevFlag == STORG_FLAG_HD))
        {
            *(UINT32 *)pv_get_info = (UINT32)STORG_GetDevStandbyTime(prDev->u4DevFlag);
        }
        else
        {
            ASSERT(0); /* error, return. */

            LINT_SUPPRESS_NEXT_EXPRESSION(527);
            return i4_ret;
        }
        
        *pz_get_info_len = sizeof(UINT32);
        i4_ret = RMR_OK;
        break;

#ifdef CC_NAND_ENABLE
    case BLKDEV_GET_TYPE_SECTOR_TBL_INFO:
        if (prDev->u4DevFlag == STORG_FLAG_NAND)
        {
            INT32 i4Ret;
            SECTTBL_T *pt_sector_tbl;
            
            if (*pz_get_info_len < sizeof(BLKDEV_SECTOR_TBL_INFO_T))
            {
                *pz_get_info_len = sizeof(BLKDEV_SECTOR_TBL_INFO_T);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            
            pt_sector_tbl = (SECTTBL_T *)pv_get_info;
            
            i4Ret = NANDPART_GetSectorTbl(pt_sector_tbl, pt_comp_id->ui2_id);
            if (i4Ret != 0) { /* not enouth space. */
                *pz_get_info_len = sizeof(BLKDEV_SECTOR_TBL_INFO_T) +
                    (sizeof(BLKDEV_SECTOR_SEG_INFO_T) * (UINT32)i4Ret);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            else
            {
                pt_sector_tbl->u4SegNum = 1;
                *pz_get_info_len = sizeof(BLKDEV_SECTOR_TBL_INFO_T) +
                    (sizeof(BLKDEV_SECTOR_SEG_INFO_T) * pt_sector_tbl->u4SegNum);
                i4_ret = RMR_OK;
            }
        }
        else
        {
            ASSERT(prDev->u4DevFlag == STORG_FLAG_NAND);
            return i4_ret;
        }
        
        break;
#endif

    case BLKDEV_GET_TYPE_STATUS:
        if (*pz_get_info_len < sizeof(BLKDEV_STATUS_T))
        {
            *pz_get_info_len = sizeof(BLKDEV_STATUS_T);
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        
        *(BLKDEV_STATUS_T *)pv_get_info = (BLKDEV_STATUS_T)_MwblkGetStatus(prDev->u4DevFlag);
        *pz_get_info_len = sizeof(BLKDEV_STATUS_T);
        i4_ret = RMR_OK;

        break;

    case BLKDEV_GET_TYPE_MEMCARD_VARIETY:
        if (*pz_get_info_len < sizeof(BLKDEV_MEMCARD_VARIETY_T))
        {
            *pz_get_info_len = sizeof(BLKDEV_MEMCARD_VARIETY_T);
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        
        t_blk_variety = _MwblkGetCardVariety(prDev->u4DevFlag);
        if (t_blk_variety == BLKDEV_MEMCARD_NONE)
        {
            return RMR_DRV_INV_GET_INFO;
        }
        
        *(BLKDEV_MEMCARD_VARIETY_T *)pv_get_info = t_blk_variety;
        *pz_get_info_len = sizeof(BLKDEV_MEMCARD_VARIETY_T);
        i4_ret = RMR_OK;

        break;

    case BLKDEV_GET_TYPE_MODEL:
        pszModel = _MwblkGetModel(prDev->u4DevFlag);
        if (pszModel == NULL)
        {
            pszModel = _szBlockEmptyModel;
        }
        
        i4_strlen = (INT32)x_strlen(pszModel) + 1;
        LINT_SUPPRESS_NEXT_EXPRESSION(534);
        x_strncpy((CHAR *)pv_get_info, pszModel, *pz_get_info_len);
        ((CHAR *)pv_get_info) [*pz_get_info_len - 1] = 0;
        *pz_get_info_len = (UINT32)i4_strlen;
        i4_ret = RMR_OK;
        break;

    default:
        /* unknown get type */
        Printf("unknow block get type:0x%x\n", e_get_type);
        break;
    }

    return i4_ret;
}

//-------------------------------------------------------------------------
/** _Mwblkset_fct
 *  Block set function registered with Resource Manager.
 *  @param      See Resource Manager specification.
 *  @retval       RMR_OK    Success.
 *  @retval       RMR_DRV_NOT_ENOUGH_SPACE    The given buffer does not have enough space.
 *  @retval       RMR_DRV_INV_GET_INFO   Other errors.
 */
//-------------------------------------------------------------------------
static INT32 _Mwblkset_fct(DRV_COMP_ID_T *pt_comp_id, DRV_SET_TYPE_T e_set_type,
                            const VOID *pv_set_info, SIZE_T z_set_info_len)
{
    BLKDEV_IO_DATA_T *pt_data;
    BLKDEV_NFY_INFO_T *pt_nfy_info;
    BLKDEV_HOTSWAP_NFY_INFO_T *pt_hotswap_nfy_info;
    BLKDEV_SECTOR_RANGE_INFO_T *pt_sector_range_info;

    STORG_DEV_T *prDev;
    STORG_OP_T eOp;
    UINT32 ui4_standby;
    INT32 i4Ret, i4_ret = RMR_DRV_INV_SET_INFO;
    UINT64 u8Offset = 0;

    if ((pt_comp_id == NULL) || (pv_set_info == NULL))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    prDev = (STORG_DEV_T *)pt_comp_id->pv_tag;
    if (prDev == NULL)
    {
        ASSERT(prDev != NULL);
        return RMR_DRV_INV_SET_INFO;
    }

    pt_nfy_info = _MwblkGetNfyInfo(prDev->u4DevFlag, pt_comp_id->ui2_id);
    if (pt_nfy_info == NULL)
    {
        ASSERT(pt_nfy_info != NULL);
        return RMR_DRV_INV_SET_INFO;
    }

    pt_hotswap_nfy_info = _MwblkGetHotswapNfyInfo(prDev->u4DevFlag);

    switch (e_set_type)
    {
    case BLKDEV_SET_TYPE_READ:
    case BLKDEV_SET_TYPE_WRITE:
        if (z_set_info_len < sizeof(BLKDEV_IO_DATA_T))
        {
            return RMR_NOT_ENOUGH_SPACE;
        }
        
        pt_data = (BLKDEV_IO_DATA_T *)pv_set_info;
        eOp = ((e_set_type == BLKDEV_SET_TYPE_WRITE) ? STORG_OP_WRITE : STORG_OP_READ);

        if (((UINT32)pt_data->pv_data & BLOCK_ALIGNMENT_MASK) != 0)
        {
            return RMR_DRV_INV_SET_INFO;
        }

        if (prDev->u4DevFlag == STORG_FLAG_MS || prDev->u4DevFlag == STORG_FLAG_SD)
        {
            u8Offset = (pt_data->ui8_blk_num << 9);   // Sector addressing => Byte addressing
        }
        else
        {
#ifdef CC_NAND_ENABLE
            u8Offset = (UINT64)(pt_data->ui8_blk_num * (UINT32)NANDPART_GetPageSize(pt_comp_id->ui2_id));
            u8Offset |= (((UINT64)pt_comp_id->ui2_id) << 32);
#else
            ASSERT(0);
#endif
        } 
        
        i4Ret = STORGSendReq(prDev, eOp,
                             pt_data, pt_nfy_info->pv_tag,
                             (STORG_NFY_FCT)pt_nfy_info->pf_blkdev_nfy, u8Offset,
                             pt_data->ui4_count, pt_data->pv_data,
                             &pt_data->ui4_trans_count);
        if (i4Ret == 0)
        {
            i4_ret = RMR_ASYNC_NFY;
        }
        else
        {
            i4_ret = RMR_DRV_SET_FAILED;
        }
        break;

    case BLKDEV_SET_TYPE_FLUSH:
        if (prDev->u4DevFlag != STORG_FLAG_HD)
        {
            ASSERT(prDev->u4DevFlag == STORG_FLAG_HD);
            return i4_ret;
        }
        
        i4Ret = STORGSendReq(prDev, STORG_OP_FLUSH, NULL, pt_nfy_info->pv_tag,
                             (STORG_NFY_FCT)pt_nfy_info->pf_blkdev_nfy, (UINT64)0, 0, NULL, NULL);
        if (i4Ret == 0)
        {
            i4_ret = RMR_ASYNC_NFY;
        }
        else
        {
            i4_ret = RMR_DRV_SET_FAILED;
        }
        break;

    case BLKDEV_SET_TYPE_NFY_FCT:
        if (z_set_info_len < sizeof(BLKDEV_NFY_INFO_T))
        {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }

        /* Save pt_nfy_info->pv_tag and pt_nfy_info->pf_blkdev_nfy; */
        pt_nfy_info->pf_blkdev_nfy = ((BLKDEV_NFY_INFO_T *)pv_set_info)->pf_blkdev_nfy;
        pt_nfy_info->pv_tag = ((BLKDEV_NFY_INFO_T *)pv_set_info)->pv_tag;

        i4_ret = RMR_OK;
        break;

    case BLKDEV_SET_TYPE_STANDBY:
        if (prDev->u4DevFlag != STORG_FLAG_HD)
        {
            ASSERT(prDev->u4DevFlag == STORG_FLAG_HD);
            return i4_ret;
        }
        
        if (z_set_info_len < sizeof(UINT32))
        {
            return RMR_NOT_ENOUGH_SPACE;
        }
        
        ui4_standby = (UINT32)pv_set_info;
        /* Sync operation. */
        i4Ret = STORGSendReq(prDev, STORG_OP_STDBYTIME, NULL, NULL, NULL, (UINT64)0, ui4_standby, NULL, NULL);
        i4_ret = RMR_OK;
        break;

    case BLKDEV_SET_TYPE_ERASE:
        if (z_set_info_len < sizeof(BLKDEV_SECTOR_RANGE_INFO_T))
        {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }
        
        pt_sector_range_info = (BLKDEV_SECTOR_RANGE_INFO_T *)pv_set_info;
        if (pt_nfy_info->pf_blkdev_nfy == NULL)
        {
            ASSERT(pt_nfy_info->pf_blkdev_nfy != NULL);
            return i4_ret;
        }

        /* call nor async read/wriet function. */
        i4Ret = STORGSendReq(prDev,
                             STORG_OP_ERASE,
                             /*NULL*/pt_sector_range_info,
                             pt_nfy_info->pv_tag,
                             (STORG_NFY_FCT)pt_nfy_info->pf_blkdev_nfy,
                             (UINT64)pt_sector_range_info->ui4_start_sector,
                             pt_sector_range_info->ui4_number_of_sectors,
                             NULL,
                             NULL);
        if (i4Ret == 0)
        {
            i4_ret = RMR_ASYNC_NFY;
        }
        else
        {
            i4_ret = RMR_DRV_SET_FAILED;
        }
        break;

    case BLKDEV_SET_TYPE_HOTSWAP_NFY_FCT:
        if (z_set_info_len < sizeof(BLKDEV_HOTSWAP_NFY_INFO_T))
        {
            return RMR_DRV_NOT_ENOUGH_SPACE;
        }

        if (!pt_hotswap_nfy_info)
        {
            return RMR_DRV_INV_SET_INFO;
        }

        /* Save pt_nfy_info->pv_tag and pt_nfy_info->pf_blkdev_nfy; */
        pt_hotswap_nfy_info->pf_blkdev_nfy = ((BLKDEV_HOTSWAP_NFY_INFO_T *)
                                                  pv_set_info)->pf_blkdev_nfy;
        pt_hotswap_nfy_info->pv_tag = ((BLKDEV_HOTSWAP_NFY_INFO_T *)pv_set_info)->pv_tag;

        prDev->pfnHotswapNfy = (STORG_HOTSWAP_NFY_FCT)_MwblkHotswapNfyFct;
        prDev->pvHotswapTag = (void *)pt_hotswap_nfy_info;

        i4_ret = RMR_OK;
        break;
        
    case BLKDEV_SET_TYPE_IO_CTRL:
        i4_ret = RMR_OK;
        break;
        
    default:
        /* unknown set type */
        break;
    }

    return i4_ret;
}

//lint -restore
//-------------------------------------------------------------------------
/** _MwblkDevInit
 *  Specific Block driver init function to resouce manager.
 *  @param      u4DevFlag    The block device id flag.
 *  @param      szDefName   The block device name string.
 *  @param      drvType         The block device type.
 *  @retval       RMR_OK    Success.
 *  @retval       RMR_INV_ARG    Invalid arguments.
 *  @retval       RMR_DRV_INV_GET_INFO   Other errors.
 */
//-------------------------------------------------------------------------
static INT32 _MwblkDevInit(UINT32 u4DevFlag, CHAR *szDefName, DRV_TYPE_T drvType,
                              UINT32 ui4_comp_flags)
{
    DRV_COMP_REG_T t_reg_comp;

    DRV_COMP_FCT_TBL_T t_comp_fct_tbl;
    CHAR ps_name[32];
    INT32 i4_ret;
    UINT16 ui2_uid;
    STORG_DEV_T *prDev;

    prDev = STORGOpenDev(u4DevFlag);
    if (prDev == NULL)
    {
        return RMR_DRV_ERROR;
    }
    
    if (drvType == DRVT_NAND_FLASH)
    {
        ui2_uid = ui4_comp_flags;
        x_snprintf(ps_name, 16, szDefName, ui2_uid);        
    }
    else
    {
        ui2_uid = _MwblkGetUid(u4DevFlag);

        if (ui2_uid == BLOCK_ERROR_UID)
        {
            return RMR_DRV_ERROR;
        }
        
        x_snprintf(ps_name, 16, "%s", szDefName);
    }

    LINT_SUPPRESS_NEXT_EXPRESSION(534);

    t_reg_comp.e_type = drvType;
    t_reg_comp.e_id_type = ID_TYPE_IND;
    t_reg_comp.u.t_ind.ui2_id = ui2_uid;
    t_reg_comp.u.t_ind.ui1_port = 0;
    t_reg_comp.u.t_ind.pv_tag = prDev; /* storage device pointer. */

    t_comp_fct_tbl.pf_rm_connect = NULL;
    t_comp_fct_tbl.pf_rm_disconnect = NULL;
    t_comp_fct_tbl.pf_rm_get = _Mwblkget_fct;
    t_comp_fct_tbl.pf_rm_set = _Mwblkset_fct;
    
    if (drvType == DRVT_NAND_FLASH)
    {
        i4_ret = x_rm_reg_comp(&t_reg_comp,
                               1,       /* num in port */
                               1,    /* num out port */
                               ps_name,
                               0,//BLKDEV_FLAG_ASYNC_READ | BLKDEV_FLAG_ASYNC_WRITE,
                               &t_comp_fct_tbl,
                               NULL,
                               0);
    }
    else
    {
        i4_ret = x_rm_reg_comp(&t_reg_comp,
                               1,       /* num in port */
                               1,    /* num out port */
                               ps_name,
                               ui4_comp_flags,
                               &t_comp_fct_tbl,
                               NULL,
                               0);

    }
    if (i4_ret != RMR_OK)
    {
        LINT_SUPPRESS_NEXT_EXPRESSION(534);
        STORGCloseDev(prDev);
        return RMR_DRV_ERROR;
    }

    return RMR_OK;
}
#endif

#ifdef CC_FCI_ENABLE
/*-----------------------------------------------------------------------------
 * Name: _fci_skt_get_fct
 *
 * Description: Storage device get function registered with Resource Manager.
 *
 * Inputs:  See Resource Manager specification.
 *
 * Outputs: -
 *
 * Returns: RMR_OK
 *              Success.
 *          RMR_DRV_NOT_ENOUGH_SPACE
 *              The given buffer does not have enough space.
 *          RMR_DRV_INV_GET_INFO
 *              Parameters are invalid or get type is not supported.
 *          RMR_DRV_GET_FAILED
 *              device is detached or get command is failed.
 ----------------------------------------------------------------------------*/
static INT32 _fci_skt_get_fct(
    DRV_COMP_ID_T  *pt_comp_id,
    DRV_GET_TYPE_T  e_get_type,
    VOID           *pv_get_info,
    SIZE_T         *pz_get_info_len)
{
    INT32 i4_ret = RMR_DRV_INV_GET_INFO;
    
    if ((pt_comp_id == NULL) || (pv_get_info == NULL) || (pz_get_info_len == NULL))
    {
        return RMR_DRV_INV_GET_INFO;
    }

    printf("\n[FCI] e_get_type=%d, e_type=%d, ui2_id=%d, pv_tag=0x%08X.\n", 
        e_get_type, pt_comp_id->e_type, pt_comp_id->ui2_id, pt_comp_id->pv_tag);

    switch (e_get_type)
    {
        case SKTDEV_GET_TYPE_SOCKET_NUM:
            i4_ret = RMR_OK;
            break;

        case SKTDEV_GET_TYPE_MODEL:     
            i4_ret = RMR_OK;                        
            break;

        default:
            i4_ret = RMR_DRV_INV_GET_INFO;
            break;
    }

    // fix lint warning.
    UNUSED(pt_comp_id);
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _fci_skt_set_fct
 *
 * Description: Storage device set function registered with Resource Manager.
 *
 * Inputs:  See Resource Manager specification.
 *
 * Outputs: -
 *
 * Returns: RMR_OK
 *              Success.
 *          RMR_DRV_INV_SET_INFO
 *              Other errors.
 ----------------------------------------------------------------------------*/
static INT32 _fci_skt_set_fct(
     DRV_COMP_ID_T  *pt_comp_id,
     DRV_SET_TYPE_T  e_set_type,
     const VOID     *pv_set_info,
     SIZE_T          z_set_info_len)
{
    INT32 i4_ret = RMR_DRV_INV_SET_INFO;
    SKTDEV_DEV_ST_NFY_T *ptMWSocketNfyInfo = NULL;
    BLKDEV_MEMCARD_VARIETY_T t_blk_variety;
    
    if ((pt_comp_id == NULL) || (pv_set_info == NULL))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    switch (e_set_type)
    {
        case SKTDEV_SET_TYPE_DEV_ST_NFY:
            if (z_set_info_len < sizeof(SKTDEV_DEV_ST_NFY_T))
            {
                return RMR_NOT_ENOUGH_SPACE;
            }

            ptMWSocketNfyInfo = (SKTDEV_DEV_ST_NFY_T *)pv_set_info;

            printf("\n[FCI] e_type = %d, tag = 0x%08X, nfy = 0x%08X.\n", 
                pt_comp_id->e_type, ptMWSocketNfyInfo->pv_tag, ptMWSocketNfyInfo->pf_sktdev_st_nfy);

            if (pt_comp_id->e_type != DRVT_HW_FCI)
            {
                return RMR_DRV_INV_SET_INFO;
            }
                              
            // save call back function and tag.
            rFciSocketNfyInfo.pv_tag = ptMWSocketNfyInfo->pv_tag;
            rFciSocketNfyInfo.pf_sktdev_st_nfy = ptMWSocketNfyInfo->pf_sktdev_st_nfy;  
              
            // call back to user's function directly if device status already change.
            // check if root device insert.
            t_blk_variety = _MwblkGetCardVariety(STORG_FLAG_MS);            
            if (t_blk_variety != BLKDEV_MEMCARD_NONE)
            {   
                SKTDEV_DEV_INFO_T rDevInfo = {0};
                
                printf("[FCI] MS: t_blk_variety = 0x%x\n", t_blk_variety);
                
                rDevInfo.eDevSt = SKTDEV_STATUS_DEV_ATTACH;
                rDevInfo.u4SocketNo = 0;    // FCI_0
                rDevInfo.u_info.rSupDev.u4DevType = DRVT_MEM_CARD;  // Memory card type
                rDevInfo.u_info.rSupDev.u4CompId = _MwblkGetUid(STORG_FLAG_MS);    // GUID of memory card
                rDevInfo.u_info.rSupDev.u4DevFlag = SKTDEV_FLAG_BLOCK_DEV;
                      
                // check and execute call back function.
                if (rFciSocketNfyInfo.pf_sktdev_st_nfy)
                {
                    printf("[FCI] SocketNo:%d, DevType:0x%x, CompId:0x%x, DevFlag:0x%x, eDevSt:0x%x\n", rDevInfo.u4SocketNo, rDevInfo.u_info.rSupDev.u4DevType, 
                      rDevInfo.u_info.rSupDev.u4CompId, rDevInfo.u_info.rSupDev.u4DevFlag, rDevInfo.eDevSt);
                    
                    rFciSocketNfyInfo.pf_sktdev_st_nfy(rFciSocketNfyInfo.pv_tag, rDevInfo.eDevSt, &rDevInfo);
                }
            }
            
            t_blk_variety = _MwblkGetCardVariety(STORG_FLAG_SD);            
            if (t_blk_variety != BLKDEV_MEMCARD_NONE)
            {
                SKTDEV_DEV_INFO_T rDevInfo = {0};
                
                printf("[FCI] SD: t_blk_variety = 0x%x\n", t_blk_variety);
                
                rDevInfo.eDevSt = SKTDEV_STATUS_DEV_ATTACH;
                rDevInfo.u4SocketNo = 0;    // FCI_0
                rDevInfo.u_info.rSupDev.u4DevType = DRVT_MEM_CARD;  // Memory card type
                rDevInfo.u_info.rSupDev.u4CompId = _MwblkGetUid(STORG_FLAG_SD);    // GUID of memory card
                rDevInfo.u_info.rSupDev.u4DevFlag = SKTDEV_FLAG_BLOCK_DEV;
                      
                // check and execute call back function.
                if (rFciSocketNfyInfo.pf_sktdev_st_nfy)
                {
                    printf("[FCI] SocketNo:%d, DevType:0x%x, CompId:0x%x, DevFlag:0x%x\n", rDevInfo.u4SocketNo, rDevInfo.u_info.rSupDev.u4DevType, 
                      rDevInfo.u_info.rSupDev.u4CompId, rDevInfo.u_info.rSupDev.u4DevFlag);
                    
                    rFciSocketNfyInfo.pf_sktdev_st_nfy(rFciSocketNfyInfo.pv_tag, rDevInfo.eDevSt, &rDevInfo);
                }
            }
            
            i4_ret = RMR_OK;
            break;
            
        case SKTDEV_SET_TYPE_USB_ST_NFY:
            if (z_set_info_len < sizeof(SKTDEV_USB_ST_NFY_T))
            {
                return RMR_NOT_ENOUGH_SPACE;
            }

            i4_ret = RMR_OK;            
            break;
            
        default:
            i4_ret = RMR_DRV_INV_SET_INFO;
        break;
    }

    // fix lint warning.
    UNUSED(pt_comp_id);
    return i4_ret;
}

static INT32 _MW_FciSktDevReg(UINT32 u4DevFlag, UINT32 u4Uid, DRV_TYPE_T drvType)
{
    DRV_COMP_REG_T      t_reg_comp      = {0};
    DRV_COMP_FCT_TBL_T  t_comp_fct_tbl  = {0};
    INT32               i4_ret          = RMR_OK;
    CHAR ps_name[32] = {0};
    STORG_DEV_T *prDev;

    prDev = STORGOpenDev(u4DevFlag);
    if (prDev == NULL)
    {
        return RMR_DRV_ERROR;
    }
    
    (VOID)x_sprintf(ps_name, "FCI_%d-%d", drvType, u4Uid);

    printf("[FCI] %s.\n", ps_name);
    
    t_reg_comp.e_type               = drvType;
    t_reg_comp.e_id_type            = ID_TYPE_IND;
    t_reg_comp.u.t_ind.ui2_id       = (UINT16)u4Uid;
    t_reg_comp.u.t_ind.ui1_port     = 0;
    t_reg_comp.u.t_ind.pv_tag       = prDev; /* storage device pointer. */

    t_comp_fct_tbl.pf_rm_connect    = NULL;
    t_comp_fct_tbl.pf_rm_disconnect = NULL;
    t_comp_fct_tbl.pf_rm_get        = _fci_skt_get_fct;
    t_comp_fct_tbl.pf_rm_set        = _fci_skt_set_fct;

    i4_ret = x_rm_reg_comp(&t_reg_comp,
                           1,               /* num in port */
                           1,               /* num out port */
                           ps_name,
                           0, /* STORG_FLAG_SOCKET_DEV */
                           &t_comp_fct_tbl,
                           NULL,
                           0);

    if (i4_ret != RMR_OK)
    {
        STORGCloseDev(prDev);
        return RMR_DRV_ERROR;
    }

    return RMR_OK;
}

static VOID _Fci_DevNfy(VOID *pvNfyTag, STORG_COND_T eNfyCond, VOID *pvData)
{
    SKTDEV_DEV_INFO_T rDevInfo = {0};
    STORG_DEV_T *prDev = NULL;

    ASSERT(pvNfyTag);

    prDev = (STORG_DEV_T *)pvNfyTag;

    switch (eNfyCond)
    {
        case COND_FAIL:         /* BLKDEV_COND_FAIL */
          rDevInfo.eDevSt = SKTDEV_STATUS_DEV_UNSUPPORT;
          break;
          
        case COND_INIT:         /* Request just initial. */
          break;
          
        case COND_INIT_OK:      /* Initialize OK */
          break;
          
        case COND_UNINIT_OK:    /* Un-Initialize OK */
          break;
          
        case COND_INSERT_OK:    /* BLKDEV_STATUS_READY */
          rDevInfo.eDevSt = SKTDEV_STATUS_DEV_ATTACH;
          break;
          
        case COND_REMOVE_OK:    /* BLKDEV_STATUS_ABSENT */
          rDevInfo.eDevSt = SKTDEV_STATUS_DEV_DETACH;
          break;
          
        case COND_HOTSWAP_FAIL: /* BLKDEV_STATUS_FAIL */
          rDevInfo.eDevSt = SKTDEV_STATUS_DEV_FAIL;
          break;
          
        default:
          break;
    }
   
    rDevInfo.u4SocketNo = 0;    // FCI_0
    rDevInfo.u_info.rSupDev.u4DevType = DRVT_MEM_CARD;  // Memory card type
    rDevInfo.u_info.rSupDev.u4CompId = _MwblkGetUid(prDev->u4DevFlag);    // GUID of memory card
    rDevInfo.u_info.rSupDev.u4DevFlag = SKTDEV_FLAG_BLOCK_DEV;
          
    // check and execute call back function.
    if (rFciSocketNfyInfo.pf_sktdev_st_nfy)
    {
        printf("[FCI] SocketNo:%d, DevType:0x%x, CompId:0x%x, DevFlag:0x%x, eDevSt:0x%x\n", rDevInfo.u4SocketNo, rDevInfo.u_info.rSupDev.u4DevType, 
          rDevInfo.u_info.rSupDev.u4CompId, rDevInfo.u_info.rSupDev.u4DevFlag, rDevInfo.eDevSt);
        
        rFciSocketNfyInfo.pf_sktdev_st_nfy(rFciSocketNfyInfo.pv_tag, rDevInfo.eDevSt, &rDevInfo);        
    }
}

static INT32 _MW_Medium_HotswapReg(UINT32 u4DevFlag, STORG_HOTSWAP_NFY_FCT pfnHotswapNfy)
{
    STORG_DEV_T *prDev;
  
    prDev = STORGOpenDev(u4DevFlag);
    if (prDev == NULL)
    {
        return RMR_DRV_ERROR;
    }

    prDev->pfnHotswapNfy = pfnHotswapNfy;
    prDev->pvHotswapTag = (VOID *)prDev;
    
    STORGCloseDev(prDev);

    return RMR_OK;
}

static INT32 _Mw_Socket_FCI_Init(void)
{   
    // init medium call back function.
    _MW_Medium_HotswapReg(STORG_FLAG_MS, _Fci_DevNfy);
    _MW_Medium_HotswapReg(STORG_FLAG_SD, _Fci_DevNfy);

    // init FCI socket device registration
    _MW_FciSktDevReg(STORG_FLAG_FCI, 0, DRVT_HW_FCI);
    
    return RMR_OK;
}
#endif  // #ifdef CC_FCI_ENABLE

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** MW_BLOCK_Init
 *  Block driver init entry function .
 *  @param      void.
 *  @retval       RMR_OK    Success.
 *  @retval       RMR_DRV_ERROR    Error.
 */
//-------------------------------------------------------------------------
INT32 MW_BLOCK_Init(void)
{
    INT32 i4Ret = 0;
#ifndef CC_USB_DISALBE
    UINT32 i = 0;
#endif

    VERIFY(0 == STORGInit());

#ifdef CC_IDE_ENABLE
    i4Ret |= _MwblkDevInit(STORG_FLAG_CF, DRVN_MEM_CARD_CF, DRVT_MEM_CARD, BLKDEV_FLAG_HOTSWAP);
    ASSERT(i4Ret == RMR_OK);
    i4Ret |= _MwblkDevInit(STORG_FLAG_HD, DRVN_HARD_DISK, DRVT_HARD_DISK, BLKDEV_FLAG_STANDBY);
    ASSERT(i4Ret == RMR_OK);
#endif

#ifdef CC_FCI_ENABLE
    i4Ret |= _MwblkDevInit(STORG_FLAG_MS, DRVN_MEM_CARD_MS, DRVT_MEM_CARD, BLKDEV_FLAG_HOTSWAP);
    ASSERT(i4Ret == RMR_OK);
    i4Ret |= _MwblkDevInit(STORG_FLAG_SD, DRVN_MEM_CARD_SD, DRVT_MEM_CARD, BLKDEV_FLAG_HOTSWAP);
    ASSERT(i4Ret == RMR_OK);
    
    #ifdef CC_FCI_SMXD_SUPPORT
    i4Ret |= _MwblkDevInit(STORG_FLAG_SMXD, DRVN_MEM_CARD_SMXD, DRVT_MEM_CARD, BLKDEV_FLAG_HOTSWAP);
    ASSERT(i4Ret == RMR_OK);
    #endif

    _Mw_Socket_FCI_Init();
#endif

#ifdef CC_NAND_ENABLE
    _i4NANDPartNum = NANDPART_GetMWPartNum();
    _arNANDNfyInfo = x_mem_alloc(sizeof(BLKDEV_NFY_INFO_T) * (UINT32)_i4NANDPartNum);
    if (_arNANDNfyInfo == NULL) 
    {
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return RMR_DRV_ERROR;
    }
    
    i4Ret = 0;
    for (i = 0; i < _i4NANDPartNum; i++)
    {
        i4Ret |= _MwblkDevInit(STORG_FLAG_NAND, DRVN_NAND_FLASH, DRVT_NAND_FLASH, i);
        ASSERT(i4Ret == RMR_OK);
    }
    
    LINT_SUPPRESS_NEXT_EXPRESSION(774);
    if (i4Ret) 
    {
        return RMR_DRV_ERROR;
    }
#endif

#ifndef CC_USB_DISABLE
    VERIFY(0 == MUSB_Init(MUSB_HOST_MODE));

    for (i = 0; i < MUSB_UCFS_UNIT_COUNT; i++)
    {
        i4Ret |= _USBMsd_Init(i, DRVN_USB_MASS, DRVT_USB_MASS_STORAGE, BLKDEV_FLAG_HOTSWAP);
        ASSERT(i4Ret == RMR_OK);
    }
#endif
    LINT_SUPPRESS_NEXT_EXPRESSION(774);

    if (i4Ret)
    {
        return RMR_DRV_ERROR;
    }

    return RMR_OK;
}

