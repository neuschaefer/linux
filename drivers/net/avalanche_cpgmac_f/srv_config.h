/*
 *
 * srv_config.h
 * Description:
 * see below
 *
 *
 * Copyright (C) 2008, Texas Instruments, Incorporated
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 */

 
/** \file   srv_config.h
    \brief  Configuration Service Header file

    This file contains interfaces for configuration service.


    \author     PSP Architecture Team
    \version    1.0
 */ 
    
#ifndef _SRV_CONFIG_H_
#define _SRV_CONFIG_H_
    
#include "pal_defs.h"
    
/**
 * \defgroup SrvCfg Configuration Service
 * 
 *  Configuration service interfaces
 */ 
    /*@{ */ 
    
/**
 *  \brief Configuration ServiceInitialization
 *
 *  This function initializes the configuration service by using the 
 *  configuration parameters provided
 *
 *  \note The implementation may be platform dependent
 *
 *  \param  initConfigInfo  Config Information structure pointer
 *  \return PAL_SUCCESS or Configuration Service Error code
 */ 
    PAL_Result SRV_configInit(Ptr initConfigInfo);

/**
 *  \brief Add configuration information
 *
 *  This function is used to add/store/modify a key/value pair in the 
 *  configuration service.  This function is required when the system requires 
 *  a capability to add/change the configuration information in the 
 *  configuration repository or original source of the configuration information. 
 *  E.g. Store modified/updated configuration information in NVRAM or Flash.
 *
 *  \note The implementation may be platform dependent 
 *
 *  \param  instance        Instance of key/value pair being added/set
 *  \param  key             Pointer to the 'key' string
 *  \param  value           Pointer to the character array for 'value'
 *  \param  valueLength     Length of the 'value' array in bytes
 *  \param  param           Optional parameters if any, else NULL
 *  \return PAL_SUCCESS or Configuration Service Error code
 */ 
    PAL_Result SRV_configAdd(UInt instance, Char * key, Char * value,
                             UInt valueLength, Ptr param);

/**
 *  \brief Get configuration information
 *
 *  This function is used to retrieve a key/value pair in the configuration 
 *  service for a given instance of the "key".  The length of the "value" is 
 *  returned in the pointer provided by the caller.
 *
 *  \note The implementation may be platform dependent 
 *
 *  \param  instance        Instance of key/value pair being added/set
 *  \param  key             Pointer to the 'key' string
 *  \param  value           Storage for pointer to the character array for 'value'
 *  \param  valueLength     Pointer to the length of the 'value' array in bytes
 *  \return PAL_SUCCESS or Configuration Service Error code
 */ 
    PAL_Result SRV_configGet(UInt instance, Char * key, Char ** value,
                             UInt * valueLength);

/**
 *  \brief Parse and Get Integer Value
 *
 *  This function is used to parse and retrieve the integer value of a given 
 *  key string from the source string. 
 *
 *  \note To be used when the value component is a string
 *
 *  \param  srcString       Pointer to the source string (The integer value of a given 
 *                          "keyString" will be retrieved from this string)
 *  \param  keyString       Pointer to the 'key' string whose integer value is sought
 *  \param  value           Pointer to the returned unsigned integer value
 *  \return PAL_SUCCESS or Configuration Service Error code
 */ 
    PAL_Result SRV_configGetIntegerValue(Char * srcString,
                                         Char * keyString, Uint * value);

/**
 *  \brief Parse and Get String Value
 *
 *  This function is used to parse and retrieve the string value of a given 
 *  key string from the source string. 
 *
 *  \note To be used when the value component is a string
 *
 *  \param  srcString       Pointer to the source string (The integer value of a given 
 *                          "keyString" will be retrieved from this string)
 *  \param  keyString       Pointer to the 'key' string whose integer value is sought
 *  \param  value           Pointer to the returned string value
 *  \return PAL_SUCCESS or Configuration Service Error code
 */ 
    PAL_Result SRV_configGetStringValue(Char * srcString, Char * keyString,
                                        Char * value);

    /*@} */ 
    
#endif  /* _SRV_CONFIG_H_ */
    
