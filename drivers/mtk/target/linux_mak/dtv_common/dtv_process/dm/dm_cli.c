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
 * $RCSfile: dm_cli.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 * $SWAuthor: $
 *
 * Description: 
 *         This file contains all the transition effect interface APIs  
 *---------------------------------------------------------------------------*/

#include "dm_cli.h"
#include "dm_util.h"

#include "dbg/u_dbg.h"
#include "inc/common.h"
#include "os/inc/x_os.h"
#include "cli/x_cli.h"
#include "util/x_lnk_list.h"

#include "dm/u_dm.h"
#include "dm/x_dm.h"
#include "file_mngr/x_fm.h"
#include "res_mngr/x_rm_dev_types.h"
#ifdef DLNA_SUPPORT
#include "inet/dlna/x_dlna_dmp_api.h"
#include "dm/x_dm_dlna.h"
#endif

#include <stdlib.h>

#ifdef CLI_LVL_ALL
typedef struct __DM_NFY_TEST_T
{
    HANDLE_T h_nfy;

    DM_HW_TYPE_T t_hw_type;
    DM_DEV_TYPE_T t_dev_type;

    SLIST_ENTRY_T(__DM_NFY_TEST_T) t_link;    
} _DM_NFY_TEST_T;

typedef struct __DM_TAG_TEST_T
{
    UINT32 ui4_a;
    UINT32 ui4_b;
} _DM_TAG_TEST_T;

static SLIST_T(__DM_NFY_TEST_T) _t_nfy_test_list;

#define DM_CLI_MAX_DEV_STAT_NS    20

static DM_DEV_STAT_T at_dev_stat[DM_CLI_MAX_DEV_STAT_NS];

static INT32 _dm_cli_get_dbg_lvl(
    INT32 i4_argc, 
    const CHAR** pps_argv);

static INT32 _dm_cli_set_dbg_lvl(
    INT32 i4_argc, 
    const CHAR** pps_argv);

static INT32 _dm_cli_reg_nfy(
    INT32 i4_argc, 
    const CHAR** pps_argv);

static INT32 _dm_cli_unreg_nfy(
    INT32 i4_argc, 
    const CHAR** pps_argv);

static INT32 _dm_cli_get_device_info(
    INT32 i4_argc, 
    const CHAR** pps_argv);

static INT32 _dm_cli_get_devices(
    INT32 i4_argc, 
    const CHAR** pps_argv);

static INT32 _dm_cli_get_usb_port_num(
    INT32 i4_argc, 
    const CHAR** pps_argv);


#ifdef DLNA_SUPPORT
static INT32 _dm_cli_dmp_start(
    INT32 i4_argc, 
    const CHAR** pps_argv);

static INT32 _dm_cli_dmp_stop(
    INT32 i4_argc, 
    const CHAR** pps_argv);

static INT32 _dm_cli_dmp_suspend(
    INT32 i4_argc, 
    const CHAR** pps_argv);

static INT32 _dm_cli_dmp_resume(
    INT32 i4_argc, 
    const CHAR** pps_argv);
#endif

