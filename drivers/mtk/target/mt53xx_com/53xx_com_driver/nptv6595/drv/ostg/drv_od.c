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
 * $RCSfile: drv_od.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/
#ifdef CC_UP8032_ATV
#include "general_mt82.h"
#endif

#ifdef CC_COPLAT_MT82
#include "util.h"
#endif

#include "general.h"
#include "hw_ckgen.h"
#include "hw_tcon.h"
#include "hw_lvds.h"
#include "hw_mlvds.h"
#include "drv_tcon.h"
#include "nptv_debug.h"
#include "fbm_drvif.h"
#include "x_timer.h"
#include "panel.h"
#include "hw_od.h"
#include "drv_od.h"
#include "drvcust_if.h"

/*************************************************************************  
* Constant/Configure Definitions
*************************************************************************/

/*************************************************************************  
* Type definitions
*************************************************************************/
typedef struct
{
	UINT32 u4Addr;
	UINT32 u4Value;
	UINT32 u4Mask;
} OD_REGTBL_T;

/*************************************************************************  
* Macro definitions
*************************************************************************/
#define OD_REGTBL_END   0xffffffff

#define OD_PANEL_WIDTH     (((PANEL_GetPanelWidth()) + 3) & 0xFFFFFFFC)
#define OD_PANEL_HEIGHT    (((PANEL_GetPanelHeight()) + 1) & 0xFFFFFFFE)  
 
/*************************************************************************  
* Imported functions (optional)
*************************************************************************/

/*************************************************************************  
* Imported variables (optional)
*************************************************************************/


/*************************************************************************  
* Global/Static variables
*************************************************************************/
static UINT32 u4ODCheck = 0;
UINT8* pODTable;

UINT8 u1SupportOD;

