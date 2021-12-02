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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: pcmcia_cmd.c,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file pcmcia_cmd.c
 *  POD diagnostic command.
 */

#ifndef DEFINE_IS_LOG
#define DEFINE_IS_LOG    CLI_IsLog
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "x_os.h"
#include "x_debug.h"
#include "x_drv_cli.h"
#include "x_assert.h"
#include "x_bim.h"
#include "x_pcmcia.h"
#include "x_hal_5381.h"
#include "x_hal_926.h"
#include "x_stl_lib.h"
#include "x_rand.h"

#include "pcmcia_cmd.h"
#include "pcmcia_if.h"
#include "spi_if.h"
#include "nfi_if.h"

#if defined(CC_INTERNAL_CI) || defined(CC_EXTERNAL_CI) || defined(CC_INTERNAL_POD) || defined(CC_EXTERNAL_POD)

/******************************************************************************
* Configuration
******************************************************************************/
//#define MISCELLANEOUS_TEST

/******************************************************************************
* Local definition
******************************************************************************/
#define MAX_MSG_REC_BUF_SIZE        256
#define MAX_MSG_XMT_BUF_SIZE        80
#define WAIT_FOR_DA_INT_COUNT       100
#define RM_SESSION_NUM              1
#define AI_SESSION_NUM              2
#define CA_SESSION_NUM              3
#define MMI_SESSION_NUM             4
#define MSG_LEN_POS                 0
#define MSG_POS                     1

typedef enum
{
    POD_T_RCV = 0,
    POD_CREATE_T_C,
    POD_T_DATA_LAST,
    POD_OPEN_SESSION_RESPONSE,
    POD_PROFILE_INQ,
    POD_PROFILE_CHANGED,
    POD_PROFILE_REPLY,
    POD_APPLICATION_INFO_REQ,
    POD_CA_INFO_INQ,
    POD_MSG_MAX,
}   POD_MSG_T;

#define T_C_ID          0x1
#define CI_T_C_ID       0x11

BOOL POD_InitWeReTest(void);


/******************************************************************************
* Local variables
******************************************************************************/
#ifdef CC_MT5363
static PCMCIA_IF_TYPE_T u4InterfaceType = PCMCIA_IF_TYPE_SPI;
#else
static PCMCIA_IF_TYPE_T u4InterfaceType = PCMCIA_IF_TYPE_NFI;
#endif
static UINT8            au1MsgRec[MAX_MSG_REC_BUF_SIZE];
static UINT8*           pu1MsgRec=au1MsgRec;
static UINT32           u4CisLen = 0;
static UINT8            au1CisMap[MAX_CIS_MAP_SIZE];
static UINT16           u2MsgRecLen;
static POD_IO_STATE_T   eIoState = POD_SDA;

static UINT8    au1DiableCisMap[MAX_CIS_MAP_SIZE]=
{
 0x1d ,0x04 ,0x00 ,0xdb ,0x08 ,0xff ,0x1c ,0x03 ,0x00 ,0x08 ,0xff ,0x15 ,0x15 ,0x05 ,0x00 ,0x53,
 0x43 ,0x4d ,0x00 ,0x44 ,0x56 ,0x42 ,0x20 ,0x43 ,0x41 ,0x20 ,0x4d ,0x6f ,0x64 ,0x75 ,0x6c ,0x65,
 0x00 ,0xff ,0x20 ,0x04 ,0xff ,0xff ,0x01 ,0x00 ,0x1a ,0x15 ,0x01 ,0x0f ,0xfe ,0x01 ,0x01 ,0xc0,
 0x0e ,0x41 ,0x02 ,0x44 ,0x56 ,0x42 ,0x5f ,0x43 ,0x49 ,0x5f ,0x56 ,0x31 ,0x2e ,0x30 ,0x30 ,0x1b,
 0x11 ,0xc9 ,0x41 ,0x19 ,0x37 ,0x55 ,0x4e ,0x5e ,0x1d ,0x56 ,0xaa ,0x60 ,0x20 ,0x03 ,0x03 ,0x50,
 0xff ,0xff ,0x1b ,0x25 ,0xcf ,0x04 ,0x09 ,0x37 ,0x55 ,0x4d ,0x5d ,0x1d ,0x56 ,0x22 ,0xc0 ,0x09,
 0x44 ,0x56 ,0x42 ,0x5f ,0x48 ,0x4f ,0x53 ,0x54 ,0x00 ,0xc1 ,0x0e ,0x44 ,0x56 ,0x42 ,0x5f ,0x43,
 0x49 ,0x5f ,0x4d ,0x4f ,0x44 ,0x55 ,0x4c ,0x45 ,0x00 ,0x14 ,0x00 ,0xff ,0x00 ,0x00 ,0x00 ,0x00,
 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
 0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00,
 0xa1 ,0x06 ,0x57 ,0x19 ,0x27 ,0x81 ,0xe2 ,0x4e ,0x81 ,0x18 ,0x94 ,0x10 ,0xea ,0xd4 ,0x08 ,0x34,
 0x48 ,0x48 ,0x08 ,0x02 ,0xc8 ,0x00 ,0x04 ,0x04 ,0x60 ,0xb9 ,0xc3 ,0x92 ,0x52 ,0x0b ,0xb5 ,0x28,
 0x09 ,0x30 ,0x08 ,0xd4 ,0xf8 ,0x49 ,0xc0 ,0x36 ,0x07 ,0x08 ,0x28 ,0x4b ,0x5c ,0x45 ,0x82 ,0xd0,
 0x40 ,0x00 ,0x80 ,0xcd ,0x10 ,0x0d ,0xb7 ,0x5c ,0xac ,0x28 ,0x02 ,0x0a ,0x14 ,0x46 ,0x4d ,0x0b,
 0x06 ,0x24 ,0x00 ,0x49 ,0x3b ,0x01 ,0x00 ,0xf1 ,0x94 ,0x64 ,0x56 ,0x64 ,0xb7 ,0x61 ,0x38 ,0x38,
 0x0a ,0x48 ,0x10 ,0x21 ,0x54 ,0x90 ,0x27 ,0x23 ,0x26 ,0x84 ,0x04 ,0xa8 ,0xa0 ,0xb5 ,0x20 ,0x00
}
;
static UINT8    au1DiableTCReply[]=
{
    0x11, 0x00 ,0x83 ,0x01 ,0x11 ,0x80 ,0x02 ,0x11 ,0x00
};
static UINT8    au1MsgXmt[POD_MSG_MAX][MAX_MSG_XMT_BUF_SIZE] =
{   /* [0]: Total length
       [1]: Transport connection ID (LPDU)
       [2]: More/Last indicator (LPDU)
       [3]: TPDU tag (TPDU)
       [4]: TPDU length field (TPDU)
       [5]: t_c_id (TPDU)
       [6]: session_number_tag (SPDU)
       [7]: length_field (SPDU)
       [8]: session_nb (MS) (SPDU)
       [9]: session_nb (LS) (SPDU) */
    {5,     0x01, 0x00, 0x81, 0x01, T_C_ID},
    {5,     0x01, 0x00, 0x82, 0x01, T_C_ID},
    {5,     0x01, 0x00, 0xA0, 0x01, T_C_ID},
    {14,    0x01, 0x00, 0xA0, 0x0A, T_C_ID, 0x92, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {13,    0x01, 0x00, 0xA0, 0x09, T_C_ID, 0x90, 0x02, 0x00, RM_SESSION_NUM, 0x9F, 0x80, 0x10, 0x00},
    {13,    0x01, 0x00, 0xA0, 0x09, T_C_ID, 0x90, 0x02, 0x00, RM_SESSION_NUM, 0x9F, 0x80, 0x12, 0x00},
    {69,    0x01, 0x00, 0xA0, 0x41, T_C_ID, 0x90, 0x02, 0x00, RM_SESSION_NUM, 0x9F, 0x80, 0x11, 0x38,
            0x00, 0x01, 0x00, 0x41, 0x00, 0x02, 0x00, 0x81, 0x00, 0x24, 0x00, 0x41, 0x00, 0x03, 0x00,
            0x42, 0x00, 0x2A, 0x00, 0x41, 0x01, 0x04, 0x00, 0x41, 0x00, 0x90, 0x00, 0x41, 0x00, 0x20,
            0x00, 0x43, 0x00, 0xA0, 0x00, 0x42, 0x00, 0x80, 0x00, 0x81, 0x00, 0x40, 0x00, 0x81, 0x00,
            0x11, 0x00, 0x42, 0x00, 0x2B, 0x00, 0x41, 0x00, 0xB0, 0x00, 0xC1},
    {22,    0x01, 0x00, 0xA0, 0x12, T_C_ID, 0x90, 0x02, 0x00, AI_SESSION_NUM, 0x9F, 0x80, 0x20, 0x09,
            0x00, 0x28, 0x00, 0x64, 0x01, 0x01, 0x01, 0x03, 0x03},
    {13,    0x01, 0x00, 0xA0, 0x09, T_C_ID, 0x90, 0x02, 0x00, CA_SESSION_NUM, 0x9F, 0x80, 0x30, 0x00},
};

static UINT8    au1CiMsgXmt[POD_MSG_MAX][MAX_MSG_XMT_BUF_SIZE] =
{   /* [0]: Total length
       [1]: Transport connection ID (LPDU)
       [2]: More/Last indicator (LPDU)
       [3]: TPDU tag (TPDU)
       [4]: TPDU length field (TPDU)
       [5]: t_c_id (TPDU)
       [6]: session_number_tag (SPDU)
       [7]: length_field (SPDU)
       [8]: session_nb (MS) (SPDU)
       [9]: session_nb (LS) (SPDU) */
    {5,     CI_T_C_ID, 0x00, 0x81, 0x01, CI_T_C_ID},
    {5,     CI_T_C_ID, 0x00, 0x82, 0x01, CI_T_C_ID},
    {5,     CI_T_C_ID, 0x00, 0xA0, 0x01, CI_T_C_ID},
    {14,    CI_T_C_ID, 0x00, 0xA0, 0x0A, CI_T_C_ID, 0x92, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {13,    CI_T_C_ID, 0x00, 0xA0, 0x09, CI_T_C_ID, 0x90, 0x02, 0x00, RM_SESSION_NUM, 0x9F, 0x80, 0x10, 0x00},
    {13,    CI_T_C_ID, 0x00, 0xA0, 0x09, CI_T_C_ID, 0x90, 0x02, 0x00, RM_SESSION_NUM, 0x9F, 0x80, 0x12, 0x00},
    {69,    CI_T_C_ID, 0x00, 0xA0, 0x41, CI_T_C_ID, 0x90, 0x02, 0x00, RM_SESSION_NUM, 0x9F, 0x80, 0x11, 0x38,
            0x00, 0x01, 0x00, 0x41, 0x00, 0x02, 0x00, 0x81, 0x00, 0x24, 0x00, 0x41, 0x00, 0x03, 0x00,
            0x42, 0x00, 0x2A, 0x00, 0x41, 0x01, 0x04, 0x00, 0x41, 0x00, 0x90, 0x00, 0x41, 0x00, 0x20,
            0x00, 0x43, 0x00, 0xA0, 0x00, 0x42, 0x00, 0x80, 0x00, 0x81, 0x00, 0x40, 0x00, 0x81, 0x00,
            0x11, 0x00, 0x42, 0x00, 0x2B, 0x00, 0x41, 0x00, 0xB0, 0x00, 0xC1},
    {22,    CI_T_C_ID, 0x00, 0xA0, 0x12, CI_T_C_ID, 0x90, 0x02, 0x00, AI_SESSION_NUM, 0x9F, 0x80, 0x20, 0x09,
            0x00, 0x28, 0x00, 0x64, 0x01, 0x01, 0x01, 0x03, 0x03},
    {13,    CI_T_C_ID, 0x00, 0xA0, 0x09, CI_T_C_ID, 0x90, 0x02, 0x00, CA_SESSION_NUM, 0x9F, 0x80, 0x30, 0x00},
};

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------
static void _PcmciaDump(CHAR* pszTitle, UINT8* pu1Msg, UINT32 u4MsgLen)
{
    UINT32 u4I = 0;

    UNUSED(u4I);
    UNUSED(pszTitle);
    UNUSED(pu1Msg);
    UNUSED(u4MsgLen);

    LOG(3, "== %s (%d bytes) ==\n", pszTitle, u4MsgLen);
    for (u4I = 0; u4I < u4MsgLen; u4I+=16)
    {
        LOG(3, "%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x \n", 
                pu1Msg[u4I], pu1Msg[u4I+1], pu1Msg[u4I+2], pu1Msg[u4I+3], pu1Msg[u4I+4], pu1Msg[u4I+5], pu1Msg[u4I+6], pu1Msg[u4I+7], 
                pu1Msg[u4I+8], pu1Msg[u4I+9], pu1Msg[u4I+10], pu1Msg[u4I+11], pu1Msg[u4I+12], pu1Msg[u4I+13], pu1Msg[u4I+14], pu1Msg[u4I+15]);
    }
    LOG(3, "== End of %s ==\n\n", pszTitle);
}

static INT32 _PcmciaRegDefaultValueTest(UINT32 REG, UINT32 MSK, UINT32 VAL)
{
    if ( MSK == 0x00000000 )
    {
        return 0;
    }
    
    if ( (PCMCIA_RawRead32(REG) & MSK) != VAL )
    {
        Printf("Reg Default Error (0x%X): Read:0x%X, Predict:0x%X\n", REG, (PCMCIA_RawRead32(REG) & MSK), VAL);
        return -1;
    }
    return 0;
}

static INT32 _PcmciaRegRwTest(UINT32 REG, UINT32 MSK)
{
    UINT32 u4RegValue;
    UINT32 u4RandValud = rand();
    if ( MSK == 0x00000000 )
    {
        return 0;
    }
    
    PCMCIA_RawWrite32(REG, PATN_Fs);
    u4RegValue = PCMCIA_RawRead32(REG) & MSK;
    if ( (u4RegValue) != (PATN_Fs & MSK) )
    {
        Printf("RW Reg Test Failed1(0x%X) : Write:0x%X, Read:0x%X\n", REG, PATN_Fs&MSK, u4RegValue);
        return -1;
    }

    PCMCIA_RawWrite32(REG, PATN_0s);
    u4RegValue = PCMCIA_RawRead32(REG) & MSK;
    if ( (u4RegValue) != (PATN_0s & MSK) )
    {
        Printf("RW Reg Test Failed2(0x%X) : Write:0x%X, Read:0x%X\n", REG, PATN_0s&MSK, u4RegValue);
        return -1;
    }
    
    PCMCIA_RawWrite32(REG, PATN_As);
    u4RegValue = PCMCIA_RawRead32(REG) & MSK;
    if ( (u4RegValue) != (PATN_As & MSK) )
    {
        Printf("RW Reg Test Failed3(0x%X) : Write:0x%X, Read:0x%X\n", REG, PATN_As&MSK, u4RegValue);
        return -1;
    }

    PCMCIA_RawWrite32(REG, PATN_5s);
    u4RegValue = PCMCIA_RawRead32(REG) & MSK;
    if ( (u4RegValue) != (PATN_5s & MSK) )
    {
        Printf("RW Reg Test Failed4(0x%X) : Write:0x%X, Read:0x%X\n", REG, PATN_5s&MSK,  u4RegValue);
        return -1;
    }

    PCMCIA_RawWrite32(REG, u4RandValud);
    u4RegValue = PCMCIA_RawRead32(REG) & MSK;
    if ( (u4RegValue) != (u4RandValud & MSK) )
    {
        Printf("RW Reg Test Failed4(0x%X) : Write:0x%X, Read:0x%X\n", REG, PATN_5s&MSK,  u4RegValue);
        return -1;
    }

    return 0;
}

static BOOL _PcmciaCmdRegDefaultValueTest(void)
{
    // The following contains the full set of 8295 related register.

    _PcmciaRegDefaultValueTest(REG_RST,                 0x00000000, 0x00000006);  // MASK : 0x0
    _PcmciaRegDefaultValueTest(REG_CTRL,                0xFF4FFFFF, 0x10000000);
    _PcmciaRegDefaultValueTest(REG_CMD,                 0x0000007F, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_PBUS_STS,            0x00000000, 0x00003618);  // MASK : 0x0
    _PcmciaRegDefaultValueTest(REG_CMD_STS,             0x000000FF, 0x00000001);
    _PcmciaRegDefaultValueTest(REG_INT_STS,             0x7FFF01FF, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_DEV_ADDR,            0x03FFFFFF, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_DRAM_ADDR,           0x0FFFFFFF, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_BYTE_CNT,            0x000FFFFF, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_SDA_DATA_WRITE,      0x0000FFFF, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_SDA_DATA_READ,       0x0001FFFF, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_MEM_TIMING0,         0xFFFFFFFF, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_MEM_TIMING1,         0xFFFF0000, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_IO_TIMING0,          0xFFFFFFFF, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_IO_TIMING1,          0xFFFFFFFF, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_PC_MEM_WIN0_1,       0x0FFFFFFF, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_PC_MEM_WIN0_2,       0x03FFFFFF, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_PC_MEM_WIN1_1,       0x0FFFFFFF, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_PC_MEM_WIN1_2,       0x03FFFFFF, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_DEV_RW_STS,          0x03FFFFFF, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_DRAM_RW_STS,         0x0FFFFFFF, 0x00000000);
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegDefaultValueTest(REG_GPIO_INT_ENABLE,     0x00007FFF, 0x00000000);
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegDefaultValueTest(REG_POD_GPR_CNTL_SET0,   0xC7FF80F7, 0x80000034);
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegDefaultValueTest(REG_POD_GPR_CNTL_SET1,   0x3FFFFFFF, 0x00000000);
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegDefaultValueTest(REG_POD_GPR_CNTL_SET2,   0xFFFFFFFF, 0x00000000);
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegDefaultValueTest(REG_POD_GPR_CNTL_SET3,   0x00007FFF, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_POD_GPR_CNTL_SET4,   0x00000000, 0x00000000);
    _PcmciaRegDefaultValueTest(REG_WRAPPER_BIST_STS,    0x00000000, 0x00000000);  // MASK : 0x0
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegDefaultValueTest(REG_PLL_CNTL_SET0,       0x1FFFFFFF, 0x14920002);
    _PcmciaRegDefaultValueTest(REG_PLL_CNTL_SET1,       0xE0003FFF, 0x00000000);
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegDefaultValueTest(REG_MONITOR_OUTPUT_SEL,  0x000003FF, 0x00000000);
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    {
//        _PcmciaRegDefaultValueTest(REG_VERSION_CODE,        0x000000FF, 0x00000035);    // MT5135
        _PcmciaRegDefaultValueTest(REG_VERSION_CODE,        0x000000FF, 0x00000008);    // MT8295BNE
    }

    if ( ePcmciaCardType == PCMCIA_CARD_TYPE_POD )
    {
        // MPOD
        _PcmciaRegDefaultValueTest(REG_MPOD_EXTRA_CTRL,                 0x00FFFFFF, 0x00186200);
        _PcmciaRegDefaultValueTest(REG_MPOD_POWER_SET,                  0x00000003, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_MPOD_WRITE_CMD,                  0xFFFFFF07, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_MPOD_WRITE_DMA_DRAM_ADDR,        0x0FFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_MPOD_WRITE_DMA_BYTE_CNT,         0x000FFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_MPOD_WRITE_CMD_HDR,              0x00FFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_MPOD_READ_CMD,                   0x00000013, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_MPOD_READ_DMA_START_ADDR,        0x0FFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_MPOD_READ_DMA_END_ADDR,          0x0FFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_MPOD_READ_DMA_BYTE_CNT,          0x000FFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_MPOD_WRITE_CMD_STATUS,           0xFFFFFF1F, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_MPOD_READ_CMD_STATUS,            0xFFFFFF0F, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_MPOD_DEV_CHANNEL_STATUS,         0x00000000, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_MPOD_DMA_DRAM_READ_PTR,          0x0FFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_MPOD_READ_DMA_DRAM_WRITE_PTR,    0x0FFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_MPOD_READ_DMA_DRAM_READ_PTR,     0x0FFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_MPOD_READ_DMA_THRESHOLD,         0x0FFFFFFF, 0x000000FF);
        _PcmciaRegDefaultValueTest(REG_MPOD_INTR_STATUS_REG,            0x0000007F, 0x00000000);

        // MMX / MDMX
        _PcmciaRegDefaultValueTest(MPOD_GLOBAL_CONTROL_REGISTER_0,              0xC0000000, 0x40000000);
        _PcmciaRegDefaultValueTest(MPOD_GLOBAL_CONTROL_REGISTER_1,              0xFFFFFFFF, 0x03020100);
        _PcmciaRegDefaultValueTest(MPOD_GLOBAL_CONTROL_REGISTER_2,              0x0000FF00, 0x0000BC00);
        _PcmciaRegDefaultValueTest(MPOD_INTERRUPT_STATUS_REGISTER,              0x0003000B, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_INTERRUPT_MASK_REGISTER,                0x0003000B, 0x00000000);       
        _PcmciaRegDefaultValueTest(MPOD_MUX_FRAMER_CONTROL_REGISTER,            0x0FFFFFFf, 0x0000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_FRAMER_CONTROL_REGISTER1,           0xFFFFFF00, 0x01044700);
        _PcmciaRegDefaultValueTest(MPOD_MUX_FRAMER_TS_ERROR_HANDLING_REGISTER1, 0xFFFFFFFF, 0xF1F0F1F0);
        _PcmciaRegDefaultValueTest(MPOD_MUX_FRAMER_TS_ERROR_HANDLING_REGISTER2, 0xFFFFFFFF, 0xF1F0F1F0);
        _PcmciaRegDefaultValueTest(MPOD_MUX_DBM_CONTROL_REGISTER,               0xC00000FF, 0x40000008);
        _PcmciaRegDefaultValueTest(MPOD_MUX_CLOCK_DIVISION_REGISTER,            0xFFFFFFFF, 0x00000008);
        _PcmciaRegDefaultValueTest(MPOD_MUX_TIMESTAMP_REGISTR,                  0x00000000, 0x00000000);  // MASK : 0x0
        _PcmciaRegDefaultValueTest(MPOD_MUX_STATUS_REGISTER1,                   0xFFFF0000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_STATUS_REGISTER2,                   0x0FFFFFFF, 0x00001001);
        _PcmciaRegDefaultValueTest(MPOD_MUX_FRAMER_0_STATUS_REGISTER1,          0x000FFFFF, 0x00010101);
        _PcmciaRegDefaultValueTest(MPOD_MUX_FRAMER_0_STATUS_REGISTER2,          0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_FRAMER_1_STATUS_REGISTER1,          0x000FFFFF, 0x00010101);
        _PcmciaRegDefaultValueTest(MPOD_MUX_FRAMER_1_STATUS_REGISTER2,          0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_FRAMER_2_STATUS_REGISTER1,          0x000FFFFF, 0x00010101);
        _PcmciaRegDefaultValueTest(MPOD_MUX_FRAMER_2_STATUS_REGISTER2,          0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_FRAMER_3_STATUS_REGISTER1,          0x000FFFFF, 0x00010101);
        _PcmciaRegDefaultValueTest(MPOD_MUX_FRAMER_3_STATUS_REGISTER2,          0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_ERROR_INTERRUPT_REASON_REGISTER,    0x0FFf00FF, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_STATUS_INTERRUPT_REASON_REGISTER,   0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_HOST_RES_REGISTER0,                 0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_HOST_RES_REGISTER1,                 0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_PREHEADER_RESERVED_REGISTER,        0x0000FFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER0,       0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER1,       0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER2,       0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER3,       0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER4,       0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER5,       0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER6,       0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER7,       0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER8,       0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER9,       0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER10,      0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER11,      0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER12,      0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER13,      0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER14,      0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER15,      0x80000000, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_DEMUX_FRAMER_CONTROL_REGISTER,          0x00310009, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_DEMUX_PACKET_COUNT_REGISTER,            0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_DEMUX_ERROR_INTERRUPT_REASON_REGISTER,  0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_DEMUX_STATUS_INTERRUPT_REASON_REGISTER, 0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_DEMUX_TS_OUTPUT_CONTROL,                0x0000000f, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_CMP_CONFIGURATION_REGISTER,             0x00FF00FF, 0x001000C8);
        _PcmciaRegDefaultValueTest(MPOD_CMP_ERROR_INTERRUPT_REASON_REGISTER,    0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_CMP_FRAMER_0_STATUS_REGISTER,           0x00000000, 0x00000000);  // MASK : 0x0
        _PcmciaRegDefaultValueTest(MPOD_CMP_FRAMER_1_STATUS_REGISTER,           0x0000FFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(MPOD_CMP_PROJGECT_NAME,                      0xFFFFFFFF, 0x38323935); // 0x38323935
        _PcmciaRegDefaultValueTest(MPOD_CMP_HARDWARE_REVISION,                  0xFFFFFFFF, 0x302E3130); // 0x302E3130

        // CKGEN
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPR0,             0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPR1,             0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPR2,             0x00FF00FF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPR3,             0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPR4,             0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPR5,             0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPR6,             0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPR7,             0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPR8,             0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPR9,             0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPR10,            0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPR11,            0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPR12,            0x000FFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_PAD_CFG0,         0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_PAD_CFG1,         0x3FFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPIO_IN0,         0x00000000, 0x00000000);  // MASK : 0x0
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPIO_IN1,         0x00000000, 0x00000000);  // MASK : 0x0
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPIOINT_EN0,      0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_GPIOINT_EN1,      0x000000FF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_ISTATUS0,         0xFFFFFFFF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_ISTATUS1,         0x000000FF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_DATAPATH_CFG0,    0x00007FC0, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_ANALOG_CFG0,      0x07FFFFFF, 0x00000003);
        _PcmciaRegDefaultValueTest(REG_CKGEN_PLL_CTL1,         0xFFF80000, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_CKGEN_CHIP_VERSION,     0xC003FFFF, 0xC000DEAD);  // 0xC000DEAD

        // WRAPPER
        _PcmciaRegDefaultValueTest(REG_WRP_STATUS,      0x00000000, 0x00000000);  // MASK : 0x0
        _PcmciaRegDefaultValueTest(REG_WRP_INTEN,       0x00000003, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_WRP_INTST,       0x0000000F, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_WRP_INTCL,       0x00000003, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_WRP_MISC,        0x000000FF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_WRP_TSI_CNT,     0x000000FF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_WRP_POD_CNT,     0x000000FF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_WRP_MONSEL,      0x00000003, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_WRP_MON,         0x00000007, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_WRP_PWM0,        0xFFFFFFF1, 0xFF800000);
        _PcmciaRegDefaultValueTest(REG_WRP_PWM1,        0xFFFFFFF1, 0xFF800000);
        _PcmciaRegDefaultValueTest(REG_WRP_PWM2,        0xFFFFFFF1, 0xFF800000);
        _PcmciaRegDefaultValueTest(REG_WRP_PWMLVDS,     0xC0FFC0FF, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_WRP_PWMPHASE,    0x0000FF03, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_WRP_SM0CTL0,     0x8FFF003F, 0x0000000C);
        _PcmciaRegDefaultValueTest(REG_WRP_SM0CTL1,     0x00FF0771, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_WRP_SM0D0,       0x00000000, 0x00000000);  // MASK : 0x0
        _PcmciaRegDefaultValueTest(REG_WRP_SM0D1,       0x00000000, 0x00000000);  // MASK : 0x0
        _PcmciaRegDefaultValueTest(REG_WRP_SM1CTL0,     0x1FFF001F, 0x0000000C);
        _PcmciaRegDefaultValueTest(REG_WRP_SM1CTL1,     0x00FF0771, 0x00000000);
        _PcmciaRegDefaultValueTest(REG_WRP_SM1D0,       0x00000000, 0x00000000);  // MASK : 0x0
        _PcmciaRegDefaultValueTest(REG_WRP_SM1D1,       0x00000000, 0x00000000);  // MASK : 0x0
    }
    return TRUE;
}

