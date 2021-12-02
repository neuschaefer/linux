/**
 * Copyright 2002-3 Cabot Communications.  All rights reserved.
 *
 * @file     ice_rs232_control_interface_types.h
 * @brief  ICE RS232 control interface
 *
 * This file defines the ICE RS232 control interface.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 */
#ifndef CABOT_ICE_ICE_RS232_CONTROL_INTERFACE_TYPES_H_INCLUDED
#define CABOT_ICE_ICE_RS232_CONTROL_INTERFACE_TYPES_H_INCLUDED

#include "frost/frost_basictypes.h"

#define ICE_RS232_MESSAGE_SIZE      128 //command + CR + LF + Command prompt
#define ICE_RS232_COMMAND_PROMPT    '>'

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    ICE_RS232_baudrate_2400,
    ICE_RS232_baudrate_4800,
    ICE_RS232_baudrate_9600,    
    ICE_RS232_baudrate_19200,    
    ICE_RS232_baudrate_28800,    
    ICE_RS232_baudrate_38400,    
    ICE_RS232_baudrate_57600,    
    ICE_RS232_baudrate_76800,    
    ICE_RS232_baudrate_115200    
} ICE_RS232_baudrate;

typedef enum
{
    ICE_RS232_databits_8,
    ICE_RS232_databits_9
} ICE_RS232_databits;

typedef enum
{
    ICE_RS232_parity_none,
    ICE_RS232_parity_even,
    ICE_RS232_parity_odd
} ICE_RS232_parity;

typedef enum
{
    ICE_RS232_stopbits_1,
    ICE_RS232_stopbits_2
} ICE_RS232_stopbits;

typedef struct
{
    ICE_RS232_baudrate baudrate;
    ICE_RS232_databits data_bits;
    ICE_RS232_parity parity;
    ICE_RS232_stopbits stop_bits;
} ICE_RS232CtrlIFOpenParams;


/**
 * ICE representation of a handle to RS232 control interface component
 *
 * ICE_RS232ControlInterfaceHandle is an opaque type, to be manipulated only by the
 * functions specified in this header file. 
 */
typedef struct ICE_RS232CtrlIFHandle ICE_RS232CtrlIFHandle;

/**
 * Callback function to inform Eclipse of a received RS232 command
 *
 * This function will not call back into any ICEISB_ functions.
 *
 * This function may block (e.g. on mutexes and message queues).  Do not call
 * this in real-time code, use a separate thread.
 *
 * The ICE implementation must not call this function directly from within
 * any ICEISB_ functions - it must be called from a separate thread.
 *
 * @param command            pointer to character array representing received command
 * @param size                     the length of the command in bytes
 * @param context               The context parameter passed to ICE_RS232ControlInterfaceRegisterCallback() function
 */
typedef void (*ICE_RS232ControlInterfaceCallbackFunction)(frost_uint8* command, frost_uint8 cmd_size, void* context);

#ifdef __cplusplus
}
#endif

#endif /*CABOT_ICE_ICE_RS232_CONTROL_INTERFACE_TYPES_H_INCLUDED*/


