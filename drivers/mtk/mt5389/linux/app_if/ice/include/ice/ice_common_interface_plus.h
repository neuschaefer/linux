/**
 * Copyright 2009 Cabot Communications.  All rights reserved.
 *
 * @file
 *
 * @brief Cabot portable interface to Common Interface Plus (CI+) licensed 
 * constant values (see CI+ Specification V1.2 (2009-04), Section 5.3 and the 
 * CI+ Licensee Specification).
 */

#ifndef CABOT_ICE_ICE_COMMON_INTERFACE_PLUS_H_INCLUDED
#define CABOT_ICE_ICE_COMMON_INTERFACE_PLUS_H_INCLUDED

#include "frost/frost_basictypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief License-constant credentials.
 *
 * This list comprises the set of credentials that must be burned into
 * a secure, read-only section of non-volatile storage during production
 * of the hardware unit. They must be protected from overwriting
 * post-production, see the CI+ Licensee Specification for more information.
 *
 * These credentials are described in the CI+ Specification V1.2 (2009-04), 
 * Section 5.3 and the CI+ Licensee Specification.
 *
 * All values must be provided as binary data, <b>NOT</b> as base-64 text or 
 * colon/space separated text. 
 */
typedef enum ICE_CIPlusLicensedConstant
{
    /**
     * @brief Root (CA) certificate in X.509 binary DER format
     *
     * CI+ Specification V1.2 (2009-04), Table 5.2 - Key: Root cert
     *
     * Certificates are variable length, usually more then 1024 bytes. It 
     * typically starts with the binary [0x30, 0x82...]
     */
    ice_ci_plus_licensed_constant_certificate_root = 0,
    
    /**
     * @brief Host brand certificate in X.509 binary DER format
     *
     * CI+ Specification V1.2 (2009-04), Table 5.2 - Key: Brand Cert
     *
     * Certificates are variable length, usually more then 1024 bytes. It 
     * typically starts with the binary [0x30, 0x82...]
     */
    ice_ci_plus_licensed_constant_certificate_brand,
    
    /**
     * @brief Host device certificate in X.509 binary DER format
     *
     * CI+ Specification V1.2 (2009-04), Table 5.2 - Key: Device Cert
     *
     * Certificates are variable length, usually more then 1024 bytes. It 
     * typically starts with the binary [0x30, 0x82...]
     */
    ice_ci_plus_licensed_constant_certificate_device,
               
    /**
     * @brief Host device private key value in binary DER format (LLP HDQ) 
     *
     * CI+ Specification V1.2 (2009-04), Table 5.2 - Key: HDQ
     *
     * RSA Private Key is variable length, usually more then 1024 bytes. It 
     * typically starts with the binary [0x30, 0x82...]
     */
    ice_ci_plus_licensed_constant_device_private_key,

    /**
     * @brief Pseudo Random Number Generator Seed (LLP prng_key_k)
     *
     * CI+ Specification V1.2 (2009-04), Table 5.2 - Key: prng_seed
     *
     * Usually 16 bytes (128 bits)
     */
    ice_ci_plus_licensed_constant_prng_seed,
    
    /**
     * @brief Diffie-Hellman p value
     *
     * CI+ Specification V1.2 (2009-04), Table 5.2 - Key: DH_p
     *
     * Usually 256 bytes (2048 bits)
     */
    ice_ci_plus_licensed_constant_dh_p,
    
    /**
     * @brief Diffie-Hellman g value
     *
     * CI+ Specification V1.2 (2009-04), Table 5.2 - Key: DH_g
     *
     * Usually 256 bytes (2048 bits)
     */
    ice_ci_plus_licensed_constant_dh_g,
    
    /**
     * @brief Diffie-Hellman q value
     *
     * CI+ Specification V1.2 (2009-04), Table 5.2 - Key: DH_q
     *
     * Usually 32 bytes (256 bits)
     */
    ice_ci_plus_licensed_constant_dh_q,

    /**
     * @brief f-sac() function Key (LLP SLK)
     *
     * Usually 16 bytes (128 bits)
     */
    ice_ci_plus_licensed_constant_slk,
    
    /**
     * @brief f-cc() function Key (LLP CLK)
     *
     * Usually 16 bytes (128 bits)
     */
    ice_ci_plus_licensed_constant_clk,
    
    /**
     * @brief SAC Initialisation Vector (LLP SIV)
     *
     * CI+ Specification V1.2 (2009-04), Table 5.2 - Key: SIV
     *
     * Usually 16 bytes (128 bits)
     */
    ice_ci_plus_licensed_constant_siv

} ICE_CIPlusLicensedConstant;

/**
 * @brief Query the size of a secure, read-only storage area.
 *
 * A predefined content identifier \a licensed_constant is used to identify 
 * specific content.
 *
 * That content stored for \a licensed_constant must not be retrieved by this 
 * function.
 *
 * @param licensed_constant Unique identifier for this content.
 *
 * @return Length of the content in bytes.
 *
 * @remark Implemented by receiver stack.
 */
frost_uint ICE_CIPlusLicensedConstantGetSize(
    ICE_CIPlusLicensedConstant licensed_constant);

/**
 * @brief Read client data from a secure, read-only storage area.
 *
 * The read uses a predefined content identifier to identify specific content.
 * The implementor must ensure that this content is stored securely on the
 * hardware unit during production, such that it cannot be easily deciphered 
 * and cannot be overwritten.
 *
 * The data that is retrieved in this function should not be held in a temporary 
 * buffer in memory, it should be read directly into the supplied \a buffer. If
 * this is not possible, any temporary buffers used must be securely overwritten 
 * once the memory is no longer required (which must happen before this function 
 * returns).
 *
 * #ICE_CIPlusLicensedConstantGetSize will always be called prior to this 
 * function to obtain the \a length.
 *
 * This function will never be called with a \a length greater than the size 
 * returned by a call to #ICE_CIPlusLicensedConstantGetSize with the same 
 * \a licensed_constant value.
 *
 * @param licensed_constant Unique identifier for this content.
 *
 * @param length The number of bytes to be copied from storage into \a buffer.
 *
 * @param buffer A buffer of at least \a length bytes in size, owned by the 
 * caller, into which the content will be retrieved. If \a length is zero then 
 * this parameter is ignored.
 *
 * @return #frost_true if either (a) \a length bytes were successfully 
 * retrieved; or (b) \a length is zero and \a licensed_constant refers to valid 
 * content.
 *
 * @remark Implemented by receiver stack.
 */
frost_bool ICE_CIPlusLicensedConstantRead(
                        ICE_CIPlusLicensedConstant licensed_constant,
                        frost_uint                 length,
                        frost_uint8 *              buffer);

#ifdef __cplusplus
}
#endif

#endif /* CABOT_ICE_ICE_COMMON_INTERFACE_PLUS_H_INCLUDED*/
