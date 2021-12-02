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
 * $RCSfile: io_papi_src_src.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_src_src.c
 *  PAPI kernel driver implementation: Src selection
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "x_typedef.h"
#include "nptv_if.h"
#include "source_table.h"
#include "vdp_if.h"
#include "mpv_if.h"
#include "aud_if.h"
#include "aud_drvif.h"
#include "aud_dsp_cfg.h"
#include "fbm_drvif.h"
#include "x_assert.h"
#include "papi.h"
#include <linux/papi_ioctl.h>
#include <linux/mt53xx_cb.h>


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------
#define AUD_DSP_CMD_ASYNC 0

//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
/// Common error code
#define PAPI_SRC_SRC_FAIL                     (-0x800)

//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------

/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size) \
    if (!access_ok(VERIFY_READ, (void __user *)(src), (size))) \
    { \
        return PAPI_SRC_SRC_FAIL ; \
    } \
    if (copy_from_user((dst), (void __user *)(src), (size))) \
    { \
        return PAPI_SRC_SRC_FAIL ; \
    }

/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
    if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size))) \
    { \
        return PAPI_SRC_SRC_FAIL ; \
    } \
    if (copy_to_user((void __user *)(dst), (src), (size))) \
    { \
        return PAPI_SRC_SRC_FAIL ; \
    }


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------
typedef struct _PAPI_Video_Src_Pair
{
    int i4Src;
    Nat32 u4Dsg;
} PAPI_Video_Src_Pair;

typedef struct _PAPI_Audio_Src_Pair
{
    int i4Node;
    int i4Src;
    Nat32 u4Dsg;    
} PAPI_Audio_Src_Pair;

static PAPI_Video_Src_Pair _arVideoPairActual[2] = 
{
    {papi_src_srcid_SrcNone, papi_src_dsgid_DesignatorNone},
    {papi_src_srcid_SrcNone, papi_src_dsgid_DesignatorNone}
};

static PAPI_Video_Src_Pair _arVideoPairSet[2] = 
{
    {papi_src_srcid_SrcNone, papi_src_dsgid_DesignatorNone},
    {papi_src_srcid_SrcNone, papi_src_dsgid_DesignatorNone}
};

static PAPI_Audio_Src_Pair _arAudioPairActual[2] = 
{
    {0, papi_src_srcid_SrcNone, papi_src_dsgid_DesignatorNone},
    {0, papi_src_srcid_SrcNone, papi_src_dsgid_DesignatorNone}
};

static PAPI_Audio_Src_Pair _arAudioPairSet[2] = 
{
    {0, papi_src_srcid_SrcNone, papi_src_dsgid_DesignatorNone},
    {0, papi_src_srcid_SrcNone, papi_src_dsgid_DesignatorNone}
};

static UINT8 _au1SrcMap[256] =
{
    SV_VS_MAX, // -1
    SV_VS_MAX, // 0
    SV_VS_ATV1, // 1
    SV_VS_CVBS2, // 2
    SV_VS_CVBS1, // 3
    SV_VS_S1, // 4
    SV_VS_YPbPr1, // 5
    SV_VS_YPbPr2, // 6
    SV_VS_MAX, // 7
    SV_VS_MAX, // 8
    SV_VS_MAX, // 9
    SV_VS_MAX, // 10
    SV_VS_MAX, // 11
    SV_VS_MAX, // 12
    SV_VS_HDMI1, // 13
    SV_VS_HDMI2, // 14
    SV_VS_HDMI3, // 15
    SV_VS_HDMI4, // 16
    SV_VS_MAX, // 17
    SV_VS_DTV1, // 18
    SV_VS_MAX, // 19
    SV_VS_VGA, // 20
};

static AUD_DEC_STREAM_FROM_T _aeStrSrc[2] = {AUD_STREAM_FROM_DIGITAL_TUNER, AUD_STREAM_FROM_DIGITAL_TUNER};
//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------

/// Put a callback event
EXTERN INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);


//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------

static BOOL _fgCheckVideoPair(UINT32 u4Main, UINT32 u4Sub)
{
    return TRUE;
}

static BOOL _fgIsVideoSelectionAllowed(UINT32 u4Src, UINT32 u4Designator)
{
    return TRUE;
}

