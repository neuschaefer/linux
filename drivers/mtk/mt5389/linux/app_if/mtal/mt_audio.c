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
 * $Author: kai.wang $
 * $Date: 2012/12/14 $
 * $RCSfile: mt_audio.c,v $
 * $Revision: #8 $
 *
 *---------------------------------------------------------------------------*/

/** @file mt_audio.c
 *  MTAL kernel driver implementation: Audio decoder
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mtauddec.h"
#include "x_assert.h"
#include "aud_if.h"
#include "aud_cfg.h"
#include "x_hal_5381.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define MT_AUD_DEC_MAX                     (3)
#define AUD_CHL_DELAY_RANGE                (400)
#define VOL_SHM_0_DB                       (0x20000)
#define AUD_CLIP_CH_SELECT                 (3)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

typedef struct _CHANNEL_INFO_T_
{
    UINT16      u2Delay;
    UINT8       u1Vol;
    BOOL        fgMute;
    INT32       i4ChlVolGain;
}CHANNEL_INFO_T;

typedef struct _EQ_BAND_SETTING_T_
{
    INT16       i2Band0;
    INT16       i2Band1;
    INT16       i2Band2;
    INT16       i2Band3;
    INT16       i2Band4;
    INT16       i2Band5;
    INT16       i2Band6;
}EQ_BAND_SETTING;

typedef struct _AUD_SETTING_T_
{
    MTAUDDEC_AC3_COMP_MODE_T    eCompMode;
    MTAUDDEC_AC3_DOWNMIX_MODE   eDmxMode;
    MTAUDDEC_AC3_DUALMONO_MODE  eDualMonoMode;
    UINT8                       u1DrcRange;
    MTAUDDEC_SOUND_MODE_T       eOutputMode;
    MTAUDDEC_SIF_STANDARD_T     eSetSifMode;
    MTAUDDEC_PCM_SETTING_T      rPcmSetting;
    UINT8                       u1MasterVol;
    BOOL                        fgAvc;
    BOOL                        fgBbe;
    BOOL                        fgSrs;
    BOOL                        fgVirSurr;
    BOOL                        fgBassBoost;
    INT16                       i2Bass;
    BOOL                        fgTrebleBoost;
    INT16                       i2Treble;
    BOOL                        fgEqEnable;
    EQ_BAND_SETTING             rEqBandSetting;
    MTAUDDEC_INPUT_ID_T         eAvMuxId;
    MTAUDDEC_IEC_T              eIecFmt;
    BOOL                        fgIecEnable;
    BOOL                        fgIecCopyright;
    UINT8                       u1IecCategoryCode;
    INT16                       i2BalanceLeftInDB;
    INT16                       i2BalanceRightInDB;
    INT16                       i2BypassMastVolumeInDB;
    MTAUDDEC_MODE_T             eAudMode;
    MTAUDDEC_SKP_OUTPUT_MODE_T  eSpkOutputMode;
}AUD_SETTING_T;

typedef struct _AUD_DEC_INFOR_T_
{
    BOOL                        fgIssuePlay;
    BOOL                        fgOnPlaying;
    BOOL                        fgPause;
    BOOL                        fgAvSync;
    MTAUDDEC_FMT_T              eDecFmt;
    MTAUDDEC_DEC_STREAM_FROM_T  eStrSrc;
    AUD_SETTING_T *             prAudSetting;
}AUD_DEC_INFOR_T;

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

static HANDLE_T         _ahMtAudSema[AUD_DEC_MAX];

#define AUD_POINTER_VALIDATE(pPara)          \
    if (pPara == NULL)                       \
    {                                        \
        return MTR_ERR_INV;                  \
    }                                        \
                                       
                                   \
 
#define MT_AUD_SEMA_LOCK(u1DecId)   VERIFY(x_sema_lock(_ahMtAudSema[u1DecId],\
                                                X_SEMA_OPTION_WAIT) == OSR_OK)

#define MT_AUD_SEMA_UNLOCK(u1DecId)    VERIFY(x_sema_unlock(_ahMtAudSema[u1DecId]) == OSR_OK)

#define MT_AUD_SEMA_INIT(u1DecId)  VERIFY(x_sema_create(&_ahMtAudSema[u1DecId],\
                                           X_SEMA_TYPE_MUTEX,\
                                           X_SEMA_STATE_UNLOCK) == OSR_OK)


#define VOL_CTL_SEMA_LOCK()      VERIFY(x_sema_lock(_hVolCtlSema, X_SEMA_OPTION_WAIT) == OSR_OK)
#define VOL_CTL_SEMA_UNLOCK()    VERIFY(x_sema_unlock(_hVolCtlSema) == OSR_OK)
#define VOL_CTL_SEMA_INIT()      VERIFY(x_sema_create(&_hVolCtlSema, X_SEMA_TYPE_BINARY, \
                                                X_SEMA_STATE_UNLOCK) == OSR_OK);

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

static AUD_DEC_INFOR_T  _arAudDec[AUD_DEC_MAX];
static AUD_SETTING_T    _rAudSetting;
static INT16            _ai2ChlVolInDB[MTAUDDEC_CH_NUM];
#ifdef CC_AUD_VBASS_SUPPORT
static UINT32           _u4VbOutputGain = 0x20000;
static UINT32           _u4VbBassGain = 0x20000;
#endif
static BOOL             _afgCbReg[MTAUDDEC_CB_FUNC_ID_MAX];
static BOOL             _fgDrvCbReg = FALSE;
static UINT32           _hHandler = 0x9;  // Just a magic number


static HANDLE_T _hVolCtlSema = (HANDLE_T)NULL;

// Negtive dB table, in steps of 0.25 dB
// 0dB, -0.25dB, -0.5dB, -0.75dB, -1dB, -1.25dB,.....
const UINT32 _au4NegDBTable[] =
{
    0x00020000, 0x0001f179, 0x0001e35b, 0x0001d5a5, 0x0001c852,
    0x0001bb5f, 0x0001aecb, 0x0001a292, 0x000196b2, 0x00018b28,
    0x00017ff2, 0x0001750d, 0x00016a77, 0x0001602f, 0x00015631,
    0x00014c7b, 0x0001430c, 0x000139e2, 0x000130fa, 0x00012853,
    0x00011feb, 0x000117bf, 0x00010fd0, 0x00010819, 0x0001009b,
    0x0000f953, 0x0000f240, 0x0000eb61, 0x0000e4b3, 0x0000de36,
    0x0000d7e8, 0x0000d1c8, 0x0000cbd4, 0x0000c60c, 0x0000c06d,
    0x0000baf8, 0x0000b5aa, 0x0000b082, 0x0000ab80, 0x0000a6a2,
    0x0000a1e8, 0x00009d50, 0x000098da, 0x00009483, 0x0000904d,
    0x00008c35, 0x0000883a, 0x0000845d, 0x0000809b, 0x00007cf5,
    0x0000796a, 0x000075f8, 0x0000729f, 0x00006f5e, 0x00006c35,
    0x00006923, 0x00006628, 0x00006342, 0x00006071, 0x00005db4,
    0x00005b0c, 0x00005876, 0x000055f4, 0x00005384, 0x00005125,
    0x00004ed8, 0x00004c9b, 0x00004a6f, 0x00004852, 0x00004645,
    0x00004446, 0x00004256, 0x00004074, 0x00003ea0, 0x00003cd9,
    0x00003b1f, 0x00003972, 0x000037d1, 0x0000363b, 0x000034b1,
    0x00003333, 0x000031bf, 0x00003055, 0x00002ef6, 0x00002da1,
    0x00002c56, 0x00002b14, 0x000029db, 0x000028ab, 0x00002784,
    0x00002665, 0x0000254e, 0x0000243f, 0x00002337, 0x00002238,
    0x0000213f, 0x0000204e, 0x00001f63, 0x00001e7f, 0x00001da1,
    0x00001cca, 0x00001bf9, 0x00001b2e, 0x00001a68, 0x000019a9,
    0x000018ee, 0x00001839, 0x00001789, 0x000016de, 0x00001638,
    0x00001597, 0x000014fa, 0x00001462, 0x000013ce, 0x0000133e,
    0x000012b2, 0x0000122a, 0x000011a6, 0x00001126, 0x000010a9,
    0x00001030, 0x00000fbb, 0x00000f49, 0x00000ed9, 0x00000e6e,
    0x00000e05, 0x00000d9f, 0x00000d3c, 0x00000cdc, 0x00000c7e,
    0x00000c24, 0x00000bcc, 0x00000b76, 0x00000b23, 0x00000ad2,
    0x00000a83, 0x00000a37, 0x000009ed, 0x000009a4, 0x0000095e,
    0x0000091a, 0x000008d8, 0x00000898, 0x0000085a, 0x0000081d,
    0x000007e2, 0x000007a9, 0x00000771, 0x0000073b, 0x00000706,
    0x000006d3, 0x000006a2, 0x00000672, 0x00000643, 0x00000615,
    0x000005e9, 0x000005be, 0x00000594, 0x0000056c, 0x00000544,
    0x0000051e, 0x000004f9, 0x000004d5, 0x000004b2, 0x00000490,
    0x0000046f, 0x0000044e, 0x0000042f, 0x00000411, 0x000003f3,
    0x000003d6, 0x000003bb, 0x0000039f, 0x00000385, 0x0000036c,
    0x00000353, 0x0000033b, 0x00000323, 0x0000030c, 0x000002f6,
    0x000002e1, 0x000002cc, 0x000002b7, 0x000002a4, 0x00000290,
    0x0000027e, 0x0000026c, 0x0000025a, 0x00000249, 0x00000238,
    0x00000228, 0x00000219, 0x00000209, 0x000001fb, 0x000001ec,
    0x000001de, 0x000001d1, 0x000001c3, 0x000001b7, 0x000001aa,
    0x0000019e, 0x00000192, 0x00000187, 0x0000017c, 0x00000171,
    0x00000166, 0x0000015c, 0x00000152, 0x00000149, 0x0000013f,
    0x00000136, 0x0000012e, 0x00000125, 0x0000011d, 0x00000115,
    0x0000010d, 0x00000105, 0x000000fe, 0x000000f6, 0x000000ef,
    0x000000e9, 0x000000e2, 0x000000dc, 0x000000d5, 0x000000cf,
    0x000000c9, 0x000000c4, 0x000000be, 0x000000b9, 0x000000b3,
    0x000000ae, 0x000000a9, 0x000000a5, 0x000000a0, 0x0000009b,
    0x00000097, 0x00000093, 0x0000008e, 0x0000008a, 0x00000086,
    0x00000083, 0x0000007f, 0x0000007b, 0x00000078, 0x00000074,
    0x00000071, 0x0000006e, 0x0000006b, 0x00000068, 0x00000065,
    0x00000062, 0x0000005f, 0x0000005c, 0x0000005a, 0x00000057,
    0x00000055, 0x00000052, 0x00000050, 0x0000004e, 0x0000004b,
    0x00000049, 0x00000047, 0x00000045, 0x00000043, 0x00000041,
    0x0000003f, 0x0000003e, 0x0000003c, 0x0000003a, 0x00000038,
    0x00000037, 0x00000035, 0x00000034, 0x00000032, 0x00000031,
    0x0000002f, 0x0000002e, 0x0000002d, 0x0000002b, 0x0000002a,
    0x00000029, 0x00000028, 0x00000027, 0x00000026, 0x00000024,
    0x00000023, 0x00000022, 0x00000021, 0x00000020, 0x0000001f,
    0x0000001f, 0x0000001e, 0x0000001d, 0x0000001c, 0x0000001b,
    0x0000001a, 0x0000001a, 0x00000019, 0x00000018, 0x00000017,
    0x00000017, 0x00000016, 0x00000016, 0x00000015, 0x00000014,
    0x00000014, 0x00000013, 0x00000013, 0x00000012, 0x00000011,
    0x00000011, 0x00000010, 0x00000010, 0x00000010, 0x0000000f,
    0x0000000f, 0x0000000e, 0x0000000e, 0x0000000d, 0x0000000d,
    0x0000000d, 0x0000000c, 0x0000000c, 0x0000000c, 0x0000000b,
    0x0000000b, 0x0000000b, 0x0000000a, 0x0000000a, 0x0000000a,
    0x00000009, 0x00000009, 0x00000009, 0x00000009, 0x00000008,
    0x00000008, 0x00000008, 0x00000008, 0x00000007, 0x00000007,
    0x00000007, 0x00000007, 0x00000006, 0x00000006, 0x00000006,
    0x00000006, 0x00000006, 0x00000006, 0x00000005, 0x00000005,
    0x00000005, 0x00000005, 0x00000005, 0x00000005, 0x00000004,
    0x00000004, 0x00000004, 0x00000004, 0x00000004, 0x00000004,
    0x00000004, 0x00000004, 0x00000003, 0x00000003, 0x00000003,
    0x00000003, 0x00000003, 0x00000003, 0x00000003, 0x00000003,
    0x00000003, 0x00000003, 0x00000002, 0x00000002, 0x00000002,
    0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002,
    0x00000002, 0x00000002, 0x00000002, 0x00000002, 0x00000002,
    0x00000002, 0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000001, 0x00000001, 0x00000001, 0x00000001, 0x00000001,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
    0x00000000, 0x00000000, 0x00000000
};

// Postive dB table, in steps of 0.25 dB
// 0dB, 0.25dB, 0.5dB, 0.75dB, 1dB, 1.25dB,.....
const UINT32 _au4PosDBTable[] =
{
    0x00020000, 0x00020ef3, 0x00021e56, 0x00022e2c, 0x00023e79,
    0x00024f3f, 0x00026083, 0x00027248, 0x00028491, 0x00029764,
    0x0002aac3, 0x0002beb3, 0x0002d338, 0x0002e856, 0x0002fe12,
    0x00031471, 0x00032b77, 0x00034329, 0x00035b8c, 0x000374a5,
    0x00038e7a, 0x0003a910, 0x0003c46d, 0x0003e097, 0x0003fd93,
    0x00041b67, 0x00043a1b, 0x000459b4, 0x00047a39, 0x00049bb2,
    0x0004be24, 0x0004e198, 0x00050615, 0x00052ba3, 0x0005524a,
    0x00057a12, 0x0005a303, 0x0005cd26, 0x0005f884, 0x00062526,
    0x00065316, 0x0006825d, 0x0006b305, 0x0006e51a, 0x000718a5,
    0x00074db1, 0x00078449, 0x0007bc7a, 0x0007f64f, 0x000831d4,
    0x00086f16, 0x0008ae21, 0x0008ef05, 0x000931cd, 0x00097688,
    0x0009bd46, 0x000a0614, 0x000a5102, 0x000a9e20, 0x000aed7f,
    0x000b3f30, 0x000b9342, 0x000be9ca, 0x000c42d8, 0x000c9e80,
    0x000cfcd5, 0x000d5deb, 0x000dc1d7, 0x000e28ae, 0x000e9286,
    0x000eff75, 0x000f6f92, 0x000fe2f5, 0x001059b7, 0x0010d3f1,
    0x001151bc, 0x0011d334, 0x00125873, 0x0012e197, 0x00136ebc,
    0x00140000, 0x00149581, 0x00152f61, 0x0015cdbe, 0x001670bc,
    0x0017187b, 0x0017c521, 0x001876d2, 0x00192db2, 0x0019e9ea,
    0x001aaba1, 0x001b7300, 0x001c4031, 0x001d1360, 0x001decb9,
    0x001ecc6c, 0x001fb2a7, 0x00209f9a, 0x0021937a, 0x00228e78,
    0x002390ca, 0x00249aa7, 0x0025ac48, 0x0026c5e6, 0x0027e7be,
    0x0029120c, 0x002a4510, 0x002b810b, 0x002cc640, 0x002e14f4,
    0x002f6d6e, 0x0030cff8, 0x00323cdb, 0x0033b467, 0x003536e9,
    0x0036c4b5, 0x00385e1e, 0x003a037c, 0x003bb528, 0x003d737e,
    0x003f3edc, 0x004117a4, 0x0042fe3b, 0x0044f306, 0x0046f672,
    0x004908ea, 0x004b2ae0, 0x004d5cc6, 0x004f9f16, 0x0051f248,
    0x005456dc, 0x0056cd53, 0x00595633, 0x005bf205, 0x005ea158,
    0x006164bc, 0x00643cc9, 0x00672a18, 0x006a2d48, 0x006d46fd,
    0x007077e0, 0x0073c09d, 0x007721e6, 0x007a9c74, 0x007e3104,
    0x0081e056, 0x0085ab35, 0x0089926d, 0x008d96d2, 0x0091b940,
    0x0095fa95, 0x009a5bb9, 0x009edd9b, 0x00a3812d, 0x00a8476e,
    0x00ad315f, 0x00b2400c, 0x00b77487, 0x00bccfea, 0x00c25359,
    0x00c80000
};

static UINT32 _u4PosDBTableSz = sizeof(_au4PosDBTable)/sizeof(UINT32);
static UINT32 _u4NegDBTableSz = sizeof (_au4NegDBTable)/sizeof(UINT32);

//-----------------------------------------------------------------------------
// Export functions
//-----------------------------------------------------------------------------

#ifdef CC_MT5363
extern AUD_TYPE_T DSP_GetAudChlInfo(UINT8 u1DspId,UINT8 u1DecId);
extern void AUD_DspSetDetectDefaultMode(UINT8 u1DspId,UINT8 u1Mode);
#else
extern AUD_TYPE_T DSP_GetAudChlInfo(UINT8 u1DecId);
extern void AUD_DspSetDetectDefaultMode(UINT8 u1Mode);
extern UINT8 AUD_GetAC3_EAC3_Info(UINT8 u1DecId);
extern UINT8 AUD_GetAacVersion(UINT8 u1DecId);
extern UINT8 AUD_GetAacTSFormat(UINT8 u1DecId);
extern void AUD_DspAC3DRCRange100(UINT8 uDRCLevel,UINT8 u1DecId);
extern void AUD_DspGetMpegAudInfowithLayer(UINT8 u1DecId, AUD_DEC_INFO_T * prAudInfo, UINT8 *prMPEGLayer);
extern void AudLoadAudioClip(UINT8 u1DecId, UINT32 u1BufferSize, UINT32 u1ClipBufferPointer, AUD_FMT_T u1DecFmt, UINT32 u1ReaptNumber );
extern void AUD_DeleteAudioClip(void);
extern AUD_FMT_T _AudAtvFmtDetection(void);
extern void AUD_DspChannelDelayNoUop(UINT16 u2Delay, AUD_CH_T eChIndex, UINT8 uDecIndex);

#endif
extern void DSP_SetDetTvSrcMode(TV_AUD_SYS_T eAudSys);
extern TV_AUD_SYS_T DSP_GetDetTvSrcMode(TV_AUD_SYS_T eAudSys);
extern void vDspSetWordLength(UINT8 u1WordLen);
extern UINT8 DspGetWordLength(void);
extern void DSP_SetDualDecMode(BOOL fgEnable);
#if MIXSOUND_MEMPLAY_USER_SPACE_BUFFER
extern UINT8* _MTAUDDEC_GetMixSoundBufAddr(void);
extern void _MTAUDDEC_ResetMixSoundBufAddr(void);
#endif
extern INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static UINT32 _DbToRaw(INT16 i2Level)
{
    INT32 i4Val;

    // Check max and min value
    if (i2Level >= (INT32)_u4PosDBTableSz)
    {
        i4Val = (INT32)_u4PosDBTableSz;
    }
    else if (i2Level < (-(INT32)_u4NegDBTableSz))
    {
        i4Val = -(INT32)_u4NegDBTableSz;
    }
    else
    {
        i4Val = i2Level;
    }

    return (i4Val <= 0) ? _au4NegDBTable[-i4Val] : _au4PosDBTable[i4Val];
}

static AUD_CH_T _AudDecCh2AudCh(MTAUDDEC_CH_T eChIndex)
{
    AUD_CH_T eAudChl = AUD_CH_FRONT_LEFT;

    switch (eChIndex)
    {
    case MTAUDDEC_CH_FRONT_LEFT:
        eAudChl = AUD_CH_FRONT_LEFT;
        break;
    case MTAUDDEC_CH_FRONT_RIGHT:
        eAudChl = AUD_CH_FRONT_RIGHT;
        break;
    case MTAUDDEC_CH_REAR_LEFT:
        eAudChl = AUD_CH_REAR_LEFT;
        break;
    case MTAUDDEC_CH_REAR_RIGHT:
        eAudChl = AUD_CH_REAR_RIGHT;
        break;
    case MTAUDDEC_CH_CENTER:
        eAudChl = AUD_CH_CENTER;
        break;
    case MTAUDDEC_CH_SUB_WOOFER:
        eAudChl = AUD_CH_SUB_WOOFER;
        break;
    case MTAUDDEC_CH_BYPASS_LEFT:
        eAudChl = AUD_CH_BYPASS_LEFT;
        break;
    case MTAUDDEC_CH_BYPASS_RIGHT:
        eAudChl = AUD_CH_BYPASS_RIGHT;
        break;
    case MTAUDDEC_CH_DMX_LEFT:
        eAudChl = AUD_CH_DMX_LEFT;
        break;
    case MTAUDDEC_CH_DMX_RIGHT:
        eAudChl = AUD_CH_DMX_RIGHT;
        break;
    case MTAUDDEC_CH_AUX_FRONT_LEFT:
        eAudChl = AUD_CH_AUX_FRONT_LEFT;
        break;
    case MTAUDDEC_CH_AUX_FRONT_RIGHT:
        eAudChl = AUD_CH_AUX_FRONT_RIGHT;
        break;
    case MTAUDDEC_CH_INPUT_SRC:
        eAudChl = AUD_CH_INPUT_SRC;
        break;
    case MTAUDDEC_CH_ALL:
        eAudChl = AUD_CH_ALL;
        break;
    default:
        break;
    }

    return eAudChl;
}

static UINT16 _DelayFromMs(UINT16 u2Delay)
{
    return (UINT16)(u2Delay*7);
}

static AUD_FMT_T _DecFmtTransfer(MTAUDDEC_FMT_T eDecFmt)
{
    AUD_FMT_T eAudFmt = AUD_FMT_AC3;

    switch (eDecFmt)
    {
    case MTAUDDEC_FMT_MPEG:
        eAudFmt = AUD_FMT_MPEG;
        break;
    case MTAUDDEC_FMT_AC3:
        eAudFmt = AUD_FMT_AC3;
        break;
    case MTAUDDEC_FMT_PCM:
        eAudFmt = AUD_FMT_PCM;
        break;
    case MTAUDDEC_FMT_MP3:
        eAudFmt = AUD_FMT_MP3;
        break;
    case MTAUDDEC_FMT_AAC:
        eAudFmt = AUD_FMT_AAC;
        break;
    case MTAUDDEC_FMT_DTS:
        eAudFmt = AUD_FMT_DTS;
        break;
    case MTAUDDEC_FMT_WMA:
        eAudFmt = AUD_FMT_WMA;
        break;
    case MTAUDDEC_FMT_RA:
        eAudFmt = AUD_FMT_RA;
        break;
    case MTAUDDEC_FMT_HDCD:
        eAudFmt = AUD_FMT_HDCD;
        break;
    case MTAUDDEC_FMT_MLP:
        eAudFmt = AUD_FMT_MLP;
        break;
    case MTAUDDEC_FMT_MTS:
        eAudFmt = AUD_FMT_MTS;
        break;
    case MTAUDDEC_FMT_A2:
        eAudFmt = AUD_FMT_A2;
        break;
    case MTAUDDEC_FMT_EIAJ:
        eAudFmt = AUD_FMT_A2;
        break;
    case MTAUDDEC_FMT_NICAM:
        eAudFmt = AUD_FMT_PAL;
        break;
    default:
        printk("Error audio format(%x) setup\n", eDecFmt);
    }

    return eAudFmt;
}

static MTAUDDEC_FMT_T _DecFmtTransferINV(AUD_FMT_T eDecFmt)
{
    MTAUDDEC_FMT_T eAudFmt = AUD_FMT_AC3;

    switch (eDecFmt)
    {
    case AUD_FMT_MPEG:
        eAudFmt = MTAUDDEC_FMT_MPEG;
        break;
    case AUD_FMT_AC3:
        eAudFmt = MTAUDDEC_FMT_AC3;
        break;
    case AUD_FMT_PCM:
        eAudFmt = MTAUDDEC_FMT_PCM;
        break;
    case AUD_FMT_MP3:
        eAudFmt = MTAUDDEC_FMT_MP3;
        break;
    case AUD_FMT_AAC:
	case AUD_FMT_TTXAAC:
        eAudFmt = MTAUDDEC_FMT_AAC;
        break;
    case AUD_FMT_DTS:
        eAudFmt = MTAUDDEC_FMT_DTS;
        break;
    case AUD_FMT_WMA:
        eAudFmt = MTAUDDEC_FMT_WMA;
        break;
    case AUD_FMT_RA:
        eAudFmt = MTAUDDEC_FMT_RA;
        break;
    case AUD_FMT_HDCD:
        eAudFmt = MTAUDDEC_FMT_HDCD;
        break;
    case AUD_FMT_MLP:
        eAudFmt = MTAUDDEC_FMT_MLP;
        break;
    case AUD_FMT_MTS:
        eAudFmt = MTAUDDEC_FMT_MTS;
        break;
    case AUD_FMT_A2:
        eAudFmt = MTAUDDEC_FMT_A2;
        break;
	case AUD_FMT_PAL:
    case AUD_FMT_NICAM:
        eAudFmt = MTAUDDEC_FMT_NICAM;
        break;
	case AUD_FMT_FMFM:
		eAudFmt = MTAUDDEC_FMT_EIAJ;
        break;
    default:
		eAudFmt = MTAUDDEC_FMT_UNKNOWN;
		break;
        //printk("Error audio format(%x) setup\n", eDecFmt);
    }

    return eAudFmt;
}

static AUD_DEC_STREAM_FROM_T _DecSrcTransfer(MTAUDDEC_DEC_STREAM_FROM_T eDecSrc)
{
    AUD_DEC_STREAM_FROM_T eAudStreamFrom = AUD_STREAM_FROM_OTHERS;

    switch (eDecSrc)
    {
    case MTAUDDEC_STREAM_FROM_OTHERS:
        eAudStreamFrom = AUD_STREAM_FROM_OTHERS;
        break;
    case MTAUDDEC_STREAM_FROM_DIGITAL_TUNER:
        eAudStreamFrom = AUD_STREAM_FROM_DIGITAL_TUNER;
        break;
    case MTAUDDEC_STREAM_FROM_ANALOG_TUNER:
        eAudStreamFrom = AUD_STREAM_FROM_ANALOG_TUNER;
        break;
    case MTAUDDEC_STREAM_FROM_SPDIF:
        eAudStreamFrom = AUD_STREAM_FROM_SPDIF;
        break;
    case MTAUDDEC_STREAM_FROM_LINE_IN:
        eAudStreamFrom = AUD_STREAM_FROM_LINE_IN;
        break;
    case MTAUDDEC_STREAM_FROM_HDMI:
        eAudStreamFrom = AUD_STREAM_FROM_HDMI;
        break;
    case MTAUDDEC_STREAM_FROM_MEMORY:
        eAudStreamFrom = AUD_STREAM_FROM_MEMORY;
        break;
    case MTAUDDEC_STREAM_FROM_MPLAYER:
        eAudStreamFrom = AUD_STREAM_FROM_FEEDER;
        break;
#ifdef CC_MT5363
    case MTAUDDEC_STREAM_FROM_INTERNAL_BUF:
        eAudStreamFrom = AUD_STREAM_FROM_INTERNAL_BUF;
        break;
#endif
    default:
        printk("Error audio stream from (%x) setup\n", eDecSrc);
    }

    return eAudStreamFrom;
}

static MTAUDDEC_DEC_STREAM_FROM_T  _DecSrcTransferINV(AUD_DEC_STREAM_FROM_T eDecSrc)
{
    MTAUDDEC_DEC_STREAM_FROM_T  eAudStreamFrom = AUD_STREAM_FROM_OTHERS;

    switch (eDecSrc)
    {
    case AUD_STREAM_FROM_OTHERS:
        eAudStreamFrom = MTAUDDEC_STREAM_FROM_OTHERS;
        break;
    case AUD_STREAM_FROM_DIGITAL_TUNER:
        eAudStreamFrom = MTAUDDEC_STREAM_FROM_DIGITAL_TUNER;
        break;
    case AUD_STREAM_FROM_ANALOG_TUNER:
        eAudStreamFrom = MTAUDDEC_STREAM_FROM_ANALOG_TUNER;
        break;
    case AUD_STREAM_FROM_SPDIF:
        eAudStreamFrom = MTAUDDEC_STREAM_FROM_SPDIF;
        break;
    case AUD_STREAM_FROM_LINE_IN:
        eAudStreamFrom = MTAUDDEC_STREAM_FROM_LINE_IN;
        break;
    case AUD_STREAM_FROM_HDMI:
        eAudStreamFrom = MTAUDDEC_STREAM_FROM_HDMI;
        break;
    case AUD_STREAM_FROM_MEMORY:
        eAudStreamFrom = MTAUDDEC_STREAM_FROM_MEMORY;
        break;
    case AUD_STREAM_FROM_FEEDER:
        eAudStreamFrom = MTAUDDEC_STREAM_FROM_MPLAYER;
        break;
#ifdef CC_MT5363
    case AUD_STREAM_FROM_INTERNAL_BUF:
        eAudStreamFrom = MTAUDDEC_STREAM_FROM_INTERNAL_BUF;
        break;
#endif
    default:
        eAudStreamFrom = MTAUDDEC_STREAM_FROM_OTHERS;
		break;
        //printk("Error audio stream from (%x) setup\n", eDecSrc);
    }

    return eAudStreamFrom;
}

static void _AudVolumeChange(AUD_CH_T eAudChl)
{
    UINT32 u4Volumn;
    UINT64 u8ChlVol;

    // Channel volume = channel volume * Balance * BypassMasterVol(only for bypass channel)

    // Calculate balance
    if ((eAudChl == AUD_CH_FRONT_LEFT) ||
            (eAudChl == AUD_CH_DMX_LEFT))
    {
        u8ChlVol = (UINT64)_DbToRaw(_ai2ChlVolInDB[eAudChl]) * ((UINT64)(_DbToRaw(_rAudSetting.i2BalanceLeftInDB)));
        u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
    }
    else if ((eAudChl == AUD_CH_FRONT_RIGHT) ||
             (eAudChl == AUD_CH_DMX_RIGHT))
    {
        u8ChlVol = (UINT64)_DbToRaw(_ai2ChlVolInDB[eAudChl]) * ((UINT64)(_DbToRaw(_rAudSetting.i2BalanceRightInDB)));
        u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
    }
    else if ((eAudChl == AUD_CH_BYPASS_RIGHT) ||
             (eAudChl == AUD_CH_BYPASS_RIGHT))
    {
        u8ChlVol = (UINT64)_DbToRaw(_ai2ChlVolInDB[eAudChl]) * ((UINT64)(_DbToRaw(_rAudSetting.i2BypassMastVolumeInDB)));
        u4Volumn = (UINT32)(u8ChlVol / VOL_SHM_0_DB);
    }
    else
    {
        u4Volumn = _DbToRaw(_ai2ChlVolInDB[eAudChl]);
    }
#ifdef CC_MT5363
    AUD_DspChannelVolShmRawValue(AUD_A_DSP,AUD_DEC_MAIN, eAudChl, u4Volumn);
#else
    AUD_DspChannelVolShmRawValue(AUD_DEC_MAIN, eAudChl, u4Volumn);
#endif

}



static void _AudHandleModeChg(UINT8 u1DecId, AUD_DEC_INFO_T* prDecInfo)
{
    //AUD_DEC_INFO_T  rDecInfo;
    MTAUDDEC_CB_T   eAudCbType;

    // Currently on handle AC3
//#ifdef AUD_THREE_DECODER
//    if ((u1DecId == AUD_DEC_AUX) && ((prDecInfo->e_aud_fmt == AUD_FMT_AC3) ||
//            (prDecInfo->e_aud_fmt == AUD_FMT_MPEG) || (prDecInfo->e_aud_fmt == AUD_FMT_AAC)))
//#else
    if (1)// Always notify vestel app if we get some aout enable notify
        /*((prDecInfo->e_aud_fmt == AUD_FMT_AC3) || (prDecInfo->e_aud_fmt == AUD_FMT_MPEG) ||
            (prDecInfo->e_aud_fmt == AUD_FMT_AAC) ||(prDecInfo->e_aud_fmt == AUD_FMT_PCM))
            */
