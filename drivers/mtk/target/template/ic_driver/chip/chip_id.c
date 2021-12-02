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

#ifdef __TEST_MAIN__

typedef int INT32;
typedef unsigned int UINT32;

// MT538x & 5335~7
#define REG_BONDING             0x0000
    #define BOND_BIM_BD0        (1U << 0)
    #define BOND_BGA_BD1        (1U << 1)
    #define BOND_KEY1_BD2       (1U << 2)
    #define BOND_KEY2_BD3       (1U << 3)
    #define ALL_IC_BONDINGS     (0x7U)
#define REG_KEY_BONDING         0x0010

#define MT5335_BOND_ID          (BOND_BIM_BD0)
#define MT5336_BOND_ID          (BOND_BIM_BD0 | BOND_BGA_BD1)
#define MT5337_BOND_ID          (BOND_BIM_BD0 | BOND_BGA_BD1 | BOND_KEY1_BD2)
#define MT5380C_BOND_ID         (BOND_KEY1_BD2)
#define MT5380_BOND_ID          (0)
#define MT5381_BOND_ID          (BOND_BGA_BD1)
#define MT5382_BOND_ID          (BOND_BGA_BD1 | BOND_KEY1_BD2)

#else /* __TEST_MAIN__ */

#include "x_typedef.h"
#include "x_ckgen.h"
#include "x_bim.h"
#include "x_chip_id.h"
#include "x_gpio.h"

#endif /* __TEST_MAIN__ */

#define SZ_8MB      (8*1024*1024)
#define SZ_16MB     (16*1024*1024)
#define SZ_32MB     (32*1024*1024)
#define SZ_64MB     (64*1024*1024)
#define SZ_128MB    (128*1024*1024)

#define SW_CHIP_ID_mt5391           (0)
#define SW_CHIP_ID_mt5380c          (MT5380C_BOND_ID)
#define SW_CHIP_ID_mt5380c_16mb     (MT5380C_BOND_ID)
#define SW_CHIP_ID_mt5380c_32mb     (MT5380C_BOND_ID)
#define SW_CHIP_ID_mt5380           (MT5380_BOND_ID)
#define SW_CHIP_ID_mt5380_16mb      (MT5380_BOND_ID)
#define SW_CHIP_ID_mt5380_32mb      (MT5380_BOND_ID)
#define SW_CHIP_ID_mt5381           (MT5381_BOND_ID)
#define SW_CHIP_ID_mt5381_32mb      (MT5381_BOND_ID)
#define SW_CHIP_ID_mt5381_64mb      (MT5381_BOND_ID)
#define SW_CHIP_ID_mt5382           (MT5382_BOND_ID)
#define SW_CHIP_ID_mt5382_32mb      (MT5382_BOND_ID)
#define SW_CHIP_ID_mt5382_64mb      (MT5382_BOND_ID)
#define SW_CHIP_ID_mt5335           (MT5335_BOND_ID)
#define SW_CHIP_ID_mt5335_16mb      (MT5335_BOND_ID)
#define SW_CHIP_ID_mt5335_32mb      (MT5335_BOND_ID)
#define SW_CHIP_ID_mt5336           (MT5336_BOND_ID)
#define SW_CHIP_ID_mt5336_32mb      (MT5336_BOND_ID)
#define SW_CHIP_ID_mt5336_64mb      (MT5336_BOND_ID)
#define SW_CHIP_ID_mt5337           (MT5337_BOND_ID)
#define SW_CHIP_ID_mt5337_32mb      (MT5337_BOND_ID)
#define SW_CHIP_ID_mt5337_64mb      (MT5337_BOND_ID)

#ifndef SW_CHIP_ID
#define SW_CHIP_ID                  (MT5382_BOND_ID)
#endif /* SW_CHIP_ID */

#ifdef __TEST_MAIN__
static INT32 i4SwChipId = 0;
static INT32 i4HwChipId = 0;
#define CHIP_GetSwChipId()          (i4SwChipId)
#else /* __TEST_MAIN__ */
UINT32 CHIP_GetSwChipId()
{
    return (UINT32)SW_CHIP_ID; // (UINT32)((UINT16)(~(SW_CHIP_ID)) << 8);
}
#endif /* __TEST_MAIN__ */

#ifndef __TEST_MAIN__

