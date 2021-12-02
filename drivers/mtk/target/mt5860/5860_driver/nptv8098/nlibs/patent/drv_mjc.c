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
 *----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------
 * Copyright (c) 2005, MediaTek, Inc.
 * All rights reserved.
 *
 * Unauthorized use, practice, perform, copy, distribution, reproduction,
 * or disclosure of this information in whole or in part is prohibited.
 *-----------------------------------------------------------------------------
 *
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: drv_mjc.c $
 * $Revision: #1 $
 *
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Header Files
 *----------------------------------------------------------------------------*/
#include "feature.h"
#include "hw_vdoin.h"
#include "hw_ckgen.h"
#include "hw_mjc.h"
#include "hw_ttd.h"
#include "hw_sw.h"
#include "hw_mjcfd.h"
#include "hw_scpos.h"
#include "drv_mjc.h"
#include "drv_mfd.h"
#include "drv_ttd.h"
#include "drv_mjc_int.h"
#include "drv_mjc_frmsch.h"
#include "drv_mjc_fbck.h"
#include "drv_mjc_if.h"
#include "drv_scaler.h"
#include "drv_scaler_drvif.h"
#include "drv_display.h"
#include "drv_video.h"
#include "drv_tdtv_drvif.h"
#include "fbm_drvif.h"
#include "mute_if.h"
#include "pe_if.h"
#include "vdo_misc.h"
#include "vdp_display.h"
#include "video_timing.h"
#include "nptv_debug.h"
#include "x_bim.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_gpio.h"
#include "x_ckgen.h"
#include "hw_di.h"
#include "frametrack_drvif.h"
#include "util.h"
#include "panel.h"

/*----------------------------------------------------------------------------*
 * Definitions/Macros
 *----------------------------------------------------------------------------*/
#define _ME(x,n,m,val) P_Fld(val, MJC_PROG_ME##x##EN_##n##_##m)
#define _POS(n,val) ((UINT32)(val)<<(n*2))

#define _ME_04(x,v0,v1,v2,v3) _ME(x,0,4,v0)|_ME(x,1,4,v1)|_ME(x,2,4,v2)|_ME(x,3,4,v3)
#define _ME_08(x,v0,v1,v2,v3,v4,v5,v6,v7)\
    _ME(x,0,8,v0)|_ME(x,1,8,v1)|_ME(x,2,8,v2)|_ME(x,3,8,v3)|_ME(x,4,8,v4)|_ME(x,5,8,v5)|_ME(x,6,8,v6)|_ME(x,7,8,v7)
#define _ME_10(x,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9)\
    _ME(x,0,10,v0)|_ME(x,1,10,v1)|_ME(x,2,10,v2)|_ME(x,3,10,v3)|_ME(x,4,10,v4)|\
    _ME(x,5,10,v5)|_ME(x,6,10,v6)|_ME(x,7,10,v7)|_ME(x,8,10,v8)|_ME(x,9,10,v9)
#define _ME_L_20(x,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9)\
    _ME(x,0,20,v0)|_ME(x,1,20,v1)|_ME(x,2,20,v2)|_ME(x,3,20,v3)|_ME(x,4,20,v4)|\
    _ME(x,5,20,v5)|_ME(x,6,20,v6)|_ME(x,7,20,v7)|_ME(x,8,20,v8)|_ME(x,9,20,v9)
#define _ME_H_20(x,v10,v11,v12,v13,v14,v15,v16,v17,v18,v19)\
    _ME(x,10,20,v10)|_ME(x,11,20,v11)|_ME(x,12,20,v12)|_ME(x,13,20,v13)|_ME(x,14,20,v14)|\
    _ME(x,15,20,v15)|_ME(x,16,20,v16)|_ME(x,17,20,v17)|_ME(x,18,20,v18)|_ME(x,19,20,v19)

#define _POS_04(v0,v1,v2,v3) (_POS(0,v0)|_POS(1,v1)|_POS(2,v2)|_POS(3,v3))
#define _POS_08(v0,v1,v2,v3,v4,v5,v6,v7)\
    (_POS(0,v0)|_POS(1,v1)|_POS(2,v2)|_POS(3,v3)|_POS(4,v4)|_POS(5,v5)|_POS(6,v6)|_POS(7,v7))
#define _POS_10(v0,v1,v2,v3,v4,v5,v6,v7,v8,v9)\
    (_POS(0,v0)|_POS(1,v1)|_POS(2,v2)|_POS(3,v3)|_POS(4,v4)|_POS(5,v5)|_POS(6,v6)|_POS(7,v7)|_POS(8,v8)|_POS(9,v9))


// Full ME mode (only for 120Hz/60Hz MEMC)
#define MJC_PROG_ME2_04_FULL        _ME_04(2,   1,1,0,0)
#define MJC_PROG_ME3_04_FULL        _ME_04(3,   1,1,0,0)
#define MJC_PROG_ME2_08_FULL        _ME_08(2,   1,1,1,1,0,0,0,0)
#define MJC_PROG_ME3_08_FULL        _ME_08(3,   1,1,1,1,0,0,0,0)
#define MJC_PROG_ME2_10_FULL        _ME_10(2,   1,1,1,1,1,0,0,0,0,0)
#define MJC_PROG_ME3_10_FULL        _ME_10(3,   1,1,1,1,1,0,0,0,0,0)
#define MJC_PROG_ME2_L_20_FULL      _ME_L_20(2, 1,1,1,1,1,1,1,1,1,1)
#define MJC_PROG_ME3_L_20_FULL      _ME_L_20(3, 1,1,1,1,1,1,1,1,1,0)  // prebi when pos = 9 (64 mode)
#define MJC_PROG_ME2_H_20_FULL      _ME_H_20(2, 0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_ME3_H_20_FULL      _ME_H_20(3, 0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_ME3_55P2_L_20_FULL _ME_L_20(3, 1,1,1,1,0,1,1,1,1,1)  // prebi when pos = 4 (55p2 mode)

#define MJC_PROG_POS_04_FULL        _POS_04(0,2,0,0)
#define MJC_PROG_POS_08_FULL        _POS_08(0,2,2,0,0,0,0,0)
#define MJC_PROG_POS_10_FULL        _POS_10(0,0,2,2,2,0,0,0,0,0)
#define MJC_PROG_POS_H_20_FULL      _POS_10(0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_POS_L_20_FULL      _POS_10(0,0,0,2,2,2,2,2,2,0)
#define MJC_PROG_POS_55P2_L_20_FULL _POS_10(0,0,2,2,0,0,0,2,2,2)

// Half run ME mode (0 can be full, 5395 mode, only for 120Hz MEMC)
#define MJC_PROG_ME2_04_HALF        _ME_04(2,   1,1,0,0)
#define MJC_PROG_ME3_04_HALF        _ME_04(3,   1,0,0,0)
#define MJC_PROG_ME2_08_HALF        _ME_08(2,   1,1,0,1,0,0,0,0)
#define MJC_PROG_ME3_08_HALF        _ME_08(3,   1,0,1,0,0,0,0,0)
#define MJC_PROG_ME2_10_HALF        _ME_10(2,   1,1,0,1,0,0,0,0,0,0)
#define MJC_PROG_ME3_10_HALF        _ME_10(3,   1,0,1,0,1,0,0,0,0,0)
#define MJC_PROG_ME2_L_20_HALF      _ME_L_20(2, 1,1,0,1,0,1,0,1,0,1)
#define MJC_PROG_ME3_L_20_HALF      _ME_L_20(3, 1,0,1,0,1,0,1,0,1,0)  // prebi when pos = 9 (64 mode)
#define MJC_PROG_ME2_H_20_HALF      _ME_H_20(2, 0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_ME3_H_20_HALF      _ME_H_20(3, 0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_ME2_55P2_L_20_HALF _ME_L_20(2, 1,1,0,0,1,0,1,0,1,0)
#define MJC_PROG_ME3_55P2_L_20_HALF _ME_L_20(3, 1,0,1,1,0,1,0,1,0,1)  // prebi when pos = 4 (55p2 mode)

#define MJC_PROG_POS_04_HALF        _POS_04(0,2,0,0)
#define MJC_PROG_POS_08_HALF        _POS_08(0,2,2,0,0,0,0,0)
#define MJC_PROG_POS_10_HALF        _POS_10(0,0,2,2,2,0,0,0,0,0)
#define MJC_PROG_POS_H_20_HALF      _POS_10(0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_POS_L_20_HALF      _POS_10(0,0,0,2,2,2,2,2,2,0)
#define MJC_PROG_POS_55P2_L_20_HALF _POS_10(0,0,2,2,0,0,0,2,2,2)

// Single ME mode with ME2 reverse (for 120Hz MEMC)
#define MJC_PROG_ME2_04_SGME        _ME_04(2,   1,0,0,0)
#define MJC_PROG_ME3_04_SGME        _ME_04(3,   0,1,0,0)
#define MJC_PROG_ME2_08_SGME        _ME_08(2,   1,0,1,0,0,0,0,0)
#define MJC_PROG_ME3_08_SGME        _ME_08(3,   0,1,0,1,0,0,0,0)
#define MJC_PROG_ME2_10_SGME        _ME_10(2,   1,0,1,0,1,0,0,0,0,0)
#define MJC_PROG_ME3_10_SGME        _ME_10(3,   0,1,0,1,0,0,0,0,0,0)
#define MJC_PROG_ME2_L_20_SGME      _ME_L_20(2, 1,0,0,1,1,0,0,1,1,1)
#define MJC_PROG_ME3_L_20_SGME      _ME_L_20(3, 0,1,1,0,0,1,1,0,0,0)  // prebi when pos = 9 (64 mode)
#define MJC_PROG_ME2_H_20_SGME      _ME_H_20(2, 0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_ME3_H_20_SGME      _ME_H_20(3, 0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_ME2_55P2_L_20_SGME _ME_L_20(2, 1,0,1,0,1,0,1,0,1,0)
#define MJC_PROG_ME3_55P2_L_20_SGME _ME_L_20(3, 0,1,0,1,0,1,0,1,0,1)  // prebi when pos = 4 (55p2 mode)

#define MJC_PROG_POS_04_SGME        _POS_04(0,0,0,0)
#define MJC_PROG_POS_08_SGME        _POS_08(0,2,2,0,0,0,0,0)
#define MJC_PROG_POS_10_SGME        _POS_10(0,0,2,2,2,0,0,0,0,0)
#define MJC_PROG_POS_H_20_SGME      _POS_10(0,0,0,0,0,0,0,0,0,0)
#define MJC_PROG_POS_L_20_SGME      _POS_10(0,0,0,2,2,2,0,2,0,0)
#define MJC_PROG_POS_55P2_L_20_SGME _POS_10(0,0,2,2,0,0,0,2,2,2)

// 240Hz MEMC Mode
#define MJC_PROG_ME2_04_240         _ME_04(2,   0,1,0,1)
#define MJC_PROG_ME3_04_240         _ME_04(3,   1,0,1,0)
#define MJC_PROG_ME2_08_240         _ME_08(2,   0,1,0,1,0,1,0,1)
#define MJC_PROG_ME3_08_240         _ME_08(3,   1,0,1,0,1,0,1,0)
#define MJC_PROG_ME2_10_240         _ME_10(2,   0,1,0,1,0,1,0,1,0,1)
#define MJC_PROG_ME3_10_240         _ME_10(3,   1,0,1,0,1,0,1,0,1,0)
#define MJC_PROG_ME2_L_20_240       _ME_L_20(2, 0,1,0,1,0,1,0,1,0,1)
#define MJC_PROG_ME3_L_20_240       _ME_L_20(3, 1,0,1,0,1,0,1,0,1,0)  // prebi when pos = 9 (64 mode)
#define MJC_PROG_ME2_H_20_240       _ME_H_20(2, 0,1,0,1,0,1,0,1,0,1)
#define MJC_PROG_ME3_H_20_240       _ME_H_20(3, 1,0,1,0,1,0,1,0,1,0)

#define MJC_PROG_POS_04_240         _POS_04(0,2,2,0)
#define MJC_PROG_POS_08_240         _POS_08(0,0,0,2,2,2,2,0)
#define MJC_PROG_POS_10_240         _POS_10(0,0,0,0,0,2,2,2,2,0)
#define MJC_PROG_POS_L_20_240       _POS_10(0,0,0,0,0,0,0,2,0,2)
#define MJC_PROG_POS_H_20_240       _POS_10(2,2,2,2,2,0,2,2,2,0)
#define MJC_PROG_POS_55P2_L_20_240  _POS_10(0,0,0,0,2,2,2,2,2,0)
#define MJC_PROG_POS_55P2_H_20_240  _POS_10(0,0,0,0,2,2,2,2,2,0)

// rdc type
#define MJC_PROG_BWRDC_N_04_120   0x0           // 0000
#define MJC_PROG_BWRDC_N_08_120   0xA0          // 00002200
#define MJC_PROG_BWRDC_N_10_120   0x280         // 0000022000
#define MJC_PROG_BWRDC_N_20_120   0x00000AA800  // 00000000002222200000
#define MJC_PROG_BWRDC_55P2_N_20_120   0x00000A0280    // 00000000002200022000


#define MJC_PROG_BWRDC_N_04_240   0xA0          // 2200
#define MJC_PROG_BWRDC_N_08_240   0xAA0         // 22220000
#define MJC_PROG_BWRDC_N_10_240   0xAA000       // 2222000000
#define MJC_PROG_BWRDC_N_20_240   0xAAAAA00000  // 22222222220000000000
#define MJC_PROG_BWRDC_55P2_N_20_240   0xAA800AA800    // 22222000002222200000

#define MJC_INIT_MUTE_COUNT    20    // Set MJC Init mute to show loader logo  

/*----------------------------------------------------------------------------*
 * Global/Static variables
 *----------------------------------------------------------------------------*/