static BOOL _fgIsAudioSelectionAllowed(UINT32 u4Src, UINT32 u4Designator)
{
    return TRUE;
}

static void _AudAvInputMuxSel(UINT16 u2Source)
{
    switch(u2Source)                                                                                   
    {                                                                                                           
    case papi_src_srcid_SrcFront:
        AUD_AvInputMuxSel(AUD_INPUT_ID_COMP_VID_1);
        break;    
    case papi_src_srcid_SrcAv1:
        AUD_AvInputMuxSel(AUD_INPUT_ID_COMP_VID_0);
        break;
    case papi_src_srcid_SrcAv2:                                                                           
        AUD_AvInputMuxSel(AUD_INPUT_ID_S_VID_0);
        break;
    case papi_src_srcid_SrcAv3: 
        AUD_AvInputMuxSel(AUD_INPUT_ID_YPBPR_0);
        break;
    case papi_src_srcid_SrcAv4: 
        AUD_AvInputMuxSel(AUD_INPUT_ID_YPBPR_1);
        break;        
    case papi_src_srcid_SrcHdmi1:
        AUD_AvInputMuxSel(AUD_INPUT_ID_HDMI_0);
        break;                                                                                   
    case papi_src_srcid_SrcHdmi2:
        AUD_AvInputMuxSel(AUD_INPUT_ID_HDMI_1);
        break;                                                                                   
    case papi_src_srcid_SrcHdmi3:
        AUD_AvInputMuxSel(AUD_INPUT_ID_HDMI_2);
        break;                                                                                     
    case papi_src_srcid_SrcHdmi4:
        AUD_AvInputMuxSel(AUD_INPUT_ID_HDMI_3);
        break;
    case papi_src_srcid_SrcVga:
        AUD_AvInputMuxSel(AUD_INPUT_ID_VGA_0);
        break;                              
    default:
        // Not support
        printk("Not support AvInputMux\n");                                                                                                   
    }
}    

static INT32 _ioctl_SelectAudioSource(unsigned long u4Arg)
{
    IOCTL_3ARG_T rArg;
    INT32 i4Src, i4Node;
    UINT32 u4Designator;    
    PAPI_SRC_CB_T rSrcCb;
    
    COPY_FROM_USER(&rArg, (void*)u4Arg, sizeof(IOCTL_3ARG_T));

    i4Node = rArg.au4Arg[0];
    i4Src = rArg.au4Arg[1];
    u4Designator = rArg.au4Arg[2];
    
    _arAudioPairSet[0].i4Node = i4Node;
    _arAudioPairSet[0].i4Src = i4Src;
    _arAudioPairSet[0].u4Dsg = u4Designator;

    printk("_ioctl_SelectAudioSource: src %d, dsg: %d\n", 
    	_arAudioPairSet[0].i4Src, (int)_arAudioPairSet[0].u4Dsg);

#if 1
    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(IOCTL_3ARG_T));
    rSrcCb.i4Type = PAPI_CB_SRC_SRC_ONAUDIOSOURCESELECTED; // 2:OnAudioSourceSelected
    rSrcCb.i4Data1 = i4Node;
    rSrcCb.i4Data2 = i4Src;
    //papi_src_srcN_OnAudioSourceSelected(i4Node, i4Src);
    _CB_PutEvent(CB_PAPI_SRC_TRIGGER, sizeof (PAPI_SRC_CB_T),
        &rSrcCb);
#endif

    return 0; 
}

    
static INT32 _ioctl_GetCurrentAudioSelection(unsigned long u4Arg)
{
    IOCTL_3ARG_T rArg;

    rArg.au4Arg[0] = _arAudioPairActual[0].i4Node;
    rArg.au4Arg[1] = _arAudioPairActual[0].i4Src;
    rArg.au4Arg[2] = _arAudioPairActual[0].u4Dsg;

    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(IOCTL_3ARG_T));

    return 0;
}

