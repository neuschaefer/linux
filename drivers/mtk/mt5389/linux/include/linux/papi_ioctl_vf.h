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
 * $RCSfile: papi_ioctl_vf.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file papi_ioctl_vf.h
 *  IO control of PAPI Video interface
 */

#ifndef PAPI_IOCTL_VF_H
#define PAPI_IOCTL_VF_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include "papi_ioctl_base.h"
#include <linux/ioctl.h>


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

// Video Improvement  (vim)
//#define PAPI_IO_VF_VIM_GETOFFDNR					    _IOW(PAPI_IOCTYPE_VF,  0, int)
//#define PAPI_IO_VF_VIM_GETMINDNR					    _IOW(PAPI_IOCTYPE_VF,  1, int)
//#define PAPI_IO_VF_VIM_GETMEDDNR					    _IOW(PAPI_IOCTYPE_VF,  2, int)
//#define PAPI_IO_VF_VIM_GETMAXDNR					    _IOW(PAPI_IOCTYPE_VF,  3, int)
#define PAPI_IO_VF_VIM_SETDNR						    _IOW(PAPI_IOCTYPE_VF,  4, int)
#define PAPI_IO_VF_VIM_GETDNR						    _IOW(PAPI_IOCTYPE_VF,  5, int)
#define PAPI_IO_VF_VIM_DNRSUPPORTED						_IOW(PAPI_IOCTYPE_VF,  6, int)
#define PAPI_IO_VF_VIM_SETLTI						    _IOW(PAPI_IOCTYPE_VF,  7, int)
#define PAPI_IO_VF_VIM_GETLTI						    _IOW(PAPI_IOCTYPE_VF,  8, int)
#define PAPI_IO_VF_VIM_LTISUPPORTED						_IOW(PAPI_IOCTYPE_VF,  9, int)
//#define PAPI_IO_VF_VIM_GETMINSHARPNESS					_IOW(PAPI_IOCTYPE_VF, 10, int)
//#define PAPI_IO_VF_VIM_GETMEDSHARPNESS					_IOW(PAPI_IOCTYPE_VF, 11, int)
//#define PAPI_IO_VF_VIM_GETMAXSHARPNESS					_IOW(PAPI_IOCTYPE_VF, 12, int)
#define PAPI_IO_VF_VIM_SETSHARPNESS						_IOW(PAPI_IOCTYPE_VF, 13, int)
#define PAPI_IO_VF_VIM_GETSHARPNESS						_IOW(PAPI_IOCTYPE_VF, 14, int)
#define PAPI_IO_VF_VIM_SHARPNESSSUPPORTED				_IOW(PAPI_IOCTYPE_VF, 15, int)
#define PAPI_IO_VF_VIM_SETCOLORENHANCEMENT				_IOW(PAPI_IOCTYPE_VF, 16, int)
#define PAPI_IO_VF_VIM_GETCOLORENHANCEMENT				_IOW(PAPI_IOCTYPE_VF, 17, int)
#define PAPI_IO_VF_VIM_COLORENHANCEMENTSUPPORTED		_IOW(PAPI_IOCTYPE_VF, 18, int)
//#define PAPI_IO_VF_VIM_GETOFFDYNAMICCONTRAST			_IOW(PAPI_IOCTYPE_VF, 19, int)
//#define PAPI_IO_VF_VIM_GETMINDYNAMICCONTRAST			_IOW(PAPI_IOCTYPE_VF, 20, int)
//#define PAPI_IO_VF_VIM_GETMEDDYNAMICCONTRAST			_IOW(PAPI_IOCTYPE_VF, 21, int)
//#define PAPI_IO_VF_VIM_GETMAXDYNAMICCONTRAST			_IOW(PAPI_IOCTYPE_VF, 22, int)
#define PAPI_IO_VF_VIM_SETDYNAMICCONTRAST				_IOW(PAPI_IOCTYPE_VF, 23, int)
#define PAPI_IO_VF_VIM_GETDYNAMICCONTRAST				_IOW(PAPI_IOCTYPE_VF, 24, int)
#define PAPI_IO_VF_VIM_DYNAMICCONTRASTSUPPORTED			_IOW(PAPI_IOCTYPE_VF, 25, int)
#define PAPI_IO_VF_VIM_SETMPEGNOISEREDUCTION			_IOW(PAPI_IOCTYPE_VF, 26, int)
#define PAPI_IO_VF_VIM_GETMPEGNOISEREDUCTION			_IOW(PAPI_IOCTYPE_VF, 27, int)
#define PAPI_IO_VF_VIM_MPEGNOISEREDUCTIONSUPPORTED		_IOW(PAPI_IOCTYPE_VF, 28, int)
//#define PAPI_IO_VF_VIM_GETOFFDIGITALNATURALMOTION   	_IOW(PAPI_IOCTYPE_VF, 29, int)
//#define PAPI_IO_VF_VIM_GETMINDIGITALNATURALMOTION   	_IOW(PAPI_IOCTYPE_VF, 30, int)
//#define PAPI_IO_VF_VIM_GETMAXDIGITALNATURALMOTION   	_IOW(PAPI_IOCTYPE_VF, 31, int)
#define PAPI_IO_VF_VIM_SETDIGITALNATURALMOTION      	_IOW(PAPI_IOCTYPE_VF, 32, int)
#define PAPI_IO_VF_VIM_GETDIGITALNATURALMOTION			_IOW(PAPI_IOCTYPE_VF, 33, int)
#define PAPI_IO_VF_VIM_ISDIGITALNATURALMOTIONSUPPORTED	_IOW(PAPI_IOCTYPE_VF, 34, int)
#define PAPI_IO_VF_VIM_ISDIGITALNATURALMOTIONALLOWED	_IOW(PAPI_IOCTYPE_VF, 35, int)
// Video Demo Mode (vdm)
//#define PAPI_IO_VF_VDM_SETDEMOMODE                  	_IOW(PAPI_IOCTYPE_VF, 50, int)
//#define PAPI_IO_VF_VDM_GETDEMOMODE                  	_IOW(PAPI_IOCTYPE_VF, 51, int)
//#define PAPI_IO_VF_VDM_DEMOMODESUPPORTED            	_IOW(PAPI_IOCTYPE_VF, 52, int)
//#define PAPI_IO_VF_VDM_DEMOMODEALLOWED              	_IOW(PAPI_IOCTYPE_VF, 53, int)

