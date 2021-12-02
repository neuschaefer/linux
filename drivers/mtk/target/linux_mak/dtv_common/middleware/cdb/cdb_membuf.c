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
 * $RCSfile: cdb_membuf.c,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file contains core database memory functions lock related
 *         function implementations.
 *---------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
  include files
  ----------------------------------------------------------------------------*/

#include "dbg/dbg.h"
#include "inc/x_common.h"
#include "os/inc/x_os.h"
#include "cdb/cdb_api.h"
#include "cdb/cdb_membuf.h"
#include "cdb/_cdb.h"
/*-----------------------------------------------------------------------------
  static function prototypes
  ----------------------------------------------------------------------------*/
static INT32 _db_create_memory_chunk
(
    CDB_MEM_CHUNK_T**   pt_mem_chunk,
    UINT32              ui4_blocksize,
    UINT32              ui4_block_count
);

static VOID _db_delete_memory_chunk
(
    CDB_MEM_CHUNK_T*      pt_mem
);

static VOID _db_clean_memory_chunk
(
    CDB_MEM_CHUNK_T*   pt_mem,
    UINT32             ui4_blocksize,
    UINT32             ui4_block_count
);

static INT32 _db_get_fb_from_memchunk
(
    CDB_MEM_T*          pt_mem,
    CDB_MEM_CHUNK_T*    pt_mem_chunk,
    UINT8**             ppui1_block
);

static INT32 _db_get_free_block_dynamic_mem
(
    CDB_MEM_T*          pt_mem,
    UINT8**             ppui1_block
);

/*-----------------------------------------------------------------------------
 * Name: _db_delete_memory_chunk
 *
 * Description: This API delete a memory buffer and its metadata.
 *
 * Inputs:  - 
 *
 * Outputs: - Sorted listed
 *
 * Returns: - 0     Success.
 *            -1    Sort failed.
 ----------------------------------------------------------------------------*/

static VOID _db_delete_memory_chunk(CDB_MEM_CHUNK_T*      pt_mem)
{
    if ( pt_mem != NULL )
    {
        /* delete freelist */
        if ( pt_mem->pui4_freelist != NULL )
        {
            x_mem_free(pt_mem->pui4_freelist);
        }
        if ( pt_mem->pui1_usedlist != NULL )
        {
            x_mem_free(pt_mem->pui1_usedlist);
        }
        if ( pt_mem->pui1_mem_area != NULL )
        {
            x_mem_free(pt_mem->pui1_mem_area);
        }
        x_mem_free(pt_mem);
    }
}

/*-----------------------------------------------------------------------------
 * Name: _db_clean_memory_chunk
 *
 * Description: This API reset the free list and used list of the memory chunk,
 *              and set the memory area with NULL value..  If the memory chunk
 *              contains records, then all the records are removed.
 *              Note: This API does not change this mem chunk's next_ptr
 *              the next mem_chunk.
 *
 * Inputs:  - 
 *
 * Outputs: - Sorted listed
 *
 * Returns: - 0     Success.
 *            -1    Sort failed.
 ----------------------------------------------------------------------------*/

static VOID _db_clean_memory_chunk(CDB_MEM_CHUNK_T*   pt_mem,
                                UINT32             ui4_blocksize,
                                UINT32             ui4_block_count
)
{
    UINT32 ui4_i;
    
    if ( pt_mem != NULL )
    {
        /* set the used count to 0 */
        pt_mem->ui4_used_count = 0;

        /*
           The first element (e.g. index 0) of freelist points
           to first free block (e.g., block 1). Also, set its
           used flag.
        */
        pt_mem->pui4_freelist[0] = 1;
        pt_mem->pui1_usedlist[0] = DB_MEM_BLOCK_USED;

        /*
          Set the freelist and used list to the clean state.
        */
        for ( ui4_i=1; ui4_i < ui4_block_count ; ui4_i++ )
        {
            pt_mem->pui4_freelist[ui4_i]=ui4_i+1;
            pt_mem->pui1_usedlist[ui4_i]=DB_MEM_BLOCK_FREE;
        }

        /*
           The last element of the freelist points to the null
           index.
        */
        pt_mem->pui4_freelist[ui4_block_count]=DB_MEM_NULL_INDEX;
        pt_mem->pui1_usedlist[ui4_block_count]=DB_MEM_BLOCK_FREE;
        /*
           Set the memory region to cleaned state with NULL (0) values
        */
        x_memset(pt_mem->pui1_mem_area,0,
                 ui4_blocksize*ui4_block_count);
    }
}

