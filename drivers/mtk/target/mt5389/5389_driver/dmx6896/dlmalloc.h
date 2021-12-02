#ifndef _DLMALLOC_H_
#define _DLMALLOC_H_

//==========================================================================
//
//      dlmalloc.h
//
//      Interface to the port of Doug Lea's malloc implementation
//
//==========================================================================

// init memory pool
// return true on success
extern int
dlmem_init(void* base, int size);

// get some memory, return NULL if none available
extern void*
dlmalloc(int size);

extern void*
dlcalloc(int nmemb, int size);

// resize existing allocation, if oldsize is non-NULL, previous
// allocation size is placed into it. If previous size not available,
// it is set to 0. NB previous allocation size may have been rounded up.
// Occasionally the allocation can be adjusted *backwards* as well as,
// or instead of forwards, therefore the address of the resized
// allocation is returned, or NULL if no resizing was possible.
// Note that this differs from ::realloc() in that no attempt is
// made to call malloc() if resizing is not possible - that is left
// to higher layers. The data is copied from old to new though.
// The effects of alloc_ptr==NULL or newsize==0 are undefined
extern void*
dlresize_alloc(void* alloc_ptr, int newsize, int* oldsize);

extern void*
dlrealloc(void* ptr, int size);

// free the memory back to the pool
// returns true on success
extern int
dlfree(void* ptr);

typedef struct
{
    const unsigned char *arenabase;     // base address of entire pool
    int   arenasize;                    // total size of entire pool
    int   freeblocks;                   // number of chunks free for use
    int   totalallocated;               // total allocated space in bytes
    int   totalfree;                    // total space in bytes not in use
    int   blocksize;                    // block size if fixed block
    int   maxfree;                      // size of largest unused block
    char  waiting;                      // are there any threads waiting for memory?
    const unsigned char *origbase;      // address of original region used when pool
                                        // created
    int   origsize;                     // size of original region used when pool
                                        // created

    // maxoverhead is the *maximum* per-allocation overhead imposed by
    // the allocator implementation. Note: this is rarely the typical
    // overhead which often depends on the size of the allocation requested.
    // It includes overhead due to alignment constraints. For example, if
    // maxfree and maxoverhead are available for this allocator, then an
    // allocation request of (maxfree-maxoverhead) bytes must always succeed
    // Unless maxoverhead is set to -1 of course, in which case the allocator
    // does not support reporting this information.
    char    maxoverhead;
} mempool_status;

// Flags to pass to mem_get_status() methods to tell it which stat(s) is/are
// being requested

#define MEMPOOL_STAT_ARENABASE       (1<<0)
#define MEMPOOL_STAT_ARENASIZE       (1<<1)
#define MEMPOOL_STAT_FREEBLOCKS      (1<<2)
#define MEMPOOL_STAT_TOTALALLOCATED  (1<<3)
#define MEMPOOL_STAT_TOTALFREE       (1<<4)
#define MEMPOOL_STAT_BLOCKSIZE       (1<<5)
#define MEMPOOL_STAT_MAXFREE         (1<<6)
#define MEMPOOL_STAT_WAITING         (1<<7)
#define MEMPOOL_STAT_ORIGBASE        (1<<9)
#define MEMPOOL_STAT_ORIGSIZE        (1<<10)
#define MEMPOOL_STAT_MAXOVERHEAD     (1<<11)
#define MEMPOOL_STAT_ALL             0xfff

// Get memory pool status
// flags is a bitmask of requested fields to fill in. The flags are
// defined in common.hxx
extern void
dlmem_get_status(unsigned int flags, mempool_status* status);

/*

#include <stdlib.h>

extern struct mallinfo
mallinfo(void);

*/

#endif // ifndef _DLMALLOC_H_

// EOF dlmalloc.h
