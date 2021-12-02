/*----------------------------------------------------------------------------*
 * No Warranty                                                                *
 * Except as may be otherwise agreed to in writing, no warranties of any      *
 * kind, whether express or implied, are given by MTK with respect to any MTK *
 * Deliverables or any use thereof, and MTK Deliverables are provided on an   *
 * "AS IS" basis.  MTK hereby expressly disclaims all such warranties,        *
 * including any implied warranties of merchantability, non-infringement and  *
 * fitness for a particular purpose and any warranties arising out of course  *
 * of performance, course of dealing or usage of trade.  Parties further      *
 * acknowledge that Company may, either presently and/or in the future,       *
 * instruct MTK to assist it in the development and the implementation, in    *
 * accordance with Company's designs, of certain softwares relating to        *
 * Company's product(s) (the "Services").  Except as may be otherwise agreed  *
 * to in writing, no warranties of any kind, whether express or implied, are  *
 * given by MTK with respect to the Services provided, and the Services are   *
 * provided on an "AS IS" basis.  Company further acknowledges that the       *
 * Services may contain errors, that testing is important and Company is      *
 * solely responsible for fully testing the Services and/or derivatives       *
 * thereof before they are used, sublicensed or distributed.  Should there be *
 * any third party action brought against MTK, arising out of or relating to  *
 * the Services, Company agree to fully indemnify and hold MTK harmless.      *
 * If the parties mutually agree to enter into or continue a business         *
 * relationship or other arrangement, the terms and conditions set forth      *
 * hereunder shall remain effective and, unless explicitly stated otherwise,  *
 * shall prevail in the event of a conflict in the terms in any agreements    *
 * entered into between the parties.                                          *
 *---------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: dmx_if.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file gcpu_if.h
 *  Interface of gcpu driver
 */


#ifndef GCPU_IF_H
#define GCPU_IF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_typedef.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
// #define CC_EMULATION

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define CSS_CMD_SID             (0x0 << 8)
#define CPPM_CMD_SID            (0x1 << 8)
#define AES_CMD_SID             (0x2 << 8)
#define VCPS_CMD_SID            (0x3 << 8)
#define AACS_CMD_SID            (0x4 << 8)
#define TDES_CMD_SID            (0x5 << 8)
#define BDRE_CMD_SID            (0x6 << 8)
#define EFUSE_CMD_SID           (0x7 << 8)
#define MISC_CMD_SID            (0x8 << 8)
#define SW_CMD_SID              (0x9 << 8)

#define GCPU_FIFO_ALIGNMENT     (32)
//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
enum
{
    S_GCPU_OK,
    E_GCPU_NO_RES,
    E_GCPU_PARAM_WRONG,
    E_GCPU_UNEXPECT,
    E_GCPU_OS_OPERA_FAIL,
    E_GCPU_OVER_LIMIT,
    E_GCPU_OPERATE_FORBID,
    E_GCPU_CMD_FAIL,
    E_GCPU_NO_MEM,
    E_GCPU_INV_ARG,
    E_GCPU_PAGE_FAULT,
    E_GCPU_FAIL,
};

enum
{
    GCPU_CSS_AUTH_DRV = CSS_CMD_SID,
    GCPU_CSS_AUTH_DEC,
    GCPU_CSS_AUTH_BK,
    GCPU_CSS_DEC_DK,
    GCPU_CSS_DEC_TK,
    GCPU_CSS_DSC_AV,
};

enum
{
    GCPU_AES_D = AES_CMD_SID,
    GCPU_AES_E,
    GCPU_AESPK_D,
    GCPU_AES_G,
    GCPU_AES_DPAK,
    GCPU_AES_CMAC,
    GCPU_AESPK_DPAK,
    GCPU_AES_EPAK,
    GCPU_AES_D_CMP,
    GCPU_AESPK_E,
    GCPU_AESEK_E,
    GCPU_AESPK_EPAK,
    GCPU_AESPK_DCBC,
    GCPU_AESPK_ECBC,
    GCPU_AESEK_D,
    GCPU_AES_DCBC,
    GCPU_AES_ECBC,
    GCPU_AESPK_EK_D,
    GCPU_AESPK_EK_E,
    GCPU_AESPK_EK_DCBC,
    GCPU_AES_H,
    GCPU_AES_CTR,
    GCPU_AES_OFB,
    GCPU_AES_WRAPD,
    GCPU_AES_WRAPE,
    GCPU_AES,
};