/*-----------------------------------------------------------------------------
 * Name: db_delete_memory_manager
 *
 * Description: This API delete a memory manager and its memory chunks.
 *
 * Inputs:  - Pointer to the memory manager
 *
 * Outputs: - NONE
 *
 * Returns: - VOID
 ----------------------------------------------------------------------------*/

VOID db_delete_memory_manager(CDB_MEM_T*      pt_mem)
{
    INT32  i4_i;
    CDB_MEM_CHUNK_T* pt_next_chunk;
    
    if ( pt_mem != NULL )
    {
        /*
          Delete the first memory chunk, then the next chunk, and next, etc.
        */
        for ( i4_i = 0 ; i4_i < (INT32)(pt_mem->ui2_chunk_count) ; i4_i++ )
        {
            pt_next_chunk=pt_mem->pt_first_chunk->pt_next_chunk;
            _db_delete_memory_chunk(pt_mem->pt_first_chunk);
            pt_mem->pt_first_chunk=pt_next_chunk;
        }
        if ( pt_mem->pt_first_chunk != NULL )
        {
            /* Last chunk's next pointer must points to NULL.
               Otherwise, there is something wrong.
            */
            ABORT(DBG_CAT_MEMORY,DBG_ABRT_MEM_MANAGER);
        }
        x_mem_free(pt_mem);
    }
}


/*-----------------------------------------------------------------------------
 * Name: db_remove_all_records
 *
 * Description: This API remove all the records from memory chunks.  After
 *              return from this API, the count for the number of record
 *              store will be 0.
 *              Note: the memory chunks previous allocated are not deleted.
 *              So if there were 3 memory chunks being used, then the 3
 *              memory chunks remain.
 *
 * Inputs:  - Pointer to the memory manager
 *
 * Outputs: - NONE
 *
 * Returns: - VOID
 ----------------------------------------------------------------------------*/

VOID db_remove_all_records(CDB_MEM_T*      pt_mem)
{
    CDB_MEM_CHUNK_T* pt_this_chunk;
    
    if ( pt_mem != NULL )
    {
        /*
          Clean the first memory chunk, then the next chunk, and next, etc.
        */
        pt_this_chunk=pt_mem->pt_first_chunk;
        while ( pt_this_chunk != NULL )
        {
            _db_clean_memory_chunk(pt_this_chunk,
                                   pt_mem->ui4_blocksize,
                                   pt_mem->ui4_block_count);
            pt_this_chunk=pt_this_chunk->pt_next_chunk;
        }

        /* Reset the record count value and free chunk pointer to
           the first memory chunk. */
        pt_mem->ui4_record_count=0;
        pt_mem->pt_free_chunk=pt_mem->pt_first_chunk;
    }
}

/*-----------------------------------------------------------------------------
 * Name: _db_create_memory_chunk
 *
 * Description: This API creates a memory chunk and initialize its.
 *              metadata. The memory chunk consists of a number of
 *              equally sized blocks.  The maximum number of
 *              blocks allowed is 2**32-1 (4294967295L).
 *
 *              The freelist contains a linked list of indices to
 *              the memory blocks.
 *
 * Inputs:  - ui4_blocksize
 *            ui4_block_count
 *            ui4_last_index: the largest index value used by the 
 *                            previous region.
 *
 * Outputs: - Sorted listed
 *
 * Returns: - 0     Success.
 *            -1    Sort failed.
 ----------------------------------------------------------------------------*/
