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
 * $RCSfile: version.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description:
 *         This file contains the version / customer string.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    include files
 ----------------------------------------------------------------------------*/

#include "cli/x_cli.h"
#include "dbg/x_dbg.h"
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "start/version.h"
#include "start/x_start_common.h"


/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/

/* Need to convert the define into a string. */
#define STR_DEF(_str)  #_str

#define CLI_START_MODULE      "start"
#define CLI_START_MODULE_HELP "START module commands"

#define CLI_GET_INFO_LONG     "info"
#define CLI_GET_INFO_SHORT    "i"

#define CLI_GET_INFO_HELP     "Outputs the build, customer and model name as well as the version number."


/*-----------------------------------------------------------------------------
                    data declaraions
 ----------------------------------------------------------------------------*/

const static CHAR  s_build_name         [] = BUILD_NAME;
const static CHAR  s_customer           [] = CUSTOMER;

#ifndef MTK_VERSION
const static CHAR  s_version            [] = VERSION;
#else
const static CHAR  s_version            [] = MTK_VERSION;
#endif

#ifdef CUSTOMER_VERSION
const static CHAR  s_customer_version   [] = CUSTOMER_VERSION;
#endif

static CHAR*  ps_model_name    = "";
static CHAR*  ps_serial_number = "";

static BOOL  b_model_name_init    = FALSE;
static BOOL  b_serial_number_init = FALSE;


/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/

static INT32 set_dest_name (CHAR**       pps_dest_name,
                            const CHAR*  ps_src_name,
                            VOID**       ppv_mem_addr,
                            SIZE_T*      pz_mem_size);

#ifdef CLI_LVL_ALL
static INT32 cli_get_info (INT32         i4_argc,
                           const CHAR**  pps_argv);


