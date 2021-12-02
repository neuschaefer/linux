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
 * $Author: hongjun.chu $
 * $Date: 2012/04/28 $
 * $RCSfile: eeprom_if.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file eeprom_if.h
 *  Declare EEPROM export function.
 */

#ifndef EEPROM_IF_H

#define EEPROM_IF_H

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN
#include "sif_if.h"

#include "x_typedef.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_printf.h"
#include "drvcust_if.h"
LINT_EXT_HEADER_END


//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------
/********************************* MOVE TO CUSTOMIZATION *******************************/
/// EEPROM is divided into 4 partitions: (1) HDMI HDCP, (2) DTV driver, (3) NPTV driver, (4) MIDDLEWARE.
/// Partition (1) HDMI HDCP 0 ~ 319 bytes.
//#define EEPROM_HDMI_HDCP_OFFSET  ((UINT32)0)
//#define EEPROM_HDMI_HDCP_SIZE  ((UINT32)320)

/// Partition (2) DTV driver 320 ~ 351 bytes.
//#define EEPROM_DTV_DRIVER_OFFSET    ((UINT32)(EEPROM_HDMI_HDCP_OFFSET + EEPROM_HDMI_HDCP_SIZE))
//#define EEPROM_DTV_DRIVER_SIZE         ((UINT32)32)

/// Partition (3) NPTV driver 352 ~ 1119 bytes.
//#define EEPROM_NPTV_DRIVER_OFFSET    ((UINT32)(EEPROM_DTV_DRIVER_OFFSET + EEPROM_DTV_DRIVER_SIZE))
//#define EEPROM_NPTV_DRIVER_SIZE         ((UINT32)(768))

/// Partition (4) MIDDLEWARE 1120 ~ eeprom end.
//#define EEPROM_DRIVER_SIZE  ((UINT32)(EEPROM_NPTV_DRIVER_OFFSET + EEPROM_NPTV_DRIVER_SIZE))
#define EEPROM_HDMI_HDCP_OFFSET		    (DRVCUST_InitGet(eEepromHdmiHdcpOffset))
#define EEPROM_HDMI_HDCP_SIZE  		    (DRVCUST_InitGet(eEepromHdmiHdcpSize))
#define EEPROM_VGA_EDID_OFFSET		    (DRVCUST_InitGet(eEepromVgaEdidOffset))
#define VGA_EDID_SIZE  		            (DRVCUST_InitGet(eEdidVgaSize))
#define VGA_EDID_STORAGE_SIZE  		    (DRVCUST_InitGet(eEdidVgaStorageSize))
#define EEPROM_HDMI_EDID_OFFSET		    (DRVCUST_InitGet(eEepromHdmiEdidOffset))
#define HDMI_EDID_SIZE  	            (DRVCUST_InitGet(eEdidHdmiSize))
#define HDMI_EDID_STORAGE_SIZE  	    (DRVCUST_InitGet(eEdidHdmiStorageSize))


#define EEPROM_ERROR_CODE_OFFSET 		(DRVCUST_InitGet(eEepromErrorCodeOffset))
#define EEPROM_ERROR_CODE_SIZE     		(DRVCUST_InitGet(eEepromErrorCodeSize))
#define EEPROM_STATE_CODE_OFFSET 		(DRVCUST_InitGet(eEepromStateCodeOffset))
#define EEPROM_STATE_CODE_SIZE     		(DRVCUST_InitGet(eEepromStateCodeSize))
#define EEPROM_DTV_DRIVER_OFFSET 		(DRVCUST_InitGet(eEepromDtvDriverOffset))
#define EEPROM_DTV_DRIVER_SIZE     		(DRVCUST_InitGet(eEepromDtvDriverSize))
#define EEPROM_NPTV_DRIVER_OFFSET	    (DRVCUST_InitGet(eEepromNptvDriverOffset))
#define EEPROM_NPTV_DRIVER_SIZE    		(DRVCUST_InitGet(eEepromNptvDriverSize))
#define EEPROM_DRIVER_SIZE 			    (DRVCUST_InitGet(eEepromDriverSize))
#define EEPROM_MW_OFFSET	            (DRVCUST_InitGet(eEepromMwOffset))
#define EEPROM_MW_SIZE    		        (DRVCUST_InitGet(eEepromMwSize))
//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

