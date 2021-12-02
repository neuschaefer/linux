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
 * $RCSfile: musb_bt_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file musb_bt_cmd.c
 *  MUSB bluetooth module CLI test function.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN 

#include "drvcust_if.h"

#include "mu_bt_if.h"

#include "x_util.h"
#include "x_stl_lib.h"
#include "x_timer.h"
#include "x_assert.h"
#include "x_pinmux.h"
#include "x_typedef.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

LINT_EXT_HEADER_END 

#ifdef CC_USB_CSR_BLUETOOTH
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#define MGC_Printf Printf
//#define MGC_Printf
//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
#define MUSB_BT_ISO_PACKET 20
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

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
// Control transfer.
static UINT8 aCtrlSetup[8];
static UINT8 aCtrlData[16];
static UINT32 _u4BtCtrlCount = 0;
static MUSB_BT_IO_T rBtCtrlData;

// Interrupt transfer.
static UINT8 aIntrData[16];
static UINT32 u4BtIntrCount = 0;
static MUSB_BT_IO_T rBtIntrData;

// Bulk transfer.
static UINT8 aBulkOutData[512];
static UINT32 u4BtBulkOutCount = 0;
static MUSB_BT_IO_T rBtBulkOutData;

// Isochronous transfer.
static UINT8 aIsoOutData[512];
static UINT8 aIsoOutData1[512];
static UINT8 aIsoInData[512];
static UINT8 aIsoInData1[512];
static MUSB_BT_ISO_T arISOpacket[MUSB_BT_ISO_PACKET];
static MUSB_BT_ISO_T arISOpacket1[MUSB_BT_ISO_PACKET];
static UINT32 u4BtIsoOutCount[2] = {0, 1};
static UINT32 u4BtIsoInCount[2] = {0, 1};
static MUSB_BT_IO_T rBtIsoOutData[2];
static MUSB_BT_IO_T rBtIsoInData[2];


static HANDLE_T Bt_hSema;
static BOOL Bt_fgInit = FALSE;

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
static void _MUSBBtIntrCallback(void* pBtIOCompleteParam, struct _MUSB_BT_IO_T *prBtIOData)
{
    UINT32 u4IntrCount = *((UINT32 *)pBtIOCompleteParam);
    
    Printf("Intr-%d len=%d, actual len=%d, status=%d.\n", 
        u4IntrCount, 
        prBtIOData->dwDataLength, prBtIOData->dwActualDataLength, prBtIOData->dwStatus);
    
    *((UINT32 *)pBtIOCompleteParam) += 1;
}

