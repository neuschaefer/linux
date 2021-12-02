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
 * $RCSfile: mem_buf.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *   This file contains implementation for the mini-buffer module.
 *   The mini-buffer is used to store object (represented by an array of
 *   bytes) with minimal overhead needed.
 *
 *   The difference between using this mini buffer to store object (e.g., array 
 *   of bytes) and dynamically allocating memory using x_mem_alloc() is that 
 *   multiple object are stored in one contiguous buffer, and that reference 
 *   to the object stored in the mini-buffer will be via a object id (e.g.,
 *   an index), not via direct memory address.
 *
 *   The reason for using the object id instead of direct memory address is
 *   to facilate storing the mini-buffer on filesystem. By using
 *   the object id, the client of mini-buffer can access the same
 *   object after the mini-buffer is read back into
 *   memory from filesystem.
 *   
 *   The mini-buffer can grow as needed
 *
 *   The length of the object stored in mini-buffer is limited
 *   to 255 bytes.  It can store object of 0 byte. 
 *
 *   The maximum size of mini-buffer size is limited to 65536*65536 bytes.
 *
 *   The memory buffer consists of two part:
 *
 *   1. A buffer where the actual objects are stored.
 *   2. A array containing offset of object in the buffer.
 *
 *   The first byte of the object is the len (1-255 bytes) of
 *   the data (e.g, user provided byte array) to follow. 
 *
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  include files
  --------------------------------------------------------------------------*/

#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "util/x_end_cnvt.h"
#include "dbg/x_dbg.h"
#include "file_mngr/x_fm.h"
#include "util/u_mem_buf.h"

#define NULL_BLOCK_OFFSET       0xFFFFFFFF
#define NULL_BYTE_OFFSET        0xFF

/*-------------------------------------------------------------
 * The size (byte) of the header in the internal object. 
 * The header is 1 byte, and it stores the length of the object 
 * data to follow.
 *-------------------------------------------------------------*/
#define MEM_BUF_OBJ_HDR_SZ     2

/*-------------------------------------------------------------
 *  MARCRO to convert the object's position (which
 *  is relative to beginning of the buffer address,
 *  'pui1_mem_buf' in the control struture) to address value.
 --------------------------------------------------------------*/
#define MEM_BUF_GET_OBJ_ADDR(_pt_sbuf_,_offset_) ((_pt_sbuf_)->pui1_mem_buf+(_offset_))

/*------------------------------------------------------------
 * Get the length of the object (not including the header) from
 * the object header.
 *------------------------------------------------------------*/
#define MEM_BUF_GET_OBJ_LEN(_pt_sbuf_,_offset_) (GET_UINT16_FROM_PTR_BIG_END(((_pt_sbuf_)->pui1_mem_buf+(_offset_))))

/*------------------------------------------------------------
 * Get the internal object size from the the object length (
 * header size + object length). If data length is 0, then
 * object size is 0. We only store object that has a 
 * minimum lenght of 1 byte.
 *-----------------------------------------------------------*/
#define MEM_BUF_GET_OBJ_SZ_FRM_LEN(_str_len_) (((_str_len_)>0)?((_str_len_)+MEM_BUF_OBJ_HDR_SZ):0)

/*---------------------------------------------------------
 * Get the size of object (bytes) [e.g., data without the object header.
-----------------------------------------------------------*/
#define MEM_BUF_GET_OBJ_SZ_FROM_ADDR(_obj_addr_) (MEM_BUF_GET_OBJ_SZ_FRM_LEN(GET_UINT16_FROM_PTR_BIG_END((_obj_addr_))))


/*----------------------------------------------------------
 * Get the length of the object data (not including the header) from object addr.
 *----------------------------------------------------------*/
#define MEM_BUF_GET_OBJ_LEN_FROM_ADDR(_obj_addr_) (GET_UINT16_FROM_PTR_BIG_END((_obj_addr_)))
                                      
/*---------------------------------------------------------
 *  Get the size of object.
----------------------------------------------------------*/
#define MEM_BUF_GET_OBJ_SZ(_pt_sbuf_,_idx_) \
 (  MEM_BUF_GET_OBJ_SZ_FROM_ADDR (MEM_BUF_GET_OBJ_ADDR(_pt_sbuf_,(_pt_sbuf_)->pui4_ary[(_idx_)]))  )

/*------------------------------------------------------------
 * Get the absolute address of object data in the internal object.
--------------------------------------------------------------*/
#define MEM_BUF_GET_OBJ_DATA_ADDR(_obj_addr_)  (((UINT8*)(_obj_addr_))+MEM_BUF_OBJ_HDR_SZ)

#define MEM_BUF_ARY_INDX_TO_OBJ_ID(_idx_)     ((_idx_)+1)

#define MEM_BUF_OBJ_ID_TO_ARY_INDX(_str_id_)  ((_str_id_)-1)

/*-------------------------------------------------------------------------
  static function prototypes
  -------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 * Name:  _mem_buf_mem_usage
 *
 * Description:
 *   Diagnostic utility: print out MEM_BUF_T structure memory usage.
 *
 * Inputs:
 *   pt_membuf     Ref to MEM_BUF_T structure.
 *
 * Outputs:
 *   None.
 *   
 * Returns:
 *   VOID
 *   
 ----------------------------------------------------------------------------*/