UINT8 OD_Table41[OD_TBL_L_SIZE]=
{
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78, 0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8, 0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8, 0xFF,
};
#if 0
static UINT8 OD_Table[OD_TBL_S_SIZE] = {
  0,   8,  18,  28,  39,  50,  60,  73,  86,  98, 109, 121, 133, 144, 155, 166, 176, 186, 195, 202, 209, 215, 220, 226, 232, 238, 243, 248, 251, 254, 255, 255, 255, 
  0,   8,  18,  28,  39,  50,  60,  73,  85,  96, 108, 120, 131, 142, 154, 164, 175, 185, 193, 201, 208, 214, 220, 226, 231, 237, 243, 248, 251, 254, 255, 255, 255, 
  0,   1,  16,  25,  36,  47,  57,  70,  82,  94, 105, 116, 127, 139, 150, 161, 171, 181, 190, 198, 205, 212, 217, 223, 229, 235, 241, 246, 250, 253, 255, 255, 255, 
  0,   3,  14,  24,  35,  46,  57,  68,  79,  90, 102, 113, 125, 136, 148, 158, 169, 179, 188, 197, 204, 210, 216, 222, 228, 234, 240, 246, 250, 253, 255, 255, 255, 
  0,   1,  12,  22,  32,  44,  54,  65,  77,  88, 100, 112, 122, 135, 145, 156, 167, 177, 187, 195, 202, 209, 215, 221, 227, 234, 239, 245, 250, 253, 255, 255, 255, 
  0,   0,  10,  21,  29,  40,  52,  63,  74,  86,  97, 108, 120, 131, 142, 153, 164, 175, 184, 192, 201, 207, 214, 220, 226, 232, 238, 245, 249, 252, 254, 255, 255, 
  0,   0,   5,  18,  27,  38,  48,  59,  71,  82,  94, 104, 116, 127, 138, 149, 160, 170, 180, 189, 197, 204, 211, 217, 223, 230, 236, 243, 248, 252, 254, 255, 255, 
  0,   0,   0,  14,  24,  35,  45,  56,  67,  78,  90, 101, 112, 123, 134, 145, 156, 167, 177, 186, 195, 202, 209, 216, 222, 228, 236, 242, 247, 251, 254, 255, 255, 
  0,   0,   0,  10,  22,  32,  43,  52,  64,  75,  87,  98, 109, 121, 132, 143, 154, 165, 175, 184, 193, 201, 208, 214, 221, 228, 235, 241, 247, 251, 254, 255, 255, 
  0,   0,   0,   7,  20,  29,  39,  50,  60,  72,  84,  95, 106, 118, 129, 140, 151, 162, 172, 182, 191, 199, 206, 213, 219, 226, 233, 240, 246, 251, 254, 255, 255, 
  0,   0,   0,   0,  15,  25,  35,  46,  57,  68,  80,  91, 102, 114, 125, 137, 148, 159, 169, 180, 188, 197, 205, 211, 218, 225, 232, 239, 245, 250, 253, 255, 255, 
  0,   0,   0,   0,  14,  24,  34,  45,  56,  65,  76,  88, 100, 112, 123, 134, 144, 156, 167, 177, 186, 195, 203, 210, 217, 224, 231, 238, 245, 250, 253, 255, 255, 
  0,   0,   0,   0,  11,  22,  31,  43,  53,  63,  73,  85,  96, 108, 119, 130, 141, 152, 163, 174, 183, 192, 200, 208, 215, 221, 229, 236, 244, 249, 253, 255, 255, 
  0,   0,   0,   0,   9,  20,  29,  39,  50,  60,  71,  81,  93, 104, 116, 127, 137, 149, 160, 171, 181, 189, 198, 206, 213, 220, 228, 236, 243, 248, 252, 255, 255, 
  0,   0,   0,   0,   0,  15,  24,  35,  46,  56,  66,  78,  88,  99, 112, 124, 135, 146, 157, 168, 178, 188, 196, 204, 212, 219, 226, 235, 242, 248, 252, 255, 255, 
  0,   0,   0,   0,   0,  14,  23,  33,  45,  53,  63,  75,  86,  97, 109, 120, 131, 143, 154, 165, 175, 185, 194, 203, 210, 218, 225, 233, 241, 247, 252, 255, 255, 
  0,   0,   0,   0,   0,   9,  20,  29,  41,  50,  60,  70,  82,  93, 105, 116, 128, 140, 151, 162, 173, 183, 192, 201, 209, 217, 225, 233, 241, 247, 252, 255, 255, 
  0,   0,   0,   0,   0,   4,  16,  26,  36,  47,  56,  67,  79,  89, 101, 112, 124, 136, 147, 159, 170, 180, 190, 199, 208, 216, 223, 232, 240, 246, 251, 255, 255, 
  0,   0,   0,   0,   0,   5,  17,  27,  36,  46,  56,  65,  77,  87,  99, 110, 121, 133, 144, 156, 167, 178, 188, 196, 205, 214, 221, 230, 238, 246, 251, 254, 255, 
  0,   0,   0,   0,   0,   0,  11,  22,  31,  42,  51,  61,  73,  83,  95, 105, 117, 129, 140, 152, 163, 174, 185, 194, 203, 212, 220, 228, 237, 245, 250, 254, 255, 
  0,   0,   0,   0,   0,   0,   4,  17,  27,  36,  47,  56,  67,  78,  90, 101, 113, 125, 136, 148, 160, 171, 182, 192, 202, 211, 220, 228, 237, 245, 250, 254, 255, 
  0,   0,   0,   0,   0,   0,   3,  16,  26,  35,  44,  55,  65,  76,  87,  98, 109, 122, 134, 145, 157, 168, 180, 190, 200, 209, 218, 227, 236, 244, 250, 254, 255, 
  0,   0,   0,   0,   0,   0,   0,  12,  22,  31,  41,  51,  61,  72,  83,  95, 106, 117, 130, 141, 152, 164, 176, 187, 197, 207, 216, 225, 234, 242, 249, 254, 255, 
  0,   0,   0,   0,   0,   0,   0,  10,  20,  29,  38,  48,  58,  69,  80,  91, 102, 115, 126, 138, 149, 161, 173, 184, 195, 205, 215, 224, 233, 241, 248, 253, 255, 
  0,   0,   0,   0,   0,   0,   0,  10,  20,  29,  38,  47,  56,  67,  78,  89, 100, 112, 124, 134, 146, 158, 170, 181, 192, 203, 213, 222, 232, 240, 248, 253, 255, 
  0,   0,   0,   0,   0,   0,   0,   1,  14,  24,  32,  42,  52,  62,  73,  83,  95, 107, 118, 130, 141, 153, 165, 177, 188, 200, 210, 220, 230, 239, 247, 252, 255, 
  0,   0,   0,   0,   0,   0,   0,   0,   9,  18,  27,  37,  47,  56,  67,  78,  89, 102, 113, 125, 136, 148, 160, 173, 185, 197, 208, 218, 228, 238, 246, 252, 255, 
  0,   0,   0,   0,   0,   0,   0,   0,   1,  13,  23,  31,  42,  51,  61,  73,  83,  96, 107, 119, 131, 143, 155, 168, 181, 193, 204, 216, 227, 236, 245, 252, 255, 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   9,  19,  28,  37,  47,  57,  67,  78,  90, 102, 114, 126, 138, 151, 163, 177, 189, 202, 213, 224, 234, 244, 252, 255, 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  12,  22,  30,  40,  50,  60,  71,  83,  95, 107, 118, 131, 144, 158, 172, 185, 198, 210, 221, 232, 242, 250, 255, 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  12,  22,  31,  41,  51,  61,  74,  86,  98, 110, 123, 136, 149, 165, 180, 194, 206, 219, 229, 240, 249, 255, 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   6,  17,  26,  36,  46,  55,  66,  79,  91, 103, 116, 130, 144, 159, 175, 190, 204, 217, 227, 238, 248, 255, 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  10,  20,  29,  40,  51,  61,  73,  86,  99, 113, 128, 144, 161, 179, 196, 211, 222, 234, 245, 255, 
};
#endif



