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
 * $RCSfile: papi_ioctl_fe.h,v $
 * $Revision: #1 $
 *
 *---------------------------------------------------------------------------*/

/** @file papi_ioctl_fe.h
 *  IO control of PAPI Frontend interface
 */

#ifndef PAPI_IOCTL_FE_H
#define PAPI_IOCTL_FE_H

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

// TSDMX callback command type
#define TSDMX_CB_SECTION                    0
#define TSDMX_CB_PES                        1
#define TSDMX_CB_OVERFLOW                   2
#define TSDMX_CB_SCRAMBLECHANGE             3
#define TSDMX_CB_AUDIOPIDSTOPPED            4
#define TSDMX_CB_AUDIODESCPIDSTOPPED        5
#define TSDMX_CB_VIDEOPIDSTOPPED            6
#define TSDMX_CB_PCRPIDSTOPPED              7
#define TSDMX_CB_REQUESTALLOWED             8
#define TSDMX_CB_REQUESTNOTALLOWED          9
#define TSDMX_CB_REQUESTCANCELLED           10
#define TSDMX_CB_SYNCLOCK                   11
#define TSDMX_CB_SYNCLOST                   12
#define TSDMX_CB_PESCANCELLED               13
#define TSDMX_CB_LISTFILTERTIMEOUT          14
#define TSDMX_CB_REQUESTLISTCANCELLED       15


// TSDMX pid type
#define TSDMX_PID_TYPE_SECTION              0
#define TSDMX_PID_TYPE_PES                  1
#define TSDMX_PID_TYPE_MASK                 0xFF


// TSDMX Timer commands
// Note: Number the constants for Timer in sequence. See _TSDMX_Command().
#define TSDMX_CMD_STOP_TIMER                0
#define TSDMX_CMD_START_TIMER               1
#define TSDMX_CMD_RESET_TIMER               2
#define TSDMX_CMD_DISABLE_FILTER            3
#define TSDMX_CMD_ENABLE_FILTER             4


// Misc: 0
#define PAPI_IO_FE_STCDEC_GETTIME           _IOW(PAPI_IOCTYPE_FE, 0, int)
#define PAPI_IO_FE_SND_ALLOWSOUNDSYSTEM     _IOW(PAPI_IOCTYPE_FE, 2, int)
#define PAPI_IO_FE_SSYS_GETSOUNDSYSTEM      _IOW(PAPI_IOCTYPE_FE, 3, int)

// VBI: 10
#define PAPI_IO_FE_VBISL_ENABLEAUTOSLICING       _IOW(PAPI_IOCTYPE_FE, 10, int)
#define PAPI_IO_FE_VBISL_GETAUTOSLICINGENABLED   _IOW(PAPI_IOCTYPE_FE, 11, int)
#define PAPI_IO_FE_VBISL_GETVBILINES             _IOW(PAPI_IOCTYPE_FE, 12, int)
#define PAPI_IO_FE_VBISL_SETSLICERMODE           _IOW(PAPI_IOCTYPE_FE, 13, int)
#define PAPI_IO_FE_VBISL_SETVBILINES             _IOW(PAPI_IOCTYPE_FE, 14, int)

