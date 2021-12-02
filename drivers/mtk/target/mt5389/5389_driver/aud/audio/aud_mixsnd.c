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
#include "aud_notify.h"
#include "aud_drv.h"
#include "drv_adsp.h"
#include "dsp_shm.h"
#include "drv_common.h"
#include "x_aud_dec.h"

#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
#include "oss_adap.h"
#endif

LINT_EXT_HEADER_END

#ifdef CC_AUD_DSP_SUPPORT_MIXSOUND_MEMPLAY

//---------------------------------------------------------------------------
// Configurations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Constant definitions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Type definitions
//---------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Imported variables
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Imported functions
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static function forward declarations
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Static variables
//---------------------------------------------------------------------------

static BOOL _fgForceStopMixSndDma = FALSE;
static HANDLE_T _hAudFeedMixSndThread = NULL_HANDLE;
static HANDLE_T _hAudFeedAlsaMixSndThread = NULL_HANDLE;

//---------------------------------------------------------------------------
// Function Implementation
//---------------------------------------------------------------------------
  
//#define MAX_AUD_MIXSND_STREAM_NUM 8 ==> Move to dsp_common.h
#define MAX_AUD_MIXSND_CLIP_NUM 9
#define MIXSND_TRANSFER_SZ             (UINT32)(0x00000100)
//#define MIXSND_OUTPUT_DBG
//#define SUPPORT_6X_OPT_FILTER

#ifdef SUPPORT_6X_OPT_FILTER
#define UP6_FIR_TAP_NO 57

INT16 fir_up6[UP6_FIR_TAP_NO] = 
{ 
	-590, 827, 1018, 1245, 1273, 970, 330, -513,
	-1313, -1780, -1676, -923, 342, 1765, 2857, 3138,
	2314, 413, -2145, -4606, -6045, -5613, -2793, 2400,
	9357, 16957, 23810, 28572, 30277, 28572, 23810, 16957,
	9357, 2400, -2793, -5613, -6045, -4606, -2145, 413,
	2314, 3138, 2857, 1765, 342, -923, -1676, -1780,
	-1313, -513, 330, 970, 1273, 1245, 1018, 827
	-590
};
INT16 history_buf_up6[MAX_AUD_MIXSND_STREAM_NUM][2][UP6_FIR_TAP_NO+1];

void filter_init_up6(UINT8 u1StreamID)
{
    int i;
    for (i=0;i<UP6_FIR_TAP_NO+1;i++)
    {
        history_buf_up6[u1StreamID][0][i] = 0;
        history_buf_up6[u1StreamID][1][i] = 0;
    }
}

INT16 do_filter_up6(UINT8 u1StreamID, INT16 cur_sample,int channel)
{
    int j;
	
    INT16 ret = 0;
    INT32 tmp = 0;
    history_buf_up6[u1StreamID][channel][0] = cur_sample;
    for (j=0;j<UP6_FIR_TAP_NO;j++)
    {
        tmp += (((INT32)history_buf_up6[u1StreamID][channel][UP6_FIR_TAP_NO-1-j] * (INT32)fir_up6[j])>>2);
        history_buf_up6[u1StreamID][channel][UP6_FIR_TAP_NO-j] = history_buf_up6[u1StreamID][channel][UP6_FIR_TAP_NO-1-j];
    }
	
    ret = (INT16)(((tmp + 0x0000)>>13) & 0xffff);	
    return ret;
}
#endif

#ifdef MIXSOUND_MIPS_MEASURE
UINT32 _AUD_ReadPsrStc1 (void);
void _AUD_SetPsrStc1 (UINT32 u4Val);
#endif

typedef struct
{
    UINT32 u4Addr;
    UINT32 u4Size;
    UINT32 u4Gain;
    UINT32 u4SampleRate;    
    UINT8 u1StereoOnOff; //0: mono, 1: stereo
    UINT8 u1BitDepth; //16,8
} AUD_MIXSND_CLIP_TYPE_T;

typedef struct
{
    UINT8 fgEnable;
    UINT8 fgStop;
    UINT8 fgPause;
    UINT8 fgRepeat;

    UINT8 u1ClipReadIndx;
    UINT8 u1ClipWriteIndx;
    UINT8 u1MixSndStereoOnOff;
    UINT8 u1BitDepth;

    UINT32 u4SampleRate;
    UINT32 u4AFifoSA;
    UINT32 u4AFifoEA;
    UINT32 u4AFifoSZ;
    UINT32 u4DesAddr;
    UINT32 u4TransferSZ;
    UINT32 u4TransferSZ2;

    UINT32 u4StreamAddr;    //input addr    
    UINT32 u4Residual;      //input size

    INT16  i2DelayBuf[2];    
} AUD_MIXSND_STREAM_TYPE_T;

AUD_MIXSND_STREAM_TYPE_T _rAudMixSndStream[MAX_AUD_MIXSND_STREAM_NUM];
AUD_MIXSND_CLIP_TYPE_T _rAudMixSndClip[MAX_AUD_MIXSND_STREAM_NUM][MAX_AUD_MIXSND_CLIP_NUM];

#ifdef ALSA_MIXSND_PATH
typedef struct
{
    UINT32 u4SA;
    UINT32 u4EA;
    UINT32 u4SZ;
    UINT32 u4RP;
    UINT32 u4WP;
    UINT32 u4Latency;
} AUD_MIXSND_RINGFIFO_TYPE_T;

AUD_MIXSND_RINGFIFO_TYPE_T _rAudMixSndRingFifo;
UINT32 u4Latency = 0; //0x1000; //for AV sync issue
BOOL fgMixSndClipMute = FALSE;

UINT32 u4GetMixSndRingFifoRP(void)
{
    return _rAudMixSndRingFifo.u4RP;
}

void vSetMixSndRingFifoWP(UINT32 u4WritePtr)
{
    _rAudMixSndRingFifo.u4WP = u4WritePtr;
}
#endif

void do_SRC_S16LE(UINT8 u1StreamId,
                INT16* input_buf, INT16* output_buf, 
                UINT32 input_sample, UINT32 output_sample, 
                UINT32 length, UINT8 mono_stereo)
{
    UINT32 i,output_length;
    UINT32 u4SRCCoeff;    
    UINT32 u4Integer;
    UINT32 u4Decimal;

    output_length = (((length * output_sample / input_sample)+1) & 0xfffffffe);
    u4SRCCoeff = (0xffff * (input_sample-1) / (output_sample-1));
    u4Integer = 0;
    u4Decimal = 0;
    i = 0;

    if (length == output_length)
    {
        // 1x upsample
        if (mono_stereo)
        {
            for (i=0;i<output_length;i++)
            {
                output_buf[i] = input_buf[i];
            }
        }
        else
        {
            for (i=0;i<output_length;i++)
            {
                output_buf[i*2] = input_buf[i];
                output_buf[i*2+1] = input_buf[i];
            }
        }
    }
    else
    {
#ifdef SUPPORT_6X_OPT_FILTER
        if (input_sample == 8000)
        {
            INT16 sample_l, sample_r;
            
            if (mono_stereo == 0)
            {
                //mono              
                for (i=0;i<length;i++)
                {                   
                    sample_l = input_buf[i];
                    output_buf[i*12+0] = do_filter_up6(u1StreamId,sample_l,0);
                    output_buf[i*12+1] = output_buf[i*12+0];
                    output_buf[i*12+2] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+3] = output_buf[i*12+2];
                    output_buf[i*12+4] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+5] = output_buf[i*12+4];
                    output_buf[i*12+6] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+7] = output_buf[i*12+6];
                    output_buf[i*12+8] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+9] = output_buf[i*12+8];
                    output_buf[i*12+10] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+11] = output_buf[i*12+10];
                }
            }
            else
            {
                //stereo                
                for (i=0;i<length/2;i++)
                {
                    sample_l = input_buf[i*2+0];
                    sample_r = input_buf[i*2+1];
                    output_buf[i*12+0] = do_filter_up6(u1StreamId,sample_l,0);
                    output_buf[i*12+1] = do_filter_up6(u1StreamId,sample_r,1);
                    output_buf[i*12+2] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+3] = do_filter_up6(u1StreamId,0,1);
                    output_buf[i*12+4] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+5] = do_filter_up6(u1StreamId,0,1);
                    output_buf[i*12+6] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+7] = do_filter_up6(u1StreamId,0,1);
                    output_buf[i*12+8] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+9] = do_filter_up6(u1StreamId,0,1);
                    output_buf[i*12+10] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+11] = do_filter_up6(u1StreamId,0,1);                  
                }
            }
        }
        else
