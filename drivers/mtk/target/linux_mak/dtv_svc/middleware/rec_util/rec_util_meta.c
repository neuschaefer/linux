/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2004, CrystalMedia Technology, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $RCSfile:  $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This header file contains Record Utitliy Metadata implementations.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#include "rec_util/rec_util_meta.h"
#include "rec_util/rec_util.h"
#include "rec_util/rec_util_utils.h"
#include "rec_util/_rec_util.h"
#include "util/x_crc32.h" 
#include "file_mngr/x_fm.h"
#include "file_mngr/x_fm_mfw.h"
#include "os/inc/x_os.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define RU_META_RFI_NODE_BUF_INIT_PADDING   16

#ifndef ALIGN_32
    #define ALIGN_32(_ui4_val)   (((((UINT32) (_ui4_val)) + 3) >> 2) << 2)
#endif

#ifndef MAX
    #define MAX(x, y)           (((x) > (y)) ? (x) : (y))    
#endif

#ifndef MIN
    #define MIN(x, y)           (((x) < (y)) ? (x) : (y))    
#endif

#define RU_META_RFI_NODE_GET_DATA(_pt_node)     \
            ((VOID *) (((UINT32) (_pt_node)) + ALIGN_32 (sizeof (RU_META_RFI_NODE_T))))
#define RU_META_RFI_NODE_CALC_SIZE(_z_data)     \
            ((SIZE_T) (ALIGN_32 (sizeof (RU_META_RFI_NODE_T)) + (_z_data)))

/*-----------------------------------------------------------------------------
                    structures
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_meta_tool_get_ext_file (
    const CHAR*             s_head_file,
    SIZE_T                  z_head_file,
    RU_META_FILE_TYPE_T     e_file_type,
    CHAR*                   s_ext_file,
    SIZE_T                  z_ext_buf);


/*-----------------------------------------------------------------------------
                    functions (tools)
 ----------------------------------------------------------------------------*/
INT32 rec_util_check_pvr_file (
    const CHAR*             s_file_path,
    BOOL*                   pb_pvr_file)
{
    INT32               i4_ret;
    FM_FILE_INFO_T      t_file_info;
    SIZE_T              z_path_len;
    
    *pb_pvr_file = FALSE;
    
    /* determine by sub file name in current stage */
    i4_ret = x_fm_mfw_get_info_by_name (FM_ROOT_HANDLE, 
                                        s_file_path,
                                        & t_file_info );
    if (i4_ret == FMR_OK)
    {
        z_path_len = x_strlen (s_file_path);
        
        if (z_path_len > (RU_META_FILE_EXT_LEN_PVR + 1))
        {
            if ( s_file_path[z_path_len - RU_META_FILE_EXT_LEN_PVR - 1] == '.' &&
                !x_strcasecmp (s_file_path + z_path_len - RU_META_FILE_EXT_LEN_PVR, RU_META_FILE_EXT_PVR ))
            {
                *pb_pvr_file = TRUE;
            }
        }
        else
        {
            *pb_pvr_file = FALSE;
        }
    }
    else
    {
        return REC_UTILR_INV_ARG;
    }    
    
    return REC_UTILR_OK;
}

INT32 rec_util_del_rec_data (
    const CHAR*             s_head_file)
{
    BOOL        b_pvr_file;
    INT32       i4_ret;
    
    i4_ret = rec_util_check_pvr_file (s_head_file, 
                                      & b_pvr_file);
    if (i4_ret != REC_UTILR_OK)
    {
        REC_UTIL_DBG_ERR (("Fail to check pvr file (%d). (L%d)\n", i4_ret, __LINE__));
        b_pvr_file = FALSE;
    }
    
    /* check if there is accompanying metadata file and also delete it */
    if (b_pvr_file)
    {
        SIZE_T   z_head_file = x_strlen (s_head_file);
        SIZE_T   z_rfi_buf;
        CHAR*    s_rfi_buf;
        
        z_rfi_buf = z_head_file + RU_META_FILE_EXT_LEN_RFI - RU_META_FILE_EXT_LEN_PVR + 1;
        s_rfi_buf = (CHAR *) x_mem_alloc (z_rfi_buf);
        if (s_rfi_buf != NULL)
        {
            /* get RFI file name */
            i4_ret = _rec_util_meta_tool_get_ext_file (s_head_file, 
                                                       z_head_file,
                                                       RU_META_FILE_TYPE_RFI,
                                                       s_rfi_buf,
                                                       z_rfi_buf);
            if (i4_ret != REC_UTILR_OK)
            {
                REC_UTIL_DBG_ERR (("Fail to convert rfi file path (%d). (L%d)\n", i4_ret, __LINE__));
            }    
            
            /* delete RFI file */
            i4_ret = x_fm_delete_file (FM_ROOT_HANDLE,
                                       s_rfi_buf);
            if (i4_ret != FMR_OK)
            {
                if (i4_ret != FMR_NO_ENTRY)
                {
                    REC_UTIL_DBG_ERR (("Fail to delete file (%d). (L%d)\n", i4_ret, __LINE__));
                }
            }
            
            x_mem_free ((VOID *) s_rfi_buf);
        }
        else
        {
            REC_UTIL_DBG_ERR (("Out of memory. (L%d)\n", __LINE__));
        }
    }
    else
    {
        /* no accompanying files for non-pvr file in current design */
    }
    
    i4_ret = x_fm_mfw_delete (FM_ROOT_HANDLE,
                              s_head_file);
    if (i4_ret != FMR_OK)
    {
        REC_UTIL_DBG_ERR (("Fail to mfw-delete file (%d). (L%d)\n", i4_ret, __LINE__));
    }
    
    return REC_UTILR_OK;
}    

static INT32 _rec_util_meta_tool_expand_buf (
    VOID*                   pv_buf_orig,
    SIZE_T                  z_buf_orig,
    SIZE_T                  z_new_sz,
    VOID**                  ppv_buf_new,
    SIZE_T*                 pz_buf_new)
{
    *ppv_buf_new = pv_buf_orig;
    *pz_buf_new  = z_buf_orig;

    if (z_new_sz <= z_buf_orig)
    {
        *ppv_buf_new = pv_buf_orig;
        *pz_buf_new  = z_buf_orig;
    }
    else
    {
        VOID*    pv_new_alloc;
        SIZE_T   z_new_alloc;
        
        z_new_alloc = z_new_sz * 2;
        pv_new_alloc = x_mem_alloc (z_new_alloc);
        if (pv_new_alloc == NULL)
        {
            REC_UTIL_DBG_ERR (("Out of memory (size = %d). (L%d)\n", z_new_alloc, __LINE__));
            return REC_UTILR_OUT_OF_MEMORY;
        }
        
        if (pv_buf_orig != NULL)
        {
            x_mem_free (pv_buf_orig);
        }
        
        *ppv_buf_new = pv_new_alloc;
        *pz_buf_new  = z_new_alloc;
    }
    
    return REC_UTILR_OK;
}
    
