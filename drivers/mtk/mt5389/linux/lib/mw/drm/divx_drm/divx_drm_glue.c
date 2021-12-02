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
 * $RCSfile: divx_drm_glue.c,v $
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

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/x_common.h"
#include "dbg/x_dbg.h"
#include "drm/drm_dbg.h"
#include "os/inc/x_os.h"
#include "util/x_crc32.h"    /* crc32 */

#include "drm/divx_drm/x_divx_drm.h"
#include "drm/divx_drm/divx_drm_hw_key.h"
#include "drm/divx_drm/divx_drm_mem.h"

#ifdef DIVX_DRM_40
#include "drm/divx_drm/divx_drm_40/AdpLib/DrmAdpTypes.h"
#include "drm/divx_drm/divx_drm_40/AdpLocalLib/DrmAdpLocal.h"
#elif defined(DIVX_DRM_51)
#include "drm/divx_drm/divx_drm_51/DrmTypes.h"
#include "drm/divx_drm/divx_drm_51/DrmDataLoad.h"
#endif

#else

#include "x_common.h"
#include "x_dbg.h"
#include "../drm_dbg.h"
#include "x_os.h"
/* #include "x_crc32.h" */    /* crc32 */

#include "x_divx_drm.h"

#include "divx_drm_mem_xxx.h"  // megaa 20090202

#ifdef DIVX_DRM_40
#include "divx_drm_40/AdpLib/DrmAdpTypes.h"
#include "divx_drm_40/AdpLocalLib/DrmAdpLocal.h"
#elif defined(DIVX_DRM_51)
#include "divx_drm_51/DrmTypes.h"
#include "divx_drm_51/DrmDataLoad.h"
#endif

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/* The following defines the extra fields added by MTK in the preceding of DRM memory. */
/* 4 bytes CRC32 checksum + 4 bytes version + 48 bytes payload */
#define DRM_MEMORY_CRC32_CHECKSUM_SIZE_BYTE 4
#define DRM_MEMORY_VERSION_SIZE_BYTE 4

/* The following defines DRM memory versions. */
#define DRM_MEMORY_VERSION_HT22 ((UINT32) 1 << 0)
#define DRM_MEMORY_VERSION_HT30 ((UINT32) 1 << 1)
#define DRM_MEMORY_VERSION_HD ((UINT32) 1 << 2)

/* The following defines default model id. */
#define MODEL_ID_LOW_BYTE  ((UINT8) (0x31))	        /* 0x31 is only simulation mode. */
#define MODEL_ID_HIGH_BYTE ((UINT8) (0x30 & 0xF0))  /* Only half of the byte counts. 0x30 is only for simulation. */

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
UINT8 gui1_divx_drm_model_id_low_byte = 0;
UINT8 gui1_divx_drm_model_id_high_byte = 0;

/* Configure DRM memory version. */
#ifdef DIVX_DRM_40

const UINT32 ui4_drm_memory_version = DRM_MEMORY_VERSION_HT22;

#elif defined (DIVX_DRM_51)

#ifdef DIVX_DRM_MEM_HT30

const UINT32 ui4_drm_memory_version = DRM_MEMORY_VERSION_HT30;

#elif defined (DIVX_DRM_MEM_HD)

const UINT32 ui4_drm_memory_version = DRM_MEMORY_VERSION_HD;

#endif /* DIVX_DRM_MEM_HT30 */

#endif /* DIVX_DRM_40 */

/* CRC32 checksum utility and HW secret key (tmp solution) for Chiling. */
#ifdef __KERNEL__

