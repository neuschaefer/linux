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
 * $Author: kai.wang $
 * $Date: 2012/06/28 $
 * $RCSfile: aud_drv.c,v $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_drv.c
 *  Brief of file aud_drv.c.
 *  Details of file aud_drv.c (optional).
 */

#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------

#include "x_lint.h"
// KERNEL

LINT_EXT_HEADER_BEGIN
#include "x_util.h"
#include "x_typedef.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_printf.h"
#include "x_assert.h"
#include "aud_drvif.h"
#include "x_ckgen.h"
#include "x_pinmux.h"
//#include "x_pinmux.h"

#include "aud_debug.h"
#include "dsp_common.h"
#include "dsp_shm.h"
#include "drv_adsp.h"
#include "aud_dsp_cfg.h"
#include "aud_if.h"
#include "aud_drv.h"
#if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
#include "../../nptv6896/inc/hw/hw_sw.h"
#else
#include "../../nptv6595/inc/hw/hw_sw.h"
#endif

LINT_EXT_HEADER_END

#ifdef CC_AUD_NCSTOOL_SUPPORT

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

#define AUD_TOOL_THREAD_PERIOD          30

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------
extern void _VolumeCtrl(UINT8 u1DecId, AUD_CH_T eChannel, UINT32 u4Value);

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Function Implementation
//---------------------------------------------------------------------------
void _AudToolThread(void* pvArg)
{
    //Declaration
    UINT32 i;
    BOOL fgTmp;
    UINT8 u1Tmp;
    UINT16 u2Tmp;
    UINT32 u4Tmp;

    AUD_DEC_STREAM_FROM_T eStreamFrom = AUD_STREAM_FROM_LINE_IN;
    AUD_FMT_T eDecFmt = AUD_FMT_PCM;

    //Initialization
    //Printf("[_AudToolThread] Create !!!!\n");

    //Reset Ctrl bit
    vIO32Write4B(AUD_TOOL_CTRL_00, 0);
    vIO32Write4B(AUD_TOOL_CTRL_01, 0);

    //monitor loop
    while (1)
    {
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_TRIG_QUERY_DEC1))
        {
            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_QUERY_DEC1\n");
            //Reset
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, 0, AUD_TOOL_CTRL_TRIG_QUERY_DEC1);
            AudShowStatus(AUD_DEC_MAIN);

            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_QUERY_DEC1 End\n");
        }

        if (IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_TRIG_QUERY_DEC2))
        {
            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_QUERY_DEC2\n");

            //Reset
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, 0, AUD_TOOL_CTRL_TRIG_QUERY_DEC2);
            AudShowStatus(AUD_DEC_AUX);

            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_QUERY_DEC2 End\n");
        }

        if (IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_TRIG_QUERY_DEC3))
        {
            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_QUERY_DEC3\n");

            //Reset
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, 0, AUD_TOOL_CTRL_TRIG_QUERY_DEC3);
            AudShowStatus(AUD_DEC_THIRD);

            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_QUERY_DEC3 End\n");
        }

        if (IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_TRIG_QUERY_DEC4))
        {
            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_QUERY_DEC4\n");

            //Reset
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, 0, AUD_TOOL_CTRL_TRIG_QUERY_DEC4);
            AudShowStatus(AUD_DEC_4TH);

            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_QUERY_DEC4 End\n");
        }

        if (IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_TRIG_QUERY_DSP))
        {
            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_QUERY_DSP\n");

            //Reset
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, 0, AUD_TOOL_CTRL_TRIG_QUERY_DSP);
            AudShowDspStatus();

            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_QUERY_DSP End\n");
        }

        if (IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_TRIG_QUERY_VC))
        {
            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_QUERY_VC\n");

            //Reset
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, 0, AUD_TOOL_CTRL_TRIG_QUERY_VC);

            Printf("=== Volume Curve ===\n");
            for (i=0; i < 101; i++) //FIXME !! should use definition.
            {
                if ((i % 5)==0)
                {
                    Printf("\n");
                }
                Printf("0x%x ", AUD_DspGetVolumeTable(i) );
            }
            Printf("\n");

            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_QUERY_VC End\n");
        }

        if (IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_TRIG_QUERY_CVC))
        {
            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_QUERY_CVC\n");

            //Reset
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, 0, AUD_TOOL_CTRL_TRIG_QUERY_CVC);

            Printf("=== Channel Volume Curve ===\n");
            for (i=0; i < 101; i++) //FIXME !! should use definition.
            {
                if ((i % 5)==0)
                {
                    Printf("\n");
                }
#ifdef CC_AUD_NEW_CV_TABLE
                Printf("0x%x ", AUD_DspGetCVTable(i) );
#else
                Printf("0x%x ", AUD_DspGetVolumeTable(i) );
#endif
            }
            Printf("\n");

            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_QUERY_CVC End\n");
        }

        if (IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_TRIG_TONE_PLAY))
        {
            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_TONE_PLAY\n");

            //Reset
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, 0, AUD_TOOL_CTRL_TRIG_TONE_PLAY);

            {
                MEM_BUFFER_INFO_T  rMemBuf;
                AUD_PCM_SETTING_T  rPcmSetting;

                //if previous playback is not tone
                if (!((_AudGetStrSource(AUD_DEC_MAIN) == AUD_STREAM_FROM_MEMORY) && (_AudGetStrFormat(AUD_DEC_MAIN) == AUD_FMT_PCM)))
                {
                    //save previous play state
                    eStreamFrom = _AudGetStrSource(AUD_DEC_MAIN);
                    eDecFmt = _AudGetStrFormat(AUD_DEC_MAIN);
                }

                if (AUD_IsDecoderPlay(AUD_DEC_MAIN))
                {
                    AUD_StopMemStr();
                    AUD_DSPCmdStop(AUD_DEC_MAIN);
                }

                rMemBuf.u4LoopCount = 0; // Infinite loop
                rPcmSetting.eSampleFreq = FS_48K;
                rPcmSetting.ePcmDataInfo = PCM_24_BITS;

                AUD_DspProcMode(AUD_DEC_MAIN, 0xdc00);
                AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_MEMORY, AUD_FMT_PCM);
                AUD_PcmSetting(AUD_DEC_MAIN, (const AUD_PCM_SETTING_T *)&rPcmSetting);
                AUD_StrSelect(AUD_DEC_MAIN, IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_TRIG_TONE_SELECT), &rMemBuf);
                AUD_SetDecMemBuffer(AUD_DEC_MAIN, &rMemBuf);
                AUD_DSPCmdPlay(AUD_DEC_MAIN);
            }

            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_TONE_PLAY End\n");
        }

        if (IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_TRIG_TONE_STOP))
        {
            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_TONE_STOP\n");

            //Reset
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, 0, AUD_TOOL_CTRL_TRIG_TONE_STOP);

            if (AUD_IsDecoderPlay(AUD_DEC_MAIN))
            {
                AUD_StopMemStr();
                AUD_DSPCmdStop(AUD_DEC_MAIN);
            }

            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_TONE_STOP End\n");
        }

        if (IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_TRIG_TONE_RESTORE))
        {
            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_TONE_RESTORE\n");

            //Reset
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, 0, AUD_TOOL_CTRL_TRIG_TONE_RESTORE);

            if (AUD_IsDecoderPlay(AUD_DEC_MAIN))
            {
                AUD_StopMemStr();
                AUD_DSPCmdStop(AUD_DEC_MAIN);
            }

            AUD_DspProcMode(AUD_DEC_MAIN, 0x0c00);
            AUD_DRVSetDecodeType(AUD_DEC_MAIN, eStreamFrom, eDecFmt);
            AUD_DSPCmdPlay(AUD_DEC_MAIN);

            //Printf("[_AudToolThread] AUD_TOOL_CTRL_TRIG_TONE_RESTORE End\n");
        }

        //Read All AQ Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_ALL))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_ALL);

            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_READ_BYPASS);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_READ_EQ);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_READ_BASS);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_READ_TREBLE);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_READ_LIMITER);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_READ_AVC);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_READ_VS);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_READ_VOLUME_TBL);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_READ_SV);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_READ_CV);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_READ_EXGAIN);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_READ_CHDELAY);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_READ_BASSMNG);
