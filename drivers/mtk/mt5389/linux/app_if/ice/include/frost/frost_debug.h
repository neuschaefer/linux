/**
 * Copyright 2002-4 Cabot Communications.  All rights reserved.
 *
 * @file
 *
 * @brief Cabot's interface to platform specific debug output.
 *
 * This is not a mandatory part of the public FROST interface, but it
 * will need to be implemented if diagnostic messages are required.
 * It is also required if the FROST, ICE or CHIL Unit Tests are going
 * to be used.
 *
 * @note If an interactive console is not required (i.e. you only wish
 *       to see debug output and/or to run the unit tests in batch mode), 
 *       it is not necessary to implement FROST_DebugStartConsole() and
 *       FROST_RegisterCommand().
 */

#ifndef CABOT_FROST_FROST_DEBUG_H_INCLUDED
#define CABOT_FROST_FROST_DEBUG_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Debug output.
 *
 * The string passed in can be very long as long as it is null terminated
 * and does not contain additional nulls or special characters.
 *
 * @param message Pointer to the first character to be output.
 * @param length Length of the string to be output.  Special value zero
 *        indicates that the string is null-terminated and all characters
 *        up to the null should be output.  Other values indicate that the
 *        string may include null characters (or other non-printing
 *        characters) and the implementation should ensure that such 
 *        characters are rendered in some readable manner (eg character
 *        zero could be represented by "#00").
 *
 * @note This is called from multiple threads, so it must be implemented
 *       in a thread-safe manner.
 */
void FROST_DebugOut(const char *message, frost_uint length);

/**
 * @brief Cause a debugger breakpoint.
 */
void FROST_DebugBreak(void);

/**
 * @brief Check that a pointer has a valid address compared to the Set
 * Top memory map.
 *
 * @param ptr The pointer to check.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool FROST_DebugCheckPointer(void* ptr);


/**
 * @brief start the debug console (if any), for interaction with the user. 
 */
void FROST_DebugStartConsole(void);

/**
 * @brief A typedef for the function to call when a command is
 * registered with the console.
 */
typedef void (*FROST_DebugCommandCallbackFunction)(void* param);

/**
 * @brief A string and/or Integer parameter, passed to a debug console command.
 */
typedef struct FROST_DebugParamStringInt
{
    /** String part */        
    char* string_part;

    /** Integer part */
    frost_int int_part;

} FROST_DebugParamStringInt;

/**
 * @brief The types of arguments that can be taken by commands.
 */
typedef enum FROST_DebugParamType
{
    /** The command takes no parameters. */
    FROST_debug_param_none, 

    /** The command takes an integer parameter. */
    FROST_debug_param_int,

    /** The command takes a FROST_DebugParamStringInt. */
    FROST_debug_param_string_int,

    /** The command takes a string parameter. */
    FROST_debug_param_string,

    /** The command takes a FROST_DebugParamStringString*/
    FROST_debug_param_string_string
} FROST_DebugParamType;

/**
 * @brief A string and string parameter, passed to a debug console command.
 * @Note first string is the parameter to function, second is the return part.
 */
typedef struct FROST_DebugParamStringString
{
    /** Parameter part */
    char param_part[300];

    /** Return part */
    char return_part[80];

} FROST_DebugParamStringString;

/**
 * @brief register any user defined commands for unit testing with the
 * console (if any).
 *
 * @param cmd_name Name of the command that the user types at the console.
 * @param cmd_desc Description of what the command does.
 * @param callback The function to be called when the command is invoked.
 * @param param_type The type of command line parameters accepted by the 
 *        command.
 *
 * @return #frost_true on success, #frost_false on failure.
 */
frost_bool FROST_RegisterCommand(const char* cmd_name, const char* cmd_desc, 
                             FROST_DebugCommandCallbackFunction callback,
                             FROST_DebugParamType param_type);

/**
 * @brief register any user defined commands for unit testing with the
 * console (if any).
 *
 * @param cmd_name Name of the command
 */
void FROST_RunTestToolCommand(char* cmd);
#ifdef __cplusplus
}
#endif

#endif /* CABOT_FROST_FROST_DEBUG_H_INCLUDED*/

