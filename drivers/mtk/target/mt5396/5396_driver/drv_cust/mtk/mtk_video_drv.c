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

// === INCLUDE =============================================================================
#include "drv_tdtv_drvif.h"
#include "drv_video.h"
#include "x_gpio.h"
#include "x_pinmux.h"

// === HW INCLUDE =============================================================================

// === DEFINE =============================================================================
// === FUNCTION PROTOTYPE =================================================================

// === EXTERN FUNCTION ====================================================================
#ifdef DRV_USE_EXTERNAL_3D_FRC

#include "panel.h"

EXTERN  void vOSTGSetBorderColor(UINT32 u4VdpId, UINT32 u4RGB);
EXTERN void vOSTGSetBorderParam(UINT32 u4VdpId, UINT32 u4X, UINT32 u4Y, UINT32 u4Width, UINT32 u4Height, UINT32 u4Thickness);
EXTERN void vOSTGSetBorderOnOff(UINT32 u4VdpId, UINT32 u4OnOff);

enum
{
    VDO_2D,
    VDO_3D_FS,
    VDO_3D_SBS,
    VDO_3D_TAB,
    VDO_3D_TTD, 
    VDO_3D_SS
};

EXTERN INT32 drv_extmjc_set_3D_fmt(
     UINT8                u13D_fmt,
     UINT32                u4Para0,
     UINT32                u4Para1,
     UINT8*              pui1_msg_error
     );

EXTERN INT32 drv_extmjc_set_Mode_Change(
     UINT8*              pui1_msg_error
     );
#endif

