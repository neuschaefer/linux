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
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: dmx_ide.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_drm.h
 *  Header of PD-DRM interfaces
 */


#ifndef _DMX_DRM_H_
#define _DMX_DRM_H_

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"
//#include "dmx_if.h"
//#include "dmx_mm_if.h"
//#include "dmx_drm_if.h"
#include "x_os.h"

//-----------------------------------------------------------------------------
//
// SHA
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define SHA1_HASH_SIZE      (20)
#define SHA224_HASH_SIZE    (28)
#define SHA256_HASH_SIZE    (32)
#define SHA_BLOCK_SIZE      (64)    //512-bit

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#ifndef _SHA_enum_
#define _SHA_enum_
enum
{
    shaSuccess = 0,
    shaNull,            /* Null pointer parameter */
    shaInputTooLong,    /* input data too long */
    shaStateError       /* called Input after Result */
};
#endif


// This structure will hold context information for the SHA-1
// hashing operation
typedef struct
{
    UINT32 Intermediate_Hash[SHA1_HASH_SIZE/4]; /* Message Digest  */

    UINT32 Length_Low;                          /* Message length in bits      */
    UINT32 Length_High;                         /* Message length in bits      */

                                                /* Index into message block array   */
    INT32 Message_Block_Index;
    UINT8 Message_Block[64];                    /* 512-bit message blocks      */

    INT32 Computed;                             /* Is the digest computed?         */
    INT32 Corrupted;                            /* Is the message digest corrupted? */
} SHA1_CONTEXT_T;

// This structure will hold context information for the SHA-256
// hashing operation
typedef struct
{
    UINT32 Intermediate_Hash[SHA256_HASH_SIZE/4];   /* Message Digest  */

    UINT32 Length_Low;                              /* Message length in bits      */
    UINT32 Length_High;                             /* Message length in bits      */

                                                    /* Index into message block array   */
    INT32 Message_Block_Index;
    UINT8 Message_Block[64];                        /* 512-bit message blocks      */

    INT32 Computed;                                 /* Is the digest computed?         */
    INT32 Corrupted;                                /* Is the message digest corrupted? */
} SHA256_CONTEXT_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------

EXTERN INT32 SHA1_Reset(SHA1_CONTEXT_T *);
EXTERN INT32 SHA1_Input(SHA1_CONTEXT_T *, const UINT8 *, UINT32);
EXTERN INT32 SHA1_Result(SHA1_CONTEXT_T *, UINT8 Message_Digest[SHA1_HASH_SIZE]);

// SHA-224 has the same struct of SHA-256,
// the main different is initial constant and result size is 224 bit
EXTERN INT32 SHA224_Reset(SHA256_CONTEXT_T *);
EXTERN INT32 SHA224_Input(SHA256_CONTEXT_T *, const UINT8 *, UINT32);
EXTERN INT32 SHA224_Result(SHA256_CONTEXT_T *, UINT8 Message_Digest[SHA224_HASH_SIZE]);

EXTERN INT32 SHA256_Reset(SHA256_CONTEXT_T *);
EXTERN INT32 SHA256_Input(SHA256_CONTEXT_T *, const UINT8 *, UINT32);
EXTERN INT32 SHA256_Result(SHA256_CONTEXT_T *, UINT8 Message_Digest[SHA256_HASH_SIZE]);
EXTERN INT32 SHA256(const UINT8 *message_array, UINT32 length, UINT8 Message_Digest[SHA256_HASH_SIZE]);
EXTERN INT32 mtk_hmac_sha256(UINT8 * key, UINT32 key_size, UINT8 * msg, UINT32 msg_size,  UINT8 result[SHA256_HASH_SIZE]);

//-----------------------------------------------------------------------------
//
// MD
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define MD5_HASH_SIZE      (16)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