// Video : 20
#define PAPI_IO_FE_VIP_GETVIDEOPRESENT      _IOW(PAPI_IOCTYPE_FE, 20, int)
#define PAPI_IO_FE_VIP_GETIMAGEFORMAT       _IOW(PAPI_IOCTYPE_FE, 21, int)
#define PAPI_IO_FE_VIP_GETMACROVISIONTYPE   _IOW(PAPI_IOCTYPE_FE, 22, int)
#define PAPI_IO_FE_VIP_GETFIELDFREQUENCY    _IOW(PAPI_IOCTYPE_FE, 23, int)
#define PAPI_IO_FE_VIP_ISFILMMODE           _IOW(PAPI_IOCTYPE_FE, 24, int)
#define PAPI_IO_FE_VIP_GETNUMBEROFVISIBLEPIXELS _IOW(PAPI_IOCTYPE_FE, 25, int)
#define PAPI_IO_FE_VIP_ISFROMVCR            _IOW(PAPI_IOCTYPE_FE, 26, int)
#define PAPI_IO_FE_VIP_GETSIGNALPRESENT     _IOW(PAPI_IOCTYPE_FE, 27, int)
#define PAPI_IO_FE_VIP_GETINTERLACE         _IOW(PAPI_IOCTYPE_FE, 28, int)
#define PAPI_IO_FE_VIP_GETNUMBEROFVISIBLELINES _IOW(PAPI_IOCTYPE_FE, 29, int)
#define PAPI_IO_FE_VIP_GETVIDEOCODING       _IOW(PAPI_IOCTYPE_FE, 30, int)
#define PAPI_IO_FE_VIP_GETNUMBEROFFRAMELINES _IOW(PAPI_IOCTYPE_FE, 31, int)
#define PAPI_IO_FE_VIP_GETVIDEOPROTECTION   _IOW(PAPI_IOCTYPE_FE, 32, int)
#define PAPI_IO_FE_VIP_ISPCFORMAT           _IOW(PAPI_IOCTYPE_FE, 33, IOCTL_4ARG_T)

// Color Decode : 40
#define PAPI_IO_FE_COL_COLORSYSTEMSUPPORTEDEXT _IOW(PAPI_IOCTYPE_FE, 40, int)
#define PAPI_IO_FE_COL_FORCECOLORSYSTEMEX _IOW(PAPI_IOCTYPE_FE, 41, int)
#define PAPI_IO_FE_COL_GETFORCECOLORSYSTEM _IOW(PAPI_IOCTYPE_FE, 42, int)
#define PAPI_IO_FE_COL_GETFORCECARRIERFREQUENCY _IOW(PAPI_IOCTYPE_FE, 43, int)
#define PAPI_IO_FE_COL_SUGGESTCOLORSYSTEMEX _IOW(PAPI_IOCTYPE_FE, 44, int)
#define PAPI_IO_FE_COL_GETSUGGESTEDCOLORSYSTEM _IOW(PAPI_IOCTYPE_FE, 45, int)
#define PAPI_IO_FE_COL_GETSUGGESTEDCARRIERFREQUENCY _IOW(PAPI_IOCTYPE_FE, 46, int)
#define PAPI_IO_FE_COL_GETCOLORSYSTEM _IOW(PAPI_IOCTYPE_FE, 47, int)
#define PAPI_IO_FE_COL_GETCARRIERFREQUENCY _IOW(PAPI_IOCTYPE_FE, 48, int)
#define PAPI_IO_FE_COL_COMBFILTERSUPPORTED _IOW(PAPI_IOCTYPE_FE, 49, int)
#define PAPI_IO_FE_COL_ENABLECOMBFILTER _IOW(PAPI_IOCTYPE_FE, 50, int)
#define PAPI_IO_FE_COL_COMBFILTERENABLED _IOW(PAPI_IOCTYPE_FE, 51, int)
#define PAPI_IO_FE_COL_COMBFILTERACTIVE _IOW(PAPI_IOCTYPE_FE, 52, int)