static INT32 _ioctl_IsAudioSelectionAllowed(unsigned long u4Arg)
{
    IOCTL_2ARG_T rArg;

    COPY_FROM_USER(&rArg, (void*)u4Arg, sizeof(IOCTL_2ARG_T));

    if(!_fgIsAudioSelectionAllowed(rArg.au4Arg[0], rArg.au4Arg[1]))
    {
        return PAPI_SRC_SRC_FAIL;
    }

    return 0;
} 
static INT32 _ioctl_ApplyAudioSourceSettings(void)
{    
    INT32 i4Ret;
    UINT8 u1DecId;
    UINT16 u2Source;
    UINT32 u4Designator;
    PAPI_SRC_CB_T rSrcCb;

    if(memcmp(&_arAudioPairActual[0], &_arAudioPairSet[0], sizeof(PAPI_Audio_Src_Pair)) == 0 && 
    	(_arAudioPairSet[0].i4Src != papi_src_srcid_SrcNone) )
    {
        // no callback is need because video has callback...
        printk("_ioctl_ApplyAudioSourceSettings skip: identical audio source selection\n");
        return 0;
    }    
    
    _arAudioPairActual[0] = _arAudioPairSet[0];
    u2Source = (UINT16)_arAudioPairActual[0].i4Src;
    u4Designator = _arAudioPairActual[0].u4Dsg;

    printk("_ioctl_ApplyAudioSourceSettings: src: %d, des: %d\n", 
    	(int)u2Source, (int)u4Designator);

    if (u4Designator != papi_src_dsgid_DesignatorDigital){
        u1DecId = AUD_DEC_MAIN;
//        AUD_WaitDspFlush(AUD_DEC_AUX);  
#ifdef CC_MT5391_AUD_3_DECODER
#if AUD_DSP_CMD_ASYNC
        AUD_DSPCmdStopAsyn(AUD_DEC_MAIN);
        _af_wait();
#else
        AUD_DSPCmdStop(u1DecId);
        AUD_DSPCmdStop(AUD_DEC_AUX);
#endif
      
#else
        AUD_DSPCmdDualDecStop(u1DecId);
#endif
        
    }
    else
    {
        u1DecId = AUD_DEC_MAIN;
//        AUD_WaitDspFlush(AUD_DEC_AUX);  
#ifdef CC_MT5391_AUD_3_DECODER
#if AUD_DSP_CMD_ASYNC
        AUD_DSPCmdStopAsyn(AUD_DEC_MAIN);
        _af_wait();
#else
        AUD_DSPCmdStop(u1DecId);
        AUD_DSPCmdStop(AUD_DEC_AUX);
#endif        
#else
        AUD_DSPCmdDualDecStop(u1DecId);
#endif    
    }
    
    //unmute codec.
    ADAC_Mute(FALSE);
        
    // Set decoder type    
    switch (u2Source)
    {
    case papi_src_srcid_SrcTuner:
        if (u4Designator == papi_src_dsgid_DesignatorDigital)
        {
        
             _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_DIGITAL_TUNER;      
         #if 0 //done in dmx 
            _AudAvInputMuxSel(u2Source);
            AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_DIGITAL_TUNER, AUD_FMT_MPEG);
            AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_DIGITAL_TUNER, AUD_FMT_PCM);
            AUD_DSPCmdPlay(AUD_DEC_AUX);
            AUD_DSPCmdPlay(AUD_DEC_MAIN);
        #endif
        }
        else if (u4Designator == papi_src_dsgid_DesignatorAnalog)
        {       
#if 1       
            _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_ANALOG_TUNER;
#ifdef CC_MT5391_AUD_3_DECODER
            AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_PCM);
            AUD_SetDecType(AUD_DEC_AUX, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_MTS);
            AUD_DSPCmdPlay(AUD_DEC_AUX);
            AUD_DSPCmdPlay(AUD_DEC_MAIN);
#else
            AUD_TriggerDspPlay(AUD_DEC_AUX); /*to start audio*/                              
            AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_MTS);                     
            AUD_DSPCmdDualDecPlay(u1DecId);
#endif
#else
        _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_ANALOG_TUNER;      
        _AudAvInputMuxSel(papi_src_srcid_SrcTuner);
#ifdef CC_MT5391_AUD_3_DECODER        

#if AUD_DSP_CMD_ASYNC
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_MTS);
        _af_wait();
        AUD_DSPCmdPlayAsyn(AUD_DEC_MAIN);
        _af_wait();
#else
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_MTS);
        AUD_DSPCmdPlay(AUD_DEC_MAIN);