static BOOL _PcmciaCmdRwRegTest(void)
{
    // The following contains the full set of 8295 related register.
    
    _PcmciaRegRwTest(REG_RST,                 0x00000000);  // MASK : 0x0
    _PcmciaRegRwTest(REG_CTRL,                0xFF4FFFFF);
    _PcmciaRegRwTest(REG_CMD,                 0x00000000);  // MASK : 0x0
    _PcmciaRegRwTest(REG_PBUS_STS,            0x00000000);  // MASK : 0x0
    _PcmciaRegRwTest(REG_CMD_STS,             0x00000000);  // MASK : 0x0
    _PcmciaRegRwTest(REG_INT_STS,             0x00000000);  // MASK : 0x0
    _PcmciaRegRwTest(REG_DEV_ADDR,            0x03FFFFFF);
    _PcmciaRegRwTest(REG_DRAM_ADDR,           0x0FFFFFFF);
    _PcmciaRegRwTest(REG_BYTE_CNT,            0x000FFFFF);
    _PcmciaRegRwTest(REG_SDA_DATA_WRITE,      0x0000FFFF);
    _PcmciaRegRwTest(REG_SDA_DATA_READ,       0x00000000);  // MASK : 0x0
    _PcmciaRegRwTest(REG_MEM_TIMING0,         0xFFFFFFFF);
    _PcmciaRegRwTest(REG_MEM_TIMING1,         0xFFFF0000);
    _PcmciaRegRwTest(REG_IO_TIMING0,          0xFFFFFFFF);
    _PcmciaRegRwTest(REG_IO_TIMING1,          0xFFFFFFFF);
    _PcmciaRegRwTest(REG_PC_MEM_WIN0_1,       0x0FFFFFFF);
    _PcmciaRegRwTest(REG_PC_MEM_WIN0_2,       0x03FFFFFF);
    _PcmciaRegRwTest(REG_PC_MEM_WIN1_1,       0x0FFFFFFF);
    _PcmciaRegRwTest(REG_PC_MEM_WIN1_2,       0x03FFFFFF);
    _PcmciaRegRwTest(REG_DEV_RW_STS,          0x00000000);  // MASK : 0x0
    _PcmciaRegRwTest(REG_DRAM_RW_STS,         0x00000000);  // MASK : 0x0
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegRwTest(REG_GPIO_INT_ENABLE,     0x00007FFF);
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegRwTest(REG_POD_GPR_CNTL_SET0,   0xC7FF80F7);
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegRwTest(REG_POD_GPR_CNTL_SET1,   0x3FFFFFFF);
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegRwTest(REG_POD_GPR_CNTL_SET2,   0xFFFFFFFF);
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegRwTest(REG_POD_GPR_CNTL_SET3,   0x00007FFF);
    _PcmciaRegRwTest(REG_POD_GPR_CNTL_SET4,   0x00000000);  // MASK : 0x0
    _PcmciaRegRwTest(REG_WRAPPER_BIST_STS,    0x00000000);  // MASK : 0x0
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegRwTest(REG_PLL_CNTL_SET0,       0x00000000);  // MASK : 0x0
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegRwTest(REG_PLL_CNTL_SET1,       0x00000000);  // MASK : 0x0
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegRwTest(REG_MONITOR_OUTPUT_SEL,  0x000003FF);
    if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL ) 
    _PcmciaRegRwTest(REG_VERSION_CODE,        0x00000000);  // MASK : 0x0



    if ( ePcmciaCardType == PCMCIA_CARD_TYPE_POD )
    {
        // MPOD
        _PcmciaRegRwTest(REG_MPOD_EXTRA_CTRL,                 0x00FFFFFF);
        _PcmciaRegRwTest(REG_MPOD_POWER_SET,                  0x00000003);
        _PcmciaRegRwTest(REG_MPOD_WRITE_CMD,                  0xFFFFFF07);
        _PcmciaRegRwTest(REG_MPOD_WRITE_DMA_DRAM_ADDR,        0x0FFFFFFF);
        _PcmciaRegRwTest(REG_MPOD_WRITE_DMA_BYTE_CNT,         0x000FFFFF);
        _PcmciaRegRwTest(REG_MPOD_WRITE_CMD_HDR,              0x00FFFFFF);
        _PcmciaRegRwTest(REG_MPOD_READ_CMD,                   0x00000013);
        _PcmciaRegRwTest(REG_MPOD_READ_DMA_START_ADDR,        0x0FFFFFFF);
        _PcmciaRegRwTest(REG_MPOD_READ_DMA_END_ADDR,          0x0FFFFFFF);
        _PcmciaRegRwTest(REG_MPOD_READ_DMA_BYTE_CNT,          0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_MPOD_WRITE_CMD_STATUS,           0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_MPOD_READ_CMD_STATUS,            0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_MPOD_DEV_CHANNEL_STATUS,         0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_MPOD_DMA_DRAM_READ_PTR,          0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_MPOD_READ_DMA_DRAM_WRITE_PTR,    0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_MPOD_READ_DMA_DRAM_READ_PTR,     0x0FFFFFFF);
        _PcmciaRegRwTest(REG_MPOD_READ_DMA_THRESHOLD,         0x0FFFFFFF);
        _PcmciaRegRwTest(REG_MPOD_INTR_STATUS_REG,            0x00000000);  // MASK : 0x0

        // MMX / MDMX
        _PcmciaRegRwTest(MPOD_GLOBAL_CONTROL_REGISTER_0,              0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_GLOBAL_CONTROL_REGISTER_1,              0xFFFFFFFF);
        _PcmciaRegRwTest(MPOD_GLOBAL_CONTROL_REGISTER_2,              0x0000FF00);
        _PcmciaRegRwTest(MPOD_INTERRUPT_STATUS_REGISTER,              0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_INTERRUPT_MASK_REGISTER,                0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_MUX_FRAMER_CONTROL_REGISTER,            0x0FFFFFFf);
        _PcmciaRegRwTest(MPOD_MUX_FRAMER_CONTROL_REGISTER1,           0xFFFFFF00);
        _PcmciaRegRwTest(MPOD_MUX_FRAMER_TS_ERROR_HANDLING_REGISTER1, 0xFFFFFFFF);
        _PcmciaRegRwTest(MPOD_MUX_FRAMER_TS_ERROR_HANDLING_REGISTER2, 0xFFFFFFFF);
        _PcmciaRegRwTest(MPOD_MUX_DBM_CONTROL_REGISTER,               0x000000FF);
        _PcmciaRegRwTest(MPOD_MUX_CLOCK_DIVISION_REGISTER,            0xFFFFFFFF);
        _PcmciaRegRwTest(MPOD_MUX_TIMESTAMP_REGISTR,                  0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_MUX_STATUS_REGISTER1,                   0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_MUX_STATUS_REGISTER2,                   0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_MUX_FRAMER_0_STATUS_REGISTER1,          0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_MUX_FRAMER_0_STATUS_REGISTER2,          0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_MUX_FRAMER_1_STATUS_REGISTER1,          0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_MUX_FRAMER_1_STATUS_REGISTER2,          0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_MUX_FRAMER_2_STATUS_REGISTER1,          0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_MUX_FRAMER_2_STATUS_REGISTER2,          0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_MUX_FRAMER_3_STATUS_REGISTER1,          0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_MUX_FRAMER_3_STATUS_REGISTER2,          0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_MUX_ERROR_INTERRUPT_REASON_REGISTER,    0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_MUX_STATUS_INTERRUPT_REASON_REGISTER,   0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_MUX_HOST_RES_REGISTER0,                 0xFFFFFFFF);
        _PcmciaRegRwTest(MPOD_MUX_HOST_RES_REGISTER1,                 0xFFFFFFFF);
        _PcmciaRegRwTest(MPOD_MUX_PREHEADER_RESERVED_REGISTER,        0x0000FFFF);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER0,       0xFFFF0000);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER1,       0xFFFF0000);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER2,       0xFFFF0000);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER3,       0xFFFF0000);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER4,       0xFFFF0000);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER5,       0xFFFF0000);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER6,       0xFFFF0000);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER7,       0xFFFF0000);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER8,       0xFFFF0000);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER9,       0xFFFF0000);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER10,      0xFFFF0000);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER11,      0xFFFF0000);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER12,      0xFFFF0000);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER13,      0xFFFF0000);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER14,      0xFFFF0000);
        _PcmciaRegRwTest(MPOD_MUX_INVERSE_PID_FILTER_REGISTER15,      0xFFFF0000);
        _PcmciaRegRwTest(MPOD_DEMUX_FRAMER_CONTROL_REGISTER,          0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_DEMUX_PACKET_COUNT_REGISTER,            0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_DEMUX_ERROR_INTERRUPT_REASON_REGISTER,  0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_DEMUX_STATUS_INTERRUPT_REASON_REGISTER, 0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_DEMUX_TS_OUTPUT_CONTROL,                0x0000000f);
        _PcmciaRegRwTest(MPOD_CMP_CONFIGURATION_REGISTER,             0x00FF00FF);
        _PcmciaRegRwTest(MPOD_CMP_ERROR_INTERRUPT_REASON_REGISTER,    0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_CMP_FRAMER_0_STATUS_REGISTER,           0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_CMP_FRAMER_1_STATUS_REGISTER,           0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_CMP_PROJGECT_NAME,                      0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(MPOD_CMP_HARDWARE_REVISION,                  0x00000000);  // MASK : 0x0

        // WRAPPER
        _PcmciaRegRwTest(REG_WRP_STATUS,      0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_WRP_INTEN,       0x00000003);
        _PcmciaRegRwTest(REG_WRP_INTST,       0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_WRP_INTCL,       0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_WRP_MISC,        0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_WRP_TSI_CNT,     0x000000FF);
        _PcmciaRegRwTest(REG_WRP_POD_CNT,     0x000000FF);
        _PcmciaRegRwTest(REG_WRP_MONSEL,      0x00000003);
        _PcmciaRegRwTest(REG_WRP_MON,         0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_WRP_PWM0,        0xFFFFFFF1);
        _PcmciaRegRwTest(REG_WRP_PWM1,        0xFFFFFFF1);
        _PcmciaRegRwTest(REG_WRP_PWM2,        0xFFFFFFF1);
        _PcmciaRegRwTest(REG_WRP_PWMLVDS,     0xC0FFC0FF);
        _PcmciaRegRwTest(REG_WRP_PWMPHASE,    0x0000FF03);
        _PcmciaRegRwTest(REG_WRP_SM0CTL0,     0x8FFF0023);
        _PcmciaRegRwTest(REG_WRP_SM0CTL1,     0x00000770);
        _PcmciaRegRwTest(REG_WRP_SM0D0,       0xFFFFFFFF);
        _PcmciaRegRwTest(REG_WRP_SM0D1,       0xFFFFFFFF);
        _PcmciaRegRwTest(REG_WRP_SM1CTL0,     0x8FFF0003);
        _PcmciaRegRwTest(REG_WRP_SM1CTL1,     0x00000770);
        _PcmciaRegRwTest(REG_WRP_SM1D0,       0xFFFFFFFF);
        _PcmciaRegRwTest(REG_WRP_SM1D1,       0xFFFFFFFF);

        // CKGEN
        _PcmciaRegRwTest(REG_CKGEN_GPR0,             0xFFFFFFFF);
        _PcmciaRegRwTest(REG_CKGEN_GPR1,             0xFFFFFFFF);
        _PcmciaRegRwTest(REG_CKGEN_GPR2,             0x00FF00FF);
        _PcmciaRegRwTest(REG_CKGEN_GPR3,             0xFFFFFFFF);
        _PcmciaRegRwTest(REG_CKGEN_GPR4,             0xFFFFFFFF);
        _PcmciaRegRwTest(REG_CKGEN_GPR5,             0xFFFFFFFF);
        _PcmciaRegRwTest(REG_CKGEN_GPR6,             0xFFFFFFFF);
        _PcmciaRegRwTest(REG_CKGEN_GPR7,             0xFFFFFFFF);
        _PcmciaRegRwTest(REG_CKGEN_GPR8,             0xFFFFFFFF);
        _PcmciaRegRwTest(REG_CKGEN_GPR9,             0xFFFFFFFF);
        _PcmciaRegRwTest(REG_CKGEN_GPR10,            0xFFFFFFFF);
        _PcmciaRegRwTest(REG_CKGEN_GPR11,            0xFFFFFFFF);
        _PcmciaRegRwTest(REG_CKGEN_GPR12,            0x000FFFFF);
        _PcmciaRegRwTest(REG_CKGEN_PAD_CFG0,         0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_CKGEN_PAD_CFG1,         0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_CKGEN_GPIO_IN0,         0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_CKGEN_GPIO_IN1,         0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_CKGEN_GPIOINT_EN0,      0xFFFFFFFF);
        _PcmciaRegRwTest(REG_CKGEN_GPIOINT_EN1,      0x000000FF);
        _PcmciaRegRwTest(REG_CKGEN_ISTATUS0,         0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_CKGEN_ISTATUS1,         0x00000000);  // MASK : 0x0
        _PcmciaRegRwTest(REG_CKGEN_DATAPATH_CFG0,    0x00007FC0);
        _PcmciaRegRwTest(REG_CKGEN_ANALOG_CFG0,      0x07FFFFFF);
        _PcmciaRegRwTest(REG_CKGEN_PLL_CTL1,         0xFFF80000);
        _PcmciaRegRwTest(REG_CKGEN_CHIP_VERSION,     0x00000000);  // MASK : 0x0
    }

    return TRUE;
}

