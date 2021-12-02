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
#ifndef __KERNEL__

#ifdef _MTK_BUILD_
#include "divx_drm_adp_common.h"
#include "libc/memory.h"
#include "libc/string.h"
#else
#include "DivXInt.h"
#include <string.h>
#endif

#else

#include "../divx_drm_adp_common.h"

#endif


//-------------------------------------------------------------------------------------
#define swap_01_in(data, dataLength) \
{ \
uint32_t i = 0; \
for ( i = 0; i < (uint32_t)dataLength; i++ ) \
{ \
    uint8_t chr = ((uint8_t*)data)[i]; \
    switch ( i % 4 ) \
    { \
    case 0: \
       chr =  ( 0x3C & ((uint8_t*)data)[i] ) | ( ( ((uint8_t*)data)[i] & 0x03 ) << 6 ); \
       chr |=  ( ( ( ((uint8_t*)data)[i] & 0xC0 ) >> 6 ) & 0x03 ); \
       break; \
    case 1: \
       chr =  ( 0xC3  & ((uint8_t*)data)[i] ) | ( ( ((uint8_t*)data)[i] & 0x0C ) << 2 ); \
       chr |=  ( ( ( ((uint8_t*)data)[i] & 0x30 ) >> 2 ) & 0x0C ); \
       break; \
    case 2: \
       chr =  ( 0xF0 & ((uint8_t*)data)[i] ) | ( ( ((uint8_t*)data)[i] & 0x03 ) << 2 ); \
       chr |=  ( ( ( ((uint8_t*)data)[i] & 0x0C ) >> 2 ) & 0x03 ); \
       break; \
    case 3: \
       chr =  ( 0x0F & ((uint8_t*)data)[i] ) | ( ( ((uint8_t*)data)[i] & 0x30 ) << 2 ); \
       chr |=  ( ( ( ((uint8_t*)data)[i] & 0xC0 ) >> 2 ) & 0x30 ); \
       break; \
    }; \
    ((uint8_t*)data)[i] = chr; \
} \
} \

#define swap_01_out(data, dataLength) \
{ \
uint32_t i = 0; \
for ( i = 0; i < (uint32_t)dataLength; i++ ) \
{ \
    uint8_t chr = ((uint8_t*)data)[i]; \
    switch ( i % 4 ) \
    { \
    case 0: \
       chr =  ( 0x3C & ((uint8_t*)data)[i] ) | ( ( ((uint8_t*)data)[i] & 0x03 ) << 6 ); \
       chr |=  ( ( ( ((uint8_t*)data)[i] & 0xC0 ) >> 6 ) & 0x03 ); \
       break; \
    case 1: \
       chr =  ( 0xC3  & ((uint8_t*)data)[i] ) | ( ( ((uint8_t*)data)[i] & 0x0C ) << 2 ); \
       chr |=  ( ( ( ((uint8_t*)data)[i] & 0x30 ) >> 2 ) & 0x0C ); \
       break; \
    case 2: \
       chr =  ( 0xF0 & ((uint8_t*)data)[i] ) | ( ( ((uint8_t*)data)[i] & 0x03 ) << 2 ); \
       chr |=  ( ( ( ((uint8_t*)data)[i] & 0x0C ) >> 2 ) & 0x03 ); \
       break; \
    case 3: \
       chr =  ( 0x0F & ((uint8_t*)data)[i] ) | ( ( ((uint8_t*)data)[i] & 0x30 ) << 2 ); \
       chr |=  ( ( ( ((uint8_t*)data)[i] & 0xC0 ) >> 2 ) & 0x30 ); \
       break; \
    }; \
    ((uint8_t*)data)[i] = chr; \
} \
} \

#define xor_01_in(data, dataLength) \
{ \
uint32_t i = 0; \
for ( i = 0; i < dataLength-1; i++ ) \
{ \
     ((uint8_t*)data)[i] = ((uint8_t*)data)[i] ^ ((uint8_t*)data)[i+1]; \
}\
} \

#define xor_01_out(data, dataLength) \
{ \
int32_t i = 0; \
for ( i = (dataLength-2); i >= 0; i-- ) \
{ \
     ((uint8_t*)data)[i] = ((uint8_t*)data)[i] ^ ((uint8_t*)data)[i+1]; \
} \
} \


const uint8_t s01replacementMap[] = \
{ \
    12, 15, 14, 7, 11, 8, 6, 13, 5, 4, 1, 0, 2, 3, 10, 9 \
}; \

#define sub_01_in(data, dataLength) \
{ \
    int i = 0; \
    for ( i = 0; i < (int)dataLength; i++ ) \
    { \
        data[i] =   ( s01replacementMap[ ( data[i] & 0x0F ) ] ) \
                  | ( ( s01replacementMap[ ( data[i] & 0xF0 ) >> 4 ] ) << 4 ); \
    } \
} \

#define sub_01_out(data, dataLength) \
{ \
    int outer; \
    int i = 0; \
    for ( outer = 0; outer < (int)dataLength; outer++ ) \
    { \
       for ( i = 0; i < (int)dataLength; i++ ) \
       { \
           if ( ( data[outer] & 0x0F ) == s01replacementMap[i] ) \
           { \
               data[outer] &= 0xF0; \
               data[outer] |= i; \
               break; \
           } \
       } \
       for ( i = 0; i < (int)dataLength; i++ ) \
       { \
           if ( ( ( data[outer] & 0xF0 ) >> 4 ) == s01replacementMap[i] ) \
           { \
               data[outer] &= 0x0F; \
               data[outer] |= ( i << 4 ); \
               break; \
           } \
       } \
    } \
} \

const uint8_t s02replacementMap[] = \
{ \
   0, 13, 1, 10, 9, 8, 4, 15, 2, 6, 14, 12, 5, 3, 7, 11 \
}; \

#define sub_02_in(data, dataLength) \
{ \
    int i = 0; \
    for ( i = 0; i < (int)dataLength; i++ ) \
    { \
        data[i] =   ( s02replacementMap[ ( data[i] & 0x0F ) ] ) \
                  | ( ( s02replacementMap[ ( data[i] & 0xF0 ) >> 4 ] ) << 4 ); \
    } \
} \

#define sub_02_out(data, dataLength) \
{ \
    int outer; \
    int i = 0; \
    for ( outer = 0; outer < (int)dataLength; outer++ ) \
    { \
       for ( i = 0; i < (int)dataLength; i++ ) \
       { \
           if ( ( data[outer] & 0x0F ) == s02replacementMap[i] ) \
           { \
               data[outer] &= 0xF0; \
               data[outer] |= i; \
               break; \
           } \
       } \
       for ( i = 0; i < (int)dataLength; i++ ) \
       { \
           if ( ( ( data[outer] & 0xF0 ) >> 4 ) == s02replacementMap[i] ) \
           { \
               data[outer] &= 0x0F; \
               data[outer] |= ( i << 4 ); \
               break; \
           } \
       } \
    } \
} \

const uint8_t s03replacementMap[] = \
{ \
    5, 1, 13, 7, 11, 14, 6, 15, 3, 4, 10, 0, 2, 12, 8, 9 \
}; \

#define sub_03_in(data, dataLength) \
{ \
    int i = 0; \
    for ( i = 0; i < (int)dataLength; i++ ) \
    { \
        data[i] =   ( s03replacementMap[ ( data[i] & 0x0F ) ] ) \
                  | ( ( s03replacementMap[ ( data[i] & 0xF0 ) >> 4 ] ) << 4 ); \
    } \
} \

