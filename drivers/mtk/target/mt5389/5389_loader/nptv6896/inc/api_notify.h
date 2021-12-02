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
 * $Author: hongjun.chu $
 * $Date: 2012/04/28 $
 * $RCSfile: api_notify.h,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/
#ifndef _API_NOTIFY_H_
#define _API_NOTIFY_H_


/** Function prototype for CC callback. */
typedef void (*NPTV_PFN_CC_NFY) (UINT8 u1Path,
                                 UINT8 u1data1, UINT8 u1data2, UINT8 u1field);

/** Function prototype for CC2X callback. */
typedef void (*NPTV_PFN_CC2X_NFY) (UINT8 u1Path,
                                  UINT32 u4NumPktAvail, UINT32 u4VbiReadPtr);

/** Function prototype for TTX callback. */
typedef void (*NPTV_PFN_TTX_NFY) (UINT8 u1Path,
                                  UINT32 u4NumPktAvail, UINT32 u4VbiReadPtr);

/** Function prototype for WSS callback. */
typedef void (*NPTV_PFN_WSS_NFY) (UINT8 u1Path, UINT32 u4Data);

/** Function prototype for VPS callback. */
typedef void (*NPTV_PFN_VPS_NFY) (UINT8 au1Data[]);

/** Function prototype for TTX Info callback. */
typedef void (*NPTV_PFN_VBI_INFO_NFY) (BOOL fgAVFieldIVS);

typedef void (*NPTV_PFN_VGA_AUTO_NFY) (UINT16 ui2_id, BOOL fgSuccess);

typedef void (*NPTV_PFN_VGA_AUTOCOLOR_NFY) (UINT16 ui2_id, BOOL fgSuccess);

typedef void (*NPTV_PFN_TVD_MVCHG_NFY) (UINT8 u1Path, BOOL fgStatus);

typedef void (*NPTV_PFN_SIG_NFY_FCT) (void *pvNfyTag,
                                      UINT32 u4SigStatus, UINT32 u4SigSrc);

typedef void (*NPTV_PFN_SCART_NFY)(UINT8 u1Source, UINT8 u1Status);

typedef void (*NPTV_PFN_HDMI_NFY) (UINT32 bHdmiPort, UINT32 bStatus);


typedef struct _NPTV_SIG_NFY_INFO_T_
{
    void *pvTag;                /* pull mode only */
    NPTV_PFN_SIG_NFY_FCT pfSigNfy;      /* pull mode only */

} NPTV_SIG_NFY_INFO_T;

#ifdef  DISABLE_NPTV_NOTIFY
#define fgApiSFirstSetup(u1Path,u1Src)	FALSE
#define vApiNotifySigChg( u1Path, u1SigState,u1ReportState)
#define vApiNotifyResolutionChg(u1Path, u1SigState)
#define vApiNotifyColorSystem(u1Path, u1SigState)
#define vApiNotifySFirst( u1Path, u1SigState)
#define vApiNotifyVGAAutoProgress(u1Path, u1Progress)
#define vApiNotifyAutoDone(u1Path,fgSuccess)
#define vApiNotifyAutoColorDone(u1Path, fgSuccess)
#define vApiNotifyMVChg(u1Path, fgSuccess)
#define vApiNotifyScartFastSwith(u1Scartid, u1FSState)
#define vAPiNotifyCCData( u1Path,  d1,  d2,  field)
#define vAPiNotifyCC2XData( u1Path,  u2PktsAvail, readptr)
#define vAPiNotifyTTXData( u1Path,  u2PktsAvail, readptr)
#define vAPiNotifyVPSData(au1Data);
#define vAPiNotifyEUWSSData(u1Path, u4data)
#define vAPiNotifyUSWSSData(u1Path, u4data)
#define vAPiNotifyVbiInfoData(BOOL)

//#elif  defined(CC_DRIVER_PROGRAM)
#else
extern BOOL fgApiMonitorSetup(UINT8 u1Path, UINT8 u1Src);

/* general */
extern void vApiNotifySigChg(UINT8 u1Path, UINT8 u1SigState,
                             UINT8 u1ReportState);
extern void vApiNotifyResolutionChg(UINT8 u1Path, UINT8 u1SigState);

/* ATV related*/
extern void vApiNotifyColorSystem(UINT8 u1Path, UINT8 u1SigState);

/* input monitor notify*/
extern void vApiNotifySFirst(UINT8 u1Path, UINT8 u1SigState);

#ifdef  SUPPORT_AV_COMP_AUTO
/* input monitor notify*/
extern void vApiNotifyAVCOMPAuto(UINT8 u1Path, UINT8 u1SigState);
#endif