#endif

#else
	AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_ANALOG_TUNER, AUD_FMT_MTS);       
       AUD_DSPCmdDualDecPlay(u1DecId);
#endif        
        break;    
#endif
        }
        else
        {
            return PAPI_SRC_SRC_FAIL;
        }
        break;
    case papi_src_srcid_SrcAv1:
        _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_LINE_IN;      
        _AudAvInputMuxSel(u2Source);
#ifdef CC_MT5391_AUD_3_DECODER      
#if AUD_DSP_CMD_ASYNC
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
        _af_wait();
        AUD_DSPCmdPlayAsyn(AUD_DEC_MAIN);
        _af_wait();
#else
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
        AUD_DSPCmdPlay(AUD_DEC_MAIN);
#endif
#else
	AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);       
       AUD_DSPCmdDualDecPlay(u1DecId);
#endif        
        break;
    case papi_src_srcid_SrcInvalid:
    case papi_src_srcid_SrcNone: 
    case papi_src_srcid_SrcFront:
        _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_LINE_IN;      
        _AudAvInputMuxSel(u2Source);
#ifdef CC_MT5391_AUD_3_DECODER        
#if AUD_DSP_CMD_ASYNC
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
        _af_wait();
        AUD_DSPCmdPlayAsyn(AUD_DEC_MAIN);
        _af_wait();
#else
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
        AUD_DSPCmdPlay(AUD_DEC_MAIN);
#endif
#else
	AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);       
       AUD_DSPCmdDualDecPlay(u1DecId);
#endif        
        break;    	
    case papi_src_srcid_SrcAv2: 
    	  _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_LINE_IN;      
         _AudAvInputMuxSel(u2Source);
#ifdef CC_MT5391_AUD_3_DECODER
#if AUD_DSP_CMD_ASYNC
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
        _af_wait();
        AUD_DSPCmdPlayAsyn(AUD_DEC_MAIN);
        _af_wait();
#else
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
        AUD_DSPCmdPlay(AUD_DEC_MAIN);
#endif
#else
	AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);       
       AUD_DSPCmdDualDecPlay(u1DecId);
#endif

        break; 
    case papi_src_srcid_SrcAv3: 
    	  _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_LINE_IN;      
    	  _AudAvInputMuxSel(u2Source);
#ifdef CC_MT5391_AUD_3_DECODER    	  
#if AUD_DSP_CMD_ASYNC
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
        _af_wait();
        AUD_DSPCmdPlayAsyn(AUD_DEC_MAIN);
        _af_wait();
#else
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
        AUD_DSPCmdPlay(AUD_DEC_MAIN);
#endif
#else
	AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);       
       AUD_DSPCmdDualDecPlay(u1DecId);
#endif        
        break; 
    case papi_src_srcid_SrcAv4: 
    	  _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_LINE_IN;      
    	  _AudAvInputMuxSel(u2Source);
#ifdef CC_MT5391_AUD_3_DECODER    	  
#if AUD_DSP_CMD_ASYNC
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
        _af_wait();
        AUD_DSPCmdPlayAsyn(AUD_DEC_MAIN);
        _af_wait();
#else
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
        AUD_DSPCmdPlay(AUD_DEC_MAIN);
#endif
#else
	AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);       
       AUD_DSPCmdDualDecPlay(u1DecId);
#endif        
        break;     	
    case papi_src_srcid_SrcAv5: 
    case papi_src_srcid_SrcAv6: 
    case papi_src_srcid_SrcAv7: 
    case papi_src_srcid_SrcAv8: 
    case papi_src_srcid_SrcAv9: 
    case papi_src_srcid_SrcAv10: 
    	break;
    case papi_src_srcid_SrcHdmi1:
    	  _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_HDMI;      
         _AudAvInputMuxSel(u2Source);
#ifdef CC_MT5391_AUD_3_DECODER         
#if AUD_DSP_CMD_ASYNC
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_HDMI, AUD_FMT_PCM);
        _af_wait();
        AUD_DSPCmdPlayAsyn(AUD_DEC_MAIN);
        _af_wait();
#else
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_HDMI, AUD_FMT_PCM);
        AUD_DSPCmdPlay(AUD_DEC_MAIN);
