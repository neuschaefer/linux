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
 * Description:
 *
 *---------------------------------------------------------------------------*/
#ifndef DDI_IOCTL_H
#define DDI_IOCTL_H

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <linux/ioctl.h>
#include <linux/mtal_ioctl.h>



//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------

#if 0
/// DDI_IOCTL_2ARG_T: Send 2 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[2];
} DDI_IOCTL_2ARG_T;

/// DDI_IOCTL_3ARG_T: Send 3 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[3];
} DDI_IOCTL_3ARG_T;

/// DDI_IOCTL_4ARG_T: Send 4 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[4];
	
} DDI_IOCTL_4ARG_T;

/// DDI_IOCTL_6ARG_T: Send 6 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[6];
} DDI_IOCTL_6ARG_T;

/// DDI_IOCTL_8ARG_T: Send 8 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[8];
} DDI_IOCTL_8ARG_T;

/// DDI_IOCTL_10ARG_T: Send 12 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[10];
} DDI_IOCTL_10ARG_T;

/// DDI_IOCTL_12ARG_T: Send 12 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[12];
} DDI_IOCTL_12ARG_T;

/// DDI_IOCTL_16ARG_T: Send 16 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[16];
} DDI_IOCTL_16ARG_T;
#endif


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

/// DDI device name
#define DEV_DDI                       "ddi"

