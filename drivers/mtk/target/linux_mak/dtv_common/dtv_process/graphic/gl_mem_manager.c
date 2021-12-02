
#include "graphic/u_gl.h"
#include "graphic/gl.h"
#include "gl_mem_manager.h"
#include "x_os.h"
#include "util/u_lnk_list.h"
#include "util/lnklist.h"


HANDLE_T h_gl_mem_pool_lock = NULL_HANDLE;

#define GL_MEM_BUFFER_SHINK  10240
DLIST_T(_MTAL_GL_MEM_POOL) gt_mem_pool;
static BOOL b_mempoolinited = FALSE;

UINT32 _ui4_mem_pool_size = 0;


GL_MEM_POOL* gl_mem_manager_alloc(UINT32 size)
{
	
	GL_MEM_POOL* pool_entry = DLIST_HEAD(&gt_mem_pool);
	GL_MEM_POOL* pool_new_entry = NULL;

	if (size > _ui4_mem_pool_size)
	{
		return NULL;
	}

	x_sema_lock(h_gl_mem_pool_lock,X_SEMA_OPTION_WAIT);
	
	while(pool_entry)
	{
		if(pool_entry->b_alloced)
		{
			pool_entry = DLIST_NEXT(pool_entry,t_gl_mem_pool_entry);
			continue;
		}
		else if (pool_entry->bmp_buffer.u4Size < size)
		{
			pool_entry = DLIST_NEXT(pool_entry,t_gl_mem_pool_entry);
			continue;
		}
		else if(pool_entry->bmp_buffer.u4Size >= size && pool_entry->bmp_buffer.u4Size <= size + GL_MEM_BUFFER_SHINK)
		{
			pool_entry->b_alloced = TRUE;
			_ui4_mem_pool_size -= pool_entry->bmp_buffer.u4Size;
			x_sema_unlock(h_gl_mem_pool_lock);
			return pool_entry;				
		}
		else if (pool_entry->bmp_buffer.u4Size > size)
		{
			//DLIST_REMOVE(pool_entry,&gt_mem_pool,t_gl_mem_pool_entry);
			pool_new_entry = (GL_MEM_POOL*)x_mem_alloc(sizeof(GL_MEM_POOL));

			pool_new_entry->b_alloced = FALSE;
			pool_entry->b_alloced     = TRUE;
			
			pool_new_entry->bmp_buffer.u4Size = pool_entry->bmp_buffer.u4Size - size;
			pool_entry->bmp_buffer.u4Size     = size;

			pool_new_entry->bmp_buffer.u4Addr     = pool_entry->bmp_buffer.u4Addr + size;
			pool_new_entry->bmp_buffer.u4PhyAddr  = pool_entry->bmp_buffer.u4PhyAddr + size;
			pool_new_entry->bmp_buffer.u4VirtAddr = pool_entry->bmp_buffer.u4VirtAddr + size;

			x_sema_create(&(pool_new_entry->h_entry_lock), X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);

			DLIST_INSERT_BEFORE(pool_new_entry, pool_entry,&gt_mem_pool,t_gl_mem_pool_entry);
			
			_ui4_mem_pool_size -= size;
			
			x_sema_unlock(h_gl_mem_pool_lock);
			return pool_entry;
		}
		else
		{
			pool_entry = DLIST_NEXT(pool_entry,t_gl_mem_pool_entry);
			continue;
		}
	}
	x_sema_unlock(h_gl_mem_pool_lock);
	return NULL;
}

VOID gl_mem_manager_free(GL_MEM_POOL* entry)
{
	GL_MEM_POOL* prev_entry = NULL;
	GL_MEM_POOL* next_entry = NULL;
	
	if(entry == NULL || entry->b_alloced == FALSE)
	{
		return;
	}

	x_sema_lock(h_gl_mem_pool_lock,X_SEMA_OPTION_WAIT);
	
	prev_entry = DLIST_PREV(entry,t_gl_mem_pool_entry);
	next_entry = DLIST_NEXT(entry,t_gl_mem_pool_entry);
	
	_ui4_mem_pool_size += entry->bmp_buffer.u4Size;
	
	if (next_entry != NULL && next_entry->b_alloced == FALSE)
	{
		DLIST_REMOVE(next_entry,&gt_mem_pool,t_gl_mem_pool_entry);
		entry->bmp_buffer.u4Size += next_entry->bmp_buffer.u4Size;
		x_sema_delete(next_entry->h_entry_lock);
		x_mem_free(next_entry);
	}
	if (prev_entry != NULL && prev_entry->b_alloced == FALSE)
	{
		DLIST_REMOVE(entry,&gt_mem_pool,t_gl_mem_pool_entry);
		prev_entry->bmp_buffer.u4Addr     = entry->bmp_buffer.u4Addr;
		prev_entry->bmp_buffer.u4PhyAddr  = entry->bmp_buffer.u4PhyAddr;
		prev_entry->bmp_buffer.u4VirtAddr = entry->bmp_buffer.u4VirtAddr;
		prev_entry->bmp_buffer.u4Size += entry->bmp_buffer.u4Size;
		x_sema_delete(entry->h_entry_lock);
		x_mem_free(entry);
	}
	else
	{
		entry->b_alloced = FALSE;
	}
	
	x_sema_unlock(h_gl_mem_pool_lock);
	
	return;
}

INT32 gl_mem_manager_init(GL_POOL_BUF_T* buffer)
{
	if (!b_mempoolinited)
    {
    	GL_MEM_POOL* entry = NULL;
	    DLIST_INIT(&(gt_mem_pool));

	    x_sema_create(&h_gl_mem_pool_lock, X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
	    if (h_gl_mem_pool_lock == NULL_HANDLE)
	    {
		    return GLR_OUT_OF_HANDLES;
	    }

		entry = (GL_MEM_POOL*)x_mem_alloc(sizeof(GL_MEM_POOL));
		x_memcpy(&(entry->bmp_buffer), buffer, sizeof(GL_POOL_BUF_T));
		entry->b_alloced = FALSE;
		
		x_sema_create(&(entry->h_entry_lock), X_SEMA_TYPE_BINARY, X_SEMA_STATE_UNLOCK);
	    if ((entry->h_entry_lock) == NULL_HANDLE)
	    {
		    return GLR_OUT_OF_HANDLES;
	    }
		
		DLIST_INSERT_HEAD(entry,&gt_mem_pool,t_gl_mem_pool_entry);
		_ui4_mem_pool_size = buffer->u4Size;
		
	    h_gl_mem_pool_lock = TRUE;
    }
	return GLR_OK;
}

INT32 gl_mem_manager_deinit(VOID)
{
	if (b_mempoolinited)
	{
		GL_MEM_POOL* entry = DLIST_HEAD(&gt_mem_pool);
		
		x_sema_lock(h_gl_mem_pool_lock,X_SEMA_OPTION_WAIT);
		
		while(entry)
		{
			DLIST_REMOVE(entry,&gt_mem_pool,t_gl_mem_pool_entry);
			x_sema_delete(entry->h_entry_lock);
			x_mem_free(entry);
			entry = DLIST_HEAD(&gt_mem_pool);
		}
		_ui4_mem_pool_size = 0;
		x_sema_unlock(h_gl_mem_pool_lock);

		x_sema_delete(h_gl_mem_pool_lock);
	}
	return GLR_OK;
}