static CLI_EXEC_T at_cli_start_cmd [] =
{
    {CLI_GET_INFO_LONG, CLI_GET_INFO_SHORT, cli_get_info, NULL, CLI_GET_INFO_HELP, CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};

static CLI_EXEC_T at_cli_start [] =
{
    {CLI_START_MODULE, NULL, NULL, &(at_cli_start_cmd [0]), CLI_START_MODULE_HELP, CLI_SUPERVISOR},
    END_OF_CLI_CMD_TBL
};
#endif


/*-----------------------------------------------------------------------------
 * Name: set_dest_name
 *
 * Description: This API will set the destination name given a source name.
 *              Note that the destination name will grab memory from the very
 *              beginning of the available chunk of memory.
 *
 * Inputs:  pps_dest_name  References the destination name.
 *          ps_src_name    References the source name.
 *          ppv_mem_addr   Contains the start address of the memory chunk.
 *          pz_mem_size    Contains the size (in Bytes) of the chunk of memory.
 *
 * Outputs: ppv_mem_addr  Contains the updated start address of the chunk of
 *                        memory.
 *          pz_mem_size   Contains the updated size (in Bytes) of the chunk of
 *                        memory.
 *
 * Returns: INITR_OK              Successful.
 *          INITR_NOT_ENOUGH_MEM  Not enough memory.
 ----------------------------------------------------------------------------*/
static INT32 set_dest_name (CHAR**       pps_dest_name,
                            const CHAR*  ps_src_name,
                            VOID**       ppv_mem_addr,
                            SIZE_T*      pz_mem_size)
{
    INT32  i4_return;
    VOID*  pv_new_mem_addr = NULL;
    SIZE_T z_len;
    SIZE_T z_new_mem_size;


    i4_return = INITR_NOT_ENOUGH_MEM;

    /* Adjust memory start address to a multiple of 4 Bytes. */
    if (*ppv_mem_addr != NULL)
    {
        z_len = ((UINT32) (*ppv_mem_addr)) % sizeof (UINT32);

        pv_new_mem_addr = (VOID*) (((UINT32) (*ppv_mem_addr)) + z_len);
        z_new_mem_size  = (*pz_mem_size) - z_len;
    }
    else
    {
        z_new_mem_size  = *pz_mem_size;
    }

    /* Calculate the required memory size and grab it. */
    z_len = 1;

    if (ps_src_name != NULL)
    {
        z_len += x_strlen (ps_src_name);
    }

    /* Adjust the length to the next 4 Byte boundary. */
    z_len += (sizeof (UINT32) - (z_len % sizeof (UINT32)));

    if (z_len < z_new_mem_size)
    {
        i4_return = INITR_OK;

        if (*ppv_mem_addr == NULL)
        {
            /* Allocate memory. */
            pv_new_mem_addr = x_mem_alloc (z_len);

            if (pv_new_mem_addr == NULL)
            {
                return (INITR_NOT_ENOUGH_MEM);
            }
        }

        /* Ok, enough memory available. Grab the chunk of memory and adjust */
        /* the memory address and size which shall be returned.             */
        (*pps_dest_name) = (CHAR*) pv_new_mem_addr;

        if (*ppv_mem_addr != NULL)
        {
            (*ppv_mem_addr) = (VOID*) (((UINT32) pv_new_mem_addr) + z_len);
            (*pz_mem_size)  = z_new_mem_size - z_len;
        }

        /* And copy the model name. */
        x_memset (((VOID*) (*pps_dest_name)), 0, z_len);

        if (ps_src_name != NULL)
        {
            x_strcpy ((*pps_dest_name), ps_src_name);
        }
    }

    return (i4_return);
}


#ifdef CLI_LVL_ALL
/*-----------------------------------------------------------------------------
 * Name: cli_get_info
 *
 * Description: This function is called by the CLI and handles the presentation
 *              of the build, customer and model name as well as the version
 *              info.
 *
 * Inputs:  i4_argc   Contains the number of arguments.
 *          pps_argv  References a list of arguments.
 *
 * Outputs: -
 *
 * Returns: Always CLIR_OK.
 ----------------------------------------------------------------------------*/
static INT32 cli_get_info (INT32         i4_argc,
                           const CHAR**  pps_argv)
{
    x_dbg_stmt ("CUSTOMER     : %s\n", &(s_customer   [0]));
    x_dbg_stmt ("BUILD        : %s\n", &(s_build_name [0]));
    x_dbg_stmt ("MODEL        : %s\n", ps_model_name);
    x_dbg_stmt ("VERSION      : %s\n", &(s_version    [0]));
    x_dbg_stmt ("SERIAL-NUMBER: %s\n", ps_serial_number);

    return (CLIR_OK);
}
#endif


/*-----------------------------------------------------------------------------
 * Name:  start_reg_cli
 *
 * Description: This API registers the Start module with the CLI module. The
 *              registration can only be done once the CLI module has been
 *              initialized.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: INITR_OK                   Routine successful.
 *          INITR_CANNOT_REG_WITH_CLI  Registration with CLI failed.
 ----------------------------------------------------------------------------*/
INT32 start_reg_cli (VOID)
{
    INT32 i4_return;


#ifdef CLI_LVL_ALL
    i4_return = INITR_CANNOT_REG_WITH_CLI;

    /* And register the Debug library with the CLI module. */
    if (x_cli_attach_cmd_tbl (&(at_cli_start [0]), CLI_CAT_MW, CLI_GRP_NONE) == CLIR_OK)
    {
        i4_return = INITR_OK;
    }
#else
    i4_return = INITR_OK;
#endif

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: start_set_model_name
 *
 * Description: This API will set the model name. Note that the model name will
 *              grab memory from the very beginning of the available chunk of
 *              memory.
 *
 * Inputs:  ps_src_model_name  References the initial model name.
 *          ppv_mem_addr       Contains the start address of the memory chunk.
 *          pz_mem_size        Contains the size (in Bytes) of the chunk of memory.
 *
 * Outputs: ppv_mem_addr  Contains the updated start address of the chunk of
 *                        memory.
 *          pz_mem_size   Contains the updated size (in Bytes) of the chunk of
 *                        memory.
 *
 * Returns: INITR_OK              Successful.
 *          INITR_ALREADY_INIT    Alreday initialized.
 *          INITR_INV_ARG         One or more arguments are invalid.
 *          INITR_NOT_ENOUGH_MEM  Not enough memory.
 ----------------------------------------------------------------------------*/
INT32 start_set_model_name (const CHAR*  ps_src_model_name,
                            VOID**       ppv_mem_addr,
                            SIZE_T*      pz_mem_size)
{
    INT32 i4_return;


    i4_return = INITR_ALREADY_INIT;

    if (! (b_model_name_init))
    {
        i4_return = INITR_INV_ARG;

        if ((ppv_mem_addr   != NULL) &&
            (pz_mem_size    != NULL) &&
            ((*pz_mem_size) != 0))
        {
            if ((i4_return = set_dest_name (&ps_model_name, 
                                            ps_src_model_name, 
                                            ppv_mem_addr, 
                                            pz_mem_size)) == INITR_OK)
            {
                b_model_name_init = TRUE;
            }
        }
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: start_set_serial_number
 *
 * Description: This API will set the serial number. Note that the model name 
 *              will grab memory from the very beginning of the available chunk
 *              of memory.
 *
 * Inputs:  ps_src_serial_number  References the serial number.
 *          ppv_mem_addr          Contains the start address of the memory 
 *                                chunk.
 *          pz_mem_size           Contains the size (in Bytes) of the chunk 
 *                                of memory.
 *
 * Outputs: ppv_mem_addr  Contains the updated start address of the chunk of
 *                        memory.
 *          pz_mem_size   Contains the updated size (in Bytes) of the chunk of
 *                        memory.
 *
 * Returns: INITR_OK              Successful.
 *          INITR_ALREADY_INIT    Alreday initialized.
 *          INITR_INV_ARG         One or more arguments are invalid.
 *          INITR_NOT_ENOUGH_MEM  Not enough memory.
 ----------------------------------------------------------------------------*/
INT32 start_set_serial_number (const CHAR*  ps_src_serial_number,
                               VOID**       ppv_mem_addr,
                               SIZE_T*      pz_mem_size)
{
    INT32 i4_return;


    i4_return = INITR_ALREADY_INIT;

    if (! (b_serial_number_init))
    {
        i4_return = INITR_INV_ARG;

        if ((ppv_mem_addr   != NULL) &&
            (pz_mem_size    != NULL) &&
            ((*pz_mem_size) != 0))
        {
            if ((i4_return = set_dest_name (&ps_serial_number,
                                            ps_src_serial_number,
                                            ppv_mem_addr,
                                            pz_mem_size)) == INITR_OK)
            {
                b_serial_number_init = TRUE;
            }
        }
    }

    return (i4_return);
}


/*-----------------------------------------------------------------------------
 * Name: x_sys_get_build_name
 *
 * Description: This API will return a reference to the zero terminated
 *              build name string.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference to the zero terminated version string.
 ----------------------------------------------------------------------------*/
const CHAR* x_sys_get_build_name (VOID)
{
    return (&(s_build_name [0]));
}


/*-----------------------------------------------------------------------------
 * Name: x_sys_get_customer
 *
 * Description: This API will return a reference to the zero terminated
 *              customer string.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference to the zero terminated version string.
 ----------------------------------------------------------------------------*/
const CHAR* x_sys_get_customer (VOID)
{
    return (&(s_customer [0]));
}


/*-----------------------------------------------------------------------------
 * Name: x_sys_get_model_name
 *
 * Description: This API will return a reference to the zero terminated
 *              mode name string.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference to the zero terminated version string.
 ----------------------------------------------------------------------------*/
const CHAR* x_sys_get_model_name (VOID)
{
    return ((const CHAR*) ps_model_name);
}


/*-----------------------------------------------------------------------------
 * Name: x_sys_get_version
 *
 * Description: This API will return a reference to the zero terminated
 *              version string.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference to the zero terminated version string.
 ----------------------------------------------------------------------------*/
const CHAR* x_sys_get_version (VOID)
{
    return (&(s_version [0]));
}

/*-----------------------------------------------------------------------------
 * Name: x_sys_get_customer_version
 *
 * Description: This API will return a reference to the zero terminated
 *              version string.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference to the zero terminated version string.
 ----------------------------------------------------------------------------*/
const CHAR* x_sys_get_customer_version (VOID)
{
#ifdef CUSTOMER_VERSION
    return (&(s_customer_version [0]));
#else
    return (&(s_version [0]));
#endif
}

/*-----------------------------------------------------------------------------
 * Name: x_sys_get_serial_number
 *
 * Description: This API will return a reference to the zero terminated
 *              serial number string.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference to the zero terminated serial number string.
 ----------------------------------------------------------------------------*/
const CHAR* x_sys_get_serial_number (VOID)
{
    return ((const CHAR*) ps_serial_number);
}


/*-----------------------------------------------------------------------------
 * Name: c_sys_get_build_name
 *
 * Description: This API will return a reference to the zero terminated
 *              build name string.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference to the zero terminated version string.
 ----------------------------------------------------------------------------*/
const CHAR* c_sys_get_build_name (VOID)
{
    return (&(s_build_name [0]));
}


/*-----------------------------------------------------------------------------
 * Name: c_sys_get_customer
 *
 * Description: This API will return a reference to the zero terminated
 *              customer string.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference to the zero terminated version string.
 ----------------------------------------------------------------------------*/
const CHAR* c_sys_get_customer (VOID)
{
    return (&(s_customer [0]));
}


/*-----------------------------------------------------------------------------
 * Name: c_sys_get_model_name
 *
 * Description: This API will return a reference to the zero terminated
 *              model name string.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference to the zero terminated version string.
 ----------------------------------------------------------------------------*/
const CHAR* c_sys_get_model_name (VOID)
{
    return ((const CHAR*) ps_model_name);
}


/*-----------------------------------------------------------------------------
 * Name: c_sys_get_version
 *
 * Description: This API will return a reference to the zero terminated
 *              version string.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference to the zero terminated version string.
 ----------------------------------------------------------------------------*/
const CHAR* c_sys_get_version (VOID)
{
    return (&(s_version [0]));
}

/*-----------------------------------------------------------------------------
 * Name: c_sys_get_customer_version
 *
 * Description: This API will return a reference to the zero terminated
 *              version string.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference to the zero terminated version string.
 ----------------------------------------------------------------------------*/
const CHAR* c_sys_get_customer_version (VOID)
{
#ifdef CUSTOMER_VERSION
    return (&(s_customer_version [0]));
#else
    return (&(s_version [0]));
#endif
}

/*-----------------------------------------------------------------------------
 * Name: c_sys_get_serial_number
 *
 * Description: This API will return a reference to the zero terminated
 *              serial number string.
 *
 * Inputs:  -
 *
 * Outputs: -
 *
 * Returns: A reference to the zero terminated serial number string.
 ----------------------------------------------------------------------------*/
const CHAR* c_sys_get_serial_number (VOID)
{
    return ((const CHAR*) ps_serial_number);
}

