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
/*-----------------------------------------------------------------------
 * This file is for object packing and address tranlsation. 
 *
 * Argument package:
 *
 * Basic      +---------------+ <---------------- pv_pack_buff
 *             | State         |
 *             | OP string     |
 *             | Callback Addr |
 *             | Return        |
 *             | Arg count     |
 *             | Element TOP   |
 *  Arguments  +---------------+ <---------------- ARG start
 *             | Argument1     |
 *             | Argument2     |
 *             ~               ~
 *             ~               ~
 *             | ArgumentN     |
 *             +---------------+ <---------------- ARG ex desc
 *             |               |
 *             | Arg1 ex desc  |
 *             ~               ~
 *            ~               ~
 *             | ArgN ex desc  |
 *  Elements   +---------------+ <---------------- Element start.
 *             | ELEM SIZE     |--- Total size of element.
 *             | DESC for Ref1 |--- Block description
 *             | DATA          |--- Pointer refrenced data.
 *             +---------------+
 *             | ELEM SIZE     |
 *             | DESC for Ref2 |
 *             | DATA          |
 *             +---------------+
 *             | ELEM          |
 *             ~               ~
 *             ~               ~
 *             | ELEM          |
 *             +---------------+ <----------------- pv_pack_buff + Element end
 *             |               |
 *             ~ ~ ~ ~ ~ ~ ~ ~ ~
 * 
 * 
 * DATA & ARGS :
 * 
 * 1, Pack the data together in the object graph from 'root' (Arguments).
 *    using DFS/BFS, which goes through each objects and copy its data to package.
 *    And the reference in data and arguments will be modified to be
 *    offset, which is the data address in package substracts package base.
 * 
 * 2, Translate the data by resolving offset to local address. The address is
 *    package base + offset. Then a new object graph is rebuilt in receiver site.
 *    We can pass it to receiver's local APIs.
 * 
 * 
 * TODO : Add return package here:
 *
 * 3, Restore the data by rewriting local address to be RAW reference if data will be
 *    updated to caller local memory. These data with return value will be packed in 
 *    return package.
 * 
 * 4, Writeback address for output from return package.
 * 
 * ---------------------------------------------------------------------*/

/*------------------------------------------------------------------------
                    include files
 -----------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include HEADER_COMMON_H
#include "_ipc.h"
#include "_rpc_ipc_util.h"
#include "_ipc_packer.h"
/*------------------------------------------------------------------------
                    macros, defines, typedefs, enums
 -----------------------------------------------------------------------*/
#define MAX_OP_SIZE 64

#define BLK_DESC_SIZE(pv_desc) \
        (SIZE_T)(\
            sizeof(BLK_DESC_T) \
          + ((BLK_DESC_T *)(pv_desc))->ui4_num_entries * sizeof(BLK_DESC_ENTRY_T)\
        )
    
#define BLK_DESC_GET_ENTRY_START(pt_desc) \
        ((BLK_DESC_ENTRY_T *)((BLK_DESC_T *)(pt_desc) + 1))
    
#define FOREACH_BLK_ENTRY(pt_blk_desc, pt_blk_entry) \
        for((pt_blk_entry) = BLK_DESC_GET_ENTRY_START((pt_blk_desc));\
            (pt_blk_entry) - BLK_DESC_GET_ENTRY_START((pt_blk_desc)) \
                < (pt_blk_desc)->ui4_num_entries;\
            (pt_blk_entry) ++)

#define PACK_REF_BASIC(pv_pack_buff) ((PACK_BASIC_T *)pv_pack_buff)

#define PACK_REF_ARG_BASE(pv_pack_buff) \
    ((ARG_DESC_T *)(\
        (CHAR *)PACK_REF_BASIC((pv_pack_buff)) + sizeof(PACK_BASIC_T)\
    ))

#define PACK_REF_ARG_END(pv_pack_buff) \
    ((ARG_DESC_T *)(\
        (CHAR *)PACK_REF_ARG_BASE((pv_pack_buff))\
    +   (PACK_REF_BASIC((pv_pack_buff)))->ui4_num_args * sizeof(ARG_DESC_T)\
    ))

#define PACK_REF_ARG_EX_DESC_BASE(pv_pack_buff) \
    ((ARG_EX_DESC_T *)(\
        (CHAR *)PACK_REF_ARG_END((pv_pack_buff))\
    ))

#define PACK_REF_ARG_EX_DESC_END(pv_pack_buff) \
    ((ARG_EX_DESC_T *)(\
        (CHAR *)PACK_REF_ARG_EX_DESC_BASE((pv_pack_buff))\
    +   (PACK_REF_BASIC((pv_pack_buff)))->ui4_num_args * sizeof(ARG_EX_DESC_T)\
    ))


#define PACK_REF_ELEM_BASE(pv_pack_buff) \
    ((VOID *)(\
        (CHAR *)PACK_REF_ARG_EX_DESC_END((pv_pack_buff))\
    ))

#define PACK_REF_ELEM_END(pv_pack_buff) \
    ((VOID *)(\
        (CHAR *)(pv_pack_buff) + PACK_REF_BASIC((pv_pack_buff))->ui4_elem_end \
    ))

#define PACK_FOR_EACH_ELEM(pv_pack_buff, pv_elem, ui4_idx) \
    for((pv_elem) = PACK_REF_ELEM_BASE((pv_pack_buff)), ui4_idx = 0;\
        (CHAR *)pv_elem < (CHAR *)PACK_REF_ELEM_END((pv_pack_buff));\
        (pv_elem) = ELEM_NEXT((pv_elem)), (ui4_idx) ++)


#define ELEM_REF_SIZE(pv_elem) ((SIZE_T *)(pv_elem))

#define ELEM_SIZE(pt_desc) \
    ((SIZE_T)(  sizeof(UINT32) \
              + BLK_DESC_SIZE(pt_desc) \
              + sizeof(VOID *) \
              + (((pt_desc)->z_size + 0x3) & ~0x3))\
    )

#define ELEM_NEXT(pv_elem) \
    ((VOID *)(\
        ((CHAR *)((pv_elem)) + *ELEM_REF_SIZE((pv_elem)))\
    ))

#define ELEM_REF_DESC(pv_elem) \
    ((BLK_DESC_T *)((CHAR *)ELEM_REF_SIZE((pv_elem)) + sizeof(SIZE_T)))

#define ELEM_REF_DATA(pv_elem) \
    ((VOID **) \
         (\
             (CHAR *)ELEM_REF_DESC((pv_elem)) \
           + BLK_DESC_SIZE(ELEM_REF_DESC((pv_elem)))\
         )\
    )