enum
{
    GCPU_C2_D = CPPM_CMD_SID,
    GCPU_C2_E,
    GCPU_C2_H,
    GCPU_C2_G,
    GCPU_CPPM_DPAK,
    GCPU_CPRM_DPAK,
    GCPU_CPRM_EPAK,
    GCPU_CPRM_DCI_VFY,
};

enum
{
    GCPU_VCPS_HASH = VCPS_CMD_SID,
    GCPU_VCPS_DPAK,
    GCPU_VCPS_EPAK,
    GCPU_VCPS_DKBH,
    GCPU_VCPS_DHDK,
    GCPU_VCPS_DCBC,
    GCPU_VCPS_ECBC,
};

enum
{
    GCPU_IF_READ = EFUSE_CMD_SID,
    GCPU_IF_WRITE,
};

enum
{
    GCPU_TDES_D = TDES_CMD_SID,
    GCPU_TDES_E,
    GCPU_TDES_DMA_D,
    GCPU_TDES_DMA_E,
    GCPU_TDES_CBC_D,
    GCPU_TDES_CBC_E,
    GCPU_TDES,
};

enum
{
    GCPU_AACS_DBD = AACS_CMD_SID,
    GCPU_AACS_EBD,
    GCPU_AACS_DTN,
    GCPU_AACS_ETN,
    GCPU_AACS_DHD,
    GCPU_AACS_EHD,
    GCPU_AACS_DV_CALC,
};

enum
{
    GCPU_BDRE_DBD = BDRE_CMD_SID,
    GCPU_BDRE_EBD,
    GCPU_BDRE_DTN,
    GCPU_BDRE_ETN,
    GCPU_BDRE_BE,
};

enum
{
    GCPU_MEMCPY = MISC_CMD_SID,
    GCPU_DMA,
    GCPU_SHA_1,
    GCPU_SHA_256,
    GCPU_MD5,
    GCPU_SHA_224,
    GCPU_MEM_XOR,
    GCPU_RNG,
    GCPU_RC4_KSA,
    GCPU_RC4,
    GCPU_ROM_BIST,
    GCPU_PTX,
    GCPU_TSDESC,
};

typedef enum
{
    HASH_FIRST_PACKET_MODE,
    HASH_SUCCESSIVE_PACKET_MODE,
} HASH_MODE;

typedef enum
{
    CPRM_HASH_FIRST_PACKET_MODE,
    CPRM_HASH_SUCCESSIVE_PACKET_MODE,
} CPRM_HASH_MODE;

typedef enum
{
    DESCRAMBLE_MODE_OFF,
    DESCRAMBLE_MODE_ON,
    DESCRAMBLE_MODE_AUTO,
    DESCRAMBLE_MODE_RESERVED,
} DESCRAMBLE_MODE;

typedef enum
{
    CPRM_CIPHER_MODE_OFF,
    CPRM_CIPHER_MODE_ON,
    CPRM_CIPHER_MODE_AUTO,
    CPRM_CIPHER_MODE_RESERVED,
} CPRM_CIPHER_MODE;

typedef enum
{
    BLOCK_CIPHER_MODE_ECB,
    BLOCK_CIPHER_MODE_CBC,
    BLOCK_CIPHER_MODE_OFB,
    BLOCK_CIPHER_MODE_CTR,
} BLOCK_CIPHER_MODE;

typedef enum
{
    KEY_BIT_LEN_40 = 40,
    KEY_BIT_LEN_64 = 64,
    KEY_BIT_LEN_128 = 128,
    KEY_BIT_LEN_192 = 192,
    KEY_BIT_LEN_256 = 256,
} KEY_BIT_LEN;

typedef enum
{
    AES_CMAC_ONE_PASS_COMPLETE_MODE,
    AES_CMAC_ONE_PASS_REMAINDER_MODE,
    AES_CMAC_START_MODE,
    AES_CMAC_UPDATE_MODE,
    AES_CMAC_FINALIZE_COMPLETE_MODE,
    AES_CMAC_FINALIZE_REMAINDER_MODE,
} AES_CMAC_MODE;

