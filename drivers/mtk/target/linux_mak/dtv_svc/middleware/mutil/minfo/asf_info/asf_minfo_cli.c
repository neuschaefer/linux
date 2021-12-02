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
 * $RCSfile: asf_minfo_cli.c,v $
 * $Revision:
 * $Date:
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_ATSC/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 37ebbb2d76fad2e8a90655e5f2bf2c3a $
 *
 * Description:
 *         This file implements get/set debug level APIs for media info.
 *---------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/
#ifndef __KERNEL__

#include "inc/x_common.h"
#include "mutil/minfo/u_minfo.h"
#include "mutil/minfo/minfo.h"
#include "mutil/minfo/x_minfo.h"
#include "handle/handle.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#include "file_mngr/u_fm.h"
#endif

#include "cli/x_cli.h"
#include "dbg/x_dbg.h"
#include "asf_parser_common.h"
#include "asf_minfo_parser.h"
#include "asf_minfo_dump.h"
#include "asf_minfo_cli.h"
#include "asf_minfo.h"

#else

#include "x_common.h"
#include "../u_minfo.h"
#include "../minfo.h"
#include "../x_minfo.h"
#include "handle.h"

#include "../../mm_util.h"

#include "x_cli.h"
#include "x_dbg.h"
#include "asf_parser_common.h"
#include "asf_minfo_parser.h"
#include "asf_minfo_dump.h"
#include "asf_minfo_cli.h"
#include "asf_minfo.h"

#endif

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
 
