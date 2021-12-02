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

/*
 * Build a small and short-life memory allocator.
 * For that user allocate blocks and free it immediatly.
 * 
 *
 *
 * */

#include "stdio.h"
#include "stdlib.h"
#include "assert.h"
#include <errno.h>
#include <semaphore.h>

#include "os/inc/x_os.h"
#ifdef __SSMA_TRACE__
#define SSMA_TRACE      printf
#else 
#define SSMA_TRACE(x, ...)    
#endif
#define SSMA_ERR        printf

#define MAX_BARREL      8 
#define PWR_TO_IDX(pwr) ((unsigned int)((unsigned int)(pwr) - 3))

#define MAX_MEM_SIZE    ((size_t)(barrel_asize[MAX_BARREL - 1] - sizeof(barrel_node_t)))
#define MIN_MEM_SIZE    ((size_t)0)


#define FETCH_COUNT     ((unsigned int)3)

/* 
 * This magic number could help us to find 
 * some memory problems like overwriten and double freeing.
 */
#define MAGIC_NUMBER_FREE       ((unsigned int)0xABCDEFFE)
#define MAGIC_NUMBER_INUSE      ((unsigned int)0x12345678)

static size_t barrel_asize[MAX_BARREL] = {
    8,
    16,
    32,
    64,
    128,
    256,
    512,
    1024
};


typedef struct _barrel_node_t
{
    int    magic_number;
    void * pnext;
    size_t asize;
}barrel_node_t;



static barrel_node_t *  p_barrel_free[MAX_BARREL];

static void * p_buff_start;
static void * p_buff_free;
size_t        buff_size;

static unsigned int inited = 0;



/*
 * Get the power of size and translate it into barrel index.
 * Using ARMv5 instruct "clz". It is fast.
 *
 * */
static size_t 
_get_idx(size_t size)
{
    register unsigned rn;
    register unsigned rd;
    size_t   t_idx;
    /*
     * Value like 100000 is to be 100000.
     * */
    rn = size - 1;

    __asm__ __volatile__ ("clz %0,%1":"=r"(rd):"r"(rn));
    
    t_idx = PWR_TO_IDX(32 - rd);
    if ( t_idx < MAX_BARREL )
    {
        return t_idx;
    }
    else
    {
        SSMA_ERR( "\n{SSMA Critical Err<File=%s,Line=%d>:t_idx=%u,size=%u}\n",
                  __FILE__, __LINE__, t_idx, size );
        return 0;
    }
    
    /*return (PWR_TO_IDX(32 - rd));*/
    
    /*int pwr;
   
    if(size <= 64)
    {
        if(size <= 16)
        {
            pwr = (size <= 8)?3:4;            
        }
        else
        {
            pwr = (size <= 32)?5:6;            
        }
    }
    else
    {
        if(size <= 256)
        {
            pwr = (size <= 128)?7:8;
        }
        else
        {
            pwr = (size <= 512)?9:10;
        }
    }

    return PWR_TO_IDX(pwr);*/

}


/*Required lock*/

static void * lock;
static void
_ssma_lock()
{
    int ret;
    do {
        /* man page says sem_wait() always returns 0 */
        /* it is not true. so we still have to check errno */
        ret = sem_wait((sem_t *) lock);

        if ((ret < 0) && (errno == EINTR))
        {
            continue;
        }
        else
        {
            break;
        }
    } while (TRUE);
    return ;
}

static void
_ssma_unlock()
{    
    sem_post((sem_t *)lock);
    return ;
}