#endif
        {    
            if (mono_stereo)
            {
                while (u4Decimal < 0x10000)
                {
                    #if 1
                    output_buf[i] = (((((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[0] * (0x10000 - u4Decimal))) + 
                                    (((INT32)input_buf[0] * u4Decimal)))>>16);
                    output_buf[i+1] = (((((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[1] * (0x10000 - u4Decimal))) + 
                                      (((INT32)input_buf[1] * u4Decimal)))>>16);
                    #else
                    output_buf[i] = (((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[0] * (0x10000 - u4Decimal))>>16) + 
                                    (((INT32)input_buf[0] * u4Decimal)>>16);
                    output_buf[i+1] = (((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[1] * (0x10000 - u4Decimal))>>16) + 
                                      (((INT32)input_buf[1] * u4Decimal)>>16);
                    #endif
                    u4Decimal += u4SRCCoeff;
                    i += 2;
                }
                u4Decimal -= 0x10000;

                //Stereo
                for (;i<output_length;i+=2)
                {
                    #if 1
                    output_buf[i] = (((((INT32)input_buf[u4Integer] * (0x10000 - u4Decimal))) + 
                                    (((INT32)input_buf[u4Integer+2] * u4Decimal)))>>16);
                    output_buf[i+1] = (((((INT32)input_buf[u4Integer+1] * (0x10000 - u4Decimal))) + 
                                      (((INT32)input_buf[u4Integer+3] * u4Decimal)))>>16);
                    #else
                    output_buf[i] = (((INT32)input_buf[u4Integer] * (0x10000 - u4Decimal))>>16) + 
                                    (((INT32)input_buf[u4Integer+2] * u4Decimal)>>16);
                    output_buf[i+1] = (((INT32)input_buf[u4Integer+1] * (0x10000 - u4Decimal))>>16) + 
                                      (((INT32)input_buf[u4Integer+3] * u4Decimal)>>16);
                    #endif                
                    u4Decimal += u4SRCCoeff;
                    if (u4Decimal >= 0x10000)
                    {
                        u4Integer += 2;
                        u4Decimal -= 0x10000;
                    }
                }

                _rAudMixSndStream[u1StreamId].i2DelayBuf[0] = input_buf[length-2];
                _rAudMixSndStream[u1StreamId].i2DelayBuf[1] = input_buf[length-1];            
            }
            else
            {
                while (u4Decimal < 0x10000)
                {
                    #if 1
                    output_buf[i*2] = (((((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[0] * (0x10000 - u4Decimal))) + 
                                      (((INT32)input_buf[0] * u4Decimal)))>>16);
                    #else
                    output_buf[i*2] = (((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[0] * (0x10000 - u4Decimal))>>16) + 
                                      (((INT32)input_buf[0] * u4Decimal)>>16);
                    #endif
                    output_buf[i*2+1] = output_buf[i*2];
                    u4Decimal += u4SRCCoeff;
                    i++;
                }
                u4Decimal -= 0x10000;
            
                for (;i<output_length;i++)
                {
                    #if 1
                    output_buf[i*2] = (((((INT32)input_buf[u4Integer] * (0x10000 - u4Decimal))) + 
                                      (((INT32)input_buf[u4Integer+1] * u4Decimal)))>>16);
                    #else
                    output_buf[i*2] = (((INT32)input_buf[u4Integer] * (0x10000 - u4Decimal))>>16) + 
                                      (((INT32)input_buf[u4Integer+1] * u4Decimal)>>16);
                    #endif
                    output_buf[i*2+1] = output_buf[i*2];
                    u4Decimal += u4SRCCoeff;
                    if (u4Decimal >= 0x10000)
                    {
                        u4Integer++;
                        u4Decimal -= 0x10000;
                    }
                }    
                _rAudMixSndStream[u1StreamId].i2DelayBuf[0] = input_buf[length-1];      
            }       
        }
    }
}

#define U8toS16(val) (((INT16)(val)-128)<<8)

void do_SRC_U8(UINT8 u1StreamId,
                UINT8* input_buf, INT16* output_buf, 
                UINT32 input_sample, UINT32 output_sample, 
                UINT32 length, UINT8 mono_stereo)
{
    UINT32 i,output_length;
    UINT32 u4SRCCoeff;    
    UINT32 u4Integer;
    UINT32 u4Decimal;

    output_length = (((length * output_sample / input_sample)+1) & 0xfffffffe);
    u4SRCCoeff = (0xffff * (input_sample-1) / (output_sample-1));
    u4Integer = 0;
    u4Decimal = 0;
    i = 0;

    if (length == output_length)
    {
        // 1x upsample
        if (mono_stereo)
        {
            for (i=0;i<output_length;i++)
            {
                output_buf[i] = U8toS16(input_buf[i]);
            }
        }
        else
        {
            for (i=0;i<output_length;i++)
            {
                output_buf[i*2] = U8toS16(input_buf[i]);
                output_buf[i*2+1] = U8toS16(input_buf[i]);
            }
        }
    }
    else
    {
#ifdef SUPPORT_6X_OPT_FILTER
        if (input_sample == 8000)
        {
            INT16 sample_l, sample_r;
            
            if (mono_stereo == 0)
            {
                //mono              
                for (i=0;i<length;i++)
                {                   
                    sample_l = U8toS16(input_buf[i]);
                    output_buf[i*12+0] = do_filter_up6(u1StreamId,sample_l,0);
                    output_buf[i*12+1] = output_buf[i*12+0];
                    output_buf[i*12+2] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+3] = output_buf[i*12+2];
                    output_buf[i*12+4] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+5] = output_buf[i*12+4];
                    output_buf[i*12+6] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+7] = output_buf[i*12+6];
                    output_buf[i*12+8] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+9] = output_buf[i*12+8];
                    output_buf[i*12+10] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+11] = output_buf[i*12+10];
                }
            }
            else
            {
                //stereo                
                for (i=0;i<length/2;i++)
                {
                    sample_l = U8toS16(input_buf[i*2+0]);
                    sample_r = U8toS16(input_buf[i*2+1]);
                    output_buf[i*12+0] = do_filter_up6(u1StreamId,sample_l,0);
                    output_buf[i*12+1] = do_filter_up6(u1StreamId,sample_r,1);
                    output_buf[i*12+2] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+3] = do_filter_up6(u1StreamId,0,1);
                    output_buf[i*12+4] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+5] = do_filter_up6(u1StreamId,0,1);
                    output_buf[i*12+6] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+7] = do_filter_up6(u1StreamId,0,1);
                    output_buf[i*12+8] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+9] = do_filter_up6(u1StreamId,0,1);
                    output_buf[i*12+10] = do_filter_up6(u1StreamId,0,0);
                    output_buf[i*12+11] = do_filter_up6(u1StreamId,0,1);                  
                }
            }
        }
        else