CODE MJC_REGTBL_T MJC_SYS_INIT[] =
{
    {MJC_SYS_00, 0x00171001, 0xFFFFFFFF},
    {MJC_SYS_01, 0x11000000, 0xFF000000},
    {MJC_SYS_02, 0x0F438780, 0x00FFFFFF},
    {MJC_SYS_03, 0x00000000, 0x18000000},
    {MJC_SYS_04, 0x00438780, 0xFFFFFFFF},
    {MJC_SYS_05, 0x10465898, 0xFFFFFFFF},
    {MJC_SYS_07, 0x02000000, 0x02000000},
    {MJC_SYS_08, 0x00000000, 0xFFFFFFFF},
    {MJC_SYS_09, 0x00000FFF, 0x0000FFFF},
    {MJC_SYS_0E, 0x00003795, 0xFFFFFFFF},
    {MJC_SYS_0F, 0x04565F95, 0xFFFFFFFF},
    {MJC_SYS_D2, 0x00030000, 0xFFFFFFFF},
    {MJC_SYS_D8, 0x001C4050, 0x0000FFFF}, // MJC_DIPS_VPOS = 28
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

CODE MJC_REGTBL_T MJC_FB_INIT[] =
{
    {MJC_FB_00, 0x03001000, 0xFFFFFFFF},
    {MJC_FB_01, 0x80000000, 0xFF000000},
    {MJC_FB_03, 0x80190010, 0xFFFFFFFF},
    {MJC_FB_0A, 0x00000001, 0xFFFFFFFF},
    {MJC_FB_0C, 0xC05A0C16, 0xFFFFFFFF},
    {MJC_FB_10, 0x07000000, 0x07000000},
    {MJC_FB_14, 0x03F0CD00, 0x03FFFF00},
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

CODE MJC_REGTBL_T MJC_FM_INIT[] =
{
    {MJC_FM_00, 0x01E1EF40, 0xFFFFFFFF}, // FC by moment, MJC_FM_EXTRA_DLY = 1
    {MJC_FM_01, 0x00200000, 0x00600000},
    {MJC_FM_04, 0x03800000, 0x03800000},
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

CODE MJC_REGTBL_T MJC_ME_INIT[] =
{
    {MJC_ME_00, 0x4C04005A, 0xFFFFFFFF},
    {MJC_ME_01, 0xE2200200, 0xFFFFFFFF},
    {MJC_ME_02, 0xA1004800, 0xFFFFFFFF},
    {MJC_ME_03, 0x9868c800, 0xFFFFFFFF},
    {MJC_ME_04, 0xF8681000, 0xFFFFFFFF},
    {MJC_ME_05, 0x38000040, 0xFFFFFFFF},
    {MJC_ME_06, 0x01020202, 0xFFFFFFFF},
    {MJC_ME_07, 0x70021C00, 0xFFFFFFFF},
    {MJC_ME_08, 0x16016B00, 0xFFFFFFFF}, // ME_ST_DLY_TH = 16
    {MJC_ME_09, 0x90103655, 0xFFFFFFFF},
    {MJC_ME_0A, 0x04058838, 0xFFFFFFFF},
    {MJC_ME_0B, 0x00000808, 0xFFFFFFFF},
    {MJC_ME_0C, 0x08080808, 0xFFFFFFFF},
    {MJC_ME_0D, 0x800410D0, 0xFFFFFFFF},
    {MJC_ME_0E, 0x00000020, 0xFFFFFFFF},
    {MJC_ME_0F, 0x00552548, 0xFFFFFFFF},
    {MJC_ME_10, 0x88080020, 0xFFFFFFFF},
    {MJC_ME_16, 0x00800080, 0xFFFFFFFF},
    {MJC_ME_17, 0x00800080, 0xFFFFFFFF},
    {MJC_ME_18, 0x37701384, 0xFFFFFFFF},
    {MJC_ME_19, 0x7D081010, 0xFFFFFFFF},
    {MJC_ME_1A, 0x80DF180A, 0xFFFFFFFF},
    {MJC_ME_1B, 0x83001E08, 0xFFFFFFFF},
    {MJC_ME_1C, 0x55400100, 0xFFFFFFFF},
    {MJC_ME_1D, 0x9E801010, 0xFFFFFFFF},
    {MJC_ME_1E, 0xDA050060, 0xFFFFFFFF},
    {MJC_ME_1F, 0x02020202, 0xFFFFFFFF},
    {MJC_ME_20, 0x0AF80100, 0xFFFFFFFF},  
    {MJC_ME_21, 0x000D2244, 0xFFFFFFFF},  
    {MJC_ME_22, 0x080800D0, 0xFFFFFFFF},
    {MJC_ME_23, 0x83001E08, 0xFFFFFFFF},
    {MJC_ME_24, 0x5A000040, 0xFFFFFFFF},  
    {MJC_ME_25, 0x3C001E0F, 0xFFFFFFFF},  
    {MJC_ME_26, 0x1F281F05, 0xFFFFFFFF},
    {MJC_ME_27, 0x1E015157, 0xFFFFFFFF},
    {MJC_ME_28, 0x1E06000C, 0xFFFFFFFF},  
    {MJC_ME_29, 0x1C444304, 0xFFFFFFFF},  
    {MJC_ME_2A, 0x04041010, 0xFFFFFFFF},
    {MJC_ME_2B, 0x00010120, 0xFFFFFFFF},
    {MJC_ME_2C, 0x002008f8, 0xFFFFFFFF},  
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

CODE MJC_REGTBL_T MJC_MV_INIT[] =
{
    {MJC_MV_00, 0x08110000, 0xFFFFFFFF},
    {MJC_MV_01, 0x4411FFFF, 0xFFFFFFFF},
    {MJC_MV_02, 0x035ABC3C, 0xFFFFFFFF},
    {MJC_MV_03, 0x20401C00, 0xFFFFFFFF},
    {MJC_MV_04, 0x60124434, 0xFFFFFFFF}, // MV_04: 
    {MJC_MV_05, 0x03811022, 0xFFFFFFFF}, // MV_05: 00000022
    {MJC_MV_06, 0x00001818, 0xFFFFFFFF}, // MV_06: 00001605
    {MJC_MV_07, 0x48202141, 0xFFFFFFFF}, // MV_07: vsi sel center
    {MJC_MV_08, 0x33881A68, 0xFFFFFFFF}, // MV_08: 33881C8A
    {MJC_MV_09, 0x30198020, 0xFFFFFFFF}, // MV_09: 
    {MJC_MV_0A, 0x10220534, 0xFFFFFFFF}, // MV_0A: 
    {MJC_MV_0B, 0x00010630, 0xFFFFFFFF}, // MV_0B: 42a setting
    {MJC_MV_0C, 0x0A10E000, 0xFFFFFFFF}, // MV_0C: bmval enable [1:0]
    {MJC_MV_0D, 0x08410841, 0xFFFFFFFF},
    {MJC_MV_0E, 0x00120000, 0xFFFFFFFF}, // MV_TF_START_ADDR=0x120000
    {MJC_MV_0F, 0x0012FFFF, 0xFFFFFFFF}, // MV_TF_END_ADDR=0x12FFFF
    {MJC_MV_10, 0x00000000, 0xFFFFFFFF}, // MV_10: 
    {MJC_MV_11, 0x00000028, 0xFFFFFFFF}, // MV_11:

    {MJC_MV_22, 0x08080808, 0xFFFFFFFF},
    {MJC_MV_24, 0x00050131, 0xFFFFFFFF}, // APL setting
    {MJC_MV_25, 0x0010005E, 0xFFFFFFFF}, // 3*1920*1080/(256*256)
    {MJC_MV_26, 0x0010017B, 0xFFFFFFFF}, // 12*1920*1080/(2^16)
    {MJC_MV_27, 0x0000011C, 0xFFFFFFFF}, // 9*1920*1080/(2^16)
    {MJC_MV_2A, 0x00000000, 0xFFFFFFFF}, // MV_2A: 
    {MJC_MV_2B, 0x50130202, 0xFFFFFFFF},
    {MJC_MV_2C, 0x84888844, 0xFFFFFFFF}, // MV_2C: 
    {MJC_MV_2D, 0x1000A051, 0xFFFFFFFF}, // MV_2D: [6]:bgmv_fgbg_chk=1,[7]:bg_keep_sel=0,
    {MJC_MV_2E, 0x34341C18, 0xFFFFFFFF}, // MV_2E: [4]:mvc_tpos_en=1, [25:24]:mvc_sml_det_mod=0
    {MJC_MV_2F, 0x44441224, 0xFFFFFFFF}, // MV_2F: toc_ex gain
    {MJC_MV_30, 0x0A0A6844, 0xFFFFFFFF}, // MV_30: fg err detect
    {MJC_MV_31, 0x443A0404, 0xFFFFFFFF}, // MV_31: [17]: range_tops_en [19]:toc_tpos=1,[20]:bgf_tpos=1,
    {MJC_MV_32, 0x00301777, 0xFFFFFFFF}, // MV_32: osd protection
    {MJC_MV_33, 0x24484421, 0xFFFFFFFF}, // MV_33: gain2,
    {MJC_MV_34, 0x80808080, 0xFFFFFFFF}, // MV_34: toc_ex max/min,
    {MJC_MV_36, 0x00000002, 0xFFFFFFFF}, // MV_36: [1]:exp_mvc=1,
    {MJC_MV_37, 0x23455653, 0xFFFFFFFF}, // MV_37: 
    {MJC_MV_38, 0x00682824, 0xFFFFFFFF}, // MV_38: 
    {MJC_MV_39, 0x00030768, 0xFFFFFFFF}, // MV_39: BND_HLR for right and down (ECO)
    {MJC_MV_3B, 0x38686543, 0xFFFFFFFF}, // MV_3B: 
    {MJC_MV_3C, 0x1F0000CC, 0xFFFFFFFF}, // MV_3C: disable near clst sel
    {MJC_MV_3D, 0x23686868, 0xFFFFFFFF}, // MV_3D: 
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

CODE MJC_REGTBL_T MJC_PSG_INIT[] =
{
    #ifdef CC_FPGA
    // BDR VSI
    {MJC_MV_04, 0x40023322, 0xFFFFFFFF}, // MV_04: 
    #endif
    // PSG
    {MJC_MV_13, 0x02020013, 0xFFFFFCFF}, // MV_13: 
    {MJC_MV_14, 0xA22A0088, 0xFFFFFFFF}, // MV_14: 
    {MJC_MV_15, 0x75321018, 0xFFFFFFFF}, // MV_15: 
    {MJC_MV_16, 0xCA865432, 0xFFFFFFFF}, // MV_16: 
    {MJC_MV_17, 0x0020200F, 0xFFFFFFFF}, // MV_17:
    {MJC_MV_18, 0xF1F7F2F7, 0xFFFFFFFF}, // MV_18:  
    {MJC_MV_19, 0x0000F03F, 0xFFFFFFFF}, // MV_19: 
    {MJC_MV_1A, 0x00070F77, 0xFFFFFFFF}, // MV_1A:   
    {MJC_MV_1B, 0x00668FAC, 0xFFFFFFFF}, // MV_1B:  
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

CODE MJC_REGTBL_T MJC_CA_INIT[] =
{
    {MJC_CA_00, 0x200C0202, 0xFFFFFFFF}, // CA_00: 
    {MJC_CA_01, 0x08081000, 0xFFFFFFFF}, // CA_01: 
    {MJC_CA_02, 0x00102000, 0xFFFFFFFF}, // CA_02: 
    {MJC_CA_03, 0x04040701, 0xFFFFFFFF}, // CA_03: 
    {MJC_CA_04, 0x88003301, 0xFFFFFFFF}, // CA_04: 88222251
    {MJC_CA_05, 0x68461125, 0xFFFFFFFF}, // CA_05: 
    {MJC_CA_06, 0x71181818, 0xFFFFFFFF}, // CA_06: 
    {MJC_CA_07, 0x66660001, 0xFFFFFFFF}, // CA_07: [0]:fgbg_msad_chk,
    {MJC_CA_08, 0x03041000, 0xFFFFFFFF}, // CA_08: 
    {MJC_CA_09, 0x00056868, 0xFFFFFFFF}, // CA_09: 
    {MJC_CA_0A, 0xC1228833, 0xFFFFFFFF}, // CA_0A: 
    {MJC_CA_0B, 0x33090909, 0xFFFFFFFF}, // CA_0B: 
    {MJC_CA_0C, 0x00000028, 0xFFFFFFFF}, // CA_0C: 
    {MJC_CA_0D, 0x3F1F0004, 0xFFFFFFFF}, // CA_0D: 
    {MJC_CA_0E, 0x0FFF7F00, 0xFFFFFFFF}, // CA_0E: 
    {MJC_CA_0F, 0x00000822, 0xFFFFFFFF}, // CA_0F: 
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

CODE MJC_REGTBL_T MJC_MC_INIT[] =
{
    {MJC_MC_00, 0x00000606, 0xFFFFFFFF}, // MC_00: 
    {MJC_MC_01, 0x06003025, 0xFFFFFFFF}, // MC_01:
    {MJC_MC_02, 0x01000000, 0xFFFFFFFF}, // MC_02: intp mirror
    {MJC_MC_03, 0x00003340, 0xFFFFFFFF}, // MC_03: 42a setting (current: still only)
    {MJC_MC_04, 0x02552222, 0xFFFFFFFF}, // MC_04: MC_1B mc lbox offset (wait debug)
    {MJC_MC_05, 0x10FB8802, 0xFFFFFFFF}, // MC_05: MC_05 ovl_uni_en, occ_mirror_off
    {MJC_MC_06, 0x00010000, 0xFFFFFFFF}, // MC_06: vdb ctrl
    {MJC_MC_07, 0xFFFF2D50, 0xFFFFFFFF}, // MC_07: vdb v,u,y,x
    {MJC_MC_08, 0xFFFF0044, 0xFFFFFFFF}, // MC_08: vdb code
    {MJC_MC_09, 0x00000000, 0xFFFFFFFF}, // MC_09: vdb misc
    {MJC_MC_0A, 0x00000000, 0xFFFFFFFF}, // MC_0A: force mv
    {MJC_MC_0B, 0x28000000, 0xFFFFFFFF}, // MC_0B: demo bound
    {MJC_MC_0C, 0x00000000, 0xFFFFFFFF}, // MC_0C: demo bar
    {MJC_MC_16, 0x00000000, 0xFFFFFFFF}, // MC_16: osd
    {MJC_MC_17, 0x00000000, 0xFFFFFFFF}, // MC_17: osd
    {MJC_MC_18, 0x00000000, 0xFFFFFFFF}, // MC_18: osd
    {MJC_MC_19, 0x00000000, 0xFFFFFFFF}, // MC_19: osd
    {MJC_MC_1A, 0x00000000, 0xFFFFFFFF}, // MC_1A: osd
    {MJC_MC_1B, 0x00000000, 0xFFFFFFFF}, // MC_1B: osd
    {MJC_MC_1C, 0x00000000, 0xFFFFFFFF}, // MC_1C: osd
    {MJC_MC_1D, 0x00000000, 0xFFFFFFFF}, // MC_1D: osd
    {MJC_MC_1E, 0x00002ACA, 0xFFFFFFFF}, // MC_1E: bd_clip
    {MJC_MC_1F, 0x201CC0BE, 0xFFFFFFFF}, // MC_1F: bd_clip
    {MJC_MC_20, 0x00008003, 0xFFFFFFFF}, // MC_20:video PartFbk
    {MJC_MC_21, 0xC0804000, 0xFFFFFFFF}, // MC_21:pd22 PartFbk
    {MJC_MC_22, 0x00000000, 0xFFFFFFFF}, //
    {MJC_MC_23, 0x99663300, 0xFFFFFFFF}, // MC_24:pd32 55mode PartFbk
    {MJC_MC_24, 0x000000CC, 0xFFFFFFFF}, //
    {MJC_MC_25, 0x00000000, 0xFFFFFFFF}, //
    {MJC_MC_26, 0x66333300, 0xFFFFFFFF}, // MC_36:pd32 64mode PartFbk
    {MJC_MC_27, 0xCC999966, 0xFFFFFFFF}, //
    {MJC_MC_28, 0x0000FFCC, 0xFFFFFFFF}, //
    {MJC_MC_29, 0x00000000, 0xFFFFFFFF}, //
    {MJC_MC_2A, 0x00000000, 0xFFFFFFFF}, //
    {MJC_REGTBL_END, 0x00000000, 0x00000000}
};

MJC_SP_THREAD MJCSPThread[] =
{
    {vDrvMJCFDStatusPolling,    "MJCFD Status Polling"},
    {vDrvMJCRegStatusPolling,   "MJC Register Status Polling"},
    {vDrvMJCMEStaPolling,       "MJC ME Status Polling"},
    {vDrvMJCFrmSchHWLog,        "MJC HW FrmSch Polling, Please use Vsync(1)"},
    {vDrvMJCFrmSchFWLog,        "MJC FW FrmSch Polling, Please use Vsync(1)"},
    {NULL,                      NULL},
    {vDrvMJCDramOvfUdfPolling,  "MJC DRAM Over/Underflow Monitoring"},
    {vDrvMJCInputLRCheck,       "MJC Input 3D FS LR Check"},
    {vDrvMJCInOutCRCCheck,      "MJC IN/OUT CRC check, must open front scaler patgen"}
};

UINT8 fgMJCStatusPolling;
UINT8 u1TestDelayCnt = 20;
UINT8 fgMJC3DModeDbg = 0;
UINT8 u1MJC3DMode = 0;
UINT8 u1ModeChangeRst = 0;
UINT8 fgMJCModeChg = SV_FALSE;

MJC_DRAM_ALLOC_T arMJCDramAlloc;
MJC_PRM_T        arMJCPrm = {SV_FALSE};
MJC_SP_REG_T     arMJCSPReg[8];
MJC_DEMO_CFG_T   arMJCDemo =
{
    E_MJC_DEMO_FULL,
    {E_MJC_RGN_NF, E_MJC_RGN_NF, E_MJC_RGN_NF},
    0, 0, 0, 0
};

static UINT8 _u1TrigSetPsAuto = 0;
static UINT8 _u1DelaySetCnt[E_MJC_DELAY_MAX] = {0};

static UINT32 _u4DramStaMonitorCount = 0xFFFFFFFF;

static BOOL _fgIsInitialized = FALSE;
//static HANDLE_T _hDrvMJCSema;

static PThreadFunc _pfnMJCAdjInputVsync = NULL;
static PThreadFunc _pfnMJCOutputVsync   = NULL;
static PThreadFunc _pfnMJCInputVsync    = NULL;
static PThreadFunc _pfnMJCProcFinished  = NULL;

static UINT32 u4MJCTTDPrev = 0;

static UINT32 _u4MJCStatusPollingEnable = 0;
UINT32 u4MJCStatusPollingCount = 0;
UINT8  u1MJCStatusCheck = 0;

MJC_SP_VSYNC_T MJCSPVsyncIsr[] =
{
    {&_pfnMJCAdjInputVsync, "MJC Adjusted Input Vsync End"},
    {&_pfnMJCOutputVsync,   "MJC Output Vsync End"},
    {&_pfnMJCInputVsync,    "MJC Input Vsync End"},
    {&_pfnMJCProcFinished,  "MJC Processing Finished"},
    {NULL,                  NULL}
};

static MJC_DRAM_MODE_T _arMJCAllDramMode[MJC_DRAM_ALL_MODE_NUM + 1] =
{
    {MJC_Y10_C10_422_0RDC_3FRM, "MJC_Y10_C10_422_0RDC_3FRM"},
    {MJC_Y10_C10_422_1RDC_3FRM, "MJC_Y10_C10_422_1RDC_3FRM"},
    {MJC_Y10_C10_420_0RDC_3FRM, "MJC_Y10_C10_420_0RDC_3FRM"},
    {MJC_Y10_C10_420_1RDC_3FRM, "MJC_Y10_C10_420_1RDC_3FRM"},
    {MJC_Y10_C10_422_0RDC_2FRM, "MJC_Y10_C10_422_0RDC_2FRM"},
    {MJC_Y10_C10_422_1RDC_2FRM, "MJC_Y10_C10_422_1RDC_2FRM"},
    {MJC_Y10_C10_420_0RDC_2FRM, "MJC_Y10_C10_420_0RDC_2FRM"},
    {MJC_Y10_C10_420_1RDC_2FRM, "MJC_Y10_C10_420_1RDC_2FRM"},
    {MJC_Y10_C10_422_0RDC_1FRM, "MJC_Y10_C10_422_0RDC_1FRM"},
    {MJC_Y10_C10_420_0RDC_1FRM, "MJC_Y10_C10_420_0RDC_1FRM"},
    {MJC_Y10_C08_422_0RDC_3FRM, "MJC_Y10_C08_422_0RDC_3FRM"},
    {MJC_Y10_C08_422_1RDC_3FRM, "MJC_Y10_C08_422_1RDC_3FRM"},
    {MJC_Y10_C08_420_0RDC_3FRM, "MJC_Y10_C08_420_0RDC_3FRM"},
    {MJC_Y10_C08_420_1RDC_3FRM, "MJC_Y10_C08_420_1RDC_3FRM"},
    {MJC_Y10_C08_422_0RDC_2FRM, "MJC_Y10_C08_422_0RDC_2FRM"},
    {MJC_Y10_C08_422_1RDC_2FRM, "MJC_Y10_C08_422_1RDC_2FRM"},
    {MJC_Y10_C08_420_0RDC_2FRM, "MJC_Y10_C08_420_0RDC_2FRM"},
    {MJC_Y10_C08_420_1RDC_2FRM, "MJC_Y10_C08_420_1RDC_2FRM"},
    {MJC_Y10_C08_422_0RDC_1FRM, "MJC_Y10_C08_422_0RDC_1FRM"},
    {MJC_Y10_C08_420_0RDC_1FRM, "MJC_Y10_C08_420_0RDC_1FRM"},
    {MJC_Y08_C08_422_0RDC_3FRM, "MJC_Y08_C08_422_0RDC_3FRM"},
    {MJC_Y08_C08_422_1RDC_3FRM, "MJC_Y08_C08_422_1RDC_3FRM"},
    {MJC_Y08_C08_420_0RDC_3FRM, "MJC_Y08_C08_420_0RDC_3FRM"},
    {MJC_Y08_C08_420_1RDC_3FRM, "MJC_Y08_C08_420_1RDC_3FRM"},
    {MJC_Y08_C08_422_0RDC_2FRM, "MJC_Y08_C08_422_0RDC_2FRM"},
    {MJC_Y08_C08_422_1RDC_2FRM, "MJC_Y08_C08_422_1RDC_2FRM"},
    {MJC_Y08_C08_420_0RDC_2FRM, "MJC_Y08_C08_420_0RDC_2FRM"},
    {MJC_Y08_C08_420_1RDC_2FRM, "MJC_Y08_C08_420_1RDC_2FRM"},
    {MJC_Y08_C08_422_0RDC_1FRM, "MJC_Y08_C08_422_0RDC_1FRM"},
    {MJC_Y08_C08_420_0RDC_1FRM, "MJC_Y08_C08_420_0RDC_1FRM"},
    {FRC_Y10_C10_444, "FRC_Y10_C10_444"},
    {FRC_Y08_C08_444, "FRC_Y08_C08_444"},
    {FRC_Y10_C10_422RPT, "FRC_Y10_C10_RPT422"},
	{FRC_Y08_C08_422RPT, "FRC_Y08_C08_RPT422"},
    {0xFF, "Unknow"}
};

static UINT32 fgMJCFrmDlyChk = SV_OFF;

//EXTERN UINT32 u4ScpipGetPscanDispmodeHLen(UINT8 bPath);
//EXTERN void vScpipOnMjcOutVsync(void);


/*----------------------------------------------------------------------------*
 * Function Members
 *----------------------------------------------------------------------------*/
static UINT32 u4DrvMJCCalcSizeYC(UINT32 u4Width, UINT32 u4Height, UINT32 u4Dram444, UINT32 u4Dram420, UINT32 u4Y8, UINT32 u4C8)
{
    UINT32 u4Bit, u4WxH, u4Size;

    u4WxH = ALIGN_16_CEIL(u4Width)*ALIGN_2_CEIL(u4Height);
    
    if (u4Dram444)
    {
        u4Bit = (u4Y8) ? 24 : 30;
        u4Size = ((u4WxH*u4Bit) >> 3)*3;
    }
    else
    {
        u4Bit = (u4Dram420) ? (15 - (u4Y8 << 1) - u4C8)  : (20 - ((u4Y8 + u4C8) << 1));
        u4Size = ((u4WxH*u4Bit) >> 3)*9;
    }

    return (u4Size << 1);
}

/*static UINT32 u4DrvMJCCalcSizeFilm(UINT32 u4Width, UINT32 u4Height, UINT32 u4Dram444)
{
    UINT32 u4Size = (u4Dram444) ? 0 : (ALIGN_8_CEIL(u4Width) * ALIGN_8_CEIL(u4Height));

    return u4Size;  
}*/

static UINT32 u4DrvMJCCalcSizeOSD(UINT32 u4Width, UINT32 u4Height, UINT32 u4Dram444)
{
    UINT32 u4Size = (u4Dram444) ? 0 : (ALIGN_64_CEIL(ALIGN_8_CEIL(u4Width) >> 2) * (ALIGN_8_CEIL(u4Height) >> 3));

    return u4Size;  
}

static UINT32 u4DrvMJCCalcSizeMV(UINT32 u4Width, UINT32 u4Height, UINT32 u4Dram444)
{
    UINT32 u4Size = (u4Dram444) ? 0 : (ALIGN_64_CEIL(ALIGN_8_CEIL(u4Width) >> 2) * (ALIGN_8_CEIL(u4Height) >> 3) * 5);

    return u4Size;  
}

static UINT32 u4DrvMJCCalcSizeTBA(UINT32 u4Width, UINT32 u4Height, UINT32 u4Dram444)
{
    UINT32 u4Size = (u4Dram444) ? 0 : (ALIGN_64_CEIL(ALIGN_8_CEIL(u4Width) >> 3) * (ALIGN_8_CEIL(u4Height) >> 3));

    return u4Size;  
}

static UINT32 u4DrvMJCCalcSizePR(UINT32 u4Width, UINT32 u4Height, UINT32 u4Dram444, UINT32 u4Dram420, UINT32 u4Y8, UINT32 u4C8)
{
    UINT32 u4Bit, u4WxH, u4Size;

    u4WxH = ALIGN_16_CEIL(u4Width)*ALIGN_2_CEIL(u4Height);
    
    if (u4Dram444)
    {
        u4Bit = (u4Y8) ? 24 : 30;
    }
    else
    {
        u4Bit = (u4Dram420) ? (15 - (u4Y8 << 1) - u4C8)  : (20 - ((u4Y8 + u4C8) << 1));
    }
    u4Size = ((u4WxH*u4Bit) >> 3) * 3;
    return (u4Size << 1);
}


UINT32 u4DrvMJCRemapMJCFbmMode(UINT32 u4FbmMode)
{
    #ifndef CC_FPGA
    switch (u4FbmMode & 0x7FFF0000)
    {
    case FBM_POOL_MODE_MJC_Y10_C10_422_0RDC_3FRM: return MJC_Y10_C10_422_0RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y10_C10_422_1RDC_3FRM: return MJC_Y10_C10_422_1RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y10_C10_420_0RDC_3FRM: return MJC_Y10_C10_420_0RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y10_C10_420_1RDC_3FRM: return MJC_Y10_C10_420_1RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y10_C08_422_0RDC_3FRM: return MJC_Y10_C08_422_0RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y10_C08_422_1RDC_3FRM: return MJC_Y10_C08_422_1RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y10_C08_420_0RDC_3FRM: return MJC_Y10_C08_420_0RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y10_C08_420_1RDC_3FRM: return MJC_Y10_C08_420_1RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y08_C08_422_0RDC_3FRM: return MJC_Y08_C08_422_0RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y08_C08_422_1RDC_3FRM: return MJC_Y08_C08_422_1RDC_3FRM;
    default:
    case FBM_POOL_MODE_FRC_Y10_C10_444: return FRC_Y10_C10_444;
    case FBM_POOL_MODE_FRC_Y08_C08_444: return FRC_Y08_C08_444;         
    case FBM_POOL_MODE_MJC_Y08_C08_420_0RDC_3FRM: return MJC_Y08_C08_420_0RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y08_C08_420_1RDC_3FRM: return MJC_Y08_C08_420_1RDC_3FRM;
    case FBM_POOL_MODE_MJC_Y08_C08_420_0RDC_1FRM: return MJC_Y08_C08_420_0RDC_1FRM;
    }
    #else
    return FRC_Y10_C10_444;
    #endif
}

UINT32 u4DrvMJCGetSpEn(void)
{
    return _u4MJCStatusPollingEnable;
}

void vDrvMJCSetSpEn(UINT32 u4Msk, UINT32 u4Count)
{
    _u4MJCStatusPollingEnable |= u4Msk;
    u4MJCStatusPollingCount = u4Count;
}

void vDrvMJCClearSpEn(UINT32 u4Msk)
{
    _u4MJCStatusPollingEnable &= ~u4Msk;
}

BOOL fgDrvMJCIsInitialized(void)
{
    return _fgIsInitialized;
}

void vDrvMJCSetPsAutoTrigOn(void)
{
    _u1TrigSetPsAuto = 1;
}

void vDrvMJCRunMbist(void)
{
    static UINT32 fgMbistDone = 0xFF;    
    #if MJC_2ND_MBIST_CHK
    UINT32 fg2ndTest = SV_TRUE;
    #endif

    if (fgMbistDone == 0xFF)
    {
        #if defined(CC_FPGA) || !defined(MJC_INIT_MBIST_FLOW)
        fgMbistDone = SV_FALSE;
        return;
        #endif
    }

    if (fgMbistDone == SV_TRUE)
    {
        return;
    }

    fgMbistDone = SV_TRUE;

    // disable MBIST reset
    vIO32WriteFld(CKGEN_MBIST_CTRL, SV_ON, FLD_MBIST_RSTB);
    
    // enable mbist repair function
    MJC_WRITE_FLD(MJC_SA_01, 1, MJC_FW_RP_ENABLE);

#if MJC_2ND_MBIST_CHK
START_BIST_PROC:
#endif
    // start bist proc
    MJC_WRITE_FLD(MJC_SA_01, 0, MJC_FW_MJCRP_RST_B);
    MJC_WRITE_FLD(MJC_SA_01, 1, MJC_FW_MJCRP_BISTEN);
    MJC_WRITE_FLD(MJC_SA_01, 1, MJC_FW_MJCRP_RST_B);

    // wait mbist done
    while (MJC_READ_FLD(MJC_STA_SA_03, MJC_STA_ALL_RP_MBIST_DONE) != SV_TRUE)
    {
        LOG(0, "MBIST is not ready\n");
    }

    // check mbist fail
    if (MJC_READ_FLD(MJC_STA_SA_03, MJC_STA_ALL_RP_MBIST_FAIL))
    {
        LOG(0, "MJC mbist is FAILED!\n");
        return;
    }

    #if MJC_2ND_MBIST_CHK
    // check mbist repair bits
    if (MJC_READ_FLD(MJC_STA_SA_00, MJC_STA_RP_OK0) ||
        MJC_READ_FLD(MJC_STA_SA_01, MJC_STA_RP_OK1) ||
        MJC_READ_FLD(MJC_STA_SA_02, MJC_STA_RP_OK2))
    {
        if (fg2ndTest == SV_FALSE)
        {
            MJC_WRITE_FLD(MJC_SA_01, 0, MJC_FW_MJCRP_BISTEN);
            LOG(0, "MJC mbist repair OK.\n");
            return;
        }
        fg2ndTest = SV_FALSE;
        goto START_BIST_PROC; // avoid repair memory fail
    }
    #endif

    MJC_WRITE_FLD(MJC_SA_01, 0, MJC_FW_MJCRP_BISTEN);
	
    vIO32WriteFld(CKGEN_MBIST_CTRL, SV_OFF, FLD_MBIST_RSTB);
    
    LOG(0, "MJC mbist is unnecessary.\n");
    return;
}

void vDrvMJCSetFBOneFrmOnOff(BOOL fgOnOff)
{
    //UINT8 u1IntMsk = 0;

    arMJCPrm.u1OneFrm = fgOnOff;

    //u1IntMsk = MJC_READ_FLD(MJC_SYS_02, MJC_INT_MASK);
    
    if (fgOnOff)
    {
    //1.  Rescan OFF
    //2.  Set to display previous frame
    //3.  Set BW reduction type to NOT dropping previous fields
    //4.  Set one_frame register

    // turn off rescan, bandwidth reduction, letter box, film mode
    // MC intp mode = repeat
        vDrvMJCSetPSAutoOnOff(SV_OFF);
        MJC_WRITE_FLD(MJC_SA_00, 0, MJC_RESCAN_EN);
        
        //if (u1IntMsk & MJC_INT_3)
        {
            MJC_WRITE_FLD_MULTI(MJC_MC_00,
                P_Fld(0, MJC_MC_INTP_MODE_RGN2) | 
                P_Fld(0, MJC_MC_INTP_MODE_RGN1) |
                P_Fld(0, MJC_MC_INTP_MODE_RGN0));
        }
        
        MJC_WRITE_FLD_MULTI(MJC_SA_00,
            P_Fld(0, MJC_ME_LBOX_HALO_REDUCE_EN) | 
            P_Fld(0, MJC_MC_LBOX_HALO_REDUCE_EN));
        
        //MJC_WRITE_FLD(MJC_FB_0A, 0, MJC_BANDWIDTH_REDUCTION_EN);
        MJC_WRITE_FLD_MULTI(MJC_FM_08,
            P_Fld(0, MJC_PROG_RDCTYPE_0_4)|P_Fld(0, MJC_PROG_RDCTYPE_1_4)|
            P_Fld(0, MJC_PROG_RDCTYPE_2_4)|P_Fld(0, MJC_PROG_RDCTYPE_3_4));

        //x_thread_delay(100);

        MJC_WRITE_FLD(MJC_FB_01, 1, MJC_FB_ONE_FRM_EN);        
    }
    else
    {
        MJC_WRITE_FLD(MJC_FB_01, 0, MJC_FB_ONE_FRM_EN);

        //x_thread_delay(100);

        MJC_WRITE_FLD_MULTI(MJC_FM_08,
            P_Fld(0, MJC_PROG_RDCTYPE_0_4)|P_Fld(0, MJC_PROG_RDCTYPE_1_4)|
            P_Fld(2, MJC_PROG_RDCTYPE_2_4)|P_Fld(2, MJC_PROG_RDCTYPE_3_4));
        
        MJC_WRITE_FLD_MULTI(MJC_SA_00, 
            P_Fld(1, MJC_ME_LBOX_HALO_REDUCE_EN) |
            P_Fld(1, MJC_MC_LBOX_HALO_REDUCE_EN));
        
        MJC_WRITE_FLD_MULTI(MJC_MC_00,
                P_Fld(6, MJC_MC_INTP_MODE_RGN2) | 
                P_Fld(6, MJC_MC_INTP_MODE_RGN1) |
                P_Fld(6, MJC_MC_INTP_MODE_RGN0));

        MJC_WRITE_FLD(MJC_SA_00, arMJCPrm.u1Rscn, MJC_RESCAN_EN);
        //MJC_WRITE_FLD(MJC_FB_0A, arMJCPrm.u1Rdc, MJC_BANDWIDTH_REDUCTION_EN);
        
        vDrvMJCSetPsAutoTrigOn();
    }

    vDrvMJCUpdateIntpCtrl();
}

void vDrvMJCInit(UINT8 bForceInit)
{
    if (_fgIsInitialized && (bForceInit!=TRUE))
    {
        LOG(0, "MJC had been initialized.\n");
        return;
    }
    
    LOG(0, "MJC Initialize.\n");

    vDrvMJCSetClock();

    vDrvMJCLoadRegTbl(MJC_SYS_INIT);
    vDrvMJCLoadRegTbl(MJC_FB_INIT);
    vDrvMJCLoadRegTbl(MJC_FM_INIT);
    #ifndef CC_FPGA
    vDrvMJCLoadRegTbl(MJC_ME_INIT);
    vDrvMJCLoadRegTbl(MJC_MV_INIT);
    vDrvMJCLoadRegTbl(MJC_CA_INIT);
    vDrvMJCLoadRegTbl(MJC_MC_INIT);
    #endif
    vDrvMJCLoadRegTbl(MJC_PSG_INIT);

    vIO32Write4B(MJC_MISC_00, 0x04142800);

    #ifdef CC_FPGA
    MJC_WRITE_FLD(MJC_SYS_03, 0, MJC_OUT_VSYNC_INV);
    MJC_WRITE_FLD(MJC_FB_10, 0x3, MJC_FILM_BIT_TRUNC);
    #endif

    #if MJC_FPGA
    MJC_WRITE_FLD(MJC_SYS_01, SV_ON, MJC_IN_HSYNC_INV);
    MJC_WRITE_FLD_MULTI(MJC_SYS_02,
        P_Fld(0x2D0, MJC_MJC_HEIGHT_FX) |
        P_Fld(0x500, MJC_MJC_WIDTH));
    MJC_WRITE_FLD_MULTI(MJC_SYS_04,
        P_Fld(0x2D0, MJC_OUT_HEIGHT) |
        P_Fld(0x500, MJC_OUT_WIDTH));
    MJC_WRITE_FLD_MULTI(MJC_SYS_05,
        P_Fld(0x672, MJC_OUT_H_TOTAL) |
        P_Fld(0x2EE, MJC_OUT_V_TOTAL));
    #endif

    x_memset((void *)&arMJCPrm, 0, sizeof(MJC_PRM_T));
    x_memset((void *)&arMJCSPReg, 0, 8 * sizeof(MJC_SP_REG_T));

    arMJCPrm.u1OnOff      = E_MJC_BYPASS;
    arMJCPrm.u1CtrlParam  = MJC_CTRL_LT_ADAPT | MJC_CTRL_LT_ON;
    arMJCPrm.u1CtrlParam |= MJC_CTRL_RT_ADAPT | MJC_CTRL_RT_ON;
    arMJCPrm.u1CtrlParam |= MJC_CTRL_FM_ON;
    arMJCPrm.eMjcEffect   = E_MJC_EFFECT_HIGH;

    #ifdef CC_FPGA
    //arMJCPrm.u1OnOff = E_MJC_ON;
    arMJCPrm.u1EffectParam = 0xff;
    arMJCPrm.u1PSAuto22OnOff = 1;
    arMJCPrm.u1PSAuto32OnOff = 1;
    arMJCPrm.arFRCConfig.u1InFrmRate = 60;
    arMJCPrm.arFRCConfig.u1OutFrmRate = 120;
    #endif

    vDrvMFDInit();
    vDrvMJCInitQuality();
    vDrvMJCFrmSchInit();
    vDrvMJCInitISR();

    vDrvMJCRunMbist();
    if (u4DrvMJCBufferInit() == SV_FAIL)
    {
        return;
    }
    
    #if MJC_FPGA
    MJC_WRITE_FLD(MJC_SYS_01, SV_ON, MJC_IN_YUV2YC_EN);
    MJC_WRITE_FLD(MJC_SYS_03, SV_ON, MJC_OUT_YC2YUV_EN);
    #endif
    _fgIsInitialized = TRUE;
    _vDrvVideoSetMute(MUTE_MODULE_MJC, SV_VP_MAIN, MJC_INIT_MUTE_COUNT, SV_TRUE);;
    x_thread_delay(20);
    MJC_SetOnOff(E_MJC_ON);
    //init TTD
    //vDrvTTDInit();
	//init PR
	vDrvMJCInitPR();
}

void vDrvMJCResetSW(void)
{
}

void vDrvMJCInitISR(void)
{
    static BOOL u1MJCISRInited = SV_FALSE;

    x_os_isr_fct    pfnOldIsr;

    if (!u1MJCISRInited)
    {
        if (x_reg_isr(VECTOR_MJC, vDrvMJCISR, &pfnOldIsr) != OSR_OK)
        {
            LOG(0, "Error: Fail to register MJC ISR!\n");
            return;
        }

        u1MJCISRInited = SV_TRUE;
    }

    LOG(0, "MJC ISR registered!\n");
}

void vDrvMJCInitPR(void)
{
    MJC_WRITE_FLD(MJC_FB_09, 0x2, MJC_DRAMR_444_FRMRST_SEL);
    MJC_WRITE_FLD_MULTI(MJC_PR_01, P_Fld(0x0, MJC_PR_W_VSYNCRST_SEL) | P_Fld(0x7, MJC_PR_R_VSYNCRST_SEL));
	MJC_WRITE_FLD_MULTI(MJC_PR_02, P_Fld(0x8, MJC_PR_WREQ_SIZE_M1) | P_Fld(0xA, MJC_PR_RREQ_SIZE_M1));
	MJC_WRITE_FLD(MJC_PR_00 , ((LVDS_DISP_3D == LVDS_DISP_3D_POLARIZED_RLRL)? 1 : 0), MJC_PR_LR_SWITCH);
	vDrvMJCSetPRModeOnOff(SV_ON);
	if(IS_PANEL_PDP)
	{
        vDrvMJCSetPRMode(E_MJC_HPR , E_MJC_HPR2160);         //set half pr 2160 for pdp panel 
	}
	else
	{
	    if(vDrvGetLCDFreq() < 96)
	    {
	        vDrvMJCSetPRMode(E_MJC_HPR , E_MJC_HPR60);      //set half pr for 60HZ panel 
	    }
	    else
	    {
            if(MJC_HALF_PR_240)
		    {
		        vDrvMJCSetPRMode(E_MJC_HPR , E_MJC_HPR240);  //set half pr 240 for 120HZ panel
		    }
		    else
		    {
		        vDrvMJCSetPRMode(E_MJC_HPR , E_MJC_HPR120);  //set half pr 120 for 120HZ panel
		    }
	    }
	}
}


void vDrvMJCLoadRegTbl(MJC_REGTBL_T * prRegTbl)
{
    while (prRegTbl->u2Addr != (UINT32)MJC_REGTBL_END)
    {
        MJC_WRITE_4BMSK(prRegTbl->u2Addr, prRegTbl->u4Value, prRegTbl->u4Mask);
        prRegTbl++;
    }
}

void vDrvMJCSetClock(void)
{
    #ifndef CC_FPGA
    MJC_WRITE_FLD_MULTI(MJC_SYS_00,
        P_Fld(SV_ON, MJC_CLK_I_EN) |
        P_Fld(SV_ON, MJC_CLK_O_EN) |
        P_Fld(SV_ON, MJC_CLK_M_EN));
    vIO32WriteFldAlign(CKGEN_MJC_CKCFG, 1, FLD_MJC_CK_SEL);
    #endif
}

void vDrvMJCSetCoreClock(BOOL fg216MHz)
{
    /*
    if (fg216MHz)
    {
        vIO32WriteFldMulti(CKGEN_PLLGP_00, P_Fld(0x7, FLD_RG_MJCPLL_FBDIV)|P_Fld(1, FLD_RG_MJCPLL_PWD));
    }
    else
    {
        vIO32WriteFldMulti(CKGEN_PLLGP_00, P_Fld(0xB, FLD_RG_MJCPLL_FBDIV)|P_Fld(1, FLD_RG_MJCPLL_PWD));
    }
    
    vIO32WriteFld(CKGEN_PLLGP_00, 0, FLD_RG_MJCPLL_PWD);   
    vIO32WriteFldAlign(MJC_MISC_00, 1, MJC_FLMR_CHG_TRIG);
    vDrvMJCModeChgDone();
    */
}

void vDrvMJCSetSACtrlMode(UINT8 u1Mode)
{
    switch (u1Mode)
    {
    case 0:
        MJC_WRITE_FLD_MULTI(MJC_SA_00,
            P_Fld(0, MJC_RW_TRANS_PATCH)|
            P_Fld(0, MJC_LADDER_PATCH));
        break;
    case 1:
        MJC_WRITE_FLD_MULTI(MJC_SA_00,
            P_Fld(0, MJC_RW_TRANS_PATCH)|
            P_Fld(1, MJC_LADDER_PATCH));
        break;
    case 2:
        MJC_WRITE_FLD_MULTI(MJC_SA_00,
            P_Fld(1, MJC_RW_TRANS_PATCH)|
            P_Fld(0, MJC_LADDER_PATCH));
        break;
    case 0xff:
    default:
        MJC_WRITE_FLD_MULTI(MJC_SA_00,
            P_Fld(1, MJC_RW_TRANS_PATCH)|
            P_Fld(1, MJC_LADDER_PATCH));
        break;
    }
}

void vDrvMJCSetDRAMAccessOnOff(BOOL fgOnOff, BOOL fgDRAM444)
{
    if (fgOnOff == SV_OFF)
    {
        MJC_WRITE_FLD(MJC_SYS_00, 0, MJC_DRAM_RW_EN);
		MJC_WRITE_FLD(MJC_MV_TBA_5, 0, MJC_MV_TBA_WT_EN);
    }
    else
    {
        MJC_WRITE_FLD(MJC_SYS_00, (fgDRAM444 ? 0x3 : 0x1F), MJC_DRAM_RW_EN);
		if(BSP_GetIcVersion() == IC_VER_5396_AA)
		{
		    MJC_WRITE_FLD(MJC_CA_M2T_5, 0x0, MJC_MV_TBA_WT_EN_ES);
		}
		else
		{
            MJC_WRITE_FLD(MJC_MV_TBA_5, (fgDRAM444 == SV_FALSE), MJC_MV_TBA_WT_EN); 
		}
    }
}

void vDrvMJCSetBuf(const MJC_DRAM_ALLOC_T * prDramAlloc)
{
    UINT32 fgDram444, fgDram420, fgY8, fgC8;
    UINT16 u2Mode = prDramAlloc->u2Mode;
    UINT32 u4BaseAddr, u4FrameDramSize,  u4OSDDramSize, u4MVDramSize, u4TBADramSize, u4PRDramSize;

    #if MJC_SET_BUF_FULL
    u2Mode = MJC_Y10_C10_422_0RDC_3FRM;
    #endif

    fgDram444 = (u2Mode & MJC_444_MODE) ? 1 : 0;
    fgDram420 = (fgDram444) ? 0 : ((u2Mode & MJC_420_MODE) ? 1 : 0);
    fgY8      = (u2Mode & MJC_Y08_MODE) ? 1 : 0;
    fgC8      = (fgDram444) ? fgY8 : ((u2Mode & MJC_C08_MODE) ? 1 : 0);

    u4BaseAddr      = prDramAlloc->u4StartAddr >> 4;
    u4FrameDramSize = u4DrvMJCCalcSizeYC(prDramAlloc->u4Width, prDramAlloc->u4Height, fgDram444, fgDram420, fgY8, fgC8);
    u4OSDDramSize   = u4DrvMJCCalcSizeOSD(prDramAlloc->u4Width, prDramAlloc->u4Height, fgDram444);
    u4MVDramSize    = u4DrvMJCCalcSizeMV(prDramAlloc->u4Width, prDramAlloc->u4Height, fgDram444);
	u4TBADramSize   = u4DrvMJCCalcSizeTBA(prDramAlloc->u4Width, prDramAlloc->u4Height, fgDram444);
	u4PRDramSize    = u4DrvMJCCalcSizePR(prDramAlloc->u4Width, prDramAlloc->u4Height, fgDram444, fgDram420, fgY8, fgC8);

    MJC_WRITE_FLD(MJC_FB_08, u4BaseAddr, MJC_DRAM_BASE_ADDR);
    MJC_WRITE_FLD(MJC_FB_0E, (u4BaseAddr >> 24), MJC_DRAM_BASE_ADDR_MSB);
	MJC_WRITE_FLD(MJC_PR_03, (u4BaseAddr >> 24), MJC_PR_ADDR_MSB);
    MJC_WRITE_FLD(MJC_FB_00, u4BaseAddr, MJC_WADDR_LO_LIMIT);

    u4BaseAddr += (u4FrameDramSize >> 4);
    MJC_WRITE_FLD(MJC_MV_00, u4BaseAddr, MJC_TD_START_ADDR_L);
	u4BaseAddr += (u4OSDDramSize >> 4);
    MJC_WRITE_FLD(MJC_MV_01, u4BaseAddr, MJC_TD_END_ADDR_L);
    MJC_WRITE_FLD(MJC_MV_3DR2, u4BaseAddr, MJC_TD_START_ADDR_R);
	u4BaseAddr += (u4OSDDramSize >> 4);
    MJC_WRITE_FLD(MJC_MV_3DR3, u4BaseAddr, MJC_TD_END_ADDR_R);
    MJC_WRITE_FLD(MJC_MV_0E, u4BaseAddr, MJC_MV_START_ADDR_L);
    u4BaseAddr += (u4MVDramSize >> 4);
    MJC_WRITE_FLD(MJC_MV_0F, u4BaseAddr, MJC_MV_END_ADDR_L);
    MJC_WRITE_FLD(MJC_MV_3DR0, u4BaseAddr, MJC_MV_START_ADDR_R);
    u4BaseAddr += (u4MVDramSize >> 4);
    MJC_WRITE_FLD(MJC_MV_3DR1, u4BaseAddr, MJC_MV_END_ADDR_R);
	MJC_WRITE_FLD(MJC_MV_TBA_5, u4BaseAddr, MJC_MV_TBA_STR_ADR_L);
	u4BaseAddr += (u4TBADramSize >> 4);
	MJC_WRITE_FLD(MJC_MV_TBA_6, u4BaseAddr, MJC_MV_TBA_END_ADR_L);
	MJC_WRITE_FLD(MJC_MV_TBA_A, u4BaseAddr, MJC_MV_TBA_STR_ADR_R);
	u4BaseAddr += (u4TBADramSize >> 4);
	MJC_WRITE_FLD(MJC_MV_TBA_B, u4BaseAddr, MJC_MV_TBA_END_ADR_R);
	MJC_WRITE_FLD(MJC_PR_00, u4BaseAddr, MJC_PR_BASE_ADDR);
	MJC_WRITE_FLD(MJC_PR_02, u4BaseAddr, MJC_PR_WADDR_LO_LIMIT);
	u4BaseAddr += (u4PRDramSize >> 4);
	MJC_WRITE_FLD(MJC_PR_01, u4BaseAddr, MJC_PR_WADDR_HI_LIMIT);
    MJC_WRITE_FLD(MJC_FB_01, u4BaseAddr, MJC_WADDR_HI_LIMIT);

    if ((u4BaseAddr << 4) > prDramAlloc->u4EndAddr)
    {
        LOG(0, "vDrvMJCSetBuf: SRM MJC DRAM allocation error\n");
        //ASSERT(0);
    }
}

void vDrvMJCSetSystemMode(UINT32 u4MjcSystemMode)
{
    UINT8 u1Dram444, u1Dram420, u1Dram422RPT, u1Y8, u1C8, u1TwoFrm, u1OneFrm, u1Rdc, u1Rscn, u1Dyn, u1Rpt;

	u1Dram422RPT = (u4MjcSystemMode & MJC_422RPT_MODE) ? 1 : 0;
    u1Dram444 = (u4MjcSystemMode & MJC_444_MODE) ? 1 : 0;
    u1Dram420 = (u4MjcSystemMode & MJC_420_MODE) ? 1 : 0;
    u1Dyn     = (u4MjcSystemMode & MJC_DYN_MODE) ? 1 : 0;
    u1Y8      = (u4MjcSystemMode & MJC_Y08_MODE) ? 1 : 0;
    u1C8      = (u4MjcSystemMode & MJC_C08_MODE) ? 1 : 0;
    u1TwoFrm  = (u4MjcSystemMode & MJC_2FRM_MODE) ? 1 : 0;
    u1OneFrm  = (u4MjcSystemMode & MJC_1FRM_MODE) ? 1 : 0;
    u1Rdc     = (u4MjcSystemMode & MJC_1RDC_MODE) ? (!u1OneFrm) : 0;
    u1Rscn    = 1; //(u4MjcSystemMode & MJC_RSCN_MODE) ? 1 : 0;
    u1Rpt     = (u4MjcSystemMode & MJC_RPT_MODE) ? (!u1Dram420) : 0;

    if ((arMJCPrm.u1Dram444 != u1Dram444) || (arMJCPrm.u1Dram422RPT != u1Dram422RPT))
    {
        vDrvMJCSetPSAutoOnOff(SV_OFF);
        if ((u1Dram444 == SV_FALSE) && (u1Dram422RPT == SV_FALSE) && (u1DrvMJCIsOnOff() == SV_TRUE))
        {
            vDrvMJCSetPsAutoTrigOn();
        }
    }

    //Keep information
    arMJCPrm.u1Dram444 = u1Dram444;
	arMJCPrm.u1Dram422RPT = u1Dram422RPT;
    arMJCPrm.u1Y8 = u1Y8;
    arMJCPrm.u1YDithEn = u1Y8;
    arMJCPrm.u1TwoFrm = u1TwoFrm;
    arMJCPrm.u1OneFrm = u1OneFrm;
    arMJCPrm.u1Rdc = u1Rdc;
    arMJCPrm.u1Dyn = u1Dyn;
    arMJCPrm.u1Rscn = u1Rscn;

    // dyn
    MJC_WRITE_FLD(MJC_FB_0C, u1Dyn, MJC_DRAM_DYNAMIC_SWITCH);
    // rescan
    MJC_WRITE_FLD(MJC_SA_00, u1Rscn, MJC_RESCAN_EN);
	// PR_10B When mjc is Y10C10
    MJC_WRITE_FLD(MJC_PR_03, ((u1Y8 && u1C8) ? 0 : 1) , MJC_PR_10B); 
	// MJC_DRAMW_REQ_SIZE_M1 setting
	MJC_WRITE_FLD(MJC_FB_0C, ((u1Y8 && u1C8) ? 31 : 24) , MJC_DRAMW_REQ_SIZE_M1); 
    
    if ((u1Dram444) || (u1Dram422RPT))
    {
        if((IO32ReadFldAlign(MJC_PR_MODE, MJC_3DMODE_IN) == E_VDO_MJC_2D) && (u1Dram444))
        {
            // 444->422 Off
            MJC_WRITE_FLD_MULTI(MJC_SYS_01,
                P_Fld(0, MJC_IN_LINE_EXT)|
                P_Fld(0, MJC_IN_LPF_EN)|
                P_Fld(0, MJC_IN_YUV2YC_EN));
        
            // 422->444 Off
            // DRAM-444 Mirror Chroma delay 0T
            MJC_WRITE_FLD_MULTI(MJC_SYS_03,
                P_Fld(0, MJC_OUT_CHROMADLY_EN) |
                P_Fld(0, MJC_OUT_YC2YUV_EN));
		    MJC_WRITE_FLD_MULTI(MJC_FB_01,
                P_Fld(1, MJC_DRAM_RPT)|P_Fld(0, MJC_DRAM_RPT_422)|P_Fld(0, MJC_FB_TWO_FRM_EN));
        
            // Processing Color Space is RGB444
            // Disable MV dram access for repeat mode
            MJC_WRITE_FLD_MULTI(MJC_SYS_00, 
                P_Fld(1, MJC_BY_IN_TRANSFORM)|
                P_Fld(1, MJC_BY_OUT_TRANSFORM));  // DRAM access should be enabled after address change.
        }
		else
		{
            // 444->422 On
            MJC_WRITE_FLD_MULTI(MJC_SYS_01,
                P_Fld(1, MJC_IN_LINE_EXT)|
                P_Fld(1, MJC_IN_LPF_EN)|
                P_Fld(1, MJC_IN_YUV2YC_EN));
        
            // 422->444 On
            // DRAM-42x Mirror Chroma delay 1T (if Mirror On)
            MJC_WRITE_FLD_MULTI(MJC_SYS_03,
                P_Fld(arMJCPrm.u1Mirror, MJC_OUT_CHROMADLY_EN) |
                P_Fld(1, MJC_OUT_YC2YUV_EN));
		    MJC_WRITE_FLD_MULTI(MJC_FB_01,
                P_Fld(1, MJC_DRAM_RPT)|P_Fld(1, MJC_DRAM_RPT_422)|P_Fld(0, MJC_FB_TWO_FRM_EN));
        
            // Processing Color Space is RGB444
            // Disable MV dram access for repeat mode
            MJC_WRITE_FLD_MULTI(MJC_SYS_00, 
                P_Fld(0, MJC_BY_IN_TRANSFORM)|
                P_Fld(0, MJC_BY_OUT_TRANSFORM));  // DRAM access should be enabled after address change.
		}
        // Y8/Y_Dither
        MJC_WRITE_FLD_MULTI(MJC_FB_00,
            P_Fld(u1Y8, MJC_DRAM_Y_8BIT)|
            P_Fld(u1Y8, MJC_DRAM_C_8BIT)|   // DRAM-444 C8: determined by u1Y8
            P_Fld(0, MJC_DRAM_420));        // DRAM 444 frame repeat
        MJC_WRITE_FLD_MULTI(MJC_SYS_D9,    //?
            P_Fld(u1Y8, MJC_DITH_SUB_Y)|     // Y8/Y_Dither
            P_Fld(u1Y8, MJC_DITH_SUB_U)|
            P_Fld(u1Y8, MJC_DITH_SUB_V));   // UV_Dither : determined by u1Y8
       
        // DRAM 444 frame repeat
        MJC_WRITE_FLD_MULTI(MJC_FB_0A,
            P_Fld(0, MJC_DRAM_420_AVG_EN)|P_Fld(0, MJC_BANDWIDTH_REDUCTION_EN));
        

        //vDrvMJCSetFBOneFrmOnOff(SV_OFF);

        //Keep information
        arMJCPrm.u1Dram420 = 0;
        arMJCPrm.u1C8 = u1Y8;
        arMJCPrm.u1CDithEn = u1Y8;
        arMJCPrm.u1Rpt = 1;

        // DRAM read/write burst setting depend on color mode and SYSTEM_RESOURCE_CONFIG
    #if 0//(SYSTEM_RESOURCE_CONFIG == DDR2X1_1066_4L_PCB)
        MJC_WRITE_FLD(MJC_FB_03, 0x03, MJC_DRAM_RBURST_SIZE);
        MJC_WRITE_FLD(MJC_FB_03, 0x19, MJC_DRAM_REQ_MAX_SIZE);
        MJC_WRITE_FLD(MJC_FB_0C, 0x0A, MJC_DRAM_LAST_AUX_CNTR_MAX);
    #endif      

    }
    else
    {
        // 444->422 On
        MJC_WRITE_FLD_MULTI(MJC_SYS_01,
            P_Fld(1, MJC_IN_LINE_EXT)|
            P_Fld(1, MJC_IN_LPF_EN)|
            P_Fld(1, MJC_IN_YUV2YC_EN));
        
        // 422->444 On
        // DRAM-42x Mirror Chroma delay 1T (if Mirror On)
        MJC_WRITE_FLD_MULTI(MJC_SYS_03,
            P_Fld(arMJCPrm.u1Mirror, MJC_OUT_CHROMADLY_EN) |
            P_Fld(1, MJC_OUT_YC2YUV_EN));
        
        // Y8/Y_Dither
        MJC_WRITE_FLD_MULTI(MJC_FB_00,
            P_Fld(u1Y8, MJC_DRAM_Y_8BIT)|
            P_Fld(u1C8, MJC_DRAM_C_8BIT)|       // DRAM-42x C8: determined by u1C8
            P_Fld(u1Dram420, MJC_DRAM_420));    // DRAM 42x MEMC

		MJC_WRITE_FLD_MULTI(MJC_FB_0D,
            P_Fld(u1Y8, MJC_DITH_SUB_Y)|     // Y8/Y_Dither
            P_Fld(u1C8, MJC_DITH_SUB_U)|
            P_Fld(u1C8, MJC_DITH_SUB_V));   // UV_Dither : determined by u1C8
        
        // DRAM 42x MEMC
        MJC_WRITE_FLD_MULTI(MJC_FB_0A,
            P_Fld(u1Dram420, MJC_DRAM_420_AVG_EN)|P_Fld(u1Rdc, MJC_BANDWIDTH_REDUCTION_EN));
        MJC_WRITE_FLD_MULTI(MJC_FB_01,
            P_Fld(u1Rpt, MJC_DRAM_RPT)|P_Fld(u1Rpt, MJC_DRAM_RPT_422)|P_Fld(u1TwoFrm, MJC_FB_TWO_FRM_EN));
        
        // Processing Color Space is YUV422
        // Enable MV dram access for repeat mode
        MJC_WRITE_FLD_MULTI(MJC_SYS_00, 
            P_Fld(0, MJC_BY_IN_TRANSFORM)|
            P_Fld(0, MJC_BY_OUT_TRANSFORM)); // DRAM access should be enabled after address change.

        vDrvMJCSetFBOneFrmOnOff(u1OneFrm);

        //Keep information
        arMJCPrm.u1Dram420 = u1Dram420;
        arMJCPrm.u1C8 = u1C8;
        arMJCPrm.u1CDithEn = u1C8;
        arMJCPrm.u1Rpt = u1Rpt;
    
        // DRAM read/write burst setting depend on color mode and SYSTEM_RESOURCE_CONFIG
    #if 0//(SYSTEM_RESOURCE_CONFIG == DDR2X1_1066_4L_PCB)
        MJC_WRITE_FLD(MJC_FB_03, 0x02, MJC_DRAM_RBURST_SIZE);
        MJC_WRITE_FLD(MJC_FB_03, 0x0F, MJC_DRAM_REQ_MAX_SIZE);
        MJC_WRITE_FLD(MJC_FB_0C, 0x01, MJC_DRAM_LAST_AUX_CNTR_MAX);
    #endif      

    }
}

void vDrvMJCSetFlipMirror(UINT32 u4FlipOnOff, UINT32 u4MirrorOnOff)
{
    arMJCPrm.u1Flip = u4FlipOnOff;
    arMJCPrm.u1Mirror = u4MirrorOnOff;

    // Flip/Mirror
    MJC_WRITE_FLD(MJC_FB_0A, (u4FlipOnOff << 1) | (u4MirrorOnOff), MJC_FLIPMRR);

    // If Mirror On & DRAM-42x : Chroma delay 1T (if Mirror On)
    if (arMJCPrm.u1Dram444)
    {
        MJC_WRITE_FLD(MJC_SYS_03, 0, MJC_OUT_CHROMADLY_EN);
    }
    else
    {
        MJC_WRITE_FLD(MJC_SYS_03, u4MirrorOnOff, MJC_OUT_CHROMADLY_EN);
    }

	if((arMJCPrm.arPicSize.u2OutHeight == 544) && ((arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_SBS )||(arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_FS)))
	{
	    if(arMJCPrm.u1Flip == 1)
	    {
	        MJC_WRITE_FLD(MJC_SYS_03,arMJCPrm.arPicSize.u2OutVStart+4, MJC_OUT_V_START);
	    }
		else
	    {
	        MJC_WRITE_FLD(MJC_SYS_03,arMJCPrm.arPicSize.u2OutVStart, MJC_OUT_V_START);
	    }
	}
}

static void vDrvMJCSetProgFrmRate(UINT8 u1InFrmRate, UINT8 u1OutFrmRate)
{
    UINT8 bFrmMultiple;
    UINT8 bDivBy2 = 0;

    if (u1InFrmRate == 0)
    {
        LOG(3, "vDrvMJCSetProgFrmRate Failed: Invalid Input Frame Rate.\n");
        return;
    }

    bFrmMultiple = u1OutFrmRate / u1InFrmRate;
    
    if (u1OutFrmRate != (u1InFrmRate * bFrmMultiple))
    {
        bDivBy2 = 1;
        bFrmMultiple = (bFrmMultiple << 1) + 1;
    }

    MJC_WRITE_FLD_MULTI(MJC_SYS_04,
        P_Fld(bDivBy2, MJC_OUT_VSYNC_DIV) |
        P_Fld(bFrmMultiple, MJC_OUT_FRM_NO));
}

static void vDrvMJCSetPsAutoOnProc(void)
{
    UINT32 fgUIFmOn;
    
    if (_u1TrigSetPsAuto &&
        (MJC_READ_FLD(MJC_STA_FM_02, MJC_STA_ME_FILM_FLG_HW) == 0))
    {
        _u1TrigSetPsAuto = 0;

        fgUIFmOn = (arMJCPrm.u1CtrlParam & MJC_CTRL_FM_ON) ? SV_ON : SV_OFF;
        MJC_WRITE_FLD_MULTI(MJC_FM_00,
            P_Fld((arMJCPrm.u1PSAuto32OnOff && fgUIFmOn), MJC_PSAUTO_32)|
            P_Fld((arMJCPrm.u1PSAuto22OnOff && fgUIFmOn), MJC_PSAUTO_22));
    }
}

void vDrvMJCSetPSAutoOnOff(BOOL fgOnOff)
{
    if ((fgOnOff == SV_OFF) || (arMJCPrm.u1OneFrm))
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_00,
            P_Fld(SV_OFF, MJC_PSAUTO_32) |
            P_Fld(SV_OFF, MJC_PSAUTO_22));
    }
    else
    {
        _u1TrigSetPsAuto = 1;
    }
}

void vDrvMJCSet32PdInfoShift(UINT32 fgIsInterlace, UINT32 u4PSCANDisplay, UINT32 fgNRSDMode)
{
    UINT32 u4PdInfo32Shift = 0;

    arMJCPrm.fgIsInterlace = fgIsInterlace;
    arMJCPrm.fgPSCANDispMode = u4PSCANDisplay;

    if (fgIsInterlace == SV_TRUE)
    {
        u4PdInfo32Shift = (fgNRSDMode == TRUE) ? 0 : 1;
        MJC_WRITE_FLD(MJC_SYS_00, u4PdInfo32Shift, MJC_PD_INFO_32_SHIFT);
        vDrvMJCFrmSchSetPdInfoDly(4);
    }
    else
    {
        if (arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_FS)
        {
            if (BSP_GetIcVersion() == IC_VER_5396_AA)
            {
                MJC_WRITE_FLD(MJC_SYS_01, SV_ON, MJC_IN_TEST_V_DLY);
            }
            else
            {
                MJC_WRITE_FLD(MJC_SYS_00, SV_ON, MJC_PD_INFO_DLY);
            }

            u4PdInfo32Shift = 1;
            MJC_WRITE_FLD(MJC_SYS_00, u4PdInfo32Shift, MJC_PD_INFO_32_SHIFT);
            vDrvMJCFrmSchSetPdInfoDly(1);
        }
        else
        {
            u4PdInfo32Shift = 2;
            MJC_WRITE_FLD(MJC_SYS_01, SV_OFF, MJC_IN_TEST_V_DLY);
            MJC_WRITE_FLD(MJC_SYS_00, SV_OFF, MJC_PD_INFO_DLY);
            MJC_WRITE_FLD(MJC_SYS_00, u4PdInfo32Shift, MJC_PD_INFO_32_SHIFT);
            vDrvMJCFrmSchSetPdInfoDly(0);
        }
    }
}

static void _vDrvMJCSetProgME_Full(BOOL fgIs55p2)
{
    if (fgIs55p2 == 0)
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_02, MJC_PROG_ME2_04_FULL|MJC_PROG_ME2_08_FULL|MJC_PROG_ME2_10_FULL|MJC_PROG_ME2_L_20_FULL);
        MJC_WRITE_FLD_MULTI(MJC_FM_03, MJC_PROG_ME2_H_20_FULL|MJC_PROG_ME3_04_FULL|MJC_PROG_ME3_08_FULL|MJC_PROG_ME3_L_20_FULL);
        MJC_WRITE_FLD_MULTI(MJC_FM_04, MJC_PROG_ME3_H_20_FULL|MJC_PROG_ME3_10_FULL);

        MJC_WRITE_4B(MJC_FM_10,
            ((MJC_PROG_POS_10_FULL & 0xFF) << 24)|(MJC_PROG_POS_08_FULL << 8)|MJC_PROG_POS_04_FULL);
        MJC_WRITE_4B(MJC_FM_11, (MJC_PROG_POS_L_20_FULL << 12)|(MJC_PROG_POS_10_FULL >> 8));
        MJC_WRITE_4B(MJC_FM_12, MJC_PROG_POS_H_20_FULL);
    }
    else
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_02, MJC_PROG_ME2_04_FULL|MJC_PROG_ME2_08_FULL|MJC_PROG_ME2_10_FULL|MJC_PROG_ME2_L_20_FULL);
        MJC_WRITE_FLD_MULTI(MJC_FM_03, MJC_PROG_ME2_H_20_FULL|MJC_PROG_ME3_04_FULL|MJC_PROG_ME3_08_FULL|MJC_PROG_ME3_55P2_L_20_FULL);
        MJC_WRITE_FLD_MULTI(MJC_FM_04, MJC_PROG_ME3_H_20_FULL|MJC_PROG_ME3_10_FULL);

        MJC_WRITE_4B(MJC_FM_10,
            ((MJC_PROG_POS_10_FULL & 0xFF) << 24)|(MJC_PROG_POS_08_FULL << 8)|MJC_PROG_POS_04_FULL);
        MJC_WRITE_4B(MJC_FM_11, (MJC_PROG_POS_55P2_L_20_FULL << 12)|(MJC_PROG_POS_10_FULL >> 8));
        MJC_WRITE_4B(MJC_FM_12, MJC_PROG_POS_H_20_FULL);
    }
}

static void _vDrvMJCSetProgME_Half(BOOL fgIs55p2)
{
    if (fgIs55p2 == 0)
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_02, MJC_PROG_ME2_04_HALF|MJC_PROG_ME2_08_HALF|MJC_PROG_ME2_10_HALF|MJC_PROG_ME2_L_20_HALF);
        MJC_WRITE_FLD_MULTI(MJC_FM_03, MJC_PROG_ME2_H_20_HALF|MJC_PROG_ME3_04_HALF|MJC_PROG_ME3_08_HALF|MJC_PROG_ME3_L_20_HALF);
        MJC_WRITE_FLD_MULTI(MJC_FM_04, MJC_PROG_ME3_H_20_HALF|MJC_PROG_ME3_10_HALF);

        MJC_WRITE_4B(MJC_FM_10,
            ((MJC_PROG_POS_10_HALF & 0xFF) << 24)|(MJC_PROG_POS_08_HALF << 8)|MJC_PROG_POS_04_HALF);
        MJC_WRITE_4B(MJC_FM_11, (MJC_PROG_POS_L_20_HALF << 12)|(MJC_PROG_POS_10_HALF >> 8));
        MJC_WRITE_4B(MJC_FM_12, MJC_PROG_POS_H_20_HALF);
    }
    else
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_02, MJC_PROG_ME2_04_HALF|MJC_PROG_ME2_08_HALF|MJC_PROG_ME2_10_HALF|MJC_PROG_ME2_55P2_L_20_HALF);
        MJC_WRITE_FLD_MULTI(MJC_FM_03, MJC_PROG_ME2_H_20_HALF|MJC_PROG_ME3_04_HALF|MJC_PROG_ME3_08_HALF|MJC_PROG_ME3_55P2_L_20_HALF);
        MJC_WRITE_FLD_MULTI(MJC_FM_04, MJC_PROG_ME3_H_20_HALF|MJC_PROG_ME3_10_HALF);

        MJC_WRITE_4B(MJC_FM_10,
            ((MJC_PROG_POS_10_HALF & 0xFF) << 24)|(MJC_PROG_POS_08_HALF << 8)|MJC_PROG_POS_04_HALF);
        MJC_WRITE_4B(MJC_FM_11, (MJC_PROG_POS_55P2_L_20_HALF << 12)|(MJC_PROG_POS_10_HALF >> 8));
        MJC_WRITE_4B(MJC_FM_12, MJC_PROG_POS_H_20_HALF);
    }
}

