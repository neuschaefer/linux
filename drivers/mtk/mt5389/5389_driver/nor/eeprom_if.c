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
 * $Author: muran.xiang $
 * $Date: 2012/07/18 $
 * $RCSfile: eeprom_if.c,v $
 * $Revision: #3 $
 *
 *---------------------------------------------------------------------------*/

/** @file eeprom_if.c
 *  This file is the implementation of EEPROM interface definition.
 */

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "storage_if.h"
#include "eeprom_if.h"
#include "nor_if.h"
#include "nor_debug.h"
#include "x_hal_5381.h"
#include "x_bim.h"
#include "x_pinmux.h"
#include "x_ldr_env.h"
#include "drvcust_if.h"
#ifdef CC_NAND_ENABLE
#include "nand_if.h"
#endif
#include "storage_if.h"
#if (defined(CC_MT5396)||defined(CC_MT5368)||defined(CC_MT5389))
#include "hw_hdmi.h"
#include "hw_vdoin.h"
#endif
LINT_EXT_HEADER_END
//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------
#define EEPDTV_LOG_ENABLE           (0)

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

#if defined(__KERNEL__) && defined(CONFIG_MTD_MT53XX_NOR)
extern INT32 NORPART_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
#endif

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------
#ifdef CC_USE_PAPI
static const UINT8 _au1HDCP[320] = 
{
0x00 ,0x14 ,0x7c ,0xe6 ,0x2c ,0x37 ,0xf3 ,0xe8 ,0xa8 ,0x4d ,0x27 ,0x66 ,0xa8 ,0xd0 ,0x2f ,0x13,
0x55 ,0x79 ,0x97 ,0xe7 ,0x87 ,0x37 ,0x18 ,0x04 ,0xd1 ,0x5f ,0x3b ,0x12 ,0x9f ,0x32 ,0xa4 ,0x6f,
0x58 ,0x1c ,0x8a ,0xbc ,0x8c ,0x7f ,0xe3 ,0xcb ,0x81 ,0x9e ,0xb4 ,0x45 ,0x7c ,0x66 ,0x6a ,0xcc,
0xdd ,0x5c ,0xc8 ,0x17 ,0xa3 ,0x90 ,0x43 ,0x91 ,0x8b ,0x01 ,0xf0 ,0x1d ,0xff ,0x8a ,0x1b ,0xde,
0x94 ,0xd6 ,0xb6 ,0xf6 ,0x6d ,0x15 ,0x7b ,0x42 ,0x13 ,0xa2 ,0x1b ,0x04 ,0xb5 ,0xdd ,0x11 ,0xcc,
0x42 ,0x28 ,0x66 ,0x65 ,0xf5 ,0x77 ,0xf1 ,0x65 ,0xc3 ,0x8c ,0x9b ,0x2c ,0xad ,0xb4 ,0xe9 ,0x7c,
0xd1 ,0xbc ,0xd6 ,0x4a ,0x5d ,0xf7 ,0x45 ,0x3e ,0x2a ,0x22 ,0xec ,0xa8 ,0xdf ,0x68 ,0x54 ,0x57,
0x5a ,0x10 ,0xc8 ,0x38 ,0x9f ,0x94 ,0xa0 ,0xa7 ,0xa0 ,0x71 ,0xa2 ,0x67 ,0x8e ,0x23 ,0xbd ,0x8d,
0x63 ,0x89 ,0x0d ,0x01 ,0x91 ,0x97 ,0x4c ,0xba ,0x5c ,0x4d ,0x94 ,0x73 ,0x36 ,0x68 ,0x12 ,0x6c,
0xe8 ,0xfa ,0xb1 ,0x51 ,0xc1 ,0x93 ,0xc6 ,0xce ,0x72 ,0x90 ,0xc1 ,0x6b ,0x4d ,0xf6 ,0x63 ,0x02,
0xd3 ,0xa6 ,0x9b ,0x80 ,0x35 ,0xb6 ,0xa9 ,0xff ,0x8e ,0xfd ,0xd9 ,0x6f ,0x24 ,0xa6 ,0xdb ,0x4c,
0xd2 ,0x0c ,0x0f ,0xcf ,0xcd ,0x1a ,0x19 ,0xe4 ,0x62 ,0x9c ,0x6d ,0x17 ,0x6b ,0x57 ,0x39 ,0xcb,
0x6a ,0x0d ,0x80 ,0x75 ,0xfa ,0xf3 ,0x69 ,0x7d ,0x9f ,0x79 ,0xe3 ,0xc0 ,0x8b ,0x5a ,0xd2 ,0xa8,
0xc9 ,0xd9 ,0x90 ,0x93 ,0xaa ,0xe6 ,0x1a ,0x1e ,0x17 ,0x93 ,0x03 ,0x2d ,0x43 ,0xc0 ,0xaf ,0x33,
0x94 ,0x66 ,0xa9 ,0x18 ,0x55 ,0xcc ,0x22 ,0xf5 ,0x23 ,0xc8 ,0xc5 ,0x37 ,0xf1 ,0x81 ,0xd2 ,0x96,
0xaf ,0x0a ,0x5a ,0xe5 ,0x8a ,0x13 ,0xef ,0x63 ,0x19 ,0x4c ,0xc6 ,0x3f ,0x6c ,0x9a ,0x7a ,0xb2,
0xa6 ,0xd4 ,0x31 ,0x5a ,0x30 ,0x01 ,0x21 ,0xcd ,0xa2 ,0x86 ,0x74 ,0x04 ,0x46 ,0x3e ,0x38 ,0x08,
0xe7 ,0x57 ,0xa7 ,0xb4 ,0x41 ,0x73 ,0x02 ,0x78 ,0x93 ,0x34 ,0x93 ,0xce ,0x93 ,0x5a ,0x46 ,0xd3,
0x0a ,0x14 ,0xf7 ,0x61 ,0x03 ,0xb7 ,0x8f ,0x22 ,0xd2 ,0xf1 ,0x3a ,0x98 ,0xba ,0x28 ,0x62 ,0x53,
0xca ,0x4c ,0x1d ,0x19 ,0xce ,0x49 ,0x87 ,0xf7 ,0xb5 ,0x26 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00
};
#else
#if  !defined(CC_MTK_LOADER)
#ifndef HDMI_NO_INITHDCP
static UINT8 *_pu1HDCP;
#endif /* HDMI_NO_INITHDCP */
#endif /* !defined(CC_MTK_LOADER) */
#endif /* CC_USE_PAPI */

#ifdef CC_DISABLE_DTVCFG_FROM_EEP
static UINT32 _u4DTVCFGInit = 0;
DTVCFG_T rVirtualDtvCfg;
#endif

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------
static UINT8 _EepDtvCalChecksum(const DTVCFG_T *prDtvCfg);

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

#if  !defined(CC_MTK_LOADER)

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------
//-------------------------------------------------------------------------
/** EEPNPTV_GetSize
 *  NPTV interface: get EEPROM device size.
 *  @param  VOID
 *  @return  device size in bytes.
 */
//-------------------------------------------------------------------------
INT32 EEPNPTV_GetSize(VOID)
{
    return (INT32)EEPROM_NPTV_DRIVER_SIZE;
}

