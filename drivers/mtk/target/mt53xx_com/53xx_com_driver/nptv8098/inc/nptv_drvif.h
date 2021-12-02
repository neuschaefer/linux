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
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: nptv_drvif.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _NPTV_DRVIF_H_
#define _NPTV_DRVIF_H_

/***********************************************************************************/
/*********************************   global part  ****************************************/
/***********************************************************************************/
#include "nptv_if.h"
#include "sv_const.h"
#include "drv_tvd.h"

 /***********************************************************************************/
 /********************************* for  Driver only**************************************/
 /***********************************************************************************/

 /**
  * @brief vApiVideoSrcChangeNotify
  * Input Change Notify, ReCalculate Source Region
  */
//void vApiVideoInputChangeNotify(UINT8 bPath);

 /* TVD */
//extern void vTvdHoldMode(UINT8 bHoldOnOff);
//extern void vTvd3dChannelChange(void);

 /**
  * @brief LboxDetectionInit
  */
void LboxDetectionInit(void);

 /*
    Function    : void vApiVideoMainloop(void)
    Description :
    Parameters  :
    Return      :
  */
void vApiVideoMainloop(void);
 BOOL VdoMLInit(void);

// *****************************************************
// Event Flags Definition
// ****************************************************
// Main Channel Related
#define VDO_FLG_MAIN_MODECHG (1<<0)
#define VDO_FLG_MAIN_MODECHG_DONE (1<<1)
#define VDO_FLG_MAIN_UPDATE_OSD (1<<2)
 // Pip Channel Related
#define VDO_FLG_PIP_MODECHG (1<<3)
#define VDO_FLG_PIP_MODECHG_DONE (1<<4)
#define VDO_FLG_PIP_UPDATE_OSD (1<<5)
 // VGA Auto
#define VDO_FLG_VGAAUTO_DOING (1<<6)
 // SV First Related
 #define VDO_FLG_SCART_SF_CHG (1<<9)


 // TVD Auto calibration
#define VDO_FLG_TVDAUTO_DOING (1<<10)

 //extern UINT16   _wEventFlag;
extern UINT16 _wEventFlag;

 /*
  * Description:
  *    Check if some event flag(s) is set, if more than two flags are
  *    given in dwFlag, this function will return TRUE only if all the flags
  *    are set.
  *  Parameters:
  *    dwFlag  --- Flag to be checked
  *  Return : TRUE -- Flag(s) is set
  *           FALSE        Flag(s) is not set
  */
#define fgApiVideoIsEvtFlgSet(arg) ((_wEventFlag & (arg)) > 0)

 /*  Description:
  *    Clear event flags
  *  Parameters:
  *    dwFlag  --- Flags to be cleared
  *  Return : None
  *  Remark:
  *    UI can use this function to clear the events which are (to be)
  */
#define vApiVideoClrEvtFlg(arg) (_wEventFlag &= (~(arg)))

 /*  Description:
  *    Set event flags
  *  Parameters:
  *    dwFlag  --- Flags to be set
  *  Return : None
  *  Remark:
  *    This function is used to set event flags to inform UI about some
  *    status or mode changes. (not including actions triggered by UI itself.
  *    Should not be called by UI)
  */
#define vApiVideoSetEvtFlg(arg) (_wEventFlag |= (arg))

#endif /* _NPTV_DRVIF_H_ */
