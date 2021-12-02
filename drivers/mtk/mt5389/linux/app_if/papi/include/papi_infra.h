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
/* (c) 2008 Koninklijke Philips Electronics N.V., All rights reserved
 * 
 * This source code and any compilation or derivative thereof is the
 * proprietary information of Koninklijke Philips Electronics N.V. and is
 * confidential in nature.
 * Under no circumstances is this software to be exposed to or placed under an
 * Open Source License of any type without the expressed written permission of
 * Koninklijke Philips Electronics N.V.
 */

#include "papi_types.h"

extern Bool _INFRA_Initialize(void);

/**
 *   @file papi_infra.h
 */
/** @defgroup papi_infra  */

/*
 *  interface cecack of type IPlfApiCecLinkReceiveAcknowledge
 */

/** @defgroup papi_infra_cec papi_infra_cec: HDMI-CEC
  * @ingroup papi_infra */

/** @defgroup papi_infra_cec_Functions Functions 
  * @ingroup papi_infra_cec */

/**  @ingroup papi_infra_cec_Functions */
/**  Notification that CEC message is processed/acknowledged. */ 
extern FResult papi_infra_cecack_MessageProcessed( void );



/*
 *  interface cecctrl of type IPlfApiCecLinkControl
 */

/** @defgroup papi_infra_cec papi_infra_cec: HDMI-CEC
  * @ingroup papi_infra */

/** @defgroup papi_infra_cec_Functions Functions 
  * @ingroup papi_infra_cec */

/**  @ingroup papi_infra_cec_Functions */
/**  Get buffered CEC message. */ 
/**  @param cec_message Pointer to the message buffer. */
/**  @param msg_length Message length. */
extern FResult papi_infra_cecctrl_GetBufferedMsg( Nat8 * cec_message, Nat8 * msg_length );

/**  @ingroup papi_infra_cec_Functions */
extern FResult papi_infra_cecctrl_GetDeviceVendorID( Nat8 * vendor_id, int * length );

/**  @ingroup papi_infra_cec_Functions */
extern FResult papi_infra_cecctrl_SetDeviceVendorID( Nat8 * vendor_id, int length );

/**  @ingroup papi_infra_cec_Functions */
/**  Disable HDMI-CEC link device. */ 
extern FResult papi_infra_cecctrl_Disable( void );

/**  @ingroup papi_infra_cec_Functions */
/**  Disable notification via CecLinkReceiveNotify(). */ 
extern FResult papi_infra_cecctrl_DisableReceive( void );

/**  @ingroup papi_infra_cec_Functions */
/**  Disable notification via CecLinkTransmitStatusNotify(). */ 
extern FResult papi_infra_cecctrl_DisableTransmitStatus( void );

/**  @ingroup papi_infra_cec_Functions */
/**  Enable HDMI-CEC link device. */ 
extern FResult papi_infra_cecctrl_Enable( void );

/**  @ingroup papi_infra_cec_Functions */
/**  Get HDMI-CEC logical address. */ 
/**  @param retval Return value with the HDMI-CEC logical address. */
extern FResult papi_infra_cecctrl_GetLogicalAddress( CecLogicalAddress * retval );

/**  @ingroup papi_infra_cec_Functions */
/**  Set HDMI-CEC logical address. */ 
/**  @param address HDMI-CEC logical address to be set. */
extern FResult papi_infra_cecctrl_SetLogicalAddress( CecLogicalAddress address );

/**  @ingroup papi_infra_cec_Functions */
extern FResult papi_infra_cecctrl_GetOSDName( char * osd_name, int * length );

/**  @ingroup papi_infra_cec_Functions */
extern FResult papi_infra_cecctrl_SetOSDName( char * osd_name, int length );

/**  @ingroup papi_infra_cec_Functions */
/**  Enable notification via CecLinkReceiveNotify(). */ 
extern FResult papi_infra_cecctrl_EnableReceive( void );