static void _vDrvMJCSetProgME_SingleME(BOOL fgIs55p2)
{
    if (fgIs55p2 == 0)
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_02, MJC_PROG_ME2_04_SGME|MJC_PROG_ME2_08_SGME|MJC_PROG_ME2_10_SGME|MJC_PROG_ME2_L_20_SGME);
        MJC_WRITE_FLD_MULTI(MJC_FM_03, MJC_PROG_ME2_H_20_SGME|MJC_PROG_ME3_04_SGME|MJC_PROG_ME3_08_SGME|MJC_PROG_ME3_L_20_SGME);
        MJC_WRITE_FLD_MULTI(MJC_FM_04, MJC_PROG_ME3_H_20_SGME|MJC_PROG_ME3_10_SGME);

        MJC_WRITE_4B(MJC_FM_10,
            ((MJC_PROG_POS_10_SGME & 0xFF) << 24)|(MJC_PROG_POS_08_SGME << 8)|MJC_PROG_POS_04_SGME);
        MJC_WRITE_4B(MJC_FM_11, (MJC_PROG_POS_L_20_SGME << 12)|(MJC_PROG_POS_10_SGME >> 8));
        MJC_WRITE_4B(MJC_FM_12, MJC_PROG_POS_H_20_SGME);
    }
    else
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_02, MJC_PROG_ME2_04_SGME|MJC_PROG_ME2_08_SGME|MJC_PROG_ME2_10_SGME|MJC_PROG_ME2_55P2_L_20_SGME);
        MJC_WRITE_FLD_MULTI(MJC_FM_03, MJC_PROG_ME2_H_20_SGME|MJC_PROG_ME3_04_SGME|MJC_PROG_ME3_08_SGME|MJC_PROG_ME3_55P2_L_20_SGME);
        MJC_WRITE_FLD_MULTI(MJC_FM_04, MJC_PROG_ME3_H_20_SGME|MJC_PROG_ME3_10_SGME);
        
        MJC_WRITE_4B(MJC_FM_10,
            ((MJC_PROG_POS_10_SGME & 0xFF) << 24)|(MJC_PROG_POS_08_SGME << 8)|MJC_PROG_POS_04_SGME);
        MJC_WRITE_4B(MJC_FM_11, (MJC_PROG_POS_55P2_L_20_SGME << 12)|(MJC_PROG_POS_10_SGME >> 8));
        MJC_WRITE_4B(MJC_FM_12, MJC_PROG_POS_H_20_SGME);
    }
}