#ifdef FAKE_CHIP_ID
UINT32 CHIP_GetFakeId(UINT32 u4Chip)
{
    UINT32 u4ChipId;

    switch(u4Chip) {
    case 0x5380C:
        u4ChipId = MT5380C_BOND_ID; break;
    case 0x5380:
        u4ChipId = MT5380_BOND_ID; break;
    case 0x5381:
        u4ChipId = MT5381_BOND_ID; break;
    case 0x5382:
        u4ChipId = MT5382_BOND_ID; break;
    case 0x5335:
        u4ChipId = MT5335_BOND_ID; break;
    case 0x5336:
        u4ChipId = MT5336_BOND_ID; break;
    case 0x5337:
        u4ChipId = MT5337_BOND_ID; break;
    default:
        u4ChipId = MT5380C_BOND_ID; break;
    }
    CKGEN_WRITE32(REG_KEY_BONDING, 0xffff);
    u4ChipId |= (CKGEN_READ32(REG_BONDING) & BOND_KEY2_BD3) ? BOND_KEY2_BD3 : 0;
    CKGEN_WRITE32(REG_KEY_BONDING, 0);
    return u4ChipId;
}
#endif /* FAKE_CHIP_ID */
#endif /* __TEST_MAIN__ */

static INT32 _ChipChkVer(void)
{
    UINT32 u4Sw, u4Hw;

    u4Sw = CHIP_GetSwChipId();
#ifdef __TEST_MAIN__
    u4Hw = i4HwChipId;
#else /* __TEST_MAIN__ */
    CKGEN_WRITE32(REG_KEY_BONDING, 0xffff);
    u4Hw = ALL_IC_BONDINGS & CKGEN_READ32(REG_BONDING);
    CKGEN_WRITE32(REG_KEY_BONDING, 0);
#endif /* __TEST_MAIN__ */

    if (u4Hw == u4Sw) { return 1; }
    if (u4Hw == MT5382_BOND_ID) { return 1; }
    if (((u4Hw & u4Sw) & 1) && (u4Hw > u4Sw)) { return 1; }
    if (((u4Hw == MT5381_BOND_ID) && ((~u4Sw) & 1)) && (u4Sw != MT5382_BOND_ID)) { return 1; }
    if ((!u4Hw) && (u4Sw == MT5380C_BOND_ID)) { return 1; }

#ifndef __TEST_MAIN__
#ifndef CC_DRIVER_DEMO
    CKGEN_WRITE32(REG_CPU_CKCFG, 0x105); // set CPU 300MHz.
#endif
#endif /* __TEST_MAIN__ */

    return 0;
}

#ifndef __TEST_MAIN__

static INT32 g_i4ChipCount = 0;

#define INLINE_CHIP_FUNC(func, statement)           \
                                                    \
    UINT32 func()                                    \
    {                                               \
        UINT32 i4Val;                                \
                                                    \
        if ((g_i4ChipCount & 0xf) == 0)             \
        {                                           \
            UNUSED(_ChipChkVer());                  \
        }                                           \
        g_i4ChipCount++;                            \
        CKGEN_WRITE32(REG_KEY_BONDING, 0xffff);     \
        i4Val = statement;                          \
        CKGEN_WRITE32(REG_KEY_BONDING, 0);          \
        return i4Val;                               \
    }

#ifdef FAKE_CHIP_ID
EXTERN UINT32 CHIP_GetFakeId(UINT32 u4Chip);

