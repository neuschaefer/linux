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
 * $RCSfile: fbm_misc.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file fbm_misc.c
 */


//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "fbm_drvif.h"
#include "fbm_fb.h"
#include "fbm_cs.h"
#include "x_os.h"
#include "x_assert.h"
#include "x_debug.h"


//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

#ifdef FBM_VIRTUAL_MPEG
#define FBM_VIRTUAL_MACHINE
#endif

#ifdef FBM_VIRTUAL_DISPLAY
#define FBM_VIRTUAL_MACHINE
#endif

#define CC_PLAYIMAGE

#ifdef FBM_VIRTUAL_MACHINE

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define FBM_VD_STACK_SIZE                           2048

#define FBM_VD_THREAD_PRIORITY                      98

#define FBM_VM_STACK_SIZE                           2048

#define FBM_VM_THREAD_PRIORITY                      99


//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Imported variables
//---------------------------------------------------------------------------

extern FBM_FBG_T _arFbg[FBG_MAX_NS];

//---------------------------------------------------------------------------
// Macro definitions
//---------------------------------------------------------------------------

#define VERIFY_FBG(id)                                  \
    ((id >= FBG_MAX_NS) ||                              \
    (_arFbg[id].ucFbgType == FBM_FBG_TYPE_UNKNOWN) ||   \
    (_arFbg[id].ucFbgId != id))

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

static void _FbmVirutualDispRoutine(void* pvArg);

static void _FbmVirutualMpegRoutine(void* pvArg);


//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static UCHAR _ucFbmVirtualDispStart = 0;

static UCHAR _ucFbmVirtualDispDestroy = 0;

static UINT32 _ucFbmVirtualDispDelay = 0;

static UCHAR _ucFbmVirtualMpegStart = 0;

static UCHAR _ucFbmVirtualMpegDestroy = 0;

static UINT32 _ucFbmVirtualMpegDelay = 0;

#ifdef CC_PLAYIMAGE
#define FBM_VM_START_ADDR                           (81 * 1024 * 1024)  // from 65 ~ 256 MB
#define FBM_VM_MAX_NS                               256
UINT32 u4AddrY[FBM_VM_MAX_NS];
UINT32 u4AddrC[FBM_VM_MAX_NS];
UINT32 u4AddrIdx;
UINT32 u4AddrNs;
#endif

//---------------------------------------------------------------------------
// Static functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** _FbmVirutualDispRoutine
 *  VD Thread
 */
//-------------------------------------------------------------------------
static void _FbmVirutualDispRoutine(void* pvArg)
{    
    UCHAR ucFbgIdx;
    UCHAR aucFbLock[FBG_MAX_NS];
    UCHAR aucDecodeStart[FBG_MAX_NS];

    UINT32 u4DisplayCount = 0;
    UINT32 u4EmptyCount = 0;
    
    for (ucFbgIdx = 0; ucFbgIdx < FBG_MAX_NS; ucFbgIdx++)
    {
        aucFbLock[ucFbgIdx] = FBM_FB_ID_UNKNOWN;
        aucDecodeStart[ucFbgIdx] = 0;
    }

    while (_ucFbmVirtualDispDestroy == 0)
    {
        for (ucFbgIdx = 0; ucFbgIdx < FBG_MAX_NS; ucFbgIdx++)
        {
            if (!VERIFY_FBG(ucFbgIdx))
            {
                UCHAR ucFbId;

                ucFbId = FBM_GetFrameBufferFromDispQ(ucFbgIdx);

                if (ucFbId != FBM_FB_ID_UNKNOWN)
                {
                    if (aucFbLock[ucFbgIdx] != FBM_FB_ID_UNKNOWN)
                    {
                        ASSERT(FBM_CheckFrameBufferStatus(ucFbgIdx, aucFbLock[ucFbgIdx], FBM_FB_STATUS_LOCK));

                        FBM_SetFrameBufferStatus(ucFbgIdx, aucFbLock[ucFbgIdx], FBM_FB_STATUS_EMPTY);

                        aucFbLock[ucFbgIdx] = FBM_FB_ID_UNKNOWN;
                    }
                
                    FBM_SetFrameBufferStatus(ucFbgIdx, ucFbId, FBM_FB_STATUS_LOCK);

                    aucFbLock[ucFbgIdx] = ucFbId;

                    aucDecodeStart[ucFbgIdx] = 1;

                    u4DisplayCount++;
                }
                else
                {
                    if (aucDecodeStart[ucFbgIdx])
                    {
                        LOG(7, "Fbg(%d) DispQ Empty\n", ucFbgIdx);

                        u4EmptyCount++;
                    }
                }
            }
        }

        // delay
        x_thread_delay(_ucFbmVirtualDispDelay);
    }

    _ucFbmVirtualDispStart = 0;

    // destroy thread
    x_thread_exit();
}