#endif
        {    
            if (mono_stereo)
            {
                while (u4Decimal < 0x10000)
                {
                    #if 1
                    output_buf[i] = (((((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[0] * (0x10000 - u4Decimal))) + 
                                    (((INT32)U8toS16(input_buf[0]) * u4Decimal)))>>16);
                    output_buf[i+1] = (((((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[1] * (0x10000 - u4Decimal))) + 
                                      (((INT32)U8toS16(input_buf[1]) * u4Decimal)))>>16);
                    #else
                    output_buf[i] = (((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[0] * (0x10000 - u4Decimal))>>16) + 
                                    (((INT32)U8toS16(input_buf[0]) * u4Decimal)>>16);
                    output_buf[i+1] = (((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[1] * (0x10000 - u4Decimal))>>16) + 
                                      (((INT32)U8toS16(input_buf[1]) * u4Decimal)>>16);
                    #endif
                    u4Decimal += u4SRCCoeff;
                    i += 2;
                }
                u4Decimal -= 0x10000;

                //Stereo
                for (;i<output_length;i+=2)
                {
                    #if 1
                    output_buf[i] = (((((INT32)U8toS16(input_buf[u4Integer]) * (0x10000 - u4Decimal))) + 
                                    (((INT32)U8toS16(input_buf[u4Integer+2]) * u4Decimal)))>>16);
                    output_buf[i+1] = (((((INT32)U8toS16(input_buf[u4Integer+1]) * (0x10000 - u4Decimal))) + 
                                      (((INT32)U8toS16(input_buf[u4Integer+3]) * u4Decimal)))>>16);
                    #else
                    output_buf[i] = (((INT32)U8toS16(input_buf[u4Integer]) * (0x10000 - u4Decimal))>>16) + 
                                    (((INT32)U8toS16(input_buf[u4Integer+2]) * u4Decimal)>>16);
                    output_buf[i+1] = (((INT32)U8toS16(input_buf[u4Integer+1]) * (0x10000 - u4Decimal))>>16) + 
                                      (((INT32)U8toS16(input_buf[u4Integer+3]) * u4Decimal)>>16);
                    #endif                
                    u4Decimal += u4SRCCoeff;
                    if (u4Decimal >= 0x10000)
                    {
                        u4Integer += 2;
                        u4Decimal -= 0x10000;
                    }
                }

                _rAudMixSndStream[u1StreamId].i2DelayBuf[0] = U8toS16(input_buf[length-2]);
                _rAudMixSndStream[u1StreamId].i2DelayBuf[1] = U8toS16(input_buf[length-1]);
            }
            else
            {
                while (u4Decimal < 0x10000)
                {
                    #if 1
                    output_buf[i*2] = (((((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[0] * (0x10000 - u4Decimal))) + 
                                      (((INT32)U8toS16(input_buf[0]) * u4Decimal)))>>16);
                    #else
                    output_buf[i*2] = (((INT32)_rAudMixSndStream[u1StreamId].i2DelayBuf[0] * (0x10000 - u4Decimal))>>16) + 
                                      (((INT32)U8toS16(input_buf[0]) * u4Decimal)>>16);
                    #endif
                    output_buf[i*2+1] = output_buf[i*2];
                    u4Decimal += u4SRCCoeff;
                    i++;
                }
                u4Decimal -= 0x10000;
            
                for (;i<output_length;i++)
                {
                    #if 1
                    output_buf[i*2] = (((((INT32)U8toS16(input_buf[u4Integer]) * (0x10000 - u4Decimal))) + 
                                      (((INT32)U8toS16(input_buf[u4Integer+1]) * u4Decimal)))>>16);
                    #else
                    output_buf[i*2] = (((INT32)U8toS16(input_buf[u4Integer]) * (0x10000 - u4Decimal))>>16) + 
                                      (((INT32)U8toS16(input_buf[u4Integer+1]) * u4Decimal)>>16);
                    #endif
                    output_buf[i*2+1] = output_buf[i*2];
                    u4Decimal += u4SRCCoeff;
                    if (u4Decimal >= 0x10000)
                    {
                        u4Integer++;
                        u4Decimal -= 0x10000;
                    }
                }    
                _rAudMixSndStream[u1StreamId].i2DelayBuf[0] = U8toS16(input_buf[length-1]);
            }       
        }
    }
}

//testing code

extern UINT32 AUD_DspGetMixSndReadPtr(UINT8 u1StreamID);
extern void AUD_DspSetMixSndWritePtr(UINT8 u1StreamID,UINT32 u4WritePtr);

BOOL fgIsSoftTransferable(UINT8 u1StreamID)
{
    UINT32 u4ReadPtr;
    UINT32 u4WritePtr;

    u4WritePtr = ((_rAudMixSndStream[u1StreamID].u4DesAddr + _rAudMixSndStream[u1StreamID].u4TransferSZ2) >= _rAudMixSndStream[u1StreamID].u4AFifoEA) ?
                  ((_rAudMixSndStream[u1StreamID].u4DesAddr + _rAudMixSndStream[u1StreamID].u4TransferSZ2) - (_rAudMixSndStream[u1StreamID].u4AFifoEA-_rAudMixSndStream[u1StreamID].u4AFifoSA)) :
                  (_rAudMixSndStream[u1StreamID].u4DesAddr + _rAudMixSndStream[u1StreamID].u4TransferSZ2);

    //Check Read Ptr First
    u4ReadPtr = AUD_DspGetMixSndReadPtr(u1StreamID);

    if (u4WritePtr < _rAudMixSndStream[u1StreamID].u4DesAddr)
    {
        //  u4WritePtr  u4DesAddr
        if ((u4ReadPtr > u4WritePtr) && (u4ReadPtr <= _rAudMixSndStream[u1StreamID].u4DesAddr))
        {
            return TRUE;
        }
    }
    else
    {
        //  u4DesAddr   u4WritePtr
        if ((u4ReadPtr > u4WritePtr) || (u4ReadPtr <= _rAudMixSndStream[u1StreamID].u4DesAddr))
        {
            return TRUE;
        }
    }
    return FALSE;
}

void vSoftTransfer(UINT32 u4SrcAddr, UINT32 u4Size,
                       UINT32 u4DstAddr, UINT32 u4DstFifoStart, UINT32 u4DstFifoEnd,
                       UINT8 u1StreamID)
{
    UINT32 u4WritePtr;

    // Clear interrupt
    //AUD_DspSetMixSndWritePtr(u1StreamID, u4DstAddr);
    u4WritePtr = ((u4DstAddr + u4Size) >= u4DstFifoEnd) ?
                  ((u4DstAddr + u4Size) - (u4DstFifoEnd-u4DstFifoStart)) :
                  (u4DstAddr + u4Size);

    if (u4WritePtr < u4DstAddr)
    {
        if (fgMixSndClipMute)
        {
            x_memset((VOID*)(VIRTUAL(u4DstAddr)),
                        0,
                        (u4DstFifoEnd-u4DstAddr));
            x_memset((VOID*)(VIRTUAL(u4DstFifoStart)),
                        0,
                        (u4Size-(u4DstFifoEnd-u4DstAddr)));
        }
        else
        {
            x_memcpy((VOID*)(VIRTUAL(u4DstAddr)),
                        (VOID*)(VIRTUAL(u4SrcAddr)),
                        (u4DstFifoEnd-u4DstAddr));
            x_memcpy((VOID*)(VIRTUAL(u4DstFifoStart)),
                        (VOID*)(VIRTUAL(u4SrcAddr+(u4DstFifoEnd-u4DstAddr))),
                        (u4Size-(u4DstFifoEnd-u4DstAddr)));
        }

        DSP_FlushInvalidateDCacheFree(u4DstAddr, (u4DstFifoEnd-u4DstAddr));
        DSP_FlushInvalidateDCacheFree(u4DstFifoStart, (u4Size-(u4DstFifoEnd-u4DstAddr)));
    }
    else
    {
        if (fgMixSndClipMute)
        {
            x_memset((VOID*)(VIRTUAL(u4DstAddr)),
                    0,
                    u4Size);        
        }
        else
        {
            x_memcpy((VOID*)(VIRTUAL(u4DstAddr)),
                    (VOID*)(VIRTUAL(u4SrcAddr)),
                    u4Size);
        }

        DSP_FlushInvalidateDCacheFree(u4DstAddr, u4Size);
    }

    if ((u1StreamID == ALSA_MIXSND_STREAM_ID) && (_rAudMixSndRingFifo.u4Latency))
    {
        if (_rAudMixSndRingFifo.u4Latency > u4Size)
        {
            _rAudMixSndRingFifo.u4Latency -= u4Size;
            return;
        }
        else
        {
            Printf("DSP start decoding\n");
            _rAudMixSndRingFifo.u4Latency = 0;
        }           
    }
    AUD_DspSetMixSndWritePtr(u1StreamID, (u4WritePtr&0xffffff00));
}

BOOL fgAudFeedMixSndThreadEnable = FALSE; //for CMPB integration
#ifdef MIXSND_OUTPUT_DBG
//POLAR_DEBUG
UINT32 u4MixSndDumpBufAddr = 0;
UINT32 u4MixSndDumpSize = 0;
UINT8 u1DumpLocation = 0; // 0: Before SW ASRC 1: After SW ASRC
//POLAR_DEBUG
UINT32 u4OutputBuf = 0; //TEST
UINT32 u4TotalOutputSize = 0;

//POLAR_DEBUG
void vSetMixSndDumpInfo(UINT32 u4Addr, UINT32 u4Size)
{
    u4MixSndDumpBufAddr = u4Addr;
    u4MixSndDumpSize = u4Size;
    u4OutputBuf = u4Addr;
    u4TotalOutputSize = 0;
    Printf("Set MixSnd Dump Start Addr: %x Size: %x\n", u4MixSndDumpBufAddr, u4MixSndDumpSize);    
}

void vResetMixSndDumpInfo(void)
{
    u4OutputBuf = u4MixSndDumpBufAddr;
    u4TotalOutputSize = 0;
    Printf("Reset MixSnd Dump Start Addr: %x Size: %x\n", u4MixSndDumpBufAddr, u4MixSndDumpSize);        
}
//POLAR_DEBUG
#endif

extern void _AUD_DspMixSndControl2(UINT16 u2Mode); //FLASHLITE_CONFLICT

#ifdef MIXSOUND_MIPS_MEASURE
UINT32 u4MixSndSendCnt = 0;
UINT32 u4MixSndMIPS = 0;

void vMixSndMIPSQuery(void)
{
    Printf("u4MixSndSendCnt: %d\n", u4MixSndSendCnt);
    Printf("u4MixSndMIPS: %d\n", u4MixSndMIPS);    
    Printf("Cycle: %d\n", (u4MixSndSendCnt == 0) ? 0 : u4MixSndMIPS/u4MixSndSendCnt);
}

void vMixSndMIPSReset(void)
{
    u4MixSndSendCnt = 0;
    u4MixSndMIPS = 0;
}
#endif

static void _AudFeedMixSndThread(const void* pvArg)
{
    int i;
    UINT32 u4AFifoTA;
    #ifndef NEW_MIXSOUND
    UINT32 u4GainOld; //FLASHLITE_CONFLICT
    UINT16 u2MixSndCfg; //FLASHLITE_CONFLICT
    #endif
    AudDecNfyFct pfAudDecNfy = NULL;

    BOOL fgDataEmpty = FALSE;
    #ifdef MIXSND_OUTPUT_DBG
    BOOL fgFifoFull = FALSE;
    #endif

    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy));

    Printf("[FeedMixSndThread] Init - 201108011600\n");

    x_memset((VOID*)VIRTUAL((UINT32)&_rAudMixSndStream), 0, sizeof(_rAudMixSndStream));
    x_memset((VOID*)VIRTUAL((UINT32)&_rAudMixSndClip), 0, sizeof(_rAudMixSndClip));
    #ifdef ALSA_MIXSND_PATH
    x_memset((VOID*)VIRTUAL((UINT32)&_rAudMixSndRingFifo), 0, sizeof(_rAudMixSndRingFifo));
    #endif
    for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
    {
        _rAudMixSndStream[i].fgEnable = 0;
        _rAudMixSndStream[i].fgStop = 0;
        _rAudMixSndStream[i].fgPause = 0;
        _rAudMixSndStream[i].fgRepeat = 0;

        _rAudMixSndStream[i].u1ClipReadIndx = 0;
        _rAudMixSndStream[i].u1ClipWriteIndx = 0;

        _rAudMixSndStream[i].u1MixSndStereoOnOff = 0;
        _rAudMixSndStream[i].u1BitDepth = 16;
        _rAudMixSndStream[i].u4SampleRate = 48000;
        _rAudMixSndStream[i].u4TransferSZ = 0;
        _rAudMixSndStream[i].u4TransferSZ2 = 0;
        #if 0 //def ALSA_MIXSND_PATH
        _rAudMixSndStream[i].u4AFifoSZ = (u4GetMixSoundBufSize2()-0x10000)/(MAX_AUD_MIXSND_STREAM_NUM-1);
        _rAudMixSndStream[i].u4AFifoSA = u4GetMixSoundStartByteAddr2() + i * (u4GetMixSoundBufSize2()-0x10000)/(MAX_AUD_MIXSND_STREAM_NUM-1);
        #else
        _rAudMixSndStream[i].u4AFifoSZ = u4GetMixSoundBufSize2()/MAX_AUD_MIXSND_STREAM_NUM;
        _rAudMixSndStream[i].u4AFifoSA = u4GetMixSoundStartByteAddr2() + i * u4GetMixSoundBufSize2()/MAX_AUD_MIXSND_STREAM_NUM;
        #endif
        _rAudMixSndStream[i].u4AFifoEA = _rAudMixSndStream[i].u4AFifoSA + _rAudMixSndStream[i].u4AFifoSZ;
        _rAudMixSndStream[i].u4DesAddr = _rAudMixSndStream[i].u4AFifoSA;

        _rAudMixSndStream[i].u4StreamAddr = 0;
        _rAudMixSndStream[i].u4Residual = 0;        

        _rAudMixSndStream[i].i2DelayBuf[0] = 0;
        _rAudMixSndStream[i].i2DelayBuf[1] = 0;
    }
    #ifdef ALSA_MIXSND_PATH
    _rAudMixSndRingFifo.u4SZ = u4GetMixSoundBufSize3();    
    _rAudMixSndRingFifo.u4SA = u4GetMixSoundStartByteAddr3();
    _rAudMixSndRingFifo.u4EA = _rAudMixSndRingFifo.u4SA + _rAudMixSndRingFifo.u4SZ;
    _rAudMixSndRingFifo.u4RP = _rAudMixSndRingFifo.u4WP = _rAudMixSndRingFifo.u4SA;    
    #if 0    
    _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4AFifoSZ = 0x10000;
    _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4AFifoEA = _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4AFifoSA + _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4AFifoSZ;
    #endif
    #endif

    u4AFifoTA = _rAudMixSndStream[MAX_AUD_MIXSND_STREAM_NUM-1].u4AFifoEA;    

    #ifdef NEW_MIXSOUND
    if (!AUD_DspIsMixSndPlay())
    {
        AUD_DspSetMixSndLength(0);
        AUD_DspMixSndControl2(1);
        while (!AUD_DspIsMixSndPlay())
        {
            x_thread_delay(1);
        }
    }
    #else
    {
        //Pre-Init
        Printf("[FeedMixSndThread] Init MixSnd DSP\n");
        u4GainOld = u4ReadShmUINT32(D_MIXSOUND_GAIN); //FLASHLITE_CONFLICT
        u2MixSndCfg = u2ReadShmUINT16(W_MIXSOUND_CFG); //FLASHLITE_CONFLICT

        AUD_DspSetMixSndLength(0x10000);
        AUD_DspSetMixSndGain(0x20000);
        AUD_DspSetMixSndStereo(TRUE);
        AUD_DspSetMixSndUpSampleRatio(0);
        AUD_DspSetMixSndMemPlay(TRUE);
        //AUD_DspMixSndControl(1);
        _AUD_DspMixSndControl2(1); //FLASHLITE_CONFLICT
        while (!AUD_DspIsMixSndPlay())
        {
            x_thread_delay(1);
        }
    }
    #endif

    Printf("[FeedMixSndThread] Enter\n");
    fgAudFeedMixSndThreadEnable = TRUE;

    while (1)
    {
        UINT8 u1EmptyStreamNum;

        //Check if force stop
        if (_fgForceStopMixSndDma)
        {
            #ifdef NEW_MIXSOUND
            AUD_DspMixSndControl2(8); //disable mixsound
            #else
            //AUD_DspMixSndControl(2);
            _AUD_DspMixSndControl2(2); //FLASHLITE_CONFLICT
            AUD_DspSetMixSndMemPlay(FALSE);
            #endif
            break;
        }

        //Check if there is stop command need to take care
        for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
        {
			#ifdef ALSA_MIXSND_PATH
			if(i==ALSA_MIXSND_STREAM_ID)
			{
				break;
			}
			#endif
			
            if (_rAudMixSndStream[i].fgStop)
            {
                _rAudMixSndStream[i].fgEnable = 0;
                _rAudMixSndStream[i].fgPause = 0;
              #ifdef ALSA_MIXSND_PATH
                if (i==ALSA_MIXSND_STREAM_ID) 
                {
                    _rAudMixSndRingFifo.u4RP = _rAudMixSndRingFifo.u4WP = _rAudMixSndRingFifo.u4SA;
                }
              #endif
                else
                {
                    while (_rAudMixSndStream[i].u1ClipReadIndx != _rAudMixSndStream[i].u1ClipWriteIndx)
                    {
                        #ifdef LINUX_TURNKEY_SOLUTION
                        pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_THIRD, AUD_COND_AUD_CLIP_DONE, _rAudMixSndStream[i].u1ClipReadIndx,i);
                        LOG(4,"N_S(%d): %d/%d\n", i, _rAudMixSndStream[i].u1ClipReadIndx, _rAudMixSndStream[i].u1ClipWriteIndx);
                        #endif
                        _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4Size = 0;
                        _rAudMixSndStream[i].u1ClipReadIndx = ((_rAudMixSndStream[i].u1ClipReadIndx+1)%MAX_AUD_MIXSND_CLIP_NUM);
                    }
                }
                _rAudMixSndStream[i].u4Residual = 0;
                _rAudMixSndStream[i].fgStop = 0;
            }
        }

        //Check if there is data need to send
        u1EmptyStreamNum = 0;
        for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
        {
			#ifdef ALSA_MIXSND_PATH
			if(i==ALSA_MIXSND_STREAM_ID)
			{
				u1EmptyStreamNum++;
				break;
			}
			#endif
            if (_rAudMixSndStream[i].fgEnable)
            {
                if (_rAudMixSndStream[i].u4Residual == 0)
                {
                    //check if there is new clip
                    if ((_rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4Size) 
                        #ifdef ALSA_MIXSND_PATH
                        && (i != ALSA_MIXSND_STREAM_ID)
                        #endif
                        )
                    {
                        _rAudMixSndStream[i].u4Residual = (_rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4Size & 0xffffff00); //Clipping to align multiple of 0x100
                        _rAudMixSndStream[i].u4StreamAddr = _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4Addr;
                        _rAudMixSndStream[i].u4SampleRate = _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4SampleRate;
                        _rAudMixSndStream[i].u1MixSndStereoOnOff = _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u1StereoOnOff;
                        _rAudMixSndStream[i].u1BitDepth = _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u1BitDepth;

                        if (_rAudMixSndStream[i].u4Residual == 0)
                        {
                            LOG(4,"_AudFeedMixSndThread: clip size < 0x100, clip and notify\n");
                        #ifdef LINUX_TURNKEY_SOLUTION
                            pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_THIRD, AUD_COND_AUD_CLIP_DONE, _rAudMixSndStream[i].u1ClipReadIndx,i);
                            LOG(4,"N_C(%d): %d/%d\n", i, _rAudMixSndStream[i].u1ClipReadIndx, _rAudMixSndStream[i].u1ClipWriteIndx);
                        #endif
                            _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4Size = 0;
                            _rAudMixSndStream[i].u1ClipReadIndx = ((_rAudMixSndStream[i].u1ClipReadIndx+1)%MAX_AUD_MIXSND_CLIP_NUM);
                            i--;
                        }                        
                    }
                    else
                    {
                        u1EmptyStreamNum++;
                    }
                }
            }
            else
            {
                u1EmptyStreamNum++;
            }
        }

        //Data Empty
        if (u1EmptyStreamNum == MAX_AUD_MIXSND_STREAM_NUM)
        {
            if (!fgDataEmpty)
            {
                fgDataEmpty = TRUE;
                LOG(4, "!!!!! DataEmpty !!!!!\n");
            }
            x_thread_delay(1);
            continue;
        }
        else
        {
            fgDataEmpty = FALSE;
        }

        //Send Data
        #ifdef MIXSOUND_MIPS_MEASURE
        PARSER_WRITE32(REG_A_STC_1, 0);
        #endif
        u1EmptyStreamNum = 0;
        for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
        {
			#ifdef ALSA_MIXSND_PATH
			if(i == ALSA_MIXSND_STREAM_ID)
			{
				u1EmptyStreamNum++;
				break;
			}
			#endif
			
            if ((_rAudMixSndStream[i].u4Residual > 0) && (!_rAudMixSndStream[i].fgPause))
            {
                BOOL fgTransfer = FALSE;

                _rAudMixSndStream[i].u4TransferSZ = (_rAudMixSndStream[i].u4Residual > MIXSND_TRANSFER_SZ) ? MIXSND_TRANSFER_SZ : _rAudMixSndStream[i].u4Residual;
                _rAudMixSndStream[i].u4TransferSZ2 = 
                    (((_rAudMixSndStream[i].u4TransferSZ/(_rAudMixSndStream[i].u1BitDepth/8) * 48000L / _rAudMixSndStream[i].u4SampleRate) + 1)&0xfffffffe) * 
                    (2 - _rAudMixSndStream[i].u1MixSndStereoOnOff) * 2;

                #ifdef ALSA_MIXSND_PATH
                if ((i == ALSA_MIXSND_STREAM_ID) && 
                    ((_rAudMixSndRingFifo.u4RP + _rAudMixSndStream[i].u4TransferSZ) > _rAudMixSndRingFifo.u4EA))
                {
                    x_memcpy((VOID*)(VIRTUAL(_rAudMixSndRingFifo.u4EA)),
                             (VOID*)(VIRTUAL(_rAudMixSndRingFifo.u4SA)),
                             (_rAudMixSndStream[i].u4TransferSZ - (_rAudMixSndRingFifo.u4EA - _rAudMixSndRingFifo.u4RP)));
                }
                #endif
                
                if (fgIsSoftTransferable(i))
                {
                    if (_rAudMixSndStream[i].u1BitDepth == 16)
                    {
                        do_SRC_S16LE(i,
                                (INT16*)(VIRTUAL(_rAudMixSndStream[i].u4StreamAddr)),
                                (INT16*)(VIRTUAL(u4AFifoTA)),
                                _rAudMixSndStream[i].u4SampleRate,
                                48000,
                                _rAudMixSndStream[i].u4TransferSZ/2, //bit 16
                                _rAudMixSndStream[i].u1MixSndStereoOnOff);
                    }
                    else
                    {
                        do_SRC_U8(i,
                                (UINT8*)(VIRTUAL(_rAudMixSndStream[i].u4StreamAddr)),
                                (INT16*)(VIRTUAL(u4AFifoTA)),
                                _rAudMixSndStream[i].u4SampleRate,
                                48000,
                                _rAudMixSndStream[i].u4TransferSZ, //bit 8
                                _rAudMixSndStream[i].u1MixSndStereoOnOff);                        
                    }
                    fgTransfer = TRUE;
                }

                if (!fgTransfer)
                {
                    //if fifo full
                    u1EmptyStreamNum++;
                }
                else
                {
                    vSoftTransfer(u4AFifoTA, _rAudMixSndStream[i].u4TransferSZ2,_rAudMixSndStream[i].u4DesAddr,
                                           _rAudMixSndStream[i].u4AFifoSA, _rAudMixSndStream[i].u4AFifoEA,i);

                    #ifdef MIXSND_OUTPUT_DBG
                    //TEST
                    if (u4TotalOutputSize < u4MixSndDumpSize)
                    {
                        if (u1DumpLocation == 1)
                        {
                            //After SW ASRC
                            if (u4TotalOutputSize + _rAudMixSndStream[i].u4TransferSZ2 < u4MixSndDumpSize)
                            {
                                x_memcpy((VOID*)(VIRTUAL(u4OutputBuf)), (VOID*)(VIRTUAL(u4AFifoTA)), _rAudMixSndStream[i].u4TransferSZ2);
                                u4OutputBuf += _rAudMixSndStream[i].u4TransferSZ2;
                                u4TotalOutputSize += _rAudMixSndStream[i].u4TransferSZ2;
                            }
                            else
                            {
                                x_memcpy((VOID*)(VIRTUAL(u4OutputBuf)), (VOID*)(VIRTUAL(u4AFifoTA)), u4MixSndDumpSize - u4TotalOutputSize);
                                u4OutputBuf = u4MixSndDumpBufAddr + u4MixSndDumpSize;
                                u4TotalOutputSize = u4MixSndDumpSize;
                                Printf("After SW ASRC Dump End. please check %08x~%08x\n", u4MixSndDumpBufAddr, u4MixSndDumpBufAddr + u4MixSndDumpSize - 1);
                            }
                        }
                        else
                        {
                            //Before SW ASRC
                            if (u4TotalOutputSize + _rAudMixSndStream[i].u4TransferSZ < u4MixSndDumpSize)
                            {
                                x_memcpy((VOID*)(VIRTUAL(u4OutputBuf)), (VOID*)(VIRTUAL(_rAudMixSndStream[i].u4StreamAddr)), _rAudMixSndStream[i].u4TransferSZ);
                                u4OutputBuf += _rAudMixSndStream[i].u4TransferSZ;
                                u4TotalOutputSize += _rAudMixSndStream[i].u4TransferSZ;
                            }
                            else
                            {
                                x_memcpy((VOID*)(VIRTUAL(u4OutputBuf)), (VOID*)(VIRTUAL(_rAudMixSndStream[i].u4StreamAddr)), u4MixSndDumpSize - u4TotalOutputSize);
                                u4OutputBuf = u4MixSndDumpBufAddr + u4MixSndDumpSize;
                                u4TotalOutputSize = u4MixSndDumpSize;
                                Printf("Before SW SRC Dump End. please check %08x~%08x\n", u4MixSndDumpBufAddr, u4MixSndDumpBufAddr + u4MixSndDumpSize - 1);
                            }
                        }
                    }
                    //TEST
                    #endif

                    _rAudMixSndStream[i].u4DesAddr = ((_rAudMixSndStream[i].u4DesAddr + _rAudMixSndStream[i].u4TransferSZ2) < _rAudMixSndStream[i].u4AFifoEA) ?
                                    (_rAudMixSndStream[i].u4DesAddr + _rAudMixSndStream[i].u4TransferSZ2) :
                                    ((_rAudMixSndStream[i].u4DesAddr + _rAudMixSndStream[i].u4TransferSZ2) - _rAudMixSndStream[i].u4AFifoSZ);

                  #ifdef ALSA_MIXSND_PATH
                    if (i==ALSA_MIXSND_STREAM_ID) 
                    {
                        //Update RingFifo RP
                        _rAudMixSndRingFifo.u4RP = 
                            (_rAudMixSndRingFifo.u4RP + _rAudMixSndStream[i].u4TransferSZ < _rAudMixSndRingFifo.u4EA) ?
                            (_rAudMixSndRingFifo.u4RP + _rAudMixSndStream[i].u4TransferSZ) : 
                            (_rAudMixSndRingFifo.u4RP + _rAudMixSndStream[i].u4TransferSZ - _rAudMixSndRingFifo.u4SZ);
                    }
                  #endif
                    else
                    {
                        _rAudMixSndStream[i].u4Residual -= _rAudMixSndStream[i].u4TransferSZ;
                        _rAudMixSndStream[i].u4StreamAddr += _rAudMixSndStream[i].u4TransferSZ;

                        if ((_rAudMixSndStream[i].u4Residual == 0) && (!_rAudMixSndStream[i].fgRepeat))
                        {
                            #ifdef LINUX_TURNKEY_SOLUTION
                            pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_THIRD, AUD_COND_AUD_CLIP_DONE, _rAudMixSndStream[i].u1ClipReadIndx,i);
                            LOG(4,"N_C(%d): %d/%d\n", i, _rAudMixSndStream[i].u1ClipReadIndx, _rAudMixSndStream[i].u1ClipWriteIndx);
                            #endif
                            _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4Size = 0;
                            _rAudMixSndStream[i].u1ClipReadIndx = ((_rAudMixSndStream[i].u1ClipReadIndx+1)%MAX_AUD_MIXSND_CLIP_NUM);
                        }
                    }
                }
            }
            else
            {
                u1EmptyStreamNum++;
            }
        }

        //FIFO Full
        if (u1EmptyStreamNum == MAX_AUD_MIXSND_STREAM_NUM)
        {
        #ifdef MIXSND_OUTPUT_DBG
            if (!fgFifoFull)
            {
                fgFifoFull = TRUE;
                //Printf("!!!!! Fifo Full !!!!!\n");
            }
        #endif
            x_thread_delay(10);
        }
    #ifdef MIXSND_OUTPUT_DBG
        else
        {
            fgFifoFull = FALSE;
        }
    #endif                
        #ifdef MIXSOUND_MIPS_MEASURE
        else
        {
            u4MixSndMIPS += PARSER_READ32(REG_A_STC_1);
            u4MixSndSendCnt++;                   
        }
        #endif
    }

    //flush clip pointer
    for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
    {
      #ifdef ALSA_MIXSND_PATH
        if (i != ALSA_MIXSND_STREAM_ID)
      #endif        
        {
            while (_rAudMixSndStream[i].u1ClipReadIndx != _rAudMixSndStream[i].u1ClipWriteIndx)
            {
                #ifdef LINUX_TURNKEY_SOLUTION
                pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_THIRD, AUD_COND_AUD_CLIP_DONE, _rAudMixSndStream[i].u1ClipReadIndx,i);
                LOG(4,"N_F(%d): %d/%d\n", i, _rAudMixSndStream[i].u1ClipReadIndx, _rAudMixSndStream[i].u1ClipWriteIndx);
                #endif
                _rAudMixSndClip[i][_rAudMixSndStream[i].u1ClipReadIndx].u4Size = 0;
                _rAudMixSndStream[i].u1ClipReadIndx = ((_rAudMixSndStream[i].u1ClipReadIndx+1)%MAX_AUD_MIXSND_CLIP_NUM);
            }
        }
    }

    #ifndef NEW_MIXSOUND
    AUD_DspSetMixSndGain(u4GainOld); //FLASHLITE_CONFLICT
    vWriteShmUINT16(W_MIXSOUND_CFG,u2MixSndCfg); //FLASHLITE_CONFLICT    
    #endif
    _hAudFeedMixSndThread = NULL_HANDLE;
    _fgForceStopMixSndDma = FALSE;
    fgAudFeedMixSndThreadEnable = FALSE;

    #ifdef LINUX_TURNKEY_SOLUTION
    pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_THIRD, AUD_COND_CTRL_DONE, AUD_DEC_CTRL_STOP, AUD_OK);
    LOG(4,"N_AUD_COND_CTRL_DONE\n");
    #endif

    Printf("[FeedMixSndThread] Exit\n");
}
#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
#define TRANSFER_BUF_SIZE (128*1024)    ///1024*1024
static UINT8 _gu1TransferBuf[TRANSFER_BUF_SIZE];
#endif