//#endif
    {
        MTAUDDEC_MODE_T eMode;
        MTAUDDEC_SAMPLE_FREQ_T eSmpRate;

        switch (prDecInfo->e_aud_type)
        {
        case AUD_TYPE_MONO:                  /* 1/0 */
            eMode = MTAUDDEC_MODE_MONO;
            break;
        case AUD_TYPE_MONO_SUB:              /* 1+sub-language */
            eMode = MTAUDDEC_MODE_MONO_SUB;
            break;
        case AUD_TYPE_DUAL_MONO:             /* 1+1 */
            eMode = MTAUDDEC_MODE_DUAL_MONO;
            break;
        case AUD_TYPE_STEREO:                /* 2/0 */
            eMode = MTAUDDEC_MODE_STEREO;
            break;
        case AUD_TYPE_STEREO_SUB:            /* 2+sub-language */
            eMode = MTAUDDEC_MODE_STEREO_SUB;
            break;
        case AUD_TYPE_STEREO_DOLBY_SURROUND: /* 2/0, dolby surround */
            eMode = MTAUDDEC_MODE_STEREO_DOLBY_SURROUND;
            break;
        case AUD_TYPE_SURROUND_2CH:          /* 2/1 */
            eMode = MTAUDDEC_MODE_SURROUND_2CH;
            break;
        case AUD_TYPE_SURROUND:              /* 3/1 */
            eMode = MTAUDDEC_MODE_SURROUND;
            break;
        case AUD_TYPE_3_0:                   /* 3/0 */
            eMode = MTAUDDEC_MODE_3_0;
            break;
        case AUD_TYPE_4_0:                   /* 2/2 */
            eMode = MTAUDDEC_MODE_4_0;
            break;
        case AUD_TYPE_5_1:                   /* 3/2 +SW */
            eMode = MTAUDDEC_MODE_5_1;
            break;
        default:
            eMode = MTAUDDEC_MODE_STEREO;
        }

        switch (prDecInfo->ui4_sample_rate)
        {
        case 8000:
            eSmpRate = MTAUDDEC_FS_8K;
            break;
        case 11000:
            eSmpRate = MTAUDDEC_FS_11K;
            break;
        case 12000:
            eSmpRate = MTAUDDEC_FS_12K;
            break;
        case 16000:
            eSmpRate = MTAUDDEC_FS_16K;
            break;
        case 22000:
            eSmpRate = MTAUDDEC_FS_22K;
            break;
        case 24000:
            eSmpRate = MTAUDDEC_FS_24K;
            break;
        case 32000:
            eSmpRate = MTAUDDEC_FS_32K;
            break;
        case 44000:
            eSmpRate = MTAUDDEC_FS_44K;
            break;
        case 48000:
            eSmpRate = MTAUDDEC_FS_48K;
            break;
        case 64000:
            eSmpRate = MTAUDDEC_FS_64K;
            break;
        case 88000:
            eSmpRate = MTAUDDEC_FS_88K;
            break;
        case 96000:
            eSmpRate = MTAUDDEC_FS_96K;
            break;
        case 176000:
            eSmpRate = MTAUDDEC_FS_176K;
            break;
        case 192000:
            eSmpRate = MTAUDDEC_FS_192K;
            break;
        default:
            eSmpRate = MTAUDDEC_FS_48K;
        }

        eAudCbType.u4Arg1 = u1DecId;
        eAudCbType.u4Arg2 = eMode;
        eAudCbType.u4Arg3 = eSmpRate;

        // Stop decoder will reset _rAudSetting.eAudMode
        //if (_rAudSetting.eAudMode != eMode)
        {
            _CB_PutEvent(CB_MTAL_AUDDEC_MODE_CHG_TRIGGER, sizeof(MTAUDDEC_CB_T), (void*)&eAudCbType);
        }

        _rAudSetting.eAudMode = eMode;

    }

}

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
static void _AudHandleMixsound(void)
{
    MTAUDDEC_CB_T   eAudCbType;

#if MIXSOUND_MEMPLAY_USER_SPACE_BUFFER
    void* pvBuf;
    pvBuf = (void*)_MTAUDDEC_GetMixSoundBufAddr();
    kfree(pvBuf);
    _MTAUDDEC_ResetMixSoundBufAddr();
#endif

    UNUSED(eAudCbType);
    eAudCbType.u4Arg1 = MTAUDDEC_MIXSOUND_CTRL_PLAY_DONE;
    _CB_PutEvent(CB_MTAL_AUDDEC_MIXSOUND_CTRL_TRIGGER, sizeof(MTAUDDEC_CB_T), (void*)&eAudCbType);
}
#endif
#if 1
static void _AudHandleHp(BOOL fgHpConnect)
{
    MTAUDDEC_CB_T   eAudCbType;
    //UNUSED(eAudCbType);
    eAudCbType.u4Arg1=fgHpConnect;
    _CB_PutEvent(CB_MTAL_AUDDEC_HP_TRIGGER, sizeof(MTAUDDEC_CB_T), (void*)&eAudCbType);
}
#endif
static void _AudHandleClipPlayDone(void)
{
    MTAUDDEC_CB_T   eAudCbType;
    //UNUSED(eAudCbType);
    eAudCbType.u4Arg1=0;
    _CB_PutEvent(CB_MTAL_AUDDEC_CLIP_PLAY_DONE_TRIGGER, sizeof(MTAUDDEC_CB_T), (void*)&eAudCbType);
}

