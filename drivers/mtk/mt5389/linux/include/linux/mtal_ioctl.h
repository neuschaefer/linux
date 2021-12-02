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
 * Description:
 *
 *---------------------------------------------------------------------------*/
#ifndef MTAL_IOCTL_H
#define MTAL_IOCTL_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <linux/ioctl.h>

#ifndef SWDMX_DBG_USB
// #define SWDMX_DBG_USB
#endif

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/// MTAL_IOCTL_2ARG_T: Send 2 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[2];
} MTAL_IOCTL_2ARG_T;

/// MTAL_IOCTL_4ARG_T: Send 3 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[3];
} MTAL_IOCTL_3ARG_T;

/// MTAL_IOCTL_4ARG_T: Send 4 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[4];

} MTAL_IOCTL_4ARG_T;

typedef struct
{
    INT32 ai4Arg[5];

} MTAL_IOCTL_5ARG_T;

/// MTAL_IOCTL_6ARG_T: Send 6 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[6];
} MTAL_IOCTL_6ARG_T;

/// MTAL_IOCTL_7ARG_T: Send 7 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[7];
} MTAL_IOCTL_7ARG_T; // 20110801

/// MTAL_IOCTL_8ARG_T: Send 8 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[8];
} MTAL_IOCTL_8ARG_T;

/// MTAL_IOCTL_10ARG_T: Send 12 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[10];
} MTAL_IOCTL_10ARG_T;

/// MTAL_IOCTL_12ARG_T: Send 12 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[12];
} MTAL_IOCTL_12ARG_T;

/// MTAL_IOCTL_16ARG_T: Send 16 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[16];
} MTAL_IOCTL_16ARG_T;


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

/// MTAL device name
#define DEV_MTAL                        "mtal"

/*
#define _IOC_NRBITS	8
#define _IOC_TYPEBITS	8
#define _IOC_SIZEBITS	14
#define _IOC_DIRBITS	2
*/

#define MTAL_IOCTYPE_MTADAC           0
#define MTAL_IOCTYPE_MTAUDDEC            1
#define MTAL_IOCTYPE_MTDMX          2
#define MTAL_IOCTYPE_MTFE_ATSC           3
#define MTAL_IOCTYPE_MTFE_COMMON               4
#define MTAL_IOCTYPE_MTFE_DVBT          5
#define MTAL_IOCTYPE_MTFE_DVBC          6
#define MTAL_IOCTYPE_MTFE_NTSC            7
#define MTAL_IOCTYPE_MTFE_PAL          8
#define MTAL_IOCTYPE_MTGFX            9
#define MTAL_IOCTYPE_MTMPLAYER              10
#define MTAL_IOCTYPE_MTOSD            11
#define MTAL_IOCTYPE_MTPERIPHERAL           12
#define MTAL_IOCTYPE_MTPMX           13
#define MTAL_IOCTYPE_MTSCART         14
#define MTAL_IOCTYPE_MTVDEC            15
#define MTAL_IOCTYPE_MTVDECEX           16
#define MTAL_IOCTYPE_MTVDO            17
#define MTAL_IOCTYPE_MTVFE           18
#define MTAL_IOCTYPE_MTCI           19
#define MTAL_IOCTYPE_MTVDECEX_VBI   20
#define MTAL_IOCTYPE_MTTVE  21
#define MTAL_IOCTYPE_MTFE_DTMB          22
#define MTAL_IOCTYPE_MTFE_ISDBT          23
#define MTAL_IOCTYPE_MTFE_ISDBS          24
#define MTAL_IOCTYPE_MTFE_SBTVD          25
#define MTAL_IOCTYPE_MTIMAGE                26
#define MTAL_IOCTYPE_MTPVR               27
#define MTAL_IOCTYPE_MTIOMMU             28
#define MTAL_IOCTYPE_MTSTC                  29
#define MTAL_IOCTYPE_MTB2R                  30
#define MTAL_IOCTYPE_MTVSS                  31

/* MTAL_IOCTYPE_SDAUXOUT	0 */
#define MTAL_IO_ADAC_INIT                      _IOW(MTAL_IOCTYPE_MTADAC, 0, INT32)
#define MTAL_IO_ADAC_SETMUTE                   _IOW(MTAL_IOCTYPE_MTADAC, 1, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_ADAC_SETDACFORMATCFG           _IOW(MTAL_IOCTYPE_MTADAC, 2, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_ADAC_SETSPEAKERMUTE            _IOW(MTAL_IOCTYPE_MTADAC, 3, INT32)
#define MTAL_IO_ADAC_SETDACCHANNEL             _IOW(MTAL_IOCTYPE_MTADAC, 4, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_ADAC_SETAMPREGISTER            _IOW(MTAL_IOCTYPE_MTADAC, 5, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_ADAC_SETARCENABLE              _IOW(MTAL_IOCTYPE_MTADAC, 6, INT32)

/*MTAL_IOCTYPE_MTAUDDEC	1*/
#define MTAL_IO_AUDDEC_INIT                              _IOW(MTAL_IOCTYPE_MTAUDDEC, 0, INT32)
#define MTAL_IO_AUDDEC_GETDECTYPE                       _IOW(MTAL_IOCTYPE_MTAUDDEC, 1, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_SETDECTYPE                       _IOW(MTAL_IOCTYPE_MTAUDDEC, 2, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_PLAY                             _IOW(MTAL_IOCTYPE_MTAUDDEC, 3, INT32)
#define MTAL_IO_AUDDEC_RESUME                          _IOW(MTAL_IOCTYPE_MTAUDDEC, 4, INT32)
#define MTAL_IO_AUDDEC_PAUSE                            _IOW(MTAL_IOCTYPE_MTAUDDEC, 5, INT32)
#define MTAL_IO_AUDDEC_STOP                             _IOW(MTAL_IOCTYPE_MTAUDDEC, 6, INT32)
#define MTAL_IO_AUDDEC_GETDECONPLAYINGSTATUS          _IOW(MTAL_IOCTYPE_MTAUDDEC, 7, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETAVSYNMODE            _IOW(MTAL_IOCTYPE_MTAUDDEC, 8, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETAC3COMPMODE        _IOW(MTAL_IOCTYPE_MTAUDDEC, 9, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETAC3DOWNMIXMODE          _IOW(MTAL_IOCTYPE_MTAUDDEC, 10, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETAC3DRCRANGE         _IOW(MTAL_IOCTYPE_MTAUDDEC, 11, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_GETATVDETECTMODE             _IOW(MTAL_IOCTYPE_MTAUDDEC, 12, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETATVOUTPUTMODE             _IOW(MTAL_IOCTYPE_MTAUDDEC, 13, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETSIFMODE                           _IOW(MTAL_IOCTYPE_MTAUDDEC, 14, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_GETDETECTSIFMODE              _IOW(MTAL_IOCTYPE_MTAUDDEC, 15, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_SETPCMINFO                           _IOW(MTAL_IOCTYPE_MTAUDDEC, 16, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETCHANNELDELAY                 _IOW(MTAL_IOCTYPE_MTAUDDEC, 17, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_SETCHANNELVOLUME              _IOW(MTAL_IOCTYPE_MTAUDDEC, 18, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_SETCHANNELMUTE                  _IOW(MTAL_IOCTYPE_MTAUDDEC, 19, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_SETCHANNELVOLGAIN            _IOW(MTAL_IOCTYPE_MTAUDDEC, 20, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_SETBALANCE                 _IOW(MTAL_IOCTYPE_MTAUDDEC, 21, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETMASTERVOLUME      _IOW(MTAL_IOCTYPE_MTAUDDEC, 22, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETAVC                         _IOW(MTAL_IOCTYPE_MTAUDDEC, 23, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETBBE                         _IOW(MTAL_IOCTYPE_MTAUDDEC, 24, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETSRS                         _IOW(MTAL_IOCTYPE_MTAUDDEC, 25, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETVIRTUALSURROUND          _IOW(MTAL_IOCTYPE_MTAUDDEC, 26, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETBASSBOOST             _IOW(MTAL_IOCTYPE_MTAUDDEC, 27, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETBASS                        _IOW(MTAL_IOCTYPE_MTAUDDEC, 28, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETTREBLEBOOST         _IOW(MTAL_IOCTYPE_MTAUDDEC, 29, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETTREBLE                    _IOW(MTAL_IOCTYPE_MTAUDDEC, 30, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETEQUALIZER              _IOW(MTAL_IOCTYPE_MTAUDDEC, 31, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETEQUALIZERBANDCONFIG   _IOW(MTAL_IOCTYPE_MTAUDDEC, 32, MTAL_IOCTL_8ARG_T)
#define MTAL_IO_AUDDEC_SETAVINPUTMUXSEL      _IOW(MTAL_IOCTYPE_MTAUDDEC, 33, INT32)
#define MTAL_IO_AUDDEC_SETIECCONFIG               _IOW(MTAL_IOCTYPE_MTAUDDEC, 34, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_SETIECCOPYRIGHT         _IOW(MTAL_IOCTYPE_MTAUDDEC, 35, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETIECCATEGORYCODE  _IOW(MTAL_IOCTYPE_MTAUDDEC, 36, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETSPEAKEROUTMODE   _IOW(MTAL_IOCTYPE_MTAUDDEC, 37, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETDUALMODE      _IOW(MTAL_IOCTYPE_MTAUDDEC, 38, INT32)


#define MTAL_IO_AUDDEC_SETHPFFC                     _IOW(MTAL_IOCTYPE_MTAUDDEC, 39, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETHPF                       _IOW(MTAL_IOCTYPE_MTAUDDEC, 40, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETPEQCFG                    _IOW(MTAL_IOCTYPE_MTAUDDEC, 41, MTAL_IOCTL_6ARG_T)
#define MTAL_IO_AUDDEC_SETPEQ                       _IOW(MTAL_IOCTYPE_MTAUDDEC, 42, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETBASSENHANCERCFG           _IOW(MTAL_IOCTYPE_MTAUDDEC, 43, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_SETVIRTUALSURROUDCFG         _IOW(MTAL_IOCTYPE_MTAUDDEC, 44, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_SETAVCCFG                    _IOW(MTAL_IOCTYPE_MTAUDDEC, 45, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_SETMASTERVOLUMEFORBYPASSCHANNELINDB  _IOW(MTAL_IOCTYPE_MTAUDDEC, 46, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETMASTERVOLUMEINDB          _IOW(MTAL_IOCTYPE_MTAUDDEC, 47, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETBALANCEINDB               _IOW(MTAL_IOCTYPE_MTAUDDEC, 48, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_SETCHANNELVOLUMEINDB         _IOW(MTAL_IOCTYPE_MTAUDDEC, 49, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_SETBASSENHANCER              _IOW(MTAL_IOCTYPE_MTAUDDEC, 50, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETIECCHANNEL                _IOW(MTAL_IOCTYPE_MTAUDDEC, 51, INT32)
#define MTAL_IO_AUDDEC_SETLIMITER                   _IOW(MTAL_IOCTYPE_MTAUDDEC, 52, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETLIMITERCFG                _IOW(MTAL_IOCTYPE_MTAUDDEC, 53, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_AUDDEC_SETCLIPPERTHRESHOLDINDB      _IOW(MTAL_IOCTYPE_MTAUDDEC, 54, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_SETPREGAININDB               _IOW(MTAL_IOCTYPE_MTAUDDEC, 55, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_GETAC3DOWNMIXMODE            _IOW(MTAL_IOCTYPE_MTAUDDEC, 56, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_GETAC3MODE                   _IOW(MTAL_IOCTYPE_MTAUDDEC, 57, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_GETSAMPLINGRATE              _IOW(MTAL_IOCTYPE_MTAUDDEC, 58, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETAC3DUALMONO               _IOW(MTAL_IOCTYPE_MTAUDDEC, 59, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_GETAC3DUALMONO               _IOW(MTAL_IOCTYPE_MTAUDDEC, 60, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_GETIECCOPYRIGHT              _IOW(MTAL_IOCTYPE_MTAUDDEC, 61, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_GETIECCATEGORYCODE           _IOW(MTAL_IOCTYPE_MTAUDDEC, 62, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETIECWORDLENGTH             _IOW(MTAL_IOCTYPE_MTAUDDEC, 63, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_GETIECWORDLENGTH             _IOW(MTAL_IOCTYPE_MTAUDDEC, 64, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SET2BANDAVCCFG               _IOW(MTAL_IOCTYPE_MTAUDDEC, 65, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_AUDDEC_GETATVSOUNDSYS               _IOW(MTAL_IOCTYPE_MTAUDDEC, 66, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETHDEVMODE                  _IOW(MTAL_IOCTYPE_MTAUDDEC, 67, INT32)
#define MTAL_IO_AUDDEC_STOPALL                      _IOW(MTAL_IOCTYPE_MTAUDDEC, 68, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETCV                        _IOW(MTAL_IOCTYPE_MTAUDDEC, 69, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETCVCFG                     _IOW(MTAL_IOCTYPE_MTAUDDEC, 70, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETADMODE                    _IOW(MTAL_IOCTYPE_MTAUDDEC, 71, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_REGCB                        _IOW(MTAL_IOCTYPE_MTAUDDEC, 72, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_AUDDEC_SETMIXSOUNDPLAY              _IOW(MTAL_IOCTYPE_MTAUDDEC, 73, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_AUDDEC_SETMIXSOUNDSTOP              _IOW(MTAL_IOCTYPE_MTAUDDEC, 74, INT32)
#define MTAL_IO_AUDDEC_SETMIXSOUNDPAUSE             _IOW(MTAL_IOCTYPE_MTAUDDEC, 75, INT32)
#define MTAL_IO_AUDDEC_SETMIXSOUNDRESUME            _IOW(MTAL_IOCTYPE_MTAUDDEC, 76, INT32)
#define MTAL_IO_AUDDEC_SETSOURCEVOLUME              _IOW(MTAL_IOCTYPE_MTAUDDEC, 77, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETCHANNELDELAYNOUOP         _IOW(MTAL_IOCTYPE_MTAUDDEC, 78, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_SETADENABLE                  _IOW(MTAL_IOCTYPE_MTAUDDEC, 79, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_LOADAUDIOCLIP                _IOW(MTAL_IOCTYPE_MTAUDDEC, 80, MTAL_IOCTL_6ARG_T)
#define MTAL_IO_AUDDEC_PLAYAUDIOCLIP                _IOW(MTAL_IOCTYPE_MTAUDDEC, 81, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_STOPAUDIOCLIP                _IOW(MTAL_IOCTYPE_MTAUDDEC, 82, INT32)
#define MTAL_IO_AUDDEC_DELETEAUDIOCLIP              _IOW(MTAL_IOCTYPE_MTAUDDEC, 83, INT32)
#define MTAL_IO_AUDDEC_GETAUDIOPTS                  _IOW(MTAL_IOCTYPE_MTAUDDEC, 84, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETSPDIFSCMS                 _IOW(MTAL_IOCTYPE_MTAUDDEC, 85, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_GETAC3ESINFO                 _IOW(MTAL_IOCTYPE_MTAUDDEC, 86, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_GETMPEGESINFO                _IOW(MTAL_IOCTYPE_MTAUDDEC, 87, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_GETHEAACESINFO               _IOW(MTAL_IOCTYPE_MTAUDDEC, 88, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETAVINPUTMUXSELBYAD         _IOW(MTAL_IOCTYPE_MTAUDDEC, 89, INT32)
#define MTAL_IO_AUDDEC_SETAC3DRCRANGE100            _IOW(MTAL_IOCTYPE_MTAUDDEC, 90, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_GETATVSOUNDSYSSTRENGTH       _IOW(MTAL_IOCTYPE_MTAUDDEC, 91, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_SETDSPSPEEDMODE              _IOW(MTAL_IOCTYPE_MTAUDDEC, 92, INT32)
#define MTAL_IO_AUDDEC_SETSCMS                      _IOW(MTAL_IOCTYPE_MTAUDDEC, 93, MTAL_IOCTL_2ARG_T)

