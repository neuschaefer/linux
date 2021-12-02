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
#ifndef ICE_IOCTL_H
#define ICE_IOCTL_H

#ifdef __cplusplus
extern "C" {
#endif

//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------

#include <linux/ioctl.h>
#include <linux/mtal_ioctl.h>



//-----------------------------------------------------------------------------
// Type definitions
//-----------------------------------------------------------------------------
#if 1
/// MTAL_IOCTL_2ARG_T: Send 2 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[2];
} ICE_IOCTL_2ARG_T;

/// MTAL_IOCTL_4ARG_T: Send 3 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[3];
} ICE_IOCTL_3ARG_T;

/// MTAL_IOCTL_4ARG_T: Send 4 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[4];
	
} ICE_IOCTL_4ARG_T;

/// MTAL_IOCTL_6ARG_T: Send 6 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[6];
} ICE_IOCTL_6ARG_T;

/// MTAL_IOCTL_8ARG_T: Send 8 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[8];
} ICE_IOCTL_8ARG_T;

/// MTAL_IOCTL_10ARG_T: Send 12 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[10];
} ICE_IOCTL_10ARG_T;

/// MTAL_IOCTL_12ARG_T: Send 12 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[12];
} ICE_IOCTL_12ARG_T;

/// MTAL_IOCTL_16ARG_T: Send 16 Arguments between user/kernel mode (ioctl interface)
typedef struct
{
    INT32 ai4Arg[16];
} ICE_IOCTL_16ARG_T;
#endif

//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Constant definitions
//-----------------------------------------------------------------------------

/// ICE device name
#define DEV_ICE                             "ice"

#define ICE_IOCTYPE_AUDIO                   0
#define ICE_IOCTYPE_VIDEO                   1
#define ICE_IOCTYPE_VGA                     2
#define ICE_IOCTYPE_HDMI                    3
#define ICE_IOCTYPE_PMX                    4
#define ICE_IOCTYPE_OSD                    5
#define ICE_IOMTPERIPHERAL_IR 		       6
#define ICE_IOCTYPE_HARDWARE 		       7
#define ICE_IOCTYPE_CEC                     8
#define ICE_IOCTYPE_FILESYSTEM       9
#define ICE_IOCTYPE_TOUCHPAD        10
#define ICE_IOCTYPE_COMMON            11

/* ICE_IOCTYPE_AUDIO    0 */
#define ICE_IO_AUDIO                                _IOW(ICE_IOCTYPE_AUDIO, 0, INT32)
#define ICE_IO_AUDIO_ISAUDIOSTREMTYPESUPPORTED      _IOW(ICE_IOCTYPE_AUDIO, 1, INT32) 
#define ICE_IO_AUDIO_PLAY_BEEP                      _IOW(ICE_IOCTYPE_AUDIO, 2, ICE_IOCTL_4ARG_T) 
#define ICE_IO_AUDIO_GETEQVALUE                     _IOW(ICE_IOCTYPE_AUDIO, 3, ICE_IOCTL_4ARG_T) 
#define ICE_IO_AUDIO_CHECKHEADPHONECOND             _IOW(ICE_IOCTYPE_AUDIO, 4, ICE_IOCTL_4ARG_T) 
#define ICE_IO_AUDIO_GETAUDIOCHLINFO                _IOW(ICE_IOCTYPE_AUDIO, 5, ICE_IOCTL_2ARG_T) 
#define ICE_IO_AUDIO_CHANNEL_MODE                   _IOW(ICE_IOCTYPE_AUDIO, 6, ICE_IOCTL_2ARG_T) 
#define ICE_IO_AUDIO_ISHEAACL2                      _IOW(ICE_IOCTYPE_AUDIO, 7, INT32) 
#define ICE_IO_AUDIO_GET_AQ_VERSION                 _IOW(ICE_IOCTYPE_AUDIO, 8, ICE_IOCTL_3ARG_T) 
#define ICE_IO_AUDIO_SET_SRS_FLAG                   _IOW(ICE_IOCTYPE_AUDIO, 9, UINT32)
#define ICE_IO_AUDIO_SET_MPEG4_FLAG                 _IOW(ICE_IOCTYPE_AUDIO, 10, UINT32)


