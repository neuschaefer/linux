/*----------------------------------------------------------------------------
 * No Warranty :  Except  as  may  be  otherwise  agreed  to  in  writing, no *
 * warranties  of  any  kind,  whether  express or  implied, are given by MTK *
 * with  respect  to  any  Confidential  Information  or any use thereof, and *
 * the  Confidential  Information  is  provided  on  an  "AS IS"  basis.  MTK *
 * hereby  expressly  disclaims  all  such  warranties, including any implied *
 * warranties   of  merchantability ,  non-infringement  and  fitness  for  a *
 * particular purpose and any warranties arising out of course of performance *
 * course  of dealing or usage of trade.  Parties further acknowledge that in *
 * connection  with  the Purpose, Company may, either presently and/or in the *
 * future,   instruct   MTK   to   assist  it  in  the  development  and  the *
 * implementation,  in accordance with Company's designs, of certain software *
 * relating  to  Company's  product(s)  (the  "Services").   Except as may be *
 * otherwise agreed to in writing, no warranties of any kind, whether express *
 * or  implied,  are  given by MTK with respect to the Services provided, and *
 * the  Services  are  provided  on  an  "AS  IS"  basis.   Company   further *
 * acknowledges  that  the  Services  may  contain  errors,  which testing is *
 * important  and  it  is  solely  responsible for fully testing the Services *
 * and/or   derivatives   thereof   before  they  are  used,  sublicensed  or *
 * distributed.   Should  there  be any third party action brought again MTK, *
 * arising  out  of  or  relating  to  the  Services,  Company agree to fully *
 * indemnify and hold MTK harmless.                                           *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2004, CrystalMedia Technology, Inc.
 * All rights reserved.
 * 
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.  
 *-----------------------------------------------------------------------------
 * $RCSfile: mp4_midxbuld_mem.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/DTV_X_ATSC/3 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 0a0e5908fac0e73885ce39efe82bcd61 $ Asa
 *
 * Description: 
 *         This file contains all the media index builder public APIs  
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#ifdef _WIN32_
#include "inc/x_os.h"
#else
#include "os/inc/x_os.h"
#endif
#include "inc/x_common.h"
#include "inc/x_mm_common.h"
#include "handle/handle.h"
#include "handle/u_handle_grp.h"
#include "mutil/midxbuld/u_midxbuld.h"
#include "mutil/midxbuld/midxbuld.h"
#include "mutil/mm_util_input.h"
#include "mutil/mm_util_memory.h"
#include "dbg/x_dbg.h"
#include "mutil/mm_util.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "util/x_lnk_list.h"
#include "mp4_midxbuld.h"

#else

#include "x_os.h"
#include "x_common.h"
#include "x_mm_common.h"
#include "handle.h"
#include "u_handle_grp.h"
#include "../u_midxbuld.h"
#include "../midxbuld.h"
#include "../../mm_util_input.h"
#include "../../mm_util_memory.h"
#include "x_dbg.h"

#include "../../mm_util.h"

#include "x_lnk_list.h"
#include "mp4_midxbuld.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

#define MP4_KEY_LEN_OF_SAMPLE_NUM   100
#define FM_ROOT_HANDLE              NULL_HANDLE   /**< Root handle used in FM       */
#define FM_READ_ONLY                ((UINT32) 0000)    /**<read only        */

#define PIFF_KEY_NUM                512
#define PIFF_INET_BUFF_SIZE         (15*1024)
#define PIFF_INET_MOOF_MAX_COUNT    20

#define ENABLE_CTTS                 1

#define dwBigFourCC(a,b,c,d)        ((((UINT32)a)<<24)|((b)<<16)|((c)<<8)|(d))
#define MP4_BOX_MOOF                dwBigFourCC('m', 'o', 'o', 'f')
#define MP4_BOX_MFHD                dwBigFourCC('m', 'f', 'h', 'd')
#define MP4_BOX_TRAF                dwBigFourCC('t', 'r', 'a', 'f')
#define MP4_BOX_TFHD                dwBigFourCC('t', 'f', 'h', 'd')
#define MP4_BOX_TRUN                dwBigFourCC('t', 'r', 'u', 'n')
#define MP4_BOX_MFRO                dwBigFourCC('m', 'f', 'r', 'o')


INT32 ui4_get_cur_pts(UINT32 ui4_ctts_value,
                      UINT32 ui4_cur_dur_sum,
                      UINT32 ui4_timescale)
{
    INT64   i8_ctts_ofst    = 0;
    INT64   i8_cur_dur_sum  = 0;
    UINT64  ui8_time        = 0;
    UINT32  ui4_curr_pts    = 0;

    i8_ctts_ofst = (INT64)((INT32)ui4_ctts_value);
    i8_cur_dur_sum = (INT64)ui4_cur_dur_sum;
    if((i8_ctts_ofst + i8_cur_dur_sum) < 0)
    {
        ui8_time = (UINT64)i8_cur_dur_sum;
    }
    else
    {
        ui8_time = (UINT64)(i8_ctts_ofst + i8_cur_dur_sum);
    }
        
    ui8_time = ui8_time * MIDXBULD_SYSTEM_CLOCK_FREQUENCY;
        
    ui4_curr_pts = (UINT32)_mm_div64(ui8_time,
                                     ui4_timescale, 
                                     NULL); 
    return ui4_curr_pts;
}
                    
/*-----------------------------------------------------------------------------
 * Name:  piff_get_key_from_mfra
 *
 * Description: 
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MIDXBULDR_OK                 The API is successful. 
 *          MIDXBULDR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/                            
INT32 piff_get_key_from_mfra(MIDXBULD_OBJ_T*  pt_midxbuld_obj, MIDXBULD_KEY_INFO_T* pt_key_info)
{
    UINT64                          ui8_time_len            = 0;
    UINT64                          ui8_ofs_len             = 0; 
    UINT32                          ui4_i                   = 0;
    UINT32                          ui4_ver                 = 0;
    UINT32                          ui4_read_size           = 0;
    UINT32                          ui4_temp                = 0;
    UINT32                          ui4_tmp                 = 0;
    UINT32                          ui4_high_word           = 0;
    UINT32                          ui4_low_word            = 0;
    UINT32                          ui4_box_size            = 0;
    UINT32                          ui4_entry_num           = 0;
    INT32                           i4_ret                  = 0;   
    UINT8*                          pui1_tfra_buf           = NULL;
    UINT8*                          pui1_pstart_buf         = NULL;

    setpos_fct                      pf_set_pos              = NULL;    
    copybytes_fct                   pf_copybytes            = NULL;     
    
    MP4_IDXBULD_OBJ_T*              pt_mp4_handler          = NULL;
    MP4_IDXBULD_KEY_EXT_TBL_T*      pt_key_ext_tbl          = NULL;

    pt_mp4_handler          = pt_midxbuld_obj->pv_handler_obj;
    pf_set_pos              = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pf_copybytes            = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    
    if((pt_mp4_handler == NULL) || (pt_mp4_handler->piff_inet_moof_info == NULL))
    {
        return MIDXBULDR_INTERNAL_ERR;
    }
    
    if( pt_mp4_handler->piff_inet_moof_info->b_is_piff_file         &&
        (!pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file   ||
            (pt_mp4_handler->t_mln_moov_info.b_is_moov_empty &&
                pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file)))
    {
        if (pt_mp4_handler->pt_key_ext_tbl != NULL)
        {
            _mm_util_mem_free_with_tag(pt_mp4_handler->pt_key_ext_tbl, MM_MEM_MIDX_TAG);
            pt_mp4_handler->pt_key_ext_tbl = NULL;
        }
        pt_mp4_handler->pt_key_ext_tbl = (MP4_IDXBULD_KEY_EXT_TBL_T*)
                    _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_KEY_EXT_TBL_T), 
                                                MM_MEM_MIDX_TAG);
        if (pt_mp4_handler->pt_key_ext_tbl == NULL)
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pt_mp4_handler->pt_key_ext_tbl, 
                 0, 
                 sizeof(MP4_IDXBULD_KEY_EXT_TBL_T));
    }
    
    pt_key_ext_tbl = pt_mp4_handler->pt_key_ext_tbl;

    i4_ret = pf_set_pos(pt_midxbuld_obj, 
                        pt_mp4_handler->piff_inet_moof_info->ui8_mfra_box_start_pos,
                        FM_SEEK_BGN);/*to mfra box*/
    if (i4_ret != INPUTR_OK)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }

    ui4_box_size = pt_mp4_handler->piff_inet_moof_info->ui4_mfra_box_size;
    pui1_tfra_buf = (UINT8*)_mm_util_mem_alloc_with_tag(sizeof(UINT8)*(ui4_box_size - 8), 
                                                        MM_MEM_MIDX_TAG); 
    if(pui1_tfra_buf == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pui1_tfra_buf, 0, (ui4_box_size - 8));

    /*copy mfra box data to buf*/
    i4_ret = pf_copybytes(pt_midxbuld_obj, 
                          pui1_tfra_buf,  
                          (ui4_box_size - 8), 
                          (ui4_box_size - 8), 
                          &ui4_read_size);
    if (i4_ret != INPUTR_OK)
    {
        return MIDXBULDR_INTERNAL_ERR;
    }

    pui1_pstart_buf  = pui1_tfra_buf;        
    pui1_tfra_buf   += 8;   /* ver 1 flag 3 id 4 bit 4*/
    ui4_ver          = *(pui1_tfra_buf);    
    LOADB_DWRD(pui1_tfra_buf + 8, ui4_temp);  
    /*ui4_tmp = (ui4_temp&0x30) + (ui4_temp&0xC) + (ui4_temp&0x3) + 3; */
    ui4_tmp = (ui4_temp&0x3F) + 3;
    LOADB_DWRD(pui1_tfra_buf + 12, ui4_entry_num);
    pui1_tfra_buf += 16; /* to key elem */  

    if( pt_mp4_handler->piff_inet_moof_info->b_is_piff_file         &&
        (!pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file   ||
            (pt_mp4_handler->t_mln_moov_info.b_is_moov_empty &&
                pt_mp4_handler->piff_inet_moof_info->b_is_bivl_file)))
    {
        pt_key_ext_tbl->pt_key_entry = (MP4_MIDXBULD_ELMT_STRM_ENTRY_T*)
            _mm_util_mem_alloc_with_tag(sizeof(MP4_MIDXBULD_ELMT_STRM_ENTRY_T) * ui4_entry_num, 
                                        MM_MEM_MIDX_TAG);
        if(pt_key_ext_tbl->pt_key_entry == NULL)
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pt_key_ext_tbl->pt_key_entry, 
                 0, 
                 sizeof(MP4_MIDXBULD_ELMT_STRM_ENTRY_T)* ui4_entry_num); 
        
        pt_key_ext_tbl->pt_key_ext = (MP4_IDXBULD_KEY_EXT_T*)
            _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_KEY_EXT_T) * ui4_entry_num, 
                                        MM_MEM_MIDX_TAG);
        if (pt_key_ext_tbl->pt_key_ext == NULL)
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pt_key_ext_tbl->pt_key_ext, 
                 0, 
                 sizeof(MP4_IDXBULD_KEY_EXT_T)* ui4_entry_num);
        pt_key_ext_tbl->ui4_key_num = 0;
        ui4_i = 0;
    }
    else
    {
        pt_key_ext_tbl->ui4_key_num = pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_key_cnt;
        ui4_i = pt_mp4_handler->t_mln_moov_info.ui4_mln_moov_key_cnt;
    }   
    
    //pt_key_ext_tbl->pt_key_ext = pt_mp4_handler->pt_key_ext_tbl->pt_key_ext;
    //pt_key_ext_tbl->pt_key_entry = pt_mp4_handler->pt_key_ext_tbl->pt_key_entry;
    
    for(; ui4_i <= ui4_entry_num; ui4_i++)
    {
        if(ui4_ver == 1)
        {
            LOADB_DWRD(pui1_tfra_buf, ui4_high_word);             
            LOADB_DWRD(pui1_tfra_buf + 4, ui4_low_word);
            ui8_time_len = ((UINT64)ui4_high_word << 32) | ui4_low_word;
            
            LOADB_DWRD(pui1_tfra_buf + 8, ui4_high_word); 
            LOADB_DWRD(pui1_tfra_buf + 12, ui4_low_word);
            ui8_ofs_len = ((UINT64)ui4_high_word << 32) | ui4_low_word;                
        }
        else if(ui4_ver == 0)
        {
            LOADB_DWRD(pui1_tfra_buf, ui8_time_len);
            LOADB_DWRD(pui1_tfra_buf + 4, ui4_low_word);  
            ui8_ofs_len = ui4_low_word;
        }

#if 0//ndef __KERNEL__
        if((ui4_i < 10) || (ui4_i + 10 > ui4_entry_num))
        {
            x_dbg_stmt("========  Build 0x%x Key, ofst = 0x%x timelen=0x%x ========\r\n", ui4_i, (UINT32)ui8_ofs_len, (UINT32)ui8_time_len);
        }
#endif
        piff_get_trun_info(pt_midxbuld_obj, pt_key_info, ui8_ofs_len, ui8_time_len, ui4_entry_num);         
        
        if(ui4_ver == 1)
        {
             pui1_tfra_buf += 16;   
        }
        else if(ui4_ver == 0)
        {
             pui1_tfra_buf += 8; 
        }
       
        pui1_tfra_buf += ui4_tmp;
    }  
    pt_key_ext_tbl->ui4_key_num = pt_mp4_handler->pt_key_ext_tbl->ui4_key_num;
    pui1_tfra_buf = pui1_pstart_buf;
    if(pui1_tfra_buf != NULL)
    {
        _mm_util_mem_free_with_tag(pui1_tfra_buf, MM_MEM_MIDX_TAG);     
        /*_mp4_midxbuld_mem_free(pt_midxbuld_obj, pui1_tfra_buf);*/
        pui1_tfra_buf = NULL;
        pui1_pstart_buf = NULL;
    }
 
    return MIDXBULDR_OK;    
}