//0x80~ MTK specific tool for 3rd party
#define MTAL_IO_AUDDEC_RECORDPCM                    _IOW(MTAL_IOCTYPE_MTAUDDEC, 0x80, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_AUDDEC_DOWNLOAD_DRAM                _IOW(MTAL_IOCTYPE_MTAUDDEC, 0x81, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_AUDDEC_UPLOAD_DRAM                  _IOW(MTAL_IOCTYPE_MTAUDDEC, 0x82, MTAL_IOCTL_3ARG_T)

/* MTAL_IOCTYPE_DMXOUT 2 */
#define MTAL_IO_DMX_RESET                      _IOW(MTAL_IOCTYPE_MTDMX, 0, INT32)

#define MTAL_IO_DMX_SETFRONTENDEX              _IOW(MTAL_IOCTYPE_MTDMX, 1, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_START                      _IOW(MTAL_IOCTYPE_MTDMX, 2, INT32)
#define MTAL_IO_DMX_STOP                       _IOW(MTAL_IOCTYPE_MTDMX, 3, INT32)
#define MTAL_IO_DMX_SETPID                     _IOW(MTAL_IOCTYPE_MTDMX, 4, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_GETPID                     _IOW(MTAL_IOCTYPE_MTDMX, 5, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_FREEPID                    _IOW(MTAL_IOCTYPE_MTDMX, 6, INT32)
#define MTAL_IO_DMX_GETPIDIDX                  _IOW(MTAL_IOCTYPE_MTDMX, 7, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_SETVDEPID                  _IOW(MTAL_IOCTYPE_MTDMX, 8, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_SETAUDPID                  _IOW(MTAL_IOCTYPE_MTDMX, 9, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_SETPCRPID                  _IOW(MTAL_IOCTYPE_MTDMX, 10, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_SETPCR                     _IOW(MTAL_IOCTYPE_MTDMX, 11, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_SETFTR                     _IOW(MTAL_IOCTYPE_MTDMX, 12, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_GETFTR                     _IOW(MTAL_IOCTYPE_MTDMX, 13, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_PSI_ALCBFR                 _IOW(MTAL_IOCTYPE_MTDMX, 14, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_PSI_FREEBFR                _IOW(MTAL_IOCTYPE_MTDMX, 15, INT32)
#define MTAL_IO_DMX_PSI_GETBFR                 _IOW(MTAL_IOCTYPE_MTDMX, 16, MTAL_IOCTL_6ARG_T)
#define MTAL_IO_DMX_PSI_ULKBFR                 _IOW(MTAL_IOCTYPE_MTDMX, 17, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_DMX_PSI_FLSBFR                 _IOW(MTAL_IOCTYPE_MTDMX, 20, INT32)
#define MTAL_IO_DMX_PSI_GETBFRSECHEAD          _IOW(MTAL_IOCTYPE_MTDMX, 21, MTAL_IOCTL_6ARG_T)
#define MTAL_IO_DMX_PES_ALKBFR                 _IOW(MTAL_IOCTYPE_MTDMX, 22, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_PES_FREEBFR                _IOW(MTAL_IOCTYPE_MTDMX, 23, INT32)
#define MTAL_IO_DMX_PES_ULKBFR                 _IOW(MTAL_IOCTYPE_MTDMX, 24, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_DMX_PES_FLSBFR                 _IOW(MTAL_IOCTYPE_MTDMX, 25, INT32)
#define MTAL_IO_DMX_PES_GETBFR                 _IOW(MTAL_IOCTYPE_MTDMX, 26, MTAL_IOCTL_6ARG_T)
#define MTAL_IO_DMX_GETPIDCNT                  _IOW(MTAL_IOCTYPE_MTDMX, 27, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_RSTPIDCNT                  _IOW(MTAL_IOCTYPE_MTDMX, 28, INT32)
#define MTAL_IO_DMX_GETLOKSTE                  _IOW(MTAL_IOCTYPE_MTDMX, 29, INT32)
#define MTAL_IO_DMX_GETSCRMSTE                 _IOW(MTAL_IOCTYPE_MTDMX, 30, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_INIT                       _IOW(MTAL_IOCTYPE_MTDMX, 31, INT32)
#define MTAL_IO_DMX_SETVIDEOTYPE               _IOW(MTAL_IOCTYPE_MTDMX, 32, INT32)
#define MTAL_IO_DMX_GETCURSTC                  _IOW(MTAL_IOCTYPE_MTDMX, 33, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_SETGENERICFILTER           _IOW(MTAL_IOCTYPE_MTDMX, 34, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_GETGENERICFILTER           _IOW(MTAL_IOCTYPE_MTDMX, 35, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_SETAUDDESPID               _IOW(MTAL_IOCTYPE_MTDMX, 36, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_SETDESKEY                  _IOW(MTAL_IOCTYPE_MTDMX, 37, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_DMX_SETDESIV                   _IOW(MTAL_IOCTYPE_MTDMX, 38, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_SETAESKEYLEN               _IOW(MTAL_IOCTYPE_MTDMX, 39, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_SETAESKEY                  _IOW(MTAL_IOCTYPE_MTDMX, 40, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_SETAESIV                   _IOW(MTAL_IOCTYPE_MTDMX, 41, INT32)
#define MTAL_IO_DMX_SETDESCPID                 _IOW(MTAL_IOCTYPE_MTDMX, 42, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_SETAESEVENODDIV            _IOW(MTAL_IOCTYPE_MTDMX, 43, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_SETAESON                   _IOW(MTAL_IOCTYPE_MTDMX, 44, INT32)
#define MTAL_IO_DMX_AES_SETDCRINFO             _IOW(MTAL_IOCTYPE_MTDMX, 45, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_AES_MVBLKDATA              _IOW(MTAL_IOCTYPE_MTDMX, 46, INT32)
#define MTAL_IO_DMX_FVR_INIT                   _IOW(MTAL_IOCTYPE_MTDMX, 47, INT32)
#define MTAL_IO_DMX_FVR_SETGBL                 _IOW(MTAL_IOCTYPE_MTDMX, 48, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_FVR_GETGBL                 _IOW(MTAL_IOCTYPE_MTDMX, 49, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_FVR_SETPID                 _IOW(MTAL_IOCTYPE_MTDMX, 50, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_FVR_GETPID                 _IOW(MTAL_IOCTYPE_MTDMX, 51, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_FVR_FREEPID                _IOW(MTAL_IOCTYPE_MTDMX, 52, INT32)
#define MTAL_IO_DMX_FVR_UNLOCK_BUF             _IOW(MTAL_IOCTYPE_MTDMX, 53, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_FVR_FLUSH_BUF              _IOW(MTAL_IOCTYPE_MTDMX, 54, INT32)
#define MTAL_IO_DMX_FVR_START                  _IOW(MTAL_IOCTYPE_MTDMX, 55, INT32)
#define MTAL_IO_DMX_FVR_STOP                   _IOW(MTAL_IOCTYPE_MTDMX, 56, INT32)
#define MTAL_IO_DMX_FVR_ISRECORDING            _IOW(MTAL_IOCTYPE_MTDMX, 57, INT32)
#define MTAL_IO_DMX_FVR_ALLOCMETABUF           _IOW(MTAL_IOCTYPE_MTDMX, 58, INT32)
#define MTAL_IO_DMX_FVR_SETBUFFER              _IOW(MTAL_IOCTYPE_MTDMX, 59, INT32)
#define MTAL_IO_DMX_FVR_GETBUFFERPTR           _IOW(MTAL_IOCTYPE_MTDMX, 60, INT32)
#define MTAL_IO_DMX_FVR_FREEGBL                _IOW(MTAL_IOCTYPE_MTDMX, 61, INT32)
#define MTAL_IO_DMX_SETPID_INSRC               _IOW(MTAL_IOCTYPE_MTDMX, 62, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_FVR_SETKEY                 _IOW(MTAL_IOCTYPE_MTDMX, 63, MTAL_IOCTL_2ARG_T)

#define MTAL_IO_DMX_MULTI2_CFG                 _IOW(MTAL_IOCTYPE_MTDMX, 64, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_MULTI2_KEY                 _IOW(MTAL_IOCTYPE_MTDMX, 65, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_MULTI2_CBCIV               _IOW(MTAL_IOCTYPE_MTDMX, 66, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_MULTI2_OFBIV               _IOW(MTAL_IOCTYPE_MTDMX, 67, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_MULTI2_SYSKEY              _IOW(MTAL_IOCTYPE_MTDMX, 68, INT32)

#define MTAL_IO_DMX_SETDESRTBMODE              _IOW(MTAL_IOCTYPE_MTDMX, 69, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_SETAESRTBMODE              _IOW(MTAL_IOCTYPE_MTDMX, 70, MTAL_IOCTL_2ARG_T)

#define MTAL_IO_DMX_SETDESEVENODDIV            _IOW(MTAL_IOCTYPE_MTDMX, 71, MTAL_IOCTL_4ARG_T)

#define MTAL_IO_DMX_DREC_INIT                  _IOW(MTAL_IOCTYPE_MTDMX, 72, INT32)
#define MTAL_IO_DMX_DREC_SETPID                _IOW(MTAL_IOCTYPE_MTDMX, 73, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_DREC_UNLOCK_BUF            _IOW(MTAL_IOCTYPE_MTDMX, 74, INT32)
#define MTAL_IO_DMX_DREC_START                 _IOW(MTAL_IOCTYPE_MTDMX, 75, INT32)
#define MTAL_IO_DMX_DREC_STOP                  _IOW(MTAL_IOCTYPE_MTDMX, 76, INT32)
#define MTAL_IO_DMX_DREC_SETCIPHER             _IOW(MTAL_IOCTYPE_MTDMX, 77, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_DREC_GETSTREAMINFO         _IOW(MTAL_IOCTYPE_MTDMX, 78, INT32)
#define MTAL_IO_DMX_DREC_GETSTREAM             _IOW(MTAL_IOCTYPE_MTDMX, 79, INT32)
#define MTAL_IO_DMX_DREC_STOPSTREAM            _IOW(MTAL_IOCTYPE_MTDMX, 80, INT32)
#define MTAL_IO_DMX_DREC_FREEPID               _IOW(MTAL_IOCTYPE_MTDMX, 81, INT32)
#define MTAL_IO_DMX_DREC_GETBUFFERPTR          _IOW(MTAL_IOCTYPE_MTDMX, 82, INT32)
#define MTAL_IO_DMX_DREC_GETREMAINDER          _IOW(MTAL_IOCTYPE_MTDMX, 83, INT32)
#define MTAL_IO_DMX_DREC_SETCONFIG             _IOW(MTAL_IOCTYPE_MTDMX, 84, INT32)

#define MTAL_IO_DMX_MWIF_CALL                  _IOW(MTAL_IOCTYPE_MTDMX, 85, MTAL_IOCTL_2ARG_T)

#define MTAL_IO_DMX_SETFILEDESCPID             _IOW(MTAL_IOCTYPE_MTDMX, 86, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_SETPCRPIDEX                _IOW(MTAL_IOCTYPE_MTDMX, 87, MTAL_IOCTL_4ARG_T)

#define MTAL_IO_DMX_DREC_GETTOTALDNBUFFERSIZE  _IOW(MTAL_IOCTYPE_MTDMX, 88, INT32)
#define MTAL_IO_DMX_GCPU_CMD                   _IOW(MTAL_IOCTYPE_MTDMX, 89, MTAL_IOCTL_5ARG_T)
#define MTAL_IO_DMX_DREC_SETDESCPID            _IOW(MTAL_IOCTYPE_MTDMX, 90, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_DREC_GETPID                _IOW(MTAL_IOCTYPE_MTDMX, 91, MTAL_IOCTL_3ARG_T)

#define MTAL_IO_DMX_AES_ALLOCENGINE            _IOW(MTAL_IOCTYPE_MTDMX, 92, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_AES_FREEENGINE             _IOW(MTAL_IOCTYPE_MTDMX, 93, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_AES_SETDCRINFOEX           _IOW(MTAL_IOCTYPE_MTDMX, 94, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_AES_MVBLKDATAEX            _IOW(MTAL_IOCTYPE_MTDMX, 95, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_SET_PIDX_INSTANCE          _IOW(MTAL_IOCTYPE_MTDMX, 96, INT32)
#define MTAL_IO_DMX_FREE_PIDX                  _IOW(MTAL_IOCTYPE_MTDMX, 97, INT32)



#define MTAL_IO_DRM_WV_SET_ASSET_KEY           _IOW(MTAL_IOCTYPE_MTDMX, 192, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DRM_WV_DERIVE_CW               _IOW(MTAL_IOCTYPE_MTDMX, 193, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DRM_WV_PASS_KEYBOX             _IOW(MTAL_IOCTYPE_MTDMX, 194, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_BIM_GET_DEVICE_DATA            _IOW(MTAL_IOCTYPE_MTDMX, 195, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_BIM_DM_GET_KEY                 _IOW(MTAL_IOCTYPE_MTDMX, 196, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_BIM_GET_ULPK                   _IOW(MTAL_IOCTYPE_MTDMX, 197, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DMX_DVB_KEY                    _IOW(MTAL_IOCTYPE_MTDMX, 198, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DMX_DVB_IV                     _IOW(MTAL_IOCTYPE_MTDMX, 199, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_DMX_SETFRONTEND                _IOW(MTAL_IOCTYPE_MTDMX, 200, INT32)



