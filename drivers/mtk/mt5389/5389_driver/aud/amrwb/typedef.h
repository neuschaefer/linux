/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/
/*******************************************************************************
 *
 * Filename:
 * ---------
 *   TYPEDEF.H
 *
 * Project:
 * --------
 *   DUMA
 *
 * Description:
 * ------------
 *   AMR-WB codec
 *
 * Author:
 * -------
 *   Eddy Wu
 *
 *------------------------------------------------------------------------------
 * $Revision: #2 $ 1.0.0
 * $Modtime:$
 * $Log:$
 *
 * 02 21 2011 richie.hsieh
 * [WCPSP00000575] [IPComm] SWIP protection
 * .
 *
 *
 *******************************************************************************/


#ifndef typedef_h
#define typedef_h "$Id $"

#if !defined(__CC_ARM)
// avoid nucleaus redefinition warning
#define __CC_ARM	0
#endif

#if !defined(__TARGET_FEATURE_DSPMUL)
// avoid nucleaus redefinition warning
#define __TARGET_FEATURE_DSPMUL 0
#endif

//#define INLINE      extern // dtv mtk platform does not support inline function defined in header

#ifdef  CC_MTK_PLATFORM
#include "x_typedef.h"
#include "x_assert.h"

#define assert ASSERT

typedef INT8  Word8;
typedef UINT8 UWord8;
typedef UINT8 UChar;
typedef INT16 Word16;
typedef UINT16 UWord16;
typedef INT32 Word32;
typedef UINT32 UWord32;
typedef INT32 Flag;

#else   // CC_MTK_PLATFORM
typedef signed char Word8;
typedef short Word16;
typedef int Word32;
//typedef int Flag;
#endif // CC_MTK_PLATFORM

#if __CC_ARM
typedef long long Word64;
#elif defined(CC_MTK_PLATFORM)
typedef INT64   Word64;
#else
typedef __int64   Word64;
#endif
#define lo64(a) (((unsigned*) &a)[0])    /* low 32 bits of a long long */
#define hi64(a) (((int*) &a)[1])        /* high 32 bits of a long long */

#define ARMDSP_WORD32_MAX ((Word32)0x7FFFFFFF)
#define ARMDSP_WORD32_MIN ((Word32)0x80000000)
#define ARMDSP_WORD16_MAX ((Word16)0x7FFF)
#define ARMDSP_WORD16_MIN ((Word16)0x8000)

/* Eddy 2008.06.23 define data types */
typedef union {
    unsigned int armdsp_flags_word;
    struct {
#ifdef __BIG_ENDIAN
        Flag armdsp_n:1, armdsp_z:1, armdsp_c:1, armdsp_v:1, armdsp_q:1, armdsp_unused:27;
#else
        Flag armdsp_unused:27, armdsp_q:1, armdsp_v:1, armdsp_c:1, armdsp_z:1, armdsp_n:1;
#endif /* __BIG_ENDIAN */
    } armdsp_bitfields;
} awb_armdsp_flagdata_union;

extern awb_armdsp_flagdata_union armdsp_flagdata_AWB;


#endif