#define sub_03_out(data, dataLength) \
{ \
    int outer; \
    int i = 0; \
    for ( outer = 0; outer < (int)dataLength; outer++ ) \
    { \
       for ( i = 0; i < (int)dataLength; i++ ) \
       { \
           if ( ( data[outer] & 0x0F ) == s03replacementMap[i] ) \
           { \
               data[outer] &= 0xF0; \
               data[outer] |= i; \
               break; \
           } \
       } \
       for ( i = 0; i < (int)dataLength; i++ ) \
       { \
           if ( ( ( data[outer] & 0xF0 ) >> 4 ) == s03replacementMap[i] ) \
           { \
               data[outer] &= 0x0F; \
               data[outer] |= ( i << 4 ); \
               break; \
           } \
       } \
    } \
} \


//-------------------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

void bundleBaseKeyScrambleIn( uint8_t* data,
            uint32_t size )
{
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    return;

}
void bundleBaseKeyScrambleOut( uint8_t* data,
             uint32_t size )
{
    xor_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    return;

}
void baseKeyScrambleIn( uint8_t* data,
            uint32_t size )
{
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    return;

}
void baseKeyScrambleOut( uint8_t* data,
             uint32_t size )
{
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    return;

}
void compileTimeKeyScrambleIn( uint8_t* data,
            uint32_t size )
{
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    return;

}
void compileTimeKeyScrambleOut( uint8_t* data,
             uint32_t size )
{
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    return;

}
void bundleCompileTimeKeyScrambleIn( uint8_t* data,
            uint32_t size )
{
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    return;

}
void bundleCompileTimeKeyScrambleOut( uint8_t* data,
             uint32_t size )
{
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    return;

}
void publicKeyScrambleIn( uint8_t* data,
            uint32_t size )
{
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    return;

}
void publicKeyScrambleOut( uint8_t* data,
             uint32_t size )
{
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    return;

}
void drmMemoryScrambleIn( uint8_t* data,
            uint32_t size )
{
    xor_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    sub_02_in( data, size )
    swap_01_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_03_in( data, size )
    sub_02_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    swap_01_in( data, size )
    sub_01_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    sub_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    xor_01_in( data, size )
    sub_02_in( data, size )
    sub_03_in( data, size )
    sub_01_in( data, size )
    sub_03_in( data, size )
    swap_01_in( data, size )
    sub_03_in( data, size )
    return;

}
void drmMemoryScrambleOut( uint8_t* data,
             uint32_t size )
{
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_03_out( data, size );
    sub_03_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    swap_01_out( data, size );
    sub_01_out( data, size );
    xor_01_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    sub_02_out( data, size );
    swap_01_out( data, size );
    sub_02_out( data, size );
    sub_01_out( data, size );
    sub_02_out( data, size );
    sub_03_out( data, size );
    xor_01_out( data, size );
    sub_02_out( data, size );
    xor_01_out( data, size );
    xor_01_out( data, size );
    return;

}
#ifdef __cplusplus
}
#endif

/*
   // $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/DrmDataLoad.c#1 $
   // Copyright (c) 2006 DivX, Inc. http://www.divx.com/corporate
   // All rights reserved.
   //
   // This software is the confidential and proprietary information of DivX
   // Inc. ("Confidential Information").  You shall not disclose such Confidential
   // Information and shall use it only in accordance with the terms of the license
   // agreement you entered into with DivX, Inc.
*/

#ifndef __KERNEL__

#include "DivXPortableAPI.h"
#include "DivXAesDecrypt.h"
#include "DivXAesEncrypt.h"
#include "DivXSha.h"
#include "DrmLocal.h"
#include "DrmTypes.h"

#else

#include "crypto/DivXPortableAPI.h"
#include "crypto/DivXAesDecrypt.h"
#include "crypto/DivXAesEncrypt.h"
#include "crypto/DivXSha.h"
#include "local/DrmLocal.h"
#include "DrmTypes.h"

#endif

DECLARE_DIVX_PORTABLE_MODULE

#define DRM_DIVX_DL_PROFILE_QMOBILE          0
#define DRM_DIVX_DL_PROFILE_MOBILE           1
#define DRM_DIVX_DL_PROFILE_HOMETHEATER      2
#define DRM_DIVX_DL_PROFILE_HIGHDEF          3

/* Bundling ID */
uint32_t bid = 0x0;