static UINT32 aui4_crc32[256] =
{
    0x00000000, 0x04c11db7, 0x09823b6e, 0x0d4326d9,
    0x130476dc, 0x17c56b6b, 0x1a864db2, 0x1e475005,
    0x2608edb8, 0x22c9f00f, 0x2f8ad6d6, 0x2b4bcb61,
    0x350c9b64, 0x31cd86d3, 0x3c8ea00a, 0x384fbdbd,
    0x4c11db70, 0x48d0c6c7, 0x4593e01e, 0x4152fda9,
    0x5f15adac, 0x5bd4b01b, 0x569796c2, 0x52568b75,
    0x6a1936c8, 0x6ed82b7f, 0x639b0da6, 0x675a1011,
    0x791d4014, 0x7ddc5da3, 0x709f7b7a, 0x745e66cd,
    0x9823b6e0, 0x9ce2ab57, 0x91a18d8e, 0x95609039,
    0x8b27c03c, 0x8fe6dd8b, 0x82a5fb52, 0x8664e6e5,
    0xbe2b5b58, 0xbaea46ef, 0xb7a96036, 0xb3687d81,
    0xad2f2d84, 0xa9ee3033, 0xa4ad16ea, 0xa06c0b5d,
    0xd4326d90, 0xd0f37027, 0xddb056fe, 0xd9714b49,
    0xc7361b4c, 0xc3f706fb, 0xceb42022, 0xca753d95,
    0xf23a8028, 0xf6fb9d9f, 0xfbb8bb46, 0xff79a6f1,
    0xe13ef6f4, 0xe5ffeb43, 0xe8bccd9a, 0xec7dd02d,
    0x34867077, 0x30476dc0, 0x3d044b19, 0x39c556ae,
    0x278206ab, 0x23431b1c, 0x2e003dc5, 0x2ac12072,
    0x128e9dcf, 0x164f8078, 0x1b0ca6a1, 0x1fcdbb16,
    0x018aeb13, 0x054bf6a4, 0x0808d07d, 0x0cc9cdca,
    0x7897ab07, 0x7c56b6b0, 0x71159069, 0x75d48dde,
    0x6b93dddb, 0x6f52c06c, 0x6211e6b5, 0x66d0fb02,
    0x5e9f46bf, 0x5a5e5b08, 0x571d7dd1, 0x53dc6066,
    0x4d9b3063, 0x495a2dd4, 0x44190b0d, 0x40d816ba,
    0xaca5c697, 0xa864db20, 0xa527fdf9, 0xa1e6e04e,
    0xbfa1b04b, 0xbb60adfc, 0xb6238b25, 0xb2e29692,
    0x8aad2b2f, 0x8e6c3698, 0x832f1041, 0x87ee0df6,
    0x99a95df3, 0x9d684044, 0x902b669d, 0x94ea7b2a,
    0xe0b41de7, 0xe4750050, 0xe9362689, 0xedf73b3e,
    0xf3b06b3b, 0xf771768c, 0xfa325055, 0xfef34de2,
    0xc6bcf05f, 0xc27dede8, 0xcf3ecb31, 0xcbffd686,
    0xd5b88683, 0xd1799b34, 0xdc3abded, 0xd8fba05a,
    0x690ce0ee, 0x6dcdfd59, 0x608edb80, 0x644fc637,
    0x7a089632, 0x7ec98b85, 0x738aad5c, 0x774bb0eb,
    0x4f040d56, 0x4bc510e1, 0x46863638, 0x42472b8f,
    0x5c007b8a, 0x58c1663d, 0x558240e4, 0x51435d53,
    0x251d3b9e, 0x21dc2629, 0x2c9f00f0, 0x285e1d47,
    0x36194d42, 0x32d850f5, 0x3f9b762c, 0x3b5a6b9b,
    0x0315d626, 0x07d4cb91, 0x0a97ed48, 0x0e56f0ff,
    0x1011a0fa, 0x14d0bd4d, 0x19939b94, 0x1d528623,
    0xf12f560e, 0xf5ee4bb9, 0xf8ad6d60, 0xfc6c70d7,
    0xe22b20d2, 0xe6ea3d65, 0xeba91bbc, 0xef68060b,
    0xd727bbb6, 0xd3e6a601, 0xdea580d8, 0xda649d6f,
    0xc423cd6a, 0xc0e2d0dd, 0xcda1f604, 0xc960ebb3,
    0xbd3e8d7e, 0xb9ff90c9, 0xb4bcb610, 0xb07daba7,
    0xae3afba2, 0xaafbe615, 0xa7b8c0cc, 0xa379dd7b,
    0x9b3660c6, 0x9ff77d71, 0x92b45ba8, 0x9675461f,
    0x8832161a, 0x8cf30bad, 0x81b02d74, 0x857130c3,
    0x5d8a9099, 0x594b8d2e, 0x5408abf7, 0x50c9b640,
    0x4e8ee645, 0x4a4ffbf2, 0x470cdd2b, 0x43cdc09c,
    0x7b827d21, 0x7f436096, 0x7200464f, 0x76c15bf8,
    0x68860bfd, 0x6c47164a, 0x61043093, 0x65c52d24,
    0x119b4be9, 0x155a565e, 0x18197087, 0x1cd86d30,
    0x029f3d35, 0x065e2082, 0x0b1d065b, 0x0fdc1bec,
    0x3793a651, 0x3352bbe6, 0x3e119d3f, 0x3ad08088,
    0x2497d08d, 0x2056cd3a, 0x2d15ebe3, 0x29d4f654,
    0xc5a92679, 0xc1683bce, 0xcc2b1d17, 0xc8ea00a0,
    0xd6ad50a5, 0xd26c4d12, 0xdf2f6bcb, 0xdbee767c,
    0xe3a1cbc1, 0xe760d676, 0xea23f0af, 0xeee2ed18,
    0xf0a5bd1d, 0xf464a0aa, 0xf9278673, 0xfde69bc4,
    0x89b8fd09, 0x8d79e0be, 0x803ac667, 0x84fbdbd0,
    0x9abc8bd5, 0x9e7d9662, 0x933eb0bb, 0x97ffad0c,
    0xafb010b1, 0xab710d06, 0xa6322bdf, 0xa2f33668,
    0xbcb4666d, 0xb8757bda, 0xb5365d03, 0xb1f740b4
};