/* ICE_IOCTYPE_VIDEO    1 */
#define ICE_IO_VIDEO_SETNONLINEAR           _IOW(ICE_IOCTYPE_VIDEO, 0, INT32)
#define ICE_IO_VIDEO_GETAFD                 _IOW(ICE_IOCTYPE_VIDEO, 1, MTAL_IOCTL_2ARG_T)
#define ICE_IO_VIDEO_RESETAFD               _IOW(ICE_IOCTYPE_VIDEO, 2, INT32)
#define ICE_IO_VIDEO_CHKVIDEOCODECSUPPORT   _IOW(ICE_IOCTYPE_VIDEO, 3, INT32)
#define ICE_IO_VIDEO_SET_TEST_MODE          _IOW(ICE_IOCTYPE_VIDEO, 4, INT32)
#define ICE_IO_VIDEO_GET_TEST_MODE          _IOW(ICE_IOCTYPE_VIDEO, 5, INT32)
#define ICE_IO_VIDEO_INIT          _IOW(ICE_IOCTYPE_VIDEO, 6, INT32)
#define ICE_IO_VIDEO_SET_CROP_MODE          _IOW(ICE_IOCTYPE_VIDEO, 7, INT32)
#define ICE_IO_VIDEO_SET_MPEG4_FLAG          _IOW(ICE_IOCTYPE_VIDEO, 8, INT32)
#define ICE_IO_VIDEO_GET_DVDMODE_SIG          _IOW(ICE_IOCTYPE_VIDEO, 9, INT32)


/* ICE_IOCTYPE_VGA      2 */
#define ICE_IO_VGA_SETSYNCMETHOD            _IOW(ICE_IOCTYPE_VGA, 0, INT32)
#define ICE_IO_VGA_GETSYNCMETHOD            _IOW(ICE_IOCTYPE_VGA, 1, INT32)
#define ICE_IO_VGA_ENABLEDISABLESOG       _IOW(ICE_IOCTYPE_VGA, 2, INT32)