const uint8_t bk[] = 
    {
        0x2b, 0xc6, 0x51, 0x2a, 0xc3, 0x91, 0xca, 0x7b, 0x25, 0xb7, 0xda, 0xdb, 0xd0, 0x70, 0x6a, 0x6d, 
        0x69, 0x5b, 0xb6, 0x53, 0xfa, 0xff, 0x29, 0xe8, 0x96, 0xc1, 0x12, 0x12, 0x4b, 0xef, 0x63, 0xf8, 
        0x6d, 0x88, 0xec, 0xdd, 0x3b, 0xb6, 0x85, 0x9c, 0xcf, 0xec, 0x14, 0x15, 0x72, 0x15, 0xef, 0x63, 
        0xd2, 0x47, 0xb4, 0x8a, 0x94, 0x9e, 0xfb, 0xba, 0x95, 0xaf, 0xa0, 0x35, 0x7c, 0xc3, 0x33, 0xd1, 
        0x8b, 0xc3, 0xf2, 0x55, 0x63, 0xcf, 0xfd, 0xc3, 0x5e, 0x15, 0xbc, 0x09, 0x4a, 0xe7, 0x2e, 0x76, 
        0x45, 0xff, 0xd8, 0xb2, 0x4b, 0x75, 0xfa, 0x9e, 0xbb, 0x00, 0xeb, 0x17, 0x05, 0x54, 0xf7, 0x5a, 
        0xe1, 0xed, 0x42, 0x75, 0x39, 0x42, 0xb2, 0x38, 0x1c, 0x5f, 0x1f, 0x26, 0xd0, 0x61, 0xc5, 0x0f, 
        0xc2, 0xf7, 0x0e, 0x49, 0xfd, 0xfc, 0x8f, 0xb6, 0x17, 0x70, 0xcf, 0xe9, 0x8e, 0xc5, 0xa1, 0x82, 
        0x74, 0x52, 0x64, 0xfd, 0xcc, 0xf0, 0xbc, 0x37, 0x49, 0x2c, 0x7e, 0x1e, 0xe3, 0x4e, 0x60, 0x4c, 
        0x5e, 0x5a, 0x8c, 0xb2, 0x50, 0xc6, 0xbb, 0x69, 0xeb, 0xab, 0x00, 0xdb, 0xf4, 0x20, 0x47, 0xc3, 
        0x10, 0x17, 0xe9, 0x0c, 0xc5, 0x4b, 0x8d, 0xcf, 0xce, 0xd8, 0x4c, 0xbb, 0x89, 0xa2, 0xe5, 0xac, 
        0xd6, 0x44, 0xeb, 0x2e, 0x68, 0xe2, 0xc4, 0x67, 0x73, 0x3c, 0x2e, 0x37, 0x48, 0x7a, 0xe5, 0x05, 
        0x1d, 0xf9, 0x1c, 0xdf, 0x59, 0x2f, 0x5b, 0x66, 0x3c, 0x57, 0x5f, 0xcf, 0xbd, 0x3e, 0xb0, 0xea, 
        0x6b, 0x41, 0xa2, 0x1f, 0xfe, 0xa9, 0xa2, 0x3f, 0xa0, 0xca, 0x79, 0x90, 0x0b, 0xfe, 0x45, 0x14, 
        0x58, 0xb0, 0x89, 0x40, 0x85, 0xbd, 0x42, 0x7f, 0x38, 0x72, 0x68, 0x18, 0x28, 0x2a, 0xbf, 0xdb, 
        0xc0, 0xf8, 0xee, 0x9e, 0x24, 0x93, 0x3d, 0xa2, 0xdb, 0x4b, 0xd5, 0x7b, 0x82, 0xf4, 0xa2, 0x2d, 
        0x1b, 0xb9, 0x58, 0xa2, 0x59, 0xc4, 0xae, 0xc5, 0x9c, 0x63, 0xf0, 0x03, 0x88, 0xcf, 0x18, 0x49, 
        0xfe, 0x06, 0x99, 0xa5, 0x47, 0xd5, 0x48, 0xb6, 0x78, 0x80, 0xf0, 0xb1, 0xb5, 0x95, 0xab, 0x75, 
        0x32, 0xf2, 0xc8, 0xb7, 0x88, 0x38, 0xed, 0x7a, 0xe6, 0xe6, 0xd0, 0x16, 0xb4, 0xd2, 0xef, 0x2b, 
        0x04, 0x69, 0xd7, 0x5d, 0x48, 0x45, 0x53, 0xd4, 0xe6, 0xbb, 0xcb, 0xaa, 0x6e, 0x08, 0xd7, 0x21, 
        0x26, 0xab, 0xa0, 0x79, 0xf5, 0x79, 0x0e, 0x0c, 0xa2, 0x9b, 0x88, 0xf4, 0x75, 0x22, 0x22, 0xce, 
        0xb8, 0xac, 0x7e, 0x57, 0xbc, 0x81, 0x9e, 0x96, 0x89, 0xab, 0x5d, 0xd9, 0x42, 0xa1, 0x0c, 0x4f, 
        0xf7, 0x53, 0x6e, 0x3e, 0x75, 0xb7, 0x9f, 0xa1, 0xf9, 0xf3, 0xcd, 0xc5, 0x3b, 0x59, 0x4b, 0x90, 
        0x1a, 0xce, 0x76, 0xac, 0x74, 0x58, 0x6e, 0x2d, 0x92, 0x2c, 0xf0, 0x85, 0x75, 0xbd, 0xdc, 0xd9, 
        0xfa, 0xc4, 0x62, 0x56, 0xdb, 0x5a, 0x3d, 0x2b, 0x9f, 0xa8, 0x43, 0x63, 0x5e, 0x92, 0x3e, 0xe5, 
        0xd1, 0x5f, 0x5e, 0xfa, 0x94, 0x34, 0x54, 0xdf, 0xa9, 0x4b, 0xb3, 0x01, 0x1b, 0x57, 0xe3, 0x6d, 
        0x62, 0xe1, 0x7f, 0xdc, 0x18, 0xce, 0x50, 0xdc, 0xef, 0xa8, 0x6d, 0x8c, 0x5c, 0x57, 0x8a, 0x89, 
        0x6f, 0x50, 0x37, 0x68, 0x28, 0xd9, 0xbf, 0x08, 0x70, 0x7b, 0xa9, 0xe2, 0xe3, 0x97, 0x3a, 0x5a, 
        0x3d, 0x95, 0x44, 0x34, 0x81, 0x96, 0x6d, 0xa2, 0x2b, 0x58, 0xd3, 0x0a, 0x1f, 0x6c, 0x64, 0x35, 
        0x9d, 0xf4, 0x9e, 0x11, 0x75, 0x58, 0x2b, 0x13, 0xb9, 0x79, 0xa2, 0x47, 0x54, 0xf4, 0xa8, 0xdf, 
        0xe7, 0xb7, 0x63, 0xe7, 0x5f, 0x77, 0x44, 0x9e, 0x82, 0x56, 0xf0, 0x7d, 0x82, 0xa9, 0x04, 0x2a, 
        0x1f, 0xf4, 0xe9, 0x81, 0x1d, 0x9c, 0x88, 0x47, 0xb4, 0xa9, 0xe2, 0x70, 0x99, 0x5c, 0x3a, 0x47, 
        0x31, 0xed, 0x1e, 0x3b, 0xd3, 0x74, 0xd5, 0xbd, 0xda, 0x1a, 0x05, 0x4f, 0x79, 0x74, 0xa0, 0xfc, 
        0x9e, 0xac, 0x88, 0x14, 0x04, 0xd3, 0x2f, 0x23, 0xbc, 0x7b, 0x1d, 0x3d, 0x13, 0xb8, 0x2e, 0x2a, 
        0xf3, 0xa9, 0xc1, 0xfb, 0x3e, 0x63, 0x8d, 0xfd, 0x82, 0x50, 0xd9, 0x8b, 0xc7, 0x23, 0x97, 0x9c, 
        0x32, 0x0d, 0x43, 0x9b, 0xd9, 0x4c, 0x88, 0xdf, 0xa5, 0x50, 0x21, 0x0e, 0x80, 0x1e, 0xde, 0x2d, 
        0x82, 0x20, 0xd2, 0x43, 0x25, 0xe9, 0x88, 0x31, 0xae, 0xc1, 0x6b, 0xe5, 0xad, 0x69, 0x44, 0xec, 
        0xa3, 0xed, 0x71, 0xd9, 0x84, 0x44, 0x72, 0x7b, 0x31, 0xc6, 0x06, 0x0d, 0xa3, 0x99, 0x57, 0xcb, 
        0x7b, 0xf0, 0x60, 0x64, 0x46, 0xd3, 0x3b, 0x10, 0xf5, 0xfd, 0xb9, 0x32, 0x9c, 0x5e, 0xa3, 0xe0, 
        0x76, 0x59, 0x70, 0x20, 0xbc, 0xbe, 0xe3, 0x0a, 0xc3, 0xa8, 0x98, 0x21, 0x9e, 0xb2, 0x0c, 0x52, 
        0xe9, 0x18, 0x66, 0xb8, 0x8f, 0x74, 0xf7, 0x11, 0x35, 0x4f, 0x18, 0x6e, 0xe6, 0xe9, 0xc8, 0x90, 
        0xde, 0xae, 0xc9, 0xec, 0x74, 0xdd, 0x89, 0x52, 0x34, 0xd8, 0xf4, 0x22, 0x4b, 0x0e, 0x19, 0x65, 
        0x21, 0x57, 0xb4, 0x49, 0x88, 0xc5, 0x15, 0x67, 0x05, 0x95, 0xb5, 0xd7, 0x7b, 0xb9, 0x06, 0x1b, 
        0x20, 0x41, 0x38, 0xfc, 0x3a, 0x29, 0xd9, 0x23, 0x85, 0xc3, 0x49, 0x0a, 0x91, 0xdb, 0xcf, 0xe7, 
        0x07, 0x9f, 0x41, 0xb5, 0xe1, 0x99, 0xce, 0xe9, 0xd6, 0xb7, 0x71, 0xbb, 0x04, 0x25, 0x2b, 0xb4, 
        0xe1, 0x00, 0x74, 0xa8, 0xe7, 0x99, 0xff, 0x1b, 0x71, 0xd5, 0xd2, 0x06, 0xbb, 0x5b, 0xfa, 0xaf, 
        0xc3, 0x63, 0x14, 0xd0, 0x73, 0xf4, 0x62, 0x60, 0xe5, 0xce, 0x07, 0xcb, 0xbe, 0x7c, 0x81, 0xe1, 
        0x3f, 0x65, 0xf1, 0xe5, 0xbb, 0x6a, 0xa3, 0xab, 0x48, 0x5d, 0xf7, 0x41, 0x70, 0x7e, 0x0b, 0x4a, 
        0x44, 0x94, 0x27, 0x5f, 0x4a, 0x64, 0x13, 0x29, 0x8e, 0x2f, 0xd0, 0x26, 0x27, 0x1b, 0x6e, 0x14, 
        0x5f, 0xf6, 0x51, 0xec, 0x1a, 0x1b, 0xf4, 0xf9, 0xc6, 0xf9, 0x06, 0xed, 0x44, 0xc6, 0xc9, 0x77, 
        0x6a, 0x88, 0x3a, 0xff, 0x59, 0x79, 0x93, 0xb4, 0x6f, 0x63, 0x9a, 0x01, 0x26, 0xad, 0xda, 0xc0, 
        0xe0, 0x29, 0x5f, 0x48, 0x29, 0xbd, 0xe6, 0xee, 0xe0, 0x48, 0x1b, 0x87, 0x75, 0x8a, 0xe6, 0x87, 
        0xfc, 0x4f, 0x13, 0xd0, 0x88, 0xbb, 0xab, 0x32, 0x18, 0x80, 0xf4, 0x2c, 0x1f, 0x7c, 0xcc, 0x68, 
        0x03, 0xe8, 0xc6, 0x27, 0xaf, 0xb3, 0x80, 0xc0, 0xbf, 0x83, 0x4a, 0x8c, 0x41, 0xf1, 0xc3, 0x4c, 
        0x1d, 0x13, 0xa2, 0x01, 0x2b, 0x3b, 0x0d, 0x4d, 0x99, 0xd3, 0x59, 0x7b, 0x8a, 0xfc, 0x10, 0xe6, 
        0xc3, 0x69, 0xb8, 0x7a, 0x7c, 0x5d, 0x45, 0x50, 0x50, 0x3c, 0x30, 0x9e, 0x7f, 0xf7, 0x5e, 0xc9, 
        0x3b, 0x9f, 0xa0, 0x1b, 0x69, 0xba, 0x55, 0x54, 0x66, 0xdf, 0x64, 0xcf, 0x9e, 0x47, 0xd6, 0x6e, 
        0x89, 0xf5, 0xcc, 0x51, 0x20, 0x5e, 0xbe, 0x01, 0x0b, 0xea, 0x86, 0xc1, 0xec, 0xa0, 0xb4, 0x45, 
        0xa6, 0xfe, 0x17, 0x6d, 0xe2, 0x6e, 0x00, 0x1e, 0xf3, 0x50, 0xde, 0xbe, 0x45, 0xb1, 0x93, 0x4d, 
        0x4a, 0xbb, 0x5e, 0x83, 0x10, 0x14, 0x49, 0xac, 0xc1, 0x4c, 0xff, 0x38, 0x98, 0xd5, 0x0c, 0x2b, 
        0x36, 0x6b, 0xe6, 0xff, 0xd1, 0x05, 0xbf, 0x32, 0x6b, 0xd9, 0x89, 0x78, 0x14, 0x45, 0xbc, 0x3e, 
        0xcc, 0x98, 0xd6, 0x72, 0x7a, 0xb5, 0x8f, 0x46, 0x11, 0x5d, 0xd5, 0xcc, 0xa8, 0xa3, 0x98, 0xf8, 
        0xe9, 0xee, 0xa9, 0x53, 0x3f, 0x15, 0xc1, 0x38, 0xac, 0xd6, 0xfa, 0x36, 0x2d, 0x09, 0xd2, 0xb5, 
        0x47, 0x5d, 0xb3, 0xe3, 0x40, 0x4a, 0xd7, 0x84, 0x7b, 0x33, 0x78, 0x91, 0x3e, 0x82, 0x32, 0x68, 
        0xc6, 0xf1, 0x38, 0x10, 0xd9, 0x46, 0xc9, 0x51, 0xcb, 0x6d, 0xe4, 0x3d, 0x18, 0xa4, 0x8c, 0xce, 
        0xc6, 0xd1, 0x24, 0x27, 0xbf, 0x6f, 0x59, 0xd2, 0x7f, 0x75, 0xca, 0x7d, 0x44, 0x0a, 0x9a, 0x2e, 
        0xd8, 0x6e, 0x29, 0x5d, 0xdb, 0x62, 0x3c, 0x94, 0x79, 0x13, 0xf1, 0xcb, 0xca, 0x4a, 0x7b, 0x19, 
        0x1e, 0xc9, 0xa8, 0x76, 0x8b, 0x67, 0x01, 0x9f, 0x05, 0x05, 0xdf, 0xc0, 0x01, 0x9d, 0xf7, 0x78, 
        0x4a, 0xb9, 0x05, 0xf6, 0xe1, 0x05, 0x98, 0x6c, 0xba, 0x93, 0xd2, 0x23, 0xb2, 0x65, 0xa7, 0xae, 
        0xe2, 0x8e, 0xed, 0xa9, 0x98, 0xf7, 0x13, 0xee, 0xd9, 0xbd, 0x23, 0xa4, 0xad, 0x4e, 0x1c, 0xb4, 
        0x62, 0xf1, 0x12, 0xff, 0x65, 0xce, 0x0b, 0x23, 0xb6, 0xe5, 0x03, 0xe0, 0x55, 0xa3, 0x3c, 0xa3, 
        0xe2, 0x68, 0x2c, 0x88, 0x43, 0xde, 0xc4, 0x2a, 0xc2, 0x3f, 0x65, 0xb7, 0x69, 0xae, 0xde, 0xaf, 
        0x74, 0x4a, 0x4d, 0x5e, 0x49, 0xee, 0x10, 0xc1, 0x44, 0x92, 0xd2, 0x3e, 0xfc, 0xff, 0xe4, 0x6e, 
        0x9d, 0x74, 0x2e, 0x91, 0x64, 0x96, 0x6c, 0xbc, 0xef, 0xaf, 0xd6, 0x3f, 0x3c, 0x11, 0xb6, 0xb5, 
        0x3c, 0x09, 0xca, 0x23, 0x6a, 0xd7, 0xaf, 0x2f, 0xf3, 0xc4, 0xff, 0x04, 0x4a, 0x34, 0x0b, 0xda, 
        0xde, 0x40, 0x4d, 0x1f, 0x72, 0x60, 0x45, 0xc7, 0xff, 0x50, 0xe1, 0x49, 0x2d, 0x31, 0xa5, 0x4a, 
        0x53, 0x23, 0x36, 0x54, 0xc6, 0x33, 0xca, 0xa3, 0x41, 0x4c, 0x66, 0xba, 0x98, 0x18, 0x3f, 0x85, 
        0xc2, 0x1e, 0xdb, 0x73, 0x67, 0x24, 0x07, 0x9d, 0x41, 0x89, 0x5e, 0xcb, 0x05, 0xaa, 0x93, 0xc8, 
        0x8c, 0xc9, 0x88, 0x06, 0xcf, 0x24, 0xf6, 0x23, 0xd4, 0x1b, 0x7f, 0xfe, 0xfa, 0xe1, 0x8e, 0x26, 
        0x6b, 0xdb, 0x5d, 0x47, 0x5e, 0x48, 0x21, 0x02, 0x5e, 0x52, 0xd4, 0x06, 0xd2, 0xa3, 0xe6, 0xd2, 
        0x87, 0x17, 0xaf, 0x99, 0xa1, 0x12, 0xf2, 0x2f, 0xc4, 0xa1, 0xf7, 0x03, 0x82, 0x7e, 0x11, 0x2a, 
        0xf6, 0x7d, 0xce, 0x62, 0x27, 0x51, 0x4e, 0x22, 0x8b, 0x6e, 0xdf, 0xc7, 0xb4, 0x46, 0x3c, 0x6f, 
        0xcd, 0xdf, 0xe8, 0xe9, 0x62, 0xf0, 0x99, 0xb6, 0xcd, 0xf5, 0xb3, 0xdb, 0xa1, 0x95, 0x3e, 0x73, 
        0xde, 0xf2, 0xe7, 0x9e, 0xea, 0x94, 0x79, 0xdf, 0x1e, 0x29, 0x51, 0x5d, 0x27, 0xc1, 0x8a, 0x0e, 
        0xf2, 0x63, 0xed, 0x72, 0xd7, 0x83, 0x5b, 0x2b, 0x41, 0x25, 0x04, 0x91, 0x1b, 0x6d, 0x76, 0xa5, 
        0x1f, 0x29, 0xeb, 0x4c, 0x48, 0x69, 0x2a, 0x47, 0x27, 0x7b, 0x1d, 0xff, 0x05, 0x04, 0x53, 0x93, 
        0x1e, 0xd6, 0x86, 0x0a, 0x1a, 0xbd, 0x2b, 0x57, 0xb6, 0x19, 0x5d, 0x5d, 0xd1, 0xc7, 0x20, 0x93, 
        0xe9, 0xf4, 0xe6, 0x06, 0xb6, 0x4b, 0xaa, 0xaf, 0x55, 0x6d, 0x2e, 0xf7, 0x8c, 0x5f, 0xd9, 0x2f, 
        0x13, 0x34, 0x2c, 0x45, 0x11, 0x54, 0xf4, 0x56, 0x29, 0x2a, 0xcd, 0x28, 0x02, 0x69, 0x8b, 0xc3, 
        0xae, 0x38, 0x29, 0x25, 0xd5, 0xd9, 0x06, 0xc3, 0x31, 0x7a, 0x0c, 0xf3, 0x52, 0xf9, 0x1c, 0x10, 
        0xda, 0xde, 0xbd, 0x68, 0x6e, 0x43, 0xc2, 0xa0, 0xd9, 0x15, 0x43, 0xf5, 0xac, 0x31, 0xb4, 0xa3, 
        0x78, 0x9b, 0x2f, 0xf5, 0x5f, 0x90, 0xb1, 0x3b, 0x53, 0x03, 0xfa, 0x2c, 0xe2, 0x5e, 0x2f, 0x83, 
        0x08, 0x1c, 0xf8, 0x1c, 0x4c, 0xff, 0x21, 0x0d, 0x06, 0xb2, 0xcd, 0xf8, 0xf5, 0xb3, 0x35, 0x90, 
        0x43, 0xb9, 0xee, 0xf7, 0x4b, 0x35, 0x41, 0xe9, 0xd4, 0x2d, 0xf0, 0x61, 0x4e, 0xf1, 0x33, 0x81, 
        0x95, 0x4c, 0xb5, 0xd0, 0x8c, 0x3b, 0xf9, 0xae, 0xbc, 0xea, 0x33, 0xcd, 0x33, 0x06, 0x08, 0xc7, 
        0x37, 0xf5, 0x7c, 0xdd, 0xd2, 0x19, 0xcc, 0x2c, 0xa9, 0x35, 0x82, 0x51, 0x52, 0xd5, 0x15, 0xb7, 
        0x82, 0x55, 0x07, 0x1a, 0x32, 0x7e, 0xc5, 0x87, 0xc7, 0x48, 0x8f, 0x65, 0x63, 0x21, 0xa8, 0xd8, 
        0x41, 0x7f, 0xae, 0xca, 0xbd, 0xb0, 0x03, 0x8c, 0xe7, 0x17, 0x13, 0x79, 0xb3, 0x14, 0x25, 0x8d, 
        0x08, 0xa8, 0x45, 0xc6, 0xc1, 0xac, 0xef, 0x95, 0x7e, 0x14, 0x73, 0xb3, 0xaf, 0x66, 0x44, 0xf3, 
        0xc8, 0xd9, 0xaf, 0xdf, 0xd4, 0x24, 0xd3, 0xb1, 0x13, 0x69, 0x3c, 0xee, 0xe3, 0x83, 0x74, 0xf5, 
        0xc3, 0xb4, 0x00, 0x2e, 0xf0, 0x0b, 0x0f, 0x8e, 0x9c, 0xd3, 0xb0, 0x08, 0xa5, 0x1f, 0x5e, 0x54, 
        0x03, 0xb7, 0x74, 0x66, 0xd3, 0x30, 0xa7, 0x6b, 0x64, 0x15, 0xbc, 0x0e, 0xe7, 0xf1, 0x8d, 0x7f, 
        0xf4, 0x85, 0x34, 0xf2, 0xe9, 0x78, 0x68, 0x5e, 0x40, 0xab, 0xd8, 0xbc, 0x0a, 0x0f, 0x63, 0x59, 
        0x8d, 0xfe, 0x26, 0x87, 0xc5, 0xc5, 0x44, 0x58, 0x45, 0x4a, 0xe3, 0x59, 0x7d, 0x28, 0xc0, 0xbe, 
        0x3a, 0x49, 0x9d, 0xce, 0x56, 0xae, 0xbc, 0x6f, 0x2e, 0x1c, 0x6a, 0xa7, 0x4f, 0xd3, 0xf4, 0x09, 
        0x3f, 0x8c, 0xc7, 0x91, 0x17, 0x5c, 0xd7, 0x2e, 0xb5, 0xbf, 0xd8, 0x01, 0x65, 0xda, 0x4f, 0xd9, 
        0xa7, 0xbb, 0x6f, 0x5f, 0xe4, 0x05, 0xaa, 0xc4, 0x1a, 0xd1, 0x9c, 0x1a, 0x90, 0xce, 0xbf, 0xcd, 
        0xfd, 0x8d, 0x92, 0x98, 0xa9, 0x5d, 0xd9, 0xd0, 0x7d, 0x64, 0xb5, 0x25, 0x5d, 0x59, 0x2d, 0x94, 
        0x52, 0x63, 0xa5, 0x6b, 0x2a, 0xa3, 0xb0, 0x5d, 0x4e, 0x74, 0x51, 0x38, 0x97, 0x1c, 0xf4, 0xf2, 
        0xb0, 0x1f, 0x83, 0xa1, 0x5e, 0x55, 0x3a, 0x8b, 0x59, 0x86, 0x2d, 0x6e, 0x7f, 0x2e, 0x1f, 0xbb, 
        0xd1, 0x78, 0x7f, 0xb9, 0xfb, 0x79, 0x03, 0xe1, 0x5b, 0x74, 0x51, 0xf0, 0xd9, 0x28, 0x86, 0xba, 
        0x18, 0x73, 0xcf, 0xab, 0xa8, 0x6f, 0x23, 0x68, 0x90, 0x15, 0x6b, 0x74, 0x7a, 0x74, 0x46, 0xec, 
        0xc1, 0x51, 0x04, 0x17, 0xf5, 0xf0, 0x77, 0x1e, 0x1b, 0xe2, 0x80, 0xb5, 0x62, 0xbc, 0xb5, 0x5a, 
        0xec, 0x99, 0x02, 0x76, 0x39, 0xb4, 0x30, 0x77, 0x6f, 0xe1, 0xc1, 0x23, 0xbf, 0x2f, 0x26, 0x83, 
        0x91, 0x59, 0x32, 0x4c, 0x80, 0x17, 0xa6, 0x44, 0x50, 0x89, 0x0f, 0x0d, 0xb9, 0x90, 0x44, 0x55, 
        0x8f, 0xe4, 0x24, 0x5e, 0x56, 0x06, 0x70, 0xd6, 0xbc, 0x68, 0xb5, 0x76, 0xde, 0xff, 0x98, 0xcf, 
        0x0e, 0xbb, 0xc8, 0xf5, 0x8f, 0xa6, 0x04, 0x8c, 0x07, 0x2d, 0x55, 0x8e, 0xe3, 0x7b, 0x8e, 0xca, 
        0xf5, 0xcc, 0x43, 0x65, 0x36, 0x2c, 0x30, 0xd2, 0x23, 0x41, 0x16, 0x3b, 0xbf, 0x5a, 0xc6, 0xb3, 
        0xe4, 0xf1, 0x16, 0xba, 0x84, 0xec, 0x5e, 0x7e, 0x57, 0x0c, 0x28, 0x85, 0xd7, 0xad, 0xe0, 0x68, 
        0x6a, 0x67, 0xeb, 0x81, 0x45, 0x55, 0x02, 0x64, 0xe0, 0x7b, 0x8b, 0x8c, 0xa3, 0x57, 0x0f, 0x28, 
    };