static BOOL _PcmciaCmdRegNoMapTest(void)
{
    UINT16  i;

    // No register-mapped IO space test
    for(i = REG_GPIO_INT_ENABLE + 4; i <= 0x3FF; i+=4)
    {
        PCMCIA_RawWrite32(i, PATN_As);
        PCMCIA_RawRead32(i);
        PCMCIA_RawWrite32(i, PATN_5s);
        PCMCIA_RawRead32(i);
    }

    return TRUE;
}

static INT32 _PcmciaMpodStrToBinary(UINT8* u1Src, UINT8* u1Dest, UINT32 u4Len)
{
    UINT32 u4CurrentSrcIndex=0;
    UINT32 u4CurrentDestIndex=0;
    UINT32 i;
    UINT8 u1TempValue=0;

    if ( u1Src[0] == '0' && u1Src[1] == 'x' )
    {
        u4CurrentSrcIndex = 2;
    }

    for ( i = 0 ; i < u4Len ; i++ )
    {
        u1TempValue = 0;
        if ( u1Src[u4CurrentSrcIndex] >= '0' && u1Src[u4CurrentSrcIndex] <= '9' )
        {
            u1TempValue |= (u1Src[u4CurrentSrcIndex]-'0')<<4;
        }
        else if ( u1Src[u4CurrentSrcIndex] >= 'A' && u1Src[u4CurrentSrcIndex] <= 'F' )
        {
            u1TempValue |= (u1Src[u4CurrentSrcIndex]-'A'+10)<<4;
        }
        else if ( u1Src[u4CurrentSrcIndex] >= 'a' && u1Src[u4CurrentSrcIndex] <= 'f' )
        {
            u1TempValue |= (u1Src[u4CurrentSrcIndex]-'a'+10)<<4;
        }

        if ( u1Src[u4CurrentSrcIndex+1] >= '0' && u1Src[u4CurrentSrcIndex+1] <= '9' )
        {
            u1TempValue |= (u1Src[u4CurrentSrcIndex+1]-'0');
        }
        else if ( u1Src[u4CurrentSrcIndex+1] >= 'A' && u1Src[u4CurrentSrcIndex+1] <= 'F' )
        {
            u1TempValue |= (u1Src[u4CurrentSrcIndex+1]-'A'+10);
        }
        else if ( u1Src[u4CurrentSrcIndex+1] >= 'a' && u1Src[u4CurrentSrcIndex+1] <= 'f' )
        {
            u1TempValue |= (u1Src[u4CurrentSrcIndex+1]-'a'+10);
        }

        u1Dest[u4CurrentDestIndex] = u1TempValue;
        u4CurrentSrcIndex+=2;
        u4CurrentDestIndex+=1;
    }

    return 0;
}

static INT32 _PcmciaParseCis(INT32 i4Argc, const CHAR** szArgv)
{
    INT32       i4Return;
    UINT8*      pu1Cis;
    CIS_INFO_T  rCisInfo;

    // pod.parse.cis

    pu1Cis = (UINT8 *)((unsigned int)(au1CisMap));

    if (u4CisLen > 0)
    {
        i4Return = PCMCIA_ParseCis(pu1Cis, &rCisInfo);
        if (i4Return == POD_OK)
        {
            LOG(3, "POD module type: %s\n", rCisInfo.fgPodModuleType ? "Yes" : "No");
            LOG(3, "Required 5V Vpp: %s\n", rCisInfo.fgVpp ? "Yes" : "No");
            LOG(3, "COR value = 0x%x\n", rCisInfo.u1CorValue);
            LOG(3, "COR address = 0x%x\n", rCisInfo.u2CorAddr);
        }
    }
    else
    {
        i4Return = POD_CIS_ERROR;
    }

    return i4Return;
}

static BOOL _PcmciaWaitForDataAvailable(void)
{
    UINT16      u2WaitForDaIntCnt;

    u2WaitForDaIntCnt = 0;
    while (u2WaitForDaIntCnt++ < WAIT_FOR_DA_INT_COUNT)
    {
        if (PCMCIA_WaitForModuleStatus(DA, 1) == POD_OK) /* Test DA */
        {
            break;
        }
        x_thread_delay(1);
    }

    if (u2WaitForDaIntCnt == WAIT_FOR_DA_INT_COUNT)
    {
        return FALSE;
    }

    return TRUE;
}

static INT32 _PcmciaCmdCreateTC(INT32 i4Argc, const CHAR** szArgv)
{
    INT32           i4Return;
    extern INT32 PODCI_WriteCard(POD_IO_STATE_T eIoState, const UINT8* pu1Data, UINT16 u2DataLen);
    extern INT32 PODCI_ReadCard(POD_IO_STATE_T   eIoState, UINT8* pu1Data, UINT16* pu2DataLen);

    i4Return = PCMCIA_SetPodCiAccessMode(POD_DATA_CHANNEL);
    if (i4Return != POD_OK)
    {
        return i4Return;
    }

    /* Send Create_T_C() */
    LOG(3, "Send Create_T_C()\n");
    if ( ePcmciaCardType == PCMCIA_CARD_TYPE_CI )
    {
        i4Return = PODCI_WriteCard(eIoState, &au1CiMsgXmt[POD_CREATE_T_C][MSG_POS],
                                (UINT16)(au1CiMsgXmt[POD_CREATE_T_C][MSG_LEN_POS]));
    }
    else
    {
        i4Return = PODCI_WriteCard(eIoState, &au1MsgXmt[POD_CREATE_T_C][MSG_POS],
                                (UINT16)(au1MsgXmt[POD_CREATE_T_C][MSG_LEN_POS]));
    }

    if (i4Return != POD_OK)
    {
        return i4Return;
    }

    /* Wait for Create_T_C_Reply() */
    x_thread_delay(100);
    if (!_PcmciaWaitForDataAvailable())
    {
        return POD_TIME_OUT;
    }

    u2MsgRecLen = MAX_MSG_REC_BUF_SIZE;
    i4Return = PODCI_ReadCard(eIoState, pu1MsgRec, &u2MsgRecLen);
    if (i4Return != POD_OK)
    {
        return i4Return;
    }

    if ( ePcmciaCardType == PCMCIA_CARD_TYPE_CI )
    {
        if (x_memcmp(pu1MsgRec, au1DiableTCReply, 9))
        {
            LOG(1, "++++++++ TC reply fail\n");
        }
    }
    else
    {}

    if ( (pu1MsgRec[0]!=0x11) || (pu1MsgRec[1]!=0x00) || (pu1MsgRec[2]!=0x83) ||
         (pu1MsgRec[3]!=0x01) || (pu1MsgRec[4]!=0x11) || (pu1MsgRec[5]!=0x80) ||
         (pu1MsgRec[6]!=0x02) || (pu1MsgRec[7]!=0x11) || (pu1MsgRec[8]!=0x00) )
    {
        LOG(0, "Create_T_C_Reply from CAM Error !\n");
        LOG(3, "\"Create_T_C_Reply\" from CAM: %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", 
                pu1MsgRec[0], pu1MsgRec[1], pu1MsgRec[2], pu1MsgRec[3], pu1MsgRec[4], 
                pu1MsgRec[5], pu1MsgRec[6], pu1MsgRec[7], pu1MsgRec[8]);
        LOG(3, "\"Create_T_C_Reply\" Golden  : 11 00 83 01 11 80 02 11 00\n");
    }
    else
    {
        LOG(0, "Create_T_C_Reply from CAM OK\n");
    }


    return i4Return;
}

static void _PcmciaConfigBadWins(BOOL bBadMem)
{
    if(bBadMem)
    {
        // Configure POD/CI memory windows
        PCMCIA_RawWrite32(REG_PC_MEM_WIN0_1, 0x0); // Memory window 0 (attribute)
        PCMCIA_RawWrite32(REG_PC_MEM_WIN0_2, 0x0);
        PCMCIA_RawWrite32(REG_PC_MEM_WIN1_1, 0x0); // Memory window 1 (common)
        PCMCIA_RawWrite32(REG_PC_MEM_WIN1_2, 0x0);
    }
    else
    {
        // Configure POD/CI memory windows
        PCMCIA_RawWrite32(REG_PC_MEM_WIN0_1, 0x0);      // Memory window 0 (attribute)
        PCMCIA_RawWrite32(REG_PC_MEM_WIN0_2, 0x2000);
        PCMCIA_RawWrite32(REG_PC_MEM_WIN1_1, 0x8000000); // Memory window 1 (common)
        PCMCIA_RawWrite32(REG_PC_MEM_WIN1_2, 0x2000);
    }
}