typedef enum
{
    CSS_AV_MODE_OFF,
    CSS_AV_MODE_ON,
    CSS_AV_MODE_AUTO,
    CSS_AV_MODE_RESERVED,
} CSS_AV_MODE;

typedef enum
{
    BDRE_ALWAYS_OFF_MODE,
    BDRE_ALWAYS_ON_MODE,
    BDRE_DEPEND_ON_PACKET_MODE,
    BDRE_DEPEND_ON_CCI_MODE,
} BDRE_MODE;

typedef enum
{
    DMA_MODE_FROM_DRAM_TO_DMEM = 1 | 8,
    DMA_MODE_FROM_DRAM_TO_IMEM = 5 | 8,
    DMA_MODE_FROM_IMEM_TO_DRAM = 7 | 8,
} DMA_MODE;

typedef enum
{
    PTX_FIRST_BYTE_MODE,
    PTX_SUCCESSIVE_BYTES_MODE,
} PTX_MODE;

typedef enum
{
    BLOCK_CIPHER_EMU_MODE_ECB,
    BLOCK_CIPHER_EMU_MODE_GEN,
    BLOCK_CIPHER_EMU_MODE_CMAC,
} BLOCK_CIPHER_EMU_MODE;

typedef enum
{
    TSDESC_188_PACKET_FORM,
    TSDESC_192_PACKET_FORM,
} TSDESC_PACKET_FORM;

typedef enum
{
    TSDESC_FIRST_PACKET_MODE,
    TSDESC_SUCCESSIVE_PACKET_MODE,
} TSDESC_MODE;

typedef enum
{
    TSDESC_DESC_SCHEME_CBC_SCTE52 = 0xE,
    TSDESC_DESC_SCHEME_CTR = 0xF,
} TSDESC_DESC_SCHEME;

//
// CSS
//
typedef struct
{
    UINT8 au1RDK[5];
    UINT8 au1EDK[5];
    UINT8 au1Res[4];
} CSS_DK_PARAM_T;

typedef struct
{
    UINT8 au1ETK[5];
} CSS_TK_PARAM_T;

typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DstStartAddr;
    UINT32 u4DstBufStart;
    UINT32 u4DstBufEnd;
    UINT32 u4DatLen;
    CSS_AV_MODE eMode;
} CSS_AV_PARAM_T;

typedef struct
{
    UINT8 au1CD[10];
    UINT8 au1RD[5];
    UINT32 u4ACC;
} CSS_AUTH_PARAM_T;

//
// CPPM/CPRM
//
typedef struct
{
    UINT8 au1Key[8];
    UINT8 au1Dat[8];
    UINT8 au1Res[8];
} C2_PARAM_T;


typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DatLen;
    BOOL fgFirstPacket;
    BOOL fgLastPacket;
    UINT8 au1Hash[8];
} C2_HASH_PARAM_T;

typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DstStartAddr;
    UINT32 u4DstBufStart;
    UINT32 u4DstBufEnd;
    UINT32 u4DatLen;
    CPRM_CIPHER_MODE eMode;
    UINT8 au1Key[8];
    UINT32 u4APSTB;
} CPRM_PARAM_T;

typedef struct
{
    UINT8 au1KT[8];
    UINT8 au1DCI[8];
    UINT8 au1DTKC[8];
    UINT8 au1Res[8];
} CPRM_DCICCI_PARAM_T;

//
// AES
//
typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DstStartAddr;
    UINT32 u4DstBufStart;
    UINT32 u4DstBufEnd;
    UINT32 u4DatLen;
    UINT8 au1Key[32];
    UINT8 au1Iv[16];
    KEY_BIT_LEN eKeyBitLen;
    BLOCK_CIPHER_MODE eMode;
    BOOL fgEncrypt;
} AES_PARAM_T;

typedef struct
{
    UINT8 au1Dat[16];
    UINT8 au1Res[16];
    UINT8 au1Key[32];
    KEY_BIT_LEN eKeyBitLen;
    UINT32 u4SecureKeyPtr;
} SIM_AES_PARAM_T;