INLINE_CHIP_FUNC(GET_CHIPID, (CHIP_GetFakeId(FAKE_CHIP_ID) & ALL_P_IC_BONDINGS))
INLINE_CHIP_FUNC(IS_MT5335, (CHIP_GetFakeId(FAKE_CHIP_ID) == MT5335_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5336, (CHIP_GetFakeId(FAKE_CHIP_ID) == MT5336_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5337, (CHIP_GetFakeId(FAKE_CHIP_ID) == MT5337_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5380C, (CHIP_GetFakeId(FAKE_CHIP_ID) == MT5380C_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5380, (CHIP_GetFakeId(FAKE_CHIP_ID) == MT5380_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5381, (CHIP_GetFakeId(FAKE_CHIP_ID) == MT5381_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5382, (CHIP_GetFakeId(FAKE_CHIP_ID) == MT5382_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5335P, (CHIP_GetFakeId(FAKE_CHIP_ID) == MT5335P_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5336P, (CHIP_GetFakeId(FAKE_CHIP_ID) == MT5336P_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5337P, (CHIP_GetFakeId(FAKE_CHIP_ID) == MT5337P_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5380CP, (CHIP_GetFakeId(FAKE_CHIP_ID) == MT5380CP_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5380P, (CHIP_GetFakeId(FAKE_CHIP_ID) == MT5380P_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5381P, (CHIP_GetFakeId(FAKE_CHIP_ID) == MT5381P_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5382P, (CHIP_GetFakeId(FAKE_CHIP_ID) == MT5382P_BOND_ID))
#else
INLINE_CHIP_FUNC(GET_CHIPID, (CKGEN_READ32(REG_BONDING) & ALL_P_IC_BONDINGS))
INLINE_CHIP_FUNC(IS_MT5335, (GET_CHIPID() == MT5335_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5336, (GET_CHIPID() == MT5336_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5337, (GET_CHIPID() == MT5337_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5380C, (GET_CHIPID() == MT5380C_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5380, (GET_CHIPID() == MT5380_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5381, (GET_CHIPID() == MT5381_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5382, (GET_CHIPID() == MT5382_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5335P, (GET_CHIPID() == MT5335P_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5336P, (GET_CHIPID() == MT5336P_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5337P, (GET_CHIPID() == MT5337P_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5380CP, (GET_CHIPID() == MT5380CP_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5380P, (GET_CHIPID() == MT5380P_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5381P, (GET_CHIPID() == MT5381P_BOND_ID))
INLINE_CHIP_FUNC(IS_MT5382P, (GET_CHIPID() == MT5382P_BOND_ID))

INT32 IS_MultiMediaSupport()
{
    UINT32 u4Val;

    CKGEN_WRITE32(REG_KEY_BONDING, 0xffff);
    u4Val = CKGEN_READ32(REG_BONDING);
    CKGEN_WRITE32(REG_KEY_BONDING, 0);
    return (u4Val & BOND_MULTIMEDIA);
}

INT32 IS_SurroundSupport()
{
    INT32 i4Val, i4OrgVal;

//    GPIO_Enable(83, &i4Val);
    i4OrgVal = i4Val;
    i4Val = 0;
//    GPIO_Enable(83, &i4Val);
//    i4Val = GPIO_Input(83);
//    GPIO_Enable(83, &i4OrgVal);
    return i4Val;
}

INT32 IS_MHEG5Support()
{
    UINT32 u4Val;

    CKGEN_WRITE32(REG_KEY_BONDING, 0xffff);
    u4Val = CKGEN_READ32(REG_BONDING);
    CKGEN_WRITE32(REG_KEY_BONDING, 0);
    return (u4Val & BOND_MHEG5);
}

#endif

#define PAD0_QFP            (1U << 29)  // LQFP package

void CHIP_CheckBoard()
{
    UINT32 u4Val;

    u4Val = CKGEN_READ32(0x0338);
    u4Val |= 0x40;
    CKGEN_WRITE32(0x338, u4Val);            // VCXO clock enable for Analog IP

    u4Val = CKGEN_READ32(REG_PADPU_CFG0);
    CKGEN_WRITE32(REG_KEY_BONDING, 0xffff);
    if (CKGEN_READ32(REG_BONDING) & BOND_BGA_BD1)
    {
        u4Val |= (PAD0_QFP);
    }
    else
    {
        u4Val &= ~(PAD0_QFP);
    }
    CKGEN_WRITE32(REG_PADPU_CFG0, u4Val);   // Check QFP
    CKGEN_WRITE32(REG_KEY_BONDING, 0);
    UNUSED(_ChipChkVer());
}

#else /* __TEST_MAIN__ */

char* aStrChipName[] = {
    "5380C",
    "5380",
    "5381",
    "5382",
    "5335",
    "5336",
    "5337",
};

int ai4ChipArray[] = {
    MT5380C_BOND_ID,
    MT5380_BOND_ID,
    MT5381_BOND_ID,
    MT5382_BOND_ID,
    MT5335_BOND_ID,
    MT5336_BOND_ID,
    MT5337_BOND_ID,
};

#define ARRAY_NUM   (sizeof(ai4ChipArray)/sizeof(int))

int main(int argc, char **argv)
{
    int i, j;

    printf("\n\t          ");
    for (i=0; i<ARRAY_NUM; i++)
    {
        printf("%s ", aStrChipName[i]);
    }
    printf("\n");
    for (j=0; j<ARRAY_NUM; j++)
    {
        printf("Sw%s 0x%02x\t| ", aStrChipName[j], ai4ChipArray[j]);
        for (i=0; i<ARRAY_NUM; i++)
        {
            i4HwChipId = ai4ChipArray[i];
            i4SwChipId = ai4ChipArray[j];
            printf("%4d ", _ChipChkVer());
        }
        printf("\n");
    }
    printf("\n");
    return 0;
}

#endif /* __TEST_MAIN__ */