/* MTAL_IOCTYPE_MTGFX	8 */
#define MTAL_IO_GFX_RESET						_IOW(MTAL_IOCTYPE_MTGFX, 0, INT32)
#define MTAL_IO_GFX_WAIT						_IOW(MTAL_IOCTYPE_MTGFX, 1, INT32)
#define MTAL_IO_GFX_FLUSH						_IOW(MTAL_IOCTYPE_MTGFX, 2, INT32)
#define MTAL_IO_GFX_FILLRECT					_IOW(MTAL_IOCTYPE_MTGFX, 3, INT32)
#define MTAL_IO_GFX_DRAWHLINE					_IOW(MTAL_IOCTYPE_MTGFX, 4, INT32)
#define MTAL_IO_GFX_DRAWVLINE					_IOW(MTAL_IOCTYPE_MTGFX, 5, INT32)
#define MTAL_IO_GFX_BITBLT						_IOW(MTAL_IOCTYPE_MTGFX, 6, INT32)
#define MTAL_IO_GFX_TRANSPARENTBITBLT			_IOW(MTAL_IOCTYPE_MTGFX, 7, INT32)
#define MTAL_IO_GFX_TRANSPARENTFILL				_IOW(MTAL_IOCTYPE_MTGFX, 8, INT32)
#define MTAL_IO_GFX_ALPHABLENDING				_IOW(MTAL_IOCTYPE_MTGFX, 9, INT32)
#define MTAL_IO_GFX_YBR2RGB						_IOW(MTAL_IOCTYPE_MTGFX, 10, INT32)
#define MTAL_IO_GFX_ALPHAMAPBITBLT				_IOW(MTAL_IOCTYPE_MTGFX, 11, INT32)
#define MTAL_IO_GFX_ROPBITBLT					_IOW(MTAL_IOCTYPE_MTGFX, 12, INT32)
#define MTAL_IO_GFX_ALPHACOMPOSITION			_IOW(MTAL_IOCTYPE_MTGFX, 13, INT32)
#define MTAL_IO_GFX_IDX2DIRBITBLT				_IOW(MTAL_IOCTYPE_MTGFX, 14, INT32)
#define MTAL_IO_GFX_ROTATE90					_IOW(MTAL_IOCTYPE_MTGFX, 15, INT32)
#define MTAL_IO_GFX_GETBUFFERPROPERTIES			_IOW(MTAL_IOCTYPE_MTGFX, 16, INT32)
#define MTAL_IO_GFX_SCALER			            _IOW(MTAL_IOCTYPE_MTGFX, 17, INT32)
#define MTAL_IO_GFX_RGB2YBR                     _IOW(MTAL_IOCTYPE_MTGFX, 18, INT32)
#define MTAL_IO_GFX_STRETCHBITBLT               _IOW(MTAL_IOCTYPE_MTGFX, 19, INT32)
#define MTAL_IO_GFX_GETBUFFERPATITION           _IOW(MTAL_IOCTYPE_MTGFX, 20, INT32)
#define MTAL_IO_GFX_STRETCHALPHACOMPOSITION	    _IOW(MTAL_IOCTYPE_MTGFX, 21, INT32)
#if (defined(CC_MT5368)||defined(CC_MT5396)|| defined(CC_MT5389)|| defined(CC_MT5398))
#define MTAL_IO_GFX_ALPHACOMPOSITION_IMPROVE	_IOW(MTAL_IOCTYPE_MTGFX, 22, INT32)
#endif
#define MTAL_IO_VGFX_SCALER	                    _IOW(MTAL_IOCTYPE_MTGFX, 23, INT32)
#define MTAL_IO_GFX_SET_MMU	                    _IOW(MTAL_IOCTYPE_MTGFX, 24, INT32)
#define MTAL_IO_GFX_LOCK	                    _IOW(MTAL_IOCTYPE_MTGFX, 25, INT32)
#define MTAL_IO_GFX_UNLOCK	                    _IOW(MTAL_IOCTYPE_MTGFX, 26, INT32)
#define MTAL_IO_GFX_SET_FLIPMIRROR              _IOW(MTAL_IOCTYPE_MTGFX, 27, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_GFX_COMPRESSBLIT	            _IOW(MTAL_IOCTYPE_MTGFX, 28, INT32)
#define MTAL_IO_GFX_SCALERCOMPOSITION		    _IOW(MTAL_IOCTYPE_MTGFX, 29, INT32)
#define MTAL_IO_GFX_GET_FBM		                _IOW(MTAL_IOCTYPE_MTGFX, 30, INT32)
#define MTAL_IO_VGFX_STRETCHBITBLT		        _IOW(MTAL_IOCTYPE_MTGFX, 31, INT32)
#define MTAL_IO_VGFX_STRETCHALPHACOMPOSITION    _IOW(MTAL_IOCTYPE_MTGFX, 32, INT32)
#define MTAL_IO_GFX_IMAGE_BITBLT	            _IOW(MTAL_IOCTYPE_MTGFX, 33, INT32)



/*MTAL_IOCTYPE_MTMPLAYER  9 */
#define MTAL_IO_MPLAYER_OPEN                          _IOW(MTAL_IOCTYPE_MTMPLAYER, 0, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MPLAYER_CLOSE                         _IOW(MTAL_IOCTYPE_MTMPLAYER, 1, INT32)
#define MTAL_IO_MPLAYER_RESET                         _IOW(MTAL_IOCTYPE_MTMPLAYER, 2, INT32)
#define MTAL_IO_MPLAYER_SET                           _IOW(MTAL_IOCTYPE_MTMPLAYER, 3, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MPLAYER_START                         _IOW(MTAL_IOCTYPE_MTMPLAYER, 4, INT32)
#define MTAL_IO_MPLAYER_STOP                          _IOW(MTAL_IOCTYPE_MTMPLAYER, 5, INT32)
#define MTAL_IO_MPLAYER_PAUSE                         _IOW(MTAL_IOCTYPE_MTMPLAYER, 6, INT32)
#define MTAL_IO_MPLAYER_RESUME                        _IOW(MTAL_IOCTYPE_MTMPLAYER, 7, INT32)
#define MTAL_IO_MPLAYER_GETINFO                       _IOW(MTAL_IOCTYPE_MTMPLAYER, 8, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MPLAYER_REGCB                         _IOW(MTAL_IOCTYPE_MTMPLAYER, 9, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_MPLAYER_SEEK                         _IOW(MTAL_IOCTYPE_MTMPLAYER, 10, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MPLAYER_DISPLAYIMG                   _IOW(MTAL_IOCTYPE_MTMPLAYER, 11, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_MPLAYER_GETBUFINFO                 _IOW(MTAL_IOCTYPE_MTMPLAYER, 12, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MPLAYER_DECIMG                     _IOW(MTAL_IOCTYPE_MTMPLAYER, 13, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MPLAYER_SUBMIT                     _IOW(MTAL_IOCTYPE_MTMPLAYER, 14, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_MPLAYER_GETIMG                     _IOW(MTAL_IOCTYPE_MTMPLAYER, 15, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MPLAYER_GETMETA                    _IOW(MTAL_IOCTYPE_MTMPLAYER, 16, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MPLAYER_STEP                       _IOW(MTAL_IOCTYPE_MTMPLAYER, 17, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MPLAYER_SETFIFO                    _IOW(MTAL_IOCTYPE_MTMPLAYER, 18, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MPLAYER_SETSPDISP                     _IOW(MTAL_IOCTYPE_MTMPLAYER, 19, MTAL_IOCTL_8ARG_T)
#define MTAL_IO_MPLAYER_ABORT                        _IOW(MTAL_IOCTYPE_MTMPLAYER, 20, INT32)
#define MTAL_IO_MPLAYER_SLIDESHOW                  _IOW(MTAL_IOCTYPE_MTMPLAYER, 21, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MPLAYER_CHKSUBTITLE                _IOW(MTAL_IOCTYPE_MTMPLAYER, 22, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MPLAYER_CONNECTIMG                  _IOW(MTAL_IOCTYPE_MTMPLAYER, 23, INT32)
#define MTAL_IO_MPLAYER_CLEANFB                  _IOW(MTAL_IOCTYPE_MTMPLAYER, 24, INT32)
#define MTAL_IO_MPLAYER_ASSIGNEXTSUBTITLE          _IOW(MTAL_IOCTYPE_MTMPLAYER, 25, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MPLAYER_GETDRMREGCODE              _IOW(MTAL_IOCTYPE_MTMPLAYER, 26, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MPLAYER_DRMDEACTIVATION            _IOW(MTAL_IOCTYPE_MTMPLAYER, 27, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MPLAYER_GETDRMUIHELPINFO           _IOW(MTAL_IOCTYPE_MTMPLAYER, 28, INT32)
#define MTAL_IO_MPLAYER_SETSRCTYPE                 _IOW(MTAL_IOCTYPE_MTMPLAYER, 29, INT32)
#define MTAL_IO_MPLAYER_SETNUMBSTM                 _IOW(MTAL_IOCTYPE_MTMPLAYER, 30, INT32)
#define MTAL_IO_MPLAYER_NETFLIXEOS                 _IOW(MTAL_IOCTYPE_MTMPLAYER, 31, INT32)
#define MTAL_IO_MPLAYER_GETDECODEDSIZE             _IOW(MTAL_IOCTYPE_MTMPLAYER, 32, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MPLAYER_SETEOS                     _IOW(MTAL_IOCTYPE_MTMPLAYER, 33, INT32)
#define MTAL_IO_MPLAYER_SEEK_POS                    _IOW(MTAL_IOCTYPE_MTMPLAYER, 34, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MPLAYER_GETCAP                      _IOW(MTAL_IOCTYPE_MTMPLAYER, 35, MTAL_IOCTL_3ARG_T)

#ifdef SWDMX_DBG_USB
#define MTAL_IO_MPLAYER_SWDMX                     _IOW(MTAL_IOCTYPE_MTMPLAYER, 36, MTAL_IOCTL_5ARG_T)
#endif

#define MTAL_IO_MPLAYER_IMAGE                      _IOW(MTAL_IOCTYPE_MTMPLAYER, 37, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MPLAYER_STILLIMAGE                 _IOW(MTAL_IOCTYPE_MTMPLAYER, 38, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MPLAYER_CONFIGIMGDECODER            _IOW(MTAL_IOCTYPE_MTMPLAYER, 39, INT32)

#define MTAL_IO_IMAGE_CONNECT                      _IOW(MTAL_IOCTYPE_MTIMAGE, 0, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_IMAGE_OPEN                      _IOW(MTAL_IOCTYPE_MTIMAGE, 1, INT32)
#define MTAL_IO_IMAGE_DECODE                      _IOW(MTAL_IOCTYPE_MTIMAGE, 2, MTAL_IOCTL_6ARG_T)
#define MTAL_IO_IMAGE_DISPLAY                      _IOW(MTAL_IOCTYPE_MTIMAGE, 3, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_IMAGE_CLOSE                      _IOW(MTAL_IOCTYPE_MTIMAGE, 4, INT32)
#define MTAL_IO_IMAGE_STOP                      _IOW(MTAL_IOCTYPE_MTIMAGE, 5, INT32)
#define MTAL_IO_IMAGE_SET                      _IOW(MTAL_IOCTYPE_MTIMAGE, 6, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_IMAGE_GETINFO                      _IOW(MTAL_IOCTYPE_MTIMAGE, 7, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_IMAGE_GETIMG                      _IOW(MTAL_IOCTYPE_MTIMAGE, 8, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_IMAGE_GETMETA                      _IOW(MTAL_IOCTYPE_MTIMAGE, 9, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_IMAGE_SLIDESHOW                      _IOW(MTAL_IOCTYPE_MTIMAGE, 10, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_IMAGE_CLEANFB                      _IOW(MTAL_IOCTYPE_MTIMAGE, 11, INT32)
#define MTAL_IO_IMAGE_GETCAP                      _IOW(MTAL_IOCTYPE_MTIMAGE, 12, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_IMAGE_FILLBUF                      _IOW(MTAL_IOCTYPE_MTIMAGE, 13, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_IMAGE_CONFIGIMGDECODER  _IOW(MTAL_IOCTYPE_MTIMAGE, 14, INT32)

/* MTAL_IOCTYPE_MTOSD	10 */
#define MTAL_IO_OSD_RESET						_IOW(MTAL_IOCTYPE_MTOSD, 0, INT32)
#define MTAL_IO_OSD_BASE_SETOSDPOSITION			_IOW(MTAL_IOCTYPE_MTOSD, 1, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_OSD_BASE_GETOSDPOSITION			_IOW(MTAL_IOCTYPE_MTOSD, 2, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_OSD_PLA_ENABLE					_IOW(MTAL_IOCTYPE_MTOSD, 3, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_OSD_PLA_FLIPTO					_IOW(MTAL_IOCTYPE_MTOSD, 4, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_OSD_PLA_GETBLENDLEVEL			_IOW(MTAL_IOCTYPE_MTOSD, 5, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_OSD_PLA_SETBLENDLEVEL			_IOW(MTAL_IOCTYPE_MTOSD, 6, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_OSD_PLA_GETFADING				_IOW(MTAL_IOCTYPE_MTOSD, 7, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_OSD_PLA_SETFADING				_IOW(MTAL_IOCTYPE_MTOSD, 8, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_OSD_PLA_DUMP					_IOW(MTAL_IOCTYPE_MTOSD, 9, INT32)
#define MTAL_IO_OSD_PLA_GETFIRSTREGION			_IOW(MTAL_IOCTYPE_MTOSD, 10, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_OSD_PLA_GETFIRSTREGIONADDRESS	_IOW(MTAL_IOCTYPE_MTOSD, 11, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_OSD_RGN_LIST_CREATE				_IOW(MTAL_IOCTYPE_MTOSD, 12, INT32)
#define MTAL_IO_OSD_RGN_LIST_DELETE				_IOW(MTAL_IOCTYPE_MTOSD, 13, INT32)
#define MTAL_IO_OSD_RGN_LIST_GETHEAD			_IOW(MTAL_IOCTYPE_MTOSD, 14, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_OSD_RGN_GET						_IOW(MTAL_IOCTYPE_MTOSD, 15, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_OSD_RGN_SET						_IOW(MTAL_IOCTYPE_MTOSD, 16, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_OSD_RGN_CREATE					_IOW(MTAL_IOCTYPE_MTOSD, 17, MTAL_IOCTL_12ARG_T)
#define MTAL_IO_OSD_RGN_DELETE					_IOW(MTAL_IOCTYPE_MTOSD, 18, INT32)
#define MTAL_IO_OSD_RGN_INSERT					_IOW(MTAL_IOCTYPE_MTOSD, 19, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_OSD_RGN_DETACH					_IOW(MTAL_IOCTYPE_MTOSD, 20, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_OSD_SC_SCALE					_IOW(MTAL_IOCTYPE_MTOSD, 21, MTAL_IOCTL_6ARG_T)
#define MTAL_IO_OSD_SC_GETSCALERINFO			_IOW(MTAL_IOCTYPE_MTOSD, 22, MTAL_IOCTL_6ARG_T)
#define MTAL_IO_OSD_SC_SETLPF					_IOW(MTAL_IOCTYPE_MTOSD, 23, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_OSD_GET_FB_RGN_LIST				_IOW(MTAL_IOCTYPE_MTOSD, 24, INT32)
#define MTAL_IO_OSD_GET_FB_RESOLUTION			_IOW(MTAL_IOCTYPE_MTOSD, 25, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_OSD_INIT					    _IOW(MTAL_IOCTYPE_MTOSD, 26, INT32)
#define MTAL_IO_OSD_SET_3D_ADDRESS				_IOW(MTAL_IOCTYPE_MTOSD, 27, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_OSD_PLA_SET_3D_MODE				_IOW(MTAL_IOCTYPE_MTOSD, 28, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_OSD_SET_SCROLLINFO				_IOW(MTAL_IOCTYPE_MTOSD, 29, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_OSD_SET_SCROLL_MODE				_IOW(MTAL_IOCTYPE_MTOSD, 30, INT32)
#define MTAL_IO_OSD_PLA_SET_3DREGION			_IOW(MTAL_IOCTYPE_MTOSD, 31, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_OSD_ENABLE_AUTOLR				_IOW(MTAL_IOCTYPE_MTOSD, 32, UINT32)
#define MTAL_IO_OSD_GET_LRSTATUS				_IOW(MTAL_IOCTYPE_MTOSD, 33, UINT32)
#define MTAL_IO_OSD_SET_TOPPLANE				_IOW(MTAL_IOCTYPE_MTOSD, 34, UINT32)
#define MTAL_IO_OSD_SET_PLA_LINESHIFT			_IOW(MTAL_IOCTYPE_MTOSD, 35, UINT32)
#define MTAL_IO_OSD_SET_FRAMEBUFFER_PROPERTY    _IOW(MTAL_IOCTYPE_MTOSD, 36, MTAL_IOCTL_5ARG_T)
#define MTAL_IO_OSD_GET_PLAN_VALID_COUNTER	    _IOW(MTAL_IOCTYPE_MTOSD, 37, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_OSD_WAIT_PLAN_STABLE			_IOW(MTAL_IOCTYPE_MTOSD, 38, MTAL_IOCTL_2ARG_T)