/*-----------------------------------------------------------------------------
 * Name:  piff_get_trun_info
 *
 * Description: get the trun info to build key tbl;
 *
 * Inputs:  -
 *          
 * Outputs: -
 *
 * Returns: MIDXBULDR_OK                 The API is successful. 
 *          MIDXBULDR_INTERNAL_ERR       The library initialize failed.
 ----------------------------------------------------------------------------*/                            
INT32 piff_get_trun_info(MIDXBULD_OBJ_T*        pt_midx_obj, 
                         MIDXBULD_KEY_INFO_T*   pt_key_info,
                         UINT64                 ui8_ofs_len, 
                         UINT64                 ui8_time_len,
                         UINT32                 ui4_key_num_max)
{  
    UINT32                  ui4_box_size                = 0;     
    UINT32                  ui4_tmp_box_size            = 0;
    UINT32                  ui4_track_id                = 0;
    UINT32                  ui4_read_size               = 0;    
    UINT32                  ui4_tmp                     = 0;  
    UINT32                  ui4_sample_cnt              = 0;
    UINT32                  ui4_data_ofs                = 0;
    UINT32                  flag_tr                     = 0;
    UINT32                  ui4_fourcc                  = 0;
    UINT32                  ui4_key_sample_size         = 0;
    UINT32                  ui4_key_ctts                = 0;
    UINT32                  ui4_curr_pts                = 0;
    INT32                   i4_ret                      = 0;
    UINT8*                  pui1_buff                   = NULL; 
    UINT8*                  pui1_pstart                 = NULL; 
    UINT8*                  pui1_trun_start             = NULL;    
    MP4_IDXBULD_OBJ_T*      pt_mp4_hdlr_obj             = NULL;

    copybytes_fct           pf_copybytes;
    setpos_fct              pf_set_pos; 
    input4bytes_fct         pf_input4;    

    pf_copybytes            = pt_midx_obj->t_input_fct_tbl.pf_copybytes; 
    pf_set_pos              = pt_midx_obj->t_input_fct_tbl.pf_set_pos;    
    pf_input4               = pt_midx_obj->t_input_fct_tbl.pf_input4;
    
    pt_mp4_hdlr_obj = (MP4_IDXBULD_OBJ_T*)pt_midx_obj->pv_handler_obj;
    
    pf_set_pos(pt_midx_obj, ui8_ofs_len, FM_SEEK_BGN); 
    pf_input4 (pt_midx_obj, &ui4_box_size);
    pf_input4 (pt_midx_obj, &ui4_fourcc);
    if(ui4_fourcc == MP4_BOX_MOOF)
    {
        pui1_buff = (UINT8*)_mm_util_mem_alloc_with_tag(ui4_box_size - 8,  MM_MEM_MIDX_TAG); 
        if(pui1_buff == NULL)
        {
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pui1_buff, 0, ui4_box_size - 8);
        
        i4_ret = pf_copybytes(pt_midx_obj, 
                              pui1_buff,  
                              ui4_box_size - 8, 
                              ui4_box_size - 8, 
                              &ui4_read_size);
        if (i4_ret != INPUTR_OK)
        {
            return MIDXBULDR_INTERNAL_ERR;
        }
        pui1_pstart = pui1_buff;
        do
        {
            LOADB_DWRD(pui1_buff,       ui4_tmp_box_size);
            LOADB_DWRD(pui1_buff + 4,   ui4_fourcc);  
            
            switch(ui4_fourcc)
            {
                case MP4_BOX_MFHD:
                    pui1_buff += 16;
                    break;
                case MP4_BOX_TRAF:
                    pui1_buff += 8;
                    break;                
                case MP4_BOX_TFHD:
                {
                    LOADB_DWRD(pui1_buff + 8, ui4_tmp); /*ver & flag */    
                    LOADB_DWRD(pui1_buff + 12, ui4_track_id);
                    if( pt_key_info->ui4_strm_id == ui4_track_id)
                    {                  
                        pui1_buff += 16;              
                        flag_tr = ui4_tmp & 0xFFFFFF;
                        if(flag_tr & 0x1)
                        {
                            pui1_buff += 8;
                        }
                        if(flag_tr & 0x2)
                        {
                            pui1_buff += 4;
                        }
                        if(flag_tr & 0x8)
                        {
                            pui1_buff += 4;                   
                        }
                        if(flag_tr & 0x10)
                        {
                            pui1_buff += 4;                  
                        }
                        if(flag_tr & 0x20)
                        {
                            pui1_buff += 4;                  
                        }
                        if(flag_tr & 0x10000)
                        {
                            //pui1_buff += 4;                    
                        }
                    }
                    else
                    {
                        pui1_buff += ui4_tmp_box_size;
                    }
                }
                break;
                case MP4_BOX_TRUN:
                {  
                    UINT32  ui4_cur_key_num = pt_mp4_hdlr_obj->pt_key_ext_tbl->ui4_key_num;
                    pui1_trun_start = pui1_buff;
                    pt_mp4_hdlr_obj->pt_key_ext_tbl->pt_key_ext[ui4_cur_key_num].ui4_key_trun_entry_ofs 
                                    = ui8_ofs_len + 8 + (pui1_buff - pui1_pstart + 8);  // to head
                    LOADB_DWRD(pui1_buff + 8, ui4_tmp);
                    LOADB_DWRD(pui1_buff + 12, ui4_sample_cnt);
                    pt_mp4_hdlr_obj->pt_key_ext_tbl->pt_key_ext[ui4_cur_key_num].ui4_key_trun_sample_cnt = ui4_sample_cnt;
                    pui1_buff += 16;
                    flag_tr = ui4_tmp & 0xFFFFFF;
                    if(flag_tr & 0x1)
                    {
                        /*add moof_ofs is first elem ofs in mdat,not include head8,
                        need to modify , default_data_ofs concerned*/
                        LOADB_DWRD(pui1_buff, ui4_data_ofs); 
                        pui1_buff += 4;
                    }
                    else
                    {
                        ui4_data_ofs = ui4_box_size;
                        pui1_buff += 4;
                    }
                    if(flag_tr & 0x4)
                    {
                        pui1_buff += 4;
                    }
                    pt_mp4_hdlr_obj->pt_key_ext_tbl->pt_key_entry[ui4_cur_key_num].ui8_relative_offset = ui8_ofs_len + ui4_data_ofs;
                    /* to elem, not head8*/
                    /*trun_ofs[ui4_trun_num].cur_trun_cnt = ui4_sample_cnt;                
                    pt_mp4_hdlr_obj->pt_key_ext_tbl->pt_key_ext[ui4_k].ui4_key_sample_entry_ofs = ui4_ofs_len
                                                        + (pui1_buff - pui1_pstart + 8); */ 
                    
                    if(flag_tr & 0x100)
                    {           
                        pui1_buff += 4;
                    }
                    if(flag_tr & 0x200)
                    {          
                        LOADB_DWRD(pui1_buff, ui4_key_sample_size); 
                        pt_mp4_hdlr_obj->pt_key_ext_tbl->pt_key_entry[ui4_cur_key_num].ui4_size = ui4_key_sample_size;
                        pui1_buff += 4;
                    }
                    if(flag_tr & 0x400)
                    {           
                        pui1_buff += 4;
                    }
                    if(flag_tr & 0x800)
                    {           
                        LOADB_DWRD(pui1_buff, ui4_key_ctts); 
                        pt_mp4_hdlr_obj->pt_key_ext_tbl->pt_key_ext[ui4_cur_key_num].ui4_key_ctts = ui4_key_ctts;
                        pui1_buff += 4;
                    } 
                    if(ui4_cur_key_num == 0)
                    {
                        pt_mp4_hdlr_obj->pt_key_ext_tbl->pt_key_ext[ui4_cur_key_num].ui4_key_ext_sample = 1;
                    }
                    else
                    {
                        pt_mp4_hdlr_obj->pt_key_ext_tbl->pt_key_ext[ui4_cur_key_num].ui4_key_ext_sample = 
                        pt_mp4_hdlr_obj->pt_key_ext_tbl->pt_key_ext[ui4_cur_key_num- 1].ui4_key_ext_sample + ui4_sample_cnt;
                    }
                    /*just find the first trun as key, if contains many trun, need to do more.*/                    

                    ui4_curr_pts = ui4_get_cur_pts(pt_mp4_hdlr_obj->pt_key_ext_tbl->pt_key_ext[ui4_cur_key_num].ui4_key_ctts,
                                                   (UINT32)ui8_time_len,
                                                   pt_key_info->ui4_scale);
                    if(ui4_cur_key_num == 0)
                    {
                        pt_mp4_hdlr_obj->pt_key_ext_tbl->pt_key_entry[ui4_cur_key_num].ui4_pts = 0; 
                    }
                    else
                    {
                        pt_mp4_hdlr_obj->pt_key_ext_tbl->pt_key_entry[ui4_cur_key_num].ui4_pts = 
                                pt_mp4_hdlr_obj->pt_key_ext_tbl->pt_key_entry[ui4_cur_key_num - 1].ui4_pts;
                    }
                    pt_mp4_hdlr_obj->pt_key_ext_tbl->pt_key_entry[ui4_cur_key_num].ui4_pts = ui4_curr_pts;                    
                    pt_mp4_hdlr_obj->pt_key_ext_tbl->pt_key_ext[ui4_cur_key_num].ui8_key_ext_duration_sum = ui8_time_len;                  
        
                    pt_mp4_hdlr_obj->pt_key_ext_tbl->ui4_key_num++;

                    pui1_buff = pui1_trun_start + ui4_tmp_box_size;
                }
                break; 
                default:
                    pui1_buff += ui4_tmp_box_size;
                    break;
            }
            if(ui4_fourcc == MP4_BOX_TRUN)
            {
                break;
            } 
        }while(pui1_buff - pui1_pstart + 8 < ui4_box_size);        
    }
    else
    {
        return MIDXBULDR_INTERNAL_ERR;
    } 
     
    pui1_buff = pui1_pstart;
    if(pui1_buff != NULL)
    {
        _mm_util_mem_free_with_tag(pui1_buff, MM_MEM_MIDX_TAG);        
        pui1_buff = NULL;
        pui1_pstart = NULL;
    }
    return MIDXBULDR_OK; 
}


