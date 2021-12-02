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
 * $RCSfile: io_papi_infra_cec.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_infra_cec.c
 *  PAPI kernel driver implementation: setup
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_linux.h"
#include "x_typedef.h"
#include "x_assert.h"
#include "cec_if.h"
#include <linux/papi_ioctl.h>	
#include <linux/cb_data.h>
#include "papi.h"
#include "pdwnc_if.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define COMMU_WITH_T8032

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define PAPI_OK         (0)
#define PAPI_FAIL       (-1)
#define PAPI_ERR_PARA   (-2)

// CEC callback command type
//#define CEC_CB_ONRECEIVED                 0
//#define CEC_CB_ONTRANSMITCOMPLETED        1
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


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
// External function prototypes
//-----------------------------------------------------------------------------

/// Put a callback event
EXTERN INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
typedef struct
{
	UINT8 u1Size;
	UINT8 au1Msg[16];
} CEC_MESSAGE_T;

UINT8 _fgCECEnable = 1;
UINT8 _fgEnTransmitStatusNotify = 1;
UINT8 _u4Version = 0x04;
CEC_OSD_NAME _rOsdName;
CEC_DEVICE_VENDOR_ID _rVendorID;
UINT8 _u4LogicalAddr = 0;
//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
/** Handler of receive cec message
 *
 *  @param  u1Pidx          The PID id
 *  @param  u2Pid           PID value
 *  @param  u4Type          Type of callback event
 *
 *  @retval TRUE            Successful
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
void _CECMWNfyDataArrival(CEC_FRAME_DESCRIPTION* frame)
{
    PAPI_INFRA_CEC_CB_T rInfraCecCb;
    INT32 i;

	rInfraCecCb.u4Type = CEC_CB_ONRECEIVED;
	rInfraCecCb.i4Size = (INT32)frame->size;
	rInfraCecCb.au1Data[0] = ((UINT8)(frame->blocks.header.initiator) << 4) + frame->blocks.header.destination;
	rInfraCecCb.au1Data[1] = frame->blocks.opcode;
	for(i = 2; i < rInfraCecCb.i4Size; i++)
    {
		rInfraCecCb.au1Data[i] = frame->blocks.operand[i-2];
    }

    _CB_PutEvent(CB_PAPI_INFRA_CEC_TRIGGER, sizeof(PAPI_INFRA_CEC_CB_T), (void *)&rInfraCecCb);
}

void _CECMWNfyTxResult(UINT8 result, void* txtag)
{
    PAPI_INFRA_CEC_CB_T rInfraCecCb;

	UNUSED(txtag);
    if (_fgEnTransmitStatusNotify == 1)
    {
       rInfraCecCb.u4Type = CEC_CB_ONTRANSMITCOMPLETED;
	rInfraCecCb.au1Data[0] = result; 
   
       _CB_PutEvent(CB_PAPI_INFRA_CEC_TRIGGER, sizeof(PAPI_INFRA_CEC_CB_T), (void *)&rInfraCecCb);
    }
} 

static int _papi_infra_cecctrl_SetNfy(unsigned long arg)
{  
    CECMWSetDataArrivalNfy(_CECMWNfyDataArrival);
    CECMWSetTxResultNfy(_CECMWNfyTxResult);
    return PAPI_OK;
}

static int _papi_infra_cecctrl_SetVersion(unsigned long arg)
{
    UINT32 u4Arg;    
    #ifdef COMMU_WITH_T8032
    PDWNC_T8032_CMD_T rCmd;  
	#endif
	
    // Copy arguments from user space
    COPY_FROM_USER(&u4Arg, arg, sizeof (UINT32));
	
    #ifdef COMMU_WITH_T8032
	rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_CEC_VERSION;
    rCmd.u1SubCmd = 0;
    rCmd.au1Data[0] = (UINT8)u4Arg;
    PDWNC_T8032Cmd(&rCmd, NULL);
    #endif
    
    _u4Version = u4Arg;
    CEC_SetVersion(u4Arg);
    return PAPI_OK;
}

static int _papi_infra_cecctrl_GetVersion(unsigned long arg)
{
    UINT32 u4Arg,u4Version;
    UINT32 *pu4Version;
    
    // Copy arguments from user space
    COPY_FROM_USER(&u4Arg, arg, sizeof (UINT32));
    pu4Version = (UINT32 *)u4Arg;
    u4Version = _u4Version;
    COPY_TO_USER(pu4Version, &u4Version, sizeof(UINT32));

    return PAPI_OK;
}

static int _papi_infra_cecctrl_SetOsdName(unsigned long arg)
{
    IOCTL_2ARG_T rArg;    
    INT32 i4Ret;
    #ifdef COMMU_WITH_T8032
    PDWNC_T8032_CMD_T rCmd; 
    UINT8 u1OsdNameSize;
    #endif
  	
    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_2ARG_T));
    
    _rOsdName.i4Size = (INT32)rArg.au4Arg[1];
    if ((_rOsdName.i4Size < 0) || (_rOsdName.i4Size > 14))
    {
        return PAPI_ERR_PARA;
    }
    COPY_FROM_USER(&(_rOsdName.acOsdName[0]), rArg.au4Arg[0], _rOsdName.i4Size);

	i4Ret = CEC_SetOsdName(&_rOsdName);
    if (i4Ret != 0)
    {
		return PAPI_ERR_PARA;
    }
    
	#ifdef COMMU_WITH_T8032
    u1OsdNameSize = (UINT8)_rOsdName.i4Size;
  	rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_OSD_NAME_1;
    rCmd.u1SubCmd = 0;
    rCmd.au1Data[0] = u1OsdNameSize;
    x_memcpy(&rCmd.au1Data[1], &(_rOsdName.acOsdName[0]), 3);  //acOsdName[0] ~ [2]
    PDWNC_T8032Cmd(&rCmd, NULL);

    if (u1OsdNameSize > 3)
    {
    	u1OsdNameSize = u1OsdNameSize - 3;
    }
    else
    {
		return PAPI_OK;
    }

  	rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_OSD_NAME_2;
    x_memcpy(&rCmd.au1Data[0], &(_rOsdName.acOsdName[3]), 4);	//acOsdName[3] ~ [6]
    PDWNC_T8032Cmd(&rCmd, NULL);

    if (u1OsdNameSize > 4)
    {
    	u1OsdNameSize = u1OsdNameSize - 4;
    }
    else
    {
		return PAPI_OK;
    }

    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_OSD_NAME_3;
    x_memcpy(&rCmd.au1Data[0], &(_rOsdName.acOsdName[7]), 4);	//acOsdName[7] ~ [10]
    PDWNC_T8032Cmd(&rCmd, NULL);

    if (u1OsdNameSize > 4)
    {
    	u1OsdNameSize = u1OsdNameSize - 4;
    }
    else
    {
		return PAPI_OK;
    }

	rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_OSD_NAME_4;
    x_memcpy(&rCmd.au1Data[0], &(_rOsdName.acOsdName[11]), 3);	//acOsdName[11] ~ [13]
    PDWNC_T8032Cmd(&rCmd, NULL);
    #endif

    return PAPI_OK;
}

static int _papi_infra_cecctrl_GetOsdName(unsigned long arg)
{
    IOCTL_2ARG_T rArg;    
    CHAR *pcOsdName;
    INT32 *pi4Length; 
    
    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof (IOCTL_2ARG_T));
    pcOsdName = (CHAR *)rArg.au4Arg[0];
    pi4Length = (INT32 *)rArg.au4Arg[1];
    COPY_TO_USER(pi4Length, &(_rOsdName.i4Size), sizeof(INT32));
    COPY_TO_USER(pcOsdName, &(_rOsdName.acOsdName[0]), _rOsdName.i4Size);

    return PAPI_OK;
}

static int _papi_infra_cecctrl_SetDeviceVendorID(unsigned long arg)
{
    IOCTL_2ARG_T rArg;    
    INT32 i4Ret;
    #ifdef COMMU_WITH_T8032
    PDWNC_T8032_CMD_T rCmd;  
	#endif
  	
    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_2ARG_T));
    _rVendorID.i4Size = (INT32)rArg.au4Arg[1];
    if (_rVendorID.i4Size != 3)
    {
        return PAPI_ERR_PARA;
    }
	COPY_FROM_USER(&(_rVendorID.au1VendorID[0]), rArg.au4Arg[0], _rVendorID.i4Size);

	i4Ret = CEC_SetDeviceVendorID(&_rVendorID);
	if (i4Ret != 0)
    {
		return PAPI_ERR_PARA;
    }

  	#ifdef COMMU_WITH_T8032
  	rCmd.u1Cmd = PDWNC_CMD_ARMT8032_SET_VENDOR_ID;
    rCmd.u1SubCmd = 0;
    x_memcpy(rCmd.au1Data, &(_rVendorID.au1VendorID[0]), 3);
    PDWNC_T8032Cmd(&rCmd, NULL);
	#endif

    return PAPI_OK;
}

static int _papi_infra_cecctrl_GetDeviceVendorID(unsigned long arg)
{
    IOCTL_2ARG_T rArg;    
    UINT8 *pu1VendorID;
    INT32 *pi4Length; 
    
    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof (IOCTL_2ARG_T));
    pu1VendorID = (UINT8 *)rArg.au4Arg[0];
    pi4Length = (INT32 *)rArg.au4Arg[1];
    COPY_TO_USER(pi4Length, &(_rVendorID.i4Size), sizeof(INT32));
    COPY_TO_USER(pu1VendorID, &(_rVendorID.au1VendorID[0]), _rVendorID.i4Size);

    return PAPI_OK;
}

static int _papi_infra_cecctrl_SetLogicalAddress(unsigned long arg)
{
    UINT32 u4Arg;    
    
    // Copy arguments from user space
    COPY_FROM_USER(&u4Arg, arg, sizeof (UINT32));

    if (u4Arg > 16)
    {
	return PAPI_ERR_PARA;
    }
    _u4LogicalAddr = u4Arg;
    CECMWSetLogicAddress((UINT8)u4Arg);
    return PAPI_OK;
}

static int _papi_infra_cecctrl_GetLogicalAddress(unsigned long arg)
{
    UINT32 u4Arg;
    UINT32 *pu4LogicalAddr;
    
    // Copy arguments from user space
    COPY_FROM_USER(&u4Arg, arg, sizeof (UINT32));
    pu4LogicalAddr = (UINT32 *)u4Arg;
    COPY_TO_USER(pu4LogicalAddr, &_u4LogicalAddr, sizeof(UINT32));

    return PAPI_OK;
}

static int _papi_infra_cecctrl_Enable(unsigned long arg)
{  
  	#ifdef COMMU_WITH_T8032
	PDWNC_T8032_CMD_T rCmd;
  	#endif
  
    CECMWSetEnableCEC(1);
     _fgCECEnable = 1;
     
  	#ifdef COMMU_WITH_T8032
    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_CEC; 
    rCmd.u1SubCmd = 0;
    rCmd.au1Data[0] = 1;
    PDWNC_T8032Cmd(&rCmd, NULL);

    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_ONE_TOUCH_PLAY;
    rCmd.au1Data[0] = 1;
    PDWNC_T8032Cmd(&rCmd, NULL);
  	#endif

    return PAPI_OK;
}

static int _papi_infra_cecctrl_Disable(unsigned long arg)
{  
	#ifdef COMMU_WITH_T8032
	PDWNC_T8032_CMD_T rCmd;
  	#endif

    CECMWSetEnableCEC(0);
    _fgCECEnable = 0;

  	#ifdef COMMU_WITH_T8032
    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_CEC; 
    rCmd.u1SubCmd = 0;
    rCmd.au1Data[0] = 0;
    PDWNC_T8032Cmd(&rCmd, NULL);

    rCmd.u1Cmd = PDWNC_CMD_ARMT8032_ENABLE_ONE_TOUCH_PLAY;
    rCmd.au1Data[0] = 0;
    PDWNC_T8032Cmd(&rCmd, NULL);
  	#endif
    return PAPI_OK;
}

static int _papi_infra_cecctrl_EnableReceive(unsigned long arg)
{  
    CEC_EnableReceive();
    return PAPI_OK;
}

static int _papi_infra_cecctrl_DisableReceive(unsigned long arg)
{  
    CEC_DisableReceive();
    return PAPI_OK;
}

static int _papi_infra_cecctrl_EnableTransmitStatus(unsigned long arg)
{  
    _fgEnTransmitStatusNotify = 1;
    return PAPI_OK;
}

static int _papi_infra_cecctrl_DisableTransmitStatus(unsigned long arg)
{  
    _fgEnTransmitStatusNotify = 0;
    return PAPI_OK;
}

static int _papi_infra_cecctrl_GetBufferedMsg(unsigned long arg)
{
    IOCTL_2ARG_T rArg;    
    UINT8 *pu1Msg;
    UINT8 *pu1Length; 
    UINT8 i;
    CEC_FRAME_DESCRIPTION* prBuf;
    CEC_MESSAGE_T rMsg;
    
    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof (IOCTL_2ARG_T));
    pu1Msg = (UINT8 *)rArg.au4Arg[0];
    pu1Length = (UINT8 *)rArg.au4Arg[1];
    
    prBuf = CEC_GetBufferedMsg();
    rMsg.u1Size = prBuf->size;
    rMsg.au1Msg[0] = ((UINT8)(prBuf->blocks.header.initiator) << 4) + prBuf->blocks.header.destination;
    rMsg.au1Msg[1] = prBuf->blocks.opcode;
    for(i = 2; i < rMsg.u1Size; i++)
    {
		rMsg.au1Msg[i] = prBuf->blocks.operand[i-2];
    }
    
    if (rMsg.u1Size > 16)
    {
		return PAPI_ERR_PARA;
    }
    COPY_TO_USER(pu1Length, &(rMsg.u1Size), sizeof(UINT8));
    COPY_TO_USER(pu1Msg, &(rMsg.au1Msg[0]), rMsg.u1Size);

    return PAPI_OK;
}

static int _papi_infra_cecctrl_MessageProcessed(unsigned long arg)
{  
    CEC_MessageProcessed();
    return PAPI_OK;
}

static int _papi_infra_cectx_Transmit(unsigned long arg)
{
    IOCTL_3ARG_T rArg;    
    UINT8 au1msg[16];
    INT32 i4Length;
    UINT16* pu2retval;
    UINT16 u2retval;
    CEC_FRAME_DESCRIPTION  rCecFrame;
    INT32 i;
    UINT8 u1Ret;
    
    // Copy arguments from user space
    COPY_FROM_USER(&rArg, arg, sizeof(IOCTL_3ARG_T));
    i4Length = (INT32)rArg.au4Arg[0];
    pu2retval = (UINT16*)rArg.au4Arg[2];

    if (pu2retval == NULL)
    {
 	return PAPI_ERR_PARA;
    }
    if ((i4Length < 0) || (i4Length > 16) || (rArg.au4Arg[1] == NULL))
    {
        u2retval = FALSE;
        COPY_TO_USER(pu2retval, &u2retval, sizeof(UINT16));
        return PAPI_ERR_PARA;
    }
	COPY_FROM_USER(&au1msg[0], rArg.au4Arg[1], i4Length);
    
  	if (_fgCECEnable == 0)
  	{
		u2retval = FALSE;
		COPY_TO_USER(pu2retval, &u2retval, sizeof(UINT16));
		return PAPI_OK;
  	}

	rCecFrame.size = (UINT8)i4Length;
	rCecFrame.sendidx = 0;
	rCecFrame.reTXcnt = 0;
	rCecFrame.txtag = NULL;
    rCecFrame.blocks.header.destination = (UINT8)((au1msg[0]) & 0x0f);
    rCecFrame.blocks.header.initiator = (UINT8)((au1msg[0]) >> 4);
    rCecFrame.blocks.opcode = au1msg[1];
    for (i = 0; i < (i4Length-2); i++)
	{
		rCecFrame.blocks.operand[i] = au1msg[2 + i];
	}

	u1Ret = _CEC_TX_Enqueue(&rCecFrame);
    if (u1Ret == 0)
    {
  		u2retval = TRUE;
    }
    else
    {
		u2retval = FALSE;
    }
	
    COPY_TO_USER(pu2retval, &u2retval, sizeof(UINT16));
    return PAPI_OK;
}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------
int papi_ioctl_infra_cecctrl(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    switch (cmd)
    {
    case PAPI_IO_INFRA_CECCTRL_SETVERSION:
        return _papi_infra_cecctrl_SetVersion(arg);

    case PAPI_IO_INFRA_CECCTRL_GETVERSION:
        return _papi_infra_cecctrl_GetVersion(arg);

    case PAPI_IO_INFRA_CECCTRL_SETOSDNAME:
        return _papi_infra_cecctrl_SetOsdName(arg);

    case PAPI_IO_INFRA_CECCTRL_GETOSDNAME:
        return _papi_infra_cecctrl_GetOsdName(arg);

	case PAPI_IO_INFRA_CECCTRL_SETDEVICEVENDORID:
        return _papi_infra_cecctrl_SetDeviceVendorID(arg);

    case PAPI_IO_INFRA_CECCTRL_GETDEVICEVENDORID:
        return _papi_infra_cecctrl_GetDeviceVendorID(arg);

	case PAPI_IO_INFRA_CECCTRL_SETLOGICALADDRESS:
        return _papi_infra_cecctrl_SetLogicalAddress(arg);

    case PAPI_IO_INFRA_CECCTRL_GETLOGICALADDRESS:
        return _papi_infra_cecctrl_GetLogicalAddress(arg);

	case PAPI_IO_INFRA_CECCTRL_ENABLE:
        return _papi_infra_cecctrl_Enable(arg);

	case PAPI_IO_INFRA_CECCTRL_DISABLE:
        return _papi_infra_cecctrl_Disable(arg);

	case PAPI_IO_INFRA_CECCTRL_ENABLERECEIVE:
        return _papi_infra_cecctrl_EnableReceive(arg);

	case PAPI_IO_INFRA_CECCTRL_DISABLERECEIVE:
        return _papi_infra_cecctrl_DisableReceive(arg);

	case PAPI_IO_INFRA_CECCTRL_ENABLETRANSMITSTAUTS:
        return _papi_infra_cecctrl_EnableTransmitStatus(arg);

	case PAPI_IO_INFRA_CECCTRL_DISABLETRANSMITSTAUTS:
        return _papi_infra_cecctrl_DisableTransmitStatus(arg);

	case PAPI_IO_INFRA_CECCTRL_GETBUFFEREDMSG:
        return _papi_infra_cecctrl_GetBufferedMsg(arg);

	case PAPI_IO_INFRA_CECACK_MESSAGEPROCESSED:
        return _papi_infra_cecctrl_MessageProcessed(arg);

	case PAPI_IO_INFRA_CECTX_TRANSMIT:
        return _papi_infra_cectx_Transmit(arg);

    case PAPI_IO_INFRA_CECCTRL_SETNFY:
        return _papi_infra_cecctrl_SetNfy(arg);
    
    default:
        printk("papi_ioctl_infra: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    return i4Ret;
}

