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
 * $RCSfile: aud_slt.c,v $
 * $Revision: #2 $
 * $Revision: #2 $
 * $Revision: #2 $
 *
 *---------------------------------------------------------------------------*/

/** @file aud_slt.c
 *  Brief of file aud_slt.c.
 *  Details of file aud_slt.c (optional).
 */
//#define __MODEL_slt__
//#define AUD_EXTERN_FILE

//---------------------------------------------------------------------------
// Include files
//---------------------------------------------------------------------------
#ifdef __MODEL_slt__

#include "x_lint.h"

LINT_EXT_HEADER_BEGIN

#include "x_typedef.h"
#include "x_os.h"
#include "x_bim.h"
#include "x_assert.h"

#include "aud_debug.h"
#include "aud_slt.h"
#include "aud_if.h"
#include "aud_cfg.h"
#include "aud_hw.h"
#include "dsp_intf.h"
//#include "drv_slt.h"
#include "x_pinmux.h"
#include "x_drv_cli.h"
#include "codec_mt5365.h"

#include "drv_adsp.h"
//#include "dsp_slt.c"
//#include "dsp_intf.c"

// Rocky Huang 070607 for compile
#include "x_timer.h"
//#include "dsp_reg.h"
#define PTR_SEL_MAINDEC_SPDIF_LINE_IN   (0x1 << 8) 

#ifdef CC_5380_SLT
#define IAD_FROM_AIN1 0
#else
#define IAD_FROM_AIN1 1
#endif
LINT_EXT_HEADER_END

/****************************************************************************
** Local definitions
****************************************************************************/

// SLT common define
#define SLT_UNKNOWN_MODE				0
#define SLT_SORTING_MODE				1
#define SLT_FLASHING_MODE				2
#define SLT_TYPE_AUD 1

// Audio SLT sorting result
#define AUD_SORTING_OK                           0
#define AUD_SORTING_DTV_FAIL                 (-1)
#define AUD_SORTING_ATV_FAIL                 (-2)
#define AUD_SORTING_FAIL                        (-3)
#define AUD_SORTING_ATV_CAPTURE_FAIL (-4)
#define AUD_SORTING_ATV_DATA_FAIL       (-5)
#define AUD_SORTING_IAD_FAIL            (-6)
#define AUD_SORTING_ROM_FAIL       (-6)

// Remapping non-cache address 
#define NOR_FLASH_BASE_ADDR		                   (0xf8000000)						
#define NOR_FLASH_BASE_ADDR_NON_CACHE	     (0xf8000000)		/* Remapping non-cache address */
#define NOR_SLT_START_ADDR				     (0x600000)		// 5M
#define NOR_SLT_AUD_ADDR_OFFSET		     (0x000000)		// 0M

// DTV SLT setting
#define AUD_BITSTREAM_LEN                               (0x80000)           // 0.5M
#define AUD_SLT_CAPTURE_PERIOD                     (3000)   // 3 second
#define AUD_SLT_TIMEOUT                                   (AUD_SLT_CAPTURE_PERIOD * 2)

#define VALID_STREAM_LENGTH                           (0x1800 * 3)
#define LINE_IN_BUF_SA                                      (u4GetAFIFOStart(AUD_DEC_AUX))
#define LINE_IN_BUF_SZ                                      (u4GetAFIFOEnd(AUD_DEC_AUX) - u4GetAFIFOStart(AUD_DEC_AUX))
#define INT_CNT                                                  ((LINE_IN_BUF_SZ / (256 * 4)) - 100)

// FFT configuration
#define FFT_LEN_IN_POW2                 12
#define FFT_PIONT_NUM                     (4096)

// FFT result and threshold
#define FREQ_POINT                          14   // 1k/288k*4096
#define DC_VALUE_MAX_THRESHOLD  5
#define DC_VALUE_MIN_THRESHOLD   (0.5)
#define THD_PLUS_N_THRESHOLD       70000
#define MAG_MIN_THRESHOLD            (50)
#define MAG_MAX_THRESHOLD           (400)

#define SLT_CH_NUM                           2
#define FREQ_POINT_AD                       85  // 1K/48K*4096
#define THD_PLUS_N_THRESHOLD_AD            150000//60000

#define FREQ_POINT_ATD0                       85//128  // 1K/32K*4096
#define FREQ_POINT_ATD1                       86//129  // 1K/32K*4096

/****************************************************************************
** Local structures and enumerations
****************************************************************************/

/****************************************************************************
** Function prototypes
****************************************************************************/
void SLT_CaptureReady(UINT32 u4Type, UINT32 u4Pts);
extern void AUD_StopMemStr(void);
extern void inplace_fft(FLOAT *io_real, FLOAT *io_imag, INT32 len_in_pow2);

/****************************************************************************
** Local variable
****************************************************************************/

// For sampling rate 48 KHz
static UINT32 _u4SltStreamLen = (AUD_SLT_CAPTURE_PERIOD*48*2*2);
static INT32 _i4AudSltResult = 0;
//static HANDLE_T _hAudCaptureTimer = NULL;

static HANDLE_T _hSltAud;
static HANDLE_T _hSltAudFinish = NULL;
static HANDLE_T _hSltAudThread;

static UINT32 _u4SltAudMode = SLT_UNKNOWN_MODE;
static HAL_TIME_T _rTime;

#define AUD_SLT_COMPARED_LEN                  (_u4SltStreamLen * 3/5)

static FLOAT afRealPart[SLT_CH_NUM][FFT_PIONT_NUM];
static FLOAT afImagPart[SLT_CH_NUM][FFT_PIONT_NUM];
static DOUBLE adfMagnitued[SLT_CH_NUM][FFT_PIONT_NUM];

/****************************************************************************
** Local functions
****************************************************************************/

extern const UINT8 _au1Stream[];