//-------------------------------------------------------------------------
/** EEPNPTV_Read
 *  NPTV interface: EEPROM read function.
 *  @param  u8Offset 	begin address to read.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	read data length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPNPTV_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    INT32 i4Ret1;
    STORG_DEV_T *prDev;
    UINT32 u4XferCnt = 0;

    // check boundary.
    if ((u8Offset + u4MemLen) > EEPROM_NPTV_DRIVER_SIZE)
    {
        return 1;
    }

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_NPTV_DRIVER_OFFSET;

    prDev = STORGOpenDev(STORG_FLAG_EEPROM);

    if (prDev == NULL)
    {
        return 1;
    }

    // send to storage task and do sync IO.
    i4Ret = STORGSendReq(prDev, STORG_OP_READ,
    NULL, NULL,
                         NULL,
    u8Offset, u4MemLen,
                         (void *)u4MemPtr, &u4XferCnt);

    if ((i4Ret) || (u4XferCnt != u4MemLen))
    {
        i4Ret = 1;
    }

    i4Ret1 = STORGCloseDev(prDev);
    UNUSED(i4Ret1);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** EEPNPTV_Write
 *  NPTV interface: EEPROM write function.
 *  @param  u8Offset 	begin address to write.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	writedata length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPNPTV_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    INT32 i4Ret1;
    STORG_DEV_T *prDev;
    UINT32 u4XferCnt = 0;

    // check boundary.
    if ((u8Offset + u4MemLen) > EEPROM_NPTV_DRIVER_SIZE)
    {
        return 1;
    }

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_NPTV_DRIVER_OFFSET;

    prDev = STORGOpenDev(STORG_FLAG_EEPROM);

    if (prDev == NULL)
    {
        return 1;
    }

    // send to storage task and do sync IO.
    i4Ret = STORGSendReq(prDev, STORG_OP_WRITE,
    NULL, NULL,
                         NULL,
    u8Offset, u4MemLen,
                         (void *)u4MemPtr, &u4XferCnt);

    if ((i4Ret) || (u4XferCnt != u4MemLen))
    {
        i4Ret = 1;
    }

    i4Ret1 = STORGCloseDev(prDev);
    UNUSED(i4Ret1);

    return i4Ret;
}

//-------------------------------------------------------------------------
/** EEPDTV_GetSize
 *  DTV interface: get EEPROM device size.
 *  @param  VOID
 *  @return  device size in bytes.
 */
//-------------------------------------------------------------------------
INT32 EEPDTV_GetSize(VOID)
{
    return (INT32)EEPROM_DTV_DRIVER_SIZE;
}

//-------------------------------------------------------------------------
/** EEPDTV_Read
 *  DTV interface: EEPROM read function.
 *  @param  u8Offset 	begin address to read.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	read data length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPDTV_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;
    #ifdef CC_DISABLE_DTVCFG_FROM_EEP
    //Printf("CC_DISABLE_DTVCFG_FROM_EEP: EEPDTV_Read\n");
    i4Ret = 0;
    UNUSED(u8Offset);
    UNUSED(u4MemPtr);
    UNUSED(u4MemLen);
    if (u4MemLen != sizeof(DTVCFG_T))
    {
        Printf("Unknow EEPDTV_Read(%d)\n", u4MemLen);
        return 1;
    }
    if (_u4DTVCFGInit == 0)
    {   // load default cfg
        //DTVCFG_T *prDtvCfg = (DTVCFG_T *)u4MemPtr;
        x_memset(&rVirtualDtvCfg, 0, sizeof(DTVCFG_T));
        rVirtualDtvCfg.u1Magic1 = DTVCFG_MAGIC_CHAR1;
        rVirtualDtvCfg.u1Magic2 = DTVCFG_MAGIC_CHAR2;
        rVirtualDtvCfg.u1Checksum = _EepDtvCalChecksum(&rVirtualDtvCfg);
        _u4DTVCFGInit = 1;
    }
    x_memcpy((void *)u4MemPtr, (void *)&rVirtualDtvCfg, u4MemLen);

    #else // CC_DISABLE_DTVCFG_FROM_EEP

    INT32 i4Ret1;
    STORG_DEV_T *prDev;
    UINT32 u4XferCnt = 0;

    // check boundary.
    if ((u8Offset + u4MemLen) > EEPROM_DTV_DRIVER_SIZE)
    {
        return 1;
    }

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_DTV_DRIVER_OFFSET;

    prDev = STORGOpenDev(STORG_FLAG_EEPROM);

    if (prDev == NULL)
    {
        return 1;
    }

    // send to storage task and do sync IO.
    i4Ret = STORGSendReq(prDev, STORG_OP_READ,
    NULL, NULL,
                         NULL,
    u8Offset, u4MemLen,
                         (void *)u4MemPtr, &u4XferCnt);

    if ((i4Ret) || (u4XferCnt != u4MemLen))
    {
        i4Ret = 1;
    }

    i4Ret1 = STORGCloseDev(prDev);
    UNUSED(i4Ret1);

    #if EEPDTV_LOG_ENABLE
    if (i4Ret == 0)
    {
        INT32 i;
        UINT8 *au1Data;

        au1Data = (UINT8 *)u4MemPtr;
        Printf("EEPDTV_Read(Off:0x%x, Cnt:%d) OK", (UINT32)((u8Offset-EEPROM_DTV_DRIVER_OFFSET) & 0xff), u4MemLen);
        for (i=0; i<u4MemLen; i++)
        {
            if (i%8==0)
            {
                Printf("\n0x%08x | 0x%02x", i+(UINT32)((u8Offset-EEPROM_DTV_DRIVER_OFFSET) & 0xff), au1Data[i]);
            }
            else
            {
                Printf(" 0x%02x", au1Data[i]);
            }
        }
        Printf("\n");
    }
    else
    {
        Printf("EEPDTV_Read(Off:0x%x, Cnt:%d) Fail\n", (UINT32)((u8Offset-EEPROM_DTV_DRIVER_OFFSET) & 0xff), u4MemLen);
    }
    #endif /* EEPDTV_LOG_ENABLE */
    #endif
    return i4Ret;
}

//-------------------------------------------------------------------------
/** EEPDTV_Write
 *  DTV interface: EEPROM write function.
 *  @param  u8Offset 	begin address to write.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	writedata length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPDTV_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;
    #ifdef CC_DISABLE_DTVCFG_FROM_EEP
    //Printf("CC_DISABLE_DTVCFG_FROM_EEP: EEPDTV_Write\n");
    i4Ret = 0;
    UNUSED(u8Offset);
    UNUSED(u4MemPtr);
    UNUSED(u4MemLen);    

    x_memcpy((void *)&rVirtualDtvCfg, (void *)u4MemPtr, u4MemLen);
    _u4DTVCFGInit = 1;
    #else   

    INT32 i4Ret1;
    STORG_DEV_T *prDev;
    UINT32 u4XferCnt = 0;

    // check boundary.
    if ((u8Offset + u4MemLen) > EEPROM_DTV_DRIVER_SIZE)
    {
        return 1;
    }

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_DTV_DRIVER_OFFSET;

    prDev = STORGOpenDev(STORG_FLAG_EEPROM);

    if (prDev == NULL)
    {
        return 1;
    }

    // send to storage task and do sync IO.
    i4Ret = STORGSendReq(prDev, STORG_OP_WRITE,
    NULL, NULL,
                         NULL,
    u8Offset, u4MemLen,
                         (void *)u4MemPtr, &u4XferCnt);

    if ((i4Ret) || (u4XferCnt != u4MemLen))
    {
        i4Ret = 1;
    }

    i4Ret1 = STORGCloseDev(prDev);
    UNUSED(i4Ret1);

    #if EEPDTV_LOG_ENABLE
    if (i4Ret == 0)
    {
        INT32 i;
        UINT8 *au1Data;

        au1Data = (UINT8 *)u4MemPtr;
        Printf("EEPDTV_Write(Off:0x%x, Cnt:%d) OK", (UINT32)((u8Offset-EEPROM_DTV_DRIVER_OFFSET) & 0xff), u4MemLen);
        for (i=0; i<u4MemLen; i++)
        {
            if (i%8==0)
            {
                Printf("\n0x%08x | 0x%02x", i+(UINT32)((u8Offset-EEPROM_DTV_DRIVER_OFFSET) & 0xff), au1Data[i]);
            }
            else
            {
                Printf(" 0x%02x", au1Data[i]);
            }
        }
        Printf("\n");
    }
    else
    {
        Printf("EEPDTV_Write(Off:0x%x, Cnt:%d) Fail\n", (UINT32)((u8Offset-EEPROM_DTV_DRIVER_OFFSET) & 0xff), u4MemLen);
    }
    #endif /* EEPDTV_LOG_ENABLE */
    #endif
    return i4Ret;
}

