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
 * $RCSfile: eeprom_if.c,v $
 * $Revision: #1 $
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
#include "drvcust_if.h"

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

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

#ifndef CC_5381_LOADER

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
    UINT32 u4XferCnt;

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
    UINT32 u4XferCnt;

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

    INT32 i4Ret1;
    STORG_DEV_T *prDev;
    UINT32 u4XferCnt;

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

    INT32 i4Ret1;
    STORG_DEV_T *prDev;
    UINT32 u4XferCnt;

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
    INT32 i4Ret;

    // check boundary.
    if ((u8Offset + u4MemLen) > EEPROM_HDMI_HDCP_SIZE)
    {
        return 1;
    }

    i4Ret = (INT32)DRVCUST_OptGet(eHDCPNorPartId);
    if (i4Ret > 1)
    {
        u8Offset |= (((UINT64)(UINT32)i4Ret) << 32);
        i4Ret = NORPART_Read(u8Offset, u4MemPtr, u4MemLen);
        return i4Ret;
    }

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_HDMI_HDCP_OFFSET;

    i4Ret = EEPROM_Read(u8Offset, u4MemPtr, u4MemLen);

    return i4Ret;
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
    INT32 i4Ret;

    // check boundary.
    if ((u8Offset + u4MemLen) > EEPROM_HDMI_HDCP_SIZE)
    {
        return 1;
    }

    i4Ret = (INT32)DRVCUST_OptGet(eHDCPNorPartId);
    if (i4Ret > 1)
    {
        u8Offset |= (((UINT64)(UINT32)i4Ret) << 32);
        i4Ret = NORPART_Erase(u8Offset, 1);
        i4Ret |= NORPART_Write(u8Offset, u4MemPtr, u4MemLen);
        return i4Ret;
    }

    // adjust offset to map to physical offset.
    u8Offset += EEPROM_HDMI_HDCP_OFFSET;

    i4Ret = EEPROM_Write(u8Offset, u4MemPtr, u4MemLen);

    return i4Ret;
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
    UINT32 u4Data;

    if (u4On)
    {
        // device address only 8 bits.
        if ((u4DevAddr > 0x100) || (u4DevAddr < 2))
        {
            return 1;
        }

        u4Data = 0x100 | u4DevAddr;

        IO_WRITE32(BIM_BASE, REG_RW_HDCP_DEV, u4Data);
    }
    else
    {
        u4Data = IO_READ32(BIM_BASE, REG_RW_HDCP_DEV);

        u4Data &= ~0x100;

        IO_WRITE32(BIM_BASE, REG_RW_HDCP_DEV, u4Data);
    }

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
    INT32 i4Ret;

    UINT32 i;
    UINT32 u4Data;

    // write to hdcp sram pointer.
    IO_WRITE32(BIM_BASE, REG_RW_HDCP_ADDR, 0);

    for (i = 0; i < EEPROM_HDMI_HDCP_SIZE; i = i + 4)
    {
        // read hdcp from eeprom.
        i4Ret = EEPHDCP_Read((UINT64)i, (UINT32)&u4Data, 4);

        if (i4Ret)
        {
            return 1;
        }

        // write to hdcp sram pointer.
        IO_WRITE32(BIM_BASE, REG_RW_HDCP_DATA, u4Data);
    }

    return 0;
}

#else /* CC_5381_LOADER */

// Here is loader's eeprom dtvcfg access function.
INT32 EEPDTV_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

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

    return i4Ret;
}