/***********************************************************
* Function      : build_win
* Description   : build the window table for inplace_fft
* Parameter     : argu 1: (i): length
				  argu 2: (o): buffer address window real part
				  argu 3: (o): buffer address window image part
* Return        : None
***********************************************************/
void build_win(INT32 n, FLOAT *wr, FLOAT *wi)
{
    FLOAT w_r, w_i, wrc_r, wrc_i, wtmp;
    FLOAT *tp_r, *tp_i;
    INT32 i;
   
    if (n == 16) // 32 points FFT
    {
    //fValue = atan(1.0);
    //fValue = 0.785398163;
    //angle = (FLOAT)(4.0*fValue/n);
    //angle = 0.19634954;
    //angle = (FLOAT)(4.0*atan(1.0)/n);
    wrc_r = w_r = 0.98078528;
    wrc_i = w_i = -0.195090321;
    }
    else if (n == 2048) // 4096 points FFT
    {
    //angle = 2pi/N
    //angle = 0.00076699;
    //wrc_r = w_r = (FLOAT)cos(angle);
    //wrc_i = w_i = (FLOAT)-sin(angle);
    wrc_r = w_r = 0.999999705;
    wrc_i = w_i = -0.00153398;
    }
    	
    tp_r = wr;
    tp_i = wi;

    for (i = 1 ; i < n ; i++) 
    {
        *tp_r++ = (FLOAT)wrc_r;
        *tp_i++ = (FLOAT)wrc_i;

        wtmp = wrc_r*w_r - wrc_i*w_i;
        wrc_i = wrc_r*w_i + wrc_i*w_r;
        wrc_r = wtmp;
    }

}

/***********************************************************
* Function      : inplace_fft
* Description   : This is a in-place radix 2 FFT, i.e. output will directly 
				  overwrite the input.
* Parameter     : argu 1: (i/o): buffer address of the real part
				  argu 2: (i/o): buffer address of the image part
				  argu 3: (i): FFT length in power of 2
* Return        : None
***********************************************************/
void inplace_fft(FLOAT *io_real, FLOAT *io_imag, INT32 len_in_pow2)
{
    static FLOAT *d8Win_r;
    static FLOAT *d8Win_i;
    //static INT32 i4Len_old = 0;  // FIXME
    INT32 i4Len_old = 0;
    static INT32 i4n = 1;

    INT32 i, j, k, l, length, widx;

    FLOAT v_r, tmp_r, tm_r;
    FLOAT v_i, tmp_i, tm_i;

    FLOAT *iop_r, *ion_r, *tp_r, *wp_r;
    FLOAT *iop_i, *ion_i, *tp_i, *wp_i;

    if(len_in_pow2 == 0) return;  // if len_in_pow2 = 0, then do nothing.

    // build table
    if(len_in_pow2 != i4Len_old) 
    {
        if(i4Len_old != 0) 
        {   // relocate later
            x_mem_free(d8Win_r);
            x_mem_free(d8Win_i);
        }

        i4Len_old = len_in_pow2;
        i4n = 1 << len_in_pow2;
        length = i4n>>1;

        VERIFY( (d8Win_r = (FLOAT *) x_mem_calloc(length-1, sizeof(FLOAT))) != NULL );
        VERIFY( (d8Win_i = (FLOAT *) x_mem_calloc(length-1, sizeof(FLOAT))) != NULL );
        // build window
        build_win(length, d8Win_r, d8Win_i);
    }

    /* fft */
    length = i4n;
    widx = 1;
    for (l = 0 ; l < len_in_pow2 ; l++) 
    {
        length = length>>1;
        /* 1st */
        for(i = 0 ; i < i4n ; i = i + (length<<1)) 
        {
            iop_r = io_real + i;
            iop_i = io_imag + i;
            ion_r = iop_r + length;
            ion_i = iop_i + length;
            tmp_r = *iop_r + *ion_r;
            tmp_i = *iop_i + *ion_i;
            *ion_r = *iop_r - *ion_r;
            *ion_i = *iop_i - *ion_i;
            *iop_r = tmp_r;
            *iop_i = tmp_i;
        }

        /* 2nd */
        wp_r = d8Win_r + widx - 1;
        wp_i = d8Win_i + widx - 1;
        for (j = 1 ; j < length ; j++) 
        {
            v_r = *wp_r;
            v_i = *wp_i;
            for (i = j ; i < i4n ; i = i + (length<<1)) 
            {
                iop_r = io_real + i;
                iop_i = io_imag + i;
                ion_r = iop_r + length;
                ion_i = iop_i + length;
                tmp_r = *iop_r + *ion_r;
                tmp_i = *iop_i + *ion_i;
                tm_r = *iop_r - *ion_r;
                tm_i = *iop_i - *ion_i;
                *ion_r = tm_r*v_r - tm_i*v_i;
                *ion_i = tm_r*v_i + tm_i*v_r;
                *iop_r = tmp_r;
                *iop_i = tmp_i;
            }
            wp_r += widx;
            wp_i += widx;
        }
        widx <<= 1;
    }            

    /* bit reversing */
    j = 0;
    for (i = 1 ; i < (i4n-1) ; i++) 
    {
        k = i4n/2;
        while(k <= j) 
        {
            j = j - k;
            k = k/2;
        }
        j = j + k;
        if (i < j) 
        {
            iop_r = io_real + i; iop_i = io_imag + i;
            tp_r = io_real + j; tp_i = io_imag + j;
            tmp_r = *tp_r; tmp_i = *tp_i;
            *tp_r = *iop_r;
            *tp_i = *iop_i;
            *iop_r = tmp_r;
            *iop_i = tmp_i;
        }
    }
}

