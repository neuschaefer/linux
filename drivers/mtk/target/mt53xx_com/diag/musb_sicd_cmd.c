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
 * $RCSfile: musb_sicd_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file musb_cmd.c
 *  MUSB module CLI test function.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN 

#include "musb_if.h"
#include "mu_cdi.h"
#include "mu_impl.h"

#include "mu_hfi_sicd_if.h"
#include "mu_sicdstate.h"
#include "mu_sicdapi.h"
#include "mu_sicdutil.h"

#include "x_util.h"
#include "x_stl_lib.h"
#include "x_timer.h"

#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

LINT_EXT_HEADER_END 

#ifndef CC_USB_DISABLE

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

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
extern MGC_UcFsDevice* MUSB_UcFsSicdDeviceGet(void);

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------
static VOID _MUSBMtpGetObjectTestTimerCB(HANDLE_T h_timer, VOID*    pv_tag);
static VOID _MUSBMtpGetPartialObjectTestTimerCB(HANDLE_T h_timer, VOID*    pv_tag);

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
static UINT32 dwSicdBufSize = 4096;
static UINT32 dwTransactionId=0;
static HAL_TIME_T SicdTimeStart, SicdTimeEnd, SicdTimeDelta;
static BOOL bMeasureTime = FALSE;
static BOOL bGetObjHandles = FALSE;
static BOOL bGetDeviceInfo = FALSE;
static UINT32 dwGetObjectRepeatTime;
static UINT32 dwGetPartialObjectRepeatTime;
static UINT8 dwGetObjectObjHnd[8];
static HANDLE_T h_get_obj_timer = 0;
static UINT8* pvSicdPayLoadBuf = 0;
static BOOL bGetObjectTest = FALSE;
static BOOL bGetPartialObjectTest = FALSE;
static UINT32 dwGetPartialObjectTestReadLength;
static UINT32 dwGetPartialObjectTestReadLength2;
static HANDLE_T phSicdThreadHdl;
static HANDLE_T phSicdSemaHdl;
static UINT32 dwRecvDataSize =0;

