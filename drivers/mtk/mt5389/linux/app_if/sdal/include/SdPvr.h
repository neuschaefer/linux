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
 * @file SdPvr.h
 * @brief Pvr Function Define Header File
 * @author S/W Platform Lab
 * @date 2006/08/22
 */

#ifndef	_SDPVR_H_
#define	_SDPVR_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#include "SdType.h"
#include "SdMisc.h"

/********************************************************************
	MACRO CONSTANT DEFINITIONS
*********************************************************************/

/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/

/********************************************************************
	TYPE DEFINITION
********************************************************************/

/** 
 * @enum SdPVR_Picture_k
 * Catalog로 부터 오는 I/P/B frame 정보. I/P/B frame information from Catalog
 */
typedef enum {
	SD_PVR_PICTURE_I, ///< I Picture
	SD_PVR_PICTURE_B, ///< B Picture
	SD_PVR_PICTURE_P, ///< P Picture
	SD_PVR_PICTURE_NULL,
	SD_PVR_PICTURE_SH, ///< Sequence Header
} SdPVR_Picture_k;

/** 
 * @enum SdPVR_CallbackType_k
 * Install될 Callback 함수의 Type 정보. Type of Callback function to be installed
 */
typedef enum
{
	SD_PVR_CALLBACK_CATALOG,
	SD_PVR_CALLBACK_STREAM,
} SdPVR_CallbackType_k;

/** 
 * @enum SdPVR_Table_k
 * Record중에 삽입시킬 Table 정보. Table information to be inserted while recording
 */
typedef enum
{
	SD_PVR_TABLE_PAT,
	SD_PVR_TABLE_PMT,
	SD_PVR_TABLE_SIT,
} SdPVR_Table_k;

typedef struct
{
	SdVideoFormat_k eVidFormat;	///< PVR Source가 되는 Stream의 Video Format. Video format of PVR Source Stream
	SdCPKey_t	sEncrytKey;		///< Packet 저장시 암호화 모듈에서 사용하는 Key값. Encryption Key value
} SdPVR_RecordSettings_t;

/**
* @brief 일정량의 stream data를 받았을 때 호출되는 callback 함수. callback called when certain amount of stream data is received
* @remarks callback이 발생하는 stream data의 일정량은 내부적으로 정의됨. the amount of stream data is defined internally
* @param pucBuffer  stream data가 저장된 DMA buffer 주소. DMA buffer address of stream data
* @param uwBufferSize  DMA buffer 크기. size of DMA buffer
* @param uwParam  사용자가 미리 지정한 정보. User predefined parameter
*/
typedef void (*SdPVR_RecordStreamCallback)(uint8_t *pucBuffer, uint32_t uwBufferSize, uint32_t uwParam);


/**
* @brief Catalog data가 발생했을 때 호출되는 callback 함수. callback called when Catalog data is received
* @remarks 
* @param uwPacketCount  kPicture로 전달되는 I/B/P Frame의 packet 위치. Packet position of I/B/P Frame transfered to kPicture
* @param kPicture  I/B/P Frame Information
* @param uwParam  사용자가 미리 지정한 정보. User predefined parameter
*/
typedef void (*SdPVR_RecordCatalogCallback)(uint32_t uwPacketCount, SdPVR_Picture_k kPicture, uint32_t uwParam);

/**
 * @brief PVR 관련 Device들을 초기화하여 사용자가 사용할 수 있도록 한다.
 It initialize the PVR-related Devices so that user can use them.
 * @remarks
 * @param ePowerState Power state: power off->on, off->standby, or standby->on. Default is off->on.
 * @code
		typedef enum
		{
			SD_PWROFF_TO_NORMAL,
			SD_PWROFF_TO_STANDBY,
			SD_STANDBY_TO_NORMAL
		} SdPowerState_k;
 * @endcode
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_Init(SdPowerState_k ePowerState);

/**
* @brief PVR Record시 필요한 정보(Video Format, 암호화 키값)를 설정함.
It sets the information (Video Format, encryption key value) which is required upon recording PVR.
* @remarks RecordConnect()보다 항상 먼저 호출되어야 하는 함수이다. RecordConnect()는 여기서 설정된 정보를 참조하여 Connect작업을 수행한다. 
RecordConnect이후에 값이 변경되는 경우에는 그와 관련한 추가 Action이 발생한다.
It is executed before RecordConnect(). RecordConnect() used information set by this function.
If it is called after RecordConnect(), the corresponding action should be performed.
* @param eRecordId
* @param pSettings
* @return SdResult SD_OK/SD_NOT_OK
* @see 
*/
SdResult SdPVR_RecordSet(SdRecordID_k eRecordId, SdPVR_RecordSettings_t* pSettings);