#ifdef CC_AUD_VBASS_SUPPORT
			vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_READ_VBASS);
#endif
#ifdef CC_AUD_SPEAKER_HEIGHT
			vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_READ_SPKHEIGHT);
#endif
        }

        //Write All AQ Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_ALL))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_ALL);

            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_WRITE_BYPASS);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_WRITE_EQ);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_WRITE_BASS);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_WRITE_TREBLE);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_WRITE_LIMITER);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_WRITE_AVC);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_WRITE_VS);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_WRITE_VOLUME_TBL);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_WRITE_SV);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_WRITE_CV);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_WRITE_EXGAIN);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_WRITE_CHDELAY);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_WRITE_BASSMNG);
#ifdef CC_AUD_VBASS_SUPPORT
			vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_WRITE_VBASS);
#endif
#ifdef CC_AUD_SPEAKER_HEIGHT
			vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 1, AUD_TOOL_CTRL_TRIG_WRITE_SPKHEIGHT);
#endif
        }

        //Read Bypass Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_BYPASS))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_BYPASS);
            {
                fgTmp = ((u2ReadShmUINT16(W_PROCMOD)&0xf000) ? TRUE : FALSE);
                vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_ALL_BYPASS);
                fgTmp = ((uReadShmUINT8(B_EQFLAG)&0x1) ? TRUE : FALSE);
                vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_EQ_ENABLE);
                fgTmp = ((uReadShmUINT8(B_EQFLAG)&0x2) ? TRUE : FALSE);
                vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_BASS_ENABLE);
                fgTmp = ((uReadShmUINT8(B_EQFLAG)&0x8) ? TRUE : FALSE);
                vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_TREBLE_ENABLE);
                fgTmp = (u2ReadShmUINT16(W_LIMITER_MODE) ? TRUE : FALSE);
                vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_LIMITER_ENABLE);
                fgTmp = ((u2ReadShmUINT16(W_AVC_FLAG)&0x3) ? TRUE : FALSE);
                vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_AVC_ENABLE);
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
                fgTmp = (uReadShmUINT8(B_VSURRFLAG) ? TRUE : FALSE);
#else
                fgTmp = (uReadShmUINT8(W_VSURRFLAG) ? TRUE : FALSE);
#endif
                vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_VS_ENABLE);
                fgTmp = ((u2ReadShmUINT16(W_PROCMOD)&BYPASS_BASS_MANAGEMENT) ? FALSE : TRUE);
                vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_BASSMNG_ENABLE);
#ifdef CC_AUD_VBASS_SUPPORT
				fgTmp = (uReadShmUINT8(B_VIR_BASS_ENABLE) ? TRUE : FALSE);
                vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_VBASS_ENABLE);
#endif
#ifdef CC_AUD_SPEAKER_HEIGHT
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
				fgTmp = (uReadShmUINT8(B_SPH_ENABLE) ? TRUE : FALSE);
#else
				fgTmp = (uReadShmUINT8(B_SPH_MODE) ? TRUE : FALSE);
#endif
				vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_SPKHEIGHT_ENABLE);