static void _vDrvMJCSetProgME_240(BOOL fgIs55p2)
{
    MJC_WRITE_FLD_MULTI(MJC_FM_02, MJC_PROG_ME2_04_240|MJC_PROG_ME2_08_240|MJC_PROG_ME2_10_240|MJC_PROG_ME2_L_20_240);
    MJC_WRITE_FLD_MULTI(MJC_FM_03, MJC_PROG_ME2_H_20_240|MJC_PROG_ME3_04_240|MJC_PROG_ME3_08_240|MJC_PROG_ME3_L_20_240);
    MJC_WRITE_FLD_MULTI(MJC_FM_04, MJC_PROG_ME3_H_20_240|MJC_PROG_ME3_10_240);

    if (fgIs55p2 == 0)
    {
        MJC_WRITE_4B(MJC_FM_10,
            ((MJC_PROG_POS_10_240 & 0xFF) << 24)|(MJC_PROG_POS_08_240 << 8)|MJC_PROG_POS_04_240);
        MJC_WRITE_4B(MJC_FM_11, (MJC_PROG_POS_L_20_240 << 12)|(MJC_PROG_POS_10_240 >> 8));
        MJC_WRITE_4B(MJC_FM_12, MJC_PROG_POS_H_20_240);
    }
    else
    {
        MJC_WRITE_4B(MJC_FM_10,
            ((MJC_PROG_POS_10_240 & 0xFF) << 24)|(MJC_PROG_POS_08_240 << 8)|MJC_PROG_POS_04_240);
        MJC_WRITE_4B(MJC_FM_11, (MJC_PROG_POS_55P2_L_20_240 << 12)|(MJC_PROG_POS_10_240 >> 8));
        MJC_WRITE_4B(MJC_FM_12, MJC_PROG_POS_55P2_H_20_240);
    }
}

void vDrvMJCSetProgME(BOOL fgIs120Hz, UINT8 fgIs55p2)
{
    UINT8 u1PRMode = u1DrvMJCGetPRMode();
    UINT8 u1ProgMEMode = IO32ReadFldAlign(MJC_MISC_00, MJC_PROG_ME_MODE);
    BOOL fgPPCIntlv = SV_TRUE;
    BOOL fgFWMvExpEn = SV_FALSE;
    
    if (fgIs120Hz)
    {
        switch (u1ProgMEMode)
        {
        default:
        case 0: // half run 324MHz (5395)
            _vDrvMJCSetProgME_Half(fgIs55p2);
            fgPPCIntlv = SV_FALSE;
            break;
        case 2: // full run 324MHz
            _vDrvMJCSetProgME_Full(fgIs55p2);
            fgPPCIntlv = SV_FALSE;
            break;
        case 1: // single ME mode
            _vDrvMJCSetProgME_SingleME(fgIs55p2);
            fgFWMvExpEn = SV_TRUE;
            break;
        }
    }
    else
    {   // 240Hz
        if (u1PRMode == E_MJC_HPR)
        {   // single ME mode for half pr 240HZ (120Hz MEMC)
            _vDrvMJCSetProgME_SingleME(fgIs55p2);
            fgFWMvExpEn = SV_TRUE;
        }
        else
        {   // 240Hz MEMC
            _vDrvMJCSetProgME_240(fgIs55p2);
        }
    }

    vIO32WriteFldAlign(MJC_MISC_01, fgFWMvExpEn, MJC_FW_MVEXP_EN);
    
	MJC_WRITE_FLD_MULTI(MJC_FM_00, 
        P_Fld((((u1PRMode == E_MJC_HPR) && (arMJCPrm.arFRCConfig.u1OutFrmRate == 240))? 1 : 0), MJC_ME_PRLST_PREBI) |
        P_Fld(fgPPCIntlv, MJC_PPC_FORCE_INTLV) | P_Fld(fgPPCIntlv, MJC_PPC_VIDEO_OFF));
}

void vDrvMJCSetRdcType(BOOL fgDft)
{
    if (fgDft == SV_FALSE)
    {
        MJC_WRITE_4B(MJC_FM_08, 0);
        MJC_WRITE_4B(MJC_FM_13, 0);
        MJC_WRITE_4BMSK(MJC_FM_14, 0, 0xFFFFF);
        MJC_WRITE_FLD_MULTI(MJC_MC_01, P_Fld(0, MJC_MC_TRUE_RPT_MAN_PV)|P_Fld(0, MJC_MC_TRUE_RPT_MAN_CU));
        return;
    }

    MJC_WRITE_FLD_MULTI(MJC_MC_01, P_Fld(1, MJC_MC_TRUE_RPT_MAN_PV)|P_Fld(1, MJC_MC_TRUE_RPT_MAN_CU));
        
    if (arMJCPrm.arFRCConfig.u1OutFrmRate < 192)
    {
        MJC_WRITE_4B(MJC_FM_08,
            (((UINT32)MJC_PROG_BWRDC_N_10_120 & 0xFF) << 24) |
            (((UINT32)MJC_PROG_BWRDC_N_08_120) << 8) |
            (((UINT32)MJC_PROG_BWRDC_N_04_120) << 0));

        if (arMJCPrm.arFRCConfig.u1FrmSchMode != FALLBACK_MODE_55P2)
        {
            MJC_WRITE_4B(MJC_FM_13,
                (((UINT32)MJC_PROG_BWRDC_N_20_120 & 0xFFFFF) << 12) |
                (((UINT32)MJC_PROG_BWRDC_N_10_120 >> 8) << 0));
            MJC_WRITE_4B(MJC_FM_14,
                (((UINT32)MJC_PROG_BWRDC_N_20_120 >> 20) << 0));
        }
        else
        {
           MJC_WRITE_4B(MJC_FM_13,
                (((UINT32)MJC_PROG_BWRDC_55P2_N_20_120 & 0xFFFFF) << 12) |
                (((UINT32)MJC_PROG_BWRDC_N_10_120 >> 8) << 0));
           MJC_WRITE_4B(MJC_FM_14,
                (((UINT32)MJC_PROG_BWRDC_55P2_N_20_120 >> 20) << 0));
        }
    }
    else
    {
	    MJC_WRITE_4B(MJC_FM_08,
			(((UINT32)MJC_PROG_BWRDC_N_10_240 & 0xFF) << 24) |
			(((UINT32)MJC_PROG_BWRDC_N_08_240) << 8) |
			(((UINT32)MJC_PROG_BWRDC_N_04_240) << 0));

		if(arMJCPrm.arFRCConfig.u1FrmSchMode != FALLBACK_MODE_55P2)
		{
		    MJC_WRITE_4B(MJC_FM_13,
			    (((UINT32)MJC_PROG_BWRDC_N_20_240 & 0xFFFFF) << 12) |
		        (((UINT32)MJC_PROG_BWRDC_N_10_240 >> 8) << 0));
		    MJC_WRITE_4B(MJC_FM_14,
			    (((UINT32)MJC_PROG_BWRDC_N_20_240 >> 20) << 0));
		}
		else
        {
            MJC_WRITE_4B(MJC_FM_13,
			    (((UINT32)MJC_PROG_BWRDC_55P2_N_20_240 & 0xFFFFF) << 12) |
		        (((UINT32)MJC_PROG_BWRDC_N_10_240 >> 8) << 0));
		    MJC_WRITE_4B(MJC_FM_14,
			    (((UINT32)MJC_PROG_BWRDC_55P2_N_20_240 >> 20) << 0));
		}
    }
}

void vDrvCliCallPME(BOOL fgIs120Hz, UINT8 fgIs55p2)
{
    vDrvMJCSetProgME(fgIs120Hz, fgIs55p2);
}

void vDrvMJCSetDbgInk(UINT8 u1INKIdex)
{
    if(u1INKIdex == 1)
    {
        MJC_WRITE_FLD(MJC_MC_01 , 1, MJC_MC_DBG_EN);
	}
	else if(u1INKIdex == 2)
	{
        vRegWriteFldAlign(MCVP_KC_21, 9, OSD_MODE);
		vRegWriteFldAlign(MCVP_KC_21, 8, OSD_Y_POS);
		vRegWriteFldAlign(MCVP_KC_21, 8, OSD_X_POS);
	}
	else if(u1INKIdex == 3)
	{
        MJC_WRITE_FLD(MJC_MC_01 , 1, MJC_APL_DBG_EN);
	}
    else if(u1INKIdex == 4)
	{
        MJC_WRITE_FLD(MJC_MC_01 , 1, MJC_SCN_DBG_EN);
	}
	else
	{
        MJC_WRITE_FLD(MJC_MC_01 , 0, MJC_MC_DBG_EN);
        vRegWriteFldAlign(MCVP_KC_21, 0, OSD_MODE);
		vRegWriteFldAlign(MCVP_KC_21, 0, OSD_Y_POS);
		vRegWriteFldAlign(MCVP_KC_21, 0, OSD_X_POS);
		MJC_WRITE_FLD(MJC_MC_01 , 0, MJC_APL_DBG_EN);
		MJC_WRITE_FLD(MJC_MC_01 , 0, MJC_SCN_DBG_EN);
	}

}


