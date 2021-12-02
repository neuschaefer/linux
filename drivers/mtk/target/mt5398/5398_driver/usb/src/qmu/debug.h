/******************************************************************
 * Copyright 2005 Mentor Graphics Corporation
 *
 * This file is part of the Inventra Controller Driver for Linux.
 * 
 * The Inventra Controller Driver for Linux is free software; you 
 * can redistribute it and/or modify it under the terms of the GNU 
 * General Public License version 2 as published by the Free Software 
 * Foundation.
 * 
 * The Inventra Controller Driver for Linux is distributed in 
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public 
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with The Inventra Controller Driver for Linux ; if not, 
 * write to the Free Software Foundation, Inc., 59 Temple Place, 
 * Suite 330, Boston, MA  02111-1307  USA
 * 
 * ANY DOWNLOAD, USE, REPRODUCTION, MODIFICATION OR DISTRIBUTION 
 * OF THIS DRIVER INDICATES YOUR COMPLETE AND UNCONDITIONAL ACCEPTANCE 
 * OF THOSE TERMS.THIS DRIVER IS PROVIDED "AS IS" AND MENTOR GRAPHICS 
 * MAKES NO WARRANTIES, EXPRESS OR IMPLIED, RELATED TO THIS DRIVER.  
 * MENTOR GRAPHICS SPECIFICALLY DISCLAIMS ALL IMPLIED WARRANTIES 
 * OF MERCHANTABILITY; FITNESS FOR A PARTICULAR PURPOSE AND 
 * NON-INFRINGEMENT.  MENTOR GRAPHICS DOES NOT PROVIDE SUPPORT 
 * SERVICES OR UPDATES FOR THIS DRIVER, EVEN IF YOU ARE A MENTOR 
 * GRAPHICS SUPPORT CUSTOMER. 
 ******************************************************************/

 
#ifndef __MUSB_LINUX_DEBUG_H__
#define __MUSB_LINUX_DEBUG_H__

#define MUSB_MONITOR_DATA

#if 0 // mhzhang
#define WARN(fmt, args...) printf(fmt, ##args)
#endif

#define INFO(fmt,args...) printf(fmt, ##args)
#define ERR(fmt,args...) printf("[Error] " fmt, ##args)



#define MUSB_DEBUG 0  // mhzhang

#if MUSB_DEBUG > 0

#define STATIC
#define MGC_GetDebugLevel()	(MGC_DebugLevel)
#define MGC_EnableDebug()	do { MGC_DebugDisable=0; } while(0) 
#define MGC_DisableDebug()	do { MGC_DebugDisable=1; } while(0)

#define _dbg_level(level)  ( !MGC_DebugDisable && ( level>=-1 && MGC_GetDebugLevel()>=level  ))

#define xprintk(level, facility, format, args...) do { if ( _dbg_level(level) ) { \
    printf ("%s %d: " format , __FUNCTION__, __LINE__ , ## args); } } while (0)
	
#define DBG(level,fmt,args...) xprintk(level,KERN_INFO,fmt, ## args)
#define DEBUG_CODE(level, code)	do { if ( _dbg_level(level) ) { code }  } while (0)

#else 

#define STATIC static
#define MGC_GetDebugLevel()	0
#define MGC_EnableDebug()
#define MGC_DisableDebug()

#define DBG(fmt,args...)	do {}  while (0)
#define DEBUG_CODE(x, y)	do {}  while (0)

#endif

/*----------------------- DEBUG function/macros -----------------------------*/

#if MUSB_DEBUG > 0

extern char * decode_ep0stage (unsigned char stage);
extern void MGC_HdrcDumpRegs (unsigned char bEnd);

#endif

#endif //  __MUSB_LINUX_DEBUG_H__