/*
#define _IOC_NRBITS	8
#define _IOC_TYPEBITS	8
#define _IOC_SIZEBITS	14
#define _IOC_DIRBITS	2
*/
#define DDI_IOCTYPE_GFXOSD    0
#define DDI_IOCTYPE_PQL          1
#define DDI_IOCTYPE_MICOM     2
/* DDI_IOCTYPE_GFXOSD	0 */
#define DDI_IO_GFXOSD_LOCK						_IOW(DDI_IOCTYPE_GFXOSD, 0, INT32)
#define DDI_IO_GFXOSD_UNLOCK					_IOW(DDI_IOCTYPE_GFXOSD, 1, INT32)
#define DDI_IO_GFXOSD_INIT                                        _IOW(DDI_IOCTYPE_GFXOSD, 2, INT32)
#define DDI_IO_GFXOSD_FINAL                                     _IOW(DDI_IOCTYPE_GFXOSD, 3, INT32)
#define DDI_IO_GFXOSD_GETOSDINFO                          _IOW(DDI_IOCTYPE_GFXOSD, 4, MTAL_IOCTL_2ARG_T)
#define DDI_IO_GFXOSD_ENABLE                                  _IOW(DDI_IOCTYPE_GFXOSD, 5, INT32)
#define DDI_IO_GFXOSD_DISABLE                                 _IOW(DDI_IOCTYPE_GFXOSD, 6, INT32)
#define DDI_IO_GFXOSD_GETVIRTUALOSDINFO            _IOW(DDI_IOCTYPE_GFXOSD, 7, MTAL_IOCTL_2ARG_T)
#define DDI_IO_GFXOSD_SETVIRTUALOSDINFO             _IOW(DDI_IOCTYPE_GFXOSD, 8, MTAL_IOCTL_2ARG_T)
#define DDI_IO_GFXOSD_ENABLEVIRTUALOSD               _IOW(DDI_IOCTYPE_GFXOSD, 9, INT32)
#define DDI_IO_GFXOSD_DISABLEVIRTUALOSD              _IOW(DDI_IOCTYPE_GFXOSD, 10, INT32)
#define DDI_IO_GFXOSD_ISVIRTUALOSDENABLED          _IOW(DDI_IOCTYPE_GFXOSD, 11, INT32)
#define DDI_IO_GFXOSD_SETVIRTUALOSDPALETTE        _IOW(DDI_IOCTYPE_GFXOSD, 12, MTAL_IOCTL_2ARG_T)
#define DDI_IO_GFXOSD_WRITEPIXEL                            _IOW(DDI_IOCTYPE_GFXOSD, 13, MTAL_IOCTL_4ARG_T)
#define DDI_IO_GFXOSD_READPIXEL                              _IOW(DDI_IOCTYPE_GFXOSD, 14, MTAL_IOCTL_4ARG_T)
#define DDI_IO_GFXOSD_FILLRECT                                 _IOW(DDI_IOCTYPE_GFXOSD, 15, MTAL_IOCTL_8ARG_T)
#define DDI_IO_GFXOSD_CLEAR                                      _IOW(DDI_IOCTYPE_GFXOSD, 16, INT32)
#define DDI_IO_GFXOSD_UPDATESCREEN                       _IOW(DDI_IOCTYPE_GFXOSD, 17, MTAL_IOCTL_6ARG_T)
#define DDI_IO_GFXOSD_ALLOCATEFRAME                     _IOW(DDI_IOCTYPE_GFXOSD, 18, INT32)
#define DDI_IO_GFXOSD_FREEFRAME                             _IOW(DDI_IOCTYPE_GFXOSD, 19, INT32)
#define DDI_IO_GFXOSD_BLIT                                         _IOW(DDI_IOCTYPE_GFXOSD, 20, MTAL_IOCTL_3ARG_T)
#define DDI_IO_GFXOSD_STARTTRUEMOTIONDEMO        _IOW(DDI_IOCTYPE_GFXOSD, 21, MTAL_IOCTL_6ARG_T)
#define DDI_IO_GFXOSD_STOPTRUEMOTIONDEMO          _IOW(DDI_IOCTYPE_GFXOSD, 22, INT32)
#define DDI_IO_GFXOSD_SETVIRTUALADDRESS          _IOW(DDI_IOCTYPE_GFXOSD, 23, INT32)
#define DDI_IO_GFXOSD_GETFRAMEBUFFERINFO          _IOW(DDI_IOCTYPE_GFXOSD, 24, INT32)
#define DDI_IO_GFXOSD_PREINIT                                  _IOW(DDI_IOCTYPE_GFXOSD, 25, INT32)
#define DDI_IO_GFXOSD_CREATEPHYSICALOSD             _IOW(DDI_IOCTYPE_GFXOSD, 26, MTAL_IOCTL_8ARG_T)
#define DDI_IO_GFXOSD_SETPHYSICALOSDPALETTE     _IOW(DDI_IOCTYPE_GFXOSD, 27, MTAL_IOCTL_2ARG_T)
#define DDI_IO_GFXOSD_CREATEVIRTUALOSD               _IOW(DDI_IOCTYPE_GFXOSD, 28, MTAL_IOCTL_8ARG_T)
#define DDI_IO_GFXOSD_SCALEPHYSICALOSD               _IOW(DDI_IOCTYPE_GFXOSD, 29, MTAL_IOCTL_10ARG_T)
#define DDI_IO_GFXOSD_ENABLEPHYSICALOSD             _IOW(DDI_IOCTYPE_GFXOSD, 30, INT32)
#define DDI_IO_GFXOSD_DISABLEPHYSICALOSD           _IOW(DDI_IOCTYPE_GFXOSD, 31, INT32)
#define DDI_IO_GFXOSD_FILLRECTVIRTUALOSD            _IOW(DDI_IOCTYPE_GFXOSD, 32, MTAL_IOCTL_8ARG_T)
#define DDI_IO_GFXOSD_FILLRECTPHYSICALOSD          _IOW(DDI_IOCTYPE_GFXOSD, 33, MTAL_IOCTL_8ARG_T)
#define DDI_IO_GFXOSD_BLITADDR                                _IOW(DDI_IOCTYPE_GFXOSD, 34, MTAL_IOCTL_3ARG_T)
#define DDI_IO_GFXOSD_ALLOCMEMORY                        _IOW(DDI_IOCTYPE_GFXOSD, 35, MTAL_IOCTL_3ARG_T)
#define DDI_IO_GFXOSD_FREEMEMORY                          _IOW(DDI_IOCTYPE_GFXOSD, 36, INT32)

