/*
 *  OS abstraction (non PAL OS) for PPD driver
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 * Copyright (C) 2007, Texas Instruments, Inc.
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 * modify it under the terms of the GNU General Public License as
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 */

#include "ppd_pvt.h"

#if ((PPD_DEBUG) & PPD_DEBUG_ERR)
#define printErr(fmt,args...) printk(fmt , ##args)
#else
#define printErr(x,...)
#endif

#if ((PPD_DEBUG) & PPD_DEBUG_MSG)
#define printMsg(fmt,args...) printk(fmt , ##args)
#else
#define printMsg(x,...)
#endif

#if ((PPD_DEBUG) & PPD_DEBUG_CMD)
#define printCmd(fmt,args...) printk(fmt , ##args)
#else
#define printCmd(x,...)
#endif

#if ((PPD_DEBUG) & PPD_DEBUG_EVN)
#define printEvn(fmt,args...) printk(fmt , ##args)
#else
#define printEvn(x,...)
#endif

#define ppd_os_get_io_virt(addr)    ((void *)IO_ADDRESS((void *)(addr)))
#define ppd_os_get_io_phys(addr)    ((void *)IO_VIRT2PHY((void *)(addr)))
