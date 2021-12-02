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

/*****************************************************************************
 *
 * Filename:
 * ---------
 *   $Workfile: scaler_table.c $ 
 *
 * Project:
 * --------
 *   MT8222
 *
 * Description:
 * ------------
 *   Driver for tuning scaler dispmode
 *   
 * Author:
 * -------
 *   Skywalker Yin
 *
 * $Modtime: 04/05/27 1:17p $  
 *
 * $Revision: #1 $
****************************************************************************/

/**************************************************************************  
 * Inclusions
 *************************************************************************/

/**************************************************************************  
 * Constant/Configure Definitions
 *************************************************************************/

/**************************************************************************  
 * Type definitions
 *************************************************************************/
 
/**************************************************************************  
 * Macro definitions
 *************************************************************************/
 
/**************************************************************************  
 * Imported functions (optional)
 *************************************************************************/

/**************************************************************************  
 * Imported variables (optional)
 *************************************************************************/

/**************************************************************************  
 * Global/Static variables
 *************************************************************************/


/**************************************************************************  
 * Function Members
 *************************************************************************/

#define _VGA_TABLE_C_
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif
#include "general.h"
#include "panel.h"
#include "drv_scaler_table.h"


#if 0//OLD_NONLINEAE_US_ONOFF
NONLINEAR_SCALING_PRM_T NonlinearTbl[PANEL_MAX][2] =
{   
    //(TV_WIDTH==1440 && TV_HEIGHT==900)
    {
        {NONLINEAR_H_640, 0X2A00,0X80,0X9}, //to test    
        {NONLINEAR_H_720, 0X31D0,0X80,0X9}, //to test    
    },
    //(TV_WIDTH==1680 && TV_HEIGHT==1050)
    {
        {NONLINEAR_H_640, 0X2090,0X80,0X9}, //OK
        {NONLINEAR_H_720, 0X1970,0X80,0Xa}, //OK
    },
    //(TV_WIDTH==1366 && TV_HEIGHT==768)
    {     
        {NONLINEAR_H_640, 0X2000,0X3d,0Xb}, //OK
        {NONLINEAR_H_720, 0X14A0,0X80,0Xb}, //For CR[DTV00124682]Video display abnormally when aspect set to "panoramic".   
    },
    //(TV_WIDTH==1920 && TV_HEIGHT==1080)
    {     
        {NONLINEAR_H_640, 0X18F0,0X80,0X9}, //to test
        {NONLINEAR_H_720, 0x1E20,0X80,0X9}, //to test   
    },
    //other
    {     
        {NONLINEAR_H_640, 0X2A00,0X80,0X9},
        {NONLINEAR_H_720, 0X1500,0X80,0Xb},    
    }

};
#endif
////////////////////////////////////////////////////////////////////////////////
SCALER_DISPMODE_PRM_T* ScalerDispmodeTbl;