static INT32 PcmciaCmdInit(INT32 i4Argc, const CHAR** szArgv)
{
    INT32       i4Return;
    UINT32      u4I;
    UINT32      u4Loop = 1;
    extern BOOL fgPcmciaIsInitedByCli;

    if (i4Argc > 1)
    {
        u4Loop = StrToInt(szArgv[1]);
    }

    fgPcmciaIsInitedByCli = TRUE;
    i4Return = PCMCIA_Init();
    if (i4Return != POD_OK)
    {
        return i4Return;
    }

    if ( PCMCIA_DetectCard() )
    {
        if (PCMCIA_EnableIsr() == POD_OK)
        {
            /* Perform POD init */
            for (u4I = 0; u4I < u4Loop; u4I++)
            {
                if (u4Loop > 1)
                {
                    LOG(5, "POD init test loop: %d\n", u4I+1);
                }

                i4Return = PCMCIA_LowLevelInit();
                if (i4Return != POD_OK)
                {
                    break;
                }
            }
        }
    }
    else
    {
        LOG(0, "CAM hasn't been inserted yet\n");
    }

    return i4Return;
}

static INT32 PcmciaCmd5VCtrl (INT32 i4Argc, const CHAR ** szArgv)
{
    UINT32 u4ManualControl;
    if(i4Argc != 2)
    {
        LOG(0, "5vpc [0:Original, 1:ManualControl]\n");
        return 0;
    }
    else
    {
        u4ManualControl = StrToInt(szArgv[1]);
    }

    if ( u4ManualControl == 0 )
    {
        PCMCIA_CamPwrControl(FALSE);
    }
    else
    {
        PCMCIA_CamPwrControl(TRUE);
    }
    
    return 0;
}

static INT32 PcmciaCmdSetAccessMode (INT32 i4Argc, const CHAR ** szArgv)
{
    if(i4Argc != 2)
    {
        LOG(0, "Usage: %s [0:SDA, 1:DMA]\n", szArgv[0]);
        LOG(0, "Current Setting : %d\n", eIoState);
    }
    else
    {
        if ( StrToInt(szArgv[1]) == 0 )
        {
            LOG(0, "    Set Access Mode to SDA. (Only affect p.v.tc)\n");
            eIoState = POD_SDA;
        }
        else
        {
            LOG(0, "    Set Access Mode to DMA. (Only affect p.v.tc)\n");
            eIoState = POD_DMA;
        }
    }

    return 0;
}

static INT32 PcmciaCmdSlt (INT32 i4Argc, const CHAR ** szArgv)
{
    if(i4Argc == 1)
    {
        PCMCIA_SLT(0, 27, 1);
        PCMCIA_SLT(1, 27, 1);
    }
    else if ( (i4Argc >=2) && (i4Argc <= 4) )
    {
        PCMCIA_SLT((UINT8)StrToInt(szArgv[1]), (UINT8)StrToInt(szArgv[2]), (UINT8)StrToInt(szArgv[3]));
    }
    else
    {
        LOG(0, "Usage: %s [TsIndex (0/1)][Clock Rate (27/30/48/54/60/72/80/108)][DataWidth(1/2/8)]\n", szArgv[0]);
    }
    
    return 0;
}

static INT32 PcmciaCmdDemo (INT32 i4Argc, const CHAR ** szArgv)
{
    PCMCIA_Demo(NULL);
    return 0;
}

static INT32 PcmciaCmdDemodTsOnOff (INT32 i4Argc, const CHAR ** szArgv)
{
    if(i4Argc != 2)
    {
        LOG(0, "Usage: %s [1/0]\n", szArgv[0]);
        return 0;
    }
    if ( StrToInt(szArgv[1]) )
    {
        PCMCIA_EnableDemodTs();
    }
    else
    {
        PCMCIA_DisableDemodTs();
    }
    return 0;
}

static INT32 PcmciaCmdSet8295ClkRate (INT32 i4Argc, const CHAR ** szArgv)
    {
    if(i4Argc != 2)
    {
        LOG(0, "Usage: %s [63/72/81/108/135]\n", szArgv[0]);
        return 0;
    }
    PCMCIA_ExternalIcSetTsClkRate(StrToInt(szArgv[1]));
    return 0;
}

static INT32 PcmciaCmdReset (INT32 i4Argc, const CHAR ** szArgv)
{
    PCMCIA_ResetPcmciaHw();
    return 0;
}

static INT32 PcmciaCmdSingleRegTest (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i;
    INT32 i4Loops = 1;
    UINT32 u4Addr;
    UINT32 u4Mask = 0x00ffffff;
    UINT32 u4Mode = 0;
    UINT32 u4OrgValue = 0, u4TempValue = 0;
    
    if( i4Argc == 2 || i4Argc == 3 || i4Argc == 4 || i4Argc == 5 )
    {
        u4Addr = (INT32)StrToInt(szArgv[1]);
        if ( i4Argc >= 3 )
        {
            i4Loops = (INT32)StrToInt(szArgv[2]);
        }
        if ( i4Argc >= 4 )
        {
            u4Mask = (INT32)StrToInt(szArgv[3]);
        }
        if ( i4Argc >= 5 )
        {
            u4Mode = (INT32)StrToInt(szArgv[4]);
        }
    }
    else
    {
        LOG(0, "Usage: %s RegAddr [Count][Mask][0:RW,1:R Only]\n", szArgv[0]);
        return 0;
    }

    if ( u4Mode == 0 )  // R/W
    {
        Printf("R/W Test\n");
        for ( i = 0 ; i < i4Loops ; i++ )
        {
            _PcmciaRegRwTest(u4Addr, u4Mask);
        }
    }
    else    // Read Only
    {
        Printf("R Only Test\n");
        u4OrgValue = PCMCIA_RawRead32(u4Addr);
        for ( i = 0 ; i < i4Loops ; i++ )
        {
            u4TempValue = PCMCIA_RawRead32(u4Addr);
            if ( u4OrgValue != u4TempValue )
            {
                Printf("Test Failed! Org:0x%X,, Read:0x%X\n", u4OrgValue, u4TempValue);
            }
        }
    }
    
    Printf("Test Done\n");
    return 0;
}

static INT32 PcmciaCmdRegTest (INT32 i4Argc, const CHAR ** szArgv)
{
    INT32 i, i4Loops;
    
    if(i4Argc == 2)
    {
        i4Loops = (INT32)StrToInt(szArgv[1]);
    }
    else if ( i4Argc == 1 )
    {
        i4Loops = 1;
    }
    else
    {
        LOG(0, "Usage: %s <loops_count>\n", szArgv[0]);
        return 0;
    }

    if ( u4InterfaceType == PCMCIA_IF_TYPE_NFI )
    {
        PCMCIA_SetIfType(PCMCIA_IF_TYPE_NFI);
        NFI_Init();
    }
    else
    {
        PCMCIA_SetIfType(PCMCIA_IF_TYPE_SPI);
        SPI_Init();
    }

    for(i=0; i<i4Loops; i++)
    {
        LOG(0, "POD/CI Register Test ... %d\n", i);
        PCMCIA_ResetPcmciaHw();

        if ( i4Loops < 10 )
        {        
            LOG(0, "    (1-a) Register Default Values Test : \n");
        }
        _PcmciaCmdRegDefaultValueTest();

        if ( i4Loops < 10 )
        {
            LOG(0, "    (1-b) Register RW Test : \n");
        }
        _PcmciaCmdRwRegTest();

        if ( i4Loops < 10 )
        {
            LOG(0, "    (1-c) Register Non-map Test : \n");
        }
        _PcmciaCmdRegNoMapTest();
    }

    LOG(0, "PcmciaCmdRegTest Done\n");

    PCMCIA_ResetPcmciaHw();

    return 0;
}

static INT32 PcmciaCmdMpodInitTs(INT32 i4Argc, const CHAR** szArgv)
{
    EXTERN INT32 MPOD_InitTs(INT32 i4Mode);
    if (i4Argc != 2)
    {
        LOG(0, "ts.i mode\n");
        return -1;
    }

    MPOD_InitTs((INT32)StrToInt(szArgv[1]));
    return 1;
}

static INT32 PcmciaCmdMpodPrintInfo(INT32 i4Argc, const CHAR** szArgv)
{
    EXTERN INT32 MPOD_PrintInfo(void);
    MPOD_PrintInfo();
    return 1;
}

static INT32 PcmciaCmdMpodInit(INT32 i4Argc, const CHAR** szArgv)
{
    EXTERN INT32 MPOD_InitM(void);
    EXTERN INT32 MPOD_InitS(void);
    if (i4Argc != 2)
    {
        LOG(0, "i [m|s]\n");
        return -1;
    }
    if ( szArgv[1][0] == 'm' )
    {
        MPOD_InitM();
    }
    else if ( szArgv[1][0] == 's' )
    {
        MPOD_InitS();
    }
    else
    {
        LOG(0, "i [m|s]\n");
        return -1;
    }

    return 1;
}

static INT32 PcmciaCmdMpodRead(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8* u1Ptr=0;
    UINT32 u4Len=0;
    UINT32 i;
    EXTERN INT32 MPOD_Read(UINT8* u1Ptr, UINT32 u4Len);

    if (i4Argc != 2)
    {
        LOG(0, "r [num of byte]\n");
        return -1;
    }
    u4Len = StrToInt(szArgv[1]);


    u1Ptr = (UINT8*)x_mem_alloc(4096);
    if ( u1Ptr == 0 )
    {
        LOG(0, "Can't allocate memory !\n");
        return -1;
    }
    x_memset(u1Ptr, 0, 4096);

    MPOD_Read(u1Ptr, u4Len);

    LOG(0, "\n");
    for ( i = 0 ; (i < u4Len) && (i<32) ; i+=4 )
    {
        LOG(0, " 0x%X 0x%X 0x%X 0x%X \n", u1Ptr[i], u1Ptr[i+1], u1Ptr[i+2], u1Ptr[i+3]);
    }
    LOG(0, "\n");

    x_mem_free(u1Ptr);
    return 0;
}

static INT32 PcmciaCmdMpodWrite(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4Len=0;
    UINT8* u1Ptr=0;
    EXTERN INT32 MPOD_Write(UINT8* u1Ptr, UINT32 u4Len);

    if (i4Argc != 3)
    {
        LOG(0, "w [num of byte] [0xXXXX]\n");
        return -1;
    }
    u1Ptr = (UINT8*)x_mem_alloc(4096);
    if ( u1Ptr == 0 )
    {
        LOG(0, "Can't allocate memory !\n");
        return -1;
    }
    x_memset(u1Ptr, 0, 4096);

    u4Len = StrToInt(szArgv[1]);
    _PcmciaMpodStrToBinary((UINT8*)szArgv[2], u1Ptr, u4Len);

    MPOD_Write(u1Ptr, u4Len);

    x_mem_free(u1Ptr);
    return 0;
}

static INT32 PcmciaCmdMpodOpenASession(INT32 i4Argc, const CHAR** szArgv)
{
    UINT8 u1Ptr[16]= {0};//[4096]={0}; for fix warning  the frame size of 4096 bytes is larger than 1024 bytes
    UINT8 u1ResourceId0,u1ResourceId1,u1ResourceId2,u1ResourceId3;
    static UINT8 u1SessionNb=1;
    EXTERN INT32 MPOD_Read(UINT8* u1Ptr, UINT32 u4Len);
    EXTERN INT32 MPOD_Write(UINT8* u1Ptr, UINT32 u4Len);

    MPOD_Read(u1Ptr, 16);
    LOG(0, "open_session_request: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X \n", u1Ptr[0], u1Ptr[1], u1Ptr[2], u1Ptr[3], u1Ptr[4], u1Ptr[5]);

    u1SessionNb++;
    u1ResourceId0 = u1Ptr[2];
    u1ResourceId1 = u1Ptr[3];
    u1ResourceId2 = u1Ptr[4];
    u1ResourceId3 = u1Ptr[5];
    u1Ptr[0] = 0x92;
    u1Ptr[1] = 0x07;
    u1Ptr[2] = 0x00;
    u1Ptr[3] = u1ResourceId0;
    u1Ptr[4] = u1ResourceId1;
    u1Ptr[5] = u1ResourceId2;
    u1Ptr[6] = u1ResourceId3;
    u1Ptr[7] = 0x00;
    u1Ptr[8] = u1SessionNb;
    MPOD_Write(u1Ptr, 9);
    LOG(0, "open_session_response: 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X\n", u1Ptr[0], u1Ptr[1], u1Ptr[2], u1Ptr[3], u1Ptr[4], u1Ptr[5], u1Ptr[6], u1Ptr[7], u1Ptr[8]);

    return 0;
}

static INT32 PcmciaCmdMpodStress(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4Count,i;
    UINT8 u1Ptr1[32];
    UINT8 u1Ptr2[32];
    EXTERN INT32 MPOD_InitM(void);
    EXTERN INT32 MPOD_Read(UINT8* u1Ptr, UINT32 u4Len);
    EXTERN INT32 MPOD_Write(UINT8* u1Ptr, UINT32 u4Len);
    
    if (i4Argc != 2)
    {
        LOG(0, "stress [test count]\n");
        return -1;
    }

    u4Count = StrToInt(szArgv[1]);

    for ( i = 0 ; i < u4Count ; i++ )
    {
        LOG(0, "=================================Iteration %d\n", i);
        x_thread_delay(10);
        if ( MPOD_InitM() == 0 )
        {
            u1Ptr1[0] = 0x90;
            u1Ptr1[1] = 0x02;
            u1Ptr1[2] = 0x00;
            u1Ptr1[3] = 0x01;
            u1Ptr1[4] = 0x9f;
            u1Ptr1[5] = 0x80;
            u1Ptr1[6] = 0x10;
            u1Ptr1[7] = 0x00;
            if ( MPOD_Write(u1Ptr1, 8) != 0 )  // profile_inq
            {
                goto _MPODStress_FAILED;
            }
            if ( MPOD_Read(u1Ptr2,16) != 0 )   // profile_reply
            {
                goto _MPODStress_FAILED;
            }

            u1Ptr1[6] = 0x12;
            if ( MPOD_Write(u1Ptr1, 8) != 0 )  // profile_changed
            {
                goto _MPODStress_FAILED;
            }
            if ( MPOD_Read(u1Ptr2,16) != 0 )   // profile_inq
            {
                goto _MPODStress_FAILED;
            }

            u1Ptr1[6] = 0x11;
            u1Ptr1[7] = 0x38;
            if ( MPOD_Write(u1Ptr1, 8) != 0 )  // profile_reply
            {
                goto _MPODStress_FAILED;
            }
        }
        else
        {
            goto _MPODStress_FAILED;
        }
    }

    LOG(0, "MPOD Stress Test for %d times OK.\n", u4Count);
    return 0;

_MPODStress_FAILED:
    LOG(0, "MPOD Stress Test for %d times failed at %d.\n", u4Count, i);
    LOG(0, "REG_MPOD_WRITE_CMD : 0x%X\n", PCMCIA_RawRead32(REG_MPOD_WRITE_CMD));
    LOG(0, "REG_MPOD_READ_CMD : 0x%X\n", PCMCIA_RawRead32(REG_MPOD_READ_CMD));
    LOG(0, "REG_MPOD_DEV_CHANNEL_STATUS : 0x%X\n", PCMCIA_RawRead32(REG_MPOD_DEV_CHANNEL_STATUS));
    LOG(0, "REG_MPOD_INTR_STATUS_REG : 0x%X\n", PCMCIA_RawRead32(REG_MPOD_INTR_STATUS_REG));
    return -1;
}

