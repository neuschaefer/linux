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
 * $RCSfile: io_papi_vf_vim.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_vf_vim.c
 *  PAPI kernel driver implementation: video improvement 
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <linux/papi_ioctl.h>
#include <linux/mt53xx_cb.h>
#include "papi_vf.h"
#include "papi_types.h"

#include "sv_const.h"
#include "nptv_if.h"
#include "vdp_if.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
#define TM_FAIL                     -0x800

//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define VERIFY_NULL(ptr) \
	do { \
		if (ptr == NULL) { return; } \
	} while (0)

		
/// Copy data from user space to kernel space
#define COPY_FROM_USER(dst, src, size) \
	if (!access_ok(VERIFY_READ, (void __user *)(src), (size))) \
	{ \
		return TM_FAIL; \
	} \
	if (copy_from_user((dst), (void __user *)(src), (size))) \
	{ \
		return TM_FAIL; \
	}
			
/// Copy data from kernel space to user space
#define COPY_TO_USER(dst, src, size) \
	if (!access_ok(VERIFY_WRITE, (void __user *)(dst), (size))) \
	{ \
		return TM_FAIL; \
	} \
	if (copy_to_user((void __user *)(dst), (src), (size))) \
	{ \
		return TM_FAIL; \
	}

//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------

static void _papi_vf_vim_GetColorEnhancement( int * retval )
{
    INT16 u2Arg;
    VERIFY_NULL(retval);
    VDP_GetArg(VDP_1, VDP_ARG_CTI, &u2Arg);
    COPY_TO_USER(retval, &u2Arg, sizeof(INT16));    
}

static void _papi_vf_vim_SetColorEnhancement( int mode )
{
    PAPI_VF_CB_T rPapiVf;
    VDP_SetArg(VDP_1, VDP_ARG_CTI, mode);
    
    // callback to user space	
    rPapiVf.i4Type = PAPI_CB_VF_VIM_ONCOLORENHANCEMENTCHANGED;
    rPapiVf.i4Data1 = mode;
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rPapiVf);		
}


static void _papi_vf_vim_ColorEnhancementSupported( int mode, Bool * retval )
{
	// TODO: VGA detection
	*retval = True;
}

static void _papi_vf_vim_GetDigitalNaturalMotion( int * retval )
{
    INT16 u2Arg;
    VERIFY_NULL(retval);
    VDP_GetArg(VDP_1, VDP_ARG_MJC_EFFECT, &u2Arg);
    COPY_TO_USER(retval, &u2Arg, sizeof(INT16));    
}

static void _papi_vf_vim_SetDigitalNaturalMotion( int mode )
{
    PAPI_VF_CB_T rPapiVf;
    VDP_SetArg(VDP_1, VDP_ARG_MJC_EFFECT, mode);
    
    // callback to user space	
    rPapiVf.i4Type = PAPI_CB_VF_VIM_ONDIGITALNATURALMOTIONCHANGED;
    rPapiVf.i4Data1 = mode;    
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rPapiVf);		
}


static void _papi_vf_vim_GetDnr( int * retval )
{
    INT16 u2Arg;
    VERIFY_NULL(retval);
    VDP_GetArg(VDP_1, VDP_ARG_NR, &u2Arg);		
    COPY_TO_USER(retval, &u2Arg, sizeof(INT16));    
}

static void _papi_vf_vim_SetDnr( int mode )
{
    PAPI_VF_CB_T rPapiVf;
    VDP_SetArg(VDP_1, VDP_ARG_NR, mode);
    VDP_SetArg(VDP_1, VDP_ARG_3D_NR, mode);    
    
    // callback to user space	
    rPapiVf.i4Type = PAPI_CB_VF_VIM_ONDNRCHANGED;
    rPapiVf.i4Data1 = mode;      
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rPapiVf);		
}

static void _papi_vf_vim_DnrSupported( int mode, Bool * retval )
{
	// TODO: 
	Bool fgEnable = True;
    COPY_TO_USER(retval, &fgEnable, sizeof(Bool));  	
}

static void _papi_vf_vim_GetDynamicContrast( int * retval )
{
    INT16 u2Arg;
    VERIFY_NULL(retval);
    VDP_GetArg(VDP_1, VDP_ARG_CTI, &u2Arg);		
    COPY_TO_USER(retval, &u2Arg, sizeof(INT16));    
}