// preset: 60
#define PAPI_IO_FE_PRS_INITPRESET _IOW(PAPI_IOCTYPE_FE, 60, int)
#define PAPI_IO_FE_PRS_GETMINPRESET _IOW(PAPI_IOCTYPE_FE, 61, int)
#define PAPI_IO_FE_PRS_GETMAXPRESET _IOW(PAPI_IOCTYPE_FE, 62, int)
#define PAPI_IO_FE_PRS_SETPRESET _IOW(PAPI_IOCTYPE_FE, 63, int)
#define PAPI_IO_FE_PRS_GETPRESET _IOW(PAPI_IOCTYPE_FE, 64, int)
#define PAPI_IO_FE_PRS_FILLPRESET _IOW(PAPI_IOCTYPE_FE, 65, int)
#define PAPI_IO_FE_PRS_STOREPRESET _IOW(PAPI_IOCTYPE_FE, 66, int)
#define PAPI_IO_FE_PRS_LOADPRESET _IOW(PAPI_IOCTYPE_FE, 67, int)
#define PAPI_IO_FE_PRS_STOREPRESETSUPPORTED _IOW(PAPI_IOCTYPE_FE, 68, int)
#define PAPI_IO_FE_PRS_SWAPPRESET _IOW(PAPI_IOCTYPE_FE, 69, int)
#define PAPI_IO_FE_PRS_SETATTRIBUTE _IOW(PAPI_IOCTYPE_FE, 70, int)
#define PAPI_IO_FE_PRS_GETATTRIBUTE _IOW(PAPI_IOCTYPE_FE, 71, int)
#define PAPI_IO_FE_PRS_LOADATTRIBUTE _IOW(PAPI_IOCTYPE_FE, 72, int)
#define PAPI_IO_FE_PRS_STOREATTRIBUTE _IOW(PAPI_IOCTYPE_FE, 73, int)
#define PAPI_IO_FE_PRS_STOREATTRIBUTESUPPORTED _IOW(PAPI_IOCTYPE_FE, 74, int)

// Tsdmx : 80
#define PAPI_IO_FE_TSDMX_SETAUDIODESCRIPTIVEPID _IOW(PAPI_IOCTYPE_FE, 80, int)
#define PAPI_IO_FE_TSDMX_GETAUDIODESCRIPTIVEPID _IOW(PAPI_IOCTYPE_FE, 81, int)
#define PAPI_IO_FE_TSDMX_STOPAUDIODESCRIPTIVEPID _IOW(PAPI_IOCTYPE_FE, 82, int)
#define PAPI_IO_FE_TSDMX_SETAUDIOPID        _IOW(PAPI_IOCTYPE_FE, 83, int)
#define PAPI_IO_FE_TSDMX_GETAUDIOPID        _IOW(PAPI_IOCTYPE_FE, 84, int)
#define PAPI_IO_FE_TSDMX_STOPAUDIOPID       _IOW(PAPI_IOCTYPE_FE, 85, int)
#define PAPI_IO_FE_TSDMX_SETVIDEOPID        _IOW(PAPI_IOCTYPE_FE, 86, int)
#define PAPI_IO_FE_TSDMX_GETVIDEOPID        _IOW(PAPI_IOCTYPE_FE, 87, int)
#define PAPI_IO_FE_TSDMX_STOPVIDEOPID       _IOW(PAPI_IOCTYPE_FE, 88, int)
#define PAPI_IO_FE_TSDMX_SETPCRPID          _IOW(PAPI_IOCTYPE_FE, 89, int)
#define PAPI_IO_FE_TSDMX_GETPCRPID          _IOW(PAPI_IOCTYPE_FE, 90, int)
#define PAPI_IO_FE_TSDMX_STOPPCRPID         _IOW(PAPI_IOCTYPE_FE, 91, int)
#define PAPI_IO_FE_TSDMX_ISREQUESTALLOWED   _IOW(PAPI_IOCTYPE_FE, 92, int)
#define PAPI_IO_FE_TSDMX_REQUESTFILTER      _IOW(PAPI_IOCTYPE_FE, 93, int)
#define PAPI_IO_FE_TSDMX_UPDATEFILTER       _IOW(PAPI_IOCTYPE_FE, 94, int)
#define PAPI_IO_FE_TSDMX_CANCELFILTER       _IOW(PAPI_IOCTYPE_FE, 95, int)
#define PAPI_IO_FE_TSDMX_FREESECTIONBUFFER  _IOW(PAPI_IOCTYPE_FE, 96, int)
#define PAPI_IO_FE_TSDMX_REQUESTPES         _IOW(PAPI_IOCTYPE_FE, 97, int)
#define PAPI_IO_FE_TSDMX_CANCELPES          _IOW(PAPI_IOCTYPE_FE, 98, int)
#define PAPI_IO_FE_TSDMX_FREEPESBUFFER      _IOW(PAPI_IOCTYPE_FE, 99, int)
#define PAPI_IO_FE_TSDMX_GETSCRAMBLINGSTATUS _IOW(PAPI_IOCTYPE_FE, 100, int)
#define PAPI_IO_FE_TSDMX_COPYDATA           _IOW(PAPI_IOCTYPE_FE, 101, int)
#define PAPI_IO_FE_TSDMX_UNLOCKDATA         _IOW(PAPI_IOCTYPE_FE, 102, int)
#define PAPI_IO_FE_TSDMX_SETAUDIOTYPE       _IOW(PAPI_IOCTYPE_FE, 103, int)
#define PAPI_IO_FE_TSDMX_SETVIDEOTYPE       _IOW(PAPI_IOCTYPE_FE, 104, int)
#define PAPI_IO_FE_TSDMX_REQUESTLISTFILTER  _IOW(PAPI_IOCTYPE_FE, 105, int)
//#define PAPI_IO_FE_TSDMX_SETGLUESN          _IOW(PAPI_IOCTYPE_FE, 106, int)
#define PAPI_IO_FE_TSDMX_COMMAND            _IOW(PAPI_IOCTYPE_FE, 107, int)
#define PAPI_IO_FE_TSDMX_CLEANUP            _IOW(PAPI_IOCTYPE_FE, 108, int)


