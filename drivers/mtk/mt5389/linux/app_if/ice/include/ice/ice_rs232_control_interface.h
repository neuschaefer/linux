/**
 * Copyright 2002-3 Cabot Communications.  All rights reserved.
 *
 * @file     ice_rs232_control_interface.h
 * @brief  ICE RS232 control interface
 *
 * This file defines the ICE RS232 control interface.
 *
 * All functions in this interface are implemented in STB-specific code
 * and called by Eclipse code.
 */
#ifndef CABOT_ICE_ICE_RS232_CONTROL_INTERFACE_H_INCLUDED
#define CABOT_ICE_ICE_RS232_CONTROL_INTERFACE_H_INCLUDED

#include "frost/frost_basictypes.h"
#include "ice/ice_rs232_control_interface_types.h"

#ifdef __cplusplus
extern "C" {
#endif

frost_bool 
ICE_RS232ControlInterfaceInit(void);

ICE_RS232CtrlIFHandle* 
ICE_RS232ControlInterfaceOpen(ICE_RS232CtrlIFOpenParams open_params);

frost_bool
ICE_RS232ControlInterfaceClose(ICE_RS232CtrlIFHandle* handle);

frost_bool
ICE_RS232ControlInterfaceSendCommand(const frost_uint8* command, frost_uint32 size);

frost_bool
ICE_RS232ControlInterfaceIsOpened(ICE_RS232CtrlIFHandle* handle);

void ICE_RS232ControlInterfaceRegisterCallback
    ( ICE_RS232CtrlIFHandle* handle, ICE_RS232ControlInterfaceCallbackFunction callback_function, void* user_data );

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_RS232_CONTROL_INTERFACE_H_INCLUDED*/

