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
/***********************************************************************/
/*                                                                     */
/*  FILE DESCRIPTION                                                   */
/*                                                                     */
/*  $RCSfile: PI_Def.h,v $                                             */
/*                                                                     */
/***********************************************************************/

#ifndef _PI_DEF_H_INCLUDED_
#define _PI_DEF_H_INCLUDED_

#include "PD_Def.h"

/***********************************************************************/
/*              Defines                                                */
/***********************************************************************/
/*********** MPEG Format structure *************************************/
/* Polarities   - remove to x_tuner.h 2009/09/09 mtk40109 */
/*typedef enum
{
    POL_HIGH_ACTIVE,
    POL_LOW_ACTIVE
} SIG_POL_T;*/

typedef struct
{
    BOOL        fgGatedClock;
    BOOL        fgParallelOutput;
    BOOL        fgGatedOutput;
    BOOL        fgHeader;
    SIG_POL_T   eValidPolarity;
    SIG_POL_T   eSyncPolarity;
    SIG_POL_T   eClockPolarity;
    SIG_POL_T   eErrorPolarity;
} MPEG_FMT_T;

enum
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

/***********************************************************************/
#if 0
enum
{
    MOD_TYPE_BEGIN = 0,
    MOD_ATSC_8VSB = MOD_TYPE_BEGIN,
    MOD_J83B_64QAM,
    MOD_J83B_256QAM,
    MOD_J83B_AUTO_QAM,
    MOD_TYPE_END,
    MOD_OOB_TYPE_BEGIN,
    MOD_OOB_FDC = MOD_OOB_TYPE_BEGIN,
    MOD_OOB_RDC,
    MOD_OOB_TYPE_END,
    MOD_ANA_TYPE_BEGIN,
    MOD_ANA_TERR = MOD_ANA_TYPE_BEGIN,
    MOD_ANA_CABLE,
    MOD_ANA_TYPE_END
};
#endif

/***********************************************************************/
/*              For Analog Tuner                                       */
/***********************************************************************/
#define cANA_TUNER_BASE         (0x80)
#define cANA_E_TUNER_BASE       (cANA_TUNER_BASE / 4)

typedef enum
{
    LIST_TUNER_TYPE_BEGIN = 0,
    LIST_TUNER_PHILIPS_TD1336O = LIST_TUNER_TYPE_BEGIN,   // add new Inband tuner type after this line
    LIST_TUNER_SAMSUNG_DTVS203,
    LIST_TUNER_PHILIPS_FQD1236,
    LIST_TUNER_FORWARD_TAEH6,
    LIST_TUNER_PHILIPS_FQD1236E,
    LIST_TUNER_SAMSUNG_DTVS203EH,
    LIST_TUNER_ALPS_TDQU2,
    LIST_TUNER_ALPS_TDQU4X524A,
    LIST_TUNER_ALPS_TDQU8,
    LIST_TUNER_SAMSUNG_DTVS227CH,
    LIST_TUNER_TUNER_TYPE_END,                  // add new Inband tuner type above this line
    LIST_TUNER_TYPE_NULL = -1
} LIST_TUNER_ATSC_T;

/*Jackson TODO: need refine this enum, it's not good direct assign value to this enum, it should boundle tuner type define*/
typedef enum
{
    LIST_TUNER_cSAMSUNG_DTOS40A= 0x20,
    LIST_TUNER_cPANASONIC_ENG37E,
    LIST_TUNER_cALPS_TDQG4,
    LIST_TUNER_cPHILIPS_FQD1116ME,
    LIST_TUNER_cPHILIPS_TD1318AF,
    LIST_TUNER_cPANASONIC_ET55DHR,
    LIST_TUNER_cTCL_F07WP,
    LIST_TUNER_cPANASONIC_ENV57S,
    LIST_TUNER_cALPS_TDAG4B03A,
    LIST_TUNER_cNXP_FT2101,
    LIST_TUNER_cNuTune_FT2607,
} LIST_TUNER_EU_T;