static uint8_t _MUSB_PicContent[715] = 
{
0xFF ,0xD8 ,0xFF ,0xE0 ,0x00 ,0x10 ,0x4A ,0x46 ,0x49 ,0x46 ,0x00 ,0x01 ,0x02 ,0x00 ,0x00 ,0x64 ,0x00 ,0x64 ,0x00 ,0x00 ,0xFF ,0xEC ,0x00 ,0x11 ,0x44 ,0x75 ,0x63 ,0x6B ,0x79 ,0x00 ,0x01 ,0x00 ,0x04 ,0x00 ,0x00 ,0x00,
0x3C ,0x00 ,0x00 ,0xFF ,0xEE ,0x00 ,0x0E ,0x41 ,0x64 ,0x6F ,0x62 ,0x65 ,0x00 ,0x64 ,0xC0 ,0x00 ,0x00 ,0x00 ,0x01 ,0xFF ,0xDB ,0x00 ,0x84 ,0x00 ,0x06 ,0x04 ,0x04 ,0x04 ,0x05 ,0x04 ,0x06 ,0x05 ,0x05 ,0x06 ,0x09 ,0x06 ,0x05 ,0x06 ,0x09 ,0x0B ,0x08 ,0x06 ,0x06 ,0x08 ,0x0B ,0x0C ,0x0A ,0x0A,
0x0B ,0x0A ,0x0A ,0x0C ,0x10 ,0x0C ,0x0C ,0x0C ,0x0C ,0x0C ,0x0C ,0x10 ,0x0C ,0x0E ,0x0F ,0x10 ,0x0F ,0x0E ,0x0C ,0x13 ,0x13 ,0x14 ,0x14 ,0x13 ,0x13 ,0x1C ,0x1B ,0x1B ,0x1B ,0x1C ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x01 ,0x07 ,0x07 ,0x07 ,0x0D ,0x0C ,0x0D ,0x18,
0x10 ,0x10 ,0x18 ,0x1A ,0x15 ,0x11 ,0x15 ,0x1A ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F,
0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0x1F ,0xFF ,0xC0 ,0x00 ,0x11 ,0x08 ,0x00 ,0x20 ,0x00 ,0x20 ,0x03 ,0x01 ,0x11 ,0x00 ,0x02 ,0x11 ,0x01 ,0x03 ,0x11 ,0x01 ,0xFF ,0xC4 ,0x00 ,0x68 ,0x00 ,0x00 ,0x01 ,0x04 ,0x03 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
0x00 ,0x01 ,0x03 ,0x04 ,0x05 ,0x07 ,0x00 ,0x06 ,0x08 ,0x01 ,0x01 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x10 ,0x00 ,0x02 ,0x01 ,0x03 ,0x02 ,0x06 ,0x01 ,0x03 ,0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x01 ,0x02 ,0x03,
0x11 ,0x04 ,0x05 ,0x00 ,0x21 ,0x31 ,0x12 ,0x22 ,0x13 ,0x06 ,0x07 ,0x14 ,0x41 ,0x32 ,0x52 ,0x51 ,0x71 ,0xA1 ,0x23 ,0x33 ,0x11 ,0x01 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0xFF ,0xDA ,0x00 ,0x0C ,0x03 ,0x01 ,0x00 ,0x02 ,0x11 ,0x03,
0x11 ,0x00 ,0x3F ,0x00 ,0xEA ,0x9D ,0x03 ,0x4C ,0x86 ,0x56 ,0xC3 ,0x1E ,0x81 ,0xAE ,0xE4 ,0xE4 ,0x05 ,0x59 ,0xC0 ,0x00 ,0xB1 ,0x2A ,0x94 ,0x2C ,0x76 ,0x07 ,0x85 ,0x46 ,0x80 ,0x63 ,0xB3 ,0x18 ,0xCC ,0x95 ,0x84 ,0x79 ,0x0B ,0x2B ,0x84 ,0x9A ,0xCE ,0x55 ,0x0C ,0x92 ,0x8E ,0x91 ,0x46 ,0x15,
0x15 ,0x0D ,0x42 ,0x2A ,0x0F ,0xD7 ,0x40 ,0xF3 ,0x41 ,0x9A ,0x0A ,0xE3 ,0xD9 ,0xB9 ,0xA1 ,0x0C ,0x17 ,0x31 ,0xC7 ,0x6D ,0xF2 ,0x66 ,0x77 ,0x83 ,0x17 ,0x02 ,0x77 ,0x1D ,0x01 ,0x92 ,0xED ,0x94 ,0xB7 ,0x52 ,0x7D ,0xBF ,0x7A ,0x6F ,0xA0 ,0x9C ,0xC1 ,0xF8 ,0xED ,0xBC ,0x56 ,0x96 ,0x69 ,0x03,
0x01 ,0x66 ,0xD2 ,0xAC ,0xC8 ,0x8C ,0xBD ,0x66 ,0x38 ,0x80 ,0xED ,0x29 ,0xA6 ,0xD4 ,0x0B ,0x1A ,0x8A ,0xFF ,0x00 ,0x1A ,0x0D ,0xAF ,0x41 ,0x56 ,0x67 ,0xF2 ,0x5E ,0xD5 ,0x4F ,0x26 ,0xC8 ,0xD8 ,0x63 ,0x67 ,0x46 ,0xC7 ,0x2C ,0xAF ,0x05 ,0x93 ,0x56 ,0xD0 ,0x4E ,0xDD ,0xEB ,0x6F ,0x9F ,0x1B,
0x2A ,0xB5 ,0x5B ,0xFA ,0xFE ,0x2C ,0xB6 ,0xAA ,0x18 ,0x6F ,0xDC ,0x12 ,0x6F ,0xC9 ,0x50 ,0x03 ,0x23 ,0xE4 ,0x9E ,0xE3 ,0x92 ,0x25 ,0x8E ,0x3C ,0x35 ,0xA9 ,0x20 ,0xC5 ,0x34 ,0x89 ,0x1A ,0xC6 ,0x55 ,0xBA ,0xD1 ,0xA4 ,0xB3 ,0x9D ,0xE5 ,0xB8 ,0x75 ,0x86 ,0x90 ,0xF3 ,0x33 ,0x4B ,0xCB ,0xD5,
0x50 ,0x14 ,0x29 ,0xEA ,0x25 ,0x0B ,0x6F ,0x2C ,0xF6 ,0xC4 ,0xD1 ,0x93 ,0x8E ,0xC0 ,0x63 ,0xE7 ,0xB5 ,0x8D ,0x24 ,0xF8 ,0xF9 ,0x00 ,0x59 ,0x54 ,0x28 ,0x05 ,0xA3 ,0x57 ,0xB5 ,0x49 ,0xA5 ,0x96 ,0x37 ,0x64 ,0x80 ,0x86 ,0x8C ,0x12 ,0x54 ,0xCB ,0x17 ,0xE2 ,0xFA ,0x22 ,0xC8 ,0xC2 ,0x5D ,0x5F,
0x5D ,0xE1 ,0xEC ,0xAE ,0x72 ,0x16 ,0xC6 ,0xCE ,0xFE ,0x68 ,0x23 ,0x7B ,0xBB ,0x46 ,0xE5 ,0xAC ,0x52 ,0xB2 ,0x02 ,0xE8 ,0x79 ,0x5A ,0x41 ,0xD2 ,0xC7 ,0xF2 ,0x3A ,0x08 ,0x9C ,0xD7 ,0xAF ,0xBC ,0x63 ,0x33 ,0x90 ,0x7C ,0x8D ,0xF5 ,0xBB ,0xBD ,0xE3 ,0x84 ,0x06 ,0x44 ,0x96 ,0x44 ,0xFF ,0x00,
0x30 ,0x42 ,0x1E ,0x55 ,0x21 ,0x6A ,0x03 ,0x30 ,0xDC ,0x70 ,0x24 ,0x70 ,0x27 ,0x40 ,0x95 ,0xAF ,0xAE ,0x3C ,0x62 ,0xD6 ,0xE9 ,0x2E ,0x61 ,0x8E ,0x65 ,0x78 ,0xC3 ,0xAA ,0xAF ,0x79 ,0xCA ,0xD1 ,0xE1 ,0x30 ,0x6E ,0x2B ,0xB9 ,0x11 ,0xB5 ,0x2A ,0x77 ,0x3C ,0x49 ,0x27 ,0x40 ,0x9D ,0xCF ,0xAC,
0x3C ,0x4E ,0x79 ,0xDA ,0x5E ,0xCC ,0x91 ,0x2B ,0x80 ,0x92 ,0xC5 ,0x14 ,0xAE ,0x88 ,0xCB ,0xCA ,0xAA ,0xC2 ,0x8A ,0x45 ,0x0C ,0x8A ,0x80 ,0x39 ,0xE2 ,0xDB ,0xFE ,0xBA ,0x09 ,0xBC ,0x0E ,0x03 ,0x19 ,0x83 ,0xB2 ,0x6B ,0x3C ,0x74 ,0x66 ,0x38 ,0x5E ,0x46 ,0x95 ,0xF9 ,0x98 ,0xB3 ,0x33 ,0xB5,
0x2A ,0xC4 ,0x9F ,0xDB ,0x41 ,0xFF ,0xD9};

