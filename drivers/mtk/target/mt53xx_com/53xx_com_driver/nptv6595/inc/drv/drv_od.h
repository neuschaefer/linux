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
 * $RCSfile: drv_od.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

#ifndef _DRV_OD_H_
#define _DRV_OD_H_

#define OD_FW_WA1   1     //reset OD and check CRC when power up
#define OD_FW_WA2   1     //OD enable toggle to reset dram CRC
#define OD_FW_WA3   1     //OD reset when mode change
#define OD_FW_WA4   1    //enable lvds 6line buffer in OD+ LVDS combination case
#define OD_EXT_FP     1     //OD use external frame protect
#define OD_TABLE_CONFIG OD_TABLE_33 // OD table size

#define OD_TBL_S_DIM	17
#define OD_TBL_M_DIM	33
#define OD_TBL_L_DIM	41
#define FB_TBL_DIM	    17
#define PCID_TBL_DIM	17

#define OD_TBL_S_SIZE	(OD_TBL_S_DIM * OD_TBL_S_DIM)
#define OD_TBL_M_SIZE	(OD_TBL_M_DIM * OD_TBL_M_DIM)
#define OD_TBL_L_SIZE	(OD_TBL_L_DIM * OD_TBL_L_DIM)
#define FB_TBL_SIZE	    (FB_TBL_DIM * FB_TBL_DIM)
#define PCID_TBL_SIZE	(PCID_TBL_DIM * PCID_TBL_DIM)

extern UINT8 OD_Table[];
extern UINT8 FB_Table[FB_TBL_SIZE];
extern UINT8 PCID_Table[PCID_TBL_SIZE];

enum 
{
    OD_RED, 
    OD_GREEN, 
    OD_BLUE, 
    OD_ALL
};

enum 
{
    OD_TABLE_33,
    OD_TABLE_41_MODE1,
    OD_TABLE_41_MODE2,
    FB_TABLE_17
};

void vDrvODOnOff(UINT8 u1OnOff);
void vDrvODBypass(UINT8 u1OnOff);
void vDrvODReset(void);
void vDrvODWriteTable(UINT8 TableSel, UINT8 ColorSel, UINT8* pTable);
UINT8 u1DrvODReadTable(UINT8 TableSel, UINT8 ColorSel, UINT8* pTable);
void vDrvODStressTest(UINT32 number);
void vDrvODDramOverflowReset(void);
UINT32 u4DrvDramOverflow(void);
void vDrvODInit(void);
void vDrvODCheckEnable(void);
void OD_OnOutputVSync(void);
void vDrvODLoadODTableVsync(UINT8* pTable );
void vDrvODLoadODTable(void);

#endif
