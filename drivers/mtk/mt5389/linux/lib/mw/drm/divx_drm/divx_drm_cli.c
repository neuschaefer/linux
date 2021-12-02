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
 * $RCSfile: divx_drm_cli.c,v $
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
#include "libc/string.h"
#include "dbg/x_dbg.h"
#include "cli/x_cli.h"
#include "drm/x_drm.h"
#include "drm/divx_drm/x_divx_drm.h"
#include "drm/divx_drm/divx_drm_cli.h"
#include "drm/divx_drm/divx_drm_adp.h"

#ifdef DIVX_DRM_40
#include "drm/divx_drm/divx_drm_40/AdpLib/DrmAdpApi.h"
#include "drm/divx_drm/divx_drm_40/AdpLib/DrmAdpTypes.h"
#include "drm/divx_drm/divx_drm_40/AdpLocalLib/DrmAdpLocal.h"
#include "drm/divx_drm/divx_drm_40/AdpLocalLib/DrmAdpKeyStoreLocal.h"
#elif defined(DIVX_DRM_51)
#include "drm/divx_drm/divx_drm_51/DrmApi.h"
#include "drm/divx_drm/divx_drm_51/DrmTypes.h"
#include "drm/divx_drm/divx_drm_51/DrmConfig.h"
#include "drm/divx_drm/divx_drm_51/local/DrmLocal.h"
#include "drm/divx_drm/divx_drm_51/DrmKeyStore.h"
#include "drm/divx_drm/divx_drm_51/DrmDataLoad.h"
#include "drm/divx_drm/divx_drm_51/HeaderMem2Structs.h"
#endif

#else

#include "x_common.h"
#include "x_dbg.h"
#include "x_cli.h"
#include "../x_drm.h"
#include "x_divx_drm.h"
#include "divx_drm_cli.h"
#include "divx_drm_adp.h"

#ifdef DIVX_DRM_40
#include "divx_drm_40/AdpLib/DrmAdpApi.h"
#include "divx_drm_40/AdpLib/DrmAdpTypes.h"
#include "divx_drm_40/AdpLocalLib/DrmAdpLocal.h"
#include "divx_drm_40/AdpLocalLib/DrmAdpKeyStoreLocal.h"
#elif defined(DIVX_DRM_51)
#include "divx_drm_51/DrmApi.h"
#include "divx_drm_51/DrmTypes.h"
#include "divx_drm_51/DrmConfig.h"
#include "divx_drm_51/local/DrmLocal.h"
#include "divx_drm_51/DrmKeyStore.h"
#include "divx_drm_51/DrmDataLoad.h"
#include "divx_drm_51/HeaderMem2Structs.h"
#endif

#endif

#ifdef CLI_SUPPORT
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/
static INT32 divx_drm_cli_drm_mem_dump (INT32 i4_argc, const CHAR** pps_argv);
static INT32 divx_drm_cli_drm_mem_reset (INT32 i4_argc, const CHAR** pps_argv);
static INT32 divx_drm_cli_drm_mem_set (INT32 i4_argc, const CHAR** pps_argv);
static INT32 divx_drm_cli_reg_code (INT32 i4_argc, const CHAR** pps_argv);
static INT32 divx_drm_cli_deact_code (INT32 i4_argc, const CHAR** pps_argv);
static INT32 divx_drm_cli_hw_key (INT32 i4_argc, const CHAR** pps_argv);
static INT32 divx_drm_cli_ui_help (INT32 i4_argc, const CHAR** pps_argv);

/* divx drm command table */
static CLI_EXEC_T at_divx_drm_cmd_tbl[] =
{
    {"dump", NULL, divx_drm_cli_drm_mem_dump, NULL, "dump drm memory", CLI_SUPERVISOR},
    {"reset", NULL, divx_drm_cli_drm_mem_reset, NULL, "reset drm memory", CLI_SUPERVISOR},
    {"set", NULL, divx_drm_cli_drm_mem_set, NULL, "set drm memory", CLI_SUPERVISOR},    
    {"code", NULL, divx_drm_cli_reg_code, NULL, "show registration code", CLI_SUPERVISOR},        
    {"deact", NULL, divx_drm_cli_deact_code, NULL, "show deactivation code", CLI_SUPERVISOR},            
    {"key", NULL, divx_drm_cli_hw_key, NULL, "show hardware secret key", CLI_SUPERVISOR},            
    {"ui", NULL, divx_drm_cli_ui_help, NULL, "show ui help info", CLI_SUPERVISOR},                
    END_OF_CLI_CMD_TBL
};