//-------------------------------------------------------------------------
/** EEPHDCP_Read
 *  HDCP interface: EEPROM read function.
 *  @param  u8Offset 	begin address to read.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	read data length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPHDCP_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
#ifndef HDMI_NO_INITHDCP
    INT32 i4Ret;
    UINT8 u1SystemEEPROMDevBus, u1SystemEEPROMDevAddr, u1HdcpEEPROMDevBus, u1HdcpEEPROMDevAddr;

    // check boundary.
    if ((u8Offset + u4MemLen) > EEPROM_HDMI_HDCP_SIZE)
    {
        return 1;
    }

#if defined(CC_NAND_ENABLE)
    // check if HDCP is stored in NAND
    i4Ret = (INT32)DRVCUST_OptGet(eHDCPNANDPartId);
    if (i4Ret > 1)
    {
        u8Offset |= (((UINT64)(UINT32)i4Ret) << 32);
        i4Ret = NANDPART_Read(u8Offset, u4MemPtr, u4MemLen);
        return i4Ret;
    }
#else
    // check if HDCP is stored in NOR
	#if !defined(CC_NOR_DISABLE) || defined(EDID_KEY_READ_FROM_NOR)
		i4Ret = (INT32)DRVCUST_OptGet(eHDCPNorPartId);
	    if (i4Ret > 1)
	    {
	        u8Offset |= (((UINT64)(UINT32)i4Ret) << 32);	
	        i4Ret = NORPART_Read(u8Offset, u4MemPtr, u4MemLen);
	        return i4Ret;
	    }
	#endif	
#endif
    u1SystemEEPROMDevBus = (UINT8) DRVCUST_InitGet(eSystemEepromBus);
    u1SystemEEPROMDevAddr = (UINT8) DRVCUST_InitGet(eSystemEepromAddress);
    u1HdcpEEPROMDevBus = (UINT8) DRVCUST_InitGet(eHdcpEepromBus);
    u1HdcpEEPROMDevAddr = (UINT8) DRVCUST_InitGet(eHdcpEepromAddress);

    u8Offset += (UINT64) DRVCUST_InitGet(eHdcpEepromOffset);

    // check if HDCP eeprom is the same as system eeprom
    if ((u1SystemEEPROMDevBus != u1HdcpEEPROMDevBus) ||
        (u1SystemEEPROMDevAddr != u1HdcpEEPROMDevAddr))
    {
        UINT16 u2HdcpEEPROMClkDiv;
        UINT32 u4HdcpEEPROMSize;

        u2HdcpEEPROMClkDiv = (UINT16) DRVCUST_InitGet(eHdcpEepromClkDiv);
        u4HdcpEEPROMSize = DRVCUST_InitGet(eHdcpEepromSize);

        return EEPROM_X_Read(u8Offset, u4MemPtr, u4MemLen,
                             u1HdcpEEPROMDevBus, u2HdcpEEPROMClkDiv, u1HdcpEEPROMDevAddr, u4HdcpEEPROMSize);
    }

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_HDMI_HDCP_OFFSET;

    i4Ret = EEPROM_Read(u8Offset, u4MemPtr, u4MemLen);

    return i4Ret;
#else // HDMI_NO_INITHDCP
    return 0;    
#endif // HDMI_NO_INITHDCP
}

//-------------------------------------------------------------------------
/** EEPHDCP_Write
 *  HDCP interface: EEPROM write function.
 *  @param  u8Offset 	begin address to write.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	writedata length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPHDCP_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
#ifndef HDMI_NO_INITHDCP
    INT32 i4Ret;
    UINT8 u1SystemEEPROMDevBus, u1SystemEEPROMDevAddr, u1HdcpEEPROMDevBus, u1HdcpEEPROMDevAddr;

    // check boundary.
    if ((u8Offset + u4MemLen) > EEPROM_HDMI_HDCP_SIZE)
    {
        return 1;
    }

#if defined(CC_NAND_ENABLE)
    // check if HDCP is stored in NOR
    i4Ret = (INT32)DRVCUST_OptGet(eHDCPNANDPartId);
    if (i4Ret > 1)
    {
        u8Offset |= (((UINT64)(UINT32)i4Ret) << 32);
        i4Ret = NANDPART_Write(u8Offset, u4MemPtr, u4MemLen);
        return i4Ret;
    }
#else 
    // check if HDCP is stored in NOR
    #ifndef CC_NOR_DISABLE    
	    i4Ret = (INT32)DRVCUST_OptGet(eHDCPNorPartId);
	    if (i4Ret > 1)
	    {
	        u8Offset |= (((UINT64)(UINT32)i4Ret) << 32);
			
			i4Ret = NORPART_Erase(u8Offset, 1);
	        i4Ret |= NORPART_Write(u8Offset, u4MemPtr, u4MemLen);
			return i4Ret;
	    }
	#endif
#endif
    u1SystemEEPROMDevBus = (UINT8) DRVCUST_InitGet(eSystemEepromBus);
    u1SystemEEPROMDevAddr = (UINT8) DRVCUST_InitGet(eSystemEepromAddress);
    u1HdcpEEPROMDevBus = (UINT8) DRVCUST_InitGet(eHdcpEepromBus);
    u1HdcpEEPROMDevAddr = (UINT8) DRVCUST_InitGet(eHdcpEepromAddress);

    u8Offset += (UINT64) DRVCUST_InitGet(eHdcpEepromOffset);

    // check if HDCP eeprom is the same as system eeprom
    if ((u1SystemEEPROMDevBus != u1HdcpEEPROMDevBus) ||
        (u1SystemEEPROMDevAddr != u1HdcpEEPROMDevAddr))
    {
        UINT32 u4HdcpEEPROMWPGpio, u4HdcpEEPROMWPEnablePolarity;
        UINT32 u4HdcpEEPROMSize;
        UINT16 u2HdcpEEPROMClkDiv;

        u2HdcpEEPROMClkDiv = (UINT16) DRVCUST_InitGet(eHdcpEepromClkDiv);
        u4HdcpEEPROMSize = DRVCUST_InitGet(eHdcpEepromSize);

        if (0 == DRVCUST_InitQuery(eHdcpEepromWPGpio, &u4HdcpEEPROMWPGpio) && 
           (0 == DRVCUST_InitQuery(eHdcpEepromWPEnablePolarity, &u4HdcpEEPROMWPEnablePolarity)))
        {
            return EEPROM_X_Write(u8Offset, u4MemPtr, u4MemLen,
                                  u1HdcpEEPROMDevBus, u2HdcpEEPROMClkDiv, u1HdcpEEPROMDevAddr, u4HdcpEEPROMSize,
                                  (INT32)u4HdcpEEPROMWPGpio, (INT32)u4HdcpEEPROMWPEnablePolarity);
        }
        else
        {
            return EEPROM_X_Write(u8Offset, u4MemPtr, u4MemLen,
                                  u1HdcpEEPROMDevBus, u2HdcpEEPROMClkDiv, u1HdcpEEPROMDevAddr, u4HdcpEEPROMSize,
                                  -1, -1);
        }
    }

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_HDMI_HDCP_OFFSET;

    i4Ret = EEPROM_Write(u8Offset, u4MemPtr, u4MemLen);

    return i4Ret;
#else // HDMI_NO_INITHDCP
    return 0;    
#endif // HDMI_NO_INITHDCP
}

//-------------------------------------------------------------------------
/** EEPHDCP_SetSram
 *  HDCP interface: Turn on/off interanl HDCP slave enable bit
 *  @param  u4On 	1: HDMI will access internal SRAM.
 *                             0: HDMI will access external EEPROM.
 *  @param  u4DevAddr  internal HDCP Slave device address.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPHDCP_SetSramDevice(UINT32 u4On, UINT32 u4DevAddr)
{
#ifndef HDMI_NO_INITHDCP
    UINT32 u4Data;

    if (u4On)
    {
        // device address only 8 bits.
        if ((u4DevAddr > 0x100) || (u4DevAddr < 2))
        {
            return 1;
        }

        u4Data = 0x100 | u4DevAddr;

	#if (defined(CC_MT5396)||defined(CC_MT5368)||defined(CC_MT5389))
		// en/adr
		vIO32Write4B(HDCP_DEV0, u4Data);
		vIO32Write4B(HDCP_DEV1, u4Data);
		vIO32Write4B(HDCP_DEV2, u4Data);
		vIO32Write4B(HDCP_DEV3, u4Data);
		// kskm
		vRegWrite4B((EPST+0xc00), 0xff001800);
    	vRegWrite4B((KS_MASK+0xc00), 0x000000C3);
		vRegWrite4B((EPST+0x800), 0xff001800);
    	vRegWrite4B((KS_MASK+0x800), 0x000000C3);
		vRegWrite4B((EPST+0x400), 0xff001800);
    	vRegWrite4B((KS_MASK+0x400), 0x000000C3);
    	vRegWrite4B(EPST, 0xff001800);
    	vRegWrite4B(KS_MASK, 0x000000C3);
    	// reload KSV
    	vRegWrite4BMsk((EPST+0xc00), Fld2Msk32(LD_KSV), Fld2Msk32(LD_KSV));
		vRegWrite4BMsk((EPST+0x800), Fld2Msk32(LD_KSV), Fld2Msk32(LD_KSV));
		vRegWrite4BMsk((EPST+0x400), Fld2Msk32(LD_KSV), Fld2Msk32(LD_KSV));
		vRegWrite4BMsk(EPST, Fld2Msk32(LD_KSV), Fld2Msk32(LD_KSV));
		vRegWrite4BMsk((EPST+0xc00), 0, Fld2Msk32(LD_KSV));
		vRegWrite4BMsk((EPST+0x800), 0, Fld2Msk32(LD_KSV));
		vRegWrite4BMsk((EPST+0x400), 0, Fld2Msk32(LD_KSV));
		vRegWrite4BMsk(EPST, 0, Fld2Msk32(LD_KSV));
		// HDCP rst
		vRegWrite4BMsk((SRST+0xc00), Fld2Msk32(HDCP_RST), Fld2Msk32(HDCP_RST));
		vRegWrite4BMsk((SRST+0xc00), 0, Fld2Msk32(HDCP_RST));
    	vRegWrite4BMsk((SRST+0x800), Fld2Msk32(HDCP_RST), Fld2Msk32(HDCP_RST));
		vRegWrite4BMsk((SRST+0x800), 0, Fld2Msk32(HDCP_RST));
    	vRegWrite4BMsk((SRST+0x400), Fld2Msk32(HDCP_RST), Fld2Msk32(HDCP_RST));
		vRegWrite4BMsk((SRST+0x400), 0, Fld2Msk32(HDCP_RST));
    	vRegWrite4BMsk(SRST, Fld2Msk32(HDCP_RST), Fld2Msk32(HDCP_RST));
		vRegWrite4BMsk(SRST, 0, Fld2Msk32(HDCP_RST));
		// ddc en
		vRegWrite1B((SYS_CTRL_1+0xc00), 0x95);
		vRegWrite1B((SYS_CTRL_1+0x800), 0x95);
		vRegWrite1B((SYS_CTRL_1+0x400), 0x95);
		vRegWrite1B(SYS_CTRL_1, 0x95);
	#else
        IO_WRITE32(BIM_BASE, REG_RW_HDCP_DEV, u4Data);
	#endif
    }
    else
    {
	#if (defined(CC_MT5396)||defined(CC_MT5368)||defined(CC_MT5389))
		u4Data = u4IO32Read4B(HDCP_DEV0);
	#else
        u4Data = IO_READ32(BIM_BASE, REG_RW_HDCP_DEV);
	#endif

        u4Data &= ~0x100;

	#if (defined(CC_MT5396)||defined(CC_MT5368)||defined(CC_MT5389))
		vIO32Write4B(HDCP_DEV0, u4Data);
		vIO32Write4B(HDCP_DEV1, u4Data);
		vIO32Write4B(HDCP_DEV2, u4Data);
		vIO32Write4B(HDCP_DEV3, u4Data);
	#else
        IO_WRITE32(BIM_BASE, REG_RW_HDCP_DEV, u4Data);
	#endif
    }
#endif // HDMI_NO_INITHDCP
    return 0;
}

//-------------------------------------------------------------------------
/** EEPHDCP_Init
 *  HDCP interface: load EEPROM HDCP content to internal HDCP SRAM.
 *  @param  void.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPHDCP_Init(void)
{
#if 0
#ifndef HDMI_NO_INITHDCP
    INT32 i4Ret;

    UINT32 i;
    UINT32 u4Data;
#ifdef CC_USE_PAPI
    // write to hdcp sram pointer.
    IO_WRITE32(BIM_BASE, REG_RW_HDCP_ADDR, 0);

    printk("EEPHDCP_Init using pre-defined array!\n");
    for (i = 0; i < EEPROM_HDMI_HDCP_SIZE; i = i + 4)
    {
        u4Data = *(UINT32*)(&_au1HDCP[i]);

        // write to hdcp sram pointer.
        IO_WRITE32(BIM_BASE, REG_RW_HDCP_DATA, u4Data);
    }
#else
    UINT8* pu1Ptr;

    // write to hdcp sram pointer.
#if (defined(CC_MT5396)||defined(CC_MT5368)||defined(CC_MT5389))
	vIO32Write4B(HDCP_ADDR,0);
#else
    IO_WRITE32(BIM_BASE, REG_RW_HDCP_ADDR, 0);
#endif

    _pu1HDCP = x_mem_alloc(EEPROM_HDMI_HDCP_SIZE + 4);
    if(!_pu1HDCP)
    {
        return 1;
    }
    pu1Ptr = (UINT8*)(((((UINT32)_pu1HDCP - 1) >> 2) << 2) + 0x4);
    // read hdcp from eeprom.
    i4Ret = EEPHDCP_Read((UINT64)0, (UINT32)pu1Ptr, EEPROM_HDMI_HDCP_SIZE);
    if (i4Ret)
    {
        x_mem_free(_pu1HDCP);    
        return 1;
    }
    for (i = 0; i < EEPROM_HDMI_HDCP_SIZE; i = i + 4)
    {
        u4Data = *(UINT32*)(&pu1Ptr[i]);

        // write to hdcp sram pointer.
    #if (defined(CC_MT5396)||defined(CC_MT5368)||defined(CC_MT5389))
		vIO32Write4B(HDCP_DATA,u4Data);
	#else
        IO_WRITE32(BIM_BASE, REG_RW_HDCP_DATA, u4Data);
	#endif
    }
    x_mem_free(_pu1HDCP);   
#endif /* CC_USE_PAPI */
#endif // HDMI_NO_INITHDCP
#endif
    return 0;
}