static void _papi_vf_vim_SetDynamicContrast( int mode )
{
    PAPI_VF_CB_T rPapiVf;
    VDP_SetArg(VDP_1, VDP_ARG_CTI, mode);
    
    // callback to user space	
    rPapiVf.i4Type = PAPI_CB_VF_VIM_ONDYNAMICCONTRASTCHANGED;		          
    rPapiVf.i4Data1 = mode;    
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rPapiVf);		
}

static void _papi_vf_vim_DynamicContrastSupported( int mode, Bool * retval )
{
	// TODO: VGA detection
	Bool fgEnable = True;
    COPY_TO_USER(retval, &fgEnable, sizeof(Bool)); 
}

static void _papi_vf_vim_IsDigitalNaturalMotionAllowed( int mode, Bool * retval )
{
	// TODO: VGA detection
	Bool fgEnable = True;
    COPY_TO_USER(retval, &fgEnable, sizeof(Bool)); 
}

static void _papi_vf_vim_IsDigitalNaturalMotionSupported( int mode, Bool * retval )
{
	// TODO: VGA detection
	Bool fgEnable = True;
    COPY_TO_USER(retval, &fgEnable, sizeof(Bool)); 
}

static void _papi_vf_vim_GetLti( Bool * retval )
{
    INT16 u2Arg;
    VERIFY_NULL(retval);
    VDP_GetArg(VDP_1, VDP_ARG_LTI, &u2Arg);		
    COPY_TO_USER(retval, &u2Arg, sizeof(INT16));    
}

static void _papi_vf_vim_SetLti( Bool onoff )
{
    PAPI_VF_CB_T rPapiVf;
    VDP_SetArg(VDP_1, VDP_ARG_LTI, onoff);
    
    // callback to user space	
    rPapiVf.i4Type = PAPI_CB_VF_VIM_ONLTICHANGED;		          
    rPapiVf.i4Data1 = onoff;     
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rPapiVf);		
}

static void _papi_vf_vim_LtiSupported( Bool * retval )
{
	// TODO: VGA detection
	Bool fgEnable = True;
    COPY_TO_USER(retval, &fgEnable, sizeof(Bool)); 	
}

static void _papi_vf_vim_GetMpegNoiseReduction( Bool * retval )
{
    INT16 u2Arg;
    VERIFY_NULL(retval);
    VDP_GetArg(VDP_1, VDP_ARG_MPEG_NR, &u2Arg);		
    COPY_TO_USER(retval, &u2Arg, sizeof(INT16));    
}

static void _papi_vf_vim_SetMpegNoiseReduction( Bool onoff )
{
    PAPI_VF_CB_T rPapiVf;
    VDP_SetArg(VDP_1, VDP_ARG_MPEG_NR, onoff);
    
    // callback to user space	
    rPapiVf.i4Type = PAPI_CB_VF_VIM_ONMPEGNOISEREDUCTIONCHANGED;
    rPapiVf.i4Data1 = onoff;     
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rPapiVf);		
}

static void _papi_vf_vim_MpegNoiseReductionSupported( Bool * retval )
{
	// TODO: VGA detection
	Bool fgEnable = True;
    COPY_TO_USER(retval, &fgEnable, sizeof(Bool)); 		
}

static void _papi_vf_vim_GetSharpness( int * retval )
{
    INT16 u2Arg;
    VERIFY_NULL(retval);
    VDP_GetArg(VDP_1, VDP_ARG_SHARPNESS, &u2Arg);	
    COPY_TO_USER(retval, &u2Arg, sizeof(INT16));    
}

static void _papi_vf_vim_SetSharpness( int mode )
{
    PAPI_VF_CB_T rPapiVf;
    VDP_SetArg(VDP_1, VDP_ARG_SHARPNESS, mode);
    
    // callback to user space	
    rPapiVf.i4Type = PAPI_CB_VF_VIM_ONSHARPNESSCHANGED;		
    rPapiVf.i4Data1 = mode;     
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rPapiVf);	
}

static void _papi_vf_vim_SharpnessSupported( int mode, Bool * retval )
{
	// TODO: VGA detection
	Bool fgEnable = True;
    COPY_TO_USER(retval, &fgEnable, sizeof(Bool)); 	
}

//-----------------------------------------------------------------------------
// public functions
//-----------------------------------------------------------------------------


