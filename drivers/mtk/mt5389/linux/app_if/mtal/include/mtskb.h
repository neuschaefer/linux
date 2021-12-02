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
/*-----------------------------------------------------------------------------
 * $RCSfile: mtdmx.h,v $
 * $Revision: #2 $
 *---------------------------------------------------------------------------*/

/** @file mtdmx_mwif.h
 *  This header file declares exported APIs of DMX module.
 */

#ifndef MT_SKB_H
#define MT_SKB_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mttype.h"
#ifdef __KERNEL__
#include "SkbSecureKeyBox.h"
#include <linux/types.h>
///========HCI============
#include "basic_types.h"
#include "dbg_api.h"
#include "mem_api.h"
#include "binstr_api.h"
#include "hci_api.h"
///========HCI============
#endif

//-----------------------------------------------------------------------------
// MTAL Constant definitions
//-----------------------------------------------------------------------------
#define L_MS3_CONTENT_ID_SIZE     (20)

//-----------------------------------------------------------------------------
// MTAL Type definitions
//-----------------------------------------------------------------------------

#ifndef __KERNEL__
typedef BOOL            bool_t;
typedef u_int32_t       np_ret_t;
typedef np_ret_t		retcode_t;
//typedef unsigned int	u_int32_t;
typedef u_int32_t		np_size_t;

typedef struct {
    np_size_t   len;
    u_int8_t   *buf;
} np_str_t;

typedef np_str_t binstr_t;
#endif

#ifndef DRM_ERROR_INF_DEFINED
#define DRM_ERROR_INF_DEFINED
  // Copy from ErrorInf.h
  typedef struct _DRM_ErrorInf
  {
    unsigned long code1;	//!< Detailed code 1 (used inside Fujitsu)
    unsigned long code2;	//!< Detailed code 2 (used inside Fujitsu)
    unsigned long code3;	//!< Detailed code 3 (public detailed error code)
  } DRM_ErrorInf;		//!< Error information structure
#endif /* DRM_ERROR_INF_DEFINED */

#ifndef DRM_EXTRACT_INFO_DEFINED
#define DRM_EXTRACT_INFO_DEFINED
  typedef struct _DRM_Extract_Info
  {
    unsigned long not_before;              //!< NotBefore
    unsigned long not_after;               //!< NotAfter
    unsigned char rendering_obligation[2]; //!< Rendering obligation
    unsigned char ip_service_provider_id;  //!< IP service provider identifier
    unsigned char reserve[1];              //!< Unused
  } DRM_ExtractInfo;
#endif /* DRM_EXTRACT_INFO_DEFINED */

#ifndef DRM_EXPORT_INFO_DEFINED
#define DRM_EXPORT_INFO_DEFINED
  typedef struct _DRM_Export_Info
  {
    unsigned char export_type;             //!< Type of the target of exportation.
    unsigned char reserve[1];              //!< Unused
    unsigned char ip_service_provider_id;  //!< IP service provider identifier
    unsigned char export_info_len;         //!< Byte size of the "ExportInfo"
    unsigned char export_info[24];         //!< The value of the "ExportInfo" that depends on the value of the export_type
  } DRM_ExportInfo;
#endif /* DRM_EXPORT_INFO_DEFINED */

#ifndef DECODER_DRMOFFSET_DEFINED
#define DECODER_DRMOFFSET_DEFINED
  typedef struct _DecoderDRM_Offset
  {
    size_t offset;                         //!< offset in the packet
    size_t size;                           //!< size of the payload to decrypt. If 0, this function doesn't decrypt that packet.
  } DecoderDRM_Offset;
#endif /* DECODER_DRMOFFSET_DEFINED */

//-----------------------------------------------------------------------------
// Private MW_IF Constant definitions
//-----------------------------------------------------------------------------
#define MTDMX_MWIF_SKB_ENGINE_GET_INSTANCE              (100)
#define MTDMX_MWIF_SKB_ENGINE_RELEASE                   (101)
#define MTDMX_MWIF_SKB_ENGINE_GET_INFO                  (102)
#define MTDMX_MWIF_SKB_ENGINE_GET_PROPERTIES_SIZE       (103)
#define MTDMX_MWIF_SKB_ENGINE_GET_PROPERTIES_DATA       (104)
#define MTDMX_MWIF_SKB_ENGINE_GET_DATA_BY_NAME          (105)
#define MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_WRAPPED  (106)
#define MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_EXPORTED (107)
#define MTDMX_MWIF_SKB_ENGINE_CREATE_TRANSFORM          (108)
#define MTDMX_MWIF_SKB_ENGINE_CREATE_CIPHER             (109)
#define MTDMX_MWIF_SKB_SECUREDATA_RELEASE               (110)
#define MTDMX_MWIF_SKB_SECUREDATA_GETINFO               (111)
#define MTDMX_MWIF_SKB_SECUREDATA_EXPORT                (112)
#define MTDMX_MWIF_SKB_SECUREDATA_DERIVE                (113)
#define MTDMX_MWIF_SKB_TRANSFORM_RELEASE                (114)
#define MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES               (115)
#define MTDMX_MWIF_SKB_TRANSFORM_ADDSECUREDATA          (116)
#define MTDMX_MWIF_SKB_TRANSFORM_GETOUTPUT              (117)
#define MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER             (118)
#define MTDMX_MWIF_SKB_CIPHER_RELEASE                   (119)