const uint8_t bunbk[] = 
    {
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x5d, 0xed, 0xa7, 0x2f, 0x09, 0xb1, 0x8c, 0x59, 0xce, 0xe0, 0x6c, 0xeb, 0x42, 0xb3, 0x99, 0xae, 
        0x8a, 0x84, 0x3a, 0xb0, 0x56, 0x2e, 0xd7, 0x61, 0xe7, 0x32, 0x5d, 0xd2, 0x28, 0x88, 0x97, 0x92, 
        0x0b, 0x5b, 0x15, 0x35, 0xd5, 0x3d, 0xe7, 0xb9, 0x9f, 0x4e, 0x29, 0x6d, 0xf1, 0xfa, 0x91, 0xf0, 
    };

 unsigned char bkeyDecoded[32*60];


unsigned long drmDataLoadGetBundlingId( )
{
	return bid;
}

int drmDataLoadGetPublicKey( uint8_t* publicKeyE,
                             uint8_t* publicKeyN )
{
    uint8_t ctk[32] = 
    {
        0x6d, 0x11, 0x12, 0x05, 0x82, 0x19, 0x08, 0x07, 0xc7, 0xbf, 0x31, 0x1d, 0x51, 0x78, 0x29, 0xac, 
        0x20, 0x63, 0x44, 0x7a, 0xb4, 0xd0, 0x6e, 0xa6, 0xf0, 0xf2, 0x0b, 0x4e, 0xa5, 0x10, 0xd6, 0x60, 
     };
    /*
    uint8_t bctk[32] = 
    {
        0x9e, 0x31, 0x87, 0xb6, 0x1c, 0x71, 0x21, 0x37, 0x08, 0x10, 0x62, 0xaf, 0xb2, 0x70, 0xc5, 0x2f, 
        0xf3, 0x00, 0x62, 0xc6, 0xfc, 0x7c, 0x7f, 0x1b, 0x51, 0x73, 0x43, 0x00, 0x7d, 0x70, 0x2c, 0x23, 
     };
     */

    uint8_t e[2] = 
    {
        0xf0, 0x89, 
     };

    uint8_t n[256] = 
    {
        0x5b, 0xed, 0xbf, 0x8c, 0xbe, 0x4c, 0x88, 0x2b, 0xc5, 0xaa, 0x7d, 0xec, 0xc8, 0xf8, 0xb6, 0x96, 
        0xf5, 0xf8, 0xb1, 0xec, 0xd7, 0x02, 0x46, 0xc9, 0x32, 0x25, 0xa9, 0x74, 0xbd, 0xfd, 0xb2, 0x28, 
        0xbb, 0x08, 0xf3, 0x9b, 0x80, 0x0a, 0x7c, 0x81, 0xd9, 0xe5, 0x28, 0x34, 0xb3, 0xb9, 0x77, 0xa7, 
        0x6e, 0x28, 0xbb, 0xf9, 0xd0, 0x70, 0xb5, 0xb8, 0xf8, 0xe6, 0x5b, 0xd5, 0x65, 0xb8, 0x43, 0x1f, 
        0x79, 0x8e, 0x34, 0x1b, 0xea, 0xc3, 0x66, 0x5c, 0x65, 0x77, 0x06, 0x3d, 0x22, 0xb2, 0x8b, 0xc0, 
        0x97, 0xdc, 0x0e, 0x2e, 0x4f, 0x5d, 0x55, 0xf6, 0x45, 0xd2, 0x89, 0xa5, 0x81, 0xd8, 0x3b, 0x29, 
        0xc4, 0x84, 0x2b, 0xa3, 0x44, 0x7d, 0xe5, 0x2c, 0x69, 0xbb, 0xaf, 0xa6, 0x16, 0xd0, 0x2f, 0xea, 
        0x45, 0xd8, 0x55, 0x3e, 0x39, 0x7c, 0xeb, 0xc2, 0xac, 0x07, 0xd5, 0xd1, 0x99, 0xac, 0xe3, 0xf6, 
        0x15, 0xb4, 0x43, 0x30, 0x6e, 0xa6, 0xa8, 0x07, 0xa7, 0x28, 0xca, 0xf8, 0x32, 0xaf, 0x96, 0xa9, 
        0x5d, 0x08, 0x75, 0xf1, 0xcd, 0x99, 0x45, 0x85, 0x93, 0x19, 0xe0, 0xe4, 0x0a, 0x94, 0x68, 0x85, 
        0xcb, 0x9b, 0x18, 0x49, 0x10, 0xc6, 0x7a, 0xc8, 0x85, 0xee, 0x0b, 0x5c, 0x15, 0xfe, 0x34, 0x80, 
        0x15, 0x7c, 0x21, 0x14, 0x1c, 0x44, 0x3e, 0xae, 0xdb, 0xcd, 0x3a, 0x74, 0x5d, 0xac, 0x0a, 0x42, 
        0xd4, 0x2f, 0x53, 0xba, 0x56, 0xa4, 0xe7, 0x70, 0x87, 0x9f, 0x93, 0xca, 0x63, 0x60, 0x8e, 0xe1, 
        0xbe, 0x92, 0xb2, 0x68, 0x5d, 0x4b, 0x8f, 0x7e, 0xba, 0xad, 0x45, 0x0b, 0xa4, 0xd0, 0x81, 0xf1, 
        0x49, 0xad, 0xfb, 0xd5, 0xff, 0x6c, 0xcd, 0xa0, 0xd4, 0x38, 0x76, 0x14, 0x9c, 0x9f, 0x2d, 0x5d, 
        0xbe, 0xd7, 0x09, 0x37, 0xa2, 0x8a, 0x11, 0x9f, 0x4d, 0x5a, 0x61, 0x53, 0xc5, 0x5e, 0x79, 0x49, 
     };


    /* Unscramble the key entry */
    {
        aesKeyHandle keyHandle;
        unsigned char pkScratch[256];
        unsigned char pkDecoded[256];
        unsigned char ctkDecoded[32];
#if DRM_DATALOAD_USE_LHS == 1
        unsigned char lhs[32];
        uint32_t i;
#endif
        unsigned long pkDecodedLength = 256;
        memcpy( pkScratch,
                n,
                sizeof(pkScratch) );
        memcpy( ctkDecoded,
                ctk,
                sizeof(ctkDecoded) );

        compileTimeKeyScrambleOut( ctkDecoded,
                                   sizeof(ctkDecoded) );
#if DRM_DATALOAD_USE_LHS == 1
        localGetLocalHardwareSecret( lhs );
        for ( i = 0; i < 32; i++ )
        {
           ctkDecoded[i] ^= lhs[i];
        }
#endif

        DivXAesCreateKey( &keyHandle, 
                          aesKeySize32Bytes,
                          ctkDecoded );
        DivXAesECBDecrypt( keyHandle,
                           pkScratch,
                           sizeof(pkScratch),
                           pkDecoded,
                           (uint32_t *)&pkDecodedLength );
        DivXAesDeleteKey( keyHandle );
        publicKeyScrambleOut( pkDecoded,
                              sizeof(pkDecoded) );

        memcpy( publicKeyE,
                e,
                2 );
        memcpy( publicKeyN,
                pkDecoded,
                256 );
        memset( pkScratch,
                0xCC,
                sizeof(pkScratch) );
        memset( pkDecoded,
                0xCC,
                sizeof(pkDecoded) );
    }
    return 0;
}