void vDrvMJCSetFrmSch(MJC_FRC_CONFIG_T * arMJCFRC)
{
    UINT32 fgProgFrmRateEn;
    UINT32 fg2XRateEn;
    UINT32 fg4XRateEn;
    UINT32 fgAuto32En;
    UINT32 fgAuto22En;
    UINT32 u4DramFrmCnt;
    UINT32 u4LR3DDly = 0;
    UINT32 fgFallback64;
    UINT32 fgFallback55p2;

    switch(arMJCFRC->u13DMode)
    {
    default:
    case E_VDO_MJC_2D:
        u4LR3DDly = 0;
	case E_VDO_MJC_TTD:
        if (arMJCFRC->u1InFrmRate > 30)
        {
            fgProgFrmRateEn = SV_OFF;
            fg2XRateEn      = ((arMJCFRC->u1OutFrmRate > 125) || ((arMJCFRC->u1OutFrmRate == 60) || (arMJCFRC->u1OutFrmRate == 50))) ? SV_OFF : SV_ON; 
            fg4XRateEn      = ((fg2XRateEn == SV_OFF) && ((arMJCFRC->u1OutFrmRate != 60) && (arMJCFRC->u1OutFrmRate != 50)));
            fgAuto32En      = (arMJCFRC->u1InFrmRate == 50) ? SV_OFF : SV_ON;
            fgAuto22En      = SV_ON;
            fgFallback64    = ((arMJCFRC->u1OutFrmRate < 125) && ((arMJCFRC->u1OutFrmRate != 60) && (arMJCFRC->u1OutFrmRate != 50))) ? SV_ON : SV_OFF;
            //fgFallback55p2  = ((arMJCFRC->u1OutFrmRate > 125) && (arMJCFRC->u1InFrmRate == 24)) ? SV_ON : SV_OFF;
            fgFallback55p2  = ((arMJCFRC->u1OutFrmRate > 125) ) ? SV_OFF : SV_OFF;
        }
        else
        {
            fgProgFrmRateEn = SV_ON;
            fg2XRateEn      = SV_OFF;
            fg4XRateEn      = SV_OFF;
            vDrvMJCSetProgFrmRate(arMJCFRC->u1InFrmRate, arMJCFRC->u1OutFrmRate);
            fgAuto32En      = SV_OFF;
            fgAuto22En      = SV_OFF;
            fgFallback64    = ((arMJCFRC->u1InFrmRate == 24) && (arMJCFRC->u1OutFrmRate == 120)) ? SV_ON : SV_OFF;
            fgFallback55p2  = ((arMJCFRC->u1InFrmRate == 24) && (arMJCFRC->u1OutFrmRate > 125)) ? SV_OFF: SV_OFF;
        }
		MJC_WRITE_FLD(MJC_ME_00, 0 , MJC_3D_SCNCHG_MODE);
        break;
    case E_VDO_MJC_FS:
        if (arMJCFRC->u1InFrmRate > 60)
        {
            fgProgFrmRateEn = SV_OFF;
            fg2XRateEn      = (arMJCFRC->u1OutFrmRate < 125) ? SV_OFF : SV_ON; 
            fg4XRateEn      = SV_OFF;
            fgAuto32En      = (arMJCFRC->u1InFrmRate == 100) ? SV_OFF : SV_ON;
            fgAuto22En      = SV_ON;
            fgFallback64    = ((arMJCFRC->u1InFrmRate == 60) && (arMJCFRC->u1OutFrmRate >= 192)) ? SV_ON : SV_OFF;
            fgFallback55p2  = SV_OFF;
        }
        else
        {
            fgProgFrmRateEn = SV_ON;
            fg2XRateEn      = SV_OFF;
            fg4XRateEn      = SV_OFF;
            vDrvMJCSetProgFrmRate(arMJCFRC->u1InFrmRate, arMJCFRC->u1OutFrmRate);
            fgAuto32En      = SV_OFF;
            fgAuto22En      = SV_OFF;
            fgFallback64    = ((arMJCFRC->u1InFrmRate == 48) && (arMJCFRC->u1OutFrmRate > 125)) ? SV_ON : SV_OFF;
            fgFallback55p2  = SV_OFF;
        }
		MJC_WRITE_FLD(MJC_ME_00, 1 , MJC_3D_SCNCHG_MODE);
        break;
    case E_VDO_MJC_SBS:
    case E_VDO_MJC_TAB:
        
        if (arMJCFRC->u1InFrmRate > 30)
        {
            fgProgFrmRateEn = SV_OFF;
            fg2XRateEn      = (arMJCFRC->u1OutFrmRate < 125) ? SV_OFF : SV_ON; 
            fg4XRateEn      = SV_OFF;
            fgAuto32En      = (arMJCFRC->u1InFrmRate == 50) ? SV_OFF : SV_ON;
            fgAuto22En      = SV_ON;
            fgFallback64    = ((arMJCFRC->u1OutFrmRate < 125) || (arMJCFRC->u1InFrmRate == 50)) ? SV_OFF : SV_ON;
            fgFallback55p2  = SV_OFF;
        }
        else
        {
            fgProgFrmRateEn = SV_ON;
            fg2XRateEn      = SV_OFF;
            fg4XRateEn      = SV_OFF;
            vDrvMJCSetProgFrmRate(arMJCFRC->u1InFrmRate, arMJCFRC->u1OutFrmRate >> 1);
            fgAuto32En      = SV_OFF;
            fgAuto22En      = SV_OFF;
            fgFallback64    = ((arMJCFRC->u1InFrmRate == 24) && (arMJCFRC->u1OutFrmRate > 125)) ? SV_ON : SV_OFF;
            fgFallback55p2  = SV_OFF;
        }
		MJC_WRITE_FLD(MJC_ME_00, 1 , MJC_3D_SCNCHG_MODE);
        break;
    }

    arMJCPrm.u1PSAuto32OnOff = fgAuto32En;
    arMJCPrm.u1PSAuto22OnOff = fgAuto22En;

    if ((arMJCFRC->u13DMode == E_VDO_MJC_TTD) && ((arMJCFRC->u1InFrmRate == 60) || (arMJCFRC->u1InFrmRate == 24)))
    {
        fgFallback64 = SV_ON;
        fgFallback55p2 = SV_OFF;
    }

    if (fgFallback64)
    {
        arMJCFRC->u1FrmSchMode = FALLBACK_MODE_64;
    }
    else
    {
        arMJCFRC->u1FrmSchMode = (fgFallback55p2) ? FALLBACK_MODE_55P2 : FALLBACK_MODE_55;
    }
    
    u4DramFrmCnt = (fgProgFrmRateEn) ? 5 : 8;
	
    u4LR3DDly = (MJC_READ_FLD(MJC_SYS_DB, MJC_TTD_BYPASS) == 1)? 3 : u4LR3DDly;
    
    MJC_WRITE_FLD(MJC_SYS_04, fgProgFrmRateEn, MJC_PROG_FRM_EN);
    MJC_WRITE_FLD(MJC_FB_03, u4DramFrmCnt, MJC_DRAM_FRM_CNT_M1);
    MJC_WRITE_FLD(MJC_SYS_D2, u4LR3DDly, MJC_OUT_LR3D_DLY);
    MJC_WRITE_FLD_MULTI(MJC_FM_00, 
        P_Fld((arMJCFRC->u1OutFrmRate > 192 ? SV_ON : SV_OFF), MJC_PPC_FORCE_INTLV) |
        P_Fld(fgFallback64, MJC_FALLBACK_64) |
        P_Fld(fgFallback55p2, MJC_FALLBACK_55P2) |
        P_Fld(fg2XRateEn, MJC_OUT_DB_RATE_EN) |
        P_Fld(fg4XRateEn, MJC_OUT_4X_RATE_EN));

    if (arMJCFRC->u13DMode == E_VDO_MJC_TTD)
    {
        MJC_WRITE_FLD_MULTI(MJC_FB_LR3D,
            P_Fld(SV_OFF, MJC_3D_MODE) |
            P_Fld(SV_OFF, MJC_3D_TYPE));
        MJC_WRITE_FLD(MJC_FB_09, SV_OFF, MJC_MID_VSYNC_IN_EN);
        MJC_WRITE_FLD(MJC_FM_00, SV_OFF, MJC_LR3D_IN_FREERUN);
    }
    else if (arMJCFRC->u13DMode)
    {
        MJC_WRITE_FLD_MULTI(MJC_FB_LR3D,
            P_Fld(SV_ON, MJC_3D_MODE) |
            P_Fld(arMJCFRC->u13DMode - 1, MJC_3D_TYPE));
        MJC_WRITE_FLD(MJC_FB_09, (arMJCFRC->u13DMode != E_VDO_MJC_FS), MJC_MID_VSYNC_IN_EN);
        MJC_WRITE_FLD(MJC_FM_00, (arMJCFRC->u13DMode != E_VDO_MJC_FS), MJC_LR3D_IN_FREERUN);
        
    }
    else
    {
        MJC_WRITE_FLD_MULTI(MJC_FB_LR3D,
            P_Fld(SV_OFF, MJC_3D_MODE) |
            P_Fld(0, MJC_3D_TYPE));
        MJC_WRITE_FLD(MJC_FB_09, SV_OFF, MJC_MID_VSYNC_IN_EN);
        MJC_WRITE_FLD(MJC_FM_00, SV_OFF, MJC_LR3D_IN_FREERUN);
        
    }

    if (arMJCFRC->u13DMode != E_VDO_MJC_FS)
    {
        MJC_WRITE_FLD(MJC_SYS_09, SV_ON, MJC_IN_LR_FORCE_ZERO);//cc:2 bit 
    }
    else
    {
        MJC_WRITE_FLD(MJC_SYS_09, SV_OFF, MJC_IN_LR_FORCE_ZERO);
    }

    //MJC_WRITE_FLD(MJC_FB_LR3D, ((arMJCFRC->u1OutFrmRate >= 192) && arMJCFRC->u13DMode ? SV_ON : SV_OFF), MJC_3D_LLRR_MODE);
    MJC_WRITE_FLD(MJC_SYS_00, ((arMJCFRC->u13DMode == E_VDO_MJC_TTD) ? SV_ON : SV_OFF), MJC_GRP_RPT_EN);
    //MJC_WRITE_FLD(MJC_SYS_04, (arMJCFRC->u1OutFrmRate >= 192 ? SV_ON : SV_OFF), MJC_B2R_INIT_FAST);

    if (arMJCFRC->u13DMode == E_VDO_MJC_TTD)
    {
		arMJCDemo.arRgn[0] = E_MJC_RGN_HF;
        arMJCDemo.arRgn[1] = E_MJC_RGN_HF;
        arMJCDemo.arRgn[2] = E_MJC_RGN_HF;
    }
    else
    {
        arMJCDemo.arRgn[0] = E_MJC_RGN_NF;
        arMJCDemo.arRgn[1] = E_MJC_RGN_NF;
        arMJCDemo.arRgn[2] = E_MJC_RGN_NF;
    }
    vDrvMJCUpdateIntpCtrl();
    
}

void vDrvMJCFrmSchPRTCPatch( MJC_PIC_SIZE_T * arMJCPicSize)
{
    if ((BSP_GetIcVersion() == IC_VER_5396_AA) && (arMJCPicSize->u2InHeight == 768) && 
	   ((arMJCPrm.u1PSAuto32OnOff == SV_ON) || (arMJCPrm.u1PSAuto32OnOff == SV_ON))) // for WXGA white line garbage due to MC_ORI + 1(NUM)/1(DEN)
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_00,
            P_Fld(SV_ON, MJC_FILM_PRTC_OFF) |
            P_Fld(SV_ON, MJC_VIDEO_PRTC_OFF));
    }
    else
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_00,
            P_Fld(SV_OFF, MJC_FILM_PRTC_OFF) |
            P_Fld(SV_OFF, MJC_VIDEO_PRTC_OFF));
    }
}


void vDrvMJCSetPicSize(const MJC_PIC_SIZE_T * arMjcPicSize)
{
    // fix 1440*900 panel issue
    if (arMjcPicSize->u2InWidth == 1440)    
    {
        MJC_WRITE_FLD_MULTI(MJC_SYS_02,
            P_Fld(1456, MJC_MJC_WIDTH)|
            P_Fld(arMjcPicSize->u2InHeight, MJC_MJC_HEIGHT_FX));
    }
    else
    {
     MJC_WRITE_FLD_MULTI(MJC_SYS_02,
         P_Fld(arMjcPicSize->u2InWidth, MJC_MJC_WIDTH)|
         P_Fld(arMjcPicSize->u2InHeight, MJC_MJC_HEIGHT_FX));
    }
 
    #ifdef SUPPORT_TV3D_OVERSCAN
    if (arMjcPicSize->u2OutHeight < (wDISPLAY_HEIGHT >> 1))
    {
        if (arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_SBS)
        {
            MJC_WRITE_FLD_MULTI(MJC_FB_09, 
                P_Fld((arMjcPicSize->u2InHeight) >> 1, MJC_MID_VSYNC_IN_CNT) |
                P_Fld(SV_ON, MJC_MID_VSYNC_IN_PROG_EN));
        }
        else if (arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_TAB)
        {
            MJC_WRITE_FLD_MULTI(MJC_FB_09, 
                P_Fld((arMjcPicSize->u2InHeight) >> 0, MJC_MID_VSYNC_IN_CNT) |
                P_Fld(SV_ON, MJC_MID_VSYNC_IN_PROG_EN));
        }
        else
        {
            MJC_WRITE_FLD_MULTI(MJC_FB_09, 
                P_Fld(0, MJC_MID_VSYNC_IN_CNT) |
                P_Fld(SV_OFF, MJC_MID_VSYNC_IN_PROG_EN));
        }
    }
    else
    #endif
    {
        MJC_WRITE_FLD_MULTI(MJC_FB_09, 
            P_Fld(0, MJC_MID_VSYNC_IN_CNT) |
            P_Fld(SV_OFF, MJC_MID_VSYNC_IN_PROG_EN));
    }

    MJC_WRITE_FLD(MJC_FB_LR3D, arMjcPicSize->fgUdp4, MJC_3D_UDP4_EN);
    MJC_WRITE_FLD(MJC_SYS_D8, arMjcPicSize->u1DispVpos, MJC_DISP_VPOS);

    MJC_WRITE_FLD(MJC_SYS_0C, ((arMjcPicSize->u2InHeight == 544) && (arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_SBS)) ? 4 : 0, MJC_IN_BLACK_H);

    // set up bnd ptg
    MJC_WRITE_FLD_MULTI(MJC_SYS_0A,
        P_Fld(SV_ON, MJC_PTG_BND_EN) |
        P_Fld(arMjcPicSize->u2InWidth, MJC_PTG_BND_W) |
        P_Fld(arMjcPicSize->u2InHeight, MJC_PTG_BND_H));
}

void vDrvMJCSetDispRDly(UINT32 u4FrmDly)
{
    MJC_WRITE_FLD(MJC_SYS_D2, u4FrmDly, MJC_OUT_LR3D_DLY);
}

void vDrvMJCSetMinDly(UINT32 fgOnOff, UINT32 u4LineThd)
{
    MJC_WRITE_FLD_MULTI(MJC_FB_RPT,
        P_Fld(fgOnOff, MJC_RPT_SHRTDLY_MODE) |
        P_Fld(u4LineThd, MJC_RPT_SHRTDLY_LINE_THD));
}


void vDrvMJCSetTTDCtrl(MJC_FRC_CONFIG_T * arMJCFRC)
{
#if 0
    if (arMJCFRC->u13DMode == E_VDO_MJC_2D)
    {
        MJC_WRITE_FLD(MJC_FB_LR3D, SV_OFF, MJC_2D_TO_3D_MODE);
        MJC_WRITE_FLD(MJC_SYS_DB, SV_ON, MJC_TTD_BYPASS);
		
        MJC_WRITE_FLD(MJC_SYS_00, SV_OFF, MJC_CLK_O_EN);
		
        vDrvTTDSetEnable(SV_OFF);
        return;
    }

    MJC_WRITE_FLD(MJC_FB_LR3D, SV_ON, MJC_2D_TO_3D_MODE);
    MJC_WRITE_FLD(MJC_SYS_DB, SV_OFF, MJC_TTD_BYPASS);
	
    MJC_WRITE_FLD(MJC_SYS_00, SV_ON, MJC_CLK_O_EN);
	
    MJC_WRITE_FLD(MJC_SYS_00, SV_ON, MJC_OUTPUT_RST);
    MJC_WRITE_FLD(MJC_SYS_00, SV_OFF, MJC_OUTPUT_RST);
    vDrvTTDSetEnable(SV_ON);
#else
	UINT32 u4TTD;
    E_TD_IN eTTDIn;
    E_TD_OUT eTTDOut;
	
    u4TTD = u4DrvTDTVTTDModeQuery();

    if (u4TTD != u4MJCTTDPrev)
    {
    	// Use eTTDIn, eTTDOut to determine vDrvTTDSetEnable & vDrvTTDSetMode
	    eTTDIn  = TD_TTD_IN(u4TTD);
	    eTTDOut = TD_TTD_OUT(u4TTD);

	#ifdef DRV_USE_EXTERNAL_3D_FRC
		//vDrvTTDSetMode(SCL_2D_TO_FS);
        MJC_WRITE_FLD(MJC_FB_LR3D, SV_OFF, MJC_2D_TO_3D_MODE);
        MJC_WRITE_FLD(MJC_SYS_DB, SV_ON, MJC_TTD_BYPASS);			
        MJC_WRITE_FLD(MJC_SYS_00, SV_OFF, MJC_CLK_O_EN);			
		vDrvTTDSetEnable(SV_OFF);
	#else
        if ((eTTDIn == E_TD_IN_2D_P) && (eTTDOut == E_TD_OUT_3D_FS))
        {
		    MJC_WRITE_FLD(MJC_FB_LR3D, SV_ON, MJC_2D_TO_3D_MODE);
		    MJC_WRITE_FLD(MJC_SYS_DB, SV_OFF, MJC_TTD_BYPASS);			
		    MJC_WRITE_FLD(MJC_SYS_00, SV_ON, MJC_CLK_O_EN);			
		    MJC_WRITE_FLD(MJC_SYS_00, SV_ON, MJC_OUTPUT_RST);
		    MJC_WRITE_FLD(MJC_SYS_00, SV_OFF, MJC_OUTPUT_RST);
		    vDrvTTDSetEnable(SV_ON);
        	vDrvTTDSetMode(MJC_2D_TO_FS);
        }
        else if ((eTTDIn == E_TD_IN_FS_P) && ((eTTDOut == E_TD_OUT_3D_FS) || (eTTDOut == E_TD_OUT_NATIVE)))
    	{
		    MJC_WRITE_FLD(MJC_FB_LR3D, SV_ON, MJC_2D_TO_3D_MODE);
		    MJC_WRITE_FLD(MJC_SYS_DB, SV_OFF, MJC_TTD_BYPASS);			
		    MJC_WRITE_FLD(MJC_SYS_00, SV_ON, MJC_CLK_O_EN);			
		    MJC_WRITE_FLD(MJC_SYS_00, SV_ON, MJC_OUTPUT_RST);
		    MJC_WRITE_FLD(MJC_SYS_00, SV_OFF, MJC_OUTPUT_RST);
		    vDrvTTDSetEnable(SV_ON);
    		vDrvTTDSetMode(MJC_FS_TO_FS);
    	}
        else
        {
	        MJC_WRITE_FLD(MJC_FB_LR3D, SV_OFF, MJC_2D_TO_3D_MODE);
	        MJC_WRITE_FLD(MJC_SYS_DB, SV_ON, MJC_TTD_BYPASS);			
	        MJC_WRITE_FLD(MJC_SYS_00, SV_OFF, MJC_CLK_O_EN);			
	        vDrvTTDSetEnable(SV_OFF);
        }
	#endif
        u4MJCTTDPrev = u4TTD;
    }
#endif    
}


#if 1
void vDrvMJCSetTTDOnOff(UINT8 u1OnOff)
{    
	if (u1OnOff)    
	{  
	    MJC_WRITE_FLD(MJC_FB_LR3D, SV_ON, MJC_2D_TO_3D_MODE);
		MJC_WRITE_FLD(MJC_SYS_DB, SV_OFF, MJC_TTD_BYPASS);
		MJC_WRITE_FLD(MJC_SYS_00, SV_ON, MJC_CLK_O_EN);
		//MJC_WRITE_FLD(MJC_SYS_00, SV_ON, MJC_OUTPUT_RST);
		//MJC_WRITE_FLD(MJC_SYS_00, SV_OFF, MJC_OUTPUT_RST);
		vDrvTTDSetEnable(SV_ON);   
	}
	else
	{	    
		MJC_WRITE_FLD(MJC_FB_LR3D, SV_OFF, MJC_2D_TO_3D_MODE);
		MJC_WRITE_FLD(MJC_SYS_DB, SV_ON, MJC_TTD_BYPASS);
		MJC_WRITE_FLD(MJC_SYS_00, SV_OFF, MJC_CLK_O_EN);
		vDrvTTDSetEnable(SV_OFF); 
	}
}
#endif

void MJC_Set3DModeDbg(UINT8 u13DMode)
{
    if(u13DMode < 6)
    {
        fgMJC3DModeDbg = 1;
		u1MJC3DMode = u13DMode;
    }
	else
	{
        fgMJC3DModeDbg = 0;
	}
	LOG(3,"fgMJC3DModeDbg:%d \n" , fgMJC3DModeDbg);
    vDrvMJCModeChangeDone(SV_VP_MAIN);
}


void vDrvMJCSetPRCtrl(MJC_FRC_CONFIG_T* arMJCFRC , MJC_PIC_SIZE_T * arMJCPicSize)
{
	UINT8 u1PRMode, fgPROnOff ,fgPRCtrl;
	u1PRMode = u1DrvMJCGetPRMode();
	fgPROnOff = u1DrvMJCGetPROnOffInfo();
	arMJCFRC->u1FHPRMode = u1DrvMJCGetHPRMode();
	if(BSP_GetIcVersion() == IC_VER_5396_AA)
	{
	    fgPRCtrl = ((u1PRMode == E_MJC_NoPR) || (arMJCFRC->u13DMode == E_VDO_MJC_2D) || 
		           (IO32ReadFldAlign(MJC_PR_MODE, MJC_3DMODE_OUT) == E_VD0_MJC_OUT_NoPR) || (fgPROnOff == SV_OFF));
	}
	else
	{
        fgPRCtrl = ((arMJCFRC->u13DMode == E_VDO_MJC_2D) || (IO32ReadFldAlign(MJC_PR_MODE, MJC_3DMODE_OUT) == E_VD0_MJC_OUT_NoPR) || (fgPROnOff == SV_OFF));
	}
    if(fgPRCtrl)
    { 
        MJC_WRITE_FLD_MULTI(MJC_SYS_PR_0, 
        P_Fld(SV_ON, MJC_PR_GEN_CORE_BYPASS) |
        P_Fld(SV_ON, MJC_PR_SYNC_REGEN_BYPASS));
		MJC_WRITE_FLD_MULTI(MJC_PR_00, 
        P_Fld(SV_OFF, MJC_PR_WREQ_EN) |
        P_Fld(SV_OFF, MJC_PR_RREQ_EN));
		if((fgPROnOff == SV_OFF) && (u1PRMode == E_MJC_HPR))
		{
            MJC_WRITE_FLD(MJC_SYS_PR_0, SV_OFF, MJC_PR_SYNC_REGEN_BYPASS);
		}
    }
	else
	{
	    MJC_WRITE_FLD(MJC_SYS_PR_0, SV_OFF, MJC_PR_GEN_CORE_BYPASS);
		MJC_WRITE_FLD_MULTI(MJC_PR_00, 
        P_Fld(SV_ON, MJC_PR_WREQ_EN) |
        P_Fld(SV_ON, MJC_PR_RREQ_EN));
		if(u1PRMode == E_MJC_HPR)
		{
            MJC_WRITE_FLD(MJC_SYS_PR_0, SV_OFF, MJC_PR_SYNC_REGEN_BYPASS);
		}
		else
		{
            MJC_WRITE_FLD(MJC_SYS_PR_0, SV_ON, MJC_PR_SYNC_REGEN_BYPASS);
		}
	}
	if(MJC_READ_FLD(MJC_SYS_PR_0, MJC_PR_SYNC_REGEN_BYPASS) == SV_OFF)
    {
	    MJC_WRITE_FLD(MJC_PR_03, 1, MJC_PR_FRM_BUF_NUM);
	    MJC_WRITE_FLD_MULTI(MJC_SYS_PR_0, P_Fld(arMJCPicSize->u2OutHTotal, MJC_PR_SYNC_REGEN_H_TOTAL) | 
			               P_Fld((arMJCPicSize->u2OutVTotal / 2), MJC_PR_SYNC_REGEN_V_TOTAL) | 
			               P_Fld( SV_ON , MJC_PR_OUT_HEIGHT_X2));
	    MJC_WRITE_FLD(MJC_SYS_PR_1, ((arMJCFRC->u1FHPRMode != E_MJC_HPR2160)? 14: 28), MJC_PR_SYNC_REGEN_V_PORCH);
		if(BSP_GetIcVersion() == IC_VER_5396_AA)
		{
		    MJC_WRITE_FLD(MJC_SYS_ATPG , SV_ON, MJC_SYS_RESERVED);
		}
		else
		{
            MJC_WRITE_FLD(MJC_SYS_PR_1 , SV_ON, MJC_PR_FOR_VSYNC_MJC);
		}
		MJC_WRITE_FLD(MJC_FM_04, ((arMJCFRC->u1FHPRMode == E_MJC_HPR240)? 1 : 0), MJC_240HZ_LR_MODE);
		if(BSP_GetIcVersion() == IC_VER_5396_AA)
		{
            if (MJC_READ_FLD(MJC_FB_0A, MJC_FLIPMRR) & 0x2)
            {
                MJC_WRITE_FLD(MJC_PR_03 , ((MJC_READ_FLD(MJC_SYS_DB, MJC_TTD_BYPASS) == 1)? 0 : 1), MJC_PR_IN_LR_INV);
            }
            else
            {
                MJC_WRITE_FLD(MJC_PR_03 , ((MJC_READ_FLD(MJC_SYS_DB, MJC_TTD_BYPASS) == 1)? 1 : 0), MJC_PR_IN_LR_INV);
            }
		}
		else
		{
            MJC_WRITE_FLD(MJC_PR_04, ((arMJCFRC->u13DMode == E_VDO_MJC_TAB) && (fgDrvMJCGetFlipMirror() > 1))?
			                           SV_ON : SV_OFF, MJC_PR_HPT_TAB_FLIP_ECO);
			MJC_WRITE_FLD(MJC_PR_03 , ((MJC_READ_FLD(MJC_SYS_DB, MJC_TTD_BYPASS) == 1)? 1 : 0), MJC_PR_IN_LR_INV);
		}
	}
	else
	{
        MJC_WRITE_FLD(MJC_PR_03, 0, MJC_PR_FRM_BUF_NUM);
		MJC_WRITE_FLD(MJC_SYS_PR_0, SV_OFF , MJC_PR_OUT_HEIGHT_X2);
	    if(BSP_GetIcVersion() == IC_VER_5396_AA)
		{
		    MJC_WRITE_FLD(MJC_SYS_ATPG , SV_OFF, MJC_SYS_RESERVED);
		}
		else
		{
            MJC_WRITE_FLD(MJC_SYS_PR_1 , SV_OFF, MJC_PR_FOR_VSYNC_MJC);
		}
	}
	MJC_WRITE_FLD(MJC_SYS_PR_0, arMJCPicSize->fgUdp4 , MJC_PR_OUT_UDP4_DISAB); 
	MJC_WRITE_FLD(MJC_SYS_PR_0, (((arMJCFRC->u1FHPRMode == E_MJC_HPR120) && (arMJCFRC->u13DMode != E_VDO_MJC_2D))? 1 : 0), MJC_PR_SYNC_REGEN_CEN_DCM);
}

void vDrvMJC1W_PR1RCtrl(void)
{
    static UINT8 u1fgcheck = 0;
	if(u1DrvMJCGet1W1Rmode() == 1)
	{
        MJC_WRITE_FLD_MULTI(MJC_FB_01,
                P_Fld(1, MJC_DRAM_RPT)|P_Fld(1, MJC_DRAM_RPT_422)|P_Fld(0, MJC_FB_TWO_FRM_EN));
		MJC_WRITE_FLD(MJC_PR_00, SV_OFF, MJC_PR_WREQ_EN);
		MJC_WRITE_FLD(MJC_SYS_00,  0x1, MJC_DRAM_RW_EN);
		MJC_WRITE_FLD(MJC_PR_00, MJC_READ_FLD(MJC_FB_08, MJC_DRAM_BASE_ADDR), MJC_PR_BASE_ADDR);
	    MJC_WRITE_FLD(MJC_PR_02, MJC_READ_FLD(MJC_FB_08, MJC_DRAM_BASE_ADDR), MJC_PR_WADDR_LO_LIMIT);
		u1fgcheck = 1;
	}
	else
	{
	    if(u1fgcheck == 1)
	    {
	        u4DrvMJCBufferInit();
            vDrvMJCSetDRAMAccessOnOff(SV_ON, arMJCPrm.u1Dram444);
		    vDrvMJCSetSystemMode(arMJCDramAlloc.u2Mode);
			u1fgcheck = 0;
	    }
		
	}
}

void vDrvMJCSet1W1Rmode(UINT8 u111Rmode)
{
    MJC_WRITE_FLD(MJC_PR_03, u111Rmode, MJC_PR_MJC1W_PR1R_MODE);
	vDrvMJCModeChangeDone(SV_VP_MAIN);
}

UINT8 u1DrvMJCGet1W1Rmode(void)
{
    return MJC_READ_FLD(MJC_PR_03, MJC_PR_MJC1W_PR1R_MODE);
}