UINT8 FB_Table[FB_TBL_SIZE]=
/*	0	16	32	48	64	80	96	112	128	144	160	176	192	208	224	240	255*/
{
0x00, 0x1D, 0x30, 0x3B, 0x4A, 0x57, 0x5C, 0x64, 0x6B, 0x75, 0x7A, 0x81, 0x87, 0x8E, 0x93, 0x9B, 0xA0,
0x00, 0x08, 0x18, 0x2F, 0x39, 0x40, 0x4F, 0x59, 0x62, 0x6A, 0x72, 0x78, 0x7F, 0x86, 0x8C, 0x93, 0x9A,
0x00, 0x05, 0x10, 0x1B, 0x2D, 0x39, 0x42, 0x4D, 0x56, 0x60, 0x67, 0x70, 0x78, 0x80, 0x88, 0x8D, 0x93,
0x00, 0x08, 0x0E, 0x18, 0x20, 0x2D, 0x36, 0x40, 0x4E, 0x56, 0x5F, 0x67, 0x70, 0x75, 0x7D, 0x84, 0x8D,
0x00, 0x05, 0x09, 0x18, 0x20, 0x28, 0x35, 0x3E, 0x4A, 0x53, 0x5C, 0x63, 0x6D, 0x74, 0x7C, 0x84, 0x8B,
0x00, 0x06, 0x0A, 0x17, 0x20, 0x28, 0x30, 0x3C, 0x48, 0x51, 0x59, 0x63, 0x6A, 0x72, 0x7A, 0x82, 0x8A,
0x00, 0x05, 0x0A, 0x15, 0x1B, 0x28, 0x30, 0x38, 0x41, 0x4A, 0x55, 0x5E, 0x66, 0x6D, 0x77, 0x7D, 0x86,
0x00, 0x06, 0x0B, 0x12, 0x19, 0x23, 0x30, 0x38, 0x40, 0x48, 0x54, 0x5C, 0x65, 0x6D, 0x75, 0x7E, 0x85,
0x00, 0x06, 0x0A, 0x11, 0x17, 0x23, 0x2C, 0x38, 0x40, 0x48, 0x51, 0x5A, 0x64, 0x6D, 0x74, 0x7C, 0x85,
0x00, 0x05, 0x09, 0x10, 0x1A, 0x21, 0x2E, 0x38, 0x40, 0x48, 0x50, 0x58, 0x63, 0x6B, 0x73, 0x7C, 0x84,
0x00, 0x06, 0x0A, 0x10, 0x14, 0x1F, 0x27, 0x31, 0x3C, 0x48, 0x50, 0x58, 0x60, 0x6A, 0x73, 0x7B, 0x83,
0x00, 0x06, 0x0A, 0x0F, 0x14, 0x1E, 0x28, 0x32, 0x3B, 0x48, 0x50, 0x58, 0x60, 0x68, 0x73, 0x7B, 0x83,
0x00, 0x05, 0x0A, 0x0F, 0x15, 0x20, 0x28, 0x32, 0x3C, 0x45, 0x4E, 0x58, 0x60, 0x68, 0x71, 0x79, 0x82,
0x00, 0x06, 0x0A, 0x0F, 0x13, 0x1D, 0x25, 0x2F, 0x39, 0x42, 0x4C, 0x58, 0x60, 0x68, 0x70, 0x78, 0x81,
0x00, 0x06, 0x0A, 0x0E, 0x13, 0x1D, 0x25, 0x2E, 0x38, 0x42, 0x4C, 0x54, 0x60, 0x68, 0x70, 0x78, 0x80,
0x00, 0x05, 0x0A, 0x0E, 0x12, 0x1D, 0x25, 0x2D, 0x38, 0x42, 0x4C, 0x55, 0x5D, 0x68, 0x70, 0x78, 0x80,
0x00, 0x06, 0x09, 0x0E, 0x13, 0x1C, 0x23, 0x2C, 0x36, 0x41, 0x4A, 0x54, 0x5E, 0x67, 0x70, 0x78, 0x80,
};

UINT8 PCID_Table[PCID_TBL_SIZE];