#endif
            }
        }

        //Read Volume Table Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_VOLUME_TBL))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_VOLUME_TBL);
            {
                //Update Master Volume
                vIO32Write4B(AUD_TOOL_VOL_MASTER, u4ReadShmUINT32(D_VOL));

                //Update Volume Table
                for (i=0;i<101;i++)
                {
                    vIO32Write4B(AUD_TOOL_VOL_TBL+(UINT32)i*4, AUD_DspGetVolumeTable(i));
                }
            }
        }

        //Read Source Volume Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_SV))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_SV);
            {
                //Update Inpur Source Gain
                for (i=0; i<AUD_STREAM_FROM_NUM; i++)
                {
                    vIO32WriteFldAlign(AUD_TOOL_SRC_GAIN_OTHERS+i*4,
                                        AUD_DspGetSrcVol(AUD_DEC_MAIN, (AUD_DEC_STREAM_FROM_T)i),
                                        AUD_TOOL_SRC_GAIN_FLD);
                }
            }
        }

        //Read Channel Volume Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_CV))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_CV);
            {
                //Update Channel Volume Gain
                vIO32Write4B(AUD_TOOL_CH_VOL_C, u4ReadShmUINT32(D_TRIM_C));
                vIO32Write4B(AUD_TOOL_CH_VOL_L, u4ReadShmUINT32(D_TRIM_L));
                vIO32Write4B(AUD_TOOL_CH_VOL_R, u4ReadShmUINT32(D_TRIM_R));
                vIO32Write4B(AUD_TOOL_CH_VOL_LS, u4ReadShmUINT32(D_TRIM_LS));
                vIO32Write4B(AUD_TOOL_CH_VOL_RS, u4ReadShmUINT32(D_TRIM_RS));
                vIO32Write4B(AUD_TOOL_CH_VOL_CH7, u4ReadShmUINT32(D_TRIM_CH7));
                vIO32Write4B(AUD_TOOL_CH_VOL_CH8, u4ReadShmUINT32(D_TRIM_CH8));
                vIO32Write4B(AUD_TOOL_CH_VOL_SUB, u4ReadShmUINT32(D_TRIM_SUB));
                //vIO32Write4B(AUD_TOOL_CH_VOL_LFE, u4ReadShmUINT32(D_TRIM_LFE));
                vIO32Write4B(AUD_TOOL_CH_VOL_CH9, u4ReadShmUINT32(D_TRIM_CH9));
                vIO32Write4B(AUD_TOOL_CH_VOL_CH10, u4ReadShmUINT32(D_TRIM_CH10));
                vIO32Write4B(AUD_TOOL_CH_VOL_AUXL, u4ReadShmUINT32(D_TRIM_L_DEC2));
                vIO32Write4B(AUD_TOOL_CH_VOL_AUXR, u4ReadShmUINT32(D_TRIM_R_DEC2));
                //vIO32Write4B(AUD_TOOL_CH_VOL_AUXLFE, u4ReadShmUINT32(D_TRIM_LFE_DEC2));
            }
        }

        //Read Extra-Gain Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_EXGAIN))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_EXGAIN);
            {
                //Update Extra-Gain
                vIO32WriteFldAlign(AUD_TOOL_EXTRA_GAIN_00,AUD_DspGetChannelVolGain(AUD_DEC_MAIN,AUD_CH_FRONT_LEFT),AUD_TOOL_EXTRA_GAIN_L);
                vIO32WriteFldAlign(AUD_TOOL_EXTRA_GAIN_00,AUD_DspGetChannelVolGain(AUD_DEC_MAIN,AUD_CH_FRONT_RIGHT),AUD_TOOL_EXTRA_GAIN_R);
                vIO32WriteFldAlign(AUD_TOOL_EXTRA_GAIN_00,AUD_DspGetChannelVolGain(AUD_DEC_MAIN,AUD_CH_REAR_LEFT),AUD_TOOL_EXTRA_GAIN_LS);
                vIO32WriteFldAlign(AUD_TOOL_EXTRA_GAIN_00,AUD_DspGetChannelVolGain(AUD_DEC_MAIN,AUD_CH_REAR_RIGHT),AUD_TOOL_EXTRA_GAIN_RS);

                vIO32WriteFldAlign(AUD_TOOL_EXTRA_GAIN_01,AUD_DspGetChannelVolGain(AUD_DEC_MAIN,AUD_CH_CENTER),AUD_TOOL_EXTRA_GAIN_C);
                vIO32WriteFldAlign(AUD_TOOL_EXTRA_GAIN_01,AUD_DspGetChannelVolGain(AUD_DEC_MAIN,AUD_CH_SUB_WOOFER),AUD_TOOL_EXTRA_GAIN_SUB);
                vIO32WriteFldAlign(AUD_TOOL_EXTRA_GAIN_01,AUD_DspGetChannelVolGain(AUD_DEC_MAIN,AUD_CH_BYPASS_LEFT),AUD_TOOL_EXTRA_GAIN_CH7);
                vIO32WriteFldAlign(AUD_TOOL_EXTRA_GAIN_01,AUD_DspGetChannelVolGain(AUD_DEC_MAIN,AUD_CH_BYPASS_RIGHT),AUD_TOOL_EXTRA_GAIN_CH8);

                vIO32WriteFldAlign(AUD_TOOL_EXTRA_GAIN_02,AUD_DspGetChannelVolGain(AUD_DEC_MAIN,AUD_CH_DMX_LEFT),AUD_TOOL_EXTRA_GAIN_CH9);
                vIO32WriteFldAlign(AUD_TOOL_EXTRA_GAIN_02,AUD_DspGetChannelVolGain(AUD_DEC_MAIN,AUD_CH_DMX_RIGHT),AUD_TOOL_EXTRA_GAIN_CH10);
                vIO32WriteFldAlign(AUD_TOOL_EXTRA_GAIN_02,AUD_DspGetChannelVolGain(AUD_DEC_AUX,AUD_CH_FRONT_LEFT),AUD_TOOL_EXTRA_GAIN_AUXL);
                vIO32WriteFldAlign(AUD_TOOL_EXTRA_GAIN_02,AUD_DspGetChannelVolGain(AUD_DEC_AUX,AUD_CH_FRONT_RIGHT),AUD_TOOL_EXTRA_GAIN_AUXR);
            }
        }

        //Read Channel Delay Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_CHDELAY))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_CHDELAY);
            {
                //Update Extra-Gain
                vIO32WriteFldAlign(AUD_TOOL_CHDELAY_00,AUD_DspGetChannelDelay(AUD_CH_FRONT_LEFT),AUD_TOOL_CHDELAY_L);
                vIO32WriteFldAlign(AUD_TOOL_CHDELAY_00,AUD_DspGetChannelDelay(AUD_CH_FRONT_RIGHT),AUD_TOOL_CHDELAY_R);
                vIO32WriteFldAlign(AUD_TOOL_CHDELAY_00,AUD_DspGetChannelDelay(AUD_CH_REAR_LEFT),AUD_TOOL_CHDELAY_LS);
                vIO32WriteFldAlign(AUD_TOOL_CHDELAY_00,AUD_DspGetChannelDelay(AUD_CH_REAR_RIGHT),AUD_TOOL_CHDELAY_RS);

                vIO32WriteFldAlign(AUD_TOOL_CHDELAY_01,AUD_DspGetChannelDelay(AUD_CH_CENTER),AUD_TOOL_CHDELAY_C);
                vIO32WriteFldAlign(AUD_TOOL_CHDELAY_01,AUD_DspGetChannelDelay(AUD_CH_SUB_WOOFER),AUD_TOOL_CHDELAY_SUB);
                vIO32WriteFldAlign(AUD_TOOL_CHDELAY_01,AUD_DspGetChannelDelay(AUD_CH_BYPASS_LEFT),AUD_TOOL_CHDELAY_CH7);
                vIO32WriteFldAlign(AUD_TOOL_CHDELAY_01,AUD_DspGetChannelDelay(AUD_CH_BYPASS_RIGHT),AUD_TOOL_CHDELAY_CH8);

                vIO32WriteFldAlign(AUD_TOOL_CHDELAY_02,AUD_DspGetChannelDelay(AUD_CH_DMX_LEFT),AUD_TOOL_CHDELAY_CH9);
                vIO32WriteFldAlign(AUD_TOOL_CHDELAY_02,AUD_DspGetChannelDelay(AUD_CH_DMX_RIGHT),AUD_TOOL_CHDELAY_CH10);
            }
        }

        //Read EQ Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_EQ))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_EQ);
            fgTmp = ((uReadShmUINT8(B_EQFLAG)&0x1) ? TRUE : FALSE);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_EQ_ENABLE);
            {
                INT8 eqValue[CH_EQ_BAND_NO+1];
                AUD_EQ_TYPE_T eEqType;

                AUD_DspGetEqType(AUD_DEC_MAIN, &eEqType);
                AUD_DspChQryEqTable(AUD_DEC_MAIN, eEqType, eqValue);

                for (i = 1; i <= CH_EQ_BAND_NO; i++)
                {
                    vIO32WriteFldAlign(AUD_TOOL_EQ_BAND1+(i-1)*4,
                                        eqValue[i],
                                        AUD_TOOL_EQ_BAND_FLD);
                }
            }
        }

        //Read Bass Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_BASS))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_BASS);
            fgTmp = ((uReadShmUINT8(B_EQFLAG)&0x2) ? TRUE : FALSE);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_BASS_ENABLE);
            {
                vIO32WriteFldAlign(AUD_TOOL_BASS, AUD_DspGetBassBoostGain(AUD_DEC_MAIN), AUD_TOOL_SBASS_FLD);
            }
        }

        //Read Treble Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_TREBLE))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_TREBLE);
            fgTmp = ((uReadShmUINT8(B_EQFLAG)&0x8) ? TRUE : FALSE);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_TREBLE_ENABLE);
            {
                vIO32WriteFldAlign(AUD_TOOL_TREBLE, AUD_DspGetClearBoostGain(AUD_DEC_MAIN),AUD_TOOL_SBASS_FLD);
            }
        }

        //Read Limiter Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_LIMITER))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_LIMITER);
            fgTmp = (u2ReadShmUINT16(W_LIMITER_MODE) ? TRUE : FALSE);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_LIMITER_ENABLE);
            {
                AUD_DspGetLimiterConfig(&u1Tmp, &u2Tmp, &u4Tmp);
                vIO32WriteFldAlign(AUD_TOOL_LIMITER, u1Tmp, AUD_TOOL_LIMITER_MODE);
                vIO32WriteFldAlign(AUD_TOOL_LIMITER, u4Tmp, AUD_TOOL_LIMITER_THRES);
            }
        }

        //Read AVC Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_AVC))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_AVC);
            fgTmp = ((u2ReadShmUINT16(W_AVC_FLAG)&0x3) ? TRUE : FALSE);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_AVC_ENABLE);
            {
                UINT16 u2CurrVal[8];
                for (i=0; i<8; i++)
                {
                    AUD_DspGetAVCPara(i,&u2CurrVal[i],0);
                }

                vIO32WriteFldAlign(AUD_TOOL_AVC_TARGET_LVL, u2CurrVal[1], AUD_TOOL_AVC_FLD);
                vIO32WriteFldAlign(AUD_TOOL_AVC_MAX_GAIN_UP, u2CurrVal[3], AUD_TOOL_AVC_FLD);
                vIO32WriteFldAlign(AUD_TOOL_AVC_ADJUST_RATE, u2CurrVal[5], AUD_TOOL_AVC_FLD);
                vIO32WriteFldAlign(AUD_TOOL_AVC_UI_ADJUST_RATE, u2CurrVal[6], AUD_TOOL_AVC_FLD);
            }
        }

        //Read VS Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_VS))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_VS);