INT32 EEPDTV_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen)
{
    INT32 i4Ret;

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

    return i4Ret;
}
#endif /* CC_5381_LOADER */

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
    // u1Ret += (prDtvCfg->u1Reserved1 + prDtvCfg->u1Reserved2);
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
            (rDtvCfg.u1Reserved1 != prDtvCfg->u1Reserved1) || (rDtvCfg.u1Reserved2 != prDtvCfg->u1Reserved2))
        {
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_UPGRADE, (UINT32)(void *)&prDtvCfg->u1Upgrade, 4)!=0) { return 1; }
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
        if (EEPDTV_Read((UINT64)12, (UINT32)(void *)&rDtvCfg, sizeof(DTVCFG_T))!=0) { return 1; }
        if (rDtvCfg.u1Flags != prDtvCfg->u1Flags)
        {
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_FLAGS+12, (UINT32)(void *)&prDtvCfg->u1Flags, 1)!=0) { return 1; }
        }
        if ((rDtvCfg.u1Checksum != prDtvCfg->u1Checksum) || (rDtvCfg.u1Upgrade != prDtvCfg->u1Upgrade) ||
            (rDtvCfg.u1Reserved1 != prDtvCfg->u1Reserved1) || (rDtvCfg.u1Reserved2 != prDtvCfg->u1Reserved2))
        {
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_UPGRADE+12, (UINT32)(void *)&prDtvCfg->u1Upgrade, 4)!=0) { return 1; }
        }
        if ((rDtvCfg.u1Magic1 != DTVCFG_MAGIC_CHAR1) || (rDtvCfg.u1Magic2 != DTVCFG_MAGIC_CHAR2))
        {
            au1Magic[0] = DTVCFG_MAGIC_CHAR1;
            au1Magic[1] = DTVCFG_MAGIC_CHAR2;
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_MAGIC1+12, (UINT32)(void *)&prDtvCfg->u1Magic1, 2)!=0) { return 1; }
        }
    }

    if (fgSync3)
    {
        if (EEPDTV_Read((UINT64)16, (UINT32)(void *)&rDtvCfg, sizeof(DTVCFG_T))!=0) { return 1; }
        if (rDtvCfg.u1Flags != prDtvCfg->u1Flags)
        {
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_FLAGS+16, (UINT32)(void *)&prDtvCfg->u1Flags, 1)!=0) { return 1; }
        }
        if ((rDtvCfg.u1Checksum != prDtvCfg->u1Checksum) || (rDtvCfg.u1Upgrade != prDtvCfg->u1Upgrade) ||
            (rDtvCfg.u1Reserved1 != prDtvCfg->u1Reserved1) || (rDtvCfg.u1Reserved2 != prDtvCfg->u1Reserved2))
        {
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_UPGRADE+16, (UINT32)(void *)&prDtvCfg->u1Upgrade, 4)!=0) { return 1; }
        }
        if ((rDtvCfg.u1Magic1 != DTVCFG_MAGIC_CHAR1) || (rDtvCfg.u1Magic2 != DTVCFG_MAGIC_CHAR2))
        {
            au1Magic[0] = DTVCFG_MAGIC_CHAR1;
            au1Magic[1] = DTVCFG_MAGIC_CHAR2;
            if (EEPDTV_Write((UINT64)EEPDTV_DTVCFG_MAGIC1+16, (UINT32)(void *)&prDtvCfg->u1Magic1, 2)!=0) { return 1; }
        }
    }
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
    i4Ret = EEPDTV_Read((UINT64)12, (UINT32)(void *)&rDtvCfg2, sizeof(DTVCFG_T));
    if (i4Ret) { return i4Ret; }
    i4Ret = EEPDTV_Read((UINT64)16, (UINT32)(void *)&rDtvCfg3, sizeof(DTVCFG_T));
    if (i4Ret) { return i4Ret; }

    // Check non-configured eeprom.
    if (((rDtvCfg1.u1Magic1 != DTVCFG_MAGIC_CHAR1) || (rDtvCfg1.u1Magic2 != DTVCFG_MAGIC_CHAR2)) &&
        (!_EepDtvIsValid(&rDtvCfg2)) && (!_EepDtvIsValid(&rDtvCfg3)))
    {
        // no initial value on 1, and 2/3 are invalid.
        prDtvCfg->u1Flags = 0;
        prDtvCfg->u1Upgrade = 0;
        prDtvCfg->u1Reserved1 = 0;
        prDtvCfg->u1Reserved2 = 0;
        return _EepDtvSyncData(prDtvCfg, 1, 1, 1);
    }
    // Check upgrade from old non-checksum version or only 1 is valid.
    else if ((!_EepDtvIsValid(&rDtvCfg1)) && (!_EepDtvIsValid(&rDtvCfg2)) && (!_EepDtvIsValid(&rDtvCfg3)))
    {   // 0, 0, 0
        prDtvCfg->u1Flags = rDtvCfg1.u1Flags;
        prDtvCfg->u1Upgrade = rDtvCfg1.u1Upgrade;
        prDtvCfg->u1Reserved1 = rDtvCfg1.u1Reserved1;
        prDtvCfg->u1Reserved2 = rDtvCfg1.u1Reserved2;
        return _EepDtvSyncData(prDtvCfg, 1, 1, 1);
    }
    // only 3 is valid.
    else if ((!_EepDtvIsValid(&rDtvCfg1)) && (!_EepDtvIsValid(&rDtvCfg2)))
    {   // 0, 0, 1
        prDtvCfg->u1Flags = rDtvCfg3.u1Flags;
        prDtvCfg->u1Upgrade = rDtvCfg3.u1Upgrade;
        prDtvCfg->u1Reserved1 = rDtvCfg3.u1Reserved1;
        prDtvCfg->u1Reserved2 = rDtvCfg3.u1Reserved2;
        return _EepDtvSyncData(prDtvCfg, 1, 1, 0);
    }
    // only 2 is valid.
    else if ((!_EepDtvIsValid(&rDtvCfg1)) && (_EepDtvIsValid(&rDtvCfg2)))
    {   // 0, 1, 0/0, 1, 1 both use 2.
        prDtvCfg->u1Flags = rDtvCfg2.u1Flags;
        prDtvCfg->u1Upgrade = rDtvCfg2.u1Upgrade;
        prDtvCfg->u1Reserved1 = rDtvCfg2.u1Reserved1;
        prDtvCfg->u1Reserved2 = rDtvCfg2.u1Reserved2;
        return _EepDtvSyncData(prDtvCfg, 1, 0, 1);
    }
    // then if each of 2/3 is invalid.
    else if ((_EepDtvIsValid(&rDtvCfg1)) && ((!_EepDtvIsValid(&rDtvCfg2)) || (!_EepDtvIsValid(&rDtvCfg3))))
    {   // 1, 0, 0/1, 1, 0/1, 0, 1, all use 1
        prDtvCfg->u1Flags = rDtvCfg1.u1Flags;
        prDtvCfg->u1Upgrade = rDtvCfg1.u1Upgrade;
        prDtvCfg->u1Reserved1 = rDtvCfg1.u1Reserved1;
        prDtvCfg->u1Reserved2 = rDtvCfg1.u1Reserved2;
        return _EepDtvSyncData(prDtvCfg, 0, 1, 1);
    }

    // Here is all of them are valid.
    prDtvCfg->u1Flags = _EepDtvDecideTriple(rDtvCfg1.u1Flags, rDtvCfg2.u1Flags, rDtvCfg3.u1Flags);
    prDtvCfg->u1Upgrade = _EepDtvDecideTriple(rDtvCfg1.u1Upgrade, rDtvCfg2.u1Upgrade, rDtvCfg3.u1Upgrade);
    prDtvCfg->u1Reserved1 = rDtvCfg1.u1Reserved1;
    prDtvCfg->u1Reserved2 = rDtvCfg1.u1Reserved2;
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

    ASSERT(prDtvCfg!=NULL);
    if (EEPDTV_Correction(prDtvCfg))
    {
        return 1;
    }

    // read the first DtvCfg location.
    i4Ret = EEPDTV_Read((UINT64) 0, (UINT32)(void *)prDtvCfg, sizeof(DTVCFG_T));
    if (i4Ret) { return i4Ret; }

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