const OD_REGTBL_T CODE  OD_INIT[]=
{
    {OD_REG00, 0x02040201, 0xffffffff},
    {OD_REG01, 0x00070E01, 0xffffffff},
    {OD_REG02, 0x00000480, 0xffffffff},
    {OD_REG03, 0x00E0FF04, 0xffffffff},
    //{OD_REG06, 0x00000000, 0xffffffff}, //dram address
    //{OD_REG27, 0x000000a0, 0x000000ff}, //dram address
    //{OD_REG08, 0x04380780, 0xffffffff}, //frame protect width and heigh
    //{OD_REG09, 0x000A0048, 0xffffffff},
    //{OD_REG10, 0xffffffff, 0xffffffff},
    //{OD_REG11, 0x00081388, 0xffffffff},
    
    {OD_REG30, 0x00010042, 0xffffffff}, //set cr rate ==0x42
    //{OD_REG32, 0x04055600, 0xffffffff},
    {OD_REG33, 0x00021000, 0xffffffff},
    {OD_REG34, 0x0674A0E6, 0xffffffff},
    {OD_REG35, 0x000622E0, 0xffffffff},//0x00051170
    {OD_REG36, 0xF0049C40, 0xffffffff},//0xF0044E20
    {OD_REG37, 0x05CFDE08, 0xffffffff},//0x04E6D998
    {OD_REG38, 0x011F1010, 0xffffffff},//line size=0xf8
    {OD_REG39, 0x00200000, 0x00ff0000},//dram_crc_cnt=0x20
    {OD_REG40, 0x20000610, 0xffffffff},//0x27207210         //GM_EN=1
    {OD_REG41, 0x001E02D0, 0xffffffff},
    {OD_REG42, 0x00327C7C, 0xffffffff},
    {OD_REG43, 0x00180003, 0xffffffff}, //enable film mode detection
    {OD_REG44, 0x006400C8, 0xffffffff},
    {OD_REG45, 0x00000032, 0x000000fc},
    {OD_REG46, 0x4E00023F, 0xffffffff},
    {OD_REG47, 0x4006B16A, 0xffffffff},
    {OD_REG48, 0x10240408, 0xffffffff},
    {OD_REG49, 0x02900000, 0x0ff00000},//fix div7 
    //{OD_REG12, 0x00000001, 0xffffffff},
    {OD_REGTBL_END, 0x00000000, 0x00000000}
};


/**************************************************************************  
 * Function Members
 *************************************************************************/
void vDrvODLoadRegTbl(const OD_REGTBL_T* prRegTbl)
{
	while (prRegTbl->u4Addr != OD_REGTBL_END)
	{
		vIO32Write4BMsk(prRegTbl->u4Addr, prRegTbl->u4Value, prRegTbl->u4Mask);
		prRegTbl++;
	}
}

void vDrvODOnOff(UINT8 u1OnOff)
{
    if (u1OnOff)
    {
        vIO32WriteFldAlign(OD_REG00, 1, OD_ENABLE);
        vIO32WriteFldAlign(OD_REG39, 0, WDRAM_DIS);
        vIO32WriteFldAlign(OD_REG39, 0, RDRAM_DIS);
    }
    else
    {
        vIO32WriteFldAlign(OD_REG00, 0, OD_ENABLE);
        vIO32WriteFldAlign(OD_REG39, 1, WDRAM_DIS);
        vIO32WriteFldAlign(OD_REG39, 1, RDRAM_DIS);
    }
    //vIO32WriteFldAlign(OD_REG12, u1OnOff, RG_OD_START);
}

void vDrvODBypass(UINT8 u1OnOff)
{
    vIO32WriteFldAlign(OD_REG00, u1OnOff, BYPASS_ALL);
}

void vDrvODReset(void)
{
    vIO32WriteFldAlign(OD_REG01, 1, OD_RST);
    vIO32WriteFldAlign(OD_REG01, 0, OD_RST);
}


void vDrvODClearDramCRC(void)
{
    vIO32WriteFldAlign(OD_REG38, 1, DRAM_CRC_CLR);
    vIO32WriteFldAlign(OD_REG38, 0, DRAM_CRC_CLR);
}

