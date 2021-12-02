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
 * $RCSfile: divx_drm_mem.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $CCRevision:  $
 * $SWAuthor:  $
 * $MD5HEX:  $
 *
 * Description:
 *         
 *---------------------------------------------------------------------------*/

#ifndef __KERNEL__
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "inc/x_common.h"
#include "dbg/x_dbg.h"
#include "drm/drm_dbg.h"
#include "file_mngr/x_fm.h"
#include "inc/u_sys_name.h"
#include "drm/divx_drm/x_divx_drm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static UINT16 ui2_divx_drm_memory_offset;
static UINT16 ui2_divx_drm_memory_size_byte;

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: divx_drm_mem_init
 *
 * Description: This API initializes the divx drm memory.
 *
 * Inputs: t_divx_drm_init_config  Reference contains the divx drm configuration structure.
 *
 * Outputs: None.
 *
 * Returns: TRUE  Success. The API is successful. 
 *          FALSE  Fail.
 ----------------------------------------------------------------------------*/
INT32 divx_drm_mem_init (
    DIVX_DRM_INIT_CONFIG_T* pt_divx_drm_init_config)
{
    if (0 == pt_divx_drm_init_config->ui2_divx_drm_memory_size_byte)
    {
        return FALSE;
    }

    ui2_divx_drm_memory_offset = pt_divx_drm_init_config->ui2_divx_drm_memory_offset;
    ui2_divx_drm_memory_size_byte = pt_divx_drm_init_config->ui2_divx_drm_memory_size_byte;

    DBG_INFO((_INFO_HEADER"%s: divx drm memory: offset=%d, size=%d\n", __FUNCTION__, ui2_divx_drm_memory_offset, ui2_divx_drm_memory_size_byte));

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_mem_info
 *
 * Description: This API provides the divx drm memory info.
 *
 * Inputs: pui2_divx_drm_memory_offset  Reference to retrieve the divx drm offset in whole eeprom.
 *         pui2_divx_drm_memory_size_byte  Reference to retrieve the divx drm size in byte.
 *
 * Outputs: pui2_divx_drm_memory_offset  Reference contains the divx drm offset in whole eeprom.
 *          pui2_divx_drm_memory_size_byte  Reference contains the divx drm size in byte.
 *
 * Returns: NONE.
 ----------------------------------------------------------------------------*/
VOID divx_drm_mem_info (
    UINT16* pui2_divx_drm_memory_offset,
    UINT16* pui2_divx_drm_memory_size_byte)
{
    *pui2_divx_drm_memory_offset = ui2_divx_drm_memory_offset;
    *pui2_divx_drm_memory_size_byte = ui2_divx_drm_memory_size_byte;
}
 
/*-----------------------------------------------------------------------------
 * Name: divx_drm_mem_read
 *
 * Description: This API reads the divx drm memory.
 *
 * Inputs: pui1_divx_drm_mem  Reference to retrieve the divx drm memory.
 *
 * Outputs: pui1_divx_drm_mem  Reference contains the divx drm memory.
 *
 * Returns: TRUE  Success.
 *          FALSE  Fail.
 ----------------------------------------------------------------------------*/
BOOL divx_drm_mem_read (
    UINT8* pui1_divx_drm_mem)
{
    HANDLE_T h_eep = NULL_HANDLE;
    INT32 i4_ret;
    UINT32 ui4_n;
    UINT32 ui4_read;
    UINT32 ui4_count;
    UINT64 ui8_pos;
 
    /* open the EEPROM file */
    i4_ret = x_fm_open(FM_ROOT_HANDLE, (SN_DEV_PATH "/" SN_EEPROM_3), FM_READ_WRITE, 0777, TRUE, &h_eep);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR((_ERROR_HEADER"%s: FM opens the divx drm memory failed, return=%d\n", __FUNCTION__, i4_ret));        
        printk("FM opens the divx drm memory failed\n");
        return FALSE;
    }

    i4_ret = x_fm_lseek(h_eep, (INT64)ui2_divx_drm_memory_offset, FM_SEEK_BGN, &ui8_pos);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR((_ERROR_HEADER"%s: FM seeks the divx drm memory failed, return=%d\n", __FUNCTION__, i4_ret));
        printk("FM seeks the divx drm memory failed\n");

        x_fm_close(h_eep);

        return FALSE;
    }

    ui4_read = 0;
    ui4_count = ui2_divx_drm_memory_size_byte;                
    do
    {
        i4_ret = x_fm_read(h_eep, pui1_divx_drm_mem+ui4_read, ui4_count, &ui4_n);
        if (i4_ret == FMR_EOF)
        {
            DBG_ERROR((_ERROR_HEADER"%s: FM reads the divx drm memory failed, dev_size=%d required=%d\n", __FUNCTION__, ui4_read, ui4_count));
            printk("FM reads the divx drm memory failed\n");

            x_fm_close(h_eep);            

            return FALSE;
        }
        if (i4_ret != FMR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"%s: FM reads the divx drm memory failed, return=%d\n", __FUNCTION__, i4_ret));
            printk("FM reads the divx drm memory failed\n");

            x_fm_close(h_eep);            

            return FALSE;
        }
        ui4_read += ui4_n;
        ui4_count -= ui4_n;
    } while (ui4_count > 0);

    i4_ret = x_fm_close(h_eep);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR((_ERROR_HEADER"%s: FM closes the divx drm memory failed, return=%d\n", __FUNCTION__, i4_ret));
        printk("FM closes the divx drm memory failed\n");

        return FALSE;
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_mem_write
 *
 * Description: This API writes the divx drm memory.
 *
 * Inputs: pui1_divx_drm_mem  Reference contains the divx drm memory.
 *
 * Outputs: None.
 *
 * Returns: TRUE  Success.
 *          FLASE  Fail.
 ----------------------------------------------------------------------------*/
