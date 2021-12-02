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
/* (c) 2007 Koninklijke Philips Electronics N.V., All rights reserved
 *
 * This source code and any compilation or derivative thereof is the
 * proprietary information of Koninklijke Philips Electronics N.V. and is
 * confidential in nature.
 * Under no circumstances is this software to be exposed to or placed under an
 * Open Source License of any type without the expressed written permission of
 * Koninklijke Philips Electronics N.V.
 */

#include "drv_common.h"
#include "papi_types.h"



//-----------------------------------------------------------------------------
/** MTK_CUST_GetDmxSharedMemoryProperties(void** ppvStart, UINT32* pu4Size)
 *  Get DMX shared memory buffer properties
 *  You should call MTK_CUST_ConfigBoardIO() to read in board configuration file
 *
 *  @param  ppv1Start    Start address of the shared memory buffer for DMX section and PES data
 *  @param  pu4Size      Size (in bytes) of the shared memory buffer for DMX section and PES data
 *
 *  @retval 0            Succeed
 *  @retval -1           Fail
 */
//-----------------------------------------------------------------------------
extern int MTK_CUST_GetDmxSharedMemoryProperty(unsigned int *pu4Start, unsigned int* pu4Size);

//-----------------------------------------------------------------------------
/** MTK_CUST_ConfigBoardIO(char szConfigFile[])
 *  Platform will configure I/O pins according to board IO configuration file.
 *
 *  @param  szIOConfigFile[]          Name of the configuration file to be read in.
 *
 *  @retval 0            Succeed
 *  @retval -1           Fail
 */
//-----------------------------------------------------------------------------
extern int MTK_CUST_ConfigBoardIO(char szConfigFile[]);

//-----------------------------------------------------------------------------
/** MTK_CUST_InstallKeyboard(UINT32 u4ButtonCount, MTK_CUST_KEYBOARD_BUTTON_T arKeyboard[])
 *  Install local keyboard to platform
 *
 *  @param  u4ButtonCount          Button count of the keyboard
 *  @param  arKeyboard             The value (voltage) range of each button of the keyboard
 *
 *  @retval 0            Succeed
 *  @retval -1           Fail
 */
//-----------------------------------------------------------------------------
extern int MTK_CUST_InstallKeyboard(unsigned int u4ButtonCount, MTK_CUST_KEYBOARD_BUTTON_T arKeyboard[]);


//-----------------------------------------------------------------------------
/** MTK_CUST_GetNoClearMemoryProperties(unsigned int *pu4Start, unsigned int* pu4Size)
 *  Get no clear memory properties
 *  @param  pu4Start     Start address of the no clear memory
 *  @param  pu4Size      Size (in bytes) of the no clear memory
 *
 *  @retval 0            Succeed
 *  @retval -1           Fail
 */
//-----------------------------------------------------------------------------
extern int MTK_CUST_GetNoClearMemoryProperties(unsigned int *pu4Start, unsigned int* pu4Size);

//-----------------------------------------------------------------------------
/** MTK_CUST_GetPCLNoClearMemoryProperties(unsigned int *pu4Start, unsigned int* pu4Size)
 *  Get PCL no clear memory properties
 *  @param  pu4Start     Start address of the no clear memory
 *  @param  pu4Size      Size (in bytes) of the no clear memory
 *
 *  @retval 0            Succeed
 *  @retval -1           Fail
 */
//-----------------------------------------------------------------------------
extern int MTK_CUST_GetPCLNoClearMemoryProperties(unsigned int *pu4Start, unsigned int* pu4Size);

//-----------------------------------------------------------------------------
/** MTK_CUST_SetPanelTiming(MTK_CUST_PANEL_ATTR_T *ppdattr)
 *  Set panel timing
 *  @param  ppdattr      The panel setting parameters
 *
 *  @retval 0            Succeed
 *  @retval -1           Fail
 */
//-----------------------------------------------------------------------------
int MTK_CUST_SetPanelTiming(MTK_CUST_PANEL_ATTR_T *ppdattr);

//-----------------------------------------------------------------------------
/** MTK_GetHTimerTicksPerSec(unsigned int* pu4Count)
 *  Get high resolution timer ticks per second
 *  @param  pu4Count     Counts per second
 *  @retval 0            Succeed
 *  @retval -1           Fail
 */
//-----------------------------------------------------------------------------
extern int MTK_GetHTimerTicksPerSec(unsigned int* pu4Count);

//-----------------------------------------------------------------------------
/** MTK_GetHTimerCurrentTick(unsigned int* pu4Tick)
 *  Get current timer tick value
 *  @param  pu4TickH     High 32 bit timer tick value
 *  @param  pu4TickL     Low 32 bit timer tick value
 *  @retval 0            Succeed
 *  @retval -1           Fail
 */
//-----------------------------------------------------------------------------
extern int MTK_GetHTimerCurrentTick(unsigned int* pu4TickH, unsigned int* pu4TickL);


//-----------------------------------------------------------------------------
/** MTK_CUST_SetFeDiversity(MTK_CUST_FE_DIV_T* pFeDiv)
 *  Set FE diversity 
 *  @param  pFeDiv      FE diversity setting parameters
 *
 *  @retval 0            Succeed
 *  @retval -1           Fail
 */
//-----------------------------------------------------------------------------
extern int MTK_CUST_SetFeDiversity(MTK_CUST_FE_DIV_T* pFeDiv);