#ifndef CC_AUD_NEW_POST_MULTI_VS_SUPPORT
            fgTmp = (uReadShmUINT8(B_VSURRFLAG) ? TRUE : FALSE);
#else
            fgTmp = (uReadShmUINT8(W_VSURRFLAG) ? TRUE : FALSE);
#endif
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_VS_ENABLE);
            {
                AUD_VSURR_CFG_T rVSurrCfg;
                AUD_Surround_Get_Config(&rVSurrCfg);

                vIO32Write4B(AUD_TOOL_VS_CLARITY, rVSurrCfg.u4Clarity);
                vIO32Write4B(AUD_TOOL_VS_WIDTH, rVSurrCfg.u4Width);
                vIO32Write4B(AUD_TOOL_VS_LRGAIN, rVSurrCfg.u4LRGain);
                vIO32Write4B(AUD_TOOL_VS_CROSSTALK, rVSurrCfg.u4Xtalk);
                vIO32Write4B(AUD_TOOL_VS_OUTPUTGAIN, rVSurrCfg.u4OutputGain);
                vIO32Write4B(AUD_TOOL_VS_BASSGAIN, rVSurrCfg.u4BassGain);
                vIO32Write4B(AUD_TOOL_VS_FO, rVSurrCfg.u4Fo);
            }
        }

        //Read Bass Management Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_BASSMNG))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_BASSMNG);
            fgTmp = ((u2ReadShmUINT16(W_PROCMOD)&BYPASS_BASS_MANAGEMENT) ? FALSE : TRUE);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_BASSMNG_ENABLE);
            u4Tmp = u4ReadShmUINT32(D_SPKCFG);
            fgTmp = (((u4Tmp & (0x01<<12)) == (0x01<<12)) ? TRUE : FALSE);
            vIO32WriteFldAlign(AUD_TOOL_BASS_MANAGEMENT, fgTmp, AUD_TOOL_CENTER_SPKTYPE_LARGE);
            fgTmp = (((u4Tmp & (0x06<<12)) == (0x06<<12)) ? TRUE : FALSE);
            vIO32WriteFldAlign(AUD_TOOL_BASS_MANAGEMENT, fgTmp, AUD_TOOL_LR_SPKTYPE_LARGE);
            fgTmp = (((u4Tmp & (0x18<<12)) == (0x18<<12)) ? TRUE : FALSE);
            vIO32WriteFldAlign(AUD_TOOL_BASS_MANAGEMENT, fgTmp, AUD_TOOL_SLR_SPKTYPE_LARGE);
            fgTmp = (((u4Tmp & (0x3<<19)) == (0x3<<19)) ? TRUE : FALSE);
            vIO32WriteFldAlign(AUD_TOOL_BASS_MANAGEMENT, fgTmp, AUD_TOOL_CH910_SPKTYPE_LARGE);
            fgTmp = (((u4Tmp & (0x1<<5)) == (0x1<<5)) ? TRUE : FALSE);
            vIO32WriteFldAlign(AUD_TOOL_BASS_MANAGEMENT, fgTmp, AUD_TOOL_SUBWOOFER_ON);
            u4Tmp = u4ReadShmUINT32(D_CUTOFF_FREQ);
            vIO32WriteFldAlign(AUD_TOOL_BASS_MANAGEMENT, u4Tmp, AUD_TOOL_BASS_MNG_CUTOFF_FREQ);
        }
