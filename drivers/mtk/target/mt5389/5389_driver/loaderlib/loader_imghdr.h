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
#ifndef LOADER_IMGHDR_H
#define LOADER_IMGHDR_H

/*--------------------------------------------------------------------------*/
/* Common part of loader/packing */

/* loader version 0.2v */
#define IMAGE_SIGNATURE                 0x35333958  /* "539X" */
#define IMAGE_SIGNATURE_UPGRADE         0x35333950  /* "539P" */

#ifndef CC_LOADER_MEM_OFFSET
    #define CC_LOADER_MEM_OFFSET            (0xd00000)
#endif /* CC_LOADER_MEM_OFFSET */

#ifdef CC_ROM_BOOT
    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5860)
        #define PRELOADER_MAX_SIZE              0x0000A000
    #else
        #ifdef CC_SECURE_BOOT
            #define PRELOADER_MAX_SIZE              0x00004000  /* 16 KB = 15KB + key*/
        #else
            #ifdef CC_MT5387
                #define PRELOADER_MAX_SIZE              0x00003800  /* 14 KB */
            #else
                #define PRELOADER_MAX_SIZE              0x00003c00  /* 15 KB */
            #endif
        #endif //CC_SECURE_BOOT
    #endif //5368/5396
#else
    #define PRELOADER_MAX_SIZE              0x00008000  /* 32 KB */
#endif //CC_ROM_BOOT

#ifdef LOADER_USB_UPGRADE
    #define USB_STACK_SIZE              0x10000 /*64 KB*/
#else
    #define USB_STACK_SIZE              0
#endif

#ifdef CC_LOAD_UBOOT
    #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389) || defined(CC_MT5398) || defined(CC_MT5880) || defined(CC_MT5860)
        #define LOADER_MAX_SIZE                 (0x00080000) /* 512 KB */
    #else
        #define LOADER_MAX_SIZE                 (0x00040000) /* 256 KB */
    #endif
#else
    #ifndef LOADER_MAX_SIZE
        #ifndef CC_UBOOT
            #define LOADER_MAX_SIZE                 (0x00030000 + USB_STACK_SIZE) /* 192 KB */
        #else
            #define LOADER_MAX_SIZE                 (0x00030000 + USB_STACK_SIZE) /* 192 KB */
        #endif
    #endif /* LOADER_MAX_SIZE */
#endif

#define METHOD_PLAIN_TEXT               0
#define METHOD_DEXOR_TEXT               1
#define METHOD_DECRYPT_TEXT             2

#define COMPRESS_TYPE_NO_COMPRESS       0
#define COMPRESS_TYPE_GZIP              1
#define COMPRESS_TYPE_LZHS              2

typedef struct
{
     UINT32 u4Signature;
     UINT32 u4Size;
     UINT32 fgCompress;
     UINT32 u4FirmwareExecAddr;
     UINT32 u4Method;
     UINT32 u4Key;
     UINT32 u4FirmwareChksum;
     UINT32 u4PQDataAddr;
     UINT32 u4Reserved4;
     UINT32 u4Reserved5;
     UINT32 u4Reserved6;
     UINT32 u4Reserved7;
     UINT32 u4Reserved8;
     UINT32 u4Reserved9;
     UINT32 u4ReservedA;
     UINT32 u4ReservedB;
} IMAGE_HEADER_T;

typedef struct
{
     UINT32 u4Signature;
     UINT32 u4Size;
     UINT32 fgCompress;
     UINT32 u4FirmwareExecAddr;
     UINT32 u4Method;
     UINT32 u4Key;
     UINT32 u4FirmwareChksum;
     UINT32 u4PQDataAddr;
     UINT8 au1CustomerId[16];
     UINT8 au1ModelName[32];
     UINT8 au1Version[64];
} IMAGE_UPGRADE_HEADER_T;

/*---------------------------------------------------------------------
 * Type definitions
 *---------------------------------------------------------------------*/

typedef void (*PFN_IMAGE)(void);

#endif /* LOADER_IMGHDR_H */