static INT32 _db_create_memory_chunk
(
    CDB_MEM_CHUNK_T**   pt_mem_chunk,
    UINT32              ui4_blocksize,
    UINT32              ui4_block_count
)
{
    CDB_MEM_CHUNK_T*  pt_mem=NULL;

    *pt_mem_chunk = NULL;
    
    pt_mem = (CDB_MEM_CHUNK_T*)x_mem_alloc(sizeof(CDB_MEM_CHUNK_T));
    if ( pt_mem != NULL )
    {
        /*
           Note:
                  
           The number of elements allocated for the freelist and
           used list is 1 + number of block count.
           
           The first element of freelist is reserved for the internal
           used by the memory chunk. The indices for accessing each block
           in the contiguous memory ranges from to 1 to N number of block.
        */
        pt_mem->pui4_freelist=(UINT32*)
            x_mem_alloc(sizeof(UINT32)*(ui4_block_count+1));
        if ( pt_mem->pui4_freelist != NULL )
        {
            pt_mem->pui1_usedlist=(UINT8*)
                x_mem_alloc(sizeof(UINT8)*(ui4_block_count+1));
            if ( pt_mem->pui1_usedlist != NULL )
            {
                pt_mem->pui1_mem_area=(UINT8*)
                    x_mem_alloc(ui4_blocksize*ui4_block_count);
                if ( pt_mem->pui1_mem_area != NULL )
                {
                    _db_clean_memory_chunk(pt_mem,
                                           ui4_blocksize,
                                           ui4_block_count);
                    /*
                      Set the next chunk pointer to null.
                    */
                    pt_mem->pt_next_chunk = NULL;
                }
                else
                {
                    _db_delete_memory_chunk(pt_mem);
                    return DBR_OUT_OF_MEMORY;
                }
            }
            else
            {
                _db_delete_memory_chunk(pt_mem);
                return DBR_OUT_OF_MEMORY;
            }
        }
        else
        {
            _db_delete_memory_chunk(pt_mem);
            return DBR_OUT_OF_MEMORY;
        }
    }
    else
    {
        return DBR_FAIL;
    }
    /*
      Assign the memory region sturcture to the return argument.
    */
    *pt_mem_chunk = pt_mem;
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: _db_get_fb_from_memchunk
 *
 * Description: This API get the address of a free memory block from the
 *              specified memory chunk pointed to by 'pt_mem_chunk'.
 *
 * Inputs:  - pt_mem:       memory manager.
 *            pt_mem_chunk: specified the memory chunk to look for free block.
 *           
 * Outputs: -  ppui1_block: address of the free block found.
 *
 * Returns: - DBR_OK               Success.
 *            DBR_OUT_OF_MEMORY    No free block available.
 ----------------------------------------------------------------------------*/
static INT32 _db_get_fb_from_memchunk
(
    CDB_MEM_T*          pt_mem,
    CDB_MEM_CHUNK_T*    pt_mem_chunk,
    UINT8**             ppui1_block
)
{
    UINT32             free_block_index;
    
    if ( pt_mem_chunk->pui4_freelist[0] != DB_MEM_NULL_INDEX ) 
    {
        free_block_index = pt_mem_chunk->pui4_freelist[0];

        /* Debug code. */
#ifdef CDB_DEBUG
        if ( pt_mem_chunk->pui1_usedlist[free_block_index] !=
             DB_MEM_BLOCK_FREE )
        {
            /*
               This used flag for this block must be un-used.
            */
            ABORT(DBG_CAT_MEMORY,DBG_ABRT_MEM_WRONG_FREE_BLOCKS);
        }
#endif        
        /* Save the new next_free_block index. */
        pt_mem_chunk->pui4_freelist[0]=
            pt_mem_chunk->pui4_freelist[free_block_index];

        /* Increment the number of block used in this memory chunk. */
        pt_mem_chunk->ui4_used_count++;

        /* Set the used flag for the block. */
        pt_mem_chunk->pui1_usedlist[free_block_index]=DB_MEM_BLOCK_USED;

        /* Increment the total number of record */
        pt_mem->ui4_record_count++;
        
        /* compute the address of this free block. */
        *ppui1_block = pt_mem_chunk->pui1_mem_area +
            pt_mem->ui4_blocksize * ( free_block_index - 1 );
        return DBR_OK;
    }
    else
    {
        /* Debug code. */
#ifdef CDB_DEBUG
        /*
           When the memory chunk has no more free block left,
           then the number of block used in
           this memory chunk must be equal to the maximum number of
           the memory blocks allocated for a chunk.
        */        
        if ( pt_mem_chunk->ui4_used_count != pt_mem->ui4_block_count )
        {
            ABORT(DBG_CAT_MEMORY,DBG_ABRT_MEM_WRONG_FREE_BLOCKS);
        }
#endif
        *ppui1_block=NULL;
        return DBR_OUT_OF_MEMORY;
    }
}



/*-----------------------------------------------------------------------------
 * Name: _db_get_free_block_dynamic_mem
 *
 * Description: This API get the address of a free memory block with dynamic
 *              memory chunk policy. 
 *
 * Inputs:  - pt_mem:         memory manager.
 *            
 * Outputs: - ppui1_block:    address of the free block found.
 *
 * Returns: - DBR_OK               Success.
 *            DBR_OUT_OF_MEMORY    No free block available.
 ----------------------------------------------------------------------------*/
static INT32 _db_get_free_block_dynamic_mem
(
    CDB_MEM_T*          pt_mem,
    UINT8**             ppui1_block
)
{
    CDB_MEM_CHUNK_T*   pt_mem_chunk=NULL;
    CDB_MEM_CHUNK_T*   pt_last_mem_chunk=NULL;  

    /*
       First, look for free block in the memory chunk pointed to by free
       memory chunk pointer.
    */
    pt_mem_chunk=pt_mem->pt_free_chunk;

    if ( _db_get_fb_from_memchunk(pt_mem,
                                  pt_mem_chunk,
                                  ppui1_block) == DBR_OK )
    {
        return DBR_OK;
    }

    /*
      If no free block found in the current free chunk pointer, then
      look for free block in other memory chunks, start with the first
      memory chunk, and then the next, and next, etc.
    */
    pt_mem_chunk=pt_mem->pt_first_chunk;
    
    while ( pt_mem_chunk != NULL )
    {
        /*
          Save the last valid memory chunk address.
        */
        pt_last_mem_chunk=pt_mem_chunk;
        
        /*
          Look for free block in this memory chunk.  Skip if this chunk
          is the pt_free_chunk, because we have search this memory
          block already.
        */
        if ( pt_mem_chunk != pt_mem->pt_free_chunk   &&
             _db_get_fb_from_memchunk(pt_mem,
                                      pt_mem_chunk,
                                      ppui1_block) == DBR_OK )
        {
            return DBR_OK;
        }
        
        /* Go to the next chunk. */
        pt_mem_chunk=pt_mem_chunk->pt_next_chunk;
    }
    
    /*
      If no free block can be found in any of the memory chunk, then
      we need to allocate a new memory chunk.
    */
#ifdef CDB_DEBUG
    /* Debug check:

    When all the memory chunks are used in DYNAMIC memory usage
    mode, then the total number of the records stored must equal
    to the maximum number of records per chunk * number of chunk, 
    */
    if ( pt_mem->ui4_record_count != ( pt_mem->ui2_chunk_count *
                                       pt_mem->ui4_block_count ) )
    {        
        ABORT(DBG_CAT_MEMORY,DBG_ABRT_MEM_MANAGER);   
    }

#endif                
    /* allocate a new
       memory chunk. */
    _db_create_memory_chunk(&(pt_mem_chunk),
                            pt_mem->ui4_blocksize,
                            pt_mem->ui4_block_count
        );
    if ( pt_mem_chunk != NULL )
    {
        /*
          Increment the memory chunk count.
        */
        pt_mem->ui2_chunk_count++;
        
        /*
          Attach the new memory chunk to the
          last memory chunk
        */
        pt_last_mem_chunk->pt_next_chunk=pt_mem_chunk;

        /*
          Set the pt_free_chunk pointer to the newly allocated
          memory chunk. */
        pt_mem->pt_free_chunk=pt_mem_chunk;
        
        if ( _db_get_fb_from_memchunk(pt_mem,
                                      pt_mem_chunk,
                                      ppui1_block) == DBR_OK )
        {
            return DBR_OK;
        }
        else
        {
            /* There should be free block in the memory chunk
               just create, therefore we should never get here. */
             ABORT(DBG_CAT_MEMORY,DBG_ABRT_MEM_MANAGER);
        }
    }
    else
    {
        /* Can not allocate any more memory chunk, for the
           DYNAMIC memory policy, we will return error
           status.
        */
        *ppui1_block=NULL;
        return DBR_OUT_OF_MEMORY;
    }
    return DBR_OUT_OF_MEMORY;
}

/*-----------------------------------------------------------------------------
 * Name: db_memory_manager_init
 *
 * Description: This API initialize the memory manager and create a
 *              internal memory to store the record.
 *              A memory buffer consists of a number of
 *              equally sized blocks. Each block store a record of
 *              specified size. The maximum number of blocks allowed is
 *              2**32-1 (4294967295L).
 *
 *             
 * Inputs:  - ui4_blocksize:    Width of a record block
 *            ui4_block_count:  Number of the record blocks per memory
 *                              chunk.                             
 *            e_memory_policy:  Specifies the whether the number of
 *                              memory chunk can grow.  
 *
 * Outputs: - pt_mem_manager:   pointer to the memory manager of this 
 *                              memory buffer.
 *
 * Returns: - 0     Success.
 *            -1    Sort failed.
 ----------------------------------------------------------------------------*/
INT32 db_memory_manager_init
(
    CDB_MEM_T**     pt_mem_manager,
    UINT32          ui4_blocksize,
    UINT32          ui4_block_count,
    DB_MEM_POLICY_T e_memory_policy
)
{
    CDB_MEM_T* pt_mem;

    *pt_mem_manager=NULL;
    if ( ui4_blocksize != 0 && ui4_block_count != 0 )
    {
        /*
           allocate memory for the memory manager.
        */
        pt_mem=(CDB_MEM_T*)
            x_mem_alloc(sizeof(CDB_MEM_T));
        if ( pt_mem != NULL )
        {
            pt_mem->ui4_blocksize = ui4_blocksize;
            pt_mem->ui4_block_count = ui4_block_count;
            pt_mem->ui4_record_count = 0;
            pt_mem->ui2_chunk_count = 0;
            pt_mem->e_memory_policy = e_memory_policy;
            pt_mem->pt_first_chunk = NULL;
            pt_mem->pt_free_chunk = NULL;
            
            /* Create first memory chunk. */
            _db_create_memory_chunk(&(pt_mem->pt_first_chunk),
                                    ui4_blocksize,
                                    ui4_block_count
                                   );
            if ( pt_mem->pt_first_chunk != NULL )
            {
                pt_mem->pt_free_chunk = pt_mem->pt_first_chunk;
                pt_mem->ui2_chunk_count = 1;
                *pt_mem_manager=pt_mem;
            }
            else
            {
                db_delete_memory_manager(pt_mem);
                return DBR_OUT_OF_MEMORY ;
            }
        }
        else
        {
            return DBR_OUT_OF_MEMORY ;
        }    
    }
    else
    {
        return DBR_FAIL;
    }
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: db_get_next_freeblock
 *
 * Description: This API get the address of next free memory block.
 *              With the static memory chunk policy, if the memory chunk
 *              pointed by the pt_free_chunk contains no more free block,
 *              then we return with DBR_OUT_OF_MEMORY.
 *
 * Inputs:  - pt_mem:         memory manager.
 *            ppui1_block:    variable to store the address of the free block found.
 *
 * Outputs: - Sorted listed
 *
 * Returns: - 0     Success.
 *            -1    Sort failed.
 ----------------------------------------------------------------------------*/
INT32 db_get_next_freeblock
(
    CDB_MEM_T*          pt_mem,
    UINT8**             ppui1_block
)
{
    CDB_MEM_CHUNK_T*    pt_mem_chunk;
    
    if ( pt_mem->e_memory_policy == DB_MEM_STATIC )
    {
        /*
          For the static memory policy, there is only one memory chunk,
          It is always the first chunk.
        */
        pt_mem_chunk = pt_mem->pt_first_chunk;
        return _db_get_fb_from_memchunk(pt_mem,
                                        pt_mem_chunk,
                                        ppui1_block);
    }
    else
    {
        /*
          Dynamic memory policy.
        */
        return _db_get_free_block_dynamic_mem(pt_mem, ppui1_block);
    }    
}

/*-----------------------------------------------------------------------------
 * Name: db_get_used_list
 *
 * Description: This API get the addresses of all the used memory blocks. 
 *
 * Inputs:  - pt_mem:         memory manager.
 *            paui1_block:    variable to store the addresses of the all
 *                            the used blocks found.
 *
 * Outputs: - used block addreses
 *
 * Returns: - 0     Success.
 *            -1    Sort failed.
 ----------------------------------------------------------------------------*/
INT32 db_get_used_list
(
    CDB_MEM_T*          pt_mem,
    UINT32              ui4_record_count, 
    UINT8*              paui1_block[]
)
{
    INT32            i4_i;
    INT32            i4_counter;
    UINT8*           pui1_addr;
    CDB_MEM_CHUNK_T* pt_chunk;

    i4_counter = 0;
    if ( (pt_mem != NULL) && (ui4_record_count == pt_mem->ui4_record_count) )
    {
        pt_chunk=pt_mem->pt_first_chunk;
        while ( pt_chunk != NULL ) 
        {
            pui1_addr = pt_chunk->pui1_mem_area;
            
            for ( i4_i = 1 ; i4_i <= (INT32)(pt_mem->ui4_block_count) ; i4_i++ )
            {
                if ( pt_chunk->pui1_usedlist[i4_i] == DB_MEM_BLOCK_USED )
                {
                    paui1_block[i4_counter] = pui1_addr +
                        (i4_i-1)*pt_mem->ui4_blocksize ;
                    i4_counter++;
                }
            }
            pt_chunk=pt_chunk->pt_next_chunk;
        }
        /*
          Assertion check: the number of used blocks found must
          equal to the number of records
        */
        if ( i4_counter != ((INT32)ui4_record_count) )
        {
            ABORT(DBG_CAT_MEMORY,DBG_ABRT_INVALID_RECORD_COUNT);
        }
        return DBR_OK;
    }
    return DBR_FAIL;
} 


/*-----------------------------------------------------------------------------
 * Name: db_get_next_used_block
 *
 * Description: This API get the next used block address from the memory
 *              buffer.
 *
 * Inputs:  - pt_mem:         memory manager.
 *            pt_mem_cursor:  cursor to the location in the memory buffer
 *                            where the search is started.
 *                            
 *
 * Outputs: - pt_mem_cursor:  cursor is set to the location where
 *                            the used record is found.
 *            ppui1_record:   variable to store the addresses of the
 *                            used block found.
 *
 * Returns: - DBR_OK    Success.
 *            DBR_FAIL  No more record left.
 ----------------------------------------------------------------------------*/
INT32 db_get_next_used_block
(
    CDB_MEM_T*          pt_mem,
    CDB_MEM_CURSOR_T*   pt_mem_cursor,
    UINT8**             ppui1_record
)
{
    INT32            i4_i;
    UINT8*           pui1_addr;
    CDB_MEM_CHUNK_T* pt_chunk;

    *ppui1_record = NULL;
    
    pt_chunk=pt_mem_cursor->pt_current_chunk;
    while ( pt_chunk != NULL )
    {
        pui1_addr = pt_chunk->pui1_mem_area;

        /*
          Only search this chunk if there are used blocks
        */
        if ( pt_chunk->ui4_used_count > 0 )
        {    
            for ( i4_i = ((INT32)(pt_mem_cursor->ui4_current_index+1)) ;
                  i4_i <= ((INT32)pt_mem->ui4_block_count) ;
                  i4_i++ )
            {
                if ( pt_chunk->pui1_usedlist[i4_i] == DB_MEM_BLOCK_USED )
                {
                    (*ppui1_record) = pui1_addr +
                        (i4_i-1)*pt_mem->ui4_blocksize ;

                    /* Update the cursor to point to this record entry. */
                    pt_mem_cursor->pt_current_chunk=pt_chunk;
                    pt_mem_cursor->ui4_current_index= (UINT32) i4_i;
                
                    return DBR_OK; 
                }
            }
        }
        /*
          Go to the next chunk.
        */
        pt_chunk=pt_chunk->pt_next_chunk;
        if ( pt_chunk != NULL )
        {
            /*
              Update the current chunk pointer.
            */
            pt_mem_cursor->pt_current_chunk=pt_chunk;
            /*
              Reset the starting index to the beginning.
            */
            pt_mem_cursor->ui4_current_index=DB_MEM_NULL_INDEX;
        }
    }
    return DBR_FAIL;
}

/*-----------------------------------------------------------------------------
 * Name: db_set_cursor_start
 *
 * Description: This API set the cursor to the beginning of the memory buffer. 
 *
 * Inputs:  - pt_mem:         memory manager.
 *            pt_mem_cursor:  memory cursor that holds the iteration
 *                            state information.
 *
 * Outputs: - used block addreses
 *
 * Returns: - 0     Success.
 *            -1    Sort failed.
 ----------------------------------------------------------------------------*/
INT32 db_set_cursor_start
(
    CDB_MEM_T*          pt_mem,
    CDB_MEM_CURSOR_T*   pt_mem_cursor
)
{
    pt_mem_cursor->pt_current_chunk=pt_mem->pt_first_chunk;
    pt_mem_cursor->ui4_current_index=DB_MEM_NULL_INDEX;
    return DBR_OK;
} 


/*-----------------------------------------------------------------------------
 * Name: db_free_memory_block
 *
 * Description: This API get free up the used block address from the memory
 *              buffer.
 *
 * Inputs:  - pt_mem:         memory manager.
 *            pui1_block:     addresses of the used blocks to be freed.
 *
 * Outputs: - used block addreses
 *
 * Returns: - 0     Success.
 *            -1    Sort failed.
 ----------------------------------------------------------------------------*/
INT32 db_free_memory_block
(
    CDB_MEM_T*          pt_mem,
    CDB_MEM_CURSOR_T*   pt_mem_cursor,
    UINT8*              pui1_record
)
{
    CDB_MEM_CHUNK_T* pt_chunk;
    UINT32           ui4_block_index;
    
    pt_chunk=pt_mem_cursor->pt_current_chunk;

    /*
      Compute the index for this block from its
      memory address.
    */
    ui4_block_index = ( (pui1_record - pt_chunk->pui1_mem_area) /
                        pt_mem->ui4_blocksize ) + 1;

    /* Set the next free memory chunk to this memory chunk. */
    pt_mem->pt_free_chunk = pt_chunk;

    /* Save the next free block index */
    pt_chunk->pui4_freelist[ui4_block_index]=
        pt_chunk->pui4_freelist[0];
    
    /* The free block index of the memory chunk
       points to this new freed block. */
    pt_chunk->pui4_freelist[0] = ui4_block_index;

    /* Make this new block as free. */
    pt_chunk->pui1_usedlist[ui4_block_index]=DB_MEM_BLOCK_FREE;
    
    /* Decrement the number of used block in this chunk */
    pt_chunk->ui4_used_count--;

    /* Decrement the total number of record block used. */
    pt_mem->ui4_record_count--;

    /* Null out the memory block */
    x_memset(pui1_record,0,
             pt_mem->ui4_blocksize);
    
    return DBR_OK;
}

/*-----------------------------------------------------------------------------
 * Name: db_copy_mem_obj
 *
 * Description: This API copies all record entries from one database object 
 *      to second database object.  Note: the existing records in the
 *      destination data object are lost.
 *
 *      For example: if the 'from' object contains 0 records, then
 *      it means all the 'to' object's records will be zero'd out.
 *.
 * Inputs:  - 
 *    pt_do_from:  reference to the source database object.
 *
 * Outputs: -
 *    pt_do_to:    reference to the destination data object.
 *    pui4_rec_copied  number of records copied
 *
 * Returns: - DBR_OK:     record is found.   
 *            DBR_FAIL:   reach end of the memory buffer, no more
 *                        record match.
 *            
 ----------------------------------------------------------------------------*/
INT32 db_copy_mem_obj
(
    CDB_MEM_T*          pt_from,
    CDB_MEM_T*          pt_to,
    UINT32*             pui4_rec_copied
)
{
    UINT8*              pui1_block_from;
    UINT8*              pui1_block_to;
    CDB_MEM_CURSOR_T    t_mem_cursor_from;
    INT32               i4_rc = DBR_OK;
    
    *pui4_rec_copied = 0 ;
    
    /*
       Evaluate if two memory block objects are compatible so
       the copy operation can continue.
    */
    if ( pt_from->ui4_blocksize != pt_to->ui4_blocksize )
    {
        return DBR_FAIL;
    }

    /* remove all record from 'destination' object. */
    db_remove_all_records(pt_to);
        
    /* set the pointer to the first record. */
    db_set_cursor_start(pt_from, &t_mem_cursor_from);

    while ( db_get_next_used_block(pt_from,
                                   &t_mem_cursor_from,
                                   &pui1_block_from)
            == DBR_OK )
    {
        if ( (i4_rc=db_get_next_freeblock(pt_to, &pui1_block_to))
             == DBR_OK )
        {
            x_memcpy(pui1_block_to,
                     pui1_block_from,
                     pt_to->ui4_blocksize);
        }
        else
        {
            break;
        }
    }

    *pui4_rec_copied = pt_to->ui4_record_count ;
    
    return i4_rc;
}
