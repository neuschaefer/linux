/*************************************************************************************
                  Silicon Laboratories Broadcast Si2156 Layer 1 API
   API functions prototypes used by commands and properties
   FILE: Si2156_L1_API.h
   Supported IC : Si2156
   Compiled for ROM 12 firmware 1_E_build_6
   Revision: 0.1
   Date: May 31 2011
  (C) Copyright 2011, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#ifndef    _Si2156_L1_API_H_
#define    _Si2156_L1_API_H_

#include "si2156_L0_TV_Chassis.h"

#include "si2156_Commands.h"
#include "si2156_Properties.h"
#include "si2156_typedefs.h"
#include "si2156_Commands_Prototypes.h"

#include "PD_Def.h"

#define NO_Si2156_ERROR                     0x00
#define ERROR_Si2156_PARAMETER_OUT_OF_RANGE 0x01
#define ERROR_Si2156_ALLOCATING_CONTEXT     0x02
#define ERROR_Si2156_SENDING_COMMAND        0x03
#define ERROR_Si2156_CTS_TIMEOUT            0x04
#define ERROR_Si2156_ERR                    0x05
#define ERROR_Si2156_POLLING_CTS            0x06
#define ERROR_Si2156_POLLING_RESPONSE       0x07
#define ERROR_Si2156_LOADING_FIRMWARE       0x08
#define ERROR_Si2156_LOADING_BOOTBLOCK      0x09
#define ERROR_Si2156_STARTING_FIRMWARE      0x0a
#define ERROR_Si2156_SW_RESET               0x0b
#define ERROR_Si2156_INCOMPATIBLE_PART      0x0c

#define SiTRACE mcPRINTF


unsigned char Si2156_L1_API_Init    (L1_Si2156_Context *api, int add);
unsigned char Si2156_L1_API_Patch   (L1_Si2156_Context *api, int iNbBytes, unsigned char *pucDataBuffer);
unsigned char Si2156_L1_CheckStatus (L1_Si2156_Context *api);
char*         Si2156_L1_API_ERROR_TEXT(unsigned char  error_code);


#ifdef    Si2156_GET_PROPERTY_STRING
void Si2156_L1_FillPropertyStringText(L1_Si2156_Context *api, unsigned int prop, char *separator, char *msg);
#endif /* Si2156_GET_PROPERTY_STRING */

#endif /* _Si2156_L1_API_H_ */