#define ICE_IO_HDMI_DEBUG_LEVEL                               _IOW(ICE_IOCTYPE_HDMI, 0 , INT32)
#define ICE_IO_HDMI_INIT                                      _IOW(ICE_IOCTYPE_HDMI, 1 , INT32)
#define ICE_IO_HDMI_SET_RESOLUTION                            _IOW(ICE_IOCTYPE_HDMI, 2 , INT32)
#define ICE_IO_HDMI_SET_OUTPUT_COLOUR_SPACE                   _IOW(ICE_IOCTYPE_HDMI, 3 , INT32)
#define ICE_IO_HDMI_GET_CURRENT_OUTPUT_COLOUR_SPACE           _IOW(ICE_IOCTYPE_HDMI, 4 , INT32)
#define ICE_IO_HDMI_SET_ASPECT_RATIO                          _IOW(ICE_IOCTYPE_HDMI, 5 , INT32)
#define ICE_IO_HDMI_GET_CURRENT_HDMI_VIDEO_OUTPUT_RESOLUTION  _IOW(ICE_IOCTYPE_HDMI, 6 , INT32)
#define ICE_IO_HDMI_SET_CURRENT_HDMI_VIDEO_OUTPUT_RESOLUTION  _IOW(ICE_IOCTYPE_HDMI, 7 , INT32)
#define ICE_IO_HDMI_GET_CURRENT_ASPECT_RATIO                  _IOW(ICE_IOCTYPE_HDMI, 8 , INT32)
#define ICE_IO_HDMI_SET_CURRENT_ASPECT_RATIO                  _IOW(ICE_IOCTYPE_HDMI, 9 , INT32)
#define ICE_IO_HDMI_SET_TV_ASPECT_RATIO                       _IOW(ICE_IOCTYPE_HDMI, 10, INT32)
#define ICE_IO_HDMI_HARDWARE_RESET_TRANSMITTER                _IOW(ICE_IOCTYPE_HDMI, 11, INT32)
#define ICE_IO_HDMI_HARDWARE_RESET_SCALER                     _IOW(ICE_IOCTYPE_HDMI, 12, INT32)
#define ICE_IO_HDMI_HARDWARE_RESET_SCALER_AND_TRANSMITTER     _IOW(ICE_IOCTYPE_HDMI, 13, INT32)
#define ICE_IO_HDMI_MUTE                                      _IOW(ICE_IOCTYPE_HDMI, 14, INT32)
#define ICE_IO_HDMI_UNMUTE                                    _IOW(ICE_IOCTYPE_HDMI, 15, INT32)
#define ICE_IO_HDMI_GET_OUTPUT_AVAILABILITY                   _IOW(ICE_IOCTYPE_HDMI, 16, INT32)
#define ICE_IO_HDMI_IS_SIMPLAY_DEFINED                        _IOW(ICE_IOCTYPE_HDMI, 17, INT32)
#define ICE_IO_HDMI_HDCP_ENABLE_DISABLE                       _IOW(ICE_IOCTYPE_HDMI, 18, INT32)
#define ICE_IO_HDMI_HDCP_RECEIVE_SRM                          _IOW(ICE_IOCTYPE_HDMI, 19, INT32)
#define ICE_IO_HDMI_AUDIO_DELAY                               _IOW(ICE_IOCTYPE_HDMI, 20, INT32)
#define ICE_IO_HDMI_REG_CB_FUNC                               _IOW(ICE_IOCTYPE_HDMI, 21, INT32)
#define ICE_IO_HDMI_GET_CURRENT_AFD                           _IOW(ICE_IOCTYPE_HDMI, 22, INT32)
#define ICE_IO_HDMI_PRINT                                     _IOW(ICE_IOCTYPE_HDMI, 23, INT32)
#define ICE_IO_HDMI_LOAD_HDCP_KEY                          	_IOW(ICE_IOCTYPE_HDMI, 24, INT32)
#define ICE_IO_HDMI_IS_HDCPKEY_OK                             _IOW(ICE_IOCTYPE_HDMI, 25, INT32)

/* ICE_IOCTYPE_PMX    4 */
#define ICE_IO_PMX_SELECTPANEL                               _IOW(ICE_IOCTYPE_PMX, 0, INT32)
#define ICE_IO_PMX_PATTERN                               _IOW(ICE_IOCTYPE_PMX, 1, INT32)
#define ICE_IO_PMX_LISTPANEL                               _IOW(ICE_IOCTYPE_PMX, 2, INT32)
#define ICE_IO_PMX_VIDEOMUTE                               _IOW(ICE_IOCTYPE_PMX, 3, INT32)

#define ICE_IO_OSD_GFXSCALER                               _IOW(ICE_IOCTYPE_OSD, 0, INT32)
#define ICE_IO_OSD_GFXSCALER_ALCOM                               _IOW(ICE_IOCTYPE_OSD, 1, INT32)
#define ICE_IO_OSD_REGIONSCALER                               _IOW(ICE_IOCTYPE_OSD, 2, MTAL_IOCTL_12ARG_T)