typedef struct
{
    UINT8 au1Dat[16];
    UINT8 au1Res[16];
    UINT8 au1Key[32];
    KEY_BIT_LEN eKeyBitLen;
    UINT32 u4CmpRes;
    UINT32 u4Value;
} AES_DCMP_PARAM_T;

//
// VCPS
//
typedef struct
{
    UINT8 au1Key[16];
    UINT8 au1Dat[16];
    UINT8 au1Res[16];
} VCPS_HASH_PARAM_T;

typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DstStartAddr;
    UINT32 u4DstBufStart;
    UINT32 u4DstBufEnd;
    UINT32 u4DatLen;
    DESCRAMBLE_MODE eMode;
    UINT8 au1Key[16];
} VCPS_PARAM_T;

typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DatLen;
    HASH_MODE eMode;
    UINT8 au1HashValue[16];
} VCPS_DKB_HASH_PARAM_T;

typedef struct
{
    UINT8 au1Key[16];
    UINT8 au1Dat[32];
} VCPS_AES_CBC_PARAM_T;

typedef struct
{
    UINT32 u4SecureDatPtr;
    UINT32 u4SecureResPtr;
} VCPS_DHDK_PARAM_T;

//
// AACS
//
typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DstStartAddr;
    UINT32 u4DstBufStart;
    UINT32 u4DstBufEnd;
    UINT32 u4DatLen;
    UINT8 au1Uk[16];
} AACS_AV_PARAM_T;

typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DstStartAddr;
    UINT32 u4DstBufStart;
    UINT32 u4DstBufEnd;
    UINT32 u4DatLen;
    UINT8 au1Kt[16];
    UINT8 au1Cpi[12];
    DESCRAMBLE_MODE eMode;
} AACS_HDDVD_PARAM_T;

typedef struct
{
    UINT8 au1Kmsi[16];
    UINT8 au1Xfcr[16];
    UINT8 au1Dkder[12];
    UINT8 au1Dv[12];
    UINT32 u4KmsiSecureSlotPtr;
    UINT32 u4DvSecureSlotPtr;
    UINT32 u4Mode;
    UINT32 u4DVLSB10;
    UINT32 u4Cmp;
} AACS_DVCALC_PARAM_T;


//
// BDRE
//
typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DstStartAddr;
    UINT32 u4DstBufStart;
    UINT32 u4DstBufEnd;
    UINT32 u4DatLen;
    UINT8 au1Krec[16];
    UINT32 u4Vbkg;
    BDRE_MODE eMode;
    UINT32 u4Cci;
} BDRE_PARAM_T;

typedef struct
{
    UINT8 au1Dat[8];
} BDRE_BE_PARAM_T;

//
// DES
//
typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DstStartAddr;
    UINT32 u4DstBufStart;
    UINT32 u4DstBufEnd;
    UINT32 u4DatLen;
    UINT8 au1Key[24];
    UINT8 au1Iv[8];
    KEY_BIT_LEN eKeyBitLen;
    BLOCK_CIPHER_MODE eMode;
    BOOL fgEncrypt;
} DES_PARAM_T;


typedef struct
{
    UINT8 au1Dat[16];
    UINT8 au1Res[16];
    UINT8 au1Key[24];
    KEY_BIT_LEN eKeyBitLen;
    UINT32 u4SecureKeyPtr;
} SIM_DES_PARAM_T;

//
// MISC
//
typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DatLen;
    UINT64 u8BitCnt;
    UINT8 au1Hash[32];
    BOOL fgFirstPacket;
    BOOL fgLastPacket;
} MD_PARAM_T;

typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DstStartAddr;
    UINT32 u4DstBufStart;
    UINT32 u4DstBufEnd;
    UINT32 u4DatLen;
} MEMCPY_PARAM_T;

typedef struct
{
    UINT8 au1Key[12];
    UINT32 u4KeyLen;
    UINT32 u4keyAddr;
} RC4_KEY_PARAM_T;

typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DstStartAddr;
    UINT32 u4DstBufStart;
    UINT32 u4DstBufEnd;
    UINT32 u4DatLen;
    UINT32 u4ResidualLen;
    UINT8 au1Key[12];
    UINT32 u4keyAddr;
    UINT32 u4InitIJ;
    UINT32 au4ResidualBytes[2];
} RC4_PARAM_T;

