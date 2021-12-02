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
 * $RCSfile: ddi.h,v $
 * $Revision: #1 $
 * $Date: 2012/04/27 $
 * $Author: dtvbm11 $
 *---------------------------------------------------------------------------*/ 

/** @file ddi.h
 *  This header file declares exported DDI APIs.
 */

#ifndef DDI_H
#define DDI_H
#include "mtal.h"
#include "common.h"
#ifndef __KERNEL__
#include <string.h>
#include <pthread.h>
#endif


#ifndef EXTERN
    #ifdef __cplusplus
        #define EXTERN          extern "C"
    #else
        #define EXTERN          extern
    #endif
#endif  // EXTERN

#ifndef BCM_PLATFORM 
#define BCM_PLATFORM 0
#endif

#ifndef MTK_PLATFORM
#define MTK_PLATFORM 1
#endif

#ifndef PLATFORM_TYPE 
#define PLATFORM_TYPE MTK_PLATFORM
#endif

#ifndef INCLUDE_10POINTS
#define INCLUDE_10POINTS
#endif


#ifndef _LINUX_
#define _LINUX_
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#if 1

#ifdef INCLUDE_HDMICEC
#define		CP_WRITE_CECM_MESSAGE				0x47  	/* INCLUDE_HDMICEC */
#define		CP_WRITE_CECM_SETMODE				0x48  	/* INCLUDE_HDMICEC */
#define		CP_WRITE_CECM_READY					0x49  	/* INCLUDE_HDMICEC */

#define		CP_READ_HDMI_REP_RESULT				0xA3	/* INCLUDE_HDMICEC */
#define		CP_READ_HDMI_CEC_DATA				0xA2	/* INCLUDE_HDMICEC */
#endif


typedef enum
{
	AUDIO_SIF_SYSTEM_BG	= 0x00,		// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	AUDIO_SIF_SYSTEM_I,				// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	AUDIO_SIF_SYSTEM_DK,			// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	AUDIO_SIF_SYSTEM_L,				// keep the position. 	refer to IF_PLL_DATA_TYPE_t
	AUDIO_SIF_SYSTEM_MN,			// keep the position.	refer to IF_PLL_DATA_TYPE_t
	AUDIO_SIF_SYSTEM_LP,			/* 20080724 : Added by Goldman for FE/TU */
	AUDIO_SIF_SYSTEM_END,			/* 20080724 : Added by Goldman for FE/TU */
 	AUDIO_SIF_SYSTEM_UNKNOWN = 0xf0
} AUDIO_SIF_SOUNDSYSTEM_T;

typedef enum {
	VIDEO_DDI_DISP_BKGRND_BLACK,
	VIDEO_DDI_DISP_BKGRND_GRAY
} VIDEO_DDI_DISP_BKGRND_COLOR_T;

typedef enum
{
    MSG_SDEC2SIPSIP_SECTION_ACQUIRED,
    MSG_SDEC2TTXSUBT_PES_ACQUIRED
} SDEC_MSG_ID_T;

typedef struct 
{
    UINT32  u4BufferSize;
    UINT32 u4BufferSizeOrder;
    UINT32  u4PhysicalAddr;
} GFXOSD_BUFFER_INFO_T;

typedef enum
{
	I2C_CHANNEL_1		=	1,		/** I2 Channel 1	*/
	I2C_CHANNEL_2		=	2,		/** I2 Channel 2	*/
  I2C_CHANNEL_3		=	3,		/** I2 Channel 3	*/
}	I2C_CHANNEL_T;

typedef struct
{
	UINT8	nSubBrightness;
	UINT8	nSubContrast;
} 	ADJUST_SUB_BC_T;


typedef enum
{
    INCH_70     	   = 0,
    INCH_60             = 1,
    INCH_55             = 2,
    INCH_52             = 3,
    INCH_50             = 4,
    INCH_47             = 5,
    INCH_42             = 6,
    INCH_37             = 7,
    INCH_32             = 8,
    INCH_26             = 9,
    INCH_22             = 10,
    INCH_19             = 11,
    INCH_BASE           = 12
}	INCH_TYPE_T;

typedef enum
{
    TOOL_PG70 = 0,
    TOOL_PG69,
    TOOL_PG65,
    TOOL_PG63,
    TOOL_PG60,
    TOOL_PG50,
    TOOL_PG30,
    TOOL_PG10,
    TOOL_LG70,
    TOOL_LG60,
    TOOL_LG50,
    TOOL_LG30,
    TOOL_LG10,

    TOOL_BASE,
}	TOOL_TYPE_T;

typedef enum
{
    MODULE_BASE         = 0,
    LGE_FHD             = 1,
    LGE_WXGA            = 2,
    LGE_XGA             = 3,
    LGE_32XGA           = 4,
    LGE_FHD_120         = 5,
    LGE_WXGA_120        = 6,


    LPL_FHD             = 32,
    LPL_WXGA            = 33,
    LPL_FHD_120         = 34,
    LPL_WXGA_120        = 35,
    AUO_FHD             = 36,
    AUO_WXGA            = 37,
    AUO_FHD_120         = 38,
    AUO_WXGA_120        = 39,
    CMO_FHD             = 40,
    CMO_WXGA            = 41,
    CMO_FHD_120         = 42,
    CMO_WXGA_120        = 43,
    SHARP_FHD           = 44,
    SHARP_WXGA          = 45,
    SHARP_FHD_120       = 46,
    SHARP_WXGA_120      = 47,

}	MODULE_TYPE_T;


typedef enum
{
    EN_LCD_LPL,
    EN_LCD_CMO,
    EN_LCD_AUO,
    EN_LCD_SHARP,//wcbae 071128
    EN_LCD_MODULE_NUM
} MODULE_MAKER_T;
#endif 

#ifndef __KERNEL__
#include "adec_ddi.h"
#include "audio_ddi.h"
#include "ci_ddi.h"
#include "cmnio_ddi.h"
#include "edid_data.h"
#include "emp_ddi.h"
#include "fan_ddi.h"
#include "frc_ddi.h"
#include "gfxosd_ddi.h"
#include "gpio_ddi.h"
#include "hdd_ddi.h"
#include "hdmisw_ddi.h"
#include "i2c_ddi.h"
#include "index_ddi.h"
#include "micom_ddi.h"
#include "nvm_ddi.h"
#include "pdpmodule_ddi.h"
#include "pql_ddi.h"
#include "pvr_ddi.h"
#include "pwm_ddi.h"
#include "sdec_ddi.h"
#include "tu_ddi.h"
#include "vdec_ddi.h"
#include "video_ddi.h"
#include "vps_ddi.h"


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


EXTERN BOOLEAN CreateSemaphore(pthread_mutex_t *mutex);
EXTERN BOOLEAN LockSemaphore(pthread_mutex_t *mutex);
EXTERN BOOLEAN UnlockSemaphore(pthread_mutex_t *mutex);
EXTERN BOOLEAN DestroySemaphore(pthread_mutex_t *mutex);
#endif


#endif //DDI_H
