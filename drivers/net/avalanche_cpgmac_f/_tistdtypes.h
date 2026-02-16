/*
 *
 * _tistdtypes.h
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


/** \file   _tistdtypes.h
    \brief  TI Standard defines for primitive "C" types only

    This file provides TI Standard defines for primitive "C" types only


    \author     PSP Architecture Team
    \version    1.0
 */ 
    
#ifndef _TI_STD_TYPES
#define _TI_STD_TYPES
    
/**
 * \defgroup TIBasicTypes TI Basic Types
 * 
 * All components - PAL, SRV, DDC, CSL shall use TI basic types 
 * to maintain compatibility of code with all systems. 
 * \n File _tistdtypes.h will adapt the types to the native compiler.
 */ 
    /*@{ */ 
    
/**
 * \enum Bool - Boolean Type
 */ 
typedef enum  { False = 0, True = 1 
} Bool;
typedef int Int;              /**< Signed base integer quanity */
typedef unsigned int Uns;      /**< This is depricated support only */
typedef unsigned int Uint;     /**< Unsigned base integer quantity */
typedef unsigned long  Ulong;
typedef char Char;             /**< Character quantity */
typedef char *String;          /**< Character pointer quantity */
typedef void *Ptr;             /**< Arbitrary (void) pointer (works for pointing to any datum) */

    /* Signed integer definitions (32bit, 16bit, 8bit) follow... */ 
typedef int Int32;
typedef short Int16;
typedef char Int8;

    /* Unsigned integer definitions (32bit, 16bit, 8bit) follow... */ 
typedef unsigned int Uint32;
typedef unsigned short Uint16;
typedef unsigned char Uint8;

    /*@} */ 
#endif  /* _TI_STD_TYPES */
