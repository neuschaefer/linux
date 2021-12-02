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
 * $RCSfile: loader_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file loader_if.h
 *  Loader library interface function header file
 *  This is loader library interface function header file.
 *  Here are all interface functions for loader_main.c and loader_cmd.c.
 */


//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#ifndef LOADER_DRVIF_H
#define LOADER_DRVIF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "eeprom_if.h"
#include "loader_imghdr.h"
#include "x_dlm.h"

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/** LDR_DATA_T is loader data structure among functions.
 */
typedef struct _LdrData
{
    IMAGE_UPGRADE_HEADER_T  rLdrHdr;    ///< Image header information.
    DTVCFG_T    rDtvCfg;        ///< EEPROM boot info data structure
    UINT32      u4WakeupReason; ///< System wakeup reason info
    UINT32      u4PowerDown;    ///< Systam enter standby mode (IRRX, VGA, Keypad...)
    UINT32      u4LdrFlags;     ///< System flags for previous behavior.
    UINT32      u4ImageSrc;     ///< System image source.
    UINT32      u4ExternalStatus;    ///< System enable loader standby at AC on.
    UINT32      u4UsePowerBtnCount;     //<  Use Power button State to set standby
    UINT32      u4UsePrevPowerState;    //<  Use previous power state to set standby
    UINT32      u4UseVGAStatus;         //<  Use VGA status to enter standby mode when power key trigger.
} LDR_DATA_T;

///< defines for LDR_DATA_T.u4LdrFlags
#define LDRFLAG_UPGRADE_DONE        (1U << 0)
#define LDRFLAG_BOOTLOGO_DISABLE    (1U << 1)

///< defines for LDR_DATA_T.u4ImageSrc
#define IMAGESRC_NORFLASH           (1U << 0)
#define IMAGESRC_SMALLNAND          (1U << 1)
#define IMAGESRC_BIGNAND            (1U << 2)
#define IMAGESRC_USBFILE            (1U << 3)

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** LDR_IsEnterCli(): Loader check escape from execution to cli environment
 *  This function is to check whether uart has a ESC key in a period, then
 *  return the result.
 *
 *  @param  u4BootWaitTime  cli wait time.
 *  @retval 0               No ESC, continue to execute.
 *  @retval otherwise       break into cli mode.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 LDR_IsEnterCli(UINT32 u4BootWaitTime);

//-----------------------------------------------------------------------------
/** LDR_GetLdrData(): get the loader data structure pointer.
 *
 *  @param  no parameter
 *  @retval LDR_DATA_T pointer. the pointer to loader data structure.
 */
//-----------------------------------------------------------------------------
EXTERN LDR_DATA_T *LDR_GetLdrData(VOID);


//-----------------------------------------------------------------------------
/** LDR_EEPWriteEnable(): Loader enable/disable EEPROM write access.
 *  This function enable/disable WP Pin of EEPROM
 *  @param  fgState       The write access state
 *  @retval                   No return value.
 */
//-----------------------------------------------------------------------------
EXTERN void LDR_EEPWriteEnable(BOOL fgState);

//-----------------------------------------------------------------------------
/** LDR_PreInit(): Loader initialize some setting at the beginning.
 *  This function is going to setup the basic setting for basic operations such
 *  as DRAM priority and USB initialization settings.

 *  @param  no parameter
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 LDR_PreInit(VOID);

//-----------------------------------------------------------------------------
/** LDR_EnvInit(): Loader initialize the basic setting for loader environment.
 *  This function is going to setup the basic setting for basic operations such
 *  as 8032, eeprom power, and PDWNC initialization settings.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 LDR_EnvInit(LDR_DATA_T *prLdrData);

//-----------------------------------------------------------------------------
/** LDR_CheckUpgrade(): check it's necessary to upgrade and do it if need.
 *  This function is going to check whether there is a upgrade need at loader
 *  boot up.  If it's necessary, it will continue to go to upgrade.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
EXTERN INT32 LDR_FlashWrite(DLM_ACCESS_T, UCHAR*, INT32*, UINT32, UINT32);
EXTERN UINT32 LDR_CheckUpgrade(LDR_DATA_T *prLdrData);
EXTERN UINT32 LDR_ForceUpgrade(LDR_DATA_T *prLdrData);

//-----------------------------------------------------------------------------
/** LDR_IsGoBootup(): Decide whether go to standby or continue boot up.
 *  This function is going to check wakeup reason and eeprom status (or some
 *  external uP) to decide it should enter standby or contineu boot.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Enter standby.
 *  @retval otherwise       Continue boot.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 LDR_IsGoBootup(LDR_DATA_T *prLdrData);

//-----------------------------------------------------------------------------
/** LDR_EnableCEC(): Decide whether enable CEC or not
 *
 *  @param  fgCecEnable  Enable or disable CEC or not.
 *  @retval void
 */
//-----------------------------------------------------------------------------
EXTERN void LDR_EnableCEC(BOOL fgCecEnable);

//-----------------------------------------------------------------------------
/** LDR_EnterStandby(): Enter standby mode after calling.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 LDR_EnterStandby(LDR_DATA_T *prLdrData);

//-----------------------------------------------------------------------------
/** LDR_UpSetup(): Do necessary setup settings for image boot up.
 *  This function is going to setup boot settings, such as GPIO_Init().
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 LDR_UpSetup(LDR_DATA_T *prLdrData);

//-----------------------------------------------------------------------------
/** LDR_PanelLogo(): display logo on panel.
 *  This function is going to find out the logo location and call lvds/pmx/osd 
 *  low level driver to display logo on panel.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 LDR_PanelLogo(LDR_DATA_T *prLdrData);

//-----------------------------------------------------------------------------
/** LDR_LogoDisplay(): To display logo with panel index.
 *  This function is going to display logo at boot loader.
 *
 *  @param  u4BmpAddr       The bmp data pointer.
 *  @param  u4Width         The width info of the bmp data.
 *  @param  u4Height        The height info of the bmp data.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 LDR_LogoDisplay(LDR_DATA_T *prLdrData, UINT32 u4Idx);

//-----------------------------------------------------------------------------
/** LDR_ImageDecompress(): decompress image to dram on the offset of entry.
 *  This function is going to search the target device and find out the compressed
 *  image. Then copy to dram (if necessary) and decompress the data from dram
 *  to dram by LZHS hardware.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 LDR_ImageDecompress(LDR_DATA_T *prLdrData);

//-----------------------------------------------------------------------------
/** LDR_ImageJump(): leave loader and jump to image binary in dram.
 *
 *  @param  prLdrData       The loader global data structure pointer.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 LDR_ImageJump(LDR_DATA_T *prLdrData);


//-----------------------------------------------------------------------------
/** LDR_FlashCopy(): copy flash data from system flash cueernt sys_build used.
 *  it will automatically deside copy from NOR or NAND
 *
 *  @param  pu1Buf          destination memory buffer.
 *  @param  u4Offset        flash offset.
 *  @param  u4Size          copy size.
 *  @retval 0               Success.
 *  @retval otherwise       Failed.
 */
//-----------------------------------------------------------------------------
EXTERN UINT32 LDR_FlashCopy(UINT8* pu1Buf, UINT32 u4FlashOffset, UINT32 u4Size);

#endif /* LOADER_DRVIF_H */

