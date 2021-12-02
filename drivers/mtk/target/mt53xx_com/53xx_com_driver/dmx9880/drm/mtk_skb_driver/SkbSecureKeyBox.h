/*****************************************************************
|
|   Secure Key Box API
|
|   $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/project_x/target/mt53xx_com/53xx_com_driver/dmx9880/drm/mtk_skb_driver/SkbSecureKeyBox.h#1 $
|   Original author:  Gilles Boccon-Gibod
|
|   This software is provided to you pursuant to your agreement 
|   with Intertrust Technologies Corporation ("Intertrust"). 
|   This software may be used only in accordance with the terms 
|   of the agreement.
|      
|   Copyright (c) 2005-2011 by Intertrust. All rights reserved. 
|
****************************************************************/
/** 
* @file
* @brief Secure Key Box
*/

/** @mainpage SKB API
 *
 * @section intro_sec Introduction
 * 
 * The SKB (Secure Key Box) API provides a simple interface to a code 
 * module responsible for the handling of secret data, such as 
 * cryptographic keys. It enables the use of secret data without exposing 
 * this sensitive information to the calling application.
 * 
 * The processing environment for a device is considered to be divided 
 * into two domains: a secure domain in which all handling of secrets 
 * is done, and an open domain. The SKB API is the boundary between those 
 * domains. 
 * 
 * Code executing in the open domain (for example, an application running
 * on a general-purpose CPU) is often vulnerable to attacks, so it is risky 
 * for it to access secret data in the clear. With the SKB API, such code 
 * never needs to directly access secret data. Instead, the SKB API 
 * implementation, executing in a security processor or otherwise secure 
 * environment, can be responsible for all secrets. Code in the open domain 
 * can simply invoke SKB API interfaces to ask the code executing in the 
 * secure domain to manage secret data and perform operations on it.
 * 
 * @section algorithms Cryptographic Algorithms
 *
 * The SKB API exposes common cryptographic methods to encrypt/decrypt, 
 * sign/verify, and digest data (possibly including secret data). The SKB 
 * implementation supports the following cryptographic algorithms:
 * 
 *    - RSA 1.5 encryption/decryption, sign/verify
 *    - RSA OAEP encryption/decryption
 *    - AES 128-bit encryption/decryption in ECB, CBC, or CTR mode
 *    - SHA-1, and SHA-256 digests
 *    - HMAC signatures
 * 
 * @section other_operations Other Operations
 * 
 * The API also provides a mechanism to unwrap a cryptographically wrapped 
 * key (a key encrypted with another key) and then to use the key in 
 * cryptographic operations.
 * 
 * Applications can also ask the API to export secret data, that is, 
 * provide a protected form of the secret data that the caller can store 
 * in non-secure persistent storage. When needed later, the data can be 
 * returned to its original form (in the secret domain) so that it can 
 * be used in further operations.
 * 
 * @section getting_started Getting Started
 * 
 * The interface to the SKB is a C interface, composed of a number of object 
 * classes. Even though the interface is an ANSI C interface, it adopts an 
 * object-oriented style. The header file declares object classes. An object 
 * class defines the functional interface to the class (a set of methods). 
 * Each method of a class interface is a function whose first argument is a 
 * reference to an instance of the same class. The data type that represents 
 * references to object instances is a pointer to an opaque C struct. It may 
 * be considered as analogous to a pointer to a C++ object. 
 * 
 * @section example Example
 * 
 * A concrete example is that for the class named SKB_Cipher, the data type 
 * SKB_Cipher is the name of a C struct. The function name for one of the 
 * methods of SKB_Cipher is SKB_Cipher_ProcessBuffer(), and the function 
 * takes an SKB_Cipher* as its first parameter. 
 * 
 * @section instances Obtaining Class Instances
 * 
 * An instance of a class is obtained by declaring a pointer to an object 
 * for the class and passing the address of that pointer to a particular 
 * function. The function creates the instance and sets the pointer to refer 
 * to it.
 * 
 * For example, the first object that you need to create when you are going 
 * to use the SKB API is an SKB_Engine. An SKB_Engine object represents an 
 * instance of an engine that can manage and operate on secret data. An 
 * SKB_Engine instance is obtained by calling SKB_Engine_GetInstance(), which 
 * is declared as follows:
 *
 * <pre>
 *   SKB_Result SKB_Engine_GetInstance(SKB_Engine** engine);</pre>
 * As you can see, the parameter is the address of an SKB_Engine pointer. 
 * This  method creates an SKB_Engine instance, and sets the pointer to refer to 
 * the new instance. Here is a sample call:
 * <pre>
 *   SKB_Engine* engine = NULL;
 *   SKB_Result result;
 *   result = SKB_Engine_GetInstance(&engine);</pre>
 * @section method_calls Making Method Calls
 * 
 * In the C object-oriented style for the API, a call to a method of a 
 * particular instance is done by calling a function and passing a pointer to 
 * the instance as the first parameter.
 * 
 * For example, once an SKB_Engine instance is created, as shown in the previous 
 * section, all the SKB_Engine methods can be called to operate on that instance. 
 * One such method is the SKB_Engine_GetInfo() method, which is used to obtain 
 * information about the engine (version numbers, properties, and so on). This 
 * method is declared as follows:
 * 
 * <pre>
 *     SKB_Result SKB_Engine_GetInfo(const SKB_Engine* self, SKB_EngineInfo* info);</pre>
 * 
 * This method stores the engine information in the SKB_EngineInfo structure 
 * pointed to by the info parameter. Assuming engine is the pointer previously 
 * set by SKB_Engine_GetInstance() to refer to the SKB_Engine instance it created, 
 * SKB_Engine_GetInfo() can be invoked by the following:
 * 
 * <pre>
 *     SKB_Result result;
 *     SKB_EngineInfo engineInfo;
 *     result = SKB_Engine_GetInfo(engine, &engineInfo);</pre>
 * 
 * @section header_file Header File
 * 
 * The file SkbSecureKeyBox.h contains the entire SKB API interface. 
 * 
 * @section classes Main Classes
 * 
 * As mentioned above, the first object you should instantiate is an SKB_Engine. 
 * All other class instances are instantiated by calling SKB_Engine methods.
 * 
 * The primary abstractions exposed by the API are the SKB_SecureData, 
 * SKB_Transform, and SKB_Cipher classes. 
 * 
 * The SKB_SecureData object represents secret data. It allows an application 
 * to refer to and operate on data managed by the SKB. Some SKB_SecureData 
 * objects are named, so that a calling application can locate a specific 
 * secret within the SKB. For example, each device is considered to have a 
 * well-guarded secret device key. It is a key that is sufficiently protected 
 * so as to act as a root of trust used to enable the secure marshaling of 
 * other keys and credentials into the Secure Key Box. An application can 
 * ask the SKB_Engine to create an SKB_SecureData object referencing the 
 * device key. It asks for this key by name (TBD).
 * 
 * An SKB_SecureData object is created for each (usually secret) data item 
 * to be operated on by the SKB. To create an SKB_SecureData object, call an
 * appropriate SKB_Engine method. For example, SKB_Engine_CreateDataFromWrapped() 
 * creates an SKB_SecureData object representing the data resulting from 
 * unwrapping (decrypting) specified wrapped data.
 * 
 * An SKB_Transform is used to perform operations on data, such as digest 
 * calculations. It returns the output to the caller. Due to the nature of 
 * the transforms available, the output does not expose any secret data. 
 * You create an SKB_Transform by calling the SKB_Engine_CreateTransform() method.
 * 
 * An SKB_Cipher object encapsulates the attributes and parameters necessary to 
 * perform cryptographic operations on SKB_SecureData objects. You create an 
 * SKB_Cipher by calling the SKB_Engine_CreateCipher() method.
 * 
 * @section return_values Method Return Values
 * 
 * As you can see from the examples above, most SKB API methods return an 
 * SKB_Result. 
 * 
 * An SKB_Result is an int, and its possible values are defined in 
 * SkbSecureKeyBox.h. When a method call succeeds, the return value is 
 * SKB_SUCCESS. Otherwise, it is a negative number.
 *  * 
 * @section conventions Conventions for Functions Returning Output in Variable-length Buffer
 * 
 * A number of the functions defined in the SKB API return a variable amount of output 
 * in a buffer provided by the caller. The output is returned in a variable-length 
 * application-supplied buffer. An example of a function of this sort is 
 * SKB_Transform_GetOutput().
 * 
 * These functions have some common calling conventions, which we describe here. Two of 
 * the arguments to the function are a pointer to the output buffer (say 'output') 
 * and a pointer to a location which will hold the length of the output produced 
 * (say 'output_size'). There are two ways for an application to call such a function:
 * 
 *         - If output is NULL, then all that the function does is return (in *output_size)
 * a number of bytes which would suffice to hold the output produced by the function. 
 * This number may somewhat exceed the precise number of bytes needed, but should not 
 * exceed it by a large amount. SKB_SUCCESS is returned by the function. 
 * 
 *         - If output is not NULL, then *output_size must contain the size in bytes 
 * of the buffer pointed to by 'output'. If that buffer is large enough to hold 
 * the output produced by the function, then that output is placed there, and 
 * SKB_SUCCESS is returned by the function. If the buffer is not large enough, 
 * then SKB_ERROR_BUFFER_TOO_SMALL is returned. In either case, *output_size is 
 * set to hold the exact number of bytes of output produced by the function.
 * 
 * @section thread_model Thread Model
 * 
 * An engine (SKB_Engine_GetInstance) and any object or data obtained directly or 
 * indirectly from this engine may only be accessed from the same thread 
 * as the one on which the engine was obtained. Multiple threads can each 
 * use different engines, but engines cannot be passed or shared between 
 * threads. If an instance of an SKB_SecureData object needs to be transfered 
 * from one engine to another, the SKB_SecureData_Export method can be used 
 * with the 'target 'parameter set to SKB_EXPORT_TARGET_CROSS_ENGINE, and 
 * the exported buffer can be loaded in a different engine by calling 
 * SKB_Engine_CreateDataFromExported().
 */ 
 
