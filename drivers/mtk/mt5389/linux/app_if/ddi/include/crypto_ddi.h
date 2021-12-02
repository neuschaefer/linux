
/******************************************************************************
 *   DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *   Copyright(c) 1999 by LG Electronics Inc.
 *
 *   All rights reserved. No part of this work may be reproduced, stored in a
 *   retrieval system, or transmitted by any means without prior written
 *   permission of LG Electronics Inc.
 *****************************************************************************/


/** @file crypto_ddi.h
 *
 * This source file defines the DDI functions related to crypto driver control
 *
 *  @author	inmoon kim (inmoon.kim@lge.com) / changsik yun (changsik.yun@lge.com)
 *  @version	0.1
 *  @date		2010.08.09
 *  @note		Additional information.
 *  @see		crypto_ddi.h
 */

/******************************************************************************
 	Header File Guarder
******************************************************************************/
#ifndef	_CRYPTO_DDI_H_
#define	_CRYPTO_DDI_H_

/******************************************************************************
 #include 파일들 (File Inclusions)
******************************************************************************/
#ifndef WIN32_SIM
#include "common.h"
#else
#include "common_win32.h"
#endif

/******************************************************************************
    매크로 함수 정의 (Macro Definitions)
******************************************************************************/

/******************************************************************************
   로컬 상수 정의 (Local Constant Definitions)
******************************************************************************/

/******************************************************************************
    로컬 형 정의 (Local Type Definitions)
******************************************************************************/

/******************************************************************************
    전역 형 정의 (Global Type Definitions)
******************************************************************************/

/******************************************************************************
    Extern 전역변수와 함수 prototype 선언
    (Extern Variables & Function Prototype Declarations)
******************************************************************************/

/******************************************************************************
    Static 변수와 함수 prototype 선언
    (Static Variables & Function Prototypes Declarations)
******************************************************************************/

/******************************************************************************
    로컬 변수와 함수 prototype 선언
    (Local Variables & Function Prototypes Declarations)
******************************************************************************/

/******************************************************************************
    전역 변수와 함수 prototype 선언
    (Global Variables & Function Prototypes Declarations)
******************************************************************************/

/******************************************************************************
	함수 정의 (Function Definitions)
******************************************************************************/

/* Netflix */
DTV_STATUS_T DDI_CRYPTO_NF_Initialize(char *pIDFilePath);
DTV_STATUS_T DDI_CRYPTO_NF_Finalize(void);
DTV_STATUS_T DDI_CRYPTO_NF_GetESN(UINT8 *pEsn);
DTV_STATUS_T DDI_CRYPTO_NF_GetKpe(UINT8 *pKpe);
DTV_STATUS_T DDI_CRYPTO_NF_GetKph(UINT8 *pKph);
DTV_STATUS_T DDI_CRYPTO_NF_Encrypt(UINT8 *pData, UINT32 nLength);
DTV_STATUS_T DDI_CRYPTO_NF_Decrypt(UINT8 *pData, UINT32 nLength);

/* General */
DTV_STATUS_T DDI_CRYPTO_OpenKeySlot(void);
DTV_STATUS_T DDI_CRYPTO_CloseKeySlot(void);
DTV_STATUS_T DDI_CRYPTO_GetHWRandomNumber(UINT8 *pData, UINT32 nLength);
DTV_STATUS_T DDI_CRYPTO_Encrypt(UINT8 *pData, UINT32 nLength);
DTV_STATUS_T DDI_CRYPTO_Decrypt(UINT8 *pData, UINT32 nLength);

/* Secure Firmware Update */
DTV_STATUS_T DDI_CRYPTO_SFU_Initialize(char *pSeedPath);
DTV_STATUS_T DDI_CRYPTO_SFU_Finalize(void);
DTV_STATUS_T DDI_CRYPTO_SFU_GetRSAKey(UINT8 *pRsaKey);
DTV_STATUS_T DDI_CRYPTO_SFU_GetAESKey(UINT8 *pAesKey);

