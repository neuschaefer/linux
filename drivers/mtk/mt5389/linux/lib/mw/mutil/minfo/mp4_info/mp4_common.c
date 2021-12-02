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
 * $RCSfile: mp4_common.c,v $
 * $Revision: #1 $ 1.0
 * $Date: 2012/04/27 $ 2004/11/26
 * $Author: dtvbm11 $
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the mp4 common function
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

#include "inc/u_common.h"
#include "handle/u_handle.h"

#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif

#include "inc/x_mm_common.h"

#include "mp4_common.h"

#else

#include "x_os.h"
#include "u_common.h"
#include "u_handle.h"

#include "../../mm_util.h"

#include "x_mm_common.h"

#include "mp4_common.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/
 
MP4_BOX_ID_T _r_mp4_box_id_map[MP4_BOX_ID_MAX]=
{
    { "uuid" ,  MP4_BOX_ID_UUID },
    { "ftyp" ,  MP4_BOX_ID_FTYP },
    { "moov" ,  MP4_BOX_ID_MOOV },
    { "mvhd" ,  MP4_BOX_ID_MVHD },
    { "trak" ,  MP4_BOX_ID_TRAK },
    { "tkhd" ,  MP4_BOX_ID_TKHD },
    { "mdia" ,  MP4_BOX_ID_MDIA },
    { "mdhd" ,  MP4_BOX_ID_MDHD },
    { "hdlr" ,  MP4_BOX_ID_HDLR },
    { "minf" ,  MP4_BOX_ID_MINF },
    { "stbl" ,  MP4_BOX_ID_STBL },
    { "stsd" ,  MP4_BOX_ID_STSD },
    { "esds" ,  MP4_BOX_ID_ESDS },
    { "stts" ,  MP4_BOX_ID_STTS },
    { "stsc" ,  MP4_BOX_ID_STSC },
    { "stsz" ,  MP4_BOX_ID_STSZ },
    { "stco" ,  MP4_BOX_ID_STCO },
    { "stss" ,  MP4_BOX_ID_STSS },
    { "mdat" ,  MP4_BOX_ID_MDAT },
    { "iods" ,  MP4_BOX_ID_IODS },
    { "tref" ,  MP4_BOX_ID_TREF },
    { "nmhd" ,  MP4_BOX_ID_NMHD },
    { "dinf" ,  MP4_BOX_ID_DINF },
    { "dref" ,  MP4_BOX_ID_DREF },
    { "vmhd" ,  MP4_BOX_ID_VMHD },
    { "smhd" ,  MP4_BOX_ID_SMHD },
    { "hmhd" ,  MP4_BOX_ID_HMHD },
    { "udta" ,  MP4_BOX_ID_UDTA },
    { "ctts" ,  MP4_BOX_ID_CTTS },
    { "odsm" ,  MP4_BOX_ID_ODSM },
    { "edts" ,  MP4_BOX_ID_EDTS },
    { "elst" ,  MP4_BOX_ID_ELST },
    { "co64" ,  MP4_BOX_ID_CO64 },
    { "gmhd" ,  MP4_BOX_ID_GMHD },
    { "load" ,  MP4_BOX_ID_LOAD },
    { "imap" ,  MP4_BOX_ID_IMAP },
    { "code" ,  MP4_BOX_ID_CODE },
    { "cmov" ,  MP4_BOX_ID_CMOV },
    { "mvex" ,  MP4_BOX_ID_MVEX }
};

/*-----------------------------------------------------------------------------
                    functions declarations
 ----------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------------
 * Name:  _mp4_box_is_need_check_sub
 *
 * Description: 
 *
 * Inputs:  -e_id
 *          
 * Outputs: -none
 *
 * Returns: TRUE      The box need check sub box
 *          FALSE     The box don't need check sub box
 ----------------------------------------------------------------------------*/ 
BOOL _mp4_box_is_need_check_sub(MP4_BOX_ID_ENUM_T e_id)
{
    return (e_id== MP4_BOX_ID_MOOV 
            || e_id == MP4_BOX_ID_TRAK
            || e_id == MP4_BOX_ID_MDIA
            || e_id == MP4_BOX_ID_MINF
            || e_id == MP4_BOX_ID_STBL);
}

/*-----------------------------------------------------------------------------
 * Name:  _mp4_offset_is_in_this_box
 *
 * Description: 
 *
 * Inputs:  -ui8_offset
 *          -pt_box
 *          
 * Outputs: -none
 *
 * Returns: TRUE      The offset is in the box
 *          FALSE     The offset is not in the box
 ----------------------------------------------------------------------------*/ 
BOOL _mp4_offset_is_in_this_box(UINT64           ui8_offset, 
                                       MP4_BOX_T* pt_box)
{
    return ((ui8_offset >= pt_box->ui8_offset) 
            && (ui8_offset <= (pt_box->ui8_offset + pt_box->ui4_size)));
}


