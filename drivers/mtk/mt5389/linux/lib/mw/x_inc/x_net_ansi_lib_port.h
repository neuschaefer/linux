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
/*-----------------------------------------------------------------------------------------------*
 * Description: ANSI C library porting layer
 *              Note, this file CANNOT be use with libc/stdio.h
 *-----------------------------------------------------------------------------------------------*/

#ifndef __ANSI_LIB_H
#define __ANSI_LIB_H

/* ARM tool chain header */
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#ifdef _USE_LINUX
    #include <stdio.h>
    #include <ctype.h>
    #include <time.h>
    #include <errno.h>
    #include <sys/time.h>
#else /* this is for Nucleus */

#ifdef POSIX_SUPPORT
    #include <stdio.h>
#endif

/* original in sys/type.h but we do not have */
#ifndef __SSIZE_T_
#define __SSIZE_T_
    typedef int ssize_t;
#endif

/* original in stdio.h */
#ifndef POSIX_SUPPORT
    #define BUFSIZ  1024
#endif

/* middleware header file */
#include "x_os.h"
#include "x_dbg.h"

#include "x_net_rand.h"

/**
 * struct timeval, the arm tool chain do not have this 
 * we define it in libc/u_time.h  
 */
#ifdef MT8520
#include "u_time.h"
#else
#ifndef _USE_LINUX
#include "time.h"
#else
#include <time.h>
#endif
#endif

/* in ctype.h the follow is lowcase */
#define ISXDIGIT        isxdigit
#define ISDIGIT         isdigit
//#define ISSPACE         isspace
#define ISSPACE(c)      ((c) == ' ' || ((c) >= 0x09 && (c) <= 0x0D))//haifei change it for vincent.
#define ISALPHA         isalpha
#define ISALNUM         isalnum

/* system call isdigit now has bug. so use this macro to replace.  */
#ifdef isdigit
#undef isdigit
#endif
#define isdigit(c)    ((c) >='0' && (c) <= '9'? 1:0)

/* memory allocate and free action */
#if 0
#ifndef malloc 
    #define malloc          x_mem_alloc
#endif

#ifndef free
    #define free            x_mem_free
#endif

#ifndef realloc
    #define realloc         x_mem_realloc
#endif

#ifndef calloc
    #define calloc          x_mem_calloc
#endif
#endif

/* RVCT 2.2/2.1 does not include these APIs, use middleware API instead of it */
//#ifndef POSIX_SUPPORT
    //#define strcasecmp      x_strcasecmp
    //#define strncasecmp     x_strncasecmp
//#endif

/* network module provide rand like API */
#define srand           x_net_rand_set_seed
#define rand            x_net_rand_get

/* I/O */
#ifndef printf
    #define printf          x_dbg_stmt
#endif

/* some of stdio.h api declare... tool chain's stdio.h conflict with libc/stdio.h */
#ifndef POSIX_SUPPORT
	#define SEEK_SET       ((UINT8) 1) /*add by mtk40135*/
	#define SEEK_CUR       ((UINT8) 2) /*add by mtk40135*/
	#define SEEK_END       ((UINT8) 3) /*add by mtk40135*/
    #define FILE            void
    #define stdin           ((void*)0)
    #define stdout          ((void*)1)
    #define stderr          ((void*)1)

    extern int              sscanf(const char *s, const char * format, ... );
    extern int              snprintf(char *s, size_t n, const char * format, ...);
    extern int              sprintf(char * s, const char * format, ...);
    extern int              vprintf(const char * format, va_list ap);
    extern int              vsnprintf(char * s, size_t n, const char * format, va_list ap);
    extern int              vsprintf(char * s, const char * format, va_list ap);

    /* replace stdio api to our glue layer */
    extern void *           _glue_fopen(const char *filename, const char *mode);
    extern int              _glue_feof(void *stream);
    extern int              _glue_fseek(void *stream, long offset, int whence);
    extern size_t           _glue_fread(void *ptr, size_t size, size_t nitems, void *stream);
    extern long             _glue_ftell(void *stream);
    extern size_t           _glue_fwrite(const void *buffer, size_t size, size_t count, void *stream);
    extern int              _glue_fputc(int c, void *stream);
    extern int              _glue_fputs(const char* ps_str, void *stream);
    extern int              _glue_fgetc(void *stream);
    extern int              _glue_fgets(char* ps_str, int n, void *stream);
    extern int              _glue_fprintf(void *stream, const CHAR *ps_format, ...);
    extern int              _glue_fclose(void *stream);
    extern int              _glue_fflush(void *stream);
    extern int              _glue_file_size(const char *file_name);
    extern int              _glue_ferror(void *stream);
    extern int              _glue_getuid(void);
    extern int              _glue_vfprintf(void *stream, const char *format, va_list ap);
    extern int              _glue_stat(const char *file_name, void *buf);

    /* glue stdio.h to our glue api */
    #define fopen           _glue_fopen
    #define feof            _glue_feof
    #define fseek           _glue_fseek
    #define fread           _glue_fread 
    #define ftell           _glue_ftell 
    #define fwrite          _glue_fwrite
    #define fputc           _glue_fputc
    #define fputs           _glue_fputs
    #define fgetc           _glue_fgetc
    #define fgets           _glue_fgets
    #define fprintf         _glue_fprintf
    #define fclose          _glue_fclose
    #define ferror          _glue_ferror
    #define fflush          _glue_fflush
    #define vfprintf        _glue_vfprintf
    #define stat            _glue_stat
#endif

/* 
 * TV use RVCT 2.1 which sscanf cause system crash 
 * in case BD use more newer RVCT and it's work fine.
 * So, when TV use, use x_sscanf.
*/
#ifndef MT8520
    #define sscanf      x_sscanf
#endif
    
/* something wrong on RVCT 2.1's atoi. */
#ifndef MT8520
    #define atoi(str)   ((int)x_strtoll(str, NULL, 10))
    #define strtoll(pc_beg_ptr, ppc_end_ptr, ui1_base)   ((int)x_strtoll(pc_beg_ptr, ppc_end_ptr, ui1_base))
    #define strtoul(pc_beg_ptr, ppc_end_ptr, ui1_base)   ((int)x_strtoll(pc_beg_ptr, ppc_end_ptr, ui1_base))
#endif

/* for stat use. */
#ifndef POSIX_SUPPORT
    struct _glue_stat
    {
        CHAR value[100];
    };
#endif

/* unix relate */
#define uid_t           int
#define getuid          _glue_getuid

#endif /*__ANSI_LIB_H*/
#endif /* Nucleus c tool chain porting layer */
