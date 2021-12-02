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
 * Description: This file implements middleware char devices driver interface.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#include "x_typedef.h"
#include "x_os.h"
#include "x_rm.h"
#include "x_rm_dev_types.h"
#include "x_chr_dev.h"
#include "x_lint.h"
#include "nor_if.h"
#include "storage_if.h"
#include "drv_name.h"
#include "eeprom_if.h"
#include "x_pinmux.h"

#ifndef CC_NOR_ENABLE
#define CC_NOR_ENABLE
#endif

#ifndef CC_EEPROM_ENABLE
#define CC_EEPROM_ENABLE
#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
#define NOR_DEF_NAME            DRVN_NOR_FLASH
#define NOR_MAX_SYNC_READ       ((UINT32) 8 * 1024 * 1024)
#define NOR_MAX_SYNC_WRITE      ((UINT32) 8 * 1024 * 1024)
#define EEPROM_DEF_NAME         DRVN_EEPROM

#define EEPROM_PID_MW                    ((UINT16)0)
#define EEPROM_PID_NPTV                 ((UINT16)1)
#define EEPROM_PID_DTV                   ((UINT16)2)
// all eeprom partition.
#define EEPROM_PID_ALL                   ((UINT16)3)
/*-----------------------------------------------------------------------------
                    data declarations
-----------------------------------------------------------------------------*/
/* 1 NfyInfo of EEPROM */
static CHRDEV_NFY_INFO_T _rEEPROMNfyInfo = { NULL, NULL };
/* _i4NORPartNum number of NfyInfo of NORFLASH */
static CHRDEV_NFY_INFO_T *_arNORNfyInfo;
static INT32 _i4NORPartNum;

/*-----------------------------------------------------------------------------
                    functions declaraions
-----------------------------------------------------------------------------*/
static INT32 _char_get_fct(
    DRV_COMP_ID_T  *pt_comp_id,
    DRV_GET_TYPE_T  e_get_type,
    VOID           *pv_get_info,
    SIZE_T         *pz_get_info_len);

static INT32 _char_set_fct(
    DRV_COMP_ID_T  *pt_comp_id,
    DRV_SET_TYPE_T  e_set_type,
    const VOID     *pv_set_info,
    SIZE_T          z_set_info_len);

INT32 MW_CHAR_Init(void);

/*-----------------------------------------------------------------------------
                    Internal functions implementations
-----------------------------------------------------------------------------*/
static UINT32 _CHARGetEEPOffset(UINT16 ui2_id)
{
    UINT32 u4Offset = 0;

    switch (ui2_id)
    {
    case EEPROM_PID_MW:
        u4Offset = EEPROM_DRIVER_SIZE;
        break;

    case EEPROM_PID_NPTV:
        u4Offset = EEPROM_NPTV_DRIVER_OFFSET;
        break;

    case EEPROM_PID_DTV:
        u4Offset = EEPROM_DTV_DRIVER_OFFSET;
        break;

    case EEPROM_PID_ALL:
        u4Offset = 0;
        break;

    default:
        ASSERT(0);
    }

    return u4Offset;
}

static UINT32 _CHARGetSize(UINT32 u4DevFlag, UINT16 ui2_id)
{
    UINT32 u4Size = 0;
		
    if (u4DevFlag == STORG_FLAG_EEPROM)
    {
        switch (ui2_id)
        {
            case EEPROM_PID_MW:
                u4Size = (UINT32)STORG_GetDevSize(u4DevFlag, ui2_id);
                ASSERT (u4Size > EEPROM_DRIVER_SIZE);
                // driver use EEPROM_DRIVER_SIZE bytes in front of MW.
                u4Size -= EEPROM_DRIVER_SIZE;				            
            break;
            
            case EEPROM_PID_NPTV:
                u4Size = EEPROM_NPTV_DRIVER_SIZE;
            break;

            case EEPROM_PID_DTV:
                u4Size = EEPROM_DTV_DRIVER_SIZE;
            break;

            case EEPROM_PID_ALL:
                u4Size = (UINT32)STORG_GetDevSize(u4DevFlag, ui2_id);
            break;

            default:
                ASSERT(0);
        }
    }	
    else
    {
        u4Size = (UINT32)STORG_GetDevSize(u4DevFlag, ui2_id);
    }
		
    return u4Size;
}