/* Vudu */
/******************************************************************************
Function Name
    DDI_CRYPTO_VUDU_OpenM2mKeySlot

Function Description
    Allocate Memory to Memory Keyslot Handle

Function Parameters
    None

Return Value
    If the function succeeds, the return value is OK.
    If the function fails, the return value is NOT_OK.

Remarks
    None
******************************************************************************/
DTV_STATUS_T DDI_CRYPTO_VUDU_OpenM2mKeySlot(void);

/******************************************************************************
Function Name
    DDI_CRYPTO_VUDU_CloseM2mKeySlot

Function Description
    Free Memory to Memory Keyslot Handle

Function Parameters
    None

Return Value
    If the function succeeds, the return value is OK.

Remarks
    None
******************************************************************************/
DTV_STATUS_T DDI_CRYPTO_VUDU_CloseM2mKeySlot(void);

/******************************************************************************
Function Name
    DDI_CRYPTO_VUDU_SelectM2mKeyAlgorithm

Function Description
    Select Encryption/Decryption Algorithm

Function Parameters
    alg	[IN] 		Select Algorithm
    dir	[IN]		Select Mode

Return Value
    If the function succeeds, the return value is OK.
    If the function fails, the return value is NOT_OK.

Remarks
    None
******************************************************************************/
DTV_STATUS_T DDI_CRYPTO_VUDU_SelectM2mKeyAlgorithm(UINT8 alg, UINT8 dir);

/******************************************************************************
Function Name
    DDI_CRYPTO_VUDU_LoadClearM2mKey

Function Description
    Loads a Clear Key and a Clear IV to KeySlot.

Function Parameters
    keyLen	[IN] 		Key Length
    pKeyData	[IN]		Key Data
    ivLen		[IN]		Initial Vector Length
    pIvData	[IN]		Initial Vector Data

Return Value
    If the function succeeds, the return value is OK.
    If the function fails, the return value is NOT_OK.

Remarks
    None
******************************************************************************/
DTV_STATUS_T DDI_CRYPTO_VUDU_LoadClearM2mKey(const UINT32 keyLen, const UINT8 *pKeyData, const UINT32 ivLen, const UINT8 *pIvData);

/******************************************************************************
Function Name
    DDI_CRYPTO_VUDU_LoadWrappedM2mKey

Function Description
    Loads a Wrapped Key and a Wrapped IV to KeySlot

Function Parameters
    keyLen	[IN] 		Key Length of pKey4Data and pIvData
    pKey3Data	[IN]		Seed of Session Key
    pKey4Data	[IN]		Seed of Control Word
    pIvData	[IN]		Initial Vector Data

Return Value
    If the function succeeds, the return value is OK.
    If the function fails, the return value is NOT_OK.

Remarks
    None
******************************************************************************/
DTV_STATUS_T DDI_CRYPTO_VUDU_LoadWrappedM2mKey(const UINT32 keyLen, const UINT8 *pKey3Data, const UINT8 *pKey4Data, const UINT8 *pIvData);

/******************************************************************************
Function Name
    DDI_CRYPTO_VUDU_LoadWrappedM2mKeyClearIv

Function Description
    Loads a Wrapped Key and a Clear IV to KeySlot

Function Parameters
    keyLen	[IN] 		Key Length of pKey4Data and pIvData
    pKey3Data	[IN]		Seed of Session Key
    pKey4Data	[IN]		Seed of Control Word
    ivLen		[IN]		Initial Vector Length
    pIvData	[IN]		Initial Vector Data

Return Value
    If the function succeeds, the return value is OK.
    If the function fails, the return value is NOT_OK.

Remarks
    None
******************************************************************************/
DTV_STATUS_T DDI_CRYPTO_VUDU_LoadWrappedM2mKeyClearIv(const UINT32 keyLen, 	const UINT8 *pKey3Data, const UINT8 *pKey4Data, const UINT32 ivLen, const UINT8 *pIvData);

/******************************************************************************
Function Name
    DDI_CRYPTO_VUDU_RunM2mKey

Function Description
    Start the Cipher Process

Function Parameters
    length		[IN] 		Length of Both Src and Dst
    pSrc		[IN]		Source Pointer
    pDst		[OUT]	Destination Pointer

Return Value
    If the function succeeds, the return value is OK.
    If the function fails, the return value is NOT_OK.

Remarks
    None
******************************************************************************/
DTV_STATUS_T DDI_CRYPTO_VUDU_RunM2mKey(const UINT32 length, const UINT8 *pSrc, UINT8 *pDst);