#else /* CC_MTK_LOADER */
// Here is loader's eeprom dtvcfg access function.
INT32 EEPDTV_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    #ifdef CC_DISABLE_DTVCFG_FROM_EEP
    //Printf("CC_DISABLE_DTVCFG_FROM_EEP: EEPDTV_Read\n");
    i4Ret = 0;
    UNUSED(u8Offset);
    UNUSED(u4MemPtr);
    UNUSED(u4MemLen);
    if (u4MemLen != sizeof(DTVCFG_T))
    {
        Printf("Unknow EEPDTV_Read(%d)\n", u4MemLen);
        return 1;
    }
    if (_u4DTVCFGInit == 0)
    {   // load default cfg
        //DTVCFG_T *prDtvCfg = (DTVCFG_T *)u4MemPtr;
        x_memset(&rVirtualDtvCfg, 0, sizeof(DTVCFG_T));
        rVirtualDtvCfg.u1Magic1 = DTVCFG_MAGIC_CHAR1;
        rVirtualDtvCfg.u1Magic2 = DTVCFG_MAGIC_CHAR2;
        rVirtualDtvCfg.u1Checksum = _EepDtvCalChecksum(&rVirtualDtvCfg);
        _u4DTVCFGInit = 1;
    }
    x_memcpy((void *)u4MemPtr, (void *)&rVirtualDtvCfg, u4MemLen);

    #else // CC_DISABLE_DTVCFG_FROM_EEP
    // check boundary.
    if ((u8Offset + u4MemLen) > EEPROM_DTV_DRIVER_SIZE)
    {
        return 1;
    }

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_DTV_DRIVER_OFFSET;

    i4Ret = EEPROM_Read(u8Offset, u4MemPtr, u4MemLen);

    #if EEPDTV_LOG_ENABLE
    if (i4Ret == 0)
    {
        INT32 i;
        UINT8 *au1Data;

        au1Data = (UINT8 *)u4MemPtr;
        Printf("EEPDTV_Read(Off:0x%x, Cnt:%d) OK", (UINT32)((u8Offset-EEPROM_DTV_DRIVER_OFFSET) & 0xff), u4MemLen);
        for (i=0; i<u4MemLen; i++)
        {
            if (i%8==0)
            {
                Printf("\n0x%08x | 0x%02x", i+(UINT32)((u8Offset-EEPROM_DTV_DRIVER_OFFSET) & 0xff), au1Data[i]);
            }
            else
            {
                Printf(" 0x%02x", au1Data[i]);
            }
        }
        Printf("\n");
    }
    else
    {
        Printf("EEPDTV_Read(Off:0x%x, Cnt:%d) Fail\n", (UINT32)((u8Offset-EEPROM_DTV_DRIVER_OFFSET) & 0xff), u4MemLen);
    }
    #endif /* EEPDTV_LOG_ENABLE */
    #endif  // CC_DISABLE_DTVCFG_FROM_EEP
    
    return i4Ret;
}