#ifdef CC_AUD_VBASS_SUPPORT
        //Read Virtual Bass Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_VBASS))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_VBASS);
            fgTmp = (uReadShmUINT8(B_VIR_BASS_ENABLE) ? TRUE : FALSE);
            vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_VBASS_ENABLE);
            u4Tmp = u4ReadShmUINT32(D_VIR_BASS_GAIN);
            vIO32Write4B(AUD_TOOL_VBASS_BASSGAIN, u4Tmp);
            u4Tmp = u4ReadShmUINT32(D_VIR_BASS_GAIN_2);
            vIO32Write4B(AUD_TOOL_VBASS_OUTPUTGAIN, u4Tmp);
            u4Tmp = AUD_GetVBassCutOff();
			if(u4Tmp == 0)
			{
			    u4Tmp = 50;
			}
			else if(u4Tmp == 1)
			{
			    u4Tmp = 80;
			}
			else if(u4Tmp == 2)
			{
			    u4Tmp = 110;
			}
			else if(u4Tmp == 3)
			{
			    u4Tmp = 140;
			}
			else if(u4Tmp == 4)
			{
			    u4Tmp = 170;
			}
			else
			{
			    u4Tmp = 200;
        	}
			vIO32Write4B(AUD_TOOL_VBASS_CUTOFF_FREQ, u4Tmp);
        }
#endif
#ifdef CC_AUD_SPEAKER_HEIGHT
		//Read Speaker Height Status
		if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_READ_SPKHEIGHT))
		{
			vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_READ_SPKHEIGHT);
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
			fgTmp = (uReadShmUINT8(B_SPH_ENABLE) ? TRUE : FALSE);
#else
			fgTmp = (uReadShmUINT8(B_SPH_MODE) ? TRUE : FALSE);
#endif
			vIO32WriteFldAlign(AUD_TOOL_CTRL_00, fgTmp, AUD_TOOL_CTRL_SPKHEIGHT_ENABLE);
#ifdef CC_AUD_SPEAKER_HEIGHT_MODE
			u4Tmp = uReadShmUINT8(B_SPH_MODE);
			if(u4Tmp == 1)
			{
				u4Tmp = 2;
			}
			else if(u4Tmp == 2)
			{
				u4Tmp = 4;
			}
			else if(u4Tmp == 3)
			{
				u4Tmp = 6;
			}
			else 
			{
				u4Tmp = 0;
			}
			vIO32Write4B(AUD_TOOL_SPKHEIGHT_BOOSTGAIN, u4Tmp);
#endif
		}
#endif
        //Write Bypass Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_BYPASS))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_BYPASS);
            {
                //Check post-proc bypass
                fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_ALL_BYPASS);
                AUD_DspBypassPostProcess(AUD_DEC_MAIN,fgTmp);

                //Check EQ enable
                fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_EQ_ENABLE);
                AUD_DspChannelEqEnable(AUD_DEC_MAIN, fgTmp);

                //Check Bass enable
                fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_BASS_ENABLE);
                AUD_DspBassBoostEnable(AUD_DEC_MAIN, fgTmp);

                //Check Treble enable
                fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_TREBLE_ENABLE);
                AUD_DspClearBoostEnable(AUD_DEC_MAIN, fgTmp);

                //Check limiter enable
                fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_LIMITER_ENABLE);
                AUD_DspSetLimiterMode(fgTmp ? IO32ReadFldAlign(AUD_TOOL_LIMITER,AUD_TOOL_LIMITER_MODE) : 0);

                //Check AVC enable
                fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_AVC_ENABLE);
                AUD_DspAVCEnable(AUD_DEC_MAIN, fgTmp);

                //Check VS enable
                fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_VS_ENABLE);
                AUD_DspVirtualSurroundEnalbe(AUD_DEC_MAIN, fgTmp);

                //Check Bass Management Enable
                fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_BASSMNG_ENABLE);
                AUD_DspBassMngEnalbe(AUD_DEC_MAIN, fgTmp);
				