/*MTAL_IOCTYPE_MTPERIPHERAL 11*/
#define MTAL_IO_MTPERIPHERAL_GPIO_SETDIRECTION                         _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 0, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MTPERIPHERAL_GPIO_GETDIRECTION                         _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 1, INT32)
#define MTAL_IO_MTPERIPHERAL_GPIO_INTRQ                                      _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 2, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MTPERIPHERAL_GPIO_QUERY                                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 3, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_MTPERIPHERAL_GPIO_REG                                         _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 4, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MTPERIPHERAL_GPIO_SETOUT                                   _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 5, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MTPERIPHERAL_GPIO_GETIN                                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 6, INT32)
#define MTAL_IO_MTPERIPHERAL_PWM_SETFREQUENCY                       _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 7, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_MTPERIPHERAL_PDWNC_READSERVOVALUE               _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 8, INT32)
#define MTAL_IO_MTPERIPHERAL_SIF_INIT                                          _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 9, INT32)
#define MTAL_IO_MTPERIPHERAL_SIF_STOP                                        _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 10,INT32)
#define MTAL_IO_MTPERIPHERAL_SIF_WRITEMULTIPLE                         _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 11, MTAL_IOCTL_8ARG_T)
#define MTAL_IO_MTPERIPHERAL_SIF_READMULTIPLE                          _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 12, MTAL_IOCTL_8ARG_T)
#define MTAL_IO_MTPERIPHERAL_RTC_RTCTOUTC                                _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 13, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MTPERIPHERAL_RTC_UTCTORTC                                _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 14, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MTPERIPHERAL_RTC_INIT                                        _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 15, INT32)
#define MTAL_IO_MTPERIPHERAL_RTC_GETTIMEDATA                          _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 16, INT32)
#define MTAL_IO_MTPERIPHERAL_RTC_SETTIMEDATA                           _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 17, INT32)
#define MTAL_IO_MTPERIPHERAL_RTC_SETALERTTIME                         _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 18, INT32)
#define MTAL_IO_MTPERIPHERAL_RTC_GETALERTTIME                         _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 19, INT32)
#define MTAL_IO_MTPERIPHERAL_RTC_HOOKALERTHANDLER                _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 20, INT32)
#define MTAL_IO_MTPERIPHERAL_RTC_SETWAKEUP                             _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 21, INT32)
#define MTAL_IO_MTPERIPHERAL_NOR_ERASE                                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 22, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MTPERIPHERAL_NOR_ERASEADDR                             _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 23, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MTPERIPHERAL_NOR_GETSIZE                                 _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 24, INT32)
#define MTAL_IO_MTPERIPHERAL_NOR_READ                                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 25, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MTPERIPHERAL_NOR_WRITE                                   _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 26, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MTPERIPHERAL_NOR_WRONE                                  _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 27, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MTPERIPHERAL_NOR_CALCRC                                 _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 28, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MTPERIPHERAL_NAND_INIT                                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 29, INT32)
#define MTAL_IO_MTPERIPHERAL_NAND_RESET                                  _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 30, INT32)
#define MTAL_IO_MTPERIPHERAL_NAND_READID                               _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 31, INT32)
#define MTAL_IO_MTPERIPHERAL_NAND_GETSIZE                              _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 32, INT32)
#define MTAL_IO_MTPERIPHERAL_NAND_READBLOCKS                       _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 33, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MTPERIPHERAL_NAND_WRITEBLOCKS                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 34, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MTPERIPHERAL_IR_INIT                       _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 35, INT32)
#define MTAL_IO_MTPERIPHERAL_IR_POLL                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 36, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MTPERIPHERAL_PINMUX_SET                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 37, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MTPERIPHERAL_POWER_ENTERSTANDBY                       _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 38, INT32)
#define MTAL_IO_MTPERIPHERAL_POWER_SETWAKEUPSOURCE                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 39, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MTPERIPHERAL_GPIO_PERIODLEVELCALLBACK                   _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 40, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_MTPERIPHERAL_GPIO_INIT                                  _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 41, INT32)
#define MTAL_IO_MTPERIPHERAL_PWM_SETPHASE                       _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 42, MTAL_IOCTL_8ARG_T)
#define MTAL_IO_MTPERIPHERAL_DRAM_GETBANDWIDTHUSAGE             _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 43, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MTPERIPHERAL_T8032_CMD             _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 44, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MTPERIPHERAL_POWER_WAKEUP_REASON                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 45, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MTPERIPHERAL_IR_WAIT                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 46, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MTPERIPHERAL_IR_SET_RC56_SYSTEM_CODE     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 47, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_MTPERIPHERAL_BUS_TUNER_INIT              _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 48, INT32)
#define MTAL_IO_MTPERIPHERAL_IR_GETRAWDATA               _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 49, UINT32)
#define MTAL_IO_MTPERIPHERAL_IR_GETTouchPadRAWDATA               _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 50, UINT32)
#define MTAL_IO_MTPERIPHERAL_I2C_CHECK               _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 51, UINT32)
#define MTAL_IO_MTPERIPHERAL_WD_THREAD               _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 52, UINT32)


#define MTAL_IO_MTPERIPHERAL_SMC_INIT                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 50, INT32)
#define MTAL_IO_MTPERIPHERAL_SMC_GETCARDPRESENCE                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 51, INT32)
#define MTAL_IO_MTPERIPHERAL_SMC_RESET                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 52, INT32)
#define MTAL_IO_MTPERIPHERAL_SMC_SENDRECEIVEAPDU                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 53, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_MTPERIPHERAL_SMC_REQUESTIFS                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 54, INT32)
#define MTAL_IO_MTPERIPHERAL_SMC_ACTIVATE                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 55, INT32)
#define MTAL_IO_MTPERIPHERAL_SMC_DEACTIVATE                     _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 56, INT32)

#define MTAL_IO_MTPERIPHERAL_EEPROM_INIT                                          _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 60, INT32)
#define MTAL_IO_MTPERIPHERAL_EEPROM_READ                                          _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 61, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MTPERIPHERAL_EEPROM_WRITE                                          _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 62, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_MTPERIPHERAL_EEPROM_GETPROPERTY                                          _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 63, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MTPERIPHERAL_SCRBT_CHECK                       _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 64, INT32)
#define MTAL_IO_MTPERIPHERAL_SET_BW_PARAMETERS                       _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 65, MTAL_IOCTL_3ARG_T)

#define MTAL_IO_MTPERIPHERAL_SIFSW_INIT                              _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 66, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MTPERIPHERAL_SIFSW_WRITEDATA                         _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 67, MTAL_IOCTL_6ARG_T)
#define MTAL_IO_MTPERIPHERAL_SIFSW_READDATA                          _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 68, MTAL_IOCTL_6ARG_T)
#define MTAL_IO_MTPERIPHERAL_SIFSW_REMOVE                            _IOW(MTAL_IOCTYPE_MTPERIPHERAL, 69, INT32)
/* MTAL_IOCTYPE_MTPMX	12 */
#define MTAL_IO_PMX_INIT						_IOW(MTAL_IOCTYPE_MTPMX, 0, INT32)
#define MTAL_IO_PMX_GETPLANEORDER				_IOW(MTAL_IOCTYPE_MTPMX, 1, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_PMX_SETPLANEORDER				_IOW(MTAL_IOCTYPE_MTPMX, 2, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_PMX_SETBG						_IOW(MTAL_IOCTYPE_MTPMX, 3, INT32)
#define MTAL_IO_PMX_SETALPHA					_IOW(MTAL_IOCTYPE_MTPMX, 4, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_PMX_SETALLMUTE					_IOW(MTAL_IOCTYPE_MTPMX, 5, INT32)
#define MTAL_IO_PMX_PANEL_SETFRAMERATE			_IOW(MTAL_IOCTYPE_MTPMX, 6, INT32)
#define MTAL_IO_PMX_GETFRAMERATE				_IOW(MTAL_IOCTYPE_MTPMX, 7, INT32)
#define MTAL_IO_PMX_ENABLELVDSPOWER				_IOW(MTAL_IOCTYPE_MTPMX, 8, INT32)
#define MTAL_IO_PMX_ENABLEBACKLIGHTPOWER		_IOW(MTAL_IOCTYPE_MTPMX, 9, INT32)
#define MTAL_IO_PMX_ENABLELVDSSIGNAL			_IOW(MTAL_IOCTYPE_MTPMX, 10, INT32)
#define MTAL_IO_PMX_SETDRIVINGCURRENT			_IOW(MTAL_IOCTYPE_MTPMX, 11, INT32)
#define MTAL_IO_PMX_TVE_ENABLE					_IOW(MTAL_IOCTYPE_MTPMX, 12, INT32)
#define MTAL_IO_PMX_SETBACKLTDIMMING			_IOW(MTAL_IOCTYPE_MTPMX, 13, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_PMX_POWERSEQUENCE			_IOW(MTAL_IOCTYPE_MTPMX, 14, INT32)
#define MTAL_IO_PMX_PANEL_SELECT				_IOW(MTAL_IOCTYPE_MTPMX, 15, INT32)
#define MTAL_IO_PMX_PANEL_SETFILMMODE			_IOW(MTAL_IOCTYPE_MTPMX, 16, INT32)
#define MTAL_IO_PMX_PANEL_GETFILMMODE			_IOW(MTAL_IOCTYPE_MTPMX, 17, INT32)
#define MTAL_IO_PMX_SETSPREADSPECTRUM					_IOW(MTAL_IOCTYPE_MTPMX, 18, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_PMX_PANEL_GETRESOLUTION                 _IOW(MTAL_IOCTYPE_MTPMX, 19, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_PMX_PANEL_SYNCLOCK			_IOW(MTAL_IOCTYPE_MTPMX, 20, INT32)
#define MTAL_IO_PMX_PANEL_GETLOCKSTATUS			_IOW(MTAL_IOCTYPE_MTPMX, 21, INT32)
#define MTAL_IO_PMX_3D_MODE			_IOW(MTAL_IOCTYPE_MTPMX, 22, INT32)
#define MTAL_IO_PMX_3D_GET_LR_INFO			_IOW(MTAL_IOCTYPE_MTPMX, 23, INT32)
#define MTAL_IO_PMX_3D_SET_LR_POLARITY			_IOW(MTAL_IOCTYPE_MTPMX, 24, INT32)
#define MTAL_IO_PMX_SETPLANEORDERARRAY			_IOW(MTAL_IOCTYPE_MTPMX, 25, MTAL_IOCTL_8ARG_T)
#define MTAL_IO_PMX_PANEL_SETFRAMETRACK			_IOW(MTAL_IOCTYPE_MTPMX, 26, INT32)
#define MTAL_IO_PMX_PANEL_SETCOLORDEPTH			_IOW(MTAL_IOCTYPE_MTPMX, 27, INT32)
#define MTAL_IO_PMX_PANEL_SETLVDSSELECT			_IOW(MTAL_IOCTYPE_MTPMX, 28, INT32)
#define MTAL_IO_PMX_PANEL_SETLVDSPIXEL			_IOW(MTAL_IOCTYPE_MTPMX, 29, INT32)
#define MTAL_IO_PMX_PANEL_SETRESOLUTION			_IOW(MTAL_IOCTYPE_MTPMX, 30, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_PMX_PANEL_SETHORIPERIOD			_IOW(MTAL_IOCTYPE_MTPMX, 31, INT32)
#define MTAL_IO_PMX_PANEL_SETVERTPERIOD			_IOW(MTAL_IOCTYPE_MTPMX, 32, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_PMX_PANEL_SETVERTSYNCWIDTH		_IOW(MTAL_IOCTYPE_MTPMX, 33, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_PMX_PANEL_SETVERTBACKPROCH		_IOW(MTAL_IOCTYPE_MTPMX, 34, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_PMX_PANEL_GETPANELINDEX			_IOW(MTAL_IOCTYPE_MTPMX, 35, INT32)
#define MTAL_IO_PMX_GETPLANEORDERARRAY			_IOW(MTAL_IOCTYPE_MTPMX, 36, MTAL_IOCTL_8ARG_T)
#define MTAL_IO_PMX_MIX_ALPHA   			        _IOW(MTAL_IOCTYPE_MTPMX, 37, MTAL_IOCTL_4ARG_T)
/*add the new ioctl.h*/
#define MTAL_IO_PMX_GETBACKLIGHTPIXEL                       _IOW(MTAL_IOCTYPE_MTPMX, 38, INT32)
#define MTAL_IO_PMX_SETBACKLIGHTBRIGHT                         _IOW(MTAL_IOCTYPE_MTPMX, 39, INT32)
#define MTAL_IO_PMX_GETPWMOUTPUT                         _IOW(MTAL_IOCTYPE_MTPMX, 40, INT32)
#define MTAL_IO_PMX_GETDYNAMICALBKVALUE                 _IOW(MTAL_IOCTYPE_MTPMX, 41, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_PMX_GETAPLPIXEL                       _IOW(MTAL_IOCTYPE_MTPMX, 42, INT32)
#define MTAL_IO_PMX_GETBACKLIGHTWHITEPIXEL                       _IOW(MTAL_IOCTYPE_MTPMX, 43, INT32)


/* MTAL_IOCTYPE_MTSCART 13 */
#define MTAL_IO_SCART_SETINPMODE                        _IOW(MTAL_IOCTYPE_MTSCART, 0, INT32)
#define MTAL_IO_SCART_REGPIN8FUNC                       _IOW(MTAL_IOCTYPE_MTSCART, 1, INT32)
#define MTAL_IO_SCART_GETPIN8STE                        _IOW(MTAL_IOCTYPE_MTSCART, 2, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_SCART_GETINPMODE                        _IOW(MTAL_IOCTYPE_MTSCART, 3, INT32)
#define MTAL_IO_SCART_GETHWINPMODE                      _IOW(MTAL_IOCTYPE_MTSCART, 4, INT32)
#define MTAL_IO_SCART_SETRGBOVERLAY                     _IOW(MTAL_IOCTYPE_MTSCART, 5, INT32)
#define MTAL_IO_SCART_SETSCARTMATRIXENABLE                         _IOW(MTAL_IOCTYPE_MTSCART, 6, INT32)
#define MTAL_IO_SCART_SETSCARTMATRIXYOFFSET                         _IOW(MTAL_IOCTYPE_MTSCART, 7, INT32)
#define MTAL_IO_SCART_SETSCARTMATRIXRGBOFFSET                         _IOW(MTAL_IOCTYPE_MTSCART, 8, INT32)
#define MTAL_IO_SCART_SETSCARTRGBPORCH                         _IOW(MTAL_IOCTYPE_MTSCART, 9, INT32)
#define MTAL_IO_SCART_GETSCARTRGBPORCH                         _IOW(MTAL_IOCTYPE_MTSCART, 10, INT32)


