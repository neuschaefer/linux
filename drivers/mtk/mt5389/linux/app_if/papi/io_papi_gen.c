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
/*-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: io_papi_gen.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_gen.c
 *  PAPI kernel driver implementation: General
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <linux/papi_ioctl.h>


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
/// Common error code
#define TM_OK                   0
#define TM_FAIL                 -0x800


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct _PAPI_GEN_ALN_INFO_T
{
    int IsSupported;
    int Value;
    int Default;
    int Max;
    int Min;
} PAPI_GEN_ALN_INFO_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size) \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size))) \
    { \
        return TM_FAIL ; \
    } \
    if (copy_from_user((dst), (void __user *)(src), (size))) \
    { \
        return TM_FAIL ; \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size))) \
    { \
        return TM_FAIL ; \
    } \
    if (copy_to_user((void __user *)(dst), (src), (size))) \
    { \
        return TM_FAIL ; \
    }


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static PAPI_GEN_ALN_INFO_T sGenAlnInfo[] = 
//  support  value  default   max  min
    {{FALSE,     0,      0,    7,    0},
     {FALSE,     0,      0,    7,    0}};

static int iGenAlnNum = sizeof(sGenAlnInfo)/sizeof(PAPI_GEN_ALN_INFO_T);

//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------

extern int papi_ioctl_gen_vgtp(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int papi_ioctl_gen_sbp(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static INT32 _ioctl_gen_aln_Default(unsigned long arg)
{
    IOCTL_2ARG_T rArg;
    int alignment;
    int fgRet = TM_FAIL;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_2ARG_T));
    alignment = (int)rArg.au4Arg[0];

    if (alignment < iGenAlnNum) {
        rArg.au4Arg[1] = sGenAlnInfo[alignment].Default;
        fgRet = TM_OK;
    }

    // Copy data to user memory
    COPY_TO_USER(arg, &rArg, sizeof(IOCTL_2ARG_T));

    return fgRet;
}

static INT32 _ioctl_gen_aln_Get(unsigned long arg)
{
    IOCTL_2ARG_T rArg;
    int alignment;
    int fgRet = TM_FAIL;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_2ARG_T));
    alignment = (int)rArg.au4Arg[0];

    if (alignment < iGenAlnNum) {
        rArg.au4Arg[1] = sGenAlnInfo[alignment].Value;
        fgRet = TM_OK;
    }

    // Copy data to user memory
    COPY_TO_USER(arg, &rArg, sizeof(IOCTL_2ARG_T));

    return fgRet;
}

static INT32 _ioctl_gen_aln_IsSupported(unsigned long arg)
{
    IOCTL_2ARG_T rArg;
    int alignment;
    int fgRet = TM_FAIL;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_2ARG_T));
    alignment = (int)rArg.au4Arg[0];

    if (alignment < iGenAlnNum) {
        rArg.au4Arg[1] = sGenAlnInfo[alignment].IsSupported;
        fgRet = TM_OK;
    }

    // Copy data to user memory
    COPY_TO_USER(arg, &rArg, sizeof(IOCTL_2ARG_T));

    return fgRet;
}

static INT32 _ioctl_gen_aln_Max(unsigned long arg)
{
    IOCTL_2ARG_T rArg;
    int alignment;
    int fgRet = TM_FAIL;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_2ARG_T));
    alignment = (int)rArg.au4Arg[0];

    if (alignment < iGenAlnNum) {
        rArg.au4Arg[1] = sGenAlnInfo[alignment].Max;
        fgRet = TM_OK;
    }

    // Copy data to user memory
    COPY_TO_USER(arg, &rArg, sizeof(IOCTL_2ARG_T));

    return fgRet;
}

static INT32 _ioctl_gen_aln_Min(unsigned long arg)
{
    IOCTL_2ARG_T rArg;
    int alignment;
    int fgRet = TM_FAIL;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_2ARG_T));
    alignment = (int)rArg.au4Arg[0];

    if (alignment < iGenAlnNum) {
        rArg.au4Arg[1] = sGenAlnInfo[alignment].Min;
        fgRet = TM_OK;
    }

    // Copy data to user memory
    COPY_TO_USER(arg, &rArg, sizeof(IOCTL_2ARG_T));

    return fgRet;
}

static INT32 _ioctl_gen_aln_Set(unsigned long arg)
{
    IOCTL_2ARG_T rArg;
    int alignment;
    int value;
    int fgRet = TM_FAIL;

    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_2ARG_T));
    alignment = (int)rArg.au4Arg[0];
    value = (int)rArg.au4Arg[1];

    // [Justin TODO]
    if (alignment < iGenAlnNum) {
        sGenAlnInfo[alignment].Value = value;
        fgRet = TM_OK;
    }

    return fgRet;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_gen_aln(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;
    //_ApiLock();

    switch (cmd)
    {
    case PAPI_IO_GEN_ALN_DEFAULT:
       i4Ret = _ioctl_gen_aln_Default(arg);
    	break;
    case PAPI_IO_GEN_ALN_GET:
       i4Ret = _ioctl_gen_aln_Get(arg);
    	break;
    case PAPI_IO_GEN_ALN_ISSUPPORTED:
       i4Ret = _ioctl_gen_aln_IsSupported(arg);
    	break;
    case PAPI_IO_GEN_ALN_MAX:
       i4Ret = _ioctl_gen_aln_Max(arg);
    	break;
    case PAPI_IO_GEN_ALN_MIN:
       i4Ret = _ioctl_gen_aln_Min(arg);
    	break;
    case PAPI_IO_GEN_ALN_SET:
       i4Ret = _ioctl_gen_aln_Set(arg);
    	break;
    default:
        printk("papi_ioctl_gen_aln: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }
    
    //_ApiUnlock();

    return i4Ret;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_gen(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    switch (cmd)
    {
    case PAPI_IO_GEN_ALN_DEFAULT:
    case PAPI_IO_GEN_ALN_GET:
    case PAPI_IO_GEN_ALN_ISSUPPORTED:
    case PAPI_IO_GEN_ALN_MAX:
    case PAPI_IO_GEN_ALN_MIN:
    case PAPI_IO_GEN_ALN_SET:
        i4Ret = papi_ioctl_gen_aln(inode, file, cmd, arg);
        break;

    case PAPI_IO_GEN_VGTP_ISTESTPATTERNALLOWED:
    case PAPI_IO_GEN_VGTP_GETTESTPATTERN:
    case PAPI_IO_GEN_VGTP_SETTESTPATTERN:
        i4Ret = papi_ioctl_gen_vgtp(inode, file, cmd, arg);
        break;

    case PAPI_IO_GEN_SBP_GETFREQ :
    case PAPI_IO_GEN_SBP_SETFREQ :
    case PAPI_IO_GEN_SBP_GETMUTE :
    case PAPI_IO_GEN_SBP_SETMUTE :
    case PAPI_IO_GEN_SBP_GETVOLUME :
    case PAPI_IO_GEN_SBP_SETVOLUME :
         i4Ret = papi_ioctl_gen_sbp(inode, file, cmd, arg);
         break;
         
    default:
        printk("papi_ioctl_gen: unknown IO command 0x%08x\n", cmd);
        i4Ret = -EFAULT;
    }

    return i4Ret;
}