#ifdef CLI_LVL_ALL
static INT32 minfo_cli_asf_fopen(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_asf_mopen(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_asf_getnum(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_asf_getinfo(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_asf_close(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _minfo_cli_set_dump_flag(INT32 i4_argc, const CHAR** pps_argv);
static INT32 _minfo_cli_dump_object(INT32 i4_argc, const CHAR** pps_argv);
static INT32 minfo_cli_asf_stress_test(INT32  i4_argc, const CHAR** pps_argv);
#if 0
static INT32 minfo_cli_asf_wmv_supported(INT32 i4_argc, const CHAR** pps_argv);
extern INT32 _asf_wmv_supported_test( MINFO_OBJ_T*  pt_minfo_obj,
                                      UINT64        ui8_data_packet_begin,
                                      UINT32        ui4_per_packet_size,
                                      BOOL*         pb_supported);
#endif

/* asf minfo command table */
static CLI_EXEC_T at_asf_minfo_cmd_tbl[] =
{
    {
        "dump obj switch",       "setdump",           _minfo_cli_set_dump_flag,        
        NULL,                    "set_dump_flag",     CLI_SUPERVISOR
    },
    {
        "fopen",                 NULL,                minfo_cli_asf_fopen,        
        NULL,                    "minfo_fopen",       CLI_SUPERVISOR
    },
    {
        "mopen",                NULL,                 minfo_cli_asf_mopen,        
        NULL,                   "minfo_mopen",        CLI_SUPERVISOR
    },
    {
        "dump object info ",    "dump",               _minfo_cli_dump_object,        
        NULL,                   "dump_obj",           CLI_SUPERVISOR
    },
    {
        "get record num",       "getnum",             minfo_cli_asf_getnum,     
        NULL,                   "minfo_get_num_recs", CLI_SUPERVISOR
    },
    {
        "get record info",      "getinfo",            minfo_cli_asf_getinfo,    
        NULL,                   "minfo_getinfo",      CLI_SUPERVISOR
    },
    {
        "close",                NULL,                 minfo_cli_asf_close,        
        NULL,                   "minfo_close",        CLI_SUPERVISOR
    },
    {
        "mstress",              NULL,                 minfo_cli_asf_stress_test,        
        NULL,                   "minfo_stress_test",  CLI_SUPERVISOR
    },
    #if 0
    {
        "wmvtest",              NULL,                 minfo_cli_asf_wmv_supported,        
        NULL,                   "minfo_wmv_test",     CLI_SUPERVISOR
    },
    #endif
        
    END_OF_CLI_CMD_TBL
};

/*asf minfo handle*/
static  HANDLE_T gh_minfo_asf = NULL_HANDLE;
static BOOL   gb_dump_obj_flg = FALSE;
static ASF_CLIP_OBJ* gpt_clip_obj = NULL;  


/*-----------------------------------------------------------------------------
                    function declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Name: _minfo_cli_get_dump_flag
 *
 * Description: get dump flag, if true dump objects
 *
 * Inputs:  NULL
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK           Routine successful.
 *          other non values  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
BOOL  _minfo_cli_get_dump_flag(VOID)
{
    return gb_dump_obj_flg;
}


/*-----------------------------------------------------------------------------
 * Name: _minfo_cli_set_dump_flag
 *
 * Description: set dump switch ,if set when openning a file it will dummp all
                parsed objects
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK           Routine successful.
 *          other non values  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
static INT32 _minfo_cli_set_dump_flag( INT32        i4_argc,
                                       const CHAR** pps_argv )
{  

    /*parameters check, 2 parameters*/
    if ((2 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        DBG_INFO(("usage:dump flag(0/1)\r\n"));
        return CLIR_INV_ARG;
    }
    
    if (0 == x_strncmp(pps_argv[1], "?", (SIZE_T)1))
    {
        DBG_INFO(("usage:dump flag(0/1)\r\n"));
        return CLIR_OK;
    }
    
    if (0 == x_strncmp(pps_argv[1], "1", (SIZE_T)1))
    {
        gb_dump_obj_flg = TRUE;
    }
    else
    {
        gb_dump_obj_flg = FALSE;
    }
        
    DBG_INFO(("set dump flag = %d  successfully!\r\n", gb_dump_obj_flg));
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _minfo_cli_asf_nfy_fct
 *
 * Description: simulates the client's event callback function
 *
 * Inputs:  
 *         h_minfo,   Specifies which handle will be notified.
 *         e_event,   it identifies the event type of notification. 
 *         pv_data1,  specify the notification data1 which is followed with 
 *                    the notification event.
 *         pv_data2,  specify the notification data2 which is followed with 
 *                    the notification event.   
 *         pv_tag,    The client data specified by the client when callback
 *                    has been called, the callback function will send it again.            
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK           Routine successful.
 *          other non values  Routine unsuccessful.
 ----------------------------------------------------------------------------*/

 static VOID _minfo_cli_asf_nfy_fct (
                        HANDLE_T                    h_minfo,  
                        MINFO_NFY_EVENT_TYPE_T      e_event, 
                        const VOID*                 pv_data1, 
                        const VOID*                 pv_data2, 
                        const VOID*                 pv_tag)   
{
    CHAR *apsz_parse_Info[] = { 
                                "PARSING DONE", 
                                "HANDLE CLOSED",   
                                "ERROR"
                            };

    
    x_dbg_stmt("minfo_cli_asf_nfy_fct:Event type=%s\r\n", 
              apsz_parse_Info[e_event]);
    return;
}

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_asf_fopen
 *
 * Description: open a file ready for parsing a as file
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 
 * Note: usage:  fopen initflag filename formatflag
 
 * Returns: CLIR_OK           Routine successful.
 *          other non values  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
static INT32 minfo_cli_asf_fopen( INT32        i4_argc,
                                  const CHAR** pps_argv )
{
    INT32           i4_ret;
    CHAR*           ps_path_name;
    MEDIA_FORMAT_T  t_format;

    /*parameters check, 3 parameters*/
    if ((3 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        DBG_INFO(("usage:fopen filename formatflag(0:unknown/1:asf)\r\n"));
        return CLIR_INV_ARG;
    }

    /*get filename*/
    if (NULL == pps_argv[1])
    {
        DBG_ERROR(("usage:fopen initflag(0/1) filename formatflag(0/1)\r\n"));
        return CLIR_INV_ARG;
    }
    else
    {
        ps_path_name = (CHAR*)pps_argv[1];
    }

    /*get format info*/
    if (0 == x_strncmp(pps_argv[2], "1", (SIZE_T)1))
    {
        t_format.e_media_type = MEDIA_TYPE_CONTAINER;
        t_format.t_media_subtype.u.e_contnr_subtype 
                              = MEDIA_CONTNR_SUBTYPE_ASF;
    }
    else
    {
        t_format.e_media_type = MEDIA_TYPE_UNKNOWN;
        t_format.t_media_subtype.u.e_contnr_subtype 
                              = MEDIA_CONTNR_SUBTYPE_UNKNOWN;
    }
    /*construct parameter according to cmd parameters*/

    /*call x_minfo_fopen*/
    i4_ret = x_minfo_fopen(
                           ps_path_name, 
                           _minfo_cli_asf_nfy_fct,        
                           NULL,        
                           &t_format,
                           &gh_minfo_asf);
    if (MINFOR_OK != i4_ret )
    {
        DBG_ERROR(("Failed to open file(%s)!,i4_ret=%d\r\n", 
                   ps_path_name, i4_ret));
        return CLIR_CMD_EXEC_ERROR;
    }


    DBG_INFO(("Open file(%s) successfully!\r\n", ps_path_name));
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_asf_mopen
 *
 * Description: open a memory clip
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK           Routine successful.
 *          other non values  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
static INT32 minfo_cli_asf_mopen( INT32        i4_argc,
                                  const CHAR** pps_argv)
{
    INT32           i4_ret;
    CHAR*           ps_path_name;
    MEDIA_FORMAT_T  t_format;
    ASF_CLIP_OBJ*   pt_clip_obj;  
    
    /*parameters check, 3 parameters*/
    if ((4 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        DBG_INFO(("usage:fopen filename formatflag (0:unknown/1:asf) "
                  "zero_len(0/1)\r\n"));
        return CLIR_INV_ARG;
    }

    /*get filename*/
    if (NULL == pps_argv[1])
    {
        DBG_ERROR(("usage:fopen initflag(0/1) filename formatflag(0/1)"
                   "zero_len(0/1)\r\n"));
        return CLIR_INV_ARG;
    }
    else
    {
        ps_path_name = (CHAR*)pps_argv[1];
    }

    /*get format info*/
    if (0 == x_strncmp(pps_argv[2], "1", (SIZE_T)1))
    {
        t_format.e_media_type = MEDIA_TYPE_CONTAINER;
        t_format.t_media_subtype.u.e_contnr_subtype 
                              = MEDIA_CONTNR_SUBTYPE_ASF;
    }
    else
    {
        t_format.e_media_type = MEDIA_TYPE_UNKNOWN;
        t_format.t_media_subtype.u.e_contnr_subtype 
                              = MEDIA_CONTNR_SUBTYPE_UNKNOWN;
    }

    /*get clip object*/
    i4_ret = _cli_asf_get_obj(ps_path_name, &pt_clip_obj);
    if (ASFR_CLIP_OK != i4_ret)
    {
        DBG_ERROR(("Fail to get clip object!\r\n"));
        return CLIR_INV_ARG;
    }
    gpt_clip_obj = pt_clip_obj;

    /*get zero len info call x_minfo_fopen*/
    if (0 == x_strncmp(pps_argv[3], "0", (SIZE_T)1))
    {
        i4_ret =  x_minfo_mopen ((VOID* )pt_clip_obj,
                                  0, 
                                  _minfo_cli_asf_nfy_fct,        
                                  NULL,        
                                  &t_format,
                                  &gh_minfo_asf);
    }
    else
    {
        i4_ret =  x_minfo_mopen ( pt_clip_obj->pv_clip_buf,
                                  (SIZE_T)pt_clip_obj->i8_clip_len, 
                                  _minfo_cli_asf_nfy_fct,        
                                   NULL,        
                                  &t_format,
                                  &gh_minfo_asf);
    }

    if (MINFOR_OK != i4_ret )
    {
        DBG_ERROR(("Failed to open file(%s)!,i4_ret=%d\r\n", 
                   ps_path_name, i4_ret));
    }
    else
    {
        DBG_INFO(("Open file(%s) successfully!\r\n", ps_path_name));
    }
    
    return i4_ret;
}

/*-----------------------------------------------------------------------------
 * Name: _minfo_cli_dump_object
 *
 * Description: dump given ID object
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK           Routine successful.
 *          other non values  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
static INT32 _minfo_cli_dump_object(INT32        i4_argc,
                                    const CHAR** pps_argv)
{  
    ASF_OBJ_TYPE_E  e_obj_type;
    HANDLE_TYPE_T   e_handle_type;
    VOID*           pv_obj;
    INT32           i4_ret;
    
    /*parameters check, 4 parameters*/
    if ((2 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        DBG_INFO(("usage:dump object_ID\r\n"));
        DBG_INFO(("     dump %d:dump all objects \r\n", ASF_OBJECT_BUTT));
        _asf_current_supported_object();
        return CLIR_INV_ARG;
    }
    
    if (0 == x_strncmp(pps_argv[1], "?", (SIZE_T)1))
    {
        DBG_INFO(("usage:dump object_ID \r\n"));
        DBG_INFO(("     dump %d:dump all objects \r\n", ASF_OBJECT_BUTT));
        _asf_current_supported_object();
        return CLIR_OK;
    }

    e_obj_type = (ASF_OBJ_TYPE_E)x_strtoll(pps_argv[1], NULL, 10);
    DBG_INFO(("_minfo_cli_dump_object:ID=%d\r\n", e_obj_type));

    i4_ret = handle_get_type_obj(gh_minfo_asf, &e_handle_type, &pv_obj);
    if ((i4_ret != HR_OK) 
        || ((e_handle_type != HT_MINFO_FILE_TYPE) 
             && (e_handle_type != HT_MINFO_MEM_TYPE))
        || (pv_obj == NULL))
    {
        DBG_ERROR(("handle_get_type_obj return %d\r\n", i4_ret));
        return CLIR_CMD_EXEC_ERROR;
    }

    /*search the object node according to object_ID*/
    _asf_cli_dump_object((MINFO_OBJ_T*)pv_obj, e_obj_type);
        
    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: minfo_cli_asf_close
 *
 * Description: close asf minfo
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK           Routine successful.
 *          other non values  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
static INT32 minfo_cli_asf_close(INT32        i4_argc,
                                 const CHAR** pps_argv)
{  
    INT32 i4_ret;

    /*parameters check*/

    /*Do*/
    
    i4_ret = x_minfo_close(gh_minfo_asf);
    /*free clip object*/
    _cli_asf_free_obj(&gpt_clip_obj);

    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("asf closed error=%d\r\n", i4_ret));
        return CLIR_CMD_EXEC_ERROR;
    }
    else
    {
        DBG_INFO(("asf closed successfully!\r\n"));
        return CLIR_OK;
    }
    
}


/*-----------------------------------------------------------------------------
 * Name: minfo_cli_asf_getnum
 *
 * Description: get minfo records number
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK           Routine successful.
 *          other non values  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
static INT32 minfo_cli_asf_getnum(INT32        i4_argc,
                                  const CHAR** pps_argv)
{ 
    MINFO_INFO_TYPE_T  e_info_type;
    UINT16             ui2_num_recs;
    INT32              i4_ret;
    CHAR               c_type;

    /*parameters check*/
    if ((2 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        DBG_INFO(("usage:getnum A/V/G\r\n"));
        DBG_INFO(("      A:Audio; V:Video; G:Generic; I:Index\r\n"));
        return CLIR_INV_ARG;
    }


    if (NULL == pps_argv[1])
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        return CLIR_INV_ARG;
    }
    
    /*get minfo type*/
    c_type = pps_argv[1][0];
    switch (c_type)
    {
        case 'V':
            e_info_type = MINFO_INFO_TYPE_VIDEO_ATTR;
            break;
        case 'A':
            e_info_type = MINFO_INFO_TYPE_AUDIO_ATTR;
            break;
        case 'G':
            e_info_type = MINFO_INFO_TYPE_GENERIC;
            break;
        case 'I':
            e_info_type =  MINFO_INFO_TYPE_IDX_INFO;
            break;
        default:
            e_info_type = MINFO_INFO_TYPE_UNKNOWN;
            break;
    }

    /*get minfo number*/
    i4_ret = x_minfo_get_num_recs( gh_minfo_asf,
                                   e_info_type,
                                   &ui2_num_recs);
    if (MINFOR_OK == i4_ret)
    {
        DBG_INFO(("getnum steam_type=%d successfully num=%u\r\n", 
                  e_info_type,
                  ui2_num_recs));
    }
    else
    {
        DBG_INFO(("getnum stream_type=%d unsuccessfully(%d)(A/V/G)\r\n", 
                  e_info_type,
                  i4_ret));
    }


    return CLIR_OK;
}



/*-----------------------------------------------------------------------------
 * Name: minfo_cli_asf_getinfo
 *
 * Description: get minfo records info
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK           Routine successful.
 *          other non values  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
static INT32 minfo_cli_asf_getinfo(INT32        i4_argc,
                                    const CHAR** pps_argv)
{ 
    MINFO_INFO_TYPE_T  e_info_type;
    INT32              i4_ret;
    CHAR               c_type;
    UINT16             ui2_index;
    MINFO_INFO_T       t_minfo_rec;
    SIZE_T             t_size;

    
    /*parameters check*/
    if ((3 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        DBG_INFO(("usage:getinfo A/V/G count\r\n"));
        DBG_INFO(("      A:Audio; V:Video; G:Generic; I:Index\r\n"));
        return CLIR_INV_ARG;
    }


    if (NULL == pps_argv[1])
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        return CLIR_INV_ARG;
    }
    
    /*get minfo type*/
    c_type = pps_argv[1][0];
    switch (c_type)
    {
        case 'V':
            e_info_type = MINFO_INFO_TYPE_VIDEO_ATTR;
            break;
        case 'A':
            e_info_type = MINFO_INFO_TYPE_AUDIO_ATTR;
            break;
        case 'G':
            e_info_type = MINFO_INFO_TYPE_GENERIC;
            break;
        case 'I':
            e_info_type =  MINFO_INFO_TYPE_IDX_INFO;
            break;
        default:
            e_info_type = MINFO_INFO_TYPE_UNKNOWN;
            break;
    }

    /*init 0*/
    t_size = sizeof(MINFO_INFO_T);
    x_memset(&t_minfo_rec, 0, t_size);

    /*get count */
    ui2_index = (UINT16)x_strtoll(pps_argv[2], NULL, 10);
    
    i4_ret = x_minfo_get_info ( gh_minfo_asf,    
                                e_info_type,
                                ui2_index,          
                                (VOID*)&t_minfo_rec,       
                                &t_size);
    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("asf_minfo_get_info return %d\r\n", i4_ret));
        return MINFOR_INTERNAL_ERR;
    }

    /*show obtainted info*/
    _minfo_dump_minfo_rec(&t_minfo_rec);
    return CLIR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: minfo_cli_asf_stress_test
 *
 * Description:  asf minfo module stress test command. This function will loop
 *               vocating minfo fopen/getnum/getinfo/close for different
 *               type file. 
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK           Routine successful.
 *          other non values  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
static INT32 minfo_cli_asf_stress_test(INT32        i4_argc,
                                       const CHAR** pps_argv)
{  
    INT32           i4_ret;
    UINT32          ui4_i;
    UINT32          ui4_j;
    CHAR            ac_file_name[35];
    UINT32          ui4_count;
    CHAR*           ppc_argv[4];
    UINT16          ui2_number;
    MINFO_INFO_T    t_minfo_rec;
    SIZE_T          t_size;
    UINT32          ui4_idx;
    UINT32          ui4_loop_count;
    BOOL            b_dead_loop;
    
    /*parameters check*/
    /*parameters check*/
    if ((2 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        DBG_INFO(("usage:mstress count\r\n"));
        return CLIR_INV_ARG;
    }

    /*Do*/
    /*get loop count*/
    ui4_loop_count = (UINT32)x_strtoll(pps_argv[1], NULL, 10);
    /*if 0 set dead loop*/
    if (0 == ui4_loop_count)
    {
        b_dead_loop = TRUE;
    }
    else
    {
        b_dead_loop = FALSE;
    }
    
    /*while loop */
    while(TRUE)
    {
        for (ui4_i = 1 ; ui4_i < 5; ++ui4_i)
        {
           for (ui4_j = 0 ; ui4_j <= 99; ++ui4_j)
            {
                /*1.get filename******************************************/
                ui4_count = 0;
                if (1 == ui4_i)
                {
                    ui4_count += x_sprintf(ac_file_name, 
                                           "/mnt/usb/Mass-000/%d.wmv", 
                                           (100 * ui4_i + ui4_j));
                }
                else if (2 == ui4_i)
                {
                    ui4_count += x_sprintf(ac_file_name, 
                                           "/mnt/usb/Mass-000/%d.wma", 
                                           (100 * ui4_i + ui4_j));
                }
                else if (3 == ui4_i)
                {
                    ui4_count += x_sprintf(ac_file_name, 
                                           "/mnt/usb/Mass-000/%d.asf", 
                                           (100 * ui4_i + ui4_j));
                }
                else
                {
                    ui4_count += x_sprintf(ac_file_name, 
                                           "/mnt/usb/Mass-000/%d.avi", 
                                           (100 * ui4_i + ui4_j));
                }
                
                if (ui4_count > 33) /*max name len buffer 35*/
                {
                    DBG_ERROR(("internal error!\r\n"));
                    return CLIR_INV_ARG;
                }
                ac_file_name[ui4_count] = 0;
                
                /*2.fopen***************************************************/
                /*set dump*/
                gb_dump_obj_flg = TRUE;
                if (NULL == ac_file_name)
                {
                    DBG_ERROR(("file name null error!\r\n"));
                    return CLIR_INV_ARG;
                }
                
                ppc_argv[1] = "fopen";
                ppc_argv[1] = ac_file_name;
                ppc_argv[2] = "0";
                i4_ret = minfo_cli_asf_fopen(3, (const CHAR**)ppc_argv);
                if (MINFOR_OK != i4_ret)
                {
                    if (MINFOR_FILE_ERR == i4_ret)
                    {
                        DBG_INFO(("asf open error=%s does not exist\r\n",
                                  ac_file_name));
                    }
                    else
                    {
                        DBG_ERROR(("asf open error=%d\r\n", i4_ret));
                    }
                    continue;
                }

                DBG_INFO(("Begin File: %s\r\n", ac_file_name));
                
                /*3.get num and getinfo**********************************/
                /*1) get generic num should be 1*/
                i4_ret = x_minfo_get_num_recs( gh_minfo_asf,
                                               MINFO_INFO_TYPE_GENERIC,
                                               &ui2_number);
                if (MINFOR_OK == i4_ret)
                {
                    if (1 != ui2_number)
                    {
                        DBG_INFO(("getnum Generic unsuccessfully num=%u\r\n", 
                                  ui2_number));
                    }
                    else
                    {
                        DBG_INFO(("getnum Generic successfully\r\n"));
                    }
                }
                else
                {
                    DBG_INFO(("getnum Generic unsuccessfully(%d)\r\n", 
                              i4_ret));
                }
                
                t_size = sizeof(MINFO_INFO_T);
                x_memset(&t_minfo_rec, 0, t_size);
                i4_ret = x_minfo_get_info( gh_minfo_asf,    
                                           MINFO_INFO_TYPE_GENERIC,
                                           1,          
                                           (VOID*)&t_minfo_rec,       
                                           &t_size);
                if (MINFOR_OK != i4_ret)
                {
                    DBG_ERROR(("minfo_cli_asf_stress_test return %d "
                               "for generic \r\n", 
                               i4_ret));
                    return MINFOR_INTERNAL_ERR;
                }
                /*show obtainted info*/
                DBG_INFO(("Genric Info:\r\n"));
                _minfo_dump_minfo_rec(&t_minfo_rec);
                

                /*2) get Video num and info*/
                i4_ret = x_minfo_get_num_recs( gh_minfo_asf,
                                               MINFO_INFO_TYPE_VIDEO_ATTR,
                                               &ui2_number);
                if (MINFOR_OK == i4_ret)
                {
                    DBG_INFO(("getnum Video successfully num=%u\r\n", 
                              ui2_number));
                }
                else
                {
                    DBG_INFO(("getnum Video unsuccessfully(%d)\r\n", 
                              i4_ret));
                }
                /*get info*/
                for (ui4_idx = 1 ; ui4_idx < ui2_number; ++ui4_idx)
                {
                    t_size = sizeof(MINFO_INFO_T);
                    x_memset(&t_minfo_rec, 0, t_size);
                    i4_ret = x_minfo_get_info( gh_minfo_asf,    
                                               MINFO_INFO_TYPE_VIDEO_ATTR,
                                               (MINFO_REC_IDX_T)ui4_idx,          
                                               (VOID*)&t_minfo_rec,       
                                               &t_size);
                    if (MINFOR_OK != i4_ret)
                    {
                        DBG_ERROR(("minfo_cli_asf_stress_test return %d "
                                   "for video\r\n", 
                                   i4_ret));
                        return MINFOR_INTERNAL_ERR;
                    }
                    /*show obtainted info*/
                    DBG_INFO(("Video Count[%d] Info:\r\n", ui4_idx));
                    _minfo_dump_minfo_rec(&t_minfo_rec);
                }

                /*3)get audio num and info*/

                i4_ret = x_minfo_get_num_recs( gh_minfo_asf,
                                               MINFO_INFO_TYPE_AUDIO_ATTR,
                                               &ui2_number);
                if (MINFOR_OK == i4_ret)
                {
                    DBG_INFO(("getnum Audio successfully num=%u\r\n", 
                              ui2_number));
                }
                else
                {
                    DBG_INFO(("getnum Audio unsuccessfully(%d)\r\n", 
                              i4_ret));
                }
                /*get info*/
                for (ui4_idx = 1 ; ui4_idx < ui2_number; ++ui4_idx)
                {
                    t_size = sizeof(MINFO_INFO_T);
                    x_memset(&t_minfo_rec, 0, t_size);
                    i4_ret = x_minfo_get_info( gh_minfo_asf,    
                                               MINFO_INFO_TYPE_AUDIO_ATTR,
                                               (MINFO_REC_IDX_T)ui4_idx,          
                                               (VOID*)&t_minfo_rec,       
                                               &t_size);
                    if (MINFOR_OK != i4_ret)
                    {
                        DBG_ERROR(("minfo_cli_asf_stress_test return %d "
                                   "for audio\r\n", 
                                   i4_ret));
                        return MINFOR_INTERNAL_ERR;
                    }
                    /*show obtainted info*/
                    DBG_INFO(("Audio Count[%d] Info:\r\n", ui4_idx));
                    _minfo_dump_minfo_rec(&t_minfo_rec);
                }

                /*4)get index info*/

                i4_ret = x_minfo_get_num_recs( gh_minfo_asf,
                                               MINFO_INFO_TYPE_IDX_INFO,
                                               &ui2_number);
                if (MINFOR_OK == i4_ret)
                {
                    if (1 != ui2_number)
                    {
                        DBG_INFO(("getnum Index Info unsuccessfully num=%u\r\n", 
                                  ui2_number));
                    }
                    else
                    {
                        DBG_INFO(("getnum Index Info successfully\r\n"));
                    }
                }
                else
                {
                    DBG_INFO(("getnum Index Info unsuccessfully(%d)\r\n", 
                              i4_ret));
                }
                
                t_size = sizeof(MINFO_INFO_T);
                x_memset(&t_minfo_rec, 0, t_size);
                i4_ret = x_minfo_get_info( gh_minfo_asf,    
                                           MINFO_INFO_TYPE_IDX_INFO,
                                           1,          
                                           (VOID*)&t_minfo_rec,       
                                           &t_size);
                if (MINFOR_OK != i4_ret)
                {
                    DBG_ERROR(("minfo_cli_asf_stress_test return %d "
                               "for Index Info\r\n", 
                               i4_ret));
                    return MINFOR_INTERNAL_ERR;
                }
                /*show obtainted info*/
                DBG_INFO(("Index Info:\r\n"));
                _minfo_dump_minfo_rec(&t_minfo_rec);
                

                /*4.close***********************************************/
                i4_ret = x_minfo_close(gh_minfo_asf);
                if (MINFOR_OK != i4_ret)
                {
                    DBG_ERROR(("asf closed error=%d\r\n", i4_ret));
                    return CLIR_CMD_EXEC_ERROR;
                }

                DBG_INFO(("End File: %s\r\n", ac_file_name));
            
            }
        }
        
        if (FALSE == b_dead_loop)
        {
            --ui4_loop_count;
            if (0 == ui4_loop_count)
            {
                break;
            }
        }

        
    }

    return CLIR_OK;
}

#if 0
/*-----------------------------------------------------------------------------
 * Name: minfo_cli_asf_wmv_supported
 *
 * Description: Test whether support wmv file playback
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK           Routine successful.
 *          other non values  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
static INT32 minfo_cli_asf_wmv_supported(INT32        i4_argc,
                                         const CHAR** pps_argv)
{  
    INT32                      i4_ret;
    HANDLE_TYPE_T              e_handle_type;
    MINFO_OBJ_T*               pt_minfo_obj;

    setpos_fct                 pf_set_pos;    
    copybytes_fct              pf_copybyte;
    getpos_fct                 pf_get_pos;

    UINT64                     ui8_data_packet_begin;
    UINT32                     ui4_per_packe_size;
    BOOL                       b_wmv_supported;
    
    /*parameters check, 4 parameters*/
    if ((3 != i4_argc) || (NULL == pps_argv))
    {
        DBG_ERROR(("Invalid argument!\r\n"));
        DBG_INFO(("usage:wmvtest packet_offset  packet_size\r\n"));
        return CLIR_INV_ARG;
    }

    /*get packet offset*/
    ui8_data_packet_begin = (UINT64)x_strtoll(pps_argv[1], NULL, 10);
    /*get packet size*/
    ui4_per_packe_size = (UINT32)x_strtoll(pps_argv[2], NULL, 10);
    
    /*============================Do============================*/
    i4_ret = handle_get_type_obj(gh_minfo_asf, 
                                 &e_handle_type, 
                                 (VOID**)(&pt_minfo_obj));
    if ((i4_ret != HR_OK) 
        || ((e_handle_type != HT_MINFO_FILE_TYPE) 
             && (e_handle_type != HT_MINFO_MEM_TYPE))
        || (pt_minfo_obj == NULL))
    {
        DBG_ERROR(("handle_get_type_obj return %d\r\n", i4_ret));
        return CLIR_CMD_EXEC_ERROR;
    }


    pf_set_pos = pt_minfo_obj->t_input_fct_tbl.pf_set_pos;
    pf_copybyte = pt_minfo_obj->t_input_fct_tbl.pf_copybytes;
    pf_get_pos = pt_minfo_obj->t_input_fct_tbl.pf_get_pos;

    if ((NULL == pf_set_pos) 
        || (NULL == pf_copybyte)
        || (NULL == pf_get_pos))
    {
        DBG_ERROR(("NULL input\r\n"));
        return CLIR_CMD_EXEC_ERROR;
    }
    
    b_wmv_supported = FALSE;
    i4_ret = _asf_wmv_supported_test( pt_minfo_obj,
                                      ui8_data_packet_begin,
                                      ui4_per_packe_size,
                                      &b_wmv_supported);
    if (MINFOR_OK != i4_ret)
    {
        DBG_ERROR(("_asf_wmv_supported_test error\r\n"));
        return MINFOR_INTERNAL_ERR;
    }
    
    if(FALSE != b_wmv_supported)
    {
        DBG_INFO(("codec support wmv8 intra\r\n"));
    }
    else
    {
        DBG_INFO(("codec doesn't support wmv8 intra\r\n"));
        
    }
    
    return CLIR_OK;
    
}
#endif

#endif

/*-----------------------------------------------------------------------------
 * Name: asf_minfo_cli_init
 *
 * Description: This API initializes asf Minfo CLI component.
 *
 * Inputs:  NULL
 *
 * Outputs: NULL
 *
 * Returns: CLIR_OK           Routine successful.
 *          other non values  Routine unsuccessful.
 ----------------------------------------------------------------------------*/
INT32 asf_minfo_cli_init(CLI_EXEC_T **ppt_command_array)
{
#ifdef CLI_LVL_ALL
    *ppt_command_array = at_asf_minfo_cmd_tbl;
#endif
    return CLIR_OK;
}

#ifdef __cplusplus
    }
#endif


