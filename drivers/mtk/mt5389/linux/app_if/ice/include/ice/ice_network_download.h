/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 * @brief Defines ICE Networking Download / Upload API
 *
 * This API only needs to be implemented if you want to support
 * download & upload operations.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 *
 * @note This API is work in progress, and may be significantly different
 *       in the final release version.
 */

#ifndef CABOT_ICE_ICE_NETWORK_DOWNLOAD_H_INCLUDED
#define CABOT_ICE_ICE_NETWORK_DOWNLOAD_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
*
*
*/
typedef struct ICE_DownloadHandle ICE_DownloadHandle;

/**
 * 
 */
typedef struct ICE_DownloadHeaderList ICE_DownloadHeaderList ;

/**
 * @brief This function will be called as the download proceeds.
 * 
 * @param handle Pointer to the download handle

 * @param data Owner data pointer

 * @param total_size size of the file to be downloaded

 * @param bytes_downloaded Bytes downloaded so far
 *
 * @return None
 */

typedef  void (*ICE_DownloadCallback)(ICE_DownloadHandle* handle, 
                                     void* callback_data, 
                                     double total_size, 
                                     double bytes_downloaded);


/**
 * @brief Initializes a file download over network.
 *
 * @note ICE_NetworkDownloadStart should be called after initialized.
 *
 * @param url URL of the file to be downloaded
 * @param target_filespec Full path of the downloaded file to be saved
 * @param username Username (Can be null)
 * @param password Password (Can be null)
 * @param resume Resumes download if true   
 * @param callback Pointer to the function that is called when there is a progress 
 * @param callback_data Data to be passed to the callback function
 * @param header_list Headers for download
 * @return download handle on success. null on error.
 */
ICE_DownloadHandle* ICE_DownloadInitialise( char const *url,                                   
                                            char const *target_filespec, 
                                            char const *username, 
                                            char const *password,
                                            frost_bool resume,
                                            ICE_DownloadCallback callback,
                                            void *callback_data,
                                            ICE_DownloadHeaderList *header_list);

/**
 * @brief Starts a file download over network.
 *
 * @param handle download handle
 *
 * @return frost_true on success.
 *
 * @note this is a blocking function. It does not return until download is completed.
 * @note This function deletes download handle on exit
 */
frost_bool ICE_DownloadStart(ICE_DownloadHandle* handle);
//frost_bool ICE_DownloadURL(  char const *url,  char const *target_filespec);
/**
 * @brief Stops a file download over network.
 *
 * @param handle download handle
 *
 * @return frost_true on success. 
 *
 */
frost_bool ICE_DownloadStop(ICE_DownloadHandle* handle);        

ICE_DownloadHeaderList* ICE_DownloadHeaderListAppend(ICE_DownloadHeaderList *list, const char *data);


#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_NETWORK_DOWNLOAD_H_INCLUDED*/
