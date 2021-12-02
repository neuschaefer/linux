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
#include "hw_predown.h"
#include "drv_predown.h"
#include "drv_scaler_drvif.h"

 /**
  * @brief Get H pre-scaling down factor
  * @param bPath VDP_1/VDP_2
  * @return pre-scaling down factor
  * @6896 porting done
  */
 UINT32 u4ScpipGetHwHPS(UINT8 bPath)
 {
     return RegReadFldAlign(PRE_DS_MAIN_00 + getPDSOffset(bPath), PRE_DS_MAIN_00_PRE_DSCALE_H_1);
 }
  /**
  * @brief Get V pre-scaling down factor
  * @param bPath VDP_1/VDP_2
  * @return pre-scaling down factor
  * @6896 porting done
  */
 UINT32 u4ScpipGetHwVPS(UINT8 bPath)
 {
     return RegReadFldAlign(PRE_DS_MAIN_00 + getPDSOffset(bPath), PRE_DS_MAIN_00_PRE_DSCALER_V_1);
 }
 /**
  * @brief Set pre-scaling down factor
  * @param u4VdpId VDP_1/VDP_2
  */
 void vScpipSetHwHPSFactor(UINT32 u4VdpId, UINT32 u4Factor)
 {        
     UINT32 u4HpsFactor; 
 
     // defaul value:0x8000 means auto init phase, no need to set the phase again.
     //u4HInitValue = DFT_HPD_FACTOR - u4Factor;
     u4HpsFactor = u4Factor;
     vRegWriteFldAlign(PRE_DS_MAIN_00 + getPDSOffset(getScalerPath(u4VdpId)), (UINT16)u4HpsFactor, PRE_DS_MAIN_00_PRE_DSCALE_H_1);
     vRegWriteFldAlign(PRE_DS_MAIN_01 + getPDSOffset(getScalerPath(u4VdpId)), (UINT16)(0x8000-u4HpsFactor), PRE_DS_MAIN_01_PRE_DSCALE_H_INIT_1);

 }
 /**
   * @brief Set pre-scaling down factor
   * @param u4VdpId VDP_1/VDP_2
   */
  void vScpipSetHwVPSFactor(UINT32 u4VdpId, UINT32 u4Factor)
  {        
      UINT32 u4HpsFactor; 
  
      // defaul value:0x8000 means auto init phase, no need to set the phase again.
      //u4VInitValue = DFT_HPD_FACTOR - u4Factor;
      u4HpsFactor = u4Factor;
      vRegWriteFldAlign(PRE_DS_MAIN_00 + getPDSOffset(getScalerPath(u4VdpId)), (UINT16)u4HpsFactor, PRE_DS_MAIN_00_PRE_DSCALER_V_1);
      vRegWriteFldAlign(PRE_DS_MAIN_01 + getPDSOffset(getScalerPath(u4VdpId)), (UINT16)(0x8000-u4HpsFactor), PRE_DS_MAIN_01_PRE_DSCALE_V_INIT_1);
  }

 /**
   * @brief Set pre-scaling input resolution
   * @param u4VdpId VDP_1/VDP_2
   */
  void vScpipSetPreDownInputResolution(UINT32 u4VdpId, UINT32 u4InW, UINT32 u4InH)
  {        
      vRegWriteFldAlign(PRE_DS_MAIN_04 + getPDSOffset(getScalerPath(u4VdpId)), (UINT16)u4InW, PRE_DS_MAIN_04_PREDS_MAIN_IN_H);
      vRegWriteFldAlign(PRE_DS_MAIN_04 + getPDSOffset(getScalerPath(u4VdpId)), (UINT16)u4InH, PRE_DS_MAIN_04_PREDS_MAIN_IN_V);
  }

 /**
   * @brief Set pre-scaling output resolution
   * @param u4VdpId VDP_1/VDP_2
   */
  void vScpipSetPreDownOutputResolution(UINT32 u4VdpId, UINT32 u4InW, UINT32 u4InH)
  {        
      vRegWriteFldAlign(PRE_DS_MAIN_03 + getPDSOffset(getScalerPath(u4VdpId)), (UINT16)u4InW, PRE_DS_MAIN_03_PREDS_MAIN_OUT_H);
      vRegWriteFldAlign(PRE_DS_MAIN_03 + getPDSOffset(getScalerPath(u4VdpId)), (UINT16)u4InH, PRE_DS_MAIN_03_PREDS_MAIN_OUT_V);
  }

 UINT32 u4ScpipGetPreDownInputWidth(UINT8 bPath)
 {
     return RegReadFldAlign(PRE_DS_MAIN_04 + getPDSOffset(bPath), PRE_DS_MAIN_04_PREDS_MAIN_IN_H);
 }

 UINT32 u4ScpipGetPreDownInputHeight(UINT8 bPath)
  {
      return RegReadFldAlign(PRE_DS_MAIN_04 + getPDSOffset(bPath), PRE_DS_MAIN_04_PREDS_MAIN_IN_V);
  }
 UINT32 u4ScpipGetPreDownOutputWidth(UINT8 bPath)
  {
      return RegReadFldAlign(PRE_DS_MAIN_03 + getPDSOffset(bPath), PRE_DS_MAIN_03_PREDS_MAIN_OUT_H);
  }

 UINT32 u4ScpipGetPreDownOutputHeight(UINT8 bPath)
  {
      return RegReadFldAlign(PRE_DS_MAIN_03 + getPDSOffset(bPath), PRE_DS_MAIN_03_PREDS_MAIN_OUT_V);
  }