static void _AudFeedAlsaMixSndThread(const void* pvArg)
{
    int i;
    UINT32 u4AFifoTA;
    #ifndef NEW_MIXSOUND
    UINT32 u4GainOld; //FLASHLITE_CONFLICT
    UINT16 u2MixSndCfg; //FLASHLITE_CONFLICT
    #endif
    AudDecNfyFct pfAudDecNfy = NULL;

    BOOL fgDataEmpty = FALSE;
    #ifdef MIXSND_OUTPUT_DBG
    BOOL fgFifoFull = FALSE;
    #endif

	UINT8 u1EmptyStreamNum;
	UINT8 u1SRC_Enable = 0;

	#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
	AUD_OSS_ADAP_ST_TYPE_T status;
	AUD_OSS_ADAP_SRC_TYPE_T source;
	UINT32 u4tempStart;
	UINT32 u4tempEnd;
	UINT32 u4tempPtr;
	UINT32 u4tempFlag;
	u4tempStart = (UINT32)_gu1TransferBuf;
	u4tempEnd = (UINT32)_gu1TransferBuf + TRANSFER_BUF_SIZE;
	u4tempPtr = u4tempStart;
	u4tempFlag = 0;
	#endif
    VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy));

    Printf("[_AudFeedAlsaMixSndThread] Init - 201109161652\n");

    x_memset((VOID*)VIRTUAL((UINT32)&_rAudMixSndStream), 0, sizeof(_rAudMixSndStream));
    x_memset((VOID*)VIRTUAL((UINT32)&_rAudMixSndClip), 0, sizeof(_rAudMixSndClip));
    #ifdef ALSA_MIXSND_PATH
    x_memset((VOID*)VIRTUAL((UINT32)&_rAudMixSndRingFifo), 0, sizeof(_rAudMixSndRingFifo));
    #endif

	// Initialize ALSA mixsound stream information
	i = ALSA_MIXSND_STREAM_ID;
  
    _rAudMixSndStream[i].fgEnable = 0;
    _rAudMixSndStream[i].fgStop = 0;
    _rAudMixSndStream[i].fgPause = 0;
    _rAudMixSndStream[i].fgRepeat = 0;

    _rAudMixSndStream[i].u1ClipReadIndx = 0;
    _rAudMixSndStream[i].u1ClipWriteIndx = 0;

    _rAudMixSndStream[i].u1MixSndStereoOnOff = 0;
    _rAudMixSndStream[i].u1BitDepth = 16;
    _rAudMixSndStream[i].u4SampleRate = 48000;
    _rAudMixSndStream[i].u4TransferSZ = 0;
    _rAudMixSndStream[i].u4TransferSZ2 = 0;
    #if 0//#ifdef ALSA_MIXSND_PATH
    _rAudMixSndStream[i].u4AFifoSZ = (u4GetMixSoundBufSize2()-0x10000)/(MAX_AUD_MIXSND_STREAM_NUM-1);
    _rAudMixSndStream[i].u4AFifoSA = u4GetMixSoundStartByteAddr2() + i * (u4GetMixSoundBufSize2()-0x10000)/(MAX_AUD_MIXSND_STREAM_NUM-1);
    #else
    _rAudMixSndStream[i].u4AFifoSZ = u4GetMixSoundBufSize2()/MAX_AUD_MIXSND_STREAM_NUM;
    _rAudMixSndStream[i].u4AFifoSA = u4GetMixSoundStartByteAddr2() + i * u4GetMixSoundBufSize2()/MAX_AUD_MIXSND_STREAM_NUM;
    #endif
    _rAudMixSndStream[i].u4AFifoEA = _rAudMixSndStream[i].u4AFifoSA + _rAudMixSndStream[i].u4AFifoSZ;
    _rAudMixSndStream[i].u4DesAddr = _rAudMixSndStream[i].u4AFifoSA;

    _rAudMixSndStream[i].u4StreamAddr = 0;
    _rAudMixSndStream[i].u4Residual = 0;        

    _rAudMixSndStream[i].i2DelayBuf[0] = 0;
    _rAudMixSndStream[i].i2DelayBuf[1] = 0;
    
    #ifdef ALSA_MIXSND_PATH
    _rAudMixSndRingFifo.u4SZ = u4GetMixSoundBufSize3();    
    _rAudMixSndRingFifo.u4SA = u4GetMixSoundStartByteAddr3();
    _rAudMixSndRingFifo.u4EA = _rAudMixSndRingFifo.u4SA + _rAudMixSndRingFifo.u4SZ;
    _rAudMixSndRingFifo.u4RP = _rAudMixSndRingFifo.u4WP = _rAudMixSndRingFifo.u4SA;
	#if 0
    _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4AFifoSZ = 0x10000;
    _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4AFifoEA = _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4AFifoSA + _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4AFifoSZ;
	#endif
    #endif

    u4AFifoTA = _rAudMixSndStream[MAX_AUD_MIXSND_STREAM_NUM-1].u4AFifoEA;    

    #ifdef NEW_MIXSOUND
    if (!AUD_DspIsMixSndPlay())
    {
        AUD_DspSetMixSndLength(0);
        AUD_DspMixSndControl2(1);
        while (!AUD_DspIsMixSndPlay())
        {
            x_thread_delay(1);
        }
    }
    #else
    {
        //Pre-Init
        Printf("[_AudFeedAlsaMixSndThread] Init MixSnd DSP\n");
        u4GainOld = u4ReadShmUINT32(D_MIXSOUND_GAIN); //FLASHLITE_CONFLICT
        u2MixSndCfg = u2ReadShmUINT16(W_MIXSOUND_CFG); //FLASHLITE_CONFLICT

        AUD_DspSetMixSndLength(0x10000);
        AUD_DspSetMixSndGain(0x20000);
        AUD_DspSetMixSndStereo(TRUE);
        AUD_DspSetMixSndUpSampleRatio(0);
        AUD_DspSetMixSndMemPlay(TRUE);
        //AUD_DspMixSndControl(1);
        _AUD_DspMixSndControl2(1); //FLASHLITE_CONFLICT
        while (!AUD_DspIsMixSndPlay())
        {
            x_thread_delay(1);
        }
    }
    #endif

    Printf("[_AudFeedAlsaMixSndThread] Enter\n");
    fgAudFeedMixSndThreadEnable = TRUE;

    while (1)
    {
		u1SRC_Enable = 0;
			
        //Check if force stop
        if (_fgForceStopMixSndDma)
        {
            #ifdef NEW_MIXSOUND
            AUD_DspMixSndControl2(8); //disable mixsound
            #else
            //AUD_DspMixSndControl(2);
            _AUD_DspMixSndControl2(2); //FLASHLITE_CONFLICT
            AUD_DspSetMixSndMemPlay(FALSE);
            #endif
            break;
        }

        //Check if there is stop command need to take care
		#ifdef ALSA_MIXSND_PATH
		i = ALSA_MIXSND_STREAM_ID;
		#endif
        if (_rAudMixSndStream[i].fgStop)
        {
            _rAudMixSndStream[i].fgEnable = 0;
            _rAudMixSndStream[i].fgPause = 0;
          #ifdef ALSA_MIXSND_PATH
            _rAudMixSndRingFifo.u4RP = _rAudMixSndRingFifo.u4WP = _rAudMixSndRingFifo.u4SA;
          #endif

            _rAudMixSndStream[i].u4Residual = 0;
            _rAudMixSndStream[i].fgStop = 0;
        }

      	#ifdef ALSA_MIXSND_PATH
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4Residual = 
            ((_rAudMixSndRingFifo.u4WP >= _rAudMixSndRingFifo.u4RP) ? 
                (_rAudMixSndRingFifo.u4WP - _rAudMixSndRingFifo.u4RP) : 
                (_rAudMixSndRingFifo.u4SZ - _rAudMixSndRingFifo.u4RP + _rAudMixSndRingFifo.u4WP));
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4StreamAddr = _rAudMixSndRingFifo.u4RP;
      	#endif

        //Check if there is data need to send
        u1EmptyStreamNum = 0;
		#ifdef ALSA_MIXSND_PATH
		i = ALSA_MIXSND_STREAM_ID;
		#endif

        if (_rAudMixSndStream[i].fgEnable)
        {
            if (_rAudMixSndStream[i].u4Residual == 0)
            {
                //check if there is new clip
                 u1EmptyStreamNum++;
            }
        }
        else
        {
            u1EmptyStreamNum++;
        }


        //Data Empty
        if (u1EmptyStreamNum != 0)
        {
            if (!fgDataEmpty)
            {
                fgDataEmpty = TRUE;
                LOG(4, "!!!!! DataEmpty !!!!!\n");
            }
            x_thread_delay(1);
            continue;
        }
        else
        {
            fgDataEmpty = FALSE;
        }

        //Send Data
        #ifdef MIXSOUND_MIPS_MEASURE
        PARSER_WRITE32(REG_A_STC_1, 0);
        #endif
		
        u1EmptyStreamNum = 0;
		#ifdef ALSA_MIXSND_PATH
		i = ALSA_MIXSND_STREAM_ID;
		#endif
        //for (i=ALSA_MIXSND_STREAM_ID;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
        //{
            if ((_rAudMixSndStream[i].u4Residual) && (!_rAudMixSndStream[i].fgPause))
            {
                BOOL fgTransfer = FALSE;

                _rAudMixSndStream[i].u4TransferSZ = (_rAudMixSndStream[i].u4Residual > MIXSND_TRANSFER_SZ) ? MIXSND_TRANSFER_SZ : _rAudMixSndStream[i].u4Residual;
				if((_rAudMixSndStream[i].u4SampleRate == 48000) && (_rAudMixSndStream[i].u1MixSndStereoOnOff == 1)
					&&(_rAudMixSndStream[i].u1BitDepth == 16))
				{
					_rAudMixSndStream[i].u4TransferSZ2 = _rAudMixSndStream[i].u4TransferSZ;
					u1SRC_Enable = 0;
				}
				else
				{
	                _rAudMixSndStream[i].u4TransferSZ2 = 
	                    (((_rAudMixSndStream[i].u4TransferSZ/(_rAudMixSndStream[i].u1BitDepth/8) * 48000L / _rAudMixSndStream[i].u4SampleRate) + 1)&0xfffffffe) * 
	                    (2 - _rAudMixSndStream[i].u1MixSndStereoOnOff) * 2;
					u1SRC_Enable = 1;
				}

                #ifdef ALSA_MIXSND_PATH
                if ((i == ALSA_MIXSND_STREAM_ID) && 
                    ((_rAudMixSndRingFifo.u4RP + _rAudMixSndStream[i].u4TransferSZ) > _rAudMixSndRingFifo.u4EA))
                {
                    x_memcpy((VOID*)(VIRTUAL(_rAudMixSndRingFifo.u4EA)),
                             (VOID*)(VIRTUAL(_rAudMixSndRingFifo.u4SA)),
                             (_rAudMixSndStream[i].u4TransferSZ - (_rAudMixSndRingFifo.u4EA - _rAudMixSndRingFifo.u4RP)));
                }
                #endif
                
                if (fgIsSoftTransferable(i) && (u1SRC_Enable == 1))
                {
                    if (_rAudMixSndStream[i].u1BitDepth == 16)
                    {
                        do_SRC_S16LE(i,
                                (INT16*)(VIRTUAL(_rAudMixSndStream[i].u4StreamAddr)),
                                (INT16*)(VIRTUAL(u4AFifoTA)),
                                _rAudMixSndStream[i].u4SampleRate,
                                48000,
                                _rAudMixSndStream[i].u4TransferSZ/2, //bit 16
                                _rAudMixSndStream[i].u1MixSndStereoOnOff);
                    }
                    else
                    {
                        do_SRC_U8(i,
                                (UINT8*)(VIRTUAL(_rAudMixSndStream[i].u4StreamAddr)),
                                (INT16*)(VIRTUAL(u4AFifoTA)),
                                _rAudMixSndStream[i].u4SampleRate,
                                48000,
                                _rAudMixSndStream[i].u4TransferSZ, //bit 8
                                _rAudMixSndStream[i].u1MixSndStereoOnOff);                        
                    }
                    fgTransfer = TRUE;
                }
				else if(fgIsSoftTransferable(i) && (u1SRC_Enable == 0))
				{
					x_memcpy((VOID*)(VIRTUAL(u4AFifoTA)), (VOID*)(VIRTUAL(_rAudMixSndStream[i].u4StreamAddr)), _rAudMixSndStream[i].u4TransferSZ * 2);
					fgTransfer = TRUE;
				}

                if (!fgTransfer)
                {
                    //if fifo full
                    u1EmptyStreamNum++;
                }
                else
                {
					#ifdef CC_AUD_SUPPORT_OSS_ADAPTION_LAYER
					status = _AudOssAdap_GetStatus();
					source = _AudOssAdap_GetSourceType();
					if((AUD_OSS_ADAP_ST_WORKING == status) && (AUD_OSS_ADAP_MIXSOUND == source))
					{
						_AudDataTransferLock();
						if((u4tempPtr + _rAudMixSndStream[i].u4TransferSZ2) < u4tempEnd)
						{
							x_memcpy((VOID*)(u4tempPtr), (VOID*)(VIRTUAL(u4AFifoTA)), _rAudMixSndStream[i].u4TransferSZ2);
						}
						else
						{
							u4tempPtr = u4tempStart;
							x_memcpy((VOID*)(u4tempPtr), (VOID*)(VIRTUAL(u4AFifoTA)), _rAudMixSndStream[i].u4TransferSZ2);
						}

						_AudDataTransferUpdate(u4tempPtr, _rAudMixSndStream[i].u4TransferSZ2);
						u4tempPtr += _rAudMixSndStream[i].u4TransferSZ2;
						_AudDataTransferNotify();
					}
					else
					#endif
					{
                    vSoftTransfer(u4AFifoTA, _rAudMixSndStream[i].u4TransferSZ2,_rAudMixSndStream[i].u4DesAddr,
                                           _rAudMixSndStream[i].u4AFifoSA, _rAudMixSndStream[i].u4AFifoEA,i);


                    _rAudMixSndStream[i].u4DesAddr = ((_rAudMixSndStream[i].u4DesAddr + _rAudMixSndStream[i].u4TransferSZ2) < _rAudMixSndStream[i].u4AFifoEA) ?
                                    (_rAudMixSndStream[i].u4DesAddr + _rAudMixSndStream[i].u4TransferSZ2) :
                                    ((_rAudMixSndStream[i].u4DesAddr + _rAudMixSndStream[i].u4TransferSZ2) - _rAudMixSndStream[i].u4AFifoSZ);
					}
                  #ifdef ALSA_MIXSND_PATH
                    //Update RingFifo RP
                    _rAudMixSndRingFifo.u4RP = 
                        (_rAudMixSndRingFifo.u4RP + _rAudMixSndStream[i].u4TransferSZ < _rAudMixSndRingFifo.u4EA) ?
                        (_rAudMixSndRingFifo.u4RP + _rAudMixSndStream[i].u4TransferSZ) : 
                        (_rAudMixSndRingFifo.u4RP + _rAudMixSndStream[i].u4TransferSZ - _rAudMixSndRingFifo.u4SZ);
                   
                  #endif
                }
            }
            else
            {
                u1EmptyStreamNum++;
            }
        //}

        //FIFO Full
        if (u1EmptyStreamNum != 0)
        {
        #ifdef MIXSND_OUTPUT_DBG
            if (!fgFifoFull)
            {
                fgFifoFull = TRUE;
                //Printf("!!!!! Fifo Full !!!!!\n");
            }
        #endif
            x_thread_delay(10);
        }
    #ifdef MIXSND_OUTPUT_DBG
        else
        {
            fgFifoFull = FALSE;
        }
    #endif                
        #ifdef MIXSOUND_MIPS_MEASURE
        else
        {
            u4MixSndMIPS += PARSER_READ32(REG_A_STC_1);
            u4MixSndSendCnt++;                   
        }
        #endif
    }

    #ifndef NEW_MIXSOUND
    AUD_DspSetMixSndGain(u4GainOld); //FLASHLITE_CONFLICT
    vWriteShmUINT16(W_MIXSOUND_CFG,u2MixSndCfg); //FLASHLITE_CONFLICT    
    #endif
    _hAudFeedMixSndThread = NULL_HANDLE;
    _fgForceStopMixSndDma = FALSE;
    fgAudFeedMixSndThreadEnable = FALSE;

    #ifdef LINUX_TURNKEY_SOLUTION
    pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_THIRD, AUD_COND_CTRL_DONE, AUD_DEC_CTRL_STOP, AUD_OK);
    LOG(4,"N_AUD_COND_CTRL_DONE\n");
    #endif

    Printf("[_AudFeedAlsaMixSndThread] Exit\n");
}