void vDrvMJCSetPRMode(UINT8 u1PRMode , UINT8 u1FHPRMode)
{
    if(BSP_GetIcVersion() == IC_VER_5396_AA)
	{ 
        MJC_WRITE_FLD(MJC_PR_00, u1PRMode, MJC_FPR_MODE);
	    vIO32WriteFldAlign(MJC_PR_MODE, u1FHPRMode, MJC_HALF_PR_MODE);
    }
	else
	{
	    if(u1PRMode == E_MJC_FPR)       
	    {
            MJC_WRITE_FLD(MJC_PR_03, 0, MJC_HPR_MODE);     //u1PRMode express half pr on or off  1: HALF PR /0 :FULL PR
			MJC_WRITE_FLD(MJC_PR_00, u1FHPRMode, MJC_FPR_MODE);
	    }
		else
		{
            MJC_WRITE_FLD(MJC_PR_03, 1, MJC_HPR_MODE); 
			vIO32WriteFldAlign(MJC_PR_MODE, u1FHPRMode, MJC_HALF_PR_MODE);
		}
			
	}
    vDrvMJCModeChangeDone(SV_VP_MAIN);
}

void vDrvMJCSetPRModeOnOff(UINT8 fgPROnOff)
{
    vIO32WriteFldAlign(MJC_PR_MODE, fgPROnOff, MJC_PR_MODE_EN);
	vDrvMJCModeChangeDone(SV_VP_MAIN);
}

UINT8 u1DrvMJCGetPROnOffInfo(void)
{
    UINT8 fgPROnOff;
    fgPROnOff = IO32ReadFldAlign(MJC_PR_MODE, MJC_PR_MODE_EN);
	return fgPROnOff;
}

UINT8 u1DrvMJCGetPRMode(void)
{
    UINT8 u1PRMode;
	if(BSP_GetIcVersion() == IC_VER_5396_AA)
	{
        u1PRMode = MJC_READ_FLD(MJC_PR_00, MJC_FPR_MODE);
	}
	else
	{
        u1PRMode = MJC_READ_FLD(MJC_PR_03, MJC_HPR_MODE);
	}
	return u1PRMode;
}

UINT8 u1DrvMJCGetHPRMode(void)
{
    UINT8 u1HPRMode;
    u1HPRMode = IO32ReadFldAlign(MJC_PR_MODE, MJC_HALF_PR_MODE);
	return u1HPRMode;
}

UINT8 u1DrvMJCGetFPRMode(void)
{
    UINT8 u1FPRMode;
    u1FPRMode = MJC_READ_FLD(MJC_PR_00,  MJC_FPR_MODE);
	return u1FPRMode;
}


void vDrvMJCSet3DOverScan(const MJC_PIC_SIZE_T * arMjcPicSize)
{
    if (arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_2D)
    {
        MJC_WRITE_FLD(MJC_SYS_01, 0, MJC_IN_V_START);
        MJC_WRITE_FLD(MJC_SYS_MID, 0, MJC_IN_MID_BLANK);        
    }
    else if (arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_TAB)
    {
        MJC_WRITE_FLD(MJC_SYS_01, arMjcPicSize->rClipInfo.wTop, MJC_IN_V_START);
        MJC_WRITE_FLD(MJC_SYS_MID, (arMjcPicSize->rClipInfo.wTop + arMjcPicSize->rClipInfo.wBottom) >> 1, MJC_IN_MID_BLANK);        
    }
    else
    {
        MJC_WRITE_FLD(MJC_SYS_01, arMjcPicSize->rClipInfo.wTop, MJC_IN_V_START);
        MJC_WRITE_FLD(MJC_SYS_MID, 0, MJC_IN_MID_BLANK);
    }
}

void vDrvMJCSetDispVpos(UINT32 u4PscanPrelen)
{
    UINT32 u4DispVpos;
    u4DispVpos = u4PscanPrelen - 2;
    arMJCPrm.arPicSize.u1DispVpos = (UINT8)MIN(28, u4DispVpos);
    MJC_WRITE_FLD(MJC_SYS_D8, arMJCPrm.arPicSize.u1DispVpos, MJC_DISP_VPOS);
}

void vDrvMJCSetDispMode(UINT32 fgOnOff)
{
    MJC_WRITE_FLD(MJC_SYS_D2, !(fgOnOff), MJC_OUT_SYNC_SELF);
}

UINT32 fgDrvMJCGetDispMode(void)
{
    UINT32 fgMJCDispMode;

    fgMJCDispMode = MJC_READ_FLD(MJC_SYS_D2, MJC_OUT_SYNC_SELF) ? E_MJC_SELF_MODE : E_MJC_DISP_MODE;
    return fgMJCDispMode;
}

UINT32 fgDrvMJCGetFlipMirror(void)
{
    return MJC_READ_FLD(MJC_FB_0A, MJC_FLIPMRR);
}


static BOOL fgDrvMJCGetFRCConfig(MJC_FRC_CONFIG_T* arMJCFRC)
{
    UINT8 u4MJCInputFrameRate;
    arMJCFRC->u1FHPRMode = u1DrvMJCGetHPRMode();
    arMJCPrm.arMJCPRConfig.u1OutPRFrameRate = ((arMJCFRC->u1FHPRMode == E_MJC_HPR2160)? (vDrvGetLCDFreq()/2) : vDrvGetLCDFreq());
    if(fgMJC3DModeDbg == 1)
    {
        arMJCFRC->u13DMode = u1MJC3DMode;
	}
	else
	{
        arMJCFRC->u13DMode = IO32ReadFldAlign(MJC_PR_MODE, MJC_3DMODE_IN); 
	}
    arMJCFRC->u1OutFrmRate = (((arMJCFRC->u1FHPRMode > E_MJC_HPR120) && (arMJCFRC->u13DMode != E_VDO_MJC_2D) && (IO32ReadFldAlign(MJC_PR_MODE, MJC_3DMODE_OUT) != E_VD0_MJC_OUT_NoPR))? (arMJCPrm.arMJCPRConfig.u1OutPRFrameRate * 2) : arMJCPrm.arMJCPRConfig.u1OutPRFrameRate);                                          
    u4MJCInputFrameRate = bDrvVideoGetRefreshRate(SV_VP_MAIN);

	if((bDrvVideoSignalStatus(SV_VP_MAIN) == (UINT8)SV_VDO_STABLE) || (bDrvVideoSignalStatus(SV_VP_MAIN) == (UINT8)SV_VDO_UNKNOWN))
	{
	    arMJCFRC->u1InFrmRate  = u4MJCInputFrameRate;  
	}
	else
	{
        arMJCFRC->u1InFrmRate  = arMJCPrm.arFRCConfig.u1InFrmRate; 
	} 	
    if (arMJCFRC->u1OutFrmRate < 96)
    {
        arMJCFRC->u1PanelType = E_MJC_60HZ;
    }
    else if (arMJCFRC->u1OutFrmRate < 192)
    {
        arMJCFRC->u1PanelType = E_MJC_120HZ;
    }
    else
    {
        arMJCFRC->u1PanelType = E_MJC_240HZ;
    }
    if ((arMJCFRC->u1OutFrmRate != arMJCPrm.arFRCConfig.u1OutFrmRate) ||
        (arMJCFRC->u1InFrmRate != arMJCPrm.arFRCConfig.u1InFrmRate) ||
        (arMJCFRC->u13DMode != arMJCPrm.arFRCConfig.u13DMode) ||
        (arMJCFRC->u1FHPRMode != arMJCPrm.arFRCConfig.u1FHPRMode))
    {
        return SV_TRUE;
    }
    else
    {
        return SV_FALSE;
    }
}

static BOOL fgDrvMJCGetPicSize(MJC_PIC_SIZE_T * arPicSize)
{
    UINT32 u4DispVpos , u4Prelen;
    UINT8 u1PRMode;
    UINT16 u2Height, u2Width;

    arPicSize->u2InHStart  = 0;
    arPicSize->u2InVStart  = 0;
    arPicSize->u2OutHStart = 0;
    arPicSize->u2OutVStart = 0;
    arPicSize->fgUdp4      = SV_OFF;
	
    u1PRMode = u1DrvMJCGetPRMode();
    u2Height = PANEL_GetPanelHeight();
    u2Width  = u2FscGetMaxOutWidth();

    u1ScpipGetMJCClipInfo(SV_VP_MAIN, &(arPicSize->rClipInfo));

    #ifdef SUPPORT_TV3D_OVERSCAN
    //u2Width = (arPicSize->rClipInfo.wTargetWidth == 0) ? u2FscGetMaxOutWidth() : arPicSize->rClipInfo.wTargetWidth;
    //arPicSize->u2OutWidth  = (arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_SBS) ? (u2Width >> 1) : u2Width;
    //arPicSize->u2InWidth   = ALIGN_16_CEIL(arPicSize->u2OutWidth);
    if (arPicSize->rClipInfo.wTargetWidth != 0)
    {
        arPicSize->u2OutWidth = arPicSize->rClipInfo.wTargetWidth;
        arPicSize->u2OutWidth = (arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_SBS) ?
            (arPicSize->u2OutWidth >> 1) : arPicSize->u2OutWidth;
        arPicSize->u2InWidth  = ALIGN_16_CEIL(arPicSize->u2OutWidth);
        #if 1 // mjc full screen output patch
        if (arMJCPrm.arFRCConfig.u13DMode != E_VDO_MJC_SBS)
        {
            arPicSize->u2OutWidth = u2Width;
        }
        #endif
    }
    else
    #endif
    {
        arPicSize->u2OutWidth = (arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_SBS) ? (u2Width >> 1) : u2Width;
        arPicSize->u2InWidth  = ALIGN_16_CEIL(arPicSize->u2OutWidth);
    }

    #ifdef SUPPORT_TV3D_OVERSCAN
    //arPicSize->u2OutHeight = (((u1PRMode == E_MJC_HPR) && (u1DrvMJCGetPROnOffInfo() == SV_OFF))? (u2Height / 2) : u2Height);   
    //arPicSize->u2InHeight = ALIGN_8_CEIL(((u1PRMode == E_MJC_HPR) && (arMJCPrm.arFRCConfig.u13DMode != E_VDO_MJC_2D) && (IO32ReadFldAlign(MJC_PR_MODE, MJC_3DMODE_OUT) != E_VD0_MJC_OUT_NoPR))? 
    //    (u2Height / 2) : u2Height);
    if (arPicSize->rClipInfo.wTargetHeight != 0)
    {
        arPicSize->u2OutHeight = arPicSize->rClipInfo.wTargetHeight;
        arPicSize->u2OutHeight = ((u1PRMode == E_MJC_HPR) && (u1DrvMJCGetPROnOffInfo() == SV_OFF)) ?
            (arPicSize->u2OutHeight >> 1) : arPicSize->u2OutHeight;
        #if 0 // mjc full screen output patch
        if (arMJCPrm.arFRCConfig.u13DMode != E_VDO_MJC_SBS)
        {
            arPicSize->u2OutHeight = u2Height;
        }
        #endif
        arPicSize->u2InHeight  = ((u1PRMode == E_MJC_HPR) && (arMJCPrm.arFRCConfig.u13DMode != E_VDO_MJC_2D) && (IO32ReadFldAlign(MJC_PR_MODE, MJC_3DMODE_OUT) != E_VD0_MJC_OUT_NoPR)) ?
            (arPicSize->rClipInfo.wTargetHeight >> 1) : arPicSize->rClipInfo.wTargetHeight;
        arPicSize->u2InHeight  = ALIGN_8_CEIL(arPicSize->u2InHeight);
    }
    else
    #endif
    {
        arPicSize->u2OutHeight = u2Height;
        arPicSize->u2InHeight  = ((u1PRMode == E_MJC_HPR) && (arMJCPrm.arFRCConfig.u13DMode != E_VDO_MJC_2D) && (IO32ReadFldAlign(MJC_PR_MODE, MJC_3DMODE_OUT) != E_VD0_MJC_OUT_NoPR)) ?
            (u2Height >> 1) : u2Height;
        arPicSize->u2InHeight  = ALIGN_8_CEIL(arPicSize->u2InHeight);
    }

    arPicSize->u2OutHTotal = (arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_SBS) ? 
        (wDrvGetOutputHTotal() >> 1) : wDrvGetOutputHTotal();         
    arPicSize->u2OutVTotal = wDrvGetOutputVTotal();          

    if ((arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_TAB) && (arPicSize->u2OutHeight & 0xF) && (u1PRMode == E_MJC_HPR) && (IO32ReadFldAlign(MJC_PR_MODE, MJC_3DMODE_OUT) != E_VD0_MJC_OUT_NoPR))
    {
        arPicSize->fgUdp4 = SV_ON;
    }

    if ((arMJCPrm.u1Flip == 1) && (arPicSize->u2OutHeight & 0xF)  && (IO32ReadFldAlign(MJC_PR_MODE, MJC_3DMODE_OUT) != E_VD0_MJC_OUT_NoPR) &&
        ((arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_SBS )||(arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_FS) || (arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_TTD)))
    {
        arPicSize->u2OutVStart += 8;
    }
    
    u4Prelen = IO32ReadFldAlign(PSPIP1_0A, PSPIP1_0A_VS_PSCAN_PRELEN_1) - 2;
    u4DispVpos = fgIsSupport120HzPanel()? MJC_DISP_VPOS_120_TARGET : MJC_DISP_VPOS_060_TARGET;
    u4DispVpos = (u4DispVpos * ((UINT32)arPicSize->u2OutVTotal) + 0x8000) >> 16;
    arPicSize->u1DispVpos = (((u1PRMode == E_MJC_HPR) && (arMJCPrm.arFRCConfig.u13DMode != E_VDO_MJC_2D)  && (IO32ReadFldAlign(MJC_PR_MODE, MJC_3DMODE_OUT) != E_VD0_MJC_OUT_NoPR))?
        (u4Prelen - arPicSize->u2OutVStart) :(UINT8)MIN(u4Prelen , (u4DispVpos - arPicSize->u2OutVStart)));

    if ((arPicSize->u2InHStart              != arMJCPrm.arPicSize.u2InHStart) ||
        (arPicSize->u2InVStart              != arMJCPrm.arPicSize.u2InVStart) ||
        (arPicSize->u2InWidth               != arMJCPrm.arPicSize.u2InWidth) ||
        (arPicSize->u2InHeight              != arMJCPrm.arPicSize.u2InHeight) ||
        (arPicSize->u2OutWidth              != arMJCPrm.arPicSize.u2OutWidth) ||
        (arPicSize->u2OutHeight             != arMJCPrm.arPicSize.u2OutHeight) ||
        (arPicSize->u2OutHTotal             != arMJCPrm.arPicSize.u2OutHTotal) ||
        (arPicSize->u2OutVTotal             != arMJCPrm.arPicSize.u2OutVTotal) ||
        (arPicSize->u2OutVStart             != arMJCPrm.arPicSize.u2OutVStart) ||
        (arPicSize->rClipInfo.wTop          != arMJCPrm.arPicSize.rClipInfo.wTop) ||
        (arPicSize->rClipInfo.wBottom       != arMJCPrm.arPicSize.rClipInfo.wBottom) ||
        (arPicSize->rClipInfo.wWidth        != arMJCPrm.arPicSize.rClipInfo.wWidth) ||
        (arPicSize->rClipInfo.wHeight       != arMJCPrm.arPicSize.rClipInfo.wHeight) ||
        (arPicSize->rClipInfo.wTargetWidth  != arMJCPrm.arPicSize.rClipInfo.wTargetWidth) ||
        (arPicSize->rClipInfo.wTargetHeight != arMJCPrm.arPicSize.rClipInfo.wTargetHeight) ||
        (arPicSize->rClipInfo.wClipX        != arMJCPrm.arPicSize.rClipInfo.wClipX) ||
        (arPicSize->rClipInfo.wClipY        != arMJCPrm.arPicSize.rClipInfo.wClipY) ||
        (arPicSize->fgUdp4                  != arMJCPrm.arPicSize.fgUdp4) ||
        (arPicSize->u1DispVpos              != arMJCPrm.arPicSize.u1DispVpos))
    {
        return SV_TRUE;
    }
    else
    {
        return SV_FALSE;
    }
}


void vDrvMJCModeChgProc(UINT32 u4Path)
{
    MJC_FRC_CONFIG_T arMJCFRC;
    MJC_PIC_SIZE_T arMJCPicSize;
    UINT32 fgModeChg = SV_FALSE;
    UINT32 u4MJCMuteCount;
	
    if ((u4Path != SV_VP_MAIN) || _fgIsInitialized == SV_FALSE)
        return;

    if (fgDrvMJCGetFRCConfig(&arMJCFRC) || IO32ReadFldAlign(MJC_MISC_00, MJC_FLMR_CHG_TRIG))
    {
        if (arMJCPrm.u1Dram444 == SV_FALSE)
        {
            u4MJCMuteCount = IO32ReadFldAlign(MJC_MISC_00, MODE_CHG_MUTE_COUNT);
            u4MJCMuteCount = (arMJCFRC.u13DMode || arMJCPrm.arFRCConfig.u13DMode) ? (u4MJCMuteCount << 1) : u4MJCMuteCount;
            _vDrvVideoSetMute(MUTE_MODULE_MJC, SV_VP_MAIN, u4MJCMuteCount, SV_TRUE);
        }
        vIO32WriteFldAlign(MJC_MISC_00, 0, MJC_FLMR_CHG_TRIG);
		vDrvMJCSetPSAutoOnOff(SV_OFF);
        vDrvMJCSetFrmSch(&arMJCFRC);
        vDrvMJCSetProgME(arMJCFRC.u1OutFrmRate < 192, arMJCFRC.u1FrmSchMode == FALLBACK_MODE_55P2); 
        vDrvMJCSetTTDCtrl(&arMJCFRC);
        arMJCPrm.arFRCConfig = arMJCFRC;
	    vIO32MJCWriteFldAlign(MJC_FBCK_13, 1, FBCK_THR_TRIG);
        fgModeChg = SV_TRUE;
        LOG(3, "vDrvMJCModeChgProc: fgDrvMJCGetFRCConfig mode change\n");
    }
    // Trigger to set psauto
    vDrvMJCSetPsAutoTrigOn();

    if (fgDrvMJCGetPicSize(&arMJCPicSize))
    {
        if ((arMJCPrm.u1Dram444 == SV_FALSE) && (fgModeChg == SV_FALSE))
        {
            u4MJCMuteCount = IO32ReadFldAlign(MJC_MISC_00, MODE_CHG_MUTE_COUNT);
            u4MJCMuteCount = arMJCPrm.arFRCConfig.u13DMode ? (u4MJCMuteCount << 1) : u4MJCMuteCount;
            _vDrvVideoSetMute(MUTE_MODULE_MJC, SV_VP_MAIN, u4MJCMuteCount, SV_TRUE);
        }
        vDrvMJCSetPicSize(&arMJCPicSize);
        vDrvTTDSetPicSize(arMJCPicSize.u2InWidth, arMJCPicSize.u2InHeight);
        vDrvMJCSetOutputTiming(&arMJCPicSize);
        #ifdef SUPPORT_TV3D_OVERSCAN
        vDrvMJCSet3DOverScan(&arMJCPicSize);
        #endif
        arMJCPrm.arPicSize = arMJCPicSize;
        fgModeChg = SV_TRUE;
        LOG(3, "vDrvMJCModeChgProc: fgDrvMJCGetPicSize mode change\n");
    }
    
    if (fgModeChg)
    {
        vDrvMJC1W_PR1RCtrl();
        vDrvMFDModeChgDone(SV_VP_MAIN);
        vDrvMJCFrmSchPRTCPatch(&arMJCPicSize);
        MJC_WRITE_FLD(MJC_SYS_04, (((arMJCFRC.u1OutFrmRate >= 192) || (arMJCPrm.arPicSize.fgUdp4 == SV_ON))? SV_ON : SV_OFF), MJC_B2R_INIT_FAST);
        vDrvMJCSetPRCtrl(&arMJCFRC , &arMJCPicSize);
    }
}

void vDrvMJCModeChangeDone(UINT32 u4Path)
{
    if (u4Path == SV_VP_MAIN)
    {
        LOG(3, "vDrvMJCModeChangeDone\n");
        //vDrvMJCSetPsAutoTrigOn();
        fgMJCModeChg = SV_TRUE;
    }
}


void vDrvMJCSetOutputTiming(const MJC_PIC_SIZE_T * arMjcPicSize)
{
    MJC_WRITE_FLD_MULTI(MJC_SYS_04,
        P_Fld(arMjcPicSize->u2OutWidth, MJC_OUT_WIDTH) |
        P_Fld(arMjcPicSize->u2OutHeight, MJC_OUT_HEIGHT));
    MJC_WRITE_FLD_MULTI(MJC_SYS_05,
        P_Fld(arMjcPicSize->u2OutHTotal, MJC_OUT_H_TOTAL) |
        P_Fld(arMjcPicSize->u2OutVTotal, MJC_OUT_V_TOTAL));
    MJC_WRITE_FLD_MULTI(MJC_SYS_03,
        P_Fld(arMjcPicSize->u2OutHStart, MJC_OUT_H_START) |
        P_Fld(arMjcPicSize->u2OutVStart, MJC_OUT_V_START));
}


void vDrvMJCSetVsyncDelay(UINT32 u4InVStart, UINT32 u4OutVact)
{
    MJC_WRITE_FLD(MJC_SYS_01, u4InVStart, MJC_IN_V_START);
    MJC_WRITE_FLD(MJC_SYS_D2, u4OutVact, MJC_OUT_VACT_PRE);
}

UINT16 u2DrvMJCGetOutputHTotal(void)
{
    return MJC_READ_FLD(MJC_SYS_05, MJC_OUT_H_TOTAL);
}

UINT16 u2DrvMJCGetOutputVTotal(void)
{
    return MJC_READ_FLD(MJC_SYS_05, MJC_OUT_V_TOTAL);
}

UINT32 u4MJCBaseAddress = (UINT32)NULL;

UINT32 u4DrvMJCBufferInit(void)
{
    #ifndef CC_FPGA
    FBM_POOL_T * prPool;

    prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_MJC , NULL);
    if (prPool == NULL)
    {
        LOG(0, "u4DrvMJCBufferInit: Fail to get pool\n");
        return SV_FAIL;
    }
    if (prPool->u4Size == 0)
    {
        return SV_FAIL;
    }
    if (u4MJCBaseAddress == (UINT32)NULL)
    {
        u4MJCBaseAddress = (UINT32)BSP_MapReservedMem((void *)prPool->u4Addr, prPool->u4Size);
    }

    arMJCDramAlloc.u4StartAddr = prPool->u4Addr;
    arMJCDramAlloc.u4EndAddr   = prPool->u4Addr + prPool->u4Size;
    arMJCDramAlloc.u4Width     = ALIGN_16_CEIL(prPool->u4Width);
    arMJCDramAlloc.u4Height    = ALIGN_8_CEIL(prPool->u4Height);
    arMJCDramAlloc.u2Mode      = u4DrvMJCRemapMJCFbmMode(prPool->u4Mode);
    arMJCDramAlloc.u2DebugMode = 0xFFFF;

    vDrvMJCSetBuf(&arMJCDramAlloc);
    #endif
    arMJCDramAlloc.u2Mode = FRC_Y08_C08_444;
    vDrvMJCSetSystemMode(arMJCDramAlloc.u2Mode);

	return SV_SUCCESS;
}