// ks 3.1
const char keyIdTable[60][45] = 
    {
    "68350122382077155371609790526077843323069757", 
    "17526956348125519278333379155775791404842465", 
    "07307313514643969633523034904779465997361500", 
    "54733977128285238605431520951629624437529331", 
    "95271872508096602223778323164547956766606317", 
    "19667620404347912572341155491659903450839945", 
    "58243572270469309545831683269013152299633661", 
    "48838714380920672270656324638255297385378768", 
    "06784853705815447409483489174064856573505938", 
    "00770111886686676706976265862940452724180617", 
    "53815691764967922077658113912876220915162495", 
    "04312177729617650695571887594358724097271608", 
    "63945301141543851075872730428987261348525394", 
    "66414933091815151592855735169031788977421980", 
    "02141859567992995294356684725134005814458843", 
    "63206695224090071797749557616551851913902367", 
    "32166867332190936760942334059749874131461526", 
    "91088194239951573940293382004345521751844727", 
    "92197528495960049416305937361261639677050840", 
    "17330240856107841543438997133983009913947010", 
    "13259619259730801548398619739515974326438404", 
    "66345463236824977876404683416022743228556287", 
    "91553349920876007590411937535009683049432052", 
    "79786200184216186882037198523609202442582284", 
    "31155661106773667020894658514427212212186554", 
    "02574728702394599625389263039554984837302983", 
    "47704049973879049017481401823158168936109838", 
    "33181267047034345884861232414177923873921402", 
    "95712790986433222039238508365438234687799024", 
    "87434633147336734561911836670412892584701219", 
    "46772667241777182717868628890966442544385406", 
    "49035216358942073160728495530141596252425411", 
    "44508642441058233617170196497655845865406903", 
    "99055669180276878044689398437906169977933251", 
    "03813755964652239163393926083853548089623593", 
    "28939308329163842853794464938127049670250679", 
    "59244270204420534452424052558003457077094806", 
    "55566801756599491237873714262460883030792124", 
    "83587403621836396861992555063439857575195580", 
    "52547543021062104974446625289907080736360596", 
    "91412179475374455877131236071755329986548065", 
    "32870575011359671262202908573719122311244667", 
    "37870178042902037285821789276709322633955330", 
    "81623869174821026392711321648697760125077043", 
    "77175978978407074183671384422084867582512807", 
    "70707855331253054607736514670342511748550476", 
    "30739520204616807107902904407700118418437529", 
    "93237679788650327314041572175858788454649358", 
    "20003680192182883691364474798010405378792219", 
    "24911149782112544619931610657653650768180246", 
    "64756654061975392234341568603967457924664286", 
    "28369458320179089501859698186278323861884704", 
    "77085984738317849374167555264822524492814026", 
    "13179997570629766352239598502386239172948199", 
    "93683866498185774193545671187072456445691852", 
    "13371809450093784565001999346455108800080097", 
    "74639151292922765147004097208050988050587443", 
    "68166662441223935918083060322500394517228908", 
    "39287903616748935672781132639227679536993421", 
    "77658077013762464610390453611281332768339559", 
    };
