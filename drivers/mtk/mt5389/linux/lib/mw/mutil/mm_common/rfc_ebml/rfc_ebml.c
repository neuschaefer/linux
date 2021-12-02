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
 * rfc_ebml.c,v
 * 1.1.2.1 1.0
 * $Date: 2012/06/20 $ 2004/11/26
 * willyhuang
 * $CCRevision: /main/DTV_X_HQ_int/4 $
 * $SWAuthor: Asa Wang $
 * $MD5HEX: 9ce950eb85bf90ea84aa0781ae2e804f $ Asa
 *
 * Description: 
 *         This file contains the implementation of the ebml 1.0 basic parse
           Author: lianming mcn07123 
           History:
           (1)2008-10-27  lianming mcn07123  initial

 *---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"  {
#endif


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
#include "mutil/mm_util.h"
#include "mutil/mm_util_input.h"
#include "mutil/mm_util_memory.h"
#include "mutil/mm_common/mm_common_dbg.h"


#ifndef __NO_FM__
#include "file_mngr/x_fm.h"
#endif
#include "unicode/x_uc_str.h"

#include "rfc_ebml.h"

#else

#include "x_os.h"
#include "u_common.h"
#include "u_handle.h"
#include "../../mm_util.h"
#include "../../mm_util_input.h"
#include "../../mm_util_memory.h"
#include "x_uc_str.h"

#include "rfc_ebml.h"
#include "../../mm_common/mm_common_dbg.h"

#endif
/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 ----------------------------------------------------------------------------*/
#define MAX_VINT_NUMBER         8

/*-----------------------------------------------------------------------------
                    extern data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data declarations
 ----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
                    data definition
 ----------------------------------------------------------------------------*/
static const INT64 g_vsint_subtr[MAX_VINT_NUMBER] = 
{
    0x3F, 0x1FFF, 0x0FFFFF, 0x07FFFFFFll, 0x03FFFFFFFFll, 0x01FFFFFFFFFFll,
    0x00FFFFFFFFFFFFFFll, 0x007FFFFFFFFFFFFFll
};

/*-----------------------------------------------------------------------------
 * Name:  _ebml_lib_open
 *
 * Description: Open a ebml lib
 *
 * Inputs:   pi1_name:        App name 
 *           pt_input_stream: Input stream 
 *          
 * Outputs:  Ebml lib object handle 
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
 INT32 _ebml_lib_open(CHAR*                pi1_name, 
                      EBML_INPUT_STREAM_T* pt_input_stream, 
                      HANDLE_T*            ph_ebml)
{
    if (NULL != pt_input_stream)
    {
        /*generate handle for app if input stream is valid*/
    }
    return EBMLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:  _ebml_get_app_name
 *
 * Description: get app name
 *
 * Inputs:   h_ebml: Ebml lib object handle 
 *          
 * Outputs:  App name 
 * 
 * Returns:  NULL   
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/

