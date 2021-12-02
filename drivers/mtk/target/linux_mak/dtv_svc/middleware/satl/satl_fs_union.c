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
 * $RCSfile: satl_fs_union.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: This file contains endian conversion functions for
 *              the union data field in the SATL record.
 *
 *              The conversion functions convert 1) structure fields into
 *              an array of bytes (record) for output to file, and 2)
 *              an array of bytes (record) to structures when reading
 *              from file.
 *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "cdb/cdb_api.h"
#include "satl/satl.h"
#include "satl/_satl.h"
#include "satl/satl_rec.h"
#include "util/x_end_cnvt.h"
#include "satl/satl_dbg.h"

/*-------------------------------------------------------------------------
            SATL data union field  endian conversion functions
 --------------------------------------------------------------------------*/
/*----------------------------------------------------------------------
 * Name: satl_union_end_conv_from_file
 *
 * Description: This API interpret the SATL record from persistent storage
 *              and perform the necessary conversion to the
 *              structure fields.
 *
 * Inputs: 
 *   
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          file handle is successfully closed. 
 *   
 -----------------------------------------------------------------------*/
VOID satl_union_end_conv_from_file
(
    DB_FIELD_T       t_fld_type,
    DB_UNION_TAG_T   t_union_tag,
    const VOID*      pv_input,
    UINT16           ui2_size,
    VOID*            pv_output
)
{
    UINT8*           pui1_src;
    UINT8*           pui1_dest;

    pui1_src  = (UINT8*) pv_input;
    pui1_dest = (UINT8*)pv_output;

    /*
    DBG_INFO(("{SATL} File to SATL memory conversion\n"));
    */
    
    switch (t_fld_type)
    {
        case SATL_CDB_TYPE_LNB:
        {
            TUNER_LNB_INFO_T    t_lnb = {(TUNER_LNB_TYPE_T) 0};
            /*
            DBG_INFO(("{SATL} SATL LNB: size: %d tag value: %d\n",
                       ui2_size, t_union_tag));
            */
            
            /*
              convert the TUNER_LNB_INFO_T structure.
            */

            /*
              lnb type
            */
            t_lnb.e_lnb_type = (TUNER_LNB_TYPE_T) *pui1_src;
            pui1_src  += 1;
            
            /*
              This follows by 3 UINT16 fields:
              1) LNB low frequency in MHz
              2) LNB high frequency in MHz
              3) LNB switch frequency in MHz
            */
            x_end_cnvt_from_big_16(pui1_src, 1, &t_lnb.ui2_lnb_low_freq);
            pui1_src  += 2;
            x_end_cnvt_from_big_16(pui1_src, 1, &t_lnb.ui2_lnb_high_freq);
            pui1_src  += 2;
            x_end_cnvt_from_big_16(pui1_src, 1, &t_lnb.ui2_lnb_switch_freq);
            pui1_src  += 2;

            x_memcpy(pui1_dest, &t_lnb, sizeof(TUNER_LNB_INFO_T) );
        }
        break;

        case SATL_CDB_TYPE_DISEQC:
        {
            TUNER_DISEQC_INFO_T t_diseqc_info = {(TUNER_DISEQC_TYPE_T) 0};
            /*
            DBG_INFO(("{SATL} SATL DiSEqC: size: %d tag value: %d\n",
                       ui2_size, t_union_tag));
            */
            
            /*
              convert the TUNER_DISEQC_INFO_T structure.
            */

            /*
              diseqc type
            */
            t_diseqc_info.e_diseqc_type = (TUNER_DISEQC_TYPE_T) *pui1_src;
            pui1_src  += 1;
            
            /*
              This follows by 5 UINT8 fields:
              1) Port
              2) Casade type
              3) Casade port
              4) 22K band switching on/off
              5) Tone burst off/position
            */
            t_diseqc_info.ui1_port          = *pui1_src++;
            t_diseqc_info.ui1_cascade_type  = *pui1_src++;
            t_diseqc_info.ui1_cascade_port  = *pui1_src++;
            t_diseqc_info.ui1_22k           = *pui1_src++;
            t_diseqc_info.ui1_tone_burst    = *pui1_src++;
            
            /*
              polar
            */
            t_diseqc_info.e_polar = (TUNER_POLARIZATION_T) *pui1_src;
            pui1_src  += 1;
            
            /*
              wxyz
            */
            x_end_cnvt_from_big_16(pui1_src, 1, &t_diseqc_info.ui2_wxyz);
            pui1_src  += 2;

            x_memcpy(pui1_dest, &t_diseqc_info, sizeof(TUNER_DISEQC_INFO_T) );
        }
        break;

        default:
        {
            DBG_ERROR(("{SATL} Error: SATL union field type: %d",t_fld_type));
        }
        break ;
    }
    return;
}

