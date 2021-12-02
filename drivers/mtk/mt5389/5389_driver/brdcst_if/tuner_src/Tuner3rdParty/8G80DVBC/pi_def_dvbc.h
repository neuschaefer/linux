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

#ifndef _PI_DEF_DVBC_H_
#define _PI_DEF_DVBC_H_

#include "PI_Def.h"
#include "pd_def_dvbc.h"
/*---------------------------------------------------------------------*/
/*                  defines, typedefs                                  */
/*---------------------------------------------------------------------*/
/*********** MPEG Format structure *************************************/

/*enum
{
    DEMOD_TS_FORMAT_VALID_LOW = 0,
    DEMOD_TS_FORMAT_ERR_LOW,
    DEMOD_TS_FORMAT_SYNC_LOW,
    DEMOD_TS_FORMAT_PARALLEL,
    DEMOD_TS_FORMAT_CLK_CONT,
    DEMOD_TS_FORMAT_DATA_FALL,
    DEMOD_TS_FORMAT_OUTPUT_EN,
    DEMOD_TS_FORMAT_SIZE
};                                                 
*/

enum
{
    PORT_CFG_MTK = 0,
    PORT_CFG_CTMR,
    PORT_CFG_COM,
    PORT_CFG_USB,
    PORT_CFG_SIZE
};

typedef enum
{
    DEMOD_ON = 0,
    DEMOD_OFF
} DEMOD_ONOFF_MODE_T;

enum
{
	SAW_BW_6M = 0,
	SAW_BW_7M,
	SAW_BW_8M,
    SAW_BW_5M
};
/*
enum
{
	cIF_TARGET_POWER_51_2 = 0,
	cIF_TARGET_POWER_42_36
};
*/

enum
{
    LOG_TAG_FFT = 0,
    LOG_TAG_GI,
    LOG_TAG_CR,
    LOG_TAG_SNR,
    LOG_TAG_PEC_PER,
    LOG_TAG_PEC_ESR,
    LOG_TAG_PEC_PKT_ERR,
    LOG_TAG_PEC_PKT_TOT,
    LOG_TAG_PEC_COR_BIT,
    LOG_TAG_ACQ_CNT,	
    LOG_TAG_RST_CNT,
    LOG_TAG_AR_COEF,
    LOG_TAG_END
};
/*
#define	AGC_IF_POWER_LEVEL__6dBm  0
#define	AGC_IF_POWER_LEVEL__3dBm  1
*/
enum
{
    cIF_TARGET_LEVEL_0CCC = 0,
    cIF_TARGET_LEVEL_0A97,
    cIF_TARGET_LEVEL_09CC,
    cIF_TARGET_LEVEL_07CC,
    cIF_TARGET_LEVEL_0CCC_0D36
};
    

enum
{
	constellation_qam4_dvbc = 0,
	constellation_qam4NR_dvbc,
	constellation_qam16_dvbc,
	constellation_qam32_dvbc,
	constellation_qam64_dvbc,
	constellation_qam128_dvbc,
	constellation_qam256_dvbc,
	constellation_AutoQam_dvbc,
	constellation_UNKONW_dvbc
	
};


#define ccTUNER_LOCK_CONF_THRESHOLD      0
#define cALC_ADC_INPUT_SWING             2  //Volt
#define cALC_ADC_COMM_VOLT               1.2
#define cALC_ADC_BIAS                    (cALC_ADC_COMM_VOLT - cALC_ADC_INPUT_SWING/2)

/*------------------------------------------------------------*/
/*                  macros, defines, typedefs, enums          */
/*------------------------------------------------------------*/
/************************ Bit Process *************************/
#undef mcBITL
#define mcBITL(b)               (1UL << (b))
//#define mcBIT(b)                (1 << (b))
#define mcBIT(b)                mcBITL(b)
#define mcMASK(w)               (mcBIT(w) - 1)
#define mcMASKS(w, b)           (mcMASK(w) << (b))
//#define mcCLR_BIT_C(a)          ((-1 - (Bit_#a)) & 0xFFFF)
#define mcCLR_BIT_C(a)          ((~Bit_##a) & 0xFFFF)

