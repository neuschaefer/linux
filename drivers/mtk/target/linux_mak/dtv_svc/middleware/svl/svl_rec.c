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
 * $RCSfile: svl_rec.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *    This file implements function for specifying the SVL database name
 *    to be used for CDB, and for creating SVL record definition.
 *   
 *         
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
                    include files
----------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "handle/x_handle.h"
#include "os/inc/x_os.h"
#include "util/x_end_cnvt.h"
#include "svl_bldr/u_sb.h"
#include "cdb/cdb_api.h"
#include "svl/svl.h"
#include "svl/_svl.h"
#include "svl/svl_rec.h"

/*-------------------------------------------------------------------------
                    functions implementation
 --------------------------------------------------------------------------*/

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((SIZE_T) &((TYPE *)0)->MEMBER)
#endif

/**********************************************************************
   Following are private functions only known to the SVL library.
***********************************************************************/

#define  SVL_PRIVATE_FIELD_TYPES       SVL_CDB_TYPE_PREV_REC_ID, \
                                       SVL_CDB_TYPE_NEXT_REC_ID,
#ifdef ANDROID
#define  SVL_REC_FIELD_TYPES           SVL_CDB_TYPE_REC_ID,      \
                                       SVL_CDB_TYPE_CHANNEL_ID,  \
                                       SVL_CDB_TYPE_NETMASK_ID,  \
                                       SVL_CDB_TYPE_OPT_MASK_ID, \
                                       SVL_CDB_TYPE_PROG_ID,     \
                                       SVL_CDB_TYPE_TSL_ID,      \
                                       SVL_CDB_TYPE_TSL_REC_ID,  \
                                       SVL_CDB_TYPE_NWL_ID,      \
                                       SVL_CDB_TYPE_NWL_REC_ID,  \
                                       SVL_CDB_TYPE_SATL_ID,     \
                                       SVL_CDB_TYPE_SATL_REC_ID, \
                                       SVL_CDB_TYPE_BRDCST_T,    \
                                       SVL_CDB_TYPE_SERVICE_T,   \
                                       SVL_CDB_TYPE_AC_NAME,     \
                                       SVL_CDB_TYPE_CUSTOM_DATA, \
                                       SVL_CDB_TYPE_ANDROID_PRIVATE_DATA,     \
                                       SVL_CDB_TYPE_UNION_TAG,   \
                                       SVL_CDB_TYPE_DATA
#else
#define  SVL_REC_FIELD_TYPES           SVL_CDB_TYPE_REC_ID,      \
                                       SVL_CDB_TYPE_CHANNEL_ID,  \
                                       SVL_CDB_TYPE_NETMASK_ID,  \
                                       SVL_CDB_TYPE_OPT_MASK_ID, \
                                       SVL_CDB_TYPE_PROG_ID,     \
                                       SVL_CDB_TYPE_TSL_ID,      \
                                       SVL_CDB_TYPE_TSL_REC_ID,  \
                                       SVL_CDB_TYPE_NWL_ID,      \
                                       SVL_CDB_TYPE_NWL_REC_ID,  \
                                       SVL_CDB_TYPE_SATL_ID,     \
                                       SVL_CDB_TYPE_SATL_REC_ID, \
                                       SVL_CDB_TYPE_BRDCST_T,    \
                                       SVL_CDB_TYPE_SERVICE_T,   \
                                       SVL_CDB_TYPE_AC_NAME,     \
                                       SVL_CDB_TYPE_CUSTOM_DATA, \
                                       SVL_CDB_TYPE_UNION_TAG,   \
                                       SVL_CDB_TYPE_DATA
#endif

static DB_FIELD_T     aui4_db_recfld_type[SVL_DB_NUM_OF_FIELDS] =
{
    /*
       private data for linked list of SVL record ID.
	 */
    (DB_FIELD_T) SVL_CDB_TYPE_PREV_REC_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_NEXT_REC_ID,
    /*
       Field types for the SVL record.
	 */
    (DB_FIELD_T) SVL_CDB_TYPE_REC_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_CHANNEL_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_NETMASK_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_OPT_MASK_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_PROG_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_TSL_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_TSL_REC_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_NWL_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_NWL_REC_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_SATL_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_SATL_REC_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_BRDCST_T,
    (DB_FIELD_T) SVL_CDB_TYPE_SERVICE_T,
    (DB_FIELD_T) SVL_CDB_TYPE_AC_NAME,
    (DB_FIELD_T) SVL_CDB_TYPE_CUSTOM_DATA,
#ifdef ANDROID
    (DB_FIELD_T) SVL_CDB_TYPE_ANDROID_PRIVATE_DATA,
#endif
    (DB_FIELD_T) SVL_CDB_TYPE_UNION_TAG,
    (DB_FIELD_T) SVL_CDB_TYPE_DATA,
    /*
      Client's private data field.
    */
    (DB_FIELD_T) SVL_CDB_TYPE_CLIENT_DATA_TAG,
    (DB_FIELD_T) SVL_CDB_TYPE_CLIENT_DATA,

    /*multi-language*/
    (DB_FIELD_T) SVL_CDB_TYPE_MULTI_LANG_TAG,
    (DB_FIELD_T) SVL_CDB_TYPE_MULTI_LANG
};

static DB_FIELD_INFO_T  at_db_recfld_info[SVL_DB_NUM_OF_FIELDS] =
{
    /* field types private data for record link list. */
    { (DB_FIELD_T) SVL_CDB_TYPE_PREV_REC_ID,    1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_NEXT_REC_ID,    1 },

    /* SVL record field type */  /* number of element per field. */
    { (DB_FIELD_T) SVL_CDB_TYPE_REC_ID,         1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_CHANNEL_ID,     1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_NETMASK_ID,     1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_OPT_MASK_ID,    1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_PROG_ID,        1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_TSL_ID,         1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_TSL_REC_ID,     1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_NWL_ID,         1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_NWL_REC_ID,     1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_SATL_ID,        1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_SATL_REC_ID,    1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_BRDCST_T,       1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_SERVICE_T,      1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_AC_NAME,        MAX_PROG_NAME_LEN+1  },
    { (DB_FIELD_T) SVL_CDB_TYPE_CUSTOM_DATA,    MW_SVL_HEADER_REC_CUSTOM_BYTES  },    
#ifdef ANDROID
    { (DB_FIELD_T) SVL_CDB_TYPE_ANDROID_PRIVATE_DATA,        20  },
#endif
    { (DB_FIELD_T) SVL_CDB_TYPE_UNION_TAG,      1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_DATA,           SVL_REC_DATA_PADDING },
    { (DB_FIELD_T) SVL_CDB_TYPE_CLIENT_DATA_TAG,1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_CLIENT_DATA,    255 },
    { (DB_FIELD_T) SVL_CDB_TYPE_MULTI_LANG_TAG, 1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_MULTI_LANG,     255 },
};

static DB_FIELD_T        aui4_db_keyfld_type[SVL_DB_NUM_OF_KEY_FIELDS] =
{
    (DB_FIELD_T) SVL_CDB_TYPE_REC_ID
};

static DB_FIELD_T        aui4_svl_recfld_type[SVL_NUM_OF_REC_FIELDS] =
{
    /*
       Field types for the SVL record.
	 */
    (DB_FIELD_T) SVL_CDB_TYPE_REC_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_CHANNEL_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_NETMASK_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_OPT_MASK_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_PROG_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_TSL_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_TSL_REC_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_NWL_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_NWL_REC_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_SATL_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_SATL_REC_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_BRDCST_T,
    (DB_FIELD_T) SVL_CDB_TYPE_SERVICE_T,
    (DB_FIELD_T) SVL_CDB_TYPE_AC_NAME,
    (DB_FIELD_T) SVL_CDB_TYPE_CUSTOM_DATA,
#ifdef ANDROID
    (DB_FIELD_T) SVL_CDB_TYPE_ANDROID_PRIVATE_DATA,
#endif
    (DB_FIELD_T) SVL_CDB_TYPE_UNION_TAG,
    (DB_FIELD_T) SVL_CDB_TYPE_DATA
};

static DB_FIELD_INFO_T  at_svl_recfld_info[SVL_NUM_OF_REC_FIELDS] =
{
    /* SVL record field type */  /* number of element per field. */
    { (DB_FIELD_T) SVL_CDB_TYPE_REC_ID,         1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_CHANNEL_ID,     1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_NETMASK_ID,     1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_OPT_MASK_ID,    1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_PROG_ID,        1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_TSL_ID,         1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_TSL_REC_ID,     1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_NWL_ID,         1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_NWL_REC_ID,     1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_SATL_ID,        1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_SATL_REC_ID,    1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_BRDCST_T,       1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_SERVICE_T,      1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_AC_NAME,        MAX_PROG_NAME_LEN+1  },
    { (DB_FIELD_T) SVL_CDB_TYPE_CUSTOM_DATA,    MW_SVL_HEADER_REC_CUSTOM_BYTES  },
#ifdef ANDROID
    { (DB_FIELD_T) SVL_CDB_TYPE_ANDROID_PRIVATE_DATA,        20  },
#endif
    { (DB_FIELD_T) SVL_CDB_TYPE_UNION_TAG,      1 },
    { (DB_FIELD_T) SVL_CDB_TYPE_DATA,           SVL_REC_DATA_PADDING },
};

static DB_FIELD_T        aui4_chan_nw_type[2] =
{
    (DB_FIELD_T) SVL_CDB_TYPE_CHANNEL_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_NETMASK_ID
};