#endif
#else
	AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_HDMI, AUD_FMT_PCM);       
       AUD_DSPCmdDualDecPlay(u1DecId);
#endif               
        break;
    case papi_src_srcid_SrcHdmi2:
         _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_HDMI;      
         _AudAvInputMuxSel(u2Source);
#ifdef CC_MT5391_AUD_3_DECODER         
#if AUD_DSP_CMD_ASYNC
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_HDMI, AUD_FMT_PCM);
        _af_wait();
        AUD_DSPCmdPlayAsyn(AUD_DEC_MAIN);
        _af_wait();
#else
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_HDMI, AUD_FMT_PCM);
        AUD_DSPCmdPlay(AUD_DEC_MAIN);
#endif
#else
	AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_HDMI, AUD_FMT_PCM);       
       AUD_DSPCmdDualDecPlay(u1DecId);
#endif               
        break;
    case papi_src_srcid_SrcHdmi3:
         _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_HDMI;      
         _AudAvInputMuxSel(u2Source);
#ifdef CC_MT5391_AUD_3_DECODER         
#if AUD_DSP_CMD_ASYNC
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_HDMI, AUD_FMT_PCM);
        _af_wait();
        AUD_DSPCmdPlayAsyn(AUD_DEC_MAIN);
        _af_wait();
#else
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_HDMI, AUD_FMT_PCM);
        AUD_DSPCmdPlay(AUD_DEC_MAIN);
#endif
#else
	AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_HDMI, AUD_FMT_PCM);       
       AUD_DSPCmdDualDecPlay(u1DecId);
#endif               
        break;
    case papi_src_srcid_SrcUrl: 
         _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_MULTI_MEDIA;      
         _AudAvInputMuxSel(u2Source);
        break;
    case papi_src_srcid_SrcHdmi4:
         _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_HDMI;      
         _AudAvInputMuxSel(u2Source);
#ifdef CC_MT5391_AUD_3_DECODER         
#if AUD_DSP_CMD_ASYNC
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_HDMI, AUD_FMT_PCM);
        _af_wait();
        AUD_DSPCmdPlayAsyn(AUD_DEC_MAIN);
        _af_wait();
#else
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_HDMI, AUD_FMT_PCM);
        AUD_DSPCmdPlay(AUD_DEC_MAIN);
#endif
#else
	AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_HDMI, AUD_FMT_PCM);       
       AUD_DSPCmdDualDecPlay(u1DecId);
#endif               
        break;    	
    case papi_src_srcid_SrcHdmi5:
    case papi_src_srcid_SrcBolton:
        return  PAPI_SRC_SRC_FAIL;
    case papi_src_srcid_SrcVga: 
    	  _aeStrSrc[u1DecId]  = AUD_STREAM_FROM_LINE_IN;      
    	  _AudAvInputMuxSel(u2Source);
#ifdef CC_MT5391_AUD_3_DECODER    	  
#if AUD_DSP_CMD_ASYNC
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
        _af_wait();
        AUD_DSPCmdPlayAsyn(AUD_DEC_MAIN);
        _af_wait();
#else
        AUD_SetDecType(AUD_DEC_MAIN, AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);
        AUD_DSPCmdPlay(AUD_DEC_MAIN);
#endif
#else
	AUD_DSPCmdDualDecSetDecType(AUD_STREAM_FROM_LINE_IN, AUD_FMT_PCM);       
       AUD_DSPCmdDualDecPlay(u1DecId);