/*-----------------------------------------------------------------------------
 * Name: printDrmMemoryDebug
 *
 * Description: This API dumps drm memory.
 *
 * Inputs: pui1_memory  Specify pointer to drm memory.
 *
 * Outputs: NONE.
 *
 * Returns: NONE.
 ----------------------------------------------------------------------------*/
static VOID printDrmMemoryDebug (
    UINT8* pui1_memory)
{
#ifdef DIVX_DRM_40
    UINT32 ui4_i, ui4_j, ui4_offset;
    
    x_dbg_stmt("*** Debugging DRM Memory Contents Dump ****\n\n");
    x_dbg_stmt("-- %02X %02X %02X : Owner Guard\n", pui1_memory[0], pui1_memory[1], pui1_memory[2]);

    ui4_offset = OWNER_GUARD_BYTES;

    for (ui4_j = 0; ui4_j < KEY_SIZE_BYTES; ui4_j++)
    {
        x_dbg_stmt("%02X ", pui1_memory[ui4_j + ui4_offset]);
        if ((ui4_j + 1) % 4 == 0 && ui4_j != KEY_SIZE_BYTES - 1)
        {
            x_dbg_stmt("\n");
        }
    }
    x_dbg_stmt(": Owner Key (16 bytes)\n");

    ui4_offset = ui4_offset + KEY_SIZE_BYTES;

    x_dbg_stmt("%02X %02X %02X %02X\n         %02X : Owner User Id (5 bytes)\n", 
    pui1_memory[ui4_offset + 0], pui1_memory[ui4_offset + 1], pui1_memory[ui4_offset + 2], pui1_memory[ui4_offset + 3], pui1_memory[ui4_offset + 4]);
    x_dbg_stmt("\n");

    ui4_offset = ui4_offset + OWNER_USER_ID_BYTES;

    for (ui4_i = 0; ui4_i < TOTAL_PLAY_SLOTS * (SLOT_SERIAL_NUMBER_BYTES + 1); ui4_i = ui4_i + SLOT_SERIAL_NUMBER_BYTES + 1)
    {
        x_dbg_stmt("-- -- %02X %02X : Slot %d Serial Number\n", pui1_memory[ui4_offset + ui4_i], pui1_memory[ui4_offset + 1 + ui4_i], ui4_i / (SLOT_SERIAL_NUMBER_BYTES + 1));
        x_dbg_stmt("-- -- -- %02X : Slot %d Counter\n", pui1_memory[ui4_offset + SLOT_SERIAL_NUMBER_BYTES + ui4_i], ui4_i / (SLOT_SERIAL_NUMBER_BYTES + 1));
        x_dbg_stmt("\n");   
    }
#elif defined(DIVX_DRM_51)

    UINT32 ui4_i, ui4_j;
    drmMemory_t t_drmMemory;
    
    mem2DrmMemory(&pui1_memory, &t_drmMemory, 0);

    x_dbg_stmt( "*** Debugging DRM Memory Contents Dump ****\n\n" );
#if (DRM_OTHER_SECURE_PLATFORM == 0)
    x_dbg_stmt( "%02X %02X %02X %02X : Prefix\n",
             t_drmMemory.prefixPad[0],
             t_drmMemory.prefixPad[1],
             t_drmMemory.prefixPad[2],
             t_drmMemory.prefixPad[3] );

    x_dbg_stmt( "%02X %02X %02X %02X : Random Pad\n",
             t_drmMemory.randomPad1[0],
             t_drmMemory.randomPad1[1],
             t_drmMemory.randomPad1[2],
             t_drmMemory.randomPad1[3] );
#endif
    x_dbg_stmt( "%02X %02X %02X %02X : Owner Guard\n",
             t_drmMemory.owner.guard[0],
             t_drmMemory.owner.guard[1],
             t_drmMemory.owner.guard[2],
             t_drmMemory.owner.guard[3]);
#if (DRM_OTHER_SECURE_PLATFORM == 0)            
    x_dbg_stmt( "-- -- -- %02X : Random Pad\n",
            t_drmMemory.owner.randomPad1);
#endif
    for ( ui4_j = 0; ui4_j < 16; ui4_j++ )
    {
        if ( ( ui4_j != 0 ) && ( ui4_j % 4 == 0 ) )
        {
            x_dbg_stmt( "\n" );
        }
        x_dbg_stmt( "%02x ", t_drmMemory.owner.key[ui4_j]);
    }

    x_dbg_stmt( ": Owner Key (16 bytes)\n" );
#if (DRM_OTHER_SECURE_PLATFORM == 0)
    x_dbg_stmt( "-- -- -- %02X : Random Pad\n",
            t_drmMemory.owner.randomPad2 );
#endif
    x_dbg_stmt( "%02X %02X %02X %02X\n-- -- -- %02X : Owner User Id (5 bytes)\n\n", 
        t_drmMemory.owner.userId[0],
	t_drmMemory.owner.userId[1],
	t_drmMemory.owner.userId[2],
	t_drmMemory.owner.userId[3],
	t_drmMemory.owner.userId[4] );

    for ( ui4_i = 0; ui4_i < TOTAL_PLAY_SLOTS; ui4_i++ )
    {
        /* UINT32 offset = ( ui4_i * 4 ); */
        x_dbg_stmt( "-- -- %02X %02X : Slot %d Serial Number\n", t_drmMemory.slots[ui4_i].serialNumber[0], t_drmMemory.slots[ui4_i].serialNumber[1], ui4_i );
        x_dbg_stmt( "-- -- -- %02X : Slot %d Counter\n", t_drmMemory.slots[ui4_i].counter, ui4_i);
        x_dbg_stmt( "\n" );
    }
#endif
}

