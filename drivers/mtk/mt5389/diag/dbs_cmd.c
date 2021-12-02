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

#include "drv_dbase.h"
#ifdef __MW_CLI_DEF__
#include "x_mid_cli.h"
#else
#include "x_drv_cli.h"
#endif

#include "drv_common.h"
#include "dmx_if.h"
#include "nim_drvif.h"
#include "srm_drvif.h"
#include "aud_if.h"
#include "aud_drvif.h"
#include "x_assert.h"
#include "x_printf.h"
#include "x_stl_lib.h"
#include "x_hal_5381.h"
#include "x_os.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------

#define DMX_MSG_ENABLE      // Eanble Demux DBS
#define VDEC_MSG_ENABLE      // Eanble Vdieo Decoder DBS
#define VDP_MSG_ENABLE      // Eanble Video DBS
#define AUD_MSG_ENABLE      // Eanble Audio DBS

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

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

#define B2R_PCR_CTRL1               (BLK2RS_BASE + 0x304)


/******************************************************************************
* Function prototype
******************************************************************************/
static INT32 _DbsInitCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _DbsStopCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _DbsShowCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _DbsPrintCmd (INT32 i4Argc, const CHAR ** szArgv);
static INT32 _DbsResetCmd (INT32 i4Argc, const CHAR ** szArgv);

#ifdef DMX_MSG_ENABLE
static BOOL _DbsDmxMessages(VOID);
#endif

#ifdef VDEC_MSG_ENABLE
static VOID _DbsVdecMessages(VOID);
#endif

#ifdef VDP_MSG_ENABLE
static VOID _DbsVdpMessages(VOID);
#endif

#ifdef AUD_MSG_ENABLE
static VOID _DbsAudMessages(VOID);
#endif


/******************************************************************************
* CLI Command Table
******************************************************************************/

static CLI_EXEC_T _arDbsCmdTbl[] =
{
 	{"init",		"i",	_DbsInitCmd, NULL,			"Dbs init", CLI_GUEST},
 	{"stop",		"s",	_DbsStopCmd, NULL,			"Dbs stop", CLI_SUPERVISOR},
 	{"go",			NULL,	_DbsShowCmd, NULL,			"Dbs start", CLI_SUPERVISOR},
 	{"print",		"p",	_DbsPrintCmd, NULL,			"Dbs print at once", CLI_GUEST},
 	{"reset",		"r",	_DbsResetCmd, NULL,			"Dbs reset", CLI_GUEST},

 	{NULL, NULL, NULL, NULL, NULL, CLI_SUPERVISOR}
};

CLI_MAIN_COMMAND_ITEM(Dbs)
{
	"dbs",
	NULL,
	NULL,
	_arDbsCmdTbl,
	"Dbs command",
	CLI_GUEST
};

/******************************************************************************
* Local Function
******************************************************************************/
static DRV_DBASE_T* _prDbase = NULL;
static volatile UINT32 _u4ThreadLoop = 0;
static UINT32 _u4ThreadDelayMs = 30000;
static HANDLE_T _hDbsThread;

extern UINT32 DmxHW_GetPidEntryIdx(UINT16 u2Entry, UINT32 u4idx );

