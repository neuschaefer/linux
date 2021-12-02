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
 * $Author: jifeng.fan $
 * $Date: 2012/11/15 $
 * $RCSfile: panel_table.c,v $
 * $Revision: #10 $
 *
 *---------------------------------------------------------------------------*/

/** @file panel_tablc.c
 *  This file includes panel timing configuration.
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#ifndef CC_MTK_PRELOADER

#include "panel.h"
#include "eeprom_if.h"
#include "drvcust_if.h"
#include "nand_if.h"
#include "x_lint.h"
LINT_EXT_HEADER_BEGIN
#include "x_typedef.h"
#include "x_printf.h"
#include "x_pinmux.h"
#include "x_hal_5381.h"
#include "x_assert.h"
#include "x_os.h"
LINT_EXT_HEADER_END
#ifndef CC_MTK_LOADER
#include "linux/file_ops.h"
#endif

#ifdef CUSTOM_CFG_FILE
#include CUSTOM_CFG_FILE
#endif /* CUSTOM_CFG_FILE */

#include "drv_default.h"
//#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
#include"panel_cust.h"
//#endif
extern UINT32 PANEL_GetPanelWidth(void);
extern 	UINT32 PANEL_GetPanelHeight(void);
#define PANEL_FILE_HEAD_VER_SIZE                16
#define PANEL_TBL_HEAD_SCRIPT_SIZE           48

#ifdef CUSTOM_EEPROM_FILE
#include CUSTOM_EEPROM_FILE
#endif
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------
#ifdef SUPPORT_VESTEL_FLASH_PANEL
#define PANEL_FILE_HASH                      4
#define PANEL_FILE_ID_SIZE                   6
#define PANEL_FILE_HEAD_VER_SIZE             16
#define PANEL_TBL_HEAD_ID_SIZE               8
#define PANEL_TBL_HEAD_SCRIPT_SIZE           48
#define PANEL_TBL_HEAD_DATE_SIZE             12
#define VESTEL_FLASH_PANEL_INDEX             1
//-----------------------------------------------------------------------------------
// Flash Panel Header Structure
typedef struct
{
    char bID[PANEL_FILE_ID_SIZE];

    UINT16 wNumOfPanel;
    UINT16 wSizeOfPanel;

    char bPtblXmlVer[PANEL_FILE_HEAD_VER_SIZE];

    UINT8 bHash[PANEL_FILE_HASH];
    UINT8 bReserved[2];
    //32 byte
}tagPanelFileHead;

typedef struct 
{
    char bID[PANEL_TBL_HEAD_ID_SIZE];
    char bScript[PANEL_TBL_HEAD_SCRIPT_SIZE];
    char bDate[PANEL_TBL_HEAD_DATE_SIZE];
    UINT8 bReserve[12];
    //80byte
}tagPanelTblHead;

//Ptf Version info
typedef struct 
{
    UINT8 bPtfVer[PANEL_FILE_HEAD_VER_SIZE];
    UINT8 bPtfPanelName[PANEL_TBL_HEAD_SCRIPT_SIZE];
} PTF_INFO;

typedef struct
{
    UINT16 u2PanelWidth;
    UINT16 u2PanelHeight;

    UINT32 u4PixelClkMax;
    UINT32 u4PixelClk60Hz;
    UINT32 u4PixelClk50Hz;
    UINT32 u4PixelClk48Hz;
    UINT32 u4PixelClkMin;

    UINT16 u2HTotalMax;
    UINT16 u2HTotal60Hz;
    UINT16 u2HTotal50Hz;
    UINT16 u2HTotal48Hz;
    UINT16 u2HTotalMin;

    UINT16 u2VTotalMax;
    UINT16 u2VTotal60Hz;
    UINT16 u2VTotal50Hz;
    UINT16 u2VTotal48Hz;
    UINT16 u2VTotalMin;

    UINT16 u1VClkMax;
    UINT16 u1VClkMin;
	
    UINT16 u2HPosition;
    UINT16 u2VPosition;

    UINT8 u1HSyncWidth;
    UINT8 u1VSyncWidth;
	UINT16 u2Reserved; //for 32bit allign
		
    UINT32 u4ControlWord;

    UINT16 u2LVDS_Amplitude;
    UINT16 u2LVDS_SpreadRange;//u2LVDS_SpreadStep; 1/1000
    UINT16 u2LVDS_ModulationRate;//u2LVDS_SpreadSpan; 30K ~ 200K
    UINT16 u2DRAM_SpreadRange;
    UINT16 u2DRAM_SpreadModulationRate;
	UINT16 u2Reserved1; //for 32bit allign

    UINT8 u1BacklightHigh;
    UINT8 u1BacklightMiddle;
    UINT8 u1BacklightLow;
	UINT8 u1Reserved; //for 32bit allign

	UINT32 u4DimingFreq60Hz;
    UINT32 u4DimingFreq50Hz;
    UINT32 u4DimingFreq48Hz;
	
    UINT16 u1LvdsOnDalay;
    UINT16 u1BacklightOnDelay;
    UINT16 u1BacklightOffDelay;
    UINT16 u1LvdsOffDalay;

	UINT32 u4Reserved0;
	UINT32 u4ControlWord2;
    UINT32 u4Reserved2;
	UINT32 u4Reserved3;
	UINT32 u4Reserved4;
} PANEL_FLASH_ATTRIBUTE_T;

typedef struct
{
    UINT16 u2PanelWidth;
    UINT16 u2PanelHeight;

    UINT32 u4PixelClkMax;
    UINT32 u4PixelClk60Hz;
    UINT32 u4PixelClk50Hz;
    UINT32 u4PixelClk48Hz;
    UINT32 u4PixelClkMin;

    UINT16 u2HTotalMax;
    UINT16 u2HTotal60Hz;
    UINT16 u2HTotal50Hz;
    UINT16 u2HTotal48Hz;
    UINT16 u2HTotalMin;

    UINT16 u2VTotalMax;
    UINT16 u2VTotal60Hz;
    UINT16 u2VTotal50Hz;
    UINT16 u2VTotal48Hz;
    UINT16 u2VTotalMin;

    UINT16 u1VClkMax;
    UINT16 u1VClkMin;
	
    UINT16 u2HPosition;
    UINT16 u2VPosition;

    UINT8 u1HSyncWidth;
    UINT8 u1VSyncWidth;
	UINT16 u2Reserved; //for 32bit allign
		
    UINT32 u4ControlWord;

    UINT16 u2LVDS_Amplitude;
    UINT16 u2LVDS_SpreadRange;//u2LVDS_SpreadStep; 1/1000
    UINT16 u2LVDS_ModulationRate;//u2LVDS_SpreadSpan; 30K ~ 200K
    UINT16 u2DRAM_SpreadRange;
    UINT16 u2DRAM_SpreadModulationRate;
	UINT16 u2Reserved1; //for 32bit allign

    UINT8 u1BacklightHigh;
    UINT8 u1BacklightMiddle;
    UINT8 u1BacklightLow;
	UINT8 u1Reserved; //for 32bit allign

	UINT32 u4DimingFreq60Hz;
    UINT32 u4DimingFreq50Hz;
    UINT32 u4DimingFreq48Hz;
	
    UINT16 u1LvdsOnDalay;
    UINT16 u1BacklightOnDelay;
    UINT16 u1BacklightOffDelay;
    UINT16 u1LvdsOffDalay;

	UINT32 u4Reserved0;
	UINT32 u4ControlWord2;
    UINT32 u4Reserved2;
	UINT32 u4Reserved3;
	UINT32 u4Reserved4;
}PANEL_ATTRIBUTE_T;
UINT32 u4VestelFlashPNLIndex=VESTEL_FLASH_PANEL_INDEX;
#else

typedef struct
{
    UINT16 u2PanelWidth;
    UINT16 u2PanelHeight;

    // Clk60Hz = htotal*vtotal*60/1.001
    // Clk50Hz = htotal*vtotal*50
    UINT32 u4PixelClkMax;
    UINT32 u4PixelClk60Hz;
    UINT32 u4PixelClk50Hz;
    UINT32 u4PixelClkMin;

    // HTotal = spec value
    UINT16 u2HTotalMax;
    UINT16 u2HTotal60Hz;
    UINT16 u2HTotal50Hz;
    UINT16 u2HTotalMin;

    // VTotal = spec value
    UINT16 u2VTotalMax;
    UINT16 u2VTotal60Hz;
    UINT16 u2VTotal50Hz;
    UINT16 u2VTotalMin;
    UINT8 u1VClkMax;
    UINT8 u1VClkMin;

    UINT8 u1HSyncWidth;
    UINT8 u1VSyncWidth;
    UINT16 u2HPosition;
    UINT16 u2VPosition;
    UINT32 u4ControlWord;

    // backlight ragne
    UINT8 u1BacklightHigh;
    UINT8 u1BacklightMiddle;
    UINT8 u1BacklightLow;
    UINT8 u1Reserved;

    UINT16 u2DimmingFrequency60Hz;
    UINT16 u2DimmingFrequency50Hz;

    // delay unit: 10ms
    UINT8 u1LvdsOnDalay;
    UINT8 u1BacklightOnDelay;
    UINT8 u1BacklightOffDelay;
    UINT8 u1LvdsOffDalay;

    UINT16 u2MaxOverscan;
    UINT16 u2Reserved;
} PANEL_ATTRIBUTE_T;


typedef struct
{
    UINT16 u2PanelWidth;
    UINT16 u2PanelHeight;

    UINT32 u4PixelClkMax;
    UINT32 u4PixelClk60Hz;
    UINT32 u4PixelClk50Hz;
    UINT32 u4PixelClkMin;

    UINT32 u4Reserved1;

    UINT16 u2HTotalMax;
    UINT16 u2HTotal60Hz;
    UINT16 u2HTotal50Hz;
    UINT16 u2HTotalMin;

    UINT16 u2Reserved2;
    UINT16 u2VTotalMax;
    UINT16 u2VTotal60Hz;
    UINT16 u2VTotal50Hz;
    UINT16 u2VTotalMin;
    UINT16 u2Reserved3;

    UINT32 u4Reserved4;

    UINT16 u2Reserved5;
    UINT8 u1VClkMax;
    UINT8 u1VClkMin;

    UINT16 u2Reserved6;
    UINT8 u1HSyncWidth;
    UINT8 u1VSyncWidth;

    UINT8 u2HPosition;
    UINT8 u2VPosition;
    UINT16 u2Reserved7;

    UINT16 u4Reserved8;
    UINT8 u1LvdsMapping;
    UINT8 u1MaxOverscan1;

    UINT8 u1MaxOverscan2;
    UINT8 u1ControlWord1;
    UINT8 u1ControlWord2;
    UINT8 u1PanelOnTime1;

    UINT8 u1LvdsOnDalay;

    UINT8 u1BacklightOnDelay;
    UINT8 u1PanelOnTime4;
    UINT8 u1Reserved8;

    UINT8 u1PanelOffTime1;
    UINT8 u1BacklightOffDelay;
    UINT8 u1LvdsOffDalay;
    UINT8 u1PanelOffTime4;
} PANEL_FLASH_ATTRIBUTE_T;
#endif
//-----------------------------------------------------------------------------
// Macro definitions
UINT32 u4gbDitherMode = 0;
//-----------------------------------------------------------------------------

/*lint -save -e960 */
#define PANEL_ATTRIBUTE_SET_TMPL(NAME, FIELD) \
    void PANEL_Set##NAME(UINT32 u4Value) \
    { \
        _prPanelAttribute->FIELD = u4Value; \
    } \

#define PANEL_ATTRIBUTE_GET_TMPL(NAME, FIELD) \
    UINT32 PANEL_Get##NAME(void) \
    { \
        return _prPanelAttribute->FIELD; \
    } \

#define PANEL_ATTRIBUTE_SETGET_TMPL(NAME, FIELD) \
    PANEL_ATTRIBUTE_SET_TMPL(NAME, FIELD) \
    PANEL_ATTRIBUTE_GET_TMPL(NAME, FIELD) \

/* above line is intendedly left blank */
/*lint -restore */

#define IS_PANEL_INDEX_INVALID(X) ((X) >= u4TotalNumber)
/*
#define CHECK_ZERO(X) \
    { \
        if ((X) == 0) \
        { \
            return FALSE; \
        } \
    }
*/

#define CHECK_ZERO(X) \
		{ \
			Printf("%d\n", X);\
		}


#define EXT_PANEL_INDEX_INVALID 0xffff

#define SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET (10)

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

#ifndef CC_MULTI_PANEL
// we build all panel timing in default
#define CC_MULTI_PANEL 1
#endif

#ifdef CC_NAND_LOADER
#define NAND_PAGE_SIZE  (2048)
#endif

// this is the panel_table_cust.c in each customer's directory
#include PANEL_TABLE_CUST_FILE

#if CC_MULTI_PANEL
static PANEL_ATTRIBUTE_T* _prPanelAttribute =
                          &_arPanelAttribute[DEFAULT_PANEL_SELECT];
static UINT32 _u4PanelIndex = DEFAULT_PANEL_SELECT;
static UINT32 _u4EepromPanelIndex = DEFAULT_PANEL_SELECT;

//Flash Panel variable define
tagPanelFileHead aFlashPanelFileHead;
tagPanelTblHead aFlashPanelTblHead;

#else
static PANEL_ATTRIBUTE_T* _prPanelAttribute = &_arPanelAttribute[0];
static UINT32 _u4PanelIndex = 0;
static UINT32 _u4EepromPanelIndex = 0;
#endif
static UINT32 u4TotalNumber = PANEL_TOTAL_NUMBER;
static INT32 _i4HPixelShift = 0;
static INT32 _i4VPixelShift = 0;
static BOOL _fgPixelShiftEnable = FALSE;
static UINT32 _fgPanelIdInEeprom = FALSE;

#if defined(SUPPORT_PANEL_48HZ) || defined(SUPPORT_PANEL_CUSTOMER_SPEC) || defined(SUPPORT_PANEL_SS) || defined(SUPPORT_PANEL_DITHER)  || defined(SUPPORT_PANEL_3D_PIXEL_SIZE)
static UINT32 PANEL_GetExtAttributeIndex(void);
#endif
//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

#ifndef LOAD_PANEL_FROM_FLASH_CUSTOM

