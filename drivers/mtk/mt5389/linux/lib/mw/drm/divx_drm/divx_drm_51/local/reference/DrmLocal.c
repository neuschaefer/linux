/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_51/local/reference/DrmLocal.c#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/
#ifndef __KERNEL__

#include "DrmLocal.h"
#include "DrmKeyStore.h"
#include "DivXAesDecrypt.h"
#include "DivXAesEncrypt.h"
#ifdef _MTK_BUILD_
#include "libc/stdio.h"
#include "libc/string.h"
#include "os/inc/x_os.h"    /* timer */
#else
#include <stdio.h>
#include <string.h>  // strcmp 
#include <time.h>    // clock
#endif

#include "DrmConfig.h"

#include "divx_drm_glue.h"

#ifdef _WIN32_WCE
#include <Windows.h>
#include "Winbase.h"
#include "tapi.h"
#include "extapi.h"
#endif

#else

#include "../DrmLocal.h"
#include "../../DrmKeyStore.h"
#include "../../crypto/DivXAesDecrypt.h"
#include "../../crypto/DivXAesEncrypt.h"

#include "../../DrmConfig.h"

#include "../../../divx_drm_glue.h"

#endif

/***************************************************************************
 *  THESE MACROS SPECIFY THE LOCATION OF THE MEMORY FRAGMENTS
 *
 *  For secure platforms, #define SECURE_PLATFORM can be used such that
 *  the first fragment is written to secured memory
 *
 *  All other platforms should fragment the memory appropriately.
 *
 *  To define new paths and filenames for the three fragments, you must take
 *  the ASCII value of each character and add 100 decimal to it to offset the
 *  value.  This will obscure the data in the binary code such that no strings
 *  are visible in the binary.
 */
#if DRM_GO_LIVE == 1
#if DRM_MOBILE_WINDOWS_PLATFORM == 1
#define FRAG1 { 192, 180, 214, 211, 203, 214, 197, 209, 132, 170, 205, 208, 201, 215, 192, 187, 205, 210, 200, 211, 219, 215, 132, 177, 201, 200, 205, 197, 132, 180, 208, 197, 221, 201, 214, 192, 212, 208, 197, 221, 201, 214, 218, 205, 215, 146, 200, 198, 0 }
#define FRAG2 { 192, 187, 205, 210, 200, 211, 219, 215, 192, 199, 199, 211, 214, 201, 200, 209, 146, 200, 208, 208, 0 }
#define FRAG3 { 192, 187, 205, 210, 200, 211, 219, 215, 192, 183, 221, 215, 216, 201, 209, 192, 200, 201, 202, 197, 217, 208, 216, 149, 197, 146, 209, 207, 221, 0 }
#endif

#if DRM_DESKTOP_WINDOWS_PLATFORM == 1
#define FRAG1 { 167, 158, 192, 180, 214, 211, 203, 214, 197, 209, 132, 170, 205, 208, 201, 215, 192, 187, 205, 210, 200, 211, 219, 215, 132, 177, 201, 200, 205, 197, 132, 180, 208, 197, 221, 201, 214, 192, 212, 208, 197, 221, 201, 214, 218, 205, 215, 146, 200, 198, 0 }
#define FRAG2 { 167, 158, 192, 187, 205, 210, 200, 211, 219, 215, 192, 199, 199, 211, 214, 201, 200, 209, 146, 200, 208, 208, 0 }
#define FRAG3 { 167, 158, 192, 187, 205, 210, 200, 211, 219, 215, 192, 183, 221, 215, 216, 201, 209, 151, 150, 192, 209, 215, 207, 200, 209, 212, 219, 150, 146, 200, 208, 208, 0}
#endif

#if DRM_DESKTOP_LINUX_PLATFORM == 1
#error "Must define the FRAG macros to point to valid files in the file system"
#endif

#if DRM_OTHER_SECURE_PLATFORM == 1
/* #error "Must define the FRAG macros to point to valid files in the file system" */
#define FRAG1 { 212, 208, 197, 221, 201, 214, 218, 205, 215, 146, 200, 198, 0 }
#endif

#if DRM_MOBILE_SYMBIAN_PLATFORM == 1
#error "Must define the FRAG macros to point to valid files in the file system"
#endif

#else

/* TESTING PURPOSES ONLY */
#define FRAG1 { 212, 208, 197, 221, 201, 214, 218, 205, 215, 146, 200, 198, 0 }
#define FRAG2 { 199, 199, 211, 214, 201, 200, 209, 146, 200, 208, 208, 0 }
#define FRAG3 { 209, 215, 207, 200, 209, 212, 219, 150, 146, 200, 208, 208, 0 }

#endif