static void _AudMtalNfyFunc(void *          pvNfyTag,
                            UINT8           u1DecId,
                            AUD_COND_NFY_T  eNfyCond,
                            UINT32          u4Data1,
                            UINT32          u4Data2)
{
    UINT32 u4NfyTag = 0;

    u4NfyTag = ((UINT32)pvNfyTag & 0xF);

    UNUSED(u4Data2);
    if ((AUD_NFY_TAG_T)u4NfyTag == AUD_NFY_STREAM)
    {
        if ((eNfyCond == AUD_COND_AUD_INFO_CHG) && _afgCbReg[MTAUDDEC_CB_FUNC_MODE_CHG_ID])
        {
            _AudHandleModeChg(u1DecId, (AUD_DEC_INFO_T *)u4Data1);
        }

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
        if ((eNfyCond == AUD_COND_AUD_CLIP_DONE) && _afgCbReg[MTAUDDEC_CB_FUNC_MIXSOUND_CTRL_ID])
        {
            _AudHandleMixsound();
        }
#endif
        if ((eNfyCond == AUD_COND_AUD_CLIP_PLAY_DONE) && _afgCbReg[MTAUDDEC_CB_FUNC_CLIP_DONE_ID])
        {
            _AudHandleClipPlayDone();
        }
        // TODO for mp3 callback
    }
    else if ((AUD_NFY_TAG_T)u4NfyTag == AUD_NFY_HP)
    {
        _AudHandleHp((BOOL)eNfyCond);
    }
        


}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------


/**Initialize Auio Decider module.
*@param void
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_Init (void)
{
    static BOOL fgInit = FALSE;
    INT32 i4Ret = MTR_OK;
    INT32 i;

    if (!fgInit)
    {
        fgInit = TRUE;

        // Clear audio setting ..
        x_memset((void *)&(_rAudSetting), 0, sizeof(AUD_SETTING_T));

        // Individual decoder initialization
        for (i=0; i<AUD_DEC_MAX; i++)
        {
            // Semphore initialization
            MT_AUD_SEMA_INIT(i);
            MT_AUD_SEMA_LOCK(i);

            // Clear decoder info ..
            x_memset((void *)&(_arAudDec[i]), 0, sizeof(AUD_DEC_INFOR_T));

            // Initial value
            _arAudDec[i].fgIssuePlay = FALSE;
            _arAudDec[i].fgOnPlaying = FALSE;
            _arAudDec[i].fgPause     = FALSE;
            _arAudDec[i].fgAvSync    = FALSE;
            _arAudDec[i].eDecFmt     = MTAUDDEC_FMT_UNKNOWN;
            _arAudDec[i].eStrSrc     = MTAUDDEC_FMT_PCM;

            _arAudDec[i].prAudSetting = &_rAudSetting;

            MT_AUD_SEMA_UNLOCK(i);
        }

        // Audio detail setting
        _rAudSetting.eCompMode     = MTAUDDEC_AC3_COMP_LINE;
        _rAudSetting.eDmxMode      = MTAUDDEC_AC3_DOWNMIX_LTRT;
        _rAudSetting.eDualMonoMode = MTAUDDEC_AC3_DUALMONO_STEREO;
        _rAudSetting.u1DrcRange    = 8;  // Full compression
        _rAudSetting.eOutputMode   = MTAUDDEC_SOUND_MODE_UNKNOWN;
        _rAudSetting.eSetSifMode   = MTAUDDEC_SIF_UNKNOWN;
        _rAudSetting.rPcmSetting.eSampleFreq = MTAUDDEC_FS_48K;
        _rAudSetting.rPcmSetting.ePcmDataInfo = MTAUDDEC_PCM_24_BITS;
        _rAudSetting.u1MasterVol   = 100;
        _rAudSetting.fgAvc         = FALSE;
        _rAudSetting.fgBbe         = FALSE;
        _rAudSetting.fgSrs         = FALSE;
        _rAudSetting.fgVirSurr     = FALSE;
        _rAudSetting.fgBassBoost   = FALSE;
        _rAudSetting.i2Bass        = 0;
        _rAudSetting.fgTrebleBoost = FALSE;
        _rAudSetting.i2Treble      = 0;
        _rAudSetting.fgEqEnable    = FALSE;
        _rAudSetting.rEqBandSetting.i2Band0 = 0;
        _rAudSetting.rEqBandSetting.i2Band1 = 0;
        _rAudSetting.rEqBandSetting.i2Band2 = 0;
        _rAudSetting.rEqBandSetting.i2Band3 = 0;
        _rAudSetting.rEqBandSetting.i2Band4 = 0;
        _rAudSetting.rEqBandSetting.i2Band5 = 0;
        _rAudSetting.rEqBandSetting.i2Band6 = 0;
        _rAudSetting.eAvMuxId    = MTAUDDEC_INPUT_ID_MAX; // TODO
        _rAudSetting.eIecFmt    = MTAUDDEC_IEC_CFG_RAW;  // TODO
        _rAudSetting.fgIecEnable    = TRUE;
        _rAudSetting.fgIecCopyright = FALSE;
        _rAudSetting.u1IecCategoryCode = IEC_ORIGINAL_BROAD_CAST;
        _rAudSetting.eSpkOutputMode = MTAUDDEC_SPK_MODE_LR;
        _rAudSetting.i2BalanceLeftInDB      = 0;  // Default set as 0dB
        _rAudSetting.i2BalanceRightInDB     = 0;  // Default set as 0dB
        _rAudSetting.i2BypassMastVolumeInDB = 0;  // Default set as 0dB
        _rAudSetting.eAudMode = MTAUDDEC_MODE_UNKNOWN;

        // Channel volume initialization
        for (i = 0; i < MTAUDDEC_CH_NUM; i++)
        {
            _ai2ChlVolInDB[i] = 0;  // Default set as 0dB
        }

        // Driver initialization
        i4Ret = AUD_Init();


#ifdef CC_MT5363
        // Defualt set as Lt/Rt downmix
        AUD_DspSpeakerOutputConfig(AUD_A_DSP, AUD_DEC_MAIN, 0);
        // Suppot multi-pair output(Copy CH9/10 to LS/RS/C/SUB)
        AUD_DspMultiPairOutput(AUD_A_DSP, AUD_DEC_MAIN, 1);
        // Downmix channel(CH9/10) do not do post processing
        AUD_DspDownmixPosition(AUD_A_DSP, AUD_DEC_MAIN, 3);

#elif defined(CC_MT5391) || defined(CC_MT5360B) || defined(CC_MT5387)
        // Defualt set as Lt/Rt downmix
        AUD_DspSpeakerOutputConfig(AUD_DEC_MAIN, 0);
        // Suppot multi-pair output(Copy CH9/10 to LS/RS/C/SUB)
        AUD_DspMultiPairOutput(AUD_DEC_MAIN, 1);
        // Downmix channel(CH9/10) do not do post processing
        AUD_DspDownmixPosition(AUD_DEC_MAIN, 3);

#endif



        // Setup SPDIF output to CH9/10 (Copy CH9/10 to LS/RS/C/SUB)
        AUD_DspIecChannel(AUD_IEC_PCM_CH_LS_RS);

        // Volume control semaphore initialization
        VOL_CTL_SEMA_INIT();

        //Unmute
        ADAC_Mute(FALSE);

        // TODO
    }

    return MTR_OK;
}


#ifdef CC_MT5363
/**Get audio decoder type and stream source configuration.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peStreamFrom               Stream source.
*@param prDecType                  Decoder format.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_GetDecType (UINT8 u1DecId,
                                MTAUDDEC_DEC_STREAM_FROM_T *peStreamFrom,
                                MTAUDDEC_FMT_T *prDecType)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    *peStreamFrom = _arAudDec[u1DecId].eStrSrc;
    *prDecType    = _arAudDec[u1DecId].eDecFmt;

    return MTR_OK;
}

/**Set audio decoder type and stream source.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eStreamFrom                Stream source.
*@param eDecFmt                    Decoder format.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetDecType (UINT8 u1DecId,
                                MTAUDDEC_DEC_STREAM_FROM_T eStreamFrom,
                                MTAUDDEC_FMT_T eDecFmt)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    MT_AUD_SEMA_LOCK(u1DecId);

    _arAudDec[u1DecId].eStrSrc = eStreamFrom;
    _arAudDec[u1DecId].eDecFmt = eDecFmt;

    AUD_SetDecType(AUD_A_DSP ,AUD_DEC_MAIN, AUD_STREAM_FROM_INTERNAL_BUF, AUD_FMT_PCM);
    if (u1DecId == AUD_DEC_THIRD)
    {
        AUD_SetDecType(AUD_D_DSP ,AUD_DEC_MAIN, _DecSrcTransfer(eStreamFrom), _DecFmtTransfer(eDecFmt));
    }
    else
    {
        AUD_SetDecType(AUD_D_DSP ,u1DecId, _DecSrcTransfer(eStreamFrom), _DecFmtTransfer(eDecFmt));
    }

    MT_AUD_SEMA_UNLOCK(u1DecId);
    return MTR_OK;
}

/**Audio decoder playback.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_Play (UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    MT_AUD_SEMA_LOCK(u1DecId);

    _arAudDec[u1DecId].fgIssuePlay = TRUE;

    if (u1DecId == AUD_DEC_THIRD)
    {
        AUD_DSPCmdPlay(AUD_D_DSP ,AUD_DEC_MAIN);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        AUD_DSPCmdPlay(AUD_D_DSP ,AUD_DEC_AUX);
    }
    else
    {
        if ((_arAudDec[u1DecId].eStrSrc == MTAUDDEC_STREAM_FROM_ANALOG_TUNER) || (_arAudDec[u1DecId].eStrSrc == MTAUDDEC_STREAM_FROM_DIGITAL_TUNER))
        {
            AUD_DSPCmdPlay(AUD_D_DSP ,AUD_DEC_AUX);
        }
        else
        {
            AUD_DSPCmdPlay(AUD_D_DSP ,AUD_DEC_MAIN);
        }
        AUD_DSPCmdPlay(AUD_A_DSP ,AUD_DEC_MAIN);
    }

    MT_AUD_SEMA_UNLOCK(u1DecId);
    return MTR_OK;
}

/**Stop audio decoder.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_Stop (UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    MT_AUD_SEMA_LOCK(u1DecId);

    _arAudDec[u1DecId].fgIssuePlay = FALSE;
    _rAudSetting.eAudMode = MTAUDDEC_MODE_UNKNOWN;

    if (u1DecId == AUD_DEC_THIRD)
    {
        AUD_DSPCmdStop(AUD_D_DSP ,AUD_DEC_MAIN);
    }
    else if (u1DecId == AUD_DEC_AUX)
    {
        AUD_DSPCmdStop(AUD_D_DSP ,AUD_DEC_AUX);
    }
    else
    {
        AUD_DSPCmdStop(AUD_A_DSP ,AUD_DEC_MAIN);
        if ((_arAudDec[u1DecId].eStrSrc == MTAUDDEC_STREAM_FROM_ANALOG_TUNER) || (_arAudDec[u1DecId].eStrSrc == MTAUDDEC_STREAM_FROM_DIGITAL_TUNER))
        {
            AUD_DSPCmdStop(AUD_D_DSP ,AUD_DEC_AUX);
        }
        else
        {
            AUD_DSPCmdStop(AUD_D_DSP ,AUD_DEC_MAIN);
        }
    }

    MT_AUD_SEMA_UNLOCK(u1DecId);
    return MTR_OK;
}

/**Get the information of audio decoder.
*  Since in some case, even application issue play to decoder, but if the
*  bitstream is not feed to decoder, there is no audio output yet.
*  This function indicates the status that if the decoding is correctly getting
*  bistream and successfully decoded.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pfgPlay                    Is decoder real play
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_GetDecOnPlayingStatus (UINT8 u1DecId,
        BOOL* pfgOnPlay)
{
//    BOOL fgOnPlay = TRUE;

    AUD_DEC_ID_VALIDATE(u1DecId);

    // TODO
    //fgOnplay = DSP_GetAoutEnable();
    _arAudDec[u1DecId].fgOnPlaying = TRUE;
    *pfgOnPlay = TRUE;

    return MTR_OK;
}

#else

/**Get audio decoder type and stream source configuration.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peStreamFrom               Stream source.
*@param prDecType                  Decoder format.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_GetDecType (UINT8 u1DecId,
									MTAUDDEC_DEC_STREAM_FROM_T *peStreamFrom,
									MTAUDDEC_FMT_T *prDecType)
{
	AUD_DEC_STREAM_FROM_T eStreamFrom = AUD_STREAM_FROM_OTHERS;
	AUD_FMT_T eDecType = AUD_FMT_UNKNOWN;
	AUD_DEC_ID_VALIDATE(u1DecId);
	*peStreamFrom = _arAudDec[u1DecId].eStrSrc;
	*prDecType    = _arAudDec[u1DecId].eDecFmt;
	AUD_GetDecodeType(u1DecId, &eStreamFrom, &eDecType);
	*peStreamFrom = _DecSrcTransferINV(eStreamFrom);
	*prDecType = _DecFmtTransferINV(eDecType);
	
	return MTR_OK;
}

/**Set audio decoder type and stream source.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eStreamFrom                Stream source.
*@param eDecFmt                    Decoder format.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetDecType (UINT8 u1DecId,
                                MTAUDDEC_DEC_STREAM_FROM_T eStreamFrom,
                                MTAUDDEC_FMT_T eDecFmt)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    MT_AUD_SEMA_LOCK(u1DecId);

    _arAudDec[u1DecId].eStrSrc = eStreamFrom;
    _arAudDec[u1DecId].eDecFmt = eDecFmt;

    AUD_SetDecType(u1DecId, _DecSrcTransfer(eStreamFrom), _DecFmtTransfer(eDecFmt));

    MT_AUD_SEMA_UNLOCK(u1DecId);
    return MTR_OK;
}

/**Audio decoder playback.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_Play (UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    MT_AUD_SEMA_LOCK(u1DecId);

    _arAudDec[u1DecId].fgIssuePlay = TRUE;

    AUD_DSPCmdPlay(u1DecId);

    MT_AUD_SEMA_UNLOCK(u1DecId);
    return MTR_OK;
}

/**Stop audio decoder.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_Stop (UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    MT_AUD_SEMA_LOCK(u1DecId);

    _arAudDec[u1DecId].fgIssuePlay = FALSE;

    AUD_DSPCmdStop(u1DecId);

    MT_AUD_SEMA_UNLOCK(u1DecId);
    return MTR_OK;
}

/**Get the information of audio decoder.
*  Since in some case, even application issue play to decoder, but if the
*  bitstream is not feed to decoder, there is no audio output yet.
*  This function indicates the status that if the decoding is correctly getting
*  bistream and successfully decoded.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pfgPlay                    Is decoder real play
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_GetDecOnPlayingStatus (UINT8 u1DecId,
        BOOL* pfgOnPlay)
{
//    BOOL fgOnPlay = TRUE;

    AUD_DEC_ID_VALIDATE(u1DecId);

    // TODO
    //fgOnplay = DSP_GetAoutEnable();
    _arAudDec[u1DecId].fgOnPlaying = AUD_IsDecoderRealPlay(u1DecId);
    *pfgOnPlay = AUD_IsDecoderRealPlay(u1DecId);
    return MTR_OK;
}

#endif

/**_MTAUD_GetAudioPTS.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param Pts                            pts
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_GetAudioPTS(UINT8 u1DecId, UINT32 *Pts)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    AUD_GetAudioPTS(u1DecId, Pts);
    return MTR_OK;
}

#if 1
/**_MTAUD_LoadAudioClip.
*@param u1DecId 				   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1BufferSize			  u1BufferSize
*@param ClipBufferPointer		 ClipBufferPointer
*@param u1DecFmt				  DecFmt
*@param u1RepeatNumber		  RepeatNumber
*@retval MTR_OK 				   Success.
*@retval MTR_NOT_OK 			   Fail.
*/
MT_RESULT_T  _MTAUD_LoadAudioClip (UINT8 u1DecId, UINT32 u1BufferSize, UINT32 ClipBufferPointer, MTAUDDEC_FMT_T u1DecFmt, UINT32 u1RepeatNumber)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    MT_AUD_SEMA_LOCK(u1DecId);
	AUD_LineInCtrlEnable(u1DecId,FALSE);
    AudLoadAudioClip(u1DecId,u1BufferSize,ClipBufferPointer,_DecFmtTransfer(u1DecFmt),u1RepeatNumber);
    AUD_PlayAudClip(u1DecId, AUD_CLIP_CH_SELECT,FALSE);
    MT_AUD_SEMA_UNLOCK(u1DecId);
    return MTR_OK;
}
#else
/**_MTAUD_LoadAudioClip.
*@param u1DecId 				   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1BufferSize			  u1BufferSize
*@param ClipBufferPointer		 ClipBufferPointer
*@retval MTR_OK 				   Success.
*@retval MTR_NOT_OK 			   Fail.
*/
MT_RESULT_T  _MTAUD_LoadAudioClip (UINT8 u1DecId, UINT32 u1BufferSize, UINT32 ClipBufferPointer)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    MT_AUD_SEMA_LOCK(u1DecId);
    AudLoadAudioClip(u1DecId,u1BufferSize,ClipBufferPointer);
    MT_AUD_SEMA_UNLOCK(u1DecId);
    return MTR_OK;
}