/// DTVCFG_T Offset 0: user 1: DTVCFG by Joshua, 4 bytes.
typedef struct __DtvCfg {
    UINT8 u1Magic1;         ///< MTK fix value = 0x53.
    UINT8 u1Magic2;         ///< MTK fix value = 0x81.
//    UINT8 u1PanelIdx;       ///< MTK internal setting value.
    UINT8 u1Flags;			///< MTK internal setting value.
//    UINT32 u4Skip;          // NIM configuration.
    UINT8 u1Upgrade;
    UINT8 u1Upgrade_Ext;
    UINT8 u1Flags2;
    UINT8 u1Flags3;    
    UINT8 u1Flags4;        
    UINT8 u1Checksum;
} DTVCFG_T;

/// DTVCFG parameter macro.
#define DTVCFG_MAGIC_CHAR1                  (0x53)
#define DTVCFG_MAGIC_CHAR2                  (0x81)

/// DTVCFG flag for system configuration.
#define DTVCFG_FLAG_POWERON_BOOT            (1U << 0)   /* Map to PC_MISC_CTRL_NO_STANDBY, if 1, just boot, otherwise standby if ext_status on */
#define DTVCFG_FLAG_FACTORY_MODE            (1U << 1)   /* Map to PC_MISC_CTRL_FACTORY_MODE */
#define DTVCFG_FLAG_POWERON_LAST            (1U << 2)   /* Map to PC_MISC_CTRL_NO_STANDBY_2, if 1, read system_state_on to decide boot or standby */
#define DTVCFG_FLAG_8280_UPGRADE_ON         (1U << 3)   /* Map to PC_MISC_CTRL_COMPANION_CHIP_UPGRADE */
#define DTVCFG_FLAG_POWER_BTN_ON            (1U << 4)   /* Use to record power button count, 1 is on, 0 is off */
#define DTVCFG_FLAG_SYSTEM_STATE_ON         (1U << 5)   /* Use to record previous system state, 1 is on, 0 is off. */
#define DTVCFG_FLAG_POWER_LEDOFF            (1U << 6)   /* Map to PC_MISC_CTRL_POWER_LED_OFF */
#define DTVCFG_FLAG_USB_UPGRADE_ON          (1U << 7)   /* Map to PC_MISC_CTRL_FIRMWARE_UPGRADE */

/// DTVCFG flags2 for system configuration.
#define DTVCFG_FLAG2_IMAGE_BANK             (1U << 0)
#define DTVCFG_FLAG2_8032UP_BANK            (1U << 1)
#define DTVCFG_FLAG2_QUIET_BOOT             (1U << 2)   /* Map to PC_MISC_CTRL_RTC_QUIET_BOOT, if 1, set quiet boot. */
#define DTVCFG_FLAG2_DISABLE_LOGO           (1U << 3)   /* Use to record logo display state, if 1, disable logo. otherwise show logo */
#define DTVCFG_FLAG2_FLIP_ON                (1U << 4)   /* FLIP_ENABLE */
#define DTVCFG_FLAG2_MIRROR_ON              (1U << 5)   /* MIRROR_ENABLE*/
#define DTVCFG_FLAG2_FP_LOCK                (1U << 6)   /* FP_LOCK, default 0. if 1 keypad cannot wakeup from standby. */
#define DTVCFG_FLAG2_UPGRADE_GO	            (1U << 7)   /* Use to record the usb upgrade status. */

/// DTVCFG flags3 for system configuration.
#define DTVCFG_FLAG3_KERNEL_BANK                    (1U << 0)
#define DTVCFG_FLAG3_ROOTFS_BANK                    (1U << 1)
#define DTVCFG_FLAG3_FAST_BOOT                      (1U << 2)
#define DTVCFG_FLAG3_NORMAL_BOOT_WITH_SUSPEND       (1U << 3)
#define DTVCFG_FLAG3_NORMAL_BOOT_WITHOUT_SUSPEND    (1U << 4)
#define DTVCFG_FLAG3_SUPPORT_CLI                    (1U << 5)
#define DTVCFG_FLAG3_BASIC_BANK                     (1U << 6)

/// DTVCFG_T Offset 4: user 2: NIM parameter by James Wei, 4 bytes.
typedef struct __EEPNIMCfg {
    UINT8 u1Tag;     			///< MTK define NIM module tag.
    UINT8 u1TunerType;    ///< MTK define NIM Tuner type.
    UINT8 u1Reserved;     ///< MTK define NIM reserved value.
    UINT8 u1Checksum;			///< MTK define NIM checksum byte.
} EEPNIMCFG_T;