#if DRM_OTHER_SECURE_PLATFORM == 0
static int32_t s_readFragment( uint32_t fragmentNum,
                                uint8_t* data,
                                uint32_t dataLength )
{
    uint32_t returnCode = DRM_LOCAL_SUCCESS;
	FILE *input;
    uint8_t i;

    /* playervis.db ASCII values are + 100 to offset into non-ASCII characters */
    uint8_t filenameInAscii[3][128] = { FRAG1,
#if DRM_OTHER_SECURE_PLATFORM == 0
                                        FRAG2,
                                        FRAG3 };
#else
                                       };         
#endif
    
    
    /* must be the length of "playervis.db" + 1 for the string */
    uint8_t fileName[128] = { 0 }; 

    {
        i = 0;
        while ( filenameInAscii[fragmentNum-1][i] != 0 )
        {
            fileName[i] = (int8_t)filenameInAscii[fragmentNum-1][i] - 100;
            i++;
        }
    }
    
    input = fopen( (const char *)fileName,
                   "rb" );
    if ( input == NULL )
    {
        returnCode = DRM_ERROR_READING_MEMORY;
        return returnCode;
    }
    
    for ( i = 0; i < dataLength; i++ )
    {
        if ( fread( data + i,
                    1,
                    1,
                    input ) != 1 )
        {
            returnCode = DRM_ERROR_READING_MEMORY;
            return returnCode;
        }
    }
    
    fclose(input);

    return returnCode;
}

static int32_t s_writeFragment( uint32_t fragmentNum,
                                uint8_t* data,
                                uint32_t dataLength )
{
    uint32_t returnCode = DRM_LOCAL_SUCCESS;

    FILE *output;
    uint8_t i;

    /* playervis.db ASCII values are + 100 to offset into non-ASCII characters */
    uint8_t filenameInAscii[3][128] = { FRAG1,
#if DRM_OTHER_SECURE_PLATFORM == 0
                                        FRAG2,
                                        FRAG3 };
#else
                                       };         
#endif

    /* must be the length of "playervis.db" + 1 for the string */
    uint8_t fileName[128] = { 0 }; 

    
    {
        i = 0;
        while ( filenameInAscii[fragmentNum-1][i] != 0 )
        {
            fileName[i] = (int8_t)filenameInAscii[fragmentNum-1][i] - 100;
            i++;
        }
    }

    output = fopen( (const char *)fileName,
                    "wb");
    if ( output == NULL )
    {
        returnCode = DRM_ERROR_WRITING_MEMORY;
        return returnCode;
    }
    
    for ( i = 0; i < dataLength; i++ )
    {
        if ( fwrite( data + i,
                     1,
                     1,
                     output ) != 1 )
        {
            returnCode = DRM_ERROR_WRITING_MEMORY;
            return returnCode;
        }
    }
    
    fclose( output );

    return returnCode;

}
#endif

#if DRM_DATALOAD_USE_LHS == 1
uint32_t localGetLocalHardwareSecret( uint8_t *lhs )
{
#ifdef DRM_HARDWARE_SIMULATED
    memset( lhs,
            0xA7,
            32 );
	return 0;
#else
/* #error "localGetLocalHardwareSecret: Return the hardware secret." */
    divx_drm_hw_secret_key_glue(lhs);
    return 0;
#endif
}

#endif

uint32_t localClock( void )
{
#if 1

    return (uint32_t)x_os_get_sys_tick();

#else

#ifdef DRM_HARDWARE_SIMULATED

	return (uint32_t) clock();

#else

#if DRM_MOBILE_WINDOWS_PLATFORM == 1
	return (uint32_t) GetTickCount();
#endif

#error "localClock: Put in platform's clock.  Clock must update at least 60 times a second."

#endif

#endif
}

uint32_t localClockTicksPerSec( void )
{
#if 1 

    return (uint32_t)x_os_get_sys_tick_period();

#else

#ifdef DRM_HARDWARE_SIMULATED

	return (uint32_t) CLOCKS_PER_SEC;

#else

#if DRM_MOBILE_WINDOWS_PLATFORM == 1
	return (uint32_t) CLOCKS_PER_SEC;
#endif

#error "localClockTicksPerSec: Put in platform's clock ticks per second." n

#endif

#endif
}

uint32_t localLoadDrmMemoryFragment1( uint8_t *frag,
                                      uint32_t fragLength )
{
#if DRM_OTHER_SECURE_PLATFORM == 0
    return s_readFragment( 1,
                            frag,
                            fragLength );
#else
    /* #error "localLoadDrmMemoryFragment1: Write all 80 bytes of memory out here.  Ignore the other fragment writers" */
    if (!divx_drm_mem_read_glue(frag))
    {
        return DRM_ERROR_READING_MEMORY;
    }    

    return DRM_LOCAL_SUCCESS;
#endif
}