void vDrvODWriteTable(UINT8 TableSel, UINT8 ColorSel, UINT8* pTable)
{
    UINT32 i, u4TblSize;
    
    if (TableSel == FB_TABLE_17)
    {
        u4TblSize = FB_TBL_SIZE;
    }
    else if (TableSel == OD_TABLE_33)
    {
        u4TblSize = OD_TBL_M_SIZE;
        vIO32WriteFldAlign(OD_REG32, 0, OD_IDX_41);
    }
    else
    {
        u4TblSize = OD_TBL_L_SIZE;
        vIO32WriteFldAlign(OD_REG32, 1, OD_IDX_41);
    }

    //get channel and write channel
    vIO32WriteFldAlign(OD_REG12, 0, RG_OD_START);	//OD_Start[0] = 0
    vIO32WriteFldAlign(OD_REG04, 1, TABLE_ONLY_W_ADR_INC);
    vIO32WriteFldAlign(OD_REG04, 0, ADDR_YX);
    
    if (TableSel != FB_TABLE_17)
    {
        vIO32WriteFldAlign(OD_REG04, (ColorSel == OD_ALL) ? 7 : (1<<ColorSel), TABLE_RW_SEL_OD_BGR);
    }
    else
    {
        vIO32WriteFldAlign(OD_REG04, (ColorSel == OD_ALL) ? 7 : (1<<ColorSel), TABLE_RW_SEL_FB_BGR);
    }
    
    for (i = 0; i < u4TblSize; i++)
    {
        vIO32Write1B(OD_REG05, pTable[i]);
    }

    vIO32WriteFldAlign(OD_REG04, 0, OD_FLD_ALL);
    vIO32WriteFldAlign(OD_REG12, 0, RG_OD_START);
    vIO32WriteFldAlign(OD_REG01, 1, OD_RST);
    vIO32WriteFldAlign(OD_REG01, 0, OD_RST);
    vIO32WriteFldAlign(OD_REG12, 1, RG_OD_START);
}

UINT8 u1DrvODReadTable(UINT8 TableSel, UINT8 ColorSel, UINT8* pTable)
{
    UINT8 u1ErrorFlag = 0;
    UINT32 i, u4TblSize, u4Temp;

    if (TableSel == FB_TABLE_17)
    {
        u4TblSize = FB_TBL_SIZE;
    }
    else if (TableSel == OD_TABLE_33)
    {
        u4TblSize = OD_TBL_M_SIZE;
        vIO32WriteFldAlign(OD_REG32, 0, OD_IDX_41);
    }
    else
    {
        u4TblSize = OD_TBL_L_SIZE;
        vIO32WriteFldAlign(OD_REG32, 1, OD_IDX_41);
    }

    vIO32WriteFldAlign(OD_REG02, 1, ODT_BYPASS);
    vIO32WriteFldAlign(OD_REG04, 0, ADDR_YX);
    
    if (TableSel != FB_TABLE_17)
    {
        vIO32WriteFldAlign(OD_REG04, 1<<ColorSel, TABLE_RW_SEL_OD_BGR);
    }
    else
    {
        vIO32WriteFldAlign(OD_REG04, 1<<ColorSel, TABLE_RW_SEL_FB_BGR);
    }

    for(i = 0; i < u4TblSize; i++)
    {
        u4Temp = IO32ReadFldAlign(OD_REG05, TABLE_RW_DATA);
        
        if (pTable[i] != u4Temp)
        {          	
            LOG(0, "Table Check Fail: i =%d value =%d TableSel =%d ColorSel =%d \n", i, u4Temp, TableSel, ColorSel);
            u1ErrorFlag = 1;
        }
    }

    vIO32WriteFldAlign(OD_REG04, 0, OD_FLD_ALL);
    vIO32WriteFldAlign(OD_REG02, 0, ODT_BYPASS);

    return u1ErrorFlag;
}

void vDrvODStressTest(UINT32 number)
{
    UINT32 i;
    UINT32 error_cnt = 0;
    
    for (i = 0; i < number; i++)
    {
        //Write OD table
        vDrvODWriteTable(OD_TABLE_41_MODE1, OD_ALL, OD_Table41);
        
        //Check OD table
        if (u1DrvODReadTable(OD_TABLE_41_MODE1, OD_RED, OD_Table41))
        {
            error_cnt++;
        }
        
        if (u1DrvODReadTable(OD_TABLE_41_MODE1, OD_GREEN, OD_Table41))
        {
            error_cnt++;
        }

        if (u1DrvODReadTable(OD_TABLE_41_MODE1, OD_BLUE, OD_Table41))
        {
            error_cnt++;
        }    

        //Write FB table
        vDrvODWriteTable(FB_TABLE_17, OD_ALL, FB_Table);
        
        //Check FB table        
        if (u1DrvODReadTable(FB_TABLE_17, OD_RED, FB_Table))
        {
            error_cnt++;
        }
        if (u1DrvODReadTable(FB_TABLE_17, OD_GREEN, FB_Table))
        {
            error_cnt++;
        }
        if (u1DrvODReadTable(FB_TABLE_17, OD_BLUE, FB_Table))
        {
            error_cnt++;
        }
    }

    LOG(0, "error cnt =%d\n", error_cnt);
}

 
/**************************************************************************  
 * Function Members
 *************************************************************************/

