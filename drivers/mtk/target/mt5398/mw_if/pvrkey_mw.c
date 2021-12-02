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
 * $RCSfile: $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file pvrkey_mw.c
 *  PVR key management driver
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_if.h"
#include "fvr_if.h"
#include "dmx_mw.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_assert.h"
#include "x_hal_5381.h"
#include "x_pvr.h"
#include "x_os.h"

#define DEFINE_IS_LOG   DMX_IsLog
#include "x_debug.h"

LINT_EXT_HEADER_END

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Import functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
static MWPVR_KEY_INFO_T _arMwPVRRecKey[FVR_SCRAMBLE_PID_INDEX];

static MWPVR_KEY_INFO_T _arMwPVRPlayKey[FVR_SCRAMBLE_PID_INDEX];

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _MwPvrSearchKeyIdx
 */
//-----------------------------------------------------------------------------
static UINT8 _MwPvrSearchKeyIdx(MWPVR_KEY_INFO_T *prKeyInfo, MM_CRYPT_INFO_T *ptEncryptInfo)
{
    UINT8 i, j, u1KeyIdx = 0xFF;
    BOOL fgMatch;

    if((prKeyInfo == NULL) || (ptEncryptInfo == NULL))
    {
        return 0xFF;
    }

    for(i=0; i<FVR_SCRAMBLE_PID_INDEX; i++)
    {
        if(prKeyInfo[i].fgValid &&
           (prKeyInfo[i].rCryptInfo.u.t_pvr_crypt.e_mode == ptEncryptInfo->u.t_pvr_crypt.e_mode) &&
           (prKeyInfo[i].rCryptInfo.u.t_pvr_crypt.e_key_idx == ptEncryptInfo->u.t_pvr_crypt.e_key_idx))
        {
            switch(ptEncryptInfo->u.t_pvr_crypt.e_key_idx)
            {
            case MM_CRYPT_KEY_ODD:
                fgMatch = TRUE;
                for(j=0; j<2; j++)
                {
                    if(prKeyInfo[i].rCryptInfo.u.t_pvr_crypt.aui4_odd_key_1[j] != ptEncryptInfo->u.t_pvr_crypt.aui4_odd_key_1[j])
                    {
                        fgMatch = FALSE;
                        break;
                    }
                    if(prKeyInfo[i].rCryptInfo.u.t_pvr_crypt.aui4_odd_key_2[j] != ptEncryptInfo->u.t_pvr_crypt.aui4_odd_key_2[j])
                    {
                        fgMatch = FALSE;
                        break;
                    }                    
                }

                if(fgMatch)
                {
                    return i;
                }
                break;

            case MM_CRYPT_KEY_EVEN:
                fgMatch = TRUE;
                for(j=0; j<2; j++)
                {
                    if(prKeyInfo[i].rCryptInfo.u.t_pvr_crypt.aui4_even_key_1[j] != ptEncryptInfo->u.t_pvr_crypt.aui4_even_key_1[j])
                    {
                        fgMatch = FALSE;
                        break;
                    }
                    if(prKeyInfo[i].rCryptInfo.u.t_pvr_crypt.aui4_even_key_2[j] != ptEncryptInfo->u.t_pvr_crypt.aui4_even_key_2[j])
                    {
                        fgMatch = FALSE;
                        break;
                    }                    
                }

                if(fgMatch)
                {
                    return i;
                }
                break;

            case MM_CRYPT_KEY_ALTERNATE:
                fgMatch = TRUE;
                for(j=0; j<2; j++)
                {
                    if((prKeyInfo[i].rCryptInfo.u.t_pvr_crypt.aui4_odd_key_1[j] != ptEncryptInfo->u.t_pvr_crypt.aui4_odd_key_1[j]) ||
                       (prKeyInfo[i].rCryptInfo.u.t_pvr_crypt.aui4_even_key_1[j] != ptEncryptInfo->u.t_pvr_crypt.aui4_even_key_1[j]))
                    {
                        fgMatch = FALSE;
                        break;
                    }
                    if((prKeyInfo[i].rCryptInfo.u.t_pvr_crypt.aui4_odd_key_2[j] != ptEncryptInfo->u.t_pvr_crypt.aui4_odd_key_2[j]) ||
                       (prKeyInfo[i].rCryptInfo.u.t_pvr_crypt.aui4_even_key_2[j] != ptEncryptInfo->u.t_pvr_crypt.aui4_even_key_2[j]))
                    {
                        fgMatch = FALSE;
                        break;
                    }                    
                }

                if(fgMatch)
                {
                    return i;
                }
                break;
            }
        }
    }

    return u1KeyIdx;
}


//-----------------------------------------------------------------------------
/** _MwPvrAllocateKeyIdx
 */
