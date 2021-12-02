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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fcntl.h>
#include <stdio.h>
#include <string.h>

typedef unsigned int UINT32;
typedef unsigned short UINT16;
typedef int INT32;
typedef short INT16;
typedef char UINT8;

#include "loader_imghdr.h"

#define BUFFER_SIZE                     0x00100000  /* 1MB */

static char _achBuf[BUFFER_SIZE];

int main(int argc, char **argv)
{
    int fdloader, fddiag, fdtarget, i4_len, i4_ret, i4_cnt;
    IMAGE_HEADER_T rImgHdr;
    struct stat st;

    if (argc != 4) {
        printf("%s [preloader] [loader] [target]\n", argv[0]);
        return 1;
    }

    /* open loader, diag, and target. */
    fdloader = open(argv[1], O_RDONLY);
    fddiag = open(argv[2], O_RDONLY);
    fdtarget = open(argv[3], O_CREAT | O_WRONLY | O_TRUNC, 0644);

    if ((fdloader < 0) || (fddiag < 0) || (fdtarget < 0)) {
        printf("%s, %s, or %s cannot be opened.\n", argv[1], argv[2], argv[3]);
        if (fdloader >= 0) { close(fdloader); }
        if (fddiag >= 0) { close(fddiag); }
        if (fdtarget >= 0) { close(fdtarget); }
        return 1;
    }

    i4_len = 0;
    memset(_achBuf, 0xff, BUFFER_SIZE);
    i4_ret = read(fdloader, _achBuf, BUFFER_SIZE);
    i4_len = ((i4_ret + 0x3f) & 0xffffffc0);
    printf("preloader size: 0x%08x, partition: 0x%08x, MAX_SIZE: 0x%08x\n",
            i4_ret, i4_len, PRELOADER_MAX_SIZE);
    if ((i4_ret >= 0) && (i4_ret <= (PRELOADER_MAX_SIZE))) {
        i4_cnt = write(fdtarget, _achBuf, i4_len);
        if (i4_cnt <= (PRELOADER_MAX_SIZE)) {
        } else {
            printf("%s(%d) write failed\n", __FILE__, __LINE__);
            return 1;
        }
    } else {
        printf("%s(%d) preloader is too bigger\n", __FILE__, __LINE__);
        return 1;
    }

    i4_ret = fstat(fddiag, &st);
    if (i4_ret != 0) {
        printf("fstat diag failed\n");
        return 1;
    }

    memset(&rImgHdr, 0, sizeof(rImgHdr));
    rImgHdr.u4Signature = IMAGE_SIGNATURE;
    rImgHdr.u4Size = (UINT32)st.st_size;
    printf("%s size is 0x%08x\n", argv[2], (UINT32)st.st_size);
    rImgHdr.fgCompress = COMPRESS_TYPE_LZHS;
    rImgHdr.u4FirmwareChksum = 0;
    rImgHdr.u4FirmwareExecAddr = 0;
    rImgHdr.u4PQDataAddr = 0;

    if (sizeof(rImgHdr) != 64) {
        printf("rImgHdr is %d, wrong\n", sizeof(rImgHdr));
        return 1;
    }

    i4_cnt = write(fdtarget, &rImgHdr, sizeof(rImgHdr));
    if (i4_cnt != sizeof(rImgHdr)) {
        printf("write rImgHdr failed\n");
        return 1;
    }
    i4_len += i4_cnt;

    do {
        i4_ret = read(fddiag, _achBuf, BUFFER_SIZE);
        if (i4_ret > 0) {
            i4_cnt = write(fdtarget, _achBuf, i4_ret);
            if (i4_cnt != i4_ret) {
                printf("%s(%d) failed\n", __FILE__, __LINE__);
                return 1;
            }
            i4_len += i4_cnt;
        }
    } while (i4_ret > 0);
    
    if(i4_len>LOADER_MAX_SIZE)
     {  
     	  printf("loader  size 0x%08x \n",i4_len);
     	  printf("LOADER_MAX_SIZE  0x%08x \n",LOADER_MAX_SIZE);
        printf("%s(%d) loader is too big \n", __FILE__, __LINE__);
        return 1;
     }
    close(fdloader);
    close(fddiag);
    close(fdtarget);
    return 0;
}