void vDrvMJCSrmCb(UINT32 u4VdpId, UINT32 u4Mode, UINT32 u4PDSize, UINT32 u4SramMode)
{
    UINT8 u13DModeIn , u13DModeOut;
	UINT8 u1MJC3DModeIn = 0, u1MJC3DModeOut = 0;
    UINT32 u4MuteDelay = 0;

    if (_fgIsInitialized == FALSE)
    {
        return;
    }
    if (IO32ReadFldAlign(MJC_MISC_00, SRM_CB_OFF) || ((u4Mode & 0x7FFF8000) == FBM_POOL_MODE_UNSTABLE))
    {
    	return;
    }
    #ifdef CC_FLIP_MIRROR_SUPPORT 
    if(u4GetFlipMirrorModule(VDP_1) == FLIP_BY_MJC)
    {
        vDrvMJCSetFlipMirror(u1GetFlipMirrorConfig() & SYS_FLIP_CONFIG_ON, 
        ((u1GetFlipMirrorConfig() & SYS_MIRROR_CONFIG_ON) >> 1));
    }
    #endif
    if (IO32ReadFldAlign(MJC_MISC_00, MJC_PR_MODE_CHG_OFF) == SV_OFF)
    {
    	u13DModeIn = TD_MJC_IN(u4SramMode);
    	u13DModeOut = TD_MJC_OUT(u4SramMode);
		u1MJC3DModeIn = (u13DModeIn == E_TD_IN_TTD_P) ? E_VDO_MJC_TTD :
			            (u13DModeIn == E_TD_IN_TB_P) ? E_VDO_MJC_TAB :
			            (u13DModeIn == E_TD_IN_SBS_P) ? E_VDO_MJC_SBS :
			            (u13DModeIn == E_TD_IN_FS_P) ? E_VDO_MJC_FS :
			            (u13DModeIn == E_TD_IN_2D_P) ? E_VDO_MJC_2D : E_VDO_MJC_OTHER ;
	    if(u1MJC3DModeIn == E_VDO_MJC_OTHER)
	    {
            LOG(3,"vDrvMJCSrmCb get wrong mjc input 3d format ->u1MJC3DModeIn:%d\n", u1MJC3DModeIn);
			return;
		}
		u1MJC3DModeOut = (u13DModeOut == E_TD_OUT_3D_FPR) ? E_VD0_MJC_OUT_FPR : 
			             (u13DModeOut == E_TD_OUT_3D_LI) ? E_VD0_MJC_OUT_HPR : 
			             ((u13DModeOut == E_TD_OUT_3D_FS) || (u13DModeOut == E_TD_OUT_NATIVE)) ? E_VD0_MJC_OUT_NoPR : E_VD0_MJC_OUT_OTHER;
		if(u1MJC3DModeOut == E_VD0_MJC_OUT_OTHER)
	    {
            LOG(3,"vDrvMJCSrmCb get wrong mjc output 3d format ->u1MJC3DModeOut:%d\n", u1MJC3DModeOut);
			return;
		}
		if(u1MJC3DModeIn != IO32ReadFldAlign(MJC_PR_MODE, MJC_3DMODE_IN))
		{ 
		    vIO32WriteFldAlign(MJC_PR_MODE, u1MJC3DModeIn, MJC_3DMODE_IN);
		    fgMJCModeChg = SV_TRUE;
		}
    	if(u1MJC3DModeOut != IO32ReadFldAlign(MJC_PR_MODE, MJC_3DMODE_OUT))
    	{
    	    vIO32WriteFldAlign(MJC_PR_MODE, u1MJC3DModeOut, MJC_3DMODE_OUT);
    	    if(IS_PANEL_PDP)
	        {
                vDrvMJCSetPRMode(E_MJC_HPR , E_MJC_HPR2160);         //set half pr 2160 for pdp panel 
	        }
            if (u1MJC3DModeOut == E_VD0_MJC_OUT_HPR)
            {
                if(vDrvGetLCDFreq() < 96)
	            {
	                vDrvMJCSetPRMode(E_MJC_HPR , E_MJC_HPR60);    
	            }
	            else
	            {
                    if(MJC_HALF_PR_240)
		            {
		                vDrvMJCSetPRMode(E_MJC_HPR , E_MJC_HPR240); 
		            }
		            else
		            {
		                vDrvMJCSetPRMode(E_MJC_HPR , E_MJC_HPR120);  
		            }
	            }
            }
            
    	    if (u1MJC3DModeOut == E_VD0_MJC_OUT_FPR) 
    	    {
    	        if(BSP_GetIcVersion() == IC_VER_5396_AA)
    	        {
                    vDrvMJCSetPRMode(((MJC_FULL_PR == 0) ? (E_MJC_FPRV1 + 1) : (E_MJC_FPRV2 + 1)) , E_MJC_NOHPR);
    	        }
				else
				{
                    vDrvMJCSetPRMode(E_MJC_FPR , ((MJC_FULL_PR == 1) ? E_MJC_FPRV1 :
						                             (MJC_FULL_PR == 2) ? E_MJC_FPRV2 : 
						                             (MJC_FULL_PR == 3) ? E_MJC_FPRV3 : E_MJC_FPRV0));
				}
    	    }
	    }
    }

    if ((arMJCDramAlloc.u2DebugMode != 0xFFFF) || (arMJCDramAlloc.u2Mode != u4DrvMJCRemapMJCFbmMode(u4Mode)) || 
        ((u1MJC3DModeIn != arMJCPrm.arFRCConfig.u13DMode) && (arMJCPrm.u1Dram444)))
    {
        arMJCDramAlloc.u2Mode      = u4DrvMJCRemapMJCFbmMode(u4Mode);
        arMJCDramAlloc.u2DebugMode = 0xFFFF;
        u4MuteDelay = IO32ReadFldAlign(MJC_MISC_00, MJC_MUTE_COUNT);

        LOG(3, "vDrvMJCSrmCb: %x\n", arMJCDramAlloc.u2Mode);

        if (u1DrvMJCIsOnOff())
        {
            _vDrvVideoSetMute(MUTE_MODULE_MJC, SV_VP_MAIN, u4MuteDelay, SV_TRUE);
            if (IO32ReadFldAlign(MJC_MISC_00, MJC_MODE_CHG_AFTER_MUTE) == 0)
            {
                vDrvMJCSetFrmFreeze(SV_OFF);
                vDrvMJCSetDRAMAccessOnOff(SV_OFF, 0);
                vDrvMJCSetSystemMode(arMJCDramAlloc.u2Mode);
                vDrvMJCSetDRAMAccessOnOff(SV_ON, arMJCPrm.u1Dram444);
            }
            else
            {
                vIO32WriteFldAlign(MJC_MISC_00, SV_ON, MJC_MODE_CHG_TRIG);
            }
        }
        else
        {
            vDrvMJCSetSystemMode(arMJCDramAlloc.u2Mode);
        }
        fgMJCModeChg = SV_TRUE;
    }
}

void vDrvMJCGetSPReg(void)
{
    UINT32 u4Count;

    for (u4Count = 0; u4Count < 8; u4Count++)
    {
        Printf("(%d) 0x%08X[%02d:%02d]\n", u4Count, arMJCSPReg[u4Count].u4Addr, arMJCSPReg[u4Count].u1Msb, arMJCSPReg[u4Count].u1Lsb);
    }
}

UINT32 u4DrvMJCSetSPReg(UINT32 u4Index, UINT32 u4Addr, UINT32 u4Msb, UINT32 u4Lsb)
{
    if (u4Index >= 8)
    {
        return SV_FALSE;
    }

    arMJCSPReg[u4Index].u4Addr = u4Addr;
    arMJCSPReg[u4Index].u1Msb  = u4Msb;
    arMJCSPReg[u4Index].u1Lsb  = u4Lsb;

    return SV_TRUE;
}

void vDrvMJCRegStatusPolling(void *pvArgv)
{
    UINT32 u4Status;
    UINT32 u4Count = 0;
    while ((u4Count < 8) && (arMJCSPReg[u4Count].u4Addr != 0))
    {
        if ((arMJCSPReg[u4Count].u4Addr & 0xFFFFF000) == 0x20006000)
        {
            u4Status = MJC_READ_FLD(arMJCSPReg[u4Count].u4Addr&0x0000FFFF, Fld((arMJCSPReg[u4Count].u1Msb - arMJCSPReg[u4Count].u1Lsb + 1), arMJCSPReg[u4Count].u1Lsb, AC_MSKDW));
        }
        else if (arMJCSPReg[u4Count].u4Addr < 0x10000)
        {
            u4Status = IO32ReadFldAlign(u4SWReg(arMJCSPReg[u4Count].u4Addr), Fld((arMJCSPReg[u4Count].u1Msb - arMJCSPReg[u4Count].u1Lsb + 1), arMJCSPReg[u4Count].u1Lsb, AC_MSKDW));
        }
        else
        {
            u4Status = IO32ReadFldAlign(arMJCSPReg[u4Count].u4Addr, Fld((arMJCSPReg[u4Count].u1Msb - arMJCSPReg[u4Count].u1Lsb + 1), arMJCSPReg[u4Count].u1Lsb, AC_MSKDW));
        }
        Printf("%08X ", u4Status);
        u4Count++;
    }
}

void vDrvMJCSetDramStaMonitorCount(UINT32 u4Val)
{
    _u4DramStaMonitorCount = u4Val;
}

UINT32 u4DrvMJCGetDramStaMonitorCount(void)
{
    return _u4DramStaMonitorCount;
}

void vDrvMJCClearDramStatus(void)
{
    // clear
    MJC_WRITE_FLD(MJC_FB_01, 1, MJC_DRAM_STATUS_CLR);
    MJC_WRITE_FLD(MJC_SYS_00, 1, MJC_RD_UNDER_RST);
    MJC_WRITE_FLD(MJC_FB_01, 0, MJC_DRAM_STATUS_CLR);
    MJC_WRITE_FLD(MJC_SYS_00, 0, MJC_RD_UNDER_RST);
}

UINT8 u1DrvMJCChkDramOvfUdf(UINT32 * pu4StaFB01, UINT32 * pu4StaFB02, UINT32 * pu4StaSys00)
{
    UINT8 u1Ret = 0;
    UINT32 u4Msk42X_0, u4Msk42X_1, u4Msk42X_2;
    UINT32 u4Sta0, u4Sta1, u4Sta2;

    if ((!arMJCPrm.u1Dram444) && (!arMJCPrm.u1Dram422RPT))
    {
        u4Msk42X_0 = Fld2Msk32(MJC_STA_WADDR_OOB)|
            Fld2Msk32(MJC_STA_WFIFO_MSB_OVF)|Fld2Msk32(MJC_STA_WFIFO_LSB_OVF)|
            Fld2Msk32(MJC_STA_WFIFO_MSB_UDF)|Fld2Msk32(MJC_STA_WFIFO_LSB_UDF);
        u4Msk42X_1 = Fld2Msk32(MJC_STA_FILM_WOVF)|Fld2Msk32(MJC_STA_FILM_WUDF)|
            Fld2Msk32(MJC_STA_FILM_ROVF)|Fld2Msk32(MJC_STA_FILM_RUDF);
        u4Msk42X_2 = Fld2Msk32(MJC_STA_INIT_UNDER)|Fld2Msk32(MJC_STA_PROC_UNDER);
    }
    else
    {
        u4Msk42X_0 = Fld2Msk32(MJC_STA_WADDR_OOB)|
            Fld2Msk32(MJC_STA_WFIFO_MSB_OVF)|Fld2Msk32(MJC_STA_WFIFO_LSB_OVF)|
            Fld2Msk32(MJC_STA_RFIFO_PYE_OVF)|Fld2Msk32(MJC_STA_RFIFO_PCE_OVF)|
            Fld2Msk32(MJC_STA_RFIFO_CYE_OVF)|Fld2Msk32(MJC_STA_RFIFO_CCE_OVF)|
            Fld2Msk32(MJC_STA_RFIFO_PYO_OVF)|Fld2Msk32(MJC_STA_RFIFO_NYO_OVF)|
            Fld2Msk32(MJC_STA_RFIFO_PCO_OVF)|Fld2Msk32(MJC_STA_RFIFO_CCO_OVF)|
            Fld2Msk32(MJC_STA_RFIFO_CYO_OVF)|Fld2Msk32(MJC_STA_WFIFO_MSB_UDF)|
            Fld2Msk32(MJC_STA_WFIFO_LSB_UDF)|Fld2Msk32(MJC_STA_RFIFO_PYE_UDF)|
            Fld2Msk32(MJC_STA_RFIFO_PCE_UDF)|Fld2Msk32(MJC_STA_RFIFO_CYE_UDF)|
            Fld2Msk32(MJC_STA_RFIFO_PCO_UDF)|
            Fld2Msk32(MJC_STA_RFIFO_CCO_UDF)|Fld2Msk32(MJC_STA_RFIFO_CYO_UDF);
        u4Msk42X_1 = 0;
        u4Msk42X_2 = 0;
    }

    u4Sta0 = MJC_READ_4B(MJC_STA_FB_01) & u4Msk42X_0;
    if (u4Sta0)
    {
        u1Ret ++;
    }
    
    if (pu4StaFB01)
        (*pu4StaFB01) = u4Sta0;

    u4Sta1 = MJC_READ_4B(MJC_STA_FB_02) & u4Msk42X_1;
    if (u4Sta1)
    {
        u1Ret ++;
    }
    
    if (pu4StaFB02)
        (*pu4StaFB02) = u4Sta1;

    u4Sta2 = MJC_READ_4B(MJC_STA_SYS_00) & u4Msk42X_2;
    if (u4Sta2)
    {
        u1Ret ++;
    }

    if (pu4StaSys00)
        (*pu4StaSys00) = u4Sta2;

    return u1Ret;
}

void vDrvMJCSetMVSortOnOff(UINT32 u4OnOff)
{
    MJC_WRITE_FLD(MJC_MV_00, u4OnOff, MJC_MV_SORT_EN);
}

void vDrvMJCInputLRCheck(void *pvArgv)
{
    static UINT8 fg;
	static UINT8 u1MJCLR_IN1, u1MJCLR_IN2;
	if((u1MJCStatusCheck == 1) || (u1MJCStatusCheck == 6))
    {
        u1MJCLR_IN1 = u1MJCLR_IN2;
		u1MJCLR_IN2 = MJC_READ_FLD(MJC_STA_LR3D, MJC_STA_LR3D_INFO_I);
		if(((u1MJCLR_IN1 + u1MJCLR_IN2) != 1) && (fg == 0))
        {
            printf("LR ERROR!\n");
			fg = 1;
		 
	    }
	    if(fg)
		{
            if(fg == 8)     
		    {
			    u1MJCStatusCheck = 0;
            }
		    fg++;
	    }
		Printf("u1MJCLR_IN:%d \n ", u1MJCLR_IN2);
		if(u1MJCStatusCheck == 6)
		{
            u1MJCStatusCheck = 0;
			printf("MJC Output FS LR Check interrupt \n");
		}
	}
	if(u1MJCStatusCheck == 2)
	{
        u1MJCLR_IN2 = MJC_READ_FLD(MJC_STA_LR3D, MJC_STA_LR3D_INFO_I);
	    u1MJCStatusCheck = 1;
		Printf("u1MJCLR_IN:%d \n ", u1MJCLR_IN2);
	}
	if(u1MJCStatusCheck == 3)
	{
        u1MJCLR_IN1 = MJC_READ_FLD(MJC_STA_LR3D, MJC_STA_LR3D_INFO_I);
	    u1MJCStatusCheck = 2;
		fg = 0;
		Printf("u1MJCLR_IN:%d \n ", u1MJCLR_IN1);
	}
}

void vDrvMJCInOutCRCCheck(void *pvArgv)
{
    static UINT32 _u4MJCCRC_OUT1, _u4MJCCRC_OUT2;
    static UINT32 _u4MJCCRC_IN1, _u4MJCCRC_IN2 ;
    UINT32 u4MJCCRC_OUT = 0;
    UINT32 u4MJCCRC_IN = 0;
    if ((u1MJCStatusCheck == 1) || (u1MJCStatusCheck == 6))
   	{
        u4MJCCRC_IN = MJC_READ_FLD(MJC_STA_CRC_2, MJC_CRC_OUT_1);
        u4MJCCRC_OUT = MJC_READ_FLD(MJC_STA_CRC_1, MJC_CRC_OUT_0);
		if ((u4MJCCRC_OUT != _u4MJCCRC_OUT1) && (u4MJCCRC_OUT != _u4MJCCRC_OUT2))
        {
            u1MJCStatusCheck = 0;
			printf("MJC Output CRC is Error \n");
        }
        if ((u4MJCCRC_IN != _u4MJCCRC_IN1) && (u4MJCCRC_IN != _u4MJCCRC_IN2))
        {
                       
            u1MJCStatusCheck = 0;
			printf("MJC Output CRC is Error \n");
        }
		if (u1MJCStatusCheck == 6)
		{
            u1MJCStatusCheck = 0;
			printf("MJC Output CRC Check interrupt \n");
		}
	}
    if (u1MJCStatusCheck == 2)
    {
        _u4MJCCRC_OUT2 = MJC_READ_FLD(MJC_STA_CRC_1, MJC_CRC_OUT_0);
        _u4MJCCRC_IN2 = MJC_READ_FLD(MJC_STA_CRC_2, MJC_CRC_OUT_1);
        u1MJCStatusCheck = 1;
    }
	if (u1MJCStatusCheck == 3)
    {
        _u4MJCCRC_OUT1 = MJC_READ_FLD(MJC_STA_CRC_1, MJC_CRC_OUT_0);
        _u4MJCCRC_IN1 = MJC_READ_FLD(MJC_STA_CRC_2, MJC_CRC_OUT_1);
        u1MJCStatusCheck = 2;
    }  
   
    Printf("u1MJCRC_IN:%d      u4MJCCRC_OUT:%d  \n", u4MJCCRC_IN , u4MJCCRC_OUT);
}

void vDrvMJCDramOvfUdfPolling(void *pvArgv)
{
    UINT32 u4StaFB01, u4StaFB02, u4StaSYS00;

    if (u1DrvMJCChkDramOvfUdf(&u4StaFB01, &u4StaFB02, &u4StaSYS00))
    {
        Printf("\nvDrvMJCDramOvfUdfPolling is stopped! There are bandwidth issues!\n");
        Printf("Remaining Polling Count = %d\n", u4MJCStatusPollingCount);
        Printf("Current DRAM %s Mode\n", (arMJCPrm.u1Dram444 ? "444" : "42x"));
        if (u4StaSYS00 & Fld2Msk32(MJC_STA_INIT_UNDER))
        {
            Printf("Issue Flag: MJC_STA_INIT_UNDER\n");
        }
        if (u4StaSYS00 & Fld2Msk32(MJC_STA_PROC_UNDER))
        {
            Printf("Issue Flag: MJC_STA_PROC_UNDER\n");
        }
        
        if (u4StaFB01)
        {
            Printf("Issue Flag: STA_FB_01 = 0x%08X\n", u4StaFB01);
        }
        if (u4StaFB02)
        {
            Printf("Issue Flag: STA_FB_02 = 0x%08X\n", u4StaFB02);
        }


        u4MJCStatusPollingCount = 1;
        return;
    }

    if ((u4MJCStatusPollingCount & 0x1F) == 0)
    {
        Printf(".");
    }

    if (u4MJCStatusPollingCount == 1)
    {
        Printf("\nvDrvMJCDramOvfUdfPolling finished. No Ovf/Udf is found\n");
        Printf("Current DRAM %s Mode\n", (arMJCPrm.u1Dram444 ? "444" : "42x"));
        vDrvMJCSetMVSortOnOff(SV_ON);
    }
}

UINT32 u4DrvMJCGetVDist(void)
{
    return MJC_READ_FLD(MJC_STA_FM_01, MJC_STA_FRM_VSYNC_IN_EARLY);
}

void vDrvMJCSetMuteColor(UINT8 u1R, UINT8 u1G, UINT8 u1B)
{
    MJC_WRITE_FLD_MULTI(MJC_SYS_MUTE_0,
        P_Fld(u1R, MJC_MUTE_R)|P_Fld(u1G, MJC_MUTE_G)|P_Fld(u1B, MJC_MUTE_B));
}

void vDrvMJCSetOutMuteOnOff(BOOL fgOnOff)
{
    LOG(3, "MJC Output Data Mute: %d\n", fgOnOff);

    if (fgOnOff)
    {
        MJC_WRITE_FLD(MJC_SYS_MUTE_0, 2, MJC_MUTE_1TYPE);
    }
    else
    {
        MJC_WRITE_FLD(MJC_SYS_MUTE_0, 0, MJC_MUTE_1TYPE);
    }

    MJC_WRITE_FLD(MJC_SYS_MUTE_1, SV_OFF, MJC_MUTE_SET);
    MJC_WRITE_FLD(MJC_SYS_MUTE_1, SV_ON, MJC_MUTE_SET);
}

void vDrvMJCSetAutoMute(UINT8 u1Count)
{
    vDrvMJCSetOutMuteOnOff(SV_ON);
    vDrvMJCDelaySet(E_MJC_DELAY_UNMUTE, u1Count);
    x_thread_delay(20);
}

void vDrvMJCEnablePatGen(UINT32 u4InOut, UINT32 u4OnOff)
{
    if (u4InOut == 0)
    {
        MJC_WRITE_FLD(MJC_SYS_0A, arMJCPrm.arFRCConfig.u13DMode == E_VDO_MJC_FS ? 1 : 0, MJC_PTG_3D_MODE);
        MJC_WRITE_FLD(MJC_SYS_0F, u4OnOff, MJC_PTG_PAT_EN);
    }
    else
    {
        MJC_WRITE_FLD(MJC_SYS_0A, arMJCPrm.arFRCConfig.u13DMode != E_VDO_MJC_2D ? (arMJCPrm.arFRCConfig.u1OutFrmRate >= 192 ? 2 : 1) : 0, MJC_PTG_3D_MODE);
        MJC_WRITE_FLD(MJC_SYS_0E, u4OnOff, MJC_PTG2_PAT_EN);
    }
}

static UINT32 _u4PrevCRC0 = 0;
static UINT32 _u4PrevCRC1 = 0;
void vDrvMJCDelaySet(E_MJC_DELAY_SET eDelayType, UINT8 u1Count)
{
    UINT32 u4DelayType = eDelayType;

    _u1DelaySetCnt[eDelayType] = u1Count;

    switch (u4DelayType)
    {
    case E_MJC_DELAY_DM_BW_CHK:
        vDrvMJCClearDramStatus();
        break;
    case E_MJC_DELAY_DM_CRC_CHK:
        _u4PrevCRC0 = MJC_READ_FLD(MJC_STA_CRC_1, MJC_CRC_OUT_0);
        _u4PrevCRC1 = MJC_READ_FLD(MJC_STA_CRC_2, MJC_CRC_OUT_1);
        break;
    }
}

void vDrvMJCDelaySetProc(void)
{
    if (_u1DelaySetCnt[E_MJC_DELAY_UNMUTE])
    {
        #if 0
        if (u1DrvMJCChkDramOvfUdf(NULL, NULL))
        {
            vDrvMJCClearDramStatus();
        }
        else
        #endif
        {
            _u1DelaySetCnt[E_MJC_DELAY_UNMUTE]--;
            if (_u1DelaySetCnt[E_MJC_DELAY_UNMUTE] == 0)
                vDrvMJCSetOutMuteOnOff(SV_OFF);
        }
    }
}

UINT8 u1DrvMJCGetMuteCount(void)
{
    return _u1DelaySetCnt[E_MJC_DELAY_UNMUTE];
}

void vDrvMJCSetMuteCount(UINT32 u4MuteCount)
{
    _u1DelaySetCnt[E_MJC_DELAY_UNMUTE] = (UINT8)u4MuteCount;

    if (u4MuteCount == 0)
    {
        vDrvMJCSetOutMuteOnOff(SV_OFF);
    }
    else
    {
        vDrvMJCSetOutMuteOnOff(SV_ON);
        u1TestDelayCnt = (UINT8)u4MuteCount;
    }
}

void vDrvMJCSetDemoCfg(const MJC_DEMO_CFG_T* prDemo)
{
    arMJCDemo = (*prDemo);
    vDrvMJCUpdateIntpCtrl();
}

static void _vDrvMJCChgCoreClock(void)
{
    /*static BOOL fgPrevClk = 0;
    BOOL fgCurrClk = IO32ReadFldAlign(MJC_MISC_00, MJC_CORE_CLK_SW);

    if (fgCurrClk != fgPrevClk)
    {
        vDrvMJCSetCoreClock(fgCurrClk);
        fgPrevClk = fgCurrClk;
    }*/
}

void vDrvMJCMainloop(void)
{
    if (fgMJCModeChg)
    {
        vDrvMJCModeChgProc(SV_VP_MAIN);
        fgMJCModeChg = SV_FALSE;
    }

    if (IO32ReadFldAlign(MJC_MISC_00, MJC_MODE_CHG_TRIG))
    {
        vDrvMJCSetFrmFreeze(SV_OFF);
        vDrvMJCSetDRAMAccessOnOff(SV_OFF, 0);
        vDrvMJCSetSystemMode(arMJCDramAlloc.u2Mode);
        vDrvMJCSetDRAMAccessOnOff(SV_ON, arMJCPrm.u1Dram444);
        vIO32WriteFldAlign(MJC_MISC_00, SV_OFF, MJC_MODE_CHG_TRIG);
    }

    if (u1ModeChangeRst)
    {
        u1ModeChangeRst --;
        if (u1ModeChangeRst == 0)
        {
            MJC_WRITE_FLD(MJC_SYS_00, 0x18, MJC_RST_CTRL);
            MJC_WRITE_FLD(MJC_SYS_00, 0x00, MJC_RST_CTRL);
            //Printf("Reset MJC_RST_CTRL\n");
        }
    }
	
    vDrvMJCDelaySetProc();

    vDrvMJCFbckModeChg();

    _vDrvMJCChgCoreClock();
}

void vDrvMJCFrmDlyChkOnOff(UINT32 u4OnOff)
{
    fgMJCFrmDlyChk = u4OnOff;
}

void vDrvMJCFrmDlyChk(UINT32 fgInVsync)
{
    static UINT32 u4Index = 0;
    static UINT32 fg1stFrm = SV_FALSE;
    static UINT32 u4Prev1CRCOut0, u4Prev1CRCOut1;
    static UINT32 u4Prev2CRCOut0, u4Prev2CRCOut1;
    UINT32 u4CRCOut0 = 0, u4CRCOut1 = 0;

    if (fgMJCFrmDlyChk == SV_OFF)
    {
        return;
    }
    
    if (fgInVsync == SV_TRUE)
    {
        if (u4Index == 0)
        {
            MJC_WRITE_FLD_MULTI(MJC_SYS_0F, 
                P_Fld(SV_OFF, MJC_PTG_PAT_EN) |
                P_Fld(SV_OFF, MJC_PTG_FREERUN_EN));
            Printf("\n");
        }
        else if (u4Index == 9)
        {
            MJC_WRITE_FLD(MJC_SYS_0F, SV_ON, MJC_PTG_PAT_EN);
            Printf("==========================\n");
        }
        u4Index ++;
        fg1stFrm = SV_TRUE;
    }
    else
    {
        if (u4Index > 0)
        {
            u4CRCOut0 = MJC_READ_4B(MJC_STA_CRC_1);
            u4CRCOut1 = MJC_READ_4B(MJC_STA_CRC_2);
            Printf("%02d, 0x%08X, 0x%08X\n", u4Index, u4CRCOut0, u4CRCOut1);
        }
        
        if ((u4Index > 10) && (u4CRCOut0 == u4Prev1CRCOut0) && (u4CRCOut1 == u4Prev1CRCOut1) &&
            (u4CRCOut0 == u4Prev2CRCOut0) && (u4CRCOut1 == u4Prev2CRCOut1))
        {
            Printf("Frame Delay = %d.%d\n", (u4Index - 11), (fg1stFrm == SV_FALSE ? 5 : 0));
            fgMJCFrmDlyChk = SV_OFF;
            u4Index        = 0;
            MJC_WRITE_FLD_MULTI(MJC_SYS_0F, 
                P_Fld(SV_OFF, MJC_PTG_PAT_EN) |
                P_Fld(SV_OFF, MJC_PTG_FREERUN_EN));
        }
        
        u4Prev2CRCOut0 = u4Prev1CRCOut0;
        u4Prev2CRCOut1 = u4Prev1CRCOut1;
        u4Prev1CRCOut0 = u4CRCOut0;
        u4Prev1CRCOut1 = u4CRCOut1;

        if (fg1stFrm == SV_TRUE)
        {
            fg1stFrm = SV_FALSE;
        }
    }
}