// === GLOBAL VARIABLE ====================================================================
#ifdef DRV_USE_EXTERNAL_3D_FRC
UINT64 u8VdoPathProcCfgExt[NUM_OF_PACKING_TYPE]=
{
    //Reserve 
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_2D_I,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_2D_I,E_TD_OUT_NATIVE),MIB(E_TD_IN_2D_I,E_TD_OUT_NATIVE), SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_2D_P ,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MIB(E_TD_IN_2D_P,E_TD_OUT_NATIVE), SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_2D_P ,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    //2D
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_2D_I,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_2D_I,E_TD_OUT_NATIVE),MIB(E_TD_IN_2D_I,E_TD_OUT_NATIVE), SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_2D_P ,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MIB(E_TD_IN_2D_P,E_TD_OUT_NATIVE), SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_2D_P ,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    //TTD   
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_2D_I,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_2D_I,E_TD_OUT_NATIVE),MIB(E_TD_IN_2D_I,E_TD_OUT_NATIVE), SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_2D_P ,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MIB(E_TD_IN_2D_P,E_TD_OUT_NATIVE), SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_2D_P ,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    //FS  
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_FS_I,E_TDTV_DECODER_OUTPUT_3D_FS  ),DQC(E_TD_IN_2D_I,E_TD_OUT_NATIVE),MIB(E_TD_IN_FS_I,E_TD_OUT_NATIVE), SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_FS_P ,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_FS_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_FS_P,E_TD_OUT_3D_FS)),
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_FS_P,E_TDTV_DECODER_OUTPUT_3D_FS  ),DQC(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MIB(E_TD_IN_FS_P,E_TD_OUT_NATIVE), SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_FS_P ,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_FS_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_FS_P,E_TD_OUT_3D_FS)),
    //FP   
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_FP_I,E_TDTV_DECODER_OUTPUT_3D_FS  ),DQC(E_TD_IN_2D_I,E_TD_OUT_NATIVE),MIB(E_TD_IN_FS_I,E_TD_OUT_NATIVE), SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_FS_P ,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_FS_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_FS_P,E_TD_OUT_3D_FS)),
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_FP_P,E_TDTV_DECODER_OUTPUT_3D_FS  ),DQC(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MIB(E_TD_IN_FS_P,E_TD_OUT_NATIVE), SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_FS_P ,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_FS_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_FS_P,E_TD_OUT_3D_FS)),
    //TB  
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_TB_I,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_2D_I,E_TD_OUT_NATIVE),MIB(E_TD_IN_TB_I,E_TD_OUT_NATIVE), SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_TB_P ,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_TB_P,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MIB(E_TD_IN_TB_P,E_TD_OUT_NATIVE), SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_TB_P ,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    //SBS
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_SBS_I,E_TDTV_DECODER_OUTPUT_NATIVE),DQC(E_TD_IN_2D_I,E_TD_OUT_NATIVE),MIB(E_TD_IN_SBS_I,E_TD_OUT_NATIVE),SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_SBS_P,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_SBS_P,E_TDTV_DECODER_OUTPUT_NATIVE),DQC(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MIB(E_TD_IN_SBS_P,E_TD_OUT_NATIVE),SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_SBS_P,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    //RD
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_2D_I,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_2D_I,E_TD_OUT_NATIVE),MIB(E_TD_IN_SBS_I,E_TD_OUT_NATIVE),SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_SBS_P,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_2D_P,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MIB(E_TD_IN_SBS_P,E_TD_OUT_NATIVE),SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_SBS_P,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    //Sensio
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_SS_I,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_SS_I,E_TD_OUT_3D_SBS),MIB(E_TD_IN_SBS_I,E_TD_OUT_NATIVE),SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_SBS_P,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_SS_P,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_SS_P,E_TD_OUT_3D_SBS),MIB(E_TD_IN_SBS_P,E_TD_OUT_NATIVE),SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_SBS_P,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    //Line Interleave MT5389 Only can handle LI by SCL, no DQC
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_LI_I,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_2D_I,E_TD_OUT_NATIVE),MIB(E_TD_IN_LI_I ,E_TD_OUT_NATIVE),SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_LI_P ,E_TD_OUT_3D_SBS,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_LI_P,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MIB(E_TD_IN_LI_P ,E_TD_OUT_NATIVE),SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_LI_P ,E_TD_OUT_3D_SBS,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    //Checker Board
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_CB_I,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_CB_I,E_TD_OUT_3D_SBS),MIB(E_TD_IN_SBS_I,E_TD_OUT_NATIVE),SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_SBS_P,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_CB_P,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_CB_P,E_TD_OUT_3D_SBS),MIB(E_TD_IN_SBS_P,E_TD_OUT_NATIVE),SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_SBS_P,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    //Dot Alternative
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_DA_I,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_DA_I,E_TD_OUT_3D_SBS),MIB(E_TD_IN_SBS_I,E_TD_OUT_NATIVE),SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_SBS_P,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
    TD_TBL_PACK(DEC(E_TDTV_DECODER_INPUT_DA_P,E_TDTV_DECODER_OUTPUT_NATIVE ),DQC(E_TD_IN_DA_P,E_TD_OUT_3D_SBS),MIB(E_TD_IN_SBS_P,E_TD_OUT_NATIVE),SCL(E_TD_RES_NORMAL,E_TD_RES_NORMAL,E_TD_IN_SBS_P,E_TD_OUT_NATIVE,E_TD_RES_NORMAL,E_TD_RES_NORMAL),TTD(E_TD_IN_2D_P,E_TD_OUT_NATIVE),MJC(E_TD_IN_2D_P,E_TD_OUT_NATIVE)),
};

#endif

// === EXTERN VARIABLE ====================================================================


// === STATIC VARIABLE ====================================================================
// === END OF STATIC VARIABLE =============================================================
// === BODY ===============================================================================
#ifdef DRV_USE_EXTERNAL_3D_FRC

#define IS_INV          0
#define IS_INV_EN       1
#define IS_PROTUDE_EN   2
#define IS_DISTANT_EN   3
#define IS_OSDDEP_EN    4


static UINT8 u13DFrcDepth=16;
static UINT8 u1Vdo3DFmt=VDO_2D;
static UINT8 u1IsInverse=SV_OFF;
static UINT8 u1FrcProtude=2;
static UINT8 u1DistanceFromTV=10;
static UINT8 u1FrcOSDDepth=16;

#define IS_INV                          0
#define IS_INV_EN                       1
#define IS_PROTUDE_DISTANCE_OSDDEP_EN   2


static inline void DRVCUST_SendFrc3DCmd()
{
    UINT8 u1ErrMsg;
    UINT32 u4Para0=u13DFrcDepth;    
    UINT32 u4Para1=(u1FrcOSDDepth<<24)|(u1DistanceFromTV<<16)|(u1FrcProtude<<8) |
        (1<<IS_PROTUDE_DISTANCE_OSDDEP_EN)|(1<<IS_INV_EN)|u1IsInverse;

    drv_extmjc_set_3D_fmt(u1Vdo3DFmt,u4Para0,u4Para1,&u1ErrMsg);
}