static UINT8 _aui1DspRom[] = 
{
      0xFF, 0xFF, 0x00, 0x00, 0x62, 0xFF, 0xF0, 0x36, 
      0x88, 0xFD, 0xE3, 0x6D, 0x73, 0xFA, 0xD9, 0xA4, 
      0x22, 0xF6, 0xD5, 0xDB, 0x94, 0xF0, 0xD9, 0x12, 
      0xCC, 0xE9, 0xE7, 0x49, 0xC7, 0xE1, 0x00, 0x81  
};

#endif /* __KERNEL__ */

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
 #ifdef __KERNEL__
 /*----------------------------------------------------------------------------
 * Function: x_crc32_gen()
 *
 * Description:
 *      generate a CRC32 check-sum value from an array of data pointed to
 *      by "pui1_data" over "z_size" bytes.
 *
 *      This function can be called cyclic to generate a CRC32 value 
 *      from multiple data arrays.
 *
 *      
 *      Usage info: Before calling this API, the crc_seed value needs to
 *      initialized to 0xFFFFFFFF.
 *
 *           ui4_crc_seed = 0xFFFFFFFF;
 *
 *           for (i=1; i< num_data_sect ; i++ )
 *           {
 *              pui1_data = data_sections[i];
 *              z_size    = data_section_size[i];
 *              ui4_result = x_crc32_gen(pui1_data, z_size, ui4_crc_seed) ;
 *              ui4_crc_seed = ui4_result ;
 *           }
 *
 *     The result CRC32 value need to convert ot Big-end format.
 *     ui4_result contain the CRC32 value computed over buffer
 *     data_section[num_data_sect]
 *           
 *           PUT_UINT32_TO_PTR_BIG_END(ui4_crc_seed, &ui4_result);
 *
 *     To check the if the CRC value is valid for the data buffer, the same
 *     API is used to compute CRC value over the length of the data buffer plus
 *     the 4 bytes containing the original CRC value. The resulting value should
 *     be 0 if the contain of the data buffer has not change.
 *   
 *           ui4_crc_seed = 0xFFFFFFFF;
 *
 *           for (i=1; i< num_data_sect ; i++ )
 *           {
 *              pui1_data = data_sections[i];
 *              z_size    = data_section_size[i];
 *              ui4_rc = x_crc32_gen(pui1_data, z_size, ui4_crc_seed) ;
 *              ui4_crc_seed = ui4_rc;
 *           }
 *
 *     Now compute the CRC value with the previous CRC value.
 *
 *           if ( x_crc32_gen(&ui4_result, 4, ui4_crc_seed) == 0 )
 *           {
 *                checksum is ok. 
 *           }
 *           else
 *           {
 *                data buffer has changed.
 *           }
 *
 *
 * Inputs:
 *      pui1_data: an array of data to generate its CRC32
 *      z_size:    byte count of array of data "pui1_data"
 *      ui4_crc:   crc32 seed value to generate the check sum value from
 *                 'pui1_data' buffer.
 *
 * Outputs:
 *      None
 *
 * Returns:
 *      a CRC32 value
 *      0xFFFFFFFF     If *pui1_data = NULL or z_size = 0. ???
 *---------------------------------------------------------------------------*/
