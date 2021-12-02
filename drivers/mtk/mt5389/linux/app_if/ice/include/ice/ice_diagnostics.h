/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file ice_diagnostics.h
 * @brief ICE Diagnostics Interface
 *
 * This API can be used to test software / hardware functionality
 *
 */
#ifndef CABOT_ICE_ICE_DIAGNOSTICS_H_INCLUDED
#define CABOT_ICE_ICE_DIAGNOSTICS_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include "ice/ice_device_number.h"
#include "ice/ice_tuner.h"

/**
 * @brief Test RS232 functionality
 *
 * TX and RX pins (pins 2 and 3) should be short-circuited before running the test.
 * Note that this means that trace output cannot use the serial port. The test writes
 * a byte of data out then reads a byte of data back in, and compares them.
 *
 * @return <code>true</code> if the byte received matches the byte written.
 */
frost_bool ICE_TestRS232(void);

/**
 * @brief Test RAM
 *
 * @return <code>true</code> Return true if the RAM is healty.
 */

frost_bool ICE_TestRAM(void);

/**
 * @brief Start audio and video pids start playing on the primary decoders, decoding from
 *        a specified source. This is for the purpose of testing the tuners.
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_StartVideoAndAudioPids(ICE_DeviceSource source, frost_int video_pid, frost_int audio_pid);

/**
 * @brief Stop playback of current audio and video pids on the primary decoders.
 *        This also hides the video.
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_StopVideoAndAudioPids(void);

frost_int32 ICE_isTunerI2cHealty(void);
frost_bool ICE_isHdmiI2cHealty(void);
frost_bool ICE_isIfI2cHealty(void);
frost_bool ICE_isInTestMode(void);
/**
 * @brief Test LNB Power
 *
 * @return <code>true</code> Return true if the RAM is healty.
 */
frost_bool ICE_SetLNBPower(frost_bool power_on);

/**
 * @brief Test LNB Polarization
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_SetLNBPolarization(ICE_Voltage voltage);

/**
 * @brief Test 22kHz
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_Set22kHz(frost_bool set_22kHz);

/**
 * @brief Test Tuning
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_Tune(void);

/**
 * @brief Test Network
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_TestNetwork(char* ip_address);

/**
 * @brief Test HDCP Key
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_TestHDCPKey(void);

/**
 * @brief Load HDCP Key
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_LoadHDCPKey(void);

/**
 * @brief Get product specific information such as brand name,model name,system id etc.
 *
 * @param product_info The product specific information of the device.
 *
 * @param num_of_bytes Size of the product specific information in byte.
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_GetProductInfo(frost_uint8* product_info,frost_uint32 num_of_bytes);

/**
 * @brief Test Product Info
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_TestProductInfo(void);

/**
 * @brief Load Product Info
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_LoadProductInfo(void);


/**
 * @brief Test EDID keys
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_isEdidAvailable(void);

/**
 * @brief Test HDCP keys
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_isHdcpAvailable(void);

/**
 * @brief Test DDR settings are availble in spi
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_isDDRSettingsAvailable(void);

/**
 * @brief Test CI+ credidentals available in spi
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_isCIpluscreditAvailable(void);

/**
 * @brief Enbale trace logging on USB
 *
 * @return <code>true</code> on success.
 */
frost_bool ICE_EnableUSBLogging(frost_bool enable);

/**
 * @brief get mac address from spi
 *
 * @note Gets MAC address of wired network interface.
 *
 * @return mac value.
 */
frost_int8* ICE_DiagnosticGetMacAddress(void);

/**
 * @brief Set mac address from remote interface
 * @param mac_address TV MAC  address
 */
 void ICE_SetMacAddress(const char *mac_address);//[d]:added


/**
 *@brief get serial number
 *@return serial number
 */
char* ICE_GetSerialNumber(void);

/**
 * @brief intrinsic mode state
 *
 * @return state of instrinsic mode.
 */
frost_bool ICE_ReturnIntrinsicMode(void);

/**
 * @brief Enable Uart receive mode
 *
 * @return.
 */
void ICE_EnableUartRx(frost_bool enable);

/**
 * @brief Test Uart Rx is enabled.
 *
 * @return <code>true</code> if Uart Rx is enabled.
 */
frost_bool ICE_IsUartRxEnabled(void);

/**
 * @brief Enable Telnet deamon.
 *
 * @return.
 */
void ICE_EnableTelnet(frost_bool enable);

/**
 * @brief Test Telnet is enabled.
 *
 * @return <code>true</code> if Telnet is enabled.
 */
frost_bool ICE_IsTelnetEnabled(void);

/**
 * @brief Free user space memory
 *
 * @return amount of free user space memory in kB.
 */
frost_long ICE_GetFreeMemInfo(void);

/**
 * @brief Date and Time info
 *
 * @return pointer to a string with date and time information.
 */

char *  ICE_GetDateTime(void);

/**
 * @brief create diagnostic file
 *
 * @return true if file is successfully created.
 */
frost_bool ICE_DiagnosticFileCreate(void);


/**
 * @brief append info at the end of file
 *
 * @return true if information are successfully added.
 */

frost_bool ICE_DiagnosticFileAppend(const char*);


/**
 * @brief send diagnostic log to ftp server
 *
 * @return 
 */

void ICE_SendDiagnosticLogToFTP(void);


/**
 * @brief save diagnostic log to usb device
 *
 * @return 
 */

frost_bool ICE_SaveDiagnosticFileToUsbDevice(void);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_DIAGNOSTICS_H_INCLUDED */