VOID DRVCUST_SetExtFrc3DDepth(UINT32 u4Depth)
{
    u13DFrcDepth=(UINT8)u4Depth;    
    DRVCUST_SendFrc3DCmd();
}

VOID DRVCUST_SetExtFrc3DInverse(UINT32 u4IsInverse)
{
    UINT8 u1ErrMsg;
    u1IsInverse=(UINT8)u4IsInverse; 
    DRVCUST_SendFrc3DCmd();
    drv_extmjc_set_Mode_Change(&u1ErrMsg);
}

VOID DRVCUST_SetExtFrcProtrude(UINT32 u4Protrude)
{
    u1FrcProtude=(UINT8)u4Protrude; 
    DRVCUST_SendFrc3DCmd();
}

VOID DRVCUST_SetExtFrcDistanceFromTV(UINT32 u4Distance)
{
    u1DistanceFromTV=(UINT8)u4Distance; 
    DRVCUST_SendFrc3DCmd();
}

VOID DRVCUST_SetExtFrcOSDDepth(UINT32 u4OSDDepth)
{
    u1FrcOSDDepth=(UINT8)u4OSDDepth; 
    DRVCUST_SendFrc3DCmd();
}

#endif


VOID DRVCUST_SetExtFrc3DFormat(E_TDTV_UI_3D_PACKING eUIPackingSel)
{
#if defined(DRV_USE_EXTERNAL_3D_FRC)
    UINT8 u1ErrMsg;
    static E_TDTV_UI_3D_PACKING ePreFMT=E_TDTV_UI_3D_MODE_END;

    if(bDrvVideoSignalStatus(SV_VP_MAIN)!=SV_VDO_STABLE)
    {
        eUIPackingSel=E_TDTV_UI_3D_MODE_OFF;
    }
    
    if(ePreFMT!=eUIPackingSel)
    {
        if(E_TDTV_UI_3D_MODE_RD==eUIPackingSel)
        {
            vOSTGSetBorderColor(VDP_1, 0x0); /*set color ==> black*/
            vOSTGSetBorderParam(VDP_1, 0, PANEL_GetPanelHeight()-4, PANEL_GetPanelWidth(), 4, 2);
            vOSTGSetBorderOnOff(VDP_1, SV_ON);
        }
        else
        {
            vOSTGSetBorderOnOff(VDP_1, SV_OFF);
        }    
        switch(eUIPackingSel)
        {
            case E_TDTV_UI_3D_MODE_FS:
                u1Vdo3DFmt=VDO_3D_FS;
                break;
            case E_TDTV_UI_3D_MODE_SBS:
            case E_TDTV_UI_3D_MODE_RD:     
            case E_TDTV_UI_3D_MODE_SS:
            case E_TDTV_UI_3D_MODE_LI:                
            case E_TDTV_UI_3D_MODE_CB:
            case E_TDTV_UI_3D_MODE_DA:
                u1Vdo3DFmt=VDO_3D_SBS;
                break;
            case E_TDTV_UI_3D_MODE_TB:
                u1Vdo3DFmt=VDO_3D_TAB;
                break;
            case E_TDTV_UI_3D_MODE_TTD:
                u1Vdo3DFmt=VDO_3D_TTD;
                break;              
            default:
                u1Vdo3DFmt=VDO_2D;                
        }

        DRVCUST_SendFrc3DCmd();
        drv_extmjc_set_Mode_Change(&u1ErrMsg);
        ePreFMT=eUIPackingSel;
    }
    return;
#elif defined(CC_MT5396)

    UINT32 u4Ext3DEnableGPIO;
    DRVCUST_OptQuery(e3DEnableGPIO,&u4Ext3DEnableGPIO);

    if((E_TDTV_UI_3D_MODE_OFF!=eUIPackingSel)&&
     (bDrvVideoSignalStatus(SV_VP_MAIN)==SV_VDO_STABLE))
    {
         GPIO_SetOut(u4Ext3DEnableGPIO, 1);
         //Output LR Singal Sync
         BSP_PinSet(PIN_GPIO54, 1);
    }
    else
    {
         GPIO_SetOut(u4Ext3DEnableGPIO, 0);
         //Disable LR Signal Sync
         BSP_PinSet(PIN_GPIO54, 0);
    }
#endif

}