static INT32 _rec_util_meta_tool_get_ext_file (
    const CHAR*             s_head_file,
    SIZE_T                  z_head_file,
    RU_META_FILE_TYPE_T     e_file_type,
    CHAR*                   s_ext_file,
    SIZE_T                  z_ext_buf)
{
    SIZE_T    z_head_symbol = RU_META_FILE_EXT_LEN_PVR;
    CHAR*     s_ext_symbol;
    SIZE_T    z_ext_symbol;
    BOOL      b_pvr_file;
    INT32     i4_ret;
    
    /* s_head_file must be a PVR file */
    i4_ret = rec_util_check_pvr_file (s_head_file, 
                                      & b_pvr_file);
    if (i4_ret != REC_UTILR_OK)
    {
        return i4_ret;
    }
    
    if (!b_pvr_file)
    {
        return REC_UTILR_INV_ARG;
    }
    
    switch (e_file_type)
    {
    case RU_META_FILE_TYPE_RFI:
        s_ext_symbol = RU_META_FILE_EXT_RFI;
        z_ext_symbol = RU_META_FILE_EXT_LEN_RFI;
        break;
        
    default:
        return REC_UTILR_INV_ARG;
    }
    
    /* check buffer size */
    if (z_head_file <= (z_head_symbol + 1) ||
        ((z_head_file - z_head_symbol + z_ext_symbol + 1) > z_ext_buf))
    {
        REC_UTIL_DBG_ERR (("Invalid arguments. (L%d)\n", __LINE__));
        return REC_UTILR_INV_ARG;
    }
    
    x_strncpy (s_ext_file, s_head_file, z_head_file - z_head_symbol);
    x_strncpy (s_ext_file + (z_head_file - z_head_symbol), s_ext_symbol, z_ext_symbol);
    s_ext_file[z_head_file - z_head_symbol + z_ext_symbol] = '\0';
    
    return REC_UTILR_OK;
}

static VOID _rec_util_meta_tool_scramble (
    UINT32                  ui4_seed,
    const VOID*             pv_data,
    SIZE_T                  z_data,
    VOID*                   pv_result)
{
    UINT8*  aui1_data   = (UINT8 *) pv_data;
    UINT8*  aui1_result = (UINT8 *) pv_result;
    UINT32  ui4_shift, ui4_i, ui4_k;
    UINT8   ui1_mask_low;
    UINT32  ui4_mid_pt;
    UINT8   ui1_temp;
    
    if (pv_data == NULL ||
        z_data == 0     ||
        pv_result == NULL )
    {
        REC_UTIL_ABORT(REC_UTIL_DBG_ABRT_INTERNAL_ERROR);
        return;
    }
    
    ui4_shift = (ui4_seed % 3) + 3;
    ui1_mask_low = (((UINT8) 1) << ui4_shift) - 1;
    
    for (ui4_i = 0; ui4_i < ((UINT32) z_data - 1); ui4_i++)
    {
        aui1_result[ui4_i] = ((aui1_data[ui4_i] >> ui4_shift) |
                               ((aui1_data[ui4_i + 1] & ui1_mask_low) << (8 - ui4_shift)));
    }
    aui1_result[z_data - 1] = ((aui1_data[z_data - 1] >> ui4_shift) |
                               ((aui1_data[0] & ui1_mask_low) << (8 - ui4_shift)));
    
    ui4_mid_pt = (UINT32) (z_data / 2);
    for (ui4_i = 1; ui4_i < ui4_mid_pt; ui4_i += 2)
    {
        ui4_k = ui4_mid_pt - ui4_i;
        ui1_temp = aui1_result[ui4_k];
        aui1_result[ui4_k] = aui1_result[z_data - ui4_k - 1];
        aui1_result[z_data - ui4_k - 1] = ui1_temp;
    }
}

static VOID _rec_util_meta_tool_descramble (
    UINT32                  ui4_seed,
    const VOID*             pv_data,
    SIZE_T                  z_data,
    VOID*                   pv_result)
{
    UINT8*  aui1_data   = (UINT8 *) pv_data;
    UINT8*  aui1_result = (UINT8 *) pv_result;
    UINT32  ui4_shift, ui4_i, ui4_k;
    UINT8   ui1_mask_high;
    UINT32  ui4_mid_pt;
    UINT8   ui1_temp, ui1_bak_high;
    
    if (pv_data == NULL ||
        z_data == 0     ||
        pv_result == NULL )
    {
        REC_UTIL_ABORT (REC_UTIL_DBG_ABRT_INTERNAL_ERROR);
        return;
    }
    
    x_memcpy (aui1_result, aui1_data, z_data);
    
    ui4_mid_pt = (UINT32) (z_data / 2);
    for (ui4_i = 1; ui4_i < ui4_mid_pt; ui4_i += 2)
    {
        ui4_k = ui4_mid_pt - ui4_i;
        ui1_temp = aui1_result[ui4_k];
        aui1_result[ui4_k] = aui1_result[z_data - ui4_k - 1];
        aui1_result[z_data - ui4_k - 1] = ui1_temp;
    }
    
    ui4_shift     = (ui4_seed % 3) + 3;
    ui1_mask_high = ((((UINT8) 1) << ui4_shift) - 1) << (8 - ui4_shift);
    ui1_bak_high  = aui1_result[z_data - 1] & ui1_mask_high;
    for (ui4_k = 1; ui4_k < z_data; ui4_k ++)
    {
        ui4_i = ((UINT32) z_data) - ui4_k;
        
        aui1_result[ui4_i] = ((aui1_result[ui4_i] << ui4_shift) |
                              ((aui1_result[ui4_i - 1] & ui1_mask_high) >> (8 - ui4_shift)));
    }
    
    aui1_result[0] = (aui1_result[0] << ui4_shift) | 
                      (ui1_bak_high >> (8 - ui4_shift));
}
            
/*-----------------------------------------------------------------------------
                    functions (RFI)
 ----------------------------------------------------------------------------*/
static INT32 _rec_util_meta_rfi_check_buf (
    RU_RFI_CTX_T*           pt_ctx,
    SIZE_T                  z_new_sz)
{
    INT32   i4_ret;
    
    i4_ret = _rec_util_meta_tool_expand_buf (pt_ctx->pv_buf_wk,
                                             pt_ctx->z_buf_wk,
                                             z_new_sz,
                                             & pt_ctx->pv_buf_wk,
                                             & pt_ctx->z_buf_wk);
    if (i4_ret != REC_UTILR_OK)
    {
        return i4_ret;
    }
    
    i4_ret = _rec_util_meta_tool_expand_buf (pt_ctx->pv_buf_wk2,
                                             pt_ctx->z_buf_wk2,
                                             z_new_sz,
                                             & pt_ctx->pv_buf_wk2,
                                             & pt_ctx->z_buf_wk2);
    if (i4_ret != REC_UTILR_OK)
    {
        pt_ctx->z_buf_wk = pt_ctx->z_buf_wk2;
        return i4_ret;
    }
    
    return REC_UTILR_OK;
}      