#ifndef NEW_MIXSOUND
#ifdef CC_AUD_MIXSOUND_SUPPORT
extern BOOL _fgMixSndFlag;
extern UINT8 _u1MixSndClipIdx;
#endif
#endif

//-----------------------------------------------------------------------------
/** AUD_EnableMixSndClip
 *
 *  @param void
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_EnableMixSndClip(void)
{
    if (_hAudFeedMixSndThread)
    {
        LOG(5, "[AUD_EnableMixSndClip] !!!! _AudFeedMixSndThread already created !!!!\n");
        return;
    }

    if (OSR_OK != x_thread_create(&_hAudFeedMixSndThread,
                                   AUD_DRV_FEEDMIXSND_THREAD_NAME,
                                   AUD_DRV_THREAD_STACK_SIZE,
                                   50, //AUD_DRV_THREAD_PRIORITY, // 2011/3/29, by Daniel, Zink said for Skype integration, it needs to set _AudFeedMixSndThread priority to 50 to prevent from data copy delay
                                   (x_os_thread_main_fct) _AudFeedMixSndThread,
                                   0,
                                   NULL))
    {
        LOG(5, "[AUD_EnableMixSndClip] !!!!Create _AudFeedMixSndThread Fail!!!!\n");
    }
    else
    {
        LOG(5, "[AUD_EnableMixSndClip] !!!!Create _AudFeedMixSndThread Succ!!!!\n");
        while (!fgAudFeedMixSndThreadEnable) //for CMPB integration
        {
            x_thread_delay(1);
        }
    }
}

void AUD_EnableAlsaMixSnd(void)
{
	if (_hAudFeedAlsaMixSndThread)
    {
        LOG(5, "[AUD_EnableAlsaMixSnd] !!!! _hAudFeedAlsaMixSndThread already created !!!!\n");
        return;
    }

    if (OSR_OK != x_thread_create(&_hAudFeedAlsaMixSndThread,
                                   AUD_DRV_ALSA_FEEDMIXSND_THREAD_NAME,
                                   AUD_DRV_THREAD_STACK_SIZE,
                                   50, //AUD_DRV_THREAD_PRIORITY, // 2011/3/29, by Daniel, Zink said for Skype integration, it needs to set _AudFeedMixSndThread priority to 50 to prevent from data copy delay
                                   (x_os_thread_main_fct) _AudFeedAlsaMixSndThread,
                                   0,
                                   NULL))
    {
        LOG(5, "[AUD_EnableAlsaMixSnd] !!!!Create _hAudFeedAlsaMixSndThread Fail!!!!\n");
    }
    else
    {
        LOG(5, "[AUD_EnableAlsaMixSnd] !!!!Create _hAudFeedAlsaMixSndThread Succ!!!!\n");
        while (!fgAudFeedMixSndThreadEnable) //for CMPB integration
        {
            x_thread_delay(1);
        }
    }
}

//-----------------------------------------------------------------------------
/** AUD_DisableMixSndClip
 *
 *  @param void
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_DisableMixSndClip(void)
{
    if (_hAudFeedMixSndThread)
    {
        LOG(5, "[AUD_DisableMixSndClip] trigger\n");
        _fgForceStopMixSndDma = TRUE;
        while (!_fgForceStopMixSndDma);
        x_thread_delay(100);
    }
    else
    {
        LOG(5, "[AUD_DisableMixSndClip] _hAudFeedMixSndThread not available\n");
    }
}

//-----------------------------------------------------------------------------
/** AUD_PlayMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_PlayMixSndClip(UINT8 u1StreamID)
{
    if (_hAudFeedMixSndThread)
    {
        _rAudMixSndStream[u1StreamID].fgEnable = 1;
        LOG(5, "[AUD_PlayMixSndClip] trigger %d\n", u1StreamID);
        #ifdef SUPPORT_6X_OPT_FILTER
        filter_init_up6(u1StreamID);
        #endif        
    }
    else
    {
        LOG(5, "[AUD_PlayMixSndClip] _hAudFeedMixSndThread not available\n");
    }
}

//-----------------------------------------------------------------------------
/** AUD_StopMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_StopMixSndClip(UINT8 u1StreamID)
{
    if (_hAudFeedMixSndThread)
    {
        _rAudMixSndStream[u1StreamID].fgStop = 1;
        LOG(5, "[AUD_StopMixSndClip] trigger %d\n", u1StreamID);
    }
    else
    {
        LOG(5, "[AUD_StopMixSndClip] _hAudFeedMixSndThread not available\n");
    }
    
    while (_rAudMixSndStream[u1StreamID].fgStop)
    {
        x_thread_delay(1);
    }

    {    
      AudDecNfyFct pfAudDecNfy = NULL;
    
      VERIFY(_AUD_GetNotifyFunc(&pfAudDecNfy));
      
      #ifdef LINUX_TURNKEY_SOLUTION
      pfAudDecNfy((void *)AUD_NFY_STREAM, AUD_DEC_THIRD, AUD_COND_CTRL_DONE, AUD_DEC_CTRL_STOP, AUD_OK);
      LOG(4,"N_AUD_COND_CTRL_DONE\n");
      #endif    
    }
}

//-----------------------------------------------------------------------------
/** AUD_PauseMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_PauseMixSndClip(UINT8 u1StreamID)
{
    if (_hAudFeedMixSndThread)
    {
        _rAudMixSndStream[u1StreamID].fgPause = 1;
        LOG(5, "[AUD_PauseMixSndClip] trigger %d\n",u1StreamID);
    }
    else
    {
        LOG(5, "[AUD_PauseMixSndClip] _hAudFeedMixSndThread not available\n");
    }
}

//-----------------------------------------------------------------------------
/** AUD_ResumeMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_ResumeMixSndClip(UINT8 u1StreamID)
{
    if (_hAudFeedMixSndThread)
    {
        _rAudMixSndStream[u1StreamID].fgPause = 0;
        LOG(5, "[AUD_ResumeMixSndClip] trigger %d\n",u1StreamID);
    }
    else
    {
        LOG(5, "[AUD_ResumeMixSndClip] _hAudFeedMixSndThread not available\n");
    }
}

//-----------------------------------------------------------------------------
/** AUD_RepeatMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @retval void
 */