// Vendor Bundling 
const char bundlingKeyIdTable[60][45] = 
    {
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    "", 
    };


int drmDataLoadGetBaseKey( uint8_t* baseKeyIndexString,
                           uint8_t* baseKey )
{

    uint8_t ctk[32] = 
    {
        0x6d, 0x11, 0x12, 0x05, 0x82, 0x19, 0x08, 0x07, 0xc7, 0xbf, 0x31, 0x1d, 0x51, 0x78, 0x29, 0xac, 
        0x20, 0x63, 0x44, 0x7a, 0xb4, 0xd0, 0x6e, 0xa6, 0xf0, 0xf2, 0x0b, 0x4e, 0xa5, 0x10, 0xd6, 0x60, 
     };
    uint8_t bctk[32] = 
    {
        0x9e, 0x31, 0x87, 0xb6, 0x1c, 0x71, 0x21, 0x37, 0x08, 0x10, 0x62, 0xaf, 0xb2, 0x70, 0xc5, 0x2f, 
        0xf3, 0x00, 0x62, 0xc6, 0xfc, 0x7c, 0x7f, 0x1b, 0x51, 0x73, 0x43, 0x00, 0x7d, 0x70, 0x2c, 0x23, 
     };

    int32_t i = 0;
    int32_t index = 0;
	int32_t bundlingKey = 0;

#if DRM_GO_LIVE_BASEKEYS == 0

    if ( strncmp( keyIdTable[59],
                  (const char *)baseKeyIndexString,
                  44 ) == 0 )
    {
        index = 59;
    }
    else
    {
		for ( i = 0; i < 60; i++ )
		{
			if ( strncmp( bundlingKeyIdTable[i],
						  (const char *)baseKeyIndexString,
						  44 ) == 0 )
			{
				index = i;
				bundlingKey = 1;
				break;
			}
		}
		if ( i == 60 )
        {
            return -1;
        }
    }

#else

    /* Find key */
    for ( i = 0; i < 60; i++ )
    {
        if ( strncmp( keyIdTable[i],
                      (const char *)baseKeyIndexString,
                      44 ) == 0 )
        {
            index = i;
            break;
        }
    }
    if ( i == 60 )
    {
		for ( i = 0; i < 60; i++ )
		{
			if ( strncmp( bundlingKeyIdTable[i],
						  (const char *)baseKeyIndexString,
						  44 ) == 0 )
			{
				index = i;
				bundlingKey = 1;
				break;
			}
		}
		if ( i == 60 )
        {
            return -1;
        }
    }

#endif

    /* Unscramble the key entry */
    {
        aesKeyHandle keyHandle;
        //unsigned char bkeyDecoded[32*60];
        unsigned long bkeyDecodedLength = 32*60;
        unsigned char ctkDecoded[32];
#if DRM_DATALOAD_USE_LHS == 1
        unsigned char lhs[32];
        uint32_t i = 0;
#endif

		if ( bundlingKey == 1 )
		{
			memcpy( ctkDecoded,
					bctk,
					sizeof(ctkDecoded) );
			bundleCompileTimeKeyScrambleOut( ctkDecoded,
											 sizeof(ctkDecoded) );
		}
		else
		{
			memcpy( ctkDecoded,
					ctk,
					sizeof(ctkDecoded) );
			compileTimeKeyScrambleOut( ctkDecoded,
									   sizeof(ctkDecoded) );
		}
		


#if DRM_DATALOAD_USE_LHS == 1
        localGetLocalHardwareSecret( lhs );
        for ( i = 0; i < 32; i++ )
        {
           ctkDecoded[i] ^= lhs[i];
        }
#endif

        DivXAesCreateKey( &keyHandle, 
                          aesKeySize32Bytes,
                          ctkDecoded );
		if ( bundlingKey == 1 )
		{
			DivXAesECBDecrypt( keyHandle,
				               bunbk,
					           sizeof(bunbk),
						       bkeyDecoded,
							   (uint32_t *)&bkeyDecodedLength );
		}
		else
		{
			DivXAesECBDecrypt( keyHandle,
				               bk,
					           sizeof(bk),
						       bkeyDecoded,
							   (uint32_t *)&bkeyDecodedLength );
		}
        DivXAesDeleteKey( keyHandle );
        baseKeyScrambleOut( bkeyDecoded,
                            sizeof(bkeyDecoded) );
        memcpy( baseKey,
                bkeyDecoded + (index * 32),
                32 );
        memset( bkeyDecoded,
                0xCC,
                sizeof(bkeyDecoded) );
    }
    return 0;

}