/* Device Manager command table */
static CLI_EXEC_T at_dm_cmd_tbl[] = 
{
    {CLI_GET_DBG_LVL_STR, NULL, _dm_cli_get_dbg_lvl, NULL, "get debug level", CLI_SUPERVISOR},
    {CLI_SET_DBG_LVL_STR, NULL, _dm_cli_set_dbg_lvl, NULL, "set debug level", CLI_SUPERVISOR},   
    {"reg_nfy",   NULL, _dm_cli_reg_nfy, NULL, "register notification", CLI_SUPERVISOR},
    {"unreg_nfy",   NULL, _dm_cli_unreg_nfy, NULL, "un-register notification", CLI_SUPERVISOR},
    {"get_device_info",   NULL, _dm_cli_get_device_info, NULL, "get device information", CLI_SUPERVISOR},
    {"get_devices",   NULL, _dm_cli_get_devices, NULL, "get devices", CLI_SUPERVISOR},
    {"usb_port_num",   NULL, _dm_cli_get_usb_port_num, NULL, "Get USB port number", CLI_SUPERVISOR},
#ifdef DLNA_SUPPORT
    {"dmp_start",   NULL, _dm_cli_dmp_start, NULL, "dlna dmp start", CLI_SUPERVISOR},
    {"dmp_stop", NULL, _dm_cli_dmp_stop, NULL, "dlna dmp stop", CLI_SUPERVISOR},
    {"dmp_suspend",   NULL, _dm_cli_dmp_suspend, NULL, "dlna dmp suspend", CLI_SUPERVISOR},
    {"dmp_resume",   NULL, _dm_cli_dmp_resume, NULL, "dlna dmp resume", CLI_SUPERVISOR},
#endif
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_dm_root_cmd_tbl[] =
{
	  {"dm",    NULL,   NULL,   at_dm_cmd_tbl,    "Device Manager commands",    CLI_SUPERVISOR},
	  END_OF_CLI_CMD_TBL
};
#endif

/*-----------------------------------------------------------------------------
 * Name: dm_cli_init
 *
 * Description: This API initializes Device Manager CLI component.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: DMR_OK                     Routine successful.
 *          DMR_CLI_ERROR              CLI operation is failed.
 ----------------------------------------------------------------------------*/
INT32 dm_cli_init(VOID)
{
#ifdef CLI_LVL_ALL
    INT32       i4_ret;    
    
    /* Attach Device Manager CLI command table to CLI */
    i4_ret = x_cli_attach_cmd_tbl(at_dm_root_cmd_tbl, CLI_CAT_MW, CLI_GRP_NONE);
    
    if (i4_ret != CLIR_OK)
    {
        return DMR_CLI_ERR;
    }

    SLIST_INIT(&(_t_nfy_test_list));

    return DMR_OK;
#else
    return DMR_OK;
#endif
}

#ifdef CLI_LVL_ALL

#ifdef DLNA_SUPPORT
static INT32 _dm_cli_dmp_start(
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    x_dm_dlna_dmp_start();
    return DMR_OK;
}

static INT32 _dm_cli_dmp_stop(
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    x_dm_dlna_dmp_stop();
    return DMR_OK;
}

static INT32 _dm_cli_dmp_suspend(
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    x_dm_dlna_dmp_suspend();
    return DMR_OK;
}

static INT32 _dm_cli_dmp_resume(
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    x_dm_dlna_dmp_resume();
    return DMR_OK;
}
#endif

static INT32 __dm_parse_int(const CHAR* psNum, INT32 *pi4_result)
{
    UINT32 ui4_cnt;
    INT32 i4_ret = CLIR_OK;

    if (pi4_result == NULL)
    {
        i4_ret = CLIR_INV_ARG;
        goto exit;
    }

    *pi4_result = 0;

    for (ui4_cnt = 0; ;ui4_cnt ++)
    {
        if (psNum[ui4_cnt] == 0)
        {
            goto exit;
        }
        else if ((psNum[ui4_cnt] > '9') || (psNum[ui4_cnt] < '0'))
        {
            i4_ret = CLIR_INV_ARG;
            goto exit;
        }

        *pi4_result *=10;
        *pi4_result += psNum[ui4_cnt] - '0';
    }

exit:

    return i4_ret;
}

static INT32 __dm_get_evt_cat(
    const CHAR *ps_arg,
    DM_EVT_CAT_T *pt_evt_cat)
{
    INT32 i4_ret;
    INT32 i4_cat;

    i4_ret = __dm_parse_int(ps_arg, &i4_cat);

    if (i4_ret != CLIR_OK)
    {
        return i4_ret;
    }

    switch (i4_cat)
    {
        case 0: 
        {
            *pt_evt_cat = DM_EVT_CAT_DEV;
            return CLIR_OK;
        }
        case 1:
        {
            *pt_evt_cat = DM_EVT_CAT_FS;
            return CLIR_OK;
        }
        case 2:
        {
            *pt_evt_cat = DM_EVT_CAT_LDR;
            return CLIR_OK;
        }
        default: 
            return CLIR_INV_ARG;
    }
}

static INT32 __dm_get_hw_type(
    const CHAR *ps_arg,
    DM_HW_TYPE_T *pt_hw_type)
{
    INT32 i4_ret;
    INT32 i4_type;

    i4_ret = __dm_parse_int(ps_arg, &i4_type);

    if (i4_ret != CLIR_OK)
    {
        return i4_ret;
    }

    switch (i4_type)
    {
        case 0: 
        {
            *pt_hw_type = DM_HW_TYPE_UNKNOWN;
            return CLIR_OK;
        }
        case 1:
        {
            *pt_hw_type = DM_HW_TYPE_EEPROM;
            return CLIR_OK;
        }
        case 2:
        {
            *pt_hw_type = DM_HW_TYPE_NAND;
            return CLIR_OK;
        }
        case 3:
        {
            *pt_hw_type = DM_HW_TYPE_NOR;
            return CLIR_OK;
        }
        case 4:
        {
            *pt_hw_type = DM_HW_TYPE_USB;
            return CLIR_OK;
        }
        case 5:
        {
            *pt_hw_type = DM_HW_TYPE_IDE;
            return CLIR_OK;
        }
        case 6:
        {
            *pt_hw_type = DM_HW_TYPE_FCI;
            return CLIR_OK;
        }
        case 7:
        {
            *pt_hw_type = DM_HW_TYPE_1394;
            return CLIR_OK;
        }
        default: 
            return CLIR_INV_ARG;
    }
}

static INT32 __dm_get_dev_type(
    const CHAR *ps_arg,
    DM_DEV_TYPE_T *pt_dev_type)
{
    INT32 i4_ret;
    INT32 i4_type;

    i4_ret = __dm_parse_int(ps_arg, &i4_type);

    if (i4_ret != CLIR_OK)
    {
        return i4_ret;
    }

    switch (i4_type)
    {
        case 0: 
        {
            *pt_dev_type = DM_DEV_UKW_TYPE_UNKNOWN;
            return CLIR_OK;
        }
        case 1:
        {
            *pt_dev_type = DM_DEV_SKT_TYPE_UNKNOWN;
            return CLIR_OK;
        }
        case 2:
        {
            *pt_dev_type = DM_DEV_MED_TYPE_UNKNOWN;
            return CLIR_OK;
        }
        case 3:
        {
            *pt_dev_type = DM_DEV_SKT_TYPE_EEPROM_HW;
            return CLIR_OK;
        }
        case 4:
        {
            *pt_dev_type = DM_DEV_SKT_TYPE_NAND_HW;
            return CLIR_OK;
        }
        case 5:
        {
            *pt_dev_type = DM_DEV_SKT_TYPE_NOR_HW;
            return CLIR_OK;
        }
        case 6:
        {
            *pt_dev_type = DM_DEV_SKT_TYPE_USB_HW;
            return CLIR_OK;
        }
        case 7: 
        {
            *pt_dev_type = DM_DEV_SKT_TYPE_IDE_HW;
            return CLIR_OK;
        }
        case 8:
        {
            *pt_dev_type = DM_DEV_SKT_TYPE_FCI_HW;
            return CLIR_OK;
        }
        case 9:
        {
            *pt_dev_type = DM_DEV_SKT_TYPE_1394_HW;
            return CLIR_OK;
        }
        case 10:
        {
            *pt_dev_type = DM_DEV_SKT_TYPE_HUB;
            return CLIR_OK;
        }
        case 11:
        {
            *pt_dev_type = DM_DEV_SKT_TYPE_CARD_READER;
            return CLIR_OK;
        }
        case 12:
        {
            *pt_dev_type = DM_DEV_SKT_TYPE_OPTI_DRV;
            return CLIR_OK;
        }
        case 13:
        {
            *pt_dev_type = DM_DEV_MED_TYPE_EEPROM;
            return CLIR_OK;
        }
        case 14:
        {
            *pt_dev_type = DM_DEV_MED_TYPE_NAND;
            return CLIR_OK;
        }
        case 15: 
        {
            *pt_dev_type = DM_DEV_MED_TYPE_NOR;
            return CLIR_OK;
        }
        case 16:
        {
            *pt_dev_type = DM_DEV_MED_TYPE_HDD;
            return CLIR_OK;
        }
        case 17:
        {
            *pt_dev_type = DM_DEV_MED_TYPE_DISC;
            return CLIR_OK;
        }
        case 18:
        {
            *pt_dev_type = DM_DEV_MED_TYPE_MEM_CARD;
            return CLIR_OK;
        }
        case 19:
        {
            *pt_dev_type = DM_DEV_MED_TYPE_MASS_STRG;
            return CLIR_OK;
        }
        default: 
            return CLIR_INV_ARG;
    }
}

VOID __dm_show_evt(
    DM_EVT_T t_evt)
{   
    switch(t_evt)
    {
        case DM_DEV_EVT_UNSUPPORT:
        {
            x_dbg_stmt("[CB]Receives event DM_DEV_EVT_UNSUPPORT.\n\r");
            return;
        }
         
        case DM_DEV_EVT_FAIL:
        {
            x_dbg_stmt("[CB]Receives event DM_DEV_EVT_FAIL.\n\r");
            return;
        }
        
        case DM_DEV_EVT_ATTACH:
        {
            x_dbg_stmt("[CB]Receives event DM_DEV_EVT_ATTACH.\n\r");
            return;
        }

        case DM_DEV_EVT_DETACH:
        {
            x_dbg_stmt("[CB]Receives event DM_DEV_EVT_DETACH.\n\r");
            return;
        }
 
        case DM_FS_EVT_MOUNTFAIL:
        {
            x_dbg_stmt("[CB]Receives event DM_FS_EVT_MOUNTFAIL.\n\r");
            return;
        }

        case DM_FS_EVT_MOUNTED:
        {
            x_dbg_stmt("[CB]Receives event DM_FS_EVT_MOUNTED.\n\r");
            return;
        }

        case DM_FS_EVT_UNMOUNTED:
        {
            x_dbg_stmt("[CB]Receives event DM_FS_EVT_UNMOUNTED.\n\r");
            return;
        }
 
        case DM_LDR_EVT_TRAY_ERROR:
        {
            x_dbg_stmt("[CB]Receives event DM_LDR_EVT_TRAY_ERROR.\n\r");
            return;
        }

        case DM_LDR_EVT_TRAY_OPEN:
        {
            x_dbg_stmt("[CB]Receives event DM_LDR_EVT_TRAY_OPEN.\n\r");
            return;
        }

        case DM_LDR_EVT_TRAY_CLOSE:
        {
            x_dbg_stmt("[CB]Receives event DM_LDR_EVT_TRAY_CLOSE.\n\r");
            return;
        }

        case DM_LDR_EVT_TRAY_OPENING:
        {
            x_dbg_stmt("[CB]Receives event DM_LDR_EVT_TRAY_OPENING.\n\r");
            return;
        }

        case DM_LDR_EVT_TRAY_CLOSING:
        {
            x_dbg_stmt("[CB]Receives event DM_LDR_EVT_TRAY_CLOSING.\n\r");
            return;
        }

        case DM_LDR_EVT_DISC_LOADING:
        {
            x_dbg_stmt("[CB]Receives event DM_LDR_EVT_DISC_LOADING.\n\r");
            return;
        }

        case DM_LDR_EVT_DISC_ABSENT:
        {
            x_dbg_stmt("[CB]Receives event DM_LDR_EVT_DISC_ABSENT.\n\r");
            return;
        }

        case DM_LDR_EVT_DISC_PRESENT:
        {
            x_dbg_stmt("[CB]Receives event DM_LDR_EVT_DISC_PRESENT.\n\r");
            return;
        }

        default:
            DM_ASSERT(0);
    }
}

VOID __dm_get_hw_str(
    DM_HW_TYPE_T t_hw_type,
    CHAR *ps_hw_type)
{
    switch (t_hw_type)
    {
        case DM_HW_TYPE_UNKNOWN:
        {
            x_strcpy(ps_hw_type, "UNKNOWN");
            return;
        }
        
        case DM_HW_TYPE_EEPROM:
        {
            x_strcpy(ps_hw_type, "EEPROM");
            return;
        }
        
        case DM_HW_TYPE_NAND:
        {
            x_strcpy(ps_hw_type, "NAND");
            return;
        }
        
        case DM_HW_TYPE_NOR:
        {
            x_strcpy(ps_hw_type, "NOR");
            return;
        }
        
        case DM_HW_TYPE_USB:
        {
            x_strcpy(ps_hw_type, "USB");
            return;
        }
        
        case DM_HW_TYPE_IDE:
        {
            x_strcpy(ps_hw_type, "IDE");
            return;
        }
        
        case DM_HW_TYPE_FCI:
        {
            x_strcpy(ps_hw_type, "FCI");
            return;
        }

        case DM_HW_TYPE_1394:
        {
            x_strcpy(ps_hw_type, "1394");
            return;
        }

        case DM_HW_TYPE_ETHERNET:
        {
            x_strcpy(ps_hw_type, "ETHERNET");
            return;
        }
        default:
            return;
    }
}

VOID __dm_get_dev_type_str(
    DM_DEV_TYPE_T t_dev_type,
    CHAR *ps_dev_type)
{
    switch (t_dev_type)
    {
        case DM_DEV_UKW_TYPE_UNKNOWN:
        {
            x_strcpy(ps_dev_type, "DM_DEV_UKW_TYPE_UNKNOWN");
            return;
        }
        case DM_DEV_SKT_TYPE_UNKNOWN:
        {
            x_strcpy(ps_dev_type, "DM_DEV_SKT_TYPE_UNKNOWN");
            return;
        }
        case DM_DEV_SKT_TYPE_EEPROM_HW:
        {
            x_strcpy(ps_dev_type, "DM_DEV_SKT_TYPE_EEPROM_HW");
            return;
        }
        case DM_DEV_SKT_TYPE_NAND_HW:
        {
            x_strcpy(ps_dev_type, "DM_DEV_SKT_TYPE_NAND_HW");
            return;
        }
        case DM_DEV_SKT_TYPE_NOR_HW:
        {
            x_strcpy(ps_dev_type, "DM_DEV_SKT_TYPE_NOR_HW");
            return;
        }
        case DM_DEV_SKT_TYPE_USB_HW:
        {
            x_strcpy(ps_dev_type, "DM_DEV_SKT_TYPE_USB_HW");
            return;
        }
        case DM_DEV_SKT_TYPE_IDE_HW:
        {
            x_strcpy(ps_dev_type, "DM_DEV_SKT_TYPE_IDE_HW");
            return;
        }
        case DM_DEV_SKT_TYPE_FCI_HW:
        {
            x_strcpy(ps_dev_type, "DM_DEV_SKT_TYPE_FCI_HW");
            return;
        }
        case DM_DEV_SKT_TYPE_1394_HW:
        {
            x_strcpy(ps_dev_type, "DM_DEV_SKT_TYPE_1394_HW");
            return;
        }
        case DM_DEV_SKT_TYPE_HUB:
        {
            x_strcpy(ps_dev_type, "DM_DEV_SKT_TYPE_HUB");
            return;
        }
        case DM_DEV_SKT_TYPE_CARD_READER:
        {
            x_strcpy(ps_dev_type, "DM_DEV_SKT_TYPE_CARD_READER");
            return;
        }
        case DM_DEV_SKT_TYPE_OPTI_DRV:
        {
            x_strcpy(ps_dev_type, "DM_DEV_SKT_TYPE_OPTI_DRV");
            return;
        }
        case DM_DEV_SKT_TYPE_ETHERNET:
        {
            x_strcpy(ps_dev_type, "DM_DEV_SKT_TYPE_ETHERNET");
            return;
        }
        case DM_DEV_MED_TYPE_UNKNOWN:
        {
            x_strcpy(ps_dev_type, "DM_DEV_MED_TYPE_UNKNOWN");
            return;
        }
        case DM_DEV_MED_TYPE_EEPROM:
        {
            x_strcpy(ps_dev_type, "DM_DEV_MED_TYPE_EEPROM");
            return;
        }
        case DM_DEV_MED_TYPE_NAND:
        {
            x_strcpy(ps_dev_type, "DM_DEV_MED_TYPE_NAND");
            return;
        }
        case DM_DEV_MED_TYPE_NOR:
        {
            x_strcpy(ps_dev_type, "DM_DEV_MED_TYPE_NOR");
            return;
        }
        case DM_DEV_MED_TYPE_HDD:
        {
            x_strcpy(ps_dev_type, "DM_DEV_MED_TYPE_HDD");
            return;
        }
        case DM_DEV_MED_TYPE_DISC:
        {
            x_strcpy(ps_dev_type, "DM_DEV_MED_TYPE_DISC");
            return;
        }
        case DM_DEV_MED_TYPE_MEM_CARD:
        {
            x_strcpy(ps_dev_type, "DM_DEV_MED_TYPE_MEM_CARD");
            return;
        }
        case DM_DEV_MED_TYPE_MASS_STRG:
        {
            x_strcpy(ps_dev_type, "DM_DEV_MED_TYPE_MASS_STRG");
            return;
        }
        case DM_DEV_MISC_TYPE_USB_BT:
        {
            x_strcpy(ps_dev_type, "DM_DEV_MISC_TYPE_USB_BT");
            return;
        }
        case DM_DEV_MED_TYPE_DLNA_DMS:
        {
            x_strcpy(ps_dev_type, "DM_DEV_MED_TYPE_DLNA_DMS");
            return;
        }
        case DM_DEV_MED_TYPE_SMB:
        {
            x_strcpy(ps_dev_type, "DM_DEV_MED_TYPE_SMB");
            return;
        }
        default:
            return;
    }
}

VOID __dm_get_card_str(
    DM_DEV_MED_CARD_TYPE_T t_card_type,
    CHAR *ps_card_type)
{
    switch (t_card_type)
    {
        case DM_DEV_MED_CARD_TYPE_CF:
        {
            x_strcpy(ps_card_type, "CF");
            return;
        }
        case DM_DEV_MED_CARD_TYPE_SD:
        {
            x_strcpy(ps_card_type, "SD");
            return;
        }
        case DM_DEV_MED_CARD_TYPE_MMC:
        {
            x_strcpy(ps_card_type, "MMC");
            return;
        }
        case DM_DEV_MED_CARD_TYPE_MS:
        {
            x_strcpy(ps_card_type, "MS");
            return;
        }
        case DM_DEV_MED_CARD_TYPE_MS_PRO:
        {
            x_strcpy(ps_card_type, "MS_PRO");
            return;
        }
        case DM_DEV_MED_CARD_TYPE_SM:
        {
            x_strcpy(ps_card_type, "SM");
            return;
        }
        case DM_DEV_MED_CARD_TYPE_XD:
        {
            x_strcpy(ps_card_type, "XD");
            return;
        }
        case DM_DEV_MED_CARD_TYPE_USB:
        {
            x_strcpy(ps_card_type, "USB_MASS");
            return;
        }
        default:
            return;
    }

}

VOID __dm_nfy_fct(
    DM_EVT_T t_evt, 
    VOID *pv_data, 
    VOID *pv_tag)
{
    _DM_NFY_TEST_T *pt_dm_nfy_test = (_DM_NFY_TEST_T *) pv_tag;

    CHAR ps_buf[32];
    UINT32 ui4_len = 32;

    INT32 i4_ret;

    __dm_show_evt(t_evt);

    x_dbg_stmt("[CB]NFY's handle:%d\n", pt_dm_nfy_test->h_nfy);

    __dm_get_hw_str(pt_dm_nfy_test->t_hw_type, ps_buf);
    x_dbg_stmt("[CB]NFY's hardware type:%s\n", ps_buf);

    __dm_get_dev_type_str(pt_dm_nfy_test->t_dev_type, ps_buf);
    x_dbg_stmt("[CB]NFY's device type:%s\n", ps_buf);

    if (DM_EVT_CAT(t_evt) == DM_EVT_CAT_DEV)
    {
        DM_DEV_EVT_DATA_T *pt_evt_data = (DM_DEV_EVT_DATA_T *)pv_data;

        x_dbg_stmt("[CB]Device's handle:%d\n", pt_evt_data->h_dev);

        __dm_get_hw_str(pt_evt_data->t_hw_type, ps_buf);
        x_dbg_stmt("[CB]Device's hardware type:%s\n", ps_buf);

        __dm_get_dev_type_str(pt_evt_data->t_dev_type, ps_buf);
        x_dbg_stmt("[CB]Device's device type:%s\n", ps_buf);
        
        x_dbg_stmt("[CB]Device's unit id:%d\n", pt_evt_data->ui4_unit_id);
        x_dbg_stmt("[CB]Device's socket number:%d\n", pt_evt_data->ui4_skt_no);
        x_dbg_stmt("[CB]Device's availability:%d\n", pt_evt_data->b_avail);

        if (DM_DEV_CHR(pt_evt_data->t_dev_type) == DM_DEV_CHR_MEDIUM)
        {
            i4_ret = x_fm_get_dir_path(pt_evt_data->h_dev_root_lbl, 
                                       ps_buf, 
                                       &ui4_len);

            if (i4_ret != FMR_OK)
            {
                x_dbg_stmt("[CB]Device's root directory:ERROR\n");
            }
            else
            {
                x_dbg_stmt("[CB]Device's root directory:%s\n", ps_buf);
            }

            x_dbg_stmt("[CB]Device's name:%s\n", pt_evt_data->ps_dev_name);
        }
    }
    else if (DM_EVT_CAT(t_evt) == DM_EVT_CAT_FS)
    {
        DM_FS_EVT_DATA_T *pt_evt_data = (DM_FS_EVT_DATA_T *)pv_data;

        x_dbg_stmt("[CB]Device's handle:%d\n", pt_evt_data->h_dev);

        i4_ret = x_fm_get_dir_path(pt_evt_data->h_dev_root_lbl, 
                                   ps_buf, 
                                   &ui4_len);

        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("[CB]Device root directory:ERROR\n");
        }
        else
        {
            x_dbg_stmt("[CB]Device root directory:%s\n", ps_buf);
        }        

        x_dbg_stmt("[CB]Device name:%s\n", pt_evt_data->ps_dev_name);
        x_dbg_stmt("[CB]Device partition name:%s\n", pt_evt_data->ps_part_name);
        x_dbg_stmt("[CB]Mount full path:%s\n", pt_evt_data->ps_mnt_path);
        x_dbg_stmt("[CB]Mount fs index:%d\n", pt_evt_data->ui4_part_idx);
    }
    else if (DM_EVT_CAT(t_evt) == DM_EVT_CAT_LDR)
    {
        DM_LDR_EVT_DATA_T *pt_evt_data = (DM_LDR_EVT_DATA_T *)pv_data;

        x_dbg_stmt("[CB]Device's handle:%d\n", pt_evt_data->h_dev);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _dm_cli_reg_nfy
 *
 * Description: This API registers notify function.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _dm_cli_reg_nfy(
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    INT32 i4_ret;
    DM_EVT_CAT_T t_evt_cat;

    DM_COND_T at_cond[1];
    DM_EVT_FILTR_T t_ent_filtr;
    _DM_NFY_TEST_T *pt_dm_nfy_test = NULL;

    if ((i4_argc != 4) || (pps_argv == NULL))
    {
        x_dbg_stmt("dm.reg_nfy [evt_cat] [hw_type] [dev_type]\n\r");
        x_dbg_stmt("[evt_cat]: 0=device, 1=fs, 2=loader\n\r");
        x_dbg_stmt("[hw_type]: 0=unknown (all), 1=eeprom, 2=nand, 3=nor, 4=usb, 5=ide, 6=fci 7=1394 hw\n\r");
        x_dbg_stmt("[dev_type]: 0=unknown (all), 1=socket, 2=medium\n\r");
        x_dbg_stmt("[dev_type]: 3=eeprom hw, 4=nand hw, 5=nor hw, 6=usb hw, 7=ide hw, 8=fci hw 9=1394 hw\n\r");
        x_dbg_stmt("[dev_type]: 10=hub, 11=card reader, 12=optical drive\n\r");
        x_dbg_stmt("[dev_type]: 13=eeprom, 14=nand, 15=nor, 16=hdd, 17=disc, 18=mem card, 19=mass storage\n\r");

        goto exit;
    }    

    i4_ret = __dm_get_evt_cat((const CHAR *) pps_argv[1], &t_evt_cat);

    if (i4_ret != CLIR_OK)
    {
        x_dbg_stmt("[evt_cat] parameter is wrong.\n");
        goto exit;
    }

    i4_ret = __dm_get_hw_type((const CHAR *) pps_argv[2], &(at_cond[0].t_hw_type));

    if (i4_ret != CLIR_OK)
    {
        x_dbg_stmt("[hw_type] parameter is wrong.\n");
        goto exit;
    }

    i4_ret = __dm_get_dev_type((const CHAR *) pps_argv[3], &(at_cond[0].t_dev_type));

    if (i4_ret != CLIR_OK)
    {
        x_dbg_stmt("[dev_type] parameter is wrong.\n");
        goto exit;
    }

    pt_dm_nfy_test = x_mem_alloc(sizeof(_DM_NFY_TEST_T));

    if (pt_dm_nfy_test == NULL)
    {
        x_dbg_stmt("not enough mem for pt_dm_nfy_test.\n");
        goto exit;
    }

    t_ent_filtr.pt_evt_filtr = (DM_COND_T *)(&at_cond[0]);
    t_ent_filtr.ui4_evt_filtr_ns = 1;

    pt_dm_nfy_test->t_hw_type = at_cond[0].t_hw_type;
    pt_dm_nfy_test->t_dev_type = at_cond[0].t_dev_type;

    i4_ret = x_dm_reg_nfy_fct(t_evt_cat, 
                              __dm_nfy_fct, 
                              &t_ent_filtr, 
                              (VOID *) pt_dm_nfy_test, 
                              &(pt_dm_nfy_test->h_nfy));

    if (i4_ret != DMR_OK)
    {
        x_dbg_stmt("register notify function fails.\n");
        goto exit;
    }

    SLIST_INSERT_HEAD(pt_dm_nfy_test, &(_t_nfy_test_list), t_link);

    x_dbg_stmt("notify function registered.  HANDLE=%d.\n", pt_dm_nfy_test->h_nfy);

    return CLIR_OK;

exit:

    if (pt_dm_nfy_test != NULL)
    {
        x_mem_free(pt_dm_nfy_test);
    }

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _dm_cli_reg_nfy
 *
 * Description: This API un-registers notify function.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _dm_cli_unreg_nfy(
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    HANDLE_T h_nfy;
    INT32 i4_ret;

    _DM_NFY_TEST_T *pt_nfy_test;

    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("dm.unreg_nfy [nfy_handle]\n\r");
        goto exit;
    }

    i4_ret = __dm_parse_int(pps_argv[1], (INT32 *)&h_nfy);

    if (i4_ret != CLIR_OK)
    {
        x_dbg_stmt("[nfy_handle] parameter is wrong.\n");
        goto exit;
    }

    i4_ret = x_dm_unreg_nfy_fct(h_nfy);

    if (i4_ret != DMR_OK)
    {
        x_dbg_stmt("un-register notify function fails.\n");
        goto exit;
    }

    SLIST_FOR_EACH(pt_nfy_test, &(_t_nfy_test_list), t_link)
    {
        if ((pt_nfy_test->h_nfy == h_nfy))
        {
            SLIST_REMOVE(pt_nfy_test, t_link);
            x_mem_free(pt_nfy_test);
            break;
        }
    }
   
    return CLIR_OK;

exit:

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _dm_cli_get_device_info
 *
 * Description: This API get relevant device information.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _dm_cli_get_device_info(
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    INT32 i4_ret;
    HANDLE_T h_dev;

    DM_DEV_INF_T t_dev_inf;

    CHAR ps_buf[32];

    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("dm.get_device_info [device_handle]\n\r");
        goto exit;
    }

    i4_ret = __dm_parse_int(pps_argv[1], (INT32 *)&h_dev);

    if (i4_ret != CLIR_OK)
    {
        x_dbg_stmt("[device_handle] parameter is wrong.\n");
        goto exit;
    }

    i4_ret = x_dm_get_device_info(h_dev, &t_dev_inf);

    if (i4_ret != DMR_OK)
    {
        x_dbg_stmt("get device information fails.\n");
        goto exit;
    }

    __dm_get_hw_str(t_dev_inf.t_hw_type, ps_buf);
    x_dbg_stmt("[DEV_INF]Device's hardware type:%s\n", ps_buf);

    __dm_get_dev_type_str(t_dev_inf.t_dev_type, ps_buf);
    x_dbg_stmt("[DEV_INF]Device's device type:%s\n", ps_buf);    

    x_dbg_stmt("[DEV_INF]Device's root device handle:%d\n", t_dev_inf.t_root_stat.h_dev);

    __dm_get_hw_str(t_dev_inf.t_root_stat.t_hw_type, ps_buf);
    x_dbg_stmt("[DEV_INF]Device's root hardware type:%s\n", ps_buf);    

    __dm_get_dev_type_str(t_dev_inf.t_root_stat.t_dev_type, ps_buf);
    x_dbg_stmt("[DEV_INF]Device's root device type:%s\n", ps_buf);    

    x_dbg_stmt("[DEV_INF]Device's root device unit id:%d\n", t_dev_inf.t_root_stat.ui4_unit_id);
    x_dbg_stmt("[DEV_INF]Device's root device socket number:%d\n", t_dev_inf.t_root_stat.ui4_skt_no);

    x_dbg_stmt("[DEV_INF]Device's root device availability:%d\n", t_dev_inf.t_root_stat.b_avail);
/*
    if (t_dev_inf.t_prnt_stat.t_hw_type != DM_HW_TYPE_UNKNOWN)
    {
        x_dbg_stmt("[DEV_INF]Device's parent device handle:%d\n", t_dev_inf.t_prnt_stat.h_dev);

        __dm_get_hw_str(t_dev_inf.t_prnt_stat.t_hw_type, ps_buf);
        x_dbg_stmt("[DEV_INF]Device's parent hardware type:%s\n", ps_buf);    

        __dm_get_dev_type_str(t_dev_inf.t_prnt_stat.t_dev_type, ps_buf);
        x_dbg_stmt("[DEV_INF]Device's parent device type:%s\n", ps_buf);    

        x_dbg_stmt("[DEV_INF]Device's parent device unit id:%d\n", t_dev_inf.t_prnt_stat.ui4_unit_id);
        x_dbg_stmt("[DEV_INF]Device's parent device socket number:%d\n", t_dev_inf.t_prnt_stat.ui4_skt_no);

        x_dbg_stmt("[DEV_INF]Device's parent device availability:%d\n", t_dev_inf.t_prnt_stat.b_avail);
    }
    else
    {
        x_dbg_stmt("[DEV_INF]Device has no parent device.\n");    
    }
*/

    x_dbg_stmt("[DEV_INF]Device's unit id:%d\n", t_dev_inf.ui4_unit_id);
    x_dbg_stmt("[DEV_INF]Device's socket number:%d\n", t_dev_inf.ui4_skt_no);

    x_dbg_stmt("[DEV_INF]Device's max io request:%d\n", t_dev_inf.ui4_max_io_request);
    x_dbg_stmt("[DEV_INF]Device's model string:%s\n", t_dev_inf.aui1_model_str);
    x_dbg_stmt("[DEV_INF]Device's mem align:%d\n", t_dev_inf.ui4_mem_align);

    x_dbg_stmt("[DEV_INF]Device's availability:%d\n", t_dev_inf.b_avail);
/*
    if (DM_DEV_CHR(t_dev_inf.t_dev_type) == DM_DEV_CHR_SOCKET)
    {
        UINT32 ui4_cnt;

        x_dbg_stmt("[DEV_INF]Device is a socket device.\n");
        x_dbg_stmt("[DEV_INF]Device's total slot ns:%d\n", t_dev_inf.u.t_skt_inf.ui4_total_slot_ns);
        x_dbg_stmt("[DEV_INF]Device's used slot ns:%d\n", t_dev_inf.u.t_skt_inf.ui4_used_slot_ns);

        for (ui4_cnt = 0; ui4_cnt < t_dev_inf.u.t_skt_inf.ui4_used_slot_ns; ui4_cnt++)
        {
            x_dbg_stmt("[DEV_INF]Device's #%d child device slot no:%d\n", ui4_cnt, t_dev_inf.u.t_skt_inf.at_slot_inf[ui4_cnt].ui4_slot_no);

            x_dbg_stmt("[DEV_INF]Device's #%d child device handle:%d\n", ui4_cnt, t_dev_inf.u.t_skt_inf.at_slot_inf[ui4_cnt].t_dev_stat.h_dev);

            __dm_get_hw_str(t_dev_inf.u.t_skt_inf.at_slot_inf[ui4_cnt].t_dev_stat.t_hw_type, ps_buf);
            x_dbg_stmt("[DEV_INF]Device's #%d child hardware type:%s\n", ui4_cnt, ps_buf);    

            __dm_get_dev_type_str(t_dev_inf.u.t_skt_inf.at_slot_inf[ui4_cnt].t_dev_stat.t_dev_type, ps_buf);
            x_dbg_stmt("[DEV_INF]Device's #%d child device type:%s\n", ui4_cnt, ps_buf);    

            x_dbg_stmt("[DEV_INF]Device's #%d child device unit id:%d\n", ui4_cnt, t_dev_inf.u.t_skt_inf.at_slot_inf[ui4_cnt].t_dev_stat.ui4_unit_id);
            x_dbg_stmt("[DEV_INF]Device's #%d child device socket number:%d\n", ui4_cnt, t_dev_inf.u.t_skt_inf.at_slot_inf[ui4_cnt].t_dev_stat.ui4_skt_no);

            x_dbg_stmt("[DEV_INF]Device's #%d child device availability:%d\n", ui4_cnt, t_dev_inf.u.t_skt_inf.at_slot_inf[ui4_cnt].t_dev_stat.b_avail);
        }
    }
    else if (DM_DEV_CHR(t_dev_inf.t_dev_type) == DM_DEV_CHR_MEDIUM)
    {
        if (t_dev_inf.u.t_med_inf.t_tx_type == DM_DEV_MED_TX_TYPE_BLKDEV)
        {
            x_dbg_stmt("[DEV_INF]Device is a block device.\n");
        }
        else if (t_dev_inf.u.t_med_inf.t_tx_type == DM_DEV_MED_TX_TYPE_CHRDEV)
        {
            x_dbg_stmt("[DEV_INF]Device is a character device.\n");
        }
        else
        {
            DM_ASSERT(0);
        }
        
        x_dbg_stmt("[DEV_INF]Device's total size:%lld\n", t_dev_inf.u.t_med_inf.ui8_total_size);

        i4_ret = x_fm_get_dir_path(t_dev_inf.u.t_med_inf.h_dev_root_lbl, 
                                   ps_buf, 
                                   &ui4_len);

        if (i4_ret != FMR_OK)
        {
            x_dbg_stmt("[DEV_INF]Device's device directory:ERROR\n");
        }
        else
        {
            x_dbg_stmt("[DEV_INF]Device's device directory:%s\n", ps_buf);
        }        

        x_dbg_stmt("[DEV_INF]Device's name:%s\n", t_dev_inf.u.t_med_inf.ps_dev_name);

        if (t_dev_inf.u.t_med_inf.t_tx_type == DM_DEV_MED_TX_TYPE_BLKDEV)
        {
            x_dbg_stmt("[DEV_INF]Device's sector size:%d\n", t_dev_inf.u.t_med_inf.u.t_blk_inf.ui4_sec_size);
            x_dbg_stmt("[DEV_INF]Device's sector ns:%lld\n", t_dev_inf.u.t_med_inf.u.t_blk_inf.ui8_sec_ns);

            x_dbg_stmt("[DEV_INF]Device's max tx sector ns:%d\n", t_dev_inf.u.t_med_inf.u.t_blk_inf.ui4_max_tx_sec_ns);
            x_dbg_stmt("[DEV_INF]Device's best tx sector ns:%d\n", t_dev_inf.u.t_med_inf.u.t_blk_inf.ui4_best_tx_sec_ns);
        
            if (t_dev_inf.t_dev_type == DM_DEV_MED_TYPE_MEM_CARD)
            {
                __dm_get_card_str(t_dev_inf.u.t_med_inf.u.t_blk_inf.u.t_card_inf.t_card_type, ps_buf);
                x_dbg_stmt("[DEV_INF]Device's memory card type:%s\n", ps_buf);
            }
        }
        else if (t_dev_inf.u.t_med_inf.t_tx_type == DM_DEV_MED_TX_TYPE_CHRDEV)
        {
            x_dbg_stmt("[DEV_INF]Device's max sync read size:%d\n", t_dev_inf.u.t_med_inf.u.t_chr_inf.ui4_max_sync_read_sz);
            x_dbg_stmt("[DEV_INF]Device's max sync write size:%d\n", t_dev_inf.u.t_med_inf.u.t_chr_inf.ui4_max_sync_write_sz);

            x_dbg_stmt("[DEV_INF]Device's sector ns:%d\n", t_dev_inf.u.t_med_inf.u.t_chr_inf.ui4_sec_ns);
        }
        else
        {
            DM_ASSERT(0);
        }

    }
    else
    {
    		x_dbg_stmt("[DEV_INF]Device is an unknown device.\n");
    		x_dbg_stmt("[DEV_INF]Description String: %s\n", (UINT8 *)t_dev_inf.pv_data);
    }
*/

    return CLIR_OK;

exit:

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _dm_cli_get_devices
 *
 * Description: This API get relevant device information.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _dm_cli_get_devices(
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    INT32 i4_ret;

    DM_COND_T at_cond[1];
    DM_DEV_FILTR_T t_dev_filtr = {0};

    UINT32 ui4_dev_ns;
    UINT32 ui4_cnt;

    CHAR ps_buf[32];

    if ((i4_argc != 3) || (pps_argv == NULL))
    {
        x_dbg_stmt("dm.get_devices [hw_type] [dev_type]\n\r");
        x_dbg_stmt("[hw_type]: 0=unknown (all), 1=eeprom, 2=nand, 3=nor, 4=usb, 5=ide, 6=fci 7=1394 hw\n\r");
        x_dbg_stmt("[dev_type]: 0=unknown (all), 1=socket, 2=medium\n\r");
        x_dbg_stmt("[dev_type]: 3=eeprom hw, 4=nand hw, 5=nor hw, 6=usb hw, 7=ide hw, 8=fci hw 9=1394 hw\n\r");
        x_dbg_stmt("[dev_type]: 10=hub, 11=card reader, 12=optical drive\n\r");
        x_dbg_stmt("[dev_type]: 13=eeprom, 14=nand, 15=nor, 16=hdd, 17=disc, 18=mem card, 19=mass storage\n\r");

        goto exit;
    }    

    i4_ret = __dm_get_hw_type((const CHAR *) pps_argv[1], &(at_cond[0].t_hw_type));

    if (i4_ret != CLIR_OK)
    {
        x_dbg_stmt("[hw_type] parameter is wrong.\n");
        goto exit;
    }

    i4_ret = __dm_get_dev_type((const CHAR *) pps_argv[2], &(at_cond[0].t_dev_type));

    if (i4_ret != CLIR_OK)
    {
        x_dbg_stmt("[dev_type] parameter is wrong.\n");
        goto exit;
    }

    t_dev_filtr.pt_dev_filtr = (DM_COND_T *)(&at_cond[0]);
    t_dev_filtr.ui4_dev_filtr_ns = 1;

    i4_ret = x_dm_get_devices(&t_dev_filtr, 
                              at_dev_stat,
                              &ui4_dev_ns);

    if (i4_ret != DMR_OK)
    {
        x_dbg_stmt("get devices fails.\n");
        goto exit;
    }

    for (ui4_cnt = 0; ui4_cnt < ui4_dev_ns; ui4_cnt++)
    {
        x_dbg_stmt("[DEV_INF]#%d device's handle:%d\n", ui4_cnt, at_dev_stat[ui4_cnt].h_dev);

        __dm_get_hw_str(at_dev_stat[ui4_cnt].t_hw_type, ps_buf);
        x_dbg_stmt("[DEV_INF]#%d device's hardware type:%s\n", ui4_cnt, ps_buf);    

        __dm_get_dev_type_str(at_dev_stat[ui4_cnt].t_dev_type, ps_buf);
        x_dbg_stmt("[DEV_INF]#%d device's device type:%s\n", ui4_cnt, ps_buf);    

        x_dbg_stmt("[DEV_INF]#%d device's unit id:%d\n", ui4_cnt, at_dev_stat[ui4_cnt].ui4_unit_id);
        x_dbg_stmt("[DEV_INF]#%d device's socket number:%d\n", ui4_cnt, at_dev_stat[ui4_cnt].ui4_skt_no);

        x_dbg_stmt("[DEV_INF]#%d device's availability:%d\n", ui4_cnt, at_dev_stat[ui4_cnt].b_avail);
    }

exit:

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _dm_cli_get_dbg_lvl
 *
 * Description: This API gets the current setting of Device Manager debug level.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _dm_cli_get_dbg_lvl(
    INT32        i4_argc,
    const CHAR** pps_argv)
{   
    if (i4_argc != 1)
    {
        x_dbg_stmt("dm.sdl\n\r");
        return CLIR_OK;
    }    

    return x_cli_show_dbg_level(dm_get_dbg_level());
}

/*-----------------------------------------------------------------------------
 * Name: _dm_cli_set_dbg_level
 *
 * Description: This API sets the debug level of Device Manager.
 *
 * Inputs:  i4_argc         Contains the argument count.
 *          pps_argv        Contains the arguments.
 *
 * Outputs: -
 *
 * Returns: CLIR_OK                 Routine successful.
 ----------------------------------------------------------------------------*/
static INT32 _dm_cli_set_dbg_lvl(
    INT32        i4_argc,
    const CHAR** pps_argv)
{
    INT32       i4_ret;
    UINT16      ui2_dbg_level;     
   
    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        x_dbg_stmt("dm.sdl [level]\n\r");
        x_dbg_stmt("[level]: n=none, e=error, a=api, i=info");
        return CLIR_OK;
    }    
    
    i4_ret = x_cli_parse_dbg_level(i4_argc, pps_argv, &ui2_dbg_level);
    
    if (i4_ret == CLIR_OK)
    {
        if (dm_set_dbg_level(ui2_dbg_level) == FALSE)
        {
            i4_ret = CLIR_CMD_EXEC_ERROR;  
        }
    }
    else
    {
        x_dbg_stmt("dm.sdl [level]\n\r");
        x_dbg_stmt("[level]: n=none, e=error, a=api, i=info");
        i4_ret = CLIR_OK;
    }

    x_dbg_stmt("set debug level %s\n\r", (i4_ret == CLIR_OK) ? "successful" : "failed");
    return i4_ret;
}


static INT32 _dm_cli_get_usb_port_num(
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    INT32       i4_ret;
    UINT8       ui1_port_number;     
   
    if (i4_argc != 1)
    {
        x_dbg_stmt("[usage:]usb_port_num \n\r");
        return CLIR_OK;
    }

    i4_ret = x_dm_get_usb_port_number(&ui1_port_number);
    if (i4_ret == DMR_OK)
    {
        x_dbg_stmt("%x", ui1_port_number);
    }

    return CLIR_OK;
}


#endif