//-----------------------------------------------------------------------------
void AUD_RepeatMixSndClip(UINT8 u1StreamID)
{
    if (_hAudFeedMixSndThread)
    {
        _rAudMixSndStream[u1StreamID].fgRepeat = !_rAudMixSndStream[u1StreamID].fgRepeat;
        LOG(5, "[AUD_RepeatMixSndClip] trigger %d\n", u1StreamID);
    }
    else
    {
        LOG(5, "[AUD_RepeatMixSndClip] _hAudFeedMixSndThread not available\n");
    }
}

//-----------------------------------------------------------------------------
/** AUD_SetMixSndClip
 *
 *  @param u1StreamID           MixSound Stream ID (0~MAX_AUD_MIXSOUND_STREAM_NUM)
 *  @param u4Addr               MixSound Clip Memory Physical Address
 *  @param u4Size               MixSound Clip Size (in byte)
 *  @param u4Gain               MixSound Clip Gain (0db => 0x20000)
 *  @param u1StereoOnOff        MixSound Clip Mono or Stereo (FALSE: mono, TRUE: stereo)
 *  @param u4SampleRate         MixSound Clip Sampling Rate (48000/44100/32000 ....)
 *  @param u1BitDepth           MixSound Clip Bit Depth (8/16)
 *  @retval BOOL                TRUE - set succ, FALSE - set fail
 */
