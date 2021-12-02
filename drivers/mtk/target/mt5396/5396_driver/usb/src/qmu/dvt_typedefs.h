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
#ifndef DVT_TYPEDEFS_H
#define DVT_TYPEDEFS_H

/*==== CONSTANTS ==================================================*/

#define IMPORT  EXTERN
#ifndef __cplusplus
  #define EXTERN  extern
#else
  #define EXTERN  extern "C"
#endif
#define LOCAL     static
#define GLOBAL
#define EXPORT    GLOBAL

#define EQ        ==
#define NEQ       !=
#define AND       &&
#define OR        ||
#define XOR(A,B)  ((!(A) AND (B)) OR ((A) AND !(B)))

#ifndef FALSE
  #define FALSE   0
#endif

#ifndef TRUE
  #define TRUE    1
#endif

#ifndef NULL
  #define NULL    0
#endif

//enum boolean { false, true};
enum {RX, TX, NONE};

#ifndef BOOL
//typedef unsigned char  BOOL;
#endif

/*==== TYPES ======================================================*/

typedef unsigned char   kal_uint8;
typedef signed char     S8;
typedef unsigned short  kal_uint16;
typedef signed short    S16;
typedef unsigned int    kal_uint32;
typedef signed int      S32;

typedef unsigned long long  kal_uint64;

typedef unsigned char   US8;
typedef unsigned short  US16;
typedef unsigned int    US32;

typedef volatile unsigned char  *P_kal_uint8;
typedef volatile signed char    *P_S8;
typedef volatile unsigned short *P_kal_uint16;
typedef volatile signed short   *P_S16;
typedef volatile unsigned int   *P_kal_uint32;
typedef volatile signed int     *P_S32;

typedef long            LONG;
typedef unsigned char   UBYTE;
typedef short           SHORT;

#if 0
typedef signed char     kal_int8;
typedef signed short    kal_int16;
typedef signed int      kal_int32;
typedef long long       kal_int64;
typedef unsigned char   kal_uint8;
typedef unsigned short  kal_uint16;
typedef unsigned int    kal_uint32;
typedef char            kal_char;
#endif

typedef unsigned int            *UINT32P;
typedef volatile unsigned short *UINT16P;
typedef volatile unsigned char  *UINT8P;
typedef unsigned char           *U8P;

#if 0
typedef volatile unsigned char  *P_U8;
typedef volatile signed char    *P_S8;
typedef volatile unsigned short *P_U16;
typedef volatile signed short   *P_S16;
typedef volatile unsigned int   *P_U32;
typedef volatile signed int     *P_S32;
typedef unsigned long long      *P_U64;
typedef signed long long        *P_S64;

typedef unsigned char       U8;
typedef signed char         S8;
typedef unsigned short      U16;
typedef signed short        S16;
typedef unsigned int        U32;
typedef signed int          S32;
typedef unsigned long long  U64;
typedef signed long long    S64;
#endif
//typedef unsigned char       bool;

typedef enum {
   KAL_FALSE = 0,
   KAL_TRUE  = 1,
   kal_false = KAL_FALSE,
   kal_true  = KAL_TRUE,
} kal_bool;

//typedef unsigned char   UINT8;
//typedef unsigned short  UINT16;
//typedef unsigned int    UINT32;
typedef unsigned short  USHORT;
//typedef signed char     INT8;
//typedef signed short    INT16;
//typedef signed int      INT32;
//typedef unsigned short  USHORT;
typedef signed int      DWORD;
//typedef void            VOID;
//typedef unsigned char   BYTE;
//typedef float           FLOAT;

typedef char           *LPCSTR;
typedef short          *LPWSTR;

/*==== EXPORT =====================================================*/

#define MAXIMUM(A,B)       (((A)>(B))?(A):(B))
#define MINIMUM(A,B)       (((A)<(B))?(A):(B))

typedef enum DVT_BOOL_TAG
{
	DVT_FALSE,
	DVT_TRUE
} DVT_BOOL_T;

typedef enum DVT_STATUS_TAG
{
	DVT_SUCCESS,
	DVT_FAIL
} DVT_STATUS_T;

#endif //DVT_TYPEDEFS_H