//-------------------------------------------------------------------------
/** _BtIntrIn
 *  USB Bluetooth control out function.
 *  @param  void
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static UINT32 _BtIntrIn(void)
{
    MU_BT_DEVICE_TYPE *prBtDriver;

    prBtDriver = MUSB_BtDriverGet();

    rBtIntrData.bTrafficType = MUSB_BT_XFER_INT;
    rBtIntrData.bIsTx = FALSE;

    rBtIntrData.pSetupPacket = NULL;
    
    // data phase.
    rBtIntrData.pDataBuffer = aIntrData;
    rBtIntrData.dwDataLength = 16;

    rBtIntrData.pfBtIOComplete = _MUSBBtIntrCallback;
    rBtIntrData.pBtIOCompleteParam = &u4BtIntrCount;
    
    return prBtDriver->pfBtIOTransfer(0, &rBtIntrData);
}

static void _MUSBBtCtrlCallback(void* pBtIOCompleteParam, struct _MUSB_BT_IO_T *prBtIOData)
{
    UINT32 u4CtrlCount = *((UINT32 *)pBtIOCompleteParam);
    
    Printf("Ctrl-%d len=%d, actual len=%d, status=%d.\n", 
        u4CtrlCount, 
        prBtIOData->dwDataLength, prBtIOData->dwActualDataLength, prBtIOData->dwStatus);

    VERIFY(OSR_OK == x_sema_unlock(Bt_hSema));    
}

//-------------------------------------------------------------------------
/** _BtCtrlOut
 *  USB Bluetooth control out function.
 *  @param  u1Size		out data size.
 *  @param  pCtrlData	out data pointer.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _BtCtrlOut(UINT8 u1Size, UINT8 *pCtrlData)
{
    UINT32 u4Status;
    
    MU_BT_DEVICE_TYPE *prBtDriver;

    prBtDriver = MUSB_BtDriverGet();

    rBtCtrlData.bTrafficType = MUSB_BT_XFER_CONTROL;
    rBtCtrlData.bIsTx = TRUE;

    // setup phase.
    x_memset(aCtrlSetup, 0, 8);
    aCtrlSetup[0] = 0x20;
    aCtrlSetup[6] = u1Size;
    rBtCtrlData.pSetupPacket = aCtrlSetup;

    // data phase.
    rBtCtrlData.pDataBuffer = pCtrlData;
    rBtCtrlData.dwDataLength = u1Size;

    rBtCtrlData.pfBtIOComplete = _MUSBBtCtrlCallback;
    _u4BtCtrlCount ++;
    rBtCtrlData.pBtIOCompleteParam = &_u4BtCtrlCount;
    
    u4Status = prBtDriver->pfBtIOTransfer(0, &rBtCtrlData);

    if (0 == u4Status)
    {
        VERIFY(OSR_OK == x_sema_lock(Bt_hSema, X_SEMA_OPTION_WAIT));
    }
    
    return 0;
}

//-------------------------------------------------------------------------
/** _MUSBBtTestCtrlIntrCmd
 *  USB Bluetooth test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBBtTestCtrlIntrCmd(INT32 i4Argc, const CHAR **szArgv)
{
    MU_BT_DEVICE_TYPE *prBtDriver;
    UINT32 u4Status;

    prBtDriver = MUSB_BtDriverGet();

    prBtDriver->pfInitDevice(0);

    if (!Bt_fgInit)
    {
        Bt_fgInit = TRUE;
        VERIFY(OSR_OK == x_sema_create((HANDLE_T *)&Bt_hSema, \
            X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK));
    }

    u4Status = _BtIntrIn();
    if (0 != u4Status)
    {
        Printf("Interrupt in error.\n");
        return 0;
    }
    
    // Control out data.
    x_memset(aCtrlData, 0, 16);
    aCtrlData[0] = 0x03;
    aCtrlData[1] = 0x10;
    _BtCtrlOut(3, aCtrlData);

    // Control out data.
    x_memset(aCtrlData, 0, 16);
    aCtrlData[0] = 0x05;
    aCtrlData[1] = 0x10;
    _BtCtrlOut(3, aCtrlData);

    // Control out data.
    x_memset(aCtrlData, 0, 16);
    aCtrlData[0] = 0x09;
    aCtrlData[1] = 0x10;
    _BtCtrlOut(3, aCtrlData);

    // Control out data.
    x_memset(aCtrlData, 0, 16);
    aCtrlData[0] = 0x25;
    aCtrlData[1] = 0x0C;
    _BtCtrlOut(3, aCtrlData);

    // Control out data.
    x_memset(aCtrlData, 0, 16);
    aCtrlData[0] = 0x05;
    aCtrlData[1] = 0x0C;
    aCtrlData[2] = 0x02;
    aCtrlData[3] = 0x00;
    aCtrlData[4] = 0x8E;
    _BtCtrlOut(5, aCtrlData);

    // Control out data.
    x_memset(aCtrlData, 0, 16);
    aCtrlData[0] = 0x18;
    aCtrlData[1] = 0x0C;
    aCtrlData[2] = 0x02;
    aCtrlData[3] = 0x00;
    aCtrlData[4] = 0x80;
    _BtCtrlOut(5, aCtrlData);

    // Control out data.
    x_memset(aCtrlData, 0, 16);
    aCtrlData[0] = 0x16;
    aCtrlData[1] = 0x0C;
    aCtrlData[2] = 0x02;
    aCtrlData[3] = 0x00;
    aCtrlData[4] = 0x7D;
    _BtCtrlOut(5, aCtrlData);

    return 0;
}

static void _MUSBBtBulkCallback(void* pBtIOCompleteParam, struct _MUSB_BT_IO_T *prBtIOData)
{
    UINT32 u4BulkCount = *((UINT32 *)pBtIOCompleteParam);
    
    Printf("Bulk-%d len=%d, actual len=%d, status=%d.\n", 
        u4BulkCount, 
        prBtIOData->dwDataLength, prBtIOData->dwActualDataLength, prBtIOData->dwStatus);
    
    *((UINT32 *)pBtIOCompleteParam) += 1;
}

//-------------------------------------------------------------------------
/** _BtBulk
 *  USB Bluetooth bulk function.
 *  @param  bIsTx		1 = out, 0 = in.
 *  @param  u1Size		out data size.
 *  @param  pBulkData	out data pointer.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static UINT32 _BtBulk(UINT8 bIsTx, UINT32 u4Size, UINT8 *pBulkData)
{
    MU_BT_DEVICE_TYPE *prBtDriver;

    prBtDriver = MUSB_BtDriverGet();

    rBtBulkOutData.bTrafficType = MUSB_BT_XFER_BULK;
    rBtBulkOutData.bIsTx = bIsTx;

    rBtBulkOutData.pSetupPacket = NULL;
    
    // data phase.
    rBtBulkOutData.pDataBuffer = pBulkData;
    rBtBulkOutData.dwDataLength = u4Size;

    rBtBulkOutData.pfBtIOComplete = _MUSBBtBulkCallback;
    rBtBulkOutData.pBtIOCompleteParam = &u4BtBulkOutCount;
    
    return prBtDriver->pfBtIOTransfer(0, &rBtBulkOutData);
}

//-------------------------------------------------------------------------
/** _MUSBBtTestBulkOutCmd
 *  USB Bluetooth bulk out test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBBtTestBulkOutCmd(INT32 i4Argc, const CHAR **szArgv)
{
    MU_BT_DEVICE_TYPE *prBtDriver;

    prBtDriver = MUSB_BtDriverGet();

    prBtDriver->pfInitDevice(0);

    if (!Bt_fgInit)
    {
        Bt_fgInit = TRUE;
        VERIFY(OSR_OK == x_sema_create((HANDLE_T *)&Bt_hSema, \
            X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK));
    }
   
    // Control out data.
    x_memset(aBulkOutData, 0x55, 512);
    _BtBulk(TRUE, 512, aBulkOutData);

    return 0;
}

//-------------------------------------------------------------------------
/** _MUSBBtTestBulkInCmd
 *  USB Bluetooth bulk In test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBBtTestBulkInCmd(INT32 i4Argc, const CHAR **szArgv)
{
    MU_BT_DEVICE_TYPE *prBtDriver;

    prBtDriver = MUSB_BtDriverGet();

    prBtDriver->pfInitDevice(0);

    if (!Bt_fgInit)
    {
        Bt_fgInit = TRUE;
        VERIFY(OSR_OK == x_sema_create((HANDLE_T *)&Bt_hSema, \
            X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK));
    }
   
    // Control out data.
    x_memset(aBulkOutData, 0x55, 512);
    _BtBulk(FALSE, 512, aBulkOutData);

    return 0;
}

static void _MUSBBtIsoOutCallback(void* pBtIOCompleteParam, struct _MUSB_BT_IO_T *prBtIOData)
{
    /*
    UINT32 u4IsoCount = *((UINT32 *)pBtIOCompleteParam);
    UINT32 i;
    MUSB_BT_ISO_T *prISOpacket; 
    */
    MU_BT_DEVICE_TYPE *prBtDriver;
    UINT32 u4Status;

    /*
    prISOpacket = prBtIOData->prISOpacket;
    for (i=0; i<prBtIOData->dwNofPackets; i++)
    {        
        Printf("IsoOut-%d, pkt-%d, len=%d, actual len=%d, status=%d.\n", 
            u4IsoCount, i,
            prISOpacket[i].dwDataLength, prISOpacket[i].dwActualDataLength, prISOpacket[i].dwStatus);
    }
    */

    // retransmit.
    prBtDriver = MUSB_BtDriverGet();
    u4Status = prBtDriver->pfBtIOTransfer(0, prBtIOData);
    if (u4Status)
    {
        Printf("Iso tx 1 error.\n");
    }    
}

