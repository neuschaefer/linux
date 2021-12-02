/*----------------------------------------------------------------------------*
 * Copyright Statement:                                                       *
 *                                                                            *
 *   This software/firmware and related documentation ("MediaTek Software")   *
 * are protected under international and related jurisdictions'copyright laws *
 * as unpublished works. The information contained herein is confidential and *
 * proprietary to MediaTek Inc. Without the prior written permission of       *
 * MediaTek Inc., any reproduction, modification, use or disclosure of        *
 * MediaTek Software, and information contained herein, in whole or in part,  *
 * shall be strictly prohibited.                                              *
 * MediaTek Inc. Copyright (C) 2010. All rights reserved.                     *
 *                                                                            *
 *   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND     *
 * AGREES TO THE FOLLOWING:                                                   *
 *                                                                            *
 *   1)Any and all intellectual property rights (including without            *
 * limitation, patent, copyright, and trade secrets) in and to this           *
 * Software/firmware and related documentation ("MediaTek Software") shall    *
 * remain the exclusive property of MediaTek Inc. Any and all intellectual    *
 * property rights (including without limitation, patent, copyright, and      *
 * trade secrets) in and to any modifications and derivatives to MediaTek     *
 * Software, whoever made, shall also remain the exclusive property of        *
 * MediaTek Inc.  Nothing herein shall be construed as any transfer of any    *
 * title to any intellectual property right in MediaTek Software to Receiver. *
 *                                                                            *
 *   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its *
 * representatives is provided to Receiver on an "AS IS" basis only.          *
 * MediaTek Inc. expressly disclaims all warranties, expressed or implied,    *
 * including but not limited to any implied warranties of merchantability,    *
 * non-infringement and fitness for a particular purpose and any warranties   *
 * arising out of course of performance, course of dealing or usage of trade. *
 * MediaTek Inc. does not provide any warranty whatsoever with respect to the *
 * software of any third party which may be used by, incorporated in, or      *
 * supplied with the MediaTek Software, and Receiver agrees to look only to   *
 * such third parties for any warranty claim relating thereto.  Receiver      *
 * expressly acknowledges that it is Receiver's sole responsibility to obtain *
 * from any third party all proper licenses contained in or delivered with    *
 * MediaTek Software.  MediaTek is not responsible for any MediaTek Software  *
 * releases made to Receiver's specifications or to conform to a particular   *
 * standard or open forum.                                                    *
 *                                                                            *
 *   3)Receiver further acknowledge that Receiver may, either presently       *
 * and/or in the future, instruct MediaTek Inc. to assist it in the           *
 * development and the implementation, in accordance with Receiver's designs, *
 * of certain softwares relating to Receiver's product(s) (the "Services").   *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MediaTek Inc. with respect  *
 * to the Services provided, and the Services are provided on an "AS IS"      *
 * basis. Receiver further acknowledges that the Services may contain errors  *
 * that testing is important and it is solely responsible for fully testing   *
 * the Services and/or derivatives thereof before they are used, sublicensed  *
 * or distributed. Should there be any third party action brought against     *
 * MediaTek Inc. arising out of or relating to the Services, Receiver agree   *
 * to fully indemnify and hold MediaTek Inc. harmless.  If the parties        *
 * mutually agree to enter into or continue a business relationship or other  *
 * arrangement, the terms and conditions set forth herein shall remain        *
 * effective and, unless explicitly stated otherwise, shall prevail in the    *
 * event of a conflict in the terms in any agreements entered into between    *
 * the parties.                                                               *
 *                                                                            *
 *   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and    *
 * cumulative liability with respect to MediaTek Software released hereunder  *
 * will be, at MediaTek Inc.'s sole discretion, to replace or revise the      *
 * MediaTek Software at issue.                                                *
 *                                                                            *
 *   5)The transaction contemplated hereunder shall be construed in           *
 * accordance with the laws of Singapore, excluding its conflict of laws      *
 * principles.  Any disputes, controversies or claims arising thereof and     *
 * related thereto shall be settled via arbitration in Singapore, under the   *
 * then current rules of the International Chamber of Commerce (ICC).  The    *
 * arbitration shall be conducted in English. The awards of the arbitration   *
 * shall be final and binding upon both parties and shall be entered and      *
 * enforceable in any court of competent jurisdiction.                        *
 *---------------------------------------------------------------------------*/