// Tuner : 120
#define PAPI_IO_FE_TUN_INIT                             _IOW(PAPI_IOCTYPE_FE, 120, int)
#define PAPI_IO_FE_TUN_GETSEARCHMODESUPP                _IOW(PAPI_IOCTYPE_FE, 121, int)
#define PAPI_IO_FE_TUN_SETFREQ                          _IOW(PAPI_IOCTYPE_FE, 122, int)
#define PAPI_IO_FE_TUN_GETCHAN                          _IOW(PAPI_IOCTYPE_FE, 123, int)
#define PAPI_IO_FE_TUN_STARTSEARCH                      _IOW(PAPI_IOCTYPE_FE, 124, int)
#define PAPI_IO_FE_TUN_SETTVSYSTEM                      _IOW(PAPI_IOCTYPE_FE, 125, int)
#define PAPI_IO_FE_TUN_GETSTATIONPRESENT                _IOW(PAPI_IOCTYPE_FE, 126, int)
#define PAPI_IO_FE_TUN_GETTVSYSTEM                      _IOW(PAPI_IOCTYPE_FE, 127, int)
#define PAPI_IO_FE_TUN_GETSEARCHSTATE                   _IOW(PAPI_IOCTYPE_FE, 128, int)
#define PAPI_IO_FE_TUN_STOPSEARCH                       _IOW(PAPI_IOCTYPE_FE, 129, int)
#define PAPI_IO_FE_TUN_GETSEARCHMODE                    _IOW(PAPI_IOCTYPE_FE, 130, int)
#define PAPI_IO_FE_TUN_GETAFCVALUE                      _IOW(PAPI_IOCTYPE_FE, 131, int)
#define PAPI_IO_FE_TUN_GETAFCENABLED                    _IOW(PAPI_IOCTYPE_FE, 132, int)
#define PAPI_IO_FE_TUN_GETAFCSUPP                       _IOW(PAPI_IOCTYPE_FE, 133, int)
#define PAPI_IO_FE_TUN_ENABLEAFC                        _IOW(PAPI_IOCTYPE_FE, 134, int)
#define PAPI_IO_FE_TUN_GETFREQRANGE                     _IOW(PAPI_IOCTYPE_FE, 135, int)
#define PAPI_IO_FE_TUN_GETTVSYSTEMSUPP                  _IOW(PAPI_IOCTYPE_FE, 136, int)
#define PAPI_IO_FE_TUN_GETFREQ                          _IOW(PAPI_IOCTYPE_FE, 137, int)
#define PAPI_IO_FE_TUN_SETCHAN                          _IOW(PAPI_IOCTYPE_FE, 138, int)
#define PAPI_IO_FE_TUN_GETCHANOFFSETRANGE               _IOW(PAPI_IOCTYPE_FE, 139, int)
#define PAPI_IO_FE_TUN_GETCHANOFFSET                    _IOW(PAPI_IOCTYPE_FE, 140, int)