/**  @ingroup papi_infra_cec_Functions */
/**  Enable notification via CecLinkTransmitStatusNotify(). */ 
extern FResult papi_infra_cecctrl_EnableTransmitStatus( void );

/**  @ingroup papi_infra_cec_Functions */
extern FResult papi_infra_cecctrl_GetVersion( CecVersion * retval );

/**  @ingroup papi_infra_cec_Functions */
extern FResult papi_infra_cecctrl_SetVersion( CecVersion version );



/*
 *  interface cectx of type IPlfApiCecLinkTransmit
 */

/** @defgroup papi_infra_cec papi_infra_cec: HDMI-CEC
  * @ingroup papi_infra */

/** @defgroup papi_infra_cec_Functions Functions 
  * @ingroup papi_infra_cec */

/**  @ingroup papi_infra_cec_Functions */
/**  Transmit a CEC message on the CEC bus. */ 
/**  @param size Size of the message buffer. */
/**  @param msg Pointer to the memory buffer containing the CEC message. */
/**  @param retval Standard return value. */
extern FResult papi_infra_cectx_Transmit( int size, Nat8 * msg, Bool * retval );



/*
 *  interface clk of type IPlfApiClock
 */

/** @defgroup papi_infra_clk papi_infra_clk: Clock
  * @ingroup papi_infra */

/** @defgroup papi_infra_clk_Functions Functions 
  * @ingroup papi_infra_clk */

/**  @ingroup papi_infra_clk_Functions */
/**  Get the alarm time. */ 
/**  @param retval Return value with the alarm time that has been set. */
extern FResult papi_infra_clk_GetAlarm( DateTime * retval );

/**  @ingroup papi_infra_clk_Functions */
/**  Test whether the alarm has been set. */ 
/**  @param retval Return value is true if alarm is set, false otherwise. */
extern FResult papi_infra_clk_AlarmIsSet( Bool * retval );

/**  @ingroup papi_infra_clk_Functions */
/**  Set the alarm. When the specified time is reached, a corresponding notification function is called. During standby, reaching the specified time will cause a wakeup of the set without a notification function being called. If the set wakes up for another reason, the alarm is cancelled. */ 
/**  @param alarm Input parameter with the alarm time. */
extern FResult papi_infra_clk_SetAlarm( DateTime alarm );

/**  @ingroup papi_infra_clk_Functions */
/**  Cancel the alarm. */ 
extern FResult papi_infra_clk_CancelAlarm( void );

/**  @ingroup papi_infra_clk_Functions */
/**  Verify whether the clock has been set. Clock has to be set anew after a power off. */ 
/**  @param retval True if the clock has been set, false otherwise. */
extern FResult papi_infra_clk_ClockIsSet( Bool * retval );

/**  @ingroup papi_infra_clk_Functions */
/**  Set the clock. DateTime is an integer-based value denoting number of seconds since day-zero, which is fixed at January 1st 1996. */ 
/**  @param utc Input parameter with the coordinated universal date and time (UTC). */
/**  @param lto Input parameter with the local time offset. */
extern FResult papi_infra_clk_SetClock( DateTime utc, DateTime lto );

/**  @ingroup papi_infra_clk_Functions */
/**  Returns the local date and time. */ 
/**  @param retval Output parameter with the local date and time. */
extern FResult papi_infra_clk_GetLocalTime( DateTime * retval );

/**  @ingroup papi_infra_clk_Functions */
/**  Returns the local time offset. */ 
/**  @param retval Output parameter with the local time offset. */
extern FResult papi_infra_clk_GetLocalTimeOffset( DateTime * retval );

/**  @ingroup papi_infra_clk_Functions */
/**  Enable/disable whether the clock has to keep running during standby. */ 
/**  @param enable True to keep the clock running during standby, false otherwise. */
extern FResult papi_infra_clk_RunDuringStandby( Bool enable );

