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
 * $RCSfile: io_papi_vf_vio.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_vf_vio.c
 *  PAPI kernel driver implementation: video output
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <linux/papi_ioctl.h>
#include <linux/mt53xx_cb.h>
#include "papi_vf.h"

#include "sv_const.h"
#include "nptv_if.h"
#include "vdp_if.h"
//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------
/// Common error code
#define TM_FAIL                     -0x800


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// External functions
//-----------------------------------------------------------------------------

EXTERN INT32 _CB_PutEvent(CB_FCT_ID_T eFctId, INT32 i4TagSize, void *pvTag);


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#define VERIFY_NULL(ptr)                                    \
          do {                                              \
            if (ptr == NULL) { return; }      \
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

static void _papi_vf_vio_GetBrightness( int * retval )
{
    INT16 u2Arg;
    VERIFY_NULL(retval);
    VDP_GetArg(VDP_1, VDP_ARG_BRIGHTNESS, &u2Arg);	
    COPY_TO_USER(retval, &u2Arg, sizeof(INT16));
}

static void _papi_vf_vio_SetBrightness( int brightness )
{
    PAPI_VF_CB_T rPapiVf;
    VDP_SetArg(VDP_1, VDP_ARG_BRIGHTNESS, brightness);
    
    // callback to user space	
    rPapiVf.i4Type = PAPI_CB_VF_VIO_ONBRIGHTNESSCHANGED;		          
    rPapiVf.i4Data1 = brightness;     
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rPapiVf);	
}

static void _papi_vf_vio_GetColor( int * retval )
{
    INT16 u2Arg;
    VERIFY_NULL(retval);
    VDP_GetArg(VDP_1, VDP_ARG_SATURATION, &u2Arg);		
    COPY_TO_USER(retval, &u2Arg, sizeof(INT16));    
}

static void _papi_vf_vio_SetColor( int color )
{
    PAPI_VF_CB_T rPapiVf;
    VDP_SetArg(VDP_1, VDP_ARG_SATURATION, color);
    
    // callback to user space	
    rPapiVf.i4Type = PAPI_CB_VF_VIO_ONCOLORCHANGED;		          
    rPapiVf.i4Data1 = color;       
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rPapiVf);	
}

static void _papi_vf_vio_GetContrast( int * retval )
{
    INT16 u2Arg;
    VERIFY_NULL(retval);
    VDP_GetArg(VDP_1, VDP_ARG_CONTRAST, &u2Arg);			
    COPY_TO_USER(retval, &u2Arg, sizeof(INT16));    
}

static void _papi_vf_vio_SetContrast( int contrast )
{
    PAPI_VF_CB_T rPapiVf;
    VDP_SetArg(VDP_1, VDP_ARG_CONTRAST, contrast);
    
    // callback to user space	
    rPapiVf.i4Type = PAPI_CB_VF_VIO_ONCONTRASTCHANGED;		          
    rPapiVf.i4Data1 = contrast;       
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rPapiVf);		
}

static void _papi_vf_vio_GetFieldFrequency( int * retval )
{
}

static void _papi_vf_vio_HueAllowed( Bool * retval )
{
	// TODO: VGA detection
	Bool fgEnable = True;
    COPY_TO_USER(retval, &fgEnable, sizeof(Bool)); 
}

static void _papi_vf_vio_GetHue( int * retval )
{
    INT16 u2Arg;
    VERIFY_NULL(retval);
    VDP_GetArg(VDP_1, VDP_ARG_HUE, &u2Arg);		
    COPY_TO_USER(retval, &u2Arg, sizeof(INT16));    
}

static void _papi_vf_vio_SetHue( int hue )
{
    PAPI_VF_CB_T rPapiVf;
    VDP_SetArg(VDP_1, VDP_ARG_HUE, hue);
    
    // callback to user space	
    rPapiVf.i4Type = PAPI_CB_VF_VIO_ONHUECHANGED;		          
    rPapiVf.i4Data1 = hue;       
    _CB_PutEvent(CB_PAPI_VF_TRIGGER, sizeof (PAPI_VF_CB_T), &rPapiVf);			
}


static void _papi_vf_vio_HueSupported( Bool * retval )
{
	// TODO: VGA detection
	Bool fgEnable = True;
    COPY_TO_USER(retval, &fgEnable, sizeof(Bool)); 
}

			
//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_vf_vio(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
   int i4Ret = 0;
   IOCTL_2ARG_T rArg2;	
   
    //	  _ApiLock();
    if ((_IOC_SIZE(cmd)) > sizeof(unsigned long))
    {
        COPY_FROM_USER(arg, &rArg2, _IOC_SIZE(cmd));
    }    

    switch (cmd)
    {
    case PAPI_IO_VF_VIO_SETBRIGHTNESS:
        _papi_vf_vio_SetBrightness((int)arg);
	    break;        
    case PAPI_IO_VF_VIO_GETBRIGHTNESS:
        _papi_vf_vio_GetBrightness((int*)arg);
	    break;         
    case PAPI_IO_VF_VIO_SETCONTRAST:
        _papi_vf_vio_SetContrast((int)arg);
	    break;        
    case PAPI_IO_VF_VIO_GETCONTRAST:
        _papi_vf_vio_GetContrast((int*)arg);
	    break;
    case PAPI_IO_VF_VIO_SETCOLOR:
        _papi_vf_vio_SetColor((int)arg);
	    break;        
    case PAPI_IO_VF_VIO_GETCOLOR:
        _papi_vf_vio_GetColor((int*)arg);
	    break;
    case PAPI_IO_VF_VIO_SETHUE:
        _papi_vf_vio_SetHue((int)arg);
	    break;        
    case PAPI_IO_VF_VIO_GETHUE:
        _papi_vf_vio_GetHue((int*)arg);
	    break;	    
    case PAPI_IO_VF_VIO_HUEALLOWED:
        _papi_vf_vio_HueAllowed((Bool*)arg);
	    break;        
    case PAPI_IO_VF_VIO_HUESUPPORTED:
        _papi_vf_vio_HueSupported((Bool*)arg);
	    break;    
	    
	default:
        printk("papi_ioctl_vf_vio: unknown IO command %u\n", cmd);
		i4Ret = -EFAULT;
    }

//    _ApiUnlock();

    return i4Ret;
}



