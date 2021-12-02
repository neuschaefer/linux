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
 * $RCSfile: dmx_cmd.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file dmx_cmd.c
 *  Demux CLI commands
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "dmx_mm_if.h"
#include "fvr_if.h"
#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "vdec_drvif.h"
#include "aud_if.h"
#include "aud_drvif.h"
#include "fbm_drvif.h"
#include "stc_drvif.h"
#include "x_drv_cli.h"
#include "x_stl_lib.h"
#include "x_printf.h"
#include "x_assert.h"
#include "x_hal_arm.h"
#include "x_hal_5381.h"
#include "x_rand.h"
#include "x_os.h"
#include "x_bim.h"


//---------------------------------------------------------------------
// LOG macro related

// #define DEFINE_IS_LOG    CLI_IsLog
#define DEFINE_IS_LOG   DMX_IsLog
#include "x_debug.h"

LINT_EXT_HEADER_END


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

LINT_SAVE_AND_DISABLE
CLIMOD_DEBUG_FUNCTIONS(DMX)
LINT_RESTORE

LINT_SUPPRESS_SYMBOL(818, aszArgv)

#ifdef CC_DMX_ATSC_MPEG_ONLY
#undef CC_DMX_ATSC_MPEG_ONLY
#endif // CC_DMX_ATSC_MPEG_ONLY

// For 5368 emulation
#ifdef CC_MT5387
#undef CC_MT5387
#endif

#ifdef CC_MT5363
#undef CC_MT5363
#endif

#define CC_MT5365

#if defined (CC_MT5387) && !defined (CC_MT5363)
#define CC_DMX_ATSC_MPEG_ONLY
#endif

//#define DMX_CLI_SUPPORT_MULTI2

//#define CC_DMX_DEBUG
//#define CC_EMULATION
//#define __MODEL_slt__
extern void _DMX_SetPtsInfo(BOOL fgEnableVideoPts,BOOL fgEnableAudioPts);
extern void _DmxSetPcrInfo(BOOL fgEnable);
extern INT32 STC_SetPtsDrift(UCHAR ucStcId, INT32 i4Drift);
extern INT32 STC_GetPtsDrift(UCHAR ucStcId);
//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

#define DMX_DEFAULT_ES_FIFO_SIZE                0x100000
#define DMX_DEFAULT_PSI_FIFO_SIZE               0x10000
#ifndef CC_MT5391_AUD_3_DECODER
#define NUM_AUDIO_DEVICE                        2
#else
#define NUM_AUDIO_DEVICE                        3
#endif
#define REG_DMX_RESET                           0
#define REG_PWM                                 (BIM_BASE + 0x230)

#define CAP_THREAD_NAME                         "Capture"
#define CAP_THREAD_STACK_SIZE                   8192
#define CAP_THREAD_PRIORITY                     60


//#define CC_DMX_DEBUG

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

/** Brief of PWM_REGISTER_T
 */
typedef struct
{
    UINT32 u4Enable     :    1;
    UINT32 u4LoadDmx    :    1;
    UINT32              :    6;
    UINT32 u4Prescaler  :    8;
    UINT32 u4High       :    8;
    UINT32 u4Resolution :    8;
} PWM_REGISTER_T;

/// Section info
typedef struct
{
    UINT16 u2SectionLength;
    UINT16 u2TableIdExtension;
    UINT8 u1TableId;
    UINT8 u1VersionNumber;
    UINT8 u1SectionNumber;
    UINT8 u1LastSectionNumber;
    BOOL fgSectionSyntaxIndicator;
    BOOL fgPrivateIndicator;
    BOOL fgCurrentNextIndicator;
} SECTION_INFO_T;


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Imported functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------

/// TS index
static UINT8 _u1TsIndex = 0;

/// Is steering to PVR
static BOOL _fgToPvr = FALSE;

/// Is steering to uP
static BOOL _fgToFTuP = TRUE;