INT32 EEPDTV_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;
    #ifdef CC_DISABLE_DTVCFG_FROM_EEP
    //Printf("CC_DISABLE_DTVCFG_FROM_EEP: EEPDTV_Write\n");
    i4Ret = 0;
    UNUSED(u8Offset);
    UNUSED(u4MemPtr);
    UNUSED(u4MemLen);    

    x_memcpy((void *)&rVirtualDtvCfg, (void *)u4MemPtr, u4MemLen);
    _u4DTVCFGInit = 1;
    #else   
    // check boundary.
    if ((u8Offset + u4MemLen) > EEPROM_DTV_DRIVER_SIZE)
    {
        return 1;
    }

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_DTV_DRIVER_OFFSET;

    i4Ret = EEPROM_Write(u8Offset, u4MemPtr, u4MemLen);

    #if EEPDTV_LOG_ENABLE
    if (i4Ret == 0)
    {
        INT32 i;
        UINT8 *au1Data;

        au1Data = (UINT8 *)u4MemPtr;
        Printf("EEPDTV_Write(Off:0x%x, Cnt:%d) OK", (UINT32)((u8Offset-EEPROM_DTV_DRIVER_OFFSET) & 0xff), u4MemLen);
        for (i=0; i<u4MemLen; i++)
        {
            if (i%8==0)
            {
                Printf("\n0x%08x | 0x%02x", i+(UINT32)((u8Offset-EEPROM_DTV_DRIVER_OFFSET) & 0xff), au1Data[i]);
            }
            else
            {
                Printf(" 0x%02x", au1Data[i]);
            }
        }
        Printf("\n");
    }
    else
    {
        Printf("EEPDTV_Write(Off:0x%x, Cnt:%d) Fail\n", (UINT32)((u8Offset-EEPROM_DTV_DRIVER_OFFSET) & 0xff), u4MemLen);
    }
    #endif /* EEPDTV_LOG_ENABLE */
    #endif
    
    return i4Ret;
}
#endif /* CC_MTK_LOADER */