SCALER_DISPMODE_PRM_T ScalerDispmodeTbl_1440_900[] =
{     
    {SCALER_DISPMODE_DOWN_SCALING_MODE        ,0,0x11331211,0,1,0,2,0,0},
    {SCALER_DISPMODE_UP_SCALING_MODE          ,0,0x11441251,0,1,0,3,0,0},
//    {SCALER_DISPMODE_OFF_MODE ,2,0x11771254,0,0,0,4,0,0},
    {SCALER_DISPMODE_OFF_MODE ,2,0x11771452,0,0,0,4,0,0},    
#ifdef MT5387_TODO
    //640*480P
    {MODE_0640x0480x60HZ_95   ,0,0x11441251,0,1,0,3,0,0xF8},
    //480P
    {MODE_0720x0480x60HZ_83   ,0,0x11441251,0,1,0,3,0,0x15C},
    //480p oversample
    {MODE_1440x0480x60HZ_81   ,0,0x11441251,0,1,0,3,0,0x1D0},
     //576P
    {MODE_0720x0576x50HZ_84   ,0,0x11441251,0,1,0,3,0,0xFC},
    //720P
    {MODE_1280x0720x60HZ_86   ,0,0x11441251,0,1,0,3,0x3,0x0},
    {MODE_1280x0720x50HZ_85   ,0,0x11441251,0,1,0,3,0x3,0x0},    
    //1080i
    {MODE_1920x1080x50HZ_88   ,0,0x11331211,0,0,0,2,0,0xB1},
    {MODE_1920x1080x60HZ_89   ,0,0x11331211,0,0,0,2,0,0xB1},    
    //1080P
    {MODE_1920x1080x50HZ_93   ,0,0x11331211,0,0,0,2,0,0x0},
    {MODE_1920x1080x60HZ_94   ,0,0x11331211,0,0,0,2,0,0x0},    
    //640x480
    {MODE_0640x0480x60HZ_08   ,0,0x11441251,0,1,0,3,0,0xF4},
    {MODE_0640x0480x72HZ_21   ,0,0x11441251,0,1,0,3,0,0xFC},
    {MODE_0640x0480x75HZ_22   ,0,0x11441251,0,1,0,3,0,0x128},
    //800x600
    {MODE_0800x0600x56HZ_26   ,0,0x11441251,0,1,0,3,0,0x94},    
    {MODE_0800x0600x60HZ_27   ,0,0x11441251,0,1,0,3,0,0x5C},
    {MODE_0800x0600x72HZ_29   ,0,0x11441251,0,1,0,3,0,0},
    {MODE_0800x0600x75HZ_30   ,0,0x11441251,0,1,0,3,0,0x9C},    
    //1024x768
    {MODE_1024x0768x60HZ_35   ,0,0x11441251,0,1,0,3,0,0x60},
    {MODE_1024x0768x70HZ_39   ,0,0x11441251,0,1,0,3,0,0x60},    
    {MODE_1024x0768x75HZ_41   ,0,0x11441251,0,1,0,3,0,0x68},    
    //1152x864
    {MODE_1152x0864x75HZ_53   ,0,0x11441251,0,1,0,3,0,0x4C},    
    //1280x768
    {MODE_1280x0768x60HZ_60   ,0,0x11441251,0,1,0,3,0,0x34},
    {MODE_1280x0768x75HZ_68   ,0,0x11441251,0,1,0,3,0,0x6C},
    //1280x960    
    {MODE_1280x0960x60HZ_61   ,0,0x11331211,0,0,0,2,0,0x4},    
    //1280x1024
    {MODE_1280x1024x60HZ_64   ,0,0x11341211,0,0,0,2,0,0x8},        
    {MODE_1280x1024x75HZ_66   ,0,0x11341211,0,0,0,2,0,0x8},        
    //1360x768
    {MODE_1360x0768x60HZ_33   ,0,0x11441251,0,1,0,3,0,0},        
    //1440x900
    {MODE_1440x0900x60HZ_50   ,0,0x11331211,0,1,0,2,0,0},      
    //1600x1200
    {MODE_1600x1200x60HZ_70   ,0,0x11331211,0,1,0,2,0,0},     
#endif
};

SCALER_DISPMODE_PRM_T ScalerDispmodeTbl_1680_1050[] =
{     
    {SCALER_DISPMODE_DOWN_SCALING_MODE        ,0,0x11331211,0,1,0,2,0,0},
    {SCALER_DISPMODE_UP_SCALING_MODE          ,0,0x11441251,0,1,0,3,0,0},
    {SCALER_DISPMODE_OFF_MODE ,2,0x11771254,0,0,0,4,0,0},
#ifdef MT5387_TODO
    //720P
    {MODE_1280x0720x60HZ_86   ,0,0x11441251,0,1,0,3,0,0x9C},
    {MODE_1280x0720x50HZ_85   ,0,0x11441251,0,1,0,3,0,0x84},    
    //1080P
    {MODE_1920x1080x50HZ_93   ,0,0x11441251,0,1,0,3,0,0x14},
    {MODE_1920x1080x60HZ_94   ,0,0x11441251,0,1,0,3,0,0x14},    
    //640x480
    {MODE_0640x0480x60HZ_08   ,0,0x11441251,0,1,0,3,0,0x188},
    {MODE_0640x0480x72HZ_21   ,0,0x11441251,0,1,0,3,0,0x1CC},
    {MODE_0640x0480x75HZ_22   ,0,0x11441251,0,1,0,3,0,0x1AC},
    //800x600
    {MODE_0800x0600x56HZ_26   ,0,0x11441251,0,1,0,3,0,0x104},    
    {MODE_0800x0600x60HZ_27   ,0,0x11441251,0,1,0,3,0,0xD8},
    {MODE_0800x0600x72HZ_29   ,0,0x11441251,0,1,0,3,0,0x114},
    {MODE_0800x0600x75HZ_30   ,0,0x11441251,0,1,0,3,0,0x10C},    
    //1024x768
    {MODE_1024x0768x60HZ_35   ,0,0x11441251,0,1,0,3,0,0x84},
    {MODE_1024x0768x70HZ_39   ,0,0x11441251,0,1,0,3,0,0x84},    
    {MODE_1024x0768x75HZ_41   ,0,0x11441251,0,1,0,3,0,0x9C},    
    //1152x864
    {MODE_1152x0864x75HZ_53   ,0,0x11441251,0,1,0,3,0,0x68},    
    //1280x768
    {MODE_1280x0768x60HZ_60   ,0,0x11441251,0,1,0,3,0,0x7C},
    {MODE_1280x0768x75HZ_68   ,0,0x11441251,0,1,0,3,0,0xA4},
    //1280x960    
    {MODE_1280x0960x60HZ_61   ,0,0x11441251,0,1,0,3,0,0x48},    
    //1280x1024
    {MODE_1280x1024x60HZ_64   ,0,0x11441251,0,1,0,3,0,0x1C},        
    {MODE_1280x1024x75HZ_66   ,0,0x11441251,0,1,0,3,0,0x1C},        
    //1360x768
    {MODE_1360x0768x60HZ_33   ,0,0x11441251,0,1,0,3,0,0x88},        
    //1440x900
    {MODE_1440x0900x60HZ_50   ,0,0x11441251,0,1,0,3,0,0x30},            
    //1600x1200
    {MODE_1600x1200x60HZ_70   ,0,0x11331211,0,0,0,2,0,0xC},    
#endif
};