/*DDI_IOCTYPE_PQL   1*/
#define DDI_IO_PQL_SETFRESHCOLOR							_IOW(DDI_IOCTYPE_PQL, 0, MTAL_IOCTL_3ARG_T)
#define DDI_IO_PQL_SETCOLORSTANDARD						_IOW(DDI_IOCTYPE_PQL, 1, MTAL_IOCTL_2ARG_T)
#define DDI_IO_PQL_SETEXPCOLORPARAM						_IOW(DDI_IOCTYPE_PQL, 2, MTAL_IOCTL_2ARG_T)
#define DDI_IO_PQL_SETCOLORTEMP								_IOW(DDI_IOCTYPE_PQL, 3, INT32)
#define DDI_IO_PQL_SETGAMMA									_IOW(DDI_IOCTYPE_PQL, 4, MTAL_IOCTL_4ARG_T)
#define DDI_IO_PQL_SETSHARPBYSHOOTGAIN						_IOW(DDI_IOCTYPE_PQL, 5, MTAL_IOCTL_2ARG_T)
#define DDI_IO_PQL_SETSHARPBYCHIPSPECIFICALGORITHM		_IOW(DDI_IOCTYPE_PQL, 6, MTAL_IOCTL_2ARG_T)
#define DDI_IO_PQL_SETSHARPBYLTI							_IOW(DDI_IOCTYPE_PQL, 7, MTAL_IOCTL_2ARG_T)
#define DDI_IO_PQL_SETSHARPBYCTI							_IOW(DDI_IOCTYPE_PQL, 8, MTAL_IOCTL_2ARG_T)
#define DDI_IO_PQL_SETSHARPBYEDGEENHANCE					_IOW(DDI_IOCTYPE_PQL, 9, MTAL_IOCTL_2ARG_T)
#define DDI_IO_PQL_CTRLSHARPBYSCALINGFILTER				_IOW(DDI_IOCTYPE_PQL, 10, MTAL_IOCTL_2ARG_T)
#define DDI_IO_PQL_INITMAINSCALERFILTERID					_IOW(DDI_IOCTYPE_PQL, 11, INT32)
#define DDI_IO_PQL_INITSUBSCALERFILTERID					_IOW(DDI_IOCTYPE_PQL, 12, INT32)
#define DDI_IO_PQL_SETTNRPARAM								_IOW(DDI_IOCTYPE_PQL, 13, INT32)
#define DDI_IO_PQL_SETSPATIALNRPARAM						_IOW(DDI_IOCTYPE_PQL, 14, MTAL_IOCTL_2ARG_T)
#define DDI_IO_PQL_SETMPEGNRPARAM							_IOW(DDI_IOCTYPE_PQL, 15, MTAL_IOCTL_2ARG_T)
#define DDI_IO_PQL_GETADJSUBBRIGHTCONT						_IOW(DDI_IOCTYPE_PQL, 16, INT32)
#define DDI_IO_PQL_SETADJSUBBRIGHTCONT						_IOW(DDI_IOCTYPE_PQL, 17, INT32)
#define DDI_IO_PQL_SETPC										_IOW(DDI_IOCTYPE_PQL, 18, MTAL_IOCTL_3ARG_T)
#define DDI_IO_PQL_SETDCFRESHCONTRAST						_IOW(DDI_IOCTYPE_PQL, 19, INT32)
#define DDI_IO_PQL_SETDCFRESHCONTRASTLUT					_IOW(DDI_IOCTYPE_PQL, 20, INT32)
#define DDI_IO_PQL_GETDCCHIPRAWDATA						_IOW(DDI_IOCTYPE_PQL, 21, INT32)
#define DDI_IO_PQL_GETDCCHIPDATATYPEINFO					_IOW(DDI_IOCTYPE_PQL, 22, INT32)
#define DDI_IO_PQL_SETFILMMODE								_IOW(DDI_IOCTYPE_PQL, 23, MTAL_IOCTL_2ARG_T)
#define DDI_IO_PQL_SETDEINTERPARAM							_IOW(DDI_IOCTYPE_PQL, 24, MTAL_IOCTL_2ARG_T)
#define DDI_IO_PQL_SETDCDYNAMICCOLORVAL					_IOW(DDI_IOCTYPE_PQL, 25, MTAL_IOCTL_2ARG_T)
#define DDI_IO_PQL_SETBLACKLEVEL								_IOW(DDI_IOCTYPE_PQL, 26, MTAL_IOCTL_3ARG_T)
#define DDI_IO_PQL_GETCOLORSYSTEM							_IOW(DDI_IOCTYPE_PQL, 27, INT32)
#define DDI_IO_PQL_SETSCARTCLAMP							_IOW(DDI_IOCTYPE_PQL, 28, INT32)
#define DDI_IO_PQL_SETAFEFILTER								_IOW(DDI_IOCTYPE_PQL, 29, MTAL_IOCTL_4ARG_T)
#define DDI_IO_PQL_CTRLDECODERPARAM						_IOW(DDI_IOCTYPE_PQL, 30, INT32)
#define DDI_IO_PQL_SETBLUEONLY								_IOW(DDI_IOCTYPE_PQL, 31, INT32)
#define DDI_IO_PQL_SETCOLORGAMUT								_IOW(DDI_IOCTYPE_PQL, 32, MTAL_IOCTL_2ARG_T)	//PQL_COLORGAMUT_CTRL_T
#define DDI_IO_PQL_SETXVYCCDATAT								_IOW(DDI_IOCTYPE_PQL, 33, MTAL_IOCTL_2ARG_T)	//PQL_COLORGAMUT_CTRL_T
#define DDI_IO_PQL_SETCOLORFILTER                                                _IOW(DDI_IOCTYPE_PQL, 34, MTAL_IOCTL_4ARG_T)
#define DDI_IO_PQL_SETSHARPBYUSERCONTROL  					_IOW(DDI_IOCTYPE_PQL, 35, MTAL_IOCTL_2ARG_T)
#define DDI_IO_PQL_SETSHARPSYSTEMPARAM						_IOW(DDI_IOCTYPE_PQL, 36, MTAL_IOCTL_4ARG_T)
#define DDI_IO_PQL_GETHDMICOLORDOMAIN								_IOW(DDI_IOCTYPE_PQL, 37, INT32)
#define DDI_IO_PQL_SETNRSYSTEMPARAM								_IOW(DDI_IOCTYPE_PQL, 38, MTAL_IOCTL_2ARG_T)
#define DDI_IO_PQL_SETBLUESTRETCH							_IOW(DDI_IO_PQL_SETCOLORSTANDARD,39 ,INT32)
#define DDI_IO_PQL_SETPARTIALGAIN						_IOW(DDI_IO_PQL_SETCOLORSTANDARD,40 ,INT32)
#define DDI_IO_PQL_SETHUEOFFSETBYLUMA							_IOW(DDI_IO_PQL_SETCOLORSTANDARD,41 ,INT32)
#define DDI_IO_PQL_SETHUEOFFSETBYSAT							_IOW(DDI_IO_PQL_SETCOLORSTANDARD,42 ,INT32)
#define DDI_IO_PQL_SETPARTIALLUMA						_IOW(DDI_IO_PQL_SETCOLORSTANDARD,43 ,INT32)
#define DDI_IO_PQL_SETHUE256							_IOW(DDI_IO_PQL_SETCOLORSTANDARD,44 ,INT32)
#define DDI_IO_PQL_SETSATGAINONY							_IOW(DDI_IO_PQL_SETCOLORSTANDARD,45 ,INT32)