// Video Output
#define PAPI_IO_VF_VIO_SETBRIGHTNESS					_IOW(PAPI_IOCTYPE_VF, 60, int)
#define PAPI_IO_VF_VIO_GETBRIGHTNESS					_IOW(PAPI_IOCTYPE_VF, 61, int) 
//#define PAPI_IO_VF_VIO_GETMINBRIGHTNESS					_IOW(PAPI_IOCTYPE_VF, 62, int)
//#define PAPI_IO_VF_VIO_GETMAXBRIGHTNESS					_IOW(PAPI_IOCTYPE_VF, 63, int) 
#define PAPI_IO_VF_VIO_SETCONTRAST						_IOW(PAPI_IOCTYPE_VF, 64, int)
#define PAPI_IO_VF_VIO_GETCONTRAST						_IOW(PAPI_IOCTYPE_VF, 65, int) 
//#define PAPI_IO_VF_VIO_GETMINCONTRAST					_IOW(PAPI_IOCTYPE_VF, 66, int)
//#define PAPI_IO_VF_VIO_GETMAXCONTRAST					_IOW(PAPI_IOCTYPE_VF, 67, int) 
#define PAPI_IO_VF_VIO_SETCOLOR					    	_IOW(PAPI_IOCTYPE_VF, 68, int)
#define PAPI_IO_VF_VIO_GETCOLOR							_IOW(PAPI_IOCTYPE_VF, 69, int)
#define PAPI_IO_VF_VIO_SETHUE							_IOW(PAPI_IOCTYPE_VF, 70, int)
#define PAPI_IO_VF_VIO_GETHUE							_IOW(PAPI_IOCTYPE_VF, 71, int)
#define PAPI_IO_VF_VIO_HUEALLOWED						_IOW(PAPI_IOCTYPE_VF, 72, int)
#define PAPI_IO_VF_VIO_HUESUPPORTED						_IOW(PAPI_IOCTYPE_VF, 73, int)
//#define PAPI_IO_VF_VIO_GETTINT							_IOW(PAPI_IOCTYPE_VF, 74, int)
//#define PAPI_IO_VF_VIO_SETTINT							_IOW(PAPI_IOCTYPE_VF, 75, int)
//#define PAPI_IO_VF_VIO_TINTSUPPORTED					_IOW(PAPI_IOCTYPE_VF, 76, int)