#ifndef _SKB_SECURE_KEY_BOX_H_
#define _SKB_SECURE_KEY_BOX_H_

/*----------------------------------------------------------------------
|   constants
+---------------------------------------------------------------------*/
/* Current SKB engine version numbers. */
#define SKB_API_VERSION_MAJOR    1
#define SKB_API_VERSION_MINOR    0
#define SKB_API_VERSION_REVISION 0
#define SKB_API_VERSION_STRING   "1.0.0"

/* SKB_Result values */
#define SKB_SUCCESS 0
#define SKB_FAILURE (-1)
#define SKB_ERROR_BASE               (-80000)
#define SKB_ERROR_INTERNAL           ((SKB_ERROR_BASE) - 1)
#define SKB_ERROR_INVALID_PARAMETERS ((SKB_ERROR_BASE) - 2)
#define SKB_ERROR_NOT_SUPPORTED      ((SKB_ERROR_BASE) - 3)
#define SKB_ERROR_OUT_OF_RESOURCES   ((SKB_ERROR_BASE) - 4)
#define SKB_ERROR_BUFFER_TOO_SMALL   ((SKB_ERROR_BASE) - 5)
#define SKB_ERROR_INVALID_FORMAT     ((SKB_ERROR_BASE) - 6)
#define SKB_ERROR_ILLEGAL_OPERATION  ((SKB_ERROR_BASE) - 7)
#define SKB_ERROR_INVALID_STATE      ((SKB_ERROR_BASE) - 8)
#define SKB_ERROR_OUT_OF_RANGE       ((SKB_ERROR_BASE) - 9)