// This structure will hold context information for the MD5
// hashing operation
typedef struct {
  UINT32 au4State[4];                           /* state  */
  UINT32 au4Count[2];                           /* number of bits, modulo 2^64 (lsb first) */
  UINT8 au1Buffer[64];                          /* input buffer */
} MD5_CONTEXT_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------
EXTERN void MD5_Reset(MD5_CONTEXT_T*);
EXTERN void MD5_Input(MD5_CONTEXT_T *, const UINT8 *, UINT32);
EXTERN void MD5_Result(MD5_CONTEXT_T *, UINT8 Message_Digest[MD5_HASH_SIZE]);

//-----------------------------------------------------------------------------
//
// RC4
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define RC4_TABLESIZE 256

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


/// RC4 key structure
typedef struct
{
	UINT8 S[(RC4_TABLESIZE)];       /* State table */
	UINT8 i, j;                     /* Indices */
} RC4_KEY_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------

EXTERN void RC4_Init(RC4_KEY_T *prRC4Key, UINT32 u4KeyLen, const UINT8 *pu1Key);
EXTERN void RC4_Output(RC4_KEY_T *prRC4Key, UINT32 u4Size, UINT8 *pu1Buf);

//-----------------------------------------------------------------------------
//
// CBC-MAC
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct
{
    UINT32 a1, b1, c1, d1, e1, f1, a2, b2, c2, d2, e2, f2;
} DRM_CBCKey;

typedef struct
{
    UINT32 sum,t;
    UINT8 buf[8];
    UINT32 dwBufLen;
} DRM_CBCState_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#ifndef __KERNEL__
#define min(x, y) ((x < y) ? x : y)
#endif

#define WORDSWAP(d) ( ( (d) >> 16 ) + ( (d) << 16 ) )

#define BYTES_TO_DWORD BYTE2LITTLEENDIAN
#define DWORD_TO_BYTES DWORD2LITTLEENDIAN

#define BYTE2LITTLEENDIAN(dword, byte)     DRM_BYT_CopyBytes((UINT8*)&(dword),0,(byte),0,sizeof(UINT32));
#define DWORD2LITTLEENDIAN(byte, dword)  DRM_BYT_CopyBytes((byte),0,(UINT8*)&(dword),0,sizeof(UINT32));

#define DRM_BYT_CopyBytes(to,tooffset,from,fromoffset,count) x_memcpy(&((to)[(tooffset)]),&((from)[(fromoffset)]),(count))

//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------

EXTERN UINT32 DRM_MAC_inv32(UINT32 n);
EXTERN void DRM_CBC_Mac(UINT8 *pbData, UINT32 cBlocks, UINT32 rgdwKeys[2], const DRM_CBCKey *pCBCkey);
EXTERN void DRM_CBC_InverseMac(UINT8 *pbData,UINT32 cBlocks, const DRM_CBCKey *key, const DRM_CBCKey *ikey);
EXTERN void DRM_CBC64InitState( DRM_CBCState_T *cbcstate );
EXTERN void DRM_CBC64Init( DRM_CBCKey *cbckey, DRM_CBCState_T *cbcstate, UINT8 *pKey );
EXTERN void DRM_CBC64InvKey( DRM_CBCKey *cbckey, DRM_CBCKey *cbcInvKey );
EXTERN void DRM_CBC64Update(DRM_CBCKey *key, DRM_CBCState_T *cbcstate, UINT32 cbData, UINT8 *pbData);
EXTERN UINT32 DRM_CBC64Finalize( DRM_CBCKey *key, DRM_CBCState_T *cbcstate, UINT32 *pKey2 );
EXTERN UINT32 DRM_CBC64Invert( DRM_CBCKey *key, DRM_CBCKey *ikey, UINT32 MacA1, UINT32 MacA2,
        UINT32 MacB1, UINT32 MacB2, UINT32 *pInvKey2);

//-----------------------------------------------------------------------------
//
// DES
//
//-----------------------------------------------------------------------------
typedef unsigned long DES_KS[16][2];	/* Single-key DES key schedule */
typedef unsigned long DES3_KS[48][2];	/* Triple-DES key schedule */

/* In deskey.c: */
void deskey(DES_KS,unsigned char *,int);
void des3key(DES3_KS,unsigned char *,int);

