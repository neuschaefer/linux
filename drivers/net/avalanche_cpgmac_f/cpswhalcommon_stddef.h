/*
 *
 * cpswhalcommon_stddef.h 
 * Description:
 * see below
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */


/**@file*********************************************************************
 *  TNETDxxxx Software Support
 *
 *  FILE: CPSWHALCOMMON_STDDEF.H
 *
 *  DESCRIPTION:
 *      Standard definitions needed for CPSWHAL
 *
 *  HISTORY:
 *      Date      Modifier              Ver    Notes
 *      01Oct03   Michael Hanrahan      1.00   
 *****************************************************************************/
#ifndef _CPSWHALCOMMON_STDDEF_H
#define _CPSWHALCOMMON_STDDEF_H

#ifndef _INC_ADAM2

#ifndef size_t
#define size_t unsigned int
#endif

typedef char           bit8;
typedef short          bit16;
typedef int            bit32;

typedef unsigned char  bit8u;
typedef unsigned short bit16u;
typedef unsigned int   bit32u;
#endif

#ifndef TRUE
#define TRUE (1==1)
#endif

#ifndef FALSE
#define FALSE !(TRUE)
#endif

#ifndef NULL
#define NULL 0
#endif

typedef const char HAL_CONTROL_KEY;
#endif
