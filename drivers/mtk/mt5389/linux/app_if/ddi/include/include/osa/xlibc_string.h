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
/******************************************************************************

    LGE. DTV RESEARCH LABORATORY
    COPYRIGHT(c) LGE CO.,LTD. 1998-2007. SEOUL, KOREA.
    All rights are reserved.
    No part of this work covered by the copyright hereon may be
    reproduced, stored in a retrieval system, in any form or
    by any means, electronic, mechanical, photocopying, recording
    or otherwise, without the prior permission of LG Electronics.

    FILE NAME   : xlibc_string.h
    VERSION     : 1.0
    AUTHOR      : Jackee, Lee(이재길, jackee@lge.com)
    DATE        : 2007/02/22
	DESCRIPTION	: This is a sample header file for all source codes.
				  It is highly recommended that all programmers should
				  follow this	standard format and fill out every item.

*******************************************************************************/

#ifndef	_XLIBC_STRING_H_
#define	_XLIBC_STRING_H_

/*-----------------------------------------------------------------------------
	제어 상수
	(Control Constants)
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	#include 파일들
	(File Inclusions)
------------------------------------------------------------------------------*/

#ifdef	__cplusplus
extern "C"
{
#endif /* __cplusplus */

/*-----------------------------------------------------------------------------
	상수 정의
	(Constant Definitions)
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	매크로 함수 정의
	(Macro Definitions)
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	형 정의
	(Type Definitions)
------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	외부 전역변수와 함수 prototype 선언
	(Extern Variables & Function Prototype Declarations)
------------------------------------------------------------------------------*/
#ifndef _EMUL_WIN
/*
extern int		strnicmp(const char *s1, const char *s2, size_t len);
extern char*	strcpy(char * dest,const char *src);
extern char*	strncpy(char * dest,const char *src,size_t count);
extern char*	strcat(char * dest, const char * src);
extern char*	strncat(char *dest, const char *src, size_t count);
#ifndef	__HAVE_ARCH_STRCMP
extern int		strcmp(const char * cs,const char * ct);
#endif
extern int		strcasecmp(const char * cs,const char * ct);
#ifndef	__HAVE_ARCH_STRNCMP
extern int		strncmp(const char * cs,const char * ct,size_t count);
#endif
#ifndef	__HAVE_ARCH_STRCHR
extern char*	strchr(const char * s, int c);
#endif
extern char*	strrchr(const char * s, int c);
extern size_t	strlen(const char * s);
extern size_t	strnlen(const char * s, size_t count);
#ifndef __HAVE_ARCH_STRDUP
extern char*	strdup(const char *s);
#endif
#ifndef __HAVE_ARCH_STRSPN
extern size_t	strspn(const char *s, const char *accept);
#endif
#ifndef __HAVE_ARCH_STRPBRK
extern char*	strpbrk(const char * cs,const char * ct);
#endif
extern char*	strtok(char * s,const char * ct);
#ifndef	__HAVE_ARCH_STRSEP
extern char*	strsep(char **s, const char *ct);
#endif
extern char*	strswab(const char *s);
extern void*	memset(void * s,int c,size_t count);
extern void*	memcpy(void * dest,const void *src,size_t count);
extern void*	memmove(void * dest,const void *src,size_t count);
extern int		memcmp(const void * cs,const void * ct,size_t count);
extern void*	memscan(void * addr, int c, size_t size);
extern char*	strstr(const char * s1,const char * s2);
extern void*	memchr(const void *s, int c, size_t n);
extern char*	octsdup(const char *src, size_t len);
extern char*	strlwr( const char *s_str );
extern char*	strupr( const char *s_str );
extern long int
				strtol(const char *nptr, char **endptr, int base);
extern long long int
				strtoll(const char *nptr, char **endptr, int base);
extern unsigned long int
				strtoul(const char *nptr, char **endptr, int base);
extern unsigned long long int
				strtoull(const char *nptr, char **endptr, int base);
extern double	strtod(const char *nptr, char **endptr);
extern float	strtof(const char *nptr, char **endptr);
extern long double
				strtold(const char *nptr, char **endptr);
*/
extern long int					strtol(const char *nptr, char **endptr, int base);
extern long long int			strtoll(const char *nptr, char **endptr, int base);
extern unsigned long int		strtoul(const char *nptr, char **endptr, int base);
extern unsigned long long int	strtoull(const char *nptr, char **endptr, int base);
extern double					strtod(const char *nptr, char **endptr);
extern float					strtof(const char *nptr, char **endptr);
extern long double				strtold(const char *nptr, char **endptr);
#else /* _EMUL_WIN */
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define strtoull	_strtoui64
#endif /* _EMUL_WIN */
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _XLIBC_STRING_H_ */