static INT32 PcmciaCmdMpodCkgenStress(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4Count, i, j, u4Reg, u4Reg1, u4Reg2, u4Reg3, u4Read;
    UINT32 u4ErrCount;

    if (i4Argc != 2)
    {
        LOG(0, "ck_stress [test count]\n");
        return -1;
    }

    u4Count = StrToInt(szArgv[1]);
    u4ErrCount = 0;
    for ( i = 0 ; i < u4Count ; i++ )
    {
        //LOG(0, "=================================Iteration %d\n", i);

        for (j = 0; j < 2; j++)
        {
            if (j == 0)
            {
                u4Reg = 0xFFFF0000;
            }
            else
            {
                u4Reg = 0x0000FFFF;
            }
            PCMCIA_RawWrite32(0xB060, u4Reg);
            u4Read = PCMCIA_RawRead32(0xB060);
            if (u4Read != u4Reg)
            {
                u4ErrCount++;
            }
        }
    }
    if (u4ErrCount)
        LOG(0, "Total error count = %d\n", u4ErrCount);

#if 1
    for ( i = 0 ; i < u4Count ; i++ )
    {
        //LOG(0, "=================================Iteration %d\n", i);

        for (j = 0; j < 0xFF; j++)
        {
            u4Reg = j << 16;
            PCMCIA_RawWrite32(0xB03C, u4Reg);
            u4Reg = PCMCIA_RawRead32(0xB03C);
            if (u4Reg != (j << 16))
            {
                LOG(0, "(i, j): (%d, %d), Ckgen register 0xB03C stress test fail, expected = 0x%x, read = 0x%x\n", i, j, j << 16, u4Reg);
            }
        }
    }

    for ( i = 0 ; i < u4Count ; i++ )
    {
        //LOG(0, "=================================Iteration %d\n", i);

        for (j = 0; j < 0xFF; j++)
        {
            u4Reg = j << 16;
            PCMCIA_RawWrite32(0xB008, u4Reg);
            u4Reg = PCMCIA_RawRead32(0xB008);
            if (u4Reg != (j << 16))
            {
                LOG(0, "(i, j): (%d, %d), Ckgen register 0xB008 stress test fail, expected = 0x%x, read = 0x%x\n", i, j, j << 16, u4Reg);
            }
        }
    }

    for ( i = 0 ; i < u4Count ; i++ )
    {
        //LOG(0, "=================================Iteration %d\n", i);

        for (j = 0; j < 0xFF; j++)
        {
            u4Reg = j;
            PCMCIA_RawWrite32(0x011c, u4Reg);
            u4Reg = PCMCIA_RawRead32(0x011c);
            if (u4Reg != j)
            {
                LOG(0, "(i, j): (%d, %d), MPOD register 0x011c stress test fail, expected = 0x%x, read = 0x%x\n", i, j, j, u4Reg);
            }
        }
    }

    for ( i = 0 ; i < u4Count ; i++ )
    {
        //LOG(0, "=================================Iteration %d\n", i);

        for (j = 0; j < 0xFF; j++)
        {
            u4Reg = j;
            PCMCIA_RawWrite32(0x0440, u4Reg);
            u4Reg = PCMCIA_RawRead32(0x0440);
            if (u4Reg != j)
            {
                LOG(0, "(i, j): (%d, %d), MMUX register 0x0440 stress test fail, expected = 0x%x, read = 0x%x\n", i, j, j, u4Reg);
            }
        }
    }

    for ( i = 0 ; i < u4Count ; i++ )
    {
        //LOG(0, "MMUX/CKGEN/Wrapper =================================Iteration %d\n", i);

        for (j = 0; j < 0xFF; j++)
        {
            u4Reg = j;
            u4Reg1 = j << 16;
            u4Reg2 = j;
            u4Reg3 = j;
            PCMCIA_RawWrite32(0x0440, u4Reg);
            PCMCIA_RawWrite32(0xB03C, u4Reg1);
            PCMCIA_RawWrite32(0x7C08, u4Reg2);
            PCMCIA_RawWrite32(0xB060, u4Reg3);
            u4Reg = PCMCIA_RawRead32(0x0440);
            u4Reg1 = PCMCIA_RawRead32(0xB03C);
            u4Reg2 = PCMCIA_RawRead32(0x7C08);
            u4Reg3 = PCMCIA_RawRead32(0xB060);
            if (u4Reg != j)
            {
                LOG(0, "(i, j): (%d, %d), MMUX register 0x0440 stress test fail, expected = 0x%x, read = 0x%x\n", i, j, j, u4Reg);
            }
            if (u4Reg1 != (j << 16))
            {
                LOG(0, "(i, j): (%d, %d), CKGEN register 0xB03C stress test fail, expected = 0x%x, read = 0x%x\n", i, j, j << 16, u4Reg1);
            }
            if (u4Reg2 != j)
            {
                LOG(0, "(i, j): (%d, %d), Wrapper register 0x7C08 stress test fail, expected = 0x%x, read = 0x%x\n", i, j, j, u4Reg2);
            }
            if (u4Reg3 != j)
            {
                LOG(0, "(i, j): (%d, %d), CKGEN register 0xB060 stress test fail, expected = 0x%x, read = 0x%x\n", i, j, j, u4Reg3);
            }
        }
    }
#endif

    //LOG(0, "Ckgen Register Stress Test for %d times OK.\n", u4Count);
    return 0;
}

static INT32 PcmciaCmdGetCis(INT32 i4Argc, const CHAR** szArgv)
{
    INT32  i4Return = 0;
    UINT8* pu1CurCisMap;
    UINT32 i;
    UINT32 u4TimeCount = 0 ;
    UINT32 u4TimeOut = 500;
    extern BOOL fgPcmciaIsInitedByCli;

    fgPcmciaIsInitedByCli = TRUE;
    PCMCIA_Init();

    while( (PCMCIA_DetectCard()!=1) )
    {
        if ( ++u4TimeCount > u4TimeOut )
        {
            LOG(0, "Please insert card\n");
            return -1;
        }
        x_thread_delay(1);
    }

    while( (PCMCIA_TestCardReady()!=1) )
    {
        if ( ++u4TimeCount > u4TimeOut )
        {
            LOG(0, "Ready pin is low... (card not reset?) \n");
            return -2;
        }
        x_thread_delay(1);
    }

    PCMCIA_PcmciaToPod( FALSE );       //change to PCMCIA mode (attribute memory)

    VERIFY( NULL != (pu1CurCisMap=x_mem_alloc(MAX_CIS_MAP_SIZE)) );
    if ( pu1CurCisMap == NULL )
    {
        return -1;
    }
    x_memset((void*)pu1CurCisMap, 0, MAX_CIS_MAP_SIZE);
    for( i = 0 ; i < MAX_CIS_MAP_SIZE ; i ++)
    {
        pu1CurCisMap[i] = (UINT8)PCMCIA_ReadCor((UINT16)(i*2));          //read even bytes
    }
    _PcmciaDump("CIS map", pu1CurCisMap, MAX_CIS_MAP_SIZE);

    {
        UINT16 addrcor;
        UINT8 cor;
        PCMCIA_CheckCIS(pu1CurCisMap, &addrcor, &cor);
        LOG(3, "cor:0x%x, val:0x%x\n", addrcor, cor);
    }

    x_memcpy(au1CisMap, pu1CurCisMap , MAX_CIS_MAP_SIZE );

    for (i = 0; i < MAX_CIS_MAP_SIZE-1; i++)
    {
        if (au1CisMap[i] != au1DiableCisMap[i])
        {
            LOG(1, "======Diable CIS map compare fail============\n");
            LOG(1, "index:0x%x, read:0x%x, org:0x%x\n", i, au1CisMap[i], au1DiableCisMap[i]);
            LOG(1, " ");
            break;
        }
    }
    LOG(1, "\n");
    u4CisLen = MAX_CIS_MAP_SIZE;

    _PcmciaParseCis(0, NULL);

    x_mem_free(pu1CurCisMap) ;
    return i4Return;
}

static INT32 PcmciaCmdWndTest (INT32 i4Argc, const CHAR ** szArgv)
{
    LOG(0, "Memory Window Test ... \n");
    LOG(0, "This test should be done using SysBuild. \n");
    
    LOG(0, "Set window to invalid ==========================================\n");
    _PcmciaConfigBadWins(TRUE);
    PcmciaCmdGetCis(0, NULL);
    LOG(0, "Set window to valid ============================================\n");
    _PcmciaConfigBadWins(FALSE);
    PcmciaCmdGetCis(0, NULL);

    LOG(0, "Test Done. Please compare the above two CISs.\n");

    return 0;
}

static INT32 PcmciaCmdPBusTest(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4RegValue;
    u4RegValue = PCMCIA_RawRead32(REG_PBUS_STS);
    if ( u4RegValue & 0x00002000 )
    {
        LOG(0, "VS2 is High\n");
    }
    else
    {
        LOG(0, "VS2 is Low\n");
    }

    if ( u4RegValue & 0x00001000 )
    {
        LOG(0, "VS1 is High\n");
    }
    else
    {
        LOG(0, "VS1 is Low\n");
    }

    if ( u4RegValue & 0x00000800 )
    {
        LOG(0, "WP is High\n");
    }
    else
    {
        LOG(0, "WP is Low\n");
    }

    if ( u4RegValue & 0x00000400 )
    {
        LOG(0, "CD2 is High\n");
    }
    else
    {
        LOG(0, "CD2 is Low\n");
    }

    if ( u4RegValue & 0x00000200 )
    {
        LOG(0, "CD1 is High\n");
    }
    else
    {
        LOG(0, "CD1 is Low\n");
    }
    
    if ( u4RegValue & 0x00000100 )
    {
        LOG(0, "IREQ is High\n");
    }
    else
    {
        LOG(0, "IREQ is Low\n");
    }

    return 0;
}

static INT32 PcmciaCmdGpioTest(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4RegValue, u4RegTmpValue;
    UINT32 i;
    BOOL bTestOK = TRUE;

    PCMCIA_ResetPcmciaHw();

    if ( u4InterfaceType == PCMCIA_IF_TYPE_NFI )
    {
        PCMCIA_SetIfType(PCMCIA_IF_TYPE_NFI);
        NFI_Init();
    }
    else
    {
        PCMCIA_SetIfType(PCMCIA_IF_TYPE_SPI);
        SPI_Init();
    }
    
    LOG(0, "Set all GPIO pad pull-down.\n");
    PCMCIA_RawWrite32(REG_POD_GPR_CNTL_SET2, 0x8208090);
    LOG(0, "Set all GPIO as GPI.\n");
    PCMCIA_RawWrite32(REG_POD_GPR_CNTL_SET1, 0x0);
    LOG(0, "Sense GPI status:\n");
    u4RegValue = PCMCIA_RawRead32(REG_POD_GPR_CNTL_SET4);
    bTestOK = TRUE;
    for ( i = 0 ; i < 15 ; i++ )
    {
        u4RegTmpValue = u4RegValue & (1<<i);
        if ( u4RegTmpValue != 0 )
        {
            LOG(0, "===>GPI %d is high, it maybe connected with a pull-high resister!\n", i);
            bTestOK = FALSE;
        }
    }
    if ( bTestOK )
    {
        LOG(0, "Sense GPI status: OK\n");
    }
    LOG(0, "Set all GPIO as GPO and value 1.\n");
    PCMCIA_RawWrite32(REG_POD_GPR_CNTL_SET1, 0x3fffffff);
    LOG(0, "    Please check GPIOs real voltage, they should be high ......\n");
    x_thread_delay(10000);
    LOG(0, "Sense GPI status:\n");
    u4RegValue = PCMCIA_RawRead32(REG_POD_GPR_CNTL_SET4);
    bTestOK = TRUE;
    for ( i = 0 ; i < 15 ; i++ )
    {
        u4RegTmpValue = u4RegValue & (1<<i);
        if ( u4RegTmpValue == 0 )
        {
            LOG(0, "!!!!GPI %d is low, it maybe a problem!\n", i);
            bTestOK = FALSE;
        }
    }
    if ( bTestOK )
    {
        LOG(0, "Sense GPI status: OK\n");
    }
    //--------------------------------------------------------------------------
    LOG(0, "\n");
    LOG(0, "Set all GPIO pad pull-high.\n");
    PCMCIA_RawWrite32(REG_POD_GPR_CNTL_SET2, 0x8208088);
    LOG(0, "Set all GPIO as GPI.\n");
    PCMCIA_RawWrite32(REG_POD_GPR_CNTL_SET1, 0x0);
    LOG(0, "Sense GPI status:\n");
    u4RegValue = PCMCIA_RawRead32(REG_POD_GPR_CNTL_SET4);
    bTestOK = TRUE;
    for ( i = 0 ; i < 15 ; i++ )
    {
        u4RegTmpValue = u4RegValue & (1<<i);
        if ( u4RegTmpValue == 0 )
        {
            LOG(0, "===>GPI %d is low, it maybe connected with a pull-down resister!\n", i);
            bTestOK = FALSE;
        }
    }
    if ( bTestOK )
    {
        LOG(0, "Sense GPI status: OK\n");
    }
    LOG(0, "Set all GPIO as GPO and value 0.\n");
    PCMCIA_RawWrite32(REG_POD_GPR_CNTL_SET1, 0x3fff8000);
    LOG(0, "    Please check GPIOs real voltage, they should be low ........\n");
    x_thread_delay(10000);
    LOG(0, "Sense GPI status:\n");
    u4RegValue = PCMCIA_RawRead32(REG_POD_GPR_CNTL_SET4);
    bTestOK = TRUE;
    for ( i = 0 ; i < 15 ; i++ )
    {
        u4RegTmpValue = u4RegValue & (1<<i);
        if ( u4RegTmpValue != 0 )
        {
            LOG(0, "!!!!GPI %d is high, it maybe a problem!\n", i);
            bTestOK = FALSE;
        }
    }
    if ( bTestOK )
    {
        LOG(0, "Sense GPI status: OK\n");
    }

    LOG(0, "GPIO Test Done.\n");
    return 0;
}

static INT32 PcmciaCmdPllTest(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4Frequency, u4FastWindow;
    if(i4Argc != 3)
    {
        LOG(0, "This cli should be done using SysBuild & real TS\n");
        LOG(0, "pll [Frequency:63,72,81] [0:600ns, 1:300ns]\n");
        return 0;
    }
    else
    {
        u4Frequency = StrToInt(szArgv[1]);
        u4FastWindow = StrToInt(szArgv[2]);
    }

    if ( u4Frequency == 63 )
    {
        LOG(0, "Set PLL to %d Mhz\n", u4Frequency);
        PCMCIA_RawWrite32(REG_PLL_CNTL_SET0, 0x0890606F);
        PCMCIA_RawWrite32(REG_PLL_CNTL_SET1, 0x80002800);
        
        if ( u4FastWindow == 0 )
        {
            LOG(0, "Set Memory Timing : Slow, 600ns\n");
            PCMCIA_RawWrite32(REG_MEM_TIMING0, 0x080E1C1C);
            PCMCIA_RawWrite32(REG_MEM_TIMING1, 0x222A0000);
            PCMCIA_RawWrite32(REG_IO_TIMING0, 0x0C021A22);
            PCMCIA_RawWrite32(REG_IO_TIMING1, 0x28260000);
        }
        else
        {
            LOG(0, "Set I/O Timing : Fast, 300ns\n");
            PCMCIA_RawWrite32(REG_MEM_TIMING0, 0x04070E0E);
            PCMCIA_RawWrite32(REG_MEM_TIMING1, 0x11150000);
            PCMCIA_RawWrite32(REG_IO_TIMING0, 0x06010D11);
            PCMCIA_RawWrite32(REG_IO_TIMING1, 0x14130000);
        }
    }
    else if ( u4Frequency == 72 )
    {
        LOG(0, "Set PLL to %d Mhz\n", u4Frequency);
        PCMCIA_RawWrite32(REG_PLL_CNTL_SET0, 0x089070FF);
        PCMCIA_RawWrite32(REG_PLL_CNTL_SET1, 0x80002800);
        
        if ( u4FastWindow == 0 )
        {
            LOG(0, "Set Memory Timing : Slow, 600ns\n");
            PCMCIA_RawWrite32(REG_MEM_TIMING0, 0x08102020);
            PCMCIA_RawWrite32(REG_MEM_TIMING1, 0x262E0000);
            PCMCIA_RawWrite32(REG_IO_TIMING0, 0x0E021E28);
            PCMCIA_RawWrite32(REG_IO_TIMING1, 0x2E2C0000);
        }
        else
        {
            LOG(0, "Set I/O Timing : Fast, 300ns\n");
            PCMCIA_RawWrite32(REG_MEM_TIMING0, 0x04081010);
            PCMCIA_RawWrite32(REG_MEM_TIMING1, 0x13170000);
            PCMCIA_RawWrite32(REG_IO_TIMING0, 0x07010F14);
            PCMCIA_RawWrite32(REG_IO_TIMING1, 0x17160000);
        }
    }
    else if ( u4Frequency == 81 )
    {
        LOG(0, "Set PLL to %d Mhz\n", u4Frequency);
        PCMCIA_RawWrite32(REG_PLL_CNTL_SET0, 0x0890506F);
        PCMCIA_RawWrite32(REG_PLL_CNTL_SET1, 0x80001400);
        
        if ( u4FastWindow == 0 )
        {
            LOG(0, "Set Memory Timing : Slow, 600ns\n");
            PCMCIA_RawWrite32(REG_MEM_TIMING0, 0x08122222);
            PCMCIA_RawWrite32(REG_MEM_TIMING1, 0x28340000);
            PCMCIA_RawWrite32(REG_IO_TIMING0, 0x1004222C);
            PCMCIA_RawWrite32(REG_IO_TIMING1, 0x32320000);
        }
        else
        {
            LOG(0, "Set I/O Timing : Fast, 300ns\n");
            PCMCIA_RawWrite32(REG_MEM_TIMING0, 0x04091111);
            PCMCIA_RawWrite32(REG_MEM_TIMING1, 0x141A0000);
            PCMCIA_RawWrite32(REG_IO_TIMING0, 0x08021116);
            PCMCIA_RawWrite32(REG_IO_TIMING1, 0x19190000);
        }
    }
    LOG(0, "Done. Check the following :\n");
    LOG(0, "    1. Keep CI CAM connected and check display screen. \n");
    LOG(0, "    2. Use scope to check 8295_CLK_OUT frequency.\n");
    return 0;
}

