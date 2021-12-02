/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/

/** @file 		ci_ddi.h
 *
 *  header of Common Interface DDI Fuction.
 *
 *
 *  @author		hwajeong.lee
 *  @author
 *  @version	1.0
 *  @date		2007.5.1
 *  @date
 *  @note		Additional information.
 *  @see
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef _CI_DDI_H_
#define _CI_DDI_H_

/******************************************************************************
    전역 제어 상수(Control Constants)
******************************************************************************/

/******************************************************************************
	#include 파일들 (File Inclusions)
******************************************************************************/
#include <osa_api.h>
#include <common.h>

/******************************************************************************
 	상수 정의(Constant Definitions)
******************************************************************************/

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/


/******************************************************************************
	형 정의 (Type Definitions)
******************************************************************************/

#ifdef INCLUDE_CIPLUS
/**
* CIPLUS_DATARATE_T
*
* @see
*/
typedef enum
{
    CIPLUS_DATARATE_72  = 0,
    CIPLUS_DATARATE_96,
} CIPLUS_DATARATE_T;


/**
* CIPLUS_CRYPTOGRAPHY_T
*
* @see
*/
typedef enum
{
    CIPLUS_CRYPTOGRAPHY_DES                 = 0,
    CIPLUS_CRYPTOGRAPHY_AES                 = 1
} CIPLUS_CRYPTOGRAPHY_T;

/**
* CIPLUS_CHIPHER_KEY
*
* @see
*/
typedef enum
{
    CIPLUS_CIPHER_KEY_EVEN                  = 0,
    CIPLUS_CIPHER_KEY_ODD                   = 1
} CIPLUS_CIPHER_KEY_T;

#endif



/******************************************************************************
	함수 선언 (Function Declaration)
******************************************************************************/
DTV_STATUS_T DDI_CI_Init(SINT32 *pDeviceHandle);
DTV_STATUS_T DDI_CI_Close(SINT32 *pDeviceHandle);
UINT8 DDI_CI_DetectCard(SINT32 deviceHandle);
DTV_STATUS_T DDI_CI_Reset(SINT32 deviceHandle);

DTV_STATUS_T DDI_CI_CheckCIS(SINT32 deviceHandle);
DTV_STATUS_T DDI_CI_WriteCOR(SINT32 deviceHandle);
DTV_STATUS_T DDI_CI_Read(SINT32 deviceHandle, UINT8 *pData, UINT16 *pSize);
DTV_STATUS_T DDI_CI_Write(SINT32 deviceHandle, UINT8 *pData, UINT16 size);
DTV_STATUS_T DDI_CI_NegoBuf(SINT32 deviceHandle, UINT16 *pBufSize);
SINT32 DDI_CI_ReadDAStatus(SINT32 deviceHandle);
void DDI_CI_SetCardPresent(BOOLEAN cardPresent);

void DDI_CI_PLUS_GetCiPlusStdKey( UINT8 *pPath, UINT8 *dkey );

DTV_STATUS_T DDI_CI_Restart( SINT32 deviceHandle );


//#if (PLATFORM_TYPE == BCM_PLATFORM)
UINT8 DDI_CI_CheckCIPLUSCapability( SINT32 deviceHandle );
//#else
//UINT8 DDI_CI_CheckCIPLUSCapability(SINT32 deviceHandle);
//DTV_STATUS_T DDI_CI_GetManufacturerInfo(SINT32 deviceHandle, UINT8 *pu8Manufacturer,
//                                                                                UINT8 u8ManufaturerSize, UINT8 *pu8Product, UINT8 u8ProductSize);
//#endif

#ifdef INCLUDE_CIPLUS
/******************************************************************************
Function Name
    DDI_CI_PLUS_ResetPCMCIA

Function Description
    Physicallly resets the CICAM(Common Interface Conditional Access Module) for
    CI Plus. During reset host must set the RS(Reset) bit.
    It must be used only for CI Plus.

Function Parameters
    deviceHandle    [IN] device handle for CICAM device

Return Value
    If the function succeeds, the return value is OK.
    If the function fails, the return value is NOT_OK.

Remarks
    None
******************************************************************************/
DTV_STATUS_T DDI_CI_PLUS_ResetPCMCIA(SINT32 deviceHandle);

