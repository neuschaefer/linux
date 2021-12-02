#ifndef TOMCRYPT_CUSTOM_H_
#define TOMCRYPT_CUSTOM_H_

/* macros for various libc functions you can change for embedded targets */
#ifndef XMALLOC
   /*#ifdef malloc
   #define LTC_NO_PROTOTYPES
   #endif
#define XMALLOC  malloc*/
#define XMALLOC  tom_malloc
#endif
#ifndef XREALLOC
   /*#ifdef realloc
   #define LTC_NO_PROTOTYPES
   #endif
#define XREALLOC realloc*/
#define XREALLOC tom_realloc
#endif
#ifndef XCALLOC
   /*#ifdef calloc
   #define LTC_NO_PROTOTYPES
   #endif
#define XCALLOC  calloc*/
#define XCALLOC  tom_calloc
#endif
#ifndef XFREE
   /*#ifdef free
   #define LTC_NO_PROTOTYPES
   #endif
#define XFREE    free*/
#define XFREE    tom_free
#endif

#ifndef XMEMSET
   /*#ifdef memset
   #define LTC_NO_PROTOTYPES
   #endif
#define XMEMSET  memset*/
#define XMEMSET  tom_memset
#endif
#ifndef XMEMCPY
   /*#ifdef memcpy
   #define LTC_NO_PROTOTYPES
   #endif
#define XMEMCPY  memcpy*/
#define XMEMCPY  tom_memcpy
#endif
#ifndef XMEMCMP
   /*#ifdef memcmp
   #define LTC_NO_PROTOTYPES
   #endif
#define XMEMCMP  memcmp*/
#define XMEMCMP  tom_memcmp
#endif
#ifndef XSTRCMP
   /*#ifdef strcmp
   #define LTC_NO_PROTOTYPES
   #endif
#define XSTRCMP strcmp*/
#define XSTRCMP tom_strcmp
#endif

#ifndef XCLOCK
#define XCLOCK   HAL_GetFineTick
#endif

#ifndef XCLOCKS_PER_SEC
#define XCLOCKS_PER_SEC CLOCKS_PER_SEC
#endif

#ifndef XQSORT
   /*#ifdef qsort
   #define LTC_NO_PROTOTYPES
   #endif
#define XQSORT qsort*/
#define XQSORT MTK_QSORT
#endif

/* Easy button? */
#ifdef LTC_EASY
   #define LTC_NO_CIPHERS
   #define RIJNDAEL
   #define BLOWFISH
   #define DES
   #define CAST5

   #define LTC_NO_MODES
   #define LTC_ECB_MODE
   #define LTC_CBC_MODE
   #define LTC_CTR_MODE

   #define LTC_NO_HASHES
   #define SHA1
   #define SHA512
   #define SHA384
   #define SHA256
   #define SHA224

   #define LTC_NO_MACS
   #define HMAC
   #define OMAC
   #define CCM_MODE

   #define LTC_NO_PRNGS
   #define SPRNG
   #define YARROW
   #define DEVRANDOM
   #define TRY_URANDOM_FIRST

   #define LTC_NO_PK
   #define MRSA
   #define MECC
#endif

/* Use small code where possible */
/* #define LTC_SMALL_CODE */

/* Enable self-test test vector checking */
#ifndef LTC_NO_TEST
   #define LTC_TEST
#endif

/* clean the stack of functions which put private information on stack */
/* #define LTC_CLEAN_STACK */

/* disable all file related functions */
/* #define LTC_NO_FILE */

/* disable all forms of ASM */
/* #define LTC_NO_ASM */

/* disable FAST mode */
/* #define LTC_NO_FAST */

/* disable BSWAP on x86 */
/* #define LTC_NO_BSWAP */

/* ---> Symmetric Block Ciphers <--- */
#ifndef LTC_NO_CIPHERS

#undef BLOWFISH
#undef RC2
#undef RC5
#undef RC6
#undef SAFERP
#define RIJNDAEL
#undef XTEA
/* _TABLES tells it to use tables during setup, _SMALL means to use the smaller scheduled key format
 * (saves 4KB of ram), _ALL_TABLES enables all tables during setup */