int drmDataLoadGetRTK( uint8_t* rtk )
{

#if DRM_GO_LIVE == 0
    memset( rtk,
            0xA5,
            32 );
    return 0;
#else

    uint32_t hashLength = 0;
    uint8_t uniqueDataString[256] = { 0 };
    uint32_t uniqueDataStringLength = 0;
#if DRM_DATALOAD_USE_LHS == 1
    uint8_t lhs[32];
    uint32_t lhsIdx = 0;
#endif

/*** OTHER SECURE Platforms ***/
#if DRM_OTHER_SECURE_PLATFORM == 1
    uint8_t rtkStatic[32] = 
    {
        0xfc, 0xdc, 0xfe, 0x7f, 0xbc, 0xba, 0xef, 0x4b, 0xfb, 0xc1, 0xdc, 0x4e, 0xf2, 0xe4, 0x51, 0x1e, 
        0xc7, 0xad, 0x9b, 0xba, 0x1b, 0x8e, 0xde, 0xac, 0x4b, 0x92, 0xe9, 0xab, 0x3f, 0xc3, 0xa6, 0x7d, 
     };
    memcpy( rtk,
            rtkStatic,
            32 );
    memset( uniqueDataString,
            0xA5,
            sizeof(uniqueDataString) );
#endif

/*** WIN32 and LINUX DESKTOP Platforms ***/
#if DRM_DESKTOP_WINDOWS_PLATFORM == 1 || DRM_DESKTOP_LINUX_PLATFORM == 1
    uint32_t length = 0;
    if ( localGetMacAddress( uniqueDataString,
                             &length ) != 0 )
    {
        return -1;
    }
    uniqueDataStringLength += length;
    if ( localGetProcessorString( uniqueDataString + length,
                                  &length ) != 0 )
    {
        return -1;
    }
    if ( localGetBootDriveName( uniqueDataString + length,
                                &length ) != 0 )
    {
        return -1;
    }
    uniqueDataStringLength += length;
#endif

/*** DRM Mobile Platforms ***/
#if DRM_MOBILE_WINDOWS_PLATFORM == 1 || DRM_MOBILE_LINUX_PLATFORM == 1 || DRM_MOBILE_SYMBIAN_PLATFORM == 1

/* Get the unique data elements here */
#if DRM_MOBILE_WINDOWS_PLATFORM == 1

    uint32_t length = 0;
    if ( localGetSerial( uniqueDataString,
                         &length ) != 0 )
    {
        return -1;
    }
    uniqueDataStringLength += length;
    if ( localGetRevision( uniqueDataString + length,
                           &length ) != 0 )
    {
        return -1;
    }
    uniqueDataStringLength += length;
    if ( localGetManufacturer( uniqueDataString + length,
                               &length ) != 0 )
    {
        return -1;
    }
    uniqueDataStringLength += length;

#endif

#if DRM_MOBILE_SYMBIAN_PLATFORM == 1

    uint32_t length = 0;
    if ( localGetIMEI( uniqueDataString,
                       &length ) != 0 )
    {
        return -1;
    }
    uniqueDataStringLength += length;
    if ( localGetMachineId( uniqueDataString + length,
                            &length ) != 0 )
    {
        return -1;
    }
    uniqueDataStringLength += length;

#endif

#endif

    /* Check to make sure that the data string was
       even populated by something */
    {
        int32_t i = 0;
        uint32_t sum = 0;
        for ( i = 0; i < sizeof(uniqueDataString); i++ )
        {
           sum += uniqueDataString[i];
        }
        if ( sum == 0 )
        {
            return -1;
        }
    }

    /* Hash the data string */
    hashLength = 32;
    DivXSha256( uniqueDataString,
                uniqueDataStringLength,
                rtk,
                &hashLength );

#if DRM_DATALOAD_USE_LHS == 1
    localGetLocalHardwareSecret( lhs );
    for ( lhsIdx = 0; lhsIdx < 32; lhsIdx++ )
    {
        rtk[lhsIdx] ^= lhs[lhsIdx];
    }
#endif

    return 0;

#endif

}