void vDrvMJCDramModeAutoTestCheck(void)
{
    UINT32 u4Crc0 = 0, u4Crc1 = 0;
    UINT32 u4StaFB01 = 0, u4StaFB02 = 0, u4StaSYS00 = 0;

    if (_u1DelaySetCnt[E_MJC_DELAY_DM_START] == 0)
    {
        if (_u1DelaySetCnt[E_MJC_DELAY_DM_CRC_CHK])
        {
            _u1DelaySetCnt[E_MJC_DELAY_DM_CRC_CHK] --;
            
            u4Crc0 = MJC_READ_FLD(MJC_STA_CRC_1, MJC_CRC_OUT_0);
            u4Crc1 = MJC_READ_FLD(MJC_STA_CRC_2, MJC_CRC_OUT_1);

            if ((u4Crc0 != _u4PrevCRC0) || (u4Crc1 != _u4PrevCRC1))
            {
                Printf("Current CRC = (0x%08x, 0x%08x)\n", u4Crc0, u4Crc1);
                Printf("Previous CRC = (0x%08x, 0x%08x)\n", _u4PrevCRC0, _u4PrevCRC1);
                _u1DelaySetCnt[E_MJC_DELAY_DM_CRC_CHK] = 0;
            }
            else
            {
                if (_u1DelaySetCnt[E_MJC_DELAY_DM_CRC_CHK] == 0)
                {
                    Printf("CRC OK.\n");
                }
            }

            _u4PrevCRC0 = u4Crc0;
            _u4PrevCRC1 = u4Crc1;
        }

        if (_u1DelaySetCnt[E_MJC_DELAY_DM_BW_CHK])
        {
            _u1DelaySetCnt[E_MJC_DELAY_DM_BW_CHK] --;
            if (u1DrvMJCChkDramOvfUdf(&u4StaFB01, &u4StaFB02, &u4StaSYS00))
            {
                Printf("=====MJC BW issue: MJC_STA_FB_01 = 0x%08X\n", u4StaFB01);
                Printf("=====MJC BW issue: MJC_STA_FB_02 = 0x%08X\n", u4StaFB02);
                Printf("=====MJC BW issue: MJC_STA_SYS_00 =0x%08X\n", u4StaSYS00);
                Printf("vDrvMJCDramOvfUdfPolling stopped! There are BW issues!\n");
                Printf("Current DRAM %s Mode\n", (arMJCPrm.u1Dram444 ? "444" : "42x"));
                _u1DelaySetCnt[E_MJC_DELAY_DM_BW_CHK] = 0;
            }
            else
            {
                if (_u1DelaySetCnt[E_MJC_DELAY_DM_BW_CHK] == 0)
                {
                    Printf("BW Check OK.\n");
                }
            }
        }
    }
    else
    {
        _u1DelaySetCnt[E_MJC_DELAY_DM_START] --;
    }
}

static void _vDrvMJCBadRstPatch(void)
{
    UINT32 u4FdInfo = MJC_READ_FLD(MJCFD_08, MJCFD_FD_INFO);

    if ((u4FdInfo >= 1) && (u4FdInfo <= 5))
    {
        MJC_WRITE_FLD(MJCFD_06, 1, MJCFD_C_FD_SEQ_BAD_RST);
    }
    else
    {
        MJC_WRITE_FLD(MJCFD_06, 0, MJCFD_C_FD_SEQ_BAD_RST);
    }
}

static void _vDrvMJCSpAdjInputVsync(void)
{
    if (_pfnMJCAdjInputVsync && u4MJCStatusPollingCount)
    {
        Printf("\n");
        (*_pfnMJCAdjInputVsync)(NULL);
        
        u4MJCStatusPollingCount --;
        if (u4MJCStatusPollingCount == 0)
        {
            _pfnMJCAdjInputVsync = NULL;
            Printf("\n");
        }
    }
}

static void _vDrvMJCSpInputVsync(void)
{
    if (_pfnMJCInputVsync && u4MJCStatusPollingCount)
    {
        Printf("\n");
        (*_pfnMJCInputVsync)(NULL);
        
        u4MJCStatusPollingCount --;
        if (u4MJCStatusPollingCount == 0)
        {
            _pfnMJCInputVsync = NULL;
            Printf("\n");
        }
    }
	if (_pfnMJCInputVsync && u1MJCStatusCheck)
    {
        Printf("\n");
        (*_pfnMJCInputVsync)(NULL);
		
        if (u1MJCStatusCheck == 0)
        {
            _pfnMJCInputVsync = NULL;
            Printf("\n");
        }
    }
}

static void _vDrvMJCSpOutputVsync(void)
{
    if (_pfnMJCOutputVsync && u4MJCStatusPollingCount)
    {
        if (_pfnMJCOutputVsync != vDrvMJCDramOvfUdfPolling)
            Printf("\n");
        
        (*_pfnMJCOutputVsync)(NULL);
        
        u4MJCStatusPollingCount --;
        if (u4MJCStatusPollingCount == 0)
        {
            _pfnMJCOutputVsync = NULL;
            Printf("\n");
        }
    }
	if (_pfnMJCOutputVsync && u1MJCStatusCheck)
    {
        Printf("\n");
        (*_pfnMJCOutputVsync)(NULL);
		
        if (u1MJCStatusCheck == 0)
        {
            _pfnMJCOutputVsync = NULL;
            Printf("\n");
        }
    }
}

static void _vDrvMJCSpProcFinished(void)
{
    if (_pfnMJCProcFinished && u4MJCStatusPollingCount)
    {
        Printf("\n");
        (*_pfnMJCProcFinished)(NULL);
        
        u4MJCStatusPollingCount --;
        if (u4MJCStatusPollingCount == 0)
        {
            _pfnMJCProcFinished = NULL;
            Printf("\n");
        }
    }
}

#if 0
static void _vDrvMJCProcFrmFreeze(void)
{
    static UINT8 _u1FrzMode = 0xFF;
    static UINT8 _u1UnFrzCnt = 18;    

    if (arMJCPrm.fgFreeze != _u1FrzMode)
    {
        if (arMJCPrm.fgFreeze == SV_ON)
        {
            if (MJC_READ_FLD(MJC_STA_FM_02, MJC_STA_MEMC_NUM_HW) == 0 &&
                MJC_READ_FLD(MJC_STA_FM_02, MJC_STA_ME_FILM_FLG_HW) == 0)
            {
                MJC_WRITE_FLD(MJC_MC_01, 0, MJC_MC_INTP_NUM);
                MJC_WRITE_FLD(MJC_SYS_00, 1, MJC_FRM_FREEZE);
                _u1FrzMode = arMJCPrm.fgFreeze;
                _u1UnFrzCnt = 60;
            }    
        }
        else if (arMJCPrm.fgFreeze == SV_OFF)
        {
            MJC_WRITE_FLD(MJC_SYS_00, 0, MJC_FRM_FREEZE);
            if (--_u1UnFrzCnt == 0)
            {
                vDrvMJCSetPsAutoTrigOn();
                _u1FrzMode = arMJCPrm.fgFreeze;
            }
        }
    }
}
#endif

void vDrvMJCImportProtection(UINT8 u1OnOff)
{
    /*if (u1OnOff)
    {
        MJC_WRITE_FLD(MJC_SYS_D4, SV_ON, MJC_OUT_TOTAL_MATCH_SET);//[31:24]in 8283
    }
    else
    {
        MJC_WRITE_FLD(MJC_SYS_D4, SV_OFF, MJC_OUT_TOTAL_MATCH_SET);
    }
	*/
}

void vDrvMJCFireImportProtection(void)
{
    /*
    MJC_WRITE_FLD(MJC_SYS_D4, SV_ON, MJC_SET_DATA_ONE_FRM);
    MJC_WRITE_FLD(MJC_SYS_D4, SV_OFF, MJC_SET_DATA_ONE_FRM);
	*/
}

void vDrvMJCSwitchFrameTrackOnOff(UINT32 u4OnOff)
{
    static UINT32 u4PrevOnOffState = 0xFF;

    if (u4PrevOnOffState == u4OnOff)
    {
        return;
    }

    u4PrevOnOffState = u4OnOff;
    
    if (u4OnOff)
    {    
        MJC_WRITE_FLD(MJC_SYS_DDDS, SV_ON, MJC_DDDS_TRK_EN);
    }
    else
    {
        MJC_WRITE_FLD(MJC_SYS_DDDS, SV_OFF, MJC_DDDS_TRK_EN);        
    }
}

void vDrvMJCSetFrmMask(UINT32 u4FrmMaskI, UINT32 u4FrmMaskO)
{
    /*
    {
        MJC_WRITE_FLD(MJC_FM_REV1, u4FrmMaskI, MJC_FRM_MASK_I_ECO);
        MJC_WRITE_FLD(MJC_FM_TRK, u4FrmMaskO, MJC_FRM_MASK_O);
    }
    */
	
    {
        MJC_WRITE_FLD_MULTI(MJC_FM_TRK,
            P_Fld(u4FrmMaskI, MJC_FRM_MASK_I) |
            P_Fld(u4FrmMaskO, MJC_FRM_MASK_O));
    }
}

UINT8 u1DrvMJCIsOnOff(void)
{
    return arMJCPrm.u1OnOff != E_MJC_BYPASS;
}

void vDrvMJCSetFrmFreeze(UINT32 u4OnOff)
{
    arMJCPrm.fgFreeze = (UINT8)u4OnOff;

    MJC_WRITE_FLD(MJC_FM_07, arMJCPrm.arFRCConfig.u13DMode ? u4OnOff : SV_OFF, MJC_FREEZE_3D_EN);
    MJC_WRITE_FLD(MJC_SYS_00, u4OnOff, MJC_FRM_FREEZE);
    vDrvMJCSetPSAutoOnOff(u4OnOff);
}

void vDrvMJCISR(UINT16 u2Vector)
{
    UINT8    u1StaInt;
    ASSERT(u2Vector == VECTOR_MJC);
    u1StaInt = MJC_READ_FLD(MJC_STA_SYS_00, MJC_STA_INT);

    if (u1StaInt & MJC_INT_0) // Adjusted input vsync end
    {
        // status polling
        _vDrvMJCSpAdjInputVsync();
#ifdef CC_MT5396
        vDrvADLProc(); 
#endif
    }

    if (u1StaInt & MJC_INT_2) // Input vsync end
    {        
        vScpipOnMJCInputVSync();
        vDrvMJCFrmSchInputVsyncProc();
        _vDrvMJCBadRstPatch();
        vDrvMJCSetPsAutoOnProc();
        vApiApplyVideoEvent(PEQTY_APPLY_TIME_MJC_INPUT_VSYNC_ISR);  
        vDrvMJCFrmDlyChk(SV_TRUE);
        
        // status polling
        _vDrvMJCSpInputVsync();
		
    }


    if (u1StaInt & MJC_INT_1) // Output vsync end
    {        
        #if 0
        TIME_StopMeasure(0);
        TIME_PrintElapsedTime(0);
        TIME_StartMeasure(0);
        #endif
        
        //vScpipOnMjcOutVsync();
        
        
        vDrvTTDSetQualityISR();
        
        // for verification of DRAM mode
        //vDrvMJCDramModeAutoTestCheck();

        vDrvMJCFrmDlyChk(SV_FALSE);

        // MJC freeze
        //_vDrvMJCProcFrmFreeze();
        vDrvMJCSetQualityISR();

        // status polling
        _vDrvMJCSpOutputVsync();
		vDrvMJCFrmSchOutputVsyncProc();
    }


    if (u1StaInt & MJC_INT_3) // MJC frame processing finished
    {
        //vDrvMJCSetQualityISR();

        // status polling
        _vDrvMJCSpProcFinished();
    }

    if (u1StaInt)
    {
        MJC_WRITE_FLD(MJC_SYS_02, u1StaInt, MJC_INT_CLEAR);
        MJC_WRITE_FLD(MJC_SYS_02, 0x0, MJC_INT_CLEAR);
    }
}

UINT8 u1DrvMJCGetFrmDelay(void)
{
    UINT8 u1Delay = 0;
    
    if (u1DrvMJCIsOnOff())
    {
        if (arMJCPrm.u1Dram444)
        {
            u1Delay = 2;
        }
        else
        {
            u1Delay = 7;
        }
    }

    return u1Delay;
}

UINT8 u1DrvMJCGetFrmDelay10x(void)
{
    UINT8 u1Delay = 0;
    
    if (u1DrvMJCIsOnOff())
    {
        if (arMJCDramAlloc.u2Mode & MJC_444_MODE)
        {
            u1Delay = 15;
        }
        else
        {
            u1Delay = 70;
        }
    }

    return u1Delay;
}


const MJC_DRAM_MODE_T * rDrvMJCGetDramMode(UINT32 u4DramModeIdx)
{
    if (u4DramModeIdx < MJC_DRAM_ALL_MODE_NUM)
    {
        return &_arMJCAllDramMode[u4DramModeIdx];
    }
    
    for (u4DramModeIdx = 0; u4DramModeIdx < MJC_DRAM_ALL_MODE_NUM; u4DramModeIdx++)
    {
        if (_arMJCAllDramMode[u4DramModeIdx].u2DramMode == arMJCDramAlloc.u2Mode)
            break;
    }

    return &_arMJCAllDramMode[u4DramModeIdx];
}

void vDrvMJCQueryInOutCtrl(void)
{
    Printf("MJC Input/Output Control\n\n");
    Printf("\tInput WxH HTxVT ........: (%dx%d, %dx%d)\n", MJC_READ_FLD(MJC_SYS_02, MJC_MJC_WIDTH), MJC_READ_FLD(MJC_SYS_02, MJC_MJC_HEIGHT_FX), MJC_READ_FLD(MJC_STA_SYS_01, MJC_STA_H_TOTAL) + 1, MJC_READ_FLD(MJC_STA_SYS_01, MJC_STA_V_TOTAL)+1);
    Printf("\tInput Start (H,V) ......: (%d, %d)\n", MJC_READ_FLD(MJC_SYS_01, MJC_IN_H_START), MJC_READ_FLD(MJC_SYS_01, MJC_IN_V_START));
    Printf("\tOutput WxH HTxVT .......: (%dx%d, %dx%d)\n", MJC_READ_FLD(MJC_SYS_04, MJC_OUT_WIDTH), MJC_READ_FLD(MJC_SYS_04, MJC_OUT_HEIGHT), MJC_READ_FLD(MJC_SYS_05, MJC_OUT_H_TOTAL), MJC_READ_FLD(MJC_SYS_05, MJC_OUT_V_TOTAL));
    Printf("\tOutput Start (H,V)......: (%d, %d)\n", MJC_READ_FLD(MJC_SYS_03, MJC_OUT_H_START), MJC_READ_FLD(MJC_SYS_03, MJC_OUT_V_START));
    Printf("\n");
}

void vDrvMJCQueryFrmSchCtrl(void)
{
    Printf("MJC Frame Schedule Control\n\n");
    #ifndef CC_FPGA
    //Printf("\tOCLK ...................: %d Hz\n", BSP_GetDomainClock(CAL_SRC_VOPLL));
    #endif
    Printf("\tInput Frame Rate .......: %d\n", arMJCPrm.arFRCConfig.u1InFrmRate);
    Printf("\tOutput Frame Rate ......: %d\n", arMJCPrm.arFRCConfig.u1OutFrmRate);
	Printf("\tPR Output Frame Rate ...: %d\n", arMJCPrm.arMJCPRConfig.u1OutPRFrameRate);
    Printf("\t(2X, 4X) Rate ..........: (%d, %d)\n", MJC_READ_FLD(MJC_FM_00, MJC_OUT_DB_RATE_EN), MJC_READ_FLD(MJC_FM_00, MJC_OUT_4X_RATE_EN));
    Printf("\tProgram Frame Rate .....: (%d, %d/%d)\n", MJC_READ_FLD(MJC_SYS_04, MJC_PROG_FRM_EN), MJC_READ_FLD(MJC_SYS_04, MJC_OUT_FRM_NO), MJC_READ_FLD(MJC_SYS_04, MJC_OUT_VSYNC_DIV) + 1);
    Printf("\tPsAuto(32,22) ..........: (%d, %d)\n", MJC_READ_FLD(MJC_FM_00, MJC_PSAUTO_32), MJC_READ_FLD(MJC_FM_00, MJC_PSAUTO_22));
    Printf("\tPdShif(32,22) ..........: (%d, %d)\n", MJC_READ_FLD(MJC_SYS_00, MJC_PD_INFO_32_SHIFT), MJC_READ_FLD(MJC_SYS_00, MJC_PD_INFO_22_SHIFT));
    Printf("\tIntp Mode (64, 55P2) ...: (%d, %d)\n", MJC_READ_FLD(MJC_FM_00, MJC_FALLBACK_64), MJC_READ_FLD(MJC_FM_00, MJC_FALLBACK_55P2));
    Printf("\n");
}

void vDrvMJCQueryDramCtrl(void)
{
    const MJC_DRAM_MODE_T * prDramMode;
    prDramMode = rDrvMJCGetDramMode(MJC_DRAM_MODE_QUERY);

    Printf("MJC DRAM Control\n\n");
    Printf("\tDRAM Mode ..............: %s(0x%02X)\n", prDramMode->strDramMode, prDramMode->u2DramMode);
    Printf("\tDATA Mode ..............: %s\n", MJC_READ_FLD(MJC_FB_01, MJC_DRAM_RPT) ? ((MJC_READ_FLD(MJC_FB_01, MJC_DRAM_RPT_422))? "RPT422":"RPT444") : (MJC_READ_FLD(MJC_FB_00, MJC_DRAM_420) ? "420" : "422"));
    Printf("\tY/C ....................: %s/%s\n", MJC_READ_FLD(MJC_FB_00, MJC_DRAM_Y_8BIT) ? "8-bit" : "10-bit", MJC_READ_FLD(MJC_FB_00, MJC_DRAM_C_8BIT) ? "8-bit" : "10-bit");
    Printf("\tDRAM Start Addr ........: 0x%X%06X0\n", MJC_READ_FLD(MJC_FB_0E, MJC_DRAM_BASE_ADDR_MSB), MJC_READ_FLD(MJC_FB_08, MJC_DRAM_BASE_ADDR));
    Printf("\n");
}

void vDrvMJCQuery3DCtrl(void)
{
    UINT8 u12D3DMode;
	u12D3DMode = IO32ReadFldAlign(MJC_PR_MODE, MJC_3DMODE_IN);                     
    Printf("MJC 3D Control\n\n");
    Printf("\tInput 3D Mode ..........: %s\n", (u12D3DMode != E_VDO_MJC_2D)? (u12D3DMode == E_VDO_MJC_FS ? "3D_FS" : (u12D3DMode == E_VDO_MJC_SBS ? "3D_SBS" : (u12D3DMode == E_VDO_MJC_TAB? "3D_TAB" : "3D_TTD"))) : "2D");
	Printf("\tPanel PR ...............: %d %s\n", IS_LVDS_DISP_3D_POLARIZED, (IS_LVDS_DISP_3D_POLARIZED) ? (LVDS_DISP_3D == LVDS_DISP_3D_POLARIZED_LRLR ? "LRLR" : "RLRL") : "");
    Printf("\tPanel SG ...............: %d\n", IS_LVDS_DISP_3D_SHUTTER);
    Printf("\t3D_MODE ................: %d\n", MJC_READ_FLD(MJC_FB_LR3D, MJC_3D_MODE));
    Printf("\n");
}


void vDrvMJCPRQueryCtrl(void)
{
    UINT8 u1MJCPRMode, u1MJCHPRMode, u1MJCFPRMode;
	u1MJCPRMode = u1DrvMJCGetPRMode();
	u1MJCHPRMode = u1DrvMJCGetHPRMode();
	u1MJCFPRMode = u1DrvMJCGetFPRMode();
	Printf("MJC PR MODE Control\n\n");
	Printf("\tTDTV PR ................: %d\n", IO32ReadFldAlign(MJC_PR_MODE, MJC_3DMODE_OUT));
	if(BSP_GetIcVersion() == IC_VER_5396_AA)
	{
	    Printf("\tPR Mode ................: %s\n", ((u1MJCPRMode == E_MJC_NoPR)?  "NO PR" : (u1MJCPRMode == E_MJC_HPR)? "HPR" :
			                                        (u1MJCPRMode == E_MJC_FPR)?  "FPR" : "UNKNOWN"));
    }
	else
	{
        Printf("\tPR Mode ................: %s\n", ((u1MJCPRMode == E_MJC_HPR)? "HPR" : "NO PR"));
    }
	Printf("\tHPR Type ...............: %s\n", ((u1MJCHPRMode == E_MJC_HPR2160)?   "HPR2160" : (u1MJCHPRMode == E_MJC_HPR120)? "HPR120" : 
		                                        (u1MJCHPRMode == E_MJC_HPR60)?  "HPR60" :(u1MJCHPRMode == E_MJC_HPR240)?  "HPR240" : "NOHPR"));
	Printf("\tFPR Type ...............: %s\n", ((u1MJCFPRMode == E_MJC_FPRV3)? "FPRV3" : (u1MJCFPRMode == E_MJC_FPRV2)? "FPRV2" : 
		                                        (u1MJCHPRMode == E_MJC_FPRV1)? "FPRV1" :
		                                        (u1MJCFPRMode == E_MJC_FPRV0)? "FPRV0" : "UNKNOWN"));
	Printf("\tHPR CEN_DCM.............: %d\n", MJC_READ_FLD(MJC_SYS_PR_0, MJC_PR_SYNC_REGEN_CEN_DCM));
	Printf("\tMJC PR 10bit ...........: %d\n", MJC_READ_FLD(MJC_PR_03, MJC_PR_10B));
	Printf("\tMJC 240LR_Mode .........: %d\n", MJC_READ_FLD(MJC_FM_04, MJC_240HZ_LR_MODE));
}

#ifdef CC_FPGA
#define TTDVP_ATPG 0 //added by luis for build pass
static void vChkRS232Sel(UINT32 u4Addr)
{
    if ((((MJC_MV_00 + IO_ADDR_OFST) <= u4Addr) && (u4Addr <= (MJC_STA_OHT_06 + IO_ADDR_OFST))) ||
        (((MJC_MC_00 + IO_ADDR_OFST) <= u4Addr) && (u4Addr <= (MJC_MC_ATPG + IO_ADDR_OFST))) ||
        (((TTD_00 + IO_ADDR_OFST) <= u4Addr) && (u4Addr <= (TTDVP_ATPG + IO_ADDR_OFST))) ||
        (((MJC_STA_SYS_00 + IO_ADDR_OFST) <= u4Addr) && (u4Addr <= (MJC_SYS_ATPG + IO_ADDR_OFST))) ||
        (u4Addr == (MJC_STA_LR3D + IO_ADDR_OFST)) ||
        (u4Addr == (MJC_STA_BG + IO_ADDR_OFST)))
    {
        MJC_WRITE_FLD(MJC_FPGA_00, SV_ON, MJC_RS232_SEL);
    }
}

UINT8 u1DrvMJCRead1B(UINT32 u4Addr)
{
    UINT8   u1Data;

    vChkRS232Sel(u4Addr);
    u1Data = u1IO32Read1B(u4Addr);
    MJC_WRITE_FLD(MJC_FPGA_00, SV_OFF, MJC_RS232_SEL);

    return u1Data;
}

UINT16 u2DrvMJCRead2B(UINT32 u4Addr)
{
    UINT16  u2Data;

    vChkRS232Sel(u4Addr);
    u2Data = u2IO32Read2B(u4Addr);
    MJC_WRITE_FLD(MJC_FPGA_00, SV_OFF, MJC_RS232_SEL);

    return u2Data;
}

UINT32 u4DrvMJCRead4B(UINT32 u4Addr)
{
    UINT32  u4Data;

    vChkRS232Sel(u4Addr);
    u4Data = u4IO32Read4B(u4Addr);
    MJC_WRITE_FLD(MJC_FPGA_00, SV_OFF, MJC_RS232_SEL);

    return u4Data;
}

UINT32 u4DrvMJCReadFldAlign(UINT32 u4Addr, UINT32 u4Fld)
{
    UINT32  u4Data;

    vChkRS232Sel(u4Addr);
    u4Data = IO32ReadFldAlign(u4Addr, u4Fld);
    MJC_WRITE_FLD(MJC_FPGA_00, SV_OFF, MJC_RS232_SEL);

    return u4Data;
}
#endif

