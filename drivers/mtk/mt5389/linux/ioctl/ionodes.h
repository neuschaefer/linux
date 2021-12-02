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
#ifndef IONODES_H
#define IONODES_H


#include <linux/device.h>


#define IO_NODE_MAJOR_PAPI      240

/* major 241 devices */
#define IO_NODE_MAJOR_CLI       241
#define IO_NODE_MAJOR_MTAL      241
#define IO_NODE_MAJOR_CB        241
#define IO_NODE_MAJOR_GPIO      241
#define IO_NODE_MAJOR_DDI       242
#define IO_NODE_MAJOR_CLI       241
#define IO_NODE_MAJOR_ICE       244  // 243 is for b2r

/* major 241 devices' minor number */
#define IO_NODE_MINOR_MTAL        0
#define IO_NODE_MINOR_CLI         4
#define IO_NODE_MINOR_CB          8
#define IO_NODE_MINOR_GPIO       12

#define IO_NODE_MAJOR_JPG       251
#define IO_NODE_MAJOR_SDAL      250
#define IO_NODE_MAJOR_IR        249
#define IO_NODE_MAJOR_FEEDER    247
#define IO_NODE_MAJOR_EEP       246
#define IO_NODE_MAJOR_TSREC     245
#define IO_NODE_MAJOR_MPEG      239

#define IO_NODE_MAJOR_KMEM      244
#define IO_NODE_MAJOR_VOMX      238
#define IO_NODE_MAJOR_AOMX      237

#define IO_NODE_MAJOR_PVR       236
#define IO_NODE_MAJOR_B2R       243

#define IO_NODE_MAJOR_FBM       251
#define IO_NODE_MINOR_FBM       64
#define IO_NODE_NUMBER_FBM      64
#define IO_NODE_MAJOR_B2R       243

#define DEV_SDAL                "sdal"
#define DEV_CLI                 "cli"
#define DEV_CB                  "cb"
#define DEV_GPIO                "gpio"
#define DEV_JPG                 "jpg"
#define DEV_FEEDER              "feeder"
#define DEV_EEP                 "mtk_eep"
#define DEV_TSREC               "tsrec"
#define DEV_FBM                 "fbm"
#define DEV_PVR                 "pvr"
#define DEV_MPEG                "mpeg"

#define DEV_KMEM                "kmem2"
#define DEV_VOMX                "vomx"
#define DEV_AOMX                "aomx"
#define DEV_ICE					"ice"
#define DEV_B2R                 "b2r"

#define DEV_PAPI                "papi"
#define DEV_MTAL                "mtal"
#define DEV_DDI                 "ddi"

typedef struct
{
	unsigned int            major;
	unsigned int            minor;
	unsigned int            numbers;
	char                    *name;
	umode_t                 mode;
	struct file_operations  *fops;
} DEVLIST_T;

#endif //IONODES_H