/*DDI_IOCTYPE_MICOM   2*/
#define DDI_IO_MICOM_GETVERSION						_IOW(DDI_IOCTYPE_MICOM, 0, INT32)
#define DDI_IO_MICOM_SETRTCTIME						_IOW(DDI_IOCTYPE_MICOM, 1, INT32)
#define DDI_IO_MICOM_GETRTCTIME						_IOW(DDI_IOCTYPE_MICOM, 2, INT32)	// added by LGE
#define DDI_IO_MICOM_SETONTIME						_IOW(DDI_IOCTYPE_MICOM, 3, MTAL_IOCTL_3ARG_T)
#define DDI_IO_MICOM_CANCELONTIME					_IOW(DDI_IOCTYPE_MICOM, 4, INT32)
#define DDI_IO_MICOM_SETRESERVETIME					_IOW(DDI_IOCTYPE_MICOM, 5, MTAL_IOCTL_2ARG_T)
#define DDI_IO_MICOM_CANCELRESERVETIME				_IOW(DDI_IOCTYPE_MICOM, 6, INT32)
#define DDI_IO_MICOM_GETRESERVETIME					_IOW(DDI_IOCTYPE_MICOM, 7, INT32)
#define DDI_IO_MICOM_GETOTATIME						_IOW(DDI_IOCTYPE_MICOM, 8, INT32)
#define DDI_IO_MICOM_SETOTAONTIME					_IOW(DDI_IOCTYPE_MICOM, 9, INT32)
#define DDI_IO_MICOM_SETOTATIME						_IOW(DDI_IOCTYPE_MICOM, 10, INT32)
#define DDI_IO_MICOM_SENDHOSTREADYCOMMAND			_IOW(DDI_IOCTYPE_MICOM, 11, INT32)
#define DDI_IO_MICOM_SENDSYSPOWERCTRLCOMMAND		_IOW(DDI_IOCTYPE_MICOM, 12, MTAL_IOCTL_2ARG_T)
#define DDI_IO_MICOM_SETPOWERONMODE					_IOW(DDI_IOCTYPE_MICOM, 13, INT32)
#define DDI_IO_MICOM_GETPOWERONMODE					_IOW(DDI_IOCTYPE_MICOM, 14, INT32)
#define DDI_IO_MICOM_SENDCECDATA					_IOW(DDI_IOCTYPE_MICOM, 15, INT32)
#define DDI_IO_MICOM_GETCECDATA						_IOW(DDI_IOCTYPE_MICOM, 16, INT32)
#define DDI_IO_MICOM_GETCECRESULT					_IOW(DDI_IOCTYPE_MICOM, 17, INT32)
#define DDI_IO_MICOM_GETCECFORWARDDATA				_IOW(DDI_IOCTYPE_MICOM, 18, INT32)
#define DDI_IO_MICOM_REQUESTCECFORWARD				_IOW(DDI_IOCTYPE_MICOM, 19, INT32)
#define DDI_IO_MICOM_GETPOWERSTATE					_IOW(DDI_IOCTYPE_MICOM, 20, INT32)
#define DDI_IO_MICOM_CANCELPOWEROFF					_IOW(DDI_IOCTYPE_MICOM, 21, INT32)
#define DDI_IO_MICOM_SETCOMMSETID					_IOW(DDI_IOCTYPE_MICOM, 22, INT32)
#define DDI_IO_MICOM_SETAUTOPOWEROFFON				_IOW(DDI_IOCTYPE_MICOM, 23, MTAL_IOCTL_2ARG_T)
#define DDI_IO_MICOM_GETCECSTATUS					_IOW(DDI_IOCTYPE_MICOM, 24, INT32)