//-------------------------------------------------------------------------
/** _FbmVirutualMpegRoutine
 *  VM Thread
 */
//-------------------------------------------------------------------------
static void _FbmVirutualMpegRoutine(void* pvArg)
{
    static UINT32 u4State = 0;
    
    UCHAR ucFbgIdx;
    UCHAR ucFbId;

    ucFbgIdx = FBM_CreateGroup(FBM_FBG_TYPE_PAL, FBM_VDEC_MPEG2, 720, 480);
    ASSERT(ucFbgIdx != FBM_FBG_ID_UNKNOWN);

    FBM_FbgChgNotify(ucFbgIdx, 0);

    _arFbg[ucFbgIdx].u4FbgFlag = (FBM_FLAG_SEQ_CHG | FBM_FLAG_DISP_READY);
    _arFbg[ucFbgIdx].rSeqHdr.u2LineSize = 768;
    _arFbg[ucFbgIdx].rSeqHdr.u2HSize = 704;
    _arFbg[ucFbgIdx].rSeqHdr.u2VSize = 480;
    _arFbg[ucFbgIdx].rSeqHdr.ucAspRatInf = 3;        // 16:9
    _arFbg[ucFbgIdx].rSeqHdr.ucFrmRatCod = 5;        // 30 fps
//    _arFbg[ucFbgIdx].rSeqHdr.fgProgressiveSeq = 1;    // MPEG2
    _arFbg[ucFbgIdx].rSeqHdr.fgProgressiveSeq = 0;    // MPEG2

    for (ucFbId = 0; ucFbId < _arFbg[ucFbgIdx].ucFbNs; ucFbId++)
    {
        x_memset((void *) &(_arFbg[ucFbgIdx].prPicHdr[ucFbId]), 0, sizeof(FBM_PIC_HDR_T));

        _arFbg[ucFbgIdx].prPicHdr[ucFbId].fgTopFldFirst = 1;
//        _arFbg[ucFbgIdx].prPicHdr[ucFbId].fgTopFldFirst = 0;
        _arFbg[ucFbgIdx].prPicHdr[ucFbId].fgRepFirstFld = 0;
        _arFbg[ucFbgIdx].prPicHdr[ucFbId].fgProgressiveFrm = 0;
//        _arFbg[ucFbgIdx].prPicHdr[ucFbId].fgProgressiveFrm = 1;
        
#if 0
        FBM_SetFrameBufferColor(ucFbgIdx, ucFbId, (64 * ucFbId * 0x010101));
#else

        if (ucFbId == 0)
        {
            FBM_SetFrameBufferZebraColor(ucFbgIdx, ucFbId);
        }
        else
        {
            FBM_SetFrameBufferColor(ucFbgIdx, ucFbId, 0x00EB00);
        }
#endif
    }

    while (_ucFbmVirtualMpegDestroy == 0)
    {
        UCHAR ucFbId;
        
        // delay, decode time
        x_thread_delay(_ucFbmVirtualMpegDelay);

            ucFbId =  FBM_GetEmptyRefFrameBuffer(ucFbgIdx, 1000);

//        LOG(1, "VM (%d)\n", ucFbId);

#ifdef CC_PLAYIMAGE
        _arFbg[ucFbgIdx].au4AddrY[ucFbId] = u4AddrY[u4AddrIdx];
        _arFbg[ucFbgIdx].au4AddrC[ucFbId] = u4AddrC[u4AddrIdx];

        if (++u4AddrIdx >= u4AddrNs)
        {
        
            u4AddrIdx = 0;

            // Test
#if 0
            // Deinterlacer R/W Address
            IO_WRITE32(PSCAN_BASE, 0x010, (128 * 1024 * 1024));
            IO_WRITE32(PSCAN_BASE, 0x014, (192 * 1024 * 1024));
            IO_WRITE32(PSCAN_BASE, 0x018, (128 * 1024 * 1024));
            IO_WRITE32(PSCAN_BASE, 0x01C, (192 * 1024 * 1024));

            IO_WRITE32(PSCAN_BASE, 0x054, 0x001F001F);

            // Write Protection
            IO_WRITE32(PSCAN_BASE, 0x060, 0xFFFFFFFF);

            if (++u4State >= 2)
            {
                // DRAM R/W Enable
                IO_WRITE32(PSCAN_BASE, 0x000, 0x2A0);

                LOG(1, "Step Mode\n");
            }            
#endif        
        }

#if 0
        if (++u4State >= 64)
        {
            // DRAM R/W Enable
            IO_WRITE32(PSCAN_BASE, 0x000, 0x2A0);

            LOG(1, "Step Mode\n");
        }
#endif        

#endif

            if (ucFbId != FBM_FB_ID_UNKNOWN)
            {
                FBM_SetFrameBufferStatus(ucFbgIdx, ucFbId, FBM_FB_STATUS_READY);
                FBM_SetFrameBufferStatus(ucFbgIdx, ucFbId, FBM_FB_STATUS_DISPLAYQ);
            }
        }

    FBM_ReleaseGroup(ucFbgIdx);

    _ucFbmVirtualMpegStart = 0;

    // destroy thread
    x_thread_exit();
}