/// EEPROM DTV offset list.
typedef enum __EEPDTV_Offset_T{
///Offset 0: user 1: DTVCFG by Joshua, 4 bytes.
    EEPDTV_DTVCFG_MAGIC1 = 0,
    EEPDTV_DTVCFG_MAGIC2,
//    EEPDTV_DTVCFG_PANELIDX,
    EEPDTV_DTVCFG_FLAGS,
///Offset 4: user 2: NIM parameter by James Wei, 4 bytes.
    /*
    EEPDTV_NIMCFG_TAG,
    EEPDTV_NIMCFG_TUNERTYPE,
    EEPDTV_NIMCFG_RESERVED,
    EEPDTV_NIMCFG_CHECKSUM,
    */
///Offset 8: user 3: extended for DTVCFG.
    EEPDTV_DTVCFG_UPGRADE,
    EEPDTV_DTVCFG_UPGRADE_EXT,
    EEPDTV_DTVCFG_FLAGS2,
    EEPDTV_DTVCFG_FLAGS3,    
    EEPDTV_DTVCFG_FLAGS4,        
    EEPDTV_DTVCFG_CHECKSUM,
///Offset 12: the following is free for usage.

///terminal flag.
    EEPDTV_END
} EEPDTV_Offset_T;

#ifdef CLI_SUPPORT
#define EEPROM_ARRAYINDEX 0x1000
typedef struct {
	UINT8 _i4Devnum;    
    UINT32 _i4writebytescount[EEPROM_ARRAYINDEX];      

} EEP_HISTORY;

#endif

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

/** EEPROM_OpertionCount
 *  printf the EEPROM Opertion Count
 *  @param  VOID
 *  @return  VOID
 */
#ifdef CLI_SUPPORT

EXTERN VOID EEPROM_OpertionCount(VOID);

#endif

/** EEPROM_SetSpeed
 *  Set EEPROM device I2C clk rate divider.
 *  @param I2C clk rate divider.
 *  @return  VOID
 */
EXTERN VOID EEPROM_SetClkDiv(UINT16 u2Speed);

/** EEPROM_InitClkDiv
 *  get EEPROM device I2C clk rate divider.
 *  @param  VOID
 *  @return  VOID
 */
EXTERN VOID EEPROM_InitClkDiv(VOID);

/** EEPROM_Init
 *  Do necessary EEPROM initialization
 *  @param  VOID
 *  @retval   0		SUCCESS.
 *  @retval   <0	Error
 */
EXTERN INT16  EEPROM_Init(VOID);

//-------------------------------------------------------------------------
/** EEPROM_SetSize
 *  Set EEPROM device size.
 *  @param device size in bytes
 *  @return  VOID.
 */
EXTERN VOID EEPROM_SetSize(UINT32 u4Size);

/** EEPROM_GetSize
 *  get EEPROM device size.
 *  @param  VOID
 *  @return  device size in bytes.
 */
EXTERN INT32 EEPROM_GetSize(VOID);

//-------------------------------------------------------------------------
/** EEPROM_GetPage
 *  get EEPROM page write size.
 *  @param  VOID
 *  @return  device size in bytes.
 */
//-------------------------------------------------------------------------
EXTERN INT32 EEPROM_GetPage(VOID);

//-------------------------------------------------------------------------
/** EEPROM_SetPage
 *  Set EEPROM page write size.
 *  @param device size in bytes
 *  @return  VOID.
 */
//-------------------------------------------------------------------------
EXTERN VOID EEPROM_SetPage(UINT32 u4Size);

/** EEPROM_Read
 *  EEPROM read function.
 *  @param  u8Offset 	begin address to read.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	read data length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
EXTERN INT32 EEPROM_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

/** EEPROM_Write
 *  EEPROM write function.
 *  @param  u8Offset 	begin address to write.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	writedata length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
EXTERN INT32 EEPROM_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

// The following interface is only for NPTV.

/** EEPNPTV_GetSize
 *  NPTV interface: get EEPROM device size.
 *  @param  VOID
 *  @return  device size in bytes.
 */
EXTERN INT32 EEPNPTV_GetSize(VOID);

/** EEPNPTV_Read
 *  NPTV interface: EEPROM read function.
 *  @param  u8Offset 	begin address to read.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	read data length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
EXTERN INT32 EEPNPTV_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

/** EEPNPTV_Write
 *  NPTV interface: EEPROM write function.
 *  @param  u8Offset 	begin address to write.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	writedata length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
EXTERN INT32 EEPNPTV_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

// The following interface is only for DTV.

/** EEPDTV_GetSize
 *  DTV interface: get EEPROM device size.
 *  @param  VOID
 *  @return  device size in bytes.
 */