#endif


/**_MTAUD_PlayAudioClip.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param ReaptNumber             clip ReaptNumber
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_PlayAudioClip(UINT8 u1DecId,  UINT32 ReaptNumber )
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    MT_AUD_SEMA_LOCK(u1DecId);
    AUD_PlayAudClipWithRPN(u1DecId, AUD_CLIP_CH_SELECT, ReaptNumber, FALSE);
    MT_AUD_SEMA_UNLOCK(u1DecId);
    return MTR_OK;
}

/**Stop audio clip.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_StopAudioClip (UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    MT_AUD_SEMA_LOCK(u1DecId);
    AUD_StopAudClip(u1DecId);
    MT_AUD_SEMA_UNLOCK(u1DecId);
    return MTR_OK;
}

/**_MTAUD_DeleteAudioClip.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_DeleteAudioClip (void)
{
    AUD_DeleteAudioClip();
    return MTR_OK;
}
/**Audio decoder call back function register.
*@param eFuncType                  The call back function type.
*@param u4FuncPtr                  The call back function pointer.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_RegCb (MTAUDDEC_CB_FUNC_ID_T eFuncType, UINT32 u4FuncPtr,
                           UINT32 u4Arg1, UINT32 u4Arg2)
{
    // Parameter check
    if ((eFuncType >= MTAUDDEC_CB_FUNC_ID_MAX) || ((void *)u4FuncPtr == NULL))
    {
        return MTR_PARAMETER_ERROR;
    }

    MT_AUD_SEMA_LOCK(AUD_DEC_MAIN);
    MT_AUD_SEMA_LOCK(AUD_DEC_AUX);

    UNUSED(u4Arg1);
    UNUSED(u4Arg2);

    _afgCbReg[eFuncType] = TRUE;

    if (!_fgDrvCbReg)
    {
        AUD_RegNotifyFunc(&_hHandler, _AudMtalNfyFunc);
        _fgDrvCbReg = TRUE;
    }

    MT_AUD_SEMA_UNLOCK(AUD_DEC_AUX);
    MT_AUD_SEMA_UNLOCK(AUD_DEC_MAIN);

    return MTR_OK;
}


/**Resume audio decoder.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_Resume (UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    MT_AUD_SEMA_LOCK(u1DecId);

    _arAudDec[u1DecId].fgPause = FALSE;

#ifdef CC_MT5363
    AUD_DSPCmdResume(AUD_D_DSP, u1DecId);
#else
    AUD_DSPCmdResume(u1DecId);
#endif

    MT_AUD_SEMA_UNLOCK(u1DecId);
    return MTR_OK;
}


/**Pause audio decoder.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_Pause (UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    MT_AUD_SEMA_LOCK(u1DecId);

    _arAudDec[u1DecId].fgPause = TRUE;

#ifdef CC_MT5363
    AUD_DSPCmdPause(AUD_D_DSP, u1DecId);
#else
    AUD_DSPCmdPause(u1DecId);
#endif

    MT_AUD_SEMA_UNLOCK(u1DecId);
    return MTR_OK;
}

/**Set AV synchronization mode.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eSynMode                   AV sync mode or just free run in DTV playback.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetAvSynMode (UINT8 u1DecId,
                                  MTAV_SYNC_MODE_T eSynMode)
{
    MT_RESULT_T    eRet = MTR_OK;
    AV_SYNC_MODE_T eAvSyncMode = AV_SYNC_SLAVE;

    AUD_DEC_ID_VALIDATE(u1DecId);

    switch (eSynMode)
    {
    case MTAV_SYNC_FREE_RUN:
        eAvSyncMode = AV_SYNC_FREE_RUN;
        break;
    case MTAV_SYNC_MASTER:
        eAvSyncMode = AV_SYNC_AUDIO_MASTER;
        break;
    case MTAV_SYNC_SLAVE:
        eAvSyncMode = AV_SYNC_SLAVE;
        break;
    default:
        eRet = MTR_ERR_INV;
        break;
    }

    _arAudDec[u1DecId].fgAvSync =  (eSynMode == MTAV_SYNC_FREE_RUN) ?
                                   TRUE : FALSE;
    AUD_SetAvSynMode(u1DecId, eAvSyncMode);

    return eRet;
}

/**Setup AC3 compression mode.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eCompMode                  Compression mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetAc3CompMode (UINT8 u1DecId,
                                    MTAUDDEC_AC3_COMP_MODE_T eCompMode)
{
    MT_RESULT_T    eRet = MTR_OK;
    UINT16 u2CompMode = 0;

    AUD_DEC_ID_VALIDATE(u1DecId);

    switch (eCompMode)
    {
    case MTAUDDEC_AC3_COMP_CUSTOM_0:
        u2CompMode = 0;
        break;
    case MTAUDDEC_AC3_COMP_CUSTOM_1:
        u2CompMode = 1;
        break;
    case MTAUDDEC_AC3_COMP_LINE:
        u2CompMode = 2;
        break;
    case MTAUDDEC_AC3_COMP_RF:
        u2CompMode = 3;
        break;
    default:
        u2CompMode = 2;
        eRet = MTR_ERR_INV;
    }

    _arAudDec[u1DecId].prAudSetting->eCompMode = eCompMode;
    AUD_DspAC3CompMode(u2CompMode, u1DecId);

    return eRet;
}


/**Setup AC3 downmix mode.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eDownmixMode               Downmix mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetAc3DownmixMode (UINT8 u1DecId,
                                       MTAUDDEC_AC3_DOWNMIX_MODE eDownmixMode)
{
    MT_RESULT_T    eRet = MTR_OK;
    UINT8 u1DmxMode = 0;

    AUD_DEC_ID_VALIDATE(u1DecId);

    switch (eDownmixMode)
    {
    case MTAUDDEC_AC3_DOWNMIX_LTRT:
        u1DmxMode = 0;
        break;
    case MTAUDDEC_AC3_DOWNMIX_LORO:
        u1DmxMode = 2;
        break;
    default:
        u1DmxMode = 0;
        eRet = MTR_ERR_INV;
    }

    _arAudDec[u1DecId].prAudSetting->eDmxMode = eDownmixMode;

#ifdef CC_MT5363
    AUD_DspSpeakerOutputConfig(AUD_A_DSP, u1DecId, u1DmxMode);
#else
    AUD_DspSpeakerOutputConfig(u1DecId, u1DmxMode);
#endif

    return eRet;
}

/**Get AC3 downmix mode.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eDownmixMode               Downmix mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_GetAc3DownmixMode (UINT8 u1DecId,
                                       MTAUDDEC_AC3_DOWNMIX_MODE * peDownmixMode)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    *peDownmixMode = _arAudDec[u1DecId].prAudSetting->eDmxMode;
    return MTR_OK;
}

/**Get AC3 audio mode.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peMode                     AC3 channel information.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_GetAc3Mode (UINT8 u1DecId, MTAUDDEC_MODE_T * peMode)
{
    AUD_DEC_INFO_T rAudInfo;
    AUD_TYPE_T     eAudType=AUD_TYPE_UNKNOWN;
    MTAUDDEC_MODE_T  eMtAudMode=MTAUDDEC_MODE_UNKNOWN;

    AUD_GetDtvAudInfo(u1DecId, &rAudInfo);

    eAudType = rAudInfo.e_aud_type;

    switch (eAudType)
    {
    case AUD_TYPE_MONO:
        eMtAudMode = MTAUDDEC_MODE_MONO;
        break;
    case AUD_TYPE_MONO_SUB:
        eMtAudMode = MTAUDDEC_MODE_MONO_SUB;
        break;
    case AUD_TYPE_DUAL_MONO:
        eMtAudMode = MTAUDDEC_MODE_DUAL_MONO;
        break;
    case AUD_TYPE_STEREO:
        eMtAudMode = MTAUDDEC_MODE_STEREO;
        break;
    case AUD_TYPE_STEREO_SUB:
        eMtAudMode = MTAUDDEC_MODE_STEREO_SUB;
        break;
    case AUD_TYPE_STEREO_DOLBY_SURROUND:
        eMtAudMode = MTAUDDEC_MODE_STEREO_DOLBY_SURROUND;
        break;
    case AUD_TYPE_SURROUND_2CH:
        eMtAudMode = MTAUDDEC_MODE_SURROUND_2CH;
        break;
    case AUD_TYPE_SURROUND:
        eMtAudMode = MTAUDDEC_MODE_SURROUND;
        break;
    case AUD_TYPE_3_0:
        eMtAudMode = MTAUDDEC_MODE_3_0;
        break;
    case AUD_TYPE_4_0:
        eMtAudMode = MTAUDDEC_MODE_4_0;
        break;
    case AUD_TYPE_5_1:
        eMtAudMode = MTAUDDEC_MODE_5_1;
        break;
    default:
        eMtAudMode = MTAUDDEC_MODE_STEREO;
        break;
    }

    *peMode = eMtAudMode;

    return MTR_OK;
}

/**Get AC3 ES info.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peAC3ESInfo             AC3 ES information.
*@retval MTR_OK                     Success.
*@retval MTR_NOT_OK              Fail.
*/

MT_RESULT_T  _MTAUD_GetAC3ESInfo (UINT8 u1DecId, MTAUDDEC_AC3_ES_INFO_T *peAC3ESInfo)
{
    AUD_DEC_INFO_T rAudInfo;
    AUD_TYPE_T     eAudType=AUD_TYPE_UNKNOWN;
    AUD_GetDtvAudInfo(u1DecId, &rAudInfo);
    eAudType = rAudInfo.e_aud_type;
    switch (eAudType)
    {
    case AUD_TYPE_MONO:
        (*peAC3ESInfo).channelNum=0;    //AC3 channel Num for L*A1   0~mono; 1~dual mono;  2~stereo; 3-7~ multi; other ~unknown;
        break;
    case AUD_TYPE_DUAL_MONO:
        (*peAC3ESInfo).channelNum=1;
        break;
    case AUD_TYPE_STEREO:
        (*peAC3ESInfo).channelNum=2;
        break;
    case AUD_TYPE_STEREO_DOLBY_SURROUND:
        (*peAC3ESInfo).channelNum=3;
        break;
    case AUD_TYPE_SURROUND_2CH:
        (*peAC3ESInfo).channelNum=3;
        break;
    case AUD_TYPE_SURROUND:
        (*peAC3ESInfo).channelNum=3;
        break;
    case AUD_TYPE_3_0:
        (*peAC3ESInfo).channelNum=3;
        break;
    case AUD_TYPE_4_0:
        (*peAC3ESInfo).channelNum=3;
        break;
    case AUD_TYPE_5_0:
        (*peAC3ESInfo).channelNum=3;
        break;
    case AUD_TYPE_3_1:
        (*peAC3ESInfo).channelNum=3;
        break;
    case AUD_TYPE_4_1:
        (*peAC3ESInfo).channelNum=3;
        break;
    case AUD_TYPE_5_1:
        (*peAC3ESInfo).channelNum=3;
        break;
    default:
        (*peAC3ESInfo).channelNum=8;
        break;
    }
    if ((rAudInfo.ui4_data_rate/1000)>=256)
    {
        (*peAC3ESInfo).bitRate=255;
        //Printf("AC3 bit rate > 255KHz\n");
    }
    else
    {
        (*peAC3ESInfo).bitRate=(UINT8)(rAudInfo.ui4_data_rate/1000);
    }
    if ((rAudInfo.ui4_sample_rate/1000)>=256)
    {
        (*peAC3ESInfo).sampleRate=255;
        //Printf("AC3 sample rate > 255KHz\n");
    }
    else
    {
        (*peAC3ESInfo).sampleRate=(UINT8)(rAudInfo.ui4_sample_rate/1000);
    }
    (*peAC3ESInfo).EAC3=AUD_GetAC3_EAC3_Info(u1DecId);

    return MTR_OK;
}


/**Get MPEG ES info.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peMPEGESInfo           MPEG ES information.
*@retval MTR_OK                     Success.
*@retval MTR_NOT_OK              Fail.
*/
MT_RESULT_T  _MTAUD_GetMPEGESInfo (UINT8 u1DecId, MTAUDDEC_MPEG_ES_INFO_T *peMPEGESInfo)
{
    AUD_DEC_INFO_T rAudInfo;
    AUD_TYPE_T     eAudType=AUD_TYPE_UNKNOWN;
    UINT8 rMPEGLayer=0;
    AUD_DspGetMpegAudInfowithLayer(u1DecId, &rAudInfo, &rMPEGLayer);
    eAudType = rAudInfo.e_aud_type;
    switch (eAudType)
    {
    case AUD_TYPE_MONO:
        (*peMPEGESInfo).channelNum=3;            //mpeg channel Num for L*A1   3~mono; 2~dual mono;  0~stereo; 1~ joint stereo; other ~unknown;
        break;
    case AUD_TYPE_DUAL_MONO:
        (*peMPEGESInfo).channelNum=2;
        break;
    case AUD_TYPE_STEREO:
        (*peMPEGESInfo).channelNum=0;
        break;
    case AUD_TYPE_JOINT_STEREO:
        (*peMPEGESInfo).channelNum=1;
        break;
    default:
        (*peMPEGESInfo).channelNum=8;
        break;
    }
    (*peMPEGESInfo).layer=rMPEGLayer;
    if ((rAudInfo.ui4_data_rate/1000)>=256)
    {
        (*peMPEGESInfo).bitRate=255;
        //Printf("mpeg bit rate > 255KHz\n");
    }
    else
    {
        (*peMPEGESInfo).bitRate=(UINT8)(rAudInfo.ui4_data_rate/1000);
    }
    if ((rAudInfo.ui4_sample_rate/1000)>=256)
    {
        (*peMPEGESInfo).sampleRate=255;
        //Printf("mpeg sample rate > 255KHz\n");
    }
    else
    {
        (*peMPEGESInfo).sampleRate=(UINT8)(rAudInfo.ui4_sample_rate/1000);
    }
    return MTR_OK;
}


/**Get HEAAC ES info.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peHEAACESInfo         HEAAC ES information.
*@retval MTR_OK                     Success.
*@retval MTR_NOT_OK              Fail.
*/
MT_RESULT_T  _MTAUD_GetHEAACESInfo (UINT8 u1DecId, MTAUDDEC_HEAAC_ES_INFO_T *peHEAACESInfo)
{
    AUD_DEC_INFO_T rAudInfo;
    AUD_TYPE_T     eAudType=AUD_TYPE_UNKNOWN;
    AUD_GetDtvAudInfo(u1DecId, &rAudInfo);
    eAudType = rAudInfo.e_aud_type;
    switch (eAudType)
    {
    case AUD_TYPE_MONO:
        (*peHEAACESInfo).channelNum=0;     ////AAC channel Num for L*A1   0~mono; 1~dual mono;  2~stereo; 3-7~ mult;other ~unknown;
        break;
    case AUD_TYPE_DUAL_MONO:
        (*peHEAACESInfo).channelNum=1;
        break;
    case AUD_TYPE_STEREO:
        (*peHEAACESInfo).channelNum=2;
        break;
    case AUD_TYPE_SURROUND_2CH:
        (*peHEAACESInfo).channelNum=3;
        break;
    case AUD_TYPE_SURROUND:
        (*peHEAACESInfo).channelNum=3;
        break;
    case AUD_TYPE_3_0:
        (*peHEAACESInfo).channelNum=3;
        break;
    case AUD_TYPE_4_0:
        (*peHEAACESInfo).channelNum=3;
        break;
    case AUD_TYPE_5_0:
        (*peHEAACESInfo).channelNum=3;
        break;
    case AUD_TYPE_3_1:
        (*peHEAACESInfo).channelNum=3;
        break;
    case AUD_TYPE_4_1:
        (*peHEAACESInfo).channelNum=3;
        break;
    case AUD_TYPE_5_1:
        (*peHEAACESInfo).channelNum=3;
        break;

    default:
        (*peHEAACESInfo).channelNum=8;
        break;
    }

    (*peHEAACESInfo).version=AUD_GetAacVersion(u1DecId);
    (*peHEAACESInfo).transmissionformat=AUD_GetAacTSFormat(u1DecId);
    return MTR_OK;
}


