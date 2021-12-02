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
 * $RCSfile: tshift_buf.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file ts_manual.c
 *  TimeShift demo application
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "x_assert.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_hal_926.h"

#include "dmx_mm_if.h"
#include "fvr_if.h"

#define DEFINE_IS_LOG	CLI_IsLog
#include "x_debug.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
// Buffer
#define TSHIFT_RECBUF_SIZE          		   0x200000
#define TSHIFT_PLAYBUF_SIZE         		   0x200000

#define TSHIFT_REC_PKTCOUNT                    0x200
#define TSHIFT_PLAY_THRESHOLD                  0x20000

#define TSHIFT_DATA_SIZE			           0x1000000
#define TSHIFT_DATA_CHUNK_SIZE		           0x200000
#define TSHIFT_CHUNK_NUM			           (TSHIFT_DATA_SIZE / TSHIFT_DATA_CHUNK_SIZE)

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define EMPTYSIZE(rp, wp, size)	    \
    (((wp) < (rp)) ? ((rp) - (wp)) : (((rp) + (size)) - (wp)))

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

// Data management
static UINT32 _u4TShiftBufStart, _u4TShiftBufEnd, _u4TShiftWP, _u4TShiftRP;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** _TShiftBufGetEmptySize
 */
//-----------------------------------------------------------------------------
static UINT32 _TShiftBufGetEmptySize()
{
    return EMPTYSIZE(_u4TShiftRP, _u4TShiftWP, (_u4TShiftBufEnd - _u4TShiftBufStart));
}


//-----------------------------------------------------------------------------
/** _TShiftBufGetDataSize
 */
//-----------------------------------------------------------------------------
static UINT32 _TShiftBufGetDataSize()
{
    return DATASIZE(_u4TShiftRP, _u4TShiftWP, (_u4TShiftBufEnd - _u4TShiftBufStart));
}


//-----------------------------------------------------------------------------
/** _TShiftBufFillData
 */
//-----------------------------------------------------------------------------
static BOOL _TShiftBufFillData(UINT32 u4Addr, UINT32 u4Size)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** _TShiftBufGetData
 */
//-----------------------------------------------------------------------------
static BOOL _TShiftBufGetData(UINT32 u4Addr, UINT32 u4Size)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** TShift_Buf_Init
 */
//-----------------------------------------------------------------------------
BOOL TShift_Buf_Init(UINT32 u4BufSize)
{
    _u4TShiftBufStart = (UINT32)x_mem_alloc(u4BufSize);
    if(_u4TShiftBufStart == 0)
    {        
        return FALSE;
    }
    
    _u4TShiftWP = _u4TShiftBufStart;
    _u4TShiftRP = _u4TShiftBufStart;
    _u4TShiftBufEnd = _u4TShiftBufStart + u4BufSize;

    return TRUE;
}


//-----------------------------------------------------------------------------
/** TShift_Buf_Write
 */
//-----------------------------------------------------------------------------
BOOL TShift_Buf_Write(UINT32 u4Addr, UINT32 u4Size, UINT32* pu4RetSize)
{
    UINT32 u4EmptySize, u4RetAddr, u4RetSize, u4CpSize, u4CpAddr;
    
    u4EmptySize = _TShiftBufGetEmptySize();

    *pu4RetSize = 0x0;
    if(u4EmptySize < u4Size)
    {
        Printf("Capture empty size is not enough\n");
        return FALSE;
    }
    else
    {
        while(1)
        {
            if(!FVR_GetWritePointer(u4Addr, u4Size, &u4RetAddr, &u4RetSize))
            {
                LOG(3, "Get Buffer fail\n");
            }

            // Copy data, blocking operation
            if(!_TShiftBufFillData(u4Addr, u4RetSize))
            {
                LOG(3, "Write timeshift buffer fail\n");
            }

            u4Size -= u4RetSize;
            u4Addr = u4RetAddr;
            if(u4Size == 0)
            {
                break;
            }
        }
    }    

    return TRUE;
}


//-----------------------------------------------------------------------------
/** TShift_Buf_Read
 */
//-----------------------------------------------------------------------------
BOOL TShift_Buf_Read(UINT32 u4Addr, UINT32 u4Size, UINT32 *pu4RetSize)
{
    UINT32 u4DataSize, u4RetAddr, u4RetSize;

    u4DataSize = _TShiftBufGetDataSize();

    while(1)
    {
        if(!DMX_PVRPlay_GetBufPointer(u4Addr, u4Size, &u4RetAddr, &u4RetSize))
        {
            LOG(3, "Get play buf pointer fail\n");
        }

        // Blocking operation
        if(!_TShiftBufGetData(u4Addr, u4RetSize))
        {
            LOG(3, "Write timeshift buffer fail\n");
        }

        u4Size -= u4RetSize;
        u4Addr = u4RetAddr;

        if(u4Size == 0)
        {
            break;
        }
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** TShift_Meta_Init
 */
//-----------------------------------------------------------------------------
BOOL TShift_Meta_Init(void)
{
    return TRUE;
}


//-----------------------------------------------------------------------------
/** TShift_Meta_Add
 */
//-----------------------------------------------------------------------------
BOOL TShift_Meta_Add(void)
{
    return TRUE;
}

