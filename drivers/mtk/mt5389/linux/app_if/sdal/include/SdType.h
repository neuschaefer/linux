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
/**
 * @file SdType.h
 * @brief Samsung DAL API type definition header file
 * @author S/W Platform Lab
 * @date 2006/07/07
 */

#ifndef	_SDTYPE_H_
#define	_SDTYPE_H_

/********************************************************************
	INCLUDE FILES
********************************************************************/
#if 0

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>

#else

#include "x_typedef.h"

#ifdef __KERNEL__
#include <linux/types.h>
#endif

/* this is for linux kernel */
#define printf(fmt...)  printk(fmt)

#endif

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/
#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef NULL
#define	NULL	0
#endif

#define SD_INVALID	(-1)
#define SD_UINT32_INVALID (0xFFFFFFFF)
#define SD_DEBUG_INVALID_CMD		0xFF

extern unsigned long gSdPrintMask;
extern unsigned long gSdTraceMask;

/********************************************************************
	MACRO CONSTANT DEFINITIONS
********************************************************************/

#define	SDAUXOUT_PRINT_MASK			(1<<0)
#define	SDAVDEC_PRINT_MASK			(1<<1)
#define	SDBDCONFIG_PRINT_MASK		(1<<2)
#define	SDCOMMON_PRINT_MASK			(1<<3)
#define	SDCP_PRINT_MASK				(1<<4)
#define	SDDISPLAY_PRINT_MASK		(1<<5)
#define	SDEXTIN_PRINT_MASK			(1<<6)
#define	SDFACTORY_PRINT_MASK		(1<<7)
#define	SDNVRAM_PRINT_MASK			(1<<8)
#define	SDPOD_PRINT_MASK			(1<<9)
#define	SDSOUND_PRINT_MASK			(1<<10)
#define	SDTSDATA_PRINT_MASK			(1<<11)
#define	SDTUNER_PRINT_MASK			(1<<12)
#define	SDUSERDATA_PRINT_MASK		(1<<13)
#define	SDVIDEO_PRINT_MASK			(1<<14)
#define SDFEEDER_PRINT_MASK			(1<<15)
#define	SDIMAGE_PRINT_MASK			(1<<16)
#define	SDAVENC_PRINT_MASK			(1<<17)
#define	SDMCARD_PRINT_MASK			(1<<18)
#define	SDMICOM_PRINT_MASK			(1<<19)
#define SDMISC_PRINT_MASK			(1<<20)
#define	SDPVR_PRINT_MASK			(1<<21)
#define	SDCI_PRINT_MASK				(1<<22)
#define	SDSCART_PRINT_MASK			(1<<23)
#define	SDTTX_PRINT_MASK			(1<<24)

#define IS_DEBUG_SDAUXOUT()			(gSdPrintMask & SDAUXOUT_PRINT_MASK)
#define IS_DEBUG_SDAVDEC()			(gSdPrintMask & SDAVDEC_PRINT_MASK)
#define IS_DEBUG_SDBDCONFIG()		(gSdPrintMask & SDBDCONFIG_PRINT_MASK)
#define IS_DEBUG_SDCOMMON()			(gSdPrintMask & SDCOMMON_PRINT_MASK)
#define IS_DEBUG_SDCP()				(gSdPrintMask & SDCP_PRINT_MASK)
#define IS_DEBUG_SDDISPLAY()		(gSdPrintMask & SDDISPLAY_PRINT_MASK)
#define IS_DEBUG_SDEXTIN()			(gSdPrintMask & SDEXTIN_PRINT_MASK)
#define IS_DEBUG_SDFACTORY()		(gSdPrintMask & SDFACTORY_PRINT_MASK)
#define IS_DEBUG_SDNVRAM()			(gSdPrintMask & SDNVRAM_PRINT_MASK)
#define IS_DEBUG_SDPOD()			(gSdPrintMask & SDPOD_PRINT_MASK)
#define IS_DEBUG_SDSOUND()			(gSdPrintMask & SDSOUND_PRINT_MASK)
#define IS_DEBUG_SDTSDATA()			(gSdPrintMask & SDTSDATA_PRINT_MASK)
#define IS_DEBUG_SDTUNER()			(gSdPrintMask & SDTUNER_PRINT_MASK)
#define IS_DEBUG_SDUSERDATA()		(gSdPrintMask & SDUSERDATA_PRINT_MASK)
#define IS_DEBUG_SDVIDEO()			(gSdPrintMask & SDVIDEO_PRINT_MASK)
#define IS_DEBUG_SDFEEDER()			(gSdPrintMask & SDFEEDER_PRINT_MASK)
#define IS_DEBUG_SDIMAGE()			(gSdPrintMask & SDIMAGE_PRINT_MASK)
#define IS_DEBUG_SDAVENC()			(gSdPrintMask & SDAVENC_PRINT_MASK)
#define IS_DEBUG_SDMCARD()			(gSdPrintMask & SDMCARD_PRINT_MASK)
#define IS_DEBUG_SDMICOM()			(gSdPrintMask & SDMICOM_PRINT_MASK)
#define IS_DEBUG_SDMISC()			(gSdPrintMask & SDMISC_PRINT_MASK)
#define IS_DEBUG_SDPVR()			(gSdPrintMask & SDPVR_PRINT_MASK)
#define IS_DEBUG_SDCI()				(gSdPrintMask & SDCI_PRINT_MASK)
#define IS_DEBUG_SDSCART()			(gSdPrintMask & SDSCART_PRINT_MASK)
#define IS_DEBUG_SDTTX()			(gSdPrintMask & SDTTX_PRINT_MASK)