static CHRDEV_NFY_INFO_T *_CHARGetNfyInfo(UINT32 u4DevFlag, UINT16 ui2_id)
{
    ASSERT(ui2_id < _i4NORPartNum);

    switch(u4DevFlag) {
    case STORG_FLAG_NOR:
        if (ui2_id < _i4NORPartNum) {
            return &_arNORNfyInfo[ui2_id];
        }
        break;
    case STORG_FLAG_EEPROM:
        return &_rEEPROMNfyInfo;
    default:
        ASSERT(0);
    }
    return NULL;
}

//lint -save -e818
/*-----------------------------------------------------------------------------
 * Name: _char_get_fct
 *
 * Description: NOR get function registered with Resource Manager.
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
 *              Other errors.
 ----------------------------------------------------------------------------*/
static INT32 _char_get_fct(
    DRV_COMP_ID_T  *pt_comp_id,
    DRV_GET_TYPE_T  e_get_type,
    VOID           *pv_get_info,
    SIZE_T         *pz_get_info_len)
{
    CHRDEV_SHORT_DATA_INFO_T *pt_sdata;
    CHRDEV_LONG_DATA_INFO_T  *pt_ldata;
    CHRDEV_SECTOR_TBL_INFO_T *pt_sector_tbl;
    UINT32 *pui4_sect_num;
    UINT64 u8Offset;

    STORG_DEV_T *prDev;
    INT32 i4Ret, i4_ret = RMR_DRV_INV_GET_INFO;

    if ((pt_comp_id == NULL)  ||
        (pv_get_info == NULL) ||
        (pz_get_info_len == NULL))
    {
        return RMR_DRV_INV_GET_INFO;
    }

    prDev = (STORG_DEV_T *) pt_comp_id->pv_tag;

    switch (e_get_type)
    {
        case CHRDEV_GET_TYPE_READ_BYTE:
            if (*pz_get_info_len < sizeof(CHRDEV_SHORT_DATA_INFO_T))
            {
                *pz_get_info_len = sizeof(CHRDEV_SHORT_DATA_INFO_T);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }

            pt_sdata = (CHRDEV_SHORT_DATA_INFO_T *)pv_get_info;

            if (prDev->u4DevFlag == STORG_FLAG_EEPROM)
            {
                if (pt_sdata->ui4_offset >= _CHARGetSize(STORG_FLAG_EEPROM, pt_comp_id->ui2_id))
                {
                    return RMR_DRV_INV_GET_INFO;
                }
                u8Offset = (UINT64)pt_sdata->ui4_offset + (UINT64)_CHARGetEEPOffset(pt_comp_id->ui2_id);
            }
            else
            {
                // Max Liao, NOR FLASH: the high 32 bits of u8Offset is fill with partition id.                
                u8Offset = (UINT64)pt_sdata->ui4_offset;
                u8Offset |= ((UINT64)pt_comp_id->ui2_id) << 32; 
            }
            /* do sync read 1 byte job. */
            i4Ret = STORGSendReq(prDev, STORG_OP_READ, NULL, NULL, NULL,
                            u8Offset, 1, (void *)&pt_sdata->ui1_data, NULL);
            if (i4Ret == 0) {
                i4_ret = RMR_OK;
            }
            break;

        case CHRDEV_GET_TYPE_SYNC_READ:
            if (*pz_get_info_len < sizeof(CHRDEV_LONG_DATA_INFO_T))
            {
                *pz_get_info_len = sizeof(CHRDEV_LONG_DATA_INFO_T);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }

            pt_ldata = (CHRDEV_LONG_DATA_INFO_T *)pv_get_info;

            if (pt_ldata->ui4_len == 0)
            {
                return RMR_DRV_INV_GET_INFO;
            }

            if (prDev->u4DevFlag == STORG_FLAG_EEPROM)
            {
                if (pt_ldata->ui4_offset >= _CHARGetSize(STORG_FLAG_EEPROM, pt_comp_id->ui2_id))
                {
                    return RMR_DRV_INV_GET_INFO;
                }            
                u8Offset = (UINT64)pt_ldata->ui4_offset + (UINT64)_CHARGetEEPOffset(pt_comp_id->ui2_id);
            }
            else
            {
                // Max Liao, NOR FLASH: the high 32 bits of u8Offset is fill with partition id.                
                u8Offset = (UINT64)pt_ldata->ui4_offset;
                u8Offset |= ((UINT64)pt_comp_id->ui2_id) << 32; 
            }

            /* do sync read n byte job. */
            i4Ret = STORGSendReq(prDev, STORG_OP_READ, NULL, NULL, NULL,
                            u8Offset, pt_ldata->ui4_len, pt_ldata->pv_data, NULL);
            if (i4Ret == 0) {
                *pz_get_info_len = sizeof(UINT32);
                i4_ret = RMR_OK;
            }
            break;

        case CHRDEV_GET_TYPE_SIZE:
        case CHRDEV_GET_TYPE_MAX_SYNC_READ_SIZE:
        case CHRDEV_GET_TYPE_MAX_SYNC_WRITE_SIZE:
            if (*pz_get_info_len < sizeof(UINT32))
            {
                *pz_get_info_len = sizeof(UINT32);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            
            *(UINT32 *)pv_get_info = _CHARGetSize(prDev->u4DevFlag, pt_comp_id->ui2_id);

            *pz_get_info_len = sizeof(UINT32);
            i4_ret = RMR_OK;

            break;

        case CHRDEV_GET_TYPE_NUMBER_OF_SECTORS:
            if (prDev->u4DevFlag != STORG_FLAG_NOR) {
                ASSERT(prDev->u4DevFlag == STORG_FLAG_NOR);
                return i4_ret;
            }
            if (*pz_get_info_len < sizeof(UINT32))
            {
                *pz_get_info_len = sizeof(UINT32);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }

            pui4_sect_num = (UINT32 *)pv_get_info;
            if (NORPART_GetPartSectorNum(pt_comp_id->ui2_id) > 0) {
                *pui4_sect_num = NORPART_GetPartSectorNum(pt_comp_id->ui2_id);
                *pz_get_info_len = sizeof(UINT32);
                i4_ret = RMR_OK;
            }
            break;
        case CHRDEV_GET_TYPE_SECTOR_TBL_INFO:
            if (prDev->u4DevFlag != STORG_FLAG_NOR) {
                ASSERT(prDev->u4DevFlag == STORG_FLAG_NOR);
                return i4_ret;
            }
            if (*pz_get_info_len < sizeof(CHRDEV_SECTOR_TBL_INFO_T))
            {
                *pz_get_info_len = sizeof(CHRDEV_SECTOR_TBL_INFO_T);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }

            pt_sector_tbl = (CHRDEV_SECTOR_TBL_INFO_T *) (void *)pv_get_info;
            i4Ret = NORPART_GetSectorTbl((SECTTBL_T *)(void *)pt_sector_tbl, pt_comp_id->ui2_id);

            if (i4Ret > 0) { /* not enouth space. */
                *pz_get_info_len = sizeof(CHRDEV_SECTOR_TBL_INFO_T) +
                                (sizeof(CHRDEV_SECTOR_SEG_INFO_T) * (UINT32)i4Ret);
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }

            if (i4Ret == 0) {
                *pz_get_info_len = sizeof(CHRDEV_SECTOR_TBL_INFO_T) +
                                (pt_sector_tbl->ui4_num_entries*sizeof(CHRDEV_SECTOR_SEG_INFO_T));
                i4_ret = RMR_OK;
            }
            break;
        default:
            /* unknown get type */
            break;
    }

    return i4_ret;
}


/*-----------------------------------------------------------------------------
 * Name: _char_set_fct
 *
 * Description: NOR set function registered with Resource Manager.
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
 *              Other errors.
 ----------------------------------------------------------------------------*/
static INT32 _char_set_fct(
     DRV_COMP_ID_T  *pt_comp_id,
     DRV_SET_TYPE_T  e_set_type,
     const VOID     *pv_set_info,
     SIZE_T          z_set_info_len)
{
    CHRDEV_SHORT_DATA_INFO_T *pt_sdata;
    CHRDEV_LONG_DATA_INFO_T  *pt_ldata;
    CHRDEV_NFY_INFO_T        *pt_nfy_info;
    CHRDEV_SECTOR_RANGE_INFO_T *pt_sector_range_info;

    STORG_DEV_T *prDev;
    STORG_OP_T eOp;
    UINT64 u8Offset;
    INT32 i4Ret, i4_ret = RMR_DRV_INV_SET_INFO;

    if ((pt_comp_id == NULL) || (pv_set_info == NULL))
    {
        return RMR_DRV_INV_SET_INFO;
    }

    prDev = (STORG_DEV_T *) pt_comp_id->pv_tag;
    pt_nfy_info = _CHARGetNfyInfo(prDev->u4DevFlag, pt_comp_id->ui2_id);

    if ((prDev == NULL) || (pt_nfy_info == NULL)) {
        ASSERT((prDev != NULL) && (pt_nfy_info != NULL));
        return RMR_DRV_INV_SET_INFO;
    }

    switch (e_set_type)
    {
        case CHRDEV_SET_TYPE_WRITE_BYTE:
            if (z_set_info_len < sizeof(CHRDEV_SHORT_DATA_INFO_T))
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }

            pt_sdata = (CHRDEV_SHORT_DATA_INFO_T *)pv_set_info;

            if (prDev->u4DevFlag == STORG_FLAG_EEPROM)
            {
                if (pt_sdata->ui4_offset >= _CHARGetSize(STORG_FLAG_EEPROM, pt_comp_id->ui2_id))
                {
                    return RMR_DRV_INV_GET_INFO;
                }            
                u8Offset = (UINT64)pt_sdata->ui4_offset + (UINT64)_CHARGetEEPOffset(pt_comp_id->ui2_id);
            }
            else
            {
                // Max Liao, NOR FLASH: the high 32 bits of u8Offset is fill with partition id.                
                u8Offset = (UINT64)pt_sdata->ui4_offset;
                u8Offset |= ((UINT64)pt_comp_id->ui2_id) << 32;            
            }

            /* do sync write 1 byte job. */
            i4Ret = STORGSendReq(prDev, STORG_OP_WRITE, NULL, NULL, NULL,
                            u8Offset, 1, (void *)&pt_sdata->ui1_data, NULL);
            if (i4Ret == 0) {
                i4_ret = RMR_OK;
            }
            break;
        case CHRDEV_SET_TYPE_SYNC_WRITE:
            if (z_set_info_len < sizeof(CHRDEV_LONG_DATA_INFO_T))
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }

            pt_ldata = (CHRDEV_LONG_DATA_INFO_T *)pv_set_info;

            if (pt_ldata->ui4_len == 0)
            {
                return RMR_DRV_INV_GET_INFO;
            }

            if (prDev->u4DevFlag == STORG_FLAG_EEPROM)
            {
                if (pt_ldata->ui4_offset >= _CHARGetSize(STORG_FLAG_EEPROM, pt_comp_id->ui2_id))
                {
                    return RMR_DRV_INV_GET_INFO;
                }            
                u8Offset = (UINT64)pt_ldata->ui4_offset + (UINT64)_CHARGetEEPOffset(pt_comp_id->ui2_id);
            }
            else
            {
                // Max Liao, NOR FLASH: the high 32 bits of u8Offset is fill with partition id.                
                u8Offset = (UINT64)pt_ldata->ui4_offset;
                u8Offset |= ((UINT64)pt_comp_id->ui2_id) << 32;            
            }

            /* do sync write n byte job. */
            i4Ret = STORGSendReq(prDev, STORG_OP_WRITE, NULL, NULL, NULL,
                            u8Offset, pt_ldata->ui4_len, pt_ldata->pv_data, NULL);
            if (i4Ret == 0) {
                i4_ret = RMR_OK;
            }
            break;

        case CHRDEV_SET_TYPE_ASYNC_READ:
        case CHRDEV_SET_TYPE_ASYNC_WRITE:
            if (z_set_info_len < sizeof(CHRDEV_LONG_DATA_INFO_T))
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }

            if (pt_nfy_info->pf_chrdev_nfy == NULL) {
                ASSERT(pt_nfy_info->pf_chrdev_nfy != NULL);
                return i4_ret;
            }

            pt_ldata = (CHRDEV_LONG_DATA_INFO_T *)pv_set_info;

            if (pt_ldata->ui4_len == 0)
            {
                return RMR_DRV_INV_GET_INFO;
            }

            if (prDev->u4DevFlag == STORG_FLAG_EEPROM)
            {
                if (pt_ldata->ui4_offset >= _CHARGetSize(STORG_FLAG_EEPROM, pt_comp_id->ui2_id))
                {
                    return RMR_DRV_INV_GET_INFO;
                }            
                u8Offset = (UINT64)pt_ldata->ui4_offset + (UINT64)_CHARGetEEPOffset(pt_comp_id->ui2_id);
            }
            else
            {
                // Max Liao, NOR FLASH: the high 32 bits of u8Offset is fill with partition id.                
                u8Offset = (UINT64)pt_ldata->ui4_offset;
                u8Offset |= ((UINT64)pt_comp_id->ui2_id) << 32;            
            }
            
            
            eOp = ((e_set_type == CHRDEV_SET_TYPE_ASYNC_WRITE) ? STORG_OP_WRITE : STORG_OP_READ);

            /* call nor async read/write function. */
            i4Ret = STORGSendReq(prDev, eOp, pt_ldata, pt_nfy_info->pv_tag,
                                (STORG_NFY_FCT)pt_nfy_info->pf_chrdev_nfy,
                                u8Offset, pt_ldata->ui4_len,
                                pt_ldata->pv_data, NULL);
            if (i4Ret == 0) {
                i4_ret = RMR_ASYNC_NFY;
            }
            break;

        case CHRDEV_SET_TYPE_NFY_FCT:
            if (z_set_info_len < sizeof(CHRDEV_NFY_INFO_T))
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }

            /* Save pt_nfy_info->pv_tag and pt_nfy_info->pf_chrdev_nfy; */
            pt_nfy_info->pf_chrdev_nfy = ((CHRDEV_NFY_INFO_T *)pv_set_info)->pf_chrdev_nfy;
            pt_nfy_info->pv_tag = ((CHRDEV_NFY_INFO_T *)pv_set_info)->pv_tag;

            i4_ret = RMR_OK;
            break;

        case CHRDEV_SET_TYPE_ERASE:
            if (prDev->u4DevFlag != STORG_FLAG_NOR) {
                ASSERT(prDev->u4DevFlag == STORG_FLAG_NOR);
                return i4_ret;
            }

            if (z_set_info_len < sizeof(CHRDEV_SECTOR_RANGE_INFO_T))
            {
                return RMR_DRV_NOT_ENOUGH_SPACE;
            }
            pt_sector_range_info = (CHRDEV_SECTOR_RANGE_INFO_T *)pv_set_info;

            if ((pt_sector_range_info->ui4_start_sector + pt_sector_range_info->ui4_number_of_sectors) >
                            NORPART_GetPartSectorNum(pt_comp_id->ui2_id)) {
                ASSERT(0);
                LINT_SUPPRESS_NEXT_EXPRESSION(527);
                return i4_ret;
            }

            if (pt_nfy_info->pf_chrdev_nfy == NULL) {
                ASSERT(pt_nfy_info->pf_chrdev_nfy != NULL);
                return i4_ret;
            }

            // Max Liao, NOR FLASH: the high 32 bits of u8Offset is fill with partition id.                
            u8Offset = (UINT64)pt_sector_range_info->ui4_start_sector;
            u8Offset |= ((UINT64)pt_comp_id->ui2_id) << 32;            

            /* call nor async erase function. */
            i4Ret = STORGSendReq(prDev, STORG_OP_ERASE,
                                pt_sector_range_info, pt_nfy_info->pv_tag,
                                (STORG_NFY_FCT)pt_nfy_info->pf_chrdev_nfy,
                                u8Offset,
                                pt_sector_range_info->ui4_number_of_sectors, NULL, NULL);
            if (i4Ret == 0) {
                i4_ret = RMR_ASYNC_NFY;
            }
            break;

        default:
            /* unknown set type */
            break;
    }

    return i4_ret;
}
//lint -restore