/***********************************************************************/
enum __enumSysSubTotal
{
/* PAL Sub system */
    SYS_SUB_PAL_BG = 0,
//    SYS_SUB_PAL_G,
    SYS_SUB_PAL_DK,
//    SYS_SUB_PAL_DKCH,
//    SYS_SUB_PAL_DKEU,
    SYS_SUB_PAL_I,

    SYS_SUB_SECAM_L,
    SYS_SUB_SECAM_L1,

    SYS_SUB_NTSC_M,
    SYS_SUB_TOTAL
};


#define SCAN_END_FLAG           0xFF
#define SCAN_ROADMAP_SIZE       ((UINT8) SYS_SUB_TOTAL + 1)
#define TUNERARRAYDEPTH         ((UINT8) SYS_SUB_TOTAL)

/***********************************************************************/
/*              Common Macro                                           */
/***********************************************************************/
/************************ Bit Process *************************/
#define mcBITL(b)               (1UL << (b))
//#define mcBIT(b)                (1 << (b))
#define mcBIT(b)                mcBITL(b)
#define mcMASK(w)               (mcBIT(w) - 1)
#define mcMASKS(w, b)           (mcMASK(w) << (b))
#define mcFIELD(val, msk, pos)  (((val) << (pos)) & (msk))
#define mcGET_FIELD(var, mask, pos)     (((var) & (mask)) >> (pos))

#define mcSET_MASK(a, b)        ((a) |= (b))
#define mcCLR_MASK(a, b)        ((a) &= (~(b)))
#define mcCHK_MASK(a, b)        ((a) & (b))
#define mcSET_BIT(a, b)         mcSET_MASK(a, mcBIT(b))
#define mcCLR_BIT(a, b)         mcCLR_MASK(a, mcBIT(b))
#define mcCHK_BIT1(a, b)        ((a) & mcBIT(b))
#define mcCHK_BITM(a, b, m)     (((a) >> (b)) & (m))
#define mcCHK_BITS(a, b, w)     mcCHK_BITM(a, b, mcMASK(w))
#define mcTEST_BIT(a, b)        mcCHK_BIT1(a, b)

/***********************************************************************/
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

/***********************************************************************/
#define mcLOW_MASK(VarW)        ((UINT8) ((VarW) & mcMASK(8)))
#define mcHIGH_MASK(VarW)       ((UINT8) ((VarW) & ~mcMASK(8)))
#define mcLOW2HIGH(VarW)        (((UINT16) (VarW)) << 8)
/* (1234, 5678) -> 7834 */
#define mc2BYTE(ch1L, ch2H)     (mcLOW_MASK(ch1L) | mcLOW2HIGH(ch2H))
/* (1234) -> 0034 */
#define mcLOW_BYTE(VarW)        mcLOW_MASK(VarW)
/* (1234) -> 0012 */
#define mcHIGH_BYTE(VarW)       ((UINT8) (((UINT16) (VarW)) >> 8))

/***********************************************************************/
/* xxxHHHHH + LLLLLxxx */
#define mcUNSIGN_2BYTE(u8h, u8l, hmsk, lmsk)                \
    (((mcLOW2HIGH((u8h) & mcMASK(hmsk)) | (u8l))            \
        << (8 - (hmsk))) >> ((8 - (hmsk)) + (8 - (lmsk))))

#define mcSIGN_2BYTE(u8h, u8l, hmsk, lmsk)                  \
    (((INT16) ((mcLOW2HIGH((u8h) & mcMASK(hmsk)) | (u8l))     \
        << (8 - (hmsk)))) >> ((8 - (hmsk)) + (8 - (lmsk))))

/***********************************************************************/
/*              RISC-IF                                                */
/***********************************************************************/

#endif  // _PI_DEF_H_INCLUDED_