static INT32 PcmciaCmdSetChipType(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4TypeValue;
    if(i4Argc != 2)
    {
        LOG(0, "chipt [0:EXTERNAL, 1:INTERNAL]\n");
        if ( ePcmciaChipType == PCMCIA_CHIP_TYPE_EXTERNAL )
        {
            LOG(0, "    Current Setting is EXTERNAL\n");
        }
        else
        {
            LOG(0, "    Current Setting is INTERNAL\n");
        }
        return 0;
    }
    else
    {
        u4TypeValue = StrToInt(szArgv[1]);
    }

    if ( u4TypeValue == 0 )
    {
        ePcmciaChipType = PCMCIA_CHIP_TYPE_EXTERNAL;
        LOG(0, "Set to External Mode\n");
    }
    else
    {
        ePcmciaChipType = PCMCIA_CHIP_TYPE_INTERNAL;
        LOG(0, "Set to Internal Mode\n");
    }

    return 0;
}

static INT32 PcmciaCmdSetCardType(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4TypeValue;
    if(i4Argc != 2)
    {
        LOG(0, "cardt [0:CI, 1:POD]\n");
        if ( ePcmciaCardType == PCMCIA_CARD_TYPE_CI )
        {
            LOG(0, "    Current Setting is CI\n");
        }
        else
        {
            LOG(0, "    Current Setting is POD\n");
        }
        return 0;
    }
    else
    {
        u4TypeValue = StrToInt(szArgv[1]);
    }

    if ( u4TypeValue == 0 )
    {
        ePcmciaCardType = PCMCIA_CARD_TYPE_CI;
        LOG(0, "Set to CI Mode\n");
    }
    else
    {
        ePcmciaCardType = PCMCIA_CARD_TYPE_POD;
        LOG(0, "Set to POD Mode\n");
    }

    return 0;
}

static INT32 PcmciaCmdSetParallelTs(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4TypeValue, u4RegValue;

    if ( i4Argc == 1 )
    {
        u4RegValue = (PCMCIA_RawRead32(REG_POD_GPR_CNTL_SET0) & 0x40000000);
        if ( u4RegValue != 0 )
        {
            LOG(0, "Current Setting : Parallel TS \n");
        }
        else
        {
            LOG(0, "Current Setting : Serial TS \n");
        }
        return 0;
    }
    else if(i4Argc > 2)
    {
        LOG(0, "pts [0:OFF, 1:ON]\n");
        return 0;
    }
    else
    {
        u4TypeValue = StrToInt(szArgv[1]);
    }

    if ( u4TypeValue == 0 )
    {
        PCMCIA_ParallelTsOnOff(FALSE);
        LOG(0, "Set Parallel TS Off\n");
    }
    else
    {
        PCMCIA_ParallelTsOnOff(TRUE);
        LOG(0, "Set Parallel TS On\n");
    }

    return 0;
}

static INT32 PcmciaCmdSetExtDemod(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4TypeValue, u4RegValue;

    if ( i4Argc == 1 )
    {
        u4RegValue = (PCMCIA_RawRead32(REG_POD_GPR_CNTL_SET3) & 0x00004000);
        if ( u4RegValue != 0 )
        {
            LOG(0, "Current Setting : Use External Demod\n");
        }
        else
        {
            LOG(0, "Current Setting : Use Internal Demod\n");
        }
        return 0;
    }
    else if(i4Argc > 2)
    {
        LOG(0, "sed [0:OFF, 1:ON]\n");
        return 0;
    }
    else
    {
        u4TypeValue = StrToInt(szArgv[1]);
    }

    if ( u4TypeValue == 0 )
    {
        PCMCIA_ExternalDemodOnOff(FALSE);
        LOG(0, "Set External Demod Off\n");
    }
    else
    {
        PCMCIA_ExternalDemodOnOff(TRUE);
        LOG(0, "Set External Demod On\n");
    }

    return 0;    
}

static INT32 PcmciaCmdThreadNoAssert(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4ThreadNoAssert;
    if(i4Argc != 2)
    {
        LOG(0, "tda [0:Ast, 1:NoAst]\n");
        return 0;
    }
    else
    {
        u4ThreadNoAssert = StrToInt(szArgv[1]);
    }

    if ( u4ThreadNoAssert == 0 )
    {
        PCMCIA_ThreadNoAssert(FALSE);
    }
    else
    {
        PCMCIA_ThreadNoAssert(TRUE);        
    }
    
    return 0;
}

static INT32 PcmciaCmdStopThread(INT32 i4Argc, const CHAR** szArgv)
{
    PCMCIA_StopThread();
    LOG(0, "Note! This is not recoverable !\n");
    return 0;
}

static INT32 PcmciaCmdSetIfType(INT32 i4Argc, const CHAR** szArgv)
{
    if(i4Argc != 2)
    {
        LOG(0, "sit [0:NFI, 1:SPI]\n");
        return 0;
    }
    else
    {
        u4InterfaceType = (PCMCIA_IF_TYPE_T)StrToInt(szArgv[1]);
    }

    return 0;    
}

static INT32 PcmciaCmdNfiInit(INT32 i4Argc, const CHAR** szArgv)
{
    PCMCIA_SetIfType(PCMCIA_IF_TYPE_NFI);
    NFI_Init();
    return POD_OK;
}

static INT32 PcmciaCmdNfiSWrite(INT32 i4Argc, const CHAR** szArgv)
{
    INT32   i4Return = POD_OK;
    UINT32   u4Data;
    UINT32  u4Addr;

    if (i4Argc != 3)
    {
        LOG(0, "sw addr data\n");
        return -1;
    }
    u4Addr = StrToInt(szArgv[1]) / 4;
    u4Data = StrToInt(szArgv[2]);

    i4Return = NFI_SDAWrite(u4Addr, u4Data);

    return i4Return;
}

static INT32 PcmciaCmdNfiSRead(INT32 i4Argc, const CHAR** szArgv)
{
    INT32  i4Return = POD_OK;
    UINT32 u4Data=0;
    UINT32 u4Addr;
    UINT32 u4Count = 1;
    UINT32 i;

    if ((i4Argc != 2) && (i4Argc != 3))
    {
        LOG(0, "sr addr [count]\n");
        return -1;
    }
    if (i4Argc == 3)
    {
        u4Count = StrToInt(szArgv[2]);
        if (u4Count > 32)
        {
            u4Count = 32;
        }
    }

    u4Addr = StrToInt(szArgv[1]) / 4;
    for (i = 0; i < u4Count; i++)
    {
        i4Return = NFI_SDARead(u4Addr, &u4Data);
        LOG(0, "SDA read@0x%x=0x%x\n", u4Addr * 4, u4Data);
        u4Addr++;
    }
    return i4Return;
}

static INT32 PcmciaCmdNfiBWrite(INT32 i4Argc, const CHAR** szArgv)
{
    INT32   i4Return = POD_OK;
    UINT32  *pu4Data;
    UINT32  u4Addr;
    UINT32  u4DataLen;
    UINT32  i;

    if (i4Argc != 4)
    {
        LOG(0, "bw addr databuf len\n");
        return -1;
    }
    u4Addr = StrToInt(szArgv[1]) / 4;
    pu4Data = (UINT32 *)StrToInt(szArgv[2]);
    u4DataLen = StrToInt(szArgv[3]);
    i4Return = NFI_BurstWrite((UINT32)pu4Data, u4Addr, u4DataLen);

    LOG(0, "Burst Write:\n");
    for (i = 0; (i < 4)&& (i < u4DataLen); i++)
    {
        Printf("0x%08x ", pu4Data[i]);
    }
    Printf("\n");

    return i4Return;
}

static INT32 PcmciaCmdNfiBRead(INT32 i4Argc, const CHAR** szArgv)
{
    INT32   i4Return = POD_OK;
    UINT32  *pu4Data;
    UINT32  u4Addr;
    UINT32  u4DataLen;
    UINT32  i;

    if (i4Argc != 4)
    {
        LOG(0, "br addr databuf len\n");
        return -1;
    }
    u4Addr = StrToInt(szArgv[1]) / 4;
    pu4Data = (UINT32 *)StrToInt(szArgv[2]);
    u4DataLen = StrToInt(szArgv[3]);

    for (i = 0; i < u4DataLen; i++)
    {
        pu4Data[i] = 0;
    }

    i4Return = NFI_BurstRead((UINT32)pu4Data, u4Addr, u4DataLen);

    LOG(0, "Burst Read:\n");
    for (i = 0; (i < 4)&& (i < u4DataLen); i++)
    {
        Printf("0x%08x ", pu4Data[i]);
    }
    Printf("\n");

    return i4Return;
}

static INT32 PcmciaCmdResetCam(INT32 i4Argc, const CHAR** szArgv)
{
    PCMCIA_ResetCam();
    return 0;
}

static INT32 PcmciaCmdCreateTC(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4Loop=1, i;
    INT32 i4Result = -1;
    if( i4Argc < 2)
    {
        LOG(0, "Usage: [loop=1]\n");
    }
    else
    {
        u4Loop = StrToInt( szArgv[1] );
    }

    LOG(0, "loop:%d\n", u4Loop);
    for( i = 0 ; i < u4Loop ;  i++ )
    {
        PcmciaCmdInit(0, NULL);
        LOG(0, "Loop : %d , ***********************************\n", i);
        if( PCMCIA_DetectCard() )
        {
            i4Result = _PcmciaCmdCreateTC(0, NULL);
            if (i4Result != POD_OK)
            {
                LOG(0, "p.v.tc failed (%d)\n", i4Result);
                break;
            }
        }
    }
    return i4Result;
}

static INT32 PcmciaCmdCommonReadReg(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4Data;
    UINT32 u4Addr;
    UINT32 u4Count = 1;
    UINT32 i;

    if ((i4Argc != 2) && (i4Argc != 3))
    {
        LOG(0, "r addr [count]\n");
        return -1;
    }
    if (i4Argc == 3)
    {
        u4Count = StrToInt(szArgv[2]);
    }

    u4Addr = StrToInt(szArgv[1]);
    for (i = 0; i < u4Count; i++)
    {
        u4Data = PCMCIA_RawRead32(u4Addr);
        LOG(0, "0x%x=0x%x\n", u4Addr, u4Data);
        u4Addr+=4;
    }

    return 0;
}

static INT32 PcmciaCmdCommonWriteReg(INT32 i4Argc, const CHAR** szArgv)
{
    INT32   i4Return = POD_OK;
    UINT32   u4Data;
    UINT32  u4Addr;

    if (i4Argc != 3)
    {
        LOG(0, "w addr data\n");
        return -1;
    }
    u4Addr = StrToInt(szArgv[1]);
    u4Data = StrToInt(szArgv[2]);

    i4Return = PCMCIA_RawWrite32(u4Addr, u4Data);

    return i4Return;
}

static INT32 PcmciaCmdSetBypass(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 fgBypass, u4RegValue;
    extern BOOL fgPcmciaIsInitedByCli;

    fgPcmciaIsInitedByCli = TRUE;
    PCMCIA_Init();

    if ( i4Argc == 1 )
    {
        u4RegValue = (PCMCIA_RawRead32(REG_POD_GPR_CNTL_SET0) & 0x80000000);
        if ( u4RegValue != 0 )
        {
            LOG(0, "Current Setting : 8295 Bypass Is On\n");
        }
        else
        {
            LOG(0, "Current Setting : 8295 Bypass Is Off\n");
        }
        return 0;
    }
    else if( i4Argc > 2)
    {
        LOG(0, "Usage: %s 1:Bypass, 0:non-Bypass\n", szArgv[0]);
        return 0;
    }
    else
    {
        fgBypass = StrToInt( szArgv[1] );
    }
    
    if ( fgBypass )    
    {
        PCMCIA_SetBypass(TRUE);
        LOG(0, "8295 bypass on\n");
    }
    else
    {
        PCMCIA_SetBypass(FALSE);
        LOG(0, "8295 bypass off\n");
    }
    return 0;
}

static INT32 PcmciaCmdSetTsType(INT32 i4Argc, const CHAR** szArgv)
{
    if( i4Argc != 2)
    {
        LOG(0, "Usage: %s [0:DVB-T, 0:DVB-C]\n", szArgv[0]);
        return 0;
    }
    else 
    {
        if ( StrToInt(szArgv[1]) == 0 )
        {
            PCMCIA_SetTsType(PCMCIA_TS_TYPE_DVB_T);
        }
        else
        {
            PCMCIA_SetTsType(PCMCIA_TS_TYPE_DVB_C);
        }
    }
    return 0;
}

static INT32 PcmciaCmdGpioEnable(INT32 i4Argc, const CHAR** szArgv)
{
    INT32 i4Polarity, i4Gpio, i4Ret;

    if (i4Argc == 2)
    {
        i4Gpio = StrToInt(szArgv[1]);
        i4Ret = CI_GPIO_Enable(i4Gpio, NULL);
        LOG(0, "GPIO %d enable is %d\n", i4Gpio, i4Ret);
        return 0;
    }

    if (i4Argc == 3)
    {
        i4Gpio = StrToInt(szArgv[1]);
        i4Polarity = StrToInt(szArgv[2]);
        CI_GPIO_Enable(i4Gpio, &i4Polarity);
        return 0;
    }
    
    LOG(0, "Usage: %s [GpioNum:0-30] [0:gpi, 1:gpo, none:query]\n", szArgv[0]);
    return 0;
}

static INT32 PcmciaCmdGpioInput(INT32 i4Argc, const CHAR** szArgv)
{
    INT32 i4Gpio, i4Ret;

    if (i4Argc != 2)
    {
        LOG(0, "Usage: %s [GpioNum:0-30]\n", szArgv[0]);
        return 0;
    }

    i4Gpio = StrToInt(szArgv[1]);
    i4Ret = CI_GPIO_Input(i4Gpio);
    LOG(0, "GPIO %d input value is %d\n", i4Gpio, i4Ret);
    return 0;
}

static INT32 PcmciaCmdGpioOutput(INT32 i4Argc, const CHAR** szArgv)   
{
    INT32 i4Gpio, i4Drive, i4Ret;
    if (i4Argc == 2)
    {
        i4Gpio = StrToInt(szArgv[1]);
        i4Ret = CI_GPIO_Output(i4Gpio, NULL);
        LOG(0, "GPIO %d output value is %d\n", i4Gpio, i4Ret);
        return 0;
    }

    if (i4Argc == 3)
    {
        i4Gpio = StrToInt(szArgv[1]);
        i4Drive = StrToInt(szArgv[2]);
        CI_GPIO_Output(i4Gpio, &i4Drive);
        return 0;
    }

    LOG(0, "Usage: %s [GpioNum:0-30] [0:drive0, 1:drive1, none:query]\n", szArgv[0]);
    return 0;
}

static INT32 PcmciaCmdGpioIntrq(INT32 i4Argc, const CHAR** szArgv)   
{
    INT32 i4Gpio, i4Set, i4Ret;
    
    if (i4Argc == 2)
    {
        i4Gpio = StrToInt(szArgv[1]);
        i4Ret = CI_GPIO_Intrq(i4Gpio, NULL);
        LOG(0, "GPIO %d interrupt enable is %d\n", i4Gpio, i4Ret);
        return 0;
    }

    if (i4Argc == 3)
    {
        i4Gpio = StrToInt(szArgv[1]);
        i4Set = StrToInt(szArgv[2]);
        CI_GPIO_Intrq(i4Gpio, &i4Set);
        return 0;
    }

    LOG(0, "Usage: %s [GpioNum:0-30] [0:drive0, 1:drive1, none:query]\n", szArgv[0]);
    return 0;
}

static INT32 PcmciaCmdSet2Bit(INT32 i4Argc, const CHAR** szArgv)
{
    BOOL fg2Bit;
    if (i4Argc == 1)
    {
        fg2Bit = PCMCIA_Get2Bit();
        if (fg2Bit)
        {
            LOG(0, "2 bit mode\n");
        }
        else
        {
            LOG(0, "1 bit mode\n");
        }
        return 0;
    }
    else if (i4Argc == 2)
    {
        fg2Bit = StrToInt(szArgv[1]);
        PCMCIA_Set2Bit(fg2Bit);
        if (fg2Bit)
        {
            LOG(0, "Set 2 bit mode\n");
        }
        else
        {
            LOG(0, "Set 1 bit mode\n");
        }
        return 0;
    }
    LOG(0, "Usage: %s [fg2Bit]\n", szArgv[0]);
    return 0;
}

static INT32 PcmciaCmdSetDriving(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4Driving = 0;
    
    if( i4Argc != 2)
    {
        LOG(0, "Usage: %s [2/4/6/8] [2:2mA,4:4mA,6:6mA,8:8mA]\n", szArgv[0]);        
    }
    else 
    {
        u4Driving = StrToInt(szArgv[1]);
        PcmciaSetDriving(u4Driving); 
    }    
    return 0;
}

static INT32 PcmciaCmdSetTsClock(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4TSClock = 0;
    
    if( i4Argc != 2)
    {
        LOG(0, "Usage: %s [27/36/48/54/62/72/86/108/144 MHz]\n", szArgv[0]);        
    }
    else 
    {
        u4TSClock = StrToInt(szArgv[1]);
        PcmciaSetTsClock(u4TSClock); 
    }    
    return 0;
}