//-----------------------------------------------------------------------------
BOOL AUD_SetMixSndClip(UINT8 u1StreamID, UINT32 u4Addr, UINT32 u4Size, UINT32 u4Gain, UINT8 u1StereoOnOff, UINT32 u4SampleRate, UINT8 u1BitDepth)
{
    if (_hAudFeedMixSndThread)
    {
        if (_rAudMixSndStream[u1StreamID].u1ClipWriteIndx<MAX_AUD_MIXSND_CLIP_NUM)
        {
            if (((_rAudMixSndStream[u1StreamID].u1ClipWriteIndx+1)%MAX_AUD_MIXSND_CLIP_NUM) == _rAudMixSndStream[u1StreamID].u1ClipReadIndx)
            {
                LOG(5, "[AUD_SetMixSndClip] Stream %d Clip Full\n", u1StreamID);
                return FALSE;
            }

            LOG(5, "[AUD_SetMixSndClip] ID: %d u4Addr: 0x%08x Size: 0x%08x Gain: 0x%08x Stereo: %d SampleRate: %d u1BitDepth: %d\n", 
                    u1StreamID, u4Addr, u4Size, u4Gain, u1StereoOnOff, u4SampleRate, u1BitDepth);

            _rAudMixSndClip[u1StreamID][_rAudMixSndStream[u1StreamID].u1ClipWriteIndx].u4Addr = u4Addr;
            _rAudMixSndClip[u1StreamID][_rAudMixSndStream[u1StreamID].u1ClipWriteIndx].u4Size = u4Size;            
            _rAudMixSndClip[u1StreamID][_rAudMixSndStream[u1StreamID].u1ClipWriteIndx].u4Gain = u4Gain;
            _rAudMixSndClip[u1StreamID][_rAudMixSndStream[u1StreamID].u1ClipWriteIndx].u1StereoOnOff = u1StereoOnOff;
            _rAudMixSndClip[u1StreamID][_rAudMixSndStream[u1StreamID].u1ClipWriteIndx].u4SampleRate = u4SampleRate;
            _rAudMixSndClip[u1StreamID][_rAudMixSndStream[u1StreamID].u1ClipWriteIndx].u1BitDepth= u1BitDepth;
            _rAudMixSndStream[u1StreamID].u1ClipWriteIndx = ((_rAudMixSndStream[u1StreamID].u1ClipWriteIndx+1)%MAX_AUD_MIXSND_CLIP_NUM);

            return TRUE;
        }
        else
        {
            LOG(5, "[AUD_SetMixSndClip] Stream %d Error\n", u1StreamID);
            return FALSE;
        }
    }
    else
    {
        LOG(5, "[AUD_SetMixSndClip] _hAudFeedMixSndThread not available\n");
        return FALSE;
    }
 }