/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_key_frame_table_alloc
 *
 * Description: This function initialize key frame table obj for indexing
 *
 * Inputs:  
 *         pt_key_info:     specifies key frame table
 *         pv_obj:          specify the media info object  
 *
 * Outputs: NULL 
 *
 * Returns: MIDXBULDR_OK    Success. The routine was successful and 
 *                                   the operation has been completed.
 *          Non-zero values: Fail
 ----------------------------------------------------------------------------*/
INT32 piff_midxbuld_key_frame_table_alloc(MIDXBULD_KEY_INFO_T* pt_key_info, 
                                          VOID*                pv_obj)
{
    INT32                           i4_ret                  = 0; 
    MIDXBULD_ELMT_TBL_DESCRIB_T     t_describ;    
    MIDXBULD_OBJ_T*                 pt_midxbuld_obj         = NULL;    
    MP4_IDXBULD_OBJ_T*              pt_mp4_handler          = NULL;
    MP4_IDXBULD_RANGE_OBJ_T*        pt_range_obj            = NULL;
    MP4_IDXBULD_ELMT_TBL_OBJ_T*     pt_tbl_obj              = NULL;    
    MIDXBULD_ELMT_STRM_ENTRY_T*     pt_idx_entry            = NULL;
    
    pt_midxbuld_obj     = (MIDXBULD_OBJ_T*) pv_obj;  
    pt_mp4_handler      = pt_midxbuld_obj->pv_handler_obj;
   
    if(pt_mp4_handler->piff_inet_moof_info->b_is_mfra_key_exist)
    {   
        i4_ret = piff_get_key_from_mfra(pt_midxbuld_obj, pt_key_info);
    }
    else
    {
        i4_ret = piff_get_key_info(pt_midxbuld_obj,  pt_key_info);
    }
    if(i4_ret != MIDXBULDR_OK)
    {
        return i4_ret;
    }
    
    pt_key_info->ui4_range_id   = MIDXBULD_NULL_ID;
    pt_key_info->ui4_tbl_id     = MIDXBULD_NULL_ID;
    /* allocate elmt tbl range obj memory */
    pt_range_obj = (MP4_IDXBULD_RANGE_OBJ_T*) 
            _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_RANGE_OBJ_T), 
                                        MM_MEM_MIDX_TAG);
    if (pt_range_obj == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    if (TRUE != _mp4_midxbuld_alloc_range_id(pt_midxbuld_obj, 
                                            &pt_key_info->ui4_range_id))
    {
        _mm_util_mem_free_with_tag(pt_range_obj, MM_MEM_MIDX_TAG);
        return MIDXBULDR_OUT_OF_HANDLE;
    }
    pt_mp4_handler->pt_key_range_obj = pt_range_obj;    
    x_memset(pt_range_obj, 0, sizeof(MP4_IDXBULD_RANGE_OBJ_T));
    
    pt_range_obj->e_tbl_type                    = MIDXBULD_TBL_TYPE_ELMT_STRM;
    pt_range_obj->u_range.t_es.ui4_start_pts    = 0;
    pt_range_obj->u_range.t_es.ui4_end_pts      = MIDXBULD_IDX_RANGE_FULL;
    pt_range_obj->ui4_range_id                  = pt_key_info->ui4_range_id;
    pt_range_obj->b_is_key_frame                = TRUE;
    pt_range_obj->b_index_is_valid              = FALSE;
    pt_range_obj->ui1_tbl_total                 = 0;

    t_describ.u_info.t_es.e_es_type             = MIDXBULD_ELEM_STRM_TYPE_KEY;
    t_describ.u_info.t_es.ui4_strm_id           = pt_key_info->ui4_strm_id;
    /* here scale and rate are used to caculate frame rate*/
    t_describ.u_info.t_es.ui4_scale             = pt_key_info->ui4_scale;
    t_describ.u_info.t_es.ui4_rate              = pt_key_info->ui4_rate;
    t_describ.u_info.t_es.ui4_sample_sz         = pt_key_info->ui4_sample_sz;
    t_describ.u_info.t_es.ui4_avg_bytes_per_sec = pt_key_info->ui4_avg_bytes_per_sec;

    /* allocate elmt tbl obj memory */
    pt_tbl_obj = (MP4_IDXBULD_ELMT_TBL_OBJ_T*) 
                _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_ELMT_TBL_OBJ_T), 
                                            MM_MEM_MIDX_TAG);
    if (pt_tbl_obj == NULL)
    {
        _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    if (TRUE != _mp4_midxbuld_alloc_table_id(pt_range_obj, &pt_key_info->ui4_tbl_id))
    {
        _mm_util_mem_free_with_tag(pt_tbl_obj, MM_MEM_MIDX_TAG);
        _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
        pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
        return MIDXBULDR_OUT_OF_HANDLE;
    }
    pt_range_obj->pt_tbl_obj = pt_tbl_obj;    
    x_memset(pt_tbl_obj, 0, sizeof(MP4_IDXBULD_ELMT_TBL_OBJ_T));
    pt_tbl_obj->t_elmt_tbl.ui8_base_offset      = 0;
    pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry  = 0;
    pt_tbl_obj->t_elmt_tbl.e_tbl_type           = MIDXBULD_TBL_TYPE_ELMT_STRM;

    //refer to state, just to estimate the key tbl size
    if(pt_mp4_handler->piff_inet_moof_info->b_is_mfra_key_exist)
    {
        pt_tbl_obj->ui4_tbl_size = pt_mp4_handler->pt_key_ext_tbl->ui4_key_num; 
    }
    else
    {
        pt_tbl_obj->ui4_tbl_size = 0; 
    }   
    pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info = t_describ.u_info.t_es;
    if(pt_tbl_obj->ui4_tbl_size == 0)/*no key table exist*/
    {
        pt_tbl_obj->ui1_is_key_exist = 0;
        pt_idx_entry = NULL;
    }
    else
    {  
        pt_tbl_obj->ui1_is_key_exist = 1;
        pt_idx_entry = (MIDXBULD_ELMT_STRM_ENTRY_T*) 
                _mm_util_mem_alloc_with_tag(sizeof(MIDXBULD_ELMT_STRM_ENTRY_T)
                        * pt_tbl_obj->ui4_tbl_size, 
                        MM_MEM_MIDX_TAG);
        if (pt_idx_entry == NULL)
        {
            pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = NULL;
            _mp4_midxbuld_free_range_obj(pt_midxbuld_obj, pt_key_info->ui4_range_id);
            pt_key_info->ui4_range_id = MIDXBULD_NULL_ID;
            pt_key_info->ui4_tbl_id = MIDXBULD_NULL_ID;
            return MIDXBULDR_OUT_OF_MEMORY;
        }
        x_memset(pt_idx_entry,
                0, 
                sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * pt_tbl_obj->ui4_tbl_size);
    }    
    pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry = pt_idx_entry;
    pt_tbl_obj->ui4_tbl_id = pt_key_info->ui4_tbl_id;
    pt_tbl_obj->pt_next = NULL;    
    pt_mp4_handler->pt_curr_range_obj = pt_range_obj;

    return MIDXBULDR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_key_ext_table_build
 *
 * Description: This function builds key ext table
 *
 * Inputs:  pt_midxbuld_obj     specify the media index builder object  
 *          pt_key_info ,       specifies key frame info
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. Build the key ext table successful
 *          Non-zero values: Fail
 *
 ----------------------------------------------------------------------------*/
/*INT32 piff_midxbuld_parse_inet_buff(MIDXBULD_OBJ_T* pt_midxbuld_obj, UINT8* pui1_buff, 
                                        UINT8* pui1_head_buff)
{
}*/

/*-----------------------------------------------------------------------------
 * Name:  mp4_midxbuld_key_ext_table_build
 *
 * Description: This function builds key ext table
 *
 * Inputs:  pt_midxbuld_obj     specify the media index builder object  
 *          pt_key_info ,       specifies key frame info
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. Build the key ext table successful
 *          Non-zero values: Fail
 *
 ----------------------------------------------------------------------------*/
INT32 piff_midxbuld_key_ext_table_build(
                            MIDXBULD_OBJ_T*         pt_midxbuld_obj,
                            MIDXBULD_KEY_INFO_T*    pt_key_info)
{
    UINT32                          ui4_key_num     = 0; 
    MP4_IDXBULD_OBJ_T*              pt_mp4_handler  = NULL;
    MP4_IDXBULD_KEY_EXT_TBL_T*      pt_key_ext_tbl  = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T*     pt_idx_entry    = NULL;
   
    pt_mp4_handler  = (MP4_IDXBULD_OBJ_T* )pt_midxbuld_obj->pv_handler_obj;   
    ui4_key_num     = pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->ui4_tbl_size;
    pt_key_ext_tbl  = (MP4_IDXBULD_KEY_EXT_TBL_T*)
                _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_KEY_EXT_TBL_T), 
                                            MM_MEM_MIDX_TAG);
    if(pt_key_ext_tbl == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_key_ext_tbl, 0, sizeof(MP4_IDXBULD_KEY_EXT_TBL_T));
    pt_key_ext_tbl->ui4_track_id    = pt_key_info->ui4_strm_id;
    pt_key_ext_tbl->ui8_base_offset = 0;
    pt_key_ext_tbl->pt_next         = NULL;    
    pt_key_ext_tbl->ui4_key_num     = ui4_key_num;
    if(ui4_key_num == 0)
    {
        pt_key_ext_tbl->pt_key_entry= NULL;
        pt_key_ext_tbl->pt_key_ext  = NULL;
        return MIDXBULDR_OK;
    }
    pt_key_ext_tbl->pt_key_entry    = (MP4_MIDXBULD_ELMT_STRM_ENTRY_T*)
                 _mm_util_mem_alloc_with_tag(sizeof(MP4_MIDXBULD_ELMT_STRM_ENTRY_T)
                                            *ui4_key_num, 
                                            MM_MEM_MIDX_TAG);
    pt_key_ext_tbl->pt_key_ext      = (MP4_IDXBULD_KEY_EXT_T*)
                _mm_util_mem_alloc_with_tag(sizeof(MP4_IDXBULD_KEY_EXT_T)
                                            *ui4_key_num, 
                                            MM_MEM_MIDX_TAG);
    if ((  pt_key_ext_tbl->pt_key_entry == NULL) 
        || (pt_key_ext_tbl->pt_key_ext == NULL))
    {
        /*_mp4_free_key_ext_tbl(pt_mp4_handler->pt_key_ext_tbl);*/
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_key_ext_tbl->pt_key_entry, 
             0, 
             sizeof(MP4_MIDXBULD_ELMT_STRM_ENTRY_T) * ui4_key_num);
    x_memset(pt_key_ext_tbl->pt_key_ext, 
             0, 
             sizeof(MP4_IDXBULD_KEY_EXT_T) * ui4_key_num); 
    pt_key_ext_tbl = pt_mp4_handler->pt_key_ext_tbl;
    pt_key_ext_tbl->pt_key_ext = pt_mp4_handler->pt_key_ext_tbl->pt_key_ext;
    pt_key_ext_tbl->pt_key_entry = pt_mp4_handler->pt_key_ext_tbl->pt_key_entry;
    pt_idx_entry = (MIDXBULD_ELMT_STRM_ENTRY_T*) _mm_util_mem_alloc_with_tag(
                    sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * ui4_key_num, 
                    MM_MEM_MIDX_TAG);
    if (pt_idx_entry == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }
    x_memset(pt_idx_entry, 0, sizeof(MIDXBULD_ELMT_STRM_ENTRY_T) * ui4_key_num);
    pt_mp4_handler->pt_key_range_obj->pt_tbl_obj->t_elmt_tbl
                                    .u_fmt.t_es.pt_idx_entry = pt_idx_entry;

    return MIDXBULDR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  piff_midxbuld_elmt_tbl_size_estimate
 *
 * Description: This function builds the index table
 *
 * Inputs:  pt_midxbuld_obj     specify the media index builder object  
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. Build the index table successful
 *          Non-zero values: Fail
 *
 ----------------------------------------------------------------------------*/