#undef TWOFISH
#ifndef LTC_NO_TABLES
   #define TWOFISH_TABLES
   /* #define TWOFISH_ALL_TABLES */
#else
   #define TWOFISH_SMALL
#endif
/* #define TWOFISH_SMALL */
/* DES includes EDE triple-DES */
#undef DES
#undef CAST5
#undef NOEKEON
#undef SKIPJACK
#undef SAFER
#undef KHAZAD
#undef ANUBIS
#undef ANUBIS_TWEAK
#undef KSEED
#undef LTC_KASUMI

#endif /* LTC_NO_CIPHERS */


/* ---> Block Cipher Modes of Operation <--- */
#ifndef LTC_NO_MODES

#undef LTC_CFB_MODE
#undef LTC_OFB_MODE
#define LTC_ECB_MODE
#define LTC_CBC_MODE
#define LTC_CTR_MODE

/* F8 chaining mode */
#undef LTC_F8_MODE

/* LRW mode */
#undef LTC_LRW_MODE
#ifndef LTC_NO_TABLES
   /* like GCM mode this will enable 16 8x128 tables [64KB] that make
    * seeking very fast.
    */
   #define LRW_TABLES
#endif

#endif /* LTC_NO_MODES */

/* ---> One-Way Hash Functions <--- */
#ifndef LTC_NO_HASHES

#undef CHC_HASH
#undef WHIRLPOOL
#undef SHA512
#undef SHA384
#define SHA256
#undef SHA224
#undef TIGER
#define SHA1
#define MD5
#undef MD4
#undef MD2
#undef RIPEMD128
#undef RIPEMD160
#undef RIPEMD256
#undef RIPEMD320

#endif /* LTC_NO_HASHES */

/* ---> MAC functions <--- */
#ifndef LTC_NO_MACS

#define LTC_HMAC
#undef LTC_OMAC
#undef LTC_PMAC
#undef LTC_XCBC
#undef LTC_F9_MODE
#undef PELICAN

#if defined(PELICAN) && !defined(RIJNDAEL)
   #error Pelican-MAC requires RIJNDAEL
#endif

/* ---> Encrypt + Authenticate Modes <--- */

#undef EAX_MODE
#if defined(EAX_MODE) && !(defined(LTC_CTR_MODE) && defined(LTC_OMAC))
   #error EAX_MODE requires CTR and OMAC mode
#endif

#undef OCB_MODE
#undef CCM_MODE
#undef GCM_MODE

/* Use 64KiB tables */
#ifndef LTC_NO_TABLES
   #define GCM_TABLES
#endif

/* USE SSE2? requires GCC works on x86_32 and x86_64*/
#ifdef GCM_TABLES
/* #define GCM_TABLES_SSE2 */
#endif

#endif /* LTC_NO_MACS */

/* Various tidbits of modern neatoness */
#define BASE64

/* --> Pseudo Random Number Generators <--- */
#ifndef LTC_NO_PRNGS

/* Yarrow */
#undef YARROW
/* which descriptor of AES to use?  */
/* 0 = rijndael_enc 1 = aes_enc, 2 = rijndael [full], 3 = aes [full] */
#define YARROW_AES 0

#if defined(YARROW) && !defined(LTC_CTR_MODE)
   #error YARROW requires LTC_CTR_MODE chaining mode to be defined!
#endif

/* a PRNG that simply reads from an available system source */
#define SPRNG

/* The RC4 stream cipher */
#undef RC4

/* Fortuna PRNG */
#define FORTUNA
/* reseed every N calls to the read function */
#define FORTUNA_WD    10
/* number of pools (4..32) can save a bit of ram by lowering the count */
#define FORTUNA_POOLS 32

/* Greg's SOBER128 PRNG ;-0 */
#undef SOBER128

/* the *nix style /dev/random device */
#define DEVRANDOM
/* try /dev/urandom before trying /dev/random */
#define TRY_URANDOM_FIRST

#endif /* LTC_NO_PRNGS */