static uint8_t _MUSB_PropertyList[115] = 
{
0x05 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x07 ,0xDC,
0xFF ,0xFF ,0x08 ,0x31 ,0x00 ,0x32 ,0x00 ,0x33 ,0x00 ,0x2E,
0x00 ,0x6A ,0x00 ,0x70 ,0x00 ,0x67 ,0x00 ,0x00 ,0x00 ,0x00,
0x00 ,0x00 ,0x00 ,0x09 ,0xDC ,0xFF ,0xFF ,0x12 ,0x32 ,0x00,
0x30 ,0x00 ,0x30 ,0x00 ,0x36 ,0x00 ,0x30 ,0x00 ,0x34 ,0x00,
0x32 ,0x00, 0x35 ,0x00 ,0x54 ,0x00 ,0x31 ,0x00 ,0x30 ,0x00,
0x31 ,0x00 ,0x38 ,0x00 ,0x31 ,0x00 ,0x30 ,0x00 ,0x2E ,0x00,
0x30 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x87 ,0xDC,
0x06 ,0x00 ,0x20 ,0x00, 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
0x44 ,0xDC ,0xFF ,0xFF ,0x04 ,0x31 ,0x00 ,0x32 ,0x00 ,0x33,
0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x88 ,0xDC ,0x06,
0x00 ,0x20 ,0x00 ,0x00 ,0x00};

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------     
void _MUSB_PrintMtpDeviceInfo(void* pv_data)
{
    UINT32 i;
    UINT32 VendExtDes_Length;
    UINT32 OpsSuppCount;
    UINT32 OpsSuppCountOffset;
    UINT32 OpsSuppOffset;
    UINT16 OpCode;
    UINT8 OpsH;
    UINT8 OpsL;
    UINT8* pPayload;
    UINT8 bSupportGetPartialObject = FALSE;
            
    pPayload = ((MUSB_SICD_BLOCK_T*)pv_data)->pbPayLoadBuf;
    VendExtDes_Length = pPayload[8] *2;
    OpsSuppCountOffset = 8 + 1 + VendExtDes_Length + 2;
    OpsSuppOffset = 8 + 1 + VendExtDes_Length + 2 + 4;
    OpsSuppCount = pPayload[OpsSuppCountOffset];
    for ( i = 0 ; i < OpsSuppCount ; i++ )
    {
        OpsL = pPayload[OpsSuppOffset+(i*2)];
        OpsH = pPayload[OpsSuppOffset+(i*2)+1];
        OpCode = OpsH * 256 + OpsL;
        Printf("OpsSupported : 0x%X\n", (UINT16)OpCode);
        if ( 0x101B  == OpCode )
        {
            bSupportGetPartialObject = TRUE;
        }
    }

    if ( bSupportGetPartialObject )
    {
        Printf("This device supports GetPartialObject\n");
    }
    else
    {
        Printf("This device does not support GetPartialObject\n");
    }
}

void _MUSB_PrintMtpObjectHandles(void* pv_data)
{
    UINT32 i;
    UINT32 payload_length;
    UINT32* ptr;

    payload_length = ((MUSB_SICD_BLOCK_T*)pv_data)->dwPayLoadBufLen;
    for ( i = 4 ; i < payload_length ; i+=4 )
    {
        ptr = (UINT32*)&(((MUSB_SICD_BLOCK_T*)pv_data)->pbPayLoadBuf[i]);
        Printf("0x%X,  ", *ptr);
    }
    Printf("\n");    
}

void _MUSB_StartTimerForGetObjectTest(void)
{
        INT32 i4_return;
        
        x_timer_delete(h_get_obj_timer);
        
        i4_return = x_timer_create(&h_get_obj_timer);
        if (i4_return != OSR_OK)
        {
            Printf("timer create error!!\n");
            return;
        }
        
        i4_return = x_timer_start(h_get_obj_timer,
                                  1000,
                                  X_TIMER_FLAG_ONCE,
                                  _MUSBMtpGetObjectTestTimerCB,
                                  NULL);
        if (i4_return != OSR_OK)
        {
            Printf("timer start error!!\n");
            return;
        }
}

void _MUSB_StartTimerForGetPartialObjectTest(void)
{
        INT32 i4_return;
        
        x_timer_delete(h_get_obj_timer);
        
        i4_return = x_timer_create(&h_get_obj_timer);
        if (i4_return != OSR_OK)
        {
            Printf("timer create error!!\n");
            return;
        }
        
        i4_return = x_timer_start(h_get_obj_timer,
                                  1,
                                  X_TIMER_FLAG_ONCE,
                                  _MUSBMtpGetPartialObjectTestTimerCB,
                                  NULL);
        if (i4_return != OSR_OK)
        {
            Printf("timer start error!!\n");
            return;
        }
}

