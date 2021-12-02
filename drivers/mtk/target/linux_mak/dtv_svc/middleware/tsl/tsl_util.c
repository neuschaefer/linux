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
 * $RCSfile: tsl_util.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file implements function for specifying the TSL database name
 *    to be used for CDB, and for creating TSL record definition.
 *   
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "os/inc/x_os.h"
#include "cdb/cdb_api.h"
#include "file_mngr/x_fm.h"
#include "util/fs_raw.h"
#include "tsl/tsl.h"
#include "tsl/_tsl.h"


/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/


/**********************************************************************
   Following are private functions only known to the TSL library.
***********************************************************************/
/*-----------------------------------------------------------------------------
 * Name: tsl_get_database_name
 *
 * Description: This function create the TSL object's database name to used
 *              for the internal CDB.
 *
 * Inputs: 
 *   ps_tsl_name   Name of the TSL object. 
 *   ui2_tsl_id    Unique id for the TSL object.  
 *                 The limit for TSL ID is between 1 and 65535.
 *   
 * Outputs:
 *   ps_cdb_name   Name to used to create the CDB object
 *
 * Returns: 
 *   TSLR_OK           TSL object's CDB name is created.
 *
 ----------------------------------------------------------------------------*/
INT32 tsl_get_database_name
(
    const CHAR*      ps_tsl_name,
    UINT16           ui2_tsl_id,
    CHAR*            ps_cdb_name
) 
{
    x_sprintf(ps_cdb_name,"TSL_%s_%d.db\0",ps_tsl_name,ui2_tsl_id);
    return TSLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: tsl_cdb_to_tsl_err_code
 *
 * Description: API to translate CDB error code to TSL error code.
 *             
 *
 * Inputs:  - i4_cdb_error: CDB error code
 *
 * Outputs: - None.
 *                           
 *
 * Returns: - tanslated TSLR error code.   
 *                        
 ----------------------------------------------------------------------------*/

INT32 tsl_cdb_to_tsl_err_code(INT32 i4_cdb_error)
{
    INT32 i4_rc;
    
    switch (i4_cdb_error)
    {
        case DBR_OK:
            i4_rc=TSLR_OK;
            break;

        case DBR_FAIL:
            i4_rc=TSLR_FAIL;
            break;
            
        case DBR_OUT_OF_MEMORY:
            i4_rc=TSLR_OUT_OF_MEM;
            break;

        case DBR_INV_HANDLE:
            i4_rc=TSLR_INV_HANDLE;
            break;

        case DBR_OUT_OF_HANDLE:
            i4_rc=TSLR_OUT_OF_HANDLE;
            break;

        case DBR_NO_WRITE_LOCK:
            i4_rc=TSLR_NO_WRITE_LOCK;
            break;

        case DBR_NOT_FOUND:
            i4_rc=TSLR_NOT_FOUND;
            break;
            
        case DBR_REC_NOT_FOUND:
            i4_rc=TSLR_REC_NOT_FOUND;
            break;

        case DBR_INV_ARG:
            i4_rc=TSLR_INV_ARG;
            break;

        case DBR_MODIFIED:
            i4_rc=TSLR_MODIFIED;
            break;

        case DBR_DB_LIMIT_EXCEED:
            i4_rc=TSLR_FAIL;
            break;
            
        case DBR_FILE_WRITE_FAIL:
            i4_rc=TSLR_FILE_WRITE_FAIL;
            break;

        case DBR_FILE_READ_FAIL:
            i4_rc=TSLR_FILE_READ_FAIL;
            break;

        case DBR_END_OF_REC:
        {
            i4_rc=TSLR_END_OF_ITERATION;
        }
        break;
            
        default:
            i4_rc=TSLR_FAIL;
    }
    return i4_rc;
}



/*-----------------------------------------------------------------------------
 * Name: tsl_fm_to_tsl_err_code
 *
 * Description: API to translate File Manager error code to TSL error code.
 *             
 *
 * Inputs:  - i4_cdb_error: CDB error code
 *
 * Outputs: - None.
 *                           
 *
 * Returns: - tanslated TSLR error code.   
 *                        
 ----------------------------------------------------------------------------*/

INT32 tsl_fm_to_tsl_err_code(INT32 i4_cdb_error)
{
    INT32 i4_rc;
    
    switch (i4_cdb_error)
    {
        case FMR_OK:
            i4_rc=TSLR_OK;
            break;

        case FMR_HANDLE:
            i4_rc=TSLR_OUT_OF_HANDLE;
            break;

        case FMR_ARG:
            i4_rc=TSLR_INV_ARG;
            break;

        case FMR_NO_ENTRY:
        case FMR_NAME_TOO_LONG:
        case FMR_INVAL:
        case FMR_NO_SUCH_DEVICE:
            i4_rc=TSLR_INV_FILE_PATH;
            break;
            
        default:
            i4_rc=TSLR_FAIL;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: tsl_fsraw_to_tsl_err_code
 *
 * Description: API to translate FS RAW error code to TSL error code.
 *             
 *
 * Inputs:  - i4_cdb_error: CDB error code
 *
 * Outputs: - None.
 *                           
 *
 * Returns: - tanslated TSLR error code.   
 *                        
 ----------------------------------------------------------------------------*/

INT32 tsl_fsraw_to_tsl_err_code(INT32 i4_error)
{
    INT32 i4_rc;
    
    switch (i4_error)
    {
        case FSRAWR_OK:
            i4_rc=TSLR_OK;
            break;

        case FSRAWR_OUT_OF_SPACE:
            i4_rc=TSLR_OUT_OF_SPACE;
            break;

        case FSRAWR_NOT_FOUND:
            i4_rc=TSLR_NOT_FOUND;
            break;

        case FSRAWR_INV_ARG:
            i4_rc=TSLR_INV_ARG;
            break;

        case FSRAWR_WRITE_FAIL:
            i4_rc=TSLR_FILE_WRITE_FAIL;
            break;

        case FSRAWR_READ_FAIL:
            i4_rc=TSLR_FILE_READ_FAIL;
            break;            
            
        default:
            i4_rc=TSLR_FAIL;
    }
    return i4_rc;
}