/* ---> math provider? <--- */
#ifndef LTC_NO_MATH

/* LibTomMath */
/* #define LTM_DESC */

/* TomsFastMath */
/* #define TFM_DESC */

#endif /* LTC_NO_MATH */

/* ---> Public Key Crypto <--- */
#ifndef LTC_NO_PK

/* Include RSA support */
#define MRSA

/* Include Katja (a Rabin variant like RSA) */
/* #define MKAT */

/* Digital Signature Algorithm */
#undef MDSA

/* ECC */
#undef MECC

/* use Shamir's trick for point mul (speeds up signature verification) */
#undef LTC_ECC_SHAMIR

#if defined(TFM_DESC) && defined(MECC)
   #define MECC_ACCEL
#endif

/* do we want fixed point ECC */
/* #define MECC_FP */

/* Timing Resistant? */
/* #define LTC_ECC_TIMING_RESISTANT */

#endif /* LTC_NO_PK */

/* PKCS #1 (RSA) and #5 (Password Handling) stuff */
#ifndef LTC_NO_PKCS

#define PKCS_1
#undef PKCS_5

/* Include ASN.1 DER (required by DSA/RSA) */
#define LTC_DER

#endif /* LTC_NO_PKCS */

/* cleanup */

#ifdef MECC
/* Supported ECC Key Sizes */
#ifndef LTC_NO_CURVES
   #define ECC112
   #define ECC128
   #define ECC160
   #define ECC192
   #define ECC224
   #define ECC256
   #define ECC384
   #define ECC521
#endif
#endif

#if defined(MECC) || defined(MRSA) || defined(MDSA) || defined(MKATJA)
   /* Include the MPI functionality?  (required by the PK algorithms) */
   #define MPI
#endif

#ifdef MRSA
   #define PKCS_1
#endif

#if defined(LTC_DER) && !defined(MPI)
   #error ASN.1 DER requires MPI functionality
#endif

#if (defined(MDSA) || defined(MRSA) || defined(MECC) || defined(MKATJA)) && !defined(LTC_DER)
   #error PK requires ASN.1 DER functionality, make sure LTC_DER is enabled
#endif

/* THREAD management */
#ifdef LTC_PTHREAD
#include <pthread.h>

#define LTC_MUTEX_GLOBAL(x)   pthread_mutex_t x = PTHREAD_MUTEX_INITIALIZER;
#define LTC_MUTEX_PROTO(x)    extern pthread_mutex_t x;
#define LTC_MUTEX_TYPE(x)     pthread_mutex_t x;
#define LTC_MUTEX_INIT(x)     pthread_mutex_init(x, NULL);
#define LTC_MUTEX_LOCK(x)     pthread_mutex_lock(x);
#define LTC_MUTEX_UNLOCK(x)   pthread_mutex_unlock(x);

#else

#ifndef CC_SECURE_WORLD
#include <linux/semaphore.h>
/* default no functions */
#define LTC_MUTEX_GLOBAL(x) DECLARE_MUTEX(x);
#define LTC_MUTEX_PROTO(x)  extern struct semaphore x;
#define LTC_MUTEX_TYPE(x)   struct semaphore x;
#define LTC_MUTEX_INIT(x)
#define LTC_MUTEX_LOCK(x)   down(x);
#define LTC_MUTEX_UNLOCK(x) up(x);
#else
/* default no functions */
#define LTC_MUTEX_GLOBAL(x)
#define LTC_MUTEX_PROTO(x)
#define LTC_MUTEX_TYPE(x)
#define LTC_MUTEX_INIT(x)
#define LTC_MUTEX_LOCK(x)
#define LTC_MUTEX_UNLOCK(x)
#endif

#endif

/* Debuggers */

/* define this if you use Valgrind, note: it CHANGES the way SOBER-128 and RC4 work (see the code) */
/* #define LTC_VALGRIND */

#endif



/* $Source: /cvs/libtom/libtomcrypt/src/headers/tomcrypt_custom.h,v $ */
/* $Revision: #1 $ */
/* $Date: 2012/04/27 $ */