/*-----------------------------------------------------------------------------
 * Name:  _mp4_swap_4bytes
 *
 * Description: 
 *
 * Inputs:  -ui4_in
 *          
 * Outputs: -none
 *
 * Returns: 4 bytes after swap
 ----------------------------------------------------------------------------*/         
UINT32 _mp4_swap_4bytes(UINT32 ui4_in)
{
    UINT32 ui4_ret;

    ui4_ret = (ui4_in >> 24);
    ui4_ret |= ((ui4_in >> 16) & 0x0ff) << 8;
    ui4_ret |= ((ui4_in >>  8) & 0x0ff) << 16;
    ui4_ret |= (ui4_in << 24);
    return ui4_ret;
}


/*-----------------------------------------------------------------------------
 * Name:  _mp4_get_par_box_by_offset
 *
 * Description: 
 *
 * Inputs:  -pt_head_box
 *          -ui4_Offset
 *          
 * Outputs: -ppt_parent_box
 *
 * Returns: MINFOR_OK                     The API is successful. 
 ----------------------------------------------------------------------------*/ 
INT32 _mp4_get_par_box_by_offset(MP4_BOX_T*    pt_head_box, 
                                 UINT64        ui8_Offset, 
                                        MP4_BOX_T**   ppt_parent_box)
{
    MP4_BOX_T* pt_cur_box;

    pt_cur_box = pt_head_box;
    *ppt_parent_box = NULL;

    while (pt_cur_box != NULL)
    {
        if (_mp4_offset_is_in_this_box(ui8_Offset, pt_cur_box))
        {
            if (pt_cur_box->pt_first_sub_box!= NULL)
            {
                pt_cur_box = pt_cur_box->pt_first_sub_box;
            }
            else
            {
                *ppt_parent_box = pt_cur_box;
                break;
            }
        }
        else if (pt_cur_box->pt_next_box!= NULL)
        {
            pt_cur_box = pt_cur_box->pt_next_box;
        }
        else
        {
            *ppt_parent_box = pt_cur_box ->pt_parent_box;
            break;
        }
    }
    return MP4_OK;
}


/*-----------------------------------------------------------------------------
 * Name:  _mp4_get_last_sub_box
 *
 * Description: 
 *
 * Inputs:  -pt_par_box
 *          
 * Outputs: -ppt_last_sub_box
 *
 * Returns: MINFOR_OK                     The API is successful. 
 ----------------------------------------------------------------------------*/ 
INT32 _mp4_get_last_sub_box(MP4_BOX_T*  pt_par_box, 
                                   MP4_BOX_T** ppt_last_sub_box)
{
    MP4_BOX_T* pt_cur_box;
    
    pt_cur_box = pt_par_box->pt_first_sub_box;
    *ppt_last_sub_box = NULL;
    while (pt_cur_box != NULL)
    {
        *ppt_last_sub_box = pt_cur_box;
        pt_cur_box=pt_cur_box->pt_next_box;
    }
    return MP4_OK;
}    


/*-----------------------------------------------------------------------------
 * Name:  _mp4_get_box_by_id
 *
 * Description: 
 *
 * Inputs:  -pt_head_box
 *          -e_id
 *          
 * Outputs: -ppt_e_id_box
 *
 * Returns: MINFOR_OK                     The API is successful. 
 ----------------------------------------------------------------------------*/ 
INT32 _mp4_get_box_by_id(MP4_BOX_T*            pt_head_box, 
                                MP4_BOX_ID_ENUM_T     e_id,
                                MP4_BOX_T**           ppt_e_id_box)
{
    MP4_BOX_T* pt_cur_box;
    MP4_BOX_T* pt_par_box;

    pt_cur_box = pt_head_box;
    *ppt_e_id_box = NULL;
    while (pt_cur_box != NULL)
    {
        if (pt_cur_box->e_box_id == e_id)
        {
            *ppt_e_id_box = pt_cur_box;
            break;
        }
        else
        {
            if (pt_cur_box->pt_first_sub_box != NULL)
            {
                pt_cur_box = pt_cur_box->pt_first_sub_box;
            }
            else if (pt_cur_box->pt_next_box != NULL)
            {
                pt_cur_box = pt_cur_box->pt_next_box;
            }
            else
            {
                pt_par_box = pt_cur_box->pt_parent_box;
                if (pt_par_box == NULL)
                {
                    break;
                }
                pt_cur_box = pt_par_box->pt_next_box;
                while (pt_cur_box == NULL)
                {
                    pt_cur_box = pt_par_box;
                    pt_par_box = pt_cur_box->pt_parent_box;
                    if (pt_par_box == NULL)
                    {
                        break;
                    }
                    pt_cur_box = pt_par_box->pt_next_box;
                }
                if (pt_cur_box == pt_head_box)
                {
                    break;
                }
            }
        }
    }
    return MP4_OK;
}