static void _MUSB_pfNfy(void *pvHfiNfyTag, int32_t e_nfy_cond, void *pv_data)
{
    UINT32 u4TotalTransferUs;
    UINT16 u2ResponseCode;

    switch ( e_nfy_cond )
    {
    case MUSB_HFI_COND_FAIL:
        Printf("_MUSB_pfNfy callback : Some error in data phase\n");
        
    case MUSB_HFI_COND_READ_OK:
        dwRecvDataSize += ((MUSB_SICD_BLOCK_T*)pv_data)->dwPayLoadBufLen;
        
        if ( bGetObjHandles )
        {
            bGetObjHandles = FALSE;
            _MUSB_PrintMtpObjectHandles(pv_data);
        }
        
        if ( bGetDeviceInfo )
        {
            bGetDeviceInfo = FALSE;
            _MUSB_PrintMtpDeviceInfo(pv_data);
        }
        break;
        
    case MUSB_HFI_COND_WRITE_OK:
        break;
    
    case MUSB_HFI_NFY_PHASE_CMD_COMPLETE:
        
        Printf("Command = 0x%04X Complete.\n", 
            ((MUSB_SICD_BLOCK_T *)pv_data)->wOperationCode);
        dwRecvDataSize = 0;
        break;
        
    case MUSB_HFI_NFY_PHASE_DATA_COMPLETE:
        if ( bMeasureTime )
        {
            bMeasureTime = FALSE;
            HAL_GetTime(&SicdTimeEnd);
            HAL_GetDeltaTime(&SicdTimeDelta, &SicdTimeStart, &SicdTimeEnd);
            u4TotalTransferUs = ((SicdTimeDelta.u4Seconds * 1000000) + SicdTimeDelta.u4Micros);
            Printf("Total time in micro seconds: %d\n", u4TotalTransferUs);
        }

        Printf("Data Phase Complete\n");
        
        dwRecvDataSize += ((MUSB_SICD_BLOCK_T*)pv_data)->dwPayLoadBufLen;

        Printf("Total Callback Data Size = %d\n", dwRecvDataSize);
        
        if ( bGetObjHandles )
        {
            bGetObjHandles = FALSE;
            _MUSB_PrintMtpObjectHandles(pv_data);
        }

        if ( bGetDeviceInfo )
        {
            bGetDeviceInfo = FALSE;
            _MUSB_PrintMtpDeviceInfo(pv_data);
        }

        // memory boundary checking
        {
            uint8_t* pbBufPtr = ((MUSB_SICD_BLOCK_T*)pv_data)->pbPayLoadBuf;
            uint32_t dwBufSize = dwSicdBufSize;
            if ( pbBufPtr[dwBufSize] != 'a' || pbBufPtr[dwBufSize+1] != 'b' ||
                    pbBufPtr[dwBufSize+2] != 'c' ||pbBufPtr[dwBufSize+3] != 'd' )
            {
                Printf("Memory boundary check error !!\n");
                ASSERT(0);
            }
        }
        
        break;
        
    case MUSB_HFI_NFY_PHASE_RESP_COMPLETE:
        u2ResponseCode = ((MUSB_SICD_BLOCK_T *)pv_data)->wOperationCode;
        Printf("Response = 0x%04X...", 
            u2ResponseCode);        
        
        if ( u2ResponseCode == 0x2001 )
            Printf(" OK\n");
        else if ( u2ResponseCode == 0x2000 )
            Printf(" Undefined\n");
        else if ( u2ResponseCode == 0x2002 )
            Printf(" General Error\n");
        else if ( u2ResponseCode == 0x2003 )
            Printf(" Session Not Open\n");
        else if ( u2ResponseCode == 0x2004 )
            Printf(" Invalid Transaction ID\n");
        else if ( u2ResponseCode == 0x2005 )
            Printf(" Operation Not Supported\n");
        else if ( u2ResponseCode == 0x2006 )
            Printf(" Parameter Not Supported\n");
        else if ( u2ResponseCode == 0x2007 )
            Printf(" Incomplete Transfer\n");
        else if ( u2ResponseCode == 0x2008 )
            Printf(" Invalid Storage ID\n");
        else if ( u2ResponseCode == 0x2009 )
            Printf(" Invalid ObjectHandle\n");
        else if ( u2ResponseCode == 0x201e )
            Printf(" Session already opened\n");
        else
            Printf(" Error !\n");
        
        if ( bGetObjectTest )
        {
            _MUSB_StartTimerForGetObjectTest();
        }

        if ( bGetPartialObjectTest )
        {
            _MUSB_StartTimerForGetPartialObjectTest();
        }
        
        break;
        
    case MUSB_HFI_NFY_GOT_EVENT:
        Printf("Got Event, Code = 0x%X \n", ((MUSB_SICD_BLOCK_T*)pv_data)->wOperationCode);
        break;
        
    case MUSB_HFI_NFY_OPERATION_ERROR:
        Printf("_MUSB_pfNfy callback : Operation Error\n");
        break;
        
    case MUSB_HFI_NFY_GET_EXT_EVENT_COMPLETE:
        Printf("_MUSB_pfNfy callback : Get Ext Event Complete\n");
        break;
        
    case MUSB_HFI_NFY_RESET_SUCCESS:
        Printf("_MUSB_pfNfy callback : Reset OK\n");
        break;
        
    case MUSB_HFI_NFY_RESET_FAIL:
        Printf("_MUSB_pfNfy callback : Reset Failed\n");
        break;
        
    case MUSB_HFI_NFY_OPERATION_ABORT_SUCCESS:
        Printf("_MUSB_pfNfy callback : Abort OK\n");
        break;
        
    case MUSB_HFI_NFY_OPERATION_ABORT_FAIL:
        Printf("_MUSB_pfNfy callback : Abort Failed\n");
        break;
    default: 
        break;
    }
    return;
}

static INT32 _MUSBMtpGetDevInfo(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 iResult;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;

    ASSERT(szArgv);

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;

    if (!pvSicdPayLoadBuf)
    {
        pvSicdPayLoadBuf = MUSB_MemAlloc(dwSicdBufSize);
        if ( pvSicdPayLoadBuf == NULL )
        {
            LOG(0, "_MUSBMtpGetDevInfo allocate memory failed\n");
            return 0;
        }
    }       
    
    MUSB_MemSet(pvSicdPayLoadBuf, 0, dwSicdBufSize);
    pvSicdPayLoadBuf[dwSicdBufSize] = 'a';
    pvSicdPayLoadBuf[dwSicdBufSize+1] = 'b';
    pvSicdPayLoadBuf[dwSicdBufSize+2] = 'c';
    pvSicdPayLoadBuf[dwSicdBufSize+3] = 'd';

    SicdIoData.u2OperationCode = 0x1001;
    SicdIoData.u2ParamNum = 0;
    SicdIoData.u4TransactionID = 0x00000000;
    SicdIoData.dwDataBufLen = dwSicdBufSize;
    SicdIoData.pbDataBuf = pvSicdPayLoadBuf;
    SicdIoData.eDataDirection = MUSB_SICD_DATA_IN;

    bGetDeviceInfo = TRUE;
    
    iResult = SicdDriver->pfRWBurst(0, MUSB_SICD_RW_SEND_OPERATION, (MUSB_IO_DATA_T *)&SicdIoData);
    if ( iResult )
        Printf("Result : %d\n", iResult);
    return iResult;

}