static DB_FIELD_T     aui4_prv_and_svl_recfld_type[]=
{
    /*
       private data for linked list of SVL record ID.
	*/
    (DB_FIELD_T) SVL_CDB_TYPE_PREV_REC_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_NEXT_REC_ID,
    /*
       Field types for the SVL record.
	 */
    (DB_FIELD_T) SVL_CDB_TYPE_REC_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_CHANNEL_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_NETMASK_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_OPT_MASK_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_PROG_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_TSL_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_TSL_REC_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_NWL_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_NWL_REC_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_SATL_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_SATL_REC_ID,
    (DB_FIELD_T) SVL_CDB_TYPE_BRDCST_T,
    (DB_FIELD_T) SVL_CDB_TYPE_SERVICE_T,
    (DB_FIELD_T) SVL_CDB_TYPE_AC_NAME,
    (DB_FIELD_T) SVL_CDB_TYPE_CUSTOM_DATA,
#ifdef ANDROID
    (DB_FIELD_T) SVL_CDB_TYPE_ANDROID_PRIVATE_DATA,
#endif
    (DB_FIELD_T) SVL_CDB_TYPE_UNION_TAG,
    (DB_FIELD_T) SVL_CDB_TYPE_DATA
};

/*-----------------------------------------------------------------------------
 * Name: svl_db_rec_and_key_def
 *
 * Description: This function get the record field and key field
 * definitions that will be used to store record entry in CDB object.
 *
 *    The CDB record for the SVL data is defined by the following 
 *    record fields:
 *          field                               data type
 *       <ui2_prev_rec_id>                       UINT16
 *       <ui2_next_rec_id>                       UINT16
 *
 *       <ui2_svl_rec_id>                        UINT16
 *       <ui4_channel_id>                        UINT32
 *       <ui4_nw_mask>                           UINT32
 *       <ui4_option_mask>                       UINT32
 *       <ui2_prog_id>                           UINT16
 *       <ui2_tsl_id>                            UINT16
 *       <ui2_tsl_rec_id>	                     UINT16
 *       <ui2_nwl_id>                            UINT16
 *       <ui2_nwl_rec_id>	                     UINT16
 *       <ui2_satl_id>                           UINT16
 *       <ui2_satl_rec_id>	                     UINT16
 *       <e_brdcst_type>                         UINT8
 *       <e_serv_type>                           UINT8
 *       <ac_name<[MAX_PROG_NAME_LEN+1]>         String_ID (UINT16) 
 *       <data_type_union_tag>                   (4 bytes)
 *       <data>   byte stream                    (24 bytes)
 *
 *
 *       <SVL_CLIENT_DATA_T->ui2_data_tag        UINT16
 *       <SVL_CLIENT_DATA_T->pui1_data_buf       Memory object ID (UINT16)
 *
 * Inputs: 
 *    None.
 *
 * Outputs:
 *    pui1_fld_count:  number of fields in the record.  
 *    ppt_rec_fields:  pointer to an array containing field info structure.
 *                     each field info structure element contains the
 *                     field Type and the number of element for that field
 *                     type.
 *    ppui4_recfld_type: pointer to an array of record field type.
 *    pui1_key_count:  number of the keys in the record.
 *    ppui4_keyfld_type: pointer to an array of key field type.
 *
 * Returns: 
 *   SVLR_OK       A valid SVL record definition is returned
 *   
 ----------------------------------------------------------------------------*/
INT32 svl_db_rec_and_key_def
(
    UINT8*             pui1_fld_count,
    DB_FIELD_INFO_T**  ppt_rec_fields,
    DB_FIELD_T**       ppui4_recfld_type,
    UINT8*             pui1_key_count,
    DB_FIELD_T**       ppui4_keyfld_type
)
{
    /* DB Record field definition. */
    *pui1_fld_count=(UINT8)SVL_DB_NUM_OF_FIELDS;
    *ppt_rec_fields=at_db_recfld_info;
    *ppui4_recfld_type=aui4_db_recfld_type;

    /* DB Key field definition. */
    *pui1_key_count=(UINT8)SVL_DB_NUM_OF_KEY_FIELDS;
    *ppui4_keyfld_type=aui4_db_keyfld_type;
    
    return SVLR_OK;
}

INT32 svl_get_rec_def
(
    UINT8*             pui1_fld_count,
    DB_FIELD_INFO_T**  ppt_rec_fields,
    DB_FIELD_T**       ppui4_recfld_type
)
{
    /* Record field definition. */
    *pui1_fld_count=(UINT8)SVL_NUM_OF_REC_FIELDS;
    *ppt_rec_fields=at_svl_recfld_info;
    *ppui4_recfld_type=aui4_svl_recfld_type;
    return SVLR_OK;
}

INT32 svl_get_prv_and_svlrec_def
(
    UINT8*             pui1_fld_count,
    DB_FIELD_T**       ppui4_recfld_type
)
{
    /* Record field definition. */
    *pui1_fld_count=(UINT8)(SVL_NUM_PRIVATE_FIELDS + SVL_NUM_OF_REC_FIELDS);
    *ppui4_recfld_type=aui4_prv_and_svl_recfld_type;
    return SVLR_OK;
}

INT32 svl_get_key_def
(
    UINT8*             pui1_key_count,
    DB_FIELD_T**       ppui4_keyfld_type
)
{
    /* Key field definition. */
    *pui1_key_count=(UINT8)SVL_DB_NUM_OF_KEY_FIELDS;
    *ppui4_keyfld_type=aui4_db_keyfld_type;
    return SVLR_OK;
}

