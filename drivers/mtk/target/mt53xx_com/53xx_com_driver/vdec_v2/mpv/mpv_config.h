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
 *-----------------------------------------------------------------------------
 * $Author: dtvbm11 $
 * $Date: 2012/04/27 $
 * $RCSfile: mpv_config.h,v $
 * $Revision: #1 $
 *---------------------------------------------------------------------------*/

/** @file mpv_config.h
 *  This header file contains configurations of MPV.
 */

#ifndef MPV_CONFIG_H
#define MPV_CONFIG_H


//-----------------------------------------------------------------------------
// Configurations
//-----------------------------------------------------------------------------


// Message Queue Size
#define MPV_MSG_Q_SIZE                   (MPV_MAX_PES_NS_PER_QUEUE + 16)

// Command Queue Size
#define MPV_CMD_Q_SIZE                   20

// Command Size
#define MPV_CMD_SIZE                     4

// Closed caption info queue size
#define MPV_CC_QSIZE                     600         //1800/3

//Error Count Threshold
#define MPV_ERR_THRSD                    10000

//Error Concealment method
#define MPV_EC_METHOD                    2      
    #define MPV_EC_5381_IPB              0
    #define MPV_EC_5381_PB               1   
    #define MPV_EC_1389                  2
    
#define MPV_DTS_INVALID_DRIFT            45000       // 0.5sec
#define MPV_DTS_DRIFT                    9000        // 0.1sec

#define MPV_PTS_INVALID_DRIFT            900000      // 10sec
#define MPV_PTS_DRIFT                    18000

#define MPV_WAIT_DEC_TIME                500    //SW wait decoding time (ms)
#ifdef MPV_SEMI_HOST
#define MPV_WAIT_SEARCH_TIME                2000    //SW wait searching time (ms)
#else
#define MPV_WAIT_SEARCH_TIME                100    //SW wait searching time (ms)
#endif
#define MPV_HW_WAIT_DEC                  0x7B98A0     //HW wait decoding time (bus cycles) => 100ms when BUS_CLK = 81MHz

//SW deblocking
#define MPV_SLICE_QP_THRSD           	 408 // HD, 6 slices each row
#define MPV_QP_INTERVAL                  15

// Detect if reference picture is missing
#define MPV_DETECT_REF_MISSING           0

#ifdef __MODEL_slt__
    #undef MPV_AVSYNC_WAIT_DISP_TIME
    #define MPV_AVSYNC_WAIT_DISP_TIME    0xFFFFFFFF
#endif

#define MPV_WAIT_DISP_UNLOCK_TIME        20         // 40 ms, wait display unlock when change fbg

#ifdef MPV_VIRTUAL_PRS
    #undef MPV_WAIT_DISP_TIME
    #undef MPV_AVSYNC_WAIT_DISP_TIME
    #define MPV_WAIT_DISP_TIME           0xFFFFFFFF
    #define MPV_AVSYNC_WAIT_DISP_TIME    0xFFFFFFFF            
#endif

#define MPV_WAIT_CHGFLD_TIME             600

//for dynamic DBK
#define MPV_DISABLE_DBK_TIME             32000
#define MPV_ENABLE_DBK_TIME              19000    

//for DBK threshold
#define MPV_DBK_QP_THRSD                 9
#define MPV_DBK_QP_THRSD_P               10   
#define MPV_DBK_RATIO_THRSD              45
#define MPV_DBK_RATIO_THRSD_P            55   
#define MPV_DBK_MV_THRSD                2

#define MPV_WAIT_SRAM_THRD    0x1000

// repeat frame case
//#define MPV_REPEAT_TEST
#ifdef MPV_REPEAT_TEST
#define MPV_REPEAT_LOAD_GOLDEN
#define MPV_PARSE_REPEAT_START 9
#define MPV_PARSE_REPEAT_END 9
#define MPV_DECODE_REPEAT_START 7
#define MPV_DECODE_REPEAT_END 7
#endif

// debug with FBM
//#define LOG_MFBM(fmt...) Printf(fmt)
#define LOG_MFBM(fmt...) 

#endif //MPV_CONFIG_H


