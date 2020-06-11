/* linux/mm/bigphysarea.h, M. Welsh (mdw@cs.cornell.edu)
 * Copyright (c) 1996 by Matt Welsh.
 * Extended by Roger Butenuth (butenuth@uni-paderborn.de), October 1997
 * Extended for linux-2.1.121 till 2.4.0 (June 2000)
 *     by Pauline Middelink <middelink@polyware.nl>
 *
 * This is a set of routines which allow you to reserve a large (?)
 * amount of physical memory at boot-time, which can be allocated/deallocated
 * by drivers. This memory is intended to be used for devices such as
 * video framegrabbers which need a lot of physical RAM (above the amount
 * allocated by kmalloc). This is by no means efficient or recommended;
 * to be used only in extreme circumstances.
 *
 */

#ifndef __LINUX_BIGPHYSAREA_H
#define __LINUX_BIGPHYSAREA_H

#include <linux/types.h>

/* original interface */
extern caddr_t	bigphysarea_alloc(int size);
extern void	bigphysarea_free(caddr_t addr, int size);

/* new interface */
extern caddr_t	bigphysarea_alloc_pages(int count, int align, int priority);
extern void	bigphysarea_free_pages(caddr_t base);

#endif // __LINUX_BIGPHYSAREA_H
