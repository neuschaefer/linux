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
#if ! defined(DRV_COMMON_H)
#define DRV_COMMON_H


/* diagnostic program */

#define MTK_CLIWATCHDOG_NAME    		"DogThrd"
#define MTK_CLIIR_NAME					"CliMtkIr"
#define MTK_CLIIRSPTN_NAME      		"IrSPtn"
#define MTK_CLIIRTX_NAME        		"CliIrTx"
#define MTK_CLIIR_STACK_SZ				4096
#define MTK_CLIIR_PRIORITY				100

#define MTK_NAND_NAME					"nandread"
#define MTK_NAND_STACK_SZ				4096
#define MTK_NAND_PRIORITY				100

#define MTK_NOR_NAME					"noread"
#define MTK_NOR_STACK_SZ				4096
#define MTK_NOR_PRIORITY				100

#define DHRYSTONE_THREAD_NAME			"Dhry"
#define DHRYSTONE_STACK_SIZE			8192

#define DIAG_GFX_PWCT_NAME				"gfxpwr"
#define DIAG_GFX_PWCT_STACK_SZ			2048
#define DIAG_GFX_PWCT_PRIORITY			250
                                		
#define D_NIM_MONITOR_THR_NAME			"tserr"
#define D_NIM_MONITOR_STACK_SZ			1024
#define D_NIM_MONITOR_PRIORITY			100

/* Log Print Thread */
#define LOG_PRINT_NAME              	"LogThread"
#define LOG_PRINT_STACK_SZ          	2048
#define LOG_PRINT_PRIORITY          	230

/* FPD MW Scan Thread */
#define FPD_MW_NAME                 	"FPDScan"
#define FPD_MW_STACK_SZ             	2048
#define FPD_MW_PRIORITY             	230

/* Demux thread */
#define DMX_THREAD_NAME                 "Demux"
#define DMX_THREAD_STACK_SIZE           2048
#define DMX_THREAD_PRIORITY             60

/* OsdDma thread */
#define OSD_DMA_THREAD_NAME             "OsdDma"
#define OSD_DMA_THREAD_STACK_SIZE       2048
#define OSD_DMA_THREAD_PRIORITY         100

/* testing code - should be remove from here */

/* demo navigator */

#define NAV_OSD_NAME					"NVGT_OSD"
#define NAV_OSD_STACK_SZ				2048
#define NAV_OSD_PRIORITY				100

#define NAV_1394_NAME					"NVGT_1394"
#define NAV_1394_STACK_SZ				10240
#define NAV_1394_PRIORITY				100

#define NAV_IR_NAME						"NVGT_IR"
#define NAV_IR_STACK_SZ					10240
#define NAV_IR_PRIORITY					100

#define NAV_PSI_NAME					"NVGT_PSI"
#define NAV_PSI_STACK_SZ				2048
#define NAV_PSI_PRIORITY				100

#define NAV_NIM_NAME					"NVGT_NIM"
#define NAV_NIM_STACK_SZ				2048
#define NAV_NIM_PRIORITY				250

#define NAV_REC_NAME					"NVGT_REC"
#define NAV_REC_STACK_SZ				2048
#define NAV_REC_PRIORITY				100

/* driver */
#define GPIO_POLL_THREAD_NAME       	"GPIOPoll"
#define GPIO_POLL_STACK_SIZE        	2048
#define GPIO_POLL_THREAD_PRIORITY   	100

#define IRRX_BTN_THREAD_NAME       	    "IRRXBtn"
#define IRRX_BTN_STACK_SIZE        	    2048
#define IRRX_BTN_THREAD_PRIORITY   	    59

#define IDE_CQ_THREAD_NAME				"CQThread"
#define IDE_CQ_STACK_SIZE				16384
#define IDE_CQ_THREAD_PRIORITY			100

#define FBM_VD_THREAD_NAME				"VD Thread"
#define FBM_VD_STACK_SIZE				2048
#define FBM_VD_THREAD_PRIORITY			98

#define FBM_VM_THREAD_NAME				"VM Thread"
#define FBM_VM_STACK_SIZE				2048
#define FBM_VM_THREAD_PRIORITY			99

#define LOG_THREAD_NAME					"PrintT"
#define LOG_STACK_SIZE					2048
#define LOG_THREAD_PRIORITY				230