#ifdef DMX_MSG_ENABLE
static BOOL _DbsDmxMessages(VOID)
{
    static CHAR szMsg[256], szBuf[128];
    DMX_PID_T rPid;
    DMX_PID_COUNTERS_T rCounters;
    CHAR* pszType;
    UINT32 u4DataSize, i;

    for (i = 0; i < DMX_NUM_PID_INDEX; i++)
    {
        // Get PID info
        if (!DMX_GetPid(i, DMX_PID_FLAG_ALL, &rPid))
        {
            return FALSE;
        }

        if (!rPid.fgEnable)
        {
            continue;
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

        x_snprintf(szMsg, 256, "DMX(%u) PID: 0x%x, %s", i, rPid.u2Pid, pszType);

        // PID counters
        if (!DMX_GetPidCounters(i, &rCounters))
        {
            return FALSE;
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
        if (rCounters.u4SecCount > 0)
        {
            x_snprintf(szBuf, 128, ", Sec: %u", rCounters.u4SecCount);
            x_strncat(szMsg, szBuf, 128);
        }

        u4DataSize = DATASIZE(rPid.u4Rp, rPid.u4Wp, rPid.u4BufSize);
        x_snprintf(szBuf, 128, ", Errors: %u, data: %u, PeakBufFull: %u\n", rCounters.u4TotalErrors,
            u4DataSize, rPid.u4PeakBufFull);
        x_strncat(szMsg, szBuf, 128);

        // Show PID info
        Printf(szMsg);
    }

    return TRUE;
}
#endif

#ifdef VDEC_MSG_ENABLE
static VOID _DbsVdecMessages(VOID)
{
	UINT32 u4Idx;
	
	for (u4Idx = 0; u4Idx < MPV_MAX_ES; u4Idx++)
	{
			if ((_prDbase->rMpv[u4Idx].u4PsrDropNs != 0) ||
				(_prDbase->rMpv[u4Idx].u4RetrieveNs != 0) ||
				(_prDbase->rMpv[u4Idx].u4EsmSkipNs != 0) ||
				(_prDbase->rMpv[u4Idx].u4VpsrDropNs != 0) ||
				(_prDbase->rMpv[u4Idx].u4DecOkNs != 0) ||
				(_prDbase->rMpv[u4Idx].u4TimeOutNs != 0) ||
				(_prDbase->rMpv[u4Idx].u4ErrDropNs != 0)||
				(_prDbase->rMpv[u4Idx].u4EsmQCnt!= 0))
			{
				Printf("MPV(%d) Receive(%d) OK(%d) Skip(%d) Drop(%d) Timeout(%d) Q(%d) ",
				u4Idx,  _prDbase->rMpv[u4Idx].u4RetrieveNs, _prDbase->rMpv[u4Idx].u4DecOkNs, 
				_prDbase->rMpv[u4Idx].u4EsmSkipNs, _prDbase->rMpv[u4Idx].u4VpsrDropNs,
				_prDbase->rMpv[u4Idx].u4TimeOutNs, _prDbase->rMpv[u4Idx].u4EsmQCnt);

				if (_prDbase->rMpv[u4Idx].u4DecOkNs != 0)
				{
					if (_prDbase->rMpv[u4Idx].u4HaveI == 0)
					{
						Printf("(%s/", "No_I");
					}
					else if (_prDbase->rMpv[u4Idx].u4NoIAfterSeqH== 1)
					{
						Printf("(%s/", "No_I_AfterSeqHdr");
					}
					else
					{
						Printf("(%d.%06d/", _prDbase->rMpv[u4Idx].rIDeltaTime.u4Seconds, _prDbase->rMpv[u4Idx].rIDeltaTime.u4Micros);
					}

					Printf("%dx%d", _prDbase->rMpv[u4Idx].u2HSize, _prDbase->rMpv[u4Idx].u2VSize);

					if (_prDbase->rMpv[u4Idx].ucPicStruct == 3)
					{
						Printf("/%s", "Frame");
					}
					else
					{
						Printf("/%s", "Field");
					}

					switch (_prDbase->rMpv[u4Idx].ucFrameRate)
					{
						case 1:
							Printf("/%s)\n", "23.976");
							break;
						case 2:
							Printf("/%s)\n", "24");
							break;
						case 3:
							Printf("/%s)\n", "25");
							break;
						case 4:
							Printf("/%s)\n", "29.97");
							break;
						case 5:
							Printf("/%s)\n", "30");
							break;
						case 6:
							Printf("/%s)\n", "50");
							break;
						case 7:
							Printf("/%s)\n", "59.94");
							break;
						case 8:
							Printf("/%s)\n", "60");
							break;
						default:
							Printf(")\n");
							break;
					}
				}
				else
				{
					Printf("\n");
				}
			}
	}
}
#endif

#ifdef VDP_MSG_ENABLE
static VOID _DbsVdpMessages(VOID)
{
	UINT32 u4Idx;
	UINT32 u4RegValue;

	for (u4Idx = 0; u4Idx < VDP_NS; u4Idx++)
	{
		if (_prDbase->rVdp.arFbCounter[u4Idx][0].u4DispOk != 0)
		{
			Printf("VDP(%d) OK(%d) Fail(%d) Skip(%d) Repeat(%d) Drop(%d) UF(%d) T(%d, %d, %d)",
				u4Idx,
				_prDbase->rVdp.arFbCounter[u4Idx][0].u4DispOk,
                _prDbase->rVdp.arFbCounter[u4Idx][0].u4DispFail,
                _prDbase->rVdp.arFbCounter[u4Idx][0].u4DispSkip,
                _prDbase->rVdp.arFbCounter[u4Idx][0].u4DispRepeat,
                _prDbase->rVdp.arFbCounter[u4Idx][0].u4DispDrop,
                _prDbase->rVdp.au4UnderFlow[u4Idx],
                _prDbase->rVdp.au4Tearing[u4Idx],
                _prDbase->rVdp.au4StartLineCnt[u4Idx],
                _prDbase->rVdp.au4StopLineCnt[u4Idx]);

			switch (_prDbase->rVdp.arFbCounter[u4Idx][0].u4SyncMode)
			{
				case 0:
					Printf(" (Disable)\n");
					break;
				case 1:
					Printf(" (STC Invalid)\n");
					break;
				case 2:
					Printf(" (Wait 1st)\n");
					break;
				case 4:
					Printf(" (Sync)\n");
					break;
				case 5:
					Printf(" (Repeat)\n");
					break;
				case 6:
					Printf(" (Drop)\n");
					break;
				case 7:
					Printf(" (PTS Jitter)\n");
					break;
				case 8:
					Printf(" (NO PTS)\n");
					break;
				default:
					Printf("(?)\n");
					break;
			}
		}
	}

#ifdef CC_VCXO_ON
    u4RegValue = (IO_REG32(BIM_BASE, 0x230) >> 16) & 0xFF;
	Printf("VCxO(%d)\n", u4RegValue);
#else
    u4RegValue = IO_REG32(BLK2RS_BASE, 0x304);

    if (u4RegValue >= 0x100)
    {
    	Printf("VCxO S(+%d)\n", (u4RegValue & 0xFF));
    }
    else
    {
    	Printf("VCxO S(-%d)\n", (u4RegValue & 0xFF));
    }
#endif

    u4RegValue = IO_REG32(DEMUX0_BASE, 0x250); 
	Printf("PCR Counter(%d)\n", u4RegValue);

#if 0
	Printf("DRAM BW MAX(%d) AVG(%d) MIN(%d) COUNT(%d)\n"
        , _prDbase->rDram.u4MaxBw
        , _prDbase->rDram.u4AvgBw
        , _prDbase->rDram.u4MinBw
        , _prDbase->rDram.u4Count);
#endif
}
#endif

#ifdef AUD_MSG_ENABLE
static VOID _DbsAudMessages(VOID)
{
    // AUD statistics
    if (AUD_IsDecoderPlay(AUD_DEC_MAIN) || AUD_IsDecoderPlay(AUD_DEC_AUX))
    {
        UINT8 u1Acmode;
        UINT8 u1SmpRate;
        UINT32 u4DataRate;
        INT32 i4Idx;

        UNUSED(fgAudStatistics());

        if (AUD_IsDecoderPlay(AUD_DEC_MAIN))
        {
            Printf("AUD(0) (%d,%d,%d,%d) (%d,%d,%d) (%d,%d,%d,%d)(%d,%d,%d,%d)(%d,%d,%d,%d)(%d,%d,%d,%d)\n",
        	       _prDbase->rAud.dwTotalFrameCount,
        	       _prDbase->rAud.dwTotalErrorCount,
        	       _prDbase->rAud.dwFrameRepeatCount,
        	       _prDbase->rAud.dwFrameDropCount,
        	       _prDbase->rAud.dwInputUnderRun,
        	       _prDbase->rAud.dwOutputUnderRun,
        	       _prDbase->rAud.dwInputUnderRunMixSound,
                   _prDbase->rAud.dwDetailLogs[0],
                   _prDbase->rAud.dwDetailLogs[1],
                   _prDbase->rAud.dwDetailLogs[2],
                   _prDbase->rAud.dwDetailLogs[3],
                   _prDbase->rAud.dwDetailLogs[4],
                   _prDbase->rAud.dwDetailLogs[5],
                   _prDbase->rAud.dwDetailLogs[6],
                   _prDbase->rAud.dwDetailLogs[7],
                   _prDbase->rAud.dwDetailLogs[8],
                   _prDbase->rAud.dwDetailLogs[9],
                   _prDbase->rAud.dwDetailLogs[10],
                   _prDbase->rAud.dwDetailLogs[11],
                   _prDbase->rAud.dwDetailLogs[12],
                   _prDbase->rAud.dwDetailLogs[13],
                   _prDbase->rAud.dwDetailLogs[14],
                   _prDbase->rAud.dwDetailLogs[15]);
        }
        if (AUD_IsDecoderPlay(AUD_DEC_AUX))
        {
            Printf("AUD(1) (%d,%d,%d,%d) (%d,%d,%d) (%d,%d,%d,%d)(%d,%d,%d,%d)(%d,%d,%d,%d)(%d,%d,%d,%d)\n",
        	       _prDbase->rAud.dwTotalFrameCountDec2,
        	       _prDbase->rAud.dwTotalErrorCountDec2,
        	       _prDbase->rAud.dwFrameRepeatCountDec2,
        	       _prDbase->rAud.dwFrameDropCountDec2,
        	       _prDbase->rAud.dwInputUnderRunDec2,
        	       _prDbase->rAud.dwOutputUnderRunDec2,
        	       _prDbase->rAud.dwInputUnderRunMixSound,
        	       _prDbase->rAud.dwDetailLogsDec2[0],
                   _prDbase->rAud.dwDetailLogsDec2[1],
                   _prDbase->rAud.dwDetailLogsDec2[2],
                   _prDbase->rAud.dwDetailLogsDec2[3],
                   _prDbase->rAud.dwDetailLogsDec2[4],
                   _prDbase->rAud.dwDetailLogsDec2[5],
                   _prDbase->rAud.dwDetailLogsDec2[6],
                   _prDbase->rAud.dwDetailLogsDec2[7],
                   _prDbase->rAud.dwDetailLogsDec2[8],
                   _prDbase->rAud.dwDetailLogsDec2[9],
                   _prDbase->rAud.dwDetailLogsDec2[10],
                   _prDbase->rAud.dwDetailLogsDec2[11],
                   _prDbase->rAud.dwDetailLogsDec2[12],
                   _prDbase->rAud.dwDetailLogsDec2[13],
                   _prDbase->rAud.dwDetailLogsDec2[14],
                   _prDbase->rAud.dwDetailLogsDec2[15]);
        }

        for (i4Idx = 0; i4Idx < AUD_DEC_NUM; i4Idx ++)
        {
            if (AUD_IsDecoderPlay(i4Idx))
            {
                AUD_GetStreamInfo(i4Idx, &u1Acmode, &u1SmpRate, &u4DataRate);

                Printf("AUD(%d) (",i4Idx);

                switch (u1Acmode)
                {
                    case 0:
                    	Printf("%s,", "1+1");
                    	break;
                    case 1:
                    	Printf("%s,", "1/0");
                    	break;
                    case 2:
                    	Printf("%s,", "2/0");
                    	break;
                    case 3:
                    	Printf("%s,", "3/0");
                    	break;
                    case 4:
                    	Printf("%s,", "2/1");
                    	break;
                    case 5:
                    	Printf("%s,", "3/1");
                    	break;
                    case 6:
                    	Printf("%s,", "2/2");
                    	break;
                    case 7:
                    	Printf("%s,", "3/2");
                    	break;
                    default:
                    	Printf("%s,", "Error acmode");
                    	break;
                }

                Printf("%d bps,",u4DataRate);

                switch (u1SmpRate)
                {
		    	    case 13:
		    			Printf("%s)\n", "48 KHz");
		    			break;
		    		case 8:
		    			Printf("%s)\n", "44.1 KHz");
		    			break;
		    		case 3:
		    			Printf("%s)\n", "32 KHz");
		    			break;
		    		default:
		    			Printf("%s)\n", "Error sample rate");
		    			break;
                }
            }
        }
    }
}
#endif

// record dram bandwidth
void DBS_RecordDramBw(UINT32 *pu4Max, UINT32 *pu4Avg, UINT32 *pu4Min, 
    UINT32 *pu4Count)
{
    *pu4Max = _prDbase->rDram.u4MaxBw;
    *pu4Avg = _prDbase->rDram.u4AvgBw;
    *pu4Min = _prDbase->rDram.u4MinBw;
    *pu4Count = _prDbase->rDram.u4Count;
}


// print dram bandwidth
void DBS_DramBwPrintf(void)
{
	Printf("   Dram Bandwidth -> MAX(%d) AVG(%d) MIN(%d) COUNT(%d)\n"
        , _prDbase->rDram.u4MaxBw
        , _prDbase->rDram.u4AvgBw
        , _prDbase->rDram.u4MinBw
        , _prDbase->rDram.u4Count);
}

void DBS_Printf(void)
{
#ifdef DMX_MSG_ENABLE
    VERIFY(_DbsDmxMessages());
#endif

#ifdef VDEC_MSG_ENABLE
    _DbsVdecMessages();
#endif

#ifdef VDP_MSG_ENABLE
    _DbsVdpMessages();
#endif

#ifdef AUD_MSG_ENABLE
    _DbsAudMessages();
#endif

#if 0
    SRM_QuesyFifoStatus();
#endif
}

static INT32 _DbsInitCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	CRIT_STATE_T _rCritState;

    UNUSED(i4Argc);
    UNUSED(szArgv);

	_prDbase = DBS_Lock(&_rCritState);
	ASSERT(_prDbase);
    VERIFY(DBS_Unlock(_prDbase, _rCritState));

	return 0;
}

static void _DbsShowThread(void *pvArgv)
{
	while (_u4ThreadLoop)
	{
		DBS_Printf();

		x_thread_delay(_u4ThreadDelayMs);
	}

	Printf("DBS Thread Exist\n");
}


static INT32 _DbsStopCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	_u4ThreadLoop = 0;

	return 0;
}

static INT32 _DbsShowCmd (INT32 i4Argc, const CHAR ** szArgv)
{
	if (_prDbase == NULL)
	{
		return -1;
	}

	if (i4Argc >= 2)
	{
		_u4ThreadDelayMs =  StrToInt(szArgv[1]) * 1000;
	}

	if (_u4ThreadLoop == 0)
	{
		INT32 i4Ret;
		_u4ThreadLoop = 1;

		i4Ret = x_thread_create(&_hDbsThread, DBS_THREAD_NAME, DBS_STACK_SIZE, DBS_THREAD1_PRIORITY, _DbsShowThread, 0, NULL);

		if (i4Ret != OSR_OK) {
			Printf("Create thread failed.\n");
			return 1;
		}
	}

	return 0;
}

static INT32 _DbsPrintCmd (INT32 i4Argc, const CHAR ** szArgv)
{
    UNUSED(i4Argc);
    UNUSED(szArgv);

	DBS_Printf();

	return 0;
}

static INT32 _DbsResetCmd (INT32 i4Argc, const CHAR ** szArgv)
{
#if 0
    SRM_ResetFifoStatus();
#endif

    if (DBS_Init() == TRUE)
    {
    	return 0;
    }
    else
    {
        return -1;
    }
}