/**  @ingroup papi_infra_clk_Functions */
/**  Returns the number of seconds since wakeup. */ 
/**  @param retval Output parameter returning the number of seconds since wakeup. */
extern FResult papi_infra_clk_SecondsSinceWakeup( Nat32 * retval );

/**  @ingroup papi_infra_clk_Functions */
/**  Returns the coordinated universal date and time (UTC). */ 
/**  @param retval Output parameter with the coordinated universal date and time (UTC). */
extern FResult papi_infra_clk_GetUniversalTime( DateTime * retval );



/*
 *  interface gio of type IPlfApiGeneralIo
 */

/** @defgroup papi_infra_gio papi_infra_gio: GeneralIo
  * @ingroup papi_infra */

/** @defgroup papi_infra_gio_Datatypes Datatypes 
  * @ingroup papi_infra_gio */
/**
 *   @defgroup papi_infra_gio_IoMode Mode IO Constants
 *   @ingroup papi_infra_gio_Datatypes
 *   Mode IO Constants
 *   @{ */
 /**  The pin is in input mode only. */
#define papi_infra_gio_HighImpedance 0
 /**  The pin is in output mode. Setting the pin to 1 leaves the line floating and allows the external circuitry to pull the line to 0. */
#define papi_infra_gio_OpenDrain 1
 /**  Setting the pin to 1 actively switches the line to 1 and setting the pin to 0 actively switches the line to 0. */
#define papi_infra_gio_PushPull 2
 /**  The pin operates in push-pull mode for one clock cycle after a change in state from 0 to 1 and then reverts to open drain mode of operation. */
#define papi_infra_gio_QuasiBidirectional 3
/*   @} */

/** @defgroup papi_infra_gio_Functions Functions 
  * @ingroup papi_infra_gio */

/**  @ingroup papi_infra_gio_Functions */
/**  Returns the latched value of the pin identified with pinid set using eg PinSetLatch. */ 
/**  @param pinid The id of a pin in a port. */
/**  @param retval The latched value of pin pinid  (0 or 1). */
extern FResult papi_infra_gio_PinGetLatch( int pinid, int * retval );

/**  @ingroup papi_infra_gio_Functions */
/**  Returns the line value of the pin identified with pinid. */ 
/**  @param pinid The id of a pin in a port. */
/**  @param retval The value of the line of pin pinid (0 or 1). */
extern FResult papi_infra_gio_PinGetLine( int pinid, int * retval );

/**  @ingroup papi_infra_gio_Functions */
/**  Returns the mode of the pin identified with pinid. */ 
/**  @param pinid The id of a pin in a port. */
/**  @param retval The mode of pin pinid. See datatype \ref papi_infra_gio_IoMode. */
extern FResult papi_infra_gio_PinGetMode( int pinid, int * retval );

/**  @ingroup papi_infra_gio_Functions */
/**  Sets the latch of the pin identified with pinid to value. */ 
/**  @param pinid The id of a pin in a port. */
/**  @param value The value written to the latch (0 or 1). */
extern FResult papi_infra_gio_PinSetLatch( int pinid, int value );

/**  @ingroup papi_infra_gio_Functions */
/**  Sets the mode of the pin identified with pinid to mode. */ 
/**  @param pinid The id of a pin in a port. */
/**  @param mode The mode for the pin. See datatype \ref papi_infra_gio_IoMode. */
extern FResult papi_infra_gio_PinSetMode( int pinid, int mode );



/*
 *  interface i2cc of type IPlfApiI2cControl
 */

/** @defgroup papi_infra_i2cc papi_infra_i2cc: I2cControl
  * @ingroup papi_infra */

/** @defgroup papi_infra_i2cc_Datatypes Datatypes 
  * @ingroup papi_infra_i2cc */
/**
 *   @defgroup papi_infra_i2cc_I2CCtrlRetParameters I2C Control Return Parameters
 *   @ingroup papi_infra_i2cc_Datatypes
 *   I2C Control Return Parameters.
 *   @{ */