static INT32 _MUSBMtpSetBufSize(INT32 i4Argc, const CHAR **szArgv)
{
    UINT32 dwBufSize;
    ASSERT(szArgv);

    if ( i4Argc == 2 )
    {
        dwBufSize = StrToInt(szArgv[1]);
    }
    else
    {
        Printf("Usage : msbf 4B_object_handle\n");
        return 0;
    }

    if ( dwBufSize == 0x12345678 )
    {
        dwBufSize = dwGetPartialObjectTestReadLength2;
    }

    if ( pvSicdPayLoadBuf != 0 )
        MUSB_MemFree(pvSicdPayLoadBuf);

    dwSicdBufSize = dwBufSize;
    pvSicdPayLoadBuf = MUSB_MemAlloc(dwSicdBufSize+4);
    if ( pvSicdPayLoadBuf == NULL )
    {
        LOG(0, "_MUSBMtpSetBufSize allocate memory failed!\n");
        return 0;
    }
    MUSB_MemSet(pvSicdPayLoadBuf, 0, dwSicdBufSize);
    pvSicdPayLoadBuf[dwSicdBufSize] = 'a';
    pvSicdPayLoadBuf[dwSicdBufSize+1] = 'b';
    pvSicdPayLoadBuf[dwSicdBufSize+2] = 'c';
    pvSicdPayLoadBuf[dwSicdBufSize+3] = 'd';
    return 1;
}

static INT32 _MUSBMtpOpenSession(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 iResult;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;

    ASSERT(szArgv);

    if (!pvSicdPayLoadBuf)
    {
        pvSicdPayLoadBuf = MUSB_MemAlloc(dwSicdBufSize);
        if ( pvSicdPayLoadBuf == NULL )
        {
            LOG(0, "_MUSBMtpOpenSession allocate memory failed!\n");
            return 0;
        }
    }       
    
    MUSB_MemSet(pvSicdPayLoadBuf, 0, dwSicdBufSize);
    pvSicdPayLoadBuf[dwSicdBufSize] = 'a';
    pvSicdPayLoadBuf[dwSicdBufSize+1] = 'b';
    pvSicdPayLoadBuf[dwSicdBufSize+2] = 'c';
    pvSicdPayLoadBuf[dwSicdBufSize+3] = 'd';

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;

    SicdIoData.u2OperationCode = 0x1002;
    SicdIoData.u2ParamNum = 1;
    SicdIoData.u4TransactionID = 0x00000000;
    SicdIoData.u4Param[0] = 0x00000001;
    SicdIoData.dwDataBufLen = 32;
    SicdIoData.pbDataBuf = pvSicdPayLoadBuf;
    SicdIoData.eDataDirection = MUSB_SICD_NO_DATA;
    
    iResult = SicdDriver->pfRWBurst(0, MUSB_SICD_RW_SEND_OPERATION, (MUSB_IO_DATA_T *)&SicdIoData);
    if ( iResult )
        Printf("Result : %d\n", iResult);
    return iResult;
}

static INT32 _MUSBMtpCloseSession(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 iResult;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;

    ASSERT(szArgv);

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;

    SicdIoData.u2OperationCode = 0x1003;
    SicdIoData.u2ParamNum = 0;
    SicdIoData.u4TransactionID = 0x00000000;
    SicdIoData.dwDataBufLen = 32;
    SicdIoData.pbDataBuf = pvSicdPayLoadBuf;
    SicdIoData.eDataDirection = MUSB_SICD_NO_DATA;
    
    iResult = SicdDriver->pfRWBurst(0, MUSB_SICD_RW_SEND_OPERATION, (MUSB_IO_DATA_T *)&SicdIoData);
    if ( iResult )
        Printf("Result : %d\n", iResult);
    return iResult;
}

static VOID _MUSBMtpGetObjectTestTimerCB(HANDLE_T h_timer, VOID*    pv_tag)
{
    VERIFY(OSR_OK == x_sema_unlock(phSicdSemaHdl));
}

static VOID _MUSBMtpGetPartialObjectTestTimerCB(HANDLE_T h_timer, VOID*    pv_tag)
{
    VERIFY(OSR_OK == x_sema_unlock(phSicdSemaHdl));
}

static VOID _MUSBMtpGetObjectThread(VOID *pvArgv)
{
    UNUSED(pvArgv);
        
    while(1)
    {
        x_sema_lock(phSicdSemaHdl, X_SEMA_OPTION_WAIT);
        dwGetObjectRepeatTime++;
        Printf("\n\nAuto Execute : mgo %s, Remain %d times.\n", dwGetObjectObjHnd, dwGetObjectRepeatTime);
        CLI_Parser_Arg("mgo %s", dwGetObjectObjHnd);
    }
}

static VOID _MUSBMtpGetPartialObjectThread(VOID *pvArgv)
{
    UNUSED(pvArgv);

    dwGetPartialObjectTestReadLength = dwSicdBufSize;
    
    while(1)
    {
        x_sema_lock(phSicdSemaHdl, X_SEMA_OPTION_WAIT);
        dwGetPartialObjectRepeatTime++;
        Printf("\n\nAuto Execute : GetPartialObject, the %dth times.\n", dwGetPartialObjectRepeatTime);
        CLI_Parser_Arg("mgpo %s 00000000 00000000", dwGetObjectObjHnd);
        dwGetPartialObjectTestReadLength--;
    }
}

static VOID _MUSBMtpGetPartialObjectThread2(VOID *pvArgv)
{
    UNUSED(pvArgv);

    dwGetPartialObjectTestReadLength2 = 1;
    
    while(1)
    {
        x_sema_lock(phSicdSemaHdl, X_SEMA_OPTION_WAIT);
        dwGetPartialObjectRepeatTime++;
        Printf("\n\nAuto Execute : GetPartialObject, the %dth times.\n", dwGetPartialObjectRepeatTime);
        CLI_Parser_Arg("msbf 12345678");
        CLI_Parser_Arg("mgpo %s 00000000 12345678", dwGetObjectObjHnd);
        dwGetPartialObjectTestReadLength2++;
    }
}