//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------
#ifdef __KERNEL__
    #define DDI_LOG_MASK               (0)
#else
    #define DDI_LOG_MASK               (0xffffffff)
#endif

#define DDIGFXOSD_LOG_MASK   (1 << 0)
#define DDIPQL_LOG_MASK   (1 << 1)
#define DDIMICOM_LOG_MASK (1 << 2)

#define IS_DDIGFXOSD_LOG_ENABLE ((DDI_LOG_MASK & DDIGFXOSD_LOG_MASK) > 0)
#define IS_DDIPQL_LOG_ENABLE ((DDI_LOG_MASK & DDIPQL_LOG_MASK) > 0)
#define IS_DDIMICOM_LOG_ENABLE ((DDI_LOG_MASK & DDIMICOM_LOG_MASK) > 0)

#define DDI_LOG(ENABLE, MOD, fmt...) \
    do \
    {\
        if((ENABLE)) \
        { \
            printf("["MOD"]:%s\n" , __FUNCTION__); \
            printf(fmt); \
        } \
									} while (0)

								
#define DDIGFXOSD_PRINT(fmt...)    DDI_LOG(IS_DDIGFXOSD_LOG_ENABLE, "DDIGFXOSD", fmt)
#define DDIPQL_PRINT(fmt...)    DDI_LOG(IS_DDIPQL_LOG_ENABLE, "DDIPQL", fmt)
#define DDIMICOM_PRINT(fmt...)    DDI_LOG(IS_DDIMICOM_LOG_ENABLE, "DDIMICOM", fmt)
//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------

#ifdef __KERNEL__
extern int ddi_ioctl_gfxosd(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg);
extern int ddi_ioctl_pql(struct inode *inode, struct file *file, unsigned int cmd,
              unsigned long arg);
#endif              
#endif  // MTAL_IOCTL_H