INT32 piff_midxbuld_inet_elmt_tbl_size_estimate(MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                                     MP4_IDXBULD_RANGE_OBJ_T*       pt_range_obj, 
                                     MP4_IDXBULD_ELMT_TBL_OBJ_T*    pt_curr_tbl_obj, 
                                     MIDXBULD_ELMT_TBL_DESCRIB_T*   pt_describ)
{
    UINT32                          ui4_tmp                 = 0;
    UINT32                          ui4_sample_cnt          = 0;
    UINT32                          ui4_track_id            = 0;
    UINT32                          ui4_tbl_size            = 0;
    UINT32                          flag_tr                 = 0;
    UINT32                          ui4_box_size            = 0;   
    UINT32                          ui4_fourcc              = 0;
    UINT32                          ui4_trun_size           = 0;
    UINT32                          ui4_moof_size           = 0;   
    UINT32                          ui4_trun_num            = 0;
    UINT32                          ui4_moof_num            = 0;    
    INT32                           ui4_data_ofs            = 0;
    UINT8*                          pui1_buff               = NULL;
    UINT8*                          pui1_pstart             = NULL; 
    UINT8*                          pui1_trace              = NULL;
    UINT8*                          pui1_trun_start         = NULL; 
    UINT8*                          pui1_traf               = NULL; 
    MP4_IDXBULD_OBJ_T*              pt_mp4_handler          = NULL;
    PIFF_BOX_INET_INFO_T*           pt_moof_info            = NULL;
    
    pt_midxbuld_obj->t_input_fct_tbl.pf_input4 = mm_util_finput4bytes_b_fct;
    pt_mp4_handler          = pt_midxbuld_obj->pv_handler_obj;
    pt_moof_info            = pt_mp4_handler->piff_inet_moof_info;

    pui1_trace = pt_moof_info->pui1_moof_buff;
    pui1_pstart = pt_moof_info->pui1_moof_buff;
    pui1_buff = pt_moof_info->pui1_moof_buff;
    pui1_traf = pt_moof_info->pui1_moof_buff;
    
    while(ui4_moof_num < pt_moof_info->ui4_moof_cnt)
    {               
        LOADB_DWRD(pui1_buff, ui4_moof_size); /*THIS MOOF SIZE*/ 
        LOADB_DWRD(pui1_buff + 4, ui4_fourcc); /* MOOF*/
        //pui1_buff += 8;    
        if(ui4_fourcc == MP4_BOX_MOOF)
        {
            LOADB_DWRD(pui1_buff, ui4_moof_size); /*THIS MOOF SIZE*/
            /*pt_midx_obj move 8 bytes, call copybytes fun*/
            pui1_buff += 8; // to mfhd head
            LOADB_DWRD(pui1_buff + 4, ui4_fourcc);/* mfhd */
            if(ui4_fourcc == MP4_BOX_MFHD)
            {
                pui1_buff += 16;
            }

            /* this moof may contain saveral trafs, A,V;*/
            do
            {
                pui1_traf = pui1_buff;  /* to traf head */
                LOADB_DWRD(pui1_buff, ui4_box_size);
                LOADB_DWRD(pui1_buff + 4, ui4_fourcc);/*traf */
                if(ui4_fourcc == MP4_BOX_TRAF)
                {
                    pui1_buff += 8;                    
                    LOADB_DWRD(pui1_buff + 4, ui4_fourcc);/*tfhd*/
                    if(ui4_fourcc == MP4_BOX_TFHD)
                    {
                        LOADB_DWRD(pui1_buff + 8, ui4_tmp); /*ver & flag */  
                        LOADB_DWRD(pui1_buff + 12, ui4_track_id); //  track id;
                        if( pt_describ->u_info.t_es.ui4_strm_id == ui4_track_id)
                        {                   
                            pui1_buff += 16; 
                            flag_tr = ui4_tmp & 0xFFFFFF;
                            if(flag_tr & 0x1)
                            {
                                LOADB_DWRD(pui1_buff, pt_curr_tbl_obj->tfhd_box_info[ui4_moof_num].base_data_ofs); 
                                pui1_buff += 8;
                            }
                            if(flag_tr & 0x2)
                            {
                                pui1_buff += 4;
                            }
                            if(flag_tr & 0x8)
                            {
                                LOADB_DWRD(pui1_buff, pt_curr_tbl_obj->tfhd_box_info[ui4_moof_num].default_sample_duration); 
                                pui1_buff += 4;                   
                            }
                            else
                            {
                                //just for test
                                pt_curr_tbl_obj->tfhd_box_info[ui4_moof_num].default_sample_duration = 0x03E9;
                            }
                            if(flag_tr & 0x10)
                            {
                                LOADB_DWRD(pui1_buff, pt_curr_tbl_obj->tfhd_box_info[ui4_moof_num].default_sample_size); 
                                pui1_buff += 4;                  
                            }
                            if(flag_tr & 0x20)
                            {
                                pui1_buff += 4;                  
                            }
                            if(flag_tr & 0x10000)
                            {                                
                                /* just to check whether duration is empty; */
                            }
                        
                            /* this moof box may contain sevaral truns;*/
                            do
                            {
                                pui1_trun_start = pui1_buff;
                                LOADB_DWRD(pui1_buff , ui4_trun_size);/*trun */
                                LOADB_DWRD(pui1_buff + 4, ui4_fourcc);/*trun */
                                if(ui4_fourcc == MP4_BOX_TRUN)
                                { 
                                    pt_curr_tbl_obj->trun_qseek_tbl[ui4_trun_num].moof_seq_num = ui4_moof_num;
                                    pt_curr_tbl_obj->trun_qseek_tbl[ui4_trun_num].trun_ofs = (pui1_buff - pui1_pstart); // to trun head; 
                                   // x_dbg_stmt("trun ofs: %d\n", pt_curr_tbl_obj->trun_qseek_tbl[ui4_trun_num].trun_ofs);
                                    LOADB_DWRD(pui1_buff + 8, ui4_tmp);
                                    LOADB_DWRD(pui1_buff + 12, ui4_sample_cnt);                    
                                    pui1_buff += 16;
                                    flag_tr = ui4_tmp & 0xFFFFFF;
                                    if(flag_tr & 0x1)
                                    {
                                        /*add moof_ofs is first elem ofs in mdat,not include head8,
                                        need to modify , default_data_ofs concerned*/
                                        LOADB_DWRD(pui1_buff, ui4_data_ofs); 
                                        ui4_data_ofs += pt_curr_tbl_obj->tfhd_box_info[ui4_moof_num].base_data_ofs;
                                        pui1_buff += 4;
                                    }               
                                    if(flag_tr & 0x4)
                                    {
                                        pui1_buff += 4;
                                    }

                                    pt_curr_tbl_obj->trun_qseek_tbl[ui4_trun_num].mdat_elem_ofs = pt_moof_info->pui4_moof_pos[ui4_moof_num] + ui4_data_ofs; /* to elem, not head8*/
                                    pt_curr_tbl_obj->trun_qseek_tbl[ui4_trun_num].cur_trun_cnt = ui4_sample_cnt; 
                                    ui4_tbl_size += ui4_sample_cnt;
                                    pui1_trun_start += ui4_trun_size;
                                    pui1_buff = pui1_trun_start;
                                    ui4_trun_num ++;               
                                }
                                else
                                {
                                    break;
                                }
                            }while(pui1_buff - pui1_pstart < ui4_moof_size);
                        }
                        else
                        {
                            pui1_buff = pui1_traf + ui4_box_size;
                        }
                    }
                }
                else
                {
                   pui1_buff += ui4_box_size;
                }
            }while(pui1_buff - pui1_pstart < ui4_moof_size);
            /*dont parse anymore, to the end of this moof box, to next moof box*/
            pui1_trace +=  ui4_moof_size;
            pui1_buff = pui1_trace; 
            
        }
        else 
        { 
            return 0;
        }
        ui4_moof_num++;    
    }
    pui1_trun_start = NULL;
    pui1_buff = NULL;
    pui1_pstart = NULL;
    pui1_trace = NULL;
    return ui4_tbl_size;
}