BOOL divx_drm_mem_write (
    UINT8* pui1_divx_drm_mem)
{
    HANDLE_T h_eep = NULL_HANDLE;
    INT32 i4_ret;
    UINT32 ui4_n;
    UINT32 ui4_written;
    UINT32 ui4_count;    
    UINT64 ui8_pos;

    /* open the EEPROM file */
    i4_ret = x_fm_open(FM_ROOT_HANDLE, (SN_DEV_PATH "/" SN_EEPROM_3), FM_READ_WRITE, 0777, TRUE, &h_eep);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR((_ERROR_HEADER"%s: FM opens the divx drm memory failed, return=%d\n", __FUNCTION__, i4_ret));
        printk("FM opens the divx drm memory failed\n");

        return FALSE;
    }

    i4_ret = x_fm_lseek(h_eep, (INT64)ui2_divx_drm_memory_offset, FM_SEEK_BGN, &ui8_pos);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR((_ERROR_HEADER"%s: FM seeks the divx drm memory failed, return=%d\n", __FUNCTION__, i4_ret));
        printk("FM seeks the divx drm memory failed\n");

        x_fm_close(h_eep);        

        return FALSE;
    }
    
    ui4_written = 0;
    ui4_count = ui2_divx_drm_memory_size_byte;
    do
    {
        i4_ret = x_fm_write(h_eep, pui1_divx_drm_mem + ui4_written, ui4_count, &ui4_n);
        if (i4_ret == FMR_EOF)
        {
            break;
        }
        else if (i4_ret != FMR_OK)
        {
            DBG_ERROR((_ERROR_HEADER"%s: FM writes the divx drm memory failed, return=%d\n", __FUNCTION__, i4_ret));
            printk("FM writes the divx drm memory failed\n");

            x_fm_close(h_eep);            

            return FALSE;
        }
        ui4_written = ui4_written + ui4_n;
        ui4_count = ui4_count - ui4_n;
    } while (ui4_count > 0);

    i4_ret = x_fm_close(h_eep);
    if (i4_ret != FMR_OK)
    {
        DBG_ERROR((_ERROR_HEADER"%s: FM closes the divx drm memory failed, return=%d\n", __FUNCTION__, i4_ret));
        printk("FM closes the divx drm memory failed\n");

        return FALSE;
    }

    return TRUE;
}

#endif