#if defined(CC_MT5395) || defined(CC_MT5365) || defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
#ifndef CC_VGA_EDID_DISABLE
//-------------------------------------------------------------------------
/** EEPVGAEDID_Read
 *  VGAEDID interface: EEPROM read function.
 *  @param  u8Offset 	begin address to read.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	read data length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPVGAEDID_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    // check boundary.
    if ((u8Offset + u4MemLen) > VGA_EDID_STORAGE_SIZE)
    {
        return 1;
    }
#if defined(CC_NAND_ENABLE) 
    // check if VGA EDID is stored in NAND
    i4Ret = (INT32)DRVCUST_InitGet(eVGAEDIDNANDPartId);
    if (i4Ret > 1)
    {
        UINT32 u4PartOffset=0;
        u4PartOffset= (INT32) DRVCUST_InitGet(eVGAEDIDNANDOffset);
        u4PartOffset += u8Offset ;
        Printf("read from nand flash %d %d\n", i4Ret, u4PartOffset);
#ifdef CC_MTK_LOADER
        i4Ret = Loader_ReadNandFlash(i4Ret, (UINT32)u4PartOffset, (VOID*) u4MemPtr, u4MemLen);
#else
        i4Ret = STORG_SyncNandRead(i4Ret, (UINT32)u4PartOffset, (VOID*)u4MemPtr, u4MemLen);
#endif

#if 0
		if(i4Ret == u4MemLen)
		{
			return 0 ;
		}
		else
		{
			return -1 ;
		}
#endif
		return 0 ;
    }
#else
#if !defined(CC_NOR_DISABLE) || defined(EDID_KEY_READ_FROM_NOR)
#ifndef CC_UBOOT
    i4Ret = (INT32)DRVCUST_InitGet(eVGAEDIDNorPartId);
    if (i4Ret > 1)
    {
        UINT32 u4PartOffset=0;
        u4PartOffset = (INT32) DRVCUST_InitGet(eVGAEDIDNorOffset);
        u8Offset += u4PartOffset;
        Printf("read from nor %x %x %x\n", i4Ret, (UINT32)u8Offset, u4MemLen);
        u8Offset |= (((UINT64)(UINT32)i4Ret) << 32);	
        i4Ret = NORPART_Read(u8Offset, u4MemPtr, u4MemLen);
        return 0;
    }
#endif
#endif
#endif

    Printf("read from eeprom ioff %x phy_off %x\n", (UINT32)u8Offset, (UINT32)u8Offset+EEPROM_VGA_EDID_OFFSET);
    // adjust offset to map to physical offset.
    u8Offset += EEPROM_VGA_EDID_OFFSET;

    i4Ret = EEPROM_Read(u8Offset, u4MemPtr, u4MemLen);

    return i4Ret;
}

#ifndef CC_MTK_LOADER
//-------------------------------------------------------------------------
/** EEPVGAEDID_Write
 *  VGA EDID interface: EEPROM write function.
 *  @param  u8Offset 	begin address to write.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	writedata length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPVGAEDID_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    // check boundary.
    if ((u8Offset + u4MemLen) > VGA_EDID_STORAGE_SIZE)
    {
        return 1;
    }
#if defined(CC_NAND_ENABLE) 
    // check if VGAEDID is stored in NAND
    i4Ret = (INT32)DRVCUST_InitGet(eVGAEDIDNANDPartId);
    if (i4Ret > 1)
    {
        UINT32 u4BlkSize=0;
        VOID* pvTmpBuffer = NULL;
        
        Printf("write to nand flash\n");
        //write should be block align , don't add the offset here
        u4BlkSize = STORG_NandGetBlkSize();
        pvTmpBuffer = x_mem_alloc(u4BlkSize);
        x_memset((void *)pvTmpBuffer, 0xff, u4BlkSize);
        x_memcpy((void *)(pvTmpBuffer), (void *)u4MemPtr, u4MemLen);
        
        i4Ret = STORG_SyncNandWriteBlock(i4Ret, (UINT32)u8Offset, pvTmpBuffer, u4BlkSize);
        
        x_mem_free(pvTmpBuffer);
        if(i4Ret == u4BlkSize)
        {
            return 0 ;
        }
        else
        {
            return -1 ;
        }
    }
#else
#ifndef CC_NOR_DISABLE
    i4Ret = (INT32)DRVCUST_OptGet(eVGAEDIDNorPartId);
    if (i4Ret > 1)
    {
        UINT32 u4PartOffset=0;
        u4PartOffset = (INT32) DRVCUST_InitGet(eVGAEDIDNorOffset);
        u8Offset += u4PartOffset;
        Printf("write to nor %x %x\n", i4Ret, u8Offset);
        u8Offset |= (((UINT64)(UINT32)i4Ret) << 32);	
        i4Ret = NORPART_Erase(u8Offset, 1);
        i4Ret |= NORPART_Write(u8Offset, u4MemPtr, u4MemLen);
        return 0;
    }
#endif
#endif

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_VGA_EDID_OFFSET;

    i4Ret = EEPROM_Write(u8Offset, u4MemPtr, u4MemLen);

    return i4Ret;
}
#endif
#endif //CC_VGA_EDID_DISABLE
#endif // 65/95

#if defined(CC_MT5395) || defined(CC_MT5365) || defined(CC_MT5396) || defined(CC_MT5368) || defined(CC_MT5389)
#ifndef CC_HDMI_EDID_DISABLE
//-------------------------------------------------------------------------
/** EEPHDMIEDID_Read
 *  HDMIEDID interface: EEPROM read function.
 *  @param  u8Offset 	begin address to read.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	read data length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPHDMIEDID_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    // check boundary.
    if ((u8Offset + u4MemLen) > HDMI_EDID_STORAGE_SIZE)
    {
        return 1;
    }
#if defined(CC_NAND_ENABLE) 
    // check if HDMI EDID is stored in NAND
    i4Ret = (INT32)DRVCUST_InitGet(eHDMIEDIDNANDPartId);
    if (i4Ret > 1)
    {
		UINT32 u4PartOffset=0;
		u4PartOffset= (INT32) DRVCUST_InitGet(eHDMIEDIDNANDOffset);
		u4PartOffset += u8Offset ;
		Printf("read from nand flash %d %d\n", i4Ret, u4PartOffset);
#ifdef CC_MTK_LOADER
        i4Ret = Loader_ReadNandFlash(i4Ret, (UINT32)u4PartOffset, (VOID*) u4MemPtr, u4MemLen);
#else
        i4Ret = STORG_SyncNandRead(i4Ret, (UINT32)u4PartOffset, (VOID*)u4MemPtr, u4MemLen);
#endif
#if 0
		if(i4Ret == u4MemLen)
		{
			return 0 ;
		}
		else
		{
			return -1 ;
		}
#endif
		return 0 ;
    }
#else

#if !defined(CC_NOR_DISABLE) || defined(EDID_KEY_READ_FROM_NOR)
#ifndef CC_UBOOT
    i4Ret = (INT32)DRVCUST_InitGet(eHDMIEDIDNorPartId);
    if (i4Ret > 1)
    {
        UINT32 u4PartOffset=0;
        u4PartOffset = (INT32) DRVCUST_InitGet(eHDMIEDIDNorOffset);
        u8Offset += u4PartOffset;
        Printf("read from nor %x %x %x\n", i4Ret, (UINT32)u8Offset, u4MemLen);
        u8Offset |= (((UINT64)(UINT32)i4Ret) << 32);	
        i4Ret = NORPART_Read(u8Offset, u4MemPtr, u4MemLen);
        return 0;
    }
#endif
#endif
#endif

    Printf("read from eeprom ioff %x phy_off %x\n", (UINT32)u8Offset, (UINT32)u8Offset+EEPROM_HDMI_EDID_OFFSET);

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_HDMI_EDID_OFFSET;

    i4Ret = EEPROM_Read(u8Offset, u4MemPtr, u4MemLen);

    return i4Ret;
}

#ifndef CC_MTK_LOADER
//-------------------------------------------------------------------------
/** EEPHDMIEDID_Write
 *  HDMI EDID interface: EEPROM write function.
 *  @param  u8Offset 	begin address to write.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	writedata length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPHDMIEDID_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

    // check boundary.
    if ((u8Offset + u4MemLen) > HDMI_EDID_STORAGE_SIZE)
    {
        return 1;
    }
#if defined(CC_NAND_ENABLE) 
    // check if HDCP is stored in NOR
    i4Ret = (INT32)DRVCUST_InitGet(eHDMIEDIDNANDPartId);
    if (i4Ret > 1)
    {
		UINT32 u4BlkSize=0;
		VOID* pvTmpBuffer = NULL;
		
		Printf("write to nand flash\n");
		//write should be block align , don't add the offset here
		u4BlkSize = STORG_NandGetBlkSize();
		pvTmpBuffer = x_mem_alloc(u4BlkSize);
		x_memset((void *)pvTmpBuffer, 0xff, u4BlkSize);
		x_memcpy((void *)(pvTmpBuffer), (void *)u4MemPtr, u4MemLen);

        i4Ret = STORG_SyncNandWriteBlock(i4Ret, (UINT32)u8Offset, pvTmpBuffer, u4BlkSize);

		x_mem_free(pvTmpBuffer);
		if(i4Ret == u4BlkSize)
		{
			return 0 ;
		}
		else
		{
			return -1 ;
		}
    }

#else

#ifndef CC_NOR_DISABLE
    i4Ret = (INT32)DRVCUST_OptGet(eHDMIEDIDNorPartId);
    if (i4Ret > 1)
    {
        UINT32 u4PartOffset=0;
        u4PartOffset = (INT32) DRVCUST_InitGet(eHDMIEDIDNorOffset);
        u8Offset += u4PartOffset;
        Printf("write to nor %x %x\n", i4Ret, u8Offset);
        u8Offset |= (((UINT64)(UINT32)i4Ret) << 32);	
        i4Ret = NORPART_Erase(u8Offset, 1);
        i4Ret |= NORPART_Write(u8Offset, u4MemPtr, u4MemLen);
        return 0;
    }
#endif //CC_NOR_DISABLE
#endif //CC_NAND_ENABLE

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_HDMI_EDID_OFFSET;

    i4Ret = EEPROM_Write(u8Offset, u4MemPtr, u4MemLen);

    return i4Ret;
}
#endif //CC_MTK_LOADER
#endif //CC_HDMI_EDID_DISABLE
#endif // 65/95

//-------------------------------------------------------------------------
/** _EepDtvCalChecksum()
 *  Calculate DTVCFG_T checksum value.
 *  @param prDtvCfg the DtvCfg data structure pointer.
 *  @return the UINT8 checksum value.
 */
//-------------------------------------------------------------------------
static UINT8 _EepDtvCalChecksum(const DTVCFG_T *prDtvCfg)
{
    UINT8 u1Ret;

    ASSERT(prDtvCfg!=NULL);
    u1Ret = 0;
    u1Ret += (prDtvCfg->u1Magic1 + prDtvCfg->u1Magic2);
    u1Ret += (prDtvCfg->u1Flags + prDtvCfg->u1Upgrade);
    u1Ret += (prDtvCfg->u1Upgrade_Ext + prDtvCfg->u1Flags2);
    return u1Ret;
}

//-------------------------------------------------------------------------
/** _EepDtvIsValid()
 *  Check whether the DTVCFG_T is valid.
 *  @param prDtvCfg the test DtvCfg data structure.
 *  @return 0 is invalid, otherwise is valid.
 *      Valid must magic chars are correct and checksum is correct.
 */