static VOID _rec_util_meta_rfi_calc_file_sz (  
    RU_RFI_CTX_T*           pt_ctx,
    SIZE_T*                 pz_data_sz,
    SIZE_T*                 pz_file_sz)
{
    SIZE_T              z_data_sz;
    RU_META_RFI_NODE_T* pt_node;
    
    z_data_sz = 0;
    
    DLIST_FOR_EACH (pt_node, &pt_ctx->t_lst_info, t_link)
    {
        z_data_sz += (sizeof (RU_META_RFI_ENTRY_T) + 
                      pt_node->z_data_sz);
    }
    
    *pz_data_sz = z_data_sz;
    *pz_file_sz = sizeof (RU_META_COMMON_HDR_T) + z_data_sz;
}

static INT32 _rec_util_meta_rfi_check_crc32 (
    RU_RFI_CTX_T*           pt_ctx,
    HANDLE_T                h_file,
    UINT64                  ui8_file_sz,
    BOOL*                   pb_passed)
{
    RU_META_COMMON_HDR_T*   pt_comm_hdr_scrm;
    UINT32                  ui4_crc = 0xFFFFFFFF;
    SIZE_T                  z_chk_buf, z_req_chk, z_req_read;
    UINT32                  ui4_read, ui4_file_crc, ui4_result;
    UINT64                  ui8_pos_new, ui8_pos_cur, ui8_pos_next;
    INT32                   i4_ret;
    
    *pb_passed = FALSE;
    
    if (ui8_file_sz == 0)
    {
        return REC_UTILR_INV_ARG;
    }
 
    /* check buffer size */
    z_chk_buf = MAX (sizeof (RU_META_COMMON_HDR_T), RU_META_RFI_CRC_CHK_BUF_SIZE);
    _rec_util_meta_rfi_check_buf (pt_ctx, 
                                  z_chk_buf);
    
    /* seek to entry area */
    ui8_pos_new = (UINT64) sizeof (RU_META_COMMON_HDR_T);
    i4_ret = x_fm_lseek (h_file, 
                         (INT64) ui8_pos_new,
                         FM_SEEK_BGN,
                         & ui8_pos_cur );
    if (i4_ret != FMR_OK ||
        ui8_pos_cur != ui8_pos_new)
    {
        REC_UTIL_DBG_ERR (("RFI file seek error (%d). (L%d)\n", i4_ret, __LINE__));
        return REC_UTILR_EXTNL_MODULE_FAIL;
    }
    
    /* read & calc CRC of entry area */
    ui8_pos_next = ui8_pos_cur;
    while (ui8_pos_next < ui8_file_sz)
    {
        z_req_chk = MIN (pt_ctx->z_buf_wk, 
                         (SIZE_T) (ui8_file_sz - ui8_pos_next));
        
        i4_ret = x_fm_read (h_file,
                            pt_ctx->pv_buf_wk,
                            (UINT32) z_req_chk,
                            & ui4_read );
        if (i4_ret != FMR_OK ||
            ((SIZE_T) ui4_read) != z_req_chk)
        {
            REC_UTIL_DBG_ERR (("Fail to read entry (%d). (L%d)\n", i4_ret, __LINE__));
            return REC_UTILR_EXTNL_MODULE_FAIL;
        }

        ui4_crc = x_crc32_gen ((UINT8 *) pt_ctx->pv_buf_wk,
                               z_req_chk,
                               ui4_crc );
        
        ui8_pos_next += (UINT64) z_req_chk;
    } 
    
    /* seek to header scramble area */
    ui8_pos_new = 0;
    i4_ret = x_fm_lseek (h_file, 
                         (INT64) ui8_pos_new,
                         FM_SEEK_BGN,
                         & ui8_pos_cur );
    if (i4_ret != FMR_OK ||
        ui8_pos_cur != ui8_pos_new)
    {
        REC_UTIL_DBG_ERR (("RFI file seek error (%d). (L%d)\n", i4_ret, __LINE__));
        return REC_UTILR_EXTNL_MODULE_FAIL;
    }
    
    /* read header area */
    z_req_read = sizeof (RU_META_COMMON_HDR_T);
    i4_ret = x_fm_read (h_file,
                        pt_ctx->pv_buf_wk,
                        (UINT32) z_req_read,
                        & ui4_read );
    if (i4_ret != FMR_OK ||
        ((SIZE_T) ui4_read) != z_req_read)
    {
        REC_UTIL_DBG_ERR (("Fail to read entry (%d). (L%d)\n", i4_ret, __LINE__));
        return REC_UTILR_EXTNL_MODULE_FAIL;
    }
    pt_comm_hdr_scrm = (RU_META_COMMON_HDR_T *) pt_ctx->pv_buf_wk;
    
    /* calc CRC of header scramble area */
    /* exclude the ui4_checksum field */
    z_req_chk = ((UINT32) (&pt_comm_hdr_scrm->ui4_checksum)) - ((UINT32) pt_comm_hdr_scrm);
    ui4_crc = x_crc32_gen ((UINT8 *) pt_ctx->pv_buf_wk,
                           z_req_chk,
                           ui4_crc );
    
    ui4_file_crc = pt_comm_hdr_scrm->ui4_checksum;
    ui4_result = x_crc32_gen ( (UINT8 *) &ui4_file_crc,
                               sizeof (UINT32),
                               ui4_crc );

    *pb_passed = (ui4_result == 0) ? TRUE : FALSE;
    
    return REC_UTILR_OK;
}    
     
INT32 rec_util_meta_rfi_init (
    RU_RFI_CTX_T*           pt_ctx)
{
    pt_ctx->b_loaded    = FALSE;
    pt_ctx->s_head_file = NULL;
    pt_ctx->s_rfi_file  = NULL;
    pt_ctx->b_dirty     = FALSE;
    pt_ctx->pv_buf_wk   = NULL;
    pt_ctx->z_buf_wk    = 0;
    pt_ctx->pv_buf_wk2  = NULL;
    pt_ctx->z_buf_wk2   = 0;
    DLIST_INIT (& pt_ctx->t_lst_info);
    DLIST_INIT (& pt_ctx->t_lst_free);
    
    return REC_UTILR_OK;
}
        
INT32 rec_util_meta_rfi_deinit (
    RU_RFI_CTX_T*           pt_ctx)
{
    RU_META_RFI_NODE_T*  pt_node;
    
    while (!DLIST_IS_EMPTY (&pt_ctx->t_lst_free))
    {
        pt_node = DLIST_HEAD (&pt_ctx->t_lst_free);
        
        DLIST_REMOVE (pt_node, &pt_ctx->t_lst_free, t_link);
        
        if (pt_node->pv_buf != NULL &&
            pt_node->pv_buf != RU_META_RFI_NODE_GET_DATA (pt_node))
        {
            x_mem_free (pt_node->pv_buf);
        }
        
        x_mem_free (pt_node);
    }
    
    while (!DLIST_IS_EMPTY (&pt_ctx->t_lst_info))
    {
        pt_node = DLIST_HEAD (&pt_ctx->t_lst_info);
        
        DLIST_REMOVE (pt_node, &pt_ctx->t_lst_info, t_link);
        
        if (pt_node->pv_buf != NULL &&
            pt_node->pv_buf != RU_META_RFI_NODE_GET_DATA (pt_node))
        {
            x_mem_free (pt_node->pv_buf);
        }
        
        x_mem_free (pt_node);
    }
    
    if (pt_ctx->s_head_file)
    {
        x_mem_free (pt_ctx->s_head_file);
        pt_ctx->s_head_file = NULL;
    }
    
    if (pt_ctx->s_rfi_file)
    {
        x_mem_free (pt_ctx->s_rfi_file);
        pt_ctx->s_rfi_file = NULL;
    }
    
    if (pt_ctx->pv_buf_wk)
    {
        x_mem_free (pt_ctx->pv_buf_wk);
        pt_ctx->pv_buf_wk = NULL;
        pt_ctx->z_buf_wk  = 0;
    }
    
    if (pt_ctx->pv_buf_wk2)
    {
        x_mem_free (pt_ctx->pv_buf_wk2);
        pt_ctx->pv_buf_wk2 = NULL;
        pt_ctx->z_buf_wk2  = 0;
    }
    
    pt_ctx->b_loaded = FALSE;
    pt_ctx->b_dirty  = FALSE;
    
    return REC_UTILR_OK;
}