//---------------------------------------------------------------------------
// Inter-file functions
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
// Public functions
//---------------------------------------------------------------------------

//-------------------------------------------------------------------------
/** FBM_VirtualDispStart
 *  Start VP
 */
//-------------------------------------------------------------------------
void FBM_VirtualDispStart(UINT32 u4Delay)
{
    if (_ucFbmVirtualDispStart == 0)
    {
        HANDLE_T hThread1;
        
        _ucFbmVirtualDispDestroy = 0;
        _ucFbmVirtualDispStart = 1;
        
        _ucFbmVirtualDispDelay = u4Delay;

        // create thread
        VERIFY(x_thread_create(&hThread1, "VD Thread", FBM_VD_STACK_SIZE, FBM_VD_THREAD_PRIORITY,
            _FbmVirutualDispRoutine, 0, NULL) == OSR_OK);
    }
}

//-------------------------------------------------------------------------
/** FBM_VirtualDispStop
 *  Stop VP
 */
//-------------------------------------------------------------------------
void FBM_VirtualDispStop(void)
{
    if (_ucFbmVirtualDispStart == 1)
    {
        _ucFbmVirtualDispDestroy = 1;
    }
}

//-------------------------------------------------------------------------
/** FBM_VirtualMpegStart
 *  Start VM
 */
//-------------------------------------------------------------------------
void FBM_VirtualMpegStart(UINT32 u4Delay)
{
#ifdef CC_PLAYIMAGE
    UINT32 u4AddrShift;

    u4AddrNs = 96;
    u4AddrShift = 4;

    for (u4AddrIdx = 0; u4AddrIdx < u4AddrNs; u4AddrIdx++)
    {
        if (u4AddrIdx == 0)
        {
            u4AddrY[0] = FBM_VM_START_ADDR;
        }
        else
        {
            u4AddrY[u4AddrIdx] = u4AddrC[u4AddrIdx - 1] + FBM_FBG_TYPE_PAL_Y_SIZE;
        }

        u4AddrC[u4AddrIdx] = u4AddrY[u4AddrIdx] + FBM_FBG_TYPE_PAL_Y_SIZE;

        LOG(1, "d.load.binary d:/B2R/32pd/out_%d.ramY 0x%x\n", (u4AddrIdx + u4AddrShift) * 1, u4AddrY[u4AddrIdx]);
        LOG(1, "d.load.binary d:/B2R/32pd/out_%d.ramCbCr 0x%x\n", (u4AddrIdx + u4AddrShift) * 1, u4AddrC[u4AddrIdx]);
    }

    u4AddrIdx = 0;
#endif

    if (_ucFbmVirtualMpegStart == 0)
    {
        HANDLE_T hThread1;
        
        _ucFbmVirtualMpegDestroy = 0;
        _ucFbmVirtualMpegStart = 1;

        _ucFbmVirtualMpegDelay = u4Delay;

        // create thread
        VERIFY(x_thread_create(&hThread1, "VM Thread", FBM_VM_STACK_SIZE, FBM_VM_THREAD_PRIORITY,
            _FbmVirutualMpegRoutine, 0, NULL) == OSR_OK);
    }
}

//-------------------------------------------------------------------------
/** FBM_VirtualMpegStop
 *  Stop VM
 */
//-------------------------------------------------------------------------
void FBM_VirtualMpegStop(void)
{
    if (_ucFbmVirtualMpegStart == 1)
    {
        _ucFbmVirtualMpegDestroy = 1;
    }
}

#endif