#ifdef SUPPORT_VESTEL_FLASH_PANEL
#ifndef CC_MTK_LOADER
static INT32 Panel_ReadNOR(UINT32 u4Offset, void *pvMemPtr, UINT32 u4MemLen)
{
	UINT32 u4PartId = 0;
	UINT64 u8Offset = 0;
	UINT32 u4Len = 0;

	u4PartId=DRVCUST_PanelGet(ePanelFlashTablePartitionID);
	Printf(" u4PartId = %d \n",u4PartId);
	u8Offset = ((((UINT64)u4PartId)<<32) | u4Offset);
	Printf(" u8Offset = %d \n",u8Offset);

	struct file *file = file_open("/ro/ptf.bin", O_RDONLY, 0);

    if (!file)
    {
        return -1;
    }

    u4Len = file_read(file, pvMemPtr, u4MemLen, &u8Offset);
    Printf(" u4Len = %d \n",u4Len);

    file_close(file);

    if (u4Len != u4MemLen)
    {
		return -1;
    }
    else
    {
    #if 0
		for(i = 0; i < u4MemLen; i++)
		{
			Printf("pvMemPtr[%d] = 0x%x \n",i, *((UINT8*)pvMemPtr + i));	
		}
    #endif
		return (INT32)u4MemLen;
    }
}

static BOOL _PanelLoadTableFromNOR(UINT32 u4Offset)
{
	PANEL_FLASH_ATTRIBUTE_T rFlashTable;
	UINT8  aui1_buf[sizeof(PANEL_FLASH_ATTRIBUTE_T)];
	UINT32 u4MemLen = sizeof(PANEL_FLASH_ATTRIBUTE_T);
    
    Printf("#### Vestel load panel table from NOR! ####\n");
    Printf(" u4Offset=%d, u4MemLen=%d\n",u4Offset,u4MemLen);
	Panel_ReadNOR(u4Offset,aui1_buf,u4MemLen);

	x_memcpy(&rFlashTable, aui1_buf, sizeof(PANEL_FLASH_ATTRIBUTE_T));

	_arPanelAttribute[0].u2PanelWidth = rFlashTable.u2PanelWidth;
    CHECK_ZERO(_arPanelAttribute[0].u2PanelWidth);
    _arPanelAttribute[0].u2PanelHeight = rFlashTable.u2PanelHeight;
    CHECK_ZERO(_arPanelAttribute[0].u2PanelHeight);
	
    _arPanelAttribute[0].u4PixelClkMax = rFlashTable.u4PixelClkMax;
    CHECK_ZERO(_arPanelAttribute[0].u4PixelClkMax);
    _arPanelAttribute[0].u4PixelClk60Hz = rFlashTable.u4PixelClk60Hz;
    CHECK_ZERO(_arPanelAttribute[0].u4PixelClk60Hz);
    _arPanelAttribute[0].u4PixelClk50Hz = rFlashTable.u4PixelClk50Hz;
    CHECK_ZERO(_arPanelAttribute[0].u4PixelClk50Hz);
    _arPanelAttribute[0].u4PixelClk48Hz = rFlashTable.u4PixelClk48Hz;
    CHECK_ZERO(_arPanelAttribute[0].u4PixelClk48Hz);
    _arPanelAttribute[0].u4PixelClkMin = rFlashTable.u4PixelClkMin;
    CHECK_ZERO(_arPanelAttribute[0].u4PixelClkMin);
	
    _arPanelAttribute[0].u2HTotalMax = rFlashTable.u2HTotalMax;
    CHECK_ZERO(_arPanelAttribute[0].u2HTotalMax);
    _arPanelAttribute[0].u2HTotal60Hz = rFlashTable.u2HTotal60Hz;
    CHECK_ZERO(_arPanelAttribute[0].u2HTotal60Hz);
    _arPanelAttribute[0].u2HTotal50Hz = rFlashTable.u2HTotal50Hz;
    CHECK_ZERO(_arPanelAttribute[0].u2HTotal50Hz);
    _arPanelAttribute[0].u2HTotal48Hz = rFlashTable.u2HTotal48Hz;
    CHECK_ZERO(_arPanelAttribute[0].u2HTotal48Hz);
    _arPanelAttribute[0].u2HTotalMin = rFlashTable.u2HTotalMin;
    CHECK_ZERO(_arPanelAttribute[0].u2HTotalMin);

    _arPanelAttribute[0].u2VTotalMax = rFlashTable.u2VTotalMax;
    CHECK_ZERO(_arPanelAttribute[0].u2VTotalMax);
    _arPanelAttribute[0].u2VTotal60Hz = rFlashTable.u2VTotal60Hz;
    CHECK_ZERO(_arPanelAttribute[0].u2VTotal60Hz);
    _arPanelAttribute[0].u2VTotal50Hz = rFlashTable.u2VTotal50Hz;
    CHECK_ZERO(_arPanelAttribute[0].u2VTotal50Hz);
    _arPanelAttribute[0].u2VTotal48Hz = rFlashTable.u2VTotal48Hz;
    CHECK_ZERO(_arPanelAttribute[0].u2VTotal48Hz);
    _arPanelAttribute[0].u2VTotalMin = rFlashTable.u2VTotalMin;
    CHECK_ZERO(_arPanelAttribute[0].u2VTotalMin);
	
    _arPanelAttribute[0].u1VClkMax = rFlashTable.u1VClkMax;
    CHECK_ZERO(_arPanelAttribute[0].u1VClkMax);
    _arPanelAttribute[0].u1VClkMin = rFlashTable.u1VClkMin;
    CHECK_ZERO(_arPanelAttribute[0].u1VClkMin);

	_arPanelAttribute[0].u2HPosition = rFlashTable.u2HPosition ;
    CHECK_ZERO(_arPanelAttribute[0].u2HPosition);
    _arPanelAttribute[0].u2VPosition = rFlashTable.u2VPosition;
    CHECK_ZERO(_arPanelAttribute[0].u2VPosition);
	
    _arPanelAttribute[0].u1HSyncWidth = rFlashTable.u1HSyncWidth;
    CHECK_ZERO(_arPanelAttribute[0].u1HSyncWidth);
    _arPanelAttribute[0].u1VSyncWidth = rFlashTable.u1VSyncWidth;
    CHECK_ZERO(_arPanelAttribute[0].u1VSyncWidth);
    
    _arPanelAttribute[0].u4ControlWord = rFlashTable.u4ControlWord;

	Printf("rFlashTable.u1VSyncWidth = 0x%0x,rFlashTable.u4ControlWord = 0x%0x,rFlashTable.u2LVDS_Amplitude = 0x%0x \n",rFlashTable.u1VSyncWidth,rFlashTable.u4ControlWord,rFlashTable.u2LVDS_Amplitude);
    _arPanelAttribute[0].u2LVDS_Amplitude= rFlashTable.u2LVDS_Amplitude;
	_arPanelAttribute[0].u2LVDS_SpreadRange= rFlashTable.u2LVDS_SpreadRange;
    _arPanelAttribute[0].u2LVDS_ModulationRate= rFlashTable.u2LVDS_ModulationRate;
    _arPanelAttribute[0].u2DRAM_SpreadRange= rFlashTable.u2DRAM_SpreadRange;
    _arPanelAttribute[0].u2DRAM_SpreadModulationRate= rFlashTable.u2DRAM_SpreadModulationRate;
	
    _arPanelAttribute[0].u1BacklightHigh = rFlashTable.u1BacklightHigh;
    _arPanelAttribute[0].u1BacklightMiddle = rFlashTable.u1BacklightMiddle;
    _arPanelAttribute[0].u1BacklightLow = rFlashTable.u1BacklightLow;
	
    _arPanelAttribute[0].u4DimingFreq60Hz = rFlashTable.u4DimingFreq60Hz;
    _arPanelAttribute[0].u4DimingFreq50Hz = rFlashTable.u4DimingFreq50Hz;	
    _arPanelAttribute[0].u4DimingFreq48Hz = rFlashTable.u4DimingFreq48Hz;	
	
    _arPanelAttribute[0].u1LvdsOnDalay = rFlashTable.u1LvdsOnDalay;
    _arPanelAttribute[0].u1BacklightOnDelay = rFlashTable.u1BacklightOnDelay;
    _arPanelAttribute[0].u1BacklightOffDelay = rFlashTable.u1BacklightOffDelay;
    _arPanelAttribute[0].u1LvdsOffDalay = rFlashTable.u1LvdsOffDalay;
    //_arPanelAttribute[0].u2MaxOverscan = rFlashTable.u2MaxOverscan;

    u4gbDitherMode = 0x8000 | rFlashTable.u4Reserved0; //for Dither

    _arPanelAttribute[0].u4ControlWord2= rFlashTable.u4ControlWord2;

    SelectPanel(0);
#ifdef CC_EXTERNAL_LVDS_CHIP
		{
			PanelDataStructType rPanel;
			UINT8 bOutType;
			UINT8 bHSYNCP;
			UINT8 bVSYNCP;
			UINT8 bDataClk;
			UINT8 bDataEn;
			UINT8 bMSBOnOff;
	
			if (rFlashTable.u2PanelHeight == 768)
			{
				rPanel.PanelType_R = 0; // WXGA/768p = 0
			}
			else
			{
				rPanel.PanelType_R = 1; // WUXGA/1080p = 1
			}
			if (LVDS_OUTPUT_PORT == SINGLE_PORT)
			{
				rPanel.PanelPixPerClk_R = 0; // single
			}
			else
			{
				rPanel.PanelPixPerClk_R = 1; // dule
			}
			switch (DISP_BIT)
			{
				case DISP_30BIT:
					rPanel.PanelDepth_R = 10; // 10 bit
					break;
	
				case DISP_24BIT:
					rPanel.PanelDepth_R = 8; // 8 bit
					break;
	
				default:
					rPanel.PanelDepth_R = 6; // 6 bit
					break;
			}
			rPanel.PanelWidth_R = rFlashTable.u2PanelWidth;
			rPanel.PanelHeight_R = rFlashTable.u2PanelHeight;
			rPanel.PanelMinHTotal_R = rFlashTable.u2HTotalMin;
			rPanel.PanelMinHSyncWidth_R = rFlashTable.u1HSyncWidth;
			rPanel.PanelMinHSyncBackPorch_R = HSYNCLEN_TYPI - rFlashTable.u1HSyncWidth - rFlashTable.u2HPosition;
			rPanel.PanelTypVTotal_R = rFlashTable.u2VTotal60Hz;
			rPanel.PanelMinVSyncWidth_R = rFlashTable.u1VSyncWidth;
			rPanel.PanelMinVSyncBackPorch_R = rFlashTable.u1VSyncWidth + rFlashTable.u2VPosition + 1;
	
        #if (HSYNC_POLARITY == HSYNC_LOW)
			{
				bHSYNCP = 1;
			}
        #else
			{
				bHSYNCP = 0;
			}
        #endif
	
        #if (VSYNC_POLARITY == VSYNC_LOW)
			{
				bVSYNCP = 1;
			}
        #else
			{
				bVSYNCP = 0;
			}
        #endif
	
			bDataClk = 0;
			bDataEn = 0;
	
			if (LVDS_DISP_FORMAT == LVDS_MSB_SW_ON) // NS
			{
				bMSBOnOff = 1;
			}
			else // JEIDA
			{
				bMSBOnOff = 0;
			}
			rPanel.PanelOutputType_R = (bVSYNCP << 7) | (bHSYNCP << 6) | (bDataClk << 5) | (bDataEn << 4) | (bMSBOnOff << 3);
	
			IMpac3P_SetPanelParam(&rPanel, sizeof(rPanel));
			IMpac3P_SetPower(1);
		}
#endif /* CC_EXTERNAL_LVDS_CHIP */
	return TRUE;
}
#endif

void Flash_SelectPanel(UINT32 u4Index)
{
	UINT32 offset  = 0;
	if(u4Index > 1)
	{
		u4Index  -= 1;
	}
#if defined(SUPPORT_FLASH_PANEL_CHPDP)	// only support one panel data
	u4Index  = 0;
#endif
    _u4EepromPanelIndex = u4Index;
	offset = (u4Index * DRVCUST_PanelGet(ePanelFlashTableSize) + (u4Index + 1)*sizeof(tagPanelTblHead)) + sizeof(tagPanelFileHead);

	#ifndef SUPPORT_VESTEL_FLASH_PANEL
		#ifndef CC_UBOOT
	    if (!_PanelLoadTableFromFlash(offset))
	    {
	    	Printf("===========================Load panel table from FLASH failed\n");
	        SelectPanel(PANEL_INVALID);
	    }
	    #endif
    #else
    	#ifndef CC_MTK_LOADER
		if (!_PanelLoadTableFromNOR(offset))
		{
	    	Printf("===========================Load panel table from NOR failed\n");
	        SelectPanel(PANEL_INVALID);
	    }
		#endif
    #endif

	Printf("===========================Load panel table from FLASH Finished\n");
}