/*-----------------------------------------------------------------------------
 * Name: strToHex
 *
 * Description: This API transfers string to hex.
 *
 * Inputs: ps_instr  Specify the input string.
 *         pai1_outarray  Specify pointer to retrieve the output array.
 *         pui4_len  Specify pointer to retrieve the output length.

 * Outputs: pai1_outarray  Specify pointer contains the output array.
 *          pui4_len  Specify pointer contains the output length.
 *
 * Returns: TRUE  Success.
            FALSE  Fail.
 ----------------------------------------------------------------------------*/
static BOOL strToHex (
    CHAR* ps_instr, 
    UINT8* pai1_outarray, 
    UINT32* pui4_len)
{
    UINT8 ui1_upper, ui1_lower;

    if ((ps_instr == NULL) || (pai1_outarray == NULL) || (pui4_len == NULL))
    {
        return FALSE;
    }

    while (1) 
    {
        ui1_upper = *ps_instr++;
        ui1_lower = *ps_instr++;

        if (ui1_lower == '\0')
        {
            ui1_lower = '0';
            *ps_instr = '\0';
        }

        if ((ui1_upper >= '0') && (ui1_upper <= '9'))
        {
            ui1_upper-= '0';
        }
        else if ((ui1_upper >= 'a') && (ui1_upper <= 'f')) 
        {
            ui1_upper -= 'a' - 10;
        }
        else if ((ui1_upper >= 'A') && (ui1_upper <= 'F')) 
        {
            ui1_upper -= 'A' - 10;        
        }
        else 
        {
            return FALSE;
        }

        if ((ui1_lower >= '0') && (ui1_lower <= '9')) 
        {
            ui1_lower -= '0';
        }
        else if ((ui1_lower >= 'a') && (ui1_lower <= 'f')) 
        {
            ui1_lower -= 'a' - 10;
        }
        else if ((ui1_lower >= 'A') && (ui1_lower <= 'F')) 
        {
            ui1_lower -= 'A' - 10;        
        }
        else
        {
            return FALSE;
        }
        
        *pai1_outarray++ = (ui1_upper << 4) | ui1_lower;
        
        (*pui4_len)++;       

        if (*ps_instr == '\0')
        {
            break;
        }
    }

    return TRUE;
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_cli_drm_mem_dump
 *
 * Description: This API dumps the drm memory.
 *
 * Inputs: i4_argc  Specify the argument count.
 *         pps_argv  Specify the arguments.
 *
 * Outputs: NONE.
 *
 * Returns: CLIR_OK  Success.
 *          CLIR_CMD_EXEC_ERROR  Fail.
 ----------------------------------------------------------------------------*/
static INT32 divx_drm_cli_drm_mem_dump (
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    INT8 i1_ret;
    UINT8 aui1_memory[PACKED_ALLOCATION_BYTES] = {0};

#ifdef DIVX_DRM_40    
    i1_ret = localLoadDrmMemory(aui1_memory);
    if (ADP_LOCAL_SUCCESS != i1_ret)
#elif defined(DIVX_DRM_51)
    i1_ret = drmDataLoadLoadDrmMemory(aui1_memory, PACKED_ALLOCATION_BYTES);
    if ( 0 != i1_ret)
#endif
    {
        x_dbg_stmt("Load DRM memory failed\n");

        return CLIR_CMD_EXEC_ERROR;
    }
    
    printDrmMemoryDebug(aui1_memory);

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_cli_drm_mem_reset
 *
 * Description: This API resets the drm memory.
 *
 * Inputs: i4_argc  Specify the argument count.
 *         pps_argv  Specify the arguments.
 *
 * Outputs: NONE.
 *
 * Returns: CLIR_OK  Success.
 *          CLIR_CMD_EXEC_ERROR  Fail.
 ----------------------------------------------------------------------------*/
static INT32 divx_drm_cli_drm_mem_reset (
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    INT8 i1_ret;
    UINT8 aui1_memory[PACKED_ALLOCATION_BYTES] = {0};

#ifdef DIVX_DRM_40
    i1_ret = localSaveDrmMemory(aui1_memory);
    if (ADP_LOCAL_SUCCESS != i1_ret)
#elif defined(DIVX_DRM_51)
    i1_ret = drmDataLoadSaveDrmMemory(aui1_memory, PACKED_ALLOCATION_BYTES);
    if (0 != i1_ret)
#endif
    {
        x_dbg_stmt("Save DRM memory failed\n");

        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_cli_drm_mem_set
 *
 * Description: This API sets the drm memory.
 *
 * Inputs: i4_argc  Specify the argument count.
 *         pps_argv  Specify the arguments.
 *
 * Outputs: NONE.
 *
 * Returns: CLIR_OK  Success.
 *          CLIR_INV_ARG  Fail.
 *          CLIR_CMD_EXEC_ERROR  Fail. 
 ----------------------------------------------------------------------------*/
static INT32 divx_drm_cli_drm_mem_set (
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    INT8 i1_ret;
    UINT8 ui1_len = 0;
    UINT8 aui1_memory[PACKED_ALLOCATION_BYTES] = {0};
    
    if ((i4_argc != 2) || (pps_argv == NULL))
    {
        return CLIR_INV_ARG;
    }
    
    if (strlen(pps_argv[1]) > PACKED_ALLOCATION_BYTES*2)
    {
        return CLIR_INV_ARG;
    }

    if (!strToHex((char *)pps_argv[1], aui1_memory, (UINT32 *)&ui1_len))
    {
        x_dbg_stmt("strToHex error\n");

        return CLIR_INV_ARG;    
    }

    if (ui1_len > PACKED_ALLOCATION_BYTES)
    {
        x_dbg_stmt("Exceed DRM memory size\n");

        return CLIR_INV_ARG;
    }

#ifdef DIVX_DRM_40    
    i1_ret = localSaveDrmMemory(aui1_memory);
    if (ADP_LOCAL_SUCCESS != i1_ret)
#elif defined(DIVX_DRM_51)
    i1_ret = drmDataLoadSaveDrmMemory(aui1_memory, PACKED_ALLOCATION_BYTES);
    if (0 != i1_ret)
#endif
    {
        x_dbg_stmt("Save DRM memory failed\n");

        return CLIR_CMD_EXEC_ERROR;
    }

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_cli_reg_code
 *
 * Description: This API shows registration code.
 *
 * Inputs: i4_argc  Specify the argument count.
 *         pps_argv  Specify the arguments.
 *
 * Outputs: NONE.
 *
 * Returns: CLIR_OK  Success.
 *          CLIR_CMD_EXEC_ERROR  Fail.
 ----------------------------------------------------------------------------*/
static INT32 divx_drm_cli_reg_code (
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
#if 0
    INT32 i4_ret;
    DRM_COMMAND_T at_drm_cmd_sets[2];
    char s_registration_code[DRM_REGISTRATION_CODE_BYTES] = {0};    
    HANDLE_T h_drm;
    
    at_drm_cmd_sets[0].e_code= DRM_CMD_CODE_DIVX_DRM_REGISTRATION_CODE;
    at_drm_cmd_sets[0].u.ps_text = s_registration_code;
    at_drm_cmd_sets[1].e_code = DRM_CMD_CODE_END;    
    
    i4_ret = x_drm_open(at_drm_cmd_sets, DRM_TYPE_DIVX_DRM, &h_drm);
    if (i4_ret != DRMR_OK)
    {
        x_dbg_stmt("Get Registration code failed: %d\n", i4_ret);

        return CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = x_drm_close(h_drm);
    if (i4_ret != DRMR_OK)
    {
        x_dbg_stmt("Get Registration code failed: %d\n", i4_ret);

        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Registration code: %s\n", s_registration_code);    
    
    return CLIR_OK;    
#else

    INT32 i4_ret;
    DIVX_DRM_UNION_GENERIC_INFO_T t_divx_drm_generic_info= {0};

    i4_ret = x_drm_get_generic_info(DRM_TYPE_DIVX_DRM, DRM_GET_TYPE_DIVX_DRM_GENERIC_REGISTRATION_INFO, 
        &(t_divx_drm_generic_info.t_divx_drm_registration_info.s_registration_code), sizeof(DIVX_DRM_UNION_GENERIC_INFO_T));
    if (i4_ret != DRMR_OK)
    {
        x_dbg_stmt("Get Registration code failed: %d\n", i4_ret);

        return CLIR_CMD_EXEC_ERROR;
    }

    if (x_strcmp(t_divx_drm_generic_info.t_divx_drm_registration_info.s_registration_code, "") != 0)
    {
        x_dbg_stmt("Registration code: %s\n", t_divx_drm_generic_info.t_divx_drm_registration_info.s_registration_code);    
    }

    return CLIR_OK;    
#endif
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_cli_deact_code
 *
 * Description: This API shows deactivation code.
 *
 * Inputs: i4_argc  Specify the argument count.
 *         pps_argv  Specify the arguments.
 *
 * Outputs: NONE.
 *
 * Returns: CLIR_OK  Success.
 *          CLIR_CMD_EXEC_ERROR  Fail.
 ----------------------------------------------------------------------------*/
static INT32 divx_drm_cli_deact_code (
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
#if 0
    INT32 i4_ret;
    DRM_COMMAND_T at_drm_cmd_sets[2];
    char s_deactivation_code[9] = {0};    
    HANDLE_T h_drm;
    
    at_drm_cmd_sets[0].e_code= DRM_CMD_CODE_DIVX_DRM_DEACTIVATION_CODE;
    at_drm_cmd_sets[0].u.ps_text = s_deactivation_code;
    at_drm_cmd_sets[1].e_code = DRM_CMD_CODE_END;    
    
    i4_ret = x_drm_open(at_drm_cmd_sets, DRM_TYPE_DIVX_DRM, &h_drm);
    if (i4_ret != DRMR_OK)
    {
        x_dbg_stmt("Get Deactivation code failed: %d\n", i4_ret);

        return CLIR_CMD_EXEC_ERROR;
    }

    i4_ret = x_drm_close(h_drm);
    if (i4_ret != DRMR_OK)
    {
        x_dbg_stmt("Get Deactivation code failed: %d\n", i4_ret);

        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Deactivation code: %s\n", s_deactivation_code);    
        
    return CLIR_OK;
#else

    INT32 i4_ret;
    DIVX_DRM_UNION_GENERIC_INFO_T t_divx_drm_generic_info= {0};

    i4_ret = x_drm_set_generic_info(DRM_TYPE_DIVX_DRM, DRM_SET_TYPE_DIVX_DRM_GENERIC_DEACTIVATION, 
        &(t_divx_drm_generic_info.t_divx_drm_deactivation_info.s_deactivation_code), sizeof(DIVX_DRM_UNION_GENERIC_INFO_T));
    if (i4_ret != DRMR_OK)
    {
        x_dbg_stmt("Set Deactivation failed: %d\n", i4_ret);

        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("Deactivation code: %s\n", t_divx_drm_generic_info.t_divx_drm_deactivation_info.s_deactivation_code);

    return CLIR_OK;    
#endif
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_cli_hw_key
 *
 * Description: This API shows hardware secret key.
 *
 * Inputs: i4_argc  Specify the argument count.
 *         pps_argv  Specify the arguments.
 *
 * Outputs: NONE.
 *
 * Returns: CLIR_OK  Success.
 ----------------------------------------------------------------------------*/
static INT32 divx_drm_cli_hw_key (
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    UINT8 ui1_idx;
    DIVX_DRM_HW_SECRET_KEY_T t_divx_drm_hw_secret_key;

#ifdef DIVX_DRM_40        
    localGetHardwareSecret((UINT8*)&t_divx_drm_hw_secret_key.aui1_divx_drm_hw_secret_key);
#elif defined(DIVX_DRM_51)
#if DRM_DATALOAD_USE_LHS == 1
    localGetLocalHardwareSecret((UINT8*)&t_divx_drm_hw_secret_key.aui1_divx_drm_hw_secret_key);
#endif
#endif

    x_dbg_stmt("Hardware secret key: ");
    for (ui1_idx = 0; ui1_idx < DIVX_DRM_HW_SECRET_KEY_SIZE_BYTE ; ui1_idx++)
    {
        x_dbg_stmt("%02x", t_divx_drm_hw_secret_key.aui1_divx_drm_hw_secret_key[ui1_idx]);    
    }
    x_dbg_stmt("\n");

    return CLIR_OK;    
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_cli_ui_help
 *
 * Description: This API shows UI help info.
 *
 * Inputs: i4_argc  Specify the argument count.
 *         pps_argv  Specify the arguments.
 *
 * Outputs: NONE.
 *
 * Returns: CLIR_OK  Success.
 *          CLIR_CMD_EXEC_ERROR  Fail.
 ----------------------------------------------------------------------------*/
static INT32 divx_drm_cli_ui_help (
    INT32 i4_argc, 
    const CHAR** pps_argv)
{
    INT32 i4_ret;
    DIVX_DRM_UNION_GENERIC_INFO_T t_divx_drm_generic_info = {0};

    i4_ret = x_drm_get_generic_info(DRM_TYPE_DIVX_DRM, DRM_GET_TYPE_DIVX_DRM_GENERIC_UI_HELP_INFO, 
        &(t_divx_drm_generic_info.t_divx_drm_ui_help_info.ui4_divx_drm_ui_help_info), sizeof(DIVX_DRM_UNION_GENERIC_INFO_T));
    if (i4_ret != DRMR_OK)
    {
        x_dbg_stmt("Get UI help info failed: %d\n", i4_ret);
        return CLIR_CMD_EXEC_ERROR;
    }

    x_dbg_stmt("%s: %s to confirm for deactivation\n", __FUNCTION__, t_divx_drm_generic_info.t_divx_drm_ui_help_info.ui4_divx_drm_ui_help_info & DIVX_DRM_DEACTIVATION_CONFIRMATION ? "Need" : "Don't need");
    x_dbg_stmt("%s: %s to hide registration code\n", __FUNCTION__, t_divx_drm_generic_info.t_divx_drm_ui_help_info.ui4_divx_drm_ui_help_info & DIVX_DRM_REGISTRATION_CODE_HIDE ? "Need" : "Don't need");

    return CLIR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: divx_drm_cli_init
 *
 * Description: This API initializes divx_drm CLI component.
 *
 * Inputs: ppt_command_array  Specify pointer to retrive the command array.
 *
 * Outputs: ppt_command_array  Specify pointer contains the command array.
 *
 * Returns: CLIR_OK  Success.
 ----------------------------------------------------------------------------*/
INT32 divx_drm_cli_init (
    CLI_EXEC_T** ppt_command_array)
{
    *ppt_command_array = at_divx_drm_cmd_tbl;

    return CLIR_OK;
}

#else

/*-----------------------------------------------------------------------------
 * Name: divx_drm_cli_init
 *
 * Description: This API initializes divx_drm CLI component.
 *
 * Inputs: ppt_command_array  Specify pointer to retrive the command array.
 *
 * Outputs: ppt_command_array  Specify pointer contains the command array.
 *
 * Returns: CLIR_OK  Success.
 ----------------------------------------------------------------------------*/
INT32 divx_drm_cli_init (
    CLI_EXEC_T** ppt_command_array)
{
    return CLIR_OK;
}

#endif /* CLI_SUPPORT */