#define papi_infra_i2cc_Ok 0
#define papi_infra_i2cc_IllegalUse 1
#define papi_infra_i2cc_BusError 2
/*   @} */

/** @defgroup papi_infra_i2cc_Functions Functions 
  * @ingroup papi_infra_i2cc */

/**  @ingroup papi_infra_i2cc_Functions */
/**  This function is used to set the speed (bit rate) of the bus. */ 
/**  @param bus I2C bus indication. */
/**  @param bitrate Speed in terms of KHz. */
/**  @param retval Returns Ok  if success, corresponding error ID otherwise. If bitrate is not within the range, IsI2cIllegalUse is returned. See datatype \ref papi_infra_i2cc_I2CCtrlRetParameters. */
extern FResult papi_infra_i2cc_SetBitRate( int bus, int bitrate, FResult * retval );

/**  @ingroup papi_infra_i2cc_Functions */
/**  These functions are used to block (BlockBus) and unblock (UnblockBus) the specified bus by respectively generating a start and a stop condition. */ 
/**  @param bus I2C bus indication. */
/**  @param retval The functions return TRUE if is successful and FALSE otherwise. */
extern FResult papi_infra_i2cc_BlockBus( int bus, Bool * retval );

/**  @ingroup papi_infra_i2cc_Functions */
/**  Retrieve the blocked status of an I2C bus. */ 
/**  @param retval Standard return value. */
extern FResult papi_infra_i2cc_GetBlocked( int * retval );

/**
 *   I2C Control Mode Return Parameters.
 *   @{ */
#define MasterEnabled 0x1
#define SlaveEnabled  0x2
#define Idle                0x4
#define Blocked         0x8
/*   @} */

/**  @ingroup papi_infra_i2cc_Functions */
/**  This function is used to request the current control mode of the specified bus. */ 
/**  @param bus I2C bus indication. */
/**  @param retval The current active control mode being a possible logical OR of the following parameters: MasterEnabled, SlaveEnabled, Idle, Blocked. */
extern FResult papi_infra_i2cc_GetControlWord( int bus, int * retval );

/**  @ingroup papi_infra_i2cc_Functions */
/**  These functions are used to set (EnterIdleMode) and reset (LeaveIdleMode) the idle mode of the specified I2C bus. The idle mode is initially not active. In idle mode all master write and read operations on the specified bus will return the status I2cOk. */ 
/**  @param bus I2C bus indication. */
/**  @param retval The functions return TRUE if is successful and FALSE otherwise. */
extern FResult papi_infra_i2cc_EnterIdleMode( int bus, Bool * retval );

/**  @ingroup papi_infra_i2cc_Functions */
/**  Retrieve the idle mode of an I2C bus. */ 
/**  @param retval Standard return value. */
extern FResult papi_infra_i2cc_GetIdle( int * retval );

/**  @ingroup papi_infra_i2cc_Functions */
/**  These functions are used to set (EnterIdleMode) and reset (LeaveIdleMode) the idle mode of the specified I2C bus. The idle mode is initially not active. In idle mode all master write and read operations on the specified bus will return the status I2cOk. */ 
/**  @param bus I2C bus indication. */
/**  @param retval The functions return TRUE if is successful and FALSE otherwise. */
extern FResult papi_infra_i2cc_LeaveIdleMode( int bus, Bool * retval );

/**  @ingroup papi_infra_i2cc_Functions */
/**  These functions are used to enable (MasterEnable) and disable (MasterDisable) the I2C master mode of the specified bus. The master mode of a bus is initially disabled. If disabled, all master write and read operations on the specified bus will return the status I2cBusError. */ 
/**  @param bus I2C bus indication. */
/**  @param retval The functions return TRUE if is successful and FALSE otherwise. */
extern FResult papi_infra_i2cc_MasterDisable( int bus, Bool * retval );

