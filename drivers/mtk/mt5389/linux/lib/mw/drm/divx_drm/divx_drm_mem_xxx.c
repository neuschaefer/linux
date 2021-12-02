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
 * $RCSfile: divx_drm_mem_xxx.c,v $
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

#ifdef __KERNEL__
/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "x_common.h"
#include "x_dbg.h"
#include "x_os.h"
#include "../drm_dbg.h"
#include "x_divx_drm.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
static UINT16 ui2_divx_drm_memory_offset_xxx;
static UINT16 ui2_divx_drm_memory_size_byte_xxx;

static UINT8 _aui1_divx_drm_memory_eeprom[56];  // simulate EEPROM, tmp solution

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: divx_drm_mem_init_xxx
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
INT32 divx_drm_mem_init_xxx (
    DIVX_DRM_INIT_CONFIG_T* pt_divx_drm_init_config)
{
    if (0 == pt_divx_drm_init_config->ui2_divx_drm_memory_size_byte)
    {
        return FALSE;
    }

    ui2_divx_drm_memory_offset_xxx = pt_divx_drm_init_config->ui2_divx_drm_memory_offset;
    ui2_divx_drm_memory_size_byte_xxx = pt_divx_drm_init_config->ui2_divx_drm_memory_size_byte;

    DBG_INFO((_INFO_HEADER"%s: divx drm memory: offset=%d, size=%d\n", __FUNCTION__, ui2_divx_drm_memory_offset_xxx, ui2_divx_drm_memory_size_byte_xxx));

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_mem_info_xxx
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
VOID divx_drm_mem_info_xxx (
    UINT16* pui2_divx_drm_memory_offset,
    UINT16* pui2_divx_drm_memory_size_byte)
{
    *pui2_divx_drm_memory_offset = ui2_divx_drm_memory_offset_xxx;
    *pui2_divx_drm_memory_size_byte = ui2_divx_drm_memory_size_byte_xxx;
}
 
/*-----------------------------------------------------------------------------
 * Name: divx_drm_mem_read_xxx
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
BOOL divx_drm_mem_read_xxx (
    UINT8* pui1_divx_drm_mem)
{
    if (ui2_divx_drm_memory_size_byte_xxx < 56)
    {
        // read pui1_divx_drm_mem from EEPROM
        x_memcpy(pui1_divx_drm_mem, _aui1_divx_drm_memory_eeprom, ui2_divx_drm_memory_size_byte_xxx);
    }
    
    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_mem_write_xxx
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
BOOL divx_drm_mem_write_xxx (
    UINT8* pui1_divx_drm_mem)
{
    if (ui2_divx_drm_memory_size_byte_xxx < 56)
    {
        // write pui1_divx_drm_mem to EEPROM
        x_memcpy(_aui1_divx_drm_memory_eeprom, pui1_divx_drm_mem, ui2_divx_drm_memory_size_byte_xxx);
    }
    
    return TRUE;
}

#endif

