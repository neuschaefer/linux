/********************************************************************************************
 *     LEGAL DISCLAIMER
 *
 *     (Header of MediaTek Software/Firmware Release or Documentation)
 *
 *     BY OPENING OR USING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 *     THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE") RECEIVED
 *     FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON AN "AS-IS" BASIS
 *     ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED,
 *     INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
 *     A PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY
 *     WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 *     INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK
 *     ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
 *     NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION
 *     OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
 *
 *     BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE LIABILITY WITH
 *     RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION,
 *     TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE
 *     FEES OR SERVICE CHARGE PAID BY BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 *     THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE WITH THE LAWS
 *     OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF LAWS PRINCIPLES.
 ************************************************************************************************/
#ifndef _X_TZ_CUSTOMER_H_
#define _X_TZ_CUSTOMER_H_

/*-----------------------------------------------------------------------------
                    include files
-----------------------------------------------------------------------------*/
#ifndef CC_SECURE_WORLD
#include "u_common.h"
#endif
//#include "x_common.h"

/*-----------------------------------------------------------------------------
                    macros, defines, typedefs, enums
-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
                    functions declarations
-----------------------------------------------------------------------------*/
extern BOOL x_mtk_tz_in_api_cpsm_marlin_get_dev_id
(
UINT8* pui1_dev_id
);

/// MTK SHA256 Init Crypto API
/// \return None.
extern void x_mtk_tz_in_api_sha256_Init(
UINT32* pbHandle    ///< [OUT] SHA256 CTX Handle
);

/// MTK SHA256 Update Crypto API
/// \return None.
extern void x_mtk_tz_in_api_sha256_Update(
  UINT32* pbHandle,    ///< [IN] SHA256 CTX Handle
  UINT32 u4Len,           ///< [IN] SHA256 Message Length (Unit: Byte)
  UINT8* pbbuf              ///< [IN] SHA256 Message Buffer Address
);

/// MTK SHA256 Final Crypto API
/// \return None.
extern void x_mtk_tz_in_api_sha256_Final(
  UINT32* pbHandle,       ///< [IN] SHA256 CTX Handle
  UINT8* pbout_digest     ///< [OUT] SHA256 Hash Result Buffer Address (32 Byte)
);

/// MTK SHA256 Crypto API
/// \return None.
extern void x_mtk_tz_in_api_sha256_MakeDigest(
  UINT8* pbbuf,                  ///< [IN] Source Message Buffer Address
  UINT32 u4len,                 ///< [IN] Source Message Data Length (Unit: Byte)
  UINT8* pbout_digest       ///< [OUT] Hash Result Buffer Address
);

/// MTK SHA1 Init Crypto API
/// \return None.
extern void x_mtk_tz_in_api_sha1_Init(
  UINT32* pbHandle         ///< [OUT] SHA1 CTX Handle
);

/// MTK SHA1 Update Crypto API
/// \return None.
extern void x_mtk_tz_in_api_sha1_Update(
  UINT32* pbHandle,       ///< [IN] SHA1 CTX Handle
  UINT32 u4Len,              ///< [IN] SHA1 Message Length (Unit: Byte)
  UINT8* pbbuf                 ///< [IN] SHA1 Message Buffer Address
);

/// MTK SHA1 Final Crypto API
/// \return None.
extern void x_mtk_tz_in_api_sha1_Final(
  UINT32* pbHandle,         ///< [IN] SHA1 CTX Handle
  UINT8* pbout_digest       ///< [IN] SHA1 Hash Result Buffer Address (20 Byte)
);

/// MTK SHA1 Crypto API
/// \return None.
extern void x_mkt_tz_in_api_sha1_MakeDigest(
  UINT8* pbbuf,                  ///< [IN] Source Message Buffer Address
  UINT32 u4len,                 ///< [IN] Source Message Data Length (Unit: Byte)
  UINT8* pbout_digest       ///< [OUT] Hash Result Buffer Address
);

/// MTK AES 128Bit ECB Encryption Crypto API
/// \return None.
extern void x_mtk_tz_in_api_aes128e_ecb(
  UINT8* pbKey,               ///< [IN] Encruption Key Buffer Address
  UINT8* bpin_buf,           ///< [IN] Input Data Buffer Address
  UINT8* bpout_buf         ///< [OUT] Encryption Result Buffer Address
);


/// MTK AES 128Bit ECB Decryption Crypto API
/// \return None.
extern void x_mtk_tz_in_api_aes128d_ecb(
  UINT8* pbKey,              ///< [IN] Decryption Key Buffer Address
  UINT8* bpin_buf,          ///< [IN] Input Data Buffer Address
  UINT8* bpout_buf        ///< [OUT] Decryption Result Buffer Address
);

/// MTK Random Number Generator
/// \return None.
extern void x_mtk_tz_in_api_rng(
  UINT8* pbRandomDataBuf,          ///< [OUT] Random Data Buffer Address
  UINT32 u4DataLength                ///< [IN] Random Data Length (Unit: Byte)
);

extern BOOL x_mtk_tz_in_api_vendor_enc_pd0
(
UINT8* pui1_src,         ///< [IN]     source
UINT8* pui1_des,         ///< [IN/OUT] destination
UINT32 ui4_sz            ///< [IN]     size of source/destination, 16 byte alignment
);

extern BOOL x_mtk_tz_in_api_vendor_dec_pd0
(
UINT8* pui1_src,         ///< [IN]     source
UINT8* pui1_des,         ///< [IN/OUT] destination
UINT32 ui4_sz            ///< [IN]     size of source/destination, 16 byte alignment
);

/// MTK Set Key API for Nautilius
/// \return If return value = FALSE, it's failed.
extern BOOL x_mtk_tz_in_api_cpsa_marlin_set_content_key (
  void* pv_priv,                                   ///< [IN] Private Data for MTK Internal Use (Get this data pointer from Get Content Key API)
  UINT8* pui1_content_key,               ///< [IN] Marlin Content Key Pointer
  UINT32* pui4_content_key_sz         ///< [IN] Maile Content Key Size
);

extern BOOL x_mtk_tz_in_api_marlin_get_dev_key_sz
(
UINT32* pui4_sz          ///< [IN/OUT] device key size
);

extern BOOL x_mtk_tz_in_api_marlin_get_dev_key
(
UINT8* pui1_dev_key      ///< [IN/OUT] device key
);

#endif /* _X_TZ_CUSTOMER_H_ */