// CHANTAB: 143
#define PAPI_IO_FE_CHANTAB_GETCARRIERFREQ               _IOW(PAPI_IOCTYPE_FE, 143, int)
#define PAPI_IO_FE_CHANTAB_GETFIRSTCHAN                 _IOW(PAPI_IOCTYPE_FE, 144, int)
#define PAPI_IO_FE_CHANTAB_GETFIRSTTABLE                _IOW(PAPI_IOCTYPE_FE, 145, int)
#define PAPI_IO_FE_CHANTAB_FREQ2CHAN                    _IOW(PAPI_IOCTYPE_FE, 146, int)
#define PAPI_IO_FE_CHANTAB_GETISCHAN                    _IOW(PAPI_IOCTYPE_FE, 147, int)
#define PAPI_IO_FE_CHANTAB_GETLASTCHAN                  _IOW(PAPI_IOCTYPE_FE, 148, int)
#define PAPI_IO_FE_CHANTAB_GETLOWERFREQ                 _IOW(PAPI_IOCTYPE_FE, 149, int)
#define PAPI_IO_FE_CHANTAB_GETNEXTCHAN                  _IOW(PAPI_IOCTYPE_FE, 150, int)
#define PAPI_IO_FE_CHANTAB_GETNEXTTABLE                 _IOW(PAPI_IOCTYPE_FE, 151, int)
#define PAPI_IO_FE_CHANTAB_GETNEXTVALIDCHAN             _IOW(PAPI_IOCTYPE_FE, 152, int)
#define PAPI_IO_FE_CHANTAB_GETPREVCHAN                  _IOW(PAPI_IOCTYPE_FE, 153, int)
#define PAPI_IO_FE_CHANTAB_GETPREVVALIDCHAN             _IOW(PAPI_IOCTYPE_FE, 154, int)
#define PAPI_IO_FE_CHANTAB_GETSEGMENTBOUNDARIES         _IOW(PAPI_IOCTYPE_FE, 155, int)
#define PAPI_IO_FE_CHANTAB_GETTABLESUPP                 _IOW(PAPI_IOCTYPE_FE, 156, int)
#define PAPI_IO_FE_CHANTAB_GETTABLE                     _IOW(PAPI_IOCTYPE_FE, 157, int)
#define PAPI_IO_FE_CHANTAB_SETTABLE                     _IOW(PAPI_IOCTYPE_FE, 158, int)
#define PAPI_IO_FE_CHANTAB_GETUPPERFREQ                 _IOW(PAPI_IOCTYPE_FE, 159, int)