/* In desport.c, desborl.cas or desgnu.s: */
void des(DES_KS,unsigned char *);
/* In des3port.c, des3borl.cas or des3gnu.s: */
void des3(DES3_KS,unsigned char *);

extern int Asmversion;	/* 1 if we're linked with an asm version, 0 if C */

void des_crypt_cfb( DES_KS ks,
                    int mode,
                    int length,
                    int *iv_off,
                    unsigned char iv[8],
                    unsigned char *input,
                    unsigned char *output );

void des_crypt_ofb( DES_KS ks,
                    int mode,
                    int length,
                    int *iv_off,
                    unsigned char iv[8],
                    unsigned char *input,
                    unsigned char *output );

void des3_crypt_cfb( DES3_KS ks,
                    int mode,
                    int length,
                    int *iv_off,
                    unsigned char iv[8],
                    unsigned char *input,
                    unsigned char *output );

void des3_crypt_ofb( DES3_KS ks,
                    int mode,
                    int length,
                    int *iv_off,
                    unsigned char iv[8],
                    unsigned char *input,
                    unsigned char *output );

//-----------------------------------------------------------------------------
//
// PD-DRM
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define PK_ENC_PLAINTEXT_LEN	16

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct
{
    UINT8 au1SHAOut[SHA1_HASH_SIZE];
    UINT32 au4DESS1[2];
    UINT32 au4DESS2[2];
    UINT32 u4DES_KS[16][2];
    DRM_CBCKey rMacKey;
    DRM_CBCKey rInvMacKey;
    RC4_KEY_T rRC4Key;
    BOOL fgInited;
    BOOL fgDecryptInited;
    DRM_CBCState_T rCBCState;
    UINT32 au4RC4Key[2];
    UINT32 au4Last8[2];
    UINT32 u4TotalLen;
    UINT32 u4OPedLen;
    UINT32 u4ContentKeyLen;
    UINT8 au1ContentKey[PK_ENC_PLAINTEXT_LEN];
} DRM_CONTEXT_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

#ifndef NO_OF
    #define NO_OF( x )  ( sizeof( (x) ) / sizeof( (x)[0] ) )
#endif

//-----------------------------------------------------------------------------
//
// AES
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define AES_MAX_ENGINE_INDEX    (32)
#define AES_NULL_ENGINE_INDEX   (0xff)

#define AES_RESERVED_ENGINE_NUM (3)     // 0: Legacy AES 1: MTD AES 2. WV AES
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define AES_LEGACY_INDEX        (0)
#define AES_MTD_INDEX           (1)
#define AES_WV_INDEX            (2)

#define AES_MTD_SECURE_KEY_PTR  (92)

#define AES_BLOCK_LEN           (16)
//-----------------------------------------------------------------------------
/** _DMX_NAND_AES_INIT
 *  NAND AES Initialization
 *
 *  @param  pu1Key          key pointer
 *  @param  u2KeyLen        key length [128|192|256]
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL _DMX_NAND_AES_INIT(UINT8 *pu1Key, UINT16 u2KeyLen);

//-----------------------------------------------------------------------------
/** _DMX_NAND_AES_Encryption
 *  NAND AES encryption
 *
 *  @param  u4InBufStart    input buffer address
 *  @param  u4OutBufStart   output buffer address
 *  @param  u4BufSize       buffer size
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL _DMX_NAND_AES_Encryption(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize);

//-----------------------------------------------------------------------------
/** _DMX_NAND_AES_Decryption
 *  NAND AES decryption
 *
 *  @param  u4InBufStart    input buffer address
 *  @param  u4OutBufStart   output buffer address
 *  @param  u4BufSize       buffer size
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Fail
 */
//-----------------------------------------------------------------------------
EXTERN BOOL _DMX_NAND_AES_Decryption(UINT32 u4InBufStart, UINT32 u4OutBufStart, UINT32 u4BufSize);

#endif	// _DMX_DRM_H_