/**Get audio sampling rate.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pSmpRate                   Sampling rate.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_GetSamplingRate (UINT8 u1DecId,
                                     MTAUDDEC_SAMPLE_FREQ_T * pSmpRate)
{
    SAMPLE_FREQ_T eSampRate;
    MTAUDDEC_SAMPLE_FREQ_T eMtSampRate = MTAUDDEC_FS_48K;
    AUD_DEC_ID_VALIDATE(u1DecId);

    eSampRate = AUD_GetSampleFreq(u1DecId);

    switch (eSampRate)
    {
    case FS_16K:
        eMtSampRate = MTAUDDEC_FS_16K;
        break;
    case FS_22K:
        eMtSampRate = MTAUDDEC_FS_22K;
        break;
    case FS_24K:
        eMtSampRate = MTAUDDEC_FS_24K;
        break;
    case FS_32K:
        eMtSampRate = MTAUDDEC_FS_32K;
        break;
    case FS_44K:
        eMtSampRate = MTAUDDEC_FS_44K;
        break;
    case FS_48K:
        eMtSampRate = MTAUDDEC_FS_48K;
        break;
    case FS_64K:
        eMtSampRate = MTAUDDEC_FS_64K;
        break;
    case FS_88K:
        eMtSampRate = MTAUDDEC_FS_88K;
        break;
    case FS_96K:
        eMtSampRate = MTAUDDEC_FS_96K;
        break;
    case FS_176K:
        eMtSampRate = MTAUDDEC_FS_176K;
        break;
    case FS_192K:
        eMtSampRate = MTAUDDEC_FS_192K;
        break;
    case FS_8K:
        eMtSampRate = MTAUDDEC_FS_8K;
        break;
    case FS_11K:
        eMtSampRate = MTAUDDEC_FS_11K;
        break;
    case FS_12K:
        eMtSampRate = MTAUDDEC_FS_12K;
        break;
    default:
        break;
    }

    *pSmpRate = eMtSampRate;

    return MTR_OK;
}

/**Setup range for dynamix range compression.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1DrcLevel                 DRC range.
*                                  0 ~ 8 (step size is 0.125)
*                                  0 -> 0.0
*                                  1 -> 0.125
*                                  8 -> 1.0
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetAc3DrcRange (UINT8 u1DecId, UINT8 u1DrcLevel)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Range check
    if (u1DrcLevel > 8)
    {
        return MTR_ERR_INV;
    }

    _arAudDec[u1DecId].prAudSetting->u1DrcRange = u1DrcLevel;
    AUD_DspAC3DRCRange(u1DrcLevel, u1DecId);
    return MTR_OK;
}

/**Setup 0~100 range for dynamix range compression.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1DrcLevel                 DRC range 0~100.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetAc3DrcRange100 (UINT8 u1DecId, UINT8 u1DrcLevel)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Range check
    if (u1DrcLevel > 100 ||u1DrcLevel < 0)
    {
        return MTR_ERR_INV;
    }

    _arAudDec[u1DecId].prAudSetting->u1DrcRange = (UINT8)u1DrcLevel*8/100;
    AUD_DspAC3DRCRange100(u1DrcLevel, u1DecId);
    return MTR_OK;
}

/**Setup AC3 dual mono mode.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eDualMonoMode              Dual mono mode
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetAc3DualMono (UINT8 u1DecId,
                                    MTAUDDEC_AC3_DUALMONO_MODE eDualMonoMode)
{
    UINT16 u2Mode;
    AUD_DEC_ID_VALIDATE(u1DecId);

    switch (eDualMonoMode)
    {
    case MTAUDDEC_AC3_DUALMONO_STEREO:
        u2Mode = 0;
        break;
    case MTAUDDEC_AC3_DUALMONO_MONO_L:
        u2Mode = 1;
        break;
    case MTAUDDEC_AC3_DUALMONO_MONO_R:
        u2Mode = 2;
        break;
    case MTAUDDEC_AC3_DUALMONO_MIX_MONO:
        u2Mode = 3;
        break;
    default:
        u2Mode = 0;
    }

    _rAudSetting.eDualMonoMode = eDualMonoMode;

    AUD_DspAC3DualMono(u2Mode, u1DecId);
    return MTR_OK;
}


/**Setup AC3 dual mono mode.
* US/Austria model specific function
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eDualMonoMode              Dual mono mode
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_GetAc3DualMono (UINT8 u1DecId,
                                    MTAUDDEC_AC3_DUALMONO_MODE * peDualMonoMode)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    if (peDualMonoMode)
    {
        *peDualMonoMode = _rAudSetting.eDualMonoMode;
    }
    return MTR_OK;
}


/**Get the analog TV sound system of current channel.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peATVSoundSys              Sound system.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_GetAtvSoundSys (UINT8 u1DecId,
                                    MTAUDDEC_SIF_STANDARD_T *peATVSoundSys)
{
    TV_AUD_SYS_T eTvSys;
    MTAUDDEC_SIF_STANDARD_T eATVSoundSys;

    UNUSED(u1DecId);

    eTvSys = AUD_DspGetAtvTvSystem();

    switch (eTvSys)
    {
    case SV_NTSC_M:
        eATVSoundSys = MTAUDDEC_SIF_A2_KOREA;
        break;
    case SV_MTS:
        eATVSoundSys = MTAUDDEC_SIF_BTSC;
        break;
    case SV_FM_FM:
        eATVSoundSys = MTAUDDEC_SIF_EIAJ;
        break;
    case SV_A2_BG:
        eATVSoundSys = MTAUDDEC_SIF_BG_A2;
        break;
    case SV_A2_DK1:
        eATVSoundSys = MTAUDDEC_SIF_DK1_A2;
        break;
    case SV_A2_DK2:
        eATVSoundSys = MTAUDDEC_SIF_DK2_A2;
        break;
    case SV_A2_DK3:
        eATVSoundSys = MTAUDDEC_SIF_DK3_A2;
        break;
    case SV_PAL_I:
        eATVSoundSys = MTAUDDEC_SIF_I;
        break;
    case SV_PAL_BG:
        eATVSoundSys = MTAUDDEC_SIF_BG;
        break;
    case SV_PAL_DK:
        eATVSoundSys = MTAUDDEC_SIF_DK;
        break;
    case SV_SECAM_L:
    case SV_SECAM_L_PLUM:
        eATVSoundSys = MTAUDDEC_SIF_L;
        break;
    default:
        eATVSoundSys = MTAUDDEC_SIF_UNKNOWN;
        break;
    }

    *peATVSoundSys = eATVSoundSys;

    return MTR_OK;
}


/**Get the analog TV stereo mode of current channel.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param peATVMode                  Detected mode
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_GetAtvDetectMode (UINT8 u1DecId,
                                      MTAUDDEC_ATV_DETECT_MODE_T *peATVMode)
{
    MT_RESULT_T    eRet = MTR_OK;
    AUD_TYPE_T         eAudType;
    MTAUDDEC_ATV_DETECT_MODE_T  eAtvDetectMode = MTAUDDEC_ATV_DETECT_STEREO;

    AUD_DEC_ID_VALIDATE(u1DecId);
#ifdef CC_MT5363
    eAudType = DSP_GetAudChlInfo(AUD_D_DSP, u1DecId);
#else
    eAudType = DSP_GetAudChlInfo(u1DecId);
#endif

    switch (eAudType)
    {
    case AUD_TYPE_UNKNOWN:
        break;
    case AUD_TYPE_MONO:
        eAtvDetectMode = MTAUDDEC_ATV_DETECT_MONO;
        break;
    case AUD_TYPE_STEREO:
        eAtvDetectMode = MTAUDDEC_ATV_DETECT_STEREO;
        break;
    case AUD_TYPE_STEREO_SUB:
        eAtvDetectMode = MTAUDDEC_ATV_DETECT_STEREO_SAP;
        break;
    case AUD_TYPE_MONO_SUB:
        eAtvDetectMode = MTAUDDEC_ATV_DETECT_SAP;
        break;
        /*
            case MTAUDDEC_ATV_DETECT_SAP:
                eAtvDetectMode = MTAUDDEC_ATV_DETECT_SAP;
                break;
        */
    case AUD_TYPE_DUAL_MONO:
        eAtvDetectMode = MTAUDDEC_ATV_DETECT_DUAL;
        break;
    case AUD_TYPE_FM_MONO_NICAM_MONO:
        eAtvDetectMode = MTAUDDEC_ATV_DETECT_NICAM_MONO;
        break;
    case AUD_TYPE_FM_MONO_NICAM_STEREO:
        eAtvDetectMode = MTAUDDEC_ATV_DETECT_NICAM_STEREO;
        break;
    case AUD_TYPE_FM_MONO_NICAM_DUAL:
        eAtvDetectMode = MTAUDDEC_ATV_DETECT_NICAM_DUAL;
        break;
    default:
        eRet = MTR_ERR_INV;
        break;
    }

    *peATVMode = eAtvDetectMode;

    return eRet;
}

