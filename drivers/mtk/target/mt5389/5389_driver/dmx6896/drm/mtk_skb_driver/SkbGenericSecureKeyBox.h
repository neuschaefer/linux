/*****************************************************************
|
|   Secure Key Box - Generic Implementation
|
|   $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/target/mt53xx_com/53xx_com_driver/dmx6896/drm/mtk_skb_driver/SkbGenericSecureKeyBox.h#1 $
|   Original author: Julien Boeuf 
|
|   This software is provided to you pursuant to your agreement 
|   with Intertrust Technologies Corporation ("Intertrust"). 
|   This software may be used only in accordance with the terms 
|   of the agreement.
|      
|   Copyright (c) 2005-2011 by Intertrust. All rights reserved. 
|
****************************************************************/

#ifndef _SKB_GENERIC_SECURE_KEY_BOX_H_
#define _SKB_GENERIC_SECURE_KEY_BOX_H_

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include "mtk_util.h"
#include "SkbSecureKeyBox.h"
#include "tomcrypt.h"

/*----------------------------------------------------------------------
|   types
+---------------------------------------------------------------------*/
struct SKB_Engine {
    unsigned int        reference_count;
    unsigned int        property_count;
    SKB_EngineProperty* properties;
    const SKB_Byte*     persistent_export_key;
    const SKB_Byte*     cross_engine_export_key;
};

typedef struct {
    SKB_Result (*Release)(SKB_SecureData* self);
    SKB_Result (*GetInfo)(const SKB_SecureData* self, SKB_DataInfo* info);
    SKB_Result (*Derive)(const SKB_SecureData*   self, 
                         SKB_DerivationAlgorithm algorithm, 
                         const void*             parameters,
                         SKB_SecureData**        data);
    /* extra method not part of the 'public' interface */
    MTK_UInt32 (*GetClassId)(const SKB_SecureData* self);
    SKB_Result (*Serialize)(const SKB_SecureData* self, 
                            SKB_Byte*             buffer, 
                            SKB_Size*             buffer_size);
} SKB_SecureDataInterface;

typedef struct {
    SKB_Result (*Release)(SKB_Transform* self);
    SKB_Result (*AddBytes)(SKB_Transform*  self, 
                           const SKB_Byte* data, 
                           SKB_Size        data_size);
    SKB_Result (*AddSecureData)(SKB_Transform*        self, 
                                const SKB_SecureData* data);
    SKB_Result (*GetOutput)(SKB_Transform* self, 
                            SKB_Byte*      output, 
                            SKB_Size*      output_size);    
} SKB_TransformInterface;

typedef struct {
    SKB_Result (*Release)(SKB_Cipher* self);
    SKB_Result (*ProcessBuffer)(SKB_Cipher*     self, 
                                const SKB_Byte* in_buffer, 
                                SKB_Size        in_buffer_size,
                                SKB_Byte*       out_buffer,
                                SKB_Size*       out_buffer_size,
                                const SKB_Byte* iv,
                                SKB_Size        iv_size);
} SKB_CipherInterface;

/* base abstract class for SKB_SecureData concrete subclasses */
struct SKB_SecureData {
    const SKB_SecureDataInterface* iface;
    SKB_Engine*                    engine;
    SKB_DataType                   type;
    unsigned int                   flags;
    SKB_Size                       size;
};

typedef struct {
    SKB_SecureData  base;
    MTK_DataBuffer* bytes;
} SKB_BytesData;

typedef struct {
    SKB_SecureData base;
    rsa_key        key;
} SKB_RsaKeyData;

/* base abstract class for SKB_Transform concrete subclasses */
struct SKB_Transform {
    const SKB_TransformInterface* iface;
    SKB_Engine*                   engine;
    MTK_Boolean                   output_given;
};

typedef struct {
    SKB_Transform base;
    int           hash_idx;
    hash_state    hash;
} SKB_DigestTransform;

typedef struct {
    SKB_Transform base;
    rsa_key*      key;
    int           hash_idx;
    hash_state    hash;
} SKB_RsaDigestSignTransform;

typedef struct {
    SKB_Transform   base;
    rsa_key*        key;
    MTK_DataBuffer* digested_data;
} SKB_RsaSignTransform;

typedef struct {
    SKB_Transform   base;
    int             hash_idx;
    hmac_state      hmac;
    MTK_Boolean     initialized;
} SKB_HmacSignTransform;

typedef struct {
    SKB_Transform   base;
    int             hash_idx;
    hmac_state      hmac;
    MTK_Boolean     initialized;
    MTK_DataBuffer* compare;
} SKB_HmacVerifyTransform;

/* base class for SKB_Cipher */
struct SKB_Cipher {
    const SKB_CipherInterface* iface;
    SKB_Engine*                engine;
    SKB_CipherDirection        direction;
};

typedef struct {
    SKB_Cipher    base;
    symmetric_ECB ecb;
} SKB_AesEcbCipher;

typedef struct {
    SKB_Cipher    base;
    symmetric_CTR ctr;
} SKB_AesCtrCipher;

typedef struct {
    SKB_Cipher    base;
    symmetric_CBC cbc;
} SKB_AesCbcCipher;

typedef struct {
    SKB_Cipher base;
    rsa_key*   key;
    int        padding;
} SKB_RsaCipher;

/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
/*
 * Class IDs used in the exported format to identify which implementation
 * class to use to represent the data.
 * IMPORTANT: these values must not change from release to release.
 */
#define SKB_CLASS_ID_BYTES   0x62797465 /* 'byte' */
#define SKB_CLASS_ID_RSA_KEY 0x7273616b /* 'rsak' */

/*----------------------------------------------------------------------
|   methods
+---------------------------------------------------------------------*/
SKB_Result 
SKB_MapTomCryptResult(int crypt_result);

SKB_Result
SKB_GenericSecureData_CreateFromWrapped(SKB_Engine*           engine,
                                        const SKB_Byte*       wrapped,
                                        SKB_Size              wrapped_size,
                                        SKB_DataType          wrapped_type,
                                        SKB_DataFormat        wrapped_format,
                                        SKB_CipherAlgorithm   wrapping_algorithm,
                                        const void*           wrapping_parameters,
                                        const SKB_SecureData* unwrapping_key,
                                        SKB_SecureData**      data);

SKB_Result
SKB_GenericSecureData_CreateFromExported(SKB_Engine*      engine,
                                         const SKB_Byte*  exported,
                                         SKB_Size         exported_size,
                                         SKB_SecureData** data);

SKB_Result
SKB_GenericTransform_Create(SKB_Engine*       engine,
                            SKB_TransformType type,
                            const void*       parameters,
                            SKB_Transform**   transform);

SKB_Result
SKB_GenericCipher_Create(SKB_Engine*           engine,
                         SKB_CipherAlgorithm   algorithm,
                         SKB_CipherDirection   direction,
                         unsigned int          flags,
                         const void*           parameters,
                         const SKB_SecureData* key,
                         SKB_Cipher**          cipher);
                         
SKB_Result
SKB_GetAes128KeyFromSecureData(const SKB_SecureData* sdata,
                               const SKB_Byte**      key);

#endif /* _SKB_GENERIC_SECURE_KEY_BOX_H_ */