/**  @ingroup papi_infra_i2cc_Functions */
/**  These functions are used to enable (MasterEnable) and disable (MasterDisable) the I2C master mode of the specified bus. The master mode of a bus is initially disabled. If disabled, all master write and read operations on the specified bus will return the status I2cBusError. */ 
/**  @param bus I2C bus indication. */
/**  @param retval The functions return TRUE if is successful and FALSE otherwise. */
extern FResult papi_infra_i2cc_MasterEnable( int bus, Bool * retval );

/**  @ingroup papi_infra_i2cc_Functions */
/**  Retrieve the master enable status of an I2C bus. */ 
/**  @param retval Standard return value. */
extern FResult papi_infra_i2cc_GetMasterEnabled( int * retval );

/**  @ingroup papi_infra_i2cc_Functions */
/**  This function is used to set the timeout value for master transfer for a given bus. */ 
/**  @param bus I2C bus indication. */
/**  @param timeout Timeout in seconds. */
/**  @param retval Returns Ok  if success, corresponding error ID otherwise. See datatype \ref papi_infra_i2cc_I2CCtrlRetParameters. */
extern FResult papi_infra_i2cc_SetMasterTimeout( int bus, int timeout, FResult * retval );

/**  @ingroup papi_infra_i2cc_Functions */
/**  These functions are used to enable (SlaveEnable) and disable (SlaveDisable) the I2C slave mode of the specified bus. The slave mode is initially disabled. */ 
/**  @param bus I2C bus indication. */
/**  @param retval The functions return TRUE if is successful and FALSE otherwise. */
extern FResult papi_infra_i2cc_SlaveDisable( int bus, Bool * retval );

/**  @ingroup papi_infra_i2cc_Functions */
/**  These functions are used to enable (SlaveEnable) and disable (SlaveDisable) the I2C slave mode of the specified bus. The slave mode is initially disabled. */ 
/**  @param address A 10-bits I2C slave address and an indication of the I2C bus the slave is connected to. */
/**  @param buf A pointer to a buffer that will hold the bytes to be transmitted to and received from a master device. */
/**  @param size The size in bytes of the I2C message buffer. */
/**  @param retval The functions return TRUE if is successful and FALSE otherwise. */
extern FResult papi_infra_i2cc_SlaveEnable( int address, Nat8 * buf, int size, Bool * retval );

/**  @ingroup papi_infra_i2cc_Functions */
/**  Retrieve the slave enable status of an I2C bus. */ 
/**  @param retval Standard return value. */
extern FResult papi_infra_i2cc_GetSlaveEnabled( int * retval );

/**  @ingroup papi_infra_i2cc_Functions */
/**  These functions are used to block (BlockBus) and unblock (UnblockBus) the specified bus by respectively generating a start and a stop condition. */ 
/**  @param bus I2C bus indication. */
/**  @param retval The functions return TRUE if is successful and FALSE otherwise. */
extern FResult papi_infra_i2cc_UnblockBus( int bus, Bool * retval );



/*
 *  interface i2cm of type IPlfApiI2cMaster
 */

/** @defgroup papi_infra_i2cm papi_infra_i2cm: I2cMaster
  * @ingroup papi_infra */

/** @defgroup papi_infra_i2cm_Datatypes Datatypes 
  * @ingroup papi_infra_i2cm */
/**
 *   @defgroup papi_infra_i2cm_I2CMasterRetParameters I2C Master Return Parameters
 *   @ingroup papi_infra_i2cm_Datatypes
 *   I2C Master Return Parameters
 *   @{ */
#define papi_infra_i2cm_Ok 0
#define papi_infra_i2cm_BusError 1
#define papi_infra_i2cm_StartFailed 2
#define papi_infra_i2cm_DeviceError 3
#define papi_infra_i2cm_WriteError 4
#define papi_infra_i2cm_ReadError 5
#define papi_infra_i2cm_StopFailed 6
#define papi_infra_i2cm_Timeout 7
#define papi_infra_i2cm_OtherError 8
#define papi_infra_i2cm_IllegalUse 9
#define papi_infra_i2cm_HardwareError 10
#define papi_infra_i2cm_NoBusPower 11
#define papi_infra_i2cm_BusBusyTimeout 12
#define papi_infra_i2cm_MasterModeClockStretchTimeout 13
#define papi_infra_i2cm_SlaveModeClockSignalTimeout 14
/*   @} */