/******************************************************************************
Function Name
    DDI_CI_PLUS_ReadIIRStatus

Function Description
    Reads IIR(Initialize Interface Request) bit of status register for CI Plus.
    It must be used only for CI Plus.
    The CICAM sets this bit to request a physical interface reset. After setting
    the IIR bit, the CICAM shall not send any other APDUs to the host. The CICAM
    clears the IIR bit when the host sets the RS bit during the reset.

Function Parameters
    deviceHandle    [IN] device handle of CI device to check IIR

Return Value
    It returns the IIR bit of status register. (0,1)

Remarks
    None
******************************************************************************/
SINT32 DDI_CI_PLUS_ReadIIRStatus(SINT32 deviceHandle);

/******************************************************************************
Function Name
    DDI_CI_PLUS_GetDataRate

Function Description
    Gets the data rate on PCMICA bus for CI Plus.
    It must be used only for CI Plus.

Function Parameters
    deviceHandle    [IN] device handle of CI device

Return Value
    It returns the data rate on PCMICA bus.

Remarks
    None
******************************************************************************/
CIPLUS_DATARATE_T DDI_CI_PLUS_GetDataRate(SINT32 deviceHandle);

/******************************************************************************
Function Name
    DDI_CI_PLUS_SetDataPath

Function Description
    Set the input/output datapath to TSP for CI Plus cryptography engine.

Function Parameters
    deviceHandle    [IN] device handle for CICAM device
    eInPath         [IN] input TSIF0_LIVE or TSIF0_FILE or TSIF1
    eOutPath        [IN] output PLAY_LIVE or PLAY_FILE or REC_DESCRMB
    bEnableOutputAV [IN] Enable this flag if data will output to AV fifo

Return Value
    If the function succeeds, the return value is OK.
    If the function fails, the return value is NOT_OK.

Remarks
    None
******************************************************************************/
//DTV_STATUS_T  DDI_CI_PLUS_SetDataPath(SINT32 deviceHandle,
//                                                CIPLUS_DATAPATH_T eInPath,
//                                                CIPLUS_DATAPATH_T eOutPath,
//                                                UINT16 bEnableOutputAV);

/******************************************************************************
Function Name
    DDI_CI_PLUS_SetCryptography

Function Description
    Set the cryptography to CI Plus cryptography engine.

Function Parameters
    deviceHandle    [IN] device handle for CICAM device
    eCryptography   [IN] Cryptography for protected stream: DES / AES

Return Value
    If the function succeeds, the return value is OK.
    If the function fails, the return value is NOT_OK.

Remarks
    None
******************************************************************************/
DTV_STATUS_T  DDI_CI_PLUS_SetCryptography(SINT32 deviceHandle,
                                                    CIPLUS_CRYPTOGRAPHY_T eCryptography);

/******************************************************************************
Function Name
    DDI_CI_PLUS_SetCipherKeys

Function Description
    Set the cipher keys to CI Plus cryptography engine.

Function Parameters
    deviceHandle    [IN] device handle for CICAM device
    eCryptography   [IN] Cryptography for protected stream: DES / AES
    eKey            [IN] Even or Odd chipher key of Cryptography
    UINT8FID           [IN] Pid filter number
    pUINT32CipherKeys  [IN] Content of Chipher key
                            AES: IVs 128bits + Keys 128bits,
                            DES: Keys 64bits.
                            E.g.
                                IVs[] = {0xa4,0xa3,0xa2,0xa1, 0xc4,0xc3,0xc2,0xc1, 0xb4,0xb3,0xb2,0xb1, 0xd4,0xd3,0xd2,0xd1};
                                Key[] = {0x08,0x07,0x06,0x05, 0x28,0x27,0x26,0x25, 0x18,0x17,0x16,0x15, 0x38,0x37,0x36,0x35};
                                pUINT32CipherKeys[8] = {0xA4A3A2A1, 0xC4C3C2C1, 0xB4B3B2B1, 0xD4D3D2D1,
                                                     0x08070605, 0x28272625, 0x18171615, 0x38373635};

Return Value
    If the function succeeds, the return value is OK.
    If the function fails, the return value is NOT_OK.

Remarks
    None
******************************************************************************/
DTV_STATUS_T  DDI_CI_PLUS_SetCipherKeys(SINT32 deviceHandle,
                                                  CIPLUS_CRYPTOGRAPHY_T eCryptography,
                                                  CIPLUS_CIPHER_KEY_T eKey,
                                                  UINT32* pUINT32CipherKeys);

#endif

#endif /*_CI_DDI_H_ */