/* MTAL_IOCTYPE_MTVDEC 14 */
#define MTAL_IO_VDEC_INIT                                _IOW(MTAL_IOCTYPE_MTVDEC, 0, INT32)
#define MTAL_IO_VDEC_TERMINT                             _IOW(MTAL_IOCTYPE_MTVDEC, 1, INT32)
#define MTAL_IO_VDEC_RESET                               _IOW(MTAL_IOCTYPE_MTVDEC, 2, INT32)
#define MTAL_IO_VDEC_PLAY                                _IOW(MTAL_IOCTYPE_MTVDEC, 3, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDEC_PLAY_I_FRAME                        _IOW(MTAL_IOCTYPE_MTVDEC, 4, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_VDEC_PAUSE                               _IOW(MTAL_IOCTYPE_MTVDEC, 5, INT32)
#define MTAL_IO_VDEC_STOP                                _IOW(MTAL_IOCTYPE_MTVDEC, 6, INT32)
#define MTAL_IO_VDEC_STOP_I_FRAME                        _IOW(MTAL_IOCTYPE_MTVDEC, 7, INT32)
#define MTAL_IO_VDEC_GETRESOLUTION                       _IOW(MTAL_IOCTYPE_MTVDEC, 8, MTAL_IOCTL_8ARG_T)
#define MTAL_IO_VDEC_SETAVSYNMODE                        _IOW(MTAL_IOCTYPE_MTVDEC, 9, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDEC_GETSTC                              _IOW(MTAL_IOCTYPE_MTVDEC, 10, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDEC_GETDECSTATUS                        _IOW(MTAL_IOCTYPE_MTVDEC, 11, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDEC_REGDECERRCB                         _IOW(MTAL_IOCTYPE_MTVDEC, 12, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDEC_REGUSERDATACB                       _IOW(MTAL_IOCTYPE_MTVDEC, 13, INT32)
#define MTAL_IO_VDEC_STARTUSERDATA                       _IOW(MTAL_IOCTYPE_MTVDEC, 14, INT32)
#define MTAL_IO_VDEC_STOPUSERDATA                        _IOW(MTAL_IOCTYPE_MTVDEC, 15, INT32)
#define MTAL_IO_VDEC_GETUSERDATA                         _IOW(MTAL_IOCTYPE_MTVDEC, 16, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDEC_REGPICTYPECB                        _IOW(MTAL_IOCTYPE_MTVDEC, 17, INT32)
#define MTAL_IO_VDEC_REGSTATUSCB                         _IOW(MTAL_IOCTYPE_MTVDEC, 18, INT32)
#define MTAL_IO_VDEC_GETDBKINFO                          _IOW(MTAL_IOCTYPE_MTVDEC, 19, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDEC_SETSPEED                            _IOW(MTAL_IOCTYPE_MTVDEC, 20, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDEC_SETFRAMEADVANCE                     _IOW(MTAL_IOCTYPE_MTVDEC, 21, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDEC_SETFLUSHESDATA                      _IOW(MTAL_IOCTYPE_MTVDEC, 22, INT32)
#define MTAL_IO_VDEC_SETDECODEMODE                       _IOW(MTAL_IOCTYPE_MTVDEC, 23, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDEC_DECODE_I_FRAME                      _IOW(MTAL_IOCTYPE_MTVDEC, 24, MTAL_IOCTL_7ARG_T)  // 20110801
#ifdef CC_ITIMING_ALWAYS
#define MTAL_IO_VDEC_SETFHDDISPLAY                       _IOW(MTAL_IOCTYPE_MTVDEC, 25, MTAL_IOCTL_2ARG_T)
#endif
#define MTAL_IO_VDEC_GETSRCSTATE                         _IOW(MTAL_IOCTYPE_MTVDEC, 26, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDEC_GETAFDSTATE                         _IOW(MTAL_IOCTYPE_MTVDEC, 27, MTAL_IOCTL_2ARG_T)



/* MTAL_IOCTYPE_MTVDECEX 31 */
#define MTAL_IO_VDECEX_ISPCTIMING                                _IOW(MTAL_IOCTYPE_MTVDECEX, 0, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DECEX_SETVIDEOSRC                                _IOW(MTAL_IOCTYPE_MTVDECEX, 1, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDECEX_SETMAINSUBSRC                             _IOW(MTAL_IOCTYPE_MTVDECEX, 2, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDECEX_SETCOLORSYSTEM                            _IOW(MTAL_IOCTYPE_MTVDECEX, 3, INT32)
#define MTAL_IO_VDECEX_GETCOLORSYSTEM                            _IOW(MTAL_IOCTYPE_MTVDECEX, 4, INT32)
#define MTAL_IO_VDECEX_SETPORCH                                  _IOW(MTAL_IOCTYPE_MTVDECEX, 5, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDECEX_GETPORCH                                  _IOW(MTAL_IOCTYPE_MTVDECEX, 6, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDECEX_STARTVGAAUTO                              _IOW(MTAL_IOCTYPE_MTVDECEX, 7, INT32)
#define MTAL_IO_VDECEX_STARTAUTOCOLOR                            _IOW(MTAL_IOCTYPE_MTVDECEX, 8, INT32)
#define MTAL_IO_VDECEX_SETPHASE                                  _IOW(MTAL_IOCTYPE_MTVDECEX, 9, INT32)
#define MTAL_IO_VDECEX_GETPHASE                                  _IOW(MTAL_IOCTYPE_MTVDECEX, 10, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDECEX_SETCLOCK                                  _IOW(MTAL_IOCTYPE_MTVDECEX, 11, INT32)
#define MTAL_IO_VDECEX_GETCLOCK                                  _IOW(MTAL_IOCTYPE_MTVDECEX, 12, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDECEX_VGA_GETAMBIGUOUSTIMINGNUM                 _IOW(MTAL_IOCTYPE_MTVDECEX, 13, INT32)
#define MTAL_IO_VDECEX_VGA_QUERYAMBIGUOUSTIMING                  _IOW(MTAL_IOCTYPE_MTVDECEX, 14, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDECEX_VGA_SETAMBIGUOUSINDEX                     _IOW(MTAL_IOCTYPE_MTVDECEX, 15, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDECEX_VGA_GETCURRENTAMBIGUOUSINDEX              _IOW(MTAL_IOCTYPE_MTVDECEX, 16, INT32)
#define MTAL_IO_VDECEX_REGCB                            			_IOW(MTAL_IOCTYPE_MTVDECEX, 17, MTAL_IOCTL_3ARG_T)
//#define MTAL_IO_VDECEX_REGSIGCHGNFYCB                            _IOW(MTAL_IOCTYPE_MTVDECEX, 17, INT32)
//#define MTAL_IO_VDECEX_REGVGAAUTOCB                              _IOW(MTAL_IOCTYPE_MTVDECEX, 18, INT32)
//#define MTAL_IO_VDECEX_REGAUTOCOLORCB                            _IOW(MTAL_IOCTYPE_MTVDECEX, 19, INT32)
#define MTAL_IO_VDECEX_HDMI_GETASPECTRATIO                   _IOW(MTAL_IOCTYPE_MTVDECEX, 19, INT32)
#define MTAL_IO_VDECEX_HDMI_SETHPDADJUST                         _IOW(MTAL_IOCTYPE_MTVDECEX, 20, INT32)
#define MTAL_IO_VDECEX_HDMI_GETINFOFRAME                         _IOW(MTAL_IOCTYPE_MTVDECEX, 21, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDECEX_HDMI_GETMODE                              _IOW(MTAL_IOCTYPE_MTVDECEX, 22, INT32)
#define MTAL_IO_VDECEX_HDMI_GETDEEPCOLORMODE                              _IOW(MTAL_IOCTYPE_MTVDECEX, 23, INT32)
#define MTAL_IO_VDECEX_STOPVGAAUTO				_IOW(MTAL_IOCTYPE_MTVDECEX, 24, INT32)
#define MTAL_IO_VDECEX_VGA_SETAMBIGUOUSUSRSEL		_IOW(MTAL_IOCTYPE_MTVDECEX, 25, INT32)
#define MTAL_IO_VDECEX_TVD_SETMAGC		_IOW(MTAL_IOCTYPE_MTVDECEX, 26, INT32)
#define MTAL_IO_VDECEX_TVD_SET2CHPGAGAIN		_IOW(MTAL_IOCTYPE_MTVDECEX, 27, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDECEX_TVD_SETAGCDIGITALGAIN		_IOW(MTAL_IOCTYPE_MTVDECEX, 28, INT32)
#define MTAL_IO_VDECEX_TVD_SETCAGC		_IOW(MTAL_IOCTYPE_MTVDECEX, 29, INT32)
#define MTAL_IO_VDECEX_TVD_SETUVDELAY		_IOW(MTAL_IOCTYPE_MTVDECEX, 30, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDECEX_TVD_SET2CHCLAMPLEVEL		_IOW(MTAL_IOCTYPE_MTVDECEX, 31, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDECEX_TVD_SET2CHAGCMODE		_IOW(MTAL_IOCTYPE_MTVDECEX, 32, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDECEX_TVD_GETSTATUS		_IOW(MTAL_IOCTYPE_MTVDECEX, 33, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDECEX_TVD_SETAAF		_IOW(MTAL_IOCTYPE_MTVDECEX, 34, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDECEX_HDMI_ISPCM                              _IOW(MTAL_IOCTYPE_MTVDECEX, 35, INT32)
#define MTAL_IO_VDECEX_HDMI_GETAUDIOFREQUENCY                              _IOW(MTAL_IOCTYPE_MTVDECEX, 36, INT32)
#define MTAL_IO_VDECEX_HDMI_GETDAUDIOBITWIDTH                              _IOW(MTAL_IOCTYPE_MTVDECEX, 37, INT32)
#define MTAL_IO_VDECEX_ISOVERSAMPLETIMING                                _IOW(MTAL_IOCTYPE_MTVDECEX, 38, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDECEX_SETOVERSAMPLEDEPENDENT                                _IOW(MTAL_IOCTYPE_MTVDECEX, 39, INT32)
#define MTAL_IO_VDECEX_GETOVERSAMPLEDEPENDENT                                _IOW(MTAL_IOCTYPE_MTVDECEX, 40, INT32)
#define MTAL_IO_VDECEX_SETAUTOSEARCH                         _IOW(MTAL_IOCTYPE_MTVDECEX, 41, INT32)
#define MTAL_IO_VDECEX_TVD_SETSECAMSETTING                         _IOW(MTAL_IOCTYPE_MTVDECEX, 42, INT32)
#define MTAL_IO_VDECEX_STARTCVBSAUTOCOLOR                            _IOW(MTAL_IOCTYPE_MTVDECEX, 43, INT32)
#define MTAL_IO_VDECEX_TVD_HOLDMODE                            _IOW(MTAL_IOCTYPE_MTVDECEX, 44, INT32)
#define MTAL_IO_VDECEX_HDMI_GETDAUDIOCATEGORYCODE                    _IOW(MTAL_IOCTYPE_MTVDECEX, 45, INT32)
#define MTAL_IO_VDECEX_HDMI_GETCOLORDOMAIN                      _IOW(MTAL_IOCTYPE_MTVDECEX, 46, INT32)
#define MTAL_IO_VDECEX_TVD_PEDESTAL_EN                            _IOW(MTAL_IOCTYPE_MTVDECEX, 47, INT32)
#define MTAL_IO_VDECEX_GETVIDEODECODERTYPE                            _IOW(MTAL_IOCTYPE_MTVDECEX, 48, INT32)
#define MTAL_IO_VDECEX_TVD_GETDEFAULTPORCH                                  _IOW(MTAL_IOCTYPE_MTVDECEX, 49, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDECEX_TVD_SETCHANNELCHANGE                                  _IOW(MTAL_IOCTYPE_MTVDECEX, 50, INT32)
#define MTAL_IO_VDECEX_TVD_SETENABLEDCOLORSYSTEM                                  _IOW(MTAL_IOCTYPE_MTVDECEX, 51, INT32)
#define MTAL_IO_VDECEX_TVD_SETVPRESONOFFGAIN                                  _IOW(MTAL_IOCTYPE_MTVDECEX, 52, INT32)
#define MTAL_IO_VDECEX_TVD_SETHPLL                                  _IOW(MTAL_IOCTYPE_MTVDECEX, 53, INT32)
#define MTAL_IO_VDECEX_TVD_GETTVDNOISELEVEL                       _IOW(MTAL_IOCTYPE_MTVDECEX, 54, INT32)
#define MTAL_IO_VDECEX_YPBPR_SETYPBPRTOVGA                       _IOW(MTAL_IOCTYPE_MTVDECEX, 55, INT32)



#define MTAL_IO_VDECEX_VBI_STARTNOTIFYCCONOFF                    _IOW(MTAL_IOCTYPE_MTVDECEX_VBI, 0, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDECEX_VBI_STARTNOTIFYTTXONOFF                   _IOW(MTAL_IOCTYPE_MTVDECEX_VBI, 1, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDECEX_VBI_STARTNOTIFYWSSONOFF                   _IOW(MTAL_IOCTYPE_MTVDECEX_VBI, 2, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDECEX_VBI_STARTNOTIFYVPSONOFF                   _IOW(MTAL_IOCTYPE_MTVDECEX_VBI, 3, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDECEX_VBI_SETTTXNFY                             _IOW(MTAL_IOCTYPE_MTVDECEX_VBI, 4, INT32)
#define MTAL_IO_VDECEX_VBI_SETCCNFY                              _IOW(MTAL_IOCTYPE_MTVDECEX_VBI, 5, INT32)
#define MTAL_IO_VDECEX_VBI_SETVPSNFY                             _IOW(MTAL_IOCTYPE_MTVDECEX_VBI, 6, INT32)
#define MTAL_IO_VDECEX_VBI_SETWSSNFY                             _IOW(MTAL_IOCTYPE_MTVDECEX_VBI, 7, INT32)
#define MTAL_IO_VDECEX_VBI_SETINFONFY                            _IOW(MTAL_IOCTYPE_MTVDECEX_VBI, 8, INT32)
#define MTAL_IO_VDECEX_VBI_GETVBIFIFOINFO                        _IOW(MTAL_IOCTYPE_MTVDECEX_VBI, 9, INT32)
#define MTAL_IO_VDECEX_VBI_SETVBILINE                        _IOW(MTAL_IOCTYPE_MTVDECEX_VBI, 10, INT32)