#if DRM_OTHER_SECURE_PLATFORM == 0

uint32_t localLoadDrmMemoryFragment2( uint8_t *frag,
                                      uint32_t fragLength )
{
    return s_readFragment( 2,
                            frag,
                            fragLength );
}

uint32_t localLoadDrmMemoryFragment3( uint8_t *frag,
                                      uint32_t fragLength )
{
    return s_readFragment( 3,
                           frag,
                           fragLength );
}

#endif

uint32_t localSaveDrmMemoryFragment1( uint8_t *frag,
                                      uint32_t fragLength  )
{
#if DRM_OTHER_SECURE_PLATFORM == 0
    return s_writeFragment( 1,
                           frag,
                           fragLength );
#else
    /* #error "localSaveDrmMemoryFragment1: Load all 80 bytes of memory out here.  Ignore the other fragment writers" */
    if (!divx_drm_mem_write_glue(frag))
    {
        return DRM_ERROR_WRITING_MEMORY;
    }

    return DRM_LOCAL_SUCCESS;
#endif
}

#if DRM_OTHER_SECURE_PLATFORM == 0

uint32_t localSaveDrmMemoryFragment2( uint8_t *frag,
                                      uint32_t fragLength  )
{
    return s_writeFragment( 2,
                           frag,
                           fragLength );
}

uint32_t localSaveDrmMemoryFragment3( uint8_t *frag,
                                      uint32_t fragLength  )
{  
    return s_writeFragment( 3,
                           frag,
                           fragLength );
}

#endif

/*****************************************************************************************************************
 *****************************************************************************************************************
 *
 *   WINDOWS MOBILE DEVICES
 *
 *****************************************************************************************************************
 *****************************************************************************************************************/
#if DRM_MOBILE_WINDOWS_PLATFORM == 1