/**Setup analog TV output mode.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eSoundMode                 UNKNOWN = 0,
*                                  MONO,
*                                  STEREO,
*                                  SAP,
*                                  DUAL1,
*                                  DUAL2,
*                                  NICAM_MONO,
*                                  NICAM_STEREO,
*                                  NICAM_DUAL1,
*                                  NICAM_DUAL2,
*                                  A2_DUAL1,
*                                  A2_DUAL2,
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetAtvOutputMode (UINT8 u1DecId,
                                      MTAUDDEC_SOUND_MODE_T eSoundMode)
{
    AUD_SOUND_MODE_T  eOutputMode = AUD_SOUND_MODE_MONO;

    AUD_DEC_ID_VALIDATE(u1DecId);

    _arAudDec[u1DecId].prAudSetting->eOutputMode = eSoundMode;

    switch (eSoundMode)
    {
    case MTAUDDEC_SOUND_MODE_MONO:
        eOutputMode = AUD_SOUND_MODE_MONO;
        break;
    case MTAUDDEC_SOUND_MODE_STEREO:
        eOutputMode = AUD_SOUND_MODE_STEREO;
        break;
    case MTAUDDEC_SOUND_MODE_SAP:
        eOutputMode = AUD_SOUND_MODE_SUB_LANG;
        break;
    case MTAUDDEC_SOUND_MODE_DUAL1:
        eOutputMode = AUD_SOUND_MODE_DUAL1;
        break;
    case MTAUDDEC_SOUND_MODE_DUAL2:
        eOutputMode = AUD_SOUND_MODE_DUAL2;
        break;
    case MTAUDDEC_SOUND_MODE_NICAM_MONO:
        eOutputMode = AUD_SOUND_MODE_NICAM_MONO;
        break;
    case MTAUDDEC_SOUND_MODE_NICAM_STEREO:
        eOutputMode = AUD_SOUND_MODE_NICAM_STEREO;
        break;
    case MTAUDDEC_SOUND_MODE_NICAM_DUAL1:
        eOutputMode = AUD_SOUND_MODE_NICAM_DUAL1;
        break;
    case MTAUDDEC_SOUND_MODE_NICAM_DUAL2:
        eOutputMode = AUD_SOUND_MODE_NICAM_DUAL2;
        break;
    case MTAUDDEC_SOUND_MODE_A2_DUAL1_DUAL2:
        eOutputMode = AUD_SOUND_MODE_A2_DUAL1_DUAL2;
        break;
    default:
        break;
    }

#ifdef CC_MT5363
    AUD_DspSetAtvOutputMode(AUD_D_DSP, u1DecId, eOutputMode);
#else
    AUD_DspSetAtvOutputMode(u1DecId, eOutputMode);
#endif


    return MTR_OK;
}

/**Setup SIF input mode
* DVB EU model specific function
* Should set to audio decoder before issue playback at EU/KOREA/EIAJ model.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eSIFMode                   SIF Mode
*                                  NICAM BG,
*                                  FM BG,
*                                  A2 BG,
*                                  NICAM I,
*                                  FM I,
*                                  NICAM DK,
*                                  FM DK,
*                                  A2 DK1,
*                                  A2 DK3,
*                                  NICAM L,
*                                  AM L,
*                                  A2 KOREA
*                                  BTSC
*                                  EIAJ
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetSIFMode (UINT8 u1DecId,
                                MTAUDDEC_SIF_STANDARD_T eSIFMode)
{
    TV_AUD_SYS_T    eSifMode = SV_PAL_BG;

    AUD_DEC_ID_VALIDATE(u1DecId);

    _arAudDec[u1DecId].prAudSetting->eSetSifMode = eSIFMode;

    switch (eSIFMode)
    {
    case MTAUDDEC_SIF_BG:
        eSifMode = SV_PAL_BG;
        break;
    case MTAUDDEC_SIF_BG_A2:
        eSifMode = SV_A2_BG;
        break;
    case MTAUDDEC_SIF_I:
        eSifMode = SV_PAL_I;
        break;
    case MTAUDDEC_SIF_DK:
        eSifMode = SV_PAL_DK;
        break;
    case MTAUDDEC_SIF_DK1_A2:
        eSifMode = SV_A2_DK1;
        break;
    case MTAUDDEC_SIF_DK2_A2:
        eSifMode = SV_A2_DK2;
        break;
    case MTAUDDEC_SIF_DK3_A2:
        eSifMode = SV_A2_DK3;
        break;
    case MTAUDDEC_SIF_L:
        eSifMode = SV_SECAM_L;
        break;
    case MTAUDDEC_SIF_A2_KOREA:
        // FIXME
        eSifMode = SV_NTSC_M;
        break;
    case MTAUDDEC_SIF_BTSC:
        eSifMode = SV_MTS;
        break;
    case MTAUDDEC_SIF_EIAJ:
        // FIXME
        eSifMode = SV_FM_FM;
        break;
    default:
        break;
    }

    AUD_DspSetTvSrcMode(eSifMode);

    return MTR_OK;
}

/**Get detected SIF mode
* DVB EU model specific function
* Issue decoder to do SIF mode detection. It may take some time for the detection.
* Notes that application should stop decoder before invoking this function.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eSIFMode                   FM DK or AM L
*                                  Since audio decoder cannot distinguish from
*                                  FM DK and AM L, so application should set this
*                                  information to audio decoder at first before
*                                  issue decoder to do detection.
*@param peSIFMode                  Detection result
*                                  MODE_DETECT = 0,
*                                  NICAM BG,
*                                  FM BG,
*                                  A2 BG,
*                                  NICAM I,
*                                  FM I,
*                                  NICAM DK,
*                                  FM DK,
*                                  A2 DK1,
*                                  A2 DK3,
*                                  NICAM L,
*                                  AM L,
*                                  A2 KOREA
*                                  BTSC
*                                  EIAJ
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/

MT_RESULT_T  _MTAUD_GetDetectSIFMode (UINT8 u1DecId,
                                      MTAUDDEC_SIF_STANDARD_T eSIFMode,
                                      MTAUDDEC_SIF_STANDARD_T* peSIFMode)
{
#if 0
    TV_AUD_SYS_T  eAudSys = SV_NONE_DETECTED;
    MTAUDDEC_SIF_STANDARD_T  eSifMode = MTAUDDEC_SIF_UNKNOWN;

    AUD_DEC_ID_VALIDATE(u1DecId);
    MT_AUD_SEMA_LOCK(u1DecId);

    // Make sure decoder was stopped
    AUD_DSPCmdStop(MT_AUD_DEC_EU_SCART);
    _arAudDec[u1DecId].fgIssuePlay = FALSE;

    // Setup default format
    if (eSIFMode == MTAUDDEC_SIF_L)
    {
        DSP_SetDetTvSrcMode(SV_SECAM_L_PLUM);
        AUD_DspSetDetectDefaultMode(4); // SECAM-L or SECAM-L'
    }
    else
    {
        DSP_SetDetTvSrcMode(SV_SECAM_DK);
        AUD_DspSetDetectDefaultMode(0); // PAL-DK
    }

    // Do detection
    AUD_SetDecType(MT_AUD_DEC_EU_SCART, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_DETECTOR);
    AUD_DSPCmdPlay(MT_AUD_DEC_EU_SCART);

    // Get detection result
    eAudSys = DSP_GetDetTvSrcMode(eAudSys);

    // Stop decoder
    AUD_DSPCmdStop(MT_AUD_DEC_EU_SCART);

    switch (eAudSys)
    {
    case SV_NONE_DETECTED:
        eSifMode = MTAUDDEC_SIF_UNKNOWN;
        break;
    case SV_MTS:
        eSifMode = MTAUDDEC_SIF_BTSC;
        break;
    case SV_FM_FM:
        eSifMode = MTAUDDEC_SIF_A2_KOREA;
        break;
    case SV_NTSC_M:
        eSifMode = MTAUDDEC_SIF_BTSC;
        break;
    case SV_A2_BG:
        eSifMode = MTAUDDEC_SIF_BG_A2;
        break;
    case SV_A2_DK1:
        eSifMode = MTAUDDEC_SIF_DK1_A2;
        break;
    case SV_A2_DK2:
        eSifMode = MTAUDDEC_SIF_DK2_A2;
        break;
    case SV_A2_DK3:
        eSifMode = MTAUDDEC_SIF_DK3_A2;
        break;
    case SV_PAL_I:
        eSifMode = MTAUDDEC_SIF_I;
        break;
    case SV_PAL_BG:
        eSifMode = MTAUDDEC_SIF_BG;
        break;
    case SV_PAL_DK:
        eSifMode = MTAUDDEC_SIF_DK;
        break;
    case SV_SECAM_L:
        eSifMode = MTAUDDEC_SIF_L;
        break;
    case SV_SECAM_L_PLUM:
        eSifMode = MTAUDDEC_SIF_L;
        break;
    case SV_SECAM_BG:
        //eSifMode = MTAUDDEC_SIF_UNKNOWN;
        break;
    case SV_SECAM_DK:
        //eSifMode = MTAUDDEC_SIF_UNKNOWN;
        break;
    default:
        break;

    }

    *peSIFMode = eSifMode;

    // Load decoder format back
    AUD_SetDecType(MT_AUD_DEC_EU_SCART, _arAudDec[u1DecId].eStrSrc,
                   _arAudDec[u1DecId].eDecFmt);

    MT_AUD_SEMA_UNLOCK(u1DecId);
#endif

    AUD_FMT_T eAudFmt;

    AUD_DEC_ID_VALIDATE(u1DecId);
    MT_AUD_SEMA_LOCK(u1DecId);

    // Setup default format
    if (eSIFMode == MTAUDDEC_SIF_L)
    {
        DSP_SetDetTvSrcMode(SV_SECAM_L_PLUM);
        Aud_SetTvSystemMask(0, TV_SYS_MASK_L_PRIME);
#ifdef CC_MT5363
        AUD_DspSetDetectDefaultMode(AUD_D_DSP, 4); // SECAM-L or SECAM-L'
#else
        AUD_DspSetDetectDefaultMode(4); // SECAM-L or SECAM-L'
#endif
    }
    else
    {
        DSP_SetDetTvSrcMode(SV_SECAM_DK);
        Aud_SetTvSystemMask(0, TV_SYS_MASK_D|TV_SYS_MASK_K);
#ifdef CC_MT5363
        AUD_DspSetDetectDefaultMode(AUD_D_DSP, 0); // PAL-DK
#else
        AUD_DspSetDetectDefaultMode(0); // PAL-DK
#endif
    }

    eAudFmt = _AudAtvFmtDetection();

    switch (eAudFmt)
    {
    case AUD_FMT_A2:
        eSIFMode = MTAUDDEC_SIF_BG_A2;
        break;
    case AUD_FMT_PAL:
        eSIFMode = MTAUDDEC_SIF_DK;
        break;
    case AUD_FMT_MTS:
        eSIFMode = MTAUDDEC_SIF_BTSC;
        break;
    default:
        break;
    }
    *peSIFMode = eSIFMode;

    MT_AUD_SEMA_UNLOCK(u1DecId);


    return MTR_OK;
}

/**Setup PCM information.
* Used in audio clip playback.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param prPcmSetting               PCM setting
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetPcmInfo (UINT8 u1DecId,
                                MTAUDDEC_PCM_SETTING_T* prPcmSetting)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    return MTR_OK;
}

/**Setup individual channel delay
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChIndex                   Individual channel.
*@param u2Delay                    Delay (0~300 ms with 1 ms/step).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetChannelDelay (UINT8 u1DecId, MTAUDDEC_CH_T eChIndex,
                                     UINT16 u2Delay)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Parameter range checking
    if (u2Delay > AUD_CHL_DELAY_RANGE)
    {
        return MTR_ERR_INV;
    }

#ifdef CC_MT5363
    AUD_DspChannelDelay(AUD_A_DSP, _DelayFromMs(u2Delay), _AudDecCh2AudCh(eChIndex), u1DecId);
#else
    AUD_DspChannelDelay(_DelayFromMs(u2Delay), _AudDecCh2AudCh(eChIndex), u1DecId);
#endif

    return MTR_OK;
}

/**Setup individual channel delay no uop
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChIndex                   Individual channel.
*@param u2Delay                    Delay (0~300 ms with 1 ms/step).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetChannelDelayNoUop (UINT8 u1DecId, MTAUDDEC_CH_T eChIndex,
        UINT16 u2Delay)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Parameter range checking
    if (u2Delay > AUD_CHL_DELAY_RANGE)
    {
        return MTR_ERR_INV;
    }

#ifdef CC_MT5363
    AUD_DspChannelDelayNoUop(AUD_A_DSP, _DelayFromMs(u2Delay), _AudDecCh2AudCh(eChIndex), u1DecId);
#else
    AUD_DspChannelDelayNoUop(_DelayFromMs(u2Delay), _AudDecCh2AudCh(eChIndex), u1DecId);
#endif

    return MTR_OK;
}

/**Setup individual channel volume.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChIndex                   Individual channel.
*@param u1Value                    0~100.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetChannelVolume (UINT8 u1DecId, MTAUDDEC_CH_T eChannel,
                                      UINT8 u1Value)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Parameter range checking
    if (u1Value > 100)
    {
        return MTR_ERR_INV;
    }

#ifdef CC_MT5363
    if (u1DecId == AUD_DEC_AUX)
    {
        AUD_DspChannelVolume(AUD_D_DSP, u1DecId, _AudDecCh2AudCh(eChannel), u1Value);
    }
    else
    {
        AUD_DspChannelVolume(AUD_A_DSP, u1DecId, _AudDecCh2AudCh(eChannel), u1Value);
    }
#else
    AUD_DspChannelVolume(u1DecId, _AudDecCh2AudCh(eChannel), u1Value);
#endif

    return MTR_OK;
}

/**Setup audio description mode.
*@param u1DecId                   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Mode                    mix AD to L/R or downmix.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetADMode(UINT8 u1DecId,  UINT16 u1Mode)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    AUD_DspADModeSet(AUD_DEC_MAIN, u1Mode);
    return MTR_OK;
}

/**Setup audio description enable.
*@param u1DecId                   Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   enable/disable.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetADEnable(UINT8 u1DecId,  BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    AUD_DspADEnable(AUD_DEC_MAIN, fgEnable);
    return MTR_OK;
}

/**Mute individual channel
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChannel                   Individual channel.
*@param fgMute                     Mute or not.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetChannelMute (UINT8 u1DecId, MTAUDDEC_CH_T eChannel,
                                    BOOL fgMute)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

#ifdef CC_MT5363
    if (u1DecId == AUD_DEC_AUX)
    {
        AUD_DspChannelMute(AUD_D_DSP, u1DecId, _AudDecCh2AudCh(eChannel), fgMute);
    }
    else
    {
        AUD_DspChannelMute(AUD_A_DSP, u1DecId, _AudDecCh2AudCh(eChannel), fgMute);
    }
#else
    AUD_DspChannelMute(u1DecId, _AudDecCh2AudCh(eChannel), fgMute);
#endif

    return MTR_OK;
}

/**Setup extra gain for individual channel.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChannel                   Individual channel.
*@param i4Value                    -256~96. (-128 ~ 48 dB with 0.5 dB/step)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetChannelVolGain (UINT8 u1DecId, MTAUDDEC_CH_T eChannel,
                                       INT32 i4Value)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

#ifdef CC_MT5363
    if (u1DecId == AUD_DEC_AUX)
    {
        AUD_DspChannelVolGain(AUD_D_DSP, u1DecId, _AudDecCh2AudCh(eChannel), i4Value);
    }
    else
    {
        AUD_DspChannelVolGain(AUD_A_DSP, u1DecId, _AudDecCh2AudCh(eChannel), i4Value);
    }
#else
    AUD_DspChannelVolGain(u1DecId, _AudDecCh2AudCh(eChannel), i4Value);
#endif

    return MTR_OK;
}

/**Setup volume balance.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Balance                  Balance value (0~100).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetBalance (UINT8 u1DecId, UINT8 u1Balance)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Parameter range checking
    if (u1Balance > 100)
    {
        return MTR_ERR_INV;
    }

#ifdef CC_MT5363
    AUD_DspBalance(AUD_A_DSP, u1DecId, u1Balance);
#else
    AUD_DspBalance(u1DecId, u1Balance);
#endif

    return MTR_OK;
}


/**Setup master volume.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Volume                   Volume value (0~100).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetMasterVolume (UINT8 u1DecId, UINT8 u1Volume)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Parameter range checking
    if (u1Volume > 100)
    {
        return MTR_ERR_INV;
    }

#ifdef CC_MT5363
    AUD_DspMasterVolume(AUD_A_DSP, u1DecId, u1Volume);
#else
    AUD_DspMasterVolume(u1DecId, u1Volume);
#endif

    return MTR_OK;
}

/**Setup source volume.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/

MT_RESULT_T  _MTAUD_SetSourceVolume (UINT8 u1DecId)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    AUD_DspVolumeChange(u1DecId,AUD_CH_INPUT_SRC);
    return MTR_OK;
}

/**Setup Auto volume control (AVC).
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetAvc(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

#if 0
    AUD_DspAVCEnable(u1DecId, fgEnable);
#else
#ifdef CC_MT5363
    AUD_DspAVCEnable(AUD_A_DSP, 0, fgEnable);
    AUD_DspAVCEnable(AUD_A_DSP, 1, fgEnable);
#else
    AUD_DspAVCEnable(0, fgEnable);
    AUD_DspAVCEnable(1, fgEnable);
#endif
#endif

    return MTR_OK;
}

/**Setup 2 band AVC parameters
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Band                     Band number (0: Low band, 1: High band)
*@param eAVCCfgType                Parameter type
*@param i4Parameter                Parameter value
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_Set2BandAvcCfg(UINT8 u1DecId, UINT8 u1Band,
                                  MTAUDDEC_AVC_CFG_TYPE_T eAVCCfgType, INT32 i4Parameter)
{
    UINT16 u2Type = 0xff;
    AUD_DEC_ID_VALIDATE(u1DecId);

    switch (eAVCCfgType)
    {
    case MTAUDDEC_AVC_TARGET_LEVEL:
        u2Type = 1;
        break;
    case MTAUDDEC_AVC_MAX_GAIN_UP:
        u2Type = 3;
        break;
    case MTAUDDEC_AVC_ATTACK_RATE:
        u2Type = 0xff;  // not open in driver
        break;
    case MTAUDDEC_AVC_RELEASE_RATE:
        u2Type = 5;
        break;
    case MTAUDDEC_AVC_FC:
        u2Type = 0xff;
        break;
    default:
        u2Type = 0xff;
        break;
    }

#ifdef CC_MT5363
    AUD_DspChange2BandAVCPara(AUD_A_DSP, u1Band, u2Type, (UINT16)((INT16)i4Parameter), 0);
#else
    AUD_DspChange2BandAVCPara(u1Band, u2Type, (UINT16)((INT16)i4Parameter), 0);
#endif

    return MTR_OK;
}

/**Setup BBE
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetBBE(UINT8 u1DecId, BOOL fgEnable)
{
    UINT8 u1BbeMode = 0;

    AUD_DEC_ID_VALIDATE(u1DecId);

    u1BbeMode = fgEnable ? 1 : 0;

#ifdef CC_MT5363
    AUD_DspBbeMode(AUD_A_DSP, u1DecId, u1BbeMode);
#else
#ifdef CC_AUD_BBE_SUPPORT
    AUD_DspBbeMode(u1DecId, u1BbeMode);
#endif
#endif

    return MTR_OK;
}


/**Setup SRS
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetSRS(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    AUD_DspVirtualSurroundEnalbe(u1DecId, fgEnable);
    return MTR_OK;
}

/**Setup virtural surround
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetVirtualSurround(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    AUD_DspVirtualSurroundEnalbe(u1DecId, fgEnable);
    return MTR_OK;
}

/**Setup bass boost.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetBassBoost (UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

#ifdef CC_MT5363
    AUD_DspBassBoostEnable(AUD_A_DSP, u1DecId, fgEnable);
#else
    AUD_DspBassBoostEnable(u1DecId, fgEnable);
#endif
    return MTR_OK;
}

/**Setup bass
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2Bass                     bass value  ( +-14 steps, with 0.5 dB/step)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetBass (UINT8 u1DecId, INT16 i2Bass)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Parameter range checking
    if ((i2Bass > 14*2) || (i2Bass < -14*2))
    {
        return MTR_ERR_INV;
    }

#ifdef CC_AUD_3_5K_TREBLE
    // 0.5dB/step
#ifdef CC_MT5363
    AUD_DspBassBoostGain(AUD_A_DSP, u1DecId, i2Bass + 14*2);
#else
    AUD_DspBassBoostGain(u1DecId, i2Bass + 14*2);
#endif
#else
    // 1dB/step
#ifdef CC_MT5363
    AUD_DspBassBoostGain(AUD_A_DSP, u1DecId, i2Bass/2 + 14);
#else
    AUD_DspBassBoostGain(u1DecId, i2Bass/2 + 14);
#endif
#endif
    return MTR_OK;
}


/**Setup treble boost.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u2CompMode                 Compression mode.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetTrebleBoost(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

#ifdef CC_MT5363
    AUD_DspClearBoostEnable(AUD_A_DSP, u1DecId, fgEnable);
#else
    AUD_DspClearBoostEnable(u1DecId, fgEnable);
#endif
    return MTR_OK;
}

/**Setup Treble
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2Treble                   Treble value  ( +-14 steps, with 0.5 dB/step)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetTreble (UINT8 u1DecId, INT16 i2Treble)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Parameter range checking
    if ((i2Treble > 14*2) || (i2Treble < -14*2))
    {
        return MTR_ERR_INV;
    }

#ifdef CC_AUD_3_5K_TREBLE
    // 0.5dB/step
#ifdef CC_MT5363
    AUD_DspClearBoostGain(AUD_A_DSP, u1DecId, i2Treble + 14*2);
#else
    AUD_DspClearBoostGain(u1DecId, i2Treble + 14*2);
#endif
#else
    // 1dB/step
#ifdef CC_MT5363
    AUD_DspClearBoostGain(AUD_A_DSP, u1DecId, i2Treble + 14);
#else
    AUD_DspClearBoostGain(u1DecId, i2Treble + 14);
#endif
#endif

    return MTR_OK;
}

/**Setup EQ.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetEqualizer(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    _rAudSetting.fgEqEnable = fgEnable;

#ifdef CC_MT5363
    AUD_DspEqEnable(AUD_A_DSP, u1DecId, fgEnable);
#else
    AUD_DspEqEnable(u1DecId, fgEnable);
#endif

    return MTR_OK;
}

/**Setup EQ band configuration.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@param i2Band0                    band 0 setup ( +-28, with 0.5 dB/step)
*@param i2Band1                    band 1 setup ( +-28, with 0.5 dB/step)
*@param i2Band2                    band 2 setup ( +-28, with 0.5 dB/step)
*@param i2Band3                    band 3 setup ( +-28, with 0.5 dB/step)
*@param i2Band4                    band 4 setup ( +-28, with 0.5 dB/step)
*@param i2Band5                    band 5 setup ( +-28, with 0.5 dB/step)
*@param i2Band6                    band 6 setup ( +-28, with 0.5 dB/step)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetEqualizerBandConfig(UINT8 u1DecId, INT16 i2Band0,
        INT16 i2Band1, INT16 i2Band2, INT16 i2Band3,
        INT16 i2Band4, INT16 i2Band5, INT16 i2Band6)
{
    UINT8 u1BandChgNum = 0;
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Parameter range checking
    if ((i2Band0 > 28) || (i2Band0 < -28) ||
            (i2Band1 > 28) || (i2Band1 < -28) ||
            (i2Band2 > 28) || (i2Band2 < -28) ||
            (i2Band3 > 28) || (i2Band3 < -28) ||
            (i2Band4 > 28) || (i2Band4 < -28) ||
            (i2Band5 > 28) || (i2Band5 < -28) ||
            (i2Band6 > 28) || (i2Band6 < -28))
    {
        return MTR_ERR_INV;
    }

    // Check just individual band change or a set of band change
    if (i2Band0 != _rAudSetting.rEqBandSetting.i2Band0)
    {
        u1BandChgNum++;
    }
    if (i2Band1 != _rAudSetting.rEqBandSetting.i2Band1)
    {
        u1BandChgNum++;
    }
    if (i2Band2 != _rAudSetting.rEqBandSetting.i2Band2)
    {
        u1BandChgNum++;
    }
    if (i2Band3 != _rAudSetting.rEqBandSetting.i2Band3)
    {
        u1BandChgNum++;
    }
    if (i2Band4 != _rAudSetting.rEqBandSetting.i2Band4)
    {
        u1BandChgNum++;
    }
    if (i2Band5 != _rAudSetting.rEqBandSetting.i2Band5)
    {
        u1BandChgNum++;
    }
    if (i2Band6 != _rAudSetting.rEqBandSetting.i2Band6)
    {
        u1BandChgNum++;
    }

    if (u1BandChgNum == 1)  // Only one band change
    {
        if (i2Band0 != _rAudSetting.rEqBandSetting.i2Band0)
        {
#ifdef CC_MT5363
            AUD_DspSetUserDefinedEqCfg(AUD_A_DSP, u1DecId, AUD_EQ_CUST1, 0, i2Band0);
#else
            AUD_DspSetUserDefinedEqCfg(u1DecId, AUD_EQ_CUST1, 0, i2Band0);
#endif
        }
        if (i2Band1 != _rAudSetting.rEqBandSetting.i2Band1)
        {
#ifdef CC_MT5363
            AUD_DspSetUserDefinedEqCfg(AUD_A_DSP, u1DecId, AUD_EQ_CUST1, 0, i2Band1);
#else
            AUD_DspSetUserDefinedEqCfg(u1DecId, AUD_EQ_CUST1, 1, i2Band1);
#endif
        }
        if (i2Band2 != _rAudSetting.rEqBandSetting.i2Band2)
        {
#ifdef CC_MT5363
            AUD_DspSetUserDefinedEqCfg(AUD_A_DSP, u1DecId, AUD_EQ_CUST1, 0, i2Band2);
#else
            AUD_DspSetUserDefinedEqCfg(u1DecId, AUD_EQ_CUST1, 2, i2Band2);
#endif
        }
        if (i2Band3 != _rAudSetting.rEqBandSetting.i2Band3)
        {
#ifdef CC_MT5363
            AUD_DspSetUserDefinedEqCfg(AUD_A_DSP, u1DecId, AUD_EQ_CUST1, 0, i2Band3);
#else
            AUD_DspSetUserDefinedEqCfg(u1DecId, AUD_EQ_CUST1, 3, i2Band3);
#endif
        }
        if (i2Band4 != _rAudSetting.rEqBandSetting.i2Band4)
        {
#ifdef CC_MT5363
            AUD_DspSetUserDefinedEqCfg(AUD_A_DSP, u1DecId, AUD_EQ_CUST1, 0, i2Band4);
#else
            AUD_DspSetUserDefinedEqCfg(u1DecId, AUD_EQ_CUST1, 4, i2Band4);
#endif
        }
        if (i2Band5 != _rAudSetting.rEqBandSetting.i2Band5)
        {
#ifdef CC_MT5363
            AUD_DspSetUserDefinedEqCfg(AUD_A_DSP, u1DecId, AUD_EQ_CUST1, 0, i2Band5);
#else
            AUD_DspSetUserDefinedEqCfg(u1DecId, AUD_EQ_CUST1, 5, i2Band5);
#endif
        }
        if (i2Band6 != _rAudSetting.rEqBandSetting.i2Band6)
        {
#ifdef CC_MT5363
            AUD_DspSetUserDefinedEqCfg(AUD_A_DSP, u1DecId, AUD_EQ_CUST1, 0, i2Band6);
#else
            AUD_DspSetUserDefinedEqCfg(u1DecId, AUD_EQ_CUST1, 6, i2Band6);
#endif
        }

    }
#if 0
    else if (u1BandChgNum > 1) // A set of band change
    {
        AUD_DspSetUserDefinedSuitEqCfg7Bands(u1DecId, AUD_EQ_CUST1, i2Band0,
                                             i2Band1, i2Band2, i2Band3,i2Band4, i2Band5, i2Band6);

    }
#endif

    // Save to local varaibles
    _rAudSetting.rEqBandSetting.i2Band0 = i2Band0;
    _rAudSetting.rEqBandSetting.i2Band1 = i2Band1;
    _rAudSetting.rEqBandSetting.i2Band2 = i2Band2;
    _rAudSetting.rEqBandSetting.i2Band3 = i2Band3;
    _rAudSetting.rEqBandSetting.i2Band4 = i2Band4;
    _rAudSetting.rEqBandSetting.i2Band5 = i2Band5;
    _rAudSetting.rEqBandSetting.i2Band6 = i2Band6;


    return MTR_OK;
}

/**Setup line-in input mux
*@param  eInputId                  Input mux id.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetAvInputMuxSel (MTAUDDEC_INPUT_ID_T eInputId)
{
    MT_RESULT_T    eRet = MTR_OK;
    AUD_INPUT_ID_T eInput = AUD_INPUT_ID_MAX;

    switch (eInputId)
    {
    case MTAUDDEC_INPUT_ID_COMP_VID_0:
        eInput = AUD_INPUT_ID_COMP_VID_0;
        break;
    case MTAUDDEC_INPUT_ID_COMP_VID_1:
        eInput = AUD_INPUT_ID_COMP_VID_1;
        break;
    case MTAUDDEC_INPUT_ID_COMP_VID_2:
        eInput = AUD_INPUT_ID_COMP_VID_2;
        break;
    case MTAUDDEC_INPUT_ID_COMP_VID_3:
        eInput = AUD_INPUT_ID_COMP_VID_3;
        break;
    case MTAUDDEC_INPUT_ID_S_VID_0:
        eInput = AUD_INPUT_ID_S_VID_0;
        break;
    case MTAUDDEC_INPUT_ID_S_VID_1:
        eInput = AUD_INPUT_ID_S_VID_1;
        break;
    case MTAUDDEC_INPUT_ID_S_VID_2:
        eInput = AUD_INPUT_ID_S_VID_2;
        break;
    case MTAUDDEC_INPUT_ID_YPBPR_0:
        eInput = AUD_INPUT_ID_YPBPR_0;
        break;
    case MTAUDDEC_INPUT_ID_YPBPR_1:
        eInput = AUD_INPUT_ID_YPBPR_1;
        break;
    case MTAUDDEC_INPUT_ID_YPBPR_2:
        eInput = AUD_INPUT_ID_YPBPR_2;
        break;
    case MTAUDDEC_INPUT_ID_VGA_0:
        eInput = AUD_INPUT_ID_VGA_0;
        break;
    case MTAUDDEC_INPUT_ID_VGA_1:
        eInput = AUD_INPUT_ID_VGA_1;
        break;
    case MTAUDDEC_INPUT_ID_HDMI_0:
        eInput = AUD_INPUT_ID_HDMI_0;
        break;
    case MTAUDDEC_INPUT_ID_HDMI_1:
        eInput = AUD_INPUT_ID_HDMI_1;
        break;
    case MTAUDDEC_INPUT_ID_HDMI_2:
        eInput = AUD_INPUT_ID_HDMI_2;
        break;
    case MTAUDDEC_INPUT_ID_HDMI_3:
        eInput = AUD_INPUT_ID_HDMI_3;
        break;
    case MTAUDDEC_INPUT_ID_HDMI_4:
        eInput = AUD_INPUT_ID_HDMI_4;
        break;
    case MTAUDDEC_INPUT_ID_DVI_0:
        eInput = AUD_INPUT_ID_DVI_0;
        break;
    case MTAUDDEC_INPUT_ID_DVI_1:
        eInput = AUD_INPUT_ID_DVI_1;
        break;
    case MTAUDDEC_INPUT_ID_DVI_2:
        eInput = AUD_INPUT_ID_DVI_2;
        break;
    case MTAUDDEC_INPUT_ID_DVI_3:
        eInput = AUD_INPUT_ID_DVI_3;
        break;
    case MTAUDDEC_INPUT_ID_DVI_4:
        eInput = AUD_INPUT_ID_DVI_4;
        break;
    case MTAUDDEC_INPUT_ID_SCART_0:
        eInput = AUD_INPUT_ID_SCART_0;
        break;
    case MTAUDDEC_INPUT_ID_SCART_1:
        eInput = AUD_INPUT_ID_SCART_1;
        break;
    case MTAUDDEC_INPUT_ID_SCART_2:
        eInput = AUD_INPUT_ID_SCART_2;
        break;
    case MTAUDDEC_INPUT_ID_SCART_3:
        eInput = AUD_INPUT_ID_SCART_3;
        break;
    case MTAUDDEC_INPUT_ID_AUXIN_0:
        eInput = AUD_INPUT_ID_AUXIN_0;
        break;
    case MTAUDDEC_INPUT_ID_AUXIN_1:
        eInput = AUD_INPUT_ID_SCART_1;
        break;
    default:
        eRet = MTR_ERR_INV;
        break;
    }

    _arAudDec[MT_AUD_DEC_MAIN].prAudSetting->eAvMuxId = eInput;
    AUD_AvInputMuxSel(eInput);

    return eRet;
}

/**Setup line-in input mux by AD
*@param  eADNum                  Input mux ADC id.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetAvInputMuxSelByAD(UINT8 eADNum)
{
    MT_RESULT_T    eRet = MTR_OK;
    AUD_AvInputMuxSelByAD(eADNum);
    return eRet;
}

/**Setup Dual mode configuration.
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetDualMode (BOOL fgEnable)
{
    DSP_SetDualDecMode(fgEnable);
    AUD_DrvClkCfgDualDecMode(fgEnable);
    return MTR_OK;
}

/**Setup High Deveation mode configuration.
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetHDevMode (BOOL fgEnable)
{
    //AUD_DSPCmdStop(MT_AUD_DEC_AUX);
    AUD_DspSetHdevMode(AUD_ATV_DECODER_PAL, fgEnable);
    //AUD_DSPCmdPlay(MT_AUD_DEC_AUX);
    return MTR_OK;
}


/**Setup IEC configuration.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eIecCfg                    IEC format.
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetIecConfig (UINT8 u1DecId, MTAUDDEC_IEC_T eIecCfg,
                                  BOOL fgEnable)
{
    UNUSED(u1DecId);

    _arAudDec[u1DecId].prAudSetting->fgIecEnable = fgEnable;
    _arAudDec[u1DecId].prAudSetting->eIecFmt = eIecCfg;
    AUD_DspIECConfig(eIecCfg, fgEnable);
    return MTR_OK;
}

/**Setup IEC refer to which channel.
*@param eIecChannel                Channel number.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetIecChannel (MTAUDDEC_PCM_CH_T eChannel)
{
    AUD_IEC_CH_T eChannelId;

    switch (eChannel)
    {
    case MTAUDDEC_PCM_CH_L_R:
        eChannelId = AUD_IEC_PCM_CH_L_R;
        break;
    case MTAUDDEC_PCM_CH_LS_RS:
        eChannelId = AUD_IEC_PCM_CH_LS_RS;
        break;
    case MTAUDDEC_PCM_CH_C_SW:
        eChannelId = AUD_IEC_PCM_CH_C_SW;
        break;
    case MTAUDDEC_PCM_CH_BYPASS_L_R:
        eChannelId = AUD_IEC_PCM_CH_7_8;
        break;
    case MTAUDDEC_PCM_CH_DOWNMIX_L_R:
        eChannelId = AUD_IEC_PCM_CH_9_10;
        break;
    case MTAUDDEC_PCM_CH_SPEC_BYPASS_L_R:
        // Suppot multi-pair output(Copy CH9/10 to LS/RS/C/SUB)
        eChannelId = AUD_IEC_PCM_CH_LS_RS;
        break;
    case MTAUDDEC_PCM_CH_TV_SCART_L_R:
        eChannelId = AUD_IEC_PCM_CH_L_R;  // Not support yet
        break;
    default:
        eChannelId = AUD_IEC_PCM_CH_L_R;
    }

    AUD_DspIecChannel(eChannelId);

    return MTR_OK;
}

/**Setup IEC copyright.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetIecCopyright(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    _arAudDec[u1DecId].prAudSetting->fgIecCopyright = fgEnable;
#ifndef CC_MT5363
    AUD_DspIecCopyright(u1DecId, fgEnable);
#endif
    return MTR_OK;
}


/**Setup IEC copyright.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetSpdifSCMS(UINT8 spdif_ctrl)
{
    UINT8 copyright;

    switch (spdif_ctrl)
    {
    case 0:
        copyright = IEC_COPY_FREELY;
        break;
    case 2:
        copyright = IEC_COPY_ONCE;
        break;
    case 3:
    default:
        copyright = IEC_COPY_NO_MORE;
        break;
    }
    _arAudDec[AUD_DEC_MAIN].prAudSetting->fgIecCopyright = copyright;
#ifdef CC_AUD_SUPPORT_SPDIF_V20
    AUD_DspSetSpdifCopyright(AUD_STREAM_FROM_NUM, copyright);
#else
    AUD_DspSetSpdifCopyright(SPDIF_REG_TYPE_DEFAULT, AUD_STREAM_FROM_NUM, copyright);
    AUD_DspSetSpdifRegType (SPDIF_REG_TYPE_DEFAULT, AUD_STREAM_FROM_NUM);
#endif

    return MTR_OK;
}

/**Get IEC copyright.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pfgEnable                  on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_GetIecCopyright(UINT8 u1DecId, BOOL * pfgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    UNUSED(u1DecId);
    UNUSED(pfgEnable);
    //AUD_DspGetIecCopyright(u1DecId, pfgEnable);
    return MTR_OK;
}

/**Setup IEC category code
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1CategoryCode             Category code.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetIecCategoryCode(UINT8 u1DecId, UINT8 u1CategoryCode)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    _arAudDec[u1DecId].prAudSetting->u1IecCategoryCode = u1CategoryCode;
    //AUD_DspIecCategoryCode(u1DecId, u1CategoryCode);
    return MTR_OK;
}

/**Get IEC category code
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pu1CategoryCode            Category code.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_GetIecCategoryCode(UINT8 u1DecId, UINT8 * pu1CategoryCode)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    UNUSED(pu1CategoryCode);
    //AUD_DspGetIecCategoryCode(u1DecId, pu1CategoryCode);
    return MTR_OK;
}

/**Setup IEC category code
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1WordLength               Word length.(in bits, range: 16 or 24)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetIecWordLength(UINT8 u1DecId, UINT8 u1WordLength)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    UNUSED(u1WordLength);
    //vDspSetWordLength(u1WordLength);
    return MTR_OK;
}

/**Get IEC category code
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param pu1WordLength              Word length.(in bits, range: 16~24)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_GetIecWordLength(UINT8 u1DecId, UINT8 * pu1WordLength)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    UNUSED(pu1WordLength);
    //*pu1WordLength = DspGetWordLength();
    return MTR_OK;
}

/**Setup Speaker output mode.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eOutputMode                L+R = 0,
*                                  L+L,
*                                  R+R,
*                                  MIX
*
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetSpeakerOutMode (UINT8 u1DecId,
                                       MTAUDDEC_SKP_OUTPUT_MODE_T eOutputMode)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    _arAudDec[u1DecId].prAudSetting->eOutputMode = eOutputMode;

    switch (eOutputMode)
    {
    case MTAUDDEC_SPK_MODE_LR :
#ifdef CC_MT5363
        AUD_DspMonoDownMix(AUD_A_DSP, u1DecId, FALSE);
#else
        AUD_DspMonoDownMix(u1DecId, FALSE);
#endif
        break;
    case MTAUDDEC_SPK_MODE_LL:
#ifdef CC_MT5363
        AUD_DspMonoDownMix(AUD_A_DSP, u1DecId, TRUE);
        AUD_DspSetKaraokeMixRatio(AUD_A_DSP,u1DecId, 0x0);
#else
        AUD_DspMonoDownMix(u1DecId, TRUE);
        AUD_DspSetKaraokeMixRatio(u1DecId, 0x0);
#endif
        break;
    case MTAUDDEC_SPK_MODE_RR:
#ifdef CC_MT5363
        AUD_DspMonoDownMix(AUD_A_DSP,u1DecId, TRUE);
        AUD_DspSetKaraokeMixRatio(AUD_A_DSP,u1DecId, 0x800000);
#else
        AUD_DspMonoDownMix(u1DecId, TRUE);
        AUD_DspSetKaraokeMixRatio(u1DecId, 0x800000);
#endif
        break;
    case MTAUDDEC_SPK_MODE_MIX:
#ifdef CC_MT5363
        AUD_DspMonoDownMix(AUD_A_DSP,u1DecId, TRUE);
        AUD_DspSetKaraokeMixRatio(AUD_A_DSP,u1DecId, 0x400000);
#else
        AUD_DspMonoDownMix(u1DecId, TRUE);
        AUD_DspSetKaraokeMixRatio(u1DecId, 0x400000);
#endif
        break;
    default:
        break;
    }
    return MTR_OK;
}

/**Setup individual channel volume.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChIndex                   Individual channel.
*@param i2Value                    Volume value in 0.25dB/step.
*                                  Range: 80 ~ -512 (+20dB ~ -128dB)
*                                  80   ->  +20   dB
*                                  79   ->  +19.75dB
*                                  78   ->  +19.5 dB
*                                  77   ->  +19.25dB
*                                  76   ->  +19   dB
*                                   :        :
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetChannelVolumeInDB (UINT8 u1DecId,
        MTAUDDEC_CH_T eChannel, INT16 i2Value)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Range check
    if ((i2Value > 80) || (i2Value < -512))
    {
        return MTR_ERR_INV;
    }
    if (eChannel > MTAUDDEC_CH_NUM)
    {
        return MTR_ERR_INV;
    }

    printk("_MTAUD_SetChannelVolumeInDB = (%d)(%d) \n", eChannel, i2Value);

    VOL_CTL_SEMA_LOCK();
    if(eChannel<MTAUDDEC_CH_NUM)
    {
        _ai2ChlVolInDB[eChannel] = i2Value;
        _AudVolumeChange(_AudDecCh2AudCh(eChannel));
    }

    VOL_CTL_SEMA_UNLOCK();
    return MTR_OK;
}

/**Setup balance value in dB.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2LeftDB                   Volume value in 0.25dB/step.
*                                  Range: 0 ~ -512
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@param i2LeftDB                   Volume value in 0.25dB/step.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetBalanceInDB (UINT8 u1DecId, INT16 i2LeftDB,
                                    INT16 i2RightDB)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Range check
    if ((i2LeftDB > 0) || (i2LeftDB < -512) ||
            (i2RightDB > 0) || (i2RightDB < -512))
    {
        return MTR_ERR_INV;
    }

    VOL_CTL_SEMA_LOCK();

    printk("_MTAUD_SetBalanceInDB = (%d)(%d) \n", i2LeftDB, i2RightDB);

    _rAudSetting.i2BalanceLeftInDB = i2LeftDB;
    _rAudSetting.i2BalanceRightInDB = i2RightDB;

    _AudVolumeChange(AUD_CH_DMX_LEFT);
    _AudVolumeChange(AUD_CH_DMX_RIGHT);
    _AudVolumeChange(AUD_CH_FRONT_LEFT);
    _AudVolumeChange(AUD_CH_FRONT_RIGHT);

    VOL_CTL_SEMA_UNLOCK();
    return MTR_OK;
}



/**Setup master volume in dB.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2Volume                   Volume value in 0.25dB/step.
*                                  Range: 0 ~ -512
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetMasterVolumeInDB (UINT8 u1DecId, INT16 i2Volume)
{
    UINT32          u4VolRaw = 0;
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Volume range check
    if ((i2Volume > 0) || (i2Volume < -512))
    {
        return MTR_ERR_INV;
    }

    printk("_MTAUD_SetMasterVolumeInDB = (%d) \n", i2Volume);

    // Translate dB value to DSP raw format
    u4VolRaw = _DbToRaw(i2Volume);

#ifdef CC_MT5363
    AUD_DspMasterVolShmRawValue(AUD_A_DSP, u1DecId, u4VolRaw);
#else
    AUD_DspMasterVolShmRawValue(u1DecId, u4VolRaw);
#endif

    return MTR_OK;
}

/**Setup master volume for bypass channel in dB.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2Volume                   Volume value in 0.25dB/step.
*                                  Range: 0 ~ -512
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetMasterVolumeForBypassChannelInDB (UINT8 u1DecId,
        INT16 i2Volume)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

    // Range check
    if ((i2Volume > 0) || (i2Volume < -512))
    {
        return MTR_ERR_INV;
    }

    VOL_CTL_SEMA_LOCK();

    _rAudSetting.i2BypassMastVolumeInDB = i2Volume;

    _AudVolumeChange(AUD_CH_BYPASS_LEFT);
    _AudVolumeChange(AUD_CH_BYPASS_RIGHT);

    VOL_CTL_SEMA_UNLOCK();
    return MTR_OK;
}

/**Setup clipper threshold in dB.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eChIndex                   Individual channel.
*@param i2Value                    Volume value in 0.25dB/step.
*                                  Range: 0 ~ -512 (0dB ~ -128dB)
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetClipperThresholdInDB (UINT8 u1DecId,
        MTAUDDEC_CH_T eChannel, INT16 i2Value)
{
    UNUSED(u1DecId);
    UNUSED(eChannel);
    UNUSED(i2Value);
	return MTR_OK;
}

/**Setup pre-gain in dB.
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param i2Value                    Volume value in 0.25dB/step.
*                                  Range: 0 ~ -512 (0dB ~ -128dB)
*                                  0    ->  0     dB
*                                  -1   ->  -0.25 dB
*                                  -2   ->  -0.5  dB
*                                  -3   ->  -0.75 dB
*                                  -4   ->  -1    dB
*                                   :       :
*                                  -512 ->  -128  dB
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetPreGainInDB (UINT8 u1DecId, INT16 i2Value)
{
    INT32 i;
    AUD_DEC_ID_VALIDATE(u1DecId);

    for (i=0; i<AUD_STREAM_FROM_NUM; i++)
    {
#ifdef CC_MT5363
        AUD_DspSetSrcVol(AUD_A_DSP, u1DecId, (AUD_DEC_STREAM_FROM_T)i, i2Value/2);
#else
        AUD_DspSetSrcVol(u1DecId, (AUD_DEC_STREAM_FROM_T)i, i2Value/2);
#endif
    }
	return MTR_OK;
}

/**Setup AVC parameters
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eAVCCfgType                Parameter type
*@param i4Parameter                Parameter value
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_SetAvcCfg(UINT8 u1DecId,
                             MTAUDDEC_AVC_CFG_TYPE_T eAVCCfgType, INT32 i4Parameter)
{
    UINT16 u2Type = 0xff;
    AUD_DEC_ID_VALIDATE(u1DecId);

    switch (eAVCCfgType)
    {
    case MTAUDDEC_AVC_TARGET_LEVEL:
        u2Type = 1;
        break;
    case MTAUDDEC_AVC_MAX_GAIN_UP:
        u2Type = 3;
        break;
    case MTAUDDEC_AVC_ATTACK_RATE:
        u2Type = 0xff;  // not open in driver
        break;
    case MTAUDDEC_AVC_RELEASE_RATE:
        u2Type = 5;
        break;
    case MTAUDDEC_AVC_FC:
        u2Type = 0xff;
        break;
    default:
        u2Type = 0xff;
        break;
    }

#ifdef CC_MT5363
    AUD_DspChangeAVCPara(AUD_A_DSP,u2Type, (UINT16)((INT16)i4Parameter), 0);
#else
    AUD_DspChangeAVCPara(u2Type, (UINT16)((INT16)i4Parameter), 0);
#endif

    return MTR_OK;
}


/**Setup virtural surround parameters
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eVSCfgType                 Parameter type
*@param u4Parameter                Parameter value
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_SetVirtualSurroundCfg(UINT8 u1DecId,
        MTAUDDEC_VSURR_CFG_TYPE_T eVSCfgType, UINT32 u4Parameter)
{
    AUD_VSURR_CFG_TYPE_T eVsurConf;

    AUD_DEC_ID_VALIDATE(u1DecId);

    UNUSED(u1DecId);

    switch (eVSCfgType)
    {
    case MTAUDDEC_SUR_CLARITY:
        eVsurConf = AUD_SUR_CLARITY;
        break;
    case MTAUDDEC_SUR_WIDTH:
        eVsurConf = AUD_SUR_WIDTH;
        break;
    case MTAUDDEC_SUR_LRGAIN:
        eVsurConf = AUD_SUR_LRGAIN;
        break;
    case MTAUDDEC_SUR_XTALK:
        eVsurConf = AUD_SUR_XTALK;
        break;
    case MTAUDDEC_SUR_OUTPUT_GAIN:
        eVsurConf = AUD_SUR_OUTPUT_GAIN;
        break;
    case MTAUDDEC_SUR_BASS_GAIN:
        eVsurConf = AUD_SUR_BASS_GAIN;
        break;
    case MTAUDDEC_SUR_FO:
        eVsurConf = AUD_SUR_FO;
        break;
    default:
        eVsurConf = AUD_SUR_CLARITY;
        break;
    }

#ifdef CC_MT5363
    AUD_Surround_Set_Config(AUD_A_DSP,eVsurConf, u4Parameter);
#else
    AUD_Surround_Set_Config(eVsurConf, u4Parameter);
#endif

    return MTR_OK;
}


/**Setup bass enhancer switch
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   on/off.
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T  _MTAUD_SetBassEnhancer (UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

#ifdef CC_AUD_VBASS_SUPPORT
#ifdef CC_MT5363
    AUD_DspSetVirBassEnable(AUD_A_DSP, fgEnable);
#else
    AUD_DspSetVirBassEnable(fgEnable);
#endif
#endif

    return MTR_OK;
}

/**Setup bass enhancer parameers
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param eBassEnhancerType          Parameter type
*@param u4Parameter                Parameter value
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_SetBassEnhancerCfg(UINT8 u1DecId,
                                      MTAUDDEC_BE_PARAM_TYPE_T eBassEnhancerType, UINT32 u4Parameter)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
#ifdef CC_AUD_VBASS_SUPPORT
    UINT32 u4VbRefOutputGain = 0x20000/0x800;
    UINT32 u4VbRefBassGain = 0x20000/0x1000;
#endif

    switch (eBassEnhancerType)
    {
    case MTAUDDEC_BE_OUTPUT_GAIN:
#ifdef CC_AUD_VBASS_SUPPORT
        _u4VbOutputGain = u4Parameter;
#ifdef CC_MT5363
        AUD_DspSetVirBassCfg(AUD_A_DSP, u1DecId, 0,
                             u4Parameter/u4VbRefOutputGain, _u4VbBassGain/u4VbRefBassGain);
#else
        AUD_DspSetVirBassCfg(u1DecId, 0,
                             u4Parameter/u4VbRefOutputGain, _u4VbBassGain/u4VbRefBassGain);
#endif
#endif
        break;
    case MTAUDDEC_BE_BASS_GAIN:
#ifdef CC_AUD_VBASS_SUPPORT
        _u4VbBassGain = u4Parameter;
#ifdef CC_MT5363
        AUD_DspSetVirBassCfg(AUD_A_DSP, u1DecId, 0,
                             _u4VbOutputGain/u4VbRefOutputGain, u4Parameter/u4VbRefBassGain);
#else
        AUD_DspSetVirBassCfg(u1DecId, 0,
                             _u4VbOutputGain/u4VbRefOutputGain, u4Parameter/u4VbRefBassGain);
#endif
#endif
        break;
    default:
        break;
    }

    return MTR_OK;
}


/**Setup PEQ switch on/off
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   Enable
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_SetPEQ(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

#ifdef CC_MT5363
    AUD_DspSetPEQEnable(AUD_A_DSP, fgEnable);
#else
    AUD_DspSetPEQEnable(fgEnable);
#endif
    return MTR_OK;
}

/**Setup PEQ parameters
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Set                      PEQ set (0 ~ 7)
*@param u4Frequency                Frequency
*@param eQValue                    Q
*@param i2Gain                     Gain (+-12dB) with 0.5dB/step
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_SetPEQCfg(UINT8 u1DecId, UINT8 u1Set,
                             UINT32 u4Frequency, MTAUDDEC_PEQ_Q_TYPE_T eQValue, INT16 i2Gain)
{
    AUD_PEQ_Q_TYPE_T  eQ = AUD_PEQ_Q_MAX;
    AUD_DEC_ID_VALIDATE(u1DecId);

    switch (eQValue)
    {
    case MTAUDDEC_PEQ_Q_0_33:
        eQ = AUD_PEQ_Q_0_33;
        break;
    case MTAUDDEC_PEQ_Q_0_43:
        eQ = AUD_PEQ_Q_0_43;
        break;
    case MTAUDDEC_PEQ_Q_0_56:
        eQ = AUD_PEQ_Q_0_56;
        break;
    case MTAUDDEC_PEQ_Q_0_75:
        eQ = AUD_PEQ_Q_0_75;
        break;
    case MTAUDDEC_PEQ_Q_1_00:
        eQ = AUD_PEQ_Q_1_00;
        break;
    case MTAUDDEC_PEQ_Q_1_20:
        eQ = AUD_PEQ_Q_1_20;
        break;
    case MTAUDDEC_PEQ_Q_1_50:
        eQ = AUD_PEQ_Q_1_50;
        break;
    case MTAUDDEC_PEQ_Q_1_80:
        eQ = AUD_PEQ_Q_1_80;
        break;
    case MTAUDDEC_PEQ_Q_2_20:
        eQ = AUD_PEQ_Q_2_20;
        break;
    case MTAUDDEC_PEQ_Q_2_27:
        eQ = AUD_PEQ_Q_2_27;
        break;
    case MTAUDDEC_PEQ_Q_3_30:
        eQ = AUD_PEQ_Q_3_30;
        break;
    case MTAUDDEC_PEQ_Q_3_39:
        eQ = AUD_PEQ_Q_3_39;
        break;
    case MTAUDDEC_PEQ_Q_4_70:
        eQ = AUD_PEQ_Q_4_70;
        break;
    case MTAUDDEC_PEQ_Q_5_60:
        eQ = AUD_PEQ_Q_5_60;
        break;
    case MTAUDDEC_PEQ_Q_6_80:
        eQ = AUD_PEQ_Q_6_80;
        break;
    case MTAUDDEC_PEQ_Q_8_20:
        eQ = AUD_PEQ_Q_8_20;
        break;
    default:
        break;
    }

#ifdef CC_MT5363
    AUD_DspSetPEQCfg2(AUD_A_DSP, u1DecId, u1Set, u4Frequency, eQ, i2Gain);
#else
    AUD_DspSetPEQCfg2(u1DecId, u1Set, u4Frequency, eQ, i2Gain);
#endif

    return MTR_OK;
}

/**Setup Limiter switch on/off
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   Enable
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_SetLimiter(UINT8 u1DecId, BOOL fgEnable)
{
    UINT8 u1Mode;
    AUD_DEC_ID_VALIDATE(u1DecId);

    u1Mode = fgEnable ? 1 : 0;
#ifdef CC_MT5363
    AUD_DspSetLimiterMode(AUD_A_DSP, u1Mode);
#else
    AUD_DspSetLimiterMode(u1Mode);
#endif
    return MTR_OK;
}

/**Setup Limiter configuration
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u1Mode                     limiter mode
*                                  (0: off, 1: adaptive mode, 2: fixed mode).
*@param u1Flag                     limiter flag (0: off, 1: on)
*                                  bit 0: vol
*                                  bit 1: vsurr
*                                  bit 2: sbass
*                                  bit 3: EQ
*                                  bit 4: reverb
*                                  bit 5: virtual bass
*@param  u4Thresh                  limiter threshold (0x0 ~ 0x7fffff)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_SetLimiterCfg(UINT8 u1DecId, UINT8 u1Mode,
                                 UINT16 u2Flag, UINT32 u4Thresh)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

#ifdef CC_MT5363
    AUD_DspSetLimiterMode(AUD_A_DSP, u1Mode);
    AUD_DspSetLimiterFlag(AUD_A_DSP,(UINT8)u2Flag);
    AUD_DspSetLimiterThreshold(AUD_A_DSP, u4Thresh);
#else
    AUD_DspSetLimiterMode(u1Mode);
    AUD_DspSetLimiterFlag((UINT8)u2Flag);
    AUD_DspSetLimiterThreshold(u4Thresh);
#endif

    return MTR_OK;
}

/**Setup HPF Fc switch
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param fgEnable                   On/Off
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_SetHPF(UINT8 u1DecId, BOOL fgEnable)
{
    AUD_DEC_ID_VALIDATE(u1DecId);
    UNUSED(u1DecId);

#ifdef CC_MT5363
    AUD_DspSetHPFEnable(AUD_A_DSP, fgEnable);
#else
#ifdef CC_AUD_HPF_SUPPORT
    AUD_DspSetHPFEnable(fgEnable);
#endif
#endif
    return MTR_OK;
}

/**Setup HPF Fc
*@param u1DecId                    Decoder Id (AUD_DEC_MAIN or AUD_DEC_AUX).
*@param u2Fc                       Frequency (50 ~ 300 Hz)
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_SetHPFFc(UINT8 u1DecId, UINT16 u2Fc)
{
    AUD_DEC_ID_VALIDATE(u1DecId);

#ifdef CC_MT5363
    AUD_DspSetHPFFc(AUD_A_DSP, u2Fc);
#else
#ifdef CC_AUD_HPF_SUPPORT
    AUD_DspSetHPFFc(u2Fc);
#endif
#endif

    return MTR_OK;
}

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY
/**Mixound Play
*@param u4addr
*@param u4len
*@param u4gain                     0x20000 means 0dB
*@param u2config                   bit  0:3  samplerate  0: 48k   1: 44.1k 2: 32k 3: 24k 4:16k 5:12k
*                                            bit  4: source mode : 0->mono, 1->stereo
*                                            bit  5: 0: 8bit, 1 16bits
*                                            bit  6: 0: bigendian  1:littleendian
*                                            bit  7: 0: no repeat   1:repeat
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_MixSoundPlay(UINT32 u4addr, UINT32 u4len, UINT32 u4gain, UINT16 u2config)
{
    //INT32 i;
    //UINT32 *u4AddrTmp;
    UINT32 u1RepeatNum;
    UINT32 u1UpSample=0;
    UINT8 u1StereoOnOff=0;
    UINT8 u1EndianFlag=0;
    UINT8 u1Bit=0;
    //UINT8 u1Msb=0,u1Lsb=0;
    UINT32 u4SampleRate=0; //new interface for AUD_SetMixSndClip
    u1UpSample = (u2config & 0xf);
    u1StereoOnOff = (u2config & 0x10)>>4;
    u1Bit = (u2config & 0x20)>>5;
    u1EndianFlag= (u2config & 0x40)>>6;
    u1RepeatNum= (u2config & 0x80)>>7;
#if 0
    if (u1Bit)
    {
        for (i=0; i<u4len; i+=2)
        {
            u1Msb = (*((UINT8*)(u4AddrTmp+i)));
            u1Lsb = (*((UINT8*)(u4AddrTmp+i+1)));
            (*((UINT8*)(u4AddrTmp+i)))=(u1Lsb);
            (*((UINT8*)(u4AddrTmp+i+1)))=(u1Msb);
        }
    }
    else
    {
        for (i=0; i<u4len; i++)
        {
            u1Msb = (*((UINT8*)(u4AddrTmp+u4len-i)));
            (*((UINT8*)(u4AddrTmp+2*u4len-2*i)))=u1Msb;
            (*((UINT8*)(u4AddrTmp+2*u4len-2*i-1)))=0;
        }
    }
#endif

#if 1 //new interface for AUD_SetMixSndClip
    switch (u1UpSample)
    {
    case 0:
        u4SampleRate = 48000;
        break;
    case 1:
        u4SampleRate = 44100;
        break;
    case 2:
        u4SampleRate = 32000;
        break;
    case 3:
        u4SampleRate = 24000;
        break;
    case 4:
        u4SampleRate = 16000;
        break;
    case 5:
        u4SampleRate = 12000;
        break;
    default:
        u4SampleRate = 48000;
        break;

    };
#endif
    if (u1Bit)
    {
        u1Bit=16;
    }
    else
    {
        u1Bit=8;
    }
    AUD_EnableMixSndClip();
    if (u1RepeatNum!=0)
    {
        AUD_RepeatMixSndClip(0);
    }
    AUD_SetMixSndClip(0,u4addr, u4len, u4gain, u1StereoOnOff, u4SampleRate, u1Bit);
    AUD_PlayMixSndClip(0);

    return MTR_OK;
}

/**Mixound Stop
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_MixSoundStop(void)
{
    AUD_StopMixSndClip(0);
    #if !defined(CC_MT5368) && !defined(CC_MT5396) && !defined(CC_MT5389)    
    AUD_DisableMixSndClip();
    #endif

    return MTR_OK;
}

/**Mixound Pause
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_MixSoundPause(void)
{
    AUD_PauseMixSndClip(0);

    return MTR_OK;
}

/**Mixound Resume
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_MixSoundResume(void)
{
    AUD_ResumeMixSndClip(0);

    return MTR_OK;
}

#endif



extern void AUD_RecordPCM(UINT32 u4BufferSize, UINT32 u4ClipBufferPointer);
MT_RESULT_T _MTAUD_RecordPCM (UINT32 u4ClipBufferPointer, UINT32 u4BufferSize)
{
    Printf("_MTAUD_RecordPCM #1\n");
    AUD_RecordPCM(u4BufferSize, u4ClipBufferPointer);
    Printf("_MTAUD_RecordPCM #2\n");
    return MTR_OK;
}

MT_RESULT_T _MTAUD_DownloadDram (UINT32 u4BufPtr, UINT32 u4Addr, UINT32 u4Size)
{
    if(copy_to_user((void __user *)u4BufPtr, (VOID*)VIRTUAL(u4Addr), u4Size))
    {
		return MTR_NOT_OK;
    }
    return MTR_OK;
}

MT_RESULT_T _MTAUD_UploadDram (UINT32 u4BufPtr, UINT32 u4Addr, UINT32 u4Size)
{
    if(copy_from_user((VOID*)VIRTUAL(u4Addr), (void __user*)u4BufPtr, u4Size))
	{
		return MTR_NOT_OK;
	}
    return MTR_OK;
}
/**set DSP speed
*@param u4Mode                   step is 0.1, 10: normal speed; 5: 0.5X speed ; 20: 2X speed
*@retval MTR_OK                    Success.
*@retval MTR_NOT_OK                Fail.
*/
MT_RESULT_T _MTAUD_SetDSPSpeedMode(UINT32 u4Mode )
{
    #ifdef FRAC_SPEED_SUPPORT
    DSP_SpeedSetMode(u4Mode);
    #endif
    return MTR_OK;
}