/**
* @brief PVR Record에 설정된 정보를 읽어옴
It gets the information which is set in PVR Record.
* @remarks 
* @param eRecordId
* @param pSettings
* @return SdResult SD_OK/SD_NOT_OK
* @see 
*/
SdResult SdPVR_RecordGet(SdRecordID_k eRecordId, SdPVR_RecordSettings_t* pSettings);

/**
* @brief callback 함수 지정함
It sets the Callback function.
* @remarks 
* @param eRecordId
* @param eCallbackType
* @param Callback  callback 함수의 주소. callback function
* @param uwParam  callback 호출 시 uwParam로 전달될 정보. parameter of the callback
* @return SdResult SD_OK/SD_NOT_OK
* @see 
*/
SdResult SdPVR_InstallRecordCallback(SdRecordID_k eRecordId, SdPVR_CallbackType_k eCallbackType, void * Callback, uint32_t uwParam);

/**
 * @brief Record Path를 Connect함
 It connects the Record Path.
 * @remarks 항상 Connect전에는 SdPVR_RecordSet()을 이용하여 Video Format과 Encrypt Key를 미리 설정해주어야 한다.
 SdPVR_RecordSet() should be called before this function to get video format and encrypt key.
 * @param eRecordId
 * @param DmxHndl  입력 Stream의 소스가 되는 Demuxer의 handle값. demux handle for input stream
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_RecordConnect(SdRecordID_k eRecordId, SdAVDec_DmxHandle DmxHndl);

/**
 * @brief 설정된 Record Path를 Disconnect함
 It disconnects the Record Path.
 * @remarks 
 * @param eRecordId
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_RecordDisconnect(SdRecordID_k eRecordId);

/**
 * @brief PVR 녹화를 시작함
 It starts the PVR recording.
 * @remarks Digital 소스에 대한 Pids 설정은 SdAVDec모듈의 SdAVDec_StreamOutSetPid() API를 이용한다.
 Use SdAVDec_StreamOutSetPid() in SdAVDec for settings of Pids of digital source 
 * @param eRecordId
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_RecordStart(SdRecordID_k eRecordId);

/**
 * @brief PVR 녹화를 중지함
 It stops the PVR recording.
 * @remarks 
 * @param eRecordId
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_RecordStop(SdRecordID_k eRecordId);

/**
 * @brief Record시 저장될 Table Packet과 관련한 자원 할당및 초기화 수행함
 It allots the Table Packet-related resource to be saved upon recording and performs the initialization.
 * @remarks 
 * @param eRecordId
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_RecordTableOpen(SdRecordID_k eRecordId);

/**
 * @brief Record시 저장될 Table Packet과 관련한 자원 반환 및 정리함
 It returns and arranges the Table Packet-related resources to be saved upon Recording.
 * @remarks 
 * @param eRecordId
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_RecordTableClose(SdRecordID_k eRecordId);

/**
 * @brief Record시 저장될 Table Packet을 설정함
 It sets the Table Packet to be saved upon Recording.
 * @remarks Table Packet을 삽입하는 방식은 HW 지원방식에 따라서 Insert방식과 Replace방식으로 나뉜다.
 Table Packet setting method is defined internally as either Insert method or Replace method depending on the HW support. 
 * @param eRecordId
 * @param pTablePackets 저장될 Table Packet의 배열로서 188byte의 배수 크기를 갖는다. Table Replace방식인 경우는 188byte의 배열사이즈로 고정이다.
 The pointer of Table Packet array and the size is a multiple of 188 byte. This size is fixed to be 188byte for Replace method.
 * @param NumOfPackets 입력된 Table Packet배열에서의 Packet수이다. Table Insert방식의 경우 같은 Table Type의 입력 Packet 수이고, Table Replace방식의 경우에는 무조건 1이다.
 The number of packets in the Table Packet array. It is fixed to be 1 for Replace method.
 * @param hwParam Table Insert방식의 경우 Table Packet 삽입 주기값(단위:ms)이고, Table Replace방식의 경우 Replace의 Target이 되는 Pid 값이다.
 For Insert method it is Table Packet insertion period (unit: ms) and for Replace method it is Pid to be replaced.
 * @param eTableType 저장될 Table의 Type을 정의. Type of Table to be saved
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_RecordTableSet(SdRecordID_k eRecordId, uint8_t *pTablePackets, uint32_t NumOfPackets, uint32_t hwParam, SdPVR_Table_k eTableType);

/**
 * @brief 해당 Table Packet의 삽입 작업을 중지함
 It stops the work of inserting the concerned Table Packet.
 * @remarks 
 * @param eRecordId
 * @param eTableType 저장될 Table의 Type을 정의. Type of Table to be saved
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_RecordTableClear(SdRecordID_k eRecordId, SdPVR_Table_k eTableType);

/**
 * @brief Record Stream에 만들어진 Table Packet 삽입을 시작함
 It starts inserting the Table Packet which has been created in Record Stream.
 * @remarks 
 * @param eRecordId
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_RecordTableStart(SdRecordID_k eRecordId);

/**
 * @brief Record Stream에 만들어진 Table Packet 삽입을 중지함
 It stops the inserting the Table Packet which has been created in Record Stream.
 * @remarks 
 * @param eRecordId
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_RecordTableStop(SdRecordID_k eRecordId);

/**
 * @brief PVR 재생 모듈과 AVDec모듈간의 연결을 설정함
 It sets the connection between PVR playback module and AVDec module.
 * @remarks Timestamp 모듈 동작. 재생과 관련한 Decoder 설정은 SdAVDec 모듈의 API를 사용한다. 
 Connection for Timestamp module. Connection for playback decoder is set by SdAVDec.
 * @param ePlayId 
 * @param eAVDecId 
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_PlayConnect(SdPlayID_k ePlayId, SdAVDecID_k eAVDecId);

/**
 * @brief PVR 재생 모듈과 AVDec모듈간의 연결을 해제함
 It releases the connection between PVR playback module and AVDec module.
 * @remarks 
 * @param ePlayId 
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_PlayDisconnect(SdPlayID_k ePlayId);

/**
 * @brief PVR 재생을 시작함
 It starts the PVR playback.
 * @remarks Timestamp 모듈 동작
 * @param ePlayId 
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_PlayStart(SdPlayID_k ePlayId);

/**
 * @brief PVR 재생을 중지함
 It stops the PVR playback.
 * @remarks 
 * @param ePlayId 
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_PlayStop(SdPlayID_k ePlayId);

/**
 * @brief 사용가능한 Play Buffer를 얻어옴
 It gets the available Play Buffer.
 * @remarks 
 * @param ePlayId
 * @param ppucBuffer Buffer 정보가 저장된 주소. SdPVR을 사용하는 모듈에서 할당해서 제공하는 것이 아니라 함수를 호출하면 내부적으로 할당되고 그 주소만 반환된다.
 Buffer 정보를 전송하기 위해 사용되는 공간은 SdPVR내부에 static array로 SI Buffer 개수 만큼 정의하면 할당/해제를 관리할 필요없이 사용될 수 있다.
 Buffer Address. The buffer is allocated internally in SdPVR and returned.
 
 * @param puwBufferSize Buffer의 크기. size of Buffer
 * @param puwNumBuffers ppucBuffer에 저장된 Buffer 주소의 개수. The number of Buffer addresses stored in ppucBuffer
 * @param ppVideoBaseAddr VideoBase 어드레스를 얻어온다. The address of VideoBase
 * @param TrickState
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_PlayGetBuffers(SdPlayID_k ePlayId, uint8_t** ppucBuffer, uint32_t* puwBufferSize, uint32_t* puwNumBuffers, uint8_t** ppVideoBaseAddr, uint32_t TrickState);

/**
 * @brief 해당 Play Buffer의 데이타의 Demux 이동을 요청함
 It requests to shift the concerned Play Buffer's data to Demux.
 * @remarks 
 * @param ePlayId
 * @param pucBuffer Stream data가 저장된 Buffer의 주소값 Array. Array of Buffer addresses storing Stream data.
 * @param uwNumBuffers Stream data가 저장된 Buffer의 개수. The number of Buffers storing Stream data
 * @param ppucoffsetBuffer 이동한 SI Offset번지를 얻어온다. Address of SI Offset
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_PlaySubmit(SdPlayID_k ePlayId, uint8_t *pucBuffer, uint32_t uwNumBuffers, uint8_t** ppucoffsetBuffer);

/**
 * @brief 메모리상의 암호화된 data를 해독하여 다른 메모리에 복사한다.
 It decrypts the encrypted data in memory and copy to another address.
 * @remarks 보통 썸네일을 만들기 위한 작업에서 사용된다. This function is generally used in creating a thumbnail.
 * @param pDestination 해독된 data가 저장될 메모리 주소. Memory address for decrypted data.
 * @param pSource 암호화된 data가 저장된 메모리 주소. Memory addres for encrypted source data.
 * @param ucBuffersize 소스 data가 저장된 버퍼의 크기(해독할 data의 크기). size of source data to be decrypted
 * @param sDecryptKey 암호해독에 사용되는 Key값. Key for the decryption
 * @return SdResult SD_OK/SD_NOT_OK
 * @see 
 */
SdResult SdPVR_DecryptMemdata(uint8_t* pDestination, uint8_t* pSource, uint32_t ucBuffersize, SdCPKey_t sDecryptKey);

/**
 * @brief SdPVR API의 Manual Debug 함수
 Manual Debug function of SdPVR API
 * @remarks SdDebug.h에 정의된 SdDebug()에 의해 호출된다. It is called by SdDebug() in SdDebug.h. 
 * @return 
 * @see 
 */
void SdPVR_Debug(void);

#endif	/*_SDPVR_H_*/