#if !defined(CC_MTK_LOADER) || !defined(CC_LOADER_SHRINK_NORMAL)  //loader shrink
//Interface of getting Ptf Version, provided to ICE.
UINT8 bApiGetPtfVersion(UINT8 *pPtfInfo)
{
	PTF_INFO *prPtfInfo;
	UINT32 dwPtfOffset;
		
	prPtfInfo = (PTF_INFO *) pPtfInfo;
	
	if(prPtfInfo == NULL)
	{
            Printf("Pointer is Null, Get Ptf info fail !\n");
            return (SV_FALSE);
	}

	dwPtfOffset = (_u4EepromPanelIndex-1) * (sizeof(tagPanelTblHead) + DRVCUST_PanelGet(ePanelFlashTableSize)) + sizeof(tagPanelFileHead);
#ifndef SUPPORT_VESTEL_FLASH_PANEL
	#ifdef CC_MTK_LOADER
		Loader_ReadNandFlash(DRVCUST_PanelGet(ePanelFlashTablePartitionID), (UINT32)0, (VOID*) &aFlashPanelFileHead, sizeof(tagPanelFileHead));
		Loader_ReadNandFlash(DRVCUST_PanelGet(ePanelFlashTablePartitionID), (UINT32)dwPtfOffset, (VOID*) &aFlashPanelTblHead, sizeof(tagPanelTblHead));
	#else
		STORG_SyncNandRead(DRVCUST_PanelGet(ePanelFlashTablePartitionID), (UINT32)0, (VOID*)&aFlashPanelFileHead, sizeof(tagPanelFileHead));
		STORG_SyncNandRead(DRVCUST_PanelGet(ePanelFlashTablePartitionID), (UINT32)dwPtfOffset, (VOID*)&aFlashPanelTblHead, sizeof(tagPanelTblHead));
	#endif	
#else
	#ifndef CC_MTK_LOADER
		Panel_ReadNOR((UINT32)0, (VOID*) &aFlashPanelFileHead, sizeof(tagPanelFileHead));
		Panel_ReadNOR((UINT32)dwPtfOffset, (VOID*) &aFlashPanelTblHead, sizeof(tagPanelTblHead));
	#endif
#endif
    x_strcpy((CHAR *) (prPtfInfo->bPtfVer), (CHAR *) (aFlashPanelFileHead.bPtblXmlVer));
	x_strcpy((CHAR *) (prPtfInfo->bPtfPanelName), (CHAR *) (aFlashPanelTblHead.bScript));

	Printf("bPtfVer = %s, bPtfPanelName = %s\n", prPtfInfo->bPtfVer, prPtfInfo->bPtfPanelName);
	
	return (SV_TRUE);	
}
#endif
UINT16 wGetFlashPanel_Number(void)
{
	UINT8   aui1_buf[2];
	UINT16 wPanelNumber = 0;
    UINT8*  pui1_buf = NULL;
    pui1_buf  = (UINT8 *)&aui1_buf;
#ifndef SUPPORT_VESTEL_FLASH_PANEL
	#ifndef CC_UBOOT
		#ifdef CC_MTK_LOADER
    		Loader_ReadNandFlash(DRVCUST_PanelGet(ePanelFlashTablePartitionID), (UINT32)6, (VOID*) pui1_buf, 2);
		#else
 			STORG_SyncNandRead(DRVCUST_PanelGet(ePanelFlashTablePartitionID), (UINT32)6, (VOID*)pui1_buf, 2);
		#endif	
	#endif	
#else
	#ifndef CC_MTK_LOADER
		Panel_ReadNOR((UINT32)6, (VOID*) pui1_buf, 2);
	#endif
#endif
	wPanelNumber = aui1_buf[0] + (aui1_buf[1]<<8);	
	Printf("wPanelNumber == %d \n",wPanelNumber);
	return wPanelNumber;
}


#else
static BOOL _PanelLoadTableFromFlash(UINT32 u4Offset)
{
#if defined(CC_NAND_ENABLE) //by xk .shi
	PANEL_FLASH_ATTRIBUTE_T rFlashTable;
#ifndef CC_NAND_LOADER
	Printf("Load panel table from flash, address=0x%x\n", u4Offset);
	// FIXME!!
	// Need to replace with new interface of NOR flash access

	x_memcpy((void*)&rFlashTable, (void*)u4Offset, sizeof(PANEL_FLASH_ATTRIBUTE_T));
#else
	// FIXME!!
	// Need to replace with new interface of NAND flash access

	UINT64  u8Offset = (UINT64)u4Offset;
	UINT8   aui1_buf[sizeof(PANEL_FLASH_ATTRIBUTE_T)+NAND_PAGE_SIZE*2];

	/* must be aligned */
	UINT64  u8_algin_bytes;
	UINT32  ui4_read_size;  /* should be aligned */
	UINT8*  pui1_buf = NULL;
	UINT8   ui1_idx = 0;


	u8_algin_bytes = u8Offset % NAND_PAGE_SIZE;

	ui4_read_size = sizeof(PANEL_FLASH_ATTRIBUTE_T)+(UINT32)u8_algin_bytes;
	ui4_read_size += NAND_PAGE_SIZE - (ui4_read_size % NAND_PAGE_SIZE);

	pui1_buf  = (UINT8 *)&aui1_buf;
	//if (u8_algin_bytes != 0)
	{
	    while (ui4_read_size > 0)
	    {
	        NAND_Read(u8Offset-u8_algin_bytes+ui1_idx*NAND_PAGE_SIZE, 
	                 (UINT32)pui1_buf+ui1_idx*NAND_PAGE_SIZE, 
	                  NAND_PAGE_SIZE);
	        ui1_idx++;
	        ui4_read_size -= NAND_PAGE_SIZE;
	    }
	    x_memcpy(&rFlashTable, pui1_buf+(UINT32)u8_algin_bytes, sizeof(PANEL_FLASH_ATTRIBUTE_T));
	}
#endif
	_arPanelAttribute[0].u2PanelWidth = rFlashTable.u2PanelWidth;
	CHECK_ZERO(_arPanelAttribute[0].u2PanelWidth);
	_arPanelAttribute[0].u2PanelHeight = rFlashTable.u2PanelHeight;
	CHECK_ZERO(_arPanelAttribute[0].u2PanelHeight);
	_arPanelAttribute[0].u4PixelClkMax = rFlashTable.u4PixelClkMax;
	CHECK_ZERO(_arPanelAttribute[0].u4PixelClkMax);
	_arPanelAttribute[0].u4PixelClk60Hz = rFlashTable.u4PixelClk60Hz;
	CHECK_ZERO(_arPanelAttribute[0].u4PixelClk60Hz);
	_arPanelAttribute[0].u4PixelClk50Hz = rFlashTable.u4PixelClk50Hz;
	CHECK_ZERO(_arPanelAttribute[0].u4PixelClk50Hz);
	_arPanelAttribute[0].u4PixelClkMin = rFlashTable.u4PixelClkMin;
	CHECK_ZERO(_arPanelAttribute[0].u4PixelClkMin);

	_arPanelAttribute[0].u2HTotalMax = rFlashTable.u2HTotalMax;
	CHECK_ZERO(_arPanelAttribute[0].u2HTotalMax);
	_arPanelAttribute[0].u2HTotal60Hz = rFlashTable.u2HTotal60Hz;
	CHECK_ZERO(_arPanelAttribute[0].u2HTotal60Hz);
	_arPanelAttribute[0].u2HTotal50Hz = rFlashTable.u2HTotal50Hz;
	CHECK_ZERO(_arPanelAttribute[0].u2HTotal50Hz);
	_arPanelAttribute[0].u2HTotalMin = rFlashTable.u2HTotalMin;
	CHECK_ZERO(_arPanelAttribute[0].u2HTotalMin);

	_arPanelAttribute[0].u2VTotalMax = rFlashTable.u2VTotalMax;
	CHECK_ZERO(_arPanelAttribute[0].u2VTotalMax);
	_arPanelAttribute[0].u2VTotal60Hz = rFlashTable.u2VTotal60Hz;
	CHECK_ZERO(_arPanelAttribute[0].u2VTotal60Hz);
	_arPanelAttribute[0].u2VTotal50Hz = rFlashTable.u2VTotal50Hz;
	CHECK_ZERO(_arPanelAttribute[0].u2VTotal50Hz);
	_arPanelAttribute[0].u2VTotalMin = rFlashTable.u2VTotalMin;
	CHECK_ZERO(_arPanelAttribute[0].u2VTotalMin);

	_arPanelAttribute[0].u1VClkMax = rFlashTable.u1VClkMax;
	CHECK_ZERO(_arPanelAttribute[0].u1VClkMax);
	_arPanelAttribute[0].u1VClkMin = rFlashTable.u1VClkMin;
	CHECK_ZERO(_arPanelAttribute[0].u1VClkMin);

	_arPanelAttribute[0].u1HSyncWidth = rFlashTable.u1HSyncWidth;
	CHECK_ZERO(_arPanelAttribute[0].u1HSyncWidth);
	_arPanelAttribute[0].u1VSyncWidth = rFlashTable.u1VSyncWidth;
	CHECK_ZERO(_arPanelAttribute[0].u1VSyncWidth);
	_arPanelAttribute[0].u2HPosition = rFlashTable.u2HPosition << 4;
	CHECK_ZERO(_arPanelAttribute[0].u2HPosition);
	_arPanelAttribute[0].u2VPosition = rFlashTable.u2VPosition << 3;
	CHECK_ZERO(_arPanelAttribute[0].u2VPosition);
	_arPanelAttribute[0].u2MaxOverscan = (UINT16)rFlashTable.u1MaxOverscan1;
	_arPanelAttribute[0].u2MaxOverscan |= (UINT16)rFlashTable.u1MaxOverscan2 << 8;
	_arPanelAttribute[0].u4ControlWord = (UINT32)rFlashTable.u1ControlWord1;
	_arPanelAttribute[0].u4ControlWord |= (UINT32)rFlashTable.u1ControlWord2 << 8;
	_arPanelAttribute[0].u4ControlWord |= (UINT32)rFlashTable.u4Reserved8 << 16;	

	_arPanelAttribute[0].u1LvdsOnDalay = rFlashTable.u1LvdsOnDalay;
	_arPanelAttribute[0].u1BacklightOnDelay = rFlashTable.u1BacklightOnDelay;
	_arPanelAttribute[0].u1BacklightOffDelay = rFlashTable.u1BacklightOffDelay;
	_arPanelAttribute[0].u1LvdsOffDalay = rFlashTable.u1LvdsOffDalay;

	SelectPanel(0);

#ifdef CC_EXTERNAL_LVDS_CHIP
	{
	    PanelDataStructType rPanel;
	    UINT8 bOutType;
	    UINT8 bHSYNCP;
	    UINT8 bVSYNCP;
	    UINT8 bDataClk;
	    UINT8 bDataEn;
	    UINT8 bMSBOnOff;

	    if (rFlashTable.u2PanelHeight == 768)
	    {
	        rPanel.PanelType_R = 0; // WXGA/768p = 0
	    }
	    else
	    {
	        rPanel.PanelType_R = 1; // WUXGA/1080p = 1
	    }
	    if (LVDS_OUTPUT_PORT == SINGLE_PORT)
	    {
	        rPanel.PanelPixPerClk_R = 0; // single
	    }
	    else
	    {
	        rPanel.PanelPixPerClk_R = 1; // dule
	    }
	    switch (DISP_BIT)
	    {
	        case DISP_30BIT:
	            rPanel.PanelDepth_R = 10; // 10 bit
	            break;

	        case DISP_24BIT:
	            rPanel.PanelDepth_R = 8; // 8 bit
	            break;

	        default:
	            rPanel.PanelDepth_R = 6; // 6 bit
	            break;
	    }
	    rPanel.PanelWidth_R = rFlashTable.u2PanelWidth;
	    rPanel.PanelHeight_R = rFlashTable.u2PanelHeight;
	    rPanel.PanelMinHTotal_R = rFlashTable.u2HTotalMin;
	    rPanel.PanelMinHSyncWidth_R = rFlashTable.u1HSyncWidth;
	    rPanel.PanelMinHSyncBackPorch_R = HSYNCLEN_TYPI - rFlashTable.u1HSyncWidth - rFlashTable.u2HPosition;
	    rPanel.PanelTypVTotal_R = rFlashTable.u2VTotal60Hz;
	    rPanel.PanelMinVSyncWidth_R = rFlashTable.u1VSyncWidth;
	    rPanel.PanelMinVSyncBackPorch_R = rFlashTable.u1VSyncWidth + rFlashTable.u2VPosition + 1;

	    #if (HSYNC_POLARITY == HSYNC_LOW)
	    {
	        bHSYNCP = 1;
	    }
	    #else
	    {
	        bHSYNCP = 0;
	    }
	    #endif

	    #if (VSYNC_POLARITY == VSYNC_LOW)
	    {
	        bVSYNCP = 1;
	    }
	    #else
	    {
	        bVSYNCP = 0;
	    }
	    #endif

	    bDataClk = 0;
	    bDataEn = 0;

	    if (LVDS_DISP_FORMAT == LVDS_MSB_SW_ON) // NS
	    {
	        bMSBOnOff = 1;
	    }
	    else // JEIDA
	    {
	        bMSBOnOff = 0;
	    }
	    rPanel.PanelOutputType_R = (bVSYNCP << 7) | (bHSYNCP << 6) | (bDataClk << 5) | (bDataEn << 4) | (bMSBOnOff << 3);

	    IMpac3P_SetPanelParam(&rPanel, sizeof(rPanel));
	    IMpac3P_SetPower(1);
	}
#endif /* CC_EXTERNAL_LVDS_CHIP */
#endif
	return TRUE;
}

#endif
#endif

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