/**
 * @file SdCP.h
 * @brief Copy Protection Module header file\n
 * @remarks Copy Protection Module의 header file이다.\n
 Copy Protection Module은 SdCP_Open, SdCP_Close, SdCP_Start, SdCP_Stop로 구성된다.\n
 이 네 함수들은 Open, Start, Stop, Close 순서로 호출되어야 하며 Start가 호출되었을 경우\n
 해당하는 Video, Audio가 Decording 된다.\n
 Copy Protection Module consists of SdCP_Open, SdCP_Close, SdCP_Start, SdCP_Stop. \n
 In these four functions are called up in the sequence of Open, Start, Stop and Close.\n
 When Start is called up, the concerned Video and Audio are decorded.\n
 * @author S/W Platform Lab
 * @date 2006/07/10
 * @code 
 	////////------ Sample Code ------////////
	// CP Module을 Open한다.  Open CP Module
 	SdCP_Open(SD_WINDOW_MAIN);				
 	SdCP_Param_t pInfo;
 	pInfo.eCipher = SD_CP_DESCRAMBLE;
	pInfo.eType = SD_CP_DES;
	pInfo.eMode = SD_CP_ECB;
	 
	pInfo.sFirstKey.ohcw = cpkey_high;
	pInfo.sFirstKey.olcw = cpkey_low;
	pInfo.sFirstKey.ehcw = cpkey_high;
	pInfo.sFirstKey.elcw = cpkey_low;	

	// Main Video에 대한 CP를 Start한다. CP Start for Main Video
	if(videoPID < 0x1fff && videoPID > 0)
	{
		pInfo.eAv = SD_CP_VIDEO;
		SdCP_Start(SD_WINDOW_MAIN, &pInfo); 
	}

	// Main Audio에 대한 CP를 Start한다. CP Start for Main Audio
	if(audioPID < 0x1fff && audioPID > 0)
	{
		pInfo.eAv = SD_CP_AUDIO;
		SdCP_Start(SD_WINDOW_MAIN, &pInfo); 
	}

	// 이 시점에서 Descramble된 영상을 본다. Show descrambled video
	
	// Main Video에 대한 CP를 Stop한다. Stop CP for Main Video
	pInfo.eAv = SD_CP_VIDEO;
	SdCP_Stop(SD_WINDOW_MAIN, &pInfo);		

	// Main Audio에 대한 CP를 Stop한다. Stop CP for Main Audio
	pInfo.eAv = SD_CP_AUDIO;
	SdCP_Stop(SD_WINDOW_MAIN, &pInfo);		

	// CP Module을 Close한다. Close CP Module
	SdCP_Close(SD_WINDOW_MAIN); 		
	
 * @endcode
 */

#ifndef	_SDCP_H_
#define	_SDCP_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "SdType.h"
#include "SdMisc.h"

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
	TYPE DEFINITION
********************************************************************/

typedef enum
{
	SD_CP_AUDIO,
	SD_CP_VIDEO 
} SdCP_AV_k;

typedef enum
{
	SD_CP_NONE,
	SD_CP_DES,
	SD_CP_3DES,
	SD_CP_AES,
	SD_CP_MULTI2,
} SdCP_Type_k;

typedef enum
{
	SD_CP_DEFAULT,
	SD_CP_ECB,
	SD_CP_CBC
} SdCP_Mode_k;

typedef enum
{
	SD_CP_SCRAMBLE,
	SD_CP_DESCRAMBLE
} SdCP_Cipher_k;

typedef struct
{
	SdCP_AV_k eAv;			///< Audio, Video
	SdCP_Cipher_k eCipher;	///< Scramble, Descramble
	SdCP_Type_k eType;		///< None, DES, 3DES
	SdCP_Mode_k eMode;		///< Default, ECB, CBC
	SdCPKey_t sFirstKey;		///< First Cipher Key
	SdCPKey_t sSecondKey;		///< Second Cipher Key
	SdCPKey_t sThirdKey;		///< Third Cipher Key
	int32_t pid; 
} SdCP_Param_t;

typedef struct
{
    SdCP_Type_k eType;          ///< None, DES, 3DES
    SdCP_Mode_k eMode;          ///< Default, ECB, CBC 

    uint8_t *pKey;              ///< [in]       AES-128 key
    uint32_t keyLen;            ///< [in]       Length of key (fixed:16 bytes)
    uint8_t *pBuf_plain;        ///< [in,out]   Buffer for data to be encrypted or Buffer for decrypted data
    uint32_t bufLen_plain;    	///< [in,out]   Buffer length (multiple of 16)
    uint8_t *pIv;               ///< [in]       16 bytes buffer for iv (Initial Vector), only CBC mode
    uint32_t ivLen;             ///< [in]       IV length (fixed:16 bytes), only CBC mode
    uint8_t *pBuf_cipher;       ///< [in,out]   Buffer for encrypted data or  Buffer for data to be decrypted
    uint32_t bufLen_cipher;   	///< [in,out]   Buffer length (multiple of 16)
} SdCP_ParamEncDec_t;



/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/
	