/* Values for the 'flags' parameter of SKB_Engine_CreateCipher. */
/** 
 * Set when the cipher is intended to be used with high throughput 
 * (for example, media content decryption) 
 */
#define SKB_CIPHER_FLAG_HIGH_SPEED 1 

/*----------------------------------------------------------------------
|   objects types
+---------------------------------------------------------------------*/
/** @defgroup SKB_SecureData SKB_SecureData Class
 * @{
 */

/**
 * An SKB_SecureData object represents secret data that cannot be accessed
 * directly by the caller. Secret data is typed; it can represent cipher
 * keys or arbitrary byte sequences.
 */
typedef struct SKB_SecureData SKB_SecureData;

/** @} */

/** @defgroup SKB_Transform SKB_Transform Class
 * @{
 */

/**
 * An SKB_Transform object represents a data transform. The purpose of such a
 * transform is to be able to transform data supplied by the caller, as well as
 * secret data, and return to the caller the output of the transform, which,
 * due to the nature of the transforms available (such as digests), does not 
 * reveal any of the secret data used as input to the transform.
 */
typedef struct SKB_Transform SKB_Transform;

/** @} */

/** @defgroup SKB_Cipher SKB_Cipher Class
 * @{
 */
 
/**
 * An SKB_Cipher object can encrypt or decrypt data supplied by the caller.
 */
typedef struct SKB_Cipher SKB_Cipher;

/** @} */

/** @defgroup SKB_Engine SKB_Engine Class
 * @{
 */

/**
 * An SKB_Engine object represents an instance of an engine that
 * can manage and operate on secret data that cannot be accessed
 * by the caller.
 */
typedef struct SKB_Engine SKB_Engine;

/** @} */

/*----------------------------------------------------------------------
|   other types and enums
+---------------------------------------------------------------------*/
/* Basic data types. */
typedef int SKB_Result;
typedef unsigned char SKB_Byte;
typedef unsigned int SKB_Size;

/* Engine property. */
typedef struct {
    const char* name;
    const char* value;
} SKB_EngineProperty;

/* Information about an SKB_Engine. */
typedef struct {
    struct {
        unsigned int major;
        unsigned int minor;
        unsigned int revision;
    } api_version;
    unsigned int        flags;
    unsigned int        property_count;
    SKB_EngineProperty* properties; /**< array of properties */
} SKB_EngineInfo;

/* Possible types of data in the value encapsulated by an SKB_SecureData object. */
typedef enum {
    SKB_DATA_TYPE_BYTES, /* can be used as a symmetric key */
    SKB_DATA_TYPE_RSA_PRIVATE_KEY
} SKB_DataType;

/* Information about an SKB_SecureData. */
typedef struct {
    SKB_DataType type;   /** Data type  */
    SKB_Size     size;   /** Data size, in bytes (or 0 if not available). 
                             For data of type SKB_DATA_TYPE_RSA_PRIVATE_KEY,
                             this value is the modulus (in bytes) */
} SKB_DataInfo;

/* Digest algorithm types. */
typedef enum {
    SKB_DIGEST_ALGORITHM_SHA1,
    SKB_DIGEST_ALGORITHM_SHA256
} SKB_DigestAlgorithm;