SCALER_DISPMODE_PRM_T ScalerDispmodeTbl_1366_768[] =
{     
    {SCALER_DISPMODE_DOWN_SCALING_MODE        ,0,0x11331211,0,1,0,2,0,0},        
    {SCALER_DISPMODE_UP_SCALING_MODE          ,0,0x11441251,0,1,0,3,0,0},    
    {SCALER_DISPMODE_OFF_MODE ,2,0x11771254,0,0,0,4,0,0},
#ifdef MT5387_TODO
//    {MODE_0640x0480x60HZ_06   ,0,0x11441251,0,1,0,3,0,0x9c},
//    {MODE_0640x0480x60HZ_08   ,0,0x11441251,0,1,0,3,0,0x9c},    
     //640*480P
    {MODE_0640x0480x60HZ_95   ,0,0x11441251,0,1,0,3,0,0xA4},
     //480P
    {MODE_1440x0480x60HZ_81   ,0,0x11441251,0,1,0,3,0,0xA8},     
    {MODE_0720x0480x60HZ_83   ,0,0x11441251,0,1,0,3,0,0xE8},
     //576P
    {MODE_1440x0576x50HZ_82   ,0,0x11441251,0,1,0,3,0,0x7C},     
    {MODE_0720x0576x50HZ_84   ,0,0x11441251,0,1,0,3,0,0xA4},
    //720P
    {MODE_1280x0720x60HZ_86   ,0,0x11441251,0,0,0,3,0x3,0x0},
    {MODE_1280x0720x50HZ_85   ,0,0x11441251,0,0,0,3,0x3,0x0},        
    //1080P
//    {MODE_1920x1080x50HZ_93   ,0,0x11331211,0,1,0,2,0,0x0},
//    {MODE_1920x1080x60HZ_94   ,0,0x11331211,0,1,0,2,0,0x0},    
    //640x480
    {MODE_0640x0480x60HZ_08   ,0,0x11441251,0,1,0,3,0,0x25C},
//    {MODE_0640x0480x72HZ_21   ,0,0x11441251,0,1,0,3,0,0},
//    {MODE_0640x0480x75HZ_22   ,0,0x11441251,0,1,0,3,0,0},
    //800x600
    {MODE_0800x0600x56HZ_26   ,0,0x11441251,0,1,0,3,0,0x54},    
    {MODE_0800x0600x60HZ_27   ,0,0x11441251,0,1,0,3,0,0x58},
//    {MODE_0800x0600x72HZ_29   ,0,0x11441251,0,1,0,3,0,0},
//    {MODE_0800x0600x75HZ_30   ,0,0x11441251,0,1,0,3,0,0},    
    //1024x768
//    {MODE_1024x0768x60HZ_35   ,0,0x11441251,0,1,0,3,0,0},
//    {MODE_1024x0768x70HZ_39   ,0,0x11441251,0,1,0,3,0,0},    
//    {MODE_1024x0768x75HZ_41   ,0,0x11441251,0,1,0,3,0,0},    
    //1152x864
//    {MODE_1152x0864x75HZ_53   ,0,0x11331211,0,1,0,2,0,0},    
    //1280x768
//    {MODE_1280x0768x60HZ_60   ,0,0x11441251,0,1,0,3,0,0},
//    {MODE_1280x0768x75HZ_68   ,0,0x11441251,0,1,0,3,0,0},
    //1280x960    
//    {MODE_1280x0960x60HZ_61   ,0,0x11331211,0,1,0,2,0,0},    
    //1280x1024
//    {MODE_1280x1024x60HZ_64   ,0,0x11331211,0,1,0,2,0,0x10},        
//    {MODE_1280x1024x75HZ_66   ,0,0x11331211,0,1,0,2,0,0},        
    //1360x768
//    {MODE_1360x0768x60HZ_33   ,0,0x11331211,0,1,0,2,0,0},        
    //1440x900
//    {MODE_1440x0900x60HZ_50   ,0,0x11331211,0,1,0,2,0,0},         
    //1600x1200
//    {MODE_1600x1200x60HZ_70   ,0,0x11331211,0,1,0,2,0,0}, 
#endif
    
};