#ifdef CC_AUD_VBASS_SUPPORT
				//Check Virtual Bass Enable
                fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_VBASS_ENABLE);
                AUD_DspSetVirBassEnable(fgTmp);
#endif
#ifdef CC_AUD_SPEAKER_HEIGHT
				//Check Speaker Height Enable
				fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_SPKHEIGHT_ENABLE);
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
				AUD_DspSetSPHEnable(fgTmp); 	
#else
				AUD_DspSetSPHMode(fgTmp); 
#endif
#endif
            }
        }

        //Write Volume Table Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_VOLUME_TBL))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_VOLUME_TBL);
            {
                u4Tmp = u4IO32Read4B(AUD_TOOL_VOL_MASTER);
                AUD_DspFineTuneVolume(AUD_DEC_MAIN, AUD_CH_ALL, u4Tmp);

                //Check Volume Table
                for (i=0;i<101;i++)
                {
                    AUD_DspSetVolTable(i, u4IO32Read4B(AUD_TOOL_VOL_TBL+(UINT32)i*4));
                }
            }
        }

        //Write Source Volume Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_SV))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_SV);
            {
                //Update Inpur Source Gain
                for (i=0; i<AUD_STREAM_FROM_NUM; i++)
                {
                    AUD_DspSetSrcVol(AUD_DEC_MAIN, (AUD_DEC_STREAM_FROM_T)i, (INT8)(IO32ReadFldAlign(AUD_TOOL_SRC_GAIN_OTHERS+i*4, AUD_TOOL_SRC_GAIN_FLD)));
                }
            }
        }

        //Write Channel Volume Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_CV))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_CV);
            {
                //Update Channel Volume Gain
                _VolumeCtrl(AUD_DEC_MAIN, AUD_CH_CENTER, u4IO32Read4B(AUD_TOOL_CH_VOL_C));
                _VolumeCtrl(AUD_DEC_MAIN, AUD_CH_FRONT_LEFT, u4IO32Read4B(AUD_TOOL_CH_VOL_L));
                _VolumeCtrl(AUD_DEC_MAIN, AUD_CH_FRONT_RIGHT, u4IO32Read4B(AUD_TOOL_CH_VOL_R));
                _VolumeCtrl(AUD_DEC_MAIN, AUD_CH_REAR_LEFT, u4IO32Read4B(AUD_TOOL_CH_VOL_LS));
                _VolumeCtrl(AUD_DEC_MAIN, AUD_CH_REAR_RIGHT, u4IO32Read4B(AUD_TOOL_CH_VOL_RS));
                _VolumeCtrl(AUD_DEC_MAIN, AUD_CH_BYPASS_LEFT, u4IO32Read4B(AUD_TOOL_CH_VOL_CH7));
                _VolumeCtrl(AUD_DEC_MAIN, AUD_CH_BYPASS_RIGHT, u4IO32Read4B(AUD_TOOL_CH_VOL_CH8));
                _VolumeCtrl(AUD_DEC_MAIN, AUD_CH_SUB_WOOFER, u4IO32Read4B(AUD_TOOL_CH_VOL_SUB));
                //_VolumeCtrl(AUD_DEC_MAIN, AUD_CH_CENTER, u4IO32Read4B(AUD_TOOL_CH_VOL_LFE));
                _VolumeCtrl(AUD_DEC_MAIN, AUD_CH_DMX_LEFT, u4IO32Read4B(AUD_TOOL_CH_VOL_CH9));
                _VolumeCtrl(AUD_DEC_MAIN, AUD_CH_DMX_RIGHT, u4IO32Read4B(AUD_TOOL_CH_VOL_CH10));
                _VolumeCtrl(AUD_DEC_AUX, AUD_CH_FRONT_LEFT, u4IO32Read4B(AUD_TOOL_CH_VOL_AUXL));
                _VolumeCtrl(AUD_DEC_AUX, AUD_CH_FRONT_RIGHT, u4IO32Read4B(AUD_TOOL_CH_VOL_AUXR));
            }
        }

        //Write Extra-Gain Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_EXGAIN))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_EXGAIN);
            {
                //Update Extra-Gain
                AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_FRONT_LEFT, (INT8)(IO32ReadFldAlign(AUD_TOOL_EXTRA_GAIN_00, AUD_TOOL_EXTRA_GAIN_L)));
                AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_FRONT_RIGHT, (INT8)(IO32ReadFldAlign(AUD_TOOL_EXTRA_GAIN_00, AUD_TOOL_EXTRA_GAIN_R)));
                AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_REAR_LEFT, (INT8)(IO32ReadFldAlign(AUD_TOOL_EXTRA_GAIN_00, AUD_TOOL_EXTRA_GAIN_LS)));
                AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_REAR_RIGHT, (INT8)(IO32ReadFldAlign(AUD_TOOL_EXTRA_GAIN_00, AUD_TOOL_EXTRA_GAIN_RS)));

                AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_CENTER, (INT8)(IO32ReadFldAlign(AUD_TOOL_EXTRA_GAIN_01, AUD_TOOL_EXTRA_GAIN_C)));
                AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_SUB_WOOFER, (INT8)(IO32ReadFldAlign(AUD_TOOL_EXTRA_GAIN_01, AUD_TOOL_EXTRA_GAIN_SUB)));
                AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_BYPASS_LEFT, (INT8)(IO32ReadFldAlign(AUD_TOOL_EXTRA_GAIN_01, AUD_TOOL_EXTRA_GAIN_CH7)));
                AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_BYPASS_RIGHT, (INT8)(IO32ReadFldAlign(AUD_TOOL_EXTRA_GAIN_01, AUD_TOOL_EXTRA_GAIN_CH8)));

                AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_DMX_LEFT, (INT8)(IO32ReadFldAlign(AUD_TOOL_EXTRA_GAIN_02, AUD_TOOL_EXTRA_GAIN_CH9)));
                AUD_DspChannelVolGain(AUD_DEC_MAIN, AUD_CH_DMX_RIGHT, (INT8)(IO32ReadFldAlign(AUD_TOOL_EXTRA_GAIN_02, AUD_TOOL_EXTRA_GAIN_CH10)));
                AUD_DspChannelVolGain(AUD_DEC_AUX, AUD_CH_FRONT_LEFT, (INT8)(IO32ReadFldAlign(AUD_TOOL_EXTRA_GAIN_02, AUD_TOOL_EXTRA_GAIN_AUXL)));
                AUD_DspChannelVolGain(AUD_DEC_AUX, AUD_CH_FRONT_RIGHT, (INT8)(IO32ReadFldAlign(AUD_TOOL_EXTRA_GAIN_02, AUD_TOOL_EXTRA_GAIN_AUXR)));
            }
        }

        //Update Channel Delay Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_CHDELAY))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_CHDELAY);
            {
                AUD_DspChannelDelay(IO32ReadFldAlign(AUD_TOOL_CHDELAY_00,AUD_TOOL_CHDELAY_L), AUD_CH_FRONT_LEFT, AUD_DEC_MAIN);
                AUD_DspChannelDelay(IO32ReadFldAlign(AUD_TOOL_CHDELAY_00,AUD_TOOL_CHDELAY_R), AUD_CH_FRONT_RIGHT, AUD_DEC_MAIN);
                AUD_DspChannelDelay(IO32ReadFldAlign(AUD_TOOL_CHDELAY_00,AUD_TOOL_CHDELAY_LS), AUD_CH_REAR_LEFT, AUD_DEC_MAIN);
                AUD_DspChannelDelay(IO32ReadFldAlign(AUD_TOOL_CHDELAY_00,AUD_TOOL_CHDELAY_RS), AUD_CH_REAR_RIGHT, AUD_DEC_MAIN);

                AUD_DspChannelDelay(IO32ReadFldAlign(AUD_TOOL_CHDELAY_01,AUD_TOOL_CHDELAY_C), AUD_CH_CENTER, AUD_DEC_MAIN);
                AUD_DspChannelDelay(IO32ReadFldAlign(AUD_TOOL_CHDELAY_01,AUD_TOOL_CHDELAY_SUB), AUD_CH_SUB_WOOFER, AUD_DEC_MAIN);
                AUD_DspChannelDelay(IO32ReadFldAlign(AUD_TOOL_CHDELAY_01,AUD_TOOL_CHDELAY_CH7), AUD_CH_BYPASS_LEFT, AUD_DEC_MAIN);
                AUD_DspChannelDelay(IO32ReadFldAlign(AUD_TOOL_CHDELAY_01,AUD_TOOL_CHDELAY_CH8), AUD_CH_BYPASS_RIGHT, AUD_DEC_MAIN);

                AUD_DspChannelDelay(IO32ReadFldAlign(AUD_TOOL_CHDELAY_02,AUD_TOOL_CHDELAY_CH9), AUD_CH_DMX_LEFT, AUD_DEC_MAIN);
                AUD_DspChannelDelay(IO32ReadFldAlign(AUD_TOOL_CHDELAY_02,AUD_TOOL_CHDELAY_CH10), AUD_CH_DMX_RIGHT, AUD_DEC_MAIN);
            }
        }

        //Write EQ Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_EQ))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_EQ);

            //Check EQ enable
            fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_EQ_ENABLE);
            AUD_DspChannelEqEnable(AUD_DEC_MAIN, fgTmp);
            {
                INT8  eqValue[CH_EQ_BAND_NO+1];
                int   i;
                AUD_EQ_TYPE_T eEqType;

                AUD_DspGetEqType(AUD_DEC_MAIN, &eEqType);
                if (eEqType == AUD_EQ_OFF)
                {
                    eEqType = AUD_EQ_CUST1;
                }

                eqValue[0] = 0;
                for (i = 1; i <= CH_EQ_BAND_NO; i++)
                {
                    eqValue[i] = (INT8)(IO32ReadFldAlign(AUD_TOOL_EQ_BAND1+(i-1)*4,AUD_TOOL_EQ_BAND_FLD));
                }
                AUD_DspChSetEqTable(AUD_DEC_MAIN, eEqType, eqValue); //user defined EQ
                AUD_DspChEqLoadPreset(AUD_DEC_MAIN, eEqType);
            }
        }

        //Write Bass Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_BASS))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_BASS);

            //Check Bass enable
            fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_BASS_ENABLE);
            AUD_DspBassBoostEnable(AUD_DEC_MAIN, fgTmp);
            {
                AUD_DspBassBoostGain(AUD_DEC_MAIN, IO32ReadFldAlign(AUD_TOOL_BASS, AUD_TOOL_SBASS_FLD));
            }
        }

        //Write Treble Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_TREBLE))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_TREBLE);

            //Check Treble enable
            fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_TREBLE_ENABLE);
            AUD_DspClearBoostEnable(AUD_DEC_MAIN, fgTmp);
            {
                AUD_DspClearBoostGain(AUD_DEC_MAIN, IO32ReadFldAlign(AUD_TOOL_TREBLE, AUD_TOOL_SBASS_FLD));
            }
        }

        //Write Limiter Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_LIMITER))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_LIMITER);

            //Check limiter enable
            fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_LIMITER_ENABLE);
            AUD_DspSetLimiterMode(fgTmp ? IO32ReadFldAlign(AUD_TOOL_LIMITER,AUD_TOOL_LIMITER_MODE) : 0);
            {
                AUD_DspSetLimiterThreshold(IO32ReadFldAlign(AUD_TOOL_LIMITER,AUD_TOOL_LIMITER_THRES));
            }
        }

        //Write AVC Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_AVC))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_AVC);

            //Check AVC enable
            fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_AVC_ENABLE);
            AUD_DspAVCEnable(AUD_DEC_MAIN, fgTmp);
            {
                AUD_DspChangeAVCPara(1,IO32ReadFldAlign(AUD_TOOL_AVC_TARGET_LVL, AUD_TOOL_AVC_FLD), AUD_DEC_MAIN);
                AUD_DspChangeAVCPara(3,IO32ReadFldAlign(AUD_TOOL_AVC_MAX_GAIN_UP, AUD_TOOL_AVC_FLD), AUD_DEC_MAIN);
                AUD_DspChangeAVCPara(5,IO32ReadFldAlign(AUD_TOOL_AVC_ADJUST_RATE, AUD_TOOL_AVC_FLD), AUD_DEC_MAIN);
                AUD_DspChangeAVCPara(6,IO32ReadFldAlign(AUD_TOOL_AVC_UI_ADJUST_RATE, AUD_TOOL_AVC_FLD), AUD_DEC_MAIN);
            }
        }

        //Write VS Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_VS))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_VS);

            //Check VS enable
            fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_VS_ENABLE);
            AUD_DspVirtualSurroundEnalbe(AUD_DEC_MAIN, fgTmp);
            {
                AUD_Surround_Set_Config(AUD_SUR_CLARITY, u4IO32Read4B(AUD_TOOL_VS_CLARITY));
                AUD_Surround_Set_Config(AUD_SUR_WIDTH, u4IO32Read4B(AUD_TOOL_VS_WIDTH));
                AUD_Surround_Set_Config(AUD_SUR_LRGAIN, u4IO32Read4B(AUD_TOOL_VS_LRGAIN));
                AUD_Surround_Set_Config(AUD_SUR_XTALK, u4IO32Read4B(AUD_TOOL_VS_CROSSTALK));
                AUD_Surround_Set_Config(AUD_SUR_OUTPUT_GAIN, u4IO32Read4B(AUD_TOOL_VS_OUTPUTGAIN));
                AUD_Surround_Set_Config(AUD_SUR_BASS_GAIN, u4IO32Read4B(AUD_TOOL_VS_BASSGAIN));
                AUD_Surround_Set_Config(AUD_SUR_FO, u4IO32Read4B(AUD_TOOL_VS_FO));
            }
        }

        //Write Bass Management Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_BASSMNG))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_BASSMNG);

            fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_BASSMNG_ENABLE);
            AUD_DspBassMngEnalbe(AUD_DEC_MAIN, fgTmp);

            AUD_DspSpeakerLargeSmallConfig(AUD_DEC_MAIN,
                                            IO32ReadFldAlign(AUD_TOOL_BASS_MANAGEMENT, AUD_TOOL_LR_SPKTYPE_LARGE),
                                            IO32ReadFldAlign(AUD_TOOL_BASS_MANAGEMENT, AUD_TOOL_CENTER_SPKTYPE_LARGE),
                                            IO32ReadFldAlign(AUD_TOOL_BASS_MANAGEMENT, AUD_TOOL_SLR_SPKTYPE_LARGE),
                                            IO32ReadFldAlign(AUD_TOOL_BASS_MANAGEMENT, AUD_TOOL_CH910_SPKTYPE_LARGE));

            AUD_DspSpeakerSubwooferEnable(AUD_DEC_MAIN, IO32ReadFldAlign(AUD_TOOL_BASS_MANAGEMENT, AUD_TOOL_SUBWOOFER_ON));
            AUD_DspSpkSizeCfg(AUD_DEC_MAIN, IO32ReadFldAlign(AUD_TOOL_BASS_MANAGEMENT, AUD_TOOL_BASS_MNG_CUTOFF_FREQ));
        }