static INT32 PcmciaCmdSetSysClock(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 u4SysClock = 0;
    
    if( i4Argc != 2)
    {
        LOG(0, "Usage: %s [27/36/48/54/62/72/86/108/144 MHz]\n", szArgv[0]);        
    }
    else 
    {
        u4SysClock = StrToInt(szArgv[1]);
        PcmciaSetSysClock(u4SysClock); 
    }    
    return 0;
}

static INT32 PcmciaCmdEnableTs(INT32 i4Argc, const CHAR** szArgv)
{
    if( i4Argc != 2)
    {
        LOG(0, "Usage: %s [0:Off, 0:On]\n", szArgv[0]);
        return 0;
    }
    else 
    {
        if ( StrToInt(szArgv[1]) == 0 )
        {
            PCMCIA_PcmciaEnableTS(FALSE);
        }
        else
        {
            PCMCIA_PcmciaEnableTS(TRUE);
        }
    }
    return 0;
}

static INT32 PcmciaCmdEnableSlot(INT32 i4Argc, const CHAR** szArgv)
{
    if( i4Argc != 2)
    {
        LOG(0, "Usage: %s [0:Off, 0:On]\n", szArgv[0]);
        return 0;
    }
    else 
    {
        if ( StrToInt(szArgv[1]) == 0 )
        {
            PCMCIA_PcmciaToPod(FALSE);
        }
        else
        {
            PCMCIA_PcmciaToPod(TRUE);
        }
    }
    return 0;
}

static INT32 PcmciaCmdSetTsRoute(INT32 i4Argc, const CHAR** szArgv)
{
    BOOL fgExternalDemod, fgThroughCard;
    if( i4Argc != 3)
    {
        LOG(0, "Usage: %s fgExternalDemod fgThroughCard\n", szArgv[0]);
        return 0;
    }
    else 
    {
        fgExternalDemod = StrToInt(szArgv[1]);
        fgThroughCard = StrToInt(szArgv[2]);
        PCMCIA_SetTsPath(fgExternalDemod, fgThroughCard);
        return POD_OK;
    }
}
#endif

static INT32 PcmciaCmdSpiInit(INT32 i4Argc, const CHAR** szArgv)
{
    PCMCIA_SetIfType(PCMCIA_IF_TYPE_SPI);
    SPI_Init();
    return POD_OK;
}

static INT32 PcmciaCmdSpiReset(INT32 i4Argc, const CHAR** szArgv)
{
    SPI_Reset();
    return POD_OK;
}

static INT32 PcmciaCmdSpiSRead(INT32 i4Argc, const CHAR** szArgv)
{
    INT32  i4Return = POD_OK;
    UINT32 u4Data;
    UINT32 u4Addr;
    UINT32 u4Count = 1;
    UINT32 i;

    if ((i4Argc != 2) && (i4Argc != 3))
	{
        LOG(0, "sr addr [count]\n");
        return -1;
    }
    if (i4Argc == 3)
		{
        u4Count = StrToInt(szArgv[2]);
    }

    u4Addr = StrToInt(szArgv[1]) / 4;
    for (i = 0; i < u4Count; i++)
			{
        i4Return = SPI_SDARead(&u4Data, u4Addr);
        LOG(0, "SDA read@0x%x=0x%x\n", u4Addr * 4, u4Data);
        u4Addr++;
	}

    return i4Return;
}

static INT32 PcmciaCmdSpiSWrite(INT32 i4Argc, const CHAR** szArgv)
{
    INT32   i4Return = POD_OK;
    UINT32   u4Data;
    UINT32  u4Addr;

    if (i4Argc != 3)
    {
        LOG(0, "sw addr data\n");
        return -1;
    }
    u4Addr = StrToInt(szArgv[1]) / 4;
    u4Data = StrToInt(szArgv[2]);

    i4Return = SPI_SDAWrite((const UINT32 *) &u4Data, u4Addr);

    return i4Return;
}

static INT32 PcmciaCmdSpiDRead(INT32 i4Argc, const CHAR** szArgv)
{
    INT32   i4Return = POD_OK;
    UINT32  *pu4Data;
    UINT32  u4Addr;
    UINT32  u4DataLen;
    UINT32  i;

    if (i4Argc != 4)
    {
        LOG(0, "dr addr databuf len\n");
        return -1;
    }
    u4Addr = StrToInt(szArgv[1]) / 4;
    pu4Data = (UINT32 *)StrToInt(szArgv[2]);
    u4DataLen = StrToInt(szArgv[3]);

    for (i = 0; i < u4DataLen; i++)
    {
        pu4Data[i] = 0;
}

    i4Return = SPI_DMARead(pu4Data, u4Addr, u4DataLen);

    LOG(0, "DMA Read:\n");
    for (i = 0; (i < 4)&& (i < u4DataLen); i++)
    {
        Printf("0x%08x ", pu4Data[i]);
    }
    Printf("\n");

    return i4Return;
}

static INT32 PcmciaCmdSpiDWrite(INT32 i4Argc, const CHAR** szArgv)
{
    INT32   i4Return = POD_OK;
    UINT32  *pu4Data;
    UINT32  u4Addr;
    UINT32  u4DataLen;
    UINT32  i;

    if (i4Argc != 4)
{
        LOG(0, "dw addr databuf len\n");
        return -1;
    }
    u4Addr = StrToInt(szArgv[1]) / 4;
    pu4Data = (UINT32 *)StrToInt(szArgv[2]);
    u4DataLen = StrToInt(szArgv[3]);
    i4Return = SPI_DMAWrite( (const UINT32 *) pu4Data, u4Addr, u4DataLen);

    LOG(0, "DMA Write:\n");
    for (i = 0; (i < 4)&& (i < u4DataLen); i++)
    {
        Printf("0x%08x ", pu4Data[i]);
    }
    Printf("\n");

    return i4Return;
}

static INT32 PcmciaCmdSpiDTest(INT32 i4Argc, const CHAR** szArgv)
{
    INT32   i4Return = POD_OK;
    UINT32  u4Addr;
    UINT32  u4DataLen;
    UINT8 *pu4TestBuf, *pu4GoldBuf, *pu4TempBuf;
    UINT32 i, j;
    UINT32 u4TestLoop = 1;
    UINT32 u4VariantLenTest = 0;
    UINT32 v4TestLen=1;
    BOOL   fgErrorOccur = FALSE;


    if (i4Argc == 2)
{
        u4TestLoop = StrToInt(szArgv[1]);
    }
    else if (i4Argc == 3 )
  {
        u4TestLoop = StrToInt(szArgv[1]);
        v4TestLen = StrToInt(szArgv[2]);
  }
    else if (i4Argc == 4 )
  {
        u4TestLoop = StrToInt(szArgv[1]);
        v4TestLen = StrToInt(szArgv[2]);
        u4VariantLenTest = StrToInt(szArgv[3]);
  }
    else
  {
        LOG(0, "dt [loop][len (1~512)][1:variant len test]\n");
        return -1;
  }

    if ( v4TestLen > 1024 )
  {
        LOG(0, "len should be less than or equal to 1024\n");
  }

    // Set FIFO MODE
    PCMCIA_RawWrite32(REG_POD_GPR_CNTL_SET0, ((PCMCIA_RawRead32(REG_POD_GPR_CNTL_SET0)&~FIFO_MODE)|FIFO4FLASH));

    pu4TestBuf = (UINT8*)BSP_AllocAlignedDmaMemory(4096, 32);
    pu4GoldBuf = (UINT8*)BSP_AllocAlignedDmaMemory(4096, 32);
    pu4TempBuf = (UINT8*)BSP_AllocAlignedDmaMemory(4096, 32);
    if ( pu4TestBuf == NULL || pu4GoldBuf == NULL )
  {
        return -1;
  }
#if 0
    // fix length, fix pattern
    for ( i = 1 ; i <= u4TestLoop ; i++ )
  {
  	    if ((i % 5000) == 1)
        {
            Printf("Fix length, fix pattern, Test loop: %d\n", i);
        }
        for( j = 0 ; j < 4096 ; j++ )
  {
            pu4GoldBuf[j] = 0xca;
            pu4TestBuf[j] = 0;
  }

        u4Addr = 0;
        u4DataLen = v4TestLen;
        SPI_DMAWrite((const UINT32 *)pu4GoldBuf, u4Addr, u4DataLen);
        SPI_DMARead((UINT32*)pu4TestBuf, u4Addr, u4DataLen);
        
        for(j=0; j<(u4DataLen*4); j++)
  {
            if ( pu4GoldBuf[j] != pu4TestBuf[j] )
  {
                Printf("Cmp fail (fix pattern), Loop:%d, offset:%d, Predict:0x%x, Read:0x%x\n", i, j, pu4GoldBuf[j], pu4TestBuf[j]);
                fgErrorOccur = TRUE;
  }
  }
        if ( fgErrorOccur == TRUE )
  {
            return 0;
  }
  }
#endif

    // fix length, random pattern
    for ( i = 1 ; i <= u4TestLoop ; i++ )
    {
        if ((i % 5000) == 1)
        {
            Printf("fix length, random pattern, Test loop: %d\n", i);
        }
        if (i > 1)
        {
            x_memcpy(pu4GoldBuf, pu4TempBuf, 4096);
        }
        for( j = 0 ; j < 4096 ; j++ )
        {
            pu4GoldBuf[j] = (UINT32)rand();
            pu4TestBuf[j] = 0;
        }

        u4Addr = 0;
        u4DataLen = v4TestLen;
        SPI_DMAWrite((const UINT32 *)pu4GoldBuf, u4Addr, u4DataLen);
        SPI_DMARead((UINT32*)pu4TestBuf, u4Addr, u4DataLen);
        
        for(j=0; j<(u4DataLen*4); j++)
        {
            if ( pu4GoldBuf[j] != pu4TestBuf[j] )
            {
                Printf("Cmp fail (random pattern), Loop:%d, offset:%d, Predict:0x%x, Read:0x%x, Last:0x%x\n", i, j, pu4GoldBuf[j], pu4TestBuf[j], pu4TempBuf[j]);
                ASSERT(0);
                fgErrorOccur = TRUE;
            }
        }
        if ( fgErrorOccur == TRUE )
        {
            //return 0;
            continue;
        }
    }

    // Variant len
    if ( u4VariantLenTest == 1 )
    {
        Printf("Variant Len Test Start\n");

        for ( i = 1 ; i < v4TestLen ; i++ )
        {
            u4DataLen = i;
            u4Addr = 0;

            srand(REG_RW_TIMER0_LOW);
            for( j = 0 ; j < 4096 ; j++ )
            {
                pu4GoldBuf[j] = (UINT32)rand();
                pu4TestBuf[j] = 0;
            }

            SPI_DMAWrite((const UINT32 *)pu4GoldBuf, u4Addr, u4DataLen);
            SPI_DMARead((UINT32*)pu4TestBuf, u4Addr, u4DataLen);
            
            for(j=0; j<(u4DataLen*4); j++)
      {
                if ( pu4GoldBuf[j] != pu4TestBuf[j] )
        {
                    Printf("Cmp fail (random pattern/length), Loop:%d, offset:%d, Predict:0x%x, Read:0x%x\n", i, j, pu4GoldBuf[j], pu4TestBuf[j]);
                    fgErrorOccur = TRUE;
        }
      }
            
            if ( fgErrorOccur == TRUE )
            {
                return 0;
            }
      }
    }
    
    Printf("Test Done\n");
    BSP_FreeAlignedDmaMemory((UINT32)pu4TestBuf);
    BSP_FreeAlignedDmaMemory((UINT32)pu4GoldBuf);
    BSP_FreeAlignedDmaMemory((UINT32)pu4TempBuf);
    return i4Return;
}


static INT32 PcmciaCmdSpiDTAlloc(INT32 i4Argc, const CHAR** szArgv)
{
    UINT32 pu4TestBuf, pu4GoldBuf;
    pu4TestBuf = BSP_AllocAlignedDmaMemory(4096, 32);
    pu4GoldBuf = BSP_AllocAlignedDmaMemory(4096, 32);
    LOG(0, "pu4TestBuf=0x%x, pu4GoldBuf=0x%x\n", pu4TestBuf, pu4GoldBuf);
    return 0;
}

static INT32 PcmciaCmdSpiDTStart(INT32 i4Argc, const CHAR** szArgv)
{
    INT32   i4Return = POD_OK;
    UINT32  u4Addr;
    UINT32  u4DataLen;
    UINT8 *pu4TestBuf, *pu4GoldBuf;
    UINT32 i, j;
    UINT32 u4TestLoop = 1;
    UINT32 u4VariantLenTest = 0;
    UINT32 v4TestLen=1;
    BOOL   fgErrorOccur = FALSE;

    if (i4Argc == 6 )
  {
  	    pu4TestBuf = (UINT8*)StrToInt(szArgv[1]);
  	    pu4GoldBuf = (UINT8*)StrToInt(szArgv[2]);
        u4TestLoop = StrToInt(szArgv[3]);
        v4TestLen = StrToInt(szArgv[4]);
        u4VariantLenTest = StrToInt(szArgv[5]);
  }
    else
  {
        LOG(0, "dt u4TestBuf u4GoldBuf [loop][len (1~512)][1:variant len test]\n");
        return -1;
  }

    if ( v4TestLen > 1024 )
  {
        LOG(0, "len should be less than or equal to 1024\n");
  }

    // Set FIFO MODE
    PCMCIA_RawWrite32(REG_POD_GPR_CNTL_SET0, ((PCMCIA_RawRead32(REG_POD_GPR_CNTL_SET0)&~FIFO_MODE)|FIFO4FLASH));

    if ( pu4TestBuf == NULL || pu4GoldBuf == NULL )
  {
        return -1;
  }

    // fix length, fix pattern
    for ( i = 1 ; i <= u4TestLoop ; i++ )
  {
        for( j = 0 ; j < 4096 ; j++ )
  {
            pu4GoldBuf[j] = 0xca;
            pu4TestBuf[j] = 0;
  }

        u4Addr = 0;
        u4DataLen = v4TestLen;
        SPI_DMAWrite((const UINT32 *)pu4GoldBuf, u4Addr, u4DataLen);
        SPI_DMARead((UINT32*)pu4TestBuf, u4Addr, u4DataLen);
        
        for(j=0; j<(u4DataLen*4); j++)
  {
            if ( pu4GoldBuf[j] != pu4TestBuf[j] )
  {
                Printf("Cmp fail (fix pattern), Loop:%d, offset:%d, Predict:0x%x, Read:0x%x\n", i, j, pu4GoldBuf[j], pu4TestBuf[j]);
                fgErrorOccur = TRUE;
  }
  }
        if ( fgErrorOccur == TRUE )
  {
            return 0;
  }
  }

    // fix length, random pattern
    for ( i = 1 ; i <= u4TestLoop ; i++ )
  {
        for( j = 0 ; j < 4096 ; j++ )
  {
            pu4GoldBuf[j] = (UINT32)rand();
            pu4TestBuf[j] = 0;
}

        u4Addr = 0;
        u4DataLen = v4TestLen;
        SPI_DMAWrite((const UINT32 *)pu4GoldBuf, u4Addr, u4DataLen);
        SPI_DMARead((UINT32*)pu4TestBuf, u4Addr, u4DataLen);
        
        for(j=0; j<(u4DataLen*4); j++)
{
            if ( pu4GoldBuf[j] != pu4TestBuf[j] )
    {
                Printf("Cmp fail (random pattern), Loop:%d, offset:%d, Predict:0x%x, Read:0x%x\n", i, j, pu4GoldBuf[j], pu4TestBuf[j]);
                fgErrorOccur = TRUE;
    }
        }
        if ( fgErrorOccur == TRUE )
        {
            return 0;
        }
      }

    // Variant len
    if ( u4VariantLenTest == 1 )
    {
        Printf("Variant Len Test Start\n");
        
        for ( i = 1 ; i < v4TestLen ; i++ )
        {
            u4DataLen = i;
            u4Addr = 0;

            srand(REG_RW_TIMER0_LOW);
            for( j = 0 ; j < 4096 ; j++ )
    {
                pu4GoldBuf[j] = (UINT32)rand();
                pu4TestBuf[j] = 0;
    }

            SPI_DMAWrite((const UINT32 *)pu4GoldBuf, u4Addr, u4DataLen);
            SPI_DMARead((UINT32*)pu4TestBuf, u4Addr, u4DataLen);
            
            for(j=0; j<(u4DataLen*4); j++)
      {
                if ( pu4GoldBuf[j] != pu4TestBuf[j] )
        {
                    Printf("Cmp fail (random pattern/length), Loop:%d, offset:%d, Predict:0x%x, Read:0x%x\n", i, j, pu4GoldBuf[j], pu4TestBuf[j]);
                    fgErrorOccur = TRUE;
        }
      }
            
            if ( fgErrorOccur == TRUE )
    {
                return 0;
      }
      }
    }
    
    Printf("Test Done\n");
    return i4Return;
}