#define IS_TRACE_SDAUXOUT()			(gSdTraceMask & SDAUXOUT_PRINT_MASK)
#define IS_TRACE_SDAVDEC()			(gSdTraceMask & SDAVDEC_PRINT_MASK)
#define IS_TRACE_SDBDCONFIG()		(gSdTraceMask & SDBDCONFIG_PRINT_MASK)
#define IS_TRACE_SDCOMMON()			(gSdTraceMask & SDCOMMON_PRINT_MASK)
#define IS_TRACE_SDCP()				(gSdTraceMask & SDCP_PRINT_MASK)
#define IS_TRACE_SDDISPLAY()		(gSdTraceMask & SDDISPLAY_PRINT_MASK)
#define IS_TRACE_SDEXTIN()			(gSdTraceMask & SDEXTIN_PRINT_MASK)
#define IS_TRACE_SDFACTORY()		(gSdTraceMask & SDFACTORY_PRINT_MASK)
#define IS_TRACE_SDNVRAM()			(gSdTraceMask & SDNVRAM_PRINT_MASK)
#define IS_TRACE_SDPOD()			(gSdTraceMask & SDPOD_PRINT_MASK)
#define IS_TRACE_SDSOUND()			(gSdTraceMask & SDSOUND_PRINT_MASK)
#define IS_TRACE_SDTSDATA()			(gSdTraceMask & SDTSDATA_PRINT_MASK)
#define IS_TRACE_SDTUNER()			(gSdTraceMask & SDTUNER_PRINT_MASK)
#define IS_TRACE_SDUSERDATA()		(gSdTraceMask & SDUSERDATA_PRINT_MASK)
#define IS_TRACE_SDVIDEO()			(gSdTraceMask & SDVIDEO_PRINT_MASK)
#define IS_TRACE_SDFEEDER()			(gSdTraceMask & SDFEEDER_PRINT_MASK)
#define IS_TRACE_SDIMAGE()			(gSdTraceMask & SDIMAGE_PRINT_MASK)
#define IS_TRACE_SDAVENC()			(gSdTraceMask & SDAVENC_PRINT_MASK)
#define IS_TRACE_SDMCARD()			(gSdTraceMask & SDMCARD_PRINT_MASK)
#define IS_TRACE_SDMICOM()			(gSdTraceMask & SDMICOM_PRINT_MASK)
#define IS_TRACE_SDMISC()			(gSdTraceMask & SDMISC_PRINT_MASK)
#define IS_TRACE_SDPVR()			(gSdTraceMask & SDPVR_PRINT_MASK)
#define IS_TRACE_SDCI()				(gSdTraceMask & SDCI_PRINT_MASK)
#define IS_TRACE_SDSCART()			(gSdTraceMask & SDSCART_PRINT_MASK)
#define IS_TRACE_SDTTX()			(gSdTraceMask & SDTTX_PRINT_MASK)


