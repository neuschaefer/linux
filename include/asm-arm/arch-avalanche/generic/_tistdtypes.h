/*
 *
 * _tistdtypres.h
 * Description:
 * TI Standard defines for primitive "C" types only
 *
  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2009-2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License 
  along with this program; if not, write to the Free Software 
  Foundation, Inc., 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution 
  in the file called LICENSE.GPL.


  Contact Information:
  Intel Corporation
  2200 Mission College Blvd.
  Santa Clara, CA  97052

  BSD LICENSE 

  Copyright(c) 2009-2015 Intel Corporation. All rights reserved.

  Redistribution and use in source and binary forms, with or without 
  modification, are permitted provided that the following conditions 
  are met:

    * Redistributions of source code must retain the above copyright 
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright 
      notice, this list of conditions and the following disclaimer in 
      the documentation and/or other materials provided with the 
      distribution.

    * Neither the name of Intel Corporation nor the names of its 
      contributors may be used to endorse or promote products derived 
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CONFIG_ARM_AVALANCHE_SOC /*AP-CPU*/
#include <linux/types.h>
#endif

/** \file   _tistdtypes.h
    \brief  TI Standard defines for primitive "C" types only

    This file provides TI Standard defines for primitive "C" types only


    \author     PSP Architecture Team
    \version    1.1
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
/*@{*/

/**
 * \enum Bool - Boolean Type
 */

typedef enum 
{
    False = 0,
    True = 1
} Bool;


typedef int             Int;    /**< Signed base integer quanity */
typedef unsigned int    Uns;    /**< This is depricated support only */
typedef unsigned int    Uint;   /**< Unsigned base integer quantity */
typedef unsigned long   Ulong;  /**< Unsigned base long quantity */
typedef char            Char;   /**< Character quantity */
typedef char*           String; /**< Character pointer quantity */
typedef void*           Ptr;    /**< Arbitrary (void) pointer (works for pointing to any datum) */

/* Signed integer definitions (64bit, 32bit, 16bit, 8bit) follow... */ 
typedef long long       Int64; 
typedef int             Int32;
typedef short           Int16;
typedef signed char     Int8; 

/* Unsigned integer definitions (64bit, 32bit, 16bit, 8bit) follow... */ 
typedef unsigned long long  Uint64; 

typedef unsigned int        Uint32;
typedef unsigned short      Uint16;
typedef unsigned char       Uint8;


/*@}*/
#endif /* _TI_STD_TYPES */