#endif        
        break;     	        
    default:
        return  PAPI_SRC_SRC_FAIL;
    }
    
    printk("\nPAPISound: u2Source %u is connected\n", u2Source);   
    i4Ret = 0;

     rSrcCb.i4Type = 0;  //speaker
    _CB_PutEvent(CB_PAPI_FE_SND_TRIGGER, sizeof (PAPI_SRC_CB_T), &rSrcCb);
    rSrcCb.i4Type = 1; //headphone
    _CB_PutEvent(CB_PAPI_FE_SND_TRIGGER, sizeof (PAPI_SRC_CB_T), &rSrcCb);

    return i4Ret;
}

    
static INT32 _ioctl_SelectVideoSource(unsigned long u4Arg)
{
    IOCTL_2ARG_T rArg;
    INT32 i4Src;
    UINT32 u4Designator;
    UINT8 u1MainSrc, u1SubSrc;
    PAPI_SRC_CB_T rSrcCb;
    
    COPY_FROM_USER(&rArg, (void*)u4Arg, sizeof(IOCTL_2ARG_T));

    i4Src = rArg.au4Arg[0];
    u4Designator = rArg.au4Arg[1];
    
    if( (u1MainSrc = _au1SrcMap[(UINT32)(i4Src+1)]) == SV_VS_ATV1)
    {
        if(u4Designator == papi_src_dsgid_DesignatorDigital)
        {
            u1MainSrc = SV_VS_DTV1;  
        }
    }

    if(!_fgCheckVideoPair(u1MainSrc, u1SubSrc))
    {
        return PAPI_SRC_SRC_FAIL;
    }

    _arVideoPairSet[0].i4Src = i4Src;
    _arVideoPairSet[0].u4Dsg = u4Designator;

    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(IOCTL_2ARG_T));

    rSrcCb.i4Type = PAPI_CB_SRC_SRC_ONVIDEOSOURCESELECTED;
    rSrcCb.i4Data1 = i4Src;
    _CB_PutEvent(CB_PAPI_SRC_TRIGGER, sizeof (PAPI_SRC_CB_T),
        &rSrcCb);

    return 0; 
}

static INT32 _ioctl_GetCurrentVideoSelection(unsigned long u4Arg)
{
    IOCTL_2ARG_T rArg;

    rArg.au4Arg[0] = _arVideoPairActual[0].i4Src;
    rArg.au4Arg[1] = _arVideoPairActual[0].u4Dsg;

    COPY_TO_USER((void*)u4Arg, &rArg, sizeof(IOCTL_2ARG_T));

    return 0;
}

static INT32 _ioctl_IsVideoSelectionAllowed(unsigned long u4Arg)
{
    IOCTL_2ARG_T rArg;

    COPY_FROM_USER(&rArg, (void*)u4Arg, sizeof(IOCTL_2ARG_T));

    if(!_fgIsVideoSelectionAllowed(rArg.au4Arg[0], rArg.au4Arg[1]))
    {
        return PAPI_SRC_SRC_FAIL;
    }

    return 0;
} 