static void _sltAudInit(void)
{
    MEM_BUFFER_INFO_T rDecMemBuf;

    rDecMemBuf.pData = (UINT8 *)_au1Stream;
    rDecMemBuf.u4Length = AUD_BITSTREAM_LEN;

    LOG(5, "Audio stream addr = 0x%0x, len = %d\n", rDecMemBuf.pData, rDecMemBuf.u4Length);

    // Setup memory buffer address
    AUD_SetDecMemBuffer(AUD_DEC_MAIN, (const MEM_BUFFER_INFO_T *) &rDecMemBuf);
    // Feed audio bitstream from memory
    //CLI_Parser("aud.s 0 6 2");
    //AUD_SetDecType(AUD_DEC_MAIN,6, 2);
    AUD_SetDecType(AUD_DEC_MAIN,AUD_STREAM_FROM_MEMORY, AUD_FMT_AC3);
    AUD_SltLineInInit();
}
/*
static VOID _sltAudCaptureReady(HANDLE_T  pt_tm_handle, VOID *pv_tag)
{
    UNUSED(pt_tm_handle);
    UNUSED(pv_tag);
    
    // Turn off line-in buffer control	
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);

    // Stop stream source
    AUD_StopMemStr();
    
    SLT_CaptureReady(SLT_TYPE_AUD, 0);
}
*/
static VOID _sltAudCaptureReady1(void)
{
        
    // Turn off line-in buffer control	
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);

    // Stop stream source
    AUD_StopMemStr();    
//    SLT_CaptureReady(SLT_TYPE_AUD, 0);
}

extern const unsigned char _au1SltPcm[];

static INT32 _sltAudCompare(void)
{
    UINT8 * pu1ValidStr;
    UINT8 * pu1StrStart;
    INT32  i4Idx = 0;
    INT32  i4Ret = 0;

    pu1StrStart = (UINT8 *)VIRTUAL(u4GetAFIFOStart(AUD_DEC_AUX));

    // Search none zero byte as string start
    for (i4Idx = 0; i4Idx < _u4SltStreamLen; i4Idx++)
    {
        if ( *(pu1StrStart + i4Idx) != 0)
        {
            pu1ValidStr = (pu1StrStart + i4Idx);
            LOG(5, "Find stream start at addr = 0x%x , length = %d\n", pu1ValidStr, i4Idx);
            break;
        }
        else if (i4Idx == (_u4SltStreamLen - 1))
        {
            Printf("All stream are zero\n");
        }
    }

    i4Ret = x_memcmp((const VOID *) (pu1ValidStr), 
    	             (const VOID *) ( (NOR_SLT_START_ADDR + NOR_SLT_AUD_ADDR_OFFSET) | NOR_FLASH_BASE_ADDR), 
    	             AUD_SLT_COMPARED_LEN);
/*
    i4Ret = x_memcmp((const VOID *) (pu1ValidStr), 
    	             (const VOID *) (_au1SltPcm), 
    	             AUD_SLT_COMPARED_LEN);
  */  

    //Printf("SLT compared len = 0x%x, Ret = %d\n", AUD_SLT_COMPARED_LEN, i4Ret);
    return i4Ret;
}

static INT32 _sltAudFlash(void)
{
    UINT8 * pu1ValidStr;
    UINT8 * pu1StrStart;
    INT32 i4Idx;
    INT32 i4Ret = 0;
    pu1StrStart = (UINT8 *)u4GetAFIFOStart(AUD_DEC_AUX);

    // Search none zero byte as string start
    for (i4Idx = 0; i4Idx < _u4SltStreamLen; i4Idx++)
    {
        if ( *(pu1StrStart + i4Idx) != 0)
        {
            pu1ValidStr = (pu1StrStart + i4Idx);
            LOG(5, "Find addr = 0x%x , length = %d\n", pu1ValidStr, i4Idx);
            break;
        }
        else if (i4Idx == (_u4SltStreamLen - 1))
        {
            LOG(5, "All stream are zero\n");
            i4Ret = -1;
        }
    }
    
    // Write golden to flash
    CLI_Parser_Arg("nor.e 0x%x 0x%x", NOR_SLT_START_ADDR + NOR_SLT_AUD_ADDR_OFFSET, _u4SltStreamLen);
    CLI_Parser_Arg("nor.wr 0x%x 0x%x 0x%x", NOR_SLT_START_ADDR + NOR_SLT_AUD_ADDR_OFFSET, pu1ValidStr, _u4SltStreamLen);

    LOG(5, "Audio golden flashing completely\n");

    return i4Ret;
}

static void _sltAudThread(void *pvArgv)
{
    _i4AudSltResult = 0;
   
    // Audio slt init
    _sltAudInit();
    AUD_SET_BIT(REG_TEST_CFG, DTVSLT);
    AUD_DspBypassPostProcess(AUD_DEC_MAIN,TRUE);
    VERIFY(x_sema_create(&_hSltAud, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

    //CLI_Parser("aud.io.cfg.isw 0");	
    //AUD_AvMuxSel(0);
    AUD_AvMuxSel(AUD_INPUT_ID_COMP_VID_0);
    AUD_WRITE32(REG_EXTADC_CFG, 0x172e); 
    // Start playing audio
    //CLI_Parser("aud.play");
     AUD_DSPCmdPlay(AUD_DEC_MAIN);   
    
#if 1
    // Work around for error data generated at the first decoder play
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, TRUE); 
    x_thread_delay(10);     
    AUD_DSPCmdStop(AUD_DEC_MAIN);
    x_thread_delay(20); 	
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);        
    //CLI_Parser("aud.play");
    AUD_DSPCmdPlay(AUD_DEC_MAIN);   
#endif
 
    // Open line-in and start capturing data
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, TRUE);    

#if 0
    HAL_GetTime(&_rTime);
    LOG(5, "Audio start capturing .. Time (%d, %d)\n", _rTime.u4Seconds, _rTime.u4Micros);

    VERIFY(x_timer_create(&_hAudCaptureTimer) == OSR_OK);
    VERIFY(x_timer_start(_hAudCaptureTimer, AUD_SLT_CAPTURE_PERIOD, X_TIMER_FLAG_ONCE, 
		                 _sltAudCaptureReady, NULL) == OSR_OK);

    // Waiting for audio capture ready
    VERIFY(x_sema_lock(_hSltAud, X_SEMA_OPTION_WAIT) == OSR_OK);

    HAL_GetTime(&_rTime);
    LOG(5, "Audio stop capturing .. Time (%d, %d)\n", _rTime.u4Seconds, _rTime.u4Micros);
