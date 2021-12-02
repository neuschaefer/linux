/** @addtogroup hci hci
 * @ingroup marlin
 * @{ */

/*
 * Nautilus version 3.5
 *
 * Any code and mechanism in this module may not be used
 * in any form without permissions.
 * Copyright (C) 2010,2011 Sony Corporation, All Rights Reserved.
 */

#ifndef HCI_API_H
#define HCI_API_H

#ifdef __cplusplus
extern "C"
{
#endif

/* -------------------------------------------------------------------------- */
/*                                                     PUBLIC DATA STRUCTURES */
/* -------------------------------------------------------------------------- */

/** Abstract type for an hci lock */
typedef void* hci_lock_t;

/** Abstract type for a reference to a key protected by the hci interface */
typedef void* hci_key_ref_t;

/**
 * HCI key types
 *
 * Type to specific the key being passed to the interface
 **/
typedef enum  {
    HCI_KEY_ALGO_TYPE_AES             = 0,     /** AES CBC */
    HCI_KEY_ALGO_TYPE_RSA             = 1,     /** RSA PKCS1 v1.5 */
    HCI_KEY_ALGO_TYPE_UNSPECIFIED              /** Unknown algorithm */
} hci_key_algo_type_t;

typedef enum  {
    HCI_KEY_TYPE_NPPR_ENC         = 0,   /** Nemo private rsa key (encoding) */
    HCI_KEY_TYPE_NPPR_SIG         = 1,   /** Nemo private rsa key (signing) */
    HCI_KEY_TYPE_OPPR_PRV         = 2,   /** Octopis private rsa key */
    HCI_KEY_TYPE_OPPR_SEC         = 3,   /** Octopus secret aes key */
    HCI_KEY_TYPE_DEV_UNIQ         = 4,   /** Device unique key */
    HCI_KEY_TYPE_DOMAIN           = 5,   /** Domain key */
    HCI_KEY_TYPE_BROADCAST        = 6,   /** Broadcast key */
    HCI_KEY_TYPE_CONTENT          = 7,   /** Content key */
    HCI_KEY_TYPE_MESSAGE          = 8,   /** Nemo message key */
    HCI_KEY_TYPE_UNSPECIFIED             /** Unknown Key Type */
} hci_key_type_t;

/**
 * HCI Signature Algorithm types
 *
 * Type to specify the algorithm for a signing operation
 **/
typedef enum  {
    HCI_SIG_ALGO_RSA_PKCS1_V1_5_SHA1   = 0, /** Use SHA1 hashing before signing */
    HCI_SIG_ALGO_RSA_PKCS1_V1_5_SHA256 = 1, /** Use SHA256 hashing before signing */
    HCI_SIG_ALGO_UNSPECIFIED                /** Unknown hash algorithm */
} hci_sig_algo_t;

/**
 * HCI Encryption Algorithm types
 *
 * Type to specify the algorithm for an encryption operation
 **/
typedef enum  {
    HCI_ENC_ALGO_RSA_PKCS1_V1_5 = 0,
    HCI_ENC_ALGO_RSA_OAEP       = 1,
    HCI_ENC_ALGO_AES_CBC        = 2,
    HCI_ENC_ALGO_AES_CTR        = 3,
    HCI_ENC_ALGO_UNSPECIFIED
} hci_enc_algo_t;

/**
 * HCI Encryption Padding types
 *
 * Type to specify the padding for an encryption operation
 **/
typedef enum  {
    HCI_ENC_PAD_NONE        = 0,
    HCI_ENC_PAD_PKCS5       = 1,
    HCI_ENC_PAD_RESIDUAL    = 2, /* For BBTS MPEG-TS' residual termination block processing*/
    HCI_ENC_PAD_UNSPECIFIED
} hci_enc_pad_t;

/**
 * HCI Digest Algorithm types
 *
 * Type to specify the algorithm for a digest operation
 **/
typedef enum  {
    HCI_DIG_ALGO_SHA1 = 0,
    HCI_DIG_ALGO_SHA256 = 1,
    HCI_DIG_ALGO_UNSPECIFIED
} hci_dig_algo_t;


/**
 * HCI Personality types
 *
 * Type to specify the personality
 **/
typedef enum  {
    HCI_PERSONALITY_KEY_TYPE_OCTPRIV,
    HCI_PERSONALITY_KEY_TYPE_NEMOPRIV
} hci_pers_key_type_t;

/**
 *
 * Type to specify the SAS keys for ms3
 **/
typedef struct hci_sas_keys_s {
    u_int32_t           key_count;         /* number of keys */
    binstr_t          **contentids;        /* list of content ids */
    hci_key_ref_t     **key_refs;          /* list of key refs */
} hci_sas_keys_t;

/** The signature check failure under the HCI interface  */
#define ERR_HCI_INVALIDSIG                  (ERR_HCI_BASE|1)
/** An operation was attempted on a key whose keydata is not in the HCI key reference */
#define ERR_HCI_KEYNOTINREF                 (ERR_HCI_BASE|2)
/** Decryption with an HCI key reference failed */
#define ERR_HCI_DECRYPTIONFAILED            (ERR_HCI_BASE|3)
/** Illegal personality found */
#define ERR_HCI_ILLEGALPERSONALITY          (ERR_HCI_BASE|4)
/** No personality was available to be used */
#define ERR_HCI_NOPERSONALITYREGISTERED     (ERR_HCI_BASE|5)
/** An operation was attempted without having acquired the lock */
#define ERR_HCI_NOTLOCKED                   (ERR_HCI_BASE|6)
/** Acquiring the lock failed */
#define ERR_HCI_LOCKUNAVAILABLE             (ERR_HCI_BASE|7)
/** Parsing the SAS failed */
#define ERR_HCI_ILLEGALSAS                  (ERR_HCI_BASE|8)

/* -------------------------------------------------------------------------- */
/*                                                                 PUBLIC API */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/**
 * @brief Init the hci interface. This will be called during np_Init(). It 
 *        gives the platform a chance to setup the hci side before any other 
 *        hci calls are invoked.
 */
/* -------------------------------------------------------------------------- */
extern retcode_t 
hci_Init(void);


/* -------------------------------------------------------------------------- */
/**
 * @brief Finalise the hci interface. Called in np_Fin. It allows the platform
 *        a chance to cleanup any resources held by the hci side before 
 *        Nautilus is finalised.
 */
/* -------------------------------------------------------------------------- */
extern void
hci_Fin(void);


/* -------------------------------------------------------------------------- */
/**
 * @brief Lock the hci interface. 
 * Generally, the hci interface needs to be locked before operations using key 
 * references.
 * The hci interface can be unlocked again with @ref hci_Unlock.
 *
 * @param[out] out_lock   Lock handle
 *
 * @note If the lock was already acquired by another thread, the implementation
 *       must block until the lock is released again.
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_LOCKUNAVAILABLE
 */
/* -------------------------------------------------------------------------- */
extern retcode_t 
hci_Lock(hci_lock_t *out_lock);


/* -------------------------------------------------------------------------- */
/**
 * @brief Unlock the hci interface.
 *
 * @param[in,out] io_lock   Lock handle
 *
 * @return ::ERR_OK
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
extern retcode_t 
hci_Unlock(hci_lock_t io_lock);


/* -------------------------------------------------------------------------- */
/**
 * @brief Create a key reference. This is used any time an HCI operation is 
 * performed using the key.
 * A key reference can be distroyed with @ref hci_KeyRefDestroy.
 *
 * @param[in]  in_key_type The requested key type. Supported key types are:
 *                           @ref HCI_KEY_TYPE_NPPR_ENC
 *                           @ref HCI_KEY_TYPE_NPPR_SIG
 *                           @ref HCI_KEY_TYPE_OPPR_PRV
 *                           @ref HCI_KEY_TYPE_OPPR_SEC
 *
 * @note @ref HCI_KEY_TYPE_DEV_UNIQ will be supported in the future 
 *
 * @param[out] out_ref     Reference to the key
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
extern retcode_t 
hci_KeyRefCreate(hci_key_type_t     in_key_type, 
                 hci_key_ref_t     *out_ref);


/* -------------------------------------------------------------------------- */
/**
 * @brief Check whether a key reference is still valid.
 *
 * @param[in]  in_ref      Key reference to check
 *
 * @return TRUE if the key reference is valid
 * @return FALSE otherwise
 */
/* -------------------------------------------------------------------------- */
extern bool_t
hci_KeyRefCheck(hci_key_ref_t in_ref);

/* -------------------------------------------------------------------------- */
/**
 * @brief Destroy the key reference.
 *
 * @param[in,out] io_ref Reference to the key to be destroyed.
 *
 * @return ::ERR_OK
 */
/* -------------------------------------------------------------------------- */
extern retcode_t 
hci_KeyRefDestroy(hci_key_ref_t io_ref);


/* -------------------------------------------------------------------------- */
/** @brief Export the key in a reference to an external caller. It is assumed
 *         that the key will be protected by HCI implmentation before export.
 *         No protection is added in the Nautilus for export.
 *
 *         The HCI layer is also responsible to only allow the export of
 *         content keys.
 *
 * @param[in]  in_ref           Reference to the key to export
 * @param[out] out_key          Binary string holding the key data
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_KEYNOTINREF
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
#if defined( NPOPT_ENABLE_EXPORT_ACTION )
extern retcode_t
hci_ExportKey(hci_key_ref_t    in_ref,
              binstr_t       **out_key);
#endif

/* -------------------------------------------------------------------------- */
/**
 * @brief Get the ids for the personality keys held by device which are
 *        protected within the HCI component. These ids are used to detect if
 *        a octopus node or content key is encrypted with the device
 *        personality private sharing or secret key.
 *
 * @param[out] out_ids     Array of binary string containing the device key ids
 * @param[out] out_num_ids Number of ids in the out_ids array
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
extern retcode_t
hci_DevIdsGet(binstr_t          ***out_ids,
              u_int32_t           *out_num_ids);


/* -------------------------------------------------------------------------- */
/**
 * @brief Import a key that is encrypted with the supplied key reference. This
 *        function will decrypt the supplied key and create a reference for it.
 *        The new key reference can therefore be used in turn to decrypt other 
 *        (import) keys.
 *
 * @param[in]  in_ref       The reference to the key to decrypt the protected
 *                          key
 * @param[in]  in_enc_algo   Type of the decryption algorithm to use.
 * @param[in]  in_oaep_param Optional oaep data to use during the decrypt with
 *              @ref HCI_ENC_ALGO_RSA_OAEP
 * @param[in]  in_key_data   Binary string containing the encrypted key data
 * @param[in]  in_key_type   Type of the encrypted key
 * @param[out] out_ref       Output key reference to hold the decrypted key
 *
 * @note @ref HCI_ENC_ALGO_AES_CTR is not supported
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_DECRYPTIONFAILED
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
extern retcode_t
hci_ImportKey(hci_key_ref_t      in_ref,
              hci_enc_algo_t     in_enc_algo,
              binstr_t          *in_oaep_param,
              binstr_t          *in_key_data,
              hci_key_type_t     in_key_type,
              hci_key_ref_t     *out_ref);


/* -------------------------------------------------------------------------- */
/**
 * @brief Get the torpedo id for the device protected with the HCI interface.
 *        This is used when removing starfish protection of a content key.
 *
 * @param[out] out_id      Binary string containing the id
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
extern retcode_t
hci_StarfishIdGet(binstr_t **out_id);


/* -------------------------------------------------------------------------- */
/**
 * @brief Return the key data from a key that was previously exported to the 
 *        key reference if the key is protected with starfish. Otherwise NULL.
 *
 * @param[in]  in_ref   Reference to the key
 * @param[out] out_data Binary string containing the key data (starfish encrypted)
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_KEYNOTINREF
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
extern retcode_t 
hci_KeyDataGetIfStarfish(hci_key_ref_t      in_ref,
                         binstr_t         **out_data);


/* -------------------------------------------------------------------------- */
/**
 * @brief Decrypt the input encrypted key data using starfish and the device
 *        key set.
 * 
 * @param[in]  in_num_iterations number of hash iterations
 * @param[in]  in_dks_key_index  index of the key in the dks
 * @param[in]  in_enc_bk         encrypted broadcast key
 * @param[in]  in_bk_hash        hash of the plain broadcast key. Checked
 *                               after decrypting the broadcast key to ensure
 *                               it is correct
 * @param[in]  in_iv             IV used to the decryption of the content key
 * @param[in]  in_enc_ck         Encrypted content key to extract
 * @param[out] out_ref           Output key reference to hold the content key
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
extern retcode_t 
hci_ImportStarfishKey(u_int32_t       in_num_iterations,
                      u_int32_t       in_dks_key_index,
                      binstr_t       *in_enc_bk,
                      binstr_t       *in_bk_hash,
                      binstr_t       *in_iv,
                      binstr_t       *in_enc_ck,
                      hci_key_ref_t  *out_ref);


/* -------------------------------------------------------------------------- */
/**
 * @brief Sign the supplied data using the key in the supplied key reference.
 *
 * @param[in]  in_ref       Reference to the key to sign the data with
 *                          (support:
 *                              @ref HCI_KEY_TYPE_NPPR_SIG
 *                          )  
 * @param[in]  in_sig_algo  Type of the signature to generate
 * @param[in]  in_data      Data to generate the signature for
 * @param[out] out_sig_data Binary string holding the signature
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_INVALIDSIG
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
extern retcode_t 
hci_DataSign(hci_key_ref_t       in_ref,
             hci_sig_algo_t      in_sig_algo,
             binstr_t           *in_data,
             binstr_t          **out_sig_data);


/* -------------------------------------------------------------------------- */
/**
 * @brief Decrypt the supplied data using the key in the supplied key reference.
 *        The hci lock is checked and will block until available.
 *        It will only be possible to use this decrypt call with a NEMO
 *        message key or a content key.
 *
 * @param[in]  in_ref          Reference to the key to decrypt the data with
 * @param[in]  in_enc_algo     Type of the decryption algorithm to use
 * @param[in]  in_enc_pad      Type of padding used
 * @param[in]  in_salt_buf     Buffer holding salt value
 * @param[in]  in_oaep_param   Optional oaep data to use during the decrypt
 * @param[in]  in_data         Data to decrypt
 * @param[out] out_data        Binary string holding the decrypted data
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_KEYNOTINREF
 * @return ::ERR_HCI_DECRYPTIONFAILED
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
extern retcode_t 
hci_DataDecrypt(hci_key_ref_t     in_ref,
                hci_enc_algo_t    in_enc_algo,
                hci_enc_pad_t     in_enc_pad,
                u_int8_t*         in_salt_buf,
                binstr_t         *in_oaep_param,
                binstr_t         *in_data,
                binstr_t        **out_data);


/* -------------------------------------------------------------------------- */
/**
 * @brief Decrypt the supplied data using the key in the supplied key reference.
 *        This API can be called outside of a hci lock in order to prevent 
 *        content decryption calls being blocked by other crypto operations.
 *       
 *        It will only be possible to use this decrypt call with a content key.
 *
 * @param[in]  in_ref          Reference to the key to decrypt the data with
 * @param[in]  in_enc_algo     Type of the decryption algorithm to use
 * @param[in]  in_enc_pad      Type of padding used
 * @param[in]  in_salt_buf     Buffer holding salt value
 * @param[in]  in_oaep_param   Optional oaep data to use during the decrypt
 * @param[in]  in_data         Data to decrypt
 * @param[out] out_data        Binary string holding the decrypted data
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_KEYNOTINREF
 * @return ::ERR_HCI_DECRYPTIONFAILED
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
extern retcode_t
hci_StreamDecrypt(hci_key_ref_t     in_ref,
                hci_enc_algo_t    in_enc_algo,
                hci_enc_pad_t     in_enc_pad,
                u_int8_t*         in_salt_buf,
                binstr_t         *in_oaep_param,
                binstr_t         *in_data,
                binstr_t        **out_data);


/* -------------------------------------------------------------------------- */
/**
 * @brief Encrypt the key data in the supplied reference with the supplied
 *        public rsa key.
 *
 * @param[in]  in_ref         Reference to the key to encrypt with the supplied 
 *                            public key
 * @param[in]  in_pub_n       Modulus of the public key to encrypt the key data 
 *                            in the input reference
 * @param[in]  in_pub_e       Exponent of the public key to encrypt the key data
 *                            in the input reference 
 * @param[out] out_data       Binary string holding the encrypted data
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_KEYNOTINREF
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
extern retcode_t 
hci_KeyDataEncrypt(hci_key_ref_t     in_ref,
                   binstr_t         *in_pub_n,
                   binstr_t         *in_pub_e,
                   binstr_t        **out_data);


/* -------------------------------------------------------------------------- */
/**
 * @brief Calculate the HMAC on the supplied data using the key in keyref.
 *
 * @param[in]  in_ref           Reference to the key to calculate the hmac with
 * @param[in]  in_dig_algo      Digest algorithm to be used
 * @param[in]  in_data          Data to calculate the MAC over
 * @param[in]  in_expected_hmac Expected HMAC value
 * @param[out] out_result       Flag holding the check result. TRUE for a match
 *                              FALSE for a difference
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_KEYNOTINREF
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
extern retcode_t 
hci_HmacCheck(hci_key_ref_t   in_ref,
              hci_dig_algo_t  in_dig_algo,
              binstr_t       *in_data,
              binstr_t       *in_expected_hmac,
              u_int32_t      *out_result);


/* -------------------------------------------------------------------------- */
/**
 * @brief Calculate the hash on the supplied data concatenated with the key in
 *        reference, i.e. in_data || in_ref.keydata. If the in_data is NULL then
 *        the hash is just calcualted on in_ref.keydata .
 *
 * @param[in]  in_ref           Reference to the key to calculate the hash with
 * @param[in]  in_dig_algo      Digest algorithm to be used
 * @param[in]  in_data          Data to prepend to the key before hashing
 * @param[in]  in_expected_hash Expected hash value
 * @param[out] out_result       Flag holding the check result. TRUE for a match,
 *                              FALSE otherwise.
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_KEYNOTINREF
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
extern retcode_t 
hci_HashCheck(hci_key_ref_t   in_ref,
              hci_dig_algo_t  in_dig_algo,
              binstr_t       *in_data,
              binstr_t       *in_expected_hash,
              u_int32_t      *out_result);
   
/* -------------------------------------------------------------------------- */
/**
 * @brief Move the hci key data in a platform specific way to another location.
 *        The key reference becomes unavailable.
 *
 * @param[in,out]  io_ref   Reference to the key to move
 * @param[in]  in_pv        Platform data from the Application used when
 *                          moving the ley
 * @param[out] out_pv       Platform data returned after the key is moved
 *
 * @note This functionality is only supported for content keys.
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_HCI_KEYNOTINREF
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
extern retcode_t 
hci_MoveKey(hci_key_ref_t   io_ref,
            binstr_t       *in_pv,
            binstr_t      **out_pv);

/* -------------------------------------------------------------------------- */
/**
 * @brief This API will be used to load device personality private keys into
 *        the HCI component. If the device keys are loaded at factory time then
 *        they will be accessed directly from the hcicomponent.
 *        It is called when private keys are to be set into the device.
 *        It is the platform owners responsibility to protect the keys
 *        loaded into the hci component. If in_buf = NULL then the function should
 *        delete the Personality key.
 *
 * @param[in]  in_type          Personality key type (nemopriv or octpriv)
 * @param[in]  in_len           Size of the personality private key
 * @param[in]  in_buf           Personality key data
 *
 * @return ::ERR_NP_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
extern retcode_t 
hci_PersonalityKeyLoad(hci_pers_key_type_t in_type, 
                       np_size_t in_len, 
                       u_int8_t *in_buf);

/* -------------------------------------------------------------------------- */
/**
 * @brief Check whether the personality private keys are present.
 *
 * @param[in] in_type     Personality type
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_HCI_NOPERSONALITYREGISTERED
 * @return ::ERR_HCI_NOTLOCKED
 */
/* -------------------------------------------------------------------------- */
retcode_t
hci_PersonalityKeyCheck (hci_pers_key_type_t in_type);


/* -------------------------------------------------------------------------- */
/**
 * @brief Parse SAS to extract the content keys and content ids from it.
 *
 * Note: platform has to add the implementation to decrypt the SAS, if it is in encrypted form.
 *
 * @param[in]  in_sas                       sas info (encrypted/unencrypted)
 * @param[in]  in_application_data          opaque application data, maybe
 *                                          used to protect SAS
 * @param[out] out_public_sas               public sas info (excluding keys and content ids)
 * @param[out] out_sas_keys                 structure holds keycount,contentids and keyrefs
 *
 * @return ::ERR_OK
 * @return ::ERR_ILLEGALARGS
 * @return ::ERR_NOMEM
 * @return ::ERR_HCI_ILLEGALSAS
 */
/* -------------------------------------------------------------------------- */
extern retcode_t
hci_ImportSAS(binstr_t          *in_sas,
              binstr_t          *in_application_data,
              binstr_t         **out_public_sas,
              hci_sas_keys_t   **out_sas_keys);

/* -------------------------------------------------------------------------- */
/**
 * @brief Free SAS keys structure.
 *
 * @param[in,out]  io_sas_keys          sas keys structure to be freed
 *
 * @return ::ERR_OK
 */
/* -------------------------------------------------------------------------- */
extern retcode_t
hci_FreeSASKeys(hci_sas_keys_t    *io_sas_keys);

#ifdef __cplusplus
}
#endif

#endif                          /* HCI_API_H */
/** @} */