static INT32 _ioctl_ApplySourceSettings(unsigned long u4Arg)
{
    UINT32 u4MainSrcNew;    
    INT32 i4Ret = 0;
    UINT8 u1MainSrc, u1SubSrc;
    PAPI_SRC_CB_T rSrcCb;
	
    //MPV_Stop(0);
    AUD_DSPCmdStop(AUD_DEC_MAIN);
    AUD_DSPCmdStop(AUD_DEC_AUX);
 
    if(memcmp(&_arVideoPairActual[0], &_arVideoPairSet[0], sizeof(PAPI_Video_Src_Pair)) == 0 && 
    	(_arVideoPairSet[0].i4Src != papi_src_srcid_SrcNone) )
    {
        rSrcCb.i4Type = PAPI_CB_SRC_SRC_ONSOURCESETTINGAPPLIED;
        _CB_PutEvent(CB_PAPI_SRC_TRIGGER, sizeof (PAPI_SRC_CB_T),
        &rSrcCb);
        
        return 0;
    }    
    else if( (u1MainSrc = _au1SrcMap[(UINT32)(_arVideoPairSet[0].i4Src + 1)]) == SV_VS_ATV1)
    {
        if(_arVideoPairSet[0].u4Dsg == papi_src_dsgid_DesignatorDigital)
        {
            u1MainSrc = SV_VS_DTV1;  
        }
        
        if(bApiVideoSetVideoSrc(SV_VP_MAIN, u1MainSrc) == SV_FAIL)
        return PAPI_SRC_SRC_FAIL;
    }
    
    if(bApiVideoSetVideoSrc(SV_VP_MAIN, u1MainSrc) == SV_FAIL)
    {
        return PAPI_SRC_SRC_FAIL;
    }    
    
    VDP_SetMode(0, VDP_MODE_NORMAL);        
    VDP_SetEnable(0, TRUE);
    if(u1MainSrc == SV_VS_DTV1)
    {
        VDP_SetInput(0, 0, 0);
        //MPV_Play(0); Should not use this here, you need to know mpeg2/h.264
    }
    
    _arVideoPairActual[0] = _arVideoPairSet[0];

    //Apply audio source setting ~
    _ioctl_ApplyAudioSourceSettings();

    rSrcCb.i4Type = PAPI_CB_SRC_SRC_ONSOURCESETTINGAPPLIED;
    _CB_PutEvent(CB_PAPI_SRC_TRIGGER, sizeof (PAPI_SRC_CB_T),
        &rSrcCb);

#if 1
    if(_TSDMX_IsRequestAllowed(u4MainSrcNew) == TM_OK)
    {
        TSDMX_CB_T rTsdmxCb;
    
        rTsdmxCb.u4Type = TSDMX_CB_REQUESTALLOWED;
    
        UNUSED(_CB_PutEvent(CB_TSDMX_TRIGGER, sizeof (TSDMX_CB_T),
            &rTsdmxCb));        
    }
    else
    {
        TSDMX_CB_T rTsdmxCb;
        
        rTsdmxCb.u4Type = TSDMX_CB_REQUESTNOTALLOWED;
        
        UNUSED(_CB_PutEvent(CB_TSDMX_TRIGGER, sizeof (TSDMX_CB_T),
        &rTsdmxCb));           
    }
#else
    if(_arVideoPairSet[0].u4Dsg == papi_src_dsgid_DesignatorDigital)
    {
        TSDMX_CB_T rTsdmxCb;
    
        rTsdmxCb.u4Type = TSDMX_CB_REQUESTALLOWED;
        rTsdmxCb.u2Pid = 1;
        printk("[IO_PAPI] papi_fe_tsdmxN_OnRequestAllowed()\n");
    
        UNUSED(_CB_PutEvent(CB_TSDMX_TRIGGER, sizeof (TSDMX_CB_T),
            &rTsdmxCb));
    }     
    else
    {
        
        TSDMX_CB_T rTsdmxCb;
        
        rTsdmxCb.u4Type = TSDMX_CB_REQUESTNOTALLOWED;
        rTsdmxCb.u2Pid = 1;
        printk("[IO_PAPI] papi_fe_tsdmxN_OnRequestNotAllowed()\n");
        
        UNUSED(_CB_PutEvent(CB_TSDMX_TRIGGER, sizeof (TSDMX_CB_T),
        &rTsdmxCb)); 
        
    }
#endif

    return i4Ret;
}

//-----------------------------------------------------------------------------
// Inter-module functions
//-----------------------------------------------------------------------------
void _papi_src_src_GetCurrentVideoSelection( int * source, Nat32 * designator )
{

    *source = _arVideoPairActual[0].i4Src;
    *designator = _arVideoPairActual[0].u4Dsg;

    return;
}
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_src_src(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    _af_lock();

    switch (cmd)
    {
    case PAPI_IO_SRC_SRC_SELECTVIDEOSOURCE:
        i4Ret = _ioctl_SelectVideoSource(arg);
        break;
    case PAPI_IO_SRC_SRC_APPLYSOURCESETTINGS:
        i4Ret = _ioctl_ApplySourceSettings(arg);
        break;
    case PAPI_IO_SRC_SRC_ISVIDEOSELECTIONALLOWED:
        i4Ret = _ioctl_IsVideoSelectionAllowed(arg);
        break;
    case PAPI_IO_SRC_SRC_GETCURRENTVIDEOSELECTION:
    	 i4Ret = _ioctl_GetCurrentVideoSelection(arg);
    	 break;    	
    case PAPI_IO_SRC_SRC_GETCURRENTAUDIOSELECTION:
    	 i4Ret = _ioctl_GetCurrentAudioSelection(arg);
    	 break;    	 
    case PAPI_IO_SRC_SRC_SELECTAUDIOSOURCE:
  	    i4Ret = _ioctl_SelectAudioSource(arg);
    	 break;  
    case PAPI_IO_SRC_SRC_ISAUDIOSELECTIONALLOWED:
    	 i4Ret = _ioctl_IsAudioSelectionAllowed(arg);
    	 break;
    default:
        printk("papi_ioctl_src_src: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

    _af_unlock();

    return i4Ret;
}