#define MTAL_IO_ATSC_OPEN                   _IOW(MTAL_IOCTYPE_MTFE_ATSC, 0, INT32)
#define MTAL_IO_ATSC_CLOSE                   _IOW(MTAL_IOCTYPE_MTFE_ATSC, 1, INT32)
#define MTAL_IO_ATSC_CONNECT                   _IOW(MTAL_IOCTYPE_MTFE_ATSC, 2, INT32)
#define MTAL_IO_ATSC_DISCONNECT                   _IOW(MTAL_IOCTYPE_MTFE_ATSC, 3, INT32)
#define MTAL_IO_ATSC_GETSYNC                _IOW(MTAL_IOCTYPE_MTFE_ATSC, 4, INT32)
#define MTAL_IO_ATSC_GETSIGLEVEL                   _IOW(MTAL_IOCTYPE_MTFE_ATSC, 5, INT32)
#define MTAL_IO_ATSC_GETPER                   _IOW(MTAL_IOCTYPE_MTFE_ATSC, 6, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_ATSC_GETSNR                   _IOW(MTAL_IOCTYPE_MTFE_ATSC, 7, INT32)
#define MTAL_IO_ATSC_GETIFAGC                   _IOW(MTAL_IOCTYPE_MTFE_ATSC, 8, INT32)
#define MTAL_IO_ATSC_GETTCMBER                _IOW(MTAL_IOCTYPE_MTFE_ATSC, 9, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_ATSC_SETSPCPOLAR                   _IOW(MTAL_IOCTYPE_MTFE_ATSC, 10, INT32)
#define MTAL_IO_ATSC_SETTSDIROUT                   _IOW(MTAL_IOCTYPE_MTFE_ATSC, 11, INT32)
#define MTAL_IO_ATSC_GETVER                   _IOW(MTAL_IOCTYPE_MTFE_ATSC, 12, INT32)
#define MTAL_IO_ATSC_REGCB                   _IOW(MTAL_IOCTYPE_MTFE_ATSC, 13, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_ATSC_SETIFFREQ                   _IOW(MTAL_IOCTYPE_MTFE_ATSC, 14, INT32)
#define MTAL_IO_ATSC_TUNER_CONFIG           _IOW(MTAL_IOCTYPE_MTFE_ATSC, 15, INT32)
#define MTAL_IO_ATSC_DIGTUNERBYPASSI2C           _IOW(MTAL_IOCTYPE_MTFE_ATSC, 16, INT32)
#define MTAL_IO_ATSC_GETFREQOFFSET           _IOW(MTAL_IOCTYPE_MTFE_ATSC, 17, INT32)
#define MTAL_IO_ATSC_DTD_Acquire            _IOW(MTAL_IOCTYPE_MTFE_ATSC, 18, INT32)

#define MTAL_IO_ISDBT_OPEN                   _IOW(MTAL_IOCTYPE_MTFE_ISDBT, 0, INT32)
#define MTAL_IO_ISDBT_CLOSE                  _IOW(MTAL_IOCTYPE_MTFE_ISDBT, 1, INT32)
#define MTAL_IO_ISDBT_CONNECT             _IOW(MTAL_IOCTYPE_MTFE_ISDBT, 2, INT32)
#define MTAL_IO_ISDBT_DISCONNECT        _IOW(MTAL_IOCTYPE_MTFE_ISDBT, 3, INT32)
#define MTAL_IO_ISDBT_TSF                      _IOW(MTAL_IOCTYPE_MTFE_ISDBT, 4, INT32)
#define MTAL_IO_ISDBT_GETPER                _IOW(MTAL_IOCTYPE_MTFE_ISDBT, 5, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_ISDBT_GETBER                _IOW(MTAL_IOCTYPE_MTFE_ISDBT, 6, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_ISDBT_GETCNR                _IOW(MTAL_IOCTYPE_MTFE_ISDBT, 7, INT32)
#define MTAL_IO_ISDBT_GETSYNC              _IOW(MTAL_IOCTYPE_MTFE_ISDBT, 8, INT32)
#define MTAL_IO_ISDBT_GETIFAGC            _IOW(MTAL_IOCTYPE_MTFE_ISDBT, 9, INT32)
#define MTAL_IO_ISDBT_GETSIGLEVEL       _IOW(MTAL_IOCTYPE_MTFE_ISDBT, 10, INT32)
#define MTAL_IO_ISDBT_GETVER                _IOW(MTAL_IOCTYPE_MTFE_ISDBT, 11, INT32)
#define MTAL_IO_ISDBT_TSID                     _IOW(MTAL_IOCTYPE_MTFE_ISDBT, 12, INT32)
#define MTAL_IO_ISDBT_LNA                       _IOW(MTAL_IOCTYPE_MTFE_ISDBT, 13, INT32)
#define MTAL_IO_ISDBT_REGCB                   _IOW(MTAL_IOCTYPE_MTFE_ISDBT, 14, MTAL_IOCTL_3ARG_T)

#define MTAL_IO_ISDBS_OPEN                   _IOW(MTAL_IOCTYPE_MTFE_ISDBS, 0, INT32)
#define MTAL_IO_ISDBS_CLOSE                  _IOW(MTAL_IOCTYPE_MTFE_ISDBS, 1, INT32)
#define MTAL_IO_ISDBS_CONNECT             _IOW(MTAL_IOCTYPE_MTFE_ISDBS, 2, INT32)
#define MTAL_IO_ISDBS_DISCONNECT        _IOW(MTAL_IOCTYPE_MTFE_ISDBS, 3, INT32)
#define MTAL_IO_ISDBS_TSF                      _IOW(MTAL_IOCTYPE_MTFE_ISDBS, 4, INT32)
#define MTAL_IO_ISDBS_GETPER                _IOW(MTAL_IOCTYPE_MTFE_ISDBS, 5, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_ISDBS_GETBER                _IOW(MTAL_IOCTYPE_MTFE_ISDBS, 6, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_ISDBS_GETCNR                _IOW(MTAL_IOCTYPE_MTFE_ISDBS, 7, INT32)
#define MTAL_IO_ISDBS_GETSYNC              _IOW(MTAL_IOCTYPE_MTFE_ISDBS, 8, INT32)
#define MTAL_IO_ISDBS_GETIFAGC            _IOW(MTAL_IOCTYPE_MTFE_ISDBS, 9, INT32)
#define MTAL_IO_ISDBS_GETSIGLEVEL       _IOW(MTAL_IOCTYPE_MTFE_ISDBS, 10, INT32)
#define MTAL_IO_ISDBS_GETVER                _IOW(MTAL_IOCTYPE_MTFE_ISDBS, 11, INT32)
#define MTAL_IO_ISDBS_TSID                     _IOW(MTAL_IOCTYPE_MTFE_ISDBS, 12, INT32)
#define MTAL_IO_ISDBS_LNB                       _IOW(MTAL_IOCTYPE_MTFE_ISDBS, 13, INT32)
#define MTAL_IO_ISDBS_REGCB                   _IOW(MTAL_IOCTYPE_MTFE_ISDBS, 14, MTAL_IOCTL_3ARG_T)

#define MTAL_IO_DVBT_OPEN                     _IOW(MTAL_IOCTYPE_MTFE_DVBT, 0, INT32)
#define MTAL_IO_DVBT_CLOSE                    _IOW(MTAL_IOCTYPE_MTFE_DVBT, 1, INT32)
#define MTAL_IO_DVBT_CONNECT                  _IOW(MTAL_IOCTYPE_MTFE_DVBT, 2, INT32)/*fix me*/
#define MTAL_IO_DVBT_DISCONNECT               _IOW(MTAL_IOCTYPE_MTFE_DVBT, 3, INT32)
#define MTAL_IO_DVBT_GETSYNC                  _IOW(MTAL_IOCTYPE_MTFE_DVBT, 4, INT32)
#define MTAL_IO_DVBT_GETSIGLEVEL              _IOW(MTAL_IOCTYPE_MTFE_DVBT, 5, INT32)
#define MTAL_IO_DVBT_GETPER                   _IOW(MTAL_IOCTYPE_MTFE_DVBT, 6, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DVBT_GETSNR                   _IOW(MTAL_IOCTYPE_MTFE_DVBT, 7, INT32)/*fix me*/
#define MTAL_IO_DVBT_GETIFAGC                 _IOW(MTAL_IOCTYPE_MTFE_DVBT, 8, INT32)
#define MTAL_IO_DVBT_GETTCMBER                _IOW(MTAL_IOCTYPE_MTFE_DVBT, 9, INT32)
#define MTAL_IO_DVBT_GETVER                   _IOW(MTAL_IOCTYPE_MTFE_DVBT, 10, INT32)/*fix me*/
#define MTAL_IO_DVBT_REGCB                    _IOW(MTAL_IOCTYPE_MTFE_DVBT, 11, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DVBT_GETTPS                   _IOW(MTAL_IOCTYPE_MTFE_DVBT, 12, INT32)
#define MTAL_IO_DVBT_GETFREQOFFSET            _IOW(MTAL_IOCTYPE_MTFE_DVBT, 13, INT32)
#define MTAL_IO_DVBT_TUNER_CONFIG            _IOW(MTAL_IOCTYPE_MTFE_DVBT, 14, INT32)
#define MTAL_IO_DVBT_SETTSPATH         		_IOW(MTAL_IOCTYPE_MTFE_DVBT, 15, INT32)
#define MTAL_IO_DVBT_SETTSINTERFACE          _IOW(MTAL_IOCTYPE_MTFE_DVBT, 16, INT32)
#define MTAL_IO_DVBT_GETSQI                   _IOW(MTAL_IOCTYPE_MTFE_DVBT, 17, INT32)
#define MTAL_IO_DVBT_SETTSPAD                   _IOW(MTAL_IOCTYPE_MTFE_DVBT, 18, INT32)
#define MTAL_IO_DVBT_SETTSCTRL                   _IOW(MTAL_IOCTYPE_MTFE_DVBT, 19, INT32)


#define MTAL_IO_DVBC_OPEN                     _IOW(MTAL_IOCTYPE_MTFE_DVBC, 0, INT32)
#define MTAL_IO_DVBC_CLOSE                    _IOW(MTAL_IOCTYPE_MTFE_DVBC, 1, INT32)
#define MTAL_IO_DVBC_CONNECT                  _IOW(MTAL_IOCTYPE_MTFE_DVBC, 2, INT32)/*fix me*/
#define MTAL_IO_DVBC_DISCONNECT               _IOW(MTAL_IOCTYPE_MTFE_DVBC, 3, INT32)
#define MTAL_IO_DVBC_GETSYNC                  _IOW(MTAL_IOCTYPE_MTFE_DVBC, 4, INT32)
#define MTAL_IO_DVBC_GETSIGLEVEL              _IOW(MTAL_IOCTYPE_MTFE_DVBC, 5, INT32)
#define MTAL_IO_DVBC_GETPER                   _IOW(MTAL_IOCTYPE_MTFE_DVBC, 6, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DVBC_GETSNR                   _IOW(MTAL_IOCTYPE_MTFE_DVBC, 7, INT32)/*fix me*/
#define MTAL_IO_DVBC_GETIFAGC                 _IOW(MTAL_IOCTYPE_MTFE_DVBC, 8, INT32)
#define MTAL_IO_DVBC_GETTCMBER                _IOW(MTAL_IOCTYPE_MTFE_DVBC, 9, INT32)
#define MTAL_IO_DVBC_GETVER                   _IOW(MTAL_IOCTYPE_MTFE_DVBC, 10, INT32)/*fix me*/
#define MTAL_IO_DVBC_REGCB                    _IOW(MTAL_IOCTYPE_MTFE_DVBC, 11, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DVBC_GETFREQOFFSET            _IOW(MTAL_IOCTYPE_MTFE_DVBC, 12, INT32)
#define MTAL_IO_DVBC_TUNER_CONFIG            _IOW(MTAL_IOCTYPE_MTFE_DVBC, 13, INT32)
#define MTAL_IO_DVBC_SETTSPATH         		_IOW(MTAL_IOCTYPE_MTFE_DVBC, 14, INT32)
#define MTAL_IO_DVBC_SETTSINTERFACE          _IOW(MTAL_IOCTYPE_MTFE_DVBC, 15, INT32)
#define MTAL_IO_DVBC_GetTPSINFO                  _IOW(MTAL_IOCTYPE_MTFE_DVBC, 16, INT32)
#define MTAL_IO_DVBC_GETSQI                   _IOW(MTAL_IOCTYPE_MTFE_DVBC, 17, INT32)




#define MTAL_IO_DTMB_OPEN                     _IOW(MTAL_IOCTYPE_MTFE_DTMB, 0, INT32)
#define MTAL_IO_DTMB_CLOSE                    _IOW(MTAL_IOCTYPE_MTFE_DTMB, 1, INT32)
#define MTAL_IO_DTMB_CONNECT                  _IOW(MTAL_IOCTYPE_MTFE_DTMB, 2, INT32)/*fix me*/
#define MTAL_IO_DTMB_DISCONNECT               _IOW(MTAL_IOCTYPE_MTFE_DTMB, 3, INT32)
#define MTAL_IO_DTMB_GETSYNC                  _IOW(MTAL_IOCTYPE_MTFE_DTMB, 4, INT32)
#define MTAL_IO_DTMB_GETSIGLEVEL              _IOW(MTAL_IOCTYPE_MTFE_DTMB, 5, INT32)
#define MTAL_IO_DTMB_GETPER                   _IOW(MTAL_IOCTYPE_MTFE_DTMB, 6, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_DTMB_GETSNR                   _IOW(MTAL_IOCTYPE_MTFE_DTMB, 7, INT32)/*fix me*/
#define MTAL_IO_DTMB_GETIFAGC                 _IOW(MTAL_IOCTYPE_MTFE_DTMB, 8, INT32)
#define MTAL_IO_DTMB_GETTCMBER                _IOW(MTAL_IOCTYPE_MTFE_DTMB, 9, INT32)
#define MTAL_IO_DTMB_GETVER                   _IOW(MTAL_IOCTYPE_MTFE_DTMB, 10, INT32)/*fix me*/
#define MTAL_IO_DTMB_REGCB                    _IOW(MTAL_IOCTYPE_MTFE_DTMB, 11, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_DTMB_GETTPS                   _IOW(MTAL_IOCTYPE_MTFE_DTMB, 12, INT32)
#define MTAL_IO_DTMB_GETFREQOFFSET            _IOW(MTAL_IOCTYPE_MTFE_DTMB, 13, INT32)
#define MTAL_IO_DTMB_TUNER_CONFIG            _IOW(MTAL_IOCTYPE_MTFE_DTMB, 14, INT32)


#define MTAL_IO_NTSC_OPEN                    _IOW(MTAL_IOCTYPE_MTFE_NTSC, 0, INT32)
#define MTAL_IO_NTSC_CLOSE                   _IOW(MTAL_IOCTYPE_MTFE_NTSC, 1, INT32)
#define MTAL_IO_NTSC_CONNECT                 _IOW(MTAL_IOCTYPE_MTFE_NTSC, 2, INT32)
#define MTAL_IO_NTSC_DISCONNECT              _IOW(MTAL_IOCTYPE_MTFE_NTSC, 3, INT32)
#define MTAL_IO_NTSC_TSF                     _IOW(MTAL_IOCTYPE_MTFE_NTSC, 4, INT32)
#define MTAL_IO_NTSC_SSNF                    _IOW(MTAL_IOCTYPE_MTFE_NTSC, 5, INT32)
#define MTAL_IO_NTSC_BSNF                    _IOW(MTAL_IOCTYPE_MTFE_NTSC, 6, INT32)
#define MTAL_IO_NTSC_REGCB                   _IOW(MTAL_IOCTYPE_MTFE_NTSC, 7, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_NTSC_GETVER                  _IOW(MTAL_IOCTYPE_MTFE_NTSC, 8, INT32)
#define MTAL_IO_NTSC_GETSYNC                 _IOW(MTAL_IOCTYPE_MTFE_NTSC, 9, INT32)