/* Cipher algorithm types. */
typedef enum {
    SKB_CIPHER_ALGORITHM_NULL,
    SKB_CIPHER_ALGORITHM_AES_128_ECB,
    SKB_CIPHER_ALGORITHM_AES_128_CBC, /* with xmlenc padding for unwrapping: http://www.w3.org/TR/xmlenc-core/#sec-Alg-Block  */
    SKB_CIPHER_ALGORITHM_AES_128_CTR, /* not supported for unwrapping */
    SKB_CIPHER_ALGORITHM_RSA_1_5,
    SKB_CIPHER_ALGORITHM_RSA_OAEP
} SKB_CipherAlgorithm;

/* Cipher parameters */
typedef struct {
    /**
     * size in bytes of the counter (4, 8, 16) 
     * This value is purely indicative, and can be used as 
     * an optimization hint: it indicates that the counter 
     * values will be such that only the rightmost counter_size
     * bytes in the IV will ever change, the others won't.
     * An implementation may ignore that parameter.
     */
    SKB_Size counter_size; 
} SKB_CtrModeCipherParameters;

/* Signature algorithm types. */
typedef enum {
    SKB_SIGNATURE_ALGORITHM_HMAC_SHA1,
    SKB_SIGNATURE_ALGORITHM_HMAC_SHA256,
    SKB_SIGNATURE_ALGORITHM_RSA,         /**< PKCS #1 block type 2. */
    SKB_SIGNATURE_ALGORITHM_RSA_SHA1,
    SKB_SIGNATURE_ALGORITHM_RSA_SHA256
} SKB_SignatureAlgorithm;

/* Derivation Algorithms */
typedef enum {
    SKB_DERIVATION_ALGORITHM_SLICE,      /* data type must be SKB_DATA_TYPE_BYTES */
    SKB_DERIVATION_ALGORITHM_SHA_1       /* data type must be SKB_DATA_TYPE_BYTES */
} SKB_DerivationAlgorithm;

/* Indication as to whether an SKB_Cipher can be used for encryption or decryption. */
typedef enum {
    SKB_CIPHER_DIRECTION_ENCRYPT,
    SKB_CIPHER_DIRECTION_DECRYPT
} SKB_CipherDirection;

/* Data formats. */
typedef enum {
    SKB_DATA_FORMAT_RAW,
    SKB_DATA_FORMAT_PKCS8
} SKB_DataFormat;

/* Different types of transforms. */
typedef enum {
    /**< Compute a message digest. The transform parameter is a pointer to an SKB_DigestTransformParameters */
    SKB_TRANSFORM_TYPE_DIGEST, 

    /**< Compute a signature. The transform parameter is a pointer to an SKB_SignTransformParameters */
    SKB_TRANSFORM_TYPE_SIGN,   

    /**
     * Verify a signature. The transform parameter is a pointer to an SKB_VerifyTransformParameters
     * The output of this transform is a single byte with the value 1 if the signature is verified, 
     * or 0 if it is not 
     */
    SKB_TRANSFORM_TYPE_VERIFY   
} SKB_TransformType;

/* Structure defining parameters for a transform of type SKB_TRANSFORM_TYPE_DIGEST. */
typedef struct {
    SKB_DigestAlgorithm algorithm;
} SKB_DigestTransformParameters;

/* Structure defining parameters for a transform of type SKB_TRANSFORM_TYPE_SIGN. */
typedef struct {
    SKB_SignatureAlgorithm  algorithm; /**< Signature algorithm. */
    const SKB_SecureData*   key;       /**< Signature key. This key must not be released before the transform object that uses it is released */
} SKB_SignTransformParameters;

/* Structure defining parameters for a transform of type SKB_TRANSFORM_TYPE_VERIFY. */
typedef struct {
    SKB_SignatureAlgorithm  algorithm;      /**< Signature algorithm. */
    const SKB_SecureData*   key;            /**< Signature key. This key must not be released before the transform object that uses it is released */
    const SKB_Byte*         signature;      /**< Signature to verify the data against */
    SKB_Size                signature_size; /**< Size of the signature */
} SKB_VerifyTransformParameters;

/* Structure defining parameters for a derivation of type SKB_DERIVATION_ALGORITHM_SHA_1.
 * If this parameter is NULL in the SKB_SecureData_Derive method, the default values
 * are 1 for round_count and 20 (size of SHA-1 digest) for output_size */
typedef struct {
    unsigned int round_count;
    unsigned int output_size;
} SKB_Sha1DerivationParameters;

/* Structure defining parameters for a derivation of type SKB_DERIVATION_ALGORITHM_SLICE */
typedef struct {
    unsigned int first;
    unsigned int size;
} SKB_SliceDerivationParameters;

/* Different types of possible targets for a secure data export (see 
    SKB_SecureData_Export). */
typedef enum {
    /**< Export in cleartext form. No parameters.                                                     */
    /* NOTE: This type of export is normally only used in 'debug' or 'test'                           */
    /* implementations of SKB, and would be dissallowed in 'release' or 'production' implementations. */ 
    SKB_EXPORT_TARGET_CLEARTEXT,   

    /**< Export in a persistent form that can be reloaded after a reboot/reset. No parameters. */
    SKB_EXPORT_TARGET_PERSISTENT,   

    /**< Export in a form that can be loaded in a different engine, but not across reboot/reset. No parameters. */
    SKB_EXPORT_TARGET_CROSS_ENGINE, 

    /**< Export to a custom form. The 'target_parameters' must point to an SKB_ExportCustomParameters structure. */
    SKB_EXPORT_TARGET_CUSTOM        
} SKB_ExportTarget;