/*-----------------------------------------------------------------------------
 * Name:  piff_midxbuld_elmt_tbl_size_estimate
 *
 * Description: This function builds the index table
 *
 * Inputs:  pt_midxbuld_obj     specify the media index builder object  
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. Build the index table successful
 *          Non-zero values: Fail
 *
 ----------------------------------------------------------------------------*/
INT32 piff_midxbuld_elmt_tbl_size_estimate(MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                                     MP4_IDXBULD_RANGE_OBJ_T*       pt_range_obj, 
                                     MP4_IDXBULD_ELMT_TBL_OBJ_T*    pt_curr_tbl_obj, 
                                     MIDXBULD_ELMT_TBL_DESCRIB_T*   pt_describ)
{   
    UINT32                      ui4_key_num                     = 0;
    UINT32                      ui4_start_sample                = (UINT32)0xFFFFFFFF;
    UINT32                      ui4_end_sample                  = 0;
    UINT32                      ui4_i                           = 0;
    UINT32                      ui4_tbl_size                    = 0;
    MP4_IDXBULD_OBJ_T*          pt_mp4_handler                  = NULL;
    MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_tbl                  = NULL; 

    pt_mp4_handler = (MP4_IDXBULD_OBJ_T*) pt_midxbuld_obj->pv_handler_obj;
    pt_key_ext_tbl = pt_mp4_handler->pt_key_ext_tbl;
    ui4_key_num    = pt_key_ext_tbl->ui4_key_num; 

    if (pt_range_obj->u_range.t_es.ui4_start_pts
                    >= pt_range_obj->u_range.t_es.ui4_end_pts)
    {
        return 0;
    }

    if(ui4_key_num == 0)
    {
        /*ui4_start_sample  = 0;
        pt_range_obj->u_range.t_es.ui4_start_pts = ui4_last_range_end_pts;
        pt_curr_tbl_obj->ui4_start_pts = ui4_last_range_end_pts;  

        
        ui4_end_sample = pt_key_ext_tbl->pt_key_ext[ui4_key_num - 1].ui4_key_ext_sample
                                            + pt_key_ext_tbl->pt_key_ext[ui4_key_num - 1].ui4_key_trun_sample_cnt - 1;
        pt_range_obj->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL; 
        pt_curr_tbl_obj->ui4_end_pts = pt_range_obj->u_range.t_es.ui4_end_pts;
        pt_curr_tbl_obj->ui4_start_sample = ui4_start_sample;
        pt_curr_tbl_obj->ui4_end_sample = ui4_end_sample;
        ui4_tbl_size = ui4_end_sample - ui4_start_sample + 1;*/
        return 0;    
    }
    /* cal start key ext index by start pts */    
    for(ui4_i = 0; ui4_i < ui4_key_num; ui4_i++)
    {
        if (pt_key_ext_tbl->pt_key_entry[ui4_i].ui4_pts 
                    > pt_range_obj->u_range.t_es.ui4_start_pts)
        {            
            if(ui4_i > 0)
            {
                ui4_i -= 1;/* Get previous key idx, if first one, just get first*/
            }
        }
        else if (pt_key_ext_tbl->pt_key_entry[ui4_i].ui4_pts 
                    == pt_range_obj->u_range.t_es.ui4_start_pts)
        {/*do nothing with the ui4_i*/
        }
        else
        {
            continue;
        }
            ui4_start_sample  = pt_key_ext_tbl->pt_key_ext[ui4_i]
                                                            .ui4_key_ext_sample;
            pt_range_obj->u_range.t_es.ui4_start_pts
                              = pt_key_ext_tbl->pt_key_entry[ui4_i].ui4_pts;
            pt_curr_tbl_obj->ui4_start_pts
                              = pt_key_ext_tbl->pt_key_entry[ui4_i].ui4_pts;          
            break;
        }
                
    if(((UINT32)0xFFFFFFFF) == ui4_start_sample)
    {
        ui4_start_sample
            = pt_key_ext_tbl->pt_key_ext[ui4_key_num - 1].ui4_key_ext_sample;
        pt_curr_tbl_obj->ui4_start_pts
            = pt_key_ext_tbl->pt_key_entry[ui4_key_num - 1].ui4_pts;
        pt_range_obj->u_range.t_es.ui4_start_pts
            = pt_key_ext_tbl->pt_key_entry[ui4_key_num - 1].ui4_pts;
    }
    /* Get end sample index&pts */
    for(; ui4_i < ui4_key_num; ui4_i++)
    {
       if (pt_key_ext_tbl->pt_key_entry[ui4_i].ui4_pts 
           >= pt_range_obj->u_range.t_es.ui4_end_pts)
       {
           ui4_end_sample = pt_key_ext_tbl->pt_key_ext[ui4_i]
                                           .ui4_key_ext_sample - 1;
           pt_range_obj->u_range.t_es.ui4_end_pts
                       = pt_key_ext_tbl->pt_key_entry[ui4_i].ui4_pts - 1;
           break;
       }
    }
    
    if(ui4_end_sample == 0)
    {   
        ui4_end_sample = pt_key_ext_tbl->pt_key_ext[ui4_key_num - 1].ui4_key_ext_sample
                                        + pt_key_ext_tbl->pt_key_ext[ui4_key_num - 1].ui4_key_trun_sample_cnt - 1;
        pt_range_obj->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;        
    }   
    pt_curr_tbl_obj->ui4_end_pts = pt_range_obj->u_range.t_es.ui4_end_pts;
    pt_curr_tbl_obj->ui4_start_sample = ui4_start_sample;
    pt_curr_tbl_obj->ui4_end_sample = ui4_end_sample;
    ui4_tbl_size = ui4_end_sample - ui4_start_sample + 1;
    
    return ui4_tbl_size;
}
/*-----------------------------------------------------------------------------
 * Name: piff_inet_get_data
 *
 * Description: 
 *
 * Inputs:  pt_midxbuld_obj     specify the media index builder object  
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. Build the index table successful
 *          Non-zero values: Fail
 *
 ----------------------------------------------------------------------------*/