#else
    x_thread_delay(AUD_SLT_CAPTURE_PERIOD);
    _sltAudCaptureReady1();
#endif
    // Notes: use async stop instread of sync stop for fail IC, DSP won't ack flush done ..	
    AUD_DSPCmdStop(AUD_DEC_MAIN);
    
    // Delay to insure DSP has finished stop command	
    x_thread_delay(20); 	
    //AUD_UopCommandDone(AUD_DEC_MAIN, AUD_CMD_STOP);
    //AUD_CLR_BIT_SPECIAL(REG_TEST_CFG, DTVSLT);
    
    if (_u4SltAudMode == SLT_SORTING_MODE)
    {	
        // Audio comparition with golden pattern
        if (_sltAudCompare() == 0)
        {
            // pattern match
            _i4AudSltResult = 0;
            LOG(5, "Aud compare match (ok) \n");
        }
        else
        {
            // pattern unmatch
            _i4AudSltResult = -1;
            LOG(5, "Aud compare unmatch (fail) \n");
        }
		
        if (_hSltAudFinish != NULL)
        {
            //Printf("Realse aud semaphore\n");
            VERIFY(x_sema_unlock(_hSltAudFinish) == OSR_OK);
        }
    }
    else if (_u4SltAudMode == SLT_FLASHING_MODE)
    {
        if (_sltAudFlash() != 0)
        {
            _i4AudSltResult = -1;
            Printf("Flash audio golden fail!\n");
        }
        else
        {
            _i4AudSltResult = 0;
        }
    }
    else
    {
    }

   // VERIFY(x_timer_delete(_hAudCaptureTimer) == OSR_OK);
   // VERIFY(x_sema_delete(_hSltAud) == OSR_OK);
   // AUD_DspBypassPostProcess(AUD_DEC_MAIN,FALSE);
    // destroy thread
    x_thread_exit();
}        

static void _sltDTV(void)
{
    _i4AudSltResult = 0;
   
    // Audio slt init
    _sltAudInit();
    AUD_SET_BIT(REG_TEST_CFG, DTVSLT);
    AUD_DspBypassPostProcess(AUD_DEC_MAIN,TRUE);
    //VERIFY(x_sema_create(&_hSltAud, X_SEMA_TYPE_BINARY, X_SEMA_STATE_LOCK) == OSR_OK);

    //CLI_Parser("aud.io.cfg.isw 0");	
    //AUD_AvMuxSel(0);
    AUD_AvMuxSel(AUD_INPUT_ID_COMP_VID_0);
    AUD_WRITE32(REG_EXTADC_CFG, 0x172e); 
    // Start playing audio
    //CLI_Parser("aud.play");
     AUD_DSPCmdPlay(AUD_DEC_MAIN);   
    
#if 1
    // Work around for error data generated at the first decoder play
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, TRUE); 
    x_thread_delay(10);     
    AUD_DSPCmdStop(AUD_DEC_MAIN);
    x_thread_delay(20); 	
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);        
    //CLI_Parser("aud.play");
    AUD_DSPCmdPlay(AUD_DEC_MAIN);   
#endif
 
    // Open line-in and start capturing data
    //AUD_LineInCtrlEnable(AUD_DEC_MAIN, TRUE);    
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, TRUE);
#if defined(CC_MT5396)
    AUD_SET_BIT(REG_INTADC_SDADC0, RST_FILTER | 0x40);
#else
    AUD_SET_BIT(REG_INTADC_SDADC0, RST_FILTER);
#endif
    AUD_CLR_BIT(REG_INTADC_SDADC0, RST_FILTER);

#if 0
    HAL_GetTime(&_rTime);
    LOG(5, "Audio start capturing .. Time (%d, %d)\n", _rTime.u4Seconds, _rTime.u4Micros);

    VERIFY(x_timer_create(&_hAudCaptureTimer) == OSR_OK);
    VERIFY(x_timer_start(_hAudCaptureTimer, AUD_SLT_CAPTURE_PERIOD, X_TIMER_FLAG_ONCE, 
		                 _sltAudCaptureReady, NULL) == OSR_OK);

    // Waiting for audio capture ready
    VERIFY(x_sema_lock(_hSltAud, X_SEMA_OPTION_WAIT) == OSR_OK);

    HAL_GetTime(&_rTime);
    LOG(5, "Audio stop capturing .. Time (%d, %d)\n", _rTime.u4Seconds, _rTime.u4Micros);
#else
    x_thread_delay(AUD_SLT_CAPTURE_PERIOD);
    _sltAudCaptureReady1();
#endif
    // Notes: use async stop instread of sync stop for fail IC, DSP won't ack flush done ..	
    AUD_DSPCmdStop(AUD_DEC_MAIN);
    
    // Delay to insure DSP has finished stop command	
    x_thread_delay(180); 	
    //AUD_UopCommandDone(AUD_DEC_MAIN, AUD_CMD_STOP);
    //AUD_CLR_BIT_SPECIAL(REG_TEST_CFG, DTVSLT);
    // Disable Line-In  interrupt
    //VERIFY(BIM_DisableIrq(VECTOR_SPDIF));
    
    if (_u4SltAudMode == SLT_SORTING_MODE)
    {	
        // Audio comparition with golden pattern
        if (_sltAudCompare() == 0)
        {
            // pattern match
            _i4AudSltResult = 0;
            LOG(5, "Aud compare match (ok) \n");
        }
        else
        {
            // pattern unmatch
            _i4AudSltResult = -1;
            LOG(5, "Aud compare unmatch (fail) \n");
        }
#if 0
        if (_hSltAudFinish != NULL)
        {
            //Printf("Realse aud semaphore\n");
            VERIFY(x_sema_unlock(_hSltAudFinish) == OSR_OK);
        }
#endif        
    }
    else if (_u4SltAudMode == SLT_FLASHING_MODE)
    {
        if (_sltAudFlash() != 0)
        {
            _i4AudSltResult = -1;
            Printf("Flash audio golden fail!\n");
        }
        else
        {
            _i4AudSltResult = 0;
        }
    }
    else
    {
    }

   // VERIFY(x_timer_delete(_hAudCaptureTimer) == OSR_OK);
   // VERIFY(x_sema_delete(_hSltAud) == OSR_OK);
   // AUD_DspBypassPostProcess(AUD_DEC_MAIN,FALSE);
    // destroy thread
    //x_thread_exit();
}        