static UINT32 _u4CaptureBufAddr = 0;
static UINT32 _u4CaptureBufSize = 0;
static BOOL _fgCaptureEnable = FALSE;

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _ShowPidInfo(UINT8 u1Pidx, const DMX_PID_T* prPid)
{
    UINT32 u4DataSize;
    CHAR szBuf[64];
    CHAR* pszType;
    CHAR* pszMode;

    ASSERT(prPid != NULL);

    // Index, valid, PCR, callback, PID
    Printf("\nIndex: %u, Enable: %s, PCR: %s, Callback: %s, PID: %u(0x%x)\n",
        u1Pidx, (prPid->fgEnable ? "Y" : "N"),
        ((prPid->ePcrMode != DMX_PCR_MODE_NONE) ? "Y" : "-"),
        ((prPid->pfnNotify != NULL) ? "Y" : "-"),
        prPid->u2Pid, prPid->u2Pid);

    // PID type
    switch (prPid->ePidType)
    {
    case DMX_PID_TYPE_NONE:
        pszType = "NONE";
        break;

    case DMX_PID_TYPE_PSI:
        pszType = "PSI";
        break;

    case DMX_PID_TYPE_PES:
        pszType = "PES";
        break;

    case DMX_PID_TYPE_ES_VIDEO:
        pszType = "VIDEO ES";
        break;

    case DMX_PID_TYPE_ES_AUDIO:
        pszType = "AUDIO ES";
        break;

    default:
        pszType = "UNKNOWN";
        break;
    }

    // Descramble mode
    switch (prPid->eDescMode)
    {
    case DMX_DESC_MODE_NONE:
        pszMode = "NONE";
        break;

    case DMX_DESC_MODE_DES_ECB:
        pszMode = "DES ECB";
        break;

    case DMX_DESC_MODE_DES_CBC:
        pszMode = "DES CBC";
        break;

    case DMX_DESC_MODE_3DES_ECB:
        pszMode = "3DES ECB";
        break;

    case DMX_DESC_MODE_3DES_CBC:
        pszMode = "3DES CBC";
        break;

    case DMX_DESC_MODE_DVB:
        pszMode = "DVB";
        break;

    case DMX_DESC_MODE_DVB_CONF:
        pszMode = "DVB CONF";
        break;

    default:
        pszMode = "UNKNOWN";
        break;
    }

    // Steering
    szBuf[0] = '\0';
    if ((prPid->u1SteerMode & DMX_STEER_TO_FTUP) != 0)
    {
        x_strncat(szBuf, " uP", sizeof(" uP"));
    }
    if ((prPid->u1SteerMode & DMX_STEER_TO_PVR) != 0)
    {
        x_strncat(szBuf, "PVR", sizeof("PVR"));
    }

    Printf("  Descramble mode: %s, Key index: %u, Steering:%s, Type: %s\n",
        pszMode, prPid->u1KeyIndex, szBuf, pszType);

    // Buffer
    Printf("  Buffer: 0x%08x - 0x%08x (0x%08x)\n",
        prPid->u4BufAddr, prPid->u4BufAddr + prPid->u4BufSize,
        prPid->u4BufSize);

    // Data pointers
    u4DataSize = DATASIZE(prPid->u4Rp, prPid->u4Wp, prPid->u4BufSize);
    Printf("  RP: 0x%08x, WP: 0x%08x, Data size: %u\n",
        prPid->u4Rp, prPid->u4Wp, u4DataSize);
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _ShowFilterInfo(UINT8 u1Index, UINT8 u1Bank,
    const DMX_FILTER_T* prFilter)
{
    DMX_PID_T rPid;
    CHAR* pszMode;

    ASSERT(prFilter != NULL);

    VERIFY(DMX_CLIGetPid(prFilter->u1Pidx, DMX_PID_FLAG_PID, &rPid));

    // Index, valid, pidx, pid, offset
    Printf("\nIndex: %u, Bank: %u, Enable: %s, PID index: %u, PID: %u(0x%x)\n",
        u1Index, u1Bank, (prFilter->fgEnable ? "Y" : "N"), prFilter->u1Pidx,
        rPid.u2Pid, rPid.u2Pid);

    // Translate mode to string
    switch (prFilter->eMode)
    {
    case DMX_FILTER_MODE_SHORT:
        pszMode = "SHORT";
        break;

    case DMX_FILTER_MODE_14_2:
        pszMode = "14+2";
        break;

    case DMX_FILTER_MODE_LONG:
        pszMode = "LONG";
        break;

    case DMX_FILTER_MODE_POS_NEG:
        pszMode = "POS+NEG";
        break;

    default:
        pszMode = "UNKNWON";
        break;
    }

    // Show offset, CRC checking, mode
    Printf("  Offset: %u, CRC: %s, Mode: %s\n", prFilter->u1Offset,
        (prFilter->fgCheckCrc ? "Y" : "N"), pszMode);

    // Show pattern and mask
    Printf("  Pattern: 0x%08x-0x%08x, Mask: 0x%08x-0x%08x\n",
        prFilter->au4Data[0], prFilter->au4Data[1], prFilter->au4Mask[0],
        prFilter->au4Mask[1]);
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _ShowGenericFilterInfo(UINT8 u1Index, const DMX_FILTER_GENERIC_T *prFilter)
{
    DMX_PID_T rPid;
    UINT8 i;
/* FIXME: Put the next statement into dmx_if.h or dmx_mm_if.h. */
#define DMX_MAX_GENERIC_FILTER_LEN              16

    ASSERT(prFilter != NULL);

    VERIFY(DMX_CLIGetPid(prFilter->u1Pidx, DMX_PID_FLAG_PID, &rPid));

    // Filter Index, valid, CRC on/off, Offset, Pidx, PID value
    Printf("FltIdx: %u, Enable: %c, CRC: %c, Offset: %u, Pidx: %u, PID: 0x%x\n",
        u1Index, (prFilter->fgEnable ? 'Y' : 'N'),
        (prFilter->fgCheckCrc ? 'Y' : 'N'), prFilter->u1Offset,
        prFilter->u1Pidx, rPid.u2Pid);

    // Show pattern
    Printf("Pattern:  0x");
    for (i = 0; i < DMX_MAX_GENERIC_FILTER_LEN; i++)
    {
        Printf("%02X", prFilter->au1Data[i]);
        if ((i % 4) == 3)
        {
            if (i != (DMX_MAX_GENERIC_FILTER_LEN - 1))
            {
                Printf("-");
            }
            else
            {
                Printf("\n");
            }
        }
    }

    // Show mask
    Printf("Mask:     0x");
    for (i = 0; i < DMX_MAX_GENERIC_FILTER_LEN; i++)
    {
        Printf("%02X", prFilter->au1Mask[i]);
        if ((i % 4) == 3)
        {
            if (i != (DMX_MAX_GENERIC_FILTER_LEN - 1))
            {
                Printf("-");
            }
            else
            {
                Printf("\n");
            }
        }
    }

    // Show Positive/Negative mode
    Printf("P/N(0/1): 0x");
    for (i = 0; i < DMX_MAX_GENERIC_FILTER_LEN; i++)
    {
        Printf("%02X", prFilter->au1PosNeg[i]);
        if ((i % 4) == 3)
        {
            if (i != (DMX_MAX_GENERIC_FILTER_LEN - 1))
            {
                Printf("-");
            }
            else
            {
                Printf("\n\n");
            }
        }
    }
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _ShowPidCounters(UINT8 u1Pidx)
{
    DMX_PID_COUNTERS_T rCounters;

    // Get PID counters
    if (!DMX_GetPidCounters(u1Pidx, &rCounters))
    {
        Printf("Fail to get counters of PID index %u\n", u1Pidx);
        return;
    }

    // Show PID counters
    if (rCounters.u4PicCount != 0)
    {
        Printf("\nPID index: %u, PES count: %u, Pictures: %u, Sections: %u\n",
            u1Pidx, rCounters.u4PesCount, rCounters.u4PicCount,
            rCounters.u4SecCount);
    }
    else
    {
        Printf("\nPID index: %u, PES count: %u, Sections: %u\n", u1Pidx,
            rCounters.u4PesCount, rCounters.u4SecCount);
    }

    Printf("-------------------------------------------------------------\n");
    Printf("DBM: %u, DESC: %u, PCR: %u, Steer: %u\n", rCounters.u4DbmErrors,
        rCounters.u4DescErrors, rCounters.u4PcrErrors,
        rCounters.u4SteerErrors);
    Printf("FTuP: %u, Memory: %u, PVR: %u\n", rCounters.u4FTuPErrors,
        rCounters.u4MemoryErrors, rCounters.u4PvrErrors);
    Printf("-------------------------------------------------------------\n");
    Printf("Total errors: %u\n", rCounters.u4TotalErrors);
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _QueryPid(UINT8 u1Pidx)
{
    static CHAR szMsg[256], szBuf[128];
    DMX_PID_T rPid;
    DMX_PID_COUNTERS_T rCounters;
    CHAR* pszType;
    UINT32 u4DataSize;

    // Get PID info
    if (!DMX_CLIGetPid(u1Pidx, DMX_PID_FLAG_ALL, &rPid))
    {
        return;
    }

    if (!rPid.fgEnable)
    {
        return;
    }

    // PID type
    switch (rPid.ePidType)
    {
    case DMX_PID_TYPE_NONE:
        pszType = "NONE";
        break;

    case DMX_PID_TYPE_PSI:
        pszType = "PSI";
        break;

    case DMX_PID_TYPE_PES:
        pszType = "PES";
        break;

    case DMX_PID_TYPE_ES_VIDEO:
        pszType = "VIDEO";
        break;

    case DMX_PID_TYPE_ES_AUDIO:
        pszType = "AUDIO";
        break;

    default:
        pszType = "UNKNOWN";
        break;
    }

    x_snprintf(szMsg, 256, "Pidx: %u, TS_idx: %d, PID: 0x%x, %s", u1Pidx, rPid.u1TsIndex, rPid.u2Pid, pszType);

    // PID counters
    if (!DMX_GetPidCounters(u1Pidx, &rCounters))
    {
        return;
    }

    if (rCounters.u4PesCount > 0)
    {
        x_snprintf(szBuf, 128, ", PES: %u", rCounters.u4PesCount);
        x_strncat(szMsg, szBuf, 128);
    }
    if (rCounters.u4PicCount > 0)
    {
        x_snprintf(szBuf, 128, ", Pic: %u", rCounters.u4PicCount);
        x_strncat(szMsg, szBuf, 128);
    }
#if defined(CC_MT5387) || defined(CC_MT5363)    
    if (rCounters.u4OneByteSecCount > 0)
    {
        x_snprintf(szBuf, ", 128, OBSec: %u", rCounters.u4OneByteSecCount);
        x_strncat(szMsg, szBuf, 128);
    }
#endif
    if (rCounters.u4SecCount > 0)
    {
        x_snprintf(szBuf, 128, ", Sec: %u", rCounters.u4SecCount);
        x_strncat(szMsg, szBuf, 128);
    }

    u4DataSize = DATASIZE(rPid.u4Rp, rPid.u4Wp, rPid.u4BufSize);
    x_snprintf(szBuf, 128, ", Errors: %u, data: %u\n", rCounters.u4TotalErrors, u4DataSize);
    x_strncat(szMsg, szBuf, 128);

    // Show PID info
    Printf("%s", szMsg);
}


#if 0

//-----------------------------------------------------------------------------
/** _DumpBuffer
 *  Dump a buffer
 *
 *  @param  au1Buf          The buffer
 *  @param  u4Size          Size of the buffer
 *  @param  u4ByesPerLine   Bytes per print line
 *
 *  @retval void
 */
//-----------------------------------------------------------------------------
static void _DumpBuffer(const UINT8 au1Buf[], UINT32 u4Size,
    UINT32 u4BytesPerLine)
{
    UINT32 i, j;

    ASSERT(au1Buf != NULL);

    j = 0;
    for (i = 0; i < u4Size; i++)
    {
        if (j == 0)
        {
            Printf("\n%04x:", i);
        }

        Printf(" %02x", au1Buf[i]);

        if (j == 7)
        {
            Printf(" |");
        }

        j++;
        if (j == u4BytesPerLine)
        {
            j = 0;
        }
    }
    Printf("\n");
}

#endif  // 0

//=====================================================================
// Command functions
//=====================================================================


// These functions exist only in SLT mode
#ifdef __MODEL_slt__
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_Diag(INT32 i4Argc, const CHAR ** aszArgv)
{
    INT32 i4Ret;

    Printf("Self diagnostic testing...\n");
    i4Ret = DMX_Diag();
    Printf("%s\n", (i4Ret == 0) ? "OK!" : "Failed");

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}

#ifdef CC_DMX_ATSC_MPEG_ONLY
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_JChipDiag(INT32 i4Argc, const CHAR **aszArgv)
{
    INT32 i4Ret;

    Printf("J-Chip functionality test...\n");
    if (BIM_READ32(0x660) & 0x200000)
    {
        Printf("JCHIP_OK!\n");
        return 0;
    }
    i4Ret = DMX_JChipDiag(_u1TsIndex);
    Printf("    Return code: %d\n", i4Ret);
    Printf("%s\n", (i4Ret == 0) ? "JCHIP_OK!" : "JCHIP_FAIL!");

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}
#endif  // CC_DMX_ATSC_MPEG_ONLY
#endif  // __MODEL_slt__

//-----------------------------------------------------------------------------
/** _DMXCLI_PtsDriftInfo
 *  Command: EnablePtsInfo
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_PtsDriftInfo(INT32 i4Argc, const CHAR** aszArgv)
{

    INT32 i4PtsDrift = 0;
    UINT32 u4PtsDrift = 0;
    UINT32 u4InputCmd = 0;
    if((i4Argc != 3) && (i4Argc != 2))
    {
        Printf("usage1 : dmx.ptsdrift 0 ---> query current pts drift\n");
        Printf("usage2 : dmx.ptsdrift 1 ptsdrift(90k base) ---> set pts drift value\n");
        Printf("usage2 example : dmx.ptsdrift 1 3000 ---> add pts drift to 3000/90000 = 33ms\n");
        return 0;
    }

    u4InputCmd = StrToInt(aszArgv[1]);

    if(i4Argc == 3)
    {
        if(u4InputCmd == 1)
        {
            u4PtsDrift = StrToInt(aszArgv[2]);
            i4PtsDrift = u4PtsDrift;
            Printf("set pts drift to %d\n",i4PtsDrift);
            STC_SetPtsDrift(0,i4PtsDrift);
        }
    }
    else if(i4Argc == 2)
    {
        i4PtsDrift = STC_GetPtsDrift(0);
        Printf("Current pts drift = %d\n",i4PtsDrift);
    }


    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_EnablePtsInfo
 *  Command: EnablePtsInfo
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_EnablePtsInfo(INT32 i4Argc, const CHAR** aszArgv)
{

    BOOL fgVideoPtsInfo = FALSE;
    BOOL fgAudioPtsInfo = FALSE;
    BOOL fgPcrInfo = FALSE;
    
    if(i4Argc != 4)
    {
        Printf("usage : dmx.enpts fgVideoPtsInfo fgAudioPtsInfo fgPcrInfo\n");
        Printf("usage exmaple 1 : dmx.enpts 1 0 0 ---> only enable video pts info, you should set dmx.d_l 5 to log pts info\n");
        Printf("usage exmaple 2 : dmx.enpts 0 1 0 ---> only enable audio pts info, you should set dmx.d_l 5 to log pts info\n");
        Printf("usage exmaple 3 : dmx.enpts 0 0 1 ---> only enable pcr value info, you should set dmx.d_l 5 to log pts info\n");
        return 0;
    }

    fgVideoPtsInfo = (BOOL)StrToInt(aszArgv[1]);
    fgAudioPtsInfo = (BOOL)StrToInt(aszArgv[2]);
    fgPcrInfo = (BOOL)StrToInt(aszArgv[3]);
    
    _DMX_SetPtsInfo(fgVideoPtsInfo,fgAudioPtsInfo);
    _DmxSetPcrInfo(fgPcrInfo);

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}



//-----------------------------------------------------------------------------
/** _DMXCLI_Init
 *  Command: init
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_Init(INT32 i4Argc, const CHAR** aszArgv)
{
    if (!DMX_Init())
    {
        Printf("DMX_Init fail\n");
        return -1;
    }

    Printf("DMX_Init OK\n");

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_SetFrontEnd
 *  Command: init
 */
//-----------------------------------------------------------------------------
#if defined(CC_MT5387) || defined(CC_MT5363)
static INT32 _DMXCLI_SetFrontEnd(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_FRONTEND_T eFEType;

    if(i4Argc != 2)
    {
        Printf("%s [FE type]\n", aszArgv[0]);
        Printf("   0: ATSC, 1: DVB, 2: Ext parallel, 3: Ext serial\n");
#ifndef CC_DMX_ATSC_MPEG_ONLY
        Printf("   4: Ext s0, 5: Ext s1\n");
        Printf("   6. DVB-C + Internal CI\n");
        Printf("   7: pal_ser, 8: pal_pal, 9. DVBT+Internal CI\n");
#endif // CC_DMX_ATSC_MPEG_ONLY
        Printf("Current FE type: %u\n", (UINT32)DMX_GetFrontEnd());
        return 0;
    }

    eFEType = (DMX_FRONTEND_T)StrToInt(aszArgv[1]);

    if(!DMX_SetFrontEnd(eFEType))
    {
        Printf("Set front end fail\n");
        return -1;
    }

    _u1TsIndex = DMX_GetFramerIndex();

    Printf("OK\n");

    return 0;
}
#else
static INT32 _DMXCLI_SetFrontEnd(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1TsIndex = 0;
    DMX_FRONTEND_T eFEType;

    if(i4Argc < 2)
    {
        Printf("%s fe_type ts_idx\n", aszArgv[0]);
        Printf("      fe_type: 0, 1, 2, ...\n");
        Printf("Current FE type: tsidx 0, %u\n", (UINT32)DMX_GetFrontEndEx(0));
        Printf("Current FE type: tsidx 1, %u\n", (UINT32)DMX_GetFrontEndEx(1));
        return 0;
    }

    eFEType = (DMX_FRONTEND_T)StrToInt(aszArgv[1]);
    u1TsIndex = StrToInt(aszArgv[2]);

    if(!DMX_SetFrontEndEx(u1TsIndex, eFEType))
    {
        Printf("Set front end fail\n");
        return -1;
    }

//    _u1TsIndex = DMX_GetFramerIndex();

    Printf("OK\n");

    return 0;
}
#endif

//-----------------------------------------------------------------------------
/** _DMXCLI_ResetFramer
 *  Command: reset framer
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_ResetFramer(INT32 i4Argc, const CHAR** aszArgv)
{
    UNUSED(i4Argc);
    UNUSED(aszArgv);

    DMX_ResetFramer();

    return 0;
}


//-----------------------------------------------------------------------------
static void _DmxCLI_FramerUsage(const CHAR* szArgv)
{
    Printf("%s [-p 0|1] [-e 0|1] [-s 0|1] [-f 0|1]\n", szArgv);
    Printf("     -p:  0=serial, 1=parallel\n");
    Printf("     -e:  0=neg_edge, 1=pos_edge\n");
    Printf("     -s:  0=inernal_sync, 1=ext_sync\n");
    Printf("     -f:  0=framer0, 1=framer1\n");
}

//-----------------------------------------------------------------------------
/** _DMXCLI_SetFramer
 *  Command: set framer mode
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetFramer(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i;
    UINT32 u4FramerIdx, u4Sync, u4Edge, u4Parallel, u4Val;

    u4FramerIdx = DMX_GetFramerIndex();
    u4Val = (IO_READ32(DEMUX0_BASE, 0x20) & 0xFF) >> (u4FramerIdx * 4);
    u4Parallel = (u4Val >> 1) & 0x1;
    u4Sync = (u4Val >> 2) & 0x1;
    u4Edge = (u4Val >> 3) & 0x1;

    if(i4Argc < 2)
    {
        _DmxCLI_FramerUsage(aszArgv[0]);
        Printf("Current framer: %u, %s, %s, %s", u4FramerIdx,
                        u4Parallel ? "parallel" : "serial",
                        u4Sync ? "ext_sync" : "inc_sync",
                        u4Edge ? "pos_edge" : "neg_edge");
        return 0;
    }

    for (i=1; i<(i4Argc-1); i++)
    {
        if (aszArgv[i][0] != '-')
        {
            continue;
        }
        switch(aszArgv[i][1])
        {
        case 'P': case 'p':
            u4Parallel = (StrToInt(aszArgv[i+1])) & 0x1;
            break;
        case 'E': case 'e':
            u4Edge = (StrToInt(aszArgv[i+1])) & 0x1;
            break;
        case 'S': case 's':
            u4Sync = (StrToInt(aszArgv[i+1])) & 0x1;
            break;
        case 'F': case 'f':
            u4FramerIdx = (StrToInt(aszArgv[i+1])) & 0x1;
            break;
        default:
            break;
        }
    }

    if(u4FramerIdx == 0)
    {
        u4Val = IO_READ32(DEMUX0_BASE, 0x20) & 0xFFFFFFF0;
    }
    else
    {
        u4Val = IO_READ32(DEMUX0_BASE, 0x20) & 0xFFFFFF0F;
    }
    u4Val |= (1 | (u4Parallel << 1) | (u4Sync << 2) | (u4Edge << 3)) << (4 * u4FramerIdx);

    IO_WRITE32(DEMUX0_BASE, 0x20, u4Val);

    DMX_ResetFramer();

    Printf("OK\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetTuner(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1TsIndex;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 2)
    {
        Printf("Usage: %s tunerindex[%u-%u]\n", aszArgv[0],
            DMX_MIN_TUNER_INDEX, DMX_MAX_TUNER_INDEX);
        Printf("0:framer0, 1:framer1, 2:no use 3:ddi2dbm, 4:framer3\n");
        return 0;
    }

    u1TsIndex = (UINT8)StrToInt(aszArgv[1]);
    if (u1TsIndex > DMX_MAX_TUNER_INDEX)
    {
        return -1;
    }

    _u1TsIndex = u1TsIndex;

    if(u1TsIndex == 4)
    {
        DMX_SetExternalFramer(DMX_EXT_FRAMER_2_PVR_3_EXT);
        _u1TsIndex = 3;
    }
    else
    {
        DMX_SetExternalFramer(DMX_EXT_FRAMER_NORMAL);
    }    

#ifdef CC_DMX_EMULATION
    _DMX_EMU_SetTsIndex(_u1TsIndex);
#endif  // CC_EMULATION

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _ShowSetPidUsage(const CHAR* szCmd)
{
    Printf("Usage: %s pid_index(0-%u) pid [pid_type(pes,video,audio,none]"
        " [device_id(0..x)] [file(0|1)] [ca_mode(0-%u)] [key_index(0-%u)]\n",
        szCmd, DMX_NUM_PID_INDEX - 1, DMX_MAX_CA_MODE, DMX_DMEM_CA_KEY_NUM - 1);
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetPid(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PID_T rPid;
    UINT8 u1Pidx, u1File = 0;
    BOOL fgRet;

    ASSERT(aszArgv != NULL);

    if (i4Argc < 3)
    {
        _ShowSetPidUsage(aszArgv[0]);
        return -1;
    }

    rPid.u1TsIndex = _u1TsIndex;
    rPid.u1DeviceId = 0;
    rPid.u1KeyIndex = 0;
    rPid.fgEnable = TRUE;
    rPid.u1SteerMode = (_fgToPvr ? DMX_STEER_TO_PVR : 0) |
        (_fgToFTuP ? DMX_STEER_TO_FTUP : 0);
    rPid.fgAllocateBuffer = TRUE;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;
    rPid.ePidType = DMX_PID_TYPE_ES_OTHER;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = DMX_DEFAULT_ES_FIFO_SIZE;
    rPid.pfnNotify = NULL;
    rPid.pvNotifyTag = NULL;
    rPid.fgPrimary = FALSE;
#if !defined(CC_MT5387) && !defined(CC_MT5363)
    rPid.u1ChannelId = 0x0;
#endif

    u1Pidx = (UINT8)StrToInt(aszArgv[1]);
    rPid.u2Pid = (UINT16)StrToInt(aszArgv[2]);

    if (i4Argc > 3)
    {
        if (x_strcmp(aszArgv[3], "pes") == 0)
        {
            rPid.ePidType = DMX_PID_TYPE_PES;
        }
        else if (x_strcmp(aszArgv[3], "video") == 0)
        {
            rPid.ePidType = DMX_PID_TYPE_ES_VIDEO;
        }
        else if (x_strcmp(aszArgv[3], "audio") == 0)
        {
            rPid.ePidType = DMX_PID_TYPE_ES_AUDIO;
        }
        else if (x_strcmp(aszArgv[3], "none") == 0)
        {
            rPid.ePidType = DMX_PID_TYPE_NONE;
        }
        else
        {
            _ShowSetPidUsage(aszArgv[0]);
            return -1;
        }
    }

    if (i4Argc > 4)
    {
        rPid.u1DeviceId = (UINT8)StrToInt(aszArgv[4]);
    }

    if (i4Argc > 5)
    {
        u1File = (UINT8)StrToInt(aszArgv[5]);
    }

    if (i4Argc > 6)
    {
        rPid.eDescMode = (DMX_DESC_MODE_T)(UINT8)StrToInt(aszArgv[6]);
    }

    if (i4Argc > 7)
    {
        rPid.u1KeyIndex = (UINT8)StrToInt(aszArgv[7]);
    }

    if (i4Argc > 8)
    {
        rPid.fgPrimary = (UINT8)StrToInt(aszArgv[8]);
    }

    if (i4Argc > 9)
    {
        _ShowSetPidUsage(aszArgv[9]);
        return -1;
    }

    // Process audio FIFO specially
    if (rPid.ePidType == DMX_PID_TYPE_ES_AUDIO)
    {
        UINT32 au4BufStart[2], au4BufEnd[2];
#ifndef CC_MT5391_AUD_3_DECODER
        ASSERT(NUM_AUDIO_DEVICE == 2);
#else
        ASSERT(NUM_AUDIO_DEVICE >= 3);
#endif  // CC_MT5391_AUD_3_DECODER
        if (rPid.u1DeviceId >= NUM_AUDIO_DEVICE)
        {
            Printf("Audio device ID: 0 - %u\n", NUM_AUDIO_DEVICE - 1);
            return -1;
        }

#ifdef AUD_OLDFIFO_INTF
        if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
            &au4BufEnd[1]) != AUD_OK)
#else
        if ((AUD_GetAudFifo(0, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK) ||
            (AUD_GetAudFifo(1, &au4BufStart[1], &au4BufEnd[1]) != AUD_OK))
#endif
        {
            Printf("Can't get audio buffers!\n");
            return -1;
        }
        if ((au4BufStart[0] == 0) || (au4BufStart[1] == 0))
        {
            Printf("Can't get audio buffers!\n");
            return -1;
        }
#ifndef CC_MT5391_AUD_3_DECODER
        rPid.u4BufAddr = au4BufStart[rPid.u1DeviceId];
        rPid.u4BufSize = au4BufEnd[rPid.u1DeviceId] -
            au4BufStart[rPid.u1DeviceId];
#else
        //gallen, decoder 1/3 both use AFIFO1, decoder 2 uses AFIFO2
        rPid.u4BufAddr = au4BufStart[(((rPid.u1DeviceId)==1)?1:0)];
        rPid.u4BufSize = au4BufEnd[(((rPid.u1DeviceId)==1)?1:0)] -
            au4BufStart[(((rPid.u1DeviceId)==1)?1:0)];
#endif  // CC_MT5391_AUD_3_DECODER
        rPid.fgAllocateBuffer = FALSE;
    }
    else if (rPid.ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
#if 1
        FBM_POOL_T* prFbmPool;

        // Allocate video buffer from FBM
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0);

        // Setup buffer attributes
        rPid.fgAllocateBuffer = FALSE;
        rPid.u4BufAddr = prFbmPool->u4Addr;
        rPid.u4BufSize = prFbmPool->u4Size;
#else
        rPid.fgAllocateBuffer = TRUE;
        rPid.u4BufSize = 0x0500000;
#endif  // 1
    }

#if !defined(CC_MT5387) && !defined(CC_MT5363)
    fgRet = DMX_SetPidEx(u1Pidx, DMX_PID_FLAG_ALL, &rPid, 
                (u1File == 0) ? DMX_SOURCE_DTV0 : DMX_SOURCE_TSFILE0);
#else
    UNUSED(u1File);
    fgRet = DMX_SetPid(u1Pidx, DMX_PID_FLAG_ALL, &rPid);
#endif

    return fgRet ? 0 : -1;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_FreePid(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1Pidx;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 2)
    {
        Printf("Usage: %s pid_index(0-%u)\n", aszArgv[0],
            DMX_NUM_PID_INDEX - 1);
        return -1;
    }

    u1Pidx = (UINT8)StrToInt(aszArgv[1]);
    if (!DMX_FreePid(u1Pidx))
    {
        return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_GetPid(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PID_T rPid;
    UINT32 i;
    UINT8 u1Pidx;

    ASSERT(aszArgv != NULL);

    if (i4Argc > 2)
    {
        Printf("Usage: %s [all|pid_index(0-%u)]\n", aszArgv[0],
            DMX_NUM_PID_INDEX - 1);
        return -1;
    }

    if ((i4Argc == 1) || (x_strcmp(aszArgv[1], "all") == 0))
    {
        // Show infos of all active PIDs
        for (i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
            if (!DMX_CLIGetPid(i, DMX_PID_FLAG_ALL, &rPid))
            {
                return -1;
            }

            if (rPid.fgEnable)
            {
                _ShowPidInfo(i, &rPid);
            }
        }
    }
    else
    {
        // Show single PID info
        u1Pidx = (UINT8)StrToInt(aszArgv[1]);
        if (!DMX_CLIGetPid(u1Pidx, DMX_PID_FLAG_ALL, &rPid))
        {
            return -1;
        }
        _ShowPidInfo(u1Pidx, &rPid);
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_EnablePid(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PID_T rPid;
    UINT8 u1Pidx;

    ASSERT(aszArgv != NULL);

    if ((i4Argc != 2) && (i4Argc != 3))
    {
        Printf("Usage: %s pid_index [on|off]\n", aszArgv[0]);
        return -1;
    }

    u1Pidx = (UINT8)StrToInt(aszArgv[1]);
    if (i4Argc == 2)
    {
        if (!DMX_CLIGetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
        {
            return -1;
        }

        Printf("Pid_index %u %s\n", u1Pidx,
            rPid.fgEnable ? "enabled" : "disabled");

        return 0;
    }

    if (x_strcmp(aszArgv[2], "on") == 0)
    {
        rPid.fgEnable = TRUE;
    }
    else if (x_strcmp(aszArgv[2], "off") == 0)
    {
        rPid.fgEnable = FALSE;
    }
    else
    {
        return -1;
    }

    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rPid))
    {
        return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _ShowPcrUsage(const CHAR* szCmd)
{
    Printf("Usage: %s pid_index(0-%u) [mode(2|0)] [on|off]\n", szCmd,
        DMX_NUM_PID_INDEX - 1);
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetPcr(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1PidIndex;
    BOOL fgEnable = TRUE, fgNewMode = TRUE;

    ASSERT(aszArgv != NULL);

    if (i4Argc < 2)
    {
        _ShowPcrUsage(aszArgv[0]);
        return -1;
    }

    u1PidIndex = (UINT8)StrToInt(aszArgv[1]);
    if (u1PidIndex >= DMX_NUM_PID_INDEX)
    {
        _ShowPcrUsage(aszArgv[0]);
        return -1;
    }

    if (i4Argc > 2)
    {
        UINT8 u1Mode;

        u1Mode = (UINT8)StrToInt(aszArgv[2]);
        if ((u1Mode != 0) && (u1Mode != 2))
        {
            _ShowPcrUsage(aszArgv[0]);
            return -1;
        }

        fgNewMode = (u1Mode == 2);
    }

    if (i4Argc > 3)
    {
        if (x_strcmp(aszArgv[3], "on") == 0)
        {
            fgEnable = TRUE;
        } else if (x_strcmp(aszArgv[3], "off") == 0)
        {
            fgEnable = FALSE;
        }
        else
        {
            _ShowPcrUsage(aszArgv[0]);
            return -1;
        }
    }

    if (!DMX_SetPcr(u1PidIndex, fgEnable, fgNewMode))
    {
        return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetPsiPid(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PID_T rPid;
    UINT16 u2Pid;
    UINT8 u1PidIndex;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 3)
    {
        Printf("Usage: %s pid_index pid\n", aszArgv[0]);
        return -1;
    }

    u1PidIndex = (UINT8)StrToInt(aszArgv[1]);
    u2Pid = (UINT16)StrToInt(aszArgv[2]);

    rPid.u1TsIndex = _u1TsIndex;
    rPid.u1DeviceId = 0;
    rPid.u1KeyIndex = 0;
    rPid.fgEnable = TRUE;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.fgAllocateBuffer = TRUE;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;
    rPid.ePidType = DMX_PID_TYPE_PSI;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = DMX_DEFAULT_PSI_FIFO_SIZE;
    rPid.u2Pid = u2Pid;
    rPid.pfnNotify = NULL;
    rPid.pvNotifyTag = NULL;

    if (!DMX_SetPid(u1PidIndex, DMX_PID_FLAG_ALL, &rPid))
    {
        return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _PrintSetFilterUsage(const CHAR* szCmd)
{
    Printf("%s index bank pid_index offset mode crc byte0 ... byte7"
        " [mask_word0 mask_word1]\n", szCmd);
    Printf("    index: 0 - %u\n", DMX_NUM_FILTER_INDEX - 1);
    Printf("    bank: 0 - %u\n", DMX_NUM_FILTER_BANK - 1);
    Printf("    pid_index: 0 - %u\n", DMX_NUM_PID_INDEX - 1);
    Printf("    offset: %u - %u\n", DMX_MIN_FILTER_OFFSET,
        DMX_MAX_FILTER_OFFSET);
    Printf("    mode: short, long, pos_neg, 14_2\n");
    Printf("    crc: on, off\n");
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetFilter(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_FILTER_T rFilter;
    UINT32 i;
    UINT8 u1FilterIndex, u1Bank, au1Data[8];

    ASSERT(aszArgv != NULL);

    if (i4Argc < 15)
    {
        _PrintSetFilterUsage(aszArgv[0]);
        return -1;
    }

    x_memset(&rFilter, 0, sizeof(rFilter));
    rFilter.fgEnable = TRUE;

    u1FilterIndex = (UINT8)StrToInt(aszArgv[1]);
    u1Bank = (UINT8)StrToInt(aszArgv[2]);
    rFilter.u1Pidx = (UINT8)StrToInt(aszArgv[3]);
    rFilter.u1Offset = (UINT8)StrToInt(aszArgv[4]);
    if ((u1FilterIndex >= DMX_NUM_FILTER_INDEX) ||
        (u1Bank >= DMX_NUM_FILTER_BANK) ||
        (rFilter.u1Pidx >= DMX_NUM_PID_INDEX) ||
//      (rFilter.u1Offset < DMX_MIN_FILTER_OFFSET) ||
        (rFilter.u1Offset > DMX_MAX_FILTER_OFFSET))
    {
        _PrintSetFilterUsage(aszArgv[0]);
        return -1;
    }

    if (x_strcmp(aszArgv[5], "short") == 0)
    {
        rFilter.eMode = DMX_FILTER_MODE_SHORT;
    }
    else if (x_strcmp(aszArgv[5], "long") == 0)
    {
        rFilter.eMode = DMX_FILTER_MODE_LONG;
    }
    else if (x_strcmp(aszArgv[5], "pos_neg") == 0)
    {
        rFilter.eMode = DMX_FILTER_MODE_POS_NEG;
    }
    else if (x_strcmp(aszArgv[5], "14_2") == 0)
    {
        rFilter.eMode = DMX_FILTER_MODE_14_2;
    }
    else
    {
        _PrintSetFilterUsage(aszArgv[0]);
        return -1;
    }

    if (x_strcmp(aszArgv[6], "on") == 0)
    {
        rFilter.fgCheckCrc = TRUE;
    }
    else if (x_strcmp(aszArgv[6], "off") == 0)
    {
        rFilter.fgCheckCrc = FALSE;
    }
    else
    {
        _PrintSetFilterUsage(aszArgv[0]);
        return -1;
    }

    for (i = 7; i < 15; i++)
    {
        au1Data[i - 7] = (UINT8)StrToInt(aszArgv[i]);
    }

    rFilter.au4Data[0] = (au1Data[0] << 24 ) | (au1Data[1] << 16) |
        (au1Data[2] << 8) | au1Data[3];
    rFilter.au4Data[1] = (au1Data[4] << 24 ) | (au1Data[5] << 16) |
        (au1Data[6] << 8) | au1Data[7];

    rFilter.au4Mask[0] = 0xffffffff;
    rFilter.au4Mask[1] = 0xffffffff;
    if (i4Argc > 15)
    {
        rFilter.au4Mask[0] = (UINT32)StrToInt(aszArgv[15]);
    }
    if (i4Argc > 16)
    {
        rFilter.au4Mask[1] = (UINT32)StrToInt(aszArgv[16]);
    }

    if (!DMX_SetFilter(u1FilterIndex, u1Bank, DMX_FILTER_FLAG_ALL, &rFilter))
    {
        return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_GetFilter(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_FILTER_T rFilter;
    UINT32 i, j;
    UINT8 u1Pidx, u1Bank;

    ASSERT(aszArgv != NULL);

    if (i4Argc > 3)
    {
        Printf("Usage: %s [all|(pid_index [bank])]\n", aszArgv[0]);
        return -1;
    }

    if ((i4Argc == 1) || (x_strcmp(aszArgv[1], "all") == 0))
    {
        for (i = 0; i < DMX_NUM_FILTER_INDEX; i++)
        {
            for (j = 0; j < DMX_NUM_FILTER_BANK; j++)
            {
                if (!DMX_GetFilter(i, j, DMX_FILTER_FLAG_ALL, &rFilter))
                {
                    return -1;
                }

                if (rFilter.fgEnable)
                {
                    _ShowFilterInfo(i, j, &rFilter);
                }
            }
        }
    }
    else
    {
        u1Pidx = (UINT8)StrToInt(aszArgv[1]);
        u1Bank = 0;
        if (i4Argc > 2)
        {
            u1Bank = (UINT8)StrToInt(aszArgv[2]);
        }

        if (!DMX_GetFilter(u1Pidx, u1Bank, DMX_FILTER_FLAG_ALL, &rFilter))
        {
            return -1;
        }

        _ShowFilterInfo(u1Pidx, u1Bank, &rFilter);
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_GetGenericFilter(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_FILTER_GENERIC_T rFilter;
    UINT32 i;
    UINT8 u1FilterIndex;

    ASSERT(aszArgv != NULL);

    if (i4Argc > 2)
    {
        Printf("Usage: %s [all|pid_index]\n", aszArgv[0]);
        return -1;
    }

    if ((i4Argc == 1) || (x_strcmp(aszArgv[1], "all") == 0))
    {
        for (i = 0; i < DMX_NUM_FILTER_INDEX; i++)
        {
            if (!DMX_GetGenFilter(i, DMX_FILTER_FLAG_ALL, &rFilter))
            {
                return -1;
            }
            if (rFilter.fgEnable)
            {
                _ShowGenericFilterInfo(i, &rFilter);
            }
        }
    }
    else
    {
        u1FilterIndex = (UINT8)StrToInt(aszArgv[1]);
        if (!DMX_GetGenFilter(u1FilterIndex, DMX_FILTER_FLAG_ALL, &rFilter))
        {
            return -1;
        }
        _ShowGenericFilterInfo(u1FilterIndex, &rFilter);
    }

    return 0;
}


//-----------------------------------------------------------------------------
static BOOL _MPEGVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rMpvPes;

    ASSERT(prPes != NULL);

    x_memset((void*)&rMpvPes, 0, sizeof(rMpvPes));

    switch (prPes->u4FrameType)
    {
    case PIC_TYPE_I:
        rMpvPes.ucPicType = MPV_I_TYPE;
        break;

    case PIC_TYPE_P:
        rMpvPes.ucPicType = MPV_P_TYPE;
        break;

    case PIC_TYPE_B:
        rMpvPes.ucPicType = MPV_B_TYPE;
        break;

    default:
        return TRUE;
    }

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;
    rMpvPes.u4VldReadPtr = prPes->u4FrameAddr;
    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    rMpvPes.fgGop = prPes->fgGop;
    rMpvPes.fgEos = prPes->fgEOS;

    return (VDEC_SendEs((void *)&rMpvPes) == 1);
}

//-----------------------------------------------------------------------------
#ifndef CC_DMX_ATSC_MPEG_ONLY
static BOOL _H264VideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T  rMpvPes;
    UINT32 u4Addr;

    ASSERT(prPes != NULL);

    x_memset((void*)&rMpvPes, 0, sizeof(rMpvPes));

    u4Addr = prPes->u4FrameAddr + 3;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }
    rMpvPes.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;

    u4Addr = prPes->u4FrameAddr + 4;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }
    rMpvPes.u4VldReadPtr = u4Addr;

    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts;
    //rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    //rMpvPes.fgGop = prPes->fgGop;

    rMpvPes.fgEos = prPes->fgEOS;
    rMpvPes.u1SliceCount = prPes->u1SliceCount;

    return (VDEC_SendEs((void *)&rMpvPes) == 1);
}


//-----------------------------------------------------------------------------
static BOOL _VC1VideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rVdecPes;

    ASSERT(prPes != NULL);

    x_memset((void*)&rVdecPes, 0, sizeof(rVdecPes));

    rVdecPes.fgSeqHdr = prPes->fgSeqHeader;
    rVdecPes.fgGop = prPes->fgGop;
    rVdecPes.fgDtsValid = prPes->fgPtsDts;
    rVdecPes.ucMpvId = prPes->u1DeviceId;
    rVdecPes.ucEsId = prPes->u1Channel;
    rVdecPes.ucPicType = prPes->u4FrameType;
    rVdecPes.u4FifoStart = prPes->u4BufStart;
    rVdecPes.u4FifoEnd = prPes->u4BufEnd;
    rVdecPes.u4VldReadPtr = prPes->u4FrameAddr;
    rVdecPes.u4PTS = prPes->u4Pts;
    rVdecPes.u4DTS = prPes->u4Dts;
    rVdecPes.fgEos = prPes->fgEOS;
    // What about the eVideoType?     // FIXME

    return (VDEC_SendEs(&rVdecPes) == 1);

}

#if !defined(CC_MT5387) && !defined(CC_MT5363)
static BOOL _AVSVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T  rMpvPes;
    UINT32 u4Addr;

    ASSERT(prPes != NULL);

    x_memset((void*)&rMpvPes, 0, sizeof(rMpvPes));

    u4Addr = prPes->u4FrameAddr + 3;
    if(u4Addr >= prPes->u4BufEnd)
    {
        u4Addr -= (prPes->u4BufEnd - prPes->u4BufStart);
    }
    rMpvPes.ucPicType = *(UINT8*)VIRTUAL(u4Addr);

    rMpvPes.ucMpvId = prPes->u1DeviceId;
    rMpvPes.ucEsId = prPes->u1Channel;
    rMpvPes.u4FifoStart = prPes->u4BufStart;
    rMpvPes.u4FifoEnd = prPes->u4BufEnd;

    rMpvPes.u4VldReadPtr = prPes->u4FrameAddr;

    rMpvPes.u4PTS = prPes->u4Pts;
    rMpvPes.u4DTS = prPes->u4Dts;
    rMpvPes.fgDtsValid = prPes->fgPtsDts; 
    //rMpvPes.fgSeqHdr = prPes->fgSeqHeader;
    //rMpvPes.fgGop = prPes->fgGop;

    rMpvPes.fgEos = prPes->fgEOS;

    return (VDEC_SendEs((void *)&rMpvPes) == 1);
}
#endif

//-----------------------------------------------------------------------------
static BOOL _WMVVideoCallback(const DMX_PES_MSG_T* prPes)
{
    VDEC_PES_INFO_T rVdecPes;

    ASSERT(prPes != NULL);

    x_memset((void*)&rVdecPes, 0, sizeof(rVdecPes));

    rVdecPes.fgSeqHdr = prPes->fgSeqHeader;
    rVdecPes.fgGop = prPes->fgGop;
    rVdecPes.fgDtsValid = prPes->fgPtsDts;
    rVdecPes.ucMpvId = prPes->u1DeviceId;
    rVdecPes.ucEsId = prPes->u1Channel;
    rVdecPes.ucPicType = prPes->u4FrameType;
    rVdecPes.u4FifoStart = prPes->u4BufStart;
    rVdecPes.u4FifoEnd = prPes->u4BufEnd;
    rVdecPes.u4VldReadPtr = prPes->u4FrameAddr;
    rVdecPes.u4PTS = prPes->u4Pts;
    rVdecPes.u4DTS = prPes->u4Dts;
    rVdecPes.fgEos = prPes->fgEOS;
    // What about the eVideoType?     // FIXME

    return (VDEC_SendEs(&rVdecPes) == 1);

}
#endif // CC_DMX_ATSC_MPEG_ONLY

//-----------------------------------------------------------------------------
static BOOL _VideoCallback(const DMX_PES_MSG_T* prPes)
{
    switch(prPes->eVideoType)
    {
    case DMX_VIDEO_MPEG:
        return _MPEGVideoCallback(prPes);

    case DMX_VIDEO_H264:
        return _H264VideoCallback(prPes);

    case DMX_VIDEO_VC1:
        return _VC1VideoCallback(prPes);

    case DMX_VIDEO_WMV7:
    case DMX_VIDEO_WMV8:
    case DMX_VIDEO_WMV9:
        return _WMVVideoCallback(prPes);

    case DMX_VIDEO_AVS:
        return _AVSVideoCallback(prPes);

    default:
        Printf("Unknown video type\n");
        break;
    }

    return TRUE;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static void _ShowToDecoderUsage(const CHAR* szCmd)
{
    Printf("Usage: %s [ToDecoder(on|off)] [VirtualDecoder(on|off)]]\n", szCmd);
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetToDecoder(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgToDecoder;
    static BOOL _fgVirtualDecoder = TRUE;

    ASSERT(aszArgv != NULL);

    fgToDecoder = DMX_IsToDecoder();

    if (i4Argc != 3)
    {
        Printf("ToDecoder: %s, VirtualDecoder: %s, ",
            fgToDecoder ? "on" : "off", _fgVirtualDecoder ? "on" : "off");
        return 0;
    }

    // To decoder
    if (x_strcmp(aszArgv[1], "on") == 0)
    {
        fgToDecoder = TRUE;
    }
    else if (x_strcmp(aszArgv[1], "off") == 0)
    {
        fgToDecoder = FALSE;
    }
    else
    {
        _ShowToDecoderUsage(aszArgv[0]);
        return -1;
    }

    // To virtual decoder
    if (x_strcmp(aszArgv[2], "on") == 0)
    {
        _fgVirtualDecoder = TRUE;
    }
    else if (x_strcmp(aszArgv[2], "off") == 0)
    {
        _fgVirtualDecoder = FALSE;
    }
    else
    {
        _ShowToDecoderUsage(aszArgv[0]);
        return -1;
    }

    DMX_SetToDecoder(fgToDecoder);

    if (!_fgVirtualDecoder)
    {
        DMX_DECODER_CALLBACKS_T rDecoderCallbacks;

        rDecoderCallbacks.pfnSendVideoPes = _VideoCallback;
        rDecoderCallbacks.pfnUpdateVideoWp = VDEC_SetWptr;
        rDecoderCallbacks.pfnSendAudioPes = AUD_SendAudioPes;

        DMX_SetDecoderCallbacks(&rDecoderCallbacks);
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_GetCounters(INT32 i4Argc, const CHAR** aszArgv)
{
    ASSERT(aszArgv != NULL);

    if (i4Argc == 1)
    {
        // Show all counters of active PIDs
        DMX_PID_T rPid;
        UINT32 i;

        for (i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
            if (DMX_CLIGetPid(i, DMX_PID_FLAG_VALID, &rPid))
            {
                if (rPid.fgEnable)
                {
                    _ShowPidCounters(i);
                }
            }
        }
    }
    else
    {
        UINT8 u1Pidx;

        u1Pidx = (UINT8)StrToInt(aszArgv[1]);
        _ShowPidCounters(u1Pidx);
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_ResetCounters(INT32 i4Argc, const CHAR** aszArgv)
{
    ASSERT(aszArgv != NULL);

    if (i4Argc == 1)
    {
        // Reset all counters
        UINT8 i;

        for (i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
            if (!DMX_ResetPidCounters(i))
            {
                Printf("Fail to reset counters of PID index %u\n", i);
                return -1;
            }
        }
    }
    else
    {
        UINT8 u1Pidx;

        u1Pidx = (UINT8)StrToInt(aszArgv[1]);
        if (!DMX_ResetPidCounters(u1Pidx))
        {
            Printf("Fail to reset counters of PID index %u\n", u1Pidx);
            return -1;
        }
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_Query(INT32 i4Argc, const CHAR** aszArgv)
{
    ASSERT(aszArgv != NULL);

    if (i4Argc == 1)
    {
        // Show all counters of active PIDs
        DMX_PID_T rPid;
        UINT32 i;

        for (i = 0; i < DMX_NUM_PID_INDEX; i++)
        {
            if (DMX_CLIGetPid(i, DMX_PID_FLAG_VALID, &rPid))
            {
                if (rPid.fgEnable)
                {
                    _QueryPid(i);
                }
            }
        }
    }
    else
    {
        UINT8 u1Pidx;

        u1Pidx = (UINT8)StrToInt(aszArgv[1]);
        _QueryPid(u1Pidx);
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_RcvPktCount(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4RcvPkt0, u4DropPkt0, u4RcvPkt1, u4DropPkt1, u4RcvCount, u4DropCount;
    UINT32 i, u4MsTime, u4Period, u4Reg;
    UINT8 u1Framer;

#ifdef CC_DMX_ATSC_MPEG_ONLY
    u1Framer = 0;
#else
    u1Framer = DMX_GetFramerIndex();
#endif  // CC_DMX_ATSC_MPEG_ONLY

    u4MsTime = 100;

    if(i4Argc < 2)
    {
        Printf("%s ms_sec framer\n", aszArgv[0]);
    }

    if(i4Argc >= 2)
    {
        u4MsTime = StrToInt(aszArgv[1]);
    }

    if(i4Argc >= 3)
    {
        u1Framer = StrToInt(aszArgv[2]);
    }

    if(u1Framer >= 2)
    {
        Printf("Framer exceed\n");
        return -1;
    }

    u4Period = 10;
    u4Reg = (u1Framer == 0) ? 0x434 : 0x438;
    u4RcvCount = 0;
    u4DropCount = 0;

    u4RcvPkt0 = (UINT32)(IO_READ32(DEMUX0_BASE, u4Reg) & 0xFFFF);
    u4DropPkt0 = (UINT32)((IO_READ32(DEMUX0_BASE, u4Reg) >> 16) & 0xFFFF);

    for(i=0; i<u4MsTime; i+= u4Period)
    {
        x_thread_delay(u4Period);
        u4RcvPkt1 = (UINT32)(IO_READ32(DEMUX0_BASE, u4Reg) & 0xFFFF);
        u4DropPkt1 = (UINT32)((IO_READ32(DEMUX0_BASE, u4Reg) >> 16) & 0xFFFF);

        if(u4RcvPkt1 < u4RcvPkt0)
        {
            u4RcvCount += ((0x10000 + u4RcvPkt1) - u4RcvPkt0);
        }
        else
        {
            u4RcvCount += (u4RcvPkt1 - u4RcvPkt0);
        }

        if(u4DropPkt1 < u4DropPkt0)
        {
            u4DropCount += ((0x10000 + u4DropPkt1) - u4DropPkt0);
        }
        else
        {
            u4DropCount += (u4DropPkt1 - u4DropPkt0);
        }

        u4RcvPkt0 = u4RcvPkt1;
        u4DropPkt0 = u4DropPkt1;
    }

    Printf("Framer: %u, Period: %u ms, receive %u pkt, drop %u pkt\n",
                u1Framer, u4MsTime, u4RcvCount, u4DropCount);

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
#ifdef CC_MT5363
static INT32 _DMXCLI_Measure(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4MsTime, i, u4PsiTotal;
    DMX_INT_COUNTERS_T rInt;
    DMX_PID_COUNTERS_T rCounters;
    DMX_PID_T rPid;    
    
    if(i4Argc != 2)
    {
        Printf("%s time_ms\n", aszArgv[0]);
        return 0;
    }

    u4MsTime = StrToInt(aszArgv[1]);
    if(!DMX_EnableMeasure(u4MsTime))
    {
        return -1;
    }

    x_thread_delay(u4MsTime + 500);

    DMX_GetIntrCounters(&rInt);

    Printf("Measure time: %d ms\n", u4MsTime);
    Printf("Int counters, total:%d, video:%d, audio:%d, pes:%d, psi:%d, fvr:%d\n",
        rInt.u4TotalIntr, rInt.u4VidIntr, rInt.u4AudIntr, 
        (rInt.u4PesIntr - rInt.u4VidIntr - rInt.u4AudIntr), 
        rInt.u4PsiIntr, rInt.u4FvrIntr);

    u4PsiTotal = 0;
    for(i=0; i<DMX_NUM_PID_INDEX; i++)
    {
        if(DMX_CLIGetPid(i, DMX_PID_FLAG_VALID | DMX_PID_FLAG_BUFFER, &rPid))
        {
            if(rPid.fgEnable)
            {
                UNUSED(DMX_GetPidCounters(i, &rCounters));
                switch(rPid.ePidType)
                {
                case DMX_PID_TYPE_PSI:
                    u4PsiTotal += rCounters.u4DataSize;
                    Printf("%02d, PSI, size:%d\n", i, rCounters.u4DataSize);
                    break;

                case DMX_PID_TYPE_ES_VIDEO:    
                    Printf("%02d, VID, size:%d\n", i, rCounters.u4DataSize);
                    break;

                case DMX_PID_TYPE_ES_AUDIO:
                    Printf("%02d, AUD, size:%d\n", i, rCounters.u4DataSize);
                    break;

                case DMX_PID_TYPE_PES:
                    Printf("%02d, PES, size:%d\n", i, rCounters.u4DataSize);
                    break;

                default:
                    break;
                }
            }
        }
    }

    Printf("PSI total size: %d\n", u4PsiTotal);

    return 0;
}
#endif // CC_MT5363

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
#ifdef CC_MT5387
static INT32 _DMXCLI_MemoryUsage(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1Pidx;
    DMX_MEM_TRACKER_T memTracker;

    if(i4Argc != 1)
    {
        Printf("Usage: %s\n", aszArgv[0]);
        Printf("Print the status of DMX memory alloation.\n");
        return -1;
    }

    DMX_GetMemoryUsage(&memTracker);

    Printf("[DMX]Memory allocation (global peak): %u\n", memTracker.u4GlobalPeak);
    Printf("[DMX]Memory allocation (global current): %u\n", memTracker.u4GlobalCurrent);
    for (u1Pidx = 0; u1Pidx < DMX_NUM_PID_INDEX; u1Pidx++)
    {
        if (memTracker.arPidMemUsage[u1Pidx].fgValid)
        {
            Printf("[DMX]  Memory (pidx %2d, peak): %u\n",
                u1Pidx, memTracker.arPidMemUsage[u1Pidx].u4PeakUsage);
            Printf("[DMX]  Memory (pidx %2d, current): %u\n",
                u1Pidx, memTracker.arPidMemUsage[u1Pidx].u4CurrentUsage);
        }
        else
        {
            if (memTracker.arPidMemUsage[u1Pidx].u4PeakUsage != 0)
            {
                Printf("[DMX]  Memory (pidx %2d, inactive, peak): %u\n",
                    u1Pidx, memTracker.arPidMemUsage[u1Pidx].u4PeakUsage);
            }
        }
    }

    return 0;
}
#endif // CC_MT5387

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_Version(INT32 i4Argc, const CHAR** aszArgv)
{
    UNUSED(i4Argc);
    UNUSED(aszArgv);

    DMX_Version();

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_Power(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgEnable;

    if(i4Argc != 2)
    {
        Printf("%s 0 | 1\n", aszArgv[0]);
        Printf("    0: off\n");
        Printf("    1: on\n");
        return 0;
    }

    fgEnable = (StrToInt(aszArgv[1]) == 0) ? FALSE : TRUE;

    DMX_EnablePower(fgEnable);

    Printf("OK\n");

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  The handler routine for the capture function in the one-shot mode.
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static BOOL _CaptureHandlerOneshotMode(DMX_CAPTURE_NOTIFY_CODE_T eCode,
    const DMX_CAPTURE_INFO_T* prInfo, void* pvTag)
{
    DMX_CAPTURE2_T rCapture;

    x_memset((void*)&rCapture, 0, sizeof(DMX_CAPTURE2_T));
    DMX_SetCapture2(FALSE, &rCapture);

    _fgCaptureEnable = FALSE;

    LOG(0, "Capture finish\n");

    return TRUE;
}


//-----------------------------------------------------------------------------
/** Function
 *  The handler routine for the capture function in the continuous mode.
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static BOOL _CaptureHandlerContinuousMode(DMX_CAPTURE_NOTIFY_CODE_T eCode,
    const DMX_CAPTURE_INFO_T* prInfo, void* pvTag)
{
    DMX_UpdateCaptureReadPointer(prInfo->u4Wp);
    LOG(5, "CapBuf start:0x%08X, end:0x%08X, RP:0x%08X, WP:0x%08X\n",
        prInfo->u4BufStart, prInfo->u4BufEnd, prInfo->u4Rp, prInfo->u4Wp);

    return TRUE;
}


//-----------------------------------------------------------------------------
static void _PrintCaptureUsage(const CHAR* str)
{
    Printf("Usage: %s [on|off] [bypass_dev] [oneshot|loop]\n", str);
    Printf("   bypass_dev: [none | framer0 | framer1 | pvr | ddi]\n");
    Printf("        0: none\n");
    Printf("        1: framer0\n");
    Printf("        2: framer1\n");
    Printf("        3: pvr\n");
    Printf("        4: ddi\n");
    Printf("   default mode: oneshot\n");
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_Capture(INT32 i4Argc, const CHAR ** aszArgv)
{
    DMX_CAPTURE2_T rCapture;
    UINT32 u4Dev, u4BypassDev;
    BOOL fgEnable;

    ASSERT(aszArgv != NULL);

    if ((i4Argc < 3) || (i4Argc > 4))
    {
        _PrintCaptureUsage(aszArgv[0]);
        return -1;
    }

    u4Dev = StrToInt(aszArgv[2]);
    switch(u4Dev)
    {
    case 0:
        u4BypassDev = (UINT32)DMX_CAPTURE_BYPASS_NONE;
        break;
    case 1:
        u4BypassDev = (UINT32)DMX_CAPTURE_BYPASS_FRAMER0;
        break;
    case 2:
        u4BypassDev = (UINT32)DMX_CAPTURE_BYPASS_FRAMER1;
        break;
    case 3:
        u4BypassDev = (UINT32)DMX_CAPTURE_BYPASS_PVR;
        break;
    case 4:
        u4BypassDev = (UINT32)DMX_CAPTURE_BYPASS_DDI;
        break;
    default:
        _PrintCaptureUsage(aszArgv[0]);
        return -1;
    }

    if(x_strcmp(aszArgv[1], "on") == 0)
    {
        fgEnable = TRUE;
    }
    else if(x_strcmp(aszArgv[1], "off") == 0)
    {
        fgEnable = FALSE;
    }
    else
    {
        _PrintCaptureUsage(aszArgv[0]);
        return -1;
    }

    if(fgEnable)
    {
        if(_fgCaptureEnable)
        {
            Printf("Already enable\n");
            return -1;
        }

        if((_u4CaptureBufAddr == 0) || (_u4CaptureBufSize == 0))
        {
            Printf("Please call capturebuf CLI to allocate buffer first\n");
            return -1;
        }

        rCapture.u4BytePassDev = u4BypassDev;
        rCapture.u4BufStart = _u4CaptureBufAddr;
        rCapture.u4BufSize = _u4CaptureBufSize;
        rCapture.u4Threshold = _u4CaptureBufSize - 1024;
        rCapture.pvCaptureTag = NULL;

        // Determine to use the One-shot mode or the Continuous mode?
        // Default: one-shot mode
        if (i4Argc == 4)
        {
            if (x_strcmp(aszArgv[3], "oneshot") == 0)
            {
                rCapture.pfnCaptureHandler = _CaptureHandlerOneshotMode;
            }
            else if (x_strcmp(aszArgv[3], "loop") == 0)
            {
                rCapture.pfnCaptureHandler = _CaptureHandlerContinuousMode;
            }
            else
            {
                _PrintCaptureUsage(aszArgv[0]);
                return -1;
            }
        }
        else
        {
            rCapture.pfnCaptureHandler = _CaptureHandlerOneshotMode;
        }

        if (!DMX_SetCapture2(TRUE, &rCapture))
        {
            Printf("Fail to enable capture\n");
            return -1;
        }

        _fgCaptureEnable = TRUE;

        Printf("Capture start...\n");
    }
    else
    {
        x_memset((void*)&rCapture, 0, sizeof(DMX_CAPTURE2_T));
        if (!DMX_SetCapture2(FALSE, &rCapture))
        {
            Printf("Fail to disable capture\n");
            return -1;
        }

        _fgCaptureEnable = FALSE;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_CaptureAllocBuf
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_CaptureAllocBuf(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Size;

    ASSERT(aszArgv != NULL);

    if(i4Argc != 2)
    {
        Printf("%s [size]\n", aszArgv[0]);
        return 0;
    }

    u4Size = StrToInt(aszArgv[1]);

    if(_u4CaptureBufAddr != 0)
    {
        if(!BSP_FreeAlignedDmaMemory(_u4CaptureBufAddr))
        {
            return -1;
        }

        _u4CaptureBufAddr = 0;
        _u4CaptureBufSize = 0;
    }

    // Do not use x_mem_alloc() because of the cache consistency problem.
    _u4CaptureBufAddr = (UINT32)BSP_AllocAlignedDmaMemory(u4Size,
                                    DMX_CA_BUFFER_ALIGNMENT);
    if(_u4CaptureBufAddr == 0)
    {
        Printf("Allocate buffer fail!\n");
        return -1;
    }

    _u4CaptureBufSize = u4Size;

    Printf("Buffer addr: 0x%x, size: 0x%x\n", VIRTUAL(_u4CaptureBufAddr), VIRTUAL(_u4CaptureBufSize));

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_CaptureInfo
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_CaptureInfo(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_CAPTURE_INFO_T rInfo;

    if(!DMX_GetCaptureInfo(&rInfo))
    {
        return -1;
    }

//    Printf("Buffer: 0x%x ~ 0x%x, size: 0x%x\n", rInfo.u4BufStart, rInfo.u4BufEnd,
//                    rInfo.u4BufEnd - rInfo.u4BufStart);
    Printf("Buffer: 0x%x ~ 0x%x\n", VIRTUAL(rInfo.u4Rp), VIRTUAL(rInfo.u4Wp));
    Printf("Datasize: 0x%x\n", rInfo.u4DataSize);

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_CaptureInfo
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_ChangeChannelCaptureInit(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Size;

    ASSERT(aszArgv != NULL);

    if(i4Argc != 2)
    {
        Printf("%s [size]\n", aszArgv[0]);
        return 0;
    }

    u4Size = StrToInt(aszArgv[1]);

    DMX_Capture_ChannelChangeInit(u4Size);    

    return 0;
}


static INT32 _DMXCLI_ChangeChannelCapture(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_InformChannelChange();    

    return 0;
}

    
//-----------------------------------------------------------------------------
/** _DMXCLI_SetErrLevel
 */
//-----------------------------------------------------------------------------
EXTERN void _DMX_PanicDisplay(void);
static INT32 _DMXCLI_SetErrLevel(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4ErrLevel;

    _DMX_PanicDisplay();

    if(i4Argc != 2)
    {
        Printf("%s 0|1|2\n", aszArgv[0]);
        Printf("   0:assert, 1:reset, 2:bypass\n");
        return 0;
    }

    u4ErrLevel = StrToInt(aszArgv[1]);
    if(u4ErrLevel >= 3)
    {
        Printf("%s on|off\n", aszArgv[0]);
        Printf("   on|off: reset on or off\n");
        return -1;
    }

    DMX_SetErrLevel((DMX_ERR_LEVEL_T)u4ErrLevel);

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_SetScrambleScheme
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetScrambleScheme(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgOrg;

    if(i4Argc != 2)
    {
        Printf("%s 0|1\n", aszArgv[0]);
        Printf("        0: org, 1: new\n");
        if(DMX_GetScrambleScheme())
        {
            Printf("Scramble new\n");
        }
        else
        {
            Printf("Scramble org\n");
        }
        return 0;
    }

    fgOrg = (BOOL)StrToInt(aszArgv[1]);
   
    DMX_SetScrambleScheme(!fgOrg);
    
    return 0;
}


#ifdef TIME_SHIFT_SUPPORT
//-----------------------------------------------------------------------------
/** _DMXCLI_TimeShiftQuery
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_TimeShiftQuery(INT32 i4Argc, const CHAR** aszArgv)
{
    FVR_QUERY_INFO_T rInfo;
    FVR_PID_T rFVRPid;
    UINT8 i;
    DMX_PVRPLAY_COUNTER_T rCounter;

    //------------------------------------------------------------
    // FVR buffer
    //------------------------------------------------------------
    if(FVR_IsRecording())
    {
        Printf("FVR is recording\n");
    }
    else
    {
        Printf("FVR stop record\n");
    }

    FVR_QueryStatus(&rInfo);
    Printf("Receive pkt count: %d\n", rInfo.u4PktCount);
    Printf("Buf: 0x%08x ~ 0x%08x, WP: 0x%08x, RP: 0x%08x\n",
                        rInfo.u4BufStart, rInfo.u4BufEnd, rInfo.u4Wp, rInfo.u4Rp);
    Printf("Buf peak: 0x%x\n", rInfo.u4PktCount);
    Printf("Overflow count: %d\n", rInfo.u4OverflowCount);

    //------------------------------------------------------------
    // Reocrd PID
    //------------------------------------------------------------
    for(i=0; i<FVR_NUM_PID_INDEX; i++)
    {
        if(!FVR_GetPid(i, FVR_PID_FLAG_ENABLE | FVR_PID_FLAG_PID | FVR_PID_FLAG_PICSEARCH |
                          FVR_PID_FLAG_PKTSWAP | FVR_PID_FLAG_SCRAMBLE_ENABLE, &rFVRPid))
        {
            return -1;
        }

        if(rFVRPid.fgEnable)
        {
            Printf("FVR, %02d, Pid: 0x%x enable, pic_serach:%d, swap:%d, encry:%d\n",
                        i, rFVRPid.u2Pid, rFVRPid.fgPicSearch, rFVRPid.fgPktSwap,
                        rFVRPid.fgScramble);
        }
    }

    Printf("\n");

    //------------------------------------------------------------
    // Play info
    //------------------------------------------------------------
    DMX_PVRPlay_GetCounter(&rCounter);
    Printf("Play----\n");
    Printf("Use timestamp: %d\n", (INT32)rCounter.fgUseTimestamp);
    Printf("x1:Move, count:%d, check:%d, size:0x%x\n",
                    rCounter.u4MoveCount, rCounter.u4MoveCheck, rCounter.u4MoveSize);
    Printf("not1:Move, count:%d, check:%d, size:0x%x\n",
                    rCounter.u4SingleCount, rCounter.u4SingleCheck, rCounter.u4SingleSize);
    Printf("Datasize: 0x%x\n", rCounter.u4DataSize);

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_TimeShiftResetCounter
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_TimeShiftResetCounter(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PVRPlay_ResetCounter();

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}
#endif  // TIME_SHIFT_SUPPORT


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
#ifndef CC_DMX_ATSC_MPEG_ONLY
static INT32 _DMXCLI_SetVideoType(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_VIDEO_TYPE_T eVideoType;
    
    if(i4Argc != 2)
    {
        Printf("%s [mpeg|h264|avs|wmv...]\n", aszArgv[0]);
        Printf("      mpeg:0, h264:1, avs:2, wvm:3...\n");
        return 0;
    }

    eVideoType = (DMX_VIDEO_TYPE_T)StrToInt(aszArgv[1]);

    if (!DMX_SetVideoType(eVideoType))
    {
        return -1;
    }

    return 0;
}
#endif  // CC_DMX_ATSC_MPEG_ONLY


#ifdef CC_DMX_ERR_RECOVERY_TEST
//-----------------------------------------------------------------------------
/** _DMXCLI_PanicTest
 */
//-----------------------------------------------------------------------------
BOOL DMX_Test_Enable = FALSE;
static INT32 _DMXCLI_PanicTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Loop, u4Period, i;

    if(i4Argc != 3)
    {
        Printf("%s loop_count period_ms\n", aszArgv[0]);
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Period = StrToInt(aszArgv[2]);

    for(i=0; i<u4Loop; i++)
    {
        Printf("------ %d ------\n", i);
        DMX_Test_Enable = TRUE;
        x_thread_delay(u4Period);
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_MonPanicTest
 */
//-----------------------------------------------------------------------------
EXTERN void _DMX_PanicMicroCode(void);
static INT32 _DMXCLI_MonPanicTest(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Loop, u4Period, i;

    if(i4Argc != 3)
    {
        Printf("%s loop_count period_ms\n", aszArgv[0]);
        return 0;
    }

    u4Loop = StrToInt(aszArgv[1]);
    u4Period = StrToInt(aszArgv[2]);

    for(i=0; i<u4Loop; i++)
    {
        Printf("------ %d ------\n", i);
        _DMX_PanicMicroCode();
        x_thread_delay(u4Period);
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** DMXCLI_FVROverflow
*/
//-----------------------------------------------------------------------------
extern void _FVR_uPErrorInt(UINT8 u1Idx);
static INT32 DMXCLI_FVROverflow(INT32 i4Argc, const CHAR** aszArgv)
{
    _FVR_uPErrorInt(0);

    return 0;
}
#endif  // CC_DMX_ERR_RECOVERY_TEST

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_CheckAW(INT32 i4Argc, const CHAR** aszArgv)
{
#if defined(CC_MT5387) && !defined(CC_MT5363)
    UINT32 u4RegVal;
#endif  // CC_MT5387 && !CC_MT5363
    BOOL fgDriverSupport = FALSE, fguCodeSupport = FALSE;
    //-------------------------------------------------------------------------
    // --- Beginning of "check for AW Eraser" ---
    const UINT32 u4Timeout = 500;
    INT32 i4Result;
    // --- End of "check for AW Eraser" ---
    //-------------------------------------------------------------------------

    if (i4Argc != 1)
    {
        Printf("Usage: %s\n", aszArgv[0]);
        return -1;
    }

#if defined(CC_MT5387) && !defined(CC_MT5363)
    u4RegVal = BIM_READ32(0x660);
    if (u4RegVal & 0x200000)
    {
        // Demod does not clear VCT.
        Printf("J-Chip feature: OFF\n");
    }
    else
    {
        // Demod clears VCT.
        Printf("J-Chip feature: ON\n");
    }
#endif  // CC_MT5387 && !CC_MT5363

    DMX_CheckAwSupport(&fgDriverSupport, &fguCodeSupport);

    Printf("AW support in driver: %s\n", fgDriverSupport ? "YES" : "NO");
    Printf("AW support in micro-code: %s\n", fguCodeSupport ? "YES" : "NO");

    //-------------------------------------------------------------------------
    // --- Beginning of "check for AW Eraser" ---
    DMX_AwEraserCheck_Init();
    DMX_AwEraserCheck_Set(TRUE);
    i4Result = DMX_AwEraserCheck_Get(u4Timeout);
    DMX_AwEraserCheck_Free();
    Printf("AW eraser check result: ");
    switch (i4Result)
    {
    case DMX_AW_ERR_OK:
        Printf("OK\n");
        break;
    case DMX_AW_ERR_ADDRESS_RANGE:
        Printf("out of address range\n");
        break;
    case DMX_AW_ERR_SECTION_LEN:
        Printf("incorrect section length\n");
        break;
    case DMX_AW_ERR_NO_MEM:
        Printf("out of memory\n");
        break;
    case DMX_AW_ERR_COPY_BUFFER:
        Printf("copy error\n");
        break;
    case DMX_AW_ERR_INSUFFICIENT_LEN:
        Printf("insufficient length\n");
        break;
    case DMX_AW_ERR_NOT_ZEROED_OUT:
        Printf("VCT/SLD not cleared\n");
        break;
    case DMX_AW_ERR_TIMEOUT:
        Printf("timeout\n");
        break;
    default:
        Printf("unknown error\n");
        break;
    }

    if (i4Result)
    {
        return i4Result;
    }
    // --- End of "check for AW Eraser" ---
    //-------------------------------------------------------------------------

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Error code
 */
//-----------------------------------------------------------------------------
static BOOL _ParseSection(const UINT8* au1Buf, UINT32 u4Size,
    SECTION_INFO_T* prSecInfo)
{
    if (u4Size < 3)
    {
        return FALSE;
    }

    if ((au1Buf == NULL) || (prSecInfo == NULL))
    {
        return FALSE;
    }

    x_memset(prSecInfo, 0, sizeof(SECTION_INFO_T));

    prSecInfo->u1TableId = au1Buf[0];
    prSecInfo->fgSectionSyntaxIndicator = ((au1Buf[1] & 0x80) != 0);
    prSecInfo->fgPrivateIndicator = ((au1Buf[1] & 0x40) != 0);
    prSecInfo->u2SectionLength = ((au1Buf[1] & 0xf) << 8) | au1Buf[2];
    if (prSecInfo->fgSectionSyntaxIndicator)
    {
        if (u4Size < 8)
        {
            return FALSE;
        }

        prSecInfo->u2TableIdExtension = (au1Buf[3] << 8) | au1Buf[4];
        prSecInfo->u1VersionNumber = (au1Buf[5] & 0x3e) >> 1;
        prSecInfo->fgCurrentNextIndicator = ((au1Buf[5] & 1) != 0);
        prSecInfo->u1SectionNumber = au1Buf[6];
        prSecInfo->u1LastSectionNumber = au1Buf[7];
    }

    return TRUE;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval TRUE            Succeed
 *  @retval FALSE           Error code
 */
//-----------------------------------------------------------------------------
static BOOL _SectionHandler(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
    UINT32 u4Data, const void* pvNotifyTag)
{
    SECTION_INFO_T rSecInfo;
    UINT32 u4Size;
    DMX_NOTIFY_INFO_PSI_T* prInfo;
    UINT8 au1Buf[16];

    ASSERT(eCode == DMX_NOTIFY_CODE_PSI);
    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    prInfo = (DMX_NOTIFY_INFO_PSI_T*)u4Data;
    ASSERT(prInfo != NULL);

    u4Size = MIN(prInfo->u4SecLen, 16);
    VERIFY(DMX_PSI_GetBuffer(u1Pidx, prInfo->u1SerialNumber, prInfo->u4SecAddr,
        0, u4Size, au1Buf));
    VERIFY(_ParseSection(au1Buf, u4Size, &rSecInfo));

    LOG(5, "Section - pidx: %u, section: 0x%08x, size: %u\n",
        u1Pidx, prInfo->u4SecAddr, prInfo->u4SecLen);

    LOG(5, "  Data: %02x  %02x  %02x  %02x  %02x  %02x  %02x %02x\n",
        au1Buf[0], au1Buf[1], au1Buf[2], au1Buf[3], au1Buf[4], au1Buf[5],
        au1Buf[6], au1Buf[7]);

    LOG(5, "  table_id: 0x%02x, section_syntax_indicator: %u, section_length: %u\n",
        rSecInfo.u1TableId, rSecInfo.fgSectionSyntaxIndicator,
        rSecInfo.u2SectionLength);

    LOG(5, "  table_id_extension: 0x%04x, version_number: %u, section_number: %u\n",
        rSecInfo.u2TableIdExtension, rSecInfo.u1VersionNumber,
        rSecInfo.u1SectionNumber);

    if (!DMX_PSI_UnlockBuffer(u1Pidx, prInfo->u1SerialNumber, prInfo->u4SecLen,
        prInfo->u4SecAddr))
    {
        LOG(5, "Fail to unlock PSI buffer!\n");
        return FALSE;
    }

    UNUSED(pvNotifyTag);
    return TRUE;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetPsiPidCallback(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PID_T rPid;
    UINT16 u2Pid;
    UINT8 u1PidIndex;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 3)
    {
        Printf("Usage: %s pid_index pid\n", aszArgv[0]);
        return -1;
    }

    u1PidIndex = (UINT8)StrToInt(aszArgv[1]);
    u2Pid = (UINT16)StrToInt(aszArgv[2]);

    rPid.u1TsIndex = _u1TsIndex;
    rPid.u1DeviceId = 0;
    rPid.u1KeyIndex = 0;
    rPid.fgEnable = TRUE;
    rPid.u1SteerMode = DMX_STEER_TO_FTUP;
    rPid.fgAllocateBuffer = TRUE;
    rPid.ePcrMode = DMX_PCR_MODE_NONE;
    rPid.ePidType = DMX_PID_TYPE_PSI;
    rPid.eDescMode = DMX_DESC_MODE_NONE;
    rPid.u4BufAddr = 0;
    rPid.u4BufSize = DMX_DEFAULT_PSI_FIFO_SIZE;
    rPid.u2Pid = u2Pid;
    rPid.pfnNotify = _SectionHandler;
    rPid.pvNotifyTag = NULL;

    if (!DMX_SetPid(u1PidIndex, DMX_PID_FLAG_ALL, &rPid))
    {
        return -1;
    }

    return 0;
}


#ifdef CC_DMX_TS_LOOPBACK
//-----------------------------------------------------------------------------
/** _DMXCLI_Loopback
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_Loopback(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgEnable;

    if(i4Argc != 2)
    {
        Printf("%s [on|off]\n", aszArgv[0]);
        return 0;
    }

    if(x_strcmp(aszArgv[1], "on") == 0)
    {
        fgEnable = TRUE;
    }
    else if(x_strcmp(aszArgv[1], "off") == 0)
    {
        fgEnable = FALSE;
    }
    else
    {
        Printf("%s [on|off]\n", aszArgv[0]);
        return -1;
    }

    if(fgEnable)
    {
        if(!DMX_Loopback_Set())
        {
            return -1;
        }

        if(!DMX_Loopback_Enable(TRUE))
        {
            return -1;
        }
    }
    else
    {
        if(!DMX_Loopback_Enable(FALSE))
        {
            return -1;
        }

        if(!DMX_Loopback_Free())
        {
            return -1;
        }
    }

    return 0;
}
#endif  // CC_DMX_TS_LOOPBACK


#ifdef CC_DMX_DEBUG

//-----------------------------------------------------------------------------
/** _DMXCLI_SelInputType
 *  Command: init
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SelInputType(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_INPUT_TYPE_T rType;

    if(i4Argc != 2)
    {
        Printf("%s [bts|pts|ps|pvr_ts|mm|es|drm]\n", aszArgv[0]);
        Printf("    bts:    broadcast ts\n");
        Printf("    pts:    playback ts\n");
        Printf("    pvr_ts: PVR playback \n");
        Printf("    ps:     program stream\n");
        Printf("    mm:     Multimedia playback \n");
        Printf("    es:     elementary stream\n");
        Printf("    drm:    DRM\n");
        return 0;
    }

    if(x_strncmp(aszArgv[1], "bts", sizeof("bts"))==0)
    {
        rType = DMX_IN_BROADCAST_TS;
    }
    else if(x_strncmp(aszArgv[1], "pts", sizeof("pts"))==0)
    {
        rType = DMX_IN_PLAYBACK_TS;
    }
    else if(x_strncmp(aszArgv[1], "pvr_ts", sizeof("pvr_ts"))==0)
    {
        rType = DMX_IN_PVR_TS;
    }
    else if(x_strncmp(aszArgv[1], "mm", sizeof("mm"))==0)
    {
        rType = DMX_IN_PLAYBACK_MM;
    }
    else if(x_strncmp(aszArgv[1], "ps", sizeof("ps"))==0)
    {
        rType = DMX_IN_PLAYBACK_PS;
    }
    else if (x_strncmp(aszArgv[1], "es", sizeof("es"))==0)
    {
        rType = DMX_IN_PLAYBACK_ES;
    }
    else
    {
        Printf("%s [bts|pts|ps|pvr_ts|mm|es|drm]\n", aszArgv[0]);
        return -1;
    }

    if (!DMX_SelectInputType(rType))
    {
        return -1;
    }

    UNUSED(i4Argc);
    UNUSED(aszArgv);

    return 0;
}


#ifdef DMX_MEASURE_PSI_TIME
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_PSIGetMaxTime(INT32 i4Argc, const CHAR** aszArgv)
{
    HAL_TIME_T rTime;

    DMX_PSI_GetMaxTime(&rTime);

    Printf("Section max time: %u.%06u\n", rTime.u4Seconds, rTime.u4Micros);

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_PSIResetMaxTime(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_ResetPSIMaxTime();

    Printf("Reset OK\n");

    return 0;
}
#endif  // DMX_MEASURE_PSI_TIME

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetToPvr(INT32 i4Argc, const CHAR** aszArgv)
{
    ASSERT(aszArgv != NULL);

    if (i4Argc == 1)
    {
        Printf("Steering to PVR: %s\n", _fgToPvr ? "on" : "off");
    }

    if (x_strcmp(aszArgv[1], "on") == 0)
    {
        _fgToPvr = TRUE;
    }
    else if (x_strcmp(aszArgv[1], "off") == 0)
    {
        _fgToPvr = FALSE;
    }
    else
    {
        Printf("Usage: %s [on|off]\n", aszArgv[0]);
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetToFTuP(INT32 i4Argc, const CHAR** aszArgv)
{
    ASSERT(aszArgv != NULL);

    if (i4Argc == 1)
    {
        Printf("Steering to uP: %s\n", _fgToFTuP ? "on" : "off");
    }

    if (x_strcmp(aszArgv[1], "on") == 0)
    {
        _fgToFTuP = TRUE;
    }
    else if (x_strcmp(aszArgv[1], "off") == 0)
    {
        _fgToFTuP = FALSE;
    }
    else
    {
        Printf("Usage: %s [on|off]\n", aszArgv[0]);
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetDebugPictureFinder(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgDebugPictureFinder;

    ASSERT(aszArgv != NULL);

    if (i4Argc == 1)
    {
        fgDebugPictureFinder = DMX_IsDebugPictureFinder();
        Printf("debug_picture_finder: %u\n", fgDebugPictureFinder ? 1 : 0);
        return 0;
    }

    if (x_strcmp(aszArgv[1], "on") == 0)
    {
        fgDebugPictureFinder = TRUE;
    }
    else if (x_strcmp(aszArgv[1], "off") == 0)
    {
        fgDebugPictureFinder = FALSE;
    }
    else
    {
        Printf("Usage: %s [on|off]\n", aszArgv[0]);
        return -1;
    }

    DMX_SetDebugPictureFinder(fgDebugPictureFinder);

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetBypassMode(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1TsIndex = _u1TsIndex, u1PacketSize = 188;
    BOOL fgRet;

    ASSERT(aszArgv != NULL);

    if (i4Argc > 1)
    {
        u1PacketSize = (UINT8)StrToInt(aszArgv[1]);
    }

    if (i4Argc > 2)
    {
        u1TsIndex = (UINT8)StrToInt(aszArgv[2]);
    }

    if ((i4Argc > 3) || (u1TsIndex > 1))
    {
        Printf("Usage: %s [packet_size] [TS]\n", aszArgv[0]);
    }

    Printf("Setting bypass mode: TS = %u, packet size = %u...\n",
        u1TsIndex, u1PacketSize);

    fgRet = DMX_SetBypassMode(u1TsIndex, u1PacketSize);
    Printf("%s!\n", fgRet ? "done" : "fail");

    return fgRet ? 0 : -1;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetErrorHandling(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Code;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 2)
    {
        Printf("Usage: %s handling_code\n", aszArgv[0]);
        return -1;
    }

    u4Code = (UINT32)StrToInt(aszArgv[1]);
    DMX_SetFramerErrorHandlingTable(u4Code);

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetDesKey(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 au4Key[6], i;
    UINT8 u1Index;
    BOOL fgEven, fgOdd, fg3Des;

    ASSERT(aszArgv != NULL);

    if (i4Argc < 5)
    {
        Printf("Usage: %s index (even/odd/both) k1_hi k1_lo"
            " [k2_hi k2_lo k3_hi k3_lo]\n", aszArgv[0]);
        return -1;
    }

    u1Index = (UINT8)StrToInt(aszArgv[1]);

    fgEven = (fgOdd = FALSE);
    if (x_strcmp(aszArgv[2], "even") == 0)
    {
        fgEven = TRUE;
    }
    else if (x_strcmp(aszArgv[2], "odd") == 0)
    {
        fgOdd = TRUE;
    }
    else if (x_strcmp(aszArgv[2], "both") == 0)
    {
        fgEven = (fgOdd = TRUE);
    }

    for (i = 0; i < 6; i++)
    {
        if (i4Argc > (INT32)(i + 3))
        {
            au4Key[i] = (UINT32)StrToInt(aszArgv[i + 3]);
        }
        else
        {
            au4Key[i] = 0;
        }
    }

    fg3Des = (i4Argc > 5);  // Only 3DES uses all 3 keys

    if (fgEven)
    {
        VERIFY(DMX_SetDesKey(u1Index, fg3Des, TRUE, au4Key));
    }

    if (fgOdd)
    {
        VERIFY(DMX_SetDesKey(u1Index, fg3Des, FALSE, au4Key));
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetDesIV(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4IvHi, u4IvLo;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 3)
    {
        Printf("Usage: %s IV_hi IV_lo\n", aszArgv[0]);
        return -1;
    }

    u4IvHi = (UINT32)StrToInt(aszArgv[1]);
    u4IvLo = (UINT32)StrToInt(aszArgv[2]);

    DMX_SetDesIV(u4IvHi, u4IvLo);

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetCaOutput(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4BufStart, u4BufEnd, u4BufSize = 0, u4Threshold;
    BOOL fgIsPs;

    ASSERT(aszArgv != NULL);

    if (i4Argc == 4)
    {
        fgIsPs = (BOOL)StrToInt(aszArgv[1]) ? TRUE : FALSE;
        u4BufStart = (UINT32)StrToInt(aszArgv[2]);
        u4BufEnd = (UINT32)StrToInt(aszArgv[3]);
        if (!DMX_IsAligned(u4BufStart, DMX_CA_BUFFER_ALIGNMENT) ||
            !DMX_IsAligned(u4BufEnd, DMX_CA_BUFFER_ALIGNMENT))
        {
            Printf("CA output buffer addresses must be %u-byte alignment!\n",
                DMX_CA_BUFFER_ALIGNMENT);
            return -1;
        }
    }
    else if (i4Argc == 3)
    {
        fgIsPs = (BOOL)StrToInt(aszArgv[1]) ? TRUE : FALSE;
        u4BufSize = (UINT32)StrToInt(aszArgv[2]);
        if (!DMX_IsAligned(u4BufSize, DMX_CA_BUFFER_ALIGNMENT))
        {
            Printf("CA output buffer size must be %u-byte alignment!\n",
                DMX_CA_BUFFER_ALIGNMENT);
            return -1;
        }
        u4BufStart = (UINT32)BSP_AllocAlignedDmaMemory(u4BufSize,
            DMX_CA_BUFFER_ALIGNMENT);
        if (u4BufStart == 0)
        {
            Printf("Out of memory!\n");
            return -1;
        }
        u4BufEnd = u4BufStart + u4BufSize;
    }
    else
    {
        Printf("Usage: %s is_ps(0,1) [buf_start buf_end]|[buf_size]\n",
            aszArgv[0]);
        return -1;
    }

    u4Threshold = u4BufSize / 4;    // FIXME

    if (DMX_SetCaOutputBuffer(TRUE, fgIsPs, u4BufStart, u4BufEnd, u4Threshold))
    {
        Printf("OK! Buffer: 0x%08x - 0x%08x\n", u4BufStart, u4BufEnd);
    }
    else
    {
        Printf("Fail\n");
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetAesKey(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i, u4KeyLen;
    UINT8 u1KeyIdx;
    BOOL fgEvenKey;
    UINT32 au4Key[8];

    if(i4Argc < 8)
    {
        Printf("%s [k_idx] [k_len] [e/o] [key...]\n", aszArgv[0]);
        return -1;
    }

    u1KeyIdx = StrToInt(aszArgv[1]);
    u4KeyLen = StrToInt(aszArgv[2]);

    if((u4KeyLen != 128) && (u4KeyLen != 192) && (u4KeyLen != 256))
    {
        Printf("Error key length, %d\n", u4KeyLen);
        return -1;
    }

    if(x_strncmp(aszArgv[3], "e", sizeof("e")) == 0)
    {
        fgEvenKey = TRUE;
    }
    else if(x_strncmp(aszArgv[3], "o", sizeof("o")) == 0)
    {
        fgEvenKey = FALSE;
    }
    else
    {
        Printf("%s [pidx] [k_len] [e/o] [key...]\n", aszArgv[0]);
        return -1;
    }

    for(i=0; i<8; i++)
    {
        au4Key[i] = 0;
    }

    if(u4KeyLen == 128)
    {
        for(i=0; i<4; i++)
        {
            au4Key[i] = StrToInt(aszArgv[4 + i]);
        }
    }
    else if(u4KeyLen == 192)
    {
        if(i4Argc < 10)
        {
            Printf("Not enough key len\n");
            return -1;
        }

        for(i=0; i<6; i++)
        {
            au4Key[i] = StrToInt(aszArgv[4 + i]);
        }
    }
    else if(u4KeyLen == 256)
    {
        if(i4Argc < 12)
        {
            Printf("Not enough key len\n");
            return -1;
        }

        for(i=0; i<8; i++)
        {
            au4Key[i] = StrToInt(aszArgv[4 + i]);
        }
    }

    if(!DMX_SetAesKeyLen(u4KeyLen))
    {
        return -1;
    }

    if(!DMX_SetAesKey(u1KeyIdx, fgEvenKey, au4Key))
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("Set AES key OK\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_SetAesIV(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 au4IV[4];

    ASSERT(aszArgv != NULL);

    if (i4Argc != 5)
    {
        Printf("Usage: %s iv_0 iv_1 iv_2 iv_3\n", aszArgv[0]);
        return -1;
    }

    au4IV[0] = (UINT32)StrToInt(aszArgv[1]);
    au4IV[1] = (UINT32)StrToInt(aszArgv[2]);
    au4IV[2] = (UINT32)StrToInt(aszArgv[3]);
    au4IV[3] = (UINT32)StrToInt(aszArgv[4]);

    DMX_SetAesIV(au4IV);

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 */
//-----------------------------------------------------------------------------
static INT32 _CmdSetPidKeyIdx(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PID_T rPid;
    UINT8 u1Pidx, u1Keyx;

    if(i4Argc != 4)
    {
        Printf("%s [pidx] [key_index] [mode]\n", aszArgv[0]);
        return -1;
    }

    u1Pidx = StrToInt(aszArgv[1]);
    u1Keyx = StrToInt(aszArgv[2]);

    ASSERT(u1Pidx < DMX_NUM_PID_INDEX);

    rPid.eDescMode = (DMX_DESC_MODE_T)StrToInt(aszArgv[3]);
    rPid.u1KeyIndex = u1Keyx;                   // Start from index 8;

    if (!DMX_SetPid(u1Pidx, DMX_PID_FLAG_DESC_MODE | DMX_PID_FLAG_KEY_INDEX, &rPid))
    {
        return FALSE;
    }

    Printf("Set Play PID OK\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_IdeCtrl(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgEnableIde = FALSE;

    ASSERT(aszArgv != NULL);

    if (i4Argc == 1)
    {
      //  Printf("IDE %s\n", DMX_IDE_IsEnabled() ? "on" : "off");
        return 0;
    }

    if (x_strcmp(aszArgv[1], "on") == 0)
    {
        fgEnableIde = TRUE;
    }
    else if (x_strcmp(aszArgv[1], "off") == 0)
    {
        fgEnableIde = FALSE;
    }
    else
    {
        Printf("Usage: %s (on,off)\n", aszArgv[0]);
        return -1;
    }

   // DMX_IDE_Enable(fgEnableIde);

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_ResetTest(INT32 i4Argc, const CHAR ** aszArgv)
{
    UINT32 u4Times = 100, i;

    ASSERT(aszArgv != NULL);

    if (i4Argc > 1)
    {
        u4Times = (UINT32)StrToInt(aszArgv[1]);
    }

    Printf("Reset %u times...\n", u4Times);

    for (i = 0; i < u4Times; i++)
    {
        IO_WRITE32(DEMUX0_BASE, REG_DMX_RESET, 0x80000000);

        while(1)
        {
            // Check if FTI is halted
            if (0x1 != ((IO_READ32(DEMUX0_BASE, REG_DMX_RESET) >> 28) & 0x3))
            {
                x_thread_delay(1);
            }
            else
            {
                break;
            }
        }

        Printf(".");
    }

    Printf("\nOK!\n");

    return 0;
}


#if 0
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_FrontEndTest(INT32 i4Argc, const CHAR ** aszArgv)
{
    UINT32 i, u4Count;

    u4Count = 1000;

    if(i4Argc == 2)
    {
        u4Count = StrToInt(aszArgv[1]);
    }

    for(i=0; i<1000; i++)
    {
        Printf("Loop %u -------------------\n", i);
        DMX_SetFrontEnd(0);
        x_thread_delay(5000);
        DMX_SetFrontEnd(2);
        x_thread_delay(500);
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_DramMeasure(INT32 i4Argc, const CHAR ** aszArgv)
{
    UINT32 u4Device, u4Times;

    if(i4Argc != 3)
    {
        Printf("%s device ms_time\n", aszArgv[0]);
        Printf("  device, 0:dmx, 1:ddi, 2:parser\n");
        return -1;
    }

    u4Device = StrToInt(aszArgv[1]);
    if(u4Device >= DMX_LOCAL_ARBITOR_NUM)
    {
        return -1;
    }

    u4Times = StrToInt(aszArgv[2]);

    if(!DMX_DRAMMeasure((DMX_LOCAL_ARBITOR_DEVICE_T)u4Device, u4Times))
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");

    return 0;
}
#endif  // 0

#if defined(__linux__)
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_FlushPSIBuffer(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1Pidx;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 2)
    {
        Printf("Usage: %s pid_index(0-%u)\n", aszArgv[0],
            DMX_NUM_PID_INDEX - 1);
        return -1;
    }

    u1Pidx = (UINT8)StrToInt(aszArgv[1]);
    if (! DMX_PSI_FlushBuffer(u1Pidx))
    {
        return -1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_FreePSIBuffer(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1Pidx;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 2)
    {
        Printf("Usage: %s pid_index(0-%u)\n", aszArgv[0],
            DMX_NUM_PID_INDEX - 1);
        return -1;
    }

    u1Pidx = (UINT8)StrToInt(aszArgv[1]);
    if (! DMX_PSI_FreeBuffer(u1Pidx))
    {
        return -1;
    }

    return 0;
}

//-----------------------------------------------------------------------------
/** _DMXCLI_DisableFilter
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_DisableFilter(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_FILTER_T rFilter;

    ASSERT(aszArgv != NULL);

    if(i4Argc < 2)
    {
        return -1;
    }

    rFilter.fgEnable = FALSE;

    if(!DMX_SetFilter(StrToInt(aszArgv[1]), 0, DMX_FILTER_FLAG_VALID, &rFilter));
    {
        return -1;
    }
    return 0;
}

static INT32 _DMXCLI_GetScrambleState(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4Pidx;

    ASSERT(aszArgv != NULL);

    if(i4Argc < 2)
    {
        return -1;
    }

    u4Pidx = StrToInt(aszArgv[1]);
    if(u4Pidx >= 32)
    {
        return -1;
    }

    return (INT32)DMX_GetScrambleState(u4Pidx);
}

#endif  // defined(__linux__)


#ifdef DMX_CLI_SUPPORT_MULTI2
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdSetMulti2CbcIV(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4IvHi, u4IvLo;

    if (i4Argc != 3)
    {
        Printf("Usage: %s IV_hi IV_lo\n", aszArgv[0]);
        return -1;
    }

    u4IvHi = (UINT32)StrToInt(aszArgv[1]);
    u4IvLo = (UINT32)StrToInt(aszArgv[2]);

    DMX_SetMulti2CbcIV(u4IvHi, u4IvLo);

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdSetMulti2OfbIV(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 u4IvHi, u4IvLo;

    if (i4Argc != 3)
    {
        Printf("Usage: %s IV_hi IV_lo\n", aszArgv[0]);
        return -1;
    }

    u4IvHi = (UINT32)StrToInt(aszArgv[1]);
    u4IvLo = (UINT32)StrToInt(aszArgv[2]);

    DMX_SetMulti2OfbIV(u4IvHi, u4IvLo);

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdSetMulti2SysKey(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT32 i;
    UINT32 au4Key[8];

    if (i4Argc != 9)
    {
        Printf("Usage: %s k0 k1 k2 k3 k4 k5 k6 k7\n", aszArgv[0]);
        return -1;
    }

    for (i = 0; i < 8; i++)
    {
        au4Key[i] = (UINT32)StrToInt(aszArgv[i + 1]);
    }

    DMX_SetMulti2SysKey(au4Key);

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _CmdSetMulti2Config(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT16 u2Iteration;
    BOOL fgOfbFreeRun;

    if (i4Argc != 3)
    {
        Printf("Usage: %s ofb_free_run(1,0) iteration(0-65535)\n",
            aszArgv[0]);
        return -1;
    }

    fgOfbFreeRun = (BOOL)StrToInt(aszArgv[1]);
    u2Iteration = (UINT16)StrToInt(aszArgv[2]);

    DMX_SetMulti2Config(fgOfbFreeRun, u2Iteration);

    return 0;
}
#endif   // DMX_CLI_SUPPORT_MULTI2

#ifdef ENABLE_MULTIMEDIA

//-----------------------------------------------------------------------------
/** _Dmx_PS_PreparsingCallback
 */
//-----------------------------------------------------------------------------
static BOOL _Dmx_PS_PreparsingCallback(UINT8 u1Pidx, DMX_NOTIFY_CODE_T eCode,
                                       UINT32 u4Data, const VOID* pvNotifyTag)
{
    DMX_NOTIFY_INFO_STREAM_ID_T rNotifyInfo;
    UINT32 u4Pairs;
    UINT8 *pu1Buf;

    if (eCode != DMX_NOTIFY_CODE_STREAM_ID)
    {
        Printf("Unknown notification code in preparsing callback\n");
        return FALSE;
    }

    rNotifyInfo = * (DMX_NOTIFY_INFO_STREAM_ID_T*) u4Data;
    pu1Buf = (UINT8*)(rNotifyInfo.u4DataAddr);
    u4Pairs = (rNotifyInfo.u4DataSize >> 1);
    Printf("Preparsing callback is invoked\n");
    Printf("    address: 0x%x\n", (UINT32)pu1Buf);
    Printf("    size: %u pairs (2-bytes pair)\n", u4Pairs);
/*
    UINT32 u4Index;
    for (u4Index = 0; u4Index < u4Pairs; u4Index++)
    {
        UINT8 u1SID, u1SSID;
        u1SSID = pu1Buf[u4Index * 2];
        u1SID  = pu1Buf[u4Index * 2 + 1];
        Printf("{SID, SSID} = {%02x, %02x}\n", u1SID, u1SSID);
    }
*/

    return TRUE;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_PS_SetStream
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_PS_SetStream(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PS_T rPs;
    UINT8 u1Pidx;
    UINT32 u4Flags;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 4)
    {
        Printf("Usage: %s pid_index stream_id video|audio\n", aszArgv[0]);
        return -1;
    }

    u4Flags = DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_STREAM_ID | DMX_PID_FLAG_VALID;
    rPs.fgAllocateBuffer = TRUE;
    rPs.u4BufAddr = 0;
    rPs.u4BufSize = DMX_DEFAULT_ES_FIFO_SIZE;
    rPs.fgEnable = TRUE;
    u1Pidx = (UINT8)StrToInt(aszArgv[1]);
    rPs.u1StreamID = (UINT8)StrToInt(aszArgv[2]);
    rPs.u1SubstreamID = 0;
    rPs.fgEnableSsidFilter = FALSE;
    if(x_strncmp(aszArgv[3], "video", sizeof("video")) == 0)
    {
        rPs.ePidType = DMX_PID_TYPE_ES_VIDEO;
    }
    else if(x_strncmp(aszArgv[3], "audio", sizeof("audio")) == 0)
    {
        rPs.ePidType = DMX_PID_TYPE_ES_AUDIO;
    }
    else
    {
        Printf("Usage: %s pid_index stream_id video|audio\n", aszArgv[0]);
        return -1;
    }

    if (rPs.ePidType == DMX_PID_TYPE_ES_AUDIO)
    {
        UINT32 au4BufStart[2], au4BufEnd[2];

        ASSERT(NUM_AUDIO_DEVICE == 2);
        
#ifdef AUD_OLDFIFO_INTF
        if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
            &au4BufEnd[1]) != AUD_OK)
#else
        if ((AUD_GetAudFifo(0, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK) ||
            (AUD_GetAudFifo(1, &au4BufStart[1], &au4BufEnd[1]) != AUD_OK))
#endif        
        {
            Printf("Can't get audio buffers!\n");
            return -1;
        }
        if ((au4BufStart[0] == 0) || (au4BufStart[1] == 0))
        {
            Printf("Can't get audio buffers!\n");
            return -1;
        }
        rPs.u4BufAddr = au4BufStart[0];
        rPs.u4BufSize = au4BufEnd[0] - au4BufStart[0];
        rPs.fgAllocateBuffer = FALSE;
    }
    else if (rPs.ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        FBM_POOL_T* prFbmPool;

        // Allocate video buffer from FBM
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0);

        rPs.fgAllocateBuffer = FALSE;
        rPs.u4BufAddr = prFbmPool->u4Addr;
        rPs.u4BufSize = prFbmPool->u4Size;
    }

    return (DMX_PS_SetStream(u1Pidx, u4Flags, &rPs) ? 0 : -1);
}


//-----------------------------------------------------------------------------
/** _DMXCLI_PS_SetAudioType
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_PS_SetAudioType(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1Pidx;
    DMX_AUDIO_TYPE_T eAudioType = DMX_AUDIO_UNKNOWN;

    if (i4Argc != 3)
    {
        Printf("Usage: %s pidx ac3|lpcm\n", aszArgv[0]);
        return -1;
    }

    u1Pidx = (UINT8)StrToInt(aszArgv[1]);
    if (x_strncmp(aszArgv[2], "ac3", sizeof("ac3")) == 0)
    {
        eAudioType = DMX_AUDIO_AC3;
    }
    else if (x_strncmp(aszArgv[2], "lpcm", sizeof("lpcm")) == 0)
    {
        eAudioType = DMX_AUDIO_LPCM;
    }
    else
    {
        Printf("Usage: %s pidx ac3|lpcm\n", aszArgv[0]);
        return -1;
    }

    if (!DMX_PS_SetAudioType(u1Pidx, eAudioType))
    {
        Printf("Failed to set audio type!\n");
        return -1;
    }
    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_PS_EnableStream
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_PS_EnableStream(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_PS_T rPs;
    UINT8 u1Pidx;

    ASSERT(aszArgv != NULL);

    if ((i4Argc != 2) && (i4Argc != 3))
    {
        Printf("Usage: %s pid_index [on|off]\n", aszArgv[0]);
        return -1;
    }

    u1Pidx = (UINT8)StrToInt(aszArgv[1]);

    if (i4Argc == 2)  // Query
    {
        if (!DMX_PS_GetStream(u1Pidx, DMX_PID_FLAG_VALID, &rPs))
        {
            return -1;
        }
        Printf("PS at PID index %u: %s\n", u1Pidx,
                rPs.fgEnable ? "enabled" : "disabled");
        return 0;
    }

    if (x_strcmp(aszArgv[2], "on") == 0)
    {
        rPs.fgEnable = TRUE;
    }
    else if (x_strcmp(aszArgv[2], "off") == 0)
    {
        rPs.fgEnable = FALSE;
    }
    else
    {
        Printf("Usage: %s pid_index [on|off]\n", aszArgv[0]);
        return -1;
    }

    if (!DMX_PS_SetStream(u1Pidx, DMX_PID_FLAG_VALID, &rPs))
    {
        return -1;
    }
    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_PS_FreeStream
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_PS_FreeStream(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1Pidx;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 2)
    {
        Printf("Usage: %s pid_index\n", aszArgv[0]);
        return -1;
    }

    u1Pidx = (UINT8)StrToInt(aszArgv[1]);
    if (!DMX_PS_FreeStream(u1Pidx))
    {
        Printf("Failed to free PS buffer!\n");
        return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_PS_DetectSID
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_PS_DetectSID(INT32 i4Argc, const CHAR** aszArgv)
{
    BOOL fgDetectSID = FALSE;
    UINT16 u2Pairs = 100;
    UINT32 u4Len = 0x1000;

    ASSERT(aszArgv != NULL);

    if ((i4Argc != 2) && (i4Argc != 3) && (i4Argc != 4))
    {
        Printf("Usage: %s on/off [pairs] [len]\n", aszArgv[0]);
        return -1;
    }

    if (x_strcmp(aszArgv[1], "on") == 0)
    {
        fgDetectSID = TRUE;
    }
    else if (x_strcmp(aszArgv[1], "off") == 0)
    {
        fgDetectSID = FALSE;
    }
    else
    {
        Printf("Usage: %s on/off [pairs] [len]\n", aszArgv[0]);
        return -1;
    }

    if (i4Argc > 2)
    {
        u2Pairs = (UINT16) StrToInt(aszArgv[2]);
    }

    if (i4Argc > 3)
    {
        u4Len = (UINT32) StrToInt(aszArgv[3]);
    }

    if (!DMX_PS_DetectStreamID(fgDetectSID, u2Pairs, u4Len))
    {
        Printf("Failed to set to the preparsing mode!\n");
        return -1;
    }

    if (fgDetectSID)
    {
        DMX_PS_T rPs;
        rPs.pfnNotify = _Dmx_PS_PreparsingCallback;
        rPs.pvNotifyTag = NULL;
        if (!DMX_PS_SetStream(0, DMX_PID_FLAG_CALLBACK, &rPs))
        {
            Printf("Failed to register Preparsing callback\n");
            return -1;
        }
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_ES_SetPid
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_ES_SetPid(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_ES_T rEs;
    UINT8 u1Pidx;
    UINT32 u4Flags;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 3)
    {
        Printf("Usage: %s pid_index video|audio\n", aszArgv[0]);
        return -1;
    }

    u4Flags = DMX_PID_FLAG_BUFFER | DMX_PID_FLAG_VALID;
    rEs.fgEnable = TRUE;
    rEs.fgAllocateBuffer = TRUE;
    rEs.u4BufAddr = 0;
    rEs.u4BufSize = DMX_DEFAULT_ES_FIFO_SIZE;
    u1Pidx = (UINT8)StrToInt(aszArgv[1]);
    if(x_strncmp(aszArgv[2], "video", sizeof("video")) == 0)
    {
        rEs.ePidType = DMX_PID_TYPE_ES_VIDEO;
    }
    else if(x_strncmp(aszArgv[2], "audio", sizeof("audio")) == 0)
    {
        rEs.ePidType = DMX_PID_TYPE_ES_AUDIO;
    }
    else
    {
        Printf("Usage: %s pid_index video|audio\n", aszArgv[0]);
        return -1;
    }

    if (rEs.ePidType == DMX_PID_TYPE_ES_AUDIO)
    {
        UINT32 au4BufStart[2], au4BufEnd[2];

        ASSERT(NUM_AUDIO_DEVICE == 2);
#ifdef AUD_OLDFIFO_INTF
        if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
            &au4BufEnd[1]) != AUD_OK)
#else
        if ((AUD_GetAudFifo(0, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK) ||
            (AUD_GetAudFifo(1, &au4BufStart[1], &au4BufEnd[1]) != AUD_OK))
#endif
        {
            Printf("Can't get audio buffers!\n");
            return -1;
        }
        if ((au4BufStart[0] == 0) || (au4BufStart[1] == 0))
        {
            Printf("Can't get audio buffers!\n");
            return -1;
        }
        rEs.u4BufAddr = au4BufStart[0];
        rEs.u4BufSize = au4BufEnd[0] - au4BufStart[0];
        rEs.fgAllocateBuffer = FALSE;
    }
    else if (rEs.ePidType == DMX_PID_TYPE_ES_VIDEO)
    {
        FBM_POOL_T* prFbmPool;

        // Allocate video buffer from FBM
        prFbmPool = FBM_GetPoolInfo((UINT8)FBM_POOL_TYPE_DMX);
        ASSERT(prFbmPool != NULL);
        ASSERT(prFbmPool->u4Addr != 0);

        rEs.fgAllocateBuffer = FALSE;
        rEs.u4BufAddr = prFbmPool->u4Addr;
        rEs.u4BufSize = prFbmPool->u4Size;
    }

    return (DMX_ES_SetPid(u1Pidx, u4Flags, &rEs) ? 0 : -1);
}


//-----------------------------------------------------------------------------
/** _DMXCLI_ES_EnablePid
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_ES_EnablePid(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_ES_T rEs;
    UINT8 u1Pidx;

    ASSERT(aszArgv != NULL);

    if ((i4Argc != 2) && (i4Argc != 3))
    {
        Printf("Usage: %s pid_index [on|off]\n", aszArgv[0]);
        return -1;
    }

    u1Pidx = (UINT8)StrToInt(aszArgv[1]);

    if (i4Argc == 2)  // Query
    {
        if (!DMX_ES_GetPid(u1Pidx, DMX_PID_FLAG_VALID, &rEs))
        {
            return -1;
        }
        Printf("ES at PID index %u: %s\n", u1Pidx,
                rEs.fgEnable ? "enabled" : "disabled");
        return 0;
    }

    if (x_strcmp(aszArgv[2], "on") == 0)
    {
        rEs.fgEnable = TRUE;
    }
    else if (x_strcmp(aszArgv[2], "off") == 0)
    {
        rEs.fgEnable = FALSE;
    }
    else
    {
        return -1;
    }

    if (!DMX_ES_SetPid(u1Pidx, DMX_PID_FLAG_VALID, &rEs))
    {
        return -1;
    }
    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_ES_FreePid
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_ES_FreePid(INT32 i4Argc, const CHAR** aszArgv)
{
    UINT8 u1Pidx;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 2)
    {
        Printf("Usage: %s pid_index\n", aszArgv[0]);
        return -1;
    }

    u1Pidx = (UINT8)StrToInt(aszArgv[1]);
    if (!DMX_ES_FreePid(u1Pidx))
    {
        Printf("Failed to free ES buffer!\n");
        return -1;
    }

    return 0;
}



typedef struct
{
    UINT32 u4InDataAddr;    // original address of the input data
    UINT32 u4InDataSize;    // input data size
    UINT32 u4SliceSize;     // send a slice of data each time
    UINT32 u4AccuBytes;     // accumulated count sent to DDI
} DDI_INPUT_INFO_T;


DDI_INPUT_INFO_T _rInputInfo;
static BOOL (*_pfnDdiFillData)(DDI_EVENT_CODE_T) = NULL;


//-----------------------------------------------------------------------------
/** _DMXCLI_DDI_Init
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_DDI_Init(INT32 i4Argc, const CHAR** aszArgv)
{
    if (i4Argc != 1)
    {
        Printf("Usage: %s\n", aszArgv[0]);
        return -1;
    }

    if(!DMX_DDI_Init())
    {
        Printf("Failed to initialize DDI!\n");
        return -1;
    }

    Printf("OK\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_DDI_Rate
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_DDI_Rate(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_DDI_T rDDI;

    ASSERT(aszArgv != NULL);

    if (i4Argc != 3)
    {
        Printf("Usage: %s RateM RateN\n", aszArgv[0]);
        return -1;
    }

    rDDI.u4RateM = StrToInt(aszArgv[1]);
    rDDI.u4RateN = StrToInt(aszArgv[2]);

    if(!DMX_DDI_Set(DMX_DDI_FLAG_RATE, &rDDI))
    {
        Printf("Failed to set DDI properties!\n");
        return -1;
    }

    Printf("DDI date rate is set successfully.\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_DDI_SetMode
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_DDI_SetMode(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_DDI_T rDDI;

    if (i4Argc != 2)
    {
        Printf("Usage: %s single|stream\n", aszArgv[0]);
    }

    if (x_strcmp(aszArgv[1], "single") == 0)
    {
        rDDI.eMode = DMX_DDI_MODE_SINGLE;
    }
    else if (x_strcmp(aszArgv[1], "stream") == 0)
    {
        rDDI.eMode = DMX_DDI_MODE_STREAM;
    }
    else
    {
        Printf("Invalid argument!\n");
        return -1;
    }

    if(!DMX_DDI_Set(DMX_DDI_FLAG_MODE, &rDDI))
    {
        Printf("Failed to set DDI properties!\n");
        return -1;
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_DDI_SetBuf
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_DDI_SetBuf(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_DDI_T rDDI;
    UINT32 u4Addr , u4Size, u4Threshold, u4Flags;

    ASSERT(aszArgv != NULL);

    if(i4Argc < 2)
    {
        Printf("Usage: %s addr|0 [buf_size] [threshold]\n", aszArgv[0]);
        return -1;
    }

    u4Addr = StrToInt(aszArgv[1]);

    u4Size = 0x0;
    if (i4Argc > 2)
    {
        u4Size = StrToInt(aszArgv[2]);
        u4Size = (u4Size == 0) ? 0x100000 : u4Size;
    }

    u4Threshold = 0;
    if (i4Argc > 3)
    {
        u4Threshold = StrToInt(aszArgv[3]);
    }

    rDDI.u4BufAddr = u4Addr;
    rDDI.fgAllocBuf = (u4Addr == 0) ? TRUE : FALSE;
    rDDI.u4BufSize = u4Size;
    rDDI.u4Threshold = u4Threshold;
    rDDI.pfnDDINotify = _pfnDdiFillData;

    u4Flags = DMX_DDI_FLAG_CALLBACK;
    if (rDDI.fgAllocBuf)
    {
        u4Flags |= DMX_DDI_FLAG_ALLOCBUF;
    }

    if(!DMX_DDI_Set(u4Flags, &rDDI))
    {
        Printf("Failed to set DDI properties!\n");
        return -1;
    }

    Printf("Successful in setting DDI properties!\n");
    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_DDI_FlushBuf
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_DDI_FlushBuf(INT32 i4Argc, const CHAR** aszArgv)
{
    if(!DMX_DDI_FlushBuf())
    {
        Printf("Failed to flush DDI buffer!\n");
        return -1;
    }

    Printf("OK\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_DDI_FreeBuf
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_DDI_FreeBuf(INT32 i4Argc, const CHAR** aszArgv)
{
    if(!DMX_DDI_Free())
    {
        Printf("Failed to Reset DDI and Free DDI buffer!\n");
        return -1;
    }

    Printf("OK\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_DDI_Query
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_DDI_Query(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_DDI_T rDDI;
    UINT32 u4Flags;

    if (i4Argc != 1)
    {
        Printf("Usage: %s\n", aszArgv[0]);
        return -1;
    }

    u4Flags = DMX_DDI_FLAG_DEBUG_INFO | DMX_DDI_FLAG_MODE;
    if (!DMX_DDI_Get(u4Flags, &rDDI))
    {
        Printf("Failed to get DDI properties!\n");
        return -1;
    }

    Printf("Buf_start: 0x%08x, Buf_end(+1): 0x%08x\n",
        rDDI.rDebugInfo.u4BufStart, rDDI.rDebugInfo.u4BufEnd);
    Printf("RP: 0x%08x, WP: 0x%08x, RIP: 0x%08x, AP: 0x%08x\n",
        rDDI.rDebugInfo.u4Rp,  rDDI.rDebugInfo.u4Wp, rDDI.rDebugInfo.u4Rip,
        rDDI.rDebugInfo.u4Ap);

    Printf("State: ");
    if (rDDI.rDebugInfo.eState == DMX_DDI_STOP)
    {
        Printf("stopped");
    }
    else if (rDDI.rDebugInfo.eState == DMX_DDI_PLAY)
    {
        Printf("started");
    }
    else
    {
        Printf("unknown");
    }

    Printf(", Mode: ");
    if (rDDI.eMode == DMX_DDI_MODE_STREAM)
    {
        Printf("stream");
    }
    else if (rDDI.eMode == DMX_DDI_MODE_SINGLE)
    {
        Printf("single");
    }
    else
    {
        Printf("unknown");
    }

    Printf(", Transferred: %u\n", rDDI.rDebugInfo.u4TransferredSize);

    //Printf("Minimal audio buffer size: %u\n", DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_AUDIO));
    //Printf("Minimal video buffer size: %u\n", DMX_GetMinFreeBufferSize(DMX_PID_TYPE_ES_VIDEO));

    return 0;
}


//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  none
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_DDI_PowerDown(INT32 i4Argc, const CHAR** aszArgv)
{
    if (i4Argc != 1)
    {
        Printf("Usage: %s\n", aszArgv[0]);
        return -1;
    }

    if (!DMX_DDI_PowerDown())
    {
        return -1;
    }
    return 0;
}

#ifndef CC_DMX_ATSC_MPEG_ONLY
//-----------------------------------------------------------------------------
/** _DMXCLI_MMInit
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_MMInit(INT32 i4Argc, const CHAR** aszArgv)
{
    if(!DMX_MM_Init())
    {
        Printf("Fail\n");
        return -1;
    }

    Printf("OK\n");

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_SetMultiMedia
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_MMSetMultiMedia(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_MM_T rMM;
    UINT8 u1Idx, u1DevID;
    UINT32 au4BufStart[2], au4BufEnd[2];

    if(i4Argc != 3)
    {
        Printf("%s [idx] [video|audio]\n", aszArgv[0]);
        return -1;
    }

    u1Idx = StrToInt(aszArgv[1]);
    if(x_strcmp(aszArgv[2], "video") == 0)
    {
        rMM.fgAllocBuf = TRUE;
        rMM.u4BufSize = 0x100000;    // 1MB
        rMM.ePidType = DMX_PID_TYPE_ES_VIDEO;
        rMM.pfnCallback = NULL;
        rMM.fgEnable = TRUE;

        if(!DMX_MM_Set(u1Idx, DMX_MM_FLAG_ALL, &rMM))
        {
            return -1;
        }
    }
    else if(x_strcmp(aszArgv[2], "audio") == 0)
    {
        ASSERT(NUM_AUDIO_DEVICE == 2);

        u1DevID = 0x0;
/*
        if (rPid.u1DeviceId >= NUM_AUDIO_DEVICE)
        {
            Printf("Audio device ID: 0 - %u\n", NUM_AUDIO_DEVICE - 1);
            return -1;
        }
*/
#ifdef AUD_OLDFIFO_INTF
        if (AUD_GetAudFifo(&au4BufStart[0], &au4BufEnd[0], &au4BufStart[1],
            &au4BufEnd[1]) != AUD_OK)
#else
        if ((AUD_GetAudFifo(0, &au4BufStart[0], &au4BufEnd[0]) != AUD_OK) ||
            (AUD_GetAudFifo(1, &au4BufStart[1], &au4BufEnd[1]) != AUD_OK))
#endif
        {
            Printf("Can't get audio buffers!\n");
            return -1;
        }
        if ((au4BufStart[0] == 0) || (au4BufStart[1] == 0))
        {
            Printf("Can't get audio buffers!\n");
            return -1;
        }

        rMM.ePidType = DMX_PID_TYPE_ES_AUDIO;
        rMM.fgAllocBuf = FALSE;
        rMM.u4BufAddr = au4BufStart[u1DevID];
        rMM.u4BufSize = au4BufEnd[u1DevID] - au4BufStart[u1DevID];
    }
    else
    {
        Printf("%s [idx] [VIDEO|AUDIO]\n", aszArgv[0]);
        return -1;
    }

    Printf("OK\n");

    return 0;
}


//--------------------------------------------------------------------------
/** _DMXCLI_MMMoveData
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_MMMoveData(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_MM_DATA_T rData;
    DMX_MM_T rMM;
    UINT32 u4BufStart, u4BufEnd, u4BufSize, u4Offset, u4Addr, u4Size, u4SentSize;
    UINT32 i, u4OrgSize;
    UINT8 u1Idx;

    if(i4Argc != 6)
    {
        Printf("%s [idx] [buf_start] [buf_size] [offset] [size]\n", aszArgv[0]);
        return -1;
    }

    u1Idx = StrToInt(aszArgv[1]);
    u4BufStart = StrToInt(aszArgv[2]);
    u4BufSize = StrToInt(aszArgv[3]);
    u4Offset = StrToInt(aszArgv[4]);
    u4Size = StrToInt(aszArgv[5]);

    u4Addr = u4Offset + u4BufStart;
    u4BufEnd = u4BufStart + u4BufSize;
    rData.u4BufStart = u4BufStart;
    rData.u4BufEnd = u4BufEnd;

    if((u4Addr<u4BufStart) || (u4Addr>=u4BufEnd))
    {
        Printf("Offset is not in buffer range\n");
        return -1;
    }

    if(!DMX_MM_Get(u1Idx, DMX_MM_FLAG_BUF, &rMM))
    {
        return -1;
    }

    u4OrgSize = u4Size;
    while(u4Size > 0)
    {
        u4SentSize = u4Size;
        if(u4Size > 37501)  // 188 * 200
        {
            u4SentSize = 37501;///37600; /37501;
        }

        rData.u4StartAddr = u4Addr;
        rData.u4FrameSize = u4SentSize;

        if(!DMX_MM_MoveData(u1Idx, &rData, 10000))
        {
            Printf("Fail\n");
            return -1;
        }
        u4Size -= u4SentSize;
        u4Addr += u4SentSize;
        if(u4Addr >= u4BufEnd)
        {
            u4Addr -= u4BufSize;
        }
    }

    // Compare result
    u4Size = MIN((rMM.u4BufAddr + rMM.u4BufSize - rMM.u4Wp), u4OrgSize);
    for(i=0; i<u4Size; i+=4)
    {
        if((*(UINT32*)(rMM.u4Wp + i)) != (*(UINT32*)(u4BufStart + u4Offset + i)))
        {
            Printf("Compare error\n");
            return -1;
        }
    }
    u4OrgSize -= u4Size;
    u4BufStart += u4Size;
    if(u4OrgSize > 0)
    {
        for(i=0; i<u4OrgSize; i+=4)
        {
            if((*(UINT32*)(rMM.u4BufAddr + i)) != (*(UINT32*)(u4BufStart + u4Offset + i)))
            {
                Printf("Compare error\n");
                return -1;
            }
        }
    }

    Printf("OK\n");

    return 0;
}


//--------------------------------------------------------------------------
/** _DMXCLI_MMQuery
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_MMQuery(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_MM_T rMM;
    UINT32 i;

    for(i=0; i<DMX_MM_STRUCT_NUM; i++)
    {
        if(!DMX_MM_Get(i, DMX_MM_FLAG_ALL, &rMM))
        {
            return -1;
        }

        if(rMM.fgEnable)
        {
            switch(rMM.ePidType)
            {
            case DMX_PID_TYPE_ES_VIDEO:
                Printf("Video, ");
                break;

            case DMX_PID_TYPE_ES_AUDIO:
                Printf("Audio, ");
                break;

            default:
                Printf("Other, ");
                break;
            }
            Printf("BufStart:0x%x, BufEnd:0x%x, Wp:0x%x, Rp:0x%x\n",
                rMM.u4BufAddr, rMM.u4BufAddr + rMM.u4BufSize, rMM.u4Wp, rMM.u4Rp);
        }
    }

    return 0;
}


//-----------------------------------------------------------------------------
/** _Init_MM_WMVTransfer
 */
//-----------------------------------------------------------------------------
static INT32 _Init_MM_WMVTransfer(UINT32 *pu4BufStart, UINT32 *pu4BufSize)
{
    char* aszArray[7];
    INT32 i4Ret;
    DMX_DDI_T rDDI;

    Printf("Initialize DMX...\n");
    aszArray[0] = "i";
    aszArray[1] = 0;        // NULL
    i4Ret = _DMXCLI_Init(1, (const CHAR**)aszArray);
    if (i4Ret)
    {
        return i4Ret;
    }

    Printf("Initialize DDI...\n");
    aszArray[0] = "ddi.i";
    aszArray[1] = 0;
    i4Ret = _DMXCLI_DDI_Init(1, (const CHAR**)aszArray);
    if (i4Ret)
    {
        return i4Ret;
    }

    // Printf("Select \"mm\" as the input type...\n");
    aszArray[0] = "si";
    aszArray[1] = "mm";
    aszArray[2] = 0;
    i4Ret = _DMXCLI_SelInputType(2, (const CHAR**)aszArray);
    if (i4Ret)
    {
        return i4Ret;
    }

    // Set DDI to SINGLE mode and 8 MB/sec, ie (N/M Mbps).
    // When we issue the CLI command "dmx.si mm", the DDI is set to 4MB/sec.
    // Printf("Set DDI to SINGLE mode and 8MB/sec...\n");
    rDDI.eMode = DMX_DDI_MODE_SINGLE;
    rDDI.u4RateN = 64;
    rDDI.u4RateM = 1;
    if(!DMX_DDI_Set(DMX_DDI_FLAG_MODE | DMX_DDI_FLAG_RATE, &rDDI))
    {
        LOG(3, "Failed to set DDI properties!\n");
        return -1;
    }

    // Set Video Type
    // Printf("Set \"wmv7\" as the video type...\n");
    aszArray[0] = "svt";
    aszArray[1] = "wmv7";
    aszArray[2] = 0;
    i4Ret = _DMXCLI_SetVideoType(2, (const CHAR**)aszArray);
    if (i4Ret)
    {
        return i4Ret;
    }

    // Printf("Allocate a buffer for storing source data...\n");
    *pu4BufStart = VIRTUAL((UINT32)BSP_AllocAlignedDmaMemory(*pu4BufSize, 16));
    if(*pu4BufStart == 0)
    {
        Printf("Buffer allocation failed!\n");
        return -1;
    }

    ///*************************************************************************
    //
    // Set "to decoder" if you want to see the decoder in action.
    //
    Printf("Set \"to decoder\"...\n");
    aszArray[0] = "std";
    aszArray[1] = "on";
    aszArray[2] = "off";
    aszArray[3] = 0;
    i4Ret = _DMXCLI_SetToDecoder(3, (const CHAR**)aszArray);
    if (i4Ret)
    {
        return i4Ret;
    }
    //*************************************************************************/

    return 0;
}


//-----------------------------------------------------------------------------
/** _DMXCLI_MM_WMVTransfer
 */
//-----------------------------------------------------------------------------
static INT32 _DMXCLI_MM_WMVTransfer(INT32 i4Argc, const CHAR** aszArgv)
{
    DMX_MM_DATA_T rData;
    DMX_MM_T rMM;
    UINT8 u1Pidx;
    UINT32 u4BufStart, u4BufEnd, u4BufSize, u4Offset, u4StartAddr;
    UINT32 u4TotalLen, u4RemainingLen, u4SegmentLen;
    UINT32 i, u4CmpSize;
    char* aszArray[2];

    if (i4Argc != 4)
    {
        Printf("%s idx offset total_len\n", aszArgv[0]);
        return -1;
    }

    u4BufSize = 0x2000B0;       // source buffer size must be 16*n (for DDI)
    if (_Init_MM_WMVTransfer(&u4BufStart, &u4BufSize))
    {
        return -1;
    }

    // Do NOT change the value of u4BufStart because it is used in BSP_Free()!
    u1Pidx     = (UINT8)  StrToInt(aszArgv[1]);
    u4Offset   = (UINT32) StrToInt(aszArgv[2]);
    u4TotalLen = (UINT32) StrToInt(aszArgv[3]);

    u4RemainingLen = u4TotalLen;
    u4StartAddr = u4BufStart + u4Offset;
    u4BufEnd = u4BufStart + u4BufSize;
    rData.u4BufStart = u4BufStart;
    rData.u4BufEnd = u4BufEnd;

    /*************************************************************************
    ****** This is checked in DDI, and no need to check it again here. *******
    if (u4BufSize < u4TotalLen)
    {
        Printf("Buffer size is less than the size of data to be transferred.\n");
        return -1;
    }
    *************************************************************************/

    rMM.fgAllocBuf = TRUE;
    rMM.u4BufSize = 0x300000;  // 3MB (Must > the size of source data)
    rMM.ePidType = DMX_PID_TYPE_ES_VIDEO;
    rMM.fgSearchStartCode = FALSE;
    rMM.pfnCallback = NULL;
    rMM.fgEnable = TRUE;
    if (!DMX_MM_Set(u1Pidx, DMX_MM_FLAG_ALL, &rMM))
    {
        BSP_FreeAlignedDmaMemory((void*)u4BufStart);   // Return value is not checked.
        return -1;
    }
    if (!DMX_MM_Get(u1Pidx, DMX_PID_FLAG_BUFFER, &rMM))  // Get buffer address
    {
        BSP_FreeAlignedDmaMemory((void*)u4BufStart);   // Return value is not checked.
        DMX_MM_Free(u1Pidx);                    // Return value is not checked.
        return -1;
    }

    while (u4RemainingLen > 0)
    {
        rData.fgFrameHead = ! random(4);
        // 256KB < u4SegmentLen <= 40KB
        u4SegmentLen = random(262144);
        u4SegmentLen = (u4SegmentLen < 40960 ) ? 40960 : u4SegmentLen;
        if (u4SegmentLen >= u4RemainingLen)
        {
            u4SegmentLen = u4RemainingLen;
            rData.fgFrameHead = TRUE;
        }

        rData.u4StartAddr = u4StartAddr;
        rData.u4FrameSize = u4SegmentLen;
        if (!DMX_MM_MoveData(u1Pidx, &rData, 10000))
        {
            Printf("%s:%d:Failed!\n", __FILE__, __LINE__);
            BSP_FreeAlignedDmaMemory((void*)u4BufStart);   // Return value is not checked.
            DMX_MM_Free(u1Pidx);                    // Return value is not checked.
            return -1;
        }
        u4StartAddr += u4SegmentLen;
        u4RemainingLen -= u4SegmentLen;
    }

    // The data to be compared with each other may not reside at 4-byte
    // boundary.  Therefore, one byte of data is compared each time.
    // This is slow, but at least correct.
    u4CmpSize = MIN((rMM.u4BufAddr + rMM.u4BufSize - rMM.u4Wp), u4TotalLen);
    Printf("Begin data comparison (length = 0x%08x)...\n", u4CmpSize);
    for (i = 0; i < u4CmpSize; i++)
    {
        if ((*(UINT8*)(rMM.u4Wp + i)) != (*(UINT8*)(u4BufStart + u4Offset + i)))
        {
            Printf("Data comparison failed!\n");
            BSP_FreeAlignedDmaMemory((void*)u4BufStart);   // Return value is not checked.
            DMX_MM_Free(u1Pidx);                    // Return value is not checked.
            return -1;
        }
    }
    u4TotalLen -= u4CmpSize;
    if (u4TotalLen > 0)
    {
        for (i = 0; i < u4TotalLen; i++)
        {
            if ((*(UINT8*)(rMM.u4BufAddr + i)) !=
                (*(UINT8*)(u4BufStart + u4CmpSize + u4Offset + i)))
            {
                Printf("Data comparison failed!\n");
                BSP_FreeAlignedDmaMemory((void*)u4BufStart);   // Return value is not checked.
                DMX_MM_Free(u1Pidx);                    // Return value is not checked.
                return -1;
            }
        }
    }

    aszArray[0] = "q";
    aszArray[1] = 0;
    _DMXCLI_Query(1, (const CHAR**)aszArray);

    Printf("Data comparison passed!\n");
    BSP_FreeAlignedDmaMemory((void*)u4BufStart);   // Return value is not checked.
    DMX_MM_Free(u1Pidx);                    // Return value is not checked.
    return 0;
}

#endif  // CC_DMX_ATSC_MPEG_ONLY
#endif  // ENABLE_MULTIMEDIA
#endif  // CC_DMX_DEBUG

#ifdef  CC_DMX_DEBUG
#ifdef ENABLE_MULTIMEDIA
#ifndef CC_DMX_ATSC_MPEG_ONLY
//-----------------------------------------------------------------------------
static CLI_EXEC_T _arDmxMMCmdTbl[] =
{
    {
        "init", "i", _DMXCLI_MMInit, NULL, "i", CLI_SUPERVISOR
    },

    {
        "setmm", "smm", _DMXCLI_MMSetMultiMedia, NULL, "smm [idx]", CLI_SUPERVISOR
    },

    {
        "movedata", "mv", _DMXCLI_MMMoveData, NULL, "", CLI_SUPERVISOR
    },

    {
        "wmv", "wmv", _DMXCLI_MM_WMVTransfer, NULL, "wmv idx offset total_len", CLI_SUPERVISOR
    },

    {
        "query", "q", _DMXCLI_MMQuery, NULL, "", CLI_SUPERVISOR
    },

    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
};
#endif  // CC_DMX_ATSC_MPEG_ONLY

//-----------------------------------------------------------------------------
static CLI_EXEC_T _arDmxPSCmdTbl[] =
{
    {
        "setstream", "ss", _DMXCLI_PS_SetStream, NULL, "ss pid_index stream_id video|audio", CLI_SUPERVISOR
    },

    {
        "setaudiotype", "sat", _DMXCLI_PS_SetAudioType, NULL, "sat pidx ac3|lpcm", CLI_SUPERVISOR
    },

    {
        "freestream", "fs", _DMXCLI_PS_FreeStream, NULL, "fs pid_index", CLI_SUPERVISOR
    },

    {
        "enablestream", "es", _DMXCLI_PS_EnableStream, NULL, "es pid_index [on|off]", CLI_SUPERVISOR
    },

    {
        "detectsid", "dsid", _DMXCLI_PS_DetectSID, NULL, "dsid on/off [pairs] [len]", CLI_SUPERVISOR
    },

    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
};


//-----------------------------------------------------------------------------
static CLI_EXEC_T _arDmxESCmdTbl[] =
{
    {
        "setpid", "sp", _DMXCLI_ES_SetPid, NULL, "sp pid_index video|audio", CLI_SUPERVISOR
    },

    {
        "freepid", "fp", _DMXCLI_ES_FreePid, NULL, "fp pid_index", CLI_SUPERVISOR
    },

    {
        "ep", "ep", _DMXCLI_ES_EnablePid, NULL, "ep pid_index [on|off]", CLI_SUPERVISOR
    },

    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
};


//-----------------------------------------------------------------------------
static CLI_EXEC_T _arDmxDDICmdTbl[] =
{
    {
        "init", "i", _DMXCLI_DDI_Init, NULL, "init", CLI_SUPERVISOR
    },

    {
        "rate", "rate", _DMXCLI_DDI_Rate, NULL, "rate RateM RateN", CLI_SUPERVISOR
    },

    {
        "setmode", "sm", _DMXCLI_DDI_SetMode, NULL, "sm single|stream", CLI_SUPERVISOR
    },

    {
        "setbuf", "sbuf", _DMXCLI_DDI_SetBuf, NULL, "sbuf addr|0 [buf_size] [threshold]", CLI_SUPERVISOR
    },

    {
        "flushbuf", "flbuf", _DMXCLI_DDI_FlushBuf, NULL, "flbuf", CLI_SUPERVISOR
    },

    {
        "freebuf", "frbuf", _DMXCLI_DDI_FreeBuf, NULL, "frbuf", CLI_SUPERVISOR
    },

    {
        "query", "q", _DMXCLI_DDI_Query, NULL, "query", CLI_SUPERVISOR
    },

    {
        "powerdown", "pdwn", _DMXCLI_DDI_PowerDown, NULL, "powerdown", CLI_SUPERVISOR
    },

    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }
};
#endif  // ENABLE_MULTIMEDIA
#endif // CC_DMX_DEBUG

//-----------------------------------------------------------------------------
static CLI_EXEC_T _arDmxCmdTbl[] =
{
    {
        "init", "i", _DMXCLI_Init, NULL, "", CLI_SUPERVISOR
    },

    {
        "setfe", "sfe", _DMXCLI_SetFrontEnd, NULL, "", CLI_SUPERVISOR
    },

    {
        "setframer", "sframer", _DMXCLI_SetFramer, NULL, "", CLI_SUPERVISOR
    },

    {
        "resetframer", "rframer", _DMXCLI_ResetFramer, NULL, "", CLI_SUPERVISOR
    },

    {
        "settuner", "st", _DMXCLI_SetTuner, NULL, "", CLI_SUPERVISOR
    },

    {
        "setpid", "sp", _DMXCLI_SetPid, NULL, "", CLI_SUPERVISOR
    },

    {
        "freepid", "fp", _DMXCLI_FreePid, NULL, "", CLI_SUPERVISOR
    },

    {
        "getpid", "gp", _DMXCLI_GetPid, NULL, "", CLI_SUPERVISOR
    },

    {
        "enablepid", "ep", _DMXCLI_EnablePid, NULL, "", CLI_SUPERVISOR
    },

    {
        "setpcr", "spcr", _DMXCLI_SetPcr, NULL, "", CLI_SUPERVISOR
    },

    {
        "setpsipid", "spp", _DMXCLI_SetPsiPid, NULL, "", CLI_SUPERVISOR
    },

    {
        "setfilter", "sf", _DMXCLI_SetFilter, NULL, "", CLI_SUPERVISOR
    },

    {
        "getfilter", "gf", _DMXCLI_GetFilter, NULL, "", CLI_SUPERVISOR
    },

    {
        "getgenericfilter", "ggf", _DMXCLI_GetGenericFilter, NULL, "", CLI_SUPERVISOR
    },

    {
        "settodecoder", "std", _DMXCLI_SetToDecoder, NULL, "", CLI_SUPERVISOR
    },

    {
        "getcounters", "gc", _DMXCLI_GetCounters, NULL, "", CLI_SUPERVISOR
    },

    {
        "resetcounters", "rc", _DMXCLI_ResetCounters, NULL, "", CLI_SUPERVISOR
    },

    {
        "query", "q", _DMXCLI_Query, NULL, "", CLI_GUEST
    },

#ifdef CC_MT5387
    {
        "memoryusage", "mem", _DMXCLI_MemoryUsage, NULL, "", CLI_SUPERVISOR
    },
#endif // CC_MT5387

    {
        "packet", "pkt", _DMXCLI_RcvPktCount, NULL, "", CLI_GUEST
    },

#ifdef CC_MT5363        
    {
        "measure", "m", _DMXCLI_Measure, NULL, "", CLI_SUPERVISOR
    },
#endif // CC_MT5387

    {
        "version", "ver", _DMXCLI_Version, NULL, "Get version", CLI_SUPERVISOR
    },

    {
        "power", "power", _DMXCLI_Power, NULL, "Set power on off", CLI_SUPERVISOR
    },

    {
        "CheckAW", "aw", _DMXCLI_CheckAW, NULL, "Check AW support", CLI_GUEST
    },

    {
        "setpsihandler", "sph", _DMXCLI_SetPsiPidCallback, NULL, "", CLI_SUPERVISOR
    },

    {
        "capture", "cap", _DMXCLI_Capture, NULL, "Capture test", CLI_SUPERVISOR
    },

    {
        "capturebuf", "cbuf", _DMXCLI_CaptureAllocBuf, NULL, "cbuf [size]", CLI_SUPERVISOR
    },

    {
        "captureinfo", "cinfo", _DMXCLI_CaptureInfo, NULL, "cinfo", CLI_SUPERVISOR
    },

    {
        "cccaptureinit", "ccci", _DMXCLI_ChangeChannelCaptureInit, NULL, "CC capture init", CLI_SUPERVISOR
    },

    {
        "cccapture", "ccc", _DMXCLI_ChangeChannelCapture, NULL, "CC Capture event", CLI_SUPERVISOR
    },
    
    {
        "errlevel", "el", _DMXCLI_SetErrLevel, NULL, "el", CLI_SUPERVISOR
    },

    {
        "scrtype", "scrt", _DMXCLI_SetScrambleScheme, NULL, "scrt", CLI_SUPERVISOR
    },       

#ifndef CC_DMX_ATSC_MPEG_ONLY
    {
        "setvideotype", "svt", _DMXCLI_SetVideoType, NULL, "", CLI_SUPERVISOR
    },
#endif // CC_DMX_ATSC_MPEG_ONLY        

#ifdef TIME_SHIFT_SUPPORT
    {
        "timeshiftq", "tsq", _DMXCLI_TimeShiftQuery, NULL, "tsq", CLI_GUEST
    },

    {
        "timeshiftresetc", "tsrc", _DMXCLI_TimeShiftResetCounter, NULL, "tsrc", CLI_SUPERVISOR
    },
#endif  // TIME_SHIFT_SUPPORT

#ifdef CC_DMX_TS_LOOPBACK
    {
        "loopback", "lp", _DMXCLI_Loopback, NULL, "lp", CLI_SUPERVISOR
    },
#endif // CC_DMX_TS_LOOPBACK

#ifdef CC_DMX_DEBUG
    {
        "selinput", "si", _DMXCLI_SelInputType, NULL, "", CLI_SUPERVISOR
    },

#ifdef DMX_MEASURE_PSI_TIME
    {
        "psigettime", "psigt", _DMXCLI_PSIGetMaxTime, NULL, "", CLI_SUPERVISOR
    },

    {
        "psiresettime", "psirt", _DMXCLI_PSIResetMaxTime, NULL, "", CLI_SUPERVISOR
    },
#endif  // DMX_MEASURE_PSI_TIME

    {
        "settopvr", "stpvr", _DMXCLI_SetToPvr, NULL, "", CLI_SUPERVISOR
    },

    {
        "settoup", "stup", _DMXCLI_SetToFTuP, NULL, "", CLI_SUPERVISOR
    },

    {
        "setdebugpicturefinder", "sdpf", _DMXCLI_SetDebugPictureFinder, NULL, "", CLI_SUPERVISOR
    },

    {
        "setbypassmode", "sb", _DMXCLI_SetBypassMode, NULL, "", CLI_SUPERVISOR
    },

    {
        "seterrorhandling", "seh", _DMXCLI_SetErrorHandling, NULL, "", CLI_SUPERVISOR
    },

    {
        "setdeskey", "sk", _DMXCLI_SetDesKey, NULL, "", CLI_SUPERVISOR
    },

    {
        "setdesiv", "sdiv", _DMXCLI_SetDesIV, NULL, "", CLI_SUPERVISOR
    },

    {
        "setcaoutput", "sco", _DMXCLI_SetCaOutput, NULL, "", CLI_SUPERVISOR
    },

    {
        "setpidkeyidx", "skeyidx", _CmdSetPidKeyIdx, NULL, "skeyidx [pidx] [key_idx]", CLI_SUPERVISOR
    },

  //  {
  //      "ide", "ide", _DMXCLI_IdeCtrl, NULL, "", CLI_SUPERVISOR
  //  },

    {
        "resettest", "rt", _DMXCLI_ResetTest, NULL, "Reset test", CLI_SUPERVISOR
    },

    {
        "setaeskey", "saesk", _DMXCLI_SetAesKey, NULL, "", CLI_SUPERVISOR
    },

    {
        "setaesiv", "saesiv", _DMXCLI_SetAesIV, NULL, "", CLI_SUPERVISOR
    },

//    {
//        "drammeasure", "dm", _DMXCLI_DramMeasure, NULL, "dram measure", CLI_SUPERVISOR
//    },

//    {
//        "fetest", "fet", _DMXCLI_FrontEndTest, NULL, "Front end test", CLI_SUPERVISOR
//    },

#ifdef DMX_CLI_SUPPORT_MULTI2
    {
        "setm2config", "smcfg", _CmdSetMulti2Config, NULL, "", CLI_SUPERVISOR
    },

    {
        "setm2cbciv", "smciv", _CmdSetMulti2CbcIV, NULL, "", CLI_SUPERVISOR
    },

    {
        "setm2ofbiv", "smoiv", _CmdSetMulti2OfbIV, NULL, "", CLI_SUPERVISOR
    },

    {
        "setm2syskey", "smk", _CmdSetMulti2SysKey, NULL, "", CLI_SUPERVISOR
    },
#endif  // DMX_CLI_SUPPORT_MULTI2
#ifdef ENABLE_MULTIMEDIA

    {
        "ps", "ps", NULL, _arDmxPSCmdTbl, "Program stream commands", CLI_SUPERVISOR
    },

    {
        "els", "els", NULL, _arDmxESCmdTbl, "Elementary stream commands", CLI_SUPERVISOR
    },

    {
        "ddi", "ddi", NULL, _arDmxDDICmdTbl, "Pull control commands", CLI_SUPERVISOR
    },

#ifndef CC_DMX_ATSC_MPEG_ONLY
    {
        "multimedia", "mm", NULL, _arDmxMMCmdTbl, "Multimedia commands", CLI_SUPERVISOR
    },
#endif // CC_DMX_ATSC_MPEG_ONLY

#endif  // ENABLE_MULTIMEDIA

#if defined(__linux__)
       {
           "flushpsibuffer", "flpb", _DMXCLI_FlushPSIBuffer, NULL, "", CLI_SUPERVISOR
       },

       {
           "freepsibuffer", "fpb", _DMXCLI_FreePSIBuffer, NULL, "", CLI_SUPERVISOR
       },

       {
           "disablefilter", "df", _DMXCLI_DisableFilter, NULL, "", CLI_SUPERVISOR
       },

       {
           "getscramblestate", "gss", _DMXCLI_GetScrambleState, NULL, "", CLI_SUPERVISOR
       },

#endif  // defined(__linux__)
#endif  // CC_DMX_DEBUG

// These functions exist only in SLT mode
#ifdef __MODEL_slt__
    {
        "diag", "diag", _DMXCLI_Diag, NULL, "Self diagnostic test", CLI_SUPERVISOR
    },

#ifdef CC_DMX_ATSC_MPEG_ONLY
    {
        "jchip", "jchip", _DMXCLI_JChipDiag, NULL, "J-chip functionality test", CLI_SUPERVISOR
    },
#endif  // CC_DMX_ATSC_MPEG_ONLY

#endif  // __MODEL_slt__

#ifdef CC_DMX_ERR_RECOVERY_TEST
    {
        "panictest", "pnt", _DMXCLI_PanicTest, NULL, "pnt", CLI_SUPERVISOR
    },

    {
        "monpanictest", "mpnt", _DMXCLI_MonPanicTest, NULL, "", CLI_SUPERVISOR
    },

    {
        "overflowtest", "ot", DMXCLI_FVROverflow, NULL, "", CLI_SUPERVISOR
    },
#endif // CC_DMX_ERR_RECOVERY_TEST

    {
        "enptslog", "enptslog", _DMXCLI_EnablePtsInfo, NULL, "", CLI_SUPERVISOR
    },

    {
        "ptsdrift", "ptsdrift", _DMXCLI_PtsDriftInfo, NULL, "", CLI_SUPERVISOR
    },

    // LOG macro support
LINT_SAVE_AND_DISABLE
    CLIMOD_DEBUG_CLIENTRY(DMX),
LINT_RESTORE

    {
        NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR
    }

};


LINT_SAVE_AND_DISABLE
CLI_MAIN_COMMAND_ITEM(Dmx)
{
        "dmx", "d", NULL, _arDmxCmdTbl, "Demux commands", CLI_GUEST

};
LINT_RESTORE


//-----------------------------------------------------------------------------
// Inter-file functions
//-----------------------------------------------------------------------------

#if 0
//-----------------------------------------------------------------------------
/** Function
 *  Command: description
 *
 *  @param  i4Argc          Number of arguments
 *  @param  aszArgv         Argument array
 *
 *  @retval 0               Succeed
 *  @retval Others          Error code
 */
//-----------------------------------------------------------------------------
#ifdef __MW_CLI_DEF__
CLI_EXEC_T* GetDmxMwCmdTbl(void)
#else
CLI_EXEC_T* GetDmxCmdTbl(void)
#endif  // __MW_CLI_DEF__
{
    return _arDmxCmd;
}

#endif  // 0