//-------------------------------------------------------------------------
/** _MUSBBtTestIsoOutCmd
 *  USB Bluetooth Isochronous Out test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBBtTestIsoOutCmd(INT32 i4Argc, const CHAR **szArgv)
{
    MU_BT_DEVICE_TYPE *prBtDriver;
    UINT32 u4Status;
    UINT32 i;

    prBtDriver = MUSB_BtDriverGet();

    prBtDriver->pfInitDevice(0);

    rBtCtrlData.bTrafficType = MUSB_BT_XFER_CONTROL;
    rBtCtrlData.bIsTx = TRUE;

    // setup phase.
    x_memset(aCtrlSetup, 0, 8);
    aCtrlSetup[0] = 0x01;
    aCtrlSetup[1] = 0x0B;
    aCtrlSetup[2] = 0x02;
    aCtrlSetup[3] = 0x00;
    aCtrlSetup[4] = 0x01;
    rBtCtrlData.pSetupPacket = aCtrlSetup;

    // data phase.
    rBtCtrlData.pDataBuffer = NULL;
    rBtCtrlData.dwDataLength = 0;

    rBtCtrlData.pfBtIOComplete = _MUSBBtCtrlCallback;
    _u4BtCtrlCount ++;
    rBtCtrlData.pBtIOCompleteParam = &_u4BtCtrlCount;
    
    u4Status = prBtDriver->pfBtIOTransfer(0, &rBtCtrlData);
    if (0 == u4Status)
    {
        VERIFY(OSR_OK == x_sema_lock(Bt_hSema, X_SEMA_OPTION_WAIT));
    }

    // Init data phase.
    for (i=0; i<512; i++)
    {
        aIsoOutData[i] = i & 0xFF;
    }

    // Iso rx 1st transfer packet.
    rBtIsoOutData[0].bTrafficType = MUSB_BT_XFER_ISOC;
    rBtIsoOutData[0].bIsTx = TRUE;
    rBtIsoOutData[0].pSetupPacket = NULL;   
    rBtIsoOutData[0].pDataBuffer = aIsoOutData;
    rBtIsoOutData[0].dwNofPackets = MUSB_BT_ISO_PACKET;
    rBtIsoOutData[0].prISOpacket = arISOpacket;
    for (i=0; i<MUSB_BT_ISO_PACKET; i++)
    {
        rBtIsoOutData[0].prISOpacket[i].dwDataLength = 12;
        rBtIsoOutData[0].prISOpacket[i].dwActualDataLength = 0;
        rBtIsoOutData[0].prISOpacket[i].dwStatus = 0;
    }    
    rBtIsoOutData[0].pfBtIOComplete = _MUSBBtIsoOutCallback;
    rBtIsoOutData[0].pBtIOCompleteParam = &u4BtIsoOutCount[0];
   
    u4Status = prBtDriver->pfBtIOTransfer(0, &rBtIsoOutData[0]);
    if (u4Status)
    {
        Printf("Iso tx 1 error.\n");
    }

    // Init data phase.
    for (i=0; i<512; i++)
    {
        aIsoOutData1[i] = 0x80 | (i & 0xFF);
    }

    // Iso rx 2nd transfer packet.
    rBtIsoOutData[1].bTrafficType = MUSB_BT_XFER_ISOC;
    rBtIsoOutData[1].bIsTx = TRUE;
    rBtIsoOutData[1].pSetupPacket = NULL;   
    rBtIsoOutData[1].pDataBuffer = aIsoOutData1;
    rBtIsoOutData[1].dwNofPackets = MUSB_BT_ISO_PACKET;
    rBtIsoOutData[1].prISOpacket = arISOpacket1;
    for (i=0; i<MUSB_BT_ISO_PACKET; i++)
    {
        rBtIsoOutData[1].prISOpacket[i].dwDataLength = 15;
        rBtIsoOutData[1].prISOpacket[i].dwActualDataLength = 0;
        rBtIsoOutData[1].prISOpacket[i].dwStatus = 0;
    }    
    rBtIsoOutData[1].pfBtIOComplete = _MUSBBtIsoOutCallback;
    rBtIsoOutData[1].pBtIOCompleteParam = &u4BtIsoOutCount[1];
   
    u4Status = prBtDriver->pfBtIOTransfer(0, &rBtIsoOutData[1]);
    if (u4Status)
    {
        Printf("Iso rx 2 error.\n");
    }

    return 0;
}

static void _MUSBBtIsoInCallback(void* pBtIOCompleteParam, struct _MUSB_BT_IO_T *prBtIOData)
{
    /*
    UINT32 u4IsoCount = *((UINT32 *)pBtIOCompleteParam);
    UINT32 i;
    MUSB_BT_ISO_T *prISOpacket; 
    */
    MU_BT_DEVICE_TYPE *prBtDriver;
    UINT32 u4Status;

    /*
    prISOpacket = prBtIOData->prISOpacket;
    for (i=0; i<prBtIOData->dwNofPackets; i++)
    {        
        Printf("IsoIn-%d, pkt-%d, len=%d, actual len=%d, status=%d.\n", 
            u4IsoCount, i,
            prISOpacket[i].dwDataLength, prISOpacket[i].dwActualDataLength, prISOpacket[i].dwStatus);
    }    
    */
    
    // retransmit.
    prBtDriver = MUSB_BtDriverGet();
    u4Status = prBtDriver->pfBtIOTransfer(0, prBtIOData);
    if (u4Status)
    {
        Printf("Iso rx 1 error.\n");
    }    
}