static UINT32 x_crc32_gen (
    const UINT8* pui1_data,
    SIZE_T z_size,
    UINT32 ui4_crc)
{
    SIZE_T z_i;
    
    if ( (pui1_data != (UINT8 *) NULL) &&
         (z_size > 0) )
    {
        for (z_i = (SIZE_T) 0; z_i < z_size; z_i++)
        {
            ui4_crc = ( ui4_crc << 8) ^
                aui4_crc32[( ui4_crc >> 24) ^ pui1_data[z_i]];
        }
    }
    return(ui4_crc);
}
#endif /* __KERNEL__ */

/*-----------------------------------------------------------------------------
 * Name: divx_drm_hw_secret_key_glue
 *
 * Description: This API retrieves the divx drm hardware secret key.
 *
 * Inputs: pui1_divx_drm_hw_secret_key  Reference to retrieve the divx drm hardware secret key.
 *
 * Outputs: pui1_divx_drm_hw_secret_key  Reference contains the divx drm hardware secret key.
 *
 * Returns: TRUE  Success.
 *          FALSE  Fail.
 ----------------------------------------------------------------------------*/
BOOL divx_drm_hw_secret_key_glue (
    UINT8* pui1_divx_drm_hw_secret_key)
{
    if (NULL == pui1_divx_drm_hw_secret_key)
        return FALSE;

#ifndef __KERNEL__
    return divx_drm_hw_secret_key(pui1_divx_drm_hw_secret_key);
#else
    /* driver implementation */
    x_memcpy(pui1_divx_drm_hw_secret_key, _aui1DspRom, 32);  // megaa 20090202 tmp solution
#endif
}

/*-----------------------------------------------------------------------------
 * Name: _divx_drm_model_id_init
 *
 * Description: This API initializes the divx drm model id.
 *
 * Inputs: t_divx_drm_init_config  Reference contains the divx drm configuration structure.
 *
 * Outputs: None.
 *
 * Returns: TRUE  Success.
 ----------------------------------------------------------------------------*/