#define MTAL_IO_PAL_OPEN                     _IOW(MTAL_IOCTYPE_MTFE_PAL, 0, INT32)
#define MTAL_IO_PAL_CLOSE                    _IOW(MTAL_IOCTYPE_MTFE_PAL, 1, INT32)
#define MTAL_IO_PAL_CONNECT                  _IOW(MTAL_IOCTYPE_MTFE_PAL, 2, INT32)
#define MTAL_IO_PAL_DISCONNECT               _IOW(MTAL_IOCTYPE_MTFE_PAL, 3, INT32)
#define MTAL_IO_PAL_TSF                      _IOW(MTAL_IOCTYPE_MTFE_PAL, 4, INT32)
#define MTAL_IO_PAL_SSNF                     _IOW(MTAL_IOCTYPE_MTFE_PAL, 5, INT32)
#define MTAL_IO_PAL_BSNF                     _IOW(MTAL_IOCTYPE_MTFE_PAL, 6, INT32)
#define MTAL_IO_PAL_REGCB                    _IOW(MTAL_IOCTYPE_MTFE_PAL, 7, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_PAL_GETFREQBOUND             _IOW(MTAL_IOCTYPE_MTFE_PAL, 8, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_PAL_BypassSIF                _IOW(MTAL_IOCTYPE_MTFE_PAL,9, INT32)
#define MTAL_IO_PAL_GETLOCKSTATUS            _IOW(MTAL_IOCTYPE_MTFE_PAL, 10, INT32)
#define MTAL_IO_PAL_AUTOSEARCH               _IOW(MTAL_IOCTYPE_MTFE_PAL, 11, INT32)

#define MTAL_IO_PAL_GET_SYSTEM               _IOW(MTAL_IOCTYPE_MTFE_PAL, 12, INT32)
#define MTAL_IO_PAL_SET_SYSTEM               _IOW(MTAL_IOCTYPE_MTFE_PAL, 13, INT32)
#define MTAL_IO_PAL_GETSIGLEVEL              _IOW(MTAL_IOCTYPE_MTFE_PAL, 14, INT32)
#define MTAL_IO_PAL_SET_AFT                  _IOW(MTAL_IOCTYPE_MTFE_PAL, 15, INT32)  /*wenming 110924*/

#define MTAL_IO_SBTVD_OPEN                  _IOW(MTAL_IOCTYPE_MTFE_SBTVD, 0, INT32)
#define MTAL_IO_SBTVD_CLOSE                 _IOW(MTAL_IOCTYPE_MTFE_SBTVD, 1, INT32)
#define MTAL_IO_SBTVD_CONNECT               _IOW(MTAL_IOCTYPE_MTFE_SBTVD, 2, INT32)
#define MTAL_IO_SBTVD_DISCONNECT            _IOW(MTAL_IOCTYPE_MTFE_SBTVD, 3, INT32)
#define MTAL_IO_SBTVD_GETSYNC               _IOW(MTAL_IOCTYPE_MTFE_SBTVD, 4, INT32)
#define MTAL_IO_SBTVD_GETSIGLEVEL           _IOW(MTAL_IOCTYPE_MTFE_SBTVD, 5, INT32)
#define MTAL_IO_SBTVD_GETPER                _IOW(MTAL_IOCTYPE_MTFE_SBTVD, 6, UINT32)
#define MTAL_IO_SBTVD_GETBER                _IOW(MTAL_IOCTYPE_MTFE_SBTVD, 7, UINT32)

#define MTAL_IO_VDO_INIT                        _IOW(MTAL_IOCTYPE_MTVDO, 0, INT32)
#define MTAL_IO_VDO_STOP                        _IOW(MTAL_IOCTYPE_MTVDO, 1, INT32)
#define MTAL_IO_VDO_RESET                       _IOW(MTAL_IOCTYPE_MTVDO, 2, INT32)
#define MTAL_IO_VDO_SET_ENABLE                  _IOW(MTAL_IOCTYPE_MTVDO, 3, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_GET_ENABLE                  _IOW(MTAL_IOCTYPE_MTVDO, 4, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_MODE                    _IOW(MTAL_IOCTYPE_MTVDO, 5, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_GET_MODE                    _IOW(MTAL_IOCTYPE_MTVDO, 6, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_FREEZE                  _IOW(MTAL_IOCTYPE_MTVDO, 7, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_GET_FREEZE                  _IOW(MTAL_IOCTYPE_MTVDO, 8, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_ARG                     _IOW(MTAL_IOCTYPE_MTVDO, 9, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDO_GET_ARG                     _IOW(MTAL_IOCTYPE_MTVDO, 10, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDO_SET_OVERSCAN                _IOW(MTAL_IOCTYPE_MTVDO, 11, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_GET_OVERSCAN                _IOW(MTAL_IOCTYPE_MTVDO, 12, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_SRCREGION               _IOW(MTAL_IOCTYPE_MTVDO, 13, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_GET_SRCREGION               _IOW(MTAL_IOCTYPE_MTVDO, 14, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_OUTREGION               _IOW(MTAL_IOCTYPE_MTVDO, 15, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_GET_OUTREGION               _IOW(MTAL_IOCTYPE_MTVDO, 16, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_BG                      _IOW(MTAL_IOCTYPE_MTVDO, 17, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_GET_BG                      _IOW(MTAL_IOCTYPE_MTVDO, 18, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_REGCB                       _IOW(MTAL_IOCTYPE_MTVDO, 19, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_VDO_SET_GAMEMODE                _IOW(MTAL_IOCTYPE_MTVDO, 20, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_3DGAMEMODE              _IOW(MTAL_IOCTYPE_MTVDO, 21, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_GET_UNMUTE                  _IOW(MTAL_IOCTYPE_MTVDO, 22, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_GET_FRAMEBUFFER_AVG         _IOW(MTAL_IOCTYPE_MTVDO, 23, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_YUV2RGB_MATRIX          _IOW(MTAL_IOCTYPE_MTVDO, 28, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_MJC_ONFF                _IOW(MTAL_IOCTYPE_MTVDO, 29, INT32)
#define MTAL_IO_VDO_SET_MJC_DEMO_MODE           _IOW(MTAL_IOCTYPE_MTVDO, 30, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDO_SET_MJC_FLIP_MIRROR         _IOW(MTAL_IOCTYPE_MTVDO, 31, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_MJC_FILM_MODE           _IOW(MTAL_IOCTYPE_MTVDO, 32, INT32)
#define MTAL_IO_VDO_SET_MJC_EFCT_LVL            _IOW(MTAL_IOCTYPE_MTVDO, 33, INT32)
#define MTAL_IO_VDO_SET_DOTBYDOT                _IOW(MTAL_IOCTYPE_MTVDO, 34, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_NONLINEAR               _IOW(MTAL_IOCTYPE_MTVDO, 35, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDO_GET_TIMINGINFO		          _IOW(MTAL_IOCTYPE_MTVDO, 36, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_MJC_RESETSW             _IOW(MTAL_IOCTYPE_MTVDO, 37, INT32)
#define MTAL_IO_VDO_SET_MJC_VIDEO_BLOCK         _IOW(MTAL_IOCTYPE_MTVDO, 38, INT32)
#define MTAL_IO_VDO_SET_MJC_JUDDER_LVL          _IOW(MTAL_IOCTYPE_MTVDO, 39, INT32)
#define MTAL_IO_VDO_SET_MJC_BLUR_LVL            _IOW(MTAL_IOCTYPE_MTVDO, 40, INT32)
#define MTAL_IO_VDO_GET_MJC_FW_VER              _IOW(MTAL_IOCTYPE_MTVDO, 41, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_FACTORY_MODE            _IOW(MTAL_IOCTYPE_MTVDO, 45, INT32)
#define MTAL_IO_VDO_GET_CHROMA_HIST             _IOW(MTAL_IOCTYPE_MTVDO, 46, INT32) 
#define MTAL_IO_VDO_GET_LUMA_HIST           _IOW(MTAL_IOCTYPE_MTVDO, 47, INT32) 
#define MTAL_IO_VDO_SET_MJC_QTY_CURVE       _IOW(MTAL_IOCTYPE_MTVDO, 48, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_GET_MJC_QTY_CURVE       _IOW(MTAL_IOCTYPE_MTVDO, 49, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDO_GET_APL                 _IOW(MTAL_IOCTYPE_MTVDO, 50, INT32)
#define MTAL_IO_VDO_SET_SCALING_FILTER      _IOW(MTAL_IOCTYPE_MTVDO, 51, INT32)
#define MTAL_IO_VDO_GET_SCALING_FILTER      _IOW(MTAL_IOCTYPE_MTVDO, 52, INT32)
#define MTAL_IO_VDO_SET_HDMITRUEBLACK       _IOW(MTAL_IOCTYPE_MTVDO, 53, INT32)
#define MTAL_IO_VDO_SET_MJC_INITIALIZE          _IOW(MTAL_IOCTYPE_MTVDO, 54, INT32)
#define MTAL_IO_VDO_SET_MJC_BYPASS_WINDOW       _IOW(MTAL_IOCTYPE_MTVDO, 55, MTAL_IOCTL_6ARG_T)
#define MTAL_IO_VDO_SET_MJC_DEMO_BAR            _IOW(MTAL_IOCTYPE_MTVDO, 56, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_VDO_SET_MJC_MC_ONOFF            _IOW(MTAL_IOCTYPE_MTVDO, 57, INT32)
#define MTAL_IO_VDO_SET_MJC_MC                  _IOW(MTAL_IOCTYPE_MTVDO, 58, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDO_GET_FRAMEBUFFER_ONOFF	    _IOW(MTAL_IOCTYPE_MTVDO, 59, INT32)
#define MTAL_IO_VDO_GAIN_SEL                  _IOW(MTAL_IOCTYPE_MTVDO, 60, INT32)
#define MTAL_IO_VDO_GET_PE_UI_MIN_MAX_DFT     _IOW(MTAL_IOCTYPE_MTVDO, 61, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDO_SET_INPUT_MATRIX          _IOW(MTAL_IOCTYPE_MTVDO, 62, INT32)
#define MTAL_IO_VDO_SET_GAMMA_TABLE           _IOW(MTAL_IOCTYPE_MTVDO, 63, INT32)
#define MTAL_IO_VDO_SET_AUTOMUTE_ONOFF        _IOW(MTAL_IOCTYPE_MTVDO, 64, INT32)
#define MTAL_IO_VDO_SET_SCALER_PIXELBASED	    _IOW(MTAL_IOCTYPE_MTVDO, 65, INT32)
#define MTAL_IO_VDO_SET_XVYCC_INNER_PTGEN     _IOW(MTAL_IOCTYPE_MTVDO, 66, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_VDO_SET_PE_UI_MIN_MAX_DFT     _IOW(MTAL_IOCTYPE_MTVDO, 67, MTAL_IOCTL_3ARG_T)

#define MTAL_IO_VDO_SET_SCANMODE                _IOW(MTAL_IOCTYPE_MTVDO, 69, INT32)
#define MTAL_IO_VDO_SET_DS_FACTOR               _IOW(MTAL_IOCTYPE_MTVDO, 70, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_PQ                      _IOW(MTAL_IOCTYPE_MTVDO, 71, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_VDO_GET_PQ                      _IOW(MTAL_IOCTYPE_MTVDO, 72, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_VDO_GET_3D_TAG                  _IOW(MTAL_IOCTYPE_MTVDO, 73, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_3D_PANEL                _IOW(MTAL_IOCTYPE_MTVDO, 74, INT32)
#define MTAL_IO_VDO_SET_3DFP_ONFF               _IOW(MTAL_IOCTYPE_MTVDO, 75, INT32)
#define MTAL_IO_VDO_SET_SRAMDRAMMODE_ONOFF      _IOW(MTAL_IOCTYPE_MTVDO, 76, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_FLIPMIRROR              _IOW(MTAL_IOCTYPE_MTVDO, 77, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_OUTPUTWINDOW			_IOW(MTAL_IOCTYPE_MTVDO, 78, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SRM_SETTVMODE				  _IOW(MTAL_IOCTYPE_MTVDO, 79, INT32)
#define MTAL_IO_VDO_GetPlaneWH			        _IOW(MTAL_IOCTYPE_MTVDO, 80, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SETALLMUTE			        _IOW(MTAL_IOCTYPE_MTVDO, 81, INT32)
#define MTAL_IO_VDO_SET_LCDIM_INITIALIZE    	_IOW(MTAL_IOCTYPE_MTVDO, 82, INT32)
#define MTAL_IO_VDO_SET_LCDIM_ONOFF				_IOW(MTAL_IOCTYPE_MTVDO, 83, INT32)
#define MTAL_IO_VDO_SET_LCDIM_LUTIDX			_IOW(MTAL_IOCTYPE_MTVDO, 84, INT32)
#define MTAL_IO_VDO_SET_LCDS_ONOFF				_IOW(MTAL_IOCTYPE_MTVDO, 85, INT32)
#define MTAL_IO_VDO_SET_LCDIM_DEMO				_IOW(MTAL_IOCTYPE_MTVDO, 86, INT32)
#define MTAL_IO_VDO_EnableOD				      _IOW(MTAL_IOCTYPE_MTVDO, 87, INT32)
//#define MTAL_IO_VDO_SetODTable				  _IOW(MTAL_IOCTYPE_MTVDO, 88, INT32)
#define MTAL_IO_VDO_GETFRAMERATE				  _IOW(MTAL_IOCTYPE_MTVDO, 89, INT32)
#define MTAL_IO_VDO_GETINPUTFRAMERATE           _IOW(MTAL_IOCTYPE_MTVDO, 90, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_DI_FREEZE_ONOFF         _IOW(MTAL_IOCTYPE_MTVDO, 91, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_DI_GET_PIXEL                _IOW(MTAL_IOCTYPE_MTVDO, 92, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_VDO_DI_GET_DRAM_PIXELS          _IOW(MTAL_IOCTYPE_MTVDO, 93, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDO_DI_WRITE_DRAM_PIXELS        _IOW(MTAL_IOCTYPE_MTVDO, 94, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_VDO_DI_GET_PSCAN_MODE                _IOW(MTAL_IOCTYPE_MTVDO, 95, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_DI_SET_PSCAN_MODE                _IOW(MTAL_IOCTYPE_MTVDO, 96, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_DI_GET_FORMAT_INFO               _IOW(MTAL_IOCTYPE_MTVDO, 97, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_DI_DUMP_DRAM_INIT_RELEASE    _IOW(MTAL_IOCTYPE_MTVDO, 98, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_FBM_DI_TEST_MODE                 _IOW(MTAL_IOCTYPE_MTVDO, 99, INT32)
#define MTAL_IO_VDO_GET_3D_NAVI_DETECT	        _IOW(MTAL_IOCTYPE_MTVDO, 100, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_BYPASS_PREPROC              _IOW(MTAL_IOCTYPE_MTVDO, 101, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_3D_NAVI_MODE	          _IOW(MTAL_IOCTYPE_MTVDO, 102, INT32)
#define MTAL_IO_VDO_SET_3D_DEPTH_CTRL_ON_OFF   	_IOW(MTAL_IOCTYPE_MTVDO, 103, INT32)
#define MTAL_IO_VDO_SET_FBM_MODE                _IOW(MTAL_IOCTYPE_MTVDO, 104, INT32)
#define MTAL_IO_VDO_GET_NET_SWAP_BUFFER         _IOW(MTAL_IOCTYPE_MTVDO, 105, INT32)
#define MTAL_IO_VDO_GET_PTF_INFO                _IOW(MTAL_IOCTYPE_MTVDO, 106, MTAL_IOCTL_2ARG_T)

#define MTAL_IO_VDO_SET_BACKLIGHT_MODE          _IOW(MTAL_IOCTYPE_MTVDO, 108, MTAL_IOCTL_2ARG_T) 
#define MTAL_IO_VDO_GET_PCF_PQF_INFO            _IOW(MTAL_IOCTYPE_MTVDO, 109, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_VIDEO_MUTE              _IOW(MTAL_IOCTYPE_MTVDO, 110, INT32) //Caihua 20120601