/*-----------------------------------------------------------------------------
 * Name: _MW_CharDevInit();
 *
 * Description: Char driver init function.
 *
 * Inputs:  u4DevFlag
 *              The block device id flag.
 *          szDefName
 *              The block device name string.
 *          drvType
 *              The block device type.
 *
 * Outputs: -
 *
 * Returns: RMR_OK
 *              Success.
 *          RMR_INV_ARG
 *              Invalid arguments.
 *          RMR_DRV_ERROR
 *              Other errors.
 ----------------------------------------------------------------------------*/
static INT32 _MW_CharDevInit(UINT32 u4DevFlag, const CHAR *szDefName, DRV_TYPE_T drvType, UINT16 ui2_uid)
{
    DRV_COMP_REG_T      t_reg_comp;
    DRV_COMP_FCT_TBL_T  t_comp_fct_tbl;
    CHAR ps_name[32];
    INT32 i4_ret;
    STORG_DEV_T *prDev;

    prDev = STORGOpenDev(u4DevFlag);
    if (prDev == NULL) {
        return RMR_DRV_ERROR;
    }

	if (szDefName)
	{
        LINT_SUPPRESS_NEXT_EXPRESSION(534);
	    x_sprintf(ps_name, szDefName, ui2_uid);
	}

    t_reg_comp.e_type               = drvType;
    t_reg_comp.e_id_type            = ID_TYPE_IND;
    t_reg_comp.u.t_ind.ui2_id       = ui2_uid;
    t_reg_comp.u.t_ind.ui1_port     = 0;
    t_reg_comp.u.t_ind.pv_tag       = prDev; /* storage device pointer. */

    t_comp_fct_tbl.pf_rm_connect    = NULL;
    t_comp_fct_tbl.pf_rm_disconnect = NULL;
    t_comp_fct_tbl.pf_rm_get        = _char_get_fct;
    t_comp_fct_tbl.pf_rm_set        = _char_set_fct;

//lint -save -e645
    i4_ret = x_rm_reg_comp(&t_reg_comp,
                           1,               /* num in port */
                           1,               /* num out port */
                           (szDefName == NULL) ? NULL : ps_name,
                           CHRDEV_FLAG_ASYNC_READ | CHRDEV_FLAG_ASYNC_WRITE,
                           &t_comp_fct_tbl,
                           NULL,
                           0);
//lint -restore

    if (i4_ret != RMR_OK)
    {
        LINT_SUPPRESS_NEXT_EXPRESSION(534);
        STORGCloseDev(prDev);
        return RMR_DRV_ERROR;
    }

    return RMR_OK;
}

