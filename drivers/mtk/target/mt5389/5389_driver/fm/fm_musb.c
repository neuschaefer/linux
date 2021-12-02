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
 * $RCSfile: fm_musb.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file musb_if.c
 *  MUSB module api function.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "musb_if.h"
#include "x_blk_dev.h"
#include "x_rm.h"
#include "x_fm.h"

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
typedef struct
{
    UINT32 u4Cond;
    VOID* pvData;
} FM_MUSB_MSG_T;

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static HANDLE_T _hDummyFileHandle = 0x12345678;
static HANDLE_T _hMusbNfyThread = NULL; 
static HANDLE_T _hMusbNfyMsgq = NULL; 
static BLKDEV_NFY_INFO_T _rMusbNfyInfo = {NULL, NULL};
//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
void _MusbNfyThread(VOID* pvArg)
{
    UINT16 u2QueueIndex;
    FM_MUSB_MSG_T rMsg;
    UINT32 u4Size = sizeof(rMsg);
    UINT32 i4Ret;
    UNUSED(i4Ret);    
    while(1)
    {
        i4Ret = x_msg_q_receive(&u2QueueIndex, (VOID*)&rMsg, (SIZE_T *)&u4Size,
            &_hMusbNfyMsgq, 1, X_MSGQ_OPTION_WAIT);

        if(_rMusbNfyInfo.pf_blkdev_nfy)
        {
            _rMusbNfyInfo.pf_blkdev_nfy(_rMusbNfyInfo.pv_tag,
                             (BLKDEV_COND_T)rMsg.u4Cond,
                             rMsg.pvData);
        }
    }
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
INT32 FM_MUSB_Get(HANDLE_T hHandle, UINT32 u4GetType, VOID* pvData, UINT32 *pu4Size)
{
    UINT64 *pu8Data;
    UINT32 *pu4Data;
    switch(u4GetType)
    {
        case (BLKDEV_GET_TYPE_SIZE):
        	pu8Data = (UINT64*)pvData;
        	*pu8Data = (UINT64)(MUSB_GetSize() * MUSB_GetBlockSize());
        	*pu4Size = sizeof(UINT64);        	
        	break;
        case (BLKDEV_GET_TYPE_NUMBER_OF_SECTORS):
        	pu8Data = (UINT64*)pvData;        	
        	*pu8Data = (UINT64)MUSB_GetSize();
        	*pu4Size = sizeof(UINT64);
        	break;        		
        case (BLKDEV_GET_TYPE_SECTOR_SIZE):	
        	pu4Data = (UINT32*)pvData;        	
        	*pu4Data = (UINT32)MUSB_GetBlockSize();
        	*pu4Size = sizeof(UINT32);        	
        	break;
        case (BLKDEV_GET_TYPE_MAX_TRANS_SECTORS):
              pu4Data = (UINT32*)pvData;        
              *pu4Data = 1;
        	*pu4Size = sizeof(UINT32);        	              
              break;
        case (BLKDEV_GET_TYPE_MAX_IO_REQUEST):        	
              pu4Data = (UINT32*)pvData;        
              *pu4Data = 1;        	
        	*pu4Size = sizeof(UINT32);        	              
              break;
        case (BLKDEV_GET_TYPE_STATUS):
              pu4Data = (UINT32*)pvData;        
              *pu4Data = (UINT32)MUSB_GetStatus();        	
        	*pu4Size = sizeof(UINT32);        	              
              break;        	
        case (BLKDEV_GET_TYPE_ALIGNMENT):
              pu4Data = (UINT32*)pvData;        
              *pu4Data = (UINT32)512;        	
        	*pu4Size = sizeof(UINT32);        	              
              break;               
        default:
        	
        	return -1;        	
    }
    return 0;
}

INT32 FM_MUSB_Set(HANDLE_T hHandle, UINT32 u4SetType, VOID* pvData, UINT32 u4Size)
{
    FM_MUSB_MSG_T rMsg;
    BLKDEV_IO_DATA_T *prBlkDev;    
    UINT32 i4Ret;
    UNUSED(i4Ret);
    switch(u4SetType)
    {
        case BLKDEV_SET_TYPE_NFY_FCT:
        {           
            VERIFY(pvData != NULL);
            x_memcpy(&_rMusbNfyInfo, (BLKDEV_NFY_INFO_T*)pvData, sizeof(_rMusbNfyInfo));
            break;            
        }
        case BLKDEV_SET_TYPE_READ:
        {
            prBlkDev = (BLKDEV_IO_DATA_T *)pvData;            
            i4Ret = MUSB_Read(prBlkDev->ui8_blk_num * MUSB_GetBlockSize(), (UINT32)prBlkDev->pv_data, prBlkDev->ui4_count);
            
            prBlkDev->ui4_trans_count = prBlkDev->ui4_count;
            rMsg.u4Cond = (UINT32)BLKDEV_COND_READ_OK;
            rMsg.pvData = pvData;
            	
            x_msg_q_send(_hMusbNfyMsgq, &rMsg, sizeof(rMsg), 200);        	
            break;	
        }
        case BLKDEV_SET_TYPE_WRITE:
        {
            prBlkDev = (BLKDEV_IO_DATA_T *)pvData;            
            i4Ret = MUSB_Write(prBlkDev->ui8_blk_num * MUSB_GetBlockSize(), (UINT32)prBlkDev->pv_data, prBlkDev->ui4_count);

            prBlkDev->ui4_trans_count = prBlkDev->ui4_count;
            rMsg.u4Cond = (UINT32)BLKDEV_COND_WRITE_OK;
            rMsg.pvData = pvData;
            
            x_msg_q_send(_hMusbNfyMsgq, &rMsg, sizeof(rMsg), 200);        	
            break;	
        }
        default:
            return -1;
    }
    return 0;
}

INT32 FM_MUSB_Init(VOID)
{
    static BOOL fgInit = FALSE;
    INT32 i4Ret = 0;

    if(!fgInit)
    {
        fgInit = TRUE;
        
        i4Ret = x_msg_q_create(&_hMusbNfyMsgq, "musbfmq", sizeof(FM_MUSB_MSG_T), 16);
        i4Ret = x_thread_create(&_hMusbNfyThread, "musbfmt", 1024, 200,
                   _MusbNfyThread, 0, (void *)NULL);
    }
    return i4Ret;
}

INT32 FM_MUSB_Open(HANDLE_T* hHandle)
{
    FM_MUSB_Init();
    *hHandle = _hDummyFileHandle;
    return 0;
}