INT32 _sltGetDtvResult(void)
{
    UINT32 i4RValue;

	if (_u4SltAudMode == SLT_SORTING_MODE)
	{
		VERIFY(x_sema_create(&_hSltAudFinish, X_SEMA_TYPE_BINARY, 
			   X_SEMA_STATE_LOCK) == OSR_OK);

		//Printf("Aud create audio semaphore\n");
	
		if ((x_sema_lock_timeout(_hSltAudFinish, AUD_SLT_TIMEOUT)) != OSR_OK)
		{
		    LOG(5, "Audio SLT get result time out!\n");
                  // fail
                  _i4AudSltResult = AUD_SORTING_DTV_FAIL;
		    return AUD_SORTING_DTV_FAIL;
		}
		else
		{
                  return _i4AudSltResult;
		}
	}
	else
	{
            LOG(5, "Only sorting mode can get result\n");
	}

	VERIFY(x_sema_delete(_hSltAudFinish) == OSR_OK);
	_hSltAudFinish = NULL;

	return i4RValue;
}


UINT32 _sltGetAoutStrAddr(UINT32 * pu4StrLen)
{
    UINT32 u4CompactAddr;   
    u4CompactAddr =((((AUD_READ32(REG_ADSP_CFG) & ADSP_AUDIO_BLOCK_MASK))>>ADSP_AUDIO_BLOCK_POS) <<24)| 
        ((AUD_READ32(REG_DSP_CMPT_DRAM_SBLK01) & 0x0000ffff)  << 8);
    if (pu4StrLen)
    {
        *pu4StrLen = (0xd80*4);
    }    
    return (u4CompactAddr + (0x1000)*4);
}

BOOL _sltAtdPathVerify(VOID)
{
    UINT8 * pu1Buf;
    UINT8   u1MsbH, u1MsbL, u1Lsb;
    BOOL    fgSign;
    INT32   i4Value, i;
    UINT32  u4StrLen;
    UINT32  u4FFTNum;
    FLOAT   fMagSum = 0;
    DOUBLE dThdPlusN;
    UINT32  u4MaxFreqPoint = 0;
    DOUBLE dfTemp = 0;
    CHAR cBuf[32];
    UNUSED(cBuf);
    pu1Buf = (UINT8 *)_sltGetAoutStrAddr(&u4StrLen);
    u4FFTNum = u4StrLen/3;
    LOG(5,"buf address %d \n", pu1Buf); 
    LOG(5,"input samples %d \n", u4FFTNum); 

    // Load data
    for (i=0 ; i<u4FFTNum; i++)
    {  
#if 0    
        u1MsbH = *pu1Buf;
        u1MsbL = *(pu1Buf+1);
        u1Lsb  = *(pu1Buf+2);
#else
        u1Lsb = *pu1Buf;
        u1MsbL = *(pu1Buf+1);
        u1MsbH = *(pu1Buf+2);
#endif

        fgSign = ((u1MsbH & 0x80) == 0x80) ? TRUE: FALSE;
        i4Value = ((u1MsbH & 0x7f) << 16) + (u1MsbL << 8) + u1Lsb;
        afRealPart[0][i] = (FLOAT)((fgSign == TRUE) ? (i4Value - 0x800000) : i4Value);
        afRealPart[0][i] = afRealPart[0][i]/80000;
        afImagPart[0][i] = 0;
        pu1Buf += 3;
    }

    LOG(5,"afRealPart[0][i] %d \n", afRealPart[0][1]); 
    // FFT
    inplace_fft(afRealPart[0], afImagPart[0], FFT_LEN_IN_POW2);

    // Compute magnitued
    for (i=0 ; i<FFT_PIONT_NUM; i++)
    { 
        adfMagnitued[0][i] = afRealPart[0][i]*afRealPart[0][i] + afImagPart[0][i]*afImagPart[0][i];
    }   

    // Normalized
    for (i=0 ; i<FFT_PIONT_NUM; i++)
    { 
        //adfMagnitued[0][i] = adfMagnitued[0][i] / 237000;
        //adfMagnitued[0][i] = adfMagnitued[0][i] / 6370000;
        adfMagnitued[0][i] = adfMagnitued[0][i] / 3000000;//1250000; // Tune for mt5388
    }    

#if 0 // linking error
    LOG(5,"DC value = %s\n", ftoa(cBuf, (FLOAT)adfMagnitued[0], 6));
#endif

/*    
    // Check DC value
    if ((adfMagnitued[0] > DC_VALUE_MAX_THRESHOLD) ||
    	 (adfMagnitued[0] < DC_VALUE_MIN_THRESHOLD))
    {
        LOG(5,"invalid DC value\n");  
        //return FALSE;
    }
*/
    
    // Check maximum magnitude
    for (i=0; i<((FFT_PIONT_NUM/2) - 1) ; i++)
    {
        if (adfMagnitued[0][i] > dfTemp)
        {
            u4MaxFreqPoint = i;   
            dfTemp = adfMagnitued[0][i];
        }
    }

    // Compute THD+N
    for (i=0 ; i<FFT_PIONT_NUM; i++)
    { 
        fMagSum += adfMagnitued[0][i]*adfMagnitued[0][i];
    }   
   
    dThdPlusN =  (UINT32)(fMagSum - adfMagnitued[0][u4MaxFreqPoint]*adfMagnitued[0][u4MaxFreqPoint]);

#if 0 // linking error
    LOG(5,"THD+N = %s\n",ftoa(cBuf, (FLOAT)dThdPlusN, 6));
#endif

    LOG(1,"ATV THD+N = %d \n", (INT32)dThdPlusN); 
    if (dThdPlusN > THD_PLUS_N_THRESHOLD)
    {
        LOG(5,"Invalid THD+N value \n");
        return FALSE;
    }
    
#if 0 // linking error
    LOG(5,"u4MaxMag[%d] = %s\n", u4MaxFreqPoint, ftoa(cBuf, (FLOAT)adfMagnitued[u4MaxFreqPoint], 6));
#endif

    LOG(1,"ATV adfMagnitued = %d, %d \n",u4MaxFreqPoint, (INT32)adfMagnitued[0][u4MaxFreqPoint]); 
    printf("\n\nu4MaxFreqPoint=%d\n\n",u4MaxFreqPoint);
    if ((u4MaxFreqPoint < FREQ_POINT_ATD0) || 
        (u4MaxFreqPoint > FREQ_POINT_ATD1) ||     	
    	 (adfMagnitued[0][u4MaxFreqPoint] < MAG_MIN_THRESHOLD) || 
    	 (adfMagnitued[0][u4MaxFreqPoint] > MAG_MAX_THRESHOLD))
    {
        LOG(5,"Invalid Max Freq\n");
        return FALSE;
    }   
    return TRUE;
}


