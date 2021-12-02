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
 * $RCSfile: io_papi_fe.c,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file io_papi_fe.c
 *  PAPI kernel driver implementation: Frontend
 */

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <linux/papi_ioctl.h>
#include "papi_types.h"

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Static variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// External function prototypes
//-----------------------------------------------------------------------------

extern int papi_ioctl_fe_tsdmx(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int papi_ioctl_fe_vip(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int papi_ioctl_fe_tun(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int papi_ioctl_fe_vbi(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int papi_ioctl_fe_col(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

extern int papi_ioctl_fe_sound(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);

//-----------------------------------------------------------------------------
// Static functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

int papi_ioctl_fe(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg)
{
    int i4Ret = 0;

    switch (cmd)
    {
    case PAPI_IO_FE_VIP_GETVIDEOPRESENT:
    case PAPI_IO_FE_VIP_GETIMAGEFORMAT:
    case PAPI_IO_FE_VIP_GETMACROVISIONTYPE:
    case PAPI_IO_FE_VIP_GETFIELDFREQUENCY:
    case PAPI_IO_FE_VIP_ISFILMMODE:
    case PAPI_IO_FE_VIP_GETNUMBEROFVISIBLEPIXELS:
    case PAPI_IO_FE_VIP_ISFROMVCR:
    case PAPI_IO_FE_VIP_GETSIGNALPRESENT:
    case PAPI_IO_FE_VIP_GETINTERLACE:
    case PAPI_IO_FE_VIP_GETNUMBEROFVISIBLELINES:
    case PAPI_IO_FE_VIP_GETVIDEOCODING:
    case PAPI_IO_FE_VIP_GETNUMBEROFFRAMELINES:
    case PAPI_IO_FE_VIP_GETVIDEOPROTECTION:
    case PAPI_IO_FE_VIP_ISPCFORMAT:
        i4Ret = papi_ioctl_fe_vip(inode, file, cmd, arg);
        break;

    case PAPI_IO_FE_TUN_INIT:
    case PAPI_IO_FE_TUN_GETTVSYSTEM:
    case PAPI_IO_FE_TUN_GETSEARCHSTATE:
    case PAPI_IO_FE_TUN_GETSEARCHMODE:
    case PAPI_IO_FE_TUN_GETFREQ:
    case PAPI_IO_FE_TUN_GETSTATIONPRESENT:
    case PAPI_IO_FE_TUN_GETCHAN:
    case PAPI_IO_FE_TUN_ENABLEAFC:
    case PAPI_IO_FE_TUN_GETAFCENABLED:
    case PAPI_IO_FE_TUN_GETAFCVALUE:
    case PAPI_IO_FE_TUN_GETAFCSUPP:
	case PAPI_IO_FE_TUN_STOPSEARCH:
	case PAPI_IO_FE_TUN_SETTVSYSTEM:
	case PAPI_IO_FE_TUN_GETFREQRANGE:
	case PAPI_IO_FE_TUN_GETTVSYSTEMSUPP:
    case PAPI_IO_FE_TUN_GETSEARCHMODESUPP:
    case PAPI_IO_FE_TUN_GETCHANOFFSET:
    case PAPI_IO_FE_TUN_GETCHANOFFSETRANGE:
	case PAPI_IO_FE_TUN_STARTSEARCH:
    case PAPI_IO_FE_TUN_SETFREQ:
    case PAPI_IO_FE_TUN_SETCHAN:
    case PAPI_IO_FE_QSSM_GETMEASVALID:
    case PAPI_IO_FE_QSSM_GETSIGSTRENGTHRANGE:
    case PAPI_IO_FE_QSSM_GETSIGSTRENGTH:
    case PAPI_IO_FE_QSSM_START:
    case PAPI_IO_FE_QSSM_STOP:
    case PAPI_IO_FE_CHANTAB_GETCARRIERFREQ:
    case PAPI_IO_FE_CHANTAB_GETFIRSTCHAN:
    case PAPI_IO_FE_CHANTAB_GETFIRSTTABLE:
    case PAPI_IO_FE_CHANTAB_FREQ2CHAN:
    case PAPI_IO_FE_CHANTAB_GETISCHAN:
    case PAPI_IO_FE_CHANTAB_GETLASTCHAN:
    case PAPI_IO_FE_CHANTAB_GETLOWERFREQ:
    case PAPI_IO_FE_CHANTAB_GETNEXTCHAN:
    case PAPI_IO_FE_CHANTAB_GETNEXTTABLE:
    case PAPI_IO_FE_CHANTAB_GETNEXTVALIDCHAN:
    case PAPI_IO_FE_CHANTAB_GETPREVCHAN:
    case PAPI_IO_FE_CHANTAB_GETPREVVALIDCHAN:
    case PAPI_IO_FE_CHANTAB_GETSEGMENTBOUNDARIES:
    case PAPI_IO_FE_CHANTAB_GETTABLESUPP:
    case PAPI_IO_FE_CHANTAB_GETTABLE:
    case PAPI_IO_FE_CHANTAB_SETTABLE:
    case PAPI_IO_FE_CHANTAB_GETUPPERFREQ:
    // ISDB-T part
    case PAPI_IO_FE_CHDECISDBT_GETCHANNELINFORMATION:
    case PAPI_IO_FE_CHDECISDBT_SETCHANNELINFORMATION:
    case PAPI_IO_FE_CHDECISDBT_GETACTUALCODERATES:
    case PAPI_IO_FE_CHDECISDBT_GETSUPPCODERATES:
    case PAPI_IO_FE_CHDECISDBT_GETCODERATES:
    case PAPI_IO_FE_CHDECISDBT_SETCODERATES:
    case PAPI_IO_FE_CHDECISDBT_GETACTUALCONSTELLATION:
    case PAPI_IO_FE_CHDECISDBT_GETCONSTELLATION:
    case PAPI_IO_FE_CHDECISDBT_SETCONSTELLATION:
    case PAPI_IO_FE_CHDECISDBT_GETSUPPCONSTELLATIONS:
    case PAPI_IO_FE_CHDECISDBT_GETACTUALGUARDINTERVAL:
    case PAPI_IO_FE_CHDECISDBT_GETGUARDINTERVAL:
    case PAPI_IO_FE_CHDECISDBT_SETGUARDINTERVAL:
    case PAPI_IO_FE_CHDECISDBT_GETSUPPGUARDINTERVALS:
    case PAPI_IO_FE_CHDECISDBT_GETLAYER:
    case PAPI_IO_FE_CHDECISDBT_SETLAYER:
    case PAPI_IO_FE_CHDECISDBT_GETSUPPORTEDLAYERS:
        i4Ret = papi_ioctl_fe_tun(inode, file, cmd, arg);
        break;

    case PAPI_IO_FE_VBISL_ENABLEAUTOSLICING:
    case PAPI_IO_FE_VBISL_GETAUTOSLICINGENABLED:
    case PAPI_IO_FE_VBISL_GETVBILINES:
    case PAPI_IO_FE_VBISL_SETSLICERMODE:
    case PAPI_IO_FE_VBISL_SETVBILINES:
    	i4Ret = papi_ioctl_fe_vbi(inode, file, cmd, arg);
    	break;

    case PAPI_IO_FE_TSDMX_SETAUDIODESCRIPTIVEPID:
    case PAPI_IO_FE_TSDMX_GETAUDIODESCRIPTIVEPID:
    case PAPI_IO_FE_TSDMX_STOPAUDIODESCRIPTIVEPID:
    case PAPI_IO_FE_TSDMX_SETAUDIOPID:
    case PAPI_IO_FE_TSDMX_GETAUDIOPID:
    case PAPI_IO_FE_TSDMX_STOPAUDIOPID:
    case PAPI_IO_FE_TSDMX_SETVIDEOPID:
    case PAPI_IO_FE_TSDMX_GETVIDEOPID:
    case PAPI_IO_FE_TSDMX_STOPVIDEOPID:
    case PAPI_IO_FE_TSDMX_SETPCRPID:
    case PAPI_IO_FE_TSDMX_GETPCRPID:
    case PAPI_IO_FE_TSDMX_STOPPCRPID:
    case PAPI_IO_FE_TSDMX_ISREQUESTALLOWED:
    case PAPI_IO_FE_TSDMX_REQUESTFILTER:
    case PAPI_IO_FE_TSDMX_UPDATEFILTER:
    case PAPI_IO_FE_TSDMX_CANCELFILTER:
    case PAPI_IO_FE_TSDMX_FREESECTIONBUFFER:
    case PAPI_IO_FE_TSDMX_REQUESTPES:
    case PAPI_IO_FE_TSDMX_CANCELPES:
    case PAPI_IO_FE_TSDMX_FREEPESBUFFER:
    case PAPI_IO_FE_TSDMX_GETSCRAMBLINGSTATUS:
    case PAPI_IO_FE_TSDMX_COPYDATA:
    case PAPI_IO_FE_TSDMX_UNLOCKDATA:
    case PAPI_IO_FE_TSDMX_SETAUDIOTYPE:
    case PAPI_IO_FE_TSDMX_SETVIDEOTYPE:
    case PAPI_IO_FE_TSDMX_REQUESTLISTFILTER:
    case PAPI_IO_FE_TSDMX_COMMAND:
    case PAPI_IO_FE_TSDMX_CLEANUP:
        i4Ret = papi_ioctl_fe_tsdmx(inode, file, cmd, arg);
        break;

    case PAPI_IO_FE_COL_COLORSYSTEMSUPPORTEDEXT:
    case PAPI_IO_FE_COL_FORCECOLORSYSTEMEX:
    case PAPI_IO_FE_COL_GETFORCECOLORSYSTEM:
    case PAPI_IO_FE_COL_GETFORCECARRIERFREQUENCY:
    case PAPI_IO_FE_COL_SUGGESTCOLORSYSTEMEX:
    case PAPI_IO_FE_COL_GETSUGGESTEDCOLORSYSTEM:
    case PAPI_IO_FE_COL_GETSUGGESTEDCARRIERFREQUENCY:
    case PAPI_IO_FE_COL_GETCOLORSYSTEM:
    case PAPI_IO_FE_COL_GETCARRIERFREQUENCY:
    case PAPI_IO_FE_COL_COMBFILTERSUPPORTED:
    case PAPI_IO_FE_COL_ENABLECOMBFILTER:
    case PAPI_IO_FE_COL_COMBFILTERENABLED:
    case PAPI_IO_FE_COL_COMBFILTERACTIVE:
        i4Ret = papi_ioctl_fe_col(inode, file, cmd, arg);
        break;

    case PAPI_IO_FE_SOUND_GETSNDSYS:
        i4Ret = papi_ioctl_fe_sound(inode, file, cmd, arg);
        break;

    default:
        printk("%s(): unknown I/O command 0x%08x\n", __FUNCTION__, cmd);
        i4Ret = -EFAULT;
    }

    return i4Ret;
}