// Video Mute (vmt)
#define PAPI_IO_VF_VMT_ISMUTECOLORSUPPORTED	_IOW(PAPI_IOCTYPE_VF, 90, IOCTL_4ARG_T)
#define PAPI_IO_VF_VMT_ISMUTESUPPORTED	_IOW(PAPI_IOCTYPE_VF, 91, int) 
#define PAPI_IO_VF_VMT_ISAUTOMUTESUPPORTED 	_IOW(PAPI_IOCTYPE_VF, 92, int)
#define PAPI_IO_VF_VMT_GETACTUALMUTECOLOR	_IOW(PAPI_IOCTYPE_VF, 93, IOCTL_3ARG_T) 
#define PAPI_IO_VF_VMT_SETAUTOMUTE	_IOW(PAPI_IOCTYPE_VF, 94, int) 
#define PAPI_IO_VF_VMT_GETMUTE	_IOW(PAPI_IOCTYPE_VF, 95, int) 
#define PAPI_IO_VF_VMT_GETMUTECOLOR	_IOW(PAPI_IOCTYPE_VF, 96, IOCTL_3ARG_T) 
#define PAPI_IO_VF_VMT_GETACTUALMUTE	_IOW(PAPI_IOCTYPE_VF, 97, int) 
#define PAPI_IO_VF_VMT_SETMUTE	_IOW(PAPI_IOCTYPE_VF, 98, IOCTL_2ARG_T) 
#define PAPI_IO_VF_VMT_SETMUTECOLOR	_IOW(PAPI_IOCTYPE_VF, 99, IOCTL_3ARG_T) 
#define PAPI_IO_VF_VMT_GETAUTOMUTE	_IOW(PAPI_IOCTYPE_VF, 100, int) 

// vclipN of type IPlfApiVideoClipNotify

// vfrN of type IPlfApiVideoFreezeNotify

// vvmtN of type IPlfApiVideoMuteNotify
#define PAPI_CB_VF_VMT_ONACTUALMUTECHANGED 90
#define PAPI_CB_VF_VMT_ONACTUALMUTECOLORCHANGED 91

// vioN of type IPlfApiVideoOutputNotify
#define PAPI_CB_VF_VIO_ONBRIGHTNESSCHANGED 							100
#define PAPI_CB_VF_VIO_ONCONTRASTCHANGED 							101
#define PAPI_CB_VF_VIO_ONCOLORCHANGED 								102
#define PAPI_CB_VF_VIO_ONHUEALLOWEDCHANGED 							103
#define PAPI_CB_VF_VIO_ONHUECHANGED 								104
//#define PAPI_CB_VF_VIO_ONTINTCHANGED 								105
#define PAPI_CB_VF_VIO_ONBLANKEDCHANGED								106
#define PAPI_CB_VF_VIO_ONENABLEBLANKINGCHANGED								107
#define PAPI_CB_VF_VIO_ONFIELDFREQUENCYCHANGED								108
#define PAPI_CB_VF_VIO_ONINTERLACEDCHANGED								109
#define PAPI_CB_VF_VIO_ONNUMBEROFLINESCHANGED								110

// vimN of type IPlfApiVideoImprovementNotify
#define PAPI_CB_VF_VIM_ONDNRCHANGED 								120
#define PAPI_CB_VF_VIM_ONLTICHANGED 								121
#define PAPI_CB_VF_VIM_ONSHARPNESSCHANGED 							122
#define PAPI_CB_VF_VIM_ONCOLORENHANCEMENTCHANGED 					123
#define PAPI_CB_VF_VIM_ONDYNAMICCONTRASTCHANGED 					124
#define PAPI_CB_VF_VIM_ONMPEGNOISEREDUCTIONCHANGED 					125
#define PAPI_CB_VF_VIM_ONDIGITALNATURALMOTIONCHANGED 				126
#define PAPI_CB_VF_VIM_ONDIGITALNATURALMOTIONALLOWEDNESSCHANGED 	117

// demoN of type IPlfApiDemoModeNotify
//#define PAPI_CB_VF_VDM_ONDEMOMODECHANGED 							130
//#define PAPI_CB_VF_VDM_ONDEMOMODECOMPLETED 							131
//#define PAPI_CB_VF_VDM_ONDEMOMODEALLOWEDCHANGED 					133

#define papi_vf_vim_OffDigitalNaturalMotion 0
#define papi_vf_vim_MinDigitalNaturalMotion 0
#define papi_vf_vim_MaxDigitalNaturalMotion 3
#define papi_vf_vim_OffDnr 0
#define papi_vf_vim_MinDnr 0
#define papi_vf_vim_MedDnr 2
#define papi_vf_vim_MaxDnr 3
#define papi_vf_vim_OffDynamicContrast 0
#define papi_vf_vim_MinDynamicContrast 0
#define papi_vf_vim_MedDynamicContrast 2
#define papi_vf_vim_MaxDynamicContrast 3
#define papi_vf_vim_MinSharpness 0
#define papi_vf_vim_MedSharpness 4
#define papi_vf_vim_MaxSharpness 7

#endif  // PAPI_IOCTL_VF_H