/** @defgroup papi_infra_i2cm_Functions Functions 
  * @ingroup papi_infra_i2cm */

/**  @ingroup papi_infra_i2cm_Functions */
/**  This function reads len bytes from the device with the I2C address address into the buffer pointed to by msg. */ 
/**  @param address The I2C bus type indication and I2C address of the addressed device. */
/**  @param msg A pointer to a buffer to store the received (read) data. */
/**  @param len The number of bytes to be read. */
/**  @param retval I2C Master Return Parameter. See datatype \ref papi_infra_i2cm_I2CMasterRetParameters. */
extern FResult papi_infra_i2cm_Read( int address, Nat8 * msg, int len, FResult * retval );

/**  @ingroup papi_infra_i2cm_Functions */
/**  These functions write len bytes to the device with the I2C address address from the RAM buffer pointed to by msg in case of Write and from the ROM buffer pointed to by msg in case of WriteRom. */ 
/**  @param address The I2C bus type indication and I2C address of the addressed device. */
/**  @param msg A pointer to a buffer containing the data to be written. */
/**  @param len The number of bytes to be written. */
/**  @param retval I2C Master Return Parameter. See datatype \ref papi_infra_i2cm_I2CMasterRetParameters. */
extern FResult papi_infra_i2cm_Write( int address, Nat8 * msg, int len, FResult * retval );

/**  @ingroup papi_infra_i2cm_Functions */
/**  This function writes the I2C message pointed to by msgw (of length lenw) to the device with address address, and then reads lenr bytes from the same device into the buffer pointed to by msgr, by generating a repeated start condition (a start condition without a prior stop condition). This ensures that writing and reading occurs as a single atomic action. Some devices require this way of writing/reading, e.g. to write a register address first, and then read the addressed register. */ 
/**  @param address The I2C bus type indication and I2C address of the addressed device. */
/**  @param msgw A pointer to a buffer containing the data to be written. */
/**  @param lenw The number of bytes to be written. */
/**  @param msgr A pointer to a buffer to store the received (read) data. */
/**  @param lenr The number of bytes to be read. */
/**  @param retval I2C Master Return Parameter. See datatype \ref papi_infra_i2cm_I2CMasterRetParameters. */
extern FResult papi_infra_i2cm_WriteRead( int address, Nat8 * msgw, int lenw, Nat8 * msgr, int lenr, FResult * retval );



/*
 *  interface led of type IPlfApiLedControl
 */

/** @defgroup papi_infra_led papi_infra_led: LedControl
  * @ingroup papi_infra */

/** @defgroup papi_infra_led_Functions Functions 
  * @ingroup papi_infra_led */

/**  @ingroup papi_infra_led_Functions */
extern FResult papi_infra_led_SetBlinking( int led, int msecson, int msecsoff );

/**  @ingroup papi_infra_led_Functions */
extern FResult papi_infra_led_SetFlash( int led, int msecs );

/**  @ingroup papi_infra_led_Functions */
extern FResult papi_infra_led_SetPermanent( int led, Bool offon );

#define papi_infra_led_SBYLed 0
#define papi_infra_led_MainLed 1


/*
 *  interface shmem of type IPlfApiSharedMemory
 */

/** @defgroup papi_infra_shmem papi_infra_shmem: SharedMemory
  * @ingroup papi_infra */

/** @defgroup papi_infra_shmem_Datatypes Datatypes 
  * @ingroup papi_infra_shmem */
/**
 *   @defgroup papi_infra_shmem_Error Shared Memory Return Parameters
 *   @ingroup papi_infra_shmem_Datatypes
 *   Shared Memory Return Parameters.
 *   @{ */