VOID _ebml_get_app_name(HANDLE_T h_ebml, CHAR* pi1_name)
{
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _ebml_lib_close
 *
 * Description: Close a ebml lib
 *
 * Inputs:   h_ebml: Ebml lib object handle 
 *          
 * Outputs:  NULL
 * 
 * Returns:  NULL
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/

VOID _ebml_lib_close(HANDLE_T h_ebml)
{
    return;
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_vint
 * Description: get a vint value from a stream
 *
 * Inputs:   h_ebml: Ebml lib object handle 
 *           ui8_offset: stream offset
 *          
 * Outputs:  pt_this: Variable int
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/

INT32 _ebml_vint(HANDLE_T h_ebml, UINT64 ui8_offset, EBML_VINT_T* pt_this)
{
    return EBMLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_buf_vint
 * Description: get a vint value from buffer
 *
 * Inputs:   pv_buf: Buffur address 
 *           ui4_len: Buffer length
 *          
 * Outputs:  pt_this: Variable int
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_buf_vint(VOID* pv_buf, UINT32 ui4_len, EBML_VINT_T* pt_this)
{
    UINT8   ui1_mask;
    BOOL    b_id_found;
    UINT8*  pui1_buf;
    UINT8   ui1_id_len;
    
    if ((NULL == pv_buf) || (NULL == pt_this))
    {
        return EBMLR_NULL_PTR;
    }
    
    /*at least mask=10000000*/
    ui1_mask = (UINT8)1 << 7;
    b_id_found = FALSE;
    pui1_buf = (UINT8*)pv_buf;
    ui1_id_len = 0;
    x_memset(pt_this, 0, sizeof(EBML_VINT_T));
    
    while (!b_id_found)
    {
        /*read byte one by one*/
        if (ui1_id_len > 7)/*not more than 8 bytes*/
        {
            return EBMLR_INVALID_VINT;
        }
        if (ui1_id_len >= ((UINT8)ui4_len))
        {
        }
        pt_this->aui1_data[ui1_id_len] = *pui1_buf;
        if (pt_this->aui1_data[0] & (ui1_mask >> ui1_id_len))
        {
            b_id_found = TRUE;
            
        }
        ++ui1_id_len;
        ++pui1_buf;
    };
    
    /*find ID*/
    if (TRUE == b_id_found)
    {
        pt_this->ui1_id_len = ui1_id_len;
        return EBMLR_OK;

    }
    else
    {
        pt_this->ui1_id_len = 0;
        return EBMLR_INVALID_VINT;
    }
    
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_vint_is_valid
 * Description: Judge a Id is valid
 *
 * Inputs:   pt_vint: Vint 
 *          
 * Outputs:  NULL
 * 
 * Returns:  TRUE   : Valid Vint
 *           FALSE  : Invalid Vint
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
BOOL _ebml_vint_is_valid(EBML_VINT_T* pt_vint)
{
    UINT8  ui1_mask;
    UINT8  ui1_idx;
    BOOL   b_found;
    
    if (NULL == pt_vint)
    {
        DBG_ERROR(("ebml_vint_is_valid:NULL PTR\r\n"));
        return FALSE;
    }

    ui1_mask = (UINT8)1 << 7;
    b_found = FALSE;
    for (ui1_idx = 0; ui1_idx < 8; ++ui1_idx)
    {
        if (ui1_mask & pt_vint->aui1_data[0])
        {
            ++ui1_idx;
            b_found = TRUE;
            break;
        }
        ui1_mask >>= 1;
    }
    /*If find length*/
    if (TRUE == b_found)
    {
        /*check every byte ?= 0xFF*/
        if (ui1_idx == pt_vint->ui1_id_len)
        {
            /*check first byte*/
            ui1_mask = 0xFF;
            ui1_mask >>=  ui1_idx;
            if (ui1_mask != pt_vint->aui1_data[0])
            {
                return TRUE;
            }
            /*check remainder bytes*/
            for (; ui1_idx > 1; --ui1_idx)
            {
                if (0xFF != pt_vint->aui1_data[ui1_idx - 1])
                {
                    return TRUE;
                }
            }
            return FALSE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
    
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_vint_compare
 * Description: compares two vints if it is equal
 *
 * Inputs:   pt_vint1: vint1 
 *           pt_vint2: vint2
 *          
 * Outputs:  NULL
 * 
 * Returns:  0   :if equal
 *           -1  :pt_vint1 less than pt_vint2
 *           1   :pt_vint1 more than pt_vint2
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_vint_compare(EBML_VINT_T* pt_vint1, EBML_VINT_T* pt_vint2)
{
    UINT32 ui4_idx;
        
    if ((NULL == pt_vint1) || (NULL == pt_vint2))
    {
        DBG_ERROR(("ebml_vint_compare:NULL PTR!!!\r\n"));
        return EBMLR_VINT_COMP_UNKNOWN;
    }

    /*If these vints are valid?*/
    if (!(_ebml_vint_is_valid(pt_vint1) && _ebml_vint_is_valid(pt_vint2)))
    {
        DBG_ERROR(("ebml_vint_compare:invalid ID!!!\r\n"));
        return EBMLR_VINT_COMP_UNKNOWN;
    }
    
    if (pt_vint1->ui1_id_len > pt_vint2->ui1_id_len)
    {
        return EBMLR_VINT_COMP_MORE;
    }
    else if (pt_vint1->ui1_id_len < pt_vint2->ui1_id_len)
    {
        return EBMLR_VINT_COMP_LESS;
    }
    else
    {
        for (ui4_idx = 0; ui4_idx < pt_vint1->ui1_id_len; ++ui4_idx)
        {
            if (ui4_idx >= MAX_VINT_NUMBER)
            {
                DBG_ERROR(("ebml_vint_compare:error length!!!\r\n"));
                return EBMLR_VINT_COMP_UNKNOWN;
            }
            if (pt_vint1->aui1_data[ui4_idx] < pt_vint2->aui1_data[ui4_idx])
            {
                return EBMLR_VINT_COMP_LESS;
            }
            else if(pt_vint1->aui1_data[ui4_idx] > pt_vint2->aui1_data[ui4_idx])
            {
                return EBMLR_VINT_COMP_MORE;
            }
            else
            {
                
            }
        }
        return EBMLR_VINT_COMP_EQUAL;
    }
    
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_vint_get_value
 * Description: Return value of a vint
 *
 * Inputs:   pt_vint: Vint 
 *          
 * Outputs:  NULL
 * 
 * Returns:  Value of a ID
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
UINT64 _ebml_vint_get_value(EBML_VINT_T* pt_vint)
{
    UINT64 ui8_value;
    UINT8  ui1_idx;
    UINT8  ui1_mask;
    
    if (NULL == pt_vint)
    {
        DBG_ERROR(("ebml_vint_get_value:NULL PTR!!!\r\n"));
        return (UINT64)EBML_INVALID_VINT;
    }
    if (pt_vint->ui1_id_len > 8)/*no more than 8 bytes*/
    {
        DBG_ERROR(("ebml_vint_get_value:wrong length!!!\r\n"));
        return (UINT64)EBML_INVALID_VINT;
    }
    /*get value*/
    ui1_mask = (UINT8)0x7F >> (pt_vint->ui1_id_len - 1);
    ui8_value = pt_vint->aui1_data[0] & ui1_mask;
    for (ui1_idx = 1; ui1_idx < pt_vint->ui1_id_len; ++ ui1_idx)
    {
        ui8_value <<= 8;
        ui8_value |= pt_vint->aui1_data[ui1_idx];
    }

    return ui8_value;
}
/*-----------------------------------------------------------------------------
 * Name:    _ebml_svint_get_value
 * Description: Return value of a vint
 *
 * Inputs:   pt_vint: Vint 
 *          
 * Outputs:  NULL
 * 
 * Returns:  Value of a ID
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT64 _ebml_svint_get_value(EBML_SVINT_T* pt_svint)
{
    UINT64 ui8_value;
    UINT8  ui1_idx;
    UINT8  ui1_mask;
    
    if (NULL == pt_svint)
    {
        DBG_ERROR(("_ebml_svint_get_value:NULL PTR!!!\r\n"));
        return EBML_INVALID_SVINT;
    }
    if ((pt_svint->ui1_id_len > MAX_VINT_NUMBER)/*no more than 8 bytes*/
        ||(pt_svint->ui1_id_len == 0))
    {
        DBG_ERROR(("_ebml_svint_get_value:wrong length!!!\r\n"));
        return EBML_INVALID_SVINT;
    }
    /*get value*/
    ui1_mask = (UINT8)0x7F >> (pt_svint->ui1_id_len - 1);
    ui8_value = pt_svint->aui1_data[0] & ui1_mask;
    for (ui1_idx = 1; ui1_idx < pt_svint->ui1_id_len; ++ ui1_idx)
    {
        ui8_value <<= 8;
        ui8_value |= pt_svint->aui1_data[ui1_idx];
    }
    if (pt_svint->ui1_id_len - 1 >= MAX_VINT_NUMBER)
    {
        DBG_ERROR(("_ebml_svint_get_value:wrong subtr length!!!\r\n"));
        return EBML_INVALID_SVINT;
    }
    return (INT64)(ui8_value - g_vsint_subtr[pt_svint->ui1_id_len - 1]);
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_id
 * Description: get a vint id from a stream
 *
 * Inputs:   h_ebml: Ebml lib object handle 
 *           ui8_offset: stream offset
 *          
 * Outputs:  pt_this: Variable int ID
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_id(HANDLE_T h_ebml, UINT64 ui8_offset, EBML_ID_T* pt_this)
{
    return _ebml_vint(h_ebml, ui8_offset, pt_this);
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_buf_id
 * Description: get a vint value from buffer
 *
 * Inputs:   pv_buf: Buffur address 
 *           ui4_len: Buffer length
 *          
 * Outputs:  pt_this: Variable int ID
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_buf_id(VOID* pv_buf, UINT32 ui4_len, EBML_ID_T* pt_this)
{
     
    /*call base operation*/
    return _ebml_buf_vint(pv_buf, ui4_len, (EBML_VINT_T*)pt_this);
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_id_get_value
 * Description: Return value of a ID
 *
 * Inputs:   pt_vint: EBML_ID_T 
 *          
 * Outputs:  NULL
 * 
 * Returns:  Value of a EBML_ID_T
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
UINT32 _ebml_id_get_value(EBML_ID_T* pt_vint)
{
    UINT32 ui4_idx;
    UINT32 ui4_id = 0;
    UINT8 ui1_len = pt_vint->ui1_id_len;
    if (ui1_len > 8)/*no more than 8 bytes*/
    {
        DBG_ERROR(("_ebml_id_get_value:wrong length!!!\r\n"));
        return (UINT32)0xFFFFFFFF;
    }
    /* return (UINT32)_ebml_vint_get_value((EBML_VINT_T*)pt_vint);*/
    for (ui4_idx = 0; ui4_idx < ui1_len; ++ui4_idx)
    {
        ui4_id  |= pt_vint->aui1_data[ui4_idx] << ((ui1_len -1 - ui4_idx)*8);
    }
    return ui4_id;
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_id_compare
 * Description: compares two Ids if it is equal
 *
 * Inputs:   pt_vint: ID
 *           ui4_id_value: ID value
 *          
 * Outputs:  NULL
 * 
 * Returns:  0   :if equal
 *           -1  :pt_vint1 less than pt_vint2
 *           1   :pt_vint1 more than pt_vint2
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_id_compare(EBML_VINT_T* pt_vint, UINT32 ui4_id_value)
{
    UINT32 ui4_id;
    
    /* Parameters Check*/
    if (NULL == pt_vint)
    {
        DBG_ERROR(("ebml_id_compare:NULL PTR\r\n"));
        return EBMLR_VINT_COMP_UNKNOWN;
    }
    
    ui4_id = _ebml_id_get_value(pt_vint);
    if (ui4_id == ui4_id_value)
    {
        return EBMLR_VINT_COMP_EQUAL;
    }
    else if (ui4_id < ui4_id_value)
    {
        return EBMLR_VINT_COMP_LESS;
    }
    else
    {
        return EBMLR_VINT_COMP_MORE;
    }
    
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_size
 * Description: get a vint size from a stream
 *
 * Inputs:   h_ebml: Ebml lib object handle 
 *           ui8_offset: stream offset
 *          
 * Outputs:  pt_this: Variable int Size
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_size(HANDLE_T h_ebml,  UINT64 ui8_offset, EBML_SIZE_T* pt_this)
{
    return _ebml_vint(h_ebml, ui8_offset, pt_this);
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_buf_size
 * Description: get a vint size from buffer
 *
 * Inputs:   pv_buf: Buffur address 
 *           ui4_len: Buffer length
 *          
 * Outputs:  pt_this: Variable int Size
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_buf_size(VOID* pv_buf, UINT32 ui4_len, EBML_SIZE_T* pt_this)
{
    /*call base operation*/
    return _ebml_buf_vint(pv_buf, ui4_len, (EBML_VINT_T*)pt_this);
}

/*-----------------------------------------------------------------------------
 * Name:     _ebml_size_get_value
 * Description: Return value of a size
 *
 * Inputs:   pt_vint: EBML_SIZE_T 
 *          
 * Outputs:  NULL
 * 
 * Returns:  Value of a EBML_SIZE_T
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
UINT64 _ebml_size_get_value(EBML_SIZE_T* pt_vint)
{
    return (UINT64)_ebml_vint_get_value((EBML_VINT_T*)pt_vint);
}

/*-----------------------------------------------------------------------------
 * Name:     _ebml_ele_base
 * Description: Parse the ID and Size of a element
 *
 * Inputs:   h_ebml: Ebml lib object handle 
 *           ui8_offset: stream offset
 *          
 * Outputs:  pt_this
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_ele_base(HANDLE_T          h_ebml, 
                     UINT64            ui8_offset, 
                     EBML_ELE_BASIC_T* pt_this)
{
    /*Parameters Check*/
    
    return EBMLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_buf_ele_base
 * Description: Parse ID and Size of a element  from buffer
 *
 * Inputs:   pv_buf: Buffur address 
 *           ui4_len: Buffer length
 *            ui8_offset: Element offset
 *          
 * Outputs:  pt_this: Element
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_buf_ele_base(VOID*             pv_buf, 
                         UINT32            ui4_len, 
                         UINT64            ui8_offset,
                         EBML_ELE_BASIC_T* pt_this)
{
    UINT8* pui1_buf;
    INT32  i4_ret;
    /*Parameters Check*/
    if ((NULL == pv_buf) || (NULL == pt_this))
    {
        return EBMLR_NULL_PTR;
    }
    
    x_memset(pt_this, 0, sizeof(EBML_ELE_BASIC_T));
    pt_this->ui8_ele_offset = ui8_offset;
    pui1_buf = (UINT8*)pv_buf;
    /*1.get Id*/
    i4_ret =  _ebml_buf_id((VOID*)pui1_buf, ui4_len, &(pt_this->t_id));
    if (EBMLR_OK != i4_ret)
    {
        return EBMLR_ERR;
    }
    pui1_buf += pt_this->t_id.ui1_id_len;
    
    /*2.get Size*/
    i4_ret =  _ebml_buf_size((VOID*)pui1_buf, ui4_len, &(pt_this->t_size));
    if (EBMLR_OK != i4_ret)
    {
        return EBMLR_ERR;
    }
    pui1_buf += pt_this->t_size.ui1_id_len;    
    return EBMLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_buf_ele_get_data_offset
 * Description: Return data offset in stream or buffer
 *
 * Inputs:   pt_vint: EBML_ID_T 
 *          
 * Outputs:  NULL
 * 
 * Returns:  Data Offset in a stream of buffer
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
UINT64 _ebml_buf_ele_get_data_offset(EBML_ELE_BASIC_T* pt_this)
{
    /*Parameters Check*/
    if (NULL == pt_this)
    {
        return (UINT64)EBML_INVALID_OFFSET;
    }

    if ((UINT64)EBML_INVALID_OFFSET == pt_this->ui8_ele_offset)
    {
        return (UINT64)EBML_INVALID_OFFSET;
    }

    return (pt_this->ui8_ele_offset 
            + pt_this->t_id.ui1_id_len
            + pt_this->t_size.ui1_id_len);
    
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_buf_ele_get_ele_size
 * Description: Return element size
 *
 * Inputs:   pt_vint: EBML_ID_T 
 *          
 * Outputs:  NULL
 * 
 * Returns:  Data Offset in a stream of buffer
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
UINT32 _ebml_buf_ele_get_ele_size(EBML_ELE_BASIC_T* pt_this)
{
    /*Parameters Check*/
    if (NULL == pt_this)
    {
        return (UINT32)EBML_INVALID_OFFSET;
    }

    return ((UINT32)_ebml_size_get_value(&(pt_this->t_size))
            + pt_this->t_id.ui1_id_len
            + pt_this->t_size.ui1_id_len);
    
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_buf_ele_get_head_size
 * Description: Return element head size
 *
 * Inputs:   pt_vint: EBML_ID_T 
 *          
 * Outputs:  NULL
 * 
 * Returns:  Data Offset in a stream of buffer
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
UINT32 _ebml_buf_ele_get_head_size(EBML_ELE_BASIC_T* pt_this)
{
    /*Parameters Check*/
    if (NULL == pt_this)
    {
        return (UINT32)EBML_INVALID_OFFSET;
    }

    return (pt_this->t_id.ui1_id_len
            + pt_this->t_size.ui1_id_len);
    
}

/*-----------------------------------------------------------------------------
 * Name:     _ebml_uint
 * Description: Get a Uint value from stream
 *
 * Inputs:   h_ebml: Ebml lib object handle 
 *           ui8_offset: stream offset
 *           ui4_size:  data size
 * Outputs:  pui8_value
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_uint(HANDLE_T    h_ebml,
                 UINT64      ui8_offset, 
                 UINT32      ui4_size, 
                 EBML_UINT*  pui8_value)
{
    return EBMLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_buf_uint
 * Description: Get a Uint value from buffer
 *
 * Inputs:   pv_buf: Buffur address 
 *           ui4_len: Buffer length
 *           ui4_size:  data size
 * Outputs:  pui8_value
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_buf_uint(VOID*      pv_buf, 
                     UINT32     ui4_len, 
                     UINT32     ui4_size,
                     EBML_UINT* pui8_value)
{
    UINT32 ui4_idx;
    UINT64 ui8_value;
    UINT8* pui1_buf;
    
    /*Parameter Check*/
    if (NULL == pv_buf)
    {
        DBG_ERROR(("_ebml_buf_uint:NULL PTR!!!\r\n"));
        return EBMLR_NULL_PTR;
    }
    
    /*size check*/
    if (ui4_size > ui4_len)
    {
        DBG_ERROR(("_ebml_buf_uint:Size(%d)>Length(%d)!!!\r\n", 
                   ui4_size, ui4_len));
        return EBMLR_ERR;
    }

    ui8_value = 0;
    pui1_buf = (UINT8*)pv_buf;
    for (ui4_idx = 0; ui4_idx < ui4_size; ++ui4_idx)
    {
        ui8_value <<= 8;
        ui8_value |= pui1_buf[ui4_idx];
    }
    
    *pui8_value = ui8_value;
    return EBMLR_OK;
}

INT32 _ebml_buf_luint(VOID*      pv_buf, 
                     UINT32     ui4_len, 
                     UINT32     ui4_size,
                     EBML_UINT* pui8_value1,
                     EBML_UINT* pui8_value2)
{
    UINT32 ui4_idx;
    UINT64 ui8_value;
    UINT8* pui1_buf;
    
    /*Parameter Check*/
    if (NULL == pv_buf)
    {
        DBG_ERROR(("_ebml_buf_uint:NULL PTR!!!\r\n"));
        return EBMLR_NULL_PTR;
    }
    
    /*size check*/
    if (ui4_size > ui4_len)
    {
        DBG_ERROR(("_ebml_buf_uint:Size(%d)>Length(%d)!!!\r\n", 
                   ui4_size, ui4_len));
        return EBMLR_ERR;
    }

    ui8_value = 0;
    pui1_buf = (UINT8*)pv_buf;
    
    *pui8_value1 = 0;
    *pui8_value2 = 0;
    for (ui4_idx = 0; ui4_idx < ui4_size; ++ui4_idx)
    {
        ui8_value <<= 8;
        ui8_value |= pui1_buf[ui4_idx];
        if(ui4_idx == 7 && ui4_size > 8)
        {
            *pui8_value1 = ui8_value;   
        }
    }
    
    *pui8_value2 = ui8_value;
    return EBMLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:     _ebml_int
 * Description: Get a int value from stream
 *
 * Inputs:   h_ebml: Ebml lib object handle 
 *           ui8_offset: stream offset
 *           ui4_size:  data size
 * Outputs:  pi8_value
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_int(HANDLE_T    h_ebml,
                UINT64      ui8_offset, 
                UINT32      ui4_size, 
                EBML_INT*   pi8_value)
{
    return EBMLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_buf_int
 * Description: Get a int value from buffer
 *
 * Inputs:   pv_buf: Buffur address 
 *           ui4_len: Buffer length
 *           ui4_size:  data size
 * Outputs:  pi8_value
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_buf_int(VOID*      pv_buf, 
                    UINT32     ui4_len, 
                    UINT32     ui4_size,
                    EBML_INT*  pi8_value)
{
    INT64  i8_value;
    UINT32 ui4_idx;

    
    /*Parameter Check*/
    if (NULL == pv_buf)
    {
        DBG_ERROR(("_ebml_buf_int:NULL PTR!!!\r\n"));
        return EBMLR_NULL_PTR;
    }
    
    /*size check*/
    if (ui4_size > ui4_len)
    {
        DBG_ERROR(("_ebml_buf_int:Size(%d)>Length(%d)!!!\r\n", 
                   ui4_size, ui4_len));
        return EBMLR_ERR;
    }

    if (ui4_size > 8)
    {
        DBG_ERROR(("_ebml_buf_int:Size(%d) Error!!!\r\n", 
                   ui4_size));
        return EBMLR_ERR;
    }
     /*If is a negative value*/
    if (((UINT8*)pv_buf)[0] & 0x80)
    {
        i8_value = -1;
    }
    else
    {
        i8_value = 0;
    }
    for (ui4_idx = 0; ui4_idx < ui4_size; ++ui4_idx)
    {
        i8_value <<= 8;
        i8_value |= ((UINT8*)pv_buf)[ui4_idx];
    }
    
    *pi8_value = i8_value;
    return EBMLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:     _ebml_float
 * Description: Get a float value from stream
 *
 * Inputs:   h_ebml: Ebml lib object handle 
 *           ui8_offset: stream offset
 *           ui4_size:  data size
 * Outputs:  pf8_value
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_float(HANDLE_T    h_ebml, 
                  UINT64      ui8_offset,
                  UINT32      ui4_size,
                  EBML_FLOAT* pf8_value)
{
    return EBMLR_OK;
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_buf_float
 * Description: Get a float value from buffer
 *
 * Inputs:   pv_buf: Buffur address 
 *           ui4_len: Buffer length
 *           ui4_size:  data size
 * Outputs:  pf8_value
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_buf_float(VOID*       pv_buf,  
                      UINT32      ui4_len,
                      UINT32      ui4_size,
                      EBML_FLOAT* pf8_value)
{
#ifdef _EBML_KERNEL_NO_VFP_
#else
    FLOAT  f4_value;
    DOUBLE f8_value;
#endif

    /*Parameter Check*/
    if ((NULL == pv_buf) || (NULL == pf8_value))
    {
        DBG_ERROR(("_ebml_buf_float:NULL PTR!!!\r\n"));
        return EBMLR_NULL_PTR;
    }
    
    /*size check*/
    if (ui4_size > ui4_len)
    {
        DBG_ERROR(("_ebml_buf_float:Size(%d)>Length(%d)!!!\r\n", 
                   ui4_size, ui4_len));
        return EBMLR_ERR;
    }

    if (ui4_size > 8)
    {
        DBG_ERROR(("_ebml_buf_float:Size(%d) Error!!!\r\n", 
                   ui4_size));
        return EBMLR_ERR;
    }
    
#ifdef _EBML_KERNEL_NO_VFP_
    if ((4 == ui4_size) || (8 == ui4_size))
    {
        x_memcpy((VOID*)pf8_value, pv_buf, ui4_size);
    }
    else
    {
        DBG_ERROR(("_ebml_buf_float:do not support %d size float!!!\r\n", 
                   ui4_size));
        return EBMLR_ERR;
    }
#else  
    f4_value = 0;
    f8_value = 0;
    
    if (4 == ui4_size)
    {
        x_memcpy((VOID*)&f4_value, pv_buf, ui4_size);
        *pf8_value = (EBML_FLOAT)f4_value;
    }
    else if (8 == ui4_size)
    {
        x_memcpy((VOID*)&f8_value, pv_buf, ui4_size);
        *pf8_value = f8_value;
    }
    else if(10 == ui4_size)
    {
        /*if 80 float do not support*/
        DBG_ERROR(("_ebml_buf_float:do not support 80 float!!!\r\n"));
        *pf8_value = (EBML_FLOAT)0;
        return EBMLR_ERR;
    }
    else if(0 == ui4_size)
    {
        *pf8_value = (EBML_FLOAT)0;
    }
    else
    {
        DBG_ERROR(("_ebml_buf_float:do not support %d size float!!!\r\n", 
                   ui4_size));
        return EBMLR_ERR;
    }
#endif
    return EBMLR_OK;
}


UINT32 _ebml_float32_significand(UINT32 ui4_value)
{
    UINT32 ui4_count;
    UINT32 ui4_mask = 1 ;

    for (ui4_count = 0; ui4_count < 23 ; ++ui4_count)
    {
        if ((ui4_mask << ui4_count) & ui4_value)
        {
            break;
        }
    }
    return (23 - ui4_count);
    
}

UINT32 _ebml_double64_significand(UINT64 ui8_value)
{
    UINT32 ui4_count;
    UINT64 ui8_mask = 1;

    for (ui4_count = 0; ui4_count < 52 ; ++ui4_count)
    {
        if ((ui8_mask << ui4_count) & ui8_value)
        {
            break;
        }
    }
    return (52 - ui4_count);
    
}

INT32 _ebml_float32_to_fra(EBML_FLOAT32      f4_value, 
                           FLOAT_FRACTION_T* pt_fraction)
{
    UINT8    ui1_sign;
    UINT8    ui1_exponent;
    INT8     i1_e;
    UINT32   ui4_mantissa;
    UINT8*   pui1_buf;
    UINT32   ui4_significand;
    UINT32   ui4_temp;
    UINT64   ui8_temp;
    UINT8    ui1_idx;
    UINT32   ui4_decimal = 0;
    
    if (NULL == pt_fraction)
    {
        DBG_ERROR(("_ebml_float32_to_fra:NULL PTR!!!\r\n"));
        return EBMLR_NULL_PTR;
    }
    /*
    IEEE754 float format:
                 SEEEEEEE EMMMMMMM MMMMMMMM MMMMMMMM
    Type                   Sign      Exponent        Mantissa 
    Zeroes                  +/-      0               0 
    Denormalized numbers    +/-      0               non zero 
    Normalized numbers      +/-      1 to 2^e-2      any 
    Infinities              +/-      2^e-1           0 
    NaNs                    +/-      2^e-1           non zero 
    */

    pui1_buf = (UINT8*)&f4_value;
    /*get sign*/
    if (pui1_buf[0] & 0x80)
    {
        ui1_sign = 1;   /*negative*/
    }
    else
    {
        ui1_sign = 0;   /*positive*/
    }
    /*get exponent*/
    ui1_exponent = ((pui1_buf[0] & 0x7f) << 1) | ((pui1_buf[1] & 0x80) >> 7);
     /*subtract bias 127 to get true value*/
    if (ui1_exponent >= 127)
    {
        i1_e = (INT8)(ui1_exponent - 127);
    }
    else
    {
        i1_e = (INT8)((-1)*(127 - ui1_exponent));
    }
    
    ui4_mantissa = 0;
    ui4_mantissa |= (((UINT32)(pui1_buf[1] & 0x7f)) << 16);
    ui4_mantissa |= (((UINT32)pui1_buf[2]) << 8);
    ui4_mantissa |= (UINT32)pui1_buf[3];

    /*init */
    x_memset(pt_fraction->aui8_nume_scale, 1, 
             sizeof(pt_fraction->aui8_nume_scale));
    x_memset(pt_fraction->aui8_deno_scale, 1, 
             sizeof(pt_fraction->aui8_deno_scale));
    pt_fraction->ui1_sign = ui1_sign;
    pt_fraction->ui8_int_part = 0;
    pt_fraction->ui8_scale_part = 1;
    
    /*get mantissa significand*/
    ui4_significand = _ebml_float32_significand(ui4_mantissa);

    /*Process ......*/
    if ((0 != ui1_exponent) && (0xFF != ui1_exponent))
    {
        /*Normalized numbers :value = (+/-) 1.m * 2^e*/
        pt_fraction->ui1_type = EBML_NORMAL_NUMBER_M;
        
        
        /*leading bit default value is 1*/
        ui4_mantissa |= (1 << 23);
        pt_fraction->ui8_numerator = 
                     (UINT64)(ui4_mantissa >> (23 - ui4_significand));
        
        if (i1_e < 0)
        {   
            ui4_temp = ((UINT32)((-1) * i1_e) + ui4_significand);
            if (ui4_temp < 64)
            {
                /*pt_fraction->ui8_denominator = (UINT64)(1 << ui4_temp);*/
                if (ui4_temp < 32)
                {
                    pt_fraction->ui8_denominator = (UINT64)1 << 31;
                }
                else
                {
                    pt_fraction->ui8_denominator = (UINT64)1 << 31;
                    pt_fraction->ui8_denominator *= (UINT64)1 << (ui4_temp - 31);
                }
                
                
                ui4_decimal = (UINT32)((-1) * i1_e);
            }
            else/*very small near to zero*/
            {
                pt_fraction->ui1_type = EBML_NORMAL_NUMBER_DENO_SCALE_M;
                pt_fraction->ui8_denominator = (UINT64)1 << 63;
                if (ui4_temp <= 126) /*63 + 63 */
                {
                    pt_fraction->aui8_deno_scale[0] = (UINT64)1 << (ui4_temp - 63);
                }
                else if (ui4_temp <= 189) /*63 + 63 + 63*/
                {
                    pt_fraction->aui8_deno_scale[0] = (UINT64)1 << 63;
                    pt_fraction->aui8_deno_scale[1] = (UINT64)1 << (ui4_temp - 189);

                }
                else
                {
                    /*impossible path*/
                }
            }

        }
        else if ((UINT32)i1_e >= ui4_significand)
        {
            ui4_temp = (UINT32)i1_e - ui4_significand;
            if (ui4_temp <= (64 -1 - ui4_significand))  /*M:default bit is 1*/
            {
                pt_fraction->ui8_numerator <<= ui4_temp;
                pt_fraction->ui8_denominator = 1;
                ui4_decimal = 0;
            }
            else 
            {
                pt_fraction->ui1_type = EBML_NORMAL_NUMBER_NUME_SCALE_M;
                pt_fraction->ui8_numerator <<= (64 - 1 - ui4_significand);
                ui4_temp = ui4_temp - (64 - 1 - ui4_significand);
                if (ui4_temp <= 63)
                {
                    pt_fraction->aui8_nume_scale[0] = (UINT64)1 << ui4_temp;
                }
                else
                {
                    pt_fraction->aui8_nume_scale[0] = (UINT64)1 << 63;
                    pt_fraction->aui8_nume_scale[1] = (UINT64)1 << (ui4_temp - 63);
                }
            }
        }
        else
        {   
            pt_fraction->ui8_denominator = 
                      (UINT64)(1 << (ui4_significand - (UINT32)i1_e));
            ui4_decimal = (UINT32)(ui4_significand - (UINT32)i1_e);
        }


        if (EBML_NORMAL_NUMBER_M == pt_fraction->ui1_type)
        {
            ui8_temp = 1;
            for (ui1_idx = 0; ui1_idx < (UINT8)ui4_decimal; ++ui1_idx)
            {
                ui8_temp *= 10;
            }

            /*maybe overlow... ...*/
            pt_fraction->ui8_int_part = 
                   _mm_div64((pt_fraction->ui8_numerator) * ui8_temp,
                              pt_fraction->ui8_denominator,
                      NULL);
            pt_fraction->ui8_scale_part = ui8_temp;
        }
        else
        {
            /*here will truncate some too small numerical value*/
        }
        
    }
    else if (0 == ui1_exponent)
    {
        pt_fraction->ui1_type = EBML_NORMAL_NUMBER_M;
        /*Zero */
        if (0 == ui4_mantissa)
        {
            pt_fraction->ui8_numerator = 0;
            pt_fraction->ui8_denominator = 1;
        }
        else /*very very very samll */
        {
            /*Denormalized :value: (+/-) 0.m * 2^(-126)*/
            pt_fraction->ui1_type = EBML_SMALL_NUMBER_M;
            pt_fraction->ui8_numerator =
                         (UINT64)(ui4_mantissa >> (23 - ui4_significand));
            pt_fraction->ui8_denominator = (UINT64)1 << 63;
            pt_fraction->aui8_deno_scale[0] = (UINT64)1 << 63;
            pt_fraction->aui8_deno_scale[1] = (UINT64)1 << ui4_significand;
        }
        
    }
    else  /*(0xFF == ui1_exponent)*/
    {

        if (0 == ui4_mantissa) /*infinity*/
        {
            pt_fraction->ui1_type = (UINT8)(ui1_sign? EBML_NEG_INFINITY_M: EBML_POS_INFINITY_M);
            pt_fraction->ui8_numerator = (UINT64)(-1);
            pt_fraction->aui8_nume_scale[0] = (UINT64)-1;
            pt_fraction->aui8_nume_scale[1] = (UINT64)-1;
            pt_fraction->ui8_denominator = 1;
        }
        else /*Not a number*/
        {
            pt_fraction->ui1_type = EBML_NOT_A_NUMBER_M;
            pt_fraction->ui8_numerator = 0;
            pt_fraction->ui8_denominator = 1;
        }
    }
    
    /*do not take account of EBML_NOT_A_NUMBER_M*/
    if ((EBML_NORMAL_NUMBER_NUME_SCALE_M == pt_fraction->ui1_type)
        || (EBML_NEG_INFINITY_M == pt_fraction->ui1_type)
        || (EBML_POS_INFINITY_M == pt_fraction->ui1_type))
    {
        pt_fraction->ui8_int_part = (UINT64)(-1);
    }

    return EBMLR_OK;
}

INT32 _ebml_double64_to_fra(EBML_DOUBLE64     f8_value, 
                            FLOAT_FRACTION_T* pt_fraction)
{
    UINT8    ui1_sign;
    UINT16   ui2_exponent;
    INT16    i2_e;
    UINT64   ui8_mantissa;
    UINT8*   pui1_buf;
    UINT32   ui4_significand;
    UINT32   ui4_temp;
    UINT64   ui8_temp;
    UINT8    ui1_idx;
    UINT32   ui4_decimal = 0;
    UINT16   ui2_e_nm;
    
    if (NULL == pt_fraction)
    {
        DBG_ERROR(("_ebml_double64_to_fra:NULL PTR!!!\r\n"));
        return EBMLR_NULL_PTR;
    }
    /*
    IEEE754 double format:
      SEEEEEEE EEEEMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM
    Type                   Sign      Exponent        Mantissa 
    Zeroes                  +/-      0               0 
    Denormalized numbers    +/-      0               non zero 
    Normalized numbers      +/-      1 to 2^e-2      any 
    Infinities              +/-      2^e-1           0 
    NaNs                    +/-      2^e-1           non zero 
    */

    pui1_buf = (UINT8*)&f8_value;
    /*get sign*/
    if (pui1_buf[0] & 0x80)
    {
        ui1_sign = 1;    /*negative*/
    }
    else
    {
        ui1_sign = 0;    /*positive*/
    }
    /*get exponent*/
    ui2_exponent = ((pui1_buf[0] & 0x7f) << 4) | ((pui1_buf[1] & 0xF0) >> 4);
    ui2_e_nm = ((pui1_buf[0] & 0x7f) << 4) | (pui1_buf[1] >> 4);
     /*subtract bias 1023 to get true value*/
    if (ui2_exponent >= 1023)
    {
        i2_e = (INT16)(ui2_exponent - 1023);
    }
    else
    {
        i2_e = (INT16)((-1)*(1023 - ui2_exponent));
    }
    
    ui8_mantissa = 0;
    ui8_mantissa |= (((UINT64)(pui1_buf[1] & 0x0f)) << 48);
    ui8_mantissa |= (((UINT64)pui1_buf[2]) << 40);
    ui8_mantissa |= (((UINT64)pui1_buf[3]) << 32);
    ui8_mantissa |= (((UINT64)pui1_buf[4]) << 24);
    ui8_mantissa |= (((UINT64)pui1_buf[5]) << 16);
    ui8_mantissa |= (((UINT64)pui1_buf[6]) << 8);
    ui8_mantissa |= (UINT64)pui1_buf[7];

    /*init */
    x_memset(pt_fraction->aui8_nume_scale, 1, 
             sizeof(pt_fraction->aui8_nume_scale));
    x_memset(pt_fraction->aui8_deno_scale, 1, 
             sizeof(pt_fraction->aui8_deno_scale));
    pt_fraction->ui1_sign = ui1_sign;
    pt_fraction->ui8_int_part = 0;
    pt_fraction->ui8_scale_part = 1;
    
    /*get mantissa significand
    ui4_significand = _ebml_double64_significand(ui8_mantissa)*/;
    ui4_significand = (ui2_e_nm < 1023) ? (UINT32)(1023 - ui2_e_nm) : (UINT32)(ui2_e_nm - 1023);

    /*Process ......*/
    if ((0 != ui2_exponent) && (0x07FF != ui2_exponent))
    {
        /*Normalized numbers :value = (+/-) 1.m * 2^e*/
        pt_fraction->ui1_type = EBML_NORMAL_NUMBER_M;
        
        
        /*leading bit default value is 1*/
        ui8_mantissa |= (UINT64)1 << 52;
        pt_fraction->ui8_numerator = 
                     (ui8_mantissa >> (52 - ui4_significand));
        
        if (i2_e < 0)
        {   
            ui4_temp = ((UINT32)((-1) * i2_e) + ui4_significand);
            if (ui4_temp < 64)
            {
                pt_fraction->ui8_denominator = (UINT64)1 << ui4_temp;
                ui4_decimal = (UINT32)((-1) * i2_e);
            }
            else/*very small near to zero*/
            {
                pt_fraction->ui1_type = EBML_NORMAL_NUMBER_DENO_SCALE_M;
                pt_fraction->ui8_denominator = (UINT64)1 << 63;
                if (ui4_temp <= 126) /*63 + 63 */
                {
                    pt_fraction->aui8_deno_scale[0] = (UINT64)1 << (ui4_temp - 63);
                }
                else if (ui4_temp <= 189) /*63 + 63 + 63*/
                {
                    pt_fraction->aui8_deno_scale[0] = (UINT64)1 << 63;
                    pt_fraction->aui8_deno_scale[1] = (UINT64)1 << (ui4_temp - 189);

                }
                else
                {
                    /*Note: Simple process:it is too small consider zero*/
                    /*pt_fraction->ui8_numerator = 0;*/
                    pt_fraction->ui8_denominator = 1;
                }
            }
            
                      
        }
        else if ((UINT32)i2_e >= ui4_significand)
        {
            ui4_temp = (UINT32)i2_e - ui4_significand;
            if (ui4_temp <= (64 -1 - ui4_significand))  /*M:default bit is 1*/
            {
                pt_fraction->ui8_numerator <<= ui4_temp;
                pt_fraction->ui8_denominator = 1;
                ui4_decimal = 0;
            }
            else 
            {
                pt_fraction->ui1_type = EBML_NORMAL_NUMBER_NUME_SCALE_M;
                pt_fraction->ui8_numerator <<= (64 - 1 - ui4_significand);
                ui4_temp = ui4_temp - (64 - 1 - ui4_significand);
                if (ui4_temp <= 63)
                {
                    pt_fraction->aui8_nume_scale[0] = (UINT64)1 << ui4_temp;
                }
                else if (ui4_temp <= 126) /*63 + 63 */
                {
                    pt_fraction->aui8_nume_scale[0] = (UINT64)1 << 63;
                    pt_fraction->aui8_nume_scale[1] = (UINT64)1 << (ui4_temp - 63);
                }
                else /*Note:Simple process:it is too large consider infinity*/
                {
                    pt_fraction->ui8_numerator = (UINT64)-1;
                    pt_fraction->aui8_nume_scale[0] = (UINT64)-1;
                    pt_fraction->aui8_nume_scale[1] = (UINT64)-1;
                    pt_fraction->ui8_denominator = 1;
                }
                
            }
            
        }
        else
        {   
            pt_fraction->ui8_denominator = 
                      (UINT64)(1 << (ui4_significand - (UINT32)i2_e));
            ui4_decimal = (UINT32)(ui4_significand - (UINT32)i2_e);
        }

        if (EBML_NORMAL_NUMBER_M == pt_fraction->ui1_type)
        {
            ui8_temp = 1;
            for (ui1_idx = 0; ui1_idx < (UINT8)ui4_decimal; ++ui1_idx)
            {
                ui8_temp *= 10;
            }

            /*maybe overlow... ...*/
            pt_fraction->ui8_int_part = 
                   _mm_div64((pt_fraction->ui8_numerator) * ui8_temp,
                              pt_fraction->ui8_denominator,
                      NULL);
            pt_fraction->ui8_scale_part = ui8_temp;
        }
        else
        {
            /*here will truncate some too small numerical value*/
        }
        
    }
    else if (0 == ui2_exponent)
    {
        pt_fraction->ui1_type = EBML_NORMAL_NUMBER_M;
        /*Zero */
        if (0 == ui8_mantissa)
        {
            pt_fraction->ui8_numerator = 0;
            pt_fraction->ui8_denominator = 1;
        }
        else /*very very very samll */
        {
            /*Denormalized :value: (+/-) 0.m * 2^(-1022)*/
            /*Note: Simple process:it is too small consider zero*/            
            pt_fraction->ui1_type = EBML_SMALL_NUMBER_M;
            pt_fraction->ui8_numerator = 0;
            pt_fraction->ui8_denominator = 1;
        }
        
    }
    else  /*(0x07FF == ui2_exponent)*/
    {

        if (0 == ui8_mantissa) /*infinity*/
        {
            pt_fraction->ui1_type = (UINT8)(ui1_sign? EBML_NEG_INFINITY_M: EBML_POS_INFINITY_M);
            pt_fraction->ui8_numerator = (UINT64)(-1);
            pt_fraction->aui8_nume_scale[0] = (UINT64)-1;
            pt_fraction->aui8_nume_scale[1] = (UINT64)-1;
            pt_fraction->ui8_denominator = 1;
        }
        else /*Not a number*/
        {
            pt_fraction->ui1_type = EBML_NOT_A_NUMBER_M;
            pt_fraction->ui8_numerator = 0;
            pt_fraction->ui8_denominator = 1;
        }
    }

    /*do not take account of EBML_NOT_A_NUMBER_M*/
    if ((EBML_NORMAL_NUMBER_NUME_SCALE_M == pt_fraction->ui1_type)
        || (EBML_NEG_INFINITY_M == pt_fraction->ui1_type)
        || (EBML_POS_INFINITY_M == pt_fraction->ui1_type))
    {
        pt_fraction->ui8_int_part = (UINT64)(-1);
    }


    
    
    return EBMLR_OK;
}
/*-----------------------------------------------------------------------------
 * Name:    _ebml_float_to_fraction
 * Description: convert IEEE754 float format to a fraction presentation
 *
 * Inputs:   f8_value: Buffur address 
 *           ui4_size, 4 or 8 bytes
 * Outputs:  pt_fraction 
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-11-11 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_float_to_fraction(EBML_FLOAT        f8_value, 
                              UINT32            ui4_size,
                              FLOAT_FRACTION_T* pt_fraction)
{
    EBML_FLOAT32  f4_temp;
    EBML_DOUBLE64 f8_temp;
    
    if (NULL == pt_fraction)
    {
        DBG_ERROR(("_ebml_float_to_int:NULL PTR!!!\r\n"));
        return EBMLR_NULL_PTR;
    }

    if (4 == ui4_size) /*float32*/
    {
        x_memcpy(&f4_temp, &f8_value, 4);
        return _ebml_float32_to_fra(f4_temp, pt_fraction);        
    } 
    else if (8 == ui4_size) /*float64*/
    {
        /**/
        x_memcpy(&f8_temp, &f8_value, 8);
        return _ebml_double64_to_fra(f8_temp, pt_fraction);        
    }
    else
    {
        DBG_ERROR(("_ebml_float_to_int:non-support size(%d)!!!\r\n", ui4_size));
        return EBMLR_ERR;
    }
}
/*-----------------------------------------------------------------------------
 * Name:     _ebml_date
 * Description: Get a date value from stream
 *
 * Inputs:   h_ebml: Ebml lib object handle 
 *           ui8_offset: stream offset
 *           ui4_size:  data size
 * Outputs:  pi8_date
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_date(HANDLE_T   h_ebml, 
                 UINT64     ui8_offset,
                 UINT32     ui4_size,
                 EBML_DATE* pi8_date)
{
    return _ebml_int(h_ebml, ui8_offset, ui4_size, pi8_date);
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_buf_date
 * Description: Get a date value from buffer
 *
 * Inputs:   pv_buf: Buffur address 
 *           ui4_len: Buffer length
 *           ui4_size:  data size
 * Outputs:  pi8_date
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_buf_date(VOID*      pv_buf,
                     UINT32     ui4_len,
                     UINT32     ui4_size,
                     EBML_DATE* pi8_date)
{
    return _ebml_buf_int(pv_buf, ui4_len, ui4_size, pi8_date);
}

/*-----------------------------------------------------------------------------
 * Name:     _ebml_string
 * Description: Get a string from stream
 *
 * Inputs:   h_ebml: Ebml lib object handle 
 *           ui8_offset: stream offset
 *           ui4_size:  string length
 * Outputs:  NULL
 * 
 * Returns:  pi1_str
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
EBML_CHAR* _ebml_string(HANDLE_T   h_ebml, 
                        UINT64     ui8_offset,
                        UINT32     ui4_size)
{
    return NULL;
}


/*-----------------------------------------------------------------------------
 * Name:    _ebml_buf_string
 * Description: Get a string from buffer
 *
 * Inputs:   pv_buf: Buffur address 
 *           ui4_len: Buffer length
 *           ui4_size:  data size
 * Outputs:  NULL
 * 
 * Returns:  pi1_str
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
EBML_CHAR* _ebml_buf_string(VOID* pv_buf, 
                            UINT32 ui4_len,
                            UINT32 ui4_size)
{
    EBML_CHAR* pi1_str = NULL;
    
    /*Parameter Check*/
    if (NULL == pv_buf)
    {
        DBG_ERROR(("_ebml_buf_string:NULL PTR!!!\r\n"));
        return pi1_str;
    }
    
    /*size check*/
    if (ui4_size > ui4_len)
    {
        DBG_ERROR(("_ebml_buf_string:Size(%d)>Length(%d)!!!\r\n", 
                   ui4_size, ui4_len));
        return pi1_str;
    }

    if (ui4_size > 0)
    {
        pi1_str = (EBML_CHAR*)_mm_util_mem_alloc(ui4_len + 1);
        if (NULL == pi1_str)
        {
            DBG_ERROR(("_ebml_buf_string:fail to allocate memory!!!\r\n"));
            return pi1_str;
        }
        x_memcpy(pi1_str, pv_buf, ui4_size);
        pi1_str[ui4_size] = 0;
    }
    
    return pi1_str;
}

/*-----------------------------------------------------------------------------
 * Name:    _ebml_string_free
 * Description: Free a string
 *
 * Inputs:   ppi1_str: string address 
 * Outputs:  NULL
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_string_free(EBML_CHAR** ppi1_str)
{
    if ((NULL == ppi1_str) || (NULL == *ppi1_str))
    {
        DBG_ERROR(("_ebml_string_free:NULL PTR!!!\r\n"));
        return EBMLR_ERR;
    }
    _mm_util_mem_free(*ppi1_str);
    *ppi1_str = NULL;
    
    return EBMLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:     _ebml_utf8_string
 * Description: Get a UTF-8 string from stream and convert to UTF16
 *
 * Inputs:   h_ebml: Ebml lib object handle 
 *           ui8_offset: stream offset
 *           ui4_size:  string length
 * Outputs:  NULL
 * 
 * Returns:  pui2_str
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
EBML_WCHAR* _ebml_utf8_string(HANDLE_T h_ebml, 
                              UINT64   ui8_offset,
                              UINT32   ui4_size)
{
    return NULL;
}


/*-----------------------------------------------------------------------------
 * Name:    _ebml_buf_utf8_string
 * Description: Get a UTF-8 string from buffer
 *
 * Inputs:   pv_buf:   Buffur address 
 *           ui4_len:  Buffer length
 *           ui4_size: Data size
 * Outputs:  NULL
 * 
 * Returns:  pui2_str  UTF16*
 *
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
EBML_WCHAR* _ebml_buf_utf8_string(VOID*   pv_buf, 
                                  UINT32  ui4_len,
                                  UINT32  ui4_size)
{
    EBML_WCHAR* pui2_str = NULL;
    UINT8*      pui1_str;
    UINT32      ui4_count;
    UINT32      ui4_idx;
    UINT32      ui4_value;
    
    /*Parameter Check*/
    if (NULL == pv_buf)
    {
        DBG_ERROR(("_ebml_buf_utf8_string:NULL PTR!!!\r\n"));
        return pui2_str;
    }
    
    /*size check*/
    if (ui4_size > ui4_len)
    {
        DBG_ERROR(("_ebml_buf_utf8_string:Size(%d)>Length(%d)!!!\r\n", 
                   ui4_size, ui4_len));
        return pui2_str;
    }

    if (ui4_size <= 0)
    {
        DBG_ERROR(("_ebml_buf_utf8_string:zero length!!!\r\n"));
        return pui2_str;
    }
    
    /*allocate memory to store utf8 string : convert to UTF16*/
        pui2_str = (EBML_WCHAR*)_mm_util_mem_alloc(ui4_size * 2 + 2);
        if (NULL == pui2_str)
        {
            DBG_ERROR(("_ebml_buf_utf8_string:fail to allocate memory!!!\r\n"));
            return pui2_str;
        }
    x_memset(pui2_str,0,(ui4_size * 2 + 2));
    /*begin retrieve UTF8 and convert to UTF16*/
    pui1_str = (UINT8*)pv_buf;
    ui4_count = ui4_size;
    ui4_idx = 0;
    
    while (ui4_count > 0)
    {
        /*1. 0XXXXXXX                        
             0000 0000 ~ 0000 007F */
        if (pui1_str[0] < 0x80)
        {
            ui4_value = (UINT32)pui1_str[0];
            ++pui1_str;
            --ui4_count;
        }
        /*2. 110xxxxx 10xxxxxx               
             0000 0080 ~ 0000 07FF */
        else if (((pui1_str[0] >= 0xc0) && (pui1_str[0] < 0xe0)) 
                 && (ui4_count >= 2))
        {
            ui4_value = ((UINT32)(pui1_str[0] & 0x1f)) << 6;
            ui4_value += (UINT32)(pui1_str[1] & 0x3f);
            pui1_str += 2;
            ui4_count -= 2;
        }
        /*3. 1110xxxx 10xxxxxx 10xxxxxx      
             0000 0800 ~ 0000 FFFF*/
        else if (((pui1_str[0] >= 0xe0) && (pui1_str[0] < 0xf0)) 
                 && (ui4_count >= 3))
        {
            ui4_value = ((UINT32)(pui1_str[0] & 0x0f)) << 12;
            ui4_value += ((UINT32)(pui1_str[1] & 0x3f)) << 6;
            ui4_value += (UINT32)(pui1_str[2] & 0x3f);
            pui1_str += 3;
            ui4_count -= 3;            
        }
        /*4. 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx   
             0001 0000 ~ 001F FFFF */
        else if (((pui1_str[0] >= 0xf0) && (pui1_str[0] < 0xf8)) 
                 && (ui4_count >= 4))
        {
            ui4_value = ((UINT32)(pui1_str[0] & 0x07)) << 18;
            ui4_value += ((UINT32)(pui1_str[1] & 0x3f)) << 12;
            ui4_value += ((UINT32)(pui1_str[2] & 0x3f)) << 6;
            ui4_value += (UINT32)(pui1_str[3] & 0x3f);
            pui1_str += 4;
            ui4_count -= 4;            
        }
        /*5. 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
             0020 0000 ~ 03FF FFFF*/
        else if (((pui1_str[0] >= 0xf8) && (pui1_str[0] < 0xfc)) 
                 && (ui4_count >= 5))
        {
            ui4_value = ((UINT32)(pui1_str[0] & 0x03)) << 24;
            ui4_value += ((UINT32)(pui1_str[1] & 0x3f)) << 18;
            ui4_value += ((UINT32)(pui1_str[2] & 0x3f)) << 12;
            ui4_value += ((UINT32)(pui1_str[3] & 0x3f)) << 6;
            ui4_value += (UINT32)(pui1_str[4] & 0x3f);
            pui1_str += 5;
            ui4_count -= 5;            
        }
        /*6. 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 
             0400 0000 ~ 7FFF FFFF*/
        else if (((pui1_str[0] >= 0xfc) && (pui1_str[0] < 0xfe)) 
                 && (ui4_count >= 6))
        {
            ui4_value = ((UINT32)(pui1_str[0] & 0x01)) << 30;
            ui4_value += ((UINT32)(pui1_str[1] & 0x3f)) << 24;
            ui4_value += ((UINT32)(pui1_str[2] & 0x3f)) << 18;
            ui4_value += ((UINT32)(pui1_str[3] & 0x3f)) << 12;
            ui4_value += ((UINT32)(pui1_str[4] & 0x3f)) << 6;
            ui4_value += (UINT32)(pui1_str[5] & 0x3f);
            pui1_str += 6;
            ui4_count -= 6;            
        }
        /*error, skip the byte*/
        else
        {
            ++pui1_str;
            --ui4_count;
            continue;
        }
    
        /*assignment*/
        pui2_str[ui4_idx++] = (UINT16)ui4_value;
        if (ui4_idx > ui4_size)
        {
            DBG_ERROR(("_ebml_buf_utf8_string:fail to parse!!!\r\n"));
            _mm_util_mem_free(pui2_str);
            return NULL;
        }

    }
    
    /*NULL termination*/
    pui2_str[ui4_idx] = (UINT16)0;
    
    return pui2_str;
}

/*-----------------------------------------------------------------------------
 * Name:     _ebml_utf16_string_free
 * Description: free a UTF-16 string 
 *
 * Inputs:   ppui2_str
 * Outputs:  NULL
 * 
 * Returns:  EBMLR_OK   :if successful
 *           EBMLR_ERR  :if failed
 * Note:     NULL
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
INT32 _ebml_utf16_string_free(EBML_WCHAR** ppui2_str)
{
    if ((NULL == ppui2_str) || (NULL == *ppui2_str))
    {
        DBG_ERROR(("_ebml_utf16_string_free:NULL PTR!!!\r\n"));
        return EBMLR_ERR;
    }
    _mm_util_mem_free(*ppui2_str);
    *ppui2_str = NULL;
    
    return EBMLR_OK;
}


/*-----------------------------------------------------------------------------
 * Name:     _ebml_get_str_from_uft16
 * Description: convert a UTF-16 string to a ascii one
 *
 * Inputs:   ppui2_str
 * Outputs:  NULL
 * 
 * Returns:  pi1_str
 * Note:     afer using this string need to free it
 *           together with _ebml_string_free
 *
 * Author :  Lianming Lin(mcn07123)
 * 
 * History:
 *           (1)2008-10-20 : initial
 ----------------------------------------------------------------------------*/
EBML_CHAR* _ebml_get_str_from_uft16(EBML_WCHAR* pui2_str)
{
    EBML_CHAR* pi1_str = NULL;
    UINT32     ui4_buf_len;
    INT32      i4_ret;
    
    /*Parameter Check*/
    if (NULL == pui2_str)
    {
        DBG_ERROR(("_ebml_get_str_from_uft16:NULL PTR!!!\r\n"));
        return pi1_str;
    }
    
    ui4_buf_len = x_uc_w2s_strlen(pui2_str) + 1;

    pi1_str = (EBML_CHAR*)_mm_util_mem_alloc(ui4_buf_len);
    if(NULL == pi1_str)
    {
        DBG_ERROR(("_ebml_get_str_from_uft16: failed "
                   "to allocate memory for pi1_str\r\n"));
        return pi1_str;
    }

    i4_ret = x_uc_w2s_to_ps(pui2_str, pi1_str, ui4_buf_len);
    if(i4_ret != UCR_OK)
    {
        _mm_util_mem_free(pui2_str);
        pui2_str = NULL;
        DBG_ERROR(("_ebml_get_str_from_uft16:"
                   "x_uc_w2s_to_ps failed(%d)\r\n", i4_ret));
    }
    
    return pi1_str;

}

/*-----------------------------------------------------------------------------
 * Name:     _ebml_get_id
 * Description: get the ebml element id
 *
 * Inputs:   pt_element
 * Outputs:  NULL
 * 
 * Returns:  UINT32
 ----------------------------------------------------------------------------*/
UINT32 _ebml_get_ele_id(EBML_ELE_BASIC_T* pt_element)
{
    return _ebml_id_get_value(&(pt_element->t_id));
}

/*-----------------------------------------------------------------------------
 * Name:     _ebml_get_size
 * Description: get the ebml element size
 *
 * Inputs:   pt_element
 * Outputs:  NULL
 * 
 * Returns:  UINT64
 ----------------------------------------------------------------------------*/
UINT64 _ebml_get_ele_size(EBML_ELE_BASIC_T* pt_element)
{
    return _ebml_size_get_value(&(pt_element->t_size));
} 

/*-----------------------------------------------------------------------------
 * Name:     _ebml_get_ele_head_len
 * Description: get the ebml element head length
 *
 * Inputs:   pt_element
 * Outputs:  NULL
 * 
 * Returns:  UINT8
 ----------------------------------------------------------------------------*/
UINT8 _ebml_get_ele_head_len(EBML_ELE_BASIC_T* pt_element)
{
    UINT8 ui1_len;
    
    ui1_len = pt_element->t_id.ui1_id_len + pt_element->t_size.ui1_id_len;
    return ui1_len;
}

#ifdef __cplusplus
}
#endif