SCALER_DISPMODE_PRM_T ScalerDispmodeTbl_1920_1080[] =
{     
    {SCALER_DISPMODE_DOWN_SCALING_MODE        ,0,0x11331311,0,0,0,4,0,0},
    {SCALER_DISPMODE_UP_SCALING_MODE          ,0,0x11441351,0,1,0,4,0,0},
    {SCALER_DISPMODE_OFF_MODE ,2,0x11771254,0,0,0,4,0,0},
#ifdef MT5387_TODO
    {MODE_1440x0576x50HZ_82   ,0,0x11441251,0,1,0,3,0,0x1ec},
    {MODE_1280x0720x50HZ_85	  ,0,0x11441251,0,1,0,3,0,0xcc},
    {MODE_1280x0720x60HZ_86	  ,0,0x11441251,0,1,0,3,0,0xe4},
    {MODE_0640x0480x60HZ_08   ,0,0x11441251,0,1,0,3,0,0x1e4},
    {MODE_0640x0480x66HZ_20   ,0,0x11441251,0,1,0,3,0,0x1c8},
    {MODE_0800x0600x56HZ_26   ,0,0x11441251,0,1,0,3,0,0x13c},
    {MODE_0800x0600x60HZ_27   ,0,0x11441251,0,1,0,3,0,0x128},
    {MODE_1024x0768x60HZ_34   ,0,0x11441251,0,1,0,3,0,0xa4},
    {MODE_720p_50          ,0,0x11441251,0,1,0,3,0,0x100},
    {MODE_720p_60          ,0,0x11441251,0,1,0,3,0,0x100},
    {MODE_1080p_50          ,0,0x11441251,0,1,0,3,0,0x85},
    {MODE_1080i          ,0,0x11441251,0,1,0,3,0,0x85},
    {MODE_1080p_60          ,0,0x11441251,0,1,0,3,0,0x85},
    //720p  
    {MODE_1280x0720x50HZ_85   ,0,0x11441251,0,1,0,3,0,0x100},
    {MODE_1280x0720x60HZ_86   ,0,0x11441251,0,1,0,3,0,0x100},    
    //1080i
    {MODE_1920x1080x50HZ_88   ,0,0x11331211,0,0,0,2,0,0x85},
    {MODE_1920x1080x60HZ_89   ,0,0x11331211,0,0,0,2,0,0x85},    
    //1080P
    {MODE_1920x1080x50HZ_93   ,0,0x11331211,0,0,0,2,0,0x85},
    {MODE_1920x1080x60HZ_94   ,0,0x11331211,0,0,0,2,0,0x85},    
    
    //too small, no need
    //{MODE_1280x0960x60HZ_61   ,0,0x11441251,0,1,0,3,0,0x2c},
    //{MODE_1280x1024x60HZ_64   ,0,0x11441251,0,1,0,3,0,0x20},
    //{MODE_1024x0768x60HZ_34   ,0,0x11441251,0,1,0,3,0,0xa4},
#endif
};

SCALER_DISPMODE_PRM_T ScalerDispmodeTbl_Default[] =
{     
    {SCALER_DISPMODE_DOWN_SCALING_MODE        ,0,0x11331211,0,1,0,2,0,0},
    {SCALER_DISPMODE_UP_SCALING_MODE          ,0,0x11441251,0,1,0,3,0,0},
    {SCALER_DISPMODE_OFF_MODE ,2,0x11771254,0,0,0,4,0,0},
};