/**
 * @brief SdCP Module Open 
 * @remarks Copy Protection Module을 Demux Handle 값에 따라서 별도로 Open한다. SdCP_Close와 쌍으로 존재한다. 
 Copy Protection Module separately depending on the Demux Handle value.  It exists in pair with SdCP_Close. 
 * @param dmxHndl Demux Handle SdAVDec_DmxHandle을 받아와서 넣어줘야 한다. 
 * @code
	typedef enum
	{
		SD_WINDOW_MAIN,
		SD_WINDOW_SUB0,
		SD_STREAM_TSOUT,
		SD_STREAM_EXTOUT0,
		SD_STREAM_EXTOUT1,
		SD_WINDOW_MAX
	} SdWindowID_k;
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCP_Open(SdAVDec_DmxHandle dmxHndl);

/**
 * @brief SdCP Module Close
 * @remarks Copy Protection Module을 Demux Handle 값에 따라서 별도로 Close한다. SdCP_Open과 쌍으로 존재한다. 
 It closes the SdCP Module Close. Copy Protection Module separately depending on Demux Handle.   
 It exists in pair with value. SdCP_Open.
 * @param dmxHndl Demux Handle
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCP_Close(SdAVDec_DmxHandle dmxHndl);

/**
 * @brief Start SdCP Module
 It starts the Start SdCP Module. 
 * @remarks Copy Protection Module을 Demux Handle 값에 따라서 별도로 Start한다.  
 pInfo에는 decoding을 위한 모든 정보가 들어 있다. Start를 호출시 decoding이 시작된다. 
 SdCP_Stop과 쌍으로 존재한다. 
 Copy Protection Module separately depending on the Demux Handle. It exists in pair with value. SdCP_Stop.
 * @param dmxHndl Demux Handle
 * @param pInfo include CP information
 * @code 
 	typedef struct
	{
		SdCP_AV_k eAv;
		SdCP_Cipher_k eCipher;
		SdCP_Type_k eType;
		SdCP_Mode_k eMode;
		SdCPKey_t sFirstKey;	
		SdCPKey_t sSecondKey;		
		SdCPKey_t sThirdKey;		
	} SdCP_Param_t;

	typedef enum
	{
		SD_CP_AUDIO,
		SD_CP_VIDEO 
	} SdCP_AV_k;

	typedef enum
	{
		SD_CP_SCRAMBLE,
		SD_CP_DESCRAMBLE
	} SdCP_Cipher_k;

	typedef enum
	{
		SD_CP_NONE,
		SD_CP_DES,
		SD_CP_3DES
	} SdCP_Type_k;

	typedef enum
	{
		SD_CP_DEFAULT,
		SD_CP_ECB,
		SD_CP_CBC
	} SdCP_Mode_k;

	typedef struct
	{
		uint32_t ohcw;
		uint32_t olcw;
		uint32_t ehcw;
		uint32_t elcw;
	} SdCPKey_t;
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCP_Start(SdAVDec_DmxHandle dmxHndl, SdCP_Param_t* pInfo);

/**
 * @brief Stop SdCP Module
 * @remarks Copy Protection Module을 Demux Handle 값에 따라서 별도로 Stop한다. 
 pInfo에는 decoding을 위한 모든 정보가 들어 있다.  
 Stop를 호출시 decoding이 멈춘다.
 SdCP_Start와 쌍으로 존재한다.
 Start Copy Protection Module separately depending on the Demux Handle.
 pInfo has the required information for decoding.
 It stops the decoding.
 It exists in pair with SdCP_Start.
 
 * @param dmxHndl Demux Handle
 * @param pInfo include CP information
 * @code 
 	typedef struct
	{
		SdCP_AV_k eAv;
		SdCP_Cipher_k eCipher;
		SdCP_Type_k eType;
		SdCP_Mode_k eMode;
		SdCPKey_t sFirstKey;		///< First Cipher Key
		SdCPKey_t sSecondKey;		///< Second Cipher Key
		SdCPKey_t sThirdKey;		///< Third Cipher Key
	} SdCP_Param_t;
 * @endcode	
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCP_Stop(SdAVDec_DmxHandle dmxHndl, SdCP_Param_t* pInfo);

/*
 * @brief This function returns unique Chip ID 
 * @remarks
 * @param pChipID buffer address for Chip ID. Length of Chip ID is 16 bytes
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCP_GetChipID(uint8_t *pChipID); 

/**
 * @brief This function provides Encryption
 * @remarks
 * @param pParam 
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCP_Encrypt(SdCP_ParamEncDec_t* pParam); 

/**
 * @brief This function provides Decryption 
 * @remarks
 * @param pParam 
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdCP_Decrypt(SdCP_ParamEncDec_t* pParam);

/**
 * @brief SdCP API의 Manual Debug 함수. Manual Debug API for SdCP
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdCP_Debug(void);

#endif


