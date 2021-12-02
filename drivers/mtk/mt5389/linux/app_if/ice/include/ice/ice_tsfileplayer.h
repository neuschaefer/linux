/**
 * Copyright 2012 Vestel Elektronik  All rights reserved.
 *
 * @file
 * @brief interface from the Eclipse system to ts file playback functionality
 */

#ifndef CABOT_ICE_ICE_TSFILEPLAYER_H_INCLUDED
#define CABOT_ICE_ICE_TSFILEPLAYER_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_tuner.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enumeration of transport stream source type.
 *
 */
typedef enum
{
    from_tuner,
    from_file
}ICE_TSFilePlayerStreamType;

/**
 * @brief A typedef for a transport stream file playback handle.
 *
 */
typedef struct ICE_TSFilePlayerHandle ICE_TSFilePlayerHandle;

/**
 * @brief Open a connection to a transport stream file playback and
 * initialise the playback device.
 *
 * Attempts to open a connection to a transport stream file and return a handle
 * for subsequent communication with the playback device.
 *
 * @return A pointer to the handle to the device on success and a
 *         NULL pointer on failure.
 */
ICE_TSFilePlayerHandle * ICE_TSFilePlayerInitialise(void);

/**
 * @brief Close a previously open connection
 *
 * Closes a device connection, such that playback functionality
 * becomes available for use by any other client such as media player.
 *
 * This may also be used before restarting the playback with different
 * file.
 *
 * @return Returns #frost_true if the tuner was successfully closed,
 *         and #frost_false if the tuner could not be closed.
 */
frost_bool ICE_TSFilePlayerClose(ICE_TSFilePlayerHandle *handle);

/**
 * @brief Attempt to tune to a given stream file
 *
 * Attempts to play a given stream file and set demux to receive
 * the sections of that stream
 *
 * @param handle A pointer to the device handle returned by the
 *               #ICE_TSFilePlayerInitialise() call.
 * @param file_name The stream file's path.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool ICE_TSFilePlayerStart(ICE_TSFilePlayerHandle *handle, char* file_name);

/*
frequency.txt
107000 "ts/Package_E_V1.2.0/CAK-MER-LSP-PSI-Cancel-080814-1505-04.mpg"
107000 "ts/Package_E_V1.2.0/CAK-MER-LSP-PSI-080818-1500-05.mpg"
*/
/**
 * @brief Find the stream file's name, absolute path, which is associated with the given
 * frequency in the given config file
 *
 * *@param handle A pointer to the device handle returned by the
 *               #ICE_TSFilePlayerInitialise() call.
 * @param channel_frequency_khz The frequency in khz
 * @param ts_frequency_file_name config file name
 *
 *  @return the stream file's name, absolute path, if exists
 *         NULL pointer on failure.
 */
char* ICE_TSFilePlayerGetTSFileName( ICE_TSFilePlayerHandle *handle, ICE_Frequency channel_frequency_khz, const char* ts_frequency_file_name );


#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_TSFILEPLAYER_H_INCLUDED*/