//-------------------------------------------------------------------------
/** _MUSBBtTestIsoInCmd
 *  USB Bluetooth Isochronous In test function.
 *  @param  i4Argc		user's CLI input length.
 *  @param  szArgv		user's CLI input string.
 *  @retval  1 	SUCCESS.
 *  @retval  0	FAIL. 
 */
//-------------------------------------------------------------------------
static INT32 _MUSBBtTestIsoInCmd(INT32 i4Argc, const CHAR **szArgv)
{
    MU_BT_DEVICE_TYPE *prBtDriver;
    UINT32 u4Status;
    UINT32 i;

    prBtDriver = MUSB_BtDriverGet();

    prBtDriver->pfInitDevice(0);

    rBtCtrlData.bTrafficType = MUSB_BT_XFER_CONTROL;
    rBtCtrlData.bIsTx = TRUE;

    // setup phase.
    x_memset(aCtrlSetup, 0, 8);
    aCtrlSetup[0] = 0x01;
    aCtrlSetup[1] = 0x0B;
    aCtrlSetup[2] = 0x02;
    aCtrlSetup[3] = 0x00;
    aCtrlSetup[4] = 0x01;
    rBtCtrlData.pSetupPacket = aCtrlSetup;

    // data phase.
    rBtCtrlData.pDataBuffer = NULL;
    rBtCtrlData.dwDataLength = 0;

    rBtCtrlData.pfBtIOComplete = _MUSBBtCtrlCallback;
    _u4BtCtrlCount ++;
    rBtCtrlData.pBtIOCompleteParam = &_u4BtCtrlCount;
    
    u4Status = prBtDriver->pfBtIOTransfer(0, &rBtCtrlData);
    if (0 == u4Status)
    {
        VERIFY(OSR_OK == x_sema_lock(Bt_hSema, X_SEMA_OPTION_WAIT));
    }

    // Init data phase.
    for (i=0; i<512; i++)
    {
        aIsoInData[i] = i & 0xFF;
    }

    // Iso rx 1st transfer packet.
    rBtIsoInData[0].bTrafficType = MUSB_BT_XFER_ISOC;
    rBtIsoInData[0].bIsTx = FALSE;
    rBtIsoInData[0].pSetupPacket = NULL;   
    rBtIsoInData[0].pDataBuffer = aIsoInData;
    rBtIsoInData[0].dwNofPackets = MUSB_BT_ISO_PACKET;
    rBtIsoInData[0].prISOpacket = arISOpacket;
    for (i=0; i<MUSB_BT_ISO_PACKET; i++)
    {
        rBtIsoInData[0].prISOpacket[i].dwDataLength = 12;
        rBtIsoInData[0].prISOpacket[i].dwActualDataLength = 0;
        rBtIsoInData[0].prISOpacket[i].dwStatus = 0;
    }    
    rBtIsoInData[0].pfBtIOComplete = _MUSBBtIsoInCallback;
    rBtIsoInData[0].pBtIOCompleteParam = &u4BtIsoInCount[0];
   
    u4Status = prBtDriver->pfBtIOTransfer(0, &rBtIsoInData[0]);
    if (u4Status)
    {
        Printf("Iso rx 1 error.\n");
    }

    // Init data phase.
    for (i=0; i<512; i++)
    {
        aIsoInData1[i] = 0x80 | (i & 0xFF);
    }

    // Iso rx 2nd transfer packet.
    rBtIsoInData[1].bTrafficType = MUSB_BT_XFER_ISOC;
    rBtIsoInData[1].bIsTx = FALSE;
    rBtIsoInData[1].pSetupPacket = NULL;   
    rBtIsoInData[1].pDataBuffer = aIsoInData1;
    rBtIsoInData[1].dwNofPackets = MUSB_BT_ISO_PACKET;
    rBtIsoInData[1].prISOpacket = arISOpacket1;
    for (i=0; i<MUSB_BT_ISO_PACKET; i++)
    {
        rBtIsoInData[1].prISOpacket[i].dwDataLength = 15;
        rBtIsoInData[1].prISOpacket[i].dwActualDataLength = 0;
        rBtIsoInData[1].prISOpacket[i].dwStatus = 0;
    }    
    rBtIsoInData[1].pfBtIOComplete = _MUSBBtIsoInCallback;
    rBtIsoInData[1].pBtIOCompleteParam = &u4BtIsoInCount[1];
   
    u4Status = prBtDriver->pfBtIOTransfer(0, &rBtIsoInData[1]);
    if (u4Status)
    {
        Printf("Iso rx 2 error.\n");
    }

    return 0;
}
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
/* Declare the debug on/off/level and RegTest functions */