static int32_t s_getWindowsGeneralInfo( LINEGENERALINFO** lineGeneralInfo )
{
    #define TAPI_API_HIGH_VERSION   0x00020000
    #define EXT_API_LOW_VERSION     0x00010000
    #define EXT_API_HIGH_VERSION    0x00010000

    //line general info variables for obtaining the ESN number


    //ex param variables to initialize
    HLINEAPP hLineApp = 0;
    HLINE hLine = 0;
    DWORD dwNumDevs;
    DWORD dwAPIVersion = TAPI_API_HIGH_VERSION;
    DWORD dwExtVersion = 0;
    DWORD dwDeviceID;
    DWORD dwMediaMode = LINEMEDIAMODE_DATAMODEM | LINEMEDIAMODE_INTERACTIVEVOICE;
    LONG tapiresult;
    LINEINITIALIZEEXPARAMS lineInitializeExParams; 

    //initialize
    lineInitializeExParams.dwTotalSize = sizeof(lineInitializeExParams);
    
    //The application desires to use the Event Handle event notification mechanism
    lineInitializeExParams.dwOptions = LINEINITIALIZEEXOPTION_USEEVENT; 
    tapiresult = lineInitializeEx( &hLineApp,
                                   0,
                                   0,
                                   L"MyApp",
                                   &dwNumDevs,
                                   &dwAPIVersion,
                                   &lineInitializeExParams );
  

    *lineGeneralInfo = (LINEGENERALINFO *)malloc(1024);
    (*lineGeneralInfo)->dwTotalSize = 1024;

    for ( dwDeviceID = 0; dwDeviceID < dwNumDevs; dwDeviceID++ )
    {
        tapiresult = lineNegotiateExtVersion( hLineApp,
                                              dwDeviceID,
                                              dwAPIVersion,
                                              EXT_API_LOW_VERSION,
                                              EXT_API_HIGH_VERSION,
                                              &dwExtVersion );
        if ( tapiresult == 0 )
        {
            tapiresult = lineOpen( hLineApp,
                                   dwDeviceID,
                                   &hLine,
                                   dwAPIVersion,
                                   0,
                                   0,
                                   LINECALLPRIVILEGE_OWNER,
                                   dwMediaMode,
                                   0 );
            if ( tapiresult == 0 )
            {
                tapiresult = lineGetGeneralInfo( hLine,
                                                 *lineGeneralInfo );
                if ( tapiresult == 0 )
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

uint32_t localGetSerial( uint8_t* serial,
                         uint32_t* serialLength )
{
#ifdef DRM_HARDWARE_SIMULATED
    memset( serial,
            0xA7,
            4 );
    *serialLength = 4; /* Simulate an ESN, 4 bytes long */
    return 0;
#else
    LINEGENERALINFO* lineGeneralInfo;
    if ( s_getWindowsGeneralInfo( &lineGeneralInfo ) == 0 )
    {
        memcpy( serial,
                (char*)lineGeneralInfo + lineGeneralInfo->dwSerialNumberOffset,
                lineGeneralInfo->dwSerialNumberSize );
        *serialLength = lineGeneralInfo->dwSerialNumberSize;
        free( lineGeneralInfo );
        return 0;
    }
    return 1;
#endif
}

/**
*/
uint32_t localGetRevision( uint8_t* revision,
                           uint32_t* revisionLength )
{
#ifdef DRM_HARDWARE_SIMULATED
    memset( revision,
            0xA2,
            10 );
    *revisionLength = 10;
    return 0;
#else
   LINEGENERALINFO* lineGeneralInfo;
    if ( s_getWindowsGeneralInfo( &lineGeneralInfo ) == 0 )
    {
        memcpy( revision,
                (char*)lineGeneralInfo  + lineGeneralInfo->dwRevisionOffset,
                lineGeneralInfo->dwRevisionSize );
        free( lineGeneralInfo );
        *revisionLength = lineGeneralInfo->dwRevisionSize;
        return 0;
    }
    return 1;
#endif
}

/**
*/
uint32_t localGetManufacturer( uint8_t* manufacturer,
                               uint32_t* manufacturerLength )
{
#ifdef DRM_HARDWARE_SIMULATED
    strcpy( manufacturer,
            "DivXMobileSystems" );
    *manufacturerLength = strlen("DivXMobileSystems");
    return 0;
#else
    LINEGENERALINFO* lineGeneralInfo;
    if ( s_getWindowsGeneralInfo( &lineGeneralInfo ) == 0 )
    {
        memcpy( manufacturer,
                (char*)lineGeneralInfo  + lineGeneralInfo->dwManufacturerOffset,
                lineGeneralInfo->dwManufacturerSize );
        free( lineGeneralInfo );
        *manufacturerLength = lineGeneralInfo->dwManufacturerSize;
        return 0;
    }
    return 1;
#endif
}

#endif

/*****************************************************************************************************************
 *****************************************************************************************************************
 *
 *   SYMBIAN MOBILE DEVICES
 *
 *****************************************************************************************************************
 *****************************************************************************************************************/
#if DRM_MOBILE_SYMBIAN_PLATFORM == 1

/**
*/
uint32_t localGetIMEI( uint8_t* imei,
                       uint32_t* imeiLength )
{
#ifdef DRM_HARDWARE_SIMULATED
    memset( imei,
            0xCC,
            16 );
    *imeiLength = 16;
    return 0;
#else

#error "localGetIMEI: Return the machine IMEI"

#endif
}

/**
*/
uint32_t localGetMachineId( uint8_t* machineId,
                            uint32_t* machineIdLength )
{
#ifdef DRM_HARDWARE_SIMULATED
    memset( machineId,
            0xA1,
            15 );
    *machineIdLength = 15;
    return 0;
#else

#error "localGetMachineId: Return the machine ID"

#endif
}

#endif

/*****************************************************************************************************************
 *****************************************************************************************************************
 *
 *   DESKTOP WINDOWS and DESKTOP LINUX DEVICES
 *
 *****************************************************************************************************************
 *****************************************************************************************************************/
#if DRM_DESKTOP_WINDOWS_PLATFORM == 1 || DRM_DESKTOP_LINUX_PLATFORM == 1

uint32_t localGetMacAddress( uint8_t* mac,
                             uint32_t* macLength )
{
#ifdef DRM_HARDWARE_SIMULATED
    strcpy( mac,
            "00:00:00:00:00:00" );
    *macLength = strlen("00:00:00:00:00:00");
    return 0;
#else

#error "localGetMacAddress: Return the mac address"

#endif
}

uint32_t localGetProcessorString( uint8_t* processorString,
                                  uint32_t* processorStringLength )
{
#ifdef DRM_HARDWARE_SIMULATED
    strcpy( processorString,
            "Intel Celeron" );
    *processorStringLength = strlen("Intel Celeron");
    return 0;
#else

#error "localGetProcessorString: Return the processor string."

#endif
}

uint32_t localGetBootDriveName( uint8_t* bootDriveName,
                                uint32_t* bootDriveNameLength )
{
#ifdef DRM_HARDWARE_SIMULATED
    strcpy( bootDriveName,
            "WD Caviar 7200" );
    *bootDriveNameLength = strlen("WD Caviar 7200");
    return 0;
#else

#error "localGetBootDriveName: Return the boot drive name"

#endif
}
#endif

