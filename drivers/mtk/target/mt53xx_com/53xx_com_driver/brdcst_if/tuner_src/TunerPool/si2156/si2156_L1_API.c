/*************************************************************************************
                  Silicon Laboratories Broadcast Si2156 Layer 1 API
   API functions definitions used by commands and properties
   FILE: Si2156_L1_API.c
   Supported IC : Si2156
   Compiled for ROM 12 firmware 1_E_build_6
   Revision: 0.1
   Tag:  001
   Date: May 31 2011
  (C) Copyright 2011, Silicon Laboratories, Inc. All rights reserved.
**************************************************************************************/
#define   Si2156_COMMAND_PROTOTYPES

#include "si2156_L1_API.h"

/***********************************************************************************************************************
  Si2156_L1_API_Init function
  Use:        software initialisation function
              Used to initialize the software context
  Returns:    0 if no error
  Comments:   It should be called first and once only when starting the application
  Parameter:   **ppapi         a pointer to the api context to initialize
  Parameter:  add            the Si2156 I2C address
  Porting:    Allocation errors need to be properly managed.
  Porting:    I2C initialization needs to be adapted to use the available I2C functions
 ***********************************************************************************************************************/
unsigned char    Si2156_L1_API_Init      (L1_Si2156_Context *api, int add) {

    api->i2c = &(api->i2cObj);

    L0_Init(api->i2c);
    L0_SetAddress(api->i2c, add, 0);

    api->cmd    = &(api->cmdObj);
    api->rsp    = &(api->rspObj);
    api->prop   = &(api->propObj);
    api->status = &(api->statusObj);

    return NO_Si2156_ERROR;
}
/***********************************************************************************************************************
  Si2156_L1_API_Patch function
  Use:        Patch information function
              Used to send a number of bytes to the Si2156. Useful to download the firmware.
  Returns:    0 if no error
  Parameter:  error_code the error code.
  Porting:    Useful for application development for debug purposes.
  Porting:    May not be required for the final application, can be removed if not used.
 ***********************************************************************************************************************/
unsigned char    Si2156_L1_API_Patch     (L1_Si2156_Context *api, int iNbBytes, unsigned char *pucDataBuffer) {
    unsigned char res;
    unsigned char rspByteBuffer[1];

    SiTRACE("Si2156 Patch %d bytes\n",iNbBytes);

    res = L0_WriteCommandBytes(api->i2c, iNbBytes, pucDataBuffer);
    if (res!=iNbBytes) {
      SiTRACE("Si2156_L1_API_Patch error 0x%02x writing bytes: %s\n", res, Si2156_L1_API_ERROR_TEXT(res) );
      return res;
    }

    res = Si2156_pollForResponse(api, 1, rspByteBuffer);
    if (res != NO_Si2156_ERROR) {
      SiTRACE("Si2156_L1_API_Patch error 0x%02x polling response: %s\n", res, Si2156_L1_API_ERROR_TEXT(res) );
      return ERROR_Si2156_POLLING_RESPONSE;
    }

    return NO_Si2156_ERROR;
}
/***********************************************************************************************************************
  Si2156_L1_CheckStatus function
  Use:        Status information function
              Used to retrieve the status byte
  Returns:    0 if no error
  Parameter:  error_code the error code.
 ***********************************************************************************************************************/
unsigned char    Si2156_L1_CheckStatus   (L1_Si2156_Context *api) {
    unsigned char rspByteBuffer[1];
    return Si2156_pollForResponse(api, 1, rspByteBuffer);
}
/***********************************************************************************************************************
  Si2156_L1_API_ERROR_TEXT function
  Use:        Error information function
              Used to retrieve a text based on an error code
  Returns:    the error text
  Parameter:  error_code the error code.
  Porting:    Useful for application development for debug purposes.
  Porting:    May not be required for the final application, can be removed if not used.
 ***********************************************************************************************************************/
char*            Si2156_L1_API_ERROR_TEXT(unsigned char error_code) {
    switch (error_code) {
        case NO_Si2156_ERROR                     : return "No Si2156 error";
        case ERROR_Si2156_ALLOCATING_CONTEXT     : return "Error while allocating Si2156 context";
        case ERROR_Si2156_PARAMETER_OUT_OF_RANGE : return "Si2156 parameter(s) out of range";
        case ERROR_Si2156_SENDING_COMMAND        : return "Error while sending Si2156 command";
        case ERROR_Si2156_CTS_TIMEOUT            : return "Si2156 CTS timeout";
        case ERROR_Si2156_ERR                    : return "Si2156 Error (status 'err' bit 1)";
        case ERROR_Si2156_POLLING_CTS            : return "Si2156_Error while polling CTS";
        case ERROR_Si2156_POLLING_RESPONSE       : return "Si2156 Error while polling response";
        case ERROR_Si2156_LOADING_FIRMWARE       : return "Si2156 Error while loading firmware";
        case ERROR_Si2156_LOADING_BOOTBLOCK      : return "Si2156 Error while loading bootblock";
        case ERROR_Si2156_STARTING_FIRMWARE      : return "Si2156 Error while starting firmware";
        case ERROR_Si2156_SW_RESET               : return "Si2156 Error during software reset";
        case ERROR_Si2156_INCOMPATIBLE_PART      : return "Si2156 Error Incompatible part";
        default : return "Unknown Si2156 error code";
    }
}