EXTERN INT32 EEPDTV_GetSize(VOID);

/** EEPDTV_Read
 *  DTV interface: EEPROM read function.
 *  @param  u8Offset 	begin address to read.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	read data length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
EXTERN INT32 EEPDTV_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

/** EEPDTV_Write
 *  DTV interface: EEPROM write function.
 *  @param  u8Offset 	begin address to write.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	writedata length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
EXTERN INT32 EEPDTV_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

// The following interface is only for HDMI HDCP storage.

/** EEPHDCP_Read
 *  HDCP interface: EEPROM read function.
 *  @param  u8Offset 	begin address to read.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	read data length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
EXTERN INT32 EEPHDCP_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

/** EEPHDCP_Write
 *  HDCP interface: EEPROM write function.
 *  @param  u8Offset 	begin address to write.
 *  @param  u4MemPtr 	user's buffer address.
 *  @param  u4MemLen 	writedata length in bytes.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
EXTERN INT32 EEPHDCP_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
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
INT32 EEPVGAEDID_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

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
INT32 EEPVGAEDID_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);
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
INT32 EEPHDMIEDID_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

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
INT32 EEPHDMIEDID_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen);

//-------------------------------------------------------------------------
/** EEPROM_X_Read
 *  Generic EEPROM read function.
 *  @param  u8Offset    begin address to read.
 *  @param  u4MemPtr    user's buffer address.
 *  @param  u4MemLen    read data length in bytes.
 *  @param  u1Bus       SIF bus ID
 *  @param  u2ClkDiv    EEPROM clock divider
 *  @param  u1Addr      EEPROM SIF device address
 *  @param  u4Size      EEPROM Size
 *  @retval   0     SUCCESS.
 *  @retval   1     FAIL.
 */
//-------------------------------------------------------------------------
EXTERN INT32 EEPROM_X_Read(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen,
                           UINT8 u1Bus, UINT16 u2ClkDiv, UINT8 u1Addr, UINT32 u4Size);

//-------------------------------------------------------------------------
/** EEPROM_X_Write
 *  Generic EEPROM write function.
 *  @param  u8Offset    begin address to write.
 *  @param  u4MemPtr    user's buffer address.
 *  @param  u4MemLen    writedata length in bytes.
 *  @param  u1Bus       SIF bus ID
 *  @param  u2ClkDiv    EEPROM clock divider
 *  @param  u1Addr      EEPROM SIF device address
 *  @param  u4Size      EEPROM Size
 *  @param  i4SysWP     EEPROM write protect GPIO pin number
 *  @param  i4SysWPEnable EEPROM write protect enable polarity
 *  @retval   0     SUCCESS.
 *  @retval   1     FAIL.
 */
//-------------------------------------------------------------------------
EXTERN INT32 EEPROM_X_Write(UINT64 u8Offset, UINT32 u4MemPtr, UINT32 u4MemLen,
                            UINT8 u1Bus, UINT16 u2ClkDiv, UINT8 u1Addr, UINT32 u4Size,
                            INT32 i4SysWP, INT32 i4SysWPEnable);

/** EEPHDCP_Init
 *  HDCP interface: load EEPROM HDCP content to internal HDCP SRAM.
 *  @param  void.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
EXTERN INT32 EEPHDCP_Init(void);

/** EEPHDCP_SetSram
 *  HDCP interface: Turn on/off interanl HDCP slave enable bit
 *  @param  u4On 	1: HDMI will access internal SRAM.
 *                             0: HDMI will access external EEPROM.
 *  @param  u4DevAddr  internal HDCP Slave device address.
 *  @retval   0		SUCCESS.
 *  @retval   1		FAIL.
 */
EXTERN INT32 EEPHDCP_SetSramDevice(UINT32 u4On, UINT32 u4DevAddr);

EXTERN INT32 EEPROM_SetShadowMode( INT32 * pi4_mode );
EXTERN INT32 EEPROM_GetShadowMode(void);

EXTERN INT32 EEPDTV_Correction(DTVCFG_T *prDtvCfg);
EXTERN INT32 EEPDTV_GetCfg(DTVCFG_T *prDtvCfg);
EXTERN INT32 EEPDTV_SetCfg(DTVCFG_T *prDtvCfg);

#endif // EEPROM_IF_H
