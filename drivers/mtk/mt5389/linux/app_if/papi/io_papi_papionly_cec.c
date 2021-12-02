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
 * $RCSfile: io_papi_papionly_cec.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_src_src.c
 *  PAPI kernel driver implementation: Src selection
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
/*
#include "x_typedef.h"
#include "nptv_if.h"
#include "source_table.h"
#include "vdp_if.h"
#include "mpv_if.h"
#include "aud_if.h"
#include "aud_drvif.h"
#include "aud_dsp_cfg.h"
#include "fbm_drvif.h"
#include "x_assert.h"
#include "papi.h"
#include <linux/papi_ioctl.h>
#include <linux/mt53xx_cb.h>
*/

#include "x_linux.h"
#include "x_typedef.h"
#include "x_assert.h"
#include "cec_if.h"
#include <linux/papi_ioctl.h>	
#include "papi.h"
#include "pdwnc_if.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define COMMU_WITH_T8032

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
/// Common error code
#define PAPI_OK         (0)
#define PAPI_FAIL       (-1)
#define PAPI_ERR_PARA   (-2)

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size) \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size))) \
    { \
        return -EACCES; \
    } \
    if (copy_from_user((dst), (void __user *)(src), (size))) \
    { \
        return -EACCES; \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size))) \
    { \
        return -EACCES; \
    } \
    if (copy_to_user((void __user *)(dst), (void *)(src), (size))) \
    { \
        return -EACCES; \
    }
        
//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
typedef struct
{
	UINT8 u1Size;
	UINT8 au1WkpCode[10];
} CEC_WAKEUP_CODE_T;

typedef struct
{
	UINT32 u4opcode;
	INT32 i4length;
	UINT8 au1buf[16];
} CEC_WAKEUP_BUFFER_T;

CEC_WAKEUP_CODE_T _rCecWkpCode;
//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static int _papi_papionly_wakeup_ProgramCecWakeupCodes(unsigned long arg)
{
    IOCTL_2ARG_T rArg;    
    UINT32 au4WkpCode[10];
    UINT8 au1WkpCode[10];
    INT32 i4Length;
    UINT8 i;
    #ifdef COMMU_WITH_T8032
    PDWNC_T8032_CMD_T rCmd; 
    UINT8 u1WkpCodeSize;
    #endif
  	
    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_2ARG_T));
    i4Length = (INT32)rArg.au4Arg[0];
    if ((i4Length < 0) || (i4Length > 10))
    {
        return PAPI_ERR_PARA;
    }
	COPY_FROM_USER(&au4WkpCode[0], rArg.au4Arg[1], (4 * i4Length));

	_rCecWkpCode.u1Size = (UINT8)i4Length;
    for (i = 0; i < _rCecWkpCode.u1Size; i++)
    {
		_rCecWkpCode.au1WkpCode[i] = (UINT8)(au4WkpCode[i]);
    }
	 
	#ifdef COMMU_WITH_T8032
    u1WkpCodeSize = _rCecWkpCode.u1Size;

  	rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_WAKEUP_OPCODE_1;
    rCmd.u1SubCmd = 0;
    rCmd.au1Data[0] = u1WkpCodeSize;
    x_memcpy(&rCmd.au1Data[1], &(_rCecWkpCode.au1WkpCode[0]), 3);  //au1WkpCode[0] ~ [2]
    PDWNC_T8032Cmd(&rCmd, NULL);

    if (u1WkpCodeSize > 3)
    {
    	u1WkpCodeSize = u1WkpCodeSize - 3;
    }
    else
    {
		return PAPI_OK;
    }

  	rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_WAKEUP_OPCODE_2;
    x_memcpy(&rCmd.au1Data[0], &(_rCecWkpCode.au1WkpCode[3]), 4);	//au1WkpCode[3] ~ [6]
    PDWNC_T8032Cmd(&rCmd, NULL);

    if (u1WkpCodeSize > 4)
    {
    	u1WkpCodeSize = u1WkpCodeSize - 4;
    }
    else
    {
		return PAPI_OK;
    }

    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_WAKEUP_OPCODE_3;
    x_memcpy(&rCmd.au1Data[0], &(_rCecWkpCode.au1WkpCode[7]), 4);	//au1WkpCode[7] ~ [9]
    PDWNC_T8032Cmd(&rCmd, NULL);
	#endif

    return PAPI_OK;
}

static int _papi_papionly_wakeup_GetWakeupDetailsCec(unsigned long arg)
{
    IOCTL_3ARG_T rArg;
    UINT32 *pu4Opcode;
    INT32 *pi4Length;
    UINT8 *pu1Buf;
    INT32 i;
    CEC_FRAME_DESCRIPTION *prBuffer;
    CEC_WAKEUP_BUFFER_T rWkpBuf;
    
    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof (IOCTL_3ARG_T));
    pu4Opcode = (UINT32 *)rArg.au4Arg[0];
    pi4Length = (INT32 *)rArg.au4Arg[1];
    pu1Buf = (UINT8 *)rArg.au4Arg[2];
    
    prBuffer = CEC_GetWakeupDetails();
    rWkpBuf.u4opcode = (UINT32)prBuffer->blocks.opcode;
    rWkpBuf.i4length = (INT32)prBuffer->size;
    rWkpBuf.au1buf[0] = ((UINT8)(prBuffer->blocks.header.initiator) << 4) + prBuffer->blocks.header.destination;
    rWkpBuf.au1buf[1] = prBuffer->blocks.opcode;
    for(i = 2; i < rWkpBuf.i4length; i++)
    {
		rWkpBuf.au1buf[i] = prBuffer->blocks.operand[i-2];
    }
    
    COPY_TO_USER(pu4Opcode, &(rWkpBuf.u4opcode), sizeof(UINT32));
    COPY_TO_USER(pi4Length, &(rWkpBuf.i4length), sizeof(INT32));
    COPY_TO_USER(pu1Buf, &(rWkpBuf.au1buf[0]), rWkpBuf.i4length);

    return PAPI_OK;
}
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_papionly_cec(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    switch (cmd)
    {
    case PAPI_IO_PAPIONLY_PROGRAMCECWAKEUPCODES:
        i4Ret = _papi_papionly_wakeup_ProgramCecWakeupCodes(arg);
        break;

    case PAPI_IO_PAPIONLY_GETWAKEUPDETAILSCEC:
        i4Ret = _papi_papionly_wakeup_GetWakeupDetailsCec(arg);
        break;
       
    default:
        printk("papi_ioctl_papiony_key: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    return i4Ret;
}