// CHDEC: 160
/*
#define PAPI_IO_FE_CHDEC_DVBT_SETCHANBANDWIDTH	        _IOW(PAPI_IOCTYPE_FE, 160, int)
#define PAPI_IO_FE_CHDEC_GETCARRIERPRESENT		        _IOW(PAPI_IOCTYPE_FE, 161, int)
#define	PAPI_IO_FE_CHDEC_GETSUPPCONSTELLATIONS	        _IOW(PAPI_IOCTYPE_FE, 162, int)
#define PAPI_IO_FE_CHDEC_SETCONSTELLATION		        _IOW(PAPI_IOCTYPE_FE, 163, int)
#define PAPI_IO_FE_CHDEC_GETACTUALCONSTELLATION         _IOW(PAPI_IOCTYPE_FE, 164, int)
#define PAPI_IO_FE_CHDEC_GETCONSTELLATION               _IOW(PAPI_IOCTYPE_FE, 165, int)
#define PAPI_IO_FE_CHDEC_SETIQMODE				        _IOW(PAPI_IOCTYPE_FE, 166, int)
#define PAPI_IO_FE_CHDEC_GETSUPPIQMODES			        _IOW(PAPI_IOCTYPE_FE, 167, int)
#define PAPI_IO_FE_CHDEC_GETIQMODE				        _IOW(PAPI_IOCTYPE_FE, 168, int)
#define PAPI_IO_FE_CHDEC_GETACTUALIQMODE		        _IOW(PAPI_IOCTYPE_FE, 169, int)
#define PAPI_IO_FE_CHDEC_SETBERTHRESHOLD		        _IOW(PAPI_IOCTYPE_FE, 170, int)
#define PAPI_IO_FE_CHDEC_GETBERTHRESHOLDRANGE	        _IOW(PAPI_IOCTYPE_FE, 171, int)
#define PAPI_IO_FE_CHDEC_GETBER					        _IOW(PAPI_IOCTYPE_FE, 172, int)
#define PAPI_IO_FE_CHDEC_GETBERTHRESHOLD		        _IOW(PAPI_IOCTYPE_FE, 173, int)
#define PAPI_IO_FE_CHDEC_GETTIMEERROR                   _IOW(PAPI_IOCTYPE_FE, 174, int)
#define PAPI_IO_FE_CHDEC_SETTIMEERROR                   _IOW(PAPI_IOCTYPE_FE, 175, int)
#define PAPI_IO_FE_CHDEC_GETTIMEERRORSUPP		        _IOW(PAPI_IOCTYPE_FE, 176, int)
#define PAPI_IO_FE_CHDEC_SETFREQERROR                   _IOW(PAPI_IOCTYPE_FE, 177, int)
#define PAPI_IO_FE_CHDEC_GETFREQERRORSUPP		        _IOW(PAPI_IOCTYPE_FE, 178, int)
#define PAPI_IO_FE_CHDEC_GETFREQERROR                   _IOW(PAPI_IOCTYPE_FE, 179, int)
#define PAPI_IO_FE_CHDEC_SETSYMBOLERROR                 _IOW(PAPI_IOCTYPE_FE, 180, int)
#define PAPI_IO_FE_CHDEC_GETSYMBOLERROR                 _IOW(PAPI_IOCTYPE_FE, 181, int)
#define PAPI_IO_FE_CHDEC_GETSYMBOLERRORSUPP		        _IOW(PAPI_IOCTYPE_FE, 182, int)
*/