#define mcSET_MASK(a, b)        ((a) |= (b))
#define mcCLR_MASK(a, b)        ((a) &= (~(b)))
//#define mcCLR_SET_MASK(a, b, c)  ((a) = ((a) & (~(b))) | (c))
//          \_ same as mcCLR_MASK + mcSET_MASK at optimized code
#define mcSET_BIT(a, b)         mcSET_MASK(a, mcBIT(b))
#define mcCLR_BIT(a, b)         mcCLR_MASK(a, mcBIT(b))
#define mcCHK_BIT1(a, b)        ((a) & mcBIT(b))
#define mcCHK_BITM(a, b, m)     (((a) >> (b)) & (m))
#define mcCHK_BITS(a, b, w)     mcCHK_BITM(a, b, mcMASK(w))
//#define mcTEST_BIT(a, b)        mcCHK_BITM(a, b, 1)
#define mcTEST_BIT(a, b)        mcCHK_BIT1(a, b)
#define mcCHG_BIT(a, b)         ((a) ^= mcBIT(b))

#undef mcSET_FIELD0
#define mcSET_FIELD0(var, value, mask, pos) mcSET_MASK(var, ((value) << (pos)) & (mask))

#define mcSET_FIELD(var, value, mask, pos)  \
{                                           \
    mcCLR_MASK(var, mask);                  \
    mcSET_FIELD0(var, value, mask, pos);    \
}

#define mcGET_FIELD(var, mask, pos)     (((var) & (mask)) >> (pos))

#ifndef BIT0
#define BIT0                    mcBIT(0)
#define BIT1                    mcBIT(1)
#define BIT2                    mcBIT(2)
#define BIT3                    mcBIT(3)
#define BIT4                    mcBIT(4)
#define BIT5                    mcBIT(5)
#define BIT6                    mcBIT(6)
#define BIT7                    mcBIT(7)
#endif

#undef mcLOW_MASK
#define mcLOW_MASK(u16)         ((UCHAR) ((u16) & mcMASK(8)))

#undef mcHIGH_MASK
#define mcHIGH_MASK(u16)        ((UCHAR) ((u16) & ~mcMASK(8)))

#undef mcLOW2HIGH
#define mcLOW2HIGH(u16)         (((UINT16) (u16)) << 8)
/* (1234, 5678) -> 7834 */
#define mc2BYTE(ch1L, ch2H)     (mcLOW_MASK(ch1L) | mcLOW2HIGH(ch2H))
/* (1234) -> 0034 */
//#define mcLOW_BYTE(u16)         ((UCHAR)(u16))

#undef mcLOW_BYTE
#define mcLOW_BYTE(u16)         mcLOW_MASK(u16)
/* (1234) -> 0012 */

#undef mcHIGH_BYTE
#define mcHIGH_BYTE(u16)        ((UCHAR) (((UINT16) (u16)) >> 8))

/* xxxHHHHH + LLLLLxxx */
#define mcUNSIGN_2BYTE(u8h, u8l, hmsk, lmsk)                \
    (((mcLOW2HIGH((u8h) & mcMASK(hmsk)) | (u8l))            \
        << (8 - (hmsk))) >> ((8 - (hmsk)) + (8 - (lmsk))))

#define mcSIGN_2BYTE(u8h, u8l, hmsk, lmsk)                  \
    (((INT16) ((mcLOW2HIGH((u8h) & mcMASK(hmsk)) | (u8l))     \
        << (8 - (hmsk)))) >> ((8 - (hmsk)) + (8 - (lmsk))))

#define mcSET_MASKS(par)                mcMASKS(cw##par, cp##par)
#define mcFIELD_CMB(val, par)           mcFIELD(val, cm##par, cp##par)
#define mcGET_FIELD_CMB(var, par)       mcGET_FIELD(var, cm##par, cp##par)
#define mcSET_FIELD_CMB(var, val, par)  mcSET_FIELD(var, val, cm##par, cp##par)

#endif  // _PI_DEF_DVBT_H_