PANEL_ATTRIBUTE_SET_TMPL(PanelWidth, u2PanelWidth)
PANEL_ATTRIBUTE_SET_TMPL(PanelHeight, u2PanelHeight)
PANEL_ATTRIBUTE_SETGET_TMPL(PixelClkMax, u4PixelClkMax)
#if !SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET 
PANEL_ATTRIBUTE_SETGET_TMPL(PixelClk60Hz, u4PixelClk60Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(PixelClk50Hz, u4PixelClk50Hz)
#else
PANEL_ATTRIBUTE_SET_TMPL(PixelClk60Hz, u4PixelClk60Hz)
PANEL_ATTRIBUTE_SET_TMPL(PixelClk50Hz, u4PixelClk50Hz)
#endif
PANEL_ATTRIBUTE_SETGET_TMPL(PixelClkMin, u4PixelClkMin)
PANEL_ATTRIBUTE_SETGET_TMPL(HTotalMax, u2HTotalMax)
PANEL_ATTRIBUTE_SETGET_TMPL(HTotal60Hz, u2HTotal60Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(HTotal50Hz, u2HTotal50Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(HTotalMin, u2HTotalMin)
PANEL_ATTRIBUTE_SETGET_TMPL(VTotalMax, u2VTotalMax)
#if !SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET 
PANEL_ATTRIBUTE_SETGET_TMPL(VTotal60Hz, u2VTotal60Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(VTotal50Hz, u2VTotal50Hz)
#else
PANEL_ATTRIBUTE_SET_TMPL(VTotal60Hz, u2VTotal60Hz)
PANEL_ATTRIBUTE_SET_TMPL(VTotal50Hz, u2VTotal50Hz)
#endif
PANEL_ATTRIBUTE_SETGET_TMPL(VTotalMin, u2VTotalMin)
PANEL_ATTRIBUTE_SETGET_TMPL(VClkMax, u1VClkMax)
PANEL_ATTRIBUTE_SETGET_TMPL(VClkMin, u1VClkMin)
//#ifndef SUPPORT_PANEL_CUSTOMER_SPEC
PANEL_ATTRIBUTE_SETGET_TMPL(HSyncWidth, u1HSyncWidth)
PANEL_ATTRIBUTE_SETGET_TMPL(VSyncWidth, u1VSyncWidth)
//#else
//    PANEL_ATTRIBUTE_SET_TMPL(HSyncWidth, u1HSyncWidth)
//    PANEL_ATTRIBUTE_SET_TMPL(VSyncWidth, u1VSyncWidth)
//#endif
PANEL_ATTRIBUTE_SET_TMPL(HPosition60Hz, u2HPosition)
PANEL_ATTRIBUTE_SET_TMPL(VPosition, u2VPosition)
PANEL_ATTRIBUTE_SETGET_TMPL(ControlWord, u4ControlWord)
#ifdef SUPPORT_VESTEL_FLASH_PANEL
PANEL_ATTRIBUTE_SETGET_TMPL(DimmingFrequency60Hz, u4DimingFreq60Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(DimmingFrequency50Hz, u4DimingFreq50Hz)
#else
PANEL_ATTRIBUTE_SETGET_TMPL(DimmingFrequency60Hz, u2DimmingFrequency60Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(DimmingFrequency50Hz, u2DimmingFrequency50Hz)
PANEL_ATTRIBUTE_SETGET_TMPL(MaxOverscan, u2MaxOverscan)
#endif
PANEL_ATTRIBUTE_SETGET_TMPL(LvdsOnDalay, u1LvdsOnDalay)
PANEL_ATTRIBUTE_SETGET_TMPL(BacklightOnDelay, u1BacklightOnDelay)
PANEL_ATTRIBUTE_SETGET_TMPL(BacklightOffDelay, u1BacklightOffDelay)
PANEL_ATTRIBUTE_SETGET_TMPL(LvdsOffDalay, u1LvdsOffDalay)
#ifdef SUPPORT_VESTEL_FLASH_PANEL
	PANEL_ATTRIBUTE_SETGET_TMPL(ControlWord2, u4ControlWord2)
#endif



//----------------------------------------------------------------------------
/** DRVCUST_PanelQuery() return customization paramter.
 */
//----------------------------------------------------------------------------
INT32 DRVCUST_PanelQuery(QUERY_TYPE_T eQryType, UINT32 *pu4Data)
{
#if defined(CC_MTK_LOADER) && defined(CC_LOADER_SHRINK_QUERY) // loader shrink
//    Printf("eQryType = %d_________\n", eQryType);
    return -1;
#else
    ASSERT(pu4Data!=NULL);
    //    query_test();

    switch(eQryType)
    {
    // Panel
    case eDefaultPanelSelect:
        *pu4Data = DEFAULT_PANEL_SELECT;
        return 0;
    case eLvdsACtrlEven:
        *pu4Data = LVDS_A_CTRL_EVEN;
        return 0;
    case eLvdsACtrlOdd:
        *pu4Data = LVDS_A_CTRL_ODD;
        return 0;
    case eLvdsBCtrlEven:
        *pu4Data = LVDS_B_CTRL_EVEN;
        return 0;
    case eLvdsBCtrlOdd:
        *pu4Data = LVDS_B_CTRL_ODD;
        return 0;
    case eBackLightGpio:
#ifdef BACKLIGHT_GPIO
        *pu4Data = BACKLIGHT_GPIO;
        return 0;
#else
        return -1;
#endif
    case eBackLightOnVal:
#ifdef BACKLIGHT_ON_VAL
        *pu4Data = BACKLIGHT_ON_VAL;
        return 0;
#else
        return -1;
#endif
    case ePanelPowerGpio:
#ifdef PANELPOWER_GPIO
        *pu4Data = PANELPOWER_GPIO;
        return 0;
#else
        return -1;
#endif
    case ePanelPowerOnVal:
#ifdef PANELPOWER_ON_VAL
        *pu4Data = PANELPOWER_ON_VAL;
        return 0;
#else
        return -1;
#endif
    case ePanelDCRGpio:
#ifdef PANEL_DCR_GPIO
        *pu4Data = PANEL_DCR_GPIO;
        return 0;
#else
        return -1;
#endif /* PANEL_DCR_GPIO */

    case ePanelDCROnVal:
#ifdef PANEL_DCR_ON_VAL
        *pu4Data = PANEL_DCR_ON_VAL;
        return 0;
#else
        return -1;
#endif /* PANEL_DCR_ON_VAL */

    case ePanelPdpVsGpio:
#ifdef PANEL_PDP_VS_GPIO
        *pu4Data = PANEL_PDP_VS_GPIO;
        return 0;
#else
        return -1;
#endif /* PANEL_PDP_VS_GPIO */

    case ePanelPdpVsOnVal:
#ifdef PANEL_PDP_VS_ON_VAL
        *pu4Data = PANEL_PDP_VS_ON_VAL;
        return 0;
#else
        return -1;
#endif /* PANEL_PDP_VS_ON_VAL */

    case eLvdsMsbSw:
#ifdef LVDS_MSB_SW
        *pu4Data = LVDS_MSB_SW;
        return 0;
#else
        return -1;
#endif /* LVDS_MSB_SW */

    case ePanelPwmPrescalar:
#ifdef PWM_PRESCALAR
        *pu4Data = PWM_PRESCALAR;
        return 0;
#else
        return -1;
#endif /* PWM_PRESCALAR */

    case ePanelDisableDimming:
#ifdef PANEL_DISABLE_DIMMING
        *pu4Data = PANEL_DISABLE_DIMMING;
        return 0;
#else
        return -1;
#endif /* PANEL_DISABLE_DIMMING */

    case ePanelChannelSwap:
        *pu4Data = PANEL_CHANNEL_SWAP;
        return 0;

    case ePanelPwmPolarity:
#ifdef PWM_POLARITY
        *pu4Data = PWM_POLARITY;
        return 0;
#else
        return -1;
#endif /* PWM_POLARITY */
    case ePanelIndexFromEeprom:
#ifdef PANEL_INDEX_FROM_EEPROM
        *pu4Data = PANEL_INDEX_FROM_EEPROM;
        return 0;
#else
        return -1;
#endif /* PANEL_INDEX_FROM_EEPROM */

    case ePanelIndexOffset1:
#ifdef PANEL_INDEX_OFFSET1
        *pu4Data = PANEL_INDEX_OFFSET1;
        return 0;
#else
        return -1;
#endif /* PANEL_INDEX_OFFSET1 */

    case ePanelIndexOffset2:
#ifdef PANEL_INDEX_OFFSET2
        *pu4Data = PANEL_INDEX_OFFSET2;
        return 0;
#else
        return -1;
#endif /* PANEL_INDEX_OFFSET2 */

    case ePanelIndexOffset3:
#ifdef PANEL_INDEX_OFFSET3
        *pu4Data = PANEL_INDEX_OFFSET3;
        return 0;
#else
        return -1;
#endif /* PANEL_INDEX_OFFSET3 */
    case ePanelIndexFromGpioNum:
        *pu4Data = PANEL_INDEX_FROM_GPIO_NUM;
        return 0;

    case ePanelIndexFromGpioIdx:
        *pu4Data = (UINT32)PANEL_INDEX_FROM_GPIO_IDX;
        return 0;

    case ePanelIndexFromGpioMap:
        *pu4Data = (UINT32)PANEL_INDEX_FROM_GPIO_MAP;
        return 0;

    case ePanelLvds8bitPinMap:
        *pu4Data = (UINT32)PANEL_LVDS_8BIT_PIN_MAP;
        return 0;

    case ePanelLvds10bitPinMap:
        *pu4Data = (UINT32)PANEL_LVDS_10BIT_PIN_MAP;
        return 0;

    case ePanelLvds10bitPNSwap:
        *pu4Data = (UINT32)PANEL_LVDS_10BIT_PN_SWAP;
        return 0;


    case eBacklightHigh:
#ifdef BACKLIGHT_HIGH_PWM
        *pu4Data = BACKLIGHT_HIGH_PWM;
        return 0;
#else
        return -1;
#endif /* BACKLIGHT_HIGH_PWM */

    case eBacklightMiddle:
#ifdef BACKLIGHT_MIDDLE_PWM
        *pu4Data = BACKLIGHT_MIDDLE_PWM;
        return 0;
#else
        return -1;
#endif /* BACKLIGHT_MIDDLE_PWM */

    case eBacklightLow:
#ifdef BACKLIGHT_LOW_PWM
        *pu4Data = BACKLIGHT_LOW_PWM;
        return 0;
#else
        return -1;
#endif /* BACKLIGHT_LOW_PWM */

    case eBackLightGpio2:
#ifdef BACKLIGHT_GPIO2
        *pu4Data = BACKLIGHT_GPIO2;
        return 0;
#else
        return -1;
#endif /* BACKLIGHT_GPIO2 */

    case eSpreadSpectrumPermillage:
#ifdef SPREAD_SPECTRUM_PERMILLAGE
        *pu4Data = SPREAD_SPECTRUM_PERMILLAGE;
        return 0;
#else
        return -1;
#endif /* SPREAD_SPECTRUM_PERMILLAGE */

    case eTimerDimmingFreq:
#ifdef TIMER_DIMMING_FREQ
        *pu4Data = TIMER_DIMMING_FREQ;
        return 0;
#else
        return -1;
#endif /* TIMER_DIMMING_FREQ */

    case eDimmingFreq60:
        *pu4Data = DIMMING_FREQ_60;
        return 0;

    case eDimmingFreq50:
        *pu4Data = DIMMING_FREQ_50;
        return 0;

    case ePanelTTLOutput:
        *pu4Data = PANEL_TTL_OUTPUT;
        return 0;

    case ePanelFlashTableSize:
        *pu4Data = PANEL_FLASH_TABLE_SIZE;
        return 0;
#ifndef SUPPORT_VESTEL_FLASH_PANEL
    case ePanelFlashTableAddress:
        *pu4Data = PANEL_FLASH_TABLE_ADDRESS;
        return 0;
#endif
//#ifdef SUPPORT_VESTEL_FLASH_PANEL
	case ePanelFlashTablePartitionID:
		*pu4Data = PANEL_FLASH_TABLE_PARTITION_ID;
		return 0;
//#endif
    case ePanelEepromToFlashIndexMap:
        *pu4Data = (UINT32)PANEL_EEPROM_TO_FLASH_INDEX_MAP;
        return 0;

    case eLvdsDrivingCurrent:
#ifdef LVDS_DRIVING_CURRENT
        *pu4Data = LVDS_DRIVING_CURRENT;
        return 0;
#else
        return -1;
#endif /* LVDS_DRIVING_CURRENT */

    case ePanelBacklightPwmPort:
#ifdef PANEL_BACKLIGHT_PWM_PORT
		*pu4Data = PANEL_BACKLIGHT_PWM_PORT;
#else
		*pu4Data = 2; // Default using PWM2 as backlight control.
#endif /* PANEL_BACKLIGHT_PWM_PORT */
		return 0;

    case ePanelBacklightGPIO:
#ifdef PANEL_BACKLIGHT_PWM_GPIO
		*pu4Data = PANEL_BACKLIGHT_PWM_GPIO;
#else
		*pu4Data = 0; // Default using PWM output
#endif /* PANEL_BACKLIGHT_PWM_GPIO */
    return 0;

	case eFlagBacklightBoostCtrl:
#ifdef SUPPORT_BACKLIGHT_BOOST_CTRL
		*pu4Data = (UINT32)SUPPORT_BACKLIGHT_BOOST_CTRL;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_BACKLIGHT_BOOST_CTRL
		return 0;
	case eFlagBacklightBoostPort:
#ifdef PANEL_BACKLIGHT_BOOST_PORT
		*pu4Data = (UINT32)PANEL_BACKLIGHT_BOOST_PORT;
#else
		*pu4Data = (UINT32)1;
#endif // #ifdef PANEL_BACKLIGHT_BOOST_PORT
		return 0;
	case eFlagBacklightBoostInvert:
#ifdef SUPPORT_BACKLIGHT_BOOST_INVERT
		*pu4Data = (UINT32)SUPPORT_BACKLIGHT_BOOST_INVERT;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_BACKLIGHT_BOOST_INVERT
		return 0;
	case eFlagBacklightDimmingLock:
#ifdef SUPPORT_BACKLIGHT_DIMMING_LOCK
		*pu4Data = (UINT32)SUPPORT_BACKLIGHT_DIMMING_LOCK;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_BACKLIGHT_DIMMING_LOCK
		return 0;
	case eFlagBacklightDimmingInvert:
#ifdef SUPPORT_BACKLIGHT_DIMMING_INVERT
		*pu4Data = (UINT32)SUPPORT_BACKLIGHT_DIMMING_INVERT;
#else
		*pu4Data = (UINT32)0;
#endif // #ifdef SUPPORT_BACKLIGHT_DIMMING_INVERT
		return 0;
    case ePanelPixelShiftHMax:
        *pu4Data = PANEL_PIXEL_SHIFT_H_MAX;
        return 0;
    case ePanelPixelShiftVMax:
        *pu4Data = PANEL_PIXEL_SHIFT_V_MAX;
        return 0;

    case ePanelBitSelectCtrlGpio:
#ifdef PANEL_BIT_SELECT_CTRL_GPIO
        *pu4Data = PANEL_BIT_SELECT_CTRL_GPIO;
        return 0;
#else
        *pu4Data = (UINT32)-1;
        return -1;
#endif

        case ePanelLvdsSelectCtrlGpio:
#ifdef PANEL_LVDS_SELECT_CTRL_GPIO
            *pu4Data = PANEL_LVDS_SELECT_CTRL_GPIO;
            return 0;
#else
            *pu4Data = (UINT32)-1;
            return -1;
#endif

        case ePanel242530pOutput:
            #ifdef PANEL_SUPPORT_24_25_30_OUTPUT           
            *pu4Data = (UINT32)PANEL_SUPPORT_24_25_30_OUTPUT;
            #else
            *pu4Data = (UINT32)0;
            #endif
            return 0;


        case eDDDSErrorLimit:
            #ifdef DDDS_ERROR_LIMIT
            *pu4Data = DDDS_ERROR_LIMIT;
            return 0;
            #else
            *pu4Data = (UINT32)-1;
            return -1;
            #endif

        case eDDDSLockOnOthers:
            #ifdef DDDS_LOCK_OTHERS
            *pu4Data = DDDS_LOCK_OTHERS;
            #else
            *pu4Data = (UINT32)1;
            #endif
            return 0;

		case eDISP_RLVDSEncodeLink:
		#ifdef DISP_RLVDSENCODE_PORT
			*pu4Data = DISP_RLVDSENCODE_LINK;
			return 0;
		#else
			*pu4Data = (UINT32)-1;
			return -1;
		#endif
    default:
        return -1;
    }
#endif
}

UINT32 DRVCUST_PanelGet(QUERY_TYPE_T eQryType)
{
    UINT32 u4Ret = 0xffffffff;

    DRVCUST_PanelQuery(eQryType, &u4Ret);
    return u4Ret;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 IsLvdsMsbSwOn(void)
{
    UINT32 u4Value;
    if (DRVCUST_PanelQuery(eLvdsMsbSw, &u4Value) == 0)
    {
        return u4Value;
    }
    if (IS_COMPANION_CHIP_ON())
    {
        return LVDS_JEIDA;
    }
   return (PANEL_GetControlWord() & (LVDS_MSB_SW_ON | LVDS_SPECIAL_NS));
}


/**
* @brief Check if support 120Hz Panel
* @retval SV_TURE/SV_FALSE
*/
UINT8 fgIsSupport120HzPanel(void)
{
    if (PANEL_GetVClkMax() > 100)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 IsPwmLowPanelBright(void)
{
    UINT32 u4Value;
    if (DRVCUST_PanelQuery(ePanelPwmPolarity, &u4Value) == 0)
    {
        return u4Value;
    }
    return PANEL_GetControlWord() & PWM_LOW_PANEL_BRIGHT;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void SelectPanel(UINT32 u4Index)
{
    // read panel index from customer define
    if (u4Index == PANEL_INVALID)
    {
        u4Index = DRVCUST_PanelGet(eDefaultPanelSelect);
    }
    if (u4Index != PANEL_INVALID)
    {
        _u4PanelIndex = u4Index;

#ifndef CC_MULTI_PANEL
        _u4PanelIndex = 0;
#endif

        _prPanelAttribute = &_arPanelAttribute[_u4PanelIndex];
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

CHAR* GetCurrentPanelName(void)
{
#if !defined(CC_MTK_LOADER) || !defined(CC_LOADER_SHRINK_NORMAL)  //loader shrink
    if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
    {
        return GetPanelName(PANEL_GetEepromPanelIndex());
    }
    else
    {
        return GetPanelName(GetCurrentPanelIndex());
    }
#endif
    return "UNKNOWN";
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 GetCurrentPanelIndex(void)
{
    if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
    {
        return PANEL_GetEepromPanelIndex();
    }
    else
    {
	    return _u4PanelIndex;
    }
}

UINT32 GetFlagPanelIdInEeprom(void)
{
    return _fgPanelIdInEeprom;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void DumpPanelAttribute(UINT32 u4Flag)
{
#ifndef CC_MTK_LOADER
    UINT32 u4Size = sizeof(_arPanelAttribute) / sizeof(PANEL_ATTRIBUTE_T);
    UINT32 i = 0;
    PANEL_ATTRIBUTE_T* prTbl;

    if ((u4Flag & PANEL_DUMP_ALL) == 0)
    {
        i = _u4PanelIndex;
        u4Size = _u4PanelIndex + 1;
    }
    for (; i < u4Size; i++)
    {
        prTbl = &_arPanelAttribute[i];
        if ((DRVCUST_PanelGet(ePanelFlashTableSize) > 0) && (i == 0))
        {
	       #ifdef SUPPORT_VESTEL_FLASH_PANEL
            Printf("---------- Panel[%3d] %s %dx%d ----------\n",
	        GetCurrentPanelIndex(), GetPanelName(GetCurrentPanelIndex()), prTbl->u2PanelWidth, prTbl->u2PanelHeight);
		   #else
	        Printf("---------- Panel[%3d] %s %dx%d ----------\n",
                   PANEL_GetEepromPanelIndex(), GetPanelName(PANEL_GetEepromPanelIndex()), prTbl->u2PanelWidth, prTbl->u2PanelHeight);
	       #endif
        }
        else
        {
            Printf("---------- Panel[%3d] %s %dx%d ----------\n",
                   i, GetPanelName(i), prTbl->u2PanelWidth, prTbl->u2PanelHeight);
        }
        if ((u4Flag & PANEL_DUMP_TITLE) == PANEL_DUMP_TITLE)
        {
            continue;
        }
        Printf("PixelClk[Max=%d, 60Hz=%d, 50Hz=%d, Min=%d]\n",
               prTbl->u4PixelClkMax, prTbl->u4PixelClk60Hz,
               prTbl->u4PixelClk50Hz, prTbl->u4PixelClkMin);
        Printf("HTotal[Max=%d, 60Hz=%d, 50Hz=%d, Min=%d] HSyncLen[60Hz=%d, 50Hz=%d]\n",
               prTbl->u2HTotalMax, prTbl->u2HTotal60Hz,
               prTbl->u2HTotal50Hz, prTbl->u2HTotalMin,
               PANEL_GetHSyncLen60Hz(), PANEL_GetHSyncLen50Hz());
        Printf("VTotal[Max=%d, 60Hz=%d, 50Hz=%d, Min=%d] VClk[Max=%d, Min=%d]\n",
               prTbl->u2VTotalMax, prTbl->u2VTotal60Hz,
               prTbl->u2VTotal50Hz, prTbl->u2VTotalMin,
               prTbl->u1VClkMax, prTbl->u1VClkMin);
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        Printf("HSyncWidth60Hz=%d, ", PANEL_GetHSyncWidthByTiming(SV_DCLK_60HZ));
        Printf("HSyncWidth50Hz=%d, ", PANEL_GetHSyncWidthByTiming(SV_DCLK_50HZ));
        Printf("VSyncWidth60Hz=%d, ", PANEL_GetVSyncWidthByTiming(SV_DCLK_60HZ));
        Printf("VSyncWidth50Hz=%d\n", PANEL_GetVSyncWidthByTiming(SV_DCLK_50HZ));
        #else
        Printf("HSyncWidth=%d, ", prTbl->u1HSyncWidth);
        Printf("VSyncWidth=%d, ", prTbl->u1VSyncWidth);
        #endif
        Printf("HPosition60Hz=0x%X, ", H_POS_60HZ);
        Printf("HPosition50Hz=0x%X, ", H_POS_50HZ);
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
        Printf("VPosition60Hz=%d, ", PANEL_GetVPositionByTiming(SV_DCLK_60HZ));
        Printf("VPosition50Hz=%d\n", PANEL_GetVPositionByTiming(SV_DCLK_50HZ));
        #else
        Printf("VPosition=%d\n", prTbl->u2VPosition);
        #endif        

        #ifdef SUPPORT_PANEL_48HZ
        Printf("-SUPPORT_48HZ ExtId[%d]- Pclk=%d, HTot48Hz=%d, VTot48Hz=%d\n"
               "HsWid48Hz=%d, VsWid48Hz=%d, HPos48Hz=0x%X, VPos48Hz=%d\n",
                PANEL_GetExtAttributeIndex(),
                PANEL_GetPixelClk48Hz(), PANEL_GetHTotal48Hz(), PANEL_GetVTotal48Hz(),
        #ifdef SUPPORT_PANEL_CUSTOMER_SPEC
                PANEL_GetHSyncWidthByTiming(SV_DCLK_48HZ), 
                PANEL_GetVSyncWidthByTiming(SV_DCLK_48HZ),
                PANEL_GetHPositionByTiming(SV_DCLK_48HZ),
                PANEL_GetVPositionByTiming(SV_DCLK_48HZ) 
        #else
                prTbl->u1HSyncWidth, prTbl->u1VSyncWidth,
                H_POS_60HZ, prTbl->u2VPosition
        #endif
                );
                
        #endif

		Printf("prTbl->u4ControlWord = 0x %x \n",prTbl->u4ControlWord);

        if (prTbl->u4ControlWord & LVDS_MSB_SW_ON)
        {
            Printf("LVDS_NS, ");
        }
        else if(prTbl->u4ControlWord & LVDS_SPECIAL_NS)
		{
            Printf("LVDS_SPECIAL_NS, ");
        }
        else
        {
            Printf("LVDS_JEIDA, ");
        }
        if (prTbl->u4ControlWord & LVDS_ODD_SW_ON)
        {
            Printf("LVDS_ODD_SW_ON, ");
        }
        else
        {
            Printf("LVDS_ODD_SW_OFF, ");
        }
        if (prTbl->u4ControlWord & DUAL_PORT)
        {
            Printf("DUAL_PORT, ");
        }
        else if (prTbl->u4ControlWord & FOUR_PORT)
        {
            Printf("FOUR_PORT, ");
        }
        else
        {
            Printf("SINGLE_PORT, ");
        }

        if ((prTbl->u4ControlWord & DISP_BIT_MSK) == DISP_30BIT)
        {
            Printf("DISP_30BIT, ");
        }
        else if ((prTbl->u4ControlWord & DISP_BIT_MSK) == DISP_24BIT)
        {
            Printf("DISP_24BIT\n");
        }
        else
        {
            Printf("DISP_18BIT\n");
        }
    #ifdef SUPPORT_VESTEL_FLASH_PANEL
		if (prTbl->u4ControlWord & LVDS_PNSWAP_ON)
        {
            Printf("LVDS_PNSWAP_ON, ");
        }
        else
        {
            Printf("LVDS_PNSWAP_OFF, ");
        }
        if (prTbl->u4ControlWord & LVDS_DESWAP_ON)
        {
            Printf("LVDS_DESWAP_ON, ");
        }
        else
        {
            Printf("LVDS_DESWAP_OFF, ");
        }
        if (prTbl->u4ControlWord & LVDS_HSYNCSWAP_ON)
        {
            Printf("LVDS_HSYNCSWAP_ON, ");
        }
        else
        {
            Printf("LVDS_HSYNCSWAP_OFF, ");
        }
        if (prTbl->u4ControlWord & LVDS_VSYNCSWAP_ON)
        {
            Printf("LVDS_VSYNCSWAP_ON, ");
        }
        else
        {
            Printf("LVDS_VSYNCSWAP_ON, ");
        }
        if (prTbl->u4ControlWord & LVDS_VFLIP_ON)
        {
            Printf("LVDS_VFLIP_ON, ");
        }
        else
        {
            Printf("LVDS_VFLIP_OFF, ");
        }
        if (prTbl->u4ControlWord & LVDS_HFLIP_ON)
        {
            Printf("LVDS_HFLIP_ON, ");
        }
        else
        {
            Printf("LVDS_HFLIP_OFF, ");
        }
        if (prTbl->u4ControlWord & PWM_LOCK_VSYNC_ON)
        {
            Printf("PWM_LOCK_VSYNC_ON, ");
        }
        else
        {
            Printf("PWM_LOCK_VSYNC_OFF, ");
        }
        if ((prTbl->u4ControlWord & PANEL_TYPE_MASK) == PANEL_TYPE_43)
        {
            Printf("PANEL_TYPE_43, ");
        }
        else if ((prTbl->u4ControlWord & PANEL_TYPE_MASK) == PANEL_TYPE_169)
        {
            Printf("PANEL_TYPE_169\n");
        }
        else
        {
            Printf("PANEL_TYPE_OTHER\n");
        }
        if (prTbl->u4ControlWord & LVDS_LINKTYPE_LVDS_ON)
        {
            Printf("LVDS_LINKTYPE_LVDS_ON, ");
        }
        else
        {
            Printf("LVDS_LINKTYPE_LVDS_OFF, ");
        }
        if (prTbl->u4ControlWord & LVDS_SUPPORT_48HZ_ON)
        {
            Printf("LVDS_SUPPORT_48HZ_ON, ");
        }
        else
        {
            Printf("LVDS_SUPPORT_48HZ_OFF, ");
        }
    #endif
        if (prTbl->u4ControlWord & WFB_PANEL_RESET_ON)
        {
            Printf("WFB_PANEL_RESET_ON, ");
        }
        else
        {
            Printf("WFB_PANEL_RESET_OFF, ");
        }
        if (prTbl->u4ControlWord & PWM_LOW_PANEL_BRIGHT)
        {
            Printf("PWM_LOW_PANEL_BRIGHT\n");
        }
        else
        {
            Printf("PWM_HIGH_PANEL_BRIGHT\n");
        }

        if(prTbl->u4ControlWord & LVDS_USE_INDEPENDENT_SETTING)
        {
		if (LVDS_A_INDEPENDENT_SETTING == 3)
        {
            Printf("LVDS_A_FROM_BO,");
        }
		else if (LVDS_A_INDEPENDENT_SETTING == 2)
        {
            Printf("LVDS_A_FROM_BE,");
        }
        else if (LVDS_A_INDEPENDENT_SETTING == 1)
        {
            Printf("LVDS_A_FROM_AO,");
        }
		else if (LVDS_A_INDEPENDENT_SETTING == 0)
		{
			Printf("LVDS_A_FROM_AE,");
		}

		if (LVDS_B_INDEPENDENT_SETTING == 3)
        {
            Printf("LVDS_B_FROM_BO,");
        }
        else if (LVDS_B_INDEPENDENT_SETTING == 2)
        {
            Printf("LVDS_B_FROM_BE,");
        }
        else if (LVDS_B_INDEPENDENT_SETTING == 1)
        {
            Printf("LVDS_B_FROM_AO,");
        }
		else if (LVDS_B_INDEPENDENT_SETTING == 0)
		{
			Printf("LVDS_B_FROM_AE,");
		}

		if (LVDS_C_INDEPENDENT_SETTING == 3)
        {
            Printf("LVDS_C_FROM_BO,");
        }
        else if (LVDS_C_INDEPENDENT_SETTING == 2)
        {
            Printf("LVDS_C_FROM_BE,");
        }
        else if (LVDS_C_INDEPENDENT_SETTING == 1)
        {
            Printf("LVDS_C_FROM_AO,");
        }
		else if (LVDS_C_INDEPENDENT_SETTING == 0)
		{
			Printf("LVDS_C_FROM_AE,");
		}

		if (LVDS_D_INDEPENDENT_SETTING == 3)
        {
            Printf("LVDS_D_FROM_BO,\n");
        }
        else if (LVDS_D_INDEPENDENT_SETTING == 2)
        {
            Printf("LVDS_D_FROM_BE,\n");
        }
        else if (LVDS_D_INDEPENDENT_SETTING == 1)
        {
            Printf("LVDS_D_FROM_AO,\n");
        }
		else if (LVDS_D_INDEPENDENT_SETTING == 0)
		{
			Printf("LVDS_D_FROM_AE,\n");
		}		
        }
		
        if(LVDS_DISP_3D==LVDS_DISP_3D_POLARIZED_LRLR)
        {
            Printf("3D_POLARIZED_LRLR, ");
        }
        else if(LVDS_DISP_3D==LVDS_DISP_3D_POLARIZED_RLRL)
        {
            Printf("3D_POLARIZED_RLRL, ");
        }
        else if(LVDS_DISP_3D==LVDS_DISP_3D_SHUTTER)
        {
            Printf("3D_SHUTTER, ");
        }
        else
        {
            Printf("3D_NA, ");
        }
		if(IS_OD_ENABLE)
        {
            Printf("OD_ENABLE,");
        }
        if(LVDS_DISP_SPLIT)
        {
            Printf("SPLIT_ON, ");
        }
        else
        {
            Printf("SPLIT_OFF, ");
        }

        if(IS_VBYONE)
        {
            Printf("VB1");
        }
        if(IS_MLVDS)
        {
            Printf("MLVDS");
        }
        if(IS_EPI)
        {
            Printf("EPI");
        }
        Printf("\n");

        if(IS_LVDS_HIGHSPEED)
        {
            Printf("LVDS_HIGHSPEED,");
        }
		
        if(IS_PANEL_WXGA240)
        {
            Printf("PANEL_WXGA240,");
        }

        if(IS_PANEL_PDP)
        {
            Printf("PANEL_PDP,");
        }		

        if(IS_PANEL_ASPECT_RATIO_21_9)
        {
            Printf("PANEL_ASPECT_RATIO_21_9,");
        }	

        if(IS_PANEL_REODER_SEQ_TO_HALF)
        {
            Printf("PANEL_REODER_SEQ_TO_HALF,");
        }	

        if(IS_FB_ENABLE)
        {
            Printf("FB_ENABLE,");
        }	

        if(IS_LBRB_3D_ON)
        {
            Printf("LBRB_3D_ON");
        }	

		Printf("\n");

        Printf("Backlight[High=0x%x, Middle=0x%x, Low=0x%x]\n",
               prTbl->u1BacklightHigh, prTbl->u1BacklightMiddle,
               prTbl->u1BacklightLow);
        Printf("DimFreq60Hz=%d, DimFreq50Hz=%d"
#ifdef SUPPORT_PANEL_48HZ
               ", DimFreq48Hz=%d"
#endif
               "\n",
#ifdef SUPPORT_VESTEL_FLASH_PANEL
		prTbl->u4DimingFreq60Hz, prTbl->u4DimingFreq50Hz
#else
               prTbl->u2DimmingFrequency60Hz, prTbl->u2DimmingFrequency50Hz
#endif
#ifdef SUPPORT_PANEL_48HZ
               ,PANEL_GetDimmingFrequency48Hz()
#endif
               );
        Printf("Delay[LvdsOn=%d0, BacklightOn=%d0, BacklightOff=%d0, LvdsOff=%d0]\n",
               prTbl->u1LvdsOnDalay, prTbl->u1BacklightOnDelay,
               prTbl->u1BacklightOffDelay, prTbl->u1LvdsOffDalay);
		 #ifdef SUPPORT_VESTEL_FLASH_PANEL
		 Printf("LVDS[Swing= %d, Range= %d, ModulationRate= %d],DRAM[Range= %d, ModulationRate= %d]\n",
				prTbl->u2LVDS_Amplitude, prTbl->u2LVDS_SpreadRange,
			    prTbl->u2LVDS_ModulationRate,prTbl->u2DRAM_SpreadRange,
			    prTbl->u2DRAM_SpreadModulationRate);
		 Printf("prTbl->u4Reserved0 = 0x %x \n",prTbl->u4Reserved0);
		 Printf("prTbl->u4ControlWord2 = 0x %x \n",prTbl->u4ControlWord2);
		 #else
        Printf("MaxOverscan=%d\n", prTbl->u2MaxOverscan);
		 #endif
    }
    Printf("\nCurrent panel setting is %s\n", GetCurrentPanelName());
    Printf("Backlight gpio=%d, turn on value=%d, ",
           DRVCUST_PanelGet(eBackLightGpio), DRVCUST_PanelGet(eBackLightOnVal));
    Printf("LVDS gpio=%d, turn on value=%d\n",
           DRVCUST_PanelGet(ePanelPowerGpio), DRVCUST_PanelGet(ePanelPowerOnVal));
    Printf("Pixel Shift=%d (%d x %d)\n", _fgPixelShiftEnable,
           PANEL_GetPanelWidth(), PANEL_GetPanelHeight());
#endif

}


UINT32 PANEL_TranslateEepromToFlashIndex(UINT32 u4EepromIndex)
{
    UINT32 u4Size, u4FlashIndex;
    UINT32* pu4Array;

    u4Size = DRVCUST_PanelGet(ePanelFlashTableSize);
    if ((pu4Array = (UINT32*)DRVCUST_PanelGet(ePanelEepromToFlashIndexMap)) !=  NULL)
    {
        for (u4FlashIndex = 0; u4FlashIndex < u4Size; u4FlashIndex++)
        {
            if (pu4Array[u4FlashIndex] == u4EepromIndex)
            {
                return u4FlashIndex;
            }
        }
        if (u4FlashIndex == u4Size)
        {
            return 0;
        }
    }
    return u4EepromIndex;
}

UINT32 PANEL_GetEepromPanelIndex(void)
{
    return _u4EepromPanelIndex;
}

//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------
 
void LoadPanelIndex(void)
{
    UINT8 au1Index[] = {PANEL_INVALID, PANEL_INVALID, PANEL_INVALID};
    QUERY_TYPE_T arKey[] = {ePanelIndexOffset1, ePanelIndexOffset2,
                            ePanelIndexOffset3};
    UINT32 u4Size = sizeof(arKey) / sizeof(QUERY_TYPE_T);
    UINT32 u4Value, u4ValidCounter = 0, i, j;
    UINT32 u4FinalIndex = PANEL_INVALID, i4Ret;
    UINT32 u4WriteIdxToEeprom = 0;

    if ((DRVCUST_PanelQuery(ePanelIndexFromEeprom, &u4Value) == 0) &&
        (u4Value == 1))
    {
        _fgPanelIdInEeprom = TRUE;

        // hack u4TotalNumber to pass IS_PANEL_INDEX_INVALID
        if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
        {
            u4TotalNumber = 0xff;
        }

	#ifndef SUPPORT_VESTEL_FLASH_PANEL
        // read configuration from eeprom
        for (i = 0; i < u4Size; i++)
        {
            if (DRVCUST_PanelQuery(arKey[i], &u4Value) == 0)
            {
                i4Ret = EEPROM_Read((UINT64)u4Value, (UINT32)&au1Index[i], 1);
                if ((i4Ret != 0) || IS_PANEL_INDEX_INVALID(au1Index[i]))
                {
                    au1Index[i] = PANEL_INVALID;
                }
                else
                {
                    u4ValidCounter++;
                }
            }
        }

        // decide valid panel index
        if (u4ValidCounter == 1)
        {
            for (i = 0; i < u4Size; i++)
            {
                if (au1Index[i] != PANEL_INVALID)
                {
                    u4FinalIndex = au1Index[i];
                    break;
                }
            }
        }
        else if (u4ValidCounter >= 2)
        {
            for (i = 0; i < u4Size; i++)
            {
                if (au1Index[i] == PANEL_INVALID)
                {
                    continue;
                }
                for (j = 0; j < u4Size; j++)
                {
                    if ((i != j) && (au1Index[i] == au1Index[j]))
                    {
                        u4FinalIndex = au1Index[i];
                        break;
                    }
                }
                if (u4FinalIndex != PANEL_INVALID)
                {
                    break;
                }
            }
        }

        // Here, u4FinalIndex is the EEPROM panel id value.
        // Check this is use gpio number.
#ifdef PANEL_INDEX_FLAG_OFFSET
        u4Value = 0;
        i4Ret = EEPROM_Read((UINT64)PANEL_INDEX_MAGIC, (UINT32)&u4Value, 1);
        if (i4Ret == 0)
        {
            if ((u4Value & 0xff) != 0x5a)
            {
                u4Value = 0;
                EEPROM_Write((UINT64)PANEL_INDEX_FLAG_OFFSET, (UINT32)&u4Value, 1);
                u4Value = 0x5a;
                EEPROM_Write((UINT64)PANEL_INDEX_MAGIC, (UINT32)&u4Value, 1);
            }
        }

        u4Value = 0;    // default flag is not in eeprom (gpio).
        i4Ret = EEPROM_Read((UINT64)PANEL_INDEX_FLAG_OFFSET, (UINT32)&u4Value, 1);
        if (i4Ret != 0) { u4FinalIndex = 0; } // if eeprom failed, force gpio directly.
        if (u4Value != 1) { u4FinalIndex = 0; } // if eeprom flag is not 1, force gpio directly.
#endif /* PANEL_INDEX_FLAG_OFFSET */
	#else
		u4FinalIndex = u4VestelFlashPNLIndex;
	#endif /* SUPPORT_VESTEL_FLASH_PANEL */
        {
            _u4EepromPanelIndex = u4FinalIndex;
            Printf("########1 _u4EepromPanelIndex=%d\n", _u4EepromPanelIndex);
#ifdef SUPPORT_VESTEL_FLASH_PANEL

            if (u4FinalIndex == PANEL_INVALID)
            {
                u4FinalIndex = DRVCUST_PanelGet(eDefaultPanelSelect);
            }
#endif
            if (u4FinalIndex != PANEL_INVALID)
            {
			#ifndef SUPPORT_VESTEL_FLASH_PANEL
                Printf("Find panel index %d from EEPROM\n", u4FinalIndex);

                // check if we need to write panel idx back to EEPROM
                for (i = 0; i < u4Size; i++)
                {
                    if (DRVCUST_PanelQuery(arKey[i], &u4Value) == 0)
                    {
                        if (au1Index[i] != (UINT8)u4FinalIndex)
                        {
                            u4WriteIdxToEeprom = 1;
                            Printf("Recover panel index in EEPROM\n");
                        }
                    }
                }
            #else
				Printf("Vestel panel index: %d\n", u4FinalIndex);
            #endif

                // 1. check if we need to load panel table from flash 
                // 2. check if we need to map EEPROM index to flash index
                // 3. Load panel table from flash
                if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
                {
                    // if there is no mapping table, return the same index
                    // if can't find flash index from mapping table, return 0
                    u4FinalIndex = PANEL_TranslateEepromToFlashIndex(u4FinalIndex);
                    Printf("Translate panel index to %d\n", u4FinalIndex);

                    #ifdef LOAD_PANEL_FROM_FLASH_CUSTOM
                    if (!LOAD_PANEL_FROM_FLASH_CUSTOM(LOAD_PANEL_FROM_FLASH_BY_ID, &u4FinalIndex))
                    {
                            Printf("Load panel table from EEPROM failed\n");

                            // FIXME!
                            // FIXME!
                            // start self-diag.
                            SelectPanel(0);    

                    }
                    #else  // turnkey load panel table from flash
                    {
						#ifdef SUPPORT_VESTEL_FLASH_PANEL
							if(u4FinalIndex > wGetFlashPanel_Number())
							{
								u4FinalIndex = wGetFlashPanel_Number();
							}
							_u4EepromPanelIndex = u4FinalIndex;
							Printf("########2 _u4EepromPanelIndex = %d \n",_u4EepromPanelIndex);
							#if 0
							if (u4WriteIdxToEeprom)
							{
								WritePanelIndexToEeprom(_u4EepromPanelIndex);
							}
							#endif
							if(u4FinalIndex >= 1)
							{
								u4FinalIndex -=  1;
							}	
							Printf("########3 u4FinalIndex = %d \n",u4FinalIndex);
							UINT32 offset  = (u4FinalIndex * DRVCUST_PanelGet(ePanelFlashTableSize) + (u4FinalIndex + 1)*sizeof(tagPanelTblHead)) + sizeof(tagPanelFileHead);						
							#ifndef CC_MTK_LOADER
								if (!_PanelLoadTableFromNOR(offset))
							#endif			
						#else
							UINT32 offset = DRVCUST_PanelGet(ePanelFlashTableAddress)+(u4FinalIndex * DRVCUST_PanelGet(ePanelFlashTableSize));		
							if (!_PanelLoadTableFromFlash(offset))
						#endif
                        {
                            Printf("Load panel table from EEPROM failed\n");
                            SelectPanel(PANEL_INVALID);
                        }
                    }
                    #endif
                }
                else
                {
                    if (u4FinalIndex >= PANEL_TOTAL_NUMBER)
                    {
                        Printf("Panel index loaded from EEPROM is out of panel table size\n");
                        u4FinalIndex = PANEL_INVALID;
                    }
                    SelectPanel(u4FinalIndex);
                }
            }
            else
            {
                Printf("No valid index found from EEPROM\n");
                SelectPanel(PANEL_INVALID);
            }
        #ifndef SUPPORT_VESTEL_FLASH_PANEL
            if (u4WriteIdxToEeprom)
            {
                WritePanelIndexToEeprom(_u4EepromPanelIndex);
            }
        #endif
            _fgPanelIdInEeprom = TRUE;
        }
    }
    else if ((u4Value = DRVCUST_PanelGet(ePanelIndexFromGpioNum)) > 0)
    {
        UINT32* pu4Array = (UINT32*)DRVCUST_PanelGet(ePanelIndexFromGpioIdx);
        u4FinalIndex = 0;
        for (i = 0; i < u4Value; i++)
        {
            u4FinalIndex <<= 1;
            u4FinalIndex |= GPIO_Input(pu4Array[i]);
            //Printf("gpio[%d]=%d, u4FinalIndex=%d\n", i, pu4Array[i], u4FinalIndex);
        }
        pu4Array = (UINT32*)DRVCUST_PanelGet(ePanelIndexFromGpioMap);
        u4FinalIndex = pu4Array[u4FinalIndex];
        if (IS_PANEL_INDEX_INVALID(u4FinalIndex))
        {
            Printf("No valid index found from GPIO\n");
            SelectPanel(PANEL_INVALID);
        }
        else
        {
            SelectPanel(u4FinalIndex);
            Printf("Find panel index %d(%s) from GPIO\n",
                   u4FinalIndex, GetCurrentPanelName());
        }
        _fgPanelIdInEeprom = FALSE;
    }
    else
    {
        // use compile time defined panel index
        SelectPanel(PANEL_INVALID);
        _fgPanelIdInEeprom = FALSE;
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

void WritePanelIndexToEeprom(UINT32 u4PanelIndex)
{
    UINT32 u4Value, i;
    QUERY_TYPE_T arKey[] = {ePanelIndexOffset1, ePanelIndexOffset2,
                            ePanelIndexOffset3};
    UINT32 u4Size = sizeof(arKey) / sizeof(QUERY_TYPE_T);

    if ((DRVCUST_PanelQuery(ePanelIndexFromEeprom, &u4Value) == 0) &&
        (u4Value == 1))
    {
        for (i = 0; i < u4Size; i++)
        {
            if (DRVCUST_PanelQuery(arKey[i], &u4Value) == 0)
            {
                if (EEPROM_Write((UINT64)u4Value, (UINT32)&u4PanelIndex, 1) != 0)
                {
                    Printf("WritePanelIndexToEeprom fail, eeprom addr=0x%x\n",
                           u4Value);
                }
            }
        }
    }
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 PANEL_GetBacklightHigh(void)
{
    UINT32 value;
    if (DRVCUST_PanelQuery(eBacklightHigh, &value) == 0)
    {
        return value;
    }

    return _prPanelAttribute->u1BacklightHigh;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 PANEL_GetBacklightMiddle(void)
{
    UINT32 value;
    if (DRVCUST_PanelQuery(eBacklightMiddle, &value) == 0)
    {
        return value;
    }
    return _prPanelAttribute->u1BacklightMiddle;
}


//-----------------------------------------------------------------------------
/** Brief
 *  @param
 *  @return
 */
//-----------------------------------------------------------------------------

UINT32 PANEL_GetBacklightLow(void)
{
    UINT32 value;
    if (DRVCUST_PanelQuery(eBacklightLow, &value) == 0)
    {
        return value;
    }
    return _prPanelAttribute->u1BacklightLow;
}

#if SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET 
UINT32 PANEL_GetVTotal60Hz(void)
{
    if(_prPanelAttribute->u2VTotal60Hz 
        < (PANEL_GetPanelHeight() + SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET))
    {
        return (PANEL_GetPanelHeight() + SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET);
    }
    else
    {
        return _prPanelAttribute->u2VTotal60Hz;        
    }
}

UINT32 PANEL_GetPixelClk60Hz(void)
{
    UINT32 u4Clk;
    UINT32 u4VTotal;
    UINT32 u4HTotal;
    
    u4VTotal = PANEL_GetVTotal60Hz();
    u4HTotal = PANEL_GetHTotal60Hz();
    
    if((_prPanelAttribute->u2VTotal60Hz) < u4VTotal)
    {
        u4Clk = u4HTotal * u4VTotal * 60;
        if (fgIsSupport120HzPanel())
        {
            u4Clk <<= 1;
        }
    }
    else
    {
        u4Clk = _prPanelAttribute->u4PixelClk60Hz;
    }
    
    return u4Clk;
}

UINT32 PANEL_GetVTotal50Hz(void)
{
    if(_prPanelAttribute->u2VTotal50Hz 
        < (PANEL_GetPanelHeight() + SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET))
    {
        return (PANEL_GetPanelHeight() + SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET);
    }
    else
    {
        return _prPanelAttribute->u2VTotal50Hz;        
    }
}

UINT32 PANEL_GetPixelClk50Hz(void)
{
    UINT32 u4Clk;
    UINT32 u4VTotal;
    UINT32 u4HTotal;
    
    u4VTotal = PANEL_GetVTotal50Hz();
    u4HTotal = PANEL_GetHTotal50Hz();
    
    if((_prPanelAttribute->u2VTotal50Hz) < u4VTotal)
    {
        u4Clk = u4HTotal * u4VTotal * 50;
        if (fgIsSupport120HzPanel())
        {
            u4Clk <<= 1;
        }
    }
    else
    {
        u4Clk = _prPanelAttribute->u4PixelClk50Hz;
    }
    
    return u4Clk;
}
#endif

UINT32 PANEL_GetHSyncLen60Hz(void)
{
    UINT32 u4Len = 0;

#if 0
    if (_fgPixelShiftEnable)
    {
        // leave right mask as (PIXEL_SHIFT_H_MAX + 1) - _i4HPixelShift
        u4Len = (PANEL_GetHTotal60Hz() - PANEL_GetPanelWidth()) -
                ((PIXEL_SHIFT_H_MAX + 1) - _i4HPixelShift);
    }
    else
    {
        // leave right mask as PIXEL_SHIFT_H_MAX*2 + 1
        u4Len = (PANEL_GetHTotal60Hz() - PANEL_GetPanelWidth()) -
                (PIXEL_SHIFT_H_MAX * 2 + 1);
    }
#else
//    u4Len = (PANEL_GetHTotal60Hz() - _prPanelAttribute->u2PanelWidth) - (PIXEL_SHIFT_H_MAX * 2 + RIGHT_MASK_TYPI);
    u4Len = (PANEL_GetHTotal60Hz() - _prPanelAttribute->u2PanelWidth) - (PIXEL_SHIFT_H_MAX * 2);
#endif

    // protect overflow
    return (u4Len > 2047)?2047:u4Len; // 5360B is 511.
//    return (u4Len > 511)?511:u4Len;
}

UINT32 PANEL_GetHSyncLen50Hz(void)
{
    UINT32 u4Len = 0;

#if 0
    if (_fgPixelShiftEnable)
    {
        // leave right mask as (PIXEL_SHIFT_H_MAX + 1) - _i4HPixelShift
        u4Len = (PANEL_GetHTotal50Hz() - PANEL_GetPanelWidth()) -
                ((PIXEL_SHIFT_H_MAX + 1) - _i4HPixelShift);
    }
    else
    {
        // leave right mask as PIXEL_SHIFT_H_MAX*2 + 1
        u4Len = (PANEL_GetHTotal50Hz() - PANEL_GetPanelWidth()) -
                (PIXEL_SHIFT_H_MAX * 2 + 1);
    }
#else
//    u4Len = (PANEL_GetHTotal50Hz() - _prPanelAttribute->u2PanelWidth) - (PIXEL_SHIFT_H_MAX * 2 + RIGHT_MASK_50HZ);
    u4Len = (PANEL_GetHTotal50Hz() - _prPanelAttribute->u2PanelWidth) - (PIXEL_SHIFT_H_MAX * 2);
#endif

    // protect overflow
//    return (u4Len > 2047)?2047:u4Len; // 5360B is 511.
    //return (u4Len > 511)?511:u4Len;
    return (u4Len > 2047)?2047:u4Len; //5387 is 11 bits
}

//#ifndef SUPPORT_PANEL_CUSTOMER_SPEC
UINT32 PANEL_GetHPosition60Hz(void)
{
    if (_fgPixelShiftEnable)
    {
        return (UINT32)((INT32)(_prPanelAttribute->u2HPosition + PIXEL_SHIFT_H_MAX) + (_i4HPixelShift));
    }
    else
    {
        return _prPanelAttribute->u2HPosition;
    }
}

UINT32 PANEL_GetHPosition50Hz(void)
{
    return ((PANEL_GetHPosition60Hz() + PANEL_GetHTotal50Hz()) - PANEL_GetHTotal60Hz());
}

UINT32 PANEL_GetVPosition(void)
{
    if (_fgPixelShiftEnable)
    {
        return (UINT32)((INT32)(_prPanelAttribute->u2VPosition + PIXEL_SHIFT_V_MAX) + _i4VPixelShift);
    }
    else
    {
        return _prPanelAttribute->u2VPosition;
    }
}
//#endif  //SUPPORT_PANEL_CUSTOMER_SPEC

UINT32 PANEL_GetPanelWidth(void)
{
#if 1 // customers' spec
    return _prPanelAttribute->u2PanelWidth + (PIXEL_SHIFT_H_MAX * 2);
#else
    if (_fgPixelShiftEnable)
    {
        return _prPanelAttribute->u2PanelWidth + (PIXEL_SHIFT_H_MAX * 2);
    }
    else
    {
        return _prPanelAttribute->u2PanelWidth;
    }
#endif
}

UINT32 PANEL_GetPanelHeight(void)
{
#if 1 // customers' spec
    return _prPanelAttribute->u2PanelHeight + (PIXEL_SHIFT_V_MAX * 2);
#else
    if (_fgPixelShiftEnable)
    {
        return _prPanelAttribute->u2PanelHeight + (PIXEL_SHIFT_V_MAX * 2);
    }
    else
    {
        return _prPanelAttribute->u2PanelHeight;
    }
#endif
}

UINT32 PANEL_GetPanelPhysicalWidth(void)
{
    return _prPanelAttribute->u2PanelWidth;
}

UINT32 PANEL_GetPanelPhysicalHeight(void)
{
    return _prPanelAttribute->u2PanelHeight;
}

UINT32 PANEL_GetPixelShiftHMax(void)
{
    UINT32 u4Value;
    if (DRVCUST_PanelQuery(ePanelPixelShiftHMax, &u4Value) != 0)
    {
        u4Value = 0;
    }
    return u4Value;
}

UINT32 PANEL_GetPixelShiftVMax(void)
{
    UINT32 u4Value;
    if (DRVCUST_PanelQuery(ePanelPixelShiftVMax, &u4Value) != 0)
    {
        u4Value = 0;
    }
    return u4Value;
}

void PANEL_SetPixelShift(BOOL fgEnable, INT32 i4HPixelShift, INT32 i4VPixelShift)
{
    _fgPixelShiftEnable = fgEnable;
    _i4HPixelShift = i4HPixelShift;
    _i4VPixelShift = i4VPixelShift;
}

BOOL PANEL_IsPixelShift(void)
{
    return _fgPixelShiftEnable;
}

INT32 PANEL_GetPixelShift_H(void)
{
    return _i4HPixelShift;
}

INT32 PANEL_GetPixelShift_V(void)
{
    return _i4VPixelShift;
}

CHAR * PANEL_GetVersion(void)
{
    #ifdef LOAD_PANEL_FROM_FLASH_CUSTOM
    return FlashPanel_GetVersion();
    #else
    return "Unknow";
    #endif
}

UINT32 PANEL_GetSpreadSpectrum(UINT32 *pu4Freq, UINT32 *pu4Permillage)
{
    if ((pu4Freq == NULL) || (pu4Permillage == NULL))
    {
        return 1;
    }
	#ifdef SUPPORT_VESTEL_FLASH_PANEL
	*pu4Freq = _arPanelAttribute[0].u2LVDS_ModulationRate;
	*pu4Permillage = (UINT32)_arPanelAttribute[0].u2LVDS_SpreadRange;
	#else
    #ifdef SUPPORT_PANEL_SS
    {
        UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
        if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
        {           
            *pu4Freq = 30000;
            *pu4Permillage = 0; 
            return 1;
        }
        *pu4Freq = _arPanelAttributeExt[u4ExtIdx].u4SSFreq;
        *pu4Permillage = (UINT32)_arPanelAttributeExt[u4ExtIdx].u1SSPermillage;
    }
    #else
    *pu4Freq = 30000;
    #ifdef CC_MTK_LOADER
    *pu4Permillage = 0;
    #else
    *pu4Permillage = DRVCUST_OptGet(eSpreadSpectrumPermillage);
    #endif
    #endif
	#endif
    return 0;
}

UINT32 PANEL_GetDRAMSpreadSpectrum(UINT32 *pu4Freq, UINT32 *pu4Permillage)
{
    if ((pu4Freq == NULL) || (pu4Permillage == NULL))
    {
        return 1;
    }
	
#ifdef SUPPORT_VESTEL_FLASH_PANEL
	*pu4Freq = _arPanelAttribute[0].u2DRAM_SpreadModulationRate;
	*pu4Permillage = (UINT32)_arPanelAttribute[0].u2DRAM_SpreadRange;
#else
	*pu4Freq = DRVCUST_InitGet(eDmpllSpectrumFrequency);
	*pu4Permillage = DRVCUST_InitGet(eDmpllSpectrumPermillage);
#endif

#ifdef SUPPORT_VESTEL_FLASH_PANEL
	if (*pu4Freq <= 60 && *pu4Permillage <= 100)
	{
		//The setting is OK.
		return 0;
	}
	else
	{
		//The setting is invalid , set to default value.
		*pu4Freq = DRVCUST_InitGet(eDmpllSpectrumFrequency);
		*pu4Permillage = DRVCUST_InitGet(eDmpllSpectrumPermillage);
	}
    #endif
    return 0;
}

UINT32 PANEL_GetDither(void)
{
    #ifdef SUPPORT_PANEL_DITHER
    {
        UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
        if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
        {   
            return 0xffffffff;
        }
        return (UINT32)_arPanelAttributeExt[u4ExtIdx].u4Dither;
    }
    #else
    return 0xffffffff;
    #endif
}

UINT32 PANEL_GetDrivingCurrent(void)
{
    UINT32 u4Value;
	#ifdef SUPPORT_VESTEL_FLASH_PANEL
		u4Value = _arPanelAttribute[0].u2LVDS_Amplitude;
	#else
    #ifdef SUPPORT_PANEL_DRIVING
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx < EXT_PANEL_INDEX_INVALID)
    {   
        u4Value = (UINT32)_arPanelAttributeExt[u4ExtIdx].u1DrivingCurrent;
    }
    else
    #endif
    if (DRVCUST_PanelQuery(eLvdsDrivingCurrent, &u4Value) == -1)
    {
        u4Value = 0xffffffff;
    }
    #endif
    return u4Value;
}

UINT16 PANEL_Get3DPixelSize(void)
{
    UINT32 u4Value;
    #ifdef SUPPORT_PANEL_3D_PIXEL_SIZE
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx < EXT_PANEL_INDEX_INVALID)
    {   
        u4Value = (UINT32)_arPanelAttributeExt[u4ExtIdx].u2PixelSize;
    }
    else
    #endif
    {
        u4Value = 320; // Pixel Size = 0.320 mm = 320 um
    }
    return u4Value;
}

UINT32 Panel_GetAQIndex(UINT32 u4Idx)
{
    #ifdef LOAD_PANEL_FROM_FLASH_CUSTOM
    return FlashPanel_GetAQIndex(u4Idx);
    #else
    return 0;
    #endif
}

UINT32 Panel_GetPQIndex(UINT32 u4Idx)
{
    #ifdef LOAD_PANEL_FROM_FLASH_CUSTOM
    return FlashPanel_GetPQIndex(u4Idx);
    #else
    return 0;
    #endif
}


// ------------------------------------------------------------------
//  Optional function
// ------------------------------------------------------------------
#if defined(SUPPORT_PANEL_48HZ) || defined(SUPPORT_PANEL_CUSTOMER_SPEC) || defined(SUPPORT_PANEL_SS) || defined(SUPPORT_PANEL_DITHER) || defined(SUPPORT_PANEL_ERRORLIMIT_TABLE) || defined(SUPPORT_PANEL_3D_PIXEL_SIZE)
static UINT32 PANEL_GetExtAttributeIndex(void)
{
    UINT32 u4ExtIdx;
	UINT32 i ;

    if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
    {   // load panel table from flash
        u4ExtIdx = 0;
    }
    else
    {
        u4ExtIdx = EXT_PANEL_INDEX_INVALID;
        for(i=0 ; i < sizeof(_arPanelAttributeExt)/sizeof(PANEL_ATTRIBUTE_EXT_T); i++)
        {
            if(GetCurrentPanelIndex() == _arPanelAttributeExt[i].u4PanelIndex)
            {
                u4ExtIdx = i;
                break;
            }
        }
    }
    return u4ExtIdx;
}


UINT32 PANEL_IsSupportExtAttribute(void)
{
    if(PANEL_GetExtAttributeIndex() >= EXT_PANEL_INDEX_INVALID)
    {
        return 0; 
    }
    return 1;
}

#endif

#ifdef SUPPORT_PANEL_48HZ
UINT32 PANEL_GetPixelClk48Hz(void)
{
#ifdef SUPPORT_VESTEL_FLASH_PANEL
		return (UINT32) _arPanelAttribute[0].u4PixelClk48Hz;
#else
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    UINT32 u4Clk;
#if SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET 
    UINT32 u4VTotal;
#endif    
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return PANEL_GetPixelClk60Hz();
    }

#if SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET 
    u4VTotal = PANEL_GetVTotal48Hz();
    
    if(_arPanelAttributeExt[u4ExtIdx].u2VTotal48Hz < u4VTotal)
    {
        u4Clk = PANEL_GetHTotal48Hz() * u4VTotal * 48;
        if (fgIsSupport120HzPanel())
        {
            u4Clk <<= 1;
        }
    }
    else
#endif
    {
        u4Clk = _arPanelAttributeExt[u4ExtIdx].u4PixelClk48Hz;
    }
    
    return u4Clk;
#endif
}

UINT32 PANEL_GetHTotal48Hz(void)
{
#ifdef SUPPORT_VESTEL_FLASH_PANEL
	return (UINT32) _arPanelAttribute[0].u2HTotal48Hz;
#else
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return PANEL_GetHTotal60Hz();
    }
    return _arPanelAttributeExt[u4ExtIdx].u2HTotal48Hz;
#endif
}

UINT32 PANEL_GetVTotal48Hz(void)
{
#ifdef SUPPORT_VESTEL_FLASH_PANEL
	return (UINT32) _arPanelAttribute[0].u2VTotal48Hz;
#else
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return PANEL_GetVTotal60Hz();
    }
#if SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET 
    if(_arPanelAttributeExt[u4ExtIdx].u2VTotal48Hz 
        < (PANEL_GetPanelHeight() + SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET))
    {
        return (PANEL_GetPanelHeight() + SCALER_PSCAN_DISPMODE_MIN_VTOTAL_OFFSET);
    }
    else
#endif
    {
        return _arPanelAttributeExt[u4ExtIdx].u2VTotal48Hz;
    }
#endif
}

UINT32 PANEL_GetHSyncLen48Hz(void)
{
    UINT32 u4Len;
	#ifdef SUPPORT_VESTEL_FLASH_PANEL
    u4Len = _arPanelAttribute[0].u2HTotal48Hz - PANEL_GetPanelWidth();
	#else
    u4Len = (PANEL_GetHTotal48Hz() - PANEL_GetPanelWidth());
	#endif
    // protect overflow
    return (u4Len > 2047)?2047:u4Len;
}

UINT32 PANEL_GetDimmingFrequency48Hz(void)
{
#ifdef SUPPORT_VESTEL_FLASH_PANEL
    return (UINT32) _arPanelAttribute[0].u4DimingFreq48Hz;
#else
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return PANEL_GetDimmingFrequency60Hz();
    }
    return (UINT32)_arPanelAttributeExt[u4ExtIdx].u2DimmingFrequency48Hz;
#endif	
}

void PANEL_SetPixelClk48Hz(UINT32 u4Value)
{
#ifdef SUPPORT_VESTEL_FLASH_PANEL
	_arPanelAttribute[0].u4PixelClk48Hz= u4Value;
#else
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return ;
    }
    _arPanelAttributeExt[u4ExtIdx].u4PixelClk48Hz = u4Value;
#endif
    return;
}

void PANEL_SetHTotal48Hz(UINT32 u4Value)
{
#ifdef SUPPORT_VESTEL_FLASH_PANEL
	_arPanelAttribute[0].u2HTotal48Hz= (UINT16)u4Value;
#else
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return;
    }
    _arPanelAttributeExt[u4ExtIdx].u2HTotal48Hz = (UINT16)u4Value;
#endif
    return;
}

void PANEL_SetVTotal48Hz(UINT32 u4Value)
{
#ifdef SUPPORT_VESTEL_FLASH_PANEL
	_arPanelAttribute[0].u2VTotal48Hz = (UINT16)u4Value;
#else
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return;
    }
    _arPanelAttributeExt[u4ExtIdx].u2VTotal48Hz = (UINT16)u4Value;
#endif
    return;
}
UINT32 PANEL_GetHPosition48Hz(void)
{
    return ((PANEL_GetHPosition60Hz() + PANEL_GetHTotal48Hz()) - PANEL_GetHTotal60Hz());
}

#endif //#ifdef SUPPORT_PANEL_48HZ




#ifdef SUPPORT_PANEL_CUSTOMER_SPEC
INT32 PANEL_GetHPositionByTiming(UINT8 bDclkType)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if ((u4ExtIdx >= EXT_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return PANEL_GetHPosition60Hz();
    }
    return (_arPanelAttributeExt[u4ExtIdx].u2HFrontPorch[bDclkType]+0x800);
}

INT32 PANEL_GetVPositionByTiming(UINT8 bDclkType)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if ((u4ExtIdx >= EXT_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return PANEL_GetVPosition();
    }
    
    UINT16 u2VSyncWidth = _arPanelAttributeExt[u4ExtIdx].u1VSyncWidth[bDclkType];
    UINT16 u2VBackPorch = _arPanelAttributeExt[u4ExtIdx].u2VbackPorch[bDclkType];
    UINT16 u2VSyncDelSel =5;

#if 0//CC_SCPOS_BOTTOM_MASK_BIT_WA
    #ifndef CC_5391_LOADER
    return (INT32)(0x400 -u2VSyncWidth -u2VBackPorch + u2BottomMaskToTop +u2VSyncDelSel);
    #else
    return (INT32)(0x400 -u2VSyncWidth -u2VBackPorch +u2VSyncDelSel);
    #endif
#else
    return (INT32)(0x400 -u2VSyncWidth -u2VBackPorch +u2VSyncDelSel);
#endif
}

UINT32 PANEL_GetHSyncWidthByTiming(UINT8 bDclkType)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if ((u4ExtIdx >= EXT_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return PANEL_GetHSyncWidth();
    }
    return _arPanelAttributeExt[u4ExtIdx].u1HSyncWidth[bDclkType];
}

UINT32 PANEL_GetVSyncWidthByTiming(UINT8 bDclkType)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if ((u4ExtIdx >= EXT_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return PANEL_GetVSyncWidth();
    }
    return _arPanelAttributeExt[u4ExtIdx].u1VSyncWidth[bDclkType];
}

UINT32 PANEL_GetVSyncBpByTiming(UINT8 bDclkType)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if ((u4ExtIdx >= EXT_PANEL_INDEX_INVALID) || (bDclkType >= 3))
    {
        return PANEL_GetVSyncWidth();
    }
    return _arPanelAttributeExt[u4ExtIdx].u2VbackPorch[bDclkType];
}

UINT32 PANEL_GetHSyncPolarity(void)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return 0;
    }
    return _arPanelAttributeExt[u4ExtIdx].u1HSyncPolarity;
}

UINT32 PANEL_GetVSyncPolarity(void)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();
    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return 0;
    }
    return _arPanelAttributeExt[u4ExtIdx].u1VSyncPolarity;
}

/*
//return num*10; for 120Hz panel
UINT32 PANEL_GetFrameDelay(void)
{    
    return 0;
}*/
#endif //#SUPPORT_PANEL_CUSTOMER_SPEC

#ifdef SUPPORT_PANEL_ERRORLIMIT_TABLE
UINT32 PANEL_GetErrorLimit(void)
{
    UINT32 u4ExtIdx = PANEL_GetExtAttributeIndex();

    if (u4ExtIdx >= EXT_PANEL_INDEX_INVALID)
    {
        return 0x3F;
    }    
    return (UINT32)_arPanelAttributeExt[u4ExtIdx].u4ErrorLimit;
}
#endif

// ====== Local Dimming ============================

UINT32 PANEL_IsSupportLocalDimming(void)
{
    UINT32 u4PanelIdx;
    UINT32 i;

    u4PanelIdx = GetCurrentPanelIndex();

    for (i = 0; i < sizeof(_arPanelLDAttribute)/sizeof(PANEL_LOCAL_DIMMING_ATTRIBUTE_T); i++)
    {
        if (_arPanelLDAttribute[i].u4PanelIndex == u4PanelIdx)
        {
            return SV_TRUE;
        }
    }
    return SV_FALSE;
}


UINT32 PANEL_GetLocalDimmingParam(PANEL_LOCAL_DIMMING_ATTRIBUTE_T **pParam)
{
    UINT32 u4PanelIdx;
    UINT32 i;

    u4PanelIdx = GetCurrentPanelIndex();

    for (i = 0; i < sizeof(_arPanelLDAttribute)/sizeof(PANEL_LOCAL_DIMMING_ATTRIBUTE_T); i++)
    {
        if (_arPanelLDAttribute[i].u4PanelIndex == u4PanelIdx)
        {
            *pParam = &_arPanelLDAttribute[i];
            return SV_TRUE;
        }
    }
    *pParam = NULL;
    return SV_FALSE;
}

// ====== miniLVDS ============================
UINT32 PANEL_IsSupportMlvds(void)
{
#ifndef SUPPORT_VESTEL_FLASH_PANEL
	return ((PANEL_GetControlWord() & DISP_TYPE_MASK) == DISP_TYPE_MLVDS);
#else
    return ((PANEL_GetControlWord2() & DISP_TYPE_MASK) == DISP_TYPE_MLVDS);
#endif
}


UINT32 PANEL_GetMlvdsParam(PANEL_MINILVDS_ATTRIBUTE_T **pParam)
{
    UINT32 u4PanelIdx;
    UINT32 i;

    if (DRVCUST_PanelGet(ePanelFlashTableSize) > 0)
    {   
        if(PANEL_IsSupportMlvds()) //Adam check
//        if((PANEL_GetControlWord() & DISP_TYPE_VB1) != 0) //Adam check
        {
            *pParam = &_arPanelMiniLVDSAttribute[0];
            return  SV_TRUE;
        }

    }
    else
    {
    u4PanelIdx = GetCurrentPanelIndex();

    for (i = 0; i < sizeof(_arPanelMiniLVDSAttribute)/sizeof(PANEL_MINILVDS_ATTRIBUTE_T); i++)
    {
        if (_arPanelMiniLVDSAttribute[i].u4PanelIndex == u4PanelIdx)
        {
            *pParam = &_arPanelMiniLVDSAttribute[i];
            return SV_TRUE;
        }
    }
    }
    *pParam = NULL;
    return SV_FALSE;
}


static UINT32 _u4ProtDisplayTypeMsk = 
#ifdef PANEL_DISPLAY_TYPE_ALLOWED_MASK
    PANEL_DISPLAY_TYPE_ALLOWED_MASK;
#else
    // defaultly set to lvds only mode
    (1<<PANEL_DISPLAY_TYPE_LVDS);
#endif
#if !defined(CC_MTK_LOADER) || !defined(CC_LOADER_SHRINK_PANEL_OTHER)  //loader shrink
UINT32 PANEL_SetProtDisplayTypeMsk(UINT32 *u4ProtDisplayTypeMsk)
{
    if(u4ProtDisplayTypeMsk != NULL)
    {
        _u4ProtDisplayTypeMsk = *u4ProtDisplayTypeMsk; 
    }
    return _u4ProtDisplayTypeMsk;
}

// -- for multiple panel interface configuration --
UINT8 PANEL_IsSupportVb1(void)
{
#ifndef SUPPORT_VESTEL_FLASH_PANEL
	return ((PANEL_GetControlWord() & DISP_TYPE_MASK) == DISP_TYPE_VB1);
#else
    return ((PANEL_GetControlWord2() & DISP_TYPE_MASK) == DISP_TYPE_VB1); 
#endif
}
UINT8 PANEL_IsSupportEpi(void)
{
#ifndef SUPPORT_VESTEL_FLASH_PANEL
	return ((PANEL_GetControlWord() & DISP_TYPE_MASK) == DISP_TYPE_EPI);
#else
    return ((PANEL_GetControlWord2() & DISP_TYPE_MASK) == DISP_TYPE_EPI); 
#endif
}

UINT32 PANEL_GetDisplayType(void)
{
    UINT32 u4PanelDisplayType = PANEL_DISPLAY_TYPE_LVDS;

    if( PANEL_IsSupportMlvds() )
    {
        u4PanelDisplayType = PANEL_DISPLAY_TYPE_MLVDS;
    }
    if( PANEL_IsSupportVb1() )
    {
        // check if multiple panel interface is defined
        ASSERT(u4PanelDisplayType == PANEL_DISPLAY_TYPE_LVDS);

        u4PanelDisplayType = PANEL_DISPLAY_TYPE_VB1;
    }

//#if 0
    if( PANEL_IsSupportEpi() )
    {
        // check if multiple panel interface is defined
        ASSERT(u4PanelDisplayType == PANEL_DISPLAY_TYPE_LVDS);

        u4PanelDisplayType = PANEL_DISPLAY_TYPE_EPI;
    }
//#endif

    // set to allowed mode only if confirmed 
    if(0 == (_u4ProtDisplayTypeMsk & (1<<u4PanelDisplayType)))
    {
        return PANEL_DISPLAY_TYPE_MAX;
    } 

    return u4PanelDisplayType;
}
#endif
#endif /* CC_MTK_PRELOADER */