/*-----------------------------------------------------------------------------
                    External functions implementations
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name: MW_CHAR_Init
 *
 * Description: CHAR driver init function.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: RMR_OK
 *              Success.
 *          RMR_INV_ARG
 *              Invalid arguments.
 *          RMR_DRV_ERROR
 *              Other errors.
 ----------------------------------------------------------------------------*/
INT32 MW_CHAR_Init()
{
    INT32 i, i4Ret = 0;

    VERIFY(0 == STORGInit());

#ifdef CC_EEPROM_ENABLE
    // EEPROM id = 0 is for Middleware.
    i4Ret = _MW_CharDevInit(STORG_FLAG_EEPROM, EEPROM_DEF_NAME, DRVT_EEPROM, EEPROM_PID_MW);
    if (i4Ret) {
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return RMR_DRV_ERROR;
    }
    // EEPROM id = 1 is for NPTV.
    i4Ret = _MW_CharDevInit(STORG_FLAG_EEPROM, EEPROM_DEF_NAME, DRVT_EEPROM, EEPROM_PID_NPTV);
    if (i4Ret) {
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return RMR_DRV_ERROR;
    }
    // EEPROM id = 2 is for DTV.
    i4Ret = _MW_CharDevInit(STORG_FLAG_EEPROM, EEPROM_DEF_NAME, DRVT_EEPROM, EEPROM_PID_DTV);
    if (i4Ret) {
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return RMR_DRV_ERROR;
    }    
    // EEPROM id = 3 is for ALL EEPROM.
    i4Ret = _MW_CharDevInit(STORG_FLAG_EEPROM, EEPROM_DEF_NAME, DRVT_EEPROM, EEPROM_PID_ALL);
    if (i4Ret) {
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return RMR_DRV_ERROR;
    }    

#endif

#ifdef CC_NOR_ENABLE
    i4Ret = NORPART_GetMWPartNum();
    _arNORNfyInfo = x_mem_alloc(sizeof(CHRDEV_NFY_INFO_T) * (UINT32)i4Ret);
    if (_arNORNfyInfo == NULL) {
        ASSERT(0);
        LINT_SUPPRESS_NEXT_EXPRESSION(527);
        return RMR_DRV_ERROR;
    }
    _i4NORPartNum = i4Ret;

    i4Ret = 0;
    for (i=0; i<_i4NORPartNum; i++) {
        i4Ret |= _MW_CharDevInit(STORG_FLAG_NOR, NOR_DEF_NAME, DRVT_NOR_FLASH, i);
        ASSERT(i4Ret == RMR_OK);
    }
    LINT_SUPPRESS_NEXT_EXPRESSION(774);
    if (i4Ret) {
        return RMR_DRV_ERROR;
    }
#endif

    return RMR_OK;
}