static INT32 _MUSBMtpGetObjectTest(INT32 i4Argc, const CHAR **szArgv)
{
    static BOOL fgSicdGetObjTestInit = FALSE;
    INT32 i;
    ASSERT(szArgv);

    dwGetObjectRepeatTime = 0;
    
    if ( i4Argc == 2 )
    {
        for ( i = 0 ; i < 8 ; i++ )
        {
            dwGetObjectObjHnd[i] = szArgv[1][i];
        }
    }
    else
    {
        Printf("Usage : mgo 4B_object_handle\n");
        return 0;
    }

    if ( !fgSicdGetObjTestInit )
    {
        VERIFY(OSR_OK == x_sema_create(&phSicdSemaHdl, X_SEMA_TYPE_BINARY,
                                        X_SEMA_STATE_UNLOCK));
            
        UNUSED(x_thread_create(&phSicdThreadHdl, "USBVbusTest",
                                8092,  200,
                                _MUSBMtpGetObjectThread, 0,
                                NULL));
        
        bGetObjectTest = TRUE;
        fgSicdGetObjTestInit = TRUE;
    }
    else
    {
        VERIFY(OSR_OK == x_sema_unlock(phSicdSemaHdl));
    }

    return 1;
}

static INT32 _MUSBMtpGetObject(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 iResult;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;
    uint32_t ObjectHandle;

    ASSERT(szArgv);

    if ( i4Argc == 2 )
    {
        ObjectHandle = StrToInt(szArgv[1]);
    }
    else
    {
        Printf("Usage : mgo 4B_object_handle\n");
        return 0;
    }    

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;

    SicdIoData.u2OperationCode = 0x1009;
    SicdIoData.u2ParamNum = 1;
    SicdIoData.u4TransactionID = 0x00000001;
    SicdIoData.u4Param[0] = ObjectHandle;
    SicdIoData.dwDataBufLen = dwSicdBufSize;
    SicdIoData.pbDataBuf = pvSicdPayLoadBuf;
    SicdIoData.eDataDirection = MUSB_SICD_DATA_IN;

    HAL_GetTime(&SicdTimeStart);
    bMeasureTime = TRUE;
    
    iResult = SicdDriver->pfRWBurst(0, MUSB_SICD_RW_SEND_OPERATION, (MUSB_IO_DATA_T *)&SicdIoData);
    if ( iResult )
        Printf("Result : %d\n", iResult);
    return iResult;
}

static INT32 _MUSBMtpGetPartialObjectTest2(INT32 i4Argc, const CHAR **szArgv)
{
    static BOOL fgSicdGetPartialObjTestInit = FALSE;
    int i;

    dwGetPartialObjectRepeatTime = 0;

    if ( i4Argc == 2 )
    {
        for ( i = 0 ; i < 8 ; i++ )
        {
            dwGetObjectObjHnd[i] = szArgv[1][i];
        }
    }
    else
    {
        Printf("Usage : mgpt 4B_object_handle \n");
        Printf("Purpose : Test getting partial object with variable size (Accumulate).\n");
        return 0;
    }


    if ( fgSicdGetPartialObjTestInit == FALSE )
    {
        VERIFY(OSR_OK == x_sema_create(&phSicdSemaHdl, X_SEMA_TYPE_BINARY,
                                        X_SEMA_STATE_UNLOCK));
            
        UNUSED(x_thread_create(&phSicdThreadHdl, "USBVbusTest",
                                8092,  200,
                                _MUSBMtpGetPartialObjectThread2, 0,
                                NULL));
                                    
        bGetPartialObjectTest = TRUE;
        fgSicdGetPartialObjTestInit = TRUE;
    }
    else
    {
        VERIFY(OSR_OK == x_sema_unlock(phSicdSemaHdl));
    }

    return 1;
}

static INT32 _MUSBMtpGetPartialObjectTest(INT32 i4Argc, const CHAR **szArgv)
{
    static BOOL fgSicdGetPartialObjTestInit = FALSE;
    int i;

    dwGetPartialObjectRepeatTime = 0;

    if ( i4Argc == 2 )
    {
        for ( i = 0 ; i < 8 ; i++ )
        {
            dwGetObjectObjHnd[i] = szArgv[1][i];
        }
    }
    else
    {
        Printf("Usage : mgpt 4B_object_handle \n");
        Printf("Purpose : Test getting partial object with variable size (length size count down with fix buffer size.\n");
        return 0;
    }


    if ( fgSicdGetPartialObjTestInit == FALSE )
    {
        VERIFY(OSR_OK == x_sema_create(&phSicdSemaHdl, X_SEMA_TYPE_BINARY,
                                        X_SEMA_STATE_UNLOCK));
            
        UNUSED(x_thread_create(&phSicdThreadHdl, "USBVbusTest",
                                8092,  200,
                                _MUSBMtpGetPartialObjectThread, 0,
                                NULL));
                                    
        bGetPartialObjectTest = TRUE;
        fgSicdGetPartialObjTestInit = TRUE;
    }
    else
    {
        VERIFY(OSR_OK == x_sema_unlock(phSicdSemaHdl));
    }

    return 1;
}

static INT32 _MUSBMtpGetObjectInfo(INT32 i4Argc, const CHAR **szArgv)
{
    uint32_t ObjectHandle;
    INT32 iResult;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;
    
    ASSERT(szArgv);
    
    if ( i4Argc == 2 )
    {
        ObjectHandle = StrToInt(szArgv[1]);
    }
    else
    {
        Printf("Usage : mgoi 4B_object_handle\n");
        return 0;
    }

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;
    
    SicdIoData.u2OperationCode = 0x1008;
    SicdIoData.u2ParamNum = 1;
    SicdIoData.u4TransactionID = 0x00000001;
    SicdIoData.u4Param[0] = ObjectHandle;
    SicdIoData.dwDataBufLen = dwSicdBufSize;
    SicdIoData.pbDataBuf = pvSicdPayLoadBuf;
    SicdIoData.eDataDirection = MUSB_SICD_DATA_IN;
    
    iResult = SicdDriver->pfRWBurst(0, MUSB_SICD_RW_SEND_OPERATION, (MUSB_IO_DATA_T *)&SicdIoData);
    if ( iResult )
        Printf("Result : %d\n", iResult);
    return iResult;

}