#define papi_infra_shmem_ErrorOk 0
#define papi_infra_shmem_ErrorUnknown -1001
#define papi_infra_shmem_ErrorHandleIsNull -1002
#define papi_infra_shmem_ErrorAddressIsNull -1003
#define papi_infra_shmem_ErrorInvalidFlags -1004
#define papi_infra_shmem_ErrorInvalidHandle -1005
#define papi_infra_shmem_ErrorOutOfMemory -1006
/*   @} */

/**
 *   @defgroup papi_infra_shmem_Malloc Malloc Parameters
 *   @ingroup papi_infra_shmem_Datatypes
 *   Malloc Flags.
 *   @{ */
 /**  The memory block is not cache aligned nor is its contents cleared to 0 */
#define papi_infra_shmem_MallocNone 0
 /**  The memory block contents is cleared to 0. */
#define papi_infra_shmem_MallocCleared 1
 /**  The memory block is page aligned. */
#define papi_infra_shmem_MallocPageAligned 2
/*   @} */

/** @defgroup papi_infra_shmem_Functions Functions 
  * @ingroup papi_infra_shmem */

/**  @ingroup papi_infra_shmem_Functions */
/**  Flushes the cache. */ 
/**  @param address A pointer to an allocated block of memory. */
/**  @param size Size in bytes. */
/**  @param retval Standard Return Value. */
extern FResult papi_infra_shmem_CacheFlush( Address address, Nat32 size, FResult * retval );

/**  @ingroup papi_infra_shmem_Functions */
/**  Invalidates the cache. */ 
/**  @param address A pointer to an allocated block of memory. */
/**  @param size Size in bytes. */
/**  @param retval Standard Return Value. */
extern FResult papi_infra_shmem_CacheInvalidate( Address address, Nat32 size, FResult * retval );

/**  @ingroup papi_infra_shmem_Functions */
/**  Deallocates or frees the memory block identified by ptr, making the space available for further allocation. If ptr is NULL, free returns immediately. */ 
/**  @param address A pointer to an allocated block of memory. */
/**  @param retval Standard Return Value. */
extern FResult papi_infra_shmem_Free( Address address, FResult * retval );

/**  @ingroup papi_infra_shmem_Functions */
/**  Allocates space for a block of size bytes. The block is not initialized (to e.g. zeros). Blocks are always aligned on a 4-byte boundary and they do not span segments. If size is 0, malloc() returns NULL (unlike other implementations where this behavior is undefined or an empty but unique block is allocated). For reasons of symmetry, Free() will accept a NULL pointer. */ 
/**  @param handle Memory space handle. */
/**  @param size Size in bytes. */
/**  @param address A pointer to an allocated block of memory. */
/**  @param flags Malloc Flag. See datatype \ref papi_infra_shmem_Malloc. */
/**  @param retval Standard Return Value. */
extern FResult papi_infra_shmem_Malloc( Nat32 handle, Nat32 size, Address * address, Nat32 flags, FResult * retval );

/**  @ingroup papi_infra_shmem_Functions */
/**  Allocates space for a block of size bytes. The block is not initialized (to e.g. zeros). Blocks are always aligned on a 4-byte boundary and they do not span segments. If size is 0, malloc() returns NULL (unlike other implementations where this behavior is undefined or an empty but unique block is allocated). For reasons of symmetry, Free() will accept a NULL pointer. */ 
/**  @param handle Memory space handle. */
/**  @param size Size in bytes. */
/**  @param address A pointer to an allocated block of memory. */
/**  @param flags Malloc Flag. See datatype \ref papi_infra_shmem_Malloc. */
/**  @param alignment Memory Alignment. */
/**  @param retval Standard Return Value. */
extern FResult papi_infra_shmem_MallocAlign( Nat32 handle, Nat32 size, Address * address, Nat32 flags, Nat32 alignment, FResult * retval );