/**Set the S/PDIF Serial Copying Management System (SCMS) output bits.
*@param c_bit                   bit2
*@param l_bit                    bit15
*@retval MTR_OK               Success.
*@retval MTR_NOT_OK       Fail.
*/
#define IEC_COPYRIGHT_DISABLE 0
#define IEC_COPYRIGHT_ENABLE 1
    
#define IEC_CATEGORY_LBIT_NUMBER 7
#define IEC_CATEGORY_LBIT (1 << IEC_CATEGORY_LBIT_NUMBER)
#define IEC_CATEGORY_LBIT_MASK (~IEC_CATEGORY_LBIT)

MT_RESULT_T  _MTAUD_SetSCMS(BOOL c_bit, BOOL l_bit)
{
    UINT8 u1CopyRight, u1CategoryCode;
    u1CopyRight = IEC_COPY_NEVER;
    u1CategoryCode = AUD_DspGetCategoryCode();
    
    if( c_bit && !l_bit)  //copy free
    {                                                 // AUD_DEC_COPY_PROTECT_NO_RESTRICT/* Cp-bit = 1, L-bit = 0 */
        u1CopyRight = IEC_COPY_FREELY;
        u1CategoryCode = u1CategoryCode & IEC_CATEGORY_LBIT_MASK;
    }
    else if( !c_bit && l_bit) 
    {                                               // case AUD_DEC_COPY_NO_FURTHER_COPY:      /* Cp-bit = 0, L-bit = 1 */
        u1CategoryCode = u1CategoryCode | IEC_CATEGORY_LBIT;
    }
    else if( !c_bit && !l_bit )
    {                                              //case AUD_DEC_COPY_PROTECT_COPY_ONCE:   /* Cp-bit = 0, L-bit = 0 */
        u1CategoryCode = u1CategoryCode & IEC_CATEGORY_LBIT_MASK;
    }
    else // copy never
    {                                             // case AUD_DEC_COPY_PROTECT_NEVER_COPY:  /* Cp-bit = 0, L-bit = 1 */        
        u1CopyRight = IEC_COPY_FREELY;
        u1CategoryCode = u1CategoryCode | IEC_CATEGORY_LBIT;
    }

    printk("_MTAUD_SetSCMS: copyright = %d, category code = 0x%x\n", u1CopyRight, u1CategoryCode);
    UNUSED(AUD_DspSetSCMS (u1CopyRight, u1CategoryCode));             

   return MTR_OK;
}