#ifdef CC_AUD_VBASS_SUPPORT
		//Write Virtual Bass Status
        if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_VBASS))
        {
            vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_VBASS);

			fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_VBASS_ENABLE);
			AUD_DspSetVirBassEnable(fgTmp);			
			AUD_DspSetVirBassCfg(AUD_DEC_MAIN, 0, u4IO32Read4B(AUD_TOOL_VBASS_OUTPUTGAIN),u4IO32Read4B(AUD_TOOL_VBASS_BASSGAIN));
			u4Tmp = u4IO32Read4B(AUD_TOOL_VBASS_CUTOFF_FREQ);
			if(u4Tmp == 50)
			{
				u4Tmp = 0;
			}
			else if(u4Tmp == 80)
			{
				u4Tmp = 1;
			}
			else if(u4Tmp == 110)
			{
				u4Tmp = 2;
			}
			else if(u4Tmp == 140)
			{
				u4Tmp = 3;
			}
			else if(u4Tmp == 170)
			{
				u4Tmp = 4;
			}
			else
			{
				u4Tmp = 5;
			}
			AUD_SetVBassCutOff(u4Tmp);
            AUD_DspSetVirBassEnable(FALSE);    
            AUD_DspSetVirBassEnable(TRUE);  
        }
#endif
#ifdef CC_AUD_SPEAKER_HEIGHT
		//Write  Speaker Height  Status
		if (IO32ReadFldAlign(AUD_TOOL_CTRL_01, AUD_TOOL_CTRL_TRIG_WRITE_SPKHEIGHT))
		{
			vIO32WriteFldAlign(AUD_TOOL_CTRL_01, 0, AUD_TOOL_CTRL_TRIG_WRITE_SPKHEIGHT);		
			fgTmp = IO32ReadFldAlign(AUD_TOOL_CTRL_00, AUD_TOOL_CTRL_SPKHEIGHT_ENABLE);
#ifndef CC_AUD_SPEAKER_HEIGHT_MODE
			AUD_DspSetSPHEnable(fgTmp); 	
#else
            AUD_DspSetSPHMode(fgTmp); 
#endif	
#ifdef CC_AUD_SPEAKER_HEIGHT_MODE
			u4Tmp = u4IO32Read4B(AUD_TOOL_SPKHEIGHT_BOOSTGAIN);
			if(u4Tmp == 2)
			{
				u4Tmp = 1;
			}
			else if(u4Tmp == 4)
			{
				u4Tmp = 2;
			}
			else if(u4Tmp == 6)
			{
				u4Tmp = 3;
			}
			else
			{
				u4Tmp = 0;
			}
			vWriteShmUINT8(B_SPH_MODE, u4Tmp);
			AUD_DspSetSPHMode(u4Tmp);
#endif
		}
#endif
        x_thread_delay(AUD_TOOL_THREAD_PERIOD);
    }
}
#endif