#define MTDMX_MWIF_HCI_INIT                             (200)
#define MTDMX_MWIF_HCI_FIN                              (201)
#define MTDMX_MWIF_HCI_KEYREF_CREATE                    (202)
#define MTDMX_MWIF_HCI_KEYREF_CHECK                     (203)
#define MTDMX_MWIF_HCI_KEYREF_DESTROY                   (204)
#define MTDMX_MWIF_HCI_DEVICE_GET_NUM                   (205)
#define MTDMX_MWIF_HCI_DEVICE_GET_SIZE                  (206)
#define MTDMX_MWIF_HCI_DEVICE_GET_NAME                  (207)
#define MTDMX_MWIF_HCI_IMPORT_KEY                       (208)
#define MTDMX_MWIF_HCI_GET_START_FISH_ID_SIZE           (209)
#define MTDMX_MWIF_HCI_GET_START_FISH_ID                (210)
#define MTDMX_MWIF_HCI_GET_START_FISH_KEYDATA_SIZE      (211)
#define MTDMX_MWIF_HCI_GET_START_FISH_KEYDATA           (212)
#define MTDMX_MWIF_HCI_IMPORT_START_FISH_KEY            (213)
#define MTDMX_MWIF_HCI_DATA_SIGN_SIZE                   (214)
#define MTDMX_MWIF_HCI_DATA_SIGN                        (215)
#define MTDMX_MWIF_HCI_DATA_DECRYPT_SIZE                (216)
#define MTDMX_MWIF_HCI_DATA_DECRYPT                     (217)
#define MTDMX_MWIF_HCI_STREAM_DECRYPT_SIZE              (218)
#define MTDMX_MWIF_HCI_STREAM_DECRYPT                   (219)
#define MTDMX_MWIF_HCI_KEYDATA_ENCRYPT_SIZE             (220)
#define MTDMX_MWIF_HCI_KEYDATA_ENCRYPT                  (221)
#define MTDMX_MWIF_HCI_HMAC_CHECK                       (222)
#define MTDMX_MWIF_HCI_HASH_CHECK                       (223)
#define MTDMX_MWIF_HCI_MOVE_KEY_SIZE                    (224)
#define MTDMX_MWIF_HCI_MOVE_KEY                         (225)
#define MTDMX_MWIF_PERSONALITYKEY_LOAD                  (226)
#define MTDMX_MWIF_PERSONALITYKEY_CHECK                 (227)
#define MTDMX_MWIF_IMPORT_SAS_SIZE                      (228)
#define MTDMX_MWIF_IMPORT_SAS                           (229)
#define MTDMX_MWIF_IMPORT_SAS_IDS                       (230)
#define MTDMX_MWIF_FREE_SAS                             (231)
#define MTDMX_MWIF_HCI_EXPORT_KEY_SIZE                  (232)
#define MTDMX_MWIF_HCI_EXPORT_KEY                       (233)

#define MTDMX_MWIF_DRM_TZ_SETSECRETDATATYPE             (300)
#define MTDMX_MWIF_DRM_TZ_OPEN                          (301)
#define MTDMX_MWIF_DRM_TZ_CLOSE                         (302)
#define MTDMX_MWIF_DRM_TZ_DECODEPACKETS_VOD             (303)
#define MTDMX_MWIF_DRM_TZ_LOADSECRETDATATYPE            (304)

//-----------------------------------------------------------------------------
//  Private MW_IF Type definitions
//-----------------------------------------------------------------------------
typedef struct
{
    UINT32 u4InterfaceId;
    SKB_Engine *pEngine;
    SKB_Result rRet;
} MTDMX_MWIF_SKB_ENGINE_GET_INSTANCE_T;

typedef struct
{
    UINT32 u4InterfaceId;
    SKB_Engine *pEngine;
    SKB_Result rRet;
} MTDMX_MWIF_SKB_ENGINE_RELEASE_T;