// : 200
//! DVBT
/*
#define PAPI_IO_FE_CHDECDVBT_GETSUPPCODERATES           _IOW(PAPI_IOCTYPE_FE, 200, int)
#define PAPI_IO_FE_CHDECDVBT_GETSUPPHierarchyModes      _IOW(PAPI_IOCTYPE_FE, 201, int)
#define PAPI_IO_FE_CHDECDVBT_GetActualHierarchyMode     _IOW(PAPI_IOCTYPE_FE, 202, int)
#define PAPI_IO_FE_CHDECDVBT_GetHierarchyMode		    _IOW(PAPI_IOCTYPE_FE, 203, int)
#define PAPI_IO_FE_CHDECDVBT_SETHIERARCHYMODE		    _IOW(PAPI_IOCTYPE_FE, 204, int)
#define PAPI_IO_FE_CHDECDVBT_GETGUARDINTERVAl		    _IOW(PAPI_IOCTYPE_FE, 205, int)
#define PAPI_IO_FE_CHDECDVBT_GETACTUALGUARDINTERVAL     _IOW(PAPI_IOCTYPE_FE, 206, int)
#define PAPI_IO_FE_CHDECDVBT_GETSUPPGUARDINTERVALS      _IOW(PAPI_IOCTYPE_FE, 207, int)
#define PAPI_IO_FE_CHDECDVBT_SETGUARDINTERVAL           _IOW(PAPI_IOCTYPE_FE, 208, int)
#define PAPI_IO_FE_CHDECDVBT_GETSUPPTXMODES             _IOW(PAPI_IOCTYPE_FE, 209, int)
#define PAPI_IO_FE_CHDECDVBT_GETTXMODE                  _IOW(PAPI_IOCTYPE_FE, 210, int)
#define PAPI_IO_FE_CHDECDVBT_SETTXMODE                  _IOW(PAPI_IOCTYPE_FE, 211, int)
#define PAPI_IO_FE_CHDECDVBT_GETACTUALTXMODE            _IOW(PAPI_IOCTYPE_FE, 212, int)
#define PAPI_IO_FE_CHDECDVBT_GETSUPPCHANBANDWIDTHS      _IOW(PAPI_IOCTYPE_FE, 213, int)
#define PAPI_IO_FE_CHDECDVBT_GETCHANBANDWIDTH           _IOW(PAPI_IOCTYPE_FE, 214, int)
#define PAPI_IO_FE_CHDECDVBT_GETACTUALCHANBANDWIDTH     _IOW(PAPI_IOCTYPE_FE, 215, int)
#define PAPI_IO_FE_CHDECDVBT_GETTPSCELLID               _IOW(PAPI_IOCTYPE_FE, 216, int)
#define PAPI_IO_FE_CHDECDVBT_SETSTREAMPRIORITY          _IOW(PAPI_IOCTYPE_FE, 217, int)
#define PAPI_IO_FE_CHDECDVBT_GETSTREAMPRIORITY          _IOW(PAPI_IOCTYPE_FE, 218, int)
#define PAPI_IO_FE_CHDECDVBT_SETCHANNELINFORMATION      _IOW(PAPI_IOCTYPE_FE, 219, int)
#define PAPI_IO_FE_CHDECDVBT_GETCHANNELINFORMATION      _IOW(PAPI_IOCTYPE_FE, 220, int)
#define PAPI_IO_FE_CHDECDVBT_GETSUPPFREQUENCYOFFSETS    _IOW(PAPI_IOCTYPE_FE, 221, int)
#define PAPI_IO_FE_CHDECDVBT_GETACTUALFREQUENCYOFFSET   _IOW(PAPI_IOCTYPE_FE, 222, int)
#define PAPI_IO_FE_CHDECDVBT_SETFREQUENCYOFFSET         _IOW(PAPI_IOCTYPE_FE, 223, int)
#define PAPI_IO_FE_CHDECDVBT_GETFREQUENCYOFFSET         _IOW(PAPI_IOCTYPE_FE, 224, int)
#define PAPI_IO_FE_CHDECDVBT_SETCODERATES               _IOW(PAPI_IOCTYPE_FE, 225, int)
#define PAPI_IO_FE_CHDECDVBT_GETACTUALCODERATES         _IOW(PAPI_IOCTYPE_FE, 226, int)
#define PAPI_IO_FE_CHDECDVBT_GETCODERATES               _IOW(PAPI_IOCTYPE_FE, 227, int)
*/
//! ISDBT
#define PAPI_IO_FE_CHDECISDBT_GETCHANNELINFORMATION     _IOW(PAPI_IOCTYPE_FE, 200, int)
#define PAPI_IO_FE_CHDECISDBT_SETCHANNELINFORMATION     _IOW(PAPI_IOCTYPE_FE, 201, int)
#define PAPI_IO_FE_CHDECISDBT_GETACTUALCODERATES        _IOW(PAPI_IOCTYPE_FE, 202, int)
#define PAPI_IO_FE_CHDECISDBT_GETSUPPCODERATES          _IOW(PAPI_IOCTYPE_FE, 203, int)
#define PAPI_IO_FE_CHDECISDBT_GETCODERATES              _IOW(PAPI_IOCTYPE_FE, 204, int)
#define PAPI_IO_FE_CHDECISDBT_SETCODERATES              _IOW(PAPI_IOCTYPE_FE, 205, int)
#define PAPI_IO_FE_CHDECISDBT_GETACTUALCONSTELLATION    _IOW(PAPI_IOCTYPE_FE, 206, int)
#define PAPI_IO_FE_CHDECISDBT_GETCONSTELLATION          _IOW(PAPI_IOCTYPE_FE, 207, int)
#define PAPI_IO_FE_CHDECISDBT_SETCONSTELLATION          _IOW(PAPI_IOCTYPE_FE, 208, int)
#define PAPI_IO_FE_CHDECISDBT_GETSUPPCONSTELLATIONS     _IOW(PAPI_IOCTYPE_FE, 209, int)
#define PAPI_IO_FE_CHDECISDBT_GETACTUALGUARDINTERVAL    _IOW(PAPI_IOCTYPE_FE, 210, int)
#define PAPI_IO_FE_CHDECISDBT_GETGUARDINTERVAL          _IOW(PAPI_IOCTYPE_FE, 211, int)
#define PAPI_IO_FE_CHDECISDBT_SETGUARDINTERVAL          _IOW(PAPI_IOCTYPE_FE, 212, int)
#define PAPI_IO_FE_CHDECISDBT_GETSUPPGUARDINTERVALS     _IOW(PAPI_IOCTYPE_FE, 213, int)
#define PAPI_IO_FE_CHDECISDBT_GETLAYER                  _IOW(PAPI_IOCTYPE_FE, 214, int)
#define PAPI_IO_FE_CHDECISDBT_SETLAYER                  _IOW(PAPI_IOCTYPE_FE, 215, int)
#define PAPI_IO_FE_CHDECISDBT_GETSUPPORTEDLAYERS        _IOW(PAPI_IOCTYPE_FE, 216, int)