void vDrvODLoadODTableVsync(UINT8* pTable )
{
    LOG(0, "Load OD table\n");
    pODTable = pTable;
}

/**
 * @brief vDrvLoadODTable
 * Load OD table
 * @param  void
 * @retval void
 */
void vDrvODLoadODTable(void)
{
    LOG(0, "Load OD table\n");
    
    //Write OD table
    vDrvODWriteTable(OD_TABLE_CONFIG, OD_ALL, pODTable);
    //Check OD table
    u1DrvODReadTable(OD_TABLE_CONFIG, OD_RED, pODTable);
    u1DrvODReadTable(OD_TABLE_CONFIG, OD_GREEN, pODTable);
    u1DrvODReadTable(OD_TABLE_CONFIG, OD_BLUE, pODTable);
}

/**
 * @brief vDrvLoadODTable
 * Load FB table
 * @param  void
 * @retval void
 */
void vDrvODLoadFBTable(void)
{	
    LOG(0, "Load FB table\n");	
    
    //Write FB table
    vDrvODWriteTable(FB_TABLE_17, OD_ALL, FB_Table);
    //Check FB table
    u1DrvODReadTable(FB_TABLE_17, OD_RED, FB_Table);
    u1DrvODReadTable(FB_TABLE_17, OD_GREEN, FB_Table);
    u1DrvODReadTable(FB_TABLE_17, OD_BLUE, FB_Table);	
}

void vDrvODSetBufAddress(void)
{
    FBM_POOL_T* prPool;
    UINT32 u4ODAddress;
    
    prPool = FBM_GetPoolInfoAuto((UCHAR)FBM_POOL_TYPE_OD, NULL);
    u4ODAddress = prPool->u4Addr+63;
    
    vIO32WriteFldAlign(OD_REG06, u4ODAddress>>6, FRAM0_ADR);
    vIO32WriteFldAlign(OD_REG27, u4ODAddress>>22, FRAM0_ADR_MSB);    
}


/**
 * @brief vDrvFrameProtectInit(void)
 * Frame Protection Init
 * @param  void
 * @retval void
 */
void vDrvFrameProtectionInit(void)
{
    vIO32WriteFldAlign(OD_REG08, OD_PANEL_WIDTH, OD_H_ACTIVE);
    vIO32WriteFldAlign(OD_REG32, OD_PANEL_WIDTH, OD_DE_WIDTH);
    vIO32WriteFldAlign(OD_REG08, OD_PANEL_HEIGHT, OD_V_ACTIVE);    
    
    vIO32WriteFldAlign(OD_REG09, 0x48, RG_H_BLANK);     //h_blank  = htotal - h_active
    vIO32WriteFldAlign(OD_REG09, 0xA, RG_H_OFFSET);     //tolerrance       
    
    vIO32WriteFldAlign(OD_REG10, 0xFFF, RG_H_BLANK_MAX);            
    vIO32WriteFldAlign(OD_REG10, 0xFFFF, RG_V_BLANK_MAX);    //pixel-based counter
    
    vIO32WriteFldAlign(OD_REG11, 0x1388, RG_V_BLANK);    //v_blank  = vtotal - v_active    
    vIO32WriteFldAlign(OD_REG11, 2, RG_FRAME_SET);     

    //pattern gen init
    vIO32WriteFldAlign(OD_REG25, 0, OD_FLD_ALL);
    vIO32WriteFldAlign(OD_REG22, 0, OD_FLD_ALL);
}

#if OD_FW_WA1
void vDrvODCheck(void)
{
    UINT32 u4OD_Curr_CRC;
    static UINT32 u4ODChk_cnt, u4OD_CRC;

    if (u4ODCheck == 1)
    {
        vIO32WriteFldAlign(OD_REG02, 1, FORCE_8B_BTC);
        vIO32WriteFldAlign(OD_REG02, 1, FORCE_8B_DC);
        vIO32WriteFldAlign(OD_REG02, 1, ODT_BYPASS);
        vIO32WriteFldAlign(OD_REG02, 1, DISPLAY_COMPRESSION);
        vIO32WriteFldAlign(OD_REG49, 1, RDRAM_MODEL);       
        
        vIO32WriteFldAlign(OD_REG32, 0, OD_CRC_START);
        vIO32WriteFldAlign(OD_REG32, 1, OD_CRC_CLR);
        vIO32WriteFldAlign(OD_REG32, 0, OD_CRC_CLR);
        vIO32WriteFldAlign(OD_REG32, 1, OD_CRC_START);
        
        u4ODChk_cnt = 0;
        u4ODCheck = 2; // go to read CRC data
    }

    if ((u4ODCheck == 2) && (IO32ReadFldAlign(OD_REG53, CRC_RDY) == 1))
    {       
        u4OD_Curr_CRC = IO32ReadFldAlign(OD_REG53, CRC_OUT);
        LOG(0, "u4OD_Curr_CRC = %d\n", u4OD_Curr_CRC);
        
        if (u4OD_CRC == u4OD_Curr_CRC)
        {
            u4ODChk_cnt++;
            LOG(0, "CRC Correct %d\n", u4ODChk_cnt);
        }
        else
        {
            u4ODCheck = 1;    //return to CRC clear
            u4OD_CRC = u4OD_Curr_CRC;
            vDrvODReset();
            LOG(0, "CRC different and Reset\n");
        }

        if (u4ODChk_cnt == 8)
        {
            u4ODCheck = 0;
            vIO32WriteFldAlign(OD_REG02, 0, FORCE_8B_BTC);
            vIO32WriteFldAlign(OD_REG02, 0, FORCE_8B_DC);
            vIO32WriteFldAlign(OD_REG02, 0, ODT_BYPASS);
            vIO32WriteFldAlign(OD_REG02, 0, DISPLAY_COMPRESSION);
            vIO32WriteFldAlign(OD_REG49, 0, RDRAM_MODEL);
            LOG(0, "OD Ready\n");
        }
    }
}   