/* Custom parameters for an export with target SKB_EXPORT_TARGET_CUSTOM */
typedef struct {
    const char* uid;     /**< Unique identifier for the export format */
    const void* options; /**< Optional pointer to options for the export format */
} SKB_ExportCustomParameters;

/*----------------------------------------------------------------------
|   macros
+---------------------------------------------------------------------*/
#define SKB_SUCCEEDED(_result) ((_result)==SKB_SUCCESS)
#define SKB_FAILED(_result)    ((_result)!=SKB_SUCCESS)

/*----------------------------------------------------------------------
|   interfaces
+---------------------------------------------------------------------*/
#if defined(__cplusplus)
extern "C" {
#endif

/** @ingroup SKB_Engine
 * @{
 */

/**
 * Obtains an engine instance.
 * This instance must be released by calling SKB_Engine_Release when no
 * longer needed.
 * 
 * @param engine Address of an SKB_Engine pointer that will be set to
 * point to the newly created SKB_Engine object.
 */
SKB_Result
SKB_Engine_GetInstance(SKB_Engine** engine);

/**
 * Releases the specified SKB_Engine object. 
 * An SKB_Engine object must be released after it is no longer needed,
 * by calling this method.
 * The object can no longer be used by the caller after this call returns.
 * All objects returned by this engine, such as SKB_SecureData,
 * SKB_Cipher, and SKB_Transform objects, must be released before
 * calling this method.
 * 
 * @param self The SKB_Engine to release.
 */
SKB_Result
SKB_Engine_Release(SKB_Engine* self);

/**
 * Obtains information (version numbers, properties, and so on) about the engine.
 * 
 * @param self The SKB_Engine whose information is obtained.
 * @param info Pointer to an SKB_EngineInfo structure that will be populated
 * with the engine information.
 */
SKB_Result
SKB_Engine_GetInfo(const SKB_Engine* self, SKB_EngineInfo* info);

/**
 * Obtains an SKB_SecureData object representing 
 * particular secret data that is specified by name.
 * Specific names to be used to reference certain data objects will be
 * documented in these comments.
 * 
 * @param self The SKB_Engine that will create an SKB_SecureData representing
 * the specified data.
 * @param name The name of the data object to obtain.
 * @param data Address of an SKB_SecureData pointer that will be set to point
 * to a new SKB_SecureData object representing the specified secret data.
 * This object must be released by calling SKB_SecureData_Release when no 
 * longer needed. 
 *
 * @return SKB_SUCCESS if the data object was found, SKB_ERROR_NO_SUCH_ITEM if
 * no such data object exists, or another error code if the call could not be
 * completed successfully.
 */
SKB_Result
SKB_Engine_GetDataByName(SKB_Engine* self, const char* name, SKB_SecureData** data);

/**
 * Creates an SKB_SecureData object representing the bytes obtained by "unwrapping"
 * (decrypting) a specified sequence of "wrapped" bytes, or an SKB_SecureData object
 * simply representing the specified bytes, if they are not wrapped.
 * Wrapped data is data that has been encrypted. Frequently, it is a key that has been
 * encrypted using another key. 
 * You pass this method the bytes, and information about the data format and 
 * what type of data the bytes represent.
 * If the 'wrapped' parameter contains bytes that were previously wrapped, you also
 * pass a pointer to an SKB_SecureData representing the key that was used to encrypt
 * the data, and information about the cipher algorithm used, as well as any
 * parameters used by the algorithm.
 * If 'wrapped' contains bytes that are NOT wrapped,
 * wrapping_algorithm should be SKB_CIPHER_ALGORITHM_NULL, and 
 * wrapping_parameters and unwrapping_key should both be NULL.
 * 
 * @param self The SKB_Engine responsible for creating the SKB_SecureData.
 * @param wrapped The bytes to be optionally unwrapped (if they were previously 
 *   wrapped, as determined by the wrapping_algorithm value) and then represented by
 *   a new SKB_SecureData.
 * @param wrapped_size The number of bytes in the 'wrapped' parameter.
 * @param wrapped_type The data type to be used for the SKB_SecureData created by
 *   this method.
 * @param wrapped_format The data format to be used for the SKB_SecureData created by
 *   this method.
 * @param wrapping_algorithm The cryptographic algorithm that was used to encrypt
 *   the wrapped bytes (SKB_CIPHER_ALGORITHM_NULL if the bytes are not encrypted).
 * @param wrapping_parameters Parameters for the algorithm (NULL if the bytes are
 *   not encrypted).
 * @param unwrapping_key Pointer to an SKB_SecureData object representing the key 
 *   needed to decrypt the data (NULL if the bytes are not encrypted).
 * @param data Address of an SKB_SecureData pointer that will be set to point
 *   to a new SKB_SecureData object representing either the bytes specified in
 *   the 'wrapped' parameter (if they are not wrapped) or the result of unwrapping 
 *   the specified wrapped bytes. This object must be released by calling 
 *   SKB_SecureData_Release when no longer needed.
 */
SKB_Result
SKB_Engine_CreateDataFromWrapped(SKB_Engine*           self, 
                                 const SKB_Byte*       wrapped,
                                 SKB_Size              wrapped_size,
                                 SKB_DataType          wrapped_type,
                                 SKB_DataFormat        wrapped_format,
                                 SKB_CipherAlgorithm   wrapping_algorithm,
                                 const void*           wrapping_parameters,
                                 const SKB_SecureData* unwrapping_key,
                                 SKB_SecureData**      data);

/**
 * Creates an SKB_SecureData object representing the bytes obtained by "importing"
 * specified previously "exported" secret data. Secret data is exported (see
 * SKB_SecureData_Export) when a client wants to store it for future use. Clients
 * are not allowed to obtain the secret data bytes in the clear, so exporting
 * the bytes results in the client receiving a serialized and encrypted form
 * of the secret data. Later, when the client wants operations to be done
 * on the secret data, it calls this SKB_Engine_CreateDataFromExported method to
 * decrypt and deserialize the exported data and create an SKB_SecureData 
 * representing it.
 *  
 * @param self The SKB_Engine responsible for creating the SKB_SecureData.
 * @param exported The previously exported bytes.
 * @param exported_size The number of bytes in the 'exported' parameter.
 * @param data Address of an SKB_SecureData pointer that will be set to point
 *   to a new SKB_SecureData object representing the result of decrypting and
 *   deserializing the exported bytes. This object must be released by 
 *   calling SKB_SecureData_Release when no longer needed.
 */
SKB_Result
SKB_Engine_CreateDataFromExported(SKB_Engine*      self, 
                                  const SKB_Byte*  exported, 
                                  SKB_Size         exported_size,
                                  SKB_SecureData** data);

    
/**
 * Creates an SKB_Transform object with the specified type and parameters.
 * 
 * @param self The SKB_Engine responsible for creating the SKB_Transform.
 * @param transform_type The type of transform to create.
 * @param transform_parameters Any parameters required by the transform,
 *   or NULL if the specified type of transform does not
 *   require any parameters.
 * @param transform Address of an SKB_Transform pointer that will be set
 *   to point to the newly created SKB_Transform object.
 *   This object must be released by calling SKB_Transform_Release when no 
 *   longer needed.
 */
SKB_Result  
SKB_Engine_CreateTransform(SKB_Engine*       self, 
                           SKB_TransformType transform_type, 
                           const void*       transform_parameters,
                           SKB_Transform**   transform);

/**
 * Creates an SKB_Cipher object with the specified characteristics.
 * 
 * @param self The SKB_Engine responsible for creating the SKB_Cipher.
 * @param cipher_algorithm The algorithm to be used by this cipher.
 * @param cipher_direction An indication as to whether the cipher 
 *   will be used for encryption or decryption.
 * @param cipher_flags Flags indicating any special cipher characteristics.
 * @param cipher_parameters Any parameters required by the cipher.
 *   For ciphers in CTR mode, this must point to a SKB_CtrModeCipherParameters
 *   parameters structure, or NULL for the default counter size (16). 
 *   For other ciphers, this parameter must be NULL.
 * @param cipher_key The cipher key.
 * @param cipher Address of an SKB_Cipher pointer that will be set to
 *   point to the newly created SKB_Cipher object with the specified
 *   characteristics. This object must be released by calling 
 *   SKB_Cipher_Release when no longer needed.
 */
SKB_Result  
SKB_Engine_CreateCipher(SKB_Engine*           self, 
                        SKB_CipherAlgorithm   cipher_algorithm,
                        SKB_CipherDirection   cipher_direction, 
                        unsigned int          cipher_flags,
                        const void*           cipher_parameters,
                        const SKB_SecureData* cipher_key,
                        SKB_Cipher**          cipher);

/** @} */

/** @ingroup SKB_SecureData
 * @{
 */

/**
 * Releases the specified SKB_SecureData object. 
 * An SKB_SecureData object must be released when it is no longer needed,
 * by calling this method.
 * The object can no longer be used by the caller after this call returns.
 * 
 * @param self The SKB_SecureData to release.
 */
SKB_Result
SKB_SecureData_Release(SKB_SecureData* self);

/**
 * Gets information about the data represented by an SKB_SecureData object.
 * 
 * @param self The SKB_SecureData whose information is obtained.
 * @param info Address of a pointer to an SKB_DataInfo structure that will be
 * populated with the requested information.
 */
SKB_Result
SKB_SecureData_GetInfo(const SKB_SecureData* self, SKB_DataInfo* info);

/**
 * Exports the secret data bytes represented by a specified SKB_SecureData
 * object. This method serializes and encrypts the data bytes
 * such that the caller will be returned a protected form of the bytes
 * that can later be reloaded into an SKB_Engine, that is,
 * converted back to their original form and represented
 * by an SKB_SecureData object, using the SKB_Engine_CreateDataFromExported
 * method. 
 * 
 * An example of what this enables is storage by the caller of the protected
 * (exported) form for later use.
 * 
 * If the output buffer you pass to this method is not large enough to hold
 * the exported data, this method sets *buffer_size to the number of bytes
 * required, and returns SKB_ERROR_BUFFER_TOO_SMALL. If you like, you can find 
 * out in advance the maximum size the output buffer should be by 
 * first calling this method with NULL for the 'buffer' parameter. In that case,
 * the method outputs, in *buffer_size, the maximum number of bytes required,
 * and returns SKB_SUCCESS. 
 * 
 * @param self The SKB_SecureData whose exported serialized payload is obtained.
 * @param target The target of the export. If the target is
 *   SKB_EXPORT_TARGET_PERSISTENT, the exported data can be reloaded in an engine 
 *   even after a complete reset/reboot of the CPU or system hosting the engine.
 *   If the target is SKB_EXPORT_TARGET_CROSS_ENGINE,
 *   the data can be reloaded in the same or a different SKB_Engine, as long as 
 *   a reboot/reset has not occurred, and
 *   the engine from which the data was exported and the engine into which it
 *   is loaded are considered to be compatible. (They can share some common state, 
 *   which typically does not persist across a CPU or security processor 
 *   shutdown or reset.) If the target is SKB_EXPORT_TARGET_CUSTOM, the export
 *   is customized according to the specified target_parameters.
 * @param target_parameters Parameters for a custom export, or NULL if
 *   target is not SKB_EXPORT_TARGET_CUSTOM.
 * @param buffer Memory buffer where the exported data is to be written, or
 *   NULL if you just want the method to return, in *buffer_size, a number of
 *   bytes sufficient to hold the exported data. The memory buffer, if
 *   supplied, must be large enough to hold the number of bytes specified
 *   by the buffer_size parameter.
 * @param buffer_size Pointer to the size of the memory buffer, if the
 *   'buffer' parameter is not NULL; otherwise, pointer to a zero value. 
 *   This parameter
 *   is in/out: the caller sets the value pointed to to the size of the memory
 *   buffer, and upon return the value pointed to will be equal to the number 
 *   of bytes written (or the number of bytes that would be written, if either the 
 *   value passed was too small or 'buffer' is NULL).
 * 
 * @return SKB_SUCCESS if the call succeeds, SKB_ERROR_BUFFER_TOO_SMALL if
 * a buffer was supplied but there is not enough space in it to write the 
 * exported data, or a different error code if some other failure occurred.
 */
SKB_Result
SKB_SecureData_Export(const SKB_SecureData* self, 
                      SKB_ExportTarget      target,
                      const void*           target_parameters,
                      SKB_Byte*             buffer, 
                      SKB_Size*             buffer_size);

/**
 * Derives a new SKB_SecureData object from an existing one. 
 * 
 * @param self The SKB_SecureData whose derivative is obtained.
 * @param algorithm The derivation algorithm.
 * @param parameters Parameters for the derivation algorithm, or NULL if
 *   the algorithm does not require any parameters.
 * @param data Address of an SKB_SecureData pointer that will be set to point
 *   to a new SKB_SecureData object representing the derived data.
 *   This object must be released by calling SKB_SecureData_Release when no 
 *   longer needed.
 */
SKB_Result
SKB_SecureData_Derive(const SKB_SecureData*   self, 
                      SKB_DerivationAlgorithm algorithm,
                      const void*             parameters, 
                      SKB_SecureData**        data);

/** @} */

/** @ingroup SKB_Transform
 * @{
 */

/**
 * Releases the specified SKB_Transform object.
 * An SKB_Transform object must be released when it is no longer needed,
 * by calling this method.
 * The object can no longer be used by the caller after this call returns.
 * 
 * @param self The SKB_Transform to release.
 */
SKB_Result
SKB_Transform_Release(SKB_Transform* self);

/**
 * Adds caller-supplied bytes to the specified transform.
 * 
 * @param self The SKB_Transform to which bytes should be added.
 * @param data The bytes to be added.
 * @param data_size The number of bytes in the 'data' parameter.
 */
SKB_Result  
SKB_Transform_AddBytes(SKB_Transform*  self, 
                       const SKB_Byte* data, 
                       SKB_Size        data_size);

/**
 * Adds the data bytes represented by an SKB_SecureData object to 
 * the transform.
 * Only SKB_SecureData objects of the following types can be used:
 *  SKB_DATA_TYPE_BYTES
 * 
 * @param self The SKB_Transform to which the bytes should be added.
 * @param data The SKB_SecureData representing the secret data whose
 * bytes should be added to the transform.
 */
SKB_Result  
SKB_Transform_AddSecureData(SKB_Transform* self, const SKB_SecureData* data);

/**
 * Gets the output of the transform. That is, after you have
 * supplied to the transform all the input data it needs
 * (by calls to SKB_Transform_AddSecureData and/or
 * SKB_Transform_AddBytes), you call this method to have the
 * transform operation (such as a digest calculation) executed
 * and the result returned. After this method has been called, no other 
 * method can be called on the SKB_Transform object but SKB_Transform_Release.
 * 
 * If the output buffer you pass to this method is not large enough to hold
 * the transform output, this method sets *output_size to the number of bytes
 * required, and returns SKB_ERROR_BUFFER_TOO_SMALL. If you like, you can find 
 * out in advance the maximum size the output buffer should be by 
 * first calling this method with NULL for the 'output' parameter. In that case,
 * the method outputs, in *output_size, the maximum number of bytes required,
 * and returns SKB_SUCCESS. 
 * 
 * @param self The SKB_Transform whose output will be returned.
 * @param output Buffer where the output of the transform operation is to be
 *   written, or NULL if you just want the method to return, in *output_size, 
 *   a number of bytes sufficient to hold the output. The buffer, if
 *   supplied, must be large enough to hold the number of bytes specified
 *   by the output_size parameter.
 * @param output_size Pointer to the size of the output buffer, if the
 *   'output' parameter is not NULL; otherwise, pointer to a zero value. This 
 *   parameter is in/out: the caller sets the value pointed to to the size of the 
 *   buffer, and upon return the value pointed to will be equal to the number 
 *   of bytes written (or the number of bytes that would be written, if either the 
 *   value passed was too small or 'output' is NULL). 
 * 
 * @return SKB_SUCCESS if the call succeeds, SKB_ERROR_BUFFER_TOO_SMALL if
 * a buffer was supplied but there is not enough space in it to write the 
 * output, or a different error code if some other failure occurred.
 */
SKB_Result  
SKB_Transform_GetOutput(SKB_Transform* self, 
                        SKB_Byte*      output,
                        SKB_Size*      output_size);

/** @} */

/** @ingroup SKB_Cipher
 * @{
 */

/**
 * Executes a cipher algorithm on the specified input buffer
 * bytes, and places the result in the specified output buffer.
 * 
 * If the output buffer you pass to this method is not large enough to hold
 * the cipher output, this method sets *out_buffer_size to the number of bytes
 * required, and returns SKB_ERROR_BUFFER_TOO_SMALL. If you like, you can find 
 * out in advance the maximum size the output buffer should be by 
 * first calling this method with NULL for the 'out_buffer' parameter. In that case,
 * the method outputs, in *out_buffer_size, the maximum number of bytes required,
 * and returns SKB_SUCCESS.
 * 
 * @param self The SKB_Cipher whose algorithm will be executed.
 * @param in_buffer The buffer of bytes on which to execute
 *   the cipher algorithm. For block ciphers, this MUST point to the beginning
 *   of a cipher block.
 * @param in_buffer_size The number of bytes in in_buffer. For ciphers in ECB or CBC mode,
 *   this MUST be a multiple of the cipher block size. For RSA ciphers, this must be
 *   the size of the entire encrypted message.
 * @param out_buffer The buffer to which the output is to be written, or
 *   NULL if you just want the method to return, in *out_buffer_size, a number of
 *   bytes sufficient to hold the output. The memory buffer, if
 *   supplied, must be large enough to hold the number of bytes specified
 *   by the out_buffer_size parameter.
 * @param out_buffer_size Pointer to the size of out_buffer, if the
 *   out_buffer parameter is not NULL; otherwise, pointer to a zero value. 
 *   This parameter
 *   is in/out: the caller sets the value pointed to to the size of the 
 *   buffer, and upon return the value pointed to will be equal to the number 
 *   of bytes written (or the number of bytes that would be written, if either the 
 *   value passed was too small or out_buffer is NULL).
 * @param iv Pointer to the Initialization Vector, if required by the algorithm, or
 *   NULL if no Initialization Vector is required, or if the Initialization Vector is
 *   implicitly all zeros.
 * @param iv_size Size in bytes of the Initialization Vector. Set to 0 if the iv 
 * parameter is NULL.
 * 
 * @return SKB_SUCCESS if the call succeeds, SKB_ERROR_BUFFER_TOO_SMALL if
 * a buffer was supplied but there is not enough space in it to write the 
 * output, or a different error code if some other failure occurred.
 */
SKB_Result  
SKB_Cipher_ProcessBuffer(SKB_Cipher*     self, 
                         const SKB_Byte* in_buffer, 
                         SKB_Size        in_buffer_size,
                         SKB_Byte*       out_buffer,
                         SKB_Size*       out_buffer_size,
                         const SKB_Byte* iv,
                         SKB_Size        iv_size);

/**
 * Releases the specified SKB_Cipher object.
 * An SKB_Cipher object must be released when it is no longer needed,
 * by calling this method.
 * The object can no longer be used by the caller after this call returns.
 * 
 * @param self The SKB_Cipher to be released.
 */
SKB_Result
SKB_Cipher_Release(SKB_Cipher* self);

/** @} */

#if defined(__cplusplus)
}
#endif

#endif /* _SKB_SECURE_KEY_BOX_H_ */
