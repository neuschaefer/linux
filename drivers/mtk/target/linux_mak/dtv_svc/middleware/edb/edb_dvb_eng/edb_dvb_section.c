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
 * Copyright (c) 2006, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 * $RCSfile: ,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         
 *---------------------------------------------------------------------------*/
#include "inc/common.h" 
#include "os/inc/x_os.h"
#include "handle/u_handle_grp.h"
#include "dbg/x_dbg.h"
#include "edb/edb_slctr.h"
#include "edb/x_edb.h"
#include "edb/edb_pool.h"
#include "edb/edb_hash_api.h"
#include "edb_dvb_eng.h"
#include "edb_dvb_section.h"
#include "dt/u_dt.h"
#include "dt/x_dt.h"
#include "tbl_mngr/dvb_si_eng/x_dvb_si_eng.h"
#include "tbl_mngr/dvb_si_eng/dvb_si_txt_def.h"
#include "tbl_mngr/dvb_si_eng/dvb_si_txt_api.h"
#include "os/msgQ/msgq_lib.h"



#define MAX_DESCRIPT_FOR_SAME_LANGUAGE          ((UINT8)0xF     )
#define EPG_DURATION_BETWEEN_WINDOW_START_TIME_AND_CURRENT_TIME     ((TIME_T)  3600)


#ifdef MEASURE_OVERHEAD
UINT32 ui4_section_raw_memory_used = 0;
#endif


typedef struct _DESCRIPTOR_ITEM
{
    UINT8           ui1_tag;
    UINT8           ui1_len;
    UINT8*          pui1_start_addr;
}DESCRIPTOR_ITEM;


typedef struct _LANG_EVENT_DETAIL
{
    ISO_639_LANG_T      t_lang;
/*    UINT8               ui1_short_len;                */
/*    UINT8               aui1_short_desc_pos[10];      */
/*    UINT8               ui1_extended_len;             */
/*    UINT8               aui1_extended_desc_pos[10];   */
    CHAR*               ps_short_text;
    CHAR*               ps_extended_text;
}LANG_EVENT_DETAIL;

typedef struct _MULT_BYTE_UTF8_T
{
        CHAR*   s_buffer;
            UINT32  ui4_len;
}MULT_BYTE_UTF8_T;


static VOID _edb_section_thread_main(VOID*  pv_arg);
static INT32 _dvb_process_linkage(UINT8 ui1_tag,EDB_POOL_EVENT_INFO_T* pt_edb_event_info);
static INT32 _dvb_process_short_event(
        UINT8 ui1_tag,
        EDB_POOL_EVENT_INFO_T* pt_edb_event_info,
        EDB_PREF_LANG_T*  pt_pref_lang,
        ISO_3166_COUNT_T  t_country,
        UINT32   t_operator,
        LANG_EVENT_DETAIL* pt_lang_event_detail,
        UINT16  ui2_svl_id);

static INT32 _edb_section_process_section_nfy(
        VOID*                   pv_section_data,
        UINT32                  ui4_section_len,
        VOID*                   pv_tag,                         /*Is engine data*/
        EDB_EVENT_POOL_CH_KEY   t_locate,
        UINT8                   ui1_table_id,
        UINT8                   ui1_sect_num,
        HANDLE_T                h_eit,
        UINT8*                  pui1_sec_mask,
        UINT8                   ui1_mask);



static INT32 _dvb_process_extended_event(
        UINT8 ui1_tag,
        EDB_POOL_EVENT_INFO_T* pt_edb_event_info,
        EDB_PREF_LANG_T*  pt_pref_lang,
        ISO_3166_COUNT_T  t_country,
        CHAR*             ps_event_detail_separator,
        UINT32   t_operator,
        LANG_EVENT_DETAIL* pt_lang_event_detail,
        UINT16             ui2_svl_id);


static INT32 _dvb_process_guidance(
        UINT8 ui1_tag,
        EDB_POOL_EVENT_INFO_T* pt_edb_event_info,
        EDB_PREF_LANG_T*  pt_pref_lang,
        ISO_3166_COUNT_T  t_country,
        UINT32   t_operator,
        UINT16  ui2_svl_id
        );


static INT32 _dvb_process_time_shift(UINT8 ui1_tag,EDB_POOL_EVENT_INFO_T* pt_edb_event_info);

static INT32 _dvb_process_component(
        UINT8 ui1_tag,
        EDB_POOL_EVENT_INFO_T* pt_edb_event_info,
        EDB_PREF_LANG_T*  pt_pref_lang,
        ISO_3166_COUNT_T  t_country);

static INT32 _dvb_process_ca_identifier(UINT8 ui1_tag,EDB_POOL_EVENT_INFO_T* pt_edb_event_info);

static INT32 _dvb_process_content(UINT8 ui1_tag,EDB_POOL_EVENT_INFO_T* pt_edb_event_info);

static INT32 _dvb_process_rating(UINT8 ui1_tag,EDB_POOL_EVENT_INFO_T* pt_edb_event_info);

static INT32 _dvb_process_private(UINT8 ui1_tag,EDB_POOL_EVENT_INFO_T* pt_edb_event_info);

static BOOL _dvb_check_active_window_and_nfy(
        DVB_ENG_DATA*           pv_eng_data,
        EDB_EVENT_POOL_CH_T*    pt_pool_channel,
        EDB_POOL_EVENT_INFO_T*  pt_edb_event_info);

static BOOL _dvb_check_pf_and_nfy(
        DVB_ENG_DATA*           pv_eng_data,
        EDB_EVENT_POOL_CH_T*    pt_pool_channel,
        EDB_POOL_EVENT_INFO_T*  pt_edb_event_info,
        EDB_NFY_REASON_T        e_reason);

static INT32 _i4_process_utf8(CHAR* buffer,UINT32 ui4_len,MULT_BYTE_UTF8_T* pt_mult_byte);

static INT32 _dvb_pre_process_short_extended(EDB_PREF_LANG_T*  pt_pref_lang,ISO_3166_COUNT_T t_country,UINT16 ui2_svl_id);
static INT32 _dvb_after_process_short_extended(EDB_POOL_EVENT_INFO_T*  pt_edb_event_info,CHAR* ps_event_detail_separator);

static THREAD_DESCR_T  t_edb_section_thread_descr  = {  DEFAULT_STACK_SIZE, 
    DEFAULT_PRIORITY, 
    DEFAULT_NUM_MSGS };

/* EDB Section Thread info */
static HANDLE_T                 h_edb_section_thread                           = NULL_HANDLE;
static HANDLE_T                 h_edb_section_msg_q                            = NULL_HANDLE;
static BOOL                     b_edb_section_inited                           = FALSE;

#ifdef EDB_INTERNAL_MONITOR_EVENT
/*static*/UINT16                ui2_monitor_event_id                           = 0x0;        /*Monitor event id ,For debug used*/
#endif
static DESCRIPTOR_ITEM          t_descript_items[EDB_EVENT_MAX_DES_SIZE]       = {{0}};
static UINT16                   aui2_descript_position[EDB_EVENT_MAX_DES_SIZE] = {0};           /*Recored valid discriptor position in t_descript_items */
static HANDLE_T                 h_txt                                          = NULL_HANDLE;

static UINT8                    ui1_event_extended_item_len                    = 0;
static EDB_EVENT_EXTENDED_ITEM  at_event_extended_item[20]                     = {{NULL,NULL}};
static ISO_639_LANG_T           at_lang[EDB_EVENT_MAX_DES_SIZE]                = {{0}};
static UINT32                   ui4_private_data_specifier                     = 0; 
static LANG_EVENT_DETAIL        at_lang_event_detail[EDB_EVENT_MAX_DES_SIZE]   ;/*= {{0}};*/
static UINT8                    ui1_total_lang_count                           = 0;
CHAR                            s_event_detail_buffer[EDB_EVENT_DETAIL_BUFFER] = {0};
static CHAR                     s_addi_mult_byte[10]                           = {0};
static UINT8                    ui1_addi_mult_byte_len                         = 0;

extern HANDLE_T    g_edb_client_hash;

static CHAR      s_time_log[100] = {0};

CHAR*    edb_dvb_print_local_time(CHAR* s_pre,TIME_T t_time)
{
    DTG_T     t_dtg;
    DTG_T     t_local_dtg;
    x_memset(s_time_log,0x0,100);
    x_dt_utc_sec_to_dtg(t_time, &t_dtg);
    x_dt_conv_utc_local(&t_dtg, &t_local_dtg);
    x_sprintf(s_time_log,"%llu %d/%d/%d %d:%d:%d ",
              t_time,
              t_local_dtg.ui2_yr,
              t_local_dtg.ui1_mo,
              t_local_dtg.ui1_day,
              t_local_dtg.ui1_hr,
              t_local_dtg.ui1_min,
              t_local_dtg.ui1_sec);
    DBG_INFO(("%s %s\r\n",s_pre,s_time_log));
    return s_time_log;
}

VOID edb_convert_raw_text_data(
                        UINT8**           ppui1_raw_data,
                        UINT8             ui1_data_length,
                        ISO_3166_COUNT_T  t_country,
                        UINT16            ui2_svl_id
            )
{
    UINT8*   pui1_raw_data = *ppui1_raw_data;
    UINT8    ui1_start_filter_byte = 1;   /*  fisrt byte indicates coding table , simply ignore it */
    UINT8    ui1_i = 0;

    if(pui1_raw_data[0] == 0x1F)
    {
        return;    /*  if the first byt is 0x1F,we can not replace 0D with ' ' derectly because it is encoded */
    }
    
    if(pui1_raw_data[0] == 0x10)
    {
        ui1_start_filter_byte += 2;    /*  if the first byt is 0x10, we should also ignore the following two bytes according to 300468 */
    }
    
    for(ui1_i=ui1_start_filter_byte;ui1_i<ui1_data_length;ui1_i++)
    {
        if(*(pui1_raw_data + ui1_i) == (CHAR)0x0D) 
        {
            *(pui1_raw_data + ui1_i) = ' ';
        }
    }
    if ((x_strncmp(t_country,"GBR",3) == 0) &&
        (ui2_svl_id == EDB_SVL_ID_DVB_T))
    {
        for(ui1_i=ui1_start_filter_byte;ui1_i<ui1_data_length;ui1_i++)
        {
            if(*(pui1_raw_data + ui1_i) == (CHAR)0x0A) 
            {
                *(pui1_raw_data + ui1_i) = ' ';
            }
        }
    }
}


HANDLE_T edb_dvb_section_get_msg_q_hdl (VOID)
{
    return h_edb_section_msg_q;
}



INT32 edb_section_init(
        const THREAD_DESCR_T*   pt_edb_section_thread_descr)
{
    INT32                       i4_ret = 0;

    if ( !edb_is_inited())
    {
        return EDBR_INTERNAL_ERROR;
    }

    if (b_edb_section_inited == TRUE)
    {
        return EDBR_ALREADY_INIT;
    }

    if ( pt_edb_section_thread_descr == NULL)
    {
        t_edb_section_thread_descr.ui1_priority = EDB_SECTION_THREAD_DEFAULT_PRIORITY;
        t_edb_section_thread_descr.ui2_num_msgs = EDB_SECTION_DEFAULT_NUM_OF_MSGS;/*(UINT16)(MSGQ_MAX_MSGS) ;//EDB_SECTION_DEFAULT_NUM_OF_MSGS;*/
        t_edb_section_thread_descr.z_stack_size = EDB_SECTION_THREAD_DEFAULT_STACK_SIZE;
    }
    else
    {
        t_edb_section_thread_descr.ui1_priority = 
            (pt_edb_section_thread_descr->ui1_priority == DEFAULT_PRIORITY)?
            EDB_SECTION_THREAD_DEFAULT_PRIORITY:
            pt_edb_section_thread_descr->ui1_priority;

        t_edb_section_thread_descr.ui2_num_msgs = 
            (pt_edb_section_thread_descr->ui2_num_msgs == DEFAULT_NUM_MSGS)?
            EDB_SECTION_DEFAULT_NUM_OF_MSGS:
            pt_edb_section_thread_descr->ui2_num_msgs;

        t_edb_section_thread_descr.z_stack_size = 
            (pt_edb_section_thread_descr->z_stack_size == DEFAULT_STACK_SIZE)?
            EDB_SECTION_THREAD_DEFAULT_STACK_SIZE:
            pt_edb_section_thread_descr->z_stack_size;
    }


    i4_ret = x_thread_create( &h_edb_section_thread,
                              EDB_SECTION_THREAD_NAME,
                              t_edb_section_thread_descr.z_stack_size,
                              t_edb_section_thread_descr.ui1_priority,
                              &_edb_section_thread_main,
                              0,
                              NULL );
    if ( i4_ret != OSR_OK )
    {
        i4_ret = EDBR_INTERNAL_ERROR;
        goto EDB_INIT_EXIT;
    }

    b_edb_section_inited=TRUE;

    i4_ret = EDBR_OK;
EDB_INIT_EXIT:

    return i4_ret;
}