int drmDataLoadSaveDrmMemory( uint8_t* marshalledMemory,
                              uint32_t marshalledMemoryLength )
{

    uint8_t rtk[32];
    aesKeyHandle keyHandle;
    uint8_t encryptedMemory[PACKED_ALLOCATION_BYTES];
    uint32_t encryptedMemoryLength = PACKED_ALLOCATION_BYTES;

    if ( encryptedMemoryLength != marshalledMemoryLength )
    {
        return -1;
    }

    if ( drmDataLoadGetRTK( rtk ) != 0 )
    {
        return -1;
    }

    drmMemoryScrambleIn( marshalledMemory,
                         marshalledMemoryLength );
    
    DivXAesCreateKey( &keyHandle, 
                      aesKeySize32Bytes,
                      rtk );
    DivXAesECBEncrypt( keyHandle,
                       marshalledMemory,
                       marshalledMemoryLength,
                       encryptedMemory,
                       &encryptedMemoryLength );

    if ( encryptedMemoryLength != PACKED_ALLOCATION_BYTES )
    {
        return -1;
    }
    DivXAesDeleteKey( keyHandle );

    memset( rtk,
            0xCC,
            sizeof(rtk) );

#if DRM_OTHER_SECURE_PLATFORM == 1

    if ( localSaveDrmMemoryFragment1( encryptedMemory,
                                      PACKED_ALLOCATION_BYTES ) != 0 )
    {
        return -1;
    }

#else

    if ( localSaveDrmMemoryFragment1( encryptedMemory,
                                      20 ) != 0 )
    {
        return -1;
    }

    if ( localSaveDrmMemoryFragment2( encryptedMemory + 20,
                                      50 ) != 0 )
    {
        return -1;
    }

    if ( localSaveDrmMemoryFragment3( encryptedMemory + 70,
                                      10 ) != 0 )
    {
        return -1;
    }

#endif

    return 0;
}

int drmDataLoadLoadDrmMemory( uint8_t* marshalledMemory,
                              uint32_t marshalledMemoryLength )
{

    uint8_t rtk[32];
    aesKeyHandle keyHandle;
    uint8_t encryptedMemory[PACKED_ALLOCATION_BYTES];
    uint32_t encryptedMemoryLength = PACKED_ALLOCATION_BYTES;

    if ( encryptedMemoryLength != marshalledMemoryLength )
    {
        return -1;
    }

#if DRM_OTHER_SECURE_PLATFORM == 1

    if ( localLoadDrmMemoryFragment1( encryptedMemory,
                                      PACKED_ALLOCATION_BYTES ) != 0 )
    {
        return -1;
    }

#else

    if ( localLoadDrmMemoryFragment1( encryptedMemory,
                                      20 ) != 0 )
    {
        return -1;
    }

    if ( localLoadDrmMemoryFragment2( encryptedMemory + 20,
                                      50 ) != 0 )
    {
        return -1;
    }

    if ( localLoadDrmMemoryFragment3( encryptedMemory + 70,
                                      10 ) != 0 )
    {
        return -1;
    }

#endif

    if ( drmDataLoadGetRTK( rtk ) != 0 )
    {
        return -1;
    }

    DivXAesCreateKey( &keyHandle, 
                      aesKeySize32Bytes,
                      rtk );
    DivXAesECBDecrypt( keyHandle,
                       encryptedMemory,
                       encryptedMemoryLength,
                       marshalledMemory,
                       &encryptedMemoryLength );
    if ( encryptedMemoryLength != PACKED_ALLOCATION_BYTES )
    {
        return -1;
    }
    DivXAesDeleteKey( keyHandle );

    drmMemoryScrambleOut( marshalledMemory,
                          marshalledMemoryLength );
    memset( rtk,
            0xCC,
            sizeof(rtk) );
    return 0;
}

#if 1
extern uint8_t gui1_divx_drm_model_id_low_byte;
extern uint8_t gui1_divx_drm_model_id_high_byte;
#endif

int drmDataLoadModelId( uint8_t* modelId )
{
#if 0
    uint8_t mid[] = 
    {
        0x31, 0x30, 
    };
#else
    uint8_t mid[] = 
    {
        gui1_divx_drm_model_id_low_byte, gui1_divx_drm_model_id_high_byte, 
    };
#endif

    memcpy( modelId,
            mid,
            sizeof(mid) );
    return 0;

}

int drmDataLoadProfile( void )
{
    return DRM_DIVX_DL_PROFILE_HIGHDEF;
}