/*----------------------------------------------------------------------
 * Name: satl_union_end_conv_to_file
 *
 * Description: This API interpret the SATL structures and perform the
 *              necessary conversion to data record field.
 *
 * Inputs: 
 *   
 *
 * Outputs: None
 *
 * Returns: 
 *   SATLR_OK          file handle is successfully closed. 
 *   
 -----------------------------------------------------------------------*/
VOID satl_union_end_conv_to_file
(
    DB_FIELD_T       t_fld_type,
    DB_UNION_TAG_T   t_union_tag,
    const VOID*      pv_input,
    UINT16           ui2_size,
    VOID*            pv_output
)
{
    UINT8*           pui1_src;
    UINT8*           pui1_dest;

    pui1_src  = (UINT8*) pv_input;
    pui1_dest = (UINT8*)pv_output;
      
    
    /*   
    DBG_INFO(("{SATL} SATL memory to File conversion\n"));
    */
    
    
    switch (t_fld_type)
    {
        case SATL_CDB_TYPE_LNB:
        {
            TUNER_LNB_INFO_T    t_lnb;
            /*
            DBG_INFO(("{SATL} SATL LNB: size: %d tag value: %d\n",
                       ui2_size, t_union_tag));
            */
            
            x_memcpy(&t_lnb, pui1_src, sizeof(TUNER_LNB_INFO_T));
            /*
              convert the TUNER_LNB_INFO_T structure.
            */

            /*
              lnb type
            */
            *((UINT8*)pui1_dest) = (UINT8) t_lnb.e_lnb_type ;
            pui1_dest += 1;
            
            /*
              This follows by 3 UINT16 fields:
              1) LNB low frequency in MHz
              2) LNB high frequency in MHz
              3) LNB switch frequency in MHz
            */
            x_end_cnvt_to_big_16(&t_lnb.ui2_lnb_low_freq   , 1, pui1_dest);
            pui1_dest += 2;                                
            x_end_cnvt_to_big_16(&t_lnb.ui2_lnb_high_freq  , 1, pui1_dest);
            pui1_dest += 2;                                
            x_end_cnvt_to_big_16(&t_lnb.ui2_lnb_switch_freq, 1, pui1_dest);
            pui1_dest += 2;
        }
        break;

        case SATL_CDB_TYPE_DISEQC:
        {
            TUNER_DISEQC_INFO_T t_diseqc_info;
            /*
            DBG_INFO(("{SATL} SATL DiSEqC: size: %d tag value: %d\n",
                       ui2_size, t_union_tag));
            */
            
            x_memcpy(&t_diseqc_info, pui1_src, sizeof(TUNER_DISEQC_INFO_T));
            /*
              convert the TUNER_DISEQC_INFO_T structure.
            */

            /*
              diseqc type
            */
            *((UINT8*)pui1_dest) = (UINT8) t_diseqc_info.e_diseqc_type ;
            pui1_dest += 1;
            
            /*
              This follows by 5 UINT8 fields:
              1) Port
              2) Casade type
              3) Casade port
              4) 22K band switching on/off
              5) Tone burst off/position
            */
            *pui1_dest++ = t_diseqc_info.ui1_port         ;
            *pui1_dest++ = t_diseqc_info.ui1_cascade_type ;
            *pui1_dest++ = t_diseqc_info.ui1_cascade_port ;
            *pui1_dest++ = t_diseqc_info.ui1_22k          ;
            *pui1_dest++ = t_diseqc_info.ui1_tone_burst   ;
            
            /*
              polar
            */
            *((UINT8*)pui1_dest) = (UINT8) t_diseqc_info.e_polar ;
            pui1_dest += 1;
            
            /*
              wxyz
            */
            x_end_cnvt_to_big_16(&t_diseqc_info.ui2_wxyz, 1, pui1_dest);
            pui1_dest += 2;
        }
        break;

        default:
        {
            DBG_ERROR(("{SATL} Error: SATL record union field type: %d",t_fld_type));
        }
        break ;
    }
    return;
}