INT32 _sltAtdSorting(void)
{
    UINT32 u4Data;

    // Set AUDIN_CFG
    u4Data = (AUD_READ32(REG_AUDIN_CFG) & (~REG_AUDIN_CFG_MASK));
    u4Data = u4Data | APGA_GAIN_0DB | APGA_OFFSET_0V | APGA_GAIN_BY_DSP | ADCK_INV_RISING | ADC_SEL_DEMOD;
    AUD_WRITE32(REG_AUDIN_CFG, u4Data);
    DSP_ChangeSourceMode(1);
    
    //x_thread_delay(2000);
    // Power on DAC1 R channel only
    AUD_WRITE32(REG_IDACHCG, 0);
    AUD_DspBypassPostProcess(AUD_DEC_MAIN,TRUE);
    
    //DSP_ChangeSourceMode(1);
    
    CLI_Parser("aud.s 0 2 11");
    CLI_Parser("aud.play");
    CLI_Parser("aud.uop.v 0 50");
    CLI_Parser("aud.t.s 0");       
    
    x_thread_delay(1200);
    
    AUD_DSPCmdStopAsyn(AUD_DEC_MAIN);
    // Delay to insure DSP has finished stop command	
    x_thread_delay(200); 	
    AUD_UopCommandDone(AUD_DEC_MAIN, AUD_CMD_STOP);
    AUD_DspBypassPostProcess(AUD_DEC_MAIN,FALSE);

    if (_sltAtdPathVerify() != TRUE)
    {        
        return AUD_SORTING_ATV_FAIL;
    }
 
    return AUD_SORTING_OK;
}

INT32 _sltAtdEuSorting(void)
{
    return AUD_SORTING_OK;
}

// check 1 or 2 channels
BOOL _sltIntAdPathVerify(UINT8 u1ChNum)
{
    UINT8 * pu1Buf;
    UINT8   u1MsbH, u1MsbL, u1Lsb;
    BOOL    fgSign;
    INT32   i4Value, i, q;
    FLOAT   fDcValue[2] = {0};
    FLOAT   fMagSum[2] = {0};
    DOUBLE dThdPlusN[2];
    UINT32  u4MaxFreqPoint = 0;
    DOUBLE dfTemp = 0;
    BOOL fgResult = TRUE;
    
    pu1Buf = (UINT8 *)LINE_IN_BUF_SA + 6*5000;  //ignore the first 11000 samples
    //pu1Buf = (UINT8 *)LINE_IN_BUF_SA;
    // Load data
    for (i=0 ; i<FFT_PIONT_NUM; i++)
    {  
        for (q=0 ; q<2; q++)     // Jessica: Fix to 2 channels for Line in
        {
            u1MsbH = *pu1Buf;
            u1MsbL = *(pu1Buf+1);
            u1Lsb  = *(pu1Buf+2);

            fgSign = ((u1MsbH & 0x80) == 0x80) ? TRUE: FALSE;
            i4Value = ((u1MsbH & 0x7f) << 16) + (u1MsbL << 8) + u1Lsb;
            afRealPart[q][i] = (FLOAT)((fgSign == TRUE) ? (i4Value - 0x800000) : i4Value);
            afImagPart[q][i] = 0;
            pu1Buf += 3;
        }        
    }

    for (q=0 ; q<u1ChNum; q++)
    {    
        // Remove DC value and normalize data
        for (i=0 ; i<FFT_PIONT_NUM; i++)
        {
            fDcValue[q] += afRealPart[q][i] / FFT_PIONT_NUM;
        }

        for (i=0 ; i<FFT_PIONT_NUM; i++)
        {      
            afRealPart[q][i] = afRealPart[q][i] - fDcValue[q];
            afRealPart[q][i] = afRealPart[q][i] / 0x07ffff;
        }

        // FFT
        inplace_fft(afRealPart[q], afImagPart[q], FFT_LEN_IN_POW2);

        for (i=0 ; i<FFT_PIONT_NUM; i++)
        { 
            // Compute magnitude
            adfMagnitued[q][i] = afRealPart[q][i]*afRealPart[q][i] + afImagPart[q][i]*afImagPart[q][i];
            // Normalized
           adfMagnitued[q][i] = adfMagnitued[q][i] / 440000;///
            // Compute THD+N
            fMagSum[q] += adfMagnitued[q][i]*adfMagnitued[q][i];
        }    
   
        dThdPlusN[q] =  (UINT32)(fMagSum[q] - adfMagnitued[q][FREQ_POINT_AD]*adfMagnitued[q][FREQ_POINT_AD]);

        LOG(1, "In-AD THD+N = %d \n", (INT32)dThdPlusN[q]); 
        if ((dThdPlusN[q] > THD_PLUS_N_THRESHOLD_AD))
        {
            LOG(5,"Internal AD SLT failed at THD+N of ch %d \n", q);
            fgResult = FALSE;
       }

        // Check maximum magnitude of channel 1
        for (i=0; i<((FFT_PIONT_NUM/2) - 1) ; i++)
        {
            if (adfMagnitued[q][i] > dfTemp)
            {
                u4MaxFreqPoint = i;   
                dfTemp = adfMagnitued[q][i];
            }
        }

        LOG(1,"In-AD adfMagnitued = %d \n", (INT32)adfMagnitued[q][u4MaxFreqPoint]); 
        //LOG(5,"u4MaxMag[%d] = %s\n", u4MaxFreqPoint, ftoa(cBuf, (FLOAT)adfMagnitued[u4MaxFreqPoint], 6));
        printf("\n\nu4MaxFreqPoint=%d\n\n",u4MaxFreqPoint);
        if ((u4MaxFreqPoint != FREQ_POINT_AD) || 
        	 (adfMagnitued[q][u4MaxFreqPoint] < MAG_MIN_THRESHOLD) || 
        	 (adfMagnitued[q][u4MaxFreqPoint] > MAG_MAX_THRESHOLD))
        {
            LOG(5,"Invalid Max Freq of ch %d \n", q);
            fgResult = FALSE;
        }
    }
    
    return fgResult;
}

