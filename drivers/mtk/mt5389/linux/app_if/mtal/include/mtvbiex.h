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
 * $RCSfile: mtvbiex.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/ 
/** @file mtvbiex.h
 *  This header file declares exported APIs of external VBI module.
 */

#ifndef MT_VBIEX_H
#define MT_VBIEX_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "mttype.h"
   
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
/** VBI callback Notify type. */
typedef enum
{
    MTVBI_CB_TTX_NFY = 1,         
    MTVBI_CB_WSS_NFY = 2,        
    MTVBI_CB_VPS_NFY = 4,
    MTVBI_CB_CC_NFY  = 8,
    MTVBI_CB_INFO_NFY = 16,
}MTVBI_CB_NFY_T;

/** VBI callback function type id. */
typedef enum
{
    MTVBI_CB_TTX_DATA,         
    MTVBI_CB_WSS_DATA ,        
    MTVBI_CB_VPS_DATA ,
    MTVBI_CB_CC_DATA ,
    MTVBI_CB_INFO_DATA,
    MTVBI_CB_NO_TTX_DATA ,    
    MTVBI_CB_NO_WSS_DATA ,    
    MTVBI_CB_NO_VPS_DATA ,
    MTVBI_CB_NO_CC_DATA ,
}MTVBI_NFY_T;

typedef enum
{
    MTVBI_TTX_SERVICE,         
    MTVBI_CC_SERVICE ,        
    MTVBI_CC2X_SERVICE ,
    MTVBI_WSS525_SERVICE ,
    MTVBI_WSS625_SERVICE,
    MTVBI_VPS_SERVICE ,    
    MTVBI_SERVICE_NUM ,
}MTVBI_SERVICE_T;

/** VBI WSS callback Notify type id. */
typedef enum
{
    MTVBI_WSS625 = 0x00000000,
    MTVBI_WSS525 = 0x00000001
}MTVBI_WSS_T; 

/** VBI TTX callback Notify type id. */
typedef enum
{
    MTVBI_EU_WST = 0x0, //625 World Standard Teletext 
    MTVBI_US_WST = 0x1, //525 World Standard Teletext
    MTVBI_NABTS = 0x2, //North American Broadcast Teletext Specification ,525 line
    MTVBI_MOJI  = 0x3, //Japanese teletext ,525line
    MTVBI_CCST  = 0x4, //Chinese teletext ,625 line
} MTVBI_TTX_T;

/** VBI CC callback Notify type id. */
typedef enum
{
    MTVBI_EU_CC = 0x0, //625 Closed Caption 
    MTVBI_US_CC = 0x1, //525 Closed Caption
} MTVBI_CC_T;

/** VBI Information callback Data. */
typedef union
{
    struct 
    {
        UINT32 u4TTX_Type;
        UINT32 u4Path;
        UINT32 u4PktsAvail;
        UINT32 u4ReadPtr;
    }TTXInfo;
    
    struct
    {
        UINT32 u4WSS_Type;
        UINT32 u4Path;
        UINT32 u4WSSData;
        UINT32 u4Reserved;
    }WSSInfo;

    struct
    {
        UINT32 u4CC_Type;
        UINT32 u4Path;
        UINT32 u4Field;
        UINT8  u1CCData0;
        UINT8  u1CCData1;
        UINT8  u1Reserved[2];
    }CCInfo;

    struct
    {
        UINT8 u1VPSData[13];
        UINT8 u1Reserved[3];
    }VPSInfo;

    struct 
    {
        UINT8 u1AVFieldIVS;
        UINT8 u1Reserved[15];
    }VBIInfo;

    UINT32 u4AllData[4];
}CBDATA;   

/** VBI Information callback function. */
typedef struct 
{
    UINT32 u4Type;
    CBDATA CbData;
}MTVBI_CB_T;



/** Function prototype for CC callback. */
typedef VOID (*MTVDECEX_PFN_CC_NFY)(
    MTVDO_PATH_T ePath,
    UINT8 			u1data1, 
    UINT8			u1data2, 
    UINT8 			u1field
);

/** Function prototype for TTX callback. */
typedef VOID (*MTVDECEX_PFN_TTX_NFY)(
    MTVDO_PATH_T ePath,
    UINT32		u4NumPktAvail, 
    UINT32 		u4VbiReadPtr    
);

/** Function prototype for WSS callback. */
typedef VOID (*MTVDECEX_PFN_WSS_NFY)(
    MTVDO_PATH_T ePath,
    UINT32 		u4Data
);