#endif

#if OD_FW_WA2
void vDrvODDramCRCCheck(void)
{
    static UINT32 fgDramCRCReset = 0;
    
    if (u4ODCheck != 0)
    {
        return;
    }
    
    if (fgDramCRCReset)
    {
        vDrvODOnOff(SV_ON);
        fgDramCRCReset = 0;
        LOG(0, "OD dram CRC Reset \n");
        return;
    }

    if (IO32ReadFldAlign(OD_REG50, DRAM_CRC_ERROR) == 1)
    {
        vDrvODOnOff(SV_OFF);
        vDrvODClearDramCRC();
        fgDramCRCReset = 1;
        LOG(0, "OD dram CRC error \n");
        return;
    }
}
#endif

void vDrvODParam(void)
{
    UINT32 u4GMV_width;    
   
    //set gmv detection width
    u4GMV_width = OD_PANEL_WIDTH/6;
    vIO32WriteFldAlign(OD_REG40, (u4GMV_width*1)>>4, GM_R0_CENTER);
    vIO32WriteFldAlign(OD_REG40, (u4GMV_width*2)>>4, GM_R1_CENTER);
    vIO32WriteFldAlign(OD_REG41, (u4GMV_width*3)>>4, GM_R2_CENTER);
    vIO32WriteFldAlign(OD_REG41, (u4GMV_width*4)>>4, GM_R3_CENTER);
    vIO32WriteFldAlign(OD_REG42, (u4GMV_width*5)>>4, GM_R4_CENTER);
    
    vIO32WriteFldAlign(OD_REG43, 12>>2,GM_V_ST);
    vIO32WriteFldAlign(OD_REG43, (OD_PANEL_HEIGHT-12)>>2, GM_V_END);
    vIO32WriteFldAlign(OD_REG42, (100*OD_PANEL_HEIGHT)/1080, GM_LGMIN_DIFF);
    vIO32WriteFldAlign(OD_REG44, (400*OD_PANEL_HEIGHT)/1080, GM_LMIN_THR);
    vIO32WriteFldAlign(OD_REG44, (200*OD_PANEL_HEIGHT)/1080, GM_GMIN_THR);

    //set auto 8bit parameters
    if (OD_PANEL_WIDTH >1900)
    {
        vIO32WriteFldAlign(OD_REG35, 140000, DET8B_DC_NUM);
        vIO32WriteFldAlign(OD_REG36, 40000, DET8B_BTC_NUM);
        vIO32WriteFldAlign(OD_REG37, 1900000 ,DET8B_BIT_MGN);
    }
    else
    {
        vIO32WriteFldAlign(OD_REG35, 70000, DET8B_DC_NUM);
        vIO32WriteFldAlign(OD_REG36, 20000, DET8B_BTC_NUM);
        vIO32WriteFldAlign(OD_REG37, 950000 ,DET8B_BIT_MGN);
    }

    //set OD threshold
    vIO32WriteFldAlign(OD_REG01, 10, MOTION_THR);
    vIO32WriteFldAlign(OD_REG48, 8, ODT_INDIFF_TH);
    vIO32WriteFldAlign(OD_REG02, 1, FBT_BYPASS);
}

void vDrvODDramOverflowReset(void)
{
    vIO32WriteFldAlign(OD_REG47, 1, WOV_CLR);
    vIO32WriteFldAlign(OD_REG47, 1, ROV_CLR);
    vIO32WriteFldAlign(OD_REG47, 0, WOV_CLR);
    vIO32WriteFldAlign(OD_REG47, 0, ROV_CLR);
}