// Measurement: 230
#define PAPI_IO_FE_QSSM_GETMEASVALID			        _IOW(PAPI_IOCTYPE_FE, 230, int)
#define PAPI_IO_FE_QSSM_GETSIGSTRENGTHRANGE		        _IOW(PAPI_IOCTYPE_FE, 231, int)
#define PAPI_IO_FE_QSSM_STOP					        _IOW(PAPI_IOCTYPE_FE, 232, int)
#define PAPI_IO_FE_QSSM_GETSIGSTRENGTH			        _IOW(PAPI_IOCTYPE_FE, 233, int)
#define PAPI_IO_FE_QSSM_START					        _IOW(PAPI_IOCTYPE_FE, 234, int)

// Sound : 250
#define PAPI_IO_FE_SOUND_GETSNDSYS                      _IOW(PAPI_IOCTYPE_FE, 250, int)
#define PAPI_IO_FE_SOUND_H_GETSNDSYS              _IOW(PAPI_IOCTYPE_FE, 251, int)
#define PAPI_IO_FE_SOUND_GET_ALLOW_SNDSYS       _IOW(PAPI_IOCTYPE_FE, 252, int)
#define PAPI_IO_FE_SOUND_ALLOW_SNDSYS           _IOW(PAPI_IOCTYPE_FE, 253, int)


// vipN of type IPlfApiVideoPropertiesNotify
#define PAPI_CB_FE_VIP_ONVIDEOPRESENTCHANGED          0
#define PAPI_CB_FE_VIP_ONSIGNALPRESENTCHANGED         1
#define PAPI_CB_FE_VIP_ONFIELDFREQUENCYCHANGED        2
#define PAPI_CB_FE_VIP_ONNUMBEROFVISIBLELINESCHANGED  3
#define PAPI_CB_FE_VIP_ONNUMBEROFVISIBLEPIXELSCHANGED 4
#define PAPI_CB_FE_VIP_ONNUMBEROFFRAMELINESCHANGED    5
#define PAPI_CB_FE_VIP_ONINTERLACEDCHANGED            6
#define PAPI_CB_FE_VIP_ONIMAGEFORMATCHANGED           7
#define PAPI_CB_FE_VIP_ONFILMMODECHANGED              8
#define PAPI_CB_FE_VIP_ONFROMVCRCHANGED               9
#define PAPI_CB_FE_VIP_ONVIDEOCODINGCHANGED           10
#define PAPI_CB_FE_VIP_ONVIDEOPROTECTIONCHANGED       11

// colN of type IPlfApiVideoPropertiesNotify
#define PAPI_CB_FE_COL_ONCOLORSYSTEMCHANGED	      0
#define PAPI_CB_FE_COL_ONCOMBFILTERACTIVECHANGED      1

#endif  // PAPI_IOCTL_FE_H