LINT_EXT_HEADER_BEGIN 


static CLI_EXEC_T _arMUSBBtCmdTbl [] =
{
    { "start", "s", _MUSBBtTestCtrlIntrCmd, NULL, "MUSB bluetooth start", CLI_GUEST },
    { "bulkout", "bo", _MUSBBtTestBulkOutCmd, NULL, "MUSB bluetooth bulk out test", CLI_GUEST },
    { "bulkin", "bi", _MUSBBtTestBulkInCmd, NULL, "MUSB bluetooth bulk in test", CLI_GUEST },
    { "Isoout", "isoout", _MUSBBtTestIsoOutCmd, NULL, "MUSB bluetooth iso out test", CLI_GUEST },
    { "Isoin", "isoin", _MUSBBtTestIsoInCmd, NULL, "MUSB bluetooth iso in test", CLI_GUEST },

    { NULL, NULL, NULL, NULL, NULL, CLI_GUEST }
};

#else
static CLI_EXEC_T _arMUSBBtCmdTbl [] =
{
    { NULL, NULL, NULL, NULL, NULL, CLI_GUEST }
};

#endif


CLI_MAIN_COMMAND_ITEM(rBluetooth)
{
    "bt", NULL, NULL, _arMUSBBtCmdTbl, "Bluetooth command", CLI_GUEST
};

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
LINT_EXT_HEADER_END