//-------------------------------------------------------------------------
static UINT32 _EepDtvIsValid(const DTVCFG_T *prDtvCfg)
{
    ASSERT(prDtvCfg!=NULL);
    if (prDtvCfg->u1Magic1 != DTVCFG_MAGIC_CHAR1) { return 0; }
    if (prDtvCfg->u1Magic2 != DTVCFG_MAGIC_CHAR2) { return 0; }
    if (prDtvCfg->u1Checksum != _EepDtvCalChecksum(prDtvCfg)) { return 0; }
    return 1;
}

//-------------------------------------------------------------------------
/** _EepDtvDecideTriple()
 *  Calculate the u1Vals to a decision.
 *  @param u1Val1/2/3.
 *  @return the final data.
 */
//-------------------------------------------------------------------------
static UINT8 _EepDtvDecideTriple(UINT8 u1Val1, UINT8 u1Val2, UINT8 u1Val3)
{
    UINT8 u1Ret = 0;
    INT32 i, i4Count;

    for (i=0; i<8; i++)
    {
        i4Count = 0;
        if (u1Val1 & (1U << i)) { i4Count++; }
        if (u1Val2 & (1U << i)) { i4Count++; }
        if (u1Val3 & (1U << i)) { i4Count++; }
        if (i4Count > 1) { u1Ret |= (1U << i); }
    }
    return u1Ret;
}

//-------------------------------------------------------------------------
/** _EepDtvSyncData()
 *  Copy final DTVCFG_T to the location.
 *  @param prDtvCfg the final DTVCFG data.
 *  @param fgSync1 copy DTVCFG to location 1.
 *  @param fgSync2 copy DTVCFG to location 2.
 *  @param fgSync3 copy DTVCFG to location 3.
 */
//-------------------------------------------------------------------------
static INT32 _EepDtvSyncData(DTVCFG_T *prDtvCfg, UINT32 fgSync1, UINT32 fgSync2, UINT32 fgSync3)
{
    DTVCFG_T rDtvCfg;
    UINT8 au1Magic[2];
#ifdef CC_MTK_LOADER
    LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
#endif
    x_memset((void*)&rDtvCfg, 0, sizeof(DTVCFG_T));
    ASSERT(prDtvCfg!=NULL);
    prDtvCfg->u1Magic1 = DTVCFG_MAGIC_CHAR1;
    prDtvCfg->u1Magic2 = DTVCFG_MAGIC_CHAR2;
    prDtvCfg->u1Checksum = _EepDtvCalChecksum(prDtvCfg);

    if (fgSync1)
    {
        if (EEPDTV_Read((UINT64)0, (UINT32)(void *)&rDtvCfg, sizeof(DTVCFG_T))!=0) { return 1; }
        if (rDtvCfg.u1Flags != prDtvCfg->u1Flags)
        {
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_FLAGS, (UINT32)(void *)&prDtvCfg->u1Flags, 1)!=0) { return 1; }
        }
        if ((rDtvCfg.u1Checksum != prDtvCfg->u1Checksum) || (rDtvCfg.u1Upgrade != prDtvCfg->u1Upgrade) ||
            (rDtvCfg.u1Upgrade_Ext != prDtvCfg->u1Upgrade_Ext) || (rDtvCfg.u1Flags2 != prDtvCfg->u1Flags2) ||
            (rDtvCfg.u1Flags3 != prDtvCfg->u1Flags3))
        {
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_UPGRADE, (UINT32)(void *)&prDtvCfg->u1Upgrade, 6)!=0) { return 1; }
        }
        if ((rDtvCfg.u1Magic1 != DTVCFG_MAGIC_CHAR1) || (rDtvCfg.u1Magic2 != DTVCFG_MAGIC_CHAR2))
        {
            au1Magic[0] = DTVCFG_MAGIC_CHAR1;
            au1Magic[1] = DTVCFG_MAGIC_CHAR2;
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_MAGIC1, (UINT32)(void *)au1Magic, 2)!=0) { return 1; }
        }
    }

    if (fgSync2)
    {
        if (EEPDTV_Read((UINT64)9, (UINT32)(void *)&rDtvCfg, sizeof(DTVCFG_T))!=0) { return 1; }
        if (rDtvCfg.u1Flags != prDtvCfg->u1Flags)
        {
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_FLAGS+9, (UINT32)(void *)&prDtvCfg->u1Flags, 1)!=0) { return 1; }
        }
        if ((rDtvCfg.u1Checksum != prDtvCfg->u1Checksum) || (rDtvCfg.u1Upgrade != prDtvCfg->u1Upgrade) ||
            (rDtvCfg.u1Upgrade_Ext != prDtvCfg->u1Upgrade_Ext) || (rDtvCfg.u1Flags2 != prDtvCfg->u1Flags2) ||
            (rDtvCfg.u1Flags3 != prDtvCfg->u1Flags3))
        {
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_UPGRADE+9, (UINT32)(void *)&prDtvCfg->u1Upgrade, 6)!=0) { return 1; }
        }
        if ((rDtvCfg.u1Magic1 != DTVCFG_MAGIC_CHAR1) || (rDtvCfg.u1Magic2 != DTVCFG_MAGIC_CHAR2))
        {
            au1Magic[0] = DTVCFG_MAGIC_CHAR1;
            au1Magic[1] = DTVCFG_MAGIC_CHAR2;
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_MAGIC1+9, (UINT32)(void *)&prDtvCfg->u1Magic1, 2)!=0) { return 1; }
        }
    }

    if (fgSync3)
    {
        if (EEPDTV_Read((UINT64)18, (UINT32)(void *)&rDtvCfg, sizeof(DTVCFG_T))!=0) { return 1; }
        if (rDtvCfg.u1Flags != prDtvCfg->u1Flags)
        {
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_FLAGS+18, (UINT32)(void *)&prDtvCfg->u1Flags, 1)!=0) { return 1; }
        }
        if ((rDtvCfg.u1Checksum != prDtvCfg->u1Checksum) || (rDtvCfg.u1Upgrade != prDtvCfg->u1Upgrade) ||
            (rDtvCfg.u1Upgrade_Ext != prDtvCfg->u1Upgrade_Ext) || (rDtvCfg.u1Flags2 != prDtvCfg->u1Flags2) ||
            (rDtvCfg.u1Flags3 != prDtvCfg->u1Flags3))
        {
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_UPGRADE+18, (UINT32)(void *)&prDtvCfg->u1Upgrade, 6)!=0) { return 1; }
        }
        if ((rDtvCfg.u1Magic1 != DTVCFG_MAGIC_CHAR1) || (rDtvCfg.u1Magic2 != DTVCFG_MAGIC_CHAR2))
        {
            au1Magic[0] = DTVCFG_MAGIC_CHAR1;
            au1Magic[1] = DTVCFG_MAGIC_CHAR2;
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_MAGIC1+18, (UINT32)(void *)&prDtvCfg->u1Magic1, 2)!=0) { return 1; }
        }
    }
#ifdef CC_MTK_LOADER
    // save dtvcfg in dram cache
    x_memcpy((void*)prLdrEnv->au1DtvCfg, (void*)prDtvCfg, sizeof(DTVCFG_T));
#endif
    return 0;
}