INT32 rec_util_meta_rfi_load (
    RU_RFI_CTX_T*           pt_ctx,
    const CHAR*             ps_head_file)
{
    HANDLE_T                h_file;
    FM_FILE_INFO_T          t_file_info;
    RU_META_COMMON_HDR_T    t_comm_hdr = {0};
    RU_META_RFI_ENTRY_T     t_entry;
    SIZE_T                  z_head_file, z_rfi_buf;
    CHAR*                   s_head_file;
    CHAR*                   s_rfi_file;
    SIZE_T                  z_hdr_sz, z_ehdr_sz, z_hdr_scrm_area;
    UINT32                  ui4_read, ui4_scrm_seed;
    BOOL                    b_opened, b_passed, b_inserted, b_file_err, b_pvr_file;
    UINT64                  ui8_entry_area, ui8_pos_entry, ui8_pos_cur;
    SIZE_T                  z_node_sz, z_node_buf_sz;
    RU_META_RFI_NODE_T*     pt_node = NULL;
    INT32                   i4_ret_fct, i4_ret;

    /* check parameter */
    z_head_file = x_strlen (ps_head_file);
    if (ps_head_file == NULL ||
        z_head_file == 0)
    {
        REC_UTIL_DBG_ERR (("Invalid arguments. (L%d)\n", __LINE__));
        return REC_UTILR_INV_ARG;
    }
    
    i4_ret = rec_util_check_pvr_file (ps_head_file, 
                                      & b_pvr_file);
    if (i4_ret != REC_UTILR_OK ||
        !b_pvr_file)
    {
        REC_UTIL_DBG_ERR (("Invalid arguments. (L%d)\n", __LINE__));
        return REC_UTILR_INV_ARG;
    }
    
    if (pt_ctx->b_loaded)
    {
        if (!x_strcasecmp (ps_head_file, pt_ctx->s_head_file))
        {
            return REC_UTILR_OK;
        }
        else
        {
            REC_UTIL_DBG_ERR (("Cannot load by other file. (L%d)\n", __LINE__));
            return REC_UTILR_OP_NOT_ALLOWED;
        }
    }
    
    h_file      = NULL_HANDLE;
    s_head_file = NULL;
    s_rfi_file  = NULL;
    b_opened    = FALSE;
    b_file_err  = FALSE;
    i4_ret_fct  = REC_UTILR_OK;
    do 
    {
        s_head_file = (CHAR *) x_mem_alloc (z_head_file + 1);
        if (s_head_file == NULL)
        {
            REC_UTIL_DBG_ERR (("Out of memory. (L%d)\n", __LINE__));
            i4_ret_fct = REC_UTILR_OUT_OF_MEMORY;
            break;
        }
        
        z_rfi_buf = z_head_file + RU_META_FILE_EXT_LEN_RFI - RU_META_FILE_EXT_LEN_PVR + 1;
        s_rfi_file = (CHAR *) x_mem_alloc (z_rfi_buf);
        if (s_rfi_file == NULL)
        {
            REC_UTIL_DBG_ERR (("Out of memory. (L%d)\n", __LINE__));
            i4_ret_fct = REC_UTILR_OUT_OF_MEMORY;
            break;
        }
        
        i4_ret = _rec_util_meta_tool_get_ext_file (ps_head_file,
                                                   z_head_file,
                                                   RU_META_FILE_TYPE_RFI,
                                                   s_rfi_file,
                                                   z_rfi_buf);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR (("Fail to get ext file (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret_fct = i4_ret;
            break;
        }
        
        /* check if file exist */
        i4_ret = x_fm_get_info_by_name (FM_ROOT_HANDLE,
                                        s_rfi_file,
                                        & t_file_info);
        if (i4_ret != FMR_OK)
        {
            if (i4_ret == FMR_NO_ENTRY)
            {
                REC_UTIL_DBG_INFO (("RFI file not exist yet. May create a new one when store. (L%d)\n", __LINE__));
                b_file_err = TRUE;
                i4_ret_fct = REC_UTILR_OK;
                break;
            }
            else
            {
                REC_UTIL_DBG_ERR (("Fail to get file info (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret_fct = REC_UTILR_EXTNL_MODULE_FAIL;
                break;
            }
        }

        /* check file size */
        if (t_file_info.ui8_size < sizeof (RU_META_COMMON_HDR_T))
        {
            REC_UTIL_DBG_ERR (("Invalid RFI file. (L%d)\n", __LINE__));
            b_file_err = TRUE;
            i4_ret_fct = REC_UTILR_OK;
            break;
        }
        ui4_scrm_seed = (UINT32) t_file_info.ui8_size;
        
        /* open RFI, does not create a new one if not exist, creation only be done in store operation */
        i4_ret = x_fm_open (FM_ROOT_HANDLE,
                            s_rfi_file,
                            FM_READ_WRITE,
                            FM_MODE_USR_READ | FM_MODE_USR_WRITE,
                            FALSE,
                            & h_file );
        if (i4_ret != FMR_OK)
        {
            REC_UTIL_DBG_ERR (("Fail to open file (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret_fct = REC_UTILR_EXTNL_MODULE_FAIL;
            break;
        }
        b_opened = TRUE;
        
        i4_ret = _rec_util_meta_rfi_check_crc32 (pt_ctx,
                                                 h_file,
                                                 t_file_info.ui8_size,
                                                 & b_passed);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR (("Fail to check CRC32 (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret_fct = i4_ret;
            break;
        }
        
        if (!b_passed)
        {
            REC_UTIL_DBG_ERR (("CRC error. (L%d)\n", __LINE__));
            b_file_err = TRUE;
            i4_ret_fct = REC_UTILR_OK;
            break;
        }
        
        z_hdr_sz = sizeof (RU_META_COMMON_HDR_T);
        i4_ret = _rec_util_meta_rfi_check_buf (pt_ctx,
                                               z_hdr_sz);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR (("Out of memory. (L%d)\n", __LINE__));
            i4_ret_fct = REC_UTILR_OUT_OF_MEMORY;
            break;
        }
        
        i4_ret = x_fm_lseek (h_file, 
                             (INT64) 0,
                             FM_SEEK_BGN,
                             & ui8_pos_cur);
        if (i4_ret != FMR_OK ||
            ui8_pos_cur != 0)
        {
            REC_UTIL_DBG_ERR (("RFI file seek error (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret_fct = REC_UTILR_EXTNL_MODULE_FAIL;
            break;
        }
        
        i4_ret = x_fm_read (h_file,
                            pt_ctx->pv_buf_wk,
                            (UINT32) z_hdr_sz,
                            & ui4_read);
        if (i4_ret != FMR_OK ||
            ui4_read != (UINT32) z_hdr_sz)
        {
            REC_UTIL_DBG_ERR (("Fail to read header (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret_fct = REC_UTILR_EXTNL_MODULE_FAIL;
            break;
        }
        
        x_memset (&t_comm_hdr, 0, z_hdr_sz);
        z_hdr_scrm_area = (SIZE_T) (((UINT32) (&t_comm_hdr.ui4_checksum)) - ((UINT32) (&t_comm_hdr)));
        
        _rec_util_meta_tool_descramble (ui4_scrm_seed,
                                        pt_ctx->pv_buf_wk,
                                        z_hdr_scrm_area,
                                        pt_ctx->pv_buf_wk2);
        /* rebuild common header */
        x_memcpy (& t_comm_hdr, 
                  pt_ctx->pv_buf_wk2, 
                  z_hdr_scrm_area);
        x_memcpy ( (VOID *) (((UINT32) (&t_comm_hdr)) + ((UINT32) z_hdr_scrm_area)), 
                   (VOID *) (((UINT32) pt_ctx->pv_buf_wk) + ((UINT32) z_hdr_scrm_area)),
                   z_hdr_sz - z_hdr_scrm_area );
        
        if (t_comm_hdr.ui4_strct_ver != 1                              ||
            t_comm_hdr.e_file_type != RU_META_FILE_TYPE_RFI            ||
            t_comm_hdr.ui4_schema_ver != 1                             ||
            t_comm_hdr.ui8_data_offset < sizeof (RU_META_COMMON_HDR_T) ||
            (t_comm_hdr.ui8_data_offset + t_comm_hdr.ui8_data_sz) != t_file_info.ui8_size )
        {
            REC_UTIL_DBG_ERR (("RFI file header error. (L%d)\n", __LINE__));
            b_file_err = TRUE;
            i4_ret_fct = REC_UTILR_OK;
            break;
        }
        
        /* check buffer size for entry header */
        z_ehdr_sz = sizeof (RU_META_RFI_ENTRY_T);
        i4_ret = _rec_util_meta_rfi_check_buf (pt_ctx,
                                               z_ehdr_sz);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR (("Out of memory. (L%d)\n", __LINE__));
            i4_ret_fct = REC_UTILR_OUT_OF_MEMORY;
            break;
        }

        pt_node        = NULL;
        b_inserted     = FALSE;
        ui8_entry_area = 0;
        while (ui8_entry_area < t_comm_hdr.ui8_data_sz)
        {
            /* init variables */
            pt_node = NULL;
            b_inserted = FALSE;
            
            /* seek to begin of entry */
            ui8_pos_entry = t_comm_hdr.ui8_data_offset + ui8_entry_area;
            i4_ret = x_fm_lseek (h_file, 
                                 (INT64) ui8_pos_entry,
                                 FM_SEEK_BGN,
                                 & ui8_pos_cur);
            if (i4_ret != FMR_OK ||
                ui8_pos_cur != ui8_pos_entry)
            {
                REC_UTIL_DBG_ERR (("RFI file seek error (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret_fct = REC_UTILR_EXTNL_MODULE_FAIL;
                break;
            }
            
            /* read entry header */
            i4_ret = x_fm_read (h_file,
                                pt_ctx->pv_buf_wk,
                                (UINT32) z_ehdr_sz,
                                & ui4_read);
            if (i4_ret != FMR_OK ||
                ui4_read != (UINT32) z_ehdr_sz)
            {
                REC_UTIL_DBG_ERR (("Fail to read entry header (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret_fct = REC_UTILR_EXTNL_MODULE_FAIL;
                break;
            }
            
            /* descramble entry header */
            _rec_util_meta_tool_descramble (ui4_scrm_seed,
                                            pt_ctx->pv_buf_wk,
                                            z_ehdr_sz,
                                            pt_ctx->pv_buf_wk2);
            t_entry = *((RU_META_RFI_ENTRY_T *) pt_ctx->pv_buf_wk2);
            
            /* check if entry header correct or not */
            if (t_entry.ui4_key_id == 0 ||
                (t_entry.ui4_data_sz + sizeof (RU_META_RFI_ENTRY_T)) != t_entry.ui4_entry_sz)
            {
                REC_UTIL_DBG_ERR (("RFI entry header error. (L%d)\n", __LINE__));
                b_file_err = TRUE;
                i4_ret_fct = REC_UTILR_OK;
                break;
            }
            
            /* allocate an entry node with sufficient buffer for entry data */
            z_node_buf_sz = (SIZE_T) (t_entry.ui4_data_sz + RU_META_RFI_NODE_BUF_INIT_PADDING);
            z_node_sz     = RU_META_RFI_NODE_CALC_SIZE (z_node_buf_sz);
            pt_node = (RU_META_RFI_NODE_T *) x_mem_alloc (z_node_sz);
            if (pt_node == NULL)
            {
                REC_UTIL_DBG_ERR (("Out of memory. (L%d)\n", __LINE__));
                i4_ret_fct = REC_UTILR_OUT_OF_MEMORY;
                break;
            }
            
            /* check buffer size before read entry data */
            i4_ret = _rec_util_meta_rfi_check_buf (pt_ctx,
                                                   (SIZE_T) t_entry.ui4_data_sz);
            if (i4_ret != REC_UTILR_OK)
            {
                REC_UTIL_DBG_ERR (("Out of memory. (L%d)\n", __LINE__));
                i4_ret_fct = REC_UTILR_OUT_OF_MEMORY;
                break;
            }
            
            /* read entry data */
            i4_ret = x_fm_read (h_file,
                                pt_ctx->pv_buf_wk,
                                t_entry.ui4_data_sz,
                                & ui4_read);
            if (i4_ret != FMR_OK ||
                ui4_read != t_entry.ui4_data_sz)
            {
                REC_UTIL_DBG_ERR (("Fail to read entry header (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret_fct = REC_UTILR_EXTNL_MODULE_FAIL;
                break;
            }
            
            /* descramble entry data */
            _rec_util_meta_tool_descramble (ui4_scrm_seed,
                                            pt_ctx->pv_buf_wk,
                                            (SIZE_T) t_entry.ui4_data_sz,
                                            pt_ctx->pv_buf_wk2);
            
            /* init node here */
            pt_node->z_buf_sz   = z_node_buf_sz;
            pt_node->pv_buf     = RU_META_RFI_NODE_GET_DATA (pt_node);
            pt_node->ui4_key_id = t_entry.ui4_key_id;
            pt_node->z_data_sz  = (SIZE_T) t_entry.ui4_data_sz;

            /* store entry data */
            x_memcpy ( pt_node->pv_buf,
                       pt_ctx->pv_buf_wk2,
                       (SIZE_T) t_entry.ui4_data_sz );
            
            DLIST_INSERT_TAIL (pt_node, & pt_ctx->t_lst_info, t_link);
            b_inserted = TRUE;
            
            ui8_entry_area += ((UINT64) t_entry.ui4_entry_sz);
        } 
        
        if (i4_ret_fct != REC_UTILR_OK || 
            b_file_err)
        {
            if (pt_node != NULL &&
                !b_inserted)
            {
                x_mem_free (pt_node);
            }
            
            break;
        }
        
        /* consistency checking */
        if (ui8_entry_area != t_comm_hdr.ui8_data_sz)
        {
            REC_UTIL_DBG_ERR (("Internal error. (L%d)\n", __LINE__));
        }
        
    } while (FALSE);

    if (b_opened)
    {
        i4_ret = x_fm_close (h_file);
        if (i4_ret != FMR_OK)
        {
            REC_UTIL_DBG_ERR (("Fail to close file (%d). (L%d)\n", i4_ret, __LINE__));
        }
        h_file = NULL_HANDLE;
        b_opened = FALSE;
    }
    
    if (i4_ret_fct == REC_UTILR_OK)
    {
        pt_ctx->b_loaded    = TRUE;
        pt_ctx->s_head_file = s_head_file;
        x_strcpy (pt_ctx->s_head_file, ps_head_file);
        pt_ctx->s_rfi_file  = s_rfi_file;
    }
    else
    {
        if (s_head_file != NULL)
        {
            x_mem_free (s_head_file);
        }
        if (s_rfi_file != NULL)
        {
            x_mem_free (s_rfi_file);
        }
    }
    
    return i4_ret_fct;
}

INT32 rec_util_meta_rfi_store (
    RU_RFI_CTX_T*           pt_ctx)
{
    HANDLE_T                h_file;
    RU_META_COMMON_HDR_T    t_comm_hdr;
    RU_META_RFI_NODE_T*     pt_node;
    SIZE_T                  z_data_sz, z_file_sz;
    UINT32                  ui4_scrm_seed;
    BOOL                    b_opened;
    UINT64                  ui8_entry_area, ui8_pos_entry, ui8_pos_cur;
    SIZE_T                  z_hdr_sz, z_ehdr_sz, z_hdr_scrm_area;
    UINT32                  ui4_written;
    RU_META_RFI_ENTRY_T     t_entry;
    VOID*                   pv_entry_data;
    UINT32                  ui4_crc = 0xFFFFFFFF;
    RU_META_COMMON_HDR_T*   pt_comm_hdr_scrm;
    INT32                   i4_ret, i4_ret_fct;
    
    /* return fail if not loaded yet */
    if (!pt_ctx->b_loaded)
    {
        REC_UTIL_DBG_ERR (("Not loaded yet. (L%d)\n", __LINE__));
        return REC_UTILR_OP_NOT_ALLOWED;
    }
    
    /* quick return if not dirty */
    if (!pt_ctx->b_dirty)
    {
        return REC_UTILR_OK;
    }
    
    b_opened   = FALSE;
    h_file     = NULL_HANDLE;
    i4_ret_fct = REC_UTILR_OK;
    do
    {
        /* estimate total file size to generate scramble seed */
        _rec_util_meta_rfi_calc_file_sz (pt_ctx,
                                         & z_data_sz,
                                         & z_file_sz);
        ui4_scrm_seed = (UINT32) z_file_sz;
        
        /* init common header info */
        x_memset (&t_comm_hdr, 0, sizeof (RU_META_COMMON_HDR_T));
        t_comm_hdr.ui4_strct_ver   = 1;
        t_comm_hdr.e_file_type     = RU_META_FILE_TYPE_RFI;
        t_comm_hdr.ui4_schema_ver  = 1;
        t_comm_hdr.ui8_data_offset = sizeof (RU_META_COMMON_HDR_T);
        t_comm_hdr.ui8_data_sz     = (UINT64) z_data_sz;
        
        /* delete rfi file if exist */
        i4_ret = x_fm_delete_file (FM_ROOT_HANDLE,
                                   pt_ctx->s_rfi_file);
        if (!(i4_ret == FMR_OK ||
              i4_ret == FMR_NO_ENTRY))
        {
            REC_UTIL_DBG_ERR (("Fail to delete file (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret_fct = REC_UTILR_EXTNL_MODULE_FAIL;
            break;
        }
        
        /* open file */
        i4_ret = x_fm_open (FM_ROOT_HANDLE,
                            pt_ctx->s_rfi_file,
                            FM_READ_WRITE | FM_OPEN_CREATE,
                            FM_MODE_USR_READ | FM_MODE_USR_WRITE,
                            FALSE,
                            & h_file );
        if (i4_ret != FMR_OK)
        {
            REC_UTIL_DBG_ERR (("Fail to open file (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret_fct = REC_UTILR_EXTNL_MODULE_FAIL;
            break;
        }
        b_opened = TRUE;
        
        /* extent file */
        i4_ret = x_fm_extend_file (h_file,
                                   (INT64) z_file_sz);
        if (i4_ret != FMR_OK)
        {
            REC_UTIL_DBG_ERR (("Fail to extend file (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret_fct = REC_UTILR_EXTNL_MODULE_FAIL;
            break;
        }
        
        /* check buffer size for entry header */
        z_ehdr_sz = sizeof (RU_META_RFI_ENTRY_T);
        i4_ret = _rec_util_meta_rfi_check_buf (pt_ctx,
                                               z_ehdr_sz);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR (("Out of memory. (L%d)\n", __LINE__));
            i4_ret_fct = REC_UTILR_OUT_OF_MEMORY;
            break;
        }
        
        /* write entries */
        ui8_entry_area = 0;
        DLIST_FOR_EACH (pt_node, &pt_ctx->t_lst_info, t_link)
        {
            /* seek to the begin of the entry */
            ui8_pos_entry = t_comm_hdr.ui8_data_offset + ui8_entry_area;
            i4_ret = x_fm_lseek (h_file, 
                                 (INT64) ui8_pos_entry,
                                 FM_SEEK_BGN,
                                 & ui8_pos_cur);
            if (i4_ret != FMR_OK ||
                ui8_pos_cur != ui8_pos_entry)
            {
                REC_UTIL_DBG_ERR (("RFI file seek error (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret_fct = REC_UTILR_EXTNL_MODULE_FAIL;
                break;
            }
            
            /* init entry header */
            x_memset (&t_entry, 0, z_ehdr_sz);
            t_entry.ui4_entry_sz = (UINT32) (z_ehdr_sz + pt_node->z_data_sz);
            t_entry.ui4_key_id   = pt_node->ui4_key_id;
            t_entry.ui4_data_sz  = (UINT32) pt_node->z_data_sz;
            
            /* scramble entry header */
            _rec_util_meta_tool_scramble (ui4_scrm_seed, 
                                          (VOID *) &t_entry,
                                          z_ehdr_sz,
                                          pt_ctx->pv_buf_wk);
            
            /* calculate CRC */
            ui4_crc = x_crc32_gen ( (UINT8 *) pt_ctx->pv_buf_wk,
                                    z_ehdr_sz,
                                    ui4_crc);
            
            /* write entry header */
            i4_ret = x_fm_write (h_file, 
                                 pt_ctx->pv_buf_wk,
                                 (UINT32) z_ehdr_sz,
                                 & ui4_written );
            if (i4_ret != FMR_OK ||
                ui4_written != (UINT32) z_ehdr_sz)
            {
                REC_UTIL_DBG_ERR (("Fail to write entry header (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret_fct = REC_UTILR_EXTNL_MODULE_FAIL;
                break;
            }
            
            /* check buffer size for entry data */
            i4_ret = _rec_util_meta_rfi_check_buf (pt_ctx,
                                                   pt_node->z_data_sz);
            if (i4_ret != REC_UTILR_OK)
            {
                REC_UTIL_DBG_ERR (("Out of memory. (L%d)\n", __LINE__));
                i4_ret_fct = REC_UTILR_OUT_OF_MEMORY;
                break;
            }
            
            /* scramble entry data */
            pv_entry_data = RU_META_RFI_NODE_GET_DATA (pt_node);
            _rec_util_meta_tool_scramble (ui4_scrm_seed,
                                          pv_entry_data,
                                          pt_node->z_data_sz,
                                          pt_ctx->pv_buf_wk );
            
            /* calculate CRC */
            ui4_crc = x_crc32_gen ( (UINT8 *) pt_ctx->pv_buf_wk,
                                    pt_node->z_data_sz,
                                    ui4_crc);
            
            /* write entry data */
            i4_ret = x_fm_write (h_file, 
                                 pt_ctx->pv_buf_wk,
                                 pt_node->z_data_sz,
                                 & ui4_written );
            if (i4_ret != FMR_OK ||
                ui4_written != pt_node->z_data_sz)
            {
                REC_UTIL_DBG_ERR (("Fail to write entry data (%d). (L%d)\n", i4_ret, __LINE__));
                i4_ret_fct = REC_UTILR_EXTNL_MODULE_FAIL;
                break;
            }
            
            ui8_entry_area += ((UINT64) t_entry.ui4_entry_sz);
        }
            
        if (i4_ret_fct != REC_UTILR_OK)
        {
            break;
        }
        
        /* check buffer size for common header */
        z_hdr_sz = sizeof (RU_META_COMMON_HDR_T);
        i4_ret = _rec_util_meta_rfi_check_buf (pt_ctx,
                                               z_hdr_sz);
        if (i4_ret != REC_UTILR_OK)
        {
            REC_UTIL_DBG_ERR (("Out of memory. (L%d)\n", __LINE__));
            i4_ret_fct = REC_UTILR_OUT_OF_MEMORY;
            break;
        }
        x_memset (pt_ctx->pv_buf_wk, 0, z_hdr_sz);
        
        /* scramble common header (exclude checksum) */
        /* exclude the ui4_checksum field */
        z_hdr_scrm_area = (SIZE_T) (((UINT32) (&t_comm_hdr.ui4_checksum)) - ((UINT32) (&t_comm_hdr)));
        _rec_util_meta_tool_scramble (ui4_scrm_seed,
                                      (VOID *) & t_comm_hdr,
                                      z_hdr_scrm_area, 
                                      pt_ctx->pv_buf_wk);

        /* calculate CRC */
        ui4_crc = x_crc32_gen ( (UINT8 *) pt_ctx->pv_buf_wk,
                                z_hdr_scrm_area,
                                ui4_crc);
        
        /* calculate & keep result CRC */
        pt_comm_hdr_scrm = (RU_META_COMMON_HDR_T *) pt_ctx->pv_buf_wk;
        PUT_UINT32_TO_PTR_BIG_END (ui4_crc, &pt_comm_hdr_scrm->ui4_checksum);
        
        /* seek to the begin of the file */
        i4_ret = x_fm_lseek (h_file, 
                             (INT64) 0,
                             FM_SEEK_BGN,
                             & ui8_pos_cur);
        if (i4_ret != FMR_OK ||
            ui8_pos_cur != 0)
        {
            REC_UTIL_DBG_ERR (("RFI file seek error (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret_fct = REC_UTILR_EXTNL_MODULE_FAIL;
            break;
        }
        
        /* write common header */
        i4_ret = x_fm_write (h_file, 
                             (VOID *) pt_comm_hdr_scrm,
                             z_hdr_sz,
                             & ui4_written );
        if (i4_ret != FMR_OK ||
            ui4_written != z_hdr_sz)
        {
            REC_UTIL_DBG_ERR (("Fail to write common header (%d). (L%d)\n", i4_ret, __LINE__));
            i4_ret_fct = REC_UTILR_EXTNL_MODULE_FAIL;
            break;
        }
        
    } while (FALSE);

    /* close file */
    if (b_opened)
    {
        i4_ret = x_fm_close (h_file);
        if (i4_ret != FMR_OK)
        {
            REC_UTIL_DBG_ERR (("Fail to close file (%d). (L%d)\n", i4_ret, __LINE__));
        }
        h_file = NULL_HANDLE;
        b_opened = FALSE;
    }
    
    if (i4_ret_fct == REC_UTILR_OK)
    {
        pt_ctx->b_dirty = FALSE;        
    }
    else
    {
        /* free resource here */
        /* do nothing */
    }
    
    /* consistency check between estimated file size & final file size */
    #ifdef DEBUG
    {
        FM_FILE_INFO_T  t_file_info;
        
        i4_ret = x_fm_get_info_by_name (FM_ROOT_HANDLE,
                                        pt_ctx->s_rfi_file,
                                        & t_file_info);
        if (i4_ret == FMR_OK)
        {
            if (t_file_info.ui8_size != (UINT64) z_file_sz)
            {
                REC_UTIL_DBG_ERR (("Internal error. (L%d)\n", __LINE__));
            }
        }
        else
        {
            REC_UTIL_DBG_ERR (("Fail to get written file info (%d). (L%d)\n", i4_ret, __LINE__));
        }
    }
    #endif
    
    return i4_ret_fct;
}

INT32 rec_util_meta_rfi_set_info (
    RU_RFI_CTX_T*           pt_ctx,
    UINT32                  ui4_key_id,
    const VOID*             pv_set_info,
    SIZE_T                  z_set_info )
{
    RU_META_RFI_NODE_T*  pt_node;
    BOOL                 b_found;
    
    /* check parameter */
    if ( ui4_key_id == 0  ||
        !(z_set_info == 0 ||
          pv_set_info != NULL))
    {
        REC_UTIL_DBG_ERR (("Invalid arguments. (L%d)\n", __LINE__));
        return REC_UTILR_INV_ARG;
    }

    /* return fail if not loaded yet */
    if (!pt_ctx->b_loaded)
    {
        REC_UTIL_DBG_ERR (("Not loaded yet. (L%d)\n", __LINE__));
        return REC_UTILR_OP_NOT_ALLOWED;
    }
    
    /* search if already exsit node */
    b_found = FALSE;
    DLIST_FOR_EACH (pt_node, &pt_ctx->t_lst_info, t_link)
    {
        if (pt_node->ui4_key_id == ui4_key_id)
        {
            b_found = TRUE;
            break;
        }
    }
    
    if (z_set_info != 0)
    {
        /* add this entry */
        if (!b_found)
        {
            RU_META_RFI_NODE_T* pt_node_iter;
            
            /* find the minimal node with enough buffer size in the free list */
            pt_node = NULL;
            DLIST_FOR_EACH (pt_node_iter, &pt_ctx->t_lst_free, t_link)
            {
                if (pt_node_iter->z_buf_sz >= z_set_info)
                {
                    if (pt_node)
                    {
                        if (pt_node_iter->z_buf_sz < pt_node->z_buf_sz)
                        {
                            pt_node = pt_node_iter;
                        }
                    }
                    else
                    {
                        pt_node = pt_node_iter;
                    }
                }
            }
            
            if (pt_node)
            {
                DLIST_REMOVE (pt_node, &pt_ctx->t_lst_free, t_link);
            }
            else
            {
                SIZE_T  z_node_sz, z_node_buf_sz;
                
                /* alloc a new node */
                z_node_buf_sz = z_set_info + RU_META_RFI_NODE_BUF_INIT_PADDING;
                z_node_sz     = RU_META_RFI_NODE_CALC_SIZE (z_node_buf_sz);
                pt_node = (RU_META_RFI_NODE_T *) x_mem_alloc (z_node_sz);
                if (pt_node == NULL)
                {
                    REC_UTIL_DBG_ERR (("Out of memory. (L%d)\n", __LINE__));
                    return REC_UTILR_OUT_OF_MEMORY;
                }
                
                pt_node->z_buf_sz   = z_node_buf_sz;
                pt_node->pv_buf     = RU_META_RFI_NODE_GET_DATA (pt_node);
                pt_node->ui4_key_id = 0;
                pt_node->z_data_sz  = 0;
            }
            
            DLIST_INSERT_TAIL (pt_node, &pt_ctx->t_lst_info, t_link);
        }
        else if (((SIZE_T) pt_node->z_buf_sz) < z_set_info)
        {
            /* check if buffer size is enough */
            SIZE_T  z_new_sz = z_set_info * 2;
            VOID*   pv_buf_new;
            
            /* realloc buffer */
            pv_buf_new = x_mem_alloc (z_new_sz);
            if (pv_buf_new == NULL)
            {
                REC_UTIL_DBG_ERR (("Out of memory. (L%d)\n", __LINE__));
                return REC_UTILR_OUT_OF_MEMORY;
            }
            
            if (pt_node->pv_buf != NULL &&
                pt_node->pv_buf != RU_META_RFI_NODE_GET_DATA (pt_node))
            {
                x_mem_free (pt_node->pv_buf);
            }
            pt_node->pv_buf   = pv_buf_new;
            pt_node->z_buf_sz = z_new_sz;
        }
    
        pt_node->ui4_key_id = ui4_key_id;
        pt_node->z_data_sz  = z_set_info;
        x_memcpy (pt_node->pv_buf, pv_set_info, z_set_info);
    }
    else
    {
        /* remove this entry */
        if (b_found)
        {
            DLIST_REMOVE (pt_node, &pt_ctx->t_lst_info, t_link);
            DLIST_INSERT_TAIL (pt_node, &pt_ctx->t_lst_free, t_link);
        }
    }
    
    pt_ctx->b_dirty = TRUE;
    
    return REC_UTILR_OK;
}
    
INT32 rec_util_meta_rfi_get_info (
    RU_RFI_CTX_T*           pt_ctx,
    UINT32                  ui4_key_id,
    VOID*                   pv_get_info,
    SIZE_T*                 pz_get_info )
{
    RU_META_RFI_NODE_T*  pt_node;
    BOOL                 b_found;
    INT32                i4_ret;
    
    /* check parameter */
    if (ui4_key_id == 0     ||
        pz_get_info == NULL ||
        *pz_get_info == 0   ||
        pv_get_info == NULL)
    {
        REC_UTIL_DBG_ERR (("Invalid arguments. (L%d)\n", __LINE__));
        return REC_UTILR_INV_ARG;
    }
    
    /* return fail if not loaded yet */
    if (!pt_ctx->b_loaded)
    {
        REC_UTIL_DBG_ERR (("Not loaded yet. (L%d)\n", __LINE__));
        return REC_UTILR_OP_NOT_ALLOWED;
    }
    
    /* search node */
    b_found = FALSE;
    DLIST_FOR_EACH (pt_node, &pt_ctx->t_lst_info, t_link)
    {
        if (pt_node->ui4_key_id == ui4_key_id)
        {
            b_found = TRUE;
            break;
        }
    }
    
    if (b_found)
    {
        if (pt_node->z_data_sz <= *pz_get_info)
        {
            x_memcpy (pv_get_info, pt_node->pv_buf, pt_node->z_data_sz);
            *pz_get_info = pt_node->z_data_sz;
            
            i4_ret = REC_UTILR_OK;
        }
        else
        {
            REC_UTIL_DBG_ERR (("Not enough buffer size. (L%d)\n", __LINE__));
            i4_ret = REC_UTILR_NOT_ENOUGH_BUF;
        }
    }
    else
    {
        REC_UTIL_DBG_ERR (("Specified key id (%d) not found. (L%d)\n", ui4_key_id, __LINE__));
        i4_ret = REC_UTILR_NOT_FOUND;
    }    
    
    return i4_ret;
}
    
INT32 rec_util_meta_rfi_get_info_len (
    RU_RFI_CTX_T*           pt_ctx,
    UINT32                  ui4_key_id,
    SIZE_T*                 pz_get_info)
{
    RU_META_RFI_NODE_T*  pt_node;
    BOOL                 b_found;
    INT32                i4_ret;
    
    /* check parameter */
    if (ui4_key_id == 0 ||
        pz_get_info == NULL)
    {
        REC_UTIL_DBG_ERR (("Invalid arguments. (L%d)\n", __LINE__));
        return REC_UTILR_INV_ARG;
    }
    
    /* return fail if not loaded yet */
    if (!pt_ctx->b_loaded)
    {
        REC_UTIL_DBG_ERR (("Not loaded yet. (L%d)\n", __LINE__));
        return REC_UTILR_OP_NOT_ALLOWED;
    }
    
    /* search node */
    b_found = FALSE;
    DLIST_FOR_EACH (pt_node, &pt_ctx->t_lst_info, t_link)
    {
        if (pt_node->ui4_key_id == ui4_key_id)
        {
            b_found = TRUE;
            break;
        }
    }
    
    if (b_found)
    {
        *pz_get_info = pt_node->z_data_sz;
        i4_ret = REC_UTILR_OK;
    }
    else
    {
        REC_UTIL_DBG_ERR (("Specified key id (%d) not found. (L%d)\n", ui4_key_id, __LINE__));
        i4_ret = REC_UTILR_NOT_FOUND;
    }
    
    return i4_ret;
}
 
 