static INT32 PcmciaCmdSpiDemodRead(INT32 i4Argc, const CHAR** szArgv)
{
    INT32  i4Return = POD_OK;
    UINT8 u1Data;
    UINT32 u4Addr;
    UINT32 u4Count = 1;
    UINT32 i;

    if ((i4Argc != 2) && (i4Argc != 3))
    {
        LOG(0, "pr addr [count]\n");
        return -1;
    }
    if (i4Argc == 3)
    {
        u4Count = StrToInt(szArgv[2]);
    }

    u4Addr = StrToInt(szArgv[1]);
    for (i = 0; i < u4Count; i++)
      {
        i4Return = SPI_DemodSingleRead(&u1Data, u4Addr);
        LOG(0, "Demod read@0x%x=0x%x\n", u4Addr, u1Data);
        u4Addr++;
        }

    return i4Return;
}

static INT32 PcmciaCmdSpiDemodWrite(INT32 i4Argc, const CHAR** szArgv)
{
    INT32   i4Return = POD_OK;
    UINT8   u1Data;
    UINT32  u4Addr;

    if (i4Argc != 3)
    {
        LOG(0, "pw addr data\n");
        return -1;
    }
    u4Addr = StrToInt(szArgv[1]);
    u1Data = StrToInt(szArgv[2]);

    i4Return = SPI_DemodSingleWrite((const UINT8 *) &u1Data, u4Addr);

    return i4Return;
}

//-----------------------------------------------------------------------------
// Command Tables
//-----------------------------------------------------------------------------
#if defined(CC_INTERNAL_CI) || defined(CC_EXTERNAL_CI) || defined(CC_INTERNAL_POD) || defined(CC_EXTERNAL_POD)
static CLI_EXEC_T _arNFICmdTbl[] =
{
    {"init",        "i",     PcmciaCmdNfiInit,      NULL,   "Init NFI module",      CLI_SUPERVISOR},
    {"sdawrite",    "sw",    PcmciaCmdNfiSWrite,    NULL,   "SDA write",            CLI_SUPERVISOR},
    {"sdaread",     "sr",    PcmciaCmdNfiSRead,     NULL,   "SDA read",             CLI_SUPERVISOR},
    {"burstwrite",  "bw",    PcmciaCmdNfiBWrite,    NULL,   "Burst write",          CLI_SUPERVISOR},
    {"burstread",   "br",    PcmciaCmdNfiBRead,     NULL,   "Burst read",           CLI_SUPERVISOR},
    {NULL,          NULL,    NULL,                  NULL,   NULL,                   CLI_SUPERVISOR}
};

static CLI_EXEC_T _arSPICmdTbl[] =
{
    {"init",        "i",     PcmciaCmdSpiInit,      NULL,   "Init SPI module",      CLI_SUPERVISOR},
    {"reset",       "r",     PcmciaCmdSpiReset,     NULL,   "Reset SPI module",     CLI_SUPERVISOR},
    {"sdawrite",    "sw",    PcmciaCmdSpiSWrite,    NULL,   "SDA write",            CLI_SUPERVISOR},
    {"sdaread",     "sr",    PcmciaCmdSpiSRead,     NULL,   "SDA read",             CLI_SUPERVISOR},
    {"dmawrite",    "dw",    PcmciaCmdSpiDWrite,    NULL,   "DMA write",            CLI_SUPERVISOR},
    {"dmaread",     "dr",    PcmciaCmdSpiDRead,     NULL,   "DMA read",             CLI_SUPERVISOR},
    {"dmatest",     "dt",    PcmciaCmdSpiDTest,     NULL,   "DMA test",             CLI_SUPERVISOR},
    {"dmatest",     "dta",    PcmciaCmdSpiDTAlloc,   NULL,   "DMA alloc",             CLI_SUPERVISOR},
    {"dmatest",     "dts",    PcmciaCmdSpiDTStart,   NULL,   "DMA start",             CLI_SUPERVISOR},
    {"proxyread",   "pr",    PcmciaCmdSpiDemodRead, NULL,   "Demod proxy read",     CLI_SUPERVISOR},
    {"proxywrite",  "pw",    PcmciaCmdSpiDemodWrite,NULL,   "Demod proxy write",    CLI_SUPERVISOR},
    {NULL,          NULL,    NULL,                  NULL,   NULL,                   CLI_SUPERVISOR}
};

static CLI_EXEC_T _arMwIfCmdTbl[] =
{
    {"et",  "et",    PcmciaCmdEnableTs,  NULL,   "Enable TS, 0:Off, 1:On",      CLI_SUPERVISOR},
    {"es",  "es",    PcmciaCmdEnableSlot,NULL,   "Enable Slot, 0:Off, 1:On",    CLI_SUPERVISOR},
    {NULL,  NULL,    NULL,               NULL,   NULL,                          CLI_SUPERVISOR}
};

static CLI_EXEC_T _arMPODTSCmdTbl[] =
{
    {"init",        "i",        PcmciaCmdMpodInitTs,    NULL,   "MPOD TS Init", CLI_SUPERVISOR},
    {"info",        "info",     PcmciaCmdMpodPrintInfo, NULL,   "MPOD TS Info", CLI_SUPERVISOR},
    {NULL,          NULL,       NULL,                   NULL,   NULL,           CLI_SUPERVISOR}
};

static CLI_EXEC_T _arMPodCmdTbl[] =
{
    {"init",        "i",        PcmciaCmdMpodInit,          NULL,               "Init MPOD",                            CLI_SUPERVISOR},
    {"read",        "r",        PcmciaCmdMpodRead,          NULL,               "MPOD read",                            CLI_SUPERVISOR},
    {"write",       "w",        PcmciaCmdMpodWrite,         NULL,               "MPOD write",                           CLI_SUPERVISOR},
    {"oas",         "oas",      PcmciaCmdMpodOpenASession,  NULL,               "MPOD open a session",                  CLI_SUPERVISOR},
    {"stress",      "stress",   PcmciaCmdMpodStress,        NULL,               "MPOD CPU Interface Stress Test",       CLI_SUPERVISOR},
    {"cks",         "cks",      PcmciaCmdMpodCkgenStress,   NULL,               "Ckgen Register Stress Test",           CLI_SUPERVISOR},
    {"ts",          "ts",       NULL,                       _arMPODTSCmdTbl,    "MPOD TS Command",                     CLI_SUPERVISOR},
    {NULL,          NULL,       NULL,                       NULL,               NULL,                                   CLI_SUPERVISOR}
};

static CLI_EXEC_T _arPodCiCmdTbl[] =
{
    {"cis",         "cis",  PcmciaCmdGetCis,    NULL,   "Get CIS",                          CLI_SUPERVISOR},
    {NULL,          NULL,   NULL,               NULL,   NULL,                               CLI_SUPERVISOR}
};

static CLI_EXEC_T _arCommonCmdTbl[] =
{
    {"read",        "r",        PcmciaCmdCommonReadReg, NULL,   "Read CI module registers",             CLI_SUPERVISOR},
    {"write",       "w",        PcmciaCmdCommonWriteReg,NULL,   "Read CI module registers",             CLI_SUPERVISOR},
    {"bypass",      "bps",      PcmciaCmdSetBypass,     NULL,   "TS bypass 8295, 0:Off, 1:ON",          CLI_SUPERVISOR},
    {"pts",         "pts",      PcmciaCmdSetParallelTs, NULL,   "Set Paralles TS, 0:Serial, 1:Parallel",CLI_SUPERVISOR},
    {"extd",        "extd",     PcmciaCmdSetExtDemod,   NULL,   "Set External Demod, 0:OFF, 1:ON",      CLI_SUPERVISOR},    
    {"settstype",   "stt",      PcmciaCmdSetTsType,     NULL,   "Set TS Type, [0:DVB-T, 1:DVB-C]",      CLI_SUPERVISOR},
    {"gpioenable",  "ge",       PcmciaCmdGpioEnable,    NULL,   "GPIO Enable, Gpio, Polarity",          CLI_SUPERVISOR},
    {"gpioinput",   "gi",       PcmciaCmdGpioInput,     NULL,   "Get GPIO Input Value",                 CLI_SUPERVISOR},
    {"gpiooutput",  "go",       PcmciaCmdGpioOutput,    NULL,   "Set GPIO Output Value",                CLI_SUPERVISOR},
    {"gpiointrq",   "gint",     PcmciaCmdGpioIntrq,     NULL,   "Enable/Disable GPIO Interrupt",        CLI_SUPERVISOR},
    {"set2bit",     "s2b",      PcmciaCmdSet2Bit,       NULL,   "Set/Get 2 bit",                        CLI_SUPERVISOR},
    {"setdrivin",   "sd",       PcmciaCmdSetDriving,    NULL,   "Set Drivin",                           CLI_SUPERVISOR},
    {"settsclock",  "stc",      PcmciaCmdSetTsClock,    NULL,   "Set TS Clock",                         CLI_SUPERVISOR},
    {"setsysclock", "ssc",      PcmciaCmdSetSysClock,   NULL,   "Set Sys Clock",                        CLI_SUPERVISOR},
    {NULL,          NULL,       NULL,                   NULL,   NULL,                                   CLI_SUPERVISOR}
};

static CLI_EXEC_T _arIcVfyCmdTbl[] =
{
    {"reg",     "reg",      PcmciaCmdRegTest,       NULL,   "Register test",                        CLI_SUPERVISOR},
    {"sreg",    "sreg",     PcmciaCmdSingleRegTest, NULL,   "Single Register R/W test",             CLI_SUPERVISOR},
    {"wnd",     "wnd",      PcmciaCmdWndTest,       NULL,   "Memory Window test (SysBuild)",        CLI_SUPERVISOR},
    {"pll",     "pll",      PcmciaCmdPllTest,       NULL,   "PLL Test (SysBuild)",                  CLI_SUPERVISOR},
    {"gpio",    "gpio",     PcmciaCmdGpioTest,      NULL,   "Test GPIO Pings",                      CLI_SUPERVISOR},
    {"tc",      "tc",       PcmciaCmdCreateTC,      NULL,   "Automatically create T_C",             CLI_SUPERVISOR},
    {"pbus",    "pbus",     PcmciaCmdPBusTest,      NULL,   "PBus Test (DrvBuild)",                 CLI_SUPERVISOR},
    {"tnas",    "tnas",     PcmciaCmdThreadNoAssert,NULL,   "Thread Don't Assert, 0:Ast, 1:NoAst,", CLI_SUPERVISOR},
    {"st",      "st",       PcmciaCmdStopThread,    NULL,   "Stop Pcmcia Thread",                   CLI_SUPERVISOR},
    {"5vpc",    "5vpc",     PcmciaCmd5VCtrl,        NULL,   "CI 5V Control, 0:Original, 1:Manual",  CLI_SUPERVISOR},
    {"scpt",    "scpt",     PcmciaCmdSetChipType,   NULL,   "Set Chip Type, 0:External, 1:Internal",CLI_SUPERVISOR},   
    {"scdt",    "scdt",     PcmciaCmdSetCardType,   NULL,   "Set Card Type, 0:CI, 1:POD",           CLI_SUPERVISOR},
    {"sit",     "sit",      PcmciaCmdSetIfType,     NULL,   "Set Interface Type, 0:NFI, 1:SPI",     CLI_SUPERVISOR},
    {"resetcam","rstc",     PcmciaCmdResetCam,      NULL,   "Reset Card",                           CLI_SUPERVISOR},
    {"reset",   "rst",      PcmciaCmdReset,         NULL,   "Reset MT8295",                         CLI_SUPERVISOR},
    {"sam",     "sam",      PcmciaCmdSetAccessMode, NULL,   "Set Access Mode, 0:SDM, 1:DMA",        CLI_SUPERVISOR},
    {"slt",     "slt",      PcmciaCmdSlt,           NULL,   "Start SLT procedure",                  CLI_SUPERVISOR},    
    {"demo",    "demo",     PcmciaCmdDemo,          NULL,   "Start for Demo Program",               CLI_SUPERVISOR},    
    {"ckr",     "ckr",      PcmciaCmdSet8295ClkRate,NULL,   "Set External IC Serial TS Clk Out Rate",CLI_SUPERVISOR},
    {"dts",     "dts",      PcmciaCmdDemodTsOnOff,  NULL,   "Enable/Disable  Demod TS",             CLI_SUPERVISOR},
    {"rt",      "rt",       PcmciaCmdSetTsRoute,    NULL,   "Set TS route",                         CLI_SUPERVISOR},
    {NULL,      NULL,       NULL,                   NULL,               NULL,                       CLI_SUPERVISOR}
};

CLIMOD_DEBUG_FUNCTIONS(PCMCIA) 
static CLI_EXEC_T arPcmciaCmdTbl[] =
{
    CLIMOD_DEBUG_CLIENTRY(PCMCIA),
    
    {"init",    "i",    PcmciaCmdInit,  NULL,               "Init POD driver, get cis, nego buf size4",CLI_SUPERVISOR},
    {"vfy",     "v",    NULL,           _arIcVfyCmdTbl,     "IC Vrification",                       CLI_SUPERVISOR},
    {"common",  "c",    NULL,           _arCommonCmdTbl,    "Common Commands",                      CLI_SUPERVISOR},
    {"ci",      "ci",   NULL,           _arPodCiCmdTbl,     "CI Commands",                          CLI_SUPERVISOR},
    {"pod",     "p",    NULL,           _arPodCiCmdTbl,     "POD Commands",                         CLI_SUPERVISOR},
    {"mpod",    "m",    NULL,           _arMPodCmdTbl,      "MPOD Commands",                        CLI_SUPERVISOR},
    {"mwif",    "mi",   NULL,           _arMwIfCmdTbl,      "Middleware Interface Commands",        CLI_SUPERVISOR},    
    {"spi",     "s",    NULL,           _arSPICmdTbl,       "SPI Interface Commands",               CLI_SUPERVISOR},
    {"nfi",     "n",    NULL,           _arNFICmdTbl,       "NFI Interface Commands",               CLI_SUPERVISOR},
    {NULL,      NULL,   NULL,           NULL,               NULL,                                   CLI_SUPERVISOR}
};

CLI_MAIN_COMMAND_ITEM(Pcmcia)
{
    "pcmcia",
    "p",
    NULL,
    arPcmciaCmdTbl,
    "Pcmcia command",
    CLI_GUEST
};

#else
static CLI_EXEC_T _arSPICmdTbl[] =
{
    {"init",        "i",     PcmciaCmdSpiInit,      NULL,   "Init SPI module",      CLI_SUPERVISOR},
    {"reset",       "r",     PcmciaCmdSpiReset,     NULL,   "Reset SPI module",     CLI_SUPERVISOR},
    {"sdawrite",    "sw",    PcmciaCmdSpiSWrite,    NULL,   "SDA write",            CLI_SUPERVISOR},
    {"sdaread",     "sr",    PcmciaCmdSpiSRead,     NULL,   "SDA read",             CLI_SUPERVISOR},
    {"dmawrite",    "dw",    PcmciaCmdSpiDWrite,    NULL,   "DMA write",            CLI_SUPERVISOR},
    {"dmaread",     "dr",    PcmciaCmdSpiDRead,     NULL,   "DMA read",             CLI_SUPERVISOR},
    {"dmatest",     "dt",    PcmciaCmdSpiDTest,     NULL,   "DMA test",             CLI_SUPERVISOR},
    {"dmatest",     "dta",    PcmciaCmdSpiDTAlloc,   NULL,   "DMA alloc",             CLI_SUPERVISOR},
    {"dmatest",     "dts",    PcmciaCmdSpiDTStart,   NULL,   "DMA start",             CLI_SUPERVISOR},
    {"proxyread",   "pr",    PcmciaCmdSpiDemodRead, NULL,   "Demod proxy read",     CLI_SUPERVISOR},
    {"proxywrite",  "pw",    PcmciaCmdSpiDemodWrite,NULL,   "Demod proxy write",    CLI_SUPERVISOR},
    {NULL,          NULL,    NULL,                  NULL,   NULL,                   CLI_SUPERVISOR}
};


CLIMOD_DEBUG_FUNCTIONS(PCMCIA) 
static CLI_EXEC_T arPcmciaCmdTbl[] =
{
    CLIMOD_DEBUG_CLIENTRY(PCMCIA),
    {"spi",     "s",    NULL,           _arSPICmdTbl,       "SPI Interface Commands",               CLI_SUPERVISOR},
    {NULL,      NULL,   NULL,           NULL,               NULL,                                   CLI_SUPERVISOR}
};

CLI_MAIN_COMMAND_ITEM(Pcmcia)
{
    "pcmcia",
    "p",
    NULL,
    arPcmciaCmdTbl,
    "Pcmcia command",
    CLI_GUEST
};
#endif

LINT_EXT_HEADER_END