//-----------------------------------------------------------------------------
static BOOL _MwPvrAllocateKeyIdx(MWPVR_KEY_INFO_T *prKeyInfo, MM_CRYPT_INFO_T *ptEncryptInfo, 
                          BOOL *pfgMatch, UINT8 *pu1Idx)
{
    UINT8 i, u1KeyIdx;

    if((prKeyInfo == NULL) || (ptEncryptInfo == NULL) || (pfgMatch == NULL) || (pu1Idx == NULL))
    {
        return FALSE;
    }

    *pfgMatch = FALSE;
    *pu1Idx = 0xFF;

    u1KeyIdx = _MwPvrSearchKeyIdx(prKeyInfo, ptEncryptInfo);
    if(u1KeyIdx < FVR_SCRAMBLE_PID_INDEX)
    {
        *pfgMatch = TRUE;
    }
    else
    {
        *pfgMatch = FALSE;

        //-------------------------------------------
        // Search available index
        //-------------------------------------------
        for(i=0; i<FVR_SCRAMBLE_PID_INDEX; i++)
        {
            if(!prKeyInfo[i].fgValid)
            {
                u1KeyIdx = i;
                break;
            }
        }

        if(i == FVR_SCRAMBLE_PID_INDEX)
        {
            LOG(1, "No available key index\n");
            return FALSE;
        }
    }

    *pu1Idx = u1KeyIdx;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwPvrSaveKey
 */
//-----------------------------------------------------------------------------
static BOOL _MwPvrSaveKey(MWPVR_KEY_INFO_T *prKeyInfo, MM_CRYPT_INFO_T *ptEncryptInfo, UINT8 u1KeyIdx)
{
    if((prKeyInfo == NULL) || (ptEncryptInfo == NULL))
    {
        return FALSE;
    }

    if(u1KeyIdx >= FVR_SCRAMBLE_PID_INDEX)
    {
        return FALSE;
    }

    prKeyInfo[u1KeyIdx].fgValid = TRUE;
    prKeyInfo[u1KeyIdx].rCryptInfo = *ptEncryptInfo;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _MwPvrResetKeyInfo
 */
//-----------------------------------------------------------------------------
static BOOL _MwPvrResetKeyInfo(MWPVR_KEY_INFO_T *prKeyInfo)
{
    UINT8 i;

    if(prKeyInfo == NULL)
    {
        return FALSE;
    }

    x_memset((void*)prKeyInfo, 0, sizeof(MWPVR_KEY_INFO_T) * FVR_SCRAMBLE_PID_INDEX);
    
    for(i=0; i<FVR_SCRAMBLE_PID_INDEX; i++)
    {        
        prKeyInfo[i].fgValid = FALSE;        
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _MwPVRAllocatePlayKeyIdx
 */
//-----------------------------------------------------------------------------
BOOL _MwPVRAllocatePlayKeyIdx(MM_CRYPT_INFO_T *ptEncryptInfo, BOOL *pfgMatch, UINT8 *pu1Idx)
{
    if((ptEncryptInfo == NULL) || (pfgMatch == NULL) || (pu1Idx == NULL))
    {
        return FALSE;
    }

    return _MwPvrAllocateKeyIdx(_arMwPVRPlayKey, ptEncryptInfo, pfgMatch, pu1Idx);
}


//-----------------------------------------------------------------------------
/** _MwPVRSavePlayKey
 */
//-----------------------------------------------------------------------------
BOOL _MwPVRSavePlayKey(MM_CRYPT_INFO_T *ptEncryptInfo, UINT8 u1KeyIdx)
{
    if(ptEncryptInfo == NULL)
    {
        return FALSE;
    }
    
    return _MwPvrSaveKey(_arMwPVRPlayKey, ptEncryptInfo, u1KeyIdx);
}


//-----------------------------------------------------------------------------
/** _MwPVRResetPlayKeyInfo
 */
//-----------------------------------------------------------------------------
BOOL _MwPVRResetPlayKeyInfo(void)
{
    return _MwPvrResetKeyInfo(_arMwPVRPlayKey);
}


//-----------------------------------------------------------------------------
/** _MwPVRAllocateRecKeyIdx
 */
//-----------------------------------------------------------------------------
BOOL _MwPVRAllocateRecKeyIdx(MM_CRYPT_INFO_T *ptEncryptInfo, BOOL *pfgMatch, UINT8 *pu1Idx)
{
    if((ptEncryptInfo == NULL) || (pfgMatch == NULL) || (pu1Idx == NULL))
    {
        return FALSE;
    }

    return _MwPvrAllocateKeyIdx(_arMwPVRRecKey, ptEncryptInfo, pfgMatch, pu1Idx);
}


//-----------------------------------------------------------------------------
/** _MwPVRSaveRecKey
 */
//-----------------------------------------------------------------------------
BOOL _MwPVRSaveRecKey(MM_CRYPT_INFO_T *ptEncryptInfo, UINT8 u1KeyIdx)
{
    if(ptEncryptInfo == NULL)
    {
        return FALSE;
    }
    
    return _MwPvrSaveKey(_arMwPVRRecKey, ptEncryptInfo, u1KeyIdx);
}


//-----------------------------------------------------------------------------
/** _MwPVRResetRecKeyInfo
 */
//-----------------------------------------------------------------------------
BOOL _MwPVRResetRecKeyInfo(void)
{
    return _MwPvrResetKeyInfo(_arMwPVRRecKey);
}