VOID  _mem_buf_mem_usage(MEM_BUF_T*  pt_membuf)
{
#ifdef DEBUG
    x_dbg_stmt("memory buffer allocated: %d (bytes), object array allocated: %d (bytes) \n",
               pt_membuf->ui4_obj_buf_sz,
               pt_membuf->ui2_ary_len*sizeof(UINT32));
    x_dbg_stmt("memory address for the buffer: 0x%x\n", pt_membuf->pui1_mem_buf);
    x_dbg_stmt("memory address for the offset array: 0x%x\n",
               pt_membuf->pui4_ary);
    
    x_dbg_stmt("number of objects stored: %d, total memory used by "
               "objects: %d\n",
               pt_membuf->ui2_nb_obj,
               pt_membuf->ui4_mem_used);
    
#endif
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _mem_buf_diagnostic
 *
 * Description:
 *   Diagnostic function print out the mini-buffer (
 *   memory locations of object, object value).
 *
 * Inputs:
 *   pt_membuf     Ref to MEM_BUF_T structure.
 *
 * Outputs:
 *   None.
 *   
 * Returns:
 *   VOID
 *   
 ----------------------------------------------------------------------------*/
VOID  _mem_buf_diagnostic(MEM_BUF_T*  pt_membuf)
{
#ifdef  DEBUG
    INT32            i4_i;
    UINT32           ui4_offset = NULL_BLOCK_OFFSET;
    CHAR*            s_str = NULL ;
    UINT8*           pui1_str = NULL;
    UINT32           ui4_unused_space = 0;
    INT32            i4_x;
    INT32            i4_len;

    if ( pt_membuf == NULL )
    {
        return;
    }
    
    _mem_buf_mem_usage(pt_membuf);

#define SZ_DIAGNOSTIC_BUF    256
    if ( (s_str = x_mem_alloc(SZ_DIAGNOSTIC_BUF)) == NULL )
    {
        return ;
    }
    
    /* print out all the object ID and it position in the internal
       memory buffer.
    */
    x_dbg_stmt("print out all object information.\n");
    for (i4_i=0; i4_i < pt_membuf->ui2_ary_len; i4_i++ )
    {
        if ( pt_membuf->pui4_ary[i4_i] != NULL_BLOCK_OFFSET )
        {
            x_dbg_stmt("idx: [%d] object ID: %d offset: %d\n",
                       i4_i,
                       MEM_BUF_ARY_INDX_TO_OBJ_ID(i4_i),
                       pt_membuf->pui4_ary[i4_i]);

            if ( pt_membuf->pui4_ary[i4_i] < ui4_offset )
            {
                ui4_offset = pt_membuf->pui4_ary[i4_i];
            }
        }
        else
        {
            x_dbg_stmt("idx: [%d] empty slot.\n",i4_i);
        }
    }
    
    x_dbg_stmt("print out all objects.\n");
    while ( ui4_offset < pt_membuf->ui4_obj_buf_sz )
    {
        INT32    i4_diag_len ;
        
        /* Get the length of the object */
        i4_i=(INT32) MEM_BUF_GET_OBJ_LEN(pt_membuf,ui4_offset);

        /* Limit the print output of object content to the size of diagnostic 
           buffer. For Diagnostic buffer of 256 bytes, only first 255 bytes 
           of object content is print out.
        */
        i4_diag_len = (i4_i > (SZ_DIAGNOSTIC_BUF-1) ) ? (SZ_DIAGNOSTIC_BUF-1) : i4_i ;
            
        pui1_str = pt_membuf->pui1_mem_buf + ui4_offset ;
        
        x_memcpy(s_str,(pui1_str+MEM_BUF_OBJ_HDR_SZ), i4_diag_len);
        s_str[i4_diag_len] = '\0';

        i4_len = MEM_BUF_GET_OBJ_SZ_FRM_LEN(i4_i);
        
        x_dbg_stmt("obj pos: %d  obj len: %d  obj: [",
                   ui4_offset, i4_len , s_str);

        for ( i4_x=0; i4_x < i4_diag_len ; i4_x++ )
        {
            x_dbg_stmt("%c",s_str[i4_x]);
        }
        x_dbg_stmt("]\n");

        /* goto the next object. */
        ui4_offset += MEM_BUF_GET_OBJ_SZ_FRM_LEN(i4_i);

        /*
         * When MEM_BUF_OBJ_HDR_SZ > 1, the ui4_offset may be not exactly at
         * the start of the object, but some bytes ahead. We need to check 
         * pt_membuf->pui4_ary to determine the actual offset.
         */
        if (MEM_BUF_OBJ_HDR_SZ > 1)
        {
            while (1)
            {
                for (i4_x = 0 ; i4_x < pt_membuf->ui2_ary_len ; i4_x++ )
                {
                    if (pt_membuf->pui4_ary[i4_x] == ui4_offset)
                    {
                        /* ui4_offset is in deed an object's offset. Simply break */
                        break;
                    }
                    else if ( (pt_membuf->pui4_ary[i4_x] > ui4_offset) &&
                              (pt_membuf->pui4_ary[i4_x] - ui4_offset <
                               MEM_BUF_OBJ_HDR_SZ))
                    {
                        ui4_unused_space+=pt_membuf->pui4_ary[i4_x] - ui4_offset;
                        ui4_offset = pt_membuf->pui4_ary[i4_x];
                        break;
                    }
                }
                if (i4_x == pt_membuf->ui2_ary_len && /* next object not reached */
                    ui4_offset < pt_membuf->ui4_obj_buf_sz)
                {
                    ui4_offset++ ;
                    ui4_unused_space++;
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            while (1)
            {
                if ( (ui4_offset < pt_membuf->ui4_obj_buf_sz)  &&
                     (pt_membuf->pui1_mem_buf[ui4_offset] == 0)
                   )
                {
                    ui4_offset++ ;
                    ui4_unused_space++;
                }
                else
                {
                    break;
                }
            }
        }
        if ( ui4_unused_space > 0 )
        {
            x_dbg_stmt("unused space: %d (byte) from %d to %d\n",
                       ui4_unused_space,
                       (ui4_offset-ui4_unused_space),
                       (ui4_offset-1));
            ui4_unused_space = 0;
        }
    }

    x_mem_free(s_str);
    
#endif
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  x_mem_buf_diagnostic
 *
 * Description:
 *   This function dumps the information about buffer memory usage
 *   and the data.
 *
 * Inputs:
 *   pt_membuf     Ref to MEM_BUF_T structure.
 *
 * Outputs:
 *   None.
 *   
 * Returns:
 *   void. 
 *   
 ----------------------------------------------------------------------------*/
VOID x_mem_buf_diagnostic(MEM_BUF_T*       pt_membuf)
{
    _mem_buf_diagnostic(pt_membuf);
    return;
}

/*-----------------------------------------------------------------------------
 * Name:  _mem_buf_compact
 *
 * Description:
 *   This function compacts the internal buffer and update the offset
 *   array.
 *
 * Inputs:
 *   pt_membuf     Ref to MEM_BUF_T structure.
 *
 * Outputs:
 *   None.
 *   
 * Returns:
 *   Amount of the contiguous space created from compaction. 
 *   
 ----------------------------------------------------------------------------*/
static INT32  _mem_buf_compact
(
    MEM_BUF_T*  pt_membuf
)
{
    UINT32      ui4_offset = 0;
    UINT32      ui4_write = 0;
    UINT32      ui4_space_found = 0;
    UINT32      ui4_nbyte = 0;
    UINT32      ui4_len   = 0;
    UINT16      ui2_k;

    /*
    x_dbg_stmt("[_mem_buf_compact] space allocated: [%d bytes] space used [%d]\n",
               pt_membuf->ui4_obj_buf_sz, pt_membuf->ui4_mem_used);
    x_dbg_stmt("[_mem_buf_compact] available space: [%d bytes]\n",
               (pt_membuf->ui4_obj_buf_sz-pt_membuf->ui4_mem_used));
    */
    
    while ( ui4_offset < pt_membuf->ui4_obj_buf_sz )
    {
        /*
         * When MEM_BUF_OBJ_HDR_SZ > 1, there may be not enough space left
         * in the buffer for an object header.
         */
        if ( ui4_offset > pt_membuf->ui4_obj_buf_sz - MEM_BUF_OBJ_HDR_SZ ||
             (ui4_len = MEM_BUF_GET_OBJ_LEN(pt_membuf,ui4_offset)) == 0)
        {
            /* x_dbg_stmt("space at loc: %d\n", ui4_offset); */
            
            /* This 'if' should only be executed once, it sets
               the write position of the very first space found.
               Once the space is found, we will always need to shift the
               object up. */
            if ( ui4_space_found == 0 )
            {
                ui4_write = ui4_offset ;
            }
            ui4_space_found++;
            ui4_offset++;
            continue;
        }

        /*
         * When MEM_BUF_OBJ_HDR_SZ > 1, the ui4_offset may be not exactly at
         * the start of the object, but some bytes ahead. We need to check 
         * pt_membuf->pui4_ary to determine the actual offset.
         */
        if (MEM_BUF_OBJ_HDR_SZ > 1)
        {
            for (ui2_k = 0 ; ui2_k < pt_membuf->ui2_ary_len ; ui2_k++ )
            {
                if (pt_membuf->pui4_ary[ui2_k] == ui4_offset)
                {
                    /* ui4_offset is in deed an object's offset. Simply break */
                    break;
                }
                else if ( (pt_membuf->pui4_ary[ui2_k] > ui4_offset) &&
                          (pt_membuf->pui4_ary[ui2_k] - ui4_offset <
                           MEM_BUF_OBJ_HDR_SZ))
                {
                    if ( ui4_space_found == 0 )
                    {
                        ui4_write = ui4_offset ;
                    }
                    ui4_space_found+=pt_membuf->pui4_ary[ui2_k] - ui4_offset;
                    ui4_offset = pt_membuf->pui4_ary[ui2_k];
                    /* [DTV00211385] [CDB enhancement] Support variable-length memory
                     * objects larger than 255 bytes
                     * don't forget to update the ui4_len! */
                    ui4_len = MEM_BUF_GET_OBJ_LEN(pt_membuf,ui4_offset);
                    break;
                }
            }
        }

        /*
          Found a object, get the size of this object
          and determine if we need to move up to fill the empty
          space that was found before this object's location.
        */
        ui4_nbyte = (UINT32) MEM_BUF_GET_OBJ_SZ_FRM_LEN(ui4_len);

        if ( (ui4_space_found > 0)    &&
             (ui4_write < ui4_offset)
            )
        {
            /* Move this object up to fill the space. */
            x_memcpy(MEM_BUF_GET_OBJ_ADDR(pt_membuf,ui4_write),
                     MEM_BUF_GET_OBJ_ADDR(pt_membuf,ui4_offset),
                     ui4_nbyte);

            /* update this object position in the offset array. */
            for (ui2_k = 0 ; ui2_k < pt_membuf->ui2_ary_len ; ui2_k++ )
            {
                if ( (pt_membuf->pui4_ary[ui2_k] != ((UINT32)NULL_BLOCK_OFFSET)) &&
                     (pt_membuf->pui4_ary[ui2_k] == ui4_offset )
                    )
                {
                    pt_membuf->pui4_ary[ui2_k] = ui4_write;
                    break;
                }
            }

            /* keep track how much space is compacted. */
            
            /* update the write position at the end of this
               object that has just been moved up.
            */
            ui4_write  += ui4_nbyte ;

            /* Done moving this object, look for the next obj. */
            ui4_offset += ui4_nbyte;
        }
        else
        {
            /* Skip this object to look for the next obj. */
            ui4_offset += ui4_nbyte ;
        }
    }

    /*
      The contigous space might contains left over data from the
      object just moved, so we need to null it out.
    */
    if ( ui4_space_found > 0 )
    {
        x_memset(
            MEM_BUF_GET_OBJ_ADDR(pt_membuf,
                                 (pt_membuf->ui4_obj_buf_sz-ui4_space_found)),
            0,
            ui4_space_found);
    }
    
#ifdef DEBUG
    /*
    x_dbg_stmt("[_mem_buf_compact] contigous space created:  %d [bytes]\n",
               ui4_space_found);
    */
#endif
    
    return (INT32)ui4_space_found;
}

/*-----------------------------------------------------------------------------
 * Name:  _mem_buf_get_low_high_mark_and_obj_id
 *
 * Description:
 *   This function find the first byte and last byte used in the memory
 *   buffer, and an object ID that has not been used.
 *
 * Inputs:
 *   pt_membuf     Ref to MEM_BUF_T structure.
 *
 * Outputs:
 *   pui2_low      low water mark (offset to the first byte used by
 *                 an internal data object.)
 *
 *   pui2_high     high water mark (offset to the last byte used by
 *                 an internal data object.)
 *
 *   pt_obj_id   object ID.
 *   
 * Returns:
 *   VOID
 *   
 ----------------------------------------------------------------------------*/
static INT32  _mem_buf_get_low_high_mark_and_obj_id
(
    MEM_BUF_T*       pt_membuf,
    UINT32*          pui4_low,
    UINT32*          pui4_high,
    OBJECT_ID_T*     pt_obj_id
)
{
    INT32       i4_i;
    INT32       i4_rc    = MEMBUFR_OK;
    UINT8*      pui1_buf = NULL;
    INT32       i4_offset = 0;
    UINT16      ui2_str_found = 0;
    UINT32      ui4_new_sz = 0;
    UINT32      ui4_old_sz = 0;
    UINT8*      pui1_tmp   = NULL;
    UINT16      ui2_new_nb = 0;

    *pui4_low     = NULL_BLOCK_OFFSET ;
    *pui4_high    = 0;
    *pt_obj_id    = MEM_BUF_NULL_OBJ_ID ;
    
    if ( pt_membuf->ui2_nb_obj == 0 )
    {
        *pui4_low  = 0;
        *pui4_high = 0;
        *pt_obj_id = MEM_BUF_ARY_INDX_TO_OBJ_ID(0);
    }
    else
    {
        for ( i4_i = 0 ; i4_i < ((INT32)pt_membuf->ui2_ary_len) ; i4_i++ )
        {
            if ( pt_membuf->pui4_ary[i4_i] != ((UINT32)NULL_BLOCK_OFFSET) )
            {
                if ( pt_membuf->pui4_ary[i4_i] < *pui4_low )
                {
                    *pui4_low = pt_membuf->pui4_ary[i4_i];
                }
            
                /* compute the offset to the last byte position
                   of this object. */
                i4_offset = (INT32) ( pt_membuf->pui4_ary[i4_i] +
                                      MEM_BUF_GET_OBJ_SZ(pt_membuf,i4_i) - 1 ) ;

                if ( ((UINT32)i4_offset) > (*pui4_high) )
                {
                    *pui4_high = (UINT32)i4_offset;
                }

                ui2_str_found++;
            }
            else
            {
                if ( *pt_obj_id == MEM_BUF_NULL_OBJ_ID )
                {
                    *pt_obj_id = (OBJECT_ID_T) MEM_BUF_ARY_INDX_TO_OBJ_ID(i4_i);

                    /* quick exit from this loop: once we have
                       inspect all the available objects and
                       that next 'pui4_str_id' is found.  */
                    if ( ui2_str_found == pt_membuf->ui2_nb_obj )
                    {
                        break;
                    }
                }
            }
        }
    }
    
    
    /* If no valid object ID found, then increase object id array. */
    if ( *pt_obj_id == MEM_BUF_NULL_OBJ_ID )
    {   
        if(((INT32)(pt_membuf->ui2_ary_len)) >= MEM_BUF_MAX_NB_STR_OBJ)
        {
            return MEMBUFR_FAIL;
        }
        ui2_new_nb = (UINT16)(pt_membuf->ui2_ary_len + MEM_BUF_ARY_GROW_INC) ;

        if ( ((INT32)ui2_new_nb) > MEM_BUF_MAX_NB_STR_OBJ )
        {
            ui2_new_nb = (UINT16) MEM_BUF_MAX_NB_STR_OBJ;
        }
        
        ui4_old_sz = pt_membuf->ui2_ary_len * sizeof(UINT32);
        ui4_new_sz = (ui2_new_nb) * sizeof(UINT32);
        
        /* expand the object ID array. */
        pui1_buf = x_mem_realloc((UINT8*)pt_membuf->pui4_ary,ui4_new_sz);

        if ( pui1_buf != NULL )
        {
            /*
              The new memory area contains unknown data, so we must
              initialize it with  NULL_BYTE_OFFSET.
            */
            pui1_tmp = pui1_buf + ui4_old_sz;
            x_memset(pui1_tmp,
                     NULL_BYTE_OFFSET,
                     (ui4_new_sz-ui4_old_sz));
            
            pt_membuf->pui4_ary     = (UINT32*)pui1_buf;
            *pt_obj_id = (OBJECT_ID_T) MEM_BUF_ARY_INDX_TO_OBJ_ID(pt_membuf->ui2_ary_len);
            pt_membuf->ui2_ary_len  = ui2_new_nb ;
        }
        else
        {
            i4_rc = MEMBUFR_OUT_OF_MEM;
        }
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name:  _mem_buf_alloc_space
 *
 * Description:
 *   This function does most of the work of find a contiguous block in
 *   the memory buffer to store the object.  If a contiguous block is
 *   not availabe, it will try deframent the memory buffer by removing 
 *   gap between the objects in the memory buffer.  If after
 *   defragmentataion, not enough space is available, then it will try
 *   x_mem_realloc() to expand the memory buffer space.
 *
 * Inputs:
 *   pt_membuf       Ref to MEM_BUF_T structure.
 *
 *   ui4_space_req   Amount of space to allocat.
 *
 * Outputs:
 *   pt_obj_id     Ref to a object which is allocated with
 *                   specified space. 
 *   
 * Returns:
 *   VOID
 *   
 ----------------------------------------------------------------------------*/
static UINT8*  _mem_buf_alloc_space
(
    MEM_BUF_T*       pt_membuf,
    UINT32           ui4_space_req,
    OBJECT_ID_T*     pt_obj_id
)
{
    INT32    i4_rc;
    UINT32   ui4_new_sz = 0;
    UINT8*   pui1_tmp = NULL;
    INT32    i4_offset;
    INT32    i4_nb_space = 0 ;
    UINT32   ui4_low;
    UINT32   ui4_high;
    UINT32   ui4_len;

    *pt_obj_id =  MEM_BUF_NULL_OBJ_ID ;
    
    /* Find an unused object id. */
    i4_rc = _mem_buf_get_low_high_mark_and_obj_id(pt_membuf,
                                                  &ui4_low,
                                                  &ui4_high,
                                                  pt_obj_id);
               
    /*
      Find the existing object and look for space available.
    */
    if ( i4_rc == MEMBUFR_OK )
    {
        if ( (pt_membuf->ui4_obj_buf_sz - pt_membuf->ui4_mem_used)
             >= ui4_space_req )
        {
            /* case 1: empty buffer, use offset 0 */
            if ( ui4_high == 0 )
            {
                pt_membuf->pui4_ary[
                    MEM_BUF_OBJ_ID_TO_ARY_INDX(*pt_obj_id)] = 0 ;
                pt_membuf->ui4_mem_used += ui4_space_req;
                pt_membuf->ui2_nb_obj++;
                return MEM_BUF_GET_OBJ_ADDR(
                    pt_membuf,
                    pt_membuf->pui4_ary[
                        MEM_BUF_OBJ_ID_TO_ARY_INDX(*pt_obj_id)]
                    );
            }

            /* case 2:  Append the new object after last object. */
            if ( (pt_membuf->ui4_obj_buf_sz-(ui4_high+1))  >=
                 ui4_space_req
                )
            {
                pt_membuf->pui4_ary[
                    MEM_BUF_OBJ_ID_TO_ARY_INDX(*pt_obj_id)]
                    = ui4_high+1;
                pt_membuf->ui4_mem_used += ui4_space_req;
                pt_membuf->ui2_nb_obj++;
                return MEM_BUF_GET_OBJ_ADDR(
                    pt_membuf,
                    pt_membuf->pui4_ary[
                        MEM_BUF_OBJ_ID_TO_ARY_INDX(*pt_obj_id)]
                    );
            }
            
            /*
              case 3: find space by looking for gaps between
                      object.  Start with the first object.
            */

            /* goto the end of first object. */
            i4_offset = (INT32) (ui4_low +
                                 MEM_BUF_GET_OBJ_SZ_FROM_ADDR(
                                     MEM_BUF_GET_OBJ_ADDR(pt_membuf,ui4_low)));
            
            while ( ((UINT32)(i4_offset+MEM_BUF_OBJ_HDR_SZ-1)) < pt_membuf->ui4_obj_buf_sz )
            {
                if ( (ui4_len=MEM_BUF_GET_OBJ_LEN(pt_membuf,i4_offset)) != 0 )
                {
                    /* jump to the end of object. */
                    i4_offset += MEM_BUF_GET_OBJ_SZ_FRM_LEN(ui4_len);
                    i4_nb_space = 0 ;
                }
                else
                {
                    /* start counting the number of space. */
                    i4_nb_space++;
                    i4_offset++;
                    if ( ((UINT32)i4_nb_space) == ui4_space_req )
                    {
                        /* start of this blank space. */
                        pt_membuf->pui4_ary[
                            MEM_BUF_OBJ_ID_TO_ARY_INDX(*pt_obj_id)] = 
                            (UINT32)(i4_offset-i4_nb_space);
                        pt_membuf->ui4_mem_used += ui4_space_req ;
                        pt_membuf->ui2_nb_obj++;
                        return MEM_BUF_GET_OBJ_ADDR(
                            pt_membuf,
                            pt_membuf->pui4_ary[
                                MEM_BUF_OBJ_ID_TO_ARY_INDX(*pt_obj_id)]
                            );
                    }
                }
            }

            /*
              case 4: space is available, but is fragmented, so there
              is no one contiguous space to store the new object.

              Solution is to compact, but this operation is expansive,
            */
            /*
            x_dbg_stmt("space allocated: [%d bytes], space used [%d bytes], "
                       "space available [%d bytes] space request [%d bytes]\n",
                       pt_membuf->ui4_obj_buf_sz,
                       pt_membuf->ui4_mem_used,
                       (pt_membuf->ui4_obj_buf_sz - pt_membuf->ui4_mem_used),
                       ui4_space_req);
                       
            x_dbg_stmt("space available but is fragmented, compact to
get the space.\n");
            */
            if ( _mem_buf_compact(pt_membuf) != 
                 (INT32) (pt_membuf->ui4_obj_buf_sz - pt_membuf->ui4_mem_used)
                )
            {
                /* the contigous space created from compacting is
                   not the same as the avaiable space that can be
                   reclaimed, something is wrong.
                */
                ABORT(DBG_CAT_INV_OP,DBG_ABRT_MEM_BUF_CORRUPTED);
            }
            /* call this function again to allocate space. */
            return (_mem_buf_alloc_space(pt_membuf,
                                         ui4_space_req,
                                         pt_obj_id));
        }
        

        /* there is not enough space left in the memory buffer to
           store the new object so we need to reallocate the additional
           space.
           
           Note: we will grow at least the request space or
           the STR_BUF_MEM_GROW_IN size, so it will be sufficient to
           store the new object.
        */
        if ( ui4_space_req > MEM_BUF_MEM_GROW_INC )
        {
            ui4_len = ui4_space_req;
        }
        else
        {
            ui4_len = MEM_BUF_MEM_GROW_INC;
        }

        ui4_new_sz =  pt_membuf->ui4_obj_buf_sz + ui4_len ;
        
        if ( ui4_new_sz > MEM_BUF_MAX_SZ )
        {
            ui4_new_sz = MEM_BUF_MAX_SZ;
        }

        if ( (ui4_new_sz - pt_membuf->ui4_mem_used) < ui4_space_req )
        {
            /* hit buffer memory limit, return null. */
            return NULL;
        }

        pt_membuf->pui1_mem_buf =
            x_mem_realloc(pt_membuf->pui1_mem_buf, ui4_new_sz);
        
        if ( pt_membuf->pui1_mem_buf != NULL )
        {
            /*
              Initialize the new space returned to NULL.
            */
            pui1_tmp = pt_membuf->pui1_mem_buf + pt_membuf->ui4_obj_buf_sz;
            x_memset(pui1_tmp, 0, (ui4_new_sz - pt_membuf->ui4_obj_buf_sz));
            
            /*
              set the offset to the high water mark + 1
            */
            /* [DTV00211385] [CDB enhancement] Support variable-length memory
             * objects larger than 255 bytes
             * 
             * We need to consider the case of empty buffer and avoid starting
             * from offset 1.  */
            pt_membuf->pui4_ary[MEM_BUF_OBJ_ID_TO_ARY_INDX(*pt_obj_id)] =
                (ui4_high == 0) ? 0 : ui4_high+1;
            pt_membuf->ui4_mem_used += ui4_space_req;
            pt_membuf->ui2_nb_obj++;
            pt_membuf->ui4_obj_buf_sz = ui4_new_sz;

            return MEM_BUF_GET_OBJ_ADDR(pt_membuf,
                            pt_membuf->pui4_ary[
                                MEM_BUF_OBJ_ID_TO_ARY_INDX(*pt_obj_id)]
                            );
        }
    }
    return NULL;
}

/*-----------------------------------------------------------------------------
 * Name: x_mem_buf_create
 *
 * Description: Allocate a MEM_BUF_T structure with user provided paramters.
 *
 * Inputs:  -
 *   ui2_nb_of_str  Initial number of object in object positon array. If 
 *                  less than MEM_BUF_MIN_ARY_LEN, it will be set to 
 *                  the minimum value.
 *
 *   ui4_buf_sz     Initial size of memory buffer to store object.
 *                  If less than MEM_BUF_MIN_BUF_SZ, then
 *                  it will be set to the minimum value.
 *
 * Outputs: -
 *   ppt_membuf     Ref to a pointer to MEM_BUF_T structure. On return,
 *                  it ref to allocated MEM_BUF_T structure.
 *   
 * Returns:
 *   MEMBUFR_OK          Success.
 *   MEMBUFR_INV_ARG     'ppt_membuf' is NULL.
 *   MEMBUFR_OUT_OF_MEM  Out of memory.
 ----------------------------------------------------------------------------*/
INT32 x_mem_buf_create
(
    UINT16           ui2_nb_of_str,
    UINT32           ui4_buf_sz,
    MEM_BUF_T**      ppt_membuf
)
{
    INT32            i4_rc = MEMBUFR_OUT_OF_MEM;
    
    if ( ppt_membuf != NULL )
    {
        *ppt_membuf = NULL ;
        if ( (*ppt_membuf=(MEM_BUF_T*)x_mem_alloc(sizeof(MEM_BUF_T)))
             != NULL )
        {
            x_memset(*ppt_membuf,0,sizeof(MEM_BUF_T));
            
            ui4_buf_sz = (UINT32)((ui4_buf_sz < MEM_BUF_MIN_BUF_SZ) ?
                                  MEM_BUF_MIN_BUF_SZ : ui4_buf_sz) ;
            
            ui2_nb_of_str = (UINT16) ((ui2_nb_of_str < MEM_BUF_MIN_ARY_LEN) ?
                                      MEM_BUF_MIN_ARY_LEN : ui2_nb_of_str) ;
            
            if ( ((*ppt_membuf)->pui1_mem_buf=x_mem_alloc(ui4_buf_sz))
                 != NULL )
            {
                if ( ((*ppt_membuf)->pui4_ary=
                      (UINT32*)x_mem_alloc(sizeof(UINT32)*
                                  ui2_nb_of_str))
                     != NULL )
                {
                    (*ppt_membuf)->ui4_obj_buf_sz = ui4_buf_sz;
                    (*ppt_membuf)->ui2_ary_len    = ui2_nb_of_str;
                    
                    x_memset((*ppt_membuf)->pui1_mem_buf,0,ui4_buf_sz);
                    x_memset((UINT8*)(*ppt_membuf)->pui4_ary,
                             NULL_BYTE_OFFSET,
                             sizeof(UINT32)*ui2_nb_of_str);
                    i4_rc = MEMBUFR_OK;
#if 0
                    x_dbg_stmt("{MEM_BUF} memory buf created: [%d entry] [%d bytes]\n",
                               (*ppt_membuf)->ui2_ary_len,(*ppt_membuf)->ui4_obj_buf_sz);
#endif                    
                }
                else
                {
                    x_mem_free((*ppt_membuf)->pui1_mem_buf);
                    x_mem_free(*ppt_membuf);
                }    
            }
            else
            {
                x_mem_free(*ppt_membuf);
            }
        }
    }
    else
    {
        i4_rc = MEMBUFR_INV_ARG;
    }
        
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: x_mem_buf_write_object
 *
 * Description:
 *   Copy the input data and store the data in the object buffer.
 *   Return a object ID to the data stored in memory buffer.  Currently,
 *   The maximum data that is can be stored is 255 bytes. Data greater
 *   than 255 bytes is silently truncated.
 *
 * Inputs:  -
 *   pt_membuf    Ref to a pointer to MEM_BUF_T structure.
 *
 *   pv_object    Pointer to the object to store. If 'NULL', then API
 *                return OK and no data is written to the memory buffer.
 *
 *   ui4_len      lenght of object data.  If '0', then API returns OK
 *                and no data is written to the memory buffer.
 *
 * Outputs: -
 *   pui4_object_id  An ID value referencing the copy of the object in the
 *                   memory buffer. 
 *   
 * Returns: -
 *   MEMBUFR_OK     Success.
 *   MEMBUFR_BAD_MAGIC   File is not a raw file.
 ----------------------------------------------------------------------------*/
extern INT32 x_mem_buf_write_object
(
    MEM_BUF_T*       pt_membuf,
    const VOID*      pv_object,
    UINT32           ui4_len,
    OBJECT_ID_T*     pt_object_id
)
{
    INT32            i4_rc     = MEMBUFR_OK;
    UINT8*           pui1_buf  = NULL;

    if ( pt_object_id != NULL )
    {
        *pt_object_id = MEM_BUF_NULL_OBJ_ID ;
        
        if ( (pv_object   != NULL)     &&
             (ui4_len     >  0 )        
            )
        {
            ui4_len = (UINT32) ((ui4_len > MEM_BUF_MAX_OBJ_LEN) ?
                                MEM_BUF_MAX_OBJ_LEN : ui4_len) ;

            pui1_buf = _mem_buf_alloc_space(pt_membuf,
                                            ui4_len+MEM_BUF_OBJ_HDR_SZ,
                                            pt_object_id);
            
            if ( (pui1_buf != NULL) && (*pt_object_id != MEM_BUF_NULL_OBJ_ID) )
            {
                /* copy the length of data to the header (first byte)
                   of object.

                   A object is header (1 byte: nb of char byte to follow)
                   +
                   nb of bytes of the actual data.
                   
                */
                PUT_UINT16_TO_PTR_BIG_END(ui4_len, pui1_buf);
                pui1_buf+=MEM_BUF_OBJ_HDR_SZ;
                x_memcpy(pui1_buf, pv_object, ui4_len);
            }
            else
            {
                i4_rc = MEMBUFR_OUT_OF_MEM;
            }
        }
    }
    else
    {
        i4_rc = MEMBUFR_INV_ARG;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: x_mem_buf_get_object_len
 *
 * Description: This API get the length of object.
 *             
 *
 * Inputs:  -
 *   pt_membuf    Ref to a pointer to MEM_BUF_T structure.
 *
 *   t_object_id     Specify the object ID. 
 *
 * Outputs: -
 *   pui4_len     Length of the object ref by the object ID.
 *
 * Returns:
 *   MEMBUFR_OK          Success.
 *   MEMBUFR_INV_OBJ_ID  Object is not found.
 ----------------------------------------------------------------------------*/
INT32 x_mem_buf_get_object_len
(
    MEM_BUF_T*       pt_membuf,
    OBJECT_ID_T      t_object_id,
    UINT32*          pui4_len
)
{
    INT32            i4_rc = MEMBUFR_OK;
    INT32            i4_len;

    *pui4_len = 0;

    /*
       If the t_object_id is a NULL value, then just return a len
       of 0.
    */
    if ( MEM_BUF_NULL_OBJ_ID == t_object_id )
    {
        return i4_rc;
    }
    
    /*
       If the t_object_id is invalid, e.g., too large, or does
       not exist in this memory buffer, then  return a len of 0,
       with MEMBUFR_INV_OBJ_ID error code.
    */
    if ( (MEM_BUF_OBJ_ID_TO_ARY_INDX(t_object_id) < ((INT32)pt_membuf->ui2_ary_len)) &&
         pt_membuf->pui4_ary[MEM_BUF_OBJ_ID_TO_ARY_INDX(t_object_id)]
         != ((UINT32)NULL_BLOCK_OFFSET)
        )
    {
        i4_len = (INT32) MEM_BUF_GET_OBJ_LEN(
            pt_membuf,
            pt_membuf->pui4_ary[MEM_BUF_OBJ_ID_TO_ARY_INDX(t_object_id)]
            );
        
        if ( i4_len > 0 )
        {
            *pui4_len = (UINT32) i4_len;
        }
        else
        {
            /* Object has a valid object ID but a length of 0,
               something is wrong. */
            ABORT(DBG_CAT_INV_OP,DBG_ABRT_MEM_BUF_CORRUPTED);
        }
    }
    else
    {
        i4_rc = MEMBUFR_INV_OBJ_ID;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: x_mem_buf_get_object
 *   copy the object ref by the specified object id to the user provided
 *   buffer.
 *   If the buffer is too small, up to the buffer size are copied.
 *   If the object ID is NULL or invalid, then null object is returned.
 *
 * Description: 
 *   pt_membuf     Ref to a mini-buffer structure.           
 *
 *   t_object_id   Specify object ID to retrive.
 *
 *   pz_buf_len    size of buffer pointed to by 'pv_buf'.
 *
 * Inputs:  -
 *   pv_buf     A buffer containing the retrived object.
 *
 *   pz_buf_len     Len of the object in copied to 'pv_buf' 
 *
 * Outputs: -
 *
 * Returns: -
 *   MEMBUFR_OK:      Success.
 *   MEMBUFR_INV_ARG: pv_buf or pz_buf_len is NULL, or *pz_buf_len is 0.
 *   MEMBUFR_INV_OBJ_ID 't_object_id' is not a valid value or is not found.
 *   MEMBUFR_INPUT_BUF_TOO_SMALL  Input buffer pv_buf is too small.
 ----------------------------------------------------------------------------*/
INT32 x_mem_buf_get_object
(
    MEM_BUF_T*            pt_membuf,
    OBJECT_ID_T           t_object_id,
    VOID*                 pv_buf,
    SIZE_T*               pz_buf_len
)
{
    UINT32           ui4_len;
    UINT8*           pui1_addr;
    INT32            i4_rc;

    if ( pv_buf   != NULL   &&
         pz_buf_len  != NULL   &&
         *pz_buf_len >  0 )
    {
        if ( (i4_rc=x_mem_buf_get_object_len(pt_membuf, t_object_id, &ui4_len))
             == MEMBUFR_OK     &&
             (ui4_len > 0)
            )
        {
            if ( *pz_buf_len >= ui4_len )
            {
                *pz_buf_len = ui4_len;
            }
            else
            {
                /* copy to the maximum of the input buffer provided.*/
                ui4_len = *pz_buf_len ;
                i4_rc = MEMBUFR_INPUT_BUF_TOO_SMALL;
            }
            
            pui1_addr = MEM_BUF_GET_OBJ_ADDR(
                pt_membuf,
                pt_membuf->pui4_ary[MEM_BUF_OBJ_ID_TO_ARY_INDX(t_object_id)]);
            
            x_memcpy(pv_buf,
                     MEM_BUF_GET_OBJ_DATA_ADDR(pui1_addr),
                     ui4_len);
        }
        else
        {
            /* If the object ID has a length of 0 or is invalid,
               then return a null object
            */
            *pz_buf_len  = 0;
        }
    }
    else
    {
        i4_rc = MEMBUFR_INV_ARG;
    }
    
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: x_mem_buf_cmp_object
 *
 * Description: This API compare object referenced by object ID with
 *    specified object.
 *
 * Inputs:  
 *    pt_membuf      Ref to a object buffer.
 *
 *    t_object_id    Specify object ID to compare with 'pv_object'.
 *
 *    pv_object      Specify the object (bytes) to compare.
 *
 * Outputs:
 *    None.
 *
 * Returns:
 *    MEMBUFR_OK          object match.
 *    MEMBUFR_INV_OBJ_ID  Object associated with object ID is not found.
 ----------------------------------------------------------------------------*/
INT32 x_mem_buf_cmp_object
(
    MEM_BUF_T*       pt_membuf,
    OBJECT_ID_T      t_object_id,
    const VOID*      pv_object,
    UINT32           ui4_obj_len
)
{
    INT32            i4_rc;
    UINT8*           pui1_addr;
    UINT32           ui4_len;
    

    if ( (i4_rc=x_mem_buf_get_object_len(pt_membuf, t_object_id, &ui4_len))
             == MEMBUFR_OK
        )
    {
        if ( ui4_len == ui4_obj_len )
        {
            if ( ui4_len == 0 )
            {
                /* compare two null object, return 0. */
                i4_rc = MEMBUFR_OK;
            }
            else
            {
                pui1_addr = MEM_BUF_GET_OBJ_ADDR(
                    pt_membuf,
                    pt_membuf->pui4_ary[
                        MEM_BUF_OBJ_ID_TO_ARY_INDX(t_object_id)]);
                
                i4_rc = x_memcmp(
                    (const VOID*)MEM_BUF_GET_OBJ_DATA_ADDR(pui1_addr),
                    pv_object,
                    ui4_len);

                if ( i4_rc == 0 )
                {
                    i4_rc = MEMBUFR_OK;
                }
                else
                {
                    i4_rc = MEMBUFR_FAIL;
                }
            }
        }
        else
        {
            /* two objects have different size, thus they don't match. */
            i4_rc = MEMBUFR_FAIL;
        }
    }
    
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: x_mem_buf_del_object
 *
 * Description: This API deletes a specified object from mini-buffer.
 *
 * Inputs:  
 *    pt_membuf      Ref to a memory buffer.
 *
 *    t_object_id    Specify object ID to delete from 'pt_membuf'.
 *                   If this object ID is invalid (e.g., NULL, not found),
 *                   then this API return without side effect.
 *
 * Outputs:
 *    None.
 *
 * Returns:
 *    MEMBUFR_OK          Success.
 *  
 ----------------------------------------------------------------------------*/
INT32 x_mem_buf_del_object
(
    MEM_BUF_T*       pt_membuf,
    OBJECT_ID_T      t_object_id
)
{
    INT32            i4_rc = MEMBUFR_OK;
    UINT32           ui4_offset;
    INT32            i4_len;
    
    if ( ( MEM_BUF_NULL_OBJ_ID != t_object_id )                 &&
         ( ((INT32)MEM_BUF_OBJ_ID_TO_ARY_INDX(t_object_id))<((INT32)pt_membuf->ui2_ary_len)) &&
         (ui4_offset=pt_membuf->pui4_ary[MEM_BUF_OBJ_ID_TO_ARY_INDX(t_object_id)])
         != ((UINT32)NULL_BLOCK_OFFSET) )
    {
        i4_len = MEM_BUF_GET_OBJ_SZ_FROM_ADDR(
            MEM_BUF_GET_OBJ_ADDR(pt_membuf,ui4_offset));
        x_memset(MEM_BUF_GET_OBJ_ADDR(pt_membuf,ui4_offset),0, (SIZE_T)i4_len);

        pt_membuf->pui4_ary[MEM_BUF_OBJ_ID_TO_ARY_INDX(t_object_id)]=
            NULL_BLOCK_OFFSET;

        pt_membuf->ui2_nb_obj -= 1 ;
        pt_membuf->ui4_mem_used -= i4_len ;
    }
    
    return i4_rc;
} 

/*-----------------------------------------------------------------------------
 * Name: x_mem_buf_del_all_objects
 *
 * Description: This API deletes all objects from mini-buffer.
 *
 * Inputs:  
 *    pt_membuf      Ref to a object buffer.
 *
 * Outputs:
 *    None.
 *
 * Returns:
 *    MEMBUFR_OK          Success.
 *    
 ----------------------------------------------------------------------------*/
INT32 x_mem_buf_del_all_objects
(
    MEM_BUF_T*       pt_membuf
)
{
    INT32            i4_rc = MEMBUFR_OK;

    if ( pt_membuf != NULL )
    {
        x_memset(pt_membuf->pui1_mem_buf, 0, pt_membuf->ui4_obj_buf_sz);
        x_memset((UINT8*)pt_membuf->pui4_ary,
                 NULL_BYTE_OFFSET,
                 pt_membuf->ui2_ary_len*sizeof(UINT32));
        pt_membuf->ui2_nb_obj = 0;
        pt_membuf->ui4_mem_used = 0;
    }
    
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: x_mem_buf_free
 *
 * Description: This free the memory resource for the specified
 *    mini-buffer.
 *
 * Inputs:  
 *    pt_membuf      Ref to a object buffer.
 *
 * Outputs:
 *    None.
 *
 * Returns:
 *    MEMBUFR_OK          Success.
 *    
 ----------------------------------------------------------------------------*/
INT32 x_mem_buf_free
(
    MEM_BUF_T*       pt_membuf
)
{
    INT32            i4_rc = MEMBUFR_OK;

    if ( pt_membuf != NULL )
    {
        x_mem_free(pt_membuf->pui1_mem_buf);
        x_mem_free(pt_membuf->pui4_ary);
        x_mem_free(pt_membuf);
    }
    return i4_rc;
}


/*-----------------------------------------------------------------------------
 * Name: x_mem_buf_persistent_sz
 *
 * Description: This API computes the space (bytes) needed to store
 *    (serialize) the mini-buffer.on filesystem.
 *
 * Inputs:  
 *    pt_membuf      Ref to a object buffer.
 *
 * Outputs:
 *    VOID
 *
 * Returns:
 *    MEMBUFR_OK          Success.
 *    
 ----------------------------------------------------------------------------*/
INT32 x_mem_buf_persistent_sz
(
    MEM_BUF_T*       pt_membuf
)
{
    INT32            i4_sz = 0;
    INT32            i4_i;
    UINT16           ui2_valid_entry = 0;
    

    if ( pt_membuf != NULL              &&
         pt_membuf->ui4_mem_used > 0    &&
         pt_membuf->ui2_nb_obj   > 0
        )
    {
        /* Find the last valid entry to determine the
           size of offset array to save.
        */
        if (  pt_membuf->ui2_ary_len == pt_membuf->ui2_nb_obj )
        {
            ui2_valid_entry = pt_membuf->ui2_nb_obj;
        }
        else 
        {
            i4_i = pt_membuf->ui2_ary_len - 1 ;
            while ( i4_i >= 0 )
            {
                if ( pt_membuf->pui4_ary[i4_i] != ((UINT32)NULL_BLOCK_OFFSET) )
                {
                    ui2_valid_entry = (UINT16)(i4_i + 1);
                    break;
                }
                i4_i--;
            }
        }

        /*
          The serialized data byte stream consists of
          1.  size of object array (4 bytes)
          2.  size of mem buffer (4 bytes)
          3.  the object array (n bytes)
          4.  memory buffer containing object data (n bytes).
        */
#if 0        
        x_dbg_stmt("{STR_BUF} number of object entry that is computed for saving: %d "
                   "size of str arry: %d\n",
                   ui2_valid_entry, pt_membuf->ui2_ary_len);
#endif        
        
        if ( ui2_valid_entry > 0 )
        {
            i4_sz = (INT32) ( 2 * sizeof(UINT32)         +
                              ui2_valid_entry*sizeof(UINT32) +
                              pt_membuf->ui4_mem_used );
        }
        else
        {
            ABORT(DBG_CAT_INV_OP,DBG_ABRT_MEM_BUF_CORRUPTED);
        }   
    }
    else
    {
        /* If membuf is NULL or the memory buffer contains no object,
           then during the serialization, we will write out
           8 bytes (0 and 0) to inidcates no offset array data and object
           data.
        */
        i4_sz = (INT32) (2 * sizeof(UINT32)) ;
    }   
    
    return i4_sz;
}

/*-----------------------------------------------------------------------------
 * Name: x_mem_buf_dup
 *
 * Description: This API make a copy of mini buffer
 *
 * Inputs:  
 *    pt_membuf      Ref to a memory buffer.
 *
 * Outputs:
 *    None
 *
 * Returns:
 *    Ref to a new mini-buffer.
 *    
 ----------------------------------------------------------------------------*/
MEM_BUF_T*  x_mem_buf_dup
(
    MEM_BUF_T*       pt_membuf
)
{
    MEM_BUF_T*       pt_new_strbuf = NULL ;

    /*
    x_mem_buf_diagnostic(pt_membuf);
    */
    
    if ( x_mem_buf_create(pt_membuf->ui2_ary_len,
                          pt_membuf->ui4_obj_buf_sz,
                          &pt_new_strbuf)
         == MEMBUFR_OK
        )
    {
        x_memcpy(pt_new_strbuf->pui1_mem_buf,
                 pt_membuf->pui1_mem_buf,
                 pt_membuf->ui4_obj_buf_sz);

        x_memcpy((UINT8*)pt_new_strbuf->pui4_ary,
                 (UINT8*)pt_membuf->pui4_ary,
                 pt_membuf->ui2_ary_len*sizeof(UINT32));

        pt_new_strbuf->ui2_nb_obj    = pt_membuf->ui2_nb_obj;
        pt_new_strbuf->ui4_mem_used  = pt_membuf->ui4_mem_used;
    }
    return pt_new_strbuf;
}

/*-----------------------------------------------------------------------------
 * Name: x_mem_buf_serialize
 *
 * Description: This API compacts the mini buffer data into a serial form
 *    (e.g., stream of bytes) which can be stored on filesystem.
 *
 *    It returns addresses vector and number of bytes for each
 *    address which need to be copied out to serialize the data.
 *
 * Inputs:  
 *    pt_membuf      Ref to a memory buffer.
 *
 * Outputs:
 *    aui1_addr_vec  array contains address of mini-buf data to serialize.
 *
 *    aui4_byte_to_copy array contains the nb of bytes to copy for the
 *                      address specified in 'aui1_addr_vec'.
 *
 * Returns:
 *    MEMBUFR_OK          Success.
 *    
 ----------------------------------------------------------------------------*/
INT32 x_mem_buf_serialize
(
    MEM_BUF_T*       pt_membuf,
    UINT32           aui4_byte_to_copy[],
    UINT8*           aui1_addr_vec[]
)
{
    INT32            i4_rc = MEMBUFR_OK;
    INT32            i4_i;
    UINT16           ui2_valid_entry = 0;
    

    if ( pt_membuf != NULL              &&
         pt_membuf->ui4_mem_used > 0    &&
         pt_membuf->ui2_nb_obj   > 0
        )
    {
        /* compact the data first.*/
        _mem_buf_compact(pt_membuf);


        if (  pt_membuf->ui2_ary_len == pt_membuf->ui2_nb_obj )
        {
            ui2_valid_entry = pt_membuf->ui2_nb_obj;
        }
        else 
        {
            i4_i = pt_membuf->ui2_ary_len - 1 ;
            while ( i4_i >= 0 )
            {
                if ( pt_membuf->pui4_ary[i4_i] != ((UINT32)NULL_BLOCK_OFFSET) )
                {
                    ui2_valid_entry = (UINT16)(i4_i + 1);
                    break;
                }
                i4_i--;
            }
        }
        
        if ( ui2_valid_entry > 0 )
        {
#if 0            
            x_dbg_stmt("{MEM_BUF} number of object entry to save: %d "
                       "size of str array: %d\n",
                       ui2_valid_entry, pt_membuf->ui2_ary_len);
#endif            
            
            aui1_addr_vec[0]      =   (UINT8*)pt_membuf->pui4_ary;
            aui4_byte_to_copy[0]  =   ui2_valid_entry*sizeof(UINT32);
            aui1_addr_vec[1]      =   pt_membuf->pui1_mem_buf;
            aui4_byte_to_copy[1]  =   pt_membuf->ui4_mem_used;
        }
        else
        {
            ABORT(DBG_CAT_INV_OP,DBG_ABRT_MEM_BUF_CORRUPTED);
        }   
    }
    else
    {
        aui4_byte_to_copy[0]  =   0;
        aui4_byte_to_copy[1]  =   0;
    }
    return i4_rc;
}

/*-----------------------------------------------------------------------------
 * Name: x_mem_buf_create_from_byte_stream
 *
 * Description: This API re-creates the mini buffer data from serial data
 *    (e.g., stream of bytes) that was saved.
 *
 * Inputs:  
 *    pt_membuf      Ref to a memory buffer.
 *
 * Outputs:
 *    aui1_addr_vec  array contains address of mini-buf data to serialize.
 *                   The data should be a sequence of
 *
 *                   size of offset array (UINT32)
 *                   size of internal memory buffer (UINT32)
 *                   array of bytes represent offset array
 *                   array of bytes represent the memory buffer.
 *                   
 * Returns:
 *    Ref to the new created mini-buffer object. If error, return NULL.
 *    
 ----------------------------------------------------------------------------*/
MEM_BUF_T* x_mem_buf_create_from_byte_stream
(
    UINT32           aui4_byte_to_copy[],
    UINT8*           aui1_addr_vec[]
)
{
    MEM_BUF_T*       pt_membuf  = NULL;
    UINT32           ui4_sz_buf = 0;
    UINT32           ui4_nb_entry = 0;
    UINT16           ui2_i;
    UINT16           ui2_nb_of_str =0 ;
    
    

    /* Determine the size of input byte array to read.
       Round up 4 bytes multiple for the number of bytes to
       store the input data.
    */
    ui4_nb_entry = (( aui4_byte_to_copy[0] + 3 ) & 0xFFFFFFFC)
        / sizeof(UINT32);
    
    ui4_sz_buf   = ( aui4_byte_to_copy[1]  + 3 ) & 0xFFFFFFFC;


    if ( x_mem_buf_create((UINT16)ui4_nb_entry, ui4_sz_buf, &pt_membuf)
         == MEMBUFR_OK )       
    {
#ifdef DEBUG        
   x_dbg_stmt("copy offset array: %d bytes.\n",aui4_byte_to_copy[0]);
#endif
        /* copy the object array. */
        x_memcpy(((UINT8*)pt_membuf->pui4_ary),
                 aui1_addr_vec[0],
                 aui4_byte_to_copy[0]);

        ui2_nb_of_str = (UINT16) ( aui4_byte_to_copy[0] / sizeof(UINT32) ) ;

        pt_membuf->ui2_nb_obj = 0;
        for ( ui2_i = 0 ; ui2_i < ui2_nb_of_str; ui2_i++ )
        {
            if ( pt_membuf->pui4_ary[ui2_i] != ((UINT32)0xFFFFFFFF) )
            {
                pt_membuf->ui2_nb_obj++;
            }
        }

#ifdef DEBUG        
   x_dbg_stmt("copy memory buffer: %d bytes.\n",aui4_byte_to_copy[1]);
#endif        
        /* copy the string buffer data. */        
        x_memcpy((UINT8*)pt_membuf->pui1_mem_buf,
                 aui1_addr_vec[1],
                 aui4_byte_to_copy[1]);
        pt_membuf->ui4_mem_used = aui4_byte_to_copy[1];
    }
    return pt_membuf;
}