INT32 _sltIntAdSorting(UINT8 u1DacId, UINT8 u1ChNum)
{
    UINT32 u4DacPwdReg;
    UNUSED(u4DacPwdReg);
    // Power on only 1 DAC
//    u4DacPwdReg = (0x77 & ~(0x11 << u1DacId)) << 4;
  #if defined(CC_MT5368) || defined(CC_MT5396) || defined(CC_MT5389)
    #if defined(CC_MT5389)
    ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) & ~ADC_INPUT_MUX_MASK) | ADC_IN2L2R);
	#else
	ANA_WRITE32(REG_AADC_CFG0, (ANA_READ32(REG_AADC_CFG0) & ~ADC_INPUT_MUX_MASK) | ADC_IN3L3R);
	#endif
    ANA_CLR_BIT(REG_AADC_CFG0, (ADC_L_PD | ADC_R_PD)); // power on
  #else
    AUD_WRITE32(REG_INTADC_SDADC2, (AUD_READ32(REG_INTADC_SDADC2) & ~ADC_INPUT_MUX_MASK) | ADC_IN3L3R);
  #endif  
    AUD_WRITE32(REG_ACK2_CFG, 0x22);        
    vSetSPDIFInFIFO(AUD_DEC_MAIN, LINE_IN_BUF_SA, LINE_IN_BUF_SA + LINE_IN_BUF_SZ );    
    AUD_WRITE32(REG_LIN_CFG0, SPDIF_LINE_IN_DISABLE | 
    	                       DATA_PCM_24_BITS |
    	                       RISC_INT_PERIOD_256 |
    	                       PCM_CH_NUM_2_CH |
    	                       INT_ADC_INTLIN);
    AUD_WRITE32(REG_EXTADC_CFG, 0x172e);            	                       

    AUD_WRITE32(REG_IDACHCG, 0);    

    // play test tone from first decoder 
#if defined(CC_MT5389)
    CLI_Parser("aud.t.t 0");
#else
    CLI_Parser("aud.t.t 1");
#endif
   #if 0
   MEM_BUFFER_INFO_T  rMemBuf;
   AUD_PCM_SETTING_T  rPcmSetting;
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
    AUD_StrSelect(AUD_DEC_MAIN, 0, &rMemBuf);
    AUD_SetDecMemBuffer(AUD_DEC_MAIN, &rMemBuf);
    AUD_DSPCmdPlay(AUD_DEC_MAIN);
    #endif
#if defined(CC_MT5389)
		CLI_Parser("aud.t.b 1");
#else
		CLI_Parser("aud.t.s 0");	
		//AUD_DspSoundEffectFlag(AUD_DEC_MAIN, FALSE);
		
		CLI_Parser("aud.uop.v 0 100");	
		//AUD_DspMasterVolume(AUD_DEC_MAIN, 100);
#endif


    
    
    // Turn on line-in buffer control	
    //AUD_LineInCtrlEnable(AUD_DEC_MAIN, TRUE);
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, TRUE);
#if defined(CC_MT5396)    
    AUD_SET_BIT(REG_INTADC_SDADC0, RST_FILTER | 0x40);
#else
    AUD_SET_BIT(REG_INTADC_SDADC0, RST_FILTER);
#endif
    AUD_CLR_BIT(REG_INTADC_SDADC0, RST_FILTER);
   
    x_thread_delay(1200);

    // Turn off line-in buffer control	
    AUD_LineInCtrlEnable(AUD_DEC_MAIN, FALSE);

    AUD_StopMemStr();
    // Notes: use async stop instread of sync stop for fail IC, DSP won't ack flush done ..	
    AUD_DSPCmdStopAsyn(AUD_DEC_MAIN);

    //CLI_Parser("aud.stop");
    // Delay to insure DSP has finished stop command	
    x_thread_delay(200); 	
    AUD_UopCommandDone(AUD_DEC_MAIN, AUD_CMD_STOP);

    // Power on all DAC

    if (_sltIntAdPathVerify(u1ChNum) != TRUE)
    {
        LOG(1, "Internal DAC%d fail\n", u1DacId);
        return AUD_SORTING_IAD_FAIL;
    }

    LOG(1, "Internal DAC%d pass\n", u1DacId);
    return AUD_SORTING_OK;
}