/**  @ingroup papi_infra_shmem_Functions */
/**  Creates a memory space. */ 
/**  @param handle Memory space handle. */
/**  @param size Size in bytes. */
/**  @param retval Standard Return Value. */
extern FResult papi_infra_shmem_MmspCreate( Nat32 * handle, Nat32 size, FResult * retval );

/**  @ingroup papi_infra_shmem_Functions */
/**  Deletes a memory space. */ 
/**  @param handle Memory space handle. */
/**  @param retval Standard Return Value. */
/**  @param retval Standard Return Value. */
extern FResult papi_infra_shmem_MmspDelete( Nat32 handle, FResult * retval );

#define papi_infra_shmem_iVersion 1


/*
 *  interface uartprint of type IPlfApiEnable
 */

/** @defgroup papi_infra_uartprint papi_infra_uartprint: Enable
  * @ingroup papi_infra */

/** @defgroup papi_infra_uartprint_Functions Functions 
  * @ingroup papi_infra_uartprint */

/**  @ingroup papi_infra_uartprint_Functions */
extern FResult papi_infra_uartprint_Disable( void );

/**  @ingroup papi_infra_uartprint_Functions */
extern FResult papi_infra_uartprint_Enable( void );

/**  @ingroup papi_infra_uartprint_Functions */
extern FResult papi_infra_uartprint_IsEnabled( Bool * retval );



/*
 *  interface cecrxntf of type IPlfApiCecLinkReceiveNotify
 */

/** @defgroup papi_infra_cec papi_infra_cec: HDMI-CEC
  * @ingroup papi_infra */

/** @defgroup papi_infra_cec_Notifications Notifications 
  * @ingroup papi_infra_cec */

/**  @ingroup papi_infra_cec_Notifications */
/**  Notification of the reception of a CEC message. */ 
/**  @param size Size of the message buffer. */
/**  @param pMsg Pointer to the message buffer containing the CEC message. */
extern void papi_infra_cecrxntf_OnReceived( int size, Nat8 * pMsg );



/*
 *  interface cectxntf of type IPlfApiCecLinkTransmitStatusNotify
 */

/** @defgroup papi_infra_cec papi_infra_cec: HDMI-CEC
  * @ingroup papi_infra */

/** @defgroup papi_infra_cec_Datatypes Datatypes 
  * @ingroup papi_infra_cec */
/**
 *   @defgroup papi_infra_cec_Cec CEC Return Parameters
 *   @ingroup papi_infra_cec_Datatypes
 *   CEC Return Parameters
 *   @{ */
#define papi_infra_cectxntf_Ok 0
#define papi_infra_cectxntf_IsCecBusBlocked 1
#define papi_infra_cectxntf_IsCecDeviceBusy 2
#define papi_infra_cectxntf_IsCecMessageRejected 3
#define papi_infra_cectxntf_IsCecDeviceNotPresent 4
/*   @} */

/** @defgroup papi_infra_cec_Notifications Notifications 
  * @ingroup papi_infra_cec */

/**  @ingroup papi_infra_cec_Notifications */
/**  Notification that CEC transmit is completed. */ 
/**  @param result CEC Return Parameter. See datatype \ref papi_infra_cec_Cec. */
extern void papi_infra_cectxntf_OnTransmitCompleted( FResult result );



/*
 *  interface clkntf of type IPlfApiClockNotify
 */

/** @defgroup papi_infra_clk papi_infra_clk: Clock
  * @ingroup papi_infra */

/** @defgroup papi_infra_clkntf_Notifications Notifications 
  * @ingroup papi_infra_clk */

/**  @ingroup papi_infra_clkntf_Notifications */
/**  This notification is called when an alarm is reached. */ 
extern void papi_infra_clkntf_OnAlarm( void );

/**  @ingroup papi_infra_clkntf_Notifications */
/**  This notification is called when the clock is set anew after it has been set the first time. */ 
/**  @param old Old time. */
/**  @param new New time. */
extern void papi_infra_clkntf_OnClockChanged( DateTime old, DateTime new );