int papi_ioctl_vf_vim(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
	int i4Ret = 0;
    IOCTL_2ARG_T rArg2;  
    
     //    _ApiLock();
     if ((_IOC_SIZE(cmd)) > sizeof(unsigned long))
     {
         COPY_FROM_USER(arg, &rArg2, _IOC_SIZE(cmd));
     }    

    switch (cmd)
    {
    case PAPI_IO_VF_VIM_SETDNR:
        _papi_vf_vim_SetDnr((int)arg);
        break;
    case PAPI_IO_VF_VIM_GETDNR:
        _papi_vf_vim_GetDnr((int*)arg);    
	    break;    
    case PAPI_IO_VF_VIM_DNRSUPPORTED:
        _papi_vf_vim_DnrSupported((int)rArg2.au4Arg[0], (Bool*)rArg2.au4Arg[1]);
        break;
    case PAPI_IO_VF_VIM_SETLTI:
        _papi_vf_vim_SetLti((Bool)arg);
        break;    
    case PAPI_IO_VF_VIM_GETLTI:
        _papi_vf_vim_GetLti((Bool*)arg);    
	    break;    
    case PAPI_IO_VF_VIM_LTISUPPORTED:
        _papi_vf_vim_LtiSupported((Bool*)arg);    
        break;    
    case PAPI_IO_VF_VIM_SHARPNESSSUPPORTED:
        _papi_vf_vim_SharpnessSupported((int)rArg2.au4Arg[0], (Bool*)rArg2.au4Arg[1]);    
        break;        
    case PAPI_IO_VF_VIM_SETSHARPNESS:
    	_papi_vf_vim_SetSharpness((int)arg);
    	break;    	
    case PAPI_IO_VF_VIM_GETSHARPNESS:
	    _papi_vf_vim_GetSharpness((int*)arg);
	    break;	        	      
    case PAPI_IO_VF_VIM_SETCOLORENHANCEMENT:
	    _papi_vf_vim_SetColorEnhancement((int)arg);
	    break;	      
    case PAPI_IO_VF_VIM_GETCOLORENHANCEMENT:
	    _papi_vf_vim_GetColorEnhancement((int*)arg);
	    break;    
    case PAPI_IO_VF_VIM_COLORENHANCEMENTSUPPORTED:
	    _papi_vf_vim_ColorEnhancementSupported((int)rArg2.au4Arg[0], (Bool*)rArg2.au4Arg[1]);
	    break;           
    case PAPI_IO_VF_VIM_SETDYNAMICCONTRAST:
	    _papi_vf_vim_SetDynamicContrast((int)arg);
	    break;	          
    case PAPI_IO_VF_VIM_GETDYNAMICCONTRAST:
	    _papi_vf_vim_GetDynamicContrast((int*)arg);
	    break;     
    case PAPI_IO_VF_VIM_DYNAMICCONTRASTSUPPORTED:
	    _papi_vf_vim_DynamicContrastSupported((int)rArg2.au4Arg[0], (Bool*)rArg2.au4Arg[1]);
	    break;     
    case PAPI_IO_VF_VIM_SETMPEGNOISEREDUCTION:
	    _papi_vf_vim_SetMpegNoiseReduction((Bool)arg);
	    break;	          
    case PAPI_IO_VF_VIM_GETMPEGNOISEREDUCTION:
	    _papi_vf_vim_GetMpegNoiseReduction((Bool*)arg);
	    break;      
    case PAPI_IO_VF_VIM_MPEGNOISEREDUCTIONSUPPORTED:
	    _papi_vf_vim_MpegNoiseReductionSupported((Bool*)arg);
	    break;          
    case PAPI_IO_VF_VIM_SETDIGITALNATURALMOTION:
	    _papi_vf_vim_SetDigitalNaturalMotion((int)arg);
	    break;	          
    case PAPI_IO_VF_VIM_GETDIGITALNATURALMOTION:
	    _papi_vf_vim_GetDigitalNaturalMotion((int*)arg);
	    break;      
    case PAPI_IO_VF_VIM_ISDIGITALNATURALMOTIONSUPPORTED:
	    _papi_vf_vim_IsDigitalNaturalMotionSupported((int)rArg2.au4Arg[0], (Bool*)rArg2.au4Arg[1]);
	    break;         
    case PAPI_IO_VF_VIM_ISDIGITALNATURALMOTIONALLOWED:
	    _papi_vf_vim_IsDigitalNaturalMotionAllowed((int)rArg2.au4Arg[0], (Bool*)rArg2.au4Arg[1]);
	    break;         
    
    default:
        printk("_papi_ioctl_vf_vim: unknown IO command %u\n", cmd);
        i4Ret = -EFAULT;
    }

//    _ApiUnlock();

    return i4Ret;
}