#define MPV_THREAD_NAME					"MPV"
#define MPV_STACK_SIZE					2048
#define MPV_THREAD_PRIORITY				60

#define DV_THREAD_NAME					"DV"
#define DV_STACK_SIZE					2048
#define DV_THREAD_PRIORITY				60

#define FEEDER_THREAD_NAME              "FEEDER"
#define FEEDER_STACK_SIZE               2048
#define FEEDER_THREAD_PRIORITY          60

#define MPV_VPRS_THREAD_NAME			"VPrs"
#define MPV_VPRS_STACK_SIZE				2048
#define MPV_VPRS_THREAD_PRIORITY		100

#define ADSPTASK_NAME					"ADSPTask Thread"
#define ADSPTASK_STACK_SIZE				2048
#define ADSPTASK_THREAD_PRIORITY		100

#define AUD_DRV_THREAD_NAME				"AudDrvThread"
#define AUD_DRV_THREAD_STACK_SIZE		2048
#define AUD_DRV_THREAD_PRIORITY			100

#define PSR_THREAD_NAME					"Parser"
#define PSR_THREAD_STACK_SIZE			16384
#define PSR_THREAD_PRIORITY				90

#define IRRC_THREAD_NAME				"IRRC Thread"
#define IRRC_STACK_SIZE					1024
#define IRRC_THREAD_PRIORITY			59

#define STORG_FLASH_NAME            	"FlashIO"
#define STORG_IDE_NAME              	"IdeIO"
#define STORG_FCI_NAME              	"FciIO"
#define STORG_STACK_SIZE            	1024
#define STORG_THREAD_PRIORITY       	200

#define VIN_THREAD_NAME					"VIN"
#define VIN_STACK_SIZE					1024
#define VIN_THREAD_PRIORITY         	60

#define VSYNC_THREAD_NAME				"VSYNC"
#define VSYNC_STACK_SIZE				2048
#define VSYNC_THREAD_PRIORITY			50

#define CLI_THREAD_NAME					"CLI"
#define CLI_STACK_SIZE					16384
#define CLI_THREAD1_PRIORITY			100

#define DBS_THREAD_NAME					"DBS"
#define DBS_STACK_SIZE					1024
#define DBS_THREAD1_PRIORITY			LOG_PRINT_PRIORITY

/* 1394 section */
#define MTK_I1394_TASK_DEFINED
#define I1394_STACK_SIZE                8192
#define I1394_BUSRST_PRIORITY           50
#define I1394_THREAD_PRIORITY           100

/* field record function */
#define D_REC_FLOW_NAME					"REC_F"
#define D_REC_FLOW_STACK_SZ				8192
#define D_REC_FLOW_PRIO					100
#define D_REC_ELOG_NAME					"REC_E"
#define D_REC_ELOG_STACK_SZ				8192
#define D_REC_ELOG_PRIO					100

/* pvr playback */
#define PVR_PB_NAME						"PVR_PB"
#define PVR_PB_STACK_SZ					8192
#define PVR_PB_PRIORITY					100

#define SLT_ECHO_NAME					"SLT_ECHO"
#define SLT_ECHO_STACK_SZ				2048
#define SLT_ECHO_PRIORITY				100

/* Scpos */
#define SCPOS_THREAD_NAME				"Scpos"
#define SCPOS_STACK_SIZE				2048
#define SCPOS_THREAD_PRIORITY			60

/* MDDi */
#define DI_THREAD_NAME			  		"Di"
#define DI_STACK_SIZE			    	2048
#define DI_THREAD_PRIORITY				60

/* Main Loop Video */
#define MLVDO_THREAD_NAME				"MLVdo"
#define MLVDO_STACK_SIZE				2048
#define MLVDO_THREAD_PRIORITY			60

/* Software PS demux */
#define SWDMX_THREAD_NAME               "SwDmxThread"
#define SWDMX_THREAD_STACK_SIZE         4096
#define SWDMX_THREAD_PRIORITY           100

/* OS driver reaper thread */
#define REAPER_THREAD_NAME              "Reaper"
#define REAPER_STACK_SIZE               2048
#define REAPER_PRIORITY                 200

/* Ethernet thread */
#define ETHER_THREAD_NAME           "Ethernet Thread"
#define ETHER_STACK_SIZE            2048
#define ETHER_THREAD_PRIORITY       90


#endif //DRV_COMMON_H