#define PACK_FOREACH_ARG(pv_pack_buff, ui4_arg_idx, pt_arg_itr, pt_arg_ex_desc) \
for((ui4_arg_idx)    = 0, \
    (pt_arg_itr)     = PACK_REF_ARG_BASE((pv_pack_buff)), \
    (pt_arg_ex_desc) = PACK_REF_ARG_EX_DESC_BASE((pv_pack_buff));\
    (ui4_arg_idx)    < PACK_REF_BASIC((pv_pack_buff))->ui4_num_args;\
    (ui4_arg_idx) ++, (pt_arg_itr) ++, (pt_arg_ex_desc) ++)

/*#define IPC_MAKE_OFFSET(pv_base, z_off) ((VOID *)((CHAR *)(pv_base) + (z_off)))*/



#define RET_PACK_REF_RETURN(pv_ret_pack_buff)    ((ARG_DESC_T *)(pv_ret_pack_buff))
#define RET_PACK_REF_BLK_COUNT(pv_ret_pack_buff) ((CHAR *)RET_PACK_REF_RETURN(pv_ret_pack_buff) + sizeof(ARG_DESC_T))
#define RET_PACK_REF_BLK_START(pv_ret_pack_buff) ((CHAR *)RET_PACK_REF_BLK_COUNT(pv_ret_pack_buff) + sizeof(UINT32))

#define RET_PACK_BLK_SIZE(z_data_size)  ((SIZE_T)(((z_data_size) + sizeof(VOID *) + sizeof(SIZE_T) + 0x3) & ~0x3))

#define RET_PACK_BLK_REF_RAW(pv_blk)    ((VOID **)(pv_blk))
#define RET_PACK_BLK_REF_SIZE(pv_blk)   ((SIZE_T *)((CHAR *)RET_PACK_BLK_REF_RAW(pv_blk) + sizeof(VOID *)))
#define RET_PACK_BLK_REF_DATA(pv_blk)   ((SIZE_T *)((CHAR *)RET_PACK_BLK_REF_SIZE(pv_blk) + sizeof(SIZE_T)))
#define RET_PACK_BLK_REF_NEXT(pv_blk)   ((VOID *)((CHAR *)(pv_blk) + RET_PACK_BLK_SIZE(*RET_PACK_BLK_REF_SIZE(pv_blk))))



typedef struct _BLK_DESC_T
{  
    ARG_TYPE_T      e_type;     
    VOID *          pv_start_raw;
    SIZE_T          z_size;
    UINT32          ui4_num_entries;   

    /* Entries will be followed this header structure */
    /*BLK_DESC_ENTRY_T  at_desc_entries [];*/
}  BLK_DESC_T;

typedef struct _BLK_DESC_ENTRY_T
{    
    ARG_TYPE_T      e_type;     
    SIZE_T          z_offs; 

    VOID *          pv_raw;   

    /* This is the initial data relative addr 
     * in message , used to check whether this is modified          
     */
    SIZE_T          z_init_offs;

    /* TODO : Make this in union */
    SIZE_T          z_const_size;
}  BLK_DESC_ENTRY_T;

typedef struct _PACK_BASIC_T
{    
    CHAR         ps_op[MAX_OP_SIZE];
    VOID *       pv_cb_addr;
/*    ARG_DESC_T   t_return;*/
    UINT32       ui4_num_args;
    UINT32       ui4_elem_end;
}PACK_BASIC_T;


typedef struct _ARG_EX_DESC_T
{
    VOID *  pv_raw;
}ARG_EX_DESC_T;


/*------------------------------------------------------------------------
                    functions declarations
 -----------------------------------------------------------------------*/
static VOID * _push_elem(VOID * pv_pack_buff, SIZE_T z_max_size, BLK_DESC_T * pt_desc);


static VOID * _find_elem_by_raw(VOID * pv_pack_buff, VOID * pv_raw_addr);

static VOID * _find_elem_by_data(VOID * pv_pack_buff, VOID * pv_data);

static BLK_DESC_T * _get_blk_desc(
    ARG_TYPE_T              t_arg_type,
    VOID *                  pv_addr,
    ipc_get_rpc_desc_fct    pf_get_rpc_desc,
    VOID *                  pv_tag);

static VOID _free_blk_desc(BLK_DESC_T * pt_desc);

static SIZE_T _pack_arg_ref(
    VOID *                      pv_pack_buff,
    SIZE_T                      z_max_size,
    ARG_DESC_T *                pt_arg,
    ipc_get_rpc_desc_fct        pf_get_rpc_desc,
    VOID *                      pv_tag);


static VOID * _get_raw_addr(VOID * pv_pack_buff, VOID * pv_data, BLK_DESC_ENTRY_T * pt_entry);

static VOID _trace_package(VOID * pv_pack_buff);

/*------------------------------------------------------------------------
                    data declarations
 -----------------------------------------------------------------------*/

/*------------------------------------------------------------------------
 * Name:  _push_elem
 *
 * Description: Push a new element to the message buff.
 *
 * Inputs:  pv_pack_buff:    Buff start address
 *          z_max_size: Buff size range          
 *          pv_addr:    Raw address
 *          pt_desc:    Block description
 *
 * Outputs: -
 *
 * Returns: Element start address.
 -----------------------------------------------------------------------*/
static VOID *
_push_elem(
    VOID *          pv_pack_buff,
    SIZE_T          z_max_size,
    BLK_DESC_T *    pt_desc)
{
    VOID *              pv_elem;
    PACK_BASIC_T *   pt_basic;
    VOID *              pv_elem_top;
    SIZE_T              z_elem_size;

    pt_basic     = PACK_REF_BASIC(pv_pack_buff);
    pv_elem_top  = PACK_REF_ELEM_END(pv_pack_buff);
    pv_elem      = pv_elem_top;

    z_elem_size  = ELEM_SIZE(pt_desc);
    IPC_ASSERT(pt_basic->ui4_elem_end + z_elem_size <= z_max_size);
    
    pt_basic->ui4_elem_end += z_elem_size;

    memcpy(ELEM_REF_SIZE(pv_elem), &z_elem_size,            sizeof(UINT32));
    memcpy(ELEM_REF_DESC(pv_elem), pt_desc,                 BLK_DESC_SIZE(pt_desc));
    memcpy(ELEM_REF_DATA(pv_elem), pt_desc->pv_start_raw,   pt_desc->z_size);

    IPC_TL_LOG("Pushed ELEM elem:0x%x size (0x%x)\n", 
            (unsigned)pv_elem,
            z_elem_size);

    return pv_elem;
}


/*------------------------------------------------------------------------
 * Name:  _find_elem_by_raw
 *
 * Description: 
 *   Find raw addr in message buff. 
 *
 * 
 * Inputs:  pv_pack_buff:     Buff start address
 *          pv_raw_addr: Raw address
 *
 * Outputs: -
 *
 * Returns: Element whose block region convers raw address passed.
 -----------------------------------------------------------------------*/