typedef struct
{
    UINT32 u4InterfaceId;
    const SKB_Engine *pEngine;
    SKB_EngineInfo *pInfo;
    SKB_EngineProperty *pProperties;
    UINT32 u4EngineInfoSize;
    UINT32 u4PropertiesIndex;
    UINT32 u4PropertiesNameSize;
    UINT32 u4PropertiesValueSize;
    SKB_Result rRet;
} MTDMX_MWIF_SKB_ENGINE_GET_INFO_T;


typedef struct
{
    UINT32 u4InterfaceId;
    SKB_Engine *pEngine;
    const SKB_Byte* wrapped;
    SKB_Size wrapped_size;
    SKB_DataType wrapped_type;
    SKB_DataFormat wrapped_format;
    SKB_CipherAlgorithm wrapping_algorithm;
    const void* wrapping_parameters;
    const SKB_SecureData* unwrapping_key;
    SKB_SecureData* data;
    SKB_Result rRet;
} MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_WRAPPED_T;


typedef struct
{
    UINT32 u4InterfaceId;
    SKB_Engine *pEngine;
    const SKB_Byte* exported;
    SKB_Size exported_size;
    SKB_SecureData* data;
    SKB_Result rRet;
} MTDMX_MWIF_SKB_ENGINE_CREATE_DATA_FROM_EXPORTED_T;


typedef struct
{
    UINT32 u4InterfaceId;
    SKB_Engine *pEngine;
    SKB_TransformType transform_type;
    const void* transform_parameters;
    SKB_Transform* transform;
    SKB_Result rRet;
} MTDMX_MWIF_SKB_ENGINE_CREATE_TRANSFORM_T;


typedef struct
{
    UINT32 u4InterfaceId;
    SKB_Engine *pEngine;
    SKB_CipherAlgorithm cipher_algorithm;
    SKB_CipherDirection cipher_direction;
    unsigned int cipher_flags;
    const void* cipher_parameters;
    const SKB_SecureData* cipher_key;
    SKB_Cipher* cipher;
    SKB_Result rRet;
} MTDMX_MWIF_SKB_ENGINE_CREATE_CIPHER_T;

typedef struct
{
    UINT32 u4InterfaceId;
    const SKB_SecureData* self;
    SKB_DataInfo* info;
    SKB_Result rRet;
} MTDMX_MWIF_SKB_SECUREDATA_T;

typedef struct
{
    UINT32 u4InterfaceId;
    const SKB_SecureData* self;
    SKB_ExportTarget target;
    const void* target_parameters;
    SKB_Byte* buffer;
    SKB_Size* buffer_size;
    SKB_Result rRet;
} MTDMX_MWIF_SKB_SECUREDATA_EXPORT_T;

typedef struct
{
    UINT32 u4InterfaceId;
    const SKB_SecureData* self;
    SKB_DerivationAlgorithm algorithm;
    const void* parameters;
    SKB_SecureData* data;
    SKB_Result rRet;
} MTDMX_MWIF_SKB_SECUREDATA_DERIVE_T;

typedef struct
{
    UINT32 u4InterfaceId;
    SKB_Transform* self;
    const SKB_Byte* data;
    SKB_Size data_size;
    SKB_Result rRet;
} MTDMX_MWIF_SKB_TRANSFORM_ADDBYTES_T;

typedef struct
{
    UINT32 u4InterfaceId;
    SKB_Transform* self;
    const SKB_SecureData* data;
    SKB_Result rRet;
} MTDMX_MWIF_SKB_TRANSFORM_ADDSECUREDATA_T;

typedef struct
{
    UINT32 u4InterfaceId;
    SKB_Transform* self;
    SKB_Byte* output;
    SKB_Size* output_size;
    SKB_Result rRet;
} MTDMX_MWIF_SKB_TRANSFORM_GETOUTPUT_T;

typedef struct
{
    UINT32 u4InterfaceId;
    SKB_Cipher* self;
    const SKB_Byte* in_buffer;
    SKB_Size in_buffer_size;
    SKB_Byte* out_buffer;
    SKB_Size* out_buffer_size;
    const SKB_Byte* iv;
    SKB_Size iv_size;
    SKB_Result rRet;
} MTDMX_MWIF_SKB_CIPHER_PROCESSBUFFER_T;

typedef struct
{
    UINT32 u4InterfaceId;
    UINT32 u4Ret;
} MTDMX_MWIF_HCI_INIT_T;

typedef struct
{
    UINT32 u4InterfaceId;
    UINT32 u4KeyType;
    void *pKeyRef;
    UINT32 u4Ret;
} MTDMX_MWIF_HCI_KEY_REF_T;