SCALER_TABLE_MAPPING_T PanelResolutionTbl[] = 
{
    //{panel width, height}
    {1440,900,ScalerDispmodeTbl_1440_900},
    {1680,1050,ScalerDispmodeTbl_1680_1050},
    {1366,768,ScalerDispmodeTbl_1366_768},
    {1920,1080,ScalerDispmodeTbl_1920_1080},
};

UINT8 GetScalerTableSize(void)
{
    UINT8 u4Size;
    
    if (ScalerDispmodeTbl == ScalerDispmodeTbl_1440_900)
    {
        u4Size = sizeof(ScalerDispmodeTbl_1440_900)/sizeof(SCALER_DISPMODE_PRM_T);
    }
    else if (ScalerDispmodeTbl == ScalerDispmodeTbl_1680_1050)
    {
        u4Size = sizeof(ScalerDispmodeTbl_1680_1050)/sizeof(SCALER_DISPMODE_PRM_T);
    }
    else if (ScalerDispmodeTbl == ScalerDispmodeTbl_1366_768)
    {
        u4Size = sizeof(ScalerDispmodeTbl_1366_768)/sizeof(SCALER_DISPMODE_PRM_T);
    }
    else if (ScalerDispmodeTbl == ScalerDispmodeTbl_1920_1080)
    {
        u4Size = sizeof(ScalerDispmodeTbl_1920_1080)/sizeof(SCALER_DISPMODE_PRM_T);
    }
    else
    {
        u4Size = sizeof(ScalerDispmodeTbl_Default)/sizeof(SCALER_DISPMODE_PRM_T);
    }
    
    Printf("table size %d\n", u4Size);
    return u4Size;
}

UINT8 GetScalerDispmodePrmTimingID(UINT8 index)
{
    return ScalerDispmodeTbl[index].bTimingID;
}

UINT8 GetScalerDispmodePrmVsOutSel(UINT8 index)
{
    return ScalerDispmodeTbl[index].bVsOutSel;
}

UINT32 GetScalerDispmodePrmUpScalerAdj(UINT8 index)
{
    return ScalerDispmodeTbl[index].u4UpScalerAdj;
}

UINT8 GetScalerDispmodePrmVsDataThp(UINT8 index)
{
    return ScalerDispmodeTbl[index].bVsDataThp;
}

UINT8 GetScalerDispmodePrmVsEdgeSel(UINT8 index)
{
    return ScalerDispmodeTbl[index].bVsEdgeSel;
}

UINT8 GetScalerDispmodePrmVsDataFh(UINT8 index)
{
    return ScalerDispmodeTbl[index].bVsDataFh;
}

UINT8 GetScalerDispmodePrmVSyncDelSel(UINT8 index)
{
    return ScalerDispmodeTbl[index].bVSyncDelSel;
}

UINT8 GetScalerDispmodePrmInitialVSP(UINT8 index)
{
    return ScalerDispmodeTbl[index].bInitialVSP;
}
UINT16 GetScalerDispmodePrmInitialTune(UINT8 index)
{
    return ScalerDispmodeTbl[index].u2InitialTune;
}

void vBindScalerTable()
{
    UINT16 panelW;
    UINT16 panelH;
    UINT8 i;

    panelW = PANEL_GetPanelWidth();
    panelH = PANEL_GetPanelHeight();
    
    for(i=0;i<4;i++)
    {
        if(PanelResolutionTbl[i].panelWidth == panelW &&
           PanelResolutionTbl[i].panelHeight== panelH )
        {
            ScalerDispmodeTbl = PanelResolutionTbl[i].scalerTable;
            return;
        }
    }

    //if not found, point to default table
    ScalerDispmodeTbl = ScalerDispmodeTbl_Default;
}

#if 0//OLD_NONLINEAE_US_ONOFF
UINT16 GetNonlinerPrmPrmUScaleH(UINT8 index)
{
    UINT8 bID = bGetScalerPanelId();
    return NonlinearTbl[bID][index].u4UScaleH;
}
UINT8 GetNonlinerPrmNonlstop(UINT8 index)
{
    UINT8 bID = bGetScalerPanelId();
    return NonlinearTbl[bID][index].bNonlstop;
}
UINT8 GetNonlinerPrmPrmSHOfst(UINT8 index)
{
    UINT8 bID = bGetScalerPanelId();
    return NonlinearTbl[bID][index].bSHOfst;
}
#endif