static VOID *
_find_elem_by_raw(VOID * pv_pack_buff, VOID * pv_raw_addr)
{
    VOID *          pv_elem;
    UINT32          ui4_i;
    BLK_DESC_T *    pt_desc;
    /*
     * This is slow. Here packer could build another lookup 
     * structure but that would be a waste. If we have some 
     * callback funtions to upper layer like get_tag and 
     * set_tag to address in the database, that maybe helpful.
     */
    PACK_FOR_EACH_ELEM(pv_pack_buff, pv_elem, ui4_i)
    {
        pt_desc = ELEM_REF_DESC(pv_elem);
        if(     (CHAR *)pv_raw_addr >=  (CHAR *)pt_desc->pv_start_raw 
            &&  (CHAR *)pv_raw_addr <   (CHAR *)pt_desc->pv_start_raw +  pt_desc->z_size)
        {            
            return pv_elem;
        }
    }
    return NULL;
}

/*------------------------------------------------------------------------
 * Name:  _find_elem_by_raw
 *
 * Description: 
 *   Find data addr in message buff. 
 * 
 * Inputs:  pv_pack_buff:  Buff start address
 *          pv_data:  Data address in buff
 *
 * Outputs: -
 *
 * Returns: Element whose block region convers data address passed.
 -----------------------------------------------------------------------*/
static VOID *
_find_elem_by_data(VOID * pv_pack_buff, VOID * pv_data)
{
    VOID *              pv_elem;
    UINT32              ui4_i;

    PACK_FOR_EACH_ELEM(pv_pack_buff, pv_elem, ui4_i)
    {
        if(    (CHAR *)pv_data >= (CHAR *)ELEM_REF_DATA(pv_elem) 
            && (CHAR *)pv_data < 
                      (CHAR *)ELEM_REF_DATA(pv_elem) 
                  +   ELEM_REF_DESC(pv_elem)->z_size)
        {            
            return pv_elem;
        }
    }
    return NULL;
}

/*------------------------------------------------------------------------
 * Name:  _find_elem_by_raw
 *
 * Description: Get size of reference type.
 * 
 * Inputs:  
 *
 * Outputs: -
 *
 * Returns: Element whose block region convers data address passed.
 -----------------------------------------------------------------------*/
static SIZE_T _get_size_of(ARG_TYPE_T e_type, VOID * pv_ref)
{
    switch(e_type & ARG_MASK_TYPE)
    {
        case ARG_TYPE_REF_CHAR:                  
            return sizeof(CHAR);
        case ARG_TYPE_REF_INT8:
            return sizeof(INT8);
        case ARG_TYPE_REF_INT16:
            return sizeof(INT16);
        case ARG_TYPE_REF_INT32:
            return sizeof(INT32);
        case ARG_TYPE_REF_UINT8:
            return sizeof(UINT8);
        case ARG_TYPE_REF_UINT16:
            return sizeof(UINT16);
        case ARG_TYPE_REF_UINT32:
            return sizeof(UINT32);
        case ARG_TYPE_REF_SIZE_T:            
            return sizeof(SIZE_T);
        case ARG_TYPE_REF_INT64:
            return sizeof(INT64);
        case ARG_TYPE_REF_UINT64:        
            return sizeof(UINT64);
        case ARG_TYPE_REF_BOOL: 
            return sizeof(BOOL);
        case ARG_TYPE_REF_STR:
            return strlen(pv_ref) + 1;
        default:
            IPC_ASSERT(0);
            return (SIZE_T)0;
    }
}
/*------------------------------------------------------------------------
 * Name:  _get_blk_desc
 *
 * Description: 
 *   Create block description (BLK_DESC_T) from RPC_DESC_T
 * 
 * Inputs:  t_arg_type:      Arg type 
 *          pv_addr:         Block address 
 *          pf_get_rpc_desc: The callback to get RPC_DESC_T
 *          pv_tag         : Tag for callback
 * Outputs: -
 *
 * Returns: Block description or NULL if there is error.
 -----------------------------------------------------------------------*/
static BLK_DESC_T * _get_blk_desc(
    ARG_TYPE_T              t_arg_type,
    VOID *                  pv_addr,
    ipc_get_rpc_desc_fct    pf_get_rpc_desc,
    VOID *                  pv_tag)
{
    RPC_DESC_ENTRY_T *  pt_rpc_entry;
    VOID *              pv_start_raw;
    BLK_DESC_T *        pt_desc;
    BLK_DESC_ENTRY_T *  pt_entry;
    RPC_DESC_T *        pt_rpc_desc;
    
    if((t_arg_type & ARG_MASK_TYPE) != ARG_TYPE_REF_DESC)
    {
        pt_desc = (BLK_DESC_T *)RPC_MALLOC(sizeof(BLK_DESC_T));     
        if(pt_desc != NULL)
        {
            pt_desc->e_type          = ARG_TYPE_STRUCT;
            pt_desc->z_size          = _get_size_of(t_arg_type, pv_addr);            
            pt_desc->ui4_num_entries = 0;
            pt_desc->pv_start_raw    = pv_addr;
        }
    }
    else
    {
        pt_rpc_desc = pf_get_rpc_desc(pv_addr, pv_tag, &pv_start_raw);

        if(pt_rpc_desc == NULL)
        {
            return NULL;
        }

        pt_desc = RPC_MALLOC(sizeof(BLK_DESC_T) + sizeof(BLK_DESC_ENTRY_T) * pt_rpc_desc->ui4_num_entries);
        if(pt_desc == NULL)
        {
            return NULL;
        }
        pt_desc->e_type             = ARG_TYPE_STRUCT;
        pt_desc->pv_start_raw       = pv_start_raw;
        pt_desc->z_size             = pt_rpc_desc->z_size;
        pt_desc->ui4_num_entries    = 0;

        FOREACH_RPC_ENTRY(pt_rpc_desc, pt_rpc_entry)
        {
            BLK_DESC_ENTRY_T *  pt_insert_entry;
            BLK_DESC_ENTRY_T *  pt_move_entry;
            FOREACH_BLK_ENTRY(pt_desc, pt_insert_entry)
            {
                if(pt_insert_entry->z_offs > pt_rpc_entry->u.z_offs)
                {   
                    pt_move_entry = &BLK_DESC_GET_ENTRY_START(pt_desc)[pt_desc->ui4_num_entries];
                    while(pt_move_entry > pt_insert_entry)
                    {
                        memcpy(pt_move_entry, pt_move_entry - 1, sizeof(BLK_DESC_ENTRY_T));
                        pt_move_entry --;
                    }
                    break;
                }
            }
            
            pt_entry                = pt_insert_entry;
            pt_entry->e_type        = pt_rpc_entry->e_type;                
            pt_entry->z_offs        = pt_rpc_entry->u.z_offs;            
            pt_entry->pv_raw        = NULL;
            pt_entry->z_init_offs   = 0;
            /*Fix Me:*/
            pt_entry->z_const_size  = pt_rpc_entry->ui4_num_entries;            

            pt_desc->ui4_num_entries ++;
        }            
    }

    
    return pt_desc;
}