static INT32 _MUSBMtpGetPartialObject(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 iResult;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;
    uint32_t ObjectHandle;
    uint32_t Offset;
    uint32_t ReadLength;

    ASSERT(szArgv);

    if ( i4Argc == 4 )
    {
        ObjectHandle = StrToInt(szArgv[1]);
        Offset = StrToInt(szArgv[2]);
        ReadLength = StrToInt(szArgv[3]);
    }
    else
    {
        Printf("Usage : mgpo 4B_object_handle 4B_offset 4B_length\n");
        return 0;
    }

    if ( ReadLength == 0 )
    {
        ReadLength = dwGetPartialObjectTestReadLength;
    }
    else if ( ReadLength == 0x12345678 )
    {
        ReadLength = dwGetPartialObjectTestReadLength2 ;
    }
    

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;

    dwTransactionId++;
    SicdIoData.u2OperationCode = 0x101B;
    SicdIoData.u2ParamNum = 3;
    SicdIoData.u4TransactionID = dwTransactionId;
    SicdIoData.u4Param[0] = ObjectHandle;
    SicdIoData.u4Param[1] = Offset;
    SicdIoData.u4Param[2] = ReadLength;
    SicdIoData.dwDataBufLen = dwSicdBufSize;
    SicdIoData.pbDataBuf = pvSicdPayLoadBuf;
    SicdIoData.eDataDirection = MUSB_SICD_DATA_IN;

    HAL_GetTime(&SicdTimeStart);
    bMeasureTime = TRUE;
    
    iResult = SicdDriver->pfRWBurst(0, MUSB_SICD_RW_SEND_OPERATION, (MUSB_IO_DATA_T *)&SicdIoData);
    if ( iResult )
        Printf("Result : %d\n", iResult);
    return iResult;
}

static INT32 _MUSBSicdDeviceReset(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 iResult;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;

    ASSERT(szArgv);

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;

    iResult = SicdDriver->pfRWBurst(0, MUSB_SICD_RW_SET_DEVICE_RESET, (MUSB_IO_DATA_T *)&SicdIoData);
    if ( iResult )
        Printf("Result : %d\n", iResult);
    return iResult;
}

static INT32 _MUSBSicdAbortOperation(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 iResult;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;

    ASSERT(szArgv);

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;

    iResult = SicdDriver->pfRWBurst(0, MUSB_SICD_RW_SET_OPERATION_ABORT, (MUSB_IO_DATA_T *)&SicdIoData);
    if ( iResult )
        Printf("Result : %d\n", iResult);
    return iResult;
}

static INT32 _MUSBMtpGetObjectHandles(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 iResult;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;

    ASSERT(szArgv);

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;

    SicdIoData.u2OperationCode = 0x1007;
    SicdIoData.u2ParamNum = 3;
    SicdIoData.u4TransactionID = 0x00000001;
    SicdIoData.u4Param[0] = 0xffffffff;
    SicdIoData.u4Param[1] = 0x00000000;
    SicdIoData.u4Param[2] = 0x00000000;
    SicdIoData.dwDataBufLen = dwSicdBufSize;
    SicdIoData.pbDataBuf = pvSicdPayLoadBuf;
    SicdIoData.eDataDirection = MUSB_SICD_DATA_IN;
    
    bGetObjHandles = TRUE;
    
    iResult = SicdDriver->pfRWBurst(0, MUSB_SICD_RW_SEND_OPERATION, (MUSB_IO_DATA_T *)&SicdIoData);
    if ( iResult )
        Printf("Result : %d\n", iResult);
    return iResult;
}


static INT32 _MUSBMtpGetExtEventData(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 iResult;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;

    ASSERT(szArgv);

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;

    iResult = SicdDriver->pfRWBurst(0, MUSB_SICD_RW_GET_EXTENDED_EVENT_DATA, (MUSB_IO_DATA_T *)&SicdIoData);
    if ( iResult )
        Printf("Result : %d\n", iResult);
    return iResult;
}

static INT32 _MUSBSicdDpcbm(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 iResult;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;

    ASSERT(szArgv);

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;

    iResult = SicdDriver->pfIoCtl(0, MUSB_SICD_IOCTL_DP_CTRL_BY_MW, 0, (MUSB_IO_DATA_T *)&SicdIoData);
    if ( iResult )
        Printf("Result : %d\n", iResult);
    return iResult;
}


static INT32 _MUSBSicdDpcbs(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 iResult;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;

    ASSERT(szArgv);

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;

    iResult = SicdDriver->pfIoCtl(0, MUSB_SICD_IOCTL_DP_CTRL_BY_SICD, 0, (MUSB_IO_DATA_T *)&SicdIoData);
    if ( iResult )
        Printf("Result : %d\n", iResult);
    return iResult;    
}

static INT32 _MUSBSicdGetNextData(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 iResult;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;
    uint32_t ObjectHandle;

    ASSERT(szArgv);

    if ( i4Argc == 2 )
    {
        ObjectHandle = StrToInt(szArgv[1]);
    }
    else
    {
        ObjectHandle = 0x00010003;
    }
    

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;

    SicdIoData.u2OperationCode = 0x1009;
    SicdIoData.u2ParamNum = 1;
    SicdIoData.u4TransactionID = 0x00000001;
    SicdIoData.u4Param[0] = ObjectHandle;
    SicdIoData.dwDataBufLen = dwSicdBufSize;
    SicdIoData.pbDataBuf = pvSicdPayLoadBuf;   
    
    iResult = SicdDriver->pfRWBurst(0, MUSB_SICD_RW_GET_NEXT_DATA, (MUSB_IO_DATA_T *)&SicdIoData);
    if ( iResult )
        Printf("Result : %d\n", iResult);
    return iResult;
}

static INT32 _MUSBSicdSilentBusReset(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 iResult;
    MUSB_SICD_IO_DATA_T SicdIoData;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;

    ASSERT(szArgv);

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;

    iResult = SicdDriver->pfRWBurst(0, MUSB_SICD_RW_SET_SILENT_BUS_RESET, (MUSB_IO_DATA_T *)&SicdIoData);
    if ( iResult )
        Printf("Result : %d\n", iResult);
    return iResult;  
}