/********************************************************************
	MACRO FUNCTION DEFINITIONS
********************************************************************/
#define SDAUXOUT_PRINT(fmt...) 		do {\
										if (IS_DEBUG_SDAUXOUT() )	\
										{printf("[SdAUXOUT]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDAVDEC_PRINT(fmt...) 		do {\
										if(IS_DEBUG_SDAVDEC() )	\
										{printf("[SdAVDEC]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDBDCONFIG_PRINT(fmt...) 	do {\
										if(IS_DEBUG_SDBDCONFIG() )	\
										{printf("[SdBDCONFIG]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDCOMMON_PRINT(fmt...) 		do {\
										if(IS_DEBUG_SDCOMMON() )	\
										{printf("[SdCOMMON]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDCP_PRINT(fmt...) 			do {\
										if(IS_DEBUG_SDCP() )	\
										{printf("[SdCP]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDDISPLAY_PRINT(fmt...) 	do {\
										if(IS_DEBUG_SDDISPLAY() )	\
										{printf("[SdDISPLAY]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDEXTIN_PRINT(fmt...) 		do {\
										if(IS_DEBUG_SDEXTIN() )	\
										{printf("[SdEXTIN]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDFACTORY_PRINT(fmt...) 	do {\
										if(IS_DEBUG_SDFACTORY() )	\
										{printf("[SdFACTORY]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDNVRAM_PRINT(fmt...) 		do {\
										if(IS_DEBUG_SDNVRAM())	\
										{printf("[SdNVRAM]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)
							
#define SDPOD_PRINT(fmt...) 		do {\
										if(IS_DEBUG_SDPOD())	\
										{printf("[SdPOD]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDSOUND_PRINT(fmt...) 		do {\
										if(IS_DEBUG_SDSOUND())	\
										{printf("[SdSOUND]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDTSDATA_PRINT(fmt...) 		do {\
										if(IS_DEBUG_SDTSDATA())	\
										{printf("[SdTSDATA]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDTUNER_PRINT(fmt...)	 	do {\
										if(IS_DEBUG_SDTUNER())	\
										{printf("[SdTUNER]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDUSERDATA_PRINT(fmt...) 	do {\
										if(IS_DEBUG_SDUSERDATA())	\
										{printf("[SdUSERDATA]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)							

#define SDVIDEO_PRINT(fmt...) 		do {\
										if(IS_DEBUG_SDVIDEO())	\
										{printf("[SdVIDEO]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)	

#define SDFEEDER_PRINT(fmt...) 		do {\
										if(IS_DEBUG_SDFEEDER())	\
										{printf("[SdFEEDER]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)	
									
#define SDIMAGE_PRINT(fmt...) 		do {\
										if(IS_DEBUG_SDIMAGE())	\
										{printf("[SdIMAGE]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)	

#define SDAVENC_PRINT(fmt...)	 	do {\
										if(IS_DEBUG_SDAVENC())	\
										{printf("[SdAVENC]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDMCARD_PRINT(fmt...) 	do {\
										if(IS_DEBUG_SDMCARD())	\
										{printf("[SdMCARD]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)							

#define SDMICOM_PRINT(fmt...) 		do {\
										if(IS_DEBUG_SDMICOM())	\
										{printf("[SdMICOM]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)	

#define SDMISC_PRINT(fmt...) 		do {\
										if(IS_DEBUG_SDMISC())	\
										{printf("[SdMISC]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)	
									
#define SDPVR_PRINT(fmt...) 		do {\
										if(IS_DEBUG_SDPVR())	\
										{printf("[SdPVR]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)	
									
#define SDCI_PRINT(fmt...) 			do {\
										if(IS_DEBUG_SDCI())	\
										{printf("[SdCI]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)
									
#define SDSCART_PRINT(fmt...) 		do {\
										if(IS_DEBUG_SDSCART())	\
										{printf("[SdScart]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDTTX_PRINT(fmt...) 		do {\
										if(IS_DEBUG_SDTTX())	\
										{printf("[SdTTX]%s():", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)
									
#define SD_ERROR(fmt...)			do {\
										printf("[ERROR][%s():%d] ",__FUNCTION__, __LINE__);\
										printf(fmt);	\
									} while(0)								


// SDAL TRACE MACRO - It is only used for showing Function Calling Squence. Don't use this macro to print Debug messages.
#define SDAUXOUT_TRACE(fmt...) 		do {\
										if (IS_TRACE_SDAUXOUT() )	\
										{printf("\t---[SdAUXOUT] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDAVDEC_TRACE(fmt...) 		do {\
										if(IS_TRACE_SDAVDEC() )	\
										{printf("\t---[SdAVDEC] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDBDCONFIG_TRACE(fmt...) 	do {\
										if(IS_TRACE_SDBDCONFIG() )	\
										{printf("\t---[SdCONFIG] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDCOMMON_TRACE(fmt...) 		do {\
										if(IS_TRACE_SDCOMMON() )	\
										{printf("\t---[SdCOMMON] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDCP_TRACE(fmt...) 			do {\
										if(IS_TRACE_SDCP() )	\
										{printf("\t---[SdCP] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDDISPLAY_TRACE(fmt...) 	do {\
										if(IS_TRACE_SDDISPLAY() )	\
										{printf("\t---[SdDISPLAY] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDEXTIN_TRACE(fmt...) 		do {\
										if(IS_TRACE_SDEXTIN() )	\
										{printf("\t---[SdEXTIN] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDFACTORY_TRACE(fmt...) 	do {\
										if(IS_TRACE_SDFACTORY() )	\
										{printf("\t---[SdFACTORY] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDNVRAM_TRACE(fmt...) 		do {\
										if(IS_TRACE_SDNVRAM())	\
										{printf("\t---[SdNVRAM] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)
							
#define SDPOD_TRACE(fmt...) 		do {\
										if(IS_TRACE_SDPOD())	\
										{printf("\t---[SdPOD] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDSOUND_TRACE(fmt...) 		do {\
										if(IS_TRACE_SDSOUND())	\
										{printf("\t---[SdSOUND] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDTSDATA_TRACE(fmt...) 		do {\
										if(IS_TRACE_SDTSDATA())	\
										{printf("\t---[SdTSDATA] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDTUNER_TRACE(fmt...)	 	do {\
										if(IS_TRACE_SDTUNER())	\
										{printf("\t---[SdTUNER] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDUSERDATA_TRACE(fmt...) 	do {\
										if(IS_TRACE_SDUSERDATA())	\
										{printf("\t---[SdUSERDATA] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)							

#define SDVIDEO_TRACE(fmt...) 		do {\
										if(IS_TRACE_SDVIDEO())	\
										{printf("\t---[SdVIDEO] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)										

#define SDFEEDER_TRACE(fmt...) 		do {\
										if(IS_TRACE_SDFEEDER())	\
										{printf("\t---[SdFEEDER] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)										

#define SDIMAGE_TRACE(fmt...) 		do {\
										if(IS_TRACE_SDIMAGE())	\
										{printf("\t---[SdIMAGE] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)										

#define SDAVENC_TRACE(fmt...)	 	do {\
										if(IS_TRACE_SDAVENC())	\
										{printf("\t---[SdAVENC] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDMCARD_TRACE(fmt...) 	do {\
										if(IS_TRACE_SDMCARD())	\
										{printf("\t---[SdMCARD] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)							

#define SDMICOM_TRACE(fmt...) 		do {\
										if(IS_TRACE_SDMICOM())	\
										{printf("\t---[SdMICOM] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)	

#define SDMISC_TRACE(fmt...) 		do {\
										if(IS_TRACE_SDMISC())	\
										{printf("\t---[SdMISC] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)	
									
#define SDPVR_TRACE(fmt...) 		do {\
										if(IS_TRACE_SDPVR())	\
										{printf("\t---[SdPVR] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)	

#define SDCI_TRACE(fmt...) 			do {\
										if(IS_TRACE_SDCI())	\
										{printf("\t---[SdCI] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)	
									
#define SDSCART_TRACE(fmt...) 		do {\
										if(IS_TRACE_SDSCART())	\
										{printf("\t---[SdScart] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define SDTTX_TRACE(fmt...) 		do {\
										if(IS_TRACE_SDTTX())	\
										{printf("\t---[SdTTX] :%s() ", __FUNCTION__);	\
										printf(fmt);}	\
									} while (0)

#define	IS_SOURCE_NONE(src)		(src == SD_SOURCE_NONE)
#define	IS_SOURCE_ATV(src)		((src&SOURCE_TYPE_MASK)==SD_SOURCE_ATV)
#define	IS_SOURCE_DTV(src)		((src&SOURCE_TYPE_MASK)==SD_SOURCE_DTV)
#define	IS_SOURCE_RF(src)		(IS_SOURCE_ATV(src) || IS_SOURCE_DTV(src))
#define	IS_SOURCE_AV(src)		((src&SOURCE_TYPE_MASK)==SD_SOURCE_AV)
#define	IS_SOURCE_SVIDEO(src)	((src&SOURCE_TYPE_MASK)==SD_SOURCE_SVIDEO)
#define	IS_SOURCE_COMP(src)		((src&SOURCE_TYPE_MASK)==SD_SOURCE_COMP)
#define	IS_SOURCE_VGA(src)		((src&SOURCE_TYPE_MASK)==SD_SOURCE_PC)
#define	IS_SOURCE_DVI(src)		((src&SOURCE_TYPE_MASK)==SD_SOURCE_DVI)		
#define	IS_SOURCE_HDMI(src)		((src&SOURCE_TYPE_MASK)==SD_SOURCE_HDMI)
#define	IS_SOURCE_DVR(src)		((src&SOURCE_TYPE_MASK)==SD_SOURCE_DVR)
#define	IS_SOURCE_DLINK(src)	((src&SOURCE_TYPE_MASK)==SD_SOURCE_DLINK)
#define	IS_SOURCE_PCM(src)		((src&SOURCE_TYPE_MASK)==SD_SOURCE_PCM)
#define	IS_SOURCE_MEDIA(src)	((src&SOURCE_TYPE_MASK)==SD_SOURCE_MEDIA)
#define IS_SOURCE_SCART(src)	((src&SOURCE_TYPE_MASK)==SD_SOURCE_SCART)
#define IS_SOURCE_OLINK(src)	((src&SOURCE_TYPE_MASK)==SD_SOURCE_OLINK)

#define	IS_SOURCE_EXT(src)	(IS_SOURCE_COMP(src) || \
				 IS_SOURCE_AV(src) ||	\
				 IS_SOURCE_SVIDEO(src) ||	\
				 IS_SOURCE_SCART(src))

#define	IS_SOURCE_DIGITAL(src)	(IS_SOURCE_DTV(src) || \
				 IS_SOURCE_VGA(src) ||	\
				 IS_SOURCE_DVR(src) ||	\
				 IS_SOURCE_DVI(src) ||	\
				 IS_SOURCE_HDMI(src) ||	\
				 IS_SOURCE_DLINK(src))

#define	IS_SOURCE_TS(src)	(IS_SOURCE_DTV(src) ||	\
				IS_SOURCE_DVR(src) ||	\
				IS_SOURCE_DLINK(src) ||	\
				IS_SOURCE_PCM(src))

#define	IS_SOURCE_COMB(src) (IS_SOURCE_ATV(src) || \
				IS_SOURCE_AV(src) || \
				IS_SOURCE_SVIDEO(src) ||	\
				IS_SOURCE_SCART(src))
				
/********************************************************************
	TYPE DEFINITION
********************************************************************/
#define rint32_t 		register int
#define ruint32_t 		register unsigned int

typedef int 			SdBool_t;
typedef unsigned long	SdMask_t;
typedef void*			SdAVDec_DmxHandle;

typedef float			SdFloat32_t;
typedef double			SdFloat64_t;

typedef enum
{
	SD_OK 						= 0
	,SD_NOT_OK					= -1
	,SD_NOT_SUPPORTED 			= -2
	,SD_PARAMETER_ERROR			= -3
	,SD_DONT_CARE				= -4
	,SD_ERR_SYS					= -5		///< operating system call failure 
	,SD_ERR_INV					= -6		///< invalid argument passed 
	,SD_ERR_TIMEDOUT			= -7		///< timeout occured 
	,SD_ERR_DEFERRED			= -8		///< action has been deferred 
	,SD_ERR_UNIMPLEM			= -9		///< Unimplemented service 
	,SD_ERR_LOCKED				= -10		///< object locked 
	,SD_ERR_DELETED				= -11		///< Object has been deleted 
	,SD_ERR_HEAP				= -12		///< Heap full 
	,SD_ERR_UNINIT				= -13		///< module is not initialised 
	,SD_ERR_UNIQUE				= -14		///< unique id needed 
	,SD_ERR_STATE				= -15		///< invalid state to invoke function 
	,SD_ERR_DEV					= -16		///< no valid device exists 
	,SD_ERR_NOT_FOUND			= -17		///< Not found 
	,SD_ERR_EOL					= -18		///< a list traversal has ended 
	,SD_ERR_TERM				= -19		///< module has terminated 
	,SD_ERR_LIMIT				= -20		///< System limit was reached. 
	,SD_ERR_RESOURCE			= -21		///< insufficient resources for request 
	,SD_ERR_NOT_AVAIL			= -22		///< resources not available 
	,SD_ERR_BAD_DATA			= -23		///< Bad data in transport stream 
	,SD_ERR_INLINE_SUCCESS		= -24
	,SD_ERR_DEFERRED_SUCCESS	= -25
	,SD_ERR_IN_USE				= -26		///< Object is in use. 
	,SD_ERR_INTERMEDIATE		= -27		///< Intermediate return - successful but not complete 
	,SD_ERR_OVERRUN				= -28		///< Buffer overrun occured (e.g. in filters). 
	,SD_ERR_ABORT				= -29		///< Request not supported. 
	,SD_ERR_NOTIMPLEMENTED		= -30		///< Request not implemented. 
	,SD_ERR_INVALID_HANDLE		= -31
	,SD_ERR_NOMEM				= -32		///< No Memory 
 	,SD_ERR_USER 				= -99		///< start of subsystem specific errors 
} SdResult; ///< SDAL Error code

typedef enum
{
	SD_TUNER0,
	SD_TUNER1,
	SD_TUNER_SUB, ///< for MCard
	SD_TUNER_MAX
} SdTunerID_k;

typedef enum
{ 
	SD_AVDEC0,
	SD_AVDEC1,
	SD_AVDEC_MAX
} SdAVDecID_k;

typedef enum
{
	SD_EXTIN0,
	SD_EXTIN1,
	SD_EXTIN2,
	SD_EXTIN_MAX
} SdExtInID_k;

typedef enum
{
	SD_BE_IN_AVDEC0,
	SD_BE_IN_AVDEC1,
	SD_BE_IN_EXTIN0,
	SD_BE_IN_EXTIN1,
	SD_BE_IN_IMAGE,	
	SD_BE_IN_MAX	
} SdBackEndInID_k;

typedef enum
{
	SD_VIDEO0, 
	SD_VIDEO1,
	SD_VIDEO_MAX
} SdVideoID_k;

typedef enum
{
	SD_SOUND0,
	SD_SOUND1,
	SD_SOUND_MAX
} SdSoundID_k;

typedef enum
{
	SD_AUXOUT0,
	SD_AUXOUT1,
	SD_AUXOUT_MAX
} SdAuxOutID_k;

typedef enum
{
	SD_AUXOUT_480i,
	SD_AUXOUT_576i,
	SD_AUXOUTFMT_MAX
} SdAuxOutFmt_k;

typedef enum
{
	SD_WINDOW_MAIN,		///< MainDisplay 
	SD_WINDOW_SUB0,		///< SubDisplay 
	SD_STREAM_TSOUT,	///< TSOut (IEEE 1394)	
	SD_STREAM_RECORD0,	///< PVR or TimeShift
	SD_STREAM_RECORD1,	///< PVR or TimeShift
	SD_WINDOW_MAX,
} SdWindowID_k;

typedef enum
{
	SD_SPEAKER_MAIN,
	SD_SPEAKER_SUB0,
	SD_SPEAKER_SUB1,
	SD_SPEAKER_SUB2,
	SD_SPEAKER_MAX
} SdSpeakerID_k;

typedef enum
{
	SD_AVENC0,
	SD_AVENC1,
	SD_AVENC_MAX
} SdAVEncID_k;

typedef enum 
{
	SD_RECORD0,
	SD_RECORD1,
	SD_RECORD_MAX
} SdRecordID_k;

typedef enum 
{
	SD_PLAY0,
	SD_PLAY1,
	SD_PLAY_MAX
} SdPlayID_k;

typedef enum
{
    SD_SCART0,
    SD_SCART1,
    SD_SCART2,
    SD_SCART3,
    SD_SCART_MAX,
} SdScartID_k;

typedef enum
{
	SD_PWROFF_TO_NORMAL,
	SD_PWROFF_TO_STANDBY,
	SD_STANDBY_TO_NORMAL
} SdPowerState_k;

typedef enum
{
	SD_STREAM_PES, 	///< Packetized Elementary Stream
	SD_STREAM_ES, 	///< Elementary Stream
	SD_STREAM_MAX,
} SdStreamType_k;	///< Decorder Input Stream Type 

typedef enum
{
	SD_VIDEO_FORMAT_NONE,
	SD_VIDEO_FORMAT_MPEG1,
	SD_VIDEO_FORMAT_MPEG2,
	SD_VIDEO_FORMAT_MPEG4,
	SD_VIDEO_FORMAT_H264,
	SD_VIDEO_FORMAT_DIVX,
	SD_VIDEO_FORMAT_WMV3, ///< WMV Ver.9
	SD_VIDEO_FORMAT_XVID,
	SD_VIDEO_FORMAT_MJPEG,
	SD_VIDEO_FORMAT_MAX,
} SdVideoFormat_k; ///< Video Stream Format 

typedef enum
{
	SD_AUDIO_FORMAT_NONE,
	SD_AUDIO_FORMAT_PCM,
	SD_AUDIO_FORMAT_ADPCM,
	SD_AUDIO_FORMAT_MPEG1,
	SD_AUDIO_FORMAT_MPEG1_L3, //MP3
	SD_AUDIO_FORMAT_MPEG2,
	SD_AUDIO_FORMAT_AAC,
	SD_AUDIO_FORMAT_AC3,
	SD_AUDIO_FORMAT_HEAAC,
	SD_AUDIO_FORMAT_EAC3,
	SD_AUDIO_FORMAT_DTS,
	SD_AUDIO_FORMAT_VORBIS,
	SD_AUDIO_FORMAT_WMA,	
	SD_AUDIO_FORMAT_MAX,
} SdAudioFormat_k; ///< Audio Stream Format

typedef enum
{
	SD_COLOR_FORMAT_RGB,
	SD_COLOR_FORMAT_YCbCr420,
	SD_COLOR_FORMAT_YCbCr422,
	SD_COLOR_FORMAT_YCbCr444,
	SD_COLOR_FORMAT_XVYCC,
	SD_COLOR_FORMAT_YTBCTB, ///< Y top, bottom, C top, bottom
	SD_COLOR_FORMAT_UNKNOWN,
} SdColorFormat_k;

typedef enum
{
	SD_COLOR_DEPTH_8,
	SD_COLOR_DEPTH_10,
	SD_COLOR_DEPTH_12,
	SD_COLOR_DEPTH_16,
	SD_COLOR_DEPTH_MAX,
} SdColorDepth_k;

typedef enum
{
	SD_MATRIX_COEFF_BT709,
	SD_MATRIX_COEFF_BT470_2,	///< ITU-R BT.470-2 System B,G
	SD_MATRIX_COEFF_FCC,	
	SD_MATRIX_COEFF_SMPTE170M,	
	SD_MATRIX_COEFF_SMPTE240M,
	SD_MATRIX_COEFF_UNKNOWN,
	SD_MATRIX_COEFF_MAX,
} SdMatrixCoeff_k;

typedef enum 
{	
	SD_AUDIO_FS_NONE,
	SD_AUDIO_FS_8KHZ,
	SD_AUDIO_FS_11KHZ,
	SD_AUDIO_FS_12KHZ,
	SD_AUDIO_FS_16KHZ,
	SD_AUDIO_FS_22KHZ,
	SD_AUDIO_FS_24KHZ,
	SD_AUDIO_FS_32KHZ,
	SD_AUDIO_FS_44KHZ,
	SD_AUDIO_FS_48KHZ,	
	SD_AUDIO_FS_88KHZ,
	SD_AUDIO_FS_96KHZ,
	SD_AUDIO_FS_176KHZ,
	SD_AUDIO_FS_192KHZ,
	SD_AUDIO_FS_MAX,
} SdAudioFsRate_k;

typedef enum
{
    SD_AUDIO_PCM_NONE,
    SD_AUDIO_PCM_S16LE,
    SD_AUDIO_PCM_S16BE,
    SD_AUDIO_PCM_U16LE,
    SD_AUDIO_PCM_U16BE,
    SD_AUDIO_PCM_S8,
    SD_AUDIO_PCM_U8,
    SD_AUDIO_PCM_MULAW,
    SD_AUDIO_PCM_ALAW,
    SD_AUDIO_PCM_S32LE,
    SD_AUDIO_PCM_S32BE,
    SD_AUDIO_PCM_U32LE,
    SD_AUDIO_PCM_U32BE,
    SD_AUDIO_PCM_S24LE,
    SD_AUDIO_PCM_S24BE,
    SD_AUDIO_PCM_U24LE,
    SD_AUDIO_PCM_U24BE,
    SD_AUDIO_PCM_S24DAUD,           
} SdAudioPCMFormat_k; ///< PCM Audio SubType

typedef enum
{
    SD_AUDIO_ADPCM_NONE,
    SD_AUDIO_ADPCM_IMA_QT,
    SD_AUDIO_ADPCM_IMA_WAV,
    SD_AUDIO_ADPCM_IMA_DK3,
    SD_AUDIO_ADPCM_IMA_DK4,
    SD_AUDIO_ADPCM_IMA_WS,
    SD_AUDIO_ADPCM_IMA_SMJPEG,
    SD_AUDIO_ADPCM_MS,
    SD_AUDIO_ADPCM_4XM,
    SD_AUDIO_ADPCM_XA,
    SD_AUDIO_ADPCM_ADX,
    SD_AUDIO_ADPCM_EA,
    SD_AUDIO_ADPCM_G726,
    SD_AUDIO_ADPCM_CT,
    SD_AUDIO_ADPCM_SWF,
    SD_AUDIO_ADPCM_YAMAHA,
    SD_AUDIO_ADPCM_SBPRO_4,
    SD_AUDIO_ADPCM_SBPRO_3,
    SD_AUDIO_ADPCM_SBPRO_2,
    SD_AUDIO_ADPCM_MAX,
} SdAudioADPCMFormat_k; ///< ADPCM Audio Codec

#define SOURCE_TYPE_MASK  0xff00

typedef enum
{
	/* normal TV mode */
	SD_SOURCE_NONE	= 0x0000,	  

	// ATV
	SD_SOURCE_ATV	= 0x0100,
	SD_SOURCE_ATV1	= 0x0101,          //!< Use Analog TV of Main Tuner as Source Path.
	SD_SOURCE_ATV2	= 0x0102,          //!< Use Analog TV of PIP Tuner as Source Path.

	// DTV
	SD_SOURCE_DTV	= 0x0200,
	SD_SOURCE_DTV1	= 0x0201,          //!< Use Digital TV of Main Tuner as Source Path.
	SD_SOURCE_DTV2	= 0x0202,          //!< Use Digital TV of PIP Tuner as Source Path.

	// AV
	SD_SOURCE_AV		= 0x0300,
	SD_SOURCE_AV1		= 0x0301,         //!< Use AV1 as source.
	SD_SOURCE_AV2		= 0x0302,         //!< Use AV2 as source.
	SD_SOURCE_AV3		= 0x0303,         //!< Use AV3 as source.
	SD_SOURCE_AV4		= 0x0304,         //!< Use AV4 as source.

	//S-Video
	SD_SOURCE_SVIDEO	= 0x0400,
	SD_SOURCE_SVIDEO1	= 0x0401,         //!< Use SVIDEO1 as source.
	SD_SOURCE_SVIDEO2	= 0x0402,         //!< Use SVIDEO2 as source.
	SD_SOURCE_SVIDEO3	= 0x0403,         //!< Use SVIDE03 as source.
	SD_SOURCE_SVIDEO4	= 0x0404,         //!< Use SVIDE04 as source.

	//Component
	SD_SOURCE_COMP		= 0x0500,
	SD_SOURCE_COMP1		= 0x0501,         //!< Use COMP1 as source.
	SD_SOURCE_COMP2		= 0x0502,         //!< Use COMP2 as source.
	SD_SOURCE_COMP3		= 0x0503,         //!< Use COMP3 as source.
	SD_SOURCE_COMP4		= 0x0504,         //!< Use COMP4 as source.

	// PC
	SD_SOURCE_PC		= 0x0600,
	SD_SOURCE_PC1		= 0x0601,         //!< Use PC1 as source.
	SD_SOURCE_PC2		= 0x0602,         //!< Use PC2 as source.
	SD_SOURCE_PC3		= 0x0603,         //!< Use PC3 as source.
	SD_SOURCE_PC4		= 0x0604,         //!< Use PC4 as source.

	// DVI
	SD_SOURCE_DVI		= 0x0700,
	SD_SOURCE_DVI1		= 0x0701,         //!< Use DVI1 as source
	SD_SOURCE_DVI2		= 0x0702,         //!< Use DVI2 as source
	SD_SOURCE_DVI3		= 0x0703,         //!< Use DVI3 as source
	SD_SOURCE_DVI4		= 0x0704,         //!< Use DVI4 as source

	// HDMI
	SD_SOURCE_HDMI		= 0x0800,
	SD_SOURCE_HDMI1		= 0x0801,         //!< Use HDMI1 as source.
	SD_SOURCE_HDMI2		= 0x0802,         //!< Use HDMI2 as source.
	SD_SOURCE_HDMI3		= 0x0803,         //!< Use HDMI3 as source.
	SD_SOURCE_HDMI4		= 0x0804,         //!< Use HDMI4 as source.
	
	// Optical LINK
	SD_SOURCE_OLINK		= 0x0900,         //!< Use Optical Link as source.	

	// DNET, IEEE1394
	SD_SOURCE_DLINK		= 0x1000,         //!< Use DLINK as source.

	// PCM(Software Decoder)
	SD_SOURCE_PCM		= 0x1100,         //!< Use PCM as source.

	// WiseLink, PVR
	SD_SOURCE_MEDIA		= 0x1200,			//!< Media Source

	SD_SOURCE_USBUPGRADE = 0x1300,		//!< USB for S/W Upgrade 
	
	// DLNA
	SD_SOURCE_DLNA		= 0x1400,			//!< Use DLNA as source
	
	// SCART
    SD_SOURCE_SCART      = 0x1500,
    SD_SOURCE_SCART1     = 0x1501,         //!< Use SCART1 as source.
    SD_SOURCE_SCART2     = 0x1502,         //!< Use SCART2 as source.
    SD_SOURCE_SCART3     = 0x1503,         //!< Use SCART3 as source.
    SD_SOURCE_SCART4     = 0x1504,         //!< Use SCART4 as source.
    
    // DVR(HDD Play)
	SD_SOURCE_DVR		= 0x1600,         //!< Use DVR as source.

	SD_SOURCE_MAX		= 0xffff,         //!< Source MAX
} SdSource_k;

typedef struct
{
	uint32_t red;		///<Range:0~255	
	uint32_t green;		///<Range:0~255	
	uint32_t blue;		///<Range:0~255	
} SdColor_t;

typedef struct
{
	int32_t x;
	int32_t y;
	uint32_t w;
	uint32_t h;
} SdGeometry_t;

typedef struct
{	
	uint32_t w;
	uint32_t h;
} SdSize_t;

typedef struct
{
	uint32_t frequency;
	uint32_t phase;
	SdGeometry_t sPCPosition;
} SdPCAdjustInfo_t;

typedef struct
{
	uint32_t ohcw;
	uint32_t olcw;
	uint32_t ehcw;
	uint32_t elcw;
} SdCPKey_t;

typedef struct
{
	uint32_t	cmd;				///< user command value
	const char*		name;				///< command string
	void		(*dbg_func)(void);	///< command function
} SdDebugCmd_t;

/********************************************************************
	EXTERN VARIABLES & FUNCTION PROTOTYPES DECLARATIONS
********************************************************************/

#endif