void AUD_SltLineInInit(void)
{

    vSetSPDIFInFIFO(AUD_DEC_MAIN, LINE_IN_BUF_SA, LINE_IN_BUF_SA + LINE_IN_BUF_SZ );
    AUD_WRITE32(REG_LIN_CFG0, SPDIF_LINE_IN_DISABLE | 
    	                       DATA_PCM_24_BITS |
    	                       RISC_INT_PERIOD_256 |
    	                       PCM_CH_NUM_2_CH);
    AUD_SET_BIT(REG_BS_CFG, PTR_SEL_BANK0_SPDIF_LINE_IN);

    // Clear line in buffer
    
    x_memset((VOID *)VIRTUAL(LINE_IN_BUF_SA), 0x80, LINE_IN_BUF_SZ);

    // Line in format config
    // AUD_LineInCfg(); // TODO

    // Enable Line-In  interrupt
    //VERIFY(BIM_EnableIrq(VECTOR_SPDIF));
}
INT32 SLT_DtvSorting(void)
{
    INT32 i4AudResult;
    // DTV part sorting
    ADAC_Mute(FALSE);

    _u4SltAudMode = SLT_SORTING_MODE;
#if 0
    if (x_thread_create(&_hSltAudThread, "SLT_AUD", 2048, CLI_THREAD1_PRIORITY, _sltAudThread, 0, NULL) != OSR_OK)
    {
         LOG(5,"Create SLT_AUD thead fail!\n");
    }
    i4AudResult = _sltGetDtvResult();
#else
    _sltDTV();
     //x_thread_delay(AUD_SLT_TIMEOUT);
    i4AudResult = _i4AudSltResult;              
#endif
    //x_thread_delay(200);
    return i4AudResult;
}
        
INT32 SLT_AtvSorting(void)
{    
    return TRUE;
}

INT32 SLT_AtdSorting(void)
{
    return _sltAtdSorting();
}

INT32 SLT_AtdEuSorting(void)
{
    return _sltAtdEuSorting();
}

INT32 SLT_IntAdSorting(void)
{
    INT32 i4AudResult;
    ADAC_Mute(FALSE);
    
    i4AudResult = _sltIntAdSorting(INTERNAL_DAC_ID_MAIN, 2);
    if (i4AudResult != AUD_SORTING_OK)
    {
        return i4AudResult;
    }

    i4AudResult = _sltIntAdSorting(INTERNAL_DAC_ID_AUX, 1);
    if (i4AudResult != AUD_SORTING_OK)
    {
        return i4AudResult;
    }    

    //sunman  PWM only 2 outputs
    //i4AudResult = _sltIntAdSorting(INTERNAL_DAC_ID_THIRD, 2);
    return i4AudResult;
}

#define AUD_ROMCOD        0xFC

INT32 _sltRomSorting(void)
{
    UINT32 u4Data;
    BOOL    fgResult = FALSE ;
    UINT32 u4Counter1=0;
    UINT32 u4Counter2=0;
    UNUSED(fgResult);
    x_thread_delay(300);
    //disable DSP
    u4Data = (AUD_READ32(REG_ADSP_CFG) & (~ADSP_CFG_RST_B) );
    AUD_WRITE32(REG_ADSP_CFG, u4Data); 
    
    // Load Rom Code
    DSP_LoadRamCode (AUD_ROMCOD);
    //DSP_LoadRamCode (AUD_COMMRAM);
    //DSP_LoadRamCode (AUD_AININTRAM);
    // Enable Ram Boot
    vSetDspBootFromRam ();

    // enable DSP    
    vTriggerDSP();

    x_thread_delay(5);
    DSP_ReadDecDram(0x20, &u4Counter1);
    x_thread_delay(10);
    DSP_ReadDecDram(0x20, &u4Counter2);
    LOG(5, "u4Counter1,  u4Counter2= (%d, %d)\n", u4Counter1, u4Counter2);

    if (u4Counter1==u4Counter2)
    {
        return AUD_SORTING_ROM_FAIL;
    }

    return AUD_SORTING_OK;


}
INT32 SLT_ROMSorting(void)
{
    BOOL fgFail = FALSE;
    UINT32 u4Data;

    fgFail =  _sltRomSorting();

    x_thread_delay(300);
    //disable DSP & set to rom boot
    u4Data = AUD_READ32(REG_ADSP_CFG);
    AUD_WRITE32(REG_ADSP_CFG, ((u4Data & (~ADSP_BOOT_FROM_RAM)) & (~ADSP_CFG_RST_B)));
    vTriggerDSP();

    if (fgFail)
    {
        return AUD_SORTING_ROM_FAIL;
    }

    return AUD_SORTING_OK;
}

void SLT_CaptureReady(UINT32 u4Type, UINT32 u4Pts)
{
	if (u4Type == SLT_TYPE_AUD)
	{
        HAL_GetTime(&_rTime);
        LOG(5, "Audio capture ready = (%d, %d)\n", _rTime.u4Seconds, _rTime.u4Micros);
		VERIFY(x_sema_unlock(_hSltAud) == OSR_OK);
	}	
}

void SLT_FlashingAudInit(void)
{
	// Audio slt init
	_sltAudInit();

	_u4SltAudMode = SLT_FLASHING_MODE;

	if (x_thread_create(&_hSltAudThread, "SLT_AUD", 2048, CLI_THREAD1_PRIORITY, 
		                _sltAudThread, 0, NULL) != OSR_OK)
	{
		Printf("Create SLT_AUD thead fail!\n");
	}
}

INT32 SLT_SortingAud(void)
{
    INT32 i4AudResult = AUD_SORTING_OK;

    i4AudResult = SLT_DtvSorting();
    LOG(5,"SLT_DtvSorting = %d\n",i4AudResult);
    
    if (i4AudResult != AUD_SORTING_OK)
    {
    	return AUD_SORTING_DTV_FAIL;
    }
    
    i4AudResult = SLT_AtvSorting();
    LOG(5,"SLT_AtvSorting = %d\n",i4AudResult);
    if (i4AudResult != AUD_SORTING_OK)
    {
    	return AUD_SORTING_ATV_FAIL;
    }
    
    
    i4AudResult = SLT_IntAdSorting();
    LOG(5,"SLT_IntAdSorting = %d\n",i4AudResult);
    
    
    return i4AudResult;
    
}

#endif // __MODEL_slt__