#define MTAL_IO_VDO_SET_PWM                 _IOW(MTAL_IOCTYPE_MTVDO, 111, INT32)
#define MTAL_IO_VDO_SET_3DMODE              _IOW(MTAL_IOCTYPE_MTVDO, 112, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_2DSETLR                 _IOW(MTAL_IOCTYPE_MTVDO, 113, INT32)
#define MTAL_IO_VDO_3DINVERSE                 _IOW(MTAL_IOCTYPE_MTVDO, 114, INT32)
#define MTAL_IO_VDO_SET3DDEPTH                 _IOW(MTAL_IOCTYPE_MTVDO, 115, INT32)
#define MTAL_IO_VDO_GET_ISHDMIFP                    _IOW(MTAL_IOCTYPE_MTVDO, 116, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VDO_SET_COLORMODE				  _IOW(MTAL_IOCTYPE_MTVDO, 117, INT32)

/*********MTAL_IOCTYPE_MTVFE   17*******************/
#define MTAL_IO_VFE_ADCSETTINGRESET	_IOW(MTAL_IOCTYPE_MTVFE, 0, INT32)
#define MTAL_IO_VFE_ADCCALIBRATION	_IOW(MTAL_IOCTYPE_MTVFE, 1, INT32)
#define MTAL_IO_VFE_GETHDADCOFFSET	_IOW(MTAL_IOCTYPE_MTVFE, 2, INT32)
#define MTAL_IO_VFE_GETHDADCGAIN	_IOW(MTAL_IOCTYPE_MTVFE, 3, INT32)
#define MTAL_IO_VFE_SETHDADCOFFSET	_IOW(MTAL_IOCTYPE_MTVFE, 4, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VFE_SETHDADCGAIN	_IOW(MTAL_IOCTYPE_MTVFE, 5, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VFE_GETADCMAXVALUE	_IOW(MTAL_IOCTYPE_MTVFE, 6, INT32)
#define MTAL_IO_VFE_GETBLANKVAL		_IOW(MTAL_IOCTYPE_MTVFE, 7, INT32)
#define MTAL_IO_VFE_SETSYNCSLICELEVEL  _IOW(MTAL_IOCTYPE_MTVFE, 8, INT32)
#define MTAL_IO_VFE_SET_PGAGAIN_OFFSET_INIT_VALUE  _IOW(MTAL_IOCTYPE_MTVFE, 9, INT32)
#define MTAL_IO_VFE_GET_GAIN_STORAGE		_IOW(MTAL_IOCTYPE_MTVFE, 10, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VFE_EBGAIN_GAIN_OVERRULE _IOW(MTAL_IOCTYPE_MTVFE, 11, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VFE_Embedded_GAIN _IOW(MTAL_IOCTYPE_MTVFE, 12, MTAL_IOCTL_2ARG_T)



/*MTAL_IOCTYPE_MTCI 18*/
#define MTAL_IO_CI_INIT                         _IOW(MTAL_IOCTYPE_MTCI, 0, INT32)
#define MTAL_IO_CI_DECTECTCARD                  _IOW(MTAL_IOCTYPE_MTCI, 1, INT32)
#define MTAL_IO_CI_RESET                        _IOW(MTAL_IOCTYPE_MTCI, 2, INT32)
#define MTAL_IO_CI_CHECKCIS                     _IOW(MTAL_IOCTYPE_MTCI, 3, INT32)
#define MTAL_IO_CI_WRITECOR                     _IOW(MTAL_IOCTYPE_MTCI, 4, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_CI_BYPASSONOFF                  _IOW(MTAL_IOCTYPE_MTCI, 5, INT32)
#define MTAL_IO_CI_READ                         _IOW(MTAL_IOCTYPE_MTCI, 6, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_CI_WRITE                        _IOW(MTAL_IOCTYPE_MTCI, 7, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_CI_NEGOBUF                      _IOW(MTAL_IOCTYPE_MTCI, 8, INT32)
#define MTAL_IO_CI_READDASTATUS                 _IOW(MTAL_IOCTYPE_MTCI, 9, INT32)
#define MTAL_IO_CI_READREG                      _IOW(MTAL_IOCTYPE_MTCI, 10, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_CI_WRITEREG                     _IOW(MTAL_IOCTYPE_MTCI, 11, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_CI_READCOR                      _IOW(MTAL_IOCTYPE_MTCI, 12, INT32)
#define MTAL_IO_CI_TESTCARDREADY                _IOW(MTAL_IOCTYPE_MTCI, 13, INT32)
#define MTAL_IO_CI_ENABLESLOT                   _IOW(MTAL_IOCTYPE_MTCI, 14, INT32)
#define MTAL_IO_CI_DETECTPODACTIVE              _IOW(MTAL_IOCTYPE_MTCI, 15, INT32)
#define MTAL_IO_CI_PCMCIAENABLETS               _IOW(MTAL_IOCTYPE_MTCI, 16, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_CI_RESETCAM                     _IOW(MTAL_IOCTYPE_MTCI, 17, INT32)
#define MTAL_IO_CI_SETCARDDETECTONOFF           _IOW(MTAL_IOCTYPE_MTCI, 18, INT32)
#define MTAL_IO_CI_SETBYPASS                    _IOW(MTAL_IOCTYPE_MTCI, 19, INT32)
#define MTAL_IO_CI_EXTERNALDEMOD                _IOW(MTAL_IOCTYPE_MTCI, 20, INT32)
#define MTAL_IO_CI_SETTSPATH                    _IOW(MTAL_IOCTYPE_MTCI, 21, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_CI_CHECKCIPLUSCAP               _IOW(MTAL_IOCTYPE_MTCI, 22, INT32)
#define MTAL_IO_CI_SETTSCLOCK                   _IOW(MTAL_IOCTYPE_MTCI, 23, INT32)
#define MTAL_IO_CI_UPDATETSSTATUS               _IOW(MTAL_IOCTYPE_MTCI, 24, INT32)
#define MTAL_IO_CI_GETCIS                       _IOW(MTAL_IOCTYPE_MTCI, 25, INT32)



/*MTAL_IOCTYPE_MTTVE 20*/
#define MTAL_IO_TVE_INIT                              _IOW(MTAL_IOCTYPE_MTTVE, 0, INT32)
#define MTAL_IO_TVE_CONNECT                      _IOW(MTAL_IOCTYPE_MTTVE, 1, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_TVE_DISCONNECT                _IOW(MTAL_IOCTYPE_MTTVE, 2, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_TVE_VIDEOSTART                 _IOW(MTAL_IOCTYPE_MTTVE, 3, INT32)
#define MTAL_IO_TVE_VIDEOSTOP                   _IOW(MTAL_IOCTYPE_MTTVE, 4, INT32)
#define MTAL_IO_TVE_VIDEOMUTE                   _IOW(MTAL_IOCTYPE_MTTVE, 5, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_TVE_VIDEOFMT                    _IOW(MTAL_IOCTYPE_MTTVE, 6, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_TVE_SENDVBIDATA              _IOW(MTAL_IOCTYPE_MTTVE, 7, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_TVE_VBISTART                     _IOW(MTAL_IOCTYPE_MTTVE, 8, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_TVE_VBISTOP                       _IOW(MTAL_IOCTYPE_MTTVE, 9, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_TVE_SETHSCALING                   _IOW(MTAL_IOCTYPE_MTTVE, 10, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_TVE_SETHCENTERCUT                 _IOW(MTAL_IOCTYPE_MTTVE, 11, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_TVE_SETLETTERBOX                  _IOW(MTAL_IOCTYPE_MTTVE, 12, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_TVE_GETSCALINGSTATUS              _IOW(MTAL_IOCTYPE_MTTVE, 13, INT32)
#define MTAL_IO_TVE_VIDEOMODE 					  _IOW(MTAL_IOCTYPE_MTTVE, 14, MTAL_IOCTL_2ARG_T)

#define MTAL_IO_PVR_INIT        _IOW(MTAL_IOCTYPE_MTPVR, 0, INT32)
#define MTAL_IO_PVR_START       _IOW(MTAL_IOCTYPE_MTPVR, 1, INT32)
#define MTAL_IO_PVR_STOP        _IOW(MTAL_IOCTYPE_MTPVR, 2, INT32)
#define MTAL_IO_PVR_GETBUFFER   _IOW(MTAL_IOCTYPE_MTPVR, 3, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_PVR_PUTBUFFER   _IOW(MTAL_IOCTYPE_MTPVR, 4, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_PVR_GETTHUMBNAIL    _IOW(MTAL_IOCTYPE_MTPVR, 5, MTAL_IOCTL_6ARG_T)
#define MTAL_IO_PVR_STOPTHUMBNAIL   _IOW(MTAL_IOCTYPE_MTPVR, 6, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_PVR_SETSPEED   _IOW(MTAL_IOCTYPE_MTPVR, 7, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_PVR_RESETBUFFER                   _IOW(MTAL_IOCTYPE_MTPVR, 8, INT32)

/* MTAL_IOCTYPE_MTIOMMU */
#define MTAL_IO_MMU_GETPGD                      _IOW(MTAL_IOCTYPE_MTIOMMU, 0, INT32)
#define MTAL_IO_MMU_PINMEM                      _IOW(MTAL_IOCTYPE_MTIOMMU, 1, INT32)
#define MTAL_IO_MMU_UNPINMEM                    _IOW(MTAL_IOCTYPE_MTIOMMU, 2, INT32)
#define MTAL_IO_MMU_MAPSINGLE                   _IOW(MTAL_IOCTYPE_MTIOMMU, 3, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_MMU_UNMAPSINGLE                 _IOW(MTAL_IOCTYPE_MTIOMMU, 4, MTAL_IOCTL_2ARG_T)

/* MTAL_IOCTYPE_MTSTC */
#define MTAL_IO_STC_STOP_STC                    _IOW(MTAL_IOCTYPE_MTSTC, 0, INT32)
#define MTAL_IO_STC_SET_SYNC_MODE               _IOW(MTAL_IOCTYPE_MTSTC, 1, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_STC_SET_STC_VALUE               _IOW(MTAL_IOCTYPE_MTSTC, 2, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_STC_START_STC                   _IOW(MTAL_IOCTYPE_MTSTC, 3, INT32)
#define MTAL_IO_STC_GET_STC_VALUE               _IOW(MTAL_IOCTYPE_MTSTC, 4, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_STC_GET_SRC                     _IOW(MTAL_IOCTYPE_MTSTC, 5, MTAL_IOCTL_2ARG_T)

/*MTAL_IOCTYPE_MTB2R*/
#define MTAL_IO_B2R_GET_PTS                    _IOW(MTAL_IOCTYPE_MTB2R, 0, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_GET_STEP_FORWARD           _IOW(MTAL_IOCTYPE_MTB2R, 1, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_GET_POS_INFO               _IOW(MTAL_IOCTYPE_MTB2R, 2, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_GET_PIC_INFO               _IOW(MTAL_IOCTYPE_MTB2R, 3, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_GET_PLAY_MODE              _IOW(MTAL_IOCTYPE_MTB2R, 4, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_GET_SLOW_FORWARD           _IOW(MTAL_IOCTYPE_MTB2R, 5, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_SET_AB_INFO                _IOW(MTAL_IOCTYPE_MTB2R, 6, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_SET_THUMBNAIL_MODE         _IOW(MTAL_IOCTYPE_MTB2R, 7, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_SET_SEEK                   _IOW(MTAL_IOCTYPE_MTB2R, 8, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_B2R_SET_PAUSE_MM               _IOW(MTAL_IOCTYPE_MTB2R, 9, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_SET_STEP_FORWARD           _IOW(MTAL_IOCTYPE_MTB2R, 10, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_SET_FB_NO_TIMEOUT          _IOW(MTAL_IOCTYPE_MTB2R, 11, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_SET_SPEED                  _IOW(MTAL_IOCTYPE_MTB2R, 12, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_SET_TRICK_MODE             _IOW(MTAL_IOCTYPE_MTB2R, 13, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_SET_INPUT                  _IOW(MTAL_IOCTYPE_MTB2R, 14, MTAL_IOCTL_3ARG_T)
#define MTAL_IO_B2R_SET_SLOW_FORWARD           _IOW(MTAL_IOCTYPE_MTB2R, 15, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_GET_CAPTURE_INFO           _IOW(MTAL_IOCTYPE_MTB2R, 16, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_SET_CB_ATTACH_ID           _IOW(MTAL_IOCTYPE_MTB2R, 17, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_SELECT_GROUP_TYPE          _IOW(MTAL_IOCTYPE_MTB2R, 18, MTAL_IOCTL_5ARG_T)
#define MTAL_IO_B2R_GET_EMPTY_FRAME_BUFFER     _IOW(MTAL_IOCTYPE_MTB2R, 19, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_SET_SEQ_HDR                _IOW(MTAL_IOCTYPE_MTB2R, 20, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_SET_DISP_FRAME             _IOW(MTAL_IOCTYPE_MTB2R, 21, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_RELEASE_FBG                _IOW(MTAL_IOCTYPE_MTB2R, 22, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_FREE_FRAME_BUFFER          _IOW(MTAL_IOCTYPE_MTB2R, 23, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_B2R_GET_DISP_STABLE            _IOW(MTAL_IOCTYPE_MTB2R, 24, MTAL_IOCTL_2ARG_T)

/*MTAL_IOCTYPE_MTVSS 31*/
#define MTAL_IO_VSS_INIT                       _IOW(MTAL_IOCTYPE_MTVSS, 0, INT32)
#define MTAL_IO_VSS_CONNECT                    _IOW(MTAL_IOCTYPE_MTVSS, 1, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VSS_DISCONNECT                 _IOW(MTAL_IOCTYPE_MTVSS, 2, MTAL_IOCTL_2ARG_T)
#define MTAL_IO_VSS_GET_VIDEO_INFO             _IOW(MTAL_IOCTYPE_MTVSS, 3, MTAL_IOCTL_4ARG_T)
#define MTAL_IO_VSS_GET_PLATFORM_INFO          _IOW(MTAL_IOCTYPE_MTVSS, 4, INT32)
#define MTAL_IO_VSS_GET_FRAME_BUFFER           _IOW(MTAL_IOCTYPE_MTVSS, 5, MTAL_IOCTL_5ARG_T)
#define MTAL_IO_VSS_CALLBACK                   _IOW(MTAL_IOCTYPE_MTVSS, 6, INT32)
#define MTAL_IO_VSS_COLOR_FORMAT               _IOW(MTAL_IOCTYPE_MTVSS, 7, INT32)

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------
#ifdef __KERNEL__
extern int mtal_ioctl_mtadac(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtauddec(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtdmx(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtfe_atsc(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtfe_dvbt(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtfe_dvbc(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtfe_dtmb(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtfe_ntsc(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtfe_pal(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtfe_sbtvd(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtgfx(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtmplayer(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtimage(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtosd(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtperipheral(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtpmx(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtscart(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtvdo(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtvfe(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtci(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtvdec(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtvdecex(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtvbiex(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg);
extern int mtal_ioctl_mttve(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg);
extern int mtal_ioctl_mtb2r(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtstc(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtpvr(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int mtal_ioctl_mtmmu(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);
    

extern int mtal_ioctl_mtvss(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

#endif  // #ifdef  __kernel__

#ifdef __cplusplus
}
#endif

#endif  // MTAL_IOCTL_H

