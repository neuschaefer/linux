/**
 * Copyright 2010 Vestel Elektronik AS. All rights reserved.
 *
 * @file ice_internet.h
 *
 * @brief ICE Interface for internet related work
 *
 */

#ifndef CABOT_ICE_ICE_INTERNET_H_INCLUDED
#define CABOT_ICE_ICE_INTERNET_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Pings destionation once.
 *
 * @param destination Ping destionation
 * 
 * @param timeout max. waiting time for a ping. If destination is unreachable, timeout is maximum waiting time.
 * 
 * @returns On success, returns average delay time, otherwise -1
 */
frost_int ICE_InternetPing(frost_int8 const * const destination, frost_uint const timeout);

/**
 * @brief Calculate download speed in Kbit/s
 *
 * @param url Destination address of file to be downloaded
 * 
 * @param timeout max. waiting time for download process.
 * 
 * @param user_agent User agent to be sent in the HTTP request
 * 
 * @returns On success, returns download speed, otherwise -1.
 * 
 * @note If download is slow enough that timeout expires, it calculates speed from partial downloaded data.
 */
frost_int ICE_InternetDoSpeedTest(frost_int8 const * const url, frost_uint const timeout, frost_int8 const * const user_agent);

/**
 * @brief Checks whether Internet connection is successfull or not.
 *
 * @param url Destination address of file to be downloaded
 * 
 * @param timeout max. waiting time for download process.
 * 
 * @param user_agent User agent to be sent in the HTTP request
 * 
 * @returns On success, returns 1, on error -1, 0 if can not find the file on the server.
*/

frost_bool ICE_CheckInternetConnection(frost_int8 const * const url, frost_uint const timeout, frost_int8 const * const user_agent);

/**
 * @brief Log TVs MAC address on predefined server using http post method.
 *
 * @returns On success, returns frost_true, otherwise frost_false.
 */
frost_bool ICE_InternetLogTVsMAC(void);


/**
 * @brief Notify Vestel Servers when Tv is connected to the Internet.
 */
void ICE_InternetPostConnectivityLog(const frost_int8* message, frost_uint length);


/**
 * Not to be implemented
 */
char * ICE_InternetGetDefaultPath(void);

#ifdef __cplusplus
}
#endif

#endif