INT32 piff_inet_get_data(MIDXBULD_OBJ_T*  pt_midx_obj)
{  
    UINT32                  ui4_box_size                = 0;
    UINT32                  ui4_fourcc_type             = 0;
    UINT32                  ui4_read_size               = 0;
    UINT32                  ui4_moof_cnt                = 0;
    UINT64                  ui8_cur_moof_pos            = 0;
    INT32                   i4_ret                      = 0;
    UINT8*                  pui1_pstart                 = NULL; 
    UINT8*                  pui1_head_buff              = NULL;   
    MP4_IDXBULD_OBJ_T*      pt_mp4_hdlr_obj             = NULL;
    PIFF_BOX_INET_INFO_T*   pt_moof_info                = NULL;
    setpos_fct              pf_set_pos;   
    copybytes_fct           pf_copybyte;  
    
    pf_copybyte             = pt_midx_obj->t_input_fct_tbl.pf_copybytes;   
    pf_set_pos              = pt_midx_obj->t_input_fct_tbl.pf_set_pos; 
    pt_moof_info            = (PIFF_BOX_INET_INFO_T*)(pt_midx_obj->t_src_info.t_mp4_extra_info.pt_piff_inet_info);
    pt_mp4_hdlr_obj         =(MP4_IDXBULD_OBJ_T*)pt_midx_obj->pv_handler_obj;
    pt_mp4_hdlr_obj->piff_inet_moof_info = pt_moof_info;
    i4_ret                  = MINFOR_INTERNAL_ERR;
    
    pui1_head_buff = (UINT8*)
                    _mm_util_mem_alloc_with_tag(sizeof(UINT8)*8, 
                                                MM_MEM_MIDX_TAG); 
    if(pui1_head_buff == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY;
    }   
    pui1_pstart = pt_moof_info->pui1_moof_buff;  
    ui8_cur_moof_pos = pt_moof_info->ui8_moof_start_pos;   
    do
    {        
        i4_ret = pf_set_pos(pt_midx_obj, ui8_cur_moof_pos ,FM_SEEK_BGN);  /*to moof head */
        i4_ret = pf_copybyte(pt_midx_obj,
                                 pui1_head_buff, 
                                 8, 
                                 8, 
                                 &ui4_read_size);
        if (i4_ret != MINFOR_OK)
        {
            /* to end of file? end flag? do what?*/
           break;
        }
        i4_ret = pf_set_pos(pt_midx_obj, -8 ,FM_SEEK_CUR);  /*to moof head */
        
        LOADB_DWRD(pui1_head_buff , ui4_box_size);
        LOADB_DWRD(pui1_head_buff + 4, ui4_fourcc_type);   
        if(ui4_fourcc_type == MP4_BOX_MOOF)
        {
            if( ((pt_moof_info->pui1_moof_buff - pui1_pstart) + ui4_box_size > PIFF_INET_BUFF_SIZE) ||
                ui4_moof_cnt >= PIFF_INET_MOOF_MAX_COUNT)
            {
                /* buff is full; */            
                break;
            }
            i4_ret = pf_copybyte(pt_midx_obj,
                             pt_moof_info->pui1_moof_buff, 
                             ui4_box_size , 
                             ui4_box_size , 
                             &ui4_read_size); /*add next mdat head 8 bytes, for skip mdat box;*/
            if(i4_ret != INPUTR_OK)
            {
                return MINFOR_INTERNAL_ERR;
            }
            pt_moof_info->pui4_moof_pos[ui4_moof_cnt] = ui8_cur_moof_pos;
            
            pt_moof_info->pui1_moof_buff += ui4_box_size;
            ui4_moof_cnt ++;
        }
                  
        ui8_cur_moof_pos += ui4_box_size;          
        
    }while(1);
    pt_moof_info->ui4_moof_cnt = ui4_moof_cnt;
    pt_moof_info->pui1_moof_buff = pui1_pstart;    
    pt_moof_info->ui8_moof_start_pos = ui8_cur_moof_pos;

    if (i4_ret != MINFOR_OK)
    {
       pt_moof_info->b_is_end_file = TRUE;       
    }
    if(pui1_head_buff != NULL)
    {
        _mm_util_mem_free_with_tag(pui1_head_buff, MM_MEM_MIDX_TAG);  
        pui1_head_buff = NULL;
    }
    pui1_pstart = NULL;
    return MINFOR_OK;  
}

/*-----------------------------------------------------------------------------
 * Name: piff_inet_midxbuld_indexing
 *
 * Description: This function builds the index table
 *
 * Inputs:  pt_midxbuld_obj     specify the media index builder object  
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. Build the index table successful
 *          Non-zero values: Fail
 *
 ----------------------------------------------------------------------------*/