/** Function prototype for VPS callback. */
typedef VOID (*MTVDECEX_PFN_VPS_NFY)(
    UINT8 au1Data[]
);

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

/**
 * @brief Enable or disable CC notification
 * @param u1OnOff: SV_ON, SV_OFF
 * @param pfnCCnotify: Future reserved 
 * @retval MTR_OK Success.
 * @retval MTR_NOT_OK Fail. 
 */
EXTERN MT_RESULT_T MTVDECEX_VBI_StartNotifyCCOnOff(MTVDO_PATH_T ePath, UINT8 u1OnOff, MTVDECEX_PFN_CC_NFY pfnCCnotify);

/**
 * @brief Enable or disable TTX notification
 * @param u1OnOff: SV_ON, SV_OFF
 * @param pfnTTXnotify: Future reserved 
 * @retval MTR_OK Success.
 * @retval MTR_NOT_OK Fail. 
 */
EXTERN MT_RESULT_T MTVDECEX_VBI_StartNotifyTTXOnOff(MTVDO_PATH_T ePath, UINT8 u1OnOff, MTVDECEX_PFN_TTX_NFY pfnTTXnotify);

/**
 * @brief Enable or disable WSS notification
 * @param u1OnOff: SV_ON, SV_OFF
 * @param pfnWSSnotify: Future reserved 
 * @retval MTR_OK Success.
 * @retval MTR_NOT_OK Fail. 
 */
EXTERN MT_RESULT_T MTVDECEX_VBI_StartNotifyWSSOnOff(MTVDO_PATH_T ePath, UINT8 u1OnOff, MTVDECEX_PFN_WSS_NFY pfnWSSnotify);

/**
 * @brief Enable or disable VPS notification
 * @param u1OnOff: SV_ON, SV_OFF
 * @param pfnVPSnotify: Future reserved 
 * @retval MTR_OK Success.
 * @retval MTR_NOT_OK Fail. 
 */
EXTERN MT_RESULT_T MTVDECEX_VBI_StartNotifyVPSOnOff(MTVDO_PATH_T ePath, UINT8 u1OnOff, MTVDECEX_PFN_VPS_NFY pfnVPSnotify);

/**
 * @brief Register TTX callback function
 * @param pfnFct: TTX callback function 
 * @retval MTR_OK Success.
 * @retval MTR_NOT_OK Fail.
 */
EXTERN MT_RESULT_T MTVDECEX_VBI_SetTtxNfy(const MTVDECEX_PFN_TTX_NFY pfnFct);

/**
 * @brief Register CC callback function
 * @param pfnFct: CC callback function 
 * @retval MTR_OK Success.
 * @retval MTR_NOT_OK Fail.
 */
EXTERN MT_RESULT_T MTVDECEX_VBI_SetCcNfy(const MTVDECEX_PFN_CC_NFY pfnFct);

/**
 * @brief Register VPS callback function
 * @param pfnFct: VPS callback function 
 * @retval MTR_OK Success.
 * @retval MTR_NOT_OK Fail.
 */
EXTERN MT_RESULT_T MTVDECEX_VBI_SetVpsNfy(const MTVDECEX_PFN_VPS_NFY pfnFct);

/**
 * @brief Register WSS callback function
 * @param pfnFct: WSS callback function 
 * @retval MTR_OK Success.
 * @retval MTR_NOT_OK Fail.
 */
EXTERN MT_RESULT_T MTVDECEX_VBI_SetWssNfy(const MTVDECEX_PFN_WSS_NFY pfnFct);

/**
 * @brief Set VBI Line
 * @param bIsNTSC: Is NTSC(525) system or not
 * @param eMode: VBI Service Mode (MTVBI_CC_SERVICE,MTVBI_WSS525_SERVICE,MTVBI_WSS625_SERVICE,MTVBI_VPS_SERVICE
 * @param u1LineNum: Line number
 * @retval MTR_OK Success.
 * @retval MTR_NOT_OK Fail.
 */
EXTERN MT_RESULT_T MTVDECEX_VBI_SetVBILine(UINT8 bIsNTSC, MTVBI_SERVICE_T eMode, UINT8 u1LineNum);

/**
 * @brief Flush VBI Data
 * @param eCbType: callback type
 * @retval MTR_OK Success.
 * @retval MTR_NOT_OK Fail.
 */
EXTERN MT_RESULT_T MTVDECEX_VBI_FlushVBIData(MTVBI_NFY_T eCbType);


#ifdef __cplusplus
}
#endif

#endif //MT_VDECEX_H