UINT32 u4DrvDramOverflow(void)
{    
    if (IO32ReadFldAlign(OD_REG52, R_UNDERFLOW) == 1)
    {
        LOG(0, "OD dram read overflow \n");
        return 0;
    }
    else if (IO32ReadFldAlign(OD_REG52, W_UNDERFLOW) == 1)
    {
        LOG(0, "OD dram write overflow \n");
        return 0;
    }    
    else 
    {
        LOG(0, "OD dram No overflow \n");
        return 0;
    }    
}

void vDrvODCheckEnable(void)
{
    #if OD_FW_WA1
    u4ODCheck = 1;
    #else
    LOG(0, "OD check not support \n");
    #endif
}

/**
 * @brief vDrvOdInit
 * Init OD driver
 * @param  void
 * @retval void
 */
void vDrvODInit(void)
{	
    vErrorHandleInit();
    vDrvODSetBufAddress();
    vDrvFrameProtectionInit();
    vDrvODLoadRegTbl(OD_INIT);    

#if OD_EXT_FP
    LOG(0, "OD use external error handling\n");
    vIO32WriteFldAlign(OD_REG32, 1, OD_EXT_FP_EN);
    vIO32WriteFldAlign(OD_REG32, 1, OD_USE_EXT_DE_TOTAL);
    vIO32WriteFldAlign(OD_REG37, 1, OD_USE_EXT_DE_OUT);

    #ifdef CC_MT5365
        vIO32WriteFldAlign(OD_REG37, 1, DE_PROTECT_EN);
        vIO32WriteFldAlign(OD_REG37, 1, VDE_PROTECT_EN);
    #endif
    #ifdef CC_MT5395
        vIO32WriteFldAlign(OD_REG49, 1, DE_PROTECT_EN);
        vIO32WriteFldAlign(OD_REG49, 1, VDE_PROTECT_EN);
    #endif

#else
    LOG(0, "OD use internal frame protect\n");
    vIO32WriteFldAlign(OD_REG32, 0, OD_EXT_FP_EN);
    vIO32WriteFldAlign(OD_REG32, 0, OD_USE_EXT_DE_TOTAL);
    vIO32WriteFldAlign(OD_REG37, 0, OD_USE_EXT_DE_OUT);

    #ifdef CC_MT5365
        vIO32WriteFldAlign(OD_REG37, 0, DE_PROTECT_EN);
        vIO32WriteFldAlign(OD_REG37, 0, VDE_PROTECT_EN);
    #endif
    #ifdef CC_MT5395
        vIO32WriteFldAlign(OD_REG49, 0, DE_PROTECT_EN);
        vIO32WriteFldAlign(OD_REG49, 0, VDE_PROTECT_EN);
    #endif

#endif
    vDrvODLoadODTableVsync(OD_Table);
    vDrvODLoadODTable();
    vDrvODLoadFBTable();	
    vDrvODParam();

#if OD_FW_WA1
    vIO32WriteFldAlign(OD_REG12, 1, RG_OD_START);
    vDrvODBypass(SV_OFF);
    vDrvODCheckEnable();
#endif

#ifdef CC_SUPPORT_OD
    // Disable OD when panel is SYNC mode
    if ((GetCurrentPanelIndex() != 0) && (GetCurrentPanelIndex() != 1) && (IS_OD_ENABLE == OD_ON))
    {
        LOG(0, "Support OD\n");
        u1SupportOD=1;
        vDrvODOnOff(SV_ON);
        vDrvODBypass(SV_OFF);
        u1TconSourceSet(RG_MLVDSM_IN_SEL_EH_DI_OD);
    }
    else
#endif    
    {
        LOG(0, "NOT Support OD\n");
        u1SupportOD=0;
        vDrvODOnOff(SV_OFF);
        vDrvODBypass(SV_ON);
        if((!(PANEL_IsSupportMlvds()))&&((GetCurrentPanelIndex() == 0) || (GetCurrentPanelIndex() == 1)))
        {        
         u1TconSourceSet(RG_MLVDSM_IN_SEL_OTSG);        
        }
    } 
    
    vIO32WriteFldAlign(OD_REG12, 1, RG_OD_START);
}

void OD_OnOutputVSync(void)
{
#ifdef CC_SUPPORT_OD    //if (DRVCUST_OptGet(eFlagSupportOD) == SV_ON)

    #if OD_FW_WA1
        if ((BSP_GetIcVersion() == IC_VER_5365_AA) || (BSP_GetIcVersion() == IC_VER_5395_AA))
        {
            vDrvODCheck();
        }    
    #endif

    #if OD_FW_WA2
        vDrvODDramCRCCheck();
    #endif

#endif    
}