/* VGA/Ypbpr related*/
//#ifdef CC_DRIVER_PROGRAM
extern void vApiNotifyVGAAutoProgress(UINT8 u1Path, UINT8 u1Progress);
//#else
//#define vApiNotifyVGAAutoProgress(u1Path, u1Progress)
//#endif
extern void vApiNotifyAutoDone(UINT8 u1Path, BOOL fgSuccess);
extern void vApiNotifyAutoColorDone(UINT8 u1Path, BOOL fgSuccess);
extern void vApiNotifyMVChg(UINT8 u1Path,BOOL fgStatus);
/*auto source detect*/
#ifdef CC_SOURCE_AUTO_DETECT
extern void vApiNotifyAutoDetectSource(UINT8 u1SrcIndex, UINT8 u1SrcStatus);
#endif

/* non-path related*/
extern void vApiNotifyScartFastSwith(UINT8 u1Scartid, UINT8 u1FSState);

/* EU related*/
extern void vAPiNotifyCCData(UINT8 u1Path, UINT8 d1, UINT8 d2, UINT8 field);
extern void vAPiNotifyCC2XData(UINT8 u1Path, UINT8 u2PktsAvail, UINT32 readptr);
extern void vAPiNotifyTTXData(UINT8 u1Path, UINT8 u2PktsAvail, UINT32 readptr);

extern void vAPiNotifyVPSData(UINT8 au1Data[]);

extern void vAPiNotifyEUWSSData(UINT8 u1Path, UINT32 u4data);

extern void vAPiNotifyUSWSSData(UINT8 u1Path, UINT32 u4data);

extern void vAPiNotifyVbiInfoData(BOOL fgAVFieldIVS);
#endif


//#ifdef CC_DRIVER_PROGRAM
/**
 * @brief Register TTX callback function
 * @param pfnFct: TTX callback function
 * @retval 0 is fail, 1 is success
 */
EXTERN INT32 NPTV_VBI_SetTtxNfy(const NPTV_PFN_TTX_NFY pfnFct);

/**
 * @brief Register CC callback function
 * @param pfnFct: CC callback function
 * @retval 0 is fail, 1 is success
 */
EXTERN INT32 NPTV_VBI_SetCcNfy(const NPTV_PFN_CC_NFY pfnFct);

/**
 * @brief Register VPS callback function
 * @param pfnFct: VPS callback function
 * @retval 0 is fail, 1 is success
 */
EXTERN INT32 NPTV_VBI_SetVpsNfy(const NPTV_PFN_VPS_NFY pfnFct);

/**
 * @brief Register WSS callback function
 * @param pfnFct: WSS callback function
 * @retval 0 is fail, 1 is success
 */
EXTERN INT32 NPTV_VBI_SetWssNfy(const NPTV_PFN_WSS_NFY pfnFct);

/**
 * @brief Register VBI Info callback function
 * @param pfnFct: VBI Info callback function 
 * @retval 0 is fail, 1 is success
 */
EXTERN INT32 NPTV_VBI_SetVbiInfoNfy(const NPTV_PFN_VBI_INFO_NFY pfnFct);

/**
 * @brief Register input video signal change callback function
 * @param pfnFct: input video signal decteion done callback function
 * @retval 0 is fail, 1 is success
 */
EXTERN INT32 NPTV_SetSigChgNfyFct(const NPTV_PFN_SIG_NFY_FCT pfnFct);


EXTERN INT32 NPTV_SetHdmiNfy(const NPTV_PFN_HDMI_NFY pfnFct);

/**
 * @brief Register VgaAuto done callback function
 * @param pfnFct: VgaAuto done callback function
 * @retval 0 is fail, 1 is success
 */
EXTERN INT32 NPTV_SetVgaAutoNfy(const NPTV_PFN_VGA_AUTO_NFY pfnFct);

/**
 * @brief Register AutoColor done callback function
 * @param pfnFct: AutoColor done callback function
 * @retval 0 is fail, 1 is success
 */
EXTERN INT32 NPTV_SetVgaAutocolorNfy(const NPTV_PFN_VGA_AUTOCOLOR_NFY pfnFct);

/**
 * @brief Register TVD/YPbPr Macrovision changed callback function
 * @param pfnFct: Macrovision changed callback function
 * @retval 0 is not MV, 1 is MV
 */
EXTERN INT32 NPTV_SetTvdMVChgNfy(const NPTV_PFN_TVD_MVCHG_NFY pfnFct);

/**
 * @brief Register SCART pin8 changed callback function
 * @param pfnFct: SCART pin8 changed callback function
 * @retval 0 is fail, 1 is success
 */
EXTERN INT32 NPTV_SetScartNfy(const NPTV_PFN_SCART_NFY pfnFct);

//#endif  // CC_DRIVER_PROGRAM

#endif //_API_NOTIFY_H_
