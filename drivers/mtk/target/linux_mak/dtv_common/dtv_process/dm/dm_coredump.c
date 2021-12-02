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
/*----------------------------------------------------------------------------*
 * $RCSfile: dm_coredump.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *
 * Description: 
 *         This file is used to dump core file to usb storage.
 *---------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/resource.h>

#include "dm_util.h"
#include "dbg/x_dbg.h"

/* ASCII values for "herehere" */
#define SZ_MARK_LEN         256
#define SZ_MARK             "6865726568657265"
#define SZ_CORE_PATTERN     "/proc/sys/kernel/core_pattern"

static int bCoreFileSet = 0;

static VOID _dm_core_file_set_limit(int bUnlimited)
{
    struct rlimit limit;

    getrlimit(RLIMIT_CORE, &limit);
    limit.rlim_cur = bUnlimited? RLIM_INFINITY : 0;
    limit.rlim_max = bUnlimited? RLIM_INFINITY : 0;
    if (setrlimit(RLIMIT_CORE, &limit) < 0)
    {
        x_dbg_stmt("Error setting limit.\n");
    }
    else
    {
        if (getrlimit(RLIMIT_CORE, &limit) == 0)
        {
            if ((bUnlimited && (limit.rlim_max != RLIM_INFINITY)) || (!bUnlimited && limit.rlim_max))
            {
                x_dbg_stmt("After setting limit: cur = 0x%X, max = 0x%X, RLIM_INFINITY = 0x%X\n",
                           limit.rlim_cur, limit.rlim_max, RLIM_INFINITY);
            }
            else
            {
                x_dbg_stmt("Corefile set to %s.\n", bUnlimited? "Unlimited":"0");
            }
        }
        else
        {
            x_dbg_stmt("Error getting limit.\n");
        }
    }
}

VOID dm_core_file_enable(const CHAR* szMountDir)
{
    if (!bCoreFileSet)
    {
        FILE * pfMark;
        char szMark[SZ_MARK_LEN];

        snprintf(szMark, SZ_MARK_LEN-1, "%s/%s", szMountDir, SZ_MARK);
        szMark[SZ_MARK_LEN-1] = 0;
        pfMark = fopen(szMark, "r");

        if (pfMark)
        {
            fclose(pfMark);

            /* set core file path and format first */
            snprintf(szMark, SZ_MARK_LEN-1,
                     "echo %s/core_%%e_sig%%s_pid%%p > %s",
                     szMountDir, SZ_CORE_PATTERN);
            szMark[SZ_MARK_LEN-1] = 0;
            /*x_dbg_stmt("Cmd = [%s]\n", szMark);*/
            if (system(szMark) != 0)
            {
                x_dbg_stmt("Failed to set core file path [%s/core_%%e_sig%%s_pid%%p] to [%s]\n",
                           szMountDir, SZ_CORE_PATTERN);
            }

            /* enable core file dump */
            /*system("ulimit -c unlimited");*/
            _dm_core_file_set_limit(1);

            /* mark flag, so that only one USB disk has the corefile */
            bCoreFileSet = 1;
        }
    }
}

VOID dm_core_file_disable(VOID)
{
    if (bCoreFileSet)
    {
        /* disable core file dump */
        _dm_core_file_set_limit(0);

        bCoreFileSet = 0;
    }
}