INT32 _dvb_txt_process(
        UINT8*  pui1_source_buffer,
        SIZE_T  z_source_len,
        ISO_3166_COUNT_T  t_count_code,
        UINT32   t_operator,
        E_DVB_TEXT_CHAR_TBL e_char_tbl,
        DVB_SI_TXT_INFO_T*  pt_dvb_si_txt)
{
    INT32           i4_ret                  = 0;
    TXT_CTRL_OBJ_T* pt_txt_ctrl_obj         = NULL;
    TXT_OBJ_T*      pt_txt                  = NULL;
    SIZE_T          z_get_len_size          = sizeof (UINT16);
    SIZE_T          z_get_len               = 0;


    EDB_CHECK_RET( 
            (pui1_source_buffer == NULL || z_source_len <= (SIZE_T)(0x0) || pt_dvb_si_txt == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );


    i4_ret =  dvb_si_txt_create_descr_str_handle(
            (CHAR*)pui1_source_buffer,
            z_source_len,
            t_count_code,
            &h_txt
            );

    if (i4_ret != TMR_OK)
    {
        DBG_INFO((_EDB_INFO"create txt handle ret=%d\r\n",i4_ret));
        return i4_ret;
    }

    handle_get_obj(h_txt,(VOID**) (&pt_txt_ctrl_obj));

    if (pt_txt_ctrl_obj == NULL)
    {
        DBG_INFO((_EDB_INFO"pt_txt_ctrl_obj is NULL\r\n",i4_ret));
        return EDBR_INTERNAL_ERROR;
    }
        
    pt_txt = pt_txt_ctrl_obj->pt_txt_obj;
    pt_dvb_si_txt->e_char_table = pt_txt->t_dvb_str.e_char_tbl;

    if (e_char_tbl != DVB_TEXT_CHAR_CODE_INVALID)/*Use user asigned coding table*/
    {
        pt_txt->t_dvb_str.e_char_tbl = e_char_tbl;
        if (e_char_tbl == DVB_TEXT_CHAR_CODE_ISO_10646_1_UTF_8)
        {
            pt_txt->t_dvb_str.e_char_tbl = DVB_TEXT_CHAR_CODE_ISO_10646_1;
        }
    }

    i4_ret = dvb_si_txt_get_obj(
            pt_txt,
            DVB_SI_GET_TYPE_TXT_LEN,
            NULL,
            &z_get_len,
            &z_get_len_size
            );

    if (z_get_len == (SIZE_T)0x0)
    {
        i4_ret = EDBR_PARSE_ERROR;
        dvb_si_txt_free_obj(pt_txt_ctrl_obj);
        return i4_ret;
    }

    if (pt_dvb_si_txt->ps_txt == NULL)
    {
        pt_dvb_si_txt->ps_txt = EDB_PARTITION_ALLOC(z_get_len +1);
        if (pt_dvb_si_txt->ps_txt != NULL)
        {
            x_memset(pt_dvb_si_txt->ps_txt,0x0, z_get_len +1);
            pt_dvb_si_txt->pz_txt_len = &z_get_len;
        }
        else
        {
            dvb_si_txt_free_obj(pt_txt_ctrl_obj);
            return i4_ret;
        }
    }


    i4_ret = dvb_si_txt_get_obj(
            pt_txt,
            DVB_SI_GET_TYPE_TXT,
            NULL,
            pt_dvb_si_txt,
            NULL
            );

    dvb_si_txt_free_obj(pt_txt_ctrl_obj);
    return i4_ret;

}
/*-----------------------------------------------------------------------------
 * Name: _dvb_lang_match
 *
 * Description: This function checks whether if two specified languages match.
 *
 * Inputs:  pt_lang1    The first language. The language in tables.
 *          pt_lang2    The second language. The Menu language.
 *
 * Outputs: -
 *
 * Returns: TRUE if match, otherwise, FALSE.
 *
 ----------------------------------------------------------------------------*/
BOOL _dvb_lang_match (ISO_639_LANG_T*  pt_lang1,
                             ISO_639_LANG_T*  pt_lang2)
{
    const CHAR* ps_lang1 = (const CHAR *) *pt_lang1;
    const CHAR* ps_lang2 = (const CHAR *) *pt_lang2;
    BOOL b_res = FALSE;

    /* compare 3 characters */
    if (ps_lang2 == NULL || ps_lang2[0] == '\x0' || !x_strcmp(ps_lang2, "und"))
    {
        b_res = TRUE;
    }
    else if (!x_strncmp(ps_lang1, ps_lang2, 3))
    {
        b_res = TRUE;
    }
    else if (ps_lang1[0] == 'f' && ps_lang2[0] == 'f') /* French */ 
    {
        if (!x_strncmp(ps_lang1, "fre", 3) && !x_strncmp(ps_lang2, "fra", 3))
        {
            b_res = TRUE;
        }
    }
    else if (ps_lang1[0] == 's' && ps_lang2[0] == 'e') /* Spanish */
    {
        if (!x_strncmp(ps_lang1, "spa", 3) && !x_strncmp(ps_lang2, "esl", 3))
        {
            b_res = TRUE;
        }
    }
    else if (ps_lang1[0] == 'g' && ps_lang2[0] == 'd') /* Germany */
    {
        if (!x_strncmp(ps_lang1, "ger", 3) && !x_strncmp(ps_lang2, "deu", 3))
        {
            b_res = TRUE;
        }
    }

    return b_res;
}

BOOL _dvb_check_private_is_valid (UINT8   ui1_private_descriptor_tag ,UINT32 ui4_private_specifier )
{
    if(ui1_private_descriptor_tag >= DVB_USER_DTT_DESCR_PREFERRED_NAME_IDENTIFIER && ui1_private_descriptor_tag <= DVB_USER_DTT_DESCR_TAG_GUIDANCE )
    {
        if(ui4_private_specifier == DVB_UK_DTT_PRIVATE_DATA_SPECIFIER_VALUE)
        {
            return TRUE;
        }
    }
    return FALSE;
}

INT32 edb_section_clean_msg(DVB_ENG_DATA*       pt_eng_data )
{
    INT32               i4_ret      = 0;
#if 1
    UINT16              ui2_index   = 0;
    EDB_SECTION_MSG_T   t_msg       = {0};
    SIZE_T              z_msg_size  = 0; 
/*    UINT32                ui4_index    =0 ;*/
    
    UINT16              ui2_num_msgs= 0;
    
    #if 0
    BUFFER_PARSER               t_buffer_parser = {0};

    UINT8   ui1_tbl_id          = 0x0;
    UINT16  ui2_sect_len        = 0x0;
    UINT16  ui2_svc_id          = 0x0;
    UINT8   ui1_version         = 0x0;
    BOOL    b_curr_next_ind     = FALSE;
    UINT8   ui1_sect_num        = 0x0;
    UINT16  ui2_ts_id           = 0x0;
    UINT16  ui2_on_id           = 0x0;
    DVB_SI_ON_TS_SVC_ID_SEC_INFO_T      t_sec_info = {0};
    TM_COND_T                           e_cond;
    EDB_EVENT_POOL_T*             pt_edb_event_pool = NULL;
    EDB_EVENT_POOL_CH_KEY       t_key = {0};
    EDB_EVENT_POOL_CH_T*        pt_channel = NULL;



    EDB_CHECK_RET( 
            (pt_eng_data == NULL || pt_eng_data->pt_edb == NULL),
            (EDBR_INTERNAL_ERROR),
            ((_EDB_INFO"Wrong  argument\r\n"))
            );


    pt_edb_event_pool = &(pt_eng_data->pt_edb->t_edb_event_pool);
    #endif


    if (x_msg_q_num_msgs (edb_dvb_section_get_msg_q_hdl(), &ui2_num_msgs) == OSR_OK)
    {
        if (ui2_num_msgs > 0)
        {
            x_dbg_stmt("{EDB} begin free %d message\r\n",ui2_num_msgs);
        }

        for (ui2_num_msgs=0;ui2_num_msgs<ui2_num_msgs;ui2_num_msgs++)
        {
            z_msg_size = sizeof( EDB_SECTION_MSG_T );

            i4_ret = x_msg_q_receive( &ui2_index,
                                      &t_msg,
                                      &z_msg_size,
                                      &h_edb_section_msg_q,
                                      (UINT16)1,
                                      X_MSGQ_OPTION_WAIT );

            if( t_msg.e_msg_type == EDB_SECTION_MSG_SECTION_NFY)
            {
                #ifndef SEC_MEMORY_DEBUG 
                    x_mem_free((VOID*)t_msg.t_section_msg_nfy.pv_section_data);
                #else
                    EDB_PARTITION_FREE((VOID*)t_msg.t_section_msg_nfy.pv_section_data);
                #endif
            }
        }

#if 0     
        while(FALSE &&  ui2_num_msgs > 0 )
        {

            z_msg_size = sizeof( EDB_SECTION_MSG_T );

            i4_ret = x_msg_q_receive( &ui2_index,
                                      &t_msg,
                                      &z_msg_size,
                                      &h_edb_section_msg_q,
                                      (UINT16)1,
                                      X_MSGQ_OPTION_WAIT );

            #ifndef SEC_MEMORY_DEBUG 
                x_mem_free((VOID*)t_msg.t_section_msg_nfy.pv_section_data);
            #else
                EDB_PARTITION_FREE((VOID*)t_msg.t_section_msg_nfy.pv_section_data);
            #endif
            

            if (
                    t_msg.e_msg_type ==EDB_SECTION_MSG_SECTION_NFY &&
                    t_msg.t_section_msg_nfy.pv_section_data != NULL
               )
            {

                t_buffer_parser.pui1_buffer = (VOID*)t_msg.t_section_msg_nfy.pv_section_data;//Raw data start address

                buffer_parse_data(&t_buffer_parser, 8, &ui1_tbl_id     );     //table_id 8 uimsbf
                buffer_parse_data(&t_buffer_parser, 1, NULL            );     //section_syntax_indicator 1 bslbf
                buffer_parse_data(&t_buffer_parser, 1, NULL            );     //reserved_future_use 1 bslbf
                buffer_parse_data(&t_buffer_parser, 2, NULL            );     //reserved 2 bslbf
                buffer_parse_data(&t_buffer_parser, 12,&ui2_sect_len  );     //section_length 12 uimsbf
                buffer_parse_data(&t_buffer_parser, 16,&ui2_svc_id    );     //service_id 16 uimsbf
                buffer_parse_data(&t_buffer_parser, 2, NULL            );     //reserved 2 bslbf
                buffer_parse_data(&t_buffer_parser, 5, &ui1_version    );     //version_number 5 uimsbf
                buffer_parse_data(&t_buffer_parser, 1, &b_curr_next_ind);     //current_next_indicator 1 bslbf
                buffer_parse_data(&t_buffer_parser, 8, &ui1_sect_num   );     //section_number 8 uimsbf
                buffer_parse_data(&t_buffer_parser, 8, NULL            );     //last_section_number 8 uimsbf
                buffer_parse_data(&t_buffer_parser, 16,&ui2_ts_id     );     //transport_stream_id 16 uimsbf
                buffer_parse_data(&t_buffer_parser, 16,&ui2_on_id     );     //original_network_id 16 uimsbf

                t_sec_info.ui2_on_id = ui2_on_id;
                t_sec_info.ui2_ts_id = ui2_ts_id;
                t_sec_info.ui2_svc_id = ui2_svc_id;
                t_sec_info.ui1_tbl_id = ui1_tbl_id;
                t_sec_info.ui1_sec_num = ui1_sect_num; 

                t_key.ui2_svl_id = pt_eng_data->ui2_svl_id;
                t_key.ui2_on_id = ui2_on_id;
                t_key.ui2_ts_id = ui2_ts_id;
                t_key.ui2_svc_id = ui2_svc_id;

                i4_ret = edb_pool_get_ch_obj(pt_edb_event_pool,&t_key,&pt_channel);
              if(i4_ret != EDBR_OK)
              {
                pt_channel = NULL;
              }
                if (pt_channel != NULL)
                {
                    if (pt_channel->pv_data != NULL)
                    {
                        x_memset(pt_channel->pv_data,0x0,sizeof(EDB_EVENT_POOL_CH_VER));
                    }
                }

                x_dvb_si_set_eit_section_mask_clear(t_msg.t_section_msg_nfy.h_eit, &t_sec_info, &e_cond);
                {
                    UINT8*                  pui1_sec_mask = NULL;
                    UINT8                   ui1_mask      = 0x0;

                    pui1_sec_mask = t_msg.t_section_msg_nfy.pui1_sec_mask;
                    ui1_mask      = t_msg.t_section_msg_nfy.ui1_mask;

                    if (pui1_sec_mask != NULL)
                    {
                        (*pui1_sec_mask) &= (~ui1_mask);                //Clear section mask
                    }
                }
                
                
                //DBG_INFO((_EDB_INFO"Parse header end\r\n"));
#ifndef SEC_MEMORY_DEBUG 
                x_mem_free((VOID*)t_msg.t_section_msg_nfy.pv_section_data);
#else
                EDB_FREE((VOID*)t_msg.t_section_msg_nfy.pv_section_data);
#endif
            }

            if (x_msg_q_num_msgs (edb_dvb_section_get_msg_q_hdl(), &ui2_num_msgs) != OSR_OK)
            {
                break;
            }
        }

#endif

        
    }

#endif
    return i4_ret;
}



/*-----------------------------------------------------------------------------
 * Name: _edb_section_thread_main
 *
 * Description: main function of EDB Section thread
 *
 * Inputs:  pv_arg References.
 *
 * Outputs: - 
 *
 * Returns: -
 *
 ----------------------------------------------------------------------------*/
static VOID _edb_section_thread_main(VOID*  pv_arg)
{
    INT32               i4_ret      = 0;
    UINT16              ui2_index   = 0;
    EDB_SECTION_MSG_T   t_msg       = {0};
    SIZE_T              z_msg_size  = 0; 
    BOOL                b_shutdown  = FALSE;

    i4_ret = x_msg_q_create( &h_edb_section_msg_q,
                             (CHAR*)EDB_SECTION_MSGQ_NAME,
                             sizeof( EDB_SECTION_MSG_T ),
                             t_edb_section_thread_descr.ui2_num_msgs );
    if ( i4_ret != OSR_OK )
    {
        DBG_ABORT( DBG_MOD_EDB );
        /* program abort here. */
    }

    while( !b_shutdown )
    {

        z_msg_size = sizeof( EDB_SECTION_MSG_T );

        i4_ret = x_msg_q_receive( &ui2_index,
                                  &t_msg,
                                  &z_msg_size,
                                  &h_edb_section_msg_q,
                                  (UINT16)1,
                                  X_MSGQ_OPTION_WAIT );
        if ( i4_ret == OSR_OK )
        {
            switch( t_msg.e_msg_type )
            {
                case EDB_SECTION_MSG_SHUTDOWN:
                    b_shutdown = TRUE;
                    break;
          case EDB_SECTION_MSG_CLEAN_NFY:
                    i4_ret = edb_section_clean_msg(  (DVB_ENG_DATA*) (t_msg.t_section_msg_nfy.pv_section_data) );
                    break;
                case EDB_SECTION_MSG_SECTION_NFY:
                    {
#if 0
                        UINT32  ui4_start = 0;
                        UINT32  ui4_end = 0;
                        UINT16  ui2_msg_cnt = 0;
                        if (x_msg_q_num_msgs (h_edb_section_msg_q, &ui2_msg_cnt) == OSR_OK)
                        {
                            if (ui2_msg_cnt >= (UINT16)50 )
                            {
                                DBG_INFO((_EDB_INFO"Number of msgs [%u] \r\n",ui2_msg_cnt));
                                //x_cli_parser("mw.os.sp");
                            }
                        }
#endif


                        edb_pool_lock_wr();
                        //ui4_start = x_os_get_sys_tick();
                        i4_ret = _edb_section_process_section_nfy(      /*Will use 2-3 tick*/
                                t_msg.t_section_msg_nfy.pv_section_data,
                                t_msg.t_section_msg_nfy.ui4_section_len,
                                t_msg.t_section_msg_nfy.pv_tag,
                                t_msg.t_section_msg_nfy.t_locate,
                                t_msg.t_section_msg_nfy.ui1_table_id,
                                t_msg.t_section_msg_nfy.ui1_sect_num,
                                t_msg.t_section_msg_nfy.h_eit,
                                t_msg.t_section_msg_nfy.pui1_sec_mask,
                                t_msg.t_section_msg_nfy.ui1_mask
                                );
                        //ui4_end = x_os_get_sys_tick();
#if 0
                        DBG_INFO(("Decode single event=%d(tick)\r\n",ui4_end - ui4_start));
#endif
                        edb_pool_unlock();



                        if (i4_ret != EDBR_OK)//Process Section NFY error,cleae mask
                        {

                        }

                        if (t_msg.t_section_msg_nfy.pv_section_data != NULL)
                        {
#ifndef SEC_MEMORY_DEBUG 
                            x_mem_free((VOID*)t_msg.t_section_msg_nfy.pv_section_data);
#else
                            EDB_PARTITION_FREE((VOID*)t_msg.t_section_msg_nfy.pv_section_data);
#endif
                        }

                        break;
                    }

                default:
                    break;
            }
            if (b_shutdown)
            {
                break;
            }
        }
        x_thread_delay( 8 );
    } /* while: the main loop */

    x_msg_q_delete( h_edb_section_msg_q );

    b_edb_section_inited = FALSE;

    x_thread_exit();

}


static INT32 _edb_dvb_delete_old_secion_event(
        EDB_EVENT_POOL_CH_T*        pt_channel,
        UINT8                       ui1_table_id,
        UINT8                       ui1_sect_num
        )
{
    INT32                           i4_ret = 0;
    HANDLE_T                        h_event_hash    = NULL_HANDLE;
    UINT32                          ui4_i           = 0;
    P_ELEMENT_OBJ_T                 pt_elm          = NULL;
    EDB_HASH_T*                     pt_hash_obj     = NULL;
    HANDLE_TYPE_T                   e_hdl_type;
    EDB_POOL_EVENT_INFO_T*          pt_pool_event   = NULL;
    

    EDB_CHECK_RET( 
            (pt_channel == NULL ),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    h_event_hash = pt_channel->h_event_hash;
    if (h_event_hash == NULL_HANDLE)
    {
        return EDBR_INFO_NOT_FOUND; 
    }


    i4_ret = handle_get_type_obj( h_event_hash, &e_hdl_type, (VOID**)&pt_hash_obj );
    if ( i4_ret != HR_OK || e_hdl_type != EDBT_HASH )
    {
        return EDBR_INV_HANDLE;
    }

    for ( ui4_i=0; ui4_i<pt_hash_obj->z_table_size; ++ui4_i )
    {
        for( pt_elm = pt_hash_obj->ppt_table[ui4_i]; pt_elm != NULL; )
        {
            pt_pool_event = (EDB_POOL_EVENT_INFO_T*)pt_elm->pv_obj;
           

            if (    
                    (pt_pool_event != NULL) &&
                    (pt_pool_event->ui1_sect_num == ui1_sect_num) &&
                    (pt_pool_event->ui1_table_id == ui1_table_id) 
               )
            {
                pt_elm = pt_elm->pt_next;

                if (pt_channel->pt_present_event != NULL && pt_channel->pt_present_event->ui2_event_id == pt_pool_event->ui2_event_id)
                {
                    edb_pool_del_present_event_obj(pt_channel);
                }
                else if (pt_channel->pt_following_event != NULL && pt_channel->pt_following_event->ui2_event_id == pt_pool_event->ui2_event_id)
                {
                    edb_pool_del_following_event_obj(pt_channel);
                }
                else
                {
                    edb_pool_del_event_obj(pt_channel,pt_pool_event);
                }
                continue;
            }
            pt_elm = pt_elm->pt_next;
        }
    }

    return i4_ret;
}


static INT32 _edb_section_process_section_nfy(
        VOID*                   pv_section_data,
        UINT32                  ui4_section_len,
        VOID*                   pv_tag,                         /*Is engine data*/
        EDB_EVENT_POOL_CH_KEY   t_locate,
        UINT8                   ui1_table_id,
        UINT8                   ui1_sect_num,
        HANDLE_T                h_eit,
        UINT8*                  pui1_sec_mask,
        UINT8                   ui1_mask
)
{
    INT32                       i4_ret                     = 0;
    DVB_ENG_DATA*               pt_eng_data                = NULL;
    EDB_EVENT_POOL_T*           pt_edb_event_pool          = NULL;
    EDB_EVENT_POOL_CH_T*        pt_channel                 = NULL;
    BUFFER_PARSER               t_buffer_parser            = {0};
    DTG_T                       t_dtg                      = {0};
    EDB_POOL_EVENT_INFO_T*      pt_edb_event_info          = NULL;
    TIME_T                      t_duration                 = 0;
    UINT16                      ui2_descriptor_len         = 0;
    UINT32                      ui4_descriptot_start_token = 0;
    UINT8                       ui1_descriptor_tag         = 0;
    UINT8                       ui1_descriptor_length      = 0;
    UINT8                       ui1_descriptor_count       = 0;
    BOOL                        b_notified                 = FALSE;
    UINT8                       aui1_bcd[3]                = {0};
    EDB_PREF_LANG_T             t_edb_pre_lang             = {0};
    INT32                       i4_lang_index              = 0;

        
    EDB_CHECK_RET( 
            (pv_section_data == NULL || pv_tag == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pt_eng_data = (DVB_ENG_DATA*)pv_tag;
    pt_edb_event_pool = &(pt_eng_data->pt_edb->t_edb_event_pool);
    EDB_CHECK_RET( 
            (pt_edb_event_pool == NULL),
            (EDBR_INV_ARG),
            (("The pool is null\r\n"))
            );

    t_buffer_parser.pui1_buffer = (UINT8*)pv_section_data;                                 //Raw data start address
    buffer_parse_data(&t_buffer_parser, 14*8, NULL                                      ); //skip 14Byte

    if (t_buffer_parser.ui4_token >=  (ui4_section_len -4/*CRC code*/))                      //Have more event
    {

        return i4_ret;
    }
    edb_pool_lock_wr();
    #if 0
    x_dbg_stmt("ui4_section_len=%d ui1_table_id=%d ui1_sect_num=%d \r\n",
               ui1_sect_num,ui1_table_id,ui1_sect_num);
    #endif
        


    /*Free pool event for this section*/
    i4_ret = edb_pool_get_ch_obj(pt_edb_event_pool, &t_locate, &pt_channel);
    
    if (i4_ret == EDBR_OK && ui1_table_id != 0x4E && ui1_table_id != 0x4F)
    {
        _edb_dvb_delete_old_secion_event(pt_channel,ui1_table_id,ui1_sect_num); 
    }    



START_SINGLE_EVENT:    

/*    if ( pt_eng_data->t_edb_eng_state.b_asu_mode == TRUE)
    {
         TIME_T                  t_current_time        = NULL_TIME;
         TIME_T                  t_start_time        = NULL_TIME;
         DT_COND_T               t_dt_cond             = DT_NOT_RUNNING;
         BUFFER_PARSER           t_temp_buffer_parser  = t_buffer_parser;
         UINT8                   aui1_temp_bcd[3]                = {0};
         
         t_current_time = x_dt_get_brdcst_utc(NULL,&t_dt_cond);
         if(t_dt_cond == DT_SYNC_RUNNING)
         {
             buffer_parse_data(&t_temp_buffer_parser, 16  , NULL);
             buffer_parse_data(&t_temp_buffer_parser, 40  , &(t_start_time));
             x_dt_mjd_bcd_to_dtg( (UINT8*)(&(t_start_time)),&t_dtg);               //Get start time
             t_start_time = x_dt_dtg_to_sec(&t_dtg);   
             
             buffer_parse_data(&t_temp_buffer_parser, 8  ,  & (aui1_temp_bcd[0])                             ); 
             buffer_parse_data(&t_temp_buffer_parser, 8  ,  & (aui1_temp_bcd[1])                             ); 
             buffer_parse_data(&t_temp_buffer_parser, 8  ,  & (aui1_temp_bcd[2])                             );          
             x_dt_bcd_to_sec( (UINT8*) (& (aui1_bcd[0])) ,&t_duration );

             if(t_start_time + t_duration < t_current_time - EPG_DURATION_BETWEEN_WINDOW_START_TIME_AND_CURRENT_TIME)
             {
                // in asu mode ,we do not  need to cache the expired events,so skip this one 
                 buffer_parse_data(&t_temp_buffer_parser, 4  , NULL     ); 
                 buffer_parse_data(&t_temp_buffer_parser, 12 , &(ui2_descriptor_len)                        );

                 buffer_parse_data(&t_buffer_parser, (96+ui2_descriptor_len)  , NULL     );        //96 is event header
                 
                 if (t_buffer_parser.ui4_token <  (ui4_section_len -4//CRC code))                      //Have more event
                 {
                     goto START_SINGLE_EVENT;
                 }
                 else
                 {
                     edb_pool_unlock(); 
                     return EDBR_OK;
                 }
             }
         }
    }
    */
    pt_edb_event_info =  EDB_PARTITION_ALLOC(sizeof(EDB_POOL_EVENT_INFO_T));
    if (pt_edb_event_info == NULL)
    {
        x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
        edb_pool_unlock(); 
        return EDBR_OUT_OF_MEM;
    }
    x_memset(pt_edb_event_info ,0x0,sizeof(EDB_POOL_EVENT_INFO_T));
    //hash_create(100, &(pt_edb_event_info->h_garbage));
    //hash_add_obj(pt_edb_event_info->h_garbage,addr, sizeof(VOID*),pv_obj,NULL);

    buffer_parse_data(&t_buffer_parser, 16  , & (pt_edb_event_info->ui2_event_id)         ); EDB_EVENT_SET_MASK(EDB_EVENT_GOT_EVENT_ID);//event_id 16 uimsbf
    buffer_parse_data(&t_buffer_parser, 40  , & (pt_edb_event_info->t_start_time)         ); EDB_EVENT_SET_MASK(EDB_EVENT_GOT_START_TIME);//start_time utc 32 bit
    //buffer_parse_data(&t_buffer_parser, 24  , & (pt_edb_event_info->t_duration)           ); 
    buffer_parse_data(&t_buffer_parser, 8  ,  & (aui1_bcd[0])                             ); 
    buffer_parse_data(&t_buffer_parser, 8  ,  & (aui1_bcd[1])                             ); 
    buffer_parse_data(&t_buffer_parser, 8  ,  & (aui1_bcd[2])                             ); 

    EDB_EVENT_SET_MASK(EDB_EVENT_GOT_DURATION);//duration 24 uimsbf

    x_dt_mjd_bcd_to_dtg( (UINT8*)(&(pt_edb_event_info->t_start_time)),&t_dtg);               //Get start time
    pt_edb_event_info->t_start_time = x_dt_dtg_to_sec(&t_dtg);                               //Get start time

    x_dt_bcd_to_sec( (UINT8*) (& (aui1_bcd[0])) ,&t_duration );
    pt_edb_event_info->t_duration = t_duration;                                              //Get duration

    pt_edb_event_info->ui1_sect_num = ui1_sect_num;/*section number for this event*/
    pt_edb_event_info->ui1_table_id = ui1_table_id;/*table id for this event*/


    buffer_parse_data(&t_buffer_parser, 3  , NULL                                         ); //running_status 3 uimsbf
    buffer_parse_data(&t_buffer_parser, 1  , &(pt_edb_event_info->b_free_ca_mode)         ); EDB_EVENT_SET_MASK(EDB_EVENT_GOT_FREE_CA_MODE); //running_status 3 uimsbf
    buffer_parse_data(&t_buffer_parser, 12 , &(ui2_descriptor_len)                        ); //descriptors_loop_length 12 uimsbf
#ifdef  EDB_INTERNAL_MONITOR_EVENT
    if (ui2_monitor_event_id == pt_edb_event_info->ui2_event_id)
    {
        x_dbg_stmt(_EDB_INFO"onid[0x%04x] tsid[0x%04x] svcid[0x%04x] ui2_event_id=0x%02x  duration=%lld b_free_ca_mode =%d ui2_descriptor_len=0x%02x\r\n",
                  t_locate.ui2_on_id,t_locate.ui2_ts_id,t_locate.ui2_svc_id,
                  pt_edb_event_info->ui2_event_id,
                  pt_edb_event_info->t_duration,
                  pt_edb_event_info->b_free_ca_mode,
                  ui2_descriptor_len
                 );
        edb_dvb_print_local_time("start time:",pt_edb_event_info->t_start_time);
    }
#endif
    /* EDB memory Full*/
    if ( pt_eng_data->t_edb_eng_state.b_asu_mode == FALSE &&
         EDB_MAX_MEMORY_SIZE - edb_get_mem() < EDB_MEMORY_THRESHOLD )
    {
        BOOL                    b_need_process  = FALSE;
        EDB_EVENT_POOL_CH_T *   pt_ch_obj       = NULL;
        EDB_CACHE_ACTIVE_WIN_T* pt_win          = NULL;
        UINT32                  i               = 0;
        TIME_T                  t_start_time    = NULL_TIME;
        TIME_T                  t_end_time      = NULL_TIME;
        DVB_SI_ON_TS_SVC_ID_SEC_INFO_T      t_sec_info = {0};
        TM_COND_T                           e_cond;
        EDB_CH_ID_T*                        pt_active_window_channel = NULL;
        EDB_EVENT_POOL_CH_VER*              pt_ch_ver = NULL;
        EDB_EVENT_POOL_CH_T*                pt_ch_obj_smallest_time_stamp = NULL;

        DBG_INFO((_EDB_INFO"EDB Current  memory=%d(%dK) Max EDB memory=%d\r\n ",
                  edb_get_mem(),(UINT32)(edb_get_mem()/1024),EDB_MAX_MEMORY_SIZE
                 ));

        t_sec_info.ui2_on_id = t_locate.ui2_on_id;
        t_sec_info.ui2_ts_id = t_locate.ui2_ts_id;
        t_sec_info.ui2_svc_id = t_locate.ui2_svc_id;
        t_sec_info.ui1_tbl_id = ui1_table_id;
        t_sec_info.ui1_sec_num = ui1_sect_num;    

        //check is 0x4F/0x4F
        if (ui1_table_id == (UINT8)0x4E || ui1_table_id == (UINT8)0x4E) 
        {
            b_need_process = TRUE;
        }


        //check is active channel
        if (b_need_process ==FALSE && pt_eng_data->pt_edb != NULL)//
        {
            i4_ret = edb_pool_get_ch_obj(
                    &pt_eng_data->pt_edb->t_edb_event_pool, 
                    &t_locate, 
                    &pt_ch_obj);
            if (i4_ret == EDBR_OK && pt_ch_obj != NULL)
            {
                if (pt_ch_obj->ui4_channel_id == pt_eng_data->t_edb_eng_state.ui4_current_channel_id)
                {
                    b_need_process = TRUE;
                }
            }
        }


        //Check in active window
        pt_win = &(pt_eng_data->t_edb_eng_state.t_active_win);
        if (b_need_process == FALSE && pt_win->ui1_num_channel == (UINT8)0x0)
        {

        }
        else
        {
            i4_ret = edb_pool_get_ch_obj(pt_edb_event_pool, &t_locate, &pt_channel);
            for(i=0;i<pt_win->ui1_num_channel && i<EDB_MAX_NUM_ACTIVE_CHANNEL;i++)
            {
                pt_active_window_channel = &( pt_win->at_channel_id[i]);
                if (pt_active_window_channel->ui4_channel_id == pt_channel->ui4_channel_id)
                {
                    pt_win = &(pt_eng_data->t_edb_eng_state.t_active_win);
                    t_start_time = pt_win->t_start_time;
                    t_end_time   = t_start_time + pt_win->t_duration;
                    if (pt_edb_event_info->t_start_time >= t_start_time &&
                        pt_edb_event_info->t_start_time <= t_end_time)
                    {
                        b_need_process = TRUE;
                    }
                    break;            
                }
            }
        }


        if (b_need_process == FALSE)
        {
            x_dbg_stmt(_EDB_INFO"[x_dvb_si_set_eit_section_mask_clear]Ingore this event,not PF,not active channel,not in active window\r\n");
            edb_pool_free_single_event_obj(edb_dvb_free_single_event_obj,pt_edb_event_info);
            i4_ret = x_dvb_si_set_eit_section_mask_clear(h_eit, &t_sec_info, &e_cond);
            if (pui1_sec_mask != NULL)
            {
                (*pui1_sec_mask) &= (~ui1_mask);                //Clear section mask
            }
            
            if (i4_ret != EDBR_OK)
            {
                x_dbg_stmt(_EDB_INFO"Clear EIT filter fail\r\n");
            }
            edb_pool_unlock();    
            
            return i4_ret;
        }

        //Pool clean memory
        x_dbg_stmt(_EDB_INFO"Begin free memory\r\n");

        do 
        {
            //Look for the channel with smallest time stamp
            i4_ret = edb_pool_look_ch_obj_with_smallest_time_stamp(/*pt_eng_data*/pt_edb_event_pool,&pt_ch_obj_smallest_time_stamp);
            if (i4_ret == EDBR_OK && pt_ch_obj_smallest_time_stamp != NULL)
            {
                pt_ch_ver = (EDB_EVENT_POOL_CH_VER*)pt_ch_obj_smallest_time_stamp->pv_data;
                if(pt_ch_ver != NULL)
                {
                    t_sec_info.b_all_sections = TRUE;
                    x_dbg_stmt(_EDB_INFO"x_dvb_si_set_eit_section_mask_clear all_sections\r\n");
                    i4_ret = x_dvb_si_set_eit_section_mask_clear(h_eit,&t_sec_info,&e_cond);
                    if (i4_ret != EDBR_OK)
                    {
                        x_dbg_stmt(_EDB_INFO"Clear EIT filter fail\r\n");
                    }
                    if (pui1_sec_mask != NULL)
                    {
                        (*pui1_sec_mask) &= (~ui1_mask);                //Clear section mask
                    }
                }

                {
                    HANDLE_T                    h_event_hash            = NULL_HANDLE;
                    EDB_EVENT_POOL_CH_VER*      pt_event_pool_ch_ver    = NULL;
                    
                    /*Delete all events attach this channel*/
                    h_event_hash = pt_ch_obj_smallest_time_stamp->h_event_hash;
                    if (h_event_hash != NULL_HANDLE)
                    {
                        if (pt_ch_obj_smallest_time_stamp->pf_free_event == NULL)
                        {
                            DBG_ABORT( DBG_MOD_EDB );
                        }
                        edb_hash_delete(h_event_hash, pt_ch_obj_smallest_time_stamp->pf_free_event);
                        pt_ch_obj_smallest_time_stamp->h_event_hash = NULL_HANDLE;
                        pt_ch_obj_smallest_time_stamp->pt_present_event = NULL;
                        pt_ch_obj_smallest_time_stamp->pt_following_event = NULL;
                        
                        //re create event hash table
                        i4_ret = edb_hash_create(EDB_POOL_CHANNEL_HASH_EVENT_TABLE_SIZE,&(pt_ch_obj_smallest_time_stamp->h_event_hash));


                        //reset ch verion mask
                        pt_event_pool_ch_ver = (EDB_EVENT_POOL_CH_VER*)pt_ch_obj_smallest_time_stamp->pv_data;
                        if (pt_event_pool_ch_ver != NULL)
                        {
                            x_memset(pt_event_pool_ch_ver,0x0,sizeof(EDB_EVENT_POOL_CH_VER));
                            for(i=0 ; i<2; i++)
                            {
                                pt_event_pool_ch_ver->t_pf_tbl[i].ui1_version = 0xFF;
                            }
                            for(i=0 ; i<EDB_MAX_PER_CH_TBL; i++)
                            {
                                pt_event_pool_ch_ver->t_s_act_tbl[i].ui1_version = 0xFF;
                                pt_event_pool_ch_ver->t_s_oth_tbl[i].ui1_version = 0xFF;
                            }
                        }
                    }
                    pt_ch_obj_smallest_time_stamp->ui4_time_stamp = x_os_get_sys_tick();

                }
                #if 0
                i4_ret = edb_pool_del_ch_obj(pt_edb_event_pool,pt_ch_obj_smallest_time_stamp);
                if (i4_ret == EDBR_OK)
                {
                    x_dbg_stmt(_EDB_INFO"Delete channel from pool OK,the time stamp:%d\r\n",
                               pt_ch_obj_smallest_time_stamp->ui4_time_stamp);
                }
                #endif
            }
            else    //Can not found channel with smallest time stamp
            {
                DBG_ABORT( DBG_MOD_EDB );
            }
        }while(EDB_MAX_MEMORY_SIZE - edb_get_mem() < (EDB_MEMORY_THRESHOLD*2) );

    }
/*
#ifdef MW_EPG_CACHE_NVM_SUPPORT 
    if ( pt_eng_data->t_edb_eng_state.b_asu_mode == TRUE &&
         EDB_MAX_MEMORY_SIZE_FOR_FLASH_POOL - edb_get_mem() < EDB_MEMORY_THRESHOLD_FOR_SINGLE_EVNET )
    {
        EDB_EVENT_POOL_CH_T*                pt_ch_obj_smallest_time_stamp = NULL;
        EDB_POOL_EVENT_INFO_T*              pt_event_obj_farthest = NULL;
        SIZE_T                              t_size = 0;


        t_size = edb_get_mem();
        x_dbg_stmt("\n");
        x_dbg_stmt("{EDB} ________________before delete event used memory = %d \n",t_size);
        do
        {
            //Look for the channel with smallest time stamp
            i4_ret = edb_pool_look_ch_obj_with_smallest_time_stamp( pt_edb_event_pool,&pt_ch_obj_smallest_time_stamp);

            if (i4_ret == EDBR_OK && pt_ch_obj_smallest_time_stamp != NULL)
            {
                x_dbg_stmt("find a smallest time stamp channel OK! \n");
#ifdef CLI_SUPPORT                       
                        x_dbg_stmt(_EDB_INFO"this channle is [%s]  \n",pt_ch_obj_smallest_time_stamp->ac_name);
#endif
                i4_ret = edb_pool_look_event_obj_with_farthest_start_time(pt_ch_obj_smallest_time_stamp,&pt_event_obj_farthest);
                if (i4_ret == EDBR_OK && pt_event_obj_farthest != NULL)
                {
                    x_dbg_stmt("find a fathest event OK! \n");
                    x_dbg_stmt(_EDB_INFO" find a farthest event [0x%x]    ",pt_event_obj_farthest->ui2_event_id);
                    if(pt_event_obj_farthest->ui1_table_id != 0x4E)
                    {  
                        x_dbg_stmt("NOT PF! will delete it\n");
                    }
                    else 
                    {
                        x_dbg_stmt("It is PF! also will delete it !\n");
                    }

                    i4_ret = edb_pool_del_event_obj(pt_ch_obj_smallest_time_stamp,pt_event_obj_farthest);
                    if(i4_ret == EDBR_OK)
                    {
                        x_dbg_stmt("routine OK! \n");
                    }
                    else
                    {
                        x_dbg_stmt("routine FAIL! \n");
                    }
                    //x_dbg_stmt(_EDB_INFO"delete a farthest event [0x%x]  \n",pt_event_obj_farthest->ui2_event_id);

                    edb_dvb_print_local_time("deleted event start time:",pt_event_obj_farthest->t_start_time);

                }
                else 
                {
                    x_dbg_stmt("find a fathest event FAIL! \n");                
                }
                pt_ch_obj_smallest_time_stamp->ui4_time_stamp = x_os_get_sys_tick();

            }
            else 
            {
                x_dbg_stmt("find a smallest time stamp channel FAIL! \n");
            }
            t_size = edb_get_mem();
            x_dbg_stmt("{EDB} used memory = %d \n",t_size);
        }while(EDB_MAX_MEMORY_SIZE_FOR_FLASH_POOL - edb_get_mem() < EDB_MEMORY_THRESHOLD_FOR_SINGLE_EVNET );

        t_size = edb_get_mem();
        x_dbg_stmt("{EDB} ________________after delete event used memory = %d \n \n",t_size);
        
    }
#endif
*/
    ui4_descriptot_start_token = t_buffer_parser.ui4_token;                                  /*Current token*/
    ui1_descriptor_count       = 0;
    x_memset((VOID*) (&t_descript_items),0x0,sizeof(DESCRIPTOR_ITEM) *EDB_EVENT_MAX_DES_SIZE); 
    ui4_private_data_specifier = 0;

START_SINGLE_DESCRIPTOR:
    buffer_parse_data(&t_buffer_parser, 8 , &(ui1_descriptor_tag)                         ); //descriptor_tag 8 uimsbf
    buffer_parse_data(&t_buffer_parser, 8 , &(ui1_descriptor_length)                      ); //descriptor_length 8 uimsbf
    if (ui1_descriptor_count >= EDB_EVENT_MAX_DES_SIZE){
        edb_pool_free_single_event_obj(edb_dvb_free_single_event_obj,pt_edb_event_info);
        edb_pool_unlock();    
        return i4_ret;
    }

    if(ui1_descriptor_tag == DVB_DESCR_TAG_PRIVATE_DATA)
    {        
        t_descript_items[ui1_descriptor_count].ui1_tag = ui1_descriptor_tag;
        t_descript_items[ui1_descriptor_count].ui1_len = ui1_descriptor_length;
        t_descript_items[ui1_descriptor_count].pui1_start_addr = (t_buffer_parser.pui1_buffer + t_buffer_parser.ui4_token);
        ui1_descriptor_count ++;
        if(ui1_descriptor_length == 4)
        {
            buffer_parse_data(&t_buffer_parser, 4*8 , &ui4_private_data_specifier          );     
        }
        else
        {
            x_dbg_stmt(_EDB_INFO"private data specifier descriptor != 4  \r\n");
            buffer_parse_data(&t_buffer_parser, (UINT32)(ui1_descriptor_length*8) , NULL          ); 
        }
    }
    else 
    {
        if(     (ui1_descriptor_tag < DVB_USER_DIFINE_DESCR_START_TAG_VALUE)  || 
             ( (ui1_descriptor_tag >= DVB_USER_DIFINE_DESCR_START_TAG_VALUE) && 
              (_dvb_check_private_is_valid(ui1_descriptor_tag,ui4_private_data_specifier) == TRUE)
             )
            )
        {
            t_descript_items[ui1_descriptor_count].ui1_tag = ui1_descriptor_tag;
            t_descript_items[ui1_descriptor_count].ui1_len = ui1_descriptor_length;
            t_descript_items[ui1_descriptor_count].pui1_start_addr = (t_buffer_parser.pui1_buffer + t_buffer_parser.ui4_token);
#ifdef EDB_INTERNAL_MONITOR_EVENT            
            if (ui2_monitor_event_id == pt_edb_event_info->ui2_event_id)
            {
                x_dbg_stmt("ID = 0x%04x Descriptor tag=0x%02x Descriptor Len=%04x start addr=0x%08x\r\n",
                          ui2_monitor_event_id,
                          t_descript_items[ui1_descriptor_count].ui1_tag,
                          t_descript_items[ui1_descriptor_count].ui1_len,
                          t_descript_items[ui1_descriptor_count].pui1_start_addr);
            }
#endif
            ui1_descriptor_count ++;
        }
        
        buffer_parse_data(&t_buffer_parser, (UINT32)(ui1_descriptor_length*8) , NULL          ); 
    }
    if ( (t_buffer_parser.ui4_token - ui4_descriptot_start_token) < ui2_descriptor_len )   //Have more descriptor
    {
        goto START_SINGLE_DESCRIPTOR;
    }


    /*Here all descriptor infomation have been save to t_descript_items*/
    /*Do descriptor parse for event element infomation*/
    //Process DVB_DESCR_TAG_LINKAGE                   ((UINT8)    0x4A)  OK 
    //Process DVB_DESCR_TAG_SHORT_EVENT               ((UINT8)    0x4D)  OK
    //Process DVB_DESCR_TAG_EXTENDED_EVENT            ((UINT8)    0x4E)  OK
    //Process DVB_DESCR_TAG_TIME_SHIFTED_EVENT        ((UINT8)    0x4F)  OK
    //Process DVB_DESCR_TAG_COMPONENT                 ((UINT8)    0x50)  component_descriptor       0x400 4001
    //Process DVB_DESCR_TAG_CA_IDENTIFIER             ((UINT8)    0x53)  CA_identifier_descriptor
    //Process DVB_DESCR_TAG_CONTENT                   ((UINT8)    0x54)  content_descriptor
    //process DVB_DESCR_TAG_PRIVATE_DATA              ((UINT8)    0x5F)  private data descript
    //Process DVB_DESCR_TAG_PARENTAL_RATING           ((UINT8)    0x55)  parental_rating_descriptor

#if 1
    i4_ret = _dvb_process_linkage(DVB_DESCR_TAG_LINKAGE,pt_edb_event_info); //Process DVB_DESCR_TAG_LINKAGE ((UINT8)    0x4A)   
    if (i4_ret == EDBR_OK)
    {EDB_EVENT_SET_MASK(EDB_EVENT_GOT_LINKAGE);}

    i4_ret = _dvb_pre_process_short_extended(&(pt_eng_data->t_edb_eng_state.t_pref_lang),
                                              pt_eng_data->t_edb_eng_state.t_country,
                                              pt_eng_data->ui2_svl_id
                                            );

    if(ui1_total_lang_count >= 1)
    {
        x_memcpy( &(pt_edb_event_info->t_detail_lang),&(at_lang_event_detail[0].t_lang),3);
    }
    
    for(i4_lang_index = 0;i4_lang_index<ui1_total_lang_count;i4_lang_index++)
    {
        x_memset(&t_edb_pre_lang,0x0,sizeof(EDB_PREF_LANG_T));
        t_edb_pre_lang.ui1_num = 1;
        x_memcpy( &(t_edb_pre_lang.t_lang[0]),at_lang_event_detail[i4_lang_index].t_lang,3);

        i4_ret = _dvb_process_short_event(                                      //Process DVB_DESCR_TAG_SHORT_EVENT ((UINT8)    0x4D)
                DVB_DESCR_TAG_SHORT_EVENT,pt_edb_event_info,
                &(t_edb_pre_lang),
                pt_eng_data->t_edb_eng_state.t_country,
                pt_eng_data->t_edb_eng_state.t_operator,
                &(at_lang_event_detail[i4_lang_index]),
                pt_eng_data->ui2_svl_id); 
        /*short event desc event detail save is s_event_detail_buffer*/
        if (i4_ret == EDBR_OK)
        {EDB_EVENT_SET_MASK(EDB_EVENT_GOT_EVENT_TITLE);EDB_EVENT_SET_MASK(EDB_EVENT_GOT_EVENT_DETAIL);}


        x_memset(&t_edb_pre_lang,0x0,sizeof(EDB_PREF_LANG_T));
        t_edb_pre_lang.ui1_num = 1;
        x_memcpy( &(t_edb_pre_lang.t_lang[0]),at_lang_event_detail[i4_lang_index].t_lang,3);
        i4_ret = _dvb_process_extended_event(                                   //Process DVB_DESCR_TAG_EXTENDED_EVENT ((UINT8)    0x4E)
                DVB_DESCR_TAG_EXTENDED_EVENT,pt_edb_event_info,
                &(t_edb_pre_lang),
                pt_eng_data->t_edb_eng_state.t_country,
                pt_eng_data->t_edb_eng_state.s_event_detail_separator,
                pt_eng_data->t_edb_eng_state.t_operator,
                &(at_lang_event_detail[i4_lang_index]),
                pt_eng_data->ui2_svl_id); 

        if (i4_ret == EDBR_OK)
        {EDB_EVENT_SET_MASK(EDB_EVENT_GOT_EVENT_EXTENDED);}
    }

    i4_ret = _dvb_after_process_short_extended(pt_edb_event_info,
                                               pt_eng_data->t_edb_eng_state.s_event_detail_separator);


    if ((x_strncmp(pt_eng_data->t_edb_eng_state.t_country,"GBR",3) == 0) ||
        (pt_eng_data->ui2_svl_id == EDB_SVL_ID_DVB_T))
    {
        i4_ret = _dvb_process_guidance(                                    //Process DVB_DESCR_TAG_EXTENDED_EVENT ((UINT8)    0x4E)
                    DVB_USER_DTT_DESCR_TAG_GUIDANCE,pt_edb_event_info,
                    &(pt_eng_data->t_edb_eng_state.t_pref_lang),
                    pt_eng_data->t_edb_eng_state.t_country,
                    pt_eng_data->t_edb_eng_state.t_operator,
                    pt_eng_data->ui2_svl_id); 

        if (i4_ret == EDBR_OK)
        {EDB_EVENT_SET_MASK(EDB_EVENT_GOT_GUIDANCE);}
    }

    i4_ret = _dvb_process_time_shift(                                       //Process DVB_DESCR_TAG_TIME_SHIFTED_EVENT        ((UINT8)    0x4F)
            DVB_DESCR_TAG_TIME_SHIFTED_EVENT,pt_edb_event_info); 
    if (i4_ret == EDBR_OK)
    {EDB_EVENT_SET_MASK(EDB_EVENT_GOT_TIME_SHIFT);}


    i4_ret = _dvb_process_component(                                         //Process Process DVB_DESCR_TAG_COMPONENT                 ((UINT8)    0x50)
            DVB_DESCR_TAG_COMPONENT,pt_edb_event_info,
            &(pt_eng_data->t_edb_eng_state.t_pref_lang),
            pt_eng_data->t_edb_eng_state.t_country); 
    if (i4_ret == EDBR_OK)
    {EDB_EVENT_SET_MASK(EDB_EVENT_GOT_COMPONENT);}


    i4_ret = _dvb_process_ca_identifier(                                     //Process DVB_DESCR_TAG_CA_IDENTIFIER             ((UINT8)    0x53)  CA_identifier_descriptor
            DVB_DESCR_TAG_CA_IDENTIFIER,pt_edb_event_info); 
    if (i4_ret == EDBR_OK)
    {EDB_EVENT_SET_MASK(EDB_EVENT_GOT_CA_IDENTIFIER);}


    i4_ret = _dvb_process_content(                                          //Process DVB_DESCR_TAG_CONTENT                   ((UINT8)    0x54)  content_descriptor
            DVB_DESCR_TAG_CONTENT,pt_edb_event_info); 
    if (i4_ret == EDBR_OK)
    {EDB_EVENT_SET_MASK(EDB_EVENT_GOT_CONTENT);}


    i4_ret = _dvb_process_rating(                                          //Process DVB_DESCR_TAG_PARENTAL_RATING           ((UINT8)    0x55)  parental_rating_descriptor
            DVB_DESCR_TAG_PARENTAL_RATING,pt_edb_event_info); 
    if (i4_ret == EDBR_OK)
    {EDB_EVENT_SET_MASK(EDB_EVENT_GOT_RATING);}

    i4_ret = _dvb_process_private(                                          //Process DVB_DESCR_TAG_PRIVATE_DATA   ((UINT8)    0x55)  parental_rating_descriptor
            DVB_DESCR_TAG_PRIVATE_DATA,pt_edb_event_info); 



#endif


    pt_edb_event_info->ui2_checksum = pt_edb_event_info->ui2_event_id;//TODO

    /*Add this  event to channel pool*/

    i4_ret = edb_pool_get_ch_obj(pt_edb_event_pool, &t_locate, &pt_channel);
    
    if (i4_ret == EDBR_OK)
    {
        if (ui1_table_id == (UINT8)0x4E || ui1_table_id == (UINT8)0x4F)//PF event section
        {
            TIME_T                  t_current_time        = NULL_TIME;
            DT_COND_T               t_dt_cond             = DT_NOT_RUNNING;
            t_current_time = x_dt_get_brdcst_utc(NULL,&t_dt_cond);
            
            if (ui1_sect_num == (UINT8)0x0)//present event
            {                
                if(
                      (t_dt_cond != DT_SYNC_RUNNING)  ||
                    ((pt_edb_event_info->t_start_time <= t_current_time) && 
                     (t_current_time < (pt_edb_event_info->t_start_time + pt_edb_event_info->t_duration)))
                  )
                {
                    if (
                            (pt_channel->pt_present_event != NULL) && 
                            (pt_channel->pt_present_event->ui2_event_id == pt_edb_event_info->ui2_event_id)
                        )
                    {
                        i4_ret = edb_pool_del_present_event_obj(pt_channel);
                        if (i4_ret != EDBR_OK)
                        {
                            DBG_ABORT( DBG_MOD_EDB );
                        }
                    }
                    if( /*delete old pf event only when old present overlap new present,oherwise edb will keep old prsent event
                                                so that epg can show it*/
                        (pt_channel->pt_present_event != NULL) &&
                        (pt_channel->pt_present_event->t_start_time + pt_channel->pt_present_event->t_duration > pt_edb_event_info->t_start_time)
                       )
                    {
                        _edb_dvb_delete_old_secion_event(pt_channel,ui1_table_id,ui1_sect_num); 
                    }
                    i4_ret = edb_pool_add_present_event_obj(pt_channel,pt_edb_event_info);//Only a refrence point to event of hash 
                    DBG_INFO((_EDB_INFO"Add Present event, channel id=0x%08x event id=0x%04x\r\n",
                              pt_channel->ui4_channel_id,pt_edb_event_info->ui2_event_id
                             ));
                    edb_dvb_print_local_time("start time:",pt_edb_event_info->t_start_time);
                    _dvb_check_pf_and_nfy(pt_eng_data,pt_channel,pt_edb_event_info,EDB_REASON_NOW_EVENT_UPDATED);
                }
                else
                {
                    i4_ret = edb_pool_add_event_obj(pt_channel,pt_edb_event_info);
                }
                
                //DBG_INFO((_EDB_INFO"Add present event\r\n"));
            }
            else//Follow event
            {
                if(pt_channel->pt_present_event != NULL)
                {
                    t_current_time = pt_channel->pt_present_event->t_start_time ; 
                }
                if(    (t_dt_cond != DT_SYNC_RUNNING) ||
                    (t_current_time < pt_edb_event_info->t_start_time)
                )
                {
                    if (
                            (pt_channel->pt_following_event != NULL) &&
                            (pt_channel->pt_following_event->ui2_event_id == pt_edb_event_info->ui2_event_id)
                        )
                    {
                        i4_ret = edb_pool_del_following_event_obj(pt_channel);
                        if (i4_ret != EDBR_OK)
                        {
                            DBG_ABORT( DBG_MOD_EDB );
                        }
                    }
                    _edb_dvb_delete_old_secion_event(pt_channel,ui1_table_id,ui1_sect_num); 
                    i4_ret = edb_pool_add_following_event_obj(pt_channel,pt_edb_event_info);
                    DBG_INFO((_EDB_INFO"Add Following event, channel id=0x%08x event id=0x%04x\r\n",
                              pt_channel->ui4_channel_id,pt_edb_event_info->ui2_event_id
                             ));
                    edb_dvb_print_local_time("start time:",pt_edb_event_info->t_start_time);
                    _dvb_check_pf_and_nfy(pt_eng_data,pt_channel,pt_edb_event_info,EDB_REASON_NEX_EVENT_UPDATED);
                    //DBG_INFO((_EDB_INFO"Add Follow event\r\n"));
                }
                else
                {
                    i4_ret = edb_pool_add_event_obj(pt_channel,pt_edb_event_info);
                }
                    
            }
        }
        else                                                            /*From schedule event section*/
        {
            if (pt_channel->pt_present_event != NULL && 
                pt_edb_event_info->ui2_event_id == pt_channel->pt_present_event->ui2_event_id)//is present event, ignore
            {
                edb_pool_free_single_event_obj(edb_dvb_free_single_event_obj,pt_edb_event_info);
                i4_ret = EDBR_ALREADY_EXIST;
            }
            else if(pt_channel->pt_following_event != NULL && 
                    pt_edb_event_info->ui2_event_id == pt_channel->pt_following_event->ui2_event_id)//is followevent, ignore
            {
                edb_pool_free_single_event_obj(edb_dvb_free_single_event_obj,pt_edb_event_info);
                i4_ret = EDBR_ALREADY_EXIST;
            }
            else
            {
                i4_ret = edb_pool_add_event_obj(pt_channel,pt_edb_event_info);  //Put to hash table
                //Need Check PF notify TODO
            }
        }


        //Check is in active window
        if (i4_ret == EDBR_OK)
        {
            if (b_notified == FALSE)
            {
                b_notified =  _dvb_check_active_window_and_nfy(pt_eng_data,pt_channel,pt_edb_event_info);
            }
        }
    }
    else
    {
        x_dbg_stmt(_EDB_INFO"Can not find channel info\r\n");
        edb_pool_free_single_event_obj(edb_dvb_free_single_event_obj,pt_edb_event_info);
        edb_pool_unlock();    
        return i4_ret;
    }


    if (t_buffer_parser.ui4_token <  (ui4_section_len -4/*CRC code*/))                      //Have more event
    {
        goto START_SINGLE_EVENT;
    }

    edb_pool_unlock(); 


    if (b_notified == TRUE && pt_eng_data != NULL && pt_channel != NULL)
    {
        /*
        ui4_data1 is ui2_svl_id of current svl                 
        ui4_data2 is ui4_channel_id of updated service.
        */
        edb_handle_notify(EDB_REASON_EVENT_IN_ACTIVE_WIN_UPDATED, pt_eng_data->ui2_svl_id,pt_channel->ui4_channel_id);
    }

#ifdef MEASURE_OVERHEAD
    ui4_section_raw_memory_used += ui4_section_len;
#endif    
    return i4_ret;

}

static INT32 _edb_dvb_get_descriptor_count(
        UINT8   ui1_tag,
        UINT8*  pui1_count)
{
    INT32   i         = 0;
    INT32   ui1_index = 0;
    INT32   i4_ret    = EDBR_INFO_NOT_FOUND;

    *pui1_count = 0;
    x_memset((VOID*) (&(aui2_descript_position[0])),0x0,sizeof(UINT8) * EDB_EVENT_MAX_DES_SIZE);//Cleanr position info
    for(i=0;i<EDB_EVENT_MAX_DES_SIZE;i++)
    {
        if (t_descript_items[i].ui1_tag == (UINT8)0x0)
        {
            break;
        }
        if (t_descript_items[i].ui1_tag == ui1_tag)
        {
            (*pui1_count) = (UINT8) ((*pui1_count) +1);
            aui2_descript_position[ui1_index++] = (UINT8)(i);
            i4_ret = EDBR_OK;
        }

    }

    return i4_ret;
}






static INT32 _dvb_parse_event_linkage (
        UINT8                   ui1_position,
        EDB_EVENT_LINKAGE_T*    pt_event_linkage)
{
    UINT8                           ui1_linkage_type        = 0;
    UINT16                          ui2_transport_stream_id = 0;
    UINT16                          ui2_original_network_id = 0;
    UINT16                          ui2_service_id          = 0;
    UINT16                          ui2_target_event_id     = 0;
    UINT8                           ui1_target_listed       = 0;
    UINT8                           ui1_event_simulcast     = 0;
    UINT8                           ui1_reserved            = 0;
    UINT8*                          pui1_obj_data           = NULL;


    EDB_CHECK_RET( 
            (pt_event_linkage == NULL || ui1_position >= EDB_EVENT_MAX_DES_SIZE),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    pui1_obj_data = t_descript_items[ui1_position].pui1_start_addr;

/*
    if (pui1_obj_data == NULL ||
        pui1_obj_data[0] != DVB_DESCR_TAG_LINKAGE)
    {
        return EDBR_PARSE_ERROR;
    }


    z_descr_len = pui1_obj_data[1];

    if(z_descr_len < 7 )
    {
        return EDBR_PARSE_ERROR;
    }
*/

    ui2_transport_stream_id = GET_UINT16_FROM_PTR_BIG_END(pui1_obj_data +0);
    ui2_original_network_id = GET_UINT16_FROM_PTR_BIG_END(pui1_obj_data +2);
    ui2_service_id          = GET_UINT16_FROM_PTR_BIG_END(pui1_obj_data +4);

    ui1_linkage_type = *(pui1_obj_data +6);

    /*A linkage_type with value 0x0D is only valid when the descriptor is carried in the EIT.*/
    if (ui1_linkage_type != (UINT8)0x0D)
    {
        return EDBR_PARSE_ERROR;
    }

    if(t_descript_items[ui1_position].ui1_len > 12 )
    {
        ui2_target_event_id  = GET_UINT16_FROM_PTR_BIG_END(pui1_obj_data +7);
        ui1_target_listed    = (UINT8) ( (*(pui1_obj_data +9))&(0x80) );/*Bin 10000000*/
        ui1_event_simulcast  = (UINT8) ( (*(pui1_obj_data +9))&(0x40) );/*Bin 01000000*/
        ui1_reserved         = (UINT8) ( (*(pui1_obj_data +9))&(0x3F) );/*Bin 00111111*/
    }


    if (pt_event_linkage != NULL)
    {
        pt_event_linkage->ui2_on_id           = ui2_original_network_id;
        pt_event_linkage->ui2_ts_id           = ui2_transport_stream_id;
        pt_event_linkage->ui2_svc_id          = ui2_service_id;
        pt_event_linkage->ui2_target_event_id = ui2_target_event_id;
        pt_event_linkage->ui1_target_listed   = ui1_target_listed;
        pt_event_linkage->ui1_event_simulcast = ui1_event_simulcast;
        pt_event_linkage->ui1_reserved        = ui1_reserved;
        pt_event_linkage->ui1_linkage_type    = ui1_linkage_type;

        return EDBR_OK;
    }

    return EDBR_OK;
}    

static INT32 _dvb_process_linkage(UINT8 ui1_tag,EDB_POOL_EVENT_INFO_T* pt_edb_event_info)
{
    INT32       i4_ret                           = 0x0;
    UINT8       i                                = 0x0;
    UINT8       ui1_count                        = 0x0;

    EDB_EVENT_LINKAGE_LIST_T*   pt_event_linkage_list = NULL;
    EDB_CHECK_RET( 
            (pt_edb_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    i4_ret = _edb_dvb_get_descriptor_count(ui1_tag,&ui1_count);
    if (i4_ret == EDBR_INFO_NOT_FOUND || ui1_count == 0)
    {
        return i4_ret;
    }


    //Process linkage
    pt_event_linkage_list = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_LINKAGE_LIST_T));
    if (pt_event_linkage_list == NULL)
    {
        x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
        return EDBR_OUT_OF_MEM;
        //DBG_ABORT( DBG_MOD_EDB );
    }
    x_memset(pt_event_linkage_list,0x0,sizeof(EDB_EVENT_LINKAGE_LIST_T));
    pt_edb_event_info->pt_event_linkage_list = pt_event_linkage_list;


    pt_event_linkage_list->ui1_num_event_linkage = ui1_count;
    pt_event_linkage_list->pt_event_linkage = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_LINKAGE_T) * ui1_count);
    if (pt_event_linkage_list->pt_event_linkage == NULL)
    {
        x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
        return EDBR_OUT_OF_MEM;
        //DBG_ABORT( DBG_MOD_EDB );
    }
    x_memset(pt_event_linkage_list->pt_event_linkage,0x0,sizeof(EDB_EVENT_LINKAGE_T) * ui1_count );

    for(i=0;i<ui1_count && i<EDB_EVENT_MAX_DES_SIZE;i++)
    {
        i4_ret = _dvb_parse_event_linkage(aui2_descript_position[i],(pt_event_linkage_list->pt_event_linkage) + i);/*Parse to */
    }



    return i4_ret;    
}

static INT32 _dvb_pre_process_short_extended(EDB_PREF_LANG_T*  pt_pref_lang,
                                                          ISO_3166_COUNT_T  t_country,
                                                          UINT16            ui2_svl_id
                                                         )
{
    INT32               i4_ret           = EDBR_INTERNAL_ERROR;
    UINT8               ui1_count        = 0;
    INT32               i                = 0;
    INT32               j                = 0;
    DESCRIPTOR_ITEM*    pt_descript_item = NULL;
    ISO_639_LANG_T      t_lang           = {0};
    BOOL                fg_found         = FALSE;
    BOOL                b_match          = FALSE;

    x_memset(at_lang_event_detail,0x0,sizeof(at_lang_event_detail));
    ui1_total_lang_count = 0;

    i4_ret = _edb_dvb_get_descriptor_count(DVB_DESCR_TAG_SHORT_EVENT,&ui1_count);
    /*Put all language from short descriptor into at_lang_event_detail */
    for(i=0;i<ui1_count && i<EDB_EVENT_MAX_DES_SIZE;i++)
    {
        pt_descript_item =  &(t_descript_items[aui2_descript_position[i]]);
        if( pt_descript_item->pui1_start_addr != NULL && 
            pt_descript_item->ui1_len >3)
        {
            x_memcpy(at_lang_event_detail[i].t_lang, pt_descript_item->pui1_start_addr, 3);
            ui1_total_lang_count ++;
        }
    }

    /*Put all language from extended descriptor into at_lang_event_detail */
    i4_ret = _edb_dvb_get_descriptor_count(DVB_DESCR_TAG_EXTENDED_EVENT,&ui1_count);
    for(i=0;i<ui1_count && i<EDB_EVENT_MAX_DES_SIZE;i++)
    {
        pt_descript_item =  &(t_descript_items[aui2_descript_position[i]]);
        if( pt_descript_item->pui1_start_addr != NULL && 
            pt_descript_item->ui1_len >3)
        {
            x_memcpy(t_lang,pt_descript_item->pui1_start_addr+1, 3);

            for(j=0;j<ui1_total_lang_count && j<EDB_EVENT_MAX_DES_SIZE;j++)
            {
                if (_dvb_lang_match(&t_lang,&(at_lang_event_detail[j].t_lang)) == TRUE)/*Already exist in queue*/
                {
                    fg_found = TRUE;
                    break;
                }
            }

            if (fg_found == FALSE && ui1_total_lang_count<EDB_EVENT_MAX_DES_SIZE)//TODO
            {
                x_memcpy(at_lang_event_detail[ui1_total_lang_count].t_lang, 
                         pt_descript_item->pui1_start_addr+1, 3);/*skip descriptor_number 4 uimsbf last_descriptor_number*/
                ui1_total_lang_count ++;
            }
        }
    }

    /*Put user pre language into 1st*/
    if (pt_pref_lang != NULL)
    {
        ISO_639_LANG_T      t_bakup_lang = {0};
        INT32               k            = 0;
        for(i=0;i<pt_pref_lang->ui1_num && i<EDB_MAX_NUM_PREF_LANG;i++)/*user setting*/
        {
            for(j=0;j<ui1_total_lang_count && j<EDB_EVENT_MAX_DES_SIZE;j++)        /*TS*/
            {
                if (_dvb_lang_match(&(at_lang_event_detail[j].t_lang), &(pt_pref_lang->t_lang[0]) + i) == TRUE)
                {
                    /*TS Position j is needed*/
                    b_match = TRUE;
            
                    /*j is user pref language*/
                    if (j != 0 )/*Swap j and 0*/
                    {
                        x_memcpy(&t_bakup_lang, &(at_lang_event_detail[j].t_lang), 3);/*Bakup j*/
                        for ( k=(j-1); k>=0; k--)
                        {
                            x_memcpy(&(at_lang_event_detail[k+1].t_lang),&(at_lang_event_detail[k].t_lang), 3);/*Bakup 0*/
                        }
                        x_memcpy(&(at_lang_event_detail[0].t_lang),&(t_bakup_lang), 3);/*Bakup j*/
                    }
                    break;
                }
            }
            if (b_match == TRUE)
            {
                break;
            }
        }
    }

    if (b_match == FALSE)
    {
        UINT8                   ui1_event_name_length         = 0;
        UINT8                   ui1_event_detail_length       = 0;
        UINT8                   ui1_event_itmes_length        = 0;
        BUFFER_PARSER           t_buffer_parser               = {0};
        ISO_639_LANG_T          t_lang_has_text               = {0};
        ISO_639_LANG_T          t_lang_temp                   = {0};  
        BOOL                    b_find_has_text_lang          = FALSE;


        i4_ret = _edb_dvb_get_descriptor_count(DVB_DESCR_TAG_SHORT_EVENT,&ui1_count);
     
        for(i=0;i<ui1_count && i<EDB_EVENT_MAX_DES_SIZE;i++)
        {
            pt_descript_item =  &(t_descript_items[aui2_descript_position[i]]);
            if( pt_descript_item->pui1_start_addr != NULL && 
                pt_descript_item->ui1_len >3)
            {
                x_memset(&t_buffer_parser,0x0,sizeof(BUFFER_PARSER));
                t_buffer_parser.pui1_buffer = (UINT8*)pt_descript_item->pui1_start_addr;   //Event title Raw data start address include language
                buffer_parse_data(&t_buffer_parser, 8, &(t_lang_temp[0])                           ); 
                buffer_parse_data(&t_buffer_parser, 8, &(t_lang_temp[1])                           ); 
                buffer_parse_data(&t_buffer_parser, 8, &(t_lang_temp[2])                           ); 
                /*x_dbg_stmt("{EDB}short event language %s \r\n",t_lang_temp);*/
                buffer_parse_data(&t_buffer_parser, 8,  &ui1_event_name_length          ); //event_name_length
                buffer_parse_data(&t_buffer_parser, (UINT32)(ui1_event_name_length*8), NULL ); //skip event title
                buffer_parse_data(&t_buffer_parser, 8,  &ui1_event_detail_length          );
                if(ui1_event_detail_length > 0)
                {
                    b_find_has_text_lang = TRUE;
                    x_memcpy(&(t_lang_has_text), &(t_lang_temp), 3);
                    /*x_dbg_stmt("{EDB_________}  short has text.idx = %d \r\n",i); */
                    break;
                }
            }
        }

        if(b_find_has_text_lang == FALSE)
        {
            i4_ret = _edb_dvb_get_descriptor_count(DVB_DESCR_TAG_EXTENDED_EVENT,&ui1_count);
            
            for(i=0;i<ui1_count && i<EDB_EVENT_MAX_DES_SIZE;i++)
            {
                pt_descript_item =  &(t_descript_items[aui2_descript_position[i]]);
                if( pt_descript_item->pui1_start_addr != NULL && 
                    pt_descript_item->ui1_len >3)
                { 
                    x_memset(&t_buffer_parser,0x0,sizeof(BUFFER_PARSER));
                    t_buffer_parser.pui1_buffer = (UINT8*)pt_descript_item->pui1_start_addr;   //Event title Raw data start address include language
                    buffer_parse_data(&t_buffer_parser, 8, NULL                           ); //skip x Byte  
                    buffer_parse_data(&t_buffer_parser, 8, &(t_lang_temp[0])                           ); 
                    buffer_parse_data(&t_buffer_parser, 8, &(t_lang_temp[1])                           ); 
                    buffer_parse_data(&t_buffer_parser, 8, &(t_lang_temp[2])                           ); 
                    /*x_dbg_stmt("{EDB_________}extended event language %s \r\n",t_lang_temp);*/
                    buffer_parse_data(&t_buffer_parser, 8,  &ui1_event_itmes_length          ); 
                    //buffer_parse_data(&t_buffer_parser, (UINT32)(ui1_event_itmes_length*8), NULL ); 
                    if(ui1_event_itmes_length > 0) 
                    {
                        b_find_has_text_lang = TRUE;
                        x_memcpy(&(t_lang_has_text), &(t_lang_temp), 3);
                        /*x_dbg_stmt("{EDB_________}  extended has itmes.idx = %d \r\n",i);*/
                        break;
                    }

                    buffer_parse_data(&t_buffer_parser, 8,  &ui1_event_detail_length          ); 
                    if(ui1_event_detail_length > 0) 
                    {
                        b_find_has_text_lang = TRUE;
                        x_memcpy(&(t_lang_has_text), &(t_lang_temp), 3);
                        /*x_dbg_stmt("{EDB_________} extended has text.idx = %d \r\n",i);*/
                        break;
                    }

                }
            }
        }

        if(b_find_has_text_lang == TRUE)
        {
            ISO_639_LANG_T      t_bakup_lang = {0};
            INT32               k            = 0;
            for(j=0;j<ui1_total_lang_count && j<EDB_EVENT_MAX_DES_SIZE;j++)        /*TS*/
            {
                if (_dvb_lang_match(&(at_lang_event_detail[j].t_lang), &(t_lang_has_text)) == TRUE)
                {
                    /*TS Position j is needed*/
            
                    /*j is has text language*/
                    if (j != 0 )/*Swap j and 0*/
                    {
                        x_memcpy(&t_bakup_lang, &(at_lang_event_detail[j].t_lang), 3);/*Bakup j*/
                        for ( k=(j-1); k>=0; k--)
                        {
                            x_memcpy(&(at_lang_event_detail[k+1].t_lang),&(at_lang_event_detail[k].t_lang), 3);/*Bakup 0*/
                        }
                        x_memcpy(&(at_lang_event_detail[0].t_lang),&(t_bakup_lang), 3);/*Bakup j*/
                    }
                    break;
                }
            }
        }
      
    }


    
    if ((t_eng_cfg_flag & EDB_DVB_CONFIG_SHOW_ALL_LANG) &&
        ((x_strncmp(t_country,"GBR",3) != 0) || (ui2_svl_id != EDB_SVL_ID_DVB_T))
        )
    {
    }
    else
    {
        /*Delete other language except user pref language*/
        if (ui1_total_lang_count > (UINT8)0x1)
        {
            for(i=1;i<ui1_total_lang_count && i<EDB_EVENT_MAX_DES_SIZE;i++)
            {
                x_memset(&(at_lang_event_detail[i]),0x0,sizeof(LANG_EVENT_DETAIL));
            }
            ui1_total_lang_count = 1;
        }
    }

    x_memset(s_event_detail_buffer,0x0,EDB_EVENT_DETAIL_BUFFER);
    return i4_ret;
}

static INT32 _dvb_after_process_short_extended(
        EDB_POOL_EVENT_INFO_T*  pt_edb_event_info,
        CHAR* ps_event_detail_separator
        )
{
    INT32               i4_ret                      = EDBR_INTERNAL_ERROR;
    INT32               i                           = 0;
    LANG_EVENT_DETAIL*  pt_lang_event_detail        = NULL;
    CHAR*               ps_event_detail             = NULL;
    CHAR*               ps_event_extended_detail    = NULL;
    
    /* Get all language event detail & event extended detail
     * Put into pt_edb_event_info->ps_event_detail
     * */

    x_memset(s_event_detail_buffer,0x0,EDB_EVENT_DETAIL_BUFFER);
   
    for(i=0;i<ui1_total_lang_count && i<EDB_EVENT_MAX_DES_SIZE;i++)
    {
        pt_lang_event_detail        = &(at_lang_event_detail[i]);
        ps_event_detail             = pt_lang_event_detail->ps_short_text;
        ps_event_extended_detail    = pt_lang_event_detail->ps_extended_text;

        /*append short detail*/
        if(ps_event_detail != NULL)
        {
            x_strcat(s_event_detail_buffer,ps_event_detail);
            EDB_PARTITION_FREE(pt_lang_event_detail->ps_short_text);
            pt_lang_event_detail->ps_short_text = NULL;
        }
        s_event_detail_buffer[EDB_EVENT_DETAIL_BUFFER -1] = '\0';

        /*append sperator between short and extended*/
        if(ps_event_extended_detail != NULL && ps_event_detail_separator != NULL &&
           ps_event_detail != NULL && x_strlen(ps_event_detail)>0)
        {
            x_strcat(s_event_detail_buffer,ps_event_detail_separator);
        }
        s_event_detail_buffer[EDB_EVENT_DETAIL_BUFFER -1] = '\0';
        /*Append event extended detail*/
        if( ps_event_extended_detail != NULL && x_strlen(ps_event_extended_detail) > 0 )
        {
            x_strcat(s_event_detail_buffer,ps_event_extended_detail);
            EDB_PARTITION_FREE(pt_lang_event_detail->ps_extended_text);
            pt_lang_event_detail->ps_extended_text = NULL;
        }
        s_event_detail_buffer[EDB_EVENT_DETAIL_BUFFER -1] = '\0';

        if(
                ( i <= ui1_total_lang_count -1 -1 ) &&
                ( x_strlen(s_event_detail_buffer) > 0) &&
                ( x_strlen(s_event_detail_buffer) + x_strlen((CHAR*)EDB_SEPARATOR_OF_MULT_LANG ) < (EDB_EVENT_DETAIL_BUFFER -1) )
          )
        {
            x_memcpy(
                     (CHAR*)s_event_detail_buffer + x_strlen(s_event_detail_buffer),
                     (CHAR*)EDB_SEPARATOR_OF_MULT_LANG,
                      x_strlen(EDB_SEPARATOR_OF_MULT_LANG)
                    );
            s_event_detail_buffer[EDB_EVENT_DETAIL_BUFFER -1] = '\0';
        }
    }


    /*save to pt_pool_event->ps_event_detail*/
    if (pt_edb_event_info->ps_event_detail != NULL)
    {
        EDB_PARTITION_FREE(pt_edb_event_info->ps_event_detail);
        pt_edb_event_info->ps_event_detail = NULL;
    }

    {
        SIZE_T      _z_detail_len = 0x0;
        s_event_detail_buffer[EDB_EVENT_DETAIL_BUFFER -1] = '\0';
        _z_detail_len = x_strlen(s_event_detail_buffer);
        if (_z_detail_len > 0 && _z_detail_len < EDB_EVENT_DETAIL_BUFFER )
        {
            pt_edb_event_info->ps_event_detail = EDB_PARTITION_ALLOC(_z_detail_len + 1);
            if (pt_edb_event_info->ps_event_detail != NULL)
            {
                x_memset(pt_edb_event_info->ps_event_detail,0x0,_z_detail_len + 1);
                x_memcpy(pt_edb_event_info->ps_event_detail,s_event_detail_buffer,_z_detail_len);
                s_event_detail_buffer[EDB_EVENT_DETAIL_BUFFER -1] = '\0';
            }
        }
    }

    return i4_ret;
}



static INT32 _dvb_process_short_event(
        UINT8 ui1_tag,
        EDB_POOL_EVENT_INFO_T* pt_edb_event_info,
        EDB_PREF_LANG_T*  pt_pref_lang,
        ISO_3166_COUNT_T  t_country,
        UINT32   t_operator,
        LANG_EVENT_DETAIL* pt_lang_event_detail,
        UINT16  ui2_svl_id)
{
    INT32               i4_ret                   = EDBR_INFO_NOT_FOUND;
    UINT8               i                        = 0;
    UINT8               j                        = 0;
    UINT8               ui1_count                = 0;
    BOOL                b_match                  = FALSE;
    ISO_639_LANG_T*     pt_lang                  = &(at_lang[0]);
    DESCRIPTOR_ITEM*    pt_descript_item         = NULL;
    ISO_639_LANG_T      t_lang                   = {0};
    UINT8               ui1_match_position       = 0;
    BUFFER_PARSER       t_buffer_parser          = {0};
    ISO_639_LANG_T      t_lang_match             = {0}; /*Matched language*/


    EDB_CHECK_RET( 
            (pt_edb_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    i4_ret = _edb_dvb_get_descriptor_count(ui1_tag,&ui1_count);
    if (i4_ret == EDBR_INFO_NOT_FOUND || ui1_count == 0)
    {
        return i4_ret;
    }


    x_memset(pt_lang,0x0,sizeof(at_lang));

    /*Get all language from short event descriptors*/
    for(i=0;i<ui1_count && i<EDB_EVENT_MAX_DES_SIZE;i++)
    {
        pt_descript_item =  &(t_descript_items[aui2_descript_position[i]]);
        if( pt_descript_item->pui1_start_addr != NULL && 
            pt_descript_item->ui1_len >3)
        {
            x_memcpy(pt_lang[i], pt_descript_item->pui1_start_addr, 3);
        }
    }

    if (pt_pref_lang != NULL)
    {
        for(i=0;i<pt_pref_lang->ui1_num && i<EDB_MAX_NUM_PREF_LANG;i++)/*user setting*/
        {
            for(j=0;j<ui1_count && j<EDB_EVENT_MAX_DES_SIZE;j++)        /*TS*/
            {
                if (_dvb_lang_match(pt_lang + j, &(pt_pref_lang->t_lang[0]) + i) == TRUE)
                {
                    /*TS Position j is needed*/
                    b_match = TRUE;
                    x_memcpy(&t_lang_match, pt_lang + j, 3);/*Skip descriptor_number 4 uimsbf &  last_descriptor_number 4 uimsbf*/
                    break;
                }
            }
            if (b_match == TRUE)
            {
                break;
            }
        }
    }

    if (b_match == FALSE)//use the first language
    {
        DBG_INFO((_EDB_INFO"language not match\r\n"));
        return EDBR_INFO_NOT_FOUND;
#if 0        
        pt_descript_item =  &(t_descript_items[aui2_descript_position[0]]);
        if (pt_descript_item != NULL)
        {
            x_memcpy(&t_lang_match, pt_descript_item->pui1_start_addr , 3);
            b_match = TRUE;
        }
#endif        
    }



    if (b_match == TRUE)    /*the position save in j*/
    {
        /**/
        //x_dbg_stmt("the match language is:%s\r\n",pt_lang[j]);
        for(i=0;i<ui1_count && i<EDB_EVENT_MAX_DES_SIZE;i++)/*if same language has more than one desctiptor,use max length's des*/
        {   UINT8   ui1_des_len = 0;
            pt_descript_item =  &(t_descript_items[aui2_descript_position[i]]);
            if( pt_descript_item->pui1_start_addr != NULL && 
                pt_descript_item->ui1_len >3)
            {
                x_memcpy(&t_lang, pt_descript_item->pui1_start_addr, 3);
                if (_dvb_lang_match(&t_lang,&t_lang_match) == TRUE)
                {
                    if (pt_descript_item->ui1_len > ui1_des_len)
                    {
                        ui1_des_len = pt_descript_item->ui1_len;
                        b_match = TRUE;
                        ui1_match_position = aui2_descript_position[i];
                    }
                }
            }
        }
    }



    //t_descript_items[ui1_match_position] is we want
    if (b_match == TRUE && ui1_match_position < EDB_EVENT_MAX_DES_SIZE)/*Begin decode event title and detail from this short event descriptor*/
    {
        pt_descript_item = &(t_descript_items[ui1_match_position]);
    }
    else/*Not match,use the first*/
    {
        pt_descript_item =  &(t_descript_items[aui2_descript_position[0]]);
    }


    /*Start decode*/
    {
        UINT8                   ui1_event_name_length         = 0;
        UINT8*                  pui1_event_name_start_addr    = NULL;
        UINT8                   ui1_event_detail_length       = 0;
        UINT8*                  pui1_event_detail_start_addr  = NULL;
        DVB_SI_TXT_INFO_T       t_dvb_si_txt                  = {0};
        CHAR*                   ps_temporary                  = NULL;
        UINT16                  ui2_i                         = 0;

        
        x_memset(s_event_detail_buffer,0x0,EDB_EVENT_DETAIL_BUFFER);

        t_buffer_parser.pui1_buffer = (UINT8*)pt_descript_item->pui1_start_addr;   //Event title Raw data start address include language
        buffer_parse_data(&t_buffer_parser, 3*8, NULL                           ); //skip x Byte till event_name_length
        buffer_parse_data(&t_buffer_parser, 8,  &ui1_event_name_length          ); //event_name_length
        pui1_event_name_start_addr = t_buffer_parser.pui1_buffer + t_buffer_parser.ui4_token;
        buffer_parse_data(&t_buffer_parser, (UINT32)(ui1_event_name_length*8), NULL ); //skip event title

        /*For event title,do not show multiple language,only show the exactly
         * matched language*/
        if (ui1_event_name_length>0 && (pt_edb_event_info->ps_event_title == NULL))
        {
            x_memset(&t_dvb_si_txt,0x0,sizeof(DVB_SI_TXT_INFO_T));
            i4_ret = _dvb_txt_process(pui1_event_name_start_addr,ui1_event_name_length,
                                      t_country,t_operator, DVB_TEXT_CHAR_CODE_INVALID,
                                      &t_dvb_si_txt);

            if (i4_ret == EDBR_OK && 
                t_dvb_si_txt.ps_txt!= NULL /*&& (*(t_dvb_si_txt.pz_txt_len)) > 0*/
               )
            {   //event title in t_dvb_si_txt.ps_txt (UTF8)
                pt_edb_event_info->ps_event_title = t_dvb_si_txt.ps_txt;
                ps_temporary = pt_edb_event_info->ps_event_title;
                for(ui2_i=0;ui2_i<x_strlen(ps_temporary);ui2_i++)
                {
                    if( (*(ps_temporary + ui2_i) == (CHAR)0x0D) || (*(ps_temporary + ui2_i) == (CHAR)0x0A) )
                    {
                        *(ps_temporary + ui2_i) = ' ';
                    }
                }
            }
            else
            {
                if(t_dvb_si_txt.ps_txt != NULL) {EDB_PARTITION_FREE(t_dvb_si_txt.ps_txt);}
            }
        }


        buffer_parse_data(&t_buffer_parser, 8,  &ui1_event_detail_length          ); //event_detail_length
        pui1_event_detail_start_addr = t_buffer_parser.pui1_buffer + t_buffer_parser.ui4_token;
        if (ui1_event_detail_length > 0)
        {
            edb_convert_raw_text_data(&pui1_event_detail_start_addr,ui1_event_detail_length,t_country,ui2_svl_id);
            x_memset(&t_dvb_si_txt,0x0,sizeof(DVB_SI_TXT_INFO_T));

#ifdef CLI_LVL_ALL
            extern BOOL TEST_MODE;
            extern UINT8*          g_pui1_test_source_buffer;
            extern SIZE_T          g_pui1_test_source_buffer_len;

            if (TEST_MODE == TRUE)
            {
                pui1_event_detail_start_addr = g_pui1_test_source_buffer;
                ui1_event_detail_length = g_pui1_test_source_buffer_len ;
            }
#endif

            i4_ret = _dvb_txt_process(pui1_event_detail_start_addr,ui1_event_detail_length,
                                      t_country,t_operator, DVB_TEXT_CHAR_CODE_INVALID,
                                      &t_dvb_si_txt);

            if (i4_ret == EDBR_OK && 
                t_dvb_si_txt.ps_txt!= NULL /*&& (*(t_dvb_si_txt.pz_txt_len)) > 0*/
               )
            {   /*event detail in t_dvb_si_txt.ps_txt (UTF8)*/
                s_event_detail_buffer[EDB_EVENT_DETAIL_BUFFER -1] = '\0';
                x_memcpy(
                        (CHAR*)s_event_detail_buffer + x_strlen(s_event_detail_buffer),
                        (CHAR*)t_dvb_si_txt.ps_txt,
                        x_strlen(t_dvb_si_txt.ps_txt)
                        );

                EDB_PARTITION_FREE(t_dvb_si_txt.ps_txt);           
            }
            else
            {
                if(t_dvb_si_txt.ps_txt != NULL) {EDB_PARTITION_FREE(t_dvb_si_txt.ps_txt);}
            }
           

            /*copy event detail from s_event_detail_buffer*/ 
            {
                SIZE_T  _temp_len = 0;
                s_event_detail_buffer[EDB_EVENT_DETAIL_BUFFER -1] = '\0';
                _temp_len =x_strlen(s_event_detail_buffer);
                if (_temp_len > 0 && pt_lang_event_detail!= NULL && _temp_len < EDB_EVENT_DETAIL_BUFFER)
                {
                    if (pt_lang_event_detail->ps_short_text == NULL) 
                    {
                        pt_lang_event_detail->ps_short_text =(CHAR*) EDB_PARTITION_ALLOC(_temp_len + 1);
                        if (pt_lang_event_detail->ps_short_text != NULL)
                        {
                            x_memset(pt_lang_event_detail->ps_short_text,0x0, (_temp_len +1));
                            x_memcpy(pt_lang_event_detail->ps_short_text,s_event_detail_buffer,_temp_len);
                        }
                    }
                }
            }
        }
    }
    return i4_ret;    
}


static INT32 _dvb_process_guidance(
        UINT8 ui1_tag,
        EDB_POOL_EVENT_INFO_T* pt_edb_event_info,
        EDB_PREF_LANG_T*  pt_pref_lang,
        ISO_3166_COUNT_T  t_country,
        UINT32   t_operator,
        UINT16  ui2_svl_id)
{
    INT32               i4_ret                   = EDBR_INFO_NOT_FOUND;
    UINT8               i                        = 0;
    UINT8               j                        = 0;
    UINT8               ui1_count                = 0;
    BOOL                b_match                  = FALSE;
    DESCRIPTOR_ITEM*    pt_descript_item         = NULL;
    BUFFER_PARSER       t_buffer_parser          = {0};
    ISO_639_LANG_T      t_lang                                     = {0};
                             //Matched language


    EDB_CHECK_RET( 
            (pt_edb_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    i4_ret = _edb_dvb_get_descriptor_count(ui1_tag,&ui1_count);
    if (i4_ret == EDBR_INFO_NOT_FOUND || ui1_count == 0)
    {
        return i4_ret;
    }


    /*Get all language from short event descriptors*/
   

    if (pt_pref_lang != NULL)
    {
        for(i=0;i<pt_pref_lang->ui1_num && i<EDB_MAX_NUM_PREF_LANG;i++)/*user setting*/
        {
            for(j=0;j<ui1_count && j<EDB_EVENT_MAX_DES_SIZE;j++)
            {
                pt_descript_item =    &(t_descript_items[aui2_descript_position[j]]);
                if( pt_descript_item->pui1_start_addr != NULL && 
                    pt_descript_item->ui1_len >4)
                {
                    //x_memcpy(&t_lang, pt_descript_item->pui1_start_addr, 3);
                    if((pt_descript_item->pui1_start_addr[0] & 0x3) == 0x0)
                    {
                        x_memcpy(&t_lang, pt_descript_item->pui1_start_addr + 1, 3);
                        if (_dvb_lang_match(&t_lang, &(pt_pref_lang->t_lang[0]) + i) == TRUE)
                        {
                            /*TS Position j is needed*/
                            b_match = TRUE;
                            break;
                        }
                    }
                    else if((pt_descript_item->pui1_start_addr[0] & 0x3) == 0x1)
                    {
                        x_memcpy(&t_lang, pt_descript_item->pui1_start_addr + 2, 3);
                        if (_dvb_lang_match(&t_lang, &(pt_pref_lang->t_lang[0]) + i) == TRUE)
                        {
                            /*TS Position j is needed*/
                            b_match = TRUE;
                            break;
                        }                
                    }

                }
            }
    
  /*          for(j=0;j<ui1_count && j<EDB_EVENT_MAX_DES_SIZE;j++)       
            {
                if (_dvb_lang_match(pt_lang + j, &(pt_pref_lang->t_lang[0]) + i) == TRUE)
                {
                  
                    b_match = TRUE;
                    x_memcpy(&t_lang_match, pt_lang + j, 3);
                    break;
                }
            }
             */
            if (b_match == TRUE)
            {
                break;
            }
        }
    }

    if (b_match == FALSE)//use the first language
    {
        pt_descript_item =  &(t_descript_items[aui2_descript_position[0]]);
        /*
        if (pt_descript_item != NULL)
        {
            x_memcpy(&t_lang_match, pt_descript_item->pui1_start_addr , 3);
            b_match = TRUE;
        }
         */
    }


    /*Start decode*/
    {
        UINT8*                  pui1_guidance_text_start_addr = NULL;
        UINT8                    ui1_guidance_type              = 0;
        UINT8                    ui1_guidance_text_length      = 0;
        DVB_SI_TXT_INFO_T       t_dvb_si_txt                  = {0};
        UINT8                    ui1_guidance_mode             = GUIDANCE_MODE_NOT_DEFINE;

        t_buffer_parser.pui1_buffer = (UINT8*)pt_descript_item->pui1_start_addr;   //Event title Raw data start address include language
        buffer_parse_data(&t_buffer_parser, 6, NULL                           ); //skip reserved 
        buffer_parse_data(&t_buffer_parser, 2,  &ui1_guidance_type            ); //event_name_length

        if(ui1_guidance_type == 0x0)
        {
            ui1_guidance_text_length = pt_descript_item->ui1_len - 4;
        }
        else if(ui1_guidance_type == 0x1)
        {
            buffer_parse_data(&t_buffer_parser, 7, NULL                           ); //skip reserved 
            buffer_parse_data(&t_buffer_parser, 1, &ui1_guidance_mode              );
            if(ui1_guidance_mode == 1)
            {
                pt_edb_event_info->ui1_guidance_mode = UNSUITABLE_UNTIL_AFTER_THE_WATERSHED; 
            }
            ui1_guidance_text_length = pt_descript_item->ui1_len - 5;
        }    
        buffer_parse_data(&t_buffer_parser, 3*8, NULL                           ); //skip x Byte till event_name_length
        
        pui1_guidance_text_start_addr = t_buffer_parser.pui1_buffer + t_buffer_parser.ui4_token;


        if (ui1_guidance_text_length>0)
        {
            x_memset(&t_dvb_si_txt,0x0,sizeof(DVB_SI_TXT_INFO_T));
            edb_convert_raw_text_data(&pui1_guidance_text_start_addr,ui1_guidance_text_length,t_country,ui2_svl_id);
            i4_ret = _dvb_txt_process(pui1_guidance_text_start_addr,ui1_guidance_text_length,
                                      t_country,t_operator, DVB_TEXT_CHAR_CODE_INVALID,

                                      &t_dvb_si_txt);

            if (i4_ret == EDBR_OK && 
                t_dvb_si_txt.ps_txt!= NULL /*&& (*(t_dvb_si_txt.pz_txt_len)) > 0*/
               )
            {   //event title in t_dvb_si_txt.ps_txt (UTF8)
                pt_edb_event_info->ps_event_guidance = t_dvb_si_txt.ps_txt;
#if 0
                x_memcpy((UINT8*)( &(pt_edb_event_info->ac_event_title[0])),
                         t_dvb_si_txt.ps_txt,
                         ((UINT8)(*(t_dvb_si_txt.pz_txt_len)) > EDB_MAX_LENGTH_EVENT_TITLE)?
                         EDB_MAX_LENGTH_EVENT_TITLE:(UINT8)(*(t_dvb_si_txt.pz_txt_len))
                        );
                EDB_FREE(t_dvb_si_txt.ps_txt);
#endif
            }
            else
            {
                if(t_dvb_si_txt.ps_txt != NULL) {EDB_PARTITION_FREE(t_dvb_si_txt.ps_txt);}
            }
        }      
    }

    /*Get user setting pref language*/
    //EDB_FREE(pt_lang);
    return i4_ret;    
}


static INT32 _dvb_process_extended_event(
        UINT8 ui1_tag,
        EDB_POOL_EVENT_INFO_T* pt_edb_event_info,
        EDB_PREF_LANG_T*  pt_pref_lang,
        ISO_3166_COUNT_T  t_country,
        CHAR*             ps_event_detail_separator,
        UINT32   t_operator,
        LANG_EVENT_DETAIL*  pt_lang_event_detail,
        UINT16              ui2_svl_id
        )
{
    INT32               i4_ret                                     = EDBR_INFO_NOT_FOUND;
    INT32               i                                          = 0;
    INT32               j                                          = 0;
    UINT8               ui1_count                                  = 0;
    BOOL                b_match                                    = FALSE;
    ISO_639_LANG_T*     pt_lang                                    = &(at_lang[0]);
    DESCRIPTOR_ITEM*    pt_descript_item                           = NULL;
    ISO_639_LANG_T      t_lang                                     = {0};
    BUFFER_PARSER       t_buffer_parser                            = {0};
    UINT8               ui1_matched_count_for_same_lang            = 0;
    UINT8               aui1_matched_position_for_same_language[MAX_DESCRIPT_FOR_SAME_LANGUAGE] = {0};
    ISO_639_LANG_T      t_lang_match                               = {0};                               //Matched language
    UINT16              ui2_temp                                    = 0;
    EDB_EVENT_EXTENDED_LIST_T*     pt_event_extended_list          = NULL;

    EDB_CHECK_RET( 
            (pt_edb_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    i4_ret = _edb_dvb_get_descriptor_count(ui1_tag,&ui1_count);
    if (i4_ret == EDBR_INFO_NOT_FOUND || ui1_count == 0)
    {
        return i4_ret;
    }

    // sort the aui2_descript_position
    for(i=0;i<ui1_count-1;i++)
    {
        for(j=0;j<ui1_count-i-1;j++)
        {
            if( 
                    ((t_descript_items[aui2_descript_position[j]].pui1_start_addr[0] & 0xf0) >> 4) 
                    > ((t_descript_items[aui2_descript_position[j+1]].pui1_start_addr[0] & 0xf0) >> 4)
              )
            {
                ui2_temp = aui2_descript_position[j];
                aui2_descript_position[j] = aui2_descript_position[j+1];
                aui2_descript_position[j+1] = ui2_temp;            
            }
        }
    }



#ifdef CLI_LVL_ALL
    extern BOOL TEST_MODE;
    extern UINT8*          g_pui1_test_source_buffer;
    extern SIZE_T          g_pui1_test_source_buffer_len;

    if (TEST_MODE == TRUE)
    {
        return EDBR_OK;
    }
#endif


    x_memset(pt_lang,0x0,sizeof(at_lang));

    /*Get language from extended event descriptors*/
    for(i=0;i<ui1_count && i<EDB_EVENT_MAX_DES_SIZE;i++)
    {
        pt_descript_item =  &(t_descript_items[aui2_descript_position[i]]);
        if( pt_descript_item->pui1_start_addr != NULL && 
            pt_descript_item->ui1_len >3)
        {
            x_memcpy(pt_lang[i], pt_descript_item->pui1_start_addr+1, 3);/*skip descriptor_number 4 uimsbf last_descriptor_number*/
        }
    }

    /*Get match language*/
    if (pt_pref_lang != NULL)
    {
        for(i=0;i<pt_pref_lang->ui1_num && i<EDB_MAX_NUM_PREF_LANG;i++)/*user setting*/
        {
            for(j=0;j<ui1_count && j<EDB_EVENT_MAX_DES_SIZE;j++)        /*TS*/
            {
                if (_dvb_lang_match(pt_lang + j, &(pt_pref_lang->t_lang[0]) + i) == TRUE)
                {
                    /*TS Position j is needed*/
                    //pt_lang + j is AP matched  language
                    b_match = TRUE;
                    x_memcpy(&t_lang_match, pt_lang + j, 3);/*Skip descriptor_number 4 uimsbf &  last_descriptor_number 4 uimsbf*/
                    break;
                }
            }
            if (b_match == TRUE)
            {
                break;
            }
        }
    }

    if (b_match == FALSE)//use the first language
    {
        DBG_INFO((_EDB_INFO"language not match\r\n"));
        return EDBR_INFO_NOT_FOUND;
    }

    //matched language is t_lang_match 


    /*
descriptor_number: This 4-bit field gives the number of the descriptor. 
It is used to associate information which cannot be fitted into a single descriptor. 
The descriptor_number of the first extended_event_descriptor of an associated 
set of extended_event_descriptors shall be "0x00". 
The descriptor_number shall be incremented by 1 with each additional extended_event_descriptor in this section.

last_descriptor_number: This 4-bit field specifies the number of the last extended_event_descriptor 
(that is, the descriptor with the highest value of descriptor_number) 
of the associated set of descriptors of which this descriptor is part.
*/


    /*Find count of this matched language*/
    if (b_match == TRUE)    /*the position save in j*/
    {

        /**/
        //x_dbg_stmt("the match language is:%s\r\n",t_lang_match);
        for(i=0;i<ui1_count && i<EDB_EVENT_MAX_DES_SIZE;i++)/*if same language has more than one desctiptor*/
        {   
            pt_descript_item =  &(t_descript_items[aui2_descript_position[i]]);
            if( pt_descript_item->pui1_start_addr != NULL && 
                pt_descript_item->ui1_len >3)
            {
                x_memcpy(&t_lang, pt_descript_item->pui1_start_addr + 1, 3);/*Skip descriptor_number 4 uimsbf &  last_descriptor_number 4 uimsbf*/
                if (_dvb_lang_match(&t_lang,&t_lang_match) == TRUE)
                {
                    if (ui1_matched_count_for_same_lang < MAX_DESCRIPT_FOR_SAME_LANGUAGE)
                    { 
                        aui1_matched_position_for_same_language[ui1_matched_count_for_same_lang++] = aui2_descript_position[i];
                    }
                }
            }
        }

    }else
    {
        ui1_matched_count_for_same_lang = 0x1;
        //aui1_matched_position_for_same_language[ui1_matched_count_for_same_lang++] = aui2_descript_position[0];
        aui1_matched_position_for_same_language[0] = aui2_descript_position[0];
    }

    /*parse extended event desc to pt_event_extended_list */
    /*Start decode*/
    {
        UINT8                               ui1_length_of_items             = 0;
        UINT8                               ui1_text_length                 = 0;
        UINT8*                              pui1_text_start_addr            = NULL;
        DVB_SI_TXT_INFO_T                   t_dvb_si_txt                    = {0};
        EDB_EVENT_EXTENDED*                 pt_event_extended               = NULL;
        UINT32                              ui4_event_item_start            = 0;
        UINT8                               ui1_item_description_length     = 0;
        UINT8*                              pui1_item_description_char_addr = NULL;
        UINT8                               ui1_item_length                 = 0;
        UINT8                               ui1_item_index                  = 0;
        UINT8*                              pui1_item_char_addr             = NULL;
        EDB_EVENT_EXTENDED_ITEM_LIST_T*     pt_event_extended_item_list     = NULL;
        E_DVB_TEXT_CHAR_TBL                 e_last_extended_text_char_tbl   = DVB_TEXT_CHAR_CODE_INVALID;
        UINT8                               ui1_avalid_control_code_count   = 0;
        MULT_BYTE_UTF8_T                    mult_byte_utf8_t                = {0};

        /*Initial multbyte buffer*/
        x_memset(s_addi_mult_byte,0x0,sizeof(s_addi_mult_byte));
        ui1_addi_mult_byte_len = 0;

        pt_edb_event_info->pt_event_extended_list = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_EXTENDED_LIST_T));
        if (pt_edb_event_info->pt_event_extended_list == NULL)
        {
            x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
            return EDBR_OUT_OF_MEM;
        }

        x_memset(pt_edb_event_info->pt_event_extended_list,0x0,sizeof(EDB_EVENT_EXTENDED_LIST_T));
        pt_edb_event_info->pt_event_extended_list->ui1_event_extended_num = ui1_matched_count_for_same_lang;


        pt_edb_event_info->pt_event_extended_list->pt_event_extended 
            = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_EXTENDED) * ui1_matched_count_for_same_lang);

        if (pt_edb_event_info->pt_event_extended_list->pt_event_extended == NULL)
        {
            x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
            return EDBR_OUT_OF_MEM;
        }

        pt_event_extended = pt_edb_event_info->pt_event_extended_list->pt_event_extended;
        x_memset(pt_event_extended,0x0,(sizeof(EDB_EVENT_EXTENDED) * ui1_matched_count_for_same_lang));

        for (i=0;i<ui1_matched_count_for_same_lang&& i < MAX_DESCRIPT_FOR_SAME_LANGUAGE;i++)
        {
            pt_descript_item = &(t_descript_items[aui1_matched_position_for_same_language[i]]);
            x_memset(&t_buffer_parser,0x0,sizeof(BUFFER_PARSER));
            t_buffer_parser.pui1_buffer = (UINT8*)pt_descript_item->pui1_start_addr;    //Extended Event Raw data start address 
            buffer_parse_data(&t_buffer_parser, 4*8, NULL                           );  //Skip 4 Byte descriptor_number 4 uimsbf
            //last_descriptor_number 4 uimsbf
            //ISO_639_language_code 24
            //Till length_of_items
            buffer_parse_data(&t_buffer_parser, 8,  &ui1_length_of_items          );    //length_of_items 8 uimsbf


            //Parse items            
            //buffer_parse_data(&t_buffer_parser, (UINT32)(ui1_length_of_items*8),  NULL);//Skip items
            if (ui1_length_of_items>0)
            {
                ui4_event_item_start = t_buffer_parser.ui4_token;
                ui1_event_extended_item_len =0;
                x_memset(&at_event_extended_item,0x0,sizeof(at_event_extended_item));
                while (t_buffer_parser.ui4_token - ui4_event_item_start < ui1_length_of_items && ui1_event_extended_item_len<20)
                {

                    buffer_parse_data(&t_buffer_parser, 8,  &ui1_item_description_length);//item_description_length 8 uimsbf
                    pui1_item_description_char_addr = t_buffer_parser.pui1_buffer + t_buffer_parser.ui4_token;
                    if (ui1_item_description_length>0)
                    {
                        edb_convert_raw_text_data(&pui1_item_description_char_addr,ui1_item_description_length,t_country,ui2_svl_id);
                        x_memset(&t_dvb_si_txt,0x0,sizeof(DVB_SI_TXT_INFO_T));
                        i4_ret = _dvb_txt_process(pui1_item_description_char_addr,ui1_item_description_length,
                                                  t_country,t_operator, DVB_TEXT_CHAR_CODE_INVALID,

                                                  &t_dvb_si_txt);
                        at_event_extended_item[ui1_event_extended_item_len].ps_extended_item_descript = t_dvb_si_txt.ps_txt;
                        buffer_parse_data(&t_buffer_parser,(UINT32)(ui1_item_description_length* 8),NULL);//Skip item_description
                    }



                    buffer_parse_data(&t_buffer_parser, 8,  &ui1_item_length);//item_length 8 uimsbf
                    pui1_item_char_addr = t_buffer_parser.pui1_buffer + t_buffer_parser.ui4_token;
                    if (ui1_item_length > 0)
                    {
                        edb_convert_raw_text_data(&pui1_item_char_addr,ui1_item_length,t_country,ui2_svl_id);
                        x_memset(&t_dvb_si_txt,0x0,sizeof(DVB_SI_TXT_INFO_T));
                        i4_ret = _dvb_txt_process(pui1_item_char_addr,ui1_item_length,
                                                  t_country,t_operator, DVB_TEXT_CHAR_CODE_INVALID,

                                                  &t_dvb_si_txt);
                        at_event_extended_item[ui1_event_extended_item_len].ps_extended_item_text= t_dvb_si_txt.ps_txt;
                        buffer_parse_data(&t_buffer_parser,(UINT32)(ui1_item_length* 8),NULL);//Skip item_description
                    }

                    ui1_event_extended_item_len++;
                }

                pt_event_extended[i].pt_extended_item_list = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_EXTENDED_ITEM_LIST_T));
                if (pt_event_extended[i].pt_extended_item_list == NULL)
                {
                    x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
                    return EDBR_OUT_OF_MEM;
                }

                x_memset(pt_event_extended[i].pt_extended_item_list,0x0,sizeof(EDB_EVENT_EXTENDED_ITEM_LIST_T) );

                pt_event_extended_item_list = pt_event_extended[i].pt_extended_item_list;
                pt_event_extended_item_list->ui1_extended_items_num = ui1_event_extended_item_len;
                pt_event_extended_item_list->pt_extended_items = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_EXTENDED_ITEM) * ui1_event_extended_item_len );
                if (pt_event_extended_item_list->pt_extended_items == NULL)
                {
                    x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
                    return EDBR_OUT_OF_MEM;
                }
                x_memset(pt_event_extended_item_list->pt_extended_items,0x0,sizeof(EDB_EVENT_EXTENDED_ITEM) * ui1_event_extended_item_len  );

                for (ui1_item_index=0;ui1_item_index<ui1_event_extended_item_len && ui1_item_index < 20;ui1_item_index++)
                {
                    pt_event_extended_item_list->pt_extended_items[ui1_item_index].ps_extended_item_descript = 
                        at_event_extended_item[ui1_item_index].ps_extended_item_descript;
                    pt_event_extended_item_list->pt_extended_items[ui1_item_index].ps_extended_item_text = 
                        at_event_extended_item[ui1_item_index].ps_extended_item_text;
                }

            }


            buffer_parse_data(&t_buffer_parser, 8,  &ui1_text_length          );    //text_length 8 uimsbf
            pui1_text_start_addr = t_buffer_parser.pui1_buffer + t_buffer_parser.ui4_token;    
            if (ui1_text_length > 0)
            {
                edb_convert_raw_text_data(&pui1_text_start_addr,ui1_text_length,t_country,ui2_svl_id);
                x_memset(&t_dvb_si_txt,0x0,sizeof(DVB_SI_TXT_INFO_T));


                if (
                        (pui1_text_start_addr[0] >= 0x20) &&
                        (pui1_text_start_addr[0] <= 0xFF)/* &&
                        (t_eng_cfg_flag & EDB_DVB_CONFIG_USE_FIRST_EXTENDED_CONTROL_CODE)*/
                   )/*Have no control code*/
                {
                    if (DVB_TEXT_CHAR_CODE_ISO_10646_1_UTF_8 == e_last_extended_text_char_tbl)
                    {
                        _i4_process_utf8((CHAR*)pui1_text_start_addr,ui1_text_length,&mult_byte_utf8_t);
                        //Add 0x15 control code to header
                        if (mult_byte_utf8_t.s_buffer[0] != (CHAR)0x15) 
                        {
                            UINT8 ui1_idx = 0;
                            for ( ui1_idx = mult_byte_utf8_t.ui4_len; ui1_idx>=1;  ui1_idx--)
                            {
                                 mult_byte_utf8_t.s_buffer[ui1_idx] = mult_byte_utf8_t.s_buffer[ui1_idx -1];
                            }
                            mult_byte_utf8_t.ui4_len ++;
                            mult_byte_utf8_t.s_buffer[0] = 0x15;
                        }
                        pui1_text_start_addr = (UINT8*)mult_byte_utf8_t.s_buffer;
                        ui1_text_length      = mult_byte_utf8_t.ui4_len;
                    }

                    if (ui1_avalid_control_code_count == (UINT8)0x1)
                    {
                        i4_ret = _dvb_txt_process(pui1_text_start_addr,ui1_text_length,
                                                  t_country,t_operator, e_last_extended_text_char_tbl,
                                                  &t_dvb_si_txt);
                    }
                    else{
                        i4_ret = _dvb_txt_process(pui1_text_start_addr,ui1_text_length,
                                                  t_country,t_operator, DVB_TEXT_CHAR_CODE_INVALID,
                                                  &t_dvb_si_txt);
                        e_last_extended_text_char_tbl = t_dvb_si_txt.e_char_table;/*Save lastest extended text char table for other used*/
                        ui1_avalid_control_code_count++;
                    }


                    if (DVB_TEXT_CHAR_CODE_ISO_10646_1_UTF_8 == e_last_extended_text_char_tbl)
                    {
                        if (mult_byte_utf8_t.s_buffer != NULL) { x_mem_free(mult_byte_utf8_t.s_buffer); }
                    }
                }
                else
                {
                    if (pui1_text_start_addr[0]==(CHAR)0x15)
                    {
                        _i4_process_utf8((CHAR*)pui1_text_start_addr,ui1_text_length,&mult_byte_utf8_t);
                        pui1_text_start_addr = (UINT8*)mult_byte_utf8_t.s_buffer;
                        ui1_text_length      = mult_byte_utf8_t.ui4_len;
                    }

                    i4_ret = _dvb_txt_process(pui1_text_start_addr,ui1_text_length,
                                              t_country,t_operator, DVB_TEXT_CHAR_CODE_INVALID,
                                              &t_dvb_si_txt);
                    e_last_extended_text_char_tbl = t_dvb_si_txt.e_char_table;/*Save lastest extended text char table for other used*/
                    ui1_avalid_control_code_count++;

                    if (pui1_text_start_addr[0]==(CHAR)0x15)
                    {
                        /*UTF8 will forward to UTF16 decode*/
                        e_last_extended_text_char_tbl = DVB_TEXT_CHAR_CODE_ISO_10646_1_UTF_8;
                        if (mult_byte_utf8_t.s_buffer != NULL) 
                        { 
                            x_mem_free(mult_byte_utf8_t.s_buffer); 
                        }
                    }
                }


                pt_event_extended[i].ps_extended_text = t_dvb_si_txt.ps_txt;  /*Will be free when free event*/
            }
        }
        //Reset ui1_addi_mult_byte_len
        x_memset(s_addi_mult_byte,0x0,sizeof(s_addi_mult_byte));
        ui1_addi_mult_byte_len = 0;
    } 



    /*combine extended text to temp buffer*/
    /*Process extended event*/
    pt_event_extended_list = pt_edb_event_info->pt_event_extended_list;
    if (pt_event_extended_list != NULL)
    {
        UINT8   ui1_extended_num = pt_event_extended_list->ui1_event_extended_num;
        CHAR*                               ps_extended_text            = NULL;
        EDB_EVENT_EXTENDED_ITEM_LIST_T*     pt_extended_item_list       = NULL;
        UINT8                               ui1_extended_items_num      = 0x0;
        EDB_EVENT_EXTENDED_ITEM*            pt_extended_items           = NULL;
        SIZE_T                              z_len                       = 0;


        /*clear temp buffer*/
        x_memset(s_event_detail_buffer,0x0,EDB_EVENT_DETAIL_BUFFER);
       
#ifdef EDB_EXTENDED_STRATAGE1
        if (ui1_extended_num > (UINT8)0x0)
        {
            //Process Extended event Text
            for (i = 0 ;i < ui1_extended_num;i++)
            {
                pt_event_extended  = pt_event_extended_list->pt_event_extended + i;
                if (pt_event_extended != NULL)
                {
                    ps_extended_text = pt_event_extended->ps_extended_text;
                    if (ps_extended_text != NULL)
                    {
                        if (i == (UINT8)0x0)//is first extended event need check has short event information
                        {
                            if (pt_lang_event_detail!= NULL && pt_lang_event_detail->ps_short_text != NULL)
                            {   
                                SIZE_T     z_short_event_text_len            = 0;
                                CHAR*      ps_fisrt_extended_text            = NULL;
                                z_short_event_text_len = x_strlen(pt_lang_event_detail->ps_short_text);

                                if (ps_extended_text != NULL)
                                {
                                    ps_fisrt_extended_text = ps_extended_text;
                                    if( (z_short_event_text_len <= EDB_MIN_SED_COMPARE_LENGTH) ||
                                        (z_short_event_text_len > x_strlen(ps_fisrt_extended_text) ) ||
                                        (x_strncmp(pt_lang_event_detail->ps_short_text,ps_fisrt_extended_text,z_short_event_text_len) != 0)
                                      )
                                    {
                                    }
                                    else
                                    {
                                        if (pt_lang_event_detail->ps_short_text != NULL)
                                        {
                                            EDB_PARTITION_FREE(pt_lang_event_detail->ps_short_text);
                                            pt_lang_event_detail->ps_short_text = NULL;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            x_strcat(s_event_detail_buffer,ps_event_detail_separator);
                        }


                        x_strcat(s_event_detail_buffer,ps_extended_text);
                        z_len =  x_strlen(s_event_detail_buffer);
                        if (z_len > EDB_EVENT_DETAIL_BUFFER )
                        {
                            DBG_ABORT( DBG_MOD_EDB );
                        }
                    }

                    pt_extended_item_list  = pt_event_extended->pt_extended_item_list;
                    if (pt_extended_item_list != NULL )
                    {
                        ui1_extended_items_num = pt_extended_item_list->ui1_extended_items_num;

                        if (ui1_extended_items_num > 0 )
                        {
                            x_strcat(s_event_detail_buffer,EDB_SEPARATOR_OF_EXTENDED_TEXT_AND_ITEMS);    
                            z_len =  x_strlen(s_event_detail_buffer);
                        }
                        for (j = 0; j < ui1_extended_items_num; j++)
                        {
                            pt_extended_items  = pt_extended_item_list->pt_extended_items + j;
                            if (pt_extended_items->ps_extended_item_descript    != NULL && 
                                pt_extended_items->ps_extended_item_text        != NULL)
                            {
                                x_strcat(s_event_detail_buffer,pt_extended_items->ps_extended_item_descript);
                                x_strcat(s_event_detail_buffer,EDB_SEPARATOR_OF_ITEM_DESC_AND_ITEM_TEXT);
                                x_strcat(s_event_detail_buffer,pt_extended_items->ps_extended_item_text);
                                x_strcat(s_event_detail_buffer,EDB_SEPARATOR_OF_ITEM_AND_ITEM);

                                z_len =  x_strlen(s_event_detail_buffer);
                                if (z_len > EDB_EVENT_DETAIL_BUFFER )
                                {
                                    DBG_ABORT( DBG_MOD_EDB );
                                }

                            }
                        }
                    }
                }
            }

           
        }
#endif


#ifdef EDB_EXTENDED_STRATAGE2
        //Process Extended event Text
        if (ui1_extended_num > (UINT8)0x0)
        {
            EDB_EVENT_EXTENDED*                 pt_event_extended               = NULL;
            for (i = 0 ;i < ui1_extended_num;i++)
            {
                pt_event_extended  = pt_event_extended_list->pt_event_extended + i;
                if (pt_event_extended != NULL)
                {
                    ps_extended_text = pt_event_extended->ps_extended_text;
                    if (ps_extended_text != NULL)
                    {
                        if ((x_strncmp(t_country,"GBR",3) != 0) ||
                            (ui2_svl_id != EDB_SVL_ID_DVB_T))
                        {
                            if (i == (UINT8)0x0)//is first extended event need check has short event information
                            {
                                if (pt_lang_event_detail!= NULL && pt_lang_event_detail->ps_short_text != NULL)
                                {   
                                    SIZE_T     z_short_event_text_len            = 0;
                                    CHAR*      ps_fisrt_extended_text            = NULL;
                                    z_short_event_text_len = x_strlen(pt_lang_event_detail->ps_short_text);

                                    if (ps_extended_text != NULL)
                                    {
                                        ps_fisrt_extended_text = ps_extended_text;
                                        if( (z_short_event_text_len <= EDB_MIN_SED_COMPARE_LENGTH) ||
                                            (z_short_event_text_len > x_strlen(ps_fisrt_extended_text) ) ||
                                            (x_strncmp(pt_lang_event_detail->ps_short_text,ps_fisrt_extended_text,z_short_event_text_len) != 0)
                                          )
                                        {
                                        }
                                        else
                                        {
                                            if (pt_lang_event_detail->ps_short_text != NULL)
                                            {
                                                EDB_PARTITION_FREE(pt_lang_event_detail->ps_short_text);
                                                pt_lang_event_detail->ps_short_text = NULL;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        
                        x_strcat(s_event_detail_buffer,ps_extended_text);
                        z_len =  x_strlen(s_event_detail_buffer);
                        if (z_len > EDB_EVENT_DETAIL_BUFFER)
                        {
                            DBG_ABORT( DBG_MOD_EDB );
                        }
                    }
                }
            }
        }

        /*Process Extended event items*/
        if (ui1_extended_num > (UINT8)0x0)
        {
            EDB_EVENT_EXTENDED*                 pt_event_extended               = NULL;
            //Process Extended event Text
            for (i = 0 ;i < ui1_extended_num;i++)
            {
                pt_event_extended  = pt_event_extended_list->pt_event_extended + i;
                if (pt_event_extended != NULL)
                {
                    pt_extended_item_list  = pt_event_extended->pt_extended_item_list;
                    if (pt_extended_item_list != NULL )
                    {
                        ui1_extended_items_num = pt_extended_item_list->ui1_extended_items_num;
                        s_event_detail_buffer[EDB_EVENT_DETAIL_BUFFER -1] = '\0';
                        if (ui1_extended_items_num > 0)
                        {
                            if(x_strlen(s_event_detail_buffer) > 0)
                            {
                                //x_strncat(s_event_detail_buffer,EDB_SEPARATOR_OF_EXTENDED_TEXT_AND_ITEMS, EDB_EVENT_DETAIL_BUFFER - x_strlen(EDB_SEPARATOR_OF_EXTENDED_TEXT_AND_ITEMS) -1);
                                x_memcpy(
                                        (CHAR*)s_event_detail_buffer + x_strlen(s_event_detail_buffer),
                                        (CHAR*)EDB_SEPARATOR_OF_EXTENDED_TEXT_AND_ITEMS,
                                        x_strlen(EDB_SEPARATOR_OF_EXTENDED_TEXT_AND_ITEMS)
                                        );
                            }
                            else if (pt_lang_event_detail!= NULL && pt_lang_event_detail->ps_short_text != NULL)
                            {
                                x_memcpy(
                                        (CHAR*)s_event_detail_buffer + x_strlen(s_event_detail_buffer),
                                        (CHAR*)EDB_SEPARATOR_OF_SHORT_TEXT_AND_ITEMS_IF_THERE_IS_NO_EXTENDED_TEXT,
                                        x_strlen(EDB_SEPARATOR_OF_SHORT_TEXT_AND_ITEMS_IF_THERE_IS_NO_EXTENDED_TEXT)
                                        );
                            }
                            s_event_detail_buffer[EDB_EVENT_DETAIL_BUFFER -1] = '\0';
                            z_len =  x_strlen(s_event_detail_buffer);
                            if (z_len > EDB_EVENT_DETAIL_BUFFER )
                            {
                                DBG_ABORT( DBG_MOD_EDB );
                            }
                        }
                        for (j = 0; j < ui1_extended_items_num; j++)
                        {
                            pt_extended_items  = pt_extended_item_list->pt_extended_items + j;
                            if (pt_extended_items->ps_extended_item_descript    != NULL && 
                                pt_extended_items->ps_extended_item_text        != NULL)
                            {
                                x_strcat(s_event_detail_buffer,pt_extended_items->ps_extended_item_descript);
                                //x_strcat(s_event_detail_buffer,EDB_SEPARATOR_OF_ITEM_DESC_AND_ITEM_TEXT);
                                x_memcpy(
                                        (CHAR*)s_event_detail_buffer + x_strlen(s_event_detail_buffer),
                                        (CHAR*)EDB_SEPARATOR_OF_ITEM_DESC_AND_ITEM_TEXT,
                                        x_strlen(EDB_SEPARATOR_OF_ITEM_DESC_AND_ITEM_TEXT)
                                        );

                                x_strcat(s_event_detail_buffer,pt_extended_items->ps_extended_item_text);
                                /* E,g. has 3 item, 0 1 2
                                 * After no2 does not add separator "EDB_SEPARATOR_OF_ITEM_AND_ITEM 
                                 * */
                                if(j < ui1_extended_items_num - 1)
                                {
                                    s_event_detail_buffer[EDB_EVENT_DETAIL_BUFFER -1] = '\0';
                                    //x_strcat(s_event_detail_buffer,EDB_SEPARATOR_OF_ITEM_AND_ITEM);
                                    x_memcpy(
                                            (CHAR*)s_event_detail_buffer + x_strlen(s_event_detail_buffer),
                                            (CHAR*)EDB_SEPARATOR_OF_ITEM_AND_ITEM,
                                            x_strlen(EDB_SEPARATOR_OF_ITEM_AND_ITEM)
                                            );

                                }

                                s_event_detail_buffer[EDB_EVENT_DETAIL_BUFFER -1] = '\0';
                                z_len =  x_strlen(s_event_detail_buffer);
                                if (z_len > EDB_EVENT_DETAIL_BUFFER )
                                {
                                    DBG_ABORT( DBG_MOD_EDB );
                                }

                            }
                        }
                    }
                }
            }
        }       
#endif
    }

    /*copy event extended detail from s_event_detail_buffer*/ 
    {
        SIZE_T  _temp_len = 0;
        s_event_detail_buffer[EDB_EVENT_DETAIL_BUFFER - 1] = '\0';
        _temp_len =x_strlen(s_event_detail_buffer);
        if (_temp_len > 0 && pt_lang_event_detail!= NULL && _temp_len < EDB_EVENT_DETAIL_BUFFER )
        {
            if (pt_lang_event_detail->ps_extended_text == NULL) 
            {
                pt_lang_event_detail->ps_extended_text =(CHAR*) EDB_PARTITION_ALLOC(_temp_len + 1);
                if (pt_lang_event_detail->ps_extended_text != NULL)
                {
                    x_memset(pt_lang_event_detail->ps_extended_text,0x0, (_temp_len +1));
                    x_memcpy(pt_lang_event_detail->ps_extended_text,s_event_detail_buffer,_temp_len);
                }
            }
        }
    }



    /*Free event extended memory*/
    if (pt_edb_event_info->pt_event_extended_list != NULL)
    {
        EDB_EVENT_EXTENDED_ITEM*        pt_event_extended_item = NULL;
        if (pt_edb_event_info->pt_event_extended_list->pt_event_extended != NULL)
        {
            for(i=0;i<pt_edb_event_info->pt_event_extended_list->ui1_event_extended_num;i++)
            {
                EDB_EVENT_EXTENDED*  pt_event_extended = pt_edb_event_info->pt_event_extended_list->pt_event_extended + i;
                if (pt_event_extended->ps_extended_text != NULL)
                {
                    EDB_PARTITION_FREE(pt_event_extended->ps_extended_text);
                    pt_event_extended->ps_extended_text = NULL;
                }

                if (pt_event_extended->pt_extended_item_list != NULL)
                {
                    EDB_EVENT_EXTENDED_ITEM_LIST_T*     pt_extended_item_list = 
                        pt_event_extended->pt_extended_item_list;

                    if (pt_extended_item_list->pt_extended_items != NULL)
                    {
                        for (j=0;j<pt_extended_item_list->ui1_extended_items_num;j++)
                        {
                            pt_event_extended_item  = pt_extended_item_list->pt_extended_items + j;
                            if (pt_event_extended_item != NULL)
                            {
                                if (pt_event_extended_item->ps_extended_item_descript != NULL)
                                {
                                    EDB_PARTITION_FREE(pt_event_extended_item->ps_extended_item_descript);
                                    pt_event_extended_item->ps_extended_item_descript = NULL;

                                }
                                if (pt_event_extended_item->ps_extended_item_text != NULL)
                                {
                                    EDB_PARTITION_FREE(pt_event_extended_item->ps_extended_item_text);
                                    pt_event_extended_item->ps_extended_item_text = NULL;
                                }
                            }
                        }
                        EDB_PARTITION_FREE(pt_extended_item_list->pt_extended_items);
                        pt_extended_item_list->pt_extended_items = NULL;

                    }
                }

                if (pt_event_extended->pt_extended_item_list != NULL)
                {
                    EDB_PARTITION_FREE(pt_event_extended->pt_extended_item_list);
                    pt_event_extended->pt_extended_item_list = NULL;
                }

            }

            if (pt_edb_event_info->pt_event_extended_list->pt_event_extended != NULL)
            {
                EDB_PARTITION_FREE(pt_edb_event_info->pt_event_extended_list->pt_event_extended);
                pt_edb_event_info->pt_event_extended_list->pt_event_extended = NULL;
            }

            EDB_PARTITION_FREE(pt_edb_event_info->pt_event_extended_list );
            pt_edb_event_info->pt_event_extended_list = NULL;
        }
    }


    return i4_ret;    
}

static INT32 _dvb_process_time_shift(UINT8 ui1_tag,EDB_POOL_EVENT_INFO_T* pt_edb_event_info)
{
    INT32       i4_ret                           = 0x0;
    UINT8       ui1_count                        = 0;
    DESCRIPTOR_ITEM*        pt_descript_item     = NULL;
    BUFFER_PARSER           t_buffer_parser      = {0};
    EDB_EVENT_TIME_SHIFT*   pt_time_shift        = NULL;
    EDB_CHECK_RET( 
            (pt_edb_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    i4_ret = _edb_dvb_get_descriptor_count(ui1_tag,&ui1_count);
    if (i4_ret == EDBR_INFO_NOT_FOUND || ui1_count == 0)
    {
        return i4_ret;
    }

    if (ui1_count == (UINT8)0x1)
    {
        pt_time_shift = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_TIME_SHIFT));
        if (pt_time_shift == NULL)
        {
            x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
            return EDBR_OUT_OF_MEM;
        }
        x_memset(pt_time_shift,0x0,sizeof(EDB_EVENT_TIME_SHIFT));
        pt_edb_event_info->pt_time_shift = pt_time_shift;


        pt_descript_item =  &(t_descript_items[aui2_descript_position[0]]);
        if( pt_descript_item->pui1_start_addr != NULL)
        {
            x_memset(&t_buffer_parser,0x0,sizeof(BUFFER_PARSER));
            t_buffer_parser.pui1_buffer = (UINT8*)pt_descript_item->pui1_start_addr;    //
            buffer_parse_data(&t_buffer_parser, 2*8,  &(pt_time_shift->ui2_reference_service_id) );  //reference_service_id 16 uimsbf
            buffer_parse_data(&t_buffer_parser, 2*8,  &(pt_time_shift->ui1_reference_event_id)   );  //reference_event_id 16 uimsbf
        }
    }

    return i4_ret;    
}


/*
   component_descriptor(){
   descriptor_tag 8 uimsbf
   descriptor_length 8 uimsbf
   reserved_future_use 4 bslbf
   stream_content 4 uimsbf
   component_type 8 uimsbf
   component_tag 8 uimsbf
   ISO_639_language_code 24 bslbf
   for (i=0;i<N;i++){
   text_char 8 uimsbf
   }
   }
   */

static INT32 _dvb_process_component(
        UINT8 ui1_tag,
        EDB_POOL_EVENT_INFO_T* pt_edb_event_info,
        EDB_PREF_LANG_T*  pt_pref_lang,
        ISO_3166_COUNT_T  t_country)
{
    INT32                           i4_ret                          = EDBR_INFO_NOT_FOUND;
    INT32                           i                               = 0;
    UINT8                           ui1_count                       = 0;
    ISO_639_LANG_T*                 pt_lang                         = &(at_lang[0]);
    DESCRIPTOR_ITEM*                pt_descript_item                = NULL;
    UINT8*                          pui1_obj_data                   = NULL;
    BUFFER_PARSER                   t_buffer_parser                 = {0};
    EDB_EVENT_COMPONENT_LIST*       pt_component_list               = NULL;
    EDB_EVENT_COMPONENT*            pt_component                    = NULL;

#if 0
    BOOL                            b_match                         = FALSE;
    INT32                           j                               = 0;
    ISO_639_LANG_T                  t_lang                          = {0};
    BUFFER_PARSER                   t_buffer_parser                 = {0};
    EDB_EVENT_COMPONENT_LIST*       pt_component_list               = NULL;
    EDB_EVENT_COMPONENT*            pt_component                    = NULL;
    DVB_SI_TXT_INFO_T               t_dvb_si_txt                    = {0};
    UINT8                           ui1_matched_count_for_same_lang = 0;
    UINT8                           aui1_matched_position_for_same_language[MAX_DESCRIPT_FOR_SAME_LANGUAGE] = {0};
#endif

    EDB_CHECK_RET( 
            (pt_edb_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    i4_ret = _edb_dvb_get_descriptor_count(ui1_tag,&ui1_count);
    if (i4_ret == EDBR_INFO_NOT_FOUND || ui1_count == 0)
    {
        return i4_ret;
    }

    if (pt_edb_event_info->pt_component_list == NULL)
    {
        pt_edb_event_info->pt_component_list = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_COMPONENT_LIST));
        if (pt_edb_event_info->pt_component_list == NULL)
        {
            x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
            return EDBR_OUT_OF_MEM;
        }
        x_memset(pt_edb_event_info->pt_component_list,0x0,sizeof(EDB_EVENT_COMPONENT_LIST) );
        pt_component_list = pt_edb_event_info->pt_component_list;
        pt_component_list->ui1_num = ui1_count;
        pt_component_list->pt_event_component = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_COMPONENT) * ui1_count);
        if (pt_component_list->pt_event_component == NULL)
        {
            x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
            return EDBR_OUT_OF_MEM;
        }
        x_memset(pt_component_list->pt_event_component,0x0,sizeof(EDB_EVENT_COMPONENT) *  ui1_count);
    }

    if (pt_component_list == NULL || pt_component_list->pt_event_component == NULL)
    {
        return EDBR_INFO_NOT_FOUND;
    }


    x_memset(pt_lang,0x0,sizeof(at_lang));

    /*Get all language from component descriptors*/
    for(i=0;i<ui1_count && i<EDB_EVENT_MAX_DES_SIZE;i++)
    {
        pt_descript_item =  &(t_descript_items[aui2_descript_position[i]]);
        if( pt_descript_item->pui1_start_addr != NULL && 
            pt_descript_item->ui1_len >3)
        {
            x_memcpy(pt_lang[i], pt_descript_item->pui1_start_addr + 3, 3);/*addr[3] is language start*/
        }

#if 1
        pt_component = pt_component_list->pt_event_component + i;
        x_memset(&t_buffer_parser,0x0,sizeof(BUFFER_PARSER));
        t_buffer_parser.pui1_buffer = (UINT8*)pt_descript_item->pui1_start_addr;        //
        buffer_parse_data(&t_buffer_parser, 4,  NULL );                                 //Skip Reserved_future_use 4 bslbf
        buffer_parse_data(&t_buffer_parser, 4,  &(pt_component->ui1_stream_content) );  //stream_content 4 uimsbf
        buffer_parse_data(&t_buffer_parser, 8,  &(pt_component->ui1_component_type) );  //component_type 8 uimsbf
        buffer_parse_data(&t_buffer_parser, 8,  &(pt_component->ui1_component_tag) );   //component_tag 8 uimsbf

        buffer_parse_data(&t_buffer_parser, 8, &(pt_component->t_lang[0])  );  
        buffer_parse_data(&t_buffer_parser, 8, &(pt_component->t_lang[1])  );  
        buffer_parse_data(&t_buffer_parser, 8, &(pt_component->t_lang[2])  );  

#endif

        pui1_obj_data = (UINT8*)pt_descript_item->pui1_start_addr;
        if (pui1_obj_data != NULL && ((pui1_obj_data[2-2] & 0x0f) == 0x03) &&                                      /* stream content */
            ((pui1_obj_data[3-2] >= 0x10 && pui1_obj_data[3-2] <= 0x13) ||                  /* component type */
             (pui1_obj_data[3-2] >= 0x20 && pui1_obj_data[3-2] <= 0x23)))
        {
            pt_edb_event_info->b_caption = TRUE;
            //return i4_ret;
        }
    }


#if 0
    if (pt_pref_lang != NULL)
    {
        for(i=0;i<pt_pref_lang->ui1_num;i++)/*user setting*/
        {
            for(j=0;j<ui1_count;j++)        /*TS*/
            {
                if (_dvb_lang_match(pt_lang + j, &(pt_pref_lang->t_lang[0]) + i) == TRUE)
                {
                    /*TS Position j is needed*/
                    b_match = TRUE;
                    break;
                }
            }
            if (b_match == TRUE)
            {
                break;
            }
        }
    }


    /*Find count of this matched language*/
    if (b_match == TRUE)    /*the position save in j*/
    {

        /**/
        //x_dbg_stmt("the match language is:%s\r\n",pt_lang[j]);
        for(i=0;i<ui1_count;i++)/*if same language has more than one desctiptor*/
        {   
            pt_descript_item =  &(t_descript_items[aui2_descript_position[i]]);
            if( pt_descript_item->pui1_start_addr != NULL && 
                pt_descript_item->ui1_len >3)
            {
                x_memcpy(&t_lang, pt_descript_item->pui1_start_addr + 3, 3);/*Skip reserved_future_use stream_content component_type component_tag*/
                if (_dvb_lang_match(&t_lang,pt_lang+j) == TRUE)
                {
                    aui1_matched_position_for_same_language[ui1_matched_count_for_same_lang++] = aui2_descript_position[i];
                }
            }
        }

    }else
    {
        ui1_matched_count_for_same_lang = 0x1;
        aui1_matched_position_for_same_language[ui1_matched_count_for_same_lang++] = aui2_descript_position[0];
    }

    if (ui1_matched_count_for_same_lang>0)
    {
        pt_edb_event_info->pt_component_list = EDB_ALLOC(sizeof(EDB_EVENT_COMPONENT_LIST));
        x_memset(pt_edb_event_info->pt_component_list,0x0,sizeof(EDB_EVENT_COMPONENT_LIST) );
        pt_component_list = pt_edb_event_info->pt_component_list;
        pt_component_list->ui1_num = ui1_matched_count_for_same_lang;
        pt_component_list->pt_event_component = EDB_ALLOC(sizeof(EDB_EVENT_COMPONENT) * ui1_matched_count_for_same_lang);
        if (pt_component_list->pt_event_component == NULL)
        {
            DBG_ABORT( DBG_MOD_EDB );
        }
        x_memset(pt_component_list->pt_event_component,0x0,sizeof(EDB_EVENT_COMPONENT) * ui1_matched_count_for_same_lang );
    }

    for (i=0;i<ui1_matched_count_for_same_lang;i++)
    {
        pt_component = pt_component_list->pt_event_component + i;

        pt_descript_item = &(t_descript_items[aui1_matched_position_for_same_language[i]]);
        x_memset(&t_buffer_parser,0x0,sizeof(BUFFER_PARSER));
        t_buffer_parser.pui1_buffer = (UINT8*)pt_descript_item->pui1_start_addr;        //
        buffer_parse_data(&t_buffer_parser, 4,  NULL );                                 //Skip Reserved_future_use 4 bslbf
        buffer_parse_data(&t_buffer_parser, 4,  &(pt_component->ui1_stream_content) );  //stream_content 4 uimsbf
        buffer_parse_data(&t_buffer_parser, 8,  &(pt_component->ui1_component_type) );  //component_type 8 uimsbf
        buffer_parse_data(&t_buffer_parser, 8,  &(pt_component->ui1_component_tag) );   //component_tag 8 uimsbf
        buffer_parse_data(&t_buffer_parser, 24,  NULL );                                //Skip ISO_639_language_code 24 bslbf

        //pt_descript_item->ui1_len - t_buffer_parser.ui4_token//Text len

        x_memset(&t_dvb_si_txt,0x0,sizeof(DVB_SI_TXT_INFO_T));
        if ((UINT32)(pt_descript_item->ui1_len) - t_buffer_parser.ui4_token > 0)
        {
            i4_ret = _dvb_txt_process(t_buffer_parser.pui1_buffer + t_buffer_parser.ui4_token,
                                      (UINT32)(pt_descript_item->ui1_len) - t_buffer_parser.ui4_token,
                                      t_country, 
                                      &t_dvb_si_txt);
            if (pt_component_list!= NULL && pt_component_list->pt_event_component != NULL)
            {
                pt_component_list->pt_event_component[i].ps_text = t_dvb_si_txt.ps_txt;  /*Will be free when free event*/
            }
            else
            {
                EDB_FREE(t_dvb_si_txt.ps_txt);
            }
        }
    }
#endif

    //EDB_FREE(pt_lang);
    return i4_ret;    
}


static INT32 _dvb_process_ca_identifier(UINT8 ui1_tag,EDB_POOL_EVENT_INFO_T* pt_edb_event_info)
{
    INT32                   i4_ret               = 0x0;
    UINT8                   ui1_count            = 0;
    DESCRIPTOR_ITEM*        pt_descript_item     = NULL;
    BUFFER_PARSER           t_buffer_parser      = {0};
    UINT8                   ui1_index            = 0;

    EDB_EVENT_CA_SYSTEM_ID_LIST_T*   pt_ca_system      = NULL;
    UINT16                           ui2_num_ca_ids = 0;
    EDB_CHECK_RET( 
            (pt_edb_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    i4_ret = _edb_dvb_get_descriptor_count(ui1_tag,&ui1_count);
    if (i4_ret == EDBR_INFO_NOT_FOUND || ui1_count == 0)
    {
        return i4_ret;
    }

    if (ui1_count == (UINT8)0x1)
    {
        pt_descript_item =  &(t_descript_items[aui2_descript_position[0]]);
        if( pt_descript_item->pui1_start_addr != NULL)
        {
            x_memset(&t_buffer_parser,0x0,sizeof(BUFFER_PARSER));
            t_buffer_parser.pui1_buffer = (UINT8*)pt_descript_item->pui1_start_addr;    //
            ui2_num_ca_ids = pt_descript_item->ui1_len;

            if (ui2_num_ca_ids > (UINT16)0)
            {
                pt_ca_system = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_CA_SYSTEM_ID_LIST_T));
                if (pt_ca_system == NULL)
                {
                    x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
                    return EDBR_OUT_OF_MEM;
                }
                x_memset(pt_ca_system,0x0,sizeof(EDB_EVENT_CA_SYSTEM_ID_LIST_T));
                pt_edb_event_info->pt_ca_system = pt_ca_system;

                pt_ca_system->ui2_num_ca_ids = ui2_num_ca_ids;
                pt_ca_system->pui2_ca_ids    = EDB_PARTITION_ALLOC(sizeof(UINT16) * ui2_num_ca_ids);
                if (pt_ca_system->pui2_ca_ids == NULL)
                {
                    x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
                    return EDBR_OUT_OF_MEM;
                }

                for (ui1_index = 0;ui1_index<(UINT8)ui2_num_ca_ids;ui1_index++)
                {
                    buffer_parse_data(&t_buffer_parser, 16, &(pt_ca_system->pui2_ca_ids[ui1_index])  );  //CA_system_id 16 uimsbf
                }
            }
        }
    }

    return i4_ret;    
}

static INT32 _dvb_process_content(UINT8 ui1_tag,EDB_POOL_EVENT_INFO_T* pt_edb_event_info)
{
    INT32                   i4_ret               = 0x0;
    UINT8                   ui1_count            = 0;
    DESCRIPTOR_ITEM*        pt_descript_item     = NULL;
    BUFFER_PARSER           t_buffer_parser      = {0};
    UINT8                   ui1_index            = 0;

    //EDB_EVENT_CATEGORY_LIST_T*   pt_category_list      = NULL;
    UINT8                       ui1_category_len    = 0;
    UINT8                       ui1_category_num    = 0;
    UINT8                       ui1_category_value  = 0;

    EDB_CHECK_RET( 
            (pt_edb_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    i4_ret = _edb_dvb_get_descriptor_count(ui1_tag,&ui1_count);
    if (i4_ret == EDBR_INFO_NOT_FOUND || ui1_count == 0)
    {
        return i4_ret;
    }

    if (ui1_count == (UINT8)0x1)
    {
        pt_descript_item =  &(t_descript_items[aui2_descript_position[0]]);
        if( pt_descript_item->pui1_start_addr != NULL)
        {
            x_memset(&t_buffer_parser,0x0,sizeof(BUFFER_PARSER));
            t_buffer_parser.pui1_buffer = (UINT8*)pt_descript_item->pui1_start_addr;    //
            ui1_category_len = pt_descript_item->ui1_len;
            ui1_category_num = (UINT8)(ui1_category_len / 2);

            pt_edb_event_info->ui1_num_event_category = ui1_category_num;

			if( ui1_category_num > EDB_MAX_NUM_CATEGORY )
			{
				return EDBR_INTERNAL_ERROR;
			}
			

            for(ui1_index=0;ui1_index < ui1_category_num;ui1_index++)
            {
                if (ui1_index > (UINT8)(0x7))
                {
                    break;
                }

                buffer_parse_data(&t_buffer_parser, 8, &(ui1_category_value)                                                   );  
                buffer_parse_data(&t_buffer_parser, 8, NULL                                                                    );  

                pt_edb_event_info->aui1_event_category[ui1_index] = ui1_category_value;
            };
        }
    }

    return i4_ret;    
}

static INT32 _dvb_process_private(UINT8 ui1_tag,EDB_POOL_EVENT_INFO_T* pt_edb_event_info)
{
    INT32                       i4_ret               = 0x0;
    UINT8                       ui1_count            = 0;


    i4_ret = _edb_dvb_get_descriptor_count(ui1_tag,&ui1_count);
    if (i4_ret == EDBR_INFO_NOT_FOUND || ui1_count == 0)
    {
        pt_edb_event_info->b_has_private = FALSE;
        return i4_ret;
    }
    pt_edb_event_info->b_has_private = TRUE;
    return i4_ret;
}


static INT32 _dvb_process_rating(UINT8 ui1_tag,EDB_POOL_EVENT_INFO_T* pt_edb_event_info)
{
    INT32                       i4_ret               = 0x0;
    UINT8                       ui1_count            = 0;
    DESCRIPTOR_ITEM*            pt_descript_item     = NULL;
    BUFFER_PARSER               t_buffer_parser      = {0};
    UINT8                       ui1_index            = 0;

    EDB_EVENT_RATING_LIST_T*    pt_rating_list       = NULL;
    UINT8                       ui1_rating_desc_len  = 0;
    UINT8                       ui1_rating_num       = 0;//Total rating number of mult desc
    UINT8                       ui1_rating_index     = 0;//process mult rating desc current index

    EDB_CHECK_RET( 
            (pt_edb_event_info == NULL),
            (EDBR_INV_ARG),
            (("Wrong argument\r\n"))
            );

    i4_ret = _edb_dvb_get_descriptor_count(ui1_tag,&ui1_count);
    if (i4_ret == EDBR_INFO_NOT_FOUND || ui1_count == 0)
    {
        return i4_ret;
    }

    for(ui1_index=0;ui1_index<ui1_count && ui1_index<EDB_EVENT_MAX_DES_SIZE;ui1_index++)
    {
        pt_descript_item =  &(t_descript_items[aui2_descript_position[ui1_index]]);

        ui1_rating_desc_len = pt_descript_item->ui1_len;
        ui1_rating_num      += (UINT8)(ui1_rating_desc_len/ 4);
    }

    if (ui1_rating_num > 0)
    {
        if (pt_edb_event_info->pt_rating_list == NULL)
        {
            pt_edb_event_info->pt_rating_list = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_RATING_LIST_T));
            if (pt_edb_event_info->pt_rating_list == NULL)
            {
                x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
                return EDBR_OUT_OF_MEM;
            }
            if (pt_edb_event_info->pt_rating_list != NULL)
            {
                x_memset(pt_edb_event_info->pt_rating_list,0x0,sizeof(EDB_EVENT_RATING_LIST_T) );
                pt_rating_list = pt_edb_event_info->pt_rating_list;
            }
        }
        
        if (pt_rating_list != NULL)
        {
            pt_rating_list->ui1_num = ui1_rating_num;
            pt_rating_list->pt_rating = EDB_PARTITION_ALLOC(sizeof(EDB_EVENT_RATING_T) *  ui1_rating_num);
            if (pt_rating_list->pt_rating == NULL)
            {
                x_dbg_stmt("%s %d outof memory\r\n",__FILE__,__LINE__);
                return EDBR_OUT_OF_MEM;
            }
            
            if (pt_rating_list->pt_rating != NULL)
            {
                x_memset(pt_rating_list->pt_rating,0x0,sizeof(EDB_EVENT_RATING_T) *  ui1_rating_num );
            }
        }

    }

    for(ui1_index=0;ui1_index<ui1_count && ui1_index<EDB_EVENT_MAX_DES_SIZE;ui1_index++)
    {
        UINT8   ui1_rating_desc_len_single_desc = 0;
        UINT8   ui1_rating_num_single_desc      = 0;
        UINT8   i                               = 0;
      
        pt_descript_item                =  &(t_descript_items[aui2_descript_position[ui1_index]]);
        ui1_rating_desc_len_single_desc = pt_descript_item->ui1_len;
        ui1_rating_num_single_desc     += (UINT8)(ui1_rating_desc_len_single_desc/ 4);

        if( pt_descript_item->pui1_start_addr != NULL)
        {
            x_memset(&t_buffer_parser,0x0,sizeof(BUFFER_PARSER));
            t_buffer_parser.pui1_buffer = (UINT8*)pt_descript_item->pui1_start_addr;    //

            if (pt_rating_list != NULL && pt_rating_list->pt_rating != NULL)
            {
                for(i=0;i < ui1_rating_num_single_desc;i++)
                {
                    if (ui1_rating_index+i < ui1_rating_num )
                    {
                        buffer_parse_data(&t_buffer_parser, 8, &(pt_rating_list->pt_rating[ui1_rating_index+i].t_country[0])  );  
                        buffer_parse_data(&t_buffer_parser, 8, &(pt_rating_list->pt_rating[ui1_rating_index+i].t_country[1])  );  
                        buffer_parse_data(&t_buffer_parser, 8, &(pt_rating_list->pt_rating[ui1_rating_index+i].t_country[2])  );  
                        buffer_parse_data(&t_buffer_parser, 8, &(pt_rating_list->pt_rating[ui1_rating_index+i].ui1_rating)  );  
                        DBG_INFO((_EDB_INFO"Got rating from TS:country=%s value=%d\r\n",
                                  pt_rating_list->pt_rating[i].t_country,
                                  pt_rating_list->pt_rating[i].ui1_rating));
                    }
                };
                ui1_rating_index+= ui1_rating_num_single_desc;
            }
        }
    }

    return i4_ret;    
}




static BOOL _dvb_check_pf_and_nfy(
        DVB_ENG_DATA*           pv_eng_data,
        EDB_EVENT_POOL_CH_T*    pt_pool_channel,
        EDB_POOL_EVENT_INFO_T*  pt_edb_event_info,
        EDB_NFY_REASON_T        e_reason)
{
    UINT32                   ui4_current_channel_id = 0;

    EDB_CHECK_RET( 
            (pv_eng_data == NULL || pt_pool_channel == NULL || pt_edb_event_info == NULL),
            (FALSE),
            (("Wrong argument\r\n"))
            );

    ui4_current_channel_id = pv_eng_data->t_edb_eng_state.ui4_current_channel_id;
    if (pt_pool_channel->ui4_channel_id == ui4_current_channel_id)
    {
        /*
           EDB_REASON_NOW_EVENT_UPDATED or EDB_REASON_NEX_EVENT_UPDATED
           ui4_data1 is ui2_svl_id of current svl and 
           ui4_data2 is ui4_channel_id of current service.
           */

        if ((pt_pool_channel->t_key.ui2_on_id  ==MONITOR_ON_ID &&
             pt_pool_channel->t_key.ui2_ts_id  ==MONITOR_TS_ID&&  
             pt_pool_channel->t_key.ui2_svc_id ==MONITOR_SVC_ID))   
        {
            x_dbg_stmt("EDB Notify reason=%d\r\n",e_reason);
        }
        edb_handle_notify(e_reason, pv_eng_data->ui2_svl_id,ui4_current_channel_id);
        return TRUE;
    }




    return FALSE;    
}


static BOOL _dvb_check_active_window_and_nfy(
        DVB_ENG_DATA*           pt_eng_data,
        EDB_EVENT_POOL_CH_T*    pt_pool_channel,
        EDB_POOL_EVENT_INFO_T*  pt_edb_event_info)
{
    INT32                    i = 0x0;
    EDB_CACHE_ACTIVE_WIN_T *  pt_win = NULL;
    EDB_CH_ID_T*        pt_channel = NULL;
    TIME_T              t_start_time = 0;
    TIME_T              t_end_time   = 0;

    EDB_CHECK_RET( 
            (pt_eng_data == NULL || pt_pool_channel == NULL || pt_edb_event_info == NULL),
            (FALSE),
            (("Wrong argument\r\n"))
            );

    pt_win = &(pt_eng_data->t_edb_eng_state.t_active_win);
    if (pt_win->ui1_num_channel == (UINT8)0x0)
    {
        return FALSE;
    }

    for(i=0;i<pt_win->ui1_num_channel && i<EDB_MAX_NUM_ACTIVE_CHANNEL;i++)
    {
        pt_channel = &( pt_win->at_channel_id[i]);
        if (pt_channel->ui4_channel_id == pt_pool_channel->ui4_channel_id)
        {
            pt_win = &(pt_eng_data->t_edb_eng_state.t_active_win);
            t_start_time = pt_win->t_start_time;
            t_end_time   = t_start_time + pt_win->t_duration;
            if (pt_edb_event_info->t_start_time + pt_edb_event_info->t_duration >= t_start_time &&
                pt_edb_event_info->t_start_time<= t_end_time)
            {
                #if 0
                /*
                ui4_data1 is ui2_svl_id of current svl                 
                ui4_data2 is ui4_channel_id of updated service.
                */
                edb_handle_notify(EDB_REASON_EVENT_IN_ACTIVE_WIN_UPDATED, pv_eng_data->ui2_svl_id,pt_channel->ui4_channel_id);
                #endif
                
                return TRUE;
            }
            break;            
        }
    }

    return FALSE;
}


VOID edb_dvb_free_single_event_obj(
        VOID*       pt_event)
{
    INT32       i           = 0;
    INT32       j           = 0;
    EDB_POOL_EVENT_INFO_T* pt_edb_event_info = NULL;


    if (pt_event==NULL)
    {
        DBG_ABORT( DBG_MOD_EDB );
    }

    pt_edb_event_info = (EDB_POOL_EVENT_INFO_T*)pt_event;
#ifdef EDB_INTERNAL_MONITOR_EVENT
    if (ui2_monitor_event_id == pt_edb_event_info->ui2_event_id)
    {
        DBG_INFO((_EDB_INFO"Begin free event object\r\n"));
    }
#endif
    /*Free memory*/
    if (pt_edb_event_info->pt_event_linkage_list != NULL)
    {
        if (pt_edb_event_info->pt_event_linkage_list->pt_event_linkage != NULL)
        {
            EDB_PARTITION_FREE(pt_edb_event_info->pt_event_linkage_list->pt_event_linkage);
            pt_edb_event_info->pt_event_linkage_list->pt_event_linkage = NULL;
        }
        EDB_PARTITION_FREE(pt_edb_event_info->pt_event_linkage_list);
        pt_edb_event_info->pt_event_linkage_list = NULL;
    }


    if (pt_edb_event_info->ps_event_title != NULL)
    {   
        EDB_PARTITION_FREE(pt_edb_event_info->ps_event_title);
        pt_edb_event_info->ps_event_title = NULL;
    }

    if (pt_edb_event_info->ps_event_detail != NULL)
    {
        EDB_PARTITION_FREE(pt_edb_event_info->ps_event_detail);
        pt_edb_event_info->ps_event_detail = NULL;

    }

    if (pt_edb_event_info->ps_event_guidance != NULL)
    {
        EDB_PARTITION_FREE(pt_edb_event_info->ps_event_guidance);
        pt_edb_event_info->ps_event_guidance = NULL;
    }

    if (pt_edb_event_info->pt_event_extended_list != NULL)
    {
        EDB_EVENT_EXTENDED_ITEM*        pt_event_extended_item = NULL;
        if (pt_edb_event_info->pt_event_extended_list->pt_event_extended != NULL)
        {
            for(i=0;i<pt_edb_event_info->pt_event_extended_list->ui1_event_extended_num;i++)
            {
                EDB_EVENT_EXTENDED*  pt_event_extended = pt_edb_event_info->pt_event_extended_list->pt_event_extended + i;
                if (pt_event_extended->ps_extended_text != NULL)
                {
                    EDB_PARTITION_FREE(pt_event_extended->ps_extended_text);
                    pt_event_extended->ps_extended_text = NULL;
                }

                if (pt_event_extended->pt_extended_item_list != NULL)
                {
                    EDB_EVENT_EXTENDED_ITEM_LIST_T*     pt_extended_item_list = 
                        pt_event_extended->pt_extended_item_list;

                    if (pt_extended_item_list->pt_extended_items != NULL)
                    {
                        for (j=0;j<pt_extended_item_list->ui1_extended_items_num;j++)
                        {
                            pt_event_extended_item  = pt_extended_item_list->pt_extended_items + j;
                            if (pt_event_extended_item != NULL)
                            {
                                if (pt_event_extended_item->ps_extended_item_descript != NULL)
                                {
                                    EDB_PARTITION_FREE(pt_event_extended_item->ps_extended_item_descript);
                                    pt_event_extended_item->ps_extended_item_descript = NULL;

                                }
                                if (pt_event_extended_item->ps_extended_item_text != NULL)
                                {
                                    EDB_PARTITION_FREE(pt_event_extended_item->ps_extended_item_text);
                                    pt_event_extended_item->ps_extended_item_text = NULL;
                                }
                            }
                        }
                        EDB_PARTITION_FREE(pt_extended_item_list->pt_extended_items);
                        pt_extended_item_list->pt_extended_items = NULL;

                    }
                }

                if (pt_event_extended->pt_extended_item_list != NULL)
                {
                    EDB_PARTITION_FREE(pt_event_extended->pt_extended_item_list);
                    pt_event_extended->pt_extended_item_list = NULL;
                }

            }

            if (pt_edb_event_info->pt_event_extended_list->pt_event_extended != NULL)
            {
                EDB_PARTITION_FREE(pt_edb_event_info->pt_event_extended_list->pt_event_extended);
                pt_edb_event_info->pt_event_extended_list->pt_event_extended = NULL;
            }

            EDB_PARTITION_FREE(pt_edb_event_info->pt_event_extended_list );
            pt_edb_event_info->pt_event_extended_list = NULL;

        }
    }


    if (pt_edb_event_info->pt_time_shift != NULL)
    {
        EDB_PARTITION_FREE(pt_edb_event_info->pt_time_shift);
        pt_edb_event_info->pt_time_shift = NULL;
    }

    if (pt_edb_event_info->pt_component_list != NULL)
    {
        if (pt_edb_event_info->pt_component_list->pt_event_component != NULL)
        {
            EDB_EVENT_COMPONENT*                pt_event_component = NULL;
            for (i=0;i<pt_edb_event_info->pt_component_list->ui1_num;i++)
            {
                pt_event_component = pt_edb_event_info->pt_component_list->pt_event_component + i;
                if(pt_event_component->ps_text != NULL)
                {
                    EDB_PARTITION_FREE(pt_event_component->ps_text);
                    pt_event_component->ps_text = NULL;

                }
            }
            EDB_PARTITION_FREE(pt_edb_event_info->pt_component_list->pt_event_component);
            pt_edb_event_info->pt_component_list->pt_event_component = NULL;

        }
        EDB_PARTITION_FREE(pt_edb_event_info->pt_component_list);
        pt_edb_event_info->pt_component_list = NULL;

    }

    if (pt_edb_event_info->pt_ca_system != NULL)
    {
        if (pt_edb_event_info->pt_ca_system->pui2_ca_ids != NULL)
        {
            EDB_PARTITION_FREE(pt_edb_event_info->pt_ca_system->pui2_ca_ids );
            pt_edb_event_info->pt_ca_system->pui2_ca_ids = NULL;
        }
        EDB_PARTITION_FREE(pt_edb_event_info->pt_ca_system );
    }

#if 0
    if (pt_edb_event_info->pt_category_list != NULL)
    {
        if (pt_edb_event_info->pt_category_list->pt_category != NULL)
        {
            EDB_FREE(pt_edb_event_info->pt_category_list->pt_category );
            pt_edb_event_info->pt_category_list->pt_category = NULL;
        }

        EDB_FREE(pt_edb_event_info->pt_category_list );
        pt_edb_event_info->pt_category_list = NULL;
    }
#endif

    if (pt_edb_event_info->pt_rating_list != NULL)
    {
        if (pt_edb_event_info->pt_rating_list->pt_rating != NULL)
        {
            EDB_PARTITION_FREE(pt_edb_event_info->pt_rating_list->pt_rating );
            pt_edb_event_info->pt_rating_list->pt_rating = NULL;
        }

        EDB_PARTITION_FREE(pt_edb_event_info->pt_rating_list);
        pt_edb_event_info->pt_rating_list = NULL;
    }


    EDB_PARTITION_FREE(pt_edb_event_info);
    //pt_edb_event_info = NULL;

    return ;

}


static INT32 _i4_process_utf8(CHAR* buffer,UINT32 ui4_len,MULT_BYTE_UTF8_T* pt_mult_byte)
{
    INT32   i                           = 0;
    CHAR    first_byte                  = 0;
    UINT8   ui1_mult_byte_count         = 0;
    UINT8   ui1_mult_byte_need_count    = 0;
    CHAR*   s_multbyte_buffer           = NULL;
    UINT32  ui4_total_buffer_len        = 0;
    BOOL    b_has_multbyte_split        = FALSE;

    if (pt_mult_byte == NULL)
    {
        return FALSE;
    }
    
    ui4_total_buffer_len = ui1_addi_mult_byte_len + ui4_len;// + 2;/*Add \0 add head control code for utf8*/
    s_multbyte_buffer = x_mem_alloc(ui4_total_buffer_len);
    if (s_multbyte_buffer == NULL)
    {
        return -1;
    }

    x_memset(s_multbyte_buffer,0x0,ui4_total_buffer_len);
    if (ui1_addi_mult_byte_len < 10)
    {
        x_memcpy(s_multbyte_buffer,s_addi_mult_byte,ui1_addi_mult_byte_len);
    }
    x_memcpy(
            (CHAR*)s_multbyte_buffer + ui1_addi_mult_byte_len,
            buffer,ui4_len
            );

    pt_mult_byte->s_buffer = s_multbyte_buffer;
    pt_mult_byte->ui4_len  = ui4_total_buffer_len;


    //x_dbg_stmt("buffer addr=0x%x len=%d \r\n",buffer,ui4_len);
    for (i=ui4_len -1; i>=0; i--)
    {
        if ( (buffer[i] & 0xC0) == 0xC0)/*U+00000080  U+000007FF   110xxxxx 10xxxxxx*/

        {
            //x_dbg_stmt("Found UTF8 start byte=0x%X pos=%d \r\n",buffer[i],i);
            first_byte = buffer[i];
            break;
        }
    }


    //x_dbg_stmt("First_byte=0x%X pos=%d \r\n",first_byte,i);
    if ( (first_byte & 0xFC) == 0xFC/*11111100*/)         /*U+04000000  U+7FFFFFFF   1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx*/
    {
        ui1_mult_byte_count = 6;
    }
    else if((first_byte & 0xF8) == 0xF8/*11111000*/)      /*U+00200000  U+03FFFFFF   111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx*/
    {
        ui1_mult_byte_count = 5;
    }
    else if((first_byte & 0xF0) == 0xF0/*11110000*/)       /*U+00010000  U+001FFFFF   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx*/
    {
        ui1_mult_byte_count = 4;
    }
    else if((first_byte & 0xE0) == 0xE0/*11100000*/)       /*U+00000800  U+0000FFFF   1110xxxx 10xxxxxx 10xxxxxx*/
    {
        ui1_mult_byte_count = 3;
    }
    else if((first_byte & 0xC0) == 0xC0/*11000000*/)       /*U+00000080  U+000007FF   110xxxxx 10xxxxxx*/
    {
        ui1_mult_byte_count = 2;
    }
    else if((first_byte & 0x80) == 0x80/*10000000*/)       /*U+00000000  U+0000007F   0xxxxxxx*/
    {
        ui1_mult_byte_count = 1;
    }

    if (i>1 && i -1 + ui1_mult_byte_count  > ui4_len -1)/*position + multbyte - self first byte*/
    {
        DBG_INFO(("mult byte count = %d\r\n",ui1_mult_byte_count));
        ui1_mult_byte_need_count = (UINT32)(i + ui1_mult_byte_count -1) - (ui4_len -1);
        ui1_addi_mult_byte_len = (ui4_len - i);
        if(ui1_mult_byte_need_count>0)
        {
            DBG_INFO(("mult byte not completely = %d\r\n",ui1_mult_byte_need_count));
        }
        x_memset(s_addi_mult_byte,0x0,sizeof(s_addi_mult_byte));

        if (ui1_addi_mult_byte_len < 10)
        {
            x_memcpy(s_addi_mult_byte,buffer+i,ui1_addi_mult_byte_len);
        }
        DBG_INFO(("ui1_addi_mult_byte_len= %d\r\n",ui1_addi_mult_byte_len));
        for(i=0;i<ui1_addi_mult_byte_len;i++)
        {
            DBG_INFO(("s_addi_mult_byte[%d] = 0x%02X\r\n",i,s_addi_mult_byte[i]));
        }
        b_has_multbyte_split = TRUE;
        pt_mult_byte->ui4_len -= ui1_addi_mult_byte_len;
    }
    else
    {
        DBG_INFO(("Buffer is OK\r\n"));
        b_has_multbyte_split = FALSE;
    }

    return b_has_multbyte_split;
}