typedef struct
{
    UINT32 u4InterfaceId;
    UINT32 u4IdNum;
    UINT32 u4IdIdx;
    UINT32 u4IdSize;
    void *pBuf;
    UINT32 u4Ret;
} MTDMX_MWIF_HCI_ID_T;

typedef struct
{
    UINT32 u4InterfaceId;
    void *pKeyInRef;
    UINT32 u4EncAlg;
    void* pOAEPParam;
    void* pKeyData;
    UINT32 u4KeyType;
    void *pKeyOutRef;
    UINT32 u4Ret;
} MTDMX_MWIF_HCI_IMPORT_KEY_T;

typedef struct
{
    UINT32 u4InterfaceId;
    void *pKeyRef;
    UINT32 u4IdSize;
    void *pBuf;
    UINT32 u4Ret;
} MTDMX_MWIF_HCI_GET_STARFISH_KEYDATA_T;

typedef struct
{
    UINT32 u4InterfaceId;
    UINT32 u4IterationNum;
    UINT32 u4KeyIndex;
    void *pEncBK;
    void *pBKHash;
    void *pIv;
    void *pEncCK;
    void *pKeyOutRef;
    UINT32 u4Ret;
} MTDMX_MWIF_HCI_IMPORT_STARFISH_KEY_T;

typedef struct
{
    UINT32 u4InterfaceId;
    void *pKeyInRef;
    UINT32 u4Alg;
    UINT32 u4Pad;
    UINT8 *pInSaltBuf;
    void *pInOAEPParam;
    void *pInData;
    UINT32 u4Size;
    void *pOutData;
    UINT32 u4Ret;
} MTDMX_MWIF_HCI_DATA_OP_T;

typedef struct
{
    UINT32 u4InterfaceId;
    void *pKeyInRef;
    UINT32 u4Alg;
    void *pInData;
    void *pInExpected;
    UINT32 u4OutResult;
    UINT32 u4Ret;
} MTDMX_MWIF_HCI_HASH_T;

typedef struct
{
    UINT32 u4InterfaceId;
    void *pKeyRef;
    void *pInData;
    UINT32 u4Size;
    void *pOutData;
    UINT32 u4Ret;
} MTDMX_MWIF_HCI_MOVEKEY_T;

typedef struct
{
    UINT32 u4InterfaceId;
    UINT32 u4KeyType;
    UINT32 u4InSize;
    void *pInData;
    UINT32 u4Ret;
} MTDMX_MWIF_HCI_PERSONALITY_KEY_T;

typedef struct
{
    UINT32 u4InterfaceId;
    void *pInSAS;
    void *pInApp;
    void *pOutSAS;
    UINT32 u4OutSASSize;
    void *pOutKeys;
    UINT32 u4OutKeysCount;
    UINT32 u4Ret;
} MTDMX_MWIF_HCI_IMPORT_SAS_T;

typedef struct
{
    UINT32 u4InterfaceId;
    INT32 i4SecretDataType;
    void *pRootCertBuf;
    UINT32 u4RootCertBufLen;
    UINT32 u4RootCertLen;
    void *pCaCertBuf;
    UINT32 u4CaCertBufLen;
    UINT32 u4CaCertLen;
    void *pClientCertBuf;
    UINT32 u4ClientCertBufLen;
    UINT32 u4ClientCertLen;
    void *pClientKeyBuf;
    UINT32 u4ClientKeyBufLen;
    UINT32 u4ClientKeyLen;
    DRM_ErrorInf *prErrorInf;
    INT32 i4Ret;
} MTDMX_MWIF_DRM_TZ_SETSECRETDATATYPE_T;

typedef struct
{
    UINT32 u4InterfaceId;
    INT32 i4DecSlot;
    INT32 i4SecretDataType;
    void *pLicenseObj;
    UINT32 u4LicenseObjLen;
    void *pLid;
    UINT32 u4LidLen;
    DRM_ExtractInfo* pExtractInfo;
    DRM_ErrorInf *prErrorInf;
    INT32 i4Ret;
} MTDMX_MWIF_DRM_TZ_OPEN_T;

typedef struct
{
    UINT32 u4InterfaceId;
    INT32 i4DecSlot;
    INT32 u4PacketSize;
    void *pInBuf;
    UINT32 u4InNumOfPackets;
    DecoderDRM_Offset* pOffsets;
    void *pOutBuf;
    UINT32 u4OutBufLen;
    UINT32 u4OutNumOfPackets;
    DRM_ErrorInf *prErrorInf;
    INT32 i4Ret;
} MTDMX_MWIF_DRM_TZ_DECODEPACKETS_VOD_T;


#endif //MT_SKB_H