/* Play API */
/******************************************************************************
Function Name
    DDI_CRYPTO_VUDU_OpenCAKeySlotHandle

Function Description
    Allocate VUDU CA Descramble Handle

Function Parameters
    None

Return Value
    If the function succeeds, the return value is handle value.
    If the function fails, the return value is 0.

Remarks
    None
******************************************************************************/
DTV_STATUS_T DDI_CRYPTO_VUDU_OpenCAKeySlotHandle(void);


/******************************************************************************
Function Name
    DDI_CRYPTO_VUDU_CloseCAKeySlotHandle

Function Description
    Free VUDU CA Descramble Handle

Function Parameters
    None

Return Value
    If the function succeeds, the return value is 0.

Remarks
    None
******************************************************************************/
DTV_STATUS_T DDI_CRYPTO_VUDU_CloseCAKeySlotHandle(void);


/******************************************************************************
Function Name
    DDI_CRYPTO_VUDU_GetCAKeySlotHandle

Function Description
    Allocate VUDU CA Descramble Handle

Function Parameters
    None

Return Value
    If the function succeeds, the return value is OK.
    If the function fails, the return value is NOT_OK.

Remarks
    None
******************************************************************************/
DTV_STATUS_T DDI_CRYPTO_VUDU_GetCAKeySlotHandle(void);

/******************************************************************************
Function Name
    DDI_CRYPTO_VUDU_SetWrappedCAKey

Function Description
    Load Wrapping Key to CA Descrambler

Function Parameters
    pKey			[IN]		CA Initial Vector
    pUniqueKey	[IN]		CA Wrapping Key
    keyType		[IN]		CA Key Type

Return Value
    If the function succeeds, the return value is OK.
    If the function fails, the return value is NOT_OK.

Remarks
    None
******************************************************************************/
DTV_STATUS_T DDI_CRYPTO_VUDU_SetWrappedCAKey(const UINT8 *pKey, const UINT8 *pUniqueKey, UINT8 keyType);


/******************************************************************************
Function Name
    DDI_CRYPTO_VUDU_SetWrappedAllCAKey

Function Description
    Load Wrapping Keys to CA Descrambler

Function Parameters
    pOddKey		[IN]		CA Initial Odd Vector
    pEvenKey		[IN]		CA Initial Even Vector
    pUniqueKey	[IN]		CA Wrapping Key

Return Value
    If the function succeeds, the return value is OK.
    If the function fails, the return value is NOT_OK.

Remarks
    None
******************************************************************************/
DTV_STATUS_T DDI_CRYPTO_VUDU_SetWrappedAllCAKey(const UINT8 *pOddKey, const UINT8 *pEvenKey, const UINT8 *pUniqueKey);


/******************************************************************************
Function Name
    DDI_CRYPTO_VUDU_LoadCAClearKey

Function Description
    Load Clear Key to CA Descrambler

Function Parameters
    pKey			[IN]		CA Initial Vector
    keyType		[IN]		CA Key Type

Return Value
    If the function succeeds, the return value is OK.
    If the function fails, the return value is NOT_OK.

Remarks
    None
******************************************************************************/
DTV_STATUS_T DDI_CRYPTO_VUDU_LoadCAClearKey(const UINT8 *pKey, UINT8 keyType);


/******************************************************************************
Function Name
    DDI_CRYPTO_VUDU_StartTSDecrypt

Function Description
    Start the TS Decrypt

Function Parameters
    srcLength	[IN] 		Length of Src
    pSrc		[IN]		Source Pointer (Encrypeted VUDU TS stream)
    pDstLength	[OUT]	Length of Dst
    pDst		[OUT]	Destination Pointer (the Result should be clear TS)

Return Value
    If the function succeeds, the return value is OK.
    If the function fails, the return value is NOT_OK.

Remarks
    None
******************************************************************************/
DTV_STATUS_T DDI_CRYPTO_VUDU_StartTSDecrypt(const UINT32 srcLength, const UINT8 *pSrc, UINT32 *pDstLength, UINT8 *pDst);


#endif