INT32 piff_inet_midxbuld_indexing(MIDXBULD_OBJ_T*  pt_midxbuld_obj, 
                            UINT32* pui4_range_id)
{
    UINT64                      ui8_curr_duration_sum       = 0;
    UINT32                      ui4_i                       = 0;
    UINT32                      ui4_start_key_idx           = 0;    
    UINT64                      ui8_curr_pts                = 0;
    UINT32                      ui4_sample_duration         = 0;
    UINT32                      ui4_sample_size             = 0;
    UINT32                      ui4_sample_ctts             = 0;
    UINT32                      ui4_sample_size_sum         = 0;
    UINT32                      ui4_mem_cnt                 = 0;
    UINT32                      flag_tr                     = 0;
    UINT32                      ui4_tmp                     = 0;
    UINT32                      ui4_cur_sample_idx          = 0;
    INT32                       i4_ret                      = 0;  
    UINT8                       ui1_total_strm_num          = 0;
    UINT8                       ui1_idx                     = 0;
    UINT8                       ui1_strm_idx                = 0;
    UINT8*                      ui1_pui1_buff               = NULL;
    UINT8*                      ui1_pui1_start_buff         = NULL;
    
    MP4_IDXBULD_OBJ_T*          pt_mp4_handler              = NULL;
    MP4_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj                  = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry                = NULL;    
    PIFF_BOX_INET_INFO_T*       pt_moof_info                = NULL;
    MP4_IDXBULD_RANGE_OBJ_T*    pt_range_obj                = NULL;

    if (pt_midxbuld_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    } 
    pt_mp4_handler              = pt_midxbuld_obj->pv_handler_obj;
    if (pt_mp4_handler  == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }
    if(pt_mp4_handler->ui1_range_count >= MP4_MIDXBULD_RANGE_MAX)
    {
        return MIDXBULDR_INITIAL_ERR;
    }
    pt_moof_info                = pt_mp4_handler->piff_inet_moof_info;
    pt_range_obj                = pt_mp4_handler->apt_range_obj[pt_mp4_handler->ui1_range_count];   
  
    pt_mp4_handler->pt_curr_range_obj = pt_range_obj;
    if(!pt_moof_info->b_key_build)
    {
        /* initial local variable */
        *pui4_range_id = pt_mp4_handler->pt_curr_range_obj->ui4_range_id;    
        if (pt_mp4_handler->pt_curr_range_obj->b_index_is_valid == TRUE)
        {
            return MIDXBULDR_OK;
        }    
        if (pt_midxbuld_obj->b_abort_flag == TRUE)
        {
            return MIDXBULDR_ABORT_INDEXING;
        }         
        /*start indexing*/
        pt_tbl_obj = pt_mp4_handler->pt_curr_range_obj->pt_tbl_obj; 
        while (pt_tbl_obj != NULL)
        {
            if (pt_midxbuld_obj->b_abort_flag == TRUE)
            {
                return MIDXBULDR_ABORT_INDEXING;
            }
            if(pt_tbl_obj->ui4_tbl_size == 0)
            {
                pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
            }
            pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;   
            if(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale == 0)
            {
                return MIDXBULDR_INTERNAL_ERR;
            } 
            ui1_pui1_buff = pt_moof_info->pui1_moof_buff;
            ui1_pui1_start_buff = pt_moof_info->pui1_moof_buff;  
            /* get curr track info by pt_describ stream id */
            ui1_total_strm_num = pt_midxbuld_obj->t_src_info.ui1_total_strm_num;
            for (ui1_idx = 0; ui1_idx < ui1_total_strm_num; ui1_idx++)
            {
                if(ui1_idx >= MIDXBULD_ELMT_TBL_MAX)
                {
                    return MIDXBULDR_INTERNAL_ERR;
                }
                if (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_strm_id == 
                    pt_midxbuld_obj->t_src_info.t_es_map[ui1_idx].ui4_strm_id)
                {
                     ui1_strm_idx = ui1_idx;
                     break;
                }
            }
            if((ui1_idx >= ui1_total_strm_num) || 
               (ui1_strm_idx >= MIDXBULD_ELMT_TBL_MAX) ||
               (ui1_pui1_start_buff == NULL))
            {
                return MIDXBULDR_INTERNAL_ERR;
            }
            ui8_curr_duration_sum = pt_mp4_handler->piff_inet_last_duration[ui1_strm_idx];            
            ui4_cur_sample_idx = pt_mp4_handler->piff_inet_last_sample[ui1_strm_idx];            
            pt_tbl_obj->ui4_start_sample = ui4_cur_sample_idx + 1;           
            pt_range_obj->u_range.t_es.ui4_start_pts += 1; 
            pt_tbl_obj->ui4_start_pts = pt_range_obj->u_range.t_es.ui4_start_pts;          
            while(1)
            {                    
                ui1_pui1_buff = ui1_pui1_start_buff + pt_tbl_obj->trun_qseek_tbl[ui4_start_key_idx].trun_ofs + 8;        
                if(i4_ret != INPUTR_OK)
                {
                    return MIDXBULDR_FILE_ERR;
                }
                
                if(ui1_pui1_buff == NULL)
                { 
                    break;
                }  
                LOADB_DWRD(ui1_pui1_buff , ui4_tmp);
                LOADB_DWRD(ui1_pui1_buff + 4, ui4_mem_cnt);
                //x_dbg_stmt("-----------------trun sample cnt: %d\n", ui4_mem_cnt);
                ui1_pui1_buff += 8;
                flag_tr = ui4_tmp & 0xFFFFFF;
                if(flag_tr & 0x1)
                {
                    /*add moof_ofs is first elem ofs in mdat,not include head8,
                    need to modify , default_data_ofs concerned*/
                    //LOADB_DWRD(pui1_buff, ui4_data_ofs); 
                    ui1_pui1_buff += 4;
                }        
                if(flag_tr & 0x4)
                {
                    ui1_pui1_buff += 4;
                }

                /*read trun box data to get duration,size,to cal pts , ofs;*/
                for(ui4_i = 0; ui4_i < ui4_mem_cnt; ui4_i++)
                {           
                    if( flag_tr & 0x100)
                    {
                        LOADB_DWRD(ui1_pui1_buff, ui4_sample_duration);
                        ui1_pui1_buff += 4; 
                    }
                    else
                    {
                        ui4_sample_duration = pt_tbl_obj->tfhd_box_info[pt_tbl_obj->trun_qseek_tbl[ui4_start_key_idx].moof_seq_num].default_sample_duration;
                    }
                    if(flag_tr & 0x200)
                    {
                        LOADB_DWRD(ui1_pui1_buff, ui4_sample_size);                
                        ui1_pui1_buff += 4;                
                    }
                    else
                    {
                        ui4_sample_size = pt_tbl_obj->tfhd_box_info[pt_tbl_obj->trun_qseek_tbl[ui4_start_key_idx].moof_seq_num].default_sample_size;
                    }
                    if( flag_tr & 0x400)
                    {                
                        ui1_pui1_buff += 4; 
                    }
                    if( flag_tr & 0x800)
                    {
                        LOADB_DWRD(ui1_pui1_buff, ui4_sample_ctts);                
                        ui1_pui1_buff += 4;                 
                    }
                    
                    ui8_curr_pts = ui4_get_cur_pts(ui4_sample_ctts,
                                                   (UINT32)ui8_curr_duration_sum,
                                                   pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale);
                    pt_idx_entry->ui4_pts = ui8_curr_pts;            
                    pt_idx_entry->ui4_relative_offset = 
                        pt_tbl_obj->trun_qseek_tbl[ui4_start_key_idx].mdat_elem_ofs + ui4_sample_size_sum; 
                    pt_idx_entry->ui4_size = ui4_sample_size;
                    ui8_curr_duration_sum += ui4_sample_duration;            
                    ui4_sample_size_sum += ui4_sample_size;
                    
                    pt_idx_entry++; 
                    ui4_cur_sample_idx++;
                    if(pt_idx_entry >= (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + 
                                pt_tbl_obj->ui4_tbl_size))
                    {                        
                        break;
                    }                
                }
                /*x_dbg_stmt("---------ui4_start_key_idx : 0x%8x , mdat_ofs : 0x%8x --------\r\n",
                                ui4_start_key_idx, pt_tbl_obj->trun_qseek_tbl[ui4_start_key_idx].mdat_elem_ofs);*/
                ui4_start_key_idx++;
                ui4_sample_size_sum = 0;        
                if(pt_idx_entry >= (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + 
                                pt_tbl_obj->ui4_tbl_size))
                {
                    ui8_curr_pts = ui4_get_cur_pts(ui4_sample_ctts,
                                                   (UINT32)ui8_curr_duration_sum,
                                                   pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale);                    
                    pt_range_obj->u_range.t_es.ui4_end_pts = (UINT32)(ui8_curr_pts - 1); 
                    pt_tbl_obj->ui4_end_pts = (UINT32)(ui8_curr_pts - 1); 
                    if(pt_moof_info->b_is_end_file)
                    {
                        pt_range_obj->u_range.t_es.ui4_end_pts = MIDXBULD_IDX_RANGE_FULL;
                        if(pt_moof_info->pui1_moof_buff != NULL)
                        {
                            _mm_util_mem_free_with_tag(pt_moof_info->pui1_moof_buff, MM_MEM_MIDX_TAG); 
                            pt_moof_info->pui1_moof_buff = NULL;
                        }
                        // free buf and pos ;
                    }
                    pt_tbl_obj->ui4_end_sample = ui4_cur_sample_idx;
                    pt_mp4_handler->piff_inet_last_duration[ui1_strm_idx] = ui8_curr_duration_sum + ui4_sample_duration;
                    pt_mp4_handler->piff_inet_last_sample[ui1_strm_idx] = pt_tbl_obj->ui4_end_sample;
                    break;
                }     
                //ui1_pui1_buff = ui1_pui1_start_buff;
            }
            
            if(ui1_pui1_buff != NULL)
            { 
                ui1_pui1_buff = NULL;
                ui1_pui1_start_buff = NULL;
            }  
            pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = pt_tbl_obj->ui4_tbl_size;
            pt_mp4_handler->pt_curr_range_obj = pt_range_obj;
            pt_mp4_handler->pt_elmt_tbl_alloc->u_range.t_es = pt_range_obj->u_range.t_es;          
#if 0//ndef __KERNEL__

            x_dbg_stmt("indexing ui4_number_of_entry = %d \r\n", 
                      pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry);
            x_dbg_stmt("*****************[PIFF inet]range info start*********************\r\n");
            x_dbg_stmt("start    pts: 0x%8x\r\n", pt_tbl_obj->ui4_start_pts);
            x_dbg_stmt("end      pts: 0x%8x\r\n", pt_tbl_obj->ui4_end_pts);
            x_dbg_stmt("start tbl pts: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                        .t_es.pt_idx_entry[0].ui4_pts);
            x_dbg_stmt("end  tbl pts: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                        .t_es.pt_idx_entry[pt_tbl_obj->ui4_tbl_size -1].ui4_pts);
            x_dbg_stmt("start sample: 0x%8x\r\n", pt_tbl_obj->ui4_start_sample);
            x_dbg_stmt("end   sample: 0x%8x\r\n", pt_tbl_obj->ui4_end_sample);
            x_dbg_stmt("start offset: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                        .t_es.pt_idx_entry[0].ui4_relative_offset);
            x_dbg_stmt("end   offset: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                        .t_es.pt_idx_entry[pt_tbl_obj->ui4_tbl_size -1].ui4_relative_offset);
            x_dbg_stmt("*****************[PIFF]range info end*********************\r\n");
#endif
            pt_tbl_obj = pt_tbl_obj->pt_next;
            ui4_start_key_idx = 0;
        }
    }
    pt_mp4_handler->pt_curr_range_obj->b_index_is_valid = TRUE;

    return MIDXBULDR_OK;

    
}

/*-----------------------------------------------------------------------------
 * Name:  piff_midxbuld_indexing
 *
 * Description: This function builds the index table
 *
 * Inputs:  pt_midxbuld_obj     specify the media index builder object  
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. Build the index table successful
 *          Non-zero values: Fail
 *
 ----------------------------------------------------------------------------*/
INT32 piff_midxbuld_indexing(   MIDXBULD_OBJ_T* pt_midxbuld_obj, 
                                UINT32*         pui4_range_id)
{ 
    UINT64                      ui8_curr_duration_sum       = 0;
    UINT32                      ui4_i                       = 0;
    UINT32                      ui4_range_start_pts         = 0;    
    UINT32                      ui4_key_num                 = 0; 
    UINT32                      ui4_start_key_idx           = 0;    
    UINT64                      ui8_curr_pts                = 0;
    UINT32                      ui4_read_size               = 0;  
    UINT32                      ui4_sample_duration         = 0;
    UINT32                      ui4_sample_size             = 0;
    UINT32                      ui4_sample_ctts             = 0;
    UINT32                      ui4_sample_size_sum         = 0;
    UINT32                      ui4_mem_cnt                 = 0;
    UINT32                      flag_tr                     = 0;
    UINT32                      ui4_tmp                     = 0;
    INT32                       i4_ret                      = 0;  
    UINT8*                      ui1_pui1_buff               = NULL;
    UINT8*                      ui1_pui1_start_buff         = NULL;

    copybytes_fct               pf_copybytes;
    setpos_fct                  pf_set_pos;   
    MP4_IDXBULD_OBJ_T*          pt_mp4_handler              = NULL;
    MP4_IDXBULD_ELMT_TBL_OBJ_T* pt_tbl_obj                  = NULL;
    MIDXBULD_ELMT_STRM_ENTRY_T* pt_idx_entry                = NULL;    
    MP4_IDXBULD_KEY_EXT_TBL_T*  pt_key_ext_tbl              = NULL; 

    if (pt_midxbuld_obj == NULL)
    {
        return MIDXBULDR_INV_ARG;
    } 
    pf_copybytes                = pt_midxbuld_obj->t_input_fct_tbl.pf_copybytes;
    pf_set_pos                  = pt_midxbuld_obj->t_input_fct_tbl.pf_set_pos;
    pt_mp4_handler              = pt_midxbuld_obj->pv_handler_obj;
    pt_key_ext_tbl              = pt_mp4_handler->pt_key_ext_tbl; 

    if (pt_mp4_handler  == NULL)
    {
        return MIDXBULDR_INV_ARG;
    }    
    
    /* initial local variable */
    *pui4_range_id = pt_mp4_handler->pt_curr_range_obj->ui4_range_id;    
    if (pt_mp4_handler->pt_curr_range_obj->b_index_is_valid == TRUE)
    {
        return MIDXBULDR_OK;
    }
    if (pt_mp4_handler->pt_key_range_obj->b_index_is_valid  == TRUE &&
        pt_mp4_handler->ui1_range_count == 0)
    {
        return MIDXBULDR_OK;
    }
    if (pt_midxbuld_obj->b_abort_flag == TRUE)
    {
        return MIDXBULDR_ABORT_INDEXING;
    } 

    ui4_key_num = pt_key_ext_tbl->ui4_key_num; 
    /* key table indexing */
    if (pt_mp4_handler->pt_curr_range_obj->b_is_key_frame)
    {
        pt_tbl_obj = pt_mp4_handler->pt_key_range_obj->pt_tbl_obj;
        /* get related key ext table for key table */
       
        pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = ui4_key_num; 
        pt_tbl_obj->t_elmt_tbl.ui8_base_offset     = pt_key_ext_tbl->ui8_base_offset;
        pt_tbl_obj->t_elmt_tbl.e_tbl_type          = MIDXBULD_TBL_TYPE_ELMT_STRM;
#if 0
        x_memcpy(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry, 
                 pt_key_ext_tbl->pt_key_entry, 
                 sizeof(MIDXBULD_ELMT_STRM_ENTRY_T)*ui4_key_num);
#else
        _mp4_reorder_key_tbl(pt_tbl_obj, pt_key_ext_tbl);
#endif
        return MIDXBULDR_OK;
    }
    
    /* non-key table indexing */
    pt_tbl_obj = pt_mp4_handler->pt_curr_range_obj->pt_tbl_obj;    
    if (pt_midxbuld_obj->b_abort_flag == TRUE)
    {
        return MIDXBULDR_ABORT_INDEXING;
    }
    if(pt_tbl_obj->ui4_tbl_size == 0)
    {
        pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = 0;
    }    
    ui4_range_start_pts = pt_tbl_obj->ui4_start_pts;
    /* cal start key ext index by start pts */
    for(ui4_i = ui4_key_num; ui4_i > 0; ui4_i--)
    {
        if (pt_key_ext_tbl->pt_key_entry[ui4_i-1].ui4_pts < ui4_range_start_pts)
        {
            ui4_start_key_idx = ui4_i - 1;
            break;
        }
        else if(pt_key_ext_tbl->pt_key_entry[ui4_i-1].ui4_pts == ui4_range_start_pts)
        {
            ui4_start_key_idx = ui4_i;
            break;
        }
    }
    /* cal pts */
    if(ui4_key_num == 0)/*no key table exist*/
    {        
       return MIDXBULDR_INTERNAL_ERR;
    }
    else
    { 
        pt_mp4_handler->pt_curr_range_obj->pt_tbl_obj->ui4_start_pts
                    = pt_key_ext_tbl->pt_key_entry[ui4_start_key_idx].ui4_pts;
        pt_mp4_handler->pt_curr_range_obj->pt_tbl_obj->ui4_start_sample
                    = pt_key_ext_tbl->pt_key_ext[ui4_start_key_idx].ui4_key_ext_sample; 
        ui8_curr_duration_sum 
                    = pt_key_ext_tbl->pt_key_ext[ui4_start_key_idx].ui8_key_ext_duration_sum;       
    } 
    
    pt_idx_entry = pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry;   
    if(pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale == 0)
    {
        return MIDXBULDR_INTERNAL_ERR;
    } 
    ui1_pui1_buff = (UINT8*) _mm_util_mem_alloc_with_tag(sizeof(UINT8)*512*16, 
                                            MM_MEM_MIDX_TAG); // 512 sample & all fields present;
    if(ui1_pui1_buff == NULL)
    {
        return MIDXBULDR_OUT_OF_MEMORY; 
    }
    x_memset(ui1_pui1_buff, 0, sizeof(UINT8)*512*16);
    ui1_pui1_start_buff = ui1_pui1_buff;    
    while (1)
    {  
        ui8_curr_duration_sum = pt_key_ext_tbl->pt_key_ext[ui4_start_key_idx]
                                                .ui8_key_ext_duration_sum; 
        pf_set_pos(pt_midxbuld_obj, 
                   (INT64)(pt_key_ext_tbl->pt_key_ext[ui4_start_key_idx].ui4_key_trun_entry_ofs), 
                   FM_SEEK_BGN);       
        i4_ret = pf_copybytes(pt_midxbuld_obj, ui1_pui1_buff,
                                sizeof(UINT8)*512*16, 
                                sizeof(UINT8)*512*16, 
                                &ui4_read_size);
        if(i4_ret != INPUTR_OK)
        {
            return MIDXBULDR_FILE_ERR;
        }      
        LOADB_DWRD(ui1_pui1_buff , ui4_tmp);
        LOADB_DWRD(ui1_pui1_buff + 4, ui4_mem_cnt);           
        ui1_pui1_buff += 8;
        flag_tr = ui4_tmp & 0xFFFFFF;
        if(flag_tr & 0x1)
        {
            /*add moof_ofs is first elem ofs in mdat,not include head8,
            need to modify , default_data_ofs concerned*/
            //LOADB_DWRD(pui1_buff, ui4_data_ofs); 
            ui1_pui1_buff += 4;
        }        
        if(flag_tr & 0x4)
        {
            ui1_pui1_buff += 4;
        }

        // read trun box data to get duration,size,to cal pts , ofs;
        for(ui4_i = 0; ui4_i < ui4_mem_cnt; ui4_i++)
        {           
            if( flag_tr & 0x100)
            {
                LOADB_DWRD(ui1_pui1_buff, ui4_sample_duration);
                ui1_pui1_buff += 4; 
            }
            if(flag_tr & 0x200)
            {
                LOADB_DWRD(ui1_pui1_buff, ui4_sample_size);                
                ui1_pui1_buff += 4;                
            }
            if( flag_tr & 0x400)
            {                
                ui1_pui1_buff += 4; 
            }
            if( flag_tr & 0x800)
            {
                LOADB_DWRD(ui1_pui1_buff, ui4_sample_ctts);                
                ui1_pui1_buff += 4;                 
            }
            
            if(ui4_i == 0)
            {
                 pt_idx_entry->ui4_pts 
                    = pt_key_ext_tbl->pt_key_entry[ui4_start_key_idx].ui4_pts;
                 pt_idx_entry->ui4_relative_offset 
                    = pt_key_ext_tbl->pt_key_entry[ui4_start_key_idx].ui8_relative_offset;
                 pt_idx_entry->ui4_size = ui4_sample_size;
            }
            else
            {              
                ui8_curr_pts = ui4_get_cur_pts(ui4_sample_ctts,
                                               (UINT32)ui8_curr_duration_sum,
                                               pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.t_info.ui4_scale); 
                pt_idx_entry->ui4_pts = (UINT32)ui8_curr_pts; 
                pt_idx_entry->ui4_relative_offset = 
                    pt_key_ext_tbl->pt_key_entry[ui4_start_key_idx].ui8_relative_offset
                    + ui4_sample_size_sum; 
                pt_idx_entry->ui4_size = ui4_sample_size;
                
            }                    
            ui8_curr_duration_sum += ui4_sample_duration;            
            ui4_sample_size_sum += ui4_sample_size;
#if 0 
#ifndef __KERNEL__
            x_dbg_stmt("---------ui4_pts: 0x%8x, ofst: 0x%8x, size: 0x%8x  --------\r\n",
                        pt_idx_entry->ui4_pts,
                        pt_idx_entry->ui4_relative_offset,
                        pt_idx_entry->ui4_size);
#endif
#endif
            pt_idx_entry++; 
            if(pt_idx_entry >= (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + 
                        pt_tbl_obj->ui4_tbl_size))
            {
                break;
            }
        }              
        ui4_start_key_idx++;
        ui4_sample_size_sum = 0;
        if(pt_idx_entry >= (pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry + 
                        pt_tbl_obj->ui4_tbl_size))
        {
            break;
        }
        ui1_pui1_buff = ui1_pui1_start_buff;
    }
    ui1_pui1_buff = ui1_pui1_start_buff;
    if(ui1_pui1_buff != NULL)
    {
       //_mp4_midxbuld_mem_free(pt_midxbuld_obj, ui1_pui1_buff); 
        _mm_util_mem_free_with_tag(ui1_pui1_buff, MM_MEM_MIDX_TAG);  
        ui1_pui1_buff = NULL;
        ui1_pui1_start_buff = NULL;
    }
    pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry = pt_tbl_obj->ui4_tbl_size;

#if 0//ndef __KERNEL__
        {
            UINT32 ui4_n = 0;
            for(ui4_n =0; ui4_n < pt_tbl_obj->ui4_tbl_size; ui4_n++)
            {
                x_dbg_stmt("==piff===baseofst:%#llx, pts: 0x%x, ofst:0x%x, size:0x%x=====\r\n", 
                pt_tbl_obj->t_elmt_tbl.ui8_base_offset, 
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry[ui4_n].ui4_pts,
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry[ui4_n].ui4_relative_offset,
                pt_tbl_obj->t_elmt_tbl.u_fmt.t_es.pt_idx_entry[ui4_n].ui4_size);
            }
        }
#endif

#ifndef __KERNEL__
    x_dbg_stmt("indexing ui4_number_of_entry = %d \r\n", 
              pt_tbl_obj->t_elmt_tbl.ui4_number_of_entry);
    x_dbg_stmt("*****************piff range info start*********************\r\n");
    x_dbg_stmt("start    pts: 0x%8x\r\n", pt_tbl_obj->ui4_start_pts);
    x_dbg_stmt("end      pts: 0x%8x\r\n", pt_tbl_obj->ui4_end_pts);
    x_dbg_stmt("start tbl pts: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                .t_es.pt_idx_entry[0].ui4_pts);
    x_dbg_stmt("end  tbl pts: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                .t_es.pt_idx_entry[pt_tbl_obj->ui4_tbl_size -1].ui4_pts);
    x_dbg_stmt("start sample: 0x%8x\r\n", pt_tbl_obj->ui4_start_sample);
    x_dbg_stmt("end   sample: 0x%8x\r\n", pt_tbl_obj->ui4_end_sample);
    x_dbg_stmt("start offset: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                .t_es.pt_idx_entry[0].ui4_relative_offset);
    x_dbg_stmt("end   offset: 0x%8x\r\n", pt_tbl_obj->t_elmt_tbl.u_fmt
                .t_es.pt_idx_entry[pt_tbl_obj->ui4_tbl_size -1].ui4_relative_offset);
    x_dbg_stmt("***************** range info end*********************\r\n");
#endif

    pt_mp4_handler->pt_curr_range_obj->b_index_is_valid = TRUE;

    return MIDXBULDR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:  piff_get_key_info
 *
 * Description: This function get key info without mfra box
 *
 * Inputs:  pt_midxbuld_obj     specify the media index builder object  
 *
 * Outputs: NULL
 *
 * Returns: MINFOR_OK    Success. Build the index table successful
 *          Non-zero values: Fail
 *
 ----------------------------------------------------------------------------*/
INT32 piff_get_key_info( MIDXBULD_OBJ_T*         pt_midx_obj,
                            MIDXBULD_KEY_INFO_T*    pt_key_info)
{ 
    /*TO DO*/
    return MIDXBULDR_OK;
}