/*------------------------------------------------------------------------
 * Name:  _free_blk_desc
 *
 * Description: 
 *   Free block description
 * 
 * Inputs:  pt_desc:    Created block description by _get_blk_desc     
 *
 * Outputs: -
 *
 * Returns: 
 -----------------------------------------------------------------------*/
static VOID _free_blk_desc(BLK_DESC_T * pt_desc)
{
    RPC_FREE((VOID *)pt_desc);
}

/*------------------------------------------------------------------------
 * Name:  _pack_arg_ref
 *
 * Description: 
 *   Packing from an argument.
 * 
 * Inputs:  pv_pack_buff:         Message buff base
 *          z_max_size:      Buff range
 *          pt_arg:          Argument
 *          pf_get_rpc_desc: THe callback to get RPC_DESC_T
 *          pv_tag:          Tag for callback
 * Outputs: -
 *
 * Returns: The offset (Relative addr) from the message base or 0 
 *          if it is failed.
 -----------------------------------------------------------------------*/
static SIZE_T _pack_arg_ref(
    VOID *                      pv_pack_buff,
    SIZE_T                      z_max_size,
    ARG_DESC_T *                pt_arg,
    ipc_get_rpc_desc_fct        pf_get_rpc_desc,
    VOID *                      pv_tag)
{
    BLK_DESC_T *            pt_desc;
    BLK_DESC_ENTRY_T *      pt_entry;
    
    BLK_DESC_T *            pt_desc_chld;
    
    VOID **                 ppv_chld;    
    VOID *                  pv_elem_chld;
    VOID *                  pv_elem;
    SIZE_T                  z_offs;
    SIZE_T                  z_ref_offs;

    if(pt_arg->u.pv_desc == NULL)
    {
        return 0;
    }
    
    if((pv_elem = _find_elem_by_raw(pv_pack_buff, pt_arg->u.pv_desc)) != NULL)
    {
        pt_desc = ELEM_REF_DESC(pv_elem);
        z_ref_offs = (CHAR *)pt_arg->u.pv_desc - (CHAR *)pt_desc->pv_start_raw;

        /* If the argument is marked with ARG_DIR_OUT, 
         * this will mark the whole block by this mask .
         */
        if(pt_arg->e_type & ARG_DIR_OUT)
        {
            pt_desc->e_type |= ARG_DIR_OUT;
        }
        return (SIZE_T)(z_ref_offs + (CHAR *)ELEM_REF_DATA(pv_elem) - (CHAR *)pv_pack_buff);
    }

    pt_desc = _get_blk_desc(pt_arg->e_type,
                            pt_arg->u.pv_desc,
                            pf_get_rpc_desc,
                            pv_tag);

    if(pt_desc != NULL)
    {
        /* If the argument is marked with ARG_DIR_OUT, 
         * this will mark the whole block by this mask. 
         */
        if(pt_arg->e_type & ARG_DIR_OUT)
        {                
            pt_desc->e_type |= ARG_DIR_OUT;
        }

        pv_elem = _push_elem(pv_pack_buff, z_max_size, pt_desc);
        
        z_ref_offs = (CHAR *)pt_arg->u.pv_desc - (CHAR *)pt_desc->pv_start_raw;
        
        z_offs = z_ref_offs + (CHAR *)ELEM_REF_DATA(pv_elem) - (CHAR *)pv_pack_buff;
    }
    else
    {
        /* We cannot get description */   
        _free_blk_desc(pt_desc);
        return (SIZE_T)0;
    }
    
     _free_blk_desc(pt_desc);

    /*
     * Then for each entry, get child and put its data using BFS
     * Cursor for expending new child is by pv_elem, pt_desc and pt_entry 
     */
    do
    {        
        pt_desc = ELEM_REF_DESC(pv_elem); 
        
        FOREACH_BLK_ENTRY(pt_desc, pt_entry)        
        {                   
            if(     (pt_entry->e_type & ARG_MASK_TYPE) == ARG_TYPE_REF_DESC 
                ||  (pt_entry->e_type & ARG_MASK_TYPE) == ARG_TYPE_REF_STR )
            {
                ppv_chld = (VOID **)((CHAR *)ELEM_REF_DATA(pv_elem) + pt_entry->z_offs);

                pt_entry->pv_raw = *ppv_chld;
                
                if(*ppv_chld != NULL)
                {               
                    if((pv_elem_chld = _find_elem_by_raw(pv_pack_buff, *ppv_chld)) == NULL)
                    {                
                        pt_desc_chld = _get_blk_desc(pt_entry->e_type,
                                                     *ppv_chld, 
                                                     pf_get_rpc_desc, 
                                                     pv_tag);
                        
                        if(pt_desc_chld == NULL)
                        {
                            /* If we cannot get description, it will not be modified 
                             * pt_entry->pv_raw = NULL means this is not resolved.
                             */
                            IPC_TL_LOG("Cannot get desc for (RAW:0x%x)\n", (unsigned)*ppv_chld);
                            pt_entry->z_init_offs = 0; 
                        }       
                        else
                        {
                            pv_elem_chld = _push_elem(pv_pack_buff,
                                                      z_max_size,
                                                      pt_desc_chld);
                            _free_blk_desc(pt_desc_chld);

                        }
                    }
                    if(pv_elem_chld != NULL)
                    {                       
                        pt_desc_chld = ELEM_REF_DESC(pv_elem_chld);
                        
                        /* Determine whether the desc is needed for writeback? 
                         * Fixme: This just mark blocks 'directly' generated by this
                         * argument. If the block exits already, this will not go on
                         * marking with its children. That means this assume that the 
                         * argument which generates this block has a mask of 'OUT'.
                         */
                        if( (pt_arg->e_type & ARG_DIR_OUT) &&                         
                            ((pt_entry->e_type & ARG_MASK_DIR) != ARG_DIR_INP))
                        {                            
                            pt_desc_chld->e_type |= ARG_DIR_OUT;
                        }
                          
                        pt_entry->z_init_offs = (CHAR *)*ppv_chld 
                                              - (CHAR *)ELEM_REF_DESC(pv_elem_chld)->pv_start_raw 
                                              + (CHAR *)ELEM_REF_DATA(pv_elem_chld) 
                                              - (CHAR *)pv_pack_buff;
                                              
                          
                    }
                }
                else
                {
                    /* We should not resolve NULL reference */
                    IPC_TL_LOG("NULL field\n");                
                    pt_entry->z_init_offs = 0;
                }
            }
        }

        pv_elem = ELEM_NEXT(pv_elem);
    } while((CHAR *)pv_elem < (CHAR *)PACK_REF_ELEM_END(pv_pack_buff));


    return z_offs;
}