#define ICE_IO_IR_ISKEYREPEATON               _IOW(ICE_IOMTPERIPHERAL_IR, 0, INT32)
#define ICE_IO_IR_SETKEYREPEATPARAMETERS      _IOW(ICE_IOMTPERIPHERAL_IR, 1, INT32)
#define ICE_IO_IR_KEYREPEARON				  _IOW(ICE_IOMTPERIPHERAL_IR, 2, INT32)
#define ICE_IO_IR_KEYREPEAROFF				  _IOW(ICE_IOMTPERIPHERAL_IR, 3, INT32)
#define ICE_IO_KEYBOARD_MAGICBUTTON_SELLECTION _IOW(ICE_IOMTPERIPHERAL_IR, 4, INT32)
/* ICE_IOCTYPE_HARDWARE    7*/
#define ICE_IO_HARDWARE_LOCKEDFANEL            _IOW(ICE_IOCTYPE_HARDWARE, 0, INT32)
#define ICE_IO_HARDWARE_WATCHDOGENABLE		    _IOW(ICE_IOCTYPE_HARDWARE, 1, INT32)
#define ICE_IO_HARDWARE_SETTIMEOUT				_IOW(ICE_IOCTYPE_HARDWARE, 2, INT32)
#define ICE_IO_HARDWARE_GETTIMEOUT				_IOW(ICE_IOCTYPE_HARDWARE, 3, INT32)
#define ICE_IO_HARDWARE_LED						_IOW(ICE_IOCTYPE_HARDWARE, 4, INT32)
#define ICE_IO_HARDWARE_SETSTANDBYLED			_IOW(ICE_IOCTYPE_HARDWARE, 5, INT32)
#define ICE_IO_HARDWARE_ACTIVE					_IOW(ICE_IOCTYPE_HARDWARE, 6, INT32)
#define ICE_IO_HARDWARE_PDWNREBOOT         		_IOW(ICE_IOCTYPE_HARDWARE, 7, INT32)
#define ICE_IO_HARDWARE_LOWPOWERSTANBYMODE      _IOW(ICE_IOCTYPE_HARDWARE, 8, INT32)
#define ICE_IO_HARDWARE_WATCHDOG                _IOW(ICE_IOCTYPE_HARDWARE, 9, INT32)
#define ICE_IO_HARDWARE_INIT                    _IOW(ICE_IOCTYPE_HARDWARE, 10, INT32)
#define ICE_IO_HARDWARE_SCARTWAKEUP_CONFIG      _IOW(ICE_IOCTYPE_HARDWARE, 11, INT32)
#define ICE_IO_HARDWARE_EATCHDOG_ENABLE_DISABLE      _IOW(ICE_IOCTYPE_HARDWARE, 12, INT32)
#define ICE_IO_HARDWARE_VGAWAKEUP_CONFIG  _IOW(ICE_IOCTYPE_HARDWARE, 13, INT32)
#define ICE_IO_HARDWARE_BOOTLOGO_CONFIG  _IOW(ICE_IOCTYPE_HARDWARE, 14, INT32)


/* ICE_IOCTYPE_CEC    8 */
#define ICE_IO_CEC_INIT                    _IOW(ICE_IOCTYPE_CEC, 0 , INT32)
#define ICE_IO_CEC_OPEN                  _IOW(ICE_IOCTYPE_CEC, 1 , INT32)
#define ICE_IO_CEC_CLOSE                _IOW(ICE_IOCTYPE_CEC, 2 , INT32)
#define ICE_IO_CEC_SENDDATA          _IOW(ICE_IOCTYPE_CEC, 3 , INT32)

/* ICE_IOCTYPE_FILESYSTEM    9 */
#define ICE_IO_MMAPMALLOCFROMKERNEL                                _IOW(ICE_IOCTYPE_FILESYSTEM, 0, INT32)
#define ICE_IO_MMAPMALLOCFREE                               _IOW(ICE_IOCTYPE_FILESYSTEM, 1, INT32)
#define ICE_IO_UPGRADE_PROFILE_TO_NAND                                _IOW(ICE_IOCTYPE_FILESYSTEM, 2, INT32)
#define ICE_IO_READ_PROFILE_FROM_NAND                                _IOW(ICE_IOCTYPE_FILESYSTEM, 3, INT32)
#define ICE_IO_READ_PRODUCTION_CREDENTIALS_SIZE_FROM_NAND               _IOW(ICE_IOCTYPE_FILESYSTEM, 4, INT32)
#define ICE_IO_HOOKFBM                                                  _IOW(ICE_IOCTYPE_FILESYSTEM, 5, INT32)
#define ICE_IO_RELEASEFBMHOOK                                          _IOW(ICE_IOCTYPE_FILESYSTEM, 6, INT32)
#define ICE_IO_FILESYSTEM_GET_OAD_PARTITION          _IOW(ICE_IOCTYPE_FILESYSTEM, 7, INT32)
#define ICE_IO_FILESYSTEM_SET_OAD_KERNEL_PARTITION          _IOW(ICE_IOCTYPE_FILESYSTEM, 8, INT32)
#define ICE_IO_FILESYSTEM_SET_OAD_ROOTFS_PARTITION          _IOW(ICE_IOCTYPE_FILESYSTEM, 9, INT32)
#define ICE_IO_FILESYSTEM_SET_OAD_BASIC_PARTITION          _IOW(ICE_IOCTYPE_FILESYSTEM, 10, INT32)
#define ICE_IO_FILESYSTEM_GET_BASIC_EEP_SETTING              _IOW(ICE_IOCTYPE_FILESYSTEM, 11, INT32)
#define ICE_IO_FILESYSTEM_ERASE_PARTITION              _IOW(ICE_IOCTYPE_FILESYSTEM, 12, INT32)
#define ICE_IO_FILESYSTEM_OAD_SAVE_PKG_ATRRIBUTES              _IOW(ICE_IOCTYPE_FILESYSTEM, 13, INT32)