typedef struct
{
    UINT32 u4DramStartAddr;
    UINT32 u4DramBufStart;
    UINT32 u4DramBufEnd;
    UINT32 u4SramStartAddr;
    UINT32 u4DatLen;
    DMA_MODE eMode;
} DMA_PARAM_T;

typedef struct
{
    UINT8 au1Dat[4];
    UINT8 au1Xor[4];
} XOR_PARAM_T;

typedef struct
{
    UINT8 au1Seed[16];
    UINT8 au1Res[16];
} RNG_PARAM_T;

typedef struct
{
    UINT32 u4Res;
} ROBBIST_PARAM_T;

typedef struct
{
    UINT32 u4CmdAddr;
    UINT32 u4CmdNum;
    UINT32 u4IntStartAddr;
    UINT32 u4IntBufStart;
    UINT32 u4IntBufEnd;
    PTX_MODE eMode;
    UINT32 u4PicNum;
    UINT32 u4PicStruct[19];
} PTX_PARAM_T;

typedef struct
{
    UINT8 au1Dat[16];
    UINT8 au1Res[16];
    UINT8 au1Key[32];
    KEY_BIT_LEN eKeyBitLen;
    BLOCK_CIPHER_EMU_MODE eMode;
    BOOL fgEncrypt;
    AES_CMAC_MODE eCmacMode;
    UINT32 u4SecureKeyPtr;
} AES_EMU_PARAM_T;

typedef struct
{
    UINT32 u4ADR;
    UINT8 *pu1Data;
    UINT8 u1DataLen;
} GCPU_IF_PARAM_T;

typedef struct
{
    UINT32 u4SrcStartAddr;
    UINT32 u4SrcBufStart;
    UINT32 u4SrcBufEnd;
    UINT32 u4DstStartAddr;
    UINT32 u4DstBufStart;
    UINT32 u4DstBufEnd;
    UINT32 u4DatLen;
    TSDESC_PACKET_FORM eForm;
    TSDESC_MODE eMode;
    TSDESC_DESC_SCHEME eScheme;
    UINT32 u4TsState[4];
    UINT8 au1Key[2][16];
    UINT8 au1Iv[2][16];
} TSDESC_PARAM_T;

typedef struct
{
    UINT32 u4AesKeyPtr;
    UINT32 u4AesIvPtr;
    UINT32 u4AesRetIvPtr;
} GCPU_STRUCT_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define KEY_WRAPPER(p)  ((p[3] << 24) | (p[2] << 16) | (p[1] << 8) | (p[0]))

#define GCPU_LINER_BUFFER_START(u4Addr) (((u4Addr) % GCPU_FIFO_ALIGNMENT) == 0)?(u4Addr): \
                                    (u4Addr) - ((u4Addr) % GCPU_FIFO_ALIGNMENT)

#define GCPU_LINER_BUFFER_END(u4Addr) (((u4Addr) % GCPU_FIFO_ALIGNMENT) == 0)?(u4Addr): \
                                    (u4Addr) + (GCPU_FIFO_ALIGNMENT - ((u4Addr) % GCPU_FIFO_ALIGNMENT))

//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------
EXTERN BOOL GCPU_Init(UINT32 u4Mode);
EXTERN INT32 GCPU_Cmd(UINT32 u4Handle, UINT32 u4Cmd, void* pvParam);
EXTERN INT32 GCPU_CmdEx(UINT32 u4Handle, UINT32 u4Cmd, void* pvParam, BOOL fgIOMMU);
EXTERN INT32 GCPU_CmdSw(UINT32 u4Handle, UINT32 u4Cmd, void* pvParam);
EXTERN UINT32 GCPU_AllocSecureSlot(UINT32 u4Size);
EXTERN INT32 GCPU_FreeSecureSlot(UINT32 u4SlotPtr);
EXTERN INT32 GCPU_SetSecureSlot(UINT32 u4SlotPtr, UINT8* pu1Data, UINT32 u4Size);
EXTERN UINT32 GCPU_CreateHandle(void);
EXTERN INT32 GCPU_ClostHandle(UINT32 u4Handle);

#endif  // GCPU_IF_H