/*------------------------------------------------------------------------
 * Name:  ipc_packer_pack
 *
 * Description: 
 *   Pack objects to a message buff.
 * 
 * Inputs:  pv_pack_buff:         Message buff base
 *          z_max_size:      Buff range
 *          ps_op:           Op string( Or CB type string)
 *          pv_cb_addr:      Callback address. (Can be NULL for OP)
 *          ui4_num_args:    Argument count.
 *          pt_args:         Argument list start address
 *          pf_get_rpc_desc: Callback fro RPC_DESC_T
 *          pv_tag:          Tag for callback
 *
 * Outputs: pz_size:         The packed message size.
 *
 * Returns: IPCR_OK:         Packed it successfully.
 -----------------------------------------------------------------------*/
INT32 ipc_packer_pack(
    VOID *                      pv_pack_buff,
    SIZE_T                      z_max_size,
    const CHAR*                 ps_op,
    VOID *                      pv_cb_addr,
    UINT32                      ui4_num_args,
    ARG_DESC_T*                 pt_args,
    ipc_get_rpc_desc_fct        pf_get_rpc_desc,
    VOID *                      pv_tag,
    SIZE_T *                    pz_size)
{
    ARG_DESC_T *        pt_arg_itr;
    UINT32              ui4_arg_idx;
    VOID *              pv_elem_base;
    PACK_BASIC_T *      pt_basic;
    SIZE_T              z_offs;
    ARG_EX_DESC_T *     pt_arg_ex_desc;
    
    IPC_ASSERT(z_max_size > MAX_OP_SIZE + sizeof(PACK_BASIC_T));
    
    IPC_TL_LOG("Start packing, BASE: 0x%x, OP: %s\n", (unsigned)pv_pack_buff, ps_op);
    
    memset(pv_pack_buff, 0, z_max_size);    
    
    /* Fill message basic section */
    pt_basic = pv_pack_buff;

    strncpy(pt_basic->ps_op, ps_op, MAX_OP_SIZE);
    pt_basic->ps_op[MAX_OP_SIZE - 1] = '\0';
    
    pt_basic->pv_cb_addr = pv_cb_addr;

    /* Fill all the arugments */
    pt_basic->ui4_num_args  = ui4_num_args;
    if ( ui4_num_args != 0 )
    {
        memcpy(PACK_REF_ARG_BASE(pv_pack_buff), pt_args, sizeof(ARG_DESC_T) * ui4_num_args);    
    }

    /* Prepare to fill elements */
    pv_elem_base = PACK_REF_ELEM_BASE(pv_pack_buff);
    pt_basic->ui4_elem_end  = (CHAR *)pv_elem_base - (CHAR *)pv_pack_buff;

    PACK_FOREACH_ARG(pv_pack_buff, ui4_arg_idx, pt_arg_itr, pt_arg_ex_desc)
    {
        IPC_TL_LOG("ARG:%u , type:%u, val:0x%08x\n",
                (unsigned)ui4_arg_idx,
                (unsigned)pt_arg_itr->e_type,
                (unsigned)pt_arg_itr->u.pv_desc);

        switch(pt_arg_itr->e_type & ARG_MASK_TYPE)
        {            
            case ARG_TYPE_REF_CHAR:
            case ARG_TYPE_REF_INT8:
            case ARG_TYPE_REF_INT16:
            case ARG_TYPE_REF_INT32:
            case ARG_TYPE_REF_INT64:
            case ARG_TYPE_REF_UINT8:
            case ARG_TYPE_REF_UINT16:
            case ARG_TYPE_REF_UINT32:
            case ARG_TYPE_REF_UINT64:
            case ARG_TYPE_REF_SIZE_T:
            case ARG_TYPE_REF_BOOL:
            case ARG_TYPE_REF_STR:
            case ARG_TYPE_REF_DESC:
                z_offs = _pack_arg_ref( pv_pack_buff, 
                                        z_max_size, 
                                        pt_arg_itr, 
                                        pf_get_rpc_desc, 
                                        pv_tag);

                if(z_offs != 0)
                {
                    IPC_TL_LOG("Modify ARG (RAW 0x%x) to (OFFSET:0x%x)\n",
                            (unsigned)pt_arg_itr->u.pv_desc,
                            (unsigned)z_offs);                                                    
                    
                    pt_arg_ex_desc->pv_raw = pt_arg_itr->u.pv_desc;                       
                    pt_arg_itr->u.pv_desc  = (VOID *)z_offs;
                }
                else
                {   
                    pt_arg_ex_desc->pv_raw = NULL;
                }
            break;
            
            case ARG_TYPE_CHAR:
            case ARG_TYPE_INT8:
            case ARG_TYPE_INT16:
            case ARG_TYPE_INT32:
            case ARG_TYPE_INT64:
            case ARG_TYPE_UINT8:
            case ARG_TYPE_UINT16:
            case ARG_TYPE_UINT32:
            case ARG_TYPE_UINT64:
            case ARG_TYPE_SIZE_T:
            case ARG_TYPE_BOOL:
            /* Note, ARG_TYPE_REF_FUNC and ARG_TYPE_REF_VOID are not resolved */ 
            case ARG_TYPE_REF_FUNC:
            case ARG_TYPE_REF_VOID:
                pt_arg_ex_desc->pv_raw  = NULL;     
            default:
                break;
        }
    }

    if(pz_size != NULL)
    {
        *pz_size = (CHAR *)PACK_REF_ELEM_END(pv_pack_buff) - (CHAR *)pv_pack_buff;
        IPC_ASSERT(*pz_size < z_max_size);
    }

    if(g_b_log == TRUE)
    {
        _trace_package(pv_pack_buff);
    }
    
    return IPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:  ipc_packer_translate
 *
 * Description: 
 *   Translate packed message to be locally accessible
 * 
 * Inputs:  pv_pack_buff:         Message buff base
 *          z_max_size:      Buff range
 *
 * Outputs: pps_op:          Op string(Or CB type string)(Can be NULL)
 *          ppv_cb_addr:     Callback address. (Can be NULL)
 *          ppt_arg_start:   Argument list start address (Can be NULL)
 *          pui4_num_args:   Argument count. (Can be NULL)
 *
 * Returns: IPCR_OK:         Packed it successfully.
 *          IPCR_INV_ARGS:   Error happens.
 -----------------------------------------------------------------------*/
INT32
ipc_packer_translate(
    VOID *          pv_pack_buff,
    SIZE_T          z_max_size,
    CHAR **         pps_op,
    VOID **         ppv_cb_addr,
    ARG_DESC_T **   ppt_arg_start,
    UINT32 *        pui4_num_args)
{
    UINT32          ui4_arg_idx;
    ARG_DESC_T *    pt_arg_itr;    
    ARG_EX_DESC_T * pt_arg_ex_desc;
    
    VOID *          pv_elem;
    UINT32          ui4_i;
    PACK_BASIC_T *  pt_basic = PACK_REF_BASIC(pv_pack_buff);
    
    if(pps_op != NULL)
    {
        *pps_op = pt_basic->ps_op;
    }
    if(ppv_cb_addr != NULL)
    {
        *ppv_cb_addr = pt_basic->pv_cb_addr;
    }
    if(pui4_num_args != NULL)
    {
        *pui4_num_args = pt_basic->ui4_num_args;
    }

    if(ppt_arg_start != NULL)
    {
        *ppt_arg_start = PACK_REF_ARG_BASE(pv_pack_buff);
    }
    
    PACK_FOREACH_ARG(pv_pack_buff, ui4_arg_idx, pt_arg_itr, pt_arg_ex_desc)
    {       
        switch(pt_arg_itr->e_type & ARG_MASK_TYPE)
        {
            case ARG_TYPE_REF_CHAR:
            case ARG_TYPE_REF_INT8:
            case ARG_TYPE_REF_INT16:
            case ARG_TYPE_REF_INT32:
            case ARG_TYPE_REF_INT64:
            case ARG_TYPE_REF_UINT8:
            case ARG_TYPE_REF_UINT16:
            case ARG_TYPE_REF_UINT32:
            case ARG_TYPE_REF_UINT64:
            case ARG_TYPE_REF_SIZE_T:
            case ARG_TYPE_REF_STR:
            case ARG_TYPE_REF_DESC:
            case ARG_TYPE_REF_BOOL:
                if(pt_arg_ex_desc->pv_raw != NULL)
                {
                    /*Fix back the address */
                    IPC_TL_LOG("ARG (OFFSET:0x%x) is translated to (LOCAL:0x%x) \n", 
                            (unsigned)pt_arg_itr->u.pv_desc, 
                            (unsigned)((CHAR *)pv_pack_buff + (UINT32)pt_arg_itr->u.pv_desc));

                    pt_arg_itr->u.pv_desc = (CHAR *)pv_pack_buff + (UINT32)pt_arg_itr->u.pv_desc;
                }
            break;
            case ARG_TYPE_REF_VOID:
            break;
            default:
            break;
        }
    }

    PACK_FOR_EACH_ELEM(pv_pack_buff, pv_elem, ui4_i)
    {
        BLK_DESC_T *        pt_desc;
        VOID *              pv_data;
        BLK_DESC_ENTRY_T *  pt_entry;

        pv_data = ELEM_REF_DATA(pv_elem);
        pt_desc = ELEM_REF_DESC(pv_elem);

        pt_entry = BLK_DESC_GET_ENTRY_START(pt_desc);        
       
        FOREACH_BLK_ENTRY(pt_desc, pt_entry)
        {
            if(     (pt_entry->e_type & ARG_MASK_TYPE) == ARG_TYPE_REF_DESC 
                ||  (pt_entry->e_type & ARG_MASK_TYPE) == ARG_TYPE_REF_STR )
            {
                if(pt_entry->z_init_offs != 0)
                {
                    VOID ** ppv_chld = (VOID **)((CHAR *)pv_data + pt_entry->z_offs);
                    IPC_TL_LOG("Translated (OFFSET:0x%x)to (LOCAL:0x%x)\n", 
                            (unsigned)*ppv_chld, 
                            (unsigned)((CHAR *)pv_pack_buff + pt_entry->z_init_offs));                
                    *ppv_chld = (CHAR *)pv_pack_buff + pt_entry->z_init_offs;

                }
            }
        }
    }

    if(g_b_log == TRUE)
    {
        _trace_package(pv_pack_buff);
    }
    return IPCR_OK;
}


/*------------------------------------------------------------------------
 * Name:  _get_raw_addr
 *
 * Description: 
 *   Caculate raw address for specified data address in buff.
 *   Sometimes the data reference in message buff might be modified.
 *   If the pv_data referenced data in buffer is no longer the
 *   entry described, the entry will be updated for new.
 * 
 * Inputs:  pv_pack_buff:    Message buff base
 *          pv_data:    Data address in buff          
 *          pt_entry:   The entry in description.   
 * Outputs: pt_entry:   The entry in description.   
 *
 * Returns: Raw addresss.
 -----------------------------------------------------------------------*/
static VOID * 
_get_raw_addr(VOID * pv_pack_buff, VOID * pv_data, BLK_DESC_ENTRY_T * pt_entry)
{
    VOID * pv_raw = NULL;
    VOID * pv_elem;
    SIZE_T z_offs;    
    
    if(    (CHAR *)pv_data >= (CHAR *)PACK_REF_ELEM_BASE(pv_pack_buff) 
        && (CHAR *)pv_data < (CHAR *)PACK_REF_ELEM_END(pv_pack_buff))
    {
        z_offs = (CHAR *)pv_data - (CHAR *)pv_pack_buff;
        
        if(z_offs == pt_entry->z_init_offs)
        {
            pv_raw = pt_entry->pv_raw;
            IPC_ERR(" _get_raw_addr, Not changed ref:0x%x, data:0x%x, z_init_offs:%d\n", 
                    (unsigned)pv_raw, 
                    (unsigned)pv_data,
                    (unsigned)pt_entry->z_init_offs);
        }
        else
        {
            pv_elem = _find_elem_by_data(pv_pack_buff, pv_data);

            IPC_ASSERT(pv_elem != NULL);
            if ( !pv_elem )
            {
                IPC_ERR(" _find_elem_by_data, NULL pv_elem \n");
                _trace_package(pv_pack_buff);
                return NULL;
            }

            pv_raw  = (CHAR *)pv_data 
                    - (CHAR *)ELEM_REF_DATA(pv_elem) 
                    + (CHAR *)ELEM_REF_DESC(pv_elem)->pv_start_raw;

            IPC_ERR(" _get_raw_addr, Changed ref:0x%x, data:0x%x, z_init_offs:%d\n", 
                    (unsigned)pv_raw, 
                    (unsigned)pv_data,            
                    (unsigned)pt_entry->z_init_offs);
        }
          
    }
    else
    {
        if(pv_data != NULL)
        {
            IPC_ERR(" _get_raw_addr, !!!!Pointer outof range!!!, pv_data: 0x%x\n", pv_data);
            _trace_package(pv_pack_buff);
            pv_raw = pt_entry->pv_raw;
        }        
        else
        {
            IPC_ERR(" _get_raw_addr, NULL reference \n");
            _trace_package(pv_pack_buff);
            pv_raw = NULL;
        }
        
    }
    return pv_raw;

}


/*------------------------------------------------------------------------
 * Name:  ipc_packer_pack_ret
 *
 * Description: 
 *   Pack return package to be sent from user "used" pack buff. The translated
 * package will be restored to raw addr and This function will go through pack
 * buff to generate a new package. 
 * 
 * Inputs:  pv_pack_buff:    Message buff base
 *          pt_return:  The return value of ipc function
 *          z_max_size: Max size of pv_ret_pack_buff.
 * Outputs:
 *          pv_ret_pack_buff: Out put buff
 *          pz_size:          The final package size of pv_ret_pack_buff.
 *  pt_entry:   The entry in description.   
 *
 * Returns: Raw addresss.
 -----------------------------------------------------------------------*/
INT32
ipc_packer_pack_ret(
    VOID *                      pv_pack_buff,
    ARG_DESC_T *                pt_return,
    VOID *                      pv_ret_pack_buff,
    SIZE_T                      z_max_size,
    SIZE_T *                    pz_size)
{
    VOID *              pv_elem;
    UINT32              ui4_i;
    UINT32              ui4_blk_count;
    VOID *              pv_ret_pack;
    
    BLK_DESC_T *        pt_desc;
    VOID *              pv_data;
    VOID *              pv_ret_data;
    BLK_DESC_ENTRY_T *  pt_entry;

    VOID * pv_copy;
    SIZE_T z_copy_off;
    SIZE_T z_copy_size;
    /* Copy the return to the buff */
    memcpy(RET_PACK_REF_RETURN(pv_ret_pack_buff), pt_return, sizeof(ARG_DESC_T));

    /* Init pz_size as first  */
    pv_ret_pack = RET_PACK_REF_BLK_START(pv_ret_pack_buff);
    *pz_size = (CHAR *)pv_ret_pack - (CHAR *)pv_ret_pack_buff;
    ui4_blk_count = 0;
    
    PACK_FOR_EACH_ELEM(pv_pack_buff, pv_elem, ui4_i)
    {
        pv_data     = ELEM_REF_DATA(pv_elem);
        pt_desc     = ELEM_REF_DESC(pv_elem);        

        /* This needed for client. */
        
        if(pt_desc->e_type & ARG_DIR_OUT)
        {   
            FOREACH_BLK_ENTRY(pt_desc, pt_entry)
            {
                if(     (pt_entry->e_type & ARG_MASK_TYPE) == ARG_TYPE_REF_DESC 
                    ||  (pt_entry->e_type & ARG_MASK_TYPE) == ARG_TYPE_REF_STR )
                {
                    VOID ** ppv_chld = (VOID **)((CHAR *)pv_data + pt_entry->z_offs);                
                    VOID *  pv_raw = _get_raw_addr(pv_pack_buff, *ppv_chld, pt_entry);
                    
                    IPC_TL_LOG("Restored (DATA:0x%x) to (RAW:0x%x)\n", 
                               (unsigned)*ppv_chld, (unsigned)pv_raw);
                            
                    /* Fix pointer tobe raw */
                    *ppv_chld = pv_raw;
                }
            }   

            
            /* Assume these are sorted */
            pv_copy = pv_data;
            FOREACH_BLK_ENTRY(pt_desc, pt_entry)
            {
                if(pt_entry->e_type == (ARG_TYPE_UINT8 | ARG_DIR_INP))
                {          
                    z_copy_off   = (CHAR *)pv_copy - (CHAR *)pv_data;
                    IPC_TL_LOG(" z_copy_off %d, ENTRY OFF %d\n", z_copy_off, pt_entry->z_offs);
                    z_copy_size  = pt_entry->z_offs - z_copy_off;                
                    IPC_ASSERT(z_copy_size + z_copy_off <= pt_desc->z_size);                    
                    
                    if(z_copy_size != 0)
                    {
                        *pz_size += RET_PACK_BLK_SIZE(z_copy_size);
                        IPC_ASSERT(*pz_size <= z_max_size);            
                        
                        *RET_PACK_BLK_REF_RAW(pv_ret_pack)  = (CHAR *)pt_desc->pv_start_raw + z_copy_off;                            
                        *RET_PACK_BLK_REF_SIZE(pv_ret_pack) = z_copy_size;
                        
                        pv_ret_data = RET_PACK_BLK_REF_DATA(pv_ret_pack);
                        
                        IPC_TL_LOG("Split const pv_copy:0x%x, copy_size %d\n", (unsigned)pv_copy, (unsigned)z_copy_size);                        
                        memcpy(pv_ret_data, pv_copy, z_copy_size);

                        pv_copy = (CHAR*)pv_copy + z_copy_size + pt_entry->z_const_size;
                        pv_ret_pack = RET_PACK_BLK_REF_NEXT(pv_ret_pack);
                        ui4_blk_count ++;
                        
                    }
                    else
                    {
                        pv_copy = (CHAR*)pv_copy + pt_entry->z_const_size;
                        IPC_TL_LOG("To next const pv_copy:0x%x, pt_entry->z_const_size %d\n", (unsigned)pv_copy, (unsigned)pt_entry->z_const_size);
                    }
                    
                }                    
            }   

            z_copy_off   = (CHAR *)pv_copy - (CHAR *)pv_data;
            z_copy_size  = pt_desc->z_size - z_copy_off;                

            if(z_copy_size != 0)
            {
                *pz_size += RET_PACK_BLK_SIZE(z_copy_size);
                IPC_ASSERT(*pz_size <= z_max_size);            
                
                *RET_PACK_BLK_REF_RAW(pv_ret_pack)  = (CHAR *)pt_desc->pv_start_raw + z_copy_off;                            
                *RET_PACK_BLK_REF_SIZE(pv_ret_pack) = z_copy_size;
                
                pv_ret_data = RET_PACK_BLK_REF_DATA(pv_ret_pack);
                IPC_TL_LOG("End pv_copy:0x%x, copy_size %d\n", (unsigned)pv_copy, (unsigned)z_copy_size);
                memcpy(pv_ret_data, pv_copy, z_copy_size);

                pv_copy = (CHAR*)pv_copy + z_copy_size + z_copy_off;
                pv_ret_pack = RET_PACK_BLK_REF_NEXT(pv_ret_pack);

                ui4_blk_count ++;
            }                    

            
        }
        
    }  
    
    *RET_PACK_REF_BLK_COUNT(pv_ret_pack_buff) = ui4_blk_count;
    IPC_TL_LOG("ret packed size: %d, bc %d\n", *pz_size, ui4_blk_count);
    
    return IPCR_OK;
}

/*------------------------------------------------------------------------
 * Name:  ipc_pack_writeback
 *
 * Description: 
 *   Update data.
 * 
 * Inputs:  pv_pack_buff:    Message buff base
 *          z_max_size: Buff range    
 *              
 *
 * Outputs: pt_return:  Return value from Buff
 *
 * Returns: IPCR_OK Successful
 -----------------------------------------------------------------------*/
INT32
ipc_packer_writeback(
    VOID *         pv_ret_pack_buff,
    SIZE_T         z_max_size,
    ARG_DESC_T   * pt_return)
{
    UINT32              ui4_i;
    UINT32              ui4_count;
    VOID *              pv_blk;
    VOID *              pv_data;
    SIZE_T              z_size;
    VOID *              pv_raw;
    
    if(pt_return != NULL)
    {
        memcpy(pt_return, RET_PACK_REF_RETURN(pv_ret_pack_buff), sizeof(ARG_DESC_T));
    }

    ui4_count   = *RET_PACK_REF_BLK_COUNT(pv_ret_pack_buff);
    pv_blk      = RET_PACK_REF_BLK_START(pv_ret_pack_buff);
    
    for(ui4_i = 0; ui4_i < ui4_count; ui4_i ++)
    {
        pv_raw = *RET_PACK_BLK_REF_RAW(pv_blk);
        z_size = *RET_PACK_BLK_REF_SIZE(pv_blk);
        pv_data = RET_PACK_BLK_REF_DATA(pv_blk);

        IPC_TL_LOG("Write 0x%x, %d\n", pv_raw, z_size);
        memcpy(pv_raw, pv_data, z_size);                 

        pv_blk = RET_PACK_BLK_REF_NEXT(pv_blk);
    }
    return IPCR_OK;
}



/*************************
 * _trace_package the message buff.
 *************************/
static VOID _trace_package(VOID * pv_buff)
{
    ARG_DESC_T *         pt_arg_itr;
    UINT32               ui4_arg_idx;
    ARG_EX_DESC_T *      pt_arg_ex_desc;

    VOID *               pv_elem;
    UINT32               ui4_i;
    PACK_BASIC_T *    pt_basic;

    pt_basic = (PACK_BASIC_T * )pv_buff;


    IPC_TL_LOG("\n\n");
    IPC_TL_LOG("[IPC MSG _trace_package Base:0x%08x]\n", (unsigned)pv_buff);
    IPC_TL_LOG("+-------------------------------------+\n");
    IPC_TL_LOG(" OP:%s\n", pt_basic->ps_op);
    IPC_TL_LOG(" CB Addr:0x%x\n", (unsigned)pt_basic->pv_cb_addr);
    IPC_TL_LOG(" Num Args:%u\n", (unsigned)pt_basic->ui4_num_args);
    IPC_TL_LOG(" Element end:0x%x\n", (unsigned)pt_basic->ui4_elem_end);
    IPC_TL_LOG(".......................................\n");

    PACK_FOREACH_ARG(pv_buff, ui4_arg_idx, pt_arg_itr, pt_arg_ex_desc) 
    {
        if(pt_arg_ex_desc->pv_raw != NULL)
        {
            IPC_TL_LOG(" ARG T:%d, V:0x%x (R:%x)\n", 
                (unsigned)pt_arg_itr->e_type, 
                (unsigned)pt_arg_itr->u.pv_desc, 
                (unsigned)pt_arg_ex_desc->pv_raw);
        }
        else
        {
            IPC_TL_LOG(
                " ARG T:%d, V:0x%x \n", 
                (unsigned)pt_arg_itr->e_type, 
                (unsigned)pt_arg_itr->u.pv_desc);
        }
    }
    PACK_FOR_EACH_ELEM(pv_buff, pv_elem, ui4_i)
    {
       BLK_DESC_T *         pt_desc;
       VOID *               pv_data;
       BLK_DESC_ENTRY_T *   pt_entry;

       pv_data = ELEM_REF_DATA(pv_elem);
       pt_desc = ELEM_REF_DESC(pv_elem);

       pt_entry = BLK_DESC_GET_ENTRY_START(pt_desc);
       
       IPC_TL_LOG(".......................................\n");
       IPC_TL_LOG(" Raw addr:0x%x\n", (unsigned)(ELEM_REF_DESC(pv_elem)->pv_start_raw));
#ifndef IPC_DUMP_DATA
       {
           UCHAR * pc_c;
           for(pc_c = pv_data; pc_c - (UCHAR*)pv_data + 8 <= pt_desc->z_size; pc_c += 8)
           {
                IPC_TL_LOG(" 0x%08x [ %02x %02x %02x %02x %02x %02x %02x %02x ]\n",
                    (unsigned)pc_c,
                    (unsigned)pc_c[0], (unsigned)pc_c[1], (unsigned)pc_c[2], (unsigned)pc_c[3],
                    (unsigned)pc_c[4], (unsigned)pc_c[5], (unsigned)pc_c[6], (unsigned)pc_c[7]);
           }
           switch(pt_desc->z_size - (pc_c - (UCHAR*)pv_data))
           {
                case 1:
                    IPC_TL_LOG(" 0x%08x [ %02x                     ]\n",
                        (unsigned)pc_c,
                        (unsigned)pc_c[0]);
                    break;
                case 2:
                    IPC_TL_LOG(" 0x%08x [ %02x %02x                   ]\n",
                        (unsigned)pc_c,
                        (unsigned)pc_c[0],
                        (unsigned)pc_c[1]);
                    break;
                case 3:
                    IPC_TL_LOG(" 0x%08x [ %02x %02x %02x                ]\n",
                        (unsigned)pc_c,
                        (unsigned)pc_c[0],
                        (unsigned)pc_c[1],
                        (unsigned)pc_c[2]);
                    break;
                case 4:
                    IPC_TL_LOG(" 0x%08x [ %02x %02x %02x %02x             ]\n",
                        (unsigned)pc_c,
                        (unsigned)pc_c[0], (unsigned)pc_c[1],
                        (unsigned)pc_c[2], (unsigned)pc_c[3]);
                    break;
                case 5:
                    IPC_TL_LOG(" 0x%08x [ %02x %02x %02x %02x %02x          ]\n",
                        (unsigned)pc_c,
                        (unsigned)pc_c[0], (unsigned)pc_c[1],
                        (unsigned)pc_c[2], (unsigned)pc_c[3],
                        (unsigned)pc_c[4]);
                    break;
                case 6:
                    IPC_TL_LOG(" 0x%08x [ %02x %02x %02x %02x %02x %02x       ]\n",
                        (unsigned)pc_c,
                        (unsigned)pc_c[0], (unsigned)pc_c[1],
                        (unsigned)pc_c[2], (unsigned)pc_c[3],
                        (unsigned)pc_c[4], (unsigned)pc_c[5]);
                    break;
                case 7:
                    IPC_TL_LOG(" 0x%08x [ %02x %02x %02x %02x %02x %02x %02x      ]\n",
                        (unsigned)pc_c,
                        (unsigned)pc_c[0], (unsigned)pc_c[1], (unsigned)pc_c[2],
                        (unsigned)pc_c[3], (unsigned)pc_c[4], (unsigned)pc_c[5],
                        (unsigned)pc_c[6]);
                    break;
           }
       }
#endif
       IPC_TL_LOG(" Desc Size:%u, Num:%u \n", (unsigned)pt_desc->z_size, (unsigned)pt_desc->ui4_num_entries);
       FOREACH_BLK_ENTRY(pt_desc, pt_entry)
       {        
           IPC_TL_LOG(" Entry: type: %x, z_offs(TO block):%u, z_init_offs(TO Msg):%u \n", (unsigned)pt_entry->e_type, (unsigned)pt_entry->z_offs, (unsigned)pt_entry->z_init_offs);
           IPC_TL_LOG("    RAW : 0x%x\n", (unsigned)pt_entry->pv_raw);
       }
    }
    IPC_TL_LOG("+-------------------------------------+\n");

    IPC_TL_LOG("\n\n");
}