static BOOL _divx_drm_model_id_init (
    DIVX_DRM_INIT_CONFIG_T* pt_divx_drm_init_config)
{
    if (0 == pt_divx_drm_init_config->ui1_divx_drm_model_id_low_byte &&
        0 == pt_divx_drm_init_config->ui1_divx_drm_model_id_high_byte)
    {
        gui1_divx_drm_model_id_low_byte = MODEL_ID_LOW_BYTE;
        gui1_divx_drm_model_id_high_byte = MODEL_ID_HIGH_BYTE;

        DBG_INFO((_INFO_HEADER"%s: divx drm model id (simulation mode): low byte =%d, high byte=%d\n", __FUNCTION__, gui1_divx_drm_model_id_low_byte, gui1_divx_drm_model_id_high_byte));
    }
    else
    {
        gui1_divx_drm_model_id_low_byte = pt_divx_drm_init_config->ui1_divx_drm_model_id_low_byte;
        gui1_divx_drm_model_id_high_byte = pt_divx_drm_init_config->ui1_divx_drm_model_id_high_byte;

        DBG_INFO((_INFO_HEADER"%s: divx drm model id: low byte =%d, high byte=%d\n", __FUNCTION__, gui1_divx_drm_model_id_low_byte, gui1_divx_drm_model_id_high_byte));
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_init_glue
 *
 * Description: This API initializes the divx drm memory and model id.
 *
 * Inputs: t_divx_drm_init_config  Reference contains the divx drm configuration structure.
 *
 * Outputs: None.
 *
 * Returns: DRMR_OK  Success. The API is successful. 
 *          DRMR_INV_ARG  Fail.
 *          DRMR_INTERNAL_ERROR  Fail.
 ----------------------------------------------------------------------------*/
INT32 divx_drm_init_glue (
    DIVX_DRM_INIT_CONFIG_T* pt_divx_drm_init_config)
{
    UINT8 aui1_divx_drm_memory[DRM_MEMORY_CRC32_CHECKSUM_SIZE_BYTE + 
                               DRM_MEMORY_VERSION_SIZE_BYTE + 
                               PACKED_ALLOCATION_BYTES];
    UINT32 ui4_crc32 = 0;
    BOOL b_reset_drm_memory = FALSE;

    if (NULL == pt_divx_drm_init_config)
    {
        return DRMR_INV_ARG;
    }
    
    if (!_divx_drm_model_id_init(pt_divx_drm_init_config))
    {
        return DRMR_INV_ARG;
    }  

#ifndef __KERNEL__
    if (!divx_drm_mem_init(pt_divx_drm_init_config))
#else
    if (!divx_drm_mem_init_xxx(pt_divx_drm_init_config)) /* driver implementation, megaa 20090202 */
#endif
    {
        return DRMR_INV_ARG;
    }

    /* check the divx drm memory access */
#ifndef __KERNEL__
    if (!divx_drm_mem_read(aui1_divx_drm_memory))
#else
    if (!divx_drm_mem_read_xxx(aui1_divx_drm_memory)) /* driver implementation, megaa 20090202 */
#endif
    {
        DBG_ERROR((_ERROR_HEADER"%s: Read the divx drm memory failed\n", __FUNCTION__));
        printk("Read the divx drm memory failed\n");

        b_reset_drm_memory = TRUE;
    }
    else
    {
        /* verify CRC32 checksum */
        ui4_crc32 = x_crc32_gen(aui1_divx_drm_memory + DRM_MEMORY_CRC32_CHECKSUM_SIZE_BYTE, 
            DRM_MEMORY_VERSION_SIZE_BYTE + PACKED_ALLOCATION_BYTES, (UINT32) 0x00000000);
        if (ui4_crc32 != GET_UINT32_FROM_PTR_BIG_END(aui1_divx_drm_memory))
        {
            DBG_ERROR((_ERROR_HEADER"%s: CRC32 checksum error\n", __FUNCTION__));
            printk("CRC32 checksum error\n");

            b_reset_drm_memory = TRUE;    
        }

        /* check version */
        if ( ui4_drm_memory_version != GET_UINT32_FROM_PTR_BIG_END(aui1_divx_drm_memory + DRM_MEMORY_CRC32_CHECKSUM_SIZE_BYTE))
        {
            DBG_INFO((_INFO_HEADER"%s: Version mismatch\n", __FUNCTION__));

            b_reset_drm_memory = TRUE;
        }
    }

    /* reset the divx drm memory */
    if (b_reset_drm_memory)
    {
        x_memset(aui1_divx_drm_memory, 0, DRM_MEMORY_CRC32_CHECKSUM_SIZE_BYTE + 
                                          DRM_MEMORY_VERSION_SIZE_BYTE + 
                                          PACKED_ALLOCATION_BYTES);

        /* Can't call divx_drm_mem_write directly. In SDK 5.1, the divx drm memory is been scrambled and encrypted. */
#ifdef DIVX_DRM_40
        if (ADP_LOCAL_SUCCESS != localSaveDrmMemory(aui1_divx_drm_memory))
#elif defined(DIVX_DRM_51)
        if (0 != drmDataLoadSaveDrmMemory(aui1_divx_drm_memory, PACKED_ALLOCATION_BYTES))
#endif
        {
            DBG_ERROR((_ERROR_HEADER"%s: Reset the divx drm memory failed\n", __FUNCTION__));
            printk("Reset the divx drm memory failed\n");

            return DRMR_INTERNAL_ERROR;
        }

        DBG_INFO((_INFO_HEADER"%s: Reset the divx drm memory\n", __FUNCTION__));
    }

    return DRMR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_mem_read_glue
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
BOOL divx_drm_mem_read_glue (
    UINT8* pui1_divx_drm_mem)
{
    UINT8 aui1_divx_drm_memory[DRM_MEMORY_CRC32_CHECKSUM_SIZE_BYTE + 
                               DRM_MEMORY_VERSION_SIZE_BYTE + 
                               PACKED_ALLOCATION_BYTES];
    UINT32 ui4_crc32 = 0;

    if (NULL == pui1_divx_drm_mem)
    {
        return FALSE;
    }

#ifndef __KERNEL__
    if (!divx_drm_mem_read(aui1_divx_drm_memory))
#else
    if (!divx_drm_mem_read_xxx(aui1_divx_drm_memory)) /* driver implementation, megaa 20090202 */
#endif
    {
        return FALSE;
    }

    /* verify CRC32 checksum */
    ui4_crc32 = x_crc32_gen(aui1_divx_drm_memory + DRM_MEMORY_CRC32_CHECKSUM_SIZE_BYTE, 
        DRM_MEMORY_VERSION_SIZE_BYTE + PACKED_ALLOCATION_BYTES, (UINT32) 0x00);
    if (ui4_crc32 != GET_UINT32_FROM_PTR_BIG_END(aui1_divx_drm_memory))
    {
        DBG_ERROR((_ERROR_HEADER"%s: CRC32 checksum error\n", __FUNCTION__));
        printk("CRC32 checksum error\n");
        return FALSE;
    }    

    x_memcpy(pui1_divx_drm_mem, aui1_divx_drm_memory + DRM_MEMORY_CRC32_CHECKSUM_SIZE_BYTE + 
        DRM_MEMORY_VERSION_SIZE_BYTE, PACKED_ALLOCATION_BYTES);

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_mem_write_glue
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
BOOL divx_drm_mem_write_glue (
    UINT8* pui1_divx_drm_mem)
{
    UINT8 aui1_divx_drm_memory[DRM_MEMORY_CRC32_CHECKSUM_SIZE_BYTE + 
                               DRM_MEMORY_VERSION_SIZE_BYTE + 
                               PACKED_ALLOCATION_BYTES];

    if (NULL == pui1_divx_drm_mem)
        return FALSE;

    x_memcpy(aui1_divx_drm_memory + DRM_MEMORY_CRC32_CHECKSUM_SIZE_BYTE + DRM_MEMORY_VERSION_SIZE_BYTE, 
        pui1_divx_drm_mem, PACKED_ALLOCATION_BYTES);

    /* add version */
    PUT_UINT32_TO_PTR_BIG_END(ui4_drm_memory_version, aui1_divx_drm_memory + DRM_MEMORY_CRC32_CHECKSUM_SIZE_BYTE);

    /* add CRC32 checksum */
    PUT_UINT32_TO_PTR_BIG_END(x_crc32_gen(aui1_divx_drm_memory + DRM_MEMORY_CRC32_CHECKSUM_SIZE_BYTE, 
        DRM_MEMORY_VERSION_SIZE_BYTE + PACKED_ALLOCATION_BYTES, (UINT32) 0x00), aui1_divx_drm_memory);

#ifndef __KERNEL__
    if (!divx_drm_mem_write(aui1_divx_drm_memory))
#else
    if (!divx_drm_mem_write_xxx(aui1_divx_drm_memory)) /* driver implementation, megaa 20090202 */
#endif
    {
        return FALSE;
    }

    return TRUE;
}