static INT32 _MUSBMTPSendObjProp(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 iResult;
    MUSB_SICD_IO_DATA_T SicdIoData;
    UINT8* pbPayLoadBuf;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;

    ASSERT(szArgv);

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;

    pbPayLoadBuf = _MUSB_PropertyList;
    ASSERT(pbPayLoadBuf);

    SicdIoData.u2OperationCode = 0x9808;
    SicdIoData.u2ParamNum = 5;
    SicdIoData.u4TransactionID = 0x00000001;
    SicdIoData.u4Param[0] = 0x00010001;
    SicdIoData.u4Param[1] = 0xffffffff;
    SicdIoData.u4Param[2] = 0x00003801;
    SicdIoData.u4Param[3] = 0x00000000;
    SicdIoData.u4Param[4] = 0x000002CB;
    SicdIoData.dwDataBufLen = 115;
    SicdIoData.pbDataBuf = pbPayLoadBuf;
    SicdIoData.eDataDirection = MUSB_SICD_DATA_OUT;
    SicdIoData.dwTotalDataSize = 115;
    
    iResult = SicdDriver->pfRWBurst(0, MUSB_SICD_RW_SEND_OPERATION, (MUSB_IO_DATA_T *)&SicdIoData);
    Printf("Result : %d\n", iResult);
    return iResult;
}

static INT32 _MUSBMTPSendObject(INT32 i4Argc, const CHAR **szArgv)
{
    INT32 iResult;
    MUSB_SICD_IO_DATA_T SicdIoData;
    UINT8* pbPayLoadBuf;
    MU_FS_DEVICE_TYPE *SicdDriver;
    MGC_UcFsDevice *MUSB_aUcFsSicdDevice;

    ASSERT(szArgv);

    SicdDriver = MUSB_HfiSicdDriverGet();
    MUSB_aUcFsSicdDevice = MUSB_UcFsSicdDeviceGet();
    MUSB_aUcFsSicdDevice->rt_nfy_info.pfHfiNfy = _MUSB_pfNfy;

    pbPayLoadBuf = _MUSB_PicContent;
    ASSERT(pbPayLoadBuf);

    SicdIoData.u2OperationCode = 0x100D;
    SicdIoData.u2ParamNum = 0;
    SicdIoData.u4TransactionID = 0x00000002;
    SicdIoData.dwDataBufLen = 715;
    SicdIoData.pbDataBuf = pbPayLoadBuf;
    SicdIoData.eDataDirection = MUSB_SICD_DATA_OUT;
    SicdIoData.dwTotalDataSize = 715;
    
    iResult = SicdDriver->pfRWBurst(0, MUSB_SICD_RW_SEND_OPERATION, (MUSB_IO_DATA_T *)&SicdIoData);
    Printf("Result : %d\n", iResult);
    return iResult;
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

/* End of Declare */

static CLI_EXEC_T _arSICDCmdTbl[] = 
{
    { "msbf", NULL, _MUSBMtpSetBufSize, NULL, "Set Buf Size", CLI_GUEST },
    { "mos", NULL, _MUSBMtpOpenSession, NULL, "Open Session", CLI_GUEST },
    { "mcs", NULL, _MUSBMtpCloseSession, NULL, "Close Session", CLI_GUEST },
    { "gdi", NULL, _MUSBMtpGetDevInfo, NULL, "Device Info", CLI_GUEST },

    { "mgoi", NULL, _MUSBMtpGetObjectInfo, NULL, "Get Object Info", CLI_GUEST},
    { "mgoh", NULL,  _MUSBMtpGetObjectHandles, NULL, "Get Object Handles", CLI_GUEST},
    { "mgpo", NULL, _MUSBMtpGetPartialObject, NULL, "Get Partial Object ", CLI_GUEST },    
    { "mgpt", NULL, _MUSBMtpGetPartialObjectTest, NULL, "Get Partial Object Test", CLI_GUEST },
    { "mgpt2", NULL, _MUSBMtpGetPartialObjectTest2, NULL, "Get Partial Object Test2", CLI_GUEST },    
    { "mgo", NULL, _MUSBMtpGetObject, NULL, "Get Object", CLI_GUEST },    
    { "got", NULL, _MUSBMtpGetObjectTest, NULL, "Get Object TEST", CLI_GUEST },    

    { "mge", NULL, _MUSBMtpGetExtEventData, NULL, "Get Ext Event Data", CLI_GUEST},
    { "mao", NULL, _MUSBSicdAbortOperation, NULL, "SICD Abort Operation", CLI_GUEST},
    { "sdr", NULL, _MUSBSicdDeviceReset, NULL, "Sicd Device Reset", CLI_GUEST },
    { "sbl", NULL, _MUSBSicdSilentBusReset, NULL, "USB Silent Bus Reset", CLI_GUEST},

    { "dpcbm", NULL, _MUSBSicdDpcbm, NULL, "DataPhaseControlledByMw", CLI_GUEST},
    { "dpcbs", NULL, _MUSBSicdDpcbs, NULL, "DataPhaseControlledBySicd", CLI_GUEST},
    { "gnd", NULL, _MUSBSicdGetNextData, NULL, "Get Next Data", CLI_GUEST},

    { "sop", NULL, _MUSBMTPSendObjProp, NULL, "Send Object Property", CLI_GUEST},
    { "so", NULL, _MUSBMTPSendObject, NULL, "Send Object", CLI_GUEST},
    { NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR }
};

#else

static CLI_EXEC_T _arSICDCmdTbl [] =
{
    { NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR }
};

#endif

CLI_MAIN_COMMAND_ITEM(rSICD)
{
    "sicd", NULL, NULL, _arSICDCmdTbl, "MUSB SICD command", CLI_GUEST
};

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
LINT_EXT_HEADER_END