/* ICE_IOCTYPE_TOUCHPAD    10 */
#define ICE_IO_TOUCHPAD_INIT                                           _IOW(ICE_IOCTYPE_TOUCHPAD, 0, INT32) 
#define ICE_IO_TOUCHPAD_GETVERSION                              _IOW(ICE_IOCTYPE_TOUCHPAD, 1, INT32)
#define ICE_IO_TOUCHPAD_GETSENSING                              _IOW(ICE_IOCTYPE_TOUCHPAD, 2, INT32)
#define ICE_IO_TOUCHPAD_GETCONFIGURATION                  _IOW(ICE_IOCTYPE_TOUCHPAD, 3, INT32)
#define ICE_IO_TOUCHPAD_SETSENSING                              _IOW(ICE_IOCTYPE_TOUCHPAD, 4, INT32)
#define ICE_IO_TOUCHPAD_SETCONFIGURATION                  _IOW(ICE_IOCTYPE_TOUCHPAD, 5, INT32)
#define ICE_IO_TOUCHPAD_SETBUTTONRANGE                     _IOW(ICE_IOCTYPE_TOUCHPAD, 6, INT32)
#define ICE_IO_TOUCHPAD_READSERVADC                           _IOW(ICE_IOCTYPE_TOUCHPAD, 7, INT32)

/*ICE_IOCTYPE_COMMON       11*/
#define ICE_IO_COMMON_GET_LOADER_VERSION                                           _IOW(ICE_IOCTYPE_COMMON, 0, INT32) 
#define ICE_IO_COMMON_SET_OAD_ENABLE_FLAG                                           _IOW(ICE_IOCTYPE_COMMON, 1, INT32) 
#define ICE_IO_COMMON_GET_OAD_ENABLE_FLAG                                           _IOW(ICE_IOCTYPE_COMMON, 2, INT32) 
//-----------------------------------------------------------------------------
// Macro definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype  of inter-file functions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Prototype  of public functions
//-----------------------------------------------------------------------------

#ifdef __KERNEL__
extern int ice_ioctl_vga(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);
extern int ice_ioctl_aud(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);
extern int ice_ioctl_ir(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);
extern int ice_ioctl_hardware(struct inode *inode, struct file *file,
    unsigned int cmd, unsigned long arg);
extern int ice_ioctl_hdmi(struct inode *inode, struct file *file, unsigned int cmd,			  unsigned long arg);
	extern int ice_ioctl_pmx(struct inode *inode, struct file *file, unsigned int cmd,	unsigned long arg);
	extern int ice_ioctl_common(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg);


extern int ice_ioctl_video(struct inode *inode, struct file *file, unsigned int cmd,			  unsigned long arg);
extern int ice_ioctl_touchpad(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg);


extern int ice_ioctl_osd(struct inode *inode, struct file *file, unsigned int cmd,				  unsigned long arg);

extern int ice_ioctl_filesystem(struct inode *inode, struct file *file, unsigned int cmd,            unsigned long arg);

#endif  // #ifdef  __kernel__

#ifdef __cplusplus
}
#endif

#endif  // ICE_IOCTL_H