INT32 svl_get_chan_nwmask_def
(
    UINT8*             pui1_key_count,
    DB_FIELD_T**       ppui4_chan_nwmask_type
)
{
    /* Key field definition. */
    *pui1_key_count=(UINT8)2;
    *ppui4_chan_nwmask_type=aui4_chan_nw_type;
    
    return SVLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name: svl_rec_fld_to_addr
 *
 * Description: This function returns the address of each field for the
 *    specified SVL record structure. 
 *
 * Inputs: 
 *    pt_svl_rec:  pointer to the record structure.
 *    i2_i:        index location in the 'apv_rec_fld_addr' array to begin
 *                 writing the SVL record field address.
 *
 *    pui4_buf     Pointer to a INT32 buffer for storing UNION_TAG value.
 *
 * Outputs:
 *    apv_rec_fld_addr   Address of the rec field(s)
 *    aui2_addr_len: Optional array to store the size of the address location
 *                   in 'apv_rec_fld_addr'.
 *
 * Returns: 
 *   TRUE      SVL ID/Name is not currently in the database..
 *   FALSE     Invalid SVL ID/Name.
 ----------------------------------------------------------------------------*/
INT32 svl_rec_fld_to_addr
(
    const SVL_REC_T*   pt_svl_rec,
    const VOID*        apv_rec_fld_addr[],
    UINT16             aui2_addr_len[],
    UINT16             ui2_i,
    INT32*             p_union_tag
)
{   
    if ((aui2_addr_len == NULL && ui2_i > 0) || ui2_i > SVL_NUM_PRIVATE_FIELDS)
    {  /* try to fix Klocwork issue */
        return SVLR_FAIL;
    }

    apv_rec_fld_addr[ui2_i+0]=&(pt_svl_rec->ui2_svl_rec_id);
    apv_rec_fld_addr[ui2_i+1]=&(pt_svl_rec->uheader.t_rec_hdr.ui4_channel_id);
    apv_rec_fld_addr[ui2_i+2]=&(pt_svl_rec->uheader.t_rec_hdr.ui4_nw_mask);
    apv_rec_fld_addr[ui2_i+3]=&(pt_svl_rec->uheader.t_rec_hdr.ui4_option_mask);
    apv_rec_fld_addr[ui2_i+4]=&(pt_svl_rec->uheader.t_rec_hdr.ui2_prog_id);
    apv_rec_fld_addr[ui2_i+5]=&(pt_svl_rec->uheader.t_rec_hdr.ui2_tsl_id);
    apv_rec_fld_addr[ui2_i+6]=&(pt_svl_rec->uheader.t_rec_hdr.ui2_tsl_rec_id);
    apv_rec_fld_addr[ui2_i+7]=&(pt_svl_rec->uheader.t_rec_hdr.ui2_nwl_id);
    apv_rec_fld_addr[ui2_i+8]=&(pt_svl_rec->uheader.t_rec_hdr.ui2_nwl_rec_id);
    apv_rec_fld_addr[ui2_i+9]=&(pt_svl_rec->uheader.t_rec_hdr.ui2_satl_id);
    apv_rec_fld_addr[ui2_i+10]=&(pt_svl_rec->uheader.t_rec_hdr.ui2_satl_rec_id);
    apv_rec_fld_addr[ui2_i+11]=&(pt_svl_rec->uheader.t_rec_hdr.e_brdcst_type);
    apv_rec_fld_addr[ui2_i+12]=&(pt_svl_rec->uheader.t_rec_hdr.e_serv_type);
    apv_rec_fld_addr[ui2_i+13]=(pt_svl_rec->uheader.t_rec_hdr.ac_name);
    apv_rec_fld_addr[ui2_i+14]=(pt_svl_rec->uheader.t_rec_hdr.aui1_custom_data);
#ifdef ANDROID
    apv_rec_fld_addr[ui2_i+15]=(pt_svl_rec->uheader.t_rec_hdr.ai1_private_data);
    apv_rec_fld_addr[ui2_i+16]=(p_union_tag);
    apv_rec_fld_addr[ui2_i+17]=&(pt_svl_rec->u_data);
#else
    apv_rec_fld_addr[ui2_i+15]=(p_union_tag);
    apv_rec_fld_addr[ui2_i+16]=&(pt_svl_rec->u_data);
#endif

    if ( aui2_addr_len != NULL )
    {
        aui2_addr_len[ui2_i+0] = (UINT16) sizeof(pt_svl_rec->ui2_svl_rec_id);
        aui2_addr_len[ui2_i+1] = (UINT16) sizeof(pt_svl_rec->uheader.t_rec_hdr.ui4_channel_id);
        aui2_addr_len[ui2_i+2] = (UINT16) sizeof(pt_svl_rec->uheader.t_rec_hdr.ui4_nw_mask);
        aui2_addr_len[ui2_i+3] = (UINT16) sizeof(pt_svl_rec->uheader.t_rec_hdr.ui4_option_mask);
        aui2_addr_len[ui2_i+4] = (UINT16) sizeof(pt_svl_rec->uheader.t_rec_hdr.ui2_prog_id);
        aui2_addr_len[ui2_i+5] = (UINT16) sizeof(pt_svl_rec->uheader.t_rec_hdr.ui2_tsl_id);
        aui2_addr_len[ui2_i+6] = (UINT16) sizeof(pt_svl_rec->uheader.t_rec_hdr.ui2_tsl_rec_id);
        aui2_addr_len[ui2_i+7] = (UINT16) sizeof(pt_svl_rec->uheader.t_rec_hdr.ui2_nwl_id);
        aui2_addr_len[ui2_i+8] = (UINT16) sizeof(pt_svl_rec->uheader.t_rec_hdr.ui2_nwl_rec_id);
        aui2_addr_len[ui2_i+9] = (UINT16) sizeof(pt_svl_rec->uheader.t_rec_hdr.ui2_satl_id);
        aui2_addr_len[ui2_i+10] = (UINT16) sizeof(pt_svl_rec->uheader.t_rec_hdr.ui2_satl_rec_id);
        aui2_addr_len[ui2_i+11] = (UINT16) sizeof(pt_svl_rec->uheader.t_rec_hdr.e_brdcst_type);
        aui2_addr_len[ui2_i+12] = (UINT16) sizeof(pt_svl_rec->uheader.t_rec_hdr.e_serv_type);
        aui2_addr_len[ui2_i+13] = (UINT16) sizeof(pt_svl_rec->uheader.t_rec_hdr.ac_name);
        aui2_addr_len[ui2_i+14] = (UINT16) sizeof(pt_svl_rec->uheader.t_rec_hdr.aui1_custom_data);
#ifdef ANDROID
        aui2_addr_len[ui2_i+15] = (UINT16) sizeof(pt_svl_rec->uheader.t_rec_hdr.ai1_private_data);
        aui2_addr_len[ui2_i+16] = (UINT16) sizeof(INT32);
        aui2_addr_len[ui2_i+17] = (UINT16) sizeof(pt_svl_rec->u_data);
#else
        aui2_addr_len[ui2_i+15] = (UINT16) sizeof(INT32);
        aui2_addr_len[ui2_i+16] = (UINT16) sizeof(pt_svl_rec->u_data);
#endif
    }
    
    return SVLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: svl_get_db_fld_addr
 *
 * Description: This function returns the an array of addresses for
 *    the private data field and SVL record fields.
 *
 * Inputs:
 *    pt_rec_id_link: pointer to the private data.
 *    pt_svl_rec:  pointer to the record structure.
 *    
 *
 * Outputs:
 *    apv_rec_fld_addr   Address of the rec field(s). The size of
 *                       of this array must be number of record fields +
 *                       the number of private fields.
 *
 * Returns: 
 *   TRUE      SVL ID/Name is not currently in the database..
 *   FALSE     Invalid SVL ID/Name.
 ----------------------------------------------------------------------------*/
INT32 svl_get_db_fld_addr
(
    const SVL_REC_ID_LINK_T*  pt_rec_id_link,
    const SVL_REC_T*          pt_svl_rec,
    const VOID*               apv_rec_fld_addr[],
    UINT16                    aui2_addr_len[],
    INT32*                    p_union_tag
)
{ 
    /* copy the address of private data to the address array. */  
    apv_rec_fld_addr[0]=&(pt_rec_id_link->ui2_prev);
    apv_rec_fld_addr[1]=&(pt_rec_id_link->ui2_next);

    aui2_addr_len[0]=2;
    aui2_addr_len[1]=2;
    
    /* copy the SVL record field address to the address array, with
       starting location of address of array at 2. */
    svl_rec_fld_to_addr(pt_svl_rec,apv_rec_fld_addr, aui2_addr_len,
                        SVL_NUM_PRIVATE_FIELDS, p_union_tag);

    return SVLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: svl_get_db_key_addr
 *
 * Description: This function returns the address of key field for the
 *    specified SVL record structure. 
 *
 * Inputs: 
 *    pt_svl_rec:  pointer to the record structure.
 *    
 *
 * Outputs:
 *    apv_key_fld_addr:  Address of the key field(s)
 *
 * Returns: 
 *   SVLR_OK     SVL key field address is returned.
 *   
 ----------------------------------------------------------------------------*/
INT32 svl_get_db_key_addr
(
    const SVL_REC_T*   pt_svl_rec,
    const VOID*        apv_key_fld_addr[]
)
{
    apv_key_fld_addr[0]=&(pt_svl_rec->ui2_svl_rec_id);
    return SVLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: svl_chan_nwmask_fld_to_addr
 *
 * Description: This function returns the address of  the
 *    specified channel number and network_mask field in
 *    the SVL record structure. 
 *
 * Inputs: 
 *    pt_svl_rec:  pointer to the record structure.
 *    
 *
 * Outputs:
 *    apv_key_fld_addr:  Address of the key field(s)
 *
 * Returns: 
 *   TRUE      SVL ID/Name is not currently in the database..
 *   FALSE     Invalid SVL ID/Name.
 ----------------------------------------------------------------------------*/
INT32 svl_chan_nwmask_fld_to_addr
(
    const SVL_REC_T*   pt_svl_rec,
    const VOID*        apv_chan_nwmask_fld_addr[]
)
{
    apv_chan_nwmask_fld_addr[0]=&(pt_svl_rec->uheader.t_rec_hdr.ui4_channel_id);
    apv_chan_nwmask_fld_addr[1]=&(pt_svl_rec->uheader.t_rec_hdr.ui4_nw_mask);
    return SVLR_OK;
}

/*----------------------------------------------------------------------
 * Name: svl_data_conv_to_big_end
 *
 * Description: This API is called by the CDB to perform endian
 *              conversion on the union component of the data record
 *              field, when performing writing operation
 *              to the file system (to big-endian)
 *
 *  ******************************************************
 *  IMPORTANT: if the persistent data saved to file in
 *             this function is changed, then the function 
 *             'svl_is_updated_flds_persistent_data()'
 *             MUST be changed to match
 *             the fields that are stored. 
 *  *******************************************************
 *
 * Inputs: 
 *   
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           SVL library is initialized successfully.
 *   
 -----------------------------------------------------------------------*/
VOID svl_data_conv_to_big_end
(
   DB_FIELD_T	 	 t_fld_type,
   DB_UNION_TAG_T	 t_union_tag,
   const VOID*	     pv_input,
   UINT16		     ui2_size,
   VOID*             pv_output    
)
{
    UINT8*           pui1_input;
    UINT8*           pui1_output;
    
    if ( t_fld_type == ((DB_FIELD_T)SVL_CDB_TYPE_DATA) )
    {
        switch (t_union_tag)
        {
            case BRDCST_TYPE_ANALOG:
            {
                SVL_ANALOG_T*    pt_analog;
                UINT16           ui2_src_id;
                UINT16           ui2_vps_ni_code;
                UINT16           ui2_ttx_fmt_1_ni_code;
                UINT16           ui2_ttx_fmt_2_cni_code;
                UINT32           ui4_input_val;
                UINT8            ui1_valid_src_id;
                UINT8            ui1_color_sys_val;
                UINT8            ui1_aud_mts;
                UINT8            ui1_auto_fine_tune;
                UINT16           ui2_aft_offset;
                
                /*
                  cast the input data buffer to a SVL_ANALOG_T structure.
                  Do this so we can access the field with in the structure
                  and avoid the problem of field alignment.
                */
                pt_analog   = (SVL_ANALOG_T*) pv_input;
                pui1_output = (UINT8*) pv_output;
                
                /* analog data contains a stream component mask
                   (UINT32), a source id (UINT16), and source id
                   validity flag (BOOL) values.
                */
                ui4_input_val = pt_analog->ui4_s_comp_mask ;
                x_end_cnvt_to_big_32(&ui4_input_val, 1, pui1_output);
                pui1_output += 4 ;

                /*
                  TV system bitmask: UINT32 value, endian conversion needed.
                */
                ui4_input_val = pt_analog->ui4_tv_sys ;
                x_end_cnvt_to_big_32(&ui4_input_val, 1, pui1_output);
                pui1_output += 4 ;

                /*
                  Audio system bitmask: UINT32 value, endian conversion needed.
                */
                ui4_input_val = pt_analog->ui4_audio_sys;
                x_end_cnvt_to_big_32(&ui4_input_val, 1, pui1_output);
                pui1_output += 4 ;

                /*
                  For EU 
                */
                ui2_vps_ni_code = pt_analog->ui2_vps_ni_code;
                x_end_cnvt_to_big_16(&ui2_vps_ni_code,1,&ui2_vps_ni_code);
                x_memcpy(pui1_output, &ui2_vps_ni_code, 2);
                pui1_output += 2 ;
                
                ui2_ttx_fmt_1_ni_code = pt_analog->ui2_ttx_fmt_1_ni_code;
                x_end_cnvt_to_big_16(&ui2_ttx_fmt_1_ni_code,1,&ui2_ttx_fmt_1_ni_code);
                x_memcpy(pui1_output,&ui2_ttx_fmt_1_ni_code, 2);
                pui1_output += 2 ;
                
                ui2_ttx_fmt_2_cni_code = pt_analog->ui2_ttx_fmt_2_cni_code;
                x_end_cnvt_to_big_16(&ui2_ttx_fmt_2_cni_code,1,&ui2_ttx_fmt_2_cni_code);
                x_memcpy(pui1_output,&ui2_ttx_fmt_2_cni_code, 2);
                pui1_output += 2 ;

                /*  t_eu_data_mask is 1 byte */
                *((UINT8*)pui1_output) = (UINT8) (pt_analog->t_eu_data_mask);
                pui1_output += 1 ;         
                
                /* vps data type is 1 byte. */
                *((UINT8*)pui1_output) = (UINT8) (pt_analog->ui1_vps_data_byte_5);
                pui1_output += 1 ;    
                
                /*
                  video color system: COLOR_SYS_T. This value
                  can be one byte (ARM) or 4 bytes (PC).  Save
                  this value as one byte value since we know there
                  are only a few color system definition.
                */
                ui1_color_sys_val = (UINT8)(pt_analog->e_vid_color_sys);
                *((UINT8*) pui1_output) = ui1_color_sys_val;
                pui1_output += 1 ;
             
                /*
                  source id value.
                */
                ui2_src_id = pt_analog->ui2_src_id;
                x_end_cnvt_to_big_16(&ui2_src_id,1,&ui2_src_id);
                x_memcpy(pui1_output, &ui2_src_id, 2);
                pui1_output += 2 ;

                /*
                  source id validity flag: one byte, so no conversion
                  is needed, just copy to output buffer.
                */
                ui1_valid_src_id = pt_analog->b_valid_src_id;
                *((UINT8*) pui1_output) = ui1_valid_src_id;
                pui1_output += 1 ;                

                /*
                  auto fine tune flag: one byte, direct copy
                */
                ui1_auto_fine_tune = pt_analog->b_no_aft;
                *((UINT8*) pui1_output) = ui1_auto_fine_tune;
                pui1_output += 1 ;

                /*
                  auto fine tune offset
                */
                ui2_aft_offset = (UINT16) pt_analog->i2_aft_offset;
                x_end_cnvt_to_big_16(&ui2_aft_offset,1,&ui2_aft_offset);
                x_memcpy(pui1_output, &ui2_aft_offset, 2);
                pui1_output += 2 ;

#ifdef MW_ISDB_SUPPORT
                /*
                  schedule block settings
                */
                x_end_cnvt_to_big_16(&(pt_analog->ui2_schedule_num),1,pui1_output);
                pui1_output += 2 ;

                x_end_cnvt_to_big_16(&(pt_analog->at_schedule),
                                     SVL_ANALOG_MAX_NUM_SCHEDULE * 4,
                                     pui1_output);
                pui1_output += 2 * SVL_ANALOG_MAX_NUM_SCHEDULE * 4;
#endif

                /*
                  audio MTS selection
                */
                ui1_aud_mts = pt_analog->ui1_aud_mts;
                *((UINT8*) pui1_output) = ui1_aud_mts;
                pui1_output += 1 ;
            }
            break;

            case BRDCST_TYPE_DVB:
            {
                SVL_DVB_T*    pt_dvb;
                UINT32        ui4_input_val;
                UINT16        ui2_input_val;

                pt_dvb   = (SVL_DVB_T*) pv_input;
                pui1_output = (UINT8*) pv_output;
                /*
                  currently, only following DVB data are stored to
                  flash, a UINT32 reserve value, DVB short service name,
                  ca_system_id, t_hd_simulcast_on_ts_svc_id_info.
                  t_orig_svc_info, t_sbtl_pref, ui1_downmix_mode.
                */
                ui4_input_val = pt_dvb->ui4_reserve;
                x_end_cnvt_to_big_32(&ui4_input_val,1,pui1_output);
                pui1_output += 4 ;

                /*
                  DVB short service name.
                */
                x_memcpy(pui1_output, pt_dvb->s_svc_name,
                         MAX_DVB_SERVICE_NAME_LEN+1);
                pui1_output +=  (MAX_DVB_SERVICE_NAME_LEN+1);

                /*
                  CA system ID ui2_ca_system_id
                */
                ui2_input_val = pt_dvb->ui2_ca_system_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

                /*
                  Simulcast information:
                  
                  t_hd_simulcast_on_ts_svc_id_info:
                  
                  This field is a structure containing ui2_on_id, ui2_ts_id,
                  and ui2_svc_id.
                */
                ui2_input_val = pt_dvb->t_hd_simulcast_on_ts_svc_id_info.ui2_on_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

                ui2_input_val = pt_dvb->t_hd_simulcast_on_ts_svc_id_info.ui2_ts_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

                ui2_input_val = pt_dvb->t_hd_simulcast_on_ts_svc_id_info.ui2_svc_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

                /*
                  t_orig_svc_info field:
                  
                  original service information for this service.
                */
                ui2_input_val = pt_dvb->t_orig_svc_info.ui2_on_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

                ui2_input_val = pt_dvb->t_orig_svc_info.ui2_ts_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

                ui2_input_val = pt_dvb->t_orig_svc_info.ui2_svc_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

                
                /*
                  t_sbtl_pref field:
                  
                  user subtitle preference for this service.
                */
                x_memcpy(pui1_output, &pt_dvb->t_sbtl_pref.t_lang_1st,
                         ISO_639_LANG_LEN);
                pui1_output +=  ISO_639_LANG_LEN;

                x_memcpy(pui1_output, &pt_dvb->t_sbtl_pref.t_lang_2nd,
                         ISO_639_LANG_LEN);
                pui1_output +=  ISO_639_LANG_LEN;

                /*
                  Audio downmix mode ui1_downmix_mode
                */
                *pui1_output++ = pt_dvb->ui1_downmix_mode;

                /*
                  SVL ID for reference service, used in NVOD TS
                  services or MF sub-feed services
                */
                ui2_input_val = pt_dvb->ui2_ref_svl_rec_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;
                /* lcn */
                ui2_input_val = pt_dvb->ui2_lcn;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

				/* ts_id*/
                ui2_input_val = pt_dvb->ui2_ts_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

				/* on_id */
                ui2_input_val = pt_dvb->ui2_on_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

                /* if more data are store to file, then needs to
                   update  SZ_DVB_REC_STORED_DATA value.
                */
            }
            break;

            case BRDCST_TYPE_SCTE:
            case BRDCST_TYPE_ATSC:
            {
                pui1_input  = (UINT8*) pv_input;
                pui1_output = (UINT8*) pv_output;
                
                /*
                  currently, ATSC data contains the source id value
                  and aux program id value.
                */
                x_end_cnvt_to_big_16(pui1_input,1,pui1_output);
                pui1_input  += 2 ;
                pui1_output += 2 ;

                x_end_cnvt_to_big_16(pui1_input,1,pui1_output);
                pui1_input  += 2 ;
                pui1_output += 2 ;

                /*
                  Audio downmix mode ui1_downmix_mode
                */
                pui1_input  = (UINT8*) pv_input;
                pui1_input += offsetof(SVL_ATSC_T, ui1_downmix_mode);
                *pui1_output++ = *pui1_input++;
            }
            break;
#ifdef MW_ISDB_SUPPORT
            case BRDCST_TYPE_ISDB:
            {
                SVL_ISDB_T*    pt_isdb = NULL;   

                pt_isdb = (SVL_ISDB_T*)pv_input;
                pui1_output = (UINT8*) pv_output;
                
                /*
                  currently, ISDB data contains the ui1_reg_key value
                  and e_svc_layer value.
                */
                 
                *((UINT8*)pui1_output) = (UINT8) (pt_isdb->ui1_reg_key);
                pui1_output += 1 ;

                *((UINT8*)pui1_output) = (UINT8) (pt_isdb->e_svc_layer);
                pui1_output += 1 ;

                x_end_cnvt_to_big_16(&(pt_isdb->ui2_schedule_num),1,pui1_output);
                pui1_output += 2 ;

                x_end_cnvt_to_big_16(&(pt_isdb->at_schedule),
                                     SVL_ISDB_MAX_NUM_SCHEDULE * 4,
                                     pui1_output);
                pui1_output += 2 * SVL_ISDB_MAX_NUM_SCHEDULE * 4;

                /*
                  Audio downmix mode ui1_downmix_mode
                */
                *pui1_output++ = pt_isdb->ui1_downmix_mode;
            }
            break;
#endif
            case BRDCST_TYPE_DTMB:
            {
                SVL_DTMB_T*   pt_dtmb;
                UINT32        ui4_input_val;
                UINT16        ui2_input_val;

                pt_dtmb   = (SVL_DTMB_T*) pv_input;
                pui1_output = (UINT8*) pv_output;
                /*
                  currently, only following DTMB data are stored to
                  flash, a UINT32 reserve value, DTMB short service name,
                  ca_system_id, t_hd_simulcast_on_ts_svc_id_info.
                  t_orig_svc_info, u_aud, ui1_downmix_mode.
                */
                ui4_input_val = pt_dtmb->ui4_reserve;
                x_end_cnvt_to_big_32(&ui4_input_val,1,pui1_output);
                pui1_output += 4 ;

                /*
                  DTMB short service name.
                */
                x_memcpy(pui1_output, pt_dtmb->s_svc_name,
                         MAX_DTMB_SERVICE_NAME_LEN+1);
                pui1_output +=  (MAX_DTMB_SERVICE_NAME_LEN+1);

                /*
                  CA system ID ui2_ca_system_id
                */
                ui2_input_val = pt_dtmb->ui2_ca_system_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

                /*
                  Simulcast information:
                  
                  t_hd_simulcast_on_ts_svc_id_info:
                  
                  This field is a structure containing ui2_on_id, ui2_ts_id,
                  and ui2_svc_id.
                */
                ui2_input_val = pt_dtmb->t_hd_simulcast_on_ts_svc_id_info.ui2_on_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

                ui2_input_val = pt_dtmb->t_hd_simulcast_on_ts_svc_id_info.ui2_ts_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

                ui2_input_val = pt_dtmb->t_hd_simulcast_on_ts_svc_id_info.ui2_svc_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

                /*
                  t_orig_svc_info field:
                  
                  original service information for this service.
                */
                ui2_input_val = pt_dtmb->t_orig_svc_info.ui2_on_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

                ui2_input_val = pt_dtmb->t_orig_svc_info.ui2_ts_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

                ui2_input_val = pt_dtmb->t_orig_svc_info.ui2_svc_id;
                x_end_cnvt_to_big_16(&ui2_input_val,1,pui1_output);
                pui1_output += 2 ;

                /*
                  Audio downmix mode ui1_downmix_mode
                */
                *pui1_output++ = pt_dtmb->ui1_downmix_mode;

                /* if more data are store to file, then needs to
                   update  SZ_DTMB_REC_STORED_DATA value.
                */
            }
            break;

            default:
            {
                /* no action */
            }
            break;
        }
    }
    return;
}

/*----------------------------------------------------------------------
 * Name: svl_data_conv_from_big_end
 *
 * Description: This API is called by the CDB to perform endian
 *              conversion on the union component of the data record
 *              field, when performing reading operation
 *              from the file system (big-endian) to cpu memory order.
 *
 * Inputs: 
 *   
 *
 * Outputs: None
 *
 * Returns: 
 *   SVLR_OK           SVL library is initialized successfully.
 *   
 -----------------------------------------------------------------------*/
VOID svl_data_conv_from_big_end
(
   DB_FIELD_T	 	 t_fld_type,
   DB_UNION_TAG_T	 t_union_tag,
   const VOID*	     pv_input,
   UINT16		     ui2_size,
   VOID*             pv_output    
)
{
    UINT8*           pui1_input;
    UINT8*           pui1_output;
    
    if ( t_fld_type == ((DB_FIELD_T)SVL_CDB_TYPE_DATA) )
    {
        switch (t_union_tag)
        {
            case BRDCST_TYPE_ANALOG:
            {
                SVL_ANALOG_T*    pt_analog;
                UINT32           ui4_input_val;
                UINT16           ui2_vps_ni_code;
                UINT16           ui2_ttx_fmt_1_ni_code;
                UINT16           ui2_ttx_fmt_2_cni_code;
                UINT16           ui2_src_id;
                UINT8            ui1_valid_src_id;
                UINT8            ui1_color_sys_val;
                UINT8            ui1_aud_mts;
                UINT8            ui1_auto_fine_tune;
                UINT16           ui2_aft_offset;
                
                pui1_input  = (UINT8*) pv_input;
                pt_analog   = (SVL_ANALOG_T*) pv_output;

                /* Decoding the data from the end., we need to do this
                   otherwise we might over-write the next field with
                   the converted value. This error can happen if we
                   are doing in-place conversion, e.g., the pv_input
                   and pv_output both point to the same buffer.

                   The size of data in the SVL_ANALOG_T structure that
                   are stored to persistent storage:
                   
                   4 + 4 + 4 + (2+2+2+1+1) + 1 + 2 + 1 + 1 + 2 + 1 bytes.
                   For ISDB schedule block, there are additional
                   2 + 2 * SVL_ANALOG_MAX_NUM_SCHEDULE * 4 bytes.

                   NOTE !!!! 

                   If the number of field saved to file
                   for this record type change, then the len of
                   the record data must be updated.  Otherwise
                   on loading from file, the field values will
                   not be corrected.
                   
                */
#ifdef MW_ISDB_SUPPORT
#define  SZ_ANALOG_REC_STORED_DATA  (4+4+4+(2+2+2+1+1)+1+2+1+1+2+1+2+8*SVL_ANALOG_MAX_NUM_SCHEDULE)
#else
#define  SZ_ANALOG_REC_STORED_DATA  (4+4+4+(2+2+2+1+1)+1+2+1+1+2+1)
#endif
                
                if ( SZ_ANALOG_REC_STORED_DATA <= ui2_size )
                {
                    pui1_input += SZ_ANALOG_REC_STORED_DATA ;

#ifdef MW_ISDB_SUPPORT
                    /*
                      schedule block settings
                    */
                    pui1_input  -= 2 * SVL_ANALOG_MAX_NUM_SCHEDULE * 4;
                    x_end_cnvt_from_big_16(pui1_input,
                                           SVL_ANALOG_MAX_NUM_SCHEDULE * 4,
                                           &(pt_analog->at_schedule));

                    pui1_input  -= 2 ;
                    x_end_cnvt_from_big_16(pui1_input,1,&(pt_analog->ui2_schedule_num));
#endif
                   
                    /*
                      audio MTS selection
                    */
                    pui1_input  -= 1 ;
                    ui1_aud_mts = *((UINT8*) pui1_input);
                    pt_analog->ui1_aud_mts = ui1_aud_mts;

                    /*
                      auto fine tune offset
                    */
                    pui1_input  -= 2 ;
                    x_end_cnvt_from_big_16(pui1_input, 1, &ui2_aft_offset);
                    pt_analog->i2_aft_offset = (INT16) ui2_aft_offset;

                    /*
                     auto fine tune flag: one byte, so no conversion
                     is needed, just copy to output buffer.
                    */
                    pui1_input  -= 1 ;
                    ui1_auto_fine_tune = *((UINT8*) pui1_input);
                    pt_analog->b_no_aft = ui1_auto_fine_tune;
                    
                    /*
                     source id validity flag: one byte, so no conversion
                     is needed, just copy to output buffer.
                    */
                    pui1_input  -= 1 ;
                    ui1_valid_src_id = *((UINT8*) pui1_input);
                    pt_analog->b_valid_src_id = ui1_valid_src_id;
                    
                    /*
                      source id value.
                    */
                    pui1_input  -= 2 ;
                    x_end_cnvt_from_big_16(pui1_input, 1, &ui2_src_id);
                    pt_analog->ui2_src_id = ui2_src_id;

                    /*
                      video color system: COLOR_SYS_T. This value in memory
                      can be 1 or 4 bytes.  When this field was stored to
                      persistent storage, it is stored as one byte.
                    */
                    pui1_input  -= 1 ;
                    ui1_color_sys_val = *((UINT8*) pui1_input);
                    pt_analog->e_vid_color_sys = (COLOR_SYS_T) ui1_color_sys_val;

                    /*
                      For EU
                    */
                    pui1_input  -= 1 ;
                    pt_analog->ui1_vps_data_byte_5 = *((UINT8*) pui1_input);

                    pui1_input  -= 1 ;
                    pt_analog->t_eu_data_mask = (SVL_ANALOG_EU_DATA_MASK_T)(*((UINT8*) pui1_input));
                        
                    pui1_input  -= 2 ;
                    x_end_cnvt_from_big_16(pui1_input, 1, &ui2_ttx_fmt_2_cni_code);
                    pt_analog->ui2_ttx_fmt_2_cni_code = ui2_ttx_fmt_2_cni_code;

                    pui1_input  -= 2 ;
                    x_end_cnvt_from_big_16(pui1_input, 1, &ui2_ttx_fmt_1_ni_code);
                    pt_analog->ui2_ttx_fmt_1_ni_code = ui2_ttx_fmt_1_ni_code;

                    pui1_input  -= 2 ;
                    x_end_cnvt_from_big_16(pui1_input, 1, &ui2_vps_ni_code);
                    pt_analog->ui2_vps_ni_code = ui2_vps_ni_code;
                    
                    /*
                      Audio system bitmask: UINT32 value.
                    */
                    pui1_input  -= 4 ;
                    x_end_cnvt_from_big_32(pui1_input,1,&ui4_input_val);
                    pt_analog->ui4_audio_sys = ui4_input_val ;

                    /*
                      TV system bitmask: UINT32 value.
                    */
                    pui1_input  -= 4 ;
                    x_end_cnvt_from_big_32(pui1_input,1,&ui4_input_val);
                    pt_analog->ui4_tv_sys = ui4_input_val ;
                
                    /* analog data contains a stream component mask
                       (UINT32), a source id (UINT16), and source id
                       validity flag (BOOL) values.
                    */
                    pui1_input  -= 4 ;
                    x_end_cnvt_from_big_32(pui1_input,1,&ui4_input_val);
                    pt_analog->ui4_s_comp_mask = ui4_input_val ;
                }
            }
            break;

            case BRDCST_TYPE_DVB:
            {
                SVL_DVB_T*    pt_dvb;
                UINT32        ui4_reserve;
                UINT16        ui2_ca_system_id;
                UINT16        ui2_value;

                pui1_input  = (UINT8*) pv_input;
                pt_dvb = (SVL_DVB_T*) pv_output;

                /* Decoding the data from the end., we need to do this
                   otherwise we might over-write the next field with
                   the converted value. This error can happen if we
                   are doing in-place conversion, e.g., the pv_input
                   and pv_output both point to the same buffer.

                   We only stored in flash the following fields in
                   SVL_DVB_T structure. 
                   4 [4 bytes for ui4_reserve] +
                   [MAX_DVB_SERVICE_NAME_LEN+1] +
                   2  [2 bytes for ca_system_id) +  
                   t_hd_simulcast_on_ts_svc_id_info) [6 bytes]
                   t_orig_svc_inf [6 bytes]   [Add Feb 12, 2009]
                   t_lang_1st [ISO_639_LANG_LEN]    [Add Mar 26, 2010]
                   t_lang_2nd [ISO_639_LANG_LEN]    [Add Mar 26, 2010]
                   ui1_downmix_mode                 [Add May 14, 2010]
                   ui2_ref_svl_rec_id               [Add May 18, 2010]

                   NOTE!!!!

                   If the number of field saved to file
                   for this record type change, then the len of
                   the record data must be updated.  Otherwise
                   on loading from file, the field values will
                   not be corrected.
                   
                */
/* #define  SZ_DVB_REC_STORED_DATA  (4 + (MAX_DVB_SERVICE_NAME_LEN+1) + 2 + 6) */
                
#define  SZ_DVB_REC_STORED_DATA  (4 + (MAX_DVB_SERVICE_NAME_LEN+1) + 2 + 6 + 6 + 2*ISO_639_LANG_LEN + 1 + 2 +2 +2 + 2)
                
                if ( SZ_DVB_REC_STORED_DATA <= ui2_size )
                {
                    pui1_input += SZ_DVB_REC_STORED_DATA ;
                    /*on_id*/
                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dvb->ui2_on_id = ui2_value;

                    /*ts_id*/
                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dvb->ui2_ts_id = ui2_value;
                    /*lcn*/
                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dvb->ui2_lcn = ui2_value;

                    /*
                      SVL ID for reference service, used in NVOD TS
                      services or MF sub-feed services
                    */
                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dvb->ui2_ref_svl_rec_id = ui2_value;

                    /*
                      Audio downmix mode ui1_downmix_mode
                    */
                    pt_dvb->ui1_downmix_mode = *--pui1_input;

                    /*
                      t_sbtl_pref field:
                      
                      user subtitle preference for this service.
                    */
                    pui1_input -= ISO_639_LANG_LEN;
                    x_memcpy(&pt_dvb->t_sbtl_pref.t_lang_2nd,
                             pui1_input,
                             ISO_639_LANG_LEN);

                    pui1_input -= ISO_639_LANG_LEN;
                    x_memcpy(&pt_dvb->t_sbtl_pref.t_lang_1st,
                             pui1_input,
                             ISO_639_LANG_LEN);

                    /*
                       original service information

                       t_orig_svc_info

                       This field is a structure containing ui2_on_id, ui2_ts_id,
                       and ui2_svc_id.

                       Reading from the last field to first fields stored in flash.
                    */
                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dvb->t_orig_svc_info.ui2_svc_id = ui2_value;

                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dvb->t_orig_svc_info.ui2_ts_id = ui2_value;

                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dvb->t_orig_svc_info.ui2_on_id = ui2_value;
                       
                    /*
                       Simulcast information:
                  
                       t_hd_simulcast_on_ts_svc_id_info:
                  
                       This field is a structure containing ui2_on_id, ui2_ts_id,
                       and ui2_svc_id.

                       Reading from the last field to first fields stored in flash.
                    */
                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dvb->t_hd_simulcast_on_ts_svc_id_info.ui2_svc_id = ui2_value;

                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dvb->t_hd_simulcast_on_ts_svc_id_info.ui2_ts_id = ui2_value;

                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dvb->t_hd_simulcast_on_ts_svc_id_info.ui2_on_id = ui2_value;

                    /*
                       CA system ID ui2_ca_system_id
                    */
                    pui1_input  -= 2 ;
                    x_memcpy(&ui2_ca_system_id,pui1_input,2);
                    x_end_cnvt_from_big_16(&ui2_ca_system_id,1,&ui2_ca_system_id);
                    pt_dvb->ui2_ca_system_id = ui2_ca_system_id;

                    /*
                      DVB short service name.
                    */
                    pui1_input -= (MAX_DVB_SERVICE_NAME_LEN+1);
                    x_memcpy(pt_dvb->s_svc_name,
                             pui1_input,
                             MAX_DVB_SERVICE_NAME_LEN+1);

                    /*
                      UINT32 reserve value. 
                    */
                    pui1_input -= 4 ;
                    x_memcpy(&ui4_reserve,pui1_input,4);
                    x_end_cnvt_from_big_32(&ui4_reserve,1,&ui4_reserve);
                    pt_dvb->ui4_reserve = ui4_reserve;
                }
            }
            break;

            case BRDCST_TYPE_SCTE:
            case BRDCST_TYPE_ATSC:
            {
                pui1_input  = (UINT8*) pv_input;
                pui1_output = (UINT8*) pv_output;
                /*
                  currently, ATSC data contains the source id value
                  and aux program id value.
                */
                x_end_cnvt_from_big_16(pui1_input,1,pui1_output);
                pui1_input  += 2 ;
                pui1_output += 2 ;

                x_end_cnvt_from_big_16(pui1_input,1,pui1_output);
                pui1_input  += 2 ;
                pui1_output += 2 ;

                /*
                  Audio downmix mode ui1_downmix_mode
                */
                pui1_output = (UINT8*) pv_output;
                pui1_output += offsetof(SVL_ATSC_T, ui1_downmix_mode);
                *pui1_output++ = *pui1_input++;
            }
            break;
#ifdef MW_ISDB_SUPPORT
            case BRDCST_TYPE_ISDB:
            {
                SVL_ISDB_T* pt_isdb = NULL;

                pui1_input  = (UINT8*) pv_input;
                pt_isdb     = (SVL_ISDB_T*) pv_output;
                /*
                  currently, ISDB data contains the ui1_reg_key value
                  and e_svc_layer value.
                */
                pt_isdb->ui1_reg_key = *((UINT8*) pui1_input);
                pui1_input  += 1 ;

                pt_isdb->e_svc_layer = *((UINT8*) pui1_input);
                pui1_input  += 1 ;

                x_end_cnvt_from_big_16(pui1_input,1,&(pt_isdb->ui2_schedule_num));
                pui1_input  += 2 ;

                x_end_cnvt_from_big_16(pui1_input,
                                       SVL_ISDB_MAX_NUM_SCHEDULE * 4,
                                       &(pt_isdb->at_schedule));
                pui1_input  += 2 * SVL_ISDB_MAX_NUM_SCHEDULE * 4;

                /*
                  Audio downmix mode ui1_downmix_mode
                */
                pt_isdb->ui1_downmix_mode = *pui1_input++;
            }
            break;
#endif
            case BRDCST_TYPE_DTMB:
            {
                SVL_DTMB_T*   pt_dtmb;
                UINT32        ui4_reserve;
                UINT16        ui2_ca_system_id;
                UINT16        ui2_value;

                pui1_input  = (UINT8*) pv_input;
                pt_dtmb = (SVL_DTMB_T*) pv_output;

                /* Decoding the data from the end., we need to do this
                   otherwise we might over-write the next field with
                   the converted value. This error can happen if we
                   are doing in-place conversion, e.g., the pv_input
                   and pv_output both point to the same buffer.

                   We only stored in flash the following fields in
                   SVL_DTMB_T structure. 
                   4 [4 bytes for ui4_reserve] +
                   [MAX_DTMB_SERVICE_NAME_LEN+1] +
                   2  [2 bytes for ca_system_id) +  
                   t_hd_simulcast_on_ts_svc_id_info) [6 bytes]
                   t_orig_svc_inf [6 bytes]   [Add Feb 12, 2009]

                   NOTE!!!!

                   If the number of field saved to file
                   for this record type change, then the len of
                   the record data must be updated.  Otherwise
                   on loading from file, the field values will
                   not be corrected.
                   
                */
/* #define  SZ_DTMB_REC_STORED_DATA  (4 + (MAX_DTMB_SERVICE_NAME_LEN+1) + 2 + 6) */
                
#define  SZ_DTMB_REC_STORED_DATA  (4 + (MAX_DTMB_SERVICE_NAME_LEN+1) + 2 + 6 + 6 + 1)
                
                if ( SZ_DTMB_REC_STORED_DATA <= ui2_size )
                {
                    pui1_input += SZ_DTMB_REC_STORED_DATA ;

                    /*
                      Audio downmix mode ui1_downmix_mode
                    */
                    pt_dtmb->ui1_downmix_mode = *--pui1_input;

                    /*
                       original service information

                       t_orig_svc_info

                       This field is a structure containing ui2_on_id, ui2_ts_id,
                       and ui2_svc_id.

                       Reading from the last field to first fields stored in flash.
                    */
                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dtmb->t_orig_svc_info.ui2_svc_id = ui2_value;

                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dtmb->t_orig_svc_info.ui2_ts_id = ui2_value;

                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dtmb->t_orig_svc_info.ui2_on_id = ui2_value;
                       
                    /*
                       Simulcast information:
                  
                       t_hd_simulcast_on_ts_svc_id_info:
                  
                       This field is a structure containing ui2_on_id, ui2_ts_id,
                       and ui2_svc_id.

                       Reading from the last field to first fields stored in flash.
                    */
                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dtmb->t_hd_simulcast_on_ts_svc_id_info.ui2_svc_id = ui2_value;

                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dtmb->t_hd_simulcast_on_ts_svc_id_info.ui2_ts_id = ui2_value;

                    pui1_input -= 2 ;
                    x_memcpy(&ui2_value, pui1_input, 2);
                    x_end_cnvt_from_big_16(&ui2_value,1,&ui2_value);
                    pt_dtmb->t_hd_simulcast_on_ts_svc_id_info.ui2_on_id = ui2_value;

                    /*
                       CA system ID ui2_ca_system_id
                    */
                    pui1_input  -= 2 ;
                    x_memcpy(&ui2_ca_system_id,pui1_input,2);
                    x_end_cnvt_from_big_16(&ui2_ca_system_id,1,&ui2_ca_system_id);
                    pt_dtmb->ui2_ca_system_id = ui2_ca_system_id;

                    /*
                      DTMB short service name.
                    */
                    pui1_input -= (MAX_DTMB_SERVICE_NAME_LEN+1);
                    x_memcpy(pt_dtmb->s_svc_name,
                             pui1_input,
                             MAX_DTMB_SERVICE_NAME_LEN+1);

                    /*
                      UINT32 reserve value. 
                    */
                    pui1_input -= 4 ;
                    x_memcpy(&ui4_reserve,pui1_input,4);
                    x_end_cnvt_from_big_32(&ui4_reserve,1,&ui4_reserve);
                    pt_dtmb->ui4_reserve = ui4_reserve;
                }
            }
            break;

            default:
            {
                /* no action */
            }
            break;
        }
    }
    return;
}



/*----------------------------------------------------------------------
 * Name: svl_is_updated_flds_persistent_data
 *
 * Description: This API compares 2 SVL record, and return a
 *  status flag indicating if the differences between 2 SVL record
 *  belong to the fields that is part of persistent data (e.g., will
 *  saved to the file system.).
 *
 *  Currently: this API only evaluate SVL analog and SVL DVB records.
 *
 *  ******************************************************
 *  IMPORTANT: if the persistent data saved to file in
 *             'svl_data_conv_to_big_end(..)' or the
 *             custom function 'svl_write_persistent_data' changes,
 *             then this function MUST be changed to match
 *             the fields that are stored. 
 *  *******************************************************
 *
 * Inputs: 
 *   pt_curr_svl:    pointer to current SVL record structure to be
 *                   updated.
 *
 *   pt_update_svl:  pointer to the new SVL record strucuture.
 *
 * Outputs: None
 *
 * Returns: 
 *   1      Persistent data between 2 SVL record are different.
 *   0      Persistent data between 2 SVL record are the same.
 -----------------------------------------------------------------------*/
INT32 svl_is_updated_flds_persistent_data
(
    const SVL_REC_T*       pt_curr_svl,
    const SVL_REC_T*       pt_update_svl
)
{
    INT32  i4_rc = 1 ;   /* 1 means data to be stored in persistent
                            storage has changed between 2 SVL record */
    
    if ( (pt_curr_svl->uheader.t_rec_hdr.e_brdcst_type == BRDCST_TYPE_ANALOG) &&
         (pt_curr_svl->uheader.t_rec_hdr.ui4_option_mask & SB_RECORD_NOT_SAVE_CH_NUM)
       )
    {
        /* For the Analog SVL record and its option_mask that has
           been set to indicate don't save ui4_channel_id
           to persistent storage.

           Set the return code to 0, but continue checking for other
           fields that are different.
           
        */
        i4_rc = 0;
    }
    else
    {
        if ( pt_curr_svl->uheader.t_rec_hdr.ui4_channel_id !=
             pt_update_svl->uheader.t_rec_hdr.ui4_channel_id )
        {
            return 1;
        }
    }
    

    if ( pt_curr_svl->uheader.t_rec_hdr.ui4_nw_mask !=
         pt_update_svl->uheader.t_rec_hdr.ui4_nw_mask )
    {
        return 1;
    }

    if ( pt_curr_svl->uheader.t_rec_hdr.ui4_option_mask !=
         pt_update_svl->uheader.t_rec_hdr.ui4_option_mask )
    {
        return 1;
    }

    if ( pt_curr_svl->uheader.t_rec_hdr.ui2_prog_id !=
         pt_update_svl->uheader.t_rec_hdr.ui2_prog_id )
    {
        return 1;
    }

    if ( pt_curr_svl->uheader.t_rec_hdr.ui2_tsl_id !=
         pt_update_svl->uheader.t_rec_hdr.ui2_tsl_id )
    {
        return 1;
    }

    if ( pt_curr_svl->uheader.t_rec_hdr.ui2_tsl_rec_id !=
         pt_update_svl->uheader.t_rec_hdr.ui2_tsl_rec_id )
    {
        return 1;
    }

    if ( pt_curr_svl->uheader.t_rec_hdr.ui2_nwl_id !=
         pt_update_svl->uheader.t_rec_hdr.ui2_nwl_id )
    {
        return 1;
    }

    if ( pt_curr_svl->uheader.t_rec_hdr.ui2_nwl_rec_id !=
         pt_update_svl->uheader.t_rec_hdr.ui2_nwl_rec_id )
    {
        return 1;
    }

    if ( pt_curr_svl->uheader.t_rec_hdr.ui2_satl_id !=
         pt_update_svl->uheader.t_rec_hdr.ui2_satl_id )
    {
        return 1;
    }

    if ( pt_curr_svl->uheader.t_rec_hdr.ui2_satl_rec_id !=
         pt_update_svl->uheader.t_rec_hdr.ui2_satl_rec_id )
    {
        return 1;
    }

    if ( pt_curr_svl->uheader.t_rec_hdr.e_brdcst_type !=
         pt_update_svl->uheader.t_rec_hdr.e_brdcst_type )
    {
        return 1;
    }

    if ( pt_curr_svl->uheader.t_rec_hdr.e_serv_type !=
         pt_update_svl->uheader.t_rec_hdr.e_serv_type )
    {
        return 1;
    }

    if ( x_strncmp(pt_curr_svl->uheader.t_rec_hdr.ac_name,
                  pt_update_svl->uheader.t_rec_hdr.ac_name,
                  MAX_PROG_NAME_LEN+1) != 0 )
    {
        return 1;
    }

    if ( x_memcmp(pt_curr_svl->uheader.t_rec_hdr.aui1_custom_data,
                  pt_update_svl->uheader.t_rec_hdr.aui1_custom_data,
                  MW_SVL_HEADER_REC_CUSTOM_BYTES) != 0 )
    {
        return 1;
    }

#ifdef ANDROID
    if ( x_memcmp(pt_curr_svl->uheader.t_rec_hdr.ai1_private_data,
                  pt_update_svl->uheader.t_rec_hdr.ai1_private_data,
                  20) != 0 )
    {
        return 1;
    }
#endif
    /* Now check the data section of SVL record. */
    switch (pt_curr_svl->uheader.t_rec_hdr.e_brdcst_type)
    {
        case BRDCST_TYPE_DVB:
        {
            const SVL_DVB_T*   pt_curr_dvb;
            const SVL_DVB_T*   pt_update_dvb;

            pt_curr_dvb = &(pt_curr_svl->u_data.t_dvb);
            pt_update_dvb = &(pt_update_svl->u_data.t_dvb);

            if ( pt_curr_dvb->ui4_reserve != pt_update_dvb->ui4_reserve )
            {
                return 1;
            }

            if ( x_strncmp(pt_curr_dvb->s_svc_name,
                           pt_update_dvb->s_svc_name,
                           MAX_DVB_SERVICE_NAME_LEN+1)
                 != 0 )
            {
                return 1;
            }

            if ( pt_curr_dvb->ui2_ca_system_id != pt_update_dvb->ui2_ca_system_id )
            {
                return 1;
            }

            /*
              t_hd_simulcast_on_ts_svc_id_info is stored, therefore we
              need to check this field.
            */
            if ( pt_curr_dvb->t_hd_simulcast_on_ts_svc_id_info.ui2_on_id !=
                 pt_update_dvb->t_hd_simulcast_on_ts_svc_id_info.ui2_on_id
                )
            {
                return 1;
            }

            if ( pt_curr_dvb->t_hd_simulcast_on_ts_svc_id_info.ui2_ts_id !=
                 pt_update_dvb->t_hd_simulcast_on_ts_svc_id_info.ui2_ts_id
                )
            {
                return 1;
            }

            if ( pt_curr_dvb->t_hd_simulcast_on_ts_svc_id_info.ui2_svc_id !=
                 pt_update_dvb->t_hd_simulcast_on_ts_svc_id_info.ui2_svc_id
                )
            {
                return 1;
            }

            /*
              t_orig_svc_info is stored, therefore we
              need to check this field.
            */
            if ( pt_curr_dvb->t_orig_svc_info.ui2_on_id !=
                 pt_update_dvb->t_orig_svc_info.ui2_on_id
                )
            {
                return 1;
            }

            if ( pt_curr_dvb->t_orig_svc_info.ui2_ts_id !=
                 pt_update_dvb->t_orig_svc_info.ui2_ts_id
                )
            {
                return 1;
            }

            if ( pt_curr_dvb->t_orig_svc_info.ui2_svc_id !=
                 pt_update_dvb->t_orig_svc_info.ui2_svc_id
                )
            {
                return 1;
            }

        
            if ( x_memcmp(&pt_curr_dvb->t_sbtl_pref, &pt_update_dvb->t_sbtl_pref,
                          sizeof(SVL_DVB_SBTL_PREF_T)) != 0
                )
            {
                return 1;
            }
            
            if ( pt_curr_dvb->ui1_downmix_mode !=
                 pt_update_dvb->ui1_downmix_mode
                )
            {
                return 1;
            }

            if ( pt_curr_dvb->ui2_ref_svl_rec_id !=
                 pt_update_dvb->ui2_ref_svl_rec_id
                )
            {
                return 1;
            }
            if ( pt_curr_dvb->ui2_lcn !=
                 pt_update_dvb->ui2_lcn
                )
            {
                return 1;
            }


            if ( pt_curr_dvb->ui2_ts_id !=
                 pt_update_dvb->ui2_ts_id
                )
            {
                return 1;
            }

            if ( pt_curr_dvb->ui2_on_id !=
                 pt_update_dvb->ui2_on_id
                )
            {
                return 1;
            }
            /* Other fields in the DVB record are not part of persistent data.*/
            i4_rc = 0;
        }
        break;

        case BRDCST_TYPE_ANALOG:
        {
            const SVL_ANALOG_T*   pt_curr_ana;
            const SVL_ANALOG_T*   pt_update_ana;

            pt_curr_ana = &(pt_curr_svl->u_data.t_analog);
            pt_update_ana = &(pt_update_svl->u_data.t_analog);


            if ( pt_curr_ana->ui4_s_comp_mask != pt_update_ana->ui4_s_comp_mask )
            {
                return 1;
            }
            
            if ( pt_curr_ana->ui4_tv_sys != pt_update_ana->ui4_tv_sys )
            {
                return 1;
            }

            if ( pt_curr_ana->ui4_audio_sys != pt_update_ana->ui4_audio_sys )
            {
                return 1;
            }

            if ( pt_curr_ana->ui2_vps_ni_code != pt_update_ana->ui2_vps_ni_code )
            {
                return 1;
            }

            if ( pt_curr_ana->ui2_ttx_fmt_1_ni_code != pt_update_ana->ui2_ttx_fmt_1_ni_code )
            {
                return 1;
            }

            if ( pt_curr_ana->ui2_ttx_fmt_2_cni_code != pt_update_ana->ui2_ttx_fmt_2_cni_code )
            {
                return 1;
            }

            if ( pt_curr_ana->t_eu_data_mask != pt_update_ana->t_eu_data_mask )
            {
                return 1 ;
            }

            if ( pt_curr_ana->ui1_vps_data_byte_5 != pt_update_ana->ui1_vps_data_byte_5 )
            {
                return 1 ;
            }

            if ( pt_curr_ana->e_vid_color_sys != pt_update_ana->e_vid_color_sys )
            {
                return 1;
            }

            if ( pt_curr_ana->ui2_src_id != pt_update_ana->ui2_src_id )
            {
                return 1;
            }
            
            if ( pt_curr_ana->b_valid_src_id != pt_update_ana->b_valid_src_id )
            {
                return 1;
            }
            
            if ( pt_curr_ana->b_no_aft != pt_update_ana->b_no_aft )
            {
                return 1;
            }
            
            if ( pt_curr_ana->i2_aft_offset != pt_update_ana->i2_aft_offset )
            {
                return 1;
            }

#ifdef MW_ISDB_SUPPORT
            if ( pt_curr_ana->ui2_schedule_num != pt_update_ana->ui2_schedule_num )
            {
                return 1;
            }

            if ( x_memcmp(pt_curr_ana->at_schedule, pt_update_ana->at_schedule,
                          sizeof(pt_curr_ana->at_schedule)) != 0)
            {
                return 1;
            }
#endif

            
            if ( pt_curr_ana->ui1_aud_mts != pt_update_ana->ui1_aud_mts )
            {
                return 1;
            }
        }
        break;

        case BRDCST_TYPE_DTMB:
        {
            const SVL_DTMB_T*   pt_curr_dtmb;
            const SVL_DTMB_T*   pt_update_dtmb;

            pt_curr_dtmb = &(pt_curr_svl->u_data.t_dtmb);
            pt_update_dtmb = &(pt_update_svl->u_data.t_dtmb);
                
            /* ui4_reserve */
            if ( pt_curr_dtmb->ui4_reserve != pt_update_dtmb->ui4_reserve )
            {
                return 1;
            }

            /*DTMB short service name. */
            if ( x_strncmp(pt_curr_dtmb->s_svc_name,
                           pt_update_dtmb->s_svc_name,
                           MAX_DTMB_SERVICE_NAME_LEN+1)
                 != 0 )
            {
                return 1;
            }
                


            /*CA system ID ui2_ca_system_id*/
            if ( pt_curr_dtmb->ui2_ca_system_id != pt_update_dtmb->ui2_ca_system_id )
            {
                return 1;
            }


            /*Simulcast information:                  
                        t_hd_simulcast_on_ts_svc_id_info:                 
                        This field is a structure containing ui2_on_id, ui2_ts_id,and ui2_svc_id.
                    */
            if ( pt_curr_dtmb->t_hd_simulcast_on_ts_svc_id_info.ui2_on_id != 
                pt_update_dtmb->t_hd_simulcast_on_ts_svc_id_info.ui2_on_id )
            {
                return 1;
            }

            if ( pt_curr_dtmb->t_hd_simulcast_on_ts_svc_id_info.ui2_ts_id !=
                pt_update_dtmb->t_hd_simulcast_on_ts_svc_id_info.ui2_ts_id )
            {
                return 1;
            }

            if ( pt_curr_dtmb->t_hd_simulcast_on_ts_svc_id_info.ui2_svc_id !=
                pt_update_dtmb->t_hd_simulcast_on_ts_svc_id_info.ui2_svc_id )
            {
                return 1;
            }

            /*t_orig_svc_info field:                  
                        original service information for this service.
                     */
            if ( pt_curr_dtmb->t_orig_svc_info.ui2_on_id != pt_update_dtmb->t_orig_svc_info.ui2_on_id )
            {
                return 1;
            }

            if ( pt_curr_dtmb->t_orig_svc_info.ui2_ts_id != pt_update_dtmb->t_orig_svc_info.ui2_ts_id )
            {
                return 1;
            }

            if ( pt_curr_dtmb->t_orig_svc_info.ui2_svc_id != pt_update_dtmb->t_orig_svc_info.ui2_svc_id )
            {
                return 1;
            }

            /*Audio downmix mode ui1_downmix_mode */
            if ( pt_curr_dtmb->ui1_downmix_mode != pt_update_dtmb->ui1_downmix_mode )
            {
                return 1;
            }

            /* if more data are store to file, then needs to update  SZ_DTMB_REC_STORED_DATA value. */

            /* Other fields in the DTMB record are not part of persistent data.*/
            i4_rc = 0;
        }
        break;
        
        /* For other type of SVL data, return 1, to indicate
           fields to be stored are part of persistent data.
        */
        default:
        {
            i4_rc = 1;
        }
        break;
    }
    
    return i4_rc ;
}

/*----------------------------------------------------------------------
 * Name: svl_write_persistent_data
 *
 * Description: This API modifies the field in CDB record
 *    which are being write to the file system.  This function
 *    is registered with CDB handle and is called when
 *    CDB is performing write operation to filesystem.
 *
 *    This API is implemented to provide a work around for
 *    a patent issue:  under certain condition, we can
 *    not save the channel ID value to persistent storage
 *    and then read it back.  Therefore, under these
 *    condition, we write 0 to persistent storage for the
 *    channel ID value.  
 *
 * Inputs:
 *
 *    pv_rec_obj:      An opaque object that contains current
 *                     CDB record information.  In client persistent
 *                     write function, it should only pass this
 *                     object to db_get_field_from_rec() function
 *                     to get the current value of this field.
 *
 *    t_field_name:    the CDB field name of the data in 'pui1_buf'.
 *                     E.g., this is the name of the current field being
 *                     written out to persistent storage.
 *
 *    pui1_buf:        on input, it contains the field data to be
 *                     written to persistent storage.  On output, buffer
 *                     contains modified data which will be written to
 *                     persistent storage.
 *
 *    ui4_buf_len:     The length of the output buffer.  
 *
 * Outputs: 
 *
 *    pui1_buf:        contain modified value.
 *
 * Returns: 
 *   0      No modification to the output field.
 *   1:     Output field modified.
 *       
 -----------------------------------------------------------------------*/
INT32 svl_write_persistent_data
(
    VOID*                  pv_rec_obj,
    DB_FIELD_T             t_field_name,
    UINT8*                 pui1_buf,
    UINT32                 ui4_buf_len   
)
{
    INT32                  i4_rc = 0;

    /* for channel ID only.

       Note: Any change to this file for that modify writing of SVL record
       to persistent store, the function svl_is_updated_flds_persistent_data()
       needs to updated to reflect the logic.

       currently:

       (1) We don't save 'channel ID' value to persistent storage for
       the SVL Analog record with the ui4_option_mask value
       sets to SB_RECORD_NOT_SAVE_CH_NUM.

    */
    if ( t_field_name == ((DB_FIELD_T)SVL_CDB_TYPE_CHANNEL_ID) )
    {
        BRDCST_TYPE_T      e_brdcst_type;

        if ( db_get_field_from_rec(pv_rec_obj,
                                   (DB_FIELD_T)SVL_CDB_TYPE_BRDCST_T ,
                                   (UINT8*)&e_brdcst_type,
                                   (UINT32) sizeof(e_brdcst_type) )
             == DBR_OK )
        {
            if ( e_brdcst_type == BRDCST_TYPE_ANALOG )
            {
                UINT32     ui4_opt_mask = 0;
                
                /* get the custom flag setting. */
                if ( db_get_field_from_rec(pv_rec_obj,
                                           (DB_FIELD_T)SVL_CDB_TYPE_OPT_MASK_ID,
                                           (UINT8*)&ui4_opt_mask,
                                           (UINT32)sizeof(ui4_opt_mask))
                     == DBR_OK )
                {
                    
                    if ( ui4_opt_mask & SB_RECORD_NOT_SAVE_CH_NUM )
                    {
                        if ( ui4_buf_len >= 4 )
                        {
                            /* Don't save the channel ID valut to file,
                               instead we write out 0 for the channel ID */
                            x_memset(pui1_buf,(UINT8)0,4);
                            i4_rc = 1 ;
                        }
                        else
                        {
                            /* something is wrong, the buffer must be
                               at least the sizeof the ui4_channel_id
                            */
                            ABORT(DBG_CAT_MEMORY,DBG_ABRT_BUF_TOO_SMALL);
                        }
                    }
                }
            }
        }
    }

    return i4_rc;
}