static int
_ssma_lock_init()
{
    unsigned int val;
    int ret;
    lock = (sem_t *) malloc(sizeof(sem_t));
    if (lock == (sem_t *) NULL)
    {
        return(-1);
    }

    val = 1;

    ret = sem_init((sem_t *)lock, 0, val);
    if(ret == 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

static void
_dbg_dump()
{
    int i;
    SSMA_ERR("\n\n|*-SSMA-DUMP-------------------------\n");
    SSMA_ERR("|* Buff start:0x%08x \n|* Buff free:0x%08x\n|* Used size:%d \n |* Free size:%d\n",
            (unsigned int)p_buff_start,
            (unsigned int)p_buff_free,
            (int)((char *)p_buff_free - (char *)p_buff_start),
            (int)buff_size - (int)((char *)p_buff_free - (char *)p_buff_start));

    for(i = 0; i < MAX_BARREL; i ++)
    {
        SSMA_ERR("|* BARREL[%d] free head : 0x%08x\n", i, (unsigned int)(p_barrel_free[i]));

    }
    SSMA_ERR("|*----------------------------------\n");
}


static void *
_fetch_from_buff( unsigned int br_idx)
{
    int i;
    size_t asize;

    asize = barrel_asize[br_idx];

    /*Get FETCH_COUNT items linked from buff at most.
     *
     * 
     */
    for(i = 0; i < FETCH_COUNT; i ++)
    {
        if((char *)p_buff_free + asize - (char *)p_buff_start <= buff_size)
        { 
       
            ((barrel_node_t *)p_buff_free)->magic_number = MAGIC_NUMBER_FREE;
            ((barrel_node_t *)p_buff_free)->pnext = p_barrel_free[br_idx];
            ((barrel_node_t *)p_buff_free)->asize = asize;

            p_barrel_free[br_idx] = p_buff_free;

            p_buff_free = (char *)p_buff_free + asize;
        }
        else
        {
            break;
        }        
    }

    SSMA_TRACE("\n[SSMA]Move the buff free :0x%08x\n", (UINT32)p_buff_free);

    return p_barrel_free[br_idx];
}


static void
_dump_mem(void *p, size_t left, size_t right)
{
    int * pi = p;

    size_t i;
    
    SSMA_ERR("|* SSMA Memory dump:\n");
    pi = ((int *)p) - left;
    for(i = 0; i < right + left; i ++)
    {
        if(i == left)
        {
            SSMA_TRACE(" %08x \n", *pi);
        }
        else
        {
            SSMA_TRACE("[%08x]\n", *pi);
        
        }
        
        pi ++;
    }
    SSMA_ERR("|* ...\n");
}

void *
ssma_alloc(size_t size)
{
    unsigned int br_idx;
    size_t asize;   

    void * paddr;
    void * p;

    assert(inited == 1);

    SSMA_TRACE("[SSMA]Request to alloc size:%d\n", size);
    
    /* Add the size of node, and aligned to 4 bytes */
    size = ((size + sizeof(barrel_node_t)) + 0x4) & ~0x3;

    if(size > MAX_MEM_SIZE || size < MIN_MEM_SIZE)
    {
        SSMA_ERR("[SSMA ERR]Alloc size out of bounds %d, Caller 0x%08x\n",
                 size, (unsigned int)(__builtin_return_address(0)));
        return NULL;
    }

    br_idx = _get_idx(size);

    asize = barrel_asize[br_idx];    
      

    SSMA_TRACE("[SSMA]Alloc aligned size: %d , ajust size %d and barrel idx:%d\n\n", size, asize, br_idx);

    _ssma_lock();

    if(p_barrel_free[br_idx] == NULL)
    {
        if(_fetch_from_buff(br_idx) == NULL)
        {
            /*
             * If there is no memory in the buff, 
             * I don't want to do spliting from the br_idx + x. 
             * Just fail.
             * */
            SSMA_ERR("[SSMA]Out of memory!!!\n");
            _dbg_dump();
            _ssma_unlock();
            return NULL;        
        }
    }

    assert(p_barrel_free[br_idx] != NULL);

    paddr = p_barrel_free[br_idx];

    if(((barrel_node_t *)paddr)->magic_number != MAGIC_NUMBER_FREE)
    {
        if(((barrel_node_t *)paddr)->magic_number != MAGIC_NUMBER_INUSE)
        {
            _dbg_dump();
            _dump_mem(paddr, 10, 10);
            _ssma_unlock();

            SSMA_ERR("[SSMA ERR] Alloc: Memory overwritten? Address:0x%08x\n", (unsigned int)paddr);
            /*return NULL;*/
        }
        else
        {   _dbg_dump();
            _ssma_unlock();        
            SSMA_ERR("[SSMA ERR] SSMA BUG, why does it require an in-use memory ,p:0x%08x, asize :%d\n", (unsigned int)paddr, asize);
            /*return NULL;*/
        }
    }
    
/*_LB_OK:*/

    p_barrel_free[br_idx] = p_barrel_free[br_idx]->pnext;

    ((barrel_node_t *)paddr)->asize = asize;
    ((barrel_node_t *)paddr)->magic_number = MAGIC_NUMBER_INUSE;

    p = (void *)((char *)paddr + sizeof(barrel_node_t));

    _ssma_unlock();

    SSMA_TRACE("[SSMA]Allocated sucessful: [ %x - %x ], Caller:0x%08x\n", paddr, (char *)paddr + asize, (UINT32)(__builtin_return_address(0)));
    
    /*For test validate??*/
    /*memset(p, 0, size);*/
    return p;

}

void
ssma_free(void * p)
{
    void * paddr;
    size_t asize;
    /*unsigned int pwr;*/
    unsigned int br_idx;
   
    assert(inited == 1);

    SSMA_TRACE("[SSMA]Request to free :%x\n", p);

    if(!((char *)p >= (char *)p_buff_start && (char *)p < (char *)p_buff_free))
    {
        SSMA_ERR("[SSMA ERR]Free an invalid pointer : 0x%08x is not in [ 0x%08x, 0x%08x ], caller:0x%08x\n", 
                (unsigned int)p, 
                (unsigned int)p_buff_start, 
                (unsigned int)p_buff_free,            
                (unsigned int)(__builtin_return_address(0)));
        assert(0);
    }

    paddr = (void *)((char *)p - sizeof(barrel_node_t));

    _ssma_lock();

    if(((barrel_node_t *)paddr)->magic_number != MAGIC_NUMBER_INUSE)
    {
        if(((barrel_node_t *)paddr)->magic_number != MAGIC_NUMBER_FREE)
        {
            _ssma_unlock();
            SSMA_ERR("[SSMA ERR] Free:Memory overwritten?, Address:0x%08x\n", (unsigned int)p);
            return;
        }
        _ssma_unlock();    
        SSMA_ERR("[SSMA ERR] Memory double free? Caller 0x%08x\n", (unsigned int)(__builtin_return_address(0)));
        return;
    }

    asize = ((barrel_node_t *)paddr)->asize;

    br_idx = _get_idx(asize);

    SSMA_TRACE("[SSMA]Free ajust size %d and barrel idx:%d, address:0x%08x\n\n", asize, br_idx, (UINT32)paddr);

    ((barrel_node_t *)paddr)->pnext = p_barrel_free[br_idx];
    p_barrel_free[br_idx] = paddr;

    ((barrel_node_t *)paddr)->magic_number = MAGIC_NUMBER_FREE;

    _ssma_unlock();

}


void 
ssma_init(void * p_start_addr, size_t total_size)
{
    unsigned int i;
    /*unsigned int j;

    char * paddr;
    size_t asize;*/

    if(inited)
    {
        return;
    }
    _ssma_lock_init();
    
    _ssma_lock();
    
    /*
     * Init the buff free and buff start.
     * */
    p_buff_start 
        = p_buff_free 
            = p_start_addr;
    
    x_memset(p_start_addr, 0, total_size);

    buff_size = total_size;

    /*Init each barrel free list*/
    for(i = 0; i < MAX_BARREL; i ++)
    {        
        p_barrel_free[i] = NULL;
    }

    _ssma_unlock();

    inited = 1;

    SSMA_TRACE("[SSMA]SSMA inited : [ 0x%08x - 0x%08x ]\n", (UINT32)p_start_addr, (UINT32)((char*)p_start_addr + total_size));
}

/*int main(void)
{
    void * p;
    void * pa;
    unsigned int cmd;
    unsigned int para;

    p = malloc(1024 * 1024);

    ssma_init(p, 1024 * 1024);
    SSMA_TRACE("Start addr: %x\n", p);
    while(1)
    {        
        scanf("%x%x", &cmd, &para);
        if(cmd == 0)
        {
            SSMA_TRACE("\n---Alloc :[  %x  ]\n", ssma_alloc(para));
        }
        else
        {
            SSMA_TRACE("\n---Free  :[  %x  ]\n", para);
            ssma_free(para);
        }
    }

}*/