void AUD_QueryMixSndClip(void)
{
    UINT8 i;

    if (_hAudFeedMixSndThread)
    {
        Printf("_hAudFeedMixSndThread ongoing\n");
        #ifdef MIXSND_OUTPUT_DBG
        Printf("u4OutputBuf: 0x%08x/0x%08x Size: 0x%08x/0x%08x\n", u4OutputBuf, u4MixSndDumpBufAddr, u4TotalOutputSize, u4MixSndDumpSize); //TEST
        #endif
        for (i=0;i<MAX_AUD_MIXSND_STREAM_NUM;i++)
        {
            Printf("MixSound Stream ID: %d\n", i);
            Printf("  fgEnable: %d fgStop: %d fgPause: %d fgRepeat: %d\n",
                    _rAudMixSndStream[i].fgEnable,
                    _rAudMixSndStream[i].fgStop,
                    _rAudMixSndStream[i].fgPause,
                    _rAudMixSndStream[i].fgRepeat);
            Printf("  Read Indx: %d Write Indx: %d\n",
                    _rAudMixSndStream[i].u1ClipReadIndx,
                    _rAudMixSndStream[i].u1ClipWriteIndx);
            Printf("  SA: 0x%08x EA: 0x%08x CA: 0x%08x\n", 
                    _rAudMixSndStream[i].u4AFifoSA,
                    _rAudMixSndStream[i].u4AFifoEA,
                    _rAudMixSndStream[i].u4DesAddr);
          #ifdef ALSA_MIXSND_PATH
            if (i==ALSA_MIXSND_STREAM_ID)
            {
                Printf("  RingFifo.SA: 0x%08x\n", _rAudMixSndRingFifo.u4SA);
                Printf("  RingFifo.EA: 0x%08x\n", _rAudMixSndRingFifo.u4EA);
                Printf("  RingFifo.RP: 0x%08x\n", _rAudMixSndRingFifo.u4RP);
                Printf("  RingFifo.WP: 0x%08x\n", _rAudMixSndRingFifo.u4WP);                
            }
          #endif
        }
    }
    else
    {
        Printf("_hAudFeedMixSndThread not available\n");
    }
}

extern void DSP_FlushInvalidateDCacheFree(UINT32 u4Addr, UINT32 u4Len);
void AUD_ClearMixSndClip(void)
{
    UINT32 u4SA = VIRTUAL(u4GetMixSoundStartByteAddr2());
    UINT32 u4EA = VIRTUAL(u4GetMixSoundStartByteAddr3() + u4GetMixSoundBufSize3());

    x_memset((VOID*)u4SA, 0x00, u4EA-u4SA);
    DSP_FlushInvalidateDCacheFree(u4SA,u4EA-u4SA);
}

#ifdef ALSA_MIXSND_PATH
extern void AUD_StopMixSndRingFifo(void); //DANIEL_DEBUG
void AUD_PlayMixSndRingFifo(UINT32 u4SampleRate, UINT8 u1StereoOnOff, UINT8 u1BitDepth, UINT32 u4BufferSize)
{
    AUD_StopMixSndRingFifo(); //DANIEL_DEBUG
    
    if (_hAudFeedMixSndThread)
    {
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].fgEnable = 1;
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4SampleRate = u4SampleRate;
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u1MixSndStereoOnOff = u1StereoOnOff;
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u1BitDepth = u1BitDepth;        
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].u4DesAddr &= 0xffffff00;//DANIEL_DEBUG

        _rAudMixSndRingFifo.u4SZ = u4BufferSize;
        _rAudMixSndRingFifo.u4SA = u4GetMixSoundStartByteAddr3();
        _rAudMixSndRingFifo.u4EA = _rAudMixSndRingFifo.u4SA + _rAudMixSndRingFifo.u4SZ;
        _rAudMixSndRingFifo.u4RP = _rAudMixSndRingFifo.u4WP = _rAudMixSndRingFifo.u4SA;
        _rAudMixSndRingFifo.u4Latency = u4Latency;
    }
    else
    {
        LOG(5, "[AUD_PlayMixSndRingFifo] _hAudFeedMixSndThread not available\n");
    }
}
#ifdef LINUX_TURNKEY_SOLUTION
EXPORT_SYMBOL(AUD_PlayMixSndRingFifo);
#endif

void AUD_StopMixSndRingFifo(void)
{
    if (_hAudFeedMixSndThread)
    {
        _rAudMixSndStream[ALSA_MIXSND_STREAM_ID].fgStop = 1;
         //DANIEL_DEBUG
        while (_rAudMixSndStream[ALSA_MIXSND_STREAM_ID].fgStop)
        {
            x_thread_delay(100);
        }
        AUD_ClearMixSndClip();
        //DANIEL_DEBUG
    }
    else
    {
        LOG(5, "[AUD_StopMixSndRingFifo] _hAudFeedMixSndThread not available\n");
    }    
}
#endif

#endif 