//-------------------------------------------------------------------------
/** _EepDtvCorrection()
 *  Make DtvCfg triple redudancy sync.
 *  @param prDtvCfg Output - the final decision DtvCfg data structure pointer.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPDTV_Correction(DTVCFG_T *prDtvCfg)
{
    static UINT32 _fgEepDtvInit = 0;
    INT32 i4Ret;
    DTVCFG_T rDtvCfg1, rDtvCfg2, rDtvCfg3;
    x_memset((void*)&rDtvCfg1, 0, sizeof(DTVCFG_T));
    x_memset((void*)&rDtvCfg2, 0, sizeof(DTVCFG_T));
    x_memset((void*)&rDtvCfg3, 0, sizeof(DTVCFG_T));
    ASSERT(prDtvCfg!=NULL);
    if (_fgEepDtvInit)
    {
        return 0;
    }
    _fgEepDtvInit = 1;

    // read the first DtvCfg location.
    i4Ret = EEPDTV_Read((UINT64) 0, (UINT32)(void *)&rDtvCfg1, sizeof(DTVCFG_T));
    if (i4Ret) { return i4Ret; }

    // Get the redudancy information.
    i4Ret = EEPDTV_Read((UINT64)9, (UINT32)(void *)&rDtvCfg2, sizeof(DTVCFG_T));
    if (i4Ret) { return i4Ret; }
    i4Ret = EEPDTV_Read((UINT64)18, (UINT32)(void *)&rDtvCfg3, sizeof(DTVCFG_T));
    if (i4Ret) { return i4Ret; }

    // Check non-configured eeprom.
    if (((rDtvCfg1.u1Magic1 != DTVCFG_MAGIC_CHAR1) || (rDtvCfg1.u1Magic2 != DTVCFG_MAGIC_CHAR2)) &&
        (!_EepDtvIsValid(&rDtvCfg2)) && (!_EepDtvIsValid(&rDtvCfg3)))
    {
        // no initial value on 1, and 2/3 are invalid.
        prDtvCfg->u1Flags = 0;
        prDtvCfg->u1Upgrade = 0;
        prDtvCfg->u1Upgrade_Ext = 0;
        prDtvCfg->u1Flags2 = 0;
        prDtvCfg->u1Flags3 = 0;        
        prDtvCfg->u1Flags4 = 0;                
        return _EepDtvSyncData(prDtvCfg, 1, 1, 1);
    }
    // Check upgrade from old non-checksum version or only 1 is valid.
    else if ((!_EepDtvIsValid(&rDtvCfg1)) && (!_EepDtvIsValid(&rDtvCfg2)) && (!_EepDtvIsValid(&rDtvCfg3)))
    {   // 0, 0, 0
        prDtvCfg->u1Flags = rDtvCfg1.u1Flags;
        prDtvCfg->u1Upgrade = rDtvCfg1.u1Upgrade;
        prDtvCfg->u1Upgrade_Ext = rDtvCfg1.u1Upgrade_Ext;
        prDtvCfg->u1Flags2 = rDtvCfg1.u1Flags2;
        prDtvCfg->u1Flags3 = rDtvCfg1.u1Flags3;      
        prDtvCfg->u1Flags4 = rDtvCfg1.u1Flags4;  
        return _EepDtvSyncData(prDtvCfg, 1, 1, 1);        
    }
    // only 3 is valid.
    else if ((!_EepDtvIsValid(&rDtvCfg1)) && (!_EepDtvIsValid(&rDtvCfg2)))
    {   // 0, 0, 1
        prDtvCfg->u1Flags = rDtvCfg3.u1Flags;
        prDtvCfg->u1Upgrade = rDtvCfg3.u1Upgrade;
        prDtvCfg->u1Upgrade_Ext = rDtvCfg3.u1Upgrade_Ext;
        prDtvCfg->u1Flags2 = rDtvCfg3.u1Flags2;
        prDtvCfg->u1Flags3 = rDtvCfg2.u1Flags3;        
        prDtvCfg->u1Flags4 = rDtvCfg2.u1Flags4;             
        return _EepDtvSyncData(prDtvCfg, 1, 1, 0);
    }
    // only 2 is valid.
    else if ((!_EepDtvIsValid(&rDtvCfg1)) && (_EepDtvIsValid(&rDtvCfg2)))
    {   // 0, 1, 0/0, 1, 1 both use 2.
        prDtvCfg->u1Flags = rDtvCfg2.u1Flags;
        prDtvCfg->u1Upgrade = rDtvCfg2.u1Upgrade;
        prDtvCfg->u1Upgrade_Ext = rDtvCfg2.u1Upgrade_Ext;
        prDtvCfg->u1Flags2 = rDtvCfg2.u1Flags2;
        prDtvCfg->u1Flags3 = rDtvCfg2.u1Flags3;        
        prDtvCfg->u1Flags4 = rDtvCfg2.u1Flags4;                
        return _EepDtvSyncData(prDtvCfg, 1, 0, 1);
    }
    // then if each of 2/3 is invalid.
    else if ((_EepDtvIsValid(&rDtvCfg1)) && ((!_EepDtvIsValid(&rDtvCfg2)) || (!_EepDtvIsValid(&rDtvCfg3))))
    {   // 1, 0, 0/1, 1, 0/1, 0, 1, all use 1
        prDtvCfg->u1Flags = rDtvCfg1.u1Flags;
        prDtvCfg->u1Upgrade = rDtvCfg1.u1Upgrade;
        prDtvCfg->u1Upgrade_Ext = rDtvCfg1.u1Upgrade_Ext;
        prDtvCfg->u1Flags2 = rDtvCfg1.u1Flags2;
        prDtvCfg->u1Flags3 = rDtvCfg2.u1Flags3;        
        prDtvCfg->u1Flags4 = rDtvCfg2.u1Flags4;          
        return _EepDtvSyncData(prDtvCfg, 0, 1, 1);
    }

    // Here is all of them are valid.
    prDtvCfg->u1Flags = _EepDtvDecideTriple(rDtvCfg1.u1Flags, rDtvCfg2.u1Flags, rDtvCfg3.u1Flags);
    prDtvCfg->u1Upgrade = _EepDtvDecideTriple(rDtvCfg1.u1Upgrade, rDtvCfg2.u1Upgrade, rDtvCfg3.u1Upgrade);
    prDtvCfg->u1Upgrade_Ext = rDtvCfg1.u1Upgrade_Ext;
    prDtvCfg->u1Flags2 = rDtvCfg1.u1Flags2;
    prDtvCfg->u1Flags3 = rDtvCfg2.u1Flags3;        
    prDtvCfg->u1Flags4 = rDtvCfg2.u1Flags4;      
    return _EepDtvSyncData(prDtvCfg, 1, 1, 1);
}

//-------------------------------------------------------------------------
/** EEPDTV_GetCfg()
 *  @param prDtvCfg the output DtvCfg data structure pointer.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPDTV_GetCfg(DTVCFG_T *prDtvCfg)
{
    INT32 i4Ret;
#ifdef CC_MTK_LOADER
    static BOOL fgInit = FALSE;
    LDR_ENV_T* prLdrEnv = (LDR_ENV_T*)CC_LDR_ENV_OFFSET;
#endif
    ASSERT(prDtvCfg!=NULL);
    if (EEPDTV_Correction(prDtvCfg))
    {
        return 1;
    }

#ifdef CC_MTK_LOADER
    if (!fgInit)
    {
        // read the first DtvCfg location.
        i4Ret = EEPDTV_Read((UINT64) 0, (UINT32)(void *)prLdrEnv->au1DtvCfg, sizeof(DTVCFG_T));
        if (i4Ret) { return i4Ret; }
        fgInit = TRUE;
    }

    // load dtvcfg from dram cache
    x_memcpy((void*)prDtvCfg, (void*)prLdrEnv->au1DtvCfg, sizeof(DTVCFG_T));
#else
    i4Ret = EEPDTV_Read((UINT64) 0, (UINT32)(void *)prDtvCfg, sizeof(DTVCFG_T));
    if (i4Ret) { return i4Ret; }
#endif
    return 0;
}

//-------------------------------------------------------------------------
/** EEPDTV_SetCfg()
 *  @param prDtvCfg the input DtvCfg data structure pointer.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
//-------------------------------------------------------------------------
INT32 EEPDTV_SetCfg(DTVCFG_T *prDtvCfg)
{
    DTVCFG_T rDtvCfg;

    ASSERT(prDtvCfg!=NULL);
    if (EEPDTV_Correction(&rDtvCfg) != 0)
    {
        return 1;
    }

    prDtvCfg->u1Magic1 = DTVCFG_MAGIC_CHAR1;
    prDtvCfg->u1Magic2 = DTVCFG_MAGIC_CHAR2;
    prDtvCfg->u1Checksum = _EepDtvCalChecksum(prDtvCfg);
    return _EepDtvSyncData(prDtvCfg, 1, 1, 1);
}

